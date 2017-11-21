#include <SDL2/SDL_ttf.h>
#include "framework.h"

const Color CELL = {181, 175, 164, 255};
const Color HEAD_COLOR = {26, 118, 114, 255};
const Color TAIL_COLOR = {36, 161, 156, 255};
const Color APPLE_COLOR = {198, 46, 46, 255};
const Color NUMBER_COLOR = {55, 60, 64, 255};
const Color DISPLAY_COLOR = {166, 184, 162, 255};

SDL_Renderer* renderer = NULL;
static SDL_Window* window = NULL;
static SDL_Event event = {0};
static SDL_bool isRunning = SDL_TRUE;
static Uint32 previousTicks = 0;
static const Uint8* keyStates = NULL;
static TTF_Font* font;

void sdl_init(Uint32 width, Uint32 height) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    font = TTF_OpenFont("assets/Roboto-Bold.ttf", 48);

    keyStates = SDL_GetKeyboardState(NULL);

    window = SDL_CreateWindow("Snake Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}

void sdl_quit() {
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void sdl_event_handling() {
    while (SDL_PollEvent(&event))
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            isRunning = SDL_FALSE;
}

int getKeyState(int key) {
    return keyStates[key];
}

Uint32 sdl_delta_time() {
    Uint32 current = SDL_GetTicks();
    Uint32 delta = current - previousTicks;
    previousTicks = current;
    return delta;
}

void sdl_render_clear() {
    SDL_SetRenderDrawColor(renderer, 228, 220, 207, 255);
    SDL_RenderClear(renderer);
}

void sdl_render_present() {
    SDL_RenderPresent(renderer);
}

SDL_bool is_running() {
    return isRunning;
}

void draw_rect(Rect* rect, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, rect);
}

#define MAX_LENGTH 1024
void draw_text(int x, int y, Color color, const char *fmt, ...) {
    SDL_Rect rect = { x, y, 0, 0 };
    char buffer[MAX_LENGTH] = {0};

    va_list ap;
    va_start (ap, fmt);
    SDL_vsnprintf(buffer, MAX_LENGTH, fmt, ap);
    va_end (ap);

    SDL_Surface* text = TTF_RenderText_Solid(font, buffer, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text);

    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(text);
    SDL_DestroyTexture(texture);
}
