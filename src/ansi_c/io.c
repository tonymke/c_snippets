#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>

#include "io.h"
#include "io_internal.h"

int io_snprintf(char *str, size_t size, const char *format, ...)
{
	int res;
	va_list ap;

	va_start(ap, format);
	res = io_vsnprintf(str, size, format, ap);
	va_end(ap);

	return res;
}

int io_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
	(void)str;
	(void)size;
	(void)format;
	(void)ap;
	abort(); /* TODO */
}
