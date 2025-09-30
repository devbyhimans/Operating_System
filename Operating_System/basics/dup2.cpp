#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
using namespace std;
int main(){
 int fd=open("sample.txt",O_RDONLY);

 if(fd==-1){
  perror("Error in opening file");
  return 1;
 }

 int n;
 dup2(fd,0);
 cin>>n;
 cout<<"This will go to the file 'sample.txt'!"<<n<<endl;
 close(fd);
 return 0;
}