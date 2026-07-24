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

/* Preprocessor blocks within macro arguments */

TPP_ASSERT_EXPANDS(
	"",
#if 0
	Stuff in the middle
#endif
)

TPP_ASSERT_EXPANDS(
	"Stuff in the middle",
#if 1
	Stuff in the middle
#endif
)

#define NARGS(...) __VA_NARGS__

TPP_ASSERT_EXPANDS("0", NARGS(
#if 0
	a
#endif
))

TPP_ASSERT_EXPANDS("1", NARGS(
#if 0
	a
#endif
	b
))

TPP_ASSERT_EXPANDS("1", NARGS(
	b
#if 0
	a
#endif
))

TPP_ASSERT_EXPANDS("1", NARGS(
#if 1
	a
#endif
))

#undef NARGS

#define STR1(a, b...) #a
#define STR2(a, b...) #b

#pragma TPP extension(push, "-fmacro-argument-whitespace")
#define STR1_SPC(a, b...) #a
#define STR2_SPC(a, b...) #b
#pragma TPP extension(pop)

/* Unlike TPP2, TPP3 actually processes preprocessor
 * directives within macro argument lists! */
TPP_ASSERT_EXPANDS("\"a\\n\tb\"", STR1(
	a
#if 0
	X,Y
#endif
	b
))

TPP_ASSERT_EXPANDS("\"\"", STR2(
	a
#if 0
	X,Y
#endif
	b
))

TPP_ASSERT_EXPANDS("\"a\\n\tX\"", STR1(
	a
#if 1
	X,Y
#endif
	b
))

TPP_ASSERT_EXPANDS("\"Y\\n\tb\"", STR2(
	a
#if 1
	X,Y
#endif
	b
))

/* Because the `// COMMENT` below is inside of a `#if 0`-block,
 * it doesn't appear in the macro argument */
TPP_ASSERT_EXPANDS("\"a\\n\tX\"", STR1(
	a
#if 1
#if 0
// COMMENT
#endif
	X,Y
#endif
	b
))

/* Different case: because the `// COMMENT` is inside of a `#if 1`-block,
 * and because it is (eventually) succeeded by a non-whitespace token
 * preceding the end of the (current) argument, it *must* be included
 * in the macro argument! */
TPP_ASSERT_EXPANDS("\"a\\n// COMMENT\\n\tX\"", STR1(
	a
#if 1
#if 1
// COMMENT
#endif
	X,Y
#endif
	b
))

/* Same as the test before, but this time the "X" is removed. As such,
 * and because macro arguments have data from leading/trailing space
 * tokens trimmed, the first argument must *only* be "a" */
TPP_ASSERT_EXPANDS("\"a\"", STR1(
	a
#if 1
#if 1
// COMMENT
#endif
	,Y
#endif
	b
))





/* Repeat tests from above, but with macro-argument-whitespace turned on */

TPP_ASSERT_EXPANDS("\"\\n\ta\\n\tb\\n\"", STR1_SPC(
	a
#if 0
	X,Y
#endif
	b
))

TPP_ASSERT_EXPANDS("\"\"", STR2_SPC(
	a
#if 0
	X,Y
#endif
	b
))

TPP_ASSERT_EXPANDS("\"\\n\ta\\n\tX\"", STR1_SPC(
	a
#if 1
	X,Y
#endif
	b
))

TPP_ASSERT_EXPANDS("\"Y\\n\tb\\n\"", STR2_SPC(
	a
#if 1
	X,Y
#endif
	b
))

TPP_ASSERT_EXPANDS("\"\\n\ta\\n\tX\"", STR1_SPC(
	a
#if 1
#if 0
// COMMENT
#endif
	X,Y
#endif
	b
))

TPP_ASSERT_EXPANDS("\"\\n\ta\\n// COMMENT\\n\tX\"", STR1_SPC(
	a
#if 1
#if 1
// COMMENT
#endif
	X,Y
#endif
	b
))

/* Even this super-complex trailing whitespace sequence is perfectly retained */
TPP_ASSERT_EXPANDS("\"\\n\ta\\n// COMMENT\\n\t\"", STR1_SPC(
	a
#if 1
#if 1
// COMMENT
#endif
	,Y
#endif
	b
))

#undef STR2_SPC
#undef STR1_SPC
#undef STR2
#undef STR1
