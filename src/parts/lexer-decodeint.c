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
	tpp_intvalue_builder builder;
	unsigned int radix = 10;
	tpp_char const *iter = tpp_lexer_gettokenstart(self);
	tpp_char const *const end = tpp_lexer_gettokenend(self);
	tpp_char const *suffix_start = iter;
	tpp_char ch;
	if (iter >= end)
		goto handle_invalid;
	ch   = *iter++;
	iter = tpp_preparse_skipbse_fwd(self, iter, end);
	if (ch == '0') {
		suffix_start = iter;
		if (iter >= end) {
			error = tpp_intvalue_init_zero(result);
			goto done;
		}
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
	error = tpp_intvalue_builder_init(&builder, radix);
	if (TPP_ISERR(error))
		goto err;
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
		error = tpp_intvalue_builder_adddigit(&builder, digit);
		if (TPP_ISERR(error)) {
			if (error == TPP_ENOENT)
				goto handle_invalid_builder;
			goto err_builder;
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

	error = tpp_intvalue_builder_pack(&builder, result);
done:
	if (p_suffix_start)
		*p_suffix_start = suffix_start;
	return error;
err_builder:
	tpp_intvalue_builder_fini(&builder);
err:
	return error;
handle_invalid_builder:
	tpp_intvalue_builder_fini(&builder);
handle_invalid:
	if (p_suffix_start)
		*p_suffix_start = end;
#if TPP_HAVE_TPP_W_INVALID_INTEGER
	error = tpp_lexer_warnf(self, TPP_W_INVALID_INTEGER);
#else /* TPP_HAVE_TPP_W_INVALID_INTEGER */
	error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
	return error;
}
#endif /* TPP_HAVE_TOK_C_INT || TPP_HAVE_TOK_C_FLOAT */

#if TPP_HAVE_TOK_PASCAL_HEX
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decode_pascal_hex(tpp_lexer *tpp_restrict self,
                            /*out*/ tpp_intvalue *tpp_restrict result) {
	tpp_errno error;
	tpp_intvalue_builder builder;
	tpp_char const *iter = tpp_lexer_gettokenstart(self);
	tpp_char const *const end = tpp_lexer_gettokenend(self);
	tpp_char ch;
	error = tpp_intvalue_builder_init(&builder, 16);
	if (TPP_ISERR(error))
		return error;
	if (iter >= end)
		goto handle_invalid_builder;
	ch   = *iter++;
	iter = tpp_preparse_skipbse_fwd(self, iter, end);
	if (ch != '$')
		goto handle_invalid_builder;
	if (iter >= end)
		goto handle_invalid_builder;
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
			goto handle_invalid_builder;
		}
		error = tpp_intvalue_builder_adddigit(&builder, digit);
		if (TPP_ISERR(error)) {
			if (error == TPP_ENOENT)
				goto handle_invalid_builder;
			goto err_builder;
		}
	} while (iter < end);
	return tpp_intvalue_builder_pack(&builder, result);
handle_invalid_builder:
#if TPP_HAVE_TPP_W_INVALID_INTEGER
	error = tpp_lexer_warnf(self, TPP_W_INVALID_INTEGER);
#else /* TPP_HAVE_TPP_W_INVALID_INTEGER */
	error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
err_builder:
	tpp_intvalue_builder_fini(&builder);
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
				tpp_lexer_settokenend(self, newend);
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



#if TPP_HAVE_FLOATVALUE_PACK

static TPP_CONSTCALL TPP_WUNUSED tpp_float TPPCALL
tpp_floatvalue_pack_generic(tpp_intmax whole_intval,
                            tpp_intmax scale,
                            unsigned int radix) {
	tpp_float radix_fltval = (tpp_float)radix;
	tpp_float whole_fltval = (tpp_float)whole_intval;
	for (; scale > 0; --scale)
		whole_fltval *= radix_fltval;
	for (; scale < 0; ++scale)
		whole_fltval /= radix_fltval;
	return whole_fltval;
}

/* Package `self` into an actual, native floating-point value.
 *
 * @param: result: Store the floating-point value here
 * @param: lexer:  Used for warnings when on overflow
 * @return: TPP_EOK:       Success (`*result` was initialized)
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_floatvalue_pack(tpp_floatvalue const *tpp_restrict self,
                    tpp_float *tpp_restrict result,
                    tpp_lexer *tpp_restrict lexer) {
	tpp_float fresult;
	tpp_errno error;
	tpp_intmax whole_intval;
	error = tpp_intvalue_asintmax(&self->tfv_value, &whole_intval);
	if (error == TPP_ENOENT) {
#if TPP_HAVE_TPP_W_INTEGER_OVERFLOW
		error = tpp_lexer_warnf(lexer, TPP_W_INTEGER_OVERFLOW, "<float>", "", "");
#else /* TPP_HAVE_TPP_W_INTEGER_OVERFLOW */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INTEGER_OVERFLOW */
		whole_intval = 0;
	}
	if (TPP_ISERR(error))
		return error;

	switch (self->tfv_radix) {

#if 0
	case 10:
		/* TODO: https://research.swtch.com/fp */
		break;
#endif

	default:
		/* Generic fallback that works for any radix, but is extremely slow & lossy */
		fresult = tpp_floatvalue_pack_generic(whole_intval,
		                                      self->tfv_scale,
		                                      self->tfv_radix);
		break;
	}
	*result = fresult;
	return TPP_EOK;
}
#endif /* TPP_HAVE_FLOATVALUE_PACK */



#if TPP_HAVE_LEXER_DECODEFLOATVALUE
#if TPP_HAVE_TOK_C_FLOAT
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decode_c_float(tpp_lexer *tpp_restrict self,
                         /*out*/ tpp_floatvalue *tpp_restrict result,
                         tpp_char const **p_suffix_start) {
	/*
	 * Expected/supported format:
	 *
	 *         Whole  Fraction    Exponent
	 * >>      [0-9]*(\.[0-9]*)?([eE][+-]?[0-9]+)?.*
	 * >> 0[xX][0-9]*(\.[0-9]*)?([pP][+-]?[0-9]+)?.*
	 *
	 * The following examples need not necessarily be
	 * parseable (even though they match the above regex):
	 * - ``     (yes: entirely empty doesn't need to be parseable here)
	 * - `.`
	 * - `.E1`
	 * - `E1`
	 *
	 * iow: At least 1 of "Whole" or "Fraction" should always be
	 *      present (but don't rely on it; this function is simply
	 *      allowed to emit a warning if neither is present).
	 *      It is up to `tpp_lexer_yieldraw()` to only match
	 *      `TPP_TOK_C_FLOAT` on character-sequences where at
	 *      least one of the 2 is actually present.
	 *
	 * ref: https://en.cppreference.com/cpp/language/floating_literal
	 */
	tpp_errno error;
	tpp_intvalue_builder builder;
	unsigned int num_digits_after_dot = 0;
	tpp_char const *start = tpp_lexer_gettokenstart(self);
	tpp_char const *end = tpp_lexer_gettokenend(self);
	unsigned int radix = 10;

	/* Decode string-to-float */
	if (start < end && *start == '0') {
		/* Check for hex-prefix (other this is just a leading `0` that
		 * doesn't affect the final value and can just be ignored) */
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		if (start < end && (*start == 'x' || *start == 'X')) {
			start = tpp_preparse_skipbse_fwd(self, start + 1, end);
			radix = 16;
		}
	}

	error = tpp_intvalue_builder_init(&builder, radix);
	if (TPP_ISERR(error))
		goto err;

	while (start < end) {
		unsigned int digit;
		tpp_char ch = *start;
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
		error = tpp_intvalue_builder_adddigit(&builder, digit);
		if (TPP_ISERR(error)) {
			if (error == TPP_ENOENT)
				goto handle_invalid_builder;
			goto err_builder;
		}
#ifdef WANT_continue_with_ch
#undef WANT_continue_with_ch
continue_with_ch:
#endif /* WANT_continue_with_ch */
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
	}

	/* Check for the portion after the `.` */
	if (start < end && *start == '.') {
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		while (start < end) {
			unsigned int digit;
			tpp_char ch = *start;
			if (tpp_ascii_isdigit(ch)) {
				digit = (unsigned int)tpp_ascii_asdigit(ch);
			} else if (tpp_ascii_islwrxdigit(ch)) {
				digit = (unsigned int)tpp_ascii_aslwrxdigit(ch);
			} else if (tpp_ascii_isuprxdigit(ch)) {
				digit = (unsigned int)tpp_ascii_asuprxdigit(ch);
			} else
#if TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE
			if (ch == '_') {
				goto continue_with_ch_after_dot;
#define WANT_continue_with_ch_after_dot
			} else
#endif /* TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE */
#if TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK
			if (ch == '\'') {
				goto continue_with_ch_after_dot;
#define WANT_continue_with_ch_after_dot
			} else
#endif /* TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK */
			{
				break;
			}
			if (digit >= radix)
				break;
			++num_digits_after_dot;
			error = tpp_intvalue_builder_adddigit(&builder, digit);
			if (TPP_ISERR(error)) {
				if (error == TPP_ENOENT)
					goto handle_invalid_builder;
				goto err_builder;
			}
#ifdef WANT_continue_with_ch_after_dot
#undef WANT_continue_with_ch_after_dot
continue_with_ch_after_dot:
#endif /* WANT_continue_with_ch_after_dot */
			start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		}
	}

	result->tfv_scale = -(tpp_intmax)num_digits_after_dot;
	result->tfv_radix = radix;
	if (radix == 16) {
		/* Actually use binary as scale for hex numbers */
		result->tfv_scale *= (16 / 2);
		result->tfv_radix = 2;
	}

	/* Check for exponent suffix. */
	if (start < end && (radix < 0xe ? (*start == 'e' || *start == 'E')
	                                : (*start == 'p' || *start == 'P'))) {
		tpp_intmax final_scale, exponent;
		bool exponent_is_negative = false;
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		if (start < end && (*start == '+' || *start == '-')) {
			exponent_is_negative = *start == '-';
			start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		}
		if (start >= end || !tpp_ascii_isdigit(*start)) {
			/* The number-sequence after the exponent is MANDATORY.
			 * As such, if it is missing, we *MUST* emit a warning. */
#if TPP_HAVE_TPP_W_INVALID_FLOAT
			error = tpp_lexer_warnf(self, TPP_W_INVALID_FLOAT);
			if (TPP_ISERR(error))
				goto err_builder;
#endif /* TPP_HAVE_TPP_W_INVALID_FLOAT */
		}

		/* Parse exponent */
		exponent = 0;
		while (start < end) {
			unsigned int digit;
			tpp_char ch = *start;
			if (!tpp_ascii_isdigit(ch))
				break;
			digit = tpp_ascii_asdigit(ch);
#if TPP_HAVE_TPP_W_INVALID_FLOAT
			if ((exponent + digit) < exponent) {
				error = tpp_lexer_warnf(self, TPP_W_INVALID_FLOAT);
				if (TPP_ISERR(error))
					goto err_builder;
			}
#endif /* TPP_HAVE_TPP_W_INVALID_FLOAT */
			exponent += digit;
			start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		}

		/* Add exponent to scale of number. */
		final_scale = exponent_is_negative
		              ? (result->tfv_scale - exponent)
		              : (result->tfv_scale + exponent);
#if TPP_HAVE_TPP_W_INVALID_FLOAT
		if (exponent_is_negative ? (final_scale > result->tfv_scale)
		                         : (final_scale < result->tfv_scale)) {
			/* Warn on overflow */
			error = tpp_lexer_warnf(self, TPP_W_INVALID_FLOAT);
			if (TPP_ISERR(error))
				goto err_builder;
		}
#endif /* TPP_HAVE_TPP_W_INVALID_FLOAT */
		result->tfv_scale = final_scale;
	}

	/* Package up the "whole" part of the number */
	error = tpp_intvalue_builder_pack(&builder, &result->tfv_value);
	if (p_suffix_start)
		*p_suffix_start = start;
	return error;
handle_invalid_builder:
#if TPP_HAVE_TPP_W_INVALID_FLOAT
	error = tpp_lexer_warnf(self, TPP_W_INVALID_FLOAT);
#else /* TPP_HAVE_TPP_W_INVALID_FLOAT */
	error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INVALID_FLOAT */
err_builder:
	tpp_intvalue_builder_fini(&builder);
err:
	return error;
}
#endif /* TPP_HAVE_TOK_C_FLOAT */

/* Decode the current token (which should be `TPP_TOK_ISNUMBER`) into a float.
 * When the current token is `TPP_TOK_ISINT`, it will be parsed as integer
 * first and returned with `tfv_scale = 0` + `tfv_radix = <WEAK_UNDEFINED>`.
 *
 * This is the low-level float parser function. If this is too complicated
 * for you, you may take a look at `tpp_lexer_decodefloat_ex()`, which does
 * the final packing-into-a-float for you (though that one *can* have some
 * loss in precision, whereas this function only loses precision when the
 * limits of `tpp_intvalue` are exceeded)
 *
 * @return: TPP_EOK:       Success (`*result` was initialized)
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloatvalue_ex(tpp_lexer *tpp_restrict self,
                              /*out*/ tpp_floatvalue *tpp_restrict result,
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
		result->tfv_scale = 0;
		result->tfv_radix = 10; /* Doesn't matter */
		return tpp_lexer_decodeint_ex(self, &result->tfv_value, p_suffix_start);
#else /* TPP_HAVE_LEXER_DECODEINT */
		tpp_errno error;
		result->tfv_scale = 0;
		result->tfv_radix = 10; /* Doesn't matter */
		error = tpp_intvalue_init_zero(&result->tfv_value);
		if (TPP_ISERR(error))
			return error;
		if (p_suffix_start)
			*p_suffix_start = tpp_lexer_gettokenend(self);
/*handle_invalid:*/
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
#endif /* TPP_HAVE_LEXER_DECODEFLOATVALUE */

#if TPP_HAVE_LEXER_DECODEFLOAT
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
	tpp_errno error;
	tpp_floatvalue fvalue;

	/* Parse float... */
	error = tpp_lexer_decodefloatvalue_ex(self, &fvalue, p_suffix_start);
	if (TPP_ISERR(error))
		return error;

	/* Pack into native float... */
	error = tpp_floatvalue_pack(&fvalue, result, self);
	tpp_floatvalue_fini(&fvalue);
	return error;
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
