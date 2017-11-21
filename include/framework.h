#ifndef SNAKECLONERESTARTED_FRAMEWORK_H
#define SNAKECLONERESTARTED_FRAMEWORK_H

#include <SDL2/SDL.h>

typedef SDL_Rect Rect;
typedef SDL_Color Color;

extern const Color CELL;
extern const Color HEAD_COLOR;
extern const Color TAIL_COLOR;
extern const Color APPLE_COLOR;
extern const Color NUMBER_COLOR;
extern const Color DISPLAY_COLOR;

void sdl_init(Uint32 width, Uint32 height);
void sdl_quit();
void sdl_event_handling();
void sdl_render_clear();
void sdl_render_present();
Uint32 sdl_delta_time();
int getKeyState(int key);

SDL_bool is_running();
void draw_rect(Rect* rect, Color color);
void draw_text(int x, int y, Color color, const char *fmt, ...);

#endif //SNAKECLONERESTARTED_FRAMEWORK_H
