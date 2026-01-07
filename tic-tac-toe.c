#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<ctype.h>

//Macros for text coloring
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define BOARD_SIZE 3
#define EMPTY ' '

//Optimizations: Function Declarations
struct player;
int askTileSelection(struct player current, char boardProgress[BOARD_SIZE][BOARD_SIZE]);
char selectMarker(int playerId);
void clearBuffer(void);
void gameIntroduction(void);
void gameRunner(void);
void boardInitializer(char board[BOARD_SIZE][BOARD_SIZE]);
void displayBoard(char boardProgress[BOARD_SIZE][BOARD_SIZE], int round);
void applyMove(int tileCoordinate, struct player current ,char boardProgress[BOARD_SIZE][BOARD_SIZE]);
void showPlayerScores(struct player p1, struct player p2);
bool isTileCoordinateValid(int tileCoordinate);
bool isTileCoodinateNotOccupied(int tileCoordinate, char boardProgress[BOARD_SIZE][BOARD_SIZE]);
bool checkWinner(char boardProgress[BOARD_SIZE][BOARD_SIZE], char winningMarker);
bool checkDraw(char boardProgress[BOARD_SIZE][BOARD_SIZE]);


//Optimizations: Function Implementations
int main() {
    int menuSelection;
    char input[32];

    while (1) {
        gameIntroduction();
        printf(ANSI_COLOR_CYAN "\n[SYSTEM]: SELECT FROM THE OPTION: " ANSI_COLOR_RESET);

        if (!fgets(input, sizeof(input), stdin)) {
            printf(ANSI_COLOR_RED "\n[SYSTEM]: INPUT ERROR.\n" ANSI_COLOR_RESET);
            continue;
        }

        char *endptr;
        menuSelection = strtol(input, &endptr, 10);

        if (endptr == input || *endptr != '\n') {
            printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID INPUT! ENTER A NUMBER.\n" ANSI_COLOR_RESET);
            continue;
        }

        if (menuSelection < 1 || menuSelection > 3) {
            printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID SELECTION!\n" ANSI_COLOR_RESET);
            continue;
        }

        break;
    }

    switch (menuSelection) {
    case 1:
        gameRunner();
        break;
    case 2:
        printf(ANSI_COLOR_RED "\n[SYSTEM]: EXITING\n" ANSI_COLOR_RESET);
        break;
    
    default:
        printf(ANSI_COLOR_MAGENTA "\n[SYSTEM]: AN UNKNOWN ERROR HAS BEEN DETECTED. EXITING\n" ANSI_COLOR_RESET);
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

    printf(ANSI_COLOR_CYAN "[1]: PLAY" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "\n[2]: QUIT" ANSI_COLOR_RESET);
}

//Player base structure
struct player
{
    int playerId;
    int score;
    char playerMarker;
};

void gameRunner(void) {
    struct player One = {1, 0, selectMarker(1)};
    struct player Two = {2, 0, (One.playerMarker == 'X') ? 'O' : 'X'};
    struct player *currentPlayer;

    int round = 1;
    char boardState[BOARD_SIZE][BOARD_SIZE];
    bool playAgain = true;

    //Game loop running as long as players want to play again
    while(playAgain) {
        currentPlayer = &One;
        boardInitializer(boardState);

        //Game ROUND loop running as long as there's no declared winner or game draw 
        while(true) {
            int tileSelection;

            displayBoard(boardState, round);
            tileSelection = askTileSelection(*currentPlayer, boardState);
            applyMove(tileSelection, *currentPlayer, boardState);
            
            if(checkWinner(boardState, currentPlayer->playerMarker)) {
                printf(ANSI_COLOR_YELLOW "\nA WINNING MATCH HAS BEEN DETECTED!" ANSI_COLOR_RESET);
                printf(ANSI_COLOR_YELLOW "\nROUND %d WINNER IS PLAYER %d\n" ANSI_COLOR_RESET, round, currentPlayer->playerId);
                currentPlayer->score++;
                break;
            }

            if(checkDraw(boardState)){
                printf(ANSI_COLOR_YELLOW "\nROUND %d HAS ENDED IN A DRAW!" ANSI_COLOR_RESET, round);
                break;
            }

            currentPlayer = (currentPlayer == &One) ? &Two : &One;
        }

        showPlayerScores(One, Two);

        char choice;
        char finalChoiceOutput;

        while(true) {
            printf(ANSI_COLOR_CYAN "\nPlay another round? (Y/N): " ANSI_COLOR_RESET);
            scanf(" %c", &choice);

            finalChoiceOutput = toupper(choice);
            
            if(finalChoiceOutput != 'Y' && finalChoiceOutput != 'N') {
                printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID SELECTION! \'Y\' FOR YES  or \'N\' FOR NO ONLY.\n" ANSI_COLOR_RESET);
                clearBuffer();
            }
            else {
                break;
            }
            
        }
        playAgain = (finalChoiceOutput == 'Y');
        round++;
    }

    printf(ANSI_COLOR_CYAN "\nTHANK YOU FOR PLAYING!" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED "\n[SYSTEM]: EXITING\n" ANSI_COLOR_RESET);
}

void boardInitializer(char board[BOARD_SIZE][BOARD_SIZE]) {
    for(unsigned int i = 0; i < BOARD_SIZE; i++) {
        for(unsigned int j = 0; j < BOARD_SIZE; j++){
            board[i][j] = EMPTY;
        }
    }
}

void displayBoard(char boardProgress[BOARD_SIZE][BOARD_SIZE], int round){
    printf(ANSI_COLOR_GREEN "---------------------ROUND %d---------------------\n" ANSI_COLOR_RESET, round);

    for(int row = 0; row < 3; row++){
        for(int column = 0; column < 3; column++){
            printf(ANSI_COLOR_GREEN "|\t%c\t" ANSI_COLOR_RESET, boardProgress[row][column]);
        }
        printf(ANSI_COLOR_GREEN "|" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_GREEN "\n-------------------------------------------------" ANSI_COLOR_RESET);
        printf("\n");
    }
}

char selectMarker(int playerId) {
    char marker;

    do {
        printf(ANSI_COLOR_CYAN "\n[PLAYER %d] Choose X or O: " ANSI_COLOR_RESET, playerId);
        scanf(" %c", &marker);
        marker = toupper(marker);

        if(marker != 'X' && marker != 'O') {
            printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID SELECTION!\n" ANSI_COLOR_RESET);
            clearBuffer();
        }
    } while(marker != 'X' && marker != 'O');

    printf(ANSI_COLOR_MAGENTA "\n[SYSTEM]: PLAYER %d SELECTED %c\n" ANSI_COLOR_RESET, playerId, marker);
    
    return marker;
}

int askTileSelection(struct player current, char boardProgress[BOARD_SIZE][BOARD_SIZE]) {
    int selectedTile;

    while(true) {
        printf(ANSI_COLOR_YELLOW "\n[PLAYER %d]: Select Tile to Mark: " ANSI_COLOR_RESET, current.playerId);
        
        if(scanf("%d", &selectedTile) != 1) {
            printf(ANSI_COLOR_RED "\n[SYSTEM]: INVALID INPUT! PLEASE ONLY ENTER THE TILE COORDINATE NUMBER\n" ANSI_COLOR_RESET);
            clearBuffer();
            continue;
        }

        /*
        If tile selection is a valid number
        check if the tile coordinate and tile occupancy is valid 
        and is possible to apply the move in order to
        update board progress
        */
        if(isTileCoordinateValid(selectedTile) && isTileCoodinateNotOccupied(selectedTile, boardProgress)) {
            return selectedTile;
        }

        printf(ANSI_COLOR_RED "\n[SYSTEM]: THE SELECTED TILE COORDINATE IS EITHER NOT A VALID ROW/COLUMN RANGE OR IS OCCUPIED\n" ANSI_COLOR_RESET);
    }
}

bool isTileCoordinateValid(int tileCoordinate) {
    return (tileCoordinate >= 11 && tileCoordinate <= 33 &&             //This expression tells whether the tile coordinate selection is in the valid tile range
            tileCoordinate / 10 >= 1 && tileCoordinate / 10 <= 3 &&     //This expression gives the row (x) coordinate and tells whether the result is in the valid row range
            tileCoordinate % 10 >= 1 && tileCoordinate % 10 <= 3        //This expression gives the column (y) coordinate and tells whether the result is in the valid column range
            );
}

bool isTileCoodinateNotOccupied(int tileCoordinate, char boardProgress[BOARD_SIZE][BOARD_SIZE]) {
    /*
    The expressions below give the row and column coordinate 
    via division (remainder truncated) and modulo (remainder) 
    and correct the coordinates by subtracting the results by 1 to match the zero-based indexing in arrays
    */
    int row = (tileCoordinate / 10) - 1;
    int column = (tileCoordinate % 10) - 1;

    return boardProgress[row][column] == EMPTY;
}

void applyMove(int tileCoordinate, struct player current ,char boardProgress[BOARD_SIZE][BOARD_SIZE]) {
    int row = (tileCoordinate / 10) - 1;
    int column = (tileCoordinate % 10) - 1;

    boardProgress[row][column] = current.playerMarker;
}

bool checkWinner(char boardProgress[BOARD_SIZE][BOARD_SIZE], char winningMarker) {

    for(int i = 0; i < BOARD_SIZE; i++) {
        //Check for a winning match horizontally
        if(boardProgress[i][0] == winningMarker && 
           boardProgress[i][1] == winningMarker &&
           boardProgress[i][2] == winningMarker) return true;
        
        //Check for a winning match vertically
        if(boardProgress[0][i] == winningMarker && 
           boardProgress[1][i] == winningMarker &&
           boardProgress[2][i] == winningMarker) return true;
    }

    //Check for a winning match diagonally
    return (boardProgress[0][0] == winningMarker && boardProgress[1][1] == winningMarker && boardProgress[2][2] == winningMarker) ||
           (boardProgress[0][2] == winningMarker && boardProgress[1][1] == winningMarker && boardProgress[2][0] == winningMarker);
}

bool checkDraw(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] == EMPTY)
                return false;
    return true;
}

void showPlayerScores(struct player p1, struct player p2) {
    printf(ANSI_COLOR_MAGENTA "\n[SYSTEM]: PLAYER %d\'s CURRENT SCORE: %d" ANSI_COLOR_RESET, p1.playerId, p1.score);
    printf(ANSI_COLOR_MAGENTA "\n[SYSTEM]: PLAYER %d\'s CURRENT SCORE: %d\n" ANSI_COLOR_RESET, p2.playerId, p2.score);
}