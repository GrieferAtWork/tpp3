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

/* Any unknown feature should expand to "0" in all cases */
TPP_ASSERT_EXPANDS("0", __has_attribute(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_builtin(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_cpp_attribute(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_declspec_attribute(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_extension(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_feature(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_c_attribute(unknown_feature))

/* The legacy `#pragma TPP tpp_set_keyword_flags()` can still be used here
 * Note that this pragma is disabled by default, and is only available here
 * because unit tests are executed under `TPP_PROFILE_ALL` */
#pragma TPP tpp_set_keyword_flags("unknown_feature", 0x28) /* feature+declspec_attribute */
TPP_ASSERT_EXPANDS("0", __has_attribute(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_builtin(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_cpp_attribute(unknown_feature))
TPP_ASSERT_EXPANDS("1", __has_declspec_attribute(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_extension(unknown_feature))
TPP_ASSERT_EXPANDS("1", __has_feature(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_c_attribute(unknown_feature))

#pragma TPP tpp_set_keyword_flags("unknown_feature", 0)
TPP_ASSERT_EXPANDS("0", __has_attribute(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_builtin(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_cpp_attribute(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_declspec_attribute(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_extension(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_feature(unknown_feature))
TPP_ASSERT_EXPANDS("0", __has_c_attribute(unknown_feature))

/* TODO: Have a custom `#pragma TPP ...` to define expansions on a per-feature basis */
