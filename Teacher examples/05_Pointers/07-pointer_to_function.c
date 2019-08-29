/*
    Pointers to functions

    Examples made by Jose Enrique Estremadoyro Fort A01018990
*/

#include <stdio.h>

// Declare a global pointer variable to store the address of a function
void (*ptr)(int,int);

// Function declarations
void changeFunction1(int a, int b);
void changeFunction2(int a, int b);
void changeFunction3(int a, int b);

int main(int argc,char * argv[]){
    // Assign the address of a function to the pointer
    ptr = &changeFunction1;
    // Call the functions from the pointer
    ptr(1, 2);
    ptr(3, 4);
    ptr(5, 6);
}

void changeFunction1(int a,int b){
    printf("function1 %d %d\n", a, b);
    // Update the pointer to a different function
    ptr = &changeFunction2;
}

void changeFunction2(int a, int b){
    printf("function2 %d %d\n", a, b);
    // Update the pointer to a different function
    ptr = &changeFunction3;
}

void changeFunction3(int a, int b){
    printf("function3 %d %d\n", a, b);
    // Update the pointer to a different function
    ptr = &changeFunction1;
}
