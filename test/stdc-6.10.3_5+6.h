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

// ISO/IEC 9899:201x - 6.10.3 - 5 + 6

// ISO/IEC 9899:201x - 6.10.3 - 5
TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-parameter")
#define VARARGS1(__VA_ARGS__)      __VA_ARGS__
TPP_ASSERT_WARNING_END("-Wreserved-macro-parameter")
TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-parameter")
TPP_ASSERT_WARNING_BEGIN("-Wsyntax") /* duplicate macro parameter name */
#define VARARGS2(__VA_ARGS__, ...) __VA_ARGS__
TPP_ASSERT_WARNING_END("-Wsyntax")
TPP_ASSERT_WARNING_END("-Wreserved-macro-parameter")
TPP_ASSERT_EXPANDS("foo", VARARGS1(foo))
TPP_ASSERT_EXPANDS("foo", VARARGS2(foo))
TPP_ASSERT_EXPANDS("foo", VARARGS2(foo,))
#undef VARARGS2
#undef VARARGS1

TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-parameter")
#define VARARGS1(__VA_COMMA__)      __VA_COMMA__
TPP_ASSERT_WARNING_END("-Wreserved-macro-parameter")
TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-parameter")
#define VARARGS2(__VA_COMMA__, ...) __VA_COMMA__
TPP_ASSERT_WARNING_END("-Wreserved-macro-parameter")
TPP_ASSERT_EXPANDS("foo", VARARGS1(foo))
TPP_ASSERT_EXPANDS("",    VARARGS2(foo))
TPP_ASSERT_EXPANDS(",",   VARARGS2(foo,))
#undef VARARGS2
#undef VARARGS1

TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-parameter")
#define VARARGS1(__VA_OPT__)      __VA_OPT__(a)
TPP_ASSERT_WARNING_END("-Wreserved-macro-parameter")
TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-parameter")
#define VARARGS2(__VA_OPT__, ...) __VA_OPT__(a)
TPP_ASSERT_WARNING_END("-Wreserved-macro-parameter")
TPP_ASSERT_EXPANDS("foo(a)", VARARGS1(foo))
TPP_ASSERT_EXPANDS("",       VARARGS2(foo))
TPP_ASSERT_EXPANDS("a",      VARARGS2(foo,))
#undef VARARGS2
#undef VARARGS1

TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-parameter")
#define VARARGS1(__VA_NARGS__)      __VA_NARGS__
TPP_ASSERT_WARNING_END("-Wreserved-macro-parameter")
TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-parameter")
#define VARARGS2(__VA_NARGS__, ...) __VA_NARGS__
TPP_ASSERT_WARNING_END("-Wreserved-macro-parameter")
TPP_ASSERT_EXPANDS("foo", VARARGS1(foo))
TPP_ASSERT_EXPANDS("0",   VARARGS2(foo))
TPP_ASSERT_EXPANDS("1",   VARARGS2(foo,))
#undef VARARGS2
#undef VARARGS1

TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-keyword")
#define VARARGS(a) __VA_ARGS__
TPP_ASSERT_WARNING_END("-Wreserved-macro-keyword")
TPP_ASSERT_EXPANDS("__VA_ARGS__", VARARGS(foo))
#undef VARARGS

TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-keyword")
#define VARARGS(a) __VA_COMMA__
TPP_ASSERT_WARNING_END("-Wreserved-macro-keyword")
TPP_ASSERT_EXPANDS("__VA_COMMA__", VARARGS(foo))
#undef VARARGS

TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-keyword")
#define VARARGS(a) __VA_NARGS__
TPP_ASSERT_WARNING_END("-Wreserved-macro-keyword")
TPP_ASSERT_EXPANDS("__VA_NARGS__", VARARGS(foo))
#undef VARARGS

TPP_ASSERT_WARNING_BEGIN("-Wreserved-macro-keyword")
#define VARARGS(a) __VA_OPT__(a)
TPP_ASSERT_WARNING_END("-Wreserved-macro-keyword")
TPP_ASSERT_EXPANDS("__VA_OPT__(foo)", VARARGS(foo))
#undef VARARGS


// ISO/IEC 9899:201x - 6.10.3 - 6
TPP_ASSERT_WARNING_BEGIN("-Wsyntax") /* duplicate macro parameter name `a` */
#define ARG_NAMES(a,b,c,a) a+b+c+a
TPP_ASSERT_WARNING_END("-Wsyntax")
TPP_ASSERT_EXPANDS("10+20+30+10", ARG_NAMES(10,20,30,40))
#undef ARG_NAMES
