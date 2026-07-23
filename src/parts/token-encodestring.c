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

/* \-encode "data...+=num_bytes" by passing it to "printer"
 * NOTE: Leading/trailing " (or ')-characters are *NOT* printed!
 *
 * @return: >= 0: Sum of positive return values of "printer"
 * @return: < 0:  First negative return value of "printer".
 *                Note that this function never causes errors
 *                on its own, meaning that the meaning of
 *                *all* negative values is entirely up to the
 *                given "printer"! */
TPP_IMPL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_token_encodestring(tpp_formatprinter printer, void *arg,
                       void const *data, tpp_size num_bytes) {
	tpp_char const *output_repr;
	tpp_ssize temp, result = 0;
#if TPP_HAVE_UNICODE
	tpp_char const *new_iter;
#endif /* TPP_HAVE_UNICODE */
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
#if TPP_HAVE_UNICODE
#define TPP_TOKEN_ENCODESTRING_CASE(b, repr)  \
	case b: {                                 \
		static tpp_char const _repr[] = repr; \
		new_iter = iter;                      \
		output_repr = _repr;                  \
	}	break;
#else /* TPP_HAVE_UNICODE */
#define TPP_TOKEN_ENCODESTRING_CASE(b, repr)  \
	case b: {                                 \
		static tpp_char const _repr[] = repr; \
		output_repr = _repr;                  \
	}	break;
#endif /* !TPP_HAVE_UNICODE */

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
	TPP_TOKEN_ENCODESTRING_CASE('\0', "\\000");
#endif /* !... */

	TPP_TOKEN_ENCODESTRING_CASE('\\', "\\\\");
	TPP_TOKEN_ENCODESTRING_CASE('\'', "\\\'");
	TPP_TOKEN_ENCODESTRING_CASE('\"', "\\\"");
	TPP_TOKEN_ENCODESTRING_CASE(TPP_ASCII_CR, "\\r");
	TPP_TOKEN_ENCODESTRING_CASE(TPP_ASCII_LF, "\\n");

#if TPP_HAVE_UNICODE
/*[[[deemon
import * from ".token-encodestring-mblf";
import * from deemon;
for (local b: UTF8_LF_FIRST_BYTES.sorted()) {
	print(f'	case {b.hex(2)}:');
	for (local ord: UNICODE_LF_CHARACTERS) {
		local utf8 = string.chr(ord).encode('utf-8');
		if (utf8.first == b) {
			local remainder = utf8[1:];
			print(f'		if ({#remainder >= 2 ? f"(iter + {#remainder-1})" : "iter"} < end && {' && '.join(
				for (local i, b: remainder.enumerate())
					f'iter[{i}] == {b.hex(2)}'
			)}) \{');
			print(f'			new_iter = iter + {#remainder};');
			print(f'#if TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI)');
			if (ord <= 0xffff) {
				print(f'			output_repr = (tpp_char const *)"\\\\u{ord.tostr(16, 4)}";');
			} else {
				print(f'			output_repr = (tpp_char const *)"\\\\U{ord.tostr(16, 8)}";');
			}
			print(f'#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI_BRACE)');
			print(f'			output_repr = (tpp_char const *)"\\\\u\{{ord.tostr(16)}\}";');
			print(f'#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_XML)');
			print(f'			output_repr = (tpp_char const *)"\\\\&#{ord.tostr(16)};";');
			print(f'#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT)');
			print(f'			output_repr = (tpp_char const *)"{
				''.join(for (local b: utf8[:-1]) f'\\\\{b.tostr(8)}')
			}\\\\{utf8.last.tostr(8, 3)}";');
			print('#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX_BRACE)');
			print('#if TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX)');
			print(f'			output_repr = (tpp_char const *)"{
				''.join(for (local b: utf8[:-1]) f'\\\\x{b.tostr(16)}')
			}\\\\x\{{utf8.last.tostr(16)}\}";');
			print('#else /' '* TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX) *' '/');
			print(f'			output_repr = (tpp_char const *)"{
				''.join(for (local b: utf8) f'\\\\x\{{b.tostr(16)}\}')
			}";');
			print('#endif /' '* TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX) *' '/');
			print(f'#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT_BRACE)');
			print(f'			output_repr = (tpp_char const *)"{
				''.join(for (local b: utf8) f'\\\\o\{{b.tostr(8)}\}')
			}";');
			print('#else /' '* ... *' '/');
			print(f'			output_repr = (tpp_char const *)"{
				''.join(for (local b: utf8[:-1]) f'\\\\{b.tostr(8)}')
			}\\\\{utf8.last.tostr(8, 3)}"; /' f'* May not be decodable... *' f'/');
			print('#endif /' '* !... *' '/');
			print('			break;');
			print('		}');
		}
	}
	print(f'		goto again;');
}
]]]*/
	case 0xc2:
		if (iter < end && iter[0] == 0x85) {
			new_iter = iter + 1;
#if TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI)
			output_repr = (tpp_char const *)"\\u0085";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI_BRACE)
			output_repr = (tpp_char const *)"\\u{85}";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_XML)
			output_repr = (tpp_char const *)"\\&#85;";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT)
			output_repr = (tpp_char const *)"\\302\\205";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX_BRACE)
#if TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX)
			output_repr = (tpp_char const *)"\\xc2\\x{85}";
#else /* TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX) */
			output_repr = (tpp_char const *)"\\x{c2}\\x{85}";
#endif /* TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX) */
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT_BRACE)
			output_repr = (tpp_char const *)"\\o{302}\\o{205}";
#else /* ... */
			output_repr = (tpp_char const *)"\\302\\205"; /* May not be decodable... */
#endif /* !... */
			break;
		}
		goto again;
	case 0xe2:
		if ((iter + 1) < end && iter[0] == 0x80 && iter[1] == 0xa8) {
			new_iter = iter + 2;
#if TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI)
			output_repr = (tpp_char const *)"\\u2028";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI_BRACE)
			output_repr = (tpp_char const *)"\\u{2028}";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_XML)
			output_repr = (tpp_char const *)"\\&#2028;";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT)
			output_repr = (tpp_char const *)"\\342\\200\\250";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX_BRACE)
#if TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX)
			output_repr = (tpp_char const *)"\\xe2\\x80\\x{a8}";
#else /* TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX) */
			output_repr = (tpp_char const *)"\\x{e2}\\x{80}\\x{a8}";
#endif /* TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX) */
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT_BRACE)
			output_repr = (tpp_char const *)"\\o{342}\\o{200}\\o{250}";
#else /* ... */
			output_repr = (tpp_char const *)"\\342\\200\\250"; /* May not be decodable... */
#endif /* !... */
			break;
		}
		if ((iter + 1) < end && iter[0] == 0x80 && iter[1] == 0xa9) {
			new_iter = iter + 2;
#if TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI)
			output_repr = (tpp_char const *)"\\u2029";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_UNI_BRACE)
			output_repr = (tpp_char const *)"\\u{2029}";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_XML)
			output_repr = (tpp_char const *)"\\&#2029;";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT)
			output_repr = (tpp_char const *)"\\342\\200\\251";
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX_BRACE)
#if TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX)
			output_repr = (tpp_char const *)"\\xe2\\x80\\x{a9}";
#else /* TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX) */
			output_repr = (tpp_char const *)"\\x{e2}\\x{80}\\x{a9}";
#endif /* TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_HEX) */
#elif TPP_CONF_IS_ALWAYS(TPP_HAVE_STRING_ESCAPE_OCT_BRACE)
			output_repr = (tpp_char const *)"\\o{342}\\o{200}\\o{251}";
#else /* ... */
			output_repr = (tpp_char const *)"\\342\\200\\251"; /* May not be decodable... */
#endif /* !... */
			break;
		}
		goto again;
/*[[[end]]]*/
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
#if TPP_HAVE_UNICODE
	iter = new_iter;
#endif /* TPP_HAVE_UNICODE */
	data = (void const *)iter;
	goto again;
}
#endif /* TPP_HAVE_TOKEN_ENCODESTRING */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_TOKEN_ENCODESTRING_C */
