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
		return "File has been masked";
#endif /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
#if TPP_HAVE_WARNINGS
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
	result = tpp_utoa(buf, value);
	if (is_neg)
		*--result = '-';
	return result;
}


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
		result->ts_refcnt   = 1;
		result->ts_len      = len;
		result->ts_str[len] = '\0';
	}
	return result;
}

TPP_IMPL TPP_WUNUSED tpp_string *TPPCALL
tpp_string_malloc(tpp_size len) {
	tpp_string *result = _tpp_string_malloc(len);
	if tpp_likely(result) {
		result->ts_refcnt   = 1;
		result->ts_len      = len;
		result->ts_str[len] = '\0';
	}
	return result;
}

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/token-strtokenid.c                                       */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_STRTOKENID
/* Returns the "*" in "TPP_TOK_*" of "id", which must be a (non-keyword and non-error) token ID */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_strtokenid(tpp_token_id id) {
	switch (id) {
#define TPP_STRTOKENID_CASE(x) case TPP_TOK_##x: return #x;
	TPP_STRTOKENID_CASE(EOF)
	TPP_STRTOKENID_CASE(LF)
	TPP_STRTOKENID_CASE(SPACE)
	TPP_STRTOKENID_CASE(PLUS)
	TPP_STRTOKENID_CASE(AMP)
	TPP_STRTOKENID_CASE(EQUAL)
	TPP_STRTOKENID_CASE(AT)
	TPP_STRTOKENID_CASE(BACKSLASH)
	TPP_STRTOKENID_CASE(COLON)
	TPP_STRTOKENID_CASE(COMMA)
	TPP_STRTOKENID_CASE(SLASH)
	TPP_STRTOKENID_CASE(DOT)
	TPP_STRTOKENID_CASE(POUND)
	TPP_STRTOKENID_CASE(LANGLE)
	TPP_STRTOKENID_CASE(LBRACE)
	TPP_STRTOKENID_CASE(LBRACKET)
	TPP_STRTOKENID_CASE(LPAREN)
	TPP_STRTOKENID_CASE(PERCENT)
	TPP_STRTOKENID_CASE(STAR)
	TPP_STRTOKENID_CASE(EXCLAIM)
	TPP_STRTOKENID_CASE(PIPE)
	TPP_STRTOKENID_CASE(QMARK)
	TPP_STRTOKENID_CASE(RANGLE)
	TPP_STRTOKENID_CASE(RBRACE)
	TPP_STRTOKENID_CASE(RBRACKET)
	TPP_STRTOKENID_CASE(RPAREN)
	TPP_STRTOKENID_CASE(SEMICOLON)
	TPP_STRTOKENID_CASE(MINUS)
	TPP_STRTOKENID_CASE(TILDE)
	TPP_STRTOKENID_CASE(HAT)
#if TPP_HAVE_UNICODE
	TPP_STRTOKENID_CASE(UNICHAR)
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	TPP_STRTOKENID_CASE(INT)
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	TPP_STRTOKENID_CASE(FLOAT)
#endif /* TPP_HAVE_TPP_TOK_FLOAT */

#if TPP_HAVE_TPP_TOK_COMMENTLIKE
#if TPP_HAVE_TPP_TOK_CXX_COMMENT
	TPP_STRTOKENID_CASE(CXX_COMMENT)
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_C_COMMENT
	TPP_STRTOKENID_CASE(C_COMMENT)
#endif /* TPP_HAVE_TPP_TOK_C_COMMENT */
#if TPP_HAVE_TPP_TOK_PASCAL_COMMENT
	TPP_STRTOKENID_CASE(PASCAL_COMMENT)
#endif /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
	TPP_STRTOKENID_CASE(SHELL_COMMENT)
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#if TPP_HAVE_TPP_TOK_ASM_COMMENT
	TPP_STRTOKENID_CASE(ASM_COMMENT)
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT */
#if TPP_HAVE_TPP_TOK_SQL_COMMENT
	TPP_STRTOKENID_CASE(SQL_COMMENT)
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE */

#if TPP_HAVE_TPP_TOK_STRINGLIKE
#if TPP_HAVE_TPP_TOK_CHAR
	TPP_STRTOKENID_CASE(CHAR)
#endif /* !TPP_HAVE_TPP_TOK_CHAR */
#if TPP_HAVE_TPP_TOK_STRING
	TPP_STRTOKENID_CASE(STRING)
#endif /* TPP_HAVE_TPP_TOK_STRING */
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_STRTOKENID_CASE(CXX_RAW_STRING_LITERAL)
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	TPP_STRTOKENID_CASE(CXX_WIDE_STRING_LITERAL)
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_STRTOKENID_CASE(CXX_RAW_WIDE_STRING_LITERAL)
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	TPP_STRTOKENID_CASE(CXX_UTF8_STRING_LITERAL)
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_STRTOKENID_CASE(CXX_RAW_UTF8_STRING_LITERAL)
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	TPP_STRTOKENID_CASE(CXX_UTF16_STRING_LITERAL)
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_STRTOKENID_CASE(CXX_RAW_UTF16_STRING_LITERAL)
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	TPP_STRTOKENID_CASE(CXX_UTF32_STRING_LITERAL)
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_STRTOKENID_CASE(CXX_RAW_UTF32_STRING_LITERAL)
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
	TPP_STRTOKENID_CASE(RAW_STRING_LITERAL)
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	TPP_STRTOKENID_CASE(RAW_CHAR_LITERAL)
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	TPP_STRTOKENID_CASE(BLOCK_STRING_LITERAL)
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	TPP_STRTOKENID_CASE(BLOCK_CHAR_LITERAL)
#endif /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE */

#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	TPP_STRTOKENID_CASE(LANGLE_LANGLE)
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	TPP_STRTOKENID_CASE(RANGLE_RANGLE)
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	TPP_STRTOKENID_CASE(EQUAL_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	TPP_STRTOKENID_CASE(EXCLAIM_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	TPP_STRTOKENID_CASE(RANGLE_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	TPP_STRTOKENID_CASE(LANGLE_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	TPP_STRTOKENID_CASE(DOT_DOT_DOT)
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
	TPP_STRTOKENID_CASE(PLUS_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
	TPP_STRTOKENID_CASE(MINUS_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
	TPP_STRTOKENID_CASE(STAR_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
	TPP_STRTOKENID_CASE(SLASH_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	TPP_STRTOKENID_CASE(PERCENT_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	TPP_STRTOKENID_CASE(LANGLE_LANGLE_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	TPP_STRTOKENID_CASE(RANGLE_RANGLE_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
	TPP_STRTOKENID_CASE(AMP_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
	TPP_STRTOKENID_CASE(PIPE_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
	TPP_STRTOKENID_CASE(HAT_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	TPP_STRTOKENID_CASE(STAR_STAR_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	TPP_STRTOKENID_CASE(SLASH_SLASH_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_AT_EQUAL
	TPP_STRTOKENID_CASE(AT_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
	TPP_STRTOKENID_CASE(POUND_POUND)
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
	TPP_STRTOKENID_CASE(AMP_AMP)
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
	TPP_STRTOKENID_CASE(PIPE_PIPE)
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
	TPP_STRTOKENID_CASE(HAT_HAT)
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
	TPP_STRTOKENID_CASE(PLUS_PLUS)
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
	TPP_STRTOKENID_CASE(MINUS_MINUS)
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
	TPP_STRTOKENID_CASE(STAR_STAR)
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
	TPP_STRTOKENID_CASE(SLASH_SLASH)
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
	TPP_STRTOKENID_CASE(TILDE_TILDE)
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
	TPP_STRTOKENID_CASE(TILDE_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	TPP_STRTOKENID_CASE(MINUS_RANGLE)
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
	TPP_STRTOKENID_CASE(COLON_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
	TPP_STRTOKENID_CASE(COLON_COLON)
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	TPP_STRTOKENID_CASE(MINUS_RANGLE_STAR)
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_DOT_STAR
	TPP_STRTOKENID_CASE(DOT_STAR)
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
	TPP_STRTOKENID_CASE(DOT_DOT)
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	TPP_STRTOKENID_CASE(LANGLE_RANGLE)
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	TPP_STRTOKENID_CASE(LANGLE_LANGLE_LANGLE)
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	TPP_STRTOKENID_CASE(RANGLE_RANGLE_RANGLE)
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	TPP_STRTOKENID_CASE(LANGLE_LANGLE_LANGLE_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	TPP_STRTOKENID_CASE(RANGLE_RANGLE_RANGLE_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	TPP_STRTOKENID_CASE(EQUAL_EQUAL_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	TPP_STRTOKENID_CASE(EXCLAIM_EQUAL_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
	TPP_STRTOKENID_CASE(QMARK_QMARK)
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
	TPP_STRTOKENID_CASE(QMARK_EQUAL)
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */

#undef TPP_STRTOKENID_CASE
	default: break;
	}
	return NULL;
}
#endif /* TPP_HAVE_STRTOKENID */

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
	case TPP_FILE_KIND_MACRO:
		tpp_macro_decref(self->tf_data.td_macro.tfm_macro);
		break;
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
			/* FALLTHRU */
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

#define TPP_UTF8_1BYTE_MAX ((UINT32_C(1) << 7) - 1)
#define TPP_UTF8_2BYTE_MAX ((UINT32_C(1) << 11) - 1)
#define TPP_UTF8_3BYTE_MAX ((UINT32_C(1) << 16) - 1)
#define TPP_UTF8_4BYTE_MAX ((UINT32_C(1) << 21) - 1)
#define TPP_UTF8_5BYTE_MAX ((UINT32_C(1) << 26) - 1)
#define TPP_UTF8_6BYTE_MAX ((UINT32_C(1) << 31) - 1)

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
	tpp_assert(!old_chunk || self->tf_pos >= (old_chunk->ts_str));
	tpp_assert(!old_chunk || self->tf_end <= (old_chunk->ts_str + old_chunk->ts_len));
	if (old_chunk && !tpp_string_isshared(old_chunk)) {
		/* Can re-use the old chunk */
		tpp_size unused_head = (tpp_size)(self->tf_pos - old_chunk->ts_str);
#ifndef __OPTIMIZE_SIZE__
		if (unused_head)
#endif /* !__OPTIMIZE_SIZE__ */
		{
			self->tf_data.td_io.tff_start_lc = tpp_lcinfo_account(self,
			                                                      self->tf_data.td_io.tff_start_lc,
			                                                      old_chunk->ts_str, unused_head);
			tpp_memmovedown(old_chunk->ts_str, self->tf_pos, old_inuse);
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
		tpp_assert(self->tf_pos == old_chunk->ts_str);
		tpp_assert(self->tf_end == old_chunk->ts_str + old_inuse);
		if (old_chunk->ts_len >= new_size) {
			/* Don't even need to realloc() the old chunk! */
reuse_old_chunk:
			new_chunk = old_chunk;
			new_size  = old_chunk->ts_len;
		} else {
			/* Must realloc() the old chunk */
#if TPP_HAVE_FILE_LC_CACHE
			tpp_size lc_rel = (tpp_size)(self->tf_lcpos - old_chunk->ts_str);
#endif /* TPP_HAVE_FILE_LC_CACHE */
			new_chunk = (TPP_REF tpp_string *)tpp_tryrealloc(old_chunk, tpp_string_sizeof(new_size));
			if tpp_unlikely(!new_chunk) {
				new_size = old_inuse + TPP_FILE_MINEXTRA;
				if (old_chunk->ts_len >= new_size)
					goto reuse_old_chunk;
				new_chunk = (TPP_REF tpp_string *)tpp_realloc(old_chunk, tpp_string_sizeof(new_size));
				if tpp_unlikely(!new_chunk)
					return TPP_ENOMEM;
			}
			tpp_assert(new_chunk->ts_refcnt == 1);
			new_chunk->ts_str[new_size] = '\0';
			new_chunk->ts_len = new_size;
			self->tf_pos = new_chunk->ts_str;
			self->tf_end = new_chunk->ts_str + old_inuse;
#if TPP_HAVE_FILE_LC_CACHE
			if (self->tf_lcpos)
				self->tf_lcpos = new_chunk->ts_str + lc_rel;
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
		tpp_memcpy(new_chunk->ts_str, self->tf_pos, old_inuse);
		if (old_chunk) {
			tpp_size unused_head = (tpp_size)(self->tf_pos - old_chunk->ts_str);
			self->tf_data.td_io.tff_start_lc = tpp_lcinfo_account(self,
			                                                      self->tf_data.td_io.tff_start_lc,
			                                                      old_chunk->ts_str, unused_head);
			tpp_assert(tpp_string_isshared(old_chunk));
			tpp_refcnt_dec(&old_chunk->ts_refcnt);
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
			self->tf_lcpos = new_chunk->ts_str + (self->tf_lcpos - self->tf_pos);
			if (self->tf_lcpos < new_chunk->ts_str)
				self->tf_lcpos = NULL; /* Cache fell out-of-scope */
		}
#endif /* TPP_HAVE_FILE_LC_CACHE */
		self->tf_pos   = new_chunk->ts_str;
		self->tf_end   = new_chunk->ts_str + old_inuse;
		self->tf_chunk = new_chunk; /* Inherit reference */
	}

	tpp_assert(self->tf_chunk == new_chunk);
	tpp_assert(self->tf_pos == new_chunk->ts_str);
	tpp_assert(self->tf_end <= (new_chunk->ts_str + new_chunk->ts_len));
	io_size = (tpp_size)((new_chunk->ts_str + new_chunk->ts_len) - self->tf_end);
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

		dst_end = new_chunk->ts_str + new_chunk->ts_len;
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




/* Return line/column information (1-based) for "pos" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_lcinfo TPPCALL
tpp_file_lcinfo(tpp_file *tpp_restrict self, tpp_char const *pos) {
	tpp_lcinfo result;
	if tpp_unlikely(!self->tf_chunk)
		return tpp_lcinfo_of(0, 0);
	tpp_assert(pos >= (self->tf_chunk->ts_str));
	tpp_assert(pos <= (self->tf_chunk->ts_str + self->tf_chunk->ts_len));

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
		delta_from_chunk = (tpp_size)(pos - self->tf_chunk->ts_str);
		delta_from_lcpos = (tpp_size)(self->tf_lcpos - pos);
		if (delta_from_chunk > (delta_from_lcpos * 2)) {
			/* Given "pos" is much closer to "tf_lcpos", so try to work off of the cache! */
			tpp_char const *last_linefeed;
			last_linefeed = tpp_lcinfo_find_last_linefeed(self, self->tf_chunk->ts_str, pos);
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
		result = self->tf_data.td_io.tff_start_lc;
		result = tpp_lcinfo_account(self, result, self->tf_chunk->ts_str,
		                            (tpp_size)(pos - self->tf_chunk->ts_str));
		break;
	case TPP_FILE_KIND_TEXT:
		tpp_lcinfo_init(result, 0, 0);
		result = tpp_lcinfo_account(self, result, self->tf_chunk->ts_str,
		                            (tpp_size)(pos - self->tf_chunk->ts_str));
		break;
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
			result = tpp_lcinfo_account(self, result, self->tf_chunk->ts_str,
			                            (tpp_size)(pos - self->tf_chunk->ts_str));
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


/* Returns the filename of "self", or "NULL" if unknown. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_filename(tpp_file const *tpp_restrict self) {
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO: {
		tpp_keyword const *kwd = self->tf_data.td_io.tff_name;
		return kwd ? (char const *)kwd->tk_kwd : NULL;
	}	break;

	case TPP_FILE_KIND_TEXT:
		return self->tf_data.td_text.tft_name;

#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO: {
		tpp_macro const *macro = self->tf_data.td_macro.tfm_macro;
		tpp_keyword const *kwd = macro->tm_deffile;
		return kwd ? (char const *)kwd->tk_kwd : NULL;
	}	break;
#endif /* TPP_HAVE_CPP_MACROS */

	default: tpp_unreachable();
	}
}

/* Returns the filename "keyword" (which may not always be
 * available, even when "tpp_file_filename()" returns non-NULL) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) struct tpp_keyword *TPPCALL
tpp_file_filename_kwd(tpp_file const *tpp_restrict self) {
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO:
#if TPP_HAVE_FILE_NOKWD
		if (self->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOKWD)
			return NULL; /* Name isn't actually a keyword... */
#endif /* TPP_HAVE_FILE_NOKWD */
		return self->tf_data.td_io.tff_name;

	case TPP_FILE_KIND_TEXT:
		return NULL;

#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO:
		return self->tf_data.td_macro.tfm_macro->tm_deffile;
#endif /* TPP_HAVE_CPP_MACROS */

	default: tpp_unreachable();
	}
}


/* Returns the first tf_kind=TPP_FILE_KIND_IO file in the #include-stack
 * If no such file exists, simply re-return "self". This function never
 * returns "NULL" */
#if TPP_HAVE_INCLUDE_STACK
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getiofile(tpp_file const *tpp_restrict self) {
	while (self->tf_prev)
		self = self->tf_prev;
	return (tpp_file *)self;
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
	tpp_assert(iter < end);
#if TPP_HAVE_TRIGRAPHS
	tpp_assert(*iter == '\\' || *iter == '?');
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
	tpp_assert(*iter == '\\');
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
	return iter;
not_bse:
	return pos;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_skipbse_bck_(tpp_char const *pos, tpp_char const *start tpp_bse_file__PARAM) {
	tpp_char const *iter = pos;
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
	if (*iter == '\\')
		return iter;
#if TPP_HAVE_TRIGRAPHS
	if (*iter == '/') {
		if ((iter - 2) < start)
			goto not_bse;
		if (iter[-1] == '?' && iter[-2] == '?')
			return iter - 2;
	}
#endif /* TPP_HAVE_TRIGRAPHS */
not_bse:
	return pos;
}
#endif /* TPP_HAVE_BSE */


#if TPP_HAVE_BSE
/* Same as `tpp_hashof()', but skip over \-escaped linefeeds when calculating the hash */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof_bse_(tpp_char const *tpp_restrict kwd, tpp_size len tpp_bse_file__PARAM) {
	tpp_hash result = 1;
	tpp_char const *end = kwd + len;
	while (kwd < end) {
		tpp_char ch = *kwd++;
		result = result * 263 + ch;
		kwd = tpp_skipbse_fwd(kwd, end, file);
	}
	return result;
}

/* Copy `in_text...+=len' to `out_text', whilst removing \-escaped linefeeds
 * The caller must ensure that `out_text' has space for at least `len' bytes,
 * and the actual # of used bytes of `out_text' is returned. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_without_bse_(tpp_char *tpp_restrict out_text,
                 tpp_char const *tpp_restrict in_text,
                 tpp_size len tpp_bse_file__PARAM) {
	tpp_size result = 0;
	tpp_char const *end = in_text + len;
	while (in_text < end) {
		tpp_char ch = *in_text++;
		out_text[result++] = ch;
		in_text = tpp_skipbse_fwd(in_text, end, file);
	}
	return result;
}

/* Compare 2 strings, one of which may contain \-escaped linefeeds that must be skipped. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) int TPPCALL
tpp_memcmp_bse_(tpp_char const *lhs_without_bse, tpp_size lhs_len,
                tpp_char const *rhs_with_bse, tpp_size rhs_len
                tpp_bse_file__PARAM) {
	tpp_char const *lhs_end = lhs_without_bse + lhs_len;
	tpp_char const *rhs_end = rhs_with_bse + rhs_len;
	while (rhs_with_bse < rhs_end) {
		tpp_char lhs_ch, rhs_ch;
		if (lhs_without_bse >= lhs_end)
			return -1;
		lhs_ch = *lhs_without_bse++;
		rhs_ch = *rhs_with_bse++;
		if (lhs_ch != rhs_ch)
			return lhs_ch < rhs_ch ? -1 : 1;
		rhs_with_bse = tpp_skipbse_fwd(rhs_with_bse, rhs_end, file);
	}
	return 0;
}
#endif /* TPP_HAVE_BSE */


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
	tpp_assert(self->tk_refcnt == 1 && "Keyword still in use");
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

#if TPP_HAVE_BSE
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword_bse_(tpp_keywords const *tpp_restrict self,
                              tpp_char const *tpp_restrict kwd,
                              tpp_size len, tpp_hash hash
                              tpp_bse_file__PARAM) {
	tpp_keyword *result = self->tks_bckv[hash & self->tks_bckm];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (tpp_memcmp_bse(result->tk_kwd, result->tk_len, kwd, len, file) == 0)
			break;
	}
	return result;
}
#endif /* TPP_HAVE_BSE */


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

#if TPP_HAVE_BSE
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword_bse_(tpp_keywords const *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM) {
	tpp_keyword const *result;
	result = _tpp_keywords_getkeyword_bse(self, kwd, len, hash, file);
	if (result == NULL)
		result = tpp_builtin_getkeyword_bse(kwd, len, hash, file);
	return result;
}
#endif /* TPP_HAVE_BSE */


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
	result->tk_refcnt = 1;
	result->tk_len = len;
	tpp_memcpy(result->tk_kwd, kwd, len * sizeof(tpp_char));
	result->tk_kwd[len] = (tpp_char)'\0';
	result = tpp_keywords_inskeyword(self, result);
done:
	return result;
}

#if TPP_HAVE_BSE
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_newkeyword_bse_(tpp_keywords *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM) {
	tpp_keyword *result;
	tpp_size len_without_bse;
	result = (tpp_keyword *)tpp_keywords_getkeyword_bse(self, kwd, len, hash, file);
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
	result->tk_refcnt = 1;
	len_without_bse = tpp_without_bse(result->tk_kwd, kwd, len, file);
	tpp_assert(len_without_bse <= len);
	result->tk_len = len_without_bse;
	if (len_without_bse < len) {
		tpp_keyword *new_result;
		new_result = (tpp_keyword *)tpp_realloc(result, tpp_keyword_sizeof(len_without_bse));
		if tpp_likely(new_result)
			result = new_result;
	}
	result->tk_kwd[len_without_bse] = (tpp_char)'\0';
	result = tpp_keywords_inskeyword(self, result);
done:
	return result;
}
#endif /* TPP_HAVE_BSE */


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
	result->tk_hash   = kwd->tk_hash;
	result->tk_refcnt = 1;
	result->tk_len    = kwd->tk_len;
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
                         /*0..1*/ tpp_keyword const *tpp_restrict relative_to,
                         /*1..1*/ /*utf-8*/ char const *tpp_restrict filename,
                         /*1..1*/ tpp_file *tpp_restrict out_file,
                         tpp_keyword_flags mask_flags)
#else /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3, 4)) tpp_errno TPPCALL
tpp_keywords_openfile(/*1..1*/ tpp_keywords *tpp_restrict self,
                      /*0..1*/ tpp_keyword const *tpp_restrict relative_to,
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
		char const *rel_base = (char const *)relative_to->tk_kwd;
		char const *last_sep = rel_base + relative_to->tk_len;
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
		result_kwd->tk_refcnt = 1;
		result_kwd = tpp_keywords_inskeyword(self, result_kwd);
		if tpp_unlikely(!result_kwd) {
			tpp_io_close(handle);
			goto err_nomem;
		}
	}

	/* Initialize "out_file" */
	tpp_file_init_io(out_file, result_kwd, handle);
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
#define TPP_WGROUP(wgroup_id, names, default)                                                \
	TPP_TUPLE_FOREACH(names, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WGROUP_NAMES, wgroup_id) \
	char twgn_tail_##wgroup_id;
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WGROUP_NAMES
#undef TPP_DEFS
} const tpp_warning_group_names = {
#define TPP_DEFS
#define _TPP_EXPAND_WGROUP_NAMES(wgroup_id, index, value) \
	/* .twgn_##wgroup_id##_##index = */ value,
#define TPP_WGROUP(wgroup_id, names, default)                                                \
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
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format)                                              \
	TPP_TUPLE_FOREACH(wgroup_ids, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WARNING_GROUP_IDS, warning_id) \
	tpp_warning_group_id twig_tail_##warning_id;
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WARNING_GROUP_IDS
#undef TPP_DEFS
} const tpp_warning_groups = {
#define TPP_DEFS
#define _TPP_EXPAND_WARNING_GROUP_IDS(warning_id, index, value) \
	/* .twig_##warning_id##_##index = */ value,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format)                                              \
	TPP_TUPLE_FOREACH(wgroup_ids, TPP_TUPLE_FOREACH_DUMMY_SEP, _TPP_EXPAND_WARNING_GROUP_IDS, warning_id) \
	/* .twig_tail_##warning_id = */ TPP_WG_COUNT,
#include TPP_CONFIG_DEFS_FILENAME
#undef _TPP_EXPAND_WARNING_GROUP_IDS
#undef TPP_DEFS
};

static tpp_size const tpp_warning_group_offsets_byid[TPP_EXT_COUNT] = {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
	/* [wgroup_id] = */ tpp_offsetof(struct tpp_warning_groups_struct, twig_##warning_id##_0),
#include TPP_CONFIG_DEFS_FILENAME
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
#define TPP_DECLARE_NUMBERED_WARNING(warning_id) \
		/* .twsn_##warning_id = */ (unsigned int)_TPP_WSTATE_UNDEFINED,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
		TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, warning_id)
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
		tpp_char             tk_kwd[sizeof(str) / sizeof(char)]; \
	} tpp_builtin_keyword_##id;                                  \
	TPP_INTERN_IMPL struct tpp_builtin_keyword_struct_##id       \
	tpp_builtin_keyword_##id = {                                 \
		/* .tk_id        = */ id,                                \
		/* .tk_next      = */ NULL,                              \
		_TPP_BUILTIN_KEYWORD_tk_macro_INIT                       \
		_TPP_BUILTIN_KEYWORD_tk_misc_INIT                        \
		/* .tk_hash      = */ TPP_MAYBE_HASHOF(str),             \
		/* .tk_refcnt    = */ 1,                                 \
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
		tpp_char             tk_kwd[kwd_len + 1];            \
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
		/* .tk_refcnt    = */ 1,                                           \
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

#if TPP_HAVE_BSE
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_bse_(tpp_char const *tpp_restrict kwd,
                            tpp_size len, tpp_hash hash
                            tpp_bse_file__PARAM) {
	tpp_keyword const *result;
	tpp_init_builtin_keywords();
	result = tpp_builtin_keyword_table[hash & TPP_BUILTIN_KEYWORD_MASK];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (tpp_memcmp_bse(result->tk_kwd,
		                   result->tk_len,
		                   kwd, len, file) == 0)
			break;
	}
	return result;
}
#endif /* TPP_HAVE_BSE */



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

#if TPP_HAVE_CPP_IF_ELSE_ENDIF
/* Returns true if "kwd" should be considered to be "#if defined()"
 * Since "builtin" keywords can be considered to be "defined", even
 * when `kwd->tk_macro == NULL', this function is needed to handle
 * such macros. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_getkeyworddefined(tpp_lexer *tpp_restrict self,
                            tpp_keyword const *tpp_restrict kwd) {
	if (!TPP_TOK_ISBUILTINKEYWORD(kwd->tk_id))
		return kwd->tk_macro != NULL;
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
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */


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
tpp_extension_byname(char const *tpp_restrict name) {
	unsigned int lo, hi;
	tpp_init_extension_name_offsets_byname();
	lo = 0;
	hi = tpp_lengthof(tpp_extension_name_offsets_byname);
	while (lo < hi) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_extension_name_offsets_byname[mid];
		char const *mid_name = (char const *)&tpp_extension_names + mid_offset;
		int cmp = tpp_strcmp(name, mid_name);
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
tpp_extension_nearest(char const *tpp_restrict name) {
	(void)name;
	/* TODO */
	return TPP_EXT_COUNT;
}
#endif /* TPP_HAVE_EXTENSIONS */



#if TPP_HAVE_WARNINGS

static struct tpp_warning_format_strings_struct {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
	char twf_##warning_id[sizeof(format) / sizeof(char)];
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, expr)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBER_CASE
#undef TPP_DEFS
} const tpp_warning_format_strings = {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
	/* [twf_##warning_id] = */ format,
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, expr)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBER_CASE
#undef TPP_DEFS
};

static tpp_size const tpp_warning_format_string_offsets[TPP_W_COUNT] = {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
	/* [warning_id] = */ tpp_offsetof(struct tpp_warning_format_strings_struct, twf_##warning_id),
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, expr) \
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
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
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
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
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
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
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
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
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
tpp_warning_group_byname(char const *tpp_restrict name) {
	unsigned int lo, hi;
	tpp_init_warning_group_name_offsets_byname();
	lo = 0;
	hi = tpp_lengthof(tpp_warning_group_name_offsets_byname);
	while (lo < hi) {
		unsigned int mid = (lo + hi) / 2;
		tpp_size mid_offset = tpp_warning_group_name_offsets_byname[mid];
		char const *mid_name = (char const *)&tpp_warning_group_names + mid_offset;
		int cmp = tpp_strcmp(name, mid_name);
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
tpp_warning_group_nearest(char const *tpp_restrict name) {
	(void)name;
	/* TODO */
	return TPP_WG_COUNT;
}
#endif /* TPP_HAVE_WARNINGS */


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
/* File: parts/extension.c                                              */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_EXTENSIONS

/* Default extension state */
TPP_CONST_IMPL tpp_extension_state const tpp_extension_state_default = {
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
		if (!!tpp_extension_state_getid(&self->te_state, id) == !!enabled)
			return TPP_EOK; /* Unchanged -> no need to actually copy! */
		copy = (tpp_extensions *)tpp_malloc(sizeof(tpp_extensions));
		if tpp_unlikely(!copy)
			goto err_nomem;
		tpp_memcpy(copy, self, sizeof(tpp_extensions));
		self->te_prev    = copy;
		self->te_pushcnt = 0;
	}
	tpp_extension_state_set(&self->te_state, id, enabled);
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
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL            = */ TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL          = */ TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0 */
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
#if TPP_HAVE_TPP_TOK_SLASH_SLASH < 0
		/* .tff_TPP_FEAT_TPP_TOK_SLASH_SLASH                = */ TPP_HAVE_TPP_TOK_SLASH_SLASH == -1,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH < 0 */
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
#if TPP_HAVE_CPP_DIRECTIVES < 0
		/* .tff_TPP_FEAT_CPP_DIRECTIVES                     = */ TPP_HAVE_CPP_DIRECTIVES == -1,
#endif /* TPP_HAVE_CPP_DIRECTIVES < 0 */
#if TPP_HAVE_CPP_MACROS < 0
		/* .tff_TPP_FEAT_CPP_MACROS                         = */ TPP_HAVE_CPP_MACROS == -1,
#endif /* TPP_HAVE_CPP_MACROS < 0 */
#if TPP_HAVE_CPP_EXCLAIM < 0
		/* .tff_TPP_FEAT_CPP_EXCLAIM                        = */ TPP_HAVE_CPP_EXCLAIM == -1,
#endif /* TPP_HAVE_CPP_EXCLAIM < 0 */
#if TPP_HAVE_CPP_BLANK < 0
		/* .tff_TPP_FEAT_CPP_BLANK                          = */ TPP_HAVE_CPP_BLANK == -1,
#endif /* TPP_HAVE_CPP_BLANK < 0 */
#if TPP_HAVE_CPP_DIGIT_LINE < 0
		/* .tff_TPP_FEAT_CPP_DIGIT_LINE                     = */ TPP_HAVE_CPP_DIGIT_LINE == -1,
#endif /* TPP_HAVE_CPP_DIGIT_LINE < 0 */
#if TPP_HAVE_CPP_LINE < 0
		/* .tff_TPP_FEAT_CPP_LINE                           = */ TPP_HAVE_CPP_LINE == -1,
#endif /* TPP_HAVE_CPP_LINE < 0 */
#if TPP_HAVE_CPP_INCLUDE < 0
		/* .tff_TPP_FEAT_CPP_INCLUDE                        = */ TPP_HAVE_CPP_INCLUDE == -1,
#endif /* TPP_HAVE_CPP_INCLUDE < 0 */
#if TPP_HAVE_CPP_INCLUDE_NEXT < 0
		/* .tff_TPP_FEAT_CPP_INCLUDE_NEXT                   = */ TPP_HAVE_CPP_INCLUDE_NEXT == -1,
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT < 0 */
#if TPP_HAVE_CPP_IMPORT < 0
		/* .tff_TPP_FEAT_CPP_IMPORT                         = */ TPP_HAVE_CPP_IMPORT == -1,
#endif /* TPP_HAVE_CPP_IMPORT < 0 */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF < 0
		/* .tff_TPP_FEAT_CPP_IF_ELSE_ENDIF                  = */ TPP_HAVE_CPP_IF_ELSE_ENDIF == -1,
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF < 0 */
#if TPP_HAVE_CPP_DEFINE < 0
		/* .tff_TPP_FEAT_CPP_DEFINE                         = */ TPP_HAVE_CPP_DEFINE == -1,
#endif /* TPP_HAVE_CPP_DEFINE < 0 */
#if TPP_HAVE_CPP_ASSERT < 0
		/* .tff_TPP_FEAT_CPP_ASSERT                         = */ TPP_HAVE_CPP_ASSERT == -1,
#endif /* TPP_HAVE_CPP_ASSERT < 0 */
#if TPP_HAVE_CPP_ERROR < 0
		/* .tff_TPP_FEAT_CPP_ERROR                          = */ TPP_HAVE_CPP_ERROR == -1,
#endif /* TPP_HAVE_CPP_ERROR < 0 */
#if TPP_HAVE_CPP_WARNING < 0
		/* .tff_TPP_FEAT_CPP_WARNING                        = */ TPP_HAVE_CPP_WARNING == -1,
#endif /* TPP_HAVE_CPP_WARNING < 0 */
#if TPP_HAVE_CPP_IDENT < 0
		/* .tff_TPP_FEAT_CPP_IDENT                          = */ TPP_HAVE_CPP_IDENT == -1,
#endif /* TPP_HAVE_CPP_IDENT < 0 */
#if TPP_HAVE_CPP_SCSS < 0
		/* .tff_TPP_FEAT_CPP_SCSS                           = */ TPP_HAVE_CPP_SCSS == -1,
#endif /* TPP_HAVE_CPP_SCSS < 0 */
#if TPP_HAVE_CPP_PRAGMA < 0
		/* .tff_TPP_FEAT_CPP_PRAGMA                         = */ TPP_HAVE_CPP_PRAGMA == -1,
#endif /* TPP_HAVE_CPP_PRAGMA < 0 */
#if TPP_HAVE_STDC_PRAGMA < 0
		/* .tff_TPP_FEAT_STDC_PRAGMA                        = */ TPP_HAVE_STDC_PRAGMA == -1,
#endif /* TPP_HAVE_STDC_PRAGMA < 0 */
#if TPP_HAVE_MSVC_PRAGMA < 0
		/* .tff_TPP_FEAT_MSVC_PRAGMA                        = */ TPP_HAVE_MSVC_PRAGMA == -1,
#endif /* TPP_HAVE_MSVC_PRAGMA < 0 */
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
                    struct tpp_warning_invokeinfo *tpp_restrict result)
#else /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */
TPP_IMPL TPP_NONNULL((1, 3)) void TPPCALL
tpp_warnings_invoke_(tpp_warnings const *tpp_restrict self, tpp_warning_id warning_id,
                     struct tpp_warning_invokeinfo *tpp_restrict result)
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
	result->twii_state  = TPP_WSTATE_DISABLED;
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
		tpp_free(iter);
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
                       tpp_file_encoding encoding)
#else /* TPP_HAVE_UNICODE */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ascii(tpp_lexer *tpp_restrict self,
                          /*utf-8*/ char const *filename,
                          void const *text, tpp_size text_size)
#endif /* !TPP_HAVE_UNICODE */
{
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_init_text_ex(file, filename, text, text_size, encoding);
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
	tpp_keyword *filename_kwd = NULL;
	if (filename != NULL) {
		filename_kwd = (tpp_keyword *)((char *)filename - offsetof(tpp_keyword, tk_kwd));
		ioflags |= TPP_HAVE_FILE_NOKWD;
	}
	tpp_file_init_io_ex(file, filename_kwd, handle, ioflags);
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
	if tpp_unlikely(error != TPP_EOK)
		_tpp_lexer_fini_common(self);
	return error;
}
#endif /* TPP_HAVE_LEXER_INIT_FILENAME */


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer-warn.c                                             */
/************************************************************************/
#if TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
#include <stdio.h>
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */

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
	return (*printer)(arg, (tpp_char const *)start, (tpp_size)(buf + tpp_lengthof(buf) - start));
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_print_int(tpp_formatprinter printer, void *arg, tpp_intmax value) {
	char buf[TPP_ITOA_MAXLEN];
	char const *start = tpp_itoa(buf, value);
	return (*printer)(arg, (tpp_char const *)start, (tpp_size)(buf + tpp_lengthof(buf) - start));
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
			                            ch == 'l' ? tpp_lcinfo_getline(lcinfo)
			                                      : tpp_lcinfo_getcol(lcinfo));
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
	tpp_errno result;
	tpp_ssize printer_status;
	tpp_file *const file = tpp_lexer_getfile(self);
	char const *warning_format;
	struct tpp_warning_invokeinfo invokeinfo;
	tpp_formatprinter printer;
	void *printer_arg;

	/* Ask warning configuration how we should have this one */
	result = tpp_warnings_invoke(tpp_lexer_getwarn(self), id, &invokeinfo);
#if TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
	if (result != TPP_EOK)
		goto done;
#endif /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */

	/* Deal with certain warning states. */
	switch (invokeinfo.twii_state) {
	case TPP_WSTATE_DISABLED:
		goto done; /* Nothing to do here */
	case TPP_WSTATE_WARN:
		/* Display as a warning */
		break;
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
	printer_status = tpp_lexer_printf_warning(self, file,
	                                          pos, printer, printer_arg,
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
#define tpp_warnf(...)                                                       \
	do {                                                                     \
		printer_status = tpp_lexer_printf_warning(self, file,                \
		                                          pos, printer, printer_arg, \
		                                          __VA_ARGS__);              \
		if (printer_status < 0)                                              \
			goto err_printer;                                                \
	} while (0)
/* ... */
/************************************************************************/

#define TPP_DEFS
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, expr) \
		case warning_id: {                                    \
			expr;                                             \
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

#if TPP_HAVE_INCLUDE_STACK
//TODO:	{
//TODO:		tpp_file *caller = file->tf_prev;
//TODO:		for (; caller; caller = caller->tf_prev) {
//TODO:			printer_status = tpp_lexer_printf_warning(self, file, pos, printer, printer_arg,
//TODO:			                                          TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT);
//TODO:
//TODO:		}
//TODO:	}
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

/* clang-format off */
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
/* clang-format on */
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


#define TPP_UTF8_1BYTE_MAX ((UINT32_C(1) << 7) - 1)
#define TPP_UTF8_2BYTE_MAX ((UINT32_C(1) << 11) - 1)
#define TPP_UTF8_3BYTE_MAX ((UINT32_C(1) << 16) - 1)

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
		if (error != TPP_EOK)
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
		if (error != TPP_EOK)
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
		if (error != TPP_EOK)
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
		if (error != TPP_EOK)
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
		if (error != TPP_EOK)
			goto return_error;
	}
#endif /* TPP_HAVE_BSE_WHITESPACE */
	if tpp_unlikely(scan >= file->tf_end) {
		/* Must extend file! */
		tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
		error = tpp_file_expandchunk(file);
		if (error != TPP_EOK)
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
				if (error != TPP_EOK)
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
			if (error != TPP_EOK)
				goto return_error;
		}
#endif /* TPP_HAVE_TRIGRAPHS && TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */

		/* Check for further BSE */
		if tpp_unlikely(scan >= file->tf_end) {
			/* Must extend file! */
			tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
			error = tpp_file_expandchunk(file);
			if (error != TPP_EOK)
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
					if (error != TPP_EOK)
						goto return_error;
					scan = tpp_file_rel2ptr(file, rel_scan);
				}
				if (scan[1] == '?') {
					if ((scan + 2) >= file->tf_end) {
						tpp_size rel_scan = tpp_file_ptr2rel(file, scan);
						error = tpp_file_expandchunk(file);
						if (error != TPP_EOK)
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
		if (error != TPP_EOK)
			goto return_error;
		if (tpp_unicode_issymcont(uc)) {
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
static tpp_errno TPPCALL
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
		if (error != TPP_EOK)
			goto return_error;
		pos = tpp_file_rel2ptr(file, rel_pos);
		end = file->tf_end;
		if (pos >= end)
			goto done;
	}

	ch = *pos;
	if (tpp_ascii_issymcont(ch)) {
		++pos;
		goto again;
	}

#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(file) && ch >= 0x80) {
		tpp_unichar uc;
		tpp_char const *npos = pos;
		error = tpp_lexer_readutf8(self, &npos, &uc);
		if (error != TPP_EOK)
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
static tpp_errno TPPCALL
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
				if (error != TPP_EOK)
					return error;
				pos = tpp_file_rel2ptr(file, rel_pos);
			}
			if ((pos + 1) < file->tf_end && pos[1] == '?') {
				if ((pos + 2) >= file->tf_end) {
					tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
					error = tpp_file_expandchunk(file);
					if (error != TPP_EOK)
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
		if (error != TPP_EOK)
			return error;
		if (npos == pos)
			break;
		rel_before = tpp_file_ptr2rel(file, npos);
		error = tpp_lexer_seek_end_of_keyword(self, &npos);
		if (error != TPP_EOK)
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
		if (error != TPP_EOK)
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
		if (error != TPP_EOK)
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
			if (error != TPP_EOK)
				return error;
			pos = tpp_file_rel2ptr(file, rel_pos);
			end = file->tf_end;
		}
		if (pos < end && *pos == '?') {
			++pos;
			if (pos >= end) {
				tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
				tpp_errno error  = tpp_file_expandchunk(file);
				if (error != TPP_EOK)
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
						if (error != TPP_EOK)
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
	if (error == TPP_EOK && ch >= 0x80) {
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
		if (error != TPP_EOK)
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
				if (error != TPP_EOK)
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
		if (error != TPP_EOK)
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
		if (error != TPP_EOK)
			goto done;
		rel_after_bse = tpp_file_ptr2rel(file, pos);
		tpp_assert(rel_before_bse <= rel_after_bse);
		if (rel_before_bse >= rel_after_bse)
			goto again; /* Not a BSE sequence */

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
					if (error != TPP_EOK)
						goto done;
					if (pos >= file->tf_end)
						goto done; /* True EOF */
				}
				ch = *pos++;
				if (tpp_ascii_islf(ch)) {
					error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_before_bse),
					                           TPP_W_LINE_COMMENT_CONTINUED);
					if (error != TPP_EOK)
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
					if (error != TPP_EOK)
						goto done;
					if (tpp_unicode_islf(ch)) {
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
							if (error != TPP_EOK)
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
							if (error != TPP_EOK)
								goto done;
							if (pos >= file->tf_end)
								break;
						}
						if (pos[0] == '?') {
							if ((pos + 1) >= file->tf_end) {
								tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
								error = tpp_file_expandchunk(file);
								pos = tpp_file_rel2ptr(file, rel_pos);
								if (error != TPP_EOK)
									goto done;
								if ((pos + 1) >= file->tf_end)
									break;
							}
							if (pos[1] == '=') {
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
								error = tpp_lexer_warnf_at(self, pos - 1, TPP_W_ENCOUNTERED_TRIGRAPH);
								if (error != TPP_EOK)
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
							if (error != TPP_EOK)
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
				if (error != TPP_EOK)
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
			if (error != TPP_EOK)
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
			if (error != TPP_EOK)
				goto done;
			if ((pos + 1) >= file->tf_end)
				goto again;
		}
		if (pos[1] != '/')
			goto again;
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
		error = tpp_lexer_warnf_at(self, pos - 1, TPP_W_ENCOUNTERED_TRIGRAPH);
		if (error != TPP_EOK)
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
		if (error != TPP_EOK)
			return error;
		if (ch == quote_char)
			break;
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
		if (ch == '\\') {
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (error != TPP_EOK)
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
			if (error != TPP_EOK)
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
		if (error != TPP_EOK)
			return error;
		if (ch == quote_char) {
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (error != TPP_EOK)
				return error;
			if (ch == quote_char) {
				error = tpp_lexer_readchar(self, p_pos, &ch);
				if (error != TPP_EOK)
					return error;
				if (ch == quote_char)
					break; /* triple quote_char -> end block-string */
			}
		}
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
		if (ch == '\\') {
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (error != TPP_EOK)
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
	tpp_size rel_pattern_end;
	tpp_char ch;
	tpp_errno error;

	/* Find end of pattern string */
	for (;;) {
		rel_pattern_end = tpp_file_ptr2rel(file, *p_pos);
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (error != TPP_EOK)
			return error;
		if (ch == '(')
			break;
		if (ch == 0 && (*p_pos) >= file->tf_end)
			goto warn_premature_eof;
	}

	/* Scan string body... */
	for (;;) {
		tpp_size rel_pattern_iter;
		tpp_char const *pos2;
continue_string:
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (error != TPP_EOK)
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
			if (error != TPP_EOK)
				return error; /* Shouldn't cause errors (was already scanned before) */

			/* Read input character */
			error = tpp_lexer_readchar(self, p_pos, &ch);
			if (error != TPP_EOK)
				return error;
			if (pattern_ch != ch)
				goto continue_string;
		}

		/* Entire pattern was matched -> next character must be '"' */
		error = tpp_lexer_readchar(self, p_pos, &ch);
		if (error != TPP_EOK)
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
		if (error != TPP_EOK)
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
			if (error != TPP_EOK)
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
 * @return: * :               The newly read token
 * @return: TPP_TOK_ISERR(*): Error (s.a. `TPP_TOK_ASERR(return)' and `enum tpp_errno') */
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
 * This is used to implement `tpp_lexer_yieldraw()', which simply passes `' */
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
		if (error != TPP_EOK)                         \
			goto return_error;                        \
	} while (0)
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
	/* Call this after 'read_ch2()' returned a
	 * (used) character that is one of: #[]^{|}~?\ */
#define warn_if_ch2_is_trigraph()                                                  \
	do {                                                                           \
		if (pos[-1] != ch2) {                                                      \
			error = tpp_lexer_warnf_at(self, pos - 3, TPP_W_ENCOUNTERED_TRIGRAPH); \
			if (error != TPP_EOK)                                                  \
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
     TPP_HAVE_TPP_TOK_LANGLE_RANGLE || TPP_HAVE_DIGRAPHS)
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LANGLE_RANGLE) &&
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
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '>': {
#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||         \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||  \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL))
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
		{
		}
#endif /* ... */
	}	break;
/************************************************************************/



/************************************************************************/
	case '=': {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL))
			break;
		read_ch2();
		if (ch2 == '=') {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL)) {
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
				tpp_size rel_end_of_2char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
				read_ch2();
				if (ch2 == '=') {
					result = TPP_TOK_EQUAL_EQUAL_EQUAL; /* "===" */
					goto set_result;
				}
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
				pos = tpp_file_rel2ptr(file, rel_end_of_2char);
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_EQUAL_EQUAL)) {
				result = TPP_TOK_EQUAL_EQUAL; /* "==" */
				goto set_result;
			}
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
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
				if (error != TPP_EOK)
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
#if (TPP_HAVE_TPP_TOK_STAR_EQUAL || \
     TPP_HAVE_TPP_TOK_STAR_STAR ||  \
     TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL)
		if (!tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_EQUAL) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_STAR) &&
		    !tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL))
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
				if (error != TPP_EOK)
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
						if (error != TPP_EOK)
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
							if (error != TPP_EOK)
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
				if (error != TPP_EOK)
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
			if (error != TPP_EOK)
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
				if (error != TPP_EOK)
					goto return_error;
				pos = tpp_file_rel2ptr(file, rel_start + 1);
			}
			if (pos < file->tf_end && *pos == '?') {
				if ((pos + 1) >= file->tf_end) {
					error = tpp_file_expandchunk(file);
					if (error != TPP_EOK)
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
					if (error != TPP_EOK)
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
				if (error != TPP_EOK)
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
#if TPP_HAVE_BSE
		tpp_char const *npos;
		tpp_size rel_before, rel_after;
		npos = tpp_file_rel2ptr(file, rel_start);
		rel_before = tpp_file_ptr2rel(file, npos);
		error = tpp_lexer_skip_bse(self, &npos);
		if (error != TPP_EOK)
			goto return_error;
		rel_after = tpp_file_ptr2rel(file, npos);
		tpp_assert(rel_before <= rel_after);
		if (rel_before >= rel_after)
			break; /* No BSE -> regular backslash */

		/* BSE was skipped -> read whatever comes after... */
		*p_pos = npos;
		goto again;
#endif /* TPP_HAVE_BSE */
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
					if (error != TPP_EOK)
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
			if (error != TPP_EOK)
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
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL)) {
#if TPP_HAVE_TPP_TOK_STRING
			tpp_size rel_end_of_1char = tpp_file_ptr2rel(file, pos);
#endif /* TPP_HAVE_TPP_TOK_STRING */
			read_ch2();
			if (ch2 == '"') {
				read_ch2();
				if (ch2 == '"') {
					error = tpp_lexer_seek_end_of_block_string(self, &pos, '"');
					if (error != TPP_EOK)
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
			if (error != TPP_EOK)
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
					if (error != TPP_EOK)
						goto return_error;
					result = TPP_TOK_CXX_RAW_STRING_LITERAL; /* R"AB(foo)AB" */
					goto set_result;
				}
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
				if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL)) {
					error = tpp_lexer_seek_end_of_raw_string(self, &pos, '"');
					if (error != TPP_EOK)
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
					if (error != TPP_EOK)
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
					if (error != TPP_EOK)
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
					if (error != TPP_EOK)
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
				if (error != TPP_EOK)
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
						if (error != TPP_EOK)
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
						if (error != TPP_EOK)
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
								if (error != TPP_EOK)
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
					if (error != TPP_EOK)
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
						if (error != TPP_EOK)
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
				if (error != TPP_EOK)
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
						if (error != TPP_EOK)
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


	default: {
#if TPP_HAVE_UNICODE
		/* Check for utf-8 sequence */
		if (tpp_file_isutf8(file) && ch >= 0x80) {
			tpp_unichar uc;
			--pos;
			error = tpp_lexer_readutf8(self, &pos, &uc);
			if (error != TPP_EOK)
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
					if (error != TPP_EOK)
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
			if (error != TPP_EOK)
				goto return_error;
#if TPP_HAVE_BSE
			while (pos < file->tf_end && *pos == '\\') {
				tpp_char const *npos;
				tpp_size rel_before, rel_after;
				npos = pos;
				error = tpp_lexer_skip_bse(self, &npos);
				if (error != TPP_EOK)
					goto return_error;
				if (npos == pos)
					break;
				rel_before = tpp_file_ptr2rel(file, npos);
				error = tpp_lexer_skipspace_nolf(self, &npos);
				if (error != TPP_EOK)
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
#if TPP_HAVE_BSE
			bool uses_bse;
#endif /* TPP_HAVE_BSE */
#ifdef WANT_handle_keyword
#undef WANT_handle_keyword
handle_keyword:
#endif /* WANT_handle_keyword */
			error = tpp_lexer_seek_end_of_keyword(self, &pos);
			if (error != TPP_EOK)
				goto return_error;
#if TPP_HAVE_BSE
			uses_bse = false;
			while (pos < file->tf_end) {
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
						if (error != TPP_EOK)
							goto return_error;
						pos = tpp_file_rel2ptr(file, rel_pos);
					}
					if ((pos + 1) < file->tf_end && pos[1] == '?') {
						if ((pos + 2) >= file->tf_end) {
							tpp_size rel_pos = tpp_file_ptr2rel(file, pos);
							error = tpp_file_expandchunk(file);
							if (error != TPP_EOK)
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
					break;
				}
				npos = pos;
				error = tpp_lexer_skip_bse(self, &npos);
				if (error != TPP_EOK)
					goto return_error;
				if (npos == pos)
					break;
				rel_before = tpp_file_ptr2rel(file, npos);
				error = tpp_lexer_seek_end_of_keyword(self, &npos);
				if (error != TPP_EOK)
					goto return_error;
				rel_after  = tpp_file_ptr2rel(file, npos);
				tpp_assert(rel_before <= rel_after);
				if (rel_before >= rel_after)
					break;
				uses_bse = true;
				pos      = npos;
			}
#endif /* TPP_HAVE_BSE */

			/* Lookup/create keyword */
			kwd_start = tpp_file_rel2ptr(file, rel_start);
			kwd_len   = (tpp_size)(pos - kwd_start);
#if TPP_HAVE_BSE
			if (uses_bse) {
				kwd_hash = tpp_hashof_bse(kwd_start, kwd_len, file);
				kwd = tpp_keywords_newkeyword_bse(&self->tl_kwds, kwd_start, kwd_len, kwd_hash, file);
			} else
#endif /* TPP_HAVE_BSE */
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
			if (error != TPP_EOK)
				goto return_error;
#if TPP_HAVE_BSE
			error = tpp_lexer_skip_bse_after_keyword(self, &pos);
			if (error != TPP_EOK)
				goto return_error;
#endif /* TPP_HAVE_BSE */

#if TPP_HAVE_TPP_TOK_FLOAT
			if (pos < file->tf_end && *pos == '.' &&
			    tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_FLOAT)) {
				/* Floating point token... */
				++pos;
				error = tpp_lexer_seek_end_of_keyword(self, &pos);
				if (error != TPP_EOK)
					goto return_error;
#if TPP_HAVE_BSE
				error = tpp_lexer_skip_bse_after_keyword(self, &pos);
				if (error != TPP_EOK)
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
	if (error != TPP_EOK)
		goto return_error;
	pos = tpp_file_rel2ptr(file, rel_start);
	end = file->tf_end;
	if (pos < end)
		goto again_read_from_pos;

	/* Check if we can pop to another file */
#if TPP_HAVE_INCLUDE_STACK
	if (file->tf_prev && p_pos == &file->tf_pos &&
	    !(self->tl_state & TPP_LEXER_STATE_FLAG_NOPOPFILE)) {
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


#if TPP_HAVE_CPP_PRAGMA
/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword
 * @return: TPP_ENOENT: Unknown pragma (warning was already emitted; caller
 *                      should seek until after macro) */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);

TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	struct tpp_keyword const *kwd = token->tt_kwd;
	tpp_token_id tok = token->tt_id;
	if (!TPP_TOK_ISKEYWORD(tok))
		goto unknown_pragma;
#if TPP_HAVE_PRAGMA_PUSH_MACRO
	if (tpp_keyword_equals_cstr(kwd, "push_macro") ||
	    tpp_keyword_equals_cstr(kwd, "pop_macro")) {
		if (tpp_lexer_getext(self, TPP_EXT_PRAGMA_PUSH_MACRO)) {
			bool is_push = kwd->tk_kwd[1] == 'u';
			tok = tpp_lexer_yield(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			(void)is_push;
			/* TODO: skip "(" */
			/* TODO: Parse string */
			/* TODO: skip ")" */
		}
	} else
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_PRAGMA_ONCE
	if (tpp_keyword_equals_cstr(kwd, "once")) {
		tpp_file const *file = tpp_file_getiofile(tpp_lexer_getfile(self));
		tpp_keyword *file_kwd = tpp_file_filename_kwd(file);
		if (file_kwd) {
			tpp_keyword_misc *misc;
			misc = tpp_keyword_requiremisc(file_kwd);
			if tpp_unlikely(!misc)
				return TPP_ENOMEM;
			misc->tkm_flags |= TPP_KEYWORD_FLAG_HDR_ONCE;
		}
		tok = tpp_lexer_yieldraw(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		return TPP_EOK;
	} else
#endif /* TPP_HAVE_PRAGMA_ONCE */
	{
	}

/* Support for: #pragma deprecated("foo") */
#ifndef TPP_HAVE_PRAGMA_DEPRECATED
#define TPP_HAVE_PRAGMA_DEPRECATED (TPP_COMMON_HAVE_PRAGMA ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_DEPRECATED */

/* Support for: #pragma GCC poison foo */
#ifndef TPP_HAVE_PRAGMA_GCC_POISON
#define TPP_HAVE_PRAGMA_GCC_POISON (TPP_COMMON_HAVE_PRAGMA_GCC ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_GCC_POISON */

/* Support for: #pragma tpp_set_keyword_flags */
#ifndef TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS (TPP_COMMON_HAVE_PRAGMA ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

/* Support for: #pragma GCC system_header */
#ifndef TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#define TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER (TPP_COMMON_HAVE_PRAGMA_GCC ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

/* Support for: #pragma extension(...) */
#ifndef TPP_HAVE_PRAGMA_EXTENSION
#define TPP_HAVE_PRAGMA_EXTENSION ((TPP_COMMON_HAVE_PRAGMA && TPP_HAVE_EXTENSIONS) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_EXTENSION */

/* Support for: #pragma extension(push) */
#ifndef TPP_HAVE_PRAGMA_EXTENSION_PUSH
#define TPP_HAVE_PRAGMA_EXTENSION_PUSH ((TPP_HAVE_PRAGMA_EXTENSION && TPP_HAVE_EXTENSIONS_PUSH_POP) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_EXTENSION_PUSH */


unknown_pragma:
	/* TODO: Warning */
	return TPP_ENOENT;
}

/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_directive(tpp_lexer *tpp_restrict self) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_errno error = tpp_lexer_process_pragma(self);
	if (error != TPP_EOK) {
		if (error == TPP_ENOENT)
			goto skip_garbage_without_warning;
		return error;
	}
	while (TPP_TOK_ISSPACE_OR_COMMENT(token->tt_id)) {
		tpp_token_id tok = tpp_lexer_yieldraw(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}
	if (TPP_TOK_ISLF_OR_COMMENT(token->tt_id))
		return TPP_EOK;
	if (token->tt_id == TPP_TOK_EOF)
		return TPP_EOK;
	/* TODO: Warning: garbage after pragma */
skip_garbage_without_warning:
	for (;;) {
		tpp_token_id tok = tpp_lexer_yieldraw(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (TPP_TOK_ISLF_OR_COMMENT(tok))
			break;
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_CPP_PRAGMA */

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
	result = tpp_lexer_yieldraw_at(self, &directive_iter);
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
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_EXCLAIM))
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
			if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_INCLUDE))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_INCLUDE */
#if TPP_HAVE_CPP_INCLUDE_NEXT
		if (0) {
	case TPP_KWD_include_next:
			if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_INCLUDE_NEXT))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
#if TPP_HAVE_CPP_IMPORT
		if (0) {
	case TPP_KWD_import:
			if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IMPORT))
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
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line

	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line

	case TPP_KWD_else:
	case TPP_KWD_endif:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DEFINE
	case TPP_KWD_define:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DEFINE))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line

	case TPP_KWD_undef:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DEFINE))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_DEFINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ASSERT
	case TPP_KWD_assert:
	case TPP_KWD_unassert:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_ASSERT))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_ASSERT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
	{
		tpp_size rel_message_start;
		tpp_size rel_message_end;
		tpp_char const *message_start;
		tpp_char const *message_end;
		tpp_errno error;
#if TPP_HAVE_CPP_ERROR
		if (0) {
	case TPP_KWD_error:
			if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_ERROR))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
		if (0) {
	case TPP_KWD_warning:
			if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_WARNING))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_WARNING */
		rel_message_start = tpp_file_ptr2rel(file, directive_iter);
		error = tpp_lexer_seek_eol(self, &directive_iter tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
		rel_message_end = tpp_file_ptr2rel(file, directive_iter);
		if (error != TPP_EOK)
			return TPP_TOK_OFERR(error);

		/* Remember that this is where the next token should begin. */
		token->tt_end = directive_iter;
		message_start = tpp_file_rel2ptr(file, rel_message_start);
		message_end   = tpp_file_rel2ptr(file, rel_message_end);

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

		/* TODO: Emit error/warning message */
		break;
	}
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IDENT
	case TPP_KWD_ident:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IDENT))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_IDENT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_SCSS
	case TPP_KWD_scss:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_SCSS))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_SCSS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_PRAGMA
	case TPP_KWD_pragma: {
		tpp_errno error;
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_PRAGMA))
			goto handle_unknown_directive;
		do {
			result = tpp_lexer_yieldraw(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		error = tpp_lexer_process_pragma_directive(self);
		if (error != TPP_EOK)
			return TPP_TOK_OFERR(error);
		while (TPP_TOK_ISSPACE_OR_COMMENT(token->tt_id)) {
			result = tpp_lexer_yieldraw(self);
			if (TPP_TOK_ISERR(result))
				return result;
		}

		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}
#endif /* TPP_HAVE_CPP_PRAGMA */
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
			if (error != TPP_EOK) {
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
			/* TODO: Warning: Unknown directive */
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0 */
	
			/* Seek until we hit LF or EOF. Caller has disabled "autopopfile",
			 * so this'll always stay within the file currently being processed. */
#ifdef WANT_seek_end_of_line
#undef WANT_seek_end_of_line
seek_end_of_line:
#endif /* WANT_seek_end_of_line */
			while (result != TPP_TOK_LF && result != TPP_TOK_EOF) {
				result = tpp_lexer_yieldraw(self);
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
 * @return: * :               The newly read token (after accounting for preprocessor directives)
 * @return: TPP_TOK_ISERR(*): Error (s.a. `TPP_TOK_ASERR(return)' and `enum tpp_errno') */
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
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT && TPP_HAVE_CPP_DIRECTIVES */
		/* FALLTHRU */
#if TPP_HAVE_TPP_TOK_COMMENT <= 0 /* Always, or conditionally disabled */
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_ASM_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES */
		/* FALLTHRU */
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
#if TPP_HAVE_TPP_TOK_COMMENT < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COMMENT))
			break; /* Enabled */
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

/* Wrapper around `tpp_lexer_yieldpp()' that adds handling for macro expansion.
 * @return: * :               The newly read token (after accounting for macros)
 * @return: TPP_TOK_ISERR(*): Error (s.a. `TPP_TOK_ASERR(return)' and `enum tpp_errno') */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield(tpp_lexer *tpp_restrict self) {
	tpp_token_id result = tpp_lexer_yieldpp(self);
	switch (result) {

	/* TODO */

	default: break;
	}
	return result;
}


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
tpp_token_decodestring_basic(tpp_lexer const *tpp_restrict self,
                             tpp_char const *start,
                             tpp_char const *end,
                             tpp_formatprinter data_printer,
                             tpp_formatprinter utf8_printer,
                             void *arg) {
	tpp_assert(start <= end);
	/* TODO */
	(void)self;
	(void)start;
	(void)end;
	(void)data_printer;
	(void)utf8_printer;
	(void)arg;
	return 0;
}
#endif /* ... */


#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
/* Decode string:
 * |"""
 * |.   foobar fdasudfad
 * |    fasdf\
 * |"""      ^
 *           ^end
 *  ^ start=.
 *
 */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4, 5)) tpp_ssize TPPCALL
tpp_token_decodestring_block(tpp_lexer const *tpp_restrict self,
                             tpp_char const *start,
                             tpp_char const *end,
                             tpp_formatprinter data_printer,
                             tpp_formatprinter utf8_printer,
                             void *arg) {
	tpp_assert(start <= end);
	/* TODO */
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
tpp_token_decodestring_raw(tpp_lexer const *tpp_restrict self,
                           tpp_char const *start,
                           tpp_char const *end,
                           tpp_formatprinter data_printer,
                           void *arg) {
	/* TODO: Print input as-is, but skip over BSE */
	tpp_assert(start <= end);
	/* TODO */
	(void)self;
	(void)start;
	(void)end;
	(void)data_printer;
	(void)arg;
	return 0;
}
#else
#define tpp_token_decodestring_raw_SKIPS_BSE 0
#define tpp_token_decodestring_raw(self, start, end, data_printer, arg) \
	(*(data_printer))(arg, start, (tpp_size)((end) - (start)))
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
 * The caller must ensure that `TPP_TOK_ISSTRING(self->tt_id)'
 * @param: data_printer: Printer used to fast-forward string data from token inputs, as well as \xAB
 * @param: utf8_printer: Printer used to emit explicitly utf-8 encoded data from \uABCD and \U876543210 */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_ssize TPPCALL
tpp_lexer_decodestring(tpp_lexer const *tpp_restrict self,
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
#endif /* ... */
#else /* TPP_HAVE_BSE */
#if (TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL)
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
		++start;
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
		/* FALLTHRU */
	_TPP_CASE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF32_STRING_LITERAL
		++start;
#else /* ... */
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
		start += 2;
#endif /* ... */
#endif /* !TPP_HAVE_BSE */
		/* FALLTHRU */
	_TPP_CASE_TPP_TOK_CHAR
	_TPP_CASE_TPP_TOK_STRING {
		++start; /* Skip leading quote */
		--end;   /* Skip trailing quote */
		tpp_assert(start <= end);
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
#endif /* TPP_HAVE_TPP_TOK_CHAR || TPP_HAVE_TPP_TOK_STRING */

#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL {
		while (*start != '(')
			++start;
		while (end[-1] != ')')
			--end;
		tpp_assert(start < end);
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
		goto cxx_raw_string_common;
#else /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL || TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
		++start; /* Skip over leading '(' */
		--end;   /* Skip over trailing ')' */

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
		tpp_assert(end[-1] == '"' || end[-1] == '\'');
		++start; /* Skip leading 'R' / 'r' */
		tpp_bse_seek_until_fwd(start, end[-1]);
		tpp_assert(*start == end[-1]);
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
cxx_raw_string_common:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
		++start; /* Skip leading '"' / '\'' */
		--end;   /* Skip trailing '"' / '\'' */

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
		tpp_assert(end[-1] == quote_ch);
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
		tpp_assert(start < end);
		tpp_assert(end[-1] == quote_ch);
		--end; /* Skip second quote */
		tpp_bse_seek_until_bck(end, quote_ch);
		tpp_assert(start < end);
		tpp_assert(end[-1] == quote_ch);
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
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE */


TPP_DECL_END
/************************************************************************/


#endif /* !GUARD_TPP_C */
/* clang-format on */
