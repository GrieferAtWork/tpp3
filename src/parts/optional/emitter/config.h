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

/* Profile configuration for `TPP_EMITTER_*` defaults. */
#ifndef TPP_EMITTER_PROFILE
#define TPP_EMITTER_PROFILE TPP_PROFILE
#endif /* !TPP_EMITTER_PROFILE */

/* Provide support for `TPP_EMITTER_MODE_EMIT`, which emits tokens
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_MODE_EMIT
#define TPP_EMITTER_HAVE_MODE_EMIT 1
#endif /* !TPP_EMITTER_HAVE_MODE_EMIT */

/* Provide support for `TPP_EMITTER_MODE_DISPOSE`, which discards tokens
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_MODE_DISPOSE
#define TPP_EMITTER_HAVE_MODE_DISPOSE (TPP_EMITTER_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_EMITTER_HAVE_MODE_DISPOSE */

/* Provide support for `TPP_EMITTER_MODE_BRACKET`, where
 * tokens are emitted surrounded by brackets.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_MODE_BRACKET
#define TPP_EMITTER_HAVE_MODE_BRACKET (TPP_EMITTER_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_EMITTER_HAVE_MODE_BRACKET */

/* Provide support for `TPP_EMITTER_MODE_TYPED`, where tokens are
 * emitted surrounded as `[{TYPE}:{TOKEN}]`, where `TYPE` is the
 * result of `tpp_strtokenid()` and the canonical keyword name.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_MODE_TYPED
#define TPP_EMITTER_HAVE_MODE_TYPED (TPP_HAVE_STRTOKENID && (TPP_EMITTER_PROFILE == TPP_PROFILE_ALL))
#endif /* !TPP_EMITTER_HAVE_MODE_TYPED */

/* Provide support for `TPP_EMITTER_MODE_ZERO`, where tokens are
 * emitted in their normalized form (see `TPP_EMITTER_HAVE_NORMALIZE_*`),
 * with every token followed by a NUL-byte. This format is primarily
 * meant for preprocessing small files such that another process can
 * (fairly easily) consume those tokens without having to worry about
 * the boundaries between tokens.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_MODE_ZERO
#define TPP_EMITTER_HAVE_MODE_ZERO (TPP_EMITTER_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_EMITTER_HAVE_MODE_ZERO */

#undef TPP_EMITTER_HAVE_EMIT_TOKEN
#if (TPP_EMITTER_HAVE_MODE_EMIT ||    \
     TPP_EMITTER_HAVE_MODE_BRACKET || \
     TPP_EMITTER_HAVE_MODE_TYPED ||   \
     TPP_EMITTER_HAVE_MODE_ZERO)
#define TPP_EMITTER_HAVE_EMIT_TOKEN 1
#else /* ... */
#define TPP_EMITTER_HAVE_EMIT_TOKEN 0
#endif /* !... */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any `TPP_TOK_SPACE`-token
 * is emitted as an (appropriately long) sequence of ` `-characters, rather
 * than as an echo of the original token's space characters (thereby normalizing
 * any unicode whitespace or other control characters to `U+0020 SPACE`).
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_SPACE
#define TPP_EMITTER_HAVE_NORMALIZE_SPACE (TPP_EMITTER_HAVE_EMIT_TOKEN ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_SPACE */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any `TPP_TOK_LF`-token is
 * emitted as a `\n`-character, rather than as an echo of the original token's
 * linefeed bytes (thereby normalizing any unicode linefeed, CR, or CRLF
 * sequences to LF).
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_LF
#define TPP_EMITTER_HAVE_NORMALIZE_LF (TPP_EMITTER_HAVE_EMIT_TOKEN ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_LF */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any `TPP_TOK_ISSTRING`-token
 * is emitted as a `TPP_TOK_C_STRING` (or `TPP_TOK_C_CHAR`, when
 * `TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS` is enabled in the lexer) token
 * (though only done if the desired target token is enabled).
 *
 * In order to do this normalization, the string is decoded and re-encoded via
 * use of `tpp_lexer_decodestring()` and `tpp_token_encodestring()`, thereby
 * allowing a consumer of the preprocessor output to only have to support a
 * greatly reduced set of string tokens (and escape sequences) in order to
 * fully understand *any* kind of string token that may be produced by TPP.
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_C_STRING
#define TPP_EMITTER_HAVE_NORMALIZE_C_STRING ((TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_TOK_C_STRING && TPP_HAVE_TOKEN_ENCODESTRING && TPP_HAVE_LEXER_DECODESTRING) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_C_STRING */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any
 * `TPP_TOK_ISINT`-token is emitted as a `TPP_TOK_C_INT` token.
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_C_INT
#define TPP_EMITTER_HAVE_NORMALIZE_C_INT ((TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_TOK_C_INT && TPP_HAVE_LEXER_DECODEINT) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_C_INT */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, normalize `\u`, `\U`
 * and `\N` escape sequences in keywords names to their actual utf-8 character
 * representation (also causes BSE sequences to be normalized, though if that's
 * all you want (and not just for keywords), you could also enable
 * `TPP_EMITTER_HAVE_NORMALIZE_BSE`).
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
 *       to write that identifier.
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS
#define TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS ((TPP_EMITTER_HAVE_EMIT_TOKEN && (TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED)) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, remove `\`-escaped
 * line-feeds from generic tokens.
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_BSE
#define TPP_EMITTER_HAVE_NORMALIZE_BSE ((TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_BSE) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_BSE */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, normalize trigraph
 * sequences in generic tokens.
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS
#define TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS ((TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_TRIGRAPHS) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, normalize digraph
 * sequences in generic tokens.
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS
#define TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS ((TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_DIGRAPHS) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, inhibit emission of
 * `#line` directives, as well as (re-)alignment of the output stream in
 * order to match source L/C info. When this is enabled, it is *highly*
 * suggested that you turn on emission of SPACE+LF tokens in the source
 * lexer, since otherwise the emitter will (probably) put everything on
 * 1 line.
 *
 * Can be used to implement the `-P` CLI switch
 * (see `TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS`)
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_NOLINE
#define TPP_EMITTER_HAVE_NOLINE (TPP_EMITTER_HAVE_MODE_EMIT ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_EMITTER_HAVE_NOLINE */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, tokens emitted
 * from within a macro do not require proper alignment with `__COLUMN__`.
 *
 * So-as to prevent every token from causing another `#line`-directive
 * being emitted, don't be too precise in terms of *all* tokens needing
 * to have the proper column:
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
 * ```
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN
#define TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN ((TPP_EMITTER_HAVE_MODE_EMIT && TPP_HAVE_CPP_MACROS) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN */

/* Use `# <linenum>` instead of `#line` to align tokens
 *
 * s.a.:
 * - `TPP_HAVE_CPP_DIGIT_LINE`
 * - `TPP_HAVE_CPP_LINE`
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_USE_CPP_DIGIT
#define TPP_EMITTER_HAVE_USE_CPP_DIGIT (TPP_EMITTER_HAVE_MODE_EMIT ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT */

/* Extension to `TPP_EMITTER_HAVE_USE_CPP_DIGIT`: also emit `1/2/3/4` flags:
 *
 * - `1`: Push a dummy-file containing the old file/line/column onto the `#include`-stack,
 *        before applying the new line/filename.
 * - `2`: Do the inverse of flag `1` and pop a dummy-file off the `#include`-stack.
 * - `3`: Set `TPP_FILE_FLAGS_SYSHDR` for the current text-file. When this flag is not
 *        supplied, `TPP_FILE_FLAGS_SYSHDR` is instead cleared for the current text-file.
 * - `4`: Same as flag `3`, except for the `TPP_FILE_FLAGS_EXTERN_C` flag.
 *
 * s.a. `TPP_HAVE_CPP_DIGIT_LINE`
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#define TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS ((TPP_EMITTER_HAVE_USE_CPP_DIGIT && TPP_HAVE_FILE_PUSHED_HOOK && TPP_HAVE_FILE_POPPED_HOOK && (TPP_HAVE_FILE_SYSHDR || TPP_HAVE_FILE_EXTERN_C)) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */

/* Extension to `TPP_EMITTER_HAVE_USE_CPP_DIGIT`:
 * the first time a `# <linenum>` directive is emitted,
 * emit it 2 times, and in-between those to emissions,
 * emit another line `# <linenum> "$(PWD)//"`:
 *
 * ```c
 * # 1 "input.c"
 * # 1 "/opt/my-project//"
 * # 1 "input.c"
 * ```
 *
 * A(nother) preprocessor consuming TPP's emitter output
 * will then know how relative filenames can be resolved
 * if the intend is to turn them into absolute paths.
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
#define TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY (TPP_EMITTER_HAVE_USE_CPP_DIGIT ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */

#if !TPP_EMITTER_HAVE_USE_CPP_DIGIT && !TPP_IGNORE_INVALID_CONFIGURATION
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#error "Config 'TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS' being enabled doesn't make sense when '-DTPP_EMITTER_HAVE_USE_CPP_DIGIT=0'"
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
#error "Config 'TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY' being enabled doesn't make sense when '-DTPP_EMITTER_HAVE_USE_CPP_DIGIT=0'"
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT && !TPP_IGNORE_INVALID_CONFIGURATION */

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
 * - `tpp_emitter_get_reemit_unknown_pragma()`
 * - `tpp_emitter_set_reemit_unknown_pragma()`
 * - `tpp_emitter_enable_reemit_unknown_pragma()`
 * - `tpp_emitter_disable_reemit_unknown_pragma()` */
#ifndef TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
#define TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA \
	(TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) ? 1 : 0)
#endif /* !TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */

/* Enable support for re-emission of `#define` and `#undef` directives.
 * Requires that the TPP core is configured to allow runtime override of
 * its `TPP_HAVE_MACRO_DEFINED_HOOK` and `TPP_HAVE_MACRO_UNDEFINED_HOOK`
 * hooks (since the emitter needs to be able to override these hook during
 * its initialization)
 *
 * Can be configured in one of 3 ways:
 * - `0`:  Disabled 
 * - `1`:  Enabled (`#define`/`#undef` are re-emitted)
 * - `-1`: Available (but not enabled by default)
 *
 * When not *Disabled*, can be turned on/off using:
 * - `tpp_emitter_get_reemit_macro_definitions()`
 * - `tpp_emitter_set_reemit_macro_definitions()`
 * - `tpp_emitter_enable_reemit_macro_definitions()`
 * - `tpp_emitter_disable_reemit_macro_definitions()` */
#ifndef TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
#define TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS   \
	((TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK) && \
	  TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK)) \
	 ? -1                                           \
	 : 0)
#endif /* !TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

/* Similar end result to `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS`,
 * but taking a completely different approach in order to get there:
 * - Hook `TPP_HAVE_FILE_PUSHED_HOOK` to get informed whenever a file
 *   is pushed onto the `#include`-stack. If that file turns out to
 *   be a macro, see if that macro's most-recent definition has already
 *   been dumped.
 *   - If not, or if the macro's definition has changed, dump it now.
 *     If there was a different definition, emit a `#undef` first.
 * - Whenever a `TPP_TOK_ISKEYWORD()`-token is emitted (`tpp_emitter_emitcurrent()`
 *   is called while a keyword-token is loaded into the lexer), and the
 *   linked keyword doesn't have a user-defined macro definition (i.e.
 *   `!tpp_keyword_hasmacro()`), check what was most-recently emitted
 *   about that keyword in regards to macro definitions:
 *   - If the thing that was last emitted was a `#define`-directive, then
 *     emit a `#undef`-directive and delete the saved macro definition.
 * - In order to remember the *most-recently-dumped* macro definition
 *   linked to a keyword, `TPP_HAVE_KEYWORD_USERDATA` is used to store
 *   a reference to the `tpp_macro` that was most-recently dumped
 *
 * NOTE: In order to determine the name of the macro when it is used
 *       as a result of being expanded onto the #include-stack, this
 *       feature also requires `TPP_HAVE_MACRO_NAME` to be enabled.
 *
 * Because this feature also requires a hook, it must be turned on
 * using the following APIs, rather than directly setting its feature:
 * - `tpp_emitter_get_reemit_macro_definitions_lazy()`
 * - `tpp_emitter_set_reemit_macro_definitions_lazy()`
 * - `tpp_emitter_enable_reemit_macro_definitions_lazy()`
 * - `tpp_emitter_disable_reemit_macro_definitions_lazy()`
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
#define TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY  \
	((TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) &&       \
	  TPP_HAVE_KEYWORD_USERDATA && TPP_HAVE_MACRO_NAME) \
	 ? TPP_CONF_FEAT0                                   \
	 : 0)
#endif /* !TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

/* Extension to `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS` and
 * `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY`: when emitting
 * `#define` directives, only emit `#define <MACRO_NAME>`, excluding
 * the macro's actual definition.
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY
#define TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY ((TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS || TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY */

/* Enable support for re-emission of `#include`, `#include_next`, `#import`
 * and `#embed` directives.
 *
 * Requires that the TPP core is configured to allow runtime override of
 * its `TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK` hook (since the emitter needs
 * to be able to override that hook during its initialization)
 *
 * Can be configured in one of 3 ways:
 * - `0`:  Disabled 
 * - `1`:  Enabled (`#include` are re-emitted)
 * - `-1`: Available (but not enabled by default)
 *
 * When not *Disabled*, can be turned on/off using:
 * - `tpp_emitter_get_reemit_include_directives()`
 * - `tpp_emitter_set_reemit_include_directives()`
 * - `tpp_emitter_enable_reemit_include_directives()`
 * - `tpp_emitter_disable_reemit_include_directives()` */
#ifndef TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
#define TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES  \
	(TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) ? -1 : 0)
#endif /* !TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */

/* Trace includes (and the depth of the `#include`-stack in terms of IO files)
 * by emitting a line like the following to `tpp_lexer_gethook_mesgprinter()`
 * whenever an I/O file is pushed to the `#include`-stack:
 * ```deemon
 * print("." * NUMBER_OF_IO_FILES_ON_INCLUDE_STACK, " ", tpp_file_getrealfilename(file));
 * ```
 *
 * Because this feature uses the `TPP_HAVE_FILE_PUSHED_HOOK` hook, it
 * must be turned on using the following APIs, rather than directly
 * setting its feature:
 * - `tpp_emitter_get_trace_includes()`
 * - `tpp_emitter_set_trace_includes()`
 * - `tpp_emitter_enable_trace_includes()`
 * - `tpp_emitter_disable_trace_includes()`
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_EMITTER_HAVE_TRACE_INCLUDES
#define TPP_EMITTER_HAVE_TRACE_INCLUDES ((TPP_HAVE_FILE_PUSHED_HOOK && TPP_HAVE_MESGPRINTER_HOOK) ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_EMITTER_HAVE_TRACE_INCLUDES */

/* The max # of blank adjustment lines before the emitter will
 * instead emit a(nother) `#line` (or `# <linenum>`) directive.
 *
 * When negative, the threshold is configurable at runtime.
 * When configured as `0`, the threshold becomes infinite (at
 * runtime, use `tpp_emitter_disablelinethreshold()` for this)
 *
 * **Getter**: `tpp_emitter_getlinethreshold(emitter)`<br/>
 * **Setter**: `tpp_emitter_setlinethreshold(emitter, v)` */
#ifndef TPP_EMITTER_CONFIG_LINE_THRESHOLD
#define TPP_EMITTER_CONFIG_LINE_THRESHOLD ((TPP_EMITTER_PROFILE != TPP_PROFILE_MINIMAL) ? -4 : 4)
#endif /* !TPP_EMITTER_CONFIG_LINE_THRESHOLD */

/* Provide an API `tpp_io_printpwd()` that can be used to print the hosting
 * process's current working directory to a given `tpp_formatprinter`.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_IO_PRINTPWD
#define TPP_EMITTER_HAVE_IO_PRINTPWD (TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
#endif /* !TPP_EMITTER_HAVE_IO_PRINTPWD */




/************************************************************************/
/* EMITTER CLI CONFIG                                                   */
/************************************************************************/

/* Enable support for `tpp_emitter_cli_loader`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI
#define TPP_EMITTER_HAVE_CLI TPP_HAVE_CLI
#endif /* !TPP_EMITTER_HAVE_CLI */

/* Enable support for `tpp_emitter_cli_loader_help`, which exposes a small
 * database of supported commandline flags in a human-readable format that
 * can also be rendered (fairly) easily.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_HELP
#define TPP_EMITTER_HAVE_CLI_HELP (TPP_EMITTER_HAVE_CLI && TPP_HAVE_CLI_HELP)
#endif /* !TPP_EMITTER_HAVE_CLI_HELP */

/* Include extra spellings (i.e.: in addition to the primary spelling) of CLI options.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS
#define TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS (TPP_EMITTER_HAVE_CLI_HELP && (TPP_EMITTER_PROFILE != TPP_PROFILE_MINIMAL))
#endif /* !TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS */

/* `-P`, `--no-line-commands`:
 * Disable emission of `#line`-directives, but also turn
 * on emission of SPACE/LF tokens (if runtime-configurable).
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
#define TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_NOLINE)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */

/* `-dM`, `--dump=M`:
 * Dump builtin/predefined macros to the emitters output during the
 * CLI flush phase (i.e.: when `tpp_emitter_cli_loader_flush()` is called):
 * `tpp_lexer_dump_definitions(TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS)`
 *
 * Also turns on `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS`, and sets the emitter's
 * mode of operations to `TPP_EMITTER_MODE_DISPOSE` (see `TPP_EMITTER_HAVE_MODE_DISPOSE`).
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_M
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_M          \
	(TPP_EMITTER_HAVE_CLI &&                      \
	 TPP_HAVE_LEXER_DUMP_DEFINITIONS &&           \
	 TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS && \
	 TPP_EMITTER_HAVE_MODE_DISPOSE)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_M */

/* `-dD`, `--dump=D`:
 * Same as `TPP_EMITTER_HAVE_CLI_DASH_DUMP_M`,
 * but doesn't turn on `TPP_EMITTER_MODE_DISPOSE`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_D
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_D \
	(TPP_EMITTER_HAVE_CLI &&             \
	 TPP_HAVE_LEXER_DUMP_DEFINITIONS &&  \
	 TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_D */

/* `-dN`, `--dump=N`:
 * Same as `TPP_EMITTER_HAVE_CLI_DASH_DUMP_D`, but also turns on
 * `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_N
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_N          \
	(TPP_EMITTER_HAVE_CLI &&                      \
	 TPP_HAVE_LEXER_DUMP_DEFINITIONS &&           \
	 TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS && \
	 TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_N */

/* `-dI`, `--dump=I`:
 * Turn on `TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_I
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_I \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_I */

/* `-dU`, `--dump=U`:
 * Turn on `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_U
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_U \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_U */

/* `-H`, `--trace-includes`:
 * Turn on `TPP_EMITTER_HAVE_TRACE_INCLUDES`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
#define TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_TRACE_INCLUDES)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */

/* `-frelaxed-macro-column`, `-fno-relaxed-macro-column`:
 * Turn `TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN
#define TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN */

/* `-freemit-unknown-pragma`, `-fno-reemit-unknown-pragma`:
 * Turn `TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA
#define TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA */

/* `-fworking-directory`, `-fno-working-directory`:
 * Turn `TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY
#define TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY */

/* `-fuse-cpp-digit`, `-fno-use-cpp-digit`:
 * Turn `TPP_EMITTER_HAVE_USE_CPP_DIGIT` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT
#define TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT */

/* `-fuse-cpp-digit-flags`, `-fno-use-cpp-digit-flags`:
 * Turn `TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS
#define TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS */

/* `-fline-threshold=COUNT`, `-fno-line-threshold`:
 * Configure `TPP_EMITTER_CONFIG_LINE_THRESHOLD`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD
#define TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_CONFIG_LINE_THRESHOLD < 0)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD */

/* `-fnormalize=space`, `-fno-normalize=space`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_SPACE` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_SPACE))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE */

/* `-fnormalize=lf`, `-fno-normalize=lf`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_LF` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_LF))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF */

/* `-fnormalize=strings`, `-fno-normalize=strings`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_C_STRING` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS */

/* `-fnormalize=int`, `-fno-normalize=int`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_C_INT` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_C_INT))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT */

/* `-fnormalize=keywords`, `-fno-normalize=keywords`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS */

/* `-fnormalize=bse`, `-fno-normalize=bse`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_BSE` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_BSE))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE */

/* `-fnormalize=trigraphs`, `-fno-normalize=trigraphs`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_BSE` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS */

/* `-fnormalize=digraphs`, `-fno-normalize=digraphs`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_BSE` on/off
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS */

/* `-fnormalize`, `-fno-normalize`:
 * Turn all *normalization*-features (that can be runtime-configured) on/off at once:
 * - `TPP_EMITTER_HAVE_NORMALIZE_SPACE`
 * - `TPP_EMITTER_HAVE_NORMALIZE_LF`
 * - `TPP_EMITTER_HAVE_NORMALIZE_C_STRING`
 * - `TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS`
 * - `TPP_EMITTER_HAVE_NORMALIZE_BSE`
 * - `TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS`
 * - `TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE                 \
	(TPP_EMITTER_HAVE_CLI &&                                 \
	 (TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) ||     \
	  TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_LF) ||        \
	  TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) ||  \
	  TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) ||  \
	  TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_BSE) ||       \
	  TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) || \
	  TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE */

/* `--mode=emit`
 * - Set emitter mode to `TPP_EMITTER_MODE_EMIT`
 * - Turn off `TPP_EMITTER_HAVE_NOLINE`
 * - Turn off emission of `SPACE` and `LF` tokens
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_EMIT)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT */

/* `--mode=dispose`
 * - Set emitter mode to `TPP_EMITTER_MODE_DISPOSE`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_DISPOSE)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE */

/* `--mode=bracket`
 * - Set emitter mode to `TPP_EMITTER_MODE_BRACKET`
 * - Turn on emission of `SPACE` and `LF` tokens
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_BRACKET)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET */

/* `--mode=typed`
 * - Set emitter mode to `TPP_EMITTER_HAVE_MODE_TYPED`
 * - Turn on emission of `SPACE` and `LF` tokens
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_TYPED)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED */

/* `--mode=zero`
 * - Set emitter mode to `TPP_EMITTER_HAVE_MODE_ZERO`
 * - Turn off emission of `SPACE` and `LF` tokens
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_ZERO)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO */

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_CONFIG_H */
