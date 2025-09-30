#include<iostream>
#include <fstream>
using namespace std;

int main(){
 ifstream file("sample.txt");
if(!file){
	cerr<<"Child: failed to opem file"<<endl;
	return 1;
}
	string line;
cout<<"Child reading from the file"<<endl;

while(getline(file,line)){
	cout<<"Child:"<<line<<endl;
}
	file.close();
return 0;
}
