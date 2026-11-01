#include "state.h"


void init_state(State *state) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        state->memory[i] = 0;
    }

    for (int i = 0; i < DISPLAY_SIZE; i++) {
        state->display[i] = 0;
    }

    state->pc = 0;
    state->i = 0;
    state->top = 0;
    state->delay_timer = 10;
    state->sound_timer = 10;

    for (int i = 0; i < 16; i++) {
        state->v[i] = 0;
    }
}
