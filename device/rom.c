#include "rom.h"
#include "def.h"
#include "system.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gwromheader_t head;

u8 *rom_base = NULL;
u16 *background = NULL;
u8 *_segments = NULL;
u16 *_segments_x = NULL;
u16 *_segments_y = NULL;
u16 *_segments_width = NULL;
u16 *_segments_height = NULL;
u32 *_segments_offset = NULL;
u8 *program = NULL;
u8 *melody = NULL;
u32 *keyboard = NULL;
bool rotate = false;

bool romloader(const u8 *_rom, uint32 _len) {
  u8 *src = (u8 *)_rom;
  u32 rom_size_src = _len;
  u32 rom_size_dest = _len;

  if (memcmp(src, ROM_CPU_SM5A, 3) == 0) {
    printf("Not compressed : header OK\n");
    rom_size_src = _len;
  } else {
    printf("Unknow ROM format\n");
    return false;
  }

  printf("romsize uncompressed = %d\n", rom_size_src);

  memcpy(&head, _rom, sizeof(head));
  rom_size_dest = head.keyboard + head.keyboard_size;

  if (rom_size_src != rom_size_dest) {
    printf("ROM ERROR,size=%u,expected=%u\n", rom_size_src, rom_size_dest);
    return false;
  } else {
    printf("ROM size: OK\n");
  }

  if (head.background_pixel_size != 0)
    background = (u16 *)&_rom[head.background_pixel];
  else
    background = (u16 *)&_rom[rom_size_src];

  _segments = (u8 *)&_rom[head.segments_pixel];
  _segments_x = (u16 *)&_rom[head.segments_x];
  _segments_y = (u16 *)&_rom[head.segments_y];
  _segments_width = (u16 *)&_rom[head.segments_width];
  _segments_height = (u16 *)&_rom[head.segments_height];
  _segments_offset = (u32 *)&_rom[head.segments_offset];
  program = (u8 *)&_rom[head.program];
  keyboard = (u32 *)&_rom[head.keyboard];

  if (head.melody_size != 0)
    melody = (u8 *)&_rom[head.melody];

  return true;
}
