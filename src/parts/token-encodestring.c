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
#ifndef GUARD_TPP_TOKEN_ENCODESTRING_C
#define GUARD_TPP_TOKEN_ENCODESTRING_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_TOKEN_ENCODESTRING
/* `\`-encode `data...+=num_bytes` by passing it to `printer`
 * NOTE: the leading/trailing `"` (or `'`) character is *NOT* printed!
 *
 * @return: >= 0: Sum of positive return values of `printer`
 * @return: < 0:  First negative return value of `printer`.
 *                Note that this function never causes errors
 *                on its own, meaning that the meaning of
 *                *all* negative values is entirely up to the
 *                given `printer`! */
TPP_IMPL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_token_encodestring(tpp_formatprinter printer, void *arg,
                       void const *data, tpp_size num_bytes) {
#if TPP_HAVE_UNICODE
#if !TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT) && TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX_BRACE)
	tpp_char esc_byte_buf[sizeof("\\x{ff}")];
#define tpp_encodestring_set_output_repr_byte(value)         \
	(esc_byte_buf[0] = '\\',                                 \
	 esc_byte_buf[1] = 'x',                                  \
	 esc_byte_buf[2] = '{',                                  \
	 esc_byte_buf[3] = tpp_ascii_tolwrxdigit((value) >> 4),  \
	 esc_byte_buf[4] = tpp_ascii_tolwrxdigit((value) & 0xf), \
	 esc_byte_buf[5] = '}',                                  \
	 esc_byte_buf[6] = '\0',                                 \
	 output_repr     = esc_byte_buf)
#elif !TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT) && TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT_BRACE)
	tpp_char esc_byte_buf[sizeof("\\o{377}")];
#define tpp_encodestring_set_output_repr_byte(value)             \
	(esc_byte_buf[0] = '\\',                                     \
	 esc_byte_buf[1] = 'o',                                      \
	 esc_byte_buf[2] = '{',                                      \
	 esc_byte_buf[3] = tpp_ascii_ofoctdigit((value) >> 6),       \
	 esc_byte_buf[4] = tpp_ascii_ofoctdigit(((value) >> 3) & 7), \
	 esc_byte_buf[5] = tpp_ascii_ofoctdigit((value) & 7),        \
	 esc_byte_buf[6] = '}',                                      \
	 esc_byte_buf[7] = '\0',                                     \
	 output_repr     = esc_byte_buf)
#else /* ... */
	tpp_char esc_byte_buf[sizeof("\\377")];
#define tpp_encodestring_set_output_repr_byte(value)             \
	(esc_byte_buf[0] = '\\',                                     \
	 esc_byte_buf[1] = tpp_ascii_ofoctdigit((value) >> 6),       \
	 esc_byte_buf[2] = tpp_ascii_ofoctdigit(((value) >> 3) & 7), \
	 esc_byte_buf[3] = tpp_ascii_ofoctdigit((value) & 7),        \
	 esc_byte_buf[4] = '\0',                                     \
	 output_repr     = esc_byte_buf)
#endif /* !... */
#endif /* TPP_HAVE_UNICODE */
	tpp_char const *output_repr;
	tpp_ssize temp, result = 0;
	tpp_char const *iter = (tpp_char const *)data;
	tpp_char const *end  = iter + num_bytes;
	tpp_char ch;
again:
	if (iter >= end) {
		temp = tpp_formatprinter_print(printer, arg, (tpp_char const *)data,
		                               (tpp_size)(end - (tpp_char const *)data));
		if (temp < 0)
			return temp;
		result += temp;
		return result;
	}
	ch = *iter++;
	switch (ch) {
#define TPP_TOKEN_ENCODESTRING_CASE(b, repr)  \
	case b: {                                 \
		static tpp_char const _repr[] = repr; \
		output_repr = _repr;                  \
	}	break;

	/* Only really need to escape \ " ' CR LF and (TPP_HAVE_UNICODE-only)
	 * ordinals >=0xC0 that *might* form unicode line-feed characters. */

	/* To prevent problems with "strlen()" and the like... 
	 * NOTE: Must encode 00h as \000 instead of \0 to prevent ambiguity with "\N{NUL}00" */
#if TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI)
	TPP_TOKEN_ENCODESTRING_CASE('\0', "\\u0000");
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI_BRACE)
	TPP_TOKEN_ENCODESTRING_CASE('\0', "\\u{0}");
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_XML)
	TPP_TOKEN_ENCODESTRING_CASE('\0', "\\&#0;");
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT)
	TPP_TOKEN_ENCODESTRING_CASE('\0', "\\000");
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX_BRACE)
	TPP_TOKEN_ENCODESTRING_CASE('\0', "\\x{0}");
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT_BRACE)
	TPP_TOKEN_ENCODESTRING_CASE('\0', "\\o{0}");
#else /* ... */
	TPP_TOKEN_ENCODESTRING_CASE('\0', "\\000"); /* May not be decodable... */
#endif /* !... */

	TPP_TOKEN_ENCODESTRING_CASE('\\', "\\\\");
	TPP_TOKEN_ENCODESTRING_CASE('\'', "\\\'");
	TPP_TOKEN_ENCODESTRING_CASE('"', "\\\"");
	TPP_TOKEN_ENCODESTRING_CASE(TPP_ASCII_CR, "\\r");
	TPP_TOKEN_ENCODESTRING_CASE(TPP_ASCII_LF, "\\n");
#if TPP_HAVE_TRIGRAPHS
	case '?':
		/* Needed to prevent trigraphs being matched */
		if (iter < end) {
			if (iter[0] != '?')
				goto again; /* Don't need to escape if next char isn't another '?' */
			if ((iter + 1) < end) {
				/* Only need to escape if final char isn't available, or
				 * could form a valid trigraph sequence. */
				switch (iter[1]) {
				case '=':
				case '(':
				case ')':
				case '\'':
				case '<':
				case '!':
				case '>':
				case '-':
				case '/':
					/* Yes: must escape! */
					break;
				default:
					/* Not a valid trigraph */
					++iter; /* Directly flush 2nd '?' */
					if (*iter == '?')
						++iter; /* Directly flush 3rd '?' */
					goto again;
				}
			} else {
				/* At least the first ?-character doesn't need to be escaped */
				++iter;
			}
		} else {
			/* If we're at the end of the string, must always escape '?',
			 * because the caller may write some more text, in which case
			 * said text may start with "?" and could accidentally form a
			 * valid trigraph */
		}
		output_repr = (tpp_char const *)"\\?";
		break;
#endif /* TPP_HAVE_TRIGRAPHS */

#if TPP_HAVE_UNICODE
	/* Must really escape *all* bytes `>= 0x80` when they don't form a valid
	 * utf-8 sequences (anything that might trigger `TPP_W_ILLEGAL_UTF8_SEQUENCE`
	 * needs to be escaped). */
	case 0xc0: case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6: case 0xc7:
	case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc: case 0xcd: case 0xce: case 0xcf:
	case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7:
	case 0xd8: case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde: case 0xdf:
	case 0xe0: case 0xe1: case 0xe2: case 0xe3: case 0xe4: case 0xe5: case 0xe6: case 0xe7:
	case 0xe8: case 0xe9: case 0xea: case 0xeb: case 0xec: case 0xed: case 0xee: case 0xef:
	case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf7:
	case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0xff: {
		tpp_char const *utf8_start = iter - 1;
		uint_least8_t utf8_size    = tpp_unicode_utf8seqlen_mb_getcur(ch);
		tpp_char const *utf8_end   = utf8_start + utf8_size;
		if (utf8_start < utf8_end && utf8_end <= end) {
			/* Verify that this is a valid utf-8 sequence */
			tpp_unichar uc;
			uint_least8_t i;

			/* Validate follow-up bytes. */
			for (i = 1; i < utf8_size; ++i) {
				if (!tpp_ascii_isutf8cont(utf8_start[i]))
					goto encode_as_byte;
			}

			/* Decode utf-8 character */
			uc = ch;
			switch (utf8_size) {

			case 2:
				uc = (uc & 0x1f) << 6;
				uc |= (utf8_start[1] & 0x3f);
				if (uc <= TPP_UTF8_1BYTE_MAX)
					goto encode_as_byte; /* under-long utf-8 sequence */
				break;

#if TPP_UTF8_CURLEN >= 3
			case 3:
				uc = (uc & 0x0f) << 12;
				uc |= (utf8_start[1] & 0x3f) << 6;
				uc |= (utf8_start[2] & 0x3f);
				if (uc <= TPP_UTF8_2BYTE_MAX)
					goto encode_as_byte; /* under-long utf-8 sequence */
				break;
#endif /* TPP_UTF8_CURLEN >= 3 */

#if TPP_UTF8_CURLEN >= 4
			case 4:
				uc = (uc & 0x07) << 18;
				uc |= (utf8_start[1] & 0x3f) << 12;
				uc |= (utf8_start[2] & 0x3f) << 6;
				uc |= (utf8_start[3] & 0x3f);
				if (uc <= TPP_UTF8_3BYTE_MAX)
					goto encode_as_byte; /* under-long utf-8 sequence */
				break;
#endif /* TPP_UTF8_CURLEN >= 4 */

#if TPP_UTF8_CURLEN >= 5
			case 5:
				uc = (uc & 0x03) << 24;
				uc |= (utf8_start[1] & 0x3f) << 18;
				uc |= (utf8_start[2] & 0x3f) << 12;
				uc |= (utf8_start[3] & 0x3f) << 6;
				uc |= (utf8_start[4] & 0x3f);
				if (uc <= TPP_UTF8_4BYTE_MAX)
					goto encode_as_byte; /* under-long utf-8 sequence */
				break;
#endif /* TPP_UTF8_CURLEN >= 5 */

#if TPP_UTF8_CURLEN >= 6
			case 6:
				uc = (uc & 0x01) << 30;
				uc |= (utf8_start[1] & 0x3f) << 24;
				uc |= (utf8_start[2] & 0x3f) << 18;
				uc |= (utf8_start[3] & 0x3f) << 12;
				uc |= (utf8_start[4] & 0x3f) << 6;
				uc |= (utf8_start[5] & 0x3f);
				if (uc <= TPP_UTF8_5BYTE_MAX)
					goto encode_as_byte; /* under-long utf-8 sequence */
				break;
#endif /* TPP_UTF8_CURLEN >= 6 */

#if TPP_UTF8_CURLEN >= 7
			case 7:
				uc = (utf8_start[1] & 0x03 /*0x3f*/) << 30;
				uc |= (utf8_start[2] & 0x3f) << 24;
				uc |= (utf8_start[3] & 0x3f) << 18;
				uc |= (utf8_start[4] & 0x3f) << 12;
				uc |= (utf8_start[5] & 0x3f) << 6;
				uc |= (utf8_start[6] & 0x3f);
				if (uc <= TPP_UTF8_6BYTE_MAX)
					goto encode_as_byte; /* under-long utf-8 sequence */
				break;
#endif /* TPP_UTF8_CURLEN >= 7 */

#if TPP_UTF8_CURLEN >= 8
#error "Unsupported 'TPP_UTF8_CURLEN'"
#endif /* TPP_UTF8_CURLEN >= 8 */
			default: tpp_unreachable();
			}

			/* If it's a line-feed, then must also escape! */
			if (tpp_unicode_islf(uc))
				goto encode_as_byte;

			/* It's valid utf-8 -- don't need to escape */
			iter = utf8_end;
			goto again;
		}
	}	TPP_FALLTHRU
		/* UTF-8 follow-up bytes must always be escaped! */
	case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
	case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f:
	case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
	case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:
	case 0xa0: case 0xa1: case 0xa2: case 0xa3: case 0xa4: case 0xa5: case 0xa6: case 0xa7:
	case 0xa8: case 0xa9: case 0xaa: case 0xab: case 0xac: case 0xad: case 0xae: case 0xaf:
	case 0xb0: case 0xb1: case 0xb2: case 0xb3: case 0xb4: case 0xb5: case 0xb6: case 0xb7:
	case 0xb8: case 0xb9: case 0xba: case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf:
encode_as_byte:
		tpp_encodestring_set_output_repr_byte(ch);
		break;
#endif /* TPP_HAVE_UNICODE */

#undef TPP_TOKEN_ENCODESTRING_CASE
	default: goto again;
	}
	temp = tpp_formatprinter_print(printer, arg, (tpp_char const *)data,
	                               (tpp_size)((iter - 1) -
	                                          (tpp_char const *)data));
	if (temp < 0)
		return temp;
	result += temp;
	temp = tpp_formatprinter_print(printer, arg, output_repr, tpp_strlen((char const *)output_repr));
	if (temp < 0)
		return temp;
	result += temp;
	data = (void const *)iter;
	goto again;
#undef tpp_encodestring_set_output_repr_byte
}
#endif /* TPP_HAVE_TOKEN_ENCODESTRING */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_TOKEN_ENCODESTRING_C */
