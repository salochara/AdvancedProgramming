/*
    Program for a simple chat client
    The server address and port are provided as arguments to the program

    Gilberto Echeverria
    gilecheverria@yahoo.com
    26/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include "sockets.h"
// Include own sockets library
#include "sockets.h"

#include "blackjack.h"

#define SERVICE_PORT 8642
#define BUFFER_SIZE 1024

void usage(char * program);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[])
{
    int client_fd;

    printf("\n=== CLIENT PROGRAM ===\n");

    if (argc != 3)
        usage(argv[0]);

    client_fd = connectToServer(argv[1], argv[2]);
    communicationLoop(client_fd);

    // Closing the socket
    printf("Closing the connection socket\n");
    close(client_fd);

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Do the actual receiving and sending of data
void communicationLoop(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    int chars_read;
    int balance;
    int guess;
    player_t playerClientSide;

    printf("Welcome to BlackJack!\n");
    // State your initial balance
    printf("Enter you initial balance: ");
    scanf("%d", &balance);
    playerClientSide.balance = balance;

    // Handshake
    // Send a request with initial balance
    sprintf(buffer, "BALANCE:%d", playerClientSide.balance);
    send(connection_fd, buffer, strlen(buffer)+1, 0);




    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    if (strncmp(buffer, "READY", 6) != 0)
    {
        printf("Invalid server. Exiting!\n");
        return;
    }
    printf("Game ready!\n");
    card_t card;
    while(1)
    {
        // Send a request
        printf("Enter a number: ");
        scanf("%d", &guess);
        sprintf(buffer, "GUESS:%d", guess);
        send(connection_fd, buffer, strlen(buffer)+1, 0);

        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        if (chars_read <= 0)
            break;
        if (strncmp(buffer, "OK", 3) == 0)
        {
            printf("You guessed correctly!\n");
            break;
        }
        printf("Try again with a %s number\n", buffer);
        card = newCard();
        printf("value of new card: %d\n",card.value);
    }
    // Close the connection
    send(connection_fd, "BYE", 4, 0);
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    printf("The server sent me: '%s'\n", buffer);
}
