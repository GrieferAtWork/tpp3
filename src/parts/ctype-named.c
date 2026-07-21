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
#ifndef GUARD_TPP_CTYPE_NAMED_C
#define GUARD_TPP_CTYPE_NAMED_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "lexer.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_DECODE_NAMED_ESCAPE

/* Decode a named \N{...} sequence and update `*p_iter` to point to
 * its end. This function implement the routing between the different
 * (currently enabled) named escape decoders, as configured by:
 * - `TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES`
 * - `TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD`
 * - `TPP_HAVE_ESCAPE_NAMED_XML`
 *
 * @return: 0 : Unknown named sequence (`*p_iter` is unchanged) 
 * @return: * : The # of characters written to `result` (always `<= TPP_DECODE_NAMED_ESCAPE_MAXLEN`)*/
#if TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_size TPPCALL
tpp_decode_named_escape(tpp_char const **p_iter, tpp_char const *end,
                        tpp_unichar result[TPP_DECODE_NAMED_ESCAPE_MAXLEN],
                        struct tpp_lexer const *tpp_restrict lexer)
#else /* TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
_tpp_decode_named_escape(tpp_char const **p_iter, tpp_char const *end,
                         tpp_unichar result[TPP_DECODE_NAMED_ESCAPE_MAXLEN])
#endif /* !TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM */
{
	tpp_char ch;
	tpp_char const *iter = *p_iter;
	if (iter >= end)
		goto nope;
	ch = *iter++;
	iter = tpp_skipbse_fwd(iter, end, tpp_lexer_getfile(lexer));
#if TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD
	if (((ch == 'U' && iter < end && (*iter == '+')) ||
	     (ch == '0' && iter < end && (*iter == 'x' || *iter == 'X'))) &&
	    tpp_lexer_has(lexer, ESCAPE_NAMED_UNICODE_ORD)) {
		tpp_unichar uc; /* Unicode character code (in hex) */
		tpp_char const *uc_iter;
		iter = tpp_skipbse_fwd(iter + 1, end, tpp_lexer_getfile(lexer));
		if (iter >= end)
			goto nope;
		ch = *iter++;
		if (!tpp_ascii_isxdigit(ch))
			goto nope;
		uc = tpp_ascii_asxdigit(ch);
		uc_iter = iter;
		for (;;) {
			unsigned char nibble;
			iter = uc_iter;
			uc_iter = tpp_skipbse_fwd(uc_iter, end, tpp_lexer_getfile(lexer));
			if (uc_iter >= end)
				break;
			ch = *uc_iter++;
			if (!tpp_ascii_isxdigit(ch))
				break;
			nibble = tpp_ascii_asxdigit(ch);
			if (((uc << 4) >> 4) != uc)
				break;
			uc <<= 4;
			uc |= nibble;
		}
		*p_iter = iter;
		result[0] = uc;
		return 1;
	}
	if (tpp_ascii_isdigit(ch) &&
	    tpp_lexer_has(lexer, ESCAPE_NAMED_UNICODE_ORD)) {
		/* Decimal-encoded unicode ordinal */
		tpp_unichar uc;
		tpp_char const *uc_iter;
		uc = tpp_ascii_asdigit(ch);
		uc_iter = iter = (*p_iter + 1);
		for (;;) {
			unsigned char nibble;
			iter = uc_iter;
			uc_iter = tpp_skipbse_fwd(uc_iter, end, tpp_lexer_getfile(lexer));
			if (uc_iter >= end)
				break;
			ch = *uc_iter++;
			if (!tpp_ascii_isdigit(ch))
				break;
			nibble = tpp_ascii_asdigit(ch);
			if (((uc * 10) / 10) != uc)
				break;
			uc *= 10;
			uc += nibble;
		}
		*p_iter = iter;
		result[0] = uc;
		return 1;
	}
#endif /* TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD */

#if TPP_HAVE_ESCAPE_NAMED_XML
	if (ch == '&' && tpp_lexer_has(lexer, ESCAPE_NAMED_XML)) {
		/* In addition to `tpp_xml_entity_lookup()`, must also support:
		 * - &#<decimal>;
		 * - &#x<hex>;
		 * ... both of which allow encoding of unicode ordinals */
		tpp_unichar uc;
		char xmlname[TPP_XML_ENTITY_LOOKUP_MAXLEN + 1];
		tpp_size xmlname_size = 0;
		bool has_semi;
		if (iter >= end)
			goto nope;
		ch = *iter++;
		iter = tpp_skipbse_fwd(iter, end, tpp_lexer_getfile(lexer));
		if (ch == '#') {
#if TPP_HAVE_TRIGRAPHS
handle_xml_ord:
#endif /* TPP_HAVE_TRIGRAPHS */
			if (iter >= end)
				goto nope;
			ch = *iter++;
			iter = tpp_skipbse_fwd(iter, end, tpp_lexer_getfile(lexer));
			if (ch == 'x') { /* Only lowercase 'x' is allowed here! */
				tpp_char const *uc_iter;
				if (iter >= end)
					goto nope;
				ch = *iter++;
				if (!tpp_ascii_isxdigit(ch))
					goto nope;
				uc = tpp_ascii_asxdigit(ch);
				uc_iter = iter;
				for (;;) {
					unsigned char nibble;
					iter = uc_iter;
					uc_iter = tpp_skipbse_fwd(uc_iter, end, tpp_lexer_getfile(lexer));
					if (uc_iter >= end)
						break;
					ch = *uc_iter++;
					if (!tpp_ascii_isxdigit(ch)) {
						if (ch == ';') /* Include trailing ';' */
							iter = uc_iter;
						break;
					}
					nibble = tpp_ascii_asxdigit(ch);
					if (((uc << 4) >> 4) != uc)
						break;
					uc <<= 4;
					uc |= nibble;
				}
				*p_iter = iter;
				result[0] = uc;
				return 1;
			} else if (tpp_ascii_isdigit(ch)) {
				tpp_char const *uc_iter;
				uc = tpp_ascii_asdigit(ch);
				uc_iter = iter = (*p_iter + 1);
				for (;;) {
					unsigned char nibble;
					iter = uc_iter;
					uc_iter = tpp_skipbse_fwd(uc_iter, end, tpp_lexer_getfile(lexer));
					if (uc_iter >= end)
						break;
					ch = *uc_iter++;
					if (!tpp_ascii_isdigit(ch))
						break;
					nibble = tpp_ascii_asdigit(ch);
					if (((uc * 10) / 10) != uc)
						break;
					uc *= 10;
					uc += nibble;
				}
				*p_iter = iter;
				result[0] = uc;
				return 1;
			}
			goto nope;
		} else
#if TPP_HAVE_TRIGRAPHS
		if (ch == '?' && (iter + 1) < end &&
		    iter[0] == '?' && iter[1] == '=' &&
		    tpp_lexer_has(lexer, TRIGRAPHS)) {
			iter += 2;
			goto handle_xml_ord;
		} else
#endif /* TPP_HAVE_TRIGRAPHS */
		{
		}

		xmlname[0] = (char)ch;
		xmlname_size = 1;
#if TPP_XML_ENTITY_LOOKUP_MINLEN < 2
		has_semi = iter < end && *iter == ';';
		uc = tpp_xml_entity_lookup(xmlname, has_semi);
		if (uc != TPP_XML_ENTITY_LOOKUP_UNKNOWN) {
			if (has_semi)
				++iter;
		} else
#endif /* TPP_XML_ENTITY_LOOKUP_MINLEN < 2 */
		{
			for (;;) {
				if (iter >= end)
					goto nope;
				ch = *iter++;
				iter = tpp_skipbse_fwd(iter, end, tpp_lexer_getfile(lexer));
				if (ch == ';') {
					xmlname[xmlname_size] = '\0';
					uc = tpp_xml_entity_lookup(xmlname, true);
					if (uc == TPP_XML_ENTITY_LOOKUP_UNKNOWN)
						goto nope;
					break;
				}
				xmlname[xmlname_size++] = (char)ch;
				xmlname[xmlname_size] = '\0';
				has_semi = iter < end && *iter == ';';
				uc = tpp_xml_entity_lookup(xmlname, has_semi);
				if (uc != TPP_XML_ENTITY_LOOKUP_UNKNOWN) {
					if (has_semi)
						++iter;
					break;
				}
			}
		}
		*p_iter = iter;
		result[0] = uc;
		return 1;
	}
#endif /* TPP_HAVE_ESCAPE_NAMED_XML */

	/* TODO */
	(void)iter;
	(void)end;
	(void)result;
#if TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM
	(void)lexer;
#endif /* TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM */

nope:
	return 0;
}
#endif /* TPP_HAVE_DECODE_NAMED_ESCAPE */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CTYPE_NAMED_C */
