#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 20
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3

#define SEM_MUTEX 0
#define SEM_EMPTY 1
#define SEM_FULL 2

using namespace std;

// Semaphore union
union semun {
    int val;
    struct semid_ds* buf;
    unsigned short* array;
};

// Shared memory pointers
int* in;
int* out;
int* buffer;
int sem_id;

// Semaphore wait
void sem_wait(int sem_num) {
    struct sembuf sop;
    sop.sem_num = sem_num;  // short, but implicit narrowing is fine in C++
    sop.sem_op = -1;
    sop.sem_flg = 0;

    if (semop(sem_id, &sop, 1) == -1) {
        perror("sem_wait failed");
        exit(1);
    }
}

// Semaphore signal
void sem_signal(int sem_num) {
    struct sembuf sop;
    sop.sem_num = sem_num;
    sop.sem_op = 1;
    sop.sem_flg = 0;

    if (semop(sem_id, &sop, 1) == -1) {
        perror("sem_signal failed");
        exit(1);
    }
}

// Producer
void producer(int id) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        sem_wait(SEM_EMPTY);
        sem_wait(SEM_MUTEX);

        int item = id * 100 + i;
        buffer[*in] = item;
        cout << "Producer " << id << ": Produced " << item << " at " << *in << endl;
        *in = (*in + 1) % BUFFER_SIZE;

        sem_signal(SEM_MUTEX);
        sem_signal(SEM_FULL);

        usleep(rand() % 100000);
    }
}

// Consumer
void consumer(int id) {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        sem_wait(SEM_FULL);
        sem_wait(SEM_MUTEX);

        int item = buffer[*out];
        cout << "Consumer " << id << ": Consumed " << item << " at " << *out << endl;
        *out = (*out + 1) % BUFFER_SIZE;

        sem_signal(SEM_MUTEX);
        sem_signal(SEM_EMPTY);

        usleep(rand() % 150000);
    }
}

int main() {
    srand(time(NULL));

    key_t shm_key = ftok("producer_consumer.c", 'R');
    key_t sem_key = ftok("producer_consumer.c", 'S');

    // Allocate shared memory
    int shm_id = shmget(shm_key, sizeof(int) * (2 + BUFFER_SIZE), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }

    int* shm_ptr = (int*)shmat(shm_id, NULL, 0);
    if (shm_ptr == (int*)-1) {
        perror("shmat failed");
        exit(1);
    }

    in = &shm_ptr[0];
    out = &shm_ptr[1];
    buffer = &shm_ptr[2];

    *in = 0;
    *out = 0;

    // Create semaphores
    sem_id = semget(sem_key, 3, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }

    union semun sem_union;
    sem_union.val = 1;
    semctl(sem_id, SEM_MUTEX, SETVAL, sem_union);

    sem_union.val = BUFFER_SIZE;
    semctl(sem_id, SEM_EMPTY, SETVAL, sem_union);

    sem_union.val = 0;
    semctl(sem_id, SEM_FULL, SETVAL, sem_union);

    // Fork producers
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            producer(i + 1);
            shmdt(shm_ptr);
            exit(0);
        }
    }

    // Fork consumers
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            consumer(i + 1);
            shmdt(shm_ptr);
            exit(0);
        }
    }

    // Wait for all children
    for (int i = 0; i < NUM_PRODUCERS + NUM_CONSUMERS; i++) {
        wait(NULL);
    }

    // Cleanup
    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    cout << "All processes finished. Shared memory and semaphores cleaned up." << endl;
    return 0;
}
