//
// Created by Salomón Charabati on 2019-09-03.
// Advanced Programming
// Homework #3 Linked Lists

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#define VALUE_NOT_FOUND -1


// TODO Print recursively


// FUNCTIONS IMPLEMENTATIONS
void predefinedOperations()
{
    node_t * head = NULL;

    head = insertSomething(head);
    head = searchForSomething(head);
    head = deleteSomething(head);

    // Free the memory used
    head = clear(head);
}



node_t * insertSomething(node_t * head)
{
    head = insertHead(head, 4);
    head = insertHead(head, 286);
    head = insertHead(head, 67);
    head = insertHead(head, 729);

    printf("List after head inserts: \n");
    printList(head);

    head = insertAtPosition(head, 5, 2);
    head = insertAtPosition(head, 77, 4);
    head = insertAtPosition(head, 83, 0);

    printf("List after position inserts: \n");
    printList(head);

    printf("List printed recursively: \n");
    printRecursive(head);

    return head;
}

node_t * deleteSomething(node_t * head)
{
    head = deleteHead(head);
    head = deleteHead(head);
    head = deleteHead(head);

    printf("List after 3 head deletes: \n");
    printList(head);

    head = deleteFromPosition(head, 2);
    head = deleteFromPosition(head, 0);

    printf("List after deletes from indices 2 and 0: \n");
    printList(head);

    return head;
}

node_t * searchForSomething(node_t * head)
{
    int num_search = 67;
    int position = -1;

    position = searchIterative(head, num_search);
    if (position >= 0)
    {
        printf("Element %d found at index %d\n", num_search, position);
    }
    else
    {
        printf("Element %d not found!\n", num_search);
    }

    num_search = 100;
    position = searchIterative(head, num_search);
    if (position >= 0)
    {
        printf("Element %d found at index %d\n", num_search, position);
    }
    else
    {
        printf("Element %d not found!\n", num_search);
    }

    return head;
}

int getLength(node_t * head)
{
    // If list is empty
    if(head == NULL)
    {
        printf("The list is empty\n");
        return 0;
    }

    // If there is only one element
    if(head->next == NULL)
        return  1;

    // If there is > 1 element
    node_t * traverse = head;
    int count = 0;
    while(traverse != NULL)
    {
        traverse = traverse->next;
        count++;
    }
    return count;
}

node_t * clear(node_t * head)
{
    node_t * toDelete = NULL;
    while(head != NULL)
    {
        toDelete = head;
        head = head->next;
        free(toDelete);
    }
    return head;
}

int searchIterative(node_t * head, int data)
{
    int index = 0;
    for (node_t * traverse = head;  traverse != NULL ; traverse = traverse->next) {
        if(traverse->data == data){
            return index;
        }
        index++;
    }
    return VALUE_NOT_FOUND;
}

node_t * deleteFromPosition(node_t * head, int position)
{
    if(position == 0)
    {
        head = deleteHead(head);
    }else{
        node_t * traverse = head;
        node_t * beforeTraverse = NULL;
        int count = 0;

        while(traverse != NULL)
        {
            if(count == position){
                beforeTraverse->next = traverse->next;
                free(traverse);
                break;
            }
            beforeTraverse = traverse;
            traverse = traverse->next;
            count++;
        }
        if(traverse == NULL){
            free(beforeTraverse->next);
        }
    }
    return head;
}

node_t * deleteHead(node_t * head)
{
    if(head == NULL){
        printf("The list is empty\n");
        exit(EXIT_SUCCESS);
    }

    node_t * toDelete = head;
    head = head->next;
    free(toDelete);

    return head;
}

node_t * insertHead(node_t *head, int data)
{
    node_t * newNode = malloc(sizeof(node_t));
    mallocChecker(newNode);
    newNode->data = data;
    newNode->next = head;
    head = newNode;
    return head;
}

node_t * insertAtPosition(node_t *head, int data, int position)
{
    // If the list is empty
    if(head == NULL && position > 0)
    {
        printf("The list is empty. You can't insert at position %d\n",position);
    }else if(position == 0){ // if position is 0
        node_t * newNode = malloc(sizeof(node_t));
        mallocChecker(newNode);
        newNode->data = data;
        newNode->next = head;
        head = newNode;
    }else{  // For position > 0
        // Create a new node_struct
        node_t * traverse = head;
        node_t * traverseBefore = NULL;
        node_t * newNode = malloc(sizeof(node_t));
        newNode->data = data;
        mallocChecker(newNode);
        // Check for malloc error
        mallocChecker(traverse);

        int count = 0;
        while(traverse != NULL)
        {
            // If the index is found
            if(count == position)
            {
                newNode->next = traverseBefore->next;
                traverseBefore->next = newNode;
                break;
            }
            traverseBefore = traverse;
            traverse = traverse->next;
            count++;
        }
        // It's at the end of the list
        if(traverse == NULL)
        {
            traverseBefore->next = newNode;
            newNode->next = NULL;
        }

    }
    return head;

}

void mallocChecker(node_t * node)
{
    if(node == NULL)
    {
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }
}

void printList(node_t * head)
{
    node_t *traverse = NULL;
    traverse = head;
    while (traverse->next != NULL) {
        printf("Address of node_struct: %p pointing to -> %p \t Value: %d\n", (void*)traverse, (void*)traverse->next, traverse->data);
        traverse = traverse->next;
    }
    printf("Address of node_struct: %p pointing to -> %p \t Value: %d\n", (void*)traverse, (void*)traverse->next, traverse->data);
    printf("-------------------------------\n");
}

void printRecursive(node_t * head)
{
    node_t * traverse = head;
    if(traverse == NULL)
        return;

    printf("Address of node_struct: %p pointing to -> %p \t Value: %d\n", (void*)traverse, (void*)traverse->next, traverse->data);
    printRecursive(traverse->next);

}
