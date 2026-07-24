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

#pragma TPP extension(push, \
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


/* >> TPP_ASSERT(10 < 20)
 * >> TPP_ASSERT(10 < 20, "Go home math. You're drunk")
 *
 * Assert that `__TPP_EVAL(!!(<first-argument>))` expands to `1`
 * If that is not the case, trigger `#error`, in which case the
 * optional second argument is included in the error message. */
#define TPP_ASSERT(...) _TPP_ASSERT_##__VA_NARGS__(#!__VA_ARGS__)
#define _TPP_ASSERT_1(condition) \
	_TPP_ASSERT2(__TPP_EVAL(!!(#!condition)), #condition, "")
#define _TPP_ASSERT_2(condition, message) \
	_TPP_ASSERT2(__TPP_EVAL(!!(#!condition)), #condition, ": " #!message)
#define _TPP_ASSERT2(condition, condition_str, message_suffix) \
	_TPP_ASSERT3(condition, #!condition_str, #!message_suffix)
#define _TPP_ASSERT3(condition, condition_str, message_suffix) \
	_TPP_ASSERT4_##condition(condition_str, message_suffix)
#define _TPP_ASSERT4_1(condition_str, message_suffix)
#define _TPP_ASSERT4_0(condition_str, message_suffix) \
	TPP_FAIL("Assertion failed: " #!condition_str #!message_suffix)


/* >> TPP_ASSERT_WARNING("-Wmacros", ...)
 * Assert that the second argument `caused_by` triggers a warning
 * specified by the first argument during expansion. The warning
 * that was triggered is not printed, but it is an error if it is
 * not triggered, or is triggered more than once. */
#define TPP_ASSERT_WARNING(name, caused_by...) \
	TPP_ASSERT_WARNING_BEGIN(#!name)           \
	#!caused_by                                \
	TPP_ASSERT_WARNING_END(#!name)

/* Used to implement `TPP_ASSERT_WARNING()`, but can also be used stand-alone */
#define TPP_ASSERT_WARNING_BEGIN(name)                            \
	__pragma(TPP warning(push, enable: #!name, suppress: #!name)) \
	TPP_ASSERT(!__has_warning(#!name), "Warning " #name " could not be suppressed")
#define TPP_ASSERT_WARNING_END(name) \
	TPP_ASSERT(__has_warning(#!name), "Warning " #name " was not triggered") \
	__pragma(TPP warning(pop))

/* XXX: Turn on -Werror ? */

#pragma TPP extension(pop)
