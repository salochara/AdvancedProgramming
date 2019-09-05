//
// Created by Salomón Charabati on 2019-09-03.
// Advanced Programming
// Homework #3 Linked Lists

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"
#define VALUE_NOT_FOUND -1

//-------FUNCTIONS IMPLEMENTATIONS---------//

// ------- Functions for insertion --------//

node_t * insertAtPosition(node_t *head, int data, int position)
{
    // If the list is empty and position is 0
    if(head == NULL && position > 0)
        printf("The list is empty. You can't insert at position %d\n", position);
    // If desired position is 0
    else if (position == 0)
        head = insertHead(head, data);

    // For position > 0
    else{
        node_t * traverse = head;
        node_t * traverseBefore = NULL;
        node_t * newNode = createNode(data);
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
        // It's at the end of the list. Traverse is null.
        if(traverse == NULL)
        {
            traverseBefore->next = newNode;
            newNode->next = NULL;
        }

    }
    return head;

}

node_t * insertHead(node_t *head, int data)
{
    node_t * newNode = createNode(data);
    newNode->next = head;
    head = newNode;
    return head;
}

// ------- Functions for deletion --------//

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

//------------  Search  ----------------------//

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

// ------------- Helpers  ---------------//

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

node_t * createNode(int data)
{
    node_t * newNode = malloc(sizeof(node_t));
    if(newNode == NULL)
    {
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    return newNode;
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

void mallocChecker(node_t * node)
{
    if(node == NULL)
    {
        printf("Malloc error\n");
        exit(EXIT_FAILURE);
    }
}









