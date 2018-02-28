#include "framework.h"
#include "game.h"

int main(int argc, char *argv[]) {
    sdl_init(280, 338);

    while (is_running()) {
        sdl_event_handling();

        sdl_render_clear();

        update(sdl_delta_time());

        sdl_render_present();
    }

    sdl_quit();
    return 0;
}