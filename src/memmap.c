#include<memmap.h>

#define MEMMAP_MAX_SIZE 100	

my_mem_entry_t memory_map[MEMMAP_MAX_SIZE];
uint64_t memory_map_size = 0;

extern uint8_t text_phys_begin[];
extern uint8_t bss_phys_end[];

void add_my_entry(uint64_t base_addr, uint64_t length, uint32_t type) {
	my_mem_entry_t* entry = memory_map + memory_map_size;
	memory_map_size++;

	entry->begin  = base_addr;
	entry->end    = base_addr + length - 1;
	entry->length = length;
	entry->type   = type;
}

extern uint32_t multiboot_info;

void initialize_memory_map() {

	uint64_t multiboot_info64 = (uint64_t)multiboot_info;

	my_mem_entry_t* kernel_my_entry = memory_map + memory_map_size;
	memory_map_size++;

	kernel_my_entry->begin  = (uint64_t)text_phys_begin;
	kernel_my_entry->end    = (uint64_t)bss_phys_end;
	kernel_my_entry->length = (uint64_t)kernel_my_entry->end - (uint64_t)kernel_my_entry->begin + 1;
	kernel_my_entry->type   = 2;

    uint32_t memmap_length = *((uint32_t*)multiboot_info64 + 11);
 	uint64_t memmap_base_addr = (uint64_t)(*(uint64_t*)(multiboot_info64 + 48));
	
	for (uint64_t memmap_ptr = memmap_base_addr; memmap_ptr < memmap_base_addr + memmap_length; ) {
		mem_entry_t* entry = (mem_entry_t*)memmap_ptr;
		memmap_ptr += entry->size + sizeof(entry->size);

		uint64_t left  = entry->base_addr;
		uint64_t right = entry->base_addr + entry->length - 1;
		
		if (right < kernel_my_entry->begin || left > kernel_my_entry->end) {
			add_my_entry(left, entry->length, entry->type);
		} else if (left < kernel_my_entry->begin && kernel_my_entry->end < right) {
			add_my_entry(left, kernel_my_entry->begin - left, entry->type);	
		 	add_my_entry(kernel_my_entry->end + 1, right - kernel_my_entry->end, entry->type);	
		} else if (left < kernel_my_entry->begin) {
			add_my_entry(left, kernel_my_entry->begin - left, entry->type);	
		} else if (right > kernel_my_entry->end) {
			add_my_entry(kernel_my_entry->end + 1, right - kernel_my_entry->end, entry->type);	
		}

	}
}

void print(my_mem_entry_t entry, uint64_t num) {
	printf("memory entry #%lli: 0x%llx-0x%llx(%lli bytes), type = %d\n", 
		num, entry.begin, entry.end, entry.length, entry.type);
}

uint8_t correct(my_mem_entry_t entry) {
	if (entry.begin <= entry.end) return 1;
	return 0;
}

void print_memory_map() {
	printf("_________Memory map_________ \n");

	int num = 0;
	for (uint64_t i = 0; i < memory_map_size; i++) {
		if (correct(memory_map[i])) {
			print(memory_map[i], num++);
		}
	}
}


