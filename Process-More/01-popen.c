//
// Created by Salomón Charabati on 2019-09-12.
// Test of popen to get the output of a command

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main()
{
    // Popen

    FILE* in_data;
    char* command = "../Process/test";
    char buffer[BUFFER_SIZE];

    printf("The parent PID of popen program is: %d (which is the same as the bash's)\n", getppid());

    printf("The popen program has pid: %d\n",getpid());

    // Open the command to read its output
    in_data = popen(command,"r");

    if(!in_data)
    {
        perror("Unable to open command");
        exit(EXIT_FAILURE);
    }

    while(fgets(buffer,BUFFER_SIZE,in_data)) // to read the "file" (executable)
    {
        printf("I found this: %s", buffer);
    }

    pclose(in_data);

    return 0;
}

