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
} fileHandling;

fileHandling * checkForProgramInput(int argc, char * argv[])
{
    if(argc != 7){
        printf("File not run with needed commands\n");
        return FALSE;
    }

    fileHandling * files;
    files = malloc(1 * sizeof(fileHandling));
    int option;
    //char fileOne[MAX_FILE_NAME_SIZE], fileTwo[MAX_FILE_NAME_SIZE], resultFile[MAX_FILE_NAME_SIZE];


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
                return FALSE;
        }
    }
    return files;
}


int main(int argc, char * argv[])
{
    fileHandling * files = NULL;
    files = checkForProgramInput(argc,argv);


    return 0;
    /*if(checkForProgramInput(argc, argv))
    {
        files = checkForProgramInput(argc,argv);
        printf("hey\n");
    }*/


}


