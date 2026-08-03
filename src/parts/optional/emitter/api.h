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
#ifndef GUARD_TPP_OPTIONAL_EMITTER_API_H
#define GUARD_TPP_OPTIONAL_EMITTER_API_H 1

#undef TPP_AMALGAMATION_H
#define TPP_AMALGAMATION_H "../../../tpp.h"

/*[[[tpp-begin]]]*/
/* ========= Emitter =========
 *
 * Brief:
 * - Turns a sequence of tokens back into something that can be understood
 *   by an(other) preprocessor with only minimal capabilities.
 * - Retain (make sure it doesn't change) all information normally returned by:
 *   - tpp_file_getfilename(tpp_lexer_getlcfile(LEXER))
 *   - tpp_file_getstartlcinfo(tpp_lexer_getlcfile(LEXER))
 *   - tpp_file_getendlcinfo(tpp_lexer_getlcfile(LEXER))   (only if token-passthrough is enabled)
 * - One downside is that the emitter (must be) allowed to inject additional
 *   line-feed/whitespace tokens between any 2 regular tokens. This is required
 *   because the emitter must be able to emit `#line` directives whenever the
 *   effective line/column/filename changes between 2 adjacent tokens in such a
 *   way that those 2 tokens aren't adjacent.
 * - The emitter will also emit *magic* whitespace whenever 2 adjacent tokens
 *   require the presence of such to prevent accidental token concatenation:
 *   >> #define FOO() foo
 *   >> foo()bar
 *   emitted like this:
 *   >> foo
 *   >> #line 2
 *   >>    bar
 */

#ifndef TPP_AMALGAMATION_H
#define TPP_AMALGAMATION_H "tpp-amalgamation.h"
#endif /* !TPP_AMALGAMATION_H */
#include TPP_AMALGAMATION_H

#ifndef TPP_EMITTER_BUILDING
#define TPP_EMITTER_BUILDING 0
#endif /* !TPP_EMITTER_BUILDING */

/* Used to wrap an identifier that is considered to be internal to TPP
 * Identifiers wrapped as such should **NOT** be accessed directly.
 * Instead, the provided TPP APIs should be used. */
#ifndef TPP_EMITTER_INTERNAL
#if TPP_EMITTER_BUILDING
#define TPP_EMITTER_INTERNAL(x) x
#else /* TPP_EMITTER_BUILDING */
#define TPP_EMITTER_INTERNAL(x) _em_##x
#endif /* !TPP_EMITTER_BUILDING */
#endif /* !TPP_EMITTER_INTERNAL */

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_API_H */
