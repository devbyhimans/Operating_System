#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
    char ch;
    cout << "[Child] Remaining lines:\n";
    while (read(STDIN_FILENO, &ch, 1) == 1) {
        cout << ch;
    }
    return 0;
}
