/* Copyright (c) 2017-2026 Griefer@Work                                       *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgement (see the following) in the product     *
 *    documentation is required:                                              *
 *    Portions Copyright (c) 2017-2026 Griefer@Work                           *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 */
/* clang-format off */
#ifndef GUARD_TPP_C
#define GUARD_TPP_C 1
#undef TPP_BUILDING
#define TPP_BUILDING 1
#ifndef TPP_INTERN_DECL
#define TPP_INTERN_DECL static
#define TPP_INTERN_IMPL static
#endif /* !TPP_INTERN_DECL */

# include "tpp-amalgamation.h"

/************************************************************************/
/* File: parts/error.c                                                  */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_STRERROR
/* Return a human-readable descriptor of "error" */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_strerror(tpp_errno error) {
	switch (error) {
	case TPP_EOK:
		return "Success";
	case TPP_ENOMEM:
		return "Out of memory";
	case TPP_EIO:
		return "I/O operation failed";
#if TPP_HAVE_FILE_NONBLOCK
	case TPP_EWOULDBLOCK:
		return "Operation would have blocked";
#endif /* TPP_HAVE_FILE_NONBLOCK */
	case TPP_ENOENT:
		return "No such file or directory";
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
	case TPP_EMASKED:
		return "File cannot be opened because it has been masked";
#endif /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
#if TPP_HAVE_WARNINGS
	case TPP_ELEXERROR:
		return "User compilation/lexer error";
	case TPP_EWARNPRINT:
		return "Error while printing warning";
#endif /* TPP_HAVE_WARNINGS */
	default:
		return "Unknown error";
	}
}
#endif /* TPP_HAVE_STRERROR */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/ctype.c                                                  */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_BUILTIN_CTYPE
TPP_CONST_IMPL uint_least8_t const tpp_ctype[256] = {
	0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x10,0x04,0x04,0x10,0x04,0x04,
	0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
	0x04,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x03,
	0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
};
#endif /* TPP_HAVE_BUILTIN_CTYPE */


/* Convert an integer into a string */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL
tpp_utoa(char buf[TPP_UTOA_MAXLEN], tpp_uintmax value) {
	char *result = buf + TPP_UTOA_MAXLEN;
	do {
		uint_least8_t digit;
		digit = (uint_least8_t)(value % 10);
		value = value / 10;
		*--result = (char)('0' + digit);
	} while (value != 0);
	return result;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL
tpp_itoa(char buf[TPP_ITOA_MAXLEN], tpp_intmax value) {
	char *result;
	bool is_neg = value < 0;
	if (is_neg)
		value = -value;
	result = tpp_utoa(buf + TPP_ITOA_MAXLEN - TPP_UTOA_MAXLEN, value);
	if (is_neg)
		*--result = '-';
	return result;
}


/* Convert a float into a string */
#if TPP_HAVE_FTOA
TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL
tpp_inplace_utoa(char buf[TPP_UTOA_MAXLEN], tpp_uintmax value) {
	char *p = tpp_utoa(buf, value);
	tpp_size len = (tpp_size)((buf + TPP_UTOA_MAXLEN) - p);
	tpp_memmovedown(buf, p, len);
	return buf + len;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_size TPPCALL
tpp_ftoa(char buf[TPP_FTOA_MAXLEN], tpp_float value) {
	tpp_uintmax whole;
	char *p = buf;
	char *decimal_start;
	tpp_size decimal_len;
	if (value < 0) {
		*p++ = '-';
		value = -value;
	}
	whole = (tpp_uintmax)value;
	p = tpp_inplace_utoa(p, whole);
	value -= (tpp_float)whole;
	whole = (tpp_uintmax)(value * 10000000000.0); /* 10 digits */
	tpp_assert(whole < TPP_UINTMAX_C(10000000000));
	*p++ = '.';
	decimal_start = tpp_utoa(p, whole);
	tpp_assert(decimal_start >= p);
	decimal_len = (tpp_size)((p + TPP_UTOA_MAXLEN) - decimal_start);
	while (decimal_len < 10) {
		*--decimal_start = '0';
		++decimal_len;
	}
	tpp_assert(decimal_start >= p);
	while ((decimal_len > 1) && decimal_start[decimal_len - 1] == '0')
		--decimal_len;
	tpp_memmovedown(decimal_start, p, decimal_len);
	p += decimal_len;
	return (tpp_size)(p - buf);
}
#endif /* TPP_HAVE_FTOA */


#if TPP_HAVE_TPP_UNICODE_WRITEUTF8
/* Encode "uch" as utf-8 into "buf" and return the pointer after the last-written byte. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_unicode_writeutf8(tpp_char buf[TPP_UTF8_MAXLEN], tpp_unichar uc) {
	tpp_char *dst = buf;
	if (uc <= TPP_UTF8_1BYTE_MAX) {
		*dst++ = (uint8_t)uc;
	} else if (uc <= TPP_UTF8_2BYTE_MAX) {
		*dst++ = 0xc0 | (uint8_t)((uc >> 6) /* & 0x1f*/);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_3BYTE_MAX) {
		*dst++ = 0xe0 | (uint8_t)((uc >> 12) /* & 0x0f*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_4BYTE_MAX) {
		*dst++ = 0xf0 | (uint8_t)((uc >> 18) /* & 0x07*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_5BYTE_MAX) {
		*dst++ = 0xf8 | (uint8_t)((uc >> 24) /* & 0x03*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 18) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_6BYTE_MAX) {
		*dst++ = 0xfc | (uint8_t)((uc >> 30) /* & 0x01*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 24) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 18) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else {
		*dst++ = 0xfe;
		*dst++ = 0x80 | (uint8_t)((uc >> 30) & 0x03 /* & 0x3f*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 24) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 18) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	}
	return dst;
}
#endif /* TPP_HAVE_TPP_UNICODE_WRITEUTF8 */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/string.c                                                 */
/************************************************************************/
TPP_DECL_BEGIN

TPP_IMPL TPP_WUNUSED tpp_string *TPPCALL
tpp_string_trymalloc(tpp_size len) {
	tpp_string *result = _tpp_string_trymalloc(len);
	if tpp_likely(result) {
		tpp_refcnt_atomic_init(&result->ts_refcnt, 1);
		result->ts_len      = len;
		result->ts_str[len] = '\0';
	}
	return result;
}

TPP_IMPL TPP_WUNUSED tpp_string *TPPCALL
tpp_string_malloc(tpp_size len) {
	tpp_string *result = _tpp_string_malloc(len);
	if tpp_likely(result) {
		tpp_refcnt_atomic_init(&result->ts_refcnt, 1);
		result->ts_len      = len;
		result->ts_str[len] = '\0';
	}
	return result;
}

TPP_IMPL struct tpp_string_empty_struct _tpp_string_empty = {
	/* .ts_refcnt = */ TPP_REFCNT_ATOMIC_INIT(1),
	/* .ts_len    = */ 0,
	/* .ts_nul    = */ 0,
};


/************************************************************************/
/* STRING BUILDER                                                       */
/************************************************************************/

/* Package "self" into a tpp string and return said string.
 * This function never fails, but it *DOES* finalize "self"
 * iow: DO NOT CALL `tpp_string_builder_fini()' AFTER THIS FUNCTION!
 *
 * @return: * : The string that was written to this builder */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) TPP_REF tpp_string *TPPCALL
tpp_string_builder_pack(/*inherit(always)*/ tpp_string_builder *tpp_restrict self) {
	TPP_REF tpp_string *result;

	/* Deal with special case: empty string */
	if (self->tsb_len == 0) {
		tpp_free(self->tsb_buf);
		return tpp_string_newempty();
	}

	/* Truncate buffer to used length, and initialize reference counter */
	result = self->tsb_buf;
	tpp_assert(result);
	tpp_assert(self->tsb_len <= result->ts_len);
	if (self->tsb_len < result->ts_len) {
		result = _tpp_string_tryrealloc(result, self->tsb_len);
		if tpp_unlikely(!result)
			result = self->tsb_buf;
		result->ts_len = self->tsb_len;
		result->ts_str[result->ts_len] = (tpp_char)'\0';
	}
	tpp_refcnt_atomic_init(&result->ts_refcnt, 1);
	return result;
}

#ifndef TPP_STRING_BUILDER_MINALLOC
#define TPP_STRING_BUILDER_MINALLOC 64
#endif /* !TPP_STRING_BUILDER_MINALLOC */


/* Allocate (and return) an additional buffer of at least "num_bytes" characters,
 * to-be initialized by the caller at the end of all string data that has already
 * been allocated to the given builder.
 *
 * @return: * :   Pointer to the base of a "num_bytes"-bytes
 *                long buffer (to-be initialized by the caller)
 *                This pointer ONLY remains valid until the next
 *                call to this function with the same "self".
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_string_builder_alloc(tpp_string_builder *tpp_restrict self,
                         tpp_size num_bytes) {
	tpp_char *result;
	tpp_string *buffer = self->tsb_buf;
	tpp_size cur_alloc = buffer ? buffer->ts_len : 0;
	tpp_size min_alloc = self->tsb_len + num_bytes;
	tpp_assert(cur_alloc >= self->tsb_len);
	if (cur_alloc < min_alloc) {
		tpp_size new_alloc = cur_alloc * 2;
#if TPP_STRING_BUILDER_MINALLOC != 0
		if (new_alloc < TPP_STRING_BUILDER_MINALLOC)
			new_alloc = TPP_STRING_BUILDER_MINALLOC;
#endif /* TPP_STRING_BUILDER_MINALLOC != 0 */
		if (new_alloc < min_alloc)
			new_alloc = min_alloc;
		buffer = _tpp_string_tryrealloc(buffer, new_alloc);
		if tpp_unlikely(!buffer) {
			new_alloc = min_alloc;
			buffer = _tpp_string_realloc(self->tsb_buf, new_alloc);
			if tpp_unlikely(!buffer)
				return NULL;
		}
		buffer->ts_len = new_alloc;
		self->tsb_buf = buffer;
		tpp_assert(new_alloc >= min_alloc);
	}

	tpp_assert(buffer || !num_bytes);
	result = buffer->ts_str + self->tsb_len;
	self->tsb_len += num_bytes;
	return result;
}

/* Print "text" into "tpp_string_builder *self"
 * @return: num_bytes:            Success
 * @return: (tpp_size)TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED tpp_ssize TPP_FORMATPRINTER_CC
tpp_string_builder_print(void *arg, tpp_char const *text, tpp_size num_bytes) {
	tpp_string_builder *me = (tpp_string_builder *)arg;
	tpp_char *dst = tpp_string_builder_alloc(me, num_bytes);
	if tpp_unlikely(!dst)
		goto err_nomem;
	tpp_memcpy(dst, text, num_bytes);
	return (tpp_size)num_bytes;
err_nomem:
	return (tpp_size)TPP_ENOMEM;
}


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/token-strtokenid.c                                       */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_STRTOKENID

static struct tpp_token_str_strings_struct {
	char ttr_TPP_TOK_EOF[4];
	char ttr_TPP_TOK_LF[3];
	char ttr_TPP_TOK_SPACE[6];
	char ttr_TPP_TOK_EXCLAIM[8];
	char ttr_TPP_TOK_POUND[6];
#if TPP_HAVE_TPP_TOK_DOLLAR
	char ttr_TPP_TOK_DOLLAR[7];
#endif /* TPP_HAVE_TPP_TOK_DOLLAR */
	char ttr_TPP_TOK_PERCENT[8];
	char ttr_TPP_TOK_AMP[4];
	char ttr_TPP_TOK_LPAREN[7];
	char ttr_TPP_TOK_RPAREN[7];
	char ttr_TPP_TOK_STAR[5];
	char ttr_TPP_TOK_PLUS[5];
	char ttr_TPP_TOK_COMMA[6];
	char ttr_TPP_TOK_MINUS[6];
	char ttr_TPP_TOK_DOT[4];
	char ttr_TPP_TOK_SLASH[6];
	char ttr_TPP_TOK_COLON[6];
	char ttr_TPP_TOK_SEMICOLON[10];
	char ttr_TPP_TOK_LANGLE[7];
	char ttr_TPP_TOK_EQUAL[6];
	char ttr_TPP_TOK_RANGLE[7];
	char ttr_TPP_TOK_QMARK[6];
	char ttr_TPP_TOK_AT[3];
	char ttr_TPP_TOK_LBRACKET[9];
	char ttr_TPP_TOK_BACKSLASH[10];
	char ttr_TPP_TOK_RBRACKET[9];
	char ttr_TPP_TOK_HAT[4];
	char ttr_TPP_TOK_BACKTICK[9];
	char ttr_TPP_TOK_LBRACE[7];
	char ttr_TPP_TOK_PIPE[5];
	char ttr_TPP_TOK_RBRACE[7];
	char ttr_TPP_TOK_TILDE[6];
#if TPP_HAVE_UNICODE
	char ttr_TPP_TOK_UNICHAR[8];
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	char ttr_TPP_TOK_INT[4];
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	char ttr_TPP_TOK_FLOAT[6];
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT
	char ttr_TPP_TOK_C_COMMENT[10];
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT
	char ttr_TPP_TOK_PASCAL_COMMENT[15];
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT
	char ttr_TPP_TOK_CXX_COMMENT[12];
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT
	char ttr_TPP_TOK_SHELL_COMMENT[14];
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT
	char ttr_TPP_TOK_ASM_COMMENT[12];
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT
	char ttr_TPP_TOK_SQL_COMMENT[12];
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING
	char ttr_TPP_TOK_STRING[7];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	char ttr_TPP_TOK_CXX_RAW_STRING_LITERAL[23];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	char ttr_TPP_TOK_CXX_WIDE_STRING_LITERAL[24];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	char ttr_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL[28];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	char ttr_TPP_TOK_CXX_UTF8_STRING_LITERAL[24];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	char ttr_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL[28];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	char ttr_TPP_TOK_CXX_UTF16_STRING_LITERAL[25];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	char ttr_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL[29];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	char ttr_TPP_TOK_CXX_UTF32_STRING_LITERAL[25];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	char ttr_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL[29];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
	char ttr_TPP_TOK_RAW_STRING_LITERAL[19];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	char ttr_TPP_TOK_BLOCK_STRING_LITERAL[21];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	char ttr_TPP_TOK_CHAR[5];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	char ttr_TPP_TOK_CXX_RAW_CHAR_LITERAL[21];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	char ttr_TPP_TOK_CXX_WIDE_CHAR_LITERAL[22];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	char ttr_TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL[26];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
	char ttr_TPP_TOK_CXX_UTF8_CHAR_LITERAL[22];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	char ttr_TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL[26];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
	char ttr_TPP_TOK_CXX_UTF16_CHAR_LITERAL[23];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	char ttr_TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL[27];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
	char ttr_TPP_TOK_CXX_UTF32_CHAR_LITERAL[23];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	char ttr_TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL[27];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	char ttr_TPP_TOK_RAW_CHAR_LITERAL[17];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	char ttr_TPP_TOK_BLOCK_CHAR_LITERAL[19];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	char ttr_TPP_TOK_LANGLE_LANGLE[14];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	char ttr_TPP_TOK_RANGLE_RANGLE[14];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	char ttr_TPP_TOK_EQUAL_EQUAL[12];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	char ttr_TPP_TOK_EXCLAIM_EQUAL[14];
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	char ttr_TPP_TOK_RANGLE_EQUAL[13];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	char ttr_TPP_TOK_LANGLE_EQUAL[13];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	char ttr_TPP_TOK_DOT_DOT_DOT[12];
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
	char ttr_TPP_TOK_PLUS_EQUAL[11];
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
	char ttr_TPP_TOK_MINUS_EQUAL[12];
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
	char ttr_TPP_TOK_STAR_EQUAL[11];
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
	char ttr_TPP_TOK_SLASH_EQUAL[12];
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	char ttr_TPP_TOK_PERCENT_EQUAL[14];
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	char ttr_TPP_TOK_LANGLE_LANGLE_EQUAL[20];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	char ttr_TPP_TOK_RANGLE_RANGLE_EQUAL[20];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
	char ttr_TPP_TOK_AMP_EQUAL[10];
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
	char ttr_TPP_TOK_PIPE_EQUAL[11];
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
	char ttr_TPP_TOK_HAT_EQUAL[10];
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	char ttr_TPP_TOK_STAR_STAR_EQUAL[16];
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	char ttr_TPP_TOK_SLASH_SLASH_EQUAL[18];
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_AT_EQUAL
	char ttr_TPP_TOK_AT_EQUAL[9];
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
	char ttr_TPP_TOK_POUND_POUND[12];
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
	char ttr_TPP_TOK_AMP_AMP[8];
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
	char ttr_TPP_TOK_PIPE_PIPE[10];
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
	char ttr_TPP_TOK_HAT_HAT[8];
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
	char ttr_TPP_TOK_PLUS_PLUS[10];
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
	char ttr_TPP_TOK_MINUS_MINUS[12];
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
	char ttr_TPP_TOK_STAR_STAR[10];
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
	char ttr_TPP_TOK_SLASH_SLASH[12];
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
	char ttr_TPP_TOK_TILDE_TILDE[12];
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
	char ttr_TPP_TOK_TILDE_EQUAL[12];
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	char ttr_TPP_TOK_MINUS_RANGLE[13];
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
	char ttr_TPP_TOK_COLON_EQUAL[12];
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
	char ttr_TPP_TOK_COLON_COLON[12];
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	char ttr_TPP_TOK_MINUS_RANGLE_STAR[18];
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_DOT_STAR
	char ttr_TPP_TOK_DOT_STAR[9];
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
	char ttr_TPP_TOK_DOT_DOT[8];
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	char ttr_TPP_TOK_LANGLE_RANGLE[14];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	char ttr_TPP_TOK_LANGLE_LANGLE_LANGLE[21];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	char ttr_TPP_TOK_RANGLE_RANGLE_RANGLE[21];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	char ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL[27];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	char ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL[27];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	char ttr_TPP_TOK_EQUAL_EQUAL_EQUAL[18];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	char ttr_TPP_TOK_EXCLAIM_EQUAL_EQUAL[20];
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	char ttr_TPP_TOK_EXCLAIM_EXCLAIM[16];
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
	char ttr_TPP_TOK_QMARK_QMARK[12];
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
	char ttr_TPP_TOK_QMARK_EQUAL[12];
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	char ttr_TPP_TOK_RANGLE_LANGLE[14];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS
	char ttr_TPP_TOK_EQUAL_PLUS[11];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS
	char ttr_TPP_TOK_EQUAL_MINUS[12];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
	char ttr_TPP_TOK_EQUAL_STAR[11];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
	char ttr_TPP_TOK_EQUAL_STAR_STAR[16];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
	char ttr_TPP_TOK_EQUAL_SLASH[12];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
	char ttr_TPP_TOK_EQUAL_SLASH_SLASH[18];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
	char ttr_TPP_TOK_EQUAL_PERCENT[14];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP
	char ttr_TPP_TOK_EQUAL_AMP[10];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE
	char ttr_TPP_TOK_EQUAL_PIPE[11];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT
	char ttr_TPP_TOK_EQUAL_HAT[10];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	char ttr_TPP_TOK_EQUAL_LANGLE[13];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	char ttr_TPP_TOK_EQUAL_LANGLE_LANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	char ttr_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE[27];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	char ttr_TPP_TOK_EQUAL_RANGLE[13];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	char ttr_TPP_TOK_EQUAL_RANGLE_RANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	char ttr_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE[27];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_AT
	char ttr_TPP_TOK_EQUAL_AT[9];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE
	char ttr_TPP_TOK_EQUAL_TILDE[12];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON
	char ttr_TPP_TOK_EQUAL_COLON[12];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
	char ttr_TPP_TOK_EQUAL_EXCLAIM[14];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	char ttr_TPP_TOK_EQUAL_EQUAL_EXCLAIM[20];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK
	char ttr_TPP_TOK_EQUAL_QMARK[12];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
	char ttr_TPP_TOK_LANGLE_MINUS[13];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	char ttr_TPP_TOK_STAR_LANGLE_MINUS[18];
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_DOT
	char ttr_TPP_TOK_STAR_DOT[9];
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
	char ttr_TPP_TOK_MINUS_LANGLE[13];
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
	char ttr_TPP_TOK_RANGLE_MINUS[13];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	char ttr_TPP_TOK_LANGLE_EQUAL_RANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	char ttr_TPP_TOK_LANGLE_MINUS_RANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	char ttr_TPP_TOK_RANGLE_EQUAL_LANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	char ttr_TPP_TOK_RANGLE_MINUS_LANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	char ttr_TPP_TOK_LANGLE_EQUAL_LANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	char ttr_TPP_TOK_LANGLE_MINUS_LANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	char ttr_TPP_TOK_RANGLE_EQUAL_RANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	char ttr_TPP_TOK_RANGLE_MINUS_RANGLE[20];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
} const tpp_token_str_strings = {
	/* .ttr_TPP_TOK_EOF = */ "EOF",
	/* .ttr_TPP_TOK_LF = */ "LF",
	/* .ttr_TPP_TOK_SPACE = */ "SPACE",
	/* .ttr_TPP_TOK_EXCLAIM = */ "EXCLAIM",
	/* .ttr_TPP_TOK_POUND = */ "POUND",
#if TPP_HAVE_TPP_TOK_DOLLAR
	/* .ttr_TPP_TOK_DOLLAR = */ "DOLLAR",
#endif /* TPP_HAVE_TPP_TOK_DOLLAR */
	/* .ttr_TPP_TOK_PERCENT = */ "PERCENT",
	/* .ttr_TPP_TOK_AMP = */ "AMP",
	/* .ttr_TPP_TOK_LPAREN = */ "LPAREN",
	/* .ttr_TPP_TOK_RPAREN = */ "RPAREN",
	/* .ttr_TPP_TOK_STAR = */ "STAR",
	/* .ttr_TPP_TOK_PLUS = */ "PLUS",
	/* .ttr_TPP_TOK_COMMA = */ "COMMA",
	/* .ttr_TPP_TOK_MINUS = */ "MINUS",
	/* .ttr_TPP_TOK_DOT = */ "DOT",
	/* .ttr_TPP_TOK_SLASH = */ "SLASH",
	/* .ttr_TPP_TOK_COLON = */ "COLON",
	/* .ttr_TPP_TOK_SEMICOLON = */ "SEMICOLON",
	/* .ttr_TPP_TOK_LANGLE = */ "LANGLE",
	/* .ttr_TPP_TOK_EQUAL = */ "EQUAL",
	/* .ttr_TPP_TOK_RANGLE = */ "RANGLE",
	/* .ttr_TPP_TOK_QMARK = */ "QMARK",
	/* .ttr_TPP_TOK_AT = */ "AT",
	/* .ttr_TPP_TOK_LBRACKET = */ "LBRACKET",
	/* .ttr_TPP_TOK_BACKSLASH = */ "BACKSLASH",
	/* .ttr_TPP_TOK_RBRACKET = */ "RBRACKET",
	/* .ttr_TPP_TOK_HAT = */ "HAT",
	/* .ttr_TPP_TOK_BACKTICK = */ "BACKTICK",
	/* .ttr_TPP_TOK_LBRACE = */ "LBRACE",
	/* .ttr_TPP_TOK_PIPE = */ "PIPE",
	/* .ttr_TPP_TOK_RBRACE = */ "RBRACE",
	/* .ttr_TPP_TOK_TILDE = */ "TILDE",
#if TPP_HAVE_UNICODE
	/* .ttr_TPP_TOK_UNICHAR = */ "UNICHAR",
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	/* .ttr_TPP_TOK_INT = */ "INT",
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	/* .ttr_TPP_TOK_FLOAT = */ "FLOAT",
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT
	/* .ttr_TPP_TOK_C_COMMENT = */ "C_COMMENT",
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT
	/* .ttr_TPP_TOK_PASCAL_COMMENT = */ "PASCAL_COMMENT",
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT
	/* .ttr_TPP_TOK_CXX_COMMENT = */ "CXX_COMMENT",
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT
	/* .ttr_TPP_TOK_SHELL_COMMENT = */ "SHELL_COMMENT",
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT
	/* .ttr_TPP_TOK_ASM_COMMENT = */ "ASM_COMMENT",
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT
	/* .ttr_TPP_TOK_SQL_COMMENT = */ "SQL_COMMENT",
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING
	/* .ttr_TPP_TOK_STRING = */ "STRING",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_STRING_LITERAL = */ "CXX_RAW_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	/* .ttr_TPP_TOK_CXX_WIDE_STRING_LITERAL = */ "CXX_WIDE_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL = */ "CXX_RAW_WIDE_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	/* .ttr_TPP_TOK_CXX_UTF8_STRING_LITERAL = */ "CXX_UTF8_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL = */ "CXX_RAW_UTF8_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	/* .ttr_TPP_TOK_CXX_UTF16_STRING_LITERAL = */ "CXX_UTF16_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL = */ "CXX_RAW_UTF16_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	/* .ttr_TPP_TOK_CXX_UTF32_STRING_LITERAL = */ "CXX_UTF32_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL = */ "CXX_RAW_UTF32_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
	/* .ttr_TPP_TOK_RAW_STRING_LITERAL = */ "RAW_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	/* .ttr_TPP_TOK_BLOCK_STRING_LITERAL = */ "BLOCK_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	/* .ttr_TPP_TOK_CHAR = */ "CHAR",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_CHAR_LITERAL = */ "CXX_RAW_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	/* .ttr_TPP_TOK_CXX_WIDE_CHAR_LITERAL = */ "CXX_WIDE_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL = */ "CXX_RAW_WIDE_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
	/* .ttr_TPP_TOK_CXX_UTF8_CHAR_LITERAL = */ "CXX_UTF8_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL = */ "CXX_RAW_UTF8_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
	/* .ttr_TPP_TOK_CXX_UTF16_CHAR_LITERAL = */ "CXX_UTF16_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL = */ "CXX_RAW_UTF16_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
	/* .ttr_TPP_TOK_CXX_UTF32_CHAR_LITERAL = */ "CXX_UTF32_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* .ttr_TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL = */ "CXX_RAW_UTF32_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	/* .ttr_TPP_TOK_RAW_CHAR_LITERAL = */ "RAW_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	/* .ttr_TPP_TOK_BLOCK_CHAR_LITERAL = */ "BLOCK_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	/* .ttr_TPP_TOK_LANGLE_LANGLE = */ "LANGLE_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	/* .ttr_TPP_TOK_RANGLE_RANGLE = */ "RANGLE_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	/* .ttr_TPP_TOK_EQUAL_EQUAL = */ "EQUAL_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	/* .ttr_TPP_TOK_EXCLAIM_EQUAL = */ "EXCLAIM_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	/* .ttr_TPP_TOK_RANGLE_EQUAL = */ "RANGLE_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	/* .ttr_TPP_TOK_LANGLE_EQUAL = */ "LANGLE_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	/* .ttr_TPP_TOK_DOT_DOT_DOT = */ "DOT_DOT_DOT",
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
	/* .ttr_TPP_TOK_PLUS_EQUAL = */ "PLUS_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
	/* .ttr_TPP_TOK_MINUS_EQUAL = */ "MINUS_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
	/* .ttr_TPP_TOK_STAR_EQUAL = */ "STAR_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
	/* .ttr_TPP_TOK_SLASH_EQUAL = */ "SLASH_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	/* .ttr_TPP_TOK_PERCENT_EQUAL = */ "PERCENT_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	/* .ttr_TPP_TOK_LANGLE_LANGLE_EQUAL = */ "LANGLE_LANGLE_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	/* .ttr_TPP_TOK_RANGLE_RANGLE_EQUAL = */ "RANGLE_RANGLE_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
	/* .ttr_TPP_TOK_AMP_EQUAL = */ "AMP_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
	/* .ttr_TPP_TOK_PIPE_EQUAL = */ "PIPE_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
	/* .ttr_TPP_TOK_HAT_EQUAL = */ "HAT_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	/* .ttr_TPP_TOK_STAR_STAR_EQUAL = */ "STAR_STAR_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	/* .ttr_TPP_TOK_SLASH_SLASH_EQUAL = */ "SLASH_SLASH_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_AT_EQUAL
	/* .ttr_TPP_TOK_AT_EQUAL = */ "AT_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
	/* .ttr_TPP_TOK_POUND_POUND = */ "POUND_POUND",
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
	/* .ttr_TPP_TOK_AMP_AMP = */ "AMP_AMP",
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
	/* .ttr_TPP_TOK_PIPE_PIPE = */ "PIPE_PIPE",
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
	/* .ttr_TPP_TOK_HAT_HAT = */ "HAT_HAT",
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
	/* .ttr_TPP_TOK_PLUS_PLUS = */ "PLUS_PLUS",
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
	/* .ttr_TPP_TOK_MINUS_MINUS = */ "MINUS_MINUS",
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
	/* .ttr_TPP_TOK_STAR_STAR = */ "STAR_STAR",
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
	/* .ttr_TPP_TOK_SLASH_SLASH = */ "SLASH_SLASH",
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
	/* .ttr_TPP_TOK_TILDE_TILDE = */ "TILDE_TILDE",
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
	/* .ttr_TPP_TOK_TILDE_EQUAL = */ "TILDE_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	/* .ttr_TPP_TOK_MINUS_RANGLE = */ "MINUS_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
	/* .ttr_TPP_TOK_COLON_EQUAL = */ "COLON_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
	/* .ttr_TPP_TOK_COLON_COLON = */ "COLON_COLON",
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	/* .ttr_TPP_TOK_MINUS_RANGLE_STAR = */ "MINUS_RANGLE_STAR",
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_DOT_STAR
	/* .ttr_TPP_TOK_DOT_STAR = */ "DOT_STAR",
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
	/* .ttr_TPP_TOK_DOT_DOT = */ "DOT_DOT",
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	/* .ttr_TPP_TOK_LANGLE_RANGLE = */ "LANGLE_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	/* .ttr_TPP_TOK_LANGLE_LANGLE_LANGLE = */ "LANGLE_LANGLE_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	/* .ttr_TPP_TOK_RANGLE_RANGLE_RANGLE = */ "RANGLE_RANGLE_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	/* .ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL = */ "LANGLE_LANGLE_LANGLE_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	/* .ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL = */ "RANGLE_RANGLE_RANGLE_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	/* .ttr_TPP_TOK_EQUAL_EQUAL_EQUAL = */ "EQUAL_EQUAL_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	/* .ttr_TPP_TOK_EXCLAIM_EQUAL_EQUAL = */ "EXCLAIM_EQUAL_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	/* .ttr_TPP_TOK_EXCLAIM_EXCLAIM = */ "EXCLAIM_EXCLAIM",
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
	/* .ttr_TPP_TOK_QMARK_QMARK = */ "QMARK_QMARK",
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
	/* .ttr_TPP_TOK_QMARK_EQUAL = */ "QMARK_EQUAL",
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	/* .ttr_TPP_TOK_RANGLE_LANGLE = */ "RANGLE_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS
	/* .ttr_TPP_TOK_EQUAL_PLUS = */ "EQUAL_PLUS",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS
	/* .ttr_TPP_TOK_EQUAL_MINUS = */ "EQUAL_MINUS",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
	/* .ttr_TPP_TOK_EQUAL_STAR = */ "EQUAL_STAR",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
	/* .ttr_TPP_TOK_EQUAL_STAR_STAR = */ "EQUAL_STAR_STAR",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
	/* .ttr_TPP_TOK_EQUAL_SLASH = */ "EQUAL_SLASH",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
	/* .ttr_TPP_TOK_EQUAL_SLASH_SLASH = */ "EQUAL_SLASH_SLASH",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
	/* .ttr_TPP_TOK_EQUAL_PERCENT = */ "EQUAL_PERCENT",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP
	/* .ttr_TPP_TOK_EQUAL_AMP = */ "EQUAL_AMP",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE
	/* .ttr_TPP_TOK_EQUAL_PIPE = */ "EQUAL_PIPE",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT
	/* .ttr_TPP_TOK_EQUAL_HAT = */ "EQUAL_HAT",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	/* .ttr_TPP_TOK_EQUAL_LANGLE = */ "EQUAL_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	/* .ttr_TPP_TOK_EQUAL_LANGLE_LANGLE = */ "EQUAL_LANGLE_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	/* .ttr_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE = */ "EQUAL_LANGLE_LANGLE_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	/* .ttr_TPP_TOK_EQUAL_RANGLE = */ "EQUAL_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	/* .ttr_TPP_TOK_EQUAL_RANGLE_RANGLE = */ "EQUAL_RANGLE_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	/* .ttr_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE = */ "EQUAL_RANGLE_RANGLE_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_AT
	/* .ttr_TPP_TOK_EQUAL_AT = */ "EQUAL_AT",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE
	/* .ttr_TPP_TOK_EQUAL_TILDE = */ "EQUAL_TILDE",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON
	/* .ttr_TPP_TOK_EQUAL_COLON = */ "EQUAL_COLON",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
	/* .ttr_TPP_TOK_EQUAL_EXCLAIM = */ "EQUAL_EXCLAIM",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	/* .ttr_TPP_TOK_EQUAL_EQUAL_EXCLAIM = */ "EQUAL_EQUAL_EXCLAIM",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK
	/* .ttr_TPP_TOK_EQUAL_QMARK = */ "EQUAL_QMARK",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
	/* .ttr_TPP_TOK_LANGLE_MINUS = */ "LANGLE_MINUS",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	/* .ttr_TPP_TOK_STAR_LANGLE_MINUS = */ "STAR_LANGLE_MINUS",
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_DOT
	/* .ttr_TPP_TOK_STAR_DOT = */ "STAR_DOT",
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
	/* .ttr_TPP_TOK_MINUS_LANGLE = */ "MINUS_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
	/* .ttr_TPP_TOK_RANGLE_MINUS = */ "RANGLE_MINUS",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	/* .ttr_TPP_TOK_LANGLE_EQUAL_RANGLE = */ "LANGLE_EQUAL_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	/* .ttr_TPP_TOK_LANGLE_MINUS_RANGLE = */ "LANGLE_MINUS_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	/* .ttr_TPP_TOK_RANGLE_EQUAL_LANGLE = */ "RANGLE_EQUAL_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	/* .ttr_TPP_TOK_RANGLE_MINUS_LANGLE = */ "RANGLE_MINUS_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	/* .ttr_TPP_TOK_LANGLE_EQUAL_LANGLE = */ "LANGLE_EQUAL_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	/* .ttr_TPP_TOK_LANGLE_MINUS_LANGLE = */ "LANGLE_MINUS_LANGLE",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	/* .ttr_TPP_TOK_RANGLE_EQUAL_RANGLE = */ "RANGLE_EQUAL_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	/* .ttr_TPP_TOK_RANGLE_MINUS_RANGLE = */ "RANGLE_MINUS_RANGLE",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
};
static uint_least16_t const tpp_token_str_offsets[] = {
	/* [TPP_TOK_EOF] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EOF),
	/* [0x1] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x2] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x3] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x4] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x5] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x6] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x7] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x8] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x9] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [TPP_TOK_LF] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LF),
	/* [0xb] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x10] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x11] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x12] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x13] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x14] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x15] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x16] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x17] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x18] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x19] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x1a] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x1b] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x1c] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x1d] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x1e] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x1f] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [TPP_TOK_SPACE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_SPACE),
	/* [TPP_TOK_EXCLAIM] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EXCLAIM),
	/* [0x22] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [TPP_TOK_POUND] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_POUND),
#if TPP_HAVE_TPP_TOK_DOLLAR
	/* [TPP_TOK_DOLLAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_DOLLAR),
#else /* TPP_HAVE_TPP_TOK_DOLLAR */
	/* [0x24] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
#endif /* !TPP_HAVE_TPP_TOK_DOLLAR */
	/* [TPP_TOK_PERCENT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_PERCENT),
	/* [TPP_TOK_AMP] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_AMP),
	/* [0x27] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [TPP_TOK_LPAREN] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LPAREN),
	/* [TPP_TOK_RPAREN] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RPAREN),
	/* [TPP_TOK_STAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_STAR),
	/* [TPP_TOK_PLUS] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_PLUS),
	/* [TPP_TOK_COMMA] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_COMMA),
	/* [TPP_TOK_MINUS] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_MINUS),
	/* [TPP_TOK_DOT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_DOT),
	/* [TPP_TOK_SLASH] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_SLASH),
	/* [0x30] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x31] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x32] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x33] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x34] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x35] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x36] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x37] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x38] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x39] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [TPP_TOK_COLON] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_COLON),
	/* [TPP_TOK_SEMICOLON] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_SEMICOLON),
	/* [TPP_TOK_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE),
	/* [TPP_TOK_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL),
	/* [TPP_TOK_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE),
	/* [TPP_TOK_QMARK] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_QMARK),
	/* [TPP_TOK_AT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_AT),
	/* [0x41] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x42] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x43] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x44] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x45] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x46] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x47] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x48] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x49] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x4a] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x4b] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x4c] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x4d] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x4e] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x4f] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x50] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x51] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x52] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x53] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x54] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x55] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x56] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x57] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x58] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x59] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x5a] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [TPP_TOK_LBRACKET] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LBRACKET),
	/* [TPP_TOK_BACKSLASH] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_BACKSLASH),
	/* [TPP_TOK_RBRACKET] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RBRACKET),
	/* [TPP_TOK_HAT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_HAT),
	/* [0x5f] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [TPP_TOK_BACKTICK] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_BACKTICK),
	/* [0x61] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x62] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x63] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x64] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x65] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x66] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x67] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x68] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x69] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x6a] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x6b] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x6c] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x6d] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x6e] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x6f] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x70] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x71] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x72] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x73] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x74] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x75] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x76] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x77] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x78] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x79] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x7a] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [TPP_TOK_LBRACE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LBRACE),
	/* [TPP_TOK_PIPE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_PIPE),
	/* [TPP_TOK_RBRACE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RBRACE),
	/* [TPP_TOK_TILDE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_TILDE),
	/* [0x7f] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x80] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x81] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x82] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x83] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x84] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x85] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x86] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x87] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x88] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x89] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x8a] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x8b] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x8c] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x8d] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x8e] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x8f] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x90] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x91] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x92] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x93] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x94] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x95] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x96] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x97] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x98] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x99] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x9a] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x9b] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x9c] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x9d] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x9e] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0x9f] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa0] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa1] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa2] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa3] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa4] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa5] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa6] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa7] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa8] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xa9] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xaa] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xab] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xac] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xad] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xae] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xaf] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb0] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb1] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb2] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb3] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb4] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb5] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb6] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb7] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb8] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xb9] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xba] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xbb] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xbc] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xbd] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xbe] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xbf] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc0] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc1] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc2] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc3] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc4] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc5] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc6] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc7] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc8] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xc9] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xca] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xcb] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xcc] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xcd] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xce] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xcf] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd0] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd1] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd2] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd3] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd4] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd5] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd6] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd7] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd8] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xd9] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xda] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xdb] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xdc] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xdd] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xde] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xdf] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe0] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe1] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe2] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe3] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe4] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe5] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe6] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe7] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe8] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xe9] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xea] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xeb] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xec] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xed] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xee] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xef] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf0] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf1] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf2] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf3] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf4] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf5] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf6] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf7] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf8] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xf9] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xfa] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xfb] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xfc] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xfd] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xfe] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
	/* [0xff] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
#if TPP_HAVE_UNICODE
	/* [TPP_TOK_UNICHAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_UNICHAR),
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	/* [TPP_TOK_INT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_INT),
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	/* [TPP_TOK_FLOAT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_FLOAT),
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT
	/* [TPP_TOK_C_COMMENT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT
	/* [TPP_TOK_PASCAL_COMMENT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_PASCAL_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT
	/* [TPP_TOK_CXX_COMMENT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT
	/* [TPP_TOK_SHELL_COMMENT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_SHELL_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT
	/* [TPP_TOK_ASM_COMMENT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_ASM_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT
	/* [TPP_TOK_SQL_COMMENT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_SQL_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING
	/* [TPP_TOK_STRING] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	/* [TPP_TOK_CXX_WIDE_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_WIDE_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	/* [TPP_TOK_CXX_UTF8_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_UTF8_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	/* [TPP_TOK_CXX_UTF16_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_UTF16_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	/* [TPP_TOK_CXX_UTF32_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_UTF32_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
	/* [TPP_TOK_RAW_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RAW_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	/* [TPP_TOK_BLOCK_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_BLOCK_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	/* [TPP_TOK_CHAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	/* [TPP_TOK_CXX_WIDE_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_WIDE_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
	/* [TPP_TOK_CXX_UTF8_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_UTF8_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
	/* [TPP_TOK_CXX_UTF16_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_UTF16_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
	/* [TPP_TOK_CXX_UTF32_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_UTF32_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	/* [TPP_TOK_RAW_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RAW_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	/* [TPP_TOK_BLOCK_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_BLOCK_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	/* [TPP_TOK_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	/* [TPP_TOK_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	/* [TPP_TOK_EQUAL_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	/* [TPP_TOK_EXCLAIM_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EXCLAIM_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	/* [TPP_TOK_RANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	/* [TPP_TOK_LANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	/* [TPP_TOK_DOT_DOT_DOT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_DOT_DOT_DOT),
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
	/* [TPP_TOK_PLUS_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_PLUS_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
	/* [TPP_TOK_MINUS_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_MINUS_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
	/* [TPP_TOK_STAR_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_STAR_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
	/* [TPP_TOK_SLASH_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_SLASH_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	/* [TPP_TOK_PERCENT_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_PERCENT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	/* [TPP_TOK_LANGLE_LANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	/* [TPP_TOK_RANGLE_RANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
	/* [TPP_TOK_AMP_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_AMP_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
	/* [TPP_TOK_PIPE_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_PIPE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
	/* [TPP_TOK_HAT_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_HAT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	/* [TPP_TOK_STAR_STAR_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_STAR_STAR_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	/* [TPP_TOK_SLASH_SLASH_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_SLASH_SLASH_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_AT_EQUAL
	/* [TPP_TOK_AT_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_AT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
	/* [TPP_TOK_POUND_POUND] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_POUND_POUND),
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
	/* [TPP_TOK_AMP_AMP] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_AMP_AMP),
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
	/* [TPP_TOK_PIPE_PIPE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_PIPE_PIPE),
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
	/* [TPP_TOK_HAT_HAT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_HAT_HAT),
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
	/* [TPP_TOK_PLUS_PLUS] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_PLUS_PLUS),
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
	/* [TPP_TOK_MINUS_MINUS] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_MINUS_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
	/* [TPP_TOK_STAR_STAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_STAR_STAR),
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
	/* [TPP_TOK_SLASH_SLASH] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_SLASH_SLASH),
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
	/* [TPP_TOK_TILDE_TILDE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_TILDE_TILDE),
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
	/* [TPP_TOK_TILDE_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_TILDE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	/* [TPP_TOK_MINUS_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_MINUS_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
	/* [TPP_TOK_COLON_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_COLON_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
	/* [TPP_TOK_COLON_COLON] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_COLON_COLON),
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	/* [TPP_TOK_MINUS_RANGLE_STAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_MINUS_RANGLE_STAR),
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_DOT_STAR
	/* [TPP_TOK_DOT_STAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_DOT_STAR),
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
	/* [TPP_TOK_DOT_DOT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_DOT_DOT),
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	/* [TPP_TOK_LANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	/* [TPP_TOK_LANGLE_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	/* [TPP_TOK_RANGLE_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	/* [TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	/* [TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	/* [TPP_TOK_EQUAL_EQUAL_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_EQUAL_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	/* [TPP_TOK_EXCLAIM_EQUAL_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EXCLAIM_EQUAL_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	/* [TPP_TOK_EXCLAIM_EXCLAIM] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EXCLAIM_EXCLAIM),
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
	/* [TPP_TOK_QMARK_QMARK] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_QMARK_QMARK),
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
	/* [TPP_TOK_QMARK_EQUAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_QMARK_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	/* [TPP_TOK_RANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS
	/* [TPP_TOK_EQUAL_PLUS] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_PLUS),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS
	/* [TPP_TOK_EQUAL_MINUS] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_MINUS),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
	/* [TPP_TOK_EQUAL_STAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_STAR),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
	/* [TPP_TOK_EQUAL_STAR_STAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_STAR_STAR),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
	/* [TPP_TOK_EQUAL_SLASH] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_SLASH),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
	/* [TPP_TOK_EQUAL_SLASH_SLASH] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_SLASH_SLASH),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
	/* [TPP_TOK_EQUAL_PERCENT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_PERCENT),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP
	/* [TPP_TOK_EQUAL_AMP] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_AMP),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE
	/* [TPP_TOK_EQUAL_PIPE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_PIPE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT
	/* [TPP_TOK_EQUAL_HAT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_HAT),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	/* [TPP_TOK_EQUAL_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	/* [TPP_TOK_EQUAL_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	/* [TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	/* [TPP_TOK_EQUAL_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	/* [TPP_TOK_EQUAL_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	/* [TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_AT
	/* [TPP_TOK_EQUAL_AT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_AT),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE
	/* [TPP_TOK_EQUAL_TILDE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_TILDE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON
	/* [TPP_TOK_EQUAL_COLON] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_COLON),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
	/* [TPP_TOK_EQUAL_EXCLAIM] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_EXCLAIM),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	/* [TPP_TOK_EQUAL_EQUAL_EXCLAIM] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_EQUAL_EXCLAIM),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK
	/* [TPP_TOK_EQUAL_QMARK] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_EQUAL_QMARK),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
	/* [TPP_TOK_LANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	/* [TPP_TOK_STAR_LANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_STAR_LANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_DOT
	/* [TPP_TOK_STAR_DOT] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_STAR_DOT),
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
	/* [TPP_TOK_MINUS_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_MINUS_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
	/* [TPP_TOK_RANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	/* [TPP_TOK_LANGLE_EQUAL_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_EQUAL_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	/* [TPP_TOK_LANGLE_MINUS_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_MINUS_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	/* [TPP_TOK_RANGLE_EQUAL_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_EQUAL_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	/* [TPP_TOK_RANGLE_MINUS_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_MINUS_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	/* [TPP_TOK_LANGLE_EQUAL_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_EQUAL_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	/* [TPP_TOK_LANGLE_MINUS_LANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_LANGLE_MINUS_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	/* [TPP_TOK_RANGLE_EQUAL_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_EQUAL_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	/* [TPP_TOK_RANGLE_MINUS_RANGLE] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RANGLE_MINUS_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
};
TPP_STATIC_ASSERT(tpp_lengthof(tpp_token_str_offsets) == TPP_TOK_MULTICHAR_END);

/* Returns the "*" in "TPP_TOK_*" of "id", which must be a (non-keyword and non-error) token ID */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_strtokenid(tpp_token_id id) {
	if ((unsigned int)id < TPP_TOK_MULTICHAR_END) {
		char const *result = (char const *)&tpp_token_str_strings +
		                     tpp_token_str_offsets[(unsigned int)id];
		if (!*result)
			result = NULL;
		return result;
	}
	return NULL;
}
#endif /* TPP_HAVE_STRTOKENID */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/token-reprtokenid.c                                      */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_REPRTOKENID

static struct tpp_token_repr_strings_struct {
	char ttr_eof[6];
	char ttr_x01[5];
	char ttr_x02[5];
	char ttr_x03[5];
	char ttr_x04[5];
	char ttr_x05[5];
	char ttr_x06[5];
	char ttr_x07[5];
	char ttr_x08[5];
	char ttr_x09[5];
	char ttr_linefeed[11];
	char ttr_x0b[5];
	char ttr_x0c[5];
	char ttr_x0d[5];
	char ttr_x0e[5];
	char ttr_x0f[5];
	char ttr_x10[5];
	char ttr_x11[5];
	char ttr_x12[5];
	char ttr_x13[5];
	char ttr_x14[5];
	char ttr_x15[5];
	char ttr_x16[5];
	char ttr_x17[5];
	char ttr_x18[5];
	char ttr_x19[5];
	char ttr_x1a[5];
	char ttr_x1b[5];
	char ttr_x1c[5];
	char ttr_x1d[5];
	char ttr_x1e[5];
	char ttr_x1f[5];
	char ttr_space[8];
	char ttr_ord_21[2];
	char ttr_ord_22[2];
	char ttr_ord_23[2];
	char ttr_ord_24[2];
	char ttr_ord_25[2];
	char ttr_ord_26[2];
	char ttr_ord_27[2];
	char ttr_ord_28[2];
	char ttr_ord_29[2];
	char ttr_ord_2a[2];
	char ttr_ord_2b[2];
	char ttr_ord_2c[2];
	char ttr_ord_2d[2];
	char ttr_ord_2e[2];
	char ttr_ord_2f[2];
	char ttr_ord_30[2];
	char ttr_ord_31[2];
	char ttr_ord_32[2];
	char ttr_ord_33[2];
	char ttr_ord_34[2];
	char ttr_ord_35[2];
	char ttr_ord_36[2];
	char ttr_ord_37[2];
	char ttr_ord_38[2];
	char ttr_ord_39[2];
	char ttr_ord_3a[2];
	char ttr_ord_3b[2];
	char ttr_ord_3c[2];
	char ttr_ord_3d[2];
	char ttr_ord_3e[2];
	char ttr_ord_3f[2];
	char ttr_ord_40[2];
	char ttr_A[2];
	char ttr_B[2];
	char ttr_C[2];
	char ttr_D[2];
	char ttr_E[2];
	char ttr_F[2];
	char ttr_G[2];
	char ttr_H[2];
	char ttr_I[2];
	char ttr_J[2];
	char ttr_K[2];
	char ttr_L[2];
	char ttr_M[2];
	char ttr_N[2];
	char ttr_O[2];
	char ttr_P[2];
	char ttr_Q[2];
	char ttr_R[2];
	char ttr_S[2];
	char ttr_T[2];
	char ttr_U[2];
	char ttr_V[2];
	char ttr_W[2];
	char ttr_X[2];
	char ttr_Y[2];
	char ttr_Z[2];
	char ttr_ord_5b[2];
	char ttr_ord_5c[2];
	char ttr_ord_5d[2];
	char ttr_ord_5e[2];
	char ttr__[2];
	char ttr_ord_60[2];
	char ttr_a[2];
	char ttr_b[2];
	char ttr_c[2];
	char ttr_d[2];
	char ttr_e[2];
	char ttr_f[2];
	char ttr_g[2];
	char ttr_h[2];
	char ttr_i[2];
	char ttr_j[2];
	char ttr_k[2];
	char ttr_l[2];
	char ttr_m[2];
	char ttr_n[2];
	char ttr_o[2];
	char ttr_p[2];
	char ttr_q[2];
	char ttr_r[2];
	char ttr_s[2];
	char ttr_t[2];
	char ttr_u[2];
	char ttr_v[2];
	char ttr_w[2];
	char ttr_x[2];
	char ttr_y[2];
	char ttr_z[2];
	char ttr_ord_7b[2];
	char ttr_ord_7c[2];
	char ttr_ord_7d[2];
	char ttr_ord_7e[2];
	char ttr_x7f[5];
	char ttr_x80[5];
	char ttr_x81[5];
	char ttr_x82[5];
	char ttr_x83[5];
	char ttr_x84[5];
	char ttr_x85[5];
	char ttr_x86[5];
	char ttr_x87[5];
	char ttr_x88[5];
	char ttr_x89[5];
	char ttr_x8a[5];
	char ttr_x8b[5];
	char ttr_x8c[5];
	char ttr_x8d[5];
	char ttr_x8e[5];
	char ttr_x8f[5];
	char ttr_x90[5];
	char ttr_x91[5];
	char ttr_x92[5];
	char ttr_x93[5];
	char ttr_x94[5];
	char ttr_x95[5];
	char ttr_x96[5];
	char ttr_x97[5];
	char ttr_x98[5];
	char ttr_x99[5];
	char ttr_x9a[5];
	char ttr_x9b[5];
	char ttr_x9c[5];
	char ttr_x9d[5];
	char ttr_x9e[5];
	char ttr_x9f[5];
	char ttr_xa0[5];
	char ttr_xa1[5];
	char ttr_xa2[5];
	char ttr_xa3[5];
	char ttr_xa4[5];
	char ttr_xa5[5];
	char ttr_xa6[5];
	char ttr_xa7[5];
	char ttr_xa8[5];
	char ttr_xa9[5];
	char ttr_xaa[5];
	char ttr_xab[5];
	char ttr_xac[5];
	char ttr_xad[5];
	char ttr_xae[5];
	char ttr_xaf[5];
	char ttr_xb0[5];
	char ttr_xb1[5];
	char ttr_xb2[5];
	char ttr_xb3[5];
	char ttr_xb4[5];
	char ttr_xb5[5];
	char ttr_xb6[5];
	char ttr_xb7[5];
	char ttr_xb8[5];
	char ttr_xb9[5];
	char ttr_xba[5];
	char ttr_xbb[5];
	char ttr_xbc[5];
	char ttr_xbd[5];
	char ttr_xbe[5];
	char ttr_xbf[5];
	char ttr_xc0[5];
	char ttr_xc1[5];
	char ttr_xc2[5];
	char ttr_xc3[5];
	char ttr_xc4[5];
	char ttr_xc5[5];
	char ttr_xc6[5];
	char ttr_xc7[5];
	char ttr_xc8[5];
	char ttr_xc9[5];
	char ttr_xca[5];
	char ttr_xcb[5];
	char ttr_xcc[5];
	char ttr_xcd[5];
	char ttr_xce[5];
	char ttr_xcf[5];
	char ttr_xd0[5];
	char ttr_xd1[5];
	char ttr_xd2[5];
	char ttr_xd3[5];
	char ttr_xd4[5];
	char ttr_xd5[5];
	char ttr_xd6[5];
	char ttr_xd7[5];
	char ttr_xd8[5];
	char ttr_xd9[5];
	char ttr_xda[5];
	char ttr_xdb[5];
	char ttr_xdc[5];
	char ttr_xdd[5];
	char ttr_xde[5];
	char ttr_xdf[5];
	char ttr_xe0[5];
	char ttr_xe1[5];
	char ttr_xe2[5];
	char ttr_xe3[5];
	char ttr_xe4[5];
	char ttr_xe5[5];
	char ttr_xe6[5];
	char ttr_xe7[5];
	char ttr_xe8[5];
	char ttr_xe9[5];
	char ttr_xea[5];
	char ttr_xeb[5];
	char ttr_xec[5];
	char ttr_xed[5];
	char ttr_xee[5];
	char ttr_xef[5];
	char ttr_xf0[5];
	char ttr_xf1[5];
	char ttr_xf2[5];
	char ttr_xf3[5];
	char ttr_xf4[5];
	char ttr_xf5[5];
	char ttr_xf6[5];
	char ttr_xf7[5];
	char ttr_xf8[5];
	char ttr_xf9[5];
	char ttr_xfa[5];
	char ttr_xfb[5];
	char ttr_xfc[5];
	char ttr_xfd[5];
	char ttr_xfe[5];
	char ttr_xff[5];
#if TPP_HAVE_UNICODE
	char ttr_TPP_TOK_UNICHAR[20];
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	char ttr_TPP_TOK_INT[10];
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	char ttr_TPP_TOK_FLOAT[8];
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT
	char ttr_TPP_TOK_C_COMMENT[10];
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	char ttr_TPP_TOK_STRING[9];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	char ttr_TPP_TOK_CHAR[7];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	char ttr_TPP_TOK_LANGLE_LANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	char ttr_TPP_TOK_RANGLE_RANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	char ttr_TPP_TOK_EQUAL_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	char ttr_TPP_TOK_EXCLAIM_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	char ttr_TPP_TOK_RANGLE_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	char ttr_TPP_TOK_LANGLE_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	char ttr_TPP_TOK_DOT_DOT_DOT[4];
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
	char ttr_TPP_TOK_PLUS_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
	char ttr_TPP_TOK_MINUS_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
	char ttr_TPP_TOK_STAR_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
	char ttr_TPP_TOK_SLASH_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	char ttr_TPP_TOK_PERCENT_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	char ttr_TPP_TOK_LANGLE_LANGLE_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	char ttr_TPP_TOK_RANGLE_RANGLE_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
	char ttr_TPP_TOK_AMP_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
	char ttr_TPP_TOK_PIPE_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
	char ttr_TPP_TOK_HAT_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	char ttr_TPP_TOK_STAR_STAR_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	char ttr_TPP_TOK_SLASH_SLASH_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_AT_EQUAL
	char ttr_TPP_TOK_AT_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
	char ttr_TPP_TOK_POUND_POUND[3];
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
	char ttr_TPP_TOK_AMP_AMP[3];
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
	char ttr_TPP_TOK_PIPE_PIPE[3];
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
	char ttr_TPP_TOK_HAT_HAT[3];
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
	char ttr_TPP_TOK_PLUS_PLUS[3];
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
	char ttr_TPP_TOK_MINUS_MINUS[3];
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
	char ttr_TPP_TOK_STAR_STAR[3];
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
	char ttr_TPP_TOK_SLASH_SLASH[3];
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
	char ttr_TPP_TOK_TILDE_TILDE[3];
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
	char ttr_TPP_TOK_TILDE_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	char ttr_TPP_TOK_MINUS_RANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
	char ttr_TPP_TOK_COLON_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
	char ttr_TPP_TOK_COLON_COLON[3];
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	char ttr_TPP_TOK_MINUS_RANGLE_STAR[4];
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_DOT_STAR
	char ttr_TPP_TOK_DOT_STAR[3];
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
	char ttr_TPP_TOK_DOT_DOT[3];
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	char ttr_TPP_TOK_LANGLE_RANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	char ttr_TPP_TOK_LANGLE_LANGLE_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	char ttr_TPP_TOK_RANGLE_RANGLE_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	char ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL[5];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	char ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL[5];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	char ttr_TPP_TOK_EQUAL_EQUAL_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	char ttr_TPP_TOK_EXCLAIM_EQUAL_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	char ttr_TPP_TOK_EXCLAIM_EXCLAIM[3];
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
	char ttr_TPP_TOK_QMARK_QMARK[3];
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
	char ttr_TPP_TOK_QMARK_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	char ttr_TPP_TOK_RANGLE_LANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS
	char ttr_TPP_TOK_EQUAL_PLUS[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS
	char ttr_TPP_TOK_EQUAL_MINUS[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
	char ttr_TPP_TOK_EQUAL_STAR[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
	char ttr_TPP_TOK_EQUAL_STAR_STAR[4];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
	char ttr_TPP_TOK_EQUAL_SLASH[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
	char ttr_TPP_TOK_EQUAL_SLASH_SLASH[4];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
	char ttr_TPP_TOK_EQUAL_PERCENT[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP
	char ttr_TPP_TOK_EQUAL_AMP[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE
	char ttr_TPP_TOK_EQUAL_PIPE[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT
	char ttr_TPP_TOK_EQUAL_HAT[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	char ttr_TPP_TOK_EQUAL_LANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	char ttr_TPP_TOK_EQUAL_LANGLE_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	char ttr_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE[5];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	char ttr_TPP_TOK_EQUAL_RANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	char ttr_TPP_TOK_EQUAL_RANGLE_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	char ttr_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE[5];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_AT
	char ttr_TPP_TOK_EQUAL_AT[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE
	char ttr_TPP_TOK_EQUAL_TILDE[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON
	char ttr_TPP_TOK_EQUAL_COLON[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
	char ttr_TPP_TOK_EQUAL_EXCLAIM[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	char ttr_TPP_TOK_EQUAL_EQUAL_EXCLAIM[4];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK
	char ttr_TPP_TOK_EQUAL_QMARK[3];
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
	char ttr_TPP_TOK_LANGLE_MINUS[3];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	char ttr_TPP_TOK_STAR_LANGLE_MINUS[4];
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_DOT
	char ttr_TPP_TOK_STAR_DOT[3];
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
	char ttr_TPP_TOK_MINUS_LANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
	char ttr_TPP_TOK_RANGLE_MINUS[3];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	char ttr_TPP_TOK_LANGLE_EQUAL_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	char ttr_TPP_TOK_LANGLE_MINUS_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	char ttr_TPP_TOK_RANGLE_EQUAL_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	char ttr_TPP_TOK_RANGLE_MINUS_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	char ttr_TPP_TOK_LANGLE_EQUAL_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	char ttr_TPP_TOK_LANGLE_MINUS_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	char ttr_TPP_TOK_RANGLE_EQUAL_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	char ttr_TPP_TOK_RANGLE_MINUS_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
} const tpp_token_repr_strings = {
	/* .ttr_eof = */ "<eof>",
	/* .ttr_x01 = */ "\\x01",
	/* .ttr_x02 = */ "\\x02",
	/* .ttr_x03 = */ "\\x03",
	/* .ttr_x04 = */ "\\x04",
	/* .ttr_x05 = */ "\\x05",
	/* .ttr_x06 = */ "\\x06",
	/* .ttr_x07 = */ "\\x07",
	/* .ttr_x08 = */ "\\x08",
	/* .ttr_x09 = */ "\\x09",
	/* .ttr_linefeed = */ "<linefeed>",
	/* .ttr_x0b = */ "\\x0b",
	/* .ttr_x0c = */ "\\x0c",
	/* .ttr_x0d = */ "\\x0d",
	/* .ttr_x0e = */ "\\x0e",
	/* .ttr_x0f = */ "\\x0f",
	/* .ttr_x10 = */ "\\x10",
	/* .ttr_x11 = */ "\\x11",
	/* .ttr_x12 = */ "\\x12",
	/* .ttr_x13 = */ "\\x13",
	/* .ttr_x14 = */ "\\x14",
	/* .ttr_x15 = */ "\\x15",
	/* .ttr_x16 = */ "\\x16",
	/* .ttr_x17 = */ "\\x17",
	/* .ttr_x18 = */ "\\x18",
	/* .ttr_x19 = */ "\\x19",
	/* .ttr_x1a = */ "\\x1a",
	/* .ttr_x1b = */ "\\x1b",
	/* .ttr_x1c = */ "\\x1c",
	/* .ttr_x1d = */ "\\x1d",
	/* .ttr_x1e = */ "\\x1e",
	/* .ttr_x1f = */ "\\x1f",
	/* .ttr_space = */ "<space>",
	/* .ttr_ord_21 = */ "!",
	/* .ttr_ord_22 = */ "\"",
	/* .ttr_ord_23 = */ "#",
	/* .ttr_ord_24 = */ "$",
	/* .ttr_ord_25 = */ "%",
	/* .ttr_ord_26 = */ "&",
	/* .ttr_ord_27 = */ "\'",
	/* .ttr_ord_28 = */ "(",
	/* .ttr_ord_29 = */ ")",
	/* .ttr_ord_2a = */ "*",
	/* .ttr_ord_2b = */ "+",
	/* .ttr_ord_2c = */ ",",
	/* .ttr_ord_2d = */ "-",
	/* .ttr_ord_2e = */ ".",
	/* .ttr_ord_2f = */ "/",
	/* .ttr_ord_30 = */ "0",
	/* .ttr_ord_31 = */ "1",
	/* .ttr_ord_32 = */ "2",
	/* .ttr_ord_33 = */ "3",
	/* .ttr_ord_34 = */ "4",
	/* .ttr_ord_35 = */ "5",
	/* .ttr_ord_36 = */ "6",
	/* .ttr_ord_37 = */ "7",
	/* .ttr_ord_38 = */ "8",
	/* .ttr_ord_39 = */ "9",
	/* .ttr_ord_3a = */ ":",
	/* .ttr_ord_3b = */ ";",
	/* .ttr_ord_3c = */ "<",
	/* .ttr_ord_3d = */ "=",
	/* .ttr_ord_3e = */ ">",
	/* .ttr_ord_3f = */ "?",
	/* .ttr_ord_40 = */ "@",
	/* .ttr_A = */ "A",
	/* .ttr_B = */ "B",
	/* .ttr_C = */ "C",
	/* .ttr_D = */ "D",
	/* .ttr_E = */ "E",
	/* .ttr_F = */ "F",
	/* .ttr_G = */ "G",
	/* .ttr_H = */ "H",
	/* .ttr_I = */ "I",
	/* .ttr_J = */ "J",
	/* .ttr_K = */ "K",
	/* .ttr_L = */ "L",
	/* .ttr_M = */ "M",
	/* .ttr_N = */ "N",
	/* .ttr_O = */ "O",
	/* .ttr_P = */ "P",
	/* .ttr_Q = */ "Q",
	/* .ttr_R = */ "R",
	/* .ttr_S = */ "S",
	/* .ttr_T = */ "T",
	/* .ttr_U = */ "U",
	/* .ttr_V = */ "V",
	/* .ttr_W = */ "W",
	/* .ttr_X = */ "X",
	/* .ttr_Y = */ "Y",
	/* .ttr_Z = */ "Z",
	/* .ttr_ord_5b = */ "[",
	/* .ttr_ord_5c = */ "\\",
	/* .ttr_ord_5d = */ "]",
	/* .ttr_ord_5e = */ "^",
	/* .ttr__ = */ "_",
	/* .ttr_ord_60 = */ "`",
	/* .ttr_a = */ "a",
	/* .ttr_b = */ "b",
	/* .ttr_c = */ "c",
	/* .ttr_d = */ "d",
	/* .ttr_e = */ "e",
	/* .ttr_f = */ "f",
	/* .ttr_g = */ "g",
	/* .ttr_h = */ "h",
	/* .ttr_i = */ "i",
	/* .ttr_j = */ "j",
	/* .ttr_k = */ "k",
	/* .ttr_l = */ "l",
	/* .ttr_m = */ "m",
	/* .ttr_n = */ "n",
	/* .ttr_o = */ "o",
	/* .ttr_p = */ "p",
	/* .ttr_q = */ "q",
	/* .ttr_r = */ "r",
	/* .ttr_s = */ "s",
	/* .ttr_t = */ "t",
	/* .ttr_u = */ "u",
	/* .ttr_v = */ "v",
	/* .ttr_w = */ "w",
	/* .ttr_x = */ "x",
	/* .ttr_y = */ "y",
	/* .ttr_z = */ "z",
	/* .ttr_ord_7b = */ "{",
	/* .ttr_ord_7c = */ "|",
	/* .ttr_ord_7d = */ "}",
	/* .ttr_ord_7e = */ "~",
	/* .ttr_x7f = */ "\\x7f",
	/* .ttr_x80 = */ "\\x80",
	/* .ttr_x81 = */ "\\x81",
	/* .ttr_x82 = */ "\\x82",
	/* .ttr_x83 = */ "\\x83",
	/* .ttr_x84 = */ "\\x84",
	/* .ttr_x85 = */ "\\x85",
	/* .ttr_x86 = */ "\\x86",
	/* .ttr_x87 = */ "\\x87",
	/* .ttr_x88 = */ "\\x88",
	/* .ttr_x89 = */ "\\x89",
	/* .ttr_x8a = */ "\\x8a",
	/* .ttr_x8b = */ "\\x8b",
	/* .ttr_x8c = */ "\\x8c",
	/* .ttr_x8d = */ "\\x8d",
	/* .ttr_x8e = */ "\\x8e",
	/* .ttr_x8f = */ "\\x8f",
	/* .ttr_x90 = */ "\\x90",
	/* .ttr_x91 = */ "\\x91",
	/* .ttr_x92 = */ "\\x92",
	/* .ttr_x93 = */ "\\x93",
	/* .ttr_x94 = */ "\\x94",
	/* .ttr_x95 = */ "\\x95",
	/* .ttr_x96 = */ "\\x96",
	/* .ttr_x97 = */ "\\x97",
	/* .ttr_x98 = */ "\\x98",
	/* .ttr_x99 = */ "\\x99",
	/* .ttr_x9a = */ "\\x9a",
	/* .ttr_x9b = */ "\\x9b",
	/* .ttr_x9c = */ "\\x9c",
	/* .ttr_x9d = */ "\\x9d",
	/* .ttr_x9e = */ "\\x9e",
	/* .ttr_x9f = */ "\\x9f",
	/* .ttr_xa0 = */ "\\xa0",
	/* .ttr_xa1 = */ "\\xa1",
	/* .ttr_xa2 = */ "\\xa2",
	/* .ttr_xa3 = */ "\\xa3",
	/* .ttr_xa4 = */ "\\xa4",
	/* .ttr_xa5 = */ "\\xa5",
	/* .ttr_xa6 = */ "\\xa6",
	/* .ttr_xa7 = */ "\\xa7",
	/* .ttr_xa8 = */ "\\xa8",
	/* .ttr_xa9 = */ "\\xa9",
	/* .ttr_xaa = */ "\\xaa",
	/* .ttr_xab = */ "\\xab",
	/* .ttr_xac = */ "\\xac",
	/* .ttr_xad = */ "\\xad",
	/* .ttr_xae = */ "\\xae",
	/* .ttr_xaf = */ "\\xaf",
	/* .ttr_xb0 = */ "\\xb0",
	/* .ttr_xb1 = */ "\\xb1",
	/* .ttr_xb2 = */ "\\xb2",
	/* .ttr_xb3 = */ "\\xb3",
	/* .ttr_xb4 = */ "\\xb4",
	/* .ttr_xb5 = */ "\\xb5",
	/* .ttr_xb6 = */ "\\xb6",
	/* .ttr_xb7 = */ "\\xb7",
	/* .ttr_xb8 = */ "\\xb8",
	/* .ttr_xb9 = */ "\\xb9",
	/* .ttr_xba = */ "\\xba",
	/* .ttr_xbb = */ "\\xbb",
	/* .ttr_xbc = */ "\\xbc",
	/* .ttr_xbd = */ "\\xbd",
	/* .ttr_xbe = */ "\\xbe",
	/* .ttr_xbf = */ "\\xbf",
	/* .ttr_xc0 = */ "\\xc0",
	/* .ttr_xc1 = */ "\\xc1",
	/* .ttr_xc2 = */ "\\xc2",
	/* .ttr_xc3 = */ "\\xc3",
	/* .ttr_xc4 = */ "\\xc4",
	/* .ttr_xc5 = */ "\\xc5",
	/* .ttr_xc6 = */ "\\xc6",
	/* .ttr_xc7 = */ "\\xc7",
	/* .ttr_xc8 = */ "\\xc8",
	/* .ttr_xc9 = */ "\\xc9",
	/* .ttr_xca = */ "\\xca",
	/* .ttr_xcb = */ "\\xcb",
	/* .ttr_xcc = */ "\\xcc",
	/* .ttr_xcd = */ "\\xcd",
	/* .ttr_xce = */ "\\xce",
	/* .ttr_xcf = */ "\\xcf",
	/* .ttr_xd0 = */ "\\xd0",
	/* .ttr_xd1 = */ "\\xd1",
	/* .ttr_xd2 = */ "\\xd2",
	/* .ttr_xd3 = */ "\\xd3",
	/* .ttr_xd4 = */ "\\xd4",
	/* .ttr_xd5 = */ "\\xd5",
	/* .ttr_xd6 = */ "\\xd6",
	/* .ttr_xd7 = */ "\\xd7",
	/* .ttr_xd8 = */ "\\xd8",
	/* .ttr_xd9 = */ "\\xd9",
	/* .ttr_xda = */ "\\xda",
	/* .ttr_xdb = */ "\\xdb",
	/* .ttr_xdc = */ "\\xdc",
	/* .ttr_xdd = */ "\\xdd",
	/* .ttr_xde = */ "\\xde",
	/* .ttr_xdf = */ "\\xdf",
	/* .ttr_xe0 = */ "\\xe0",
	/* .ttr_xe1 = */ "\\xe1",
	/* .ttr_xe2 = */ "\\xe2",
	/* .ttr_xe3 = */ "\\xe3",
	/* .ttr_xe4 = */ "\\xe4",
	/* .ttr_xe5 = */ "\\xe5",
	/* .ttr_xe6 = */ "\\xe6",
	/* .ttr_xe7 = */ "\\xe7",
	/* .ttr_xe8 = */ "\\xe8",
	/* .ttr_xe9 = */ "\\xe9",
	/* .ttr_xea = */ "\\xea",
	/* .ttr_xeb = */ "\\xeb",
	/* .ttr_xec = */ "\\xec",
	/* .ttr_xed = */ "\\xed",
	/* .ttr_xee = */ "\\xee",
	/* .ttr_xef = */ "\\xef",
	/* .ttr_xf0 = */ "\\xf0",
	/* .ttr_xf1 = */ "\\xf1",
	/* .ttr_xf2 = */ "\\xf2",
	/* .ttr_xf3 = */ "\\xf3",
	/* .ttr_xf4 = */ "\\xf4",
	/* .ttr_xf5 = */ "\\xf5",
	/* .ttr_xf6 = */ "\\xf6",
	/* .ttr_xf7 = */ "\\xf7",
	/* .ttr_xf8 = */ "\\xf8",
	/* .ttr_xf9 = */ "\\xf9",
	/* .ttr_xfa = */ "\\xfa",
	/* .ttr_xfb = */ "\\xfb",
	/* .ttr_xfc = */ "\\xfc",
	/* .ttr_xfd = */ "\\xfd",
	/* .ttr_xfe = */ "\\xfe",
	/* .ttr_xff = */ "\\xff",
#if TPP_HAVE_UNICODE
	/* .ttr_TPP_TOK_UNICHAR = */ "<unicode character>",
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	/* .ttr_TPP_TOK_INT = */ "<integer>",
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	/* .ttr_TPP_TOK_FLOAT = */ "<float>",
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT
	/* .ttr_TPP_TOK_C_COMMENT = */ "<comment>",
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT || TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	/* .ttr_TPP_TOK_STRING = */ "<string>",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	/* .ttr_TPP_TOK_CHAR = */ "<char>",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	/* .ttr_TPP_TOK_LANGLE_LANGLE = */ "<<",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	/* .ttr_TPP_TOK_RANGLE_RANGLE = */ ">>",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	/* .ttr_TPP_TOK_EQUAL_EQUAL = */ "==",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	/* .ttr_TPP_TOK_EXCLAIM_EQUAL = */ "!=",
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	/* .ttr_TPP_TOK_RANGLE_EQUAL = */ ">=",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	/* .ttr_TPP_TOK_LANGLE_EQUAL = */ "<=",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	/* .ttr_TPP_TOK_DOT_DOT_DOT = */ "...",
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
	/* .ttr_TPP_TOK_PLUS_EQUAL = */ "+=",
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
	/* .ttr_TPP_TOK_MINUS_EQUAL = */ "-=",
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
	/* .ttr_TPP_TOK_STAR_EQUAL = */ "*=",
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
	/* .ttr_TPP_TOK_SLASH_EQUAL = */ "/=",
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	/* .ttr_TPP_TOK_PERCENT_EQUAL = */ "%=",
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	/* .ttr_TPP_TOK_LANGLE_LANGLE_EQUAL = */ "<<=",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	/* .ttr_TPP_TOK_RANGLE_RANGLE_EQUAL = */ ">>=",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
	/* .ttr_TPP_TOK_AMP_EQUAL = */ "&=",
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
	/* .ttr_TPP_TOK_PIPE_EQUAL = */ "|=",
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
	/* .ttr_TPP_TOK_HAT_EQUAL = */ "^=",
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	/* .ttr_TPP_TOK_STAR_STAR_EQUAL = */ "**=",
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	/* .ttr_TPP_TOK_SLASH_SLASH_EQUAL = */ "//=",
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_AT_EQUAL
	/* .ttr_TPP_TOK_AT_EQUAL = */ "@=",
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
	/* .ttr_TPP_TOK_POUND_POUND = */ "##",
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
	/* .ttr_TPP_TOK_AMP_AMP = */ "&&",
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
	/* .ttr_TPP_TOK_PIPE_PIPE = */ "||",
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
	/* .ttr_TPP_TOK_HAT_HAT = */ "^^",
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
	/* .ttr_TPP_TOK_PLUS_PLUS = */ "++",
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
	/* .ttr_TPP_TOK_MINUS_MINUS = */ "--",
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
	/* .ttr_TPP_TOK_STAR_STAR = */ "**",
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
	/* .ttr_TPP_TOK_SLASH_SLASH = */ "//",
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
	/* .ttr_TPP_TOK_TILDE_TILDE = */ "~~",
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
	/* .ttr_TPP_TOK_TILDE_EQUAL = */ "~=",
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	/* .ttr_TPP_TOK_MINUS_RANGLE = */ "->",
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
	/* .ttr_TPP_TOK_COLON_EQUAL = */ ":=",
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
	/* .ttr_TPP_TOK_COLON_COLON = */ "::",
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	/* .ttr_TPP_TOK_MINUS_RANGLE_STAR = */ "->*",
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_DOT_STAR
	/* .ttr_TPP_TOK_DOT_STAR = */ ".*",
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
	/* .ttr_TPP_TOK_DOT_DOT = */ "..",
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	/* .ttr_TPP_TOK_LANGLE_RANGLE = */ "<>",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	/* .ttr_TPP_TOK_LANGLE_LANGLE_LANGLE = */ "<<<",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	/* .ttr_TPP_TOK_RANGLE_RANGLE_RANGLE = */ ">>>",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	/* .ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL = */ "<<<=",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	/* .ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL = */ ">>>=",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	/* .ttr_TPP_TOK_EQUAL_EQUAL_EQUAL = */ "===",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	/* .ttr_TPP_TOK_EXCLAIM_EQUAL_EQUAL = */ "!==",
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	/* .ttr_TPP_TOK_EXCLAIM_EXCLAIM = */ "!!",
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
	/* .ttr_TPP_TOK_QMARK_QMARK = */ "??",
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
	/* .ttr_TPP_TOK_QMARK_EQUAL = */ "?=",
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	/* .ttr_TPP_TOK_RANGLE_LANGLE = */ "><",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS
	/* .ttr_TPP_TOK_EQUAL_PLUS = */ "=+",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS
	/* .ttr_TPP_TOK_EQUAL_MINUS = */ "=-",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
	/* .ttr_TPP_TOK_EQUAL_STAR = */ "=*",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
	/* .ttr_TPP_TOK_EQUAL_STAR_STAR = */ "=**",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
	/* .ttr_TPP_TOK_EQUAL_SLASH = */ "=/",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
	/* .ttr_TPP_TOK_EQUAL_SLASH_SLASH = */ "=//",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
	/* .ttr_TPP_TOK_EQUAL_PERCENT = */ "=%",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP
	/* .ttr_TPP_TOK_EQUAL_AMP = */ "=&",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE
	/* .ttr_TPP_TOK_EQUAL_PIPE = */ "=|",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT
	/* .ttr_TPP_TOK_EQUAL_HAT = */ "=^",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	/* .ttr_TPP_TOK_EQUAL_LANGLE = */ "=<",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	/* .ttr_TPP_TOK_EQUAL_LANGLE_LANGLE = */ "=<<",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	/* .ttr_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE = */ "=<<<",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	/* .ttr_TPP_TOK_EQUAL_RANGLE = */ "=>",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	/* .ttr_TPP_TOK_EQUAL_RANGLE_RANGLE = */ "=>>",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	/* .ttr_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE = */ "=>>>",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_AT
	/* .ttr_TPP_TOK_EQUAL_AT = */ "=@",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE
	/* .ttr_TPP_TOK_EQUAL_TILDE = */ "=~",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON
	/* .ttr_TPP_TOK_EQUAL_COLON = */ "=:",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
	/* .ttr_TPP_TOK_EQUAL_EXCLAIM = */ "=!",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	/* .ttr_TPP_TOK_EQUAL_EQUAL_EXCLAIM = */ "==!",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK
	/* .ttr_TPP_TOK_EQUAL_QMARK = */ "=?",
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
	/* .ttr_TPP_TOK_LANGLE_MINUS = */ "<-",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	/* .ttr_TPP_TOK_STAR_LANGLE_MINUS = */ "*<-",
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_DOT
	/* .ttr_TPP_TOK_STAR_DOT = */ "*.",
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
	/* .ttr_TPP_TOK_MINUS_LANGLE = */ "-<",
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
	/* .ttr_TPP_TOK_RANGLE_MINUS = */ ">-",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	/* .ttr_TPP_TOK_LANGLE_EQUAL_RANGLE = */ "<=>",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	/* .ttr_TPP_TOK_LANGLE_MINUS_RANGLE = */ "<->",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	/* .ttr_TPP_TOK_RANGLE_EQUAL_LANGLE = */ ">=<",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	/* .ttr_TPP_TOK_RANGLE_MINUS_LANGLE = */ ">-<",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	/* .ttr_TPP_TOK_LANGLE_EQUAL_LANGLE = */ "<=<",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	/* .ttr_TPP_TOK_LANGLE_MINUS_LANGLE = */ "<-<",
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	/* .ttr_TPP_TOK_RANGLE_EQUAL_RANGLE = */ ">=>",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	/* .ttr_TPP_TOK_RANGLE_MINUS_RANGLE = */ ">->",
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
};
static uint_least16_t const tpp_token_repr_offsets[] = {
	/* [0x0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_eof),
	/* [0x1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x01),
	/* [0x2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x02),
	/* [0x3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x03),
	/* [0x4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x04),
	/* [0x5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x05),
	/* [0x6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x06),
	/* [0x7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x07),
	/* [0x8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x08),
	/* [0x9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x09),
	/* [0xa] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_linefeed),
	/* [0xb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0b),
	/* [0xc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0c),
	/* [0xd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0d),
	/* [0xe] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0e),
	/* [0xf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0f),
	/* [0x10] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x10),
	/* [0x11] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x11),
	/* [0x12] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x12),
	/* [0x13] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x13),
	/* [0x14] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x14),
	/* [0x15] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x15),
	/* [0x16] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x16),
	/* [0x17] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x17),
	/* [0x18] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x18),
	/* [0x19] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x19),
	/* [0x1a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1a),
	/* [0x1b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1b),
	/* [0x1c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1c),
	/* [0x1d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1d),
	/* [0x1e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1e),
	/* [0x1f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1f),
	/* [0x20] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_space),
	/* [0x21] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_21),
	/* [0x22] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_22),
	/* [0x23] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_23),
	/* [0x24] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_24),
	/* [0x25] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_25),
	/* [0x26] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_26),
	/* [0x27] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_27),
	/* [0x28] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_28),
	/* [0x29] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_29),
	/* [0x2a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2a),
	/* [0x2b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2b),
	/* [0x2c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2c),
	/* [0x2d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2d),
	/* [0x2e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2e),
	/* [0x2f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2f),
	/* [0x30] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_30),
	/* [0x31] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_31),
	/* [0x32] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_32),
	/* [0x33] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_33),
	/* [0x34] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_34),
	/* [0x35] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_35),
	/* [0x36] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_36),
	/* [0x37] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_37),
	/* [0x38] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_38),
	/* [0x39] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_39),
	/* [0x3a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3a),
	/* [0x3b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3b),
	/* [0x3c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3c),
	/* [0x3d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3d),
	/* [0x3e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3e),
	/* [0x3f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3f),
	/* [0x40] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_40),
	/* [0x41] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_A),
	/* [0x42] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_B),
	/* [0x43] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_C),
	/* [0x44] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_D),
	/* [0x45] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_E),
	/* [0x46] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_F),
	/* [0x47] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_G),
	/* [0x48] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_H),
	/* [0x49] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_I),
	/* [0x4a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_J),
	/* [0x4b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_K),
	/* [0x4c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_L),
	/* [0x4d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_M),
	/* [0x4e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_N),
	/* [0x4f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_O),
	/* [0x50] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_P),
	/* [0x51] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_Q),
	/* [0x52] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_R),
	/* [0x53] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_S),
	/* [0x54] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_T),
	/* [0x55] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_U),
	/* [0x56] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_V),
	/* [0x57] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_W),
	/* [0x58] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_X),
	/* [0x59] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_Y),
	/* [0x5a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_Z),
	/* [0x5b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_5b),
	/* [0x5c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_5c),
	/* [0x5d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_5d),
	/* [0x5e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_5e),
	/* [0x5f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr__),
	/* [0x60] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_60),
	/* [0x61] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_a),
	/* [0x62] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_b),
	/* [0x63] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_c),
	/* [0x64] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_d),
	/* [0x65] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_e),
	/* [0x66] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_f),
	/* [0x67] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_g),
	/* [0x68] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_h),
	/* [0x69] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_i),
	/* [0x6a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_j),
	/* [0x6b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_k),
	/* [0x6c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_l),
	/* [0x6d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_m),
	/* [0x6e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_n),
	/* [0x6f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_o),
	/* [0x70] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_p),
	/* [0x71] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_q),
	/* [0x72] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_r),
	/* [0x73] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_s),
	/* [0x74] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_t),
	/* [0x75] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_u),
	/* [0x76] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_v),
	/* [0x77] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_w),
	/* [0x78] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x),
	/* [0x79] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_y),
	/* [0x7a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_z),
	/* [0x7b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_7b),
	/* [0x7c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_7c),
	/* [0x7d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_7d),
	/* [0x7e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_7e),
	/* [0x7f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x7f),
	/* [0x80] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x80),
	/* [0x81] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x81),
	/* [0x82] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x82),
	/* [0x83] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x83),
	/* [0x84] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x84),
	/* [0x85] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x85),
	/* [0x86] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x86),
	/* [0x87] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x87),
	/* [0x88] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x88),
	/* [0x89] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x89),
	/* [0x8a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8a),
	/* [0x8b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8b),
	/* [0x8c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8c),
	/* [0x8d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8d),
	/* [0x8e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8e),
	/* [0x8f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8f),
	/* [0x90] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x90),
	/* [0x91] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x91),
	/* [0x92] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x92),
	/* [0x93] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x93),
	/* [0x94] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x94),
	/* [0x95] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x95),
	/* [0x96] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x96),
	/* [0x97] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x97),
	/* [0x98] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x98),
	/* [0x99] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x99),
	/* [0x9a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9a),
	/* [0x9b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9b),
	/* [0x9c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9c),
	/* [0x9d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9d),
	/* [0x9e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9e),
	/* [0x9f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9f),
	/* [0xa0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa0),
	/* [0xa1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa1),
	/* [0xa2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa2),
	/* [0xa3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa3),
	/* [0xa4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa4),
	/* [0xa5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa5),
	/* [0xa6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa6),
	/* [0xa7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa7),
	/* [0xa8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa8),
	/* [0xa9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa9),
	/* [0xaa] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xaa),
	/* [0xab] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xab),
	/* [0xac] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xac),
	/* [0xad] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xad),
	/* [0xae] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xae),
	/* [0xaf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xaf),
	/* [0xb0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb0),
	/* [0xb1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb1),
	/* [0xb2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb2),
	/* [0xb3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb3),
	/* [0xb4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb4),
	/* [0xb5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb5),
	/* [0xb6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb6),
	/* [0xb7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb7),
	/* [0xb8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb8),
	/* [0xb9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb9),
	/* [0xba] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xba),
	/* [0xbb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbb),
	/* [0xbc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbc),
	/* [0xbd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbd),
	/* [0xbe] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbe),
	/* [0xbf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbf),
	/* [0xc0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc0),
	/* [0xc1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc1),
	/* [0xc2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc2),
	/* [0xc3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc3),
	/* [0xc4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc4),
	/* [0xc5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc5),
	/* [0xc6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc6),
	/* [0xc7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc7),
	/* [0xc8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc8),
	/* [0xc9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc9),
	/* [0xca] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xca),
	/* [0xcb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xcb),
	/* [0xcc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xcc),
	/* [0xcd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xcd),
	/* [0xce] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xce),
	/* [0xcf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xcf),
	/* [0xd0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd0),
	/* [0xd1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd1),
	/* [0xd2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd2),
	/* [0xd3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd3),
	/* [0xd4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd4),
	/* [0xd5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd5),
	/* [0xd6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd6),
	/* [0xd7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd7),
	/* [0xd8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd8),
	/* [0xd9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd9),
	/* [0xda] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xda),
	/* [0xdb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xdb),
	/* [0xdc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xdc),
	/* [0xdd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xdd),
	/* [0xde] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xde),
	/* [0xdf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xdf),
	/* [0xe0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe0),
	/* [0xe1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe1),
	/* [0xe2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe2),
	/* [0xe3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe3),
	/* [0xe4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe4),
	/* [0xe5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe5),
	/* [0xe6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe6),
	/* [0xe7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe7),
	/* [0xe8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe8),
	/* [0xe9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe9),
	/* [0xea] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xea),
	/* [0xeb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xeb),
	/* [0xec] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xec),
	/* [0xed] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xed),
	/* [0xee] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xee),
	/* [0xef] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xef),
	/* [0xf0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf0),
	/* [0xf1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf1),
	/* [0xf2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf2),
	/* [0xf3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf3),
	/* [0xf4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf4),
	/* [0xf5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf5),
	/* [0xf6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf6),
	/* [0xf7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf7),
	/* [0xf8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf8),
	/* [0xf9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf9),
	/* [0xfa] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfa),
	/* [0xfb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfb),
	/* [0xfc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfc),
	/* [0xfd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfd),
	/* [0xfe] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfe),
	/* [0xff] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xff),
#if TPP_HAVE_UNICODE
	/* [TPP_TOK_UNICHAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_UNICHAR),
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	/* [TPP_TOK_INT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_INT),
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	/* [TPP_TOK_FLOAT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_FLOAT),
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT
	/* [TPP_TOK_C_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT
	/* [TPP_TOK_PASCAL_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT
	/* [TPP_TOK_CXX_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT
	/* [TPP_TOK_SHELL_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT
	/* [TPP_TOK_ASM_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT
	/* [TPP_TOK_SQL_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING
	/* [TPP_TOK_STRING] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	/* [TPP_TOK_CXX_WIDE_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	/* [TPP_TOK_CXX_UTF8_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	/* [TPP_TOK_CXX_UTF16_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	/* [TPP_TOK_CXX_UTF32_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
	/* [TPP_TOK_RAW_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	/* [TPP_TOK_BLOCK_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	/* [TPP_TOK_CHAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	/* [TPP_TOK_CXX_WIDE_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
	/* [TPP_TOK_CXX_UTF8_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
	/* [TPP_TOK_CXX_UTF16_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
	/* [TPP_TOK_CXX_UTF32_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	/* [TPP_TOK_RAW_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	/* [TPP_TOK_BLOCK_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	/* [TPP_TOK_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	/* [TPP_TOK_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	/* [TPP_TOK_EQUAL_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	/* [TPP_TOK_EXCLAIM_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EXCLAIM_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	/* [TPP_TOK_RANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	/* [TPP_TOK_LANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	/* [TPP_TOK_DOT_DOT_DOT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_DOT_DOT_DOT),
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
	/* [TPP_TOK_PLUS_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PLUS_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
	/* [TPP_TOK_MINUS_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
	/* [TPP_TOK_STAR_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
	/* [TPP_TOK_SLASH_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_SLASH_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	/* [TPP_TOK_PERCENT_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PERCENT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	/* [TPP_TOK_LANGLE_LANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	/* [TPP_TOK_RANGLE_RANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
	/* [TPP_TOK_AMP_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_AMP_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
	/* [TPP_TOK_PIPE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PIPE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
	/* [TPP_TOK_HAT_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_HAT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	/* [TPP_TOK_STAR_STAR_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_STAR_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	/* [TPP_TOK_SLASH_SLASH_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_SLASH_SLASH_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_AT_EQUAL
	/* [TPP_TOK_AT_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_AT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
	/* [TPP_TOK_POUND_POUND] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_POUND_POUND),
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
	/* [TPP_TOK_AMP_AMP] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_AMP_AMP),
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
	/* [TPP_TOK_PIPE_PIPE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PIPE_PIPE),
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
	/* [TPP_TOK_HAT_HAT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_HAT_HAT),
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
	/* [TPP_TOK_PLUS_PLUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PLUS_PLUS),
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
	/* [TPP_TOK_MINUS_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
	/* [TPP_TOK_STAR_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_STAR),
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
	/* [TPP_TOK_SLASH_SLASH] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_SLASH_SLASH),
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
	/* [TPP_TOK_TILDE_TILDE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_TILDE_TILDE),
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
	/* [TPP_TOK_TILDE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_TILDE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	/* [TPP_TOK_MINUS_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
	/* [TPP_TOK_COLON_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_COLON_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
	/* [TPP_TOK_COLON_COLON] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_COLON_COLON),
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	/* [TPP_TOK_MINUS_RANGLE_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_RANGLE_STAR),
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_DOT_STAR
	/* [TPP_TOK_DOT_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_DOT_STAR),
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
	/* [TPP_TOK_DOT_DOT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_DOT_DOT),
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	/* [TPP_TOK_LANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	/* [TPP_TOK_LANGLE_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	/* [TPP_TOK_RANGLE_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	/* [TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	/* [TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	/* [TPP_TOK_EQUAL_EQUAL_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_EQUAL_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	/* [TPP_TOK_EXCLAIM_EQUAL_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EXCLAIM_EQUAL_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	/* [TPP_TOK_EXCLAIM_EXCLAIM] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EXCLAIM_EXCLAIM),
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
	/* [TPP_TOK_QMARK_QMARK] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_QMARK_QMARK),
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
	/* [TPP_TOK_QMARK_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_QMARK_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	/* [TPP_TOK_RANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS
	/* [TPP_TOK_EQUAL_PLUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_PLUS),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS
	/* [TPP_TOK_EQUAL_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_MINUS),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
	/* [TPP_TOK_EQUAL_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_STAR),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
	/* [TPP_TOK_EQUAL_STAR_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_STAR_STAR),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
	/* [TPP_TOK_EQUAL_SLASH] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_SLASH),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
	/* [TPP_TOK_EQUAL_SLASH_SLASH] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_SLASH_SLASH),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
	/* [TPP_TOK_EQUAL_PERCENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_PERCENT),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP
	/* [TPP_TOK_EQUAL_AMP] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_AMP),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE
	/* [TPP_TOK_EQUAL_PIPE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_PIPE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT
	/* [TPP_TOK_EQUAL_HAT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_HAT),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	/* [TPP_TOK_EQUAL_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	/* [TPP_TOK_EQUAL_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	/* [TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	/* [TPP_TOK_EQUAL_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	/* [TPP_TOK_EQUAL_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	/* [TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_AT
	/* [TPP_TOK_EQUAL_AT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_AT),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE
	/* [TPP_TOK_EQUAL_TILDE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_TILDE),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON
	/* [TPP_TOK_EQUAL_COLON] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_COLON),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
	/* [TPP_TOK_EQUAL_EXCLAIM] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_EXCLAIM),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	/* [TPP_TOK_EQUAL_EQUAL_EXCLAIM] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_EQUAL_EXCLAIM),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK
	/* [TPP_TOK_EQUAL_QMARK] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_QMARK),
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
	/* [TPP_TOK_LANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	/* [TPP_TOK_STAR_LANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_LANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_DOT
	/* [TPP_TOK_STAR_DOT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_DOT),
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
	/* [TPP_TOK_MINUS_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
	/* [TPP_TOK_RANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	/* [TPP_TOK_LANGLE_EQUAL_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_EQUAL_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	/* [TPP_TOK_LANGLE_MINUS_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_MINUS_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	/* [TPP_TOK_RANGLE_EQUAL_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_EQUAL_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	/* [TPP_TOK_RANGLE_MINUS_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_MINUS_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	/* [TPP_TOK_LANGLE_EQUAL_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_EQUAL_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	/* [TPP_TOK_LANGLE_MINUS_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_MINUS_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	/* [TPP_TOK_RANGLE_EQUAL_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_EQUAL_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	/* [TPP_TOK_RANGLE_MINUS_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_MINUS_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
};
TPP_STATIC_ASSERT(tpp_lengthof(tpp_token_repr_offsets) == TPP_TOK_MULTICHAR_END);

/* Similar to `tpp_strtokenid()', but returns a (canonical) representation of "id" */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_reprtokenid(tpp_token_id id) {
	if ((unsigned int)id < (unsigned int)TPP_TOK_MULTICHAR_END)
		return (char const *)&tpp_token_repr_strings + tpp_token_repr_offsets[(unsigned int)id];
	if ((unsigned int)id < (unsigned int)TPP_TOK_USERKEYWORD_BEGIN) {
		tpp_keyword const *kwd = tpp_builtin_getkeyword_byid(id);
		if (kwd)
			return (char const *)kwd->tk_kwd;
	}
	return NULL;
}
#endif /* TPP_HAVE_REPRTOKENID */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/token-encodestring.c                                     */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_TOKEN_ENCODESTRING

#define TPP_CASE_UTF8_FIRSTBYTE_ISLF_FOREACH(cb) \
	cb(0xc2, "\\xc2") \
	cb(0xe2, "\\xe2")


/* \-encode "data...+=num_bytes" by passing it to "printer"
 * NOTE: Leading/trailing " (or ')-characters are *NOT* printed!
 *
 * @return: >= 0: Sum of positive return values of "printer"
 * @return: < 0:  First negative return value of "printer".
 *                Note that this function never causes errors
 *                on its own, meaning that the meaning of
 *                *all* negative values is entirely up to the
 *                given "printer"! */
TPP_IMPL /*TPP_WUNUSED*/ TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_token_encodestring(tpp_formatprinter printer, void *arg,
                       void const *data, tpp_size num_bytes) {
	char const *output_repr;
	tpp_ssize temp, result = 0;
	tpp_char const *iter = (tpp_char const *)data;
	tpp_char const *end  = iter + num_bytes;
	tpp_char ch;
again:
	if (iter >= end) {
		temp = (*printer)(arg, (tpp_char const *)data, (tpp_size)(end - (tpp_char const *)data));
		if (temp < 0)
			return temp;
		result += temp;
		return result;
	}
	ch = *iter++;
	switch (ch) {
#define TPP_TOKEN_ENCODESTRING_CASE(b, repr) \
	case b:                                  \
		output_repr = repr;                  \
		break;

	/* Only really need to escape \ " ' CR LF and (TPP_HAVE_UNICODE-only)
	 * ordinals >=0xC0 that *might* form unicode line-feed characters. */

	TPP_TOKEN_ENCODESTRING_CASE('\\', "\\\\");
	TPP_TOKEN_ENCODESTRING_CASE('\'', "\\'");
	TPP_TOKEN_ENCODESTRING_CASE('\"', "\\\"");
	TPP_TOKEN_ENCODESTRING_CASE('\r', "\\r");
	TPP_TOKEN_ENCODESTRING_CASE('\n', "\\n");

#if TPP_HAVE_UNICODE
	TPP_CASE_UTF8_FIRSTBYTE_ISLF_FOREACH(TPP_TOKEN_ENCODESTRING_CASE)
#endif /* TPP_HAVE_UNICODE */
#undef TPP_TOKEN_ENCODESTRING_CASE
	default: goto again;
	}
	temp = (*printer)(arg, (tpp_char const *)data,
	                  (tpp_size)((iter - 1) -
	                             (tpp_char const *)data));
	if (temp < 0)
		return temp;
	result += temp;
#if TPP_HAVE_UNICODE
	temp = (*printer)(arg, (tpp_char const *)output_repr, tpp_strlen(output_repr));
#else /* TPP_HAVE_UNICODE */
	/* All mandatory ASCII-escape-sequences are 2 bytes long! */
	temp = (*printer)(arg, (tpp_char const *)output_repr, 2);
#endif /* !TPP_HAVE_UNICODE */
	if (temp < 0)
		return temp;
	result += temp;
	data = (void const *)iter;
	goto again;
}
#endif /* TPP_HAVE_TOKEN_ENCODESTRING */


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/file-io.c                                                */
/************************************************************************/
#ifdef tpp_io_handle_IS_BUILTIN

#ifdef tpp_io_handle_IS_HANDLE
#include <Windows.h>
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
#include <fcntl.h>
#ifdef _MSC_VER
#include <io.h>
#else /* _MSC_VER */
#include <unistd.h>
#if TPP_HAVE_FILE_NONBLOCK
#include <sys/select.h>
#endif /* !TPP_HAVE_FILE_NONBLOCK */
#endif /* !_MSC_VER */
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
#include <stdio.h>
#endif /* tpp_io_handle_IS_FILE */

TPP_DECL_BEGIN

/* Open a file for reading
 * @return: tpp_io_handle_INVALID: No such file or directory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_io_handle TPPCALL
tpp_io_open(/*utf-8*/ char const *filename) {
#ifdef tpp_io_handle_IS_HANDLE
	DWORD const dwDesiredAccess       = FILE_GENERIC_READ;
	DWORD const dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
	DWORD const dwFlagsAndAttributes  = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS;
	DWORD const dwCreationDisposition = OPEN_EXISTING;
	HANDLE hFile;

	hFile = CreateFileA(filename, dwDesiredAccess, dwShareMode, NULL,
	                    dwCreationDisposition, dwFlagsAndAttributes, NULL);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
		return hFile;
	/* TODO: Convert utf-8 to wide, then pre-pend \\.\ to work around UNC limitations */
	return tpp_io_handle_INVALID;
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
	int const mode = O_RDONLY |
#ifdef O_BINARY
	                 O_BINARY |
#endif /* O_BINARY */
#ifdef O_CLOEXEC
	                 O_CLOEXEC |
#endif /* O_CLOEXEC */
	                 0;
	return open(filename, mode, 0);
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	return fopen(filename, "rb");
#endif /* tpp_io_handle_IS_FILE */
}

/* Close a file previously opened by `tpp_io_open()' */
TPP_IMPL void TPPCALL tpp_io_close(tpp_io_handle file) {
#ifdef tpp_io_handle_IS_HANDLE
	(void)CloseHandle(file);
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
	(void)close(file);
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	(void)fclose(file);
#endif /* tpp_io_handle_IS_FILE */
}

/* Read data from a given `file' into `buf'
 * @return: * : The # of bytes read into `buf' (at most `bufsize')
 * @return: (tpp_ssize)TPP_EIO:         I/O error
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: (tpp_ssize)TPP_EWOULDBLOCK: `nonblock' was given, but operation would block
 * #endif // TPP_HAVE_FILE_NONBLOCK */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_io_read(tpp_io_handle file, void *buf,
            tpp_size bufsize tpp_io_nonblock__PARAM) {
#ifdef tpp_io_handle_IS_HANDLE
	DWORD dwResult;
	DWORD dwBufsize = (DWORD)bufsize;
#if TPP_SIZEOF_tpp_size > 4
	if (bufsize > UINT32_C(0xffffffff))
		dwBufsize = UINT32_C(0xffffffff);
#endif /* TPP_SIZEOF_tpp_size > 4 */

#if TPP_HAVE_FILE_NONBLOCK
	if (nonblock) {
		DWORD dwFileType = GetFileType(file);
		if (dwFileType == FILE_TYPE_UNKNOWN)
			return (tpp_ssize)TPP_EIO;
		if (dwFileType == FILE_TYPE_PIPE) {
			BYTE temp_buffer[1];
			/* `WaitForSingleObject()' doesn't work on pipes (for some reason...) */
			dwResult = 0;
			if (PeekNamedPipe(file, temp_buffer, sizeof(temp_buffer),
			                  &dwResult, NULL, NULL) &&
			    dwResult == 0)
				return (tpp_ssize)TPP_EWOULDBLOCK;
		} else {
			dwResult = WaitForSingleObject(file, 0);
			if (dwResult == WAIT_TIMEOUT)
				return (tpp_ssize)TPP_EWOULDBLOCK;
		}
	}
#endif /* !TPP_HAVE_FILE_NONBLOCK */

	if (!ReadFile(file, buf, dwBufsize, &dwResult, NULL))
		return TPP_EIO;
	return (tpp_ssize)dwResult;
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
#if TPP_HAVE_FILE_NONBLOCK
	if (nonblock) {
		fd_set read_fds;
		struct timeval timeout = { 0, 0 };
		int result;
		FD_ZERO(&read_fds);
		FD_SET(file, &read_fds);
		result = select(file + 1, &read_fds, NULL, NULL, &timeout);
		if (result < 0)
			return (tpp_ssize)TPP_EIO;
		if (!FD_ISSET(fd, &read_fds))
			return (tpp_ssize)TPP_EWOULDBLOCK;
	}
#endif /* !TPP_HAVE_FILE_NONBLOCK */
	return (tpp_ssize)read(file, buf, bufsize);
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	tpp_size result = (tpp_size)fread(buf, 1, bufsize, file);
	if (result == 0 && ferror(file))
		return (tpp_ssize)TPP_EIO;
	return (tpp_ssize)result;
#endif /* tpp_io_handle_IS_FILE */
}

TPP_DECL_END
#endif /* tpp_io_handle_IS_BUILTIN */
/************************************************************************/

/************************************************************************/
/* File: parts/file.c                                                   */
/************************************************************************/
TPP_DECL_BEGIN


#if TPP_HAVE_FILE_USER_FILENAME
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_user_filename) ==
                  tpp_offsetof(tpp_file, tf_data.td_text.tft_user_filename));
#endif /* TPP_HAVE_FILE_USER_FILENAME */

#if TPP_DEBUG && 1
#ifndef TPP_IO_CHUNKSIZE
#define TPP_IO_CHUNKSIZE 1
#endif /* !TPP_IO_CHUNKSIZE */
#ifndef TPP_IO_MINREAD
#define TPP_IO_MINREAD 1
#endif /* !TPP_IO_MINREAD */
#else /* TPP_DEBUG */
#ifndef TPP_IO_CHUNKSIZE
#define TPP_IO_CHUNKSIZE (64 * 1024)
#endif /* !TPP_IO_CHUNKSIZE */
#ifndef TPP_IO_MINREAD
#define TPP_IO_MINREAD 1024
#endif /* !TPP_IO_MINREAD */
#endif /* !TPP_DEBUG */


#if TPP_HAVE_IFDEF_STACK
/* Allocate an additional #ifdef-stack entry, and return a pointer to it.
 * This function will increment `self->tids_cnt', but it is up to the
 * caller to initialize the returned #ifdef-stack entry
 *
 * @return: * :   The (uninitialized) #ifdef-stack entry
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_ifdef_stack_entry *TPPCALL
tpp_ifdef_stack_append(tpp_ifdef_stack *tpp_restrict self) {
	tpp_ifdef_stack_entry *result;
	tpp_assert(self->tids_cnt <= self->tids_alc);
	if (self->tids_cnt >= self->tids_alc) {
		tpp_ifdef_stack_entry *new_vec;
		/* Only allocate in batches-of-1: #ifdef-stacks don't usually grow very deep */
		tpp_size new_alloc = self->tids_cnt + 1;

		/* Use a minimal buffer size of "4": most files won't exceed this depth. */
		if (new_alloc < 4)
			new_alloc = 4;
		new_vec = (tpp_ifdef_stack_entry *)tpp_tryrealloc(self->tids_vec,
		                                                  new_alloc *
		                                                  sizeof(tpp_ifdef_stack_entry));
		if tpp_unlikely(!new_vec) {
			new_alloc = self->tids_cnt + 1;
			new_vec = (tpp_ifdef_stack_entry *)tpp_realloc(self->tids_vec,
			                                               new_alloc *
			                                               sizeof(tpp_ifdef_stack_entry));
			if tpp_unlikely (!new_vec)
				return NULL;
		}
		self->tids_vec = new_vec;
		self->tids_alc = new_alloc;
	}
	tpp_assert(self->tids_cnt < self->tids_alc);
	result = self->tids_vec + self->tids_cnt;
	++self->tids_cnt;
#if TPP_DEBUG
	tpp_memset(result, 0xcc, sizeof(*result));
#endif /* TPP_DEBUG */
	return result;
}

/* Do the inverse of "tpp_ifdef_stack_append()", removing the last entry, and
 * potentially freeing unused memory.
 * The caller is responsible to ensure that `tpp_ifdef_stack_isnonempty(self)' */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_ifdef_stack_remove(tpp_ifdef_stack *tpp_restrict self) {
	tpp_assert(self->tids_cnt != 0);
	tpp_assert(self->tids_cnt <= self->tids_alc);
	--self->tids_cnt;
#ifndef __OPTIMIZE_SIZE__
	{
		tpp_size unused_count = self->tids_alc - self->tids_cnt;
		if (unused_count > 8) {
			/* Free unused memory */
			tpp_ifdef_stack_entry *new_vec;
			new_vec = (tpp_ifdef_stack_entry *)tpp_tryrealloc(self->tids_vec,
			                                                  self->tids_alc *
			                                                  sizeof(tpp_ifdef_stack_entry));
			if tpp_likely(new_vec) {
				self->tids_vec = new_vec;
				self->tids_alc = self->tids_cnt;
			}
		}
	}
#endif /* !__OPTIMIZE_SIZE__ */
}
#endif /* TPP_HAVE_IFDEF_STACK */


/* Finalize the given file. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_file_fini(tpp_file *tpp_restrict self) {
	if (self->tf_chunk)
		tpp_string_decref(self->tf_chunk);
#if TPP_HAVE_IFDEF_STACK
	tpp_ifdef_stack_fini(&self->tf_ifdef);
#endif /* TPP_HAVE_IFDEF_STACK */
	switch (self->tf_kind) {
	case TPP_FILE_KIND_IO:
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
		if (self->tf_pos >= self->tf_end &&
#if TPP_HAVE_FILE_NOKWD
		    !(self->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
		    self->tf_data.td_io.tff_name) {
			/* If the file's keyword still has a valid "tkm_file_guard",
			 * then we can set its "TPP_KEYWORD_FLAG_HDR_GUARD_VALID" flag. */
			tpp_keyword *kwd;
			tpp_keyword_misc *misc;
			kwd = (tpp_keyword *)((char const *)self->tf_data.td_io.tff_name -
			                      tpp_offsetof(tpp_keyword, tk_kwd));
			misc = tpp_keyword_getmisc(kwd);
			if (misc && misc->tkm_file_guard) {
				/* Yes! We got a #ifndef-style #include-guard for this file now! */
				misc->tkm_flags |= TPP_KEYWORD_FLAG_HDR_GUARD_VALID;
			}
		}
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_FILE_NOCLOSE
		if (!(self->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOCLOSE))
#endif /* TPP_HAVE_FILE_NOCLOSE */
		{
			tpp_io_close(self->tf_data.td_io.tff_file);
		}
#if TPP_HAVE_FILE_USER_FILENAME
		TPP_FALLTHRU
	case TPP_FILE_KIND_TEXT:
		if (self->tf_data.td_text.tft_user_filename)
			tpp_string_decref(self->tf_data.td_text.tft_user_filename);
		break;
#endif /* TPP_HAVE_FILE_USER_FILENAME */
#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO: {
		tpp_macro *macro = self->tf_data.td_macro.tfm_macro;
		tpp_assert(macro->tm_expansions != 0);
		--macro->tm_expansions;
		tpp_macro_decref(macro);
	}	break;
#endif /* TPP_HAVE_CPP_MACROS */
	default: break;
	}
}

/* Update "self" according to text-data from [text,text+size) */
static TPP_WUNUSED tpp_lcinfo TPPCALL
tpp_lcinfo_account(tpp_file const *tpp_restrict self, tpp_lcinfo lc,
                   tpp_char const *text, tpp_size size) {
	tpp_char const *endp = text + size;
	tpp_line line  = tpp_lcinfo_getline(lc);
	tpp_column col = tpp_lcinfo_getcol(lc);
	(void)self;
	while (text < endp) {
		tpp_char ch = *text++;
		switch (ch) {

		case '\t': {
			tpp_column ts = tpp_gettabsize();
			col += ts;
			col -= col % ts;
		}	break;

		case '\r':
			if (text < endp && *text == '\n')
				++text;
			TPP_FALLTHRU
		case '\n': {
#if TPP_HAVE_UNICODE
handle_linefeed:
#endif /* TPP_HAVE_UNICODE */
			++line;
			col = 0;

		}	break;

		default:
#if TPP_HAVE_UNICODE
			if (tpp_file_isutf8(self) && ch >= 0x80) {
				/* Check for unicode linefeed characters */
				tpp_unichar uch;
				--text;
				uch = tpp_unicode_readutf8(&text, endp);
				if (tpp_unicode_islf(uch))
					goto handle_linefeed;
			}
#endif /* TPP_HAVE_UNICODE */
			++col;
			break;
		}
	}
	return tpp_lcinfo_of(line, col);
}


#if TPP_HAVE_FILE_LC_CACHE
/* Find+return a pointer 1 past the last line-feed in [ptr,end)
 * If no such linefeed exists, return "NULL" instead. */
static TPP_WUNUSED tpp_char const *TPPCALL
tpp_lcinfo_find_last_linefeed(tpp_file const *tpp_restrict self,
                              tpp_char const *ptr, tpp_char const *end) {
	(void)self;
#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(self)) {
		while (end > ptr) {
			tpp_unichar uc;
			tpp_char const *nend = end;
			uc = tpp_unicode_readutf8_rev(&nend, ptr);
			if (tpp_unicode_islf(uc))
				return end;
			end = nend;
		}
	} else
#endif /* TPP_HAVE_UNICODE */
	{
		while (end > ptr) {
			tpp_char ch = end[-1];
			if (tpp_ascii_islf(ch))
				return end;
			--end;
		}
	}
	return NULL;
}

/* Count the # of linefeeds in [ptr,end) */
static TPP_WUNUSED tpp_line TPPCALL
tpp_lcinfo_count_linefeed(tpp_file const *tpp_restrict self,
                          tpp_char const *ptr, tpp_char const *end) {
	tpp_line result = 0;
	(void)self;
#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(self)) {
		while (ptr < end) {
			tpp_unichar uc;
			uc = tpp_unicode_readutf8_rev(&end, ptr);
			if (tpp_unicode_islf(uc))
				++result;
		}
	} else
#endif /* TPP_HAVE_UNICODE */
	{
		while (ptr < end) {
			tpp_char ch = *ptr++;
			if (tpp_ascii_islf(ch))
				++result;
		}
	}
	return result;
}
#endif /* TPP_HAVE_FILE_LC_CACHE */


#if TPP_HAVE_UNICODE
#define TPP_UNICODE_16TO8_MAXBUF(num_chars) ((num_chars) * 3) /* Max # of bytes needed to convert UTF-16 to UTF-8 */
#define TPP_UNICODE_32TO8_MAXBUF(num_chars) ((num_chars) * 7) /* Max # of bytes needed to convert UTF-32 to UTF-8 */

#define TPP_FILE_UTF16_IOSIZE(bufsize) (((bufsize) / 3) * 2) /* # of bytes to read from underlying UTF-16 file */
#define TPP_FILE_UTF32_IOSIZE(bufsize) (((bufsize) / 7) * 4) /* # of bytes to read from underlying UTF-32 file */

#define TPP_FILE_UTF16_MINEXTRA 3  /* max(2, TPP_UNICODE_16TO8_MAXBUF(1)) */
#define TPP_FILE_UTF32_MINEXTRA 7  /* max(4, TPP_UNICODE_32TO8_MAXBUF(1)) */
#define TPP_FILE_MINEXTRA       10 /* max(1, TPP_FILE_UTF16_MINEXTRA, TPP_FILE_UTF32_MINEXTRA) + lengthof(tff_tailv) */

static tpp_char *TPPCALL
tpp_writeutf8_rev(tpp_char *dst, tpp_unichar uc) {
	if (uc <= TPP_UTF8_1BYTE_MAX) {
		*--dst = (uint8_t)uc;
	} else if (uc <= TPP_UTF8_2BYTE_MAX) {
		*--dst = 0x80 | (uint8_t)((uc) & 0x3f);
		*--dst = 0xc0 | (uint8_t)((uc >> 6) /* & 0x1f*/);
	} else if (uc <= TPP_UTF8_3BYTE_MAX) {
		*--dst = 0x80 | (uint8_t)((uc) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*--dst = 0xe0 | (uint8_t)((uc >> 12) /* & 0x0f*/);
	} else if (uc <= TPP_UTF8_4BYTE_MAX) {
		*--dst = 0x80 | (uint8_t)((uc) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*--dst = 0xf0 | (uint8_t)((uc >> 18) /* & 0x07*/);
	} else if (uc <= TPP_UTF8_5BYTE_MAX) {
		*--dst = 0x80 | (uint8_t)((uc) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 18) & 0x3f);
		*--dst = 0xf8 | (uint8_t)((uc >> 24) /* & 0x03*/);
	} else if (uc <= TPP_UTF8_6BYTE_MAX) {
		*--dst = 0x80 | (uint8_t)((uc) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 18) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 24) & 0x3f);
		*--dst = 0xfc | (uint8_t)((uc >> 30) /* & 0x01*/);
	} else {
		*--dst = 0x80 | (uint8_t)((uc) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 18) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 24) & 0x3f);
		*--dst = 0x80 | (uint8_t)((uc >> 30) & 0x03 /* & 0x3f*/);
		*--dst = 0xfe;
	}
	return dst;
}

#define TPP_UTF16_HIGH_SURROGATE_MIN 0xd800
#define TPP_UTF16_HIGH_SURROGATE_MAX 0xdbff
#define TPP_UTF16_LOW_SURROGATE_MIN  0xdc00
#define TPP_UTF16_LOW_SURROGATE_MAX  0xdfff
#define TPP_UTF16_SURROGATE_SHIFT    0x10000

union tpp_word16 {
	uint_least16_t w16;
	uint_least8_t w8[2];
#define tpp_word16_getle(self)        \
	(((uint_least16_t)(self).w8[0]) | \
	 ((uint_least16_t)(self).w8[1] << 8))
#define tpp_word16_getbe(self)        \
	(((uint_least16_t)(self).w8[1]) | \
	 ((uint_least16_t)(self).w8[0] << 8))
};

union tpp_word32 {
	uint_least32_t w32;
	uint_least8_t w8[4];
#define tpp_word32_getle(self)              \
	(((uint_least32_t)(self).w8[0]) |       \
	 ((uint_least32_t)(self).w8[1] << 8) |  \
	 ((uint_least32_t)(self).w8[2] << 16) | \
	 ((uint_least32_t)(self).w8[3] << 24))
#define tpp_word32_getbe(self)              \
	(((uint_least32_t)(self).w8[3]) |       \
	 ((uint_least32_t)(self).w8[2] << 8) |  \
	 ((uint_least32_t)(self).w8[1] << 16) | \
	 ((uint_least32_t)(self).w8[0] << 24))
};

/* @param: src_count: # of utf-16 words in "src" (== # of bytes/2) */
static tpp_char *TPPCALL
tpp_utf16le_to_utf8(uint_least16_t const *src, tpp_size src_count, tpp_char *dst_end) {
	for (src += src_count; src_count; --src_count) {
		uint_least16_t ord;
		union tpp_word16 word;
		word.w16 = *--src;
		ord = tpp_word16_getle(word);
		if (ord >= TPP_UTF16_LOW_SURROGATE_MIN &&
		    ord <= TPP_UTF16_LOW_SURROGATE_MAX && src_count) {
			/* Second word of surrogate pair */
			tpp_unichar uc;
			word.w16 = *--src;
			--src_count;
			uc = tpp_word16_getle(word);
			/* "ord_lo" *should* be in range:
			 * [TPP_UTF16_HIGH_SURROGATE_MIN,TPP_UTF16_HIGH_SURROGATE_MAX] */
			uc &= (TPP_UTF16_HIGH_SURROGATE_MAX - TPP_UTF16_HIGH_SURROGATE_MIN);
			uc <<= 10;
			uc |= ord - TPP_UTF16_LOW_SURROGATE_MIN;
			uc += TPP_UTF16_SURROGATE_SHIFT;
			dst_end = tpp_writeutf8_rev(dst_end, uc);
		} else {
			dst_end = tpp_writeutf8_rev(dst_end, ord);
		}
	}
	return dst_end;
}

static tpp_char *TPPCALL
tpp_utf16be_to_utf8(uint_least16_t const *src, tpp_size src_count, tpp_char *dst_end) {
	for (src += src_count; src_count; --src_count) {
		uint_least16_t ord;
		union tpp_word16 word;
		word.w16 = *--src;
		ord = tpp_word16_getbe(word);
		if (ord >= TPP_UTF16_LOW_SURROGATE_MIN &&
		    ord <= TPP_UTF16_LOW_SURROGATE_MAX && src_count) {
			/* Second word of surrogate pair */
			tpp_unichar uc;
			word.w16 = *--src;
			--src_count;
			uc = tpp_word16_getbe(word);
			/* "ord_lo" *should* be in range:
			 * [TPP_UTF16_HIGH_SURROGATE_MIN,TPP_UTF16_HIGH_SURROGATE_MAX] */
			uc &= (TPP_UTF16_HIGH_SURROGATE_MAX - TPP_UTF16_HIGH_SURROGATE_MIN);
			uc <<= 10;
			uc |= ord - TPP_UTF16_LOW_SURROGATE_MIN;
			uc += TPP_UTF16_SURROGATE_SHIFT;
			dst_end = tpp_writeutf8_rev(dst_end, uc);
		} else {
			dst_end = tpp_writeutf8_rev(dst_end, ord);
		}
	}
	return dst_end;
}

/* @param: src_count: # of utf-32 words in "src" (== # of bytes/4) */
static tpp_char *TPPCALL
tpp_utf32le_to_utf8(uint_least32_t const *src, tpp_size src_count, tpp_char *dst_end) {
	for (src += src_count; src_count; --src_count) {
		uint_least32_t ord;
		union tpp_word32 word;
		word.w32 = *--src;
		ord = tpp_word32_getle(word);
		dst_end = tpp_writeutf8_rev(dst_end, ord);
	}
	return dst_end;
}

static tpp_char *TPPCALL
tpp_utf32be_to_utf8(uint_least32_t const *src, tpp_size src_count, tpp_char *dst_end) {
	for (src += src_count; src_count; --src_count) {
		uint_least32_t ord;
		union tpp_word32 word;
		word.w32 = *--src;
		ord = tpp_word32_getbe(word);
		dst_end = tpp_writeutf8_rev(dst_end, ord);
	}
	return dst_end;
}
#else /* TPP_HAVE_UNICODE */
#define TPP_FILE_MINEXTRA 1
#endif /* !TPP_HAVE_UNICODE */

/* The minimum amount of extra read bytes must allow for the worst-case
 * buffer limits imposed by unicode conversion and tail buffering. */
#if TPP_IO_MINREAD < TPP_FILE_MINEXTRA
#undef TPP_IO_MINREAD
#define TPP_IO_MINREAD TPP_FILE_MINEXTRA
#endif /* TPP_IO_MINREAD < TPP_FILE_MINEXTRA */



/* Try to expand the currently loaded `self->tf_chunk':
 * - If the file's kind isn't `TPP_FILE_KIND_IO', return `TPP_EOK'
 * - Allocate a new `tpp_string' suitable for holding both
 *   [tf_pos,tf_end), as well as at least 1 additional byte.
 *   HINT: When `!tpp_string_isshared(self->tf_chunk)', the old
 *         chunk string may simply be re-used (since it'd be free'd
 *         during the re-assignment below anyways). If this isn't
 *         intended (iow: you want to keep the contents of the previous
 *         chunk loaded into memory), you can simply `tpp_string_incref'
 *         it before calling this function.
 * - Copy [tf_pos,tf_end) into this new string
 * - Read from the underlying file into the tail of the new string
 * #if TPP_HAVE_UNICODE
 *   - If the underlying file's encoding is TPP_FILE_ENCODING_UTF(16|32)_(LE|BE),
 *     the read data is converted to utf-8 at this point.
 * #endif // TPP_HAVE_UNICODE
 *   - If the underlying file could not be read, return `TPP_EIO'
 *   - If nothing could be read, free the new string and return `TPP_EOK'
 *   - Else:
 *     - adjust `tf_pos' to point into the new string, and
 *       set `tf_end' to point at the end of the new string.
 *     - replace `tf_chunk' with the new string
 *     - return `TPP_EOK'
 * @return: TPP_EOK:         Either the current chunk was expanded (the delta
 *                           between `tf_pos' and `tf_end' has increased), or
 *                           no further data can be read from `self'.
 * @return: TPP_EIO:         I/O error
 * @return: TPP_ENOMEM:      Out of memory
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: TPP_EWOULDBLOCK: Operation would block.
 * #endif // TPP_HAVE_FILE_NONBLOCK */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_file_expandchunk(tpp_file *tpp_restrict self) {
	/* TLDR: retain [self->tf_pos,self->tf_end) and make sure that "self->tf_end"
	 *       is increased by at least 1 byte. If that is impossible due to EOF,
	 *       don't increase "self->tf_end", but still return EOK. */
	tpp_size old_inuse, new_size, io_size;
	TPP_REF tpp_string *old_chunk;
	TPP_REF tpp_string *new_chunk;
	tpp_ssize read_status;
	tpp_char *io_dst;
	tpp_char const *base;
#if TPP_HAVE_UNICODE
	bool is_first_chunk;
again:
#endif /* TPP_HAVE_UNICODE */

	if (self->tf_kind != TPP_FILE_KIND_IO)
		return TPP_EOK;
	base = self->tf_pos;
#if TPP_HAVE_FILE_KEEPPOS
	if (self->tf_data.td_io.ttf_keep && base > self->tf_data.td_io.ttf_keep)
		base = self->tf_data.td_io.ttf_keep;
#endif /* TPP_HAVE_FILE_KEEPPOS */
	old_inuse = (tpp_size)(self->tf_end - base);
	old_chunk = self->tf_chunk;
	new_size  = TPP_IO_CHUNKSIZE;
	if (new_size < old_inuse + TPP_IO_MINREAD)
		new_size = old_inuse + TPP_IO_MINREAD;
	tpp_assert(base <= self->tf_end);
	tpp_assert(!old_chunk || base >= tpp_string_str(old_chunk));
	tpp_assert(!old_chunk || self->tf_end <= tpp_string_end(old_chunk));
	if (old_chunk && !tpp_string_isshared(old_chunk)) {
		/* Can re-use the old chunk */
		tpp_size unused_head;
		unused_head = (tpp_size)(base - tpp_string_str(old_chunk));
#ifndef __OPTIMIZE_SIZE__
		if (unused_head)
#endif /* !__OPTIMIZE_SIZE__ */
		{
			self->tf_data.td_io.tff_start_lc = tpp_lcinfo_account(self,
			                                                      self->tf_data.td_io.tff_start_lc,
			                                                      tpp_string_str(old_chunk), unused_head);
			tpp_memmovedown(tpp_string_str(old_chunk), base, old_inuse);
			base -= unused_head;
			self->tf_pos -= unused_head;
			self->tf_end -= unused_head;
#if TPP_HAVE_FILE_KEEPPOS
			if (self->tf_data.td_io.ttf_keep)
				self->tf_data.td_io.ttf_keep -= unused_head;
#endif /* TPP_HAVE_FILE_KEEPPOS */
#if TPP_HAVE_FILE_LC_CACHE
			if (self->tf_lcpos) {
				self->tf_lcpos -= unused_head;
				if (self->tf_lcpos < base)
					self->tf_lcpos = NULL; /* Cache fell out-of-scope */
			}
#endif /* TPP_HAVE_FILE_LC_CACHE */
		}
		tpp_assert(base == tpp_string_str(old_chunk));
		tpp_assert(self->tf_end == tpp_string_str(old_chunk) + old_inuse);
		if (tpp_string_len(old_chunk) >= new_size) {
			/* Don't even need to realloc() the old chunk! */
reuse_old_chunk:
			new_chunk = old_chunk;
			new_size  = tpp_string_len(old_chunk);
		} else {
			/* Must realloc() the old chunk */
#if TPP_HAVE_FILE_LC_CACHE
			tpp_size lc_rel = (tpp_size)(self->tf_lcpos - tpp_string_str(old_chunk));
#endif /* TPP_HAVE_FILE_LC_CACHE */
#if TPP_HAVE_FILE_KEEPPOS
			tpp_size ps_rel = (tpp_size)(self->tf_pos - base);
			tpp_size kp_rel = (tpp_size)(self->tf_data.td_io.ttf_keep - base);
#endif /* TPP_HAVE_FILE_KEEPPOS */
			new_chunk = (TPP_REF tpp_string *)tpp_tryrealloc(old_chunk, tpp_string_sizeof(new_size));
			if tpp_unlikely(!new_chunk) {
				new_size = old_inuse + TPP_FILE_MINEXTRA;
				if (tpp_string_len(old_chunk) >= new_size)
					goto reuse_old_chunk;
				new_chunk = (TPP_REF tpp_string *)tpp_realloc(old_chunk, tpp_string_sizeof(new_size));
				if tpp_unlikely(!new_chunk)
					return TPP_ENOMEM;
			}
			tpp_assert(!tpp_string_isshared(new_chunk));
			new_chunk->ts_str[new_size] = '\0';
			new_chunk->ts_len = new_size;
			base = tpp_string_str(new_chunk);
#if TPP_HAVE_FILE_KEEPPOS
			self->tf_pos = base + ps_rel;
			if (self->tf_data.td_io.ttf_keep) {
				self->tf_data.td_io.ttf_keep = base + kp_rel;
			} else {
				tpp_assert(ps_rel == 0 && "Without a keep-pointer, there should be no delta between 'tf_pos' and 'base'");
			}
#else /* TPP_HAVE_FILE_KEEPPOS */
			self->tf_pos = base;
#endif /* !TPP_HAVE_FILE_KEEPPOS */
			self->tf_end = base + old_inuse;
#if TPP_HAVE_FILE_LC_CACHE
			if (self->tf_lcpos)
				self->tf_lcpos = base + lc_rel;
#endif /* TPP_HAVE_FILE_LC_CACHE */
		}
		self->tf_chunk = new_chunk;
#if TPP_HAVE_UNICODE
		is_first_chunk = false;
#endif /* TPP_HAVE_UNICODE */
	} else {
#if TPP_HAVE_FILE_KEEPPOS
		tpp_size ps_rel;
		tpp_size kp_rel;
#endif /* TPP_HAVE_FILE_KEEPPOS */
		/* Must allocate a completely new chunk */
		new_chunk = tpp_string_trymalloc(new_size);
		if tpp_unlikely(!new_chunk) {
			new_size = old_inuse + TPP_FILE_MINEXTRA;
			new_chunk = tpp_string_malloc(new_size);
			if tpp_unlikely(!new_chunk)
				return TPP_ENOMEM;
		}
		tpp_memcpy(tpp_string_str(new_chunk), base, old_inuse);
		if (old_chunk) {
			tpp_size unused_head = (tpp_size)(base - tpp_string_str(old_chunk));
#if TPP_HAVE_FILE_KEEPPOS
			ps_rel = (tpp_size)(self->tf_pos - base);
			kp_rel = (tpp_size)(self->tf_data.td_io.ttf_keep - base);
#endif /* TPP_HAVE_FILE_KEEPPOS */
			self->tf_data.td_io.tff_start_lc = tpp_lcinfo_account(self,
			                                                      self->tf_data.td_io.tff_start_lc,
			                                                      tpp_string_str(old_chunk), unused_head);
			tpp_assert(tpp_string_isshared(old_chunk));
			tpp_string_decref_nokill(old_chunk);
#if TPP_HAVE_UNICODE
			is_first_chunk = false;
#endif /* TPP_HAVE_UNICODE */
		} else {
			tpp_assert(self->tf_pos == self->tf_end);
#if TPP_HAVE_FILE_KEEPPOS
			tpp_assert(self->tf_data.td_io.ttf_keep == NULL ||
			           self->tf_data.td_io.ttf_keep == self->tf_pos);
			ps_rel = 0;
			kp_rel = 0;
#endif /* TPP_HAVE_FILE_KEEPPOS */
			tpp_lcinfo_init(self->tf_data.td_io.tff_start_lc, 0, 0);
#if TPP_HAVE_UNICODE
			is_first_chunk = true;
#endif /* TPP_HAVE_UNICODE */
		}
#if TPP_HAVE_FILE_LC_CACHE
		if (self->tf_lcpos) {
			self->tf_lcpos = tpp_string_str(new_chunk) + (self->tf_lcpos - base);
			if (self->tf_lcpos < tpp_string_str(new_chunk))
				self->tf_lcpos = NULL; /* Cache fell out-of-scope */
		}
#endif /* TPP_HAVE_FILE_LC_CACHE */
		base = tpp_string_str(new_chunk);
#if TPP_HAVE_FILE_KEEPPOS
		self->tf_pos = base + ps_rel;
		if (self->tf_data.td_io.ttf_keep) {
			self->tf_data.td_io.ttf_keep = base + kp_rel;
		} else {
			tpp_assert(ps_rel == 0 && "Without a keep-pointer, there should be no delta between 'tf_pos' and 'base'");
		}
#else /* TPP_HAVE_FILE_KEEPPOS */
		self->tf_pos = base;
#endif /* !TPP_HAVE_FILE_KEEPPOS */
		self->tf_end   = base + old_inuse;
		self->tf_chunk = new_chunk; /* Inherit reference */
	}

	tpp_assert(self->tf_chunk == new_chunk);
	tpp_assert(base == tpp_string_str(new_chunk));
	tpp_assert(self->tf_pos >= tpp_string_str(new_chunk));
	tpp_assert(self->tf_end <= tpp_string_end(new_chunk));
	tpp_assert(self->tf_pos <= self->tf_end);
#if TPP_HAVE_FILE_KEEPPOS
	tpp_assert((self->tf_data.td_io.ttf_keep == NULL) ||
	           (self->tf_data.td_io.ttf_keep >= tpp_string_str(new_chunk) &&
	            self->tf_data.td_io.ttf_keep <= tpp_string_end(new_chunk)));
#endif /* TPP_HAVE_FILE_KEEPPOS */
	io_size = (tpp_size)(tpp_string_end(new_chunk) - self->tf_end);
	tpp_assert(io_size >= 1 && "Reallocations above should have ensured sufficient space!");
	io_dst = (tpp_char *)self->tf_end;
#if TPP_HAVE_UNICODE
	switch (self->tf_enc) {
	case TPP_FILE_ENCODING_UTF16_LE:
	case TPP_FILE_ENCODING_UTF16_BE:
		io_size = TPP_FILE_UTF16_IOSIZE(io_size);
amend_tail_data:
		if (self->tf_data.td_io.tff_tailc) {
			memcpy(io_dst, self->tf_data.td_io.tff_tailv, self->tf_data.td_io.tff_tailc);
			io_dst += self->tf_data.td_io.tff_tailc;
			io_size -= self->tf_data.td_io.tff_tailc;
		}
		break;
	case TPP_FILE_ENCODING_UTF32_LE:
	case TPP_FILE_ENCODING_UTF32_BE:
		io_size = TPP_FILE_UTF32_IOSIZE(io_size);
		goto amend_tail_data;
	default: break;
	}
	tpp_assert(io_size >= 1 && "Value of 'TPP_FILE_MINEXTRA' should have ensured this");
#endif /* TPP_HAVE_UNICODE */


	/* Do an I/O read from the underlying file */
#if TPP_HAVE_FILE_NONBLOCK
	read_status = tpp_io_read(self->tf_data.td_io.tff_file,
	                          io_dst, io_size,
	                          self->tf_data.td_io.tff_flags &
	                          TPP_FILE_IOFLAGS_NONBLOCK);
#else /* TPP_HAVE_FILE_NONBLOCK */
	read_status = tpp_io_read(self->tf_data.td_io.tff_file,
	                          io_dst, io_size);
#endif /* !TPP_HAVE_FILE_NONBLOCK */

	/* Check for errors that may have happened during the read */
	if (read_status < 0)
		return (tpp_errno)read_status;

	/* Detect codec + convert to utf-8 */
#if TPP_HAVE_UNICODE
	switch (self->tf_enc) {
	case TPP_FILE_ENCODING_UTF8:
		if (is_first_chunk) {
			self->tf_data.td_io.tff_tailc = 0;

			/* Detect BOM and multi-byte encodings */
			if (read_status >= 3 && (io_dst[0] == 0xef && io_dst[1] == 0xbb && io_dst[2] == 0xbf)) {
				read_status -= 3; /* UTF-8-BOM */
				tpp_memmovedown(io_dst, io_dst + 3, read_status);
				self->tf_enc = TPP_FILE_ENCODING_FORCE_UTF8;
				if tpp_unlikely(read_status == 0)
					goto again;
			} else if (read_status >= 4 && (io_dst[0] == 0x00 && io_dst[1] == 0x00 &&
			                                io_dst[2] == 0xfe && io_dst[3] == 0xff)) {
				read_status -= 4; /* UTF-32-BE-BOM */
				tpp_memmovedown(io_dst, io_dst + 4, read_status);
				self->tf_enc = TPP_FILE_ENCODING_UTF32_BE;
				goto convert_multiword_to_utf8;
			} else if (read_status >= 4 && (io_dst[0] == 0xff && io_dst[1] == 0xfe &&
			                                io_dst[2] == 0x00 && io_dst[3] == 0x00)) {
				read_status -= 4; /* UTF-32-LE-BOM */
				tpp_memmovedown(io_dst, io_dst + 4, read_status);
				self->tf_enc = TPP_FILE_ENCODING_UTF32_LE;
				goto convert_multiword_to_utf8;
			} else if (read_status >= 2 && (io_dst[0] == 0xfe && io_dst[1] == 0xff)) {
				read_status -= 2; /* UTF-16-BE-BOM */
				tpp_memmovedown(io_dst, io_dst + 2, read_status);
				self->tf_enc = TPP_FILE_ENCODING_UTF16_BE;
				goto convert_multiword_to_utf8;
			} else if (read_status >= 2 && (io_dst[0] == 0xff && io_dst[1] == 0xfe)) {
				read_status -= 2; /* UTF-16-LE-BOM */
				tpp_memmovedown(io_dst, io_dst + 2, read_status);
				self->tf_enc = TPP_FILE_ENCODING_UTF16_LE;
				goto convert_multiword_to_utf8;
			} else {
				/* Guess multi-byte encodings based on present 0-bytes in first 2 characters */
				if (read_status >= 8 && (io_dst[0] && !io_dst[1] && !io_dst[2] && !io_dst[3] &&
				                         io_dst[4] && !io_dst[5] && !io_dst[6] && !io_dst[7])) {
					self->tf_enc = TPP_FILE_ENCODING_UTF32_LE;
					goto convert_multiword_to_utf8;
				}
				if (read_status >= 8 && (!io_dst[0] && !io_dst[1] && !io_dst[2] && io_dst[3] &&
				                         !io_dst[4] && !io_dst[5] && !io_dst[6] && io_dst[7])) {
					self->tf_enc = TPP_FILE_ENCODING_UTF32_BE;
					goto convert_multiword_to_utf8;
				}
				if (read_status >= 4 && (io_dst[0] && !io_dst[1] &&
				                         io_dst[2] && !io_dst[3])) {
					self->tf_enc = TPP_FILE_ENCODING_UTF16_LE;
					goto convert_multiword_to_utf8;
				}
				if (read_status >= 4 && (!io_dst[0] && io_dst[1] &&
				                         !io_dst[2] && io_dst[3])) {
					self->tf_enc = TPP_FILE_ENCODING_UTF16_BE;
					goto convert_multiword_to_utf8;
				}
			}
			/* Continue to assume that input is encoded as TPP_FILE_ENCODING_UTF8 */
		} else {
			/* No conversion needed... */
		}
		break;
	case TPP_FILE_ENCODING_ASCII:
	case TPP_FILE_ENCODING_FORCE_UTF8:
		/* No conversion needed for these 2... */
		break;
	case TPP_FILE_ENCODING_UTF16_LE:
	case TPP_FILE_ENCODING_UTF16_BE:
	case TPP_FILE_ENCODING_UTF32_LE:
	case TPP_FILE_ENCODING_UTF32_BE: {
		tpp_char *dst_base, *dst_end;
		tpp_size tail_mask, tail_size, out_size;
convert_multiword_to_utf8:
		if tpp_unlikely(read_status == 0)
			return TPP_EOK; /* EOF */
		if (self->tf_data.td_io.tff_tailc) {
			io_dst -= self->tf_data.td_io.tff_tailc;
			read_status += self->tf_data.td_io.tff_tailc;
			self->tf_data.td_io.tff_tailc = 0;
		}

		/* Store unaligned tail data in out-of-band "tail" buffer */
		tail_mask = (self->tf_enc == TPP_FILE_ENCODING_UTF16_LE ||
		             self->tf_enc == TPP_FILE_ENCODING_UTF16_BE)
		            ? 1
		            : 3;
		tail_size = (tpp_size)read_status & tail_mask;
		if (tail_size) {
			tpp_char *tail_base;
			read_status -= tail_size;
			tail_base = io_dst + (tpp_size)read_status;
			self->tf_data.td_io.tff_tailc = (uint_least8_t)tail_size;
			tpp_memcpy(self->tf_data.td_io.tff_tailv, tail_base, tail_size);
		}

		dst_end = tpp_string_end(new_chunk);
		switch (self->tf_enc) {
		case TPP_FILE_ENCODING_UTF16_LE:
		case TPP_FILE_ENCODING_UTF16_BE: {
			uint_least16_t const *dst16 = (uint_least16_t const *)io_dst;
			tpp_size words = (tpp_size)read_status / 2;
			union tpp_word16 raw_last_word;
			uint_least16_t last_word;
			if tpp_unlikely(!words) {
				dst_base = dst_end;
				break;
			}
			raw_last_word.w16 = dst16[words - 1];
			if (self->tf_enc == TPP_FILE_ENCODING_UTF16_LE) {
				last_word = tpp_word16_getle(raw_last_word);
			} else {
				last_word = tpp_word16_getbe(raw_last_word);
			}
			if (last_word >= TPP_UTF16_HIGH_SURROGATE_MIN &&
			    last_word <= TPP_UTF16_HIGH_SURROGATE_MAX) {
				/* Last word is a HIGH_UTF16 surrogate -> exclude from conversion
				 * and add to tail (this character can only the next word has been
				 * fully read, also, which should be the LOW_UTF16 surrogate) */
				tpp_memmoveup(self->tf_data.td_io.tff_tailv + 2,
				              self->tf_data.td_io.tff_tailv, tail_size);
				self->tf_data.td_io.tff_tailv[0] = raw_last_word.w8[0];
				self->tf_data.td_io.tff_tailv[1] = raw_last_word.w8[1];
				self->tf_data.td_io.tff_tailc += 2;
				--words;
			}
			if (self->tf_enc == TPP_FILE_ENCODING_UTF16_LE) {
				dst_base = tpp_utf16le_to_utf8(dst16, words, dst_end);
			} else {
				dst_base = tpp_utf16be_to_utf8(dst16, words, dst_end);
			}
		}	break;

		case TPP_FILE_ENCODING_UTF32_LE:
			dst_base = tpp_utf32le_to_utf8((uint_least32_t const *)io_dst,
			                               (tpp_size)read_status / 4,
			                               dst_end);
			break;
		case TPP_FILE_ENCODING_UTF32_BE:
			dst_base = tpp_utf32be_to_utf8((uint_least32_t const *)io_dst,
			                               (tpp_size)read_status / 4,
			                               dst_end);
			break;
		default: tpp_unreachable();
		}

		/* Shift decoded unicode data to its final position. */
		out_size = (tpp_size)(dst_end - dst_base);
		io_dst = (tpp_char *)self->tf_end;
		tpp_memmovedown(io_dst, dst_base, out_size);
		if tpp_unlikely(out_size == 0) {
			/* This can happen (e.g.) when a single byte was read from a pipe,
			 * but the multi-byte codec requires at least 2/4 bytes in order
			 * to form the next byte(s) of UTF-8 input data.
			 *
			 * So-as not to confuse the caller into thinking that EOF has been
			 * reached, simply loop around and read some more... */
			goto again;
		}

		self->tf_end += out_size;
		return TPP_EOK;
	}	break;

	default: tpp_unreachable();
	}
#endif /* TPP_HAVE_UNICODE */

	/* Remember that more buffer space is now available! */
	self->tf_end += (tpp_size)read_status;
	return TPP_EOK;
}



/* These are needed for the shared
 * >> case TPP_FILE_KIND_IO:
 * >> case TPP_FILE_KIND_TEXT: {
 * in "tpp_file_lcinfo()" */
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_name) ==
                  tpp_offsetof(tpp_file, tf_data.td_text.tft_name));
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_start_lc) ==
                  tpp_offsetof(tpp_file, tf_data.td_text.tft_start_lc));


#if TPP_HAVE_CPP_MACROS
/* Figure out the line/column of "pos" in "expanded_text", as produced
 * by "self", which must be "TPP_MACRO_KIND_ISFUNC(self->tm_kind)". */
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_lcinfo TPPCALL
tpp_macro_func_lcinfo(tpp_macro const *tpp_restrict self,
                      tpp_string const *expanded_text,
                      tpp_char const *pos);
#endif /* TPP_HAVE_CPP_MACROS */


/* Return line/column information (1-based) for "pos" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_lcinfo TPPCALL
tpp_file_lcinfo(tpp_file *tpp_restrict self, tpp_char const *pos) {
	tpp_lcinfo result;
	if tpp_unlikely(!self->tf_chunk)
		return tpp_lcinfo_of(0, 0);
	tpp_assert(pos >= tpp_string_str(self->tf_chunk));
	tpp_assert(pos <= tpp_string_end(self->tf_chunk));

	/* Check against the cache */
#if TPP_HAVE_FILE_LC_CACHE
	if (self->tf_lcpos) {
		tpp_size delta_from_chunk;
		tpp_size delta_from_lcpos;
		if (pos >= self->tf_lcpos) {
			result = self->tf_lcval;
			result = tpp_lcinfo_account(self, result, self->tf_lcpos,
			                            (tpp_size)(pos - self->tf_lcpos));
			goto done;
		}
		delta_from_chunk = (tpp_size)(pos - tpp_string_str(self->tf_chunk));
		delta_from_lcpos = (tpp_size)(self->tf_lcpos - pos);
		if (delta_from_chunk > (delta_from_lcpos * 2)) {
			/* Given "pos" is much closer to "tf_lcpos", so try to work off of the cache! */
			tpp_char const *last_linefeed;
			last_linefeed = tpp_lcinfo_find_last_linefeed(self, tpp_string_str(self->tf_chunk), pos);
			if (last_linefeed) {
				tpp_line num_lf = tpp_lcinfo_count_linefeed(self, last_linefeed, self->tf_lcpos);
				tpp_line last_linefeed_lno = tpp_lcinfo_getline(self->tf_lcval) - num_lf;
				tpp_lcinfo_init(result, last_linefeed_lno, 0);
				result = tpp_lcinfo_account(self, result, last_linefeed,
				                            (tpp_size)(pos - last_linefeed));
				goto done;
			}
		}
	}
#endif /* TPP_HAVE_FILE_LC_CACHE */

	/* Cannot use cache -> must calculate off of known file-base */
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO:
	case TPP_FILE_KIND_TEXT: {
		result = self->tf_data.td_io.tff_start_lc;
		result = tpp_lcinfo_account(self, result, tpp_string_str(self->tf_chunk),
		                            (tpp_size)(pos - tpp_string_str(self->tf_chunk)));
	}	break;

#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO: {
		tpp_macro const *macro = self->tf_data.td_macro.tfm_macro;
		if (self->tf_chunk != macro->tm_body_chunk) {
			/* Complicated case (where caching is also impossible):
			 * This file is the result of a function-style macro expansion,
			 * meaning that origin line/column information is related-to,
			 * but not linear to actual text data:
			 *
			 * Line   012345678911111111112
			 *                  01234567890
			 * [1] >> #define add(a, b) a+b
			 * [2] >> add(foo, bar);
			 *
			 * OUTPUT:
			 * >> foo   // line=1, col=18
			 * >> +     // line=1, col=19
			 * >> bar   // line=1, col=20 */
			tpp_assert(TPP_MACRO_KIND_ISFUNC(macro->tm_kind));
			result = tpp_macro_func_lcinfo(macro, self->tf_chunk, pos);
			goto done_nocache;
		} else {
			result = macro->tm_body_lc;
			result = tpp_lcinfo_account(self, result, macro->tm_body_start,
			                            (tpp_size)(pos - macro->tm_body_start));
		}
	}	break;
#endif /* TPP_HAVE_CPP_MACROS */
	default: tpp_unreachable();
	}


done:
#if TPP_HAVE_FILE_LC_CACHE
	self->tf_lcpos = pos;
	self->tf_lcval = result;
#endif /* TPP_HAVE_FILE_LC_CACHE */
#if TPP_HAVE_CPP_MACROS
done_nocache:
#endif /* TPP_HAVE_CPP_MACROS */
	return result;
}


/* These are needed for the shared
 * >> case TPP_FILE_KIND_IO:
 * >> case TPP_FILE_KIND_TEXT:
 * in "tpp_file_filename()" */
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_name) ==
                  tpp_offsetof(tpp_file, tf_data.td_text.tft_name));

/* Returns the filename of "self", or "NULL" if unknown. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_filename(tpp_file const *tpp_restrict self) {
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO:
	case TPP_FILE_KIND_TEXT:
		return self->tf_data.td_io.tff_name;

#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO:
		return self->tf_data.td_macro.tfm_macro->tm_deffile;
#endif /* TPP_HAVE_CPP_MACROS */

	default: tpp_unreachable();
	}
}

/* Same as `tpp_file_filename()', but may be overwritten by "#line" directives */
#if TPP_HAVE_FILE_USER_FILENAME
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_userfilename(tpp_file const *tpp_restrict self) {
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO:
	case TPP_FILE_KIND_TEXT: {
		TPP_REF tpp_string *user;
		user = self->tf_data.td_io.tff_user_filename;
		if (user != NULL)
			return (char const *)user->ts_str;
		return self->tf_data.td_io.tff_name;
	}	break;

#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO:
		return self->tf_data.td_macro.tfm_macro->tm_deffile;
#endif /* TPP_HAVE_CPP_MACROS */

	default: tpp_unreachable();
	}
}

/* Sets the user-filename override of "self" to "filename"
 *
 * NOTE: The caller must ensure that:
 *       >> self->tf_kind == TPP_FILE_KIND_IO ||
 *       >> self->tf_kind == TPP_FILE_KIND_TEXT;
 *
 * You may also pass "NULL" for `filename' to disable the override */
TPP_IMPL TPP_NONNULL((1, 2)) void TPPCALL
tpp_file_setuserfilename(tpp_file *tpp_restrict self,
                         tpp_string *tpp_restrict filename) {
	tpp_string *old_override;
	tpp_assert(self->tf_kind == TPP_FILE_KIND_IO ||
	           self->tf_kind == TPP_FILE_KIND_TEXT);
	old_override = self->tf_data.td_text.tft_user_filename;
	self->tf_data.td_text.tft_user_filename = filename;
	if (filename)
		tpp_string_incref(filename);
	if (old_override)
		tpp_string_decref(old_override);
}
#endif /* TPP_HAVE_FILE_USER_FILENAME */


/* Set the (0-based) line that applies to "pos" (as returned by "tpp_file_lcinfo") in "self"
 *
 * NOTE: The caller must ensure that:
 *       >> self->tf_kind == TPP_FILE_KIND_IO ||
 *       >> self->tf_kind == TPP_FILE_KIND_TEXT; */
#if TPP_HAVE_FILE_SETLINE
TPP_IMPL TPP_NONNULL((1, 2)) void TPPCALL
tpp_file_setline(tpp_file *tpp_restrict self,
                 tpp_char const *pos, tpp_line line) {
	tpp_lcinfo cur_info;
	tpp_line cur_line, delta, start_line;
	tpp_column start_col;
	tpp_assert(self->tf_kind == TPP_FILE_KIND_IO ||
	           self->tf_kind == TPP_FILE_KIND_TEXT);
	cur_info   = tpp_file_lcinfo(self, pos);
	cur_line   = tpp_lcinfo_getline(cur_info);
	delta      = line - cur_line;
	start_line = tpp_lcinfo_getline(self->tf_data.td_text.tft_start_lc) + delta;
	start_col  = tpp_lcinfo_getcol(self->tf_data.td_text.tft_start_lc);
	tpp_lcinfo_init(self->tf_data.td_text.tft_start_lc, start_line, start_col);
}
#endif /* TPP_HAVE_FILE_SETLINE */



/* Returns the filename "keyword" (which may not always be
 * available, even when "tpp_file_filename()" returns non-NULL) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) struct tpp_keyword *TPPCALL
tpp_file_filename_kwd(tpp_file const *tpp_restrict self) {
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO: {
		char const *filename;
#if TPP_HAVE_FILE_NOKWD
		if (self->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOKWD)
			return NULL; /* Name isn't actually a keyword... */
#endif /* TPP_HAVE_FILE_NOKWD */
		filename = self->tf_data.td_io.tff_name;
		if (filename == NULL)
			return NULL;
		return (struct tpp_keyword *)(filename - offsetof(struct tpp_keyword, tk_kwd));
	}	break;

	case TPP_FILE_KIND_TEXT:
#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO:
#endif /* TPP_HAVE_CPP_MACROS */
		return NULL;

	default: tpp_unreachable();
	}
}


/* Returns the first tf_kind=TPP_FILE_KIND_IO file in the #include-stack (using "tf_tprev")
 * If no such file exists, simply re-return "self". This function never returns "NULL" */
#if TPP_HAVE_INCLUDE_STACK
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getiofile(tpp_file const *tpp_restrict self) {
	tpp_file *iter = (tpp_file *)self;
	while (iter->tf_kind != TPP_FILE_KIND_IO) {
		iter = iter->tf_tprev;
		if (iter == NULL)
			return (tpp_file *)self;
	}
	return iter;
}

/* Returns the last file in the #include-stack (using "tf_tprev") */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getbasefile(tpp_file const *tpp_restrict self) {
	while (self->tf_tprev)
		self = self->tf_tprev;
	return (tpp_file *)self;
}

/* Returns the first tf_kind!=TPP_FILE_KIND_MACRO file in the #include-stack (using "tf_tprev")
 * If no such file exists, returns "NULL" */
#if TPP_HAVE_CPP_MACROS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_gettextfile(tpp_file const *tpp_restrict self) {
	tpp_file *iter = (tpp_file *)self;
	while (iter->tf_kind == TPP_FILE_KIND_MACRO) {
		iter = iter->tf_tprev;
		if (iter == NULL)
			break;
	}
	return iter;
}

/* Same as `tpp_file_gettextfile()', but re-return "self" instead of returning "NULL"
 * The term "lc" here refers to the fact that this is the file that's used as basis
 * for the builtin __FILE__, __LINE__ and __COLUMN__ macros. */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getlcfile(tpp_file const *tpp_restrict self) {
	tpp_file *result = tpp_file_gettextfile(self);
	return result ? result : (tpp_file *)self;
}
#endif /* TPP_HAVE_CPP_MACROS */
#endif /* TPP_HAVE_INCLUDE_STACK */


/* Size of \t as reported by `tpp_file_lcinfo()' */
#if TPP_TABSIZE < 0
TPP_IMPL tpp_column tpp_tabsize = -TPP_TABSIZE;
#endif /* TPP_TABSIZE < 0 */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/keyword.c                                                */
/************************************************************************/
TPP_DECL_BEGIN

/* Assert that "tpp_keyword" and "tpp_string" are binary-compatible. */
TPP_STATIC_ASSERT((tpp_offsetof(tpp_keyword, tk_refcnt) -
                   tpp_offsetof(tpp_keyword, _TPP_KEYWORD_STRING_ABI_START)) ==
                  (tpp_offsetof(tpp_string, ts_refcnt)));
TPP_STATIC_ASSERT((tpp_offsetof(tpp_keyword, tk_len) -
                   tpp_offsetof(tpp_keyword, _TPP_KEYWORD_STRING_ABI_START)) ==
                  (tpp_offsetof(tpp_string, ts_len)));
TPP_STATIC_ASSERT((tpp_offsetof(tpp_keyword, tk_kwd) -
                   tpp_offsetof(tpp_keyword, _TPP_KEYWORD_STRING_ABI_START)) ==
                  (tpp_offsetof(tpp_string, ts_str)));


#if TPP_HAVE_PRAGMA_PUSH_MACRO
/* Initialize/finalize a given macro-push stack */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_macro_pushstack_fini(tpp_macro_pushstack *tpp_restrict self) {
	tpp_size i;
	for (i = 0; i < self->tmps_cnt; ++i) {
		TPP_REF tpp_macro *mac;
		mac = self->tmps_vec[i].tmpe_macro;
		if (mac)
			tpp_macro_decref(mac);
	}
	tpp_free(self->tmps_vec);
}

/* Allocate space for- and return a new (uninitialized) macro-push entry
 * @return: * :   The newly allocated macro-push entry.
 * @return: NULL: Out-of-memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_pushent *TPPCALL
tpp_macro_pushstack_append(tpp_macro_pushstack *tpp_restrict self) {
	tpp_macro_pushent *new_vec;
	tpp_size new_cnt = self->tmps_cnt + 1;
	new_vec = (tpp_macro_pushent *)tpp_realloc(self->tmps_vec, new_cnt * sizeof(tpp_macro_pushent));
	if tpp_likely(new_vec) {
		self->tmps_vec = new_vec;
		self->tmps_cnt = new_cnt;
		new_vec += new_cnt - 1; /* Return pointer to last (newly allocated / uninitialized) element. */
	}
	return new_vec;
}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_KEYWORD_MISC
/* Ensure that `self->tk_misc' has been allocated and return it.
 * If it isn't already allocated, allocate+return it lazily.
 * WARNING: Only call this function on a "writable" keyword (s.a. `tpp_keywords_copybuiltin()')
 *
 * @return: * :   The "misc" data of "self" (freshly allocated)
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_misc *TPPCALL
tpp_keyword_requiremisc(tpp_keyword *tpp_restrict self) {
	tpp_keyword_misc *result = self->tk_misc;
	if tpp_unlikely(result == NULL) {
		result = (tpp_keyword_misc *)tpp_malloc(sizeof(tpp_keyword_misc));
		if tpp_likely(result) {
#if TPP_HAVE_KEYWORD_FLAGS
			result->tkm_flags = TPP_KEYWORD_FLAG_NORMAL;
#endif /* TPP_HAVE_KEYWORD_FLAGS */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
			result->tkm_file_guard = NULL;
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_PRAGMA_PUSH_MACRO
			tpp_macro_pushstack_init(&result->tkm_macro_pushstack);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_MACRO___TPP_COUNTER
			result->tkm_builtin_counter = 0;
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
#if TPP_HAVE_KEYWORD_USERDATA
			result->tkm_userdata_ptr  = NULL;
			result->tkm_userdata_dtor = NULL;
#endif /* TPP_HAVE_KEYWORD_USERDATA */
			self->tk_misc = result;
		}
	}
	return result;
}
#endif /* TPP_HAVE_KEYWORD_MISC */


#if TPP_HAVE_KEYWORD_USERDATA
/* Get the user-data pointer for "self"
 * @return: NULL: No pointer set, or set pointer is "NULL" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) void *TPPCALL
tpp_keyword_getuserdata(tpp_keyword const *tpp_restrict self) {
	tpp_keyword_misc const *misc = self->tk_misc;
	return misc ? tpp_keyword_misc_getuserdata(misc) : NULL;
}

/* Set the user-data pointer for "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_setuserdata(tpp_keyword *tpp_restrict self,
                        void *ptr, void (TPPCALL *dtor)(void *ptr)) {
	tpp_keyword_misc *misc;
	if (!ptr && !dtor && !self->tk_misc)
		return TPP_EOK;
	misc = tpp_keyword_requiremisc(self);
	if tpp_unlikely(!misc)
		return TPP_ENOMEM;
	tpp_keyword_misc_setuserdata(misc, ptr, dtor);
	return TPP_EOK;
}
#endif /* TPP_HAVE_KEYWORD_USERDATA */



#if TPP_HAVE_PRAGMA_PUSH_MACRO
/* Push the current macro-definition of "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_pushmacro(tpp_keyword *tpp_restrict self) {
	tpp_keyword_misc *const misc = tpp_keyword_requiremisc(self);
	tpp_macro_pushent *ent;
	if tpp_unlikely(!misc)
		goto err_nomem;

	/* Check if the last-pushed entry still correctly describes the state. */
	if (misc->tkm_macro_pushstack.tmps_cnt) {
		ent = &misc->tkm_macro_pushstack.tmps_vec[misc->tkm_macro_pushstack.tmps_cnt - 1];
		tpp_assert(ent->tmpe_count != 0);
		if (ent->tmpe_macro == self->tk_macro) {
			++ent->tmpe_count;
			return TPP_EOK;
		}
	}

	/* Must allocate a new push-entry. */
	ent = tpp_macro_pushstack_append(&misc->tkm_macro_pushstack);
	if tpp_unlikely(!ent)
		goto err_nomem;

	/* Initialize the new push-entry */
	ent->tmpe_count = 1;              /* First time! */
	ent->tmpe_macro = self->tk_macro; /* Current definition */
	if (ent->tmpe_macro)
		tpp_macro_incref(ent->tmpe_macro);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}


/* Pop the current macro-definition of "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: Macro-push-stack was already empty (soft-error) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_popmacro(tpp_keyword *tpp_restrict self) {
	tpp_keyword_misc *misc = self->tk_misc;
	tpp_macro_pushent *last;
	if (misc == NULL)
		goto err_empty;
	if (misc->tkm_macro_pushstack.tmps_cnt == 0)
		goto err_empty;
	last = &misc->tkm_macro_pushstack.tmps_vec[misc->tkm_macro_pushstack.tmps_cnt - 1];
	tpp_assert(last->tmpe_count != 0);

	/* Restore macro definition */
	if (last->tmpe_macro)
		tpp_macro_incref(last->tmpe_macro);
	if (self->tk_macro)
		tpp_macro_decref(self->tk_macro);
	self->tk_macro = last->tmpe_macro;

	/* Update stack-element counter. */
	--last->tmpe_count;
	if (last->tmpe_count == 0) {
		/* Remove stack element. */
		if (last->tmpe_macro)
			tpp_refcnt_dec(&last->tmpe_macro->tm_refcnt);
		--misc->tkm_macro_pushstack.tmps_cnt;
#ifndef __OPTIMIZE_SIZE__
		if (misc->tkm_macro_pushstack.tmps_cnt == 0) {
			/* Free push-stack */
			tpp_free(misc->tkm_macro_pushstack.tmps_vec);
			misc->tkm_macro_pushstack.tmps_vec = NULL;
		} else {
			/* Try to truncate push-stack */
			tpp_macro_pushent *new_vec;
			new_vec = (tpp_macro_pushent *)tpp_tryrealloc(misc->tkm_macro_pushstack.tmps_vec,
			                                              misc->tkm_macro_pushstack.tmps_cnt *
			                                              sizeof(tpp_macro_pushent));
			if tpp_likely(new_vec)
				misc->tkm_macro_pushstack.tmps_vec = new_vec;
		}
#endif /* !__OPTIMIZE_SIZE__ */
	}
	return TPP_EOK;
err_empty:
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_CPP_MACROS
/* Delete the macro definition of `self'.
 * The caller must ensure that `tpp_keyword_canundef(self)' */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keyword_undef(tpp_keyword *tpp_restrict self) {
	TPP_REF tpp_macro *old_macro;
	tpp_assert(tpp_keyword_canundef(self));
	old_macro = self->tk_macro;
	self->tk_macro = NULL;
	tpp_macro_decref(old_macro);
}
#endif /* TPP_HAVE_CPP_MACROS */



/* Calculate the hash of a given keyword string */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof(tpp_char const *tpp_restrict kwd, tpp_size len) {
	tpp_hash result = 1;
	tpp_size i;
	for (i = 0; i < len; ++i) {
		tpp_char ch = kwd[i];
		result = result * 263 + ch;
	}
	return result;
}



/* Helper macros to skip over BSE when parsing already-loaded text.
 * tpp_skipbse_fwd: If "pos" points at a \-character, skip forward until end of BSE (if it is one)
 * tpp_skipbse_bck: If "pos" points after a line-feed character, skip backward until start of BSE (if it is one) */
#if TPP_HAVE_BSE
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_skipbse_fwd_(tpp_char const *pos, tpp_char const *end tpp_bse_file__PARAM) {
	tpp_char const *iter = pos;
again:
	tpp_assert(iter < end);
	tpp_assert(_tpp_maybe_isbackslash(*iter));
#if TPP_HAVE_TRIGRAPHS
	if (*iter == '?') {
		if ((iter + 2) > end)
			goto not_bse;
		if (iter[1] != '?')
			goto not_bse;
		if (iter[2] != '/')
			goto not_bse;
		iter += 3;
	} else {
		iter += 1;
	}
#else /* TPP_HAVE_TRIGRAPHS */
	++iter;
#endif /* !TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(file)) {
		tpp_unichar uc;
#if TPP_HAVE_BSE_WHITESPACE
		for (;;) {
			uc = tpp_unicode_readutf8(&iter, end);
			if (!uc && iter >= end)
				goto not_bse;
			if (!tpp_unicode_isspace_nolf(uc)) {
				if (!tpp_unicode_islf(uc))
					goto not_bse;
				break;
			}
		}
#else /* TPP_HAVE_BSE_WHITESPACE */
		uc = tpp_unicode_readutf8(&iter, end);
		if (!tpp_unicode_islf(*iter))
			goto not_bse;
#endif /* !TPP_HAVE_BSE_WHITESPACE */
		if (uc == '\r' && iter < end && *iter == '\n')
			++iter;
	} else
#endif /* TPP_HAVE_UNICODE */
	{
#if TPP_HAVE_BSE_WHITESPACE
		for (;;) {
			if (iter >= end)
				goto not_bse;
			if (!tpp_ascii_isspace_nolf(*iter))
				break;
			++iter;
		}
#endif /* TPP_HAVE_BSE_WHITESPACE */
		if (!tpp_ascii_islf(*iter))
			goto not_bse;
		if (*iter == '\r') {
			++iter;
			if (iter < end && *iter == '\n')
				++iter;
		} else {
			++iter;
		}
	}
	/* Check if there may be multiple, consecutive BSE sequences... */
	if (iter < end && _tpp_maybe_isbackslash(*iter)) {
		pos = iter;
		goto again;
	}
	return iter;
not_bse:
	return pos;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_skipbse_bck_(tpp_char const *pos, tpp_char const *start tpp_bse_file__PARAM) {
	tpp_char const *iter = pos;
again:
	tpp_assert(iter > start);
#if TPP_HAVE_UNICODE
	tpp_assert(tpp_ascii_islfornascii(iter[-1]));
	if (tpp_file_isutf8(file)) {
		tpp_unichar uc;
		uc = tpp_unicode_readutf8_rev(&iter, start);
		if (!tpp_unicode_islf(uc))
			goto not_bse;
		if (uc == '\n' && iter > start && iter[-1] == '\r')
			--iter;
#if TPP_HAVE_BSE_WHITESPACE
		for (;;) {
			uc = tpp_unicode_readutf8_rev(&iter, start);
			if (!uc && iter <= start)
				goto not_bse;
			if (!tpp_unicode_isspace_nolf(uc)) {
				if (uc == '\\')
					return iter;
#if TPP_HAVE_TRIGRAPHS
				if (uc == '/') {
					uc = tpp_unicode_readutf8_rev(&iter, start);
					if (uc != '?')
						goto not_bse;
					uc = tpp_unicode_readutf8_rev(&iter, start);
					if (uc != '?')
						goto not_bse;
					return iter;
				}
#endif /* TPP_HAVE_TRIGRAPHS */
				goto not_bse;
			}
		}
#else /* TPP_HAVE_BSE_WHITESPACE */
		uc = tpp_unicode_readutf8_rev(&iter, start);
		if (uc == '\\')
			return iter;
#if TPP_HAVE_TRIGRAPHS
		if (uc == '/') {
			uc = tpp_unicode_readutf8_rev(&iter, start);
			if (uc != '?')
				goto not_bse;
			uc = tpp_unicode_readutf8_rev(&iter, start);
			if (uc != '?')
				goto not_bse;
			return iter;
		}
#endif /* TPP_HAVE_TRIGRAPHS */
		goto not_bse;
#endif /* !TPP_HAVE_BSE_WHITESPACE */
	}
	if (!tpp_ascii_islf(*iter))
		goto not_bse;
#else /* TPP_HAVE_UNICODE */
	tpp_assert(tpp_ascii_islf(iter[-1]));
#endif /* !TPP_HAVE_UNICODE */
	--iter;
	if (iter <= start)
		goto not_bse;
	if (iter[-1] == '\r' && *iter == '\n') {
		--iter;
		if (iter <= start)
			goto not_bse;
	}
#if TPP_HAVE_BSE_WHITESPACE
	for (;;) {
		if (iter <= start)
			goto not_bse;
		--iter;
		if (!tpp_ascii_isspace_nolf(*iter))
			break;
	}
#endif /* TPP_HAVE_BSE_WHITESPACE */
	if (*iter == '\\') {
		/* Yes! */
	} else
#if TPP_HAVE_TRIGRAPHS
	if (*iter == '/') {
		if ((iter - 2) < start)
			goto not_bse;
		if (iter[-1] != '?')
			goto not_bse;
		if (iter[-2] != '?')
			goto not_bse;
		iter -= 2;
	} else
#endif /* TPP_HAVE_TRIGRAPHS */
	{
		goto not_bse;
	}

	/* Check if there may be multiple, consecutive BSE sequences... */
	if (iter > start && _tpp_maybe_islf(iter[-1])) {
		pos = iter;
		goto again;
	}
	return iter;
not_bse:
	return pos;
}
#endif /* TPP_HAVE_BSE */


#if TPP_HAVE_ESCAPED_KEYWORDS
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS

/* Check if "*p_iter" is a BSI sequence.
 * - If so, decode it, update `*p_iter' to point to its end,
 *   and return the # of utf-8 bytes written to "buf"
 * - Otherwise, return "0"
 *
 * NOTE: Given *p_iter points **AFTER** the leading \ character
 *
 * Caller must ensure that _tpp_maybe_isbackslash((*p_iter)[-1]) */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_decode_bsi(tpp_char buf[TPP_UTF8_MAXLEN], tpp_char const **p_iter, tpp_char const *end) {
	tpp_char ch;
	tpp_char const *iter = *p_iter;
	tpp_unichar uc;
	unsigned int cur_digit;
	unsigned int max_digit;
	tpp_assert(_tpp_maybe_isbackslash(iter[-1]));
	if (iter >= end)
		goto nope;
	ch = *iter++;
#if TPP_HAVE_TRIGRAPHS
	if (ch == '?') {
		if (iter[-2] != '?')
			goto nope;
		if (iter >= end)
			goto nope;
		if (*iter != '/')
			goto nope;
		++iter;
		ch = *iter++;
	}
#endif /* TPP_HAVE_TRIGRAPHS */

	if (ch == 'u') {
		max_digit = 4;
	} else if (ch == 'U') {
		max_digit = 8;
	} else {
		goto nope;
	}
	if (iter >= end)
		goto nope;
	ch = *iter++;
	uc = 0;
	cur_digit = 0;
	for (;;) {
		uint_least8_t nibble;
		if (ch >= '0' && ch <= '9') {
			nibble = (uint_least8_t)(ch - '0');
		} else if (ch >= 'a' && ch <= 'f') {
			nibble = 10 + (uint_least8_t)(ch - 'a');
		} else if (ch >= 'A' && ch <= 'F') {
			nibble = 10 + (uint_least8_t)(ch - 'A');
		} else {
			if (cur_digit == 0)
				goto nope;
			--iter; /* Don't consume this one! */
			break;
		}
		uc <<= 4;
		uc |= nibble;
		++cur_digit;
		if (cur_digit >= max_digit)
			break;
		if (iter >= end)
			break;
		ch = *iter++;
	}

	/* Indicate to caller that the BSI sequence was consumed. */
	*p_iter = iter;

	/* (re-)encode as utf-8 */
	return (tpp_size)(tpp_unicode_writeutf8(buf, uc) - buf);
nope:
	return 0;
}
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */


/* Same as `tpp_hashof()', but skip over \-escaped linefeeds when calculating the hash */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof_esc_(tpp_char const *tpp_restrict kwd, tpp_size len tpp_bse_file__PARAM) {
	tpp_hash result = 1;
	tpp_char const *end = kwd + len;
	while (kwd < end) {
		tpp_char ch = *kwd++;
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
		tpp_char bsi[TPP_UTF8_MAXLEN];
		tpp_size bsi_len;
		if ((_tpp_maybe_isbackslash(ch)) &&
		    (bsi_len = tpp_decode_bsi(bsi, (tpp_char const **)&kwd, end)) != 0) {
			tpp_size i = 0;
			do {
				result = result * 263 + bsi[i];
			} while (++i < bsi_len);
		} else
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
		{
			result = result * 263 + ch;
		}
		kwd = tpp_skipbse_fwd(kwd, end, file);
	}
	return result;
}

/* Copy `in_text...+=len' to `out_text', whilst removing \-escaped linefeeds
 * The caller must ensure that `out_text' has space for at least `len' bytes,
 * and the actual # of used bytes of `out_text' is returned. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_without_esc_(tpp_char *tpp_restrict out_text,
                 tpp_char const *tpp_restrict in_text,
                 tpp_size len tpp_bse_file__PARAM) {
	tpp_size result = 0;
	tpp_char const *end = in_text + len;
	while (in_text < end) {
		tpp_char ch = *in_text++;
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
		tpp_size bsi_len;
		if ((_tpp_maybe_isbackslash(ch)) &&
		    (bsi_len = tpp_decode_bsi(out_text + result, (tpp_char const **)&in_text, end)) != 0) {
			result += bsi_len;
		} else
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
		{
			out_text[result++] = ch;
		}
		in_text = tpp_skipbse_fwd(in_text, end, file);
	}
	return result;
}

/* Compare 2 strings, one of which may contain \-escaped linefeeds that must be skipped. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) int TPPCALL
tpp_memcmp_esc_(tpp_char const *lhs_without_esc, tpp_size lhs_len,
                tpp_char const *rhs_with_esc, tpp_size rhs_len
                tpp_bse_file__PARAM) {
	tpp_char const *lhs_end = lhs_without_esc + lhs_len;
	tpp_char const *rhs_end = rhs_with_esc + rhs_len;
	while (rhs_with_esc < rhs_end) {
		tpp_char lhs_ch, rhs_ch;
		if (lhs_without_esc >= lhs_end)
			return -1;
		lhs_ch = *lhs_without_esc++;
		rhs_ch = *rhs_with_esc++;
		if (lhs_ch != rhs_ch) {
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
			tpp_char bsi[TPP_UTF8_MAXLEN];
			tpp_size bsi_len;
			if ((_tpp_maybe_isbackslash(rhs_ch)) &&
			    (bsi_len = tpp_decode_bsi(bsi, (tpp_char const **)&rhs_with_esc, rhs_end)) != 0) {
				tpp_size i = 0;
				do {
					if (lhs_ch != bsi[i])
						return lhs_ch < bsi[i] ? -1 : 1;
					if (lhs_without_esc >= lhs_end)
						return -1;
					lhs_ch = *lhs_without_esc++;
				} while (++i < bsi_len);
			} else
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
			{
				return lhs_ch < rhs_ch ? -1 : 1;
			}
		}
		rhs_with_esc = tpp_skipbse_fwd(rhs_with_esc, rhs_end, file);
	}
	return 0;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


#if TPP_HAVE_KEYWORD_MISC
static TPP_NONNULL((1)) void TPPCALL
tpp_keyword_misc_destroy(tpp_keyword_misc *tpp_restrict self) {
#if TPP_HAVE_PRAGMA_PUSH_MACRO
	tpp_macro_pushstack_fini(&self->tkm_macro_pushstack);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_KEYWORD_USERDATA
	if (self->tkm_userdata_dtor)
		(*self->tkm_userdata_dtor)(self->tkm_userdata_ptr);
#endif /* TPP_HAVE_KEYWORD_USERDATA */
	tpp_free(self);
}
#endif /* TPP_HAVE_KEYWORD_MISC */

static TPP_NONNULL((1)) void TPPCALL
tpp_keyword_destroy(tpp_keyword *tpp_restrict self) {
	tpp_assert(!tpp_refcnt_isshared(&self->tk_refcnt) && "Keyword still in use");
#if TPP_HAVE_CPP_MACROS
	if (self->tk_macro) {
		tpp_assert(self->tk_macro->tm_expansions == 0 && "Macro still part of #include-stack?");
		tpp_macro_decref(self->tk_macro);
	}
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	if (self->tk_misc)
		tpp_keyword_misc_destroy(self->tk_misc);
#endif /* TPP_HAVE_KEYWORD_MISC */
	tpp_free(self);
}


TPP_IMPL TPP_REF tpp_keyword *tpp_keywords_empty_map[1] = { NULL };

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_fini(tpp_keywords *tpp_restrict self) {
	TPP_REF tpp_keyword **bckv = self->tks_bckv;
	if (bckv != tpp_keywords_empty_map) {
		tpp_hash i;
		for (i = 0; i <= self->tks_bckm; ++i) {
			TPP_REF tpp_keyword *chain = bckv[i];
			while (chain) {
				TPP_REF tpp_keyword *next = chain->tk_next;
				tpp_keyword_destroy(chain);
				chain = next;
			}
		}
		tpp_free(bckv);
	}
}

/* Lookup keywords within the given keywords-table **ONLY**
 * @return: * :   The keyword in question
 * @return: NULL: No such keyword (consider using "tpp_keywords_getkeyword" to
 *                also check for builtin keywords, or "tpp_keywords_newkeyword"
 *                to do the same, but lazily create missing keywords) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword(tpp_keywords const *tpp_restrict self,
                         tpp_char const *tpp_restrict kwd,
                         tpp_size len, tpp_hash hash) {
	tpp_keyword *result = self->tks_bckv[hash & self->tks_bckm];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (result->tk_len != len)
			continue;
		if (tpp_memcmp(result->tk_kwd, kwd, len) == 0)
			break;
	}
	return result;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword_byid(tpp_keywords const *tpp_restrict self,
                              enum tpp_token_id id) {
	tpp_hash i;
#if !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
	if (!TPP_TOK_ISUSERKEYWORD(id))
		return NULL;
#endif /* !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */

	/* XXX: This is slow... */
	for (i = 0; i <= self->tks_bckm; ++i) {
		tpp_keyword *chain = self->tks_bckv[i];
		for (; chain; chain = chain->tk_next) {
			if (chain->tk_id == id)
				return chain;
		}
	}
	return NULL;
}

#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword_esc_(tpp_keywords const *tpp_restrict self,
                              tpp_char const *tpp_restrict kwd,
                              tpp_size len, tpp_hash hash
                              tpp_bse_file__PARAM) {
	tpp_keyword *result = self->tks_bckv[hash & self->tks_bckm];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (tpp_memcmp_esc(result->tk_kwd, result->tk_len, kwd, len, file) == 0)
			break;
	}
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Same as above, but also search the built-in keyword table (tpp_builtin_getkeyword) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword(tpp_keywords const *tpp_restrict self,
                        tpp_char const *tpp_restrict kwd,
                        tpp_size len, tpp_hash hash) {
	tpp_keyword const *result;
	result = _tpp_keywords_getkeyword(self, kwd, len, hash);
	if (result == NULL)
		result = tpp_builtin_getkeyword(kwd, len, hash);
	return result;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword_byid(tpp_keywords const *tpp_restrict self,
                             enum tpp_token_id id) {
	tpp_keyword const *result;
	result = _tpp_keywords_getkeyword_byid(self, id);
	if (result == NULL)
		result = tpp_builtin_getkeyword_byid(id);
	return result;
}

#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword_esc_(tpp_keywords const *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM) {
	tpp_keyword const *result;
	result = _tpp_keywords_getkeyword_esc(self, kwd, len, hash, file);
	if (result == NULL)
		result = tpp_builtin_getkeyword_esc(kwd, len, hash, file);
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Insert "kwd" into "self". If necessary, resize the hash-map.
 * If resizing fails, "kwd" is destroyed and "NULL" is returned. */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
tpp_keywords_inskeyword(tpp_keywords *tpp_restrict self,
                        /*inherit(always)*/ tpp_keyword *kwd) {
	if (self->tks_kwdc >= self->tks_bckm) {
		/* Try to resize table. */
		TPP_REF tpp_keyword **new_table;
		tpp_hash i, new_mask = (self->tks_bckm << 1) | 1;
		if (new_mask < 15)
			new_mask = 15;
		new_table = (TPP_REF tpp_keyword **)tpp_trymalloc((new_mask + 1) * sizeof(TPP_REF tpp_keyword *));
		if tpp_unlikely(!new_table) {
			new_mask = (self->tks_bckm << 1) | 1;
			new_table = (TPP_REF tpp_keyword **)tpp_trymalloc((new_mask + 1) * sizeof(TPP_REF tpp_keyword *));
			if tpp_unlikely(!new_table) {
				if (self->tks_bckm != 0)
					goto do_insert;
				new_table = (TPP_REF tpp_keyword **)tpp_malloc((new_mask + 1) * sizeof(TPP_REF tpp_keyword *));
				if tpp_unlikely(!new_table)
					goto err_oom;
			}
		}
		tpp_memset(new_table, 0, (new_mask + 1) * sizeof(TPP_REF tpp_keyword *));

		/* Transfer "self->tks_bckv" (old table) into "new_table" */
		for (i = 0; i <= self->tks_bckm; ++i) {
			TPP_REF tpp_keyword *chain = self->tks_bckv[i];
			while (chain) {
				TPP_REF tpp_keyword *next = chain->tk_next;
				TPP_REF tpp_keyword **bucket = &new_table[chain->tk_hash & new_mask];
				chain->tk_next = *bucket;
				*bucket = chain;
				chain = next;
			}
		}

		/* Free old table and assume new one. */
		if (self->tks_bckv != tpp_keywords_empty_map)
			tpp_free(self->tks_bckv);
		self->tks_bckv = new_table;
		self->tks_bckm = new_mask;
	}

do_insert:
	tpp_assert(self->tks_bckm != 0);
	tpp_assert(self->tks_bckv != tpp_keywords_empty_map);
	{
		TPP_REF tpp_keyword **bucket;
		bucket = &self->tks_bckv[kwd->tk_hash & self->tks_bckm];
		kwd->tk_next = *bucket;
		*bucket = kwd;
	}
	++self->tks_kwdc;
	return kwd;
err_oom:
	tpp_keyword_destroy(kwd);
	return NULL;
}

/* Same as above, but if the keyword doesn't exist in `self' or the builtin
 * keyword table, a new keyword is allocated, given an ID, and inserted into `self'
 * @return: * :   The keyword associated with `kwd' (possibly having been just allocated)
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_newkeyword(tpp_keywords *tpp_restrict self,
                        tpp_char const *tpp_restrict kwd,
                        tpp_size len, tpp_hash hash) {
	tpp_keyword *result;
	result = (tpp_keyword *)tpp_keywords_getkeyword(self, kwd, len, hash);
	if (result != NULL)
		goto done;

	/* Must allocate a new keyword... */
	result = (tpp_keyword *)tpp_malloc(tpp_keyword_sizeof(len));
	if (result == NULL)
		goto done;

	result->tk_id = (tpp_token_id)((unsigned int)TPP_TOK_USERKEYWORD_BEGIN + self->tks_kwdc);
#if TPP_HAVE_CPP_MACROS
	result->tk_macro = NULL;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	result->tk_misc = NULL;
#endif /* TPP_HAVE_KEYWORD_MISC */
	result->tk_hash = hash;
	tpp_refcnt_init(&result->tk_refcnt, 1);
	result->tk_len = len;
	tpp_memcpy(result->tk_kwd, kwd, len * sizeof(tpp_char));
	result->tk_kwd[len] = (tpp_char)'\0';
	result = tpp_keywords_inskeyword(self, result);
done:
	return result;
}

#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_newkeyword_esc_(tpp_keywords *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM) {
	tpp_keyword *result;
	tpp_size len_without_esc;
	result = (tpp_keyword *)tpp_keywords_getkeyword_esc(self, kwd, len, hash, file);
	if (result != NULL)
		goto done;

	/* Must allocate a new keyword... */
	result = (tpp_keyword *)tpp_malloc(tpp_keyword_sizeof(len));
	if (result == NULL)
		goto done;

	result->tk_id = (tpp_token_id)((unsigned int)TPP_TOK_USERKEYWORD_BEGIN + self->tks_kwdc);
#if TPP_HAVE_CPP_MACROS
	result->tk_macro = NULL;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	result->tk_misc = NULL;
#endif /* TPP_HAVE_KEYWORD_MISC */
	result->tk_hash = hash;
	tpp_refcnt_init(&result->tk_refcnt, 1);
	len_without_esc = tpp_without_esc(result->tk_kwd, kwd, len, file);
	tpp_assert(len_without_esc <= len);
	result->tk_len = len_without_esc;
	if (len_without_esc < len) {
		tpp_keyword *new_result;
		new_result = (tpp_keyword *)tpp_realloc(result, tpp_keyword_sizeof(len_without_esc));
		if tpp_likely(new_result)
			result = new_result;
	}
	result->tk_kwd[len_without_esc] = (tpp_char)'\0';
	result = tpp_keywords_inskeyword(self, result);
done:
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
/* Check if "kwd" is contained in "self".
 * If so: do nothing and simply re-return "kwd"
 *
 * Otherwise, assume that "kwd" is a "builtin" keyword (as returned
 * by `tpp_builtin_getkeyword()'), in which the keyword is copied,
 * inserted into "self", and said copy is returned.
 *
 * This function must be used to make a keyword "writable" (which is
 * required before its `tk_macro' / `tk_misc' fields can safely be
 * written to (and in the later case: all fields of a potentially
 * pointed-to `tpp_keyword_misc', too)
 *
 * @return: * :   A writable copy of "kwd"
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
tpp_keywords_copybuiltin(tpp_keywords *tpp_restrict self,
                         tpp_keyword const *tpp_restrict kwd) {
	TPP_REF tpp_keyword *result;

	/* Check for simple case: if it's a user-defined keyword,
	 * it can't exist in built-ins, meaning it never needs to
	 * be copied. */
	if (TPP_TOK_ISUSERKEYWORD(kwd->tk_id))
		return (tpp_keyword *)kwd;

	/* Check if "kwd" is already contained with out map. */
	result = self->tks_bckv[kwd->tk_hash & self->tks_bckm];
	for (; result; result = result->tk_next) {
		if (result == kwd)
			return result;
	}

	/* Yes: must copy "kwd" */
	result = (tpp_keyword *)tpp_malloc(tpp_keyword_sizeof(kwd->tk_len));
	if (result == NULL)
		goto done;

#if TPP_HAVE_KEYWORD_MISC
	result->tk_misc = NULL; /* Builtin keywords never have "misc" data (flags are dynamically calculated) */
#endif /* TPP_HAVE_KEYWORD_MISC */
	result->tk_id = kwd->tk_id;
#if TPP_HAVE_CPP_MACROS
	result->tk_macro = kwd->tk_macro;
	if (result->tk_macro)
		tpp_macro_incref(result->tk_macro);
#endif /* TPP_HAVE_CPP_MACROS */
	result->tk_hash = kwd->tk_hash;
	tpp_refcnt_init(&result->tk_refcnt, 1);
	result->tk_len = kwd->tk_len;
	tpp_memcpy(result->tk_kwd, kwd->tk_kwd, (kwd->tk_len + 1) * sizeof(tpp_char));
	result = tpp_keywords_inskeyword(self, result);
done:
	return result;
}
#endif /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */



#if TPP_HAVE_KEYWORDS_OPENFILE
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) /*utf-8*/char *TPPCALL
tpp_fs_normalize(/*utf-8*/ char *dst_iter,  /* Output pointer destination buffer (with at least "srclen" char-s of space) */
                 /*utf-8*/ char *dst_base,  /* Base pointer of destination buffer (start of destination filename string) */
                 /*utf-8*/ char const *src, /* Filename string to append to "dst_iter" (".."-refs don't go beyond "dst_base") */
                 tpp_size srclen) {         /* Length of "src" (in char-s) */
	char const *src_end = src + srclen;

#if TPP_FS_HAVE_DRIVES
	if (src >= src_end)
		goto done;
	if (TPP_FS_ISABS(src)) {
		dst_iter = dst_base;
		*dst_iter++ = *src++;
		*dst_iter++ = *src++;
	}
#endif /* TPP_FS_HAVE_DRIVES */

	/* Deal with leading slashes. */
	for (;;) {
		if (src >= src_end)
			goto done;
		if (!TPP_FS_ISSEP(*src))
			break;
		++src;
		if (dst_iter <= dst_base || dst_iter[-1] != TPP_FS_SEP)
			*dst_iter++ = TPP_FS_SEP;
	}

	for (;;) {
		char const *next_sep;
		tpp_size segment_len;
		if (src >= src_end)
			goto done;

		/* At this point, "src" is at the start of some path-component,
		 * and "dst_iter" points at:
		 * - The start (dst_base)
		 * - After a TPP_FS_SEP
		 * - Just after the drive base (in case of TPP_FS_HAVE_DRIVES) */
		next_sep = src;
		while (next_sep < src_end && !TPP_FS_ISSEP(*next_sep))
			++next_sep;
		segment_len = (tpp_size)(next_sep - src);
		while (next_sep < src_end && TPP_FS_ISSEP(*next_sep))
			++next_sep; /* Skip over trailing SEP of segment */

		/* Deal with special segments. */
		switch (segment_len) {
		case 0:
			goto continue_with_next_sep; /* Empty segment -> ignore */
		case 1:
			if (src[0] == '.')
				goto continue_with_next_sep; /* Current-directory-segment -> ignore */
			break;
		case 2:
			if (src[0] == '.' && src[1] == '.' && dst_iter > dst_base) {
				/* Parent-directory-segment -> delete 1 up-ref in "dst" */
				while (dst_iter > dst_base && dst_iter[-1] == TPP_FS_SEP)
					--dst_iter;
				while (dst_iter > dst_base && dst_iter[-1] != TPP_FS_SEP)
					--dst_iter;
				goto continue_with_next_sep;
			}
			break;
		default: break;
		}

		/* Copy segment into "dst_iter" */
		tpp_memcpy(dst_iter, src, segment_len * sizeof(char));
		dst_iter += segment_len;
		if (next_sep >= src_end)
			goto done;

		/* Append SEP to "dst_iter" */
		*dst_iter++ = TPP_FS_SEP;

continue_with_next_sep:
		src = next_sep;
	}


done:
	/* Remove trailing SEP */
	if (dst_iter > dst_base && dst_iter[-1] == TPP_FS_SEP)
		--dst_iter;
	return dst_iter;
}

/* Construct the filename, open the file, and initialize "out_file" accordingly
 * @param: relative_to: The `tpp_file::tf_data.td_io.tff_name' of another file,
 *                      in case "filename" is a relative path, in which case the
 *                      filename of the file to open should be relative to the
 *                      directory of "relative_to"
 * @param: out_file:    The file that should be initialized (as `TPP_FILE_KIND_IO')
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Insufficient memory
 * @return: TPP_ENOENT: File not found (if you have additional "relative_to", try them) */
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3, 4)) tpp_errno TPPCALL
tpp_keywords_openfile_ex(/*1..1*/ tpp_keywords *tpp_restrict self,
                         /*0..1*/ char const *tpp_restrict relative_to,
                         /*1..1*/ /*utf-8*/ char const *tpp_restrict filename,
                         /*1..1*/ tpp_file *tpp_restrict out_file,
                         tpp_keyword_flags mask_flags)
#else /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3, 4)) tpp_errno TPPCALL
tpp_keywords_openfile(/*1..1*/ tpp_keywords *tpp_restrict self,
                      /*0..1*/ char const *tpp_restrict relative_to,
                      /*1..1*/ /*utf-8*/ char const *tpp_restrict filename,
                      /*1..1*/ tpp_file *tpp_restrict out_file)
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE_EX */
{
	bool is_known_keyword = false;
	tpp_io_handle handle;
	tpp_keyword *result_kwd;
	tpp_size filename_len = tpp_strlen(filename);
	if (TPP_FS_ISABS(filename) || !relative_to) {
		tpp_keyword *new_result_kwd;
		tpp_char *kwd_end;
		tpp_size result_kwd_len;
without_relative_to:
		result_kwd = (tpp_keyword *)tpp_malloc(tpp_keyword_sizeof(filename_len));
		if tpp_unlikely(!result_kwd)
			goto err_nomem;
		kwd_end = (tpp_char *)tpp_fs_normalize((char *)result_kwd->tk_kwd,
		                                       (char *)result_kwd->tk_kwd,
		                                       filename, filename_len);
		*kwd_end = (tpp_char)'\0';
		result_kwd_len = (tpp_size)(kwd_end - result_kwd->tk_kwd);
		tpp_assert(result_kwd_len <= filename_len);
		new_result_kwd = (tpp_keyword *)tpp_tryrealloc(result_kwd, tpp_keyword_sizeof(result_kwd_len));
		if tpp_likely(new_result_kwd)
			result_kwd = new_result_kwd;
		result_kwd->tk_len = result_kwd_len;
	} else {
		tpp_keyword *new_result_kwd;
		tpp_size rel_size, whole_size;
		char const *rel_base = relative_to;
		char const *last_sep = rel_base + tpp_strlen(relative_to);
		char *dst_base, *dst_iter, *dst_end;
		while (last_sep > rel_base && last_sep[-1] != TPP_FS_SEP)
			--last_sep;
		if (last_sep <= rel_base)
			goto without_relative_to;
		rel_size   = (tpp_size)(last_sep - rel_base); /* Including trailing '/' */
		whole_size = rel_size + filename_len;
		result_kwd = (tpp_keyword *)tpp_malloc(tpp_keyword_sizeof(whole_size));
		if tpp_unlikely(!result_kwd)
			goto err_nomem;
		dst_base = (char *)result_kwd->tk_kwd;
		tpp_memcpy(dst_base, rel_base, rel_size * sizeof(char)); /* Including trailing '/' */
		dst_iter = dst_base + rel_size;
		dst_end = tpp_fs_normalize(dst_base, dst_iter, filename, filename_len);
		*dst_end = '\0';
		whole_size = (tpp_size)(dst_end - dst_base);
		new_result_kwd = (tpp_keyword *)tpp_tryrealloc(result_kwd, tpp_keyword_sizeof(whole_size));
		if tpp_likely(new_result_kwd)
			result_kwd = new_result_kwd;
		result_kwd->tk_len = whole_size;
	}

	/* Check if "result_kwd" is a known keyword... */
	{
		tpp_hash hash = tpp_hashof(result_kwd->tk_kwd, result_kwd->tk_len);
		tpp_keyword *bucket = self->tks_bckv[hash & self->tks_bckm];
		for (; bucket; bucket = bucket->tk_next) {
			if (bucket->tk_hash != hash)
				continue;
			if (bucket->tk_len != result_kwd->tk_len)
				continue;
			if (tpp_memcmp(bucket->tk_kwd, result_kwd->tk_kwd,
			               result_kwd->tk_len * sizeof(tpp_char)) != 0)
				continue;

			/* Keyword already exists */
			tpp_free(result_kwd);
			is_known_keyword = true;
			result_kwd = bucket;

			/* Check if the file should be marked out. */
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
			if ((result_kwd->tk_misc) != NULL &&
			    (result_kwd->tk_misc->tkm_flags & mask_flags) != 0)
				return TPP_EMASKED;
#endif /* TPP_HAVE_KEYWORDS_OPENFILE_EX */

			goto got_result_kwd;
		}
		result_kwd->tk_hash = hash;
	}
got_result_kwd:

	/* Try to open the file */
	handle = tpp_io_open((char const *)result_kwd->tk_kwd);
	if (handle == tpp_io_handle_INVALID) {
		tpp_free(result_kwd);
		return TPP_ENOENT;
	}

	/* Initialize remaining fields of "result_kwd" and insert into keyword map */
	if (!is_known_keyword) {
		result_kwd->tk_id = (tpp_token_id)((unsigned int)TPP_TOK_USERKEYWORD_BEGIN + self->tks_kwdc);
#if TPP_HAVE_CPP_MACROS
		result_kwd->tk_macro = NULL;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
		result_kwd->tk_misc = NULL;
#endif /* TPP_HAVE_KEYWORD_MISC */
		tpp_refcnt_init(&result_kwd->tk_refcnt, 1);
		result_kwd = tpp_keywords_inskeyword(self, result_kwd);
		if tpp_unlikely(!result_kwd) {
			tpp_io_close(handle);
			goto err_nomem;
		}
	}

	/* Initialize "out_file" */
	tpp_file_init_io(out_file, (char const *)result_kwd->tk_kwd, handle);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_HAVE_KEYWORDS_OPENFILE */


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/builtins.c                                               */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS
#define _TPP_BUILTIN_KEYWORD_tk_macro_DEF  struct tpp_macro *tk_macro;
#define _TPP_BUILTIN_KEYWORD_tk_macro_INIT NULL,
#else /* TPP_HAVE_CPP_MACROS */
#define _TPP_BUILTIN_KEYWORD_tk_macro_DEF  /* nothing */
#define _TPP_BUILTIN_KEYWORD_tk_macro_INIT /* nothing */
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
#define _TPP_BUILTIN_KEYWORD_tk_misc_DEF  tpp_keyword_misc *tk_misc;
#define _TPP_BUILTIN_KEYWORD_tk_misc_INIT NULL,
#else /* TPP_HAVE_KEYWORD_MISC */
#define _TPP_BUILTIN_KEYWORD_tk_misc_DEF  /* nothing */
#define _TPP_BUILTIN_KEYWORD_tk_misc_INIT /* nothing */
#endif /* !TPP_HAVE_KEYWORD_MISC */


#if TPP_HAVE_EXTENSIONS
static struct tpp_extension_names_struct {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) char ten_##id[sizeof(name) / sizeof(char)];
#include "tpp-amalgamation.h"
#undef TPP_DEFS
} const tpp_extension_names = {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) /* .ten_##id = */ name,
#include "tpp-amalgamation.h"
#undef TPP_DEFS
};

static tpp_size const tpp_extension_name_offsets_byid[TPP_EXT_COUNT] = {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) \
	/* [id] = */ tpp_offsetof(struct tpp_extension_names_struct, ten_##id),
#include "tpp-amalgamation.h"
#undef TPP_DEFS
};
#define tpp_extension_getname_fast(id) \
	((char const *)&tpp_extension_names + tpp_extension_name_offsets_byid[id])
#endif /* TPP_HAVE_EXTENSIONS */



#if TPP_HAVE_WARNINGS

static struct tpp_warning_group_names_struct {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	char twgn_##wgroup_id##_##index[sizeof(value) / sizeof(char)];
#define TPP_WGROUP(wgroup_id, names, default)                                                  \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id) \
	char twgn_tail_##wgroup_id;
#include "tpp-amalgamation.h"
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
} const tpp_warning_group_names = {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	/* .twgn_##wgroup_id##_##index = */ value,
#define TPP_WGROUP(wgroup_id, names, default)                                                  \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id) \
	/* .twgn_tail_##wgroup_id = */ 0,
#include "tpp-amalgamation.h"
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
};

static tpp_size const tpp_warning_group_name_offsets_byid[TPP_EXT_COUNT] = {
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
	/* [wgroup_id] = */ tpp_offsetof(struct tpp_warning_group_names_struct, twgn_##wgroup_id##_0),
#include "tpp-amalgamation.h"
#undef TPP_DEFS
};
#define tpp_warning_group_getname_fast(id) \
	((char const *)&tpp_warning_group_names + tpp_warning_group_name_offsets_byid[id])


/* Return a pointer to a \0\0-terminated list of strings describing the names
 * of for the given warning group "id". Returns "NULL" if "id" is "TPP_WG_COUNT"
 * or some other invalid warning group ID. */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_warning_group_getnames(tpp_warning_group_id id) {
	if ((unsigned int)id >= (unsigned int)TPP_WG_COUNT)
		return NULL;
	return tpp_warning_group_getname_fast((unsigned int)id);
}


static struct tpp_warning_groups_struct {
#define TPP_DEFS
#define _TPP_EXPAND_WARNING_GROUP_IDS(warning_id, index, value) \
	tpp_warning_group_id twig_##warning_id##_##index;
#define _TPP_EXPAND_WARNING_GROUP_TAIL(warning_id) \
	tpp_warning_group_id twig_tail_##warning_id;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)                             \
	TPP_TUPLE_FOREACH(wgroup_ids, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WARNING_GROUP_IDS, warning_id) \
	TPP_TUPLE_IF_NONEMPTY(wgroup_ids, _TPP_EXPAND_WARNING_GROUP_TAIL, warning_id)
#include "tpp-amalgamation.h"
#undef _TPP_EXPAND_WARNING_GROUP_IDS
#undef _TPP_EXPAND_WARNING_GROUP_TAIL
#undef TPP_DEFS
} const tpp_warning_groups = {
#define TPP_DEFS
#define _TPP_EXPAND_WARNING_GROUP_IDS(warning_id, index, value) \
	/* .twig_##warning_id##_##index = */ value,
#define _TPP_EXPAND_WARNING_GROUP_TAIL(warning_id) \
	/* .twig_tail_##warning_id = */ TPP_WG_COUNT,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)                             \
	TPP_TUPLE_FOREACH(wgroup_ids, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WARNING_GROUP_IDS, warning_id) \
	TPP_TUPLE_IF_NONEMPTY(wgroup_ids, _TPP_EXPAND_WARNING_GROUP_TAIL, warning_id)
#include "tpp-amalgamation.h"
#undef _TPP_EXPAND_WARNING_GROUP_IDS
#undef _TPP_EXPAND_WARNING_GROUP_TAIL
#undef TPP_DEFS
};

static tpp_size const tpp_warning_group_offsets_byid[TPP_W_COUNT] = {
#define TPP_DEFS
#define _TPP_WARNING_GROUPS_NONEMPTY(warning_id) \
	/* [warning_id] = */ tpp_offsetof(struct tpp_warning_groups_struct, twig_##warning_id##_0),
#define _TPP_WARNING_GROUPS_EMPTY(warning_id) \
	/* [warning_id] = */ sizeof(struct tpp_warning_groups_struct) - sizeof(tpp_warning_group_id),
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)   \
	TPP_TUPLE_IF_NONEMPTY(wgroup_ids, _TPP_WARNING_GROUPS_NONEMPTY, warning_id) \
	TPP_TUPLE_IF_EMPTY(wgroup_ids, _TPP_WARNING_GROUPS_EMPTY, warning_id)
#include "tpp-amalgamation.h"
#undef _TPP_WARNING_GROUPS_NONEMPTY
#undef _TPP_WARNING_GROUPS_EMPTY
#undef TPP_DEFS
};
#define tpp_warning_groups_fast(id) \
	((tpp_warning_group_id const *)((char const *)&tpp_warning_groups + tpp_warning_group_offsets_byid[id]))


/* Returns a TPP_WG_COUNT-terminated list of group IDs associated with the given warning "id".
 * When the given "id" is "TPP_W_COUNT" or invalid, return a pointer to an empty warning-group-id-list. */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1))
tpp_warning_group_id const *TPPCALL tpp_warning_getgroups(tpp_warning_id id) {
	if ((unsigned int)id >= (unsigned int)TPP_W_COUNT) {
		return (tpp_warning_group_id const *)((char const *)&tpp_warning_groups +
		                                      sizeof(tpp_warning_groups) -
		                                      sizeof(tpp_warning_group_id));
	}
	return tpp_warning_groups_fast((unsigned int)id);
}

TPP_IMPL tpp_warnings_state const tpp_warnings_state_default = {
	/* .tws_state = */ {
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
		/* .twsg_##wgroup_id  = */ (unsigned int)default,
#include "tpp-amalgamation.h"
#if TPP_HAVE_WARNING_NUMBERS
#define TPP_DECLARE_NUMBERED_WARNING(numbers_default) \
		/* .twsn_##warning_id = */ (unsigned int)numbers_default,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
		TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, numbers_default)
#include "tpp-amalgamation.h"
#undef TPP_DECLARE_NUMBERED_WARNING
#endif /* TPP_HAVE_WARNING_NUMBERS */
#undef TPP_DEFS
	}
};

#endif /* TPP_HAVE_WARNINGS */


/* Define this macro as "tpp-amalgamation-builtins.inl" */
#ifndef TPP_CONFIG_BUILTINS_FILENAME

#if TPP_HAVE_EXTENSIONS
static tpp_size tpp_extension_name_offsets_byname[TPP_EXT_COUNT] = {};
static int tpp_extension_name_offset_compare(void const *lhs, void const *rhs) {
	tpp_size lhs_value = *(tpp_size const *)lhs;
	tpp_size rhs_value = *(tpp_size const *)rhs;
	char const *lhs_string = (char const *)&tpp_extension_names + lhs_value;
	char const *rhs_string = (char const *)&tpp_extension_names + rhs_value;
	return tpp_strcmp(lhs_string, rhs_string);
}

static void tpp_init_extension_name_offsets_byname_impl(void) {
	tpp_memcpy(tpp_extension_name_offsets_byname,
	           tpp_extension_name_offsets_byid,
	           sizeof(tpp_extension_name_offsets_byid));
	qsort(tpp_extension_name_offsets_byname, TPP_EXT_COUNT, sizeof(tpp_size),
	      &tpp_extension_name_offset_compare);
}

static void tpp_init_extension_name_offsets_byname(void) {
	tpp_once({
		tpp_init_extension_name_offsets_byname_impl();
	});
}
#endif /* TPP_HAVE_EXTENSIONS */


#if TPP_HAVE_WARNINGS
enum {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	_TPP_WGN_##wgroup_id##_##index,
#define TPP_WGROUP(wgroup_id, names, default) \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id)
#include "tpp-amalgamation.h"
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
	TPP_WGN_COUNT
};

static tpp_size const tpp_warning_group_name_offsets_bynameid[TPP_WGN_COUNT] = {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	/* [_TPP_WGN_##wgroup_id##_##index] = */ tpp_offsetof(struct tpp_warning_group_names_struct, twgn_##wgroup_id##_##index),
#define TPP_WGROUP(wgroup_id, names, default) \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id)
#include "tpp-amalgamation.h"
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
};

static tpp_size tpp_warning_group_name_offsets_byname[TPP_WGN_COUNT] = {};
static int tpp_warning_group_name_offset_compare(void const *lhs, void const *rhs) {
	tpp_size lhs_value = *(tpp_size const *)lhs;
	tpp_size rhs_value = *(tpp_size const *)rhs;
	char const *lhs_string = (char const *)&tpp_warning_group_names + lhs_value;
	char const *rhs_string = (char const *)&tpp_warning_group_names + rhs_value;
	return tpp_strcmp(lhs_string, rhs_string);
}

static void tpp_init_warning_group_name_offsets_byname_impl(void) {
	tpp_memcpy(tpp_warning_group_name_offsets_byname,
	           tpp_warning_group_name_offsets_bynameid,
	           sizeof(tpp_warning_group_name_offsets_bynameid));
	qsort(tpp_warning_group_name_offsets_byname, TPP_WG_COUNT, sizeof(tpp_size),
	      &tpp_warning_group_name_offset_compare);
}

static void tpp_init_warning_group_name_offsets_byname(void) {
	tpp_once({
		tpp_init_warning_group_name_offsets_byname_impl();
	});
}
#endif /* TPP_HAVE_WARNINGS */

/* Implement TPP's hashing algorithm for constant strings, using only the preprocessor!
 * -> THIS! Is the power that TPP can wield: Full functional programming */
#if defined(__TPP_VERSION__) && (TPP_SIZEOF_tpp_hash == 4 || TPP_SIZEOF_tpp_hash == 8)
#define TPP_PRIVATE_PP_CAT2(a,b) a##b
#define TPP_PRIVATE_PP_CAT(a,b) TPP_PRIVATE_PP_CAT2(a,b)
#define TPP_PRIVATE_HASHOF_0(result,str) result
#pragma warning(disable: 108) /* Index out-of-bounds in `__TPP_EVAL' */

/* Using some sick-a$$ TPP extensions, we can actually
 * calculate keyword hashes within the preprocessor! */
#pragma extension(push,"-fmacro-recursion")
#if TPP_SIZEOF_tpp_hash == 4
#define TPP_PRIVATE_HASHOF_1(result,str) TPP_PRIVATE_HASHOF2(__TPP_EVAL((result*263+str[0])&0xfffffffful),__TPP_EVAL(str[1:]))
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_PRIVATE_HASHOF_1(result,str) TPP_PRIVATE_HASHOF2(__TPP_EVAL((result*263+str[0])&0xffffffffffffffffull),__TPP_EVAL(str[1:]))
#endif
#define TPP_PRIVATE_HASHOF2(result,str) TPP_PRIVATE_PP_CAT(TPP_PRIVATE_HASHOF_,__TPP_EVAL(!!str))(result,str)
#pragma extension(pop)
#if TPP_SIZEOF_tpp_hash == 4
#define TPP_HASHOF(str) TPP_PRIVATE_PP_CAT(TPP_PRIVATE_HASHOF2(1,str),ul)
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_HASHOF(str) TPP_PRIVATE_PP_CAT(TPP_PRIVATE_HASHOF2(1,str),ull)
#endif
#endif /* __TPP_VERSION__ && (TPP_SIZEOF_tpp_hash == 4 || TPP_SIZEOF_tpp_hash == 8) */

#ifdef TPP_HASHOF
#define TPP_MAYBE_HASHOF(s) TPP_HASHOF(s)
#else /* TPP_HASHOF */
#define TPP_MAYBE_HASHOF(s) 0
#endif /* !TPP_HASHOF */


/* Create definitions for builtin keywords */
#define TPP_DEFINE_BUILTIN_KEYWORD(id, str)                      \
	TPP_INTERN_DECL struct tpp_builtin_keyword_struct_##id {     \
		tpp_token_id         tk_id;                              \
		struct tpp_keyword  *tk_next;                            \
		_TPP_BUILTIN_KEYWORD_tk_macro_DEF                        \
		_TPP_BUILTIN_KEYWORD_tk_misc_DEF                         \
		tpp_hash             tk_hash;                            \
		tpp_refcnt           tk_refcnt;                          \
		tpp_size             tk_len;                             \
		char                 tk_kwd[sizeof(str) / sizeof(char)]; \
	} tpp_builtin_keyword_##id;                                  \
	TPP_INTERN_IMPL struct tpp_builtin_keyword_struct_##id       \
	tpp_builtin_keyword_##id = {                                 \
		/* .tk_id        = */ id,                                \
		/* .tk_next      = */ NULL,                              \
		_TPP_BUILTIN_KEYWORD_tk_macro_INIT                       \
		_TPP_BUILTIN_KEYWORD_tk_misc_INIT                        \
		/* .tk_hash      = */ TPP_MAYBE_HASHOF(str),             \
		/* .tk_refcnt    = */ TPP_REFCNT_INIT(1),                \
		/* .tk_len       = */ (sizeof(str) / sizeof(char)) - 1,  \
		/* .tk_kwd       = */ str                                \
	};

#define TPP_DEFS
#define TPP_KWD(id, string) \
	TPP_DEFINE_BUILTIN_KEYWORD(id, string)
#include "tpp-amalgamation.h"
#undef TPP_DEFS

#define tpp_builtin_keywords_list tpp_builtin_keywords_list
static tpp_keyword *tpp_builtin_keywords_list[] = {
#define TPP_DEFS
#define TPP_KWD(id, string) (tpp_keyword *)&tpp_builtin_keyword_##id,
#include "tpp-amalgamation.h"
#undef TPP_DEFS
	NULL
};

#define TPP_BUILTIN_KEYWORD_COUNT  (TPP_TOK_USERKEYWORD_BEGIN - TPP_TOK_KEYWORD_BEGIN)
#define TPP_BUILTIN_KEYWORD_MASK_1 (TPP_BUILTIN_KEYWORD_COUNT | (TPP_BUILTIN_KEYWORD_COUNT >> 16))
#define TPP_BUILTIN_KEYWORD_MASK_2 (TPP_BUILTIN_KEYWORD_MASK_1 | (TPP_BUILTIN_KEYWORD_MASK_1 >> 8))
#define TPP_BUILTIN_KEYWORD_MASK_3 (TPP_BUILTIN_KEYWORD_MASK_2 | (TPP_BUILTIN_KEYWORD_MASK_2 >> 4))
#define TPP_BUILTIN_KEYWORD_MASK_4 (TPP_BUILTIN_KEYWORD_MASK_3 | (TPP_BUILTIN_KEYWORD_MASK_3 >> 2))
#define TPP_BUILTIN_KEYWORD_MASK_5 (TPP_BUILTIN_KEYWORD_MASK_4 | (TPP_BUILTIN_KEYWORD_MASK_4 >> 1))
enum { TPP_BUILTIN_KEYWORD_MASK = TPP_BUILTIN_KEYWORD_MASK_5 };
#define TPP_BUILTIN_KEYWORD_MASK TPP_BUILTIN_KEYWORD_MASK
TPP_STATIC_ASSERT(tpp_lengthof(tpp_builtin_keywords_list) == (TPP_BUILTIN_KEYWORD_COUNT + 1));

static tpp_keyword *tpp_builtin_keyword_table[TPP_BUILTIN_KEYWORD_MASK + 1];

static void tpp_init_builtin_keywords_impl(void) {
	tpp_size i;
	for (i = 0; i < TPP_BUILTIN_KEYWORD_COUNT; ++i) {
		tpp_keyword *kwd = tpp_builtin_keywords_list[i];
		tpp_keyword **bucket;
#ifndef TPP_HASHOF
		kwd->tk_hash = tpp_hashof(kwd->tk_kwd, kwd->tk_len);
#endif /* !TPP_HASHOF */
		bucket = &tpp_builtin_keyword_table[kwd->tk_hash & TPP_BUILTIN_KEYWORD_MASK];
		kwd->tk_next = *bucket;
		*bucket = kwd;
	}
}

static void tpp_init_builtin_keywords(void) {
	tpp_once({
		tpp_init_builtin_keywords_impl();
	});
}
#else /* !TPP_CONFIG_BUILTINS_FILENAME */

#define TPP_BUILTIN_KEYWORD_DECL(id, kwd_len)                \
	TPP_INTERN_DECL struct tpp_builtin_keyword_struct_##id { \
		tpp_token_id         tk_id;                          \
		struct tpp_keyword  *tk_next;                        \
		_TPP_BUILTIN_KEYWORD_tk_macro_DEF                    \
		_TPP_BUILTIN_KEYWORD_tk_misc_DEF                     \
		tpp_hash             tk_hash;                        \
		tpp_refcnt           tk_refcnt;                      \
		tpp_size             tk_len;                         \
		char                 tk_kwd[kwd_len + 1];            \
	} tpp_builtin_keyword_##id;
#if TPP_SIZEOF_tpp_hash == 4
#define TPP_BUILTIN_MAKEHASH(hash_hi, hash_lo) UINT32_C(0x##hash_lo)
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_BUILTIN_MAKEHASH(hash_hi, hash_lo) UINT64_C(0x##hash_hi##hash_lo)
#else /* TPP_SIZEOF_tpp_hash == ... */
#endif /* TPP_SIZEOF_tpp_hash != ... */
#define TPP_BUILTIN_KEYWORD_IMPL(id, kwd_len, kwd, next, hash_hi, hash_lo) \
	TPP_INTERN_IMPL struct tpp_builtin_keyword_struct_##id                 \
	tpp_builtin_keyword_##id = {                                           \
		/* .tk_id        = */ id,                                          \
		/* .tk_next      = */ (tpp_keyword *)next,                         \
		_TPP_BUILTIN_KEYWORD_tk_macro_INIT                                 \
		_TPP_BUILTIN_KEYWORD_tk_misc_INIT                                  \
		/* .tk_hash      = */ TPP_BUILTIN_MAKEHASH(hash_hi, hash_lo),      \
		/* .tk_refcnt    = */ TPP_REFCNT_INIT(1),                          \
		/* .tk_len       = */ kwd_len,                                     \
		/* .tk_kwd       = */ kwd                                          \
	};
#define TPP_BUILTIN_KEYWORD_TABLE_ITEM(k)    (tpp_keyword const *)k,
#define TPP_BUILTIN_KEYWORD_TABLE_ITEM_BLANK NULL,
#define TPP_BUILTIN_KEYWORD_TABLE(items) \
	static tpp_keyword const *tpp_builtin_keyword_table[TPP_BUILTIN_KEYWORD_MASK + 1] = { items };

#if TPP_HAVE_EXTENSIONS
#define TPP_BUILTIN_EXTENSION_NAMES(names) \
	static tpp_size const tpp_extension_name_offsets_byname[TPP_EXT_COUNT] = { names };
#define TPP_BUILTIN_EXTENSION_NAME(id, name) tpp_offsetof(struct tpp_extension_names_struct, ten_##id),
#else /* TPP_HAVE_EXTENSIONS */
#define TPP_BUILTIN_EXTENSION_NAMES(names)   /* nothing */
#define TPP_BUILTIN_EXTENSION_NAME(id, name) /* nothing */
#endif /* !TPP_HAVE_EXTENSIONS */

#if TPP_HAVE_WARNINGS
#define TPP_BUILTIN_WARNING_GROUP_NAMES(names) \
	static tpp_size const tpp_warning_group_name_offsets_byname[/*TPP_WGN_COUNT*/] = { names };
#define TPP_BUILTIN_WARNING_GROUP_NAME(id, index, name) tpp_offsetof(struct tpp_warning_group_names_struct, twgn_##id##_##index),
#else /* TPP_HAVE_WARNING_GROUPS */
#define TPP_BUILTIN_WARNING_GROUP_NAMES(names)          /* nothing */
#define TPP_BUILTIN_WARNING_GROUP_NAME(id, index, name) /* nothing */
#endif /* !TPP_HAVE_EXTENSIONS */

/* HINT: This file gets generated by "make-amalgamation-builtins.dee" */
#include TPP_CONFIG_BUILTINS_FILENAME

#undef TPP_BUILTIN_KEYWORD_DECL
#undef TPP_BUILTIN_MAKEHASH
#undef TPP_BUILTIN_KEYWORD_IMPL
#undef TPP_BUILTIN_KEYWORD_TABLE_ITEM
#undef TPP_BUILTIN_KEYWORD_TABLE_ITEM_BLANK
#undef TPP_BUILTIN_KEYWORD_TABLE
#undef TPP_BUILTIN_EXTENSION_NAMES
#undef TPP_BUILTIN_EXTENSION_NAME
#undef TPP_BUILTIN_WARNING_GROUP_NAMES
#undef TPP_BUILTIN_WARNING_GROUP_NAME

#define tpp_init_builtin_keywords()                  (void)0
#define tpp_init_extension_name_offsets_byname()     (void)0
#define tpp_init_warning_group_name_offsets_byname() (void)0
#endif /* TPP_CONFIG_BUILTINS_FILENAME */


/* Assert that generated stuff is valid */
TPP_STATIC_ASSERT(TPP_BUILTIN_KEYWORD_COUNT == (TPP_TOK_USERKEYWORD_BEGIN - TPP_TOK_KEYWORD_BEGIN));
TPP_STATIC_ASSERT(TPP_BUILTIN_KEYWORD_MASK >= (TPP_BUILTIN_KEYWORD_COUNT + 1));
TPP_STATIC_ASSERT(tpp_lengthof(tpp_builtin_keyword_table) == (TPP_BUILTIN_KEYWORD_MASK + 1));


/* Lookup one of the built-in, pre-defined keywords */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword(tpp_char const *tpp_restrict kwd,
                       tpp_size len, tpp_hash hash) {
	tpp_keyword const *result;
	tpp_init_builtin_keywords();
	result = tpp_builtin_keyword_table[hash & TPP_BUILTIN_KEYWORD_MASK];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (result->tk_len != len)
			continue;
		if (tpp_memcmp(result->tk_kwd, kwd, len * sizeof(tpp_char)) == 0)
			break;
	}
	return result;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_byid(enum tpp_token_id id) {
	tpp_init_builtin_keywords();
	if (TPP_TOK_ISBUILTINKEYWORD(id)) {
#ifdef tpp_builtin_keywords_list
		return tpp_builtin_keywords_list[id - TPP_TOK_KEYWORD_BEGIN];
#else  /* tpp_builtin_keywords_list */
		tpp_hash i;
		for (i = 0; i <= TPP_BUILTIN_KEYWORD_MASK; ++i) {
			tpp_keyword const *chain = tpp_builtin_keyword_table[i];
			for (; chain; chain = chain->tk_next) {
				if (chain->tk_id == id)
					return chain;
			}
		}
#endif /* !tpp_builtin_keywords_list */
	}
	return NULL;
}

#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_esc_(tpp_char const *tpp_restrict kwd,
                            tpp_size len, tpp_hash hash
                            tpp_bse_file__PARAM) {
	tpp_keyword const *result;
	tpp_init_builtin_keywords();
	result = tpp_builtin_keyword_table[hash & TPP_BUILTIN_KEYWORD_MASK];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (tpp_memcmp_esc(result->tk_kwd,
		                   result->tk_len,
		                   kwd, len, file) == 0)
			break;
	}
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */



#if TPP_HAVE_KEYWORD_FLAGS
/* Return the effective set of flags for a given "kwd"
 * Since the effective flags for (certain) builtin keywords
 * can depend on active extensions/features, this can only
 * be done in the context of a specific lexer (rather than
 * stand-alone using only the "kwd")
 *
 * @return: * : Set of `TPP_KEYWORD_FLAG_*' */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_flags TPPCALL
tpp_lexer_getkeywordflags(tpp_lexer *tpp_restrict self,
                          tpp_keyword const *tpp_restrict kwd) {
	/* Special handling for built-in keywords */
	if (TPP_TOK_ISBUILTINKEYWORD(kwd->tk_id)) {
		(void)self;
		switch (kwd->tk_id) {
#define TPP_DEFS
#define TPP_KWD_FLAGS(id, flags_expr) \
		case id: return flags_expr;
#define tpp_current_lexer()      self
#define tpp_current_keyword()    kwd
#define tpp_current_keyword_id() kwd->tk_id
#include "tpp-amalgamation.h"
#undef tpp_current_lexer
#undef tpp_current_keyword
#undef tpp_current_keyword_id
#undef TPP_DEFS
		default: break;
		}
	} else if (kwd->tk_misc != NULL) {
		return kwd->tk_misc->tkm_flags;
	}
	return TPP_KEYWORD_FLAG_NORMAL;
}
#endif /* TPP_HAVE_KEYWORD_FLAGS */

#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
/* Returns true if "kwd" should be considered to be "#if defined()"
 * Since "builtin" keywords can be considered to be "defined", even
 * when `kwd->tk_macro == NULL', this function is needed to handle
 * such macros. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_getkeyworddefined(tpp_lexer *tpp_restrict self,
                            tpp_keyword const *tpp_restrict kwd) {
#if TPP_HAVE_CPP_MACROS
	if (kwd->tk_macro != NULL)
		return true;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_CPP_BUILTIN_MACROS
#if TPP_CONF_IS_RT(TPP_HAVE_CPP_BUILTIN_MACROS)
	if (!tpp_lexer_has(self, CPP_BUILTIN_MACROS))
		return false;
#endif /* TPP_CONF_IS_RT(TPP_HAVE_CPP_BUILTIN_MACROS) */
	(void)self;
	switch (kwd->tk_id) {
#define TPP_DEFS
#define TPP_MACRO(id, if_expr) \
	case id: return if_expr;
#define tpp_current_lexer()      self
#define tpp_current_keyword()    kwd
#define tpp_current_keyword_id() kwd->tk_id
#include "tpp-amalgamation.h"
#undef tpp_current_lexer
#undef tpp_current_keyword
#undef tpp_current_keyword_id
#undef TPP_DEFS
	default: break;
	}
#endif /* TPP_HAVE_CPP_BUILTIN_MACROS */
	return false;
}
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */


#if TPP_HAVE_EXTENSIONS
/* Convert between extension IDs and their human-readable names. */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_extension_getname(tpp_extension_id id) {
	if ((unsigned int)id >= (unsigned int)TPP_EXT_COUNT)
		return NULL;
	return tpp_extension_getname_fast((unsigned int)id);
}

static TPP_WUNUSED tpp_extension_id TPPCALL
tpp_extension_byname_offset(tpp_size name_offset) {
	unsigned int lo, hi;
	lo = 0;
	hi = tpp_lengthof(tpp_extension_name_offsets_byid);
	for (;;) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_extension_name_offsets_byid[mid];
		tpp_assert(lo < hi);
		if (name_offset < mid_offset) {
			hi = mid;
		} else if (name_offset > mid_offset) {
			lo = mid + 1;
		} else {
			return (tpp_extension_id)mid;
		}
	}
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_extension_id TPPCALL
tpp_extension_byname_ex(char const *tpp_restrict name, tpp_size name_maxlen) {
	unsigned int lo, hi;
	tpp_init_extension_name_offsets_byname();
	name_maxlen = tpp_strnlen(name, name_maxlen);
	lo = 0;
	hi = tpp_lengthof(tpp_extension_name_offsets_byname);
	while (lo < hi) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_extension_name_offsets_byname[mid];
		char const *mid_name = (char const *)&tpp_extension_names + mid_offset;
		int cmp = tpp_memcmp(name, mid_name, name_maxlen * sizeof(char));
		if (cmp == 0 && mid_name[name_maxlen])
			cmp = -1;
		if (cmp < 0) {
			hi = mid;
		} else if (cmp > 0) {
			lo = mid + 1;
		} else {
			/* Found offset into name-table. Now find the matching offset in
			 * "tpp_extension_name_offsets_byid" (which is sorted by said offset),
			 * at which point the index into "tpp_extension_name_offsets_byid"
			 * will be the extension id. */
			return tpp_extension_byname_offset(mid_offset);
		}
	}
	return TPP_EXT_COUNT;
}

/* Returns the ID of the extension with the name that is closest to "name"
 * When no extensions are defined (at all), this will return "TPP_EXT_COUNT" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_extension_id TPPCALL
tpp_extension_nearest_ex(char const *tpp_restrict name, tpp_size name_maxlen) {
	(void)name;
	(void)name_maxlen;
	/* TODO */
	return TPP_EXT_COUNT;
}
#endif /* TPP_HAVE_EXTENSIONS */



#if TPP_HAVE_WARNINGS

static struct tpp_warning_format_strings_struct {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	char twf_##warning_id[sizeof(format) / sizeof(char)];
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr)
#include "tpp-amalgamation.h"
#undef TPP_DEFS
} const tpp_warning_format_strings = {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	/* [twf_##warning_id] = */ format,
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr)
#include "tpp-amalgamation.h"
#undef TPP_DEFS
};

static tpp_size const tpp_warning_format_string_offsets[TPP_W_COUNT] = {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	/* [warning_id] = */ tpp_offsetof(struct tpp_warning_format_strings_struct, twf_##warning_id),
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr) \
	/* [warning_id] = */ sizeof(struct tpp_warning_format_strings_struct) - 1,
#include "tpp-amalgamation.h"
#undef TPP_DEFS
};

/* Returns the "tpp_lexer_printf_warning"-style format string assigned with "id".
 * When "id" is TPP_W_COUNT, invalid, or declared as "TPP_WARNING_EX", return "NULL" instead. */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_warning_getformat(tpp_warning_id id) {
	tpp_size offset;
	char const *result;
	if ((unsigned int)id >= (unsigned int)TPP_W_COUNT)
		return NULL;
	offset = tpp_warning_format_string_offsets[(unsigned int)id];
	result = (char const *)&tpp_warning_format_strings + offset;
	if (*result == '\0')
		result = NULL;
	return result;
}


#if TPP_HAVE_WARNING_NUMBERS
/* Returns the warning ID linked to a given "number", or "TPP_W_COUNT" if "number" is unknown */
TPP_IMPL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_ofnumber(unsigned int number) {
	switch (number) {
#define TPP_DEFS
#define TPP_WARNING_NUMBER_CASE(_, index, value) \
	case value: return _;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_FOREACH(numbers, TPP_TUPLE_FOREACH_DUMMY_SEP, TPP_WARNING_NUMBER_CASE, warning_id)
#include "tpp-amalgamation.h"
#undef TPP_WARNING_NUMBER_CASE
#undef TPP_DEFS
	default: break;
	}
	return TPP_W_COUNT;
}

/* Returns the TPP_WARNING_NUMBER_INVALID-terminated list of warning numbers
 * associated with "warning_id". If "warning_id" doesn't have any warning
 * numbers, return a pointer to "TPP_WARNING_NUMBER_INVALID". */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED unsigned int const *TPPCALL
tpp_warning_getnumbers(tpp_warning_id warning_id) {
	static unsigned int const dummy_warning_numbers[1] = { TPP_WARNING_NUMBER_INVALID };
	switch (warning_id) {
#define TPP_DEFS
#define TPP_WARNING_NUMBERS_CASE_ITEM(_, index, value) value,
#define TPP_WARNING_NUMBERS_CASE_(warning_id, numbers)                                                \
	case warning_id: {                                                                                \
		static unsigned int const this_warning_numbers[] = {                                          \
			TPP_TUPLE_FOREACH(numbers, TPP_TUPLE_FOREACH_DUMMY_SEP, TPP_WARNING_NUMBERS_CASE_ITEM, ~) \
			TPP_WARNING_NUMBER_INVALID                                                                \
		};                                                                                            \
		return this_warning_numbers;                                                                  \
	}
#define TPP_WARNING_NUMBERS_CASE(args) TPP_WARNING_NUMBERS_CASE_ args
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, TPP_WARNING_NUMBERS_CASE, (warning_id, numbers))
#include "tpp-amalgamation.h"
#undef TPP_WARNING_NUMBERS_CASE
#undef TPP_DEFS
	default: break;
	}
	return dummy_warning_numbers;
}


/* Returns the context-id of a given (should-be) "numbered" warning.
 * When "warning_id" is invalid, "TPP_W_COUNT", or not numbered, this
 * function will return "TPP_WC_COUNT" instead. */
TPP_IMPL TPP_WUNUSED tpp_warning_context_id TPPCALL
tpp_warning_context_id_ofwarning(tpp_warning_id warning_id) {
	switch (warning_id) {
#define TPP_DEFS
#define _TPP_CASE_NUMBERED_WARNING(warning_id) \
	case warning_id: return TPP_WC_##warning_id;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, _TPP_CASE_NUMBERED_WARNING, warning_id)
#include "tpp-amalgamation.h"
#undef _TPP_CASE_NUMBERED_WARNING
#undef TPP_DEFS
	default: break;
	}
	return TPP_WC_COUNT;
}

/* Returns the warning-id linked to "ctx_id", when `tpp_warning_context_id_isnumber(ctx_id)'.
 * When no warning is linked to "ctx_id" ("ctx_id" is either linked to a warning group, or
 * is "TPP_WC_COUNT" or some other invalid ID), return "TPP_W_COUNT" instead. */
TPP_IMPL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_context_id_aswarning(tpp_warning_context_id ctx_id) {
	switch (ctx_id) {
#define TPP_DEFS
#define _TPP_CASE_NUMBERED_WARNING(warning_id) \
	case TPP_WC_##warning_id: return warning_id;
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, _TPP_CASE_NUMBERED_WARNING, warning_id)
#include "tpp-amalgamation.h"
#undef _TPP_CASE_NUMBERED_WARNING
#undef TPP_DEFS
	default: break;
	}
	return TPP_W_COUNT;
}
#endif /* TPP_HAVE_WARNING_NUMBERS */


static TPP_WUNUSED tpp_warning_group_id TPPCALL
tpp_warning_group_byname_offset(tpp_size name_offset) {
	unsigned int lo, hi;
	lo = 0;
	hi = tpp_lengthof(tpp_warning_group_name_offsets_byid);
	for (;;) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_warning_group_name_offsets_byid[mid];
		tpp_assert(lo < hi);
		if (name_offset < mid_offset) {
			hi = mid;
		} else if (name_offset > mid_offset) {
			lo = mid + 1;
		} else {
			return (tpp_warning_group_id)mid;
		}
	}
}

/* @return: TPP_WG_COUNT: No such warning_group */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_group_id TPPCALL
tpp_warning_group_byname_ex(char const *tpp_restrict name, tpp_size name_maxlen) {
	unsigned int lo, hi;
	tpp_init_warning_group_name_offsets_byname();
	name_maxlen = tpp_strnlen(name, name_maxlen);
	lo = 0;
	hi = tpp_lengthof(tpp_warning_group_name_offsets_byname);
	while (lo < hi) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_warning_group_name_offsets_byname[mid];
		char const *mid_name = (char const *)&tpp_warning_group_names + mid_offset;
		int cmp = tpp_memcmp(name, mid_name, name_maxlen * sizeof(char));
		if (cmp == 0 && mid_name[name_maxlen])
			cmp = -1;
		if (cmp < 0) {
			hi = mid;
		} else if (cmp > 0) {
			lo = mid + 1;
		} else {
			/* Found offset into name-table. Now find the matching offset in
			 * "tpp_warning_group_name_offsets_byid" (which is sorted by said offset),
			 * at which point the index into "tpp_warning_group_name_offsets_byid"
			 * will be the warning_group id. */
			return tpp_warning_group_byname_offset(mid_offset);
		}
	}
	return TPP_WG_COUNT;
}


/* Returns the ID of the warning group with the name that is closest to "name"
 * When no warning groups are defined (at all), this will return "TPP_WG_COUNT" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_group_id TPPCALL
tpp_warning_group_nearest_ex(char const *tpp_restrict name, tpp_size name_maxlen) {
	(void)name;
	(void)name_maxlen;
	/* TODO */
	return TPP_WG_COUNT;
}
#endif /* TPP_HAVE_WARNINGS */


#if TPP_HAVE_CPP_MACROS
/* Return the hard-coded expansion of the builtin macro linked to "id".
 * If "id" isn't a builtin keyword, or that keyword doesn't specify a
 * value for "TPP_BUILTIN_MACRO()", return "NULL" instead. */
TPP_IMPL TPP_CONSTCALL TPP_WUNUSED tpp_builtin_macro const *TPPCALL
tpp_macro_getbuiltin(tpp_token_id id) {
	switch (id) {
#define TPP_DEFS
#define TPP_BUILTIN_MACRO(keyword_id, value)                          \
	case keyword_id: {                                                \
		static TPP_BUILTIN_MACRO_DEFINE(builtin_##keyword_id, value); \
		return (tpp_builtin_macro const *)&builtin_##keyword_id;      \
	}	break;
#include "tpp-amalgamation.h"
#undef TPP_DEFS
	default: break;
	}
	return NULL;
}
#endif /* TPP_HAVE_CPP_MACROS */


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/macro.c                                                  */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_macro_destroy(tpp_macro *tpp_restrict self) {
	if (self->tm_body_chunk)
		tpp_string_decref(self->tm_body_chunk);
	if (TPP_MACRO_KIND_ISFUNC(self->tm_kind)) {
		tpp_free(self->tm_data.tmd_func.tmf_argv);
		tpp_free(self->tm_data.tmd_func.tmf_argbuf);
	}
}


#if TPP_HAVE_MACRO_EQUALS
/* Compare 2 macro definitions to see if they are identical. */
TPP_IMPL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_macro_equals(tpp_macro const *lhs, tpp_macro const *rhs) {
	(void)lhs;
	(void)rhs;
	/* TODO */
	return false;
}
#endif /* TPP_HAVE_MACRO_EQUALS */


/* Figure out the line/column of "pos" in "expanded_text", as produced
 * by "self", which must be "TPP_MACRO_KIND_ISFUNC(self->tm_kind)". */
TPP_INTERN_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_lcinfo TPPCALL
tpp_macro_func_lcinfo(tpp_macro const *tpp_restrict self,
                      tpp_string const *expanded_text,
                      tpp_char const *pos) {
	(void)self;
	(void)expanded_text;
	(void)pos;
	/* TODO */
	return tpp_lcinfo_of(0, 0);
}

#endif /* TPP_HAVE_CPP_MACROS */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/extensions.c                                             */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_EXTENSIONS

/* Default extension state */
TPP_CONST_IMPL tpp_extensions_state const tpp_extensions_state_default = {
	/* .tes_flags = */ {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) /* .tef_##id = */ default,
#include "tpp-amalgamation.h"
#undef TPP_DEFS
	}
};


#if TPP_HAVE_EXTENSIONS_PUSH_POP
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_extensions_fini(tpp_extensions *tpp_restrict self) {
	tpp_extensions *iter = self->te_prev;
	while (iter != NULL) {
		tpp_extensions *prev = iter->te_prev;
		tpp_free(iter);
		iter = prev;
	}
}

/* Pop the current extensions state (may only be called when `tpp_extensions_canpop(self)') */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_extensions_pop(tpp_extensions *tpp_restrict self) {
	tpp_assert(tpp_extensions_canpop(self));
	if (self->te_pushcnt == 0) {
		tpp_extensions *prev = self->te_prev;
		tpp_memcpy(self, prev, sizeof(tpp_extensions));
		tpp_free(prev);
		tpp_assert(self->te_pushcnt != 0);
	}
	--self->te_pushcnt;
}

/* @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: OOM */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_extensions_setid(tpp_extensions *tpp_restrict self,
                     tpp_extension_id id, bool enabled) {
	if (tpp_extensions_mustcopy(self)) {
		tpp_extensions *copy;
		if (!!tpp_extensions_state_getid(&self->te_state, id) == !!enabled)
			return TPP_EOK; /* Unchanged -> no need to actually copy! */
		copy = (tpp_extensions *)tpp_malloc(sizeof(tpp_extensions));
		if tpp_unlikely(!copy)
			goto err_nomem;
		tpp_memcpy(copy, self, sizeof(tpp_extensions));
		self->te_prev    = copy;
		self->te_pushcnt = 0;
	}
	tpp_extensions_state_setid(&self->te_state, id, enabled);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_HAVE_EXTENSIONS_PUSH_POP */
#endif /* TPP_HAVE_EXTENSIONS */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/features.c                                               */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_FEATURES
TPP_CONST_IMPL tpp_features const tpp_features_default = {
	/* .tf_flags = */ {
#if TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS)
		/* .tff_TRIGRAPHS                            = */ TPP_CONF_DEFAULT(TPP_HAVE_TRIGRAPHS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_DIGRAPHS)
		/* .tff_DIGRAPHS                             = */ TPP_CONF_DEFAULT(TPP_HAVE_DIGRAPHS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_DIGRAPHS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LF)
		/* .tff_TPP_TOK_LF                           = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LF),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LF) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SPACE)
		/* .tff_TPP_TOK_SPACE                        = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SPACE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_COMMENT)
		/* .tff_TPP_TOK_COMMENT                      = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_COMMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_COMMENT)
		/* .tff_TPP_TOK_CXX_COMMENT                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_COMMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_C_COMMENT)
		/* .tff_TPP_TOK_C_COMMENT                    = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_C_COMMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_C_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PASCAL_COMMENT)
		/* .tff_TPP_TOK_PASCAL_COMMENT               = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PASCAL_COMMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PASCAL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SHELL_COMMENT)
		/* .tff_TPP_TOK_SHELL_COMMENT                = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SHELL_COMMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SHELL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_ASM_COMMENT)
		/* .tff_TPP_TOK_ASM_COMMENT                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_ASM_COMMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_ASM_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SQL_COMMENT)
		/* .tff_TPP_TOK_SQL_COMMENT                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SQL_COMMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SQL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_DOLLAR)
		/* .tff_TPP_TOK_DOLLAR                       = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_DOLLAR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_DOLLAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_INT)
		/* .tff_TPP_TOK_INT                          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_INT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_INT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_FLOAT)
		/* .tff_TPP_TOK_FLOAT                        = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_FLOAT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_FLOAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CHAR)
		/* .tff_TPP_TOK_CHAR                         = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CHAR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CHAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STRING)
		/* .tff_TPP_TOK_STRING                       = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STRING),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STRING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL)
		/* .tff_TPP_TOK_CXX_RAW_STRING_LITERAL       = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL)
		/* .tff_TPP_TOK_CXX_WIDE_STRING_LITERAL      = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL)
		/* .tff_TPP_TOK_CXX_UTF8_STRING_LITERAL      = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL)
		/* .tff_TPP_TOK_CXX_UTF16_STRING_LITERAL     = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL)
		/* .tff_TPP_TOK_CXX_UTF32_STRING_LITERAL     = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL)
		/* .tff_TPP_TOK_CXX_RAW_CHAR_LITERAL         = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL)
		/* .tff_TPP_TOK_CXX_WIDE_CHAR_LITERAL        = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL)
		/* .tff_TPP_TOK_CXX_UTF8_CHAR_LITERAL        = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL)
		/* .tff_TPP_TOK_CXX_UTF16_CHAR_LITERAL       = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL)
		/* .tff_TPP_TOK_CXX_UTF32_CHAR_LITERAL       = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL)
		/* .tff_TPP_TOK_RAW_STRING_LITERAL           = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
		/* .tff_TPP_TOK_RAW_CHAR_LITERAL             = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL)
		/* .tff_TPP_TOK_BLOCK_STRING_LITERAL         = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL)
		/* .tff_TPP_TOK_BLOCK_CHAR_LITERAL           = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE)
		/* .tff_TPP_TOK_STRING_ALLOW_MULTILINE       = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE)
		/* .tff_TPP_TOK_LANGLE_LANGLE                = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE)
		/* .tff_TPP_TOK_RANGLE_RANGLE                = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL)
		/* .tff_TPP_TOK_EQUAL_EQUAL                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL)
		/* .tff_TPP_TOK_EXCLAIM_EQUAL                = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL)
		/* .tff_TPP_TOK_RANGLE_EQUAL                 = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL)
		/* .tff_TPP_TOK_LANGLE_EQUAL                 = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_DOT_DOT_DOT)
		/* .tff_TPP_TOK_DOT_DOT_DOT                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_DOT_DOT_DOT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_DOT_DOT_DOT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PLUS_EQUAL)
		/* .tff_TPP_TOK_PLUS_EQUAL                   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PLUS_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PLUS_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_EQUAL)
		/* .tff_TPP_TOK_MINUS_EQUAL                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_EQUAL)
		/* .tff_TPP_TOK_STAR_EQUAL                   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SLASH_EQUAL)
		/* .tff_TPP_TOK_SLASH_EQUAL                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SLASH_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SLASH_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PERCENT_EQUAL)
		/* .tff_TPP_TOK_PERCENT_EQUAL                = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PERCENT_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PERCENT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL)
		/* .tff_TPP_TOK_LANGLE_LANGLE_EQUAL          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL)
		/* .tff_TPP_TOK_RANGLE_RANGLE_EQUAL          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_AMP_EQUAL)
		/* .tff_TPP_TOK_AMP_EQUAL                    = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_AMP_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_AMP_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PIPE_EQUAL)
		/* .tff_TPP_TOK_PIPE_EQUAL                   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PIPE_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PIPE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_HAT_EQUAL)
		/* .tff_TPP_TOK_HAT_EQUAL                    = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_HAT_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_HAT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SLASH_SLASH)
		/* .tff_TPP_TOK_SLASH_SLASH                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SLASH_SLASH),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SLASH_SLASH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL)
		/* .tff_TPP_TOK_SLASH_SLASH_EQUAL            = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL)
		/* .tff_TPP_TOK_STAR_STAR_EQUAL              = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_AT_EQUAL)
		/* .tff_TPP_TOK_AT_EQUAL                     = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_AT_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_AT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_POUND_POUND)
		/* .tff_TPP_TOK_POUND_POUND                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_POUND_POUND),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_POUND_POUND) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_AMP_AMP)
		/* .tff_TPP_TOK_AMP_AMP                      = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_AMP_AMP),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_AMP_AMP) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PIPE_PIPE)
		/* .tff_TPP_TOK_PIPE_PIPE                    = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PIPE_PIPE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PIPE_PIPE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_HAT_HAT)
		/* .tff_TPP_TOK_HAT_HAT                      = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_HAT_HAT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_HAT_HAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PLUS_PLUS)
		/* .tff_TPP_TOK_PLUS_PLUS                    = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PLUS_PLUS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_PLUS_PLUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_MINUS)
		/* .tff_TPP_TOK_MINUS_MINUS                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_MINUS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_STAR)
		/* .tff_TPP_TOK_STAR_STAR                    = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_STAR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_TILDE_TILDE)
		/* .tff_TPP_TOK_TILDE_TILDE                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_TILDE_TILDE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_TILDE_TILDE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_TILDE_EQUAL)
		/* .tff_TPP_TOK_TILDE_EQUAL                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_TILDE_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_TILDE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_RANGLE)
		/* .tff_TPP_TOK_MINUS_RANGLE                 = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_COLON_EQUAL)
		/* .tff_TPP_TOK_COLON_EQUAL                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_COLON_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_COLON_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_COLON_COLON)
		/* .tff_TPP_TOK_COLON_COLON                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_COLON_COLON),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_COLON_COLON) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR)
		/* .tff_TPP_TOK_MINUS_RANGLE_STAR            = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_DOT_STAR)
		/* .tff_TPP_TOK_DOT_STAR                     = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_DOT_STAR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_DOT_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_DOT_DOT)
		/* .tff_TPP_TOK_DOT_DOT                      = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_DOT_DOT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_DOT_DOT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_RANGLE)
		/* .tff_TPP_TOK_LANGLE_RANGLE                = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE)
		/* .tff_TPP_TOK_LANGLE_LANGLE_LANGLE         = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE)
		/* .tff_TPP_TOK_RANGLE_RANGLE_RANGLE         = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
		/* .tff_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
		/* .tff_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL)
		/* .tff_TPP_TOK_EQUAL_EQUAL_EQUAL            = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL)
		/* .tff_TPP_TOK_EXCLAIM_EQUAL_EQUAL          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM)
		/* .tff_TPP_TOK_EXCLAIM_EXCLAIM              = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_QMARK_QMARK)
		/* .tff_TPP_TOK_QMARK_QMARK                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_QMARK_QMARK),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_QMARK_QMARK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_QMARK_EQUAL)
		/* .tff_TPP_TOK_QMARK_EQUAL                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_QMARK_EQUAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_QMARK_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_LANGLE)
		/* .tff_TPP_TOK_RANGLE_LANGLE                = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_PLUS)
		/* .tff_TPP_TOK_EQUAL_PLUS                   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_PLUS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_PLUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_MINUS)
		/* .tff_TPP_TOK_EQUAL_MINUS                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_MINUS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_STAR)
		/* .tff_TPP_TOK_EQUAL_STAR                   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_STAR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR)
		/* .tff_TPP_TOK_EQUAL_STAR_STAR              = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_SLASH)
		/* .tff_TPP_TOK_EQUAL_SLASH                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_SLASH),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_SLASH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH)
		/* .tff_TPP_TOK_EQUAL_SLASH_SLASH            = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_PERCENT)
		/* .tff_TPP_TOK_EQUAL_PERCENT                = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_PERCENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_PERCENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_AMP)
		/* .tff_TPP_TOK_EQUAL_AMP                    = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_AMP),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_AMP) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_PIPE)
		/* .tff_TPP_TOK_EQUAL_PIPE                   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_PIPE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_PIPE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_HAT)
		/* .tff_TPP_TOK_EQUAL_HAT                    = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_HAT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_HAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE)
		/* .tff_TPP_TOK_EQUAL_LANGLE                 = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE)
		/* .tff_TPP_TOK_EQUAL_LANGLE_LANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)
		/* .tff_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE)
		/* .tff_TPP_TOK_EQUAL_RANGLE                 = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE)
		/* .tff_TPP_TOK_EQUAL_RANGLE_RANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
		/* .tff_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE   = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_AT)
		/* .tff_TPP_TOK_EQUAL_AT                     = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_AT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_AT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_TILDE)
		/* .tff_TPP_TOK_EQUAL_TILDE                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_TILDE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_TILDE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_COLON)
		/* .tff_TPP_TOK_EQUAL_COLON                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_COLON),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_COLON) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM)
		/* .tff_TPP_TOK_EQUAL_EXCLAIM                = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM)
		/* .tff_TPP_TOK_EQUAL_EQUAL_EXCLAIM          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_QMARK)
		/* .tff_TPP_TOK_EQUAL_QMARK                  = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_QMARK),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_EQUAL_QMARK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_MINUS)
		/* .tff_TPP_TOK_LANGLE_MINUS                 = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_MINUS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS)
		/* .tff_TPP_TOK_STAR_LANGLE_MINUS            = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_DOT)
		/* .tff_TPP_TOK_STAR_DOT                     = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_DOT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_STAR_DOT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_LANGLE)
		/* .tff_TPP_TOK_MINUS_LANGLE                 = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_MINUS_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_MINUS)
		/* .tff_TPP_TOK_RANGLE_MINUS                 = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_MINUS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE)
		/* .tff_TPP_TOK_LANGLE_EQUAL_RANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE)
		/* .tff_TPP_TOK_LANGLE_MINUS_RANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE)
		/* .tff_TPP_TOK_RANGLE_EQUAL_LANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE)
		/* .tff_TPP_TOK_RANGLE_MINUS_LANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE)
		/* .tff_TPP_TOK_LANGLE_EQUAL_LANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE)
		/* .tff_TPP_TOK_LANGLE_MINUS_LANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE)
		/* .tff_TPP_TOK_RANGLE_EQUAL_RANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE)
		/* .tff_TPP_TOK_RANGLE_MINUS_RANGLE          = */ TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BSE)
		/* .tff_BSE                                  = */ TPP_CONF_DEFAULT(TPP_HAVE_BSE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BSE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BSE_WHITESPACE)
		/* .tff_BSE_WHITESPACE                       = */ TPP_CONF_DEFAULT(TPP_HAVE_BSE_WHITESPACE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BSE_WHITESPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
		/* .tff_ESCAPE_IN_IDENTIFIERS                = */ TPP_CONF_DEFAULT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_E_IN_STRINGS)
		/* .tff_ESCAPE_E_IN_STRINGS                  = */ TPP_CONF_DEFAULT(TPP_HAVE_ESCAPE_E_IN_STRINGS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_E_IN_STRINGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_S_IN_STRINGS)
		/* .tff_ESCAPE_S_IN_STRINGS                  = */ TPP_CONF_DEFAULT(TPP_HAVE_ESCAPE_S_IN_STRINGS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_S_IN_STRINGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIRECTIVES)
		/* .tff_CPP_DIRECTIVES                       = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_DIRECTIVES),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIRECTIVES) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_MACROS)
		/* .tff_CPP_MACROS                           = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BUILTIN_MACROS)
		/* .tff_CPP_BUILTIN_MACROS                   = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_BUILTIN_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BUILTIN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EXCLAIM)
		/* .tff_CPP_EXCLAIM                          = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_EXCLAIM),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BLANK)
		/* .tff_CPP_BLANK                            = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_BLANK),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BLANK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIGIT_LINE)
		/* .tff_CPP_DIGIT_LINE                       = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_DIGIT_LINE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIGIT_LINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_LINE)
		/* .tff_CPP_LINE                             = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_LINE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_LINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE)
		/* .tff_CPP_INCLUDE                          = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_INCLUDE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE_NEXT)
		/* .tff_CPP_INCLUDE_NEXT                     = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_INCLUDE_NEXT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE_NEXT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IMPORT)
		/* .tff_CPP_IMPORT                           = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_IMPORT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IMPORT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IF_ELSE_ENDIF)
		/* .tff_CPP_IF_ELSE_ENDIF                    = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_IF_ELSE_ENDIF),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IF_ELSE_ENDIF) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DEFINE)
		/* .tff_CPP_DEFINE                           = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_DEFINE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DEFINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ASSERT)
		/* .tff_CPP_ASSERT                           = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_ASSERT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ASSERT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ERROR)
		/* .tff_CPP_ERROR                            = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_ERROR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ERROR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_WARNING)
		/* .tff_CPP_WARNING                          = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_WARNING),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_WARNING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IDENT_SCCS)
		/* .tff_CPP_IDENT_SCCS                       = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_IDENT_SCCS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IDENT_SCCS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_PRAGMA)
		/* .tff_CPP_PRAGMA                           = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_PRAGMA),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_PRAGMA) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EMBED)
		/* .tff_CPP_EMBED                            = */ TPP_CONF_DEFAULT(TPP_HAVE_CPP_EMBED),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EMBED) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO__Pragma)
		/* .tff_MACRO__Pragma                        = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO__Pragma),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO__Pragma) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___pragma)
		/* .tff_MACRO___pragma                       = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___pragma),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___pragma) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_attribute)
		/* .tff_CLANG_MACRO___has_attribute          = */ TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_attribute),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_builtin)
		/* .tff_CLANG_MACRO___has_builtin            = */ TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_builtin),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_builtin) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute)
		/* .tff_CLANG_MACRO___has_cpp_attribute      = */ TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute)
		/* .tff_CLANG_MACRO___has_declspec_attribute = */ TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_extension)
		/* .tff_CLANG_MACRO___has_extension          = */ TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_extension),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_extension) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_feature)
		/* .tff_CLANG_MACRO___has_feature            = */ TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_feature),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_feature) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_c_attribute)
		/* .tff_CLANG_MACRO___has_c_attribute        = */ TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_c_attribute),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_c_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES)
		/* .tff_CLANG_EXTENSIONS_ARE_FEATURES        = */ TPP_CONF_DEFAULT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_identifier)
		/* .tff_MACRO___is_identifier                = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___is_identifier),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_identifier) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_deprecated)
		/* .tff_MACRO___is_deprecated                = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___is_deprecated),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_deprecated) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_poisoned)
		/* .tff_MACRO___is_poisoned                  = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___is_poisoned),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_poisoned) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_extension)
		/* .tff_MACRO___has_extension                = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_extension),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_extension) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_extension)
		/* .tff_MACRO___has_known_extension          = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_known_extension),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_extension) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_warning)
		/* .tff_MACRO___has_warning                  = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_warning),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_warning) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_warning)
		/* .tff_MACRO___has_known_warning            = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_known_warning),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_warning) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include)
		/* .tff_MACRO___has_include                  = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_include),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include_next)
		/* .tff_MACRO___has_include_next             = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_include_next),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include_next) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_embed)
		/* .tff_MACRO___has_embed                    = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_embed),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_embed) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE__)
		/* .tff_MACRO___FILE__                       = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___FILE__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___LINE__)
		/* .tff_MACRO___LINE__                       = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___LINE__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___LINE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIME__)
		/* .tff_MACRO___TIME__                       = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TIME__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIME__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___DATE__)
		/* .tff_MACRO___DATE__                       = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___DATE__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___DATE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COLUMN__)
		/* .tff_MACRO___COLUMN__                     = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___COLUMN__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COLUMN__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___BASE_FILE__)
		/* .tff_MACRO___BASE_FILE__                  = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___BASE_FILE__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___BASE_FILE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_LEVEL__)
		/* .tff_MACRO___INCLUDE_LEVEL__              = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___INCLUDE_LEVEL__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_LEVEL__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_DEPTH__)
		/* .tff_MACRO___INCLUDE_DEPTH__              = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___INCLUDE_DEPTH__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_DEPTH__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COUNTER__)
		/* .tff_MACRO___COUNTER__                    = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___COUNTER__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COUNTER__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIMESTAMP__)
		/* .tff_MACRO___TIMESTAMP__                  = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TIMESTAMP__),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIMESTAMP__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_DATE_MACROS)
		/* .tff_NUMERIC_DATE_MACROS                  = */ TPP_CONF_DEFAULT(TPP_HAVE_NUMERIC_DATE_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_DATE_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_TIME_MACROS)
		/* .tff_NUMERIC_TIME_MACROS                  = */ TPP_CONF_DEFAULT(TPP_HAVE_NUMERIC_TIME_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_TIME_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EVAL)
		/* .tff_MACRO___TPP_EVAL                     = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_EVAL),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EVAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_UNIQUE)
		/* .tff_MACRO___TPP_UNIQUE                   = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_UNIQUE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_UNIQUE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_LOAD_FILE)
		/* .tff_MACRO___TPP_LOAD_FILE                = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_LOAD_FILE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_LOAD_FILE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNTER)
		/* .tff_MACRO___TPP_COUNTER                  = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_COUNTER),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNTER) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_RANDOM)
		/* .tff_MACRO___TPP_RANDOM                   = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_RANDOM),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_RANDOM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE)
		/* .tff_MACRO___TPP_STR_DECOMPILE            = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SUBSTR)
		/* .tff_MACRO___TPP_STR_SUBSTR               = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_SUBSTR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SUBSTR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_PACK)
		/* .tff_MACRO___TPP_STR_PACK                 = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_PACK),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_PACK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SIZE)
		/* .tff_MACRO___TPP_STR_SIZE                 = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_SIZE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SIZE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS)
		/* .tff_MACRO___TPP_COUNT_TOKENS             = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_IDENTIFIER)
		/* .tff_MACRO___TPP_IDENTIFIER               = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_IDENTIFIER),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_IDENTIFIER) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS)
		/* .tff_ALTERNATIVE_MACRO_PARENTHESIS        = */ TPP_CONF_DEFAULT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
		/* .tff_MACRO_ARGUMENT_WHITESPACE            = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_RECURSION)
		/* .tff_MACRO_RECURSION                      = */ TPP_CONF_DEFAULT(TPP_HAVE_MACRO_RECURSION),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_RECURSION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TRADITIONAL_MACROS)
		/* .tff_TRADITIONAL_MACROS                   = */ TPP_CONF_DEFAULT(TPP_HAVE_TRADITIONAL_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TRADITIONAL_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_NAMED_VARARGS_IN_MACROS)
		/* .tff_NAMED_VARARGS_IN_MACROS              = */ TPP_CONF_DEFAULT(TPP_HAVE_NAMED_VARARGS_IN_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_NAMED_VARARGS_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_ARGS_IN_MACROS)
		/* .tff_VA_ARGS_IN_MACROS                    = */ TPP_CONF_DEFAULT(TPP_HAVE_VA_ARGS_IN_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_ARGS_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_COMMA_IN_MACROS)
		/* .tff_VA_COMMA_IN_MACROS                   = */ TPP_CONF_DEFAULT(TPP_HAVE_VA_COMMA_IN_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_COMMA_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_OPT_IN_MACROS)
		/* .tff_VA_OPT_IN_MACROS                     = */ TPP_CONF_DEFAULT(TPP_HAVE_VA_OPT_IN_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_OPT_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_NARGS_IN_MACROS)
		/* .tff_VA_NARGS_IN_MACROS                   = */ TPP_CONF_DEFAULT(TPP_HAVE_VA_NARGS_IN_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_NARGS_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS)
		/* .tff_VA_GLUE_COMMA_IN_MACROS              = */ TPP_CONF_DEFAULT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT)
		/* .tff_STRINGIZE_MACRO_ARGUMENT             = */ TPP_CONF_DEFAULT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT)
		/* .tff_CHARIZE_MACRO_ARGUMENT               = */ TPP_CONF_DEFAULT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT)
		/* .tff_DONT_EXPAND_MACRO_ARGUMENT           = */ TPP_CONF_DEFAULT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_GLUE_MACRO_ARGUMENT)
		/* .tff_GLUE_MACRO_ARGUMENT                  = */ TPP_CONF_DEFAULT(TPP_HAVE_GLUE_MACRO_ARGUMENT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_GLUE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_PUSH_MACRO)
		/* .tff_PRAGMA_PUSH_MACRO                    = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_PUSH_MACRO),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_PUSH_MACRO) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ONCE)
		/* .tff_PRAGMA_ONCE                          = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_ONCE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ONCE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_DEPRECATED)
		/* .tff_PRAGMA_DEPRECATED                    = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_DEPRECATED),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_DEPRECATED) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_EXTENSION)
		/* .tff_PRAGMA_EXTENSION                     = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_EXTENSION),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_EXTENSION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_WARNING)
		/* .tff_PRAGMA_WARNING                       = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_WARNING),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_WARNING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE)
		/* .tff_PRAGMA_MESSAGE                       = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_MESSAGE),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ERROR)
		/* .tff_PRAGMA_ERROR                         = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_ERROR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ERROR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_REGION)
		/* .tff_PRAGMA_REGION                        = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_REGION),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_REGION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_EXEC)
		/* .tff_PRAGMA_TPP_EXEC                      = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_TPP_EXEC),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_EXEC) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS)
		/* .tff_PRAGMA_TPP_SET_KEYWORD_FLAGS         = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_POISON)
		/* .tff_PRAGMA_GCC_POISON                    = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_POISON),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_POISON) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_WARNING)
		/* .tff_PRAGMA_GCC_WARNING                   = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_WARNING),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_WARNING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_ERROR)
		/* .tff_PRAGMA_GCC_ERROR                     = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_ERROR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_ERROR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER)
		/* .tff_PRAGMA_GCC_SYSTEM_HEADER             = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC)
		/* .tff_PRAGMA_GCC_DIAGNOSTIC                = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
		/* .tff_PRAGMA_GCC_DEPENDENCY                = */ TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_DEFINED)
		/* .tff_BUILTIN_EXPR_DEFINED                 = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_DEFINED),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_DEFINED) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR)
		/* .tff_DONT_EXPAND_DEFINED_IN_EXPR          = */ TPP_CONF_DEFAULT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_STRINGS)
		/* .tff_BUILTIN_EXPR_STRINGS                 = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_STRINGS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_STRINGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FLOATS)
		/* .tff_BUILTIN_EXPR_FLOATS                  = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_FLOATS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FLOATS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)
		/* .tff_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT     = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS)
		/* .tff_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS  = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR)
		/* .tff_BUILTIN_EXPR_LOGICAL_XOR             = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS)
		/* .tff_BUILTIN_EXPR_BINARY_LITERALS         = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS)
		/* .tff_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS    = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS)
		/* .tff_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS  = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS)
		/* .tff_BUILTIN_EXPR_CHARACTER_LITERALS      = */ TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS),
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS) */
	}
};
#endif /* TPP_HAVE_FEATURES */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/warnings.c                                               */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_WARNINGS

#if TPP_HAVE_WARNING_SUPPRESS
#define _tpp_warnings_fini_common(self) \
	tpp_warning_suppressions_fini(&(self)->tw_suppressions)
#else /* TPP_HAVE_WARNING_SUPPRESS */
#define _tpp_warnings_fini_common(self) (void)0
#endif /* !TPP_HAVE_WARNING_SUPPRESS */

/* Finalize a given warnings context "self" */
#if TPP_HAVE_WARNINGS_FINI

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_fini(tpp_warnings *tpp_restrict self) {
	/* Finalize common data */
	_tpp_warnings_fini_common(self);

	/* Finalized pushed warning states */
#if TPP_HAVE_WARNINGS_PUSH_POP
	{
		tpp_warnings *prev = self->tw_prev;
		while (prev) {
			tpp_warnings *pprev = prev->tw_prev;
			_tpp_warnings_fini_common(prev);
			tpp_free(prev);
			prev = pprev;
		}
	}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */
}
#endif /* TPP_HAVE_WARNINGS_FINI */


#if TPP_HAVE_WARNINGS_PUSH_POP
/* Pop the current warnings state (may only be called when `tpp_warnings_canpop(self)') */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_pop(tpp_warnings *tpp_restrict self) {
	tpp_assert(tpp_warnings_canpop(self));
	if (self->tw_pushcnt == 0) {
		tpp_warnings *prev = self->tw_prev;
		_tpp_warnings_fini_common(self);
		tpp_memcpy(self, prev, sizeof(tpp_warnings));
		tpp_free(prev);
		tpp_assert(self->tw_pushcnt != 0);
	}
	--self->tw_pushcnt;
}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */

/* Return the state of "ctx_id". The caller is
 * responsible to ensure that "ctx_id" is valid. */
#if TPP_HAVE_WARNING_SUPPRESS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_state TPPCALL
tpp_warnings_getctx(tpp_warnings const *tpp_restrict self,
                    tpp_warning_context_id ctx_id) {
	tpp_warning_state result;
	tpp_assert((unsigned int)ctx_id < (unsigned int)TPP_WC_COUNT);
	result = tpp_warnings_state_get(&self->tw_state, ctx_id);
	if (result == TPP_WSTATE_FATAL) {
		/* Check if this warning is currently being suppressed. */
		tpp_size lo = 0, hi = self->tw_suppressions.tws_ctxc;
		while (lo < hi) {
			tpp_warning_suppress_item const *it;
			tpp_size const mid = (lo + hi) / 2;
			it = &self->tw_suppressions.tws_ctxv[mid];
			if ((unsigned int)ctx_id < (unsigned int)it->twsi_ctx_id) {
				hi = mid;
			} else if ((unsigned int)ctx_id > (unsigned int)it->twsi_ctx_id) {
				lo = mid + 1;
			} else {
				tpp_assert(it->twsi_count != 0);
				result = TPP_WSTATE_SUPPRESS;
				break;
			}
		}
	}
	return result;
}
#endif /* TPP_HAVE_WARNING_SUPPRESS */

#if TPP_HAVE_WARNINGS_PUSH_POP
/* Create a heap-copy of "self".
 * @return: * :   The newly allocated copy
 * @return: NULL: Out of memory. */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_warnings *TPPCALL
tpp_warnings_copy(tpp_warnings const *tpp_restrict self) {
	tpp_warnings *result = (tpp_warnings *)tpp_malloc(sizeof(tpp_warnings));
	if tpp_unlikely(!result)
		goto err;

	/* Duplicate suppressions */
#if TPP_HAVE_WARNING_SUPPRESS
	if (self->tw_suppressions.tws_ctxc) {
		tpp_warning_suppress_item *suppress_copy;
		suppress_copy = (tpp_warning_suppress_item *)tpp_malloc(self->tw_suppressions.tws_ctxc *
		                                                        sizeof(tpp_warning_suppress_item));
		if tpp_unlikely(!suppress_copy)
			goto err_r;
		tpp_memcpy(suppress_copy, self->tw_suppressions.tws_ctxv,
		           self->tw_suppressions.tws_ctxc *
		           sizeof(tpp_warning_suppress_item));
		result->tw_suppressions.tws_ctxv = suppress_copy;
		result->tw_suppressions.tws_ctxc = self->tw_suppressions.tws_ctxc;
		result->tw_suppressions.tws_ctxa = self->tw_suppressions.tws_ctxc;
	} else {
		tpp_warning_suppressions_init(&result->tw_suppressions);
	}
#endif /* TPP_HAVE_WARNING_SUPPRESS */

	/* Duplicate remaining data fields... */
	result->tw_state = self->tw_state;
#if TPP_HAVE_WARNINGS_PUSH_POP
	result->tw_prev    = self->tw_prev;
	result->tw_pushcnt = self->tw_pushcnt;
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */
	return result;
#if TPP_HAVE_WARNING_SUPPRESS
err_r:
	tpp_free(result);
#endif /* TPP_HAVE_WARNING_SUPPRESS */
err:
	return NULL;
}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */


#if TPP_HAVE_WARNING_SUPPRESS
/* Ensure that an entry for "ctx_id" is allocated in `&self->tw_suppressions'
 * @return: * :   The suppression entry for "ctx_id"
 * @return: NULL: Out of memory. */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_suppress_item *TPPCALL
tpp_warnings_require_suppress(tpp_warnings *tpp_restrict self,
                              tpp_warning_context_id ctx_id) {
	tpp_size lo = 0, hi = self->tw_suppressions.tws_ctxc;
	tpp_warning_suppress_item *result;
	while (lo < hi) {
		tpp_size mid = (lo + hi) / 2;
		result = &self->tw_suppressions.tws_ctxv[mid];
		if (ctx_id < result->twsi_ctx_id) {
			hi = mid;
		} else if (ctx_id > result->twsi_ctx_id) {
			lo = mid + 1;
		} else {
			/* Found existing entry! */
			return result;
		}
	}
	tpp_assert(lo == hi);
	tpp_assert(self->tw_suppressions.tws_ctxc <= self->tw_suppressions.tws_ctxa);
	if (self->tw_suppressions.tws_ctxc >= self->tw_suppressions.tws_ctxa) {
		/* Must allocate more space. */
		tpp_warning_suppress_item *new_vec;
		tpp_size new_alloc = (self->tw_suppressions.tws_ctxa * 3) / 2;
		if (new_alloc < 8)
			new_alloc = 8;
		if (new_alloc < self->tw_suppressions.tws_ctxc + 1)
			new_alloc = self->tw_suppressions.tws_ctxc + 1;
		new_vec = (tpp_warning_suppress_item *)tpp_tryrealloc(self->tw_suppressions.tws_ctxv,
		                                                      new_alloc *
		                                                      sizeof(tpp_warning_suppress_item));
		if tpp_unlikely(!new_vec) {
			new_alloc = self->tw_suppressions.tws_ctxc + 1;
			new_vec = (tpp_warning_suppress_item *)tpp_realloc(self->tw_suppressions.tws_ctxv,
			                                                   new_alloc *
			                                                   sizeof(tpp_warning_suppress_item));
			if tpp_unlikely(!new_vec)
				goto err_nomem;
		}
		self->tw_suppressions.tws_ctxv = new_vec;
		self->tw_suppressions.tws_ctxa = new_alloc;
	}
	tpp_assert(self->tw_suppressions.tws_ctxc < self->tw_suppressions.tws_ctxa);
	tpp_memmoveup(&self->tw_suppressions.tws_ctxv[lo + 1],
	              &self->tw_suppressions.tws_ctxv[lo],
	              (self->tw_suppressions.tws_ctxa - lo) *
	              sizeof(tpp_warning_suppress_item));
	result = &self->tw_suppressions.tws_ctxv[lo];
	result->twsi_count  = 0;
	result->twsi_ctx_id = ctx_id;
	return result;
err_nomem:
	return NULL;
}
#endif /* TPP_HAVE_WARNING_SUPPRESS */


/* Set the state of "ctx_id" to "state".
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HAVE_WARNINGS_SETCTX_MAYFAIL
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_warnings_setctx(tpp_warnings *tpp_restrict self,
                    tpp_warning_context_id ctx_id,
                    tpp_warning_state state)
#else /* TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_setctx_(tpp_warnings *tpp_restrict self,
                     tpp_warning_context_id ctx_id,
                     tpp_warning_state state)
#endif /* !TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */
{
	tpp_assert((unsigned int)ctx_id < (unsigned int)TPP_WC_COUNT);

	/* Deal with `TPP_WSTATE_DEFAULT' */
#if TPP_HAVE_WARNING_DEFAULT
	if (state == TPP_WSTATE_DEFAULT)
		state = tpp_warnings_state_get(&tpp_warnings_state_default, ctx_id);
#endif /* TPP_HAVE_WARNING_DEFAULT */

	/* Duplicate the warnings state if it was pushed. */
#if TPP_HAVE_WARNINGS_PUSH_POP
	if (tpp_warnings_mustcopy(self)) {
		tpp_warnings *copy;
#ifndef __OPTIMIZE_SIZE__
		tpp_warning_state old_state;
		old_state = tpp_warnings_state_get(&self->tw_state, ctx_id);
		if (state == old_state)
			return TPP_EOK;
#endif /* !__OPTIMIZE_SIZE__ */
		copy = tpp_warnings_copy(self);
		if tpp_unlikely(!copy)
			return TPP_ENOMEM;
		self->tw_prev = copy;
		self->tw_pushcnt = 0;
	}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */

	/* Deal with `TPP_WSTATE_SUPPRESS' */
#if TPP_HAVE_WARNING_SUPPRESS
	if (state == TPP_WSTATE_SUPPRESS) {
		tpp_warning_suppress_item *item;
		item = tpp_warnings_require_suppress(self, ctx_id);
		if tpp_unlikely(!item)
			return TPP_ENOMEM;
		tpp_assert(item->twsi_ctx_id == ctx_id);
		if (item->twsi_count == 0) {
			item->twsi_restore = tpp_warnings_state_get(&self->tw_state, ctx_id);
			tpp_warnings_state_set(&self->tw_state, ctx_id, TPP_WSTATE_FATAL);
		} else {
			tpp_assert(tpp_warnings_state_get(&self->tw_state, ctx_id) == TPP_WSTATE_FATAL);
		}
		++item->twsi_count;
	} else
#endif /* TPP_HAVE_WARNING_SUPPRESS */
	{
		/* Regular case: set warning state in state-bitset. */
		tpp_assert(state == TPP_WSTATE_DISABLED ||
		           state == TPP_WSTATE_WARN ||
		           state == TPP_WSTATE_ERROR_OR_FATAL ||
		           state == TPP_WSTATE_FATAL);
		tpp_warnings_state_set(&self->tw_state, ctx_id, state);
	}

#if TPP_HAVE_WARNINGS_SETCTX_MAYFAIL
	return TPP_EOK;
#endif /* TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */
}



/* Invoke "warning_id" (updating suppression counters if necessary) and
 * returning information about the context/state with which the warning
 * should be processed.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (only when "TPP_HAVE_WARNINGS_INVOKE_MAYFAIL") */
#if TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_warnings_invoke(tpp_warnings *tpp_restrict self, tpp_warning_id warning_id,
                    tpp_warning_invokeinfo *tpp_restrict result)
#else /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */
TPP_IMPL TPP_NONNULL((1, 3)) void TPPCALL
tpp_warnings_invoke_(tpp_warnings const *tpp_restrict self, tpp_warning_id warning_id,
                     tpp_warning_invokeinfo *tpp_restrict result)
#endif /* !TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */
{
	tpp_size i;
	tpp_warning_group_id const *groups;
#if TPP_HAVE_WARNING_NUMBERS
	tpp_warning_context_id number_context_id;
#endif /* TPP_HAVE_WARNING_NUMBERS */
	tpp_assert((unsigned int)warning_id < (unsigned int)TPP_W_COUNT);
	groups = tpp_warning_getgroups(warning_id);

	result->twii_ctx_id = TPP_WC_COUNT;
	result->twii_state  = TPP_WSTATE_WARN; /* Default behavior for warnings without context IDs */
	for (i = 0; (unsigned int)groups[i] < (unsigned int)TPP_WG_COUNT; ++i) {
		tpp_warning_context_id ctx_id = tpp_warning_context_id_ofgroup(groups[i]);
		tpp_warning_state state = tpp_warnings_getctx(self, ctx_id);
		if (result->twii_ctx_id == TPP_WC_COUNT ||
		    tpp_warning_state_ismoreimportant(state, result->twii_state)) {
			/* Use this context instead! */
			result->twii_ctx_id = ctx_id;
			result->twii_state  = state;
		}
	}

#if TPP_HAVE_WARNING_NUMBERS
	/* Probe warning number context */
	number_context_id = tpp_warning_context_id_ofwarning(warning_id);
	if ((unsigned int)number_context_id < (unsigned int)TPP_WC_COUNT) {
		tpp_warning_state state = tpp_warnings_getctx(self, number_context_id);
		if (result->twii_ctx_id == TPP_WC_COUNT ||
		    tpp_warning_state_ismoreimportant(state, result->twii_state)) {
			/* Use this context instead! */
			result->twii_ctx_id = number_context_id;
			result->twii_state  = state;
		}
	}
#endif /* TPP_HAVE_WARNING_NUMBERS */

#if TPP_HAVE_WARNING_SUPPRESS
	if (result->twii_state == TPP_WSTATE_SUPPRESS) {
		tpp_size lo, hi, mid;
		tpp_warning_suppress_item *item;
		tpp_warning_context_id ctx_id = result->twii_ctx_id;

		/* Deal with special case: suppression. In this case:
		 * - must (potentially) copy "self"
		 * - must decrement the suppression-counter of the relevant context
		 * - if the suppression-counter hits "0", restore the warnings
		 *   previous state, then remove the suppression entry. */
#if TPP_HAVE_WARNINGS_PUSH_POP
		if (tpp_warnings_mustcopy(self)) {
			tpp_warnings *copy;
			copy = tpp_warnings_copy(self);
			if tpp_unlikely(!copy)
				return TPP_ENOMEM;
			self->tw_prev = copy;
			self->tw_pushcnt = 0;
		}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */
		lo = 0;
		hi = self->tw_suppressions.tws_ctxc;
		for (;;) {
			mid = (lo + hi) / 2;
			tpp_assert(lo < hi);
			item = &self->tw_suppressions.tws_ctxv[mid];
			if (ctx_id < item->twsi_ctx_id) {
				hi = mid;
			} else if (ctx_id > item->twsi_ctx_id) {
				lo = mid + 1;
			} else {
				break; /* Found entry! */
			}
		}
		tpp_assert(item->twsi_count != 0);
		tpp_assert(item->twsi_ctx_id == ctx_id);
		--item->twsi_count;
		if (item->twsi_count == 0) {
			/* Restore old warning state */
			tpp_warning_state restore = item->twsi_restore;
			tpp_assert(restore == TPP_WSTATE_DISABLED ||
			           restore == TPP_WSTATE_WARN ||
			           restore == TPP_WSTATE_ERROR_OR_FATAL ||
			           restore == TPP_WSTATE_FATAL);
			tpp_warnings_state_set(&self->tw_state, ctx_id, restore);

			/* Delete the suppression entry */
			--self->tw_suppressions.tws_ctxc;
			tpp_memmovedown(item, item + 1,
			                (self->tw_suppressions.tws_ctxc - mid) *
			                sizeof(tpp_warning_suppress_item));
		}
	}
#endif /* TPP_HAVE_WARNING_SUPPRESS */

#if TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
	return TPP_EOK;
#endif /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */
}

#endif /* TPP_HAVE_WARNINGS */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer.c                                                  */
/************************************************************************/
TPP_DECL_BEGIN

/* Assert that config macros work */
TPP_STATIC_ASSERT(TPP_CONF_1 && TPP_CONF_MAYBE_1(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_0 && !TPP_CONF_MAYBE_1(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_EXT1 && TPP_CONF_MAYBE_1(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(TPP_CONF_EXT0 && TPP_CONF_MAYBE_1(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_FEAT1 && TPP_CONF_MAYBE_1(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(TPP_CONF_FEAT0 && TPP_CONF_MAYBE_1(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(!TPP_CONF_MAYBE_0(TPP_CONF_1));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_0));
TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_IS_FEAT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_FEAT(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_IS_EXT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_EXT(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(TPP_CONF_IS_CONST(TPP_CONF_1));
TPP_STATIC_ASSERT(TPP_CONF_IS_CONST(TPP_CONF_0));
TPP_STATIC_ASSERT(!TPP_CONF_IS_CONST(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_CONST(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(!TPP_CONF_IS_CONST(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_CONST(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(!TPP_CONF_IS_RT(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_RT(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_IS_RT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_RT(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_IS_RT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_RT(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(TPP_CONF_DEFAULT(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_DEFAULT(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_DEFAULT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(!TPP_CONF_DEFAULT(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_DEFAULT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(!TPP_CONF_DEFAULT(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(TPP_CONF_MAKEFEAT(0) == TPP_CONF_FEAT0);
TPP_STATIC_ASSERT(TPP_CONF_MAKEFEAT(1) == TPP_CONF_FEAT1);
TPP_STATIC_ASSERT(TPP_CONF_MAKEEXT(0) == TPP_CONF_EXT0);
TPP_STATIC_ASSERT(TPP_CONF_MAKEEXT(1) == TPP_CONF_EXT1);


/* Assert that token <=> error conversion works */
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_ENOMEM));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_ENOMEM) == TPP_ENOMEM);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_ENOMEM) == TPP_ENOMEM);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_ENOMEM) == TPP_TOK_ENOMEM);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_ENOMEM) == TPP_TOK_ENOMEM);
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_EIO));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_EIO) == TPP_EIO);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_EIO) == TPP_EIO);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_EIO) == TPP_TOK_EIO);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_EIO) == TPP_TOK_EIO);
#if TPP_HAVE_FILE_NONBLOCK
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_EWOULDBLOCK));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_EWOULDBLOCK) == TPP_EWOULDBLOCK);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_EWOULDBLOCK) == TPP_EWOULDBLOCK);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_EWOULDBLOCK) == TPP_TOK_EWOULDBLOCK);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_EWOULDBLOCK) == TPP_TOK_EWOULDBLOCK);
#endif /* TPP_HAVE_FILE_NONBLOCK */
#if TPP_HAVE_WARNINGS
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_ELEXERROR));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_ELEXERROR) == TPP_ELEXERROR);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_ELEXERROR) == TPP_ELEXERROR);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_ELEXERROR) == TPP_TOK_ELEXERROR);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_ELEXERROR) == TPP_TOK_ELEXERROR);
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_EWARNPRINT));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_EWARNPRINT) == TPP_EWARNPRINT);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_EWARNPRINT) == TPP_EWARNPRINT);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_EWARNPRINT) == TPP_TOK_EWARNPRINT);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_EWARNPRINT) == TPP_TOK_EWARNPRINT);
#endif /* TPP_HAVE_WARNINGS */
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_EOK) == TPP_TOK_EOF);

TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_EOF) == TPP_EOK);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_SPACE) == TPP_EOK);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_AMP) == TPP_EOK);

TPP_STATIC_ASSERT(!TPP_TOK_ISCHAR(TPP_TOK_ENOMEM));
TPP_STATIC_ASSERT(!TPP_TOK_ISCHAR(TPP_TOK_EIO));
TPP_STATIC_ASSERT(TPP_TOK_ISCHAR(TPP_TOK_EOF));
TPP_STATIC_ASSERT(TPP_TOK_ISCHAR(TPP_TOK_AMP));
TPP_STATIC_ASSERT(!TPP_TOK_ISCHAR(TPP_INTERNAL(TPP_TOK_MULTICHAR_BEGIN) + 1));

TPP_STATIC_ASSERT(TPP_TOK_OFCHAR('~') == TPP_TOK_TILDE);
TPP_STATIC_ASSERT(TPP_TOK_OFCHAR('&') == TPP_TOK_AMP);


/* Make sure that offsets within "tpp_lexer" are properly aligned such that
 * the tail end of "tpp_token" correctly overlaps with the start of "tpp_file" */
TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer, tl_core.tlc_tok.tt_start) ==
                  tpp_offsetof(tpp_lexer, tl_core.tlc_input.tli_file.tf_tpos));
TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer, tl_core.tlc_tok.tt_end) ==
                  tpp_offsetof(tpp_lexer, tl_core.tlc_input.tli_file.tf_pos));
TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer, tl_core.tlc_tok.tt_chunk) ==
                  tpp_offsetof(tpp_lexer, tl_core.tlc_input.tli_file.tf_chunk));

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_fini_common(tpp_lexer *tpp_restrict self) {
	/* Finalize keywords */
	tpp_keywords_fini(&self->tl_kwds);

	/* Finalize extension states */
#if TPP_HAVE_EXTENSIONS
	tpp_extensions_fini(&self->tl_exts);
#endif /* TPP_HAVE_EXTENSIONS */

	/* Finalize feature states */
#if TPP_HAVE_FEATURES
	tpp_features_fini(&self->tl_feat);
#endif /* TPP_HAVE_FEATURES */

	/* Finalize warning states */
#if TPP_HAVE_WARNINGS
	tpp_warnings_fini(&self->tl_warn);
#endif /* TPP_HAVE_WARNINGS */
}

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_fini(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);

	/* Finalize whatever still remains of the #include-stack */
#if TPP_HAVE_INCLUDE_STACK
	tpp_file *iter;
	for (iter = file->tf_prev; iter;) {
		tpp_file *next = iter->tf_prev;
		tpp_file_fini(iter);
		tpp_file_free(iter);
		iter = next;
	}
#endif /* TPP_HAVE_INCLUDE_STACK */

	/* Finalize the top-most file of the #include-stack */
	tpp_file_fini(file);

	/* Finalize common data... */
	_tpp_lexer_fini_common(self);
}


/* Initialize a lexer that simply reads the given [text,text+text_size) blob.
 * @param: start_lc: [valid_if(chunk != NULL)] */
#if TPP_HAVE_UNICODE
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ex(tpp_lexer *tpp_restrict self,
                       /*utf-8*/ char const *filename,
                       /*inherit(always)*/ TPP_REF tpp_string *chunk,
                       void const *text, tpp_size text_size,
                       tpp_lcinfo start_lc, tpp_file_encoding encoding)
#else /* TPP_HAVE_UNICODE */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ascii(tpp_lexer *tpp_restrict self,
                          /*utf-8*/ char const *filename,
                          /*inherit(always)*/ TPP_REF tpp_string *chunk,
                          void const *text, tpp_size text_size,
                          tpp_lcinfo start_lc)
#endif /* !TPP_HAVE_UNICODE */
{
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_init_text_ex(file, filename, chunk, text, text_size, start_lc, encoding);
	_tpp_lexer_init_common(self);
}


#if TPP_HAVE_LEXER_INIT_IO
/* Initialize a lexer such that it starts reading from "handle"
 * @param: filename: [0..1] Filename to use for messages (s.a. `tpp_file_filename()')
 *                          WARNING: This filename is *NOT* copied -- it must remain
 *                                   allocated and valid until "self" is finalized.
 * @param: handle:   The I/O handle to read from in order to retrieve text data.
 * @param: ioflags:  Extra flags specifying how to interact with "handle":
 *                   - TPP_FILE_IOFLAGS_NONBLOCK: Do non-blocking reads (useful in case "handle" is a pipe)
 *                   - TPP_FILE_IOFLAGS_NOCLOSE:  A later call to `tpp_lexer_fini()' will not close "handle"
 *                   - TPP_FILE_IOFLAGS_SYSHDR:   Do not emit warnings */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                     tpp_io_handle handle, tpp_file_ioflags ioflags) {
	tpp_file *const file = tpp_lexer_getfile(self);
	/* It can never be a keyword, since the lexer is only being
	 * initialized right now (and the keyword would have had to
	 * be allocated by the lexer) */
	ioflags |= TPP_FILE_IOFLAGS_NOKWD;
	tpp_file_init_io_ex(file, filename, handle, ioflags);
	_tpp_lexer_init_common(self);
}
#endif /* TPP_HAVE_LEXER_INIT_IO */



#if TPP_HAVE_LEXER_INIT_FILENAME
/* Initialize a lexer such that it starts reading from "filename"
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_init_filename(tpp_lexer *tpp_restrict self,
                        /*utf-8*/ char const *tpp_restrict filename) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	_tpp_lexer_init_common(self);
	error = tpp_keywords_openfile(&self->tl_kwds, NULL, filename, file);
	if tpp_unlikely(TPP_ISERR(error))
		_tpp_lexer_fini_common(self);
	return error;
}
#endif /* TPP_HAVE_LEXER_INIT_FILENAME */


#if TPP_HAVE_INCLUDE_STACK
#if TPP_HAVE_LEXER_INIT_IO
/* Push another file onto the #include-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @param: filename: [0..1] Filename to use for messages (s.a. `tpp_file_filename()')
 *                          WARNING: This filename is *NOT* copied -- it must remain
 *                                   allocated and valid until "self" is finalized.
 * @param: handle:   The I/O handle to read from in order to retrieve text data.
 * @param: ioflags:  Extra flags specifying how to interact with "handle":
 *                   - TPP_FILE_IOFLAGS_NONBLOCK: Do non-blocking reads (useful in case "handle" is a pipe)
 *                   - TPP_FILE_IOFLAGS_NOCLOSE:  A later call to `tpp_lexer_fini()' will not close "handle"
 *                   - TPP_FILE_IOFLAGS_SYSHDR:   Do not emit warnings
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_pushfile_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                         tpp_io_handle handle, tpp_file_ioflags ioflags) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		return TPP_ENOMEM;
	*prev_file = *file;
	ioflags |= TPP_FILE_IOFLAGS_NOKWD;
	tpp_file_init_io_ex(file, filename, handle, ioflags);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_INIT_IO */

#if TPP_HAVE_LEXER_INIT_FILENAME
/* Push another file onto the #include-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_pushfile_filename(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *tpp_restrict filename) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		return TPP_ENOMEM;
	*prev_file = *file;
	error = tpp_keywords_openfile(&self->tl_kwds, NULL, filename, file);
	if tpp_likely(!TPP_ISERR(error)) {
		file->tf_prev  = prev_file;
		file->tf_tprev = prev_file;
	} else {
		*file = *prev_file;
		tpp_file_free(prev_file);
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_INIT_FILENAME */

/* Check if the current file can be popped. */
#define tpp_lexer_canpopfile(self) \
	(tpp_lexer_getfile(self)->TPP_INTERNAL(tf_prev) != NULL)

/* Pop the current file off the #include-stack.
 * The caller is responsible to ensure that "tpp_lexer_canpopfile(self) == true"
 * After a call to this function, the caller is responsible to yield the next token!
 * WARNING: It is the caller's responsibility to call "tpp_lexer_manualpopfile_popfile()"
 *          instead of this function if rollback of the pop should be possible.
 * NOTE: It is recommended to call "tpp_lexer_warn_nonempty_ifdef()" before calling
 *       this function in order to warn about unterminated #ifdef-blocks. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_popfile(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev = file->tf_prev;
	tpp_file_fini(file);
	*file = *prev;
	tpp_file_free(prev);
}

#endif /* TPP_HAVE_INCLUDE_STACK */


#if TPP_HAVE_LEXER_REPRTOKENID
/* Return the (canonical) string-representation of a given token ID */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) char const *TPPCALL
tpp_lexer_reprtokenid(tpp_lexer const *tpp_restrict self, tpp_token_id tok) {
	char const *result = tpp_reprtokenid(tok);
	if (result)
		return result;
	if (TPP_TOK_ISUSERKEYWORD(tok)) {
		/* Lookup keyword */
		tpp_keyword const *kwd = tpp_keywords_getkeyword_byid(&self->tl_kwds, tok);
		if (kwd)
			return (char const *)kwd->tk_kwd;
	}
	return NULL;
}
#endif /* TPP_HAVE_LEXER_REPRTOKENID */

#if TPP_HAVE_LEXER_MANUALPOPFILE

/* Example of the data-layout (also demonstrating how data is backed up)
 * when using the `tpp_lexer_manualpopfile_*()' set of functions:
 *
 * Input:
 * >> #define foo(a, b) a+b
 * >> #define bar       foo
 * >> #define baz       bar
 * >> #define exo       baz
 * >> 
 * >> bot:  exo[10, 20]
 *
 * Data-layout:
 * >> tpp_lexer_manualpopfile_start(self):
 *    tpp_lexer_getfile    ="bar"->tf_prev=@baz="baz"
 *    _tlmpf_orig_prev @baz="baz"->tf_prev=@exo="exo"
 *                     @exo="exo"->tf_prev=@bot="bot"
 *                     @bot="bot"->tf_prev=NULL
 *
 * >> tpp_lexer_manualpopfile_popfile(self):            // 1st
 *    tpp_lexer_getfile    ="baz"->tf_prev=@exo="exo"
 *    _tlmpf_orig_prev @baz="bar"->tf_prev=@exo="exo"
 *                     @exo="exo"->tf_prev=@bot="bot"
 *                     @bot="bot"->tf_prev=NULL
 *
 * >> tpp_lexer_manualpopfile_popfile(self):            // 2nd
 *    tpp_lexer_getfile    ="exo"->tf_prev=@bot="bot"
 *    _tlmpf_orig_prev @baz="bar"->tf_prev=@exo="baz"
 *                     @exo="baz"->tf_prev=@bot="bot"
 *                     @bot="bot"->tf_prev=NULL
 *
 * >> tpp_lexer_manualpopfile_popfile(self):            // 3rd
 *    tpp_lexer_getfile    ="bot"->tf_prev=NULL
 *    _tlmpf_orig_prev @baz="bar"->tf_prev=@exo="baz"
 *                     @exo="baz"->tf_prev=@bot="exo"
 *                     @bot="exo"->tf_prev=NULL
 */

TPP_INLINE TPP_NONNULL((1, 2)) void TPPCALL
tpp_swapmem(void *a, void *b, tpp_size num_bytes) {
	unsigned char *ca = (unsigned char *)a;
	unsigned char *cb = (unsigned char *)b;
	for (; num_bytes; --num_bytes, ++ca, ++cb) {
		unsigned char temp;
		temp = *ca;
		*ca  = *cb;
		*cb  = temp;
	}
}

TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_manualpopfile_popfile(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev = file->TPP_INTERNAL(tf_prev);
	tpp_assert(prev != NULL && "Nowhere to pop to (caller didn't check `tpp_lexer_manualpopfile_canpopfile()')");
	file->tf_prev = prev->tf_prev;
	tpp_swapmem(file, prev, sizeof(tpp_file)); /* NOTE: This could skip "tf_prev", since that's equal in both */
}

TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_manualpopfile_break_rollback(tpp_lexer *tpp_restrict self,
                                        tpp_file *tpp_restrict orig_prev) {
	tpp_file *const file = tpp_lexer_getfile(self);
	while (file->tf_prev != orig_prev) {
		tpp_file *last = orig_prev;
		tpp_file *last_prev;
		tpp_assert(last);
		while (last->tf_prev != file->tf_prev) {
			tpp_assert(last->tf_prev);
			last = last->tf_prev;
		}
		/* "tf_prev" shouldn't be swapped, so backup */
		last_prev = last->tf_prev;
		/* This could skip "tf_prev"; then wouldn't need "last_prev" */
		tpp_swapmem(file, last, sizeof(tpp_file));
		last->tf_prev = last_prev;
		file->tf_prev = last;
	}
}

TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_manualpopfile_break_commit(tpp_lexer *tpp_restrict self,
                                      tpp_file *tpp_restrict orig_prev) {
	tpp_file const *const file = tpp_lexer_getfile(self);
	while (file->tf_prev != orig_prev) {
		tpp_file *prev_prev;
		tpp_assert(orig_prev);
		prev_prev = orig_prev->tf_prev;
		tpp_file_fini(orig_prev);
		tpp_file_free(orig_prev);
		orig_prev = prev_prev;
	}
}
#endif /* TPP_HAVE_LEXER_MANUALPOPFILE */


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-warn.c                                             */
/************************************************************************/
#ifndef TPP_NO_SYSTEM_INCLUDES
#if TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
#include <stdio.h>
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */
#endif /* !TPP_NO_SYSTEM_INCLUDES */

TPP_DECL_BEGIN

#if TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER
TPP_IMPL tpp_ssize TPP_FORMATPRINTER_CC
_tpp_lexer_wrapped_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes) {
	(void)arg;
	return TPP_CONFIG_WARNPRINTER(text, num_bytes);
}
#endif /* TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER */

#if TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
TPP_IMPL tpp_ssize TPP_FORMATPRINTER_CC
_tpp_lexer_builtin_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes) {
	FILE *fp = stderr;
	(void)arg;
	fwrite(text, sizeof(tpp_char), num_bytes, fp);
	return ferror(fp) ? -1 : 0;
}
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */

#if TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER
TPP_IMPL tpp_ssize TPP_FORMATPRINTER_CC
_tpp_lexer_noop_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes) {
	(void)arg;
	(void)text;
	(void)num_bytes;
	return 0;
}
#endif /* TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER */



#if TPP_HAVE_WARNINGS
/* Interpret + print a warning-message "format" string.
 * The following %-encoded escape sequences are recognized:
 * - "%["    Start quoting text
 * - "%]"    Stop quoting text
 * - "%Pl"   1-based line of "pos" in "file"
 * - "%Pc"   1-based column of "pos" in "file"
 * - "%Pf"   Filename of given "file"
 * - "%Pt"   "%[current-token%]"
 * - "%s"    As defined by stdc, using va_arg(args, char *)
 * - "%.*s"  As defined by stdc, using va_arg(args, int) + va_arg(args, char *)
 * - "%.Ns"  As defined by stdc, using va_arg(args, char *)
 * - "%.NPt" "%[<N bytes starting at "pos">%]"
 * - "%d"    As defined by stdc, using va_arg(args, int)
 * - "%u"    As defined by stdc, using va_arg(args, unsigned int)
 * - "%c"    As defined by stdc, using va_arg(args, int)
 * - "%%"    "%" (emit a singular %-character)
 *
 * @param: pos:        [0..1] Lexer position used by certain format-patterns.
 * @param: pos_lcinfo: Lexer position used when "pos == NULL"
 * @param: file:       The file containing "pos"
 * @param: printer:    Output printer for formatted text
 * @param: arg:        Cookie argument for "printer"
 * @param: format:     Format pattern (see above)
 * @param: args:       Extra varargs-arguments for "format"
 * @return: >= 0:      Sum of return values of "printer".
 * @return: < 0:       First negative return value of "printer". The more high-level
 *                     "tpp_lexer_warnf" API returns "TPP_EWARNPRINT" in this case. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 5, 7)) tpp_ssize TPPVCALL
tpp_lexer_printf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                         tpp_lcinfo pos_lcinfo, tpp_formatprinter printer, void *arg,
                         char const *format, ...) {
	tpp_ssize result;
	va_list args;
	va_start(args, format);
	result = tpp_lexer_vprintf_warning(self, file, pos, pos_lcinfo,
	                                   printer, arg, format, args);
	va_end(args);
	return result;
}


static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_print_uint(tpp_formatprinter printer, void *arg, tpp_uintmax value) {
	char buf[TPP_UTOA_MAXLEN];
	char const *start = tpp_utoa(buf, value);
	return (*printer)(arg, (tpp_char const *)start, (tpp_size)((buf + tpp_lengthof(buf)) - start));
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_print_int(tpp_formatprinter printer, void *arg, tpp_intmax value) {
	char buf[TPP_ITOA_MAXLEN];
	char const *start = tpp_itoa(buf, value);
	return (*printer)(arg, (tpp_char const *)start, (tpp_size)((buf + tpp_lengthof(buf)) - start));
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_quote_start(tpp_formatprinter printer, void *arg) {
	/* TODO: Do something more interesting here! */
	return (*printer)(arg, (tpp_char const *)"`", 1);
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_quote_end(tpp_formatprinter printer, void *arg) {
	/* TODO: Do something more interesting here! */
	return (*printer)(arg, (tpp_char const *)"`", 1);
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_token_data(tpp_formatprinter printer, void *arg,
                      tpp_char const *start, tpp_size length) {
	/* TODO: Escape line-feeds while printing token body */
	return (*printer)(arg, start, length);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 5, 7)) tpp_ssize TPPCALL
tpp_lexer_vprintf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                          tpp_lcinfo pos_lcinfo, tpp_formatprinter printer, void *arg,
                          char const *format, va_list args) {
	bool lcinfo_loaded = pos == NULL;
	static char const null_str[] = "(null)";
	tpp_ssize temp, result = 0;
	char const *iter = format;
	char ch;
again:
	ch = *iter++;
	if (ch != '%') {
		if (ch == '\0') {
handle_eof:
			--iter;
			temp = (*printer)(arg, (tpp_char const *)format,
			                  (tpp_size)(iter - format));
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			return result;
		}
		goto again;
	}
	if ((iter - 1) > format) {
		temp = (*printer)(arg, (tpp_char const *)format,
		                  (tpp_size)((iter - 1) - format));
		if tpp_unlikely(temp < 0)
			goto err_temp;
		result += temp;
	}
	ch = *iter++;
	switch (ch) {
	case '\0':
		goto handle_eof;
	case '%':
		format = iter - 1;
		goto again;

	case '[': /* "%["    Start quoting text */
		temp = tpp_format_quote_start(printer, arg);
		break;

	case ']': /* "%]"    Stop quoting text */
		temp = tpp_format_quote_end(printer, arg);
		break;

	case 'P':
		ch = *iter++;
		switch (ch) {

		case 'l': /* "%Pl"   1-based line of "pos" in "file" */
		case 'c': /* "%Pc"   1-based column of "pos" in "file" */
		{
			/* Lazily load line/column information (if not already loaded) */
			if (!lcinfo_loaded) {
				pos_lcinfo = tpp_file_lcinfo(file, pos);
				lcinfo_loaded = true;
			}
			temp = tpp_format_print_int(printer, arg,
			                            ch == 'l' ? (tpp_lcinfo_getline(pos_lcinfo) + 1)
			                                      : (tpp_lcinfo_getcol(pos_lcinfo) + 1));
		}	break;

		case 'f': {
			/* "%Pf"   Filename of given "file" */
			char const *filename = tpp_file_userfilename(file);
			if (filename == NULL)
				filename = "?";
			temp = (*printer)(arg, (tpp_char const *)filename, tpp_strlen(filename));
		}	break;

		case 't': {
			/* "%Pt"   "%[current-token%]" */
#if TPP_HAVE_LEXER_REPRTOKENID
			char const *token_repr;
#endif /* TPP_HAVE_LEXER_REPRTOKENID */
			tpp_token const *const token = tpp_lexer_gettoken(self);
			tpp_size length;
			temp = tpp_format_quote_start(printer, arg);
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			length = (tpp_size)(token->tt_end - token->tt_start);
#if TPP_HAVE_LEXER_REPRTOKENID
			if ((length == 0) &&
			    (token_repr = tpp_lexer_reprtokenid(self, token->tt_id)) != NULL) {
				temp = (*printer)(arg, (tpp_char const *)token_repr, tpp_strlen(token_repr));
			} else
#endif /* TPP_HAVE_LEXER_REPRTOKENID */
			{
				temp = tpp_format_token_data(printer, arg, token->tt_start, length);
			}
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			temp = tpp_format_quote_end(printer, arg);
		}	break;

		default:
			format = iter - 3;
			goto again;
		}
		break;

	case 's': {
		/* "%s"    As defined by stdc, using va_arg(args, char *) */
		char const *s = va_arg(args, char const *);
		if (s == NULL)
			s = null_str;
		temp = (*printer)(arg, (tpp_char const *)s, tpp_strlen(s));
	}	break;

	case '.': {
		ch = *iter++;
		switch (ch) {

		case '*': {
			unsigned int length = va_arg(args, unsigned int);
			ch = *iter++;
			switch (ch) {

			case 's': {
				/* "%.*s"  As defined by stdc, using va_arg(args, int) + va_arg(args, char *) */
				char const *s = va_arg(args, char const *);
				if (s == NULL)
					s = null_str;
				temp = (*printer)(arg, (tpp_char const *)s, (tpp_size)length);
			}	break;

			default:
				format = iter - 4;
				goto again;
			}
		}	break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			char const *block_start = iter - 3;
			unsigned int length = (unsigned int)(ch - '0');
			for (;;) {
				ch = *iter++;
				if (ch >= '0' && ch <= '9') {
					length *= 10;
					length += (unsigned int)(ch - '0');
				} else {
					break;
				}
			}
			switch (ch) {
			case 's': {
				/* "%.Ns"  As defined by stdc, using va_arg(args, char *) */
				char const *s = va_arg(args, char const *);
				if (s == NULL)
					s = null_str;
				temp = (*printer)(arg, (tpp_char const *)s, (tpp_size)length);
			}	break;

			case 'P': {
				ch = *iter++;
				switch (ch) {

				case 't': {
					/* "%.NPt" "%[<N bytes starting at "pos">%]" */
					temp = tpp_format_quote_start(printer, arg);
					if tpp_unlikely(temp < 0)
						goto err_temp;
					result += temp;
					if (pos >= file->tf_end) {
						length = 0;
					} else {
						tpp_size maxlen = (tpp_size)(file->tf_end - pos);
						if ((tpp_size)length > maxlen)
							length = (unsigned int)maxlen;
					}
					temp = tpp_format_token_data(printer, arg, pos, (tpp_size)length);
					if tpp_unlikely(temp < 0)
						goto err_temp;
					result += temp;
					temp = tpp_format_quote_end(printer, arg);
				}	break;

				default:
					format = block_start;
					goto again;
				}
			}	break;

			default:
				format = block_start;
				goto again;
			}
		}	break;

		default:
			format = iter - 3;
			goto again;
		}

	}	break;

	case 'd': {
		/* "%d"    As defined by stdc, using va_arg(args, int) */
		int value = va_arg(args, int);
		temp = tpp_format_print_int(printer, arg, value);
	}	break;

	case 'u': {
		/* "%u"    As defined by stdc, using va_arg(args, unsigned int) */
		unsigned int value = va_arg(args, unsigned int);
		temp = tpp_format_print_uint(printer, arg, value);
	}	break;

	case 'c': {
		/* "%c"    As defined by stdc, using va_arg(args, int) */
		tpp_char ord = (tpp_char)va_arg(args, unsigned int);
		temp = (*printer)(arg, &ord, 1);
	}	break;

	default:
		format = iter - 2;
		goto again;
	}
	if tpp_unlikely(temp < 0)
		goto err_temp;
	result += temp;
	format = iter;
	goto again;
err_temp:
	return temp;
}

static char const tpp_file_and_line[] = TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT;

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_impl(tpp_lexer *tpp_restrict self, tpp_char const *pos,
                      tpp_lcinfo pos_lcinfo, tpp_warning_id id, va_list args) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno result;
	tpp_ssize printer_status;
	char const *warning_format;
	tpp_warning_invokeinfo invokeinfo;
	tpp_formatprinter printer;
	void *printer_arg;

	/* Quick check: are warnings disabled? */
	if (self->tl_state & TPP_LEXER_STATE_FLAG_NOWARNINGS)
		return TPP_EOK;

	/* Ask warning configuration how we should have this one */
	result = tpp_lexer_invokewarning(self, id, &invokeinfo);
#if TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
	if (TPP_ISERR(result))
		goto done;
#endif /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */

	/* Deal with certain warning states. */
	switch (invokeinfo.twii_state) {

	case TPP_WSTATE_DISABLED:
		goto done; /* Nothing to do here */

	case TPP_WSTATE_WARN: {
		/* Display as a warning */
#if TPP_HAVE_FILE_SYSHDR
		tpp_file const *const iofile = tpp_file_getiofile(file);
		if (iofile->tf_kind == TPP_FILE_KIND_IO &&
		    iofile->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_SYSHDR)
			return TPP_EOK; /* Suppress warnings in this file */
#endif /* TPP_HAVE_FILE_SYSHDR */
	}	break;

#if TPP_HAVE_WARNING_ERROR
	case TPP_WSTATE_ERROR: {
		tpp_size errors = tpp_lexer_geterrorcount(self);
		tpp_lexer_seterrorcount(self, errors + 1);
		if (errors >= tpp_lexer_geterrorlimit(self))
			result = TPP_ELEXERROR;
	}	break;
#endif /* TPP_HAVE_WARNING_ERROR */

	case TPP_WSTATE_FATAL:
		result = TPP_ELEXERROR;
		break;

	default: tpp_unreachable();
	}

	printer     = tpp_lexer_getwarnprinter(self);
	printer_arg = tpp_lexer_getwarnprinterarg(self);

	/* Print file-and-line prefix */
	printer_status = tpp_lexer_printf_warning(self, file, pos, pos_lcinfo,
	                                          printer, printer_arg,
	                                          tpp_file_and_line);
	if (printer_status < 0)
		goto err_printer;

	/* Print what this is about... */
	if (invokeinfo.twii_state == TPP_WSTATE_WARN) {
		printer_status = (*printer)(printer_arg, (tpp_char const *)"warning[", 8);
	} else {
		printer_status = (*printer)(printer_arg, (tpp_char const *)"error[", 6);
	}
	if (printer_status < 0)
		goto err_printer;

	/* Print the relevant context name. */
#if TPP_HAVE_WARNING_NUMBERS
	if (tpp_warning_context_id_isnumber(invokeinfo.twii_ctx_id)) {
		tpp_warning_id ctx_wid = tpp_warning_context_id_aswarning(invokeinfo.twii_ctx_id);
		unsigned int number = tpp_warning_getnumbers(ctx_wid)[0];
		if tpp_unlikely(number == TPP_WARNING_NUMBER_INVALID) {
			printer_status = (*printer)(printer_arg, (tpp_char const *)"?", 1);
		} else {
			printer_status = tpp_format_print_uint(printer, printer_arg, number);
		}
	} else
#endif /* TPP_HAVE_WARNING_NUMBERS */
	{
		tpp_warning_group_id group_id = tpp_warning_context_id_asgroup(invokeinfo.twii_ctx_id);
		char const *group_name = tpp_warning_group_getnames(group_id);
		if tpp_unlikely(group_name == NULL) {
			printer_status = (*printer)(printer_arg, (tpp_char const *)"?", 1);
		} else {
			printer_status = (*printer)(printer_arg, (tpp_char const *)"-W", 2);
			if (printer_status < 0)
				goto err_printer;
			printer_status = (*printer)(printer_arg, (tpp_char const *)group_name, tpp_strlen(group_name));
		}
	}
	if (printer_status < 0)
		goto err_printer;
	printer_status = (*printer)(printer_arg, (tpp_char const *)"]: ", 3);
	if (printer_status < 0)
		goto err_printer;


	/* Lookup the generic warning-message format for "id" */
	warning_format = tpp_warning_getformat(id);
	if (warning_format) {
		printer_status = tpp_lexer_vprintf_warning(self, file, pos, pos_lcinfo,
		                                           printer, printer_arg,
		                                           warning_format, args);
		if (printer_status < 0)
			goto err_printer;
		printer_status = (*printer)(printer_arg, (tpp_char const *)"\n", 1);
		if (printer_status < 0)
			goto err_printer;
	} else {
		/* In this case, "id" must be using a custom warning message expression! */
		switch (id) {

/************************************************************************/
/* MACROS FOR USE BY "TPP_WARNING_EX"                                   */
/************************************************************************/
#define tpp_do(expr)                       \
	do {                                   \
		if ((printer_status = (expr)) < 0) \
			goto err_printer;              \
	} while (0)
#define tpp_print_file_and_line(at_file, at_pos, at_lc)           \
	tpp_do(tpp_lexer_printf_warning(self, at_file, at_pos, at_lc, \
	                                printer, printer_arg,         \
	                                tpp_file_and_line))
#define tpp_print_file_and_line_at(at_file, at_pos) tpp_print_file_and_line(at_file, at_pos, tpp_lcinfo_of(-1, -1))
#define tpp_print_file_and_line_lc(at_file, at_lc)  tpp_print_file_and_line(at_file, NULL, at_lc)
#define tpp_warnf0(format)             tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format))
#define tpp_warnf1(format, a)          tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format, a))
#define tpp_warnf2(format, a, b)       tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format, a, b))
#define tpp_warnf3(format, a, b, c)    tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format, a, b, c))
#define tpp_warnf4(format, a, b, c, d) tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format, a, b, c, d))
#define tpp_current_va_arg(T)          va_arg(args, T)
#define tpp_current_va_args()          args
#define tpp_current_lexer()            self
#define tpp_current_warning_id()       id
/* ... */
/************************************************************************/

#define TPP_DEFS
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr) \
		case warning_id: {                                                     \
			expr;                                                              \
		}	break;
#include "tpp-amalgamation.h"
#undef TPP_DEFS

/************************************************************************/
#undef tpp_do
#undef tpp_print_file_and_line
#undef tpp_print_file_and_line_at
#undef tpp_print_file_and_line_lc
#undef tpp_warnf0
#undef tpp_warnf1
#undef tpp_warnf2
#undef tpp_warnf3
#undef tpp_warnf4
#undef tpp_current_va_arg
#undef tpp_current_va_args
#undef tpp_current_lexer
#undef tpp_current_warning_id
/************************************************************************/
		default:
			printer_status = (*printer)(printer_arg, (tpp_char const *)"UNKNOWN WARNING\n", 16);
			if (printer_status < 0)
				goto err_printer;
			break;
		}
	}

	/* Print origin traceback */
#if TPP_HAVE_INCLUDE_STACK
	{
		tpp_file *caller = file->tf_tprev;
		for (; caller; caller = caller->tf_tprev) {
			/* XXX: We could also display a range here:
			 * >> [caller->tf_tpos, caller->tf_pos)
			 *
			 * This range of bytes describes the "expression" that caused
			 * the include/macro-expansion to happen (it is either the range
			 * from the start of a macro's name, to the end of its parameter
			 * list, or the start of a #include-directive, to the trailing
			 * line-feed) */
			printer_status = tpp_lexer_printf_warning(self, caller, caller->tf_tpos,
			                                          tpp_lcinfo_of(-1, -1),
			                                          printer, printer_arg,
			                                          tpp_file_and_line);
			if (printer_status < 0)
				goto err_printer;
			printer_status = (*printer)(printer_arg, (tpp_char const *)"note: originating from here\n", 28);
			if (printer_status < 0)
				goto err_printer;
		}
	}
#endif /* TPP_HAVE_INCLUDE_STACK */

done:
#ifdef TPP_CONFIG_RAISE_LEXERROR
	if (result == TPP_ELEXERROR) {
		/* Allow users to define a hook to do additional
		 * processing when a lexer error should be raised */
		TPP_CONFIG_RAISE_LEXERROR;
	}
#endif /* TPP_CONFIG_RAISE_LEXERROR */
	return result;
err_printer:
	return TPP_EWARNPRINT;
}



/* Emits the specified lexer warning at the start of the current token.
 * @param: args: Format arguments specific to "id" (see '%'-sequences in warning expressions)
 * @return: TPP_EOK:        Warning was emitted, but you may proceed
 * @return: TPP_ELEXERROR:  Warning was emitted, but was configured as an error
 * @return: TPP_EWARNPRINT: Error while printing warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, va_list args) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	return tpp_lexer_vwarnf_at(self, token->tt_start, id, args);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf(self, id, args);
	va_end(args);
	return result;
}


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_vwarnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos,
                    tpp_warning_id id, va_list args) {
	return tpp_lexer_vwarnf_impl(self, pos, tpp_lcinfo_of(-1, -1), id, args);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf_impl(self, pos, tpp_lcinfo_of(-1, -1), id, args);
	va_end(args);
	return result;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_lc(tpp_lexer *tpp_restrict self, tpp_lcinfo lc,
                    tpp_warning_id id, va_list args) {
	return tpp_lexer_vwarnf_impl(self, NULL, lc, id, args);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf_lc(tpp_lexer *tpp_restrict self, tpp_lcinfo lc, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf_impl(self, NULL, lc, id, args);
	va_end(args);
	return result;
}
#endif /* TPP_HAVE_WARNINGS */


/* Warn if the current file's #ifdef-stack is non-empty.
 * @return: * : See `tpp_lexer_warnf()' */
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_warn_nonempty_ifdef(tpp_lexer *tpp_restrict self) {
	tpp_errno result = TPP_EOK;
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_size count = file->tf_ifdef.tids_cnt;
	while (count) {
		tpp_ifdef_stack_entry const *entry;
		--count;
		entry = &file->tf_ifdef.tids_vec[count];
		result = tpp_lexer_warnf_lc(self, entry->tidse_updated, TPP_W_EOF_BEFORE_ENDIF);
		if (TPP_ISERR(result))
			break;
	}
	return result;
}
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/expr.c                                                   */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_BUILTIN_EXPR_VALUE

/* Invoke operators on expression values. */
#if _TPP_EXPR_VALUE_KIND_MULTIPLE
static char const *TPPCALL
tpp_expr_value_kindrepr(_tpp_expr_value_kind kind) {
	switch (kind) {
	case _TPP_EXPR_VALUE_KIND_INT: return "<int>";
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: return "<float>";
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: return "<string>";
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
	default: tpp_unreachable();
	}
}

#if TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS
#define tpp_warn_bad_operands_unary(lexer, op_repr, self) \
	tpp_lexer_warnf(lexer, TPP_W_BAD_EXPRESSION_OPERANDS, \
	                op_repr, tpp_expr_value_kindrepr(_tpp_expr_value_getkind(self)), "", "", "", "")
#define tpp_warn_bad_operands_binary(lexer, lhs, op_repr, rhs)                      \
	tpp_lexer_warnf(lexer, TPP_W_BAD_EXPRESSION_OPERANDS,                           \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(lhs)), op_repr, \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(rhs)), "", "", "")
#define tpp_warn_bad_operands_binary2(lexer, lhs, op_repr1, rhs, op_repr2)           \
	tpp_lexer_warnf(lexer, TPP_W_BAD_EXPRESSION_OPERANDS,                            \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(lhs)), op_repr1, \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(rhs)), op_repr2, "", "")
#define tpp_warn_bad_operands_trinary(lexer, lhs, op_repr1, rhs1, op_repr2, rhs2, op_repr3)         \
	tpp_lexer_warnf(lexer, TPP_W_BAD_EXPRESSION_OPERANDS,                                           \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(lhs)), op_repr1,                \
	                (rhs1) ? tpp_expr_value_kindrepr(_tpp_expr_value_getkind(rhs1)) : "", op_repr2, \
	                (rhs2) ? tpp_expr_value_kindrepr(_tpp_expr_value_getkind(rhs2)) : "", op_repr3)
#else /* TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS */
#define tpp_warn_bad_operands_unary(lexer, op_repr, self)                                   TPP_EOK
#define tpp_warn_bad_operands_binary(lexer, lhs, op_repr, rhs)                              TPP_EOK
#define tpp_warn_bad_operands_binary2(lexer, lhs, op_repr1, rhs, op_repr2)                  TPP_EOK
#define tpp_warn_bad_operands_trinary(lexer, lhs, op_repr1, rhs1, op_repr2, rhs2, op_repr3) TPP_EOK
#endif /* !TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS */


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_expr_value_pos(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict self,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	(void)lexer;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	if (tpp_expr_value_isstring(self)) {
		tpp_errno error = tpp_warn_bad_operands_unary(lexer, "+", self);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
	return tpp_expr_value_copy(result, self);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_expr_value_neg(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict self,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	switch (_tpp_expr_value_getkind(self)) {
	case _TPP_EXPR_VALUE_KIND_INT:
		error = tpp_expr_value_init_int(result, -_tpp_expr_value_getint(self));
		break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		error = tpp_expr_value_init_float(result, -_tpp_expr_value_getfloat(self));
		break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		error = tpp_warn_bad_operands_unary(lexer, "-", self);
		if (TPP_ISERR(error))
			break;
		error = tpp_expr_value_copy(result, self);
		break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
	default: tpp_unreachable();
	}
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_expr_value_inv(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict self,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	switch (_tpp_expr_value_getkind(self)) {
	case _TPP_EXPR_VALUE_KIND_INT:
		error = tpp_expr_value_init_int(result, ~_tpp_expr_value_getint(self));
		break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS || TPP_HAVE_BUILTIN_EXPR_STRINGS
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		error = tpp_warn_bad_operands_unary(lexer, "~", self);
		if (TPP_ISERR(error))
			break;
		error = tpp_expr_value_copy(result, self);
		break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS || TPP_HAVE_BUILTIN_EXPR_STRINGS */
	default: tpp_unreachable();
	}
	return error;
}


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_add(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_int(result, lhs_value + _tpp_expr_value_getint(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, (tpp_float)lhs_value + _tpp_expr_value_getfloat(rhs));
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_float(result, lhs_value + (tpp_float)_tpp_expr_value_getint(rhs));
			break;
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, lhs_value + _tpp_expr_value_getfloat(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		TPP_REF tpp_string *result_string;
		tpp_string const *lhs_value;
		tpp_string const *rhs_value;
		if (!tpp_expr_value_isstring(rhs))
			goto err_bad_types;
#define WANT_err_bad_types
		lhs_value = _tpp_expr_value_getstring(lhs);
		rhs_value = _tpp_expr_value_getstring(rhs);
		result_string = tpp_string_malloc(tpp_string_len(lhs_value) +
		                                  tpp_string_len(rhs_value));
		if tpp_unlikely(!result_string)
			return TPP_ENOMEM;
		tpp_memcpy(tpp_string_str(result_string), tpp_string_str(lhs_value),
		           tpp_string_len(lhs_value) * sizeof(tpp_char));
		tpp_memcpy(tpp_string_str(result_string) + tpp_string_len(lhs_value),
		           tpp_string_str(rhs_value),
		           tpp_string_len(rhs_value) * sizeof(tpp_char));
		error = tpp_expr_value_init_string_inherited(result, result_string);
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " + ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_sub(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_int(result, lhs_value - _tpp_expr_value_getint(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, (tpp_float)lhs_value - _tpp_expr_value_getfloat(rhs));
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_float(result, lhs_value - (tpp_float)_tpp_expr_value_getint(rhs));
			break;
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, lhs_value - _tpp_expr_value_getfloat(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " - ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_mul(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_int(result, lhs_value * _tpp_expr_value_getint(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, (tpp_float)lhs_value * _tpp_expr_value_getfloat(rhs));
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING: {
			tpp_expr_value *temp = rhs;
			rhs = lhs;
			lhs = temp;
			goto handle_multiply_string;
		}
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_float(result, lhs_value * (tpp_float)_tpp_expr_value_getint(rhs));
			break;
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, lhs_value * _tpp_expr_value_getfloat(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		TPP_REF tpp_string *result_string;
		tpp_string *lhs_value;
		tpp_intmax multiplier;
		tpp_size i, lhs_length;
		tpp_size whole_size;
		tpp_char *dst;
		if (!tpp_expr_value_isint(rhs))
			goto err_bad_types;
#define WANT_err_bad_types
handle_multiply_string:
		lhs_value  = _tpp_expr_value_getstring(lhs);
		multiplier = _tpp_expr_value_getint(rhs);
		if (multiplier < 0)
			multiplier = 0; /* ??? */
		if (multiplier > (tpp_intmax)TPP_SIZE_MAX)
			multiplier = (tpp_intmax)TPP_SIZE_MAX;
		if (multiplier == 1)
			return tpp_expr_value_init_string(result, lhs_value);
		if (multiplier == 0) {
			result_string = tpp_string_newempty();
			return tpp_expr_value_init_string_inherited(result, result_string);
		}
		lhs_length = tpp_string_len(lhs_value);
		whole_size = (tpp_size)multiplier * lhs_length;
		if ((((tpp_size)multiplier | lhs_length) &
		     ~(tpp_size)(TPP_SIZE_MAX >> (TPP_CHAR_BIT * (TPP_SIZEOF_tpp_size / 2)))) &&
		    (whole_size / (tpp_size)multiplier != lhs_length))
			whole_size = TPP_SIZE_MAX;
#define TPP_MAX_STRING_LENGTH (((TPP_SIZE_MAX - tpp_offsetof(tpp_string, ts_str)) / sizeof(tpp_char)) - 1)
		if (whole_size > TPP_MAX_STRING_LENGTH)
			whole_size = TPP_MAX_STRING_LENGTH;
#undef TPP_MAX_STRING_LENGTH
		result_string = tpp_string_malloc(whole_size);
		if tpp_unlikely(!result_string)
			return TPP_ENOMEM;
		dst = tpp_string_str(result_string);
		for (i = 0; i < (tpp_size)multiplier; ++i) {
			tpp_memcpy(dst, tpp_string_str(lhs_value),
			           lhs_length * sizeof(tpp_char));
			dst += lhs_length;
		}
		error = tpp_expr_value_init_string_inherited(result, result_string);
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " * ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_div(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			if (rhs_value == 0) {
#if TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
				error = tpp_lexer_warnf(lexer, TPP_W_DIVIDE_BY_ZERO);
				if (TPP_ISERR(error))
					break;
#endif /* TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */
				error = tpp_expr_value_init_int(result, 0);
			} else {
				error = tpp_expr_value_init_int(result, lhs_value / rhs_value);
			}
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, (tpp_float)lhs_value / _tpp_expr_value_getfloat(rhs));
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_float(result, lhs_value / (tpp_float)_tpp_expr_value_getint(rhs));
			break;
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, lhs_value / _tpp_expr_value_getfloat(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " / ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_mod(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			if (rhs_value == 0) {
#if TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
				error = tpp_lexer_warnf(lexer, TPP_W_DIVIDE_BY_ZERO);
				if (TPP_ISERR(error))
					break;
#endif /* TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */
				error = tpp_expr_value_init_int(result, 0);
			} else {
				error = tpp_expr_value_init_int(result, lhs_value % rhs_value);
			}
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " % ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_shl(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value << rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " << ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_shr(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value >> rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " >> ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_and(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value & rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " & ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_xor(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value ^ rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " ^ ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_or(struct tpp_lexer *tpp_restrict lexer,
                  /*in*/ tpp_expr_value *tpp_restrict lhs,
                  /*in*/ tpp_expr_value *tpp_restrict rhs,
                  /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value | rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " | ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_impl(struct tpp_lexer *tpp_restrict lexer,
                        /*in*/ tpp_expr_value *tpp_restrict lhs,
                        /*in*/ tpp_expr_value *tpp_restrict rhs,
                        /*out*/ int *tpp_restrict p_delta,
                        char const *op_repr) {
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			if (lhs_value < rhs_value) {
				*p_delta = -1;
			} else if (lhs_value > rhs_value) {
				*p_delta = 1;
			} else {
				*p_delta = 0;
			}
			return TPP_EOK;
		}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT: {
			tpp_float rhs_value = _tpp_expr_value_getfloat(rhs);
			if (lhs_value < rhs_value) {
				*p_delta = -1;
			} else if (lhs_value > rhs_value) {
				*p_delta = 1;
			} else {
				*p_delta = 0;
			}
			return TPP_EOK;
		}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			if (lhs_value < rhs_value) {
				*p_delta = -1;
			} else if (lhs_value > rhs_value) {
				*p_delta = 1;
			} else {
				*p_delta = 0;
			}
			return TPP_EOK;
		}	break;

		case _TPP_EXPR_VALUE_KIND_FLOAT: {
			tpp_float rhs_value = _tpp_expr_value_getfloat(rhs);
			if (lhs_value < rhs_value) {
				*p_delta = -1;
			} else if (lhs_value > rhs_value) {
				*p_delta = 1;
			} else {
				*p_delta = 0;
			}
			return TPP_EOK;
		}	break;

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		tpp_string const *lhs_value;
		tpp_string const *rhs_value;
		tpp_size common_length;
		if (_tpp_expr_value_getkind(rhs) != _TPP_EXPR_VALUE_KIND_STRING)
			goto err_bad_types;
#define WANT_err_bad_types
		lhs_value = _tpp_expr_value_getstring(lhs);
		rhs_value = _tpp_expr_value_getstring(rhs);
		common_length = tpp_string_len(lhs_value);
		if (common_length > tpp_string_len(rhs_value))
			common_length = tpp_string_len(rhs_value);
		*p_delta = tpp_memcmp(tpp_string_str(lhs_value),
		                      tpp_string_str(rhs_value),
		                      common_length * sizeof(tpp_char));
		if (*p_delta == 0) {
			if (tpp_string_len(lhs_value) < tpp_string_len(rhs_value)) {
				*p_delta = -1;
			} else if (tpp_string_len(lhs_value) > tpp_string_len(rhs_value)) {
				*p_delta = 1;
			}
		}
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	tpp_unreachable();
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
	{
		tpp_errno error;
err_bad_types:
		error = tpp_warn_bad_operands_binary(lexer, lhs, op_repr, rhs);
		if (!TPP_ISERR(error))
			*p_delta = 0;
		return error;
	}
#endif /* WANT_err_bad_types */
}


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_eq(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " == ");
	*p_bool_result = delta == 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_ne(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " != ");
	*p_bool_result = delta != 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_lo(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " < ");
	*p_bool_result = delta < 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_le(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " <= ");
	*p_bool_result = delta <= 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_gr(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " > ");
	*p_bool_result = delta > 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_ge(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " >= ");
	*p_bool_result = delta >= 0;
	return error;
}


#if TPP_HAVE_BUILTIN_EXPR_STRINGS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_getindex(struct tpp_lexer *tpp_restrict lexer,
                        /*in*/ tpp_expr_value *tpp_restrict lhs,
                        /*in*/ tpp_expr_value *tpp_restrict index,
                        /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_string const *lhs_value;
	tpp_intmax index_value;
	tpp_char result_value;
	if (!tpp_expr_value_isstring(lhs) || !tpp_expr_value_isint(index)) {
		tpp_errno error = tpp_warn_bad_operands_binary2(lexer, lhs, "[", index, "]");
		if (TPP_ISERR(error))
			return error;
		return tpp_expr_value_copy(result, lhs);
	}
	lhs_value   = _tpp_expr_value_getstring(lhs);
	index_value = _tpp_expr_value_getint(index);
	if (index_value < 0 || index_value > (tpp_intmax)tpp_string_len(lhs_value))
		index_value = tpp_string_len(lhs_value); /* XXX: TPP_W_INDEX_OUT_OF_BOUNDS */
	result_value = tpp_string_str(lhs_value)[(tpp_size)index_value];
	/* XXX: If hosting compiler has "-fsigned-char", must:
	 *      >> result_value = (tpp_intmax)(signed char)result_value;
	 * iow: values [128,255] must become negative [-128,-1] */
	return tpp_expr_value_init_int(result, result_value);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 5)) tpp_errno TPPCALL
tpp_expr_value_getrange(struct tpp_lexer *tpp_restrict lexer,
                        /*in*/ tpp_expr_value *tpp_restrict lhs,
                        /*in*/ tpp_expr_value *lo,
                        /*in*/ tpp_expr_value *hi,
                        /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_string const *lhs_value;
	tpp_string *result_string;
	tpp_intmax lo_value, hi_value;
	tpp_size lhs_size, result_size;
	if (!tpp_expr_value_isstring(lhs) ||
	    (lo && !tpp_expr_value_isint(lo)) ||
	    (hi && !tpp_expr_value_isint(hi))) {
		tpp_errno error = tpp_warn_bad_operands_trinary(lexer, lhs, "[", lo, ":", hi, "]");
		if (TPP_ISERR(error))
			return error;
		return tpp_expr_value_copy(result, lhs);
	}
	lhs_value = _tpp_expr_value_getstring(lhs);
	lhs_size  = tpp_string_len(lhs_value);
	lo_value  = lo ? _tpp_expr_value_getint(lo) : (tpp_intmax)0;
	hi_value  = hi ? _tpp_expr_value_getint(hi) : (tpp_intmax)lhs_size;
	if (lo_value >= (tpp_intmax)lhs_size || !lhs_size)
		goto return_empty_string;
	if (lo_value < 0) {
		lo_value += (tpp_intmax)lhs_size;
		if (lo_value < 0)
			lo_value = (tpp_intmax)lhs_size - ((-lo_value) % (tpp_intmax)lhs_size);
	}
	tpp_assert((tpp_size)lo_value <= lhs_size);
	if (hi_value < 0) {
		hi_value += lhs_size;
		if (hi_value < 0)
			hi_value = (tpp_intmax)lhs_size - ((-hi_value) % (tpp_intmax)lhs_size);
	} else if (hi_value > (tpp_intmax)lhs_size) {
		hi_value = (tpp_intmax)lhs_size;
	}
	tpp_assert((tpp_size)hi_value <= lhs_size);
	if (hi_value < lo_value)
		hi_value = lo_value;
	if ((tpp_size)hi_value >= (tpp_size)lo_value)
		goto return_empty_string;
	result_size   = (tpp_size)hi_value - (tpp_size)lo_value;
	result_string = tpp_string_malloc(result_size);
	if tpp_unlikely(!result_string)
		return TPP_ENOMEM;
	tpp_memcpy(tpp_string_str(result_string),
	           tpp_string_str(lhs_value) + (tpp_size)lo_value,
	           result_size * sizeof(tpp_char));
	return tpp_expr_value_init_string_inherited(result, result_string);
return_empty_string:
	result_string = tpp_string_newempty();
	return tpp_expr_value_init_string_inherited(result, result_string);
}
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */


/* Determine the boolean-style value of "self"
 * Works for any kind of expression value.
 * @return: TPP_EOK: Success */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_expr_value_asbool(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict self,
                      bool *tpp_restrict p_bool_result) {
	(void)lexer;
	switch (_tpp_expr_value_getkind(self)) {
	case _TPP_EXPR_VALUE_KIND_INT:
		*p_bool_result = _tpp_expr_value_getint(self) != 0;
		break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		*p_bool_result = _tpp_expr_value_getfloat(self) != 0.0;
		break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		tpp_string const *str = _tpp_expr_value_getstring(self);
		*p_bool_result = tpp_string_len(str) != 0;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
	default: tpp_unreachable();
	}
	return TPP_EOK;
}

#else /* _TPP_EXPR_VALUE_KIND_MULTIPLE */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_div(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
	tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
	if (rhs_value == 0) {
#if TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
		error = tpp_lexer_warnf(lexer, TPP_W_DIVIDE_BY_ZERO);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */
		error = tpp_expr_value_init_int(result, 0);
	} else {
		error = tpp_expr_value_init_int(result, lhs_value / rhs_value);
	}
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_mod(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
	tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
	if (rhs_value == 0) {
#if TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
		error = tpp_lexer_warnf(lexer, TPP_W_DIVIDE_BY_ZERO);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */
		error = tpp_expr_value_init_int(result, 0);
	} else {
		error = tpp_expr_value_init_int(result, lhs_value % rhs_value);
	}
	return error;
}
#endif /* !_TPP_EXPR_VALUE_KIND_MULTIPLE */


/* Print the representation of "self" to "printer" (used to implement __TPP_EVAL)
 * @return: * : Sum of positive return value of `printer'
 * @return: (tpp_ssize)TPP_ISERR(*): An error was thrown, or `printer' returned this value */
#if TPP_HAVE_EXPR_VALUE_PRINTREPR
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_expr_value_printrepr(tpp_expr_value *tpp_restrict self,
                         tpp_formatprinter printer, void *arg) {
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	char value_buffer[TPP_ITOA_MAXLEN < TPP_FTOA_MAXLEN ? TPP_FTOA_MAXLEN : TPP_ITOA_MAXLEN];
#else /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
	char value_buffer[TPP_ITOA_MAXLEN];
#endif /* !TPP_HAVE_BUILTIN_EXPR_FLOATS */
	char *value_ptr;
#if _TPP_EXPR_VALUE_KIND_MULTIPLE
	switch (_tpp_expr_value_getkind(self)) {

	case _TPP_EXPR_VALUE_KIND_INT:
		break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float value = _tpp_expr_value_getfloat(self);
		tpp_size value_len = tpp_ftoa(value_buffer, value);
		return (*printer)(arg, (tpp_char const *)value_buffer, value_len);
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		tpp_ssize temp, result;
		tpp_string const *const str = _tpp_expr_value_getstring(self);
		result = (*printer)(arg, (tpp_char const *)"\"", 1);
		if tpp_unlikely(result < 0)
			return result;
		temp = tpp_token_encodestring(printer, arg, tpp_string_str(str),
		                              tpp_string_len(str) * sizeof(tpp_char));
		if tpp_unlikely(temp < 0)
			return temp;
		result += temp;
		temp = (*printer)(arg, (tpp_char const *)"\"", 1);
		if tpp_unlikely(temp < 0)
			return temp;
		result += temp;
		return result;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
#endif /* _TPP_EXPR_VALUE_KIND_MULTIPLE */
	value_ptr = tpp_itoa(value_buffer, _tpp_expr_value_getint(self));
	return (*printer)(arg, (tpp_char const *)value_ptr,
	                  (tpp_size)((value_buffer + TPP_ITOA_MAXLEN) - value_ptr));
}
#endif /* TPP_HAVE_EXPR_VALUE_PRINTREPR */

#endif /* TPP_HAVE_BUILTIN_EXPR_VALUE */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-yieldraw.c                                         */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_UNICODE

#define TPP_UTF8_SEQLEN_INIT(_0, _1, _2, _3, _4, _5, _6, _7, _8)         \
	{                                                                    \
		/* Unicode follow-up word (`0b10??????'). */                     \
		_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0, /* 0x80-0x8f */ \
		_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0, /* 0x90-0x9f */ \
		_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0, /* 0xa0-0xaf */ \
		_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0,_0, /* 0xb0-0xbf */ \
		/* `0b110?????' */                                               \
		_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2, /* 0xc0-0xcf */ \
		_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2,_2, /* 0xd0-0xdf */ \
		/* `0b1110????' */                                               \
		_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3,_3, /* 0xe0-0xef */ \
		/* `0b11110???' */                                               \
		_4,_4,_4,_4,_4,_4,_4,_4,                         /* 0xf0-0xf7 */ \
		_5,_5,_5,_5,                                     /* 0xf8-0xfb */ \
		_6,_6,                                           /* 0xfc-0xfd */ \
		_7,                                              /* 0xfe */      \
		_8                                               /* 0xff */      \
	}
static uint_least8_t const tpp_unicode_utf8seqlen[128] =
TPP_UTF8_SEQLEN_INIT(0, ~, 2, 3, 4, 0, 0, 0, 0);
static uint_least8_t const tpp_unicode_utf8seqlen_safe[128] =
TPP_UTF8_SEQLEN_INIT(1, ~, 2, 3, 4, 5, 6, 7, 8);
#undef TPP_UTF8_SEQLEN_INIT



/* Read a single unicode character from a given utf-8 blob.
 * WARNING: This function doesn't do any validity checking,
 *          allowing over-long utf-8 sequences, as well as
 *          incorrectly positioned UTF-8 continuation bytes. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8(tpp_char const **p_pos, tpp_char const *end) {
	tpp_char const *pos = *p_pos;
	tpp_unichar uc;
	if tpp_unlikely(pos >= end)
		return 0;
	uc = (tpp_unichar)*pos++;
	if (uc >= 0xc0) {
		uint_least8_t len = tpp_unicode_utf8seqlen_safe[uc & 0x7f];
		tpp_size maxlen = (tpp_size)(end - pos);
		if ((tpp_size)len > maxlen)
			len = (uint_least8_t)maxlen;
		switch (len) {
		case 0:
		case 1:
			break;
		case 2:
			uc = (uc & 0x1f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 3:
			uc  = (uc & 0x0f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 4:
			uc  = (uc & 0x07) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 5:
			uc = (uc & 0x03) << 24;
			uc |= (*pos++ & 0x3f) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 6:
			uc = (uc & 0x01) << 30;
			uc |= (*pos++ & 0x3f) << 24;
			uc |= (*pos++ & 0x3f) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 7:
			uc = (*pos++ & 0x03/*0x3f*/) << 30;
			uc |= (*pos++ & 0x3f) << 24;
			uc |= (*pos++ & 0x3f) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		case 8:
			/*uc = (*pos & 0x3f) << 36;*/
			++pos;
			uc = (*pos++ & 0x03/*0x3f*/) << 30;
			uc |= (*pos++ & 0x3f) << 24;
			uc |= (*pos++ & 0x3f) << 18;
			uc |= (*pos++ & 0x3f) << 12;
			uc |= (*pos++ & 0x3f) << 6;
			uc |= (*pos++ & 0x3f);
			break;
		default: tpp_unreachable();
		}
	}
	*p_pos = pos;
	return uc;
}

/* Same as `tpp_unicode_readutf8()', but read in reverse */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8_rev(tpp_char const **p_end, tpp_char const *base) {
	tpp_unichar uc;
	tpp_char const *iter = *p_end;
	uint_least8_t seqlen = 1;
	if tpp_unlikely(iter <= base)
		return 0;
	for (;;) {
		uc = *--iter;
		if ((uc & 0xc0) != 0x80)
			break;
		if (seqlen >= 8)
			break;
		++seqlen;
		if (iter <= base)
			break;
	}
	if (uc >= 0xc0) {
		switch (seqlen) {

		case 0:
		case 1:
			break;

		case 2:
			uc  = (uc & 0x1f) << 6;
			uc |= (iter[0] & 0x3f);
			break;

		case 3:
			uc  = (uc & 0x0f) << 12;
			uc |= (iter[0] & 0x3f) << 6;
			uc |= (iter[1] & 0x3f);
			break;

		case 4:
			uc  = (uc & 0x07) << 18;
			uc |= (iter[0] & 0x3f) << 12;
			uc |= (iter[1] & 0x3f) << 6;
			uc |= (iter[2] & 0x3f);
			break;

		case 5:
			uc  = (uc & 0x03) << 24;
			uc |= (iter[0] & 0x3f) << 18;
			uc |= (iter[1] & 0x3f) << 12;
			uc |= (iter[2] & 0x3f) << 6;
			uc |= (iter[3] & 0x3f);
			break;

		case 6:
			uc  = (uc & 0x01) << 30;
			uc |= (iter[0] & 0x3f) << 24;
			uc |= (iter[1] & 0x3f) << 18;
			uc |= (iter[2] & 0x3f) << 12;
			uc |= (iter[3] & 0x3f) << 6;
			uc |= (iter[4] & 0x3f);
			break;

		case 7:
			uc  = (iter[0] & 0x03/*0x3f*/) << 30;
			uc |= (iter[1] & 0x3f) << 24;
			uc |= (iter[2] & 0x3f) << 18;
			uc |= (iter[3] & 0x3f) << 12;
			uc |= (iter[4] & 0x3f) << 6;
			uc |= (iter[5] & 0x3f);
			break;

		case 8:
			/*result = (iter[0] & 0x3f) << 36;*/
			uc  = (iter[1] & 0x03/*0x3f*/) << 30;
			uc |= (iter[2] & 0x3f) << 24;
			uc |= (iter[3] & 0x3f) << 18;
			uc |= (iter[4] & 0x3f) << 12;
			uc |= (iter[5] & 0x3f) << 6;
			uc |= (iter[6] & 0x3f);
			break;

		default: tpp_unreachable();
		}
	}
	*p_end = iter;
	return uc;
}


/* Decode a single utf-8 character.
 * - If necessary, expand the current file's chunk
 * - If an illegal utf-8 byte sequence is encountered,
 *   change TPP_FILE_ENCODING_UTF8 to TPP_FILE_ENCODING_ASCII,
 *   leave `*p_pos' unchanged, set `*p_result = 0' and return
 *   `TPP_EOK'. However, if TPP_FILE_ENCODING_FORCE_UTF8 is
 *   used, this never happens and *all* characters are valid
 *
 * Caller must ensure:
 * - That `tpp_file_isutf8(tpp_lexer_getfile(self))'
 * - That `*p_pos < tpp_lexer_getfile(self)->tf_end'
 * - That `**p_pos >= 0x80'
 */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readutf8(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos,
                   tpp_unichar *tpp_restrict p_result) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_char const *end = file->tf_end;
	uint_least8_t len;
	tpp_unichar uc;
	tpp_assert(tpp_file_isutf8(file));
	tpp_assert(pos < end);
	uc = *pos;
	tpp_assert(uc >= 0x80);
	len = tpp_unicode_utf8seqlen[uc & 0x7f];
	if tpp_unlikely(len == 0)
		goto handle_ilseq;
	tpp_assert(len >= 2 && len <= 4);
	while tpp_unlikely((pos + len) > end) {
		/* Must extend the file */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		tpp_size rel_end = tpp_file_ptr2rel(file, end);
		tpp_errno error  = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			return error;
		end = tpp_file_rel2ptr(file, rel_end);
		tpp_assert(end <= file->tf_end);
		if tpp_unlikely(end >= file->tf_end)
			goto handle_ilseq; /* EOF in incomplete UTF-8 sequence :( */
		pos = tpp_file_rel2ptr(file, rel_pos);
		end = file->tf_end;
	}

	/* All required data has been loaded into memory -> parse the sequence */
	switch (len) {
	case 2:
		uc = (uc & 0x1f) << 6;
		uc |= (pos[1] & 0x3f);
		if tpp_unlikely(uc <= TPP_UTF8_1BYTE_MAX)
			goto handle_ilseq; /* under-long utf-8 sequence */
		break;
	case 3:
		uc  = (uc & 0x0f) << 12;
		uc |= (pos[1] & 0x3f) << 6;
		uc |= (pos[2] & 0x3f);
		if tpp_unlikely(uc <= TPP_UTF8_2BYTE_MAX)
			goto handle_ilseq; /* under-long utf-8 sequence */
		break;
	case 4:
		uc  = (uc & 0x07) << 18;
		uc |= (pos[1] & 0x3f) << 12;
		uc |= (pos[2] & 0x3f) << 6;
		uc |= (pos[3] & 0x3f);
		if tpp_unlikely(uc <= TPP_UTF8_3BYTE_MAX)
			goto handle_ilseq; /* under-long utf-8 sequence */
		break;
	default: tpp_unreachable();
	}

	/* Write back results... */
write_result:
	pos += len;
	*p_pos    = pos;
	*p_result = uc;
	return TPP_EOK;

handle_ilseq:
	if (file->tf_enc == TPP_FILE_ENCODING_UTF8) {
		/* Automatic UTF-8 -> switch to ASCII */
		file->tf_enc = TPP_FILE_ENCODING_ASCII;
		*p_result = 0;
		return TPP_EOK;
	}

	/* Forced UTF-8 */
	len = tpp_unicode_utf8seqlen_safe[uc & 0x7f];
	tpp_assert(len >= 1 && len <= 8);

	while ((pos + len) > end) {
		/* Must extend the file */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		tpp_size rel_end = tpp_file_ptr2rel(file, end);
		tpp_errno error  = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			return error;
		end = tpp_file_rel2ptr(file, rel_end);
		tpp_assert(end <= file->tf_end);
		pos = tpp_file_rel2ptr(file, rel_pos);
		if tpp_unlikely(end >= file->tf_end) {
			/* EOF in incomplete UTF-8 sequence :( */
			end = file->tf_end;
			len = (uint_least8_t)(tpp_size)(end - pos);
			tpp_assert(len >= 1 && len <= 8);
			break;
		}
		end = file->tf_end;
	}

	switch (len) {
	case 1:
		break;
	case 2:
		uc = (uc & 0x1f) << 6;
		uc |= (pos[1] & 0x3f);
		break;
	case 3:
		uc  = (uc & 0x0f) << 12;
		uc |= (pos[1] & 0x3f) << 6;
		uc |= (pos[2] & 0x3f);
		break;
	case 4:
		uc  = (uc & 0x07) << 18;
		uc |= (pos[1] & 0x3f) << 12;
		uc |= (pos[2] & 0x3f) << 6;
		uc |= (pos[3] & 0x3f);
		break;
	case 5:
		uc = (uc & 0x03) << 24;
		uc |= (pos[1] & 0x3f) << 18;
		uc |= (pos[2] & 0x3f) << 12;
		uc |= (pos[3] & 0x3f) << 6;
		uc |= (pos[4] & 0x3f);
		break;
	case 6:
		uc = (uc & 0x01) << 30;
		uc |= (pos[1] & 0x3f) << 24;
		uc |= (pos[2] & 0x3f) << 18;
		uc |= (pos[3] & 0x3f) << 12;
		uc |= (pos[4] & 0x3f) << 6;
		uc |= (pos[5] & 0x3f);
		break;
	case 7:
		uc = (pos[1] & 0x03/*0x3f*/) << 30;
		uc |= (pos[2] & 0x3f) << 24;
		uc |= (pos[3] & 0x3f) << 18;
		uc |= (pos[4] & 0x3f) << 12;
		uc |= (pos[5] & 0x3f) << 6;
		uc |= (pos[6] & 0x3f);
		break;
	case 8:
		/*uc = (pos[1] & 0x3f) << 36;*/
		uc = (pos[2] & 0x03/*0x3f*/) << 30;
		uc |= (pos[3] & 0x3f) << 24;
		uc |= (pos[4] & 0x3f) << 18;
		uc |= (pos[5] & 0x3f) << 12;
		uc |= (pos[6] & 0x3f) << 6;
		uc |= (pos[7] & 0x3f);
		break;
	default: tpp_unreachable();
	}
	goto write_result;
}
#endif /* TPP_HAVE_UNICODE */


/* Skip all space character (excluding line-feeds) starting
 * at "*p_pos" (this handles both ASCII and UNICODE space) */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_skipspace_nolf(tpp_lexer *tpp_restrict self,
                         tpp_char const **tpp_restrict p_pos) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_char const *end = file->tf_end;
	tpp_char ch;
again:
	if (pos >= end) {
		/* Must extend the file */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		error = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			goto return_error;
		pos = tpp_file_rel2ptr(file, rel_pos);
		end = file->tf_end;
		if (pos >= end)
			goto done;
	}

	ch = *pos;
	if (tpp_ascii_isspace_nolf(ch)) {
		++pos;
		goto again;
	}

#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(file) && ch >= 0x80) {
		tpp_unichar uc;
		tpp_char const *npos = pos;
		error = tpp_lexer_readutf8(self, &npos, &uc);
		if (TPP_ISERR(error))
			goto return_error;
		if (tpp_unicode_isspace_nolf(uc)) {
			pos = npos;
			goto again;
		}
	}
#endif /* TPP_HAVE_UNICODE */

done:
	*p_pos = pos;
	return TPP_EOK;
return_error:
	return error;
}


#if TPP_HAVE_BSE
/* Check if **p_pos is the \-character of an escaped line-feed, and if BSE is enabled.
 * If both are the case, skip over said escaped linefeed, possibly expanding the
 * currently loaded text-chunk, before returning a pointer past said BSE. (if the
 * BSE is followed by further BSE, those are all skipped also, meaning that the
 * returned pointer is either EOF or something other than the start of a BSE)
 *
 * When BSE isn't enabled, or the \-character doesn't escape a line-feed, don't alter `*p_pos'
 */
static tpp_errno TPPCALL
tpp_lexer_skip_bse(tpp_lexer *self, tpp_char const **p_pos) {
	tpp_errno error;
	tpp_char ch;
	tpp_char const *scan = *p_pos;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_size rel_pos;
#if TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
	bool is_trigraph;
#endif /* TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
	if (!tpp_lexer_has(self, BSE))
		return TPP_EOK;

again_scan:
	rel_pos = tpp_file_ptr2rel(file, scan);
	tpp_assert(scan < file->tf_end);
#if TPP_HAVE_TRIGRAPHS
	tpp_assert(*scan == '\\' || *scan == '?');
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
	is_trigraph = false;
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
	++scan;
	if (scan[-1] == '?') {
		scan += 2; /* ??/ */
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
		is_trigraph = true;
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
	}
#else /* TPP_HAVE_TRIGRAPHS */
	tpp_assert(*scan == '\\');
	++scan;
#endif /* !TPP_HAVE_TRIGRAPHS */

#if TPP_HAVE_BSE_WHITESPACE
	if (tpp_lexer_has(self, BSE_WHITESPACE)) {
		error = tpp_lexer_skipspace_nolf(self, &scan);
		if (TPP_ISERR(error))
			goto return_error;
	}
#endif /* TPP_HAVE_BSE_WHITESPACE */
	if tpp_unlikely(scan >= file->tf_end) {
		/* Must extend file! */
		tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
		error = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			goto return_error;
		scan = tpp_file_rel2ptr(file, rel_scan);
		if tpp_unlikely(scan >= file->tf_end)
			goto return_noop;
	}

	/* Now check if "scan" points at a line-feed */
	ch = *scan;
	if (tpp_ascii_islf(ch)) {
		++scan;
		if (ch == '\r') {
			if tpp_unlikely(scan >= file->tf_end) {
				/* Must extend file! */
				tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
				error = tpp_file_expandchunk(file);
				if (TPP_ISERR(error))
					goto return_error;
				scan = tpp_file_rel2ptr(file, rel_scan);
			}
			if (scan < file->tf_end && *scan == '\n')
				++scan;
		}
#if TPP_HAVE_UNICODE
got_bse_after_linefeed:
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
		if (is_trigraph) {
			error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_pos),
			                           TPP_W_ENCOUNTERED_TRIGRAPH);
			if (TPP_ISERR(error))
				goto return_error;
		}
#endif /* TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */

		/* Check for further BSE */
		if tpp_unlikely(scan >= file->tf_end) {
			/* Must extend file! */
			tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
			error = tpp_file_expandchunk(file);
			if (TPP_ISERR(error))
				goto return_error;
			scan = tpp_file_rel2ptr(file, rel_scan);
		}
		if (scan < file->tf_end) {
			if (*scan == '\\')
				goto again_scan;
#if TPP_HAVE_TRIGRAPHS
			if (*scan == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
				if ((scan + 1) >= file->tf_end) {
					tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
					error = tpp_file_expandchunk(file);
					if (TPP_ISERR(error))
						goto return_error;
					scan = tpp_file_rel2ptr(file, rel_scan);
				}
				if (scan[1] == '?') {
					if ((scan + 2) >= file->tf_end) {
						tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
						error = tpp_file_expandchunk(file);
						if (TPP_ISERR(error))
							goto return_error;
						scan = tpp_file_rel2ptr(file, rel_scan);
					}
					if (scan[2] == '/')
						goto again_scan;
				}
			}
#endif /* TPP_HAVE_TRIGRAPHS */
		}

		/* Return updated pointer */
		*p_pos = scan;
		return TPP_EOK;
	}
#if TPP_HAVE_UNICODE
	if (ch >= 0x80 && tpp_file_isutf8(file)) {
		tpp_unichar uc;
		tpp_char const *nscan = scan;
		error = tpp_lexer_readutf8(self, &nscan, &uc);
		if (TPP_ISERR(error))
			goto return_error;
		if (tpp_unicode_islf(uc)) {
			scan = nscan;
			goto got_bse_after_linefeed;
		}
	}
#endif /* TPP_HAVE_UNICODE */
return_noop:
	*p_pos = tpp_file_rel2ptr(file, rel_pos);
	return TPP_EOK;
return_error:
	return error;
}
#endif /* TPP_HAVE_BSE */

/* Given a "*p_pos" that points past the initial "issymstrt" character,
 * seek (and potentially extend the lexer's current file) until the
 * first non-issymcont character is found, then return a pointer to
 * said non-issymcont character
 *
 * NOTE: This function does NOT handle BSE!
 */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_keyword(tpp_lexer *self, tpp_char const **p_pos) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_char const *end = file->tf_end;
	tpp_char ch;
again:
	if (pos >= end) {
		/* Must extend the file */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		error = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			goto return_error;
		pos = tpp_file_rel2ptr(file, rel_pos);
		end = file->tf_end;
		if (pos >= end)
			goto done;
	}

	ch = *pos;
	if (tpp_ascii_issymcont(ch)) {
#if TPP_HAVE_TPP_TOK_DOLLAR
		if (ch == '$' && tpp_lexer_has(self, TPP_TOK_DOLLAR))
			goto done;
#endif /* TPP_HAVE_TPP_TOK_DOLLAR */
		++pos;
		goto again;
	}

#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(file) && ch >= 0x80) {
		tpp_unichar uc;
		tpp_char const *npos = pos;
		error = tpp_lexer_readutf8(self, &npos, &uc);
		if (TPP_ISERR(error))
			goto return_error;
		if (tpp_unicode_issymcont(uc)) {
			pos = npos;
			goto again;
		}
	}
#endif /* TPP_HAVE_UNICODE */

done:
	*p_pos = pos;
	return TPP_EOK;
return_error:
	return error;
}

#if TPP_HAVE_BSE
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_skip_bse_after_keyword(tpp_lexer *self, tpp_char const **p_pos) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	while (pos < file->tf_end) {
		tpp_errno error;
		tpp_char const *npos;
		tpp_size rel_before, rel_after;
		if (*pos == '\\') {
			/* Backslash */
		} else
#if TPP_HAVE_TRIGRAPHS
		if (*pos == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
			if ((pos + 1) >= file->tf_end) {
				tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
				error = tpp_file_expandchunk(file);
				if (TPP_ISERR(error))
					return error;
				pos = tpp_file_rel2ptr(file, rel_pos);
			}
			if ((pos + 1) < file->tf_end && pos[1] == '?') {
				if ((pos + 2) >= file->tf_end) {
					tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_file_expandchunk(file);
					if (TPP_ISERR(error))
						return error;
					pos = tpp_file_rel2ptr(file, rel_pos);
				}
				if ((pos + 2) < file->tf_end && pos[2] == '/') {
					/* Trigraph backslash */
				} else {
					break;
				}
			} else {
				break;
			}
		} else
#endif /* TPP_HAVE_TRIGRAPHS */
		{
			break;
		}

		npos  = pos;
		error = tpp_lexer_skip_bse(self, &npos);
		if (TPP_ISERR(error))
			return error;
		if (npos == pos)
			break;
		rel_before = tpp_file_ptr2rel(file, npos);
		error = tpp_lexer_seek_end_of_keyword(self, &npos);
		if (TPP_ISERR(error))
			return error;
		rel_after = tpp_file_ptr2rel(file, npos);
		tpp_assert(rel_before <= rel_after);
		if (rel_before >= rel_after)
			break;
		pos = npos;
	}
	*p_pos = pos;
	return TPP_EOK;
}
#endif /* TPP_HAVE_BSE */


#undef NEED_tpp_lexer_seek_eol
#if (TPP_HAVE_TPP_TOK_SQL_COMMENT ||   \
     TPP_HAVE_TPP_TOK_ASM_COMMENT ||   \
     TPP_HAVE_TPP_TOK_SHELL_COMMENT || \
     TPP_HAVE_TPP_TOK_CXX_COMMENT)
#define NEED_tpp_lexer_seek_eol 1
#else /* ... */
#define NEED_tpp_lexer_seek_eol 0
#endif /* !... */

#undef NEED_tpp_lexer_seek_end_of_string
#if (TPP_HAVE_TPP_TOK_STRING ||                   \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CHAR ||                     \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL)
#define NEED_tpp_lexer_seek_end_of_string 1
#else /* ... */
#define NEED_tpp_lexer_seek_end_of_string 0
#endif /* !... */

#undef NEED_tpp_lexer_seek_end_of_block_string
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
#define NEED_tpp_lexer_seek_end_of_block_string 1
#else /* ... */
#define NEED_tpp_lexer_seek_end_of_block_string 0
#endif /* !... */

#undef NEED_tpp_lexer_seek_end_of_cxx_raw_string
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
#define NEED_tpp_lexer_seek_end_of_cxx_raw_string 1
#else /* ... */
#define NEED_tpp_lexer_seek_end_of_cxx_raw_string 0
#endif /* !... */

#undef NEED_tpp_lexer_seek_end_of_raw_string
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
#define NEED_tpp_lexer_seek_end_of_raw_string 1
#else /* ... */
#define NEED_tpp_lexer_seek_end_of_raw_string 0
#endif /* !... */


/* Read a single character (byte) whilst accounting for BSE (if enabled)
 * and automatically extending the current file if EOF is reached.
 * On true EOF:
 * - *p_result = '\0'
 * - *p_pos = tpp_lexer_getfile(self)->tf_end
 * - return TPP_EOK;
 *
 * @return: TPP_EOK:    Character was read
 * @return: TPP_ENOMEM: Out of memory
 * @return: TPP_EIO:    Failed to read from underlying file */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readchar(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos,
                   tpp_char *tpp_restrict p_result) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
	tpp_char const *end = file->tf_end;
	tpp_char ch;
#if TPP_HAVE_BSE
continue_at_pos:
#endif /* TPP_HAVE_BSE */
	tpp_assert(pos <= end);
	if tpp_unlikely(pos >= end) {
		/* Must extend the current chunk */
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		tpp_size rel_end = tpp_file_ptr2rel(file, end);
		tpp_errno error  = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			return error;
		end = tpp_file_rel2ptr(file, rel_end);
		tpp_assert(end <= file->tf_end);
		pos = tpp_file_rel2ptr(file, rel_pos);
		if tpp_unlikely(end >= file->tf_end) {
			/* EOF */
			*p_pos    = pos;
			*p_result = '\0';
			return TPP_EOK;
		}
		end = file->tf_end;
	}
	tpp_assert(pos < end);
	ch = *pos;
#if TPP_HAVE_BSE
	if (ch == '\\') {
		/* Deal with BSE sequences... */
		tpp_size rel_after;
		tpp_size rel_before = tpp_file_ptr2rel(file, pos);
		tpp_errno error = tpp_lexer_skip_bse(self, &pos);
		if (TPP_ISERR(error))
			return error;
		rel_after = tpp_file_ptr2rel(file, pos);
		tpp_assert(rel_before <= rel_after);
		if (rel_before < rel_after) {
			end = file->tf_end;
			goto continue_at_pos;
		}
	}
#endif /* TPP_HAVE_BSE */
	++pos;
#if TPP_HAVE_TRIGRAPHS
	/* Support for character aliases:
	 * "??=" -> "#"
	 * "??(" -> "["
	 * "??/" -> "\"
	 * "??)" -> "]"
	 * "??'" -> "^"
	 * "??<" -> "{"
	 * "??!" -> "|"
	 * "??>" -> "}"
	 * "??-" -> "~"
	 * "???" -> "?" */
	if (ch == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
		if (pos >= end) {
			tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
			tpp_errno error  = tpp_file_expandchunk(file);
			if (TPP_ISERR(error))
				return error;
			pos = tpp_file_rel2ptr(file, rel_pos);
			end = file->tf_end;
		}
		if (pos < end && *pos == '?') {
			++pos;
			if (pos >= end) {
				tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
				tpp_errno error  = tpp_file_expandchunk(file);
				if (TPP_ISERR(error))
					return error;
				pos = tpp_file_rel2ptr(file, rel_pos);
				end = file->tf_end;
			}
			if (pos >= end) {
not_a_trigraph:
				--pos;
			} else {
				switch (*pos) {
				case '=': ch = '#'; break;
				case '(': ch = '['; break;
				case ')': ch = ']'; break;
				case '\'': ch = '^'; break;
				case '<': ch = '{'; break;
				case '!': ch = '|'; break;
				case '>': ch = '}'; break;
				case '-': ch = '~'; break;
				case '?': ch = '?'; break;
				case '/':
					ch = '\\';
#if TPP_HAVE_BSE
					/* Deal with BSE sequences... */
					if (tpp_lexer_has(self, BSE)) {
						tpp_size rel_after;
						tpp_size rel_before;
						tpp_errno error;
						pos -= 2;
						rel_before = tpp_file_ptr2rel(file, pos);
						error = tpp_lexer_skip_bse(self, &pos);
						if (TPP_ISERR(error))
							return error;
						rel_after = tpp_file_ptr2rel(file, pos);
						tpp_assert(rel_before <= rel_after);
						if (rel_before < rel_after) {
							end = file->tf_end;
							goto continue_at_pos;
						}
						pos = tpp_file_rel2ptr(file, rel_before) + 1;
					}
#endif /* TPP_HAVE_BSE */
					break;
				default: goto not_a_trigraph;
				}
				++pos;
			}
		}
	}
#endif /* TPP_HAVE_TRIGRAPHS */

	*p_pos = pos;
	*p_result = ch;
	return TPP_EOK;
}


#if TPP_HAVE_UNICODE
/* Same as `tpp_lexer_readchar()', but (if the current file's encoding allows
 * it, and IN(*p_pos) points at a multi-byte character), decode a multi-byte
 * character and return it. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readunichar(tpp_lexer *tpp_restrict self,
                      tpp_char const **tpp_restrict p_pos,
                      tpp_unichar *tpp_restrict p_result) {
	tpp_char ch;
	tpp_errno error = tpp_lexer_readchar(self, p_pos, &ch);
	if (!TPP_ISERR(error) && ch >= 0x80) {
		tpp_file const *const file = tpp_lexer_getfile(self);
		if (tpp_file_isutf8(file)) {
			--(*p_pos);
			return tpp_lexer_readutf8(self, p_pos, p_result);
		}
		ch &= 0x7f; /* ??? What else could be done here? */
	}
	*p_result = (tpp_unichar)ch;
	return error;
}
#endif /* TPP_HAVE_UNICODE */



#if NEED_tpp_lexer_seek_eol || (TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT)
#undef tpp_lexer_seek_eol__STYLE_PARAM
#undef tpp_lexer_seek_eol__STYLE_ARG
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
#define tpp_lexer_seek_eol__STYLE_PARAM  , tpp_token_id comment_style
#define tpp_lexer_seek_eol__STYLE_ARG(x) , x
#else /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
#define tpp_lexer_seek_eol__STYLE_PARAM  /* nothing */
#define tpp_lexer_seek_eol__STYLE_ARG(x) /* nothing */
#endif /* !TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */

/* Seek forward until *after* the next line-feed character (or true EOF)
 * Given `*p_pos' will be updated to point *after* the LF character (or *at* the EOF) */
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_eol(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos
                   tpp_lexer_seek_eol__STYLE_PARAM);
TPP_INTERN_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_eol(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos
                   tpp_lexer_seek_eol__STYLE_PARAM)
#else /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_eol(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos
                   tpp_lexer_seek_eol__STYLE_PARAM)
#endif /* !TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT */
{
	tpp_errno error = TPP_EOK;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *pos = *p_pos;
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
	tpp_size rel_after_bse, rel_before_bse;
#endif /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
	tpp_char ch;
again:
	if (pos >= file->tf_end) {
		tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
		error = tpp_file_expandchunk(file);
		pos = tpp_file_rel2ptr(file, rel_pos);
		if (TPP_ISERR(error))
			goto done;
		if (pos >= file->tf_end)
			goto done; /* True EOF */
	}
	ch = *pos++;
	if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
handle_ascii_lf:
#endif /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
		if (ch == '\r') {
			if (pos >= file->tf_end) {
				tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
				error = tpp_file_expandchunk(file);
				pos = tpp_file_rel2ptr(file, rel_pos);
				if (TPP_ISERR(error))
					goto done;
			}
			if (pos < file->tf_end && *pos == '\n')
				++pos;
		}
		goto done;
	} else
#if TPP_HAVE_UNICODE
	if (ch >= 0x80 && tpp_file_isutf8(file)) {
		tpp_unichar uc;
		--pos;
		error = tpp_lexer_readutf8(self, &pos, &uc);
		if (TPP_ISERR(error))
			goto done;
		if (tpp_unicode_islf(uc))
			goto done;
	} else
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
handle_non_lf_ch:
#endif /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */

#if TPP_HAVE_BSE
	if (ch == '\\') {
		/* Deal with BSE sequences... */
#if TPP_HAVE_TRIGRAPHS
handle_backslash:
#endif /* TPP_HAVE_TRIGRAPHS */
		--pos;
		rel_before_bse = tpp_file_ptr2rel(file, pos);
		error = tpp_lexer_skip_bse(self, &pos);
		if (TPP_ISERR(error))
			goto done;
		rel_after_bse = tpp_file_ptr2rel(file, pos);
		tpp_assert(rel_before_bse <= rel_after_bse);
		if (rel_before_bse >= rel_after_bse) {
#if TPP_HAVE_TRIGRAPHS
			if (ch != '\\')
				pos += 2;
#endif /* TPP_HAVE_TRIGRAPHS */
			++pos;
			goto again; /* Not a BSE sequence */
		}

#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
		/* Emit warning if we don't encounter a start-of-line matching "comment_style":
		 * >> // foo \    .
		 * >>    bar      << Warn here
		 *
		 * >> // foo \    .
		 * >> // bar      << Do not warn here */
		if (comment_style != TPP_TOK_EOF) {
			for (;;) {
				if (pos >= file->tf_end) {
					tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_file_expandchunk(file);
					pos = tpp_file_rel2ptr(file, rel_pos);
					if (TPP_ISERR(error))
						goto done;
					if (pos >= file->tf_end)
						goto done; /* True EOF */
				}
				ch = *pos++;
				if (tpp_ascii_islf(ch)) {
					error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_before_bse),
					                           TPP_W_LINE_COMMENT_CONTINUED);
					if (TPP_ISERR(error))
						goto done;
					goto handle_ascii_lf;
				} else if (tpp_ascii_isspace(ch)) {
					continue;
				} else
#if TPP_HAVE_UNICODE
				if (ch >= 0x80 && tpp_file_isutf8(file)) {
					tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
					tpp_unichar uc;
					--pos;
					error = tpp_lexer_readutf8(self, &pos, &uc);
					if (TPP_ISERR(error))
						goto done;
					if (tpp_unicode_islf(uc)) {
						error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_before_bse),
						                           TPP_W_LINE_COMMENT_CONTINUED);
						goto done;
					}
					pos = tpp_file_rel2ptr(file, rel_pos);
				} else
#endif /* TPP_HAVE_UNICODE */
				{
				}

				switch (comment_style) {
#if TPP_HAVE_TPP_TOK_CXX_COMMENT
				case TPP_TOK_CXX_COMMENT:
					if (ch == '/') {
						if (pos >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							pos = tpp_file_rel2ptr(file, rel_pos);
							if (TPP_ISERR(error))
								goto done;
							if (pos >= file->tf_end)
								break;
						}
						if (pos[0] == '/') {
							++pos;
							goto again;
						}
					}
					break;
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
				case TPP_TOK_SHELL_COMMENT:
					if (ch == '#')
						goto again;
#if TPP_HAVE_TRIGRAPHS
					if (ch == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
						if (pos >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							pos = tpp_file_rel2ptr(file, rel_pos);
							if (TPP_ISERR(error))
								goto done;
							if (pos >= file->tf_end)
								break;
						}
						if (pos[0] == '?') {
							if ((pos + 1) >= file->tf_end) {
								tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
								error = tpp_file_expandchunk(file);
								pos = tpp_file_rel2ptr(file, rel_pos);
								if (TPP_ISERR(error))
									goto done;
								if ((pos + 1) >= file->tf_end)
									break;
							}
							if (pos[1] == '=') {
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
								error = tpp_lexer_warnf_at(self, pos - 1, TPP_W_ENCOUNTERED_TRIGRAPH);
								if (TPP_ISERR(error))
									goto done;
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
								pos += 2;
								goto again;
							}
						}
					}
#endif /* TPP_HAVE_TRIGRAPHS */
					break;
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#if TPP_HAVE_TPP_TOK_ASM_COMMENT
				case TPP_TOK_ASM_COMMENT:
					if (ch == '/')
						goto again;
					break;
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT */
#if TPP_HAVE_TPP_TOK_SQL_COMMENT
				case TPP_TOK_SQL_COMMENT:
					if (ch == '-') {
						if (pos >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							pos = tpp_file_rel2ptr(file, rel_pos);
							if (TPP_ISERR(error))
								goto done;
						}
						if (pos[0] == '-') {
							++pos;
							goto again;
						}
					}
					break;
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT */
				default: tpp_unreachable();
				}
				error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_before_bse),
				                           TPP_W_LINE_COMMENT_CONTINUED);
				if (TPP_ISERR(error))
					goto done;
				goto handle_non_lf_ch;
			}
		}
#endif /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
		goto again;
	} else
#if TPP_HAVE_TRIGRAPHS
	if (ch == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
		if (pos >= file->tf_end) {
			tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
			error = tpp_file_expandchunk(file);
			pos = tpp_file_rel2ptr(file, rel_pos);
			if (TPP_ISERR(error))
				goto done;
			if (pos >= file->tf_end)
				goto done; /* True EOF */
		}
		if (pos[0] != '?')
			goto again;
		if ((pos + 1) >= file->tf_end) {
			tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
			error = tpp_file_expandchunk(file);
			pos = tpp_file_rel2ptr(file, rel_pos);
			if (TPP_ISERR(error))
				goto done;
			if ((pos + 1) >= file->tf_end)
				goto again;
		}
		if (pos[1] != '/')
			goto again;
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
		error = tpp_lexer_warnf_at(self, pos - 1, TPP_W_ENCOUNTERED_TRIGRAPH);
		if (TPP_ISERR(error))
			goto done;
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
		goto handle_backslash;
	} else
#endif /* TPP_HAVE_TRIGRAPHS */
#endif /* TPP_HAVE_BSE */
	{
		goto again;
	}

done:
	*p_pos = pos;
	return error;
}
#endif /* NEED_tpp_lexer_seek_eol || TPP_HAVE_CPP_DIRECTIVES */


/* Find the end of a "foo" or 'foo' string
 *      IN(*p_pos) == ^   ^ == OUT(*p_pos) */
#if NEED_tpp_lexer_seek_end_of_string
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_string(tpp_lexer *tpp_restrict self,
                             tpp_char const **tpp_restrict p_pos,
                             tpp_char quote_char) {
	tpp_file const *const file = tpp_lexer_getfile(self);
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF || TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
	tpp_size rel_start = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF || TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
	for (;;) {
		tpp_char ch;
		tpp_errno error;
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE)
		tpp_size old_pos = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE) */
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == quote_char)
			break;
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
		if (ch == '\\') {
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (TPP_ISERR(error))
				return error;
			if (ch == 0 && (*p_pos) >= file->tf_end)
				goto warn_premature_eof;
		} else
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE)
		if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_UNICODE
handle_linefeed:
#endif /* TPP_HAVE_UNICODE */
			if (!tpp_lexer_has(self, TPP_TOK_STRING_ALLOW_MULTILINE)) {
				*p_pos = tpp_file_rel2ptr(file, old_pos);
				/* Warning if a line-feed is encountered */
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
				return tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
				                          TPP_W_STRING_TERMINATED_BY_LINEFEED);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
				break;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
			} else {
#if TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED
				error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, old_pos),
				                           TPP_W_STRING_CONTINUED_AFTER_LINEFEED);
				if (TPP_ISERR(error))
					return error;
#endif /* TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED */
			}
		} else
#if TPP_HAVE_UNICODE
		if (ch >= 0x80 && tpp_file_isutf8(file)) {
			/* Check for unicode linefeed */
			tpp_unichar uc;
			*p_pos = tpp_file_rel2ptr(file, old_pos);
			error  = tpp_lexer_readutf8(self, p_pos, &uc);
			if (TPP_ISERR(error))
				return error;
			if (tpp_unicode_islf(uc))
				goto handle_linefeed;
		} else
#endif /* TPP_HAVE_UNICODE */
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE) */
		{
		}
	}
	return TPP_EOK;
warn_premature_eof:
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	return tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
	                          TPP_W_STRING_TERMINATED_BY_EOF);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
}
#endif /* NEED_tpp_lexer_seek_end_of_string */


/* Find the end of a """foo""" or '''foo''' string
 *        IN(*p_pos) == ^     ^ == OUT(*p_pos) */
#if NEED_tpp_lexer_seek_end_of_block_string
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_block_string(tpp_lexer *tpp_restrict self,
                                   tpp_char const **tpp_restrict p_pos,
                                   tpp_char quote_char) {
	/* NOTE: """triple quote \""" and string continues"""
	 * iow: \-escape sequences still exist in block-strings! */
	tpp_file const *const file = tpp_lexer_getfile(self);
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	tpp_size const rel_start = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	for (;;) {
		tpp_char ch;
		tpp_errno error;
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == quote_char) {
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (TPP_ISERR(error))
				return error;
			if (ch == quote_char) {
				error = tpp_lexer_readchar(self, p_pos, &ch);
				if (TPP_ISERR(error))
					return error;
				if (ch == quote_char)
					break; /* triple quote_char -> end block-string */
			}
		}
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
		if (ch == '\\') {
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (TPP_ISERR(error))
				return error;
			if (ch == 0 && (*p_pos) >= file->tf_end)
				goto warn_premature_eof;
		}
	}
	return TPP_EOK;
warn_premature_eof:
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	return tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
	                          TPP_W_STRING_TERMINATED_BY_EOF);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
}
#endif /* NEED_tpp_lexer_seek_end_of_block_string */


/* Find the end of a R"AB(foo)AB" string
 *       IN(*p_pos) == ^         ^ == OUT(*p_pos) */
#if NEED_tpp_lexer_seek_end_of_cxx_raw_string
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_cxx_raw_string(tpp_lexer *tpp_restrict self,
                                     tpp_char const **tpp_restrict p_pos) {
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_size rel_pattern_start = tpp_file_ptr2rel(file, *p_pos);
	tpp_size rel_pattern_end, delim_len;
	tpp_char ch;
	tpp_errno error;

	/* Find end of pattern string */
	delim_len = 0;
	for (;;) {
		rel_pattern_end = tpp_file_ptr2rel(file, *p_pos);
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == '(')
			break;
		if (tpp_ascii_islf(ch)) {
			/* TODO: Warning if a line-feed is encountered */
		}
		if (delim_len == 16) {
			/* TODO: Warning if raw string delimiter longer than 16 characters */
		}

		++delim_len;
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
	}

	/* Scan string body... */
	for (;;) {
		tpp_size rel_pattern_iter;
		tpp_char const *pos2;
continue_string:
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch != ')') {
			if (ch == 0 && (*p_pos) >= file->tf_end)
				goto warn_premature_eof;
			continue;
		}
		for (rel_pattern_iter = rel_pattern_start;
		     rel_pattern_iter < rel_pattern_end;) {
			tpp_char pattern_ch;
			tpp_size rel_pos;

			/* Read pattern character */
			rel_pos = tpp_file_ptr2rel(file, *p_pos);
			pos2    = tpp_file_rel2ptr(file, rel_pattern_iter);
			error   = tpp_lexer_readchar(self, &pos2, &pattern_ch);
			rel_pattern_iter = tpp_file_ptr2rel(file, pos2);
			(*p_pos) = tpp_file_rel2ptr(file, rel_pos);
			if (TPP_ISERR(error))
				return error; /* Shouldn't cause errors (was already scanned before) */

			/* Read input character */
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (TPP_ISERR(error))
				return error;
			if (pattern_ch != ch)
				goto continue_string;
		}

		/* Entire pattern was matched -> next character must be '"' */
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == '"')
			break;
	}
	return TPP_EOK;
warn_premature_eof:
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	return tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_pattern_start),
	                          TPP_W_STRING_TERMINATED_BY_EOF);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
}
#endif /* NEED_tpp_lexer_seek_end_of_cxx_raw_string */


/* Find the end of a R"foo" or R'foo' string
 *       IN(*p_pos) == ^   ^ == OUT(*p_pos) */
#if NEED_tpp_lexer_seek_end_of_raw_string
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_raw_string(tpp_lexer *tpp_restrict self,
                                 tpp_char const **tpp_restrict p_pos,
                                 tpp_char quote_char) {
	tpp_file const *const file = tpp_lexer_getfile(self);
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF || TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
	tpp_size rel_start = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF || TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
	for (;;) {
		tpp_char ch;
		tpp_errno error;
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE)
		tpp_size old_pos = tpp_file_ptr2rel(file, *p_pos);
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE) */
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == quote_char)
			break;
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE)
		if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_UNICODE
handle_linefeed:
#endif /* TPP_HAVE_UNICODE */
			if (!tpp_lexer_has(self, TPP_TOK_STRING_ALLOW_MULTILINE)) {
				*p_pos = tpp_file_rel2ptr(file, old_pos);
				/* Warning if a line-feed is encountered */
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
				return tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
				                          TPP_W_STRING_TERMINATED_BY_LINEFEED);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
				break;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
			} else {
#if TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED
				error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, old_pos),
				                           TPP_W_STRING_CONTINUED_AFTER_LINEFEED);
				if (TPP_ISERR(error))
					return error;
#endif /* TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED */
			}
		} else
#if TPP_HAVE_UNICODE
		if (ch >= 0x80 && tpp_file_isutf8(file)) {
			/* Check for unicode linefeed */
			tpp_unichar uc;
			*p_pos = tpp_file_rel2ptr(file, old_pos);
			error  = tpp_lexer_readutf8(self, p_pos, &uc);
			if (TPP_ISERR(error))
				return error;
			if (tpp_unicode_islf(uc))
				goto handle_linefeed;
		} else
#endif /* TPP_HAVE_UNICODE */
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_STRING_ALLOW_MULTILINE) */
		{
		}
	}
	return TPP_EOK;
warn_premature_eof:
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
	return tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
	                          TPP_W_STRING_TERMINATED_BY_EOF);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
}
#endif /* NEED_tpp_lexer_seek_end_of_raw_string */


#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
/* Seek end of unichar: foo\U12345678XY
 *                         ^=in      ^out */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_skip_bsi(tpp_lexer *tpp_restrict self, tpp_char const **p_pos) {
	/* C says that (implementations can threat) this:
	 * >> char const *\U0001f431 = "cat";
	 *
	 * as a valid identifier. -- We should support that (*and* interpret
	 * it as "\xF0\x9F\x90\xB1" (its utf-8 repr) during keyword lookup)
	 *
	 * For this purpose, the "*_bse" version of keyword lookup functions
	 * should also have another extension that lets them treat \u and \U
	 * sequences specially! */
	tpp_errno error = TPP_EOK;
	tpp_char const *scan = *p_pos;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_size rel_start = tpp_file_ptr2rel(file, scan);
	tpp_assert(scan < file->tf_end);
#if TPP_HAVE_TRIGRAPHS
	tpp_assert(*scan == '\\' || *scan == '?');
	++scan;
	if (scan[-1] == '?')
		scan += 2; /* ??/ */
#else /* TPP_HAVE_TRIGRAPHS */
	tpp_assert(*scan == '\\');
	++scan;
#endif /* !TPP_HAVE_TRIGRAPHS */
	if (scan >= file->tf_end) {
		tpp_size rel_pos = tpp_file_ptr2rel(file, scan);
		error = tpp_file_expandchunk(file);
		if (TPP_ISERR(error))
			goto done;
		scan = tpp_file_rel2ptr(file, rel_pos);
	}
	if (*scan == 'u' || *scan == 'U') {
		/* No BSE allowed in here -- \u \U happens at the same time,
		 * and I don't want to allow one escape escaping another escape.
		 *
		 * NOTE: If this ever needs to be added, would also need to add
		 *       support in "tpp_decode_bsi()" (our partner function)
		 *
		 * TODO: This is incorrect -- the C standard allows BSE inside
		 *       \u (and \U)-characters keywords since C++23:
		 * https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2621r2.pdf
		 */
		unsigned int cur_digit = 0;
		unsigned int max_digit = *scan == 'U' ? 8 : 4;
		do {
			tpp_char nibble_ch;
			if ((scan + cur_digit + 1) >= file->tf_end) {
				tpp_size rel_pos = tpp_file_ptr2rel(file, scan);
				error = tpp_file_expandchunk(file);
				if (TPP_ISERR(error))
					goto done;
				scan = tpp_file_rel2ptr(file, rel_pos);
				if ((scan + cur_digit + 1) >= file->tf_end)
					break;
			}
			nibble_ch = scan[cur_digit + 1];
			if (nibble_ch >= '0' && nibble_ch <= '9') {
				/* ... */
			} else if (nibble_ch >= 'a' && nibble_ch <= 'f') {
				/* ... */
			} else if (nibble_ch >= 'A' && nibble_ch <= 'F') {
				/* ... */
			} else {
				break;
			}
			++cur_digit;
		} while (cur_digit < max_digit);
		if (cur_digit == 0)
			goto done;
		scan += 1;
		scan += cur_digit;
		*p_pos = scan;
		return TPP_EOK;
	}
done:
	*p_pos = tpp_file_rel2ptr(file, rel_start);
	return error;
}
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */



/* Do a raw yield and update `self->tl_tok' in the process, then return `tl_tok.tt_id'.
 * - On EOF, automatically pop `tl_file->tf_prev' and continue reading from there
 * - On error, return one of `TPP_TOK_E*' (e.g. `TPP_TOK_EIO').
 *   Such error codes will NOT be stored in `tl_tok.tt_id'!
 *
 * NOTE: This function does *NOT* deal with:
 * - Preprocessor directives
 * - Builtin macros
 * - User-defined macros
 *
 * @return: * :                  The newly read token
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldraw(tpp_lexer *tpp_restrict self) {
	return tpp_lexer_yieldraw_at(self, &tpp_lexer_gettoken(self)->tt_end);
}




/* Same as `tpp_lexer_yieldraw()', but populate the token from a custom `*p_pos',
 * and don't pop files from the current #include-stack (unless `p_pos' is the top-
 * most file's `tf_pos')
 *
 * NOTES:
 *  - This function will *NOT* populate "tpp_lexer_gettoken(self)->tt_end",
 *    however the value it would have written there is OUT(*p_pos), meaning
 *    you can just use that instead, and call this function multiple times
 *    to yield more than 1 token
 *  - This function can be used to peek future tokens, as it will also expand
 *    the current file when `*p_pos' would go beyond its end. (in this case,
 *    `*p_pos' is updated such that it always remains valid)
 *  - Unlike `tpp_lexer_yieldraw()', this function will *not* modify the
 *    currently loaded file's `tf_pos' (unless `p_pos == &file->tf_pos'),
 *    meaning that if EOF is reached, the file's chunk will only ever be
 *    expanded, but no old data (that would appear before `tf_pos') will
 *    be deallocated
 *  - This function will also not automatically move on to the next file
 *    in line when the current one has been fully exhausted (unless the
 *    given `p_pos == &file->tf_pos'), meaning that TPP_TOK_EOF will be
 *    returned when no more data can be loaded.
 *
 * This is used to implement `tpp_lexer_yieldraw()', which simply
 * passes `p_pos = &tpp_lexer_gettoken(self)->tt_end'
 *
 * @return: * : See `tpp_lexer_yieldraw()' */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at(tpp_lexer *tpp_restrict self, tpp_char const **p_pos) {
#undef NEED_read_ch2
#if (NEED_tpp_lexer_seek_eol ||                   \
     NEED_tpp_lexer_seek_end_of_string ||         \
     NEED_tpp_lexer_seek_end_of_block_string ||   \
     NEED_tpp_lexer_seek_end_of_cxx_raw_string || \
     NEED_tpp_lexer_seek_end_of_raw_string ||     \
     1) /* TODO: List of all multi-char tokens */
#define NEED_read_ch2 1
#else /* ... */
#define NEED_read_ch2 0
#endif /* !... */

	tpp_errno error;
	tpp_token_id result;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char ch;
#if NEED_read_ch2
	tpp_char ch2;
#endif /* NEED_read_ch2 */
	tpp_char const *pos, *end;

	/* Relative offset from start of loaded area of file
	 * (usually `0', unless a custom "p_pos" is used) */
	tpp_size rel_start;
again:
	pos = *p_pos;
	end = file->tf_end;
	if tpp_unlikely(pos >= end)
		goto eof;
	rel_start = tpp_file_ptr2rel(file, pos);
again_read_from_pos:
	token->tt_start = pos;
	ch = *pos++;

	/* Primary CHARACTER -> TOKEN conversion switch */
#if TPP_HAVE_TRIGRAPHS
switch_on_ch:
#endif /* TPP_HAVE_TRIGRAPHS */
	switch (ch) {
#if NEED_read_ch2
#define read_ch2()                                    \
	do {                                              \
		error = tpp_lexer_readchar(self, &pos, &ch2); \
		if (TPP_ISERR(error))                         \
			goto return_error;                        \
	} while (0)
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
	/* Call this after 'read_ch2()' returned a
	 * (used) character that is one of: #[]^{|}~?\ */
#define warn_if_ch2_is_trigraph()                                                  \
	do {                                                                           \
		if (pos[-1] != ch2) {                                                      \
			error = tpp_lexer_warnf_at(self, pos - 3, TPP_W_ENCOUNTERED_TRIGRAPH); \
			if (TPP_ISERR(error))                                                  \
				goto return_error;                                                 \
		}                                                                          \
	} while (0)
#else /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
#define warn_if_ch2_is_trigraph() (void)0
#endif /* !TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
#endif /* NEED_read_ch2 */



/************************************************************************/
	case '<': {
#if (TPP_HAVE_TPP_TOK_LANGLE_LANGLE ||              \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE ||       \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_LANGLE_RANGLE ||              \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE ||        \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE ||        \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS ||               \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE ||        \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE ||        \
     TPP_HAVE_DIGRAPHS)
		if (!tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_LANGLE) &&
		    !tpp_lexer_has(self, DIGRAPHS))
			break;
		read_ch2();

#if TPP_HAVE_DIGRAPHS
		if (ch2 == '%') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
				result = (tpp_token_id)'{'; /* "<%" -> "{" */
				goto set_result;
			}
		} else if (ch2 == ':') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
				read_ch2();
				if (ch2 == ':')
					break; /* "<::" -> "<", "::"  ("break" here to follow single-char path) */
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
				result = (tpp_token_id)'['; /* "<:" -> "[" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_DIGRAPHS */
#if (TPP_HAVE_TPP_TOK_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
		if (ch2 == '<') {
#if (TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_EQUAL) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
				read_ch2();

#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
				if (ch2 == '<') {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
						tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
						read_ch2();
						if (ch2 == '=') {
							result = TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL; /* "<<<=" */
							goto set_result;
						}
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
						pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
					}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */

#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_LANGLE)) {
						result = TPP_TOK_LANGLE_LANGLE_LANGLE; /* "<<<" */
						goto set_result;
					}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
				if (ch2 == '=') {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE_EQUAL)) {
						result = TPP_TOK_LANGLE_LANGLE_EQUAL; /* "<<=" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
				{
				}

#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
			}
#endif /* ... */

#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_LANGLE)) {
				result = TPP_TOK_LANGLE_LANGLE; /* "<<" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
		} else
#endif /* ... */
#if (TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE || \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE)
		if (ch2 == '=') {
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_RANGLE)) {
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
				read_ch2();
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
				if (ch2 == '<') {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_LANGLE)) {
						result = TPP_TOK_LANGLE_EQUAL_LANGLE; /* "<=<" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
				if (ch2 == '>') {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL_RANGLE)) {
						result = TPP_TOK_LANGLE_EQUAL_RANGLE; /* "<=>" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
				{
				}
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */

#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_EQUAL)) {
				result = TPP_TOK_LANGLE_EQUAL; /* "<=" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
		} else
#endif /* ... */
#if (TPP_HAVE_TPP_TOK_LANGLE_MINUS ||        \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE || \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE)
		if (ch2 == '-') {
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_RANGLE)) {
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
				read_ch2();
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
				if (ch2 == '<') {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_LANGLE)) {
						result = TPP_TOK_LANGLE_MINUS_LANGLE; /* "<-<" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
				if (ch2 == '>') {
					if (tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS_RANGLE)) {
						result = TPP_TOK_LANGLE_MINUS_RANGLE; /* "<->" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
				{
				}
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */

#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_MINUS)) {
				result = TPP_TOK_LANGLE_MINUS; /* "<-" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
		if (ch2 == '>') {
			if (tpp_lexer_has(self, TPP_TOK_LANGLE_RANGLE)) {
				result = TPP_TOK_LANGLE_RANGLE; /* "<>" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '>': {
#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||              \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE ||       \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_RANGLE_LANGLE ||              \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS ||               \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE)
		if (!tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_RANGLE))
			break;
		read_ch2();

#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
		if (ch2 == '>') {
#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
			if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_EQUAL) ||
			    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
				read_ch2();

#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
				if (ch2 == '>') {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
					if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
						tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
						read_ch2();
						if (ch2 == '=') {
							result = TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL; /* ">>>=" */
							goto set_result;
						}
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
						pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
					}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */

#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
					if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_RANGLE)) {
						result = TPP_TOK_RANGLE_RANGLE_RANGLE; /* ">>>" */
						goto set_result;
					}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
				} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
				if (ch2 == '=') {
					if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE_EQUAL)) {
						result = TPP_TOK_RANGLE_RANGLE_EQUAL; /* ">>=" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
				{
				}

#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
			}
#endif /* ... */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_RANGLE_RANGLE)) {
				result = TPP_TOK_RANGLE_RANGLE; /* ">>" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
		} else
#endif /* ... */
#if (TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE || \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE)
		if (ch2 == '=') {
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_RANGLE)) {
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
				read_ch2();
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
				if (ch2 == '<') {
					if (tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_LANGLE)) {
						result = TPP_TOK_RANGLE_EQUAL_LANGLE; /* ">=<" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
				if (ch2 == '>') {
					if (tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL_RANGLE)) {
						result = TPP_TOK_RANGLE_EQUAL_RANGLE; /* ">=>" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
				{
				}
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
			}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */

#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_RANGLE_EQUAL)) {
				result = TPP_TOK_RANGLE_EQUAL; /* ">=" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
		} else
#endif /* ... */
#if (TPP_HAVE_TPP_TOK_RANGLE_MINUS ||        \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE || \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE)
		if (ch2 == '-') {
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_RANGLE)) {
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
				read_ch2();
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
				if (ch2 == '<') {
					if (tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_LANGLE)) {
						result = TPP_TOK_RANGLE_MINUS_LANGLE; /* ">-<" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
				if (ch2 == '>') {
					if (tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS_RANGLE)) {
						result = TPP_TOK_RANGLE_MINUS_RANGLE; /* ">->" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
				{
				}
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
			}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */

#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
			if (tpp_lexer_has(self, TPP_TOK_RANGLE_MINUS)) {
				result = TPP_TOK_RANGLE_MINUS; /* ">-" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
		if (ch2 == '<') {
			if (tpp_lexer_has(self, TPP_TOK_RANGLE_LANGLE)) {
				result = TPP_TOK_RANGLE_LANGLE; /* "><" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '=': {
#if (TPP_HAVE_TPP_TOK_EQUAL_EQUAL ||                \
     TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL ||          \
     TPP_HAVE_TPP_TOK_EQUAL_PLUS ||                 \
     TPP_HAVE_TPP_TOK_EQUAL_MINUS ||                \
     TPP_HAVE_TPP_TOK_EQUAL_STAR ||                 \
     TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR ||            \
     TPP_HAVE_TPP_TOK_EQUAL_SLASH ||                \
     TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH ||          \
     TPP_HAVE_TPP_TOK_EQUAL_PERCENT ||              \
     TPP_HAVE_TPP_TOK_EQUAL_AMP ||                  \
     TPP_HAVE_TPP_TOK_EQUAL_PIPE ||                 \
     TPP_HAVE_TPP_TOK_EQUAL_HAT ||                  \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE ||               \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE || \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE ||               \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE || \
     TPP_HAVE_TPP_TOK_EQUAL_AT ||                   \
     TPP_HAVE_TPP_TOK_EQUAL_TILDE ||                \
     TPP_HAVE_TPP_TOK_EQUAL_COLON ||                \
     TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM ||              \
     TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM ||        \
     TPP_HAVE_TPP_TOK_EQUAL_QMARK)

		if (!tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_PLUS) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_MINUS) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_STAR) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_STAR_STAR) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_SLASH) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_SLASH_SLASH) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_PERCENT) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_AMP) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_PIPE) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_HAT) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_AT) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_TILDE) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_COLON) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_EXCLAIM) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EXCLAIM) &&
		    !tpp_lexer_has(self, TPP_TOK_EQUAL_QMARK))
			break;
		read_ch2();
#if (TPP_HAVE_TPP_TOK_EQUAL_EQUAL ||       \
     TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL || \
     TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM)
		if (ch2 == '=') {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EQUAL) ||
			    tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EXCLAIM)) {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
				read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
				if (ch2 == '=') {
					if (tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EQUAL)) {
						result = TPP_TOK_EQUAL_EQUAL_EQUAL; /* "===" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
				if (ch2 == '=') {
					if (tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL_EXCLAIM)) {
						result = TPP_TOK_EQUAL_EQUAL_EXCLAIM; /* "==!" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
				{
				}
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_EQUAL)) {
				result = TPP_TOK_EQUAL_EQUAL; /* "==" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR || TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
		if (ch == '*') {
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_STAR_STAR)) {
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
				read_ch2();
				if (ch2 == '*') {
					result = TPP_TOK_EQUAL_STAR_STAR; /* "=**" */
					goto set_result;
				}
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_STAR)) {
				result = TPP_TOK_EQUAL_STAR; /* "=*" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR || TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH || TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
		if (ch == '/') {
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_SLASH_SLASH)) {
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
				read_ch2();
				if (ch2 == '/') {
					result = TPP_TOK_EQUAL_SLASH_SLASH; /* "=//" */
					goto set_result;
				}
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_SLASH)) {
				result = TPP_TOK_EQUAL_SLASH; /* "=/" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH */
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH || TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if (TPP_HAVE_TPP_TOK_EQUAL_LANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE || \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)
		if (ch2 == '<') {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
				read_ch2();
				if (ch2 == '<') {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
					if (tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
						tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
						read_ch2();
						if (ch2 == '<') {
							result = TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE; /* "=<<<" */
							goto set_result;
						}
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
						pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
					}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */

#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
					if (tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE_LANGLE)) {
						result = TPP_TOK_EQUAL_LANGLE_LANGLE; /* "=<<" */
						goto set_result;
					}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
				}
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */

#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_LANGLE)) {
				result = TPP_TOK_EQUAL_LANGLE; /* "=<" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
		} else
#endif /* ... */
#if (TPP_HAVE_TPP_TOK_EQUAL_RANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE || \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
		if (ch2 == '>') {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE) ||
			    tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
				read_ch2();
				if (ch2 == '>') {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
					if (tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
						tpp_size rel_end_of_3char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
						read_ch2();
						if (ch2 == '>') {
							result = TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE; /* "=>>>" */
							goto set_result;
						}
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
						pos = tpp_file_rel2ptr(file, rel_end_of_3char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
					}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */

#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
					if (tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE_RANGLE)) {
						result = TPP_TOK_EQUAL_RANGLE_RANGLE; /* "=>>" */
						goto set_result;
					}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
				}
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */

#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_RANGLE)) {
				result = TPP_TOK_EQUAL_RANGLE; /* "=>" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS
		if (ch2 == '+') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_PLUS)) {
				result = TPP_TOK_EQUAL_PLUS; /* "=+" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS
		if (ch2 == '-') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_MINUS)) {
				result = TPP_TOK_EQUAL_MINUS; /* "=-" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
		if (ch2 == '%') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_PERCENT)) {
				result = TPP_TOK_EQUAL_PERCENT; /* "=%" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP
		if (ch2 == '&') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_AMP)) {
				result = TPP_TOK_EQUAL_AMP; /* "=&" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE
		if (ch2 == '|') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_PIPE)) {
				result = TPP_TOK_EQUAL_PIPE; /* "=|" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT
		if (ch2 == '^') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_HAT)) {
				result = TPP_TOK_EQUAL_HAT; /* "=^" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_EQUAL_AT
		if (ch2 == '@') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_AT)) {
				result = TPP_TOK_EQUAL_AT; /* "=@" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE
		if (ch2 == '~') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_TILDE)) {
				result = TPP_TOK_EQUAL_TILDE; /* "=~" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON
		if (ch2 == ':') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_COLON)) {
				result = TPP_TOK_EQUAL_COLON; /* "=:" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
		if (ch2 == '!') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_EXCLAIM)) {
				result = TPP_TOK_EQUAL_EXCLAIM; /* "=!" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK
		if (ch2 == '?') {
			if (tpp_lexer_has(self, TPP_TOK_EQUAL_QMARK)) {
				result = TPP_TOK_EQUAL_QMARK; /* "=?" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '!': {
#if (TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL ||       \
     TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL || \
     TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM)
		if (!tpp_lexer_has(self, TPP_TOK_EXCLAIM_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_EXCLAIM_EQUAL_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_EXCLAIM_EXCLAIM))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
		if (ch2 == '=') {
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_EXCLAIM_EQUAL_EQUAL)) {
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
				read_ch2();
				if (ch2 == '=') {
					result = TPP_TOK_EXCLAIM_EQUAL_EQUAL; /* "!==" */
					goto set_result;
				}
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
			}
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_EXCLAIM_EQUAL)) {
				result = TPP_TOK_EXCLAIM_EQUAL; /* "!=" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
		} else
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
		if (ch2 == '!') {
			if (tpp_lexer_has(self, TPP_TOK_EXCLAIM_EXCLAIM)) {
				result = TPP_TOK_EXCLAIM_EXCLAIM; /* "!!" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '.': {
#if (TPP_HAVE_TPP_TOK_DOT_DOT_DOT || \
     TPP_HAVE_TPP_TOK_DOT_DOT ||     \
     TPP_HAVE_TPP_TOK_DOT_STAR)
		if (!tpp_lexer_has(self, TPP_TOK_DOT_DOT_DOT) &&
		    !tpp_lexer_has(self, TPP_TOK_DOT_DOT) &&
		    !tpp_lexer_has(self, TPP_TOK_DOT_STAR))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT || TPP_HAVE_TPP_TOK_DOT_DOT
		if (ch2 == '.') {
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
			if (tpp_lexer_has(self, TPP_TOK_DOT_DOT_DOT)) {
#if TPP_HAVE_TPP_TOK_DOT_DOT
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
				read_ch2();
				if (ch2 == '.') {
					result = TPP_TOK_DOT_DOT_DOT; /* "..." */
					goto set_result;
				}
#if TPP_HAVE_TPP_TOK_DOT_DOT
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
			}
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_DOT_DOT
			if (tpp_lexer_has(self, TPP_TOK_DOT_DOT)) {
				result = TPP_TOK_DOT_DOT; /* ".." */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
		} else
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT || TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_DOT_STAR
		if (ch2 == '*') {
			if (tpp_lexer_has(self, TPP_TOK_DOT_STAR)) {
				result = TPP_TOK_DOT_STAR; /* ".*" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '+': {
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL || TPP_HAVE_TPP_TOK_PLUS_PLUS
		if (!tpp_lexer_has(self, TPP_TOK_PLUS_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_PLUS_PLUS))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_PLUS_EQUAL)) {
				result = TPP_TOK_PLUS_EQUAL; /* "+=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
		if (ch2 == '+') {
			if (tpp_lexer_has(self, TPP_TOK_PLUS_PLUS)) {
				result = TPP_TOK_PLUS_PLUS; /* "++" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '-': {
#if (TPP_HAVE_TPP_TOK_MINUS_EQUAL ||       \
     TPP_HAVE_TPP_TOK_MINUS_MINUS ||       \
     TPP_HAVE_TPP_TOK_SQL_COMMENT ||       \
     TPP_HAVE_TPP_TOK_MINUS_RANGLE ||      \
     TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR || \
     TPP_HAVE_TPP_TOK_MINUS_LANGLE)
		if (!tpp_lexer_has(self, TPP_TOK_MINUS_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_MINUS) &&
		    !tpp_lexer_has(self, TPP_TOK_SQL_COMMENT) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_STAR) &&
		    !tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_MINUS_EQUAL)) {
				result = TPP_TOK_MINUS_EQUAL; /* "-=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS || TPP_HAVE_TPP_TOK_SQL_COMMENT
		if (ch2 == '-') {
#if TPP_HAVE_TPP_TOK_SQL_COMMENT
			if (tpp_lexer_has(self, TPP_TOK_SQL_COMMENT)) {
				error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SQL_COMMENT));
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_SQL_COMMENT; // "-- like this one!"
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
			if (tpp_lexer_has(self, TPP_TOK_MINUS_MINUS)) {
				result = TPP_TOK_MINUS_MINUS; /* "--" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
		} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS || TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
		if (ch2 == '>') {
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
			if (tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE_STAR)) {
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
				read_ch2();
				if (ch2 == '*') {
					result = TPP_TOK_MINUS_RANGLE_STAR; /* "->*" */
					goto set_result;
				}
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
			if (tpp_lexer_has(self, TPP_TOK_MINUS_RANGLE)) {
				result = TPP_TOK_MINUS_RANGLE; /* "->" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
		} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
		if (ch2 == '<') {
			if (tpp_lexer_has(self, TPP_TOK_MINUS_LANGLE)) {
				result = TPP_TOK_MINUS_LANGLE; /* "-<" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_LANGLE */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '*': {
#if (TPP_HAVE_TPP_TOK_STAR_EQUAL ||        \
     TPP_HAVE_TPP_TOK_STAR_STAR ||         \
     TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL ||   \
     TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS || \
     TPP_HAVE_TPP_TOK_STAR_DOT)
		if (!tpp_lexer_has(self, TPP_TOK_STAR_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_STAR_STAR) &&
		    !tpp_lexer_has(self, TPP_TOK_STAR_STAR_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_STAR_LANGLE_MINUS) &&
		    !tpp_lexer_has(self, TPP_TOK_STAR_DOT))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_STAR_EQUAL)) {
				result = TPP_TOK_STAR_EQUAL; /* "*=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR || TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
		if (ch2 == '*') {
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_STAR_STAR_EQUAL)) {
#if TPP_HAVE_TPP_TOK_STAR_STAR
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
				read_ch2();
				if (ch2 == '=') {
					result = TPP_TOK_STAR_STAR_EQUAL; /* "**=" */
					goto set_result;
				}
#if TPP_HAVE_TPP_TOK_STAR_STAR
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
			}
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR
			if (tpp_lexer_has(self, TPP_TOK_STAR_STAR)) {
				result = TPP_TOK_STAR_STAR; /* "**" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
		} else
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR || TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
		if (ch2 == '<') {
			if (tpp_lexer_has(self, TPP_TOK_STAR_LANGLE_MINUS)) {
				read_ch2();
				if (ch2 == '-') {
					result = TPP_TOK_STAR_LANGLE_MINUS; /* "*<-" */
					goto set_result;
				}
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_DOT
		if (ch2 == '.') {
			if (tpp_lexer_has(self, TPP_TOK_STAR_DOT)) {
				result = TPP_TOK_STAR_DOT; /* "*." */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '/': {
#if (TPP_HAVE_TPP_TOK_ASM_COMMENT || \
     TPP_HAVE_TPP_TOK_CXX_COMMENT || \
     TPP_HAVE_TPP_TOK_C_COMMENT ||   \
     TPP_HAVE_TPP_TOK_SLASH_EQUAL || \
     TPP_HAVE_TPP_TOK_SLASH_SLASH || \
     TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL)
#if TPP_HAVE_TPP_TOK_ASM_COMMENT
		tpp_size rel_end_of_1char;
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT */
		if (!tpp_lexer_has(self, TPP_TOK_CXX_COMMENT) &&
		    !tpp_lexer_has(self, TPP_TOK_C_COMMENT) &&
		    !tpp_lexer_has(self, TPP_TOK_SLASH_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_SLASH_SLASH) &&
		    !tpp_lexer_has(self, TPP_TOK_SLASH_SLASH_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_ASM_COMMENT))
			break;
#if TPP_HAVE_TPP_TOK_ASM_COMMENT
		rel_end_of_1char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT */
		read_ch2();

#if (TPP_HAVE_TPP_TOK_CXX_COMMENT || \
     TPP_HAVE_TPP_TOK_SLASH_SLASH || \
     TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL)
		if (ch2 == '/') {
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
			if (tpp_lexer_has(self, TPP_TOK_SLASH_SLASH_EQUAL)) {
#if TPP_HAVE_TPP_TOK_CXX_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH */
				read_ch2();
				if (ch2 == '=') {
					result = TPP_TOK_SLASH_SLASH_EQUAL; /* "//=" */
					goto set_result;
				}
#if TPP_HAVE_TPP_TOK_CXX_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT || TPP_HAVE_TPP_TOK_SLASH_SLASH */
			}
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_CXX_COMMENT
			if (tpp_lexer_has(self, TPP_TOK_CXX_COMMENT)) {
				error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_CXX_COMMENT));
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_CXX_COMMENT; // like this one!
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
			if (tpp_lexer_has(self, TPP_TOK_SLASH_SLASH)) {
				result = TPP_TOK_SLASH_SLASH; /* "//" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_C_COMMENT
		if (ch2 == '*') {
			if (tpp_lexer_has(self, TPP_TOK_C_COMMENT)) {
				for (;;) {
					read_ch2();
					if (ch2 == 0 && pos >= file->tf_end) {
#if TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
						error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
						                           TPP_W_COMMENT_TERMINATED_BY_EOF);
						if (TPP_ISERR(error))
							goto return_error;
#endif /* TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */
						break;
					}
					if (ch2 != '*') {
#if TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT
						if (ch2 == '/') {
							tpp_size slash_pos = tpp_file_ptr2rel(file, pos);
							read_ch2();
							if (ch2 != '*')
								continue;
							error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, slash_pos),
							                           TPP_W_SLASHSTAR_INSIDE_OF_COMMENT);
							if (TPP_ISERR(error))
								goto return_error;
						} else
#endif /* TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT */
						{
							continue;
						}
					}
					read_ch2();
					if (ch2 == '/')
						break;
				}
				result = TPP_TOK_C_COMMENT; /* like this one! */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_C_COMMENT */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_SLASH_EQUAL)) {
				result = TPP_TOK_SLASH_EQUAL; /* "/=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
		{
#if TPP_HAVE_TPP_TOK_ASM_COMMENT
			if (tpp_lexer_has(self, TPP_TOK_ASM_COMMENT)) {
				pos = tpp_file_rel2ptr(file, rel_end_of_1char);
				error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_ASM_COMMENT));
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_ASM_COMMENT; // "/ like this one!"
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT */
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '%': {
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL || TPP_HAVE_DIGRAPHS
		if (!tpp_lexer_has(self, TPP_TOK_PERCENT_EQUAL) &&
		    !tpp_lexer_has(self, DIGRAPHS))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_PERCENT_EQUAL)) {
				result = TPP_TOK_PERCENT_EQUAL; /* "%=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_DIGRAPHS
		if (ch2 == '>') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
				result = (tpp_token_id)'}'; /* "%>" -> "}" */
				goto set_result;
			}
		} else
		if (ch2 == ':') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
#if TPP_HAVE_TPP_TOK_POUND_POUND
				if (tpp_lexer_has(self, TPP_TOK_POUND_POUND)) {
					tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
					read_ch2();
					if (ch2 == '%') {
						read_ch2();
						if (ch2 == ':') {
							result = TPP_TOK_POUND_POUND; /* "%:%:" -> "##" */
							goto set_result;
						}
					}
					pos = tpp_file_rel2ptr(file, rel_end_of_2char);
				}
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
				result = (tpp_token_id)'#'; /* "%:" -> "#" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_DIGRAPHS */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '&': {
#if TPP_HAVE_TPP_TOK_AMP_EQUAL || TPP_HAVE_TPP_TOK_AMP_AMP
		if (!tpp_lexer_has(self, TPP_TOK_AMP_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_AMP_AMP))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_AMP_EQUAL)) {
				result = TPP_TOK_AMP_EQUAL; /* "&=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_AMP
		if (ch2 == '&') {
			if (tpp_lexer_has(self, TPP_TOK_AMP_AMP)) {
				result = TPP_TOK_AMP_AMP; /* "&&" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '|': {
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL || TPP_HAVE_TPP_TOK_PIPE_PIPE
		if (!tpp_lexer_has(self, TPP_TOK_PIPE_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_PIPE_PIPE))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_PIPE_EQUAL)) {
				result = TPP_TOK_PIPE_EQUAL; /* "|=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
		if (ch2 == '|') {
			if (tpp_lexer_has(self, TPP_TOK_PIPE_PIPE)) {
				warn_if_ch2_is_trigraph();  /* "??!" -> "|" */
				result = TPP_TOK_PIPE_PIPE; /* "||" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '^':{
#if TPP_HAVE_TPP_TOK_HAT_EQUAL || TPP_HAVE_TPP_TOK_HAT_HAT
		if (!tpp_lexer_has(self, TPP_TOK_HAT_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_HAT_HAT))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_HAT_EQUAL)) {
				result = TPP_TOK_HAT_EQUAL; /* "^=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_HAT
		if (ch2 == '^') {
			if (tpp_lexer_has(self, TPP_TOK_HAT_HAT)) {
				warn_if_ch2_is_trigraph(); /* "??'" -> "^" */
				result = TPP_TOK_HAT_HAT; /* "^^" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '@': {
#if TPP_HAVE_TPP_TOK_AT_EQUAL
		if (tpp_lexer_has(self, TPP_TOK_AT_EQUAL)) {
			read_ch2();
			if (ch2 == '=') {
				result = TPP_TOK_AT_EQUAL; /* "@=" */
				goto set_result;
			}
		}
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
	}	break;
/************************************************************************/



/************************************************************************/
	case '#': {
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT || TPP_HAVE_TPP_TOK_POUND_POUND
#if TPP_HAVE_TPP_TOK_POUND_POUND
		if (tpp_lexer_has(self, TPP_TOK_POUND_POUND)) {
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
			tpp_size rel_end_of_1char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
			read_ch2();
			if (ch2 == '#') {
				warn_if_ch2_is_trigraph();    /* "??=" -> "#" */
				result = TPP_TOK_POUND_POUND; /* "##" */
				goto set_result;
			}
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
			pos = tpp_file_rel2ptr(file, rel_end_of_1char);
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		}
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (tpp_lexer_has(self, TPP_TOK_SHELL_COMMENT)) {
			error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SHELL_COMMENT));
			if (TPP_ISERR(error))
				goto return_error;
			result = TPP_TOK_SHELL_COMMENT; // "# like this one!"
			goto set_result;
		}
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '~': {
#if TPP_HAVE_TPP_TOK_TILDE_TILDE || TPP_HAVE_TPP_TOK_TILDE_EQUAL
		if (!tpp_lexer_has(self, TPP_TOK_TILDE_TILDE) &&
		    !tpp_lexer_has(self, TPP_TOK_TILDE_EQUAL))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
		if (ch2 == '~') {
			if (tpp_lexer_has(self, TPP_TOK_TILDE_TILDE)) {
				warn_if_ch2_is_trigraph();    /* "??-" -> "~" */
				result = TPP_TOK_TILDE_TILDE; /* "~~" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_TILDE_EQUAL)) {
				result = TPP_TOK_TILDE_EQUAL; /* "~=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case ':': {
#if TPP_HAVE_TPP_TOK_COLON_EQUAL || TPP_HAVE_TPP_TOK_COLON_COLON || TPP_HAVE_DIGRAPHS
		if (!tpp_lexer_has(self, TPP_TOK_COLON_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_COLON_COLON) &&
		    !tpp_lexer_has(self, DIGRAPHS))
			break;
		read_ch2();
#if TPP_HAVE_DIGRAPHS
		if (ch2 == '>') {
			if (tpp_lexer_has(self, DIGRAPHS)) {
				result = (tpp_token_id)']'; /* ":>" -> "]" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_DIGRAPHS */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_COLON_EQUAL)) {
				result = TPP_TOK_COLON_EQUAL; /* ":=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
		if (ch2 == ':') {
			if (tpp_lexer_has(self, TPP_TOK_COLON_COLON)) {
				result = TPP_TOK_COLON_COLON; /* "::" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '?': {
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL || TPP_HAVE_TPP_TOK_QMARK_QMARK || TPP_HAVE_TRIGRAPHS
#if TPP_HAVE_TRIGRAPHS
		if (tpp_lexer_has(self, TRIGRAPHS)) {
			if (pos >= file->tf_end) {
				error = tpp_file_expandchunk(file);
				if (TPP_ISERR(error))
					goto return_error;
				pos = tpp_file_rel2ptr(file, rel_start + 1);
			}
			if (pos < file->tf_end && *pos == '?') {
				if ((pos + 1) >= file->tf_end) {
					error = tpp_file_expandchunk(file);
					if (TPP_ISERR(error))
						goto return_error;
					pos = tpp_file_rel2ptr(file, rel_start + 1);
				}
				if ((pos + 1) < file->tf_end) {
					pos += 2;
					switch (pos[-1]) {
					case '=': ch = '#'; break;
					case '(': ch = '['; break;
					case '/': ch = '\\'; break;
					case ')': ch = ']'; break;
					case '\'': ch = '^'; break;
					case '<': ch = '{'; break;
					case '!': ch = '|'; break;
					case '>': ch = '}'; break;
					case '-': ch = '~'; break;
					case '?': break; /* Continue with '?'-case */
					default:
						/* Not actually a trigraph */
						pos -= 2;
						goto not_a_trigraph;
					}
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
					error = tpp_lexer_warnf_at(self, pos - 3, TPP_W_ENCOUNTERED_TRIGRAPH);
					if (TPP_ISERR(error))
						goto return_error;
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
					if (ch != '?')
						goto switch_on_ch;
					/* Continue with '?'-case */
				}
			}
		}
not_a_trigraph:
#endif /* TPP_HAVE_TRIGRAPHS */

		if (!tpp_lexer_has(self, TPP_TOK_QMARK_EQUAL) &&
		    !tpp_lexer_has(self, TPP_TOK_QMARK_QMARK))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_has(self, TPP_TOK_QMARK_EQUAL)) {
				result = TPP_TOK_QMARK_EQUAL; /* "?=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
		if (ch2 == '?') {
			if (tpp_lexer_has(self, TPP_TOK_QMARK_QMARK)) {
				warn_if_ch2_is_trigraph();    /* "???" -> "?" */
				result = TPP_TOK_QMARK_QMARK; /* "??" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '(': {
#if TPP_HAVE_TPP_TOK_PASCAL_COMMENT
		if (!tpp_lexer_has(self, TPP_TOK_PASCAL_COMMENT))
			break;
		read_ch2(); // "(*like this one!*)"
		if (ch2 != '*')
			break;
		for (;;) {
			read_ch2();
			if (ch2 == 0 && pos >= file->tf_end) {
#if TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
				error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
				                           TPP_W_COMMENT_TERMINATED_BY_EOF);
				if (TPP_ISERR(error))
					goto return_error;
#endif /* TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */
				break;
			}
			if (ch2 != '*')
				continue;
			read_ch2();
			if (ch2 == ')')
				break;
		}
		result = TPP_TOK_PASCAL_COMMENT;
		goto set_result;
#endif /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT */
	}	break;
/************************************************************************/



/************************************************************************/
	case '\\': {
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
		if (tpp_lexer_has(self, ESCAPE_IN_IDENTIFIERS)) {
			tpp_char const *npos;
			tpp_size rel_before, rel_after;
			npos = tpp_file_rel2ptr(file, rel_start);
			rel_before = tpp_file_ptr2rel(file, npos);
			error = tpp_lexer_skip_bsi(self, &npos);
			if (TPP_ISERR(error))
				goto return_error;
			rel_after = tpp_file_ptr2rel(file, npos);
			tpp_assert(rel_before <= rel_after);
			if (rel_before < rel_after) {
				pos = npos;
				goto handle_keyword_with_esc;
#define WANT_handle_keyword_with_esc
			}
		}
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
		{
#if TPP_HAVE_BSE
			tpp_char const *npos;
			tpp_size rel_before, rel_after;
			npos = tpp_file_rel2ptr(file, rel_start);
			rel_before = tpp_file_ptr2rel(file, npos);
			error = tpp_lexer_skip_bse(self, &npos);
			if (TPP_ISERR(error))
				goto return_error;
			rel_after = tpp_file_ptr2rel(file, npos);
			tpp_assert(rel_before <= rel_after);
			if (rel_before >= rel_after)
				break; /* No BSE -> regular backslash */
	
			/* BSE was skipped -> read whatever comes after... */
			*p_pos = npos;
			goto again;
#endif /* TPP_HAVE_BSE */
		}
	}	break;
/************************************************************************/



/************************************************************************/
	case '\'': {
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL || TPP_HAVE_TPP_TOK_CHAR
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
		if (tpp_lexer_has(self, TPP_TOK_BLOCK_CHAR_LITERAL)) {
#if TPP_HAVE_TPP_TOK_CHAR
			tpp_size rel_end_of_1char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_CHAR */
			read_ch2();
			if (ch2 == '\'') {
				read_ch2();
				if (ch2 == '\'') {
					error = tpp_lexer_seek_end_of_block_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_BLOCK_CHAR_LITERAL; /* '''foo''' */
					goto set_result;
				}
			}
#if TPP_HAVE_TPP_TOK_CHAR
			pos = tpp_file_rel2ptr(file, rel_end_of_1char);
#endif /* TPP_HAVE_TPP_TOK_CHAR */
		}
#endif /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_CHAR
		if (tpp_lexer_has(self, TPP_TOK_CHAR)) {
			error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
			if (TPP_ISERR(error))
				goto return_error;
			result = TPP_TOK_CHAR; /* 'foo' */
			goto set_result;
		}
#endif /* TPP_HAVE_TPP_TOK_CHAR */
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '"': {
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRING
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
		if (tpp_lexer_has(self, TPP_TOK_BLOCK_STRING_LITERAL)) {
#if TPP_HAVE_TPP_TOK_STRING
			tpp_size rel_end_of_1char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_STRING */
			read_ch2();
			if (ch2 == '"') {
				read_ch2();
				if (ch2 == '"') {
					error = tpp_lexer_seek_end_of_block_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_BLOCK_STRING_LITERAL; /* """foo""" */
					goto set_result;
				}
			}
#if TPP_HAVE_TPP_TOK_STRING
			pos = tpp_file_rel2ptr(file, rel_end_of_1char);
#endif /* TPP_HAVE_TPP_TOK_STRING */
		}
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRING
		if (tpp_lexer_has(self, TPP_TOK_STRING)) {
			error = tpp_lexer_seek_end_of_string(self, &pos, '"');
			if (TPP_ISERR(error))
				goto return_error;
			result = TPP_TOK_STRING; /* "foo" */
			goto set_result;
		}
#endif /* TPP_HAVE_TPP_TOK_STRING */
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||     \
     TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
	case 'R': {
		if (tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_RAW_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_RAW_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
			if (ch2 == '"') {
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
				if (tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_RAW_STRING_LITERAL; /* R"AB(foo)AB" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
				if (tpp_lexer_has(self, TPP_TOK_RAW_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_STRING_LITERAL; /* R"foo" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
			if (ch2 == '\'') {
#if TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
				if (tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_RAW_CHAR_LITERAL; /* R'AB(f)AB' */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
				if (tpp_lexer_has(self, TPP_TOK_RAW_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_CHAR_LITERAL; /* R'foo' */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
	case 'r': {
		if (tpp_lexer_has(self, TPP_TOK_RAW_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_RAW_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_has(self, TPP_TOK_RAW_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_STRING_LITERAL; /* r"foo"  */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
			if (ch2 == '\'') {
				if (tpp_lexer_has(self, TPP_TOK_RAW_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_CHAR_LITERAL; /* r'foo' */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	case 'L': {
		if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_WIDE_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_WIDE_STRING_LITERAL; /* L"foo" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
			if (ch2 == '\'') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_WIDE_CHAR_LITERAL; /* L'f' */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
#if ((TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || \
     (TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL))
			if (ch2 == 'R') {
				if ((tpp_lexer_has(self, TPP_TOK_CXX_WIDE_STRING_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) ||
				    (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_CHAR_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL))) {
					read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_STRING_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL; /* LR"AB(foo)AB" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_WIDE_CHAR_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL; /* LR'AB(f)AB' */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
					{
					}
				}
			} else
#endif /* ... */
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL)
	case 'u': {
		if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_UTF16_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_UTF16_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
			if (ch2 == '8') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL) ||
				    tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL)) {
					read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL)) {
							error = tpp_lexer_seek_end_of_string(self, &pos, '"');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_UTF8_STRING_LITERAL; /* u8"foo" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
					if (ch2 == '\'') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL)) {
							error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_UTF8_CHAR_LITERAL; /* u8'f' */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if ((TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || \
     (TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL))
					if (ch2 == 'R') {
						if ((tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL) &&
						     tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) ||
						    (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL) &&
						     tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL))) {
							read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
							if (ch2 == '"') {
								if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_STRING_LITERAL) &&
								    tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
									error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
									if (TPP_ISERR(error))
										goto return_error;
									result = TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL; /* u8R"AB(foo)AB" */
									goto set_result;
								}
							} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
							if (ch2 == '"') {
								if (tpp_lexer_has(self, TPP_TOK_CXX_UTF8_CHAR_LITERAL) &&
								    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
									error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
									if (TPP_ISERR(error))
										goto return_error;
									result = TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL; /* u8R'AB(f)AB' */
									goto set_result;
								}
							} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
							{
							}
						}
					} else
#endif /* ... */
					{
					}
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_UTF16_STRING_LITERAL; /* u"foo" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
			if (ch2 == '\'') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_UTF16_CHAR_LITERAL; /* u'f' */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
#if ((TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || \
     (TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL))
			if (ch2 == 'R') {
				if ((tpp_lexer_has(self, TPP_TOK_CXX_UTF16_STRING_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) ||
				    (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_CHAR_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL))) {
					read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_STRING_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL; /* uR"AB(foo)AB" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF16_CHAR_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL; /* uR'AB(f)AB' */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
					{
					}
				}
			} else
#endif /* ... */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
	case 'U': {
		if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_STRING_LITERAL) ||
		    tpp_lexer_has(self, TPP_TOK_CXX_UTF32_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_UTF32_STRING_LITERAL; /* U"foo" */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
			if (ch2 == '\'') {
				if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_UTF32_CHAR_LITERAL; /* U'f' */
					goto set_result;
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
#if ((TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || \
     (TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL))
			if (ch2 == 'R') {
				if ((tpp_lexer_has(self, TPP_TOK_CXX_UTF32_STRING_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) ||
				    (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_CHAR_LITERAL) &&
				     tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL))) {
					read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_STRING_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_STRING_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL; /* UR"AB(foo)AB" */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
					if (ch2 == '"') {
						if (tpp_lexer_has(self, TPP_TOK_CXX_UTF32_CHAR_LITERAL) &&
						    tpp_lexer_has(self, TPP_TOK_CXX_RAW_CHAR_LITERAL)) {
							error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
							if (TPP_ISERR(error))
								goto return_error;
							result = TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL; /* UR'AB(f)AB' */
							goto set_result;
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
					{
					}
				}
			} else
#endif /* ... */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_DOLLAR
	case '$':
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_DOLLAR)
		if (tpp_lexer_has(self, TPP_TOK_DOLLAR))
			break; /* Follow single-char code-branch */
		goto handle_keyword;
#define WANT_handle_keyword
#else /* TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_DOLLAR) */
		break; /* Follow single-char code-branch */
#endif /* !TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_DOLLAR) */
#endif /* TPP_HAVE_TPP_TOK_DOLLAR */
/************************************************************************/

	default: {
#if TPP_HAVE_UNICODE
		/* Check for utf-8 sequence */
		if (tpp_file_isutf8(file) && ch >= 0x80) {
			tpp_unichar uc;
			--pos;
			error = tpp_lexer_readutf8(self, &pos, &uc);
			if (TPP_ISERR(error))
				goto return_error;
			if tpp_unlikely(uc == 0 && !tpp_file_isutf8(file)) {
				++pos; /* Malformed utf-8 sequence caused unicode to be disabled */
			} else {
				/* Handle unicode character traits */
				if (tpp_unicode_islf(uc))
					goto do_set_linefeed;
				if (tpp_unicode_isspace(uc))
					goto handle_space;
				if (tpp_unicode_issymstrt(uc))
					goto handle_keyword;
#define WANT_handle_keyword
#if TPP_HAVE_TPP_TOK_INT || TPP_HAVE_TPP_TOK_FLOAT
				if (tpp_unicode_isdigit(uc))
					goto handle_digit;
#endif /* TPP_HAVE_TPP_TOK_INT || TPP_HAVE_TPP_TOK_FLOAT */
				result = TPP_TOK_UNICHAR;
				goto set_result;
			}
		}
#endif /* TPP_HAVE_UNICODE */

		if (tpp_ascii_islf(ch)) {
			/* Deal with \r\n linefeed sequences */
			if (ch == '\r') {
				if tpp_unlikely(pos >= end) {
					error = tpp_file_expandchunk(file);
					if (TPP_ISERR(error))
						goto return_error;
					pos = tpp_file_rel2ptr(file, rel_start + 1);
					end = file->tf_end;
					if (pos >= end)
						goto do_set_linefeed; /* \r followed by EOF */
				}
				if (*pos == '\n')
					++pos; /* 2-byte linefeed sequence */
			}
do_set_linefeed:
			result = TPP_TOK_LF;
			goto set_result;
		}

		/* Check for space */
		if (tpp_ascii_isspace(ch)) {
#if TPP_HAVE_UNICODE
handle_space:
#endif /* TPP_HAVE_UNICODE */
			error = tpp_lexer_skipspace_nolf(self, &pos);
			if (TPP_ISERR(error))
				goto return_error;
#if TPP_HAVE_BSE
			while (pos < file->tf_end && *pos == '\\') {
				tpp_char const *npos;
				tpp_size rel_before, rel_after;
				npos = pos;
				error = tpp_lexer_skip_bse(self, &npos);
				if (TPP_ISERR(error))
					goto return_error;
				if (npos == pos)
					break;
				rel_before = tpp_file_ptr2rel(file, npos);
				error = tpp_lexer_skipspace_nolf(self, &npos);
				if (TPP_ISERR(error))
					goto return_error;
				rel_after  = tpp_file_ptr2rel(file, npos);
				tpp_assert(rel_before <= rel_after);
				if (rel_before >= rel_after)
					break;
				pos = npos;
			}
#endif /* TPP_HAVE_BSE */

			result = TPP_TOK_SPACE;
			goto set_result;
		}

		/* Check for keywords... */
		if (tpp_ascii_issymstrt(ch)) {
			tpp_char const *kwd_start;
			tpp_size kwd_len;
			tpp_hash kwd_hash;
			tpp_keyword const *kwd;
#if TPP_HAVE_ESCAPED_KEYWORDS
			bool uses_esc;
			tpp_size rel_kwd_end;
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */
#ifdef WANT_handle_keyword
#undef WANT_handle_keyword
handle_keyword:
#endif /* WANT_handle_keyword */
#if TPP_HAVE_ESCAPED_KEYWORDS
			uses_esc = false;
#ifdef WANT_handle_keyword_with_esc
#undef WANT_handle_keyword_with_esc
			if (0) {
handle_keyword_with_esc:
				uses_esc = true;
			}
#endif /* WANT_handle_keyword_with_esc */
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */
			error = tpp_lexer_seek_end_of_keyword(self, &pos);
			if (TPP_ISERR(error))
				goto return_error;
#if TPP_HAVE_ESCAPED_KEYWORDS
			rel_kwd_end = tpp_file_ptr2rel(file, pos);
			for (;;) {
				if (pos >= file->tf_end) {
					tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_file_expandchunk(file);
					if (TPP_ISERR(error))
						goto return_error;
					pos = tpp_file_rel2ptr(file, rel_pos);
					if (pos >= file->tf_end)
						break;
				}
				if (*pos == '\\') {
					/* Backslash */
				} else
#if TPP_HAVE_TRIGRAPHS
				if (*pos == '?' && tpp_lexer_has(self, TRIGRAPHS)) {
					if ((pos + 1) >= file->tf_end) {
						tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
						error = tpp_file_expandchunk(file);
						if (TPP_ISERR(error))
							goto return_error;
						pos = tpp_file_rel2ptr(file, rel_pos);
					}
					if ((pos + 1) < file->tf_end && pos[1] == '?') {
						if ((pos + 2) >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							if (TPP_ISERR(error))
								goto return_error;
							pos = tpp_file_rel2ptr(file, rel_pos);
						}
						if ((pos + 2) < file->tf_end && pos[2] == '/') {
							/* Trigraph backslash */
						} else {
							break;
						}
					} else {
						break;
					}
				} else
#endif /* TPP_HAVE_TRIGRAPHS */
				{
					break; /* No backslash */
				}
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
				if (tpp_lexer_has(self, ESCAPE_IN_IDENTIFIERS)) {
					tpp_char const *npos = pos;
					tpp_size rel_pos;
					rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_lexer_skip_bsi(self, &npos);
					pos = tpp_file_rel2ptr(file, rel_pos);
					if (TPP_ISERR(error))
						goto return_error;
					tpp_assert(pos <= npos);
					if (pos < npos) {
						pos = npos;
						uses_esc = true;
						error = tpp_lexer_seek_end_of_keyword(self, &pos);
						if (TPP_ISERR(error))
							goto return_error;
						rel_kwd_end = tpp_file_ptr2rel(file, pos);
						continue;
					}
				}
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
				{
#if TPP_HAVE_BSE
					tpp_char const *npos = pos;
					tpp_size rel_pos;
					rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_lexer_skip_bse(self, &npos);
					pos = tpp_file_rel2ptr(file, rel_pos);
					if (TPP_ISERR(error))
						goto return_error;
					tpp_assert(pos <= npos);
					if (pos < npos) {
						pos = npos;
						rel_pos = tpp_file_ptr2rel(file, pos);
						error = tpp_lexer_seek_end_of_keyword(self, &npos);
						pos = tpp_file_rel2ptr(file, rel_pos);
						if (TPP_ISERR(error))
							goto return_error;
						tpp_assert(pos <= npos);
						if (pos < npos) {
							pos = npos;
							uses_esc = true;
							rel_kwd_end = tpp_file_ptr2rel(file, pos);
						} else {
							/* Still continue in case there's more to be found... */
#if !TPP_HAVE_ESCAPE_IN_IDENTIFIERS
							break; /* No need to continue without \u-identifiers */
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
						}
						continue;
					}
#endif /* TPP_HAVE_BSE */
				}
				break;
			}
			pos = tpp_file_rel2ptr(file, rel_kwd_end);
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */

			/* Lookup/create keyword */
			kwd_start = tpp_file_rel2ptr(file, rel_start);
			kwd_len   = (tpp_size)(pos - kwd_start);
#if TPP_HAVE_ESCAPED_KEYWORDS
			if (uses_esc) {
				kwd_hash = tpp_hashof_esc(kwd_start, kwd_len, file);
				kwd = tpp_keywords_newkeyword_esc(&self->tl_kwds, kwd_start, kwd_len, kwd_hash, file);
			} else
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */
			{
				kwd_hash = tpp_hashof(kwd_start, kwd_len);
				kwd = tpp_keywords_newkeyword(&self->tl_kwds, kwd_start, kwd_len, kwd_hash);
			}
			if tpp_unlikely(!kwd) {
				error = TPP_ENOMEM;
				goto return_error;
			}
			token->tt_kwd = kwd;
			result = kwd->tk_id;
			goto set_result;
		}

		/* Check for digits */
#if TPP_HAVE_TPP_TOK_INT || TPP_HAVE_TPP_TOK_FLOAT
		if (tpp_ascii_isdigit(ch) && (tpp_lexer_has(self, TPP_TOK_FLOAT) ||
		                              tpp_lexer_has(self, TPP_TOK_INT))) {
#if TPP_HAVE_UNICODE
handle_digit:
#endif /* TPP_HAVE_UNICODE */
			error = tpp_lexer_seek_end_of_keyword(self, &pos);
			if (TPP_ISERR(error))
				goto return_error;
#if TPP_HAVE_BSE
			error = tpp_lexer_skip_bse_after_keyword(self, &pos);
			if (TPP_ISERR(error))
				goto return_error;
#endif /* TPP_HAVE_BSE */

#if TPP_HAVE_TPP_TOK_FLOAT
			if (pos < file->tf_end && *pos == '.' &&
			    tpp_lexer_has(self, TPP_TOK_FLOAT)) {
				/* Floating point token... */
				++pos;
				error = tpp_lexer_seek_end_of_keyword(self, &pos);
				if (TPP_ISERR(error))
					goto return_error;
#if TPP_HAVE_BSE
				error = tpp_lexer_skip_bse_after_keyword(self, &pos);
				if (TPP_ISERR(error))
					goto return_error;
#endif /* TPP_HAVE_BSE */
				/* TODO: 1.0E+1 */
				/* TODO: 1E+1 */
				/* TODO: .1E+1 */
				result = TPP_TOK_FLOAT;
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_FLOAT */

#if TPP_HAVE_TPP_TOK_INT
			if (tpp_lexer_has(self, TPP_TOK_INT)) {
				result = TPP_TOK_INT;
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_INT */
		}
#endif /* TPP_HAVE_TPP_TOK_INT || TPP_HAVE_TPP_TOK_FLOAT */

	}	break;

	}

	/* Fallback: single-character token */
	result = (tpp_token_id)ch;
	pos    = tpp_file_rel2ptr(file, rel_start + 1);
set_result:
	token->tt_id    = result;
	token->tt_start = tpp_file_rel2ptr(file, rel_start);
	*p_pos = pos; /* This also updates "file->tf_pos" (if "p_pos == &token->tt_end") */
	return result;
eof:
	/* Check if we can read some more data from the file */
	rel_start = tpp_file_ptr2rel(file, pos);
	error = tpp_file_expandchunk(file);
	if (TPP_ISERR(error))
		goto return_error;
	pos = tpp_file_rel2ptr(file, rel_start);
	end = file->tf_end;
	if (pos < end)
		goto again_read_from_pos;

	/* Check if we can pop to another file */
#if TPP_HAVE_INCLUDE_STACK
	if (file->tf_prev && p_pos == &file->tf_pos) {
		/* Warn if the file still has an active #ifdef-stack
		 * Only do this when we're actually going to pop the
		 * file off the #include-stack. In those cases where
		 * we're not allowed to do so, it's up to the caller
		 * to emit these sorts of warnings! */
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
		error = tpp_lexer_warn_nonempty_ifdef(self);
		if (TPP_ISERR(error))
			goto return_error;
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */

		/* Actually pop the file! */
		tpp_lexer_popfile(self);
		goto again;
	}
#endif /* TPP_HAVE_INCLUDE_STACK */

	/* EOF reached */
	tpp_assert(pos == end);
	token->tt_id    = TPP_TOK_EOF;
	token->tt_start = pos;
	*p_pos = pos;
	return TPP_TOK_EOF;

return_error:
	/* Fix the caller's `p_pos' (unless it's the one from the file) */
	if (p_pos != &file->tf_pos)
		*p_pos = tpp_file_rel2ptr(file, rel_start);
	return TPP_TOK_OFERR(error);
#undef read_ch2
#undef warn_if_ch2_is_trigraph
#undef NEED_read_ch2
}

#undef NEED_tpp_lexer_seek_eol
#undef NEED_tpp_lexer_seek_end_of_string
#undef NEED_tpp_lexer_seek_end_of_block_string
#undef NEED_tpp_lexer_seek_end_of_cxx_raw_string
#undef NEED_tpp_lexer_seek_end_of_raw_string

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-seek-rparen.c                                      */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_SEEKPP_RPAREN

#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
#define TPP_RECURSION_CC_PAREN   0
#define TPP_RECURSION_CC_BRACKET 1
#define TPP_RECURSION_CC_BRACE   2
#define TPP_RECURSION_CC_ANGLE   3

static TPP_CONSTCALL TPP_WUNUSED unsigned int TPPCALL
tpp_get_recursion_cc(tpp_token_id lparen_kind) {
	switch (lparen_kind) {
	case TPP_TOK_OFCHAR('('):
		return TPP_RECURSION_CC_PAREN;
	case TPP_TOK_OFCHAR('['):
		return TPP_RECURSION_CC_BRACKET;
	case TPP_TOK_OFCHAR('{'):
		return TPP_RECURSION_CC_BRACE;
	case TPP_TOK_OFCHAR('<'):
		return TPP_RECURSION_CC_ANGLE;
	default: tpp_unreachable();
	}
}
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
#define TPP_RECURSION_CC_PAREN 0
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */


typedef struct tpp_seek_rparen_state {
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	/* Recursively nested ( ), [ ], { }, < > -pairs (in that order)
	 * The recursion level at any index can only be altered if all
	 * recursion levels with lower indices are set to 0:
	 * >> #define ARR[x] #x
	 * >> ARR[foo(])bar([)baz]
	 * Expands to 1 token:  "foo(])bar([)baz"
	 * rather than:         "foo("   )   bar   (   [   )   baz   ]   ) */
	tpp_ssize    tsrps_recursion[4];
	unsigned int tsrps_cc;
#define tpp_seek_rparen_state_init(self, lparen_kind) \
	(void)((self)->tsrps_recursion[0] = 0,            \
	       (self)->tsrps_recursion[1] = 0,            \
	       (self)->tsrps_recursion[2] = 0,            \
	       (self)->tsrps_recursion[3] = 0,            \
	       (self)->tsrps_cc = tpp_get_recursion_cc(lparen_kind))
#define tpp_seek_rparen_state_getcc(self) ((self)->tsrps_cc)
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	tpp_ssize tsrps_recursion[1]; /* Recursively nested ( )-pairs */
#define tpp_seek_rparen_state_init(self, lparen_kind) \
	(void)((self)->tsrps_recursion[0] = 0)
#define tpp_seek_rparen_state_getcc(self) TPP_RECURSION_CC_PAREN
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */

#if TPP_HAVE_INCLUDE_STACK
	tpp_file       *tsrps_curfile_saved_prev;     /* [0..1] Saved "tf_prev" of current file */
#endif /* TPP_HAVE_INCLUDE_STACK */
	tpp_size        tsrps_curfile_saved_keep;     /* Saved "ttf_keep" of current file, relative to current "ttf_keep", or (tpp_size)-1 if there was no keep-pointer */
	tpp_size        tsrps_curfile_saved_tpos_rel; /* Saved "tf_tpos" of current file, relative to current "ttf_keep" */
	tpp_size        tsrps_curfile_saved_pos_rel;  /* Saved "tf_pos" of current file, relative to current "ttf_keep" */

	/* Already-parsed text that must be prepended before the current argument. */

	/* TODO: Come up with a smart way of tracking debug info for custom printed arguments
	 *       -> need to be able to track lcinfo for custom char ranges (any range of chars
	 *          from this string must be able to map to its own file/line/col triple)
	 *       -> also must adjust tpp_file_lcinfo() to support this, and somehow also
	 *          incorporate tpp_file_filename()/tpp_file_userfilename() to support
	 *          different filenames based on char position
	 * where this is necessary:
	 * >> #define foo(a) a a
	 * >> foo(
	 * >> #include "file1.txt"   // Contains 10
	 * >> #include "file2.txt"   // Contains 20
	 * >> )
	 *
	 * Must result in 4 tokens (not accounting for whitespace/linefeed tokens):
	 * - file1.txt:1:1: 10
	 * - file2.txt:1:1: 20
	 * - file1.txt:1:1: 10
	 * - file2.txt:1:1: 20
	 */
	tpp_string_builder tsrps_curarg_prefix;
#define tpp_seek_rparen_state_init_curarg(self) \
	tpp_string_builder_init(&(self)->tsrps_curarg_prefix)
#define tpp_seek_rparen_state_fini_curarg(self)               \
	tpp_string_builder_fini(&(self)->tsrps_curarg_prefix)
#define tpp_seek_rparen_state_curarg_append(self, data, size) \
	(tpp_string_builder_print(&(self)->tsrps_curarg_prefix, data, size) >= 0)
} tpp_seek_rparen_state;


TPP_INLINE TPP_NONNULL((1, 2)) void TPPCALL
tpp_seek_rparen_state_save_curfile(tpp_seek_rparen_state *tpp_restrict self,
                                   tpp_lexer *tpp_restrict lexer) {
	tpp_file *const file = tpp_lexer_getfile(lexer);
	tpp_char const *keep = file->tf_data.td_io.ttf_keep;
	tpp_char const *tpos = file->tf_tpos;
	if (!keep) {
		self->tsrps_curfile_saved_keep = (tpp_size)-1;
		file->tf_data.td_io.ttf_keep   = tpos;
		keep = tpos;
	} else if (keep <= tpos) {
		self->tsrps_curfile_saved_keep = 0;
	} else {
		self->tsrps_curfile_saved_keep = (tpp_size)(keep - tpos);
		file->tf_data.td_io.ttf_keep   = tpos;
		keep = tpos;
	}
	self->tsrps_curfile_saved_tpos_rel = (tpp_size)(tpos - keep);
	self->tsrps_curfile_saved_pos_rel = (tpp_size)(file->tf_pos - keep);
#if TPP_HAVE_INCLUDE_STACK
	self->tsrps_curfile_saved_prev = file->tf_prev;
	file->tf_prev = NULL;
#endif /* TPP_HAVE_INCLUDE_STACK */
}

TPP_INLINE TPP_NONNULL((1, 2)) void TPPCALL
tpp_seek_rparen_state_rstr_curfile(tpp_seek_rparen_state *tpp_restrict self,
                                   tpp_lexer *tpp_restrict lexer,
                                   bool restore_pos) {
	tpp_file *const file = tpp_lexer_getfile(lexer);
#if TPP_HAVE_INCLUDE_STACK
	file->tf_prev = self->tsrps_curfile_saved_prev;
#endif /* TPP_HAVE_INCLUDE_STACK */
	file->tf_tpos = tpp_file_keep_rel2ptr(file, self->tsrps_curfile_saved_tpos_rel);
	if (restore_pos)
		file->tf_pos = tpp_file_keep_rel2ptr(file, self->tsrps_curfile_saved_pos_rel);
	if (self->tsrps_curfile_saved_keep != (tpp_size)-1) {
		file->tf_data.td_io.ttf_keep += self->tsrps_curfile_saved_keep;
	} else {
		file->tf_data.td_io.ttf_keep = NULL;
	}
}

#define tpp_lexer_arginfo_isrelative(self) \
	((self)->tlai_chunk == (TPP_REF tpp_string *)-1)
#define tpp_lexer_arginfo_rel2ptr(self, file)                                              \
	(void)((self)->tlai_start = tpp_file_keep_rel2ptr(file, (tpp_size)(self)->tlai_start), \
	       (self)->tlai_end   = tpp_file_keep_rel2ptr(file, (tpp_size)(self)->tlai_end))
#define tpp_lexer_arginfo_ptr2rel(self, file)                                                      \
	(void)((self)->tlai_start = (tpp_char const *)tpp_file_keep_ptr2rel(file, (self)->tlai_start), \
	       (self)->tlai_end   = (tpp_char const *)tpp_file_keep_ptr2rel(file, (self)->tlai_end))


/* Seek the first unmatched ')'-token, whilst collecting information
 * about every ','-separated text-area encountered until then.
 *
 * NOTES:
 *  - This function is used to parse the argument list for user-defined
 *    macros, as well as a couple of built-in macros.
 *  - This function preserves the effective "tf_tpos" (aka. tpp_token_getstart())
 *    of the final output file (and when using "tpp_lexer_manualpopfile_start":
 *    all intermediate popped files also)
 *
 * @param: p_argv: [out]    Output buffer for the bounds of macro
 *                          arguments encountered along the way.
 *                          The size of this buffer is IN(*p_argc)
 * @param: p_argc: [in/out] In:  Size of provided "p_argv" buffer (in elements)
 *                          Out: Number of arguments actually encountered. May
 *                               be set to a number greater tha IN(*p_argc), in
 *                               which case the last argument (IN(*p_argc) - 1)
 *                               is treated as a varargs argument. Unless the
 *                               `TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS' flag is
 *                               given in this case, this also causes a warning
 *                               to be emitted.
 * @param: p_rollback_pos:  [out] Set to the position that the current file should
 *                                be rewound to for the sake of performing a rollback
 * @param: opt_function_name_for_messages:
 *                          Function name for warning messages
 * @param: flags:           Set of `TPP_LEXER_SEEK_RPAREN_FLAG_*'
 *
 * @return: TPP_TOK_EOF:         EOF was encountered before an unmatched ')' was found
 * @return: TPP_TOK_RPAREN:      Unmatched closing ')' was encountered
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen_ex(tpp_lexer *tpp_restrict self,
                           tpp_lexer_arginfo *tpp_restrict p_argv,
                           tpp_size *tpp_restrict p_argc,
                           tpp_char const **p_rollback_pos,
                           char const *opt_function_name_for_messages,
                           unsigned int flags, tpp_token_id lparen_kind)
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen(tpp_lexer *tpp_restrict self,
                        tpp_lexer_arginfo *tpp_restrict p_argv,
                        tpp_size *tpp_restrict p_argc,
                        tpp_char const **p_rollback_pos,
                        char const *opt_function_name_for_messages,
                        unsigned int flags)
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
{
#if TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
#define tpp_lexer_seekpp_rparen_keepspace() (flags & TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC)
#else /* TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#define tpp_lexer_seekpp_rparen_keepspace() (TPP_HAVE_MACRO_ARGUMENT_WHITESPACE != 0)
#endif /* !TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
	tpp_size const argv_bufsize = *p_argc;
	tpp_size argc = 0;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_token_id result;
	tpp_seek_rparen_state state;
	tpp_lexer_state_flags saved_lexer_state;
	tpp_size curarg_rel_start; /* Start of current argument (relative to current file's KEEP) */
	tpp_size curarg_rel_end;   /* End of current argument (relative to current file's KEEP) */
#if TPP_CONF_MAYBE_0(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
	tpp_size curarg_rel_rend;  /* End of current argument without trailing whitespace */
#define tpp_set_curarg_rel_rend(v) (curarg_rel_end = curarg_rel_rend = (v))
#define tpp_get_curarg_rel_rend()  curarg_rel_rend
#else /* TPP_CONF_MAYBE_0(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#define tpp_set_curarg_rel_rend(v) (curarg_rel_end = (v))
#define tpp_get_curarg_rel_rend()   curarg_rel_end
#endif /* !TPP_CONF_MAYBE_0(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
	saved_lexer_state = self->tl_state;
	self->tl_state |= TPP_LEXER_STATE_FLAG_ALLTOKENS;
	tpp_seek_rparen_state_init(&state, lparen_kind);
	tpp_seek_rparen_state_save_curfile(&state, self);
	tpp_seek_rparen_state_init_curarg(&state);

	/* NOTES:
	 * - Elements of "p_argv" are:
	 *   - Only initialized in the range [0,min(argv_bufsize-1, argc))
	 *   - When tpp_lexer_arginfo_isrelative() is true, "tlai_chunk",
	 *     "tlai_enc" and "tlai_lcinfo" are all undefined, and the
	 *     "tlai_start"+"tlai_end" fields are actually relative offsets
	 *     to the current file's keep-pointer
	 *   - When tpp_lexer_arginfo_isrelative() is false, the argument
	 *     is initialized as per the specifications from "lexer.h"
	 *
	 * - "curarg_rel_start" / "curarg_rel_end" specify additional text
	 *   that needs to be appended to the current argument
	 */

	result = token->tt_id;
	curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_end);
	tpp_set_curarg_rel_rend(curarg_rel_start);
again_yield_and_switch_tok:
	result = tpp_lexer_yieldpp_blocking(self);
again_switch_tok:
	if (file->tf_prev != NULL && !TPP_TOK_ISERR(result)) {
		/* Extra file was pushed -- include all tokens as-is and don't scan for arguments */
		if (curarg_rel_start < tpp_get_curarg_rel_rend()) { /* Save argument text */
			tpp_char const *data;
			tpp_size num_bytes = (tpp_size)(tpp_get_curarg_rel_rend() - curarg_rel_start);
			tpp_file *curfile = file->tf_prev;
			while (curfile->tf_prev)
				curfile = curfile->tf_prev;
			data = tpp_file_keep_rel2ptr(curfile, curarg_rel_start);
			if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
				goto err_nomem;
		}
		for (;;) {
			if (!tpp_seek_rparen_state_curarg_append(&state,
			                                         tpp_token_getstart(token),
			                                         tpp_token_getlen(token)))
				goto err_nomem;
			result = tpp_lexer_yieldpp_blocking(self);
			if (TPP_TOK_ISERR(result))
				break;
		} while (file->tf_prev != NULL);
		curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_start);
		tpp_set_curarg_rel_rend(curarg_rel_start);
	}

	switch (result) {

	case TPP_TOK_SPACE:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT {
#if TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
		if (tpp_lexer_seekpp_rparen_keepspace())
			break; /* When whitespace should be kept: treat it like a regular token */
#endif /* TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_MAYBE_0(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
		if (state.tsrps_curarg_prefix.tsb_len == 0 &&
		    curarg_rel_start == curarg_rel_rend) {
			/* Skip leading whitespace... */
			curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_end);
			tpp_set_curarg_rel_rend(curarg_rel_start);
			goto again_yield_and_switch_tok;
		}
		if (curarg_rel_end != tpp_file_keep_ptr2rel(file, token->tt_start)) {
			tpp_assert(curarg_rel_rend >= curarg_rel_start);
			if (curarg_rel_rend > curarg_rel_start) {
				tpp_size num_bytes = (tpp_size)(curarg_rel_rend - curarg_rel_start);
				tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
				if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
					goto err_nomem;
			}
			curarg_rel_start = curarg_rel_rend;
		}
		curarg_rel_end = tpp_file_keep_ptr2rel(file, token->tt_end);
		goto again_yield_and_switch_tok;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
	}	break;

	case TPP_TOK_EOF:
#if TPP_HAVE_INCLUDE_STACK
		if (state.tsrps_curfile_saved_prev && file->tf_kind != TPP_FILE_KIND_IO) {
			/* Make already-encoded arguments absolute */
			tpp_size i, written_argc = argc;
			if (written_argc >= argv_bufsize) {
				written_argc = argv_bufsize;
				if (written_argc)
					--written_argc;
			}
			for (i = 0; i < written_argc; ++i) {
				tpp_lexer_arginfo *arg = &p_argv[i];
				if (tpp_lexer_arginfo_isrelative(arg)) {
					tpp_lexer_arginfo_rel2ptr(arg, file);
					arg->tlai_chunk = file->tf_chunk;
					if (arg->tlai_chunk)
						tpp_string_incref(arg->tlai_chunk);
				}
			}

#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
			if (tpp_lexer_seekpp_rparen_keepspace()) {
				curarg_rel_rend = curarg_rel_end;
			} else
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */
			{
#if TPP_CONF_MAYBE_0(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
				/* FIXME: Have to print text until "curarg_rel_end" (iow: including whitespace)
				 *        if follow-up files contain tokens that also have to be appended (the
				 *        trailing whitespace of the current (old) file must only be trimmed if
				 *        the argument at the very start of the next file (possibly after being
				 *        preceded by some more whitespace)) */
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
			}

			if (curarg_rel_start < tpp_get_curarg_rel_rend()) { /* Save argument text */
				tpp_size num_bytes   = (tpp_size)(tpp_get_curarg_rel_rend() - curarg_rel_start);
				tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
				if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
					goto err_nomem;
			}

			/* Continue with next file */
			tpp_seek_rparen_state_rstr_curfile(&state, self, true);
			tpp_assert(file->tf_prev == state.tsrps_curfile_saved_prev);
			tpp_assert(file->tf_prev);
#if TPP_HAVE_LEXER_MANUALPOPFILE
			if (flags & TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK) {
				/* Special case: use an alternate (rollback-capable) mechanism to pop files. */
				tpp_lexer_manualpopfile_popfile(self);
			} else
#endif /* TPP_HAVE_LEXER_MANUALPOPFILE */
			{
				tpp_lexer_popfile(self);
			}
			tpp_seek_rparen_state_save_curfile(&state, self);
			result = tpp_lexer_yieldpp_blocking(self);
			curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_start);
			tpp_set_curarg_rel_rend(curarg_rel_start);
			goto again_switch_tok;
		} else
#endif /* TPP_HAVE_INCLUDE_STACK */
#if TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST
		if (!(flags & TPP_LEXER_SEEK_RPAREN_FLAG_NOWARNEOF)) {
			tpp_errno error;
			tpp_char const *pos = file->tf_data.td_io.ttf_keep + state.tsrps_curfile_saved_tpos_rel;
			error = tpp_lexer_warnf_at(self, pos, TPP_W_EOF_IN_ARGUMENT_LIST,
			                           opt_function_name_for_messages);
			if (TPP_ISERR(error)) {
				result = TPP_TOK_OFERR(error);
				goto err_result;
			}
		} else
#endif /* TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST */
		{
		}
		goto done;

	case '(':
		++state.tsrps_recursion[0];
		break;

	case ')':
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] == 0) {
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
			if (tpp_seek_rparen_state_getcc(&state) == TPP_RECURSION_CC_PAREN)
#endif /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
			{
				goto done;
			}
		}
		--state.tsrps_recursion[TPP_RECURSION_CC_PAREN];
		break;

#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	case '[':
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_BRACKET)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		++state.tsrps_recursion[TPP_RECURSION_CC_BRACKET];
		break;
	case ']':
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_BRACKET)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] == 0)
			goto done;
		--state.tsrps_recursion[TPP_RECURSION_CC_BRACKET];
		break;

	case '{':
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_BRACE)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		++state.tsrps_recursion[TPP_RECURSION_CC_BRACE];
		break;
	case '}':
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_BRACE)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACE] == 0)
			goto done;
		--state.tsrps_recursion[TPP_RECURSION_CC_BRACE];
		break;

#if (TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE ||              \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE ||       \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS ||               \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE ||        \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE ||        \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE ||        \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE)
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	case TPP_TOK_LANGLE_EQUAL: /* "<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE: /* "<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_EQUAL: /* "<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE_LANGLE: /* "<<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: /* "<<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
	case TPP_TOK_LANGLE_MINUS: /* "<-" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	case TPP_TOK_LANGLE_EQUAL_RANGLE: /* "<=>" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	case TPP_TOK_LANGLE_MINUS_RANGLE: /* "<->" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	case TPP_TOK_LANGLE_EQUAL_LANGLE: /* "<=<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	case TPP_TOK_LANGLE_MINUS_LANGLE: /* "<-<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
		/* Convert to "<" token */
		tpp_assert(token->tt_start < token->tt_end);
		tpp_assert(token->tt_start[0] == '<');
		token->tt_end = token->tt_start + 1;
/*		result = TPP_TOK_OFCHAR('<');  * Not necessary */
/*		token->tt_id = result;         * Not necessary */
		goto handle_langle;
#define WANT_handle_langle
		break;
#endif /* ... */



#if (TPP_HAVE_TPP_TOK_RANGLE_LANGLE ||              \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||              \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE ||       \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS ||               \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE)
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	case TPP_TOK_RANGLE_LANGLE: /* "><" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE: /* ">>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	case TPP_TOK_RANGLE_EQUAL: /* ">=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_EQUAL: /* ">>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE_RANGLE: /* ">>>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: /* ">>>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
	case TPP_TOK_RANGLE_MINUS: /* ">-" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	case TPP_TOK_RANGLE_EQUAL_LANGLE: /* ">=<" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	case TPP_TOK_RANGLE_MINUS_LANGLE: /* ">-<" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	case TPP_TOK_RANGLE_EQUAL_RANGLE: /* ">=>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	case TPP_TOK_RANGLE_MINUS_RANGLE: /* ">->" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
		/* Convert to ">" token */
		tpp_assert(token->tt_start < token->tt_end);
		tpp_assert(token->tt_start[0] == '>');
		token->tt_end = token->tt_start + 1;
		result = TPP_TOK_OFCHAR('>');
		token->tt_id = result;
		goto handle_rangle;
#define WANT_handle_rangle
		break;
#endif /* ... */

	/* Tokens where the first character is irrelevant, and need to be split into 2 */
#if (TPP_HAVE_TPP_TOK_MINUS_RANGLE ||               \
     TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR ||          \
     TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS ||          \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE ||               \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE || \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE ||               \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE || \
     TPP_HAVE_TPP_TOK_MINUS_LANGLE)
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	case TPP_TOK_MINUS_RANGLE: /* "->" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	case TPP_TOK_MINUS_RANGLE_STAR: /* "->*" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	case TPP_TOK_STAR_LANGLE_MINUS: /* "*<-" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	case TPP_TOK_EQUAL_LANGLE: /* "=<" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE: /* "=<<" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE: /* "=<<<" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	case TPP_TOK_EQUAL_RANGLE: /* "=>" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE: /* "=>>" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE: /* "=>>>" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
	case TPP_TOK_MINUS_LANGLE: /* "-<" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_LANGLE */
		/* Convert to 1-char token */
		tpp_assert(token->tt_start < token->tt_end);
		token->tt_end = token->tt_start + 1;
/*		result = TPP_TOK_OFCHAR(token->tt_start[0]); * Not necessary */
/*		token->tt_id = result;                       * Not necessary */
		break;
#endif /* ... */

	case '<':
#ifdef WANT_handle_langle
#undef WANT_handle_langle
handle_langle:
#endif /* WANT_handle_langle */
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_ANGLE)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		++state.tsrps_recursion[TPP_RECURSION_CC_ANGLE];
		break;

	case '>':
#ifdef WANT_handle_rangle
#undef WANT_handle_rangle
handle_rangle:
#endif /* WANT_handle_rangle */
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_ANGLE)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_ANGLE] == 0)
			goto done;
		--state.tsrps_recursion[TPP_RECURSION_CC_ANGLE];
		break;

#endif /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */


	case ',': {
		/* Make sure that we're not inside of a nested ( )-pair */
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_ANGLE] != 0)
			break;
#endif /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */

#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
		if (tpp_lexer_seekpp_rparen_keepspace())
			curarg_rel_rend = curarg_rel_end; /* Argument must includes whitespace preceding the ","-token */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */

		/* Write next argument */
		if (argc < (argv_bufsize - 1)) {
			tpp_lexer_arginfo *arg = &p_argv[argc];
			if (state.tsrps_curarg_prefix.tsb_len) {
				if (curarg_rel_start < tpp_get_curarg_rel_rend()) { /* Save argument text */
					tpp_size num_bytes   = (tpp_size)(tpp_get_curarg_rel_rend() - curarg_rel_start);
					tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
					if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
						goto err_nomem;
				}
				arg->tlai_chunk = tpp_string_builder_pack(&state.tsrps_curarg_prefix);
				arg->tlai_start = tpp_string_str(arg->tlai_chunk);
				arg->tlai_end   = tpp_string_end(arg->tlai_chunk);
				tpp_seek_rparen_state_init_curarg(&state);
			} else {
				arg->tlai_chunk = (TPP_REF tpp_string *)-1; /* Indicate that this is a relative argument (fixed later) */
				arg->tlai_start = (tpp_char const *)curarg_rel_start;
				arg->tlai_end   = (tpp_char const *)tpp_get_curarg_rel_rend();
			}
			curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_end);
			tpp_set_curarg_rel_rend(curarg_rel_start);
			++argc;
			goto again_yield_and_switch_tok;
		}
		++argc;
	}	break;

	default:
		if (TPP_TOK_ISERR(result))
			goto err_result;
		break;
	}
	if (curarg_rel_end != tpp_file_keep_ptr2rel(file, token->tt_start)) {
		tpp_assert(curarg_rel_end >= curarg_rel_start);
		if (curarg_rel_end > curarg_rel_start) {
			tpp_size num_bytes = (tpp_size)(curarg_rel_end - curarg_rel_start);
			tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
			if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
				goto err_nomem;
		}
		curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_start);
	}
	tpp_set_curarg_rel_rend(tpp_file_keep_ptr2rel(file, token->tt_end));
	goto again_yield_and_switch_tok;


done:
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
	if (tpp_lexer_seekpp_rparen_keepspace())
		curarg_rel_rend = curarg_rel_end; /* Argument must includes trailing whitespace */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */
	if (argc || (state.tsrps_curarg_prefix.tsb_len ||
	             tpp_get_curarg_rel_rend() > curarg_rel_start)) {
		/* Write last argument */
		tpp_lexer_arginfo *arg;
		if (argc < argv_bufsize) {
			arg = &p_argv[argc];
		} else if (argv_bufsize) {
			/* Update end-pointer of last argument (for varargs) */
			arg = &p_argv[argv_bufsize - 1];
		} else {
			++argc;
			goto done_after_last_arg;
		}
		++argc;
		if (state.tsrps_curarg_prefix.tsb_len) {
			if (tpp_get_curarg_rel_rend() > curarg_rel_start) {
				tpp_size num_bytes   = (tpp_size)(tpp_get_curarg_rel_rend() - curarg_rel_start);
				tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
				if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
					goto err_nomem;
			}
			arg->tlai_chunk = tpp_string_builder_pack(&state.tsrps_curarg_prefix);
			arg->tlai_start = tpp_string_str(arg->tlai_chunk);
			arg->tlai_end   = tpp_string_end(arg->tlai_chunk);
			tpp_seek_rparen_state_init_curarg(&state);
		} else {
			arg->tlai_chunk = (TPP_REF tpp_string *)-1; /* Indicate that this is a relative argument (fixed later) */
			arg->tlai_start = (tpp_char const *)curarg_rel_start;
			arg->tlai_end   = (tpp_char const *)tpp_get_curarg_rel_rend();
		}
	}
done_after_last_arg:
#if TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS
	if (argc > argv_bufsize && !(flags & TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS)) {
		tpp_errno error;
		tpp_char const *pos = file->tf_data.td_io.ttf_keep + state.tsrps_curfile_saved_tpos_rel;
		error = tpp_lexer_warnf_at(self, pos, TPP_W_TOO_MANY_ARGUMENTS,
		                           opt_function_name_for_messages,
		                           (unsigned int)argv_bufsize,
		                           (unsigned int)argc);
		if (TPP_ISERR(error))
			result = TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS */
err_result:
	*p_argc = argc;
	if (argc > argv_bufsize)
		argc = argv_bufsize;
	if tpp_unlikely(TPP_TOK_ISERR(result)) {
		tpp_size i;
		if (argc)
			--argc; /* Last argument is initialized at the very end */
		for (i = 0; i < argc; ++i) {
			tpp_lexer_arginfo *arg = &p_argv[i];
			if (!tpp_lexer_arginfo_isrelative(arg))
				tpp_lexer_arginfo_fini(arg);
		}
	} else {
		tpp_size i;
		for (i = 0; i < argc; ++i) {
			tpp_lexer_arginfo *arg = &p_argv[i];
			if (tpp_lexer_arginfo_isrelative(arg)) {
				tpp_lexer_arginfo_rel2ptr(arg, file);
				arg->tlai_chunk = file->tf_chunk;
				if (arg->tlai_chunk)
					tpp_string_incref(arg->tlai_chunk);
			}
		}
	}
	*p_rollback_pos = tpp_file_keep_rel2ptr(file, state.tsrps_curfile_saved_pos_rel);
	tpp_seek_rparen_state_rstr_curfile(&state, self, false);
	tpp_seek_rparen_state_fini_curarg(&state);
	self->tl_state = saved_lexer_state;
	return result;
err_nomem:
	result = TPP_TOK_ENOMEM;
	goto err_result;
#undef tpp_set_curarg_rel_rend
#undef tpp_lexer_seekpp_rparen_keepspace
}



/* Same as above, but always initializes *exactly* "argc" arguments,
 * and automatically emits "TPP_W_TOO_FEW_ARGUMENTS" when fewer were
 * parsed. */
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen_exact_ex(tpp_lexer *tpp_restrict self,
                                 tpp_lexer_arginfo *tpp_restrict p_argv, tpp_size argc,
                                 char const *opt_function_name_for_messages,
                                 unsigned int flags, tpp_token_id lparen_kind)
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen_exact(tpp_lexer *tpp_restrict self,
                              tpp_lexer_arginfo *tpp_restrict p_argv, tpp_size argc,
                              char const *opt_function_name_for_messages,
                              unsigned int flags)
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
{
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_token_id result;
	tpp_size argc_actual = argc;
	tpp_char const *rollback_pos;
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	result = tpp_lexer_seekpp_rparen_ex(self, p_argv, &argc_actual, &rollback_pos,
	                                    opt_function_name_for_messages, flags,
	                                    lparen_kind);
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	result = tpp_lexer_seekpp_rparen(self, p_pos, p_argv, &argc_actual, &rollback_pos,
	                                 opt_function_name_for_messages, flags);
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	if (!TPP_TOK_ISERR(result) && argc_actual < argc) {
		tpp_size i;
#if TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS
		tpp_errno error;
		error = tpp_lexer_warnf(self, TPP_W_TOO_FEW_ARGUMENTS,
		                        opt_function_name_for_messages,
		                        (unsigned int)argc,
		                        (unsigned int)argc_actual);
		if (TPP_ISERR(error))
			result = TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */
		for (i = argc_actual; i < argc; ++i) {
			p_argv[i].tlai_start = rollback_pos;
			p_argv[i].tlai_end   = rollback_pos;
			p_argv[i].tlai_chunk = file->tf_chunk;
			if (file->tf_chunk)
				tpp_string_incref(file->tf_chunk);
		}
	}
	return result;
}

#endif /* TPP_HAVE_LEXER_SEEKPP_RPAREN */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-pp-define.c                                        */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_DIRECTIVES && TPP_HAVE_CPP_DEFINE

typedef struct tpp_macro_builder {
	tpp_size            mab_argc;        /* # of arguments */
	tpp_size            mab_arga;        /* Allocated # of arguments */
	tpp_macro_argument *mab_argv;        /* [0..mab_argc] Argument list */
#if TPP_HAVE_MACRO_FLAGS
	tpp_macro_flag      mab_flags;       /* Macro flags */
#define _tpp_macro_builder_init_flags(self) , (self)->mab_flags = TPP_MACRO_FLAG_NORMAL
#else /* TPP_HAVE_MACRO_FLAGS */
#define _tpp_macro_builder_init_flags(self) /* nothing */
#endif /* !TPP_HAVE_MACRO_FLAGS */
	tpp_ssize           mab_skiptotal;   /* # of characters skipped during expansion. */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
	tpp_size            mab_n_vaopt;     /* Amount of extra bytes inserted when varargs are given (if: tpp_lexer_seekpp_rparen:OUT(*p_argc) > tmf_argc). */
#define _tpp_macro_builder_init_n_vaopt(self) , (self)->mab_n_vaopt = 0
#else /* TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#define _tpp_macro_builder_init_n_vaopt(self) /* nothing */
#endif /* !TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	tpp_size            mab_n_vanargs;   /* Amount of times `__VA_NARGS__' is used in `tmf_expand'. */
#define _tpp_macro_builder_init_n_vanargs(self) , (self)->mab_n_vanargs = 0
#else /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
#define _tpp_macro_builder_init_n_vanargs(self) /* nothing */
#endif /* !TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
	tpp_size            mab_tmf_expandc; /* Used buffer size in `mab_macro->tm_data.tmd_func.tmf_expand' */
	tpp_size            mab_tmf_expanda; /* Allocated buffer size in `mab_macro->tm_data.tmd_func.tmf_expand' */
	tpp_macro          *mab_macro;       /* [0..*] The macro being built */
} tpp_macro_builder;

#define tpp_macro_builder_init(self)                \
	(void)((self)->mab_argv = NULL,                 \
	       (self)->mab_argc = 0,                    \
	       (self)->mab_arga = 0                     \
	       _tpp_macro_builder_init_flags(self),     \
	       (self)->mab_skiptotal = 0                \
	       _tpp_macro_builder_init_n_vaopt(self)    \
	       _tpp_macro_builder_init_n_vanargs(self), \
	       (self)->mab_tmf_expandc = 0,             \
	       (self)->mab_tmf_expanda = 0,             \
	       (self)->mab_macro = NULL)
#define tpp_macro_builder_fini(self) \
	(tpp_free((self)->mab_argv),     \
	 tpp_macro_free((self)->mab_macro))

#ifdef __OPTIMIZE_SIZE__
#define tpp_macro_builder_truncate_argv(self) (void)0
#else /* __OPTIMIZE_SIZE__ */
static TPP_NONNULL((1)) void TPPCALL
tpp_macro_builder_truncate_argv(tpp_macro_builder *tpp_restrict self) {
	if (self->mab_argc < self->mab_arga) {
		/* Truncate unused memory */
		tpp_macro_argument *new_macro_argv;
		new_macro_argv = (tpp_macro_argument *)tpp_tryrealloc(self->mab_argv,
		                                                      self->mab_argc *
		                                                      sizeof(tpp_macro_argument));
		if tpp_likely(new_macro_argv)
			self->mab_argv = new_macro_argv;
	}
}
#endif /* !__OPTIMIZE_SIZE__ */

/* Check if "name" identifies a known argument. If so: return it. Otherwise, return "NULL" */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_argument *TPPCALL
tpp_macro_builder_getargument(tpp_macro_builder const *tpp_restrict self, tpp_token_id name) {
	tpp_size i;
	tpp_macro_argument *argv = self->mab_argv;
	for (i = 0; i < self->mab_argc; ++i) {
		if (argv[i].tma_id == name)
			return &argv[i];
	}
	return NULL;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_argument *TPPCALL
tpp_macro_builder_newargument(tpp_macro_builder *tpp_restrict self) {
	tpp_assert(self->mab_argc <= self->mab_arga);
	if (self->mab_argc >= self->mab_arga) {
		tpp_macro_argument *new_macro_argv;
		tpp_size new_macro_arga = self->mab_arga * 2;
		tpp_size min_macro_arga = self->mab_argc + 1;
		if (new_macro_arga < 3)
			new_macro_arga = 3;
		if (new_macro_arga < min_macro_arga)
			new_macro_arga = min_macro_arga;
		new_macro_argv = (tpp_macro_argument *)tpp_tryrealloc(self->mab_argv,
		                                                      new_macro_arga *
		                                                      sizeof(tpp_macro_argument));
		if tpp_unlikely(!new_macro_argv) {
			new_macro_arga = min_macro_arga;
			new_macro_argv = (tpp_macro_argument *)tpp_realloc(self->mab_argv,
			                                                   new_macro_arga *
			                                                   sizeof(tpp_macro_argument));
			if tpp_unlikely(!new_macro_argv)
				return NULL;
		}
		self->mab_argv = new_macro_argv;
		self->mab_arga = new_macro_arga;
	}
	tpp_assert(self->mab_argc < self->mab_arga);
	return &self->mab_argv[self->mab_argc++];
}


static TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_lexer_is_rparen_token(tpp_lexer *tpp_restrict self,
                          tpp_char const **p_pos,
                          tpp_token_id lparen_token) {
	(void)lparen_token;
	switch (tpp_lexer_gettoken(self)->tt_id) {
	case ')':
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
		if (lparen_token != '(')
			break;
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */
		return true;
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
	case ']':
		if (lparen_token == '[')
			return true;
		break;
	case '}':
		if (lparen_token == '{')
			return true;
		break;
	case '>':
		if (lparen_token == '<')
			return true;
		break;

#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||              \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE ||       \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_RANGLE_LANGLE)
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE: /* ">>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	case TPP_TOK_RANGLE_EQUAL: /* ">=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_EQUAL: /* ">>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE_RANGLE: /* ">>>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: /* ">>>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	case TPP_TOK_RANGLE_LANGLE: /* "><" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
		/* Convert to ">" token */
		if (lparen_token == '<') {
			tpp_assert(tpp_lexer_gettoken(self)->tt_start < *p_pos);
			tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '>');
			*p_pos = tpp_lexer_gettoken(self)->tt_start + 1;
			tpp_lexer_gettoken(self)->tt_id = TPP_TOK_OFCHAR('>');
			return true;
		}
		break;
#endif /* ... */
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

	default: break;
	}
	return false;
}

/* Parse the parameter list, with self/p_pos pointing at the opening '('-token.
 * Upon success, self/p_pos points after the closing ')'-token. */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_macro_builder_parse_params(tpp_macro_builder *tpp_restrict builder,
                               tpp_lexer *tpp_restrict self,
                               tpp_char const **p_pos) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_token_id const lparen_token = token->tt_id;
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_size rel_start = tpp_file_ptr2rel(file, *p_pos);
	tpp_token_id tok;
	(void)rel_start;
again_yield_macro_argument_list:
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tpp_lexer_is_rparen_token(self, p_pos, lparen_token))
		goto done_yield_rparen; /* Empty parameter list. */
/*again_switch_macro_argument_list:*/
	switch (tok) {
	case TPP_TOK_EOF:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT_LINE
		goto warn_unexpected_parameter_list_token;

#if TPP_HAVE_VA_ARGS_IN_MACROS
	case TPP_TOK_DOT_DOT_DOT:
		if (!tpp_lexer_has(self, VA_ARGS_IN_MACROS))
			break;
		tok = TPP_KWD___VA_ARGS__;
#if TPP_DEBUG
		/* Needed for "tpp_macro_argument::tma_name" */
		token->tt_kwd = tpp_builtin_getkeyword_byid(TPP_KWD___VA_ARGS__);
		tpp_assert(token->tt_kwd != NULL);
#endif /* TPP_DEBUG */
		builder->mab_flags |= TPP_MACRO_FLAG_VARIADIC;
		goto do_append_keyword_to_argument_list;
#define WANT_do_append_keyword_to_argument_list
#endif /* TPP_HAVE_VA_ARGS_IN_MACROS */

	default: break;
	}

	if (!TPP_TOK_ISKEYWORD(tok)) {
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
		/* Warning: expected keyword in function-style macro argument list */
		tpp_errno error;
		tpp_char const *saved_end = token->tt_end;
		token->tt_end = *p_pos;
		error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST);
		token->tt_end = saved_end;
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
		goto again_yield_macro_argument_list;
	}

	/* Append "tok" to macro argument list. */
	{
		tpp_macro_argument *arg;
#ifdef WANT_do_append_keyword_to_argument_list
#undef WANT_do_append_keyword_to_argument_list
do_append_keyword_to_argument_list:
#endif /* WANT_do_append_keyword_to_argument_list */

		/* Check if "tok" is already a known argument. */
#if TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
		arg = tpp_macro_builder_getargument(builder, tok);
		if tpp_unlikely(arg) {
			tpp_errno error;
			tpp_char const *saved_end = token->tt_end;
			token->tt_end = *p_pos;
			error = tpp_lexer_warnf(self, TPP_W_DUPLICATE_MACRO_PARAMETER_NAME);
			token->tt_end = saved_end;
			if (TPP_ISERR(error))
				return error;
		}
#endif /* TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME */

		/* Allocate new argument */
		arg = tpp_macro_builder_newargument(builder);
		if tpp_unlikely(!arg)
			return TPP_ENOMEM;
		arg->tma_id      = tok;
		arg->tma_ins_exp = 0;
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
		arg->tma_ins_str = 0;
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
		arg->tma_ins = 0;
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
#if TPP_DEBUG
		tpp_assert(token->tt_kwd);
		arg->tma_name = token->tt_kwd->tk_kwd;
#endif /* TPP_DEBUG */
	}

	/* Yield to next token (which should be one of ",", ")", or "...") */
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Check for named varargs, as in:
	 * >> #define printf(format, args...) fprintf(stderr, format,##args) */
#if TPP_HAVE_NAMED_VARARGS_IN_MACROS
	if (tok == TPP_TOK_DOT_DOT_DOT &&
	    !(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC) &&
	    tpp_lexer_has(self, NAMED_VARARGS_IN_MACROS)) {
		builder->mab_flags |= TPP_MACRO_FLAG_VARIADIC;
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}
#endif /* TPP_HAVE_NAMED_VARARGS_IN_MACROS */

	/* Check if there are more arguments... */
	if (tok == ',') {
#ifdef TPP_MACRO_FLAG_VARIADIC
		if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
#endif /* TPP_MACRO_FLAG_VARIADIC */
		{
			goto again_yield_macro_argument_list;
		}
	}

	if (tpp_lexer_is_rparen_token(self, p_pos, lparen_token)) {
done_yield_rparen:
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		return TPP_TOK_ASERR_OR_EOK(tok);
	}

warn_unexpected_parameter_list_token:
	/* Warning: expected keyword in function-style macro argument list */
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
	{
		tpp_errno error;
		tpp_char const *saved_end = token->tt_end;
		token->tt_end = *p_pos;
		error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
		                           TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST);
		token->tt_end = saved_end;
		return error;
	}
#else /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
}


/* Allocate additional opcodes */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_opcode *TPPCALL
tpp_macro_builder_requireop(tpp_macro_builder *tpp_restrict self,
                            tpp_size num_opcode_words) {
	tpp_macro_opcode *result;
	tpp_size cur_alloc = self->mab_tmf_expanda;
	tpp_size min_alloc = self->mab_tmf_expandc + num_opcode_words;
	tpp_assert(self->mab_tmf_expandc <= cur_alloc);
	if (cur_alloc < min_alloc) {
		tpp_macro *new_macro;
		tpp_size new_alloc = self->mab_tmf_expanda * 2;
		if (new_alloc < 16)
			new_alloc = 16;
		if (new_alloc < min_alloc)
			new_alloc = min_alloc;
		new_macro = tpp_macro_tryrealloc_function(self->mab_macro, new_alloc);
		if tpp_unlikely(!new_macro) {
			new_alloc = min_alloc;
			new_macro = tpp_macro_realloc_function(self->mab_macro, new_alloc);
			if tpp_unlikely(!new_macro)
				return NULL;
		}
		self->mab_macro       = new_macro;
		self->mab_tmf_expanda = new_alloc;
	}
	tpp_assert(self->mab_tmf_expanda >= min_alloc);
	result = self->mab_macro->tm_data.tmd_func.tmf_expand;
	result += self->mab_tmf_expandc;
	self->mab_tmf_expandc += num_opcode_words;
	return result;
}

#define _tpp_macro_builder_appendops(err_nomem, self, n_ops, init)                  \
	do {                                                                            \
		tpp_macro_opcode *const opcodes = tpp_macro_builder_requireop(self, n_ops); \
		if tpp_unlikely (!opcodes)                                                  \
			goto err_nomem;                                                         \
		(init);                                                                     \
	} while (0)

#define tpp_macro_builder_append_end(err_nomem, self) \
	_tpp_macro_builder_appendops(err_nomem, self, 1, (opcodes[0] = TPP_MACRO_OPCODE_END))
#define tpp_macro_builder_append_skip(err_nomem, self, num_bytes)     \
	_tpp_macro_builder_appendops(err_nomem, self, 2,                  \
	                             (opcodes[0] = TPP_MACRO_OPCODE_SKIP, \
	                              (self)->mab_skiptotal += (opcodes[1] = (num_bytes))))
#define tpp_macro_builder_append_copy(err_nomem, self, num_bytes)     \
	_tpp_macro_builder_appendops(err_nomem, self, 2,                  \
	                             (opcodes[0] = TPP_MACRO_OPCODE_COPY, \
	                              opcodes[1] = (num_bytes)))
#define tpp_macro_builder_append_ins_exp(err_nomem, self, arg, skip_bytes)              \
	_tpp_macro_builder_appendops(err_nomem, self, 3,                                    \
	                             (opcodes[0] = TPP_MACRO_OPCODE_INS_EXP,                \
	                              opcodes[1] = (tpp_size)((arg) - (self)->mab_argv),    \
	                              (self)->mab_skiptotal += (opcodes[2] = (skip_bytes)), \
	                              ++(arg)->tma_ins_exp))
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#define tpp_macro_builder_append_ins_str(err_nomem, self, opcode, arg, skip_bytes)      \
	_tpp_macro_builder_appendops(err_nomem, self, 3,                                    \
	                             (opcodes[0] = opcode, /* TPP_MACRO_OPCODE_INS_STR or   \
	                                                    * TPP_MACRO_OPCODE_INS_CHR */   \
	                              opcodes[1] = (tpp_size)((arg) - (self)->mab_argv),    \
	                              (self)->mab_skiptotal += (opcodes[2] = (skip_bytes)), \
	                              (self)->mab_skiptotal -= 2 /* leading/trailing " */,  \
	                              ++(arg)->tma_ins_str))
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
#define tpp_macro_builder_append_ins(err_nomem, self, arg, skip_bytes)                  \
	_tpp_macro_builder_appendops(err_nomem, self, 3,                                    \
	                             (opcodes[0] = TPP_MACRO_OPCODE_INS,                    \
	                              opcodes[1] = (tpp_size)((arg) - (self)->mab_argv),    \
	                              (self)->mab_skiptotal += (opcodes[2] = (skip_bytes)), \
	                              ++(arg)->tma_ins))
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
#if TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
#define tpp_macro_builder_append_va_comma(err_nomem, self, skip_bytes)                  \
	_tpp_macro_builder_appendops(err_nomem, self, 2,                                    \
	                             (opcodes[0] = TPP_MACRO_OPCODE_VA_COMMA,               \
	                              (self)->mab_skiptotal += (opcodes[1] = (skip_bytes)), \
	                              ++(self)->mab_n_vaopt))
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */
#if TPP_HAVE_VA_OPT_IN_MACROS
#define tpp_macro_builder_append_va_opt(err_nomem, self, skip1, copy, skip2)       \
	_tpp_macro_builder_appendops(err_nomem, self, 4,                               \
	                             (opcodes[0] = TPP_MACRO_OPCODE_VA_OPT,            \
	                              (self)->mab_skiptotal += (opcodes[1] = (skip1)), \
	                              (self)->mab_skiptotal += (opcodes[2] = (copy)),  \
	                              (self)->mab_skiptotal += (opcodes[3] = (skip2)), \
	                              (self)->mab_n_vaopt += (copy)))
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */
#if TPP_HAVE_VA_NARGS_IN_MACROS
#define tpp_macro_builder_append_va_nargs(err_nomem, self, skip)                  \
	_tpp_macro_builder_appendops(err_nomem, self, 2,                              \
	                             (opcodes[0] = TPP_MACRO_OPCODE_VA_NARGS,         \
	                              (self)->mab_skiptotal += (opcodes[1] = (skip)), \
	                              ++(self)->mab_n_vanargs))
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */


#if TPP_HAVE_TRADITIONAL_MACROS
/* Compile a traditional macro (allowed to clobber the token in "builder") */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile_traditional_impl(tpp_macro_builder *tpp_restrict builder,
                                           tpp_lexer *tpp_restrict self,
                                           tpp_char const *body_start,
                                           tpp_char const *body_end) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_char const *body_iter = body_start;

	/* Scan for the body for keywords (including inside of "string" or (*comment*) tokens)
	 * to see if we can find mentions of arguments taken by the macro. Anything found here
	 * must then be used as a point to inject (expanded) arguments.
	 *
	 * As a consequence, you can "stringize" (kind-of) like this:
	 * >> #define str(x) "x"
	 * However, I say "kind-of" because this won't re-escape:
	 * >> str(foo)    // OK:   "foo"           (1 token)
	 * >> str("foo")  // Huh?  ""   foo   ""   (3 tokens)
	 *
	 * ... yeah. It's argument substitution in the most literal sense (which is also why
	 * it has been superseded by "modern" macro compilation for a very long time; as a
	 * matter of fact: ever since __STDC__ has been introduced). So yes: this sort of
	 * behavior actually goes back to those good 'ol <<K&R C>> times. */

	while (body_iter < body_end) {
		tpp_macro_argument *arg;
		tpp_token_id tok;
		tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
		switch (tok) {

#if TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
		TPP_CASE_TPP_TOK_COMMENT_NOLINE {
			if (TPP_TOK_ISCOMMENT_NOLINE(tok)) {
				/* Non-line comments must be deleted in order to support traditional cat operations!
				 * Also note that line-comments shouldn't be present at all (since those should have
				 * caused our caller to terminate the macro body, in case you're wondering) */
				if (token->tt_start > body_start) {
					tpp_macro_builder_append_copy(err_nomem, builder,
						                          (tpp_size)(token->tt_start - body_start));
				}
				tpp_macro_builder_append_skip(err_nomem, builder,
					                          (tpp_size)(body_iter - token->tt_start));
				body_start = body_iter;
			}
		}	continue; /* Not a keyword */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE */

#if TPP_HAVE_TPP_TOK_STRINGLIKE
		/* Strings must not actually be parsed as whole tokens!
		 *
		 * Since this can (easily) cause warnings to be emitted
		 * (~ala "string terminated by eol"), our caller has
		 * disabled them for us!
		 *
		 * This is needed for stuff like:
		 * >> #define str(x) "x"
		 */
		TPP_CASE_TPP_TOK_STRING {
			body_iter = token->tt_start + 1;
			continue;
		}
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE */

		default:
			/* Shouldn't really be able to produce errors, but better be safe. */
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (!TPP_TOK_ISKEYWORD(tok))
				continue;
			break;
		}


		/* Check if this keyword (identified by "tok") is an argument. */
		arg = tpp_macro_builder_getargument(builder, tok);
		if (!arg)
			continue; /* Not actually an argument */

		/* Append opcodes to copy text leading up to argument. */
		if (token->tt_start > body_start) {
			tpp_macro_builder_append_copy(err_nomem, builder,
			                              (tpp_size)(token->tt_start - body_start));
		}

		/* Append opcodes to insert argument */
		tpp_macro_builder_append_ins_exp(err_nomem, builder, arg,
		                                 (tpp_size)(body_iter - token->tt_start));

		/* Remember that input body text has been
		 * flushed until the end of the keyword. */
		body_start = body_iter;
	}

	/* Copy remainder. */
	if (body_start < body_end) {
		tpp_macro_builder_append_copy(err_nomem, builder,
		                              (tpp_size)(body_end - body_start));
	}

	/* Terminate body builder (*flexes muscles*) */
	tpp_macro_builder_append_end(err_nomem, builder);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}

#if TPP_HAVE_WARNINGS
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile_traditional(tpp_macro_builder *tpp_restrict builder,
                                      tpp_lexer *tpp_restrict self,
                                      tpp_char const *body_start,
                                      tpp_char const *body_end) {
	tpp_errno result;
	/* Disable warnings because compiler does some questionable stuff in order to parse
	 * the contents of strings in order to allow arguments to be embedded within them. */
	tpp_lexer_nowarnings_pushon(self);
	result = tpp_macro_builder_compile_traditional_impl(builder, self, body_start, body_end);
	tpp_lexer_nowarnings_pop(self);
	return result;
}
#else /* TPP_HAVE_WARNINGS */
#define tpp_macro_builder_compile_traditional(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_traditional_impl(builder, self, body_start, body_end)
#endif /* !TPP_HAVE_WARNINGS */
#endif /* TPP_HAVE_TRADITIONAL_MACROS */

#if TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)
/* Compile a modern macro (allowed to clobber the token in "builder") */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile_modern(tpp_macro_builder *tpp_restrict builder,
                                 tpp_lexer *tpp_restrict self,
                                 tpp_char const *body_start,
                                 tpp_char const *body_end) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_char const *body_iter = body_start;
#if TPP_HAVE_GLUE_MACRO_ARGUMENT
	/* Specifies the start of a whitespace-area that is deleted by the ##-operator */
	tpp_char const *last_non_space_end = body_start;
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT */

	while (body_iter < body_end) {
		tpp_macro_argument *arg;
		tpp_token_id tok;
		tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
#if (TPP_HAVE_GLUE_MACRO_ARGUMENT || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS ||       \
     TPP_HAVE_VA_OPT_IN_MACROS || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT ||         \
     TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || \
     TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED || TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR)
again_switch_tok:
#endif /* ... */
		switch (tok) {


/************************************************************************/
#if TPP_HAVE_GLUE_MACRO_ARGUMENT
		case TPP_TOK_SPACE:
		TPP_CASE_TPP_TOK_COMMENT_NOLINE
			/* "continue" so-as not to update "last_non_space_end",
			 * allowing a potentially following ##-operator to delete
			 * this token. */
			continue;

		case TPP_TOK_POUND_POUND: {
			tpp_char const *argument_end;
			if (!tpp_lexer_has(self, GLUE_MACRO_ARGUMENT))
				break;
			/* Consume any whitespace that might following this operator.
			 * Whitespace preceding it is automatically consumed because
			 * we skip all not-already-flushed data after "last_non_space_end" */
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);

			/* Encode a copy operation for range: [body_start, last_non_space_end) */
			if (body_start < last_non_space_end) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(last_non_space_end - body_start));
				body_start = last_non_space_end;
			}

			/* Encode a skip operation for range: [body_start, token->tt_start) */
			if (body_start < token->tt_start) {
				tpp_macro_builder_append_skip(err_nomem, builder,
				                              (tpp_size)(token->tt_start - body_start));
				body_start = token->tt_start;
			}

handle_token_after_glue:
			/* If what follows after the ##-operator is another keyword,
			 * must treat it specially if it's a macro-argument! */
			if (!TPP_TOK_ISKEYWORD(tok))
				goto again_switch_tok;
			arg = tpp_macro_builder_getargument(builder, tok);
			if (arg == NULL)
				goto again_switch_tok; /* Must switch on "tok" in case it's something like __VA_NARGS__! */
			argument_end = body_iter; /* End of argument keyword after "##"-token */

			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);

			/* Check if there'll be another ##-operator after the argument */
			if (tok == TPP_TOK_POUND_POUND) {
				do {
					tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
				if (body_start < token->tt_start) {
					tpp_macro_builder_append_ins(err_nomem, builder, arg,
					                             (tpp_size)(token->tt_start - body_start));
					body_start = token->tt_start;
				}
				goto handle_token_after_glue;
			}

			/* End of ##-chain, but still: must insert the last argument *without* expansion! */
			tpp_macro_builder_append_ins(err_nomem, builder, arg,
			                             (tpp_size)(argument_end - body_start));
			last_non_space_end = argument_end;
			body_start = argument_end;
			goto again_switch_tok;
		}	break;
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
		case ',': {
			tpp_char const *start_of_comma;

			/* >> #define printf(format, ...) fprintf(stderr, format,##__VA_ARGS__)
			 *
			 * Compiled the same as (when ignoring whitespace):
			 * >> #define printf(format, ...) fprintf(stderr, format __VA_COMMA__ __VA_ARGS__) */
			if (!tpp_lexer_has(self, VA_GLUE_COMMA_IN_MACROS))
				break;
			if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
				break;
			start_of_comma = token->tt_start;
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (tok != TPP_TOK_POUND_POUND)
				goto again_switch_tok;
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (!TPP_TOK_ISKEYWORD(tok)) {
				/* Implement regular glue (simply deleting whitespace), or do nothing */
handle_not_varargs_argument_after_comma_glue:
#if TPP_HAVE_GLUE_MACRO_ARGUMENT
				if (tpp_lexer_has(self, GLUE_MACRO_ARGUMENT)) {
					++start_of_comma; /* Keep the "," character itself! */
					if (body_start < start_of_comma) {
						tpp_macro_builder_append_copy(err_nomem, builder,
						                              (tpp_size)(start_of_comma - body_start));
						body_start = start_of_comma;
					}
					/* Encode a skip operation for range: [body_start, token->tt_start) */
					if (body_start < token->tt_start) {
						tpp_macro_builder_append_skip(err_nomem, builder,
						                              (tpp_size)(token->tt_start - body_start));
						body_start = token->tt_start;
					}
				}
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT */
				goto again_switch_tok;
			}
			arg = tpp_macro_builder_getargument(builder, tok);
			if (!arg)
				goto handle_not_varargs_argument_after_comma_glue; /* Not an argument */
			if (arg != builder->mab_argv + builder->mab_argc - 1)
				goto handle_not_varargs_argument_after_comma_glue; /* Not the varargs argument */

			/* Yes! This *does* have to be encoded as a __VA_COMMA__! */
			if (body_start < start_of_comma) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(start_of_comma - body_start));
				body_start = start_of_comma;
			}
			/* Insert __VA_COMMA__ and skip template body until the start of the varargs argument */
			tpp_macro_builder_append_va_comma(err_nomem, builder,
			                                  (tpp_size)(token->tt_start - body_start));
			body_start = token->tt_start;
			goto handle_keyword_after_arg;
#define WANT_handle_keyword_after_arg
		}	break;
#endif /* TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_VA_COMMA_IN_MACROS
		case TPP_KWD___VA_COMMA__:
			if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
				goto handle_keyword;
			if (!tpp_lexer_has(self, VA_COMMA_IN_MACROS))
				goto handle_keyword;
#define WANT_handle_keyword
			if (body_start < token->tt_start) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(token->tt_start - body_start));
				body_start = token->tt_start;
			}
			tpp_macro_builder_append_va_comma(err_nomem, builder,
			                                  (tpp_size)(body_iter - body_start));
			body_start = body_iter;
			break;
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_VA_NARGS_IN_MACROS
		case TPP_KWD___VA_NARGS__:
			if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
				goto handle_keyword;
			if (!tpp_lexer_has(self, VA_NARGS_IN_MACROS))
				goto handle_keyword;
#define WANT_handle_keyword
			if (body_start < token->tt_start) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(token->tt_start - body_start));
				body_start = token->tt_start;
			}
			tpp_macro_builder_append_va_nargs(err_nomem, builder,
			                                  (tpp_size)(body_iter - body_start));
			body_start = body_iter;
			break;
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_VA_OPT_IN_MACROS
		case TPP_KWD___VA_OPT__: {
			tpp_char const *start_of_va_opt;
			tpp_char const *start_of_va_opt_body;
			tpp_char const *end_of_va_opt_body;
			unsigned int recursion;
			if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
				goto handle_keyword;
			if (!tpp_lexer_has(self, VA_OPT_IN_MACROS))
				goto handle_keyword;
#define WANT_handle_keyword
			start_of_va_opt = token->tt_start;

			/* Next token must be ( */
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (tok != '(') {
#if TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT
				tpp_errno error;
				tpp_char const *saved_end = token->tt_end;
				token->tt_end = body_iter;
				error = tpp_lexer_warnf_at(self, start_of_va_opt, TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT);
				token->tt_end = saved_end;
				if (TPP_ISERR(error))
					return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT */
				last_non_space_end = body_iter;
				break;
			}

			start_of_va_opt_body = body_iter;
			recursion = 0;
			for (;;) {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				switch (tok) {
				case TPP_TOK_EOF: {
#if TPP_HAVE_TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT
					tpp_errno error;
					tpp_char const *saved_end = token->tt_end;
					token->tt_end = body_iter;
					error = tpp_lexer_warnf_at(self, start_of_va_opt, TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT);
					token->tt_end = saved_end;
					if (TPP_ISERR(error))
						return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT */
					goto found_va_opt_body_end;
				}	break;
				case '(':
					++recursion;
					break;
				case ')':
					if (recursion == 0)
						goto found_va_opt_body_end;
					--recursion;
					break;
				default:
					if (TPP_TOK_ISERR(tok))
						return TPP_TOK_ASERR(tok);
					break;
				}
			}
found_va_opt_body_end:
			end_of_va_opt_body = token->tt_start;
			if (body_start < start_of_va_opt) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(start_of_va_opt - body_start));
				body_start = start_of_va_opt;
			}
			tpp_macro_builder_append_va_opt(err_nomem, builder,
			                                (tpp_size)(start_of_va_opt_body - start_of_va_opt),
			                                (tpp_size)(end_of_va_opt_body - start_of_va_opt_body),
			                                (tpp_size)(body_iter - end_of_va_opt_body));
			body_start = body_iter;
		}	break;
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || \
     TPP_HAVE_CHARIZE_MACRO_ARGUMENT ||   \
     TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT)
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		case TPP_TOK_SHELL_COMMENT:
			/* Deal with special case of shell comments (which must be re-parsed as a #-token) */
			body_iter = token->tt_start + 1;
			TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		case '#': {
			tpp_char const *start_of_pound;
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
			tpp_macro_opcode opcode;
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
			if (!tpp_lexer_has(self, STRINGIZE_MACRO_ARGUMENT) &&
			    !tpp_lexer_has(self, CHARIZE_MACRO_ARGUMENT) &&
			    !tpp_lexer_has(self, DONT_EXPAND_MACRO_ARGUMENT))
				break;
			start_of_pound = token->tt_start;
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);

#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
			if (tok == '!') {
				last_non_space_end = body_iter;
				if (!tpp_lexer_has(self, DONT_EXPAND_MACRO_ARGUMENT))
					break;
				do {
					tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
				if (!TPP_TOK_ISKEYWORD(tok))
					goto again_switch_tok;
				arg = tpp_macro_builder_getargument(builder, tok);
				if (!arg)
					goto again_switch_tok;
				if (body_start < start_of_pound) {
					tpp_macro_builder_append_copy(err_nomem, builder,
					                              (tpp_size)(start_of_pound - body_start));
					body_start = start_of_pound;
				}
				tpp_macro_builder_append_ins(err_nomem, builder, arg,
				                             (tpp_size)(body_iter - body_start));
				body_start = body_iter;
				break;
			} else
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT */
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT
			if (tok == '@') {
				if (!tpp_lexer_has(self, CHARIZE_MACRO_ARGUMENT))
					break;
				opcode = TPP_MACRO_OPCODE_INS_CHR;
				do {
					tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
			} else
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
			{
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
				if (!tpp_lexer_has(self, STRINGIZE_MACRO_ARGUMENT))
					break;
				opcode = TPP_MACRO_OPCODE_INS_STR;
#else /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
				break;
#endif /* !TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
			}
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
			if (!TPP_TOK_ISKEYWORD(tok))
				goto again_switch_tok;
			arg = tpp_macro_builder_getargument(builder, tok);
			if (!arg)
				goto again_switch_tok;
			if (body_start < start_of_pound) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(start_of_pound - body_start));
				body_start = start_of_pound;
			}
			tpp_macro_builder_append_ins_str(err_nomem, builder, opcode, arg,
			                                 (tpp_size)(body_iter - body_start));
			body_start = body_iter;
			break;
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
		}	break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED || TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR
		case TPP_KWD_defined: {
			/* case TPP_KWD_defined: -Wexpansion-to-defined
			 *
			 * Warn about use of "defined" in the body of function-style macros.
			 *
			 * BUT: Not in the following cases:
			 * >> #define foo(defined, x) defined(x)    // Macro has a parameter "defined"
			 * >> #define foo(x)          defined+x     // Next token isn't '(' or a keyword (also handles "defined(#!x)")
			 * >> #define foo(x)          defined(bar)  // Linked keyword isn't a macro argument
			 *
			 * Also: when -fdont-expand-defined is enabled, don't emit
			 *       the waring and instead suppress expansion of the
			 *       macro argument. */
#if TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED
			tpp_char const *start_of_defined = token->tt_start;
#endif /* TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED */
			arg = tpp_macro_builder_getargument(builder, TPP_KWD_defined);
			if (arg)
				goto handle_keyword_after_arg; /* "defined" is actually an argument! */
#define WANT_handle_keyword_after_arg

			last_non_space_end = body_iter;
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (tok == '(') {
				last_non_space_end = body_iter;
				do {
					tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			}
			if (!TPP_TOK_ISKEYWORD(tok))
				goto again_switch_tok;
			arg = tpp_macro_builder_getargument(builder, tok);
			if (!arg)
				goto again_switch_tok;

			/* Got a keyword that is actually a macro argument after "defined"
			 * -> This is what this extension/warning is all about! */
#if TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR
			if (tpp_lexer_has(self, DONT_EXPAND_DEFINED_IN_EXPR)) {
				if (body_start < token->tt_start) {
					tpp_macro_builder_append_copy(err_nomem, builder,
					                              (tpp_size)(token->tt_start - body_start));
					body_start = token->tt_start;
				}
				tpp_macro_builder_append_ins(err_nomem, builder, arg,
				                             (tpp_size)(body_iter - body_start));
				body_start = body_iter;
				break;
			}
#endif /* TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR */

			/* Emit the warning */
#if TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED
			{
				tpp_errno error;
				tpp_char const *saved_end = token->tt_end;
				token->tt_end = body_iter;
				error = tpp_lexer_warnf_at(self, start_of_defined, TPP_W_EXPANSION_TO_DEFINED);
				token->tt_end = saved_end;
				if (TPP_ISERR(error))
					return error;
			}
#endif /* TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED */

			goto handle_keyword_after_arg;
#define WANT_handle_keyword_after_arg
		}	break;
#endif /* TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED || TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR */
/************************************************************************/



/************************************************************************/
		default: {
			/* Shouldn't really be able to produce errors, but better be safe. */
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (TPP_TOK_ISKEYWORD(tok)) {
#ifdef WANT_handle_keyword
#undef WANT_handle_keyword
handle_keyword:
#endif /* WANT_handle_keyword */
				/* Check if this keyword (identified by "tok") is an argument. */
				arg = tpp_macro_builder_getargument(builder, tok);
				if (!arg)
					break; /* Not actually an argument */

#ifdef WANT_handle_keyword_after_arg
#undef WANT_handle_keyword_after_arg
handle_keyword_after_arg:
#endif /* WANT_handle_keyword_after_arg */
				/* Append opcodes to copy text leading up to argument. */
				if (body_start < token->tt_start) {
					tpp_macro_builder_append_copy(err_nomem, builder,
					                              (tpp_size)(token->tt_start - body_start));
					body_start = token->tt_start;
				}

#if TPP_HAVE_GLUE_MACRO_ARGUMENT
				if (tpp_lexer_has(self, GLUE_MACRO_ARGUMENT)) {
					/* Must seek ahead to the next non-whitespace token. if it's the
					 * ##-operator, then we have to insert the argument *WITHOUT* it
					 * being expanded! */
					tpp_char const *argument_end = body_iter;
					do {
						tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
					} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
					if (TPP_TOK_ISERR(tok))
						return TPP_TOK_ASERR(tok);

					/* Append opcodes to insert argument */
					if (tok == TPP_TOK_POUND_POUND) {
						do {
							tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
						} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
						if (TPP_TOK_ISERR(tok))
							return TPP_TOK_ASERR(tok);
						/* Insert argument without expansion, and skip input until the start
						 * of the first non-whitespace token following the ##-operator itself. */
						tpp_macro_builder_append_ins(err_nomem, builder, arg,
						                             (tpp_size)(token->tt_start - body_start));
						body_start = token->tt_start;
						goto handle_token_after_glue;
					}

					/* Insert argument regularly (with expansion) and (since we've already
					 * processed data until that point), continue processing on the next
					 * non-whitespace token. */
					tpp_macro_builder_append_ins_exp(err_nomem, builder, arg,
					                                 (tpp_size)(argument_end - body_start));
					last_non_space_end = argument_end;
					body_start = argument_end;
					goto again_switch_tok;
				} else
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT */
				{
#if TPP_CONF_MAYBE_0(TPP_HAVE_GLUE_MACRO_ARGUMENT)
					/* Append opcodes to insert argument */
					tpp_macro_builder_append_ins_exp(err_nomem, builder, arg,
					                                 (tpp_size)(body_iter - body_start));

					/* Remember that input body text has been
					 * flushed until the end of the keyword. */
					body_start = body_iter;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_GLUE_MACRO_ARGUMENT) */
				}
			}
		}	break;
/************************************************************************/

		}
		last_non_space_end = body_iter;
	}

	/* Copy remainder. */
	if (body_start < body_end) {
		tpp_macro_builder_append_copy(err_nomem, builder,
		                              (tpp_size)(body_end - body_start));
	}

	/* Terminate body builder */
	tpp_macro_builder_append_end(err_nomem, builder);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS) */


#if TPP_CONF_IS_RT(TPP_HAVE_TRADITIONAL_MACROS)
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile(tpp_macro_builder *tpp_restrict builder,
                          tpp_lexer *tpp_restrict self,
                          tpp_char const *body_start,
                          tpp_char const *body_end) {
	if (tpp_lexer_has(self, TRADITIONAL_MACROS))
		return tpp_macro_builder_compile_traditional(builder, self, body_start, body_end);
	return tpp_macro_builder_compile_modern(builder, self, body_start, body_end);
}
#elif TPP_HAVE_TRADITIONAL_MACROS
#define tpp_macro_builder_compile(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_traditional(builder, self, body_start, body_end)
#else /* TPP_HAVE_TRADITIONAL_MACROS */
#define tpp_macro_builder_compile(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_modern(builder, self, body_start, body_end)
#endif /* !TPP_HAVE_TRADITIONAL_MACROS */

static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) TPP_REF tpp_macro *TPPCALL
tpp_macro_builder_pack(/*inherit(on_success)*/ tpp_macro_builder *tpp_restrict self,
                       tpp_lexer *tpp_restrict lexer,
                       tpp_char const *body_start,
                       tpp_char const *body_end,
                       tpp_lcinfo deflc, tpp_token_id lparen_token) {
	TPP_REF tpp_macro *result = self->mab_macro;
	tpp_file *const file = tpp_lexer_getfile(lexer);
	if tpp_unlikely(result == NULL) {
		tpp_assert(self->mab_tmf_expandc == 0);
		tpp_assert(self->mab_tmf_expanda == 0);

		/* Special case: this can happen if the macro's body is empty. */
		result = tpp_macro_malloc_function(1); /* 1: TPP_MACRO_OPCODE_END */
		if tpp_unlikely(!result)
			return NULL;
		result->tm_data.tmd_func.tmf_expand[0] = TPP_MACRO_OPCODE_END;
	} else
#ifndef __OPTIMIZE_SIZE__
	if (self->mab_tmf_expandc < self->mab_tmf_expanda) {
		TPP_REF tpp_macro *new_result;
		new_result = tpp_macro_tryrealloc_function(result, self->mab_tmf_expandc);
		if tpp_likely(new_result)
			result = new_result;
	} else
#endif /* !__OPTIMIZE_SIZE__ */
	{
	}

	/* Fill in fields of macro. */
	tpp_refcnt_init(&result->tm_refcnt, 1);
	result->tm_kind = TPP_MACRO_KIND_OFTOK(lparen_token);
#if TPP_HAVE_MACRO_FLAGS
	result->tm_flags = self->mab_flags;
#endif /* TPP_HAVE_MACRO_FLAGS */
	result->tm_body_chunk = file->tf_chunk;
	if (result->tm_body_chunk)
		tpp_string_incref(result->tm_body_chunk);
	result->tm_body_start = body_start;
	result->tm_body_end   = body_end;
#if TPP_HAVE_UNICODE
	result->tm_body_enc = file->tf_enc;
#endif /* TPP_HAVE_UNICODE */
	result->tm_expansions = 0;
	result->tm_deffile    = tpp_file_filename(file);
	if (result->tm_deffile) {
		result->tm_deflc   = deflc;
		result->tm_body_lc = tpp_file_lcinfo(file, body_start);
	}
	result->tm_data.tmd_func.tmf_argc    = self->mab_argc;
	result->tm_data.tmd_func.tmf_argv    = self->mab_argv; /* Inherit data */
	result->tm_data.tmd_func.tmf_expbase = (tpp_size)(body_end - body_start);
	result->tm_data.tmd_func.tmf_expbase -= self->mab_skiptotal;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
	result->tm_data.tmd_func.tmf_n_vaopt = self->mab_n_vaopt;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	result->tm_data.tmd_func.tmf_n_vanargs = self->mab_n_vanargs;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
	result->tm_data.tmd_func.tmf_argbuf = NULL;
#if TPP_DEBUG
	tpp_memset(self, 0xcc, sizeof(*self));
#endif /* TPP_DEBUG */
	return result;
}


/* Parse a macro-definition, with self/p_pos pointing at the first non-inline-comment
 * token following the macro's name. (in the case of a keyword-style macro, this may
 * be a space-token!)
 *
 * Upon (successful) return, the lexer points at the TPP_TOK_LF (or line-comment)
 * following the definition, or is set to TPP_TOK_EOF if the macro definition is
 * followed by eof-of-file.
 *
 * @return: TPP_EOK: The newly parsed macro definition
 * @return: * :      Error */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_parse_macro_definition(tpp_lexer *tpp_restrict self,
                                 TPP_REF tpp_macro **p_macro,
                                 tpp_char const **p_pos,
                                 tpp_lcinfo deflc) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_token_id const lparen_token = token->tt_id;
	tpp_macro_builder builder;
	tpp_size rel_body_start;
	tpp_size rel_body_end;
	tpp_token_id tok;
	tpp_char const *body_start;
	tpp_char const *body_end;
	TPP_REF tpp_macro *macro;

	/* Check if this is going to be a function- or keyword-style definition */
	switch (lparen_token) {
	case '(':
		break;

#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
	case '[':
	case '{':
	case '<':
		if (tpp_lexer_has(self, ALTERNATIVE_MACRO_PARENTHESIS))
			break;
		TPP_FALLTHRU
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */
	default: {
		/* Keyword-style macro */
		tok = lparen_token;

		/* Find start of body */
		while (TPP_TOK_ISSPACE_OR_COMMENT(tok)) {
			tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
		rel_body_start = tpp_file_ptr2rel(file, token->tt_start);
		rel_body_end   = rel_body_start;

		/* Find end of body */
		while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			rel_body_end = tpp_file_ptr2rel(file, *p_pos);
			tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}

		/* Allocate keyword-style macro definition */
		macro = tpp_macro_malloc_keyword();
		if tpp_unlikely(!macro)
			return TPP_ENOMEM;

		/* Fill in macro information */
		tpp_refcnt_init(&macro->tm_refcnt, 1);
		macro->tm_kind = TPP_MACRO_KIND_KEYWORD;
#if TPP_HAVE_MACRO_FLAGS
		macro->tm_flags = TPP_MACRO_FLAG_NORMAL;
#endif /* TPP_HAVE_MACRO_FLAGS */
		macro->tm_body_chunk = token->tt_chunk;
		if (macro->tm_body_chunk)
			tpp_string_incref(macro->tm_body_chunk);
		macro->tm_body_start = tpp_file_rel2ptr(file, rel_body_start);
		macro->tm_body_end   = tpp_file_rel2ptr(file, rel_body_end);
#if TPP_HAVE_UNICODE
		macro->tm_body_enc = file->tf_enc;
#endif /* TPP_HAVE_UNICODE */
		macro->tm_expansions = 0;
		macro->tm_deffile = tpp_file_filename(file);
		if (macro->tm_deffile) {
			macro->tm_deflc   = deflc;
			macro->tm_body_lc = tpp_file_lcinfo(file, macro->tm_body_start);
		}
		*p_macro = macro;
		return TPP_EOK;
	}	break;

	}

	/* Initialize flags for the macro being built. */
	tpp_macro_builder_init(&builder);
#if TPP_HAVE_MACRO_FLAGS
#if TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
	if (tpp_lexer_has(self, MACRO_ARGUMENT_WHITESPACE))
		builder.mab_flags |= TPP_MACRO_FLAG_KEEPARGSPC;
#endif /* TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_RT(TPP_HAVE_MACRO_RECURSION)
	if (tpp_lexer_has(self, MACRO_RECURSION))
		builder.mab_flags |= TPP_MACRO_FLAG_SELFEXPAND;
#endif /* TPP_CONF_IS_RT(TPP_HAVE_MACRO_RECURSION) */
#endif /* TPP_HAVE_MACRO_FLAGS */

	/* Parse macro argument list */
	error = tpp_macro_builder_parse_params(&builder, self, p_pos);
	if (TPP_ISERR(error))
		goto err_builder;
	tpp_macro_builder_truncate_argv(&builder);

	/* At this point, self/p_pos point at the macro body's first token. */
	rel_body_start = tpp_file_ptr2rel(file, token->tt_start);
	rel_body_end   = rel_body_start;
	tok            = token->tt_id;

	/* Find end of body (moving the lexer to point at the trailing EOF/LF/COMMENT token) */
	while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
again_scan_end_of_macro_body:
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		rel_body_end = tpp_file_ptr2rel(file, *p_pos);
		tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_builder;
		}
	}

#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
	if (tok == TPP_TOK_SHELL_COMMENT) {
		/* Deal with special case of shell comments (which must be re-parsed as a #-token) */
		*p_pos = token->tt_start + 1;
		goto again_scan_end_of_macro_body;
	}
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */

	/* Compile the macro according to active lexer rules */
	body_start = tpp_file_rel2ptr(file, rel_body_start);
	body_end   = tpp_file_rel2ptr(file, rel_body_end);
	tpp_file_pusheof(file);
	tpp_file_seteof(file, body_end); /* This is needed by macro compilers */
	error = tpp_macro_builder_compile(&builder, self, body_start, body_end);
	tpp_file_popeof(file);
	if (TPP_ISERR(error))
		goto err_builder;

	/* Pack the macro together... */
	macro = tpp_macro_builder_pack(&builder, self,
	                               body_start, body_end,
	                               deflc, lparen_token);
	if tpp_unlikely(!macro) {
		error = TPP_ENOMEM;
		goto err_builder;
	}

	/* At this point "macro" has inherited "builder" (so *DONT* fini it) */
	*p_macro = macro;
	return TPP_EOK;
err_builder:
	tpp_macro_builder_fini(&builder);
	return error;
}

/* Handle a "#define" directive, with "self" pointing at the macro's name-keyword
 * @return: TPP_TOK_ISERR: Error
 * @return: TPP_TOK_EOF: Success; caller should yield the next raw token */
TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_define_directive(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	TPP_REF tpp_macro *macro;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_keyword *const keyword = tpp_keywords_copybuiltin(&self->tl_kwds, token->tt_kwd);
	tpp_char const *pos = token->tt_end;
	tpp_lcinfo deflc = tpp_file_lcinfo(file, pos);
	if tpp_unlikely(!keyword)
		goto err_nomem;
	token->tt_end = token->tt_start; /* Ensure that the macro's name stays loaded */
	token->tt_end = pos;
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	} while (TPP_TOK_ISCOMMENT_NOLINE(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Parse+compile the actual macro */
	error = tpp_lexer_parse_macro_definition(self, &macro, &pos, deflc);
	if tpp_unlikely(TPP_ISERR(error))
		return TPP_TOK_OFERR(error);

	/* Setup token such that it describes the entire macro definition (for messages) */
	token->tt_start = token->tt_end;
	token->tt_end   = pos;

	/* TODO: -Wkeyword-macro */

	/* Store the macro definition within the keyword. */
	if (!keyword->tk_macro) {
#if TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO
		if (!TPP_TOK_ISUSERKEYWORD(keyword->tk_id) &&
		    tpp_lexer_getkeyworddefined(self, keyword)) {
			/* Warning if macro is builtin and defined */
			error = tpp_lexer_warnf_at(self, token->tt_end,
			                           TPP_W_DEFINE_BUILTIN_MACRO,
			                           keyword->tk_kwd);
			if (TPP_ISERR(error)) {
				tpp_macro_decref(macro);
				return TPP_TOK_OFERR(error);
			}
		}
#endif /* TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO */
		keyword->tk_macro = macro; /* Inherit reference */
	} else {
#if TPP_HAVE_TPP_W_REDEFINE_MACRO
		if (!tpp_macro_equals(keyword->tk_macro, macro)) {
			/* Warning about macro redefinition */
			error = tpp_lexer_warnf_at(self, token->tt_end,
			                           TPP_W_REDEFINE_MACRO,
			                           keyword);
			if (TPP_ISERR(error)) {
				tpp_macro_decref(macro);
				return TPP_TOK_OFERR(error);
			}
		}
#endif /* TPP_HAVE_TPP_W_REDEFINE_MACRO */
		tpp_macro_decref(keyword->tk_macro);
		keyword->tk_macro = macro; /* Inherit reference */
	}

	return TPP_TOK_EOF;
err_nomem:
	return TPP_TOK_ENOMEM;
}

#endif /* TPP_HAVE_CPP_DIRECTIVES && TPP_HAVE_CPP_DEFINE */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-pp-pragma.c                                        */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_PRAGMA

/************************************************************************/
/* #pragma push_macro(undef, "foo", "bar")                              */
/* #pragma pop_macro("foo", "bar")                                      */
/************************************************************************/
#if TPP_HAVE_PRAGMA_PUSH_MACRO
struct tpp_lexer_handle_pushpopmacro_data {
	tpp_lexer   *tlhppmd_lexer; /* [1..1] Lexer */
	tpp_token_id tlhppmd_mode;  /* TPP_KWD_push_macro or TPP_KWD_pop_macro */
	bool         tlhppmd_undef; /* True if macros should be #undef'd after being pushed */
};

static tpp_errno TPPCALL
tpp_lexer_handle_pushpopmacro_cb(void *arg, tpp_string *chunk,
                                 tpp_char const *str, tpp_size length) {
	tpp_errno result;
	tpp_lexer *lexer;
	struct tpp_lexer_handle_pushpopmacro_data *data;
	tpp_keyword const *ro_keyword;
	tpp_keyword *keyword;
	tpp_hash hash = tpp_hashof(str, length);
	(void)chunk;
	data  = (struct tpp_lexer_handle_pushpopmacro_data *)arg;
	lexer = data->tlhppmd_lexer;

	/* Load keyword */
	ro_keyword = tpp_keywords_newkeyword(&lexer->tl_kwds, str, length, hash);
	if tpp_unlikely(!ro_keyword)
		goto err_nomem;

	/* Make keyword writable */
	keyword = tpp_keywords_copybuiltin(&lexer->tl_kwds, ro_keyword);
	if tpp_unlikely(!keyword)
		goto err_nomem;

	/* Push/pop the macro linked to this keyword. */
	if (data->tlhppmd_mode == TPP_KWD_push_macro) {
		result = tpp_keyword_pushmacro(keyword);
		if (data->tlhppmd_undef) {
			/* Also #undef the keyword if requested */
			if (tpp_keyword_canundef(keyword))
				tpp_keyword_undef(keyword);
		}
	} else {
		result = tpp_keyword_popmacro(keyword);
		tpp_assert(!TPP_ISERR(result) ||
		           result == TPP_ENOENT);
		if (result == TPP_ENOENT) {
			/* Emit a warning */
#if TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK
			result = tpp_lexer_warnf(lexer, TPP_W_POP_MACRO_EMPTY_STACK,
			                         (unsigned int)length, str);
#else /* TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
			result = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
		}
	}
	return result;
err_nomem:
	return TPP_ENOMEM;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_pushpop_macro(tpp_lexer *tpp_restrict self, tpp_token_id mode) {
	tpp_errno error;
	struct tpp_lexer_handle_pushpopmacro_data data;
	tpp_token_id tok;
	data.tlhppmd_lexer = self;
	data.tlhppmd_mode  = mode;
	data.tlhppmd_undef = false;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tok == TPP_KWD_undef) {
		data.tlhppmd_undef = true;
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(','));
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
			tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}

again_parse_string:
	if (TPP_TOK_ISSTRING(tok)) {
		/* Parse+process string (using "tpp_lexer_parsestring_cb()") */
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_pushpopmacro_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* TPP allows multiple strings to be specified here */
	if (tok == ',') {
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		goto again_parse_string;
	}

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */





/************************************************************************/
/* #pragma once                                                         */
/************************************************************************/
#if TPP_HAVE_PRAGMA_ONCE
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_once(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
	tpp_file const *iofile;
	tpp_keyword *iofile_kwd;
	iofile     = tpp_file_getiofile(tpp_lexer_getfile(self));
	iofile_kwd = tpp_file_filename_kwd(iofile);
	if (iofile_kwd) {
		tpp_keyword_misc *misc;
		misc = tpp_keyword_requiremisc(iofile_kwd);
		if tpp_unlikely(!misc)
			return TPP_ENOMEM;
		misc->tkm_flags |= TPP_KEYWORD_FLAG_HDR_ONCE;
	}
#if TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER
#if TPP_HAVE_INCLUDE_STACK
	if (iofile->tf_prev == NULL)
#endif /* TPP_HAVE_INCLUDE_STACK */
	{
		tpp_errno error = tpp_lexer_warnf(self, TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER */
	tok = tpp_lexer_yieldraw_blocking(self);
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_ONCE */





/************************************************************************/
/* #pragma deprecated("foo")                                            */
/************************************************************************/
#if TPP_HAVE_PRAGMA_DEPRECATED
static tpp_errno TPPCALL
tpp_lexer_process_pragma_deprecated_cb(void *arg, tpp_string *chunk,
                                       tpp_char const *str, tpp_size length) {
	tpp_lexer *lexer = (tpp_lexer *)arg;
	tpp_keyword const *ro_keyword;
	tpp_keyword *keyword;
	tpp_keyword_misc *misc;
	tpp_hash hash = tpp_hashof(str, length);
	(void)chunk;
	ro_keyword = tpp_keywords_newkeyword(&lexer->tl_kwds, str, length, hash);
	if tpp_unlikely(!ro_keyword)
		return TPP_ENOMEM;
	keyword = tpp_keywords_copybuiltin(&lexer->tl_kwds, ro_keyword);
	if tpp_unlikely(!keyword)
		return TPP_ENOMEM;
	misc = tpp_keyword_requiremisc(keyword);
	if tpp_unlikely(!misc)
		return TPP_ENOMEM;
	misc->tkm_flags |= TPP_KEYWORD_FLAG_IS_DEPRECATED;
	return TPP_EOK;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_deprecated(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
again_parse_string:
	if (TPP_TOK_ISSTRING(tok)) {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_deprecated_cb,
		                                 self, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* TPP allows multiple strings to be specified here */
	if (tok == ',') {
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		goto again_parse_string;
	}

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_DEPRECATED */





/************************************************************************/
/* #pragma extension(push, "-fmacro-recursion")                         */
/* #pragma extension("-fno-macro-recursion")                            */
/* #pragma extension(pop)                                               */
/************************************************************************/
#if TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION
static tpp_errno TPPCALL
tpp_lexer_process_pragma_extension_cb(void *arg, tpp_string *chunk,
                                      tpp_char const *str, tpp_size length) {
	tpp_errno error;
	tpp_lexer *lexer = (tpp_lexer *)arg;
	tpp_extension_id id;
	bool enable_extension = true;
	(void)chunk;
	if (length >= 2 && str[0] == '-' && str[1] == 'f')
		length -= 2, str += 2;
	if (length >= 3 && str[0] == 'n' && str[1] == 'o' && str[2] == '-')
		length -= 3, str += 3, enable_extension = false;
	id = tpp_extension_byname_ex((char const *)str, length);
	if ((unsigned int)id >= (unsigned int)TPP_EXT_COUNT) {
#if TPP_HAVE_TPP_W_UNKNOWN_EXTENSION
		tpp_extension_id nearest = tpp_extension_nearest_ex((char const *)str, length);
		char const *nearest_name = tpp_extension_getname(nearest);
		error = tpp_lexer_warnf(lexer, TPP_W_UNKNOWN_EXTENSION,
		                        (unsigned int)length, str, nearest_name);
#endif /* TPP_HAVE_TPP_W_UNKNOWN_EXTENSION */
	} else {
		error = tpp_lexer_setextension(lexer, id, enable_extension);
	}
	return error;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_extension(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
again_parse_string:
	switch (tok) {

#if TPP_HAVE_EXTENSIONS_PUSH_POP
	case TPP_KWD_push:
		tpp_lexer_pushextensions(self);
		tok   = tpp_lexer_yield_blocking(self);
		error = TPP_TOK_ASERR_OR_EOK(tok);
		break;

	case TPP_KWD_pop:
		if (tpp_lexer_canpopextensions(self)) {
			tpp_lexer_popextensions(self);
		} else {
#if TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS
			error = tpp_lexer_warnf(self, TPP_W_CANNOT_POP_EXTENSIONS);
#endif /* TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS */
			if (TPP_ISERR(error))
				break;
		}
		tok   = tpp_lexer_yield_blocking(self);
		error = TPP_TOK_ASERR_OR_EOK(tok);
		break;
#endif /* TPP_HAVE_EXTENSIONS_PUSH_POP */

	TPP_CASE_TPP_TOK_STRING
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_extension_cb,
		                                 self, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		break;

	default:
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
		break;
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* TPP allows multiple strings to be specified here */
	if (tok == ',') {
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		goto again_parse_string;
	}

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION */





/************************************************************************/
/* #pragma warning(push, disable: 4068, "[-W]unknown-pragmas")          */
/* #pragma warning(enable: 4068, "[-W]unknown-pragmas")                 */
/* #pragma warning(suppress: 4068, "[-W]unknown-pragmas")               */
/* #pragma warning(error: 4068, "[-W]unknown-pragmas")                  */
/* #pragma warning(default: 4068, "[-W]unknown-pragmas")                */
/* #pragma warning(-1: 4068)                       // #pragma warning(error: 4068)                   */
/* #pragma warning(0: 4068)                        // #pragma warning(enable: 4068)                  */
/* #pragma warning(1: 4068)                        // #pragma warning(disable: 4068)                 */
/* #pragma warning(2: 4068)                        // #pragma warning(suppress: 4068)                */
/* #pragma warning("[-W]unknown-pragmas")          // #pragma warning(enable: "-Wunknown-pragmas")   */
/* #pragma warning("[-W]no-unknown-pragmas")       // #pragma warning(disable: "-Wunknown-pragmas")  */
/* #pragma warning("[-W]def-unknown-pragmas")      // #pragma warning(default: "-Wunknown-pragmas")  */
/* #pragma warning("[-W]sup-unknown-pragmas")      // #pragma warning(suppress: "-Wunknown-pragmas") */
/* #pragma warning("[-W]suppress-unknown-pragmas") // #pragma warning(suppress: "-Wunknown-pragmas") */
/* #pragma warning(pop)                                                 */
/************************************************************************/
#if TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_warning(tpp_lexer *tpp_restrict self) {
	/* TODO */
	/* TODO: Only support push when "TPP_HAVE_WARNINGS_PUSH_POP" */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING */





/************************************************************************/
/* #pragma message("...")                                               */
/* #pragma message "..."                                                */
/************************************************************************/
#if TPP_HAVE_PRAGMA_MESSAGE
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_message(tpp_lexer *tpp_restrict self) {
	/* TODO: Need another user-overwritable output printer (similar to the lexer's warning printer) */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_MESSAGE */





/************************************************************************/
/* #pragma error("...")                                                 */
/************************************************************************/
#if TPP_HAVE_PRAGMA_ERROR
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_error(tpp_lexer *tpp_restrict self) {
	/* TODO */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_ERROR */





/************************************************************************/
/* #pragma GCC system_header                                            */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_system_header(tpp_lexer *tpp_restrict self) {
	tpp_file *iofile = tpp_file_getiofile(tpp_lexer_getfile(self));
	if (iofile->tf_kind == TPP_FILE_KIND_IO)
		iofile->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_SYSHDR;
	return TPP_EOK;
}
#endif /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */





/************************************************************************/
/* #pragma GCC diagnostic push                                          */
/* #pragma GCC diagnostic warning "-Wunknown-pragmas"                   */
/* #pragma GCC diagnostic error "-Wunknown-pragmas"                     */
/* #pragma GCC diagnostic ignored "-Wunknown-pragmas"                   */
/* #pragma GCC diagnostic pop                                           */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_diagnostic(tpp_lexer *tpp_restrict self) {
	/* TODO */
	/* TODO: Only support push when "TPP_HAVE_WARNINGS_PUSH_POP" */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */





/************************************************************************/
/* #pragma GCC poison foo bar foobar ...                                */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_POISON
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_poison(tpp_lexer *tpp_restrict self) {
	/* TODO */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_GCC_POISON */





/************************************************************************/
/* #pragma GCC warning "message"                                        */
/* #pragma GCC error "message"                                          */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_WARNING || TPP_HAVE_PRAGMA_GCC_ERROR
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_warning(tpp_lexer *tpp_restrict self, tpp_warning_id mode) {
	/* TODO */
	(void)self;
	(void)mode;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_GCC_WARNING || TPP_HAVE_PRAGMA_GCC_ERROR */





/************************************************************************/
/* #pragma GCC dependency <file> [<message>]                            */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_DEPENDENCY
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_dependency(tpp_lexer *tpp_restrict self) {
	/* TODO */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_GCC_DEPENDENCY */





/************************************************************************/
/* #pragma tpp_exec("...")                                              */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_tpp_exec(tpp_lexer *tpp_restrict self) {
	/* TODO */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC */





/************************************************************************/
/* #pragma tpp_set_keyword_flags("foo", 0x7f)                           */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_tpp_set_keyword_flags(tpp_lexer *tpp_restrict self) {
	/* TODO */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */





/************************************************************************/
/* #pragma TPP include_path(push, + "/usr/include")                     */
/* #pragma TPP include_path("/usr/local/include")   // same as '+'      */
/* #pragma TPP include_path(- "/usr/include")                           */
/* #pragma TPP include_path(pop)                                        */
/* #pragma TPP include_path(clear)                                      */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_TPP_include_path(tpp_lexer *tpp_restrict self) {
	/* TODO */
	/* TODO: Only support push when "TPP_HAVE_INCLUDE_PATH_PUSH_POP" */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */





/************************************************************************/
/* #pragma GCC ...                                                      */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
	tpp_lexer_seek_backup backup;
	tpp_char const *pos = tpp_lexer_seek_start(self, &backup);
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	switch (tok) {

#if TPP_HAVE_PRAGMA_GCC_POISON
	case TPP_KWD_poison:
		if (!tpp_lexer_has(self, PRAGMA_GCC_POISON))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_poison(self);
#endif /* TPP_HAVE_PRAGMA_GCC_POISON */

#if TPP_HAVE_PRAGMA_GCC_WARNING
	case TPP_KWD_warning:
		if (!tpp_lexer_has(self, PRAGMA_GCC_WARNING))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_warning(self, TPP_W_WARNING);
#endif /* TPP_HAVE_PRAGMA_GCC_WARNING */

#if TPP_HAVE_PRAGMA_GCC_ERROR
	case TPP_KWD_error:
		if (!tpp_lexer_has(self, PRAGMA_GCC_ERROR))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_warning(self, TPP_W_ERROR);
#endif /* TPP_HAVE_PRAGMA_GCC_ERROR */

#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
	case TPP_KWD_system_header:
		if (!tpp_lexer_has(self, PRAGMA_GCC_SYSTEM_HEADER))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_system_header(self);
#endif /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

#if TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC
	case TPP_KWD_diagnostic:
		if (!tpp_lexer_has(self, PRAGMA_GCC_DIAGNOSTIC))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_diagnostic(self);
#endif /* !TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */

#if TPP_HAVE_PRAGMA_GCC_DEPENDENCY
	case TPP_KWD_dependency:
		if (!tpp_lexer_has(self, PRAGMA_GCC_DEPENDENCY))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_dependency(self);
#endif /* !TPP_HAVE_PRAGMA_GCC_DEPENDENCY */

	default: break;
	}
	tpp_lexer_seek_rollback(self, &backup);
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_GCC */





/************************************************************************/
/* #pragma TPP ...                                                      */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_TPP(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
	tpp_lexer_seek_backup backup;
	tpp_char const *pos = tpp_lexer_seek_start(self, &backup);
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	switch (tok) {

#if TPP_HAVE_PRAGMA_TPP_EXTENSION
	case TPP_KWD_extension:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_extension(self);
#endif /* TPP_HAVE_PRAGMA_TPP_EXTENSION */

#if TPP_HAVE_PRAGMA_TPP_WARNING
	case TPP_KWD_warning:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_warning(self);
#endif /* TPP_HAVE_PRAGMA_TPP_WARNING */

#if TPP_HAVE_PRAGMA_TPP_TPP_EXEC
	case TPP_KWD_tpp_exec:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_tpp_exec(self);
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_EXEC */

#if TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
	case TPP_KWD_tpp_set_keyword_flags:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_tpp_set_keyword_flags(self);
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */

#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
	case TPP_KWD_include_path:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_TPP_include_path(self);
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */

	default: break;
	}
	tpp_lexer_seek_rollback(self, &backup);
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_TPP */





/* Process a #pragma directive, start at the first token that comes after
 * the leading "#pragma" (i.e.: the first token of the actual directive
 * itself)
 *
 * @return: TPP_EOK:    Success (but there may still be garbage after
 *                      the directive that hasn't been parsed, yet).
 * @return: TPP_ENOENT: Unknown pragma (soft-error; caller should not emit
 *                      "TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE")
 * @return: TPP_E*:     Error */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);
TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_token_id tok = token->tt_id;
	switch (tok) {

#if TPP_HAVE_PRAGMA_PUSH_MACRO
	case TPP_KWD_push_macro:
	case TPP_KWD_pop_macro:
		if (!tpp_lexer_has(self, PRAGMA_PUSH_MACRO))
			break;
		return tpp_lexer_process_pragma_pushpop_macro(self, tok);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_PRAGMA_ONCE
	case TPP_KWD_once:
		if (!tpp_lexer_has(self, PRAGMA_ONCE))
			break;
		return tpp_lexer_process_pragma_once(self);
#endif /* TPP_HAVE_PRAGMA_ONCE */

#if TPP_HAVE_PRAGMA_DEPRECATED
	case TPP_KWD_deprecated:
		if (!tpp_lexer_has(self, PRAGMA_DEPRECATED))
			break;
		return tpp_lexer_process_pragma_deprecated(self);
#endif /* TPP_HAVE_PRAGMA_DEPRECATED */

#if TPP_HAVE_PRAGMA_EXTENSION
	case TPP_KWD_extension:
		if (!tpp_lexer_has(self, PRAGMA_EXTENSION))
			break;
		return tpp_lexer_process_pragma_extension(self);
#endif /* TPP_HAVE_PRAGMA_EXTENSION */

#if TPP_HAVE_PRAGMA_WARNING
	case TPP_KWD_warning:
		if (!tpp_lexer_has(self, PRAGMA_WARNING))
			break;
		return tpp_lexer_process_pragma_warning(self);
#endif /* TPP_HAVE_PRAGMA_WARNING */

#if TPP_HAVE_PRAGMA_MESSAGE
	case TPP_KWD_message:
		if (!tpp_lexer_has(self, PRAGMA_MESSAGE))
			break;
		return tpp_lexer_process_pragma_message(self);
#endif /* TPP_HAVE_PRAGMA_MESSAGE */

#if TPP_HAVE_PRAGMA_ERROR
	case TPP_KWD_error:
		if (!tpp_lexer_has(self, PRAGMA_ERROR))
			break;
		return tpp_lexer_process_pragma_error(self);
#endif /* TPP_HAVE_PRAGMA_ERROR */

#if TPP_HAVE_PRAGMA_REGION
		/* >> #pragma region MY_REGION
		 * >> #pragma endregion MY_REGION */
	case TPP_KWD_region:
	case TPP_KWD_endregion:
		if (!tpp_lexer_has(self, PRAGMA_REGION))
			break;
		/* These #pragma-s are no-ops, and the remainder of the #pragma-line should
		 * just be ignored. Since (depending on how the #pragma is being generated),
		 * that might include an unknown number of characters, we cheat a little and
		 * return TPP_EENOENT here, which our caller(s) interprets as:
		 * - Skip all trailing tokens
		 * - Don't warn about the presence of trailing tokens
		 * - Translate to TPP_EOK */
		return TPP_ENOENT;
#endif /* TPP_HAVE_PRAGMA_REGION */

#if TPP_HAVE_PRAGMA_TPP_TPP_EXEC
	case TPP_KWD_tpp_exec:
		if (!tpp_lexer_has(self, PRAGMA_TPP_EXEC))
			break;
		return tpp_lexer_process_pragma_tpp_exec(self);
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_EXEC */

#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
	case TPP_KWD_tpp_set_keyword_flags:
		if (!tpp_lexer_has(self, PRAGMA_TPP_SET_KEYWORD_FLAGS))
			break;
		return tpp_lexer_process_pragma_tpp_set_keyword_flags(self);
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

#if TPP_HAVE_PRAGMA_GCC
	case TPP_KWD_GCC: {
		tpp_errno error = tpp_lexer_process_pragma_GCC(self);
		if (error != TPP_ENOENT)
			return error;
	}	break;
#endif /* TPP_HAVE_PRAGMA_GCC */

#if TPP_HAVE_PRAGMA_TPP
	case TPP_KWD_TPP: {
		tpp_errno error = tpp_lexer_process_pragma_TPP(self);
		if (error != TPP_ENOENT)
			return error;
	}	break;
#endif /* TPP_HAVE_PRAGMA_TPP */

	default: break;
	}

	/* TODO: User-defined callback hook to parse pragmas not known to TPP itself */

#if TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS
	{
		tpp_errno error;
		error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_PRAGMAS);
		if (!TPP_ISERR(error))
			error = TPP_ENOENT;
		return error;
	}
#else /* TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
	return TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
}
#endif /* TPP_HAVE_PRAGMA */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-skipraw.c                                          */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_TRYSKIP_RAW

/* Check if "tok == expected", with special handling when
 * "TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS && expected == '<'",
 * in which case "tok" is allowed to be some other token whose
 * first character is '<'. In that last case, "self" is updated
 * to describe '<' before "true" is returned. */
#if (TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS &&       \
     (TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||               \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE ||              \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL ||        \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE ||       \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || \
      TPP_HAVE_TPP_TOK_LANGLE_MINUS ||               \
      TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE ||        \
      TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE ||        \
      TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE ||        \
      TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE))
static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_istok(tpp_lexer *tpp_restrict self,
                tpp_token_id tok,
                tpp_token_id expected,
                tpp_char const **p_pos) {
	if (tok == expected)
		return true;
	if (expected == '<') {
		switch (tok) {
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
		case TPP_TOK_LANGLE_EQUAL: /* "<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
		case TPP_TOK_LANGLE_LANGLE: /* "<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
		case TPP_TOK_LANGLE_LANGLE_EQUAL: /* "<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
		case TPP_TOK_LANGLE_LANGLE_LANGLE: /* "<<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
		case TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: /* "<<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
		case TPP_TOK_LANGLE_MINUS: /* "<-" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
		case TPP_TOK_LANGLE_EQUAL_RANGLE: /* "<=>" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
		case TPP_TOK_LANGLE_MINUS_RANGLE: /* "<->" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
		case TPP_TOK_LANGLE_EQUAL_LANGLE: /* "<=<" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
		case TPP_TOK_LANGLE_MINUS_LANGLE: /* "<-<" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
			/* Convert to "<" token */
			tpp_assert(tpp_lexer_gettoken(self)->tt_start < (*p_pos));
			tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '<');
			(*p_pos) = tpp_lexer_gettoken(self)->tt_start + 1;
			tok      = TPP_TOK_OFCHAR('<');
			tpp_lexer_gettoken(self)->tt_id = tok;
			return true;
		default: break;
		}
	}
	return false;
}
#else /* ... */
#define tpp_lexer_istok(self, tok, expected, p_pos) ((tok) == (expected))
#endif /* !... */

/* Make use of:
 * - tpp_lexer_seek_start()
 * - tpp_lexer_yieldraw_at()
 * - tpp_lexer_manualpopfile_start(self)
 * to seek ahead to the next token, skipping whitespace/line-feed (+resp. comments)
 * based on "flags", check if said "next token" is equal to "expected" (with some extra-
 * extra handling when "TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS && expected == '<'").
 * If that is the case, commit the lexer such that it points at a token equal to
 * the specified "expected", truly disposing of any files popped in the mean-time.
 * Otherwise (the next token is "!= expected"), roll back any changes made, such
 * that "self" once again points at the same token it did upon entry. In either
 * case, return the ID of whatever token came next.
 *
 * NOTE: This function automatically handles "TPP_TOK_EWOULDBLOCK" by blocking!
 *
 * @return: * :                 The next token (rollback)
 * @return: expected:           The next token (commit; iow: this is now also the current token)
 * @return: TPP_TOK_ENOMEM:     Out of memory
 * @return: TPP_TOK_EIO:        I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:  Lexer error
 * @return: TPP_TOK_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_tryskip_raw(tpp_lexer *tpp_restrict self, tpp_token_id expected,
                      unsigned int flags) {
	tpp_lexer_seek_backup backup;
	tpp_char const *pos;
	tpp_token_id tok;
	pos = tpp_lexer_seek_start(self, &backup);
again_yield_mainfile:
	tok = tpp_lexer_yieldraw_at(self, &pos);
	/* NOTE: No need to check for errors -- TPP_TOK_ISERR(*) fails all the tests
	 *       below, meaning it is implicitly handled as rollback+propagate! */
	if (TPP_TOK_ISSPACE_OR_COMMENT(tok) && !(flags & TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONSPACE))
		goto again_yield_mainfile;
	if (TPP_TOK_ISLF_OR_COMMENT(tok) && !(flags & TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONLF))
		goto again_yield_mainfile;
	if (tpp_lexer_istok(self, tok, expected, &pos)) {
		if (flags & TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV) {
			/* Include previous token, too (HINT: tpp_lexer_seek_start()
			 * saved that token's tart in "tt_end" for the sake of the
			 * lexer's file not unloading that token's data) */
			tpp_token *const token = tpp_lexer_gettoken(self);
			token->tt_start = token->tt_end;
		}
		tpp_lexer_seek_commit(self, pos);
		return expected;
	}
	tpp_lexer_seek_rollback(self, &backup);
#if TPP_HAVE_INCLUDE_STACK
	if (tok == TPP_TOK_EOF) {
		/* Check files further up the #include-stack */
		tpp_lexer_manualpopfile_start(self);
		while (tpp_lexer_manualpopfile_canpopfile(self) &&
		       tpp_lexer_getfilekind(self) != TPP_FILE_KIND_IO) {
			tpp_lexer_manualpopfile_popfile(self);
			pos = tpp_lexer_seek_start(self, &backup);
again_yield_nextfile:
			tok = tpp_lexer_yieldraw_at(self, &pos);
			/* NOTE: No need to check for errors -- TPP_TOK_ISERR(*) fails all the tests
			 *       below, meaning it is implicitly handled as rollback+propagate! */
			if (TPP_TOK_ISSPACE_OR_COMMENT(tok) && !(flags & TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONSPACE))
				goto again_yield_nextfile;
			if (TPP_TOK_ISLF_OR_COMMENT(tok) && !(flags & TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONLF))
				goto again_yield_nextfile;
			if (tpp_lexer_istok(self, tok, expected, &pos)) {
				if (flags & TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV) {
					/* Include previous token, too (HINT: tpp_lexer_seek_start()
					 * saved that token's tart in "tt_end" for the sake of the
					 * lexer's file not unloading that token's data) */
					tpp_token *const token = tpp_lexer_gettoken(self);
					token->tt_start = token->tt_end;
				}
				tpp_lexer_seek_commit(self, pos);
				tpp_lexer_manualpopfile_break_commit(self);
				return expected;
			}
			tpp_lexer_seek_rollback(self, &backup);
			if (tok != TPP_TOK_EOF)
				break;
		}
		tpp_lexer_manualpopfile_end_rollback(self);
	}
#endif /* TPP_HAVE_INCLUDE_STACK */
	return tok;
}
#endif /* TPP_HAVE_LEXER_TRYSKIP_RAW */


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-decodeint.c                                        */
/************************************************************************/
TPP_DECL_BEGIN

/* Decode the current token (which should be TPP_TOK_INT) into an integer
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEINT
#if TPP_HAVE_INTEGER_SUFFIX_KIND
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_ex(tpp_lexer *tpp_restrict self,
                       tpp_intmax *tpp_restrict result,
                       tpp_integer_suffix_kind *p_suffix_kind)
#else /* TPP_HAVE_INTEGER_SUFFIX_KIND */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint(tpp_lexer *tpp_restrict self,
                    tpp_intmax *tpp_restrict result)
#endif /* !TPP_HAVE_INTEGER_SUFFIX_KIND */
{
	unsigned int radix = 10;
	tpp_char const *start = tpp_lexer_gettokenstart(self);
	tpp_char const *end   = tpp_lexer_gettokenend(self);
	tpp_char ch;
	*result = 0;
#if TPP_HAVE_INTEGER_SUFFIX_KIND
#define tpp_set_suffix_kind(kind) (void)(!(p_suffix_kind) || (*(p_suffix_kind) = (kind), 1))
	tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT);
#endif /* TPP_HAVE_INTEGER_SUFFIX_KIND */
	if (start >= end)
		goto handle_invalid;
	ch = *start++;
	start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
	if (ch == '0') {
		if (start >= end)
			return TPP_EOK;
		radix = 8;
		ch = *start++;
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		switch (ch) {

		case 'x':
		case 'X':
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			radix = 16;
			break;

#if TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS
		case 'b':
		case 'B':
			if (!tpp_lexer_has(self, BUILTIN_EXPR_BINARY_LITERALS))
				break;
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			radix = 2;
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS */

		default: break;
		}
	} else {
		if (!tpp_ascii_isdigit(ch))
			goto handle_invalid;
	}

	do {
		tpp_intmax new_value, old_value;
		unsigned int digit;
		if (ch >= '0' && ch <= '9') {
			digit = (unsigned int)(ch - '0');
		} else if (ch >= 'a' && ch <= 'z') {
			digit = 10 + (unsigned int)(ch - 'a');
		} else if (ch >= 'A' && ch <= 'Z') {
			digit = 10 + (unsigned int)(ch - 'A');
		} else {
			break;
		}
		if (digit >= radix)
			break;
		old_value = *result;
		new_value = old_value;
		new_value *= radix;
		new_value += digit;
		*result = new_value;
		if (new_value < old_value)
			goto handle_invalid;
		if (start >= end)
			return TPP_EOK;
		ch    = *start++;
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
	} while (tpp_ascii_isdigit(ch));

#if TPP_HAVE_INTEGER_SUFFIX_KIND
	switch (ch) {

#if TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
	case 'i':
	case 'I':
		if (!tpp_lexer_has(self, BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS))
			goto handle_invalid;
		if (start >= end)
			goto handle_invalid;
		ch    = *start++;
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		if (ch == '8') {
			if (start < end)
				goto handle_invalid;
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT8);
		} else if (ch == '1') {
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (ch != '6')
				goto handle_invalid;
			if (start < end)
				goto handle_invalid;
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT16);
		} else if (ch == '3') {
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (ch != '2')
				goto handle_invalid;
			if (start < end)
				goto handle_invalid;
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT32);
		} else if (ch == '6') {
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (ch != '4')
				goto handle_invalid;
			if (start < end)
				goto handle_invalid;
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT64);
		} else {
			goto handle_invalid;
		}
		return TPP_EOK;

	case 'u':
	case 'U': {
		if (start < end && (*start == 'i' || *start == 'I') &&
		    tpp_lexer_has(self, BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS)) {
			++start;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (ch == '8') {
				if (start < end)
					goto handle_invalid;
				tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UINT8);
			} else if (ch == '1') {
				if (start >= end)
					goto handle_invalid;
				ch    = *start++;
				start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
				if (ch != '6')
					goto handle_invalid;
				if (start < end)
					goto handle_invalid;
				tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UINT16);
			} else if (ch == '3') {
				if (start >= end)
					goto handle_invalid;
				ch    = *start++;
				start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
				if (ch != '2')
					goto handle_invalid;
				if (start < end)
					goto handle_invalid;
				tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UINT32);
			} else if (ch == '6') {
				if (start >= end)
					goto handle_invalid;
				ch    = *start++;
				start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
				if (ch != '4')
					goto handle_invalid;
				if (start < end)
					goto handle_invalid;
				tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UINT64);
			} else {
				goto handle_invalid;
			}
			return TPP_EOK;
		}
	}
#if TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS
		TPP_FALLTHRU
#else /* TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */
		break;
#endif /* !TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */


#if TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS
#if !TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
	case 'u':
	case 'U':
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */
	case 'l':
	case 'L': {
		unsigned int has_u = 0;
		unsigned int has_l = 0;
		if (!tpp_lexer_has(self, BUILTIN_EXPR_FIXED_TYPE_INTEGRALS))
			break;
		for (;;) {
			if (ch == 'u' || ch == 'U') {
				++has_u;
			} else if (ch == 'l' || ch == 'L') {
				++has_l;
			} else {
				goto handle_invalid;
			}
			if (start >= end)
				break;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		}
		if (has_u > 1)
			goto handle_invalid;
		switch (has_l) {
		case 0:
			tpp_assert(has_u == 1);
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UNSIGNED);
			break;
		case 1:
			tpp_set_suffix_kind(has_u ? TPP_INTEGER_SUFFIX_KIND_UNSIGNED_LONG
			                          : TPP_INTEGER_SUFFIX_KIND_LONG);
			break;
		case 2:
			tpp_set_suffix_kind(has_u ? TPP_INTEGER_SUFFIX_KIND_UNSIGNED_LONG_LONG
			                          : TPP_INTEGER_SUFFIX_KIND_LONG_LONG);
			break;
		default: goto handle_invalid;
		}
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */

	default: break;
	}
#undef tpp_set_suffix_kind
#endif /* TPP_HAVE_INTEGER_SUFFIX_KIND */

handle_invalid:
#if TPP_HAVE_TPP_W_INVALID_INTEGER
	return tpp_lexer_warnf(self, TPP_W_INVALID_INTEGER);
#else /* TPP_HAVE_TPP_W_INVALID_INTEGER */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
}
#endif /* TPP_HAVE_LEXER_DECODEINT */


/* Decode the current token (which should be TPP_TOK_FLOAT) into a float
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEFLOAT
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat(tpp_lexer *tpp_restrict self,
                      tpp_float *tpp_restrict result) {
	/* TODO */
	(void)self;
	*result = 0.0;
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_DECODEFLOAT */



/* Decode the current token (which should be TPP_TOK_INT) into an integer
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEINT_EXPR
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_expr(tpp_lexer *tpp_restrict self,
                         tpp_expr_value *tpp_restrict result) {
	tpp_intmax value;
	tpp_errno error = tpp_lexer_decodeint(self, &value);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_init_int(result, value);
	return error;
}
#endif /* TPP_HAVE_LEXER_DECODEINT_EXPR */

/* Decode the current token (which should be TPP_TOK_FLOAT) into a float
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEFLOAT_EXPR
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat_expr(tpp_lexer *tpp_restrict self,
                           tpp_expr_value *tpp_restrict result) {
	tpp_float value;
	tpp_errno error = tpp_lexer_decodefloat(self, &value);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_init_float(result, value);
	return error;
}
#endif /* TPP_HAVE_LEXER_DECODEFLOAT_EXPR */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-yieldpp.c                                          */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_TRIGRAPHS && TPP_HAVE_DIGRAPHS
#define tpp_is_start_of_hash(ch) ((ch) == '#' || (ch) == '?' || (ch) == '%')
#elif TPP_HAVE_DIGRAPHS
#define tpp_is_start_of_hash(ch) ((ch) == '#' || (ch) == '%')
#elif TPP_HAVE_TRIGRAPHS
#define tpp_is_start_of_hash(ch) ((ch) == '#' || (ch) == '?')
#else /* ... */
#define tpp_is_start_of_hash(ch) ((ch) == '#')
#endif /* !... */

#if TPP_HAVE_CPP_DIRECTIVES

#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT
#undef tpp_lexer_seek_eol__STYLE_PARAM
#undef tpp_lexer_seek_eol__STYLE_ARG
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
#define tpp_lexer_seek_eol__STYLE_PARAM  , tpp_token_id comment_style
#define tpp_lexer_seek_eol__STYLE_ARG(x) , x
#else /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
#define tpp_lexer_seek_eol__STYLE_PARAM  /* nothing */
#define tpp_lexer_seek_eol__STYLE_ARG(x) /* nothing */
#endif /* !TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */

/* From "./lexer-yieldraw.c" */
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_eol(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos
                   tpp_lexer_seek_eol__STYLE_PARAM);
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT */


#undef TPP_HAVE_TPP_LEXER_YIELDRAW_EOL
#define TPP_HAVE_TPP_LEXER_YIELDRAW_EOL \
	(TPP_HAVE_CPP_DEFINE || TPP_HAVE_CPP_IF_ELSE_ENDIF)

#if TPP_HAVE_TPP_LEXER_YIELDRAW_EOL
/* Skip until EOL and return TPP_TOK_EOF */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_eol(tpp_lexer *tpp_restrict self) {
	tpp_token_id result = tpp_lexer_gettok(self);
	while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
		result = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(result))
			return result;
	}
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_TPP_LEXER_YIELDRAW_EOL */


#if TPP_HAVE_PRAGMA
/* Process a #pragma directive, start at the first token that comes after
 * the leading "#pragma" (i.e.: the first token of the actual directive
 * itself)
 *
 * @return: TPP_EOK:    Success (but there may still be garbage after
 *                      the directive that hasn't been parsed, yet).
 * @return: TPP_ENOENT: Unknown pragma (soft-error; caller should not emit
 *                      "TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE")
 * @return: TPP_E*:     Error */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);

/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_pragma_directive(tpp_lexer *tpp_restrict self) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token_id tok;
	tpp_errno error;
	tpp_char const *eol_start;
	tpp_char const *eol_end;
	tpp_assert(tpp_lexer_gettok(self) == TPP_KWD_pragma);
	do {
		tok = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;
	eol_start = token->tt_start;
	eol_end   = token->tt_end;
	if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		tpp_token_id first_token_id = token->tt_id;
		struct tpp_keyword const *first_token_kwd = token->tt_kwd;
		tpp_size first_token_len = tpp_token_getlen(token);
		token->tt_end = token->tt_start;
		tpp_assert(first_token_id == tok);
		/* Seek until EOL (so we can set a parsing limit for the pragma handler) */
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, &eol_end);
			if (TPP_TOK_ISERR(tok)) {
				token->tt_end = eol_end;
				return tok;
			}
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
		/* Restore first token of #pragma directive */
		eol_start       = token->tt_start;
		token->tt_id    = first_token_id;
		token->tt_kwd   = first_token_kwd;
		token->tt_start = token->tt_end;
		token->tt_end += first_token_len;
	}

	/* Handle the pragma, but in a context where the file can't be read beyond EOL */
	tpp_file_pusheof(file);
	tpp_file_seteof(file, eol_start);
	error = tpp_lexer_process_pragma(self);
	if (error == TPP_ENOENT) {
		error = TPP_EOK;
	} else
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
	if (!TPP_ISERR(error)) {
		tok = tpp_lexer_gettok(self);
		while (TPP_TOK_ISSPACE_OR_COMMENT(tok))
			tok = tpp_lexer_yieldraw(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
		} else if (tok != TPP_TOK_EOF) {
			error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE);
		}
	} else
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE */
	{
	}
#if TPP_HAVE_INCLUDE_STACK
	/* Cleanup files pushed by the expression */
	while (file->tf_prev)
		tpp_lexer_popfile(self);
#endif /* TPP_HAVE_INCLUDE_STACK */

	tpp_file_popeof(file);
	file->tf_pos = eol_end; /* Continue parsing after EOL (comment) */
	return TPP_TOK_OFERR_OR_EOF(error);
}
#endif /* TPP_HAVE_PRAGMA */


#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_handle_error_directive(tpp_lexer *tpp_restrict self,
                                 tpp_char const *directive_iter) {
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_size rel_token_start;
	tpp_size rel_message_start;
	tpp_size rel_message_end;
	tpp_char const *message_start;
	tpp_char const *message_end;
	tpp_size message_size;
	tpp_errno error;
	rel_token_start   = tpp_file_ptr2rel(file, token->tt_start);
	rel_message_start = tpp_file_ptr2rel(file, directive_iter);
	error = tpp_lexer_seek_eol(self, &directive_iter tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
	rel_message_end = tpp_file_ptr2rel(file, directive_iter);
	token->tt_start = tpp_file_rel2ptr(file, rel_token_start);

	/* Load range of message string. */
	message_start = tpp_file_rel2ptr(file, rel_message_start);
	message_end   = tpp_file_rel2ptr(file, rel_message_end);

	/* Remember that this is where the next token should begin. */
	token->tt_end = directive_iter;
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);

	/* Strip leading/trailing whitespace */
#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(tpp_lexer_getfile(self))) {
		while (message_start < message_end) {
			tpp_char const *nstart = message_start;
			tpp_unichar uc = tpp_unicode_readutf8(&nstart, message_end);
			if (!tpp_unicode_isspace(uc))
				break;
			message_start = nstart;
		}
		while (message_start < message_end) {
			tpp_char const *nend = message_end;
			tpp_unichar uc = tpp_unicode_readutf8_rev(&nend, message_start);
			if (!tpp_unicode_isspace(uc))
				break;
			message_end = nend;
		}
	} else
#endif /* TPP_HAVE_UNICODE */
	{
		while (message_start < message_end && tpp_ascii_isspace(*message_start))
			++message_start;
		while (message_start < message_end && tpp_ascii_isspace(message_end[-1]))
			--message_end;
	}

	/* Emit error/warning message */
	message_size = (tpp_size)(message_end - message_start);
	switch (token->tt_id) {
	case TPP_KWD_error:
#if TPP_HAVE_TPP_W_ERROR
		error = tpp_lexer_warnf(self, TPP_W_ERROR, (unsigned int)message_size, message_start);
#endif /* TPP_HAVE_TPP_W_ERROR */
		break;
	case TPP_KWD_warning:
#if TPP_HAVE_TPP_W_WARNING
		error = tpp_lexer_warnf(self, TPP_W_WARNING, (unsigned int)message_size, message_start);
#endif /* TPP_HAVE_TPP_W_WARNING */
		break;
	default: tpp_unreachable();
	}
	return TPP_TOK_OFERR_OR_EOF(error);
}
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */


#if TPP_HAVE_CPP_DEFINE
/* Handle a "#define" directive, with "self" pointing at the macro's name-keyword
 * @return: TPP_TOK_ISERR: Error
 * @return: TPP_TOK_EOF: Success; caller should yield the next raw token */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_define_directive(tpp_lexer *tpp_restrict self);

static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_define_directive(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
	do {
		result = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
	if (TPP_TOK_ISERR(result))
		return result;
	if (!TPP_TOK_ISKEYWORD(result)) {
#if TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
		tpp_errno error;
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE, "define");
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
		return tpp_lexer_yieldraw_eol(self);
	}
	return tpp_lexer_process_define_directive(self);
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_undef_directive(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
	do {
		result = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
	if (TPP_TOK_ISERR(result))
		return result;
	if (!TPP_TOK_ISKEYWORD(result)) {
#if TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
		tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE, "undef");
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
	} else {
		/* Delete keyword definition */
		tpp_keyword const *ro_keyword = tpp_lexer_gettoken(self)->tt_kwd;
		if (tpp_keyword_canundef(ro_keyword)) {
			tpp_keyword *keyword = tpp_keywords_copybuiltin(&self->tl_kwds, ro_keyword);
			if tpp_unlikely(!keyword)
				return TPP_TOK_ENOMEM;
			tpp_assert(tpp_keyword_canundef(keyword));
			tpp_keyword_undef(keyword);
		} else
#if TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
		if (tpp_lexer_getkeyworddefined(self, ro_keyword)) {
			/* Builtin keyword... */
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_CANNOT_UNDEF_BUILTIN_MACRO);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		} else
#endif /* TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO */
		{
		}
	
		/* Seek to next token (which should be a line-feed) */
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
			return TPP_TOK_EOF;
		} else {
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, "undef");
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	}
	return tpp_lexer_yieldraw_eol(self);
}
#endif /* TPP_HAVE_CPP_DEFINE */



/* Delete "tpp_keyword_misc::tkm_file_guard" for the current file if appropriate
 * - Must be called just before calling "tpp_ifdef_stack_append()", to deal with
 *   the case of a file having multiple top-level #if-blocks (in which case the
 *   file can't have a #ifndef-style #include-guard)
 * - Must also be called when returning a token from tpp_lexer_yieldpp()
 */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
static TPP_NONNULL((1)) void TPPCALL
tpp_file_maybe_delete_include_guard_keyword(tpp_file *tpp_restrict self) {
	if (tpp_ifdef_stack_isempty(tpp_file_getifdef(self)) &&
	    self->tf_kind == TPP_FILE_KIND_IO &&
#if TPP_HAVE_FILE_NOKWD
	    !(self->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
	    self->tf_data.td_io.tff_name != NULL) {
		tpp_keyword *kwd;
		tpp_keyword_misc *misc;
		kwd = (tpp_keyword *)((char const *)self->tf_data.td_io.tff_name -
		                      tpp_offsetof(tpp_keyword, tk_kwd));
		misc = tpp_keyword_getmisc(kwd);
		if (misc && misc->tkm_file_guard &&
		    !(misc->tkm_flags & TPP_KEYWORD_FLAG_HDR_GUARD_VALID))
			misc->tkm_file_guard = NULL;
	}
}
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#define tpp_file_maybe_delete_include_guard_keyword(self) (void)0
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */

#if TPP_HAVE_CPP_IF_ELSE_ENDIF
/* Call with the current token loaded as "if" or "elif"
 * @param: p_directive_start: [out] On success (TPP_EOK or TPP_ENOENT), set
 *                                  to the start of the "if" or "elif" keyword
 * @return: TPP_EOK:    Directive evaluates to "true"
 * @return: TPP_ENOENT: Directive evaluates to "false"
 * @return: * :         Error */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_if_directive(tpp_lexer *tpp_restrict self,
                             tpp_char const **p_directive_start) {
	tpp_errno result;
	tpp_token_id tok;
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *trailing_lf_start;
	tpp_char const *trailing_lf_end;
	tpp_char const *directive_iter;
	tpp_expr_value expr_value;
	tpp_size directive_keyword_len;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	char const *directive_name = (char const *)token->tt_kwd->tk_kwd;
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	directive_iter = file->tf_pos;
	file->tf_pos = token->tt_start; /* Retain start of "if" / "elif" keyword */
	directive_keyword_len = (tpp_size)(directive_iter - file->tf_pos);

	/* Seek end-of-line */
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
		if (TPP_TOK_ISERR(tok)) {
			tpp_assert(TPP_TOK_ASERR(tok) != TPP_ENOENT);
			return TPP_TOK_ASERR(tok);
		}
	} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));

	trailing_lf_start = token->tt_start;
	trailing_lf_end   = directive_iter;
	tpp_file_pushifdef(file);
	tpp_file_pusheof(file);
	*p_directive_start = file->tf_pos;     /* Restore to continue pointing at effective start of expression */
	file->tf_end = trailing_lf_start;      /* Mark as EOF */
	file->tf_pos += directive_keyword_len; /* Skip over leading keyword */

	/* Parse expression */
	result = tpp_lexer_parseexpr(self, &expr_value);

	/* Evaluate expression result (and warn about trailing tokens) */
	if (!TPP_ISERR(result)) {
		bool b_expr_value;
		result = tpp_expr_value_asbool(self, &expr_value, &b_expr_value);
		tpp_expr_value_fini(&expr_value);
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		if (!TPP_ISERR(result) && tpp_lexer_gettok(self) != TPP_TOK_EOF)
			result = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, directive_name);
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
		if (!TPP_ISERR(result))
			result = b_expr_value ? TPP_EOK : TPP_ENOENT;
	}

#if TPP_HAVE_INCLUDE_STACK
	/* Cleanup files pushed by the expression */
	while (file->tf_prev)
		tpp_lexer_popfile(self);
#endif /* TPP_HAVE_INCLUDE_STACK */
	file->tf_pos = trailing_lf_end; /* Tell caller to continue parsing *after* EOL */
	tpp_file_popeof(file);
	tpp_file_popifdef(file);
	return result;
}

/* Call with the current token loaded as "ifdef", "ifndef", "elifdef" or "elifndef"
 * @param: p_directive_start: [out] On success (TPP_EOK or TPP_ENOENT), set
 *                                  to the start of the "ifdef", ... keyword
 * @return: TPP_EOK:    Directive evaluates to "true"
 * @return: TPP_ENOENT: Directive evaluates to "false"
 * @return: * :         Error */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_ifdef_directive_ex(tpp_lexer *tpp_restrict self,
                                   tpp_char const **p_directive_start,
                                   tpp_keyword const **p_macro_keyword)
#define tpp_lexer_parse_ifdef_directive(self, p_directive_start) \
	tpp_lexer_parse_ifdef_directive_ex(self, p_directive_start, NULL)
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_ifdef_directive(tpp_lexer *tpp_restrict self,
                                tpp_char const **p_directive_start)
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
{
	tpp_errno result;
	tpp_token_id tok;
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *directive_iter;
	tpp_token_id const mode = token->tt_id;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	char const *directive_name = (char const *)token->tt_kwd->tk_kwd;
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	bool is_keyword_defined;
	directive_iter = file->tf_pos;
	file->tf_pos = token->tt_start; /* Retain start of "ifdef" / "ifndef" keyword */

	/* Skip over space tokens to find the (presumably) keyword to test for being defined. */
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok)) {
		file->tf_pos = directive_iter;
		return TPP_TOK_ASERR(tok);
	}

	/* Check if keyword is defined */
	if (TPP_TOK_ISKEYWORD(tok)) {
		is_keyword_defined = tpp_lexer_getkeyworddefined(self, tpp_lexer_gettokenkwd(self));
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
		if (p_macro_keyword)
			*p_macro_keyword = tpp_lexer_gettokenkwd(self);
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF
		tpp_char const *saved_pos = file->tf_pos;
		file->tf_pos = directive_iter;
		result = tpp_lexer_warnf(self, TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF, directive_name);
		file->tf_pos = saved_pos;
		if (TPP_ISERR(result))
			return result;
#endif /* TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF */
		is_keyword_defined = false;
	}

	/* Warn about extra tokens after the #ifdef-keyword */
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok)) {
			file->tf_pos = directive_iter;
			return TPP_TOK_ASERR(tok);
		}
	}
	if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		tpp_char const *saved_pos = file->tf_pos;
		file->tf_pos = directive_iter;
		result = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, directive_name);
		file->tf_pos = saved_pos;
		if (TPP_ISERR(result))
			return result;
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
			if (TPP_TOK_ISERR(tok)) {
				tpp_assert(TPP_TOK_ASERR(tok) != TPP_ENOENT);
				return TPP_TOK_ASERR(tok);
			}
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
	}
#else /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
		if (TPP_TOK_ISERR(tok)) {
			tpp_assert(TPP_TOK_ASERR(tok) != TPP_ENOENT);
			return TPP_TOK_ASERR(tok);
		}
	}
#endif /* !TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */

	/* Load "directive_iter" into file, and extract start of directive */
	*p_directive_start = file->tf_pos;
	file->tf_pos = directive_iter;

	if (mode == TPP_KWD_ifndef ||
	    mode == TPP_KWD_elifndef)
		is_keyword_defined = !is_keyword_defined;
	return is_keyword_defined ? TPP_EOK : TPP_ENOENT;
}

/* Load the next #ifdef-like directive into "self", and return it.
 * On entry, allowed to be pretty much anywhere (method starts out
 * by seeking the next newline, then scanning for directives from
 * there on...)
 *
 * @return: TPP_KWD_ifdef:    Found an #ifdef-directive (current token points at like "# [ifdef] foo")
 * @return: TPP_KWD_ifndef:   Found an #ifdef-directive (current token points at like "# [ifndef] foo")
 * @return: TPP_KWD_elif:     Found an #ifdef-directive (current token points at like "# [elif] foo")
 * @return: TPP_KWD_elifdef:  Found an #ifdef-directive (current token points at like "# [elifdef] foo")
 * @return: TPP_KWD_elifndef: Found an #ifdef-directive (current token points at like "# [elifndef] foo")
 * @return: TPP_KWD_else:     Found an #ifdef-directive (current token points at like "# [else]")
 * @return: TPP_KWD_endif:    Found an #ifdef-directive (current token points at like "# [endif]")
 * @return: TPP_TOK_EOF:      End-of-file (no warning issued, yet)
 * @return: TPP_TOK_ISERR(*): Error
 */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seek_next_ifdef_directive(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok = tpp_lexer_gettok(self);

	/* Seek until next line-feed */
seek_next_lf:
	while (!TPP_TOK_ISLF_OR_COMMENT(tok)) {
		tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok) || tok == TPP_TOK_EOF)
			return tok;
	}

	/* Seek next non-whitespace/comment token (i.e.: the first non-whitespace token) */
	do {
		tok = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* First non-whitespace/comment token must be '#' */
	if (tok != '#') {
		/* Deal with shell comment tokens (must be re-interpreted as directives) */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (tok == TPP_TOK_SHELL_COMMENT) {
			tpp_token *const token = tpp_lexer_gettoken(self);
			token->tt_end = token->tt_start + 1;
/*			token->tt_id = tok = TPP_TOK_OFCHAR('#'); * Not needed */
		} else
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		{
			goto seek_next_lf;
		}
	}

	/* Find token that comes after the leading '#'
	 * -> This (may be) the that our caller is interested in. */
	do {
		tok = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	switch (tok) {

	case TPP_KWD_if:
	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef:
	case TPP_KWD_elif:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
	case TPP_KWD_else:
	case TPP_KWD_endif:
		/* Found a preprocessor directive of interest */
		return tok;

		/* Special case for #error / #warning directives: allow incomplete strings
		 * >> #if 0
		 * >> #error That's allowed
		 * >> #endif
		 *
		 * >> #if 0
		 * >> #not_error But this isn't
		 * >> #endif
		 */
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
	{
		tpp_errno error;
#if TPP_HAVE_CPP_ERROR
		if (0) {
	case TPP_KWD_error:
			if (!tpp_lexer_has(self, CPP_ERROR))
				break;
		}
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
		if (0) {
	case TPP_KWD_warning:
			if (!tpp_lexer_has(self, CPP_WARNING))
				break;
		}
#endif /* TPP_HAVE_CPP_WARNING */
		error = tpp_lexer_seek_eol(self, &tpp_lexer_gettoken(self)->tt_end
		                           tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		break;
	}
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */

	default: break;
	}
	goto seek_next_lf;
}

/* Seek end of an inactive "#if 1 ... #else"-style block.
 * - Warn about "#elif" / "#else" directives via "TPP_W_ELIF_OR_ELSE_AFTER_ELSE"
 */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_next_unmatched_endif(tpp_lexer *tpp_restrict self,
                                           tpp_ifdef_stack_entry *ifdef_entry) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	tpp_token_id tok;
again:
	tok = tpp_lexer_seek_next_ifdef_directive(self);
	switch (tok) {
	case TPP_TOK_EOF:
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
		error = tpp_lexer_warnf_lc(self, ifdef_entry->tidse_created, TPP_W_EOF_BEFORE_ENDIF);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */
		return TPP_EOK;

	case TPP_KWD_if:
	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef: {
		/* Skip over nested block */
		tpp_ifdef_stack_entry temp_entry;
		temp_entry.tidse_mode    = TPP_IFDEF_MODE_IFDEF;
		temp_entry.tidse_created = tpp_file_lcinfo(file, tpp_lexer_gettokenstart(self));
		temp_entry.tidse_updated = temp_entry.tidse_created;
		error = tpp_lexer_seek_end_of_next_unmatched_endif(self, &temp_entry);
		if (TPP_ISERR(error))
			return error;
	}	break;

	case TPP_KWD_elif:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
	case TPP_KWD_else: {
#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE
		if (ifdef_entry->tidse_mode == TPP_IFDEF_MODE_ELSE) {
			error = tpp_lexer_warnf(self, TPP_W_ELIF_OR_ELSE_AFTER_ELSE, ifdef_entry,
			                        (char const *)tpp_lexer_gettokenkwd(self)->tk_kwd);
			if (TPP_ISERR(error))
				return error;
		}
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
		if (tok == TPP_KWD_else)
			ifdef_entry->tidse_mode = TPP_IFDEF_MODE_ELSE;
		ifdef_entry->tidse_updated = tpp_file_lcinfo(file, tpp_lexer_gettokenstart(self));
	}	break;

	case TPP_KWD_endif:
		/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return error;
		}
		while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
#else /* TPP_HAVE_TPP_W_ENDIF_LABELS */
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
#endif /* !TPP_HAVE_TPP_W_ENDIF_LABELS */
		return TPP_EOK;

	default:
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		break;
	}
	goto again;
}

/* Seek end of an inactive "#if 0"-style block.
 * - If a "#else" or "#elif 1"-style block is found, push+create a
 *   new #ifdef-entry using "ifdef_location" as the created-position
 * - If a "#endif" is found, behave like "tpp_lexer_seek_end_of_next_unmatched_endif" */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_inactive_ifdef(tpp_lexer *tpp_restrict self,
                                     tpp_lcinfo ifdef_location) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	tpp_token_id tok;
again:
	tok = tpp_lexer_seek_next_ifdef_directive(self);
	switch (tok) {
	case TPP_TOK_EOF:
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
		error = tpp_lexer_warnf_lc(self, ifdef_location, TPP_W_EOF_BEFORE_ENDIF);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */
		return TPP_EOK;

	case TPP_KWD_if:
	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef: {
		/* Skip over nested block */
		tpp_ifdef_stack_entry temp_entry;
		temp_entry.tidse_mode    = TPP_IFDEF_MODE_IFDEF;
		temp_entry.tidse_created = tpp_file_lcinfo(file, tpp_lexer_gettokenstart(self));
		temp_entry.tidse_updated = temp_entry.tidse_created;
		error = tpp_lexer_seek_end_of_next_unmatched_endif(self, &temp_entry);
		if (TPP_ISERR(error))
			return error;
	}	break;

	{
		tpp_char const *directive_start;
		tpp_ifdef_stack_entry *ifdef_entry;
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
		error = tpp_lexer_parse_ifdef_directive(self, &directive_start);
		goto handle_pp_if_error;
	case TPP_KWD_elif:
		error = tpp_lexer_parse_if_directive(self, &directive_start);
handle_pp_if_error:
		if (error == TPP_ENOENT)
			goto again; /* *waves hand* "These aren't the #elif-s you're looking for..." */
		if (TPP_ISERR(error))
			return error;

		/* Create a new #ifdef-entry */
		tpp_file_maybe_delete_include_guard_keyword(file);
		ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
		if tpp_unlikely(!ifdef_entry)
			return TPP_ENOMEM;
		ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_IFDEF;
		ifdef_entry->tidse_created = ifdef_location;
		ifdef_entry->tidse_updated = tpp_file_lcinfo(file, directive_start);
		return TPP_EOK;
	}	break;


	case TPP_KWD_else: {
		tpp_ifdef_stack_entry *ifdef_entry;
		tpp_lcinfo updated_at = tpp_file_lcinfo(file, tpp_lexer_gettokenstart(self));

		/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return error;
		}
		while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
#else /* TPP_HAVE_TPP_W_ENDIF_LABELS */
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
#endif /* !TPP_HAVE_TPP_W_ENDIF_LABELS */

		/* Create a new #ifdef-entry */
		tpp_file_maybe_delete_include_guard_keyword(file);
		ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
		if tpp_unlikely(!ifdef_entry)
			return TPP_ENOMEM;
		ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_IFDEF;
		ifdef_entry->tidse_created = ifdef_location;
		ifdef_entry->tidse_updated = updated_at;
		return TPP_EOK;
	}	break;

	case TPP_KWD_endif:
		/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return error;
		}
		while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
#else /* TPP_HAVE_TPP_W_ENDIF_LABELS */
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
#endif /* !TPP_HAVE_TPP_W_ENDIF_LABELS */
		return TPP_EOK;

	default:
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		break;
	}
	goto again;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_if_directive(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	tpp_ifdef_stack_entry *ifdef_entry;
	tpp_char const *directive_start;
	tpp_assert(tpp_lexer_gettok(self) == TPP_KWD_if ||
	           tpp_lexer_gettok(self) == TPP_KWD_ifdef ||
	           tpp_lexer_gettok(self) == TPP_KWD_ifndef);
	switch (tpp_lexer_gettok(self)) {

#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
	case TPP_KWD_ifndef: {
		tpp_keyword const *ifndef_keyword = NULL;
		error = tpp_lexer_parse_ifdef_directive_ex(self, &directive_start, &ifndef_keyword);
		if (!(file->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOGUARD) &&
		    (error == TPP_EOK || error == TPP_ENOENT) && ifndef_keyword != NULL &&
		    file->tf_kind == TPP_FILE_KIND_IO && tpp_ifdef_stack_isempty(tpp_file_getifdef(file)) &&
#if TPP_HAVE_FILE_NOKWD
		    !(file->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
		    file->tf_data.td_io.tff_name != NULL) {
			tpp_keyword *kwd;
			tpp_keyword_misc *misc;
			kwd = (tpp_keyword *)((char const *)file->tf_data.td_io.tff_name -
			                      tpp_offsetof(tpp_keyword, tk_kwd));
			kwd = tpp_keywords_copybuiltin(&self->tl_kwds, kwd);
			if tpp_unlikely(!kwd)
				return TPP_TOK_ENOMEM;
			misc = tpp_keyword_requiremisc(kwd);
			if tpp_unlikely(!misc)
				return TPP_TOK_ENOMEM;
			misc->tkm_file_guard = ifndef_keyword;
			if (error == TPP_ENOENT)
				goto do_seek_end_of_ifdef_block;
			/* XXX: -Wheader-guard */
			goto do_append_to_ifdef_stack;
		}
	}	break;
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	case TPP_KWD_ifndef:
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	case TPP_KWD_ifdef:
		error = tpp_lexer_parse_ifdef_directive(self, &directive_start);
		break;
	case TPP_KWD_if:
		error = tpp_lexer_parse_if_directive(self, &directive_start);
		break;
	default: tpp_unreachable();
	}
	if (error == TPP_ENOENT) {
		/* false-condition -> seek end-of-block */
		tpp_lcinfo created_at;
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
do_seek_end_of_ifdef_block:
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
		created_at = tpp_file_lcinfo(file, directive_start);
		error = tpp_lexer_seek_end_of_inactive_ifdef(self, created_at);
		return TPP_TOK_OFERR_OR_EOF(error);
	}
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tpp_file_maybe_delete_include_guard_keyword(file);
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
do_append_to_ifdef_stack:
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
	if tpp_unlikely(!ifdef_entry)
		return TPP_TOK_ENOMEM;
	ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_IFDEF;
	ifdef_entry->tidse_created = tpp_file_lcinfo(file, directive_start);
	ifdef_entry->tidse_updated = ifdef_entry->tidse_created;
	return TPP_TOK_EOF;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_else_directive(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_errno error;
	tpp_lcinfo lc_update;
	tpp_ifdef_stack_entry *ifdef_entry;
	lc_update = tpp_file_lcinfo(file, token->tt_start);

	/* Check for error-case: #ifdef-stack is empty */
	if (tpp_ifdef_stack_isempty(tpp_file_getifdef(file))) {
#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF
		error = tpp_lexer_warnf(self, TPP_W_ELIF_OR_ELSE_WITHOUT_IF,
		                        (char const *)token->tt_kwd->tk_kwd);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF */
		switch (token->tt_id) {
		case TPP_KWD_elif:
			token->tt_id = TPP_KWD_if;
			return tpp_lexer_handle_if_directive(self);
		case TPP_KWD_elifdef:
			token->tt_id = TPP_KWD_ifdef;
			return tpp_lexer_handle_if_directive(self);
		case TPP_KWD_elifndef:
			token->tt_id = TPP_KWD_ifndef;
			return tpp_lexer_handle_if_directive(self);
		case TPP_KWD_else:
			break;
		default: tpp_unreachable();
		}
		tpp_file_maybe_delete_include_guard_keyword(file);
		ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
		if tpp_unlikely(!ifdef_entry)
			return TPP_TOK_ENOMEM;
		ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_ELSE;
		ifdef_entry->tidse_created = lc_update;
		ifdef_entry->tidse_updated = lc_update;
		return tpp_lexer_yieldraw_eol(self);
	}

	/* Load the most-recent #ifdef-stack entry */
	ifdef_entry = tpp_ifdef_stack_getlast(tpp_file_getifdef(file));
#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE
	if (ifdef_entry->tidse_mode == TPP_IFDEF_MODE_ELSE) {
		error = tpp_lexer_warnf(self, TPP_W_ELIF_OR_ELSE_AFTER_ELSE, ifdef_entry,
		                        (char const *)token->tt_kwd->tk_kwd);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
	if (token->tt_id == TPP_KWD_else)
		ifdef_entry->tidse_mode = TPP_IFDEF_MODE_ELSE;
	ifdef_entry->tidse_updated = lc_update;

	/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
	if (token->tt_id == TPP_KWD_else) {
		tpp_token_id result;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
			error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
	}
#endif /* TPP_HAVE_TPP_W_ENDIF_LABELS */

	/* Seek end of next unmatched #endif-directive */
	error = tpp_lexer_seek_end_of_next_unmatched_endif(self, ifdef_entry);
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);

	/* Remove #ifdef-stack entry */
	tpp_assert(!tpp_ifdef_stack_isempty(tpp_file_getifdef(file)));
	tpp_assert(ifdef_entry == tpp_ifdef_stack_getlast(tpp_file_getifdef(file)));
	tpp_ifdef_stack_remove(tpp_file_getifdef(file));
	return TPP_TOK_EOF;
}


static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_endif_directive(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	if (tpp_ifdef_stack_isempty(tpp_file_getifdef(file))) {
#if TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF
		tpp_errno error = tpp_lexer_warnf(self, TPP_W_ENDIF_WITHOUT_IF);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF */
		return tpp_lexer_yieldraw_eol(self);
	}

	/* Remove #ifdef-stack entry */
	tpp_assert(!tpp_ifdef_stack_isempty(tpp_file_getifdef(file)));
	tpp_ifdef_stack_remove(tpp_file_getifdef(file));

	/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
	{
		tpp_token_id result;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
	}
#endif /* TPP_HAVE_TPP_W_ENDIF_LABELS */

	return tpp_lexer_yieldraw_eol(self);
}
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */


/* Process a preprocessor directive, with the currently loaded token being the leading '#'
 * Upon successful return (!TPP_TOK_ISERR(return)), the caller will yield another raw token
 * @return: TPP_TOK_ISERR         : Error
 * @return: TPP_TOK_EOF           : Caller should yield the next raw token
 * @return: TPP_TOK_SHELL_COMMENT : Directive was transformed to a shell-comment which the caller should re-emit */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_directive(tpp_lexer *tpp_restrict self) {
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#define tpp_lexer_process_directive_set_noguard() (file->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_NOGUARD)
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#define tpp_lexer_process_directive_set_noguard() (void)0
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *directive_iter = token->tt_end;
	tpp_token_id result;
	tpp_assert(token->tt_id == '#');

	/* Make sure that the start of the directive (the #-token) remains loaded. */
	file->tf_pos = token->tt_start;

	/* Prevent the directive from continuing into another file */
	tpp_lexer_autopopfile_pushoff(self);

	/* Load token that comes after leading '#' */
again_yield_directive_iter:
	result = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
	switch (result) {

	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		/* Skip over multi-line comment tokens to allow code like:
		 * >> #(*
		 * >> comment
		 * >> *)define foo 42 */
	case TPP_TOK_SPACE:
		goto again_yield_directive_iter;

/************************************************************************/
#if TPP_HAVE_CPP_BLANK
	TPP_CASE_TPP_TOK_COMMENT_LINE
		/* line-like comment tokens must be treated as the "blank" directive,
		 * since they effective cap-off the directive via a commented line-feed */
	case TPP_TOK_EOF:
	case TPP_TOK_LF:
		if (!tpp_lexer_has(self, CPP_BLANK))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		result = TPP_TOK_EOF;
		break;
#endif /* TPP_HAVE_CPP_BLANK */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EXCLAIM
	case '!':
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	case TPP_TOK_EXCLAIM_EXCLAIM:
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
		if (!tpp_lexer_has(self, CPP_EXCLAIM))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_EXCLAIM */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIGIT_LINE
	case TPP_TOK_INT:
		if (!tpp_lexer_has(self, CPP_DIGIT_LINE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		tpp_lexer_process_directive_set_noguard();

		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_DIGIT_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_LINE
	case TPP_KWD_line:
		if (!tpp_lexer_has(self, CPP_LINE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		tpp_lexer_process_directive_set_noguard();

		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED
	{
#if TPP_HAVE_CPP_INCLUDE
	case TPP_KWD_include:
		if (!tpp_lexer_has(self, CPP_INCLUDE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_HAVE_CPP_INCLUDE */
#if TPP_HAVE_CPP_INCLUDE_NEXT
		if (0) {
	case TPP_KWD_include_next:
			if (!tpp_lexer_has(self, CPP_INCLUDE_NEXT))
				goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		}
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
#if TPP_HAVE_CPP_IMPORT
		if (0) {
	case TPP_KWD_import:
			if (!tpp_lexer_has(self, CPP_IMPORT))
				goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		}
#endif /* TPP_HAVE_CPP_IMPORT */
		tpp_lexer_process_directive_set_noguard();

		/* TODO */
		/* TODO: Must "tpp_lexer_autopopfile_break(self);" before pushing the new file,
		 *       and once the file's been pushed, must directly "return" to skip the pop
		 *       operation happening again below. */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}
#endif /* TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EMBED
	case TPP_KWD_embed: {
		if (!tpp_lexer_has(self, CPP_EMBED))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		tpp_lexer_process_directive_set_noguard();
		/* TODO: #embed  (https://en.cppreference.com/c/preprocessor/embed) */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}	break;
#endif /* TPP_HAVE_CPP_EMBED */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
	case TPP_KWD_ifndef:
	case TPP_KWD_ifdef:
	case TPP_KWD_if:
		if (!tpp_lexer_has(self, CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		file->tf_pos = directive_iter;
		tpp_lexer_process_directive_set_noguard();
		result = tpp_lexer_handle_if_directive(self);
		break;

	case TPP_KWD_elif:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
	case TPP_KWD_else:
		if (!tpp_lexer_has(self, CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		tpp_lexer_process_directive_set_noguard();
		file->tf_pos = directive_iter;
		result = tpp_lexer_handle_else_directive(self);
		break;

	case TPP_KWD_endif:
		if (!tpp_lexer_has(self, CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		/*tpp_lexer_process_directive_set_noguard();*/ /* Not needed... */
		file->tf_pos = directive_iter;
		result = tpp_lexer_handle_endif_directive(self);
		break;
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DEFINE
	case TPP_KWD_define:
		if (!tpp_lexer_has(self, CPP_DEFINE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		tpp_lexer_process_directive_set_noguard();
		token->tt_end = directive_iter;
		result = tpp_lexer_handle_define_directive(self);
		break;
	case TPP_KWD_undef:
		if (!tpp_lexer_has(self, CPP_DEFINE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		tpp_lexer_process_directive_set_noguard();
		token->tt_end = directive_iter;
		result = tpp_lexer_handle_undef_directive(self);
		break;
#endif /* TPP_HAVE_CPP_DEFINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ASSERT
	case TPP_KWD_assert:
	case TPP_KWD_unassert:
		if (!tpp_lexer_has(self, CPP_ASSERT))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		tpp_lexer_process_directive_set_noguard();
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_ASSERT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
#if TPP_HAVE_CPP_ERROR
	case TPP_KWD_error:
		if (!tpp_lexer_has(self, CPP_ERROR))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
		if (0) {
	case TPP_KWD_warning:
			if (!tpp_lexer_has(self, CPP_WARNING))
				goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		}
#endif /* TPP_HAVE_CPP_WARNING */
		tpp_lexer_process_directive_set_noguard();
		result = tpp_lexer_handle_error_directive(self, directive_iter);
		break;
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IDENT_SCCS
	case TPP_KWD_ident:
	case TPP_KWD_sccs:
		if (!tpp_lexer_has(self, CPP_IDENT_SCCS))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		tpp_lexer_process_directive_set_noguard();
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_IDENT_SCCS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_PRAGMA
	case TPP_KWD_pragma:
		if (!tpp_lexer_has(self, CPP_PRAGMA))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
		tpp_lexer_process_directive_set_noguard();
		token->tt_end = directive_iter;
		result = tpp_lexer_handle_pragma_directive(self);
		break;
#endif /* TPP_HAVE_CPP_PRAGMA */
/************************************************************************/



	default:
		if (TPP_TOK_ISERR(result))
			goto return_result;
#ifdef WANT_handle_unknown_directive
#undef WANT_handle_unknown_directive
handle_unknown_directive:
#endif /* WANT_handle_unknown_directive */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (tpp_lexer_has(self, TPP_TOK_SHELL_COMMENT)
#if TPP_HAVE_DIGRAPHS
		    && *file->tf_pos != '%' /* Digraph "%:" must never become a shell comment (since
		                             * "%:" is a **TOKEN** substitution, but not a **CHARACTER**
		                             * substitution like "??=" is) */
#endif /* TPP_HAVE_DIGRAPHS */
		    ) {
			/* Seek next end-of-line character (rather than next end-of-line token)
			 * The difference here comes into play when considering that multi-line
			 * comment tokens can disguise line-feed tokens, but since we're to
			 * treat the #-line as a comment on its own, that means that we're
			 * already *within* a comment (semantically speaking), and comment
			 * nesting isn't something that's allowed! */
			tpp_errno error;
			tpp_char const *eol;
			/* "file->tf_pos" was saved as the start of the '#' */
			tpp_assert(tpp_is_start_of_hash(*file->tf_pos));
			token->tt_start = eol = file->tf_pos;
			error = tpp_lexer_seek_eol(self, &eol tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SHELL_COMMENT));
			if (TPP_ISERR(error)) {
				token->tt_start = file->tf_pos;
				token->tt_end = file->tf_pos + 1;
#if TPP_HAVE_TRIGRAPHS
				if (*file->tf_pos == '?') {
					token->tt_end += 2;
				} else
#endif /* TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_DIGRAPHS
				if (*file->tf_pos == '%') {
					token->tt_end += 1;
				} else
#endif /* TPP_HAVE_DIGRAPHS */
				{
				}
				token->tt_id = (tpp_token_id)'#';
				self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
				result = TPP_TOK_OFERR(error);
				goto return_result;
			}
			token->tt_end = eol;
			token->tt_id = TPP_TOK_SHELL_COMMENT;

			/* Tell caller to re-emit what we thought was a directive as a shell comment */
			result = TPP_TOK_SHELL_COMMENT;
			goto return_result;
		} else
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		{
			tpp_lexer_process_directive_set_noguard();
#if defined(WANT_seek_end_of_line) || TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_SHELL_COMMENT)
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_SHELL_COMMENT)
#if TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE
			{
				tpp_errno error;
				error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_DIRECTIVE);
				if (TPP_ISERR(error)) {
					result = TPP_TOK_OFERR(error);
					goto return_result;
				}
			}
#endif /* TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE */
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_SHELL_COMMENT) */
	
			/* Seek until we hit LF or EOF. Caller has disabled "autopopfile",
			 * so this'll always stay within the file currently being processed. */
#ifdef WANT_seek_end_of_line
#undef WANT_seek_end_of_line
seek_end_of_line:
#endif /* WANT_seek_end_of_line */
			while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
				result = tpp_lexer_yieldraw_blocking(self);
				if (TPP_TOK_ISERR(result))
					goto return_result;
			}
#endif /* WANT_seek_end_of_line || TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_SHELL_COMMENT) */
		}
		tpp_lexer_autopopfile_break(self);
		return TPP_TOK_EOF;
	}
return_result:
	tpp_lexer_autopopfile_pop(self);
	return result;
#undef tpp_lexer_process_directive_set_noguard
}
#endif /* TPP_HAVE_CPP_DIRECTIVES */



/* Wrapper around `tpp_lexer_yieldraw()' that filters certain tokens (based on
 * configured features), and implements handling for preprocessor directives,
 * like "#define", "#include", etc:
 * - TPP_TOK_LF:      Filtered based on `TPP_HAVE_TPP_TOK_LF' / `TPP_FEAT_TPP_TOK_LF'
 * - TPP_TOK_SPACE:   Filtered based on `TPP_HAVE_TPP_TOK_SPACE' / `TPP_FEAT_TPP_TOK_SPACE'
 * - TPP_TOK_COMMENT: Filtered based on `TPP_HAVE_TPP_TOK_COMMENT' / `TPP_FEAT_TPP_TOK_COMMENT'
 *
 * @return: * :                  The newly read token (after accounting for preprocessor directives)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldraw(self);
	switch (result) {


/************************************************************************/
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_SPACE)
	case TPP_TOK_SPACE:
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
		if (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
			break;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#if TPP_CONF_IS_RT(TPP_HAVE_TPP_TOK_SPACE)
		if (tpp_lexer_has(self, TPP_TOK_SPACE))
			break; /* Enabled */
#endif /* TPP_CONF_IS_RT(TPP_HAVE_TPP_TOK_SPACE) */
		goto again;
#elif TPP_HAVE_CPP_DIRECTIVES
	case TPP_TOK_SPACE:
		break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_COMMENTLIKE
	_TPP_CASE_TPP_TOK_SHELL_COMMENT
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT && TPP_HAVE_CPP_DIRECTIVES
		if (!(self->tl_state & TPP_LEXER_STATE_FLAG_NODIRECTIVES) &&
			tpp_lexer_has(self, CPP_DIRECTIVES)) {
			tpp_token *const token = tpp_lexer_gettoken(self);

			/* Must re-parse comment as a preprocessor directive instead! */
			token->tt_id = (tpp_token_id)'#';
			tpp_assert(tpp_is_start_of_hash(*token->tt_start));
			token->tt_end = token->tt_start + 1;
#if TPP_HAVE_TRIGRAPHS
			if (*token->tt_start == '?') {
				token->tt_end += 2;
			} else
#endif /* TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_DIGRAPHS
			if (*token->tt_start == '%') {
				token->tt_end += 1;
			} else
#endif /* TPP_HAVE_DIGRAPHS */
			{
			}

			self->tl_state |= TPP_LEXER_STATE_FLAG_NODIRECTIVES;
			tpp_lexer_autopopfile_pushoff(self); /* TODO: #include-directives need to skip this step */
			result = tpp_lexer_process_directive(self);
			tpp_lexer_autopopfile_pop(self);
			self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
			if (TPP_TOK_ISERR(result))
				break;
			if (result == TPP_TOK_EOF)
				goto again;

			/* Deal with case where PP-directive wasn't recognized,
			 * and should thus be emitted as shell-comment token. */
			tpp_assert(result == TPP_TOK_SHELL_COMMENT);

			/* Fallthru to regular maybe-emit-comment code below... */
		}
		TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT && TPP_HAVE_CPP_DIRECTIVES */
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_COMMENT) /* Never, or conditionally enabled */
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_ASM_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
		TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES */
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
		if (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
			break;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#if TPP_CONF_IS_RT(TPP_HAVE_TPP_TOK_COMMENT)
		if (tpp_lexer_has(self, TPP_TOK_COMMENT))
			break; /* Comments are enabled -> emit to caller */
#endif /* TPP_CONF_IS_RT(TPP_HAVE_TPP_TOK_COMMENT) */
		goto again;
#elif TPP_HAVE_CPP_DIRECTIVES
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_ASM_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		break;
#endif /* ... */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIRECTIVES || TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_LF) /* Never, or conditionally enabled */
	case TPP_TOK_LF: {
#if TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#if TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_LF) /* Never, or conditionally enabled */
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
		if (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
			break;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#if TPP_CONF_IS_RT(TPP_HAVE_TPP_TOK_LF)
		if (tpp_lexer_has(self, TPP_TOK_LF))
			break; /* Enabled */
#endif /* TPP_CONF_IS_RT(TPP_HAVE_TPP_TOK_LF) */
		goto again;
#else /* TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_LF) */
		break;
#endif /* !TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_LF) */
	}
#endif /* TPP_HAVE_CPP_DIRECTIVES || TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_LF) */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIRECTIVES
	case '#':
		if (self->tl_state & TPP_LEXER_STATE_FLAG_NODIRECTIVES)
			break; /* Not allowed here... */
		if (!tpp_lexer_has(self, CPP_DIRECTIVES))
			break; /* Directives are disabled. */
		self->tl_state |= TPP_LEXER_STATE_FLAG_NODIRECTIVES;
		result = tpp_lexer_process_directive(self);
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
		if (TPP_TOK_ISERR(result))
			break;
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (result != TPP_TOK_EOF) {
			/* Emit as a shell-comment (if enabled). Otherwise, check next raw token. */
			tpp_assert(result == TPP_TOK_SHELL_COMMENT);
			if (tpp_lexer_has(self, TPP_TOK_COMMENT))
				break;
		}
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		goto again;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
/************************************************************************/



/************************************************************************/
	default:
		/* Remember that we've seen something that will prevent CPP directives */
#if TPP_HAVE_CPP_DIRECTIVES
		self->tl_state |= TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
		break;
/************************************************************************/
	}

#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
	/* Remember that #include-guards are no longer possible at this
	 * point (because a relevant token "result" was hit first) */
	tpp_lexer_getfile(self)->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_NOGUARD;
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */

	/* Delete a previously recognized #ifndef-guard keyword if
	 * we're at the top-level #ifdef-block for the current file. */
	tpp_file_maybe_delete_include_guard_keyword(tpp_lexer_getfile(self));
	return result;
}


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-yield-macro.c                                      */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS

TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_PAREN) == TPP_TOK_LPAREN);
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_BRACKET) == TPP_TOK_LBRACKET);
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_BRACE) == TPP_TOK_LBRACE);
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_ANGLE) == TPP_TOK_LANGLE);
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

typedef struct tpp_string_buffer {
	tpp_size  tsb_size;  /* Used buffer size */
	tpp_size  tsb_alloc; /* Allocated buffer size */
	tpp_char *tsb_data;  /* [0..1] String buffer */
} tpp_string_buffer;

#define tpp_string_buffer_init(self) \
	(void)((self)->tsb_size  = 0,    \
	       (self)->tsb_alloc = 0,    \
	       (self)->tsb_data  = NULL)
#define tpp_string_buffer_fini(self) \
	tpp_free((self)->tsb_data)

#ifndef TPP_MACRO_ARGUMENT_BUFFER_MINSIZE
#define TPP_MACRO_ARGUMENT_BUFFER_MINSIZE 64
#endif /* !TPP_MACRO_ARGUMENT_BUFFER_MINSIZE */

static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_string_buffer_append(tpp_string_buffer *tpp_restrict self,
                         tpp_char const *data, tpp_size num_bytes) {
	tpp_size min_alloc = self->tsb_size + num_bytes;
	tpp_assert(self->tsb_size <= self->tsb_alloc);
	if (min_alloc > self->tsb_alloc) {
		tpp_char *new_buffer;
		tpp_size new_alloc = self->tsb_alloc * 2;
#if TPP_MACRO_ARGUMENT_BUFFER_MINSIZE > 1
		if (new_alloc < TPP_MACRO_ARGUMENT_BUFFER_MINSIZE)
			new_alloc = TPP_MACRO_ARGUMENT_BUFFER_MINSIZE;
#endif /* TPP_MACRO_ARGUMENT_BUFFER_MINSIZE > 1 */
		if (new_alloc < min_alloc)
			new_alloc = min_alloc;
		new_buffer = (tpp_char *)tpp_tryrealloc(self->tsb_data, new_alloc);
		if tpp_unlikely(!new_buffer) {
			new_alloc = min_alloc;
			new_buffer = (tpp_char *)tpp_realloc(self->tsb_data, new_alloc);
			if tpp_unlikely(!new_buffer)
				return false;
		}
		self->tsb_data  = new_buffer;
		self->tsb_alloc = min_alloc;
	}
	tpp_assert(min_alloc <= self->tsb_alloc);
	tpp_memcpy(self->tsb_data + self->tsb_size, data, num_bytes);
	self->tsb_size += num_bytes;
	return true;
}

#ifdef __OPTIMIZE_SIZE__
#define tpp_string_buffer_truncate(self) (void)0
#else /* __OPTIMIZE_SIZE__ */
static TPP_NONNULL((1)) void TPPCALL
tpp_string_buffer_truncate(tpp_string_buffer *tpp_restrict self) {
	tpp_assert(self->tsb_size <= self->tsb_alloc);
	if (self->tsb_size < self->tsb_alloc) {
		tpp_char *newbuf = (tpp_char *)tpp_tryrealloc(self->tsb_data, self->tsb_size);
		if tpp_likely(newbuf)
			self->tsb_data = newbuf;
	}
}
#endif /* !__OPTIMIZE_SIZE__ */

typedef struct tpp_macro_expinfo {
	tpp_char *tmei_expand_data; /* [0..tmei_expand_size][owned_if(MAYBE)]
	                             * Buffer containing the expanded argument text. */
	tpp_size  tmei_expand_size; /* Length of "tmei_expand_data" (in bytes) */
} tpp_macro_expinfo;

#define tpp_macro_expinfo_fini(self, arginfo)                      \
	((self)->tmei_expand_data != (tpp_char *)(arginfo)->tlai_start \
	 ? tpp_free((self)->tmei_expand_data)                          \
	 : (void)0)
#define tpp_macro_expinfo_getdata(self) ((self)->tmei_expand_data)
#define tpp_macro_expinfo_getsize(self) ((self)->tmei_expand_size)

/* Initialize "self" by expanding `argument_start...argument_end'
 * @return: TPP_EOK: Success
 * @return: * :      Error (must be propagated) */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_macro_expinfo_init(tpp_macro_expinfo *tpp_restrict self,
                       tpp_lexer_arginfo const *tpp_restrict arginfo,
                       tpp_lexer *tpp_restrict lexer) {
	/* Must actually expand argument data! (i.e.: when text contains
	 * "__TPP_EVAL(10+20)", then our output must contain "30")
	 *
	 * NOTES:
	 *  - This can only really be implemented using tpp_lexer_yield(),
	 *    and then appending the start/end ranges of successive tokens
	 *    to a string buffer. However, for this we need a special flag
	 *    for the lexer that will cause it to *always* emit space/lf/
	 *    comment tokens (since those must be retained in expanded
	 *    text).
	 *  - In order to force-overwrite the lexer to only yield tokens
	 *    from the macro-argument area, we can use tpp_file_pusheof()
	 *    (or rather: tpp_file_pusheof_fast, whilst ensuring that
	 *    any extra files pushed by tpp_lexer_yield() are always once
	 *    again popped; including in case of errors)
	 *  - For the sake of performance, "simple" arguments are treated
	 *    specially by not making use of a secondary heap-buffer.
	 *    "simple" here means that tpp_lexer_yield()-ing "arginfo"
	 *    produces an uninterrupted stream of tokens all originating
	 *    from the file providing argument information. Or in other
	 *    words: "simple" means that expanding the argument doesn't
	 *    change anything about it.
	 *
	 * HINT:
	 * - Our caller has set-up a context as follows:
	 *   >> tpp_file_pushchunk(file);
	 *   >> tpp_file_pushifdef(file);
	 *   >> tpp_lexer_alltokens_pushon(lexer);
	 */
	tpp_token const *const token = tpp_lexer_gettoken(lexer);
	tpp_string_buffer buffer;
	tpp_token_id tok;
	tpp_char const *expected_simple_tok_start;
	tpp_assert(tpp_lexer_getfile(lexer)->tf_prev == NULL);
	expected_simple_tok_start = arginfo->tlai_start;

next_tok:
	tok = tpp_lexer_yield(lexer);
	if tpp_unlikely(TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tok == TPP_TOK_EOF) {
		/* Simple case: it's a "simple" argument (that doesn't do anything when expanded) */
		tpp_assert(expected_simple_tok_start <= arginfo->tlai_end);
		self->tmei_expand_data = (tpp_char *)arginfo->tlai_start;
		self->tmei_expand_size = (tpp_size)(expected_simple_tok_start - arginfo->tlai_start);
		goto done;
	}
	if (token->tt_start == expected_simple_tok_start) {
		/* Yup: just a simple continuation */
		expected_simple_tok_start = token->tt_end;
		tpp_assert(expected_simple_tok_start <= arginfo->tlai_end &&
		           "Token spans beyond (expected) EOF?");
		tpp_assert(tpp_lexer_getfile(lexer)->tf_prev == NULL &&
		           "Nothing should have pushed a new file (because "
		           "that wouldn't be 'simple', meaning that the "
		           "'token->tt_start == expected_simple_tok_start'"
		           "check should have failed at some point)");
		goto next_tok;
	}

	/* Complicated case: got a token that isn't adjacent to its predecessor */
	tpp_string_buffer_init(&buffer);
	if (expected_simple_tok_start > arginfo->tlai_start) {
		if (!tpp_string_buffer_append(&buffer, arginfo->tlai_start,
		                              (tpp_size)(expected_simple_tok_start -
		                                         arginfo->tlai_start)))
			goto err_builder_nomem;
	}

	/* Print representation of tokens to "buffer" */
again_print_token:
	if (!tpp_string_buffer_append(&buffer, token->tt_start,
	                              (tpp_size)(token->tt_end -
	                                         token->tt_start)))
		goto err_builder_nomem;
	tok = tpp_lexer_yield(lexer);
	if tpp_unlikely(TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tok != TPP_TOK_EOF)
		goto again_print_token;

	/* EOF reached -> argument fully expanded! */
	tpp_string_buffer_truncate(&buffer);
	self->tmei_expand_data = buffer.tsb_data;
	self->tmei_expand_size = buffer.tsb_size;
done:
	/* Verify that the #ifdef-stack of "file" is empty.
	 * If it isn't, emit warnings and clear it now. */
	return tpp_lexer_warn_nonempty_ifdef(lexer);
err_builder_nomem:
	tpp_string_buffer_fini(&buffer);
	return TPP_ENOMEM;
}

typedef struct tpp_macro_argbuf {
	tpp_lexer_arginfo tmab_arginfo[TPP_FLEX_ARRAY]; /* [:tmf_argc] Origin argument info vector */
/*	tpp_macro_expinfo tmab_expinfo[TPP_FLEX_ARRAY];  * [:tmf_argc] Argument expansion info vector. */
} tpp_macro_argbuf;

#define tpp_macro_argbuf_sizeof(argc) \
	((argc) * (sizeof(tpp_lexer_arginfo) + sizeof(tpp_macro_expinfo)))
#define tpp_macro_argbuf_getarginfo(self, argc) \
	((self)->tmab_arginfo)
#define tpp_macro_argbuf_getexpinfo(self, argc) \
	((tpp_macro_expinfo *)((self)->tmab_arginfo + (argc)))


/* >> tpp_lexer_seek_rparen_flags_frommacro(macro)
 * Return `TPP_LEXER_SEEK_RPAREN_FLAG_*' flags needed by "macro" */
#if !TPP_HAVE_MACRO_FLAGS
#define tpp_lexer_seek_rparen_flags_frommacro(macro) TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL
#else /* !TPP_HAVE_MACRO_FLAGS */
#if defined(TPP_MACRO_FLAG_VARIADIC) && !defined(TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS)
#error "Invalid configuration: macros require varargs support, but 'TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS' isn't available"
#endif /* !TPP_MACRO_FLAG_VARIADIC && !TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS */
#if defined(TPP_MACRO_FLAG_KEEPARGSPC) && !defined(TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC)
#error "Invalid configuration: macros require keep-argument-space support, but 'TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC' isn't available"
#endif /* !TPP_MACRO_FLAG_KEEPARGSPC && !TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC */
#undef TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON
#if ((defined(TPP_MACRO_FLAG_VARIADIC) && TPP_MACRO_FLAG_VARIADIC == TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS) && \
     (defined(TPP_MACRO_FLAG_KEEPARGSPC) && TPP_MACRO_FLAG_KEEPARGSPC == TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC))
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON (TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS | TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC)
#elif defined(TPP_MACRO_FLAG_VARIADIC) && TPP_MACRO_FLAG_VARIADIC == TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS
#elif (TPP_MACRO_FLAG_KEEPARGSPC) && TPP_MACRO_FLAG_KEEPARGSPC == TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC
#endif
#ifdef TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON
#define tpp_lexer_seek_rparen_flags_frommacro(macro) ((macro)->tm_flags & TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON)
#else /* TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON */
#ifdef TPP_MACRO_FLAG_VARIADIC
#define _tpp_macro_get_seek_flag_varargs(macro) (((macro)->tm_flags & TPP_MACRO_FLAG_VARIADIC) ? TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS : 0)
#else /* TPP_MACRO_FLAG_VARIADIC */
#define _tpp_macro_get_seek_flag_varargs(macro) 0
#endif /* !TPP_MACRO_FLAG_VARIADIC */
#ifdef TPP_MACRO_FLAG_KEEPARGSPC
#define _tpp_macro_get_seek_flag_keepargspc(macro) (((macro)->tm_flags & TPP_MACRO_FLAG_KEEPARGSPC) ? TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC : 0)
#else /* TPP_MACRO_FLAG_KEEPARGSPC */
#define _tpp_macro_get_seek_flag_keepargspc(macro) 0
#endif /* !TPP_MACRO_FLAG_KEEPARGSPC */
#define tpp_lexer_seek_rparen_flags_frommacro(macro) \
	(_tpp_macro_get_seek_flag_varargs(macro) |       \
	 _tpp_macro_get_seek_flag_keepargspc(macro))
#endif /* !TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON */
#endif /* TPP_HAVE_MACRO_FLAGS */


/* Return the invocation argument buffer of "macro"
 * @return: NULL: Out of memory (TPP_ENOMEM) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_argbuf *TPPCALL
tpp_macro_acquire_argbuf(tpp_macro *tpp_restrict macro) {
	tpp_macro_argbuf *result = macro->tm_data.tmd_func.tmf_argbuf;
	if (result == NULL) {
		tpp_size size = tpp_macro_argbuf_sizeof(macro->tm_data.tmd_func.tmf_argc);
		result = (tpp_macro_argbuf *)tpp_malloc(size);
	} else {
		/* Steal buffer (to prevent duplicate buffer use on recursion) */
		macro->tm_data.tmd_func.tmf_argbuf = NULL;
	}
	return result;
}

/* Release an argument buffer back to a macro. */
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_macro_release_argbuf(tpp_macro *tpp_restrict macro,
                         tpp_macro_argbuf *tpp_restrict buffer) {
	if (macro->tm_data.tmd_func.tmf_argbuf == NULL) {
		/* Likely case (when there was no recursion): can cache buffer in macro */
		macro->tm_data.tmd_func.tmf_argbuf = buffer;
	} else {
		/* This case can happen during recursion, when another invocation
		 * of the same macro appears somewhere in the arguments given to it:
		 *
		 * >> #define min(a, b) ((a) < (b) ? (a) : (b))
		 * >> min(10, min(20, 30));
		 * ^ Notice how another call to "min" appears in the arguments of
		 *   an outer call. As such, at one point both calls need to be
		 *   active at the same time, at which point there will need to
		 *   be (at least) 2 "tpp_macro_argbuf" for the same macro.
		 *
		 * The point where we get here is when the outer "min" completes,
		 * and wants to dispose of its argument buffer, but notices that
		 * the macro already has a buffer cached within (namely: the one
		 * that was used by the inner invocation) */
		tpp_free(buffer);
	}
}


#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
static tpp_ssize TPP_FORMATPRINTER_CC
tpp_count_printer(void *arg, tpp_char const *text, tpp_size num_bytes) {
	(void)arg;
	(void)text;
	return (tpp_ssize)num_bytes;
}

static tpp_ssize TPP_FORMATPRINTER_CC
tpp_buffer_printer(void *arg, tpp_char const *text, tpp_size num_bytes) {
	tpp_char **p_dst = (tpp_char **)arg;
	tpp_char *dst = *p_dst;
	tpp_memcpy(dst, text, num_bytes);
	dst += num_bytes;
	*p_dst = dst;
	return 0;
}
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */

/* Perform the expansion of a user-defined, function-style macro.
 *
 * @return: tpp_lexer_gettoken(self)->tt_id : Function-style macro cannot be expanded
 * @return: TPP_TOK_EOF: Success -- caller should yield again to load the
 *                                  first macro's first expansion token.
 * @return: TPP_TOK_ENOMEM: Out of memory */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_token_id TPPCALL
tpp_lexer_expand_macro_function(tpp_lexer *tpp_restrict self,
                                tpp_macro *tpp_restrict macro) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_keyword const *const macro_keyword = tpp_lexer_gettokenkwd(self);
	tpp_char const *rollback_pos;
	tpp_char const *macro_call_start_pos;
#if TPP_HAVE_MACRO_RECURSION
	tpp_size const macro_keyword_len = tpp_lexer_gettokenlen(self);
#endif /* TPP_HAVE_MACRO_RECURSION */
	tpp_token_id tok;
	tpp_token_id lparen_kind;
	tpp_size i, argc;    /* # of arguments given in-source during invocation */
	tpp_size macro_argc; /* == macro->tm_data.tmd_func.tmf_argc */
	tpp_macro_argbuf *argbuf;
	tpp_lexer_arginfo *invoke_arginfo; /* == argbuf->tmab_arginfo */
	tpp_macro_expinfo *invoke_expinfo; /* == argbuf->tmab_expinfo */
	tpp_file *prev_file;
	tpp_string *result_chunk;
	tpp_size result_chunk_size;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	char va_nargs[TPP_UTOA_MAXLEN]; /* Value for __VA_NARGS__ */
	tpp_size va_nargs_len = 0; /* XXX: Initialization doesn't matter; only here to shut up compiler warnings */
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
	tpp_assert(TPP_MACRO_KIND_ISFUNC(macro->tm_kind));

	/* Skip the initial macro-argument-start '('-token */
	lparen_kind = TPP_MACRO_KIND_ASTOK(macro->tm_kind);
	tok = tpp_lexer_tryskip_raw(self, lparen_kind,
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV);
	if (tok != lparen_kind) {
		if (TPP_TOK_ISERR(tok))
			goto err_tok;
		return token->tt_id; /* Missing opening '(' */
	}

	/* Load argument buffer of macro */
	argbuf = tpp_macro_acquire_argbuf(macro);
	if tpp_unlikely(!argbuf)
		return TPP_TOK_ENOMEM;
	argc = macro_argc = macro->tm_data.tmd_func.tmf_argc;
	invoke_arginfo = tpp_macro_argbuf_getarginfo(argbuf, argc);
	invoke_expinfo = tpp_macro_argbuf_getexpinfo(argbuf, argc);

	/* Create a reference to the macro (that will eventually be inherited by the produced file)
	 * Do this very early on because:
	 * - Argument scanning may need to call tpp_lexer_yieldpp(), which might #undef the macro
	 * - Argument expansion may need to call tpp_lexer_yield(), which can do the same */
	tpp_macro_incref(macro);

	/* Enter a block where file popping can be rolled back. */
	tpp_lexer_manualpopfile_start(self);
	tpp_lexer_alltokens_pushon(self);

	/* Parse arguments */
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	tok = tpp_lexer_seekpp_rparen_ex(self, invoke_arginfo, &argc, &rollback_pos,
	                                 (char const *)macro_keyword->tk_kwd,
	                                 tpp_lexer_seek_rparen_flags_frommacro(macro) |
	                                 TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK,
	                                 TPP_MACRO_KIND_ASTOK(macro->tm_kind));
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	tok = tpp_lexer_seekpp_rparen(self, invoke_arginfo, &argc, &rollback_pos,
	                              (char const *)macro_keyword->tk_kwd,
	                              tpp_lexer_seek_rparen_flags_frommacro(macro) |
	                              TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK);
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	if (TPP_TOK_ISERR(tok))
		goto err_tok_macro_argbuf_rollback;
	macro_call_start_pos = file->tf_tpos;

	tpp_assert(macro_argc == macro->tm_data.tmd_func.tmf_argc);
	if (argc < macro_argc) {
		/* Too few arguments */
		for (i = argc; i < macro_argc; ++i) {
			/* Initialize as empty arguments (actually assigned pointer
			 * here doesn't matter; so-long as "tlai_start == tlai_end",
			 * the argument is considered to be empty, and no data will
			 * be dereferenced) */
#if TPP_DEBUG
			invoke_arginfo[i].tlai_start = NULL;
			invoke_arginfo[i].tlai_end   = NULL;
#else /* TPP_DEBUG */
			invoke_arginfo[i].tlai_start = invoke_arginfo[i].tlai_end;
#endif /* !TPP_DEBUG */
			invoke_arginfo[i].tlai_chunk = NULL;
		}

		/* Check for special case: exactly 1 argument is missing, and macro takes varargs.
		 * In this case, the last argument becomes optional and must be treated as empty. */
#ifdef TPP_MACRO_FLAG_VARIADIC
		if ((macro->tm_flags & TPP_MACRO_FLAG_VARIADIC) &&
		    (argc == macro_argc - 1)) {
			/* This is OK */
		} else
#endif /* TPP_MACRO_FLAG_VARIADIC */
		{
#if TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_TOO_FEW_ARGUMENTS,
			                        (char const *)macro_keyword->tk_kwd,
			                        (unsigned int)macro_argc,
			                        (unsigned int)argc);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				goto err_tok_macro_argbuf_rollback_arginfo;
			}
		}
#endif /* TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */
	}

	/* Figure out how much space is needed for the resulting string-chunk */
	result_chunk_size = macro->tm_data.tmd_func.tmf_expbase;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
	if (argc >= macro_argc)
		result_chunk_size += macro->tm_data.tmd_func.tmf_n_vaopt;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	if (macro->tm_data.tmd_func.tmf_n_vanargs) {
		char *endp;
		tpp_size va_nargs_value = 0;
		if (argc >= macro_argc)
			va_nargs_value = (argc - macro_argc) + 1;
		endp = tpp_utoa(va_nargs, va_nargs_value);
		va_nargs_len = (tpp_size)((va_nargs + tpp_lengthof(va_nargs)) - endp);
		tpp_memmovedown(va_nargs, endp, va_nargs_len);
		result_chunk_size += va_nargs_len * macro->tm_data.tmd_func.tmf_n_vanargs;
	}
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/

	/* Account for extra space needed by inserted arguments.
	 * This is also the part where arguments are recursively
	 * expanded */
	{
		tpp_file_autopopfile_pushoff(file); /* tpp_macro_expinfo_init() needs this (to manually re-parse arguments) */
		tpp_file_pushchunk(file);           /* tpp_macro_expinfo_init() needs this (to manually re-parse arguments) */
		tpp_file_pushifdef(file);           /* tpp_macro_expinfo_init() needs this (to ensure no dangling #ifdef-blocks in arguments) */
		for (i = 0; i < macro_argc; ++i) {
			tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
			if (arg->tma_ins_exp) {
				tpp_errno error;
				tpp_macro_expinfo *expand = &invoke_expinfo[i];
				tpp_file_setchunk_fromarg(file, arginfo);
				error = tpp_macro_expinfo_init(expand, arginfo, self);
				if (TPP_ISERR(error)) {
					tok = TPP_TOK_OFERR(error);
					tpp_file_breakifdef(file);
					tpp_file_breakchunk(file);
					tpp_file_autopopfile_break(file);
					goto err_tok_macro_argbuf_rollback_arginfo_expinfo_i;
				}

				/* Account for expanded text */
				result_chunk_size += (arg->tma_ins_exp * tpp_macro_expinfo_getsize(expand));
			}

#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
			if (arg->tma_ins_str) {
				tpp_size raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
				tpp_size str_size = (tpp_size)tpp_token_encodestring(&tpp_count_printer, NULL,
				                                                     arginfo->tlai_start, raw_size);
/*				str_size += 2; * Account for leading/trailing " or ' characters -- Already account for during compilation */
				result_chunk_size += (arg->tma_ins_str * str_size);
			}
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
			if (arg->tma_ins != 0) {
				tpp_size raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
				result_chunk_size += (arg->tma_ins * raw_size);
			}
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
		}
		tpp_file_popifdef(file);
		tpp_file_popchunk(file);
		tpp_file_autopopfile_pop(file);
	}

	/* Allocate the perfectly-sized chunk that will describe the expanded macro's text */
	result_chunk = tpp_string_malloc(result_chunk_size);
	if tpp_unlikely(!result_chunk)
		goto err_nomem_macro_argbuf_rollback_arginfo_expinfo;

	/* Produce body-chunk-string for function-style macro expansion */
	{
		tpp_char *dst = tpp_string_str(result_chunk);
		tpp_char const *src = macro->tm_body_start;
		tpp_macro_opcode const *pc = macro->tm_data.tmd_func.tmf_expand;
		tpp_macro_opcode opcode;
next_op:
		opcode = *pc++;
		switch (opcode) {

		case TPP_MACRO_OPCODE_END:
			break;

		case TPP_MACRO_OPCODE_SKIP:
			src += *pc++;
			goto next_op;

		case TPP_MACRO_OPCODE_COPY: {
			tpp_size bytes = *pc++;
			tpp_memcpy(dst, src, bytes);
			src += bytes;
			dst += bytes;
			goto next_op;
		}

		case TPP_MACRO_OPCODE_INS_EXP: {
			tpp_size argi = *pc++;
			tpp_macro_expinfo const *expinfo = &invoke_expinfo[argi];
			tpp_assert(argi < macro_argc);
			tpp_assert(macro->tm_data.tmd_func.tmf_argv[argi].tma_ins_exp != 0);
			tpp_memcpy(dst,
			           tpp_macro_expinfo_getdata(expinfo),
			           tpp_macro_expinfo_getsize(expinfo));
			dst += tpp_macro_expinfo_getsize(expinfo);
			src += *pc++;
			goto next_op;
		}

#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
		case TPP_MACRO_OPCODE_INS_STR: {
			/* +2  Insert argument[ARG[0]] ("-escaped) and advance macro body template reader by ARG[1] bytes */
			tpp_size argi = *pc++;
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[argi];
			tpp_size raw_size;
			tpp_assert(argi < macro_argc);
			tpp_assert(macro->tm_data.tmd_func.tmf_argv[argi].tma_ins_str != 0);
			*dst++ = '"';
			raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
			tpp_token_encodestring(&tpp_buffer_printer, &dst, arginfo->tlai_start, raw_size);
			*dst++ = '"';
			src += *pc++;
			goto next_op;
		}
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */

#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT
		case TPP_MACRO_OPCODE_INS_CHR: {
			/* +2  Insert argument[ARG[0]] ("-escaped) and advance macro body template reader by ARG[1] bytes */
			tpp_size argi = *pc++;
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[argi];
			tpp_size raw_size;
			tpp_assert(argi < macro_argc);
			tpp_assert(macro->tm_data.tmd_func.tmf_argv[argi].tma_ins_str != 0);
			*dst++ = '\'';
			raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
			tpp_token_encodestring(&tpp_buffer_printer, &dst, arginfo->tlai_start, raw_size);
			*dst++ = '\'';
			src += *pc++;
			goto next_op;
		}
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */

#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
		case TPP_MACRO_OPCODE_INS: {
			tpp_size argi = *pc++;
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[argi];
			tpp_size raw_size;
			tpp_assert(argi < macro_argc);
			tpp_assert(macro->tm_data.tmd_func.tmf_argv[argi].tma_ins != 0);
			raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
			tpp_memcpy(dst, arginfo->tlai_start, raw_size);
			dst += raw_size;
			src += *pc++;
			goto next_op;
		}
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */

#if TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
		case TPP_MACRO_OPCODE_VA_COMMA: {
			src += *pc++;
			if (argc >= macro_argc)
				*dst++ = ',';
			goto next_op;
		}
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */

#if TPP_HAVE_VA_OPT_IN_MACROS
		case TPP_MACRO_OPCODE_VA_OPT: {
			tpp_size bytes;
			src += *pc++;
			bytes = *pc++;
			if (argc >= macro_argc) {
				tpp_memcpy(dst, src, bytes);
				dst += bytes;
			}
			src += bytes;
			src += *pc++;
			goto next_op;
		}
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */

#if TPP_HAVE_VA_NARGS_IN_MACROS
		case TPP_MACRO_OPCODE_VA_NARGS: {
			src += *pc++;

			/* NOTE: If you leave "va_nargs_len" uninitialized, this right here would
			 *       be marked as a use of an uninitialized variable by the compiler.
			 * That is technically true, but:
			 * - The "TPP_MACRO_OPCODE_VA_NARGS" opcode may only be emitted if
			 *   the macro compiler sets "tm_data.tmd_func.tmf_n_vanargs != 0"
			 * - When "tm_data.tmd_func.tmf_n_vanargs != 0", then "va_nargs_len"
			 *   gets initialized to its correct value above! */
			tpp_memcpy(dst, va_nargs, va_nargs_len);
			dst += va_nargs_len;
			goto next_op;
		}
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */

		default: tpp_unreachable();
		}
		tpp_assert((dst == tpp_string_end(result_chunk)) &&
		           "This failing means that argument specs and macro opcodes have diverged ("
		           "this shouldn't happen and indicates a bug in `tpp_macro_builder_compile()')");
		tpp_assert((src <= macro->tm_body_end) &&
		           "Similar to 'dst'-assertion, but source-body pointer went beyond end of "
		           "macro definition body (bug in `tpp_macro_builder_compile()')");
	}

	/* Cleanup temporary expansion buffers. */
	for (i = 0; i < macro_argc; ++i) {
		tpp_lexer_arginfo *arginfo = &invoke_arginfo[i];
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		if (arg->tma_ins_exp) {
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			tpp_macro_expinfo_fini(expand, arginfo);
		}
		tpp_lexer_arginfo_fini(arginfo);
	}

	/* Release argument buffer back to macro */
	tpp_macro_release_argbuf(macro, argbuf);

#if TPP_HAVE_MACRO_RECURSION
	if (macro->tm_expansions > 0) {
		/* Check if an identical body-chunk-string already exists
		 * somewhere on the tf_tprev-based #include-stack.
		 * -> If a duplicate expansion was encountered, rollback & don't expand */
		tpp_file *iter = file;
		do {
			if (iter->tf_kind == TPP_FILE_KIND_MACRO &&
			    iter->tf_data.td_macro.tfm_macro == macro) {
				tpp_string const *existing_chunk = iter->tf_chunk;
				if (tpp_string_equals(existing_chunk, result_chunk)) {
					/* Duplicate chunk!!! -> Mustn't expand (else: would result in infinite loop) */
					tpp_string_destroy(result_chunk);
					tpp_macro_decref(macro);
					goto done_rollback;
				}
			}
		} while ((iter = iter->tf_tprev) != NULL);
	}
#endif /* TPP_HAVE_MACRO_RECURSION */

	/* Set-up "result_chunk" such that it will read from "body-chunk-string" */
	prev_file = tpp_file_alloc();
	tpp_lexer_alltokens_break(self);
	if tpp_unlikely(!prev_file) {
		tpp_lexer_manualpopfile_break_rollback(self);
		tpp_string_decref(result_chunk);
		tok = TPP_TOK_ENOMEM;
		goto err_tok_macro;
	}
	tpp_lexer_manualpopfile_break_commit(self);
	*prev_file = *file;
	prev_file->tf_tpos = macro_call_start_pos;

	file->tf_pos   = tpp_string_str(result_chunk);
	file->tf_chunk = result_chunk; /* Inherit reference */
	file->tf_end   = tpp_string_end(result_chunk);
	(void)0 _tpp_file_init_common(file);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	file->tf_kind  = TPP_FILE_KIND_MACRO;
#if TPP_HAVE_UNICODE
	file->tf_enc = macro->tm_body_enc;
#endif /* TPP_HAVE_UNICODE */
	file->tf_data.td_macro.tfm_macro = macro; /* Inherit the reference created at the very start */
	++macro->tm_expansions;
	return TPP_TOK_EOF;
#if TPP_HAVE_MACRO_RECURSION
done_rollback:
	tpp_lexer_alltokens_break(self);
	file->tf_pos = rollback_pos;
	tpp_lexer_manualpopfile_break_rollback(self);

	/* NOTE: After this rollback, any preprocessor directives already
	 *       parsed will be parsed again. - While this isn't necessarily
	 *       intentional, we can only get here when "-fmacro-recursion"
	 *       is enabled, which is a TPP-specific extension, so this does
	 *       not violate any standard. */
	token->tt_end = token->tt_start + macro_keyword_len;
	token->tt_kwd = macro_keyword;
	token->tt_id  = macro_keyword->tk_id;
	return macro_keyword->tk_id;
#endif /* TPP_HAVE_MACRO_RECURSION */

err_nomem_macro_argbuf_rollback_arginfo_expinfo:
	tok = TPP_TOK_ENOMEM;
/*err_tok_macro_argbuf_rollback_arginfo_expinfo:*/
	i = macro_argc;
err_tok_macro_argbuf_rollback_arginfo_expinfo_i:
	while (i--) {
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		if (arg->tma_ins_exp) {
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
			tpp_macro_expinfo_fini(expand, arginfo);
		}
	}
err_tok_macro_argbuf_rollback_arginfo:
	while (macro_argc--)
		tpp_lexer_arginfo_fini(&invoke_arginfo[macro_argc]);
err_tok_macro_argbuf_rollback:
	tpp_lexer_alltokens_pop(self);
	tpp_lexer_manualpopfile_end_rollback(self);
/*err_tok_macro_argbuf:*/
	tpp_macro_release_argbuf(macro, argbuf);
err_tok_macro:
	tpp_macro_decref(macro);
err_tok:
	/* Reset token so another attempt to yield will get us here again */
	token->tt_end = token->tt_start;
	return tok;
}

/* Perform the expansion of a user-defined "macro", with the lexer's
 * current token set to point at the macro's identifier (meaning that
 * you have to seek ahead in order to find the opening '(' token in
 * case of a function-style macro).
 *
 * @return: tpp_lexer_gettoken(self)->tt_id : Function-style macro cannot be expanded
 * @return: TPP_TOK_EOF: Success -- caller should yield again to load the
 *                                  first macro's first expansion token.
 * @return: TPP_TOK_ENOMEM: Out of memory */
TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_expand_macro(tpp_lexer *tpp_restrict self,
                       tpp_macro *tpp_restrict macro) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file;
	if (TPP_MACRO_KIND_ISFUNC(macro->tm_kind))
		return tpp_lexer_expand_macro_function(self, macro);

	/* Simple case: keyword-style macro */

	/* Allocate a new file that will be used to backup the current input file. */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem;
	*prev_file = *file;
	file->tf_pos   = macro->tm_body_start;
	file->tf_chunk = macro->tm_body_chunk;
	file->tf_end   = macro->tm_body_end;
	if (file->tf_chunk)
		tpp_string_incref(file->tf_chunk);

	(void)0 _tpp_file_init_common(file);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	file->tf_kind  = TPP_FILE_KIND_MACRO;
#if TPP_HAVE_UNICODE
	file->tf_enc = macro->tm_body_enc;
#endif /* TPP_HAVE_UNICODE */
	file->tf_data.td_macro.tfm_macro = macro;
	tpp_macro_incref(macro);
	++macro->tm_expansions;

	(void)macro;
	return TPP_TOK_EOF;
err_nomem:
	return TPP_TOK_ENOMEM;
}
#endif /* TPP_HAVE_CPP_MACROS */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-yield.c                                            */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS

/* Perform the expansion of a user-defined "macro", with the lexer's
 * current token set to point at the macro's identifier (meaning that
 * you have to seek ahead in order to find the opening '(' token in
 * case of a function-style macro).
 *
 * @return: tpp_lexer_gettoken(self)->tt_id : Function-style macro cannot be expanded
 * @return: TPP_TOK_EOF: Success -- caller should yield again to load the
 *                                  first macro's first expansion token.
 * @return: TPP_TOK_ENOMEM: Out of memory */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_expand_macro(tpp_lexer *tpp_restrict self,
                       tpp_macro *tpp_restrict macro);

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_push_textfile_inherited(tpp_lexer *tpp_restrict self,
                                  tpp_char const *text, tpp_size textsize,
                                  /*0..1,inherit(always)*/ TPP_REF tpp_string *chunk) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem;
	*prev_file = *file;
	file->tf_pos   = text;
	file->tf_chunk = chunk; /* Inherit reference */
	file->tf_end   = text + textsize;
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	file->tf_kind  = TPP_FILE_KIND_TEXT;
	(void)0 _tpp_file_init_enc_ex(file, TPP_FILE_ENCODING_FORCE_UTF8);
	(void)0 _tpp_file_init_common(file);
	(void)0 _tpp_file_init_text_user_filename(file);
	file->tf_data.td_text.tft_name     = NULL;
	tpp_lcinfo_init(file->tf_data.td_text.tft_start_lc, -1, -1);
	return TPP_TOK_EOF;
err_nomem:
	if (chunk)
		tpp_string_decref(chunk);
	return TPP_TOK_ENOMEM;
}

/* Support for feature-test-style macros */
#undef TPP_HAVE_KEYWORD_TEST_MACROS
#define TPP_HAVE_KEYWORD_TEST_MACROS \
	(TPP_HAVE_MACRO___TPP_UNIQUE ||  \
	 TPP_HAVE_MACRO___TPP_COUNTER)

/* Support for feature-test-style macros */
#undef TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
#define TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS     \
	(TPP_HAVE_CLANG_MACRO___has_attribute ||          \
	 TPP_HAVE_CLANG_MACRO___has_builtin ||            \
	 TPP_HAVE_CLANG_MACRO___has_cpp_attribute ||      \
	 TPP_HAVE_CLANG_MACRO___has_declspec_attribute || \
	 TPP_HAVE_CLANG_MACRO___has_extension ||          \
	 TPP_HAVE_CLANG_MACRO___has_feature ||            \
	 TPP_HAVE_CLANG_MACRO___has_c_attribute ||        \
	 TPP_HAVE_MACRO___is_identifier ||                \
	 TPP_HAVE_MACRO___is_deprecated ||                \
	 TPP_HAVE_MACRO___is_poisoned)

#undef TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
#define TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS \
	(TPP_HAVE_MACRO___has_extension ||           \
	 TPP_HAVE_MACRO___has_known_extension ||     \
	 TPP_HAVE_MACRO___has_warning ||             \
	 TPP_HAVE_MACRO___has_known_warning)

#undef TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO
#define TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO \
	(TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS ||    \
	 TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS ||     \
	 TPP_HAVE_KEYWORD_TEST_MACROS)

#undef TPP_HAVE_LEXER_PUSH_TEXTFILE_INT
#define TPP_HAVE_LEXER_PUSH_TEXTFILE_INT \
	(TPP_HAVE_MACRO___COUNTER__ ||       \
	 TPP_HAVE_MACRO___LINE__ ||          \
	 TPP_HAVE_MACRO___COLUMN__ ||        \
	 TPP_HAVE_MACRO___INCLUDE_LEVEL__ || \
	 TPP_HAVE_MACRO___INCLUDE_DEPTH__ || \
	 TPP_HAVE_NUMERIC_DATE_MACROS ||     \
	 TPP_HAVE_NUMERIC_TIME_MACROS)

#undef TPP_HAVE_LEXER_PUSH_TEXTFILE
#define TPP_HAVE_LEXER_PUSH_TEXTFILE                 \
	(TPP_HAVE_LEXER_PUSH_TEXTFILE_INT ||             \
	 TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO || \
	 TPP_HAVE_MACRO___TIME__ ||                      \
	 TPP_HAVE_MACRO___DATE__ ||                      \
	 TPP_HAVE_MACRO___TIMESTAMP__)

#if TPP_HAVE_LEXER_PUSH_TEXTFILE
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_push_textfile(tpp_lexer *tpp_restrict self,
                        tpp_char const *text,
                        tpp_size textsize) {
	TPP_REF tpp_string *chunk;
	chunk = tpp_string_malloc(textsize);
	if tpp_unlikely(!chunk)
		return TPP_TOK_ENOMEM;
	tpp_memcpy(tpp_string_str(chunk), text, textsize);
	return tpp_lexer_push_textfile_inherited(self, tpp_string_str(chunk),
	                                         textsize, chunk);
}
#endif /* TPP_HAVE_LEXER_PUSH_TEXTFILE */

#if TPP_HAVE_LEXER_PUSH_TEXTFILE_INT
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_push_textfile_int(tpp_lexer *tpp_restrict self,
                            tpp_intmax value) {
	char buf[TPP_ITOA_MAXLEN];
	char const *p = tpp_itoa(buf, value);
	return tpp_lexer_push_textfile(self, (tpp_char const *)p,
	                               (tpp_size)(buf + tpp_lengthof(buf) - p));
}
#endif /* TPP_HAVE_LEXER_PUSH_TEXTFILE_INT */


#if TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO
#undef TPP_FEATURE_FLAG_EXPANSION_MAXLEN
#if TPP_HAVE_MACRO___TPP_UNIQUE || TPP_HAVE_MACRO___TPP_COUNTER
#define TPP_FEATURE_FLAG_EXPANSION_MAXLEN TPP_ITOA_MAXLEN
#else /* ... */
#define TPP_FEATURE_FLAG_EXPANSION_MAXLEN 1
#endif /* !... */

#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
struct tpp_lexer_handle_string_feature_test_data {
	tpp_lexer   *tlhsftd_lexer; /* [1..1] Lexer */
	tpp_token_id tlhsftd_mode;  /* Feature-test mode */
	tpp_char     tlhsftd_expansion[TPP_FEATURE_FLAG_EXPANSION_MAXLEN]; /* Desired expansion */
};

static tpp_errno TPPCALL
tpp_lexer_handle_string_feature_test_cb(void *arg, tpp_string *chunk,
                                        tpp_char const *str, tpp_size length) {
	struct tpp_lexer_handle_string_feature_test_data *data;
	(void)chunk;
	data = (struct tpp_lexer_handle_string_feature_test_data *)arg;
	switch (data->tlhsftd_mode) {

#if TPP_HAVE_MACRO___has_extension || TPP_HAVE_MACRO___has_known_extension
#if TPP_HAVE_MACRO___has_extension
	case TPP_KWD___has_extension:
#endif /* TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_MACRO___has_known_extension
	case TPP_KWD___has_known_extension:
#endif /* TPP_HAVE_MACRO___has_known_extension */
	{
		tpp_extension_id extension_id;
		if (length >= 2 && str[0] == '-' && str[1] == 'f') {
			str += 2;
			length -= 2;
		}
		extension_id = tpp_extension_byname_ex((char const *)str, length);
		if ((unsigned int)extension_id < (unsigned int)TPP_EXT_COUNT) {
#if TPP_HAVE_MACRO___has_extension
			if (data->tlhsftd_mode == TPP_KWD___has_extension) {
				if (tpp_extensions_getid(&data->tlhsftd_lexer->tl_exts, extension_id))
					data->tlhsftd_expansion[0] = '1';
			} else
#endif /* TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_MACRO___has_known_extension
			if (data->tlhsftd_mode == TPP_KWD___has_known_extension) {
				data->tlhsftd_expansion[0] = '1'; /* We only even get here if the extension is known! */
			} else
#endif /* TPP_HAVE_MACRO___has_known_extension */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_MACRO___has_extension || TPP_HAVE_MACRO___has_known_extension */

#if TPP_HAVE_MACRO___has_warning || TPP_HAVE_MACRO___has_known_warning
#if TPP_HAVE_MACRO___has_warning
	case TPP_KWD___has_warning:
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
	case TPP_KWD___has_known_warning:
#endif /* TPP_HAVE_MACRO___has_known_warning */
	{
		tpp_warning_group_id warning_group_id;
		if (length >= 2 && str[0] == '-' && str[1] == 'W') {
			str += 2;
			length -= 2;
		}
		warning_group_id = tpp_warning_group_byname_ex((char const *)str, length);
		if ((unsigned int)warning_group_id < (unsigned int)TPP_WG_COUNT) {
#if TPP_HAVE_MACRO___has_warning
			if (data->tlhsftd_mode == TPP_KWD___has_warning) {
				tpp_warning_context_id ctx_id = tpp_warning_context_id_ofgroup(warning_group_id);
				tpp_warning_state state = tpp_warnings_getctx(&data->tlhsftd_lexer->tl_warn, ctx_id);
				if (tpp_warning_state_willemit(state))
					data->tlhsftd_expansion[0] = '1';
			} else
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
			if (data->tlhsftd_mode == TPP_KWD___has_known_warning) {
				data->tlhsftd_expansion[0] = '1'; /* We only even get here if the warning is known! */
			} else
#endif /* TPP_HAVE_MACRO___has_known_warning */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_MACRO___has_warning || TPP_HAVE_MACRO___has_known_warning */

	default: break;
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_feature_test_macro(tpp_lexer *tpp_restrict self, tpp_token_id mode) {
	tpp_token_id tok;
	unsigned int recursion;
#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
	struct tpp_lexer_handle_string_feature_test_data data;
#define tpp_feature_test_macro_expansion data.tlhsftd_expansion
#else /* TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
	tpp_char expansion[TPP_FEATURE_FLAG_EXPANSION_MAXLEN];
#define tpp_feature_test_macro_expansion expansion
#endif /* !TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
#if TPP_FEATURE_FLAG_EXPANSION_MAXLEN > 1
	tpp_size tpp_feature_test_macro_expansion_len = 1;
#else /* TPP_FEATURE_FLAG_EXPANSION_MAXLEN > 1 */
#define tpp_feature_test_macro_expansion_len 1
#endif /* TPP_FEATURE_FLAG_EXPANSION_MAXLEN <= 1 */
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}

	do {
		tok = tpp_lexer_yieldpp_blocking(self);
		if tpp_unlikely(TPP_TOK_ISERR(tok))
			return tok;
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));

	/* Default to expanding to "0" */
	tpp_feature_test_macro_expansion[0] = '0';

	/* Deal with special case of "__has_extension()" (which is overloaded for TPP) */
#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
	if (TPP_TOK_ISSTRING(tok)) {
		tpp_errno error;
#if TPP_HAVE_CLANG_MACRO___has_extension
		if (mode == TPP_KWD___has_extension &&
		    !tpp_lexer_has(self, MACRO___has_extension))
			goto seek_end_of_macro;
#define WANT_seek_end_of_macro
#endif /* TPP_HAVE_CLANG_MACRO___has_extension */
		data.tlhsftd_lexer = self;
		data.tlhsftd_mode  = mode;

		/* Parse the string that the user entered. */
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_string_feature_test_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		tok = tpp_lexer_gettok(self);
	} else
#endif /* TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
	{
#if TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS || TPP_HAVE_KEYWORD_TEST_MACROS
		/* Deal with keyword-style feature tests... */
		if (TPP_TOK_ISKEYWORD(tok)) {
			tpp_keyword const *feature_keyword;
			tpp_keyword_flags mask, flags;

			/* Load keyword. */
			feature_keyword = tpp_lexer_gettoken(self)->tt_kwd;
			switch (mode) {

#if TPP_HAVE_MACRO___is_identifier
			case TPP_KWD___is_identifier:
				/* Something is considered to be an "identifier" if it's not a builtin keyword. */
				if (!TPP_TOK_ISBUILTINKEYWORD(feature_keyword->tk_id))
					tpp_feature_test_macro_expansion[0] = '1';
				goto after_expansion_mode_assignment;
#define WANT_after_expansion_mode_assignment
#endif /* TPP_HAVE_MACRO___is_identifier */

#if TPP_HAVE_MACRO___TPP_UNIQUE || TPP_HAVE_MACRO___TPP_COUNTER
			{
				tpp_intmax expansion_value;
				char *expansion_dst;
#if TPP_HAVE_MACRO___TPP_UNIQUE
				if(0) {
			case TPP_KWD___TPP_UNIQUE:
					expansion_value = (tpp_intmax)feature_keyword->tk_id;
				}
#endif /* TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_COUNTER
				if(0) {
					tpp_keyword_misc *misc;
					tpp_keyword *rw_keyword;
			case TPP_KWD___TPP_COUNTER:
					rw_keyword = tpp_keywords_copybuiltin(&self->tl_kwds, feature_keyword);
					if tpp_unlikely(!rw_keyword)
						return TPP_TOK_ENOMEM;
					misc = tpp_keyword_requiremisc(rw_keyword);
					if tpp_unlikely(!misc)
						return TPP_TOK_ENOMEM;
					expansion_value = misc->tkm_builtin_counter++;
				}
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
				expansion_dst = tpp_itoa((char *)tpp_feature_test_macro_expansion, expansion_value);
				tpp_feature_test_macro_expansion_len = (tpp_size)((char *)tpp_feature_test_macro_expansion +
					                                              tpp_lengthof(tpp_feature_test_macro_expansion) -
					                                              expansion_dst);
				tpp_memmovedown(tpp_feature_test_macro_expansion, expansion_dst, tpp_feature_test_macro_expansion_len);
				goto after_expansion_mode_assignment;
#define WANT_after_expansion_mode_assignment
			}	break;
#endif /* !TPP_HAVE_MACRO___TPP_UNIQUE || TPP_HAVE_MACRO___TPP_COUNTER */

#if TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
			default: break;
#else /* TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS */
			default: tpp_unreachable();
#endif /* !TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS */
			}

#if TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
			/* Load keyword flags. */
			flags = tpp_lexer_getkeywordflags(self, feature_keyword);

			/* Also include flags after stripping leading/trailing _-s */
			if (feature_keyword->tk_kwd[0] == '_' ||
			    feature_keyword->tk_kwd[feature_keyword->tk_len - 1] == '_') {
				tpp_char const *strip = feature_keyword->tk_kwd;
				tpp_size strip_len = feature_keyword->tk_len;
				tpp_hash strip_hash;
				while (*strip == '_') {
					++strip;
					--strip_len;
				}
				while (strip[strip_len - 1] == '_')
					--strip_len;
				strip_hash = tpp_hashof(strip, strip_len);
				feature_keyword = tpp_keywords_getkeyword(&self->tl_kwds, strip,
				                                          strip_len, strip_hash);
				flags |= tpp_lexer_getkeywordflags(self, feature_keyword);
			}

			/* Determine expansion based on "mode" and "flags" */
			switch (mode) {
#if TPP_HAVE_CLANG_MACRO___has_attribute
			case TPP_KWD___has_attribute:
				mask = TPP_KEYWORD_FLAG_HAS_ATTRIBUTE;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
			case TPP_KWD___has_builtin:
				mask = TPP_KEYWORD_FLAG_HAS_BUILTIN;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
			case TPP_KWD___has_cpp_attribute:
				mask = TPP_KEYWORD_FLAG_HAS_CPP_ATTRIBUTE;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
			case TPP_KWD___has_declspec_attribute:
				mask = TPP_KEYWORD_FLAG_HAS_DECLSPEC_ATTRIBUTE;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension
			case TPP_KWD___has_extension:
				mask = TPP_KEYWORD_FLAG_HAS_EXTENSION;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
			case TPP_KWD___has_feature:
				mask = TPP_KEYWORD_FLAG_HAS_FEATURE;
#if TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES
				if (tpp_lexer_has(self, CLANG_EXTENSIONS_ARE_FEATURES))
					mask |= TPP_KEYWORD_FLAG_HAS_EXTENSION;
#endif /* TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES */
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
			case TPP_KWD___has_c_attribute:
				mask = TPP_KEYWORD_FLAG_HAS_C_ATTRIBUTE;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_MACRO___is_deprecated
			case TPP_KWD___is_deprecated:
				mask = TPP_KEYWORD_FLAG_IS_DEPRECATED;
				break;
#endif /* TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_MACRO___is_poisoned
			case TPP_KWD___is_poisoned:
				mask = TPP_KEYWORD_FLAG_IS_POISONED;
				break;
#endif /* TPP_HAVE_MACRO___is_poisoned */
			default: tpp_unreachable();
			}
			tpp_feature_test_macro_expansion[0] = (flags & mask) != 0 ? '1' : '0';
#endif /* TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS */

#ifdef WANT_after_expansion_mode_assignment
#undef WANT_after_expansion_mode_assignment
after_expansion_mode_assignment:
#endif /* WANT_after_expansion_mode_assignment */
			do {
				tok = tpp_lexer_yieldpp_blocking(self);
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
			if tpp_unlikely(TPP_TOK_ISERR(tok))
				return tok;
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS || TPP_HAVE_KEYWORD_TEST_MACROS */
	}

#ifdef WANT_seek_end_of_macro
#undef WANT_seek_end_of_macro
seek_end_of_macro:
#endif /* WANT_seek_end_of_macro */
	recursion = 0;
	for (;;) {
		if (tok == '(') {
			++recursion;
		} else if (tok == ')') {
			if (recursion == 0)
				break;
			--recursion;
		}
		tpp_feature_test_macro_expansion[0] = '0';
#if TPP_FEATURE_FLAG_EXPANSION_MAXLEN > 1
		tpp_feature_test_macro_expansion_len = 1;
#endif /* TPP_FEATURE_FLAG_EXPANSION_MAXLEN > 1 */
		if (tok == TPP_TOK_EOF)
			break;
		tok = tpp_lexer_yieldpp_blocking(self);
		if tpp_unlikely(TPP_TOK_ISERR(tok))
			return tok;
	}
	return tpp_lexer_push_textfile(self, tpp_feature_test_macro_expansion,
	                               tpp_feature_test_macro_expansion_len);
#undef tpp_feature_test_macro_expansion_len
#undef tpp_feature_test_macro_expansion
}
#endif /* TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO */


#if TPP_HAVE_PRAGMA
/* Process a #pragma directive, start at the first token that comes after
 * the leading "#pragma" (i.e.: the first token of the actual directive
 * itself)
 *
 * @return: TPP_EOK:    Success (but there may still be garbage after
 *                      the directive that hasn't been parsed, yet).
 * @return: TPP_ENOENT: Unknown pragma (soft-error; caller should not emit
 *                      "TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE")
 * @return: TPP_E*:     Error */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);
#endif /* TPP_HAVE_PRAGMA */


#if TPP_HAVE_MACRO__Pragma || TPP_HAVE_MACRO___pragma
static TPP_WUNUSED tpp_errno TPPCALL 
tpp_lexer_process_pragma_until_eof(tpp_lexer *tpp_restrict self) {
	tpp_errno result = tpp_lexer_process_pragma(self);
	if (result == TPP_ENOENT) {
		result = TPP_EOK; /* Don't warn about trailing tokens in this case */
	} else
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
	if (!TPP_ISERR(result)) {
		/* Warn about trailing tokens */
		tpp_token_id tok = tpp_lexer_gettoken(self)->tt_id;
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
		if (tok != TPP_TOK_EOF)
			result = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE);
	} else
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE */
	{
	}
	return result;
}
#endif /* TPP_HAVE_MACRO__Pragma || TPP_HAVE_MACRO___pragma */

#if TPP_HAVE_MACRO__Pragma
static TPP_WUNUSED tpp_errno TPPCALL 
tpp_lexer_yield_handle__Pragma_string(void *arg, tpp_string *chunk,
                                      tpp_char const *str, tpp_size length) {
	tpp_token_id tok;
	tpp_errno result;
	tpp_lexer *self = (tpp_lexer *)arg;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_assert(file->tf_prev == NULL);
	tpp_file_autopopfile_pushoff(file);
	tpp_file_pushchunk(file);

	/* (re-)configure "file" to point at "str" (and setup LC info as close as possible)
	 * Really though: LC info will only be perfectly precise when "str" is actually still
	 * part of the original buffer. Otherwise, it will be off. */
	if (file->tf_chunk != chunk) {
		tpp_lcinfo lc = tpp_file_lcinfo(file, file->tf_pos);
		file->tf_chunk = chunk;
		file->tf_kind = TPP_FILE_KIND_TEXT;
		file->tf_data.td_text.tft_start_lc = lc;
#if TPP_HAVE_FILE_LC_CACHE
		file->tf_lcpos = NULL;
#endif /* TPP_HAVE_FILE_LC_CACHE */
	}
	file->tf_pos = str;
	file->tf_end = str + length;

	/* Yield decoded _Pragma-string as a token. */
	do {
		tok = tpp_lexer_yieldraw(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok)) {
		result = TPP_TOK_ASERR(tok);
	} else {
		/* Process _Pragma string as a pragma */
		result = tpp_lexer_process_pragma_until_eof(self);
	}

	tpp_file_popchunk(file);
	tpp_file_autopopfile_pop(file);
	return result;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle__Pragma(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
	tpp_errno error;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	do {
		tok = tpp_lexer_yieldpp_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if tpp_unlikely(TPP_TOK_ISERR(tok))
		return tok;

	if (!TPP_TOK_ISSTRING(tok)) {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	} else {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_yield_handle__Pragma_string,
		                                 self, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
	}
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return tok;
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		return tok;
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_MACRO__Pragma */


#if TPP_HAVE_MACRO___pragma
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___pragma(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_lexer_arginfo argv[1];
	tpp_token_id tok;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	tok = tpp_lexer_seekpp_rparen_exact(self, argv, 1, "__pragma",
	                                    TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL);
	if (TPP_TOK_ISERR(tok))
		return tok;
	tpp_file_autopopfile_pushoff(file);
	tpp_file_pushchunk(file);

	/* Setup file to (re-)parse the __pragma content */
	tpp_file_setchunk_fromarg(file, &argv[0]);
	do {
		tok = tpp_lexer_yieldpp_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (!TPP_TOK_ISERR(tok)) {
		tpp_errno error;
		error = tpp_lexer_process_pragma_until_eof(self);
		tok = TPP_TOK_OFERR_OR_EOF(error);
	}
	tpp_file_popchunk(file);
	tpp_file_autopopfile_pop(file);
	tpp_lexer_arginfo_fini(&argv[0]);
	return tok;
}
#endif /* TPP_HAVE_MACRO___pragma */


#if TPP_HAVE_MACRO___LINE__ || TPP_HAVE_MACRO___COLUMN__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_lcinfo(tpp_lexer *tpp_restrict self, tpp_token_id what) {
	tpp_file *const lcfile = tpp_file_getlcfile(tpp_lexer_getfile(self));
	tpp_lcinfo info;
	tpp_intmax value;

	/* HINT: Meaning of "tf_tpos" / "tf_pos" here:
	 * >> #define assert(x) (... || (_assert(x, __FILE__, __LINE__, __COLUMN__)))
	 * >> ...
	 * >> 
	 * >> if (x)
	 * >>     assert(y);
	 *        ^        ^
	 *        tf_tpos  tf_pos
	 *
	 * iow: "tf_tpos" position for tracebacks (points at what "caused" a macro/file push)
	 *      "tf_pos" position of next byte to-be parsed once lexer returns to this file
	 *
	 * For the sake of being pretty, we use "tf_tpos" since that's the location of the
	 * name of the macro that's currently being expanded. */
	info = tpp_file_lcinfo(lcfile, lcfile->tf_tpos);
	switch (what) {
#if TPP_HAVE_MACRO___LINE__
	case TPP_KWD___LINE__:
		value = tpp_lcinfo_getline(info);
		break;
#endif /* TPP_HAVE_MACRO___LINE__ */
#if TPP_HAVE_MACRO___COLUMN__
	case TPP_KWD___COLUMN__:
		value = tpp_lcinfo_getcol(info);
		break;
#endif /* TPP_HAVE_MACRO___COLUMN__ */
	default: tpp_unreachable();
	}
	++value;
	return tpp_lexer_push_textfile_int(self, value);
}
#endif /* ... */


#if TPP_HAVE_MACRO___COUNTER__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___COUNTER__(tpp_lexer *tpp_restrict self) {
	return tpp_lexer_push_textfile_int(self, self->tl_builtin_counter++);
}
#endif /* ... */


#undef TPP_HAVE_TIME_MACROS
#define TPP_HAVE_TIME_MACROS         \
	(TPP_HAVE_MACRO___TIME__ ||      \
	 TPP_HAVE_MACRO___DATE__ ||      \
	 TPP_HAVE_MACRO___TIMESTAMP__ || \
	 TPP_HAVE_NUMERIC_DATE_MACROS || \
	 TPP_HAVE_NUMERIC_TIME_MACROS)
#if TPP_HAVE_TIME_MACROS

#if TPP_HAVE_MACRO___DATE__ || TPP_HAVE_MACRO___TIMESTAMP__
static char const tpp_date_month_names[12][3] = {
	{ 'J', 'a', 'n' },
	{ 'F', 'e', 'b' },
	{ 'M', 'a', 'r' },
	{ 'A', 'p', 'r' },
	{ 'M', 'a', 'y' },
	{ 'J', 'u', 'n' },
	{ 'J', 'u', 'l' },
	{ 'A', 'u', 'g' },
	{ 'S', 'e', 'p' },
	{ 'O', 'c', 't' },
	{ 'N', 'o', 'v' },
	{ 'D', 'e', 'c' },
};
#endif /* TPP_HAVE_MACRO___DATE__ || TPP_HAVE_MACRO___TIMESTAMP__ */

#if TPP_HAVE_MACRO___TIMESTAMP__
static char const tpp_date_wday_names[7][3] = {
	{ 'S', 'u', 'n' },
	{ 'M', 'o', 'n' },
	{ 'T', 'u', 'e' },
	{ 'W', 'e', 'd' },
	{ 'T', 'h', 'u' },
	{ 'F', 'r', 'i' },
	{ 'S', 'a', 't' },
};
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */

#if TPP_HAVE_MACRO___TIME__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TIME__(tpp_lexer *tpp_restrict self, tpp_tm const *cur) {
	char buf[tpp_lengthof("\"00:00:00\"") - 1], *p = buf;
	*p++ = '"';
	*p++ = (char)('0' + (tpp_tm_gethour(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_gethour(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getmin(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getmin(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getsec(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getsec(cur) % 10));
	*p++ = '"';
	tpp_assert(p == (buf + tpp_lengthof(buf)));
	return tpp_lexer_push_textfile(self, (tpp_char const *)buf, (tpp_size)(p - buf));
}
#endif /* TPP_HAVE_MACRO___TIME__ */

#if TPP_HAVE_MACRO___DATE__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___DATE__(tpp_lexer *tpp_restrict self, tpp_tm const *cur) {
	char buf[tpp_lengthof("\"00:00:00\"") - 1], *p = buf;
	*p++ = '"';
	*p++ = (char)('0' + (tpp_tm_gethour(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_gethour(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getmin(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getmin(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getsec(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getsec(cur) % 10));
	*p++ = '"';
	tpp_assert(p == (buf + tpp_lengthof(buf)));
	return tpp_lexer_push_textfile(self, (tpp_char const *)buf, (tpp_size)(p - buf));
}
#endif /* TPP_HAVE_MACRO___DATE__ */

#if TPP_HAVE_MACRO___TIMESTAMP__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TIMESTAMP__(tpp_lexer *tpp_restrict self, tpp_tm const *cur) {
	char buf[tpp_lengthof("\"Mon Jan 12 00:00:00 2026\"") - 1], *p = buf;
	char const *mon = tpp_date_month_names[(tpp_tm_getmon(cur) - 1) % 12];
	char const *day = tpp_date_wday_names[tpp_tm_getwday(cur) % 7];
	*p++ = '"';
	*p++ = day[0];
	*p++ = day[1];
	*p++ = day[2];
	*p++ = ' ';
	*p++ = mon[0];
	*p++ = mon[1];
	*p++ = mon[2];
	*p++ = ' ';
	*p++ = (tpp_tm_getmday(cur) >= 10) ? (char)('0' + (tpp_tm_getmday(cur) / 10)) : ' ';
	*p++ = (char)('0' + (tpp_tm_getmday(cur) % 10));
	*p++ = ' ';
	*p++ = (char)('0' + (tpp_tm_gethour(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_gethour(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getmin(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getmin(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getsec(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getsec(cur) % 10));
	*p++ = ' ';
	*p++ = (char)('0' + ((tpp_tm_getyear(cur) / 1000) % 10));
	*p++ = (char)('0' + ((tpp_tm_getyear(cur) / 100) % 10));
	*p++ = (char)('0' + ((tpp_tm_getyear(cur) / 10) % 10));
	*p++ = (char)('0' + (tpp_tm_getyear(cur) % 10));
	*p++ = '"';
	tpp_assert(p == (buf + tpp_lengthof(buf)));
	return tpp_lexer_push_textfile(self, (tpp_char const *)buf, (tpp_size)(p - buf));
}
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */


static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_time_macro(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_tm cur;
	tpp_errno error;

	/* Initialize time if this is the first *time* (pun intended) we get here */
	if (tpp_time_isempty(tpp_lexer_gettimeptr(self))) {
		error = tpp_time_now(tpp_lexer_gettimeptr(self));
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#if TPP_HAVE_TPP_W_DATE_TIME
		error = tpp_lexer_warnf(self, TPP_W_DATE_TIME);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_DATE_TIME */
	}

	/* Load tm from time */
	error = tpp_tm_fromtime(&cur, tpp_lexer_gettimeptr(self));
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);

	/* Process time according to given "tok" */
	switch (tok) {
#if TPP_HAVE_MACRO___TIME__
	case TPP_KWD___TIME__:
		return tpp_lexer_yield_handle___TIME__(self, &cur);
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
	case TPP_KWD___DATE__:
		return tpp_lexer_yield_handle___DATE__(self, &cur);
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
	case TPP_KWD___TIMESTAMP__:
		return tpp_lexer_yield_handle___TIMESTAMP__(self, &cur);
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
#if TPP_HAVE_NUMERIC_DATE_MACROS
	case TPP_KWD___DATE_DAY__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getmday(&cur));
	case TPP_KWD___DATE_WDAY__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getwday(&cur));
	case TPP_KWD___DATE_YDAY__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getyday(&cur));
	case TPP_KWD___DATE_MONTH__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getmon(&cur));
	case TPP_KWD___DATE_YEAR__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getyear(&cur));
#endif /* !TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
	case TPP_KWD___TIME_SEC__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getsec(&cur));
	case TPP_KWD___TIME_MIN__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getmin(&cur));
	case TPP_KWD___TIME_HOUR__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_gethour(&cur));
#endif /* !TPP_HAVE_NUMERIC_TIME_MACROS */
	default: tpp_unreachable();
	}
	tpp_unreachable();
}
#endif /* TPP_HAVE_TIME_MACROS */


#if TPP_HAVE_MACRO___INCLUDE_LEVEL__ || TPP_HAVE_MACRO___INCLUDE_DEPTH__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___INCLUDE_LEVEL__(tpp_lexer *tpp_restrict self) {
	tpp_intmax value = -1;
	tpp_file const *iter = tpp_lexer_getfile(self);
	do {
		if (iter->tf_kind != TPP_FILE_KIND_MACRO)
			++value;
	} while ((iter = iter->tf_tprev) != NULL);
	return tpp_lexer_push_textfile_int(self, value);
}
#endif /* ... */


#if TPP_HAVE_MACRO___TPP_IDENTIFIER
struct tpp_lexer_handle_tpp_identifier_data {
	tpp_lexer         *tlhtid_lexer;   /* [1..1] Lexer */
	tpp_keyword const *tlhtid_keyword; /* [1..1] The identified keyword */
};

static TPP_WUNUSED tpp_errno TPPCALL 
tpp_lexer_handle_tpp_identifier_cb(void *arg, tpp_string *chunk,
                                   tpp_char const *str, tpp_size length) {
	tpp_keyword const *kwd;
	struct tpp_lexer_handle_tpp_identifier_data *data;
	(void)chunk;
	data = (struct tpp_lexer_handle_tpp_identifier_data *)arg;
	kwd = tpp_keywords_newkeyword(&data->tlhtid_lexer->tl_kwds,
	                              str, length, tpp_hashof(str, length));
	if tpp_unlikely(!kwd)
		return TPP_ENOMEM;
	data->tlhtid_keyword = kwd;
	return TPP_EOK;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_IDENTIFIER(tpp_lexer *tpp_restrict self) {
	struct tpp_lexer_handle_tpp_identifier_data data;
	tpp_char const *identifier_start;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_lexer_arginfo argv[1];
	tpp_token_id tok;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	tok = tpp_lexer_seekpp_rparen_exact(self, argv, 1, "__TPP_IDENTIFIER",
	                                    TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL);
	if (TPP_TOK_ISERR(tok))
		return tok;
	identifier_start = token->tt_start;
	tpp_file_autopopfile_pushoff(file);
	tpp_file_pushchunk(file);

	/* Setup file to (re-)parse the identifier string */
	tpp_file_setchunk_fromarg(file, &argv[0]);
	tok = tpp_lexer_yield(self);
	data.tlhtid_lexer   = self;
	data.tlhtid_keyword = NULL;
	if (!TPP_TOK_ISSTRING(tok)) {
		if (!TPP_TOK_ISERR(tok)) {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
			tok = TPP_TOK_OFERR_OR_EOF(error);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			tok = TPP_TOK_EOF;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
		}
	} else {
		tpp_errno error;
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_tpp_identifier_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		/* Warning if current token isn't EOF */
		if (!TPP_ISERR(error) && tpp_lexer_gettoken(self)->tt_id != TPP_TOK_EOF)
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#endif /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		tok = TPP_TOK_OFERR_OR_EOF(error);
	}
	tpp_file_popchunk(file);
	tpp_file_autopopfile_pop(file);
	if (!TPP_TOK_ISERR(tok)) {
		tpp_assert(data.tlhtid_keyword);
		/* Setup current token to refer to "data.tlhtid_keyword" */
		token->tt_id    = tok = data.tlhtid_keyword->tk_id;
		token->tt_kwd   = data.tlhtid_keyword;
		token->tt_start = identifier_start;
/*		token->tt_end   = ...;  * Already correct (points after the trailing ')'-token) */
	}
	return tok;
}
#endif /* TPP_HAVE_MACRO___TPP_IDENTIFIER */


#if TPP_HAVE_MACRO___TPP_EVAL
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_EVAL(tpp_lexer *tpp_restrict self) {
	tpp_expr_value eval_result;
	tpp_string_builder eval_repr_builder;
	TPP_REF tpp_string *eval_repr;
	tpp_ssize eval_repr_print_status;
	tpp_errno error;
	tpp_token_id tok;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}

	/* Setup file to (re-)parse the eval expression */
	error = tpp_lexer_parseexpr(self, &eval_result);
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return tok;
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Print representation of evaluation result */
	tpp_string_builder_init(&eval_repr_builder);
	eval_repr_print_status = tpp_expr_value_printrepr(&eval_result,
	                                                  &tpp_string_builder_print,
	                                                  &eval_repr_builder);
	tpp_expr_value_fini(&eval_result);
	if tpp_unlikely(eval_repr_print_status < 0) {
		tpp_string_builder_fini(&eval_repr_builder);
		error = (tpp_errno)(int)eval_repr_print_status;
		return TPP_TOK_OFERR(error);
	}

	/* Pack representation into a string... */
	eval_repr = tpp_string_builder_pack(&eval_repr_builder);

	/* ... and push that string as a text file. */
	return tpp_lexer_push_textfile_inherited(self, tpp_string_str(eval_repr),
	                                         tpp_string_len(eval_repr),
	                                         eval_repr);
}
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */



#if TPP_HAVE_CPP_BUILTIN_MACROS
/* Handle a builtin macro.
 * @return: TPP_TOK_EOF: Caller should yield again.
 * @return: * : The new expansion token after keywords were handled */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_builtin_macro(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	/* Deal with pre-defined macros. */
	switch (tok) {


/************************************************************************/
#if TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO
#if TPP_HAVE_CLANG_MACRO___has_attribute
	case TPP_KWD___has_attribute:
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
	case TPP_KWD___has_builtin:
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
	case TPP_KWD___has_cpp_attribute:
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
	case TPP_KWD___has_declspec_attribute:
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension
	case TPP_KWD___has_extension:
#endif /* TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
	case TPP_KWD___has_feature:
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
	case TPP_KWD___has_c_attribute:
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_MACRO___is_identifier
	case TPP_KWD___is_identifier:
#endif /* TPP_HAVE_MACRO___is_identifier */
#if TPP_HAVE_MACRO___is_deprecated
	case TPP_KWD___is_deprecated:
#endif /* TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_MACRO___is_poisoned
	case TPP_KWD___is_poisoned:
#endif /* TPP_HAVE_MACRO___is_poisoned */
#if TPP_HAVE_MACRO___has_known_extension
	case TPP_KWD___has_known_extension:
#endif /* TPP_HAVE_MACRO___has_known_extension */
#if TPP_HAVE_MACRO___has_warning
	case TPP_KWD___has_warning:
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
	case TPP_KWD___has_known_warning:
#endif /* TPP_HAVE_MACRO___has_known_warning */
#if TPP_HAVE_MACRO___TPP_UNIQUE
	case TPP_KWD___TPP_UNIQUE:
#endif /* !TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_COUNTER
	case TPP_KWD___TPP_COUNTER:
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */
		return tpp_lexer_handle_feature_test_macro(self, tok);
#endif /* TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_MACRO___has_include ||      \
     TPP_HAVE_MACRO___has_include_next || \
     TPP_HAVE_MACRO___has_embed)
#if TPP_HAVE_MACRO___has_include
	case TPP_KWD___has_include:
#endif /* TPP_HAVE_MACRO___has_include */
#if TPP_HAVE_MACRO___has_include_next
	case TPP_KWD___has_include_next:
#endif /* TPP_HAVE_MACRO___has_include_next */
#if TPP_HAVE_MACRO___has_embed
	case TPP_KWD___has_embed:
#endif /* TPP_HAVE_MACRO___has_embed */
	{
		/* TODO */
		/* TODO: __has_embed (https://en.cppreference.com/c/preprocessor/embed)
		 * >> __STDC_EMBED_NOT_FOUND__, __STDC_EMBED_FOUND__, __STDC_EMBED_EMPTY__ */
	}	break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__
#if TPP_HAVE_MACRO___FILE__
	case TPP_KWD___FILE__: /* TODO: Use tpp_file_getlcfile() */
#endif /* TPP_HAVE_MACRO___FILE__ */
#if TPP_HAVE_MACRO___BASE_FILE__
	case TPP_KWD___BASE_FILE__: /* TODO: Use tpp_file_getbasefile() */
#endif /* TPP_HAVE_MACRO___BASE_FILE__ */
	{
		/* TODO */
	}	break;
#endif /* TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___LINE__ || TPP_HAVE_MACRO___COLUMN__
#if TPP_HAVE_MACRO___LINE__
	case TPP_KWD___LINE__:
#endif /* TPP_HAVE_MACRO___LINE__ */
#if TPP_HAVE_MACRO___COLUMN__
	case TPP_KWD___COLUMN__:
#endif /* TPP_HAVE_MACRO___COLUMN__ */
		return tpp_lexer_yield_handle_lcinfo(self, tok);
#endif /* TPP_HAVE_MACRO___LINE__ || TPP_HAVE_MACRO___COLUMN__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TIME_MACROS
#if TPP_HAVE_MACRO___TIME__
	case TPP_KWD___TIME__:
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
	case TPP_KWD___DATE__:
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
	case TPP_KWD___TIMESTAMP__:
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
#if TPP_HAVE_NUMERIC_DATE_MACROS
	case TPP_KWD___DATE_DAY__:
	case TPP_KWD___DATE_WDAY__:
	case TPP_KWD___DATE_YDAY__:
	case TPP_KWD___DATE_MONTH__:
	case TPP_KWD___DATE_YEAR__:
#endif /* !TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
	case TPP_KWD___TIME_SEC__:
	case TPP_KWD___TIME_MIN__:
	case TPP_KWD___TIME_HOUR__:
#endif /* !TPP_HAVE_NUMERIC_TIME_MACROS */
		return tpp_lexer_yield_handle_time_macro(self, tok);
#endif /* TPP_HAVE_TIME_MACROS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__ || TPP_HAVE_MACRO___INCLUDE_DEPTH__
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__
	case TPP_KWD___INCLUDE_LEVEL__:
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ */
#if TPP_HAVE_MACRO___INCLUDE_DEPTH__
	case TPP_KWD___INCLUDE_DEPTH__:
#endif /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
		return tpp_lexer_yield_handle___INCLUDE_LEVEL__(self);
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___COUNTER__
	case TPP_KWD___COUNTER__:
		return tpp_lexer_yield_handle___COUNTER__(self);
#endif /* TPP_HAVE_MACRO___COUNTER__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO__Pragma
	case TPP_KWD__Pragma:
		return tpp_lexer_yield_handle__Pragma(self);
#endif /* TPP_HAVE_MACRO__Pragma */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___pragma
	case TPP_KWD___pragma:
		return tpp_lexer_yield_handle___pragma(self);
#endif /* TPP_HAVE_MACRO___pragma */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_IDENTIFIER
	case TPP_KWD___TPP_IDENTIFIER:
		return tpp_lexer_yield_handle___TPP_IDENTIFIER(self);
#endif /* !TPP_HAVE_MACRO___TPP_IDENTIFIER */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_EVAL
	case TPP_KWD___TPP_EVAL:
		return tpp_lexer_yield_handle___TPP_EVAL(self);
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_LOAD_FILE
	/* TODO: __TPP_LOAD_FILE */
	/* TODO: Parsing the #include-filename can be done by checking of the next token
	 *       starts with a '<' or '"' character. If it doesn't, but is instead a
	 *       keyword, then call "tpp_lexer_yield_handle_keyword(self, result)"
	 *       to (try to) expand said keyword. If that function then returns EOF
	 *       (the case where the caller is normally expected to yield the next
	 *       token), simply jump back to repeat the '<' / '"' check.
	 *       If the next token's first char isn't '<' / '"', that's "-Wsyntax"
	 *       If the next token doesn't start with '<' / '"', yield it so it is
	 *       possible to skip SPACE/COMMENT tokens before trying again.
	 * ^ An implementation like that is completely standard-confirming, since the
	 *   standard only mandates that macros be expanded in #include-filenames
	 *   before the '<' / '"' check is to-be repeated. */
#endif /* !TPP_HAVE_MACRO___TPP_LOAD_FILE */
#if TPP_HAVE_MACRO___TPP_RANDOM
	/* TODO: __TPP_RANDOM */
#endif /* !TPP_HAVE_MACRO___TPP_RANDOM */
#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE
	/* TODO: __TPP_STR_DECOMPILE */
#endif /* !TPP_HAVE_MACRO___TPP_STR_DECOMPILE */
#if TPP_HAVE_MACRO___TPP_STR_SUBSTR
	/* TODO: __TPP_STR_SUBSTR */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SUBSTR */
#if TPP_HAVE_MACRO___TPP_STR_PACK
	/* TODO: __TPP_STR_PACK */
#endif /* !TPP_HAVE_MACRO___TPP_STR_PACK */
#if TPP_HAVE_MACRO___TPP_STR_SIZE
	/* TODO: __TPP_STR_SIZE */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SIZE */
#if TPP_HAVE_MACRO___TPP_COUNT_TOKENS
	/* TODO: __TPP_COUNT_TOKENS */
#endif /* !TPP_HAVE_MACRO___TPP_COUNT_TOKENS */
/************************************************************************/



	default: {
		/* Check for a pre-defined, builtin macro expansion */
		tpp_builtin_macro const *builtin_macro;
		builtin_macro = tpp_macro_getbuiltin(tok);
		if (builtin_macro != NULL) {
			return tpp_lexer_push_textfile_inherited(self, builtin_macro->tbm_body,
			                                         builtin_macro->tbm_body_size,
			                                         NULL);
		}
	}	break;

	}

	/* Fallback: act as though the macro takes no arguments, and expands to itself:
	 * >> #define SOME_MACRO SOME_MACRO */
	return tok;
}
#endif /* TPP_HAVE_CPP_BUILTIN_MACROS */
#endif /* TPP_HAVE_CPP_MACROS */

/* Handle a keyword-style macro.
 * @return: TPP_TOK_EOF: Caller should yield again.
 * @return: * : The new expansion token after keywords were handled */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_keyword(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_keyword const *const keyword = token->tt_kwd;

	/* Emit warnings for "deprecated" keywords. */
#if TPP_HAVE_TPP_W_DEPRECATED_KEYWORD && TPP_HAVE_PRAGMA_DEPRECATED
	if (keyword->tk_misc) {
		tpp_keyword_misc const *misc = keyword->tk_misc;
		if (misc->tkm_flags & TPP_KEYWORD_FLAG_IS_DEPRECATED) {
#if TPP_HAVE_PRAGMA_GCC_POISON && TPP_HAVE_CPP_MACROS
			if ((misc->tkm_flags & TPP_KEYWORD_FLAG_IS_POISONED) &&
			    (tpp_lexer_getfile(self)->tf_kind == TPP_FILE_KIND_MACRO)) {
				/* Don't emit warning */
			} else
#endif /* TPP_HAVE_PRAGMA_GCC_POISON && TPP_HAVE_CPP_MACROS */
			{
				tpp_errno error = tpp_lexer_warnf(self, TPP_W_DEPRECATED_KEYWORD);
				if (TPP_ISERR(error))
					return TPP_TOK_OFERR(error);
			}
		}
	}
#endif /* TPP_HAVE_TPP_W_DEPRECATED_KEYWORD && TPP_HAVE_PRAGMA_DEPRECATED */

#if TPP_HAVE_CPP_MACROS

	/* Check if macros are runtime-configurable and currently disabled. */
#if TPP_CONF_IS_RT(TPP_HAVE_CPP_MACROS)
	if (!tpp_lexer_has(self, CPP_MACROS))
		return tok;
#endif /* TPP_CONF_IS_RT(TPP_HAVE_CPP_MACROS) */

	/* Check if this keyword should be expanded as a macro.
	 * This also does the is-enabled checks for builtin macros. */
#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
	if (!tpp_lexer_getkeyworddefined(self, keyword))
		return tok;
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */

	/* Check for explicitly defined macros... */
	{
		tpp_macro *const macro = keyword->tk_macro;
		if (macro) {
			/* Check if expansion of the macro is allowed. */
			if (macro->tm_expansions > 0) {
#if TPP_HAVE_MACRO_RECURSION
				if (!(macro->tm_flags & TPP_MACRO_FLAG_SELFEXPAND))
#endif /* TPP_HAVE_MACRO_RECURSION */
				{
					return tok;
				}
			}

			/* Expand user-defined macro... */
			return tpp_lexer_expand_macro(self, macro);
		}
	}

	/* Expand builtin macros (if enabled)
	 * NOTE: Don't need to check "TPP_FEAT_CPP_BUILTIN_MACROS" here; that feature
	 *       flag (if enabled) was already checked by "tpp_lexer_getkeyworddefined()" */
#if TPP_HAVE_CPP_BUILTIN_MACROS
	return tpp_lexer_yield_handle_builtin_macro(self, tok);
#else /* TPP_HAVE_CPP_BUILTIN_MACROS */
	return tok;
#endif /* !TPP_HAVE_CPP_BUILTIN_MACROS */
#else /* TPP_HAVE_CPP_MACROS */
	return tok;
#endif /* !TPP_HAVE_CPP_MACROS */
}

/* Wrapper around `tpp_lexer_yieldpp()' that adds handling for macro expansion.
 * @return: * :                  The newly read token (after accounting for macros)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldpp(self);
	if (TPP_TOK_ISKEYWORD(result)) {
		result = tpp_lexer_yield_handle_keyword(self, result);
		if (result == TPP_TOK_EOF)
			goto again;
	}
	return result;
}


#if TPP_HAVE_FILE_NONBLOCK

/* Same as `tpp_lexer_yield()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_blocking(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yield(self);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NONBLOCK);
		file->tf_data.td_io.tff_flags &= ~TPP_FILE_IOFLAGS_NONBLOCK;
		tpp_lexer_autopopfile_pushoff(self);
		result = tpp_lexer_yield(self);
		tpp_lexer_autopopfile_pop(self);
		file->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}

/* Same as `tpp_lexer_yieldpp()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp_blocking(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldpp(self);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NONBLOCK);
		file->tf_data.td_io.tff_flags &= ~TPP_FILE_IOFLAGS_NONBLOCK;
		tpp_lexer_autopopfile_pushoff(self);
		result = tpp_lexer_yieldpp(self);
		tpp_lexer_autopopfile_pop(self);
		file->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}

/* Same as `tpp_lexer_yieldraw_at()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_blocking(tpp_lexer *tpp_restrict self, tpp_char const **p_pos) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldraw_at(self, p_pos);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NONBLOCK);
		file->tf_data.td_io.tff_flags &= ~TPP_FILE_IOFLAGS_NONBLOCK;
		tpp_lexer_autopopfile_pushoff(self);
		result = tpp_lexer_yieldraw_at(self, p_pos);
		tpp_lexer_autopopfile_pop(self);
		file->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}

#endif /* TPP_HAVE_FILE_NONBLOCK */

#if TPP_HAVE_LEXER_SKIP

static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_token_matches(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token *const token = tpp_lexer_gettoken(self);

	/* Check for simple (expected) case: the current token is correct */
	if tpp_likely(token->tt_id == tok)
		return true;

	/* If "tok" is a single-char token, see if the currently
	 * loaded token is a multi-char token that starts with
	 * the same value. */
	if ((TPP_TOK_ISCHAR(tok)) &&
	    (token->tt_start < token->tt_end) &&
	    (*token->tt_start == (tpp_char)(unsigned int)tok)) {
		token->tt_end = token->tt_start + 1;
		token->tt_id  = tok;
		return true;
	}

	/* Handle stuff like "tok == '>>' && CURRENT_TOKEN == '>>>'", etc. */
	switch (token->tt_id) {

#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT && TPP_HAVE_TPP_TOK_DOT_DOT
	case TPP_TOK_DOT_DOT_DOT: /* "..." */
		if (tok == TPP_TOK_DOT_DOT)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL && TPP_HAVE_TPP_TOK_STAR_STAR
	case TPP_TOK_STAR_STAR_EQUAL: /* "**=" */
		if (tok == TPP_TOK_STAR_STAR)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL && TPP_HAVE_TPP_TOK_SLASH_SLASH
	case TPP_TOK_SLASH_SLASH_EQUAL: /* "//=" */
		if (tok == TPP_TOK_SLASH_SLASH)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	case TPP_TOK_EXCLAIM_EQUAL_EQUAL: /* "!==" */
		if (tok == TPP_TOK_EXCLAIM_EQUAL)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR && TPP_HAVE_TPP_TOK_EQUAL_STAR
	case TPP_TOK_EQUAL_STAR_STAR: /* "=**" */
		if (tok == TPP_TOK_EQUAL_STAR)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH && TPP_HAVE_TPP_TOK_EQUAL_SLASH
	case TPP_TOK_EQUAL_SLASH_SLASH: /* "=//" */
		if (tok == TPP_TOK_EQUAL_SLASH)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR && TPP_HAVE_TPP_TOK_MINUS_RANGLE
	case TPP_TOK_MINUS_RANGLE_STAR: /* "->*" */
		if (tok == TPP_TOK_MINUS_RANGLE)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if ((TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL ||    \
      TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM) && \
     TPP_HAVE_TPP_TOK_EQUAL_EQUAL)
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	case TPP_TOK_EQUAL_EQUAL_EQUAL: /* "===" */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	case TPP_TOK_EQUAL_EQUAL_EXCLAIM: /* "==!" */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
		if (tok == TPP_TOK_EQUAL_EQUAL)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if ((TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE ||       \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL) &&       \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE)
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: /* "<<<=" */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
		if (tok == TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE)
			goto set_threechar;
#define WANT_set_threechar
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
		TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE_LANGLE: /* "<<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_EQUAL: /* "<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
		if (tok == TPP_HAVE_TPP_TOK_LANGLE_LANGLE)
			goto set_twochar;
#define WANT_set_twochar
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
		break;
#elif TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: /* "<<<=" */
		if (tok == TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE)
			goto set_threechar;
#define WANT_set_threechar
		break;
#endif /* ... */

#if ((TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL || \
      TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE ||       \
      TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL) &&       \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE)
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: /* ">>>=" */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
		if (tok == TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE)
			goto set_threechar;
#define WANT_set_threechar
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
		TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE_RANGLE: /* ">>>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_EQUAL: /* ">>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
		if (tok == TPP_HAVE_TPP_TOK_RANGLE_RANGLE)
			goto set_twochar;
#define WANT_set_twochar
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
		break;
#elif TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: /* ">>>=" */
		if (tok == TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE)
			goto set_threechar;
#define WANT_set_threechar
		break;
#endif /* ... */

#if ((TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE || \
      TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE) &&       \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE)
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE: /* "=<<<" */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE: /* "=<<" */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
		if (tok == TPP_TOK_EQUAL_LANGLE)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE || \
      TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE) &&       \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE)
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE: /* "=>>>" */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE: /* "=>>" */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
		if (tok == TPP_TOK_EQUAL_RANGLE)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE ||  \
      TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE) && \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL)
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	case TPP_TOK_LANGLE_EQUAL_RANGLE: /* "<=>" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	case TPP_TOK_LANGLE_EQUAL_LANGLE: /* "<=<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
		if (tok == TPP_TOK_LANGLE_EQUAL)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE ||  \
      TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE) && \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL)
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	case TPP_TOK_RANGLE_EQUAL_LANGLE: /* ">=<" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	case TPP_TOK_RANGLE_EQUAL_RANGLE: /* ">=>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
		if (tok == TPP_TOK_RANGLE_EQUAL)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE ||  \
      TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE) && \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS)
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	case TPP_TOK_LANGLE_MINUS_RANGLE: /* "<->" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	case TPP_TOK_LANGLE_MINUS_LANGLE: /* "<-<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
		if (tok == TPP_TOK_LANGLE_MINUS)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE ||  \
      TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE) && \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS)
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	case TPP_TOK_RANGLE_MINUS_LANGLE: /* ">-<" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	case TPP_TOK_RANGLE_MINUS_RANGLE: /* ">->" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
		if (tok == TPP_TOK_RANGLE_MINUS)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#ifdef WANT_set_twochar
#undef WANT_set_twochar
		{
			tpp_char const *iter;
set_twochar:
			iter = token->tt_start + 1;
			iter = tpp_skipbse_fwd(iter, token->tt_end, tpp_lexer_getfile(self));
			token->tt_end = iter + 1;
			token->tt_id  = tok;
			return true;
		}
#endif /* WANT_set_twochar */

#ifdef WANT_set_threechar
#undef WANT_set_threechar
		{
			tpp_char const *iter;
set_threechar:
			iter = token->tt_start + 1;
			iter = tpp_skipbse_fwd(iter, token->tt_end, tpp_lexer_getfile(self));
			iter = iter + 1;
			iter = tpp_skipbse_fwd(iter, token->tt_end, tpp_lexer_getfile(self));
			token->tt_end = iter + 1;
			token->tt_id  = tok;
			return true;
		}
#endif /* WANT_set_threechar */

	default: break;
	}

	return false;
}

/* Same as "tpp_lexer_skip()", but don't advance to the next token,
 * except in those cases where the requested "tok" could be found
 * a little further up ahead, and the implementation decided that
 * the tokens that lay in-between should be skipped.
 *
 * @return: * :                 The currently loaded token
 * @return: tok:                Success
 * @return: TPP_TOK_ENOMEM:     Out of memory
 * @return: TPP_TOK_EIO:        I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:  Lexer error
 * @return: TPP_TOK_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_require(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_char const *pos;
	tpp_token_id result;
	tpp_lexer_seek_backup backup;

	/* Check if token matches */
	if (tpp_lexer_token_matches(self, tok))
		return tok;

	/* Emit warning */
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN
	{
		tpp_errno error;
		char const *expected = tpp_lexer_reprtokenid(self, tok);
		if tpp_unlikely(!expected)
			expected = "?";
		error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN, expected);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN */

	/* Start seeking ahead... */
	pos = tpp_lexer_seek_start(self, &backup);

	/* Skip over whitespace */
	result = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(result))
		result = tpp_lexer_yieldraw_at_blocking(self, &pos);
	if (TPP_TOK_ISERR(result))
		goto err_result_rollback;

	/* Check if token matches after whitespace... */
	if (tpp_lexer_token_matches(self, tok)) {
		tpp_lexer_seek_commit(self, pos);
		return tok;
	}

	/* Try to seek ahead to find "tok" when it's (e.g.) a '(' (to
	 * deal with cases where the user added some extra, unrelated
	 * tokens before the one we're expecting)
	 *
	 * - If "tok == ')", find next unmatched ')'
	 * - If "tok == ']", find next unmatched ']'
	 * - If "tok == '}", find next unmatched '}'
	 * - If "tok == '>", find next unmatched '>' */
#if TPP_HAVE_LEXER_MANUALPOPFILE
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	if (tok == ')' || tok == ']' || tok == '}' || tok == '>')
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	if (tok == ')')
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	{
		tpp_token *const token = tpp_lexer_gettoken(self);
		tpp_lexer_arginfo argv[1];
		tpp_size argc = 1;
		token->tt_start = token->tt_end;
		token->tt_end   = pos;
		tpp_lexer_manualpopfile_start(self);
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
		{
			tpp_token_id lparen_kind;
			switch (tok) {
			case ')': lparen_kind = TPP_TOK_OFCHAR('('); break;
			case ']': lparen_kind = TPP_TOK_OFCHAR('['); break;
			case '}': lparen_kind = TPP_TOK_OFCHAR('{'); break;
			case '>': lparen_kind = TPP_TOK_OFCHAR('<'); break;
			default: tpp_unreachable();
			}
			result = tpp_lexer_seekpp_rparen_ex(self, argv, &argc, &pos, NULL,
			                                    TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS |
			                                    TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK |
			                                    TPP_LEXER_SEEK_RPAREN_FLAG_NOWARNEOF,
			                                    lparen_kind);
		}
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
		result = tpp_lexer_seekpp_rparen(self, argv, &argc, &pos, NULL,
		                                 TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS |
		                                 TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK |
		                                 TPP_LEXER_SEEK_RPAREN_FLAG_NOWARNEOF);
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
		if (!TPP_TOK_ISERR(result) && argc)
			tpp_lexer_arginfo_fini(&argv[0]);
		if (result == tok) {
			/* Found it! */
			tpp_lexer_manualpopfile_break_commit(self);
			token->tt_start = token->tt_end - 1;
			return result;
		}
		tpp_lexer_getfile(self)->tf_pos = pos;
		tpp_lexer_manualpopfile_end_rollback(self);
		token->tt_end   = token->tt_start + backup.tlsb_len;
		token->tt_id    = backup.tlsb_id;
		token->tt_kwd   = backup.tlsb_kwd;
		if (!TPP_TOK_ISERR(result))
			result = backup.tlsb_id;
		return result;
	}
#endif /* TPP_HAVE_LEXER_MANUALPOPFILE */

	if (tok == ',') {
		/* TODO: Seek to the next ','-token, so-long as no unmatched ) ] } or > is found first */
	}

err_result_rollback:
	tpp_lexer_seek_rollback(self, &backup);
	return result;
}

/* Check that the currently loaded token is 'tok'. If so, "tpp_lexer_yield_blocking()" to
 * the next token (which is also returned). Otherwise, trigger 'TPP_W_UNEXPECTED_TOKEN'
 * and (if that warning wasn't fatal), try to seek ahead to see if "tok" can be found
 * somewhere close by (depending on what 'tok' and what was actually loaded on entry)
 *
 * NOTE: This function automatically handles "TPP_TOK_EWOULDBLOCK"
 *
 * @return: * :                 The token that comes after the one that was just skipped
 * @return: TPP_TOK_ENOMEM:     Out of memory
 * @return: TPP_TOK_EIO:        I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:  Lexer error
 * @return: TPP_TOK_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token_id result = tpp_lexer_require(self, tok);
	if (result == tok)
		result = tpp_lexer_yield_blocking(self);
	return result;
}

#endif /* TPP_HAVE_LEXER_SKIP */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-decodestring.c                                     */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_DECODESTRING

#if (TPP_HAVE_TPP_TOK_STRING ||                   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
     TPP_HAVE_TPP_TOK_CHAR ||                     \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL)

/* Decode string: "foobar fdasudfad"
 *                 ^start          ^end
 */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4, 5)) tpp_ssize TPPCALL
tpp_token_decodestring_basic(tpp_lexer *tpp_restrict self,
                             tpp_char const *start,
                             tpp_char const *end,
                             tpp_formatprinter data_printer,
                             tpp_formatprinter utf8_printer,
                             void *arg) {
	tpp_char ch;
	tpp_ssize temp, result = 0;
	tpp_char const *iter = start;
	tpp_assert(start <= end);

again:
	if (iter >= end)
		goto done;
	ch = *iter++;

	/* Decode trigraphs... */
#if TPP_HAVE_TRIGRAPHS
	if (ch == '?' && ((iter + 1) < end && iter[0] == '?') &&
	    tpp_lexer_has(self, TRIGRAPHS)) {
		switch (iter[1]) {
		case '=': ch = '#'; break;
		case '(': ch = '['; break;
		case ')': ch = ']'; break;
		case '\'': ch = '^'; break;
		case '<': ch = '{'; break;
		case '!': ch = '|'; break;
		case '>': ch = '}'; break;
		case '-': ch = '~'; break;
		case '?': ch = '?'; break;
		case '/': ch = '\\'; break;
		default: goto not_trigraph; /* Not actually a trigraph escape sequence */
		}
		--iter;

		/* No need to warn about trigraph -- was already done in `tpp_lexer_yieldraw()' */

		/* Print trigraph character (but also handle case where "??/" was encoded) */
		temp = (*data_printer)(arg, start, (tpp_size)(iter - start));
		if (temp < 0)
			goto err_temp;
		result += temp;
		iter += 3;
		start = iter;
		if (ch != '\\') {
			temp = (*data_printer)(arg, &ch, 1);
			if (temp < 0)
				goto err_temp;
			result += temp;
			goto again;
		}
	} else
#endif /* TPP_HAVE_TRIGRAPHS */
	{
#if TPP_HAVE_TRIGRAPHS
not_trigraph:
#endif /* TPP_HAVE_TRIGRAPHS */
		if (ch != '\\')
			goto again;

		/* Print everything up until the \-character */
		temp = (*data_printer)(arg, start, (tpp_size)((iter - 1) - start));
		if (temp < 0)
			goto err_temp;
		result += temp;
	}

	/* Deal with \-escape sequence */
	if (iter >= end) {
		ch = '\0';
		goto handle_unknown_escape_sequence;
	}
	ch = *iter++;
	switch (ch) {

	case '\\':
	case '\'':
	case '\"':
		/* Escape sequences that escape to themselves. */
		start = iter - 1;
		goto again;

#if TPP_HAVE_TRIGRAPHS
	case '?':
		if ((iter + 1) >= end)
			goto handle_unknown_escape_sequence;
		if (iter[0] != '?')
			goto handle_unknown_escape_sequence;

		/* Only '??/??/' is allowed (which is the same as \\; which is printed as \) */
		if (iter[1] != '/')
			goto handle_unknown_escape_sequence;
		if (!tpp_lexer_has(self, TRIGRAPHS))
			goto handle_unknown_escape_sequence;
		iter += 2;
		goto print_backslash_and_flush_at_iter;
#endif /* TPP_HAVE_TRIGRAPHS */

		/* Conventional escape sequences... */
	case 'a': ch = 0x07; goto print_ch;
	case 'b': ch = 0x08; goto print_ch;
	case 't': ch = 0x09; goto print_ch;
	case 'n': ch = 0x0a; goto print_ch;
	case 'v': ch = 0x0b; goto print_ch;
	case 'f': ch = 0x0c; goto print_ch;
	case 'r': ch = 0x0d; goto print_ch;

#if TPP_HAVE_ESCAPE_E_IN_STRINGS
	case 'e':
		if (!tpp_lexer_has(self, ESCAPE_E_IN_STRINGS))
			goto handle_unknown_escape_sequence;
		ch = 0x1b;
		goto print_ch;
#endif /* !TPP_HAVE_ESCAPE_E_IN_STRINGS */

#if TPP_HAVE_ESCAPE_S_IN_STRINGS
	case 's':
		if (!tpp_lexer_has(self, ESCAPE_S_IN_STRINGS))
			goto handle_unknown_escape_sequence;
		ch = 0x20;
		goto print_ch;
#endif /* !TPP_HAVE_ESCAPE_S_IN_STRINGS */

print_ch:
		temp = (*data_printer)(arg, &ch, 1);
		if (temp < 0)
			goto err_temp;
		result += temp;
		break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7': {
		/* Octal escape sequence */
		tpp_char word = (tpp_char)(ch - '0');
		if (iter < end && (*iter >= '0' && *iter <= '7')) {
			ch = *iter++;
			word <<= 3;
			word |= (tpp_char)(ch - '0');
		}
		if (iter < end && (*iter >= '0' && *iter <= '7') && (word <= 037)) {
			ch = *iter++;
			word <<= 3;
			word |= (tpp_char)(ch - '0');
		}
		ch = word;
		goto print_ch;
	}	break;

	case 'x': {
		tpp_char word;
		if (iter >= end)
			goto handle_unknown_escape_sequence;
		ch = *iter++;
		if (ch >= '0' && ch <= '9') {
			word = (tpp_char)(ch - '0');
		} else if (ch >= 'a' && ch <= 'f') {
			word = 10 + (tpp_char)(ch - 'a');
		} else if (ch >= 'A' && ch <= 'F') {
			word = 10 + (tpp_char)(ch - 'A');
		} else {
			goto handle_unknown_escape_sequence;
		}
		if (iter < end) {
			ch = *iter;
			if (ch >= '0' && ch <= '9') {
				word <<= 4;
				word |= (tpp_char)(ch - '0');
				++iter;
			} else if (ch >= 'a' && ch <= 'f') {
				word <<= 4;
				word |= 10 + (tpp_char)(ch - 'a');
				++iter;
			} else if (ch >= 'A' && ch <= 'F') {
				word <<= 4;
				word |= 10 + (tpp_char)(ch - 'A');
				++iter;
			}
		}
		ch = word;
		goto print_ch;
	}	break;

#if TPP_HAVE_BSE
	case '\r':
	case '\n':
		/* Escaped line-feed */
		if (!tpp_lexer_has(self, BSE))
			goto handle_unknown_escape_sequence;
		if (ch == '\r' && (iter < end) && *iter == '\n')
			++iter;
		break;
#endif /* TPP_HAVE_BSE */

	case 'u':
	case 'U': {
		tpp_unichar uc = 0;
		unsigned int num_nibble = ch == 'u' ? 4 : 8;
		unsigned int cur_nibble = 0;
		tpp_char utf8_buf[TPP_UTF8_MAXLEN];
		tpp_size utf8_len;
		if (iter >= end)
			goto handle_unknown_escape_sequence;
		do {
			tpp_char nibble;
			ch = *iter++;
			if (ch >= '0' && ch <= '9') {
				nibble = (tpp_char)(ch - '0');
			} else if (ch >= 'a' && ch <= 'f') {
				nibble = 10 + (tpp_char)(ch - 'a');
				++iter;
			} else if (ch >= 'A' && ch <= 'F') {
				nibble = 10 + (tpp_char)(ch - 'A');
			} else {
				if (cur_nibble == 0)
					goto handle_unknown_escape_sequence;
				break;
			}
			uc <<= 4;
			uc |= nibble;
			if (iter >= end)
				break;
		} while (++cur_nibble < num_nibble);

		/* Encode as utf-8 */
		utf8_len = (tpp_size)(tpp_unicode_writeutf8(utf8_buf, uc) - utf8_buf);
		temp = (*utf8_printer)(arg, utf8_buf, utf8_len);
		if (temp < 0)
			goto err_temp;
		result += temp;
	}	break;

	default: {
#if TPP_HAVE_BSE && TPP_HAVE_UNICODE
		tpp_char const *bse_iter;
		if (ch >= 0x80 && tpp_file_isutf8(tpp_lexer_getfile(self))) {
			if (tpp_lexer_has(self, BSE)) {
				bse_iter = iter;
				tpp_unichar uc;
#if TPP_HAVE_BSE_WHITESPACE
again_read_unicode_whitespace_after_backslash:
#endif /* TPP_HAVE_BSE_WHITESPACE */
				uc = tpp_unicode_readutf8(&bse_iter, end);
				if (tpp_unicode_islf(uc)) {
					/* Escaped unicode linefeed. */
					iter = bse_iter;
#if TPP_HAVE_BSE_WHITESPACE
					if (uc == '\r' && (iter < end) && *iter == '\n')
						++iter;
#endif /* TPP_HAVE_BSE_WHITESPACE */
					break;
				} else
#if TPP_HAVE_BSE_WHITESPACE
				if (tpp_unicode_isspace(uc)) {
					if (tpp_lexer_has(self, BSE_WHITESPACE))
						goto again_read_unicode_whitespace_after_backslash;
				} else
#endif /* TPP_HAVE_BSE_WHITESPACE */
				{
				}
			}
		} else
#endif /* TPP_HAVE_BSE && TPP_HAVE_UNICODE */
#if TPP_HAVE_BSE_WHITESPACE
		if (tpp_ascii_isspace_nolf(ch)) {
			if (tpp_lexer_has(self, BSE) &&
			    tpp_lexer_has(self, BSE_WHITESPACE)) {
				tpp_char wch;
#if !TPP_HAVE_UNICODE
				tpp_char const *bse_iter;
#endif /* TPP_HAVE_UNICODE */
				bse_iter = iter;
				do {
					if (bse_iter >= end)
						goto handle_unknown_escape_sequence;
					wch = *bse_iter++;
				} while (tpp_ascii_isspace_nolf(wch));
#if TPP_HAVE_UNICODE
				if (wch >= 0x80 && tpp_file_isutf8(tpp_lexer_getfile(self))) {
					--bse_iter;
					goto again_read_unicode_whitespace_after_backslash;
				}
#endif /* TPP_HAVE_UNICODE */
				if (tpp_ascii_islf(wch)) {
					iter = bse_iter; /* Escaped linefeed. */
					if (wch == '\r' && (iter < end) && *iter == '\n')
						++iter;
					break;
				}
			}
		} else
#endif /* TPP_HAVE_BSE_WHITESPACE */
		{
		}

handle_unknown_escape_sequence:
		--iter;
#if TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE
		{
			tpp_errno error = tpp_lexer_warnf_at(self, iter, TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE, ch);
			if (TPP_ISERR(error))
				return (tpp_ssize)error;
		}
#endif /* TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE */
#if TPP_HAVE_TRIGRAPHS
		if (iter[-1] != '\\') {
print_backslash_and_flush_at_iter:
			temp = (*data_printer)(arg, (tpp_char const *)"\\", 1);
			if (temp < 0)
				goto err_temp;
			result += temp;
			start = iter;
		} else
#endif /* TPP_HAVE_TRIGRAPHS */
		{
			start = iter - 1;
		}
		goto again;
	}	break;

	}

	start = iter;
	goto again;
done:
	if (start < end) {
		temp = (*data_printer)(arg, start, (tpp_size)(end - start));
		if (temp < 0)
			goto err_temp;
		result += temp;
	}
	return result;
err_temp:
	return temp;
}
#endif /* ... */


#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
/* Decode string:
 * |"""
 * |.   foobar fdasudfad
 * |    fasdf\
 * |"""      ^end
 *  ^ start@.
 */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4, 5)) tpp_ssize TPPCALL
tpp_token_decodestring_block(tpp_lexer *tpp_restrict self,
                             tpp_char const *start,
                             tpp_char const *end,
                             tpp_formatprinter data_printer,
                             tpp_formatprinter utf8_printer,
                             void *arg) {
	tpp_assert(start <= end);
	/* TODO: Find width of common line-prefix */
	/* TODO: If common line-prefix is empty, can use "tpp_token_decodestring_basic()" to decode */
	/* TODO: Decode each string-block line by passing it to "tpp_token_decodestring_basic()"
	 *       Include the trailing line-feed of every line here (the last line may not have a
	 *       trailing line-feed if the block-string ends with """ on the same line) */
	(void)self;
	(void)start;
	(void)end;
	(void)data_printer;
	(void)utf8_printer;
	(void)arg;
	return 0;
}
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */

#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
#if TPP_HAVE_BSE && 1 /* XXX: Should BSE really be skipped here? I mean:
                       *      this is ~supposed~ to be for *raw* strings,
                       *      so you'd think BSE should be included... */
#define tpp_token_decodestring_raw_SKIPS_BSE 1
/* Decode string: R"FOO(bla bla bla)FOO"
*                       ^start     ^end */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4, 5)) tpp_ssize TPPCALL
tpp_token_decodestring_raw(tpp_lexer *tpp_restrict self,
                           tpp_char const *start,
                           tpp_char const *end,
                           tpp_formatprinter data_printer,
                           void *arg) {
	tpp_assert(start <= end);
	(void)self;
	/* TODO: Print input as-is, but skip over BSE */
	return (*data_printer)(arg, start, (tpp_size)(end - start));
}
#else
#define tpp_token_decodestring_raw_SKIPS_BSE 0
#define tpp_token_decodestring_raw(self, start, end, data_printer, arg) \
	((*(data_printer))(arg, start, (tpp_size)((end) - (start))))
#endif
#endif /* ... */


#if TPP_HAVE_BSE
#define tpp_bse_seek_until_fwd(ptr, ch) while (*(ptr) != (ch)) ++ptr
#define tpp_bse_seek_until_bck(ptr, ch) while ((ptr)[-1] != (ch)) --ptr
#else /* TPP_HAVE_BSE */
#define tpp_bse_seek_until_fwd(ptr, ch) (void)0
#define tpp_bse_seek_until_bck(ptr, ch) (void)0
#endif /* !TPP_HAVE_BSE */


/* Print the unescaped representation of the string-token described by "self"
 * The caller must ensure that `TPP_TOK_ISSTRING(tpp_lexer_gettoken(self)->tt_id)'
 *
 * @param: data_printer: Printer used to fast-forward string data from token inputs, as well as \xAB
 * @param: utf8_printer: Printer used to emit explicitly utf-8 encoded data from \uABCD and \U876543210,
 *                       as well as regular text-data when the "tpp_file_isutf8(tpp_lexer_getfile(self))"
 *
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers
 * @return: (tpp_ssize)TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                                     a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: (tpp_ssize)TPP_ENOMEM:     Out of memory  (can only happen inside of `tpp_lexer_warnf()')
 * @return: (tpp_ssize)TPP_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_ssize TPPCALL
tpp_lexer_decodestring(tpp_lexer *tpp_restrict self,
                       tpp_formatprinter data_printer,
                       tpp_formatprinter utf8_printer,
                       void *arg) {
#undef HAVE_do_decode_basic
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_char const *start = token->tt_start;
	tpp_char const *end   = token->tt_end;
	tpp_assert(TPP_TOK_ISSTRING(token->tt_id));
	switch (token->tt_id) {

#if (TPP_HAVE_TPP_TOK_STRING ||                   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CHAR ||                     \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL)
#if TPP_HAVE_BSE
#if (TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL)
	{
		tpp_char quote_char;
	_TPP_CASE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF32_STRING_LITERAL
#if ((TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
      TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
      TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
      TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL) && \
     (TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
      TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
      TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||   \
      TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL))
		quote_char = TPP_TOK_ISSTRING_SQUOTE(token->tt_id) ? '\'' : '"';
#elif (TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
       TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
       TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
       TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL)
		quote_char = '"';
#else /* ... */
		quote_char = '\'';
#endif /* !... */
		tpp_bse_seek_until_fwd(start, quote_char);
	}	TPP_FALLTHRU
#endif /* ... */
#else /* TPP_HAVE_BSE */
#if (TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL)
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
		++start;
		TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
	_TPP_CASE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
		++start;
		TPP_FALLTHRU
#else /* ... */
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
		start += 2;
		TPP_FALLTHRU
#endif /* ... */
#endif /* !TPP_HAVE_BSE */
	_TPP_CASE_TPP_TOK_CHAR
	_TPP_CASE_TPP_TOK_STRING {
		++start; /* Skip leading quote */
		if (start < end)
			--end; /* Skip trailing quote */
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		end   = tpp_skipbse_bck(end, start, tpp_lexer_getfile(self));
		tpp_assert(start <= end);
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
do_decode_basic:
#define HAVE_do_decode_basic
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
		return tpp_token_decodestring_basic(self, start, end,
		                                    data_printer,
		                                    utf8_printer, arg);
	}	break;
#endif /* ... */

#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL {
		while ((start < end) && *start != '(')
			++start;
		while ((start < end) && end[-1] != ')')
			--end;
		tpp_assert(start <= end);
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
		goto cxx_raw_string_common;
#else /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
		++start; /* Skip over leading '(' */
		if (start < end)
			--end; /* Skip over trailing ')' */

		/* Skip Any remaining BSE sequences at the head/tail */
#if tpp_token_decodestring_raw_SKIPS_BSE
		tpp_assert(start <= end);
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		end   = tpp_skipbse_bck(end, start, tpp_lexer_getfile(self));
#endif /* tpp_token_decodestring_raw_SKIPS_BSE */
		tpp_assert(start <= end);

		/* Print string */
		return tpp_token_decodestring_raw(self, start, end, data_printer, arg);
#endif /* !TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL && !TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
	}	break;
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */

#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_RAW_STRING_LITERAL
	_TPP_CASE_TPP_TOK_RAW_CHAR_LITERAL {
		tpp_char quote_char;
		switch (token->tt_id) {
		_TPP_CASE_TPP_TOK_RAW_STRING_LITERAL
			quote_char = '"';
			break;
		_TPP_CASE_TPP_TOK_RAW_CHAR_LITERAL
			quote_char = '\'';
			break;
		default: tpp_unreachable();
		}
		/*tpp_assert(end[-1] == '"' || end[-1] == '\'');*/
		++start; /* Skip leading 'R' / 'r' */
		tpp_bse_seek_until_fwd(start, quote_char);
		tpp_assert(*start == quote_char);
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
cxx_raw_string_common:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
		if (start < end)
			++start; /* Skip leading " / ' / ( */
		if (start < end)
			--end; /* Skip trailing " / ' / ) */

		/* Skip Any remaining BSE sequences at the head/tail */
#if tpp_token_decodestring_raw_SKIPS_BSE
		tpp_assert(start <= end);
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		end   = tpp_skipbse_bck(end, start, tpp_lexer_getfile(self));
#endif /* tpp_token_decodestring_raw_SKIPS_BSE */
		tpp_assert(start <= end);

		/* Print string */
		return tpp_token_decodestring_raw(self, start, end, data_printer, arg);
	}	break;
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */

#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_BLOCK_STRING_LITERAL
	_TPP_CASE_TPP_TOK_BLOCK_CHAR_LITERAL {
		tpp_char lf_ch;
		tpp_char const quote_ch = *start++;
		tpp_assert(quote_ch == '"' || quote_ch == '\'');
		if (start < end && end[-1] == quote_ch)
			--end;
		tpp_bse_seek_until_fwd(start, quote_ch);
		tpp_assert(start < end);
		tpp_assert(*start == quote_ch);
		++start; /* Skip second quote */
		tpp_bse_seek_until_fwd(start, quote_ch);
		tpp_assert(start < end);
		tpp_assert(*start == quote_ch);
		++start; /* Skip third quote */

		tpp_bse_seek_until_bck(end, quote_ch);
		tpp_assert(start <= end);
		if ((start < end) && end[-1] == quote_ch)
			--end; /* Skip second quote */
		tpp_bse_seek_until_bck(end, quote_ch);
		tpp_assert(start <= end);
		if ((start < end) && end[-1] == quote_ch)
			--end; /* Skip third quote */

		/* Skip Any remaining BSE sequences at the head/tail */
		tpp_assert(start <= end);
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		end   = tpp_skipbse_bck(end, start, tpp_lexer_getfile(self));
		tpp_assert(start <= end);

		/* Check if block-string starts with a line-feed character.
		 * If it doesn't, must decode as a "basic" string (without
		 * the special indentation-removal behavior) */
		if (start >= end)
			goto do_decode_basic;
		lf_ch = *start;
		if (tpp_ascii_islf(lf_ch)) {
			++start;
			if (lf_ch == '\r' && start < end && *start == '\n')
				++start; /* Treat \r\n as a single line-feed */
		} else
#if TPP_HAVE_UNICODE
		/* Support for unicode line-feeds... */
		if (lf_ch >= 0x80 && tpp_file_isutf8(tpp_lexer_getfile(self))) {
			tpp_char const *nstart = start;
			tpp_unichar uc = tpp_unicode_readutf8(&nstart, end);
			if (!tpp_unicode_islf(uc))
				goto do_decode_basic;
			start = nstart;
		} else
#endif /* TPP_HAVE_UNICODE */
		{
#ifdef HAVE_do_decode_basic
			goto do_decode_basic;
#else /* HAVE_do_decode_basic */
#if TPP_HAVE_UNICODE
do_decode_basic:
#endif /* TPP_HAVE_UNICODE */
			return tpp_token_decodestring_basic(self, start, end,
			                                    data_printer,
			                                    utf8_printer, arg);
#endif /* !HAVE_do_decode_basic */
		}
		return tpp_token_decodestring_block(self, start, end,
		                                    data_printer,
		                                    utf8_printer, arg);
	}	break;
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */

	default: tpp_unreachable();
	}
	tpp_unreachable();
#undef HAVE_do_decode_basic
}


/* Same as "tpp_lexer_decodestring()", but also "tpp_lexer_yield()" to the next token.
 * Then, if that token is also string-like (TPP_TOK_ISSTRING()), decode it also,
 * then yield again, and so on, until a non-string-like token is encountered, an
 * error happens, or one of the printers returned a negative value.
 *
 * HINT: This function automatically handles "TPP_EWOULDBLOCK" during
 *       yield by trying again with TPP_FILE_IOFLAGS_NONBLOCK disabled.
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers
 * @return: (tpp_ssize)TPP_ELEXERROR:   Either one of the printers returned this value, or
 *                                      a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: (tpp_ssize)TPP_ENOMEM:      Out of memory
 * @return: (tpp_ssize)TPP_EIO:         I/O error while yielding to next token
 * @return: (tpp_ssize)TPP_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_ssize TPPCALL
tpp_lexer_parsestring_ex(tpp_lexer *tpp_restrict self,
                         tpp_formatprinter data_printer,
                         tpp_formatprinter utf8_printer,
                         void *arg, unsigned int flags) {
	tpp_token_id tok;
	tpp_ssize result = 0, temp;
again:
	temp = tpp_lexer_decodestring(self, data_printer, utf8_printer, arg);
	if tpp_unlikely(temp < 0)
		return temp;
	result += temp;

	/* Yield to next token */
again_yield:
	tok = tpp_lexer_yield_blocking(self);
	switch (tok) {

	TPP_CASE_TPP_TOK_STRING
		goto again;

	case TPP_TOK_SPACE:
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
			goto again_yield;
		break;
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT_LINE
		if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
			goto again_yield;
		break;

	default:
		if (TPP_TOK_ISERR(tok))
			result = (tpp_ssize)TPP_TOK_ASERR(tok);
		break;
	}
	return result;
}


/* Convenience wrapper around `tpp_lexer_parsestring_ex()'
 * On success (!TPP_ISERR(return)), caller must "tpp_string_decref(*p_result)"
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring(tpp_lexer *tpp_restrict self,
                      /*out*/ TPP_REF tpp_string **tpp_restrict p_result,
                      unsigned int flags) {
	tpp_ssize status;
	tpp_string_builder builder;
	tpp_string_builder_init(&builder);
	status = tpp_lexer_parsestring_ex(self,
	                                  &tpp_string_builder_print,
	                                  &tpp_string_builder_print,
	                                  &builder, flags);
	if (status < 0)
		goto err_builder;
	*p_result = tpp_string_builder_pack(&builder);
	return TPP_EOK;
err_builder:
	tpp_string_builder_fini(&builder);
	return (tpp_errno)status;
}


#define TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_EMPTY 0 /* String has 0 (non-empty) chunks */
#define TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_YES   1 /* String has 1 (non-empty) chunk */
#define TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_NO    2 /* String has 2 or more (non-empty) chunks */

#define TPP_LEXER_PARSESTRING_CHUNK_STOP ((tpp_ssize)(TPP_ELAST - 1))
static tpp_ssize TPP_FORMATPRINTER_CC
tpp_lexer_parsestring_chunk_count(void *arg, tpp_char const *text, tpp_size num_bytes) {
	unsigned int *p_count = (unsigned int *)arg;
	(void)text;
	if (num_bytes != 0) {
		/* Update counter:
		 * - TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_EMPTY  ->  TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_YES
		 * - TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_YES    ->  TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_NO
		 *   TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_NO     ->  return TPP_LEXER_PARSESTRING_CHUNK_STOP */
		++*p_count;
		if (*p_count >= TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_NO)
			return TPP_LEXER_PARSESTRING_CHUNK_STOP;
	}
	return 0;
}

/* Check if the currently loaded string-token can be printed in 0/1 chunks
 * @return: * : One of `TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_*' */
static TPP_NONNULL((1)) unsigned int TPPCALL
tpp_lexer_parsestring_is_single_chunk(tpp_lexer *tpp_restrict self) {
	unsigned int chunk_count = TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_EMPTY;
	tpp_ssize decode_status;
#if TPP_HAVE_WARNINGS
	tpp_lexer_state_flags old_state;
	old_state = self->tl_state;
	self->tl_state |= TPP_LEXER_STATE_FLAG_NOWARNINGS;
#endif /* TPP_HAVE_WARNINGS */

	/* Try to decode the string and count how many chunks we encounter */
	decode_status = tpp_lexer_decodestring(self,
	                                       &tpp_lexer_parsestring_chunk_count,
	                                       &tpp_lexer_parsestring_chunk_count,
	                                       &chunk_count);
	tpp_assert(decode_status == 0 ||
	           decode_status == TPP_LEXER_PARSESTRING_CHUNK_STOP);
	(void)decode_status;

#if TPP_HAVE_WARNINGS
	self->tl_state = old_state;
#endif /* TPP_HAVE_WARNINGS */
	return chunk_count;
}

static TPP_NONNULL((1)) unsigned int TPPCALL
tpp_lexer_parsestring_is_single_chunk_at(tpp_lexer *tpp_restrict self,
                                         tpp_char const *token_end) {
	unsigned int result;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_char const *saved_token_end = token->tt_end;
	token->tt_end = token_end;
	result = tpp_lexer_parsestring_is_single_chunk(self);
	token->tt_end = saved_token_end;
	return result;
}

struct tpp_lexer_decodestring_as_single_chunk_data {
	tpp_errno (TPPCALL *tldsascd_cb)(void *arg, tpp_string *chunk,
	                                 tpp_char const *str, tpp_size length);
	void               *tldsascd_arg;
	tpp_string         *tldsascd_chunk;
};

static tpp_ssize TPP_FORMATPRINTER_CC
tpp_lexer_decodestring_as_single_chunk_cb(void *arg, tpp_char const *text, tpp_size num_bytes) {
	tpp_errno error;
	struct tpp_lexer_decodestring_as_single_chunk_data *data;
	if tpp_unlikely(num_bytes == 0)
		return 0;
	data = (struct tpp_lexer_decodestring_as_single_chunk_data *)arg;
	tpp_assert(data->tldsascd_cb != NULL && "Multiple invocations?");
	error = (*data->tldsascd_cb)(data->tldsascd_arg, data->tldsascd_chunk, text, num_bytes);
#if TPP_DEBUG
	data->tldsascd_cb = NULL;
#endif /* TPP_DEBUG */
#ifndef __OPTIMIZE_SIZE__
	if (!TPP_ISERR(error))
		return TPP_LEXER_PARSESTRING_CHUNK_STOP;
#endif /* !__OPTIMIZE_SIZE__ */
	return (tpp_ssize)error;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_decodestring_as_single_chunk(tpp_lexer *tpp_restrict self,
                                       tpp_errno (TPPCALL *cb)(void *arg, tpp_string *chunk,
                                                               tpp_char const *str, tpp_size length),
                                       void *arg) {
	tpp_ssize status;
	struct tpp_lexer_decodestring_as_single_chunk_data data;
	tpp_assert(cb && "NULL-callback given");
	data.tldsascd_cb    = cb;
	data.tldsascd_arg   = arg;
	data.tldsascd_chunk = tpp_lexer_getfile(self)->tf_chunk;
	status = tpp_lexer_decodestring(self,
	                                &tpp_lexer_decodestring_as_single_chunk_cb,
	                                &tpp_lexer_decodestring_as_single_chunk_cb,
	                                &data);
#ifndef __OPTIMIZE_SIZE__
	if (status == TPP_LEXER_PARSESTRING_CHUNK_STOP)
		status = (tpp_ssize)TPP_EOK;
#endif /* !__OPTIMIZE_SIZE__ */
	return (tpp_errno)status;
}

/* Wrapper around `tpp_lexer_parsestring()' that passes the actual string data
 * to a given callback. This function also enables some (optional) optimizations
 * for the most common case where the string token in "self" isn't followed by
 * another string token, and can be printed as a singular chunk. When this is
 * the case, no intermediate heap-buffer needs to be created, as "cb" can just
 * be invoked using the currently loaded file's content-buffer.
 *
 * @param: cb.arg:   Cookie argument (s.a. `arg')
 * @param: cb.chunk: The string-chunk containing "str" (or "NULL" if "str" is statically allocated)
 *                   NOTE: May be non-NULL, even if "str" is statically allocated!
 * @param: flags:    Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning
 * @return: * :             Return value of given "cb" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring_cb(tpp_lexer *self,
                         tpp_errno (TPPCALL *cb)(void *arg, tpp_string *chunk,
                                                 tpp_char const *str, tpp_size length),
                         void *arg, unsigned int flags) {
	unsigned int how;
again:
	how = tpp_lexer_parsestring_is_single_chunk(self);
	if (how == TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_EMPTY) {
		tpp_token_id tok;
again_yield_after_empty:
		tok = tpp_lexer_yield_blocking(self);
		switch (tok) {

		TPP_CASE_TPP_TOK_STRING
			goto again;

		case TPP_TOK_SPACE:
		TPP_CASE_TPP_TOK_COMMENT_NOLINE
			if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
				goto again_yield_after_empty;
			break;
		case TPP_TOK_LF:
		TPP_CASE_TPP_TOK_COMMENT_LINE
			if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
				goto again_yield_after_empty;
			break;

		default:
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			break;
		}

		/* Indicate an empty chunk to the caller */
		return (*cb)(arg, tpp_lexer_getfile(self)->tf_chunk,
		             tpp_lexer_getfile(self)->tf_pos, 0);
	} else if (how == TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_YES) {
		tpp_lexer_seek_backup backup;
		tpp_char const *pos;
		tpp_token_id tok;
		tpp_errno result;

		/* Must make sure that the next token isn't another (non-empty) string */
		pos = tpp_lexer_seek_start(self, &backup);
		tpp_lexer_nowarnings_pushon(self);
again_yield_after_single:
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
		switch (tok) {

		TPP_CASE_TPP_TOK_STRING {
			how = tpp_lexer_parsestring_is_single_chunk_at(self, pos);
			if (how == TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_EMPTY)
				goto again_yield_after_single;

			/* Not possible using a single chunk... */
#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
break_nowarnings_and_do_multi_chunk_string:
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */
			tpp_lexer_nowarnings_break(self);
			tpp_lexer_seek_rollback(self, &backup);
			goto do_multi_chunk_string;
		}	break;

		case TPP_TOK_SPACE:
		TPP_CASE_TPP_TOK_COMMENT_NOLINE
			if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
				goto again_yield_after_single;
			break;
		case TPP_TOK_LF:
		TPP_CASE_TPP_TOK_COMMENT_LINE
			if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
				goto again_yield_after_single;
			break;

#if TPP_HAVE_INCLUDE_STACK
		case TPP_TOK_EOF: {
			/* Check if string continues in the next file... */
			if (!tpp_lexer_getfile(self)->tf_prev)
				break;
			tpp_lexer_seek_rollback(self, &backup);
			tpp_lexer_manualpopfile_start(self);
			tpp_lexer_manualpopfile_popfile(self);
			pos = tpp_lexer_seek_start(self, &backup);
again_yield_after_eof:
			tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
			switch (tok) {

			TPP_CASE_TPP_TOK_STRING
				how = tpp_lexer_parsestring_is_single_chunk_at(self, pos);
				if (how == TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_EMPTY)
					goto again_yield_after_eof;
#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
do_multi_chunk_string_after_eof:
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */
				tpp_lexer_seek_rollback(self, &backup);
				tpp_lexer_manualpopfile_break_rollback(self);
				tpp_lexer_nowarnings_break(self);
				goto do_multi_chunk_string;

			case TPP_TOK_SPACE:
			TPP_CASE_TPP_TOK_COMMENT_NOLINE
				if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
					goto again_yield_after_eof;
				break;

			case TPP_TOK_LF:
			TPP_CASE_TPP_TOK_COMMENT_LINE
				if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
					goto again_yield_after_eof;
				break;

			default:
#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
				if (TPP_TOK_ISKEYWORD(tok) &&
				    tpp_lexer_getkeyworddefined(self, tpp_lexer_gettokenkwd(self)))
					goto do_multi_chunk_string_after_eof;
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */
				if (TPP_TOK_ISERR(tok)) {
					tpp_lexer_seek_rollback(self, &backup);
					tpp_lexer_manualpopfile_break_rollback(self);
					tpp_lexer_nowarnings_break(self);
					return TPP_TOK_ASERR(tok);
				}
				break;
			}

			/* Following token is something that could never be a (non-empty) string
			 * -> *can* decode as a single-chunk string, but then have to follow this
			 *    up by doing a (rather complicated) seek until the next (effective)
			 *    token, whilst making sure not to do too little, or too much. */
			tpp_lexer_seek_rollback(self, &backup);
			tpp_lexer_manualpopfile_end_rollback(self);
			tpp_lexer_nowarnings_break(self);
			result = tpp_lexer_decodestring_as_single_chunk(self, cb, arg);
			if (!TPP_ISERR(result)) {
				/* Yield to the next token (which shouldn't be another string) */
again_yield_after_eof_decoded:
				tok = tpp_lexer_yieldraw_blocking(self);
				switch (tok) {
				TPP_CASE_TPP_TOK_STRING
					/* Should be an empty string! */
					tpp_assert(tpp_lexer_parsestring_is_single_chunk(self) ==
					           TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_EMPTY);
					goto again_yield_after_eof_decoded;
				case TPP_TOK_SPACE:
				TPP_CASE_TPP_TOK_COMMENT_NOLINE
					if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
						goto again_yield_after_eof_decoded;
					break;
				case TPP_TOK_LF:
				TPP_CASE_TPP_TOK_COMMENT_LINE
					if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
						goto again_yield_after_eof_decoded;
					break;
				default:
					if (TPP_TOK_ISERR(tok))
						result = TPP_TOK_ASERR(tok);
					break;
				}
			}
			return result;
		}	break;
#endif /* TPP_HAVE_INCLUDE_STACK */

		default:
			if (TPP_TOK_ISERR(tok)) {
				tpp_lexer_nowarnings_break(self);
				tpp_lexer_seek_rollback(self, &backup);
				return TPP_TOK_ASERR(tok);
			}

			/* If it's a keyword that (might) expand to macro, then that macro
			 * might contain additional strings that must also be included as
			 * part of this one:
			 * >> #define str(x) #x
			 * >> "foo" str(42)
			 *          ^^^
			 *          we're here right now
			 */
#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
			if (TPP_TOK_ISKEYWORD(tok) &&
			    tpp_lexer_getkeyworddefined(self, tpp_lexer_gettokenkwd(self)))
				goto break_nowarnings_and_do_multi_chunk_string;
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */
			break;
		}
		tpp_lexer_nowarnings_pop(self);

		/* **IS** possible using a single chunk! */
		{
			/* Remember the (non-string) token that comes after the single-chunk string.
			 * After all: this is the string we want to jump back to after passing the
			 *            discovered string to our caller. */
			tpp_token *const token = tpp_lexer_gettoken(self);
			tpp_token_id final_tt_id               = token->tt_id;
			struct tpp_keyword const *final_tt_kwd = token->tt_kwd;
			tpp_char const *final_tt_start         = token->tt_start;

			/* Restore the original token containing the single-chunk string */
			tpp_lexer_seek_rollback(self, &backup);

			/* Actually give our caller the string */
			result = tpp_lexer_decodestring_as_single_chunk(self, cb, arg);
			if (!TPP_ISERR(result)) {
				/* Restore the context of the non-string token following the single-chunk'd string */
				token->tt_id    = final_tt_id;
				token->tt_kwd   = final_tt_kwd;
				token->tt_start = final_tt_start;
				token->tt_end   = pos;
			}
		}
		return result;
	} else {
		tpp_errno result;
		TPP_REF tpp_string *string;
do_multi_chunk_string:
		result = tpp_lexer_parsestring(self, &string, flags);
		if (!TPP_ISERR(result)) {
			result = (*cb)(arg, string, tpp_string_str(string), tpp_string_len(string));
			tpp_string_decref(string);
		}
		return result;
	}
	tpp_unreachable();
}
#endif /* TPP_HAVE_LEXER_DECODESTRING */

#if TPP_HAVE_BUILTIN_LEXER_PARSESTRING_EXPR
/* Convenience wrapper around `tpp_lexer_parsestring()'
 * On success (!TPP_ISERR(return)), caller must "tpp_expr_value_fini(result)"
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring_expr(tpp_lexer *tpp_restrict self,
                           /*out*/ tpp_expr_value *tpp_restrict result,
                           unsigned int flags) {
	TPP_REF tpp_string *string;
	tpp_errno error = tpp_lexer_parsestring(self, &string, flags);
	if (TPP_ISERR(error))
		return error;
	return tpp_expr_value_init_string_inherited(result, string);
}
#endif /* TPP_HAVE_BUILTIN_LEXER_PARSESTRING_EXPR */


#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
struct tpp_lexer_decodecharacter_data {
#if TPP_HAVE_TPP_W_MULTICHAR_LITERAL
	tpp_lexer *tldcd_lexer; /* [1..1] Active lexer */
	tpp_size   tldcd_count; /* # of bytes already parsed */
#endif /* TPP_HAVE_TPP_W_MULTICHAR_LITERAL */
	tpp_intmax tldcd_value; /* Multichar value */
};

static tpp_ssize TPP_FORMATPRINTER_CC
tpp_lexer_decodecharacter_cb(void *arg, tpp_char const *text, tpp_size num_bytes) {
	tpp_size i;
	struct tpp_lexer_decodecharacter_data *data;
	data = (struct tpp_lexer_decodecharacter_data *)arg;
#if TPP_HAVE_TPP_W_MULTICHAR_LITERAL
	if ((data->tldcd_count <= 1) &&
	    (data->tldcd_count + num_bytes) > 1) {
		/* Emit warning about multi-char literals being used */
		tpp_errno error = tpp_lexer_warnf(data->tldcd_lexer, TPP_W_MULTICHAR_LITERAL);
		if (TPP_ISERR(error))
			return (tpp_ssize)(int)error;
	}
	data->tldcd_count += num_bytes;
#endif /* TPP_HAVE_TPP_W_MULTICHAR_LITERAL */
	for (i = 0; i < num_bytes; ++i) {
		data->tldcd_value <<= TPP_CHAR_BIT;
		data->tldcd_value |= text[i];
	}
	return 0;
}

/* Convenience wrapper to parse a character integer literal
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsecharacter_literal(tpp_lexer *tpp_restrict self,
                                 /*out*/ tpp_intmax *tpp_restrict p_result,
                                 unsigned int flags) {
	tpp_ssize status;
	struct tpp_lexer_decodecharacter_data data;
#if TPP_HAVE_TPP_W_MULTICHAR_LITERAL
	data.tldcd_lexer = self;
	data.tldcd_count = 0;
#endif /* TPP_HAVE_TPP_W_MULTICHAR_LITERAL */
	data.tldcd_value = 0;
	status = tpp_lexer_parsestring_ex(self,
	                                  &tpp_lexer_decodecharacter_cb,
	                                  &tpp_lexer_decodecharacter_cb,
	                                  &data, flags);
	*p_result = data.tldcd_value;
	return (tpp_errno)(int)status;
}
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */


#if TPP_HAVE_BUILTIN_LEXER_PARSECHARACTER_EXPR
/* Convenience wrapper around `tpp_lexer_parsecharacter_literal()'
 * On success (!TPP_ISERR(return)), caller must "tpp_expr_value_fini(result)"
 *
 * @param: flags: Set of `TPP_LEXER_PARSECHARACTER_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsecharacter_expr(tpp_lexer *tpp_restrict self,
                              /*out*/ tpp_expr_value *tpp_restrict result,
                              unsigned int flags) {
	tpp_intmax value;
	tpp_errno error = tpp_lexer_parsecharacter_literal(self, &value, flags);
	if (TPP_ISERR(error))
		return error;
	return tpp_expr_value_init_int(result, value);
}
#endif /* TPP_HAVE_BUILTIN_LEXER_PARSECHARACTER_EXPR */


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-parseexpr.c                                        */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_BUILTIN_EXPRPARSER
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_expr(tpp_lexer *tpp_restrict self, tpp_expr_value *result);

#undef TPP_HAVE_TPP_PX_UNARY_SUFFIX
#define TPP_HAVE_TPP_PX_UNARY_SUFFIX TPP_HAVE_BUILTIN_EXPR_STRINGS
#if TPP_HAVE_TPP_PX_UNARY_SUFFIX
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_unary(tpp_lexer *tpp_restrict self, tpp_expr_value *result);
#else /* TPP_HAVE_TPP_PX_UNARY_SUFFIX */
#define tpp_px_unary(self, result) tpp_px_unary_prefix(self, result)
#endif /* !TPP_HAVE_TPP_PX_UNARY_SUFFIX */

TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_forexpr(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
	do {
		result = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(result));
	return result;
}

TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip_forexpr(tpp_lexer *tpp_restrict self, tpp_token_id id) {
	tpp_token_id result = tpp_lexer_skip(self, id);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(result))
		result = tpp_lexer_yield_blocking(self);
	return result;
}

/************************************************************************/
/* LEVEL #0 : UNARY PREFIX                                              */
/************************************************************************/
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_unary_prefix(tpp_lexer *tpp_restrict self, tpp_expr_value *result) {
	tpp_token_id tok = tpp_lexer_gettok(self);
again:
	switch (tpp_lexer_gettok(self)) {
	case TPP_TOK_SPACE:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT
		/* Skip over whitespace */
		tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		goto again;

	case '(': {
		tpp_errno error;
		tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		error = tpp_px_expr(self, result);
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_skip_forexpr(self, TPP_TOK_OFCHAR(')'));
		return TPP_TOK_ASERR_OR_EOK(tok);
	}	break;

#if (TPP_HAVE_TPP_TOK_MINUS_MINUS || \
     TPP_HAVE_TPP_TOK_PLUS_PLUS ||   \
     TPP_HAVE_TPP_TOK_TILDE_TILDE || \
     TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM)
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
	case TPP_TOK_MINUS_MINUS:
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
	case TPP_TOK_PLUS_PLUS:
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
	case TPP_TOK_TILDE_TILDE:
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	case TPP_TOK_EXCLAIM_EXCLAIM:
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
	{
		/* Handling for multi-char tokens:  --  ++  ~~  !! */
		tpp_token *const token = tpp_lexer_gettoken(self);
		token->tt_end = token->tt_start + 1;
		token->tt_id = tok = TPP_TOK_OFCHAR(*token->tt_start);
	}	TPP_FALLTHRU
#endif /* ... */
	case '!':
	case '+':
	case '-':
	case '~': {
		tpp_errno error;
		tpp_token_id const op = tok;
		tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (!result)
			return tpp_px_unary(self, NULL);
		error = tpp_px_unary(self, result);
		if (!TPP_ISERR(error)) {
			tpp_expr_value new_result;
			switch (op) {
			case '+': error = tpp_expr_value_pos(self, result, &new_result); break;
			case '-': error = tpp_expr_value_neg(self, result, &new_result); break;
			case '~': error = tpp_expr_value_inv(self, result, &new_result); break;
			case '!': {
				bool b_value;
				error = tpp_expr_value_asbool(self, result, &b_value);
				if (TPP_ISERR(error))
					return error;
				error = tpp_expr_value_init_bool(&new_result, !b_value);
			}	break;
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(result);
			if (!TPP_ISERR(error))
				tpp_expr_value_move(result, &new_result);
		}
		return error;
	}	break;

#if TPP_HAVE_TPP_TOK_INT
	case TPP_TOK_INT: {
		tpp_errno error;
		if (result == NULL)
			break;
		error = tpp_lexer_decodeint_expr(self, result);
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_TPP_TOK_INT */

#if TPP_HAVE_BUILTIN_EXPR_FLOATS && TPP_HAVE_TPP_TOK_FLOAT
	case TPP_TOK_FLOAT: {
		tpp_errno error;
		if (result == NULL)
			break;
		error = tpp_lexer_decodefloat_expr(self, result);
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS && TPP_HAVE_TPP_TOK_FLOAT */

#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS != 0
	TPP_CASE_TPP_TOK_STRING_SQUOTE
		if (tpp_lexer_has(self, BUILTIN_EXPR_CHARACTER_LITERALS)) {
			if (result)
				return tpp_lexer_parsecharacter_expr(self, result, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
			do {
				tok = tpp_lexer_yield_forexpr(self);
			} while (TPP_TOK_ISSTRING(tok));
			return TPP_TOK_ASERR_OR_EOK(tok);
		}
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		TPP_FALLTHRU
#else /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		break;
#endif /* !TPP_HAVE_BUILTIN_EXPR_STRINGS */
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS != 0 */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS == 0
	TPP_CASE_TPP_TOK_STRING_SQUOTE
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS == 0 */
	TPP_CASE_TPP_TOK_STRING_DQUOTE
		if (!tpp_lexer_has(self, BUILTIN_EXPR_STRINGS))
			break;
		if (result)
			return tpp_lexer_parsestring_expr(self, result, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		do {
			tok = tpp_lexer_yield_forexpr(self);
		} while (TPP_TOK_ISSTRING(tok));
		return TPP_TOK_ASERR_OR_EOK(tok);
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */


#if TPP_HAVE_BUILTIN_EXPR_STRINGS || TPP_HAVE_CPP_ASSERT
	case '#':
		/* Preprocessor assertions */
#if TPP_HAVE_CPP_ASSERT
		if (tpp_lexer_has(self, CPP_ASSERT)) {
			/* TODO */
		}
#endif /* TPP_HAVE_CPP_ASSERT */

		/* length-operator for string expressions */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		if (tpp_lexer_has(self, BUILTIN_EXPR_STRINGS)) {
			/* TODO */
		}
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		goto handle_default;
#define WANT_handle_default
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS || TPP_HAVE_CPP_ASSERT */


#if TPP_HAVE_BUILTIN_EXPR_DEFINED
	case TPP_KWD_defined: {
		bool is_defined;
		bool has_paren;
		if (!tpp_lexer_has(self, BUILTIN_EXPR_DEFINED))
			goto handle_default;
#define WANT_handle_default
		tok = tpp_lexer_yield_forexpr(self);
		has_paren = tok == '(';
		if (has_paren)
			tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (TPP_TOK_ISKEYWORD(tok)) {
			is_defined = tpp_lexer_getkeyworddefined(self, tpp_lexer_gettokenkwd(self));
			tok = tpp_lexer_yield_forexpr(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		} else {
#if TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED);
			if (TPP_ISERR(error))
				return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED */
			is_defined = false;
			/* Be smart about stuff that should still be consumed here */
#if TPP_HAVE_TPP_TOK_STRINGLIKE || TPP_HAVE_TPP_TOK_INT || TPP_HAVE_TPP_TOK_FLOAT
			switch (tok) {
			TPP_CASE_TPP_TOK_STRING
#if TPP_HAVE_TPP_TOK_INT
			case TPP_TOK_INT:
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
			case TPP_TOK_FLOAT:
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
				tok = tpp_lexer_yield_forexpr(self);
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
				break;
			default: break;
			}
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE || TPP_HAVE_TPP_TOK_INT || TPP_HAVE_TPP_TOK_FLOAT */
		}
		if (has_paren) {
			tok = tpp_lexer_skip_forexpr(self, TPP_TOK_OFCHAR(')'));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
		if (result)
			return tpp_expr_value_init_bool(result, is_defined);
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_DEFINED */

#if TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
	case TPP_KWD_if: {
		tpp_errno error;
		bool is_true;
		if (!tpp_lexer_has(self, BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS))
			goto handle_default;
#define WANT_handle_default
again_handle_if:
		tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('(')); /* Doesn't have to be "tpp_lexer_skip_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		error = tpp_px_expr(self, result);
		if (TPP_ISERR(error))
			return error;
		is_true = false;
		if (result) {
			error = tpp_expr_value_asbool(self, result, &is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
		}
		tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')')); /* Doesn't have to be "tpp_lexer_skip_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		error = tpp_px_expr(self, is_true ? result : NULL);
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_gettok(self);
		if (tok == TPP_KWD_elif) {
			if (is_true /*&& result*/) {
				tpp_lexer_gettoken(self)->tt_id = TPP_KWD_if;
				error = tpp_px_unary_prefix(self, NULL);
				if (TPP_ISERR(error))
					tpp_expr_value_fini(result);
				return error;
			}
			goto again_handle_if;
		} else if (tok == TPP_KWD_else) {
			tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
			if (TPP_TOK_ISERR(tok)) {
				if (is_true /*&& result*/)
					tpp_expr_value_fini(result);
				return TPP_TOK_ASERR(tok);
			}
			if (is_true /*&& result*/) {
				error = tpp_px_expr(self, NULL);
				if (TPP_ISERR(error))
					tpp_expr_value_fini(result);
				return error;
			}
			return tpp_px_expr(self, result);
		} else {
			if (!is_true && result)
				return tpp_expr_value_init_zero(result);
			return TPP_EOK;
		}
		tpp_unreachable();
	}	break;
#endif /* !TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */

#ifdef WANT_handle_default
#undef WANT_handle_default
handle_default:
#endif /* WANT_handle_default */
	default: {
		tpp_errno error = TPP_EOK;
		if (TPP_TOK_ISKEYWORD(tok)) {
#if TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION
			if (result != NULL)
				error = tpp_lexer_warnf(self, TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION);
#endif /* TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION */
		} else {
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION
			error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION);
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION */
		}
		if (TPP_ISERR(error))
			return error;
	}	break;

	}

	/* Skip over token and set result to "0" */
	tok = tpp_lexer_yield_forexpr(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (result == NULL)
		return TPP_EOK;
	return tpp_expr_value_init_zero(result);
}

#define TPP_DEFINE_PX_PARSER(tpp_px_prev, tpp_px_this, tpp_px_this_suffix, TPP_TEST_PX_THIS_SUFFIX) \
	static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL                              \
	tpp_px_this(tpp_lexer *tpp_restrict self, tpp_expr_value *result) {                             \
		tpp_errno error = tpp_px_prev(self, result);                                                \
		if (!TPP_ISERR(error) && TPP_TEST_PX_THIS_SUFFIX(tpp_lexer_gettok(self)))                   \
			error = tpp_px_this_suffix(self, result);                                               \
		return error;                                                                               \
	}


/************************************************************************/
/* LEVEL #1 : UNARY SUFFIX                                              */
/************************************************************************/
#if TPP_HAVE_TPP_PX_UNARY_SUFFIX
#define TPP_CASE_PX_UNARY_SUFFIX      case '[':
#define TPP_TEST_PX_UNARY_SUFFIX(tok) ((tok) == '[')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_unary_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_token_id tok;
	tpp_assert(TPP_TEST_PX_UNARY_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok))
			goto err_r_tok;
		if (tok == ':') {
			tok = tpp_lexer_yield_forexpr(self);
			if (TPP_TOK_ISERR(tok))
				goto err_r_tok;
			if (tok == ']') {
				/* Full range */
				if (result) {
					tpp_expr_value new_result;
					error = tpp_expr_value_getrange(self, result, NULL, NULL, &new_result);
					tpp_expr_value_fini(result);
					if (TPP_ISERR(error))
						return error;
					tpp_expr_value_move(result, &new_result);
				}
			} else {
				/* hi-only range */
				if (result) {
					tpp_expr_value hi, new_result;
					error = tpp_px_expr(self, &hi);
					if (TPP_ISERR(error))
						goto err_r;
					error = tpp_expr_value_getrange(self, result, NULL, &hi, &new_result);
					tpp_expr_value_fini(&hi);
					tpp_expr_value_fini(result);
					if (TPP_ISERR(error))
						return error;
					tpp_expr_value_move(result, &new_result);
				} else {
					error = tpp_px_expr(self, NULL);
					if (TPP_ISERR(error))
						return error;
				}
			}
		} else if (result) {
			tpp_expr_value index, new_result;
			error = tpp_px_expr(self, &index);
			if (TPP_ISERR(error))
				goto err_r_tok;
			tok = tpp_lexer_gettok(self);
			while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
				tok = tpp_lexer_yield_blocking(self);
				if (TPP_TOK_ISERR(tok)) {
err_r_tok_index:
					tpp_expr_value_fini(&index);
					goto err_r_tok;
				}
			}
			if (tok == ':') {
				tok = tpp_lexer_yield_forexpr(self);
				if (TPP_TOK_ISERR(tok))
					goto err_r_tok_index;
				if (tok == ']') {
					error = tpp_expr_value_getrange(self, result, &index, NULL, &new_result);
				} else {
					tpp_expr_value hi;
					error = tpp_px_expr(self, &hi);
					if (TPP_ISERR(error)) {
						tpp_expr_value_fini(&index);
						goto err_r;
					}
					error = tpp_expr_value_getrange(self, result, &index, &hi, &new_result);
					tpp_expr_value_fini(&hi);
				}
			} else {
				error = tpp_expr_value_getindex(self, result, &index, &new_result);
			}
			tpp_expr_value_fini(&index);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_expr(self, NULL);
			if (TPP_ISERR(error))
				return error;
			tok = tpp_lexer_gettok(self);
			while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
				tok = tpp_lexer_yield_blocking(self);
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
			}
			if (tok == ':') {
				tok = tpp_lexer_yield_forexpr(self);
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
			}
		}
		tok = tpp_lexer_gettok(self);
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			tok = tpp_lexer_yield_blocking(self);
			if (TPP_TOK_ISERR(tok))
				goto err_r_tok;
		}
		tok = tpp_lexer_skip_forexpr(self, TPP_TOK_OFCHAR(']'));
		if (TPP_TOK_ISERR(tok))
			goto err_r_tok;
	} while (TPP_TEST_PX_UNARY_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r_tok:
	error = TPP_TOK_ASERR(tok);
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}

TPP_DEFINE_PX_PARSER(tpp_px_unary_prefix, tpp_px_unary, tpp_px_unary_suffix, TPP_TEST_PX_UNARY_SUFFIX)
#endif /* TPP_HAVE_TPP_PX_UNARY_SUFFIX */



/************************************************************************/
/* LEVEL #2 : PRODUCT                                                   */
/************************************************************************/
#define TPP_CASE_PX_PROD_SUFFIX      case '*': case '/': case '%':
#define TPP_TEST_PX_PROD_SUFFIX(tok) ((tok) == '*' || (tok) == '/' || (tok) == '%')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_prod_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_PROD_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_unary(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
			case '*': error = tpp_expr_value_mul(self, result, &rhs, &new_result); break;
			case '/': error = tpp_expr_value_div(self, result, &rhs, &new_result); break;
			case '%': error = tpp_expr_value_mod(self, result, &rhs, &new_result); break;
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_unary(self, NULL);
			if (TPP_ISERR(error))
				goto err_r;
		}
	} while (TPP_TEST_PX_PROD_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_unary, tpp_px_prod, tpp_px_prod_suffix, TPP_TEST_PX_PROD_SUFFIX)


/************************************************************************/
/* LEVEL #3 : SUM                                                       */
/************************************************************************/
#define TPP_CASE_PX_SUM_SUFFIX      case '+': case '-':
#define TPP_TEST_PX_SUM_SUFFIX(tok) ((tok) == '+' || (tok) == '-')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_sum_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_SUM_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_prod(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
			case '+': error = tpp_expr_value_add(self, result, &rhs, &new_result); break;
			case '-': error = tpp_expr_value_sub(self, result, &rhs, &new_result); break;
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_prod(self, NULL);
			if (TPP_ISERR(error))
				goto err_r;
		}
	} while (TPP_TEST_PX_SUM_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_prod, tpp_px_sum, tpp_px_sum_suffix, TPP_TEST_PX_SUM_SUFFIX)


/************************************************************************/
/* LEVEL #4 : SHIFT                                                     */
/************************************************************************/
#undef TPP_HAVE_PX_SHIFT_SUFFIX
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_HAVE_PX_SHIFT_SUFFIX 1
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_CASE_PX_SHIFT_SUFFIX \
	case TPP_TOK_LANGLE_LANGLE:  \
	case TPP_TOK_RANGLE_RANGLE:
#define TPP_TEST_PX_SHIFT_SUFFIX(tok) ((tok) == TPP_TOK_LANGLE_LANGLE || (tok) == TPP_TOK_RANGLE_RANGLE)
#elif TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#define TPP_CASE_PX_SHIFT_SUFFIX      case TPP_TOK_LANGLE_LANGLE:
#define TPP_TEST_PX_SHIFT_SUFFIX(tok) ((tok) == TPP_TOK_LANGLE_LANGLE)
#else /* ... */
#define TPP_CASE_PX_SHIFT_SUFFIX      case TPP_TOK_RANGLE_RANGLE:
#define TPP_TEST_PX_SHIFT_SUFFIX(tok) ((tok) == TPP_TOK_RANGLE_RANGLE)
#endif /* !... */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_shift_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_SHIFT_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_sum(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
			case TPP_TOK_LANGLE_LANGLE: error = tpp_expr_value_shl(self, result, &rhs, &new_result); break;
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
			case TPP_TOK_RANGLE_RANGLE: error = tpp_expr_value_shr(self, result, &rhs, &new_result); break;
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_sum(self, NULL);
			if (TPP_ISERR(error))
				return error;
		}
	} while (TPP_TEST_PX_SHIFT_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_sum, tpp_px_shift, tpp_px_shift_suffix, TPP_TEST_PX_SHIFT_SUFFIX)
#else /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#define TPP_HAVE_PX_SHIFT_SUFFIX          0
#define TPP_CASE_PX_SHIFT_SUFFIX          /* nothing */
#define TPP_TEST_PX_SHIFT_SUFFIX(tok)     0
#define tpp_px_shift_suffix(self, result) TPP_EOK
#define tpp_px_shift(self, result)        tpp_px_sum(self, result)
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE && !TPP_HAVE_TPP_TOK_RANGLE_RANGLE */


/************************************************************************/
/* LEVEL #5 : CMP                                                       */
/************************************************************************/
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL && TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TPP_CASE_PX_CMP_SUFFIX_EXTRA \
	case TPP_TOK_LANGLE_EQUAL:       \
	case TPP_TOK_RANGLE_EQUAL:
#define TPP_TEST_PX_CMP_SUFFIX_EXTRA(tok) ((tok) == TPP_TOK_LANGLE_EQUAL || (tok) == TPP_TOK_RANGLE_EQUAL)
#elif TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TPP_CASE_PX_CMP_SUFFIX_EXTRA      case TPP_TOK_LANGLE_EQUAL:
#define TPP_TEST_PX_CMP_SUFFIX_EXTRA(tok) ((tok) == TPP_TOK_LANGLE_EQUAL)
#else /* ... */
#define TPP_CASE_PX_CMP_SUFFIX_EXTRA      case TPP_TOK_RANGLE_EQUAL:
#define TPP_TEST_PX_CMP_SUFFIX_EXTRA(tok) ((tok) == TPP_TOK_RANGLE_EQUAL)
#endif /* !... */
#define TPP_CASE_PX_CMP_SUFFIX      case '<': case '>': TPP_CASE_PX_CMP_SUFFIX_EXTRA
#define TPP_TEST_PX_CMP_SUFFIX(tok) ((tok) == '<' || (tok) == '>' || TPP_TEST_PX_CMP_SUFFIX_EXTRA(tok))
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_cmp_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_CMP_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			bool cmp_result;
			tpp_expr_value rhs;
			error = tpp_px_shift(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
			case '<': error = tpp_expr_value_cmp_lo(self, result, &rhs, &cmp_result); break;
			case '>': error = tpp_expr_value_cmp_gr(self, result, &rhs, &cmp_result); break;
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
			case TPP_TOK_LANGLE_EQUAL: error = tpp_expr_value_cmp_le(self, result, &rhs, &cmp_result); break;
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
			case TPP_TOK_RANGLE_EQUAL: error = tpp_expr_value_cmp_ge(self, result, &rhs, &cmp_result); break;
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_expr_value_init_bool(result, cmp_result);
		} else {
			error = tpp_px_shift(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_CMP_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_shift, tpp_px_cmp, tpp_px_cmp_suffix, TPP_TEST_PX_CMP_SUFFIX)


/************************************************************************/
/* LEVEL #6 : CMPEQ                                                     */
/************************************************************************/
#undef TPP_HAVE_PX_CMPEQ_SUFFIX
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#define TPP_HAVE_PX_CMPEQ_SUFFIX 1
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#define TPP_CASE_PX_CMPEQ_SUFFIX \
	case TPP_TOK_EQUAL_EQUAL:    \
	case TPP_TOK_EXCLAIM_EQUAL:
#define TPP_TEST_PX_CMPEQ_SUFFIX(tok) ((tok) == TPP_TOK_EQUAL_EQUAL || (tok) == TPP_TOK_EXCLAIM_EQUAL)
#elif TPP_HAVE_TPP_TOK_EQUAL_EQUAL
#define TPP_CASE_PX_CMPEQ_SUFFIX      case TPP_TOK_EQUAL_EQUAL:
#define TPP_TEST_PX_CMPEQ_SUFFIX(tok) ((tok) == TPP_TOK_EQUAL_EQUAL)
#else /* ... */
#define TPP_CASE_PX_CMPEQ_SUFFIX      case TPP_TOK_EXCLAIM_EQUAL:
#define TPP_TEST_PX_CMPEQ_SUFFIX(tok) ((tok) == TPP_TOK_EXCLAIM_EQUAL)
#endif /* !... */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_cmpeq_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_CMPEQ_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			bool cmp_result;
			tpp_expr_value rhs;
			error = tpp_px_cmp(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
			case TPP_TOK_EQUAL_EQUAL: error = tpp_expr_value_cmp_eq(self, result, &rhs, &cmp_result); break;
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
			case TPP_TOK_EXCLAIM_EQUAL: error = tpp_expr_value_cmp_ne(self, result, &rhs, &cmp_result); break;
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_expr_value_init_bool(result, cmp_result);
		} else {
			error = tpp_px_cmp(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_CMPEQ_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_cmp, tpp_px_cmpeq, tpp_px_cmpeq_suffix, TPP_TEST_PX_CMPEQ_SUFFIX)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#define TPP_HAVE_PX_CMPEQ_SUFFIX          0
#define TPP_CASE_PX_CMPEQ_SUFFIX          /* nothing */
#define TPP_TEST_PX_CMPEQ_SUFFIX(tok)     0
#define tpp_px_cmpeq_suffix(self, result) TPP_EOK
#define tpp_px_cmpeq(self, result)        tpp_px_cmp(self, result)
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL && !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */


/************************************************************************/
/* LEVEL #7 : AND                                                       */
/************************************************************************/
#define TPP_CASE_PX_AND_SUFFIX      case '&':
#define TPP_TEST_PX_AND_SUFFIX(tok) ((tok) == '&')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_and_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_AND_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_cmpeq(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			error = tpp_expr_value_and(self, result, &rhs, &new_result);
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_cmpeq(self, NULL);
			if (TPP_ISERR(error))
				return error;
		}
	} while (TPP_TEST_PX_AND_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_cmpeq, tpp_px_and, tpp_px_and_suffix, TPP_TEST_PX_AND_SUFFIX)


/************************************************************************/
/* LEVEL #8 : XOR                                                       */
/************************************************************************/
#define TPP_CASE_PX_XOR_SUFFIX      case '^':
#define TPP_TEST_PX_XOR_SUFFIX(tok) ((tok) == '^')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_xor_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_XOR_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_and(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			error = tpp_expr_value_xor(self, result, &rhs, &new_result);
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_and(self, NULL);
			if (TPP_ISERR(error))
				return error;
		}
	} while (TPP_TEST_PX_XOR_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_and, tpp_px_xor, tpp_px_xor_suffix, TPP_TEST_PX_XOR_SUFFIX)


/************************************************************************/
/* LEVEL #9 : OR                                                        */
/************************************************************************/
#define TPP_CASE_PX_OR_SUFFIX      case '|':
#define TPP_TEST_PX_OR_SUFFIX(tok) ((tok) == '|')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_or_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_OR_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_xor(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			error = tpp_expr_value_or(self, result, &rhs, &new_result);
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_xor(self, NULL);
			if (TPP_ISERR(error))
				return error;
		}
	} while (TPP_TEST_PX_OR_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_xor, tpp_px_or, tpp_px_or_suffix, TPP_TEST_PX_OR_SUFFIX)


/************************************************************************/
/* LEVEL #10 : LAND                                                     */
/************************************************************************/
#undef TPP_HAVE_PX_LAND_SUFFIX
#define TPP_HAVE_PX_LAND_SUFFIX 1
#if TPP_HAVE_TPP_TOK_AMP_AMP
#define TPP_CASE_PX_LAND_SUFFIX      case TPP_TOK_AMP_AMP:
#define TPP_TEST_PX_LAND_SUFFIX(tok) ((tok) == TPP_TOK_AMP_AMP)
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_land_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_assert(TPP_TEST_PX_LAND_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_errno error;
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			if (result)
				tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		if (result) {
			bool is_true;
			error = tpp_expr_value_asbool(self, result, &is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_px_or(self, is_true ? result : NULL);
			if (TPP_ISERR(error))
				return error;
			if (is_true) {
				error = tpp_expr_value_asbool(self, result, &is_true);
				tpp_expr_value_fini(result);
				if (TPP_ISERR(error))
					return error;
			}
			error = tpp_expr_value_init_bool(result, is_true);
		} else {
			error = tpp_px_or(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_LAND_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
}
TPP_DEFINE_PX_PARSER(tpp_px_or, tpp_px_land, tpp_px_land_suffix, TPP_TEST_PX_LAND_SUFFIX)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#define TPP_HAVE_PX_LAND_SUFFIX          0
#define TPP_CASE_PX_LAND_SUFFIX          /* nothing */
#define TPP_TEST_PX_LAND_SUFFIX(tok)     0
#define tpp_px_land_suffix(self, result) TPP_EOK
#define tpp_px_land(self, result)        tpp_px_or(self, result)
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL && !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */


/************************************************************************/
/* LEVEL #11 : LXOR                                                     */
/************************************************************************/
#undef TPP_HAVE_PX_LXOR_SUFFIX
#define TPP_HAVE_PX_LXOR_SUFFIX 1
#if TPP_HAVE_TPP_TOK_HAT_HAT
#define TPP_CASE_PX_LXOR_SUFFIX      case TPP_TOK_HAT_HAT:
#define TPP_TEST_PX_LXOR_SUFFIX(tok) ((tok) == TPP_TOK_HAT_HAT)
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_lxor_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_assert(TPP_TEST_PX_LXOR_SUFFIX(tpp_lexer_gettok(self)));
	if (!tpp_lexer_has(self, BUILTIN_EXPR_LOGICAL_XOR))
		return TPP_EOK;
	do {
		tpp_errno error;
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			if (result)
				tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		if (result) {
			bool lhs_is_true, rhs_is_true;
			error = tpp_expr_value_asbool(self, result, &lhs_is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_px_land(self, result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_expr_value_asbool(self, result, &rhs_is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_expr_value_init_bool(result, lhs_is_true ^ rhs_is_true);
		} else {
			error = tpp_px_land(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_LXOR_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
}
TPP_DEFINE_PX_PARSER(tpp_px_land, tpp_px_lxor, tpp_px_lxor_suffix, TPP_TEST_PX_LXOR_SUFFIX)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#define TPP_HAVE_PX_LXOR_SUFFIX          0
#define TPP_CASE_PX_LXOR_SUFFIX          /* nothing */
#define TPP_TEST_PX_LXOR_SUFFIX(tok)     0
#define tpp_px_lxor_suffix(self, result) TPP_EOK
#define tpp_px_lxor(self, result)        tpp_px_land(self, result)
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL && !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */


/************************************************************************/
/* LEVEL #12 : LOR                                                      */
/************************************************************************/
#undef TPP_HAVE_PX_LOR_SUFFIX
#define TPP_HAVE_PX_LOR_SUFFIX 1
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
#define TPP_CASE_PX_LOR_SUFFIX      case TPP_TOK_PIPE_PIPE:
#define TPP_TEST_PX_LOR_SUFFIX(tok) ((tok) == TPP_TOK_PIPE_PIPE)
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_lor_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_assert(TPP_TEST_PX_LOR_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_errno error;
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			if (result)
				tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		if (result) {
			bool is_true;
			error = tpp_expr_value_asbool(self, result, &is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_px_lxor(self, is_true ? NULL : result);
			if (TPP_ISERR(error))
				return error;
			if (!is_true) {
				error = tpp_expr_value_asbool(self, result, &is_true);
				tpp_expr_value_fini(result);
				if (TPP_ISERR(error))
					return error;
			}
			error = tpp_expr_value_init_bool(result, is_true);
		} else {
			error = tpp_px_lxor(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_LOR_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
}
TPP_DEFINE_PX_PARSER(tpp_px_lxor, tpp_px_lor, tpp_px_lor_suffix, TPP_TEST_PX_LOR_SUFFIX)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#define TPP_HAVE_PX_LOR_SUFFIX          0
#define TPP_CASE_PX_LOR_SUFFIX          /* nothing */
#define TPP_TEST_PX_LOR_SUFFIX(tok)     0
#define tpp_px_lor_suffix(self, result) TPP_EOK
#define tpp_px_lor(self, result)        tpp_px_lxor(self, result)
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL && !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */


/************************************************************************/
/* LEVEL #13 : QUESTION                                                 */
/************************************************************************/
#define TPP_CASE_PX_QUESTION_SUFFIX      case '?':
#define TPP_TEST_PX_QUESTION_SUFFIX(tok) ((tok) == '?')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_question_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_token_id tok;
	tpp_assert(TPP_TEST_PX_QUESTION_SUFFIX(tpp_lexer_gettok(self)));
	tok = tpp_lexer_yield_forexpr(self);
	if (TPP_TOK_ISERR(tok))
		goto err_result_tok;
	if (result) {
		bool cond_is_true;
		error = tpp_expr_value_asbool(self, result, &cond_is_true);
		if (TPP_ISERR(error))
			goto err_result;
		tok = tpp_lexer_gettok(self);
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			tok = tpp_lexer_yield_blocking(self);
			if (TPP_TOK_ISERR(tok))
				goto err_result_tok;
		}
#if TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
		if (tok == ':' && tpp_lexer_has(self, BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)) {
			tok = tpp_lexer_yield_forexpr(self);
			if (TPP_TOK_ISERR(tok))
				goto err_result_tok;
			if (cond_is_true) {
				error = tpp_px_expr(self, NULL);
				if (TPP_ISERR(error))
					tpp_expr_value_fini(result);
				return error;
			}
			tpp_expr_value_fini(result);
			return tpp_px_expr(self, result);
		} else
#endif /* TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT */
		{
			tpp_expr_value_fini(result);
			error = tpp_px_lor(self, cond_is_true ? result : NULL);
			if (TPP_ISERR(error))
				return error;
			tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(':')); /* Doesn't have to be "tpp_lexer_skip_forexpr" */
			if (TPP_TOK_ISERR(tok)) {
				if (cond_is_true)
					tpp_expr_value_fini(result);
				return TPP_TOK_ASERR(tok);
			}
			if (cond_is_true) {
				error = tpp_px_expr(self, NULL);
				if (TPP_ISERR(error))
					tpp_expr_value_fini(result);
				return error;
			}
			return tpp_px_expr(self, result);
		}
	} else {
		if (tok != ':') {
			error = tpp_px_lor(self, NULL);
			if (TPP_ISERR(error))
				return error;
			tok = tpp_lexer_gettok(self);
		}
		if (tok == ':') {
			tok = tpp_lexer_yield_forexpr(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			return tpp_px_expr(self, NULL);
		}
	}
	return TPP_EOK;
err_result_tok:
	error = TPP_TOK_ASERR(tok);
err_result:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}






static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_expr(tpp_lexer *tpp_restrict self, tpp_expr_value *result) {
	tpp_token_id tok;
	tpp_errno error;

	/* Evaluate expression prefix. */
	error = tpp_px_unary_prefix(self, result);
	if (TPP_ISERR(error))
		return error;

	/* Evaluate expression suffix. */
	tok = tpp_lexer_gettok(self);
	switch (tok) {

#if TPP_HAVE_TPP_PX_UNARY_SUFFIX
		/*if (TPP_TEST_PX_UNARY_SUFFIX(tok))*/ {
	TPP_CASE_PX_UNARY_SUFFIX
			error = tpp_px_unary_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
		if (TPP_TEST_PX_PROD_SUFFIX(tok))
#endif /* TPP_HAVE_TPP_PX_UNARY_SUFFIX */
		{
	TPP_CASE_PX_PROD_SUFFIX
			error = tpp_px_prod_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
		if (TPP_TEST_PX_SUM_SUFFIX(tok)) {
	TPP_CASE_PX_SUM_SUFFIX
			error = tpp_px_sum_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#if TPP_HAVE_PX_SHIFT_SUFFIX
		if (TPP_TEST_PX_SHIFT_SUFFIX(tok)) {
	TPP_CASE_PX_SHIFT_SUFFIX
			error = tpp_px_shift_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_SHIFT_SUFFIX */
		if (TPP_TEST_PX_CMP_SUFFIX(tok)) {
	TPP_CASE_PX_CMP_SUFFIX
			error = tpp_px_cmp_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#if TPP_HAVE_PX_CMPEQ_SUFFIX
		if (TPP_TEST_PX_CMPEQ_SUFFIX(tok)) {
	TPP_CASE_PX_CMPEQ_SUFFIX
			error = tpp_px_cmpeq_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_CMPEQ_SUFFIX */
		if (TPP_TEST_PX_AND_SUFFIX(tok)) {
	TPP_CASE_PX_AND_SUFFIX
			error = tpp_px_and_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
		if (TPP_TEST_PX_XOR_SUFFIX(tok)) {
	TPP_CASE_PX_XOR_SUFFIX
			error = tpp_px_xor_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
		if (TPP_TEST_PX_OR_SUFFIX(tok)) {
	TPP_CASE_PX_OR_SUFFIX
			error = tpp_px_or_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#if TPP_HAVE_PX_LAND_SUFFIX
		if (TPP_TEST_PX_LAND_SUFFIX(tok)) {
	TPP_CASE_PX_LAND_SUFFIX
			error = tpp_px_land_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_LAND_SUFFIX */
#if TPP_HAVE_PX_LXOR_SUFFIX
		if (TPP_TEST_PX_LXOR_SUFFIX(tok)) {
	TPP_CASE_PX_LXOR_SUFFIX
			error = tpp_px_lxor_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_LXOR_SUFFIX */
#if TPP_HAVE_PX_LOR_SUFFIX
		if (TPP_TEST_PX_LOR_SUFFIX(tok)) {
	TPP_CASE_PX_LOR_SUFFIX
			error = tpp_px_lor_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_LOR_SUFFIX */
		if (TPP_TEST_PX_QUESTION_SUFFIX(tok)) {
	TPP_CASE_PX_QUESTION_SUFFIX
#if 1
			return tpp_px_question_suffix(self, result);
#else
			error = tpp_px_question_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
#endif
		}
		break;
	default: break;
	}
	return error;
}

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_builtin_parseexpr(tpp_lexer *tpp_restrict self,
                      tpp_expr_value *tpp_restrict result) {
	tpp_token_id tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	return tpp_px_expr(self, result);
}


#if TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_lexer_builtin_parseexpr(tpp_lexer *tpp_restrict self,
                             tpp_expr_value *tpp_restrict result) {
	return tpp_builtin_parseexpr(self, result);
}
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR */

#if TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR_WITH_ARG
TPP_IMPL TPP_WUNUSED TPP_NONNULL((2, 3)) tpp_errno TPPCALL
_tpp_lexer_builtin_parseexpr_with_arg(void *arg, tpp_lexer *tpp_restrict self,
                                      tpp_expr_value *tpp_restrict result) {
	(void)arg;
	return tpp_builtin_parseexpr(self, result);
}
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR_WITH_ARG */
#endif /* TPP_HAVE_BUILTIN_EXPRPARSER */

TPP_DECL_END
/************************************************************************/


#endif /* !GUARD_TPP_C */
/* clang-format on */
