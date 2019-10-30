/*
  PI Server - Assignment #6
  Advanced Programming

  Salomón Charabati
*/

#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <unistd.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
// Custom libraries
#include "sockets.h"
#include "fatal_error.h"

#define BUFFER_SIZE 1024
#define MAX_QUEUE 5
#define TIMEOUT_POLL 50

///// FUNCTION DECLARATIONS
void usage(char * program);
void waitForConnections(int server_fd);
void attendRequest(int client_fd);
char * computePI(unsigned long int iterations, int client_fd);
void setupHandlers();
void callOnInterrupt(int signal);

int interrupted = 0;

///// MAIN FUNCTION
int main(int argc, char * argv[])
{
    int server_fd;

    printf("\n=== SERVER FOR COMPUTING THE VALUE OF pi ===\n");

    // Check the correct arguments
    if (argc != 2)
    {
        usage(argv[0]);
    }

    // Set up handlers. Whenever the signal SIG_INT is sent, it will call the callOnInterrupt() function.
    setupHandlers();

	// Show the IPs assigned to this computer
	printLocalIPs();

    // Start the server
    server_fd = initServer(argv[1], MAX_QUEUE);
	// Listen for connections from the clients
    waitForConnections(server_fd);
    // Close the socket
    close(server_fd);

    return 0;
}

///// FUNCTION DEFINITIONS

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

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
    Main loop to wait for incomming connections
*/
void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    int poll_response;
    pid_t new_pid;

    // Polling
    struct pollfd poll_fd[1]; // Array of pollfd structures
    poll_fd[0].fd = server_fd;
    poll_fd[0].events = POLL_IN;

    while (!interrupted)
    {
        // Get the size of the structure to store client information
        client_address_size = sizeof client_address;
        while(1)
        {
            poll_response = poll(poll_fd,1,TIMEOUT_POLL);
            if(poll_response == 0)
                continue;
            else if(poll_response == -1)
            {
                perror("poll");
                break;
            }
            else{
                // ACCEPT
                // Wait for a client connection
                if(poll_fd[0].revents && POLL_IN)
                {
                    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
                    if (client_fd == -1)
                        fatalError("accept");
                    // Get the data from the client
                    inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
                    printf("Received incoming connection from %s on port %d\n", client_presentation, client_address.sin_port);
                    break; // exits the loop after a client connects
                }
            }
        }
       if(!interrupted)
       {
           // FORK
           // Create a new child process to deal with the client
           new_pid = fork();
           // Parent
           if (new_pid > 0)
           {
               // Close the new socket
               close(client_fd);
           }
           else if (new_pid == 0)
           {
               // Close the main server socket to avoid interfering with the parent
               close(server_fd);
               printf("Child process %d dealing with client\n", getpid());
               // Deal with the client
               attendRequest(client_fd);
               // Close the new socket
               close(client_fd);
               // Terminate the child process
               exit(EXIT_SUCCESS);
           }
           else
               fatalError("fork");
       }
    }
}

// Function for computing PI. Polling is needed in this function, because the client might send
// a SIGINT while the server is computing PI
// Approximation to pi using the following formula :
// pi = 4 - (4/3) + (4/5) - (4/7) + (4/9) - ...
char * computePI(unsigned long int iterations,int client_fd)
{
    double result = 4;
    int sign = -1;
    unsigned long int divisor = 3;
    unsigned long int counter = 0;
    int poll_response;
    char buffer[BUFFER_SIZE];
    char resultInString[BUFFER_SIZE];

    // Polling
    struct pollfd poll_fd[1];
    poll_fd[0].fd = client_fd;
    poll_fd[0].events = POLL_IN;

    for (counter = 0; counter<iterations; counter++)
    {
        if(!interrupted) // While the server is not interrupted
        {
            poll_response = poll(poll_fd,1,TIMEOUT_POLL);

            if(poll_response == 0)
            {
                result += sign * (4.0/divisor);
                sign *= -1;
                divisor += 2;
            }else if(poll_response == -1){
                perror("poll");
                break;
            }else{
                if (poll_fd[0].revents & POLLIN) // If there's an event, receive the message sent by the client and break
                {
                    recvString(client_fd, buffer, BUFFER_SIZE);
                    break;
                }
            }
        }else
            break;
    }

    // Need to send the iteration of pi and the result up to that iteration, rather send it as a string
    sprintf(resultInString, "%.20lf||%lu",result,iterations);
    return resultInString;
}

/*
    Hear the request from the client and send an answer
*/
void attendRequest(int client_fd)
{
    char buffer[BUFFER_SIZE];
    unsigned long int iterations_requested = 0;
    unsigned long int iterations_done = 0;
    char * result_in_string;
    double result_in_double = 0;


    // RECV
    // Receive the request
    recvString(client_fd, buffer, BUFFER_SIZE);
    sscanf(buffer, "%lu", &iterations_requested);


    printf(" > Got request from client with iterations=%lu\n", iterations_requested);

    // Compute the value of PI
    result_in_string = computePI(iterations_requested, client_fd);
    printf("%s\n",result_in_string);
    // Divide the string result in corresponding data types
    sscanf(result_in_string, "%lf||%lu", &result_in_double,&iterations_done);

    if(iterations_done < iterations_requested)
        printf("The client disconnected before the server completed computing the value of PI\n");

    printf("> Sending PI=%.20lf with %lu iterations \n", result_in_double, iterations_done);

    // Prepare the response to the client
    sprintf(buffer, "%s", result_in_string);
    // SEND
    // Send the response
    sendString(client_fd, buffer);
}


