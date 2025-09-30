#include <iostream>
#include <pthread.h>
using namespace std;

const char* st = "Child thread";

void* fun(void* arg) {
    // Cast the argument back to char*
    cout << "Created child thread: " << (char*)arg << endl;
    return nullptr;  // pthread thread function must return void*
}

int main() {
    pthread_t t;
    
    // Create the child thread, passing st as argument
    pthread_create(&t, NULL, fun, (void*)st);

    cout << "Created Main thread!" << endl;

    // Wait for child thread to finish
    pthread_join(t, NULL);

    return 0;
}

