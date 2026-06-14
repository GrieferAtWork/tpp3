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
/* clang-format off */
#ifndef GUARD_TPP_C
#define GUARD_TPP_C 1
#undef TPP_BUILDING
#define TPP_BUILDING 1
#ifndef TPP_INTERN_DECL
#define TPP_INTERN_DECL static
#define TPP_INTERN_IMPL static
#endif /* !TPP_INTERN_DECL */

# include "tpp-amalgamation.h"

#include "parts/error.c"
#include "parts/ctype.c"
#include "parts/string.c"
#include "parts/token-strtokenid.c"
#include "parts/token-reprtokenid.c"
#include "parts/token-encodestring.c"
#include "parts/file-io.c"
#include "parts/file.c"
#include "parts/keyword.c"
#include "parts/builtins.c"
#include "parts/macro.c"
#include "parts/extensions.c"
#include "parts/features.c"
#include "parts/warnings.c"
#include "parts/lexer.c"
#include "parts/lexer-warn.c"
#include "parts/expr.c"
#include "parts/lexer-yieldraw.c"
#include "parts/lexer-seek-rparen.c"
#include "parts/lexer-pp-define.c"
#include "parts/lexer-pp-pragma.c"
#include "parts/lexer-skipraw.c"
#include "parts/lexer-decodeint.c"
#include "parts/lexer-yieldpp.c"
#include "parts/lexer-yield-macro.c"
#include "parts/lexer-yield.c"
#include "parts/lexer-decodestring.c"
#include "parts/lexer-parseexpr.c"

#endif /* !GUARD_TPP_C */
/* clang-format on */
