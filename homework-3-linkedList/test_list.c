/*
Simple program to test the functionality of our linked list

Gilberto Echeverria
gilecheverria@yahoo.com
*/

#include <stdio.h>
#include "linked_list.h"

// For testing
void predefinedOperations();
node_t * insertSomething(node_t * head);
node_t * searchForSomething(node_t * head);
node_t * deleteSomething(node_t * head);


int main()
{
    predefinedOperations();

    return 0;
}

// ------- Functions for testing --------//

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

