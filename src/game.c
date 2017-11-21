#include <stdbool.h>
#include <time.h>
#include "game.h"
#include "framework.h"

Uint32 temp = 0;
const Uint32 TIME_TO_MOVE = 125;

typedef struct Snake {
    int  direction;
    int  position;
    int  prev;
    int  length;
    int  apple;
    int  boardSize;
    int  boardCols;
    int* tail;
    int* boardCells;
    int  isDead;
} Snake;

Snake snake = {0};

enum { EMPY, HEAD, BODY, APPLE };

// =======================================================================//
// ! Utilities functions                                                  //
// =======================================================================//

bool isFirstCol(int totalCols, int position) {
    return position % totalCols == 0;
}

bool isLastCol(int totalCols, int position) {
    return (position + 1) % totalCols == 0;
}

bool hasCollision(int positionA, int positionB) {
    return positionA == positionB;
}

void addToArray(int* array, int index, int value) {
    array[index] = value;
}

void sortArray(int* array, int arrayLength) {
    if (arrayLength > 0) {
        int temp = array[0];
        for (int i = 0, j = 1; i < arrayLength; i++, j++)
            array[i] = array[j];
        array[arrayLength - 1] = temp;
    }
}

// =======================================================================//
// ! Snake specific functions                                             //
// =======================================================================//

void clear(int* cells, int position) {
    addToArray(cells, position, EMPY);
}

void clearHead(Snake* snake) {
    clear(snake->boardCells, snake->position);
}

void clearBody(Snake* snake) {
    for (int i = 0; i < snake->length; i++)
        clear(snake->boardCells, snake->tail[i]);
}

void clearApple(Snake* snake) {
    clear(snake->boardCells, snake->apple);
}

void insertHead(Snake* snake) {
    addToArray(snake->boardCells, snake->position, HEAD);
}

void insertBody(Snake* snake) {
    for (int i = 0; i < snake->length; i++)
        addToArray(snake->boardCells, snake->tail[i], BODY);
}

void insertApple(Snake* snake) {
    addToArray(snake->boardCells, snake->apple, APPLE);
}

void moveHead(Snake* snake) {
    snake->prev = snake->position;
    snake->position += snake->direction;

    if (snake->direction ==  1 && isFirstCol(snake->boardCols, snake->position))
        snake->position -= snake->boardCols;

    else if (snake->direction == -1 && isLastCol(snake->boardCols, snake->position))
        snake->position += snake->boardCols;

    else if (snake->position < 0)
        snake->position += snake->boardSize;

    else if (snake->position >= snake->boardSize)
        snake->position -= snake->boardSize;
}

void moveBody(Snake* snake) {
    snake->tail[0] = snake->prev;
}

void sortBody(Snake* snake) {
    sortArray(snake->tail, snake->length);
}

bool hasCollisionWithTail(Snake* snake, int positionToCheck) {
    for (int i = 0; i < snake->length; i++)
        if ( hasCollision(snake->tail[i], positionToCheck) ) return true;
    return false;
}

bool hasCollisionWithHead(Snake* snake, int positionToCheck) {
    if ( hasCollision(snake->position, positionToCheck) ) return true;
    return false;
}

bool hasCollisionWithApple(Snake* snake, int positionToCheck) {
    if ( hasCollision(snake->apple, positionToCheck) ) return true;
    return false;
}

void placeApple(Snake* snake) {
    srand(time(NULL));
    do {
        snake->apple = rand() % snake->boardSize;
    } while( hasCollisionWithHead(snake, snake->apple) || hasCollisionWithTail(snake, snake->apple) );
}

void setDirection(Snake* snake, int direction) {
    if(direction != -snake->direction || snake->length == 0) {
        snake->direction = direction;
    }
}

// =======================================================================//
// ! Implementation dependent functions                                   //
// =======================================================================//

void drawBoard(int* cells, int size, int cols, int x, int y) {
    const int SIZE = 25;
    Rect rect = {x, y, SIZE, SIZE};
    for (int i = 0; i < size; i++) {
        switch (cells[i]) {
            case EMPY : draw_rect(&rect, CELL)         ; break;
            case HEAD : draw_rect(&rect, HEAD_COLOR)   ; break;
            case BODY : draw_rect(&rect, TAIL_COLOR)   ; break;
            case APPLE: draw_rect(&rect, APPLE_COLOR)  ; break;
        }

        rect.x += rect.w + 1;
        if ((i + 1) % cols == 0) {
            rect.x = x;
            rect.y += rect.h + 1;
        }
    }
}

void handleInput(Snake* snake) {
    if (getKeyState(SDL_SCANCODE_LEFT))
        setDirection(snake, -1);
    if (getKeyState(SDL_SCANCODE_RIGHT))
        setDirection(snake,  1);
    if (getKeyState(SDL_SCANCODE_UP))
        setDirection(snake, -snake->boardCols);
    if (getKeyState(SDL_SCANCODE_DOWN))
        setDirection(snake,  snake->boardCols);
}

// =======================================================================//
// ! Framework functions                                                  //
// =======================================================================//

void create()  {
    snake.boardCols  = 20;
    snake.boardSize  = 20 * 20;
    snake.boardCells = calloc(snake.boardSize, sizeof(*snake.boardCells));
    snake.tail       = calloc(snake.boardSize, sizeof(*snake.tail));
    snake.length     = 0;
    snake.direction  = 1;
    snake.isDead     = 0;
    snake.position   = 0;

    placeApple(&snake);
}

void update(Uint32 delta) {
    temp += delta;

    handleInput(&snake);

    if (temp >= TIME_TO_MOVE && !snake.isDead) {
        temp = 0;
        clearHead(&snake);
        clearBody(&snake);
        clearApple(&snake);

        moveHead(&snake);
        moveBody(&snake);
        sortBody(&snake);

        if (hasCollisionWithApple(&snake, snake.position)) {
            placeApple(&snake);
            snake.length++;
            for(int i = snake.length; i >= 0; i--)
                snake.tail[i] = snake.tail[i - 1];
            snake.tail[0] = snake.position;
        } else if (hasCollisionWithTail(&snake, snake.position)) {
            snake.isDead = 1;
        }
    }

    insertBody(&snake);
    insertHead(&snake);
    insertApple(&snake);

    drawBoard(snake.boardCells, snake.boardSize, snake.boardCols, 10, 70);

    Rect bg = {10, 10, 100, 50};
    draw_rect(&bg, DISPLAY_COLOR);

    draw_text(15, 7, NUMBER_COLOR, "%d", snake.length);
}

void destroy() {
    free(snake.boardCells);
    free(snake.tail);
}
