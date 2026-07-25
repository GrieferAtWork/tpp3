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

#pragma TPP extension(push)
#pragma TPP extension("-fcharacter-literals")

/* TPP Allows for use of strings in #if directives (as well as `__TPP_EVAL') */
TPP_ASSERT_EXPANDS("70", __TPP_EVAL('F'))
TPP_ASSERT_EXPANDS("70", __TPP_EVAL("FOO"[0]))
#pragma TPP extension("-fno-character-literals")
TPP_ASSERT_EXPANDS("70", __TPP_EVAL("FOO"[0])) /* Must also work when character-literals are disabled */
#pragma TPP extension("-fcharacter-literals")
TPP_ASSERT_EXPANDS("\"FOO\"", __TPP_EVAL("FOO"[0:]))
TPP_ASSERT_EXPANDS("\"OO\"", __TPP_EVAL("FOO"[1:]))
TPP_ASSERT_EXPANDS("\"F\"", __TPP_EVAL("FOO"[0:1]))
TPP_ASSERT_EXPANDS("\"FO\"", __TPP_EVAL("FOO"[0:2]))
TPP_ASSERT_EXPANDS("\"FO\"", __TPP_EVAL("FOO"[0:-1]))
TPP_ASSERT_EXPANDS("\"FO\"", __TPP_EVAL("FOO"[:-1]))
TPP_ASSERT_EXPANDS("\"O\"", __TPP_EVAL("FOO"[1:-1]))
TPP_ASSERT_EXPANDS("0", __TPP_EVAL("FOO" == "BAR"))
TPP_ASSERT_EXPANDS("1", __TPP_EVAL("FOO" != "BAR"))
TPP_ASSERT_EXPANDS("1", __TPP_EVAL("FOO" == "FOO"))
TPP_ASSERT_EXPANDS("0", __TPP_EVAL("FOO" == "foo"))

/* Multi-char character literals. */
TPP_ASSERT_EXPANDS("97", __TPP_EVAL('a'))
TPP_ASSERT_WARNING_BEGIN("-Wmultichar") /* multichar literal `'ab'` used */
TPP_ASSERT_EXPANDS("24930", __TPP_EVAL('ab')) /* Does *NOT* expand to '25185' (which TPP2 used to do incorrectly) */
TPP_ASSERT_WARNING_END("-Wmultichar")
TPP_ASSERT_WARNING_BEGIN("-Wmultichar") /* multichar literal `'abc'` used */
TPP_ASSERT_EXPANDS("6382179", __TPP_EVAL('abc')) /* Does *NOT* expand to '6513249' (which TPP2 used to do incorrectly) */
TPP_ASSERT_WARNING_END("-Wmultichar")


#define CAT2(a, b) a##b
#define CAT(a, b) CAT2(a, b)
#define ISLESS_0 more than
#define ISLESS_1 less than
#define SELECT(n) CAT(ISLESS_, __TPP_EVAL(n < 10))
TPP_ASSERT_EXPANDS("less than", SELECT(7))
TPP_ASSERT_EXPANDS("more than", SELECT(10))

#undef SELECT
#undef ISLESS_1
#undef ISLESS_0
#undef CAT
#undef CAT2

TPP_ASSERT_EXPANDS("\"Hello\"", __TPP_STR_PACK(0x48, 0x65, 0x6c, 0x6c, 0x6f))

#pragma TPP extension(pop)
