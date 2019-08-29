/*
    Program to test operations on 3D vectors
    Uses: Arrays, Pointers, Dynamic Memory, Structs

    Gilberto Echeverria
    14/08/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_VECTORS 5

// Type definitions

typedef struct {
    float i;
    float j;
    float k;
} vector3d;

// Function declarations

vector3d * create_vector_array(int num_vectors);
void initialize_vector_array(vector3d * vector_array, int num_vectors);
vector3d add_vectors(vector3d * vector_array, int num_vectors);
void print_vector(vector3d vector);

int main ()
{
    int num_vectors = NUM_VECTORS;
    vector3d * vector_array = NULL;
    vector3d * another_array = NULL;
    vector3d result;

    vector_array = create_vector_array(num_vectors);
    initialize_vector_array(vector_array, num_vectors);

    // Testing GDB
    result = add_vectors(another_array, num_vectors);
    //result = add_vectors(vector_array, num_vectors);

    print_vector(result);

    // Free the memory allocated
    free(vector_array);

    return 0;
}

/*
    Prepare the memory for an array of vector structures
*/
vector3d * create_vector_array(int num_vectors)
{
    vector3d * vector_array = NULL;
    vector_array = malloc(num_vectors * sizeof(vector3d));
    // Check that the allocation could be done correctly
    if (vector_array == NULL)
    {
        perror("Error: malloc");
        exit(EXIT_FAILURE);
    }

    return vector_array;
}

/*
    Fill an array of vectors with random float values
*/
void initialize_vector_array(vector3d * vector_array, int num_vectors)
{
    // Initialize random seed
    srand(time(NULL));

    // Add random values to each of the elements in the array
    for (int i=0; i<num_vectors; i++)
    {
        vector_array[i].i = rand() / (float) RAND_MAX;
        vector_array[i].j = rand() / (float) RAND_MAX;
        vector_array[i].k = rand() / (float) RAND_MAX;
        printf("%d -- ", i);
        print_vector(vector_array[i]);
    }
}

/*
    Add all the vectors in an array into a single resulting vector
*/
vector3d add_vectors(vector3d * vector_array, int num_vectors)
{
    // Create and initialize the result structure
    vector3d result = {0, 0, 0};

    for (int i=0; i<num_vectors; i++)
    {
        result.i += vector_array[i].i;
        result.j += vector_array[i].j;
        result.k += vector_array[i].k;
    }

    return result;
}

/*
    Print the contents of a single vector
*/
void print_vector(vector3d vector)
{
    printf("Vector: [i: %6.2f, j: %6.2f, k: %6.2f]\n", vector.i, vector.j, vector.k);
}
