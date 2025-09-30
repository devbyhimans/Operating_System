//parent writes in the pipe and child reads
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
using namespace std;

int main(){
  int pipefd[2];  //array to holf pipe decriptors
  pid_t pid;
  char message[]="Hello from the parent";
  char buffer[100];

  if(pipe(pipefd)==-1){
    perror("pipe failed");
    return 1;
  }

  pid=fork();//creating child

  if(pid==-1){
    perror("fork failed");
    return 1;
  }
  else if(pid==0){
    //child process
    close(pipefd[1]);
    read(pipefd[0],buffer,100);
    cout<<"Message received by child process: "<<buffer<<endl;
    close(pipefd[0]);
  } 
  else{
    //parent process
    close(pipefd[0]);
    write(pipefd[1],message,strlen(message)+1);
    cout<<"Message sent from parent process: "<<message<<endl;
    close(pipefd[1]);
  } 
  return 0;
}