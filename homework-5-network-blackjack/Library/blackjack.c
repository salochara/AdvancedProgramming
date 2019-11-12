/*
 Advanced Programming
 BlackJack Homework - Sockets
 Salomón Charabati
 October '19
*/
#include "blackjack.h"

// Simple function for having an Ace's counter
int sumAces(card_t card)
{
    int acesCounter = 0;
    if(card.value == 11)
        acesCounter++;
    return acesCounter;
}

// Function for creating a new card
card_t newCard()
{
    srand((unsigned int)time(NULL));
    int randomValue = rand() % 13 + 1;
    int resultingValue = 0;

    if(randomValue>10)
        resultingValue = 10;
    else if(randomValue == 1){
        resultingValue = 11;
    }
    else{
        resultingValue = randomValue;
    }
    card_t newCard;
    newCard.value = resultingValue;

    return newCard;
}
