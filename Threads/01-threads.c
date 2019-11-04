//
// First example of threads
// 4-11-19
// This program might only print "this is the main thread" only because we cannot guarantee that the new thread will be called
// before the main program finishes. Unless, we call pthread_exit which waits for threads.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// This function returns a pointer
void * threadStart(void * arg);

int main()
{
    pthread_t tid = NULL;

    // Create a new thread with the function
    // Address of thread, defines attributes for thread (commonly NULL), starting point function (has to be void), parameters to the function
    // new thread is stored in tid
    pthread_create(&tid,NULL,threadStart,NULL);
    printf("This is the main thread\n");

    // waits for threads called in _create to terminate before it exits
    pthread_exit(NULL); // we guarantee that tid has enough time to run
}

void * threadStart(void * arg)
{
    printf("This is the secondary thread\n");
    return NULL; // because it returns a pointer. Void here means that returns a pointer to anything... doesn't mean that
                 // it doesn't returns anything
}
