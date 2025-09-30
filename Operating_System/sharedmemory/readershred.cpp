#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

using namespace std;
#define SHM_SIZE 1024

int main(){
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

    cout<<"shared memory reader\n";
    cout<<"wait for messages..\n";

    string lmessage;
    while(true){
        cout<<"check"<<endl;
        if(lmessage!=ptr){
            lmessage=ptr;
        cout<<"[Received]: "<<lmessage<<endl;
        }

        if(lmessage=="exit"){
            cout<<"reader quiting\n";
            break;
        }
        
        // avoid busy spinning
        usleep(5000000);   // 5 seconds

    }
        shmdt(ptr);

        shmctl(shmid,IPC_RMID,NULL);
        return 0;
}
