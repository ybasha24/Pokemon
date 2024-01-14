typedef struct characters {
    int xCoord;
    int yCoord;
    int nextTurn; //This is what will be compared and the "priority" value. 
    int seqNum; //Our pc character will be given precendence and priority when next turn is the same
    char symbolOfCharacter;
    int symbol;
    struct characters* next;
    int direction;
    int battled;
} Characters;

Characters* newCharacter(int xCoord, int yCoord, int nextTurn, int seqNum, char symbolOfCharacter, int direction);
Characters* newCharacterOnMap(int xCoord, int yCoord, int symbolOfCharacter, int battled);
Characters* peekCharacter(Characters** head);
void popCharacter(Characters** head);
void pushCharacter(Characters** head, int xCoord, int yCoord, int nextTurn, int seqNum, char symbolOfCharacter, int direction);
int isEmptyCharacter(Characters** head);