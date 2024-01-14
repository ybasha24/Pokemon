#include <cstdio>
#include <cstdlib>
#include "characterQueue.h"

Characters* newCharacter(int xCoord, int yCoord, int nextTurn, int seqNum, char symbolOfCharacter, int direction){
    Characters* temp = (Characters*) malloc((sizeof(Characters)));
    temp->xCoord = xCoord;
    temp->yCoord = yCoord;
    temp->nextTurn = nextTurn;
    temp->seqNum = seqNum;
    temp->symbolOfCharacter = symbolOfCharacter;
    temp->symbol = -1;
    temp->battled = -1;
    temp->direction = direction;
    temp->next = NULL;
 
    return temp;
}
Characters* newCharacterOnMap(int xCoord, int yCoord, int symbolOfCharacter, int battled){
    Characters* temp = (Characters*) malloc((sizeof(Characters)));
    temp->xCoord = xCoord;
    temp->yCoord = yCoord;
    temp->symbol = symbolOfCharacter;
    temp->next = NULL;
    temp->battled = battled;
    temp->nextTurn = 0;
    temp->seqNum = -1;
    temp->symbolOfCharacter = '0';
    temp->direction = -1;
 
    return temp;
}
 //Peek is wrong as of now!
Characters* peekCharacter(Characters** head){
    return *head;
}

void popCharacter(Characters** head){
    Characters* temp = *head;
    (*head) = (*head)->next;
    free(temp);
}

void pushCharacter(Characters** head, int xCoord, int yCoord, int nextTurn, int seqNum, char symbolOfCharacter, int direction){
    Characters* start = (*head);
    
 
    // Create new Node
    Characters* temp = newCharacter(xCoord, yCoord, nextTurn, seqNum, symbolOfCharacter, direction);
 
    // Special Case: The head of list has lesser
    // priority than new node. So insert new
    // node before head node and change head node.
    if(*head == NULL){
        *head = temp;
    }
    else if ((*head)->nextTurn > nextTurn) {
 
        // Insert New Node before head
        temp->next = *head;
        (*head) = temp;
    }
    else if(((*head)->nextTurn == nextTurn) && ((*head)->seqNum > seqNum)){
        temp->next = *head;
        (*head) = temp;
    }
    else {
        // Traverse the list and find a
        // position to insert new node
        while (start->next != NULL &&
            start->nextTurn < nextTurn) {
                if(start->nextTurn == nextTurn){
                    if(start->seqNum > seqNum){
                        start = start->next;
                        break;
                    }
                    else{
                        // start = start->next->next;
                        // break;
                        continue;
                    }
                }
            start = start->next;
        }
 
        // Either at the ends of the list
        // or at required position
        temp->next = start->next;
        start->next = temp;
    }
}

int isEmptyCharacter(Characters** head)
{
    return (*head) == NULL;
}



// int main()
// {
//     // Create a Priority Queue
//     // 7->4->5->6
//     Characters* pq = newNode(1, 1, 0, 5, '-');
//     push(&pq, 1, 3, 0, 1, '<');
//     push(&pq, 1, 4, 0, 2, '>');
//     push(&pq, 1, 3, 0, 0, '@');
//     push(&pq, 1, 2, 10, 7, '^');
//     push(&pq, 1, 9, 10, 4, ')');
    
 
//     while (!isEmpty(&pq)) {
//         printf("%c ", peek(&pq)->symbolOfCharacter);
//         pop(&pq);
//     }
 
//     return 0;
// }
