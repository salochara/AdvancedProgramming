//
// Created by Salomón Charabati on 2019-10-06.
// Advanced Programming
// Homework #4 Vigenere Cypher

#ifndef ADVANCEDPROGRAMMING_VIGENERE_H
#define ADVANCEDPROGRAMMING_VIGENERE_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define MAX_STRING_SIZE 50
#define BUFFER_SIZE 200
#define ENCODE_OPTION 0
#define DECODE_OPTION 1

// FUNCTION DECLARATIONS
void getUserInput(char decodeEncodeOption[], char filename[], char key[]);
void openPipes(int *parentToChild, int *childToParent);
char * vigenereEntryFunction(char *decodeEncodeOption, char *inputFileName, char *key);
char * vigenere_algorithm(char *inputFileName, char key[], int option);
int runProgram();


#endif //ADVANCEDPROGRAMMING_VIGENERE_H
