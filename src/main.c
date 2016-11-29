#include "memmap.h"
#include "print.h"
#include "allocator.h"
#include "slab_allocator.h"
#include "malloc.h"
#include "lock.h"
#include "idt_init.h"
/*
static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}
*/



void main() {
	init_all();
	initialize_memory_map();
	print_memory_map();
	initialize_buddy_allocator();

 	while (1);
}
