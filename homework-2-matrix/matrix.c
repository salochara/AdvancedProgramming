//
// Created by Salomón Charabati on 2019-08-27.
//
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
} fileHandling;

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

    char fileOne[MAX_FILE_NAME_SIZE];
    strncpy(fileOne,files->fileOne,MAX_FILE_NAME_SIZE);

    fileOnePtr = fopen(fileOne,"r");
    if(fileOnePtr){
        int rows,columns;
        fscanf(fileOnePtr,"%d %d", &rows, &columns);
        printf("rows %d: \t columns: %d ",rows, columns);


    }else
        printf("File error\n"); return FALSE;


}




int main(int argc, char * argv[])
{
    fileHandling * files = NULL;
    files = checkForProgramInput(argc,argv);
    if(files->validInput){
        printf("hey\n");
        matrixCanMultiply(files);

    }



    return 0;
}


