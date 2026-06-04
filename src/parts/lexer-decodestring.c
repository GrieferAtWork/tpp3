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
#ifndef GUARD_TPP_LEXER_DECODESTRING_C
#define GUARD_TPP_LEXER_DECODESTRING_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "error.h"
#include "string.h"
#include "lexer.h"
#include "token.h"

/*[[[tpp-begin]]]*/
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
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_DECODESTRING_C */
