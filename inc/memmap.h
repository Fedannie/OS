#ifndef __MEMMAP__

#define __MEMMAP__

#include <print.h>
#include <memory.h>
#include <ints.h>
#include <print.h>
#include <desc.h> 

struct mem_entry {
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
}__attribute__((packed));

struct my_mem_entry {
	uint64_t begin;
	uint64_t end;
	uint64_t length;
	uint32_t type;
}__attribute__((packed));

typedef struct my_mem_entry my_mem_entry_t;
typedef struct mem_entry mem_entry_t;

extern my_mem_entry_t memory_map[];
extern uint64_t memory_map_size;

void initialize_memory_map();
void print_memory_map();


#endif
