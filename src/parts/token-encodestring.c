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
import * from deemon;

#if 1 // Cached (since enumerating all of unicode is pretty slow)
local unicodeLineFeedOrdinals = [0x85, 0x2028, 0x2029];
#else
local unicodeLineFeedOrdinals = [];
for (local ord: [0x80: 0x10ffff+1]) {
	if (string.chr(ord).islf())
		unicodeLineFeedOrdinals.append(ord);
}
print ", ".join(unicodeLineFeedOrdinals.map(e -> e.hex()));
#endif

local unicodeLineFeedUtf8FirstBytes = HashSet();
for (local ord: unicodeLineFeedOrdinals) {
	local b = string.chr(ord).encode("utf-8").first;
	unicodeLineFeedUtf8FirstBytes.insert(b);
}

local bytes = unicodeLineFeedUtf8FirstBytes.sorted();
print("#define TPP_CASE_UTF8_FIRSTBYTE_ISLF_FOREACH(cb) \\");
print("	", " \\\n	".join(bytes.map(b -> f'cb({b.hex()}, "\\\\x{b.tostr(16, 2)}")')));
]]]*/
#define TPP_CASE_UTF8_FIRSTBYTE_ISLF_FOREACH(cb) \
	cb(0xc2, "\\xc2") \
	cb(0xe2, "\\xe2")
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
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_TOKEN_ENCODESTRING_C */
