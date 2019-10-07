//
// Created by Salomón Charabati on 2019-10-06.
//

#include "blackjack.h"



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
