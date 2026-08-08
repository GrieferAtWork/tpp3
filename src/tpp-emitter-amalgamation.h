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
#ifndef GUARD_TPP_EMITTER_AMALGAMATION_H
#define GUARD_TPP_EMITTER_AMALGAMATION_H 1

/************************************************************************/
/* File: parts/optional/emitter/api.h                                   */
/************************************************************************/
/* ========= Emitter =========
 *
 * Brief:
 * - Turns a sequence of tokens back into something that can be understood
 *   by an(other) preprocessor with only minimal capabilities.
 * - Retain (make sure it doesn't change) all information normally returned by:
 *   - tpp_file_getfilename(tpp_lexer_getlcfile(LEXER))
 *   - tpp_file_getstartlcinfo(tpp_lexer_getlcfile(LEXER))
 *   - tpp_file_getendlcinfo(tpp_lexer_getlcfile(LEXER))   (only if normalization is disabled)
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

/************************************************************************/
/* File: parts/optional/emitter/config.h                                */
/************************************************************************/
/* Provide support for `TPP_EMITTER_MODE_EMIT`, which emits tokens */
#ifndef TPP_EMITTER_HAVE_MODE_EMIT
#define TPP_EMITTER_HAVE_MODE_EMIT 1
#endif /* !TPP_EMITTER_HAVE_MODE_EMIT */

/* Provide support for `TPP_EMITTER_MODE_DISPOSE`, which discards tokens */
#ifndef TPP_EMITTER_HAVE_MODE_DISPOSE
#define TPP_EMITTER_HAVE_MODE_DISPOSE (TPP_HAVE_PROFILE_ALL)
#endif /* !TPP_EMITTER_HAVE_MODE_DISPOSE */

/* Provide support for `TPP_EMITTER_MODE_BRACKET`, where
 * tokens are emitted surrounded by brackets. */
#ifndef TPP_EMITTER_HAVE_MODE_BRACKET
#define TPP_EMITTER_HAVE_MODE_BRACKET (TPP_HAVE_PROFILE_ALL)
#endif /* !TPP_EMITTER_HAVE_MODE_BRACKET */

/* Provide support for `TPP_EMITTER_MODE_TYPED`, where tokens are
 * emitted surrounded as `[{TYPE}:{TOKEN}]`, where `TYPE` is the
 * result of `tpp_strtokenid()` and the canonical keyword name. */
#ifndef TPP_EMITTER_HAVE_MODE_TYPED
#define TPP_EMITTER_HAVE_MODE_TYPED (TPP_HAVE_STRTOKENID && TPP_HAVE_PROFILE_ALL)
#endif /* !TPP_EMITTER_HAVE_MODE_TYPED */

/* Provide support for `TPP_EMITTER_MODE_ZERO`, where tokens are
 * emitted in their normalized form (see `TPP_EMITTER_HAVE_NORMALIZE_*`),
 * with every token followed by a NUL-byte. This format is primarily
 * meant for preprocessing small files such that another process can
 * (fairly easily) consume those tokens without having to worry about
 * the boundaries between tokens. */
#ifndef TPP_EMITTER_HAVE_MODE_ZERO
#define TPP_EMITTER_HAVE_MODE_ZERO (TPP_HAVE_PROFILE_ALL)
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
 * any unicode whitespace or other control characters to `U+0020 SPACE`). */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_SPACE
#define TPP_EMITTER_HAVE_NORMALIZE_SPACE (TPP_EMITTER_HAVE_EMIT_TOKEN ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_SPACE */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any `TPP_TOK_LF`-token is
 * emitted as a `\n`-character, rather than as an echo of the original token's
 * linefeed bytes (thereby normalizing any unicode linefeed, CR, or CRLF
 * sequences to LF). */
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
 * fully understand *any* kind of string token that may be produced by TPP. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_C_STRING
#define TPP_EMITTER_HAVE_NORMALIZE_C_STRING ((TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_TOK_C_STRING && TPP_HAVE_TOKEN_ENCODESTRING && TPP_HAVE_LEXER_DECODESTRING) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_C_STRING */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, any
 * `TPP_TOK_ISINT`-token is emitted as a `TPP_TOK_C_INT` token. */
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
 *       to write that identifier. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS
#define TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS ((TPP_EMITTER_HAVE_EMIT_TOKEN && (TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED)) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, remove `\`-escaped
 * line-feeds from generic tokens. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_BSE
#define TPP_EMITTER_HAVE_NORMALIZE_BSE ((TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_BSE) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_BSE */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, normalize trigraph
 * sequences in generic tokens. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS
#define TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS ((TPP_EMITTER_HAVE_EMIT_TOKEN && TPP_HAVE_TRIGRAPHS) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, normalize digraph
 * sequences in generic tokens. */
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
 * Can be used to implement the `-P` CLI switch */
#ifndef TPP_EMITTER_HAVE_NOLINE
#define TPP_EMITTER_HAVE_NOLINE (TPP_EMITTER_HAVE_MODE_EMIT ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_EMITTER_HAVE_NOLINE */

/* When enabled and in `TPP_EMITTER_MODE_EMIT`-mode, tokens emitted
 * from within a macro do not require proper alignment with __COLUMN__,
 * so-as to prevent every token from causing another `#line`-directive
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
 * ``` */
#ifndef TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN
#define TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN (TPP_EMITTER_HAVE_MODE_EMIT ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN */

/* Use `# <linenum>` instead of `#line` to align tokens */
#ifndef TPP_EMITTER_HAVE_USE_CPP_DIGIT
#define TPP_EMITTER_HAVE_USE_CPP_DIGIT (TPP_EMITTER_HAVE_MODE_EMIT ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT */

/* Extension to `TPP_EMITTER_HAVE_USE_CPP_DIGIT`: also use 1/2/3/4 flags */
#ifndef TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#define TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS ((TPP_EMITTER_HAVE_USE_CPP_DIGIT && TPP_HAVE_FILE_PUSHED_HOOK && TPP_HAVE_FILE_POPPED_HOOK && (TPP_HAVE_FILE_SYSHDR || TPP_HAVE_FILE_EXTERN_C)) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */

/* Extension to `TPP_EMITTER_HAVE_USE_CPP_DIGIT`: the first time a `# <linenum>` directive is emitted,
 * emit it 2 times, and in-between those to emissions, emit another line `# <linenum> "$(PWD)//"` */
#ifndef TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
#define TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY (TPP_EMITTER_HAVE_USE_CPP_DIGIT ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */

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
 *   - If not, or if the macro's definition has changed, dump it now
 *     If there was a different definition, emit a `#undef` first
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
 * - `tpp_emitter_disable_reemit_macro_definitions_lazy()` */
#ifndef TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
#define TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY ((TPP_HAVE_FILE_PUSHED_HOOK && TPP_HAVE_KEYWORD_USERDATA && TPP_HAVE_MACRO_NAME) ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

/* Extension to `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS` and
 * `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY`: when emitting
 * `#define` directives, only emit `#define <MACRO_NAME>`, excluding
 * the macro's actual definition. */
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
 * - `tpp_emitter_disable_trace_includes()` */
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
 * **Setter**: `tpp_emitter_setlinethreshold(emitter, v)`
 */
#ifndef TPP_EMITTER_CONFIG_LINE_THRESHOLD
#define TPP_EMITTER_CONFIG_LINE_THRESHOLD (TPP_HAVE_PROFILE_NOT_MINIMAL ? -4 : 4)
#endif /* !TPP_EMITTER_CONFIG_LINE_THRESHOLD */

/* Provide an API `tpp_io_printpwd()` that can be used to print the hosting
 * process's current working directory to a given `tpp_formatprinter`. */
#ifndef TPP_EMITTER_HAVE_IO_PRINTPWD
#define TPP_EMITTER_HAVE_IO_PRINTPWD (TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
#endif /* !TPP_EMITTER_HAVE_IO_PRINTPWD */




/************************************************************************/
/* EMITTER CLI CONFIG                                                   */
/************************************************************************/

/* Enable support for `tpp_emitter_cli_loader` */
#ifndef TPP_EMITTER_HAVE_CLI
#define TPP_EMITTER_HAVE_CLI TPP_HAVE_CLI
#endif /* !TPP_EMITTER_HAVE_CLI */

/* Enable support for `tpp_emitter_cli_loader_help`, which exposes a small
 * database of supported commandline flags in a human-readable format that
 * can also be rendered (fairly) easily. */
#ifndef TPP_EMITTER_HAVE_CLI_HELP
#define TPP_EMITTER_HAVE_CLI_HELP (TPP_HAVE_PROFILE_ALL && TPP_EMITTER_HAVE_CLI)
#endif /* !TPP_EMITTER_HAVE_CLI_HELP */

/* Include extra spellings (i.e.: in addition to the primary spelling) of CLI options. */
#ifndef TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS
#define TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS (TPP_EMITTER_HAVE_CLI_HELP && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS */

/* `-P`, `--no-line-commands`:
 * Disable emission of `#line`-directives, but also turn
 * on emission of SPACE/LF tokens (if runtime-configurable). */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
#define TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_NOLINE && (TPP_HAVE_TOK_SPACE && TPP_HAVE_TOK_LF))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */

/* `-dM`, `--dump=M`:
 * Dump builtin/predefined macros to the emitters output during the
 * CLI flush phase (i.e.: when `tpp_emitter_cli_loader_flush()` is called):
 * `tpp_lexer_dump_definitions(TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS)`
 *
 * Also turns on `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS`, and sets the emitter's
 * mode of operations to `TPP_EMITTER_MODE_DISPOSE` (see `TPP_EMITTER_HAVE_MODE_DISPOSE`). */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_M
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_M          \
	(TPP_EMITTER_HAVE_CLI &&                      \
	 TPP_HAVE_LEXER_DUMP_DEFINITIONS &&           \
	 TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS && \
	 TPP_EMITTER_HAVE_MODE_DISPOSE)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_M */

/* `-dD`, `--dump=D`:
 * Same as `TPP_EMITTER_HAVE_CLI_DASH_DUMP_M`, but doesn't turn on `TPP_EMITTER_MODE_DISPOSE` */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_D
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_D \
	(TPP_EMITTER_HAVE_CLI &&             \
	 TPP_HAVE_LEXER_DUMP_DEFINITIONS &&  \
	 TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_D */

/* `-dN`, `--dump=N`:
 * Same as `TPP_EMITTER_HAVE_CLI_DASH_DUMP_D`, but also turns on
 * `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY` */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_N
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_N          \
	(TPP_EMITTER_HAVE_CLI &&                      \
	 TPP_HAVE_LEXER_DUMP_DEFINITIONS &&           \
	 TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS && \
	 TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_N */

/* `-dI`, `--dump=I`:
 * Turn on `TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES` */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_I
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_I \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_I */

/* `-dU`, `--dump=U`:
 * Turn on `TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY` */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_DUMP_U
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP_U \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_DUMP_U */

/* `-H`, `--trace-includes`:
 * Turn on `TPP_EMITTER_HAVE_TRACE_INCLUDES` */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
#define TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_TRACE_INCLUDES)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */

/* `-frelaxed-macro-column`, `-fno-relaxed-macro-column`:
 * Turn `TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN
#define TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN */

/* `-freemit-unknown-pragma`, `-fno-reemit-unknown-pragma`:
 * Turn `TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA
#define TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA */

/* `-fworking-directory`, `-fno-working-directory`:
 * Turn `TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY
#define TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY */

/* `-fuse-cpp-digit`, `-fno-use-cpp-digit`:
 * Turn `TPP_EMITTER_HAVE_USE_CPP_DIGIT` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT
#define TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT */

/* `-fuse-cpp-digit-flags`, `-fno-use-cpp-digit-flags`:
 * Turn `TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS
#define TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS */

/* `-fline-threshold=COUNT`, `-fno-line-threshold`:
 * Configure `TPP_EMITTER_CONFIG_LINE_THRESHOLD` */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD
#define TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_CONFIG_LINE_THRESHOLD < 0)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD */

/* `-fnormalize=space`, `-fno-normalize=space`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_SPACE` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_SPACE))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE */

/* `-fnormalize=lf`, `-fno-normalize=lf`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_LF` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_LF))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF */

/* `-fnormalize=strings`, `-fno-normalize=strings`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_C_STRING` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS */

/* `-fnormalize=int`, `-fno-normalize=int`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_C_INT` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_C_INT))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT */

/* `-fnormalize=keywords`, `-fno-normalize=keywords`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS */

/* `-fnormalize=bse`, `-fno-normalize=bse`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_BSE` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_BSE))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE */

/* `-fnormalize=trigraphs`, `-fno-normalize=trigraphs`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_BSE` on/off */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS
#define TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS \
	(TPP_EMITTER_HAVE_CLI && TPP_CONF_ISRT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS))
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS */

/* `-fnormalize=digraphs`, `-fno-normalize=digraphs`:
 * Turn `TPP_EMITTER_HAVE_NORMALIZE_BSE` on/off */
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
 * - `TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS` */
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
 * - Turn off emission of `SPACE` and `LF` tokens */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_EMIT)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT */

/* `--mode=dispose`
 * - Set emitter mode to `TPP_EMITTER_MODE_DISPOSE` */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_DISPOSE)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE */

/* `--mode=bracket`
 * - Set emitter mode to `TPP_EMITTER_MODE_BRACKET`
 * - Turn on emission of `SPACE` and `LF` tokens */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_BRACKET)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET */

/* `--mode=typed`
 * - Set emitter mode to `TPP_EMITTER_HAVE_MODE_TYPED`
 * - Turn on emission of `SPACE` and `LF` tokens */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_TYPED)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED */

/* `--mode=zero`
 * - Set emitter mode to `TPP_EMITTER_HAVE_MODE_ZERO`
 * - Turn off emission of `SPACE` and `LF` tokens */
#ifndef TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO
#define TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO \
	(TPP_EMITTER_HAVE_CLI && TPP_EMITTER_HAVE_MODE_ZERO)
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO */

/************************************************************************/
/* File: parts/optional/emitter/emitter-io.h                            */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_EMITTER_HAVE_IO_PRINTPWD
#ifndef tpp_io_printpwd
/* Print the hosting process's current-working-directory ($PWD,
 * aka `getcwd()`) to the given `printer`. Needed primarily to
 * implement `TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY`
 *
 * @return: * :  Sum of positive return values of `printer`
 * @return: < 0: First negative return value of `printer`
 * @return: TPP_SSIZE_OFERR(TPP_EIO):    I/O error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM): Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_io_printpwd(tpp_formatprinter printer, void *arg);
#endif /* !tpp_io_printpwd */
#endif /* TPP_EMITTER_HAVE_IO_PRINTPWD */

/************************************************************************/
/* File: parts/optional/emitter/emitter-features.h                      */
/************************************************************************/

#undef TPP_EMITTER_HAVE_FEATURES
#if (TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) ||                    \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF) ||                       \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) ||                 \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT) ||                    \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) ||                 \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE) ||                      \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) ||                \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) ||                 \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE) ||                             \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN) ||               \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT) ||                      \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) ||                \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY) ||    \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) ||      \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) || \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES))
#define TPP_EMITTER_HAVE_FEATURES 1
#else /* ... */
#define TPP_EMITTER_HAVE_FEATURES 0
#endif/* !... */

#if TPP_EMITTER_HAVE_FEATURES
typedef enum tpp_emitter_feature_id {
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
	TPP_EMITTER_FEAT_NORMALIZE_SPACE,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF)
	TPP_EMITTER_FEAT_NORMALIZE_LF,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
	TPP_EMITTER_FEAT_NORMALIZE_C_STRING,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT)
	TPP_EMITTER_FEAT_NORMALIZE_C_INT,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
	TPP_EMITTER_FEAT_NORMALIZE_KEYWORDS,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
	TPP_EMITTER_FEAT_NORMALIZE_BSE,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
	TPP_EMITTER_FEAT_NORMALIZE_TRIGRAPHS,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
	TPP_EMITTER_FEAT_NORMALIZE_DIGRAPHS,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
	TPP_EMITTER_FEAT_NOLINE,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN)
	TPP_EMITTER_FEAT_RELAXED_MACRO_COLUMN,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT)
	TPP_EMITTER_FEAT_USE_CPP_DIGIT,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
	TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
	TPP_EMITTER_FEAT_USE_CPP_DIGIT_WORKING_DIRECTORY,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
	TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
	TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_NAME_ONLY,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
	TPP_EMITTER_FEAT_TRACE_INCLUDES,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
	TPP_EMITTER_FEAT_COUNT
} tpp_emitter_feature_id;

typedef union tpp_emitter_features {
	struct {
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_SPACE): 1;
#define _tpp_emitter_has_NORMALIZE_SPACE(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_SPACE)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_LF): 1;
#define _tpp_emitter_has_NORMALIZE_LF(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_LF)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_STRING): 1;
#define _tpp_emitter_has_NORMALIZE_C_STRING(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_STRING)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_INT): 1;
#define _tpp_emitter_has_NORMALIZE_C_INT(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_INT)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_KEYWORDS): 1;
#define _tpp_emitter_has_NORMALIZE_KEYWORDS(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_KEYWORDS)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_BSE): 1;
#define _tpp_emitter_has_NORMALIZE_BSE(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_BSE)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_TRIGRAPHS): 1;
#define _tpp_emitter_has_NORMALIZE_TRIGRAPHS(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_TRIGRAPHS)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_DIGRAPHS): 1;
#define _tpp_emitter_has_NORMALIZE_DIGRAPHS(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_DIGRAPHS)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
		unsigned int TPP_EMITTER_INTERNAL(teff_NOLINE): 1;
#define _tpp_emitter_has_NOLINE(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NOLINE)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN)
		unsigned int TPP_EMITTER_INTERNAL(teff_RELAXED_MACRO_COLUMN): 1;
#define _tpp_emitter_has_RELAXED_MACRO_COLUMN(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_RELAXED_MACRO_COLUMN)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT)
		unsigned int TPP_EMITTER_INTERNAL(teff_USE_CPP_DIGIT): 1;
#define _tpp_emitter_has_USE_CPP_DIGIT(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_USE_CPP_DIGIT)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
		unsigned int TPP_EMITTER_INTERNAL(teff_USE_CPP_DIGIT_FLAGS): 1;
#define _tpp_emitter_has_USE_CPP_DIGIT_FLAGS(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_USE_CPP_DIGIT_FLAGS)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
		unsigned int TPP_EMITTER_INTERNAL(teff_USE_CPP_DIGIT_WORKING_DIRECTORY): 1;
#define _tpp_emitter_has_USE_CPP_DIGIT_WORKING_DIRECTORY(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_USE_CPP_DIGIT_WORKING_DIRECTORY)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
		unsigned int TPP_EMITTER_INTERNAL(teff_REEMIT_MACRO_DEFINITIONS_LAZY): 1;
#define _tpp_emitter_has_REEMIT_MACRO_DEFINITIONS_LAZY(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_REEMIT_MACRO_DEFINITIONS_LAZY)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
		unsigned int TPP_EMITTER_INTERNAL(teff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY): 1;
#define _tpp_emitter_has_REEMIT_MACRO_DEFINITIONS_NAME_ONLY(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
		unsigned int TPP_EMITTER_INTERNAL(teff_TRACE_INCLUDES): 1;
#define _tpp_emitter_has_TRACE_INCLUDES(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_TRACE_INCLUDES)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
	} TPP_EMITTER_INTERNAL(tef_flags);
	unsigned char TPP_EMITTER_INTERNAL(tetf_bitset)[TPP_EMITTER_FEAT_COUNT ? ((TPP_EMITTER_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_emitter_features;

#if !TPP_USE_STATIC
TPP_CONST_DECL tpp_emitter_features const tpp_emitter_features_default;
#endif /* !TPP_USE_STATIC */

#define tpp_emitter_features_getid(self, id) \
	((self)->TPP_EMITTER_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_emitter_features_enable(self, id) \
	(void)((self)->TPP_EMITTER_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_emitter_features_disable(self, id) \
	(void)((self)->TPP_EMITTER_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_emitter_features_setid(self, id, enabled) \
	((enabled) ? tpp_emitter_features_enable(self, id) : tpp_emitter_features_disable(self, id))
#define tpp_emitter_features_init(self)            (void)(*(self) = tpp_emitter_features_default)
#define tpp_emitter_features_reset(self)           (void)(*(self) = tpp_emitter_features_default)
#define tpp_emitter_features_fini(self)            tpp_dbg_memset(self, sizeof(tpp_emitter_features))
#endif /* TPP_EMITTER_HAVE_FEATURES */

#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
#define _tpp_emitter_has_NORMALIZE_SPACE(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_SPACE) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_LF)
#define _tpp_emitter_has_NORMALIZE_LF(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_LF)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_LF) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
#define _tpp_emitter_has_NORMALIZE_C_STRING(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_C_INT)
#define _tpp_emitter_has_NORMALIZE_C_INT(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_C_INT)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_C_INT) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
#define _tpp_emitter_has_NORMALIZE_KEYWORDS(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_BSE)
#define _tpp_emitter_has_NORMALIZE_BSE(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_BSE) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
#define _tpp_emitter_has_NORMALIZE_TRIGRAPHS(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
#define _tpp_emitter_has_NORMALIZE_DIGRAPHS(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NOLINE)
#define _tpp_emitter_has_NOLINE(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NOLINE)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN)
#define _tpp_emitter_has_RELAXED_MACRO_COLUMN(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_USE_CPP_DIGIT)
#define _tpp_emitter_has_USE_CPP_DIGIT(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_USE_CPP_DIGIT) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#define _tpp_emitter_has_USE_CPP_DIGIT_FLAGS(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
#define _tpp_emitter_has_USE_CPP_DIGIT_WORKING_DIRECTORY(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
#define _tpp_emitter_has_REEMIT_MACRO_DEFINITIONS_LAZY(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#define _tpp_emitter_has_REEMIT_MACRO_DEFINITIONS_NAME_ONLY(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_TRACE_INCLUDES)
#define _tpp_emitter_has_TRACE_INCLUDES(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_TRACE_INCLUDES) */

/************************************************************************/
/* File: parts/optional/emitter/emitter.h                               */
/************************************************************************/

#undef TPP_EMITTER_HAVE_CURPOS
#if TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
#define TPP_EMITTER_HAVE_CURPOS 1
#else /* ... */
#define TPP_EMITTER_HAVE_CURPOS 0
#endif /* !... */

#if TPP_EMITTER_HAVE_CURPOS
typedef struct tpp_emitter_state_file {
	tpp_lcinfo          TPP_EMITTER_INTERNAL(tesf_curpos);    /* Current line/column position in output (with respect to emitted `#line` directives) */
	char const         *TPP_EMITTER_INTERNAL(tesf_fname);     /* [0..1] The filename (tpp_file_getfilename()) that goes with `tes_curpos` (or "NULL" if unknown, or this is the first token) */
	TPP_REF tpp_string *TPP_EMITTER_INTERNAL(tesf_fname_str); /* [0..1] Same as `tes_curfilename`, but keeps a reference to `tpp_file_getfilenamestr()` so custom filename overrides aren't free'd early */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#if TPP_HAVE_FILE_SYSHDR && TPP_HAVE_FILE_EXTERN_C
#define _TPP_EMITTER_STATE_FLAGS_MASK (TPP_FILE_FLAGS_SYSHDR | TPP_FILE_FLAGS_EXTERN_C)
#elif TPP_HAVE_FILE_SYSHDR
#define _TPP_EMITTER_STATE_FLAGS_MASK TPP_FILE_FLAGS_SYSHDR
#elif TPP_HAVE_FILE_EXTERN_C
#define _TPP_EMITTER_STATE_FLAGS_MASK TPP_FILE_FLAGS_EXTERN_C
#else /* ... */
#define _TPP_EMITTER_STATE_FLAGS_MASK 0
#endif /* !... */
#if _TPP_EMITTER_STATE_FLAGS_MASK
	tpp_file_flags          TPP_EMITTER_INTERNAL(tesf_flags); /* Set of `_TPP_EMITTER_STATE_FLAGS_MASK` */
#endif /* _TPP_EMITTER_STATE_FLAGS_MASK */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
} tpp_emitter_state_file;

typedef struct tpp_emitter_state_files {
	tpp_emitter_state_file  TPP_EMITTER_INTERNAL(tesfs_file);    /* Most-recent file */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	tpp_size                TPP_EMITTER_INTERNAL(tesfs_filec);   /* # of dummy files pushed by `# <digit> "filename" 1` */
	tpp_emitter_state_file *TPP_EMITTER_INTERNAL(tesfs_filev);   /* [0..tesfs_filec][owned] Extra files pus */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
} tpp_emitter_state_files;
#endif /* TPP_EMITTER_HAVE_CURPOS */

#undef TPP_EMITTER_HAVE_FLAGS
#if (TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || \
     TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
#define TPP_EMITTER_HAVE_FLAGS 1
#else /* ... */
#define TPP_EMITTER_HAVE_FLAGS 0
#endif /* !... */

#if TPP_EMITTER_HAVE_FLAGS
#define tpp_emitter_flags uint_least8_t
#define TPP_EMITTER_FLAG_NORMAL   UINT8_C(0x00)
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#define TPP_EMITTER_FLAG_FCHANGED UINT8_C(0x01) /* Contents of the #include-stack may have changed */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
#define TPP_EMITTER_FLAG_HASLINE  UINT8_C(0x02) /* At least 1 `# <linenum>`-directive was emitted */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */
#endif /* TPP_EMITTER_HAVE_FLAGS */


typedef struct tpp_emitter_state {
#if TPP_EMITTER_HAVE_CURPOS
	tpp_emitter_state_files TPP_EMITTER_INTERNAL(tes_curfile); /* Current file-state. */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	tpp_size                TPP_EMITTER_INTERNAL(tes_cached_filec); /* Size of alternate file-state buffer (`tes_cached_filev`). */
	tpp_emitter_state_file *TPP_EMITTER_INTERNAL(tes_cached_filev); /* [0..tes_cached_filec] Alternate file-state buffer (used internally) */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#endif /* TPP_EMITTER_HAVE_CURPOS */
#if TPP_EMITTER_HAVE_FLAGS
	tpp_emitter_flags       TPP_EMITTER_INTERNAL(tes_flags);   /* Emitter flags */
#endif /* TPP_EMITTER_HAVE_FLAGS */
	tpp_token_id            TPP_EMITTER_INTERNAL(tes_prevtok); /* Last token ID (preceding the token currently being emitted).
	                                                            * When the current token is the first, this is `TPP_TOK_EOF` */
} tpp_emitter_state;


#undef _TPP_EMITTER_MODE_DEFAULT
#undef TPP_EMITTER_MODE_HAVE_MULTIPLE
typedef enum tpp_emitter_mode {

	/* Emit tokens to the emitter output (do what e.g. `gcc -E` does) */
#if TPP_EMITTER_HAVE_MODE_EMIT
	TPP_EMITTER_MODE_EMIT,
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_EMIT
#endif /* TPP_EMITTER_HAVE_MODE_EMIT */

	/* Dispose tokens (output can only be produced by hooks or "raw" printing) */
#if TPP_EMITTER_HAVE_MODE_DISPOSE
	TPP_EMITTER_MODE_DISPOSE,
#ifndef _TPP_EMITTER_MODE_DEFAULT
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_DISPOSE
#else /* !_TPP_EMITTER_MODE_DEFAULT */
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 1
#endif /* _TPP_EMITTER_MODE_DEFAULT */
#endif /* TPP_EMITTER_HAVE_MODE_DISPOSE */

	/* Print tokens in [brackets] */
#if TPP_EMITTER_HAVE_MODE_BRACKET
	TPP_EMITTER_MODE_BRACKET,
#ifndef _TPP_EMITTER_MODE_DEFAULT
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_BRACKET
#else /* !_TPP_EMITTER_MODE_DEFAULT */
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 1
#endif /* _TPP_EMITTER_MODE_DEFAULT */
#endif /* TPP_EMITTER_HAVE_MODE_BRACKET */

	/* Print tokens as `[{TYPE}:{TOKEN}]` */
#if TPP_EMITTER_HAVE_MODE_TYPED
	TPP_EMITTER_MODE_TYPED,
#ifndef _TPP_EMITTER_MODE_DEFAULT
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_TYPED
#else /* !_TPP_EMITTER_MODE_DEFAULT */
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 1
#endif /* _TPP_EMITTER_MODE_DEFAULT */
#endif /* TPP_EMITTER_HAVE_MODE_TYPED */

	/* Print tokens as `{TOKEN}\0` */
#if TPP_EMITTER_HAVE_MODE_ZERO
	TPP_EMITTER_MODE_ZERO,
#ifndef _TPP_EMITTER_MODE_DEFAULT
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_ZERO
#else /* !_TPP_EMITTER_MODE_DEFAULT */
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 1
#endif /* _TPP_EMITTER_MODE_DEFAULT */
#endif /* TPP_EMITTER_HAVE_MODE_ZERO */

} tpp_emitter_mode;

#ifndef TPP_EMITTER_MODE_HAVE_MULTIPLE
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 0
#endif /* !TPP_EMITTER_MODE_HAVE_MULTIPLE */


typedef struct tpp_emitter {
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER)
	tpp_lexer           *TPP_EMITTER_INTERNAL(te_lexer);  /* [1..1][const] The lexer whose tokens are being emitted */
#endif /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
	tpp_formatprinter    TPP_EMITTER_INTERNAL(te_output); /* [1..1][const] Emitter output printer (the emitter itself will be passed as argument) */
	tpp_emitter_state    TPP_EMITTER_INTERNAL(te_state);  /* Emitter output state */
#if TPP_EMITTER_HAVE_FEATURES
	tpp_emitter_features TPP_EMITTER_INTERNAL(te_feat);   /* Emitter feature configuration */
#endif /* TPP_EMITTER_HAVE_FEATURES */
#if TPP_EMITTER_MODE_HAVE_MULTIPLE
	tpp_emitter_mode     TPP_EMITTER_INTERNAL(te_mode);   /* Mode in which tokens are emitted. */
#define tpp_emitter_getmode(self)    ((self)->TPP_EMITTER_INTERNAL(te_mode))
#define tpp_emitter_setmode(self, v) (void)((self)->TPP_EMITTER_INTERNAL(te_mode) = (v))
#else /* TPP_EMITTER_MODE_HAVE_MULTIPLE */
#define tpp_emitter_getmode(self)    _TPP_EMITTER_MODE_DEFAULT
#define tpp_emitter_setmode(self, v) (void)(v)
#endif /* !TPP_EMITTER_MODE_HAVE_MULTIPLE */
#if TPP_EMITTER_CONFIG_LINE_THRESHOLD < 0
	tpp_line             TPP_EMITTER_INTERNAL(te_linethreshold); /* max # of blank lines emitted for alignment purposes */
#define tpp_emitter_getlinethreshold(self)     ((self)->TPP_EMITTER_INTERNAL(te_linethreshold))
#define tpp_emitter_setlinethreshold(self, v)  (void)((self)->TPP_EMITTER_INTERNAL(te_linethreshold) = (tpp_line)(v))
#define tpp_emitter_disablelinethreshold(self) (void)((self)->TPP_EMITTER_INTERNAL(te_linethreshold) = -1)
#elif !TPP_EMITTER_CONFIG_LINE_THRESHOLD
#define tpp_emitter_getlinethreshold(self) (-1)
#else /* ... */
#define tpp_emitter_getlinethreshold(self) TPP_EMITTER_CONFIG_LINE_THRESHOLD
#endif /* !... */
} tpp_emitter;

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given emitter.
 *
 * @param: output: Output printer. On error, must return one of `TPP_SSIZE_OFERR(*)`
 * @param: lexer:  The lexer whose tokens are being emitted */
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER)
TPP_DECL TPP_NONNULL((1, 2, 3)) void TPPCALL
tpp_emitter_init(tpp_emitter *tpp_restrict self,
                 tpp_lexer *tpp_restrict lexer,
                 tpp_formatprinter output);
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
TPP_DECL TPP_NONNULL((1, 2)) void TPPCALL
_tpp_emitter_init(tpp_emitter *tpp_restrict self,
                  tpp_formatprinter output);
#define tpp_emitter_init(self, lexer, output)           \
	(tpp_assert(tpp_emitter_getlexer(self) == (lexer)), \
	 _tpp_emitter_init(self, output))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_emitter_fini(tpp_emitter *tpp_restrict self);

/* Retrieve components of the emitter. */
#define tpp_emitter_getoutput(self) (self)->TPP_EMITTER_INTERNAL(te_output)
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER)
#define tpp_emitter_getlexer(self)   ((self)->TPP_EMITTER_INTERNAL(te_lexer))
#define tpp_emitter_ofcookie(cookie) ((tpp_emitter *)(cookie))
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#ifndef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
#error "Invalid configuration: under '-DTPP_HAVE_HOOK_COOKIES=0' you must specify a macro '#define TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER (offsetof(MY_CONTAINER, emitter) - offsetof(MY_CONTAINER, lexer))' to specify how to retrieve the emitter from a lexer"
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define tpp_emitter_getlexer(self)   ((tpp_lexer *)((char *)(self) - TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER))
#define tpp_emitter_ofcookie(cookie) ((tpp_emitter *)((char *)(self) + TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */

/* Helpers for quickly printing stuff to the emitter's output.
 * WARNING: Careless use of these functions may result in the emitter's
 *          idea of its current output file/line/column becoming invalid. */
#define tpp_emitter_output_printraw(self, text, len) \
	tpp_formatprinter_print(tpp_emitter_getoutput(self), self, text, len)
#define tpp_emitter_output_printraw_cstr(self, text, len) \
	tpp_formatprinter_print_cstr(tpp_emitter_getoutput(self), self, text, len)
#define tpp_emitter_output_printraw_conststr(self, CONSTstr) \
	tpp_formatprinter_print_conststr(tpp_emitter_getoutput(self), self, CONSTstr)

/* Check if a runtime-configurable config option `conf` in `TPP_EMITTER_HAVE_<conf>` is currently enabled.
 * When `TPP_EMITTER_HAVE_<conf>` is configured as `TPP_CONF_ISCONST()`, return that constant instead. */
#define tpp_emitter_has(self, conf) _tpp_emitter_has_##conf(self)

/* Features... */
#if TPP_EMITTER_HAVE_FEATURES
#define tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_x)          tpp_emitter_features_getid(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_x, enabled) tpp_emitter_features_setid(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x, enabled)
#define tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_x)       tpp_emitter_features_enable(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_x)      tpp_emitter_features_disable(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_resetfeatures(self)                           tpp_emitter_features_reset(&(self)->TPP_EMITTER_INTERNAL(te_feat))
#else /* TPP_EMITTER_HAVE_FEATURES */
#define tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_x) 0
#define tpp_emitter_resetfeatures(self)                  (void)0
#endif /* !TPP_EMITTER_HAVE_FEATURES */


/* Emit the token currently loaded into `tpp_emitter_getlexer(self)`,
 * and update the emitter's `te_state` accordingly
 *
 * @return: * :  Sum of return values of `tpp_emitter_getoutput(self)`
 * @return: < 0: First negative return value of `tpp_emitter_getoutput(self)` */
TPP_DECL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent(tpp_emitter *tpp_restrict self);


/* API support for (re-)emission of unknown `#pragma` directives */
#if TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
#if TPP_HAVE_HOOK_COOKIES
#define tpp_emitter_enable_reemit_unknown_pragma(self) \
	tpp_lexer_sethook_unknown_pragma_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma, self)
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_enable_reemit_unknown_pragma(self) \
	tpp_lexer_sethook_unknown_pragma(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma)
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_disable_reemit_unknown_pragma(self) \
	tpp_lexer_resethook_unknown_pragma(tpp_emitter_getlexer(self))
#define tpp_emitter_get_reemit_unknown_pragma(self) \
	(tpp_lexer_gethook_unknown_pragma(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_unknown_pragma)
#define tpp_emitter_set_reemit_unknown_pragma(self, v)    \
	((v) ? tpp_emitter_enable_reemit_unknown_pragma(self) \
	     : tpp_emitter_disable_reemit_unknown_pragma(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_unknown_pragma(tpp_hook_cookie cookie);
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */

/* API support for (re-)emission of `#define` and `#undef` directives */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
#if TPP_HAVE_HOOK_COOKIES
#define tpp_emitter_enable_reemit_macro_definitions(self)                                                    \
	(tpp_lexer_sethook_macro_defined_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined, self), \
	 tpp_lexer_sethook_macro_undefined_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_undefined, self))
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_enable_reemit_macro_definitions(self)                                           \
	(tpp_lexer_sethook_macro_defined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined), \
	 tpp_lexer_sethook_macro_undefined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_undefined))
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_disable_reemit_macro_definitions(self)          \
	(tpp_lexer_resethook_macro_defined(tpp_emitter_getlexer(self)), \
	 tpp_lexer_resethook_macro_undefined(tpp_emitter_getlexer(self)))
#define tpp_emitter_get_reemit_macro_definitions(self) \
	(tpp_lexer_gethook_macro_defined(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_macro_defined)
#define tpp_emitter_set_reemit_macro_definitions(self, v)    \
	((v) ? tpp_emitter_enable_reemit_macro_definitions(self) \
	     : tpp_emitter_disable_reemit_macro_definitions(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_defined(tpp_hook_cookie cookie,
                                tpp_keyword *tpp_restrict name,
                                tpp_macro *tpp_restrict macro);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_undefined(tpp_hook_cookie cookie,
                                  tpp_keyword *tpp_restrict name);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

/* API support for (re-)emission of `#include` (and friends) directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
#if TPP_HAVE_HOOK_COOKIES
#define tpp_emitter_enable_reemit_include_directives(self) \
	tpp_lexer_sethook_include_encountered_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered, self)
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_enable_reemit_include_directives(self) \
	tpp_lexer_sethook_include_encountered(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered)
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_disable_reemit_include_directives(self) \
	tpp_lexer_resethook_include_encountered(tpp_emitter_getlexer(self))
#define tpp_emitter_get_reemit_include_directives(self) \
	(tpp_lexer_gethook_include_encountered(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_include_encountered)
#define tpp_emitter_set_reemit_include_directives(self, v)    \
	((v) ? tpp_emitter_enable_reemit_include_directives(self) \
	     : tpp_emitter_disable_reemit_include_directives(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_include_encountered(tpp_hook_cookie cookie,
                                      tpp_hook_include_kind include_kind);
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */

#undef _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED
#if (TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || \
     TPP_EMITTER_HAVE_TRACE_INCLUDES ||                \
     TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#define _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED 1
#else /* ... */
#define _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED 0
#endif /* !... */
#if _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_file_pushed(tpp_hook_cookie cookie);
#if TPP_HAVE_HOOK_COOKIES
#define _tpp_emitter_enable_file_pushed_hook(self) \
	tpp_lexer_sethook_file_pushed_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_pushed, self)
#else /* TPP_HAVE_HOOK_COOKIES */
#define _tpp_emitter_enable_file_pushed_hook(self) \
	tpp_lexer_sethook_file_pushed(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_pushed)
#endif /* !TPP_HAVE_HOOK_COOKIES */
#if ((!TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)) && \
     (!TPP_EMITTER_HAVE_TRACE_INCLUDES || TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)) && \
     (!TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)))
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
#define _tpp_emitter_candisable_file_pushed_hook_REEMIT_MACRO_DEFINITIONS_LAZY(self) && !tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY)
#else /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */
#define _tpp_emitter_candisable_file_pushed_hook_REEMIT_MACRO_DEFINITIONS_LAZY(self) /* nothing */
#endif /* !TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
#define _tpp_emitter_candisable_file_pushed_hook_TRACE_INCLUDES(self) && !tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES)
#else /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
#define _tpp_emitter_candisable_file_pushed_hook_TRACE_INCLUDES(self) /* nothing */
#endif /* !TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#define _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self) && !tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS)
#else /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#define _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self) /* nothing */
#endif /* !TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#define _tpp_emitter_candisable_file_pushed_hook(self)                              \
	(1 _tpp_emitter_candisable_file_pushed_hook_REEMIT_MACRO_DEFINITIONS_LAZY(self) \
	   _tpp_emitter_candisable_file_pushed_hook_TRACE_INCLUDES(self)                \
	   _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self))
#define _tpp_emitter_disable_file_pushed_hook(self)                \
	(_tpp_emitter_candisable_file_pushed_hook(self)                \
	 ? tpp_lexer_resethook_file_pushed(tpp_emitter_getlexer(self)) \
	 : (void)0)
#else /* ... */
#define _tpp_emitter_disable_file_pushed_hook(self) (void)0
#endif /* !... */
#endif /* _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED */

/* Extension to `TPP_EMITTER_HAVE_USE_CPP_DIGIT`: also use 1/2/3/4 flags */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_emitter_hook_file_popped(tpp_hook_cookie cookie);
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#if TPP_HAVE_HOOK_COOKIES
#define _tpp_emitter_enable_use_cpp_digit_flags(self) \
	tpp_lexer_sethook_file_popped_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_popped, self)
#else /* TPP_HAVE_HOOK_COOKIES */
#define _tpp_emitter_enable_use_cpp_digit_flags(self) \
	 tpp_lexer_sethook_file_popped(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_popped))
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_enable_use_cpp_digit_flags(self)                        \
	(tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS), \
	 _tpp_emitter_enable_file_pushed_hook(self),                            \
	 _tpp_emitter_enable_use_cpp_digit_flags(self))
#define tpp_emitter_disable_use_cpp_digit_flags(self)                        \
	(tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS), \
	 _tpp_emitter_disable_file_pushed_hook(self),                            \
	 tpp_lexer_resethook_file_popped(tpp_emitter_getlexer(self)))
#define tpp_emitter_get_use_cpp_digit_flags(self) \
	tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS)
#define tpp_emitter_set_use_cpp_digit_flags(self, v)    \
	((v) ? tpp_emitter_enable_use_cpp_digit_flags(self) \
	     : tpp_emitter_disable_use_cpp_digit_flags(self))
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */

/* API support for *lazy* (re-)emission of `#define` and `#undef` directives */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
#define tpp_emitter_enable_reemit_macro_definitions_lazy(self)                        \
	(tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY), \
	 _tpp_emitter_enable_file_pushed_hook(self))
#define tpp_emitter_disable_reemit_macro_definitions_lazy(self)                        \
	(tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY), \
	 _tpp_emitter_disable_file_pushed_hook(self))
#define tpp_emitter_get_reemit_macro_definitions_lazy(self) \
	tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY)
#define tpp_emitter_set_reemit_macro_definitions_lazy(self, v)    \
	((v) ? tpp_emitter_enable_reemit_macro_definitions_lazy(self) \
	     : tpp_emitter_disable_reemit_macro_definitions_lazy(self))
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */

/* API support for tracing of #incude-depth and files */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
#define tpp_emitter_enable_trace_includes(self)                        \
	(tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES), \
	 _tpp_emitter_enable_file_pushed_hook(self))
#define tpp_emitter_disable_trace_includes(self)                        \
	(tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES), \
	 _tpp_emitter_disable_file_pushed_hook(self))
#define tpp_emitter_get_trace_includes(self) \
	tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES)
#define tpp_emitter_set_trace_includes(self, v)    \
	((v) ? tpp_emitter_enable_trace_includes(self) \
	     : tpp_emitter_disable_trace_includes(self))
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */

/************************************************************************/
/* File: parts/optional/emitter/emitter-cli.h                           */
/************************************************************************/

#if TPP_EMITTER_HAVE_CLI

/* Publicly exposed CLI loader states */
#define TPP_EMITTER_CLI_LOADER_STATE_NORMAL 0 /* Normal state */
#define TPP_EMITTER_CLI_LOADER_STATE_DDASH  1 /* State after "--" was encountered (causing all remaining ) */

#undef TPP_EMITTER_HAVE_CLI_DASH_FLAGS
#define TPP_EMITTER_HAVE_CLI_DASH_FLAGS \
	((TPP_EMITTER_HAVE_CLI_DASH_DUMP_M || TPP_EMITTER_HAVE_CLI_DASH_DUMP_D || TPP_EMITTER_HAVE_CLI_DASH_DUMP_N))

#if TPP_EMITTER_HAVE_CLI_DASH_FLAGS
#define _tpp_emitter_cli_loader_flags uint_least32_t
#define _TPP_EMITTER_CLI_LOADER_FLAG_NORMAL UINT32_C(0x00000000)
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_M || TPP_EMITTER_HAVE_CLI_DASH_DUMP_D || TPP_EMITTER_HAVE_CLI_DASH_DUMP_N
#define _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M UINT32_C(0x00000001) /* Do `tpp_lexer_dump_definitions(TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS)` in `tpp_emitter_cli_loader_flush()` */
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_M || TPP_EMITTER_HAVE_CLI_DASH_DUMP_D || TPP_EMITTER_HAVE_CLI_DASH_DUMP_N */
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FLAGS */


typedef struct tpp_emitter_cli_loader {
	tpp_emitter *TPP_EMITTER_INTERNAL(tecl_emitter); /* [1..1][const] The emitter being configured by this CLI loader */
	unsigned int TPP_EMITTER_INTERNAL(tecl_state);   /* CLI loader state (meaning of value is internal, except for `TPP_EMITTER_CLI_LOADER_STATE_*` listed above) */
#if TPP_EMITTER_HAVE_CLI_DASH_FLAGS
	_tpp_emitter_cli_loader_flags TPP_EMITTER_INTERNAL(tecl_flags);
#define _tpp_emitter_cli_loader_init_flags(self) , (self)->TPP_EMITTER_INTERNAL(tecl_flags) = _TPP_EMITTER_CLI_LOADER_FLAG_NORMAL
#else /* TPP_EMITTER_HAVE_CLI_DASH_FLAGS */
#define _tpp_emitter_cli_loader_init_flags(self) /* nothing */
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_FLAGS */
} tpp_emitter_cli_loader;

/* Initialize a CLI loader for `emitter`
 *
 * The CLI loader must be used on a lexer/emitter that has already been initialized
 * itself (as per `tpp_emitter_init()`), though whether or not the its initial
 * file has already been initialized doesn't matter (the CLI loader will never
 * make persistent modifications to a lexer's current file/token). */
#define tpp_emitter_cli_loader_init(self, emitter)                                          \
	(void)((self)->TPP_EMITTER_INTERNAL(tecl_emitter) = (emitter),                          \
	       (self)->TPP_EMITTER_INTERNAL(tecl_state)   = TPP_EMITTER_CLI_LOADER_STATE_NORMAL \
	       _tpp_emitter_cli_loader_init_flags(self))
#define tpp_emitter_cli_loader_fini(self) \
	tpp_dbg_memset(self, sizeof(tpp_emitter_cli_loader))

/* Return the emitter that is being initialized by the given CLI loader. */
#define tpp_emitter_cli_loader_getemitter(self) \
	(self)->TPP_EMITTER_INTERNAL(tecl_emitter)

/* Check if a "--" argument was encountered during CLI parsing.
 * Once that is the case, `tpp_emitter_cli_loader_parsearg()` will
 * no longer accept additional CLI arguments, and all remaining
 * arguments should be treated as input files (for the compiler
 * that you're building) */
#define tpp_emitter_cli_loader_hasddash(self) \
	((self)->TPP_EMITTER_INTERNAL(tecl_state) == TPP_EMITTER_CLI_LOADER_STATE_DDASH)

/* Feed an argument to the loader. How exactly the argument is parsed
 * depends on the loader's current state, but sufficed to say: in its
 * default/initial state, `arg` is a CLI argument as you'd expect.
 *
 * WARNING: When you call this function, you must guaranty that `arg` remains
 *          valid, allocated, and unaltered until `tpp_emitter_cli_loader_fini()` is
 *          called.
 *
 * @return: TPP_EOK:       Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:    SOFT_ERROR: Argument could not be understood (but no
 *                         warning was emitted). You must either handle it yourself
 *                         by treating it as an argument for *your* compiler's
 *                         CLI, or as an input file for the lexer, or emit a
 *                         warning informing the user that their CLI argument
 *                         was not understood. You should also probably try to
 *                         pass it to `tpp_cli_loader_parsearg()`.
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parsearg(tpp_emitter_cli_loader *tpp_restrict self, char const *arg);

/* Try to parse a *flag*-style parameter, that is: an argument that actually consists
 * of multiple, tightly packed parameters, whilst having a singular, leading `-` (that
 * was already skipped by the caller).
 *
 * Example: `-PH` or `-HP`
 * - This argument consists of 2 flags `-H` and `-P`, which are simply concatenated
 *   into a single argument here. This function will then parse one of those flags
 *   from `**p_arg` (iow: `**p_arg` must be one of `H` or `P`), and advance `*p_arg`
 *   to either the end of the argument, or the next *flag*-style parameter.
 *
 * @return: TPP_EOK:    Success (`*p_arg` was updated to point to the next *flag*-style
 *                      parameter, or the argument string's end)
 * @return: TPP_ENOENT: Did not recognize the flag in `**p_arg` (caller should try to
 *                      handle the flag in a different context).
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parseflag(tpp_emitter_cli_loader *tpp_restrict self, char const **p_arg);

/* Convenience wrapper around `tpp_emitter_cli_loader_parsearg()`.
 * For more information, see `tpp_cli_loader_parseargv()`.
 *
 * @return: TPP_EOK:       Success (`*p_argc` and `*p_argv` were updated such that
 *                         they contain all unrecognized arguments, as well as all
 *                         input files for the lexer).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parseargv(tpp_emitter_cli_loader *tpp_restrict self,
                                 int *p_argc, char ***p_argv);

/* Ensure that `self` is in a *normal* state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_emitter_cli_loader_getemitter(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the `#include`-stack.
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_loader_flush(tpp_emitter_cli_loader *tpp_restrict self);

#if TPP_EMITTER_HAVE_CLI_HELP
/* Returns supported CLI parameters, and human-readable information
 * for them. Same format as `tpp_cli_loader_help` (see for more info) */
#if !TPP_USE_STATIC
TPP_CONST_DECL char const tpp_emitter_cli_loader_help[];
#endif /* !TPP_USE_STATIC */
#endif /* TPP_EMITTER_HAVE_CLI_HELP */
#endif /* TPP_EMITTER_HAVE_CLI */

TPP_DECL_END

#endif /* !GUARD_TPP_EMITTER_AMALGAMATION_H */
/* clang-format on */
