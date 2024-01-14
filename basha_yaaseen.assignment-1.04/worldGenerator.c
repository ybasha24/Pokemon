#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>
#include<unistd.h>
#include "worldGenerator.h"
#include "priorityQueue.h"
#include "characterQueue.h"

int probabilityOfPokeStuff(int x, int y);
void dijkstrasMethod(int grid[21][80], int hikerOrRider, int distance[21][80]);
void printDistTable(int distance[21][80]);
int costOfTerrainForHiker(int x, int y, int grid[21][80]);
void moveRival(int grid[21][80], int characterGrid[21][80], int distanceMap[21][80], Characters **head);
int costOfTerrain(int x, int y, int grid[21][80], int hikerOrRider);
void moveHiker(int grid[21][80], int characterGrid[21][80], int distanceMap[21][80], Characters **head);
void moveExplorer(int grid[21][80], int characterGrid[21][80], int *direction, Characters **head);
void moveWander(int grid[21][80], int characterGrid[21][80], int *direction, Characters **head);
void movePacer(int grid[21][80], int characterGrid[21][80], int *direction, Characters **head);
void placeCharacter(int grid[21][80], int characterGrid[21][80], int characterNumber, int seqNum, int direction, Characters **head);

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
    printf("\n");
    int characterMovements [21][80] = {0};
    for(int i = 0; i < 21; i++){
      for(int j =0; j< 80; j++){
        characterMovements[i][j] = w[200][200]->grid[i][j];
      }
    }
    //place npc method where parameter is grid, character grid, queue, character number, direction 
    int pcXcoord = getPCX();
    int pcYcoord = getPCY();

    printGrid(characterMovements);

    Characters *cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
    int distanceMapForHiker[21][80] ={0};
    int distanceMapForRider[21][80] ={0};
    dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
    dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);

    if(argc == 1){
      placeCharacter(w[200][200]->grid, characterMovements, 10, 1, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterMovements, 11, 2, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterMovements, 12, 3, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterMovements, 13, 4, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterMovements, 14, 5, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterMovements, 14, 7, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterMovements, 14, 8, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterMovements, 15, 6, -1, &cPQ);

      while(1){
        Characters *peeked = peekCharacter(&cPQ);
        if(peeked->symbolOfCharacter == '@'){
          printGrid(characterMovements);
          usleep(250000);
          int nextTurn = peeked->nextTurn;
          popCharacter(&cPQ);
          pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + 10, 0, '@', -1);
        }
        if(peeked->symbolOfCharacter == 'r'){
          moveRival(w[200][200]->grid, characterMovements, distanceMapForRider, &cPQ);
        }
        if(peeked->symbolOfCharacter == 'h'){
          moveHiker(w[200][200]->grid, characterMovements, distanceMapForHiker, &cPQ);
        }
        if(peeked->symbolOfCharacter == 'p'){
          int direction = peeked->direction;
          movePacer(w[200][200]->grid, characterMovements, &direction, &cPQ);
        }

        if(peeked->symbolOfCharacter == 's'){
          int xCoord = peeked->xCoord;
          int yCoord = peeked->yCoord;
          int seqNum = peeked->seqNum;
          int nextTurn = peeked->nextTurn + costOfTerrain(xCoord, yCoord, w[200][200]->grid, 1);
          popCharacter(&cPQ);
          pushCharacter(&cPQ, xCoord, yCoord, nextTurn, seqNum, 's', -1);
        }

        if(peeked->symbolOfCharacter == 'e'){
          int direction = peeked->direction;
          moveExplorer(w[200][200]->grid, characterMovements, &direction, &cPQ);
        }
        if(peeked->symbolOfCharacter == 'w'){
          int direction = peeked->direction;
          moveWander(w[200][200]->grid, characterMovements, &direction, &cPQ);
        }
      }
    }
    else{//user input is there!
      int numOfTrainers = atoi(argv[1]);

      if(numOfTrainers == 1){//just hiker!
        placeCharacter(w[200][200]->grid, characterMovements, 10, 1, -1, &cPQ);
        while(1){
          Characters *peeked = peekCharacter(&cPQ);
          if(peeked->symbolOfCharacter == '@'){
            printGrid(characterMovements);
            usleep(250000);
            int nextTurn = peeked->nextTurn;
            popCharacter(&cPQ);
            pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + 10, 0, '@', -1);
          }
          if(peeked->symbolOfCharacter == 'r'){
            moveRival(w[200][200]->grid, characterMovements, distanceMapForRider, &cPQ);
          }
          if(peeked->symbolOfCharacter == 'h'){
            moveHiker(w[200][200]->grid, characterMovements, distanceMapForHiker, &cPQ);
          }
        }
      }
      else if(numOfTrainers == 2){
        placeCharacter(w[200][200]->grid, characterMovements, 10, 1, -1, &cPQ);
        placeCharacter(w[200][200]->grid, characterMovements, 11, 2, -1, &cPQ);
        while(1){
          Characters *peeked = peekCharacter(&cPQ);
          if(peeked->symbolOfCharacter == '@'){
            printGrid(characterMovements);
            usleep(250000);
            int nextTurn = peeked->nextTurn;
            popCharacter(&cPQ);
            pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + 10, 0, '@', -1);
          }
          if(peeked->symbolOfCharacter == 'r'){
            moveRival(w[200][200]->grid, characterMovements, distanceMapForRider, &cPQ);
          }
          if(peeked->symbolOfCharacter == 'h'){
            moveHiker(w[200][200]->grid, characterMovements, distanceMapForHiker, &cPQ);
          }
        }
      }
      else{//numtrainers is more than 2
        placeCharacter(w[200][200]->grid, characterMovements, 10, 1, -1, &cPQ);
        placeCharacter(w[200][200]->grid, characterMovements, 11, 2, -1, &cPQ);
        int seqNum = 3;
        int counter = 2;
        while(counter != numOfTrainers){
          placeCharacter(w[200][200]->grid, characterMovements, 12, seqNum, -1, &cPQ);
          seqNum += 1;
          counter +=1;
          if(counter == numOfTrainers){
            break;
          }
          placeCharacter(w[200][200]->grid, characterMovements, 13, seqNum, -1, &cPQ);
          seqNum += 1;
          counter +=1;
          if(counter == numOfTrainers){
            break;
          }
          placeCharacter(w[200][200]->grid, characterMovements, 14, seqNum, -1, &cPQ);
          seqNum += 1;
          counter +=1;
          if(counter == numOfTrainers){
            break;
          }
          placeCharacter(w[200][200]->grid, characterMovements, 15, seqNum, -1, &cPQ);
          seqNum += 1;
          counter +=1;
          if(counter == numOfTrainers){
            break;
          }
        }

        while(1){
          Characters *peeked = peekCharacter(&cPQ);
          if(peeked->symbolOfCharacter == '@'){
            printGrid(characterMovements);
            usleep(250000);
            int nextTurn = peeked->nextTurn;
            popCharacter(&cPQ);
            pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + 10, 0, '@', -1);
          }
          if(peeked->symbolOfCharacter == 'r'){
            moveRival(w[200][200]->grid, characterMovements, distanceMapForRider, &cPQ);
          }
          if(peeked->symbolOfCharacter == 'h'){
            moveHiker(w[200][200]->grid, characterMovements, distanceMapForHiker, &cPQ);
          }
          if(peeked->symbolOfCharacter == 'p'){
            int direction = peeked->direction;
            movePacer(w[200][200]->grid, characterMovements, &direction, &cPQ);
          }

          if(peeked->symbolOfCharacter == 's'){
            int xCoord = peeked->xCoord;
            int yCoord = peeked->yCoord;
            int seqNum = peeked->seqNum;
            int nextTurn = peeked->nextTurn + costOfTerrain(xCoord, yCoord, w[200][200]->grid, 1);
            popCharacter(&cPQ);
            pushCharacter(&cPQ, xCoord, yCoord, nextTurn, seqNum, 's', -1);
          }

          if(peeked->symbolOfCharacter == 'e'){
            int direction = peeked->direction;
            moveExplorer(w[200][200]->grid, characterMovements, &direction, &cPQ);
          }
          if(peeked->symbolOfCharacter == 'w'){
            int direction = peeked->direction;
            moveWander(w[200][200]->grid, characterMovements, &direction, &cPQ);
          }
        }
      }

    }

    // printf("Going in!\n");
    // printf("%d %d\n", getPCX(), getPCY());
    // printf("%d\n", w[200][200]->grid[getPCX()][getPCY()]);
    // dijkstrasMethod(w[200][200]->grid, 0);
    // printf("\n");
    // dijkstrasMethod(w[200][200]->grid, 1);
    // printf("Coordinates: (%d, %d)", x -200, y -200);
    // printf("Enter a direction or type q to quit: ");
    // printf("\n");
    // scanf("%c", &userInput);
    // while (userInput != 'q')
    // {
        
    //     if(userInput == 'n'){
    //         if(y == 0){
    //             printf("Out of bounds!\n");
    //             printGrid(w[x][y]->grid);
    //             printf("\n");
    //             printf("Enter a direction or type q to quit: ");
    //             printf("\n");
    //             scanf(" %c", &userInput);
    //             continue;
    //         }
    //         y = y -1;
    //         if(w[x][y]==NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             w[x][y]->sExit = (w[x][y+1]->nExit);
    //             if(w[x-1][y] != NULL && (x != 0)){
    //                 // printf("wExit should be %d", *(w[x-1][y]->eExit) );
    //                 w[x][y]->wExit = (w[x-1][y]->eExit);
    //             }
    //             else{//add print statements so it works!
    //                 // printf("wExit is random");
    //                 w[x][y]->wExit = (rand() % 17) + 2;
    //             }
    //             if(w[x+1][y] != NULL && (x != 400)){
    //                 w[x][y]->eExit = (w[x+1][y]->wExit);
    //             }
    //             else{
    //                 // printf("eExit is random");
    //                 w[x][y]->eExit = (rand() % 17) + 2;
    //             }
    //             if(w[x][y-1]!= NULL && (y != 0)){
    //                 w[x][y]->nExit= (w[x][y-1]->sExit); 
    //             }
    //             else{
    //                 // printf("nExit is random");
    //                 w[x][y]->nExit = (rand() % 76) + 2;//might need an if statement as well
    //             }

    //             // initMap(w[x][y],x,y);//I need to check this with TA
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }

    //     if(userInput == 's'){ 
    //         if(y == 400){
    //             printf("Out of bounds!\n");
    //             printGrid(w[x][y]->grid);
    //             printf("\n");
    //             printf("Enter a direction or type q to quit: ");
    //             printf("\n");
    //             scanf(" %c", &userInput);
    //             continue;
    //         }
    //         y = y +1;
    //         if(w[x][y]==NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             w[x][y]->nExit = (w[x][y-1]->sExit);
    //             if(w[x-1][y] != NULL && (x != 0)){
    //                 w[x][y]->wExit= (w[x-1][y]->eExit);
    //             }
    //             else{
    //                 w[x][y]->wExit = (rand() % 17) + 2;
    //             }
    //             if(w[x+1][y] != NULL && (x != 400)){
    //                 w[x][y]->eExit = (w[x+1][y]->wExit);
    //             }
    //             else{
    //                 w[x][y]->eExit = (rand() % 17) + 2;
    //             }
    //             if(w[x][y+1]!=NULL && (y != 400)){
    //                 w[x][y]->sExit= (w[x][y+1]->nExit);
    //             }
    //             else{
    //                 w[x][y]->sExit = (rand() % 76) + 2;//might need an if statement as well
    //             }
        
    //             // initMap(w[x][y],x,y);//I need to check this with TA
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }

    //     if(userInput == 'w'){ 
    //         if (x == 0){
    //             printf("Out of bounds!\n");
    //             printGrid(w[x][y]->grid);
    //             printf("\n");
    //             printf("Enter a direction or type q to quit: ");
    //             printf("\n");
    //             scanf(" %c", &userInput);
    //             continue;
    //         }
    //         x = x-1;
    //         if(w[x][y]==NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             w[x][y]->eExit = (w[x+1][y]->wExit);
    //             if(w[x][y+1] != NULL && (y != 400)){
    //                 w[x][y]->sExit= (w[x][y+1]->nExit);
    //             }
    //             else{
    //                 w[x][y]->sExit = (rand() % 76) + 2;
    //             }
    //             if(w[x][y-1] != NULL && (y != 0)){
    //                 w[x][y]->nExit = (w[x][y-1]->sExit);
    //             }
    //             else{
    //                 w[x][y]->nExit = (rand() % 76) + 2;
    //             }
    //             if(w[x-1][y] != NULL && (x != 0)){
    //                 w[x][y]->wExit= (w[x-1][y]->eExit);
    //             }
    //             else{
    //                 w[x][y]->wExit = (rand() % 17) + 2;//might need an if statement as well
    //             }

    //             // initMap(w[x][y],x,y);//I need to check this with TA
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }

    //     if(userInput == 'e'){ 
    //         if(x == 400){
    //             printf("Out of bounds!\n");
    //             printGrid(w[x][y]->grid);
    //             printf("\n");
    //             printf("Enter a direction or type q to quit: ");
    //             printf("\n");
    //             scanf(" %c", &userInput);
    //             continue;
    //         }
    //         x = x+1;
    //         if(w[x][y]==NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             w[x][y]->wExit = (w[x-1][y]->eExit);
    //             if(w[x][y+1] != NULL && (y != 400)){
    //                 w[x][y]->sExit= (w[x][y+1]->nExit);
    //             }
    //             else{
    //                 w[x][y]->sExit = (rand() % 76) + 2;
    //             }
    //             if(w[x][y-1] != NULL && (y != 0)){
    //                 w[x][y]->nExit = (w[x][y-1]->sExit);
    //             }
    //             else{
    //                 w[x][y]->nExit = (rand() % 76) + 2;
    //             }
    //             if(w[x+1][y]!= NULL && (x != 400)){
    //                 w[x][y]->eExit= (w[x+1][y]->wExit);
    //             }
    //             else{
    //                 w[x][y]->eExit = (rand() % 17) + 2; //might need an if statement as well
    //             }
        
    //             // initMap(w[x][y],x,y);//I need to check this with TA
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }


    //     if(userInput == 'f'){
    //         scanf(" %d %d", &x, &y);
    //         if((x < -200) || (x > 200) || (y < -200) || (y > 200)){
    //             printf("Out of bounds!\n");
    //             break;
    //         }
    //         printf("flying to (%d,%d)\n", x, y);
    //         x = x + 200;
    //         y = y + 200;
            
    //         if(w[x][y] == NULL){
    //             w[x][y] = malloc((sizeof(map_t)));
    //             if(w[x][y-1]!=NULL && (y != 0)){
    //                 w[x][y]->nExit = w[x][y-1]->sExit;
    //             }
    //             else{
    //                 w[x][y]->nExit = (rand() % 76) + 2;
    //             }
    //             if(w[x][y+1]!=NULL && (y != 400)){
    //                 w[x][y]->sExit = w[x][y+1]->nExit;
    //             }
    //             else{
    //                 w[x][y]->sExit = (rand() % 76) + 2;
    //             }
    //             if(w[x+1][y]!=NULL && (x != 400)){
    //                 w[x][y]->eExit = w[x+1][y]->wExit;
    //             }
    //             else{
    //                 w[x][y]->eExit = (rand() % 17) + 2;
    //             }
    //             if(w[x-1][y] != NULL && (x != 0)){
    //                 w[x][y]->wExit = w[x-1][y]->eExit;
    //             }
    //             else{
    //                 w[x][y]->wExit = (rand() % 17) + 2;
    //             }
    //             chance1 = probabilityOfPokeStuff(x,y);
    //             chanceToSend = (rand() % 100) + 0;//pokemart
    //             chanceToSend2 = (rand() % 100) + 0;//pokecenter
    //             if((chanceToSend <= chance1) && (chanceToSend2 <= chance1)){
    //                 initMap(w[x][y],x,y,1,1);//I need to check this with TA
    //             }
    //             else if((chanceToSend >= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,0,0);//I need to check this with TA
    //             }
    //             else if((chanceToSend <= chance1) && (chanceToSend2 >= chance1)){
    //                 initMap(w[x][y],x,y,1,0);
    //             }
    //             else{
    //                 initMap(w[x][y],x,y,0,1);
    //             }
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //         else{
    //             printf("\n");
    //             printGrid(w[x][y]->grid);
    //             printf("Coordinates: (%d, %d)", x -200, y -200);
    //         }
    //     }


    //     if(userInput != 'f' && userInput != 'n' && userInput != 's' && userInput != 'e' && userInput != 'w' && userInput != 'q'){
    //         printf("Incorrect input!");
    //     }
    //     printf("\n");
    //     printf("Enter a direction or type q to quit: ");
    //     printf("\n");
    //     scanf(" %c", &userInput);
    // }
}
//have direction in the place character struct??
void placeCharacter(int grid[21][80], int characterGrid[21][80], int characterNumber, int seqNum, int direction, Characters **head){
  int xCoord = ((rand() % 17) + 2);
  int yCoord = ((rand() % 76) + 2);
  //(1 || 2 || 7 || 8 || 3 || 4 || 10 || 11 || 12 || 13 || 14 )
  while((characterGrid[xCoord][yCoord] == 1) || (characterGrid[xCoord][yCoord] == 2) || (characterGrid[xCoord][yCoord] == 3) || (characterGrid[xCoord][yCoord] == 4) 
  || (characterGrid[xCoord][yCoord] == 7) || (characterGrid[xCoord][yCoord] == 8) || (characterGrid[xCoord][yCoord] == 10) || (characterGrid[xCoord][yCoord] == 11)
  || (characterGrid[xCoord][yCoord] == 12)|| (characterGrid[xCoord][yCoord] == 13)|| (characterGrid[xCoord][yCoord] == 14)|| (characterGrid[xCoord][yCoord] == 15)){
    xCoord = ((rand() % 17) + 2);
    yCoord = ((rand() % 76) + 2);
  }

  characterGrid[xCoord][yCoord] = characterNumber;

  if(characterNumber == 10){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'h', -1);
  }
  if(characterNumber == 11){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'r', -1);
  }
  if(characterNumber == 12){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'p', -1);
  }
  if(characterNumber == 13){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 's', -1);
  }
  if(characterNumber == 14){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'e', -1);
  }
  if(characterNumber == 15){
    pushCharacter(head, xCoord, yCoord, 0, seqNum, 'w', -1);
  }
}

void movePacer(int grid[21][80], int characterGrid[21][80], int *direction, Characters **head){
        Characters *peeked = peekCharacter(head);
        if(*direction == -1){
          *direction = (rand() % 8) + 1;
        }
        int xCoord = peeked->xCoord;
        int yCoord = peeked->yCoord;
        int nextTurn = peeked->nextTurn;
        int seqNum = peeked->seqNum;
        characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
        int change = 0;
        int count = 0;
        while(change != 1 && count != 9){
          if(*direction ==1){//go north
            if(xCoord > 1 && (costOfTerrain(xCoord -1, yCoord, characterGrid, 1) != INT_MAX)&& (characterGrid[xCoord-1][yCoord] != 8)){
              popCharacter(head);
              pushCharacter(head, xCoord-1, yCoord, nextTurn + costOfTerrain(xCoord -1, yCoord, characterGrid, 1), seqNum, 'p', *direction);
              characterGrid[xCoord-1][yCoord] = 12;
              change = 1;
              break;
            }
            else{
              *direction = 5;//s
              count+=1;
              continue;
            }
          }

          if(*direction == 2){//ne
            if(xCoord > 1 && yCoord < 78 && (costOfTerrain(xCoord -1, yCoord+1, characterGrid, 1) != INT_MAX)&& (characterGrid[xCoord-1][yCoord+1] != 8)){
              popCharacter(head);
              pushCharacter(head, xCoord-1, yCoord+1, nextTurn + costOfTerrain(xCoord -1, yCoord+1, characterGrid, 1), seqNum, 'p', *direction);
              characterGrid[xCoord-1][yCoord+1] = 12;
              change = 1;
              break;
            }
            else{
              *direction = 6;//se
              count+=1;
              continue;
            }
          }

          if(*direction == 3){//e
            if(yCoord < 78 && (costOfTerrain(xCoord, yCoord+1, characterGrid, 1) != INT_MAX)&& (characterGrid[xCoord][yCoord+1] != 8)){
              popCharacter(head);
              pushCharacter(head, xCoord, yCoord+1, nextTurn + costOfTerrain(xCoord, yCoord+1, characterGrid, 1), seqNum, 'p', *direction);
              characterGrid[xCoord][yCoord+1] = 12;
              change =1;
              break;
            }
            else{
              *direction = 7;//w
              count+=1;
              continue;
            }
          }

          if(*direction == 4){//se
            if(xCoord < 19 && yCoord < 78 && (costOfTerrain(xCoord +1, yCoord+1, characterGrid, 1) != INT_MAX)&& (characterGrid[xCoord+1][yCoord+1] != 8)){
              popCharacter(head);
              pushCharacter(head, xCoord+1, yCoord+1, nextTurn + costOfTerrain(xCoord +1, yCoord+1, characterGrid, 1), seqNum, 'p', *direction);
              characterGrid[xCoord+1][yCoord+1] = 12;
              change = 1;
              break;
            }
            else{
              *direction = 8;//nw
              count+=1;
              continue;
            }
          }

          if(*direction == 5){//s
            if(xCoord < 19 && (costOfTerrain(xCoord +1, yCoord, characterGrid, 1) != INT_MAX)&& (characterGrid[xCoord+1][yCoord] != 8)){
              popCharacter(head);
              pushCharacter(head, xCoord+1, yCoord, nextTurn + costOfTerrain(xCoord +1, yCoord, characterGrid, 1), seqNum, 'p', *direction);
              characterGrid[xCoord+1][yCoord] = 12;
              change =1;
              break;
            }
            else{
              *direction = 1;//n
              count+=1;
              continue;
            }
          }

          if(*direction == 6){//sw
            if(xCoord < 19 && yCoord > 1 && (costOfTerrain(xCoord +1, yCoord-1, characterGrid, 1) != INT_MAX)&& (characterGrid[xCoord+1][yCoord-1] != 8)){
              popCharacter(head);
              pushCharacter(head, xCoord+1, yCoord-1, nextTurn + costOfTerrain(xCoord +1, yCoord-1, characterGrid, 1), seqNum, 'p', *direction);
              characterGrid[xCoord+1][yCoord-1] = 12;
              change = 1;
              break;
            }
            else{
              *direction = 2;//nw
              count+=1;
              continue;
            }
          }

          if(*direction == 7){//w
            if(yCoord > 1 && (costOfTerrain(xCoord, yCoord-1, characterGrid, 1) != INT_MAX)&& (characterGrid[xCoord][yCoord-1] != 8)){
              popCharacter(head);
              pushCharacter(head, xCoord, yCoord-1, nextTurn + costOfTerrain(xCoord, yCoord-1, characterGrid, 1), seqNum, 'p', *direction);
              characterGrid[xCoord][yCoord-1] = 12;
              change = 1;
              break;
            }
            else{
              *direction = 3;//w
              count+=1;
              continue;
            }
          }

          if(*direction == 8){//nw
            if(xCoord > 1 && yCoord > 1 && (costOfTerrain(xCoord -1, yCoord-1, characterGrid, 1) != INT_MAX)&& (characterGrid[xCoord-1][yCoord-1] != 8)){
              popCharacter(head);
              pushCharacter(head, xCoord-1, yCoord-1, nextTurn + costOfTerrain(xCoord -1, yCoord-1, characterGrid, 1), seqNum, 'p', *direction);
              characterGrid[xCoord-1][yCoord-1] = 12;
              change = 1;
              break;
            }
            else{
              *direction = 4;//nw
              count+=1;
              continue;
            }
          }

        }
        if(count == 9){
          popCharacter(head);
          pushCharacter(head, xCoord, yCoord, nextTurn + costOfTerrain(xCoord, yCoord, characterGrid, 1), seqNum, 'p', *direction);
          characterGrid[xCoord][yCoord] = 12;
        }
}

void moveExplorer(int grid[21][80], int characterGrid[21][80], int *direction, Characters **head){
  Characters *peeked = peekCharacter(head);
  if(*direction == -1){
     *direction = (rand() % 8) + 1;
  }
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int seqNum = peeked->seqNum;
  int nextTurn = peeked->nextTurn;
  characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  int change = 0;
  int count = 0;
  while(change != 1 && count != 9){
    if(*direction == 1){
      if(xCoord > 1 && (costOfTerrain(xCoord -1, yCoord, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX)&& (characterGrid[xCoord-1][yCoord] != 8)){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord, nextTurn + costOfTerrain(xCoord -1, yCoord, characterGrid, 1), seqNum, 'e', *direction);
        characterGrid[xCoord-1][yCoord] = 14;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 2){
      if(xCoord > 1 && yCoord < 78 && (costOfTerrain(xCoord -1, yCoord+1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX)&& (characterGrid[xCoord-1][yCoord+1] != 8)){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord+1, nextTurn + costOfTerrain(xCoord -1, yCoord+1, characterGrid, 1), seqNum, 'e', *direction);
        characterGrid[xCoord-1][yCoord+1] = 14;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 3){
      if(yCoord < 78 && (costOfTerrain(xCoord, yCoord+1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX)&& (characterGrid[xCoord][yCoord+1] != 8)){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord+1, nextTurn + costOfTerrain(xCoord, yCoord+1, characterGrid, 1), seqNum, 'e', *direction);
        characterGrid[xCoord][yCoord+1] = 14;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 4){
      if(xCoord < 19 && yCoord < 78 && (costOfTerrain(xCoord +1, yCoord+1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)&& (characterGrid[xCoord+1][yCoord+1] != 8)){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord+1, nextTurn + costOfTerrain(xCoord +1, yCoord+1, characterGrid, 1), seqNum, 'e', *direction);
        characterGrid[xCoord+1][yCoord+1] = 14;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 5){
      if(xCoord < 19 && (costOfTerrain(xCoord+1, yCoord, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord, grid, 1) != INT_MAX)&& (characterGrid[xCoord+1][yCoord] != 8)){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord, nextTurn + costOfTerrain(xCoord+1, yCoord, characterGrid, 1), seqNum, 'e', *direction);
        characterGrid[xCoord+1][yCoord] = 14;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 6){
      if(xCoord < 19 && yCoord > 1 && (costOfTerrain(xCoord +1, yCoord-1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX)&& (characterGrid[xCoord+1][yCoord-1] != 8)){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord-1, nextTurn + costOfTerrain(xCoord +1, yCoord-1, characterGrid, 1), seqNum, 'e', *direction);
        characterGrid[xCoord+1][yCoord-1] = 14;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 7){
      if(yCoord > 1 && (costOfTerrain(xCoord, yCoord-1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX)&& (characterGrid[xCoord][yCoord-1] != 8)){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord-1, nextTurn + costOfTerrain(xCoord, yCoord-1, characterGrid, 1), seqNum, 'e', *direction);
        characterGrid[xCoord][yCoord-1] = 14;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 8){
      if(xCoord > 1 && yCoord > 1 && (costOfTerrain(xCoord -1, yCoord-1, characterGrid, 1) != INT_MAX) && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX) && (characterGrid[xCoord-1][yCoord-1] != 8)){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord-1, nextTurn + costOfTerrain(xCoord -1, yCoord-1, characterGrid, 1), seqNum, 'e', *direction);
        characterGrid[xCoord-1][yCoord-1] = 14;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else{
      // printf("Inside Here!!!\n");
      *direction = (rand() % 8) + 1;
      // printf("New Direction is %d\n", *direction);
      continue;
    }
  }
  if(count == 9){
    popCharacter(head);
    pushCharacter(head, xCoord, yCoord, nextTurn + costOfTerrain(xCoord, yCoord, characterGrid, 1), seqNum, 'e', *direction);
    characterGrid[xCoord][yCoord] = 14;
  }
}

void moveWander(int grid[21][80], int characterGrid[21][80], int *direction, Characters **head){
  Characters *peeked = peekCharacter(head);
  if(*direction == -1){
    *direction = (rand() % 8) + 1;
  }
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int seqNum = peeked->seqNum;
  int nextTurn = peeked->nextTurn;
  characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  int change = 0;
  int count = 0;
  while(change != 1 && count != 9){
    if(*direction == 1){
      if(xCoord > 1 && (costOfTerrain(xCoord -1, yCoord, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX) && (characterGrid[xCoord-1][yCoord] == characterGrid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord, nextTurn + costOfTerrain(xCoord -1, yCoord, characterGrid, 1), seqNum, 'w', *direction);
        characterGrid[xCoord-1][yCoord] = 15;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 2){
      if(xCoord > 1 && yCoord < 78 && (costOfTerrain(xCoord -1, yCoord+1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX) && (characterGrid[xCoord-1][yCoord+1] == characterGrid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord+1, nextTurn + costOfTerrain(xCoord -1, yCoord+1, characterGrid, 1), seqNum, 'w', *direction);
        characterGrid[xCoord-1][yCoord+1] = 15;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 3){
      if(yCoord < 78 && (costOfTerrain(xCoord, yCoord+1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX) && (characterGrid[xCoord][yCoord+1] == characterGrid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord+1, nextTurn + costOfTerrain(xCoord, yCoord+1, characterGrid, 1), seqNum, 'w', *direction);
        characterGrid[xCoord][yCoord+1] = 15;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 4){
      if(xCoord < 19 && yCoord < 78 && (costOfTerrain(xCoord +1, yCoord+1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)&& (characterGrid[xCoord+1][yCoord+1] == characterGrid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord+1, nextTurn + costOfTerrain(xCoord +1, yCoord+1, characterGrid, 1), seqNum, 'w', *direction);
        characterGrid[xCoord+1][yCoord+1] = 15;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 5){
      if(xCoord < 19 && (costOfTerrain(xCoord+1, yCoord, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord, grid, 1) != INT_MAX)&& (characterGrid[xCoord+1][yCoord] == characterGrid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord, nextTurn + costOfTerrain(xCoord+1, yCoord, characterGrid, 1), seqNum, 'w', *direction);
        characterGrid[xCoord+1][yCoord] = 15;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 6){
      if(xCoord < 19 && yCoord > 1 && (costOfTerrain(xCoord +1, yCoord-1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX) && (characterGrid[xCoord+1][yCoord-1] == characterGrid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord-1, nextTurn + costOfTerrain(xCoord +1, yCoord-1, characterGrid, 1), seqNum, 'w', *direction);
        characterGrid[xCoord+1][yCoord-1] = 15;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 7){
      if(yCoord > 1 && (costOfTerrain(xCoord, yCoord-1, characterGrid, 1) != INT_MAX)&& (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX) && (characterGrid[xCoord][yCoord-1] == characterGrid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord-1, nextTurn + costOfTerrain(xCoord, yCoord-1, characterGrid, 1), seqNum, 'w', *direction);
        characterGrid[xCoord][yCoord-1] = 15;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else if(*direction == 8){
      if(xCoord > 1 && yCoord > 1 && (costOfTerrain(xCoord -1, yCoord-1, characterGrid, 1) != INT_MAX) && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX) && (characterGrid[xCoord-1][yCoord-1] == characterGrid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord-1, nextTurn + costOfTerrain(xCoord -1, yCoord-1, characterGrid, 1), seqNum, 'w', *direction);
        characterGrid[xCoord-1][yCoord-1] = 15;
        change = 1;
        break;
      }
      else{
        *direction = (rand() % 8) + 1;
        count+=1;
        continue;
      }
    }
    else{
      // printf("Inside Here!!!\n");
      *direction = (rand() % 8) + 1;
      // printf("New Direction is %d\n", *direction);
      continue;
    }
  }
  if(count == 9){
    popCharacter(head);
    pushCharacter(head, xCoord, yCoord, nextTurn + costOfTerrain(xCoord, yCoord, characterGrid, 1), seqNum, 'w', *direction);
    characterGrid[xCoord][yCoord] = 15;
  }
}


void moveRival(int grid[21][80], int characterGrid[21][80], int distanceMap[21][80], Characters **head){
  //check all 8 neighbors to find the smallest value, the smallest value should not be 00
  //move to that space.
  //update next turn
  //pop the queue
  //push the queue
  Characters *peeked = peekCharacter(head);
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int distance = INT_MAX;
  int direction = peeked->direction;
  int minX = 0;
  int minY = 0;
  characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  //if character table is not taken by a character
  if(xCoord > 1 && (characterGrid[xCoord-1][yCoord] != 10) && (characterGrid[xCoord-1][yCoord] != 11) && (characterGrid[xCoord-1][yCoord] != 8) && (characterGrid[xCoord-1][yCoord] != 12) && (characterGrid[xCoord-1][yCoord] != 13) && (characterGrid[xCoord-1][yCoord] != 14) && (characterGrid[xCoord-1][yCoord] != 15)){
    if(distanceMap[xCoord -1][yCoord] != 0 && distanceMap[xCoord -1][yCoord] < distance){
      distance = distanceMap[xCoord -1][yCoord];
      minX = xCoord -1;
      minY = yCoord;
    }
  }

  if(xCoord > 1 && yCoord < 78 && (characterGrid[xCoord-1][yCoord+1] != 10) && (characterGrid[xCoord-1][yCoord+1] != 11) && (characterGrid[xCoord-1][yCoord+1] != 8) && (characterGrid[xCoord-1][yCoord+1] != 12) && (characterGrid[xCoord-1][yCoord+1] != 13) && (characterGrid[xCoord-1][yCoord+1] != 14) && (characterGrid[xCoord-1][yCoord+1] != 15)){
    if(distanceMap[xCoord -1][yCoord+1] != 0 && distanceMap[xCoord -1][yCoord+1] < distance){
      distance = distanceMap[xCoord -1][yCoord+1];
      minX = xCoord -1;
      minY = yCoord+1;
    }
  }

  if(yCoord < 78 && (characterGrid[xCoord][yCoord+1] != 10) && (characterGrid[xCoord][yCoord+1] != 11) && (characterGrid[xCoord][yCoord+1] != 8) && (characterGrid[xCoord][yCoord+1] != 12) && (characterGrid[xCoord][yCoord+1] != 13) && (characterGrid[xCoord][yCoord+1] != 14) && (characterGrid[xCoord][yCoord+1] != 15)){
    if(distanceMap[xCoord][yCoord+1] != 0 && distanceMap[xCoord][yCoord+1] < distance){
      distance = distanceMap[xCoord][yCoord+1];
      minX = xCoord;
      minY = yCoord+1;
    }
  }

  if(xCoord < 19 && yCoord < 78 && (characterGrid[xCoord+1][yCoord+1] != 10) && (characterGrid[xCoord+1][yCoord+1] != 11) && (characterGrid[xCoord+1][yCoord+1] != 8) && (characterGrid[xCoord+1][yCoord+1] != 12) && (characterGrid[xCoord+1][yCoord+1] != 13) && (characterGrid[xCoord+1][yCoord+1] != 14) && (characterGrid[xCoord+1][yCoord+1] != 15)){
    if(distanceMap[xCoord +1][yCoord+1] != 0 && distanceMap[xCoord +1][yCoord+1] < distance){
      distance = distanceMap[xCoord +1][yCoord+1];
      minX = xCoord +1;
      minY = yCoord+1;
    }
  }

  if(xCoord < 19 && (characterGrid[xCoord+1][yCoord] != 10) && (characterGrid[xCoord+1][yCoord] != 11) && (characterGrid[xCoord+1][yCoord] != 8) && (characterGrid[xCoord+1][yCoord] != 12) && (characterGrid[xCoord+1][yCoord] != 13) && (characterGrid[xCoord+1][yCoord] != 14) && (characterGrid[xCoord+1][yCoord] != 15)){
    if(distanceMap[xCoord +1][yCoord] != 0 && distanceMap[xCoord +1][yCoord] < distance){
      distance = distanceMap[xCoord +1][yCoord];
      minX = xCoord +1;
      minY = yCoord;
    }
  }

  if(xCoord < 19 && yCoord > 1 && (characterGrid[xCoord+1][yCoord-1] != 10) && (characterGrid[xCoord+1][yCoord-1] != 11) && (characterGrid[xCoord+1][yCoord-1] != 8) && (characterGrid[xCoord+1][yCoord-1] != 12) && (characterGrid[xCoord+1][yCoord-1] != 13) && (characterGrid[xCoord+1][yCoord-1] != 14) && (characterGrid[xCoord+1][yCoord-1] != 15)){
    if(distanceMap[xCoord +1][yCoord-1] != 0 && distanceMap[xCoord +1][yCoord-1] < distance){
      distance = distanceMap[xCoord +1][yCoord-1];
      minX = xCoord +1;
      minY = yCoord-1;
    }
  }

  if(yCoord > 1 && (characterGrid[xCoord][yCoord-1] != 10) && (characterGrid[xCoord][yCoord-1] != 11) && (characterGrid[xCoord][yCoord-1] != 8) && (characterGrid[xCoord][yCoord-1] != 12) && (characterGrid[xCoord][yCoord-1] != 13) && (characterGrid[xCoord][yCoord-1] != 14) && (characterGrid[xCoord][yCoord-1] != 15)){
    if(distanceMap[xCoord ][yCoord-1] != 0 && distanceMap[xCoord][yCoord-1] < distance){
      distance = distanceMap[xCoord][yCoord-1];
      minX = xCoord;
      minY = yCoord-1;
    }
  }

   if(xCoord > 1 && yCoord > 1 && (characterGrid[xCoord-1][yCoord-1] != 10) && (characterGrid[xCoord-1][yCoord-1] != 11) && (characterGrid[xCoord-1][yCoord-1] != 8) && (characterGrid[xCoord-1][yCoord-1] != 12) && (characterGrid[xCoord-1][yCoord-1] != 13) && (characterGrid[xCoord-1][yCoord-1] != 14) && (characterGrid[xCoord-1][yCoord-1] != 15)){
    if(distanceMap[xCoord -1][yCoord-1] != 0 && distanceMap[xCoord -1][yCoord-1] < distance){
      distance = distanceMap[xCoord -1][yCoord-1];
      minX = xCoord -1;
      minY = yCoord-1;
    }
  }
 int seqNum = peeked->seqNum;
 if(minX != 0 && minY != 0){
  int nextTurn = (peeked->nextTurn) + costOfTerrain(minX, minY, characterGrid, 1);
  popCharacter(head);
  pushCharacter(head, minX, minY, nextTurn, seqNum, 'r', direction);
  characterGrid[minX][minY] = 11;
 }
 else{
  int nextTurn = (peeked->nextTurn) + costOfTerrain(xCoord, yCoord, characterGrid, 1);
  popCharacter(head);
  pushCharacter(head, xCoord, yCoord, nextTurn, seqNum, 'r', direction);
  characterGrid[xCoord][yCoord] = 11;
 }

}

void moveHiker(int grid[21][80], int characterGrid[21][80], int distanceMap[21][80], Characters **head){
  //check all 8 neighbors to find the smallest value, the smallest value should not be 00
  //move to that space.
  //update next turn
  //pop the queue
  //push the queue
  Characters *peeked = peekCharacter(head);
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int distance = INT_MAX;
  int direction = peeked->direction;
  int minX = 0;
  int minY = 0;
  characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  //if character table is not taken by a character
  if(xCoord > 1 && (characterGrid[xCoord-1][yCoord] != 10) && (characterGrid[xCoord-1][yCoord] != 11) && (characterGrid[xCoord-1][yCoord] != 8) && (characterGrid[xCoord-1][yCoord] != 12) && (characterGrid[xCoord-1][yCoord] != 13) && (characterGrid[xCoord-1][yCoord] != 14) && (characterGrid[xCoord-1][yCoord] != 15)){
    if(distanceMap[xCoord -1][yCoord] != 0 && distanceMap[xCoord -1][yCoord] < distance){
      distance = distanceMap[xCoord -1][yCoord];
      minX = xCoord -1;
      minY = yCoord;
    }
  }

  if(xCoord > 1 && yCoord < 78 && (characterGrid[xCoord-1][yCoord+1] != 10) && (characterGrid[xCoord-1][yCoord+1] != 11) && (characterGrid[xCoord-1][yCoord+1] != 8) && (characterGrid[xCoord-1][yCoord+1] != 12) && (characterGrid[xCoord-1][yCoord+1] != 13) && (characterGrid[xCoord-1][yCoord+1] != 14) && (characterGrid[xCoord-1][yCoord+1] != 15)){
    if(distanceMap[xCoord -1][yCoord+1] != 0 && distanceMap[xCoord -1][yCoord+1] < distance){
      distance = distanceMap[xCoord -1][yCoord+1];
      minX = xCoord -1;
      minY = yCoord+1;
    }
  }

  if(yCoord < 78 && (characterGrid[xCoord][yCoord+1] != 10) && (characterGrid[xCoord][yCoord+1] != 11) && (characterGrid[xCoord][yCoord+1] != 8) && (characterGrid[xCoord][yCoord+1] != 12) && (characterGrid[xCoord][yCoord+1] != 13) && (characterGrid[xCoord][yCoord+1] != 14) && (characterGrid[xCoord][yCoord+1] != 15)){
    if(distanceMap[xCoord][yCoord+1] != 0 && distanceMap[xCoord][yCoord+1] < distance){
      distance = distanceMap[xCoord][yCoord+1];
      minX = xCoord;
      minY = yCoord+1;
    }
  }

  if(xCoord < 19 && yCoord < 78 && (characterGrid[xCoord+1][yCoord+1] != 10) && (characterGrid[xCoord+1][yCoord+1] != 11) && (characterGrid[xCoord+1][yCoord+1] != 8) && (characterGrid[xCoord+1][yCoord+1] != 12) && (characterGrid[xCoord+1][yCoord+1] != 13) && (characterGrid[xCoord+1][yCoord+1] != 14) && (characterGrid[xCoord+1][yCoord+1] != 15)){
    if(distanceMap[xCoord +1][yCoord+1] != 0 && distanceMap[xCoord +1][yCoord+1] < distance){
      distance = distanceMap[xCoord +1][yCoord+1];
      minX = xCoord +1;
      minY = yCoord+1;
    }
  }

  if(xCoord < 19 && (characterGrid[xCoord+1][yCoord] != 10) && (characterGrid[xCoord+1][yCoord] != 11) && (characterGrid[xCoord+1][yCoord] != 8) && (characterGrid[xCoord+1][yCoord] != 12) && (characterGrid[xCoord+1][yCoord] != 13) && (characterGrid[xCoord+1][yCoord] != 14) && (characterGrid[xCoord+1][yCoord] != 15)){
    if(distanceMap[xCoord +1][yCoord] != 0 && distanceMap[xCoord +1][yCoord] < distance){
      distance = distanceMap[xCoord +1][yCoord];
      minX = xCoord +1;
      minY = yCoord;
    }
  }

  if(xCoord < 19 && yCoord > 1 && (characterGrid[xCoord+1][yCoord-1] != 10) && (characterGrid[xCoord+1][yCoord-1] != 11) && (characterGrid[xCoord+1][yCoord-1] != 8) && (characterGrid[xCoord+1][yCoord-1] != 12) && (characterGrid[xCoord+1][yCoord-1] != 13) && (characterGrid[xCoord+1][yCoord-1] != 14) && (characterGrid[xCoord+1][yCoord-1] != 15)){
    if(distanceMap[xCoord +1][yCoord-1] != 0 && distanceMap[xCoord +1][yCoord-1] < distance){
      distance = distanceMap[xCoord +1][yCoord-1];
      minX = xCoord +1;
      minY = yCoord-1;
    }
  }

  if(yCoord > 1 && (characterGrid[xCoord][yCoord-1] != 10) && (characterGrid[xCoord][yCoord-1] != 11) && (characterGrid[xCoord][yCoord-1] != 8) && (characterGrid[xCoord][yCoord-1] != 12) && (characterGrid[xCoord][yCoord-1] != 13) && (characterGrid[xCoord][yCoord-1] != 14) && (characterGrid[xCoord][yCoord-1] != 15)){
    if(distanceMap[xCoord ][yCoord-1] != 0 && distanceMap[xCoord][yCoord-1] < distance){
      distance = distanceMap[xCoord][yCoord-1];
      minX = xCoord;
      minY = yCoord-1;
    }
  }

   if(xCoord > 1 && yCoord > 1 && (characterGrid[xCoord-1][yCoord-1] != 10) && (characterGrid[xCoord-1][yCoord-1] != 11) && (characterGrid[xCoord-1][yCoord-1] != 8) && (characterGrid[xCoord-1][yCoord-1] != 12) && (characterGrid[xCoord-1][yCoord-1] != 13) && (characterGrid[xCoord-1][yCoord-1] != 14) && (characterGrid[xCoord-1][yCoord-1] != 15)){
    if(distanceMap[xCoord -1][yCoord-1] != 0 && distanceMap[xCoord -1][yCoord-1] < distance){
      distance = distanceMap[xCoord -1][yCoord-1];
      minX = xCoord -1;
      minY = yCoord-1;
    }
  }
 int seqNum = peeked->seqNum;
 if(minX != 0 && minY != 0){
  int nextTurn = (peeked->nextTurn) + costOfTerrain(minX, minY, characterGrid, 0);
  popCharacter(head);
  pushCharacter(head, minX, minY, nextTurn, seqNum, 'h', direction);
  characterGrid[minX][minY] = 10;
 }else{
  int nextTurn = (peeked->nextTurn) + costOfTerrain(xCoord, yCoord, characterGrid, 0);
  popCharacter(head);
  pushCharacter(head, xCoord, yCoord, nextTurn, seqNum, 'h', direction);
  characterGrid[xCoord][yCoord] = 10;
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

void dijkstrasMethod(int grid[21][80], int hikerOrRider, int distance[21][80]){
  int visited[21][80] = {0};//all nodes not visited.
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
  // printDistTable(distance);
}
