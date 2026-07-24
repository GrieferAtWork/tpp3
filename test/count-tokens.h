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

TPP_ASSERT_EXPANDS("0", __TPP_COUNT_TOKENS(""))
TPP_ASSERT_EXPANDS("1", __TPP_COUNT_TOKENS("foo"))
TPP_ASSERT_EXPANDS("2", __TPP_COUNT_TOKENS("foo+"))
TPP_ASSERT_EXPANDS("3", __TPP_COUNT_TOKENS("foo[]"))

#pragma TPP extension(push)
#pragma TPP extension("-ftok-lf")
TPP_ASSERT_EXPANDS("1", __TPP_COUNT_TOKENS("\n"))
#pragma TPP extension("-fno-tok-lf")
TPP_ASSERT_EXPANDS("0", __TPP_COUNT_TOKENS("\n"))

#pragma TPP extension("-ftok-space")
TPP_ASSERT_EXPANDS("1", __TPP_COUNT_TOKENS(" "))
#pragma TPP extension("-fno-tok-space")
TPP_ASSERT_EXPANDS("0", __TPP_COUNT_TOKENS(" "))

#pragma TPP extension("-ftok-comment")
TPP_ASSERT_EXPANDS("1", __TPP_COUNT_TOKENS("/* foo */"))
#pragma TPP extension("-fno-tok-comment")
TPP_ASSERT_EXPANDS("0", __TPP_COUNT_TOKENS("/* foo */"))
#pragma TPP extension(pop)
