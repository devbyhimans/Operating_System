//parent writes in the pipe and child reads
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

using namespace std;

int main(){

  const char* fifo_path = "mkfile";
  mkfifo(fifo_path, 0666);

  int fifo_fd = open(fifo_path,O_WRONLY);
  
  if(fifo_fd==-1){
    perror("error in writing");
    return 1;
  }

  //writing message
  cout<<"Messages Writer\n";
  cout<<"Type messages(type exit to quit)\n";

  string input;
  while(true){
    cout<<">";
    getline(cin,input);
    write(fifo_fd, input.c_str(), input.size()+1);
    if(input=="exit")
      break;
    
  }

  close(fifo_fd);
  unlink("mkfile");
  return 0;
}