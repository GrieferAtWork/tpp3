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
/*[[[deemon
for (local i = 0; i < 256; ++i) {
	if ((i % 16) == 0)
		print "\t",;
	local flags = 0;
	if ((i >= 'a'.ord() && i <= 'z'.ord()) ||
	    (i >= 'A'.ord() && i <= 'Z'.ord()) ||
	    i == '_'.ord() || i == '$'.ord())
		flags |= _TPP_CTYPE_ISSYMSTRT | _TPP_CTYPE_ISSYMCONT;
	if (i >= '0'.ord() && i <= '9'.ord())
		flags |= _TPP_CTYPE_ISSYMCONT | _TPP_CTYPE_ISDIGIT;
	if (i >= 0x80)
		flags |= _TPP_CTYPE_ISNONASCII;
	if (i in ['\r'.ord(), '\n'.ord()]) {
		flags |= _TPP_CTYPE_ISLF;
	} else if (i <= 32) {
		flags |= _TPP_CTYPE_ISSPACE;
	}
	print "0x%.2x," % flags,;
	if ((i % 16) == 15)
		print;
}
]]]*/
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
//[[[end]]]
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

	tpp_assert(buffer);
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
	char ttr_TPP_TOK_EQUAL[6];
	char ttr_TPP_TOK_AT[3];
	char ttr_TPP_TOK_BACKSLASH[10];
	char ttr_TPP_TOK_DOT[4];
	char ttr_TPP_TOK_COMMA[6];
	char ttr_TPP_TOK_COLON[6];
	char ttr_TPP_TOK_POUND[6];
	char ttr_TPP_TOK_LANGLE[7];
	char ttr_TPP_TOK_RANGLE[7];
	char ttr_TPP_TOK_LPAREN[7];
	char ttr_TPP_TOK_RPAREN[7];
	char ttr_TPP_TOK_LBRACKET[9];
	char ttr_TPP_TOK_RBRACKET[9];
	char ttr_TPP_TOK_LBRACE[7];
	char ttr_TPP_TOK_RBRACE[7];
	char ttr_TPP_TOK_PERCENT[8];
	char ttr_TPP_TOK_EXCLAIM[8];
	char ttr_TPP_TOK_AMP[4];
	char ttr_TPP_TOK_PIPE[5];
	char ttr_TPP_TOK_PLUS[5];
	char ttr_TPP_TOK_MINUS[6];
	char ttr_TPP_TOK_STAR[5];
	char ttr_TPP_TOK_SLASH[6];
	char ttr_TPP_TOK_QMARK[6];
	char ttr_TPP_TOK_SEMICOLON[10];
	char ttr_TPP_TOK_TILDE[6];
	char ttr_TPP_TOK_HAT[4];
#if TPP_HAVE_TPP_TOK_DOLLAR
	char ttr_TPP_TOK_DOLLAR[7];
#endif /* TPP_HAVE_TPP_TOK_DOLLAR */
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	char ttr_TPP_TOK_CHAR[5];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	char ttr_TPP_TOK_RAW_CHAR_LITERAL[17];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	char ttr_TPP_TOK_BLOCK_STRING_LITERAL[21];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
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
} const tpp_token_str_strings = {
	/* .ttr_TPP_TOK_EOF = */ "EOF",
	/* .ttr_TPP_TOK_LF = */ "LF",
	/* .ttr_TPP_TOK_SPACE = */ "SPACE",
	/* .ttr_TPP_TOK_EQUAL = */ "EQUAL",
	/* .ttr_TPP_TOK_AT = */ "AT",
	/* .ttr_TPP_TOK_BACKSLASH = */ "BACKSLASH",
	/* .ttr_TPP_TOK_DOT = */ "DOT",
	/* .ttr_TPP_TOK_COMMA = */ "COMMA",
	/* .ttr_TPP_TOK_COLON = */ "COLON",
	/* .ttr_TPP_TOK_POUND = */ "POUND",
	/* .ttr_TPP_TOK_LANGLE = */ "LANGLE",
	/* .ttr_TPP_TOK_RANGLE = */ "RANGLE",
	/* .ttr_TPP_TOK_LPAREN = */ "LPAREN",
	/* .ttr_TPP_TOK_RPAREN = */ "RPAREN",
	/* .ttr_TPP_TOK_LBRACKET = */ "LBRACKET",
	/* .ttr_TPP_TOK_RBRACKET = */ "RBRACKET",
	/* .ttr_TPP_TOK_LBRACE = */ "LBRACE",
	/* .ttr_TPP_TOK_RBRACE = */ "RBRACE",
	/* .ttr_TPP_TOK_PERCENT = */ "PERCENT",
	/* .ttr_TPP_TOK_EXCLAIM = */ "EXCLAIM",
	/* .ttr_TPP_TOK_AMP = */ "AMP",
	/* .ttr_TPP_TOK_PIPE = */ "PIPE",
	/* .ttr_TPP_TOK_PLUS = */ "PLUS",
	/* .ttr_TPP_TOK_MINUS = */ "MINUS",
	/* .ttr_TPP_TOK_STAR = */ "STAR",
	/* .ttr_TPP_TOK_SLASH = */ "SLASH",
	/* .ttr_TPP_TOK_QMARK = */ "QMARK",
	/* .ttr_TPP_TOK_SEMICOLON = */ "SEMICOLON",
	/* .ttr_TPP_TOK_TILDE = */ "TILDE",
	/* .ttr_TPP_TOK_HAT = */ "HAT",
#if TPP_HAVE_TPP_TOK_DOLLAR
	/* .ttr_TPP_TOK_DOLLAR = */ "DOLLAR",
#endif /* TPP_HAVE_TPP_TOK_DOLLAR */
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	/* .ttr_TPP_TOK_CHAR = */ "CHAR",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	/* .ttr_TPP_TOK_RAW_CHAR_LITERAL = */ "RAW_CHAR_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	/* .ttr_TPP_TOK_BLOCK_STRING_LITERAL = */ "BLOCK_STRING_LITERAL",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
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
	/* [0x60] = */ sizeof(struct tpp_token_str_strings_struct) - sizeof(char),
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	/* [TPP_TOK_CHAR] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	/* [TPP_TOK_RAW_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_RAW_CHAR_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	/* [TPP_TOK_BLOCK_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_str_strings_struct, ttr_TPP_TOK_BLOCK_STRING_LITERAL),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	char ttr_TPP_TOK_CHAR[7];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	char ttr_TPP_TOK_STRING[9];
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	/* .ttr_TPP_TOK_CHAR = */ "<char>",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	/* .ttr_TPP_TOK_STRING = */ "<string>",
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	/* [TPP_TOK_CHAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
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
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	/* [TPP_TOK_RAW_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	/* [TPP_TOK_BLOCK_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((2)) tpp_ssize TPPCALL
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

/* Finalize the given file. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_file_fini(tpp_file *tpp_restrict self) {
	if (self->tf_chunk)
		tpp_string_decref(self->tf_chunk);
	switch (self->tf_kind) {
	case TPP_FILE_KIND_IO:
#if TPP_HAVE_FILE_NOCLOSE
		if (!(self->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOCLOSE))
#endif /* TPP_HAVE_FILE_NOCLOSE */
		{
			tpp_io_close(self->tf_data.td_io.tff_file);
		}
		break;
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
 * - Allocate a new `struct tpp_string' suitable for holding both
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
 * @return: TPP_EOK: Either the current chunk was expanded (the delta
 *                   between `tf_pos' and `tf_end' has increased), or
 *                   no further data can be read from `self'.
 * @return: TPP_EIO: I/O error
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
#if TPP_HAVE_UNICODE
	bool is_first_chunk;
again:
#endif /* TPP_HAVE_UNICODE */

	if (self->tf_kind != TPP_FILE_KIND_IO)
		return TPP_EOK;
	old_inuse = (tpp_size)(self->tf_end - self->tf_pos);
	old_chunk = self->tf_chunk;
	new_size  = TPP_IO_CHUNKSIZE;
	if (new_size < old_inuse + TPP_IO_MINREAD)
		new_size = old_inuse + TPP_IO_MINREAD;
	tpp_assert(self->tf_pos <= self->tf_end);
	tpp_assert(!old_chunk || self->tf_pos >= tpp_string_str(old_chunk));
	tpp_assert(!old_chunk || self->tf_end <= tpp_string_end(old_chunk));
	if (old_chunk && !tpp_string_isshared(old_chunk)) {
		/* Can re-use the old chunk */
		tpp_size unused_head = (tpp_size)(self->tf_pos - tpp_string_str(old_chunk));
#ifndef __OPTIMIZE_SIZE__
		if (unused_head)
#endif /* !__OPTIMIZE_SIZE__ */
		{
			self->tf_data.td_io.tff_start_lc = tpp_lcinfo_account(self,
			                                                      self->tf_data.td_io.tff_start_lc,
			                                                      tpp_string_str(old_chunk), unused_head);
			tpp_memmovedown(tpp_string_str(old_chunk), self->tf_pos, old_inuse);
			self->tf_pos -= unused_head;
			self->tf_end -= unused_head;
#if TPP_HAVE_FILE_LC_CACHE
			if (self->tf_lcpos) {
				self->tf_lcpos -= unused_head;
				if (self->tf_lcpos < self->tf_pos)
					self->tf_lcpos = NULL; /* Cache fell out-of-scope */
			}
#endif /* TPP_HAVE_FILE_LC_CACHE */
		}
		tpp_assert(self->tf_pos == tpp_string_str(old_chunk));
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
			self->tf_pos = tpp_string_str(new_chunk);
			self->tf_end = tpp_string_str(new_chunk) + old_inuse;
#if TPP_HAVE_FILE_LC_CACHE
			if (self->tf_lcpos)
				self->tf_lcpos = tpp_string_str(new_chunk) + lc_rel;
#endif /* TPP_HAVE_FILE_LC_CACHE */
		}
		self->tf_chunk = new_chunk;
#if TPP_HAVE_UNICODE
		is_first_chunk = false;
#endif /* TPP_HAVE_UNICODE */
	} else {
		/* Must allocate a completely new chunk */
		new_chunk = tpp_string_trymalloc(new_size);
		if tpp_unlikely(!new_chunk) {
			new_size = old_inuse + TPP_FILE_MINEXTRA;
			new_chunk = tpp_string_malloc(new_size);
			if tpp_unlikely(!new_chunk)
				return TPP_ENOMEM;
		}
		tpp_memcpy(tpp_string_str(new_chunk), self->tf_pos, old_inuse);
		if (old_chunk) {
			tpp_size unused_head = (tpp_size)(self->tf_pos - tpp_string_str(old_chunk));
			self->tf_data.td_io.tff_start_lc = tpp_lcinfo_account(self,
			                                                      self->tf_data.td_io.tff_start_lc,
			                                                      tpp_string_str(old_chunk), unused_head);
			tpp_assert(tpp_string_isshared(old_chunk));
			tpp_string_decref_nokill(old_chunk);
#if TPP_HAVE_UNICODE
			is_first_chunk = false;
#endif /* TPP_HAVE_UNICODE */
		} else {
			tpp_lcinfo_init(self->tf_data.td_io.tff_start_lc, 0, 0);
#if TPP_HAVE_UNICODE
			is_first_chunk = true;
#endif /* TPP_HAVE_UNICODE */
		}
#if TPP_HAVE_FILE_LC_CACHE
		if (self->tf_lcpos) {
			self->tf_lcpos = tpp_string_str(new_chunk) + (self->tf_lcpos - self->tf_pos);
			if (self->tf_lcpos < tpp_string_str(new_chunk))
				self->tf_lcpos = NULL; /* Cache fell out-of-scope */
		}
#endif /* TPP_HAVE_FILE_LC_CACHE */
		self->tf_pos   = tpp_string_str(new_chunk);
		self->tf_end   = tpp_string_str(new_chunk) + old_inuse;
		self->tf_chunk = new_chunk; /* Inherit reference */
	}

	tpp_assert(self->tf_chunk == new_chunk);
	tpp_assert(self->tf_pos == tpp_string_str(new_chunk));
	tpp_assert(self->tf_end <= tpp_string_end(new_chunk));
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
			uint_least16_t const *base = (uint_least16_t const *)io_dst;
			tpp_size words = (tpp_size)read_status / 2;
			union tpp_word16 raw_last_word;
			uint_least16_t last_word;
			if tpp_unlikely(!words) {
				dst_base = dst_end;
				break;
			}
			raw_last_word.w16 = base[words - 1];
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
				dst_base = tpp_utf16le_to_utf8(base, words, dst_end);
			} else {
				dst_base = tpp_utf16be_to_utf8(base, words, dst_end);
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
			result = tpp_lcinfo_account(self, result, tpp_string_str(self->tf_chunk),
			                            (tpp_size)(pos - tpp_string_str(self->tf_chunk)));
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
	return (tpp_file *)iter;
}
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
 * @return: NULL: OOM (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_misc *TPPCALL
tpp_keyword_requiremisc(tpp_keyword *tpp_restrict self) {
	tpp_keyword_misc *result = self->tk_misc;
	if tpp_unlikely(result == NULL) {
		result = (tpp_keyword_misc *)tpp_malloc(sizeof(tpp_keyword_misc));
		if tpp_likely(result) {
#if TPP_HAVE_KEYWORD_FLAGS
			result->tkm_flags = TPP_KEYWORD_FLAG_NORMAL;
#endif /* TPP_HAVE_KEYWORD_FLAGS */
#if TPP_HAVE_KEYWORD_FILE_GUARD
			result->tkm_file_guard = NULL;
#endif /* TPP_HAVE_KEYWORD_FILE_GUARD */
#if TPP_HAVE_PRAGMA_PUSH_MACRO
			tpp_macro_pushstack_init(&result->tkm_macro_pushstack);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
			self->tk_misc = result;
		}
	}
	return result;
}
#endif /* TPP_HAVE_KEYWORD_MISC */


#if TPP_HAVE_PRAGMA_PUSH_MACRO
/* Push the current macro-definition of "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out-of-memory */
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
	tpp_free(self);
}
#endif /* TPP_HAVE_KEYWORD_MISC */

static TPP_NONNULL((1)) void TPPCALL
tpp_keyword_destroy(tpp_keyword *tpp_restrict self) {
	tpp_assert(!tpp_refcnt_isshared(&self->tk_refcnt) && "Keyword still in use");
#if TPP_HAVE_CPP_MACROS
	if (self->tk_macro)
		tpp_macro_decref(self->tk_macro);
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
 * @return: NULL: OOM (TPP_ENOMEM) */
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
 * @return: NULL: OOM (TPP_ENOMEM) */
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
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
} const tpp_extension_names = {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) /* .ten_##id = */ name,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
};

static tpp_size const tpp_extension_name_offsets_byid[TPP_EXT_COUNT] = {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) \
	/* [id] = */ tpp_offsetof(struct tpp_extension_names_struct, ten_##id),
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
} const tpp_warning_group_names = {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	/* .twgn_##wgroup_id##_##index = */ value,
#define TPP_WGROUP(wgroup_id, names, default)                                                  \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id) \
	/* .twgn_tail_##wgroup_id = */ 0,
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
};

static tpp_size const tpp_warning_group_name_offsets_byid[TPP_EXT_COUNT] = {
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
	/* [wgroup_id] = */ tpp_offsetof(struct tpp_warning_group_names_struct, twgn_##wgroup_id##_0),
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
#if TPP_HAVE_WARNING_NUMBERS
#define TPP_DECLARE_NUMBERED_WARNING(numbers_default) \
		/* .twsn_##warning_id = */ (unsigned int)numbers_default,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
		TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, numbers_default)
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS

#define tpp_builtin_keywords_list tpp_builtin_keywords_list
static tpp_keyword *tpp_builtin_keywords_list[] = {
#define TPP_DEFS
#define TPP_KWD(id, string) (tpp_keyword *)&tpp_builtin_keyword_##id,
#include TPP_CONFIG_DEFS_FILENAME
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
#define TPP_KWD_FLAGS(id, flags) \
		case id: return flags;
#include TPP_CONFIG_DEFS_FILENAME
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
	(void)self;
	switch (kwd->tk_id) {
#define TPP_DEFS
#define TPP_MACRO(id, if) \
	case id: return if;
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	default: break;
	}
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
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBER_CASE
#undef TPP_DEFS
} const tpp_warning_format_strings = {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	/* [twf_##warning_id] = */ format,
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBER_CASE
#undef TPP_DEFS
};

static tpp_size const tpp_warning_format_string_offsets[TPP_W_COUNT] = {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
	/* [warning_id] = */ tpp_offsetof(struct tpp_warning_format_strings_struct, twf_##warning_id),
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr) \
	/* [warning_id] = */ sizeof(struct tpp_warning_format_strings_struct) - 1,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBER_CASE
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
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
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
#define TPP_BUILTIN_MACRO(name, value)                          \
	case name: {                                                \
		static TPP_BUILTIN_MACRO_DEFINE(builtin_##name, value); \
		return (tpp_builtin_macro const *)&builtin_##name;      \
	}	break;
#include TPP_CONFIG_DEFS_FILENAME
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
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	}
};


#if TPP_HAVE_PRAGMA_EXTENSION_PUSH
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
tpp_extensions_set(tpp_extensions *tpp_restrict self,
                   tpp_extension_id id, int enabled) {
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
	tpp_extensions_state_set(&self->te_state, id, enabled);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_HAVE_PRAGMA_EXTENSION_PUSH */
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
#if TPP_HAVE_TPP_TOK_LF < 0
		/* .tff_TPP_FEAT_TPP_TOK_LF                         = */ TPP_HAVE_TPP_TOK_LF == -1,
#endif /* TPP_HAVE_TPP_TOK_LF < 0 */
#if TPP_HAVE_TPP_TOK_SPACE < 0
		/* .tff_TPP_FEAT_TPP_TOK_SPACE                      = */ TPP_HAVE_TPP_TOK_SPACE == -1,
#endif /* TPP_HAVE_TPP_TOK_SPACE < 0 */
#if TPP_HAVE_TPP_TOK_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_COMMENT                    = */ TPP_HAVE_TPP_TOK_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_CXX_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_COMMENT                = */ TPP_HAVE_TPP_TOK_CXX_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_C_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_C_COMMENT                  = */ TPP_HAVE_TPP_TOK_C_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_C_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_PASCAL_COMMENT             = */ TPP_HAVE_TPP_TOK_PASCAL_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_SHELL_COMMENT              = */ TPP_HAVE_TPP_TOK_SHELL_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_ASM_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_ASM_COMMENT                = */ TPP_HAVE_TPP_TOK_ASM_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_SQL_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_SQL_COMMENT                = */ TPP_HAVE_TPP_TOK_SQL_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_DOLLAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_DOLLAR                     = */ TPP_HAVE_TPP_TOK_DOLLAR == -1,
#endif /* TPP_HAVE_TPP_TOK_DOLLAR < 0 */
#if TPP_HAVE_TPP_TOK_INT < 0
		/* .tff_TPP_FEAT_TPP_TOK_INT                        = */ TPP_HAVE_TPP_TOK_INT == -1,
#endif /* TPP_HAVE_TPP_TOK_INT < 0 */
#if TPP_HAVE_TPP_TOK_FLOAT < 0
		/* .tff_TPP_FEAT_TPP_TOK_FLOAT                      = */ TPP_HAVE_TPP_TOK_FLOAT == -1,
#endif /* TPP_HAVE_TPP_TOK_FLOAT < 0 */
#if TPP_HAVE_TPP_TOK_CHAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_CHAR                       = */ TPP_HAVE_TPP_TOK_CHAR == -1,
#endif /* TPP_HAVE_TPP_TOK_CHAR < 0 */
#if TPP_HAVE_TPP_TOK_STRING < 0
		/* .tff_TPP_FEAT_TPP_TOK_STRING                     = */ TPP_HAVE_TPP_TOK_STRING == -1,
#endif /* TPP_HAVE_TPP_TOK_STRING < 0 */
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL     = */ TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL    = */ TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL    = */ TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL   = */ TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL   = */ TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL         = */ TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL           = */ TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL       = */ TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL         = */ TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE              = */ TPP_HAVE_TPP_TOK_LANGLE_LANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE              = */ TPP_HAVE_TPP_TOK_RANGLE_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL                = */ TPP_HAVE_TPP_TOK_EQUAL_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL              = */ TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_EQUAL               = */ TPP_HAVE_TPP_TOK_RANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_EQUAL               = */ TPP_HAVE_TPP_TOK_LANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0
		/* .tff_TPP_FEAT_TPP_TOK_DOT_DOT_DOT                = */ TPP_HAVE_TPP_TOK_DOT_DOT_DOT == -1,
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0 */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_PLUS_EQUAL                 = */ TPP_HAVE_TPP_TOK_PLUS_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_MINUS_EQUAL                = */ TPP_HAVE_TPP_TOK_MINUS_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_STAR_EQUAL                 = */ TPP_HAVE_TPP_TOK_STAR_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_SLASH_EQUAL                = */ TPP_HAVE_TPP_TOK_SLASH_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_PERCENT_EQUAL              = */ TPP_HAVE_TPP_TOK_PERCENT_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL        = */ TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL        = */ TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_AMP_EQUAL                  = */ TPP_HAVE_TPP_TOK_AMP_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_PIPE_EQUAL                 = */ TPP_HAVE_TPP_TOK_PIPE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_HAT_EQUAL                  = */ TPP_HAVE_TPP_TOK_HAT_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH < 0
		/* .tff_TPP_FEAT_TPP_TOK_SLASH_SLASH                = */ TPP_HAVE_TPP_TOK_SLASH_SLASH == -1,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL          = */ TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL            = */ TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_AT_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_AT_EQUAL                   = */ TPP_HAVE_TPP_TOK_AT_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_POUND_POUND < 0
		/* .tff_TPP_FEAT_TPP_TOK_POUND_POUND                = */ TPP_HAVE_TPP_TOK_POUND_POUND == -1,
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND < 0 */
#if TPP_HAVE_TPP_TOK_AMP_AMP < 0
		/* .tff_TPP_FEAT_TPP_TOK_AMP_AMP                    = */ TPP_HAVE_TPP_TOK_AMP_AMP == -1,
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP < 0 */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE < 0
		/* .tff_TPP_FEAT_TPP_TOK_PIPE_PIPE                  = */ TPP_HAVE_TPP_TOK_PIPE_PIPE == -1,
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE < 0 */
#if TPP_HAVE_TPP_TOK_HAT_HAT < 0
		/* .tff_TPP_FEAT_TPP_TOK_HAT_HAT                    = */ TPP_HAVE_TPP_TOK_HAT_HAT == -1,
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT < 0 */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS < 0
		/* .tff_TPP_FEAT_TPP_TOK_PLUS_PLUS                  = */ TPP_HAVE_TPP_TOK_PLUS_PLUS == -1,
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS < 0
		/* .tff_TPP_FEAT_TPP_TOK_MINUS_MINUS                = */ TPP_HAVE_TPP_TOK_MINUS_MINUS == -1,
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_STAR_STAR                  = */ TPP_HAVE_TPP_TOK_STAR_STAR == -1,
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR < 0 */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE < 0
		/* .tff_TPP_FEAT_TPP_TOK_TILDE_TILDE                = */ TPP_HAVE_TPP_TOK_TILDE_TILDE == -1,
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE < 0 */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_TILDE_EQUAL                = */ TPP_HAVE_TPP_TOK_TILDE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE               = */ TPP_HAVE_TPP_TOK_MINUS_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_COLON_EQUAL                = */ TPP_HAVE_TPP_TOK_COLON_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_COLON_COLON < 0
		/* .tff_TPP_FEAT_TPP_TOK_COLON_COLON                = */ TPP_HAVE_TPP_TOK_COLON_COLON == -1,
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR          = */ TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR == -1,
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0 */
#if TPP_HAVE_TPP_TOK_DOT_STAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_DOT_STAR                   = */ TPP_HAVE_TPP_TOK_DOT_STAR == -1,
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR < 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT < 0
		/* .tff_TPP_FEAT_TPP_TOK_DOT_DOT                    = */ TPP_HAVE_TPP_TOK_DOT_DOT == -1,
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_RANGLE              = */ TPP_HAVE_TPP_TOK_LANGLE_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE       = */ TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE       = */ TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL = */ TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL = */ TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL          = */ TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL        = */ TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK < 0
		/* .tff_TPP_FEAT_TPP_TOK_QMARK_QMARK                = */ TPP_HAVE_TPP_TOK_QMARK_QMARK == -1,
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK < 0 */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_QMARK_EQUAL                = */ TPP_HAVE_TPP_TOK_QMARK_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_LANGLE              = */ TPP_HAVE_TPP_TOK_RANGLE_LANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_PLUS                 = */ TPP_HAVE_TPP_TOK_EQUAL_PLUS == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_MINUS                = */ TPP_HAVE_TPP_TOK_EQUAL_MINUS == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_STAR                 = */ TPP_HAVE_TPP_TOK_EQUAL_STAR == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR            = */ TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_SLASH                = */ TPP_HAVE_TPP_TOK_EQUAL_SLASH == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_SLASH_SLASH          = */ TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_PERCENT              = */ TPP_HAVE_TPP_TOK_EQUAL_PERCENT == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_AMP                  = */ TPP_HAVE_TPP_TOK_EQUAL_AMP == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_PIPE                 = */ TPP_HAVE_TPP_TOK_EQUAL_PIPE == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_HAT                  = */ TPP_HAVE_TPP_TOK_EQUAL_HAT == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_LANGLE               = */ TPP_HAVE_TPP_TOK_EQUAL_LANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE        = */ TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE = */ TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_RANGLE               = */ TPP_HAVE_TPP_TOK_EQUAL_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE        = */ TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE = */ TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_AT < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_AT                   = */ TPP_HAVE_TPP_TOK_EQUAL_AT == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_TILDE                = */ TPP_HAVE_TPP_TOK_EQUAL_TILDE == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_COLON                = */ TPP_HAVE_TPP_TOK_EQUAL_COLON == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_EXCLAIM              = */ TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM        = */ TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_QMARK                = */ TPP_HAVE_TPP_TOK_EQUAL_QMARK == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_MINUS               = */ TPP_HAVE_TPP_TOK_LANGLE_MINUS == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS < 0
		/* .tff_TPP_FEAT_TPP_TOK_STAR_LANGLE_MINUS          = */ TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS == -1,
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_STAR_DOT < 0
		/* .tff_TPP_FEAT_TPP_TOK_STAR_DOT                   = */ TPP_HAVE_TPP_TOK_STAR_DOT == -1,
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT < 0 */
#if TPP_HAVE_CPP_DIRECTIVES < 0
		/* .tff_TPP_FEAT_CPP_DIRECTIVES                     = */ TPP_HAVE_CPP_DIRECTIVES == -1,
#endif /* TPP_HAVE_CPP_DIRECTIVES < 0 */
#if TPP_HAVE_CPP_MACROS < 0
		/* .tff_TPP_FEAT_CPP_MACROS                         = */ TPP_HAVE_CPP_MACROS == -1,
#endif /* TPP_HAVE_CPP_MACROS < 0 */
#if TPP_HAVE_CPP_BLANK < 0
		/* .tff_TPP_FEAT_CPP_BLANK                          = */ TPP_HAVE_CPP_BLANK == -1,
#endif /* TPP_HAVE_CPP_BLANK < 0 */
#if TPP_HAVE_CPP_DIGIT_LINE < 0
		/* .tff_TPP_FEAT_CPP_DIGIT_LINE                     = */ TPP_HAVE_CPP_DIGIT_LINE == -1,
#endif /* TPP_HAVE_CPP_DIGIT_LINE < 0 */
#if TPP_HAVE_CPP_LINE < 0
		/* .tff_TPP_FEAT_CPP_LINE                           = */ TPP_HAVE_CPP_LINE == -1,
#endif /* TPP_HAVE_CPP_LINE < 0 */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF < 0
		/* .tff_TPP_FEAT_CPP_IF_ELSE_ENDIF                  = */ TPP_HAVE_CPP_IF_ELSE_ENDIF == -1,
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF < 0 */
#if TPP_HAVE_CPP_DEFINE < 0
		/* .tff_TPP_FEAT_CPP_DEFINE                         = */ TPP_HAVE_CPP_DEFINE == -1,
#endif /* TPP_HAVE_CPP_DEFINE < 0 */
#if TPP_HAVE_CPP_PRAGMA < 0
		/* .tff_TPP_FEAT_CPP_PRAGMA                         = */ TPP_HAVE_CPP_PRAGMA == -1,
#endif /* TPP_HAVE_CPP_PRAGMA < 0 */
#if TPP_HAVE_CPP_EMBED < 0
		/* .tff_TPP_FEAT_CPP_EMBED                          = */ TPP_HAVE_CPP_EMBED == -1,
#endif /* TPP_HAVE_CPP_EMBED < 0 */
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

/* Make sure that offsets within "tpp_lexer" are properly aligned such that
 * the tail end of "tpp_token" correctly overlaps with the start of "tpp_file" */
#if TPP_HAVE_INCLUDE_STACK
TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer, tl_core.tlc_tok.tt_start) ==
                  tpp_offsetof(tpp_lexer, tl_core.tlc_input.tli_file.tf_tpos));
#endif /* !TPP_HAVE_INCLUDE_STACK */
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


/* Initialize a lexer that simply reads the given [text,text+text_size) blob. */
#if TPP_HAVE_UNICODE
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ex(tpp_lexer *tpp_restrict self,
                       /*utf-8*/ char const *filename,
                       void const *text, tpp_size text_size,
                       tpp_lcinfo start_lc, tpp_file_encoding encoding)
#else /* TPP_HAVE_UNICODE */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ascii(tpp_lexer *tpp_restrict self,
                          /*utf-8*/ char const *filename,
                          void const *text, tpp_size text_size,
                          tpp_lcinfo start_lc)
#endif /* !TPP_HAVE_UNICODE */
{
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_init_text_ex(file, filename, text, text_size, start_lc, encoding);
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
 * @return: * : TPP_ENOENT: No such file or directory
 * @return: * : TPP_ENOMEM: Out of memory */
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
 * @param: pos:     Lexer position used by certain format-patterns.
 * @param: file:    The file containing "pos"
 * @param: printer: Output printer for formatted text
 * @param: arg:     Cookie argument for "printer"
 * @param: format:  Format pattern (see above)
 * @param: args:    Extra varargs-arguments for "format"
 * @return: >= 0:   Sum of return values of "printer".
 * @return: < 0:    First negative return value of "printer". The more high-level
 *                  "tpp_lexer_warnf" API returns "TPP_EWARNPRINT" in this case. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4, 6)) tpp_ssize TPPVCALL
tpp_lexer_printf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                         tpp_formatprinter printer, void *arg,
                         char const *format, ...) {
	tpp_ssize result;
	va_list args;
	va_start(args, format);
	result = tpp_lexer_vprintf_warning(self, file, pos, printer, arg, format, args);
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

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 5, 6)) tpp_ssize TPPCALL
tpp_lexer_vprintf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                          tpp_formatprinter printer, void *arg,
                          char const *format, va_list args) {
	static char const null_str[] = "(null)";
	tpp_lcinfo lcinfo;
	tpp_ssize temp, result = 0;
	char const *iter = format;
	char ch;
	tpp_lcinfo_init(lcinfo, -1, -1);
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
			if (tpp_lcinfo_getline(lcinfo) == -1)
				lcinfo = tpp_file_lcinfo(file, pos);
			temp = tpp_format_print_int(printer, arg,
			                            ch == 'l' ? (tpp_lcinfo_getline(lcinfo) + 1)
			                                      : (tpp_lcinfo_getcol(lcinfo) + 1));
		}	break;

		case 'f': {
			/* "%Pf"   Filename of given "file" */
			char const *filename = tpp_file_filename(file);
			if (filename == NULL)
				filename = "?";
			temp = (*printer)(arg, (tpp_char const *)filename, tpp_strlen(filename));
		}	break;

		case 't': {
			/* "%Pt"   "%[current-token%]" */
			tpp_token const *const token = tpp_lexer_gettoken(self);
			temp = tpp_format_quote_start(printer, arg);
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			temp = tpp_format_token_data(printer, arg, token->tt_start,
			                             (tpp_size)(token->tt_end - token->tt_start));
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
	result = tpp_warnings_invoke(tpp_lexer_getwarn(self), id, &invokeinfo);
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
	printer_status = tpp_lexer_printf_warning(self, file, pos, printer, printer_arg,
	                                          TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT);
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
		printer_status = tpp_lexer_vprintf_warning(self, file,
		                                           pos, printer, printer_arg,
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
#if TPP_HOST_HAVE_PP_VARARGS
#define tpp_warnf(...)                                    \
	tpp_do(tpp_lexer_printf_warning(self, file, pos,      \
	                                printer, printer_arg, \
	                                __VA_ARGS__))
#endif /* TPP_HOST_HAVE_PP_VARARGS */
/* ... */
/************************************************************************/

#define TPP_DEFS
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr) \
		case warning_id: {                                                     \
			expr;                                                              \
		}	break;
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBER_CASE
#undef TPP_DEFS

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
			printer_status = tpp_lexer_printf_warning(self, caller, caller->tf_tpos, printer, printer_arg,
			                                          TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT);
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

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf_at(self, pos, id, args);
	va_end(args);
	return result;
}
#endif /* TPP_HAVE_WARNINGS */


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
	if (!tpp_lexer_getext(self, TPP_EXT_BSE))
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
	if (tpp_lexer_getext(self, TPP_EXT_BSE_WHITESPACE)) {
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
			if (*scan == '?' && tpp_lexer_getext(self, TPP_EXT_TRIGRAPHS)) {
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
		if (ch == '$' && tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_DOLLAR))
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
		if (*pos == '?' && tpp_lexer_getext(self, TPP_EXT_TRIGRAPHS)) {
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
#if (TPP_HAVE_TPP_TOK_CHAR || TPP_HAVE_TPP_TOK_STRING || \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||         \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL ||        \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||         \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL)
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
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
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
	if (ch == '?' && tpp_lexer_getext(self, TPP_EXT_TRIGRAPHS)) {
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
					if (tpp_lexer_getext(self, TPP_EXT_BSE)) {
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
					if (ch == '?' && tpp_lexer_getext(self, TPP_EXT_TRIGRAPHS)) {
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
	if (ch == '?' && tpp_lexer_getext(self, TPP_EXT_TRIGRAPHS)) {
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
		tpp_size old_pos = tpp_file_ptr2rel(file, *p_pos);
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
		} else if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_UNICODE
handle_linefeed:
#endif /* TPP_HAVE_UNICODE */
			*p_pos = tpp_file_rel2ptr(file, old_pos);
			/* Warning if a line-feed is encountered */
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
			return tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
			                          TPP_W_STRING_TERMINATED_BY_LINEFEED);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
			break;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
		} else
#if TPP_HAVE_UNICODE
		if (ch >= 0x80 && tpp_file_isutf8(file)) {
			/* Check for unicode linefeed */
			tpp_unichar uc;
			*p_pos = tpp_file_rel2ptr(file, old_pos);
			error = tpp_lexer_readutf8(self, p_pos, &uc);
			if (TPP_ISERR(error))
				return error;
			if (tpp_unicode_islf(uc))
				goto handle_linefeed;
		} else
#endif /* TPP_HAVE_UNICODE */
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
		tpp_size old_pos = tpp_file_ptr2rel(file, *p_pos);
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (TPP_ISERR(error))
			return error;
		if (ch == quote_char)
			break;
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
		if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_UNICODE
handle_linefeed:
#endif /* TPP_HAVE_UNICODE */
			*p_pos = tpp_file_rel2ptr(file, old_pos);
			/* Warning if a line-feed is encountered */
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
			return tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
			                          TPP_W_STRING_TERMINATED_BY_LINEFEED);
#else /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
			break;
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
		} else
#if TPP_HAVE_UNICODE
		if (ch >= 0x80 && tpp_file_isutf8(file)) {
			/* Check for unicode linefeed */
			tpp_unichar uc;
			*p_pos = tpp_file_rel2ptr(file, old_pos);
			error = tpp_lexer_readutf8(self, p_pos, &uc);
			if (TPP_ISERR(error))
				return error;
			if (tpp_unicode_islf(uc))
				goto handle_linefeed;
		} else
#endif /* TPP_HAVE_UNICODE */
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
		 *       support in "tpp_decode_bsi()" (our partner function) */
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
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE ||       \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_LANGLE_RANGLE ||              \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS || TPP_HAVE_DIGRAPHS)
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_RANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_MINUS) &&
		    !tpp_lexer_getext(self, TPP_EXT_DIGRAPHS))
			break;
		read_ch2();

#if TPP_HAVE_DIGRAPHS
		if (ch2 == '%') {
			if (tpp_lexer_getext(self, TPP_EXT_DIGRAPHS)) {
				result = (tpp_token_id)'{'; /* "<%" -> "{" */
				goto set_result;
			}
		} else if (ch2 == ':') {
			if (tpp_lexer_getext(self, TPP_EXT_DIGRAPHS)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL) ||
			    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE) ||
			    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
				read_ch2();

#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
				if (ch2 == '<') {
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)) {
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
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE)) {
						result = TPP_TOK_LANGLE_LANGLE_LANGLE; /* "<<<" */
						goto set_result;
					}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
				} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
				if (ch2 == '=') {
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE)) {
				result = TPP_TOK_LANGLE_LANGLE; /* "<<" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_EQUAL)) {
				result = TPP_TOK_LANGLE_EQUAL; /* "<=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
		if (ch2 == '>') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_RANGLE)) {
				result = TPP_TOK_LANGLE_RANGLE; /* "<>" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
		if (ch2 == '-') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_MINUS)) {
				result = TPP_TOK_LANGLE_MINUS; /* "<-" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS */
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '>': {
#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||              \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE ||       \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_RANGLE_LANGLE)
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_LANGLE))
			break;
		read_ch2();

#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
		if (ch2 == '>') {
#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL) ||
			    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE) ||
			    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)) {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
				read_ch2();

#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
				if (ch2 == '>') {
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)) {
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
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE)) {
						result = TPP_TOK_RANGLE_RANGLE_RANGLE; /* ">>>" */
						goto set_result;
					}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
				} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
				if (ch2 == '=') {
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE)) {
				result = TPP_TOK_RANGLE_RANGLE; /* ">>" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_EQUAL)) {
				result = TPP_TOK_RANGLE_EQUAL; /* ">=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
		if (ch2 == '<') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_LANGLE)) {
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

		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_PLUS) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_MINUS) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_STAR) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_SLASH) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_SLASH_SLASH) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_PERCENT) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_AMP) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_PIPE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_HAT) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_LANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_RANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_AT) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_TILDE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_COLON) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EXCLAIM) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_QMARK))
			break;
		read_ch2();
#if (TPP_HAVE_TPP_TOK_EQUAL_EQUAL ||       \
     TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL || \
     TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM)
		if (ch2 == '=') {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL) ||
			    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM)) {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
				read_ch2();
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
				if (ch2 == '=') {
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL)) {
						result = TPP_TOK_EQUAL_EQUAL_EQUAL; /* "===" */
						goto set_result;
					}
				} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
				if (ch2 == '=') {
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL)) {
				result = TPP_TOK_EQUAL_EQUAL; /* "==" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR || TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
		if (ch == '*') {
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_STAR)) {
				result = TPP_TOK_EQUAL_STAR; /* "=*" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR */
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR || TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH || TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
		if (ch == '/') {
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_SLASH_SLASH)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_SLASH)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE) ||
			    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
				read_ch2();
				if (ch2 == '<') {
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)) {
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
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_LANGLE)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE) ||
			    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE)) {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
				read_ch2();
				if (ch2 == '>') {
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)) {
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
					if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_RANGLE)) {
				result = TPP_TOK_EQUAL_RANGLE; /* "=>" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS
		if (ch2 == '+') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_PLUS)) {
				result = TPP_TOK_EQUAL_PLUS; /* "=+" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS
		if (ch2 == '-') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_MINUS)) {
				result = TPP_TOK_EQUAL_MINUS; /* "=-" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT
		if (ch2 == '%') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_PERCENT)) {
				result = TPP_TOK_EQUAL_PERCENT; /* "=%" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP
		if (ch2 == '&') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_AMP)) {
				result = TPP_TOK_EQUAL_AMP; /* "=&" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE
		if (ch2 == '|') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_PIPE)) {
				result = TPP_TOK_EQUAL_PIPE; /* "=|" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT
		if (ch2 == '^') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_HAT)) {
				result = TPP_TOK_EQUAL_HAT; /* "=^" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_EQUAL_AT
		if (ch2 == '@') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_AT)) {
				result = TPP_TOK_EQUAL_AT; /* "=@" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE
		if (ch2 == '~') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_TILDE)) {
				result = TPP_TOK_EQUAL_TILDE; /* "=~" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON
		if (ch2 == ':') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_COLON)) {
				result = TPP_TOK_EQUAL_COLON; /* "=:" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
		if (ch2 == '!') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EXCLAIM)) {
				result = TPP_TOK_EQUAL_EXCLAIM; /* "=!" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK
		if (ch2 == '?') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_QMARK)) {
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
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL))
			break;
		read_ch2();
		if (ch2 == '=') {
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL)) {
				result = TPP_TOK_EXCLAIM_EQUAL; /* "!=" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '.': {
#if (TPP_HAVE_TPP_TOK_DOT_DOT_DOT || \
     TPP_HAVE_TPP_TOK_DOT_DOT ||     \
     TPP_HAVE_TPP_TOK_DOT_STAR)
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_DOT_DOT_DOT) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_DOT_DOT) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_DOT_STAR))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT || TPP_HAVE_TPP_TOK_DOT_DOT
		if (ch2 == '.') {
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_DOT_DOT_DOT)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_DOT_DOT)) {
				result = TPP_TOK_DOT_DOT; /* ".." */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
		} else
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT || TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_DOT_STAR
		if (ch2 == '*') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_DOT_STAR)) {
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PLUS_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PLUS_PLUS))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PLUS_EQUAL)) {
				result = TPP_TOK_PLUS_EQUAL; /* "+=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
		if (ch2 == '+') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PLUS_PLUS)) {
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
#if (TPP_HAVE_TPP_TOK_MINUS_EQUAL ||  \
     TPP_HAVE_TPP_TOK_MINUS_MINUS ||  \
     TPP_HAVE_TPP_TOK_SQL_COMMENT ||  \
     TPP_HAVE_TPP_TOK_MINUS_RANGLE || \
     TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR)
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_MINUS_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_MINUS_MINUS) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SQL_COMMENT) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_MINUS_RANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_MINUS_EQUAL)) {
				result = TPP_TOK_MINUS_EQUAL; /* "-=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS || TPP_HAVE_TPP_TOK_SQL_COMMENT
		if (ch2 == '-') {
#if TPP_HAVE_TPP_TOK_SQL_COMMENT
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SQL_COMMENT)) {
				error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SQL_COMMENT));
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_SQL_COMMENT; // "-- like this one!"
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_MINUS_MINUS)) {
				result = TPP_TOK_MINUS_MINUS; /* "--" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
		} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS || TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
		if (ch2 == '>') {
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_MINUS_RANGLE)) {
				result = TPP_TOK_MINUS_RANGLE; /* "->" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
		} else
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_STAR) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_LANGLE_MINUS) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_DOT))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_EQUAL)) {
				result = TPP_TOK_STAR_EQUAL; /* "*=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR || TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
		if (ch2 == '*') {
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_STAR)) {
				result = TPP_TOK_STAR_STAR; /* "**" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
		} else
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR || TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
		if (ch2 == '<') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_LANGLE_MINUS)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_DOT)) {
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_COMMENT) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_C_COMMENT) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SLASH_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SLASH_SLASH) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_ASM_COMMENT))
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_COMMENT)) {
				error = tpp_lexer_seek_eol(self, &pos tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_CXX_COMMENT));
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_CXX_COMMENT; // like this one!
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SLASH_SLASH)) {
				result = TPP_TOK_SLASH_SLASH; /* "//" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
		} else
#endif /* ... */
#if TPP_HAVE_TPP_TOK_C_COMMENT
		if (ch2 == '*') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_C_COMMENT)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SLASH_EQUAL)) {
				result = TPP_TOK_SLASH_EQUAL; /* "/=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
		{
#if TPP_HAVE_TPP_TOK_ASM_COMMENT
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_ASM_COMMENT)) {
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PERCENT_EQUAL) &&
		    !tpp_lexer_getext(self, TPP_EXT_DIGRAPHS))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PERCENT_EQUAL)) {
				result = TPP_TOK_PERCENT_EQUAL; /* "%=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_DIGRAPHS
		if (ch2 == '>') {
			if (tpp_lexer_getext(self, TPP_EXT_DIGRAPHS)) {
				result = (tpp_token_id)'}'; /* "%>" -> "}" */
				goto set_result;
			}
		} else
		if (ch2 == ':') {
			if (tpp_lexer_getext(self, TPP_EXT_DIGRAPHS)) {
#if TPP_HAVE_TPP_TOK_POUND_POUND
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_POUND_POUND)) {
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_AMP_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_AMP_AMP))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_AMP_EQUAL)) {
				result = TPP_TOK_AMP_EQUAL; /* "&=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_AMP
		if (ch2 == '&') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_AMP_AMP)) {
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PIPE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PIPE_PIPE))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PIPE_EQUAL)) {
				result = TPP_TOK_PIPE_EQUAL; /* "|=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
		if (ch2 == '|') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PIPE_PIPE)) {
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_HAT_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_HAT_HAT))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_HAT_EQUAL)) {
				result = TPP_TOK_HAT_EQUAL; /* "^=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_HAT
		if (ch2 == '^') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_HAT_HAT)) {
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
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_AT_EQUAL)) {
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
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_POUND_POUND)) {
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
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SHELL_COMMENT)) {
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_TILDE_TILDE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_TILDE_EQUAL))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
		if (ch2 == '~') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_TILDE_TILDE)) {
				warn_if_ch2_is_trigraph();    /* "??-" -> "~" */
				result = TPP_TOK_TILDE_TILDE; /* "~~" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_TILDE_EQUAL)) {
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COLON_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COLON_COLON) &&
		    !tpp_lexer_getext(self, TPP_EXT_DIGRAPHS))
			break;
		read_ch2();
#if TPP_HAVE_DIGRAPHS
		if (ch2 == '>') {
			if (tpp_lexer_getext(self, TPP_EXT_DIGRAPHS)) {
				result = (tpp_token_id)']'; /* ":>" -> "]" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_DIGRAPHS */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COLON_EQUAL)) {
				result = TPP_TOK_COLON_EQUAL; /* ":=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
		if (ch2 == ':') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COLON_COLON)) {
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
		if (tpp_lexer_getext(self, TPP_EXT_TRIGRAPHS)) {
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

		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_QMARK_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_QMARK_QMARK))
			break;
		read_ch2();
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
		if (ch2 == '=') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_QMARK_EQUAL)) {
				result = TPP_TOK_QMARK_EQUAL; /* "?=" */
				goto set_result;
			}
		} else
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
		if (ch2 == '?') {
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_QMARK_QMARK)) {
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_PASCAL_COMMENT))
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
		if (tpp_lexer_getext(self, TPP_EXT_ESCAPE_IN_IDENTIFIERS)) {
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
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL)) {
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
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CHAR)) {
			error = tpp_lexer_seek_end_of_string(self, &pos, '\'');
			if (TPP_ISERR(error))
				goto return_error;
			result = TPP_TOK_CHAR; /* 'foo' */
			/* TODO: -Wno-multichar (but should also depend on "-fmultichar-constants"; aka. "EXT_MULTICHAR_CONST") */
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
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL)) {
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
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STRING)) {
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
     TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
	case 'R': {
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL) ||
		    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL) ||
		    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
			if (ch2 == '"') {
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_RAW_STRING_LITERAL; /* R"AB(foo)AB" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_STRING_LITERAL; /* R"foo" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
			if (ch2 == '\'') {
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '\'');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_RAW_CHAR_LITERAL; /* R'foo' */
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
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
	case 'r': {
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL) ||
		    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL)) {
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
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL)) {
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
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	case 'L': {
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL)) {
			read_ch2();
			if (ch2 == '"') {
				error = tpp_lexer_seek_end_of_string(self, &pos, '"');
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_CXX_WIDE_STRING_LITERAL; /* L"foo" */
				goto set_result;
			} else
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
			if (ch2 == 'R') {
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL)) {
					read_ch2();
					if (ch2 == '"') {
						error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
						if (TPP_ISERR(error))
							goto return_error;
						result = TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL; /* LR"AB(foo)AB" */
						goto set_result;
					}
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	case 'u': {
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL) ||
		    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL)) {
			read_ch2();
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
			if (ch2 == '8') {
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL)) {
					read_ch2();
					if (ch2 == '"') {
						error = tpp_lexer_seek_end_of_string(self, &pos, '"');
						if (TPP_ISERR(error))
							goto return_error;
						result = TPP_TOK_CXX_UTF8_STRING_LITERAL; /* u8"foo" */
						goto set_result;
					} else
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
					if (ch2 == 'R') {
						if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL)) {
							read_ch2();
							if (ch2 == '"') {
								error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
								if (TPP_ISERR(error))
									goto return_error;
								result = TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL; /* u8R"AB(foo)AB" */
								goto set_result;
							}
						}
					} else
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
					{
					}
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
			if (ch2 == '"') {
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_string(self, &pos, '"');
					if (TPP_ISERR(error))
						goto return_error;
					result = TPP_TOK_CXX_UTF16_STRING_LITERAL; /* u8"foo" */
					goto set_result;
				}
			} else
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
			if (ch2 == 'R') {
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL) &&
				    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL)) {
					read_ch2();
					if (ch2 == '"') {
						error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
						if (TPP_ISERR(error))
							goto return_error;
						result = TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL; /* uR"AB(foo)AB" */
						goto set_result;
					}
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	case 'U': {
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL)) {
			read_ch2();
			if (ch2 == '"') {
				error = tpp_lexer_seek_end_of_string(self, &pos, '"');
				if (TPP_ISERR(error))
					goto return_error;
				result = TPP_TOK_CXX_UTF32_STRING_LITERAL; /* U"foo" */
				goto set_result;
			} else
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
			if (ch2 == 'R') {
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL)) {
					read_ch2();
					if (ch2 == '"') {
						error = tpp_lexer_seek_end_of_cxx_raw_string(self, &pos);
						if (TPP_ISERR(error))
							goto return_error;
						result = TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL; /* UR"AB(foo)AB" */
						goto set_result;
					}
				}
			} else
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
			{
			}
			pos = tpp_file_rel2ptr(file, rel_start + 1);
		}
		goto handle_keyword;
#define WANT_handle_keyword
	}
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_DOLLAR
	case '$':
#if TPP_HAVE_TPP_TOK_DOLLAR > 0
		break; /* Follow single-char code-branch */
#else /* TPP_HAVE_TPP_TOK_DOLLAR > 0 */
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_DOLLAR))
			break; /* Follow single-char code-branch */
		goto handle_keyword;
#define WANT_handle_keyword
#endif /* TPP_HAVE_TPP_TOK_DOLLAR <= 0 */
#endif /* TPP_HAVE_TPP_TOK_DOLLAR */
/************************************************************************/

		/* TODO: C says that (implementations can threat) this:
		 * >> char const *\U0001f431 = "cat";
		 *
		 * as a valid identifier. -- We should support that (*and* interpret
		 * it as "\xF0\x9F\x90\xB1" (its utf-8 repr) during keyword lookup)
		 *
		 * For this purpose, the "*_bse" version of keyword lookup functions
		 * should also have another extension that lets them treat \u and \U
		 * sequences specially! */

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
				if (*pos == '?' && tpp_lexer_getext(self, TPP_EXT_TRIGRAPHS)) {
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
				if (tpp_lexer_getext(self, TPP_EXT_ESCAPE_IN_IDENTIFIERS)) {
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
		if (tpp_ascii_isdigit(ch) && (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_FLOAT) ||
		                              tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_INT))) {
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
			    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_FLOAT)) {
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
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_INT)) {
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
		tpp_file *const prev = file->tf_prev;
		tpp_file_fini(file);
		*file = *prev;
		tpp_free(prev);
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

#if TPP_HAVE_LEXER_SEEK_RPAREN

typedef struct tpp_lexer_arginfo_rel {
	tpp_size tlair_start; /* [<= tlai_end] Relative offset to argument start text data */
	tpp_size tlair_end;   /* [>= tlai_start] Relative offset to argument end text data */
} tpp_lexer_arginfo_rel;

TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer_arginfo_rel, tlair_start) ==
                  tpp_offsetof(tpp_lexer_arginfo, tlai_start));
TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer_arginfo_rel, tlair_end) ==
                  tpp_offsetof(tpp_lexer_arginfo, tlai_end));
TPP_STATIC_ASSERT(sizeof(tpp_lexer_arginfo_rel) == sizeof(tpp_lexer_arginfo));

#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
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
#else /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
#define TPP_RECURSION_CC_PAREN 0
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN_EX */



/* Find the position of the next unmatched ')'-token, gathering information
 * about ranges of ','-token-separated text-sequences along the way.
 *
 * NOTE: This function automatically handles "TPP_TOK_EWOULDBLOCK" (meaning it always blocks)
 *
 * @param: p_pos: [in/out]  In:  Pointer to first character to start checking for ')'
 *                          Out: Pointer to the trailing ')', or EOF
 *                               if end-of-file was reached first, or
 *                               in case of TPP_TOK_EWOULDBLOCK.
 * @param: p_argv: [out]    Output buffer for the bounds of macro
 *                          arguments encountered along the way.
 *                          The size of this buffer is IN(*p_argc)
 * @param: p_argc: [in/out] In:  Size of provided "p_argv" buffer (in elements)
 *                          Out: Number of arguments actually encountered. May
 *                               be set to a number greater tha IN(*p_argc), in
 *                               which case only info about the first IN(*p_argc)
 *                               arguments is actually returned. But note the
 *                               `TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS' flag, which
 *                               causes the last element of `p_argv' to span up
 *                               to the closing ')'
 * @param: opt_function_name_for_messages:
 *                          Function name for too-many-arguments/end-of-params message
 * @param: flags:           Set of `TPP_LEXER_SEEK_RPAREN_FLAG_*'
 *
 * @return: TPP_TOK_EOF:         EOF was encountered before an unmatched ')' was found
 * @return: TPP_TOK_RPAREN:      Unmatched closing ')' was encountered
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seek_rparen_ex(tpp_lexer *tpp_restrict self,
                         tpp_char const **tpp_restrict p_pos,
                         tpp_lexer_arginfo *tpp_restrict p_argv,
                         tpp_size *tpp_restrict p_argc,
                         char const *opt_function_name_for_messages,
                         unsigned int flags,
                         tpp_token_id lparen_kind)
#else /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seek_rparen(tpp_lexer *tpp_restrict self,
                      tpp_char const **tpp_restrict p_pos,
                      tpp_lexer_arginfo *tpp_restrict p_argv,
                      tpp_size *tpp_restrict p_argc,
                      char const *opt_function_name_for_messages,
                      unsigned int flags)
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN_EX */
{
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
#define tpp_lexer_seek_rparen_keepspace() (flags & TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC)
#else /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */
#define tpp_lexer_seek_rparen_keepspace() (TPP_HAVE_MACRO_ARGUMENT_WHITESPACE != 0)
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE >= 0 */

	tpp_lexer_arginfo_rel *const p_argv_rel = (tpp_lexer_arginfo_rel *)p_argv;
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_size rel_start = tpp_file_ptr2rel(file, *p_pos);
	tpp_token_id tok;
	tpp_size const argv_bufsize = *p_argc;
	tpp_size i, argc = 0;
	tpp_size current_arg_rel_start;
	tpp_size current_arg_rel_end;
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
	/* Recursively nested ( ), [ ], { }, < > -pairs (in that order)
	 * The recursion level at any index can only be altered if all
	 * recursion levels with lower indices are set to 0:
	 * >> #define ARR[x] #x
	 * >> ARR[foo(])bar([)baz]
	 * Expands to 1 token:  "foo(])bar([)baz"
	 * rather than:         "foo("   )   bar   (   [   )   baz   ]   ) */
	tpp_ssize recursion[4] = { 0, 0, 0, 0 };
	unsigned int const tpp_recursion_cc = tpp_get_recursion_cc(lparen_kind);
#else /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
	tpp_ssize recursion[1] = { 0 }; /* Recursively nested ( )-pairs */
#define tpp_recursion_cc TPP_RECURSION_CC_PAREN
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN_EX */
	tpp_assert(p_pos != &file->tf_pos && "This would allow for file-popping and chunk deallocation, "
	                                     "neither of which are compatible with loading an argument "
	                                     "list");
	current_arg_rel_start = rel_start;
	current_arg_rel_end   = current_arg_rel_start;

	/* Yield first token. */
again_yield_and_switch_tok:
	tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
	switch (tok) {

	case TPP_TOK_EOF: {
#if TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST
		tpp_errno error;
		error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
		                           TPP_W_EOF_IN_ARGUMENT_LIST,
		                           opt_function_name_for_messages);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto done_err;
		}
#endif /* TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST */
		goto done;
	}	break;

	case TPP_TOK_SPACE:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT {
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
		if (tpp_lexer_seek_rparen_keepspace())
			break; /* When whitespace should be kept: treat it like a regular token */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0
		if (current_arg_rel_start == current_arg_rel_end) {
			/* Skip leading whitespace... */
			current_arg_rel_start = tpp_file_ptr2rel(file, *p_pos);
			current_arg_rel_end   = current_arg_rel_start;
			goto again_yield_and_switch_tok;
		}
		goto again_yield_and_switch_tok;
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0 */
	}	break;

	case '(':
		++recursion[0];
		break;

	case ')':
		if (recursion[TPP_RECURSION_CC_PAREN] == 0) {
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
			if (tpp_recursion_cc == TPP_RECURSION_CC_PAREN)
#endif /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
			{
				goto done;
			}
		}
		--recursion[TPP_RECURSION_CC_PAREN];
		break;

	case ',': {
		/* Make sure that we're not inside of a nested ( )-pair */
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_ANGLE] != 0)
			break;
#endif /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */

#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
		if (tpp_lexer_seek_rparen_keepspace()) {
			/* Argument must includes whitespace preceding the ","-token */
			tpp_char const *comma_start = tpp_lexer_gettoken(self)->tt_start;
			current_arg_rel_end = tpp_file_ptr2rel(file, comma_start);
		}
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */

		/* Write next argument */
		if (argc < argv_bufsize) {
			p_argv_rel[argc].tlair_start = current_arg_rel_start;
			p_argv_rel[argc].tlair_end   = current_arg_rel_end;
		}
		++argc;
		current_arg_rel_start = tpp_file_ptr2rel(file, *p_pos);
	}	break;

#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
	case '[':
		if (tpp_recursion_cc < TPP_RECURSION_CC_BRACKET)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		++recursion[TPP_RECURSION_CC_BRACKET];
		break;
	case ']':
		if (tpp_recursion_cc < TPP_RECURSION_CC_BRACKET)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] == 0)
			goto done;
		--recursion[TPP_RECURSION_CC_BRACKET];
		break;

	case '{':
		if (tpp_recursion_cc < TPP_RECURSION_CC_BRACE)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		++recursion[TPP_RECURSION_CC_BRACE];
		break;
	case '}':
		if (tpp_recursion_cc < TPP_RECURSION_CC_BRACE)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACE] == 0)
			goto done;
		--recursion[TPP_RECURSION_CC_BRACE];
		break;

#if (TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE ||              \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE ||       \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS)
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
		/* Convert to "<" token */
		tpp_assert(tpp_lexer_gettoken(self)->tt_start < *p_pos);
		tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '<');
		*p_pos = tpp_lexer_gettoken(self)->tt_start + 1;
/*		tok = TPP_TOK_OFCHAR('<');             * Not necessary */
/*		tpp_lexer_gettoken(self)->tt_id = tok; * Not necessary */
		goto handle_langle;
#define WANT_handle_langle
		break;
#endif /* ... */



#if (TPP_HAVE_TPP_TOK_RANGLE_LANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||         \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||  \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
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
		/* Convert to ">" token */
		tpp_assert(tpp_lexer_gettoken(self)->tt_start < *p_pos);
		tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '>');
		*p_pos = tpp_lexer_gettoken(self)->tt_start + 1;
		tok = TPP_TOK_OFCHAR('>');
		tpp_lexer_gettoken(self)->tt_id = tok;
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
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
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
		/* Convert to 1-char token */
		tpp_assert(tpp_lexer_gettoken(self)->tt_start < *p_pos);
		*p_pos = tpp_lexer_gettoken(self)->tt_start + 1;
/*		tok = TPP_TOK_OFCHAR(tpp_lexer_gettoken(self)->tt_start[0]); * Not necessary */
/*		tpp_lexer_gettoken(self)->tt_id = tok;                       * Not necessary */
		break;
#endif /* ... */

	case '<':
#ifdef WANT_handle_langle
#undef WANT_handle_langle
handle_langle:
#endif /* WANT_handle_langle */
		if (tpp_recursion_cc < TPP_RECURSION_CC_ANGLE)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		++recursion[TPP_RECURSION_CC_ANGLE];
		break;

	case '>':
#ifdef WANT_handle_rangle
#undef WANT_handle_rangle
handle_rangle:
#endif /* WANT_handle_rangle */
		if (tpp_recursion_cc < TPP_RECURSION_CC_ANGLE)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_ANGLE] == 0)
			goto done;
		--recursion[TPP_RECURSION_CC_ANGLE];
		break;

#endif /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */


	default:
		if (TPP_TOK_ISERR(tok))
			goto done_err;
		break;
	}
	current_arg_rel_end = tpp_file_ptr2rel(file, *p_pos);
	goto again_yield_and_switch_tok;

done:
	if (argc || (current_arg_rel_end > current_arg_rel_start)) {
		/* Write last argument */
		if (argc < argv_bufsize) {
			p_argv_rel[argc].tlair_start = current_arg_rel_start;
			p_argv_rel[argc].tlair_end   = current_arg_rel_end;
		} else if (argv_bufsize) {
			/* Update end-pointer of last argument (for varargs) */
			p_argv_rel[argv_bufsize - 1].tlair_end = current_arg_rel_end;
		}
		++argc;
	}
#if TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS
	if (argc > argv_bufsize && !(flags & TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS)) {
		tpp_errno error;
		tpp_size warning_pos = rel_start;
		if (argv_bufsize) {
			warning_pos = p_argv_rel[argv_bufsize - 1].tlair_start;
			/* XXX: "warning_pos" here points at the start of the last in-bounds
			 *      argument, but for optimal effect, this warning should probably
			 *      point at the ","-token *AFTER* the last in-bounds argument... */
		}
		error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, warning_pos),
		                           TPP_W_TOO_MANY_ARGUMENTS,
		                           opt_function_name_for_messages,
		                           (unsigned int)argv_bufsize,
		                           (unsigned int)argc);
		if (TPP_ISERR(error))
			tok = TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS */
done_err:
	*p_argc = argc;
	if (argc > argv_bufsize)
		argc = argv_bufsize;
	for (i = 0; i < argc; ++i) {
		p_argv[i].tlai_start = tpp_file_rel2ptr(file, p_argv_rel[i].tlair_start);
		p_argv[i].tlai_end   = tpp_file_rel2ptr(file, p_argv_rel[i].tlair_end);
	}
	return tok;
#undef tpp_recursion_cc
#undef tpp_lexer_seek_rparen_keepspace
}

#endif /* TPP_HAVE_LEXER_SEEK_RPAREN */

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
	tpp_size            mab_n_vaopt;     /* Amount of extra bytes inserted when varargs are given (if: tpp_lexer_seek_rparen:OUT(*p_argc) > tmf_argc). */
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
		if (!tpp_lexer_getext(self, TPP_EXT_VA_ARGS_IN_MACROS))
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
	    tpp_lexer_getext(self, TPP_EXT_NAMED_VARARGS_IN_MACROS)) {
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
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		return TPP_EOK;
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


#if TPP_HAVE_TRADITIONAL_MACROS != 0
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
		tok = tpp_lexer_yieldraw_at(self, &body_iter);
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
	tpp_lexer_state_push(self, ~0, TPP_LEXER_STATE_FLAG_NOWARNINGS);
	result = tpp_macro_builder_compile_traditional_impl(builder, self, body_start, body_end);
	tpp_lexer_state_pop(self, ~0, TPP_LEXER_STATE_FLAG_NOWARNINGS);
	return result;
}
#else /* TPP_HAVE_WARNINGS */
#define tpp_macro_builder_compile_traditional(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_traditional_impl(builder, self, body_start, body_end)
#endif /* !TPP_HAVE_WARNINGS */
#endif /* TPP_HAVE_TRADITIONAL_MACROS != 0 */

#if TPP_HAVE_TRADITIONAL_MACROS <= 0
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
		tok = tpp_lexer_yieldraw_at(self, &body_iter);
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
			if (!tpp_lexer_getext(self, TPP_EXT_GLUE_MACRO_ARGUMENT))
				break;
			/* Consume any whitespace that might following this operator.
			 * Whitespace preceding it is automatically consumed because
			 * we skip all not-already-flushed data after "last_non_space_end" */
			do {
				tok = tpp_lexer_yieldraw_at(self, &body_iter);
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
				tok = tpp_lexer_yieldraw_at(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);

			/* Check if there'll be another ##-operator after the argument */
			if (tok == TPP_TOK_POUND_POUND) {
				do {
					tok = tpp_lexer_yieldraw_at(self, &body_iter);
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
			if (!tpp_lexer_getext(self, TPP_EXT_VA_GLUE_COMMA_IN_MACROS))
				break;
			if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
				break;
			start_of_comma = token->tt_start;
			do {
				tok = tpp_lexer_yieldraw_at(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (tok != TPP_TOK_POUND_POUND)
				goto again_switch_tok;
			do {
				tok = tpp_lexer_yieldraw_at(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (!TPP_TOK_ISKEYWORD(tok)) {
				/* Implement regular glue (simply deleting whitespace), or do nothing */
handle_not_varargs_argument_after_comma_glue:
#if TPP_HAVE_GLUE_MACRO_ARGUMENT
				if (tpp_lexer_getext(self, TPP_EXT_GLUE_MACRO_ARGUMENT)) {
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
			if (!tpp_lexer_getext(self, TPP_EXT_VA_COMMA_IN_MACROS))
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
			if (!tpp_lexer_getext(self, TPP_EXT_VA_NARGS_IN_MACROS))
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
			if (!tpp_lexer_getext(self, TPP_EXT_VA_OPT_IN_MACROS))
				goto handle_keyword;
#define WANT_handle_keyword
			start_of_va_opt = token->tt_start;

			/* Next token must be ( */
			do {
				tok = tpp_lexer_yieldraw_at(self, &body_iter);
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
				tok = tpp_lexer_yieldraw_at(self, &body_iter);
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
			if (!tpp_lexer_getext(self, TPP_EXT_STRINGIZE_MACRO_ARGUMENT) &&
			    !tpp_lexer_getext(self, TPP_EXT_CHARIZE_MACRO_ARGUMENT) &&
			    !tpp_lexer_getext(self, TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT))
				break;
			start_of_pound = token->tt_start;
			do {
				tok = tpp_lexer_yieldraw_at(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);

#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
			if (tok == '!') {
				last_non_space_end = body_iter;
				if (!tpp_lexer_getext(self, TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT))
					break;
				do {
					tok = tpp_lexer_yieldraw_at(self, &body_iter);
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
				if (!tpp_lexer_getext(self, TPP_EXT_CHARIZE_MACRO_ARGUMENT))
					break;
				opcode = TPP_MACRO_OPCODE_INS_CHR;
				do {
					tok = tpp_lexer_yieldraw_at(self, &body_iter);
				} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
			} else
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
			{
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
				if (!tpp_lexer_getext(self, TPP_EXT_STRINGIZE_MACRO_ARGUMENT))
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
				tok = tpp_lexer_yieldraw_at(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (tok == '(') {
				last_non_space_end = body_iter;
				do {
					tok = tpp_lexer_yieldraw_at(self, &body_iter);
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
			if (tpp_lexer_getext(self, TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR)) {
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
				if (tpp_lexer_getext(self, TPP_EXT_GLUE_MACRO_ARGUMENT)) {
					/* Must seek ahead to the next non-whitespace token. if it's the
					 * ##-operator, then we have to insert the argument *WITHOUT* it
					 * being expanded! */
					tpp_char const *argument_end = body_iter;
					do {
						tok = tpp_lexer_yieldraw_at(self, &body_iter);
					} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
					if (TPP_TOK_ISERR(tok))
						return TPP_TOK_ASERR(tok);

					/* Append opcodes to insert argument */
					if (tok == TPP_TOK_POUND_POUND) {
						do {
							tok = tpp_lexer_yieldraw_at(self, &body_iter);
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
#if TPP_HAVE_GLUE_MACRO_ARGUMENT <= 0
					/* Append opcodes to insert argument */
					tpp_macro_builder_append_ins_exp(err_nomem, builder, arg,
					                                 (tpp_size)(body_iter - body_start));

					/* Remember that input body text has been
					 * flushed until the end of the keyword. */
					body_start = body_iter;
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT <= 0 */
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
#endif /* TPP_HAVE_TRADITIONAL_MACROS <= 0 */


#if TPP_HAVE_TRADITIONAL_MACROS < 0
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile(tpp_macro_builder *tpp_restrict builder,
                          tpp_lexer *tpp_restrict self,
                          tpp_char const *body_start,
                          tpp_char const *body_end) {
	if (tpp_lexer_getext(self, TPP_EXT_TRADITIONAL_MACROS))
		return tpp_macro_builder_compile_traditional(builder, self, body_start, body_end);
	return tpp_macro_builder_compile_modern(builder, self, body_start, body_end);
}
#elif TPP_HAVE_TRADITIONAL_MACROS == 0
#define tpp_macro_builder_compile(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_modern(builder, self, body_start, body_end)
#else /* TPP_HAVE_TRADITIONAL_MACROS > 0 */
#define tpp_macro_builder_compile(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_traditional(builder, self, body_start, body_end)
#endif /* TPP_HAVE_TRADITIONAL_MACROS... */

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
		if (tpp_lexer_getext(self, TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS))
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
		while (!TPP_TOK_ISLF_OR_COMMENT(tok) && tok != TPP_TOK_EOF) {
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
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
	if (tpp_lexer_getext(self, TPP_EXT_MACRO_ARGUMENT_WHITESPACE))
		builder.mab_flags |= TPP_MACRO_FLAG_KEEPARGSPC;
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */
#if TPP_HAVE_MACRO_RECURSION < 0
	if (tpp_lexer_getext(self, TPP_EXT_MACRO_RECURSION))
		builder.mab_flags |= TPP_MACRO_FLAG_SELFEXPAND;
#endif /* TPP_HAVE_MACRO_RECURSION < 0 */
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
	while (tok != TPP_TOK_EOF && !TPP_TOK_ISLF_OR_COMMENT(tok)) {
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
	tpp_file_pusheof_fast(file, body_end); /* This is needed by macro compilers */
	error = tpp_macro_builder_compile(&builder, self, body_start, body_end);
	tpp_file_popeof_fast(file);
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


#if TPP_HAVE_PRAGMA
/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword
 * @return: TPP_ENOENT: Unknown pragma (warning was already emitted; caller
 *                      should seek until after macro) */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);

#if TPP_HAVE_PRAGMA_PUSH_MACRO
struct tpp_lexer_handle_pushpopmacro_data {
	tpp_lexer *tlhppmd_lexer; /* [1..1] Lexer */
	bool       tlhppmd_push;  /* True if "push_macro", false if "pop_macro" */
};

static tpp_errno TPPCALL
tpp_lexer_handle_pushpopmacro_cb(void *arg, tpp_char const *str, tpp_size length) {
	tpp_errno result;
	struct tpp_lexer_handle_pushpopmacro_data *data;
	tpp_keyword const *ro_keyword;
	tpp_keyword *keyword;
	tpp_hash hash = tpp_hashof(str, length);
	data = (struct tpp_lexer_handle_pushpopmacro_data *)arg;

	/* Load keyword */
	ro_keyword = tpp_keywords_newkeyword(&data->tlhppmd_lexer->tl_kwds, str, length, hash);
	if tpp_unlikely(!ro_keyword)
		goto err_nomem;

	/* Make keyword writable */
	keyword = tpp_keywords_copybuiltin(&data->tlhppmd_lexer->tl_kwds, ro_keyword);
	if tpp_unlikely(!keyword)
		goto err_nomem;

	/* Push/pop the macro linked to this keyword. */
	if (data->tlhppmd_push)
		return tpp_keyword_pushmacro(keyword);

	result = tpp_keyword_popmacro(keyword);
	tpp_assert(!TPP_ISERR(result) ||
	           result == TPP_ENOENT);
	if (result == TPP_ENOENT) {
		/* Emit a warning */
#if TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK
		result = tpp_lexer_warnf(data->tlhppmd_lexer, TPP_W_POP_MACRO_EMPTY_STACK,
		                         (unsigned int)length, str);
#else /* TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
		result = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
	}
	return result;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_token_id tok = token->tt_id;
	switch (tok) {

#if TPP_HAVE_PRAGMA_PUSH_MACRO
	case TPP_KWD_push_macro:
	case TPP_KWD_pop_macro: {
		tpp_errno error;
		struct tpp_lexer_handle_pushpopmacro_data data;
		data.tlhppmd_lexer = self;
		data.tlhppmd_push  = tok == TPP_KWD_push_macro;
		if (!tpp_lexer_getext(self, TPP_EXT_PRAGMA_PUSH_MACRO))
			goto unknown_pragma;
		tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);

		/* Skip leading '(' */
		tok = tpp_lexer_skip_blocking(self, TPP_TOK_OFCHAR('('));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (TPP_TOK_ISSTRING(tok)) {
			/* Parse+process string (using "tpp_lexer_parsestring_cb()") */
			error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_pushpopmacro_cb,
			                                 &data, TPP_LEXER_PARSESTRING_FLAG_STOPONLF);
		} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			error = TPP_EOK;;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
		}
		if (TPP_ISERR(error))
			return error;

		/* Skip trailing ')' */
		tok = tpp_lexer_skip_blocking(self, TPP_TOK_OFCHAR(')'));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}	break;
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_PRAGMA_ONCE
	case TPP_KWD_once: {
		tpp_file const *iofile;
		tpp_keyword *iofile_kwd;
		if (!tpp_lexer_getext(self, TPP_EXT_PRAGMA_ONCE))
			goto unknown_pragma;
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
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}	break;
#endif /* TPP_HAVE_PRAGMA_ONCE */

#if TPP_HAVE_PRAGMA_DEPRECATED
	/* TODO: #pragma deprecated("foo") */
#endif /* TPP_HAVE_PRAGMA_DEPRECATED */

#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
	/* TODO: #pragma tpp_set_keyword_flags("foo", 0x7f) */
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

#if TPP_HAVE_PRAGMA_EXTENSION
	/* TODO: #pragma extension(...) */
#endif /* TPP_HAVE_PRAGMA_EXTENSION */

#if TPP_HAVE_PRAGMA_EXTENSION_PUSH
	/* TODO: #pragma extension(push) */
#endif /* TPP_HAVE_PRAGMA_EXTENSION_PUSH */

#if TPP_HAVE_PRAGMA_WARNING
	/* TODO: #pragma warning(...) */
#endif /* TPP_HAVE_PRAGMA_WARNING */

#if TPP_HAVE_PRAGMA_WARNING_PUSH
	/* TODO: #pragma warning(push) */
#endif /* TPP_HAVE_PRAGMA_WARNING_PUSH */

#if TPP_HAVE_PRAGMA_TPP_EXEC
	/* TODO: #pragma tpp_exec("...") */
#endif /* TPP_HAVE_PRAGMA_TPP_EXEC */

#if TPP_HAVE_PRAGMA_MESSAGE
	/* TODO: #pragma message("...") */
#endif /* TPP_HAVE_PRAGMA_MESSAGE */

#if TPP_HAVE_PRAGMA_ERROR
	/* TODO: #pragma error("...") */
#endif /* TPP_HAVE_PRAGMA_ERROR */

#if TPP_HAVE_PRAGMA_REGION
	/* TODO: #pragma region,  #pragma endregion */
#endif /* TPP_HAVE_PRAGMA_REGION */

#if TPP_HAVE_PRAGMA_GCC_POISON
	/* TODO: #pragma GCC poison foo */
#endif /* TPP_HAVE_PRAGMA_GCC_POISON */

#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
	/* TODO: #pragma GCC system_header */
#endif /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

#if TPP_HAVE_PRAGMA_TPP_WARNING
	/* TODO: #pragma TPP warning(...)  (same as TPP_HAVE_PRAGMA_WARNING) */
#endif /* TPP_HAVE_PRAGMA_TPP_WARNING */

#if TPP_HAVE_PRAGMA_TPP_EXTENSION
	/* TODO: #pragma TPP extension(...)  (same as TPP_HAVE_PRAGMA_EXTENSION) */
#endif /* TPP_HAVE_PRAGMA_TPP_EXTENSION */

#if TPP_HAVE_PRAGMA_TPP_TPP_EXEC
	/* TODO: #pragma TPP tpp_exec(...)  (same as TPP_HAVE_PRAGMA_TPP_EXEC) */
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_EXEC */

#if TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
	/* TODO: #pragma TPP tpp_set_keyword_flags("foo", 0x7f)  (same as TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */

#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
	/* TODO: #pragma TPP include_path(...) */
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */

	default: goto unknown_pragma;
	}
	return TPP_EOK;
#if TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS
	{
		tpp_errno error;
unknown_pragma:
		error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_PRAGMAS);
		if (!TPP_ISERR(error))
			error = TPP_ENOENT;
		return error;
	}
#else /* TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
unknown_pragma:
	return TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
}

/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_pragma_directive(tpp_lexer *tpp_restrict self) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_errno error = tpp_lexer_process_pragma(self);
	if (TPP_ISERR(error)) {
		if (error == TPP_ENOENT)
			goto skip_garbage_without_warning;
		return TPP_TOK_OFERR(error);
	}
	while (TPP_TOK_ISSPACE_OR_COMMENT(token->tt_id)) {
		tpp_token_id tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
	}
	if (TPP_TOK_ISLF_OR_COMMENT(token->tt_id))
		return TPP_TOK_EOF;
	if (token->tt_id == TPP_TOK_EOF)
		return TPP_TOK_EOF;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
	error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE);
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE */
skip_garbage_without_warning:
	for (;;) {
		tpp_token_id tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
		if (TPP_TOK_ISLF_OR_COMMENT(tok))
			break;
	}
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_PRAGMA */

#if TPP_HAVE_CPP_DEFINE
/* Handle a "#define" directive, with "self" pointing at the macro's name-keyword
 * @return: TPP_TOK_ISERR: Error
 * @return: TPP_TOK_EOF: Success; caller should yield the next raw token */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_define_directive(tpp_lexer *tpp_restrict self);
#endif /* TPP_HAVE_CPP_DEFINE */


/* Process a preprocessor directive, with the currently loaded token being the leading '#'
 * Upon successful return (!TPP_TOK_ISERR(return)), the caller will yield another raw token
 * @return: TPP_TOK_ISERR         : Error
 * @return: TPP_TOK_EOF           : Caller should yield the next raw token
 * @return: TPP_TOK_SHELL_COMMENT : Directive was transformed to a shell-comment which the caller should re-emit */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_directive(tpp_lexer *tpp_restrict self) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token_id result;
	tpp_char const *directive_iter;
	tpp_assert(token->tt_id == '#');
	directive_iter = token->tt_end;
	file->tf_pos = token->tt_start; /* Make sure that the start of the directive remains loaded. */

	/* Load token that comes after leading '#' */
again_yield_directive_iter:
	result = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
	switch (result) {

#if TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		/* Skip over multi-line comment tokens to allow code like:
		 * >> #(*
		 * >> comment
		 * >> *)define foo 42 */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE */
	case TPP_TOK_SPACE:
		goto again_yield_directive_iter;

/************************************************************************/
#if TPP_HAVE_CPP_BLANK
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE
	TPP_CASE_TPP_TOK_COMMENT_LINE
		/* line-like comment tokens must be treated as the "blank" directive,
		 * since they effective cap-off the directive via a commented line-feed */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE */
	case TPP_TOK_EOF:
	case TPP_TOK_LF:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_BLANK))
			goto handle_unknown_directive;
		break;
#endif /* TPP_HAVE_CPP_BLANK */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EXCLAIM
	case '!':
		if (!tpp_lexer_getext(self, TPP_EXT_CPP_EXCLAIM))
			goto handle_unknown_directive;
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_EXCLAIM */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIGIT_LINE
	case TPP_TOK_INT:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DIGIT_LINE))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_DIGIT_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_LINE
	case TPP_KWD_line:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_LINE))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT
	{
#if TPP_HAVE_CPP_INCLUDE
		if (0) {
	case TPP_KWD_include:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_INCLUDE))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_INCLUDE */
#if TPP_HAVE_CPP_INCLUDE_NEXT
		if (0) {
	case TPP_KWD_include_next:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_INCLUDE_NEXT))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
#if TPP_HAVE_CPP_IMPORT
		if (0) {
	case TPP_KWD_import:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_IMPORT))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_IMPORT */

		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}
#endif /* TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
	case TPP_KWD_if:
	case TPP_KWD_elif:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO: -Wundef */
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line

	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO: -Wheader-guard */
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line

	case TPP_KWD_else:
	case TPP_KWD_endif:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO: -Wno-endif-labels */
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DEFINE
	case TPP_KWD_define: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DEFINE))
			goto handle_unknown_directive;
		token->tt_end = directive_iter;
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
			goto seek_end_of_line;
#define WANT_seek_end_of_line
		}
		return tpp_lexer_process_define_directive(self);
	}	break;

	case TPP_KWD_undef: {
		tpp_keyword const *ro_keyword;
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DEFINE))
			goto handle_unknown_directive;
		token->tt_end = directive_iter;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (!TPP_TOK_ISKEYWORD(result)) {
#if TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE, "undef");
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
			goto seek_end_of_line;
#define WANT_seek_end_of_line
		}

		/* Delete keyword definition */
		ro_keyword = tpp_lexer_gettoken(self)->tt_kwd;
		if (ro_keyword->tk_macro) {
			tpp_keyword *keyword = tpp_keywords_copybuiltin(&self->tl_kwds, ro_keyword);
			if tpp_unlikely(!keyword)
				return TPP_TOK_ENOMEM;
			tpp_assert(keyword->tk_macro);
			tpp_macro_decref(keyword->tk_macro);
			keyword->tk_macro = NULL;
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
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (TPP_TOK_ISLF_OR_COMMENT(result))
			break;
		if (result == TPP_TOK_EOF)
			break;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		{
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, "undef");
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}	break;
#endif /* TPP_HAVE_CPP_DEFINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ASSERT
	case TPP_KWD_assert:
	case TPP_KWD_unassert:
		if (!tpp_lexer_getext(self, TPP_EXT_CPP_ASSERT))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_ASSERT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
	{
		tpp_size rel_token_start;
		tpp_size rel_message_start;
		tpp_size rel_message_end;
		tpp_char const *message_start;
		tpp_char const *message_end;
		tpp_size message_size;
		tpp_errno error;
#if TPP_HAVE_CPP_ERROR
		if (0) {
	case TPP_KWD_error:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_ERROR))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
		if (0) {
	case TPP_KWD_warning:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_WARNING))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_WARNING */
		rel_token_start   = tpp_file_ptr2rel(file, token->tt_start);
		rel_message_start = tpp_file_ptr2rel(file, directive_iter);
		error = tpp_lexer_seek_eol(self, &directive_iter tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
		rel_message_end = tpp_file_ptr2rel(file, directive_iter);
		token->tt_start = tpp_file_rel2ptr(file, rel_token_start);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);

		/* Load range of message string. */
		message_start = tpp_file_rel2ptr(file, rel_message_start);
		message_end   = tpp_file_rel2ptr(file, rel_message_end);

		/* Remember that this is where the next token should begin. */
		token->tt_end = directive_iter;

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
		switch (result) {
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
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		break;
	}
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IDENT_SCCS
	case TPP_KWD_ident:
	case TPP_KWD_sccs:
		if (!tpp_lexer_getext(self, TPP_EXT_CPP_IDENT_SCCS))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_IDENT_SCCS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_PRAGMA
	case TPP_KWD_pragma: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_PRAGMA))
			goto handle_unknown_directive;
		token->tt_end = directive_iter;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		return tpp_lexer_process_pragma_directive(self);
	}	break;
#endif /* TPP_HAVE_CPP_PRAGMA */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EMBED
	case TPP_KWD_embed: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_EMBED))
			goto handle_unknown_directive;
		/* TODO: #embed  (https://en.cppreference.com/c/preprocessor/embed) */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}	break;
#endif /* TPP_HAVE_CPP_EMBED */
/************************************************************************/



	default:
		if (TPP_TOK_ISERR(result))
			return result;
handle_unknown_directive:
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SHELL_COMMENT)
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
				return TPP_TOK_OFERR(error);
			}
			token->tt_end = eol;
			token->tt_id = TPP_TOK_SHELL_COMMENT;

			/* Tell caller to re-emit what we thought was a directive as a shell comment */
			return TPP_TOK_SHELL_COMMENT;
		} else
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		{
#if defined(WANT_seek_end_of_line) || TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0
#if TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_DIRECTIVE);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE */
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0 */
	
			/* Seek until we hit LF or EOF. Caller has disabled "autopopfile",
			 * so this'll always stay within the file currently being processed. */
#ifdef WANT_seek_end_of_line
#undef WANT_seek_end_of_line
seek_end_of_line:
#endif /* WANT_seek_end_of_line */
			while (!TPP_TOK_ISLF_OR_COMMENT(result) && result != TPP_TOK_EOF) {
				result = tpp_lexer_yieldraw_blocking(self);
				if (TPP_TOK_ISERR(result))
					break;
			}
#endif /* WANT_seek_end_of_line || TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0 */
		}
		break;
	}
	return TPP_TOK_EOF;
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
#if TPP_HAVE_TPP_TOK_SPACE <= 0 /* Always, or conditionally disabled */
	case TPP_TOK_SPACE:
#if TPP_HAVE_TPP_TOK_SPACE < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SPACE))
			break; /* Enabled */
#endif /* TPP_HAVE_TPP_TOK_SPACE < 0 */
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
			tpp_lexer_getfeat(self, TPP_FEAT_CPP_DIRECTIVES)) {
			tpp_token *const token = tpp_lexer_gettoken(self);

			/* Must re-parse comment as a preprocessor directive instead! */
			token->tt_id  = (tpp_token_id)'#';
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

			tpp_lexer_autopopfile_pushoff(self);
			result = tpp_lexer_process_directive(self);
			tpp_lexer_autopopfile_pop(self);
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
#if TPP_HAVE_TPP_TOK_COMMENT <= 0 /* Always, or conditionally disabled */
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_ASM_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
		TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES */
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
#if TPP_HAVE_TPP_TOK_COMMENT < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COMMENT))
			break; /* Comments are enabled -> emit to caller */
#endif /* TPP_HAVE_TPP_TOK_COMMENT < 0 */
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
#if TPP_HAVE_CPP_DIRECTIVES || TPP_HAVE_TPP_TOK_LF <= 0 /* Always, or conditionally disabled */
	case TPP_TOK_LF: {
#if TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#if TPP_HAVE_TPP_TOK_LF <= 0 /* Always, or conditionally disabled */
#if TPP_HAVE_TPP_TOK_LF < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LF))
			break; /* Enabled */
#endif /* TPP_HAVE_TPP_TOK_LF < 0 */
		goto again;
#else /* TPP_HAVE_TPP_TOK_LF <= 0 */
		break;
#endif /* TPP_HAVE_TPP_TOK_LF > 0 */
	}
#endif /* TPP_HAVE_CPP_DIRECTIVES || TPP_HAVE_TPP_TOK_LF <= 0 */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIRECTIVES
	case '#':
		if (self->tl_state & TPP_LEXER_STATE_FLAG_NODIRECTIVES)
			break; /* Not allowed here... */
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DIRECTIVES))
			break; /* Directives are disabled. */
		tpp_lexer_autopopfile_pushoff(self);
		result = tpp_lexer_process_directive(self);
		tpp_lexer_autopopfile_pop(self);
		if (TPP_TOK_ISERR(result))
			break;
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (result != TPP_TOK_EOF) {
			/* Emit as a shell-comment (if enabled). Otherwise, check next raw token. */
			tpp_assert(result == TPP_TOK_SHELL_COMMENT);
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COMMENT))
				break;
		}
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		goto again;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
/************************************************************************/



/************************************************************************/
	default:
#if TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen something that will prevent CPP directives */
		self->tl_state |= TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
		break;
/************************************************************************/
	}
	return result;
}


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-yield.c                                            */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS

TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_PAREN) == TPP_TOK_LPAREN);
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_BRACKET) == TPP_TOK_LBRACKET);
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_BRACE) == TPP_TOK_LBRACE);
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_ANGLE) == TPP_TOK_LANGLE);
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

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
	 *    change anything about it. */
	(void)lexer;
	/* TODO: New lexer state flag that force-enables emission of all tokens */
	/* TODO */
	self->tmei_expand_data = (tpp_char *)arginfo->tlai_start;
	self->tmei_expand_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
	return TPP_EOK;
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
tpp_macro_require_argbuf(tpp_macro *tpp_restrict macro) {
	tpp_macro_argbuf *result = macro->tm_data.tmd_func.tmf_argbuf;
	if (result == NULL) {
		tpp_size size = tpp_macro_argbuf_sizeof(macro->tm_data.tmd_func.tmf_argc);
		result = (tpp_macro_argbuf *)tpp_malloc(size);
		macro->tm_data.tmd_func.tmf_argbuf = result;
	}
	return result;
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
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_expand_macro_function(tpp_lexer *tpp_restrict self,
                                tpp_macro *tpp_restrict macro) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_lexer_seek_backup backup;
	tpp_char const *pos;
	tpp_token_id tok;
	tpp_size i, argc;    /* # of arguments given in-source during invocation */
	tpp_size macro_argc; /* == macro->tm_data.tmd_func.tmf_argc */
	tpp_macro_argbuf *argbuf;
	tpp_lexer_arginfo *invoke_arginfo; /* == argbuf->tmab_arginfo */
	tpp_macro_expinfo *invoke_expinfo; /* == argbuf->tmab_expinfo */
	tpp_string *result_chunk;
	tpp_file *prev_file;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	char va_nargs[TPP_UTOA_MAXLEN]; /* Value for __VA_NARGS__ */
	tpp_size va_nargs_len = 0; /* XXX: Initialization doesn't matter; only here to shut up compiler warnings */
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
	tpp_size result_chunk_size;
	tpp_assert(TPP_MACRO_KIND_ISFUNC(macro->tm_kind));

	/* Seek ahead to find the '('-token expected by the macro. */
	pos = tpp_lexer_seek_begin(self, &backup, false);
	do {
		tok = tpp_lexer_yieldraw_at(self, &pos);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		goto err_rollback;

	if (tok == TPP_MACRO_KIND_ASTOK(macro->tm_kind)) {
		/* Got the opening parenthesis token! */
	} else
#if (TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS && \
     (TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||         \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE ||        \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL ||  \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL))
	if (macro->tm_kind == TPP_MACRO_KIND_FUNC_ANGLE) {
		/* Check for multi-char tokens that start with '<' */
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
			/* Convert to "<" token */
			tpp_assert(tpp_lexer_gettoken(self)->tt_start < pos);
			tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '<');
			pos = tpp_lexer_gettoken(self)->tt_start + 1;
			tok = TPP_TOK_OFCHAR('<');
			tpp_lexer_gettoken(self)->tt_id = tok;
			break;
		default: goto rollback;
		}
	} else
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */
	{
		goto rollback;
	}

	/* Load argument buffer of macro */
	argbuf = tpp_macro_require_argbuf(macro);
	if tpp_unlikely(!argbuf)
		goto err_rollback_nomem;
	argc = macro_argc = macro->tm_data.tmd_func.tmf_argc;
	invoke_arginfo = tpp_macro_argbuf_getarginfo(argbuf, argc);
	invoke_expinfo = tpp_macro_argbuf_getexpinfo(argbuf, argc);

	/* Load parameters of function-style macro */
	tok = tpp_lexer_seek_rparen_ex(self, &pos, invoke_arginfo, &argc,
	                               (char const *)backup.tlsb_kwd->tk_kwd,
	                               tpp_lexer_seek_rparen_flags_frommacro(macro),
	                               TPP_MACRO_KIND_ASTOK(macro->tm_kind));
	if (TPP_TOK_ISERR(tok))
		goto err_rollback;
	tpp_assert(macro_argc == macro->tm_data.tmd_func.tmf_argc);
	if (argc < macro_argc) {
		/* Too few arguments */

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
			error = tpp_lexer_warnf_at(self, pos, TPP_W_TOO_FEW_ARGUMENTS,
			                           (char const *)backup.tlsb_kwd->tk_kwd,
			                           (unsigned int)macro_argc,
			                           (unsigned int)argc);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				goto err_rollback;
			}
		}
#endif /* TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */
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
		}
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
	for (i = 0; i < macro_argc; ++i) {
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
		if (arg->tma_ins_exp) {
			tpp_errno error;
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			error = tpp_macro_expinfo_init(expand, arginfo, self);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				goto err_rollback_invoke_expinfo_i;
			}

			/* Account for expanded text */
			result_chunk_size += (arg->tma_ins_exp * tpp_macro_expinfo_getsize(expand));
		}

#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
		if (arg->tma_ins_str) {
			tpp_size raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
			tpp_size str_size = (tpp_size)tpp_token_encodestring(&tpp_count_printer, NULL,
			                                                     arginfo->tlai_start, raw_size);
/*			str_size += 2; * Account for leading/trailing " or ' characters -- Already account for during compilation */
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

	/* Allocate the perfectly-sized chunk that will describe the expanded macro's text */
	result_chunk = tpp_string_malloc(result_chunk_size);
	if tpp_unlikely(!result_chunk)
		goto err_rollback_invoke_expinfo_nomem;

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
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		if (arg->tma_ins_exp) {
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
			tpp_macro_expinfo_fini(expand, arginfo);
		}
	}

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
					goto rollback;
				}
			}
		} while ((iter = iter->tf_tprev) != NULL);
	}
#endif /* TPP_HAVE_MACRO_RECURSION */

	/* Set-up "result_chunk" such that it will read from "body-chunk-string" */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_rollback_result_chunk_nomem;
	*prev_file = *file;
	prev_file->tf_pos = pos; /* Override return-file to continue parsing after ')'-token */
	file->tf_pos   = tpp_string_str(result_chunk);
	file->tf_chunk = result_chunk; /* Inherit reference */
	file->tf_end   = tpp_string_end(result_chunk);
	_tpp_file_init_common(file);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	file->tf_kind  = TPP_FILE_KIND_MACRO;
#if TPP_HAVE_UNICODE
	file->tf_enc = macro->tm_body_enc;
#endif /* TPP_HAVE_UNICODE */
	file->tf_data.td_macro.tfm_macro = macro;
	tpp_macro_incref(macro);
	++macro->tm_expansions;
	return TPP_TOK_EOF;
rollback:
	return tpp_lexer_seek_rollback(self, &backup);
err_rollback_result_chunk_nomem:
	tok = TPP_TOK_ENOMEM;
/*err_rollback_result_chunk:*/
	tpp_string_destroy(result_chunk);
	goto err_rollback;
err_rollback_invoke_expinfo_nomem:
	tok = TPP_TOK_ENOMEM;
/*err_rollback_invoke_expinfo:*/
	i = macro_argc;
err_rollback_invoke_expinfo_i:
	while (i) {
		--i;
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		if (arg->tma_ins_exp) {
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
			tpp_macro_expinfo_fini(expand, arginfo);
		}
	}
err_rollback:
	tpp_lexer_seek_rollback(self, &backup);
	return tok;
err_rollback_nomem:
	tok = TPP_TOK_ENOMEM;
	goto err_rollback;
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
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
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

	_tpp_file_init_common(file);
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

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_push_textfile(tpp_lexer *tpp_restrict self,
                        tpp_char const *text, tpp_size textsize) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem;
	*prev_file = *file;

	file->tf_pos   = text;
	file->tf_chunk = NULL;
	file->tf_end   = text + textsize;
	_tpp_file_init_common(file);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	file->tf_kind  = TPP_FILE_KIND_TEXT;
#if TPP_HAVE_UNICODE
	file->tf_enc = TPP_FILE_ENCODING_FORCE_UTF8;
#endif /* TPP_HAVE_UNICODE */
	file->tf_data.td_text.tft_name = NULL;
	return TPP_TOK_EOF;
err_nomem:
	return TPP_TOK_ENOMEM;
}

/* Support for feature-test-style macros */
#undef TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
#if (TPP_HAVE_CLANG_MACRO___has_attribute ||          \
     TPP_HAVE_CLANG_MACRO___has_builtin ||            \
     TPP_HAVE_CLANG_MACRO___has_cpp_attribute ||      \
     TPP_HAVE_CLANG_MACRO___has_declspec_attribute || \
     TPP_HAVE_CLANG_MACRO___has_extension ||          \
     TPP_HAVE_CLANG_MACRO___has_feature ||            \
     TPP_HAVE_CLANG_MACRO___has_c_attribute ||        \
     TPP_HAVE_MACRO___is_identifier ||                \
     TPP_HAVE_MACRO___is_deprecated ||                \
     TPP_HAVE_MACRO___is_poisoned)
#define TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS 1
#else /* ... */
#define TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS 0
#endif /* !... */

#undef TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
#if (TPP_HAVE_MACRO___has_extension ||       \
     TPP_HAVE_MACRO___has_known_extension || \
     TPP_HAVE_MACRO___has_warning ||         \
     TPP_HAVE_MACRO___has_known_warning)
#define TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS 1
#else /* ... */
#define TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS 0
#endif /* !... */

#undef TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO
#define TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO \
	(TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS ||    \
	 TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS)

#if TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO

#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
struct tpp_lexer_handle_string_feature_test_data {
	tpp_lexer   *tlhsftd_lexer;        /* [1..1] Lexer */
	tpp_token_id tlhsftd_mode;         /* Feature-test mode */
	tpp_char     tlhsftd_expansion[1]; /* Desired expansion */
};

static tpp_errno TPPCALL
tpp_lexer_handle_string_feature_test_cb(void *arg, tpp_char const *str, tpp_size length) {
	struct tpp_lexer_handle_string_feature_test_data *data;
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
	tpp_lexer_seek_backup backup;
	tpp_char const *pos = tpp_lexer_seek_begin(self, &backup, false);
	tpp_token_id tok;
	unsigned int recursion;
#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
	struct tpp_lexer_handle_string_feature_test_data data;
#define tpp_feature_test_macro_expansion data.tlhsftd_expansion
#else /* TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
	tpp_char expansion[1];
#define tpp_feature_test_macro_expansion expansion
#endif /* !TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
again_yield:
	tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	if tpp_unlikely(TPP_TOK_ISERR(tok))
		goto err_tok;
	if (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		goto again_yield;
	if (tok != TPP_TOK_OFCHAR('('))
		goto rollback;

	/* Yield feature keyword */
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
		if tpp_unlikely(TPP_TOK_ISERR(tok))
			goto err_tok;
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));

	/* Default to expanding to "0" */
	tpp_feature_test_macro_expansion[0] = '0';

	/* Deal with special case of "__has_extension()" (which is overloaded for TPP) */
#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
	if (TPP_TOK_ISSTRING(tok)) {
		tpp_errno error;
#if TPP_HAVE_CLANG_MACRO___has_extension
		if (mode == TPP_KWD___has_extension &&
		    !tpp_lexer_getext(self, TPP_EXT_MACRO___has_extension))
			goto seek_end_of_macro;
#define WANT_seek_end_of_macro
#endif /* TPP_HAVE_CLANG_MACRO___has_extension */

		/* Parse the string that the user entered. */
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_string_feature_test_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
	} else
#endif /* TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
	{
#if TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
		/* Deal with keyword-style feature tests... */
		if (TPP_TOK_ISKEYWORD(tok)) {
			tpp_keyword const *feature_keyword;
			tpp_keyword_flags mask, flags;

			/* Load keyword flags. */
			feature_keyword = tpp_lexer_gettoken(self)->tt_kwd;
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
				if (tpp_lexer_getext(self, TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES))
					mask |= TPP_KEYWORD_FLAG_HAS_EXTENSION;
#endif /* TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES */
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
			case TPP_KWD___has_c_attribute:
				mask = TPP_KEYWORD_FLAG_HAS_C_ATTRIBUTE;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_MACRO___is_identifier
			case TPP_KWD___is_identifier:
				/* Something is considered to be an "identifier" if it's not a builtin keyword. */
				if (!TPP_TOK_ISBUILTINKEYWORD(feature_keyword->tk_id))
					tpp_feature_test_macro_expansion[0] = '1';
				goto after_expansion_mode_assignment;
#endif /* TPP_HAVE_MACRO___is_identifier */
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
#if TPP_HAVE_MACRO___is_identifier
after_expansion_mode_assignment:
#endif /* TPP_HAVE_MACRO___is_identifier */
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
				if tpp_unlikely(TPP_TOK_ISERR(tok))
					goto err_tok;
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS */
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
		if (tok == TPP_TOK_EOF)
			goto rollback;
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
		if tpp_unlikely(TPP_TOK_ISERR(tok))
			goto err_tok;
	}
	tpp_lexer_seek_commit(self, pos);
	return tpp_lexer_push_textfile(self, tpp_feature_test_macro_expansion, 1);
rollback:
	tok = backup.tlsb_id;
err_tok:
	tpp_lexer_seek_rollback(self, &backup);
	return tok;
#undef tpp_feature_test_macro_expansion
}
#endif /* TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO */

#endif /* TPP_HAVE_CPP_MACROS */

/* Handle a keyword-style macro.
 * @return: TPP_TOK_EOF: Caller should yield again.
 * @return: * : The new expansion token after keywords were handled */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
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
		/* TODO: __has_embed (https://en.cppreference.com/c/preprocessor/embed) */
	}	break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__
#if TPP_HAVE_MACRO___FILE__
	case TPP_KWD___FILE__:
#endif /* TPP_HAVE_MACRO___FILE__ */
#if TPP_HAVE_MACRO___BASE_FILE__
	case TPP_KWD___BASE_FILE__:
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
	{
		/* TODO */
	}	break;
#endif /* TPP_HAVE_MACRO___LINE__ || TPP_HAVE_MACRO___COLUMN__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TIME__ || TPP_HAVE_MACRO___DATE__ || TPP_HAVE_MACRO___TIMESTAMP__
#if TPP_HAVE_MACRO___TIME__
	case TPP_KWD___TIME__:
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
	case TPP_KWD___DATE__:
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
	case TPP_KWD___TIMESTAMP__:
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
	{
		/* TODO: -Wdate-time (disabled by default) */
		/* TODO */
	}	break;
#endif /* TPP_HAVE_MACRO___TIME__ || TPP_HAVE_MACRO___DATE__ || TPP_HAVE_MACRO___TIMESTAMP__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__ || TPP_HAVE_MACRO___INCLUDE_DEPTH__
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__
	case TPP_KWD___INCLUDE_LEVEL__:
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ */
#if TPP_HAVE_MACRO___INCLUDE_DEPTH__
	case TPP_KWD___INCLUDE_DEPTH__:
#endif /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
	{
		/* TODO */
	}	break;
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ || TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___COUNTER__
	case TPP_KWD___COUNTER__:
		/* TODO */
		break;
#endif /* TPP_HAVE_MACRO___COUNTER__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_NUMERIC_DATE_MACROS
	/* TODO: __DATE_DAY__, __DATE_WDAY__, __DATE_YDAY__, __DATE_MONTH__, __DATE_YEAR__ */
	/* TODO: -Wdate-time (disabled by default) */
#endif /* !TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
	/* TODO: __TIME_SEC__, __TIME_MIN__, __TIME_HOUR__ */
	/* TODO: -Wdate-time (disabled by default) */
#endif /* !TPP_HAVE_NUMERIC_TIME_MACROS */
#if TPP_HAVE_MACRO___TPP_EVAL
	/* TODO: __TPP_EVAL */
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */
#if TPP_HAVE_MACRO___TPP_UNIQUE
	/* TODO: __TPP_UNIQUE */
#endif /* !TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_LOAD_FILE
	/* TODO: __TPP_LOAD_FILE */
#endif /* !TPP_HAVE_MACRO___TPP_LOAD_FILE */
#if TPP_HAVE_MACRO___TPP_COUNTER
	/* TODO: __TPP_COUNTER */
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */
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
#if TPP_HAVE_MACRO___TPP_IDENTIFIER
	/* TODO: __TPP_IDENTIFIER */
#endif /* !TPP_HAVE_MACRO___TPP_IDENTIFIER */
/************************************************************************/



	default: {
		/* Check for a pre-defined, builtin macro expansion */
		tpp_builtin_macro const *builtin_macro;
		builtin_macro = tpp_macro_getbuiltin(tok);
		if (builtin_macro != NULL) {
			return tpp_lexer_push_textfile(self, builtin_macro->tbm_body,
			                               builtin_macro->tbm_body_size);
		}
	}	break;

	}

	/* Fallback: act as though the macro takes no arguments, and expands to itself:
	 * >> #define SOME_MACRO SOME_MACRO */
#endif /* TPP_HAVE_CPP_MACROS */
	return tok;
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
/* Check that the currently loaded token is 'tok'. If so, "tpp_lexer_yield_blocking()" to
 * the next token (which is also returned). Otherwise, trigger 'TPP_W_UNEXPECTED_TOKEN'
 * and (if that warning wasn't fatal), try to seek ahead to see if "tok" can be found
 * somewhere close by (depending on what 'tok' and what was actually loaded on entry)
 *
 * @return: * :                  The token that comes after the one that was just skipped
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	if tpp_likely(token->tt_id == tok)
		return tpp_lexer_yield(self);
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

	/* TODO: Try to seek ahead (within the current line) to
	 *       find "tok" when it's (e.g.) a '(' (to deal with
	 *       cases where the user added some extra, unrelated
	 *       tokens before the one we're expecting) */

	return token->tt_id;
}

#if TPP_HAVE_FILE_NONBLOCK
/* Same as `tpp_lexer_skip()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip_blocking(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token_id result = tpp_lexer_skip(self, tok);
	if (result == TPP_TOK_EWOULDBLOCK)
		result = tpp_lexer_yield_blocking(self);
	return result;
}
#endif /* TPP_HAVE_FILE_NONBLOCK */

#endif /* TPP_HAVE_LEXER_SKIP */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-decodestring.c                                     */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_TPP_TOK_STRINGLIKE

#if (TPP_HAVE_TPP_TOK_CHAR || TPP_HAVE_TPP_TOK_STRING || \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||         \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL ||        \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL ||        \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||         \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||            \
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
	    tpp_lexer_getext(self, TPP_EXT_TRIGRAPHS)) {
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
		if (!tpp_lexer_getext(self, TPP_EXT_TRIGRAPHS))
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
		if (!tpp_lexer_getext(self, TPP_EXT_ESCAPE_E_IN_STRINGS))
			goto handle_unknown_escape_sequence;
		ch = 0x1b;
		goto print_ch;
#endif /* !TPP_HAVE_ESCAPE_E_IN_STRINGS */

#if TPP_HAVE_ESCAPE_S_IN_STRINGS
	case 's':
		if (!tpp_lexer_getext(self, TPP_EXT_ESCAPE_S_IN_STRINGS))
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
		if (!tpp_lexer_getext(self, TPP_EXT_BSE))
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
			if (tpp_lexer_getext(self, TPP_EXT_BSE)) {
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
					if (tpp_lexer_getext(self, TPP_EXT_BSE_WHITESPACE))
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
			if (tpp_lexer_getext(self, TPP_EXT_BSE) &&
			    tpp_lexer_getext(self, TPP_EXT_BSE_WHITESPACE)) {
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

#if (TPP_HAVE_TPP_TOK_CHAR || TPP_HAVE_TPP_TOK_STRING || \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||         \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL ||        \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL ||        \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL)
#if TPP_HAVE_BSE
#if (TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL)
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF32_STRING_LITERAL
		tpp_bse_seek_until_fwd(start, '"');
		TPP_FALLTHRU
#endif /* ... */
#else /* TPP_HAVE_BSE */
#if (TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL)
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
		++start;
		TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
	_TPP_CASE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF32_STRING_LITERAL
		++start;
		TPP_FALLTHRU
#else /* ... */
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
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

#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL {
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
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */

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

struct tpp_lexer_decodestring_as_single_chunk_data {
	tpp_errno (TPPCALL *tldsascd_cb)(void *arg, tpp_char const *str, tpp_size length);
	void               *tldsascd_arg;
};

static tpp_ssize TPP_FORMATPRINTER_CC
tpp_lexer_decodestring_as_single_chunk_cb(void *arg, tpp_char const *text, tpp_size num_bytes) {
	tpp_errno error;
	struct tpp_lexer_decodestring_as_single_chunk_data *data;
	if tpp_unlikely(num_bytes == 0)
		return 0;
	data = (struct tpp_lexer_decodestring_as_single_chunk_data *)arg;
	tpp_assert(data->tldsascd_cb != NULL && "Multiple invocations?");
	error = (*data->tldsascd_cb)(data->tldsascd_arg, text, num_bytes);
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
                                       tpp_errno (TPPCALL *cb)(void *arg, tpp_char const *str, tpp_size length),
                                       void *arg) {
	tpp_ssize status;
	struct tpp_lexer_decodestring_as_single_chunk_data data;
	tpp_assert(cb && "NULL-callback given");
	data.tldsascd_cb  = cb;
	data.tldsascd_arg = arg;
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
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning
 * @return: * :             Return value of given "cb" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring_cb(tpp_lexer *tpp_restrict self,
                         tpp_errno (TPPCALL *cb)(void *arg, tpp_char const *str, tpp_size length),
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
		return (*cb)(arg, (tpp_char const *)"", 0);
	} else if (how == TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_YES) {
		tpp_lexer_seek_backup backup;
		tpp_char const *pos;
		tpp_token_id tok;
		tpp_errno result;

		/* Must make sure that the next token isn't another (non-empty) string */
		pos = tpp_lexer_seek_begin(self, &backup, true);
again_yield_after_single:
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
		switch (tok) {

		TPP_CASE_TPP_TOK_STRING {
			how = tpp_lexer_parsestring_is_single_chunk(self);
			if (how == TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_EMPTY)
				goto again_yield_after_single;

			/* Not possible using a single chunk... */
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

		default:
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			break;
		}

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
			result = (*cb)(arg, tpp_string_str(string), tpp_string_len(string));
			tpp_string_decref(string);
		}
		return result;
	}
	tpp_unreachable();
}
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE */

TPP_DECL_END
/************************************************************************/


#endif /* !GUARD_TPP_C */
/* clang-format on */
