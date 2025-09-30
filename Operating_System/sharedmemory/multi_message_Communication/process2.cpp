// Receiver -- P2 from P1

#include "common.h"
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
using namespace std;

int main() {
    key_t shm_key = ftok("mqfile", SHM_KEY_ID);
    int shmid = shmget(shm_key, sizeof(shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    auto *shm = (shared_memory*) shmat(shmid, nullptr, 0);
    if (shm == (void*) -1) {
        perror("shmat failed");
        return 1;
    }

    key_t sem_key = ftok("mqfile", SEM_KEY_ID);
    int semid = semget(sem_key, SEM_NUM, 0666);
    if (semid == -1) {
        perror("semget failed");
        return 1;
    }

    sembuf lock{SEM_MUTEX12, -1, 0};
    sembuf unlock{SEM_MUTEX12, 1, 0};

    while (true) {
        semop(semid, &lock, 1);

        if (shm->flag12 == 1) {
            if (strcmp(shm->buffer12, "END") == 0) {
                semop(semid, &unlock, 1);
                cout << "[Process2] Communication ended." << endl;
                break;
            }
            cout << "[Process2] Received: " << shm->buffer12 << endl;
            shm->flag12 = 0;  // reset so sender can write again
        }

        semop(semid, &unlock, 1);
        usleep(100000);  // avoid CPU 100% usage
    }

    shmdt(shm);
    return 0;
}
