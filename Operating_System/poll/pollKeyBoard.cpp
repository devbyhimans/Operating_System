#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

const int NUM_PROCS = 4;
const int BUF_SIZE = 1024;

int main() {
    FILE* procs[NUM_PROCS];
    pollfd fds[NUM_PROCS + 1];   // +1 for stdin
    char buf[BUF_SIZE];

    const char* execs[NUM_PROCS] = {
        "./p2.exe",
        "./p3.exe",
        "./p4.exe",
        "./p5.exe"
    };

    const char* names[NUM_PROCS] = {
        "p2.exe",
        "p3.exe",
        "p4.exe",
        "p5.exe"
    };

    // Start subprocesses
    for (int i = 0; i < NUM_PROCS; i++) {
        procs[i] = popen(execs[i], "r");
        if (!procs[i]) {
            perror("popen");
            exit(1);
        }
        int fd = fileno(procs[i]);
        fds[i].fd = fd;
        fds[i].events = POLLIN;
    }

    // Add stdin (keyboard) to poll list
    fds[NUM_PROCS].fd = STDIN_FILENO;
    fds[NUM_PROCS].events = POLLIN;

    int active = NUM_PROCS;

    while (active > 0 || true) {  // keep reading stdin forever
        int ret = poll(fds, NUM_PROCS + 1, -1);
        if (ret == -1) {
            perror("poll");
            exit(1);
        }

        // Check child processes
        for (int i = 0; i < NUM_PROCS; i++) {
            if (fds[i].fd == -1) continue;

            if (fds[i].revents & POLLIN) {
                if (fgets(buf, BUF_SIZE, procs[i]) != nullptr) {
                    cout << "P1 got from " << names[i] << ": " << buf;
                } else {
                    pclose(procs[i]);
                    fds[i].fd = -1;
                    active--;
                }
            }
        }

        // Check keyboard input
        if (fds[NUM_PROCS].revents & POLLIN) {
            if (fgets(buf, BUF_SIZE, stdin) != nullptr) {
                cout << "P1 got from keyboard: " << buf;
                // Example: quit if user types "exit"
                buf[strcspn(buf, "\n")] = 0; // strip newline
                if (strcmp(buf, "exit") == 0) {
                    cout << "Exit command received. Quitting...\n";
                    break;
                }
            } else {
                cout << "Keyboard closed. Exiting.\n";
                break;
            }
        }
    }

   // Cleanup
    for (int i = 0; i < NUM_PROCS; i++) {
        if (fds[i].fd != -1) {
            pclose(procs[i]);
        }
    }

    return 0;
}
