#include <SDL.h>

#define BOARD_COLS 10
#define BOARD_SIZE 100
#define TIMEOUT 150

int acc = 0;
int direction = 1;
int position = 0;
int length = 0;
int apple = 5;
int isDead = 0;
int i = 0;
int tail[BOARD_SIZE];

SDL_Renderer* renderer;
SDL_Window* window;
Uint32 previousTicks = 0;

SDL_bool keyLeft;
SDL_bool keyRight;
SDL_bool keyDown;
SDL_bool keyUp;

int getRandom(int max) {
  return rand() % max;
}

void draw(int x, int y, int w, int h, int r, int g, int b) {
  SDL_Rect rect = {x, y, w, h};
  SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &rect);
}

int isFirstCol() {
  return position % BOARD_COLS == 0;
}

int isLastCol(int pos) {
  return (pos + 1) % BOARD_COLS == 0;
}

SDL_bool hasCollisionWithTail(int positionToCheck) {
  for (int i = 0; i < length; i++)
    if (tail[i] == positionToCheck) return SDL_TRUE;
  return SDL_FALSE;
}

SDL_bool hasCollisionBeetwenHeadAndApple() {
  return position == apple;
}

void placeApple() {
  do {
    apple = getRandom(BOARD_SIZE);
  } while (hasCollisionBeetwenHeadAndApple() || hasCollisionWithTail(apple));
}

void checkDirection(int dir) {
  if (dir != -direction || length == 0) direction = dir;
}

void setDirection() {
  if (keyUp) checkDirection(-BOARD_COLS);
  if (keyDown) checkDirection(BOARD_COLS);
  if (keyLeft) checkDirection(-1);
  if (keyRight) checkDirection(1);
}

void drawBoard() {
  int SIZE = 25, PADDING = 10, x = PADDING, y = PADDING;

  for (int i = 0; i < BOARD_SIZE; i++) {
    if (i == position)
      draw(x, y, SIZE, SIZE, 26, 118, 114);
    else if (i == apple)
      draw(x, y, SIZE, SIZE, 198, 46, 46);
    else if (hasCollisionWithTail(i))
      draw(x, y, SIZE, SIZE, 36, 161, 156);
    else
      draw(x, y, SIZE, SIZE, 181, 175, 164);

    x += SIZE + 1;
    if (isLastCol(i)) {
      x = PADDING;
      y += SIZE + 1;
    }
  }
}

void move() {
  tail[i++] = position;
  position += direction;

  if (i >= length) i = 0;

  if (direction == 1 && isFirstCol())
    position -= BOARD_COLS;

  else if (direction == -1 && isLastCol(position))
    position += BOARD_COLS;

  else if (position < 0)
    position += BOARD_SIZE;

  else if (position >= BOARD_SIZE)
    position -= BOARD_SIZE;
}

void update(int delta) {
  acc += delta;

  setDirection();

  if (acc >= TIMEOUT && !isDead) {
    acc = 0;
    move();

    if (hasCollisionBeetwenHeadAndApple()) {
      placeApple();
      length++;
      tail[length - 1] = position;
    } else if (hasCollisionWithTail(position)) {
      isDead = 1;
    }
  }

  drawBoard();
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  if (SDL_CreateWindowAndRenderer(280, 280, 0, &window, &renderer) < 0) {
    SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
    return 1;
  }

  while (!SDL_QuitRequested()) {
    keyUp = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP];
    keyDown = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN];
    keyLeft = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT];
    keyRight = SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT];

    SDL_SetRenderDrawColor(renderer, 228, 220, 207, 255);
    SDL_RenderClear(renderer);

    Uint32 current = SDL_GetTicks();
    Uint32 delta = current - previousTicks;
    previousTicks = current;

    update(delta);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
