#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "idt_init.h"
#include "memmap.h"
#include "print.h"
#include "desc.h"
#include "ioport.h"

#pragma pack(push, 1)
struct page_descriptor {
            int next_page_id;
            int prev_page_id;
        void* slab;
    unsigned int in_list : 1;
    unsigned int order : 6;
};
#pragma pack(pop)

struct page_descriptor* descriptors;
int* head;
int max_order;

size_t boot_size;
void* boot_mem;

void allocator_init();

void* get_page(int k);	
void free_page(void* page_addr, int k);

void* get_mem(size_t mem_size, size_t alignment);

#endif /* __ALLOCATOR_H__ */