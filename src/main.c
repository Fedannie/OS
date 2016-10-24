#include <memmap.h>
#include <print.h>

/*
static void qemu_gdb_hang(void)
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}
*/

extern const uint32_t multiboot_info;

void main() {
	read_mmap(multiboot_info);
	print_mmap();
	while (1);
}
