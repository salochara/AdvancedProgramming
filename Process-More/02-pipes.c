//
// Created by Salomón Charabati on 2019-09-12.
//
#include  <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // UNIX function i.e. getpid() fork()
#define BUFFER_LENGTH 100

void preparePipes(int * in_pipe, int * out_pipe)
{
    // Close the pipe ends that won't be usd
    close(in_pipe[1]); // Close the writing file descriptor
    close(out_pipe[0]); // Close the reading file descriptor
}
// Write index 1
// Read index 0

int main()
{
    char buffer[BUFFER_LENGTH];

    // pid:T datatype of pid
    pid_t new_pid;
    int status;
    // Pipe arrays. En los pipes, uno tiene que estar escuavhando y otro leyendo. Los dos ends no pueden estar haciendo lo mismo
    int parent_to_child[2];
    int child_to_parent[2];
    printf("My PID is %d and my parent's PID is %d\n", getpid(), getppid());

    // Open the pipes
    status = pipe(parent_to_child);
    if(status < 0 ){ exit(EXIT_FAILURE); }
    status = pipe(child_to_parent);
    if(status < 0 ){ exit(EXIT_FAILURE); }


    // Print both pipes
    printf("Parent to child: [%d][%d]\n", parent_to_child[0],parent_to_child[1]);
    printf("Child to parent: [%d][%d]\n", child_to_parent[0],child_to_parent[1]);


    // Create a new process
    new_pid = fork();
    // Identify the new process
    if(new_pid > 0) // Parent process
    {
        preparePipes(child_to_parent,parent_to_child);
        sprintf(buffer, "Hello child\n");
        write(parent_to_child[1],buffer,strlen(buffer));
    }else if(new_pid == 0)  // Child process
    {
        preparePipes(parent_to_child,child_to_parent);
        read(parent_to_child[0],buffer,BUFFER_LENGTH);
        printf("%s\n",buffer);
    }else{
        printf("ERROR when doing fork\n");
    }


}

