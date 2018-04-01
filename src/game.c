#include <time.h>
#include "game.h"
#include "framework.h"

#define BOARD_COLS 10
#define BOARD_SIZE 100

Uint32 temp = 0;
const Uint32 TIME_TO_MOVE = 125;
int direction = 1;
int position = 0;
int length = 0;
int apple = 5;
int tail[BOARD_SIZE];
int isDead = 0;
int index = 0;

int isFirstCol() {
    return position % BOARD_COLS == 0;
}

int isLastCol() {
    return (position + 1) % BOARD_COLS == 0;
}

void move() {
    tail[index++] = position;
    position += direction;

    if (index >= length) index = 0;

    if (direction ==  1 && isFirstCol())
        position -= BOARD_COLS;

    else if (direction == -1 && isLastCol())
        position += BOARD_COLS;

    else if (position < 0)
        position += BOARD_SIZE;

    else if (position >= BOARD_SIZE)
        position -= BOARD_SIZE;
}

SDL_bool hasCollisionWithTail(int positionToCheck) {
    for (int i = 0; i < length; i++)
        if ( tail[i] == positionToCheck ) return SDL_TRUE;
    return SDL_FALSE;
}

SDL_bool hasCollisionBeetwenHeadAndApple() {
    if ( position == apple ) return SDL_TRUE;
    return SDL_FALSE;
}

void placeApple() {
    srand((unsigned) time(NULL));
    do {
        apple = rand() % BOARD_SIZE;
    } while( hasCollisionBeetwenHeadAndApple() || hasCollisionWithTail(apple) );
}

void setDirection(int dir) {
    if(dir != -direction || length == 0)
        direction = dir;
}

void drawBoard(int x, int y) {
    const int SIZE = 25;
    Rect rect = {x, y, SIZE, SIZE};
    for (int i = 0; i < BOARD_SIZE; i++) {
        if ( i == position ) draw_rect(&rect, 0x1a7672);
        else if ( i == apple ) draw_rect(&rect, 0xc62e2e);
        else draw_rect(&rect, 0xb5afa4);

        for (int j = 0; j < length; j++)
            if ( i == tail[j] )
                draw_rect(&rect, 0x24a19c);

        rect.x += rect.w + 1;
        if ((i + 1) % BOARD_COLS == 0) {
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
        setDirection(-BOARD_COLS);
    if (getKeyState(SDL_SCANCODE_DOWN))
        setDirection( BOARD_COLS);
}

void update(Uint32 delta) {
    temp += delta;

    handleInput();

    if (temp >= TIME_TO_MOVE && !isDead) {
        temp = 0;

        move();

        if (hasCollisionBeetwenHeadAndApple()) {
            placeApple();
            length++;
            tail[length - 1] = position;
        } else if (hasCollisionWithTail(position)) {
            isDead = 1;
        }
    }

    drawBoard(10, 70);

    Rect bg = {10, 10, 100, 50};
    draw_rect(&bg, 0xa6b8a2);
    draw_text(15, 7, 0x373c40, "%d", length);
}