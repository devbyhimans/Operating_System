#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

// Shared memory structure
struct SharedMemory {
    int x;
    int y;
};

// Semaphore union for IPC
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Function to perform wait operation on semaphore
void sem_wait(int semid) {
    struct sembuf sb = {0, -1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop failed (wait)");
        exit(1);
    }
}

// Function to perform signal operation on semaphore
void sem_signal(int semid) {
    struct sembuf sb = {0, 1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop failed (signal)");
        exit(1);
    }
}

int main() {
    key_t shm_key = ftok("shmfile", 65);  // Generate a unique key for shared memory
    key_t sem_key = ftok("semfile", 75);  // Generate a unique key for semaphore

    // Step 1: Create shared memory segment
    int shm_id = shmget(shm_key, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }

    // Attach the shared memory segment to process' address space
    SharedMemory* shm = (SharedMemory*) shmat(shm_id, nullptr, 0);
    if (shm == (SharedMemory*)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Step 2: Get semaphore ID
    int semid = semget(sem_key, 1, 0666);
    if (semid == -1) {
        perror("semget failed");
        exit(1);
    }

    while (true) {
        sem_wait(semid);  // Wait for P2 to proceed

        // Critical section for P2
        shm->y = shm->x + 1;  // P2 computes y = x + 1
        cout << "P2: x = " << shm->x << " -> y = " << shm->y << endl;

        sem_signal(semid);  // Signal P1 to proceed
        usleep(500000);     // Simulate delay
    }

    // Detach shared memory
    shmdt(shm);

    return 0;
}
