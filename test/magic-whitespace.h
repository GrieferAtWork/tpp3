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

/* Magic whitespace here */
TPP_ASSERT_EXPANDS("foo bar", SCAN(FOO()BAR))

/* No magic whitespace needed here. Don't get me wrong: you might
 * think that magic whitespace is needed, but it isn't because when
 * TPP parses `FOO()BAR`, it yields 2 tokens [foo][bar] without any
 * whitespace in-between; the whitespace only gets added when that
 * sequences of tokens is copied for the purpose of being reparsed,
 * like when expanded again during SCAN()! */
TPP_ASSERT_EXPANDS("foobar", FOO()BAR)

#define ADJ1(x)  x
#define ADJ(a,b) SCAN(ADJ1(a)b)

/* Here's a way you can directly test `tpp_lexer_require_whitespace()` */
TPP_ASSERT_EXPANDS("= =", ADJ(=, =))
TPP_ASSERT_EXPANDS("=foo", ADJ(=, foo))

/* Injection of whitespace is reactive to recognized tokens. */
#pragma TPP extension(push)
#pragma TPP extension("-fno-tok-equal_at")
TPP_ASSERT_EXPANDS("=@", ADJ(=, @))
#pragma TPP extension("-ftok-equal_at")
TPP_ASSERT_EXPANDS("= @", ADJ(=, @))
#pragma TPP extension(pop)

#undef ADJ
#undef ADJ1

#undef SCAN
#undef BAR
#undef FOO
