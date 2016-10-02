#include "desc.h"
#include "ints.h"
#include "memory.h"
#include "ioport.h"
#include "outstr.h"
#include "idt_init.h"
#include "timer.h"

/*static void qemu_gdb_hang(void) {
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}
*/
void main(void) {
	disable_ints();
	set_div_to_write();
	write_string("Start the programm\n");
	init_all();
	write_string("Generate interruption\n");
	
	__asm__ volatile("int $32");
	init_timer();
	enable_ints();
	while (1);
}
