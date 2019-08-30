//
// Created by Salomón Charabati on 2019-08-27.
// Advanced Programming in C
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_FILE_NAME_SIZE 20

typedef struct {
    char fileOne[MAX_FILE_NAME_SIZE];
    char fileTwo[MAX_FILE_NAME_SIZE];
    char resultFile[MAX_FILE_NAME_SIZE];
    int validInput;
    int resultingMatrixColumns;
    int resultingMatrixRows;
} fileHandling;

typedef struct {
    float ** stackPointers;
    float * array;
} matrix ;

fileHandling * checkForProgramInput(int argc, char * argv[])
{
    int option;
    fileHandling * files = NULL;
    files = malloc(1 * sizeof(fileHandling));

    if(argc != 7){
        printf("File not run with needed commands\n");
        files->validInput = FALSE; return files;
    }

    printf("This program has %d arguments \n", argc);
    for (int i = 0; i < argc; ++i) {
        printf("\t%d: %s \n",i,argv[i]);
    }

    while((option = getopt(argc,argv,"1:2:r:")) != -1)
    {
        switch(option)
        {
            case '1':
                strncpy(files->fileOne,optarg,MAX_FILE_NAME_SIZE);
                printf("%s \n",files->fileOne);
                break;
            case '2':
                strncpy(files->fileTwo,optarg,MAX_FILE_NAME_SIZE);
                printf("%s \n",files->fileTwo);
                break;
            case 'r':
                strncpy(files->resultFile,optarg,MAX_FILE_NAME_SIZE);
                printf("%s\n",files->resultFile);
                break;
            case '?':
                printf("Unknown option %c \n", option);
                files->validInput = FALSE;
                return files;
        }
    }
    // It's a valid command for running the program
    files->validInput = TRUE;
    return files;
}

int matrixCanMultiply(fileHandling * files)
{
    FILE * fileOnePtr = NULL;
    FILE * fileTwoPtr = NULL;
    int columnsFileOne, rowsFileOne, columnsFileTwo, rowsFileTwo;

    char fileOne[MAX_FILE_NAME_SIZE], fileTwo[MAX_FILE_NAME_SIZE];
    strncpy(fileOne,files->fileOne,MAX_FILE_NAME_SIZE);
    strncpy(fileTwo,files->fileTwo, MAX_FILE_NAME_SIZE);

    fileOnePtr = fopen(fileOne,"r");
    fileTwoPtr = fopen(fileTwo, "r");

    if(fileOnePtr && fileTwoPtr){
        fscanf(fileOnePtr,"%d %d", &rowsFileOne, &columnsFileOne); // %*d is uses for not storing that value read anywhere.
        fscanf(fileTwoPtr, "%d %d", &rowsFileTwo, &columnsFileTwo);
        printf("columns fileOne: %d rowsfileTwo: %d \n",columnsFileOne,rowsFileTwo);
    }else {
        printf("File error\n");
        return FALSE;
    }

    if (columnsFileOne == rowsFileTwo){
        files->resultingMatrixColumns = columnsFileTwo;
        files->resultingMatrixRows = rowsFileOne;
        return TRUE;
    }else
        return FALSE;
}



matrix * allocateMemory(fileHandling * files)
{
    matrix* matrix = NULL;
    matrix = malloc(sizeof(matrix));
    float ** localStackPointers = NULL;

    localStackPointers = (float**)malloc(files->resultingMatrixRows * sizeof(float*));

    for (int i = 0; i < files->resultingMatrixRows; ++i) {
        localStackPointers[i] = calloc(files->resultingMatrixColumns, sizeof(float));
    }
    matrix->stackPointers = localStackPointers;

    return matrix;
}


int main(int argc, char * argv[])
{
    fileHandling * files = NULL;
    matrix * matrix = NULL;
    files = checkForProgramInput(argc,argv);
    if(files->validInput) {
        if (matrixCanMultiply(files)) {
            printf("Yes they can multiply!\n");
            // Do the multiplication. Meaning
            matrix = allocateMemory(files);
            for (int j = 0; j < files->resultingMatrixRows; ++j) {
                for (int i = 0; i < files->resultingMatrixColumns; ++i) {
                    printf("%f \t",**(matrix->stackPointers));
                }
                printf("\n");
            }
            // Get the memory allocation for the result matrix
            // Do the actual multiplication
            // Write the result in an output .txt
        } else {
            printf("No multiplication can be done!\n");
        }
    }

    matrix->stackPointers[0][1] = 1;

    for (int k = 0; k < files->resultingMatrixRows; ++k) {
        for (int i = 0; i < files->resultingMatrixColumns; ++i) {
            printf("matrix at[%d][%d][%f]\n",k,i,matrix->stackPointers[k][i]);
        }
        //printf("\n");
    }

    return 0;
}


