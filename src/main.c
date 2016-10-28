#include <memmap.h>
#include <print.h>
#include <buddy.h>

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
int mmap_length;
struct extra_mmap_entry mmap[MAX_MMAP_LENGTH];

void test() {
	uint64_t **pointers = buddy_allocate();
	for (uint64_t i = 0; i < 155; i++) {
		pointers[i] = buddy_allocate();
		if ((uint64_t)pointers[i] % BLOCK_SIZE != 0ull) {
			printf("Buddy test failed.");
			return;
		}
		for (uint64_t j = 0; j < 155; j++) {
			pointers[i][j] = i;
		}
	}
	
	for (uint64_t i = 0; i < 155; i++) {
		if (pointers[i][i] != i) {
			printf("Buddy test failed.");
			return;
		}
		buddy_free(pointers[i]);
	}
	buddy_free(pointers);
	printf("OK");
}

void main() {
	mmap_length = read_mmap(mmap, multiboot_info);
	print_mmap(mmap, mmap_length);
	init_buddy(mmap, mmap_length);
	test();
	while (1);
}
