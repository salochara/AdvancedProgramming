/*
	Assignment 8: Bank - SERVER
	Advanced Programming
 
	Gabriel Schlam 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Signals library
#include <errno.h>
#include <signal.h>
// Sockets libraries
#include <netdb.h>
#include <sys/poll.h>
// Posix threads library
#include <pthread.h>

// Custom libraries
#include "bank_codes.h"
#include "sockets.h"
#include "fatal_error.h"

#define MAX_ACCOUNTS 5
#define BUFFER_SIZE 1024
#define MAX_QUEUE 5

///// Structure definitions
// Data for a single bank account
typedef struct account_struct {
	int id;
	int pin;
	float balance;
} account_t;

// Data for the bank operations
typedef struct bank_struct {
	// Store the total number of operations performed
	int total_transactions;
	// An array of the accounts
	account_t * account_array;
} bank_t;

// Structure for the mutexes to keep the data consistent
typedef struct locks_struct {
	// Mutex for the number of transactions variable
	pthread_mutex_t transactions_mutex;
	// Mutex array for the operations on the accounts
	pthread_mutex_t * account_mutex;
} locks_t;

// Data that will be sent to each structure
typedef struct data_struct {
	// The file descriptor for the socket
	int connection_fd;
	// A pointer to a bank data structure
	bank_t * bank_data;
	// A pointer to a locks structure
	locks_t * data_locks;
} thread_data_t;

// Global variables for signal handlers
int interrupt_exit = 0;

///// FUNCTION DECLARATIONS
void usage(char * program);
void setupHandlers();
void initBank(bank_t * bank_data, locks_t * data_locks);
void readBankFile(bank_t * bank_data);
void waitForConnections(int server_fd, bank_t * bank_data, locks_t * data_locks);
void * attentionThread(void * arg);
void closeBank(bank_t * bank_data, locks_t * data_locks);
int checkValidAccount(int account);

// Add your function declarations here
void onInterrupt(int signal);
void operationCheck(int connection_fd, bank_t * bank_data, locks_t * data_locks, int account_from);
void operationDeposit(int connection_fd, bank_t * bank_data, locks_t * data_locks, int account_to, float amount);
void operationWithdraw(int connection_fd, bank_t * bank_data, locks_t * data_locks, int account_from, float amount);
void operationTransfer(int connection_fd, bank_t * bank_data, locks_t * data_locks, int account_from, int account_to, float amount);
void writeBankFile(bank_t * bank_data);

///// MAIN FUNCTION
int main(int argc, char * argv[])
{
	int server_fd;
	bank_t bank_data;
	locks_t data_locks;

	printf("\n=== SIMPLE BANK SERVER ===\n");

	// Check the correct arguments
	if (argc != 2)
	{
		usage(argv[0]);
	}

	// Configure the handler to catch SIGINT
	setupHandlers();

	// Initialize the data structures
	initBank(&bank_data, &data_locks);

	// Show the IPs assigned to this computer
	printLocalIPs();
	// Start the server
	server_fd = initServer(argv[1], MAX_QUEUE);
	// Listen for connections from the clients
	waitForConnections(server_fd, &bank_data, &data_locks);
	// Close the socket
	close(server_fd);

	// Clean the memory used
	closeBank(&bank_data, &data_locks);

	// Finish the main thread
	pthread_exit(NULL);

	return 0;
}

///// FUNCTION DEFINITIONS

// Explanation to the user of the parameters required to run the program
void usage(char * program)
{
	printf("Usage:\n");
	printf("\t%s {port_number}\n", program);
	exit(EXIT_FAILURE);
}

///Modify the signal handlers for specific events
void setupHandlers()
{
	struct sigaction new_action;

	new_action.sa_handler = onInterrupt;
	sigaction(SIGINT, &new_action, NULL);
}

/*
	Function to initialize all the information necessary
	This will allocate memory for the accounts, and for the mutexes
*/
void initBank(bank_t * bank_data, locks_t * data_locks)
{
	// Set the number of transactions
	bank_data->total_transactions = 0;

	// Allocate the arrays in the structures
	bank_data->account_array = malloc(MAX_ACCOUNTS * sizeof (account_t));
	// Allocate the arrays for the mutexes
	data_locks->account_mutex = malloc(MAX_ACCOUNTS * sizeof (pthread_mutex_t));

	// Initialize the mutexes, using a different method for dynamically created ones
	//data_locks->transactions_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&data_locks->transactions_mutex, NULL);
	for (int i=0; i<MAX_ACCOUNTS; i++)
	{
		//data_locks->account_mutex[i] = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_init(&data_locks->account_mutex[i], NULL);
		// Initialize the account balances too
		bank_data->account_array[i].balance = 0.0;
	}

	// Read the data from the file
	readBankFile(bank_data);
}

// Get the data from the file to initialize the accounts
void readBankFile(bank_t * bank_data)
{
	FILE * file_ptr = NULL;
	char buffer[BUFFER_SIZE];
	int account = 0;
	char * filename = "accounts.txt";

	file_ptr = fopen(filename, "r");
	if (!file_ptr)
	{
		fatalError("ERROR: fopen");
	}

	// Ignore the first line with the headers
	fgets(buffer, BUFFER_SIZE, file_ptr);
	// Read the rest of the account data
	while( fgets(buffer, BUFFER_SIZE, file_ptr) )
	{
		sscanf(buffer, "%d %d %f", &bank_data->account_array[account].id, &bank_data->account_array[account].pin, &bank_data->account_array[account].balance); 
		account++;
	}
	
	fclose(file_ptr);
}

// Main loop to wait for incomming connections
void waitForConnections(int server_fd, bank_t * bank_data, locks_t * data_locks)
{
	struct sockaddr_in client_address;
	socklen_t client_address_size;
	char client_presentation[INET_ADDRSTRLEN];
	int client_fd;
	pthread_t new_tid;
	thread_data_t * connection_data = NULL;
	int poll_response;
	int timeout = 500;		// Time in milliseconds (0.5 seconds)

	// Create a structure array to hold the file descriptors to poll
	struct pollfd test_fds[1];
	// Fill in the structure
	test_fds[0].fd = server_fd;
	test_fds[0].events = POLLIN;    // Check for incomming data

	while(!interrupt_exit)
	{
		// Get the size of the structure to store client information
		client_address_size = sizeof client_address;

		while (1)
		{
			poll_response = poll(test_fds, 1, timeout);

			if (poll_response == 0) // Nothing to receive
				continue;

			else if (poll_response == -1)
			{
				perror("poll");
				// Exit the inner loop
				break;
			}

			else
			{
				if (test_fds[0].revents & POLLIN)
				{
					// ACCEPT
					// Wait for a client connection
					client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
					if (client_fd == -1)
					{
						fatalError("ERROR: accept");
					}
					 
					// Get the data from the client
					inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
					printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);
					break;
				}
			}
		}

		if (!interrupt_exit)
		{
			// Dynamic allocation of the thread connection data
			connection_data = malloc(sizeof(thread_data_t));

			// Prepare the structure to send to the thread
			connection_data->connection_fd = client_fd;
			connection_data->bank_data = bank_data;
			connection_data->data_locks = data_locks;

			// CREATE A THREAD
			pthread_create(&new_tid, NULL, attentionThread, connection_data);
		}
	}

	if (client_fd < 0)
	{
		printf("Server closing without any client connection\n");
	}

	else
	{
		// Show the number of total transactions
		bank_data = connection_data->bank_data;
		printf("NUMBER OF TOTAL TRANSACTIONS MADE: %d\n", bank_data->total_transactions);

		// Store any changes in the file
		writeBankFile(bank_data);
	}
}

// Hear the request from the client and send an answer
void * attentionThread(void * arg)
{
	char buffer[BUFFER_SIZE];
	int operation;
	int account_from;
	int account_to;
	float amount;
	response_t status;

	int connection_fd;
	bank_t * bank_data;
	locks_t * data_locks;

	// Receive the data for the bank, mutexes and socket file descriptor
	thread_data_t * actual_data = (thread_data_t *) arg;
	connection_fd = actual_data->connection_fd;
	bank_data = actual_data->bank_data;
	data_locks = actual_data->data_locks;

	//printf("DEBUG: connection FD: %d\n", connection_fd);

	// Loop to listen for messages from the client
	while(1)
	{
		// Receive the request
		recvString(connection_fd, buffer, BUFFER_SIZE);

		// Get data from client
		sscanf(buffer, "%d %d %d %f", &operation, &account_from, &account_to, &amount);

		// Processing the request being careful of data consistency

		// Checking which operation was selected by client
		if (operation == CHECK)
		{
			if (!checkValidAccount(account_from)) // If the number of account is not valid
			{
				// Set response
				status = NO_ACCOUNT;

				// Send a reply
				sprintf(buffer, "%d %f", status, 0.0);
				sendString(connection_fd, buffer, strlen(buffer)+1);
			}
			else
			{
				operationCheck(connection_fd, bank_data, data_locks, account_from);
			}
		}

		else if (operation == DEPOSIT)
		{
			if (!checkValidAccount(account_to)) // If the number of account is not valid
			{
				// Set response
				status = NO_ACCOUNT;

				// Send a reply
				sprintf(buffer, "%d %f", status, 0.0);
				sendString(connection_fd, buffer, strlen(buffer)+1);
			}
			else
			{
				operationDeposit(connection_fd, bank_data, data_locks, account_to, amount);
			}	
		}

		else if (operation == WITHDRAW)
		{
			if (!checkValidAccount(account_from)) // If the number of account is not valid
			{
				// Set response
				status = NO_ACCOUNT;

				// Send a reply
				sprintf(buffer, "%d %f", status, 0.0);
				sendString(connection_fd, buffer, strlen(buffer)+1);
			}
			else
			{
				operationWithdraw(connection_fd, bank_data, data_locks, account_from, amount);
			}
		}

		else if (operation == TRANSFER)
		{
			if (!checkValidAccount(account_from) || !checkValidAccount(account_to))
			{
				// Set response
				status = NO_ACCOUNT;

				// Send a reply
				sprintf(buffer, "%d %f", status, 0.0);
				sendString(connection_fd, buffer, strlen(buffer)+1);
			}
			else
			{
				operationTransfer(connection_fd, bank_data, data_locks, account_from, account_to, amount);
			}
		}

		else if(operation == EXIT)
		{
			printf("Client is finishing\n");

			// Set response
			status = BYE;

			// Send a reply
			sprintf(buffer, "%d %f", status, 0.0);
			sendString(connection_fd, buffer, strlen(buffer)+1);
			break;
		}

		else
		{
			// Set response
			status = ERROR;

			// Send a reply
			sprintf(buffer, "%d %f", status, 0.0);
			sendString(connection_fd, buffer, strlen(buffer)+1);
		}
	}

	close(connection_fd);

	pthread_exit(NULL);
}

// Free all the memory used for the bank data
void closeBank(bank_t * bank_data, locks_t * data_locks)
{
	printf("DEBUG: Clearing the memory for the thread\n");
	free(bank_data->account_array);
	free(data_locks->account_mutex);
}

/*
	Return true if the account provided is within the valid range,
	return false otherwise
*/
int checkValidAccount(int account)
{
	return (account >= 0 && account < MAX_ACCOUNTS);
}

// Signal handler
void onInterrupt(int signal)
{
	interrupt_exit = 1;
}

// Function for checking balance
void operationCheck(int connection_fd, bank_t * bank_data, locks_t * data_locks, int account_from)
{
	char buffer[BUFFER_SIZE];
	float balance;
	response_t status;

	printf("Performing operation: Check\n");

	// Locking while checking the balance of account
	pthread_mutex_lock(&data_locks->account_mutex[account_from]);
		balance = bank_data->account_array[account_from].balance;
	pthread_mutex_unlock(&data_locks->account_mutex[account_from]);

	// Set response
	status = OK;

	// Update the number of transactions, while locking the mutex
	pthread_mutex_lock(&data_locks->transactions_mutex);
		bank_data->total_transactions = bank_data->total_transactions + 1;
	pthread_mutex_unlock(&data_locks->transactions_mutex);

	printf("Balanced checked in account %d\n", account_from);

	// Send a reply
	sprintf(buffer, "%d %f", status, balance);
	sendString(connection_fd, buffer, strlen(buffer)+1);

	printf("Operation finished\n");
}

void operationDeposit(int connection_fd, bank_t * bank_data, locks_t * data_locks, int account_to, float amount)
{
	char buffer[BUFFER_SIZE];
	float balance;
	response_t status;

	printf("Performing operation: Deposit\n");

	// Locking while depositing the amount in account
	pthread_mutex_lock(&data_locks->account_mutex[account_to]);
		balance = bank_data->account_array[account_to].balance + amount;
		bank_data->account_array[account_to].balance = balance;
	pthread_mutex_unlock(&data_locks->account_mutex[account_to]);

	// Set response
	status = OK;

	// Update the number of transactions, while locking the mutex
	pthread_mutex_lock(&data_locks->transactions_mutex);
		bank_data->total_transactions = bank_data->total_transactions + 1;
	pthread_mutex_unlock(&data_locks->transactions_mutex);

	printf("$%.2f deposited in account %d\n", amount, account_to);

	// Send a reply
	sprintf(buffer, "%d %f", status, balance);
	sendString(connection_fd, buffer, strlen(buffer)+1);

	printf("Operation finished\n");
}

void operationWithdraw(int connection_fd, bank_t * bank_data, locks_t * data_locks, int account_from, float amount)
{
	char buffer[BUFFER_SIZE];
	float balance;
	response_t status;

	printf("Performing operation: Withdraw\n");

	// Check if account has sufficient fonds
	if (bank_data->account_array[account_from].balance < amount)
	{
		printf("Insufficient fonds in account %d\n", account_from);
		printf("Operation denied\n");

		// Set response
		status = INSUFFICIENT;

		// Locking while checking the balance of account, for having an accurate balance
		pthread_mutex_lock(&data_locks->account_mutex[account_from]);
			balance = bank_data->account_array[account_from].balance;
		pthread_mutex_unlock(&data_locks->account_mutex[account_from]);

		// Send a reply
		sprintf(buffer, "%d %f", status, balance);
		sendString(connection_fd, buffer, strlen(buffer)+1);
	}
	else
	{
		// Locking while withdrawing the amount from account
		pthread_mutex_lock(&data_locks->account_mutex[account_from]);
			balance = bank_data->account_array[account_from].balance - amount;
			bank_data->account_array[account_from].balance = balance;
		pthread_mutex_unlock(&data_locks->account_mutex[account_from]);

		// Set response
		status = OK;

		// Update the number of transactions, while locking the mutex
		pthread_mutex_lock(&data_locks->transactions_mutex);
			bank_data->total_transactions = bank_data->total_transactions + 1;
		pthread_mutex_unlock(&data_locks->transactions_mutex);

		printf("$%.2f withdrawn from account %d\n", amount, account_from);

		// Send a reply
		sprintf(buffer, "%d %f", status, balance);
		sendString(connection_fd, buffer, strlen(buffer)+1);

		printf("Operation finished\n");
	}
}

void operationTransfer(int connection_fd, bank_t * bank_data, locks_t * data_locks, int account_from, int account_to, float amount)
{
	char buffer[BUFFER_SIZE];
	float balance;
	response_t status;

	printf("Performing operation: Transfer\n");

	// Check if source account has sufficient fonds
	if (bank_data->account_array[account_from].balance < amount)
	{
		printf("Insufficient fonds in account %d\n", account_from);
		printf("Operation denied\n");

		// Set response
		status = INSUFFICIENT;

		// Locking while checking the balance of account, for having an accurate balance
		pthread_mutex_lock(&data_locks->account_mutex[account_from]);
			balance = bank_data->account_array[account_from].balance;
		pthread_mutex_unlock(&data_locks->account_mutex[account_from]);

		// Send a reply
		sprintf(buffer, "%d %f", status, balance);
		sendString(connection_fd, buffer, strlen(buffer)+1);
	}
	else
	{
		// Locking while accessing to both source and destination account
		pthread_mutex_lock(&data_locks->account_mutex[account_to]);
		
		while(pthread_mutex_trylock(&data_locks->account_mutex[account_from]))
		{
			pthread_mutex_unlock(&data_locks->account_mutex[account_to]);
			pthread_mutex_lock(&data_locks->account_mutex[account_to]);
		}
			// Transfering amount between accounts
			bank_data->account_array[account_to].balance = bank_data->account_array[account_to].balance + amount;
			balance = bank_data->account_array[account_from].balance - amount;
			bank_data->account_array[account_from].balance = balance;

		pthread_mutex_unlock(&data_locks->account_mutex[account_to]);
		pthread_mutex_unlock(&data_locks->account_mutex[account_from]);

		// Set response
		status = OK;

		// Update the number of transactions, while locking the mutex
		pthread_mutex_lock(&data_locks->transactions_mutex);
			bank_data->total_transactions = bank_data->total_transactions + 1;
		pthread_mutex_unlock(&data_locks->transactions_mutex);

		printf("$%.2f transfered from account %d to account %d\n", amount, account_from, account_to);

		// Send a reply
		sprintf(buffer, "%d %f", status, balance);
		sendString(connection_fd, buffer, strlen(buffer)+1);

		printf("Operation finished\n");
	}
}

// Write the updated data to the file
void writeBankFile(bank_t * bank_data)
{
	FILE * file_ptr = NULL;
	int account = 0;
	char buffer[BUFFER_SIZE];
	char * filename = "accounts.txt";

	file_ptr = fopen(filename, "r+");
	if (!file_ptr)
	{
		fatalError("ERROR: fopen");
	}

	// Ignore the first line with the headers
	fgets(buffer, BUFFER_SIZE, file_ptr);

	// Store position where headers are
	int position_in_file = ftell(file_ptr) - 1;

	// Get actual number of accounts in file.
	while(fgets(buffer, BUFFER_SIZE, file_ptr) )
	{
		account++;
	}

	// Go back to position where headers are
	fseek(file_ptr, position_in_file, SEEK_SET);
	
	// Write the updated data
	for (int i = 0; i < account; i++)
	{
		fprintf(file_ptr, "\n%d %d %f", bank_data->account_array[i].id, bank_data->account_array[i].pin, bank_data->account_array[i].balance);
	}
	
	fclose(file_ptr);
}

