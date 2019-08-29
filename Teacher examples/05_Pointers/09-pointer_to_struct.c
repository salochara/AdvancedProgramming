/*
Example of passing a pointer to a structure

Gilberto Echeverria
28/08/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Declaration of a data type
typedef struct info_struct {
    int items;
    int * data;
} info_t;

// Function declarations

info_t * createInfo(int size);
void fillRandomInfo(info_t * info);
void printInfo(info_t * info);
void * freeInfo(info_t * info);

int main()
{
    int size;
    info_t * info = NULL;

    // Initialize the random seed
    srand(time(NULL));

    // Use a random size
    size = rand() % 30 + 5;
    printf("Creating an array of size %d\n", size);

    info = createInfo(size);
    fillRandomInfo(info);
    printInfo(info);

    info = freeInfo(info);

    return 0;
}

// Allocate the memory for the structure and its data
info_t * createInfo(int size)
{
    info_t * info = NULL;

    // Allocate the structure itself
    info = malloc(sizeof (info_t));
    // Allocate the internal array
    info->data = calloc(size, sizeof (int));
    // Store the size of the array
    info->items = size;

    return info;
}

// Fill the array inside the struct with random numbers
void fillRandomInfo(info_t * info)
{
    for (int i=0; i<info->items; i++)
    {
        // Set data in the range [1, 100]
        info->data[i] = rand() % 100 + 1;
    }
}

// Show the contents of the data in the structure
void printInfo(info_t * info)
{
    for (int i=0; i<info->items; i++)
    {
        printf("Item[%d] = %d\n", i, info->data[i]);
    }
}

// Release all the memory allocated
void * freeInfo(info_t * info)
{
    free(info->data);
    free(info);
    return NULL;
}
