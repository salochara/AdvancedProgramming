/*
	Assignment 6: Pi - SERVER
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
#define MAX_QUEUE 5

///// FUNCTION DECLARATIONS
void usage(char * program);
void waitForConnections(int server_fd);
void attendRequest(int client_fd);
char * computePI(unsigned long int iterations, char res[BUFFER_SIZE], int client_fd);
void setupHandlers();
void onInterrupt(int signal);

// Global variable for the handler
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


/*
	Main loop to wait for incomming connections
*/
void waitForConnections(int server_fd)
{
	struct sockaddr_in client_address;
	socklen_t client_address_size;
	char client_presentation[INET_ADDRSTRLEN];
	int client_fd;
	pid_t new_pid;

	// Variables for polling
	struct pollfd poll_fd[1];
	int poll_response;
	int timeout = 100; // Timeout of one second

	// Prepare for the poll
	poll_fd[0].fd = server_fd;
	poll_fd[0].events = POLLIN;

	while(!interrupted)
	{
		///// ACCEPT
		// Receive incomming connections
		// Get the size of the structure where the address of the client will be stored
		client_address_size = sizeof client_address;
		
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
				printf("A client wants to connect\n");
				if (poll_fd[0].revents & POLLIN)
				{
					// ACCEPT
					// Wait for a client connection
					client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);

					// Get the data from the client
					inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
					printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);
					break;
				}
			}
		}

		if (!interrupted)
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
			{
				fatalError("fork");
			}
		}
	}
}

/*
	Hear the request from the client and send an answer
*/
void attendRequest(int client_fd)
{
	char buffer[BUFFER_SIZE];
	char res[BUFFER_SIZE];
	unsigned long int iterations;
	char * result;
	double finalRes;
	unsigned long int finalIter;

	// RECV
	// Receive the request
	recvString(client_fd, buffer, BUFFER_SIZE);
	sscanf(buffer, "%lu", &iterations);

	printf(" > Got request from client with iterations=%lu\n", iterations);

	// Compute the value of PI
	result = computePI(iterations, res, client_fd);

	// Get Result and Final Iterations
	sscanf(result, "%lf:%lu", &finalRes, &finalIter);

	printf(" > Sending PI=%.20lf with %lu iterations\n", finalRes, finalIter);

	// Prepare the response to the client
	sprintf(buffer, "%s", result);
	// SEND
	// Send the response
	sendString(client_fd, buffer);
}

char * computePI(unsigned long int iterations, char res[BUFFER_SIZE], int client_fd)
{
	char buffer[BUFFER_SIZE];
	double result = 4;
	int sign = -1;
	unsigned long int divisor = 3;
	unsigned long int counter = 0;

	// Variables for polling
	struct pollfd poll_fd[1];
	int poll_response;
	int timeout = 20;

	// Prepare for the poll
	poll_fd[0].fd = client_fd;
	poll_fd[0].events = POLLIN;

	for (counter = 0; counter<iterations; counter++)
	{
		if (!interrupted)
		{
			poll_response = poll(poll_fd, 1, timeout);

			if (poll_response == 0) // Nothing to receive
			{
				result += sign * (4.0/divisor);
				sign *= -1;
				divisor += 2;
			}

			else if (poll_response == -1)
			{
				perror("poll");
				break;
			}

			else
			{
				if (poll_fd[0].revents & POLLIN)
				{
					recvString(client_fd, buffer, BUFFER_SIZE);
					printf("Client disconnecting!\n");
					break;
				}
			}
		}

		else
		{
			break;	
		}
	}

	sprintf(res, "%.20lf:%lu", result, counter);

	return res;
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
