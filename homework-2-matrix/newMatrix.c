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
#define FILE_ONE 1
#define FILE_TWO 2

// TODO EXIT_FAILURE IN EVERY MALLOC

// TODO For freeing mallocs
// free(matrix->data[0])
// free (matrix)

// TODO Make file

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
    float ** mat;
    int numberOfRows;
    int numberOfColumns;
} matrix_t;

//FUNCTIONS DEFINITION
fileHandling * checkForProgramInput(int argc, char * argv[]);
int matrixCanMultiply(fileHandling * files);
matrix_t * allocateMemory(fileHandling * files);
matrix_t initializeMatrix(fileHandling* files, int fileNumber);
void multiplyMatrix(matrix_t matrixA, matrix_t matrixB, matrix_t* resultMatrix);
void freeingMemoryPointer(matrix_t * matrix);
void writeToFile(fileHandling * files, matrix_t * resultMatrix);
void helper_checkForRowsAndCols(matrix_t matrix, fileHandling *files, int fileNo);


int main(int argc, char * argv[])
{
    fileHandling * files = NULL;
    matrix_t * resultMatrix = NULL;
    // matrix_t * matrixA = NULL; matrix_t * matrixB = NULL;
    matrix_t matrixA, matrixB;
    files = checkForProgramInput(argc,argv);
    if(files->validInput) {
        helper_checkForRowsAndCols(matrixA, files, FILE_ONE);
        helper_checkForRowsAndCols(matrixB,files,FILE_TWO);
        if (matrixCanMultiply(files)) {
            // Allocate memory for Matrix A with info from .txt 1


            // Initialize Matrix A with .txt 1
            // Initialize Matrix B with .txt 2
            // Allocate memory for resulting matrix
            // Multiply matrix A*B
            // Write to file the result
            resultMatrix = allocateMemory(files);
            matrix_t matrixA, matrixB;
            matrixA = initializeMatrix(files,FILE_ONE);
            matrixB = initializeMatrix(files,FILE_TWO);
            multiplyMatrix(matrixA,matrixB, resultMatrix);
            writeToFile(files, resultMatrix);
            freeingMemoryPointer(resultMatrix);
        } else {
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}

// FUNCTIONS IMPLEMENTATION

void helper_checkForRowsAndCols(matrix_t matrix, fileHandling *files, int fileNo)
{
    //(matrix->numberOfRows) = 3;

    FILE * fptr = NULL;
    char nameOfFile [MAX_FILE_NAME_SIZE];
    int tmpNumCols, tmpNumRows;

    // Check if it's MatrixA or MatrixB (meaning, fileOne or fileTwo in the filesHandling struct
    (fileNo == 1) ?  strncpy(nameOfFile, files->fileOne,MAX_FILE_NAME_SIZE) : strncpy(nameOfFile,files->fileTwo,MAX_FILE_NAME_SIZE);

    //Get the number of rows and cols from the file and save it in the matrix passed by reference
    fptr = fopen(nameOfFile, "r");
    if(fptr){
        fscanf(fptr,"%d %d", &tmpNumRows, &tmpNumCols);
        matrix->numberOfRows = tmpNumRows;
        matrix->numberOfColumns = tmpNumCols;
    }else{
        printf("File error\n");
        exit(EXIT_FAILURE);
    }
}



// Function for checking the program input.
fileHandling * checkForProgramInput(int argc, char * argv[])
{
    int option;
    fileHandling * files = NULL;
    files = malloc(1 * sizeof(fileHandling));

    if(argc != 7){
        printf("File not run with needed commands\n");
        files->validInput = FALSE;
        exit(EXIT_SUCCESS);
    }

    // Checks for valid commands with a switch statement inside a while
    //: means it has to have a string mandatory after the flag
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

// Function for checking that the two matrices passed with the .txt files can multiply
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

    // Gets the rows and columns of each matrix
    if(fileOnePtr && fileTwoPtr){
        fscanf(fileOnePtr,"%d %d", &rowsFileOne, &columnsFileOne);
        fscanf(fileTwoPtr, "%d %d", &rowsFileTwo, &columnsFileTwo);
    }else {
        printf("File error. File is either not in the currrent directory or it doesn't exist\n");
        return FALSE;
    }

    // Check if both matrices can multiply
    if (columnsFileOne == rowsFileTwo){
        files->resultingMatrixColumns = columnsFileTwo;
        files->resultingMatrixRows = rowsFileOne;
        fclose(fileOnePtr); fclose(fileTwoPtr);
        return TRUE;
    }else
        printf("No multiplication can be done!\n");
        return FALSE;
}

// Function for allocating memory for the result matrix
matrix_t allocateMemoryBetter(matrix_t * matrixToAllocate)
{

    matrixToAllocate->mat = (float**)malloc(matrixToAllocate->numberOfRows * sizeof(float*));
    if(matrixToAllocate->mat == NULL)
    {
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }

    matrixToAllocate->mat[0] = (float*)calloc(matrixToAllocate->numberOfRows * matrixToAllocate->numberOfRows, sizeof(float));
    if(matrixToAllocate->mat[0] == NULL)
    {
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }

    // Assigning rows indexes to "matrixToAllocate.mat"
    for (int i = 1; i < matrixToAllocate->numberOfRows ;  ++i)
    {
        matrixToAllocate->mat[i] = matrixToAllocate->mat[0] + matrixToAllocate->numberOfColumns * i;
    }

}


matrix_t * allocateMemory(fileHandling * files)
{
    matrix_t* matrix = NULL;
    matrix = malloc(sizeof(matrix));
    float ** localStackPointers = NULL;

    localStackPointers = (float**)malloc(files->resultingMatrixRows * sizeof(float*));

    for (int i = 0; i < files->resultingMatrixRows; ++i) {
        localStackPointers[i] = calloc(files->resultingMatrixColumns, sizeof(float));
    }
    matrix->mat = localStackPointers;

    return matrix;
}




// Function for initializing matrix with the values passed with the two txt files
matrix_t initializeMatrix(fileHandling* files, int fileNumber)
{
    matrix_t matrix;
    FILE * fpointer = NULL;
    char nameOfFile[MAX_FILE_NAME_SIZE];

    // Needed fot the fopen function.
    (fileNumber == 1) ?  strncpy(nameOfFile, files->fileOne,MAX_FILE_NAME_SIZE) : strncpy(nameOfFile,files->fileTwo,MAX_FILE_NAME_SIZE);

    // Open the file and get the number of rows and cols
    fpointer = fopen(nameOfFile,"r");
    if(fpointer){
        fscanf(fpointer, "%d %d",&matrix.numberOfRows,&matrix.numberOfColumns);
    }else{
        printf("Error opening the file\n");
        exit(EXIT_SUCCESS);
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
                resultMatrix->mat[i][j] += matrixA.mat[i][k] * matrixB.mat[k][j];
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
            fprintf(fout, "%f ", resultMatrix->mat[l][i]);
        }
        fprintf(fout,"\n");
    }
    fclose(fout);
}
