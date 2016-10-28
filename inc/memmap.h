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
}__attribute__((packed));

#define MAX_MMAP_LENGTH 100
#define KT 0
#define FT 1
#define RT 2

void print_mmap(struct extra_mmap_entry *, uint32_t);
void sort_mmap(struct extra_mmap_entry *, uint32_t);
uint32_t read_mmap(struct extra_mmap_entry *, const uint32_t);
struct extra_mmap_entry *comp_last_with_kernel(struct extra_mmap_entry *, struct map_entry *);

#endif /* __MEMMAP_H__ */
