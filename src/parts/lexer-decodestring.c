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
			how = tpp_lexer_parsestring_is_single_chunk(self);
			if (how == TPP_LEXER_PARSESTRING_IS_SINGLE_CHUNK_EMPTY)
				goto again_yield_after_single;

			/* Not possible using a single chunk... */
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

		default:
			if (TPP_TOK_ISERR(tok)) {
				tpp_lexer_nowarnings_break(self);
				tpp_lexer_seek_rollback(self, &backup);
				return TPP_TOK_ASERR(tok);
			}
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
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_DECODESTRING_C */
