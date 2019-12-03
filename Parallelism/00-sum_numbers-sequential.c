/*
    Simple program to sum the cosines of all integer numbers up to a limit
    This version is a linear program, without any parallelisation
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char * argv[])
{
    unsigned int limit = 100000000;
    double total = 0;

    // Get the limit as an argument
    if (argc == 2)
    {
        limit = atoi(argv[1]);
    }

    printf("Computing the sum of cosines from 1 to %u\n", limit);

    // Compute the sum of all integers up to the limit
    for (unsigned int i=1; i<=limit; i++)
    {
        total += cos(i);
    }

    printf("The total sum is: %lf\n", total);

    return 0;
}
