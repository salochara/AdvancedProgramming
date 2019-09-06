//
// Created by Salomón Charabati on 2019-09-05.
// Demonstration of function system

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int status;
    char * command = 'ls';

    status = system(command); // system stops the program, runs ls and, returns a status and gets back to this program
    printf("System returned %d\n",status);

    return 0;
}