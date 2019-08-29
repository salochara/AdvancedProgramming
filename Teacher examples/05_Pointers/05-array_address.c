/*
   Program to compare arrays with pointers

   Gilberto Echeverria
   gilecheverria@yahoo.com
   11/01/2017
*/

#include <stdio.h>

void printArray(int data[], int size);

int main()
{
    int data[] = {1, 2, 3, 4, 5};
    int * pointer = data;
    int size = sizeof data / sizeof data[0];

    printf ("Address of array:\t\t %p\n", (void *) &data);
    printf ("Address of first element:\t %p\n", (void *) &data[0]);
    printf ("Contents of array variable:\t %p\n", (void *) data);
    printf ("Contents of pointer variable:\t %p\n", (void *) pointer);
    printf ("Address of pointer variable:\t %p\n", (void *) &pointer);

    printf ("Contents of the array:\n");
    printArray(data, size);

    printf ("Second element: %d == %d == %d\n", data[2], *(pointer+2), pointer[2]);


    return 0;
}

void printArray(int data[], int size)
{
    for (int i=0; i<size; i++)
    {
        printf("%d ", data[i]);
    }
    printf("\n");
}
