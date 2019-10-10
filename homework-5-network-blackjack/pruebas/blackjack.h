/*
 Advanced Programming
 BlackJack Homework - Sockets
 Salomón Charabati
 October '19
*/

#ifndef ADVANCEDPROGRAMMING_BLACKJACK_H
#define ADVANCEDPROGRAMMING_BLACKJACK_H
#include <time.h>
#include <stdlib.h>

// DATA TYPES
typedef struct card{
    int value;
} card_t;

typedef struct player{
    int sum;
    int balance;
} player_t;

// FUNCTION DECLARATION
int sumAces(card_t card);
card_t newCard();

#endif //ADVANCEDPROGRAMMING_BLACKJACK_H
