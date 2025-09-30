#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>

using namespace std;

#define SHM_KEY 1234

int main() {
    // Access shared memory
    int shmid = shmget(SHM_KEY, sizeof(pid_t), 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    // Attach
    pid_t* shm_ptr = (pid_t*) shmat(shmid, nullptr, 0);
    if (shm_ptr == (void*) -1) {
        perror("shmat");
        return 1;
    }

    // Read PID
    pid_t p2_pid = *shm_ptr;
    cout << "P1: Read P2 PID: " << p2_pid << " from shared memory." << endl;

    // Send signal
    if (kill(p2_pid, SIGUSR1) == -1) {
        perror("kill");
        return 1;
    }

    cout << "P1: Sent SIGUSR1 to P2 (PID " << p2_pid << ")" << endl;

    // Detach
    shmdt(shm_ptr);

    return 0;
}
