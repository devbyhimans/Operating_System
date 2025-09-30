#include <iostream>
#include <unistd.h>
using namespace std;
int main(){
 cout<<"Display contents of file sample.txt"<<endl;

execlp("cat","mycat","sample.txt",NULL);

cerr<<"execlp failed"<<endl;

}

