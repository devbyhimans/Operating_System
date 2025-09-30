#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
using namespace std;

int main() {
    // Create FIFOs if they don't exist
    mkfifo("fifo1", 0666); // B reads from here
    mkfifo("fifo2", 0666); // B writes here

    char buffer[100];

    int read_fd = open("fifo1", O_RDONLY);   // Reading from A
    int write_fd = open("fifo2", O_WRONLY);  // Writing to A

    if (read_fd < 0 || write_fd < 0) {
        perror("open");
        return 1;
    }

    cout << "Chat started (User B). Type 'exit' to quit.\n";

    while (true) {
        // Read message from A
        ssize_t bytesRead = read(read_fd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            cout << "Friend: " << buffer << endl;
        } else if (bytesRead == 0) {
            cout << "Friend disconnected.\n";
            break;
        } else {
            perror("read");
            break;
        }

        // Send message to A
        cout << "Me: ";
        string message;
        getline(cin, message);
        message[strcspn(message, "\n")] = 0;
        if (strcmp(message.c_str(),"exit")) {
            break;
        }

        write(write_fd, message.c_str(), message.length());
    }

    close(read_fd);
    close(write_fd);
    return 0;
}
