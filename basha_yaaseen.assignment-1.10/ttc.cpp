#include <iostream>
#include <stdlib.h>
#include <ncurses.h>

char ticBoard [3][3] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};
int playerTurn = 1; 


void printBoard(char ticBoard[3][3]){
    for(int i = 0; i < 3; i++){
        for(int j=0; j< 3; j++){
            printf("%c ", ticBoard[i][j]);
        }
        printf("\n");
    }
}

void playerTurner(){
    int choice;
    int row,column;
    if(playerTurn == 1){
        printf("Player 1's turn! Enter a spot: \n");
    }
    else{
        printf("Player 2's turn! Enter a spot: \n");
    }
    scanf("%d", &choice);

    while(choice < 1 || choice > 9){
        printf("Wrong input, try again: \n");
        scanf("%d", &choice);
    }
    if(choice == 1){
        row = 0; column = 0;
    }
    else if(choice == 2){
        row = 0; column = 1;
    }
    else if(choice == 3){
        row = 0; column = 2;
    }
    else if(choice == 4){
        row = 1; column = 0;
    }
    else if(choice == 5){
        row = 1; column = 1;
    }
    else if(choice == 6){
        row = 1; column = 2;
    }
    else if(choice == 7){
        row =2; column = 0;
    }
    else if(choice == 8){
        row = 2; column = 1;
    }
    else if(choice == 9){
        row = 2; column = 2;
    }

    if(playerTurn == 1 && ticBoard[row][column] != 'x' && ticBoard[row][column] != 'o'){
        ticBoard[row][column] = 'x';
        playerTurn = 2;
    }
    else if(playerTurn == 2 && ticBoard[row][column] != 'x' && ticBoard[row][column] != 'o'){
        ticBoard[row][column] = 'o';
        playerTurn = 1;
    }
    else{
        printf("Spot is taken! Try again!");
        playerTurner();
    }
}

bool continueGame(){
    for(int i =0; i<3; i++){
        if((ticBoard[i][0] == ticBoard[i][1] && ticBoard[i][0] == ticBoard[i][2]) ||(ticBoard[0][i] == ticBoard[1][i] && ticBoard[0][i] == ticBoard[2][i])){
            return false;
        }
    }

    if((ticBoard[0][0] == ticBoard[1][1] && ticBoard[0][0] == ticBoard[2][2]) || (ticBoard[0][2] == ticBoard[1][1] && ticBoard[0][2] == ticBoard[2][0])){
        return false;
    }

    return true; 
}

int main(int argc, char *argv[]){
    printf("TIC TAC TOE GAME: PLAYER 1 X, PLAYER 2 O\n");
    bool drawGame = false; 
    int count = 0;
    while(continueGame() != false && drawGame != true){
        count = 0;
        printBoard(ticBoard);
        playerTurner();
        continueGame();
        for(int i = 0; i < 3; i++){
            for(int j = 0; j< 3; j++){
                if(ticBoard[i][j] != 'x' && ticBoard[i][j]!= 'o'){
                    count++;
                }
            }
        }
        if(count == 0){
            drawGame = true;
            break;
        }
    }
    if(drawGame == true){
        printBoard(ticBoard);
        printf("Game has drawn, better luck next time!\n");
    }
    else if(continueGame() == false && playerTurn == 2){
        printBoard(ticBoard);
        printf("Congrats, player 1 (x) has won!\n");
    }
    else{
        printBoard(ticBoard);
        printf("Congrats, player 2 (o) has won!\n");
    }

}