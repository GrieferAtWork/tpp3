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

#define REDEF(m, b)  __pragma(tpp_exec("#undef " #m "\n#define " #m " " #b))
#define Word  w1
#define w1    REDEF(Word, w2)Hello
#define w2    REDEF(Word, w3)There
#define w3    REDEF(Word, w4)Good
#define w4    REDEF(Word, w1)Sir!

TPP_ASSERT_EXPANDS("Hello There Good Sir!", Word Word Word Word)
TPP_ASSERT_EXPANDS("Hello", Word)
TPP_ASSERT_EXPANDS("There", Word)
TPP_ASSERT_EXPANDS("Good", Word)
TPP_ASSERT_EXPANDS("Sir!", Word)
TPP_ASSERT_EXPANDS("Hello There", Word Word)
TPP_ASSERT_EXPANDS("Good Sir!", Word Word)

#undef w4
#undef w3
#undef w2
#undef w1
#undef Word
#undef REDEF
