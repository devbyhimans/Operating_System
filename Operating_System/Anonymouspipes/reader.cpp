#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

using namespace std;

int main() {
    const char* fifo_path = "mkfile";
    mkfifo(fifo_path, 0666);

    // Open FIFO for reading
    int fifo_fd = open(fifo_path, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open error");
        return 1;
    }

    char buffer[1024];
    while (true) {
        ssize_t bytes = read(fifo_fd, buffer, sizeof(buffer));
        if (bytes > 0) {
            cout << "Received: " << buffer << endl;
            if (string(buffer) == "exit")
                break;
        }
    }

    close(fifo_fd);
    unlink("mkfile");
    return 0;
}
