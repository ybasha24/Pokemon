#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>
#include<unistd.h>
#include <ncurses.h>
#include "worldGenerator.h"
#include "priorityQueue.h"
#include "characterQueue.h"

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
void placeCharacter(int grid[21][80], Characters *characterTable[21][80], int characterNumber, int seqNum, int direction, Characters **head);

int pcXcoord;
int pcYcoord;

// map_t *w[402][402];

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
      else if(type == 15){
        return 'w';
      }
}

void printDisplay(Characters *characterTable[21][80], int grid[21][80]){
  int y = 0;
  for(int i = 0; i < 21; i++){
    for(int j =0; j< 80; j++){
      y = j;
      if(characterTable[i][j]){
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
        else{
          mvaddch(i+1, j, '^');
        }
      }
     }
    mvaddch(i+2, y, '\n');
  }
}

int main(int argc, char *argv[]){
    // int pcXcoord; 
    // int pcYcoord;
    // pcXcoord = getPCX();
    // pcYcoord = getPCY();
    // int *pcXcoordNew = &(pcXcoord);
    // int *pcYcoordNew = &(pcYcoord);
    // *pcXcoordNew = getPCX();
    // *pcYcoordNew = getPCY();
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
    // printGrid(w[200][200]->grid);
    printf("\n");
    // int characterMovements [21][80] = {0};
    Characters *characterTable[21][80];
    // for(int i = 0; i < 21; i++){
    //   for(int j =0; j< 80; j++){
    //     characterMovements[i][j] = w[200][200]->grid[i][j];
    //   }
    // }
    // printGrid(characterMovements);
    //place npc method where parameter is grid, character grid, queue, character number, direction 
    if(x == 200 && y == 200){
      // characterMovements[getPCX()][getPCY()] = 8;
      w[200][200]->grid[getPCX()][getPCY()] = 2;
    }
  
    
    

    // printGrid(characterMovements);
    

    Characters *cPQ = newCharacter(getPCX(), getPCY(), 0, 0, '@', -1);
    // int distanceMapForHiker[21][80] ={0};
    // int distanceMapForRider[21][80] ={0};
    // dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
    // dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);



    if(argc == 1){
      char uInput = 'p';
      pcXcoord = getPCX();
      pcYcoord = getPCY();
      characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);

      placeCharacter(w[200][200]->grid, characterTable, 10, 1, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterTable, 11, 2, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterTable, 12, 3, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterTable, 13, 4, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterTable, 14, 5, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterTable, 14, 7, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterTable, 14, 8, -1, &cPQ);
      placeCharacter(w[200][200]->grid, characterTable, 15, 6, -1, &cPQ);
      int distanceMapForHiker[21][80] ={0};
      int distanceMapForRider[21][80] ={0};
      dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
      dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
      io_init_terminal();
      // printf("It is %d\n", pcXcoord);//////////////////////////////////
      move(0,0);
      clrtoeol();
      refresh();
      mvprintw(0, 0,"Your Turn!");
      refresh();
      printDisplay(characterTable, w[200][200]->grid);
      refresh();
      while(uInput != 'q'){
        Characters *peeked = peekCharacter(&cPQ);
        if(peeked->symbolOfCharacter == '@'){
          // printGrid(characterMovements);
          // usleep(250000);
          // io_init_terminal();
          // mvprintw(0, 0,"First map");
          // refresh();
          printDisplay(characterTable, w[200][200]->grid);
          refresh();
          uInput = getch();
          if(uInput == '5' || uInput == ' ' || uInput == '.'){
            move(0,0);
            clrtoeol();
            refresh();
            mvprintw(0, 0,"Your Turn!");
            refresh();
            int nextTurn = peeked->nextTurn;
            popCharacter(&cPQ);
            pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
          }
          else if(uInput == 'k' || uInput == '8'){//going up
            if(pcXcoord > 1 && costOfTerrain((pcXcoord) -1,pcYcoord,w[200][200]->grid,3) != INT_MAX){
              if(characterTable[pcXcoord-1][pcYcoord]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(characterTable[pcXcoord][pcYcoord]);
              characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord -1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&cPQ);
              pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
              characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
              }
              else if(characterTable[pcXcoord-1][pcYcoord]->battled==0){
                characterTable[pcXcoord-1][pcYcoord]->battled=1;
                WINDOW *newWindow = newwin(100,100,1,0);
                refresh();
                move(0,0);
                clrtoeol();
                wprintw(newWindow, "You are in a battle! \n");
                wrefresh(newWindow);
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  wrefresh(newWindow);
                  delwin(newWindow);
                }
              }
              else{
                move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              }
            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
            }
          }
          else if(uInput == 'y' || uInput == '7'){//going up and left
            if(pcXcoord > 1 && pcYcoord > 1 && costOfTerrain((pcXcoord) -1,(pcYcoord) - 1,w[200][200]->grid,3) != INT_MAX){
              if(characterTable[pcXcoord-1][pcYcoord-1] == NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(characterTable[pcXcoord][pcYcoord]);
              characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord -1;
              pcYcoord = pcYcoord -1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&cPQ);
              pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
              }
              else if(characterTable[pcXcoord-1][pcYcoord-1]->battled==0){
                characterTable[pcXcoord-1][pcYcoord-1]->battled=1;
                WINDOW *newWindow = newwin(100,100,1,0);
                refresh();
                move(0,0);
                clrtoeol();
                wprintw(newWindow, "You are in a battle! \n");
                wrefresh(newWindow);
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  wrefresh(newWindow);
                  delwin(newWindow);
                }
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
            }
          }
          else if(uInput == 'u' || uInput == '9'){//going up and right
            if(pcXcoord > 1 && pcYcoord < 78 && costOfTerrain((pcXcoord) -1,(pcYcoord) + 1,w[200][200]->grid,3) != INT_MAX){
              if(characterTable[pcXcoord-1][pcYcoord+1]== NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(characterTable[pcXcoord][pcYcoord]);
              characterTable[pcXcoord][pcYcoord]= NULL;
              pcXcoord = pcXcoord -1;
              pcYcoord = pcYcoord +1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&cPQ);
              pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord, 8, 0);
              dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
              }
              else if(characterTable[pcXcoord-1][pcYcoord+1]->battled==0){
                characterTable[pcXcoord-1][pcYcoord+1]->battled=1;
                WINDOW *newWindow = newwin(100,100,1,0);
                refresh();
                move(0,0);
                clrtoeol();
                wprintw(newWindow, "You are in a battle! \n");
                wrefresh(newWindow);
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  wrefresh(newWindow);
                  delwin(newWindow);
                }
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
            }
          }
          else if(uInput == 'n' || uInput == '3'){//going down and right
            if(pcXcoord < 20 && pcYcoord < 78 && costOfTerrain((pcXcoord) +1,(pcYcoord) + 1,w[200][200]->grid,3) != INT_MAX){
              if(characterTable[pcXcoord+1][pcYcoord+1]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(characterTable[pcXcoord][pcYcoord]);
              characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord +1;
              pcYcoord = pcYcoord +1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&cPQ);
              pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
              }
              else if(characterTable[pcXcoord+1][pcYcoord+1]->battled==0){
                characterTable[pcXcoord+1][pcYcoord+1]->battled=1;
                WINDOW *newWindow = newwin(100,100,1,0);
                refresh();
                move(0,0);
                clrtoeol();
                wprintw(newWindow, "You are in a battle! \n");
                wrefresh(newWindow);
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  wrefresh(newWindow);
                  delwin(newWindow);
                }
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
            }
          }
          else if(uInput == 'b' || uInput == '1'){//going down and left
            if(pcXcoord < 20 && pcYcoord > 1 && costOfTerrain((pcXcoord) +1,(pcYcoord) - 1,w[200][200]->grid,3) != INT_MAX){
              if(characterTable[pcXcoord+1][pcYcoord-1]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(characterTable[pcXcoord][pcYcoord]);
              characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord +1;
              pcYcoord = pcYcoord -1; 
              int nextTurn = peeked->nextTurn;
              popCharacter(&cPQ);
              pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8, 0);
              dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
              }
              else if(characterTable[pcXcoord+1][pcYcoord-1]->battled==0){
                characterTable[pcXcoord+1][pcYcoord-1]->battled=1;
                WINDOW *newWindow = newwin(100,100,1,0);
                refresh();
                move(0,0);
                clrtoeol();
                wprintw(newWindow, "You are in a battle! \n");
                wrefresh(newWindow);
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  wrefresh(newWindow);
                  delwin(newWindow);
                }
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
            }
          }
          else if(uInput == 'j' || uInput == '2'){//going down
            if(pcXcoord < 20 && costOfTerrain((pcXcoord) +1,pcYcoord,w[200][200]->grid,3) != INT_MAX){
              if(characterTable[pcXcoord+1][pcYcoord]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(characterTable[pcXcoord][pcYcoord]);
              characterTable[pcXcoord][pcYcoord] = NULL;
              pcXcoord = pcXcoord +1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&cPQ);
              pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
              }
              else if(characterTable[pcXcoord+1][pcYcoord]->battled==0){
                characterTable[pcXcoord+1][pcYcoord]->battled=1;
                WINDOW *newWindow = newwin(100,100,1,0);
                refresh();
                move(0,0);
                clrtoeol();
                wprintw(newWindow, "You are in a battle! \n");
                wrefresh(newWindow);
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  wrefresh(newWindow);
                  delwin(newWindow);
                }
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
            }
          }
          else if(uInput == '4' || uInput == 'h'){//going left
            if(pcYcoord > 1 && costOfTerrain((pcXcoord),(pcYcoord) -1,w[200][200]->grid,3) != INT_MAX){
              if(characterTable[pcXcoord][pcYcoord-1]==NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(characterTable[pcXcoord][pcYcoord]);
              characterTable[pcXcoord][pcYcoord] = NULL;
              pcYcoord = pcYcoord -1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&cPQ);
              pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord,8, 0);
              dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
              }
              else if(characterTable[pcXcoord][pcYcoord-1]->battled==0){
                characterTable[pcXcoord][pcYcoord-1]->battled=1;
                WINDOW *newWindow = newwin(100,100,1,0);
                refresh();
                move(0,0);
                clrtoeol();
                wprintw(newWindow, "You are in a battle! \n");
                wrefresh(newWindow);
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  wrefresh(newWindow);
                  delwin(newWindow);
                }
              }
              else{
                move(0,0);
                clrtoeol();
                refresh();
                mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
                refresh();
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
            }
          }
          else if(uInput == '6' || uInput == 'l'){//going right
            if(pcYcoord < 78 && costOfTerrain((pcXcoord),(pcYcoord) +1,w[200][200]->grid,3) != INT_MAX){
              if(characterTable[pcXcoord][pcYcoord+1] == NULL){
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Your Turn!");
              refresh();
              // characterMovements[pcXcoord][pcYcoord] = w[200][200]->grid[pcXcoord][pcYcoord];
              free(characterTable[pcXcoord][pcYcoord]);
              characterTable[pcXcoord][pcYcoord] = NULL;
              pcYcoord = pcYcoord +1;
              int nextTurn = peeked->nextTurn;
              popCharacter(&cPQ);
              pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
              // characterMovements[pcXcoord][pcYcoord] = 8;
              characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord, pcYcoord, 8, 0);
              dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
              dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
              }
              else if(characterTable[pcXcoord][pcYcoord+1]->battled==0){
                characterTable[pcXcoord][pcYcoord+1]->battled=1;
                WINDOW *newWindow = newwin(100,100,1,0);
                refresh();
                move(0,0);
                clrtoeol();
                wprintw(newWindow, "You are in a battle! \n");
                wrefresh(newWindow);
                while(uInput != 27){
                  uInput = getch();
                  if(uInput == 27){
                    break;
                  }
                }
                if(uInput == 27){
                  wrefresh(newWindow);
                  delwin(newWindow);
                }
              }
              else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"You have already defeted this trainer and is now in your way!");
              refresh();
              }

            }
            else{
              move(0,0);
              clrtoeol();
              refresh();
              mvprintw(0, 0,"Try another move!");
              refresh();
            }
          }
          else if(uInput == 't'){
            WINDOW *newWindow = newwin(100, 100, 1, 0);
            refresh();
            move(0,0);
            clrtoeol();
            refresh();
            wprintw(newWindow, "List of Trainers near you: \n");
            wrefresh(newWindow);
            for(int i = 0; i < 21; i++){
              for(int j =0; j< 80; j++){
                if(characterTable[i][j] != NULL && characterTable[i][j]->symbol != 8){//should print this on the trainer list
                  if(pcXcoord - i >= 0 && pcYcoord - j >= 0){
                    wprintw(newWindow, "%c, %d north %d west\n", typeOfNPC(characterTable[i][j]->symbol), pcXcoord - i, pcYcoord - j);
                    wrefresh(newWindow);
                  }
                  else if(pcXcoord - i <= 0 && pcYcoord - j >= 0){
                    wprintw(newWindow, "%c, %d south %d west\n", typeOfNPC(characterTable[i][j]->symbol), abs(pcXcoord - i), pcYcoord - j);
                    wrefresh(newWindow);
                  }
                  else if(pcXcoord - i <= 0 && pcYcoord - j <= 0){
                    wprintw(newWindow, "%c, %d south %d east\n", typeOfNPC(characterTable[i][j]->symbol), abs(pcXcoord - i), abs(pcYcoord - j));
                    wrefresh(newWindow);
                  }
                  else if(pcXcoord - i >= 0 && pcYcoord - j <= 0){
                    wprintw(newWindow, "%c, %d north %d east\n", typeOfNPC(characterTable[i][j]->symbol), pcXcoord - i, abs(pcYcoord - j));
                    wrefresh(newWindow);
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
               wrefresh(newWindow);
               delwin(newWindow);
            }
            
          }
          else if(uInput == '>'){
            if(w[200][200]->grid[pcXcoord][pcYcoord] == 3){//Pokemart
              WINDOW *newWindow = newwin(100,100,1,0);
              refresh();
              move(0,0);
              clrtoeol();
              wprintw(newWindow, "You have entered a pokemart!\n");
              wrefresh(newWindow);
              while(uInput != '<'){
                uInput = getch();
                if(uInput == '<'){
                  break;
                }
              }
              if(uInput == '<'){
                wrefresh(newWindow);
                delwin(newWindow);
              }
            }
            else if(w[200][200]->grid[pcXcoord][pcYcoord] == 4){//Pokecenter
              WINDOW *newWindow = newwin(100,100,1,0);
              refresh();
              move(0,0);
              clrtoeol();
              wprintw(newWindow, "You have entered a pokecenter!\n");
              wrefresh(newWindow);
              while(uInput != '<'){
                uInput = getch();
                if(uInput == '<'){
                  break;
                }
              }
              if(uInput == '<'){
                wrefresh(newWindow);
                delwin(newWindow);
              }
            }

          }
          else if(uInput == 'q'){
            continue;
          }
          else{
            continue;
          }
        }
        if(peeked->symbolOfCharacter == 'r'){
          moveRival(w[200][200]->grid, characterTable, distanceMapForRider, &cPQ);
        }
        if(peeked->symbolOfCharacter == 'h'){
          moveHiker(w[200][200]->grid, characterTable, distanceMapForHiker, &cPQ);
        }
        if(peeked->symbolOfCharacter == 'p'){
          int direction = peeked->direction;
          movePacer(w[200][200]->grid, characterTable, &direction, &cPQ);
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
          moveExplorer(w[200][200]->grid, characterTable, &direction, &cPQ);
        }
        if(peeked->symbolOfCharacter == 'w'){
          int direction = peeked->direction;
          moveWander(w[200][200]->grid, characterTable, &direction, &cPQ);
        }
      }
       endwin();
    }
    else{//user input is there!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////dont neet this
      int numOfTrainers = atoi(argv[1]);
      pcXcoord = getPCX();
      pcYcoord = getPCY();
      characterTable[pcXcoord][pcYcoord] = newCharacterOnMap(pcXcoord,pcYcoord,8,0);
      int distanceMapForHiker[21][80] ={0};
      int distanceMapForRider[21][80] ={0};
      dijkstrasMethod(w[200][200]->grid, 0, distanceMapForHiker);
      dijkstrasMethod(w[200][200]->grid, 1, distanceMapForRider);
      char uInput = 'p';
      io_init_terminal();

      if(numOfTrainers == 1){//just hiker!
        placeCharacter(w[200][200]->grid, characterTable, 14, 1, -1, &cPQ);
        // printf("It is %d\n", pcXcoord);//////////////////////////////////
        mvprintw(0, 0,"Your Turn!");
        refresh();
        printDisplay(characterTable, w[200][200]->grid);
        refresh();
        while(uInput != 'q'){
          Characters *peeked = peekCharacter(&cPQ);
          if(peeked->symbolOfCharacter == '@'){
            printDisplay(characterTable, w[200][200]->grid);
            refresh();
            uInput = getch();
            if(uInput == 'a'){
              mvprintw(0, 0,"Your Turn!");
              refresh();
              int nextTurn = peeked->nextTurn;
              popCharacter(&cPQ);
              pushCharacter(&cPQ, pcXcoord, pcYcoord, nextTurn + costOfTerrain(pcXcoord,pcYcoord,w[200][200]->grid,3), 0, '@', -1);
            }
          }
          if(peeked->symbolOfCharacter == 'r'){
          moveRival(w[200][200]->grid, characterTable, distanceMapForRider, &cPQ);
          }
          if(peeked->symbolOfCharacter == 'h'){
            moveHiker(w[200][200]->grid, characterTable, distanceMapForHiker, &cPQ);
          }
          if(peeked->symbolOfCharacter == 'p'){
            int direction = peeked->direction;
            movePacer(w[200][200]->grid, characterTable, &direction, &cPQ);
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
            moveExplorer(w[200][200]->grid, characterTable, &direction, &cPQ);
          }
          if(peeked->symbolOfCharacter == 'w'){
            int direction = peeked->direction;
            moveWander(w[200][200]->grid, characterTable, &direction, &cPQ);
          }
        }
        endwin();
      }
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
void placeCharacter(int grid[21][80], Characters *characterTable[21][80], int characterNumber, int seqNum, int direction, Characters **head){
  int xCoord = ((rand() % 17) + 2);
  int yCoord = ((rand() % 76) + 2);
  //(1 || 2 || 7 || 8 || 3 || 4 || 10 || 11 || 12 || 13 || 14 )
  while(characterTable[xCoord][yCoord]!=NULL){
    xCoord = ((rand() % 17) + 2);
    yCoord = ((rand() % 76) + 2);
  }

  // characterGrid[xCoord][yCoord] = characterNumber;
  characterTable[xCoord][yCoord] = newCharacterOnMap(xCoord, yCoord, characterNumber, 0);
  

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
  }

  exitsUnreachable(distance);
  // printDistTable(distance);
}
