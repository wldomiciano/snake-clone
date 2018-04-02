#include <time.h>
#include <SDL2/SDL.h>

extern const Uint8* keyStates;
extern SDL_Renderer* renderer;

#define BOARD_COLS 10
#define BOARD_SIZE 100

int temp      = 0;
int TIMEOUT   = 150;
int direction = 1;
int position  = 0;
int length    = 0;
int apple     = 5;
int isDead    = 0;
int i         = 0;
int tail[BOARD_SIZE];

int isFirstCol() {
    return position % BOARD_COLS == 0;
}

int isLastCol(int pos) {
    return (pos + 1) % BOARD_COLS == 0;
}

void move() {
    tail[i++] = position;
    position += direction;

    if (i >= length) i = 0;

    if (direction ==  1 && isFirstCol())
        position -= BOARD_COLS;

    else if (direction == -1 && isLastCol(position))
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
    return position == apple;
}

void placeApple() {
    do {
        apple = rand() % BOARD_SIZE;
    } while( hasCollisionBeetwenHeadAndApple() || hasCollisionWithTail(apple) );
}

void setDirection(int dir) {
    if(dir != -direction || length == 0)
        direction = dir;
}

void drawBoard(int x, int y) {
    SDL_Rect rect = {x, y, 25, 25};

    for (int i = 0; i < BOARD_SIZE; i++) {
        if ( i == position )
            SDL_SetRenderDrawColor(renderer,  26, 118, 114, 255);
        else if ( i == apple )
            SDL_SetRenderDrawColor(renderer, 198,  46,  46, 255);
        else if ( hasCollisionWithTail(i) )
            SDL_SetRenderDrawColor(renderer, 36, 161, 156, 255);
        else
            SDL_SetRenderDrawColor(renderer, 181, 175, 164, 255);

        SDL_RenderFillRect(renderer, &rect);

        rect.x += rect.w + 1;
        if ( isLastCol(i) ) {
            rect.x = x;
            rect.y += rect.h + 1;
        }
    }
}

void handleInput() {
    if (keyStates[SDL_SCANCODE_LEFT])
        setDirection(-1);
    if (keyStates[SDL_SCANCODE_RIGHT])
        setDirection( 1);
    if (keyStates[SDL_SCANCODE_UP])
        setDirection(-BOARD_COLS);
    if (keyStates[SDL_SCANCODE_DOWN])
        setDirection( BOARD_COLS);
}

void update(Uint32 delta) {
    temp += delta;

    handleInput();

    if (temp >= TIMEOUT && !isDead) {
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

    drawBoard(10, 10);
}