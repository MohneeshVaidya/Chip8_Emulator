#include "display.h"
#include "engine.h"

#define WINDOW_TITLE "Chip8_Emulator"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500
#define LOGICAL_WIDTH 64
#define LOGICAL_HEIGHT 32

#define MAX_ELAPSED 100


bool init_app(App *app) {
    app->window = NULL;
    app->renderer = NULL;
    app->texture = NULL;
    app->running = false;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("not able to initialize window: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &app->window, &app->renderer)) {
        SDL_Log("not able to create window/renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_SetRenderLogicalPresentation(app->renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    app->texture = SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    if (app->texture == NULL) {
        SDL_Log("not able to create texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        return false;
    }
    SDL_SetTextureScaleMode(app->texture, SDL_SCALEMODE_NEAREST);

    app->running = true;

    for (int i = 0; i < DISPLAY_SIZE; i++) {
        app->pixels[i] = COLOR_BLACK;
    }

    return true;
}


bool close_app(App *app) {
    if (app) {
        if (app->texture) {
            SDL_DestroyTexture(app->texture);
            app->texture = NULL;
        }
        if (app->renderer) {
            SDL_DestroyRenderer(app->renderer);
            app->renderer = NULL;
        }
        if (app->window) {
            SDL_DestroyWindow(app->window);
            app->window = NULL;
        }
        app->running = false;
    }
    return true;
}


static void process_events(App *app, int8_t *key) {
    SDL_Event event = {0};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)) {
            app->running = false;
            break;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
            *key = keymap[event.key.scancode];
            break;
        }
    }
}


static void render(App *app) {
    SDL_RenderClear(app->renderer);
    SDL_UpdateTexture(app->texture, NULL, app->pixels, DISPLAY_WIDTH * PIXEL_SIZE);
    SDL_RenderTexture(app->renderer, app->texture, NULL, NULL);
    SDL_RenderPresent(app->renderer);
}


bool app_loop(App *app, State *state) {
    const float instr_time = 1000/700.f;
    const float frame_time = 1000/60.f;

    float instr_accum = 0;
    float frame_accum = 0;

    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 prev = SDL_GetPerformanceCounter();
    Uint64 curr = 0;

    const bool *keyboard = SDL_GetKeyboardState(NULL);

    int8_t key;

    while (app->running) {
        curr = SDL_GetPerformanceCounter();
        float elaplsed = ((float)(curr - prev) / freq) * 1000;
        prev = curr;

        if (elaplsed > MAX_ELAPSED) {
            elaplsed = MAX_ELAPSED;
        }

        instr_accum += elaplsed;
        frame_accum += elaplsed;

        key = -1;

        process_events(app, &key);

        if (!app->running) {
            break;
        }

        while (instr_accum >= instr_time) {
            execute(state, app->pixels, keyboard, key);
            instr_accum -= instr_time;
        }

        while (frame_accum >= frame_time) {
            render(app);
            frame_accum -= frame_time;
            if (state->delay_timer) {
                state->delay_timer--;
            }
            if (state->sound_timer) {
                state->sound_timer--;
            }
        }
    }

    return true;
}
