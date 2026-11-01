#ifndef constants_h_
#define constants_h_


#include <SDL3/SDL.h>


#define MEMORY_SIZE 4096
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT)
#define STACK_DEPTH 16
#define PIXEL_SIZE 4
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x000000FF
#define PROG_START 0x200
#define FONT_START 0x50
#define FONT_SIZE 5
#define FONT_COUNT 16


static const uint8_t keymap[] = {
    [SDL_SCANCODE_X] = 0x0,
    [SDL_SCANCODE_1] = 0x1,
    [SDL_SCANCODE_2] = 0x2,
    [SDL_SCANCODE_3] = 0x3,
    [SDL_SCANCODE_Q] = 0x4,
    [SDL_SCANCODE_W] = 0x5,
    [SDL_SCANCODE_E] = 0x6,
    [SDL_SCANCODE_A] = 0x7,
    [SDL_SCANCODE_S] = 0x8,
    [SDL_SCANCODE_D] = 0x9,
    [SDL_SCANCODE_Z] = 0xA,
    [SDL_SCANCODE_C] = 0xB,
    [SDL_SCANCODE_4] = 0xC,
    [SDL_SCANCODE_R] = 0xD,
    [SDL_SCANCODE_F] = 0xE,
    [SDL_SCANCODE_V] = 0xF,
};


static const SDL_Scancode keymap_rev[] = {
    SDL_SCANCODE_X,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
    SDL_SCANCODE_V,
};


#endif
