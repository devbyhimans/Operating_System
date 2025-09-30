#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
using namespace std;

int main(){
  char message[]="Hello from Writer process!";
  int fifo_fd;

  //open fifo for writing
  fifo_fd=open("sample.txt",O_WRONLY);
  if(fifo_fd==-1){
    perror("Error opening FIFO for writing");
    return 1;
  } 
  //write message to the fifo
  write(fifo_fd,message,strlen(message)+1);
  close(fifo_fd);

  return 0;
}
