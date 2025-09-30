#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstring>
using namespace std;

// Simple handler (sirf signal number)
void simple_handler(int signo) {
    cout << "[Simple Handler] Got signal: " << signo << endl;
}

// Advanced handler (SA_SIGINFO use karke)
void advanced_handler(int signo, siginfo_t *info, void *context) {
    cout << "[Advanced Handler] Got signal: " << signo << endl;
    cout << "  From PID: " << info->si_pid << endl;
    cout << "  From UID: " << info->si_uid << endl;
    cout << "  Reason (si_code): " << info->si_code << endl;
}

int main() {
    cout << "My PID = " << getpid() << endl;

    // 1. Simple handler setup (without SA_SIGINFO)
    struct sigaction sa1{};
    sa1.sa_handler = simple_handler;   // simple function
    sigemptyset(&sa1.sa_mask);
    sa1.sa_flags = 0;                  // no extra info
    sigaction(SIGUSR1, &sa1, nullptr);

    // Send signal to self
    cout <<" \n Sending SIGUSR1 (simple handler)...\n";
    kill(getpid(), SIGUSR1);
    sleep(1);

    // 2. Advanced handler setup (with SA_SIGINFO)
    struct sigaction sa2{};
    sa2.sa_flags = SA_SIGINFO;          // enable advanced mode
    sa2.sa_sigaction = advanced_handler; // advanced handler
    sigemptyset(&sa2.sa_mask);
    sigaction(SIGUSR1, &sa2, nullptr);

    // Send signal to self again
    cout << "\nSending SIGUSR1 (advanced handler)...\n";
    kill(getpid(), SIGUSR1);
    sleep(1);

    cout << "\nDemo finished.\n";
    return 0;
}
