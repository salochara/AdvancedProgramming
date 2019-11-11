//
// Created by Salomón Charabati on 11/11/19.
// Example of threads working with Mutex with multiple variables.
// Example of deadlock. Threads are stopped beacause of a wrong implementation of mutexes


#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 2
#define MAX_COUNT 1000000

// For initializing mutex as a global variable
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;


int counter_1 = 0;
int counter_2 = MAX_COUNT;

void * increment(void * arg);
void * decrement(void * arg);

int main()
{
    pthread_t tid [NUM_THREADS];

    // Initialize the threads
    pthread_create((&tid[0]),NULL,increment,NULL);
    pthread_create((&tid[1]),NULL,decrement,NULL);

    // Wait for the threads to finish
    for (int j = 0; j < NUM_THREADS; ++j)
        pthread_join(tid[j],NULL);

    printf("The counter is %d / %d\n",counter_1,counter_2);

}

void * increment(void * arg)
{
    printf("Increment thread started\n");

    for (int i = 0; i < MAX_COUNT; ++i)
    {
        // Section to protect with mutex
        pthread_mutex_lock(&mutex_1);
            pthread_mutex_lock(&mutex_2);
                counter_1++;
                counter_2 --;
            pthread_mutex_unlock(&mutex_2);
        pthread_mutex_lock(&mutex_1);
    }

    pthread_exit(NULL);
}

void * decrement(void * arg)
{

    printf("Decrement thread started\n");

    for (int i = 0; i < MAX_COUNT; ++i)
    {
        // Section to protect with mutex
        pthread_mutex_lock(&mutex_2);
            pthread_mutex_lock(&mutex_1);
                counter_2 --;
                counter_1++;
            pthread_mutex_unlock(&mutex_2);
        pthread_mutex_lock(&mutex_1);
    }

    pthread_exit(NULL);
}
