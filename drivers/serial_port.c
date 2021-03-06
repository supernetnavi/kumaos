#include "serial_port.h"
#include "../assembly_interface.h"
#include "../types.h"

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

#define SERIAL_LINE_ENABLE_DLAB 0x80

void serial_configure_baud_rate(uint16_t com, uint16_t divisor)
{
  outb(SERIAL_LINE_COMMAND_PORT(com),
      SERIAL_LINE_ENABLE_DLAB);
  outb(SERIAL_DATA_PORT(com),
      (divisor >> 8) & 0x00FF);
  outb(SERIAL_DATA_PORT(com),
      divisor & 0x00FF);
}

void serial_configure_line(uint16_t com)
{
  /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
   * Content: | d | b | prty  | s | dl  |
   * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
   */
  outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_fifo(uint16_t com)
{
  /* Bit:     | 7 6 | 5  | 4 | 3   | 2   | 1   | 0 |
   * Content: | lvl | bs | r | dma | clt | clr | e |
   * Value:   | 11  | 0  | 0 | 0   | 1   | 1   | 1 |
   */
  outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

void serial_configure_modem(uint16_t com)
{
  /* Bit:     | 7 | 6 | 5  | 4  | 3   | 2   | 1   | 0   |
   * Content: | r | r | af | lb | ao2 | ao1 | rts | dtr |
   * Value:   | 0 | 0 | 0  | 0  | 0   | 0   | 1   | 1   |
   */
  outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

void serial_init(uint16_t com) {
  serial_configure_baud_rate(com, 2);
  serial_configure_line(com);
  serial_configure_fifo(com);
  serial_configure_modem(com);
}

int serial_is_transmit_fifo_empty(uint32_t com)
{
  /* 0x20 = 0010 0000 */
  return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

int serial_write(uint16_t com, const char * s)
{
  int i = 0;
  while(s[i]) {
    serial_write_char(com, s[i]);
    i++;
  }
  return i;
}

void serial_write_char(uint16_t com, uint8_t c)
{
    // Wait if buffer is full
    while (!serial_is_transmit_fifo_empty(com)) {}

    outb(SERIAL_DATA_PORT(com), c);
}
