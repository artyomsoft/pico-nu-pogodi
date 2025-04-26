#ifndef _GW_GRAPHIC_H_
#define _GW_GRAPHIC_H_
#include "def.h"

#define GW_MASK_RGB565_R 0xF800
#define GW_MASK_RGB565_G 0x07E0
#define GW_MASK_RGB565_B 0x001F
#define SEG_WHITE_COLOR 0xffff
#define SEG_BLACK_COLOR 0x0

void render_to_framebuffer(uint16 *framebuffer);

#endif
