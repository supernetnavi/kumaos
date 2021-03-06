#include "interrupts.h"
#include "stdio.h"
#include "assembly_interface.h"
#include "drivers/pic.h"
#include "drivers/keyboard.h"
#include "drivers/frame_buffer.h"

void interrupt_handler(struct cpu_state cpu, uint32_t interrupt_number, uint32_t error_code) {
  // Avoid unused parameter errors
  (void) cpu;
  (void) error_code;

  switch(interrupt_number) {
    case(0x00000009):
      log("Keyboard interrupt was called.\n");
      keyboard_interrupt_handler();
      break;
    default:
      log("Unhandled interrupt occured.\n");
      break;
  }

  pic_acknowledge();
}
