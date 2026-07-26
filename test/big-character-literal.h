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

/* Assert that the preprocessor is able to (correctly) parse character literals */
TPP_ASSERT(0 == '\0')
TPP_ASSERT(1 == '\1')
TPP_ASSERT(10 == '\n')
TPP_ASSERT(13 == '\r')
TPP_ASSERT(0x12 == '\x{12}')
TPP_ASSERT(0x123 == '\x{123}')
TPP_ASSERT(0xFFFF == '\x{FFFF}')
TPP_ASSERT(0x1234 == '\u1234')
TPP_ASSERT(0xFFFFAAAA == '\UFFFFAAAA')

/* Note however that using __TPP_EVAL("foo"[1]) actually extracts bytes! */
TPP_ASSERT_EXPANDS("7",   __TPP_EVAL(#"\UFFFFAAAA"))
TPP_ASSERT_EXPANDS("254", __TPP_EVAL("\UFFFFAAAA"[0]))
TPP_ASSERT_EXPANDS("131", __TPP_EVAL("\UFFFFAAAA"[1]))
TPP_ASSERT_EXPANDS("191", __TPP_EVAL("\UFFFFAAAA"[2]))
TPP_ASSERT_EXPANDS("191", __TPP_EVAL("\UFFFFAAAA"[3]))
TPP_ASSERT_EXPANDS("186", __TPP_EVAL("\UFFFFAAAA"[4]))
TPP_ASSERT_EXPANDS("170", __TPP_EVAL("\UFFFFAAAA"[5]))
TPP_ASSERT_EXPANDS("170", __TPP_EVAL("\UFFFFAAAA"[6]))
TPP_ASSERT_EXPANDS("0",   __TPP_EVAL("\UFFFFAAAA"[7]))


/* \x behaves differently though: it embeds *actual* bytes (rather than utf-8 sequences) */
TPP_ASSERT_EXPANDS("2",   __TPP_EVAL(#"\x{AA, FF}"))
TPP_ASSERT_EXPANDS("170", __TPP_EVAL("\x{AA, FF}"[0]))
TPP_ASSERT_EXPANDS("255", __TPP_EVAL("\x{AA, FF}"[1]))

/* If you try to embed something that's too big to fit, you get an error
 * `-Wbig-character`, while the actual character value is truncated.
 *
 * Actual compilers might then allow >1-byte strings (which in the case
 * of C/C++ is usually done via stuff like `L"foo"`, though since we're
 * language-agnostic, we don't- and can't know the width of such a string) */
TPP_ASSERT_WARNING_BEGIN("-Wbig-character")
TPP_ASSERT_EXPANDS("1", __TPP_EVAL(#"\x{107}"))
TPP_ASSERT_WARNING_END("-Wbig-character")
TPP_ASSERT_WARNING_BEGIN("-Wbig-character")
TPP_ASSERT_EXPANDS("7", __TPP_EVAL("\x{107}"[0]))
TPP_ASSERT_WARNING_END("-Wbig-character")
