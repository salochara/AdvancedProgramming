//
// Created by Salomón Charabati on 2019-10-06.
//

#ifndef ADVANCEDPROGRAMMING_BLACKJACK_H
#define ADVANCEDPROGRAMMING_BLACKJACK_H
#include <time.h>
#include <stdlib.h>

typedef struct card{
    int value;
    char * suit;
    char logo;
} card_t;

typedef struct player{
    int sum;
    int balance;
    int currentBet;
} player_t;

typedef  struct  dealer{
    int sum;
} dealer_t;



card_t newCard();

#endif //ADVANCEDPROGRAMMING_BLACKJACK_H
