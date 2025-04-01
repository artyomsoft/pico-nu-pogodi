#ifndef _GW_ROMLOADER_H_
#define _GW_ROMLOADER_H_
#include "def.h"
#include <stdint.h>
#include <unistd.h>

typedef struct gwromheader_s {
  char cpu_name[8];
  char rom_signature[8];
  u8 time_hour_address_msb;
  u8 time_hour_address_lsb;
  u8 time_min_address_msb;
  u8 time_min_address_lsb;
  u8 time_sec_address_msb;
  u8 time_sec_address_lsb;
  u8 time_hour_msb_pm;
  u8 byte_spare1;
  u32 flags;
  u32 background_pixel;
  u32 background_pixel_size;
  u32 segments_pixel;
  u32 segments_pixel_size;
  u32 segments_offset;
  u32 segments_offset_size;
  u32 segments_x;
  u32 segments_x_size;
  u32 segments_y;
  u32 segments_y_size;
  u32 segments_height;
  u32 segments_height_size;
  u32 segments_width;
  u32 segments_width_size;
  u32 melody;
  u32 melody_size;
  u32 program;
  u32 program_size;
  u32 keyboard;
  u32 keyboard_size;
} gwromheader_t;

extern u8 *rom_base;
extern u16 *background;
extern u8 *_segments;
extern u16 *_segments_x;
extern u16 *_segments_y;
extern u16 *_segments_width;
extern u16 *_segments_height;
extern u32 *_segments_offset;
extern u8 *program;
extern u8 *melody;
extern u32 *keyboard;
extern bool rotate;

extern gwromheader_t head;

bool romloader(const u8 *rom, uint32 _len);
#endif
