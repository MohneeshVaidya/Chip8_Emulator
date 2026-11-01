#ifndef display_h_
#define display_h_


#include <SDL3/SDL.h>

#include "constants.h"
#include "state.h"


typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    bool running;
    uint32_t pixels[DISPLAY_SIZE];
} App;


bool init_app(App *app);
bool close_app(App *app);

bool app_loop(App *app, State *state);

#endif
