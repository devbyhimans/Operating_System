#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
    cout << "Hello";       // Not guaranteed to appear immediately
    sleep(2);
    cout.flush();          // Force output now
    sleep(2);
    cout << " World";    // '\n' flushes, so both Hello and World appear now
    return 0;
}

/*Without flush():
"Hello" shayad turant na dikhe (kyunki buffer me pada hai).
" World\n" aane ke baad dono ek saath dikh jaayenge.

With flush():
"Hello" turant screen pe dikhega, fir 2 sec baad " World".*/


//--------
/*Jab tum newline character \n bhejte ho cout me, to C++ ka standard output stream line-buffered mode me hota hai (terminal pe likhte time).

Line-buffered ka matlab: har line ke end me (jab \n aata hai), buffer flush ho jaata hai automatically → yani output turant terminal pe chala jaata hai*/