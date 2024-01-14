#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>
#include "worldGenerator.h"
#include "priorityQueue.h"

int probabilityOfPokeStuff(int x, int y);
void dijkstrasMethod(int grid[21][80], int hikerOrRider);
void printDistTable(int distance[21][80]);
int costOfTerrainForHiker(int x, int y, int grid[21][80]);

// map_t *w[402][402];

int main(int argc, char *argv[]){
    int chance1;
    int chanceToSend;//pokemart
    int chanceToSend2;//pokecenter
    map_t *w[402][402];
    int i, j;
    for(i =0; i<=401; i++){
        for(j =0; j<=401; j++){
            w[i][j]= NULL;
        }
    }
    int x = 200;
    int y = 200;
    char userInput;
    srand(time(NULL));

    if(w[x][y] == NULL){
        w[x][y] = malloc((sizeof(map_t)));
        (w[x][y]->eExit) = (rand() % 17) + 2;
        (w[x][y]->wExit) = (rand() % 17) + 2;
        (w[x][y]->nExit) = (rand() % 76) + 2;
        (w[x][y]->sExit) = (rand() % 76) + 2;
        initMap(w[x][y],x,y,1,1);
    }
    printGrid(w[200][200]->grid);
    // printf("Going in!\n");
    // printf("%d %d\n", getPCX(), getPCY());
    // printf("%d\n", w[200][200]->grid[getPCX()][getPCY()]);
    dijkstrasMethod(w[200][200]->grid, 0);
    printf("\n");
    dijkstrasMethod(w[200][200]->grid, 1);
    printf("Coordinates: (%d, %d)", x -200, y -200);
    printf("Enter a direction or type q to quit: ");
    printf("\n");
    scanf("%c", &userInput);
    while (userInput != 'q')
    {
        
        if(userInput == 'n'){
            if(y == 0){
                printf("Out of bounds!\n");
                printGrid(w[x][y]->grid);
                printf("\n");
                printf("Enter a direction or type q to quit: ");
                printf("\n");
                scanf(" %c", &userInput);
                continue;
            }
            y = y -1;
            if(w[x][y]==NULL){
                w[x][y] = malloc((sizeof(map_t)));
                w[x][y]->sExit = (w[x][y+1]->nExit);
                if(w[x-1][y] != NULL && (x != 0)){
                    // printf("wExit should be %d", *(w[x-1][y]->eExit) );
                    w[x][y]->wExit = (w[x-1][y]->eExit);
                }
                else{//add print statements so it works!
                    // printf("wExit is random");
                    w[x][y]->wExit = (rand() % 17) + 2;
                }
                if(w[x+1][y] != NULL && (x != 400)){
                    w[x][y]->eExit = (w[x+1][y]->wExit);
                }
                else{
                    // printf("eExit is random");
                    w[x][y]->eExit = (rand() % 17) + 2;
                }
                if(w[x][y-1]!= NULL && (y != 0)){
                    w[x][y]->nExit= (w[x][y-1]->sExit); 
                }
                else{
                    // printf("nExit is random");
                    w[x][y]->nExit = (rand() % 76) + 2;//might need an if statement as well
                }

                // initMap(w[x][y],x,y);//I need to check this with TA
                chance1 = probabilityOfPokeStuff(x,y);
                chanceToSend = (rand() % 100) + 0;//pokemart
                chanceToSend2 = (rand() % 100) + 0;//pokecenter
                if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                    initMap(w[x][y],x,y,1,1);//I need to check this with TA
                }
                else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,0,0);//I need to check this with TA
                }
                else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,1,0);
                }
                else{
                    initMap(w[x][y],x,y,0,1);
                }
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
            else{
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
        }

        if(userInput == 's'){ 
            if(y == 400){
                printf("Out of bounds!\n");
                printGrid(w[x][y]->grid);
                printf("\n");
                printf("Enter a direction or type q to quit: ");
                printf("\n");
                scanf(" %c", &userInput);
                continue;
            }
            y = y +1;
            if(w[x][y]==NULL){
                w[x][y] = malloc((sizeof(map_t)));
                w[x][y]->nExit = (w[x][y-1]->sExit);
                if(w[x-1][y] != NULL && (x != 0)){
                    w[x][y]->wExit= (w[x-1][y]->eExit);
                }
                else{
                    w[x][y]->wExit = (rand() % 17) + 2;
                }
                if(w[x+1][y] != NULL && (x != 400)){
                    w[x][y]->eExit = (w[x+1][y]->wExit);
                }
                else{
                    w[x][y]->eExit = (rand() % 17) + 2;
                }
                if(w[x][y+1]!=NULL && (y != 400)){
                    w[x][y]->sExit= (w[x][y+1]->nExit);
                }
                else{
                    w[x][y]->sExit = (rand() % 76) + 2;//might need an if statement as well
                }
        
                // initMap(w[x][y],x,y);//I need to check this with TA
                chance1 = probabilityOfPokeStuff(x,y);
                chanceToSend = (rand() % 100) + 0;//pokemart
                chanceToSend2 = (rand() % 100) + 0;//pokecenter
                if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                    initMap(w[x][y],x,y,1,1);//I need to check this with TA
                }
                else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,0,0);//I need to check this with TA
                }
                else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,1,0);
                }
                else{
                    initMap(w[x][y],x,y,0,1);
                }
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
            else{
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
        }

        if(userInput == 'w'){ 
            if (x == 0){
                printf("Out of bounds!\n");
                printGrid(w[x][y]->grid);
                printf("\n");
                printf("Enter a direction or type q to quit: ");
                printf("\n");
                scanf(" %c", &userInput);
                continue;
            }
            x = x-1;
            if(w[x][y]==NULL){
                w[x][y] = malloc((sizeof(map_t)));
                w[x][y]->eExit = (w[x+1][y]->wExit);
                if(w[x][y+1] != NULL && (y != 400)){
                    w[x][y]->sExit= (w[x][y+1]->nExit);
                }
                else{
                    w[x][y]->sExit = (rand() % 76) + 2;
                }
                if(w[x][y-1] != NULL && (y != 0)){
                    w[x][y]->nExit = (w[x][y-1]->sExit);
                }
                else{
                    w[x][y]->nExit = (rand() % 76) + 2;
                }
                if(w[x-1][y] != NULL && (x != 0)){
                    w[x][y]->wExit= (w[x-1][y]->eExit);
                }
                else{
                    w[x][y]->wExit = (rand() % 17) + 2;//might need an if statement as well
                }

                // initMap(w[x][y],x,y);//I need to check this with TA
                chance1 = probabilityOfPokeStuff(x,y);
                chanceToSend = (rand() % 100) + 0;//pokemart
                chanceToSend2 = (rand() % 100) + 0;//pokecenter
                if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                    initMap(w[x][y],x,y,1,1);//I need to check this with TA
                }
                else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,0,0);//I need to check this with TA
                }
                else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,1,0);
                }
                else{
                    initMap(w[x][y],x,y,0,1);
                }
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
            else{
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
        }

        if(userInput == 'e'){ 
            if(x == 400){
                printf("Out of bounds!\n");
                printGrid(w[x][y]->grid);
                printf("\n");
                printf("Enter a direction or type q to quit: ");
                printf("\n");
                scanf(" %c", &userInput);
                continue;
            }
            x = x+1;
            if(w[x][y]==NULL){
                w[x][y] = malloc((sizeof(map_t)));
                w[x][y]->wExit = (w[x-1][y]->eExit);
                if(w[x][y+1] != NULL && (y != 400)){
                    w[x][y]->sExit= (w[x][y+1]->nExit);
                }
                else{
                    w[x][y]->sExit = (rand() % 76) + 2;
                }
                if(w[x][y-1] != NULL && (y != 0)){
                    w[x][y]->nExit = (w[x][y-1]->sExit);
                }
                else{
                    w[x][y]->nExit = (rand() % 76) + 2;
                }
                if(w[x+1][y]!= NULL && (x != 400)){
                    w[x][y]->eExit= (w[x+1][y]->wExit);
                }
                else{
                    w[x][y]->eExit = (rand() % 17) + 2; //might need an if statement as well
                }
        
                // initMap(w[x][y],x,y);//I need to check this with TA
                chance1 = probabilityOfPokeStuff(x,y);
                chanceToSend = (rand() % 100) + 0;//pokemart
                chanceToSend2 = (rand() % 100) + 0;//pokecenter
                if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                    initMap(w[x][y],x,y,1,1);//I need to check this with TA
                }
                else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,0,0);//I need to check this with TA
                }
                else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,1,0);
                }
                else{
                    initMap(w[x][y],x,y,0,1);
                }
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
            else{
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
        }


        if(userInput == 'f'){
            scanf(" %d %d", &x, &y);
            if((x < -200) || (x > 200) || (y < -200) || (y > 200)){
                printf("Out of bounds!\n");
                break;
            }
            printf("flying to (%d,%d)\n", x, y);
            x = x + 200;
            y = y + 200;
            
            if(w[x][y] == NULL){
                w[x][y] = malloc((sizeof(map_t)));
                if(w[x][y-1]!=NULL && (y != 0)){
                    w[x][y]->nExit = w[x][y-1]->sExit;
                }
                else{
                    w[x][y]->nExit = (rand() % 76) + 2;
                }
                if(w[x][y+1]!=NULL && (y != 400)){
                    w[x][y]->sExit = w[x][y+1]->nExit;
                }
                else{
                    w[x][y]->sExit = (rand() % 76) + 2;
                }
                if(w[x+1][y]!=NULL && (x != 400)){
                    w[x][y]->eExit = w[x+1][y]->wExit;
                }
                else{
                    w[x][y]->eExit = (rand() % 17) + 2;
                }
                if(w[x-1][y] != NULL && (x != 0)){
                    w[x][y]->wExit = w[x-1][y]->eExit;
                }
                else{
                    w[x][y]->wExit = (rand() % 17) + 2;
                }
                chance1 = probabilityOfPokeStuff(x,y);
                chanceToSend = (rand() % 100) + 0;//pokemart
                chanceToSend2 = (rand() % 100) + 0;//pokecenter
                if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
                    initMap(w[x][y],x,y,1,1);//I need to check this with TA
                }
                else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,0,0);//I need to check this with TA
                }
                else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
                    initMap(w[x][y],x,y,1,0);
                }
                else{
                    initMap(w[x][y],x,y,0,1);
                }
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
            else{
                printf("\n");
                printGrid(w[x][y]->grid);
                printf("Coordinates: (%d, %d)", x -200, y -200);
            }
        }


        if(userInput != 'f' && userInput != 'n' && userInput != 's' && userInput != 'e' && userInput != 'w' && userInput != 'q'){
            printf("Incorrect input!");
        }
        printf("\n");
        printf("Enter a direction or type q to quit: ");
        printf("\n");
        scanf(" %c", &userInput);
    }
    

}

int probabilityOfPokeStuff(int x, int y){
    int probability1 = abs(x - 200) + abs(y - 200);
    if(probability1 < 200){
        int probability = ((-45 * probability1)/200) + 50;
        return probability;
    }
    else{
        return 5;
    }
}

int costOfTerrain(int x, int y, int grid[21][80], int hikerOrRider){//0 for hiker, 1 for rider
  if(hikerOrRider == 0){
    if(grid[x][y] == 2){
      return 10;
    }
    else if(grid[x][y] == 8){
      return 0;
    }
    else if((grid[x][y] == 3) || grid[x][y] == 4){
      return 50;
    }
    else if(grid[x][y] == 5){
      return 15;
    }
    else if(grid[x][y] == 6){
      return 10;
    }
    else if(grid[x][y] == 1){
      return INT_MAX;
    }
    else if(grid[x][y] == 7){
      return INT_MAX;
    }
    else{
      return INT_MAX;
    }
  }
  else{
    if(grid[x][y] == 2){
      return 10;
    }
    else if(grid[x][y] == 8){
      return 0;
    }
    else if((grid[x][y] == 3) || grid[x][y] == 4){
      return 50;
    }
    else if(grid[x][y] == 5){
      return 20;
    }
    else if(grid[x][y] == 6){
      return 10;
    }
    else if(grid[x][y] == 1){
      return INT_MAX;
    }
    else if(grid[x][y] == 7){
      return INT_MAX;
    }
    else{
      return INT_MAX;
    }
  }
    return 0;
  
}

void printDistTable(int distance[21][80]){
  for(int i = 0; i < 21; i++){
    for(int j =0; j< 80; j++){
      if(distance[i][j] == INT_MAX){
        printf("   ");
      }
      else if((i==getPCX()) && (j == getPCY())){
        printf("00 ");
      }
      else{
        printf("%02d ", distance[i][j] % 100);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void exitsUnreachable(int distance[21][80]){
  for (int i = 0; i < 21; i++)
  {
    distance[i][0] = INT_MAX;
    distance[i][79] = INT_MAX;
  }
  for (int i = 0; i < 80; i++)
  {
    distance[0][i] = INT_MAX;
    distance[20][i] = INT_MAX;
  }
  
}

void dijkstrasMethod(int grid[21][80], int hikerOrRider){
  int visited[21][80] = {0};//all nodes not visited.
  int distance[21][80];
  for(int i = 0; i < 21; i++){
    for(int j =0; j< 80; j++){
      distance[i][j] = INT_MAX;
    }
  }
  distance[getPCX()][getPCY()] = 0;


  // printf("Making a queue\n");
  Node* pq = newNode(getPCX(), getPCY(), 0);
  // printf("Made a queue\n");
  // printf("%d %d\n", pq->xCoord, pq->yCoord);

  while(!isEmpty(&pq)){
    Node* current = peek(&pq);
    int cost;

    if(visited[current->xCoord][current->yCoord] == 0){

      if(current->xCoord - 1 >= 0 ){
        cost = costOfTerrain(current->xCoord-1, current->yCoord, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord -1][current->yCoord] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord -1][current->yCoord] = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord -1, current->yCoord, distance[current->xCoord -1][current->yCoord]);
        }
        }
      }
       if((current->yCoord + 1 <= 79) && (current->xCoord -1 >= 0)){
        cost = costOfTerrain(current->xCoord -1, current->yCoord +1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord - 1][current->yCoord +1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord - 1][current->yCoord +1]  = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord -1, current->yCoord +1, distance[current->xCoord - 1][current->yCoord +1]);
        }
        }
      }
      if(current->yCoord + 1 <= 79 ){
        cost = costOfTerrain(current->xCoord, current->yCoord +1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord][current->yCoord +1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord][current->yCoord +1] = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord, current->yCoord +1, distance[current->xCoord][current->yCoord +1]);
        }
        }
      }
      if((current->yCoord + 1 <= 79) && (current->xCoord + 1 <= 20)){
        cost = costOfTerrain(current->xCoord +1, current->yCoord +1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord + 1][current->yCoord +1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord + 1][current->yCoord +1]  = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord +1, current->yCoord +1, distance[current->xCoord + 1][current->yCoord +1]);
        }
        }
      }
      if(current->xCoord + 1 <= 20 ){
        cost = costOfTerrain(current->xCoord +1, current->yCoord, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord +1][current->yCoord] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord +1][current->yCoord] = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord +1, current->yCoord, distance[current->xCoord +1][current->yCoord]);
        }
        }
      }
      if((current->yCoord - 1 >= 0) && (current->xCoord +1 <= 20)){
        cost = costOfTerrain(current->xCoord +1, current->yCoord -1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord +1][current->yCoord -1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord +1][current->yCoord -1]  = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord +1, current->yCoord -1, distance[current->xCoord + 1][current->yCoord -1]);
        }
        }
      }
      if(current->yCoord - 1 >= 0 ){
        cost = costOfTerrain(current->xCoord, current->yCoord -1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord][current->yCoord -1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord][current->yCoord -1] = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord, current->yCoord -1, distance[current->xCoord][current->yCoord -1]);
        }
        }
      }
      if((current->yCoord - 1 >= 0) && (current->xCoord - 1 >= 0)){
        cost = costOfTerrain(current->xCoord -1, current->yCoord -1, grid, hikerOrRider);
        if(cost != INT_MAX){
        if(distance[current->xCoord -1][current->yCoord -1] > cost + distance[current->xCoord][current->yCoord]){
          distance[current->xCoord - 1][current->yCoord -1]  = cost + distance[current->xCoord][current->yCoord];
          push(&pq, current->xCoord -1, current->yCoord -1, distance[current->xCoord - 1][current->yCoord -1]);
        }
        }
      }
    }
    visited[current->xCoord][current->yCoord] = 1;
    pop(&pq);
  }

  exitsUnreachable(distance);
  printDistTable(distance);
}
