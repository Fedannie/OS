#include "desc.h"
#include "memory.h"
#include "outstr.h"
#include "ioport.h"

#define MASTER_COMAND_PORT 0x20 
#define MASTER_DATA_PORT 0x21
#define SLAVE_COMAND_PORT 0xa0
#define SLAVE_DATA_PORT 0xa1

uint8_t master_mask = 0;
uint8_t slave_mask = 0;

extern uint64_t handlers[];
static struct IDT_rec idt_table[TABLE_SIZE];

void create_rec(uint64_t offset, uint16_t segment_selector, uint16_t p, uint16_t type, uint8_t i) {
	idt_table[i].offset1 = (offset & 0xffff);
	idt_table[i].segment_selector = segment_selector;
	idt_table[i].its_magic_type_magic_p = ((p << 15) | (type << 8));
	idt_table[i].offset2 = (offset >> 16) & 0xffff;
	idt_table[i].offset3 = offset >> 32;
	idt_table[i].magic = 0;
}

void set_table() {
	for (uint8_t i = 0; i < TABLE_SIZE; i++) {
		create_rec(handlers[i] , KERNEL_CS, 1, 14, i);
	}
}

void set_master_mask() {
	out8(MASTER_DATA_PORT, master_mask);
}

void set_slave_mask() {
	out8(SLAVE_DATA_PORT, slave_mask);
}

void master_EOI(){
	master_mask = (1<<5);
	out8(MASTER_COMAND_PORT, (1<<5));
}

void slave_EOI(){
	slave_mask = (1<<5);
	out8(SLAVE_COMAND_PORT, (1<<5));
}

void master_mask_all(){
	master_mask = (1<<8) - 1;
	set_master_mask();
}

void slave_mask_all(){
	slave_mask = (1<<8) - 1;
	set_slave_mask();
}

void master_close_port(uint8_t port){
	master_mask |= (1<<port);
	set_master_mask();
}

void slave_close_port(uint8_t port){
	slave_mask |= (1<<port);
	set_slave_mask();
}

void master_open_port(uint8_t port){
	master_mask &= (1<<8) - 1 - (1<<port);
	set_master_mask();
}

void slave_open_port(uint8_t port){
	slave_mask &= (1<<8) - 1 - (1<<port);
	set_slave_mask();
}

void set_controller(){
	out8(MASTER_COMAND_PORT, (1<<4) + 1);
	out8(SLAVE_COMAND_PORT, (1<<4) + 1);
	out8(MASTER_DATA_PORT, 32);
	out8(SLAVE_DATA_PORT, 40);
	out8(MASTER_DATA_PORT, 4);
	out8(SLAVE_DATA_PORT, 2);
	out8(MASTER_DATA_PORT, 1);
	out8(SLAVE_DATA_PORT, 1);
	master_mask_all();
	slave_mask_all();	
	master_open_port(2);
}

void init_all() {
	write_string("Start initing\n");
	set_table();
	write_string("Table is ready\n");
	struct desc_table_ptr ptr = {sizeof(idt_table) - 1, (uint64_t) idt_table};
	write_idtr(&ptr);
	write_string("Ptr is ready\n");
	set_controller();
	write_string("Controller is ready\n");
}

void interruption(uint8_t num) {
	write_string("Interrupt number ");
	write_number(num);
	write_string("\n");
	if (num == FIRST_REC_START) {
		write_string("Timer works! We've got it's interruption.\n");
		master_EOI();
	} else if (num < FIRST_REC_START) {
		write_string("System interrupt\n");
	} else if (num >= FIRST_REC_START && num < FIRST_REC_END) {
		master_EOI();
		write_string("Master int\n");
	} else if (num >= SECOND_REC_START && num < SECOND_REC_END){
		master_EOI();
		slave_EOI();
		write_string("Slave int \n");
	}
}