#include "graphic.h"
#include "../cpu/core.h"
#include "def.h"
#include "rom.h"
#include "system.h"
#include <string.h>

uint16 *graphic_framebuffer = 0;
uint16 *source_mixer = 0;
uint16 SEG_TRANSPARENT_COLOR = 0;

uint16 rgb_multiply_8bits(uint32 bg, uint32 sg) {
  uint32 bg_r = (bg & GW_MASK_RGB565_R) >> 11;
  uint32 bg_g = (bg & GW_MASK_RGB565_G) >> 5;
  uint32 bg_b = (bg & GW_MASK_RGB565_B);

  bg_r = (bg_r * sg) >> 8;
  bg_g = (bg_g * sg) >> 8;
  bg_b = (bg_b * sg) >> 8;
  //  return (uint16)(bg_r << 11) | (bg_g << 5) | bg_b;
  //  return (uint16)(sg << 11) | (sg << 5) | sg;

  return sg;
}

void update_segment(uint8 segment_nb, bool segment_state) {
  //  if (segment_nb != 0)
  //    return;
  uint32 segment = _segments_offset[segment_nb];
  printf("%d %d \n", segment_nb, segment);
  //  uint8 cur_pixel;
  //  uint16 cur_pixel;
  int idx = 0;

  if (segment_state == 0)
    return;

  // idx = segment & 0x1;
  idx = segment;
  // segment = segment >> 1;
  //    uint8 *pixel;
  uint16 *pixel;

  pixel = (void *)_segments + segment;
  uint16 segments_x = _segments_x[segment_nb];
  uint16 segments_y = _segments_y[segment_nb];
  uint16 segments_width = _segments_width[segment_nb];
  uint16 segments_height = _segments_height[segment_nb];
  printf("pixel = %d x = %d y = %d w =%d h = %d\n", pixel, segments_x,
         segments_y, segments_width, segments_height);

  for (int line = segments_y; line < segments_height + segments_y; line++) {
    for (int x = segments_x; x < segments_width + segments_x; x++) {
      uint16 cur_pixel = *pixel++;
      // idx++;
      if (cur_pixel != SEG_TRANSPARENT_COLOR)
        graphic_framebuffer[line * GW_SCREEN_WIDTH + x] = cur_pixel;
      //   graphic_framebuffer[line * GW_SCREEN_WIDTH + x] = rgb_multiply_8bits(
      //       source_mixer[line * GW_SCREEN_WIDTH + x], cur_pixel);
    }
  }
}

void render_to_framebuffer(uint16 *framebuffer) {
  uint8 seg;
  graphic_framebuffer = framebuffer;
  memcpy(framebuffer, background, GW_SCREEN_WIDTH * GW_SCREEN_HEIGHT * 2);
  SEG_TRANSPARENT_COLOR = SEG_WHITE_COLOR;
  source_mixer = framebuffer;

  for (int h = 0; h < 2; h++) {
    for (int o = 0; o < m_o_pins; o++) {
      seg = h ? m_ox_state[o] : m_o_state[o];
      update_segment(8 * o + 0 + h, m_bp ? ((seg & 0x1) != 0) : 0);
      update_segment(8 * o + 2 + h, m_bp ? ((seg & 0x2) != 0) : 0);
      update_segment(8 * o + 4 + h, m_bp ? ((seg & 0x4) != 0) : 0);
      update_segment(8 * o + 6 + h, m_bp ? ((seg & 0x8) != 0) : 0);
    }
  }
}
