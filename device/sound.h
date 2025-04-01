#ifndef _GW_SOUND_H_
#define _GW_SOUND_H_
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define SOUND_BUFFER_SIZE 256

volatile extern uint8_t *current_buff;

extern uint8_t buffer1[SOUND_BUFFER_SIZE];
extern uint8_t buffer2[SOUND_BUFFER_SIZE];

extern int init_sound();

#endif