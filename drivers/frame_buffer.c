#include "frame_buffer.h"
#include "../assembly_interface.h"
#include "../types.h"

uint8_t *fb = (uint8_t *) 0x000B8000;

uint16_t cursor_pos = 0;

void fb_write_cell(uint32_t cell, uint8_t c, uint8_t fg, uint8_t bg)
{
  int i = cell*2;
  fb[i] = c;
  fb[i+1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

void clear_screen()
{
  for (int i = 0; i < FB_CELLS; i++) {
    fb_write_cell(i, ' ', FB_BLACK, FB_BLACK);
  }
}

void fb_write_byte(uint8_t b)
{
  fb_write_cell(cursor_pos, b, FB_WHITE, FB_BLACK);
  cursor_pos++;

  if (cursor_pos < FB_CELLS) {
    move_cursor_to_pos(cursor_pos);
  }
}

int fb_write(const char *s)
{
  int i = 0;
  while(s[i]) {
    fb_write_cell(i, s[i], FB_WHITE, FB_BLACK);
    i++;
  }
  return i;
}

void fb_backspace() {
  cursor_pos--;
  fb_write_cell(cursor_pos, ' ', FB_BLACK, FB_BLACK);
  move_cursor_to_pos(cursor_pos);
}

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

void move_cursor_to_pos(unsigned short pos)
{
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT,    pos & 0x00FF);
}

void move_cursor(unsigned short row, unsigned short col)
{
  move_cursor_to_pos(row*FB_COLS + col);
}
