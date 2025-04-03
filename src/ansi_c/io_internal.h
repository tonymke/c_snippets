#ifndef IO_INTERNAL_H
#define IO_INTERNAL_H

/* C90 7.9.6.1
 * Environmental limit
 * The minimum value for the maximum number of characters produced by a single
 * conversion shall be 509. */
#define CONV_BUF_CAP 512
#define CONV_BUF_MAX_LEN (CONV_BUF_CAP - 1)

#define FLAG_ALTFORM (1 << 0)
#define FLAG_LEADZERO (1 << 1)
#define FLAG_LJUST (1 << 2)
#define FLAG_SIGN (1 << 3)
#define FLAG_SPACE (1 << 4)

#define FLAG_ALTFORM_CH '#'
#define FLAG_LEADZERO_CH '0'
#define FLAG_LJUST_CH '-'
#define FLAG_SIGN_CH '+'
#define FLAG_SPACE_CH ' '

enum conv_spec {
	CONV_SPEC_FLT_AUTO, /* g */
	CONV_SPEC_FLT_AUTO_UP, /* G */
	CONV_SPEC_FLT_DEC, /* f */
	CONV_SPEC_FLT_SCI, /* e */
	CONV_SPEC_FLT_SCI_UP, /* E */
	CONV_SPEC_OUT_WRITTEN, /* n */
	CONV_SPEC_PERCENT_LITERAL, /* % */
	CONV_SPEC_PTR, /* p */
	CONV_SPEC_SINT_AUTO, /* i */
	CONV_SPEC_SINT_DEC, /* d */
	CONV_SPEC_STR, /* s */
	CONV_SPEC_UCHAR, /* c */
	CONV_SPEC_UINT_DEC, /* u */
	CONV_SPEC_UINT_HEX, /* x */
	CONV_SPEC_UINT_HEX_UP, /* X */
	CONV_SPEC_UINT_OCT /* o */
};

#define CONV_SPEC_FLT_AUTO_CH 'g'
#define CONV_SPEC_FLT_AUTO_UP_CH 'G'
#define CONV_SPEC_FLT_DEC_CH 'f'
#define CONV_SPEC_FLT_SCI_CH 'e'
#define CONV_SPEC_FLT_SCI_UP_CH 'E'
#define CONV_SPEC_OUT_WRITTEN_CH 'n'
#define CONV_SPEC_PERCENT_LITERAL_CH '%'
#define CONV_SPEC_PTR_CH 'p'
#define CONV_SPEC_SINT_AUTO_CH 'i'
#define CONV_SPEC_SINT_DEC_CH 'd'
#define CONV_SPEC_STR_CH 's'
#define CONV_SPEC_UCHAR_CH 'c'
#define CONV_SPEC_UINT_DEC_CH 'u'
#define CONV_SPEC_UINT_HEX_CH 'x'
#define CONV_SPEC_UINT_HEX_UP_CH 'X'
#define CONV_SPEC_UINT_OCT_CH 'o'

#define FIELD_WIDTH_NEXT_ARGUMENT_CH '*'

enum length_mod {
	LENGTH_MOD_UNSPEC,

	LENGTH_MOD_LONG, /* l */
	LENGTH_MOD_LONGDBL, /* L */
	LENGTH_MOD_SHORT /* h */
};

#define LENGTH_MOD_LONG_CH 'l'
#define LENGTH_MOD_LONGDBL_CH 'L'
#define LENGTH_MOD_SHORT_CH 'h'

#define PRECISION_CH '.'
#define PRECISION_WIDTH_NEXT_ARGUMENT_CH '*'

#endif /* IO_INTERNAL_H */
