#include "display.h"
#include "utils.h"


int main(int argc, char **argv) {
    if (argc < 2) {
        SDL_Log("valid usage: %s <rom-path>\n", argv[0]);
        return 0;
    }

    App app;

    if (!init_app(&app)) {
        return 1;
    }

    State state;

    init_state(&state);

    load_fonts(&state);

    if (load_rom(&state, argv[1])) {
        app_loop(&app, &state);
    }

    close_app(&app);

    return 0;
}
