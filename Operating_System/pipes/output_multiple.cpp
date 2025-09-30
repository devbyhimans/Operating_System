// Redirecting the output of a process to multiple files or file 
// descriptors

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    // Open output files
    int fd1 = open("output1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd2 = open("output2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd1 == -1 || fd2 == -1) {
        perror("Failed to open files");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process: redirect stdout and stderr
        dup2(fd1, STDOUT_FILENO); // stdout → output1.txt
        dup2(fd2, STDERR_FILENO); // stderr → output2.txt
        close(fd1);
        close(fd2);

        // Test output
        std::cout << "This will be written to output1.txt (stdout)" << std::endl;
        std::cerr << "This will be written to output2.txt (stderr)" << std::endl;

        return 0;
    }

    // Parent process
    wait(nullptr);

    close(fd1);
    close(fd2);

    return 0;
}
