/*🔹 Flow of Execution

Run P2 first →
Creates shared memory.
Writes its PID.
Waits for signal.

Run P1 →
Reads P2’s PID from shared memory.
Sends SIGUSR1 using kill().
P2 receives the signal →
Handler executes.
Prints: "Received signal 10 from PID: (P1’s PID)".

Exits.*/
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>

using namespace std;

#define SHM_KEY 0x1234

int main() {
    // Access existing shared memory (created by P2)
    int shmid = shmget(SHM_KEY, sizeof(pid_t), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach shared memory
    pid_t *shm_ptr = (pid_t *) shmat(shmid, nullptr, 0);
    if (shm_ptr == (pid_t *) -1) {
        perror("shmat");
        exit(1);
    }

    // Read PID of P2 from shared memory
    pid_t p2_pid = *shm_ptr;
    cout << "P1: Read P2 PID: " << p2_pid << " from shared memory." << endl;

    // Send SIGUSR1 signal to P2
    if (kill(p2_pid, SIGUSR1) == -1) {
        perror("kill");
        exit(1);
    }
    cout<<"My id is :"<<getpid()<<endl;

    cout << "P1: Sent SIGUSR1 to P2 (PID " << p2_pid << ")" << endl;

    // Detach shared memory
    shmdt(shm_ptr);

    return 0;
}
