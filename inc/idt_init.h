#ifndef __IDT_INIT_H__
#define __IDT_INIT_H__

#include <stdint.h>

void create_rec(uint64_t, uint16_t, uint16_t, uint16_t, uint8_t);
void set_table();
void set_master_mask();
void set_slave_mask();
void master_EOI();
void slave_EOI();
void master_mask_all();
void slave_mask_all();
void master_open_port(int);
void slave_open_port(int);
void master_close_port(int);
void slave_close_port(int);
void set_controller();
void init_all();
void interruption(uint8_t);

#endif /*__IDT_INIT_H__*/