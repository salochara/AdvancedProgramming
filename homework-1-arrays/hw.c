// Advanced Programming Course at ITESM
// Homework 1
// Created by Salomón Charabati on 2019-08-21.
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define ARRAY_SIZE 30
#define FILE_NAME "differences.txt"
int * fillArray();
void writeToFile(int inputNumber,const int* array);

int main()
{
    int * array, inputNumber;

    array = fillArray();
    printf("Type any number in the range of [1-100]: \t");
    scanf("%d", &inputNumber);
    writeToFile(inputNumber,array);

    return 0;
}

// void function -> receives as a constant the number input by the user and the array filled with random numbers.
// This function compares the inputNumber with each of the element of the array and prints the difference in a new file.
void writeToFile(const int inputNumber,const int* array)
{
    int subtraction;
    FILE * file_ptr = NULL; // A pointer to a file. Needed for handling files.
    file_ptr = fopen(FILE_NAME, "w");
    if(file_ptr) // to verify if the file is open
    {
        for (int j = 0; j < ARRAY_SIZE; ++j){
            subtraction = inputNumber - array[j];
            fprintf(file_ptr, "%d \n", subtraction);
        }
        fclose(file_ptr);
        printf("File successfully called: \"%s\" \n", FILE_NAME);
    }else
        printf("File not created \n");
}

// Function that fills an array of size ARRAY_SIZE and returns a pointer to the array created.
int * fillArray()
{
    srand( (unsigned)time( NULL ) ); // for having a different seed for the random numbers.
    int static array [ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        array[i] = rand() % 100 + 1; //random numbers from range [1,100]
    }
    return array;
}