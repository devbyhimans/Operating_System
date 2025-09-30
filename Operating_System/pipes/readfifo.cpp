#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
using namespace std;

int main(){
  char buffer[1024];
  int fifo_fd;

  //open fifo for writing
  fifo_fd=open("sample.txt",O_RDONLY);
  if(fifo_fd==-1){
    perror("Error opening FIFO for writing");
    return 1;
  }

  //write message to the fifo
  read(fifo_fd,buffer,1024);
  cout<<"Reader process received: "<<buffer<<endl;
  close(fifo_fd);

  return 0;
}
