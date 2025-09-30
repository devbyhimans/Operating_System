// two_way_reader.cpp
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
    const char* fifo1 = "mkfile1";
    const char* fifo2 = "mkfile2";

    int fd_read  = open(fifo1, O_RDONLY);
    int fd_write = open(fifo2, O_WRONLY);

    char buffer[100];
    string reply;

    while (true) {
        read(fd_read, buffer, sizeof(buffer));
        cout << "Writer said: " << buffer << endl;

        if (string(buffer) == "exit") break;

        cout << "Reader > ";
        getline(cin, reply);
        write(fd_write, reply.c_str(), reply.size() + 1);
    }

    close(fd_read);
    close(fd_write);
    return 0;
}
