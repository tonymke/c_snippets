#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>

#include "io.h"
#include "io_internal.h"

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
