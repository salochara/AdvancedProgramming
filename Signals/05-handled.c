/*
   This is an example of the method signal() and sigaction()
   This is for handling signals. Example 07 is for blocking signals
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define LIFETIME 5

void waitLoop();
void callOnInterrupt(int signal);

int main(int argc, char * argv[])
{
    int seconds;
    struct sigaction new_action;
    struct sigaction old_action;


    // Use a timeout obtained as an argument
    if (argc >= 2)
        seconds = atoi(argv[1]);
        // Otherwise use a default time
    else
        seconds = LIFETIME;

    //signal(SIGINT,callOnInterrupt); // ignore the signal SIGINT (ctrl+c)
                                    // this is a callback function and now, it's not being called. Also we are not passing the
                                    // argument it's expecting, because C calls it whenever SIGINT signal is sent. SIGINT is then
                                    // passed to to function callOnInterrupt as an argument



    new_action.sa_handler = callOnInterrupt;
    new_action.sa_flags = SA_RESETHAND; // use only once the first handler
    sigaction(SIGINT,&new_action,&old_action);
    // Wait a few seconds before quiting on its own
    waitLoop(seconds);

    // Sigaction is more reliable than signal()
    sigaction(SIGINT,&old_action,NULL);

    //signal(SIGINT, SIG_DFL); // set the default operation for signal SIGINT. i.e., now you can kill process with ctrl+c

    // Wait a few seconds before quiting on its own
    waitLoop(seconds);

    return 0;
}

// Better signal handler



// Signal handler. Callback function.
void callOnInterrupt(int signal)
{
    printf("\nThe signal %d has been received\n",signal);
}


// Wait a few seconds until the program ends
void waitLoop(int seconds)
{
    int i;

    // Show the PID so that it can be sent signals form the terminal
    //  or other process
    printf("My PID is: %d\n", getpid());

    printf("Will finish on my own in %3d s", seconds);
    fflush(stdout);
    // Loop to wait for something to happen
    for (i=seconds-1; i>=0; i--)
    {
        sleep(1);
        printf("\b\b\b\b\b%3d s", i);
        fflush(stdout);
    }
    printf("\nTime out. Good bye\n");
}

