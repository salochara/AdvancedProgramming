/*
    Using pointer arithmetic to work with arrays

    Example from:
    http://www.cplusplus.com/doc/tutorial/pointers/
*/

#include <stdio.h>

int main ()
{
    int numbers[5];
    int * p;

    // Use pointer arithmetic to fill the array with data
    p = numbers;  *p = 10;
    p++;  *p = 20;
    p = &numbers[2];  *p = 30;
    p = numbers + 3;  *p = 40;
    p = numbers;  *(p+4) = 50;

    // Print the contents of the array
    printf("Final contents of the array:\n");
    for (int n=0; n<5; n++)
    {
        printf("%d, ", numbers[n]);
    }
    printf("\n");
    return 0;
}
