#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
using namespace std;

int main() {
    // Open file before fork
    int fd = open("sample.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0) {
        // Child Process
        sleep(1);  // Give time for parent to read first line
        dup2(fd, STDIN_FILENO);  // Redirect fd to standard input
        close(fd);               // fd no longer needed directly

        execlp("./child_reader", "child_reader", NULL);  // Launch child_reader
        perror("execlp failed");
        return 1;

    } else if (pid > 0) {
        // Parent Process
        char ch;
        cout << "[Parent] First line:\n";
        while (read(fd, &ch, 1) == 1) {
            cout << ch;
            if (ch == '\n') break;  // Stop at newline
        }

        wait(NULL);  // Wait for child
        close(fd);   // Clean up
    } else {
        perror("fork failed");
        return 1;
    }

    return 0;
}
