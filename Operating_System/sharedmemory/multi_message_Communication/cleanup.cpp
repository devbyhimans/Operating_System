//Cleanup-->clearing the shared memory

#include "common.h"

int main() {
    key_t shm_key = ftok("mqfile", SHM_KEY_ID);
    int shmid = shmget(shm_key, sizeof(shared_memory), 0666);
    shmctl(shmid, IPC_RMID, nullptr);

    key_t sem_key = ftok("mqfile", SEM_KEY_ID);
    int semid = semget(sem_key, SEM_NUM, 0666);
    semctl(semid, 0, IPC_RMID);

    cout << "Shared memory and semaphores removed." << endl;
    return 0;
}
