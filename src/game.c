#include <stdbool.h>
#include <time.h>
#include "game.h"
#include "framework.h"

Uint32 temp = 0;
const Uint32 TIME_TO_MOVE = 125;

int  direction = 1;
int  position = 0;
int  length = 0;
int  apple = 5;
int  boardSize = 100;
int  boardCols = 10;
int tail[100];
int boardCells[100];
int  isDead = 0;


enum { EMPY, HEAD, BODY, APPLE };

// =======================================================================//
// ! Utilities functions                                                  //
// =======================================================================//

bool isFirstCol() {
    return position % boardCols == 0;
}

bool isLastCol() {
    return (position + 1) % boardCols == 0;
}

// =======================================================================//
// ! Snake specific functions                                             //
// =======================================================================//


void moveHead() {
//    prev = position;
    tail[length] = position;
    position += direction;

    if (direction ==  1 && isFirstCol())
        position -= boardCols;

    else if (direction == -1 && isLastCol())
        position += boardCols;

    else if (position < 0)
        position += boardSize;

    else if (position >= boardSize)
        position -= boardSize;
}

bool hasCollisionWithTail(int positionToCheck) {
    for (int i = 0; i < length; i++)
        if ( tail[i] == positionToCheck ) return true;
    return false;
}

bool hasCollisionBeetwenHeadAndApple() {
    if ( position == apple ) return true;
    return false;
}

void placeApple() {
    srand(time(NULL));
    do {
        apple = rand() % boardSize;
    } while( hasCollisionBeetwenHeadAndApple() || hasCollisionWithTail(apple) );
}

void setDirection(int dir) {
    if(dir != -direction || length == 0)
        direction = dir;
}

// =======================================================================//
// ! Implementation dependent functions                                   //
// =======================================================================//

void drawBoard(int* cells, int size, int cols, int x, int y) {
    const int SIZE = 25;
    Rect rect = {x, y, SIZE, SIZE};
    for (int i = 0; i < size; i++) {

        if ( i == position ) draw_rect(&rect, HEAD_COLOR);
        else if ( i == apple ) draw_rect(&rect, APPLE_COLOR);
        else draw_rect(&rect, CELL);

        for (int j = 0; j < length; j++)
            if ( i == tail[j] )
                draw_rect(&rect, TAIL_COLOR);

        rect.x += rect.w + 1;
        if ((i + 1) % cols == 0) {
            rect.x = x;
            rect.y += rect.h + 1;
        }
    }
}

void handleInput() {
    if (getKeyState(SDL_SCANCODE_LEFT))
        setDirection(-1);
    if (getKeyState(SDL_SCANCODE_RIGHT))
        setDirection( 1);
    if (getKeyState(SDL_SCANCODE_UP))
        setDirection(-boardCols);
    if (getKeyState(SDL_SCANCODE_DOWN))
        setDirection( boardCols);
}

// =======================================================================//
// ! Framework functions                                                  //
// =======================================================================//

void update(Uint32 delta) {
    temp += delta;

    handleInput();

    if (temp >= TIME_TO_MOVE && !isDead) {
        temp = 0;

        moveHead();
//        tail[length] = prev; // MOVE BODY
        for (int i = 0; i < length; i++) tail[i] = tail[i + 1]; // SORT BODY

        if (hasCollisionBeetwenHeadAndApple()) {
            placeApple();
            length++;
            for(int i = length; i > 0; i--)
                tail[i] = tail[i - 1];
            tail[0] = position;
        } else if (hasCollisionWithTail(position)) {
            isDead = 1;
        }
    }

    drawBoard(boardCells, boardSize, boardCols, 10, 70);

    Rect bg = {10, 10, 100, 50};
    draw_rect(&bg, DISPLAY_COLOR);

    draw_text(15, 7, NUMBER_COLOR, "%d", length);
}