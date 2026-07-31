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

#define FOO() foo
#define BAR   bar
#define SCAN(x) x
#pragma TPP extension(push, "-fno-magic-whitespace")
#define SCAN_NSP(x) x
#pragma TPP extension(pop)

/* Magic whitespace here */
TPP_ASSERT_EXPANDS("foo bar", SCAN(FOO()BAR))
TPP_ASSERT_EXPANDS("foobar", SCAN_NSP(FOO()BAR))

/* No magic whitespace needed here. Don't get me wrong: you might
 * think that magic whitespace is needed, but it isn't because when
 * TPP parses `FOO()BAR`, it yields 2 tokens [foo][bar] without any
 * whitespace in-between; the whitespace only gets added when that
 * sequences of tokens is copied for the purpose of being reparsed,
 * like when expanded again during SCAN()! */
TPP_ASSERT_EXPANDS("foobar", FOO()BAR)

#define ADJ1(x)  x
#define ADJ(a,b) ADJ1(a)b

/* Here's a way you can directly test `tpp_lexer_require_whitespace()` */
TPP_ASSERT_EXPANDS("= =", SCAN(ADJ(=, =)))
TPP_ASSERT_EXPANDS("=foo", SCAN(ADJ(=, foo)))

/* Injection of whitespace is reactive to recognized tokens. */
#pragma TPP extension(push)
#pragma TPP extension("-fno-tok-equal_at")
TPP_ASSERT_EXPANDS("=@", SCAN(ADJ(=, @)))
TPP_ASSERT_EXPANDS("=@", SCAN_NSP(ADJ(=, @)))
#pragma TPP extension("-ftok-equal_at")
TPP_ASSERT_EXPANDS("= @", SCAN(ADJ(=, @)))
TPP_ASSERT_EXPANDS("=@", SCAN_NSP(ADJ(=, @)))
#pragma TPP extension(pop)

/* Magic whitespace can also become necessary while building a macro
 * argument list, in which case it may be necessary to inject extra
 * whitespace across a file-pop within the argument list. */
#define CALL_SCAN()      SCAN(foo
#define CALL_SCAN_NSP()  SCAN_NSP(foo
#define CALL_SCAN2       CALL_SCAN()bar)
#define CALL_SCAN_NSP2   CALL_SCAN_NSP()bar)
TPP_ASSERT_EXPANDS("foo bar", CALL_SCAN2)
TPP_ASSERT_EXPANDS("foobar", CALL_SCAN_NSP2)
#undef CALL_SCAN_NSP2
#undef CALL_SCAN2
#undef CALL_SCAN_NSP
#undef CALL_SCAN

#undef ADJ
#undef ADJ1

#undef SCAN
#undef SCAN_NSP
#undef BAR
#undef FOO

/* Make sure that whitespace surrounding file-pops
 * in macro arguments is handled correctly */
#define STR1(x)    #x
#define STR2(x, y) #x
#define CALL_STR1_1   __TPP_STR_DECOMPILE("STR1(foo/*comment*/"))
#define CALL_STR1_2   __TPP_STR_DECOMPILE("STR1(foo/*comment*/")bar)
#define CALL_STR1_3   __TPP_STR_DECOMPILE("STR1(foo/*comment*/") bar)
#define CALL_STR1_4   __TPP_STR_DECOMPILE("STR1(foo/*comment*/") /**/ )
#define CALL_STR1_5   __TPP_STR_DECOMPILE("STR1(foo/*comment*/")bar /**/ )
#define CALL_STR1_6   __TPP_STR_DECOMPILE("STR1(foo/*comment*/") bar /**/ )
#define CALL_STR2_1   __TPP_STR_DECOMPILE("STR2(foo/*comment*/"), IGNORED)
#define CALL_STR2_2   __TPP_STR_DECOMPILE("STR2(foo/*comment*/")bar, IGNORED)
#define CALL_STR2_3   __TPP_STR_DECOMPILE("STR2(foo/*comment*/") bar, IGNORED)
#define CALL_STR2_4   __TPP_STR_DECOMPILE("STR2(foo/*comment*/") /**/ , IGNORED)
#define CALL_STR2_5   __TPP_STR_DECOMPILE("STR2(foo/*comment*/")bar /**/ , IGNORED)
#define CALL_STR2_6   __TPP_STR_DECOMPILE("STR2(foo/*comment*/") bar /**/ , IGNORED)
TPP_ASSERT_EXPANDS("\"foo\"", CALL_STR1_1)
TPP_ASSERT_EXPANDS("\"foo/*comment*/bar\"", CALL_STR1_2)
TPP_ASSERT_EXPANDS("\"foo/*comment*/ bar\"", CALL_STR1_3)
TPP_ASSERT_EXPANDS("\"foo\"", CALL_STR1_4)
TPP_ASSERT_EXPANDS("\"foo/*comment*/bar\"", CALL_STR1_5)
TPP_ASSERT_EXPANDS("\"foo/*comment*/ bar\"", CALL_STR1_6)
TPP_ASSERT_EXPANDS("\"foo\"", CALL_STR2_1)
TPP_ASSERT_EXPANDS("\"foo/*comment*/bar\"", CALL_STR2_2)
TPP_ASSERT_EXPANDS("\"foo/*comment*/ bar\"", CALL_STR2_3)
TPP_ASSERT_EXPANDS("\"foo\"", CALL_STR2_4)
TPP_ASSERT_EXPANDS("\"foo/*comment*/bar\"", CALL_STR2_5)
TPP_ASSERT_EXPANDS("\"foo/*comment*/ bar\"", CALL_STR2_6)
#undef CALL_STR1_1
#undef CALL_STR1_2
#undef CALL_STR1_3
#undef CALL_STR1_4
#undef CALL_STR1_5
#undef CALL_STR1_6
#undef CALL_STR2_1
#undef CALL_STR2_2
#undef CALL_STR2_3
#undef CALL_STR2_4
#undef CALL_STR2_5
#undef CALL_STR2_6
#undef STR1
#undef STR2

/* One final place where magic whitespace is required is in macros where
 * an empty argument might form a new token by accidentally pasting what
 * comes before/after that argument within the macro. */
#pragma TPP extension(push)
#pragma TPP extension("-ftok-plus_plus")
#define SUM3_1(a, b, c) a+b+c
#pragma TPP extension("-fno-magic-whitespace")
#define SUM3_2(a, b, c) a+b+c
#pragma TPP extension("-fno-tok-plus_plus")
#define SUM3_3(a, b, c) a+b+c
#pragma TPP extension(pop)

TPP_ASSERT_EXPANDS("10+20+30", SUM3_1(10, 20, 30))
TPP_ASSERT_EXPANDS("10+20+30", SUM3_2(10, 20, 30))
TPP_ASSERT_EXPANDS("10+20+30", SUM3_3(10, 20, 30))
//FIXME(not working):TPP_ASSERT_EXPANDS("10+ +30", SUM3_1(10, , 30))
TPP_ASSERT_EXPANDS("10++30", SUM3_2(10, , 30)) /* Doesn't inject extra whitespace because "-fmagic-whitespace" was turned off */
TPP_ASSERT_EXPANDS("10++30", SUM3_3(10, , 30)) /* Doesn't inject extra whitespace because "++" wasn't considered a singular token at the time */
