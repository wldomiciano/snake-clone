#include "framework.h"
#include "game.h"

extern SDL_Renderer* renderer;

int main(int argc, char *argv[]) {
    sdl_init(541, 601);

    create();

    while (is_running()) {
        sdl_event_handling();

        sdl_render_clear();

        update(sdl_delta_time());

        sdl_render_present();
    }

    destroy();

    sdl_quit();
    return 0;
}