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

/* ISO/IEC 9899:201x - 6.10.3.5 - 6 */


#define str(s)      # s
#define xstr(s)     str(s)
#define debug(s, t) printf("x" # s "= %d, x" # t "= %s", \
x##s,x##t)
#define INCFILE(n)  vers ## n
#define glue(a, b)  a ## b
#define xglue(a, b) glue(a, b)
#define HIGHLOW     "hello"
#define LOW         LOW ", world"
TPP_ASSERT_EXPANDS(
	"printf(\"x\" \"1\" \"= %d, x\" \"2\" \"= %s\", x1,x2);",
	debug(1, 2);
)
TPP_ASSERT_EXPANDS(
	"fputs(\"strncmp(\\\"abc\\\\0d\\\", \\\"abc\\\", \\\'\\\\4\\\') // this goes away\\n== 0\" \": @\\\\n\", s);",
	fputs(str(strncmp("abc\0d", "abc", '\4') // this goes away
== 0) str(: @\n), s);
)

TPP_ASSERT_EXPANDS(
	"\"misc/vers2.h\"",
	xstr(misc/INCFILE(2).h)
)

xstr(misc/INCFILE(2).h)
TPP_ASSERT(!defined(IN_MISC_VERS2_H))
#include xstr(misc/INCFILE(2).h)
TPP_ASSERT(defined(IN_MISC_VERS2_H))
#undef IN_MISC_VERS2_H

TPP_ASSERT_EXPANDS(
	"\"hello\";",
	glue(HIGH, LOW);
)
TPP_ASSERT_EXPANDS(
	"\"hello\" \", world\"",
	xglue(HIGH, LOW)
)
