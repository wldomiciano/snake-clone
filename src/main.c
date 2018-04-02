#include <SDL2/SDL.h>

void update(Uint32 delta);

SDL_Renderer* renderer;
SDL_Window* window;
Uint32 previousTicks = 0;
const Uint8* keyStates;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(280, 280, 0, &window, &renderer);
    keyStates = SDL_GetKeyboardState(NULL);

    while ( !SDL_QuitRequested() ) {
        SDL_SetRenderDrawColor(renderer, 228, 220, 207, 255);
        SDL_RenderClear(renderer);

        Uint32 current = SDL_GetTicks();
        Uint32 delta   = current - previousTicks;
        previousTicks  = current;

        update(delta);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}