/*
 Advanced Programming
 BlackJack Homework - Sockets
 Salomón Charabati
 October '19
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
    int balance;
    player_t playerClientSide;
    int bet;
    int chars_read;

    printf("Welcome to BlackJack!\n");
    // INITIAL BALANCE PART
    printf("Enter you initial balance: ");
    scanf("%d",&balance);
    playerClientSide.balance = balance;
    // Handshake
    // Send a request with initial balance
    sprintf(buffer, "BALANCE:%d", playerClientSide.balance);
    send(connection_fd, buffer, strlen(buffer)+1, 0);

    printf("GAME READY! GOOD LUCK\n");
    chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);


    while(1)
    {
        // BETTING PART
        printf("How much do you want to bet?\n");
        scanf(" %d", &bet);
        sprintf(buffer,"BET: %d",bet);
        send(connection_fd,buffer,strlen(buffer)+1,0);  // Send the proposed bet to the server
        // Check for server's response to bet proposed
        chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);
        if(strncmp(buffer,"Not valid",BUFFER_SIZE) == 0)
        {
            printf("You don't have enough money for placing that bet!\n");
            break;
        }else{
            printf("OKAY, you're betting %d in this hand\n",bet);
        }
        send(connection_fd,"OKAY",5,0);

        // DEALING CARDS PART
        // First card is automatically dealt and received here
        chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);
        printf("%s",buffer);

        // While the player hits, is below 21 or has exactly 21
        while(1)
        {
            printf("Do you want to hit or stay (h/s)?: \n");
            scanf(" %s", buffer);
            send(connection_fd,buffer,strlen(buffer)+1,0);

            chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);
            if(strncmp(buffer,"s",2) == 0 ||(strncmp(buffer,"Bust.",5) == 0 ) || (strncmp(buffer,"BLACKJACK!\n",15) == 0))
                break;

            printf("%s",buffer);
        }

        send(connection_fd,"OKAY",5,0);

        // RESULTS PART
        chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);
        printf("%s",buffer);


    }

}
