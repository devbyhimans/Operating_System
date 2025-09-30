//common header file

#ifndef COMMON_H
#define COMMON_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <cstring>
#include <iostream>

using namespace std;

#define SHM_KEY_ID 65
#define SEM_KEY_ID 66
#define MAX_MSG_SIZE 100

struct shared_memory {
    char buffer12[100]; // mailbox for Process1 → Process2
    int flag12;         // status flag for buffer12

    char buffer34[100]; // mailbox for Process3 → Process4
    int flag34;         // status flag for buffer34
};

#define SEM_MUTEX12 0
#define SEM_MUTEX34 1
#define SEM_NUM 2

#endif
