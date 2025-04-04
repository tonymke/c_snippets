#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "io_internal.h"

static const char *parse_conv_spec_flags(const char *s,
					 struct conv_spec *out_cs);
static const char *parse_conv_spec_width(const char *s,
					 struct conv_spec *out_cs);
static const char *parse_conv_spec_precision(const char *s,
					     struct conv_spec *out_cs);
static const char *parse_conv_spec_lengthmod(const char *s,
					     struct conv_spec *out_cs);
static const char *parse_conv_spec_type(const char *s,
					struct conv_spec *out_cs);

/* io.h implementations */

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

/* io_internal.h implementations */

int is_conv_spec_valid(const struct conv_spec *cs)
{
	if (cs == NULL) {
		assert(!"null conv_spec ptr");
		return 0;
	}

	/* Altform only has defined behavior with types { xXeEfgG }*/
	if (cs->flag_altform) {
		switch (cs->type) {
		case CONV_TYPE_UINT_HEX: /* x */
		case CONV_TYPE_UINT_HEX_UP: /* X */
		case CONV_TYPE_FLT_SCI: /* e */
		case CONV_TYPE_FLT_SCI_UP: /* E */
		case CONV_TYPE_FLT_DEC: /* f */
		case CONV_TYPE_FLT_AUTO: /* g */
		case CONV_TYPE_FLT_AUTO_UP: /* G */
			/* okay */
			break;
		default:
			assert(!"# flag with unsupported type");
			return 0;
		}
	}

	/* If leadzero and ljust are both defined, leadzero is ignored. While not
	 * undefined behavior, let's be pedantic with ourselves when asserting
	 * only. */
	if (cs->flag_leadzero && cs->flag_ljust) {
		assert(!"0 flag with - flag ignores 0");
		/* still valid */
	}

	/* Leadzero only has defined behavior with types { diouxXeEfgG } If one
	 * of { diouxX } and a precision is specified, 0 is ignored. While the
	 * latter is not undefined behavior, let's be pedantic with ourselves
	 * when asserting only.
	 */
	if (cs->flag_leadzero) {
		switch (cs->type) {
		case CONV_TYPE_SINT_DEC: /* d */
		case CONV_TYPE_SINT_AUTO: /* i */
		case CONV_TYPE_UINT_OCT: /* o */
		case CONV_TYPE_UINT_DEC: /* u */
		case CONV_TYPE_UINT_HEX: /* x */
		case CONV_TYPE_UINT_HEX_UP: /* X */
			if (cs->precision.is_specified) {
				assert(!"0 flag with int type and precision ignores 0");
				/* still valid */
			}

			/* fallthrough desired */
		case CONV_TYPE_FLT_AUTO: /* g */
		case CONV_TYPE_FLT_AUTO_UP: /* G */
		case CONV_TYPE_FLT_DEC: /* f */
		case CONV_TYPE_FLT_SCI: /* e */
		case CONV_TYPE_FLT_SCI_UP: /* E */
			/* okay */
			break;
		default:
			assert(!"0 flag with unsupported type");
			return 0;
		}
	}

	/* Length mod only has defined behavior with certain types. */
	/* - If there is an int length mod, ensure type in { dionuxX } */
	if (cs->length_mod == LENGTH_MOD_SHORT ||
	    cs->length_mod == LENGTH_MOD_LONG) {
		switch (cs->type) {
		case CONV_TYPE_SINT_DEC: /* d */
		case CONV_TYPE_SINT_AUTO: /* i */
		case CONV_TYPE_UINT_OCT: /* o */
		case CONV_TYPE_OUT_WRITTEN: /* n */
		case CONV_TYPE_UINT_DEC: /* u */
		case CONV_TYPE_UINT_HEX: /* x */
		case CONV_TYPE_UINT_HEX_UP: /* X */
			/* okay */
			break;
		default:
			assert(!"int length mod with non-int type");
			return 0;
		}
	}
	/* - If there is a double length mod, ensure type in { eEfgG } */
	else if (cs->length_mod == LENGTH_MOD_LONGDBL) {
		switch (cs->type) {
		case CONV_TYPE_FLT_SCI: /* e */
		case CONV_TYPE_FLT_SCI_UP: /* E */
		case CONV_TYPE_FLT_DEC: /* f */
		case CONV_TYPE_FLT_AUTO: /* g */
		case CONV_TYPE_FLT_AUTO_UP: /* G */
			/* okay */
			break;
		default:
			assert(!"double length mod with non-float type");
			return 0;
		}
	}

	/* Precision only has defined behavior with types { diouxXeEfgGs }. */
	if (cs->precision.is_specified) {
		switch (cs->type) {
		case CONV_TYPE_FLT_AUTO: /* g */
		case CONV_TYPE_FLT_AUTO_UP: /* G */
		case CONV_TYPE_FLT_DEC: /* f */
		case CONV_TYPE_FLT_SCI: /* e */
		case CONV_TYPE_FLT_SCI_UP: /* E */
		case CONV_TYPE_SINT_AUTO: /* i */
		case CONV_TYPE_SINT_DEC: /* d */
		case CONV_TYPE_STR: /* s */
		case CONV_TYPE_UINT_DEC: /* u */
		case CONV_TYPE_UINT_HEX: /* x */
		case CONV_TYPE_UINT_HEX_UP: /* X */
		case CONV_TYPE_UINT_OCT: /* o */
			/* okay */
			break;
		default:
			assert(!"precision specified with unsupported type");
			return 0;
		}
	}

	return 1;
}

int parse_conv_spec(const char *s, struct conv_spec *out_cs,
		    const char **out_next)
{
	enum state { FLAGS, WIDTH, PRECISION, LENGTH_MOD, TYPE, DONE } state;

	if (s == NULL || out_cs == NULL) {
		return -1;
	}

	if (*s++ != CONV_SPEC_START_CH) {
		return -1;
	}

	memset(out_cs, 0, sizeof(*out_cs));

	if (*s == CONV_SPEC_START_CH) {
		/* edge case - percent literal.  no other spec chars allowed. */
		out_cs->type = CONV_TYPE_PERCENT_LITERAL;
		return 0;
	} else {
		state = FLAGS;
	}

	while (*s != '\0') {
		const char *next_s;

		if (state == FLAGS) {
			next_s = parse_conv_spec_flags(s, out_cs);
			state = WIDTH;
		} else if (state == WIDTH) {
			next_s = parse_conv_spec_width(s, out_cs);
			state = PRECISION;
		} else if (state == PRECISION) {
			next_s = parse_conv_spec_precision(s, out_cs);
			state = LENGTH_MOD;
		} else if (state == LENGTH_MOD) {
			next_s = parse_conv_spec_lengthmod(s, out_cs);
			state = TYPE;
		} else if (state == TYPE) {
			next_s = parse_conv_spec_type(s, out_cs);
			state = DONE;
		} else if (state == DONE) {
			break;
		} else {
			assert(!"unexpected state");
			abort();
		}

		if (next_s == NULL) {
			return -1;
		}

		assert(s < next_s);
		s = next_s;
	}

	if (out_next != NULL) {
		*out_next = s;
	}

	if (state == DONE && is_conv_spec_valid(out_cs)) {
		return 0;
	}
	return -1;
}

/* static fn implementations */

static const char *parse_conv_spec_flags(const char *s,
					 struct conv_spec *out_cs)
{
	assert(s != NULL);
	assert(out_cs != NULL);

	while (*s != '\0') {
		switch (*s) {
		case FLAG_ALTFORM_CH:
			out_cs->flag_altform = 1;
			break;
		case FLAG_LEADZERO_CH:
			out_cs->flag_leadzero = 1;
			break;
		case FLAG_LJUST_CH:
			out_cs->flag_ljust = 1;
			break;
		case FLAG_SIGN_CH:
			out_cs->flag_sign = 1;
			break;
		case FLAG_SPACE_CH:
			out_cs->flag_space = 1;
			break;
		default:
			goto exit;
		}

		s++;
	}
exit:
	return s;
}

static const char *parse_conv_spec_width(const char *s,
					 struct conv_spec *out_cs)
{
	char buf[21]; /* max length of 64 bit int in base10 is 20 */
	size_t buf_i = 0;
	long val;

	assert(s != NULL);
	assert(out_cs != NULL);

	if (*s == FIELD_WIDTH_NEXT_ARGUMENT_CH) {
		/* edge case - * width */
		out_cs->min_width.is_specified = 1;
		out_cs->min_width.use_next_arg = 1;
		return ++s;
	}

	while (*s != '\0') {
		if (buf_i >= sizeof(buf) - 1) {
			/* edge case - buffer overflow */
			return NULL;
		}

		if (!isdigit(*s)) {
			break;
		}
		buf[buf_i++] = *s++;
	}

	if (buf_i) {
		/* digits found */
		buf[buf_i] = '\0';
		val = atol(buf);
		out_cs->min_width.is_specified = 1;
		out_cs->min_width.val = (unsigned long)val;
	}

	return s;
}

static const char *parse_conv_spec_precision(const char *s,
					     struct conv_spec *out_cs)
{
	char buf[21]; /* max length of 64 bit int in base10 is 20 */
	size_t buf_i = 0;
	long val;

	assert(s != NULL);
	assert(out_cs != NULL);

	if (*s != PRECISION_SEP_CH) {
		/* no precision to read */
		return s;
	}
	s++;

	if (*s == PRECISION_WIDTH_NEXT_ARGUMENT_CH) {
		/* edge case - * width */
		out_cs->precision.is_specified = 1;
		out_cs->precision.use_next_arg = 1;
		return ++s;
	}

	while (*s != '\0') {
		if (buf_i >= sizeof(buf) - 1) {
			/* edge case - buffer overflow */
			return NULL;
		}

		if (!isdigit(*s)) {
			break;
		}
		buf[buf_i++] = *s++;
	}

	if (buf_i) {
		/* digits found */
		buf[buf_i] = '\0';
		val = atol(buf);
		out_cs->precision.is_specified = 1;
		out_cs->precision.val = (unsigned long)val;
	} else {
		/* no digits means precision 0 */
		out_cs->precision.is_specified = 1;
	}

	return s;
}

static const char *parse_conv_spec_lengthmod(const char *s,
					     struct conv_spec *out_cs)
{
	assert(s != NULL);
	assert(out_cs != NULL);

	switch (*s++) {
	case LENGTH_MOD_LONG_CH:
		out_cs->length_mod = LENGTH_MOD_LONG;
		break;
	case LENGTH_MOD_LONGDBL_CH:
		out_cs->length_mod = LENGTH_MOD_LONGDBL;
		break;
	case LENGTH_MOD_SHORT_CH:
		out_cs->length_mod = LENGTH_MOD_SHORT;
		break;
	}

	return s;
}

static const char *parse_conv_spec_type(const char *s, struct conv_spec *out_cs)
{
	assert(s != NULL);
	assert(out_cs != NULL);

	/* CONV_TYPE_PERCENT_LITERAL_CH: omitted - handled in parse_conv_spec */
	switch (*s++) {
	case CONV_TYPE_FLT_AUTO_CH:
		out_cs->type = CONV_TYPE_FLT_AUTO;
		break;
	case CONV_TYPE_FLT_AUTO_UP_CH:
		out_cs->type = CONV_TYPE_FLT_AUTO_UP;
		break;
	case CONV_TYPE_FLT_DEC_CH:
		out_cs->type = CONV_TYPE_FLT_DEC;
		break;
	case CONV_TYPE_FLT_SCI_CH:
		out_cs->type = CONV_TYPE_FLT_SCI;
		break;
	case CONV_TYPE_FLT_SCI_UP_CH:
		out_cs->type = CONV_TYPE_FLT_SCI_UP;
		break;
	case CONV_TYPE_OUT_WRITTEN_CH:
		out_cs->type = CONV_TYPE_OUT_WRITTEN;
		break;
	case CONV_TYPE_PTR_CH:
		out_cs->type = CONV_TYPE_PTR;
		break;
	case CONV_TYPE_SINT_AUTO_CH:
		out_cs->type = CONV_TYPE_SINT_AUTO;
		break;
	case CONV_TYPE_SINT_DEC_CH:
		out_cs->type = CONV_TYPE_SINT_DEC;
		break;
	case CONV_TYPE_STR_CH:
		out_cs->type = CONV_TYPE_STR;
		break;
	case CONV_TYPE_UCHAR_CH:
		out_cs->type = CONV_TYPE_UCHAR;
		break;
	case CONV_TYPE_UINT_DEC_CH:
		out_cs->type = CONV_TYPE_UINT_DEC;
		break;
	case CONV_TYPE_UINT_HEX_CH:
		out_cs->type = CONV_TYPE_UINT_HEX;
		break;
	case CONV_TYPE_UINT_HEX_UP_CH:
		out_cs->type = CONV_TYPE_UINT_HEX_UP;
		break;
	case CONV_TYPE_UINT_OCT_CH:
		out_cs->type = CONV_TYPE_UINT_OCT;
		break;
	default:
		/* invalid type */
		return NULL;
	}

	return s;
}
