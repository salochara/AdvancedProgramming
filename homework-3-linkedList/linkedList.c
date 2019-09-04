//
// Created by Salomón Charabati on 2019-09-03.
// Advanced Programming
// Homework #3 Linked Lists

#include <stdio.h>
#include <stdlib.h>
#define VALUE_NOT_FOUND -1

// Indexing in the linked list starts at 0

// STRUCT DEFINITION
typedef struct node{
    int data;
    struct node * next;
} node_t;

// FUNCTIONS DEFINITIONS

void printList(node_t * head);
node_t * insertElementAtHead(node_t *head, int data);
void countList(node_t * head);
void mallocChecker(node_t * node);
void insertElementAtIndex(node_t * head, int index, int data);
node_t * deleteAtHead(node_t * head);
void deleteAtIndex(node_t * head, int index);
int searchForValue(node_t * head, int value); // why not const ?
int getLengthOfList(node_t * head);

int main()
{
    int index, lengthOfList;
    node_t * head = NULL;

    printf("***** Note: Indexing in the list starts at index 0 *****\n");
    head = insertElementAtHead(head,1);
    insertElementAtIndex(head,1,2);
    insertElementAtIndex(head,2,3);
    printList(head);
    deleteAtIndex(head, 1);
    printList(head);
    index = searchForValue(head,1);
    deleteAtIndex(head,1);
    printList(head);
    lengthOfList = getLengthOfList(head);
    printf("Length of list is: %d\n", lengthOfList);

}

// FUNCTIONS IMPLEMENTATIONS

int getLengthOfList(node_t * head)
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

int searchForValue(node_t * head, int value)
{
    int index = 0;
    for (node_t * traverse = head;  traverse != NULL ; traverse = traverse->next) {
        if(traverse->data == value)
        {
            printf("Value %d was found at index: %d\n ",value,index);
            return index;
        }
        index++;
    }
    return VALUE_NOT_FOUND;
}

void deleteAtIndex(node_t * head, int index)
{
    if(index == 0)
    {
        printf("This is not the function to use. You should use deleteAtHead instead\n");
        exit(EXIT_SUCCESS);
    }

    node_t * traverse = head;
    node_t * beforeTraverse = NULL;
    int count = 0;

    while(traverse != NULL)
    {
        if(count == index)
        {
            beforeTraverse->next = traverse->next;
            printf("**Deleting node at memory address: %p at index: %d**\n",traverse, count);
            free(traverse);
            break;
        }

        beforeTraverse = traverse;
        traverse = traverse->next;
        count++;
    }
    if(traverse == NULL)
    {
        printf("**Deleting node at memory address: %p at index: %d**\n",traverse, count);
        free(beforeTraverse->next);
    }
}

node_t * deleteAtHead(node_t * head)
{
    if(head == NULL)
    {
        printf("The list is empty\n");
        exit(EXIT_SUCCESS);
    }

    node_t * toDelete = head;
    head = head->next;
    free(toDelete);

    return head;
}

// Insert element at head when the list is empty
node_t * insertElementAtHead(node_t *head, int data)
{
    node_t * newNode = malloc(sizeof(node_t));
    mallocChecker(newNode);

    // If the list is empty
    if (head == NULL)
    {
        newNode->data = data;
        newNode->next = NULL;
        head = newNode;
        return head;
    }else{
        printf("The list is not empty. This function can't be used\n");
        exit(EXIT_SUCCESS);
    }
}

void insertElementAtIndex(node_t * head, int index, int data)
{
    // If the list is empty
    if(head == NULL)
    {
        printf("The list is empty\n");
        return;
    }

    // For index > 0
    // Create a new node
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
        if(count == index)
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
        printf("Address of node: %p pointing to -> %p \t Value: %d\n", (void*)traverse, (void*)traverse->next, traverse->data);
        traverse = traverse->next;
    }
    printf("Address of node: %p pointing to -> %p \t Value: %d\n", (void*)traverse, (void*)traverse->next, traverse->data);
    printf("-------------------------------\n");
}