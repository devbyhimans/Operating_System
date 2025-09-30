#include <iostream>
#include <unistd.h> // for sleep()

using namespace std;

int main() {
        cout << "Hello from P5, message " << 1 << endl;
        cout.flush();   // make sure it flushes immediately
        sleep(1);       // wait 1 second
    return 0;
}
