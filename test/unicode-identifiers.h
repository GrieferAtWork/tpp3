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
#include "utils/test.h"

/* HINT: This file even uses NBSP (U+00A0) as it's space character.
 *       It also uses a couple of unicode linefeed characters instead
 *       of the usual LF (U+000A) character. */

#define ㄒ卩卩(ᗪㄖ乇丂) ㄩ几丨匚ㄖᗪ乇•ᗪㄖ乇丂•山ㄖ尺Ҝ
TPP_ASSERT_EXPANDS("ㄩ几丨匚ㄖᗪ乇•sure does•山ㄖ尺Ҝ", ㄒ卩卩(sure does))
TPP_ASSERT_EXPANDS("ㄩ几丨匚ㄖᗪ乇•sure\342\200\250does•山ㄖ尺Ҝ", ㄒ卩卩(sure does)) #undef ㄒ卩卩

TPP_ASSERT(!defined(ㄒ卩卩))
TPP_ASSERT(__LINE__ == 34) /* An editor that doesn't understand unicode linefeeds will show this as line #31 */
