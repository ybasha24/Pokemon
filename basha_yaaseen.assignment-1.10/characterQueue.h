// typedef struct characters {
//     int xCoord;
//     int yCoord;
//     int nextTurn; //This is what will be compared and the "priority" value. 
//     int seqNum; //Our pc character will be given precendence and priority when next turn is the same
//     char symbolOfCharacter;
//     int symbol;
//     struct characters* next;
//     int direction;
//     int battled;
// } Characters;
#include "pokemon.h"
#include <vector>
#ifndef CHARACHTERQUEUE_H
#define CHARACHTERQUEUE_H

class Characters{
    public:
    int xCoord;
    int yCoord;
    int nextTurn; //This is what will be compared and the "priority" value. 
    int seqNum; //Our pc character will be given precendence and priority when next turn is the same
    char symbolOfCharacter;
    int symbol;
    Characters* next;
    int direction;
    int battled;
    pokemon_generator *pokemonList[6] = {NULL};

    Characters(){};
    Characters(int xCoord, int yCoord, int nextTurn, int seqNum, char symbolOfCharacter, int symbol, int direction, int battled){//dont even need this
        this->xCoord = xCoord;
        this->yCoord = yCoord;
        this->nextTurn = nextTurn;
        this->seqNum = seqNum;
        this->symbolOfCharacter = symbolOfCharacter;
        this->symbol = symbol;
        this->next = NULL;
        this->direction = direction;
        this->battled = battled;
    }
};

Characters* newCharacter(int xCoord, int yCoord, int nextTurn, int seqNum, char symbolOfCharacter, int direction);
Characters* newCharacterOnMap(int xCoord, int yCoord, int symbolOfCharacter, int battled);
Characters* peekCharacter(Characters** head);
void popCharacter(Characters** head);
void pushCharacter(Characters** head, int xCoord, int yCoord, int nextTurn, int seqNum, char symbolOfCharacter, int direction);
int isEmptyCharacter(Characters** head);

#endif