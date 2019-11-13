/*
    Client program to access the accounts in the bank
    This program connects to the server using sockets

    Gilberto Echeverria
    gilecheverria@yahoo.com
    29/03/2018
    07/11/2019  Split 'bankOperations' into three functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
// Custom libraries
#include "bank_codes.h"
#include "sockets.h"
#include "fatal_error.h"

#define BUFFER_SIZE 1024

///// FUNCTION DECLARATIONS
void usage(char * program);
void bankOperations(int connection_fd);
void displayResult(int status, int account_from, float balance);
int getOptionInputs(int option, int * account_to, int * account_from, float * amount, operation_t * operation);

///// MAIN FUNCTION
int main(int argc, char * argv[])
{
    int connection_fd;

    printf("\n=== BANK CLIENT PROGRAM ===\n");

    // Check the correct arguments
    if (argc != 3)
    {
        usage(argv[0]);
    }

    // Start the server
    connection_fd = connectSocket(argv[1], argv[2]);
	// Use the bank operations available
    bankOperations(connection_fd);
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

/*
    Main menu with the options available to the user
*/
void bankOperations(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    int account_from;
    int account_to;
    float amount;
    float balance;
    char option = 'c';
    int status;
    operation_t operation;

    while (option != 'x')
    {
        printf("\n=== BANK MENU ===\n");
        printf("\tc. Check balance\n");
        printf("\td. Deposit into account\n");
        printf("\tw. Withdraw from account\n");
        printf("\tt. Transfer between accounts\n");
        printf("\tx. Exit program\n");
        printf("Select an option: ");
        scanf(" %c", &option);

        // Get inputs 
        if (!getOptionInputs(option, &account_to, &account_from, &amount, &operation))
        {
            // If the option selected was not valid, skip the rest of this loop
            continue;
        }

        balance = 0;

        // Prepare the message to the server
        sprintf(buffer, "%d %d %d %f", operation, account_from, account_to, amount);

        // SEND
        // Send the request
        sendString(connection_fd, buffer, strlen(buffer)+1);

        // RECV
        // Receive the response
        if ( !recvString(connection_fd, buffer, BUFFER_SIZE) )
        {
            printf("Server closed the connection\n");
            break;
        }
        // Extract the data
        sscanf(buffer, "%d %f", &status, &balance);

        // Print the corresponding message
        displayResult(status, account_from, balance);
    }
}


/*
   Get the data necessary depending on the operation selected by the user
*/
int getOptionInputs(int option, int * account_to, int * account_from, float * amount, operation_t * operation)
{
    // Init variables to default values
    *account_from = 0;
    *account_to = 0;
    *amount = 0;

    switch(option)
    {
        // Check balance
        case 'c':
            printf("Enter account: ");
            scanf("%d", account_from);
            *operation = CHECK;
            break;
        // Deposit into account
        case 'd':
            printf("Enter destination account: ");
            scanf("%d", account_to);
            printf("Enter the amount to deposit: ");
            scanf("%f", amount);
            *operation = DEPOSIT;
            break;
        // Withdraw from account
        case 'w':
            printf("Enter source account: ");
            scanf("%d", account_from);
            printf("Enter the amount to deposit: ");
            scanf("%f", amount);
            *operation = WITHDRAW;
            break;
        // Transfer between accounts
        case 't':
            printf("Enter source account: ");
            scanf("%d", account_from);
            printf("Enter destination account: ");
            scanf("%d", account_to);
            printf("Enter the amount to deposit: ");
            scanf("%f", amount);
            *operation = TRANSFER;
            break;
        // Exit the bank
        case 'x':
            printf("Thanks for using the program. Bye!\n");
            *operation = EXIT;
            break;
        // Incorrect option
        default:
            printf("Invalid option. Try again ...\n");
            // Return negative for an invalid option
            return 0;
    }

    // Return positive for a valid transaction
    return 1;
}


/*
   Show a message depending on the server reply
*/
void displayResult(int status, int account_from, float balance)
{
    // Print the message corresponding to the result
    switch (status)
    {
        case OK:
            printf("\tThe balance in account %d is %.2f\n", account_from, balance);
            break;
        case INSUFFICIENT:
            printf("\tInsufficient funds for the transaction selected\n");
            break;
        case NO_ACCOUNT:
            printf("\tInvalid account number entered\n");
            break;
        case BYE:
            printf("\tThanks for connecting to the bank. Good bye!\n");
            break;
        case ERROR: default:
            printf("\tInvalid operation. Try again\n");
            break;
    }
}
