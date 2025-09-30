#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid1, pid2;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // First child: echo "Hello, world!"
    if ((pid1 = fork()) == 0) {
        close(pipe1[0]);               // close read end of pipe1
        dup2(pipe1[1], 1); // stdout -> pipe1
        close(pipe1[1]);

        execlp("echo", "echo", "Hello, world!", NULL);
        perror("execlp echo failed");
        exit(1);
    }

    // Second child: grep "Hello"
    if ((pid2 = fork()) == 0) {
        close(pipe1[1]);               // close write end of pipe1
        dup2(pipe1[0], 0);  // stdin <- pipe1
        close(pipe1[0]);

        close(pipe2[0]);               // close read end of pipe2
        dup2(pipe2[1], 1); // stdout -> pipe2
        close(pipe2[1]);

        execlp("grep", "grep", "Hello", NULL);
        perror("execlp grep failed");
        exit(1);
    }

    // Parent process: close unused ends
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[1]); // we will read from pipe2

    // Read from pipe2 and count newlines
    char buffer[1024];
    ssize_t bytes_read;
    int line_count = 0;

    while ((bytes_read = read(pipe2[0], buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                line_count++;
            }
        }
    }

    printf("Total lines: %d\n", line_count);

    close(pipe2[0]);

    // Wait for children
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
