#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <memmap.h>

#define MAX_MEMORY_SIZE (16ll << 30)
#define BLOCK_SIZE (4ll << 10)
#define BAD_POINTER (void*) 0xF0F0F0F0F0F0F0F0
#define MAX_BLOCKS (MAX_MEMORY_SIZE / BLOCK_SIZE)

void set_full(uint64_t, uint8_t);
void set_empty(uint64_t, uint8_t);
uint8_t is_full(uint64_t);
uint8_t is_empty(uint64_t);
void group(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);
void init_buddy(struct extra_mmap_entry *, uint32_t);
void buddy_free(void* block);
uint64_t allocate(uint64_t, uint64_t, uint64_t);
void* buddy_allocate();

#endif /*__BUDDY_H__*/