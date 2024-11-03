
#include <commands.h>
#include <snake.h>
#include <syscalls.h>
#include <library.h>
// QUE FUNCIONES FALTAN CORREGIR Y HACER 
// keyInput
// spawnSnake
// BEEP -> SONIDOOOO
// CHEQUEARRRRRRRRRRRR  linea 320, linea 120
#define MAX 30
#define CERO_ASCII '0'
#define PLAYER_2 2
// https://hexdocs.pm/color_palette/ansi_color_codes.html
//typedef enum Colors {  colorsEnum.GREEN = 1, GREEN=1, RED, YELLOW, BLUE };
//#define COLORS 5

// --------------------- chequear
// chequear //pixel positions
#define Y_MAX 768
#define X_MAX 1024
#define PIXEL_POS_X(i) (i) * SQUARE_SIZE
#define PIXEL_POS_Y(i) (i + 1) * SQUARE_SIZE

//board positions
#define SQUARE_SIZE 32
#define X_SQUARES (X_MAX / SQUARE_SIZE) // 32
#define Y_SQUARES 23 // 24
//------------------------------
// -------------- FUCIONES ---------------
// -------------- GETTERS ---------------

static int getc1();
static int getc2(int c1);


// ----------- PERSONALIZAR SNAKE --------------

static void copyColor(snakeStruct * s, int num);
static void chooseColors(snakeStruct * s1, snakeStruct * s2, int * flag);
static void checkstartSnake(int * flag);
static void spawnSnake(snakeStruct * s1, snakeStruct * s2, int *flag);
// -------------- APPLE -------------

static appleStruct * apple;
static void putApple();
static void get_apple();


// ------------- SNAKE MOVEMENT ---------------

static int checkSnake(struct snakeStruct * s);
static int snakeEnDir(struct snakeStruct * s1, lastMoveEnum move);
static void winner(int *flag,int * w1, int *w2,struct snakeStruct * s1,struct snakeStruct * s2 );
static void game(struct snakeStruct * s1, struct snakeStruct * s2, int * flag);
static void keyPlayerOne(snakeStruct *s, int *w1);
static void keyPlayerTwo(snakeStruct * s1,snakeStruct * s2, int *w2);
static void deleteTail(struct snakeStruct * s);

// ----------- BOARD ------------------
#define APPLE_GREEN 0x87af00

static void board();
static void tablero(int * flag, int start);





// ----------  MAIN SNAKE --------------


void snake(){
    start(); 
    sys_clearScreen(); // finish = 1 y sale de start();
    return;
}

// ----------------- GETTERS -------------------

static int getc1(){
    char c1;
    while (c1 != '1' && c1 != '2' && c1 != '3' && c1 != '4') {
        c1 = getChar();
    }
}

static int getc2(int c1){
    char c2;
    do {
        c2 = getc1();
    } while (c2 == c1);

    return c2 - CERO_ASCII;
}

// ----------------- WELCOME SNAKE ---------------------

int welcomeSnake(){
    printf("Welcome to Snake\n");
    printf("Number of players: 1 o 2");
    char game;
    while (game != '1' && game != '2') {
        game = getChar();
    }
    return game - CERO_ASCII; // devuelve en numero en int
}

// ----------------------- MAKE SNAKE --------------------------------

#define P1 1
#define P2 2

// crea la snake y la posiciona en una zona del tablero
// el cual seria p1 medio a la izq
// p2 medio a la derecha

#define MIN_DIM 5
#define P1 '$'
#define P2 '*'


static void spawnSnake(snakeStruct *s, snakeStruct *s2, int *player) {
    // Inicialización para el Jugador 1
    if (player == 1) {
        s->id = P1;
        s->head.x = MIN_DIM- 1;
        s->head.y = Y_SQUARES / 2 + 1;
        for (int i = 0; i <= s->head.x; i++) { // La cola (x, y) es el primer elemento del array "body".
            s->body[i].x = i;
            s->body[i].y = s->head.y;
            boardMatrix[s->head.y][i] = s->id;
        }
        s->bodyDim = MIN_DIM;
        s->lastMove = RIGHT;
        s->points = 0;
        boardMatrix[s->head.y][s->head.x] = s->id;
        return;
    }

    // Inicialización para el Jugador 2
    s2->id = P2;
    s2->head.x = X_SQUARES - MIN_DIM;
    s2->head.y = Y_SQUARES / 2 - 1;
    // Primer bucle para inicializar la posición x de cada segmento del cuerpo
    for (int i = X_SQUARES - 1; i >= s2->head.x; i--) {
        for (int j = 0; j < MIN_DIM; j++) {
        s2->body[j].x = i;
        s2->body[j].y = s->head.y;
        boardMatrix[s2->head.y][s2->body[j].x] = s->id;
        }
    }

    s2->bodyDim = MIN_DIM;
    s2->lastMove = LEFT;
    s2->points = 0;
    boardMatrix[s2->head.y][s2->head.x] = s2->id;
    
}

static void copyColor(snakeStruct * s, int num){
    strcpy(s->color.color, colorNames[num-1]);
    s->color.colorAscii = colorHexa[num-1];
}

// creo que se puede hacer de mejor manera
static void chooseColors(snakeStruct * s1, snakeStruct * s2, int * flag){
    printf("Player 1: choose your snake color\n");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4\n");
    int color1 = getc1();
    copyColor(s1, color1);
    if( *flag == PLAYER_2){

    printf("Player 2: choose your snake color:\n");
    printf( "GREEN:1, RED:2, YELLOW:3, BLUE:4\n");
    copyColor(s2, getc2(color1));

    }
return;
}

// -------------- BOARD --------------

#define X 30
#define Y 30// dps hay que def esto
#define CER0 '\0'

static char boardMatrix[X][Y];

static void board(){
    // setBackgroundFont(GREEN_APPLE);
      for(int i = 0; i <  Y; i++){
        for(int j = 0; j < X; j++){
            boardMatrix[i][j] = CER0;
        }
    }
return;
}

// --------------------------- START SNAKE MAIN ----------------------

void start(){

    int flag = welcomeSnake();
    snakeStruct * snake1, * snake2; // snake2 asi me tiraba error ---- CHEQUEAR
    chooseColors(&snake1, &snake2, &flag);
    printf("Player 1 chose color: %s\n", snake1->color);
    if( flag == 2){
    printf("Player 2 chose color: %s\n", snake1->color);
    }
    // tendria que hacer una matriz con snake1.color y mandarle el color de la snake

    board(); // mapa para las snakes
    spawnSnake(snake1, snake2, &flag); // fija en que posicion van a spawnear las snakes

    sys_clearScreen();

    startGame(&flag); // gameStarter();
    int start = 0;
    tablero(&flag, start); // pointsTab(0, 0);
    get_apple(); // createFood();
    
    // GAME STARTS
    putBoard(&snake1, &snake2); // printBoard(&snakeP1,&snakeP2);
    putApple(); // updateBoardFromFood();
    
    sleep(); // chequear

    game(&snake1, &snake2, &flag); // JUEGO -> WHILE
}

// ------------------ GAME -----------------------
static char finish = 0;
#define EXIT 1

// --------------- CONTROLS -----------------------
// PLAYER UNO FLECHITAS
// PLAYER DOS -> W A S D 

#define APPLE '*' // CHEQUEAR
#define NULL ((void*)0) 

static int checkSnake(struct snakeStruct * s){
    int flag=1;
    if (s->head.x < 0 || s->head.x >= X_SQUARES || s->head.y < 0 || s->head.y >= Y_SQUARES) {
    //call_beep(2000); 
    //call_beep(1000);
    // call_beep(500); SONIDOOOOOO
    return flag;

}
// Verificar colisión con el cuerpo o con cualquier otra cosa que no sea vacío o comida
    if (boardMatrix[s->head.y][s->head.x] != CER0 && boardMatrix[s->head.y][s->head.x] != APPLE) {
    //call_beep(500);
    //call_beep(300);
    // call_beep(100);
    return flag;
    }
return !flag;
}


static int snakeEnDir(struct snakeStruct * s1, lastMoveEnum move){
    static int dir[2][8] = {{-1, -1, -1, 0, 1, 1, 1, 0},
                            {-1, 0, 1, 1, 1, 0, -1, -1}};
    int flag=1;
    switch (move) {
        case UP:
            s1->head.x = dir[0][0]; // Movimiento en X
            s1->head.y = dir[1][0]; // Movimiento en Y
            if( checkSnake(s1)){
                return flag;
            }
            break;
        case RIGHT:
            s1->head.x = dir[0][2];
            s1->head.y = dir[1][2];
            if( checkSnake(s1)){
                return flag;
            }
            break;
        case DOWN:
            s1->head.x = dir[0][4];
            s1->head.y = dir[1][4];
            if( checkSnake(s1)){
                return flag;
            }
            break;
        case LEFT:
            s1->head.x = dir[0][6];
            s1->head.y = dir[1][6];
            if( checkSnake(s1)){
                return flag;
            }
            break;
        default:
            return -1; // Movimiento no válido
    }if( snakeEats(s1)){
        s1->points +=1;
        s1->bodyDim +=1;
        s1->body[s1->bodyDim-1] = s1->head;
        return 0;   
    }
deleteTail(s1);
return !flag;
}

static void deleteTail(struct snakeStruct * s){
    boardMatrix[s->body[0].x][s->body[0].y] = CER0;
    int i=1;
    while( i < s->bodyDim){
        s->body[i - 1] = s->body[i];
        i++;
    }
    s->body[i - 1] = s->head;;
}


static int checkChar(char c) {
    return c >= 'a' && c <= 'z';
}

// CONTROLES DE TECLADO

static void playersKeyboard(int *flag,struct snakeStruct * s1,struct snakeStruct * s2, int *w1, int *w2){
    if(*flag == 1){
        keyPlayerOne(&s1,&w1); // arranque haciendola toda junta pero me termine mareando y no llegue a nada
        status(&s1, NULL);
    }else{
        keyPlayerTwo(&s1, &s2, &w2); // podria volver que si w2 -> gano p2 y w2=1 -> gano p1
        status(&s1, &s2);
    }
return;
}

// osea si retorna 1 sigue estando 
static void keyPlayerOne(snakeStruct *s, int *w1){
    int flag=1;
    char c=getChar();
    if( checkChar(c)){
        if(flag){
            switch (c){
                case KEY_ESC:
                    finish = END;
                    break;
                case KEY_W:
                    if( s->lastMove != DOWN)
                        flag = snakeEnDir(s, UP);
                    break;
                case KEY_S:
                    if( s->lastMove != UP)
                        flag = snakeEnDir(s, DOWN);
                case KEY_A:
                    if( s->lastMove != RIGHT)
                        flag = snakeEnDir(s, LEFT);
                case KEY_D:
                    if( s->lastMove != LEFT)
                        flag = snakeEnDir(s, RIGHT);
                default:
                    flag = snakeEnDir(s,s->lastMove);
                    break;
            }
        }else{
            flag = snakeEnDir(s,s->lastMove);
        }
    } // osea si flag = 0 es que choco
    if( flag == 0){
        *w1=1;
        return;
    }
return;

}

// CONTROLES DE TECLADO
// CHEQUEARRRRRRRRRRRR  linea 320
static void keyPlayerTwo(snakeStruct * s1,snakeStruct * s2, int *w2){
    int flagS1=0, flagS2=0;
    char c= getChar();
    if( checkChar(c)){
        switch(c) {
    case KEY_ESC: // Quit game (ESC)
        finish = 1;
        break;
    case KEY_W: // Move up
        if(s1->lastMove != DOWN) {
            flagS1 = ( snakeEnDir(s1, UP) || snakeEnDir(s2, s2->lastMove));
        }
        break;
    case KEY_S: // Move down
        if(s1->lastMove != UP) {
            flagS1 = (snakeEnDir(s1, DOWN) || snakeEnDir(s2, s2->lastMove));
        }
        break;
    case KEY_A: // Move left
        if(s1->lastMove != RIGHT && s1->lastMove != LEFT) {
            flagS1 = ( snakeEnDir(s1, LEFT) || snakeEnDir(s2, s2->lastMove));

        }
        break;
    case KEY_D: // Move right
        if(s1->lastMove != LEFT) {
            flagS1 = (snakeEnDir(s1, RIGHT) || snakeEnDir(s2, s2->lastMove));
        }
        break;
    case KEY_I: // Move up for second player
        if(s2->lastMove != DOWN) {
            if (snakeEnDir(s2, UP)) {
                flagS1 = 2;
            } else {
                flagS1 = 0;
            }
        flagS2 = snakeEnDir(s1, s1->lastMove);
        }
        break;
    case KEY_K: // Move down for second player
        if(s2->lastMove != UP && s2->lastMove != DOWN) {
            if(snakeEnDir(s2, DOWN ))
            flagS2 = snakeEnDir(s1, s1->lastMove);
            flagS1;
        }
        break;
    case KEY_J: // Move left for second player
        if(s2->lastMove != RIGHT) {
            flagS1 = snakeEnDir(s2, LEFT) ? 2 : 0;
            flagS2 = snakeEnDir(s1, s1->lastMove);
            flagS1 = (flagS2 > flagS1) ? flagS2 : flagS1;
        }
        break;
    case KEY_L: // Move right for second player
        if(s2->lastMove != LEFT) {
            flagS1 = snakeEnDir(s2, RIGHT) ? 2 : 0;
            flagS2 = snakeEnDir(s1, s1->lastMove);
            flagS1 = (flagS2 > flagS1) ? flagS2 : flagS1;
        }
        break;
    default:
        flagS2 = snakeEnDir(s1, s1->lastMove);
        flagS1 = snakeEnDir(s2, s2->lastMove);
        flagS1 = (flagS2 > flagS1) ? flagS2 : flagS1;
        break;
    }
    }else{
        flagS2 = snakeEnDir(s1, s1->lastMove);
        flagS1 = snakeEnDir(s2, s2->lastMove);
        flagS1 = (flagS2 > flagS1) ? flagS2 : flagS1;
    }
return flagS1;
}

static void winner(int * flag, int * w1, int *w2, struct snakeStruct *s1, struct snakeStruct *s2){
    if( *w1 == 1 && *flag == 1){
        printf("Player 1 points: %d\n", s1->points);
    }
    if( *flag == 2 ){
        if( *w2 == 2){
            printf("Player 2 won\n!");
            printf("%d vs %d", s2->points, s1->points);
        }
        if( *w2 == 1){
            printf("Player 1 won\n!");
            printf("%d vs %d", s1->points, s2->points);
        }
    }
    printf("Ready for another game? Press y\n");
    char c = getChar();
    if( c == 'y' ){
        startSnake();
    }else
        finish=1;
return;

}

static void game(snakeStruct * s1,snakeStruct * s2, int * flag){
    int w1, w2;
    while( finish != EXIT){
        playersKeyboard(&flag, &s1, &s2, &w1, &w2);
    
        updateBoard(&s1, &s2);
        printBoard(&s1, &s2);   // FUNCION IMPORTANT  
        // call_sleep(100);
    }
    winner(&flag, &w1, &w2, &s1, &s2);
}


// ---------------- APPLE -------------

static void get_apple(){
    apple->color = 0xaf0000; // APPLE RED COLOR
    direcs direc;
    direc.x = rand() % 32;  // insert number
    direc.y = rand() % 24;
    while( boardMatrix[direc.x][direc.y] != CER0){
        direc.x = rand() % 32; // insert number
        direc.y = rand() % 24; // chequear
    }
    
    apple->cord.x = direc.x;
    apple->cord.y = direc.y;

}




static void putApple(){
    sys_drawRectangle(apple->color, PIXEL_POS_X((uint64_t)apple->cord.x), PIXEL_POS_Y((uint64_t)apple->cord.y), 32, 32);
}

// ----------------------- STARTER ---------------------------- 

static void check(int *flag){
    char c;
    if( *flag == 1){
        printf("Player 1, Did you check? y/n\n");
        while (c != 'y') {
            c = getChar();
        }
    }
    if( *flag == 2){
        printf("Players, Did you check? y/n \n");
        while (c != 'y') {
            c = getChar();
        }
    }
    return;
}

static void checkstartSnake(int * flag){
    printf("turn off CAPS-LOCK\n");
    char c;
    check(&flag);
    sys_clearScreen();
    if( *flag == 1){
        printf("Player 1, are you ready?\n");
        printf("Player 1 keys: WASD");
        
    }
    if( *flag == 2){
        printf("Players, are you ready?\n");
        printf("Player 1 keys: WASD\n");
        printf("Player 2 keys: IJKL");
    }
    for( int i=5; i > 0; i--)
        printf("Game starts in %d", i);
    sys_sleep(300); // chequear
    sys_clearScreen(); // chequear
}


static void tablero(int * flag, int start){
    // syscall para donde poner el tablero
    if( * flag == 1){
        printf("Points: %d\n", start);
    }
    if( *flag == 2){
        printf("P1 vs P2");
        printf(" P1: %d", start);
        printf(" P2: %d", start);
    }
}

