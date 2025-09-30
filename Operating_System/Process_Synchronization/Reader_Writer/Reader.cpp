#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHM_SIZE 1024  // Shared memory size
#define NUM_READS 10   // Number of reads

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
    int shm_id = shmget(shm_key, sizeof(struct shared_data), 0666);
    if (shm_id == -1) {
        perror("Shared memory access failed");
        exit(1);
    }

    // Attach shared memory segment
    struct shared_data* data = (struct shared_data*)shmat(shm_id, NULL, 0);
    if (data == (struct shared_data*)-1) {
        perror("Shared memory attachment failed");
        exit(1);
    }

    // Get semaphore set
    sem_id = semget(sem_key, 2, 0666);
    if (sem_id == -1) {
        perror("Semaphore access failed");
        exit(1);
    }

    // Reader loop
    for (int i = 0; i < NUM_READS; i++) {
        // Wait for mutex to safely modify reader_count
        sem_wait(SEM_MUTEX);
        data->reader_count++;
        if (data->reader_count == 1) {
            // If this is the first reader, acquire the write lock
            sem_wait(SEM_WRITE);
        }
        sem_signal(SEM_MUTEX);

        // Read the shared resource
        printf("Reader: Reading shared_resource = %d\n", data->shared_resource);
        sleep(3);  // Simulate reading delay

        // After reading, release the mutex and possibly release the write lock
        sem_wait(SEM_MUTEX);
        printf("Reader: Reading shared_resource = %d\n", data->reader_count);

        data->reader_count--;
        if (data->reader_count == 0) {
            // If this is the last reader, release the write lock
            sem_signal(SEM_WRITE);
        }
        sem_signal(SEM_MUTEX);

        usleep(3);  // Simulate delay before next read
    }

    // Cleanup: Detach shared memory
    
    shmdt(data);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}    
