/*
    Program to test operations on 3D vectors
    Uses: Arrays, Pointers, Dynamic Memory, Structs

    Gilberto Echeverria
    14/08/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LENGTH 50

// Function declarations

char * create_string(int size);
void show_strings(char * when, char * first, char * second);
void compare_strings(char * first, char * second);

int main ()
{
    char * first = NULL;
    char * second = NULL;

    // Get the memory for the strings
    first = create_string(STRING_LENGTH);
    second = create_string(STRING_LENGTH);

    // Print the current contents
    show_strings("Before initializing:", first, second);
    
    // Copy data into the new strings
    strncpy(first, "This is line number ONE", STRING_LENGTH);
    strncpy(second, "This is line number TWO", STRING_LENGTH);

    // Print the current contents
    show_strings("After initializing:", first, second);

    compare_strings(first, second);

    // Release the memory used
    free(first);
    free(second);

    return 0;
}

/*
    Dynamically allocate the memory for a new string
*/
char * create_string(int size)
{
    char * temp = NULL;
    temp = malloc(size * sizeof(char));
    if (!temp)
    {
        perror("Unable to allocate string");
        exit(EXIT_FAILURE);
    }
    return temp;
}

/*
    Print the contents of both strings, preceded by a string indicating when the data is shown
*/
void show_strings(char * when, char * first, char * second)
{
    printf("%s\n", when);
    printf("\tFirst: %s\n", first);
    printf("\tSecond: %s\n", second);
}

/*
    See which of the strings would be alphabetically first
*/
void compare_strings(char * first, char * second)
{
    int result = strncmp(first, second, STRING_LENGTH);
    if (result == 0)
    {
        printf("The strings are equal\n");
    }
    else if (result > 0)
    {
        printf("The first string goes last\n");
    }
    else // if result < 0)
    {
        printf("The first string goes first\n");
    }
}
