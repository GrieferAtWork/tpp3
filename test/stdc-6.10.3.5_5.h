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

// ISO/IEC 9899:201x - 6.10.3.5 - 5

#define x      3
#define f(a)   f(x * (a))
#undef x
#define x      2
#define g      f
#define z      z[0]
#define h      g(~
#define m(a)   a(w)
#define w      0,1
#define t(a)   a
#define p()    int
#define q(x)   x
#define r(x,y) x ## y
#define str(x) # x

#if 1 /* NON-CONFORMANCE: z gets expanded multiple times, and f() gets
       * expanded 2x because its output is copied and then re-parsed.
       * Because TPP is text-based, there's not really any way to prevent
       * this sort of thing, since we have nowhere to save any sort of
       * extra state when a macro expansion is copied into a string
       * buffer... */
TPP_ASSERT_EXPANDS(
	"f(2 * (y+1)) + f(2 * (f(2 * (z[0][0][0])))) % f(2 * (2 * (0))) + t(1);",
	f(y+1) + f(f(z)) % t(t(g)(0) + t)(1);
)
#else /* This is what the C standard says the expansion should look like... */
TPP_ASSERT_EXPANDS(
	"f(2 * (y+1)) + f(2 * (f(2 * (z[0])))) % f(2 * (0)) + t(1);",
	f(y+1) + f(f(z)) % t(t(g)(0) + t)(1);
)
#endif


/* Please excuse the use of "__TPP_STR_DECOMPILE" here. It's necessary
 * here because the # of ( and ) characters isn't balanced, because this
 * part uses some open-parenthesis-in-another-macro shenanigans.
 *
 * But: you can be happy that TPP expands this just the way STDC wants! */
TPP_ASSERT_EXPANDS(
	"f(2 * (2+(3,4)-0,1)) | f(2 * (~ 5)) & f(2 * (0,1))^m(0,1);",
	__TPP_STR_DECOMPILE("g(x+(3,4)-w) | h 5) & m (f)^m(m);")
)

TPP_ASSERT_EXPANDS(
	"int i[] = { 1, 23, 4, 5,  };",
	p() i[q()] = { q(1), r(2,3), r(4,), r(,5), r(,) };
)
TPP_ASSERT_EXPANDS(
	"char c[2][6] = { \"hello\", \"\" };",
	char c[2][6] = { str(hello), str() };
)

#undef f
#undef x
#undef g
#undef z
#undef h
#undef m
#undef w
#undef t
#undef p
#undef q
#undef r
#undef str
