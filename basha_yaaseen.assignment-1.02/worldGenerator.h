typedef struct map {
    int grid[21][80];
    int nExit;
    int eExit;
    int sExit;
    int wExit;
} map_t;

typedef struct world{
    map_t *w[402][402];
    // int xCoord;
    // int yCoord;
} world_t;

void initMap(map_t *useThis, int x, int y,int chance1, int chance2);
void printGrid(int grid [21][80]); // might not need this
// int downOrLeft();
// int downOrRight();
// int rightOrUp();
// int rightOrDown();
// void addPokeMart(int grid[21][80], int row, int column);
// void addPokeCenter(int grid[21][80], int row, int column);
// void growSeed(int grid[21][80], int row, int column, int op);
