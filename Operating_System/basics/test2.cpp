#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main() {
    // Create FIFOs if they don't exist
    mkfifo("fifo1", 0666); // B reads from here
    mkfifo("fifo2", 0666); // B writes here

    char buffer[100];

    int read_fd = open("fifo2", O_RDONLY);
    int write_fd = open("fifo1", O_WRONLY);

    cout << "Chat started (User B). Type 'exit' to quit.\n";

    while (true) {
        // Read message from A
        ssize_t bytesRead = read(read_fd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            cout << "Friend: " << buffer << endl;
        }

        // Send message to A
        // cout << "Me: ";
        // string message;
        // getline(cin, message);

        // if (message == "exit") {
        //     break;
        // }

        // write(write_fd, message.c_str(), message.length());
    }

    close(read_fd);
    close(write_fd);
    return 0;
}
