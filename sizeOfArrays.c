//
// Created by Salomón Charabati on 2019-09-02.
//

#include <stdio.h>


// When you pass the array to a function, it recieves the pointer.
void printArray(int * array, int size) // if the size is not passed, only the pointer to the first element is passes
{
    printf("Size of the array: %d\n",size);
    for (int i = 0; i < size; ++i) {
        printf("[%d]", array[i]);
    }
    printf("\n");
}

// sizeof array, which is the size of the pointer to the first element of the array. Therefore, it prints 8 bytes
void printArrayWrong(int array[])
{
    printf("Size of the array %d\n", sizeof array );
}


int main()
{
    int array[] = {1,2,3,4};
    int more[6];
    int another[7] = {1};

    printArray(array, sizeof array / sizeof array[0]); // to know the type stored in the array, access the first element of it
    printArray(more, sizeof more / sizeof more[0]);
    printArray(another, sizeof another / sizeof another[0]);

    printf("Getting the size of the array wrong: \n");
    printArrayWrong(array);


}