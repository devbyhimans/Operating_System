// sender -- P1 -> P2

#include "common.h"
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main() {
    key_t shm_key = ftok("mqfile", SHM_KEY_ID);
    int shmid = shmget(shm_key, sizeof(shared_memory), 0666 | IPC_CREAT);
    auto *shm = (shared_memory*) shmat(shmid, nullptr, 0);

    key_t sem_key = ftok("mqfile", SEM_KEY_ID);
    int semid = semget(sem_key, SEM_NUM, 0666 | IPC_CREAT);

    if (semid == -1) {
        perror("semget failed");
        return 1;
    }

    // Initialize semaphore only once (set to 1)
    semctl(semid, SEM_MUTEX12, SETVAL, 1);

    sembuf lock{SEM_MUTEX12, -1, 0};
    sembuf unlock{SEM_MUTEX12, 1, 0};

    for (int i = 0; i < 5; i++) {
        semop(semid, &lock, 1);

        snprintf(shm->buffer12, MAX_MSG_SIZE, "Message %d from Process1", i + 1);
        shm->flag12 = 1;

        semop(semid, &unlock, 1);
        cout << "[Process1] Sent: " << shm->buffer12 << endl;
        sleep(1);
    }

    semop(semid, &lock, 1);
    snprintf(shm->buffer12, MAX_MSG_SIZE, "END");
    shm->flag12 = 1;
    semop(semid, &unlock, 1);

    shmdt(shm);
    return 0;
}
