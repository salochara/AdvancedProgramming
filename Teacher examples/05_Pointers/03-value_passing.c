/*
Passing values to functions by value or by reference (using pointers)

Gilberto Echeverria
13/01/2017
*/

#include <stdio.h>

///// FUNCTION DECLARATION /////
void increment_value(int val);
void increment_reference(int * val);

int main(void)
{
    int val = 5; 

    printf("Main before: %d\n", val);
    increment_value(val);
    printf("Main after: %d\n", val);
    increment_reference(&val);
    printf("Main after: %d\n", val);

    return 0;
}

///// FUNCTION DEFINITION /////

// Parameter received by value
void increment_value(int val)
{
    printf("Increment before: %d\n", val);
    val++;
    printf("Increment after: %d\n", val);
}

// Parameter received by reference
void increment_reference(int * val)
{
    printf("Increment before: %d\n", *val);
    (*val)++;
    printf("Increment after: %d\n", *val);
}
