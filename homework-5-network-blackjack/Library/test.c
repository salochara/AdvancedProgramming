//
// Created by Salomón Charabati on 2019-10-06.
//
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
typedef struct card{
    int value;
    char * suit;
    char logo;
} card_t;

card_t newCard();

int main()
{
    card_t card;
    card = newCard();

    printf("card value: %d",card.value);
}

card_t newCard()
{
    srand((unsigned int)time(NULL));
    int randomValue = rand() % 13 + 1;
    int resultingValue = 0;

    switch(randomValue)
    {
        case 11:
            resultingValue = 10;
            break;
        case 12:
            resultingValue = 10;
            break;
        case 13:
            resultingValue = 10;
            break;
        default:
            resultingValue = randomValue;
            break;
    }

    card_t newCard;
    newCard.value = resultingValue;

    return newCard;
}