/*
 Advanced Programming
 BlackJack Homework - Sockets
 Salomón Charabati
 October '19
*/
#include "blackjack.h"



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
