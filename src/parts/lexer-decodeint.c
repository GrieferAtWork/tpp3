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

#if TPP_HAVE_LEXER_DECODEINT

/* Decode the current token (which should be TPP_TOK_ISINT) into an integer
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEINT_SUFFIX
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_ex(tpp_lexer *tpp_restrict self,
                       tpp_intmax *tpp_restrict result,
                       tpp_integer_suffix_kind *p_suffix_kind)
#else /* TPP_HAVE_LEXER_DECODEINT_SUFFIX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint(tpp_lexer *tpp_restrict self,
                    tpp_intmax *tpp_restrict result)
#endif /* !TPP_HAVE_LEXER_DECODEINT_SUFFIX */
{
	unsigned int radix = 10;
	tpp_char const *start = tpp_lexer_gettokenstart(self);
	tpp_char const *end   = tpp_lexer_gettokenend(self);
	tpp_char ch;
	*result = 0;
#if TPP_HAVE_LEXER_DECODEINT_SUFFIX
#define tpp_set_suffix_kind(kind) (void)(!(p_suffix_kind) || (*(p_suffix_kind) = (kind), 1))
	tpp_set_suffix_kind(TPP_INTEGER_SUFFIX_KIND_INT);
#endif /* TPP_HAVE_LEXER_DECODEINT_SUFFIX */
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

#if TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS
		case 'o':
		case 'O':
			if (!tpp_lexer_has(self, BUILTIN_EXPR_OCTAL_LITERALS))
				break;
			if (start >= end)
				goto handle_invalid;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
			radix = 8;
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS */

		default: break;
		}
	} else {
		if (!tpp_ascii_isdigit(ch))
			goto handle_invalid;
	}

	for (;;) {
		tpp_intmax new_value, old_value;
		unsigned int digit;
		if (tpp_ascii_isdigit(ch)) {
			digit = (unsigned int)tpp_ascii_asdigit(ch);
		} else if (tpp_ascii_islwrxdigit(ch)) {
			digit = (unsigned int)tpp_ascii_aslwrxdigit(ch);
		} else if (tpp_ascii_isuprxdigit(ch)) {
			digit = (unsigned int)tpp_ascii_asuprxdigit(ch);
		} else
#if TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE
		if (ch == '_') {
			goto continue_with_ch;
#define WANT_continue_with_ch
		} else
#endif /* TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE */
#if TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK
		if (ch == '\'') {
			goto continue_with_ch;
#define WANT_continue_with_ch
		} else
#endif /* TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK */
		{
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
#ifdef WANT_continue_with_ch
#undef WANT_continue_with_ch
continue_with_ch:
#endif /* WANT_continue_with_ch */
		if (start >= end)
			return TPP_EOK;
		ch    = *start++;
		start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
	}

#if TPP_HAVE_LEXER_DECODEINT_SUFFIX
	switch (ch) {

#if TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX
	case 'i':
	case 'I':
		if (!tpp_lexer_has(self, LEXER_DECODEINT_FIXED_LENGTH_SUFFIX))
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
		    tpp_lexer_has(self, LEXER_DECODEINT_FIXED_LENGTH_SUFFIX)) {
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
#if TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX
		TPP_FALLTHRU
#else /* TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX */
		break;
#endif /* !TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX */
#endif /* TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX */


#if TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX
#if !TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX
	case 'u':
	case 'U':
#endif /* TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX */
#if TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX
	case 'l':
	case 'L':
#endif /* TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX */
#if TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX
	case 'z':
	case 'Z':
#endif /* TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX */
	{
		unsigned int has_u = 0;
#if TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX
		unsigned int has_l = 0;
#endif /* TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX */
#if TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX
		unsigned int has_z = 0;
#endif /* TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX */
		for (;;) {
			switch (ch) {
			case 'u':
			case 'U':
				++has_u;
				break;
#if TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX
			case 'l':
			case 'L':
				if (!tpp_lexer_has(self, LEXER_DECODEINT_FIXED_LENGTH_SUFFIX))
					goto handle_invalid;
				++has_l;
				break;
#endif /* TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX */
#if TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX
			case 'z':
			case 'Z':
				if (!tpp_lexer_has(self, LEXER_DECODEINT_SIZE_TYPE_SUFFIX))
					goto handle_invalid;
				++has_z;
				break;
#endif /* TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX */
			default: goto handle_invalid;
			}
			if (start >= end)
				break;
			ch    = *start++;
			start = tpp_skipbse_fwd(start, end, tpp_lexer_getfile(self));
		}
		if (has_u > 1)
			goto handle_invalid;
#if TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX
		if (has_z != 0) {
			if (has_z != 1)
				goto handle_invalid;
#if TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX
			if (has_l != 0)
				goto handle_invalid;
#endif /* TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX */
			tpp_set_suffix_kind(has_u ? TPP_INTEGER_SUFFIX_KIND_UNSIGNED_SIZE
			                          : TPP_INTEGER_SUFFIX_KIND_SIZE);
		} else
#endif /* TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX */
		{
#if TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX
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
#endif /* TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX */
		}
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX */

	default: break;
	}
#undef tpp_set_suffix_kind
#endif /* TPP_HAVE_LEXER_DECODEINT_SUFFIX */

handle_invalid:
#if TPP_HAVE_TPP_W_INVALID_INTEGER
	return tpp_lexer_warnf(self, TPP_W_INVALID_INTEGER);
#else /* TPP_HAVE_TPP_W_INVALID_INTEGER */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
}
#endif /* TPP_HAVE_LEXER_DECODEINT */


#if TPP_HAVE_LEXER_DECODEFLOAT
/* Decode the current token (which should be TPP_TOK_ISFLOAT) into a float
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat_ex(tpp_lexer *tpp_restrict self,
                         tpp_float *tpp_restrict result,
                         tpp_float_suffix_kind *p_suffix_kind)
#else /* TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat(tpp_lexer *tpp_restrict self,
                      tpp_float *tpp_restrict result)
#endif /* !TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX */
{
	/* TODO */
	(void)self;
#if TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX
	if (p_suffix_kind)
		*p_suffix_kind = TPP_FLOAT_SUFFIX_KIND_DEFAULT;
#endif /* TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX */
	*result = 0.0;
/*handle_invalid:*/
#if TPP_HAVE_TPP_W_INVALID_FLOAT
	return tpp_lexer_warnf(self, TPP_W_INVALID_FLOAT);
#else /* TPP_HAVE_TPP_W_INVALID_FLOAT */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_FLOAT */
}
#endif /* TPP_HAVE_LEXER_DECODEFLOAT */



/* Decode the current token (which should be TPP_TOK_ISINT) into an integer
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

/* Decode the current token (which should be TPP_TOK_ISFLOAT) into a float
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
