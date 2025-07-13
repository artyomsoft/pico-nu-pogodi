/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "device/sound.h"
#include "device/system.h"
#include "gfx.h"
#include "hardware/clocks.h"
#include "ili9341.h"
#include "pico/multicore.h"
#include "stdlib.h"
#include <stdio.h>

#define TFT_SCLK 18
#define TFT_MOSI 19
#define TFT_MISO 255 // Not required, used for DC...
#define TFT_DC 21
#define TFT_RST 20
#define TFT_CS 17
#define TFT_BACKLIGHT 255 // hardwired to 3.3v

#define TFT_WIDTH 320
#define TFT_HEIGHT 240
#define TFT_ROTATION 3

int main() {
  //  busy_wait_us_32(1000000);
  //  set_sys_clock_hz(134217728, true);
  stdio_init_all();
  //  play_sound_dma();
  LCD_setPins(TFT_DC, TFT_CS, TFT_RST, TFT_SCLK, TFT_MOSI);
  LCD_initDisplay();
  LCD_setRotation(TFT_ROTATION);
  // //  getchar();
  init(1, NULL);
}
