#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <cstdlib>

#define BUFFER_SIZE 5
#define SHM_KEY 1234
#define SEM_KEY 5678

struct Shared {
    int buffer[BUFFER_SIZE];
    int in, out;
};

enum { MUTEX = 0, EMPTY = 1, FULL = 2 };

void waitSem(int semid, int semnum) {
    sembuf op = { (unsigned short)semnum, -1, 0 };
    semop(semid, &op, 1);
}

void signalSem(int semid, int semnum) {
    sembuf op = { (unsigned short)semnum, +1, 0 };
    semop(semid, &op, 1);
}

#endif
