// Parent process-->Installs handler for SIGUSR2 → (handler1)
// Will react when the child sends SIGUSR2.

// Child process-->Installs handler for SIGUSR1 → (handler2)
// Will react when the parent sends SIGUSR1.

#include <bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<signal.h>
using namespace std;

void handler1(int signo)
{
    cout<<"Signal from child"<<endl;
    cout.flush();
}
void handler2(int signo)
{
    cout<<"Signal from Parent"<<endl;
    cout.flush();
}
int main()
{
    int c;
    int pid=getpid();
    c=fork();
    if(c>0)
    {
      signal(SIGUSR2, handler1);
      sleep(1);   // Parent installs handler1 for SIGUSR2
      kill(c, SIGUSR1);            // Parent sends SIGUSR1 to child
      usleep(5000000);   // wait for child’s signal
    }
    else
    { //child program (c==0)
      signal(SIGUSR1, handler2);   // Child installs handler2 for SIGUSR1
      sleep(1);
      kill(pid, SIGUSR2);          // Child sends SIGUSR2 to parent
      usleep(5000000);   // wait for Parent’s signal
    }
    return 0;
}

/*Parent forks child.

Child installs SIGUSR1 → handler2().

Parent installs SIGUSR2 → handler1().

Child sends SIGUSR2 → parent receives → "Signal from child".

Parent sends SIGUSR1 → child receives → "Signal from parent".

So:

SIGUSR1 is the channel for parent → child communication.

SIGUSR2 is the channel for child → parent communication.*/