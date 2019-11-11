//
// Created by Salomón Charabati on 11/11/19.
// Example of threads working on the same variable.
// This program doesn't return the expected result.
// For this to work correctly, we need MUTEXES

#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 3
#define MAX_COUNT 1000000

int counter = 0;

void * increment(void * arg);

int main()
{
    pthread_t tid [NUM_THREADS];

    // Initialize the threads
    for(int i = 0; i< NUM_THREADS; i++)
        pthread_create((&tid[i]),NULL,increment,NULL);

    // Wait for the threads to finish
    for (int j = 0; j < NUM_THREADS; ++j)
        pthread_join(tid[j],NULL);

    printf("The counter is %d / %d\n",counter, NUM_THREADS * MAX_COUNT);

}

void * increment(void * arg)
{
    printf("Thread started\n");
    for (int i = 0; i < MAX_COUNT; ++i)
        counter++;

    pthread_exit(NULL);
}
