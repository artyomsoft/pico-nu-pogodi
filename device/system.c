#include "system.h"
#include "../cpu/core.h"
#include "graphic.h"
#include "ili9341.h"
#include "input.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "rom.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "sound.h"

#include "../data/o.h"

extern volatile bool emulate;
extern volatile bool draw;
extern volatile uint8 *buffer_start;

int button;

void writeR(u8 data) {
  if (data & 0x01) {
    *(current_buff++) = 255;
  } else {
    *(current_buff++) = 0;
  }
};

u32 get_buttons() { return button; }
u8 readB() { return B; }
u8 readBA() { return BA; }
u8 readK(u8 io_S) {
  if ((button == 5) && (io_S == 4)) {
    return 8;
  } else if ((button == 6) && (io_S == 4)) {
    return 4;
  } else if ((button == 7) && (io_S == 4)) {
    return 2;
  } else if ((button == 8) && (io_S == 4)) {
    return 1;
  } else if ((button == 1) && (io_S == 8)) {
    return 8;
  } else if ((button == 2) && (io_S == 8)) {
    return 4;
  } else if ((button == 3) && (io_S == 8)) {
    return 2;
  } else if ((button == 4) && (io_S == 8)) {
    return 1;
  } else {
    return 0;
  }
}

uint16_t fb[GW_LCD_WIDTH * GW_LCD_HEIGHT];

void render() {
  while (true) {
    if (draw) {
      render_to_framebuffer(fb);
      LCD_WriteBitmap(0, 0, GW_LCD_WIDTH, GW_LCD_HEIGHT, fb);
      draw = false;
    }
  }
}

int system_run(int clock_cycles) {
  m_k_active = (get_buttons() != 0);
  m_icount += (clock_cycles / 2);

  current_buff = buffer_start;
  device_run();
  return m_icount * m_clk_div;
}
void mainloop() {

  device_start();
  device_reset();

  for (int i = 6; i < 14; i++) {
    gpio_init(i);
  }

  for (int i = 6; i < 8; i++) {
    gpio_set_dir(i, GPIO_OUT);
  }

  gpio_set_dir(8, GPIO_IN);
  gpio_pull_down(8);

  gpio_set_dir(9, GPIO_IN);
  gpio_pull_down(9);

  for (int i = 10; i < 14; i++) {
    gpio_set_dir(i, GPIO_IN);
    gpio_pull_down(i);
  }

  multicore_launch_core1(render);
  init_sound();

  while (1) {
    if (emulate) {
      emulate = false;
      button = process_input();
      system_run(SYS_FREQ / 128);
    }
  }
}
int init(int argc, char **argv) {
  u8 *rom = NULL;
  bool romflg = false;
  int size = 0;

  //  size = 179348;
  romflg = romloader(o_data, sizeof(o_data));

  if (!romflg)
    return 1;

  mainloop();

  return 0;
}
