#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "logging.h"

#ifndef LOGGING_LEVEL_INITIAL
#define LOGGING_LEVEL_INITIAL LOG_LEVEL_WARNING
#endif

static enum log_level current_lvl = LOGGING_LEVEL_INITIAL;

static int is_valid_log_level(enum log_level lvl);
static const char *log_level_s(enum log_level lvl);
static size_t digit_count(unsigned n);

int log_emit(enum log_level lvl, const char *file, unsigned lineno,
	     const char *fmt, ...)
{
#ifndef LOGGING_LOG_METADATA_BUF_CAP
#define LOGGING_LOG_METADATA_BUF_CAP 512U
#endif
	static char metadata_buf[LOGGING_LOG_METADATA_BUF_CAP];
	const char *ctime_s, *lvl_s;
	va_list ap;
	time_t now;
	size_t needed_len, tot_needed_len = 2; /* terminating linebreak + \0 */
	int written;

	assert(is_valid_log_level(lvl));
	assert(file != NULL);
	assert(fmt != NULL);

	if (lvl > current_lvl) {
		return 0;
	}

	/*
	   Stderr is typically unbuffered, and interleaved log lines are not
	   very useful. As well, we want to provide a way for the use to
	   leverage printf. So, we can't easily just do a single vprintf call
	   and be done with it.

	   Instead, pre-write the log's metadata/attrs and the user's fmt string
	   into a buffer, so we can then call vfprintf against their fmt string.
	   
	   C90 does not have snprintf(3). To prevent an overflow, we therefore
	   need to pre-calculate how much metadata_buf space will be used before
	   calling sprintf.
	*/

	/* ctime string + wrapping brackets */
	now = time(NULL);
	needed_len = 2;
	if (now == (time_t)-1) {
		ctime_s = "";
	} else {
		ctime_s = ctime(&now);
		if (ctime_s == NULL) {
			ctime_s = "";
		} else {
			/* remove the suffixing newline, infer the strlen from
			 * its address if possible */
			char *newline = strchr(ctime_s, '\n');
			if (newline != NULL) {
				*newline = '\0';
				needed_len += (newline - ctime_s);
			} else {
				needed_len += strlen(ctime_s);
			}
		}
	}
	assert((size_t)-1 - tot_needed_len >= needed_len);
	tot_needed_len += needed_len;

	/* level string + prefixing space */
	lvl_s = log_level_s(lvl);
	assert(lvl_s != NULL);
	needed_len = 1 + strlen(lvl_s);
	assert((size_t)-1 - tot_needed_len >= needed_len);
	tot_needed_len += needed_len;

	/* file string + prefixing space */
	needed_len = 1 + strlen(file);
	assert((size_t)-1 - tot_needed_len >= needed_len);
	tot_needed_len += needed_len;

	/* lineno string + prefixing colon */
	needed_len = 1 + digit_count(lineno);
	assert((size_t)-1 - tot_needed_len >= needed_len);
	tot_needed_len += needed_len;

	/* fmt + prefixing colon + suffixing newline */
	needed_len = strlen(fmt);
	assert((size_t)-1 - 2 > needed_len);
	needed_len += 2;
	assert((size_t)-1 - tot_needed_len >= needed_len);
	tot_needed_len += needed_len;

	if (tot_needed_len > sizeof(metadata_buf) - 1) {
		assert(!"log fmt string too long");
		return -1;
	}

	/* write the metadata buf.
	   this becomes "(metadata) (user_fmt_str)\n" */
	written = sprintf(metadata_buf,
			  ("[%s]" /* time */
			   " %s" /* lvl */
			   " %s:%d" /* file:lineno */
			   " %s\n"), /* fmt */
			  ctime_s, lvl_s, file, lineno, fmt);
	if (written < 0) {
		return -1;
	}
	assert((unsigned)written < sizeof(metadata_buf) - 1);

	/*
	Now, use it to printf with the provided fmt str and args all in one
	printf call. If this string was less than PIPE_BUF (512byte min in
	POSIX, 4k on linux) we don't have to care about interleaving at all)
	*/
	va_start(ap, fmt);
	written = vfprintf(stderr, metadata_buf, ap);
	va_end(ap);
	if (written < 0) {
		return -1;
	}

	if (fflush(stderr) == EOF) {
		return -1;
	}

	return 0;
}

enum log_level log_level(void)
{
	assert(is_valid_log_level(current_lvl));
	return current_lvl;
}

void log_level_set(enum log_level lvl)
{
	assert(is_valid_log_level(current_lvl));
	assert(is_valid_log_level(lvl));

	current_lvl = lvl;
}

static int is_valid_log_level(enum log_level lvl)
{
	switch (lvl) {
	case LOG_LEVEL_EMERG:
	case LOG_LEVEL_ALERT:
	case LOG_LEVEL_CRIT:
	case LOG_LEVEL_ERR:
	case LOG_LEVEL_WARNING:
	case LOG_LEVEL_NOTICE:
	case LOG_LEVEL_INFO:
	case LOG_LEVEL_DEBUG:
		return 1;
	default:
		return 0;
	}
}

static const char *log_level_s(enum log_level lvl)
{
	static const char *level_ss[] = { "EMERG",   "ALERT",  "CRIT", "ERR",
					  "WARNING", "NOTICE", "INFO", "DEBUG" };

	assert(is_valid_log_level(lvl));
	return level_ss[lvl];
}

static size_t digit_count(unsigned n)
{
	size_t count = 0;

	if (n == 0) {
		return 1;
	}

	while (n != 0) {
		n /= 10;
		count++;
	}

	return count;
}
