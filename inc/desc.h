#ifndef __DESC_H__
#define __DESC_H__

#include <stdint.h>
#define TABLE_SIZE 50
#define FIRST_REC_START 32
#define FIRST_REC_END 40
#define SECOND_REC_START 40
#define SECOND_REC_END 49

struct desc_table_ptr {
	uint16_t size;
	uint64_t addr;
} __attribute__((packed));

struct IDT_rec {
	uint16_t offset1;
	uint16_t segment_selector;
	uint16_t its_magic_type_magic_p;
	uint16_t offset2;
	uint32_t offset3;
	uint32_t magic;
} __attribute__((packed));

//uint64_t handlers[TABLE_SIZE];
//extern struct IDT_rec idt_table[TABLE_SIZE];

static inline void read_idtr(struct desc_table_ptr *ptr)
{
	__asm__ ("sidt %0" : "=m"(*ptr));
}

static inline void write_idtr(const struct desc_table_ptr *ptr)
{
	__asm__ ("lidt %0" : : "m"(*ptr));
}

static inline void read_gdtr(struct desc_table_ptr *ptr)
{
	__asm__ ("sgdt %0" : "=m"(*ptr));
}

static inline void write_gdtr(const struct desc_table_ptr *ptr)
{
	__asm__ ("lgdt %0" : : "m"(*ptr));
}
#endif /*__DESC_H__*/
