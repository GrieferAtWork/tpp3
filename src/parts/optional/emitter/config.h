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

/* When enabled, any `TPP_TOK_SPACE`-token is emitted as an (appropriately long)
 * sequence of ` `-characters, rather than as an echo of the original token's
 * space characters (thereby normalizing any unicode whitespace or other control
 * characters to `U+0020 SPACE`). */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_SPACE
#define TPP_EMITTER_HAVE_NORMALIZE_SPACE TPP_CONF_FEAT1
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_SPACE */

/* When enabled, any `TPP_TOK_LF`-token is emitted as a `\n`-character, rather
 * than as an echo of the original token's linefeed bytes (thereby normalizing
 * any unicode linefeed, CR, or CRLF sequences to LF). */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_LF
#define TPP_EMITTER_HAVE_NORMALIZE_LF TPP_CONF_FEAT1
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_LF */

/* When enabled, any `TPP_TOK_ISSTRING`-token is emitted as a `TPP_TOK_C_STRING`
 * (or `TPP_TOK_C_CHAR`, when `TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS` is
 * enabled in the lexer) token (though only done if the desired target token
 * is enabled).
 *
 * In order to do this normalization, the string is decoded and re-encoded via
 * use of `tpp_lexer_decodestring()` and `tpp_token_encodestring()`, thereby
 * allowing a consumer of the preprocessor output to only have to support a
 * greatly reduced set of string tokens (and escape sequences) in order to
 * fully understand *any* kind of string token that may be produced by TPP. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_C_STRING
#define TPP_EMITTER_HAVE_NORMALIZE_C_STRING ((TPP_HAVE_TOK_C_STRING && TPP_HAVE_TOKEN_ENCODESTRING && TPP_HAVE_LEXER_DECODESTRING) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_C_STRING */

/* When enabled, normalize `\u`, `\U` and `\N` escape sequences in keywords
 * names to their actual utf-8 character representation (also causes BSE
 * sequences to be normalized, though if that's all you want (and not just
 * for keywords), you could also enable `TPP_EMITTER_HAVE_NORMALIZE_BSE`).
 *
 * This feature is also required to emit `__TPP_IDENTIFIER()` as the actual
 * identifier, rather than as a copy of the identifier itself.
 *
 * NOTE: Keywords containing unicode characters that aren't SYMCONT will
 *       still see those characters get escaped by `\U`, though that's the
 *       only escape sequence that will still be emitted (and thus needs
 *       to be understood by a consuming preprocessor).
 *
 * NOTE: The *empty* keyword (i.e. `__TPP_IDENTIFIER("")`) continues to
 *       be emitted as `__TPP_IDENTIFIER("")`, since there's no other way
 *       to write that identifier. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS
#define TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS ((TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */

/* Remove \-escaped line-feeds from generic tokens. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_BSE
#define TPP_EMITTER_HAVE_NORMALIZE_BSE (TPP_HAVE_BSE ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_BSE */

/* Normalize trigraph sequences in generic tokens. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS
#define TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS (TPP_HAVE_TRIGRAPHS ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */

/* Normalize digraph sequences in generic tokens. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS
#define TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS (TPP_HAVE_DIGRAPHS ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS */

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

/* TODO: Config to select between use of `# <linenum>` and `#line` */

/* TODO: Config to enable emission of 1/2/3/4 flags in `# <linenum>`-directives */

/* TODO: Configs for each of the CLI switches listed in "frontend.c" */


/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_CONFIG_H */
