#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
using namespace std;

// Handler for SIGUSR1
void handle_sigusr1(int signo) {
    cout << "[Handler] Caught SIGUSR1 (" << signo << "): Doing task A" << endl;
}

// Handler for SIGUSR2
void handle_sigusr2(int signo) {
    cout << "[Handler] Caught SIGUSR2 (" << signo << "): Doing task B" << endl;
}

// Handler for SIGINT (Ctrl+C)
void handle_sigint(int signo) {
    cout << "[Handler] Caught SIGINT (" << signo << "): Exiting program safely." << endl;
    exit(0);   // graceful exit
}

int main() {
    cout << "My PID = " << getpid() << endl;
    cout << "Send signals using: kill -USR1 " << getpid()
         << " | kill -USR2 " << getpid()
         << " | Ctrl+C for SIGINT" << endl;

    // Setup handlers
    struct sigaction sa1{}, sa2{}, sa3{};

    // SIGUSR1
    sa1.sa_handler = handle_sigusr1;
    sigemptyset(&sa1.sa_mask);
    sa1.sa_flags = 0;
    sigaction(SIGUSR1, &sa1, nullptr);

    // SIGUSR2
    sa2.sa_handler = handle_sigusr2;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;
    sigaction(SIGUSR2, &sa2, nullptr);

    // SIGINT (Ctrl+C)
    sa3.sa_handler = handle_sigint;   
    sigemptyset(&sa3.sa_mask);
    sa3.sa_flags = 0;
    sigaction(SIGINT, &sa3, nullptr);

    // Infinite loop waiting for signals
    while (true) {
        cout << "Waiting for signals... (PID: " << getpid() << ")\n";
        pause();  // Sleep until a signal arrives
    }

    return 0;
}
