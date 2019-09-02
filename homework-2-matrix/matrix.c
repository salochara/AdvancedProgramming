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

// STRUCT DEFINITIONS
typedef struct {
    char fileOne[MAX_FILE_NAME_SIZE];
    char fileTwo[MAX_FILE_NAME_SIZE];
    char resultFile[MAX_FILE_NAME_SIZE];
    int validInput;
    int resultingMatrixColumns;
    int resultingMatrixRows;
} fileHandling;

typedef struct matrix{
    float ** stackPointers;
    int numberOfRows;
    int numberOfColumns;
    float ** mat;
} matrix_t;

//FUNCTIONS DEFINITION
fileHandling * checkForProgramInput(int argc, char * argv[]);
int matrixCanMultiply(fileHandling * files);
matrix_t * allocateMemory(fileHandling * files);
matrix_t initializeMatrix(fileHandling* files, int fileNumber);
void multiplyMatrix(matrix_t matrixA, matrix_t matrixB, matrix_t* resultMatrix);
void freeingMemoryPointer(matrix_t * matrix);
void writeToFile(fileHandling * files, matrix_t * resultMatrix);


int main(int argc, char * argv[])
{
    fileHandling * files = NULL;
    matrix_t * resultMatrix = NULL;
    files = checkForProgramInput(argc,argv);
    if(files->validInput) {
        if (matrixCanMultiply(files)) {
            resultMatrix = allocateMemory(files);
            matrix_t matrixA, matrixB;
            matrixA = initializeMatrix(files,1);
            matrixB = initializeMatrix(files,2);
            multiplyMatrix(matrixA,matrixB, resultMatrix);
            writeToFile(files, resultMatrix);
            freeingMemoryPointer(resultMatrix);
        } else {
            printf("No multiplication can be done!\n");
        }
    }

    return 0;
}

// FUNCTIONS IMPLEMENTATION

// Function for checking the program input.
fileHandling * checkForProgramInput(int argc, char * argv[])
{
    int option;
    fileHandling * files = NULL;
    files = malloc(1 * sizeof(fileHandling));

    if(argc != 7){
        printf("File not run with needed commands\n");
        files->validInput = FALSE; return files;
    }

    while((option = getopt(argc,argv,"1:2:r:")) != -1)
    {
        switch(option)
        {
            case '1':
                strncpy(files->fileOne,optarg,MAX_FILE_NAME_SIZE);
                break;
            case '2':
                strncpy(files->fileTwo,optarg,MAX_FILE_NAME_SIZE);
                break;
            case 'r':
                strncpy(files->resultFile,optarg,MAX_FILE_NAME_SIZE);
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

// Function for checking that the two matrixes pointed by the files struct can multiply
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
    }else {
        printf("File error\n");
        return FALSE;
    }

    if (columnsFileOne == rowsFileTwo){
        files->resultingMatrixColumns = columnsFileTwo;
        files->resultingMatrixRows = rowsFileOne;
        fclose(fileOnePtr); fclose(fileTwoPtr);
        return TRUE;
    }else
        return FALSE;


}

// Function for allocating memory for the result matrix
matrix_t * allocateMemory(fileHandling * files)
{
    matrix_t* matrix = NULL;
    matrix = malloc(sizeof(matrix));
    float ** localStackPointers = NULL;

    localStackPointers = (float**)malloc(files->resultingMatrixRows * sizeof(float*));

    for (int i = 0; i < files->resultingMatrixRows; ++i) {
        localStackPointers[i] = calloc(files->resultingMatrixColumns, sizeof(float));
    }
    matrix->stackPointers = localStackPointers;

    return matrix;
}

// Function for initializing matrix with the values passed with the two txt files
matrix_t initializeMatrix(fileHandling* files, int fileNumber)
{
    matrix_t matrix;
    FILE * fpointer = NULL;
    char nameOfFile[MAX_FILE_NAME_SIZE];
    (fileNumber == 1) ?  strncpy(nameOfFile, files->fileOne,MAX_FILE_NAME_SIZE) : strncpy(nameOfFile,files->fileTwo,MAX_FILE_NAME_SIZE);

    fpointer = fopen(nameOfFile,"r");
    if(fpointer){
        fscanf(fpointer, "%d %d",&matrix.numberOfRows,&matrix.numberOfColumns);
    }

    matrix.mat=malloc(matrix.numberOfRows*sizeof(float*));

    for(int i=0;i<matrix.numberOfRows;++i)
        matrix.mat[i]=malloc(matrix.numberOfColumns*sizeof(float));

    for(int i = 0; i <matrix.numberOfRows; i++)
    {
        for(int j = 0; j < matrix.numberOfColumns; j++)
        {
            if (!fscanf(fpointer, "%f", &matrix.mat[i][j]))
                break;
            }
    }

    fclose(fpointer);
    return matrix;
}

// Function for multipliying two marixes and store the result in the pointer passed
void multiplyMatrix(matrix_t matrixA, matrix_t matrixB, matrix_t* resultMatrix)
{
    for(int i = 0; i < matrixA.numberOfRows; ++i)
    {
        for(int j = 0; j < matrixB.numberOfColumns; ++j)
        {
            for(int k=0; k<matrixA.numberOfColumns; ++k)
            {
                resultMatrix->stackPointers[i][j] += matrixA.mat[i][k] * matrixB.mat[k][j];
            }
        }
    }
}

// Function for freeing memory reserved with malloc
void freeingMemoryPointer(matrix_t * matrix)
{
    for (int i = 0; i < matrix->numberOfRows; ++i)
    {
        free(matrix->mat[i]);
    }
    free(matrix);
}

// Function for writing to file with the name of file in the files struct
void writeToFile(fileHandling * files, matrix_t * resultMatrix)
{
    char resultFileName[MAX_FILE_NAME_SIZE];
    strncpy(resultFileName, files->resultFile, MAX_FILE_NAME_SIZE);
    FILE * fout = NULL;
    fout = fopen(resultFileName,"w");

    for (int l = 0; l < files->resultingMatrixRows; ++l) {
        for (int i = 0; i < files->resultingMatrixColumns; ++i) {
            fprintf(fout, "%f ", resultMatrix->stackPointers[l][i]);
        }
        fprintf(fout,"\n");
    }
    fclose(fout);
}
