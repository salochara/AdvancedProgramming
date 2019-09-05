/*
    Simple linked list that stores integers

    Salomón Charabati Michan
*/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

// Definition of data types

// STRUCT DEFINITION
typedef struct node_struct{
    int data;
    struct node_struct * next;
} node_t;

// Function declarations

// Insertion
node_t * insertHead (node_t * head, int data);
node_t * insertAtPosition (node_t * head, int data, int position);
// Deletion
node_t * deleteHead(node_t * head);
node_t * deleteFromPosition(node_t * head, int position);
// Search
int searchIterative (node_t * head, int data);
// Helper methods
node_t * clear(node_t * head);
void printList(node_t * head);
void printRecursive(node_t * head);
int getLength(node_t * head);
node_t * createNode(int data);
void mallocChecker(node_t * node);



// For testing
void predefinedOperations();
node_t * insertSomething(node_t * head);
node_t * searchForSomething(node_t * head);
node_t * deleteSomething(node_t * head);


#endif
