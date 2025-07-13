#include "../cpu/core.h"
#include "def.h"
#include "pico/stdlib.h"

u8 B = 1;
u8 BA = 1;

u32 process_input() {
  u32 result = 0;
  gpio_put(6, 1);
  gpio_put(7, 0);
  busy_wait_us_32(10);
  if (gpio_get(10)) {
    result = 1;
  } else if (gpio_get(11)) {
    result = 2;
  } else if (gpio_get(12)) {
    result = 3;
  } else if (gpio_get(13)) {
    result = 4;
  }
  gpio_put(6, 0);
  gpio_put(7, 1);
  busy_wait_us_32(10);
  if (gpio_get(10)) {
    result = 5;
  } else if (gpio_get(11)) {
    result = 6;
  } else if (gpio_get(12)) {
    result = 7;
  } else if (gpio_get(13)) {
    result = 8;
  }
  gpio_put(6, 0);
  gpio_put(7, 0);

  BA = !gpio_get(8);
  u8 a = !gpio_get(9);
  if (!a) {
    device_reset();
  }
  return result;
}