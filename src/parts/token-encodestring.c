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

/*[[[deemon
import UTF8_LF_FIRST_BYTES from ".token-encodestring-mblf";
print("#define TPP_CASE_UTF8_FIRSTBYTE_ISLF_FOREACH(cb) \\");
print("	", " \\\n	".join(UTF8_LF_FIRST_BYTES.map(b -> f"cb({b.hex()}, '\\\\', 'x', {
	", ".join(b.tostr(16, 2).map(e -> f"'{e}'"))})")));
]]]*/
#define TPP_CASE_UTF8_FIRSTBYTE_ISLF_FOREACH(cb) \
	cb(0xc2, '\\', 'x', 'c', '2') \
	cb(0xe2, '\\', 'x', 'e', '2')
/*[[[end]]]*/


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
#define TPP_TOKEN_ENCODESTRING_CASE2(b, r1, r2)         \
	case b: {                                           \
		static tpp_char const _repr[3] = { r1, r2, 0 }; \
		output_repr = _repr;                            \
	}	break;
#define TPP_TOKEN_ENCODESTRING_CASE4(b, r1, r2, r3, r4)         \
	case b: {                                                   \
		static tpp_char const _repr[5] = { r1, r2, r3, r4, 0 }; \
		output_repr = _repr;                                    \
	}	break;
#else /* TPP_HAVE_UNICODE */
#define TPP_TOKEN_ENCODESTRING_CASE2(b, r1, r2)      \
	case b: {                                        \
		static tpp_char const _repr[2] = { r1, r2 }; \
		output_repr = _repr;                         \
	}	break;
#endif /* !TPP_HAVE_UNICODE */

	/* Only really need to escape \ " ' CR LF and (TPP_HAVE_UNICODE-only)
	 * ordinals >=0xC0 that *might* form unicode line-feed characters. */

	TPP_TOKEN_ENCODESTRING_CASE2('\\', '\\', '\\');
	TPP_TOKEN_ENCODESTRING_CASE2('\'', '\\', '\'');
	TPP_TOKEN_ENCODESTRING_CASE2('\"', '\\', '\"');
	TPP_TOKEN_ENCODESTRING_CASE2(TPP_ASCII_CR, '\\', 'r');
	TPP_TOKEN_ENCODESTRING_CASE2(TPP_ASCII_LF, '\\', 'n');

#if TPP_HAVE_UNICODE
	TPP_CASE_UTF8_FIRSTBYTE_ISLF_FOREACH(TPP_TOKEN_ENCODESTRING_CASE4)
#endif /* TPP_HAVE_UNICODE */

#undef TPP_TOKEN_ENCODESTRING_CASE4
#undef TPP_TOKEN_ENCODESTRING_CASE2
	default: goto again;
	}
	temp = tpp_formatprinter_print(printer, arg, (tpp_char const *)data,
	                               (tpp_size)((iter - 1) -
	                                          (tpp_char const *)data));
	if (temp < 0)
		return temp;
	result += temp;
#if TPP_HAVE_UNICODE
	temp = tpp_formatprinter_print(printer, arg, output_repr, tpp_strlen((char const *)output_repr));
#else /* TPP_HAVE_UNICODE */
	/* All mandatory ASCII-escape-sequences are 2 bytes long! */
	temp = tpp_formatprinter_print(printer, arg, output_repr, 2);
#endif /* !TPP_HAVE_UNICODE */
	if (temp < 0)
		return temp;
	result += temp;
	data = (void const *)iter;
	goto again;
}
#endif /* TPP_HAVE_TOKEN_ENCODESTRING */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_TOKEN_ENCODESTRING_C */
