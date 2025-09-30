#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 3

// Define a struct to hold data for each thread
struct data {
    int thread_id;
    char *msg;
};

// Thread function
void *DisplayHi(void *thread_arg) {
    // Cast the argument to a pointer to data
    struct data *val = (struct data *) thread_arg;

    // Print the data
    cout << "Thread ID : " << val->thread_id;
    cout << " Message : " << val->msg << endl;

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];  // Array of thread identifiers
    struct data td[NUM_THREADS];     // Array of data structs for each thread
    int rc;                          // Return code from pthread_create
    int i;

    for (i = 0; i < NUM_THREADS; i++) {
        cout << "Creating Thread: " << i << endl;

        // Set data for each thread
        td[i].thread_id = i;
        td[i].msg = (char *)"Just a friendly Message!";

        // Create the thread
        rc = pthread_create(&threads[i], NULL, DisplayHi, (void *)&td[i]);
        if (rc) {
            cout << "Error: unable to create thread, " << rc << endl;
            exit(-1);
        }
    }

    // Wait for all threads to finish (optional but safer than just using pthread_exit in main)
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
