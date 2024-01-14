typedef struct characters {
    int xCoord;
    int yCoord;
    int nextTurn; //This is what will be compared and the "priority" value. 
    int seqNum; //Our pc character will be given precendence and priority when next turn is the same
    char symbolOfCharacter;
    struct characters* next;
    int direction;
} Characters;

Characters* newCharacter(int xCoord, int yCoord, int nextTurn, int seqNum, char symbolOfCharacter, int direction);
Characters* peekCharacter(Characters** head);
void popCharacter(Characters** head);
void pushCharacter(Characters** head, int xCoord, int yCoord, int nextTurn, int seqNum, char symbolOfCharacter, int direction);
int isEmptyCharacter(Characters** head);