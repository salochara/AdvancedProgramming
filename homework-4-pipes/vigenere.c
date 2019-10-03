//
// Created by Salomón Charabati on 2019-09-26.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define MAX_STRING_SIZE 50
#define BUFFER_SIZE 200
#define ENCODE_OPTION 0
#define DECODE_OPTION 1

void getUserInput(char decodeEncodeOption[], char filename[], char key[]);
void openPipes(int *parentToChild, int *childToParent);
void printPipes(int pipeOne[], int pipeTwo[]);
char * vigenere(char decodeEncodeOption [], char inputFileName [], char key []);
char * vigenere_decode(char inputFileName[],char key[]);
char * prepareFileName(char inputFileName[],int option);
int main()
{
    char decodeEncodeOption[MAX_STRING_SIZE], fileName[MAX_STRING_SIZE], key[MAX_STRING_SIZE];
    char * resultFileName;
    char buffer[MAX_STRING_SIZE];


    // Open the pipes
    int parent_to_child[2];
    int child_to_parent[2];
    openPipes(parent_to_child, child_to_parent);

    printPipes(parent_to_child,child_to_parent);

    // Create a new process
    pid_t new_pid;
    new_pid = fork();

    //prepareFileName(fileName,DECODE_OPTION);


    // Identify the processes
    if(new_pid > 0) // Parent process
    {
        getUserInput(decodeEncodeOption, fileName, key);
        sprintf(buffer,"%s|%s|%s",decodeEncodeOption,fileName,key);
        write(parent_to_child[1],buffer,MAX_STRING_SIZE);
        read(child_to_parent[0],buffer,MAX_STRING_SIZE);
        printf("buffer parent: %s\n",buffer);

    }else if(new_pid == 0) // Child process
    {
        char * option, *fileNameTok, *keyTok;
        read(parent_to_child[0],buffer,MAX_STRING_SIZE);
        printf("buffer: %s\n",buffer);

        option = strtok(buffer,"|");
        fileNameTok = strtok(NULL, "|");
        keyTok = strtok(NULL, "|");
        //sscanf(buffer,"%s|%s|%s",decodeEncodeOption,fileName,key);
        //printf("buffer2: %s|%s|%s",decodeEncodeOption,fileName,key);

        printf("\naqui:%s %s %s",option,fileNameTok,keyTok);
        fflush(stdout);
        resultFileName = vigenere(option, fileNameTok, keyTok);

        write(child_to_parent[1],"hola",MAX_STRING_SIZE);
    }
    else
    {
        printf("ERROR while doing fork\n");
    }


}


char * prepareFileName(char inputFileName[],int option)
{

   if(option == 1)
   {
       char encoded_[MAX_STRING_SIZE] = "encoded_";
       strncat(encoded_,inputFileName,MAX_STRING_SIZE);
   }else if(option == 0)
   {
       char decoded_[MAX_STRING_SIZE] = "decoded_";
       strncat(decoded_,inputFileName,MAX_STRING_SIZE);
   }

}
char * vigenere(char decodeEncodeOption [], char inputFileName [], char key [])
{
    char * resultFileNameLocal;
    printf("\n opcion:%s\n",decodeEncodeOption);
    // Encode or decrypt?
    if(strncmp(decodeEncodeOption,"d",MAX_STRING_SIZE) == 0)
    {
        printf("si\n");
        resultFileNameLocal = vigenere_decode(inputFileName,key);
    }else {
        printf("no\n");
        // vigenere encode
    }


}

// call vigenere for encrypt or decrypt

char * vigenere_decode(char inputFileName[],char key[])
{

    FILE * filePointer = NULL;
    char buffer[BUFFER_SIZE];
    printf("Going to open: %s",inputFileName);
    fflush(stdout);

    filePointer = fopen(inputFileName,"r");
    if(filePointer == NULL)
    {
        printf("Cant find file '%s'", inputFileName);
        perror("File error");

    }

    FILE * fileOutputPointer = NULL;
    fileOutputPointer = fopen("AQUIII.txt", "w");


    // Read what comes from the fifo file until the end

    int keyLength = strlen(key);
    int i = 0, j= 0;
    char decryptedMsg[200];
    while (fgets(buffer, BUFFER_SIZE, filePointer))
    {
        size_t sizeOfBuffer = strlen(buffer);
        printf("\n key at beggining of line: [%c] of length %d",key[j%keyLength], sizeOfBuffer);
        for (i = 0; i < sizeOfBuffer ; ++i, ++j)
        {
            /*if(j == keyLength)
                j = 0;*/

            if(!isalpha(buffer[i])){
               // buffer[i] = buffer[i];
                decryptedMsg[i] = buffer[i]; // for decrypt
                continue;
            }
            if(isalpha(buffer[i])){
                //encryptedMsg[i] = (  (msg[i] - 'a' + key[j] - 'a') % 26 )  + 'a'; // for encrypt
                int letter = (( buffer[i] -'a')  - (key[j % keyLength] -'a') + 26 ) % 26; // for decryot
                decryptedMsg[i] = letter + 'a';
                //buffer[i] = letter +'a';
            }
            //printf("%s\n",buffer);
        }
        decryptedMsg[i] ='\0';
        fprintf(fileOutputPointer,"%s",decryptedMsg);
        //printf("\n%s\n",decryptedMsg);

        fflush(stdout);
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
   //printf("Do you want to encode or decode?(d/e): \t");
   strncpy(decodeEncodeOption, "d", MAX_STRING_SIZE);
   //scanf("%s", decodeEncodeOption);

   //printf("Type the filename:\t");
   strncpy(filename,"encoded_aladdin.txt",MAX_STRING_SIZE);
   //scanf("%s",filename);

   //printf("Type the key:\t");
   strncpy(key,"prograavanzada",MAX_STRING_SIZE);
   //scanf("%s",key);
}

