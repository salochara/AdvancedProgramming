/*
    Pointers in C

    Gilberto Echeverria
    17/08/2018
*/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int variable;
    int * pointer = NULL;
    double another;
    double * fptr = &another;
    
    variable = 123;
    pointer = &variable;
    
    // The contents of the variables and pointers
    printf("Value of 'variable' = %d\n", variable);
    printf("Address of 'variable' = %p\n", (void*) &variable);
    printf("Value of 'pointer' = %p\n", (void*) pointer);
    printf("Address of 'pointer' = %p\n", (void*) &pointer);
    printf("Value at the address pointed to by 'pointer' = %d\n", *pointer);

    // Size of the variables
    printf("Size of variable: %lu\n", sizeof variable);
    printf("Size of pointer: %lu\n", sizeof pointer);
    printf("Size of double: %lu\n", sizeof another);
    printf("Size of fptr: %lu\n", sizeof fptr);
    // Size of data types
    printf("Size of type int: %lu\n", sizeof (int));
    printf("Size of type pointer to int: %lu\n", sizeof (int*));
    printf("Size of type double: %lu\n", sizeof (double));
    printf("Size of type pointer to double: %lu\n", sizeof (double*));
    
    return 0;
}
