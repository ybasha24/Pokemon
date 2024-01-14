#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//1 in the grid is %
//2 in the grid is #
//3 in the grid is M
//4 in the grid is C
//5 in the grid is :
//6 in the grid is . 
void printGrid(int grid [21][80]);
int downOrLeft();
int downOrRight();
int rightOrUp();
int rightOrDown();
void addPokeMart(int grid[21][80], int row, int column);
void addPokeCenter(int grid[21][80], int row, int column);
void growSeed(int grid[21][80], int row, int column, int op);


int main(int argc, char *argv[]){
  int intersectionXPoint;
  int intersectionYPoint;
  int grid [21][80] = {0};
  srand(time(NULL));
  int northExit = (rand() % 76) + 2;
  int southExit = (rand() % 76) + 2;
  int westExit = (rand() % 17) + 2;
  int eastExit = (rand() % 17) + 2;
  int pokeMart = (rand() % 17) + 2;
  int pokeCenter = (rand() % 76) + 2;
  int pokeMartColumn;
  int pokeCenterRow;
  for(int j=0; j<80; j++){
    grid[0][j] = 1;
  }
  for(int j=0; j<80; j++){
    grid[20][j] = 1;
  }
  for(int j=0; j<21; j++){
    grid[j][0] = 1;
  }
  for(int j=0; j<21; j++){
    grid[j][79] = 1;
  }
  grid[0][northExit] = 2;
  grid[20][southExit] = 2;
  grid[westExit][0] = 2;
  grid[eastExit][79] = 2;

  //CREATING THE N-S PATH:
  if(northExit > southExit){//have to go down and left
    grid[1][northExit] = 2;
    int numberOfLefts = northExit - southExit;
    int counter = 0; 
    int number = northExit;
    int i = 2;
      while((counter != numberOfLefts) && i != 18){
        int choice = downOrLeft();
        if(choice == 0){//go down
        if(i == pokeMart){
          pokeMartColumn = number;
        }
          grid[i][number] = 2;
          i++;
        }
        else{//go left
        if(i == pokeMart){
          pokeMartColumn = number;
        }
          grid[i][number]=2;
          number--;
          counter++;
        }
      }

      if((counter == numberOfLefts) && i !=18){
        for(int j = i; j<19; j++){
          if(j == pokeMart){
          pokeMartColumn = number;
        }
          grid[j][number] = 2;
        }
      }
      if((counter != numberOfLefts) && i == 18){
        while(counter!=numberOfLefts){
          if(i == pokeMart){
          pokeMartColumn = number;
        }
          grid[i][number] = 2;
          number--;
          counter++;
        }
        grid[i][number]=2;
      }
    grid[19][southExit] = 2;
  }
   else{//have to go down and right
    grid[1][northExit] = 2;
    int numberOfRights = southExit - northExit;
    int counter = 0; 
    int number = northExit;
    int i = 2;
      while((counter != numberOfRights) && i != 18){
        int choice = downOrRight();
        if(choice == 0){//go down
        if(i == pokeMart){
          pokeMartColumn = number;
        }
          grid[i][number] = 2;
          i++;
        }
        else{//go right
        if(i == pokeMart){
          pokeMartColumn = number;
        }
          grid[i][number]=2;
          number++;
          counter++;
        }
      }

      if((counter == numberOfRights) && i !=18){
        for(int j = i; j<19; j++){
          if(j == pokeMart){
          pokeMartColumn = number;
        }
          grid[j][number] = 2;
        }
      }
      if((counter != numberOfRights) && i == 18){
        while(counter!=numberOfRights){
          if(i == pokeMart){
          pokeMartColumn = number;
        }
          grid[i][number] = 2;
          number++;
          counter++;
        }
        grid[i][number] =2;
      }
    grid[19][southExit] = 2;
   }

addPokeMart(grid, pokeMart, pokeMartColumn);

//CREATING THE W-E PATH:

if(westExit > eastExit){//have to go right and up
    grid[westExit][1] = 2;
    int numberOfUps = westExit - eastExit;
    int counter = 0; 
    int number = westExit;
    int i = 2;
      while((counter != numberOfUps) && i != 77){
        int choice = rightOrUp();
        if(choice == 0){//go right
          if(grid[number][i] == 2){
            intersectionXPoint = number;
            intersectionYPoint = i;
          }
          if(i == pokeCenter){
            pokeCenterRow = number;
          }
          grid[number][i] = 2;
          i++;
        }
        else{//go up
        if(grid[number][i] == 2){
            intersectionXPoint = number;
            intersectionYPoint = i;
          }
          if(i == pokeCenter){
            pokeCenterRow = number;
          }
          grid[number][i]=2;
          number--;
          counter++;
        }
      }

      if((counter == numberOfUps) && i !=77){
        for(int j = i; j<78; j++){
          if(grid[number][j] == 2){
            intersectionXPoint = number;
            intersectionYPoint = j;
          }
          if(j == pokeCenter){
            pokeCenterRow = number;
          }
          grid[number][j] = 2;
        }
      }
      if((counter != numberOfUps) && i == 77){
        while(counter!=numberOfUps){
          if(grid[number][i] == 2){
            intersectionXPoint = number;
            intersectionYPoint = i;
          }
          if(i == pokeCenter){
            pokeCenterRow = number;
          }
          grid[number][i] = 2;
          number++;
          counter++;
        }
        grid[number][i]=2;
      }
    grid[eastExit][78] = 2;
  }
   else{//have to go right and down
    grid[westExit][1] = 2;
    int numberOfDowns = eastExit - westExit;
    int counter = 0; 
    int number = westExit;
    int i = 2;
      while((counter != numberOfDowns) && i != 77){
        int choice = rightOrDown();
        if(choice == 0){//go right
        if(grid[number][i] == 2){
            intersectionXPoint = number;
            intersectionYPoint = i;
          }
          if(i == pokeCenter){
            pokeCenterRow = number;
          }
          grid[number][i] = 2;
          i++;
        }
        else{//go down
        if(grid[number][i] == 2){
            intersectionXPoint = number;
            intersectionYPoint = i;
          }
          if(i == pokeCenter){
            pokeCenterRow = number;
          }
          grid[number][i]=2;
          number++;
          counter++;
        }
      }

      if((counter == numberOfDowns) && i !=77){
        for(int j = i; j<78; j++){
          if(grid[number][j] == 2){
            intersectionXPoint = number;
            intersectionYPoint = j;
          }
          if(j == pokeCenter){
            pokeCenterRow = number;
          }
          grid[number][j] = 2;
        }
      }
      if((counter != numberOfDowns) && i == 77){
        while(counter!=numberOfDowns){
          if(grid[number][i] == 2){
            intersectionXPoint = number;
            intersectionYPoint = i;
          }
          if(i == pokeCenter){
            pokeCenterRow = number;
          }
          grid[number][i] = 2;
          number++;
          counter++;
        }
        grid[number][i]=2;
      }
    grid[eastExit][78] = 2;
   }

addPokeCenter(grid, pokeCenterRow, pokeCenter);
//adding the regions of tall grass and clearings!
int quad1or2 = (rand() % 2) + 1;
if(quad1or2 == 1){
  growSeed(grid, 1, 78, 5);
  growSeed(grid, 1, 1, 6);
}
else{
  growSeed(grid, 1, 1, 5);
  growSeed(grid, 1, 78, 6);
}

int quad3or4 = (rand() % 2) + 1;
if(quad3or4 == 1){
  growSeed(grid, 19, 1, 5);
  growSeed(grid, 19, 78, 6);
}
else{
  growSeed(grid, 19, 1, 6);
  growSeed(grid, 19, 78, 5);
}

int trees = 0;
while(trees!=24){
  int treeX = (rand() % 17) + 2;
  int treeY = (rand() % 76) + 2;
  if(grid[treeX][treeY] == 2 || grid[treeX][treeY] == 3 || grid[treeX][treeY] == 4){
    continue;
  }
  else{
    grid[treeX][treeY] = 7;
    trees++;
  }
}

int boulders = 0;
while(boulders!=24){
  int boulderX = (rand() % 17) + 2;
  int boulderY = (rand() % 76) + 2;
  if(grid[boulderX][boulderY] == 2 || grid[boulderX][boulderY] == 3 || grid[boulderX][boulderY] == 4){
    continue;
  }
  else{
    grid[boulderX][boulderY] = 1;
    boulders++;
  }
}

printGrid(grid);
printf("\n");
return 0;
}


void addPokeMart(int grid[21][80], int row, int column){ //N-S path addPokeMart(grid, pokeMart, pokeMartColumn);
    int count = 0;
     while(count!=1){

      if((grid[row][column + 1] == 0)){
        grid[row][column+1] = 3;
        count++;
        break;
      }
      else if((grid[row-1][column]==0)){
          grid[row-1][column] = 3;
          count++;
          break;
      }
      else if((grid[row + 1][column] ==0)){
        grid[row + 1][column] =3;
        count++;
        break;
      }
      else if((grid[row][column-1]==0)){
        grid[row][column-1]=3;
        count++;
        break;
      }
      else if((grid[row+1][column - 1] == 0)){
        grid[row+1][column - 1] =3;
        count++;
        break;
      }
      else if((grid[row+1][column+1]==0)){
        grid[row+1][column+1]=3;
        count++;
        break;
      }
      else if((grid[row - 1][column-1] ==0)){
        grid[row - 1][column-1] =3;
        count++;
        break;
      }
      else if((grid[row-1][column+1]==0)){
        grid[row-1][column+1]=3;
        count++;
        break;
      }
  
}
}

void addPokeCenter(int grid[21][80], int row, int column){//w-e path addPokeCenter(grid, pokeCenterRow, pokeCenter);
  int count = 0;
     while(count!=1){
      if((grid[row][column + 1] == 0)){
        grid[row][column+1] = 4;
        count++;
        break;
      }
      else if((grid[row-1][column]==0)){
          grid[row-1][column] = 4;
          count++;
          break;
      }
      else if((grid[row + 1][column] ==0)){
        grid[row + 1][column] =4;
        count++;
        break;
      }
      else if((grid[row][column-1]==0)){
        grid[row][column-1]=4;
        count++;
        break;
      }
      else if((grid[row+1][column - 1] == 0)){
        grid[row+1][column - 1] =4;
        count++;
        break;
      }
      else if((grid[row+1][column+1]==0)){
        grid[row+1][column+1]=4;
        count++;
        break;
      }
      else if((grid[row - 1][column-1] ==0)){
        grid[row - 1][column-1] =4;
        count++;
        break;
      }
      else if((grid[row-1][column+1]==0)){
        grid[row-1][column+1]=4;
        count++;
        break;
      }

}
}

void printGrid(int grid [21][80]){
  for(int i = 0; i < 21; i++){
    for(int j =0; j< 80; j++){
      if(grid[i][j] == 1){
	      printf("%%");
      }
      else if (grid[i][j] == 2)
      {
        printf("#");
      }
      else if(grid[i][j]==3){
        printf("M");
      }
      else if(grid[i][j]==4){
        printf("C");
      }
      else if(grid[i][j]==5){
        printf(":");
      }
      else if(grid[i][j]==6){
        printf(".");
      }
      else if(grid[i][j]==7){
        printf("^");
      }
      else{
        printf("^");
      }
     }
    printf("\n");
  }
}

int downOrRight(){
  int returnValue = rand() % 2;
  return returnValue;
}

int downOrLeft(){
  int returnValue = rand() % 2;
  return returnValue;
}

int rightOrUp(){
  int returnValue = rand() % 2;
  return returnValue;
}

int rightOrDown(){
  int returnValue = rand() % 2;
  return returnValue;
}

void growSeed(int grid[21][80], int row, int column, int op){
   if(row<0 || row>20){
    return;
  }
  if(column<0 || column>79){
    return;
  }
  if(grid[row][column] != 0) {
    return;
  }
  //if grid[row][column] == 0
  grid[row][column] =op;
  growSeed(grid, row, column-1, op);
  growSeed(grid, row, column+1, op);
  growSeed(grid, row+1, column, op);
  growSeed(grid, row-1, column, op);
  growSeed(grid, row+1, column+1, op);
  growSeed(grid, row-1, column-1, op);
  growSeed(grid, row+1, column-1, op);
  growSeed(grid, row-1, column+1, op);
}
