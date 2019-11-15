/*
    Program for a simple bank server
    It uses sockets and threads
    The server will process simple transactions on a limited number of accounts

    Gilberto Echeverria
    gilecheverria@yahoo.com
    29/03/2018
    08/04/2018  Added initialization of account balances
    27/10/2018  Simplify the function 'processOperation' by validating the account before the switch
    14/03/2019  Added the new operation for transfer from one account to another

    Salomón Charabati
    salochara@gmail.com
    Assignment #7. Advanced Programming at ITESM.
    13/11/2019  Added functionality and implemented a client/server bank using threads
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
#define TIMEOUT_POLL 50
#define RESULT_FILE_NAME "result.txt";
//#define INPUT_FILE_NAME "accounts.txt";

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
    // Stores the number of active accounts
    int active_bank_accounts;
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
void callOnInterrupt(int signal);
void writeBankFile(thread_data_t * connection_data);
char *switchForOperations(locks_t *data_locks, bank_t *bank_data, int account_to, float amount, int operation,
                          int account_from);
void lockMutexTransactionsAndSumToTransactions(locks_t *data_locks, bank_t *bank_data);
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

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

void writeBankFile(thread_data_t * connection_data)
{
    FILE * file_ptr = NULL;
    char * filename = RESULT_FILE_NAME;
    int numberOfAccounts = 0;

    file_ptr = fopen(filename, "w");
    if (!file_ptr)
        fatalError("ERROR: fopen");

    // Set the headers for the result file
    fprintf(file_ptr,"Account_number PIN Balance \n");
    printf("Writing into file...\n");

    // Retrieve the active accounts number from the bank data structure
    numberOfAccounts = connection_data->bank_data->active_bank_accounts;

    for (int i = 0; i < numberOfAccounts; ++i)
    {
        printf("%d %d %.2f \n", connection_data->bank_data->account_array[i].id, connection_data->bank_data->account_array[i].pin, connection_data->bank_data->account_array[i].balance);
        fprintf(file_ptr, "%d %d %.2f \n", connection_data->bank_data->account_array[i].id, connection_data->bank_data->account_array[i].pin, connection_data->bank_data->account_array[i].balance);
    }

    fclose(file_ptr);
    printf("FINISHED WRITING RESULT FILE\n");
}

/*
    Modify the signal handlers for specific events
*/
void setupHandlers()
{
    struct sigaction new_action;
    new_action.sa_handler = callOnInterrupt;
    sigaction(SIGINT, &new_action, NULL);
}

void callOnInterrupt(int signal)
{
    printf("Gracefully closing the server because of a SIG_INT signal sent\n");
    interrupt_exit = 1;
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

/*
    Get the data from the file to initialize the accounts
*/
void readBankFile(bank_t * bank_data)
{
    FILE * file_ptr = NULL;
    char buffer[BUFFER_SIZE];
    int account = 0;
    char * filename = "accounts.txt";

    file_ptr = fopen(filename, "r");
    if (!file_ptr)
        fatalError("ERROR: fopen");

    // Ignore the first line with the headers
    fgets(buffer, BUFFER_SIZE, file_ptr);
    // Read the rest of the account data
    while( fgets(buffer, BUFFER_SIZE, file_ptr) )
    {
        sscanf(buffer, "%d %d %f", &bank_data->account_array[account].id, &bank_data->account_array[account].pin, &bank_data->account_array[account].balance); 
        account++;
    }

    // Save the active accounts in the bank_data structure
    bank_data->active_bank_accounts = account;
    
    fclose(file_ptr);
}

/*
    Main loop to wait for incomming connections
*/
void waitForConnections(int server_fd, bank_t * bank_data, locks_t * data_locks)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pthread_t new_tid;
    thread_data_t * connection_data = NULL;
    int poll_response;

    // Create a structure array to hold the file descriptors to poll and fill the struct
    struct pollfd test_fds[1];
    test_fds[0].fd = server_fd;
    test_fds[0].events = POLLIN;

    while(!interrupt_exit)
    {
        // Get the size of the structure to store client information
        client_address_size = sizeof client_address;
        while (1)
        {
            // POLLING
            poll_response = poll(test_fds,1,TIMEOUT_POLL);

            if(poll_response == 0)
                continue;
            else if(poll_response == -1)
            {
                perror("poll");
                break;
            }else{
                if(test_fds[0].revents && POLL_IN)
                {
                    // ACCEPT
                    // Wait for a client connection
                    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
                    if (client_fd == -1)
                        fatalError("ERROR: accept");

                    // Get the data from the client
                    inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
                    printf("Received incoming connection from %s on port %d\n", client_presentation, client_address.sin_port);

                    break;
                }
            }
        }
        // Dynamic allocation for the connection data. A pointer to the thread_data_t struct
        connection_data = malloc(sizeof(thread_data_t));

        // Prepare structure to send to thread.
        connection_data->connection_fd = client_fd;
        connection_data->bank_data = bank_data;
        connection_data->data_locks = data_locks;

        // CREATE A THREAD
        printf("Creating a new thread!\n");
        pthread_create(&new_tid,NULL,attentionThread,connection_data);


    }
    // Only if there were any connections
    if(connection_data != NULL)
    {
        // Show the number of total transactions
        printf("Total transactions done: %d from the user connected with (%s) on port %d\n", connection_data->bank_data->total_transactions, client_presentation, client_address.sin_port);

        // Store any changes in the file
        writeBankFile(connection_data);
    }


}

/*
    Hear the request from the client and send an answer
*/
void * attentionThread(void * arg)
{
    int operation, account_from, account_to;
    float amount;
    char buffer[BUFFER_SIZE];
    response_t response;
    float balance = 0;


    // Receive the data for the bank, mutexes and socket file descriptor
    thread_data_t * threadData = (thread_data_t*) arg;
    int connection_fd = threadData->connection_fd;
    locks_t * data_locks = threadData->data_locks;
    bank_t * bank_data = threadData->bank_data;

    // Loop to listen for messages from the client
    while(1)
    {
        // Receive the request
        recvString(connection_fd,buffer,BUFFER_SIZE);

        // Process the request being careful of data consistency
        sscanf(buffer, "%d %d %d %f", &operation,&account_from,&account_to,&amount);
        printf("> Operation: %d\t From: %d \t To: %d Amount: %f\n",operation,account_from,account_to,amount);

        if(operation == EXIT)
        {
            printf("Client wants to EXIT, writing to file\n");
            pthread_mutex_lock(&data_locks->transactions_mutex);
                printf("Number of transactions done: %d\n",bank_data->total_transactions);
            pthread_mutex_unlock(&data_locks->transactions_mutex);
            break;
        }

        // Process the request from the client and prepare a reply
        if(checkValidAccount(account_to) && checkValidAccount(account_from))
        {
            switch (operation)
            {
                case CHECK:

                    pthread_mutex_lock(&data_locks->account_mutex[account_from]);

                        balance = bank_data->account_array[account_from].balance;
                        response = OK;
                        printf("BALANCE IS %f", balance);
                        sprintf(buffer, "%d %f", response, balance);


                    pthread_mutex_unlock(&data_locks->account_mutex[account_from]);

                    // Successful transaction. Sum one to the transactions done
                    lockMutexTransactionsAndSumToTransactions(data_locks, bank_data);
                    break;

                case DEPOSIT:
                    pthread_mutex_lock(&data_locks->account_mutex[account_to]);

                        // Sum amount to the balance of the account to deposit.
                        balance = bank_data->account_array[account_to].balance;
                        balance += amount;

                        printf("New balance after DEPOSIT operation is %f\n", balance);
                        sprintf(buffer, "%d %f", OK, balance);

                    pthread_mutex_unlock(&data_locks->account_mutex[account_to]);

                    // Successful transaction. Sum one to the transactions done
                    lockMutexTransactionsAndSumToTransactions(data_locks, bank_data);
                    break;

                case WITHDRAW:
                    pthread_mutex_lock(&data_locks->account_mutex[account_from]);

                        balance = bank_data->account_array[account_from].balance;
                        // Check for sufficient funds to withdraw
                        if (balance >= amount) {
                            balance -= amount;
                            printf("New balance after WITHDRAW operation is %f\n",balance);
                            sprintf(buffer, "%d %f", OK, balance);

                            // Successful transaction. Sum one to the transactions done
                            lockMutexTransactionsAndSumToTransactions(data_locks, bank_data);
                        } else
                            sprintf(buffer, "%d %f", INSUFFICIENT,balance);

                    pthread_mutex_unlock(&data_locks->account_mutex[account_from]);
                    break;

                case TRANSFER:
                    pthread_mutex_lock(&data_locks->account_mutex[account_from]);

                    // Loop while the mutex is locked by another thread
                    while (pthread_mutex_trylock(&data_locks->account_mutex[account_to])) {
                        // While account to mutex is not locked, release account from for other threads and
                        // immediately after lock it again.
                        pthread_mutex_unlock(&data_locks->account_mutex[account_from]);
                        pthread_mutex_lock(&data_locks->account_mutex[account_from]);
                    }


                    // Transfer operation begins
                    float sourceAccountBalance = bank_data->account_array[account_from].balance;
                    float destinationAccountBalance = bank_data->account_array[account_to].balance;
                    // Check for sufficient funds to transfer & amount > 0
                    if (sourceAccountBalance >= amount && amount > 0)
                    {
                        bank_data->account_array[account_from].balance -= amount;
                        bank_data->account_array[account_to].balance += amount;

                        printf("New balance after TRANSFER operation is %f in account %d\n", destinationAccountBalance,
                               account_to);
                        sprintf(buffer, "%d %f", OK, destinationAccountBalance);

                        // Successful transaction. Sum one to the transactions done
                        lockMutexTransactionsAndSumToTransactions(data_locks, bank_data);
                    } else
                        sprintf(buffer, "%d %f", INSUFFICIENT,sourceAccountBalance);

                    pthread_mutex_unlock(&data_locks->account_mutex[account_from]);
                    pthread_mutex_unlock(&data_locks->account_mutex[account_to]);
                    break;

                case EXIT:
                    sprintf(buffer, "%d", BYE);
                    break;

                default:
                    sprintf(buffer, "%d", ERROR);
            }
        }
        else
            sprintf(buffer,"%d",NO_ACCOUNT);


        // Send a reply
        printf("reply is %s\n",buffer);
        sendString(connection_fd,buffer,strlen(buffer)+1);

        if(interrupt_exit)
            break;
    }
    close(connection_fd);
    pthread_exit(NULL);
}

/*
    Lock the mutex for transactions and add 1 to the total transactions done
*/

void lockMutexTransactionsAndSumToTransactions(locks_t *data_locks, bank_t *bank_data)
{
    pthread_mutex_lock(&data_locks->transactions_mutex);
        bank_data->total_transactions++;
    pthread_mutex_unlock(&data_locks->transactions_mutex);
}

/*
    Process the request from the client and returns a reply
*/

/*char *switchForOperations(locks_t *data_locks, bank_t *bank_data, int account_to, float amount, int operation,
                          int account_from)
{


    }
    return reply;
}*/

/*
    Free all the memory used for the bank data
*/
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

