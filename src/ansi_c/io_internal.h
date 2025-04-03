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

enum conv_type {
	CONV_TYPE_FLT_AUTO, /* g */
	CONV_TYPE_FLT_AUTO_UP, /* G */
	CONV_TYPE_FLT_DEC, /* f */
	CONV_TYPE_FLT_SCI, /* e */
	CONV_TYPE_FLT_SCI_UP, /* E */
	CONV_TYPE_OUT_WRITTEN, /* n */
	CONV_TYPE_PERCENT_LITERAL, /* % */
	CONV_TYPE_PTR, /* p */
	CONV_TYPE_SINT_AUTO, /* i */
	CONV_TYPE_SINT_DEC, /* d */
	CONV_TYPE_STR, /* s */
	CONV_TYPE_UCHAR, /* c */
	CONV_TYPE_UINT_DEC, /* u */
	CONV_TYPE_UINT_HEX, /* x */
	CONV_TYPE_UINT_HEX_UP, /* X */
	CONV_TYPE_UINT_OCT /* o */
};

#define CONV_TYPE_FLT_AUTO_CH 'g'
#define CONV_TYPE_FLT_AUTO_UP_CH 'G'
#define CONV_TYPE_FLT_DEC_CH 'f'
#define CONV_TYPE_FLT_SCI_CH 'e'
#define CONV_TYPE_FLT_SCI_UP_CH 'E'
#define CONV_TYPE_OUT_WRITTEN_CH 'n'
#define CONV_TYPE_PERCENT_LITERAL_CH '%'
#define CONV_TYPE_PTR_CH 'p'
#define CONV_TYPE_SINT_AUTO_CH 'i'
#define CONV_TYPE_SINT_DEC_CH 'd'
#define CONV_TYPE_STR_CH 's'
#define CONV_TYPE_UCHAR_CH 'c'
#define CONV_TYPE_UINT_DEC_CH 'u'
#define CONV_TYPE_UINT_HEX_CH 'x'
#define CONV_TYPE_UINT_HEX_UP_CH 'X'
#define CONV_TYPE_UINT_OCT_CH 'o'

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
