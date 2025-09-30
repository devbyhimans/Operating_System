#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>

using namespace std;

#define SHM_KEY 0x1234

int signal_count = 0;
bool done = true;  // flag to exit loop

void handler(int sig, siginfo_t *si, void *context) {
    signal_count++;
    cout << "P2: Received signal " << sig 
         << " from PID: " << si->si_pid 
         << " (Count = " << signal_count << ")" << endl;

    if (signal_count !=0) {
        done = false;  // set flag instead of exit()
    }
}


int main() {
    // Create shared memory segment
    int shmid = shmget(SHM_KEY, sizeof(pid_t), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory
    pid_t *shm_ptr = (pid_t *) shmat(shmid, nullptr, 0);
    if (shm_ptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // Write own PID to shared memory
    *shm_ptr = getpid();
    cout << "P2: PID " << *shm_ptr << " written to shared memory." << endl;

    // Setup sigaction
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;          // Use siginfo_t
    sa.sa_sigaction = handler;         // Set handler function
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, nullptr) == -1) {
        perror("sigaction");
        exit(1);
    }

    cout << "P2: Waiting for signal..." << endl;

    // Wait for signals forever
    cout<<"My id is :"<<getpid()<<endl;

    while (done) {
        pause();  // Sleep until signal is caught
    }


    cout<<"BYE....Exiting"<<endl;
    // Cleanup (not reached in this program)--earlier but after implementing done cleanup is reached.
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
