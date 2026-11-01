#ifndef state_h_
#define state_h_


#include <stdint.h>

#include "constants.h"


typedef struct {
    uint8_t memory[MEMORY_SIZE];
    uint8_t display[DISPLAY_SIZE];
    uint16_t pc;
    uint16_t i;
    uint16_t stack[STACK_DEPTH];
    uint8_t top;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t v[16];
} State;


void init_state(State *state);


#endif
