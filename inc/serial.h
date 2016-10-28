#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <ioport.h>
#include <stdint.h>

#define BIT(n) (1 << (n))
#define SERIAL_PORT(n) (0x3f8 + n)

void init_serial_port(void);

void putc(char c);

void puts(const char* s);

#endif /* __SERIAL_H__ */
