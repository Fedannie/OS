#ifndef __PRINT_H__
#define __PRINT_H__

#include <stdarg.h>
#include <stddef.h>

int vprintf(const char *format, va_list arg);
int printf(const char *format, ...);

int vsnprintf (char *s, size_t n, const char *format, va_list arg);
int snprintf (char *s, size_t n, const char *format, ...);
#endif /*__PRINT_H__*/
