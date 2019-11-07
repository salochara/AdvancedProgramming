//
// Fourth example of threads.
// Creating multiple threads using structs
// 7-11-19

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct data_struct_thread{
    int num;
    int square;
    int cube;
} data_t;

void * threadStart(void * arg);

int main()
{
    pthread_t * tid = NULL;
    data_t * thread_data = NULL;
    int num_threads;

    printf("Number of threads to run: \n");
    scanf("%d",&num_threads);

    // Creating an array of structures. each one for a thread
    thread_data = malloc(num_threads * sizeof(data_t));
    tid = malloc(num_threads * sizeof(pthread_t));

    for(int i = 0; i<num_threads; i++)
    {
        thread_data[i].num = i + 1;
        pthread_create(&tid[i],NULL,threadStart,&thread_data[i]); // passing a structure to the thread
    }

    printf("This is the main thread\n");

    for (int j = 0; j < num_threads; ++j)
    {
        pthread_join(tid[j],NULL);
        printf("Thread %d returned %d %d %d\n",j+1,thread_data[j].num,thread_data[j].cube,thread_data[j].square);
    }

    free(thread_data);
    free(tid);

    pthread_exit(NULL);
}

void * threadStart(void * arg)
{
    // This function does not return anything because it modifies the struct (which is in thr heap) that's passed.
    // It directly modifies the struct in the heap
    data_t * my_data = (data_t*) arg;
    my_data->square = my_data->num * my_data->num;
    my_data ->cube = my_data ->num * my_data->num * my_data->num;

    printf("This is the secondary thread with id: %d\n",my_data->num);

    pthread_exit(NULL);
}
