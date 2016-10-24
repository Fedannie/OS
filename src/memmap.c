#include <memmap.h>
extern char text_phys_begin[];
extern char bss_phys_end[];
struct extra_mmap_entry memory_map[MAX_MMAP_LENGTH];
uint32_t map_length;
struct extra_mmap_entry kernel;
	
void print_mmap() {
	struct extra_mmap_entry *map = memory_map; 
	for (uint32_t i = 0; i < map_length; i++) {
		printf("from %lx to %lx -- ", map[i].begin, 
			map[i].end);
		if (map[i].type == KERNEL_TYPE) printf("kernel\n");
		else if (map[i].type == FREE_TYPE) printf("free\n");
		else printf("reserved\n");
	}
}

void sort_mmap() {
	struct extra_mmap_entry *map = memory_map;
	for (uint32_t i = 0; i < map_length; i++) {
		for (uint32_t j = i + 1; j < map_length; j++) {
			if (map[i].begin >= map[j].begin) {
				struct extra_mmap_entry tmp = map[i];
				map[i] = map[j];
				map[j] = tmp;
			}
		}
	}
}

struct extra_mmap_entry *set_entry(struct extra_mmap_entry *entry, uint32_t new_type, uint64_t new_begin, uint64_t new_end) {
	entry->type = new_type;
	entry->begin = new_begin;
	entry->end = new_end;
	entry++;
	return entry;
}

struct extra_mmap_entry *comp_last_with_kernel(struct extra_mmap_entry *entry, struct map_entry *prev) {
	if (kernel.begin <= prev->addr) {
		if (kernel.end >= prev->addr + prev->length) {
			entry = set_entry(entry, RESERVED_TYPE, prev->addr, prev->addr + prev->length);
		} else if (kernel.end > prev->addr) {
			entry = set_entry(entry, RESERVED_TYPE, prev->addr, kernel.end);
			entry = set_entry(entry, prev->type, kernel.end, prev->addr + prev->length);
		} else {
			entry = set_entry(entry, prev->type, prev->addr, prev->addr + prev->length);
		}
	} else {
		if (kernel.begin >= prev->addr + prev->length) {
			entry = set_entry(entry, prev->type, prev->addr, prev->addr + prev->length);
		} else if (kernel.end >= prev->addr + prev->length) {
			entry = set_entry(entry, prev->type, prev->addr, kernel.begin);
			entry = set_entry(entry, RESERVED_TYPE, kernel.begin, prev->addr + prev->length);
		} else {
			entry = set_entry(entry, prev->type, prev->addr, kernel.begin);
			entry = set_entry(entry, RESERVED_TYPE, kernel.begin, kernel.end);
			entry = set_entry(entry, prev->type, kernel.end, prev->addr + prev->length);
		}
	}
	return entry;
}

void read_mmap(const uint32_t multiboot_info) {
	struct multiboot_info *info = (struct multiboot_info *)(uint64_t)multiboot_info;
	struct map_entry *map_pos = (struct map_entry*)(uint64_t)(info->mmap_addr);
	struct extra_mmap_entry *new_map = memory_map;
	uint32_t map_size = info->mmap_length;
	kernel.begin = (uint64_t) text_phys_begin;
	kernel.end = (uint64_t) bss_phys_end;
	uint32_t cnt = 0;
	struct extra_mmap_entry *entry = new_map;
	struct map_entry *prev = map_pos;
	while (cnt < map_size) {
		entry = comp_last_with_kernel(entry, prev);
		cnt += prev->size + 4;
		prev = (struct map_entry *)((char*)prev + prev->size + 4);
	}
	map_length = entry - new_map;
	sort_mmap();
}
