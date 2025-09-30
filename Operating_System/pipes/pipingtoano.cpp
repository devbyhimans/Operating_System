#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include<sys/types.h>
using namespace std;
int main(){
  int pipefd[2];
  pid_t childpid;

  if(pipe(pipefd)==-1){
    perror("pipe failed");
    return 1;
  }

  if((childpid=fork())==-1){
    perror("fork failed");
    return 1;
  }

  if(childpid==0){
    close(pipefd[1]);
    dup2(pipefd[0],0);
    close(pipefd[0]);

    char buffer[100];
    while(fgets(buffer,100,stdin)){
      cout<<"child received: "<<buffer<<endl;
    }
  }
  else{
    close(pipefd[0]);
    dup2(pipefd[1],1);
    close(pipefd[1]);

    cout<<"Hello from parent!"<<endl;

    wait(NULL);
    return 0;
  }
}