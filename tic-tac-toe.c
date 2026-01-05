#include<stdio.h>
#include<stdbool.h>
#include<ctype.h>

//Macros for text coloring
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//Function declarations
struct player;
void clearBuffer(void);
void gameIntroduction(void);
void gameRunner(void);
char playerMarkerSelection(int playerNumber);
void printGameBoard(char gameBoardProgress[3][3], int round);
void playerTurnChanger(struct player *one, struct player *two, char boardProgress[3][3]);
bool isTileOccupied(int tileSelection, char boardProgress[3][3]);
bool isCoordinateValid(int tileSelection);
bool playerMarkValidator(char xoMarking);
void boardProgressUpdate(char boardProgress[3][3], int playerSelectedTile, char playerMove);
bool gameStatus(char gameBoardProgress[3][3], struct player *one, struct player *two, int gameRound);


int main() {
    int menuSelection = 0;
    bool isSelectionValid = false;

    while (!isSelectionValid) {
        gameIntroduction();
        printf(ANSI_COLOR_CYAN "\n[SYSTEM]: SELECT FROM THE OPTION: " ANSI_COLOR_RESET);

        if (scanf("%d", &menuSelection) != 1) {
            // Clear invalid input
            clearBuffer();

            printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID INPUT! ENTER A NUMBER.\n" ANSI_COLOR_RESET);
            continue;
        }

        if (menuSelection <= 0 || menuSelection > 3) {
            printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID SELECTION!\n" ANSI_COLOR_RESET);
        } else {
            isSelectionValid = true;
        }
    }


    switch (menuSelection)
    {
    case 1:
        gameRunner();
        break;
    case 2:
        printf(ANSI_COLOR_RED "\n[SYSTEM]: FEATURE NOT IMPLEMENTED YET\n" ANSI_COLOR_RESET);
        break;
    case 3:
        printf(ANSI_COLOR_RED "\n[SYSTEM]: EXITING\n" ANSI_COLOR_RESET);
        return 0;
        break;
    
    default:
        printf(ANSI_COLOR_MAGENTA "\n[SYSTEM]: AN UNKNOWN ERROR HAS BEEN DETECTED. EXITING\n" ANSI_COLOR_RESET);
        return 0;
        break;
    }
    return 0;
}

void clearBuffer(void) {
    while ( getchar() != '\n' );
}

void gameIntroduction(void) {
    printf("   ___________.__         ___________               ___________ \n");
    printf("  \\__    ___/|__| ____   \\__    ___/____    ____   \\__    ___/___   ____  \n");
    printf("     |    |   |  |/ ___\\    |    |  \\__  \\ _/ ___\\    |    | /  _ \\_/ __ \\ \n");
    printf("     |    |   |  \\  \\___    |    |   / __ \\\\  \\___    |    |(  <_> )  ___/ \n");
    printf("     |____|   |__|\\___  >   |____|  (____  /\\___  >   |____| \\____/ \\___  >\n");
    printf("                      \\/                 \\/     \\/                      \\/ \n");

    printf(ANSI_COLOR_CYAN "\n[1]: PLAY" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "\n[2]: INSTRUCTION" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "\n[3]: QUIT" ANSI_COLOR_RESET);
}

//Player base structure
struct player
{
    int playerNumber;
    int score;
    char playerMarker;
    bool isTurn;
};


void gameRunner(void) {
    //Player Creation and Marking Selection
    struct player One;
    One.playerNumber = 1;
    One.playerMarker = playerMarkerSelection(One.playerNumber);
    One.isTurn = true;
    One.score = 0;
    struct player *ptrOne = &One;
    
    struct player Two;
    Two.playerNumber = 2;
    Two.playerMarker = (One.playerMarker == 'X') ? 'O' : 'X';
    Two.isTurn = false;
    Two.score = 0;
    struct player *ptrTwo = &Two;

    //Game Status
    int roundCount = 1;
    bool gameOver = false;
    bool endGame = false;
    char boardGame[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    while(!gameOver){
        printGameBoard(boardGame, roundCount);
        playerTurnChanger(ptrOne, ptrTwo, boardGame);
        gameOver = gameStatus(boardGame, ptrOne, ptrTwo, roundCount);
    }
}

void playerTurnChanger(struct player *one, struct player *two, char boardProgress[3][3]){
    int tileSelection;
    bool isTileSelectionValid = false;

    if(one->isTurn) {
        while(!isTileSelectionValid){
            printf(ANSI_COLOR_YELLOW "\n[PLAYER %d]: Select Tile to Mark: " ANSI_COLOR_RESET, one->playerNumber);
            
            if(scanf("%d", &tileSelection) != 1) {
                printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID INPUT! PLEASE ONLY ENTER THE TILE COORDINATE NUMBER\n" ANSI_COLOR_RESET);
                clearBuffer();
                continue;
            }

            if(!isCoordinateValid(tileSelection)) {
                printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID INPUT! PLEASE ONLY ENTER THE TILE COORDINATE\n" ANSI_COLOR_RESET);
                clearBuffer();
                continue;
            }

            if(!isTileOccupied(tileSelection, boardProgress)){
                boardProgressUpdate(boardProgress, tileSelection, one->playerMarker);
                one->isTurn = false;
                two->isTurn = true;
                isTileSelectionValid = true;
            }
            else {
                printf(ANSI_COLOR_RED "\n[SYSTEM]: TILE IS CURRENTLY OCCUPIED. SELECT OTHER TILE.\n" ANSI_COLOR_RESET);
            }
        }
    }
    else {
        while(!isTileSelectionValid){
            printf(ANSI_COLOR_YELLOW "\n[PLAYER %d]: Select Tile to Mark: " ANSI_COLOR_RESET, two->playerNumber);
            
            if(scanf("%d", &tileSelection) != 1) {
                printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID INPUT! PLEASE ONLY ENTER THE TILE COORDINATE NUMBER\n" ANSI_COLOR_RESET);
                clearBuffer();
                continue;
            }

            if(!isCoordinateValid(tileSelection)) {
                printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID INPUT! PLEASE ONLY ENTER THE TILE COORDINATE\n" ANSI_COLOR_RESET);
                clearBuffer();
                continue;
            }

            if(!isTileOccupied(tileSelection, boardProgress)){
                boardProgressUpdate(boardProgress, tileSelection, two->playerMarker);
                one->isTurn = true;
                two->isTurn = false;
                isTileSelectionValid = true;
            }
            else {
                printf(ANSI_COLOR_RED "\n[SYSTEM]: TILE IS CURRENTLY OCCUPIED. SELECT OTHER TILE.\n" ANSI_COLOR_RESET);
            }
        }
    }
}

bool isTileOccupied(int tileSelection, char boardProgress[3][3]){
    int row, column;

    //Computation for getting the row and column coordinates
    row = (tileSelection / 10) - 1;
    column = (tileSelection % 10) - 1;
    bool isOccupied = false;
    
    isOccupied = (boardProgress[row][column] != ' ') ? true : false; 

    return isOccupied;
}

bool isCoordinateValid(int tileSelection) {
    return (tileSelection == 11 || tileSelection == 12 || tileSelection == 13 ||
            tileSelection == 21 || tileSelection == 22 || tileSelection == 23 ||
            tileSelection == 31 || tileSelection == 32 || tileSelection == 33) ? true : false;
}

char playerMarkerSelection(int playerNumber){
    char selectedMarker;
    char upperCase;
    bool isSelectionValid = false;

    while(!isSelectionValid) {
        printf(ANSI_COLOR_CYAN "\n[PLAYER %d]: SELECT MARKER (X or O): " ANSI_COLOR_RESET, playerNumber);
        scanf(" %c", &selectedMarker);

        if(!playerMarkValidator(selectedMarker)){
            printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID SELECTION!\n" ANSI_COLOR_RESET);
            continue;
        }
        else {
            isSelectionValid = true;
        }
    }

    upperCase = toupper(selectedMarker);
    
    printf(ANSI_COLOR_MAGENTA "\n[SYSTEM]: PLAYER %d SELECTED %c\n" ANSI_COLOR_RESET, playerNumber, upperCase);

    return upperCase;
}

void printGameBoard(char gameBoardProgress[3][3], int round){
    printf(ANSI_COLOR_GREEN "---------------------ROUND %d---------------------\n" ANSI_COLOR_RESET, round);

    for(int row = 0; row < 3; row++){
        for(int column = 0; column < 3; column++){
            printf(ANSI_COLOR_GREEN "|\t%c\t" ANSI_COLOR_RESET, gameBoardProgress[row][column]);
        }
        printf(ANSI_COLOR_GREEN "|" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_GREEN "\n-------------------------------------------------" ANSI_COLOR_RESET);
        printf("\n");
    }
}

bool playerMarkValidator(char xoMarking){
    if(xoMarking == 'x' || xoMarking == 'X' || xoMarking == 'o' || xoMarking == 'O'){
        return true;
    }
    else{
        return false;
    }
}

void boardProgressUpdate(char gameBoardProgress[3][3], int playerSelectedTile, char playerMove) {
    int row, column;

    //Computation for getting the row and column coordinates
    row = (playerSelectedTile / 10) - 1;
    column = (playerSelectedTile % 10) - 1;

    gameBoardProgress[row][column] = playerMove;
}

bool gameStatus(char gameBoardProgress[3][3], struct player *one, struct player *two, int gameRound) {
    int i;
    int victoriousPlayer;
    char winningMarker;

    // Check rows
    for (i = 0; i < 3; i++) {
        if (gameBoardProgress[i][0] != ' ' &&
            gameBoardProgress[i][0] == gameBoardProgress[i][1] &&
            gameBoardProgress[i][1] == gameBoardProgress[i][2]) {

            victoriousPlayer =
                (gameBoardProgress[i][0] == one->playerMarker)
                ? one->playerNumber
                : two->playerNumber;

            winningMarker =
                (victoriousPlayer == one->playerNumber)
                ? one->playerMarker
                : two->playerMarker;

            printf(ANSI_COLOR_YELLOW
                   "\n[SYSTEM]: A LINEAR %c HAS BEEN DETECTED\n"
                   ANSI_COLOR_RESET, winningMarker);
            printf(ANSI_COLOR_YELLOW
                   "\n[SYSTEM]: ROUND %d WINNER IS PLAYER %d\n"
                   ANSI_COLOR_RESET, gameRound, victoriousPlayer);
            return true;
        }
    }

    // Check columns
    for (i = 0; i < 3; i++) {
        if (gameBoardProgress[0][i] != ' ' &&
            gameBoardProgress[0][i] == gameBoardProgress[1][i] &&
            gameBoardProgress[1][i] == gameBoardProgress[2][i]) {

            victoriousPlayer =
                (gameBoardProgress[0][i] == one->playerMarker)
                ? one->playerNumber
                : two->playerNumber;

            winningMarker =
                (victoriousPlayer == one->playerNumber)
                ? one->playerMarker
                : two->playerMarker;

            printf(ANSI_COLOR_YELLOW
                   "\n[SYSTEM]: A LINEAR %c HAS BEEN DETECTED\n"
                   ANSI_COLOR_RESET, winningMarker);
            printf(ANSI_COLOR_YELLOW
                   "\n[SYSTEM]: ROUND %d WINNER IS PLAYER %d\n"
                   ANSI_COLOR_RESET, gameRound, victoriousPlayer);
            return true;
        }
    }

    // Check diagonals
    if (gameBoardProgress[0][0] != ' ' &&
        gameBoardProgress[0][0] == gameBoardProgress[1][1] &&
        gameBoardProgress[1][1] == gameBoardProgress[2][2]) {

        victoriousPlayer =
            (gameBoardProgress[0][0] == one->playerMarker)
            ? one->playerNumber
            : two->playerNumber;

        winningMarker =
            (victoriousPlayer == one->playerNumber)
            ? one->playerMarker
            : two->playerMarker;

        printf(ANSI_COLOR_YELLOW
               "\n[SYSTEM]: A LINEAR %c HAS BEEN DETECTED\n"
               ANSI_COLOR_RESET, winningMarker);
        printf(ANSI_COLOR_YELLOW
                "\n[SYSTEM]: ROUND %d WINNER IS PLAYER %d\n"
                ANSI_COLOR_RESET, gameRound, victoriousPlayer);
        return true;
    }

    if (gameBoardProgress[0][2] != ' ' &&
        gameBoardProgress[0][2] == gameBoardProgress[1][1] &&
        gameBoardProgress[1][1] == gameBoardProgress[2][0]) {

        victoriousPlayer =
            (gameBoardProgress[0][2] == one->playerMarker)
            ? one->playerNumber
            : two->playerNumber;

        winningMarker =
            (victoriousPlayer == one->playerNumber)
            ? one->playerMarker
            : two->playerMarker;

        printf(ANSI_COLOR_YELLOW
               "\n[SYSTEM]: A LINEAR %c HAS BEEN DETECTED\n"
               ANSI_COLOR_RESET, winningMarker);
        printf(ANSI_COLOR_YELLOW
                "\n[SYSTEM]: ROUND %d WINNER IS PLAYER %d\n"
                ANSI_COLOR_RESET, gameRound, victoriousPlayer);
        return true;
    }

    return false;
}