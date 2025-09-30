#include<iostream>
#include <fstream>
#include <unistd.h>
#include<sys/wait.h>
using namespace std;

int main(){
	pid_t pid=fork();

	if(pid<0){
	cerr<<"Fork failed"<<endl;
	return 1;
	}
	
	else if(pid==0){
	execlp("./child_reader","child_reader",NULL);
	cerr<<"execlp failed"<<endl;
	return 0;
	}
	else{
	
	wait(NULL);//wait for the child to complete
	
	 ifstream file("sample.txt");
	if(!file){
        	cerr<<"Parent: failed to opem file"<<endl;
        	return 1;
		}
        string line;
	cout<<"Parent reading from the file"<<endl;

	while(getline(file,line)){
        cout<<"Parent:"<<line<<endl;
	}
        file.close();
	return 0;
	}
	
}
