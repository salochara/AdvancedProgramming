//
// Third example of threads.
// Creating multiple threads with dynamic allocation (malloc)
// 7-11-19

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct data_struct_thread{
    int num;
    char letter;
}data_t;

void * threadStart(void * arg);

int main()
{
    pthread_t * tid = NULL;
    int num_threads;
    int i;
    int * index = NULL;
    int * result = NULL;

    printf("Number of threads to run: \n");
    scanf("%d",&num_threads);

    // Dynamic allocation of the array oh threads specified and tid's
    index = malloc(num_threads * sizeof(int));
    tid = malloc(num_threads * sizeof(pthread_t));

    for(i = 0; i<num_threads; i++)
    {
        index[i] = i + 1;
        pthread_create(&tid[i],NULL,threadStart,&index[i]); // index[i] is the parameter sent to threadStart function.
    }

    printf("This is the main thread\n");

    for (int j = 0; j < num_threads; ++j)
    {
        // Pthread join is the function to retrieve the result sent by the thread. It expects a tid and a double pointer where to save result
        pthread_join(tid[j],(void**)&result); // it's expecting a void**
        printf("Thread %d returned %d\n",j+1,*result);
        free(result); // important. Otherwise we would lose reference to the pointer of result.
    }

    free(index);
    free(tid);

    pthread_exit(NULL);
}

void * threadStart(void * arg)
{
    int * id = (int *) arg; // casting to an int pointer
    // We have to return a pointer. It can be a pointer to anything
    int num = *id;
    int * result = malloc(sizeof(int));
    *result = num * num;

    printf("This is the secondary thread with id: %d\n",*id);

    // This thread will return result! Which is a pointer to an int
    pthread_exit(result);
}
