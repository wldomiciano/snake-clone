#ifndef SNAKECLONERESTARTED_FRAMEWORK_H
#define SNAKECLONERESTARTED_FRAMEWORK_H

#include <SDL2/SDL.h>

typedef SDL_Rect Rect;

void sdl_init(Uint32 width, Uint32 height);
void sdl_quit();
void sdl_event_handling();
void sdl_render_clear();
void sdl_render_present();
Uint32 sdl_delta_time();
int getKeyState(int key);
SDL_Color intToColor(Uint32 hexColor);

SDL_bool is_running();
void draw_rect(Rect* rect, Uint32 hexColor);
void draw_text(int x, int y, Uint32 hexColor, const char *fmt, ...);

#endif //SNAKECLONERESTARTED_FRAMEWORK_H
