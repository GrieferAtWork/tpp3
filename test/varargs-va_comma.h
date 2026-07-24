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

// Function-style macro calling and argument count

#define arg0()           -
#define arg1(a)          a
#define arg2(a,b)        a##b
#define varg0(...)       __VA_ARGS__
#define varg1(a,...)     a##__VA_ARGS__
#define varg2(a,b,...)   a##b##__VA_ARGS__
#define vcarg0(...)      __VA_COMMA__ __VA_ARGS__ __VA_COMMA__
#define vcarg1(a,...)    __VA_COMMA__ a##__VA_ARGS__ __VA_COMMA__
#define vcarg2(a,b,...)  __VA_COMMA__ a##b##__VA_ARGS__ __VA_COMMA__

// NOTE: Space tokens [ ] are not included in the expansion comments
TPP_ASSERT_EXPANDS("-", arg0())            // OK

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("-", arg0(a))           // WARNING: Invalid argument count (expected 0)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_EXPANDS("", arg1())             // OK
TPP_ASSERT_EXPANDS("a", arg1(a))           // OK
TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("a,b", arg1(a,b))       // WARNING: Invalid argument count (expected 1)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS(",", arg1(,))           // WARNING: Invalid argument count (expected 1)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("a", arg2(a))           // WARNING: Invalid argument count (expected 2)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_EXPANDS("a", arg2(a,))          // OK
TPP_ASSERT_EXPANDS("", arg2(,))            // OK
TPP_ASSERT_EXPANDS("ab", arg2(a,b))        // OK

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS(",", arg2(,,))          // WARNING: Invalid argument count (expected 2)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("ab,c", arg2(a,b,c))    // WARNING: Invalid argument count (expected 2)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_EXPANDS("", varg0())            // OK
TPP_ASSERT_EXPANDS("a", varg0(a))          // OK
TPP_ASSERT_EXPANDS("a,", varg0(a,))        // OK
TPP_ASSERT_EXPANDS(",", varg0(,))          // OK
TPP_ASSERT_EXPANDS("a,b", varg0(a,b))      // OK
TPP_ASSERT_EXPANDS("", vcarg0())           // OK
TPP_ASSERT_EXPANDS(", a ,", vcarg0(a))     // OK
TPP_ASSERT_EXPANDS(", a, ,", vcarg0(a,))   // OK
TPP_ASSERT_EXPANDS(", , ,", vcarg0(,))     // OK
TPP_ASSERT_EXPANDS(", a,b ,", vcarg0(a,b)) // OK

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("", varg1())            // too few arguments for `varg1` which takes 2 when 0 were given
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_EXPANDS("a", varg1(a))          // OK
TPP_ASSERT_EXPANDS("a", varg1(a,))         // OK
TPP_ASSERT_EXPANDS("", varg1(,))           // OK
TPP_ASSERT_EXPANDS("ab", varg1(a,b))       // OK

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("", vcarg1())           // too few arguments for `vcarg1` which takes 2 when 0 were given
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_EXPANDS("a", vcarg1(a))         // OK
TPP_ASSERT_EXPANDS(", a ,", vcarg1(a,))    // OK  (changed in TPP3; TPP2 used to expand to "a" here)
TPP_ASSERT_EXPANDS(",  ,", vcarg1(,))      // OK  (changed in TPP3; TPP2 used to expand to "" here)
TPP_ASSERT_EXPANDS(", ab ,", vcarg1(a,b))  // OK

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("a", varg2(a))          // WARNING: Invalid argument count (expected >= 2)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("", varg2())            // WARNING: Invalid argument count (expected >= 2)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_EXPANDS("a", varg2(a,))         // OK
TPP_ASSERT_EXPANDS("", varg2(,))           // OK
TPP_ASSERT_EXPANDS("ab", varg2(a,b))       // OK

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("", vcarg2())           // WARNING: Invalid argument count (expected >= 2)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_WARNING_BEGIN("-Wmacros")
TPP_ASSERT_EXPANDS("a", vcarg2(a))         // WARNING: Invalid argument count (expected >= 2)
TPP_ASSERT_WARNING_END("-Wmacros")

TPP_ASSERT_EXPANDS("a", vcarg2(a,))        // OK
TPP_ASSERT_EXPANDS("", vcarg2(,))          // OK
TPP_ASSERT_EXPANDS("ab", vcarg2(a,b))      // OK

#undef arg0
#undef arg1
#undef arg2
#undef varg0
#undef varg1
#undef varg2
#undef vcarg0
#undef vcarg1
#undef vcarg2
