//
// Created by Salomón Charabati on 2019-09-26.
//

/* TODO
Wait for user input, to encode or decode message
Wait for user input, filename that has the message
Wait for user input, to provide key

 Fork the process
 The child executes viginerie
 Parent waits
 Child returns to parent the filename result.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX_STRING_SIZE 30

void getUserInput(char decodeEncodeOption[], char filename[], char key[]);
void openPipes(int *parentToChild, int *childToParent);
void printPipes(int pipeOne[], int pipeTwo[]);
char * vigenere(char decodeEncodeOption [], char inputFileName [], char key []);
int main()
{
    char decodeEncodeOption[MAX_STRING_SIZE], fileName[MAX_STRING_SIZE], key[MAX_STRING_SIZE];
    char resultFileName[MAX_STRING_SIZE];
    //getUserInput(decodeEncodeOption, fileName, key);

    // Open the pipes
    int parent_to_child[2];
    int child_to_parent[2];
    openPipes(parent_to_child, child_to_parent);

    printPipes(parent_to_child,child_to_parent);

    // Create a new process
    pid_t new_pid;
    new_pid = fork();

    vigenere(decodeEncodeOption, fileName, key);

    // Identify the processes
    if(new_pid > 0) // Parent process
    {

    }else if(new_pid == 0) // Child process
    {

    }
    else
    {
        printf("ERROR while doing fork\n");
    }


}

char * vigenere(char decodeEncodeOption [], char inputFileName [], char key [])
{





}

void printPipes(int pipeOne[], int pipeTwo[])
{
    printf("Pipe one content:[%d][%d]\n", pipeOne[0],pipeOne[1]);
    printf("Pipe two content:[%d][%d]\n", pipeTwo[0],pipeTwo[1]);
}

// Open the pipes and check for any errors
void openPipes(int *parentToChild, int *childToParent)
{
    int status;
    status = pipe(parentToChild);
    if(status < 0 ){ exit(EXIT_FAILURE); }
    status = pipe(childToParent);
    if(status < 0 ){ exit(EXIT_FAILURE); }

}


// Get user input needed for using the program
void getUserInput(char * decodeEncodeOption, char * filename, char * key)
{
   printf("Do you want to encode or decode?(d/e): \t");
   scanf("%s", decodeEncodeOption);

   printf("Type the filename:\t");
   scanf("%s",filename);

   printf("Type the key:\t");
   scanf("%s",key);

}

