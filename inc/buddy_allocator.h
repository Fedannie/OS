#ifndef __BUDDY_ALLOCATOR__

#define __BUDDY_ALLOCATOR__

#include <memmap.h>

void initialize_buddy_allocator();
void* buddy_alloc(uint64_t);
void buddy_free(void*);

#endif

