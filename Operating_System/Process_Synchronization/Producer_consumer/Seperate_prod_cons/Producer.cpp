#include "common.h"

int main() {
    int shmid = shmget(SHM_KEY, sizeof(Shared), 0666 | IPC_CREAT);
    if (shmid < 0) { perror("shmget"); exit(1); }
    Shared* shm = (Shared*)shmat(shmid, nullptr, 0);

    int semid = semget(SEM_KEY, 3, 0666 | IPC_CREAT);
    if (semid < 0) { perror("semget"); exit(1); }

    // initialize semaphores
    semctl(semid, MUTEX, SETVAL, 1);
    semctl(semid, EMPTY, SETVAL, BUFFER_SIZE);
    semctl(semid, FULL,  SETVAL, 0);

    shm->in = shm->out = 0; 

    for (int item = 1; item <= 20; item++) {
        waitSem(semid, EMPTY);
        waitSem(semid, MUTEX);

        shm->buffer[shm->in] = item;
        std::cout << "Produced: " << item << std::endl;
        shm->in = (shm->in + 1) % BUFFER_SIZE;

        signalSem(semid, MUTEX);
        signalSem(semid, FULL);

        sleep(1);
    }

    shmdt(shm);
    return 0;
}
