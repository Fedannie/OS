#include <memmap.h>
#define KT 0
#define FT 1
#define RT 2
extern char text_phys_begin[];
extern char bss_phys_end[];
struct extra_mmap_entry kernel;
	
void sort_mmap(struct extra_mmap_entry *memory_map, uint32_t map_length) {
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

struct extra_mmap_entry *set(struct extra_mmap_entry *entry, uint32_t new_type, uint64_t new_begin, uint64_t new_end) {
	entry->type = new_type;
	entry->begin = new_begin;
	entry->end = new_end - 1;
	return ++entry;
}

struct extra_mmap_entry *comp_last_with_kernel(struct extra_mmap_entry *entry, struct map_entry *p) {
	if (kernel.begin > p->addr) {
		if (kernel.begin >= p->addr + p->length) {
			entry = set(entry, p->type, p->addr, p->addr + p->length);
		} else if (kernel.end >= p->addr + p->length) {
			entry = set(entry, p->type, p->addr, kernel.begin);
		} else {
			entry = set(entry, p->type, p->addr, kernel.begin);
			entry = set(entry, p->type, kernel.end + 1, p->addr + p->length);
		}
	} else {
		if (kernel.end >= p->addr + p->length) {
			return entry;
		} else if (kernel.end >= p->addr) {
			entry = set(entry, p->type, kernel.end + 1, p->addr + p->length);
		} else {
			entry = set(entry, p->type, p->addr, p->addr + p->length);
		}
	}
	return entry;
}

uint32_t read_mmap(struct extra_mmap_entry *memory_map, const uint32_t multiboot_info) {
	struct multiboot_info *info = (struct multiboot_info *)(uint64_t)multiboot_info;
	struct map_entry *map_pos = (struct map_entry*)(uint64_t)(info->mmap_addr);
	struct extra_mmap_entry *new_map = memory_map;
	uint32_t map_size = info->mmap_length;
	kernel.begin = (uint64_t) text_phys_begin;
	kernel.end = (uint64_t) bss_phys_end;
	uint32_t cnt = 0;
	struct extra_mmap_entry *entry = new_map;
	struct map_entry *prev = map_pos;
	*entry = kernel;
	entry++;
	while (cnt < map_size) {
		entry = comp_last_with_kernel(entry, prev);
		cnt += prev->size + sizeof(prev->size);
		prev = (struct map_entry *)((char*)prev + prev->size + sizeof(prev->size));
	}
	uint32_t map_length = entry - new_map;
	sort_mmap(new_map, map_length);
	return map_length;
}

void print_mmap(struct extra_mmap_entry *memory_map, uint32_t map_length) {
	struct extra_mmap_entry *map = memory_map; 
	for (uint32_t i = 0; i < map_length; i++) {
		printf("from %lx to %lx -- ", map[i].begin, map[i].end);
		if (map[i].type == KT) {
			printf("kernel\n");
		} else if (map[i].type == FT) {
			printf("free\n");
		} else {
			printf("reserved\n");
		}
	}
}