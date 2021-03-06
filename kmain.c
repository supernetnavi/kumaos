#include "drivers/frame_buffer.h"
#include "drivers/serial_port.h"
#include "drivers/pic.h"
#include "assembly_interface.h"
#include "page_table.h"
#include "types.h"
#include "stdio.h"
#include "global_descriptor_table.h"
#include "interrupt_descriptor_table.h"

void kmain()
{
  clear_screen();
  // static char *welcome_message = "Welcome to KUMAOS!";
  // printf(welcome_message);

  serial_init(SERIAL_COM1_BASE);
  log("Initialized serial port.\n");

  initialize_gdt();
  log("Loaded global descriptor table.\n");

  initialize_idt();
  log("Loaded interrupt descriptor table\n");

  pic_init();
  log("Initialized PIC\n");

  page_directory_t pd = initialize_page_directory();
  log("Initialized page directory\n");

  log("Setting page directory...\n");
  set_page_directory(pd);
  log("Set page directory.\n");

  log("Enabling paging...\n");
  enable_paging();
  log("Paging enabled.\n");

  while(1) {}
}
