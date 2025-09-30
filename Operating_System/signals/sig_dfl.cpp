#include <iostream>
#include <csignal>
#include <unistd.h>
using namespace std;

void handler(int signum) {
    static int i = 0;
    if (i < 5) {
        cout << "You pressed Ctrl+C but the program will not terminate yet." << endl;
    } else {
        cout << "Now Ctrl+C will terminate the program." << endl;
        signal(SIGINT, SIG_DFL);   // Restore default action
        kill(getpid(), SIGINT);    // Send SIGINT to self
        
    }
    i++;
}

int main() {
    // Register handler once
    signal(SIGINT, handler);

    while (true) {
        cout << "Running... Press Ctrl+C." << endl;
        //pause();-->this also works same ,by keeping the program alive and here loop is not running continously just the execution is paused at that place whereas in sleep the loop was running after every 10sec of sleep.
        sleep(10);   // Avoid busy loop
    }

    return 0;
}

/*--IMPPORTANT--

👉 CASE 1: Agar sirf pehli line ho (signal(SIGINT, SIG_DFL);)

Is line se tum handler hata dete ho aur default action restore ho jaata hai (default = terminate on SIGINT).
Lekin abhi tak koi naya SIGINT aaya hi nahi.
Matlab program turant terminate nahi hoga, wo chalta rahega until tum firse Ctrl+C dabao.
Next Ctrl+C → ab default handler chalega → process exit karega.
Effect: 6th press par bhi program nahi band hoga, 7th par band hoga.



👉CASE 2: Agar sirf doosri line ho (kill(getpid(), SIGINT);)

Tum khud ko SIGINT bhej doge.
Lekin us time tak abhi bhi tumhara custom handler install hai.
To phir se wahi handler chalega, aur tum phir usi logic me ghoom jaoge (infinite ignore ho sakta hai).
 Effect: process kabhi terminate hi nahi karega, kyunki tum khud ke signal ko handle karte rahoge.

 
👉CASE 3: Dono lines rakhne ka reason

Pehle default behavior restore karo.
Phir ek dummy SIGINT khud ko bhejo → ab jo aayega us par default action (terminate) apply hoga.
 Isliye 6th Ctrl+C par program turant terminate ho jaata hai. */