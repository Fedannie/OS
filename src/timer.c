#include "outstr.h"
#include "ioport.h"
#include "idt_init.h"

#define TIMER_COMAND_PORT 0x43
#define TIMER_PORT 0x40

void init_timer() {
	write_string("Init timer.\n");
	out8(TIMER_COMAND_PORT, (1<<2) | (1<<4) | (1<<5));
	out8(TIMER_PORT, 127);
	out8(TIMER_PORT, 127);
	master_open_port(0);
}