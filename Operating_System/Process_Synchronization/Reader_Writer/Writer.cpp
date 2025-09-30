#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_SIZE 1024  // Shared memory size
#define NUM_WRITES 10  // Number of writes

// Shared memory structure
struct shared_data {
    int shared_resource;  // Shared resource (e.g., integer)
    int reader_count;     // Count of readers currently accessing the resource
};

// Semaphore IDs
int sem_id;
#define SEM_MUTEX  0  // Mutex for protecting reader_count
#define SEM_WRITE   1  // Semaphore to control access for writers

// Union for semaphore operations
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Semaphore wait operation
void sem_wait(int sem_num) {
    struct sembuf sop;
    sop.sem_num = sem_num;
    sop.sem_op = -1;  // Decrement semaphore (wait)
    sop.sem_flg = 0;
    semop(sem_id, &sop, 1);
}

// Semaphore signal operation
void sem_signal(int sem_num) {
    struct sembuf sop;
    sop.sem_num = sem_num;
    sop.sem_op = 1;  // Increment semaphore (signal)
    sop.sem_flg = 0;
    semop(sem_id, &sop, 1);
}

int main() {
    key_t shm_key = ftok("Book", 'A');
    key_t sem_key = ftok("Book", 'B');

    // Create shared memory segment
    int shm_id = shmget(shm_key, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Shared memory creation failed");
        exit(1);
    }

    // Attach shared memory segment
    struct shared_data* data = (struct shared_data*)shmat(shm_id, NULL, 0);
    if (data == (struct shared_data*)-1) {
        perror("Shared memory attachment failed");
        exit(1);
    }
    // Create semaphore set
    sem_id = semget(sem_key, 2, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("Semaphore creation failed");
        exit(1);
    }
    // Initialize semaphores
    union semun sem_union;
    sem_union.val = 1;
    semctl(sem_id, SEM_MUTEX, SETVAL, sem_union);  // Initialize mutex
    sem_union.val = 1;
    semctl(sem_id, SEM_WRITE, SETVAL, sem_union);  // Initialize write lock

    // Writer loop
    for (int i = 0; i < NUM_WRITES; i++) {
        // Wait for write lock (exclusive access)
        sem_wait(SEM_WRITE);

        // Write to the shared resource
        data->shared_resource++;
        printf("Writer: Writing shared_resource = %d\n", data->shared_resource);
        sleep(2);  // Simulate writing delay

        // Signal the write lock (allow other writers)
        sem_signal(SEM_WRITE);

        sleep(2);  // Simulate delay before next write
    }

    // Cleanup: Detach shared memory
    shmdt(data);
   

    return 0;
}
