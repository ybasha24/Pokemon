#include <cstdio>
#include <cstdlib>
#include "priorityQueue.h"

Node* newNode(int xCoord, int yCoord, int distance){
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->xCoord = xCoord;
    temp->yCoord = yCoord;
    temp->distance = distance;
    temp->next = NULL;
 
    return temp;
}
 //Peek is wrong as of now!
Node* peek(Node** head){
    return *head;
}

void pop(Node** head){
    Node* temp = *head;
    (*head) = (*head)->next;
    free(temp);
}

void push(Node** head, int xCoord, int yCoord, int distance){
    Node* start = (*head);
 
    // Create new Node
    Node* temp = newNode(xCoord, yCoord, distance);
 
    // Special Case: The head of list has lesser
    // priority than new node. So insert new
    // node before head node and change head node.
    if ((*head)->distance > distance) {
 
        // Insert New Node before head
        temp->next = *head;
        (*head) = temp;
    }
    else {
 
        // Traverse the list and find a
        // position to insert new node
        while (start->next != NULL &&
            start->next->distance < distance) {
            start = start->next;
        }
 
        // Either at the ends of the list
        // or at required position
        temp->next = start->next;
        start->next = temp;
    }
}

int isEmpty(Node** head)
{
    return (*head) == NULL;
}

// int main()
// {
//     // Create a Priority Queue
//     // 7->4->5->6
//     Node* pq = newNode(1, 1, 0);
//     push(&pq, 1, 2, 3);
//     push(&pq, 1, 3, 4);
//     push(&pq, 1, 4, 7);
 
//     while (!isEmpty(&pq)) {
//         printf("%d ", peek(&pq)->yCoord);
//         pop(&pq);
//     }
 
//     return 0;
// }