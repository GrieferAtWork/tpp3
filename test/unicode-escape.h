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

TPP_ASSERT_EXPANDS("\"\\376\\203\\277\\277\\277\\277\\277\"", __TPP_EVAL("\Uffffffff"))
TPP_ASSERT_EXPANDS("\"\\376\\203\\277\\277\\277\\277\\277\"", __TPP_EVAL("\UFFFFFFFF"))
TPP_ASSERT_EXPANDS("\"\\376\\203\\277\\277\\277\\277\\277\"", __TPP_EVAL("\UFfFfFfFf"))
TPP_ASSERT_EXPANDS("\"￿ffff\"", __TPP_EVAL("\uffffffff"))
TPP_ASSERT_EXPANDS("\"￿FFFF\"", __TPP_EVAL("\uFFFFFFFF"))
TPP_ASSERT_EXPANDS("\"￿FfFf\"", __TPP_EVAL("\uFfFfFfFf"))
TPP_ASSERT_EXPANDS("\"ሴꯍEF\"", __TPP_EVAL("\u1234\uABCDEF"))

#pragma TPP extension(push, "-fno-string-escape-hex-big")
TPP_ASSERT_EXPANDS("\"\\377ffffff\"", __TPP_EVAL("\xffffffff"))
TPP_ASSERT_EXPANDS("\"\\377FFFFFF\"", __TPP_EVAL("\xFFFFFFFF"))
TPP_ASSERT_EXPANDS("\"\\377FfFfFf\"", __TPP_EVAL("\xFfFfFfFf"))
#pragma TPP extension(pop)
