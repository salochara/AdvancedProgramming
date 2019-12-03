/*
    Simple program to sum the cosines of all integer numbers up to a limit
    Using threads with a single use of mutex to synchronize
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREADS 8

// Structure to pass information to the threads
typedef struct start_stop_struct {
    double * total;
    pthread_mutex_t * sum_lock;
    unsigned int start;
    unsigned int stop;
} start_stop_t;

///// Function declarations
void * partialSum(void * arg);

int main(int argc, char * argv[])
{
    unsigned int limit = 100000000;
    pthread_t tids[NUM_THREADS];
    start_stop_t thread_limits[NUM_THREADS];
    unsigned int range;
    int status;
    // Variable to store the final result
    double total = 0.0;
    // Mutex to lock the accumulator variable
    pthread_mutex_t sum_lock = PTHREAD_MUTEX_INITIALIZER;


    // Get the limit as an argument
    if (argc == 2)
    {
        limit = atoi(argv[1]);
    }

    printf("Computing the sum of cosines from 1 to %u\n", limit);

    // Set the number of sums each thread will do
    range = limit / NUM_THREADS;

    // Create the threads
    for (int i=0; i<NUM_THREADS; i++)
    {
        thread_limits[i].total = &total;
        thread_limits[i].sum_lock = &sum_lock;
        thread_limits[i].start = i * range + 1;
        thread_limits[i].stop = i * range + range;

        // Start each thread
        status = pthread_create(&tids[i], 0, partialSum, &thread_limits[i]);
        if (status < 0)
        {
            perror("ERROR: pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Join the threads to wait for them to finish
    for (int i=0; i<NUM_THREADS; i++)
    {
        pthread_join(tids[i], NULL);
    }

    printf("The total sum is: %lf\n", total);

    pthread_exit(NULL);
}

/*
    Thread method to do a part of the total sum
*/
void * partialSum(void * arg)
{
    start_stop_t * thread_limits = (start_stop_t *)arg;
    double partial = 0.0;

    printf("Thread computing from %10u to %10u\n", thread_limits->start, thread_limits->stop);

    // Compute the sum of all integers up to the limit
    for (unsigned int i=thread_limits->start; i<=thread_limits->stop; i++)
    {
        partial += cos(i);
    }

    // Lock access to the global variable
    pthread_mutex_lock(thread_limits->sum_lock);
    *(thread_limits->total) += partial;
    // Free access to the global variable
    pthread_mutex_unlock(thread_limits->sum_lock);

    pthread_exit(NULL);
}
