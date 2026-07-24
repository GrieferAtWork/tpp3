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

// #pragma push_macro

TPP_ASSERT_EXPANDS(
	"int *a = new int();",
	int *a = new int();
)
#define new __debug_new
TPP_ASSERT_EXPANDS(
	"int *b = __debug_new int();",
	int *b = new int();
)
#pragma push_macro("new")
#undef new
#define new __alt_default_new
TPP_ASSERT_EXPANDS(
	"int *c = __alt_default_new int();",
	int *c = new int();
)
#pragma pop_macro("new")
TPP_ASSERT_EXPANDS(
	"int *d = __debug_new int();",
	int *d = new int();
)
TPP_ASSERT_EXPANDS(
	"int *e = __debug_new int();",
	int *e = new int();
)
#undef new
TPP_ASSERT_EXPANDS(
	"int *f = new int();",
	int *f = new int();
)

