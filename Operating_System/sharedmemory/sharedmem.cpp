#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
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

  char* shared_data = (char*)shmat(shmid,NULL,0);
  if(shared_data == (char*)-1){
    perror("shmat failed");
    return 1;
  }

  cout<<"shared messages Writer\n";
  cout<<"Type messages(type exit to quit)\n";

  string input;
  while(true){
    cout<<">";
    getline(cin,input);
    strcpy(shared_data,input.c_str());
    if(input=="exit")
      break;
    
  }
  shmdt(shared_data);
  return 0;
}