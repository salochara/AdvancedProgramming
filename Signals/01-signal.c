//
// Created by Salomón Charabati on 2019-10-10.
// Simple program for sending signals to other programs

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        printf("Please provide a PID as an argument!\n");
        exit(EXIT_SUCCESS);
    }

    // Get the PID of the other process
    pid_t victim = atoi(argv[1]);
    // Kill is going to kill the victim process
    if(kill(victim, SIGKILL) == -1) // Sigkill is a constant, it's -9
        printf("Could not kill the requested process\n");

    return 0;
}
