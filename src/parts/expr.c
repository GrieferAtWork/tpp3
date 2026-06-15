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
#ifndef GUARD_TPP_EXPR_C
#define GUARD_TPP_EXPR_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "string.h"
#include "error.h"
#include "expr.h"
#include "lexer.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_BUILTIN_EXPR_VALUE

/* Invoke operators on expression values. */
#if _TPP_EXPR_VALUE_KIND_MULTIPLE
static char const *TPPCALL
tpp_expr_value_kindrepr(_tpp_expr_value_kind kind) {
	switch (kind) {
	case _TPP_EXPR_VALUE_KIND_INT: return "<int>";
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: return "<float>";
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: return "<string>";
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
	default: tpp_unreachable();
	}
}

#if TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS
#define tpp_warn_bad_operands_unary(lexer, op_repr, self) \
	tpp_lexer_warnf(lexer, TPP_W_BAD_EXPRESSION_OPERANDS, \
	                op_repr, tpp_expr_value_kindrepr(_tpp_expr_value_getkind(self)), "", "", "", "")
#define tpp_warn_bad_operands_binary(lexer, lhs, op_repr, rhs)                      \
	tpp_lexer_warnf(lexer, TPP_W_BAD_EXPRESSION_OPERANDS,                           \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(lhs)), op_repr, \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(rhs)), "", "", "")
#define tpp_warn_bad_operands_binary2(lexer, lhs, op_repr1, rhs, op_repr2)           \
	tpp_lexer_warnf(lexer, TPP_W_BAD_EXPRESSION_OPERANDS,                            \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(lhs)), op_repr1, \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(rhs)), op_repr2, "", "")
#define tpp_warn_bad_operands_trinary(lexer, lhs, op_repr1, rhs1, op_repr2, rhs2, op_repr3)         \
	tpp_lexer_warnf(lexer, TPP_W_BAD_EXPRESSION_OPERANDS,                                           \
	                tpp_expr_value_kindrepr(_tpp_expr_value_getkind(lhs)), op_repr1,                \
	                (rhs1) ? tpp_expr_value_kindrepr(_tpp_expr_value_getkind(rhs1)) : "", op_repr2, \
	                (rhs2) ? tpp_expr_value_kindrepr(_tpp_expr_value_getkind(rhs2)) : "", op_repr3)
#else /* TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS */
#define tpp_warn_bad_operands_unary(lexer, op_repr, self)                                   TPP_EOK
#define tpp_warn_bad_operands_binary(lexer, lhs, op_repr, rhs)                              TPP_EOK
#define tpp_warn_bad_operands_binary2(lexer, lhs, op_repr1, rhs, op_repr2)                  TPP_EOK
#define tpp_warn_bad_operands_trinary(lexer, lhs, op_repr1, rhs1, op_repr2, rhs2, op_repr3) TPP_EOK
#endif /* !TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS */


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_expr_value_pos(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict self,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	(void)lexer;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	if (tpp_expr_value_isstring(self)) {
		tpp_errno error = tpp_warn_bad_operands_unary(lexer, "+", self);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
	return tpp_expr_value_copy(result, self);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_expr_value_neg(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict self,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	switch (_tpp_expr_value_getkind(self)) {
	case _TPP_EXPR_VALUE_KIND_INT:
		error = tpp_expr_value_init_int(result, -_tpp_expr_value_getint(self));
		break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		error = tpp_expr_value_init_float(result, -_tpp_expr_value_getfloat(self));
		break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		error = tpp_warn_bad_operands_unary(lexer, "-", self);
		if (TPP_ISERR(error))
			break;
		error = tpp_expr_value_copy(result, self);
		break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
	default: tpp_unreachable();
	}
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_expr_value_inv(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict self,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	switch (_tpp_expr_value_getkind(self)) {
	case _TPP_EXPR_VALUE_KIND_INT:
		error = tpp_expr_value_init_int(result, ~_tpp_expr_value_getint(self));
		break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS || TPP_HAVE_BUILTIN_EXPR_STRINGS
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		error = tpp_warn_bad_operands_unary(lexer, "~", self);
		if (TPP_ISERR(error))
			break;
		error = tpp_expr_value_copy(result, self);
		break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS || TPP_HAVE_BUILTIN_EXPR_STRINGS */
	default: tpp_unreachable();
	}
	return error;
}


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_add(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_int(result, lhs_value + _tpp_expr_value_getint(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, (tpp_float)lhs_value + _tpp_expr_value_getfloat(rhs));
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_float(result, lhs_value + (tpp_float)_tpp_expr_value_getint(rhs));
			break;
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, lhs_value + _tpp_expr_value_getfloat(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		TPP_REF tpp_string *result_string;
		tpp_string const *lhs_value;
		tpp_string const *rhs_value;
		if (!tpp_expr_value_isstring(rhs))
			goto err_bad_types;
#define WANT_err_bad_types
		lhs_value = _tpp_expr_value_getstring(lhs);
		rhs_value = _tpp_expr_value_getstring(rhs);
		result_string = tpp_string_malloc(tpp_string_len(lhs_value) +
		                                  tpp_string_len(rhs_value));
		if tpp_unlikely(!result_string)
			return TPP_ENOMEM;
		tpp_memcpy(tpp_string_str(result_string), tpp_string_str(lhs_value),
		           tpp_string_len(lhs_value) * sizeof(tpp_char));
		tpp_memcpy(tpp_string_str(result_string) + tpp_string_len(lhs_value),
		           tpp_string_str(rhs_value),
		           tpp_string_len(rhs_value) * sizeof(tpp_char));
		error = tpp_expr_value_init_string_inherited(result, result_string);
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " + ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_sub(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_int(result, lhs_value - _tpp_expr_value_getint(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, (tpp_float)lhs_value - _tpp_expr_value_getfloat(rhs));
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_float(result, lhs_value - (tpp_float)_tpp_expr_value_getint(rhs));
			break;
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, lhs_value - _tpp_expr_value_getfloat(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " - ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_mul(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_int(result, lhs_value * _tpp_expr_value_getint(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, (tpp_float)lhs_value * _tpp_expr_value_getfloat(rhs));
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING: {
			tpp_expr_value *temp = rhs;
			rhs = lhs;
			lhs = temp;
			goto handle_multiply_string;
		}
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_float(result, lhs_value * (tpp_float)_tpp_expr_value_getint(rhs));
			break;
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, lhs_value * _tpp_expr_value_getfloat(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		TPP_REF tpp_string *result_string;
		tpp_string *lhs_value;
		tpp_intmax multiplier;
		tpp_size i, lhs_length;
		tpp_size whole_size;
		tpp_char *dst;
		if (!tpp_expr_value_isint(rhs))
			goto err_bad_types;
#define WANT_err_bad_types
handle_multiply_string:
		lhs_value  = _tpp_expr_value_getstring(lhs);
		multiplier = _tpp_expr_value_getint(rhs);
		if (multiplier < 0)
			multiplier = 0; /* ??? */
		if (multiplier > (tpp_intmax)TPP_SIZE_MAX)
			multiplier = (tpp_intmax)TPP_SIZE_MAX;
		if (multiplier == 1)
			return tpp_expr_value_init_string(result, lhs_value);
		if (multiplier == 0) {
			result_string = tpp_string_newempty();
			return tpp_expr_value_init_string_inherited(result, result_string);
		}
		lhs_length = tpp_string_len(lhs_value);
		whole_size = (tpp_size)multiplier * lhs_length;
		if ((((tpp_size)multiplier | lhs_length) &
		     ~(tpp_size)(TPP_SIZE_MAX >> (TPP_CHAR_BIT * (TPP_SIZEOF_tpp_size / 2)))) &&
		    (whole_size / (tpp_size)multiplier != lhs_length))
			whole_size = TPP_SIZE_MAX;
#define TPP_MAX_STRING_LENGTH (((TPP_SIZE_MAX - tpp_offsetof(tpp_string, ts_str)) / sizeof(tpp_char)) - 1)
		if (whole_size > TPP_MAX_STRING_LENGTH)
			whole_size = TPP_MAX_STRING_LENGTH;
#undef TPP_MAX_STRING_LENGTH
		result_string = tpp_string_malloc(whole_size);
		if tpp_unlikely(!result_string)
			return TPP_ENOMEM;
		dst = tpp_string_str(result_string);
		for (i = 0; i < (tpp_size)multiplier; ++i) {
			tpp_memcpy(dst, tpp_string_str(lhs_value),
			           lhs_length * sizeof(tpp_char));
			dst += lhs_length;
		}
		error = tpp_expr_value_init_string_inherited(result, result_string);
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " * ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_div(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			if (rhs_value == 0) {
#if TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
				error = tpp_lexer_warnf(lexer, TPP_W_DIVIDE_BY_ZERO);
				if (TPP_ISERR(error))
					break;
#endif /* TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */
				error = tpp_expr_value_init_int(result, 0);
			} else {
				error = tpp_expr_value_init_int(result, lhs_value / rhs_value);
			}
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, (tpp_float)lhs_value / _tpp_expr_value_getfloat(rhs));
			break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT:
			error = tpp_expr_value_init_float(result, lhs_value / (tpp_float)_tpp_expr_value_getint(rhs));
			break;
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			error = tpp_expr_value_init_float(result, lhs_value / _tpp_expr_value_getfloat(rhs));
			break;
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " / ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_mod(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			if (rhs_value == 0) {
#if TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
				error = tpp_lexer_warnf(lexer, TPP_W_DIVIDE_BY_ZERO);
				if (TPP_ISERR(error))
					break;
#endif /* TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */
				error = tpp_expr_value_init_int(result, 0);
			} else {
				error = tpp_expr_value_init_int(result, lhs_value % rhs_value);
			}
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " % ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_shl(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value << rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " << ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_shr(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value >> rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " >> ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_and(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value & rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " & ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_xor(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value ^ rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " ^ ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_or(struct tpp_lexer *tpp_restrict lexer,
                  /*in*/ tpp_expr_value *tpp_restrict lhs,
                  /*in*/ tpp_expr_value *tpp_restrict rhs,
                  /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			error = tpp_expr_value_init_int(result, lhs_value | rhs_value);
		}	break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING:
		goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	return error;
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
err_bad_types:
	error = tpp_warn_bad_operands_binary(lexer, lhs, " | ", rhs);
	if (!TPP_ISERR(error))
		error = tpp_expr_value_copy(result, lhs);
	return error;
#endif /* WANT_err_bad_types */
}

static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_impl(struct tpp_lexer *tpp_restrict lexer,
                        /*in*/ tpp_expr_value *tpp_restrict lhs,
                        /*in*/ tpp_expr_value *tpp_restrict rhs,
                        /*out*/ int *tpp_restrict p_delta,
                        char const *op_repr) {
	(void)lexer;
	switch (_tpp_expr_value_getkind(lhs)) {

	case _TPP_EXPR_VALUE_KIND_INT: {
		tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			if (lhs_value < rhs_value) {
				*p_delta = -1;
			} else if (lhs_value > rhs_value) {
				*p_delta = 1;
			} else {
				*p_delta = 0;
			}
			return TPP_EOK;
		}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		case _TPP_EXPR_VALUE_KIND_FLOAT: {
			tpp_float rhs_value = _tpp_expr_value_getfloat(rhs);
			if (lhs_value < rhs_value) {
				*p_delta = -1;
			} else if (lhs_value > rhs_value) {
				*p_delta = 1;
			} else {
				*p_delta = 0;
			}
			return TPP_EOK;
		}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

		default: tpp_unreachable();
		}
	}	break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float lhs_value = _tpp_expr_value_getfloat(lhs);
		switch (_tpp_expr_value_getkind(rhs)) {
		case _TPP_EXPR_VALUE_KIND_INT: {
			tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
			if (lhs_value < rhs_value) {
				*p_delta = -1;
			} else if (lhs_value > rhs_value) {
				*p_delta = 1;
			} else {
				*p_delta = 0;
			}
			return TPP_EOK;
		}	break;

		case _TPP_EXPR_VALUE_KIND_FLOAT: {
			tpp_float rhs_value = _tpp_expr_value_getfloat(rhs);
			if (lhs_value < rhs_value) {
				*p_delta = -1;
			} else if (lhs_value > rhs_value) {
				*p_delta = 1;
			} else {
				*p_delta = 0;
			}
			return TPP_EOK;
		}	break;

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		case _TPP_EXPR_VALUE_KIND_STRING:
			goto err_bad_types;
#define WANT_err_bad_types
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

		default: tpp_unreachable();
		}
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		tpp_string const *lhs_value;
		tpp_string const *rhs_value;
		tpp_size common_length;
		if (_tpp_expr_value_getkind(rhs) != _TPP_EXPR_VALUE_KIND_STRING)
			goto err_bad_types;
#define WANT_err_bad_types
		lhs_value = _tpp_expr_value_getstring(lhs);
		rhs_value = _tpp_expr_value_getstring(rhs);
		common_length = tpp_string_len(lhs_value);
		if (common_length > tpp_string_len(rhs_value))
			common_length = tpp_string_len(rhs_value);
		*p_delta = tpp_memcmp(tpp_string_str(lhs_value),
		                      tpp_string_str(rhs_value),
		                      common_length * sizeof(tpp_char));
		if (*p_delta == 0) {
			if (tpp_string_len(lhs_value) < tpp_string_len(rhs_value)) {
				*p_delta = -1;
			} else if (tpp_string_len(lhs_value) > tpp_string_len(rhs_value)) {
				*p_delta = 1;
			}
		}
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
	tpp_unreachable();
#ifdef WANT_err_bad_types
#undef WANT_err_bad_types
	{
		tpp_errno error;
err_bad_types:
		error = tpp_warn_bad_operands_binary(lexer, lhs, op_repr, rhs);
		if (!TPP_ISERR(error))
			*p_delta = 0;
		return error;
	}
#endif /* WANT_err_bad_types */
}


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_eq(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " == ");
	*p_bool_result = delta == 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_ne(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " != ");
	*p_bool_result = delta != 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_lo(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " < ");
	*p_bool_result = delta < 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_le(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " <= ");
	*p_bool_result = delta <= 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_gr(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " > ");
	*p_bool_result = delta > 0;
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_cmp_ge(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict lhs,
                      /*in*/ tpp_expr_value *tpp_restrict rhs,
                      /*out*/ bool *tpp_restrict p_bool_result) {
	int delta;
	tpp_errno error = tpp_expr_value_cmp_impl(lexer, lhs, rhs, &delta, " >= ");
	*p_bool_result = delta >= 0;
	return error;
}


#if TPP_HAVE_BUILTIN_EXPR_STRINGS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_getindex(struct tpp_lexer *tpp_restrict lexer,
                        /*in*/ tpp_expr_value *tpp_restrict lhs,
                        /*in*/ tpp_expr_value *tpp_restrict index,
                        /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_string const *lhs_value;
	tpp_intmax index_value;
	tpp_char result_value;
	if (!tpp_expr_value_isstring(lhs) || !tpp_expr_value_isint(index)) {
		tpp_errno error = tpp_warn_bad_operands_binary2(lexer, lhs, "[", index, "]");
		if (TPP_ISERR(error))
			return error;
		return tpp_expr_value_copy(result, lhs);
	}
	lhs_value   = _tpp_expr_value_getstring(lhs);
	index_value = _tpp_expr_value_getint(index);
	if (index_value < 0 || index_value > (tpp_intmax)tpp_string_len(lhs_value))
		index_value = tpp_string_len(lhs_value); /* XXX: TPP_W_INDEX_OUT_OF_BOUNDS */
	result_value = tpp_string_str(lhs_value)[(tpp_size)index_value];
	/* XXX: If hosting compiler has "-fsigned-char", must:
	 *      >> result_value = (tpp_intmax)(signed char)result_value;
	 * iow: values [128,255] must become negative [-128,-1] */
	return tpp_expr_value_init_int(result, result_value);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 5)) tpp_errno TPPCALL
tpp_expr_value_getrange(struct tpp_lexer *tpp_restrict lexer,
                        /*in*/ tpp_expr_value *tpp_restrict lhs,
                        /*in*/ tpp_expr_value *lo,
                        /*in*/ tpp_expr_value *hi,
                        /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_string const *lhs_value;
	tpp_string *result_string;
	tpp_intmax lo_value, hi_value;
	tpp_size lhs_size, result_size;
	if (!tpp_expr_value_isstring(lhs) ||
	    (lo && !tpp_expr_value_isint(lo)) ||
	    (hi && !tpp_expr_value_isint(hi))) {
		tpp_errno error = tpp_warn_bad_operands_trinary(lexer, lhs, "[", lo, ":", hi, "]");
		if (TPP_ISERR(error))
			return error;
		return tpp_expr_value_copy(result, lhs);
	}
	lhs_value = _tpp_expr_value_getstring(lhs);
	lhs_size  = tpp_string_len(lhs_value);
	lo_value  = lo ? _tpp_expr_value_getint(lo) : (tpp_intmax)0;
	hi_value  = hi ? _tpp_expr_value_getint(hi) : (tpp_intmax)lhs_size;
	if (lo_value >= (tpp_intmax)lhs_size || !lhs_size)
		goto return_empty_string;
	if (lo_value < 0) {
		lo_value += (tpp_intmax)lhs_size;
		if (lo_value < 0)
			lo_value = (tpp_intmax)lhs_size - ((-lo_value) % (tpp_intmax)lhs_size);
	}
	tpp_assert((tpp_size)lo_value <= lhs_size);
	if (hi_value < 0) {
		hi_value += lhs_size;
		if (hi_value < 0)
			hi_value = (tpp_intmax)lhs_size - ((-hi_value) % (tpp_intmax)lhs_size);
	} else if (hi_value > (tpp_intmax)lhs_size) {
		hi_value = (tpp_intmax)lhs_size;
	}
	tpp_assert((tpp_size)hi_value <= lhs_size);
	if (hi_value < lo_value)
		hi_value = lo_value;
	if ((tpp_size)hi_value >= (tpp_size)lo_value)
		goto return_empty_string;
	result_size   = (tpp_size)hi_value - (tpp_size)lo_value;
	result_string = tpp_string_malloc(result_size);
	if tpp_unlikely(!result_string)
		return TPP_ENOMEM;
	tpp_memcpy(tpp_string_str(result_string),
	           tpp_string_str(lhs_value) + (tpp_size)lo_value,
	           result_size * sizeof(tpp_char));
	return tpp_expr_value_init_string_inherited(result, result_string);
return_empty_string:
	result_string = tpp_string_newempty();
	return tpp_expr_value_init_string_inherited(result, result_string);
}
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */


/* Determine the boolean-style value of "self"
 * Works for any kind of expression value.
 * @return: TPP_EOK: Success */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_expr_value_asbool(struct tpp_lexer *tpp_restrict lexer,
                      /*in*/ tpp_expr_value *tpp_restrict self,
                      bool *tpp_restrict p_bool_result) {
	(void)lexer;
	switch (_tpp_expr_value_getkind(self)) {
	case _TPP_EXPR_VALUE_KIND_INT:
		*p_bool_result = _tpp_expr_value_getint(self) != 0;
		break;
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT:
		*p_bool_result = _tpp_expr_value_getfloat(self) != 0.0;
		break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		tpp_string const *str = _tpp_expr_value_getstring(self);
		*p_bool_result = tpp_string_len(str) != 0;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
	default: tpp_unreachable();
	}
	return TPP_EOK;
}

#else /* _TPP_EXPR_VALUE_KIND_MULTIPLE */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_div(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
	tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
	if (rhs_value == 0) {
#if TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
		error = tpp_lexer_warnf(lexer, TPP_W_DIVIDE_BY_ZERO);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */
		error = tpp_expr_value_init_int(result, 0);
	} else {
		error = tpp_expr_value_init_int(result, lhs_value / rhs_value);
	}
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_expr_value_mod(struct tpp_lexer *tpp_restrict lexer,
                   /*in*/ tpp_expr_value *tpp_restrict lhs,
                   /*in*/ tpp_expr_value *tpp_restrict rhs,
                   /*out*/ tpp_expr_value *tpp_restrict result) {
	tpp_errno error;
	tpp_intmax lhs_value = _tpp_expr_value_getint(lhs);
	tpp_intmax rhs_value = _tpp_expr_value_getint(rhs);
	if (rhs_value == 0) {
#if TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
		error = tpp_lexer_warnf(lexer, TPP_W_DIVIDE_BY_ZERO);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */
		error = tpp_expr_value_init_int(result, 0);
	} else {
		error = tpp_expr_value_init_int(result, lhs_value % rhs_value);
	}
	return error;
}
#endif /* !_TPP_EXPR_VALUE_KIND_MULTIPLE */


/* Print the representation of "self" to "printer" (used to implement __TPP_EVAL)
 * @return: * : Sum of positive return value of `printer'
 * @return: (tpp_ssize)TPP_ISERR(*): An error was thrown, or `printer' returned this value */
#if TPP_HAVE_EXPR_VALUE_PRINTREPR
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_expr_value_printrepr(tpp_expr_value *tpp_restrict self,
                         tpp_formatprinter printer, void *arg) {
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	char value_buffer[TPP_ITOA_MAXLEN < TPP_FTOA_MAXLEN ? TPP_FTOA_MAXLEN : TPP_ITOA_MAXLEN];
#else /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
	char value_buffer[TPP_ITOA_MAXLEN];
#endif /* !TPP_HAVE_BUILTIN_EXPR_FLOATS */
	char *value_ptr;
#if _TPP_EXPR_VALUE_KIND_MULTIPLE
	switch (_tpp_expr_value_getkind(self)) {

	case _TPP_EXPR_VALUE_KIND_INT:
		break;

#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	case _TPP_EXPR_VALUE_KIND_FLOAT: {
		tpp_float value = _tpp_expr_value_getfloat(self);
		tpp_size value_len = tpp_ftoa(value_buffer, value);
		return (*printer)(arg, (tpp_char const *)value_buffer, value_len);
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	case _TPP_EXPR_VALUE_KIND_STRING: {
		tpp_ssize temp, result;
		tpp_string const *const str = _tpp_expr_value_getstring(self);
		result = (*printer)(arg, (tpp_char const *)"\"", 1);
		if tpp_unlikely(result < 0)
			return result;
		temp = tpp_token_encodestring(printer, arg, tpp_string_str(str),
		                              tpp_string_len(str) * sizeof(tpp_char));
		if tpp_unlikely(temp < 0)
			return temp;
		result += temp;
		temp = (*printer)(arg, (tpp_char const *)"\"", 1);
		if tpp_unlikely(temp < 0)
			return temp;
		result += temp;
		return result;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

	default: tpp_unreachable();
	}
#endif /* _TPP_EXPR_VALUE_KIND_MULTIPLE */
	value_ptr = tpp_itoa(value_buffer, _tpp_expr_value_getint(self));
	return (*printer)(arg, (tpp_char const *)value_ptr,
	                  (tpp_size)((value_buffer + TPP_ITOA_MAXLEN) - value_ptr));
}
#endif /* TPP_HAVE_EXPR_VALUE_PRINTREPR */

#endif /* TPP_HAVE_BUILTIN_EXPR_VALUE */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_EXPR_C */
