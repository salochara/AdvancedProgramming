/*
Examples of creation of arrays (one and two dimensional)
Showing different ways to use Dynamic memory

Gilberto Echeverria
20/08/2019
*/

#include <stdio.h>
#include <stdlib.h>

// Function definitions
void array1D();
void array2D_N_malloc();
void array2D_1_malloc();
void array2D_2_malloc();

int main()
{
    // Call the functions to test allocation
    array1D();
    array2D_N_malloc();
    array2D_1_malloc();
    array2D_2_malloc();

    return 0;
}


// Create a one-dimensional array of ints
void array1D()
{
    int size;
    int * array_ptr = NULL;

    printf("\nEnter the size of your array: ");
    scanf("%d", &size);

    // Single call to malloc
    array_ptr = (int *) malloc(size * sizeof(int));

    for (int i=0; i<size; i++)
    {
        array_ptr[i] = i * i;
        printf("array[%d] = %d\n", i, array_ptr[i]);
    }

    // Release the memory
    free(array_ptr);
}


// Create a bi-dimensional array of floats
// Using as many mallocs as the number of rows + 1
void array2D_N_malloc()
{
    int items;
    int years;
    float ** stock = NULL;

    printf("\nEnter the items: ");
    scanf("%d", &items);
    printf("Enter the years: ");
    scanf("%d", &years);

    // Allocate memory for one dimension (rows)
    stock = (float**) malloc(items * sizeof(float*));
    // Loop to allocate memory for the second dimension (columns)
    for (int i=0; i<items; i++)
    {
        stock[i] = (float*) malloc(years * sizeof(float));
    }

    // Loop over the two dimensional array
    for(int i=0; i<items; i++)
    {
        for(int j=0; j<years; j++)
        {
            stock[i][j] = (i+1) * (j+1);
            printf("stock[%d][%d] = %8.2f\n", i, j, stock[i][j]);
        }
    }

    // Free the memory, using a loop (same as when allocating)
    for(int i=0; i<items; i++)
    {
        free(stock[i]);
    }
    free(stock);
}


// Create a bi-dimensional array of floats
// Using a single malloc
void array2D_1_malloc()
{
    int items;
    int years;
    float * stock1d = NULL;

    printf("\nEnter the items: ");
    scanf("%d", &items);
    printf("Enter the years: ");
    scanf("%d", &years);

    // Single malloc with space for the two dimensional data
    stock1d = (float*) malloc(items * years * sizeof(float));

    // Loop over the array
    for(int i=0; i<items; i++)
    {
        for(int j=0; j<years; j++)
        {
            // Use "pointer arithmetic" to get the right index
            stock1d[i * years + j] = (i+1) * (j+1);
            printf("stock1d[%d][%d] = %8.2f\n", i, j, stock1d[i * years + j]);
        }
    }

    // Single free
    free(stock1d);
}


// Create a bi-dimensional array of floats
// Using two mallocs
void array2D_2_malloc()
{
    int items;
    int years;
    float ** stock2d = NULL;

    printf("\nEnter the items: ");
    scanf("%d", &items);
    printf("Enter the years: ");
    scanf("%d", &years);

    // Single malloc for the indices of the rows
    stock2d = (float**) malloc(items * sizeof(float*));
    // Allocate the space for all the data
    // Store the pointer as the first index in the previous array
    stock2d[0] = (float*) malloc(items * years * sizeof(float));
    // Store the indices of where each row begins
    for (int i=1; i<items; i++)
    {
        stock2d[i] = stock2d[i-1] + years;
    }

    // Loop over the array
    for(int i=0; i<items; i++)
    {
        for(int j=0; j<years; j++)
        {
            // Can use 2d indices to access the data
            stock2d[i][j] = (i+1) * (j+1);
            printf("stock2d[%d][%d] = %8.2f\n", i, j, stock2d[i][j]);
        }
    }

    // Free the two arrays
    free(stock2d[0]);
    free(stock2d);
}
