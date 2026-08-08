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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_API_H
#define GUARD_TPP_OPTIONAL_MAKEFILE_API_H 1

#undef TPP_AMALGAMATION_H
#define TPP_AMALGAMATION_H "../../../tpp.h"

/*[[[tpp-begin]]]*/
/* ========= Makefile =========
 *
 * Brief:
 * - TODO
 */

#ifndef TPP_AMALGAMATION_H
#define TPP_AMALGAMATION_H "tpp-amalgamation.h"
#endif /* !TPP_AMALGAMATION_H */
#include TPP_AMALGAMATION_H

#ifndef TPP_MAKEFILE_BUILDING
#define TPP_MAKEFILE_BUILDING 0
#endif /* !TPP_MAKEFILE_BUILDING */

/* Used to wrap an identifier that is considered to be internal to TPP
 * Identifiers wrapped as such should **NOT** be accessed directly.
 * Instead, the provided TPP APIs should be used. */
#ifndef TPP_MAKEFILE_INTERNAL
#if TPP_MAKEFILE_BUILDING
#define TPP_MAKEFILE_INTERNAL(x) x
#else /* TPP_MAKEFILE_BUILDING */
#define TPP_MAKEFILE_INTERNAL(x) _mf_##x
#endif /* !TPP_MAKEFILE_BUILDING */
#endif /* !TPP_MAKEFILE_INTERNAL */

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_API_H */
