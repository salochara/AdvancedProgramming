//
// Second example of threads.
// Creating multiple threads, demonstrating that threads behave in a non-deterministic way
// 4-11-19

#include <stdio.h>
#include <pthread.h>

void * threadStart(void * arg);

int main()
{
    pthread_t tid = NULL;
    int num_threads = 5;
    int i;
    int index[num_threads]; // multiple variables for each thread.

    for(i = 0; i<num_threads; i++)
    {
        index[i] = i + 1;
        pthread_create(&tid,NULL,threadStart,&index[i]); // index[i] is the parameter sent to threadStart function.
    }
    printf("This is the main thread\n");

    pthread_exit(NULL);
}

void * threadStart(void * arg)
{
    int * id = (int *) arg; // casting to an int pointer
    printf("This is the secondary thread with id: %d\n",*id);
    return NULL;
}
