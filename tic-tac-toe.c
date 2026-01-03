#include<stdio.h>

void printGameBoard(char gameBoardProgress[3][3]);
void boardProgressUpdate(char gameBoardProgress[3][3], int playerSelectedTile, char playerMove);
void gameRunner();

void printGameBoard(char gameBoardProgress[3][3]){
    printf("-------------------------------------------------\n");

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            printf("|\t%c\t", gameBoardProgress[i][j]);
        }
        printf("|");
        printf("\n-------------------------------------------------");
        printf("\n");
    }
}

void boardProgressUpdate(char gameBoardProgress[3][3], int playerSelectedTile, char playerMove) {
    switch (playerSelectedTile)
    {
    case 11:
        gameBoardProgress[0][0] = playerMove;
        break;
    case 12:
        gameBoardProgress[0][1] = playerMove;
        break;
    case 13:
        gameBoardProgress[0][2] = playerMove;
        break;
    case 21:
        gameBoardProgress[1][0] = playerMove;
        break;
    case 22:
        gameBoardProgress[1][1] = playerMove;
        break;
    case 23:
        gameBoardProgress[1][2] = playerMove;
        break;
    case 31:
        gameBoardProgress[2][0] = playerMove;
        break;
    case 32:
        gameBoardProgress[2][1] = playerMove;
        break;
    case 33:
        gameBoardProgress[2][2] = playerMove;
        break;
    
    default:
        break;
    }
}

void gameRunner(){
    char marking = ' ';
    int tilemovement = 0;

    char boardProgress[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    while(1){
        printGameBoard(boardProgress);
        printf("Select Move X or O: ");
        scanf(" %c", &marking);
        printf("Select Tile to Mark: ");
        scanf("%d", &tilemovement);

        if(marking == 'X'){
            break;
        }

        boardProgressUpdate(boardProgress, tilemovement, marking);
    }
}

int main() {
    gameRunner();
    return 0;
}