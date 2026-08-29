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
#include "preparse.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_DECODEINT
#if TPP_HAVE_TOK_C_INT || TPP_HAVE_TOK_C_FLOAT
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decode_c_int(tpp_lexer *tpp_restrict self,
                       /*out*/ tpp_intvalue *tpp_restrict result,
                       tpp_char const **p_suffix_start) {
	tpp_errno error;
	unsigned int radix = 10;
	tpp_char const *iter = tpp_lexer_gettokenstart(self);
	tpp_char const *const end = tpp_lexer_gettokenend(self);
	tpp_char const *suffix_start = iter;
	tpp_char ch;
	error = tpp_intvalue_init_zero(result);
	if (TPP_ISERR(error))
		return error;
	if (iter >= end)
		goto handle_invalid;
	ch   = *iter++;
	iter = tpp_preparse_skipbse_fwd(self, iter, end);
	if (ch == '0') {
		suffix_start = iter;
		if (iter >= end)
			goto done;
		radix = 8;
		ch    = *iter++;
		iter  = tpp_preparse_skipbse_fwd(self, iter, end);
		switch (ch) {

#if TPP_HAVE_LEXER_DECODEINT_HEX_LITERALS
		case 'x':
		case 'X':
			if (!tpp_lexer_has(self, LEXER_DECODEINT_HEX_LITERALS))
				break;
			if (iter >= end)
				goto handle_invalid;
			ch    = *iter++;
			iter  = tpp_preparse_skipbse_fwd(self, iter, end);
			radix = 16;
			break;
#endif /* TPP_HAVE_LEXER_DECODEINT_HEX_LITERALS */

#if TPP_HAVE_LEXER_DECODEINT_BINARY_LITERALS
		case 'b':
		case 'B':
			if (!tpp_lexer_has(self, LEXER_DECODEINT_BINARY_LITERALS))
				break;
			if (iter >= end)
				goto handle_invalid;
			ch    = *iter++;
			iter  = tpp_preparse_skipbse_fwd(self, iter, end);
			radix = 2;
			break;
#endif /* TPP_HAVE_LEXER_DECODEINT_BINARY_LITERALS */

#if TPP_HAVE_LEXER_DECODEINT_OCTAL_LITERALS
		case 'o':
		case 'O':
			if (!tpp_lexer_has(self, LEXER_DECODEINT_OCTAL_LITERALS))
				break;
			if (iter >= end)
				goto handle_invalid;
			ch    = *iter++;
			iter  = tpp_preparse_skipbse_fwd(self, iter, end);
			radix = 8;
			break;
#endif /* TPP_HAVE_LEXER_DECODEINT_OCTAL_LITERALS */

		default: break;
		}
	} else {
		if (!tpp_ascii_isdigit(ch))
			goto handle_invalid;
	}

	for (;;) {
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
		error = tpp_intvalue_muladd(result, radix, digit);
		if (TPP_ISERR(error)) {
			if (error == TPP_ENOENT)
				goto handle_invalid;
			goto err_r;
		}

		suffix_start = iter;
#ifdef WANT_continue_with_ch
#undef WANT_continue_with_ch
continue_with_ch:
#endif /* WANT_continue_with_ch */
		if (iter >= end)
			break;
		ch   = *iter++;
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
	}

done:
	if (p_suffix_start)
		*p_suffix_start = suffix_start;
	return TPP_EOK;
handle_invalid:
	if (p_suffix_start)
		*p_suffix_start = end;
#if TPP_HAVE_TPP_W_INVALID_INTEGER
	error = tpp_lexer_warnf(self, TPP_W_INVALID_INTEGER);
#else /* TPP_HAVE_TPP_W_INVALID_INTEGER */
	error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
err_r:
	if (TPP_ISERR(error))
		tpp_intvalue_fini(result);
	return error;
}
#endif /* TPP_HAVE_TOK_C_INT || TPP_HAVE_TOK_C_FLOAT */

#if TPP_HAVE_TOK_PASCAL_HEX
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decode_pascal_hex(tpp_lexer *tpp_restrict self,
                            /*out*/ tpp_intvalue *tpp_restrict result) {
	tpp_errno error;
	tpp_char const *iter = tpp_lexer_gettokenstart(self);
	tpp_char const *const end = tpp_lexer_gettokenend(self);
	tpp_char ch;
	error = tpp_intvalue_init_zero(result);
	if (TPP_ISERR(error))
		return error;
	if (iter >= end)
		goto handle_invalid;
	ch   = *iter++;
	iter = tpp_preparse_skipbse_fwd(self, iter, end);
	if (ch != '$')
		goto handle_invalid;
	if (iter >= end)
		goto handle_invalid;
	do {
		unsigned int digit;
		ch   = *iter++;
		iter = tpp_preparse_skipbse_fwd(self, iter, end);
		if (tpp_ascii_isdigit(ch)) {
			digit = (unsigned int)tpp_ascii_asdigit(ch);
		} else if (tpp_ascii_islwrxdigit(ch)) {
			digit = (unsigned int)tpp_ascii_aslwrxdigit(ch);
		} else if (tpp_ascii_isuprxdigit(ch)) {
			digit = (unsigned int)tpp_ascii_asuprxdigit(ch);
		} else {
			goto handle_invalid;
		}
		error = tpp_intvalue_muladd(result, 16, digit);
		if (TPP_ISERR(error)) {
			if (error == TPP_ENOENT)
				goto handle_invalid;
			goto err_r;
		}
	} while (iter < end);
	return TPP_EOK;
handle_invalid:
#if TPP_HAVE_TPP_W_INVALID_INTEGER
	error = tpp_lexer_warnf(self, TPP_W_INVALID_INTEGER);
#else /* TPP_HAVE_TPP_W_INVALID_INTEGER */
	error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
err_r:
	if (TPP_ISERR(error))
		tpp_intvalue_fini(result);
	return error;
}
#endif /* TPP_HAVE_TOK_PASCAL_HEX */



/* Decode the current token (which should be `TPP_TOK_ISNUMBER`) into an integer
 * When the current token is `TPP_TOK_ISFLOAT`, it will be trimmed (i.e. decimal
 * and exponents will be removed from the token, such that the characters that
 * make up those parts of the token will be yielded next)
 *
 * @param: p_suffix_start: When non-NULL, store a pointer to the first character
 *                         of the int token's suffix (e.g. `ull` in `123ull`).
 *                         Note that when `TPP_HAVE_BSE` is enabled, you will
 *                         have to skip over `\`-sequences yourself. However, TPP
 *                         will ensure that pointer written here points *after*
 *                         a potential BSE between the int and the suffix. The
 *                         suffix always ends at `tpp_lexer_gettokenend(self)`,
 *                         and if there is no suffix, this function will store
 *                         a pointer to `tpp_lexer_gettokenend(self)` instead.
 * @return: TPP_EOK:       Success (caller must finalize `*result`)
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_ex(tpp_lexer *tpp_restrict self,
                       /*out*/ tpp_intvalue *tpp_restrict result,
                       tpp_char const **p_suffix_start) {
	tpp_token_id tok = tpp_lexer_gettok(self);
	tpp_assert(TPP_TOK_ISNUMBER(tok));
	switch (tok) {

#if TPP_HAVE_TOK_C_FLOAT
	case TPP_TOK_C_FLOAT: {
		/* Stop integer token prematurely if there's one of . + - */
		tpp_char const *start = tpp_lexer_gettokenstart(self);
		tpp_char const *end = tpp_lexer_gettokenend(self);
		tpp_char const *newend = start;
		while (newend < end && (*newend != '.' &&
		                        *newend != '+' &&
		                        *newend != '-'))
			++newend;
		if (newend <= start) {
			/* Special case for something like `.123` -- this can never be an integer :( */
			tpp_errno error;
			error = tpp_intvalue_init_zero(result);
			if (TPP_ISERR(error))
				return error;
			if (p_suffix_start)
				*p_suffix_start = tpp_lexer_gettokenend(self);
#if TPP_HAVE_TPP_W_INVALID_INTEGER
			error = tpp_lexer_warnf(self, TPP_W_INVALID_INTEGER);
#else /* TPP_HAVE_TPP_W_INVALID_INTEGER */
			error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
			if (TPP_ISERR(error))
				tpp_intvalue_fini(result);
			return error;
		}
		if (newend < end) {
			newend = tpp_preparse_skipbse_bck(self, start, newend);
			if (newend > start)
				tpp_lexer_gettoken(self)->tt_end = newend;
		}
	}
#if TPP_HAVE_TOK_C_INT
		TPP_FALLTHRU
#else /* TPP_HAVE_TOK_C_INT */
		return tpp_lexer_decode_c_int(self, result, p_suffix_start);
#endif /* !TPP_HAVE_TOK_C_INT */
#endif /* TPP_HAVE_TOK_C_FLOAT */
#if TPP_HAVE_TOK_C_INT
	case TPP_TOK_C_INT:
		return tpp_lexer_decode_c_int(self, result, p_suffix_start);
#endif /* TPP_HAVE_TOK_C_INT */

#if TPP_HAVE_TOK_PASCAL_HEX
	case TPP_TOK_PASCAL_HEX:
		if (p_suffix_start)
			*p_suffix_start = tpp_lexer_gettokenend(self);
		return tpp_lexer_decode_pascal_hex(self, result);
#endif /* TPP_HAVE_TOK_PASCAL_HEX */

	default: tpp_unreachable();
	}
	tpp_unreachable();
}
#endif /* TPP_HAVE_LEXER_DECODEINT */



#if TPP_HAVE_LEXER_DECODEFLOAT
#if TPP_HAVE_TOK_C_FLOAT
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decode_c_float(tpp_lexer *tpp_restrict self,
                         tpp_float *tpp_restrict result,
                         tpp_char const **p_suffix_start) {
	/* TODO: Decode string-to-float */
	*result = 0.0;
	if (p_suffix_start)
		*p_suffix_start = tpp_lexer_gettokenend(self);
/*handle_invalid:*/
#if TPP_HAVE_TPP_W_INVALID_FLOAT
	return tpp_lexer_warnf(self, TPP_W_INVALID_FLOAT);
#else /* TPP_HAVE_TPP_W_INVALID_FLOAT */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_FLOAT */
}
#endif /* TPP_HAVE_TOK_C_FLOAT */

/* Decode the current token (which should be `TPP_TOK_ISNUMBER`) into a float.
 * When the current token is `TPP_TOK_ISINT`, it will be parsed as integer
 * first, and that integer will be converted into the equivalent `tpp_float`.
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat_ex(tpp_lexer *tpp_restrict self,
                         tpp_float *tpp_restrict result,
                         tpp_char const **p_suffix_start) {
	tpp_token_id tok = tpp_lexer_gettok(self);
	tpp_assert(TPP_TOK_ISNUMBER(tok));
	switch (tok) {

#if TPP_HAVE_TOK_C_FLOAT
	case TPP_TOK_C_FLOAT:
		return tpp_lexer_decode_c_float(self, result, p_suffix_start);
#endif /* TPP_HAVE_TOK_C_FLOAT */

#if TPP_HAVE_TOK_INT
	TPP_CASE_TPP_TOK_INT {
#if TPP_HAVE_LEXER_DECODEINT
		tpp_intvalue expr_intvalue;
		tpp_errno error = tpp_lexer_decodeint_ex(self, &expr_intvalue, p_suffix_start);
		if (!TPP_ISERR(error)) {
			tpp_intmax intvalue;
			error = tpp_intvalue_asintmax(&expr_intvalue, &intvalue);
			if (TPP_ISERR(error)) {
#if TPP_INTVALUE_ASINTMAX_CANOVERFLOW
				if (error == TPP_ENOENT) {
#if TPP_HAVE_TPP_W_INVALID_INTEGER
					error = tpp_lexer_warnf(self, TPP_W_INVALID_INTEGER);
#else  /* TPP_HAVE_TPP_W_INVALID_INTEGER */
					error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
				}
#endif /* TPP_INTVALUE_ASINTMAX_CANOVERFLOW */
				intvalue = 0;
			}
			tpp_intvalue_fini(&expr_intvalue);
			*result = (tpp_float)intvalue;
		}
		return error;
#else /* TPP_HAVE_LEXER_DECODEINT */
		*result = 0.0;
		if (p_suffix_start)
			*p_suffix_start = tpp_lexer_gettokenend(self);
#if TPP_HAVE_TPP_W_INVALID_FLOAT
		return tpp_lexer_warnf(self, TPP_W_INVALID_FLOAT);
#else /* TPP_HAVE_TPP_W_INVALID_FLOAT */
		return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_FLOAT */
#endif /* !TPP_HAVE_LEXER_DECODEINT */
	}
#endif /* TPP_HAVE_TOK_INT */

	default: tpp_unreachable();
	}
	tpp_unreachable();
}
#endif /* TPP_HAVE_LEXER_DECODEFLOAT */



/* Decode the current token (which should be `TPP_TOK_ISNUMBER`) into an integer
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EUSER(*):  User-defined error from hook */
#if TPP_HAVE_LEXER_DECODEINT_EXPR
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_expr(tpp_lexer *tpp_restrict self,
                         tpp_expr_value *tpp_restrict result) {
	tpp_intvalue value;
	tpp_errno error = tpp_lexer_decodeint(self, &value);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_init_expr_intvalue(result, value);
	return error;
}
#endif /* TPP_HAVE_LEXER_DECODEINT_EXPR */

/* Decode the current token (which should be `TPP_TOK_ISNUMBER`) into a float
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EUSER(*):  User-defined error from hook */
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
