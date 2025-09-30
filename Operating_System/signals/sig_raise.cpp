// send_self.cpp
#include <csignal>
#include <unistd.h>
#include <iostream>
using namespace std;

int main() {
    cout << "PID: " << getpid() << "\n";
    cout << "raising SIGINT (default action = terminate)\n";
    raise(SIGINT);                  // same as kill(getpid(), SIGINT)
    cout << "you won't see this line\n"; // never runs
}

