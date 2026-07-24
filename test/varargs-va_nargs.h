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

#define nargs(...) __VA_NARGS__

/* __VA_NARGS__ expands to the decimal repr of the # of variadic arguments */
TPP_ASSERT_EXPANDS("0", nargs())
TPP_ASSERT_EXPANDS("0", nargs( ))
TPP_ASSERT_EXPANDS("1", nargs(~))
TPP_ASSERT_EXPANDS("2", nargs(,))
TPP_ASSERT_EXPANDS("2", nargs(a,))
TPP_ASSERT_EXPANDS("2", nargs(a,b))

#undef nargs


/* Using __VA_NARGS__, you can _very_ easily overload macros by argument count! */
#define myfunc_0()     no arguments
#define myfunc_1(a)    first argument is <a>
#define myfunc_2(a, b) first argument: <a>; and second is <b>
#define myfunc(...)    myfunc_##__VA_NARGS__(__VA_ARGS__)

TPP_ASSERT_EXPANDS(
	"no arguments",
	myfunc()
)
TPP_ASSERT_EXPANDS(
	"no arguments",
	myfunc( )
)
TPP_ASSERT_EXPANDS(
	"no arguments",
	myfunc(/**/)
)
TPP_ASSERT_EXPANDS(
	"first argument is <\"Hello\">",
	myfunc("Hello")
)
TPP_ASSERT_EXPANDS(
	"first argument: <\"Hello\">; and second is <\"World\">",
	myfunc("Hello", "World")
)

#undef myfunc_0
#undef myfunc_1
#undef myfunc_2
#undef myfunc

#define nargs1(a, ...) a __VA_NARGS__
TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS(
	"0",
	nargs1() // too few arguments for `nargs1` which takes 2 when 0 were given
)
TPP_ASSERT_WARNING_END("-Wmacros")
TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS(
	"0",
	nargs1( ) // too few arguments for `nargs1` which takes 2 when 0 were given
)
TPP_ASSERT_WARNING_END("-Wmacros")
TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS(
	"0",
	nargs1(/**/) // too few arguments for `nargs1` which takes 2 when 0 were given
)
TPP_ASSERT_WARNING_END("-Wmacros")
TPP_ASSERT_EXPANDS(
	"~ 0",
	nargs1(~)
)
TPP_ASSERT_EXPANDS(
	"1", /* Changed in TPP3 -- used to expand to "0" in TPP2 */
	nargs1(,)
)
TPP_ASSERT_EXPANDS(
	"1", /* Changed in TPP3 -- used to expand to "0" in TPP2 */
	nargs1(, )
)
TPP_ASSERT_EXPANDS(
	"a 1", /* Changed in TPP3 -- used to expand to "a 0" in TPP2 */
	nargs1(a,)
)
TPP_ASSERT_EXPANDS(
	"a 1",
	nargs1(a,b)
)
#undef nargs1

