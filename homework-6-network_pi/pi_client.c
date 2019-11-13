/*
  PI Client - Assignment #6
  Advanced Programming

  Salomón Charabati
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <poll.h>
// Custom libraries
#include "sockets.h"
#include "fatal_error.h"


#define BUFFER_SIZE 1024
#define TIMEOUT_POLL 100

///// FUNCTION DECLARATIONS
void usage(char * program);
void requestPI(int connection_fd);
void callOnInterrupt(int signal);
void setupHandlers();

int interrupted = 0;

///// MAIN FUNCTION
int main(int argc, char * argv[])
{
    int connection_fd;

    printf("\n=== CLIENT FOR COMPUTING THE VALUE OF pi ===\n");

    // Check the correct arguments
    if (argc != 3)
    {
        usage(argv[0]);
    }
    // Set up handlers. Whenever the signal SIG_INT is sent, it will call the callOnInterrupt() function.
    setupHandlers();
    // Start the server
    connection_fd = connectSocket(argv[1], argv[2]);
	// Listen for connections from the clients
    requestPI(connection_fd);
    // Close the socket
    close(connection_fd);

    return 0;
}

///// FUNCTION DEFINITIONS

void callOnInterrupt(int signal)
{
    interrupted = 1;
}

void setupHandlers()
{
    struct sigaction new_action;
    new_action.sa_handler = callOnInterrupt;
    sigaction(SIGINT, &new_action, NULL);
}

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

void requestPI(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    unsigned long int iterations_requested = 0;
    unsigned long int iterations_done = 0;
    double result = 0;

    // Polling
    struct pollfd poll_fd[1]; // Array of pollfd structures
    poll_fd[0].fd = connection_fd;
    poll_fd[0].events = POLL_IN;
    int poll_response;

    printf("Enter the number of iterations for PI: ");
    scanf("%lu", &iterations_requested);

    // Prepare the response to the client
    sprintf(buffer, "%lu", iterations_requested);
    // SEND
    // Send the response
    sendString(connection_fd, buffer);

    // Main loop. While the client is not interrupted
    while(!interrupted)
    {
        while(1)
        {
            poll_response = poll(poll_fd, 1, 100);
            // Loop for polling. Quite similar to the the one in pi_server.c
            if (poll_response == 0)
                continue;
            else if (poll_response == -1){
                perror("poll");
                break;
            }
            else{
                if (poll_fd[0].revents & POLLIN)
                {
                    recvString(connection_fd, buffer, BUFFER_SIZE);
                    break;
                }
            }
        }
        if (interrupted)
        {
            sprintf(buffer, "interrupt");
            sendString(connection_fd, buffer);

            recvString(connection_fd, buffer, BUFFER_SIZE);
            break;
        } else // If it's not interrupted, simply break the !interrupted loop
            break;
    }

    // RECV
    // Receive the request
    sscanf(buffer, "%lf||%lu", &result, &iterations_done);

    if(iterations_done < iterations_requested )
      printf("The client disconnected before the server completed computing the value of PI\n");

    // Print the result
    printf("The value for PI is: %.20lf with %lu iterations \n", result,iterations_done);
}
