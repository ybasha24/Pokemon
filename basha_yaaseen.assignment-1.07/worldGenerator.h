// typedef struct map {
//     int grid[21][80];
//     int nExit;
//     int eExit;
//     int sExit;
//     int wExit;
// } map_t;
#include "characterQueue.h"

class map_t//should have terrain map, character map, and its own character queue
{
    public:
    int grid[21][80];
    int nExit;
    int eExit;
    int sExit;
    int wExit;
    Characters *cPQ = NULL;//will hold character turns
    Characters *characterTable[21][80];

    map_t(){}
    map_t(int grid[21][80], int nExit, int eExit, int sExit, int wExit){
        this->nExit = nExit;
        this->eExit = eExit;
        this->sExit = sExit; 
        this->wExit = wExit;
    }

    void setBattledTo1(Characters *setThis){
        setThis->battled = 1;
    }

};

void initMap(map_t *useThis, int x, int y,int chance1, int chance2);
void printGrid(int grid [21][80]); // might not need this
int getPCX();
int getPCY();
// int downOrLeft();
// int downOrRight();
// int rightOrUp();
// int rightOrDown();
// void addPokeMart(int grid[21][80], int row, int column);
// void addPokeCenter(int grid[21][80], int row, int column);
// void growSeed(int grid[21][80], int row, int column, int op);
