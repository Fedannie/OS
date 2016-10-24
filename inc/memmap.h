#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#include <stdint.h>
#include <print.h>
#include <multiboot.h>

struct map_entry {
	uint32_t size;
	uint64_t addr;
	uint64_t length;
	uint32_t type;
} __attribute__((packed));

struct extra_mmap_entry {
	uint32_t type;
	uint64_t begin;
	uint64_t end;
};

#define MAX_MMAP_LENGTH 100
#define KT 0
#define FT 1
#define RT 2

typedef struct map_entry map_entry
typedef struct extra_mmap_entry extra_mmap_entry
typedef struct extra_mmap_entry* extra_entry_ptr
typedef struct map_entry* map_entry_ptr

void print_mmap();
void sort_mmap();
void read_mmap(const uint32_t);
extra_entry_ptr comp_last_with_kernel(extra_entry_ptr, map_entry_ptr);

#endif /* __MEMMAP_H__ */
