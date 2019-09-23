/*
Example from:
http://www.bogotobogo.com/Embedded/Little_endian_big_endian_htons_htonl.php
*/

#include <iostream>
using namespace std;

/* if the dereferenced pointer is 1, the machine is little-endian
   otherwise the machine is big-endian */

int endian() {
    int one = 1;
    char *ptr;
    ptr = (char *)&one;
    return (*ptr);
}

int main()
{
    if(endian())
        cout << "little endian\n";
    else
        cout << "big endian\n";
}