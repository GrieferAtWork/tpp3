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
#pragma once
#ifdef TPP_FAIL
#error "The `#pragma once` didn't work"
#endif /* TPP_FAIL */

#pragma extension(push, \
	"-fno-magic-whitespace", \
	"-fno-macro-argument-whitespace", \
	"-fdont-expand-macro-argument" \
)

#ifndef __pragma
#define __pragma(x) _Pragma(#x)
#endif /* !__pragma */

#define TPP_STR2(x) #x
#define TPP_STR(x) TPP_STR2(x)

#define TPP_CAT2(a, b) a##b
#define TPP_CAT(a, b) TPP_CAT2(a, b)

#define TPP_FAIL(mesg) \
	__pragma(error(#!mesg))
#define TPP_WARN(mesg) \
	__pragma(TPP extension(push, \
		"-fpragma-message-prints-location", \
		"-fpragma-message-omits-trailing-linefeed"\
	)) \
	__pragma(message(#!mesg)) \
	__pragma(TPP extension(pop))


/* >> #define foo 42
 * >> TPP_ASSERT_EXPANDS("42", foo)
 *
 * Assert that the second operand expands `unexpanded_actual` expands
 * to the first operand `expected_str` (after also being converted
 * to a string) */
#define TPP_ASSERT_EXPANDS(expected_str, unexpanded_actual...) \
	_TPP_ASSERT_EXPANDS1(#!expected_str, #unexpanded_actual, unexpanded_actual)
#define _TPP_ASSERT_EXPANDS1(expected_str, unexpanded_str, expanded_actual...) \
	_TPP_ASSERT_EXPANDS2(#!expected_str, #!unexpanded_str, #expanded_actual)
#define _TPP_ASSERT_EXPANDS2(expected_str, unexpanded_str, actual_str) \
	_TPP_ASSERT_EXPANDS3(__TPP_EVAL(#!expected_str == #!actual_str), \
	                     #!expected_str, #!unexpanded_str, #!actual_str)
#define _TPP_ASSERT_EXPANDS3(ok, expected_str, unexpanded_str, actual_str) \
	_TPP_ASSERT_EXPANDS4(ok, #!expected_str, #!unexpanded_str, #!actual_str)
#define _TPP_ASSERT_EXPANDS4(ok, expected_str, unexpanded_str, actual_str) \
	_TPP_ASSERT_EXPANDS_##ok(#!expected_str, #!unexpanded_str, #!actual_str)
#define _TPP_ASSERT_EXPANDS_0(expected_str, unexpanded_str, actual_str) \
	TPP_FAIL("Expected " #unexpanded_str " to expand to " #expected_str \
	         " but actually expands to " #actual_str)
#define _TPP_ASSERT_EXPANDS_1(expected_str, unexpanded_str, actual_str)


#pragma extension(pop)
