#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>

/* Duplicate the given string. */
char *strutl_dup(const char *s);

/* Remove leading whitespace from the given string, returning its new length. */
size_t strutl_lstrip(char *s);

/* Remove leading whitespace from the given string, returning its new length. */
size_t strutl_rstrip(char *s);

/* Remove leading and trailing whitespace from the given string, returning its
 * new length. */
size_t strutl_strip(char *s);

#endif /* STRING_UTILS_H */
