// typedef sturct coord{
//     int xCoord;
//     int yCoord;
// } Coordinates;

typedef struct node {
    int xCoord;
    int yCoord;
    int distance;
    struct node* next;
} Node;

Node* newNode(int xCoord, int yCoord, int distance);
Node* peek(Node** head);
void pop(Node** head);
void push(Node** head, int xCoord, int yCoord, int distance);
int isEmpty(Node** head);