#include <assert.h>
#include <math.h> /* for log10 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strchr */
#include <time.h>

#include "log.h"
#include "str.h" /* for str_rstrip */

#ifndef LOG_FILE_INITIAL
#define LOG_FILE_INITIAL stderr
#endif

#ifndef LOG_LEVEL_INITIAL
#define LOG_LEVEL_INITIAL LOG_LEVEL_WARNING
#endif

static FILE *current_file = NULL;
static enum log_level current_lvl = LOG_LEVEL_INITIAL;

static unsigned count_digits(unsigned int n);
static int is_valid_log_level(enum log_level lvl);
static const char *log_level_s(enum log_level lvl);

int log_emit(enum log_level lvl, const char *file, unsigned lineno,
	     const char *fmt, ...)
{
#ifndef LOG_EMIT_DEFAULT_BUF_SIZE
#define LOG_EMIT_DEFAULT_BUF_SIZE 1024
#endif
	static char default_buf[LOG_EMIT_DEFAULT_BUF_SIZE];
	int res = -1, written;
	va_list ap;
	time_t now;
	FILE *f;
	char *buf = default_buf, *now_s;
	size_t buf_needed = 0, buf_size = sizeof(default_buf);
	const char *lvl_s;

	/* Try our gosh darndest to avoid log interleaving by writing in a single
	 * write. On POSIX to stderr, this is nigh-guaranteed if the FILE is
	 * unbuffered - which stderr typically is - and the write is under
	 * PIPE_BUF in size. */

	assert(is_valid_log_level(lvl));
	assert(file != NULL);
	assert(fmt != NULL);

	if (lvl > log_level()) {
		goto success;
	}

	f = log_file();
	assert(f != NULL);

	/* Gather all fields to write */
	/* C90 does not have snprintf, so we write to our buf Very Carefully (tm) */
	buf_needed += 1; /* [ */
	now = time(NULL);
	if (now == (time_t)-1) {
		goto error;
	}
	now_s = ctime(&now);
	if (now_s == NULL) {
		goto error;
	}
	buf_needed += str_rstrip(now_s); /* time */
	assert(strchr(now_s, '%') == NULL);
	buf_needed += 1; /* ] */

	buf_needed += 1; /* space separator */

	lvl_s = log_level_s(lvl);
	assert(lvl_s != NULL);
	assert(strchr(lvl_s, '%') == NULL);
	buf_needed += strlen(lvl_s);

	buf_needed += 1; /* space separator */

	/* file:lineno */
	buf_needed += strlen(file);
	assert(strchr(file, '%') == NULL);
	buf_needed += 1; /* colon */
	buf_needed += count_digits(lineno);

	buf_needed += 1; /* space separator */

	/* passed fmt string */
	buf_needed += strlen(fmt);
	buf_needed += 1; /* linefeed */
	buf_needed += 1; /* nul terminator */

	if (buf_needed > buf_size) {
		buf = malloc(buf_needed);
		if (buf == NULL) {
			goto error;
		}
		buf_size = buf_needed;
	}

	written = sprintf(buf, "[%s] %s %s:%d %s\n", now_s, lvl_s, file, lineno,
			  fmt);
	if (written < 0) {
		goto error;
	}
	assert((size_t)written < buf_size);

	va_start(ap, fmt);
	written = vfprintf(f, buf, ap);
	va_end(ap);
	if (written < 0) {
		goto error;
	}

	if (fflush(f) == EOF) {
		goto error;
	}

success:
	res = 0;
error:
	if (buf != default_buf) {
		free(buf);
	}
	return res;
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

FILE *log_file(void)
{
	if (current_file == NULL) {
		log_file_set(LOG_FILE_INITIAL);
	}
	return current_file;
}

void log_file_set(FILE *f)
{
	assert(f != NULL);
	current_file = f;
}

static unsigned count_digits(unsigned int n)
{
	if (n == 0) {
		return 1;
	}
	return (unsigned int)floor(log10((double)n)) + 1;
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
