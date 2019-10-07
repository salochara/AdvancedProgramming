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
    int balance;
    player_t playerClientSide;
    int bet;
    int chars_read;

    printf("Welcome to BlackJack!\n");
    // State your initial balance
    printf("Enter you initial balance: ");
    scanf("%d", &balance);
    playerClientSide.balance = balance;

    // Handshake
    // Send a request with initial balance
    sprintf(buffer, "BALANCE:%d", playerClientSide.balance);
    send(connection_fd, buffer, strlen(buffer)+1, 0);

    printf("Game ready!\n");
    while(1)
    {
        // Get how much the client wants to bet
        printf("How much do you want to bet?\n");
        scanf("%d",&bet);
        // Send the propsed bet to the server
        sprintf(buffer,"BET: %d",bet);
        send(connection_fd,buffer,strlen(buffer)+1,0);

        // Receive the server's response to the bet proposed
        chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);
        // Good to bet
        if(strncmp(buffer,"OK",3) == 0)
        {
            printf("Dealer response: %s\n",buffer); // Should be OK
            // Get the first card dealt by the dealer
            chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);// Receives the value of the first card dealt
            printf("%s\n",buffer);

            while(1)
            {
                // Stay or hit?
                printf("Stay or hit? (s/h)\n");
                scanf("%s",buffer);
                // Send to server s or h
                send(connection_fd,buffer,strlen(buffer)+1,0);

                // It the user chooses to stay
                if(strncmp(buffer,"s",BUFFER_SIZE) == 0)
                {
                    break;
                }
                // Chose to hit
                chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);

                char * pch;
                pch = strstr(buffer,"You lost!");
                if(pch != NULL){
                    printf("%s",buffer);
                    break;
                }
                printf("%s",buffer);

            }
        }else{ // Not enough money to bet
            printf("Dealer response: %s",buffer);
            break;
        }


    }


}
