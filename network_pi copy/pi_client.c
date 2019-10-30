/*
	Assignment 6: Pi - CLIENT
	Advanced Programming
 
	Gabriel Schlam 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/poll.h>
#include <poll.h>
#include <signal.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
// Custom libraries
#include "sockets.h"
#include "fatal_error.h"


#define BUFFER_SIZE 1024

///// FUNCTION DECLARATIONS
void usage(char * program);
void requestPI(int connection_fd);
void setupHandlers();
void onInterrupt(int signal);

// Global variable for the handler
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
	unsigned long int iterations;
	double result;
	unsigned long int finalIter;

	// Variables for polling
	struct pollfd poll_fd[1];
	int poll_response;
	int timeout = 1000; // Timeout of one second

	// Prepare for the poll
	poll_fd[0].fd = connection_fd;
	poll_fd[0].events = POLLIN;

	printf("Enter the number of iterations for PI: ");
	scanf("%lu", &iterations);

	// Prepare the response to the client
	sprintf(buffer, "%lu", iterations);
	// SEND
	// Send the response
	sendString(connection_fd, buffer);

	while(!interrupted)
	{		
		while(1)
		{
			poll_response = poll(poll_fd, 1, timeout);

			if (poll_response == 0) // Nothing to receive
			{
				continue;
			}

			else if (poll_response == -1)
			{
				perror("poll");
				// Exit the inner loop
				break;
			}

			else
			{
				if (poll_fd[0].revents & POLLIN)
				{
					// RECV
					// Receive the request
					recvString(connection_fd, buffer, BUFFER_SIZE);
					break;
				}
			}
		}

		if (interrupted)
		{
			sprintf(buffer, "INTERRUPT");
			// SEND
			// Send the response
			sendString(connection_fd, buffer);

			recvString(connection_fd, buffer, BUFFER_SIZE);
			break;
		}

		else
		{
			break;
		}
	}

	// Get Result and Final Iterations
	sscanf(buffer, "%lf:%lu", &result, &finalIter);

	if (finalIter < iterations && !interrupted)
	{
		printf("The server disconnected and sent the current result!\n");
	}

	// Print the result
	printf("The value for PI is: %.20lf\n", result);
	printf("The number of iterations made: %lu\n", finalIter);

}

// Define signal handlers
void setupHandlers()
{
	struct sigaction new_action;

	new_action.sa_handler = onInterrupt;
	sigaction(SIGINT, &new_action, NULL);
}

// Signal handler
void onInterrupt(int signal)
{
	interrupted = 1;
}
