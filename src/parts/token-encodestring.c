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


#if TPP_HAVE_TOKEN_REQUIRE_WHITESPACE
static TPP_CONSTCALL TPP_WUNUSED bool TPPCALL
tpp_token_is_keyword_like(tpp_token_id tok) {
	switch (tok) {

#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_STRING_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	case TPP_TOK_CXX_WIDE_STRING_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	case TPP_TOK_CXX_UTF8_STRING_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	case TPP_TOK_CXX_UTF16_STRING_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	case TPP_TOK_CXX_UTF32_STRING_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	case TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
	case TPP_TOK_RAW_STRING_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_CHAR_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	case TPP_TOK_CXX_WIDE_CHAR_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
	case TPP_TOK_CXX_UTF8_CHAR_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
	case TPP_TOK_CXX_UTF16_CHAR_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
	case TPP_TOK_CXX_UTF32_CHAR_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	case TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	case TPP_TOK_RAW_CHAR_LITERAL:
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_UNICODE
	case TPP_TOK_UNICHAR:
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	case TPP_TOK_INT:
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	case TPP_TOK_FLOAT:
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if !TPP_HAVE_TPP_TOK_INT && !TPP_HAVE_TPP_TOK_FLOAT
	case TPP_TOK_0:
	case TPP_TOK_1:
	case TPP_TOK_2:
	case TPP_TOK_3:
	case TPP_TOK_4:
	case TPP_TOK_5:
	case TPP_TOK_6:
	case TPP_TOK_7:
	case TPP_TOK_8:
	case TPP_TOK_9:
#endif /* !TPP_HAVE_TPP_TOK_INT && !TPP_HAVE_TPP_TOK_FLOAT */
		return true;

	default: break;
	}
	return TPP_TOK_ISKEYWORD(tok);
}

/* Check if 2 tokens, when written directly adjacent to each other,
 * *might* (though not necessarily) result in a different set of
 * tokens when re-parsed. */
TPP_IMPL TPP_CONSTCALL TPP_WUNUSED bool TPPCALL
tpp_token_require_whitespace(tpp_token_id lhs, tpp_token_id rhs) {
	if (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT_OR_EOF(lhs) ||
	    TPP_TOK_ISSPACE_OR_LF_OR_COMMENT_OR_EOF(rhs))
		return false; /* Whitespace or comment -> never need extra whitespace */
	if (tpp_token_is_keyword_like(lhs) && tpp_token_is_keyword_like(rhs))
		return true; /* If both sides are keyword-like, always need whitespace */
	if (!TPP_TOK_ISKEYWORD(lhs) && !TPP_TOK_ISKEYWORD(rhs)) {
		/* This case here could be made way more complicated, since it could
		 * be made to includes stuff like:
		 * >> if (lhs == TPP_TOK_PLUS && rhs == TPP_TOK_EQUAL)
		 * >>     return tpp_lexer_has(<lexer>, TPP_TOK_PLUS_EQUAL);
		 *
		 * ... but for the sake of keeping this somewhat simple, we just always
		 *     require additional whitespace between single/multi-char tokens. */
		return true;
	}
	return false;
}
#endif /* TPP_HAVE_TOKEN_REQUIRE_WHITESPACE */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_TOKEN_ENCODESTRING_C */
