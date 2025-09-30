  // writer_sysv.c — compile with: gcc writer_sysv.c -o writer_sysv
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <iostream>
using namespace std;
#define SHM_SIZE 1024

int main() {
    key_t key = ftok("shmfile",65);
    if(key==-1){
        perror("ftok failed");
        return 1;
    }

    int shmid = shmget(key,SHM_SIZE,0666|IPC_CREAT);
    if(shmid==-1){
        perror("shmget failed");
        return 1;
    }

    char *ptr = (char*)shmat(shmid,NULL,0);
    if(ptr==(char*)-1){
        perror("shmat failed");
        return 1;
    }

    cout<<"Shared Memory Writer\n";
    cout<<"Type messages (exit to quit)\n";

    string input;
    while(true){
        cout<<"> ";
        getline(cin,input);
        strcpy(ptr,input.c_str());

        if(input=="exit"){
            cout<<"quiting the writer"<<endl;
            break;
        }
    }

    shmdt(ptr);
    return 0;
}

