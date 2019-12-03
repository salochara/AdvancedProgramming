// First example of OMP
#include <stdio.h>
// Library for OpenMP
#include <omp.h>

int main()
{
    int num_threads;
    int tid;
    printf("About to start parallel section\n");
    // default none, and then specify private/shared variables. In this case, num_threads and tid are private
#pragma omp parallel default(none) private(num_threads, tid)
    {
        // Call the OpenMP functions
        tid = omp_get_thread_num();

        /*
        // Print a message only on the master thread
        if (tid == 0)
        {
            num_threads = omp_get_num_threads();
            printf("There are %d threads\n", num_threads);
        }
        */
#pragma omp master
        {
            num_threads = omp_get_num_threads();
            printf("There are %d threads\n", num_threads);
        }

        printf("This is thread number: %d\n", tid);
    }

    return 0;
}
