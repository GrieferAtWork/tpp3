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

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#undef TPP_HAVE_BUILTIN_EXPR_VALUE
#ifndef tpp_expr_value
#define TPP_HAVE_BUILTIN_EXPR_VALUE 1
#define tpp_expr_value tpp_expr_value
typedef struct tpp_expr_value {
	tpp_intmax xv_value;
} tpp_expr_value;

#define tpp_expr_value_move(dst, src) (void)(*(dst) = *(src))
#define tpp_expr_value_copy(dst, src) (*(dst) = *(src), TPP_EOK)
#define tpp_expr_value_fini(self)     (void)0

/* Initialize to int=0/1 */
#define tpp_expr_value_init_bool(self, v) ((self)->xv_value = (v) ? 1 : 0, TPP_EOK)

#define tpp_expr_value_pos(self, result)     (*(result) = *(self), TPP_EOK)
#define tpp_expr_value_neg(self, result)     ((result)->xv_value = -(self)->xv_value, TPP_EOK)
#define tpp_expr_value_inv(self, result)     ((result)->xv_value = ~(self)->xv_value, TPP_EOK)
#define tpp_expr_value_add(lhs, rhs, result) ((result)->xv_value = (lhs)->xv_value + (rhs)->xv_value, TPP_EOK)
#define tpp_expr_value_sub(lhs, rhs, result) ((result)->xv_value = (lhs)->xv_value - (rhs)->xv_value, TPP_EOK)
#define tpp_expr_value_mul(lhs, rhs, result) ((result)->xv_value = (lhs)->xv_value * (rhs)->xv_value, TPP_EOK)
#define tpp_expr_value_div(lhs, rhs, result) ((result)->xv_value = (lhs)->xv_value / (rhs)->xv_value, TPP_EOK)
#define tpp_expr_value_mod(lhs, rhs, result) ((result)->xv_value = (lhs)->xv_value % (rhs)->xv_value, TPP_EOK)
#define tpp_expr_value_shl(lhs, rhs, result) ((result)->xv_value = (lhs)->xv_value << (rhs)->xv_value, TPP_EOK)
#define tpp_expr_value_shr(lhs, rhs, result) ((result)->xv_value = (lhs)->xv_value >> (rhs)->xv_value, TPP_EOK)
#define tpp_expr_value_and(lhs, rhs, result) ((result)->xv_value = (lhs)->xv_value & (rhs)->xv_value, TPP_EOK)
#define tpp_expr_value_or(lhs, rhs, result)  ((result)->xv_value = (lhs)->xv_value | (rhs)->xv_value, TPP_EOK)
#define tpp_expr_value_xor(lhs, rhs, result) ((result)->xv_value = (lhs)->xv_value ^ (rhs)->xv_value, TPP_EOK)

#define tpp_expr_value_cmp_eq(lhs, rhs, p_bool_result) (*(p_bool_result) = ((lhs)->xv_value == (rhs)->xv_value), TPP_EOK)
#define tpp_expr_value_cmp_ne(lhs, rhs, p_bool_result) (*(p_bool_result) = ((lhs)->xv_value != (rhs)->xv_value), TPP_EOK)
#define tpp_expr_value_cmp_lo(lhs, rhs, p_bool_result) (*(p_bool_result) = ((lhs)->xv_value < (rhs)->xv_value), TPP_EOK)
#define tpp_expr_value_cmp_le(lhs, rhs, p_bool_result) (*(p_bool_result) = ((lhs)->xv_value <= (rhs)->xv_value), TPP_EOK)
#define tpp_expr_value_cmp_gr(lhs, rhs, p_bool_result) (*(p_bool_result) = ((lhs)->xv_value > (rhs)->xv_value), TPP_EOK)
#define tpp_expr_value_cmp_ge(lhs, rhs, p_bool_result) (*(p_bool_result) = ((lhs)->xv_value >= (rhs)->xv_value), TPP_EOK)

/* Determine the boolean-style value of "self" */
#define tpp_expr_value_istrue(self, p_bool_result) (*(p_bool_result) = ((self)->xv_value != 0), TPP_EOK)

#if TPP_HAVE_EXPR_VALUE_PRINTREPR
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_expr_value_printrepr(tpp_expr_value const *tpp_restrict self,
                         tpp_formatprinter printer, void *arg);
#endif /* TPP_HAVE_EXPR_VALUE_PRINTREPR */
#endif /* !tpp_expr_value */

#ifndef TPP_HAVE_BUILTIN_EXPR_VALUE
#define TPP_HAVE_BUILTIN_EXPR_VALUE 0
#endif /* !TPP_HAVE_BUILTIN_EXPR_VALUE */


/* Initialize to int=0 */
#ifndef tpp_expr_value_init_zero
#define tpp_expr_value_init_zero(self) tpp_expr_value_init_bool(self, 0)
#endif /* !tpp_expr_value_init_zero */

/* Initialize to int=1 */
#ifndef tpp_expr_value_init_one
#define tpp_expr_value_init_one(self) tpp_expr_value_init_bool(self, 1)
#endif /* !tpp_expr_value_init_one */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_EXPR_H */
