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
#pragma TPP extension("-fstring-escape-format-paren")   /* Enable "foo: \(expr)" */
#pragma TPP extension("-fstring-escape-format-brace")   /* Enable "foo: \{expr}" */
#pragma TPP extension("-fstring-escape-format-bracket") /* Enable "foo: \[expr]" */
#pragma TPP extension("-ftok-format-string-literal")    /* Enable f"foo: {expr}" */
#pragma TPP extension("-ftok-format-char-literal")      /* Enable f'foo: {expr}' */
#pragma TPP extension("-ftok-format-backtick-literal")  /* Enable `foo: ${expr}` */
#pragma TPP extension("-fno-character-literals")        /* Treat 'foo' as a string */
#pragma TPP extension("-fstring-escape-format-builtin") /* Use builtin expression parser */

#define VALUE "bar"[3-2:] + "YO"

/* Test all the different types of format string literals.
 * Note that TPP is smart enough to allow *any* kind of tokens to appear
 * within format-expressions nested inside of strings. As such, you don't
 * have to worry about using different quoting styles (like you'd need to
 * worry about in python) */
#define ASSERT_STRING(s)                            \
	TPP_ASSERT_EXPANDS("1", __TPP_COUNT_TOKENS(#s)) \
	TPP_ASSERT_EXPANDS("\"foo: HarYOT, after\"", __TPP_EVAL(s))

/* Everything below evaluates to the string "foo: HarYOT, after" */
ASSERT_STRING("foo: \("H" + VALUE + "T"), after")
ASSERT_STRING("foo: \{"H" + VALUE + "T"}, after")
ASSERT_STRING("foo: \["H" + VALUE + "T"], after")
ASSERT_STRING('foo: \("H" + VALUE + "T"), after')
ASSERT_STRING('foo: \{"H" + VALUE + "T"}, after')
ASSERT_STRING('foo: \["H" + VALUE + "T"], after')
ASSERT_STRING(f"foo: {"H" + VALUE + "T"}, after")
ASSERT_STRING(f'foo: {"H" + VALUE + "T"}, after')
ASSERT_STRING(`foo: ${"H" + VALUE + "T"}, after`)

ASSERT_STRING("foo: \('H' + VALUE + 'T'), after")
ASSERT_STRING("foo: \{'H' + VALUE + 'T'}, after")
ASSERT_STRING("foo: \['H' + VALUE + 'T'], after")
ASSERT_STRING('foo: \('H' + VALUE + 'T'), after')
ASSERT_STRING('foo: \{'H' + VALUE + 'T'}, after')
ASSERT_STRING('foo: \['H' + VALUE + 'T'], after')
ASSERT_STRING(f"foo: {'H' + VALUE + 'T'}, after")
ASSERT_STRING(f'foo: {'H' + VALUE + 'T'}, after')
ASSERT_STRING(`foo: ${'H' + VALUE + 'T'}, after`)

ASSERT_STRING("foo: \(f"H{""}" + VALUE + f"{"xT"[1:]}"), after")
ASSERT_STRING("foo: \{f"H{""}" + VALUE + f"{"xT"[1:]}"}, after")
ASSERT_STRING("foo: \[f"H{""}" + VALUE + f"{"xT"[1:]}"], after")
ASSERT_STRING('foo: \(f"H{""}" + VALUE + f"{"xT"[1:]}"), after')
ASSERT_STRING('foo: \{f"H{""}" + VALUE + f"{"xT"[1:]}"}, after')
ASSERT_STRING('foo: \[f"H{""}" + VALUE + f"{"xT"[1:]}"], after')
ASSERT_STRING(f"foo: {f"H{""}" + VALUE + f"{"xT"[1:]}"}, after")
ASSERT_STRING(f'foo: {f"H{""}" + VALUE + f"{"xT"[1:]}"}, after')
ASSERT_STRING(`foo: ${f"H{""}" + VALUE + f"{"xT"[1:]}"}, after`)

ASSERT_STRING("foo: \(`H${""}` + VALUE + `${"xT"[1:]}`), after")
ASSERT_STRING("foo: \{`H${""}` + VALUE + `${"xT"[1:]}`}, after")
ASSERT_STRING("foo: \[`H${""}` + VALUE + `${"xT"[1:]}`], after")
ASSERT_STRING('foo: \(`H${""}` + VALUE + `${"xT"[1:]}`), after')
ASSERT_STRING('foo: \{`H${""}` + VALUE + `${"xT"[1:]}`}, after')
ASSERT_STRING('foo: \[`H${""}` + VALUE + `${"xT"[1:]}`], after')
ASSERT_STRING(f"foo: {`H${""}` + VALUE + `${"xT"[1:]}`}, after")
ASSERT_STRING(f'foo: {`H${""}` + VALUE + `${"xT"[1:]}`}, after')
ASSERT_STRING(`foo: ${`H${""}` + VALUE + `${"xT"[1:]}`}, after`)

/* Format expressions are even allowed to contain linefeeds! */
ASSERT_STRING("foo: \(
	f"H{
		""
	}" +
	VALUE +
	f"{
		"xT"[1:]
	}ignored"[:1]
), after")

#undef ASSERT_STRING
#undef VALUE

#pragma TPP extension(pop)
