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

#ifndef tpp_expr_intvalue
typedef struct tpp_expr_intvalue {
	tpp_intmax TPP_INTERNAL(teiv_value); /* Integer value */
} tpp_expr_intvalue;

/* Finalize `self` */
#define tpp_expr_intvalue_fini(self) (void)0

/* @return: TPP_EOK:      Success
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_init_zero(self) \
	((self)->TPP_INTERNAL(teiv_value) = 0, TPP_EOK)

/* @return: TPP_EOK:      Success
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_init_copy(self, from) \
	((self)->TPP_INTERNAL(teiv_value) =         \
	 (from)->TPP_INTERNAL(teiv_value),          \
	 TPP_EOK)

/* @return: TPP_EOK:      Success
 * #if TPP_EXPR_INTVALUE_ASINTMAX_CANOVERFLOW
 * @return: TPP_ENOENT:   SOFT_ERROR: Out-of-range
 * #endif // TPP_EXPR_INTVALUE_ASINTMAX_CANOVERFLOW
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_asintmax(self, p_result) \
	(*(p_result) = (self)->TPP_INTERNAL(teiv_value), TPP_EOK)

/* @return: TPP_EOK:      Yes
 * @return: TPP_ENOENT:   No
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_iszero(self) \
	(((self)->TPP_INTERNAL(teiv_value) == 0) ? TPP_EOK : TPP_ENOENT)

/* @return: TPP_EOK:      Yes
 * @return: TPP_ENOENT:   No
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_isneg(self) \
	(((self)->TPP_INTERNAL(teiv_value) < 0) ? TPP_EOK : TPP_ENOENT)

#if TPP_HAVE_LEXER_DECODEINT
/* >> [self] = ([self] * mul) + add;
 * Used to implement `tpp_lexer_decodeint()`
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Overflow
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_muladd tpp_expr_intvalue_muladd
#if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_expr_intvalue_muladd(tpp_expr_intvalue *tpp_restrict self,
                         unsigned int mul, unsigned int add) {
	tpp_intmax const oldval = self->TPP_INTERNAL(teiv_value);
	tpp_intmax const newval1 = oldval * mul;
	tpp_intmax const newval2 = newval1 + add;
	self->TPP_INTERNAL(teiv_value) = newval2;
	if tpp_unlikely(newval1 < oldval || newval2 < newval1)
		return TPP_ENOENT;
	return TPP_EOK;
}
#else /* TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#define tpp_expr_intvalue_muladd(self, mul, add) \
	((self)->TPP_INTERNAL(teiv_value) *= (mul),  \
	 (self)->TPP_INTERNAL(teiv_value) += (add),  \
	 TPP_EOK)
#endif /* !TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#endif /* TPP_HAVE_LEXER_DECODEINT */

#undef _TPP_INTMAX_MIN
#define _TPP_INTMAX_MIN ((tpp_intmax)((TPP_UINTMAX_MAX >> 1) + 1))
#undef _TPP_INTMAX_MAX
#define _TPP_INTMAX_MAX ((tpp_intmax)(TPP_UINTMAX_MAX >> 1))

#if TPP_HAVE_LEXER_PARSECHARACTER_EXPR
/* >> [self] = v;
 * @return: TPP_EOK:      OK
 * #if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ENOENT:   Overflow
 * #endif // TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ISERR(*): HARD_ERROR */
#if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
#define tpp_expr_intvalue_init_uintmax(self, v)          \
	((self)->TPP_INTERNAL(teiv_value) = (tpp_intmax)(v), \
	 ((tpp_uintmax)(v) <= (tpp_uintmax)_TPP_INTMAX_MAX)  \
	 ? TPP_EOK                                           \
	 : TPP_ENOENT)
#else /* TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#define tpp_expr_intvalue_init_uintmax(self, v)          \
	((self)->TPP_INTERNAL(teiv_value) = (tpp_intmax)(v), \
	 TPP_EOK)
#endif /* !TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#endif /* TPP_HAVE_LEXER_PARSECHARACTER_EXPR */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
/* >> [self] = v;
 * @return: TPP_EOK:      OK
 * #if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ENOENT:   Overflow
 * #endif // TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ISERR(*): HARD_ERROR */
#if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW && TPP_SIZE_MAX >= TPP_UINTMAX_MAX
#define tpp_expr_intvalue_init_size(self, /*tpp_size*/ v)           \
	((self)->TPP_INTERNAL(teiv_value) = (tpp_intmax)(tpp_ssize)(v), \
	 ((tpp_size)(v) <= (tpp_uintmax)_TPP_INTMAX_MAX)                \
	 ? TPP_EOK                                                      \
	 : TPP_ENOENT)
#else /* TPP_EXPR_INTVALUE_MATH_CANOVERFLOW && TPP_SIZE_MAX >= TPP_UINTMAX_MAX */
#define tpp_expr_intvalue_init_size(self, /*tpp_size*/ v) \
	((self)->TPP_INTERNAL(teiv_value) = (tpp_intmax)(tpp_ssize)(v), TPP_EOK)
#endif /* !TPP_EXPR_INTVALUE_MATH_CANOVERFLOW || TPP_SIZE_MAX < TPP_UINTMAX_MAX */
#define tpp_expr_intvalue_init_char(self, /*tpp_char*/ v) \
	((self)->TPP_INTERNAL(teiv_value) = (tpp_intmax)(tpp_char)(v), TPP_EOK)
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

#if TPP_HAVE_BUILTIN_EXPR_VALUE
#define tpp_expr_intvalue_init_one(self) \
	((self)->TPP_INTERNAL(teiv_value) = 1, TPP_EOK)
#define tpp_expr_intvalue_init_bool(self, v) \
	((self)->TPP_INTERNAL(teiv_value) = (v), TPP_EOK)

/* >> [p_result] = -[self];
 * @return: TPP_EOK:      OK
 * #if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ENOENT:   Overflow
 * #endif // TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ISERR(*): HARD_ERROR */
#if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
#define tpp_expr_intvalue_neg(self, p_result)                                  \
	((p_result)->TPP_INTERNAL(teiv_value) = -(self)->TPP_INTERNAL(teiv_value), \
	 (((p_result)->TPP_INTERNAL(teiv_value) < 0) ==                            \
	  ((self)->TPP_INTERNAL(teiv_value) > 0))                                  \
	 ? TPP_EOK                                                                 \
	 : TPP_ENOENT)
#else /* TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#define tpp_expr_intvalue_neg(self, p_result) \
	((p_result)->TPP_INTERNAL(teiv_value) = -(self)->TPP_INTERNAL(teiv_value), TPP_EOK)
#endif /* !TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */

/* >> [p_result] = ~[self];
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_inv(self, p_result)                                  \
	((p_result)->TPP_INTERNAL(teiv_value) = ~(self)->TPP_INTERNAL(teiv_value), \
	 TPP_EOK)

/* >> [p_result] = [lhs] + [rhs];
 * @return: TPP_EOK:      OK
 * #if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ENOENT:   Overflow
 * #endif // TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ISERR(*): HARD_ERROR */
#if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
#define tpp_expr_intvalue_add(lhs, rhs, p_result)                                                                                       \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) +                                                          \
	                                         (rhs)->TPP_INTERNAL(teiv_value)),                                                          \
	 (((rhs)->TPP_INTERNAL(teiv_value) > 0 && (lhs)->TPP_INTERNAL(teiv_value) > (_TPP_INTMAX_MAX - (rhs)->TPP_INTERNAL(teiv_value))) || \
	  ((rhs)->TPP_INTERNAL(teiv_value) < 0 && (lhs)->TPP_INTERNAL(teiv_value) < (_TPP_INTMAX_MIN - (rhs)->TPP_INTERNAL(teiv_value))))   \
	 ? TPP_ENOENT                                                                                                                       \
	 : TPP_EOK)
#else /* TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#define tpp_expr_intvalue_add(lhs, rhs, p_result)                              \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) + \
	                                         (rhs)->TPP_INTERNAL(teiv_value)), \
	 TPP_EOK)
#endif /* !TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */

/* >> [p_result] = [lhs] - [rhs];
 * @return: TPP_EOK:      OK
 * #if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ENOENT:   Overflow
 * #endif // TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ISERR(*): HARD_ERROR */
#if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
#define tpp_expr_intvalue_sub(lhs, rhs, p_result)                                                                                       \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) -                                                          \
	                                         (rhs)->TPP_INTERNAL(teiv_value)),                                                          \
	 (((rhs)->TPP_INTERNAL(teiv_value) < 0 && (lhs)->TPP_INTERNAL(teiv_value) > (_TPP_INTMAX_MAX + (rhs)->TPP_INTERNAL(teiv_value))) || \
	  ((rhs)->TPP_INTERNAL(teiv_value) > 0 && (lhs)->TPP_INTERNAL(teiv_value) < (_TPP_INTMAX_MIN + (rhs)->TPP_INTERNAL(teiv_value))))   \
	 ? TPP_ENOENT                                                                                                                       \
	 : TPP_EOK)
#else /* TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#define tpp_expr_intvalue_sub(lhs, rhs, p_result)                              \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) - \
	                                         (rhs)->TPP_INTERNAL(teiv_value)), \
	 TPP_EOK)
#endif /* !TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */

/* >> [p_result] = [lhs] * [rhs];
 * @return: TPP_EOK:      OK
 * #if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ENOENT:   Overflow
 * #endif // TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ISERR(*): HARD_ERROR */
#if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
#define tpp_expr_intvalue_mul(lhs, rhs, p_result)                                                  \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) *                     \
	                                         (rhs)->TPP_INTERNAL(teiv_value)),                     \
	 (lhs)->TPP_INTERNAL(teiv_value) > 0                                                           \
	 ? ((rhs)->TPP_INTERNAL(teiv_value) > 0                                                        \
	    ? ((lhs)->TPP_INTERNAL(teiv_value) > (_TPP_INTMAX_MAX / (rhs)->TPP_INTERNAL(teiv_value))   \
	       ? TPP_ENOENT /* +lhs * +lhs */                                                          \
	       : TPP_EOK)                                                                              \
	    : ((rhs)->TPP_INTERNAL(teiv_value) < (_TPP_INTMAX_MAX / (lhs)->TPP_INTERNAL(teiv_value))   \
	       ? TPP_ENOENT /* +lhs * -lhs */                                                          \
	       : TPP_EOK))                                                                             \
	 : ((rhs)->TPP_INTERNAL(teiv_value) > 0                                                        \
	    ? ((lhs)->TPP_INTERNAL(teiv_value) < (_TPP_INTMAX_MAX / (rhs)->TPP_INTERNAL(teiv_value))   \
	       ? TPP_ENOENT /* -lhs * +lhs */                                                          \
	       : TPP_EOK)                                                                              \
	    : ((lhs)->TPP_INTERNAL(teiv_value) &&                                                      \
	       ((rhs)->TPP_INTERNAL(teiv_value) < (_TPP_INTMAX_MAX / (lhs)->TPP_INTERNAL(teiv_value))) \
	       ? TPP_ENOENT /* -lhs * -lhs */                                                          \
	       : TPP_EOK)))
#else /* TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#define tpp_expr_intvalue_mul(lhs, rhs, p_result)                              \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) * \
	                                         (rhs)->TPP_INTERNAL(teiv_value)), \
	 TPP_EOK)
#endif /* !TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */

/* >> [p_result] = [lhs] / [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Divide-by-zero
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_div(lhs, rhs, p_result)                                 \
	((rhs)->TPP_INTERNAL(teiv_value)                                              \
	 ? ((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) / \
	                                            (rhs)->TPP_INTERNAL(teiv_value)), \
	    TPP_EOK)                                                                  \
	 : TPP_ENOENT)

/* >> [p_result] = [lhs] % [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ENOENT:   Divide-by-zero
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_mod(lhs, rhs, p_result)                                 \
	((rhs)->TPP_INTERNAL(teiv_value)                                              \
	 ? ((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) % \
	                                            (rhs)->TPP_INTERNAL(teiv_value)), \
	    TPP_EOK)                                                                  \
	 : TPP_ENOENT)

#undef _TPP_UINTMAX_BITS
#if TPP_UINTMAX_MAX <= TPP_UINT_LEAST8_C(0xff)
#define _TPP_UINTMAX_BITS 8
#elif TPP_UINTMAX_MAX <= TPP_UINT_LEAST16_C(0xffff)
#define _TPP_UINTMAX_BITS 16
#elif TPP_UINTMAX_MAX <= TPP_UINT_LEAST32_C(0xffffffff)
#define _TPP_UINTMAX_BITS 32
#elif TPP_UINTMAX_MAX <= TPP_UINT_LEAST64_C(0xffffffffffffffff)
#define _TPP_UINTMAX_BITS 64
#else /* TPP_UINTMAX_MAX <= ... */
#define _TPP_UINTMAX_BITS 128
#endif /* TPP_UINTMAX_MAX > ... */

/* >> [p_result] = [lhs] << [rhs];
 * @return: TPP_EOK:      OK
 * #if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ENOENT:   Overflow
 * #endif // TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ISERR(*): HARD_ERROR */
#if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
#define tpp_expr_intvalue_shl(lhs, rhs, p_result)                                                                   \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) << (rhs)->TPP_INTERNAL(teiv_value)),   \
	 ((rhs)->TPP_INTERNAL(teiv_value) >= _TPP_UINTMAX_BITS ||                                                       \
	  (lhs)->TPP_INTERNAL(teiv_value) != ((p_result)->TPP_INTERNAL(teiv_value) >> (rhs)->TPP_INTERNAL(teiv_value))) \
	 ? TPP_ENOENT                                                                                                   \
	 : TPP_EOK)
#else /* TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#define tpp_expr_intvalue_shl(lhs, rhs, p_result) \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) << (rhs)->TPP_INTERNAL(teiv_value)), TPP_EOK)
#endif /* !TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */

/* >> [p_result] = [lhs] >> [rhs];
 * @return: TPP_EOK:      OK
 * #if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ENOENT:   Overflow
 * #endif // TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
 * @return: TPP_ISERR(*): HARD_ERROR */
#if TPP_EXPR_INTVALUE_MATH_CANOVERFLOW
#define tpp_expr_intvalue_shr(lhs, rhs, p_result)                                                                   \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) >> (rhs)->TPP_INTERNAL(teiv_value)),   \
	 ((rhs)->TPP_INTERNAL(teiv_value) >= _TPP_UINTMAX_BITS ||                                                       \
	  (lhs)->TPP_INTERNAL(teiv_value) != ((p_result)->TPP_INTERNAL(teiv_value) << (rhs)->TPP_INTERNAL(teiv_value))) \
	 ? TPP_ENOENT                                                                                                   \
	 : TPP_EOK)
#else /* TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */
#define tpp_expr_intvalue_shr(lhs, rhs, p_result) \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) >> (rhs)->TPP_INTERNAL(teiv_value)), TPP_EOK)
#endif /* !TPP_EXPR_INTVALUE_MATH_CANOVERFLOW */

/* >> [p_result] = [lhs] & [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_and(lhs, rhs, p_result)                              \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) & \
	                                         (rhs)->TPP_INTERNAL(teiv_value)), \
	 TPP_EOK)

/* >> [p_result] = [lhs] ^ [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_xor(lhs, rhs, p_result)                              \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) ^ \
	                                         (rhs)->TPP_INTERNAL(teiv_value)), \
	 TPP_EOK)

/* >> [p_result] = [lhs] | [rhs];
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_or(lhs, rhs, p_result)                               \
	((p_result)->TPP_INTERNAL(teiv_value) = ((lhs)->TPP_INTERNAL(teiv_value) | \
	                                         (rhs)->TPP_INTERNAL(teiv_value)), \
	 TPP_EOK)

/* Store `< 0`, `== 0` or `> 0` to `*(int *)p_delta`, based on result of `lhs <=> rhs`
 * @return: TPP_EOK:      OK
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_intvalue_cmp(lhs, rhs, p_delta)                                       \
	(*(p_delta) = ((lhs)->TPP_INTERNAL(teiv_value) < (rhs)->TPP_INTERNAL(teiv_value)   \
	               ? -1                                                                \
	               : (lhs)->TPP_INTERNAL(teiv_value) > (rhs)->TPP_INTERNAL(teiv_value) \
	                 ? 1                                                               \
	                 : 0),                                                             \
	 TPP_EOK)

/* Print the representation of `self` to `printer` (in target encoding; used to implement `__TPP_EVAL`)
 * @return: *  : Sum of positive return value of `printer`
 * @return: < 0: An error was thrown (`TPP_SSIZE_ISERR`), or `printer` returned this value */
#if TPP_HAVE_EXPR_VALUE_PRINTREPR
#ifndef tpp_expr_value_printrepr
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_expr_intvalue_printrepr(struct tpp_lexer *tpp_restrict lexer,
                            tpp_expr_intvalue *tpp_restrict self,
                            tpp_formatprinter printer, void *arg);
#endif /* !tpp_expr_value_printrepr */
#endif /* TPP_HAVE_EXPR_VALUE_PRINTREPR */
#endif /* TPP_HAVE_BUILTIN_EXPR_VALUE */
#endif /* !tpp_expr_intvalue */


#if TPP_HAVE_BUILTIN_EXPR_VALUE
#ifndef tpp_expr_intvalue_init_bool
#define tpp_expr_intvalue_init_bool(self, v)   \
	((v) ? tpp_expr_intvalue_init_one(self, v) \
	     : tpp_expr_intvalue_init_zero(self, v))
#endif /* !tpp_expr_intvalue_init_bool */
#ifndef tpp_expr_intvalue_init_zero
#define tpp_expr_intvalue_init_zero(self) tpp_expr_intvalue_init_bool(self, 0)
#endif /* !tpp_expr_intvalue_init_zero */
#ifndef tpp_expr_intvalue_init_one
#define tpp_expr_intvalue_init_one(self) tpp_expr_intvalue_init_bool(self, 1)
#endif /* !tpp_expr_intvalue_init_one */
#endif /* TPP_HAVE_BUILTIN_EXPR_VALUE */


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
#define _tpp_expr_value_getkind(self)     (self)->TPP_INTERNAL(xv_kind)
#define _tpp_expr_value_setkind_(self, v) (self)->TPP_INTERNAL(xv_kind) = (v),
#else /* _TPP_EXPR_VALUE_KIND_MULTIPLE */
#define _tpp_expr_value_getkind(self)     _TPP_EXPR_VALUE_KIND_INT
#define _tpp_expr_value_setkind_(self, v) /* nothing */
#endif /* !_TPP_EXPR_VALUE_KIND_MULTIPLE */

	/* Expression value union */
	union {
		tpp_expr_intvalue   TPP_INTERNAL(xd_int);    /* [valid_if(_tpp_expr_value_getkind(self) == _TPP_EXPR_VALUE_KIND_INT)] */
#define _tpp_expr_value_getint(self) (&(self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_int))
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
 * @return: TPP_EOK:      Success
 * @return: TPP_ISERR(*): HARD_ERROR */
#define tpp_expr_value_copy(dst, src)                           \
	(*(dst) = *(src)_tpp_expr_value_incref(dst),                \
	 tpp_expr_value_isint(dst)                                  \
	 ? tpp_expr_intvalue_init_copy(_tpp_expr_value_getint(dst), \
	                               _tpp_expr_value_getint(src)) \
	 : TPP_EOK)

/* Finalize `self` (never fails) */
#define tpp_expr_value_fini(self)                          \
	((void)0 _tpp_expr_value_decref(self),                 \
	 tpp_expr_value_isint(self)                            \
	 ? tpp_expr_intvalue_fini(_tpp_expr_value_getint(dst)) \
	 : (void)0,                                            \
	 tpp_dbg_memset(self, sizeof(tpp_expr_value)))

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
#define tpp_expr_value_asintmax(self, p_result) \
	tpp_expr_intvalue_asintmax(_tpp_expr_value_getint(self), p_result)
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
#define tpp_expr_value_init_expr_intvalue(self, /*inherit(always)*/ /*tpp_expr_intvalue **/ v) \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_INT)                                  \
	 (self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_int) = (v),                                 \
	 TPP_EOK)
#if TPP_HAVE_LEXER_PARSECHARACTER_EXPR
#define tpp_expr_value_init_uintmax(self, /*tpp_uintmax*/ v)  \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_INT) \
	 tpp_expr_intvalue_init_uintmax(_tpp_expr_value_getint(self), v))
#endif /* TPP_HAVE_LEXER_PARSECHARACTER_EXPR */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
#define tpp_expr_value_init_size(self, /*tpp_size*/ v)        \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_INT) \
	 tpp_expr_intvalue_init_size(_tpp_expr_value_getint(self), v))
#define tpp_expr_value_init_char(self, /*tpp_char*/ v)        \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_INT) \
	 tpp_expr_intvalue_init_char(_tpp_expr_value_getint(self), v))
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */

/* Initialize to int=0/1 */
#define tpp_expr_value_init_bool(self, v)                     \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_INT) \
	 tpp_expr_intvalue_init_bool(_tpp_expr_value_getint(self), v))
#define tpp_expr_value_init_zero(self)                        \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_INT) \
	 tpp_expr_intvalue_init_zero(_tpp_expr_value_getint(self)))
#define tpp_expr_value_init_one(self)                         \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_INT) \
	 tpp_expr_intvalue_init_one(_tpp_expr_value_getint(self)))


/* Initialize `self` as float-typed, with `v` as value
 * @return: TPP_EOK: Success */
#if TPP_HAVE_BUILTIN_EXPR_FLOATS
#define tpp_expr_value_init_float(self, /*tpp_float*/ v)         \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_FLOAT)  \
	 (self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_float) = (v), \
	 TPP_EOK)
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS */

/* Initialize `self` as string-typed, with `v` as value
 * @param: str: [1..1] The string value to assign
 * @return: TPP_EOK: Success */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
#define tpp_expr_value_init_string(self, /*tpp_string **/ str)                 \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_STRING)               \
	 (self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_string) = (str),            \
	 tpp_string_incref((self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_string)), \
	 TPP_EOK)
#define tpp_expr_value_init_string_inherited(self, /*inherit(always)*/ /*TPP_REF tpp_string **/ str) \
	(_tpp_expr_value_setkind_(self, _TPP_EXPR_VALUE_KIND_STRING)                                     \
	 (self)->TPP_INTERNAL(xv_data).TPP_INTERNAL(xd_string) = (str),                                  \
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
/* TODO: Use `tpp_expr_intvalue_*` APIs from above */
#define tpp_expr_value_pos(lexer, self, result)               tpp_expr_value_init_expr_intvalue(result, +_tpp_expr_value_getint(self))
#define tpp_expr_value_neg(lexer, self, result)               tpp_expr_value_init_expr_intvalue(result, -_tpp_expr_value_getint(self))
#define tpp_expr_value_inv(lexer, self, result)               tpp_expr_value_init_expr_intvalue(result, ~_tpp_expr_value_getint(self))
#define tpp_expr_value_add(lexer, lhs, rhs, result)           tpp_expr_value_init_expr_intvalue(result, _tpp_expr_value_getint(lhs) + _tpp_expr_value_getint(rhs))
#define tpp_expr_value_sub(lexer, lhs, rhs, result)           tpp_expr_value_init_expr_intvalue(result, _tpp_expr_value_getint(lhs) - _tpp_expr_value_getint(rhs))
#define tpp_expr_value_mul(lexer, lhs, rhs, result)           tpp_expr_value_init_expr_intvalue(result, _tpp_expr_value_getint(lhs) * _tpp_expr_value_getint(rhs))
#define tpp_expr_value_shl(lexer, lhs, rhs, result)           tpp_expr_value_init_expr_intvalue(result, _tpp_expr_value_getint(lhs) << _tpp_expr_value_getint(rhs))
#define tpp_expr_value_shr(lexer, lhs, rhs, result)           tpp_expr_value_init_expr_intvalue(result, _tpp_expr_value_getint(lhs) >> _tpp_expr_value_getint(rhs))
#define tpp_expr_value_and(lexer, lhs, rhs, result)           tpp_expr_value_init_expr_intvalue(result, _tpp_expr_value_getint(lhs) & _tpp_expr_value_getint(rhs))
#define tpp_expr_value_xor(lexer, lhs, rhs, result)           tpp_expr_value_init_expr_intvalue(result, _tpp_expr_value_getint(lhs) | _tpp_expr_value_getint(rhs))
#define tpp_expr_value_or(lexer, lhs, rhs, result)            tpp_expr_value_init_expr_intvalue(result, _tpp_expr_value_getint(lhs) ^ _tpp_expr_value_getint(rhs))
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
#ifndef tpp_expr_value_printrepr
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_expr_value_printrepr(struct tpp_lexer *tpp_restrict lexer,
                         tpp_expr_value *tpp_restrict self,
                         tpp_formatprinter printer, void *arg);
#endif /* !tpp_expr_value_printrepr */
#endif /* TPP_HAVE_EXPR_VALUE_PRINTREPR */
#endif /* TPP_HAVE_BUILTIN_EXPR_VALUE */

/* Initialize to int=0 */
#ifndef tpp_expr_value_init_zero
#define tpp_expr_value_init_zero(self) tpp_expr_value_init_bool(self, 0)
#endif /* !tpp_expr_value_init_zero */

/* Initialize to int=1 */
#ifndef tpp_expr_value_init_one
#define tpp_expr_value_init_one(self) tpp_expr_value_init_bool(self, 1)
#endif /* !tpp_expr_value_init_one */

/* Initialize to int=0/1 */
#ifndef tpp_expr_value_init_bool
#define tpp_expr_value_init_bool(self, v) \
	((v) ? tpp_expr_value_init_one(self)  \
	     : tpp_expr_value_init_zero(self))
#endif /* !tpp_expr_value_init_bool */

#ifndef tpp_expr_value_pos
#define tpp_expr_value_pos(lexer, self, result) tpp_expr_value_copy(result, self)
#endif /* !tpp_expr_value_pos */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_EXPR_H */
