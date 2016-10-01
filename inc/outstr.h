#ifndef __OUTSTR_H__
#define __OUTSTR_H__

#include <stdint.h>

void set_div_to_write();
void write_char(char *);
int get_permission_to_write();
void write_str(char *);
void write_string(char *);
void write_number(uint8_t);

#endif /* __OUTSTR_H__ */