//
// Created by Salomón Charabati on 2019-08-29.
// Example of pointers to structures
// Running this program with valgrind ./a.out for checking memory leaking

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// DATA TYPE DEFINITION
typedef struct info_structure{
    int size;       // 4 bytes
    int * data;     // 8 bytes
}info_t;

// FUNCTION DECLARATION
info_t * initInfo(int size);
void fillRandom(info_t * info);
void printInfo(info_t * info);
void freeInfo(info_t *info);

int main()
{
    // Initialize random seed
    srand(time(NULL));

    info_t * info = NULL;
    int size = rand() % 30 + 1;

    info = initInfo(size);
    fillRandom(info);
    printInfo(info);
    freeInfo(info);

    return 0;
}

// FUNCTIONS IMPLEMENTATION

// Return a pointer to a structure
info_t * initInfo(int size)
{
    info_t * info = NULL;

    // Box (memory) for the struct
    info = malloc(sizeof(info_t));

    // Get memory for the array in the struct. THE ARRAY IS NOT STORED IN THE STRUCT, ONLY THE ADDRESS TO IT!
    // -> dereference the struct to access the array
    // *(info).data is equal to info->data
    info->data = malloc(size * sizeof(int));

    // Assign a size to the struct. The array was set with size
    info->size = size;

    // return pointer to info
    return info;
}

// Fill the array with numbers
void fillRandom(info_t * info) //
{
    // info is a pointer. It's affecting the array it receives. by reference.
    for (int i = 0; i < info->size; ++i) {
        info->data[i] = rand() % 100 + 1; // [1,100]
    }
}

void printInfo(info_t * info)
{
    printf("Array of %d ints \n", info->size);
    for (int i = 0; i < info->size; ++i) {
        printf("array[%d] = %d\n",i, info->data[i]);
    }
}

void freeInfo(info_t *info)
{
    free(info->data);
    free(info);
}

