//
// Created by Salomón Charabati on 2019-10-06.
// Advanced Programming
// Homework #4 Vigenere Cypher

#include "vigenere.h"

// FUNCTION DEFINITIONS

// Main function for running the program
int runProgram()
{
    char decodeEncodeOption[MAX_STRING_SIZE], fileName[MAX_STRING_SIZE], key[MAX_STRING_SIZE];
    char * resultFileName;
    char buffer[MAX_STRING_SIZE];
    int parent_to_child[2];
    int child_to_parent[2];

    // Opening the pipes
    openPipes(parent_to_child, child_to_parent);

    // Forking
    pid_t new_pid;
    new_pid = fork();

    // Identify the processes
    if(new_pid > 0) // Parent process
    {
        // Getting user input for running the program
        getUserInput(decodeEncodeOption, fileName, key);
        // Writes into buffer the input received from the user with specific format (separated by "|")
        sprintf(buffer,"%s|%s|%s",decodeEncodeOption,fileName,key);
        // Writing buffer into parent_to_child pipe. Meaning, passing to the child process the input typed by the user
        write(parent_to_child[1],buffer,MAX_STRING_SIZE);
        printf("- (parent) Running PID %d \n",getpid());
        // Reads pipe from child's process
        read(child_to_parent[0],buffer,MAX_STRING_SIZE);
        printf("- (parent) Received from (child) resulting file name: '%s'\n",buffer);

    }else if(new_pid == 0) // Child process
    {

        char * option, *fileNameTok, *keyTok;
        // Reading from parent_to_child pipe, the user input that's passed
        read(parent_to_child[0],buffer,MAX_STRING_SIZE);
        printf("- (child) Received from the pipe sent from (parent) this content: %s\n",buffer);
        // Tokenizing the buffer passes in the parent_to_child_pipe
        option = strtok(buffer,"|");
        fileNameTok = strtok(NULL, "|");
        keyTok = strtok(NULL, "|");
        // Applying Vigenere
        resultFileName = vigenereEntryFunction(option, fileNameTok, keyTok);
        printf("- (child) Writing to (parent) the result file name: '%s' with Vigenere algorithm applied\n",resultFileName);
        // Passing the resulting file name to the parent, in the child_to_parent pipe
        write(child_to_parent[1],resultFileName,MAX_STRING_SIZE);
        printf("- (child) Finishing my process\n");
    }
    else{ printf("ERROR while doing fork\n"); perror("fork");}

    return EXIT_SUCCESS;
}

// Vigenere entry function. Returns the resulting file name
char * vigenereEntryFunction(char *decodeEncodeOption, char *inputFileName, char *key)
{
    char * resultFileNameLocal;
    printf("- (child) You've selected to %s file '%s'\n",strncmp(decodeEncodeOption, "d", MAX_STRING_SIZE) == 0 ? "DECODE" : "ENCODE", inputFileName);

    // Encode or decrypt?
    if(strncmp(decodeEncodeOption, "d", MAX_STRING_SIZE) == 0)
        resultFileNameLocal = vigenere_algorithm(inputFileName, key, DECODE_OPTION);
    else
        resultFileNameLocal = vigenere_algorithm(inputFileName,key,ENCODE_OPTION);

    return resultFileNameLocal;
}

// Function for applying Vigenere algorithm
char * vigenere_algorithm(char *inputFileName, char key[], int option)
{
    printf("- (child) Starting Vigenere encryption algorithm\n");
    char buffer[BUFFER_SIZE];
    char * resultFileName = malloc(MAX_STRING_SIZE * sizeof(char));
    // Open up file
    FILE * fileInputPointer = NULL;
    fileInputPointer = fopen(inputFileName, "r");
    // Check for any errors in file pointer for input file
    if(fileInputPointer == NULL){ printf("Cant find file '%s'", inputFileName); perror("File error"); exit(EXIT_FAILURE);}
    // Open up output file
    FILE * fileOutputPointer = NULL;

    // Prepare output file name
    if(option == ENCODE_OPTION)
    {
        // Set the output file name to be "encoded_{file_name}.txt"
        char outputFileName[MAX_STRING_SIZE];
        strncpy(outputFileName,"encoded_",MAX_STRING_SIZE);
        strncat(outputFileName,inputFileName,MAX_STRING_SIZE);
        fileOutputPointer = fopen(outputFileName, "w");
        strncpy(resultFileName,outputFileName,MAX_STRING_SIZE);
    }else if(option == DECODE_OPTION)
    {
        // Set the output file name to be "{file_name}.txt"
        char * outputFileName = NULL;
        strtok(inputFileName,"_");
        outputFileName = strtok(NULL," ");
        fileOutputPointer = fopen(outputFileName, "w");
        strncpy(resultFileName,outputFileName,MAX_STRING_SIZE);
    }

    // Check for any errors in file pointer for output file
    if(fileOutputPointer == NULL){ printf("Can't open file for result\n"); exit(EXIT_FAILURE);}

    int keyLength = strlen(key);
    int i = 0, j= 0;
    char newMessage[200]; // new encrypted or decrypted message
    while (fgets(buffer, BUFFER_SIZE, fileInputPointer))
    {
        size_t sizeOfBuffer = strlen(buffer);
        for (i = 0; i < sizeOfBuffer ; ++i, ++j)
        {
            if(!isalpha(buffer[i])){
                newMessage[i] = buffer[i];
                continue;
            }
            if(isalpha(buffer[i]))
            {
                // Only change in algorithm for encryption or decryption
                if(option == DECODE_OPTION)
                    newMessage[i] = ((( buffer[i] -'a')  - (key[j % keyLength] -'a') + 26 ) % 26) + 'a'; // for decrypt
                else if(option == ENCODE_OPTION)
                    newMessage[i] = ((buffer[i] - 'a' + key[j % keyLength] - 'a') % 26 ) + 'a'; // for encrypt
            }
        }
        newMessage[i] ='\0';
        fprintf(fileOutputPointer, "%s", newMessage);
    }
    fclose(fileInputPointer);
    fclose(fileOutputPointer);

    return resultFileName;

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

// Get user input needed for using the program and validates the user input
void getUserInput(char * decodeEncodeOption, char * filename, char * key)
{

   printf("Do you want to encode or decode?(d/e): \t");
   scanf("%s", decodeEncodeOption);
   // Validating the option typed by the user
   if(strncmp(decodeEncodeOption,"d",MAX_STRING_SIZE) != 0 && strncmp(decodeEncodeOption,"e",MAX_STRING_SIZE) != 0)
   {
       printf("Invalid option. It's either 'd' or 'e' \n");
       exit(EXIT_SUCCESS);
   }

   printf("Type the filename (the file needs to be in this directory):\t");
   scanf("%s",filename);

   // Check that files exists in the current directory
   FILE * fin = NULL;
   fin = fopen(filename,"r");
   if(fin == NULL)
   {
       printf("The file you typed doesn't exist or it's not in your current directory\n");
       char * command = "ls";
       char * arguments[] = {"ls", (char *) NULL};
       printf("These are the files that are in your current directory:\n");
       execvp(command,arguments);
       perror("File error");
   }
   fclose(fin);

   // If the user chooses to decode, the file name must typed must be called 'encoded_{file_name}'
   if(strstr(filename,"encoded_") == NULL && strncmp(decodeEncodeOption,"d",MAX_STRING_SIZE) == 0 )
   {
       printf("You chose to decode and the file given ('%s') is not encoded\n",filename);
       exit(EXIT_SUCCESS);
   }

   printf("Type the key:\t");
   scanf("%s",key);
   printf("\n");
}

