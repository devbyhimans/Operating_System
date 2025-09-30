// What happens here?

// We create 4 threads one by one
// After creating each thread, the main thread immediately waits (pthread_join) for that thread to finish.
// So, the main thread will not proceed to create the next thread until the current one finishes.
// The child thread sleeps for 1 second, prints a message, and then ends.
// The main thread prints its own message, waits for the child thread, and repeats.

// Result:
// Threads run sequentially — main waits for each child thread to complete before moving on.

// Why use join?
// To ensure the main thread waits for child threads to finish before proceeding or terminating.

#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;

void* fun(void*) {
    sleep(1);
    cout << "Created child thread" << endl;
    return NULL;
}

int main() {
    
    pthread_t t[4];
    cout << "CSE NITW Multithreading!" << endl << endl;
  
    for(int i = 0; i < 4; i++) {
        cout << "Created -> Thread T[" << i << "]!" << endl;
        pthread_create(&t[i], NULL, fun, NULL);
        pthread_join(t[i], NULL);  // Wait for the thread to finish
    }
    return 0;
}

