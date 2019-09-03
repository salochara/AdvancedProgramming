// Created by Salomón Charabati on 2019-08-27.
// Advanced Programming in C
// Matrix Project with memory allocation contiguous
// TODO Make file
// TODO el malloc que se hace en la función fileHandling, como se libera ?

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_FILE_NAME_SIZE 20
#define FILE_ONE 1
#define FILE_TWO 2


// STRUCT DEFINITIONS
typedef struct {
    char fileOne[MAX_FILE_NAME_SIZE];
    char fileTwo[MAX_FILE_NAME_SIZE];
    char resultFile[MAX_FILE_NAME_SIZE];
    int validInput; // TRUE or FALSE
    int resultingMatrixColumns;
    int resultingMatrixRows;
} fileHandling;

typedef struct matrix{
    float ** mat;
    int numberOfRows;
    int numberOfColumns;
} matrix_t;

//FUNCTIONS DEFINITIONS
fileHandling * checkForProgramInput(int argc, char * argv[]);
int matrixCanMultiply(matrix_t * matrixA, matrix_t * matrixB, fileHandling * files);
void multiplyMatrix(const matrix_t * matrixA, const matrix_t* matrixB, matrix_t* resultMatrix);
void writeToFile(fileHandling * files, matrix_t * resultMatrix);
void helper_checkForRowsAndCols(matrix_t * matrix, fileHandling *files, int fileNo);
void allocateMemoryBetter(matrix_t * matrixToAllocate);
void initializeMatrixBetter(matrix_t * matrix, fileHandling * files, int fileNumber);
void helper_setColumnsAndRowsForResultMatrix(matrix_t * matrix, fileHandling* files);
void freeMemoryBetter(matrix_t *matrix);
void runProgram(matrix_t * matrixA, matrix_t * matrixB, matrix_t * resultMatrix, fileHandling * files);
void printMatrix(const matrix_t * matrix);

int main(int argc, char * argv[])
{
    // Initialization of pointers
    fileHandling * files = NULL;

    matrix_t * resultMatrix = NULL;
    resultMatrix = malloc(sizeof(matrix_t));

    matrix_t * matrixA = NULL;
    matrixA = malloc(sizeof(matrix_t));

    matrix_t * matrixB = NULL;
    matrixB = malloc(sizeof(matrix_t));

    if(matrixA == NULL || matrixB == NULL || resultMatrix == NULL)
    {
        printf("Malloc error.\n");
        exit(EXIT_FAILURE);
    }
    // Check of the program was run with valid arguments
    files = checkForProgramInput(argc,argv);
    if(files->validInput) {
        // The arguments input are valid, then
        helper_checkForRowsAndCols(matrixA, files, FILE_ONE);
        helper_checkForRowsAndCols(matrixB,files,FILE_TWO);
        if (matrixCanMultiply(matrixA,matrixB, files)) {
            runProgram(matrixA, matrixB, resultMatrix, files);
        } else
            exit(EXIT_SUCCESS);
    }
    return 0;
}

// FUNCTIONS IMPLEMENTATIONS
void runProgram(matrix_t * matrixA, matrix_t * matrixB, matrix_t * resultMatrix, fileHandling * files)
{
    // Allocate memory for matrixA and matrixB
    allocateMemoryBetter(matrixA);
    allocateMemoryBetter(matrixB);

    // Initialize  matrixA and matrixB with the values from both .txt files
    initializeMatrixBetter(matrixA, files, FILE_ONE);
    initializeMatrixBetter(matrixB,files,FILE_TWO);

    // Use the helper function for getting the getting the amount of cols and rows for result matrix, which are stored in files struct
    helper_setColumnsAndRowsForResultMatrix(resultMatrix,files);

    // Allocate memory for result matrix
    allocateMemoryBetter(resultMatrix);

    // Do the matrix multiplication and store it in result matrix
    multiplyMatrix(matrixA,matrixB,resultMatrix);

    // Write to file the result matrix
    writeToFile(files,resultMatrix);

    // Free the mallocs
    freeMemoryBetter(matrixA);
    freeMemoryBetter(matrixB);
    freeMemoryBetter(resultMatrix);
}

// Helper function for setting cols and rows of the result matrix, that are stored in the files struct
void helper_setColumnsAndRowsForResultMatrix(matrix_t * matrix, fileHandling* files)
{
    matrix->numberOfColumns = files->resultingMatrixColumns;
    matrix->numberOfRows = files->resultingMatrixRows;
}

// Function for freeing the mallocs
void freeMemoryBetter(matrix_t *matrix)
{
    free(matrix->mat[0]);
    free(matrix->mat);
    free(matrix);
}

// Function for printing matrix
void printMatrix(const matrix_t * matrix)
{
    for (int i = 0; i < matrix->numberOfRows; ++i) {
        for (int j = 0; j < matrix->numberOfColumns; ++j) {
            printf("%f ", matrix->mat[i][j]);
        }
        printf("\n");
    }
}

// Helper function for opening a file and check for the rows and cols of the matrix: Which should be in the first row of the .txt file
void helper_checkForRowsAndCols(matrix_t * matrix, fileHandling *files, int fileNo)
{
    FILE * fptr = NULL;
    char nameOfFile [MAX_FILE_NAME_SIZE];

    // Check if it's MatrixA or MatrixB (meaning, fileOne or fileTwo in the filesHandling struct
    (fileNo == 1) ?  strncpy(nameOfFile, files->fileOne,MAX_FILE_NAME_SIZE) : strncpy(nameOfFile,files->fileTwo,MAX_FILE_NAME_SIZE);

    //Get the number of rows and cols from the file and save it in the matrix passed by reference
    fptr = fopen(nameOfFile, "r");
    if(fptr){
        fscanf(fptr,"%d %d", &matrix->numberOfRows, &matrix->numberOfColumns);
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
        printf("File not run with needed commands\nIt needs to be in this format  ./multiplication -1 {filename_matrix_1} -2 {filename_matrix_2} -r {filename_matrix_result}\n");
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
// If true, set the cols and rows of the result matrix
int matrixCanMultiply(matrix_t * matrixA, matrix_t * matrixB, fileHandling * files)
{
    if(matrixA->numberOfColumns == matrixB->numberOfRows)
    {
        files->resultingMatrixColumns = matrixB->numberOfColumns;
        files->resultingMatrixRows = matrixA->numberOfRows;
        return TRUE;
    }else{
        printf("You cannot multiply matrices from files \"%s\" and \"%s\"\n", files->fileOne, files->fileTwo);
        return FALSE;
    }
}

// Function for allocating memory for the result matrix in contiguous memory
void allocateMemoryBetter(matrix_t * matrixToAllocate)
{
    // Allocate memory for the pointers to the rows. mat in the matrix_t struct is a double pointer
    matrixToAllocate->mat = (float**)malloc(matrixToAllocate->numberOfRows * sizeof(float*)); // it is storing pointers to floats
    if(matrixToAllocate->mat == NULL)
    {
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }

    // In the first element of the pointers to row, save the address to where the whole matrix starts
    // The matrix is saved in contiguous memory, because there is only one call to calloc
    matrixToAllocate->mat[0] = (float*)calloc(matrixToAllocate->numberOfRows * matrixToAllocate->numberOfColumns, sizeof(float));
    if(matrixToAllocate->mat[0] == NULL)
    {
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }

    // Assigning rows indexes to "matrixToAllocate.mat"
    // e.g. Assuming, the whole matrix starts at address 200 and i = 1
    // mat[1] = mat[0] + numberOfColumns * 1
    // mat[1] = 200 + 3(4) * 1
    // mat[1] = 212. This is the address to where the second row starts
    // important: numberOfColumns(bytes) used for storing floats
    for (int i = 1; i < matrixToAllocate->numberOfRows ;  ++i)
    {
        matrixToAllocate->mat[i] = matrixToAllocate->mat[0] + matrixToAllocate->numberOfColumns * i;
    }
}

// Function for initialize a matrix with values from the .txt files
void initializeMatrixBetter(matrix_t * matrix, fileHandling * files, int fileNumber)
{
    FILE* fptr = NULL;
    char fileName [MAX_FILE_NAME_SIZE];
    fileNumber == 1 ? strncpy(fileName,files->fileOne,MAX_FILE_NAME_SIZE) : strncpy(fileName,files->fileTwo,MAX_FILE_NAME_SIZE);

    fptr = fopen(fileName,"r");
    fscanf(fptr, "%*[^\n]"); // for skipping the first line
    for(int i = 0; i <matrix->numberOfRows; i++)
    {
        for(int j = 0; j < matrix->numberOfColumns; j++)
        {
            if (!fscanf(fptr, "%f", &matrix->mat[i][j]))
                break;
        }
    }
    fclose(fptr);
}


// Function for multiplying two matrices and store the result in the pointer passed
void multiplyMatrix(const matrix_t * matrixA, const matrix_t * matrixB, matrix_t * resultMatrix)
{
    for(int i = 0; i < matrixA->numberOfRows; ++i)
    {
        for(int j = 0; j < matrixB->numberOfColumns; ++j)
        {
            for(int k=0; k<matrixA->numberOfColumns; ++k)
            {
                resultMatrix->mat[i][j] += matrixA->mat[i][k] * matrixB->mat[k][j];
            }
        }
    }
}

// Function for writing to file
void writeToFile(fileHandling * files, matrix_t * resultMatrix)
{
    char resultFileName[MAX_FILE_NAME_SIZE];
    strncpy(resultFileName, files->resultFile, MAX_FILE_NAME_SIZE); // gets the name of the file from the files struct
    FILE * fptr = NULL;
    fptr = fopen(resultFileName, "w");

    for (int l = 0; l < files->resultingMatrixRows; ++l) {
        for (int i = 0; i < files->resultingMatrixColumns; ++i) {
            fprintf(fptr, "%f ", resultMatrix->mat[l][i]);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}
