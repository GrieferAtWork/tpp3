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

// Macro calling conventions

#define angle<x>   +x-
#define brace{x}   +x-
#define bracket[x] +x-
#define paren(x)   +x- // Original (Unchanged)

TPP_ASSERT_EXPANDS("+<>-", angle<<>>)
//angle<<<>>   // ERROR: EOF in macro argument list
TPP_ASSERT_EXPANDS("+{<}-", angle<{<}>)
TPP_ASSERT_EXPANDS("+[<]-", angle<[<]>)
TPP_ASSERT_EXPANDS("+(<)-", angle<(<)>)
TPP_ASSERT_EXPANDS("+{>}-", angle<{>}>)
TPP_ASSERT_EXPANDS("+[>]-", angle<[>]>)
TPP_ASSERT_EXPANDS("+(>)-", angle<(>)>)
//brace{<{>}   // ERROR: EOF in macro argument list
//brace{{{}}   // ERROR: EOF in macro argument list
TPP_ASSERT_EXPANDS("+[{]-", brace{[{]})
TPP_ASSERT_EXPANDS("+({)-", brace{({)})
TPP_ASSERT_EXPANDS("+[}]-", brace{[}]})
TPP_ASSERT_EXPANDS("+(})-", brace{(})})
//bracket[<[>] // ERROR: EOF in macro argument list
//bracket[{[}] // ERROR: EOF in macro argument list
//bracket[[[]] // ERROR: EOF in macro argument list
TPP_ASSERT_EXPANDS("+([)-", bracket[([)])
TPP_ASSERT_EXPANDS("+(])-", bracket[(])])
//paren(<(>)   // ERROR: EOF in macro argument list
//paren({(})   // ERROR: EOF in macro argument list
//paren([(])   // ERROR: EOF in macro argument list
//paren((())   // ERROR: EOF in macro argument list

#undef paren
#undef bracket
#undef brace
#undef angle

#define map_node<K,T> struct{K key; T value;}
#define point<T> struct{T x; T y;}
TPP_ASSERT_EXPANDS(
	"struct{struct{int x; int y;} x; struct{int x; int y;} y;} x;",
	point<point<int>> x;)
TPP_ASSERT_EXPANDS(
	"struct{std::string key; int value;} e;",
	map_node<std::string,int> e;)
#undef point
#undef map_node


/* Another cool example: you can use extra calling conventions
 * to have the preprocessor convert your C++ casts to C! */
#define __TPP_ARGS(...)   (__VA_ARGS__))
#define static_cast<...> ((__VA_ARGS__)__TPP_ARGS

TPP_ASSERT_EXPANDS(
	"int x = ((int)(100l));",
	int x = static_cast<int>(100l);)

#undef static_cast
#undef __TPP_ARGS
