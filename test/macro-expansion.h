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

#define MY_MACRO 42
TPP_ASSERT_EXPANDS("42", MY_MACRO)
#undef MY_MACRO
TPP_ASSERT_EXPANDS("MY_MACRO", MY_MACRO)



#define EMPTY
#define SCAN(x)     x
#define EXAMPLE_()  EXAMPLE
#define EXAMPLE(n)  EXAMPLE_ EMPTY()(n-1) (n)

TPP_ASSERT_EXPANDS(
	"EXAMPLE_ ()(5-1) (5)",
	EXAMPLE(5)
)
TPP_ASSERT_EXPANDS(
	"EXAMPLE_ ()(5-1-1) (5-1) (5)",
	SCAN(EXAMPLE(5))
)
TPP_ASSERT_EXPANDS(
	"EXAMPLE_ ()(5-1-1-1) (5-1-1) (5-1) (5)",
	SCAN(SCAN(EXAMPLE(5)))
)

#undef EMPTY
#undef SCAN
#undef EXAMPLE_
#undef EXAMPLE
