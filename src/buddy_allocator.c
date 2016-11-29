#include <buddy_allocator.h>

#define MAX_CNT_LISTS 20

typedef struct buddy_node buddy_node_t;

struct buddy_node {
	uint8_t free;
	uint8_t ord;

	buddy_node_t *left, *right;
}__attribute__((packed));

uint64_t first_page;
buddy_node_t * lists_of_blocks[MAX_CNT_LISTS];
buddy_node_t * descriptors; 

uint64_t descriptors_size = 0;

uint64_t logical_addr(uint64_t addr) {
	return addr + 0xffff800000000000ull;
}

void add_buddy_tree(uint64_t size) {
	for (int k = MAX_CNT_LISTS; k >= 0; --k) {
		if (size & (1ull << k)) {
			buddy_node_t* new_node = descriptors + descriptors_size;

			new_node->ord = k;
			new_node->free = 1;
			new_node->left = NULL;
			new_node->right = NULL;

			lists_of_blocks[k] = new_node;
			descriptors_size += (1 << k);
		}	
	}
}

my_mem_entry_t* get_max_free_block() {
	my_mem_entry_t* max_block = NULL;
	uint64_t max_length = 0;
	for (my_mem_entry_t* ptr = memory_map; ptr < memory_map + memory_map_size; ++ptr) {
		if (ptr->type == 2) continue;
		if (ptr->length > max_length) {
			max_length = ptr->length;
			max_block = ptr;
		}
	}
	return max_block;
}

void initialize_buddy_allocator() {

	my_mem_entry_t* max_block = get_max_free_block();
	if (!max_block) {	
		printf("No memory.\n");
	}

	uint64_t wanted_number_of_descriptors = max_block->length / (sizeof(buddy_node_t) + PAGE_SIZE); 
	uint64_t wanted_memory = wanted_number_of_descriptors * sizeof(buddy_node_t);

	if (wanted_number_of_descriptors < 1) {
		printf("Not enough memory.\n");
	}

	uint64_t phys_first_page = (max_block->begin + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE;
	first_page = logical_addr(phys_first_page);
	uint64_t number_of_blocks = (max_block->end - phys_first_page + 1 - wanted_memory) / PAGE_SIZE;

	descriptors = (buddy_node_t*)(logical_addr(phys_first_page + number_of_blocks * PAGE_SIZE));

	add_buddy_tree(number_of_blocks);

}

buddy_node_t* buddy(buddy_node_t* node) {
	uint64_t num_of_descriptor = node - descriptors;		
	uint64_t num_of_buddy_descriptor = num_of_descriptor ^ (1 << (node->ord));
	return descriptors + num_of_buddy_descriptor;
}

buddy_node_t* find_descriptor(uint8_t cur_ord, uint8_t wanted_ord) {

	buddy_node_t* node = lists_of_blocks[cur_ord];
	
	if (cur_ord == wanted_ord) return node;

	lists_of_blocks[cur_ord] = lists_of_blocks[cur_ord]->right;
		
	buddy_node_t* buddy_node = buddy(node);

	lists_of_blocks[cur_ord - 1] = node;
	node->free = 1;
	buddy_node->free = 1;
	node->right = buddy_node;
	node->left = NULL;
	buddy_node->right = NULL;
	buddy_node->left = node;	
	node->ord = cur_ord - 1;
	buddy_node->ord = cur_ord - 1;


	return find_descriptor(cur_ord - 1, wanted_ord);
}

void* buddy_alloc(uint64_t size) {
	size = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	if (size > (1ull << MAX_CNT_LISTS)) return 0;

	uint8_t k = 0;
	while ((1ull << k) < size) k++;

	uint8_t l = k;
	while (!lists_of_blocks[l] && l < MAX_CNT_LISTS) ++l;

	if (l == MAX_CNT_LISTS) return 0;

	buddy_node_t* descriptor = find_descriptor(l, k);
	descriptor->free = 0;
	
	return (void*)(first_page + (descriptor - descriptors) * PAGE_SIZE);

}

void insert(buddy_node_t* node, int k) {
	node->right = lists_of_blocks[k];
	if (lists_of_blocks[k]) {
		lists_of_blocks[k]->left = node;
	}
	node->left = NULL;
	node->ord = k;
}	

void erase(buddy_node_t* node, int k) {
	if (node->left) {
		node->left->right = node->right;
	}
	if (node->right) {
		node->right->left = node->left;
	}
	if (!node->left && !node->right) {
		lists_of_blocks[k] = NULL;
	}
	node->left = NULL;
	node->right = NULL;
}

void buddy_free(void* void_ptr) {
	uint64_t ptr = (uint64_t)void_ptr;
	buddy_node_t* descriptor = descriptors + (ptr - first_page) / PAGE_SIZE;

	descriptor->free = 1;
	buddy_node_t* buddy_descriptor = buddy(descriptor);	

	while (buddy_descriptor->free) {
		erase(descriptor, descriptor->ord);
		erase(buddy_descriptor, descriptor->ord);
		insert(descriptor, descriptor->ord+1);
		buddy_descriptor = buddy(descriptor);	
	}

}






