//
// Created by Salomón Charabati on 2019-08-21.
//
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 30

int * fillArray();

int main()
{
    int * array, inputNumber, substraction;
    array = fillArray();
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        printf("%d \n",array[i]);
    }
    printf("Type any number in the range of 1-100: \t");
    scanf("%d", &inputNumber);

    FILE * file_ptr = NULL;
    file_ptr = fopen("differences.txt", "w");

    for (int j = 0; j < ARRAY_SIZE; ++j) {
        substraction = inputNumber - array[j];
        fprintf(file_ptr,"%d \n",substraction);
    }
    fclose(file_ptr);
    
}

int * fillArray()
{
    //srand( (unsigned)time( NULL ) );

    int static array [ARRAY_SIZE]; // static for avoiding this warning  "warning: address of stack memory associated with local variable 'array' returned"
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = rand() % 100 + 1;
    }
    return array;
}