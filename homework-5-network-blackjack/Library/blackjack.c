//
// Created by Salomón Charabati on 2019-10-06.
//

#include "blackjack.h"



card_t newCard()
{
    srand((unsigned int)time(NULL));
    int randomValue = rand() % RAND_MAX;

    card_t newCard;
    newCard.value = randomValue;

    return newCard;
}
