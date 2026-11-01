#include <stdlib.h>
#include "engine.h"
#include "constants.h"


#define GET_NIBBLE(instruction, k) \
    (uint8_t)((instruction >> ((4 - k) * 4)) & 15)


static bool is_update;


static void update_pixels(uint8_t *display, uint32_t *pixels) {
    for (int i = 0; i < DISPLAY_SIZE; i++) {
        pixels[i] = display[i] ? COLOR_WHITE : COLOR_BLACK;
    }
}


static void clear_screen(State *state) {
    for (int i = 0; i < DISPLAY_SIZE; i++) {
        state->display[i] = 0;
    }
    is_update = true;
}


static void draw(State *state, uint8_t x, uint8_t y, uint8_t n) {
    state->v[0xF] = 0;
    uint16_t address = state->i;
    for (int i = 0; i < n; i++) {
        uint8_t byte = state->memory[address + i];
        for (int k = 7; k >= 0; k--) {
            int x_coor = x + 7 - k;
            int y_coor = y + i;
            if (x_coor >= DISPLAY_WIDTH || y_coor >= DISPLAY_HEIGHT) {
                continue;
            }
            int index = y_coor * DISPLAY_WIDTH + x_coor;

            uint8_t sprite_pixel = (byte >> k) & 1;
            if (sprite_pixel) {
                if (state->display[index]) {
                    state->v[0xF] = 1;
                }
                state->display[index] ^= sprite_pixel;
            }
        }
    }
    is_update = true;
}


void execute(State *state, uint32_t *pixels, const bool *keyboard, int8_t key) {
#define GET_12_BITS(a, b, c) \
    ((a << 8) | (b << 4) | c)

#define GET_8_BITS(a, b) \
    ((a << 4) | b)

    uint16_t instruction = (uint16_t)((state->memory[state->pc] << 8) | state->memory[state->pc + 1]);

    if (instruction == 0) {
        return;
    }

    state->pc += 2;

    uint8_t n1 = GET_NIBBLE(instruction, 1);
    uint8_t n2 = GET_NIBBLE(instruction, 2);
    uint8_t n3 = GET_NIBBLE(instruction, 3);
    uint8_t n4 = GET_NIBBLE(instruction, 4);

    is_update = false;

    switch (n1) {
        case 0x0: {
            if (n2 == 0 && n3 == 0xE && n4 == 0) {
                clear_screen(state);
            } else if (n2 == 0 && n3 == 0xE && n4 == 0xE) {
                state->pc = state->stack[--state->top];
            }
            break;
        }
        case 0x1: {
            state->pc = GET_12_BITS(n2, n3, n4);
            break;
        }
        case 0x2: {
            state->stack[state->top++] = state->pc;
            state->pc = GET_12_BITS(n2, n3, n4);
            break;
        }
        case 0x3: {
            uint16_t v1 = state->v[n2];
            uint16_t v2 = GET_8_BITS(n3, n4);
            if (v1 == v2) {
                state->pc += 2;
            }
            break;
        }
        case 0x4: {
            uint16_t v1 = state->v[n2];
            uint16_t v2 = GET_8_BITS(n3, n4);
            if (v1 != v2) {
                state->pc += 2;
            }
            break;
        }
        case 0x5: {
            uint16_t v1 = state->v[n2];
            uint16_t v2 = state->v[n3];
            if (v1 == v2) {
                state->pc += 2;
            }
            break;
        }
        case 0x6: {
            state->v[n2] = GET_8_BITS(n3, n4);
            break;
        }
        case 0x7: {
            state->v[n2] += GET_8_BITS(n3, n4);
            break;
        }
        case 0x8: {
            uint8_t *const x = &state->v[n2];
            uint8_t *const y = &state->v[n3];

            switch (n4) {
                case 0x0: {
                    *x = *y;
                    break;
                }
                case 0x1: {
                    *x |= *y;
                    break;
                }
                case 0x2: {
                    *x &= *y;
                    break;
                }
                case 0x3: {
                    *x ^= *y;
                    break;
                }
                case 0x4: {
                    state->v[0xF] = (*x > UINT8_MAX - *y) ? 1 : 0;
                    *x += *y;
                    break;
                }
                case 0x5: {
                    state->v[0xF] = (*x >= *y) ? 1 : 0;
                    *x -= *y;
                    break;
                }
                case 0x6: {
                    *x = *y;
                    state->v[0xF] = *x & 1;
                    *x >>= 1;
                    break;
                }
                case 0x7: {
                    state->v[0xF] = (*y >= *x) ? 1 : 0;
                    *x = *y - *x;
                    break;
                }
                case 0xE: {
                    uint8_t value = *y;
                    state->v[0xF] = (value >> 7) & 1;
                    *x = value << 1;
                    break;
                }
            }
            break;
        }
        case 0x9: {
            uint16_t v1 = state->v[n2];
            uint16_t v2 = state->v[n3];
            if (v1 != v2) {
                state->pc += 2;
            }
            break;
        }
        case 0xA: {
            state->i = GET_12_BITS(n2, n3, n4);
            break;
        }
        case 0xB: {
            state->pc = GET_12_BITS(n2, n3, n4) + state->v[0];
            break;
        }
        case 0xC: {
            state->v[n2] = rand() & GET_8_BITS(n3, n4);
            break;
        }
        case 0xD: {
            uint8_t x = state->v[n2] % DISPLAY_WIDTH;
            uint8_t y = state->v[n3] % DISPLAY_HEIGHT;
            draw(state, x, y, n4);
            break;
        }
        case 0xE: {
            if (n3 == 0x9 && n4 == 0xE) {
                if (keyboard[keymap_rev[state->v[n2]]]) {
                    state->pc += 2;
                }
            } else if (n3 == 0xA && n4 == 0x1) {
                if (!keyboard[keymap_rev[state->v[n2]]]) {
                    state->pc += 2;
                }
            }
            break;
        }
        case 0xF: {
            uint8_t *const x = &state->v[n2];
            if (n3 == 0x0 && n4 == 0x7) {
                *x = state->delay_timer;
            } else if (n3 == 0x1 && n4 == 0x5) {
                state->delay_timer = *x;
            } else if (n3 == 0x1 && n4 == 0x8) {
                state->sound_timer = *x;
            } else if (n3 == 0x1 && n4 == 0xE) {
                uint16_t sum = state->i + *x;
                state->v[0xF] = (sum > 0xFFF);
                state->i = sum;
            } else if (n3 == 0x0 && n4 == 0xA) {
                if (key == -1) {
                    state->pc -= 2;
                } else {
                    *x = (uint8_t)key;
                }
            } else if (n3 == 0x2 && n4 == 0x9) {
                state->i = FONT_START + (*x & 0xF) * FONT_SIZE;
            } else if (n3 == 0x3 && n4 == 0x3) {
                uint8_t value = *x;
                uint8_t d[3] = {0};
                uint8_t k = 2;
                while (value) {
                    d[k--] = value % 10;
                    value /= 10;
                }
                for (k = 0; k < 3; k++) {
                    state->memory[state->i + k] = d[k];
                }
            } else if (n3 == 0x5 && n4 == 0x5) {
                for (int i = 0; i <= n2; i++) {
                    state->memory[state->i + i] = state->v[i];
                }
            } else if (n3 == 0x6 && n4 == 0x5) {
                for (int i = 0; i <= n2; i++) {
                    state->v[i] = state->memory[state->i + i];
                }
            }
            break;
        }
        default: {
            break;
        }
    }

    if (is_update) {
        update_pixels(state->display, pixels);
    }
}
