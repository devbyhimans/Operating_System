
//Dining Philosophers Problem (Executable Code)
#include <iostream>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

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

// Function to simulate a philosopher thinking and eating
void philosopher(int id, int semids[5]) {
   for(int i=0;i<5;i++) {
    cout << "Philosopher " << id << " is thinking.\n";
    usleep(rand() % 1000000); // Thinking time

    // Deadlock avoidance: Philosopher 4 picks up right first
    if (id == 4) {
        // Pick up right chopstick first
        sem_wait(semids[(id + 1) % 5]);
        sem_wait(semids[id]);
    } else {
        // Pick up left chopstick first
        sem_wait(semids[id]);
        sem_wait(semids[(id + 1) % 5]);
    }

    // Eating
    cout << "Philosopher " << id << " is eating.\n";
    usleep(rand() % 1000000); // Eating time

    // Put down right chopstick
    sem_signal(semids[(id + 1) % 5]);

    // Put down left chopstick
    sem_signal(semids[id]);

    cout << "Philosopher " << id << " is done eating and back to thinking.\n";
}

}

int main() {
    key_t sem_key = ftok("semfile", 75);  // Generate a unique key for semaphores

    // Step 1: Create semaphores for the chopsticks
    int semids[5];  // Array to hold the semaphore IDs for each chopstick
    for (int i = 0; i < 5; i++) {
        semids[i] = semget(sem_key + i, 1, IPC_CREAT | 0666);
        if (semids[i] == -1) {
            perror("semget failed");
            exit(1);
        }

        // Initialize each semaphore to 1 (indicating chopstick is available)
        semun arg;
        arg.val = 1;
        if (semctl(semids[i], 0, SETVAL, arg) == -1) {
            perror("semctl failed");
            exit(1);
        }
    }

    // Step 2: Fork the philosopher processes
    pid_t pids[5];  // Array to store the process IDs of philosophers
    for (int i = 0; i < 5; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {  // Child process (philosopher)
            philosopher(i, semids);  // Philosopher starts eating and thinking
            exit(0);  // Exit the philosopher process
          /*exit make the children of the child to exit the program 
          and only 1 parent+5child are there , if no exit then there will be 
          2^5 processes*/  
        }
    }


    // Parent process waits for all child processes to finish (never happens in this case)
    for (int i = 0; i < 5; i++) {
        waitpid(pids[i], NULL, 0);  // Wait for all philosophers to finish
    }

    // Step 3: Clean up the semaphores
    for (int i = 0; i < 5; i++) {
        semctl(semids[i], 0, IPC_RMID, 0);  // Remove each semaphore
    }

    return 0;
}
