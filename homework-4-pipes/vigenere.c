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
#include <ctype.h>
#define MAX_STRING_SIZE 30
#define BUFFER_SIZE 200

void getUserInput(char decodeEncodeOption[], char filename[], char key[]);
void openPipes(int *parentToChild, int *childToParent);
void printPipes(int pipeOne[], int pipeTwo[]);
char * vigenere(char decodeEncodeOption [], char inputFileName [], char key []);
char * vigenere_decode(char inputFileName[],char key[]);
int main()
{
    char decodeEncodeOption[MAX_STRING_SIZE], fileName[MAX_STRING_SIZE], key[MAX_STRING_SIZE];
    char * resultFileName;

    getUserInput(decodeEncodeOption, fileName, key);

    // Open the pipes
    int parent_to_child[2];
    int child_to_parent[2];
    openPipes(parent_to_child, child_to_parent);

    printPipes(parent_to_child,child_to_parent);

    // Create a new process
    pid_t new_pid;
    new_pid = fork();

    resultFileName = vigenere(decodeEncodeOption, fileName, key);

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
    char * resultFileNameLocal;
    // Encode or decrypt?
    if(decodeEncodeOption[0] == 'd')
    {
        resultFileNameLocal = vigenere_decode(inputFileName,key);
    }


    // call vigenere for encrypt or decrypt






}
char * vigenere_decode(char inputFileName[],char key[])
{
    FILE * filePointer = NULL;
    char buffer[BUFFER_SIZE];
    filePointer = fopen(inputFileName,"r");

    FILE * fileOutputPointer = NULL;
    fileOutputPointer = fopen("AQUIII.txt", "w");

    // encoded_aladdin.txt
    // Read what comes from the fifo file until the end

    int keyLength = strlen(key);
    char decryptedMsg[MAX_STRING_SIZE];
    while (fgets(buffer, BUFFER_SIZE, filePointer))
    {
        size_t sizeOfBuffer = strlen(buffer);
        for (int i = 0, j = 0; i < sizeOfBuffer ; ++i, ++j)
        {
            if(j == keyLength)
                j = 0;

            if(!isalpha(buffer[i])){
                buffer[i] = buffer[i];
                decryptedMsg[i] = buffer[i]; // for decrypt
                continue;
            }
            if(isalpha(buffer[i])){
                //encryptedMsg[i] = (  (msg[i] - 'a' + key[j] - 'a') % 26 )  + 'a'; // for encrypt
                int letter = (( buffer[i] -'a')  - (key[j] -'a') + 26 ) % 26; // for decryot
                decryptedMsg[i] = letter + 'a';
                buffer[i] = letter +'a';
            }
        }
        fprintf(fileOutputPointer,"%s",buffer);
        printf("\n%s\n",buffer);
    }

    fclose(filePointer);


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
   strncpy(decodeEncodeOption, "d", MAX_STRING_SIZE);
   //scanf("%s", decodeEncodeOption);

   printf("Type the filename:\t");
   strncpy(filename,"encoded_aladdin.txt",MAX_STRING_SIZE);
   //scanf("%s",filename);

   printf("Type the key:\t");
   strncpy(key,"prograavanzada",MAX_STRING_SIZE);
   //scanf("%s",key);

}

