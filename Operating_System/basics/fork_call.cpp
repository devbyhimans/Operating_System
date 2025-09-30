#include <iostream>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed\n";
        return 1;
    } else if (pid == 0) {
        std::cout << "Child Process: PID = " << getpid() << ", Parent PID = " << getppid() << "\n";
    } else {
        std::cout << "Parent Process: PID = " << getpid() << ", Child PID = " << pid << "\n";
    }

    return 0;
}
