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
#ifndef GUARD_TPP_EXPR_H
#define GUARD_TPP_EXPR_H 1

#include "api.h"

#include "config.h"
#include "string.h"
#include "error.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_BUILTIN_EXPR_VALUE
#undef _TPP_EXPR_VALUE_KIND_MULTIPLE
typedef enum _tpp_expr_value_kind {
	_TPP_EXPR_VALUE_KIND_INT,
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
	_TPP_EXPR_VALUE_KIND_FLOAT,
#define _TPP_EXPR_VALUE_KIND_MULTIPLE 1
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
	_TPP_EXPR_VALUE_KIND_STRING,
#define _TPP_EXPR_VALUE_KIND_MULTIPLE 1
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
} _tpp_expr_value_kind;

#ifndef _TPP_EXPR_VALUE_KIND_MULTIPLE
#define _TPP_EXPR_VALUE_KIND_MULTIPLE 0
#endif /* !_TPP_EXPR_VALUE_KIND_MULTIPLE */

#define tpp_expr_value tpp_expr_value
typedef struct tpp_expr_value {
#if _TPP_EXPR_VALUE_KIND_MULTIPLE
	_tpp_expr_value_kind TPP_INTERNAL(xv_kind); /* Expression value kind */
#define _tpp_expr_value_getkind(self)    (self)->TPP_INTERNAL(xv_kind)
#define _tpp_expr_value_setkind(self, v) , (self)->TPP_INTERNAL(xv_kind) = (v)
#else /* _TPP_EXPR_VALUE_KIND_MULTIPLE */
#define _tpp_expr_value_getkind(self)    _TPP_EXPR_VALUE_KIND_INT
#define _tpp_expr_value_setkind(self, v) /* nothing */
#endif /* !_TPP_EXPR_VALUE_KIND_MULTIPLE */

	/* Expression value union */
	union {
		tpp_intmax          TPP_INTERNAL(xd_int);    /* [valid_if(_tpp_expr_value_getkind(self) == _TPP_EXPR_VALUE_KIND_INT)] */
#define _tpp_expr_value_getint(self) (self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_int)
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
		tpp_float           TPP_INTERNAL(xd_float);  /* [valid_if(_tpp_expr_value_getkind(self) == _TPP_EXPR_VALUE_KIND_FLOAT)] */
#define _tpp_expr_value_getfloat(self) (self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_float)
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		TPP_REF tpp_string *TPP_INTERNAL(xd_string); /* [1..1][valid_if(_tpp_expr_value_getkind(self) == _TPP_EXPR_VALUE_KIND_STRING)] */
#define _tpp_expr_value_getstring(self) (self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_string)
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
	} TPP_INTERNAL(xv_data);
} tpp_expr_value;


#if TPP_HAVE_BUILTIN_EXPR_STRINGS
#define _tpp_expr_value_incref(self)                                   \
	, (_tpp_expr_value_getkind(self) != _TPP_EXPR_VALUE_KIND_STRING || \
	   (tpp_string_incref((self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_string)), 0))
#define _tpp_expr_value_decref(self)                                   \
	, (_tpp_expr_value_getkind(self) != _TPP_EXPR_VALUE_KIND_STRING || \
	   (tpp_string_decref((self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_string)), 0))
#else /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
#define _tpp_expr_value_incref(self) /* noting */
#define _tpp_expr_value_decref(self) /* noting */
#endif /* !TPP_HAVE_BUILTIN_EXPR_STRINGS */


/* Move `src` into `dst`, invalidating `src` along the way and initializing `dst` */
#define tpp_expr_value_move(dst, src) (void)(*(dst) = *(src))

/* Copy-construct `src` into `dst`
 * @return: TPP_EOK: Success */
#define tpp_expr_value_copy(dst, src) \
	(*(dst) = *(src)_tpp_expr_value_incref(dst), TPP_EOK)

/* Finalize `self` (never fails) */
#define tpp_expr_value_fini(self) \
	((void)0 _tpp_expr_value_decref(self), tpp_dbg_memset(self, sizeof(*(self))))

/* Check which native representation is used by `self` (never fails) */
#if _TPP_EXPR_VALUE_KIND_MULTIPLE
#define tpp_expr_value_isint(self) (_tpp_expr_value_getkind(self) == _TPP_EXPR_VALUE_KIND_INT)
#else /* _TPP_EXPR_VALUE_KIND_MULTIPLE */
#define tpp_expr_value_isint(self) 1
#endif /* !_TPP_EXPR_VALUE_KIND_MULTIPLE */
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
#define tpp_expr_value_isfloat(self) (_tpp_expr_value_getkind(self) == _TPP_EXPR_VALUE_KIND_FLOAT)
#else /* TPP_HAVE_BUILTIN_EXPR_FLOATS */
#define tpp_expr_value_isfloat(self) 0
#endif /* !TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
#define tpp_expr_value_isstring(self) (_tpp_expr_value_getkind(self) == _TPP_EXPR_VALUE_KIND_STRING)
#else /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
#define tpp_expr_value_isstring(self) 0
#endif /* !TPP_HAVE_BUILTIN_EXPR_STRINGS */

/* Extract a specifically-typed value from `self`
 * Caller must ensure that `tpp_expr_value_is*` returned true.
 * @return: TPP_EOK: Success */
#define tpp_expr_value_asint(self, p_result) (*(p_result) = _tpp_expr_value_getint(self), TPP_EOK)
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
#define tpp_expr_value_asfloat(self, p_result) (*(p_result) = _tpp_expr_value_getfloat(self), TPP_EOK)
#endif /* !TPP_HAVE_BUILTIN_EXPR_FLOATS */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
#define tpp_expr_value_asstringref(self, p_result)  \
	(*(p_result) = _tpp_expr_value_getstring(self), \
	 tpp_string_incref(*(p_result)), TPP_EOK)
#endif /* !TPP_HAVE_BUILTIN_EXPR_STRINGS */


/* Initialize `self` as int-typed, with `v` as value
 * @return: TPP_EOK: Success */
#define tpp_expr_value_init_int(self, /*tpp_intmax*/ v)       \
	((self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_int) = (v) \
	 _tpp_expr_value_setkind(self, _TPP_EXPR_VALUE_KIND_INT), \
	 TPP_EOK)

/* Initialize `self` as float-typed, with `v` as value
 * @return: TPP_EOK: Success */
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
#define tpp_expr_value_init_float(self, /*tpp_float*/ v)        \
	((self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_float) = (v) \
	 _tpp_expr_value_setkind(self, _TPP_EXPR_VALUE_KIND_FLOAT), \
	 TPP_EOK)
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

/* Initialize `self` as string-typed, with `v` as value
 * @param: str: [1..1] The string value to assign
 * @return: TPP_EOK: Success */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
#define tpp_expr_value_init_string(self, /*tpp_string **/ str)                 \
	((self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_string) = (str)             \
	 _tpp_expr_value_setkind(self, _TPP_EXPR_VALUE_KIND_STRING),               \
	 tpp_string_incref((self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_string)), \
	 TPP_EOK)
#define tpp_expr_value_init_string_inherited(self, /*inherit(always)*/ /*TPP_REF tpp_string **/ str) \
	((self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_string) = (str)                                   \
	 _tpp_expr_value_setkind(self, _TPP_EXPR_VALUE_KIND_STRING),                                     \
	 TPP_EOK)
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

struct tpp_lexer;

/* Invoke operators on expression values. */
#if _TPP_EXPR_VALUE_KIND_MULTIPLE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL tpp_expr_value_pos(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict self, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL tpp_expr_value_neg(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict self, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL tpp_expr_value_inv(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict self, /*out*/ tpp_expr_value *tpp_restrict result);
#define tpp_expr_value_pos tpp_expr_value_pos
#define tpp_expr_value_neg tpp_expr_value_neg
#define tpp_expr_value_inv tpp_expr_value_inv

TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_add(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_sub(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_mul(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_shl(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_shr(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_and(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_xor(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_or(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);
#define tpp_expr_value_add tpp_expr_value_add
#define tpp_expr_value_sub tpp_expr_value_sub
#define tpp_expr_value_mul tpp_expr_value_mul
#define tpp_expr_value_shl tpp_expr_value_shl
#define tpp_expr_value_shr tpp_expr_value_shr
#define tpp_expr_value_and tpp_expr_value_and
#define tpp_expr_value_xor tpp_expr_value_xor
#define tpp_expr_value_or  tpp_expr_value_or

TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_cmp_eq(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ bool *tpp_restrict p_bool_result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_cmp_ne(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ bool *tpp_restrict p_bool_result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_cmp_lo(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ bool *tpp_restrict p_bool_result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_cmp_le(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ bool *tpp_restrict p_bool_result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_cmp_gr(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ bool *tpp_restrict p_bool_result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_cmp_ge(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ bool *tpp_restrict p_bool_result);
#define tpp_expr_value_cmp_eq tpp_expr_value_cmp_eq
#define tpp_expr_value_cmp_ne tpp_expr_value_cmp_ne
#define tpp_expr_value_cmp_lo tpp_expr_value_cmp_lo
#define tpp_expr_value_cmp_le tpp_expr_value_cmp_le
#define tpp_expr_value_cmp_gr tpp_expr_value_cmp_gr
#define tpp_expr_value_cmp_ge tpp_expr_value_cmp_ge

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL tpp_expr_value_lengthof(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict self, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_getindex(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict index, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 5)) tpp_errno TPPCALL tpp_expr_value_getrange(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *lo, /*in*/ tpp_expr_value *hi, /*out*/ tpp_expr_value *tpp_restrict result);
#define tpp_expr_value_lengthof tpp_expr_value_lengthof
#define tpp_expr_value_getindex tpp_expr_value_getindex
#define tpp_expr_value_getrange tpp_expr_value_getrange
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

/* Determine the boolean-style value of `self`
 * Works for any kind of expression value.
 * @return: TPP_EOK: Success */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL tpp_expr_value_asbool(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict self, bool *tpp_restrict p_bool_result);
#define tpp_expr_value_asbool tpp_expr_value_asbool

#else /* _TPP_EXPR_VALUE_KIND_MULTIPLE */
#define tpp_expr_value_pos(lexer, self, result)               tpp_expr_value_init_int(result, +_tpp_expr_value_getint(self))
#define tpp_expr_value_neg(lexer, self, result)               tpp_expr_value_init_int(result, -_tpp_expr_value_getint(self))
#define tpp_expr_value_inv(lexer, self, result)               tpp_expr_value_init_int(result, ~_tpp_expr_value_getint(self))
#define tpp_expr_value_add(lexer, lhs, rhs, result)           tpp_expr_value_init_int(result, _tpp_expr_value_getint(lhs) + _tpp_expr_value_getint(rhs))
#define tpp_expr_value_sub(lexer, lhs, rhs, result)           tpp_expr_value_init_int(result, _tpp_expr_value_getint(lhs) - _tpp_expr_value_getint(rhs))
#define tpp_expr_value_mul(lexer, lhs, rhs, result)           tpp_expr_value_init_int(result, _tpp_expr_value_getint(lhs) * _tpp_expr_value_getint(rhs))
#define tpp_expr_value_shl(lexer, lhs, rhs, result)           tpp_expr_value_init_int(result, _tpp_expr_value_getint(lhs) << _tpp_expr_value_getint(rhs))
#define tpp_expr_value_shr(lexer, lhs, rhs, result)           tpp_expr_value_init_int(result, _tpp_expr_value_getint(lhs) >> _tpp_expr_value_getint(rhs))
#define tpp_expr_value_and(lexer, lhs, rhs, result)           tpp_expr_value_init_int(result, _tpp_expr_value_getint(lhs) & _tpp_expr_value_getint(rhs))
#define tpp_expr_value_xor(lexer, lhs, rhs, result)           tpp_expr_value_init_int(result, _tpp_expr_value_getint(lhs) | _tpp_expr_value_getint(rhs))
#define tpp_expr_value_or(lexer, lhs, rhs, result)            tpp_expr_value_init_int(result, _tpp_expr_value_getint(lhs) ^ _tpp_expr_value_getint(rhs))
#define tpp_expr_value_cmp_eq(lexer, lhs, rhs, p_bool_result) (*(p_bool_result) = (_tpp_expr_value_getint(lhs) == _tpp_expr_value_getint(rhs)), TPP_EOK)
#define tpp_expr_value_cmp_ne(lexer, lhs, rhs, p_bool_result) (*(p_bool_result) = (_tpp_expr_value_getint(lhs) != _tpp_expr_value_getint(rhs)), TPP_EOK)
#define tpp_expr_value_cmp_lo(lexer, lhs, rhs, p_bool_result) (*(p_bool_result) = (_tpp_expr_value_getint(lhs) < _tpp_expr_value_getint(rhs)), TPP_EOK)
#define tpp_expr_value_cmp_le(lexer, lhs, rhs, p_bool_result) (*(p_bool_result) = (_tpp_expr_value_getint(lhs) <= _tpp_expr_value_getint(rhs)), TPP_EOK)
#define tpp_expr_value_cmp_gr(lexer, lhs, rhs, p_bool_result) (*(p_bool_result) = (_tpp_expr_value_getint(lhs) > _tpp_expr_value_getint(rhs)), TPP_EOK)
#define tpp_expr_value_cmp_ge(lexer, lhs, rhs, p_bool_result) (*(p_bool_result) = (_tpp_expr_value_getint(lhs) >= _tpp_expr_value_getint(rhs)), TPP_EOK)

/* Determine the boolean-style value of `self`
 * Works for any kind of expression value.
 * @return: TPP_EOK: Success */
#define tpp_expr_value_asbool(lexer, self, p_bool_result) \
	(*(p_bool_result) = (_tpp_expr_value_getint(self) != 0), TPP_EOK)
#endif /* !_TPP_EXPR_VALUE_KIND_MULTIPLE */

#define tpp_expr_value_div tpp_expr_value_div
#define tpp_expr_value_mod tpp_expr_value_mod
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_div(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL tpp_expr_value_mod(struct tpp_lexer *tpp_restrict lexer, /*in*/ tpp_expr_value *tpp_restrict lhs, /*in*/ tpp_expr_value *tpp_restrict rhs, /*out*/ tpp_expr_value *tpp_restrict result);

/* Print the representation of `self` to `printer` (in target encoding; used to implement `__TPP_EVAL`)
 * @return: *  : Sum of positive return value of `printer`
 * @return: < 0: An error was thrown (`TPP_SSIZE_ISERR`), or `printer` returned this value */
#if TPP_HAVE_EXPR_VALUE_PRINTREPR
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_expr_value_printrepr(struct tpp_lexer *tpp_restrict lexer,
                         tpp_expr_value *tpp_restrict self,
                         tpp_formatprinter printer, void *arg);
#endif /* TPP_HAVE_EXPR_VALUE_PRINTREPR */
#endif /* TPP_HAVE_BUILTIN_EXPR_VALUE */

/* Initialize to int=0 */
#ifndef tpp_expr_value_init_zero
#ifdef tpp_expr_value_init_bool
#define tpp_expr_value_init_zero(self) tpp_expr_value_init_bool(self, 0)
#else /* tpp_expr_value_init_bool */
#define tpp_expr_value_init_zero(self) tpp_expr_value_init_int(self, 0)
#endif /* !tpp_expr_value_init_bool */
#endif /* !tpp_expr_value_init_zero */

/* Initialize to int=1 */
#ifndef tpp_expr_value_init_one
#ifdef tpp_expr_value_init_bool
#define tpp_expr_value_init_one(self) tpp_expr_value_init_bool(self, 1)
#else /* tpp_expr_value_init_bool */
#define tpp_expr_value_init_one(self) tpp_expr_value_init_int(self, 1)
#endif /* !tpp_expr_value_init_bool */
#endif /* !tpp_expr_value_init_one */

/* Initialize to int=0/1 */
#ifndef tpp_expr_value_init_bool
#define tpp_expr_value_init_bool(self, v) tpp_expr_value_init_int(self, (v) ? 1 : 0)
#endif /* !tpp_expr_value_init_bool */

#ifndef tpp_expr_value_pos
#define tpp_expr_value_pos(lexer, self, result) tpp_expr_value_copy(result, self)
#endif /* !tpp_expr_value_pos */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_EXPR_H */
