#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>

using namespace std;

#define SHM_KEY 1234

void handler(int sig) {
    cout << "P2: Received signal " << sig << " from P1" << endl;
    exit(0);  // Exit after receiving signal
}

int main() {
    // Create shared memory
    int shmid = shmget(SHM_KEY, sizeof(pid_t), IPC_CREAT | 0666);
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

    // Write PID
    *shm_ptr = getpid();
    cout << "P2: PID " << *shm_ptr << " written to shared memory." << endl;

    // Install signal handler
    signal(SIGUSR1, handler);

    cout << "P2: Waiting for signal..." << endl;
    while (true) {
        pause(); // Wait for signal
    }

    return 0;
}
