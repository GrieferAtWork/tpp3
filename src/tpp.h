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
#ifndef GUARD_TPP_AMALGAMATION_H
#ifdef TPP_DEFS
#include "parts/defs.h"
#else /* TPP_DEFS */
#define GUARD_TPP_AMALGAMATION_H 1

#include "parts/api.h"
#include "parts/tuple.h"
#include "parts/config.h"
#include "parts/error.h"
#include "parts/ctype.h"
#include "parts/string.h"
#include "parts/file-io.h"
#include "parts/time.h"
#include "parts/expr.h"
#include "parts/token.h"
#include "parts/features.h"
#include "parts/file.h"
#include "parts/macro.h"
#include "parts/preparse.h"
#include "parts/keyword.h"
#include "parts/extensions.h"
#include "parts/warnings.h"
#include "parts/sysinclude.h"
#include "parts/hooks.h"
#include "parts/lexer.h"
#endif /* !TPP_DEFS */
#endif /* !GUARD_TPP_AMALGAMATION_H */
/* clang-format on */
