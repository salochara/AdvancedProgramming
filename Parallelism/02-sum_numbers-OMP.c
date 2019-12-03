/*
    Simple program to sum the cosines of all integer numbers up to a limit
    This version uses OMP for parallelization
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char * argv[])
{
    unsigned int limit = 100000000;
    double total = 0;
    double partial = 0;

    // Get the limit as an argument
    if (argc == 2)
    {
        limit = atoi(argv[1]);
    }

    printf("Computing the sum of cosines from 1 to %u\n", limit);

#pragma omp parallel default(none) shared(limit, total) private(partial)
    {
#pragma omp for
        // Compute the sum of all integers up to the limit
        for (unsigned int i=1; i<=limit; i++)
        {
            partial += cos(i);
        }
#pragma omp critical
        {
            total += partial;
        }
    }

    printf("The total sum is: %lf\n", total);

    return 0;
}
