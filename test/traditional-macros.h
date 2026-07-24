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
#include "utils/test.h"

/* Enabling "-ftraditional-macro" alters macro definitions
 * to follow more ~traditional~ expansion rules. */
#pragma extension(push, "-ftraditional-macro")
#define T_CAT1(a, b) a/**/b
#define T_STR1(x)    "x"
#define T_CAT2(a, b) a##b
#define T_STR2(x)    #x
#pragma extension(pop)

#define N_CAT1(a, b) a/**/b
#define N_STR1(x)    "x"
#define N_CAT2(a, b) a##b
#define N_STR2(x)    #x

TPP_ASSERT_EXPANDS("1020",           T_CAT1(10, 20))
TPP_ASSERT_EXPANDS("10##20",         T_CAT2(10, 20))
TPP_ASSERT_EXPANDS("\"10\"",         T_STR1(10))
TPP_ASSERT_EXPANDS("#10",            T_STR2(10))
TPP_ASSERT_EXPANDS("\"\"10\"\"",     T_STR1("10"))
TPP_ASSERT_EXPANDS("#\"10\"",        T_STR2("10"))

TPP_ASSERT_EXPANDS("10/**/20",       N_CAT1(10, 20))
TPP_ASSERT_EXPANDS("1020",           N_CAT2(10, 20))
TPP_ASSERT_EXPANDS("\"x\"",          N_STR1(10))
TPP_ASSERT_EXPANDS("\"10\"",         N_STR2(10))
TPP_ASSERT_EXPANDS("\"x\"",          N_STR1("10"))
TPP_ASSERT_EXPANDS("\"\\\"10\\\"\"", N_STR2("10"))

#undef T_CAT1
#undef T_STR1
#undef T_CAT2
#undef T_STR2
#undef N_CAT1
#undef N_STR1
#undef N_CAT2
#undef N_STR2
