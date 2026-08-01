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
#include "expr.h"
#include "lexer.h"
#include "preparse.h"
#include "string.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_DECODESTRING

/* Return the printer that should be used by default. */
#if TPP_HAVE_UNICODE
#define tpp_lexer_decodestring_config_getdefl(self, lexer) \
	(tpp_file_isutf8(tpp_lexer_getfile(lexer))             \
	 ? (self)->tldsc_utf8printer                           \
	 : (self)->tldsc_dataprinter)
#define tpp_lexer_decodestring_config_getutf8(self) ((self)->tldsc_utf8printer)
#else /* TPP_HAVE_UNICODE */
#define tpp_lexer_decodestring_config_getdefl(self, lexer) ((self)->tldsc_dataprinter)
#define tpp_lexer_decodestring_config_getutf8(self)        ((self)->tldsc_dataprinter)
#endif /* !TPP_HAVE_UNICODE */




#if TPP_HAVE_STRING_ESCAPE

#if TPP_HAVE_STRING_ESCAPE_OCT_BRACE
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_ssize TPPCALL
tpp_token_decodestring_oct_sequence(tpp_lexer *self, tpp_char const **p_iter, tpp_char const *end,
                                    tpp_lexer_decodestring_config const *tpp_restrict config) {
	tpp_ssize result;
	tpp_uintmax bigword;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_char const *iter = *p_iter;
	tpp_char ch = *iter;
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
	bool has_overflow = false;
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
	tpp_assert(tpp_ascii_isoctdigit(ch));
	bigword = tpp_ascii_asoctdigit(ch);
	for (;;) {
		++iter;
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
		if (iter >= end)
			break;
		ch = *iter;
		if (!tpp_ascii_isoctdigit(ch))
			break;
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
		if (((bigword << 3) >> 3) != bigword)
			has_overflow = true;
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
		bigword <<= 3;
		bigword |= tpp_ascii_asoctdigit(ch);
	}
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
	if ((bigword > 0xff && !config->tldsc_bigprinter) || has_overflow)
#else /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
	if ((bigword > 0xff) || has_overflow)
#endif /* !TPP_HAVE_STRING_ESCAPE_BIGCHAR */
	{
		tpp_errno error;
		tpp_char const *saved_start = token->tt_start;
		tpp_char const *saved_end = token->tt_end;
		token->tt_start = *p_iter;
		token->tt_end   = iter;
		error = tpp_lexer_warnf(self, TPP_W_CHARACTER_TOO_LARGE);
		token->tt_start = saved_start;
		token->tt_end   = saved_end;
		if (TPP_ISERR(error)) {
			result = TPP_SSIZE_OFERR(error);
			goto done;
		}
	}
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
	if (bigword > 0xff && config->tldsc_bigprinter) {
		tpp_char const *saved_start = token->tt_start;
		tpp_char const *saved_end = token->tt_end;
		token->tt_start = *p_iter;
		token->tt_end   = iter;
		result = (*config->tldsc_bigprinter)(config->tldsc_arg, self, bigword);
		token->tt_start = saved_start;
		token->tt_end   = saved_end;
		goto done;
	}
#endif /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
	ch = (tpp_char)bigword;
	result = (*config->tldsc_dataprinter)(config->tldsc_arg, &ch, 1);
done:
	*p_iter = iter;
	return result;
}
#endif /* TPP_HAVE_STRING_ESCAPE_OCT_BRACE */

#if TPP_HAVE_STRING_ESCAPE_HEX_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BIG
#if TPP_HAVE_STRING_ESCAPE_HEX_BRACE && TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_HEX_BIG)
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_ssize TPPCALL
tpp_token_decodestring_hex_sequence_ex(tpp_lexer *self, tpp_char const **p_iter, tpp_char const *end,
                                       tpp_lexer_decodestring_config const *tpp_restrict config,
                                       bool always_allow_big)
#define tpp_token_decodestring_hex_sequence(self, p_iter, end, config) \
	tpp_token_decodestring_hex_sequence_ex(self, p_iter, end, config, false)
#else /* TPP_HAVE_STRING_ESCAPE_HEX_BRACE && TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_HEX_BIG) */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_ssize TPPCALL
tpp_token_decodestring_hex_sequence(tpp_lexer *self, tpp_char const **p_iter, tpp_char const *end,
                                    tpp_lexer_decodestring_config const *tpp_restrict config)
#define tpp_token_decodestring_hex_sequence_ex(self, p_iter, config, always_allow_big) \
	tpp_token_decodestring_hex_sequence(self, p_iter, config)
#endif /* !TPP_HAVE_STRING_ESCAPE_HEX_BRACE || !TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_HEX_BIG) */
{
	tpp_char word;
	tpp_char const *iter = *p_iter;
	tpp_char ch = *iter++;
	tpp_assert(tpp_ascii_isxdigit(ch));
	if (tpp_ascii_isdigit(ch)) {
		word = (tpp_char)tpp_ascii_asdigit(ch);
	} else if (tpp_ascii_islwrxdigit(ch)) {
		word = (tpp_char)tpp_ascii_aslwrxdigit(ch);
	} else {
		tpp_assert(tpp_ascii_isuprxdigit(ch));
		word = (tpp_char)tpp_ascii_asuprxdigit(ch);
	}
	iter = tpp_preparse_skipbse_fwd(self, iter, end);
	if (iter < end) {
		ch = *iter;
		if (tpp_ascii_isdigit(ch)) {
			word <<= 4;
			word |= (tpp_char)tpp_ascii_asdigit(ch);
			++iter;
		} else if (tpp_ascii_islwrxdigit(ch)) {
			word <<= 4;
			word |= (tpp_char)tpp_ascii_aslwrxdigit(ch);
			++iter;
		} else if (tpp_ascii_isuprxdigit(ch)) {
			word <<= 4;
			word |= (tpp_char)tpp_ascii_asuprxdigit(ch);
			++iter;
		} else {
#if TPP_HAVE_STRING_ESCAPE_HEX_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BIG
			goto print_word_as_byte;
#endif /* TPP_HAVE_STRING_ESCAPE_HEX_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BIG */
		}
#if TPP_HAVE_STRING_ESCAPE_HEX_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BIG
#if TPP_HAVE_STRING_ESCAPE_HEX_BRACE && TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_HEX_BIG)
		if (tpp_lexer_has(self, STRING_ESCAPE_HEX_BIG) || always_allow_big)
#else /* TPP_HAVE_STRING_ESCAPE_HEX_BRACE && TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_HEX_BIG) */
		if (tpp_lexer_has(self, STRING_ESCAPE_HEX_BIG))
#endif /* !TPP_HAVE_STRING_ESCAPE_HEX_BRACE || !TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_HEX_BIG) */
		{
			iter = tpp_preparse_skipbse_fwd(self, iter, end);
			if (iter < end) {
				ch = *iter;
				if (tpp_ascii_isxdigit(ch)) {
					tpp_token *const token = tpp_lexer_gettoken(self);
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
					bool has_overflow = false;
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
					tpp_uintmax bigword = word;
					++iter;
					for (;;) {
						tpp_char nibble;
						if (tpp_ascii_isdigit(ch)) {
							nibble = (tpp_char)tpp_ascii_asdigit(ch);
						} else if (tpp_ascii_islwrxdigit(ch)) {
							nibble = (tpp_char)tpp_ascii_aslwrxdigit(ch);
						} else if (tpp_ascii_isuprxdigit(ch)) {
							nibble = (tpp_char)tpp_ascii_asuprxdigit(ch);
						} else {
							--iter;
							break;
						}
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
						if (((bigword << 4) >> 4) != bigword)
							has_overflow = true;
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
						bigword <<= 4;
						bigword |= nibble;
						iter = tpp_preparse_skipbse_fwd(self, iter, end);
						if (iter >= end)
							break;
						ch = *iter++;
					}
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
					if ((bigword > 0xff && !config->tldsc_bigprinter) || has_overflow)
#else /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
					if ((bigword > 0xff) || has_overflow)
#endif /* !TPP_HAVE_STRING_ESCAPE_BIGCHAR */
					{
						tpp_errno error;
						tpp_char const *saved_start = token->tt_start;
						tpp_char const *saved_end = token->tt_end;
						token->tt_start = *p_iter;
						token->tt_end   = iter;
						error = tpp_lexer_warnf(self, TPP_W_CHARACTER_TOO_LARGE);
						token->tt_start = saved_start;
						token->tt_end   = saved_end;
						if (TPP_ISERR(error)) {
							*p_iter = iter;
							return TPP_SSIZE_OFERR(error);
						}
					}
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
					if (bigword > 0xff && config->tldsc_bigprinter) {
						tpp_ssize result;
						tpp_char const *saved_start = token->tt_start;
						tpp_char const *saved_end = token->tt_end;
						token->tt_start = *p_iter;
						token->tt_end   = iter;
						result = (*config->tldsc_bigprinter)(config->tldsc_arg, self, bigword);
						token->tt_start = saved_start;
						token->tt_end   = saved_end;
						*p_iter = iter;
						return result;
					}
#endif /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
					word = (tpp_char)bigword;
					/* Fallthru to "print_word_as_byte" below... */
				}
			}
		}
#endif /* TPP_HAVE_STRING_ESCAPE_HEX_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BIG */
	}
#if TPP_HAVE_STRING_ESCAPE_HEX_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BIG
print_word_as_byte:
#endif /* TPP_HAVE_STRING_ESCAPE_HEX_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BIG */
	*p_iter = iter;
	return tpp_formatprinter_print(config->tldsc_dataprinter,
	                               config->tldsc_arg, &word, 1);
}
#endif /* TPP_HAVE_STRING_ESCAPE_HEX_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BIG */


#if TPP_HAVE_STRING_ESCAPE_UNI_BRACE
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_ssize TPPCALL
tpp_token_decodestring_uni_sequence(tpp_lexer *self, tpp_char const **p_iter, tpp_char const *end,
                                    tpp_lexer_decodestring_config const *tpp_restrict config) {
	tpp_ssize result;
	tpp_char utf8_buf[TPP_UTF8_MAXLEN];
	tpp_size utf8_len;
	tpp_unichar uc;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_char const *iter = *p_iter;
	tpp_char ch = *iter;
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
	bool has_overflow = false;
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
	tpp_assert(tpp_ascii_isxdigit(ch));
	uc = 0;
	for (;;) {
		if (tpp_ascii_isdigit(ch)) {
			uc |= tpp_ascii_asdigit(ch);
		} else if (tpp_ascii_islwrxdigit(ch)) {
			uc |= tpp_ascii_aslwrxdigit(ch);
		} else {
			tpp_assert(tpp_ascii_isuprxdigit(ch));
			uc |= tpp_ascii_asuprxdigit(ch);
		}
		++iter;
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
		if (iter >= end)
			break;
		ch = *iter;
		if (!tpp_ascii_isxdigit(ch))
			break;
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
		if (((uc << 4) >> 4) != uc)
			has_overflow = true;
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
		uc <<= 4;
	}
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
	if (has_overflow) {
		tpp_errno error;
		tpp_char const *saved_start = token->tt_start;
		tpp_char const *saved_end = token->tt_end;
		token->tt_start = *p_iter;
		token->tt_end   = iter;
		error = tpp_lexer_warnf(self, TPP_W_CHARACTER_TOO_LARGE);
		token->tt_start = saved_start;
		token->tt_end   = saved_end;
		if (TPP_ISERR(error)) {
			*p_iter = iter;
			return TPP_SSIZE_OFERR(error);
		}
	}
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */

	/* Output utf-8 encoding of unicode character */
	utf8_len = (tpp_size)(tpp_unicode_writeutf8(utf8_buf, uc) - utf8_buf);
	result = tpp_formatprinter_print(tpp_lexer_decodestring_config_getutf8(config),
	                                 config->tldsc_arg, utf8_buf, utf8_len);
	*p_iter = iter;
	return result;
}
#endif /* TPP_HAVE_STRING_ESCAPE_UNI_BRACE */


#if TPP_HAVE_STRING_ESCAPE_XML
/* In addition to `tpp_xml_entity_lookup()`, must also support:
 * - &#<decimal>;
 * - &#x<hex>;
 * ... both of which allow encoding of unicode ordinals */
#if _TPP_HAVE_BSE_FILE_PARAM || TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS)
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_size TPPCALL
tpp_decode_named_escape_xml(tpp_char const **p_iter, tpp_char const *end,
                            tpp_unichar result[1],
                            tpp_lexer const *tpp_restrict lexer);
#else /* _TPP_HAVE_BSE_FILE_PARAM || TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS) */
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
_tpp_decode_named_escape_xml(tpp_char const **p_iter, tpp_char const *end,
                             tpp_unichar result[1]);
#define tpp_decode_named_escape_xml(p_iter, end, result, lexer) \
	_tpp_decode_named_escape_xml(p_iter, end, result)
#endif /* !_TPP_HAVE_BSE_FILE_PARAM && !TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS) */
#endif /* TPP_HAVE_STRING_ESCAPE_XML */


#if TPP_HAVE_TRIGRAPHS
#define tpp_token_decodestring_is_lbrace(self, ch, p_iter, end) \
	((ch) == '{' || ((ch) == '?' && (*(p_iter) + 2) < end &&    \
	                 (*(p_iter))[1] == '?' &&                   \
	                 (*(p_iter))[2] == '<' &&                   \
	                 tpp_lexer_has(self, TRIGRAPHS)             \
	                 ? (*(p_iter) += 2, true)                   \
	                 : false))
#define tpp_token_decodestring_is_rbrace(self, ch, p_iter, end) \
	((ch) == '}' || ((ch) == '?' && (*(p_iter) + 2) < end &&    \
	                 (*(p_iter))[1] == '?' &&                   \
	                 (*(p_iter))[2] == '>' &&                   \
	                 tpp_lexer_has(self, TRIGRAPHS)             \
	                 ? (*(p_iter) += 2, true)                   \
	                 : false))
#else /* TPP_HAVE_TRIGRAPHS */
#define tpp_token_decodestring_is_lbrace(self, ch, p_iter, end) ((ch) == '{')
#define tpp_token_decodestring_is_rbrace(self, ch, p_iter, end) ((ch) == '}')
#endif /* !TPP_HAVE_TRIGRAPHS */

#if (TPP_HAVE_IDENTIFIER_ESCAPE_NAMED || \
     TPP_HAVE_STRING_ESCAPE_NAMED ||     \
     TPP_HAVE_STRING_ESCAPE_UNI_BRACE || \
     TPP_HAVE_STRING_ESCAPE_OCT_BRACE || \
     TPP_HAVE_STRING_ESCAPE_HEX_BRACE)
#if _TPP_HAVE_BSE_FILE_PARAM || TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS)
TPP_INTERN_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_char const *TPPCALL
tpp_decode_find_unmatched_rbrace(tpp_char const **p_iter, tpp_char const *end, tpp_lexer const *lexer)
#else /* _TPP_HAVE_BSE_FILE_PARAM || TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS) */
TPP_INTERN_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_decode_find_unmatched_rbrace(tpp_char const **p_iter, tpp_char const *end)
#define tpp_decode_find_unmatched_rbrace(p_iter, end, lexer) \
	_tpp_decode_find_unmatched_rbrace(p_iter, end)
#endif /* !_TPP_HAVE_BSE_FILE_PARAM && !TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS) */
{
	tpp_char const *named_end;
	tpp_size recursion = 0;
	tpp_char const *iter = *p_iter;
	for (;;) {
		tpp_char ch;
		named_end = iter;
		if (iter >= end)
			break;
		ch = *iter++;
		iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
		if (ch == '{') {
			++recursion;
		} else if (ch == '}') {
			if (recursion == 0)
				break;
			--recursion;
		} else
#if TPP_HAVE_TRIGRAPHS
		if (ch == '?' && tpp_lexer_has(lexer, TRIGRAPHS) &&
			(iter + 1) < end && iter[0] == '?') {
			ch = iter[1];
			if (ch == '<') {
				iter += 2;
				++recursion;
			} else if (ch == '>') {
				iter += 2;
				if (recursion == 0)
					break;
				--recursion;
			}
		} else
#endif /* TPP_HAVE_TRIGRAPHS */
		{
		}
	}
	*p_iter = iter;
	return named_end;
}
#endif /* ... */

#if TPP_HAVE_STRING_ESCAPE_UNI_BRACE || TPP_HAVE_STRING_ESCAPE_OCT_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BRACE
/* Find the first unmatched '}'-character and update `*p_iter` to point *after* it.
 * If no such character can be found update `*p_iter` to point at `end`. In either
 * case, emit a warning `TPP_W_UNEXPECTED_CHARACTER_IN_STRING_ESCAPE` referencing
 * the skipped portion of input. */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_braceseq_find_rbrace_and_warn_bad_chars(tpp_char const **p_iter, tpp_char const *end,
                                                  tpp_lexer *tpp_restrict self) {
#if TPP_HAVE_TPP_W_UNEXPECTED_CHARACTER_IN_STRING_ESCAPE
	tpp_char const *unmatched_start = *p_iter;
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_CHARACTER_IN_STRING_ESCAPE */
	tpp_char const *unmatched_end;
	unmatched_end = tpp_decode_find_unmatched_rbrace(p_iter, end, self);
	(void)unmatched_end;
	(void)self;
#if TPP_HAVE_TPP_W_UNEXPECTED_CHARACTER_IN_STRING_ESCAPE
	{
		tpp_errno error;
		tpp_token *const token = tpp_lexer_gettoken(self);
		tpp_char const *saved_start = token->tt_start;
		tpp_char const *saved_end = token->tt_end;
		token->tt_start = unmatched_start;
		token->tt_end   = unmatched_end;
		error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_CHARACTER_IN_STRING_ESCAPE, "}");
		token->tt_start = saved_start;
		token->tt_end   = saved_end;
		return error;
	}
#else /* TPP_HAVE_TPP_W_UNEXPECTED_CHARACTER_IN_STRING_ESCAPE */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_CHARACTER_IN_STRING_ESCAPE */
}
#endif /* TPP_HAVE_STRING_ESCAPE_UNI_BRACE || TPP_HAVE_STRING_ESCAPE_OCT_BRACE || TPP_HAVE_STRING_ESCAPE_HEX_BRACE */


#if TPP_HAVE_STRING_ESCAPE_NAMED && TPP_HAVE_TPP_W_UNKNOWN_NAMED_ESCAPE_SEQUENCE
#ifndef tpp_lexer_warn_unknown_named_escape_sequence
#define tpp_lexer_warn_unknown_named_escape_sequence tpp_lexer_warn_unknown_named_escape_sequence
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_warn_unknown_named_escape_sequence(tpp_lexer *tpp_restrict self,
                                             tpp_char const *start,
                                             tpp_char const *end) {
	tpp_errno error;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_char const *const saved_start = token->tt_start;
	tpp_char const *const saved_end = token->tt_end;
	token->tt_start = start;
	token->tt_end   = end;
	error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_NAMED_ESCAPE_SEQUENCE);
	token->tt_start = saved_start;
	token->tt_end   = saved_end;
	return error;
}
#endif /* !tpp_lexer_warn_unknown_named_escape_sequence */
#endif /* TPP_HAVE_STRING_ESCAPE_NAMED && TPP_HAVE_TPP_W_UNKNOWN_NAMED_ESCAPE_SEQUENCE */


/* Decode string: "foobar fdasudfad"
 *                 ^start          ^end
 */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_ssize TPPCALL
tpp_token_decodestring_basic(tpp_lexer *self, tpp_char const *start, tpp_char const *end,
                             tpp_lexer_decodestring_config const *tpp_restrict config) {
	tpp_formatprinter const default_printer = tpp_lexer_decodestring_config_getdefl(config, self);
	void *const arg = config->tldsc_arg;
	tpp_char ch;
	tpp_ssize temp, result = 0;
	tpp_char const *iter = start;
	tpp_char const *esc_start;
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
		case '/': ch = '\\'; break;
		default: goto not_trigraph; /* Not actually a trigraph escape sequence */
		}
		--iter;

		/* Warn about trigraph, because `tpp_lexer_yieldraw()` hadn't done so already */
#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
		{
			tpp_file *const file = tpp_lexer_getfile(self);
			tpp_errno error = tpp_lexer_warnf_at(self, file, iter, TPP_W_ENCOUNTERED_TRIGRAPH);
			if (TPP_ISERR(error))
				return TPP_SSIZE_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */

		/* Print trigraph character (but also handle case where "??/" was encoded) */
		temp = tpp_formatprinter_print(default_printer, arg, start, (tpp_size)(iter - start));
		if (temp < 0)
			goto err_temp;
		result += temp;
		esc_start = iter;
		iter += 3;
		start = iter;
		if (ch != '\\') {
			temp = tpp_formatprinter_print(default_printer, arg, &ch, 1);
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
		temp = tpp_formatprinter_print(default_printer, arg, start, (tpp_size)((iter - 1) - start));
		if (temp < 0)
			goto err_temp;
		result += temp;
		esc_start = iter - 1;
	}

	/* Deal with \-escape sequence */
	if (iter >= end) {
		ch = '\0';
		goto handle_unknown_escape_sequence;
	}
	iter = tpp_preparse_skipbse_fwd(self, iter, end);
	ch = *iter++;
	(void)esc_start;
	switch (ch) {

	case '\\':
	case '\'':
	case '\"':
#if !TPP_HAVE_TRIGRAPHS
	case '?':
#else /* TPP_HAVE_TRIGRAPHS */
escape_self_sequence:
#endif /* !TPP_HAVE_TRIGRAPHS */
		/* Escape sequences that escape to themselves. */
		start = iter - 1;
		goto again;

#if TPP_HAVE_TRIGRAPHS
	case '?':
		if ((iter + 1) >= end)
			goto escape_self_sequence;
		if (iter[0] != '?')
			goto escape_self_sequence;

		/* Only '??/??/' is allowed (which is the same as \\; which is printed as \): */
		if (iter[1] != '/')
			goto escape_self_sequence;
		if (iter[-2] != '/') /* Preceding \-character must have also been a trigraph */
			goto escape_self_sequence;
		if (!tpp_lexer_has(self, TRIGRAPHS))
			goto escape_self_sequence;
		iter += 2;
		temp = tpp_formatprinter_print(default_printer, arg, (tpp_char const *)"\\", 1);
		if (temp < 0)
			goto err_temp;
		result += temp;
		start = iter;
		break;
#endif /* TPP_HAVE_TRIGRAPHS */

		/* Conventional escape sequences... */
	case 'a': ch = 0x07; goto print_ch_as_byte;
	case 'b': ch = 0x08; goto print_ch_as_byte;
	case 't': ch = 0x09; goto print_ch_as_byte;
	case 'n': ch = 0x0a; goto print_ch_as_byte;
	case 'v': ch = 0x0b; goto print_ch_as_byte;
	case 'f': ch = 0x0c; goto print_ch_as_byte;
	case 'r': ch = 0x0d; goto print_ch_as_byte;

#if TPP_HAVE_STRING_ESCAPE_E
	case 'e':
		if (!tpp_lexer_has(self, STRING_ESCAPE_E))
			goto handle_unknown_escape_sequence;
		ch = 0x1b;
		goto print_ch_as_byte;
#endif /* !TPP_HAVE_STRING_ESCAPE_E */

#if TPP_HAVE_STRING_ESCAPE_S
	case 's':
		if (!tpp_lexer_has(self, STRING_ESCAPE_S))
			goto handle_unknown_escape_sequence;
		ch = 0x20;
		goto print_ch_as_byte;
#endif /* !TPP_HAVE_STRING_ESCAPE_S */

print_ch_as_byte:
		temp = tpp_formatprinter_print(config->tldsc_dataprinter, arg, &ch, 1);
		if (temp < 0)
			goto err_temp;
		result += temp;
		break;

#if TPP_HAVE_STRING_ESCAPE_XML
	case '&': {
		tpp_size count;
		tpp_unichar uc;
		tpp_char utf8_buf[TPP_UTF8_MAXLEN];
		tpp_size utf8_len;
		if (!tpp_lexer_has(self, STRING_ESCAPE_XML))
			goto handle_unknown_escape_sequence;
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
#if _TPP_HAVE_BSE_FILE_PARAM || TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS)
		count = tpp_decode_named_escape_xml(&iter, end, &uc, self);
#else /* _TPP_HAVE_BSE_FILE_PARAM || TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS) */
		count = tpp_decode_named_escape_xml(&iter, end, &uc);
#endif /* !_TPP_HAVE_BSE_FILE_PARAM && !TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS) */
		if (count == 0)
			goto handle_unknown_escape_sequence;
		tpp_assert(count == 1);
		utf8_len = (tpp_size)(tpp_unicode_writeutf8(utf8_buf, uc) - utf8_buf);
		temp = tpp_formatprinter_print(tpp_lexer_decodestring_config_getutf8(config),
		                               config->tldsc_arg, utf8_buf, utf8_len);
		if (temp < 0)
			goto err_temp;
		result += temp;
	}	break;
#endif /* TPP_HAVE_STRING_ESCAPE_XML */

#if TPP_HAVE_STRING_ESCAPE_OCT
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7': {
		tpp_char word;
		if (!tpp_lexer_has(self, STRING_ESCAPE_OCT))
			goto handle_unknown_escape_sequence;

		/* Octal escape sequence */
		word = (tpp_char)tpp_ascii_asoctdigit(ch);
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
		if (iter < end && tpp_ascii_isoctdigit(*iter)) {
			ch = *iter++;
			word <<= 3;
			word |= (tpp_char)tpp_ascii_asoctdigit(ch);
			iter = tpp_preparse_skipbse_fwd(self, iter, end);
		}
		if (iter < end && tpp_ascii_isoctdigit(*iter) && (word <= 037)) {
			ch = *iter++;
			word <<= 3;
			word |= (tpp_char)tpp_ascii_asoctdigit(ch);
		}
		ch = word;
		goto print_ch_as_byte;
	}	break;
#endif /* TPP_HAVE_STRING_ESCAPE_OCT */

#if TPP_HAVE_STRING_ESCAPE_HEX || TPP_HAVE_STRING_ESCAPE_HEX_BRACE
	case 'x': {
		if (iter >= end)
			goto handle_unknown_escape_sequence;
		if (!tpp_lexer_has(self, STRING_ESCAPE_HEX) &&
		    !tpp_lexer_has(self, STRING_ESCAPE_HEX_BRACE))
			goto handle_unknown_escape_sequence;
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
		ch = *iter;
#if TPP_HAVE_STRING_ESCAPE_HEX
		if (tpp_ascii_isxdigit(ch)) {
			if (tpp_lexer_has(self, STRING_ESCAPE_HEX)) {
				temp = tpp_token_decodestring_hex_sequence(self, &iter, end, config);
				if (temp < 0)
					return temp;
				result += temp;
			}
		} else
#endif /* TPP_HAVE_STRING_ESCAPE_HEX */
#if TPP_HAVE_STRING_ESCAPE_HEX_BRACE
		if (tpp_lexer_has(self, STRING_ESCAPE_HEX_BRACE) &&
		    tpp_token_decodestring_is_lbrace(self, ch, &iter, end)) {
			iter = tpp_preparse_skipbse_fwd(self, iter + 1, end);
			if (iter >= end)
				goto handle_unknown_hex_brace_sequence;
#if TPP_HAVE_STRING_ESCAPE_HEX_BRACE_MANY
			if (tpp_lexer_has(self, STRING_ESCAPE_HEX_BRACE_MANY)) {
				for (;;) {
					if (iter >= end)
						goto handle_unknown_hex_brace_sequence;
					ch = *iter;
					if (ch == '}')
						break;
#ifdef TPP_HAVE_TRIGRAPHS
					if (ch == '?')
						break; /* In case of ??> */
#endif /* TPP_HAVE_TRIGRAPHS */
					if (tpp_ascii_isspace(ch) || ch == ',') {
						iter = tpp_preparse_skipbse_fwd(self, iter + 1, end);
					} else
#if TPP_HAVE_UNICODE
					if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(self))) {
						tpp_char const *prev_iter = iter;
						tpp_unichar uc = tpp_unicode_readutf8(&iter, end);
						if (!tpp_unicode_isspace(uc)) {
							iter = prev_iter;
							goto handle_unknown_hex_brace_sequence;
						}
					} else
#endif /* TPP_HAVE_UNICODE */
					if (tpp_ascii_isxdigit(ch)) {
						temp = tpp_token_decodestring_hex_sequence_ex(self, &iter, end, config, true);
						if (temp < 0)
							return temp;
						result += temp;
					} else {
						goto handle_unknown_hex_brace_sequence;
					}
				}
			} else
#endif /* TPP_HAVE_STRING_ESCAPE_HEX_BRACE_MANY */
			{
#if TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_HEX_BRACE_MANY)
				if (!tpp_ascii_isxdigit(*iter))
					goto handle_unknown_hex_brace_sequence;
				temp = tpp_token_decodestring_hex_sequence_ex(self, &iter, end, config, true);
				if (temp < 0)
					return temp;
				result += temp;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_HEX_BRACE_MANY) */
			}
			if (iter < end && (ch = *iter, tpp_token_decodestring_is_rbrace(self, ch, &iter, end))) {
				++iter;
			} else {
				tpp_errno error;
handle_unknown_hex_brace_sequence:
				error = tpp_lexer_braceseq_find_rbrace_and_warn_bad_chars(&iter, end, self);
				if (TPP_ISERR(error))
					return TPP_SSIZE_OFERR(error);
			}
		} else
#endif /* TPP_HAVE_STRING_ESCAPE_HEX_BRACE */
		{
			goto handle_unknown_escape_sequence;
		}
	}	break;
#endif /* TPP_HAVE_STRING_ESCAPE_HEX || TPP_HAVE_STRING_ESCAPE_HEX_BRACE */

#if TPP_HAVE_STRING_ESCAPE_OCT_BRACE
	case 'o': {
		/* Brace-delimited octal escape sequence */
		if (!tpp_lexer_has(self, STRING_ESCAPE_OCT_BRACE))
			goto handle_unknown_escape_sequence;
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
		if (iter >= end)
			goto handle_unknown_escape_sequence;
		ch = *iter;
		if (!tpp_token_decodestring_is_lbrace(self, ch, &iter, end))
			goto handle_unknown_escape_sequence;
		iter = tpp_preparse_skipbse_fwd(self, iter + 1, end);
		if (iter >= end)
			goto handle_unknown_oct_brace_sequence;
#if TPP_HAVE_STRING_ESCAPE_OCT_BRACE_MANY
		if (tpp_lexer_has(self, STRING_ESCAPE_OCT_BRACE_MANY)) {
			for (;;) {
				if (iter >= end)
					goto handle_unknown_oct_brace_sequence;
				ch = *iter;
				if (ch == '}')
					break;
#ifdef TPP_HAVE_TRIGRAPHS
				if (ch == '?')
					break; /* In case of ??> */
#endif /* TPP_HAVE_TRIGRAPHS */
				if (tpp_ascii_isspace(ch) || ch == ',') {
					iter = tpp_preparse_skipbse_fwd(self, iter + 1, end);
				} else
#if TPP_HAVE_UNICODE
				if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(self))) {
					tpp_char const *prev_iter = iter;
					tpp_unichar uc = tpp_unicode_readutf8(&iter, end);
					if (!tpp_unicode_isspace(uc)) {
						iter = prev_iter;
						goto handle_unknown_oct_brace_sequence;
					}
				} else
#endif /* TPP_HAVE_UNICODE */
				if (tpp_ascii_isoctdigit(ch)) {
					temp = tpp_token_decodestring_oct_sequence(self, &iter, end, config);
					if (temp < 0)
						return temp;
					result += temp;
				} else {
					goto handle_unknown_oct_brace_sequence;
				}
			}
		} else
#endif /* TPP_HAVE_STRING_ESCAPE_OCT_BRACE_MANY */
		{
#if TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_OCT_BRACE_MANY)
			if (!tpp_ascii_isoctdigit(*iter))
				goto handle_unknown_oct_brace_sequence;
			temp = tpp_token_decodestring_oct_sequence(self, &iter, end, config);
			if (temp < 0)
				return temp;
			result += temp;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_OCT_BRACE_MANY) */
		}
		if (iter < end && (ch = *iter, tpp_token_decodestring_is_rbrace(self, ch, &iter, end))) {
			++iter;
		} else {
			tpp_errno error;
handle_unknown_oct_brace_sequence:
			error = tpp_lexer_braceseq_find_rbrace_and_warn_bad_chars(&iter, end, self);
			if (TPP_ISERR(error))
				return TPP_SSIZE_OFERR(error);
		}
	}	break;
#endif /* TPP_HAVE_STRING_ESCAPE_OCT_BRACE */

#if TPP_HAVE_STRING_ESCAPE_UNI_BRACE
	case 'u': {
		if (tpp_lexer_has(self, STRING_ESCAPE_UNI_BRACE) &&
		    (iter = tpp_preparse_skipbse_fwd(self, iter, end), iter < end) &&
		    (ch = *iter, tpp_token_decodestring_is_lbrace(self, ch, &iter, end))) {
			iter = tpp_preparse_skipbse_fwd(self, iter + 1, end);
#if TPP_HAVE_STRING_ESCAPE_UNI_BRACE_MANY
			if (tpp_lexer_has(self, STRING_ESCAPE_UNI_BRACE_MANY)) {
				for (;;) {
					if (iter >= end)
						goto handle_unknown_uni_brace_sequence;
					ch = *iter;
					if (ch == '}')
						break;
#ifdef TPP_HAVE_TRIGRAPHS
					if (ch == '?')
						break; /* In case of ??> */
#endif /* TPP_HAVE_TRIGRAPHS */
					if (tpp_ascii_isspace(ch) || ch == ',') {
						iter = tpp_preparse_skipbse_fwd(self, iter + 1, end);
					} else
#if TPP_HAVE_UNICODE
					if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(self))) {
						tpp_char const *prev_iter = iter;
						tpp_unichar uc = tpp_unicode_readutf8(&iter, end);
						if (!tpp_unicode_isspace(uc)) {
							iter = prev_iter;
							goto handle_unknown_uni_brace_sequence;
						}
					} else
#endif /* TPP_HAVE_UNICODE */
					if (tpp_ascii_isxdigit(ch)) {
						temp = tpp_token_decodestring_uni_sequence(self, &iter, end, config);
						if (temp < 0)
							return temp;
						result += temp;
					} else {
						goto handle_unknown_uni_brace_sequence;
					}
				}
			} else
#endif /* TPP_HAVE_STRING_ESCAPE_UNI_BRACE_MANY */
			{
#if TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_UNI_BRACE_MANY)
				if (iter >= end)
					goto handle_unknown_uni_brace_sequence;
				if (!tpp_ascii_isxdigit(*iter))
					goto handle_unknown_uni_brace_sequence;
				temp = tpp_token_decodestring_uni_sequence(self, &iter, end, config);
				if (temp < 0)
					return temp;
				result += temp;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ESCAPE_UNI_BRACE_MANY) */
			}
			if (iter < end && (ch = *iter, tpp_token_decodestring_is_rbrace(self, ch, &iter, end))) {
				++iter;
			} else {
				tpp_errno error;
handle_unknown_uni_brace_sequence:
				error = tpp_lexer_braceseq_find_rbrace_and_warn_bad_chars(&iter, end, self);
				if (TPP_ISERR(error))
					return TPP_SSIZE_OFERR(error);
			}
			break;
		}
#if TPP_HAVE_STRING_ESCAPE_UNI
		goto handle_escape_uni;
#endif /* TPP_HAVE_STRING_ESCAPE_UNI */
	}	break;
#endif /* TPP_HAVE_STRING_ESCAPE_UNI_BRACE */

#if TPP_HAVE_STRING_ESCAPE_NAMED
	case 'N': {
		tpp_char const *named_start;
		tpp_char const *named_end;
		/* Brace-delimited named unicode sequence */
		if (!tpp_lexer_has(self, STRING_ESCAPE_OCT_BRACE))
			goto handle_unknown_escape_sequence;
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
		if (iter >= end)
			goto handle_unknown_escape_sequence;
		ch = *iter;
		if (!tpp_token_decodestring_is_lbrace(self, ch, &iter, end))
			goto handle_unknown_escape_sequence;
		iter = tpp_preparse_skipbse_fwd(self, iter + 1, end);
		named_start = iter;
		named_end   = tpp_decode_find_unmatched_rbrace(&iter, end, self);
		for (;;) {
			tpp_size i, count, utf8_len;
			tpp_unichar uc[TPP_DECODE_NAMED_ESCAPE_MAXLEN];
			tpp_char utf8_buf[TPP_DECODE_NAMED_ESCAPE_MAXLEN * TPP_UTF8_MAXLEN], *utf8_dst;
#if TPP_HAVE_STRING_ESCAPE_NAMED_MANY
			if (tpp_lexer_has(self, STRING_ESCAPE_NAMED_MANY))
				named_start = tpp_preparse_skipspace_fwd(self, named_start, named_end);
#endif /* TPP_HAVE_STRING_ESCAPE_NAMED_MANY */
			count = tpp_decode_named_escape(&named_start, named_end, uc, self);
			if (count == 0) {
#if TPP_HAVE_STRING_ESCAPE_NAMED_MANY
				if (tpp_lexer_has(self, STRING_ESCAPE_NAMED_MANY)) {
					/* See if we can seek ahead to a ','-character */
					tpp_size remaining = (tpp_size)(named_end - named_start);
					tpp_char const *comma = (tpp_char const *)tpp_memchr(named_start, ',', remaining);
					if (!comma)
						break;
#if TPP_HAVE_TPP_W_UNKNOWN_NAMED_ESCAPE_SEQUENCE
					{
#if TPP_HAVE_PREPARSE_SKIPSPACE_BCK
						tpp_char const *before_comma = tpp_preparse_skipspace_bck(self, named_start, comma);
						tpp_errno error = tpp_lexer_warn_unknown_named_escape_sequence(self, named_start, before_comma);
#else /* TPP_HAVE_PREPARSE_SKIPSPACE_BCK */
						tpp_errno error = tpp_lexer_warn_unknown_named_escape_sequence(self, named_start, comma);
#endif /* !TPP_HAVE_PREPARSE_SKIPSPACE_BCK */
						if (TPP_ISERR(error))
							return TPP_SSIZE_OFERR(error);
					}
#endif /* TPP_HAVE_TPP_W_UNKNOWN_NAMED_ESCAPE_SEQUENCE */
					named_start = comma + 1;
					continue;
				} else
#endif /* TPP_HAVE_STRING_ESCAPE_NAMED_MANY */
				{
					break;
				}
			}

			/* Encode as utf-8 */
			utf8_dst = utf8_buf;
			for (i = 0; i < count; ++i)
				utf8_dst = tpp_unicode_writeutf8(utf8_dst, uc[i]);

			/* Write data as utf-8 */
			utf8_len = (tpp_size)(utf8_dst - utf8_buf);
			temp = tpp_formatprinter_print(tpp_lexer_decodestring_config_getutf8(config),
			                               arg, utf8_buf, utf8_len);
			if (temp < 0)
				goto err_temp;
			result += temp;

			/* Check if there are additional character names */
#if TPP_HAVE_STRING_ESCAPE_NAMED_MANY
			if (tpp_lexer_has(self, STRING_ESCAPE_NAMED_MANY)) {
				named_start = tpp_preparse_skipspace_fwd(self, named_start, named_end);
				if (named_start < named_end && *named_start == ',') {
					named_start = tpp_preparse_skipspace_fwd(self, named_start + 1, named_end);
					continue;
				}
			}
#endif /* TPP_HAVE_STRING_ESCAPE_NAMED_MANY */
			break;
		}

		/* Warn if not everything was consumed */
#if TPP_HAVE_TPP_W_UNKNOWN_NAMED_ESCAPE_SEQUENCE
		if (named_start < named_end) {
			tpp_errno error = tpp_lexer_warn_unknown_named_escape_sequence(self, named_start, named_end);
			if (TPP_ISERR(error))
				return TPP_SSIZE_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_UNKNOWN_NAMED_ESCAPE_SEQUENCE */
	}	break;
#endif /* TPP_HAVE_STRING_ESCAPE_NAMED */

#if TPP_HAVE_STRING_ESCAPE_UNI
#if TPP_HAVE_STRING_ESCAPE_UNI_BRACE
handle_escape_uni:
#endif /* TPP_HAVE_STRING_ESCAPE_UNI_BRACE */
#if !TPP_HAVE_STRING_ESCAPE_UNI_BRACE
	case 'u':
#endif /* !TPP_HAVE_STRING_ESCAPE_UNI_BRACE */
	case 'U': {
		tpp_unichar uc = 0;
		unsigned int num_nibble = ch == 'u' ? 4 : 8;
		unsigned int cur_nibble = 0;
		tpp_char utf8_buf[TPP_UTF8_MAXLEN];
		tpp_size utf8_len;
		if (!tpp_lexer_has(self, STRING_ESCAPE_UNI))
			goto handle_unknown_escape_sequence;
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
		if (iter >= end)
			goto handle_unknown_escape_sequence;
		do {
			tpp_char nibble;
			ch = *iter++;
			if (tpp_ascii_isdigit(ch)) {
				nibble = (tpp_char)tpp_ascii_asdigit(ch);
			} else if (tpp_ascii_islwrxdigit(ch)) {
				nibble = (tpp_char)tpp_ascii_aslwrxdigit(ch);
				++iter;
			} else if (tpp_ascii_isuprxdigit(ch)) {
				nibble = (tpp_char)tpp_ascii_asuprxdigit(ch);
			} else {
				if (cur_nibble == 0)
					goto handle_unknown_escape_sequence;
				break;
			}
			uc <<= 4;
			uc |= nibble;
			iter = tpp_preparse_skipbse_fwd(self, iter, end);
			if (iter >= end)
				break;
		} while (++cur_nibble < num_nibble);

		/* Encode as utf-8 */
		utf8_len = (tpp_size)(tpp_unicode_writeutf8(utf8_buf, uc) - utf8_buf);
		temp = tpp_formatprinter_print(tpp_lexer_decodestring_config_getutf8(config),
		                               arg, utf8_buf, utf8_len);
		if (temp < 0)
			goto err_temp;
		result += temp;
	}	break;
#endif /* TPP_HAVE_STRING_ESCAPE_UNI */


	default: {
		tpp_char const *esc_first;
#if TPP_HAVE_BSE && TPP_HAVE_UNICODE
		tpp_char const *bse_iter;
		if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(self))) {
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
#if TPP_HAVE_BSE_WHITESPACE && TPP_HAVE_CR_LF_DETECTION
					if (uc == TPP_ASCII_CR && (iter < end) && *iter == TPP_ASCII_LF)
						++iter;
#endif /* TPP_HAVE_BSE_WHITESPACE && TPP_HAVE_CR_LF_DETECTION */
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
				if (tpp_ascii_ismb(wch) && tpp_file_isutf8(tpp_lexer_getfile(self))) {
					--bse_iter;
					goto again_read_unicode_whitespace_after_backslash;
				}
#endif /* TPP_HAVE_UNICODE */
				if (tpp_ascii_islf(wch)) {
					iter = bse_iter; /* Escaped linefeed. */
#if TPP_HAVE_CR_LF_DETECTION
					if (wch == TPP_ASCII_CR && (iter < end) && *iter == TPP_ASCII_LF)
						++iter;
#endif /* TPP_HAVE_CR_LF_DETECTION */
					break;
				}
			}
		} else
#endif /* TPP_HAVE_BSE_WHITESPACE */
#if TPP_HAVE_BSE
#if TPP_HAVE_ASSUME_ASCII_CTYPE
		if (0)
	case TPP_ASCII_CR:
	case TPP_ASCII_LF:
#else /* TPP_HAVE_ASSUME_ASCII_CTYPE */
		if (tpp_ascii_islf(ch))
#endif /* !TPP_HAVE_ASSUME_ASCII_CTYPE */
		{
			/* Escaped line-feed */
			if (!tpp_lexer_has(self, BSE))
				goto handle_unknown_escape_sequence;
#if TPP_HAVE_CR_LF_DETECTION
			if (ch == TPP_ASCII_CR && (iter < end) && *iter == TPP_ASCII_LF)
				++iter;
#endif /* TPP_HAVE_CR_LF_DETECTION */
			break;
		} else
#endif /* TPP_HAVE_BSE */
		{
		}

handle_unknown_escape_sequence:
		esc_first = esc_start + 1;
#if TPP_HAVE_TRIGRAPHS
		if (*esc_start == '?')
			esc_first += 2;
#endif /* TPP_HAVE_TRIGRAPHS */
		/* NOTE: At this point, "iter" represents the end of the unknown escape sequence! */
		tpp_assert(iter >= esc_first);
#if TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK
		/* Hook here to allow user-code to define custom string escape sequences */
		temp = tpp_lexer_callhook_unknown_string_escape(self, &esc_first, end, config);
		if (temp >= 0) {
			/* Successfully handled via hook. */
			result += temp;
			start = iter = esc_first;
			goto again;
		}
		if (temp != TPP_SSIZE_OFERR(TPP_ENOENT))
			goto err_temp; /* Error/abort from printer callback */
#endif /* TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK */
#if TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE
		{
			tpp_errno error;
			tpp_token *const token = tpp_lexer_gettoken(self);
			tpp_char const *saved_start = token->tt_start;
			tpp_char const *saved_end = token->tt_end;
			token->tt_start = esc_start;
			token->tt_end   = iter;
			error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE);
			token->tt_start = saved_start;
			token->tt_end   = saved_end;
			if (TPP_ISERR(error))
				return TPP_SSIZE_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE */
		/* Setup flushing such that the \-character is removed */
#if TPP_HAVE_TRIGRAPHS
		if (*esc_start == '?')
			esc_start += 2;
#endif /* TPP_HAVE_TRIGRAPHS */
		start = esc_first;
		goto again;
	}	break;

	}

	start = iter;
	goto again;
done:
	if (start < end) {
		temp = tpp_formatprinter_print(default_printer, arg, start, (tpp_size)(end - start));
		if (temp < 0)
			goto err_temp;
		result += temp;
	}
	return result;
err_temp:
	return temp;
}
#endif /* TPP_HAVE_STRING_ESCAPE */


#if TPP_HAVE_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TOK_BLOCK_CHAR_LITERAL
struct tpp_block_string_prefix {
	tpp_char const *tbsp_start;  /* [1..1] Prefix start */
	tpp_char const *tbsp_end;    /* [1..1] Prefix end */
};

#define tpp_block_string_prefix_isempty(self) \
	((self)->tbsp_start >= (self)->tbsp_end)


/* Turn `lhs` into the union of `lhs` and `rhs` */
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_block_string_prefix_union(struct tpp_block_string_prefix *tpp_restrict lhs,
                              struct tpp_block_string_prefix const *tpp_restrict rhs) {
	tpp_size i;
	tpp_size lhs_len = (tpp_size)(lhs->tbsp_end - lhs->tbsp_start);
	tpp_size rhs_len = (tpp_size)(rhs->tbsp_end - rhs->tbsp_start);
	tpp_size com_len = lhs_len < rhs_len ? lhs_len : rhs_len;
	for (i = 0; i < com_len; ++i) {
		tpp_char lhs_ch = lhs->tbsp_start[i];
		tpp_char rhs_ch = rhs->tbsp_start[i];
		if (lhs_ch != rhs_ch)
			break;
	}
	lhs->tbsp_end = lhs->tbsp_start + i;
}

static TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_char const *TPPCALL
tpp_block_string_loadprefix(tpp_lexer *tpp_restrict lexer,
                            struct tpp_block_string_prefix *tpp_restrict self,
                            tpp_char const *start, tpp_char const *end) {
	tpp_char const *prefix_start = start;
	tpp_char const *prefix_end = prefix_start;
	tpp_assert(start <= end);
	(void)lexer;
	while (prefix_end < end) {
		tpp_char ch = *prefix_end++;
		if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_CR_LF_DETECTION
			if (ch == TPP_ASCII_CR && (prefix_end < end && *prefix_end == TPP_ASCII_LF))
				++prefix_end;
#endif /* TPP_HAVE_CR_LF_DETECTION */
			/* Whitespace in empty lines doesn't count */
			prefix_start = prefix_end;
			continue;
		} else if (tpp_ascii_isspace(ch)) {
			continue;
		} else
#if TPP_HAVE_UNICODE
		if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(lexer))) {
			tpp_char const *next_prefix_end = prefix_end - 1;
			tpp_unichar uc = tpp_unicode_readutf8(&next_prefix_end, end);
			if (tpp_unicode_islf(uc)) {
				/* Whitespace in empty lines doesn't count */
				prefix_start = prefix_end = next_prefix_end;
				continue;
			} else if (tpp_unicode_isspace(uc)) {
				prefix_end = next_prefix_end;
				continue;
			}
		} else
#endif /* TPP_HAVE_UNICODE */
		{
			--prefix_end; /* First non-whitespace character found */
		}
		break;
	}
	self->tbsp_start = prefix_start;
	self->tbsp_end   = prefix_end;
	return prefix_end;
}

/* Returns a pointer *after* the next linefeed character(-sequence), or at "end" */
static TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_char const *TPPCALL
tpp_block_string_seeklf(tpp_lexer *tpp_restrict lexer,
                        tpp_char const *start, tpp_char const *end,
                        tpp_char const **p_eol_start) {
	while (start < end) {
		tpp_char ch = *start++;
		if (tpp_ascii_islf(ch)) {
			if (p_eol_start)
				*p_eol_start = start - 1;
#if TPP_HAVE_CR_LF_DETECTION
			if (ch == TPP_ASCII_CR && (start < end && *start == TPP_ASCII_LF))
				++start;
#endif /* TPP_HAVE_CR_LF_DETECTION */
			return start;
		} else
#if TPP_HAVE_UNICODE
		if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(lexer))) {
			tpp_char const *after_unicode = start - 1;
			tpp_unichar uc = tpp_unicode_readutf8(&after_unicode, end);
			if (tpp_unicode_islf(uc)) {
				if (p_eol_start)
					*p_eol_start = start - 1;
				return after_unicode;
			}
		} else
#endif /* TPP_HAVE_UNICODE */
		{
		}
	}
	if (p_eol_start)
		*p_eol_start = start;
	return start;
}

/* Decode string:
 * |"""
 * |.   foobar fdasudfad
 * |    fasdf\
 * |"""      ^end
 *  ^ start@.
 */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_ssize TPPCALL
tpp_token_decodestring_block(tpp_lexer *self, tpp_char const *start, tpp_char const *end,
                             tpp_lexer_decodestring_config const *tpp_restrict config) {
	tpp_ssize temp, result = 0;
	struct tpp_block_string_prefix common_prefix;
	tpp_size common_prefix_len;
	tpp_char const *iter = start;
	tpp_assert(iter <= end);
	iter = tpp_block_string_loadprefix(self, &common_prefix, iter, end);
	if (tpp_block_string_prefix_isempty(&common_prefix))
		goto handle_empty_prefix;
	while (iter < end) {
		struct tpp_block_string_prefix next_prefix;
		iter = tpp_block_string_seeklf(self, iter, end, NULL);
		iter = tpp_block_string_loadprefix(self, &next_prefix, iter, end);
		if (iter >= end)
			break; /* Don't include prefix on last line! */
		tpp_block_string_prefix_union(&common_prefix, &next_prefix);
		if (tpp_block_string_prefix_isempty(&common_prefix))
			goto handle_empty_prefix;
	}

	/* Common line prefix has been determined at this point
	 * -> now to actually print the string. */
	iter = start;
	common_prefix_len = (tpp_size)(common_prefix.tbsp_end - common_prefix.tbsp_start);
	while (iter < end) {
		tpp_size line_length_without_eol, line_length_with_eol;
		tpp_char const *eol_start, *eol_end;
		eol_end = tpp_block_string_seeklf(self, iter, end, &eol_start);
		line_length_without_eol = (tpp_size)(eol_start - iter);
		line_length_with_eol    = (tpp_size)(eol_end - iter);
		if (common_prefix_len < line_length_without_eol) {
			temp = tpp_token_decodestring_basic(self,
			                                    iter + common_prefix_len,
			                                    iter + line_length_with_eol,
			                                    config);
		} else {
			/* Special case: blank line (without common prefix) -> only print EOL */
			tpp_size eol_size = (tpp_size)(eol_end - eol_start);
			temp = tpp_formatprinter_print(tpp_lexer_decodestring_config_getdefl(config, self),
			                               config->tldsc_arg, eol_start, eol_size);
		}
		if (temp < 0)
			return temp;
		result += temp;
		iter = eol_end;
	}
	return result;
handle_empty_prefix:
	return tpp_token_decodestring_basic(self, start, end, config);
}
#endif /* TPP_HAVE_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TOK_BLOCK_CHAR_LITERAL */

#if TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TOK_RAW_STRING_LITERAL || TPP_HAVE_TOK_RAW_CHAR_LITERAL
#if TPP_CONF_MAYBE_0(TPP_HAVE_RAW_STRING_BSE)
#define tpp_token_decodestring_raw_nobse(self, start, end, config)               \
	tpp_formatprinter_print(tpp_lexer_decodestring_config_getdefl(config, self), \
	                        (config)->tldsc_arg, start, (tpp_size)((end) - (start)))
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_RAW_STRING_BSE) */

#if TPP_CONF_MAYBE_1(TPP_HAVE_RAW_STRING_BSE)
/* Behavior here is be controllable via a config. However, default
 * is disabled (no handling of BSE), since that's how GCC behaves:
 * >> static char const foo[] =
 * >> R"AB(a\
 * >> b)AB";
 * >> typedef int x[sizeof(foo) == 5 ? 1 : -1]; // 5: {'a','\\','\n','b',0}, as opposed to 3: {'a','b',0}
 */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_ssize TPPCALL
tpp_token_decodestring_raw_bse(tpp_lexer *self, tpp_char const *start, tpp_char const *end,
                               tpp_lexer_decodestring_config const *tpp_restrict config) {
	tpp_ssize temp, result = 0;
	tpp_char const *iter;
	/* Skip Any remaining BSE sequences at the head/tail */
	start = tpp_preparse_skipbse_fwd(self, start, end);
	end   = tpp_preparse_skipbse_bck(self, start, end);
	tpp_assert(start <= end);
	iter = start;
	while (iter < end) {
		tpp_char const *next_iter;
		++iter;
		next_iter = tpp_preparse_skipbse_fwd(self, iter, end);
		if (next_iter != iter) {
			temp = tpp_formatprinter_print(tpp_lexer_decodestring_config_getdefl(config, self),
			                               config->tldsc_arg, start, (tpp_size)(iter - start));
			if (temp < 0)
				return temp;
			result += temp;
			start = iter = next_iter;
		}
	}

	/* Flush remainder */
	temp = tpp_formatprinter_print(tpp_lexer_decodestring_config_getdefl(config, self),
	                               config->tldsc_arg, start, (tpp_size)(end - start));
	if (temp < 0)
		return temp;
	result += temp;
	return result;
}
#endif /* TPP_CONF_MAYBE_1(TPP_HAVE_RAW_STRING_BSE) */

/* Decode string: R"FOO(bla bla bla)FOO"
*                       ^start     ^end */
#if TPP_CONF_IS_RT(TPP_HAVE_RAW_STRING_BSE)
#define tpp_token_decodestring_raw(self, start, end, config)    \
	(tpp_lexer_has(self, RAW_STRING_BSE)                        \
	 ? tpp_token_decodestring_raw_bse(self, start, end, config) \
	 : tpp_token_decodestring_raw_nobse(self, start, end, config))
#elif TPP_HAVE_RAW_STRING_BSE
#define tpp_token_decodestring_raw(self, start, end, config) \
	tpp_token_decodestring_raw_bse(self, start, end, config)
#else /* ... */
#define tpp_token_decodestring_raw(self, start, end, config) \
	tpp_token_decodestring_raw_nobse(self, start, end, config)
#endif /* !... */
#endif /* ... */


#if TPP_HAVE_BSE
#define tpp_bse_seek_until_fwd(ptr, ch) while (*(ptr) != (ch)) ++ptr
#define tpp_bse_seek_until_bck(ptr, ch) while ((ptr)[-1] != (ch)) --ptr
#else /* TPP_HAVE_BSE */
#define tpp_bse_seek_until_fwd(ptr, ch) (void)0
#define tpp_bse_seek_until_bck(ptr, ch) (void)0
#endif /* !TPP_HAVE_BSE */


/* Print the unescaped representation of the string-token described by `self`
 * The caller must ensure that `TPP_TOK_ISSTRING(tpp_lexer_gettoken(self)->tt_id)`
 *
 * @param: config: Printer configuration
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR):  Either one of the printers returned this value, or
 *                                           a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):     Out of memory  (can only happen inside of `tpp_lexer_warnf()`)
 * @return: TPP_SSIZE_OFERR(TPP_EWARNPRINT): Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_decodestring(tpp_lexer *tpp_restrict self,
                       tpp_lexer_decodestring_config const *tpp_restrict config) {
#undef HAVE_do_decode_basic
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_char const *start = token->tt_start;
	tpp_char const *end   = token->tt_end;
	tpp_assert(TPP_TOK_ISSTRING(token->tt_id));
	switch (token->tt_id) {

#if (TPP_HAVE_TOK_C_STRING ||                 \
     TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TOK_C_CHAR ||                   \
     TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL ||   \
     TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL)
#if TPP_HAVE_BSE
#if (TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL ||   \
     TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL)
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
#if ((TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL ||  \
      TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL || \
      TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL || \
      TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL) && \
     (TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL ||    \
      TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL ||   \
      TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL ||   \
      TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL))
		quote_char = TPP_TOK_ISSTRING_SQUOTE(token->tt_id) ? '\'' : '"';
#elif (TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL ||  \
       TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL || \
       TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL || \
       TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL)
		quote_char = '"';
#else /* ... */
		quote_char = '\'';
#endif /* !... */
		tpp_bse_seek_until_fwd(start, quote_char);
	}	TPP_FALLTHRU
#endif /* ... */
#else /* TPP_HAVE_BSE */
#if (TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL)
#if TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	_TPP_CASE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
		++start;
		TPP_FALLTHRU
#endif /* TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL */
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
	_TPP_CASE_TPP_TOK_C_CHAR
	_TPP_CASE_TPP_TOK_C_STRING {
		++start; /* Skip leading quote */
		if (start < end)
			--end; /* Skip trailing quote */
		start = tpp_preparse_skipbse_fwd(self, start, end);
		end   = tpp_preparse_skipbse_bck(self, start, end);
		tpp_assert(start <= end);
#if TPP_HAVE_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TOK_BLOCK_CHAR_LITERAL
do_decode_basic:
#define HAVE_do_decode_basic
#endif /* TPP_HAVE_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TOK_BLOCK_CHAR_LITERAL */
		return tpp_token_decodestring_basic(self, start, end, config);
	}	break;
#endif /* ... */

#if TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL
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
#if TPP_HAVE_TOK_RAW_STRING_LITERAL || TPP_HAVE_TOK_RAW_CHAR_LITERAL
		goto cxx_raw_string_common;
#else /* TPP_HAVE_TOK_RAW_STRING_LITERAL || TPP_HAVE_TOK_RAW_CHAR_LITERAL */
		++start; /* Skip over leading '(' */
		if (start < end)
			--end; /* Skip over trailing ')' */

		/* Print string */
		return tpp_token_decodestring_raw(self, start, end, config);
#endif /* !TPP_HAVE_TOK_RAW_STRING_LITERAL && !TPP_HAVE_TOK_RAW_CHAR_LITERAL */
	}	break;
#endif /* TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL || TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL */

#if TPP_HAVE_TOK_RAW_STRING_LITERAL || TPP_HAVE_TOK_RAW_CHAR_LITERAL
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
#if TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL
cxx_raw_string_common:
#endif /* TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL */
		if (start < end)
			++start; /* Skip leading " / ' / ( */
		if (start < end)
			--end; /* Skip trailing " / ' / ) */

		/* Print string */
		return tpp_token_decodestring_raw(self, start, end, config);
	}	break;
#endif /* TPP_HAVE_TOK_RAW_STRING_LITERAL || TPP_HAVE_TOK_RAW_CHAR_LITERAL */

#if TPP_HAVE_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TOK_BLOCK_CHAR_LITERAL
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
		start = tpp_preparse_skipbse_fwd(self, start, end);
		end   = tpp_preparse_skipbse_bck(self, start, end);
		tpp_assert(start <= end);

		/* Check if block-string starts with a line-feed character.
		 * If it doesn't, must decode as a "basic" string (without
		 * the special indentation-removal behavior) */
		if (start >= end)
			goto do_decode_basic;
		lf_ch = *start;
		if (tpp_ascii_islf(lf_ch)) {
			++start;
#if TPP_HAVE_CR_LF_DETECTION
			if (lf_ch == TPP_ASCII_CR && start < end && *start == TPP_ASCII_LF)
				++start; /* Treat \r\n as a single line-feed */
#endif /* TPP_HAVE_CR_LF_DETECTION */
		} else
#if TPP_HAVE_UNICODE
		/* Support for unicode line-feeds... */
		if (tpp_ascii_ismb(lf_ch) && tpp_file_isutf8(tpp_lexer_getfile(self))) {
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
			return tpp_token_decodestring_basic(self, start, end, config);
#endif /* !HAVE_do_decode_basic */
		}
		return tpp_token_decodestring_block(self, start, end, config);
	}	break;
#endif /* TPP_HAVE_TOK_BLOCK_STRING_LITERAL || TPP_HAVE_TOK_BLOCK_CHAR_LITERAL */

	default: tpp_unreachable();
	}
	tpp_unreachable();
#undef HAVE_do_decode_basic
}


/* Same as `tpp_lexer_decodestring()`, but also `tpp_lexer_yield()` to the next token.
 * Then, if that token is also string-like (`TPP_TOK_ISSTRING()`), decode it also,
 * then yield again, and so on, until a non-string-like token is encountered, an
 * error happens, or one of the printers returned a negative value.
 *
 * HINT: This function automatically handles `TPP_EWOULDBLOCK` during
 *       yield by trying again with TPP_FILE_FLAGS_NONBLOCK disabled.
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR):  Either one of the printers returned this value, or
 *                                           a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):     Out of memory
 * @return: TPP_SSIZE_OFERR(TPP_EIO):        I/O error while yielding to next token
 * @return: TPP_SSIZE_OFERR(TPP_EWARNPRINT): Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_parsestring_ex(tpp_lexer *tpp_restrict self,
                         tpp_lexer_decodestring_config const *tpp_restrict config,
                         unsigned int flags) {
	(void)flags;
#if TPP_CONF_MAYBE_0(TPP_HAVE_STRING_AUTO_CONCAT)
#if TPP_HAVE_STRING_AUTO_CONCAT
	if (!tpp_lexer_has(self, STRING_AUTO_CONCAT))
#endif /* TPP_HAVE_STRING_AUTO_CONCAT */
	{
		tpp_ssize result = tpp_lexer_decodestring(self, config);
		if (result >= 0) {
			tpp_token_id tok = tpp_lexer_yield_blocking(self);
			if (TPP_TOK_ISERR(tok))
				result = TPP_SSIZE_OFERR(TPP_TOK_ASERR(tok));
		}
		return result;
	}
#if TPP_HAVE_STRING_AUTO_CONCAT
	else
#endif /* TPP_HAVE_STRING_AUTO_CONCAT */
#endif /* TPP_HAVE_STRING_AUTO_CONCAT */
#if TPP_HAVE_STRING_AUTO_CONCAT
	{
		tpp_token_id tok;
		tpp_ssize result = 0, temp;
again:
		temp = tpp_lexer_decodestring(self, config);
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
			if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONLF))
				goto again_yield;
			break;

		default:
			if (TPP_TOK_ISERR(tok))
				result = TPP_SSIZE_OFERR(TPP_TOK_ASERR(tok));
			break;
		}
		return result;
	}
#endif /* TPP_HAVE_STRING_AUTO_CONCAT */
}


/* Convenience wrapper around `tpp_lexer_parsestring_ex()`
 * On success (`!TPP_ISERR(return)`), caller must `tpp_string_decref(*p_result)`
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring(tpp_lexer *tpp_restrict self,
                      /*out*/ TPP_REF tpp_string **tpp_restrict p_result,
                      unsigned int flags) {
	tpp_ssize status;
	tpp_string_builder builder;
	tpp_lexer_decodestring_config config;
	tpp_string_builder_init(&builder);
	tpp_lexer_decodestring_config_init_simple(&config, &tpp_string_builder_print, &builder);
	status = tpp_lexer_parsestring_ex(self, &config, flags);
	if (TPP_SSIZE_ISERR(status))
		goto err_builder;
	*p_result = tpp_string_builder_pack(&builder);
	return TPP_EOK;
err_builder:
	tpp_string_builder_fini(&builder);
	return TPP_SSIZE_ASERR(status);
}


#define TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_EMPTY 0 /* String has 0 (non-empty) chunks */
#define TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_YES   1 /* String has 1 (non-empty) chunk */
#define TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_NO    2 /* String has 2 or more (non-empty) chunks */

#define TPP_LEXER_PARSESTRING_CHUNK_STOP (TPP_SSIZE_OFERR(TPP_ELAST - 1))

struct tpp_lexer_decodestring_chunk_count_data {
	tpp_lexer const *tldsccd_lexer; /* [1..1] Current lexer (needed to see if text bounds lie within current token) */
	unsigned int     tldsccd_count; /* # of chuniks (one of `TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_*`) */
#if TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS
	unsigned int     tldsccd_flags; /* Set of `TPP_LEXER_PARSESTRING_FLAG_*` */
#endif /* TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS */
};

static TPP_FORMATPRINTER_DEFINE(tpp_lexer_decodestring_chunk_count, arg, text, num_bytes) {
	struct tpp_lexer_decodestring_chunk_count_data *data;
	data = (struct tpp_lexer_decodestring_chunk_count_data *)arg;
	(void)text;
	if (num_bytes != 0) {
		/* Update counter:
		 * - TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_EMPTY  ->  TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_YES
		 * - TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_YES    ->  TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_NO
		 *   TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_NO     ->  return TPP_LEXER_PARSESTRING_CHUNK_STOP */
		++data->tldsccd_count;
		if (data->tldsccd_count >= TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_NO)
			return TPP_LEXER_PARSESTRING_CHUNK_STOP;

		/* Check if the given [text,+=num_bytes...) range is a sub-range of the current token.
		 * It may not be in case of something like:
		 * >> "\xFF"
		 *
		 * When decoding this string, it *will* yield a single-chunk string, however that chunk's
		 * data will lie outside the currently loaded file (it lies on our thread's stack), meaning
		 * our caller can't use direct-data-propagation (doing so would work if the string is only
		 * used inside the callback given to "tpp_lexer_parsestring_cb", but not if that callback
		 * wants to persist the string (which it's allowed to by tpp_string_incref'ing the given
		 * chunk-pointer when it's non-null), as that string may be stored on the caller's stack)
		 */
#if TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS
		/* The "TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS" flag prevents the special-case check,
		 * meaning that we're able to indicate a single-chunk string, even in case of a stack-
		 * allocated string buffer. */
		if (!(data->tldsccd_flags & TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS))
#endif /* TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS */
		{
			tpp_token const *token = tpp_lexer_gettoken(data->tldsccd_lexer);
			if ((text) < tpp_token_getstart(token) ||
			    (text + num_bytes) > tpp_token_getend(token)) {
				data->tldsccd_count = TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_NO;
				return TPP_LEXER_PARSESTRING_CHUNK_STOP;
			}
		}
	}
	return 0;

}

#if TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS
#define tpp_lexer_decodestring_single_chunk_flags__param , unsigned int flags
#define tpp_lexer_decodestring_single_chunk_flags__arg   , flags
#else /* TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS */
#define tpp_lexer_decodestring_single_chunk_flags__param /* nothing */
#define tpp_lexer_decodestring_single_chunk_flags__arg   /* nothing */
#endif /* !TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS */

/* Check if the currently loaded string-token can be printed in 0/1 chunks
 * @return: * : One of `TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_*` */
static TPP_NONNULL((1)) unsigned int TPPCALL
tpp_lexer_decodestring_is_single_chunk(tpp_lexer *tpp_restrict self
                                       tpp_lexer_decodestring_single_chunk_flags__param) {
	struct tpp_lexer_decodestring_chunk_count_data data;
	tpp_ssize decode_status;
	tpp_lexer_decodestring_config config;
#if TPP_HAVE_WARNINGS
	tpp_lexer_state_flags old_state;
	old_state = self->tl_state;
	self->tl_state |= TPP_LEXER_STATE_FLAG_NOWARNINGS;
#endif /* TPP_HAVE_WARNINGS */

	data.tldsccd_lexer = self;
	data.tldsccd_count = TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_EMPTY;
#if TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS
	data.tldsccd_flags = flags;
#endif /* TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS */
	/* Try to decode the string and count how many chunks we encounter */
	tpp_lexer_decodestring_config_init_simple(&config, &tpp_lexer_decodestring_chunk_count, &data);
	decode_status = tpp_lexer_decodestring(self, &config);
	tpp_assert(decode_status == 0 ||
	           decode_status == TPP_LEXER_PARSESTRING_CHUNK_STOP);
	(void)decode_status;

#if TPP_HAVE_WARNINGS
	self->tl_state = old_state;
#endif /* TPP_HAVE_WARNINGS */
	return data.tldsccd_count;
}

#if TPP_HAVE_STRING_AUTO_CONCAT
static TPP_NONNULL((1)) unsigned int TPPCALL
tpp_lexer_decodestring_is_single_chunk_at(tpp_lexer *tpp_restrict self,
                                          tpp_char const *token_end
                                          tpp_lexer_decodestring_single_chunk_flags__param) {
	unsigned int result;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_char const *saved_token_end = token->tt_end;
	token->tt_end = token_end;
	result = tpp_lexer_decodestring_is_single_chunk(self tpp_lexer_decodestring_single_chunk_flags__arg);
	token->tt_end = saved_token_end;
	return result;
}
#endif /* TPP_HAVE_STRING_AUTO_CONCAT */

struct tpp_lexer_decodestring_as_single_chunk_data {
	tpp_errno (TPPCALL *tldsascd_cb)(void *arg, tpp_string *chunk,
	                                 tpp_char const *str, tpp_size length);
	void               *tldsascd_arg;
	tpp_string         *tldsascd_chunk;
};

static TPP_FORMATPRINTER_DEFINE(tpp_lexer_decodestring_as_single_chunk_cb, arg, text, num_bytes) {
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
	return TPP_SSIZE_OFERR(error);
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_decodestring_as_single_chunk(tpp_lexer *self,
                                       tpp_errno (TPPCALL *cb)(void *arg, tpp_string *chunk,
                                                               tpp_char const *str, tpp_size length),
                                       void *arg) {
	tpp_ssize status;
	tpp_lexer_decodestring_config config;
	struct tpp_lexer_decodestring_as_single_chunk_data data;
	tpp_assert(cb && "NULL-callback given");
	data.tldsascd_cb    = cb;
	data.tldsascd_arg   = arg;
	data.tldsascd_chunk = tpp_lexer_getfile(self)->tf_chunk;
	tpp_lexer_decodestring_config_init_simple(&config, &tpp_lexer_decodestring_as_single_chunk_cb, &data);
	status = tpp_lexer_decodestring(self, &config);
#ifndef __OPTIMIZE_SIZE__
	if (status == TPP_LEXER_PARSESTRING_CHUNK_STOP)
		status = TPP_SSIZE_OFERR(TPP_EOK);
#endif /* !__OPTIMIZE_SIZE__ */
	tpp_assert(TPP_SSIZE_ISERR_OR_EOK(status));
	return TPP_SSIZE_ASERR_OR_EOK(status);
}

/* Wrapper around `tpp_lexer_parsestring()` that passes the actual string data
 * to a given callback. This function also enables some (optional) optimizations
 * for the most common case where the string token in `self` isn't followed by
 * another string token, and can be printed as a singular chunk. When this is
 * the case, no intermediate heap-buffer needs to be created, as `cb` can just
 * be invoked using the currently loaded file's content-buffer.
 *
 * @param: cb.arg:   Cookie argument (s.a. `arg`)
 * @param: cb.chunk: The string-chunk containing `str` (or `NULL` if `str` is statically allocated)
 *                   NOTE: May be non-NULL, even if `str` is statically allocated!
 * @param: flags:    Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning
 * @return: * :             Return value of given `cb` */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring_cb(tpp_lexer *self,
                         tpp_errno (TPPCALL *cb)(void *arg, tpp_string *chunk,
                                                 tpp_char const *str, tpp_size length),
                         void *arg, unsigned int flags) {
	unsigned int how;
	/* Lexer feature to disable adjacent-string-token auto-concat
	 * (C might have it, but not all languages do; so there should
	 * be a way to disable it) */
#if TPP_CONF_MAYBE_0(TPP_HAVE_STRING_AUTO_CONCAT)
	if (!tpp_lexer_has(self, STRING_AUTO_CONCAT)) {
		tpp_errno result;
		(void)flags;
		how = tpp_lexer_decodestring_is_single_chunk(self tpp_lexer_decodestring_single_chunk_flags__arg);
		if (how == TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_EMPTY) {
			/* Indicate an empty chunk to the caller */
			result = (*cb)(arg, tpp_lexer_getfile(self)->tf_chunk,
			               tpp_lexer_getfile(self)->tf_pos, 0);
		} else if (how == TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_YES) {
			result = tpp_lexer_decodestring_as_single_chunk(self, cb, arg);
		} else {
			goto do_multi_chunk_string;
		}
		if (!TPP_TOK_ISERR(result)) {
			tpp_token_id tok = tpp_lexer_yield_blocking(self);
			if (TPP_TOK_ISERR(tok))
				result = TPP_TOK_ASERR(tok);
		}
		return result;
	}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_STRING_AUTO_CONCAT) */
#if TPP_HAVE_STRING_AUTO_CONCAT
again:
	how = tpp_lexer_decodestring_is_single_chunk(self tpp_lexer_decodestring_single_chunk_flags__arg);
	if (how == TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_EMPTY) {
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
			if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONLF))
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
	} else if (how == TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_YES) {
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
			how = tpp_lexer_decodestring_is_single_chunk_at(self, pos tpp_lexer_decodestring_single_chunk_flags__arg);
			if (how == TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_EMPTY)
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
			if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONLF))
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
				how = tpp_lexer_decodestring_is_single_chunk_at(self, pos tpp_lexer_decodestring_single_chunk_flags__arg);
				if (how == TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_EMPTY)
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
				if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONLF))
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
					tpp_assert(tpp_lexer_decodestring_is_single_chunk(self tpp_lexer_decodestring_single_chunk_flags__arg) ==
					           TPP_LEXER_DECODESTRING_IS_SINGLE_CHUNK_EMPTY);
					goto again_yield_after_eof_decoded;
				case TPP_TOK_SPACE:
				TPP_CASE_TPP_TOK_COMMENT_NOLINE
					if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE))
						goto again_yield_after_eof_decoded;
					break;
				case TPP_TOK_LF:
				TPP_CASE_TPP_TOK_COMMENT_LINE
					if (!(flags & TPP_LEXER_PARSESTRING_FLAG_STOPONLF))
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
	} else
#endif /* TPP_HAVE_STRING_AUTO_CONCAT */
	{
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

#undef tpp_lexer_decodestring_single_chunk_flags__param
#undef tpp_lexer_decodestring_single_chunk_flags__arg
#endif /* TPP_HAVE_LEXER_DECODESTRING */

#if TPP_HAVE_LEXER_PARSESTRING_EXPR
#ifndef tpp_lexer_parsestring_expr
/* Convenience wrapper around `tpp_lexer_parsestring()`
 * On success (`!TPP_ISERR(return)`), caller must `tpp_expr_value_fini(result)`
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()`).
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
#endif /* !tpp_lexer_parsestring_expr */
#endif /* TPP_HAVE_LEXER_PARSESTRING_EXPR */


#if TPP_HAVE_LEXER_PARSECHARACTER_LITERAL
struct tpp_lexer_decodecharacter_data {
#if TPP_HAVE_TPP_W_MULTICHAR_LITERAL || TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
	tpp_lexer  *tldcd_lexer; /* [1..1] Active lexer */
#endif /* TPP_HAVE_TPP_W_MULTICHAR_LITERAL || TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
#if TPP_HAVE_TPP_W_MULTICHAR_LITERAL
	tpp_size    tldcd_count; /* # of words already parsed */
#endif /* TPP_HAVE_TPP_W_MULTICHAR_LITERAL */
	tpp_uintmax tldcd_value; /* Multichar value */
};

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_decodecharacter_data_addword(struct tpp_lexer_decodecharacter_data *tpp_restrict self,
                                       tpp_uintmax word) {
#if TPP_HAVE_TPP_W_MULTICHAR_LITERAL
	if ((self->tldcd_count <= 1) &&
	    (self->tldcd_count + 1) > 1) {
		/* Emit warning about multi-char literals being used */
		tpp_errno error = tpp_lexer_warnf(self->tldcd_lexer, TPP_W_MULTICHAR_LITERAL);
		if (TPP_ISERR(error))
			return error;
	}
	self->tldcd_count += 1;
#endif /* TPP_HAVE_TPP_W_MULTICHAR_LITERAL */
#if TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
	if (((self->tldcd_value << TPP_CHAR_BIT) >> TPP_CHAR_BIT) != self->tldcd_value) {
		tpp_errno error = tpp_lexer_warnf(self->tldcd_lexer, TPP_W_CHARACTER_TOO_LARGE);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
	self->tldcd_value <<= TPP_CHAR_BIT;
	self->tldcd_value |= word;
	return TPP_EOK;
}

static TPP_FORMATPRINTER_DEFINE(tpp_lexer_decodecharacter_cb, arg, text, num_bytes) {
	tpp_size i;
	struct tpp_lexer_decodecharacter_data *data;
	data = (struct tpp_lexer_decodecharacter_data *)arg;
	for (i = 0; i < num_bytes; ++i) {
		tpp_errno error;
		tpp_char word = text[i];
		error = tpp_lexer_decodecharacter_data_addword(data, word);
		if (TPP_ISERR(error))
			return TPP_SSIZE_OFERR(error);
	}
	return 0;
}

#if TPP_HAVE_UNICODE
static TPP_FORMATPRINTER_DEFINE(tpp_lexer_decodecharacter_utf8_cb, arg, text, num_bytes) {
	tpp_char const *iter = text;
	tpp_char const *end = text + num_bytes;
	struct tpp_lexer_decodecharacter_data *data;
	data = (struct tpp_lexer_decodecharacter_data *)arg;
	while (iter < end) {
		tpp_errno error;
		tpp_unichar word = tpp_unicode_readutf8(&iter, end);
		error = tpp_lexer_decodecharacter_data_addword(data, word);
		if (TPP_ISERR(error))
			return TPP_SSIZE_OFERR(error);
	}
	return 0;
}
#endif /* TPP_HAVE_UNICODE */

#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
static TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_lexer_decodecharacter_big_cb(void *arg, tpp_lexer *tpp_restrict lexer, tpp_uintmax value) {
	tpp_errno error;
	struct tpp_lexer_decodecharacter_data *data;
	(void)lexer;
	data = (struct tpp_lexer_decodecharacter_data *)arg;
	error = tpp_lexer_decodecharacter_data_addword(data, value);
	return TPP_SSIZE_OFERR_OR_EOK(error);
}
#endif /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */


/* Convenience wrapper to parse a character integer literal
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsecharacter_literal(tpp_lexer *tpp_restrict self,
                                 /*out*/ tpp_uintmax *tpp_restrict p_result,
                                 unsigned int flags) {
	tpp_ssize status;
	tpp_lexer_decodestring_config config;
	struct tpp_lexer_decodecharacter_data data;
#if TPP_HAVE_TPP_W_MULTICHAR_LITERAL || TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE
	data.tldcd_lexer = self;
#endif /* TPP_HAVE_TPP_W_MULTICHAR_LITERAL || TPP_HAVE_TPP_W_CHARACTER_TOO_LARGE */
#if TPP_HAVE_TPP_W_MULTICHAR_LITERAL
	data.tldcd_count = 0;
#endif /* TPP_HAVE_TPP_W_MULTICHAR_LITERAL */
	data.tldcd_value = 0;

	config.tldsc_dataprinter = &tpp_lexer_decodecharacter_cb;
	config.tldsc_arg         = &data;

	/* Decode utf-8 multi-char sequence -- '\u1234' must equal 0x1234! */
#if TPP_HAVE_UNICODE
	config.tldsc_utf8printer = &tpp_lexer_decodecharacter_utf8_cb;
#endif /* TPP_HAVE_UNICODE */

	/* If input uses \x1234, must evaluate to 0x1234! */
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
	config.tldsc_bigprinter = &tpp_lexer_decodecharacter_big_cb;
#endif /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */

	status = tpp_lexer_parsestring_ex(self, &config, flags);
	*p_result = data.tldcd_value;
	tpp_assert(TPP_SSIZE_ISERR_OR_EOK(status));
	return TPP_SSIZE_ASERR_OR_EOK(status);
}
#endif /* TPP_HAVE_LEXER_PARSECHARACTER_LITERAL */


#if TPP_HAVE_LEXER_PARSECHARACTER_EXPR
#ifndef tpp_lexer_parsecharacter_expr
/* Convenience wrapper around `tpp_lexer_parsecharacter_literal()`
 * On success (!TPP_ISERR(return)), caller must "tpp_expr_value_fini(result)"
 *
 * @param: flags: Set of `TPP_LEXER_PARSECHARACTER_FLAG_*`
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsecharacter_expr(tpp_lexer *tpp_restrict self,
                              /*out*/ tpp_expr_value *tpp_restrict result,
                              unsigned int flags) {
	tpp_uintmax value;
	tpp_errno error = tpp_lexer_parsecharacter_literal(self, &value, flags);
	if (TPP_ISERR(error))
		return error;
	return tpp_expr_value_init_int(result, (tpp_intmax)value);
}
#endif /* !tpp_lexer_parsecharacter_expr */
#endif /* TPP_HAVE_LEXER_PARSECHARACTER_EXPR */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_DECODESTRING_C */