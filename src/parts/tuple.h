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
#ifndef GUARD_TPP_TUPLE_H
#define GUARD_TPP_TUPLE_H 1

#include "api.h"

/*[[[tpp-begin]]]*/

/* Preprocessor tuples:
 * >> 0()                  // Empty tuple
 * >> 1(VALUE)             // 1-element tuple
 * >> 2(VALUE1, VALUE2)    // 2-element tuple
 * >> ...
 */

/*[[[deemon
final local N = 10;
function params(n) {
	return "abcdefghijklmnopqrstuvwxyz"[:n];
}
for (local n: [:N])
	print(f"#define _TPP_TUPLE_NE_{n}({", ".join(params(n))}) {n ? 1 : 0}");
for (local n: [:N])
	print(f"#define _TPP_TUPLE_SIZE_{n}({", ".join(params(n))}) {n}");
for (local n: [1:N]) {
	for (local i: [:n])
		print(f"#define _TPP_TUPLE_GET_{i}_{n}({", ".join(params(n))}) {params(n)[i]}");
}
for (local n: [:N]) {
	print(f"#define _TPP_TUPLE_FOREACH_{n}(tuple, sep, item, _){n ? " \\" : ""}");
	if (n >= 2) {
		print(f"	_TPP_TUPLE_FOREACH_{n - 1}(tuple, sep, item, _) \\");
		print(f"	sep(_, {n - 2}, TPP_TUPLE_GET(tuple, {n - 2}), {n - 1}, TPP_TUPLE_GET(tuple, {n - 1})) \\");
	}
	if (n >= 1)
		print(f"	item(_, {n - 1}, TPP_TUPLE_GET(tuple, {n - 1}))");
}
]]]*/
#define _TPP_TUPLE_NE_0() 0
#define _TPP_TUPLE_NE_1(a) 1
#define _TPP_TUPLE_NE_2(a, b) 1
#define _TPP_TUPLE_NE_3(a, b, c) 1
#define _TPP_TUPLE_NE_4(a, b, c, d) 1
#define _TPP_TUPLE_NE_5(a, b, c, d, e) 1
#define _TPP_TUPLE_NE_6(a, b, c, d, e, f) 1
#define _TPP_TUPLE_NE_7(a, b, c, d, e, f, g) 1
#define _TPP_TUPLE_NE_8(a, b, c, d, e, f, g, h) 1
#define _TPP_TUPLE_NE_9(a, b, c, d, e, f, g, h, i) 1
#define _TPP_TUPLE_SIZE_0() 0
#define _TPP_TUPLE_SIZE_1(a) 1
#define _TPP_TUPLE_SIZE_2(a, b) 2
#define _TPP_TUPLE_SIZE_3(a, b, c) 3
#define _TPP_TUPLE_SIZE_4(a, b, c, d) 4
#define _TPP_TUPLE_SIZE_5(a, b, c, d, e) 5
#define _TPP_TUPLE_SIZE_6(a, b, c, d, e, f) 6
#define _TPP_TUPLE_SIZE_7(a, b, c, d, e, f, g) 7
#define _TPP_TUPLE_SIZE_8(a, b, c, d, e, f, g, h) 8
#define _TPP_TUPLE_SIZE_9(a, b, c, d, e, f, g, h, i) 9
#define _TPP_TUPLE_GET_0_1(a) a
#define _TPP_TUPLE_GET_0_2(a, b) a
#define _TPP_TUPLE_GET_1_2(a, b) b
#define _TPP_TUPLE_GET_0_3(a, b, c) a
#define _TPP_TUPLE_GET_1_3(a, b, c) b
#define _TPP_TUPLE_GET_2_3(a, b, c) c
#define _TPP_TUPLE_GET_0_4(a, b, c, d) a
#define _TPP_TUPLE_GET_1_4(a, b, c, d) b
#define _TPP_TUPLE_GET_2_4(a, b, c, d) c
#define _TPP_TUPLE_GET_3_4(a, b, c, d) d
#define _TPP_TUPLE_GET_0_5(a, b, c, d, e) a
#define _TPP_TUPLE_GET_1_5(a, b, c, d, e) b
#define _TPP_TUPLE_GET_2_5(a, b, c, d, e) c
#define _TPP_TUPLE_GET_3_5(a, b, c, d, e) d
#define _TPP_TUPLE_GET_4_5(a, b, c, d, e) e
#define _TPP_TUPLE_GET_0_6(a, b, c, d, e, f) a
#define _TPP_TUPLE_GET_1_6(a, b, c, d, e, f) b
#define _TPP_TUPLE_GET_2_6(a, b, c, d, e, f) c
#define _TPP_TUPLE_GET_3_6(a, b, c, d, e, f) d
#define _TPP_TUPLE_GET_4_6(a, b, c, d, e, f) e
#define _TPP_TUPLE_GET_5_6(a, b, c, d, e, f) f
#define _TPP_TUPLE_GET_0_7(a, b, c, d, e, f, g) a
#define _TPP_TUPLE_GET_1_7(a, b, c, d, e, f, g) b
#define _TPP_TUPLE_GET_2_7(a, b, c, d, e, f, g) c
#define _TPP_TUPLE_GET_3_7(a, b, c, d, e, f, g) d
#define _TPP_TUPLE_GET_4_7(a, b, c, d, e, f, g) e
#define _TPP_TUPLE_GET_5_7(a, b, c, d, e, f, g) f
#define _TPP_TUPLE_GET_6_7(a, b, c, d, e, f, g) g
#define _TPP_TUPLE_GET_0_8(a, b, c, d, e, f, g, h) a
#define _TPP_TUPLE_GET_1_8(a, b, c, d, e, f, g, h) b
#define _TPP_TUPLE_GET_2_8(a, b, c, d, e, f, g, h) c
#define _TPP_TUPLE_GET_3_8(a, b, c, d, e, f, g, h) d
#define _TPP_TUPLE_GET_4_8(a, b, c, d, e, f, g, h) e
#define _TPP_TUPLE_GET_5_8(a, b, c, d, e, f, g, h) f
#define _TPP_TUPLE_GET_6_8(a, b, c, d, e, f, g, h) g
#define _TPP_TUPLE_GET_7_8(a, b, c, d, e, f, g, h) h
#define _TPP_TUPLE_GET_0_9(a, b, c, d, e, f, g, h, i) a
#define _TPP_TUPLE_GET_1_9(a, b, c, d, e, f, g, h, i) b
#define _TPP_TUPLE_GET_2_9(a, b, c, d, e, f, g, h, i) c
#define _TPP_TUPLE_GET_3_9(a, b, c, d, e, f, g, h, i) d
#define _TPP_TUPLE_GET_4_9(a, b, c, d, e, f, g, h, i) e
#define _TPP_TUPLE_GET_5_9(a, b, c, d, e, f, g, h, i) f
#define _TPP_TUPLE_GET_6_9(a, b, c, d, e, f, g, h, i) g
#define _TPP_TUPLE_GET_7_9(a, b, c, d, e, f, g, h, i) h
#define _TPP_TUPLE_GET_8_9(a, b, c, d, e, f, g, h, i) i
#define _TPP_TUPLE_FOREACH_0(tuple, sep, item, _)
#define _TPP_TUPLE_FOREACH_1(tuple, sep, item, _) \
	item(_, 0, TPP_TUPLE_GET(tuple, 0))
#define _TPP_TUPLE_FOREACH_2(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_1(tuple, sep, item, _) \
	sep(_, 0, TPP_TUPLE_GET(tuple, 0), 1, TPP_TUPLE_GET(tuple, 1)) \
	item(_, 1, TPP_TUPLE_GET(tuple, 1))
#define _TPP_TUPLE_FOREACH_3(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_2(tuple, sep, item, _) \
	sep(_, 1, TPP_TUPLE_GET(tuple, 1), 2, TPP_TUPLE_GET(tuple, 2)) \
	item(_, 2, TPP_TUPLE_GET(tuple, 2))
#define _TPP_TUPLE_FOREACH_4(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_3(tuple, sep, item, _) \
	sep(_, 2, TPP_TUPLE_GET(tuple, 2), 3, TPP_TUPLE_GET(tuple, 3)) \
	item(_, 3, TPP_TUPLE_GET(tuple, 3))
#define _TPP_TUPLE_FOREACH_5(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_4(tuple, sep, item, _) \
	sep(_, 3, TPP_TUPLE_GET(tuple, 3), 4, TPP_TUPLE_GET(tuple, 4)) \
	item(_, 4, TPP_TUPLE_GET(tuple, 4))
#define _TPP_TUPLE_FOREACH_6(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_5(tuple, sep, item, _) \
	sep(_, 4, TPP_TUPLE_GET(tuple, 4), 5, TPP_TUPLE_GET(tuple, 5)) \
	item(_, 5, TPP_TUPLE_GET(tuple, 5))
#define _TPP_TUPLE_FOREACH_7(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_6(tuple, sep, item, _) \
	sep(_, 5, TPP_TUPLE_GET(tuple, 5), 6, TPP_TUPLE_GET(tuple, 6)) \
	item(_, 6, TPP_TUPLE_GET(tuple, 6))
#define _TPP_TUPLE_FOREACH_8(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_7(tuple, sep, item, _) \
	sep(_, 6, TPP_TUPLE_GET(tuple, 6), 7, TPP_TUPLE_GET(tuple, 7)) \
	item(_, 7, TPP_TUPLE_GET(tuple, 7))
#define _TPP_TUPLE_FOREACH_9(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_8(tuple, sep, item, _) \
	sep(_, 7, TPP_TUPLE_GET(tuple, 7), 8, TPP_TUPLE_GET(tuple, 8)) \
	item(_, 8, TPP_TUPLE_GET(tuple, 8))
/*[[[end]]]*/

/* >> TPP_TUPLE_NONEMPTY(tuple)
 * expands to "0" if "tuple" is empty; otherwise, expands to "1" */
#define TPP_TUPLE_NONEMPTY(tuple)  _TPP_TUPLE_NONEMPTY(tuple)
#define _TPP_TUPLE_NONEMPTY(tuple) _TPP_TUPLE_NONEMPTY_(_TPP_TUPLE_NE_##tuple)
#define _TPP_TUPLE_NONEMPTY_(x)    x

/* >> TPP_TUPLE_IF_NONEMPTY(tuple, what, _)
 * expands to "what(_)" if "tuple" is non-empty; otherwise, expands to nothing */
#define TPP_TUPLE_IF_NONEMPTY(tuple, what, _) _TPP_TUPLE_IF_NONEMPTY(TPP_TUPLE_NONEMPTY(tuple), what, _)
#define _TPP_TUPLE_IF_NONEMPTY(if, what, _)   _TPP_TUPLE_IF_NONEMPTY_(if, what, _)
#define _TPP_TUPLE_IF_NONEMPTY_(if, what, _)  _TPP_TUPLE_IF_NONEMPTY_##if (what, _)
#define _TPP_TUPLE_IF_NONEMPTY_1(what, _)     what(_)
#define _TPP_TUPLE_IF_NONEMPTY_0(what, _)

/* >> TPP_TUPLE_IF_EMPTY(tuple, what, _)
 * expands to "what(_)" if "tuple" is empty; otherwise, expands to nothing */
#define TPP_TUPLE_IF_EMPTY(tuple, what, _) _TPP_TUPLE_IF_EMPTY(TPP_TUPLE_NONEMPTY(tuple), what, _)
#define _TPP_TUPLE_IF_EMPTY(if, what, _)   _TPP_TUPLE_IF_EMPTY_(if, what, _)
#define _TPP_TUPLE_IF_EMPTY_(if, what, _)  _TPP_TUPLE_IF_EMPTY_##if (what, _)
#define _TPP_TUPLE_IF_EMPTY_0(what, _)     what(_)
#define _TPP_TUPLE_IF_EMPTY_1(what, _)

/* >> TPP_TUPLE_SIZE(tuple)
 * expands to a decimal number describing the size of "tuple" */
#define TPP_TUPLE_SIZE(tuple)  _TPP_TUPLE_SIZE(tuple)
#define _TPP_TUPLE_SIZE(tuple) _TPP_TUPLE_SIZE_(_TPP_TUPLE_SIZE_##tuple)
#define _TPP_TUPLE_SIZE_(x)    x

/* >> TPP_TUPLE_GET(tuple, i)
 * expands to the i'th element of "tuple" */
#define TPP_TUPLE_GET(tuple, i)  _TPP_TUPLE_GET(tuple, i)
#define _TPP_TUPLE_GET(tuple, i) _TPP_TUPLE_GET_(_TPP_TUPLE_GET_##i##_##tuple)
#define _TPP_TUPLE_GET_(x)       x

/* >> TPP_TUPLE_FOREACH(tuple, sep, item, _)
 *    >> sep(_, prev_index, prev_value, next_index, next_value)
 *    >> item(_, index, value)
 * expands to:
 *    - item(_, 0, TPP_TUPLE_GET(tuple, 0))
 *    - sep(_, 0, TPP_TUPLE_GET(tuple, 0), 1, TPP_TUPLE_GET(tuple, 1))
 *    - item(_, 1, TPP_TUPLE_GET(tuple, 1))
 *    - sep(_, 1, TPP_TUPLE_GET(tuple, 1), 2, TPP_TUPLE_GET(tuple, 2))
 *    - item(_, 2, TPP_TUPLE_GET(tuple, 2))
 *    - ...
 *    - item(_, N-1, TPP_TUPLE_GET(tuple, N-1))
 */
#define TPP_TUPLE_FOREACH(tuple, sep, item, _)         _TPP_TUPLE_FOREACH(tuple, _TPP_TUPLE_SIZE(tuple), sep, item, _)
#define _TPP_TUPLE_FOREACH(tuple, size, sep, item, _)  _TPP_TUPLE_FOREACH_(tuple, size, sep, item, _)
#define _TPP_TUPLE_FOREACH_(tuple, size, sep, item, _) _TPP_TUPLE_FOREACH_##size(tuple, sep, item, _)

#define TPP_TUPLE_FOREACH_DUMMY_SEP(_, prev_index, prev_value, next_index, next_value) /* nothing */
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_TUPLE_H */
