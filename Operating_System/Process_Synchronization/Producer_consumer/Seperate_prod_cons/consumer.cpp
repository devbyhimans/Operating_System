#include "common.h"

int main() {
    int shmid = shmget(SHM_KEY, sizeof(Shared), 0666);
    if (shmid < 0) { perror("shmget"); exit(1); }
    Shared* shm = (Shared*)shmat(shmid, nullptr, 0);

    int semid = semget(SEM_KEY, 3, 0666);
    if (semid < 0) { perror("semget"); exit(1); }

    for (int i = 1; i <= 20; i++) {
        waitSem(semid, FULL);
        waitSem(semid, MUTEX);

        int item = shm->buffer[shm->out];
        std::cout << "Consumed: " << item << std::endl;
        shm->out = (shm->out + 1) % BUFFER_SIZE;

        signalSem(semid, MUTEX);
        signalSem(semid, EMPTY);

        sleep(2);
    }

    shmdt(shm);
    shmctl(shmid, IPC_RMID, nullptr);
    semctl(semid, 0, IPC_RMID);
    return 0;
}
