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
#include <errno.h>
#include <time.h>

// Include libraries for sockets
#include <netdb.h>
#include <arpa/inet.h>
// Include own socket library
#include "sockets.h"

// Library for Blackjack
#include "blackjack.h"

#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1024

void usage(char * program);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[])
{
    int server_fd;

    printf("\n=== SERVER PROGRAM ===\n");

    if (argc != 2)
        usage(argv[0]);

    // Show the ip's for this machine
    printLocalIPs();

    server_fd = startServer(argv[1]);
    waitForConnections(server_fd);

    printf("Closing the server socket\n");
    // Close the socket
    close(server_fd);

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Stand by for connections by the clients
void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int connection_fd;
    pid_t new_pid;

    while(1)
    {
        ///// ACCEPT
        // Receive incomming connections
        // Get the size of the structure where the address of the client will be stored
        client_address_size = sizeof client_address;
        // Get the file descriptor to talk with the client
        connection_fd = accept(server_fd, (struct sockaddr *) &client_address,
                                &client_address_size);
        if (connection_fd == -1)
        {
            perror("accept");
            close(connection_fd);
        }

        // Fork a child process to deal with the new client
        new_pid = fork();
        if (new_pid > 0)    // Parent
        {
            close(connection_fd);
        }
        else if (new_pid == 0)      // Child
        {
            close(server_fd);

            // Identify the client
            // Get the ip address from the structure filled by accept
            inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, INET_ADDRSTRLEN);
            printf("Received a connection from %s : %d\n", client_presentation, client_address.sin_port);

            // Establish the communication
            communicationLoop(connection_fd);
            close(connection_fd);
            // Finish the child process
            exit(EXIT_SUCCESS);
        }
    }
}

// Do the actual receiving and sending of data
void communicationLoop(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    char * string;
    int chars_read;
    int currentBet;
    int numberOfAces = 0;
    player_t playerServerSide;


    // Initialize the random seed
    srand(time(NULL));

    // INITIAL BALANCE PART
    // Handshake
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    // Get the first part
    string = strtok(buffer, ":");
    if (strncmp(string, "BALANCE", 8) != 0){printf("Invalid client. Exiting!\n");return;}
    // Get the initial balance of the player
    string = strtok(NULL, ":");
    playerServerSide.balance = atoi(string);
    printf("YOUR INITIAL BALANCE IS: %d. GOOD LUCK!\n",playerServerSide.balance);
    sprintf(buffer,"YOUR INITIAL BALANCE IS: %d. GOOD LUCK!\n",playerServerSide.balance);
    send(connection_fd, buffer, strlen(buffer)+1, 0);




    while(1)
    {
        // BETTING PART
        chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);
        // Get the amount from the string sent
        string = strtok(buffer,":");
        string = strtok(NULL,":");
        currentBet = atoi(string);
        if(playerServerSide.balance < currentBet)
        {
            sprintf(buffer,"Not valid");
            send(connection_fd,buffer,strlen(buffer)+1,0);
            break;
        }else{
            sprintf(buffer,"OK");
            send(connection_fd,buffer,strlen(buffer)+1,0);
        }
        printf("The player wants to bet: %d\n",currentBet);

        chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);


        // DEALING CARDS PART
        // First card is sent to client
        playerServerSide.sum = 0;
        card_t card = newCard();
        numberOfAces += sumAces(card);
        playerServerSide.sum += card.value;
        printf("New card value: %d\n",card.value);
        sprintf(buffer,"Sum of your hand: %d\n",playerServerSide.sum);
        send(connection_fd,buffer,strlen(buffer)+1,0);
        while(1)
        {
            chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);
            if(strncmp(buffer,"h",2) == 0)
            {
                card_t card = newCard();
                numberOfAces += sumAces(card);
                printf("New card value: %d\n",card.value);
                playerServerSide.sum += card.value;
                if(playerServerSide.sum > 21)
                {
                    sprintf(buffer,"Bust. Sum of your hand: %d\n",playerServerSide.sum);
                    send(connection_fd,buffer,strlen(buffer)+1,0);
                    break;
                }
                else if(playerServerSide.sum == 21)
                {
                    sprintf(buffer,"BLACKJACK!\n");
                    send(connection_fd,buffer,strlen(buffer)+1,0);
                    break;
                }
                else{
                    sprintf(buffer,"Sum of your hand: %d\n",playerServerSide.sum);
                    send(connection_fd,buffer,strlen(buffer)+1,0);
                }
            }
            else{
                send(connection_fd,"s",strlen(buffer)+1,0);
                break;
            }
        }
        chars_read = receiveMessage(connection_fd,buffer,BUFFER_SIZE);

        // DEALER DEALS HIS OWN CARDS
        player_t dealer;
        dealer.sum = 0;
        while(dealer.sum < 17)
        {
            card_t newC = newCard();
            dealer.sum += newC.value;
        }
        printf("Dealer has: %d\n",dealer.sum);

        // COMPARING RESULTS SUMS OF HANDS OF DEALER AND PLAYER
        // Format of buffer. {player}wins|{player.sum}|{dealer.sum}\t player.balance}

        // Player has BLACKJACK
        if(playerServerSide.sum == 21){
            playerServerSide.balance  = playerServerSide.balance  + (currentBet * 2);
            sprintf(buffer,"Player wins with BLACKJACK!\nDealer pays double|Player sum: %d|Dealer sum:%d\t Player balance: %d\n",playerServerSide.sum,dealer.sum,playerServerSide.balance);
        }
        // Player > 21
        else if(playerServerSide.sum > 21){
            if(numberOfAces > 0){ // Special case for ACES. If player is above 21, aces now count 1
                int beforeAdjustingAces = playerServerSide.sum;
                playerServerSide.sum = playerServerSide.sum - (numberOfAces * 10);
                if((playerServerSide.sum <= 21 && playerServerSide.sum > dealer.sum) || dealer.sum > 21){
                    playerServerSide.balance += currentBet;
                    sprintf(buffer,"Player wins. Aces now count 1. You had %d and now you have %d |Dealer sum:%d\t Player balance: %d\n",beforeAdjustingAces, playerServerSide.sum,dealer.sum,playerServerSide.balance);
                }else{
                    playerServerSide.balance  -= currentBet;
                    sprintf(buffer,"BUSTED! Dealer wins|Player sum: %d\t Player balance: %d\n",playerServerSide.sum,playerServerSide.balance);
                }
            }else{
                playerServerSide.balance  -= currentBet;
                sprintf(buffer,"BUSTED! Dealer wins|Player sum: %d\t  Player balance: %d\n",playerServerSide.sum,playerServerSide.balance);
            }
        // Player <= 21 and Dealer > 21
        }else if(playerServerSide.sum <= 21 && dealer.sum > 21){
            playerServerSide.balance += currentBet;
            sprintf(buffer,"Player wins|Player sum: %d|Dealer sum:%d\t Player balance: %d\n",playerServerSide.sum,dealer.sum,playerServerSide.balance);
        }
        // Player < Dealer
        else if(playerServerSide.sum < dealer.sum)
        {
            playerServerSide.balance  -= currentBet;
            sprintf(buffer,"Dealer wins|Player sum: %d|Dealer sum:%d \t Player balance: %d\n",playerServerSide.sum,dealer.sum,playerServerSide.balance);
        // Player > Dealer
        }else if(playerServerSide.sum > dealer.sum){
            playerServerSide.balance += currentBet;
            sprintf(buffer,"Player wins|Player sum: %d|Dealer sum:%d\t Player balance: %d\n",playerServerSide.sum,dealer.sum,playerServerSide.balance);
        // Player = Dealer. Push.
        }else if (playerServerSide.sum == dealer.sum){
            sprintf(buffer,"Push|Player sum: %d|Dealer sum:%d\t Player balance: %d\n",playerServerSide.sum,dealer.sum,playerServerSide.balance);
        }
        numberOfAces = 0; // Restart number of aces for next bet
        send(connection_fd,buffer,strlen(buffer)+1,0);
    }
}
