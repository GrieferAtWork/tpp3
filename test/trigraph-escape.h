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

/* Assert that string-escape sequences using trigraphs work properly */

#pragma TPP warning(push, "-Wno-trigraphs")
TPP_ASSERT_EXPANDS("\"\\?\"",           __TPP_EVAL("\?"))
TPP_ASSERT_EXPANDS("\"\\n\"",           __TPP_EVAL("??/n"))
TPP_ASSERT_EXPANDS("\"\\\\n\"",         __TPP_EVAL("??/\n"))
TPP_ASSERT_EXPANDS("\"\\\\\"",          __TPP_EVAL("??/??/"))
TPP_ASSERT_EXPANDS("\"\\\\n\"",         __TPP_EVAL("??/??/n"))
TPP_ASSERT_EXPANDS("\"\\\??/\\n\"",     __TPP_EVAL("\??/??/n"))
TPP_ASSERT_EXPANDS("\"\\\??/\\\??/n\"", __TPP_EVAL("\??/\??/n"))
/* This is how you'd escape "??/" using only trigraphs:
 * "??/?" "??/?" "/"
 *     ?      ?   /   */
TPP_ASSERT_EXPANDS("\"\\\??/\"",        __TPP_EVAL("??/???/?/"))

/* Because the above is rather difficult to read, here's those same
 * assertions once again, but on a logical level, rather than having
 * to match the exact expansions used by TPP's internal encoder. */
TPP_ASSERT("?"             == "\?")
TPP_ASSERT("\n"            == "??/n")
TPP_ASSERT("\\n"           == "??/\n")
TPP_ASSERT("\\"            == "??/??/")
TPP_ASSERT("\\n"           == "??/??/n")
TPP_ASSERT("??" "/\n"      == "\??/??/n")
TPP_ASSERT("??" "/??" "/n" == "\??/\??/n")
TPP_ASSERT("??" "/"        == "??/???/?/")
#pragma TPP warning(pop)
