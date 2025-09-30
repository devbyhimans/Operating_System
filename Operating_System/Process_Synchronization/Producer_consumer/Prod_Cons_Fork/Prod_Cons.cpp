#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS   20

// Shared buffer and indices
int *buffer;
int *in, *out;

// Semaphore IDs
int sem_id;
#define SEM_MUTEX  0
#define SEM_EMPTY  1
#define SEM_FULL   2

// Union for semaphores (for controlling multiple semaphores)
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Semaphore operations
void sem_wait(int sem_num) {
    struct sembuf sop;
    sop.sem_num = sem_num;
    sop.sem_op = -1; // Decrement semaphore
    sop.sem_flg = 0;
    semop(sem_id, &sop, 1);
}

void sem_signal(int sem_num) {
    struct sembuf sop;
    sop.sem_num = sem_num;
    sop.sem_op = 1;  // Increment semaphore
    sop.sem_flg = 0;
    semop(sem_id, &sop, 1);
}



void producer() {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        sem_wait(SEM_EMPTY);    // Wait for space in buffer
        sem_wait(SEM_MUTEX);    // Lock the buffer (mutual exclusion)

        // Produce an item
        *buffer = i;
        printf("Produced: %d\n", *buffer);

        // Update the "in" index (circular buffer)
        *in = (*in + 1) % BUFFER_SIZE;

        sem_signal(SEM_MUTEX);  // Unlock the buffer
        sem_signal(SEM_FULL);   // Signal consumer that an item is available

        usleep(rand() % 100000); // Simulate production delay
    }
}

void consumer() {
    for (int i = 1; i <= NUM_ITEMS; i++) {
        sem_wait(SEM_FULL);     // Wait for an item in the buffer
        sem_wait(SEM_MUTEX);    // Lock the buffer (mutual exclusion)

        // Consume the item
        int item = *buffer;
        printf("Consumed: %d\n", item);

        // Update the "out" index (circular buffer)
        *out = (*out + 1) % BUFFER_SIZE;

        sem_signal(SEM_MUTEX);  // Unlock the buffer
        sem_signal(SEM_EMPTY);  // Signal producer that space is available

        usleep(rand() % 150000); // Simulate consumption delay
    }
}

int main() {
    // Key for shared memory and semaphores
    key_t shm_key = ftok("producer_consumer.c", 'R');
    key_t sem_key = ftok("producer_consumer.c", 'S');

    // Create shared memory segment for buffer and indices
    int shm_id = shmget(shm_key, sizeof(int) * (BUFFER_SIZE + 2), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Shared memory creation failed");
        exit(1);
    }

    // Attach shared memory to the process
    buffer = (int *)shmat(shm_id, NULL, 0);
    if (buffer == (int *)-1) {
        perror("Shared memory attachment failed");
        exit(1);
    }

    // Initialize shared memory
    in = &buffer[0];
    out = &buffer[1];
    *in = 0;
    *out = 0;


    // Create the semaphore set
    sem_id = semget(sem_key, 3, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("Semaphore creation failed");
        exit(1);
    }

    // Initialize semaphores
    union semun sem_union;
    sem_union.val = 1;  // SEM_MUTEX starts as 1 (unlocked)
    semctl(sem_id, SEM_MUTEX, SETVAL, sem_union);

    sem_union.val = BUFFER_SIZE;  // SEM_EMPTY starts with buffer size
    semctl(sem_id, SEM_EMPTY, SETVAL, sem_union);

    sem_union.val = 0;  // SEM_FULL starts as 0 (no items produced)
    semctl(sem_id, SEM_FULL, SETVAL, sem_union);

    // Create producer and consumer processes
    pid_t pid = fork();

    if (pid == 0) {
        // Consumer process
        consumer();
    } else if (pid > 0) {
        // Parent process is the producer
        producer();

        // Wait for the child (consumer) to finish
        wait(NULL);

        // Cleanup shared memory and semaphores
        shmctl(shm_id, IPC_RMID, NULL);
        semctl(sem_id, 0, IPC_RMID, sem_union);
    } else {
        perror("Fork failed");
        exit(1);
    }

    return 0;
}
