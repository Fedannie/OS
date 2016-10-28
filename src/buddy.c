#include <buddy.h>

uint8_t blocks[MAX_BLOCKS];

void set_full(uint64_t num, uint8_t full) {
	uint64_t new_num = num >> 2;
	uint64_t shift = (num & 3ll) << 1;
	blocks[new_num] = (blocks[new_num] & ~(1 << shift)) | (full << shift);
}

void set_empty(uint64_t num, uint8_t empty) {
	uint64_t new_num = num >> 2;
	uint64_t shift = ((num & 3ll) << 1) + 1;
	blocks[new_num] = (blocks[new_num] & ~(1 << shift)) | (empty << shift);
}

void set_next(uint8_t mode, uint64_t num, uint8_t what){
	if (mode == 0) {
		set_full(num * 2, what);
		set_full(num * 2 + 1, what);
	} else {
		set_empty(num * 2, what);
		set_empty(num * 2 + 1, what);		
	}
}

uint8_t is_full(uint64_t num) {
	uint64_t new_num = num >> 2;
	uint64_t shift = (num & 3ll) << 1;
	return (blocks[new_num] >> shift) & 1;
}

uint8_t is_empty(uint64_t num) {
	uint64_t new_num = num >> 2;
	uint64_t shift = ((num & 3ll) << 1) + 1;
	return (blocks[new_num] >> shift) & 1;
}

void group(uint64_t pos, uint64_t cur_left, uint64_t cur_right, uint64_t page_left, uint64_t page_right) {
	if (cur_left >= page_right || cur_right <= page_left) {
		return;
	}
	if (cur_left >= page_left && cur_right <= page_right) {
		set_full(pos, 1);
		set_empty(pos, 0);
		return;
	}
	int mnum = (cur_left + cur_right) / 2;
	if (is_empty(pos)) {
		set_next(1, pos, 1);
	}
	if (is_full(pos)) {
		set_next(0, pos, 1);
	}
	group(pos * 2, cur_left, mnum, page_left, page_right);
	group(pos * 2 + 1, mnum, cur_right, page_left, page_right);
	set_full(pos, is_full(pos * 2) & is_full(pos * 2 + 1));
	set_empty(pos, is_empty(pos * 2) & is_empty(pos * 2 + 1));
}

void init_buddy(struct extra_mmap_entry *memmap, uint32_t memmap_size) {
	set_full(1, 0);
	set_empty(1, 1);
	for (uint32_t i = 0; i < memmap_size; i++) {
		uint64_t cut_beginning = (memmap[i].begin + BLOCK_SIZE - 1) / BLOCK_SIZE;
		uint64_t cut_end = memmap[i].end / BLOCK_SIZE;
		if (memmap[i].type == FT && cut_end > cut_beginning) {
			group(1, 0, MAX_BLOCKS, cut_beginning, cut_end);
		}
	}
}

void buddy_free(void *ptr) {
	uint64_t num = (uint64_t) ptr / BLOCK_SIZE + MAX_BLOCKS;
	set_empty(num, 0);
	while (num > 1) {
		num /= 2;
		set_empty(num, 0);
		set_full(num, is_full(num * 2) & is_full(num * 2 + 1));
	}
}

uint64_t allocate(uint64_t num, uint64_t cur_left, uint64_t cur_right) {
	if (cur_right - cur_left == 1) {
		set_full(num, 0);
		set_empty(num, 1);
		return cur_left;
	}	
	if (is_full(num)) {
		set_next(0, num, 1);
		set_next(1, num, 0);
	}
	uint64_t block;
	uint64_t mnum = (cur_left + cur_right) / 2;
	if (!is_empty(num * 2)) {
		block = allocate(num * 2, cur_left, mnum);
	} else {
		block = allocate(num * 2 + 1, mnum, cur_right);
	}
	set_empty(num, is_empty(num * 2) & is_empty(num * 2 + 1));
	set_full(num, 0);
	return block;
}

void *buddy_allocate() {
	if (is_empty(1)) {
		return BAD_POINTER;
	}
	return (void*)(allocate(1, 0, MAX_BLOCKS) * BLOCK_SIZE);
}