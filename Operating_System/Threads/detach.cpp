// What happens here?

// We create 4 threads, but do not wait for them.
// After creating each thread, we detach it, telling the OS:
// "This thread runs independently."
// "You can reclaim its resources automatically when done."
// The main thread prints creation messages and continues.
// Since threads run detached, main does not wait for them.
// We add a sleep(2) at the end so the program stays alive long enough for child threads to run and print their messages.

// Result:
// Threads run concurrently and independently of the main thread. Main thread doesn’t wait but must stay alive long enough for children to finish.
 
// Why use detach?
// When you don’t need to synchronize or collect results from child threads, and want them to run freely.

#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;

void* fun(void*) {
    sleep(1);
    cout << "Created Child thread!" << endl;
    return NULL;
}

int main() {
    pthread_t t[4];
    for(int i = 0; i < 4; i++) {
        cout << "Created -> Thread T[" << i << "]!" << endl;
        pthread_create(&t[i], NULL, fun, NULL);
        pthread_detach(t[i]);  // Detach the thread
    }
    sleep(2); // Give time for detached threads to complete before main exits
    return 0;
}
