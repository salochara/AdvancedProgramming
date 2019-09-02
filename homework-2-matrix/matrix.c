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

typedef struct matrix{
    float ** stackPointers;
    int numberOfRows;
    int numberOfColumns;
    float *matrix;
    float ** mat;
} matrix_t;



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
        matrix.mat[i]=malloc(matrix.numberOfColumns*sizeof(double));

    for(int i = 0; i <matrix.numberOfRows; i++)
    {
        for(int j = 0; j < matrix.numberOfColumns; j++)
        {
            if (!fscanf(fpointer, "%f", &matrix.mat[i][j]))
                break;
            }
    }

    fclose(fpointer);
    printf("rows : %d columns: %d\n",matrix.numberOfRows,matrix.numberOfColumns);

    return matrix;
}


int main(int argc, char * argv[])
{
    fileHandling * files = NULL;
    matrix_t * resultMatrix = NULL;
    files = checkForProgramInput(argc,argv);
    if(files->validInput) {
        if (matrixCanMultiply(files)) {
            printf("Yes they can multiply!\n");
            resultMatrix = allocateMemory(files);
        } else {
            printf("No multiplication can be done!\n");
        }
    }

    for (int k = 0; k < files->resultingMatrixRows; ++k) {
        for (int i = 0; i < files->resultingMatrixColumns; ++i) {
            printf("at:[%d][%d][%f]", k, i, resultMatrix->stackPointers[k][i]);
        }
        printf("\n");
    }

    // Get Matrix A from txt file
   matrix_t matrixA, matrixB;
   matrixA = initializeMatrix(files,1);
   matrixB = initializeMatrix(files,2);

    for (int j = 0; j < matrixA.numberOfRows; ++j) {
        for (int i = 0; i < matrixA.numberOfColumns; ++i) {
            printf("[%d][%d]: [%f]",j,i,matrixA.mat[j][i]);
        }
        printf("\n");
    }

    for (int j = 0; j < matrixB.numberOfRows; ++j) {
        for (int i = 0; i < matrixB.numberOfColumns; ++i) {
            printf("[%d][%d]: [%f]",j,i,matrixB.mat[j][i]);
        }
        printf("\n");
    }


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

    for (int k = 0; k < files->resultingMatrixRows; ++k) {
        for (int i = 0; i < files->resultingMatrixColumns; ++i) {
            printf("at:[%d][%d][%f]", k, i, resultMatrix->stackPointers[k][i]);
        }
        printf("\n");
    }


    // wrute to file result!!

    FILE* fout;
    fout = fopen("output.txt","w");



    for (int l = 0; l < files->resultingMatrixRows; ++l) {
        for (int i = 0; i < files->resultingMatrixColumns; ++i) {
            float * tmp = &resultMatrix->stackPointers[l][i];
            fwrite(tmp,1, sizeof(float),fout);
        }
    }
    fclose(fout);
    return 0;
}


