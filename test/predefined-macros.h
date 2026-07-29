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

/* Definition of predefined macros must support the same operations as user-defined macros */
TPP_ASSERT(defined(__TPP_VERSION__))
TPP_ASSERT_EXPANDS("300", __TPP_VERSION__)

#pragma push_macro("__TPP_VERSION__")
TPP_ASSERT(defined(__TPP_VERSION__))
TPP_ASSERT_EXPANDS("300", __TPP_VERSION__)
#undef __TPP_VERSION__
TPP_ASSERT(!defined(__TPP_VERSION__))
TPP_ASSERT_EXPANDS("__TPP_VERSION__", __TPP_VERSION__)
#define __TPP_VERSION__ 1234
TPP_ASSERT(defined(__TPP_VERSION__))
TPP_ASSERT_EXPANDS("1234", __TPP_VERSION__)
#pragma pop_macro("__TPP_VERSION__")

TPP_ASSERT(defined(__TPP_VERSION__))
TPP_ASSERT_EXPANDS("300", __TPP_VERSION__)


/* Definition of builtin macros must support the same operations as user-defined macros */
TPP_ASSERT(defined(__TPP_EVAL))
TPP_ASSERT_EXPANDS("30", __TPP_EVAL(10+20))

#pragma push_macro("__TPP_EVAL")
TPP_ASSERT(defined(__TPP_EVAL))
TPP_ASSERT_EXPANDS("30", __TPP_EVAL(10+20))
#undef __TPP_EVAL
TPP_ASSERT(!defined(__TPP_EVAL))
TPP_ASSERT_EXPANDS("__TPP_EVAL(10+20)", __TPP_EVAL(10+20))
#define __TPP_EVAL(expr) 42 /* Should always be the correct answer */
TPP_ASSERT(defined(__TPP_EVAL))
TPP_ASSERT_EXPANDS("42", __TPP_EVAL(10+20))
#pragma pop_macro("__TPP_EVAL")

TPP_ASSERT(defined(__TPP_EVAL))
TPP_ASSERT_EXPANDS("30", __TPP_EVAL(10+20))


/* Expansion of builtin/predefined macros must be controllable
 * NOTE: This test must use `#if` + `#error` since "TPP_ASSERT"
 *       relies on a couple of "builtin" macros, which we
 *       temporarily turn off during this part of the test. */
#pragma TPP extension(push)
TPP_ASSERT(__has_extension("-fcpp-builtin-macros"))
TPP_ASSERT(__has_extension("-fcpp-predefined-macros"))
#ifndef __TPP_VERSION__
#error "'__TPP_VERSION__' should be defined right now"
#endif
#ifndef __TPP_EVAL
#error "'__TPP_EVAL' should be defined right now"
#endif

/* Disabling predefined macros is like passing `-undef` */
#pragma TPP extension("-fno-cpp-predefined-macros")
#ifdef __TPP_VERSION__
#error "'__TPP_VERSION__' should NOT be defined right now"
#endif
#ifndef __TPP_EVAL
#error "'__TPP_EVAL' should be defined right now"
#endif

/* Disabling builtin macros isn't something you'd normally do */
#pragma TPP extension("-fno-cpp-builtin-macros")
#ifdef __TPP_VERSION__
#error "'__TPP_VERSION__' should NOT be defined right now"
#endif
#ifdef __TPP_EVAL
#error "'__TPP_EVAL' should NOT be defined right now"
#endif

#pragma TPP extension(pop)
#ifndef __TPP_VERSION__
#error "'__TPP_VERSION__' should be defined right now"
#endif
#ifndef __TPP_EVAL
#error "'__TPP_EVAL' should be defined right now"
#endif

