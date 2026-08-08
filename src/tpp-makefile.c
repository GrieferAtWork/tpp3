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
#ifndef GUARD_TPP_MAKEFILE_AMALGAMATION_C
#define GUARD_TPP_MAKEFILE_AMALGAMATION_C 1

#ifndef TPP_MAKEFILE_AMALGAMATION_H
#define TPP_MAKEFILE_AMALGAMATION_H "tpp-makefile-amalgamation.h"
#endif /* !TPP_MAKEFILE_AMALGAMATION_H */

/* Include "tpp-makefile-amalgamation.h" header... */
#ifndef GUARD_TPP_MAKEFILE_AMALGAMATION_H
#undef TPP_MAKEFILE_BUILDING
#define TPP_MAKEFILE_BUILDING 1
#include TPP_MAKEFILE_AMALGAMATION_H
#endif /* !GUARD_TPP_MAKEFILE_AMALGAMATION_H */

#if !TPP_MAKEFILE_BUILDING
#include "parts/optional/makefile/expose-internals.h"
#endif /* !TPP_MAKEFILE_BUILDING */

#include "parts/optional/makefile/mf-features.c"
#include "parts/optional/makefile/mf.c"
#include "parts/optional/makefile/mf-cli.c"
#endif /* !GUARD_TPP_MAKEFILE_AMALGAMATION_C */
/* clang-format on */
