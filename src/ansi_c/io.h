#ifndef IO_H
#define IO_H

#include <stdarg.h>
#include <stddef.h>

/* Backport of C99's snprintf */
int io_snprintf(char *str, size_t size, const char *format, ...);

/* Backport of C99's vsnprintf */
int io_vsnprintf(char *str, size_t size, const char *format, va_list ap);

#endif
