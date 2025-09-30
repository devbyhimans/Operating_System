// two_way_writer.cpp

// Writer sends a message → reader reads.

// Reader replies → writer reads.

// Continues until someone types exit.

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
using namespace std;

int main() {
    const char* fifo1 = "mkfile1"; // writer → reader
    const char* fifo2 = "mkfile2"; // reader → writer

    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    int fd_write = open(fifo1, O_WRONLY);
    int fd_read  = open(fifo2, O_RDONLY);

    string input;
    char buffer[100];

    while (true) {
        cout << "Writer > ";
        getline(cin, input);
        write(fd_write, input.c_str(), input.size() + 1);

        if (input == "exit") break;

        // Wait for reader response
        read(fd_read, buffer, sizeof(buffer));
        cout << "Reader replied: " << buffer << endl;
    }

    close(fd_write);
    close(fd_read);
    unlink(fifo1);
    unlink(fifo2);
    return 0;
}

