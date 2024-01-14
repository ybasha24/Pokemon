#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<climits>
#include<unistd.h>
#include <ncurses.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include "worldGenerator.h"
#include "priorityQueue.h"
#include "characterQueue.h"
#include "db_parse.h"
#include "pokemon.h"

int probabilityOfPokeStuff(int x, int y);
void dijkstrasMethod(int grid[21][80], int hikerOrRider, int distance[21][80]);
void printDistTable(int distance[21][80]);
int costOfTerrainForHiker(int x, int y, int grid[21][80]);
void moveRival(int grid[21][80], Characters *characterTable[21][80], int distanceMap[21][80], Characters **head);
int costOfTerrain(int x, int y, int grid[21][80], int hikerOrRider);
void moveHiker(int grid[21][80], Characters *characterTable[21][80], int distanceMap[21][80], Characters **head);
void moveExplorer(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head);
void moveWander(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head);
void movePacer(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head);
void placeCharacter(int grid[21][80], Characters *characterTable[21][80], int characterNumber, int seqNum, int direction, Characters **head, int x, int y);
void initializeNewMap(map_t *world, int x, int y);
void battleTrainer(Characters *pc, Characters *npc);
void battleWildPokemon(Characters *pc, pokemon_generator *wildPokemon);
bool containsAlivePokemon(Characters *trainer);

int pcXcoord;
int pcYcoord;

int bagPokeballs =3;
int bagPotion = 2; 

void io_init_terminal()
    {
      initscr();
      raw();
      noecho();
      curs_set(0);
      keypad(stdscr, TRUE);
    }


int isSpotNPC(int spot){
  if(spot == 10 || spot == 11 || spot == 12 || spot == 13 || spot == 14 || spot == 15){
    return 1;
  }
  else{
    return 0;
  }
}

char typeOfNPC(int type){
     if(type == 10){
        return 'h';
      }
      else if(type == 11){
        return 'r';
      }
      else if(type == 12){
        return 'p';
      }
      else if(type == 13){
        return 's';
      }
      else if(type ==  14){
        return 'e';
      }
      else{
        return 'w';
      }
}

void printDisplay(Characters *characterTable[21][80], int grid[21][80]){
  int y = 0;
  for(int i = 0; i < 21; i++){
    for(int j =0; j< 80; j++){
      y = j;
      if(characterTable[i][j] != NULL){
        if(characterTable[i][j]->symbol ==8){
          mvaddch(i+1, j, '@');
        }
        else if(characterTable[i][j]->symbol == 10){
          mvaddch(i+1, j, 'h');
        }
        else if(characterTable[i][j]->symbol == 11){
          mvaddch(i+1, j, 'r');
        }
        else if(characterTable[i][j]->symbol == 12){
          mvaddch(i+1, j, 'p');
        }
        else if(characterTable[i][j]->symbol == 13){
          mvaddch(i+1, j, 's');
        }
        else if(characterTable[i][j]->symbol ==  14){
          mvaddch(i+1, j, 'e');
        }
        else if(characterTable[i][j]->symbol == 15){
          mvaddch(i+1, j, 'w');
        }
      }
      else{
        if(grid[i][j] == 1){
	        mvaddch(i+1, j, '%');
        }
        else if (grid[i][j]  == 2)
        {
          mvaddch(i+1, j, '#');
        }
        else if(grid[i][j] ==3){
          mvaddch(i+1, j, 'M');
        }
        else if(grid[i][j] ==4){
          mvaddch(i+1, j, 'C');
        }
        else if(grid[i][j] ==5){
          mvaddch(i+1, j, ':');
        }
        else if(grid[i][j] ==6){
          mvaddch(i+1, j, '.');
        }
        else if(grid[i][j] ==7){
          mvaddch(i+1, j, '^');
        }
        else if(grid[i][j] ==0){
          mvaddch(i+1, j, '^');
        }
      }
     }
    mvaddch(i+2, y, '\n');
  }
}

int main(int argc, char *argv[]){
    db_parse(false);

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
        w[x][y] = (map_t*) malloc((sizeof(map_t)));
        (w[x][y]->eExit) = (rand() % 17) + 2;
        (w[x][y]->wExit) = (rand() % 17) + 2;
        (w[x][y]->nExit) = (rand() % 76) + 2;
        (w[x][y]->sExit) = (rand() % 76) + 2;
        initMap(w[x][y],x,y,1,1);
    }
    // printGrid(w[200][200]->grid);
    printf("\n");
    // int characterMovements [21][80] = {0};
    Characters *characterTable[21][80];
    //place npc method where parameter is grid, character grid, queue, character number, direction 
    if(x == 200 && y == 200){
      // characterMovements[getPCX()][getPCY()] = 8;
      w[x][y]->grid[getPCX()][getPCY()] = 2;
    }
  
        

    w[x][y]->cPQ = newCharacter(getPCX(), getPCY(), 0, 0, '@', -1);


    



    if(argc == 1){
      char uInput = 'p';
      pcXcoord = getPCX();
      pcYcoord = getPCY();
      w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
      pokemon_generator *pk1 = new pokemon_generator(x, y);
      pokemon_generator *pk2 = new pokemon_generator(x, y);
      pokemon_generator *pk3 = new pokemon_generator(x, y);

      w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[0] = pk1;
      w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[1] = pk2;
      w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[2] = pk3;
      w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[3] = NULL;
      w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[4] = NULL;
      w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[5] = NULL;


      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 10, 1, -1, &(w[x][y]->cPQ),x, y);
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 11, 2, -1, &(w[x][y]->cPQ),x, y);
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 12, 3, -1, &(w[x][y]->cPQ),x, y);
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 13, 4, -1, &(w[x][y]->cPQ),x, y);
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 14, 5, -1, &(w[x][y]->cPQ),x, y);
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 14, 7, -1, &(w[x][y]->cPQ),x, y);
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 14, 8, -1, &(w[x][y]->cPQ),x, y);
      placeCharacter(w[x][y]->grid, w[x][y]->characterTable, 15, 6, -1, &(w[x][y]->cPQ),x, y);
      int distanceMapForHiker[21][80] ={0};
      int distanceMapForRider[21][80] ={0};
      dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
      dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
      io_init_terminal();
      // printf("It is %d\n", pcXcoord);//////////////////////////////////
      move(0,0);
      clrtoeol();
      refresh();
      mvprintw(0, 0,"Your Turn!");
      refresh();
      printDisplay(w[x][y]->characterTable, w[x][y]->grid);
      refresh();
      while(uInput != 'q'){
      //   move(0,0);
      // clrtoeol();
      // refresh();
      // mvprintw(0, 0,"x is %d and y is %d", x, y);
      // refresh();
        Characters *peeked = peekCharacter(&(w[x][y]->cPQ));
        // uInput = getch();
        if(peeked->symbolOfCharacter == '@'){
          // printGrid(characterMovements);
          // usleep(250000);
          // io_init_terminal();
          // mvprintw(0, 0,"First map");
          // refresh();
          mvprintw(0, 0,"Your Turn!");
          refresh();
          if(w[x][y]!= NULL){
            printDisplay(w[x][y]->characterTable, w[x][y]->grid);
            refresh();
          }
          uInput = getch();
          if(uInput == '5' || uInput == ' ' || uInput == '.'){
            move(0,0);
            clrtoeol();
            refresh();
            mvprintw(0, 0,"Your Turn!");
            refresh();
            int nextTurn = peeked->nextTurn;
            // move(0,0);
            // clrtoeol();
            // refresh();
            // mvprintw(0, 0,"next turn is %d", nextTurn);
            // refresh();
            popCharacter(&(w[x][y]->cPQ));
            pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
            // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
            // pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn, 0, '@', -1);
          }
          else if(uInput == 'k' || uInput == '8'){//going up
            if(pcXcoord > 1 && costOfTerrain((pcXcoord) -1,pcYcoord,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord-1][pcYcoord]==NULL){
                if(w[x][y]->grid[pcXcoord-1][pcYcoord] == 5){
                  int odds = rand() % 11;
                  if(odds == 1){
                    pokemon_generator *pokemonGenerated =  new pokemon_generator(x,y);
                    clear();
                    refresh();
                    std::string genderOfPokemon;
                    std::string shiny;
                    if(pokemonGenerated->isShiny == true){
                      shiny = "is SHINY";
                    }
                    else{
                      shiny = "is NOT shiny";
                    }
                    if(pokemonGenerated->isMale == true){
                      genderOfPokemon = "Male";
                    }
                    else{
                      genderOfPokemon = "Female";
                    }
                    mvprintw(0,0,"You have encountered %s!", pokemonGenerated->pokemonIdentifier);
                    mvprintw(1,0,"Battle will commence! Press 'c' to continue battle");
                    refresh();
                    uInput = 'v';
                    while(uInput != 'c'){
                      uInput = getch();
                    }
                    battleWildPokemon(w[x][y]->characterTable[pcXcoord][pcYcoord], pokemonGenerated);
                    clear();
                    refresh();
                    if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                      endwin();
                      printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                      return 0;
                    }
                  }
                }
                
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord -1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
              // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
              // characterMovements[pcXcoord][pcYcoord] = 8;
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord-1][pcYcoord]->battled==0){
                w[x][y]->characterTable[pcXcoord-1][pcYcoord]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");///////////////////////////////////////////////////////////////////////////////////////////

                battleTrainer(w[x][y]->characterTable[pcXcoord][pcYcoord], w[x][y]->characterTable[pcXcoord-1][pcYcoord]);
                clear();
                refresh();
                if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                  endwin();
                  printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                  return 0;
                }
                continue;
              }
              else{
                move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }
            }
            else if(w[x][y]->grid[pcXcoord-1][pcYcoord] == 2){//moving to the north map
              pokemon_generator *storeing[6];
              for(int i = 0; i < 6; i ++){
                storeing[i] = w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i];
              }
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              y = y -1;
              if(w[x][y]==NULL){
                  w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
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
                  pcXcoord = 19;
                  initializeNewMap(w[x][y],x, y);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  for(int i = 0; i < 6; i ++){
                    w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i] = storeing[i];
                  }
                  continue;
                  
              }
              else{
                  pcXcoord = 19;
                  // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                  w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                  // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
                
              }


            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'y' || uInput == '7'){//going up and left
            if(pcXcoord > 1 && pcYcoord > 1 && costOfTerrain((pcXcoord) -1,(pcYcoord) - 1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord-1][pcYcoord-1] == NULL){
                if(w[x][y]->grid[pcXcoord-1][pcYcoord-1] == 5){
                  int odds = rand() % 11;
                  if(odds == 1){
                    pokemon_generator *pokemonGenerated =  new pokemon_generator(x,y);
                    clear();
                    refresh();
                    std::string genderOfPokemon;
                    std::string shiny;
                    if(pokemonGenerated->isShiny == true){
                      shiny = "is SHINY";
                    }
                    else{
                      shiny = "is NOT shiny";
                    }
                    if(pokemonGenerated->isMale == true){
                      genderOfPokemon = "Male";
                    }
                    else{
                      genderOfPokemon = "Female";
                    }
                    mvprintw(0,0,"You have encountered %s!", pokemonGenerated->pokemonIdentifier);
                    mvprintw(1,0,"Battle will commence! Press 'c' to continue battle");
                    refresh();
                    uInput = 'v';
                    while(uInput != 'c'){
                      uInput = getch();
                    }
                    battleWildPokemon(w[x][y]->characterTable[pcXcoord][pcYcoord], pokemonGenerated);
                    clear();
                    refresh();
                    if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                      endwin();
                      printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                      return 0;
                    }
                  }
                }
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord -1;
              pcYcoord = pcYcoord -1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord-1][pcYcoord-1]->battled==0){
                w[x][y]->characterTable[pcXcoord-1][pcYcoord-1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");

                battleTrainer(w[x][y]->characterTable[pcXcoord][pcYcoord], w[x][y]->characterTable[pcXcoord-1][pcYcoord-1]);
                clear();
                refresh();
                if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                  endwin();
                  printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                  return 0;
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'u' || uInput == '9'){//going up and right
            if(pcXcoord > 1 && pcYcoord < 78 && costOfTerrain((pcXcoord) -1,(pcYcoord) + 1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord-1][pcYcoord+1]== NULL){
                if(w[x][y]->grid[pcXcoord-1][pcYcoord+1] == 5){
                  int odds = rand() % 11;
                  if(odds == 1){
                    pokemon_generator *pokemonGenerated =  new pokemon_generator(x,y);
                    clear();
                    refresh();
                    std::string genderOfPokemon;
                    std::string shiny;
                    if(pokemonGenerated->isShiny == true){
                      shiny = "is SHINY";
                    }
                    else{
                      shiny = "is NOT shiny";
                    }
                    if(pokemonGenerated->isMale == true){
                      genderOfPokemon = "Male";
                    }
                    else{
                      genderOfPokemon = "Female";
                    }
                    mvprintw(0,0,"You have encountered %s!", pokemonGenerated->pokemonIdentifier);
                    mvprintw(1,0,"Battle will commence! Press 'c' to continue battle");
                    refresh();
                    uInput = 'v';
                    while(uInput != 'c'){
                      uInput = getch();
                    }
                    battleWildPokemon(w[x][y]->characterTable[pcXcoord][pcYcoord], pokemonGenerated);
                    clear();
                    refresh();
                    if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                      endwin();
                      printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                      return 0;
                    }
                  }
                }
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord]= NULL;
              pcXcoord = pcXcoord -1;
              pcYcoord = pcYcoord +1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord, 8, 0);
              // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord-1][pcYcoord+1]->battled==0){
                w[x][y]->characterTable[pcXcoord-1][pcYcoord+1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");

                battleTrainer(w[x][y]->characterTable[pcXcoord][pcYcoord], w[x][y]->characterTable[pcXcoord-1][pcYcoord+1]);
                clear();
                refresh();
                if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                  endwin();
                  printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                  return 0;
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'n' || uInput == '3'){//going down and right
            if(pcXcoord < 20 && pcYcoord < 78 && costOfTerrain((pcXcoord) +1,(pcYcoord) + 1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord+1][pcYcoord+1]==NULL){
                if(w[x][y]->grid[pcXcoord+1][pcYcoord+1] == 5){
                  int odds = rand() % 11;
                  if(odds == 1){
                    pokemon_generator *pokemonGenerated =  new pokemon_generator(x,y);
                    clear();
                    refresh();
                    std::string genderOfPokemon;
                    std::string shiny;
                    if(pokemonGenerated->isShiny == true){
                      shiny = "is SHINY";
                    }
                    else{
                      shiny = "is NOT shiny";
                    }
                    if(pokemonGenerated->isMale == true){
                      genderOfPokemon = "Male";
                    }
                    else{
                      genderOfPokemon = "Female";
                    }
                    mvprintw(0,0,"You have encountered %s!", pokemonGenerated->pokemonIdentifier);
                    mvprintw(1,0,"Battle will commence! Press 'c' to continue battle");
                    refresh();
                    uInput = 'v';
                    while(uInput != 'c'){
                      uInput = getch();
                    }
                    battleWildPokemon(w[x][y]->characterTable[pcXcoord][pcYcoord], pokemonGenerated);
                    clear();
                    refresh();
                    if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                      endwin();
                      printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                      return 0;
                    }
                  }
                }
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord +1;
              pcYcoord = pcYcoord +1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord+1][pcYcoord+1]->battled==0){
                w[x][y]->characterTable[pcXcoord+1][pcYcoord+1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");

                battleTrainer(w[x][y]->characterTable[pcXcoord][pcYcoord], w[x][y]->characterTable[pcXcoord+1][pcYcoord+1]);
                clear();
                refresh();
                if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                  endwin();
                  printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                  return 0;
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'b' || uInput == '1'){//going down and left
            if(pcXcoord < 20 && pcYcoord > 1 && costOfTerrain((pcXcoord) +1,(pcYcoord) - 1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord+1][pcYcoord-1]==NULL){
                if(w[x][y]->grid[pcXcoord+1][pcYcoord-1] == 5){
                  int odds = rand() % 11;
                  if(odds == 1){
                    pokemon_generator *pokemonGenerated =  new pokemon_generator(x,y);
                    clear();
                    refresh();
                    std::string genderOfPokemon;
                    std::string shiny;
                    if(pokemonGenerated->isShiny == true){
                      shiny = "is SHINY";
                    }
                    else{
                      shiny = "is NOT shiny";
                    }
                    if(pokemonGenerated->isMale == true){
                      genderOfPokemon = "Male";
                    }
                    else{
                      genderOfPokemon = "Female";
                    }
                    mvprintw(0,0,"You have encountered %s!", pokemonGenerated->pokemonIdentifier);
                    mvprintw(1,0,"Battle will commence! Press 'c' to continue battle");
                    refresh();
                    uInput = 'v';
                    while(uInput != 'c'){
                      uInput = getch();
                    }
                    battleWildPokemon(w[x][y]->characterTable[pcXcoord][pcYcoord], pokemonGenerated);
                    clear();
                    refresh();
                    if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                      endwin();
                      printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                      return 0;
                    }
                  }
                }
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord +1;
              pcYcoord = pcYcoord -1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8, 0);
              // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord+1][pcYcoord-1]->battled==0){
                w[x][y]->characterTable[pcXcoord+1][pcYcoord-1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");

                battleTrainer(w[x][y]->characterTable[pcXcoord][pcYcoord], w[x][y]->characterTable[pcXcoord+1][pcYcoord-1]);
                clear();
                refresh();
                if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                  endwin();
                  printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                  return 0;
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 'j' || uInput == '2'){//going down
            if(pcXcoord < 19 && costOfTerrain((pcXcoord) +1,pcYcoord,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord+1][pcYcoord]==NULL){
                if(w[x][y]->grid[pcXcoord+1][pcYcoord] == 5){
                  int odds = rand() % 11;
                  if(odds == 1){
                    pokemon_generator *pokemonGenerated =  new pokemon_generator(x,y);
                    clear();
                    refresh();
                    std::string genderOfPokemon;
                    std::string shiny;
                    if(pokemonGenerated->isShiny == true){
                      shiny = "is SHINY";
                    }
                    else{
                      shiny = "is NOT shiny";
                    }
                    if(pokemonGenerated->isMale == true){
                      genderOfPokemon = "Male";
                    }
                    else{
                      genderOfPokemon = "Female";
                    }
                    mvprintw(0,0,"You have encountered %s!", pokemonGenerated->pokemonIdentifier);
                    mvprintw(1,0,"Battle will commence! Press 'c' to continue battle");
                    refresh();
                    uInput = 'v';
                    while(uInput != 'c'){
                      uInput = getch();
                    }
                    battleWildPokemon(w[x][y]->characterTable[pcXcoord][pcYcoord], pokemonGenerated);
                    clear();
                    refresh();
                    if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                      endwin();
                      printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                      return 0;
                    }
                  }
                }
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord +1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord+1][pcYcoord]->battled==0){
                w[x][y]->characterTable[pcXcoord+1][pcYcoord]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");

                battleTrainer(w[x][y]->characterTable[pcXcoord][pcYcoord], w[x][y]->characterTable[pcXcoord+1][pcYcoord]);
                clear();
                refresh();
                if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                  endwin();
                  printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                  return 0;
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else if(w[x][y]->grid[pcXcoord+1][pcYcoord] == 2){//south map
              pokemon_generator *storeing[6];
              for(int i = 0; i < 6; i ++){
                storeing[i] = w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i];
              }
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              y = y +1;
              if(w[x][y]==NULL){
                  w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
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
                  pcXcoord = 1;
                  initializeNewMap(w[x][y],x, y);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  for(int i = 0; i < 6; i ++){
                    w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i] = storeing[i];
                  }
                  continue;
              }
              else{
                  pcXcoord = 1;
                  // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                  w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                  // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
              }
            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == '4' || uInput == 'h'){//going left
            if(pcYcoord > 1 && costOfTerrain((pcXcoord),(pcYcoord) -1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord][pcYcoord-1]==NULL){
                if(w[x][y]->grid[pcXcoord][pcYcoord-1] == 5){
                  int odds = rand() % 11;
                  if(odds == 1){
                    pokemon_generator *pokemonGenerated =  new pokemon_generator(x,y);
                    clear();
                    refresh();
                    std::string genderOfPokemon;
                    std::string shiny;
                    if(pokemonGenerated->isShiny == true){
                      shiny = "is SHINY";
                    }
                    else{
                      shiny = "is NOT shiny";
                    }
                    if(pokemonGenerated->isMale == true){
                      genderOfPokemon = "Male";
                    }
                    else{
                      genderOfPokemon = "Female";
                    }
                    mvprintw(0,0,"You have encountered %s!", pokemonGenerated->pokemonIdentifier);
                    mvprintw(1,0,"Battle will commence! Press 'c' to continue battle");
                    refresh();
                    uInput = 'v';
                    while(uInput != 'c'){
                      uInput = getch();
                    }
                    battleWildPokemon(w[x][y]->characterTable[pcXcoord][pcYcoord], pokemonGenerated);
                    clear();
                    refresh();
                    if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                      endwin();
                      printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                      return 0;
                    }
                  }
                }
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcYcoord = pcYcoord -1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord,8, 0);
              // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord][pcYcoord-1]->battled==0){
                w[x][y]->characterTable[pcXcoord][pcYcoord-1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");

                battleTrainer(w[x][y]->characterTable[pcXcoord][pcYcoord], w[x][y]->characterTable[pcXcoord][pcYcoord-1]);
                clear();
                refresh();
                if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                  endwin();
                  printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                  return 0;
                }
                continue;
              }
              else{
                move(0,0);
                clrtoeol();
                refresh();
                mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
                refresh();
                continue;
              }

            }
            else if(w[x][y]->grid[pcXcoord][pcYcoord-1] == 2){//go to west map
              pokemon_generator *storeing[6];
              for(int i = 0; i < 6; i ++){
                storeing[i] = w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i];
              }
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              x = x-1;
              if(w[x][y]==NULL){
                  w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
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
                  pcYcoord = 78;
                  initializeNewMap(w[x][y],x, y);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  for(int i = 0; i < 6; i ++){
                    w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i] = storeing[i];
                  }
                  continue;
              }
              else{
                  pcYcoord = 78;
                  // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                  w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                  // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == '6' || uInput == 'l'){//going right
            if(pcYcoord < 78 && costOfTerrain((pcXcoord),(pcYcoord) +1,w[x][y]->grid,3) != INT_MAX){
              if(w[x][y]->characterTable[pcXcoord][pcYcoord+1] == NULL){
                if(w[x][y]->grid[pcXcoord][pcYcoord+1] == 5){
                  int odds = rand() % 11;
                  if(odds == 1){
                    pokemon_generator *pokemonGenerated =  new pokemon_generator(x,y);
                    clear();
                    refresh();
                    std::string genderOfPokemon;
                    std::string shiny;
                    if(pokemonGenerated->isShiny == true){
                      shiny = "is SHINY";
                    }
                    else{
                      shiny = "is NOT shiny";
                    }
                    if(pokemonGenerated->isMale == true){
                      genderOfPokemon = "Male";
                    }
                    else{
                      genderOfPokemon = "Female";
                    }
                    mvprintw(0,0,"You have encountered %s!", pokemonGenerated->pokemonIdentifier);
                    mvprintw(1,0,"Battle will commence! Press 'c' to continue battle");
                    refresh();
                    uInput = 'v';
                    while(uInput != 'c'){
                      uInput = getch();
                    }
                    battleWildPokemon(w[x][y]->characterTable[pcXcoord][pcYcoord], pokemonGenerated);
                    clear();
                    refresh();
                    if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                      endwin();
                      printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                      return 0;
                    }
                  }
                }
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              pcYcoord = pcYcoord +1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&(w[x][y]->cPQ));
              pushCharacter(&(w[x][y]->cPQ), pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[x][y]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
              dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
              continue;
              }
              else if(w[x][y]->characterTable[pcXcoord][pcYcoord+1]->battled==0){
                w[x][y]->characterTable[pcXcoord][pcYcoord+1]->battled=1;
                clear();
                refresh();
                mvprintw(0,0,"You are in a battle!");

                battleTrainer(w[x][y]->characterTable[pcXcoord][pcYcoord], w[x][y]->characterTable[pcXcoord][pcYcoord+1]);
                clear();
                refresh();
                if(containsAlivePokemon(w[x][y]->characterTable[pcXcoord][pcYcoord]) == false){
                  endwin();
                  printf("ALL YOUR POKEMON ARE DEAD, YOU LOSE! BETTER LUCK NEXT TIME!\n");
                  return 0;
                }
                continue;
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              continue;
              }

            }
            else if(w[x][y]->grid[pcXcoord][pcYcoord+1] == 2){//go to east map
              pokemon_generator *storeing[6];
              for(int i = 0; i < 6; i ++){
                storeing[i] = w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i];
              }
              free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
              w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
              x = x+1;
              if(w[x][y]==NULL){
                  w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
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
                  pcYcoord = 1;
                  initializeNewMap(w[x][y],x, y);
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  for(int i = 0; i < 6; i ++){
                    w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i] = storeing[i];
                  }
                  continue;
              }
              else{
                  pcYcoord = 1;
                  // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                  w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                  // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
                  for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  continue;
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
              continue;
            }
          }
          else if(uInput == 't'){
            clear();
            refresh();
            mvprintw(0,0,"List of Trainers near you: \n");
            int r = 1;
            for(int i = 0; i < 21; i++){
              for(int j =0; j< 80; j++){
                if(w[x][y]->characterTable[i][j] != NULL && w[x][y]->characterTable[i][j]->symbol != 8){//should print this on the trainer list
                  if(pcXcoord - i >= 0 && pcYcoord - j >= 0){
                    mvprintw(r,0,"%c, %d north %d west", typeOfNPC(w[x][y]->characterTable[i][j]->symbol), pcXcoord - i, pcYcoord - j);
                    refresh();
                    r++;
                  }
                  else if(pcXcoord - i <= 0 && pcYcoord - j >= 0){
                    mvprintw(r,0,"%c, %d south %d west", typeOfNPC(w[x][y]->characterTable[i][j]->symbol), abs(pcXcoord - i), pcYcoord - j);
                    refresh();
                    r++;
                  }
                  else if(pcXcoord - i <= 0 && pcYcoord - j <= 0){
                    mvprintw(r,0, "%c, %d south %d east", typeOfNPC(w[x][y]->characterTable[i][j]->symbol), abs(pcXcoord - i), abs(pcYcoord - j));
                    refresh();
                    r++;
                  }
                  else if(pcXcoord - i >= 0 && pcYcoord - j <= 0){
                    mvprintw(r,0,"%c, %d north %d east", typeOfNPC(w[x][y]->characterTable[i][j]->symbol), pcXcoord - i, abs(pcYcoord - j));
                    refresh();
                    r++;
                  }
                }
              }
            }
            
            while(uInput != 27){
              uInput = getch();
              if(uInput == 27){
               break;
              }
            }
            if(uInput == 27){
               clear();
            }
            continue;
            
          }
          else if(uInput == '>'){
            if(w[x][y]->grid[pcXcoord][pcYcoord] == 3){//Pokemart
              clear();
              refresh();
              mvprintw(0,0,"In a pokeMart!");
              mvprintw(1,0,"Replenishing your bag!");
              bagPokeballs = 3;
              bagPotion = 2;
              while(uInput != '<'){
                uInput = getch();
                if(uInput == '<'){
                  break;
                }
              }
              if(uInput == '<'){
                clear();
              }
              continue;
            }
            else if(w[x][y]->grid[pcXcoord][pcYcoord] == 4){//Pokecenter
              clear();
              refresh();
              mvprintw(0,0,"In a pokeCenter!");
              mvprintw(1,0,"Reviving all Pokemons to full HP!");
              for(int i = 0; i<6; i++){
                if(w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i] != NULL){
                  w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i]->currentHp = w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i]->hp;
                }
              }
              while(uInput != '<'){
                uInput = getch();
                if(uInput == '<'){
                  break;
                }
              }
              if(uInput == '<'){
                clear();
              }
              continue;
            }

          }
          else if(uInput == 'f'){
            // scanf("%d %d", &x, &y);
            pokemon_generator *storeing[6];
            for(int i = 0; i < 6; i ++){
              storeing[i] = w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i];
            }
            free(w[x][y]->characterTable[pcXcoord][pcYcoord]);
            w[x][y]->characterTable[pcXcoord][pcYcoord] = NULL;
            // clear();
            // refresh();
            while(uInput == 'f'){
              clear();
              refresh();
            move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Type 2 ints with a space between each and hit enter after!");
              refresh();
            mvscanw(0,0,"%d %d", &x, &y);
            if((x < -200) || (x > 200) || (y < -200) || (y > 200)){
                move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Can't go there, try again!");
              refresh();
                continue;
            }
            // printf("flying to (%d,%d)\n", x, y);
            x = x + 200;
            y = y + 200;
            
            if(w[x][y] == NULL){
                w[x][y] = (map_t*) malloc((sizeof(map_t)));
                  for(int i = 0; i<21; i++){
                    for(int j=0;j<80;j++){
                      w[x][y]->grid[i][j] = 0;
                    }
                  }
                  w[x][y]->nExit = 0;
                  w[x][y]->eExit = 0;
                  w[x][y]->wExit = 0;
                  w[x][y]->sExit = 0;
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
                pcXcoord = getPCX();
                pcYcoord = getPCY();
                initializeNewMap(w[x][y],x, y);
                for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                  for(int i = 0; i < 6; i ++){
                    w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i] = storeing[i];
                  }
                break;
            }
            else{
                // pcYcoord = 1;
                // w[x][y]->cPQ = newCharacter(pcXcoord, pcYcoord, 0, 0, '@', -1);
                while(w[x][y]->characterTable[pcXcoord][pcYcoord] != NULL){
                  if(w[x][y]->characterTable[pcXcoord-1][pcYcoord] == NULL){
                    pcXcoord = pcXcoord -1;
                    break;
                  }
                  else if(w[x][y]->characterTable[pcXcoord+1][pcYcoord] == NULL){
                    pcXcoord = pcXcoord +1;
                    break;
                  }
                  else if(w[x][y]->characterTable[pcXcoord][pcYcoord-1] == NULL){
                    pcYcoord = pcYcoord -1;
                    break;
                  }
                  else if(w[x][y]->characterTable[pcXcoord][pcYcoord+1] == NULL){
                    pcYcoord = pcYcoord +1;
                    break;
                  }
                }
                w[x][y]->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
                // w[x][y]->characterTable[pcXcoord][pcYcoord]->trainerPokemons = pcPokemons;
                for(int i = 0; i< 21; i++){
                    for(int j = 0; j<80; j++){
                      distanceMapForHiker[i][j]=0;
                      distanceMapForRider[i][j]=0;
                    }
                  }
                  dijkstrasMethod(w[x][y]->grid, 0, distanceMapForHiker);
                  dijkstrasMethod(w[x][y]->grid, 1, distanceMapForRider);
                break;
            }
            continue;

          }


          }
          else if(uInput == 'B'){
            clear();
            refresh();
            mvprintw(0,0,"In your bag you have %d pokeballs and %d potions", bagPokeballs, bagPotion);
            mvprintw(1,0,"To replenish your bag visit a pokemart!");
            mvprintw(2,0,"Press 'esc' to continue journey!");
            refresh();
            while(uInput != 27){
              uInput = getch();
            }
            clear();
            refresh();
            continue;
          }
          else if(uInput == 'L'){
            clear();
            refresh();
            mvprintw(0,0,"Printing out your pokemons and their respective hp's");
            int j = 1;
            for(int i=0; i < 6; i++){
              if(w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i] != NULL){
                mvprintw(j,0, "Pokemon %s with %d hp ", w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i]->pokemonIdentifier,
                w[x][y]->characterTable[pcXcoord][pcYcoord]->pokemonList[i]->currentHp);
                j++;
              }
            }

            mvprintw(j,0,"Press 'esc' to get back into the game!");
            refresh();
            while(uInput != 27){
              uInput = getch();
            }
            clear();
            refresh();
            continue;

          }
          else if(uInput == 'q'){
            continue;
          }
          else{
            uInput = getch();
          }
        }
        if(peeked->symbolOfCharacter == 'r'){
          moveRival(w[x][y]->grid, w[x][y]->characterTable, distanceMapForRider, &(w[x][y]->cPQ));
          continue;
        }
        if(peeked->symbolOfCharacter == 'h'){
          moveHiker(w[x][y]->grid, w[x][y]->characterTable, distanceMapForHiker, &(w[x][y]->cPQ));
          continue;
        }
        if(peeked->symbolOfCharacter == 'p'){
          int direction = peeked->direction;
          movePacer(w[x][y]->grid, w[x][y]->characterTable, &direction, &(w[x][y]->cPQ));
          continue;
        }

        if(peeked->symbolOfCharacter == 's'){
          int xCoord = peeked->xCoord;
          int yCoord = peeked->yCoord;
          int seqNum = peeked->seqNum;
          int nextTurn = peeked->nextTurn + costOfTerrain(xCoord, yCoord, w[x][y]->grid, 1);
          popCharacter(&(w[x][y]->cPQ));
          pushCharacter(&(w[x][y]->cPQ), xCoord, yCoord, nextTurn, seqNum, 's', -1);
          continue;
        }

        if(peeked->symbolOfCharacter == 'e'){
          int direction = peeked->direction;
          moveExplorer(w[x][y]->grid, w[x][y]->characterTable, &direction, &(w[x][y]->cPQ));
          continue;
        }
        if(peeked->symbolOfCharacter == 'w'){
          int direction = peeked->direction;
          moveWander(w[x][y]->grid, w[x][y]->characterTable, &direction, &(w[x][y]->cPQ));
          continue;
        }

        // uInput = getch();
        if(uInput == 'q'){
          break;
        }
      }
       endwin();
    }
    else{//user input is there!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////dont need this
      // int numOfTrainers = atoi(argv[1]);
      // pcXcoord = getPCX();
      // pcYcoord = getPCY();
      // characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
      // int distanceMapForHiker[21][80] ={0};
      // int distanceMapForRider[21][80] ={0};
      // dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
      // dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
      // char uInput = 'p';
      // io_init_terminal();

      // if(numOfTrainers == 1){//just hiker!
      //   placeCharacter(w[200][200]->grid, characterTable, 14, 1, -1, &cPQ);
      //   // printf("It is %d\n", pcXcoord);//////////////////////////////////
      //   mvprintw(0, 0,"Your Turn!");
      //   refresh();
      //   printDisplay(characterTable, w[200][200]->grid);
      //   refresh();
      //   while(uInput != 'q'){
      //     Characters *peeked = peekCharacter(&cPQ);
      //     if(peeked->symbolOfCharacter == '@'){
      //       printDisplay(characterTable, w[200][200]->grid);
      //       refresh();
      //       uInput = getch();
      //       if(uInput == 'a'){
      //         mvprintw(0, 0,"Your Turn!");
      //         refresh();
      //         int nextTurn = peeked->nextTurn;
      //         popCharacter(&cPQ);
      //         pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
      //       }
      //     }
      //     if(peeked->symbolOfCharacter == 'r'){
      //     moveRival(w[200][200]->grid, characterTable, distanceMapForRider, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'h'){
      //       moveHiker(w[200][200]->grid, characterTable, distanceMapForHiker, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'p'){
      //       int direction = peeked->direction;
      //       movePacer(w[200][200]->grid, characterTable, &direction, &cPQ);
      //     }

      //     if(peeked->symbolOfCharacter == 's'){
      //       int xCoord = peeked->xCoord;
      //       int yCoord = peeked->yCoord;
      //       int seqNum = peeked->seqNum;
      //       int nextTurn = peeked->nextTurn + costOfTerrain(xCoord, yCoord, w[200][200]->grid, 1);
      //       popCharacter(&cPQ);
      //       pushCharacter(&cPQ, xCoord, yCoord, nextTurn, seqNum, 's', -1);
      //     }

      //     if(peeked->symbolOfCharacter == 'e'){
      //       int direction = peeked->direction;
      //       moveExplorer(w[200][200]->grid, characterTable, &direction, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'w'){
      //       int direction = peeked->direction;
      //       moveWander(w[200][200]->grid, characterTable, &direction, &cPQ);
      //     }
      //   }
      //   endwin();
      // }
      // else if(numOfTrainers == 2){
      //   placeCharacter(w[200][200]->grid, characterMovements, 10, 1, -1, &cPQ);
      //   placeCharacter(w[200][200]->grid, characterMovements, 11, 2, -1, &cPQ);
      //   while(1){
      //     Characters *peeked = peekCharacter(&cPQ);
      //     if(peeked->symbolOfCharacter == '@'){
      //       printGrid(characterMovements);
      //       usleep(250000);
      //       int nextTurn = peeked->nextTurn;
      //       popCharacter(&cPQ);
      //       pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + 10, 0, '@', -1);
      //     }
      //     if(peeked->symbolOfCharacter == 'r'){
      //       moveRival(w[200][200]->grid, characterMovements, distanceMapForRider, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'h'){
      //       moveHiker(w[200][200]->grid, characterMovements, distanceMapForHiker, &cPQ);
      //     }
      //   }
      // }
      // else{//numtrainers is more than 2
      //   placeCharacter(w[200][200]->grid, characterMovements, 10, 1, -1, &cPQ);
      //   placeCharacter(w[200][200]->grid, characterMovements, 11, 2, -1, &cPQ);
      //   int seqNum = 3;
      //   int counter = 2;
      //   while(counter != numOfTrainers){
      //     placeCharacter(w[200][200]->grid, characterMovements, 12, seqNum, -1, &cPQ);
      //     seqNum += 1;
      //     counter +=1;
      //     if(counter == numOfTrainers){
      //       break;
      //     }
      //     placeCharacter(w[200][200]->grid, characterMovements, 13, seqNum, -1, &cPQ);
      //     seqNum += 1;
      //     counter +=1;
      //     if(counter == numOfTrainers){
      //       break;
      //     }
      //     placeCharacter(w[200][200]->grid, characterMovements, 14, seqNum, -1, &cPQ);
      //     seqNum += 1;
      //     counter +=1;
      //     if(counter == numOfTrainers){
      //       break;
      //     }
      //     placeCharacter(w[200][200]->grid, characterMovements, 15, seqNum, -1, &cPQ);
      //     seqNum += 1;
      //     counter +=1;
      //     if(counter == numOfTrainers){
      //       break;
      //     }
      //   }

      //   while(1){
      //     Characters *peeked = peekCharacter(&cPQ);
      //     if(peeked->symbolOfCharacter == '@'){
      //       printGrid(characterMovements);
      //       usleep(250000);
      //       int nextTurn = peeked->nextTurn;
      //       popCharacter(&cPQ);
      //       pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + 10, 0, '@', -1);
      //     }
      //     if(peeked->symbolOfCharacter == 'r'){
      //       moveRival(w[200][200]->grid, characterMovements, distanceMapForRider, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'h'){
      //       moveHiker(w[200][200]->grid, characterMovements, distanceMapForHiker, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'p'){
      //       int direction = peeked->direction;
      //       movePacer(w[200][200]->grid, characterMovements, &direction, &cPQ);
      //     }

      //     if(peeked->symbolOfCharacter == 's'){
      //       int xCoord = peeked->xCoord;
      //       int yCoord = peeked->yCoord;
      //       int seqNum = peeked->seqNum;
      //       int nextTurn = peeked->nextTurn + costOfTerrain(xCoord, yCoord, w[200][200]->grid, 1);
      //       popCharacter(&cPQ);
      //       pushCharacter(&cPQ, xCoord, yCoord, nextTurn, seqNum, 's', -1);
      //     }

      //     if(peeked->symbolOfCharacter == 'e'){
      //       int direction = peeked->direction;
      //       moveExplorer(w[200][200]->grid, characterMovements, &direction, &cPQ);
      //     }
      //     if(peeked->symbolOfCharacter == 'w'){
      //       int direction = peeked->direction;
      //       moveWander(w[200][200]->grid, characterMovements, &direction, &cPQ);
      //     }
      //   }
      // }

    }

}

bool containsAlivePokemon(Characters *trainer){//seg fault!!!!
  for(int i = 0; i < 6; i++){
    if(trainer->pokemonList[i] != NULL){
      if(trainer->pokemonList[i]->currentHp > 0){
        return true;
      }
      else{
        continue;
      }
    }
    else if(trainer->pokemonList[i] == NULL){
      continue;
    }
  }
  return false;
}

int indexOfFirstAlivePokemon(Characters *trainer){
  for(int i = 0; i < 6; i++){
    if(trainer->pokemonList[i] != NULL){
      if(trainer->pokemonList[i]->currentHp > 0){
        return i;
      }
    }
  }
  return 7;// 7 means they all dead
}

int indexOfNumberOfPokemons(Characters *pc){
  for(int i = 0; i < 6; i++){
    if(pc->pokemonList[i] == NULL){
      return i;
    }
  }
  return 6;
}

void fightPk(Characters *pc, pokemon_generator *wildPokemon, int pokemonChosenForPC){
    char uInput2 = 'v';
    pokemon_move_db npcMove = wildPokemon->move1;
    pokemon_move_db pcMove = pc->pokemonList[pokemonChosenForPC]->move2;
    clear();
    refresh();
    mvprintw(0,0,"Battle between your pokemon %s and wild pokemon %s", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier, wildPokemon->pokemonIdentifier);
    mvprintw(1,0,"Pick your move:");
    mvprintw(2,0,"Move 1: %s, Move 2: %s", moves[pc->pokemonList[pokemonChosenForPC]->move1.move_id].identifier, moves[pc->pokemonList[pokemonChosenForPC]->move2.move_id].identifier);
    mvprintw(3,0,"Type '1' or '2' to select a move for fighting!");
    refresh();

    while(uInput2 != '1' && uInput2 != '2'){
      uInput2 = getch();
    }

    if(uInput2 == '1'){
      pcMove = pc->pokemonList[pokemonChosenForPC]->move1;
    }
    else{
      pcMove = pc->pokemonList[pokemonChosenForPC]->move2;
    }

    //who goes first
    if(moves[pcMove.move_id].priority >= moves[npcMove.move_id].priority){//pc goes first!
      if(rand() % 100 < moves[pcMove.move_id].accuracy){//move hits!!
        int damage = ((((((2 * pc->pokemonList[pokemonChosenForPC]->level) / 5) +2) * moves[pcMove.move_id].power * (pc->pokemonList[pokemonChosenForPC]->attack / pc->pokemonList[pokemonChosenForPC]->defense))
          / 100) + 2) * rand() % 16 + 85; 
       wildPokemon->currentHp = wildPokemon->currentHp - damage;
        clear();
        refresh();
        mvprintw(1,0,"Your pokemon %s used move %s with damage %d!", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier, moves[pcMove.move_id].identifier, damage);

        if(wildPokemon->currentHp <= 0){
          mvprintw(2,0,"Press 'c' to continue with the fight!");
          refresh();
          while(uInput2 != 'c'){
            uInput2 = getch();
          }
          wildPokemon->currentHp = 0;
          return;
        }
        if(wildPokemon->currentHp){
          int damage = ((((((2 * wildPokemon->level) / 5) +2) * moves[npcMove.move_id].power * (wildPokemon->attack / wildPokemon->defense))
            / 100) + 2) * rand() % 16 + 85;
          pc->pokemonList[pokemonChosenForPC]->currentHp = pc->pokemonList[pokemonChosenForPC]->currentHp - damage;
          mvprintw(2,0,"Wild pokemon %s used move %s with damage %d!", wildPokemon->pokemonIdentifier, moves[npcMove.move_id].identifier, damage);
          mvprintw(3,0,"Press 'c' to continue with the fight!");
          refresh();
          while(uInput2 != 'c'){
            uInput2 = getch();
          }
          uInput2 = 'v';
          if(pc->pokemonList[pokemonChosenForPC]->currentHp < 0){
            pc->pokemonList[pokemonChosenForPC]->currentHp = 0;
            return;
          }
        }
      }
    }
    else{//npc goes first!
      if(rand() % 100 < moves[npcMove.move_id].accuracy){//move hits!!
        int damage = ((((((2 * wildPokemon->level) / 5) +2) * moves[npcMove.move_id].power * (wildPokemon->attack / wildPokemon->defense))
            / 100) + 2) * rand() % 16 + 85;
        pc->pokemonList[pokemonChosenForPC]->currentHp = pc->pokemonList[pokemonChosenForPC]->currentHp - damage;
        clear();
        refresh();
        mvprintw(1,0,"Wild pokemon %s used move %s with damage %d!", wildPokemon->pokemonIdentifier, moves[npcMove.move_id].identifier, damage);
        if(pc->pokemonList[pokemonChosenForPC]->currentHp <= 0){
          mvprintw(2,0,"Press 'c' to continue with the fight!");
          refresh();
          while(uInput2 != 'c'){
            uInput2 = getch();
          }
          uInput2 = 'v';
          pc->pokemonList[pokemonChosenForPC]->currentHp = 0;
          return;
        }
        if(pc->pokemonList[pokemonChosenForPC]->currentHp > 0){
          int damage = ((((((2 * pc->pokemonList[pokemonChosenForPC]->level) / 5) +2) * moves[pcMove.move_id].power * (pc->pokemonList[pokemonChosenForPC]->attack / pc->pokemonList[pokemonChosenForPC]->defense))
          / 100) + 2) * rand() % 16 + 85; 
          wildPokemon->currentHp = wildPokemon->currentHp - damage;
          mvprintw(2,0,"Your pokemon %s used move %s with damage %d!", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier, moves[pcMove.move_id].identifier, damage);
          mvprintw(3,0,"Press 'c' to continue with the fight!");
          refresh();
          while(uInput2 != 'c'){
            uInput2 = getch();
          }
          uInput2 = 'v';
          if(wildPokemon->currentHp < 0){
            wildPokemon->currentHp = 0;
            return;
          }
        }
      }
    }
}

void battleWildPokemon(Characters *pc, pokemon_generator *wildPokemon){
  char uInput = 'v';
  while(containsAlivePokemon(pc) == true && wildPokemon->currentHp > 0){
    uInput = 'v';
    int pokemonChosenForPC = indexOfFirstAlivePokemon(pc);
    clear();
    refresh();
    mvprintw(0,0,"Time for battle! Your pokemon %s with hp %d againgst wild pokemon %s with %d", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier, 
    pc->pokemonList[pokemonChosenForPC]->currentHp ,wildPokemon->pokemonIdentifier,wildPokemon->currentHp);
    mvprintw(1,0,"Your options are to fight 'f', check your bag 'B', switch pokemon 's', or escape encounter 'e'.");
    refresh();
    while(uInput != 'f' && uInput != 'B' && uInput != 's' && uInput != 'e'){
      uInput = getch();
      if(uInput == 'f' || uInput == 'B' || uInput == 's' || uInput == 'e'){
        break;
      }
    }
    
    if(pokemonChosenForPC == 7){
      break;
    }
  
    if(uInput == 'B'){
      //method possibly?
      uInput = 'v';
      if(bagPokeballs > 0){
        clear();
        refresh();
        mvprintw(0,0,"You have %d pokeballs. Would you like to use it to capture this pokemon? 'y' or 'n'", bagPokeballs);
        refresh();
        while(uInput != 'y' && uInput != 'n'){
          uInput = getch();
        }
        if(uInput == 'y'){
          if(indexOfNumberOfPokemons(pc) != 6){
            bagPokeballs = bagPokeballs - 1;
            mvprintw(1,0,"Pokemon captured!");
            pc->pokemonList[indexOfNumberOfPokemons(pc)] = wildPokemon;
          }
          mvprintw(2,0,"Press 'c' to continue this journey");
          while(uInput != 'c'){
            uInput = getch();
          }
          return;
        }
      }else{
        clear();
        refresh();
        mvprintw(0,0,"You have no pokeballs to use! Visit a pokemart to get one!");
        mvprintw(1,0,"Press 'c' to continue this battle");
        while(uInput != 'c'){
          uInput = getch();
        }
      }
    }
    if(uInput == 's'){
      clear();
      refresh();
      mvprintw(0,0,"Here is a list of your pokemons that have hp:");
      int j = 1;
      for(int i = 0; i < 6; i++){
        if(pc->pokemonList[i] != NULL){
          if(pc->pokemonList[i]->currentHp > 0){
            mvprintw(j, 0, "%d: %s ", i, pc->pokemonList[i]->pokemonIdentifier);
            j++;
          }
        }
      }
      mvprintw(j, 0, "Pick which pokemon to select based on the number it corresponds to:");
      uInput = getch();
      pokemonChosenForPC = int(uInput - '0');
      clear();
      refresh();
      mvprintw(1,0,"You selected %s", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier);
      mvprintw(2,0,"Press 'c' to continue this battle");
      while(uInput != 'c'){
        uInput = getch();
      }

      fightPk(pc,wildPokemon,pokemonChosenForPC);
      clear();
      refresh();
    }

    if(uInput == 'f'){
      fightPk(pc,wildPokemon,pokemonChosenForPC);
      clear();
      refresh();
    }

    if(uInput == 'e'){
      static int count = 1;
      int oddsEscape = floor((pc->pokemonList[pokemonChosenForPC]->speed * 32) / fmod(floor(wildPokemon->speed / 4), 256)) + 30 * count;
      count = count + 1;
      if(rand() % 256 < oddsEscape){
        clear();
        refresh();
        mvprintw(0,0,"Successfully fleed the battle. Press 'c' to continue your journey");
        mvprintw(1,0,"Press 'c' to continue this battle");
        while(uInput != 'c'){
          uInput = getch();
        }
        delete wildPokemon;
        return;
      }
      else{
        clear();
        refresh();
        mvprintw(0,0,"Attempt to escape failed! Press 'c' to continue this battle.");
        mvprintw(1,0,"Press 'c' to continue this battle");
        while(uInput != 'c'){
          uInput = getch();
        }
      }
    }

    if(containsAlivePokemon(pc)==false || wildPokemon->currentHp == 0){
      break;
    }
  }

  if(wildPokemon->currentHp == 0){
    delete wildPokemon;
    uInput = 'v';
    clear();
    refresh();
    mvprintw(1,0,"Enemy Pokemon has been defeated! Press 'c' to continue with your journey!");
    while(uInput != 'c'){
      uInput = getch();
    }
  }
}



void fight(Characters *pc, Characters *npc, int pokemonChosenForPC, int pokemonChosenForNPC, bool regularFightOrJustNPCMove){
  char uInput2 = 'v';
  pokemon_move_db npcMove = npc->pokemonList[pokemonChosenForNPC]->move1;
  pokemon_move_db pcMove = pc->pokemonList[pokemonChosenForPC]->move2;
  if(regularFightOrJustNPCMove == true){
    clear();
    refresh();
    mvprintw(0,0,"Battle between your pokemon %s and enemy pokemon %s", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier, npc->pokemonList[pokemonChosenForNPC]->pokemonIdentifier);
    mvprintw(1,0,"Pick your move:");
    mvprintw(2,0,"Move 1: %s, Move 2: %s", moves[pc->pokemonList[pokemonChosenForPC]->move1.move_id].identifier, moves[pc->pokemonList[pokemonChosenForPC]->move2.move_id].identifier);
    mvprintw(3,0,"Type '1' or '2' to select a move for fighting!");
    refresh();

    while(uInput2 != '1' && uInput2 != '2'){
      uInput2 = getch();
    }

    if(uInput2 == '1'){
      pcMove = pc->pokemonList[pokemonChosenForPC]->move1;
    }
    else{
      pcMove = pc->pokemonList[pokemonChosenForPC]->move2;
    }

    //who goes first
    if(moves[pcMove.move_id].priority >= moves[npcMove.move_id].priority){//pc goes first!
      if(rand() % 100 < moves[pcMove.move_id].accuracy){//move hits!!
        int damage = ((((((2 * pc->pokemonList[pokemonChosenForPC]->level) / 5) +2) * moves[pcMove.move_id].power * (pc->pokemonList[pokemonChosenForPC]->attack / pc->pokemonList[pokemonChosenForPC]->defense))
          / 100) + 2) * rand() % 16 + 85; 
        npc->pokemonList[pokemonChosenForNPC]->currentHp = npc->pokemonList[pokemonChosenForNPC]->currentHp - damage;
        clear();
        refresh();
        mvprintw(1,0,"Your pokemon %s used move %s with damage %d!", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier, moves[pcMove.move_id].identifier, damage);

        if(npc->pokemonList[pokemonChosenForNPC]->currentHp <= 0){
          mvprintw(2,0,"Press 'c' to continue with the fight!");
          refresh();
          while(uInput2 != 'c'){
            uInput2 = getch();
          }
          uInput2 = 'v';
          npc->pokemonList[pokemonChosenForNPC]->currentHp = 0;
          return;
        }
        if(npc->pokemonList[pokemonChosenForNPC]->currentHp > 0){
          int damage = ((((((2 * npc->pokemonList[pokemonChosenForNPC]->level) / 5) +2) * moves[npcMove.move_id].power * (npc->pokemonList[pokemonChosenForNPC]->attack / npc->pokemonList[pokemonChosenForNPC]->defense))
            / 100) + 2) * rand() % 16 + 85;
          pc->pokemonList[pokemonChosenForPC]->currentHp = pc->pokemonList[pokemonChosenForPC]->currentHp - damage;
          mvprintw(2,0,"Enemy pokemon %s used move %s with damage %d!", npc->pokemonList[pokemonChosenForNPC]->pokemonIdentifier, moves[npcMove.move_id].identifier, damage);
          mvprintw(3,0,"Press 'c' to continue with the fight!");
          refresh();
          while(uInput2 != 'c'){
            uInput2 = getch();
          }
          uInput2 = 'v';
          if(pc->pokemonList[pokemonChosenForPC]->currentHp < 0){
            pc->pokemonList[pokemonChosenForPC]->currentHp = 0;
            return;
          }
        }
      }
    }
    else{//npc goes first!
      if(rand() % 100 < moves[npcMove.move_id].accuracy){//move hits!!
        int damage = ((((((2 * npc->pokemonList[pokemonChosenForNPC]->level) / 5) +2) * moves[npcMove.move_id].power * (npc->pokemonList[pokemonChosenForNPC]->attack / npc->pokemonList[pokemonChosenForNPC]->defense))
            / 100) + 2) * rand() % 16 + 85;
        pc->pokemonList[pokemonChosenForPC]->currentHp = pc->pokemonList[pokemonChosenForPC]->currentHp - damage;
        clear();
        refresh();
        mvprintw(1,0,"Enemy pokemon %s used move %s with damage %d!", npc->pokemonList[pokemonChosenForNPC]->pokemonIdentifier, moves[npcMove.move_id].identifier, damage);
        if(pc->pokemonList[pokemonChosenForPC]->currentHp <= 0){
          mvprintw(2,0,"Press 'c' to continue with the fight!");
          refresh();
          while(uInput2 != 'c'){
            uInput2 = getch();
          }
          uInput2 = 'v';
          pc->pokemonList[pokemonChosenForPC]->currentHp = 0;
          return;
        }
        if(pc->pokemonList[pokemonChosenForPC]->currentHp > 0){
          int damage = ((((((2 * pc->pokemonList[pokemonChosenForPC]->level) / 5) +2) * moves[pcMove.move_id].power * (pc->pokemonList[pokemonChosenForPC]->attack / pc->pokemonList[pokemonChosenForPC]->defense))
          / 100) + 2) * rand() % 16 + 85; 
          npc->pokemonList[pokemonChosenForNPC]->currentHp = npc->pokemonList[pokemonChosenForNPC]->currentHp - damage;
          mvprintw(2,0,"Your pokemon %s used move %s with damage %d!", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier, moves[pcMove.move_id].identifier, damage);
          mvprintw(3,0,"Press 'c' to continue with the fight!");
          refresh();
          while(uInput2 != 'c'){
            uInput2 = getch();
          }
          uInput2 = 'v';
          if(npc->pokemonList[pokemonChosenForNPC]->currentHp < 0){
            npc->pokemonList[pokemonChosenForNPC]->currentHp = 0;
            return;
          }
        }
      }
    }
  }
  else{
    if(rand() % 100 < moves[npcMove.move_id].accuracy){//move hits!!
        int damage = ((((((2 * npc->pokemonList[pokemonChosenForNPC]->level) / 5) +2) * moves[npcMove.move_id].power * (npc->pokemonList[pokemonChosenForNPC]->attack / npc->pokemonList[pokemonChosenForNPC]->defense))
            / 100) + 2) * rand() % 16 + 85;
        pc->pokemonList[pokemonChosenForPC]->currentHp = pc->pokemonList[pokemonChosenForPC]->currentHp - damage;
        clear();
        refresh();
        mvprintw(1,0,"Enemy pokemon %s used move %s with damage %d!", npc->pokemonList[pokemonChosenForNPC]->pokemonIdentifier, moves[npcMove.move_id].identifier, damage);
        if(pc->pokemonList[pokemonChosenForPC]->currentHp <= 0){
          pc->pokemonList[pokemonChosenForPC]->currentHp = 0;
        }
        mvprintw(2,0,"Press 'c' to continue with the fight!");
        refresh();
        while(uInput2 != 'c'){
          uInput2 = getch();
        }
        uInput2 = 'v';
    }
  }

}

void battleTrainer(Characters *pc, Characters *npc){
  char uInput = 'v';
  while(containsAlivePokemon(pc) == true && containsAlivePokemon(npc) == true){
    uInput = 'v';
    int pokemonChosenForNPC = indexOfFirstAlivePokemon(npc);
    int pokemonChosenForPC = indexOfFirstAlivePokemon(pc);
    clear();
    refresh();
    mvprintw(0,0,"Time for battle! Your pokemon %s with hp %d againgst enemy pokemon %s with %d", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier, 
    pc->pokemonList[pokemonChosenForPC]->currentHp ,npc->pokemonList[pokemonChosenForNPC]->pokemonIdentifier,npc->pokemonList[pokemonChosenForNPC]->currentHp);
    mvprintw(1,0,"Your options are to fight 'f', check your bag 'B', or switch pokemon 's'.");
    refresh();
    while(uInput != 'f' && uInput != 'B' && uInput != 's'){
      uInput = getch();
      if(uInput == 'f' || uInput == 'B' || uInput == 's'){
        break;
      }
    }
    
    if(pokemonChosenForPC == 7){
      break;
    }
    if(pokemonChosenForNPC == 7){
      break;
    }
  
    if(uInput == 'B'){
      //method possibly?
      uInput = 'v';
      if(bagPotion > 0){
        clear();
        refresh();
        mvprintw(0,0,"You have %d potions/revivals. Would you like to use it on your pokemon? 'y' or 'n'", bagPotion);
        refresh();
        while(uInput != 'y' && uInput != 'n'){
          uInput = getch();
        }
        if(uInput == 'y'){
          bagPotion = bagPotion - 1;
          pc->pokemonList[pokemonChosenForPC]->currentHp = pc->pokemonList[pokemonChosenForPC]->hp;
          mvprintw(1,0,"Pokemon health restored!");
          uInput = 'v';
          mvprintw(2,0,"Press 'c' to continue this battle");
          while(uInput != 'c'){
            uInput = getch();
          }
          fight(pc, npc, pokemonChosenForPC, pokemonChosenForNPC, false);
          clear();
          refresh();
        }
      }else{
        clear();
        refresh();
        mvprintw(0,0,"You have no potions to use! Visit a pokemart to get one!");
        mvprintw(1,0,"Press 'c' to continue this battle");
        while(uInput != 'c'){
          uInput = getch();
        }
      }
    

    }
    if(uInput == 's'){
      clear();
      refresh();
      mvprintw(0,0,"Here is a list of your pokemons that have hp:");
      int j = 1;
      for(int i = 0; i < 6; i++){
        if(pc->pokemonList[i] != NULL){
          if(pc->pokemonList[i]->currentHp > 0){
            mvprintw(j, 0, "%d: %s ", i, pc->pokemonList[i]->pokemonIdentifier);
            j++;
          }
        }
      }
      mvprintw(j, 0, "Pick which pokemon to select based on the number it corresponds to:");
      uInput = getch();
      pokemonChosenForPC = int(uInput - '0');
      clear();
      refresh();
      mvprintw(1,0,"You selected %s", pc->pokemonList[pokemonChosenForPC]->pokemonIdentifier);
      mvprintw(2,0,"Press 'c' to continue this battle");
      while(uInput != 'c'){
        uInput = getch();
      }

      fight(pc,npc,pokemonChosenForPC,pokemonChosenForNPC, false);
      clear();
      refresh();
    }

    if(uInput == 'f'){
      fight(pc, npc, pokemonChosenForPC, pokemonChosenForNPC, true);
      clear();
      refresh();
    }

    if(containsAlivePokemon(pc)==false || containsAlivePokemon(npc) == false){
      break;
    }
  }

  if(containsAlivePokemon(npc) == false){
    uInput = 'v';
    clear();
    refresh();
    mvprintw(1,0,"Enemy trainer has been defeated! Press 'c' to continue with your journey!");
    while(uInput != 'c'){
      uInput = getch();
    }
  }
}

void initializeNewMap(map_t *world, int x, int y){
  world->cPQ = newCharacter(pcXcoord, pcYcoord,0,0,'@',-1);
  Characters *addCharacterPC = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
  for(int i = 0; i<21; i++){
    for(int j = 0; j<80; j++){
      world->characterTable[i][j] = NULL;
    }
  }
  world->characterTable[pcXcoord][pcYcoord] = addCharacterPC;
  // world->characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
  placeCharacter(world->grid, world->characterTable, 10, 1, -1, &(world->cPQ),x, y);
  placeCharacter(world->grid, world->characterTable, 11, 2, -1, &(world->cPQ),x, y);
  placeCharacter(world->grid, world->characterTable, 12, 3, -1, &(world->cPQ),x, y);
  placeCharacter(world->grid, world->characterTable, 13, 4, -1, &(world->cPQ),x, y);
  placeCharacter(world->grid, world->characterTable, 14, 5, -1, &(world->cPQ),x, y);
  placeCharacter(world->grid, world->characterTable, 14, 7, -1, &(world->cPQ),x, y);
  placeCharacter(world->grid, world->characterTable, 14, 8, -1, &(world->cPQ),x, y);
  placeCharacter(world->grid, world->characterTable, 15, 6, -1, &(world->cPQ),x, y);
  // dijkstrasMethod(world->grid, 0, distanceMapForHiker);
  // dijkstrasMethod(world->grid, 1, distanceMapForRider);

}



//have direction in the place character struct??
void placeCharacter(int grid[21][80], Characters *characterTable[21][80], int characterNumber, int seqNum, int direction, Characters **head, int x, int y){
  int xCoord = ((rand() % 17) + 2);
  int yCoord = ((rand() % 76) + 2);
  //(1 || 2 || 7 || 8 || 3 || 4 || 10 || 11 || 12 || 13 || 14 )
  while(characterTable[xCoord][yCoord]!=NULL){
    xCoord = ((rand() % 17) + 2);
    yCoord = ((rand() % 76) + 2);
    if(characterTable[xCoord][yCoord]==NULL){
      break;
    }
  }

  // characterGrid[xCoord][yCoord] = characterNumber;
  pokemon_generator *pk1 = new pokemon_generator(x, y);
  // pokemon_generator *pk2 = new pokemon_generator(x, y);
  characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, characterNumber, 0);
  characterTable[xCoord][yCoord]->pokemonList[0] = pk1;
  // characterTable[xCoord][yCoord]->pokemonList[1] = pk2;
  characterTable[xCoord][yCoord]->pokemonList[1] = NULL;
  characterTable[xCoord][yCoord]->pokemonList[2] = NULL;
  characterTable[xCoord][yCoord]->pokemonList[3] = NULL;
  characterTable[xCoord][yCoord]->pokemonList[4] = NULL;
  characterTable[xCoord][yCoord]->pokemonList[5] = NULL;
  int i;
  for(i = 1; i< 6; i++){
    if(rand() % 100 > 39){
      pokemon_generator *pk = new pokemon_generator(x,y);
      characterTable[xCoord][yCoord]->pokemonList[i] = pk;
    }
    else{
      break;
    }
  }
  
  

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

void movePacer(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head){
        Characters *peeked = peekCharacter(head);
        if(*direction == -1){
          *direction = (rand() % 8) + 1;
        }
        int xCoord = peeked->xCoord;
        int yCoord = peeked->yCoord;
        int nextTurn = peeked->nextTurn;
        int seqNum = peeked->seqNum;
        int battled = characterTable[xCoord][yCoord]->battled;
        // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
        free(characterTable[xCoord][yCoord]);
        characterTable[xCoord][yCoord] = NULL;
        int change = 0;
        int count = 0;
        while(change != 1 && count != 9){
          if(*direction ==1){//go north
            if(xCoord > 1 && (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX)&& (characterTable[xCoord-1][yCoord] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord-1, yCoord, nextTurn + costOfTerrain(xCoord -1, yCoord, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord-1][yCoord] = 12;
              characterTable[xCoord-1][yCoord] = newCharacterOnMap(xCoord-1,yCoord,12,battled);
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
            if(xCoord > 1 && yCoord < 78 && (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX)&& (characterTable[xCoord-1][yCoord+1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord-1, yCoord+1, nextTurn + costOfTerrain(xCoord -1, yCoord+1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord-1][yCoord+1] = 12;
              characterTable[xCoord-1][yCoord+1] = newCharacterOnMap(xCoord-1, yCoord+1, 12, battled);
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
            if(yCoord < 78 && (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX)&& (characterTable[xCoord][yCoord+1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord, yCoord+1, nextTurn + costOfTerrain(xCoord, yCoord+1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord][yCoord+1] = 12;
              characterTable[xCoord][yCoord+1] = newCharacterOnMap(xCoord, yCoord+1, 12, battled);
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
            if(xCoord < 19 && yCoord < 78 && (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)&& (characterTable[xCoord+1][yCoord+1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord+1, yCoord+1, nextTurn + costOfTerrain(xCoord +1, yCoord+1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord+1][yCoord+1] = 12;
              characterTable[xCoord+1][yCoord+1] = newCharacterOnMap(xCoord+1, yCoord+1,12,battled);
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
            if(xCoord < 19 && (costOfTerrain(xCoord +1, yCoord, grid, 1) != INT_MAX)&& (characterTable[xCoord+1][yCoord] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord+1, yCoord, nextTurn + costOfTerrain(xCoord +1, yCoord, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord+1][yCoord] = 12;
              characterTable[xCoord+1][yCoord] = newCharacterOnMap(xCoord+1,yCoord,12, battled);
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
            if(xCoord < 19 && yCoord > 1 && (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX)&& (characterTable[xCoord+1][yCoord-1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord+1, yCoord-1, nextTurn + costOfTerrain(xCoord +1, yCoord-1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord+1][yCoord-1] = 12;
              characterTable[xCoord+1][yCoord-1] = newCharacterOnMap(xCoord+1,yCoord-1,12,battled);
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
            if(yCoord > 1 && (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX)&& (characterTable[xCoord][yCoord-1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord, yCoord-1, nextTurn + costOfTerrain(xCoord, yCoord-1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord][yCoord-1] = 12;
              characterTable[xCoord][yCoord-1] = newCharacterOnMap(xCoord,yCoord-1,12,battled);
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
            if(xCoord > 1 && yCoord > 1 && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX)&& (characterTable[xCoord-1][yCoord-1] == NULL)){
              popCharacter(head);
              pushCharacter(head, xCoord-1, yCoord-1, nextTurn + costOfTerrain(xCoord -1, yCoord-1, grid, 1), seqNum, 'p', *direction);
              // characterGrid[xCoord-1][yCoord-1] = 12;
              characterTable[xCoord-1][yCoord-1] = newCharacterOnMap(xCoord-1,yCoord-1, 12, battled);
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
          pushCharacter(head, xCoord, yCoord, nextTurn + costOfTerrain(xCoord, yCoord, grid, 1), seqNum, 'p', *direction);
          // characterGrid[xCoord][yCoord] = 12;
          characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord,yCoord, 12, battled);
        }
}

void moveExplorer(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head){
  Characters *peeked = peekCharacter(head);
  if(*direction == -1){
     *direction = (rand() % 8) + 1;
  }
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int seqNum = peeked->seqNum;
  int nextTurn = peeked->nextTurn;
  int battled = characterTable[xCoord][yCoord]->battled;
  // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  free(characterTable[xCoord][yCoord]);
  characterTable[xCoord][yCoord] = NULL;
  int change = 0;
  int count = 0;
  while(change != 1 && count != 9){
    if(*direction == 1){
      if(xCoord > 1 && (characterTable[xCoord-1][yCoord] == NULL) && (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord, nextTurn + costOfTerrain(xCoord -1, yCoord, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord-1][yCoord] = 14;
        characterTable[xCoord-1][yCoord] = newCharacterOnMap(xCoord-1, yCoord, 14, battled);
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
      if(xCoord > 1 && yCoord < 78 && (characterTable[xCoord-1][yCoord+1] == NULL) && (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord+1, nextTurn + costOfTerrain(xCoord -1, yCoord+1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord-1][yCoord+1] = 14;
        characterTable[xCoord-1][yCoord+1] = newCharacterOnMap(xCoord-1, yCoord+1, 14, battled);
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
      if(yCoord < 78 && (characterTable[xCoord][yCoord+1] == NULL) && (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord+1, nextTurn + costOfTerrain(xCoord, yCoord+1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord][yCoord+1] = 14;
        characterTable[xCoord][yCoord+1] = newCharacterOnMap(xCoord, yCoord+1, 14, battled);
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
      if(xCoord < 19 && yCoord < 78 && (characterTable[xCoord+1][yCoord+1] == NULL) && (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord+1, nextTurn + costOfTerrain(xCoord +1, yCoord+1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord+1][yCoord+1] = 14;
        characterTable[xCoord+1][yCoord+1] = newCharacterOnMap(xCoord+1, yCoord+1, 14, battled);
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
      if(xCoord < 19 && (characterTable[xCoord+1][yCoord] == NULL) && (costOfTerrain(xCoord +1, yCoord, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord, nextTurn + costOfTerrain(xCoord+1, yCoord, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord+1][yCoord] = 14;
        characterTable[xCoord+1][yCoord] = newCharacterOnMap(xCoord+1, yCoord, 14, battled);
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
      if(xCoord < 19 && yCoord > 1 && (characterTable[xCoord+1][yCoord-1] == NULL) && (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord-1, nextTurn + costOfTerrain(xCoord +1, yCoord-1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord+1][yCoord-1] = 14;
        characterTable[xCoord+1][yCoord-1] = newCharacterOnMap(xCoord+1, yCoord-1, 14, battled);
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
      if(yCoord > 1 && (characterTable[xCoord][yCoord-1] == NULL) && (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord-1, nextTurn + costOfTerrain(xCoord, yCoord-1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord][yCoord-1] = 14;
        characterTable[xCoord][yCoord-1] = newCharacterOnMap(xCoord, yCoord-1, 14, battled);
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
      if(xCoord > 2 && yCoord > 1 && (characterTable[xCoord-1][yCoord-1] == NULL) && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX)){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord-1, nextTurn + costOfTerrain(xCoord -1, yCoord-1, grid, 1), seqNum, 'e', *direction);
        // characterGrid[xCoord-1][yCoord-1] = 14;
        characterTable[xCoord-1][yCoord-1] = newCharacterOnMap(xCoord-1, yCoord-1, 14, battled);
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
    pushCharacter(head, xCoord, yCoord, nextTurn + costOfTerrain(xCoord, yCoord, grid, 1), seqNum, 'e', *direction);
    // characterGrid[xCoord][yCoord] = 14;
    characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, 14, battled);
  }
}

void moveWander(int grid[21][80], Characters *characterTable[21][80], int *direction, Characters **head){
  Characters *peeked = peekCharacter(head);
  if(*direction == -1){
    *direction = (rand() % 8) + 1;
  }
  int xCoord = peeked->xCoord;
  int yCoord = peeked->yCoord;
  int seqNum = peeked->seqNum;
  int nextTurn = peeked->nextTurn;
  int battled = characterTable[xCoord][yCoord]->battled;
  // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  free(characterTable[xCoord][yCoord]);
  characterTable[xCoord][yCoord]=NULL;
  int change = 0;
  int count = 0;
  while(change != 1 && count != 9){
    if(*direction == 1){
      if(xCoord > 1 && characterTable[xCoord-1][yCoord] == NULL && (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord -1, yCoord, grid, 1) != INT_MAX) && (grid[xCoord-1][yCoord] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord, nextTurn + costOfTerrain(xCoord -1, yCoord, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord-1][yCoord] = 15;
        characterTable[xCoord-1][yCoord] = newCharacterOnMap(xCoord-1, yCoord, 15, battled);
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
      if(xCoord > 1 && yCoord < 78 && characterTable[xCoord-1][yCoord+1] == NULL && (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord -1, yCoord+1, grid, 1) != INT_MAX) && (grid[xCoord-1][yCoord+1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord+1, nextTurn + costOfTerrain(xCoord -1, yCoord+1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord-1][yCoord+1] = 15;
        characterTable[xCoord-1][yCoord+1] = newCharacterOnMap(xCoord-1, yCoord+1, 15, battled);
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
      if(yCoord < 78 && characterTable[xCoord][yCoord+1] == NULL && (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord, yCoord+1, grid, 1) != INT_MAX) && (grid[xCoord][yCoord+1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord+1, nextTurn + costOfTerrain(xCoord, yCoord+1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord][yCoord+1] = 15;
        characterTable[xCoord][yCoord+1] = newCharacterOnMap(xCoord, yCoord+1, 15, battled);
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
      if(xCoord < 19 && yCoord < 78 && characterTable[xCoord+1][yCoord+1] == NULL && (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord+1, grid, 1) != INT_MAX)&& (grid[xCoord+1][yCoord+1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord+1, nextTurn + costOfTerrain(xCoord +1, yCoord+1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord+1][yCoord+1] = 15;
        characterTable[xCoord+1][yCoord+1] = newCharacterOnMap(xCoord+1, yCoord+1, 15, battled);
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
      if(xCoord < 19 && characterTable[xCoord+1][yCoord] == NULL && (costOfTerrain(xCoord+1, yCoord, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord, grid, 1) != INT_MAX)&& (grid[xCoord+1][yCoord] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord, nextTurn + costOfTerrain(xCoord+1, yCoord, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord+1][yCoord] = 15;
        characterTable[xCoord+1][yCoord] = newCharacterOnMap(xCoord+1, yCoord, 15, battled);
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
      if(xCoord < 19 && yCoord > 1 && characterTable[xCoord+1][yCoord-1] == NULL && (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord +1, yCoord-1, grid, 1) != INT_MAX) && (grid[xCoord+1][yCoord-1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord+1, yCoord-1, nextTurn + costOfTerrain(xCoord +1, yCoord-1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord+1][yCoord-1] = 15;
        characterTable[xCoord+1][yCoord-1] = newCharacterOnMap(xCoord+1, yCoord-1, 15, battled);
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
      if(yCoord > 1 && characterTable[xCoord][yCoord-1] == NULL && (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX)&& (costOfTerrain(xCoord, yCoord-1, grid, 1) != INT_MAX) && (grid[xCoord][yCoord-1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord, yCoord-1, nextTurn + costOfTerrain(xCoord, yCoord-1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord][yCoord-1] = 15;
        characterTable[xCoord][yCoord-1] = newCharacterOnMap(xCoord, yCoord-1, 15, battled);
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
      if(xCoord > 1 && yCoord > 1 && characterTable[xCoord-1][yCoord-1] == NULL && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX) && (costOfTerrain(xCoord -1, yCoord-1, grid, 1) != INT_MAX) && (grid[xCoord-1][yCoord-1] == grid[xCoord][yCoord])){
        popCharacter(head);
        pushCharacter(head, xCoord-1, yCoord-1, nextTurn + costOfTerrain(xCoord -1, yCoord-1, grid, 1), seqNum, 'w', *direction);
        // characterGrid[xCoord-1][yCoord-1] = 15;
        characterTable[xCoord-1][yCoord-1] = newCharacterOnMap(xCoord-1, yCoord-1, 15, battled);
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
    pushCharacter(head, xCoord, yCoord, nextTurn + costOfTerrain(xCoord, yCoord, grid, 1), seqNum, 'w', *direction);
    // characterGrid[xCoord][yCoord] = 15;
    characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, 15, battled);
  }
}


void moveRival(int grid[21][80], Characters *characterTable[21][80], int distanceMap[21][80], Characters **head){
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
  int battled = characterTable[xCoord][yCoord]->battled;
  // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  free(characterTable[xCoord][yCoord]);
  characterTable[xCoord][yCoord] = NULL;
  //if character table is not taken by a character
  if(battled == 1){
    popCharacter(head);
    characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord,yCoord,11,battled);
  }
  else{
    if(xCoord > 1 && characterTable[xCoord-1][yCoord]==NULL){
      if(distanceMap[xCoord -1][yCoord] != 0 && distanceMap[xCoord -1][yCoord] < distance){
        distance = distanceMap[xCoord -1][yCoord];
        minX = xCoord -1;
        minY = yCoord;
      }
    }

    if(xCoord > 1 && yCoord < 78 && characterTable[xCoord-1][yCoord+1]==NULL){
      if(distanceMap[xCoord -1][yCoord+1] != 0 && distanceMap[xCoord -1][yCoord+1] < distance){
        distance = distanceMap[xCoord -1][yCoord+1];
        minX = xCoord -1;
        minY = yCoord+1;
      }
    }

    if(yCoord < 78 && characterTable[xCoord][yCoord+1]==NULL){
      if(distanceMap[xCoord][yCoord+1] != 0 && distanceMap[xCoord][yCoord+1] < distance){
        distance = distanceMap[xCoord][yCoord+1];
        minX = xCoord;
        minY = yCoord+1;
      }
    }

    if(xCoord < 19 && yCoord < 78 && characterTable[xCoord+1][yCoord+1]==NULL){
      if(distanceMap[xCoord +1][yCoord+1] != 0 && distanceMap[xCoord +1][yCoord+1] < distance){
        distance = distanceMap[xCoord +1][yCoord+1];
        minX = xCoord +1;
        minY = yCoord+1;
      }
    }

    if(xCoord < 19 && characterTable[xCoord+1][yCoord]==NULL){
      if(distanceMap[xCoord +1][yCoord] != 0 && distanceMap[xCoord +1][yCoord] < distance){
        distance = distanceMap[xCoord +1][yCoord];
        minX = xCoord +1;
        minY = yCoord;
      }
    }

    if(xCoord < 19 && yCoord > 1 && characterTable[xCoord+1][yCoord-1]==NULL){
      if(distanceMap[xCoord +1][yCoord-1] != 0 && distanceMap[xCoord +1][yCoord-1] < distance){
        distance = distanceMap[xCoord +1][yCoord-1];
        minX = xCoord +1;
        minY = yCoord-1;
      }
    }

    if(yCoord > 1 && characterTable[xCoord][yCoord-1]==NULL){
      if(distanceMap[xCoord ][yCoord-1] != 0 && distanceMap[xCoord][yCoord-1] < distance){
        distance = distanceMap[xCoord][yCoord-1];
        minX = xCoord;
        minY = yCoord-1;
      }
    }

    if(xCoord > 1 && yCoord > 1 && characterTable[xCoord-1][yCoord-1]==NULL){
      if(distanceMap[xCoord -1][yCoord-1] != 0 && distanceMap[xCoord -1][yCoord-1] < distance){
        distance = distanceMap[xCoord -1][yCoord-1];
        minX = xCoord -1;
        minY = yCoord-1;
      }
    }
    int seqNum = peeked->seqNum;
    if(minX != 0 && minY != 0){
      int nextTurn = (peeked->nextTurn) + costOfTerrain(minX, minY, grid, 1);
      popCharacter(head);
      pushCharacter(head, minX, minY, nextTurn, seqNum, 'r', direction);
      // characterGrid[minX][minY] = 11;
      characterTable[minX][minY] = newCharacterOnMap(minX, minY, 11, battled);
    }
    else{
      int nextTurn = (peeked->nextTurn) + costOfTerrain(xCoord, yCoord, grid, 1);
      popCharacter(head);
      pushCharacter(head, xCoord, yCoord, nextTurn, seqNum, 'r', direction);
      // characterGrid[xCoord][yCoord] = 11;
      characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, 11, battled);
    }
  }

}

void moveHiker(int grid[21][80], Characters *characterTable[21][80], int distanceMap[21][80], Characters **head){
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
  int battled = characterTable[xCoord][yCoord]->battled;
  // characterGrid[xCoord][yCoord] = grid[xCoord][yCoord];
  free(characterTable[xCoord][yCoord]);
  characterTable[xCoord][yCoord]=NULL;
  //if character table is not taken by a 
  if(battled == 1){
    popCharacter(head);
    characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord,yCoord,10,battled);
  }
  else{
    if(xCoord > 1 && characterTable[xCoord-1][yCoord]==NULL){
      if(distanceMap[xCoord -1][yCoord] != 0 && distanceMap[xCoord -1][yCoord] < distance){
        distance = distanceMap[xCoord -1][yCoord];
        minX = xCoord -1;
        minY = yCoord;
      }
    }

    if(xCoord > 1 && yCoord < 78 && characterTable[xCoord-1][yCoord+1]==NULL){
      if(distanceMap[xCoord -1][yCoord+1] != 0 && distanceMap[xCoord -1][yCoord+1] < distance){
        distance = distanceMap[xCoord -1][yCoord+1];
        minX = xCoord -1;
        minY = yCoord+1;
      }
    }

    if(yCoord < 78 && characterTable[xCoord][yCoord+1]==NULL){
      if(distanceMap[xCoord][yCoord+1] != 0 && distanceMap[xCoord][yCoord+1] < distance){
        distance = distanceMap[xCoord][yCoord+1];
        minX = xCoord;
        minY = yCoord+1;
      }
    }

    if(xCoord < 19 && yCoord < 78 && characterTable[xCoord+1][yCoord+1]==NULL){
      if(distanceMap[xCoord +1][yCoord+1] != 0 && distanceMap[xCoord +1][yCoord+1] < distance){
        distance = distanceMap[xCoord +1][yCoord+1];
        minX = xCoord +1;
        minY = yCoord+1;
      }
    }

    if(xCoord < 19 && characterTable[xCoord+1][yCoord]==NULL){
      if(distanceMap[xCoord +1][yCoord] != 0 && distanceMap[xCoord +1][yCoord] < distance){
        distance = distanceMap[xCoord +1][yCoord];
        minX = xCoord +1;
        minY = yCoord;
      }
    }

    if(xCoord < 19 && yCoord > 1 && characterTable[xCoord+1][yCoord-1]==NULL){
      if(distanceMap[xCoord +1][yCoord-1] != 0 && distanceMap[xCoord +1][yCoord-1] < distance){
        distance = distanceMap[xCoord +1][yCoord-1];
        minX = xCoord +1;
        minY = yCoord-1;
      }
    }

    if(yCoord > 1 && characterTable[xCoord][yCoord-1]==NULL){
      if(distanceMap[xCoord ][yCoord-1] != 0 && distanceMap[xCoord][yCoord-1] < distance){
        distance = distanceMap[xCoord][yCoord-1];
        minX = xCoord;
        minY = yCoord-1;
      }
    }

    if(xCoord > 1 && yCoord > 1 && characterTable[xCoord-1][yCoord-1]==NULL){
      if(distanceMap[xCoord -1][yCoord-1] != 0 && distanceMap[xCoord -1][yCoord-1] < distance){
        distance = distanceMap[xCoord -1][yCoord-1];
        minX = xCoord -1;
        minY = yCoord-1;
      }
    }
  int seqNum = peeked->seqNum;
    if(minX != 0 && minY != 0){
      int nextTurn = (peeked->nextTurn) + costOfTerrain(minX, minY, grid, 0);
      popCharacter(head);
      pushCharacter(head, minX, minY, nextTurn, seqNum, 'h', direction);
      // characterGrid[minX][minY] = 10;
      characterTable[minX][minY] = newCharacterOnMap(minX, minY, 10, battled);
    }else{
      int nextTurn = (peeked->nextTurn) + costOfTerrain(xCoord, yCoord, grid, 0);
      popCharacter(head);
      pushCharacter(head, xCoord, yCoord, nextTurn, seqNum, 'h', direction);
      // characterGrid[xCoord][yCoord] = 10;
      characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, 10, battled);
    }
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
  else if (hikerOrRider == 2){
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
  else{
    if(grid[x][y] == 2){
      return 10;
    }
    else if(grid[x][y] == 8){
      return 0;
    }
    else if((grid[x][y] == 3) || grid[x][y] == 4){
      return 10;
    }
    else if(grid[x][y] == 5){//Tall grass, put pokemon
            // move(0,0);
            // clrtoeol();
            // refresh();
            // mvprintw(0, 0,"Testing if this works");
            // refresh();
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
  distance[pcXcoord][pcYcoord] = 0;


  // printf("Making a queue\n");
  Node* pq = newNode(pcXcoord, pcYcoord, 0);
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
    if(isEmpty(&pq)){
      break;
    }
  }

  exitsUnreachable(distance);
  // printDistTable(distance);
}
