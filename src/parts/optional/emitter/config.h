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
#ifndef GUARD_TPP_OPTIONAL_EMITTER_CONFIG_H
#define GUARD_TPP_OPTIONAL_EMITTER_CONFIG_H 1

#include "api.h"

/*[[[tpp-begin]]]*/

/* Enable support for re-emission of unknown pragmas. Requires that the TPP core
 * is configured to allow runtime override of its `TPP_HAVE_UNKNOWN_PRAGMA_HOOK`
 * hook (since the emitter needs to be able to override that hook during its
 * initialization)
 *
 * Can be configured in one of 3 ways:
 * - `0`:  Disabled (unknown pragmas cause warnings and are not (re-)emitted
 * - `1`:  Enabled
 * - `-1`: Available (but not enabled by default)
 *
 * When not *Disabled*, can be turned on/off using:
 * - `tpp_emitter_set_reemit_unknown_pragma()`
 * - `tpp_emitter_enable_reemit_unknown_pragma()`
 * - `tpp_emitter_disable_reemit_unknown_pragma()`
 */
#ifndef TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
#define TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA \
	(TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) ? 1 : 0)
#endif /* !TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */

/* Inhibit emission of `#line` directives, as well as (re-)alignment
 * of the output stream in order to match source L/C info. When this
 * is enabled, it is *highly* suggested that you turn on emission of
 * SPACE+LF tokens in the source lexer, since otherwise the emitter
 * will (probably) put everything on 1 line.
 *
 * Can be used to implement the `-P` CLI switch */
#ifndef TPP_EMITTER_HAVE_NOLINE
#define TPP_EMITTER_HAVE_NOLINE TPP_CONF_FEAT0
#endif /* !TPP_EMITTER_HAVE_NOLINE */

/* When inside of a macro -- so-as to prevent every token from causing
 * another `#line`-directive being emitted, don't be too precise
 * in terms of *all* tokens needing to have the proper column:
 * ```c
 * #define my_macro  10+20+30+40
 * 5+my_macro+50
 * ```
 *
 * Without this (technically more correct):
 * ```c
 * 5+10
 * #line 2
 *   +
 * #line 2
 *   20
 * #line 2
 *   +
 * #line 2
 *   30
 * #line 2
 *   +
 * #line 2
 *   40      +50
 * ```
 *
 * With this:
 * ```c
 * 5+10+20+30+40
 * #line 2
 *           +50
 * ``` */
#ifndef TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES
#define TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES TPP_CONF_FEAT1
#endif /* !TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES */

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_CONFIG_H */
