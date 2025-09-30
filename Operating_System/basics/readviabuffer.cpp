#include <iostream>      // For std::cout, std::endl (if using C++)
#include <unistd.h>      // For read(), write(), close(), dup2(), etc.
#include <fcntl.h>       // For open(), O_RDONLY, O_WRONLY, etc.
#include <sys/stat.h>    // For open() file permission flags (like 0666)
using namespace std;

int main(){

int fd=open("sample.txt",O_RDONLY);
if(fd==-1){
perror("failed to open file");
return 1;
}

char buffer[100];
int bytesRead;

//read in loop till end of file
while((bytesRead=read(fd,buffer,sizeof(buffer)-1))>0){
buffer[bytesRead]='\0';
cout<<"READ:"<<buffer<<endl;
}

if(bytesRead==-1){
perror("failed to read");
return 1;
}
}
