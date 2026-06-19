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
#ifndef GUARD_TPP_LEXER_DECODEINT_C
#define GUARD_TPP_LEXER_DECODEINT_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "expr.h"
#include "lexer.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* Decode the current token (which should be TPP_TOK_INT) into an integer
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEINT
#if TPP_HAVE_INTEGER_SUFFIX_KIND
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_ex(tpp_lexer *tpp_restrict self,
                       tpp_intmax *tpp_restrict result,
                       tpp_integer_suffix_kind *p_suffix_kind)
#else /* TPP_HAVE_INTEGER_SUFFIX_KIND */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint(tpp_lexer *tpp_restrict self,
                    tpp_intmax *tpp_restrict result)
#endif /* !TPP_HAVE_INTEGER_SUFFIX_KIND */
{
	unsigned int radix = 10;
	tpp_char const *start = tpp_lexer_gettokenstart(self);
	tpp_char const *end   = tpp_lexer_gettokenend(self);
	tpp_char ch;
	*result = 0;
#if TPP_HAVE_INTEGER_SUFFIX_KIND
#define tpp_set_suffix_kind(kind) (void)(!(p_suffix_kind) || (*(p_suffix_kind) = (kind), 1))
	tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT);
#endif /* TPP_HAVE_INTEGER_SUFFIX_KIND */
	if (start >= end)
		goto handle_invalid;
	ch = *start++;
	start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
	if (ch == '0') {
		if (start >= end)
			return TPP_EOK;
		radix = 8;
		ch = *start++;
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		switch (ch) {

		case 'x':
		case 'X':
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			radix = 16;
			break;

#if TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS
		case 'b':
		case 'B':
			if (!tpp_lexer_has(self, BUILTIN_EXPR_BINARY_LITERALS))
				break;
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			radix = 2;
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS */

		default: break;
		}
	} else {
		if (!tpp_ascii_isdigit(ch))
			goto handle_invalid;
	}

	do {
		tpp_intmax new_value, old_value;
		unsigned int digit;
		if (ch >= '0' && ch <= '9') {
			digit = (unsigned int)(ch - '0');
		} else if (ch >= 'a' && ch <= 'z') {
			digit = 10 + (unsigned int)(ch - 'a');
		} else if (ch >= 'A' && ch <= 'Z') {
			digit = 10 + (unsigned int)(ch - 'A');
		} else {
			break;
		}
		if (digit >= radix)
			break;
		old_value = *result;
		new_value = old_value;
		new_value *= radix;
		new_value += digit;
		*result = new_value;
		if (new_value < old_value)
			goto handle_invalid;
		if (start >= end)
			return TPP_EOK;
		ch    = *start++;
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
	} while (tpp_ascii_isdigit(ch));

#if TPP_HAVE_INTEGER_SUFFIX_KIND
	switch (ch) {

#if TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
	case 'i':
	case 'I':
		if (!tpp_lexer_has(self, BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS))
			goto handle_invalid;
		if (start >= end)
			goto handle_invalid;
		ch    = *start++;
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		if (ch == '8') {
			if (start < end)
				goto handle_invalid;
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT8);
		} else if (ch == '1') {
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (ch != '6')
				goto handle_invalid;
			if (start < end)
				goto handle_invalid;
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT16);
		} else if (ch == '3') {
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (ch != '2')
				goto handle_invalid;
			if (start < end)
				goto handle_invalid;
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT32);
		} else if (ch == '6') {
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (ch != '4')
				goto handle_invalid;
			if (start < end)
				goto handle_invalid;
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT64);
		} else {
			goto handle_invalid;
		}
		return TPP_EOK;

	case 'u':
	case 'U': {
		if (start < end && (*start == 'i' || *start == 'I') &&
		    tpp_lexer_has(self, BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS)) {
			++start;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			if (ch == '8') {
				if (start < end)
					goto handle_invalid;
				tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UINT8);
			} else if (ch == '1') {
				if (start >= end)
					goto handle_invalid;
				ch    = *start++;
				start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
				if (ch != '6')
					goto handle_invalid;
				if (start < end)
					goto handle_invalid;
				tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UINT16);
			} else if (ch == '3') {
				if (start >= end)
					goto handle_invalid;
				ch    = *start++;
				start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
				if (ch != '2')
					goto handle_invalid;
				if (start < end)
					goto handle_invalid;
				tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UINT32);
			} else if (ch == '6') {
				if (start >= end)
					goto handle_invalid;
				ch    = *start++;
				start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
				if (ch != '4')
					goto handle_invalid;
				if (start < end)
					goto handle_invalid;
				tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UINT64);
			} else {
				goto handle_invalid;
			}
			return TPP_EOK;
		}
	}
#if TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS
		TPP_FALLTHRU
#else /* TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */
		break;
#endif /* !TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */


#if TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS
#if !TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
	case 'u':
	case 'U':
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */
	case 'l':
	case 'L': {
		unsigned int has_u = 0;
		unsigned int has_l = 0;
		if (!tpp_lexer_has(self, BUILTIN_EXPR_FIXED_TYPE_INTEGRALS))
			break;
		for (;;) {
			if (ch == 'u' || ch == 'U') {
				++has_u;
			} else if (ch == 'l' || ch == 'L') {
				++has_l;
			} else {
				goto handle_invalid;
			}
			if (start >= end)
				break;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		}
		if (has_u > 1)
			goto handle_invalid;
		switch (has_l) {
		case 0:
			tpp_assert(has_u == 1);
			tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_UNSIGNED);
			break;
		case 1:
			tpp_set_suffix_kind(has_u ? TPP_INTEGER_SUFFIX_KIND_UNSIGNED_LONG
			                          : TPP_INTEGER_SUFFIX_KIND_LONG);
			break;
		case 2:
			tpp_set_suffix_kind(has_u ? TPP_INTEGER_SUFFIX_KIND_UNSIGNED_LONG_LONG
			                          : TPP_INTEGER_SUFFIX_KIND_LONG_LONG);
			break;
		default: goto handle_invalid;
		}
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */

	default: break;
	}
#undef tpp_set_suffix_kind
#endif /* TPP_HAVE_INTEGER_SUFFIX_KIND */

handle_invalid:
#if TPP_HAVE_TPP_W_INVALID_INTEGER
	return tpp_lexer_warnf(self, TPP_W_INVALID_INTEGER);
#else /* TPP_HAVE_TPP_W_INVALID_INTEGER */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
}
#endif /* TPP_HAVE_LEXER_DECODEINT */


/* Decode the current token (which should be TPP_TOK_FLOAT) into a float
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEFLOAT
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat(tpp_lexer *tpp_restrict self,
                      tpp_float *tpp_restrict result) {
	/* TODO */
	(void)self;
	*result = 0.0;
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_DECODEFLOAT */



/* Decode the current token (which should be TPP_TOK_INT) into an integer
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEINT_EXPR
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_expr(tpp_lexer *tpp_restrict self,
                         tpp_expr_value *tpp_restrict result) {
	tpp_intmax value;
	tpp_errno error = tpp_lexer_decodeint(self, &value);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_init_int(result, value);
	return error;
}
#endif /* TPP_HAVE_LEXER_DECODEINT_EXPR */

/* Decode the current token (which should be TPP_TOK_FLOAT) into a float
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEFLOAT_EXPR
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat_expr(tpp_lexer *tpp_restrict self,
                           tpp_expr_value *tpp_restrict result) {
	tpp_float value;
	tpp_errno error = tpp_lexer_decodefloat(self, &value);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_init_float(result, value);
	return error;
}
#endif /* TPP_HAVE_LEXER_DECODEFLOAT_EXPR */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_DECODEINT_C */
