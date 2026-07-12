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
#ifndef GUARD_TPP_CONFIG_H
#define GUARD_TPP_CONFIG_H 1

#include "api.h"
#include "tuple.h"

/* Multi-file TPP needs to use its own "defs.h" for dynamic definitions
 * -- The amalgamation hard-overrides this macro with its own filename. */
#undef TPP_CONFIG_DEFS_FILENAME
#define TPP_CONFIG_DEFS_FILENAME "defs.h"

/* Multi-file TPP should be built using the "all" profile
 * -- Use the amalgamation if you want to use other profiles. */
#undef TPP_PROFILE
#define TPP_PROFILE TPP_PROFILE_ALL

/* Configure such that all tokens are enabled by default (different from
 * regular "TPP_PROFILE_ALL" where all tokens are still available, but
 * only tokens needed somewhere within the preprocessor itself are enabled
 * by default) */
#define TPP_COMMON_HAVE_TPP_TOK                TPP_CONF_EXT1
#define TPP_COMMON_HAVE_TPP_TOK_SPACE          TPP_CONF_EXT1
#define TPP_COMMON_HAVE_TPP_TOK_COMMENT        TPP_CONF_EXT1
#define TPP_COMMON_HAVE_TPP_TOK_GENERIC        TPP_CONF_EXT1
#define TPP_COMMON_HAVE_TPP_TOK_C_STRING       TPP_CONF_EXT1
#define TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING  TPP_CONF_EXT1
#define TPP_COMMON_HAVE_TPP_TOK_C_TOKENS       TPP_CONF_EXT1
#define TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS     TPP_CONF_EXT1
#define TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS    TPP_CONF_EXT1
#define TPP_HAVE_BUILTIN_EXPR_TT_IF_ELSE_IN_EXPRESSIONS TPP_CONF_EXT1

/*[[[tpp-begin]]]*/

/************************************************************************/
/*                                                                      */
/* To learn how to supply your own keyword/warning/extension/etc.       */
/* definitions to TPP3, see comment at start of "File: parts/defs.h"    */
/*                                                                      */
/************************************************************************/



/************************************************************************/
/* API limits:
 * - Positive: compile-time hard-code
 * - Negative: runtime configurable (with absolute value used as default)
 */
/************************************************************************/

/* TPP tab size: number of columns to assign to `U+0008` (`\t`) characters.
 * Used when calculating line/column information for messages, as well as
 * the builtin `__COLUMN__` macro (see `TPP_HAVE_MACRO___COLUMN__`)
 *
 * - when positive: compile-time hard-code
 * - when negative: runtime-configurable, with absolute value being used as default
 *
 * **Getter**: `tpp_gettabsize()`<br/>
 * **Setter**: `tpp_settabsize(v)`
 */
#ifndef TPP_TABSIZE
#define TPP_TABSIZE ((TPP_PROFILE == TPP_PROFILE_ALL) ? -4 : 4)
#endif /* !TPP_TABSIZE */

/* Max # of `TPP_WSTATE_ERROR` (continue compilation and fail later) warnings that
 * can be emitted before the next one will be treated as `TPP_WSTATE_FATAL` (fail
 * immediately)
 *
 * - `0`:  Set limit to `0` (when not also overwritten, causes `TPP_HAVE_WARNING_ERROR=0`)
 * - `N`:  Limit is hard-coded to `N` and cannot be overwritten at runtime
 * - `-N`: Limit can be overwritten at runtime, with `N` being used as the default
 *
 * **Getter**: `tpp_lexer_geterrorlimit(lexer)`<br/>
 * **Setter**: `tpp_lexer_seterrorlimit(lexer, v)`
 */
#ifndef TPP_ERROR_LIMIT
#define TPP_ERROR_LIMIT (-16)
#endif /* !TPP_ERROR_LIMIT */

/* Similar to GCC's `-fmax-include-depth`: Max # of times that the same file is allowed to
 * appear on the `#include`-stack before a warning `TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED`
 * (which is default-configured to be an error) is emitted.
 *
 * - `0`:  Disable include depth checks entirely (limit becomes *infinite*)
 * - `N`:  Limit is hard-coded to `N` and cannot be overwritten at runtime
 * - `-N`: Limit can be overwritten at runtime, with `N` being used as the default
 *
 * **Getter**: `tpp_lexer_getinclusionlimit(lexer)`<br/>
 * **Setter**: `tpp_lexer_setinclusionlimit(lexer, v)`
 */
#ifndef TPP_MAX_INCLUDE_DEPTH
#define TPP_MAX_INCLUDE_DEPTH (TPP_HAVE_INCLUDE_STACK ? (TPP_HAVE_PROFILE_NOT_MINIMAL ? -64 : 64) : 0)
#endif /* !TPP_MAX_INCLUDE_DEPTH */

/* Max # of times that the same (self-recursive; see `TPP_HAVE_MACRO_RECURSION`)
 * macro may appear on the `#include`-stack (with each instance's body having a
 * different expansion) before the macro will be treated as though its body
 * didn't change, causing it to not be expanded (anymore). At the same time
 * that this limit is reached, a warning `TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED`
 * will be emitted.
 *
 * NOTE: When `TPP_HAVE_MACRO_RECURSION` can never be enabled, this limit is
 *       entirely pointless, since the C standard (which we followed whenever
 *       `TPP_HAVE_MACRO_RECURSION` isn't enabled) essentially requires that
 *       this limit be `1`.
 *
 * - `0`:  Disable depth checks entirely (limit becomes *infinite*)
 * - `N`:  Limit is hard-coded to `N` and cannot be overwritten at runtime
 * - `-N`: Limit can be overwritten at runtime, with `N` being used as the default
 *
 * **Getter**: `tpp_lexer_getrecursivemacrolimit(lexer)`<br/>
 * **Setter**: `tpp_lexer_setrecursivemacrolimit(lexer, v)`
 */
#ifndef TPP_MAX_RECURSIVE_MACRO_DEPTH
#define TPP_MAX_RECURSIVE_MACRO_DEPTH (TPP_HAVE_MACRO_RECURSION ? (TPP_HAVE_PROFILE_NOT_MINIMAL ? -4096 : 4096) : 0)
#endif /* !TPP_MAX_RECURSIVE_MACRO_DEPTH */



/************************************************************************/
/* CONFIGURATION PROFILE                                                */
/************************************************************************/
#define TPP_PROFILE_MINIMAL 0 /* Disable everything, except dependencies of explicitly enabled features */
#define TPP_PROFILE_DEFAULT 1 /* Default configuration for a pretty good compromise between "TPP_PROFILE_MINIMAL" and "TPP_PROFILE_ALL" */
#define TPP_PROFILE_ALL     2 /* Enable (almost) all features, with everything configurable at runtime */
#define TPP_PROFILE_C       3 /* Enable features needed for a C compiler (warning: subjective) */
#define TPP_PROFILE_CXX     4 /* Enable features needed for a C++ compiler (warning: subjective) */

/* General configuration to determine default configuration of preprocessor features */
#ifndef TPP_PROFILE
#define TPP_PROFILE TPP_PROFILE_DEFAULT
#endif /* !TPP_PROFILE */

#undef TPP_HAVE_PROFILE_DEFAULT
#if (TPP_PROFILE == TPP_PROFILE_ALL || \
     TPP_PROFILE == TPP_PROFILE_DEFAULT)
#define TPP_HAVE_PROFILE_DEFAULT 1
#else /* ... */
#define TPP_HAVE_PROFILE_DEFAULT 0
#endif /* !... */

#undef TPP_HAVE_PROFILE_C_LIKE
#if (TPP_PROFILE == TPP_PROFILE_C || \
     TPP_PROFILE == TPP_PROFILE_CXX)
#define TPP_HAVE_PROFILE_C_LIKE 1
#else /* ... */
#define TPP_HAVE_PROFILE_C_LIKE 0
#endif /* !... */

#undef TPP_HAVE_PROFILE_NOT_MINIMAL
#if (TPP_PROFILE != TPP_PROFILE_MINIMAL)
#define TPP_HAVE_PROFILE_NOT_MINIMAL 1
#else /* ... */
#define TPP_HAVE_PROFILE_NOT_MINIMAL 0
#endif /* !... */



/************************************************************************/
/* CORE "TPP_HAVE_*" API-FEATURES
 *  0: Disabled
 *  1: Enabled */
/************************************************************************/

/* When defined to non-zero, disable checks for invalid/nonsensical configurations.
 * Such configurations may still be able to compile, but will include definitely
 * redundant code, or other features that are meaningless in relation to some other
 * configuration.
 *
 * You should only enable this if you _need_ _TPP_ _to_ _build_ _**NOW**_ and one of
 * those internal checks stands in your way. You should not leave this enabled, as
 * every one of the errors this disables is there for a reason! */
#ifndef TPP_IGNORE_INVALID_CONFIGURATION
#define TPP_IGNORE_INVALID_CONFIGURATION 0
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */

/* API support for non-blocking I/O. Must also be enabled on a per-file basis
 * by setting the file's `TPP_FILE_FLAGS_NONBLOCK` flag. Also: calls made to
 * `tpp_lexer_yield_blocking()` & friends can be used to force calls to become
 * blocking, even when enabled for the current file (s.a. `TPP_EWOULDBLOCK`
 * and `TPP_TOK_EWOULDBLOCK`) */
#ifndef TPP_HAVE_FILE_NONBLOCK
#define TPP_HAVE_FILE_NONBLOCK (TPP_HAVE_PROFILE_NOT_MINIMAL && (TPP_OS_WINDOWS || TPP_OS_UNIX))
#endif /* !TPP_HAVE_FILE_NONBLOCK */

/* Unicode support:
 * - Use of a unicode traits database that is either user-supplied,
 *   a dummy (where all unicode character are allows in identifiers),
 *   or built-in when `TPP_HAVE_BUILTIN_CTYPE_UNICODE` is enabled.
 * - Automatic detection of `utf-8`, `utf-8-bom`, `utf-16[le/be]`, `utf-32[le/be]` in input files.<br/>
 *   NOTE: The Lexer assumes `utf-8` by default, unless it encounters an invalid `utf-8`
 *         byte sequence, at which point it will automatically downgrade to `ASCII`-only
 *         for the remainder of the relevant file */
#ifndef TPP_HAVE_UNICODE
#define TPP_HAVE_UNICODE 1
#endif /* !TPP_HAVE_UNICODE */

#if ((defined(tpp_unicode_isspace_nolf) || \
      defined(tpp_unicode_issymstrt) ||    \
      defined(tpp_unicode_issymcont) ||    \
      defined(tpp_unicode_isspace) ||      \
      defined(tpp_unicode_islf)) &&        \
     !defined(TPP_HAVE_BUILTIN_CTYPE_UNICODE))
#define TPP_HAVE_BUILTIN_CTYPE_UNICODE 0 /* There are user-supplied unicode trait functions */
#endif /* !... */

/* Supply a built-in unicode character traits database (adds ~36KiB data to final executable).
 * Some examples of stuff that is supported when this is enabled:
 * - `U+0085` (`NEL`)  will be treated like `U+000A` (`LF` `\n`)
 * - `U+00A0` (`NBSP`) will be treated like `U+0020` (`space` `\s`)
 * - `U+00DF` (`ß`)    will be treated accepted as part of an identifier
 *
 * As such, TPP3 follows C23+-specifications, in that the builtin unicode
 * database is set-up to accept identifiers made up of `XID_Start`/`XID_Cont`.
 *
 * XXX: Config where `[C99,C23)`-compliant identifiers are recognized (i.e.: where 🐱 is an identifier)
 *      https://en.cppreference.com/c/language/identifier */
#ifndef TPP_HAVE_BUILTIN_CTYPE_UNICODE
#define TPP_HAVE_BUILTIN_CTYPE_UNICODE (TPP_HAVE_UNICODE && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_BUILTIN_CTYPE_UNICODE */

/* Provide a function `tpp_strerror()` to get a description of a given `tpp_errno` error code */
#ifndef TPP_HAVE_STRERROR
#define TPP_HAVE_STRERROR (TPP_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_HAVE_STRERROR */

/* Provide a function `tpp_strtokenid()` to get the API name of a (non-keyword) token ID */
#ifndef TPP_HAVE_STRTOKENID
#define TPP_HAVE_STRTOKENID (TPP_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_HAVE_STRTOKENID */

/* Enable support for storing custom user-data in keywords */
#ifndef TPP_HAVE_KEYWORD_USERDATA
#define TPP_HAVE_KEYWORD_USERDATA (TPP_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_HAVE_KEYWORD_USERDATA */

/* Provide support for `tpp_keyword_asstring()` that allows
 * keywords to be binary-compatible with `tpp_string` */
#ifndef TPP_HAVE_KEYWORD_ASSTRING
#define TPP_HAVE_KEYWORD_ASSTRING (TPP_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_HAVE_KEYWORD_ASSTRING */

/* Include a counter for how often a specific I/O-file appears on the
 * `#include`-stack, with that counter being stored within its filename
 * keyword (used to speed up `TPP_MAX_INCLUDE_DEPTH` error checks, though
 * those checks also work without this feature) */
#ifndef TPP_HAVE_KEYWORD_INCLCOUNT
#define TPP_HAVE_KEYWORD_INCLCOUNT (TPP_HAVE_PROFILE_NOT_MINIMAL && TPP_MAX_INCLUDE_DEPTH != 0)
#endif /* !TPP_HAVE_KEYWORD_INCLCOUNT */

/* Enable support for runtime-configurable extensions */
#ifndef TPP_HAVE_EXTENSIONS
#define TPP_HAVE_EXTENSIONS TPP_HAVE_PROFILE_NOT_MINIMAL
#endif /* !TPP_HAVE_EXTENSIONS */

/* Enable support to push/pop the extension state */
#ifndef TPP_HAVE_EXTENSIONS_PUSH_POP
#define TPP_HAVE_EXTENSIONS_PUSH_POP (TPP_HAVE_EXTENSIONS && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_EXTENSIONS_PUSH_POP */

/* Support for: compiler warnings (else: behave as though all warnings were being suppressed) */
#ifndef TPP_HAVE_WARNINGS
#define TPP_HAVE_WARNINGS TPP_HAVE_PROFILE_NOT_MINIMAL
#endif /* !TPP_HAVE_WARNINGS */

/* Enable support to push/pop the warning state */
#ifndef TPP_HAVE_WARNINGS_PUSH_POP
#define TPP_HAVE_WARNINGS_PUSH_POP (TPP_HAVE_WARNINGS && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_WARNINGS_PUSH_POP */

/* Support for: `tpp_warning_ofnumber()` and `tpp_warning_getnumbers()`
 *
 * When `TPP_HAVE_PRAGMA_WARNING` is also enabled, user-code will also
 * be able to configure warnings based on their ID. */
#ifndef TPP_HAVE_WARNING_NUMBERS
#define TPP_HAVE_WARNING_NUMBERS (TPP_HAVE_WARNINGS && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_WARNING_NUMBERS */

/* Support for: `TPP_WSTATE_ERROR` (else: only `TPP_WSTATE_FATAL` is available) */
#ifndef TPP_HAVE_WARNING_ERROR
#define TPP_HAVE_WARNING_ERROR (TPP_HAVE_WARNINGS && TPP_ERROR_LIMIT != 0)
#endif /* !TPP_HAVE_WARNING_ERROR */

/* Support for: `TPP_WSTATE_SUPPRESS` */
#ifndef TPP_HAVE_WARNING_SUPPRESS
#define TPP_HAVE_WARNING_SUPPRESS (TPP_HAVE_WARNINGS && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_WARNING_SUPPRESS */

/* Support for: `TPP_WSTATE_DEFAULT` */
#ifndef TPP_HAVE_WARNING_DEFAULT
#define TPP_HAVE_WARNING_DEFAULT (TPP_HAVE_WARNINGS && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_WARNING_DEFAULT */

/* Enable support for `TPP_FILE_FLAGS_NOCLOSE` */
#ifndef TPP_HAVE_FILE_NOCLOSE
#define TPP_HAVE_FILE_NOCLOSE (TPP_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_HAVE_FILE_NOCLOSE */

/* Enable support for `TPP_FILE_FLAGS_NOKWD` */
#ifndef TPP_HAVE_FILE_NOKWD
#define TPP_HAVE_FILE_NOKWD TPP_HAVE_PROFILE_NOT_MINIMAL
#endif /* !TPP_HAVE_FILE_NOKWD */

/* Speed up calls to `tpp_file_getlcinfo()` by caching the last-read
 * position and determining line/column information as a delta from
 * what was previously cached */
#ifndef TPP_HAVE_FILE_LC_CACHE
#define TPP_HAVE_FILE_LC_CACHE TPP_HAVE_PROFILE_NOT_MINIMAL
#endif /* !TPP_HAVE_FILE_LC_CACHE */

/* Enable support for detecting `\r\n` as a singular line-feed */
#ifndef TPP_HAVE_CR_LF_DETECTION
#define TPP_HAVE_CR_LF_DETECTION 1
#endif /* !TPP_HAVE_CR_LF_DETECTION */

/* Provide a function `tpp_lexer_copy()` that can be used to duplicate a lexer. */
#ifndef TPP_HAVE_LEXER_COPY
#define TPP_HAVE_LEXER_COPY (TPP_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_HAVE_LEXER_COPY */

/* Lexers keep track of the # of warnings they've emitted over their lifetime. Not
 * actually used for anything, but can be read using `tpp_lexer_getwarningcount()` */
#ifndef TPP_HAVE_LEXER_WARNING_COUNT
#define TPP_HAVE_LEXER_WARNING_COUNT (TPP_HAVE_WARNINGS && (TPP_PROFILE == TPP_PROFILE_ALL))
#endif /* !TPP_HAVE_LEXER_WARNING_COUNT */
/************************************************************************/
/************************************************************************/
/************************************************************************/


/* All TPP_HAVE_* options (with "-f*"-style comments) can be configured as:
 * - TPP_CONF_1     : Compile-time enabled  (always on; no #pragma extension("-f...") / TPP_FEAT_* available)
 * - TPP_CONF_0     : Compile-time disabled
 * - TPP_CONF_EXT1  : Runtime-configurable (via #pragma extension("-f...") / TPP_EXT_*, default = true)
 * - TPP_CONF_EXT0  : Runtime-configurable (via #pragma extension("-f...") / TPP_EXT_*, default = false)
 * - TPP_CONF_FEAT1 : Runtime-configurable (via TPP_FEAT_*, default = true)
 * - TPP_CONF_FEAT0 : Runtime-configurable (via TPP_FEAT_*, default = false)
 *
 * In the case of "TPP_CONF_EXT1" / "TPP_CONF_EXT0", the extension's name will be the
 * "-f..." comment, but can be overwritten via #define TPP_EXTNAME_<name> "my-name":
 * ```c
 * #define TPP_HAVE_TRIGRAPHS    TPP_CONF_EXT1
 * #define TPP_EXTNAME_TRIGRAPHS "the-cool-trigraphs"
 * ```
 *
 * User-code can then control the feature using:
 * ```c
 * #pragma extension("-fthe-cool-trigraphs")
 * ```
 *
 * WARNING: Use of "TPP_CONF_EXT1" / "TPP_CONF_EXT0" requires "#define TPP_HAVE_EXTENSIONS 1"
 */

/* Possible values for configs */
#define TPP_CONF_1     1    /* Compile-time enabled  (always on; no #pragma extension("-f...") / TPP_FEAT_* available) */
#define TPP_CONF_0     0    /* Compile-time disabled */
#define TPP_CONF_EXT1  (-1) /* Runtime-configurable (via #pragma extension("-f...") / TPP_EXT_*, default = true) */
#define TPP_CONF_EXT0  (-2) /* Runtime-configurable (via #pragma extension("-f...") / TPP_EXT_*, default = false) */
#define TPP_CONF_FEAT1 (-3) /* Runtime-configurable (via TPP_FEAT_*, default = true) */
#define TPP_CONF_FEAT0 (-4) /* Runtime-configurable (via TPP_FEAT_*, default = false) */
#if !TPP_HAVE_EXTENSIONS
#undef TPP_CONF_EXT1
#undef TPP_CONF_EXT0
#define TPP_CONF_EXT1 TPP_CONF_FEAT1
#define TPP_CONF_EXT0 TPP_CONF_FEAT0
#endif /* !TPP_HAVE_EXTENSIONS */

#define TPP_CONF_MAYBE_1(cfg)      cfg                  /* Config may be enabled (guarantied to just be "cfg [!= 0]") */
#define TPP_CONF_MAYBE_0(cfg)      ((cfg) <= 0)         /* Config may be disabled */
#define TPP_CONF_IS_FEAT(cfg)      ((cfg) <= -3)        /* Should config be runtime-configurable as a feature? */
#define TPP_CONF_IS_EXT(cfg)       (((cfg) & ~1) == -2) /* Should config be runtime-configurable as an extension? */
#define TPP_CONF_IS_CONST(cfg)     ((cfg) >= 0)         /* Should config be compile-time only? */
#define TPP_CONF_IS_RT(cfg)        ((cfg) < 0)          /* Should config be runtime configurable? */
#define TPP_CONF_DEFAULT(cfg)      ((cfg) & 1)          /* Default state of config */
#define TPP_CONF_MAKEFEAT(default) (-4 + !!(default))   /* Configure as feature */
#define TPP_CONF_MAKEEXT(default)  (-2 + !!(default))   /* Configure as extension */

/* Default configuration for tokens */
#ifndef TPP_COMMON_HAVE_TPP_TOK
#define TPP_COMMON_HAVE_TPP_TOK ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_COMMON_HAVE_TPP_TOK */

/* Default configuration for 1-char tokens */
#ifndef TPP_COMMON_HAVE_TPP_TOK_1CHAR
#define TPP_COMMON_HAVE_TPP_TOK_1CHAR (TPP_HAVE_PROFILE_NOT_MINIMAL ? TPP_CONF_EXT0 : 0)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_1CHAR */

/* Default configuration for `TPP_HAVE_TPP_TOK_LF`, `TPP_HAVE_TPP_TOK_SPACE`,
 * `TPP_HAVE_TPP_TOK_COMMENT`, indicating if tokens that are normally through
 * of as no-ops should be emitted by `tpp_lexer_yieldpp()` */
#ifndef TPP_COMMON_HAVE_TPP_TOK_SPACE
#define TPP_COMMON_HAVE_TPP_TOK_SPACE (TPP_HAVE_PROFILE_NOT_MINIMAL ? TPP_CONF_FEAT1 : 1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_SPACE */

/* Default configuration describing if otherwise unconfigured comment tokens should be supported */
#ifndef TPP_COMMON_HAVE_TPP_TOK_COMMENT
#define TPP_COMMON_HAVE_TPP_TOK_COMMENT ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_COMMON_HAVE_TPP_TOK : 0)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_COMMENT */

/* Default configuration for:
 * - `TPP_HAVE_TPP_TOK_INT`
 * - `TPP_HAVE_TPP_TOK_FLOAT`
 * - `TPP_HAVE_TPP_TOK_C_CHAR`
 * - `TPP_HAVE_TPP_TOK_C_STRING` */
#ifndef TPP_COMMON_HAVE_TPP_TOK_GENERIC
#define TPP_COMMON_HAVE_TPP_TOK_GENERIC TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_GENERIC */

/* Default configuration for extended C/C++ string tokens:
 * - `TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL` */
#ifndef TPP_COMMON_HAVE_TPP_TOK_C_STRING
#define TPP_COMMON_HAVE_TPP_TOK_C_STRING (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_FEAT0 : TPP_HAVE_PROFILE_C_LIKE)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_C_STRING */

/* Default configuration for deemon-specific string tokens:
 * - `TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL`
 * - `TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL` */
#ifndef TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
#define TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING */

/* Default configuration for tokens needed to implement a C- or C++-compiler */
#ifndef TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_C_TOKENS (TPP_HAVE_PROFILE_DEFAULT ? TPP_COMMON_HAVE_TPP_TOK : TPP_HAVE_PROFILE_C_LIKE)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_C_TOKENS */

/* Default configuration for tokens needed to implement a C++-compiler */
#ifndef TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : (TPP_PROFILE == TPP_PROFILE_CXX))
#endif /* !TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS */

/* Default configuration for tokens not covered by any
 * of the other `TPP_COMMON_HAVE_TPP_TOK_*` configs */
#ifndef TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS */

/* Default configuration for constructs defined by some C/C++ standard */
#ifndef TPP_COMMON_HAVE_CPP_DIRECTIVES_STD
#define TPP_COMMON_HAVE_CPP_DIRECTIVES_STD ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_HAVE_PROFILE_DEFAULT || TPP_HAVE_PROFILE_C_LIKE))
#endif /* !TPP_COMMON_HAVE_CPP_DIRECTIVES_STD */

/* Default configuration for constructs that should be considered extensions */
#ifndef TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT
#define TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_HAVE_PROFILE_DEFAULT || TPP_HAVE_PROFILE_C_LIKE))
#endif /* !TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT */
/************************************************************************/
/************************************************************************/
/************************************************************************/




/************************************************************************/
/* PREPROCESSOR FEATURES                                                */
/************************************************************************/

// Support for `\`-escaped line continuation: when a line ends with a `\` character
// that is immediately (but see `TPP_HAVE_BSE_WHITESPACE`) followed by a linefeed,
// then that linefeed is never yielded, and a potential multi-character token is
// continued:
// ```c
// foo\ 
// bar // Produces a single token "foobar"
// +\ 
// =   // Produces a single token "+="
// ```
//
// This DOES affect the line-continuation features of `#define` macro definitions.
// When this is disabled, `\`-escaped line continuation can't be used there, either.
// @detect: #define HAVE_BSE
//          // \ 
//          #undef HAVE_BSE
//          #ifdef HAVE_BSE
#ifndef TPP_HAVE_BSE
#define TPP_HAVE_BSE (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) /* "-fbse" */
#endif /* !TPP_HAVE_BSE */

/* Extension to `TPP_HAVE_BSE`: the `\` character is allowed to be followed by extra
 * whitespace preceding the actual linefeed:
 * ```c
 * #define multi_line line1 \␣␣␣
 *                    line2
 * ```
 *
 * @detect: #if __TPP_COUNT_TOKENS("a\\ \nb") == 1 */
#ifndef TPP_HAVE_BSE_WHITESPACE
#define TPP_HAVE_BSE_WHITESPACE (TPP_HAVE_BSE ? TPP_CONF_EXT1 : 0) /* "-fbse-whitespace" */
#endif /* !TPP_HAVE_BSE_WHITESPACE */

/* Support for `\uABCD` and `\U01234567` in identifier names (will be
 * replaced with effective UTF-8 encodings when translated to keywords)
 * ```c
 * int identifier\u0020with\u0020whitespace = 42;
 * // Same as:
 * int __TPP_IDENTIFIER("identifier with whitespace") = 42;
 * ```
 *
 * @detect: #define foo\u0062ar
 *          #ifdef foobar */
#ifndef TPP_HAVE_ESCAPE_IN_IDENTIFIERS
#define TPP_HAVE_ESCAPE_IN_IDENTIFIERS (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) /* "-fextended-identifiers" */
#endif /* !TPP_HAVE_ESCAPE_IN_IDENTIFIERS */

/* Support for `\e` (for `U+001B`) escape sequences:
 * ```c
 * printf("Error: \e[31m%d\e[0m", errno);
 * ``` */
#ifndef TPP_HAVE_ESCAPE_E_IN_STRINGS
#define TPP_HAVE_ESCAPE_E_IN_STRINGS (TPP_HAVE_STRING_ESCAPE ? TPP_CONF_EXT1 : 0) /* "-fescape-e-in-strings" */
#endif /* !TPP_HAVE_ESCAPE_E_IN_STRINGS */

/* Support for `\s` (for `U+0020`) escape sequences:
 * ```java
 * System.out.println("""
 *     This line has visible trailing whitespace:    \s
 *     """);
 * ```  */
#ifndef TPP_HAVE_ESCAPE_S_IN_STRINGS
#define TPP_HAVE_ESCAPE_S_IN_STRINGS (TPP_HAVE_STRING_ESCAPE ? TPP_CONF_EXT1 : 0) /* "-fescape-s-in-strings" */
#endif /* !TPP_HAVE_ESCAPE_S_IN_STRINGS */

/* Specifies if *any* CPP directives are supported */
#ifndef TPP_HAVE_CPP_DIRECTIVES
#define TPP_HAVE_CPP_DIRECTIVES ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL)) /* "-fcpp-directives" */
#endif /* !TPP_HAVE_CPP_DIRECTIVES */

/* Support for C-style macros */
#ifndef TPP_HAVE_CPP_MACROS
#define TPP_HAVE_CPP_MACROS (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fcpp-macros" */
#endif /* !TPP_HAVE_CPP_MACROS */

/* Enable support for magic whitespace insertions where failure
 * to do so would result in accidental token concatenation:
 * ```c
 * #define FOO() foo
 * #define BAR   bar
 * #define SCAN(x) x
 * FOO()BAR          // OK: Expands to [foo][bar] (works independent of `TPP_HAVE_MAGIC_WHITESPACE`)
 * SCAN(FOO()BAR)    // Expands to [foo][ ][bar]  (or [foobar] when `TPP_HAVE_MAGIC_WHITESPACE` is disabled)
 * ```
 *
 * The extra space (U+0020) character in `SCAN(FOO()BAR)` gets added
 * during macro argument substitution in the call to `SCAN`, and is
 * necessary because TPP is a text-based preprocessor. Trying to get
 * L/C information on the associated `TPP_TOK_SPACE` will fail.
 *
 * @detect: #define FOO()         foo
 *          #define BAR           bar
 *          #define SCAN2(x)      pre##x##post
 *          #define SCAN(x)       SCAN2(x)
 *          #define prefoobarpost 0
 *          #define prefoo        1
 *          #define barpost       +1
 *          #if SCAN(FOO()BAR) // "0" if disabled (non-standard-conforming, like mscv);
 *                             // "1 +1" if enabled (standard-conforming, like gcc)
 */
#ifndef TPP_HAVE_MAGIC_WHITESPACE
#define TPP_HAVE_MAGIC_WHITESPACE ((TPP_HAVE_CPP_MACROS || TPP_HAVE_MACRO___TPP_EXEC) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fmagic-whitespace" */
#endif /* !TPP_HAVE_MAGIC_WHITESPACE */

/* Support for builtin C-style macros (require `TPP_HAVE_CPP_MACROS` to be enabled, too) */
#ifndef TPP_HAVE_CPP_BUILTIN_MACROS
#define TPP_HAVE_CPP_BUILTIN_MACROS (TPP_HAVE_CPP_MACROS ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fcpp-builtin-macros" */
#endif /* !TPP_HAVE_CPP_BUILTIN_MACROS */

/* Support for `#!foobar`-directives (which are treated as comments) */
#ifndef TPP_HAVE_CPP_EXCLAIM
#define TPP_HAVE_CPP_EXCLAIM (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fshebang-directives" */
#endif /* !TPP_HAVE_CPP_EXCLAIM */

/* Support for `#`-directives (blank directives), which are ignored */
#ifndef TPP_HAVE_CPP_BLANK
#define TPP_HAVE_CPP_BLANK (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fblank-directives" */
#endif /* !TPP_HAVE_CPP_BLANK */

/* Support for `# 42 "foo.h" 1 2 3 4`-directives.
 *
 * These behave similar to `#line`-directives, except that following
 * the filename, a number of additional "flags" can be specified:
 * - `1`: Push a dummy-file containing the old file/line/column onto the `#include`-stack,
 *        before applying the new line/filename. After being pushed, said last position will
 *        be displayed as part of warning messages. This flag require `TPP_HAVE_FILE_DUMMY`
 *        to be enabled, otherwise it is ignored.
 * - `2`: Do the inverse of flag `1` and pop a dummy-file off the `#include`-stack. Like the
 *        `1` flag, this flag require `TPP_HAVE_FILE_DUMMY` to be enabled, otherwise it is
 *        ignored.
 * - `3`: Set `TPP_FILE_FLAGS_SYSHDR` for the current text-file. When this flag is not
 *        supplied, `TPP_FILE_FLAGS_SYSHDR` is instead cleared for the current text-file.
 *        This flag requires `TPP_HAVE_FILE_SYSHDR` to be enabled, otherwise it is ignored.
 * - `4`: Same as flag `3`, except for the `TPP_FILE_FLAGS_EXTERN_C` flag. Similarly, this
 *        flag requires `TPP_HAVE_FILE_EXTERN_C` to be enabled, otherwise it is ignored. */
#ifndef TPP_HAVE_CPP_DIGIT_LINE
#define TPP_HAVE_CPP_DIGIT_LINE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fdigit-directives" */
#endif /* !TPP_HAVE_CPP_DIGIT_LINE */

/* Support for `#line 42 "foo.h"`-directives */
#ifndef TPP_HAVE_CPP_LINE
#define TPP_HAVE_CPP_LINE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fline-directives" */
#endif /* !TPP_HAVE_CPP_LINE */

/* Support for `#include <stdio.h>`-directives */
#ifndef TPP_HAVE_CPP_INCLUDE
#define TPP_HAVE_CPP_INCLUDE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-finclude-directives" */
#endif /* !TPP_HAVE_CPP_INCLUDE */

/* Support for `#include_next <stdio.h>`-directives */
#ifndef TPP_HAVE_CPP_INCLUDE_NEXT
#define TPP_HAVE_CPP_INCLUDE_NEXT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-finclude-next-directives" */
#endif /* !TPP_HAVE_CPP_INCLUDE_NEXT */

/* Support for `#import <stdio.h>`-directives */
#ifndef TPP_HAVE_CPP_IMPORT
#define TPP_HAVE_CPP_IMPORT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fimport-directives" */
#endif /* !TPP_HAVE_CPP_IMPORT */

/* Support for: `#if`, `#ifdef`, `#ifndef`, `#elif`, `#elifdef`, `#elifndef`, `#else`, `#endif` */
#ifndef TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_HAVE_CPP_IF_ELSE_ENDIF ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fif-directives" */
#endif /* !TPP_HAVE_CPP_IF_ELSE_ENDIF */

/* Support for: `#define`, `#undef` */
#ifndef TPP_HAVE_CPP_DEFINE
#define TPP_HAVE_CPP_DEFINE ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fdefine-directives" */
#endif /* !TPP_HAVE_CPP_DEFINE */

/* Support for: `#assert`, `#unassert`
 *
 * see: https://gcc.gnu.org/onlinedocs/cpp/Obsolete-Features.html */
#ifndef TPP_HAVE_CPP_ASSERT
#define TPP_HAVE_CPP_ASSERT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fassertions" */
#endif /* !TPP_HAVE_CPP_ASSERT */

/* Support for: `#error` */
#ifndef TPP_HAVE_CPP_ERROR
#define TPP_HAVE_CPP_ERROR (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-ferror-directives" */
#endif /* !TPP_HAVE_CPP_ERROR */

/* Support for: `#warning` */
#ifndef TPP_HAVE_CPP_WARNING
#define TPP_HAVE_CPP_WARNING (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fwarning-directives" */
#endif /* !TPP_HAVE_CPP_WARNING */

/* Support for: `#ident`, `#sccs` */
#ifndef TPP_HAVE_CPP_IDENT_SCCS
#define TPP_HAVE_CPP_IDENT_SCCS (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fident-directives" */
#endif /* !TPP_HAVE_CPP_IDENT_SCCS */

/* Support for: `#pragma` */
#ifndef TPP_HAVE_CPP_PRAGMA
#define TPP_HAVE_CPP_PRAGMA (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fpragma-directives" */
#endif /* !TPP_HAVE_CPP_PRAGMA */

/* Support for: `#embed` */
#ifndef TPP_HAVE_CPP_EMBED
#define TPP_HAVE_CPP_EMBED (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fembed-directives" */
#endif /* !TPP_HAVE_CPP_EMBED */

/* Support for: `_Pragma("foo")`
 * @detect: #ifdef _Pragma */
#ifndef TPP_HAVE_MACRO__Pragma
#define TPP_HAVE_MACRO__Pragma (TPP_HAVE_CPP_BUILTIN_MACROS ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-f_Pragma" */
#endif /* !TPP_HAVE_MACRO__Pragma */

/* Support for: `__pragma(foo)`
 * @detect: #ifdef __pragma */
#ifndef TPP_HAVE_MACRO___pragma
#define TPP_HAVE_MACRO___pragma (TPP_HAVE_CPP_BUILTIN_MACROS ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-f__pragma" */
#endif /* !TPP_HAVE_MACRO___pragma */


/* Support for pragma directives */
#undef TPP_HAVE_PRAGMA
#if (TPP_HAVE_CPP_PRAGMA ||    \
     TPP_HAVE_MACRO__Pragma || \
     TPP_HAVE_MACRO___pragma)
#define TPP_HAVE_PRAGMA 1
#else /* ... */
#define TPP_HAVE_PRAGMA 0
#endif /* !... */


/* Support for clang `__has_attribute()`, which is conventionally
 * used to check support of `__attribute__((foo))` in C/C++ compilers.
 *
 * @detect: #ifdef __has_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_attribute
#define TPP_HAVE_CLANG_MACRO___has_attribute (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_attribute */

/* Support for clang `__has_builtin()`, which is conventionally
 * used to check support of `__builtin_foo()` in C/C++ compilers.
 *
 * @detect: #ifdef __has_builtin */
#ifndef TPP_HAVE_CLANG_MACRO___has_builtin
#define TPP_HAVE_CLANG_MACRO___has_builtin (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_builtin" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_builtin */

/* Support for clang `__has_cpp_attribute()`, which is conventionally
 * used to check support of `[[foo]]` in C++ compilers.
 *
 * @detect: #ifdef __has_cpp_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_cpp_attribute
#define TPP_HAVE_CLANG_MACRO___has_cpp_attribute (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_cpp_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_cpp_attribute */

/* Support for clang `__has_declspec_attribute()`, which is conventionally
 * used to check support of `__declspec(foo)` in C/C++ compilers.
 *
 * @detect: #ifdef __has_declspec_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_declspec_attribute
#define TPP_HAVE_CLANG_MACRO___has_declspec_attribute (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_declspec_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_declspec_attribute */

/* Support for clang `__has_extension()`, which is conventionally
 * used to check support of compiler extensions in C/C++ compilers.
 *
 * NOTE: Another builtin macro of the same name exists and can be
 *       enabled via `TPP_HAVE_MACRO___has_extension`. The 2 macros
 *       don't conflict with each other though (both can safely be
 *       enabled at the same time), since TPP's `__has_extension()`
 *       takes a string, whilst this one takes a keyword/identifier.
 *
 * @detect: #ifdef __has_extension */
#ifndef TPP_HAVE_CLANG_MACRO___has_extension
#define TPP_HAVE_CLANG_MACRO___has_extension (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_extension" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_extension */

/* Support for clang `__has_feature()`, which is conventionally
 * used to check support of compiler features in C/C++ compilers.
 *
 * @detect: #ifdef __has_feature */
#ifndef TPP_HAVE_CLANG_MACRO___has_feature
#define TPP_HAVE_CLANG_MACRO___has_feature (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_feature" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_feature */

/* Support for clang `__has_c_attribute()`, which is conventionally
 * used to check support of `[[foo]]` in C+ compilers.
 *
 * @detect: #ifdef __has_c_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_c_attribute
#define TPP_HAVE_CLANG_MACRO___has_c_attribute (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_c_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_c_attribute */

/* When enabled, clang's `__has_feature()` also
 * expands to `1` when `__has_extension()` would.
 *
 * See also:
 * - `TPP_HAVE_CLANG_MACRO___has_feature`
 * - `TPP_HAVE_CLANG_MACRO___has_extension` */
#ifndef TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES
#define TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES ((TPP_HAVE_CLANG_MACRO___has_extension && TPP_HAVE_CLANG_MACRO___has_feature) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0) /* "-fclang-extensions-are-features" */
#endif /* !TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES */

/* Support for clang `__is_identifier()`, which can be used
 * to check if a given keyword is a builtin identifier:
 * ```c
 * #if __is_identifier(tpp_exec)
 * // "#pragma tpp_exec()" is probably supported then...
 * #endif
 * ```
 *
 * A keyword is considered to be an "identifier" if `TPP_TOK_ISBUILTINKEYWORD()`
 *
 * @detect: #ifdef __is_identifier */
#ifndef TPP_HAVE_MACRO___is_identifier
#define TPP_HAVE_MACRO___is_identifier (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__is_identifier" */
#endif /* !TPP_HAVE_MACRO___is_identifier */

/* Support for TPP's `__is_deprecated()`, which can be used to
 * check if an identifier has been `#pragma deprecated("foo")`-ed
 *
 * @detect: #ifdef __is_deprecated */
#ifndef TPP_HAVE_MACRO___is_deprecated
#define TPP_HAVE_MACRO___is_deprecated (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__is_deprecated" */
#endif /* !TPP_HAVE_MACRO___is_deprecated */

/* Support for TPP's `__is_poisoned()`, which can be used to
 * check if an identifier has been `#pragma GCC poison foo`-ed
 *
 * @detect: #ifdef __is_poisoned */
#ifndef TPP_HAVE_MACRO___is_poisoned
#define TPP_HAVE_MACRO___is_poisoned (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__is_poisoned" */
#endif /* !TPP_HAVE_MACRO___is_poisoned */

/* Support for TPP's `__has_extension()`, which can be
 * used to check if a TPP extension is known + enabled:
 * ```c
 * #if __has_extension("-ftrigraphs")
 * ...
 * #endif
 * ```
 *
 * @detect: #ifdef __has_extension */
#ifndef TPP_HAVE_MACRO___has_extension
#define TPP_HAVE_MACRO___has_extension ((TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_extension" */
#endif /* !TPP_HAVE_MACRO___has_extension */

/* Support for TPP's `__has_known_extension()`, which
 * can be used to check if a TPP extension is known:
 * ```c
 * #if __has_known_extension("-ftrigraphs")
 * ...
 * #endif
 * ```
 *
 * @detect: #ifdef __has_known_extension */
#ifndef TPP_HAVE_MACRO___has_known_extension
#define TPP_HAVE_MACRO___has_known_extension ((TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_known_extension" */
#endif /* !TPP_HAVE_MACRO___has_known_extension */

/* Support for TPP's `__has_warning()`, which can be
 * used to check if a TPP warning is known + enabled:
 * ```c
 * #if __has_warning("-Wmultiline-string")
 * ...
 * #endif
 * ```
 *
 * @detect: #ifdef __has_warning */
#ifndef TPP_HAVE_MACRO___has_warning
#define TPP_HAVE_MACRO___has_warning ((TPP_HAVE_WARNINGS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_warning" */
#endif /* !TPP_HAVE_MACRO___has_warning */

/* Support for TPP's `__has_known_warning()`, which
 * can be used to check if a TPP warning is known:
 * ```c
 * #if __has_known_warning("-Wmultiline-string")
 * ...
 * #endif
 * ```
 *
 * @detect: #ifdef __has_known_warning */
#ifndef TPP_HAVE_MACRO___has_known_warning
#define TPP_HAVE_MACRO___has_known_warning ((TPP_HAVE_WARNINGS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_known_warning" */
#endif /* !TPP_HAVE_MACRO___has_known_warning */

/* Support for clang `__has_include()`, which
 * can be used to check if `#include` exists:
 * ```c
 * #if __has_include(<stdio.h>)
 * #include <stdio.h>
 * #endif
 * ```
 *
 * @detect: #ifdef __has_include */
#ifndef TPP_HAVE_MACRO___has_include
#define TPP_HAVE_MACRO___has_include (((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_IMPORT) && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_include" */
#endif /* !TPP_HAVE_MACRO___has_include */

/* Support for clang `__has_include_next()`, which
 * can be used to check if `#include_next` exists:
 * ```c
 * #if __has_include_next(<stdio.h>)
 * #include_next <stdio.h>
 * #endif
 * ```
 *
 * @detect: #ifdef __has_include_next */
#ifndef TPP_HAVE_MACRO___has_include_next
#define TPP_HAVE_MACRO___has_include_next ((TPP_HAVE_CPP_INCLUDE_NEXT && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_include_next" */
#endif /* !TPP_HAVE_MACRO___has_include_next */

/* Support for clang `__has_embed()`, which can be
 * used to check if `#embed` exists and/or is empty:
 * ```c
 * #if __has_embed("resource.dat" limit(10)) == __STDC_EMBED_FOUND__
 * #embed "resource.dat" limit(10) // Resource exists and is non-empty
 * #endif
 * ```
 *
 * The different values that `__has_embed` can expand to are configured by:
 * - `__STDC_EMBED_NOT_FOUND__` (usually `0`; `TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND`)
 * - `__STDC_EMBED_FOUND__` (usually `1`; `TPP_CONFIG_VALUEOF_STDC_EMBED_FOUND`)
 * - `__STDC_EMBED_EMPTY__` (usually `2`; `TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY`)
 *
 * @detect: #ifdef __has_embed */
#ifndef TPP_HAVE_MACRO___has_embed
#define TPP_HAVE_MACRO___has_embed ((TPP_HAVE_CPP_EMBED && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_embed" */
#endif /* !TPP_HAVE_MACRO___has_embed */

/* Support for the builtin macro `__FILE__`,
 * which expands to the current file's name
 *
 * @detect: #ifdef __FILE__ */
#ifndef TPP_HAVE_MACRO___FILE__
#define TPP_HAVE_MACRO___FILE__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__FILE__" */
#endif /* !TPP_HAVE_MACRO___FILE__ */

/* Support for the builtin macro `__LINE__`,
 * which expands to the current line number
 *
 * @detect: #ifdef __LINE__ */
#ifndef TPP_HAVE_MACRO___LINE__
#define TPP_HAVE_MACRO___LINE__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__LINE__" */
#endif /* !TPP_HAVE_MACRO___LINE__ */

/* Support for the builtin macro `__TIME__`,
 * which expands to something like `"12:37:58"`
 *
 * @detect: #ifdef __TIME__ */
#ifndef TPP_HAVE_MACRO___TIME__
#define TPP_HAVE_MACRO___TIME__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__TIME__" */
#endif /* !TPP_HAVE_MACRO___TIME__ */

/* Support for the builtin macro `__DATE__`,
 * which expands to something like `"Jul  6 2026"`
 *
 * @detect: #ifdef __DATE__ */
#ifndef TPP_HAVE_MACRO___DATE__
#define TPP_HAVE_MACRO___DATE__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__DATE__" */
#endif /* !TPP_HAVE_MACRO___DATE__ */

/* Support for the builtin macro `__COLUMN__`,
 * which expands to current column number
 *
 * @detect: #ifdef __COLUMN__ */
#ifndef TPP_HAVE_MACRO___COLUMN__
#define TPP_HAVE_MACRO___COLUMN__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fcolumn-macro" */
#endif /* !TPP_HAVE_MACRO___COLUMN__ */

/* Support for the builtin macro `__BASE_FILE__`,
 * which expands to base file's name
 *
 * @detect: #ifdef __BASE_FILE__ */
#ifndef TPP_HAVE_MACRO___BASE_FILE__
#define TPP_HAVE_MACRO___BASE_FILE__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fbasefile-macro" */
#endif /* !TPP_HAVE_MACRO___BASE_FILE__ */

/* Support for the builtin macro `__FILE_NAME__`.
 * Similar to `__FILE__` (see `TPP_HAVE_MACRO___FILE__`), but expand to the
 * current file's name, without its leading path components (i.e. its *basename*).
 *
 * @detect: #ifdef __FILE_NAME__ */
#ifndef TPP_HAVE_MACRO___FILE_NAME__
#define TPP_HAVE_MACRO___FILE_NAME__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__FILE_NAME__" */
#endif /* !TPP_HAVE_MACRO___FILE_NAME__ */

/* Support for the builtin macro `__INCLUDE_LEVEL__`,
 * which expands to numerical representation of include depth.
 * Whilst inside the "base"-file, it expands to `0`
 *
 * @detect: #ifdef __INCLUDE_LEVEL__ */
#ifndef TPP_HAVE_MACRO___INCLUDE_LEVEL__
#define TPP_HAVE_MACRO___INCLUDE_LEVEL__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-finclude-level-macro" */
#endif /* !TPP_HAVE_MACRO___INCLUDE_LEVEL__ */

/* Support for the builtin macro `__INCLUDE_DEPTH__`, which is an
 * alias for `__INCLUDE_LEVEL__` (see `TPP_HAVE_MACRO___INCLUDE_LEVEL__`)
 *
 * @detect: #ifdef __INCLUDE_DEPTH__ */
#ifndef TPP_HAVE_MACRO___INCLUDE_DEPTH__
#define TPP_HAVE_MACRO___INCLUDE_DEPTH__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-finclude-depth-macro" */
#endif /* !TPP_HAVE_MACRO___INCLUDE_DEPTH__ */

/* Support for the builtin macro `__COUNTER__`, which
 * expands to`1+` its previous expansion. The first time
 * this macro is used, it expands to `0`.
 *
 * @detect: #ifdef __COUNTER__ */
#ifndef TPP_HAVE_MACRO___COUNTER__
#define TPP_HAVE_MACRO___COUNTER__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fcounter-macro" */
#endif /* !TPP_HAVE_MACRO___COUNTER__ */

/* Support for the builtin macro `__TIMESTAMP__`, which
 * expands to something like `"Mon Jul  6 12:37:58 2026"`
 *
 * @detect: #ifdef __TIMESTAMP__ */
#ifndef TPP_HAVE_MACRO___TIMESTAMP__
#define TPP_HAVE_MACRO___TIMESTAMP__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftimestamp-macro" */
#endif /* !TPP_HAVE_MACRO___TIMESTAMP__ */

/* Support for the builtin macros `__DATE_DAY__`, `__DATE_WDAY__`, `__DATE_YDAY__`, `__DATE_MONTH__`, `__DATE_YEAR__`.
 * These are precise date/time macros and behave like `__LINE__`, but expand to the components of `__DATE__`
 *
 * @detect: #ifdef __DATE_DAY__, ... */
#ifndef TPP_HAVE_NUMERIC_DATE_MACROS
#define TPP_HAVE_NUMERIC_DATE_MACROS (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fnumeric-date-macros" */
#endif /* !TPP_HAVE_NUMERIC_DATE_MACROS */

/* Support for the builtin macros `__TIME_SEC__`, `__TIME_MIN__`, `__TIME_HOUR__`.
 * These are precise date/time macros and behave like `__LINE__`, but expand to components of `__TIME__`
 *
 * @detect: #ifdef __TIME_SEC__, ... */
#ifndef TPP_HAVE_NUMERIC_TIME_MACROS
#define TPP_HAVE_NUMERIC_TIME_MACROS (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fnumeric-time-macros" */
#endif /* !TPP_HAVE_NUMERIC_TIME_MACROS */

/* Support for the builtin function-like macro `__TPP_EVAL()`, which can
 * be used to evaluate an expression like in `#if` and replace the macro
 * invocation with the expression's result in the form of 1-2 tokens:
 * - `[<int>]`
 * - `[-][<int>]`
 * - `[<string>]` (if `TPP_HAVE_BUILTIN_EXPR_STRINGS` is enabled)
 *
 * Example:
 * ```c
 * __TPP_EVAL(10 + 20) // Expands to a single token: 30
 * ```
 *
 * @detect: #ifdef __TPP_EVAL */
#ifndef TPP_HAVE_MACRO___TPP_EVAL
#define TPP_HAVE_MACRO___TPP_EVAL (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-eval-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */

/* Support for the builtin function-like macro `__TPP_EXEC()`.
 *
 * Takes a string that is then re-interpreted as preprocessor input,
 * and expands to whatever that string expands to. Note that the string
 * may contain any kind of directive:
 * ```c
 * #define RESOURCE_SIZE2(...) __VA_NARGS__
 * #define RESOURCE_SIZE1(x)   RESOURCE_SIZE2(x)
 * #define RESOURCE_SIZE(name) RESOURCE_SIZE1(__TPP_EXEC("#embed " #name))
 * RESOURCE_SIZE("resource.dat") // Expands to file size in bytes
 * ```
 *
 * NOTE: If you just want to execute code but discard whatever it expands
 *       to, use `#pragma tpp_exec()` (see `TPP_HAVE_PRAGMA_TPP_EXEC`)
 *       instead, which has a lower overhead and lets you safely execute
 *       the code whilst discarding its result.
 *
 * @detect: #ifdef __TPP_EXEC */
#ifndef TPP_HAVE_MACRO___TPP_EXEC
#define TPP_HAVE_MACRO___TPP_EXEC (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-exec-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_EXEC */

/* Support for the builtin function-like macro `__TPP_UNIQUE()`.
 *
 * When called with some keyword/identifier, this macro expands a unique INT-token
 * representative of the given keyword/identifier. The value of that INT-token
 * is distinct from any other keyword/identifier and remains the same for the
 * remaining of input (though it will differ if re-run with different input;
 * this is not a hash-function)
 *
 * @detect: #ifdef __TPP_UNIQUE */
#ifndef TPP_HAVE_MACRO___TPP_UNIQUE
#define TPP_HAVE_MACRO___TPP_UNIQUE (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-unique-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_UNIQUE */

/* Support for the builtin function-like macro `__TPP_LOAD_FILE()`.
 *
 * This macro behaves similar to `#include`, but the contents of the included
 * file are not macro-expanded, but instead packaged into a string literal to
 * which this macro then expands.
 *
 * Using some other features/extensions, this macro can be implemented as:
 * ```c
 * #define __TPP_LOAD_FILE(filename) __TPP_STR_PACK(__TPP_EXEC("#embed " #filename))
 * ```
 *
 * @detect: #ifdef __TPP_LOAD_FILE */
#ifndef TPP_HAVE_MACRO___TPP_LOAD_FILE
#define TPP_HAVE_MACRO___TPP_LOAD_FILE (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-load-file-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_LOAD_FILE */

/* Support for the builtin function-like macro `__TPP_COUNTER()`.
 *
 * Called the same way as `__TPP_UNIQUE` (see `TPP_HAVE_MACRO___TPP_UNIQUE`),
 * but returns an ever-increasing value starting at `0` (same as `__COUNTER__`),
 * but that counter is specific to the given keyword. i.e.: `__TPP_COUNTER(foo)`
 * and `__TPP_COUNTER(bar)` increment different counters
 *
 * @detect: #ifdef __TPP_COUNTER */
#ifndef TPP_HAVE_MACRO___TPP_COUNTER
#define TPP_HAVE_MACRO___TPP_COUNTER (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-counter-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */

/* Support for the builtin function-like macro `__TPP_RANDOM()`.
 *
 * Overloaded macro taking 1 or 2 arguments, and expanding to a random number
 * that stays the same during repeated compilations, only changing if source
 * code is altered:
 * - The single-argument form `__TPP_RANDOM(hi)` expands to a pseudo-random
 *   integer token in the range `[0,hi)`
 * - The 2-argument form `__TPP_RANDOM(lo, hi)` expands to a pseudo-random
 *   integer token in the range `[lo,hi)`
 *
 * @detect: #ifdef __TPP_RANDOM */
#ifndef TPP_HAVE_MACRO___TPP_RANDOM
#define TPP_HAVE_MACRO___TPP_RANDOM (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-random-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_RANDOM */

/* Support for the builtin function-like macro `__TPP_STR_DECOMPILE()`.
 *
 * This macro is very similar to `__TPP_EXEC()` (see `TPP_HAVE_MACRO___TPP_EXEC`),
 * except that while `__TPP_EXEC()` will expand other macros and directives,
 * `__TPP_STR_DECOMPILE()` doesn't: it simply takes a string and expands to
 * its decoded form *without* expansion (however: expansion may still occur as
 * returned tokens are yielded, so this difference may not always be obvious).
 *
 * In practice this usually only means:
 * - `__TPP_EXEC()` accepts preprocessor directives
 * - `__TPP_STR_DECOMPILE()` only does basic string-to-token conversion
 *
 * @detect: #ifdef __TPP_STR_DECOMPILE */
#ifndef TPP_HAVE_MACRO___TPP_STR_DECOMPILE
#define TPP_HAVE_MACRO___TPP_STR_DECOMPILE (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-str-decompile-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_DECOMPILE */

/* Support for the builtin function-like macro `__TPP_STR_PACK()`.
 *
 * Expands to a string literal that is made up of the arguments taken by
 * this macro. There can be any number of arguments (and arguments don't
 * even have to be separated by `,`), but every argument must be:
 * - Another string (that is added to the result after being decoded and re-encoded)
 * - An INT token (that must evaluate to a value in range `[0,0xFF]`)
 *
 * @detect: #ifdef __TPP_STR_PACK */
#ifndef TPP_HAVE_MACRO___TPP_STR_PACK
#define TPP_HAVE_MACRO___TPP_STR_PACK (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-str-pack-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_PACK */

/* Support for the builtin function-like macro `__TPP_STR_SUBSTR()` and `__TPP_STR_AT()`.
 *
 * Stand-alone macro that takes 3 arguments and (assuming `TPP_HAVE_BUILTIN_EXPR_STRINGS`
 * and `TPP_HAVE_MACRO___TPP_EVAL` are enabled) can be implemented as follows:
 * ```c
 * #define __TPP_STR_SUBSTR_3(str, i, n) __TPP_EVAL((str)[(i):(i)+(n)])
 * #define __TPP_STR_SUBSTR_2(str, i)    __TPP_EVAL((str)[(i):(i)+1])
 * #define __TPP_STR_SUBSTR(...)         __TPP_STR_SUBSTR_##__VA_NARGS__(__VA_ARGS__)
 * ```
 *
 * The macro `__TPP_STR_AT()` behaves the same, but quotes its result using `'` instead of `"`
 *
 * @detect: #if defined(__TPP_STR_SUBSTR) || defined(__TPP_STR_AT) */
#ifndef TPP_HAVE_MACRO___TPP_STR_SUBSTR
#define TPP_HAVE_MACRO___TPP_STR_SUBSTR (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-str-substr-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SUBSTR */

/* Support for the builtin function-like macro `__TPP_STR_SIZE()`.
 *
 * Stand-alone macro that takes 1 arguments and (assuming `TPP_HAVE_BUILTIN_EXPR_STRINGS`
 * and `TPP_HAVE_MACRO___TPP_EVAL` are enabled) can be implemented as follows:
 * ```c
 * #define __TPP_STR_SIZE(str) __TPP_EVAL(#(str))
 * ```
 *
 * @detect: #ifdef __TPP_STR_SIZE */
#ifndef TPP_HAVE_MACRO___TPP_STR_SIZE
#define TPP_HAVE_MACRO___TPP_STR_SIZE (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-str-size-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SIZE */

/* Support for the builtin function-like macro `__TPP_COUNT_TOKENS()`.
 *
 * A token-feature probing macro that lets you count the number of
 * tokens that are contained within a given string. For this
 * purpose, no macro expansion or directive processing is done:
 * ```c
 * __TPP_COUNT_TOKENS("++")         // 1 if "TPP_HAVE_TPP_TOK_PLUS_PLUS" is enabled, else 2
 * #define FOO 10 20 30
 * __TPP_COUNT_TOKENS("FOO")        // 1 because macros aren't expanded
 * __TPP_COUNT_TOKENS("#undef FOO") // 3 (or 4 if TPP_HAVE_TPP_TOK_SPACE) because directives aren't parsed
 * ```
 *
 * Based on the numbers returned by this macro, it becomes possible
 * to detect the state of pretty much all configuration options that
 * affect the behavior of `tpp_lexer_yieldraw()`
 *
 * @detect: #ifdef __TPP_COUNT_TOKENS */
#ifndef TPP_HAVE_MACRO___TPP_COUNT_TOKENS
#define TPP_HAVE_MACRO___TPP_COUNT_TOKENS (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-count-tokens-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_COUNT_TOKENS */

/* Support for the builtin function-like macro `__TPP_IDENTIFIER()`.
 *
 * This builtin macro can be used to construct arbitrary keyword-like
 * tokens, including ones that aren't actually keywords. This macro
 * takes a single argument (in the form of a string), and always expands
 * to a single keyword/identifier-type token:
 * ```c
 * __TPP_IDENTIFIER("foo")   // Same "foo"
 * __TPP_IDENTIFIER("1foo")  // Still a keyword; namely: "1foo" -- there's no way
 *                           // to write this, since it'd normally be a TPP_TOK_INT
 * __TPP_IDENTIFIER("a\0b")  // Compilers probably won't like this: NUL-character in
 *                           // keyword name, meaning strlen() returns "1", but keyword
 *                           // is legally distinct from "a" (though lots of places
 *                           // will probably print the keyword as "a")
 * ```
 *
 * Similar functionality can also be achieved using `TPP_HAVE_ESCAPE_IN_IDENTIFIERS`
 *
 * Note that unlike `TPP_HAVE_ESCAPE_IN_IDENTIFIERS`, `__TPP_IDENTIFIER`
 * can only be used in places where macros are expanded:
 * ```c
 * #__TPP_IDENTIFIER("define") foo 42  // Won't work
 * #\u0064efine foo 42                 // This will work
 * ```
 *
 * @detect: #ifdef __TPP_IDENTIFIER */
#ifndef TPP_HAVE_MACRO___TPP_IDENTIFIER
#define TPP_HAVE_MACRO___TPP_IDENTIFIER (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-identifier-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_IDENTIFIER */


/* Support for alternative parenthesis pairs in user-defined macros:
 * ```c
 * #define normal_macro(x) you_should("know this", x)
 * #define array[index]    get_ident(ARRAY, index)
 * #define block{...}      do { __VA_ARGS__ } while (should_continue())
 * #define point<T>        struct { T x; T y; }
 *
 * normal_macro(foo) // you_should("know this", foo)
 * array[42]         // get_ident(ARRAY, 42)
 * block { body }    // do { body } while (should_continue())
 * point<int>        // struct { int x; int y; }
 * ```
 *
 * These alternate parenthesis pairs work in a hierarchy, where
 * higher-order parenthesis pairs respect unmatched parentheses
 * of lower orders, but (to not break standard compatibility),
 * not the opposite. iow: `point<foo(x > 10)>` works
 * as expected (the `T` parameter is `foo(x > 10)` rather
 * than `foo(x `), but `(normal_macro(foo < 10) > 20)` does not
 * look out of `< >`-paris (the `x` parameter is `foo < 10`
 * rather than `foo < 10) > 20`). See the following hierarchy:
 *
 * | order | l-paren | r-paren |
 * | ----- | ------- | ------- |
 * | 1     | `(`     | `)`     |
 * | 2     | `[`     | `]`     |
 * | 3     | `{`     | `}`     |
 * | 4     | `<`     | `>`     |
 *
 * Some more examples of the results produced by this hirarchy:
 *
 * ```c
 * point<{ int x = v > 20; }>   // T is `{ int x = v > 20; }`
 * point<arr[y > 10 ? 0 : 2]>   // T is `arr[y > 10 ? 0 : 2]`
 * point<foo(x > 10)>           // T is `foo(x > 10)`
 * point<point<int>>            // T is `point<int>`
 * point<x > 10>                // !!! T is `x` and expansion is followed by ` 10>`
 *
 * block { int y = < 20; } > }; // __VA_ARGS__ is `int y = v < 20;` and expansion is followed by ` > };`
 * block { int y = { 20; } };   // __VA_ARGS__ is `int y = { 20; }`
 * block { int y = [ 20; } ] }; // __VA_ARGS__ is `int y = [20; } ]`
 * block { int y = ( 20; } ) }; // __VA_ARGS__ is `int y = (20; } )`
 *
 * array[y < 20] > ]; // index is `y < 20` and expansion is followed by ` > ];`
 * array[y { 20] } ]; // index is `y < 20` and expansion is followed by ` } ];`
 * array[y [ 20] ];   // index is `y [ 20]`
 * array[y ( 20] ) ]; // index is `y ( 20] )`
 *
 * normal_macro(y < 20) > ); // x is `y < 20` and expansion is followed by ` > );`
 * normal_macro(y { 20) } ); // x is `y { 20` and expansion is followed by ` } );`
 * normal_macro(y [ 20) ] ); // x is `y [ 20` and expansion is followed by ` ] );`
 * normal_macro(y ( 20) );   // x is `y ( 20)`
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*:
 * ```c
 * #pragma extension("-falternative-macro-parenthesis")
 * #define point1<T> struct { T x; T y; }
 * #pragma extension("-fno-alternative-macro-parenthesis")
 * #define point2<T> struct { T x; T y; }
 *
 * point1<int>  // struct { int x; int y; }
 * point2<int>  // <T> struct { T x; T y; }<int>
 * ```
 *
 * @detect: #define foo<bar> +1
 *          #define bar 10
 *          #if (1 foo<1> +1) == 3
 */
#ifndef TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS (TPP_HAVE_CPP_MACROS ? TPP_CONF_EXT1 : 0) /* "-falternative-macro-parenthesis" */
#endif /* !TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

/* Retain whitespace around macro arguments:
 * ```c
 * #define STR1(x) #x
 * #pragma extension(push, "-fmacro-argument-whitespace")
 * #define STR2(x) #x
 * #pragma extension(pop)
 * STR1(  foo  ) // "foo"
 * STR2(  foo  ) // "  foo  "
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION* */
#ifndef TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
#define TPP_HAVE_MACRO_ARGUMENT_WHITESPACE ((TPP_HAVE_CPP_MACROS && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT0 : 0) /* "-fmacro-argument-whitespace" */
#endif /* !TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */

/* Allow self-recursion for certain macros:
 * ```c
 * #define REPEAT_0(n, x)
 * #pragma extension(push, "-fmacro-recursion")
 * #define REPEAT_1(n, x) #!x REPEAT(n, #!x)
 * #define REPEAT__(v, n, x) REPEAT_##v(n, #!x)
 * #define REPEAT_(v, n, x) REPEAT__(v, n, #!x)
 * #define REPEAT(n, x) REPEAT_(__TPP_EVAL((n) > 0), __TPP_EVAL((n)-1), #!x)
 * #pragma extension(pop)
 * REPEAT(42, HELLO_WORLD) // HELLO_WORLD HELLO_WORLD HELLO_WORLD [...] (42 times)
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION* */
#ifndef TPP_HAVE_MACRO_RECURSION
#define TPP_HAVE_MACRO_RECURSION ((TPP_HAVE_CPP_MACROS && TPP_HAVE_PROFILE_NOT_MINIMAL) ? TPP_CONF_EXT0 : 0) /* "-fmacro-recursion" */
#endif /* !TPP_HAVE_MACRO_RECURSION */

// Support for traditional macro expansion rules:
// ```c
// #define CAT(a, b) a##b
// #define STR(x)    #x
// #pragma extension(push, "-ftraditional-macro")
// #define T_CAT(a, b) a/**/b
// #define T_STR(x)    "x"
// #pragma extension(pop)
// CAT(10, 20)   // 1020
// STR(10)       // "10"
// T_CAT(10, 20) // 1020
// T_STR(10)     // "10"
// ```
//
// NOTE: affects behavior of macros at the *TIME OF DEFINITION* ")
#ifndef TPP_HAVE_TRADITIONAL_MACROS
#define TPP_HAVE_TRADITIONAL_MACROS ((TPP_HAVE_CPP_MACROS && TPP_HAVE_PROFILE_NOT_MINIMAL) ? TPP_CONF_EXT0 : 0) /* "-ftraditional-macro" */
#endif /* !TPP_HAVE_TRADITIONAL_MACROS */

/* Support for variable-argument macros with named varargs:
 * ```c
 * #define printf(format, args...) fprintf(stdout, format, args)
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION* */
#ifndef TPP_HAVE_NAMED_VARARGS_IN_MACROS
#define TPP_HAVE_NAMED_VARARGS_IN_MACROS (TPP_HAVE_CPP_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fnamed-varargs-in-macros" */
#endif /* !TPP_HAVE_NAMED_VARARGS_IN_MACROS */

/* Support for variable-argument macros:
 * ```c
 * #define printf(...) fprintf(stdout, __VA_ARGS__)
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION* */
#ifndef TPP_HAVE_VA_ARGS_IN_MACROS
#define TPP_HAVE_VA_ARGS_IN_MACROS (TPP_HAVE_CPP_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fva-args-in-macros" */
#endif /* !TPP_HAVE_VA_ARGS_IN_MACROS */

/* Support for special keyword `__VA_COMMA__` inside of variable-argument macros.
 * When varargs are non-empty, replaced with `,`. Otherwise, replaced with nothing:
 * ```c
 * #define printf(format, ...) fprintf(stdout, format __VA_COMMA__ __VA_ARGS__)
 * printf("foo\n");         // fprintf(stdout, "foo\n");
 * printf("i = %d\n", 10);  // fprintf(stdout, "i = %d\n", 10);
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*
 * @detect: #define test1(a, b, ...) __VA_ARGS__+0
 *          #define test2(...) test1(__VA_COMMA__ 0, 1)
 *          #if test2(~) */
#ifndef TPP_HAVE_VA_COMMA_IN_MACROS
#define TPP_HAVE_VA_COMMA_IN_MACROS ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fva-comma-in-macros" */
#endif /* !TPP_HAVE_VA_COMMA_IN_MACROS */

/* Support for special function `__VA_OPT__()` inside of variable-argument macros.
 * When varargs are non-empty, the content within parenthesis is kept. Otherwise,
 * said content is removed alongside the `__VA_OPT__()` call itself:
 * ```c
 * #define printf(format, ...) fprintf(stdout, format __VA_OPT__(,) __VA_ARGS__)
 * printf("foo\n");         // fprintf(stdout, "foo\n");
 * printf("i = %d\n", 10);  // fprintf(stdout, "i = %d\n", 10);
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*
 * @detect: #define test1(a, b, ...) __VA_ARGS__+0
 *          #define test2(...) test1(__VA_OPT__(,) 0, 1)
 *          #if test2(~) */
#ifndef TPP_HAVE_VA_OPT_IN_MACROS
#define TPP_HAVE_VA_OPT_IN_MACROS ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fva-opt-in-macros" */
#endif /* !TPP_HAVE_VA_OPT_IN_MACROS */

/* Support for special keyword `__VA_NARGS__` inside of variable-argument macros.
 * Expands to the number of varargs, allowing for easy overloading of macros based
 * on the number of supplied arguments:
 * ```c
 * #define min_0()        0
 * #define min_1(a)       a
 * #define min_2(a, b)    ((a) < (b) ? (a) : (b))
 * #define min_3(a, b, c) min_2(min_2(a, b), c)
 * #define min(...)       min_##__VA_NARGS__(__VA_ARGS__)
 * min()        // Expands to: "0"
 * min(10)      // Expands to: "10"
 * min(10, 20)  // Expands to: "((10) < (20) ? (10) : (20))"
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*
 * @detect: #define test___VA_NARGS__ 0
 *          #define test_1            1
 *          #define test(...) test_##__VA_NARGS__
 *          #if test2(~) */
#ifndef TPP_HAVE_VA_NARGS_IN_MACROS
#define TPP_HAVE_VA_NARGS_IN_MACROS ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fva-nargs-in-macros" */
#endif /* !TPP_HAVE_VA_NARGS_IN_MACROS */

/* Support for GCC-style va-comma: when a variable-argument macro
 * contains a construct like `, ## __VA_ARGS__` (or the name of the
 * variable-length argument when `TPP_HAVE_NAMED_VARARGS_IN_MACROS`
 * is enabled), then the `,` is deleted during expansion whenever
 * the there are no variable arguments:
 *
 * ```c
 * #define printf(format, ...) fprintf(stdout, format, ## __VA_ARGS__)
 * printf("foo\n");         // fprintf(stdout, "foo\n");
 * printf("i = %d\n", 10);  // fprintf(stdout, "i = %d\n", 10);
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*
 * @detect: #define test1(a, b, ...) __VA_ARGS__+0
 *          #define test2(...) test1(,##__VA_ARGS__, 1)
 *          #if test2() == 0 */
#ifndef TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
#define TPP_HAVE_VA_GLUE_COMMA_IN_MACROS ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fglue-comma-in-macros" */
#endif /* !TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */

/* Support for the so-called "stringize" operator in macros:
 * ```c
 * #define str(x) #x
 * str(foo)    // Expands to: "foo"
 * str("foo")  // Expands to: "\"foo\""
 * ```
 *
 * Note that this operator only takes effect during modern macro
 * compilation. When `TPP_HAVE_TRADITIONAL_MACROS` is enabled,
 * then stringization works differently and must be done like so:
 * ```c
 * #define str(x) "x"
 * str(foo)    // Expands to: "foo"
 * str("foo")  // Expands to: ""foo"" -- oops; traditional macros can't do this
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*
 * @detect: #define str(x) #x
 *          #if __TPP_COUNT_TOKENS(str(a b)) == 1 */
#ifndef TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
#define TPP_HAVE_STRINGIZE_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fstringize-macro-argument" */
#endif /* !TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */

/* Support for the so-called "charize" operator in macros:
 * ```c
 * #define str(x) #@x
 * str(foo)    // Expands to: 'foo'
 * str('foo')  // Expands to: '\'foo\''
 * ```
 *
 * Note that this operator only takes effect during modern macro
 * compilation. When `TPP_HAVE_TRADITIONAL_MACROS` is enabled,
 * then charization works differently and must be done like so:
 * ```c
 * #define chr(x) 'x'
 * chr(foo)    // Expands to: 'foo'
 * chr('foo')  // Expands to: ''foo'' -- oops; traditional macros can't do this
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*
 * @detect: #define str(x) #@x
 *          #if __TPP_COUNT_TOKENS(str(a b)) == 1 */
#ifndef TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#define TPP_HAVE_CHARIZE_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fcharize-macro-argument" */
#endif /* !TPP_HAVE_CHARIZE_MACRO_ARGUMENT */

/* Support for `#define noexpand(x) #!x`, which allows
 * you to insert macro arguments without expanding them:
 * ```c
 * #define STR1(x) #x
 * #define STR2(x) STR1(x)
 * #define STR3(x) STR1(#!x)
 * #define FOO 42
 * STR1(FOO) // "FOO"
 * STR2(FOO) // "42"
 * STR3(FOO) // "FOO"
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*
 * @detect: #define test1(x) #x
 *          #define test2(x) test1(#!x)
 *          #define test3    42
 *          #if __TPP_STR_SIZE(__TPP_COUNT_TOKENS(test3)) == 5 */
#ifndef TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
#define TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fdont-expand-macro-argument" */
#endif /* !TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT */

/* Support for `##` in macros to concatenate different tokens
 * in order to form a new token:
 * ```c
 * #define cat(a, b) a##b
 * cat(+, +)  // Expands to a single token "++" (assuming that TPP_HAVE_TPP_TOK_PLUS_PLUS is enabled)
 * ```
 *
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*
 * @detect: #define test(a, b) a##b
 *          #define str2(x) #x
 *          #define str(x) str2(x)
 *          #if __TPP_COUNT_TOKENS(str(test(10, 20))) == 1 */
#ifndef TPP_HAVE_GLUE_MACRO_ARGUMENT
#define TPP_HAVE_GLUE_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fglue-macro-argument" */
#endif /* !TPP_HAVE_GLUE_MACRO_ARGUMENT */

/************************************************************************/
/************************************************************************/
/************************************************************************/




/************************************************************************/
/* #PRAGMA DIRECTIVES                                                   */
/************************************************************************/

/* Support for: `#pragma push_macro()` / `#pragma pop_macro()`
 * @detect: #define TEST 42
 *          #pragma push_macro("TEST")
 *          #undef TEST
 *          #pragma pop_macro("TEST")
 *          #ifdef TEST */
#ifndef TPP_HAVE_PRAGMA_PUSH_MACRO
#define TPP_HAVE_PRAGMA_PUSH_MACRO ((TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-push-macro" */
#endif /* !TPP_HAVE_PRAGMA_PUSH_MACRO */

/* Support for: `#pragma once` */
#ifndef TPP_HAVE_PRAGMA_ONCE
#define TPP_HAVE_PRAGMA_ONCE (((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT) && TPP_HAVE_PRAGMA) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-once" */
#endif /* !TPP_HAVE_PRAGMA_ONCE */

/* Support for: `#pragma deprecated("foo")` */
#ifndef TPP_HAVE_PRAGMA_DEPRECATED
#define TPP_HAVE_PRAGMA_DEPRECATED (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-deprecated" */
#endif /* !TPP_HAVE_PRAGMA_DEPRECATED */

/* Support for: `#pragma extension(...)` */
#ifndef TPP_HAVE_PRAGMA_EXTENSION
#define TPP_HAVE_PRAGMA_EXTENSION ((TPP_HAVE_PRAGMA && TPP_HAVE_EXTENSIONS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-extension" */
#endif /* !TPP_HAVE_PRAGMA_EXTENSION */

/* Support for: `#pragma warning(...)` */
#ifndef TPP_HAVE_PRAGMA_WARNING
#define TPP_HAVE_PRAGMA_WARNING ((TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-warning" */
#endif /* !TPP_HAVE_PRAGMA_WARNING */

/* Support for: `#pragma message("...")` */
#ifndef TPP_HAVE_PRAGMA_MESSAGE
#define TPP_HAVE_PRAGMA_MESSAGE (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-message" */
#endif /* !TPP_HAVE_PRAGMA_MESSAGE */

/* Support for: `#pragma error("...")` */
#ifndef TPP_HAVE_PRAGMA_ERROR
#define TPP_HAVE_PRAGMA_ERROR (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-error" */
#endif /* !TPP_HAVE_PRAGMA_ERROR */

/* Support for: `#pragma region` + `#pragma endregion` */
#ifndef TPP_HAVE_PRAGMA_REGION
#define TPP_HAVE_PRAGMA_REGION (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-region" */
#endif /* !TPP_HAVE_PRAGMA_REGION */

/* Support for: `#pragma tpp_exec("...")` */
#ifndef TPP_HAVE_PRAGMA_TPP_EXEC
#define TPP_HAVE_PRAGMA_TPP_EXEC (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-tpp-exec" */
#endif /* !TPP_HAVE_PRAGMA_TPP_EXEC */

/* Support for: `#pragma tpp_set_keyword_flags("foo", 0x7f)` */
#ifndef TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-tpp-set-keyword-flags" */
#endif /* !TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

/* Support for: `#pragma GCC poison foo` */
#ifndef TPP_HAVE_PRAGMA_GCC_POISON
#define TPP_HAVE_PRAGMA_GCC_POISON (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-gcc-poison" */
#endif /* !TPP_HAVE_PRAGMA_GCC_POISON */

/* Support for: `#pragma GCC warning "message"` */
#ifndef TPP_HAVE_PRAGMA_GCC_WARNING
#define TPP_HAVE_PRAGMA_GCC_WARNING ((TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-gcc-warning" */
#endif /* !TPP_HAVE_PRAGMA_GCC_WARNING */

/* Support for: `#pragma GCC error "message"` */
#ifndef TPP_HAVE_PRAGMA_GCC_ERROR
#define TPP_HAVE_PRAGMA_GCC_ERROR ((TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-gcc-error" */
#endif /* !TPP_HAVE_PRAGMA_GCC_ERROR */

/* Support for: `#pragma GCC system_header` */
#ifndef TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#define TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-gcc-system-header" */
#endif /* !TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

/* Support for: `#pragma GCC diagnostic` */
#ifndef TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC
#define TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC ((TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-gcc-diagnostic" */
#endif /* !TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */

/* Support for: `#pragma GCC dependency` */
#ifndef TPP_HAVE_PRAGMA_GCC_DEPENDENCY
#define TPP_HAVE_PRAGMA_GCC_DEPENDENCY (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-gcc-dependency" */
#endif /* !TPP_HAVE_PRAGMA_GCC_DEPENDENCY */

/* Support for: `#pragma TPP warning(...)`  (same as `TPP_HAVE_PRAGMA_WARNING`, but doesn't require `"-fpragma-warning"`) */
#ifndef TPP_HAVE_PRAGMA_TPP_WARNING
#define TPP_HAVE_PRAGMA_TPP_WARNING TPP_HAVE_PRAGMA_WARNING
#endif /* !TPP_HAVE_PRAGMA_TPP_WARNING */

/* Support for: `#pragma TPP extension(...)`  (same as `TPP_HAVE_PRAGMA_EXTENSION`, but doesn't require `"-fpragma-extension"`) */
#ifndef TPP_HAVE_PRAGMA_TPP_EXTENSION
#define TPP_HAVE_PRAGMA_TPP_EXTENSION TPP_HAVE_PRAGMA_EXTENSION
#endif /* !TPP_HAVE_PRAGMA_TPP_EXTENSION */

/* Support for: `#pragma TPP tpp_exec(...)`  (same as `TPP_HAVE_PRAGMA_TPP_EXEC`, but doesn't require `"-fpragma-tpp-exec"`) */
#ifndef TPP_HAVE_PRAGMA_TPP_TPP_EXEC
#define TPP_HAVE_PRAGMA_TPP_TPP_EXEC TPP_HAVE_PRAGMA_TPP_EXEC
#endif /* !TPP_HAVE_PRAGMA_TPP_TPP_EXEC */

/* Support for: `#pragma TPP tpp_set_keyword_flags("foo", 0x7f)`  (same as `TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS`, but doesn't require `"-fpragma-tpp-set-keyword-flags"`) */
#ifndef TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
#define TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#endif /* !TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */

/* Support for: `#pragma TPP include_path(...)` */
#ifndef TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
#define TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH (TPP_HAVE_PRAGMA && TPP_HAVE_INCLUDE_PATH)
#endif /* !TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */

#undef TPP_HAVE_PRAGMA_GCC
#if (TPP_HAVE_PRAGMA_GCC_POISON ||        \
     TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER || \
     TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC ||    \
     TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
#define TPP_HAVE_PRAGMA_GCC 1
#else /* ... */
#define TPP_HAVE_PRAGMA_GCC 0
#endif /* !... */
#undef TPP_HAVE_PRAGMA_TPP
#if (TPP_HAVE_PRAGMA_TPP_WARNING ||               \
     TPP_HAVE_PRAGMA_TPP_EXTENSION ||             \
     TPP_HAVE_PRAGMA_TPP_TPP_EXEC ||              \
     TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS || \
     TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH)
#define TPP_HAVE_PRAGMA_TPP 1
#else /* ... */
#define TPP_HAVE_PRAGMA_TPP 0
#endif /* !... */

/************************************************************************/
/************************************************************************/
/************************************************************************/




/************************************************************************/
/* TOKENS                                                               */
/************************************************************************/

/* Support for trigraph character aliases:
 *
 * | Trigraph | Replacement |
 * | -------- | ----------- |
 * | `??=`    | `#`         |
 * | `??(`    | `[`         |
 * | `??/`    | `\`         |
 * | `??)`    | `]`         |
 * | `??'`    | `^`         |
 * | `??<`    | `{`         |
 * | `??!`    | `\|`        |
 * | `??>`    | `}`         |
 * | `??-`    | `~`         |
 * | `???`    | `?`         | */
#ifndef TPP_HAVE_TRIGRAPHS
#define TPP_HAVE_TRIGRAPHS (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) /* "-ftrigraphs" */
#endif /* !TPP_HAVE_TRIGRAPHS */

/* Support for digraph token aliases:
 * | Digraph | Replacement |
 * | ------- | ----------- |
 * | `<%`    | `{`         |
 * | `<:`    | `[`         |
 * | `%>`    | `}`         |
 * | `:>`    | `]`         |
 * | `%:`    | `#`         |
 * | `%:%:`  | `##` (requires `TPP_HAVE_TPP_TOK_POUND_POUND`) |
 * | `<::`   | `<`, `::` (requires `TPP_HAVE_TPP_TOK_COLON_COLON`) |
 *
 * @detect: #if __TPP_COUNT_TOKENS("%:") == 1 */
#ifndef TPP_HAVE_DIGRAPHS
#define TPP_HAVE_DIGRAPHS (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) /* "-fdigraphs" */
#endif /* !TPP_HAVE_DIGRAPHS */

/* Configures if line-feed tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()`
 * @detect: #if __TPP_COUNT_TOKENS("\n") != 0 */
#ifndef TPP_HAVE_TPP_TOK_LF
#define TPP_HAVE_TPP_TOK_LF TPP_COMMON_HAVE_TPP_TOK_SPACE /* "-ftok-lf" */
#endif /* !TPP_HAVE_TPP_TOK_LF */

/* Configures if whitespace tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()`
 * @detect: #if __TPP_COUNT_TOKENS(" ") != 0 */
#ifndef TPP_HAVE_TPP_TOK_SPACE
#define TPP_HAVE_TPP_TOK_SPACE TPP_COMMON_HAVE_TPP_TOK_SPACE /* "-ftok-space" */
#endif /* !TPP_HAVE_TPP_TOK_SPACE */

/* Configures if comment tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()`
 * @detect: #if __TPP_COUNT_TOKENS("// a b c") == 1 */
#ifndef TPP_HAVE_TPP_TOK_COMMENT
#define TPP_HAVE_TPP_TOK_COMMENT TPP_COMMON_HAVE_TPP_TOK_SPACE /* "-ftok-comment" */
#endif /* !TPP_HAVE_TPP_TOK_COMMENT */

/* Enable support for recognizing c++-like comments: `// like this one!`
 * @detect: #if __TPP_COUNT_TOKENS("// a b c") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT
#define TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT ((TPP_PROFILE == TPP_PROFILE_DEFAULT || TPP_HAVE_PROFILE_C_LIKE) ? 1 : TPP_COMMON_HAVE_TPP_TOK_COMMENT) /* "-ftok-slash-slash-comment" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT */

// Enable support for recognizing c-like comments: `/* like this one! */`
// @detect: #if __TPP_COUNT_TOKENS("/* a b c */") <= 1
#ifndef TPP_HAVE_TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH
#define TPP_HAVE_TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH ((TPP_PROFILE == TPP_PROFILE_DEFAULT || TPP_HAVE_PROFILE_C_LIKE) ? 1 : TPP_COMMON_HAVE_TPP_TOK_COMMENT) /* "-ftok-slash-star-comment-star-slash" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH */

/* Enable support for recognizing pascal-like comments: `(* like this one! *)`
 * @detect: #if __TPP_COUNT_TOKENS("(* a b c *)") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN
#define TPP_HAVE_TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-lparen-star-comment-star-rparen" */
#endif /* !TPP_HAVE_TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN */

/* Enable support for recognizing html-like comments: `<!-- like this one -->`
 * @detect: #if __TPP_COUNT_TOKENS("<!-- a b c -->") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-langle-exclaim-minus-minus-comment-minus-minus-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE */

/* Enable support for recognizing shell-like comments: `# like this one!`
 *
 * This still works in conjunction with "TPP_HAVE_CPP_DIRECTIVES", in
 * that unknown directives will simply be re-emit as shell comments,
 * and shell comments that don't appear at the start of lines are not
 * even processed as CPP directives.
 * @detect: #if __TPP_COUNT_TOKENS("# a b c") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_POUND_COMMENT
#define TPP_HAVE_TPP_TOK_POUND_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-pound-comment" */
#endif /* !TPP_HAVE_TPP_TOK_POUND_COMMENT */

/* Enable support for recognizing ASM-like comments: `/ like this one!`
 * @detect: #if __TPP_COUNT_TOKENS("/ a b c") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_SLASH_COMMENT
#define TPP_HAVE_TPP_TOK_SLASH_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-slash-comment" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_COMMENT */

/* Enable support for recognizing SQL-like comments: `-- like this one!`
 * @detect: #if __TPP_COUNT_TOKENS("-- a b c") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT
#define TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-minus-minus-comment" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT */

/* Enable support for recognizing deemon-doc-like comments: `@@ like this one!`
 * @detect: #if __TPP_COUNT_TOKENS("@@ a b c") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_AT_AT_COMMENT
#define TPP_HAVE_TPP_TOK_AT_AT_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-at-at-comment" */
#endif /* !TPP_HAVE_TPP_TOK_AT_AT_COMMENT */

/************************************************************************/
/* Single-char tokens                                                   */
/************************************************************************/

/* When enabled, `$` is treated as its own token, rather than as part of identifiers/keywords.
 * @detect: #if __TPP_COUNT_TOKENS("a$b") == 3 */
#ifndef TPP_HAVE_TPP_TOK_DOLLAR
#define TPP_HAVE_TPP_TOK_DOLLAR TPP_COMMON_HAVE_TPP_TOK_1CHAR /* "-ftok-dollar" */
#endif /* !TPP_HAVE_TPP_TOK_DOLLAR */

/************************************************************************/
/* Number tokens                                                        */
/************************************************************************/

/* XXX: Support for intel-assembler-style hex literals to be treated as TPP_TOK_INT:
 * https://en.wikipedia.org/wiki/Hexadecimal#Intel
 *  - FFh  (same as 0xFF)
 *  - ffh  (same as 0xff)
 *  - FFH  (same as 0xFF)
 *  - ffH  (same as 0xFF)
 */

/* XXX: Support for Pascal-style hex numbers; could be made to co-exist with
 *      TPP_HAVE_TPP_TOK_DOLLAR such that hex is only detected when the next
 *      character is 0-9, a-f, A-F:
 *  - $FF  (same as 0xFF)
 */

/* XXX: Feature to disable support for C-style "0x" radix prefixes in tpp_lexer_decodeint() */

/* `123`
 * @detect: #if __TPP_COUNT_TOKENS("123") == 1 */
#ifndef TPP_HAVE_TPP_TOK_INT
#define TPP_HAVE_TPP_TOK_INT TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "-ftok-int" */
#endif /* !TPP_HAVE_TPP_TOK_INT */

/* `123.0`
 * @detect: #if __TPP_COUNT_TOKENS("123.0") == 1 */
#ifndef TPP_HAVE_TPP_TOK_FLOAT
#define TPP_HAVE_TPP_TOK_FLOAT TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "-ftok-float" */
#endif /* !TPP_HAVE_TPP_TOK_FLOAT */

/* (Try to) be smarter regarding how float tokens are detected.
 * This tries to detect some syntax-error floating point tokens
 * and terminates them in those places where you must have wanted
 * them to terminate (even though standards say that they should
 * keep going):
 *
 * | Input       | Standard parsing                | Smart parsing                         | Comment  |
 * | ----------- | ------------------------------- | ------------------------------------- | -------- |
 * | `1E2`       | `[FLOAT:1E2]`                   | `[FLOAT:1E2]`                         | - |
 * | `1P2`       | `[INT:1P2]`                     | `[INT:1P2]`                           | - |
 * | `0x1E2`     | `[INT:0x1E2]`                   | `[INT:0x1E2]`                         | - |
 * | `0x1P2`     | `[FLOAT:0x1P2]`                 | `[FLOAT:0x1P2]`                       | - |
 * | `0xE12`     | `[INT:0xE12]`                   | `[INT:0xE12]`                         | - |
 * | `0xE+12`    | `[FLOAT:0xE+12]`                | `[INT:0xE][PLUS:+][INT:12]`           | Exponent after `x` / `X` must use `p` or `P` |
 * | `0x1E+12`   | `[FLOAT:0x1E+12]`               | `[INT:0x1E][PLUS:+][INT:12]`          | Exponent after `x` / `X` must use `p` or `P` |
 * | `0E+12`     | `[FLOAT:0E+12]`                 | `[FLOAT:0E+12]`                       | - |
 * | `0xP12`     | `[FLOAT:0xP12]`                 | `[INT:0xP12]`                         | Exponent cannot directly appear after `x` / `X` |
 * | `0xP+12`    | `[FLOAT:0xP+12]`                | `[INT:0xP][PLUS:+][INT:12]`           | Exponent cannot directly appear after `x` / `X` |
 * | `0x1P12`    | `[FLOAT:0x1P12]`                | `[FLOAT:0x1P12]`                      | - |
 * | `0x1P+12`   | `[FLOAT:0x1P+12]`               | `[FLOAT:0x1P+12]`                     | - |
 * | `0P+12`     | `[FLOAT:0P+12]`                 | `[INT:0P][PLUS:+][INT:12]`            | Without `x` / `X`, must use `e` or `E` for exponents |
 * | `0xA12`     | `[INT:0xA12]`                   | `[INT:0xA12]`                         | - |
 * | `0xA+12`    | `[INT:0xA][PLUS:+][INT:12]`     | `[INT:0xA][PLUS:+][INT:12]`           | - |
 * | `0x1A+12`   | `[INT:0x1A][PLUS:+][INT:12]`    | `[INT:0x1A][PLUS:+][INT:12]`          | - |
 * | `0A+12`     | `[INT:0A][PLUS:+][INT:12]`      | `[INT:0A][PLUS:+][INT:12]`            | - |
 * | `0x0.P12`   | `[FLOAT:0x0.P12]`               | `[FLOAT:0x0.P12]`                     | - |
 * | `0x0.P+12`  | `[FLOAT:0x0.P+12]`              | `[FLOAT:0x0.P+12]`                    | - |
 * | `0x0.1P+12` | `[FLOAT:0x0.1P+12]`             | `[FLOAT:0x0.1P+12]`                   | - |
 * | `0x.P12`    | `[FLOAT:0x.P12]`                | `[FLOAT:0x.P12]`                      | - |
 * | `0x.P+12`   | `[FLOAT:0x.P+12]`               | `[FLOAT:0x.P+12]`                     | - |
 * | `0x.1P+12`  | `[FLOAT:0x.1P+12]`              | `[FLOAT:0x.1P+12]`                    | - |
 * | `0x.E12`    | `[FLOAT:0x.E12]`                | `[FLOAT:0x.E12]`                      | - |
 * | `0x.E+12`   | `[FLOAT:0x.E+12]`               | `[FLOAT:0x.E][PLUS:+][INT:12]`        | With `x` / `X`, must use `p` or `P` for exponents |
 * | `0x.1E+12`  | `[FLOAT:0x.1E+12]`              | `[FLOAT:0x.1E][PLUS:+][INT:12]`       | With `x` / `X`, must use `p` or `P` for exponents |
 * | `0.P+12`    | `[FLOAT:0.P+12]`                | `[INT:0][DOT:.][P:P][PLUS:+][INT:12]` | Without `x` / `X`, must use `e` or `E` for exponents |
 * | `0.E+12`    | `[FLOAT:0.E+12]`                | `[FLOAT:0.E+12]`                      | - |
 * | `0x.A12`    | `[FLOAT:0x.A12]`                | `[FLOAT:0x.A12]`                      | - |
 * | `0x.A+12`   | `[FLOAT:0x.A][PLUS:+][INT:12]`  | `[FLOAT:0x.A][PLUS:+][INT:12]`        | - |
 * | `0x.1A+12`  | `[FLOAT:0x.1A][PLUS:+][INT:12]` | `[FLOAT:0x.1A][PLUS:+][INT:12]`       | - |
 * | `0.A+12`    | `[FLOAT:0.A][PLUS:+][INT:12]`   | `[INT:0][DOT:.][A:A][PLUS:+][INT:12]` | Without `x` / `X`, first character after `.` must be 0-9 |
 * | `012`       | `[INT:012]`                     | `[INT:012]`                           | - |
 * | `0.12`      | `[FLOAT:0.12]`                  | `[FLOAT:0.12]`                        | - |
 * | `0..12`     | `[FLOAT:0..12]`                 | `[INT:0][DOT_DOT:..][INT:12]`         | There can be at most 1 decimal-`.` |
 * | `0foo`      | `[INT:0foo]`                    | `[INT:0foo]`                          | - |
 * | `0.foo`     | `[FLOAT:0.foo]`                 | `[INT:0][DOT:.][foo:foo]`             | Hex-character `f` after `.` requires `x` / `X` |
 * | `0..foo`    | `[FLOAT:0..foo]`                | `[INT:0][DOT_DOT:..][foo:foo]`        | There can be at most 1 decimal-`.` |
 * | `0xfoo`     | `[INT:0xfoo]`                   | `[INT:0xfoo]`                         | - |
 * | `0x.foo`    | `[FLOAT:0x.foo]`                | `[FLOAT:0x.foo]`                      | - |
 * | `0x..foo`   | `[FLOAT:0x..foo]`               | `[INT:0x][DOT_DOT:..][foo:foo]`       | There can be at most 1 decimal-`.` |
 * | `0x1foo`    | `[INT:0x1foo]`                  | `[INT:0x1foo]`                        | - |
 * | `0x1.foo`   | `[FLOAT:0x1.foo]`               | `[FLOAT:0x1.foo]`                     | - |
 * | `0x1..foo`  | `[FLOAT:0x1..foo]`              | `[INT:0x1][DOT_DOT:..][foo:foo]`      | There can be at most 1 decimal-`.` |
 * | `0luz`      | `[INT:0luz]`                    | `[INT:0luz]`                          | - |
 * | `0.luz`     | `[FLOAT:0.luz]`                 | `[INT:0][DOT:.][luz:luz]`             | Character after `.` must be decimal |
 * | `0..luz`    | `[FLOAT:0..luz]`                | `[INT:0][DOT_DOT:..][luz:luz]`        | There can be at most 1 decimal-`.` |
 * | `0xluz`     | `[INT:0xluz]`                   | `[INT:0xluz]`                         | - |
 * | `0x.luz`    | `[FLOAT:0x.luz]`                | `[INT:0x][DOT:.][luz:luz]`            | Character after `.` must be hexadecimal |
 * | `0x..luz`   | `[FLOAT:0x..luz]`               | `[INT:0x][DOT_DOT:..][luz:luz]`       | There can be at most 1 decimal-`.` |
 * | `0x1luz`    | `[INT:0x1luz]`                  | `[INT:0x1luz]`                        | - |
 * | `0x1.luz`   | `[FLOAT:0x1.luz]`               | `[INT:0x1][DOT:.][luz:luz]`           | Character after `.` must be hexadecimal |
 * | `0x1..luz`  | `[FLOAT:0x1..luz]`              | `[INT:0x1][DOT_DOT:..][luz:luz]`      | There can be at most 1 decimal-`.` |
 * | `12`        | `[INT:12]`                      | `[INT:12]`                            | - |
 * | `.12`       | `[FLOAT:.12]`                   | `[FLOAT:.12]`                         | - |
 * | `..12`      | `[DOT_DOT:..][INT:12]`          | `[DOT_DOT:..][INT:12]`                | There can be at most 1 decimal-`.` |
 *
 * @detect: #if __TPP_COUNT_TOKENS("0x1P+12") == 1 && __TPP_COUNT_TOKENS("0xE+12") == 3 */
#ifndef TPP_HAVE_SMART_FLOAT_TOKENS
#define TPP_HAVE_SMART_FLOAT_TOKENS ((TPP_HAVE_PROFILE_DEFAULT && TPP_HAVE_TPP_TOK_FLOAT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : TPP_CONF_FEAT1) : 0) /* "-fsmart-float-tokens" */
#endif /* !TPP_HAVE_SMART_FLOAT_TOKENS */

/************************************************************************/
/* String tokens                                                        */
/************************************************************************/

/* XXX: Support for sql-style '-string literals ('' is escape for ', and line-feeds are allowed) */
/* XXX: Support for sql-style "-string literals ("" is escape for ", and line-feeds are allowed) */
/* XXX: Support for sql-style E'foo'-string literals (line-feeds are allowed, and \-escape sequences are handled) */
/* XXX: Support for sql-style E"foo"-string literals (line-feeds are allowed, and \-escape sequences are handled) */
/* XXX: Support for javascript-style `foo` format string literals (use a hook for implementing `${expr}`) */
/* XXX: Support for deemon-style f"foo" / F"foo" format string literals (use a hook for implementing `{expr}`) */

/* Support for C character literals: `'foo'`
 * @detect: #if __TPP_COUNT_TOKENS("'foo'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_C_CHAR
#define TPP_HAVE_TPP_TOK_C_CHAR TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "-ftok-char" */
#endif /* !TPP_HAVE_TPP_TOK_C_CHAR */

/* Support for C string literals: `"foo"`
 * @detect: #if __TPP_COUNT_TOKENS('"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_C_STRING
#define TPP_HAVE_TPP_TOK_C_STRING TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "-ftok-string" */
#endif /* !TPP_HAVE_TPP_TOK_C_STRING */

/* Support for string literals: `R"AB(foo)AB"`
 * @detect: #if __TPP_COUNT_TOKENS('R"AB(foo)AB"') == 1 && __TPP_STR_SIZE(R"AB(foo)AB") == 3 */
#ifndef TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-raw-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */

/* Support for string literals: `L"foo"`
 * When `TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL` is also enabled, also support `LR"AB(foo)AB")`
 * @detect: #if __TPP_COUNT_TOKENS('L"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-wide-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */

/* Support for string literals: `u8"foo"`
 * When `TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL` is also enabled, also support `u8R"AB(foo)AB"`
 * @detect: #if __TPP_COUNT_TOKENS('u8"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf8-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */

/* Support for string literals: `u"foo"`
 * When `TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL` is also enabled, also support `uR"AB(foo)AB"`
 * @detect: #if __TPP_COUNT_TOKENS('u"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf16-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */

/* Support for string literals: `U"foo"`
 * When `TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL` is also enabled, also support `UR"AB(foo)AB"`
 * @detect: #if __TPP_COUNT_TOKENS('U"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf32-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */

/* Support for string literals: `R'AB(f)AB'`
 * @detect: #if __TPP_COUNT_TOKENS("R'AB(foo)AB'") == 1 && __TPP_STR_SIZE(R'AB(foo)AB') == 3 */
#ifndef TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-raw-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */

/* Support for string literals: `L'f'`
 * When `TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL` is also enabled, also support `LR'AB(f)AB'`
 * @detect: #if __TPP_COUNT_TOKENS("L'f'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-wide-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */

/* Support for string literals: `u8'f'`
 * When `TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL` is also enabled, also support `u8R'AB(f)AB'`
 * @detect: #if __TPP_COUNT_TOKENS("u8'f'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf8-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */

/* Support for string literals: `u'f'`
 * When `TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL` is also enabled, also support `uR'AB(f)AB'`
 * @detect: #if __TPP_COUNT_TOKENS("u'f'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf16-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */

/* Support for string literals: `U'f'`
 * When `TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL` is also enabled, also support `UR'AB(f)AB'`
 * @detect: #if __TPP_COUNT_TOKENS("U'f'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf32-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */

/* Support for deemon-style raw string literals: `R"foo"` and `r"foo"`
 * @detect: #if __TPP_COUNT_TOKENS('R"foo"') == 1 && __TPP_STR_SIZE(R"AB(foo)AB") == 9 */
#ifndef TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "-ftok-raw-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */

/* Support for deemon-style raw string literals: `R'bar'` and `r'bar'`
 * @detect: #if __TPP_COUNT_TOKENS('R"foo"') == 1 && __TPP_STR_SIZE(R'AB(foo)AB') == 9 */
#ifndef TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "-ftok-raw-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */

/* Support for java-style block string literals: `"""foo"""`
 * @detect: #if __TPP_COUNT_TOKENS('"""\n a\n b"""') == 1 */
#ifndef TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "-ftok-block-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */

/* Support for java-style block string literals (but with single-ticks): `'''foo'''`
 * @detect: #if __TPP_COUNT_TOKENS("'''\n a\n b'''") == 1 */
#ifndef TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "-ftok-block-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */

/* Feature-flag: treat line-feeds like any regular character in string tokens:
 * - `TPP_HAVE_TPP_TOK_C_STRING`
 * - `TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL`
 * - `TPP_HAVE_TPP_TOK_C_CHAR`
 * - `TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL`
 * - `TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL`
 * - `TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL`
 *
 * When this flag is disabled, line-feeds in such string tokens will instead
 * terminate the string, and cause a `TPP_W_STRING_TERMINATED_BY_LINEFEED`
 * warning to be emitted.
 *
 * @detect: #if __TPP_COUNT_TOKENS("\"\n\"") == 1 */
#ifndef TPP_HAVE_STRING_ALLOW_MULTILINE
#define TPP_HAVE_STRING_ALLOW_MULTILINE (TPP_HAVE_PROFILE_NOT_MINIMAL ? TPP_CONF_FEAT0 : 0) /* "-fstring-allow-multiline" */
#endif /* !TPP_HAVE_STRING_ALLOW_MULTILINE */

/* Alter behavior of `TPP_HAVE_STRING_ALLOW_MULTILINE`
 * (that only takes effect when that feature is enabled):
 * - Multi-line strings continue to be allowed (does not affect behavior)
 * - When a multi-line string is encountered, emit a
 *   warning `TPP_W_STRING_CONTINUED_AFTER_LINEFEED`
 *   (see `TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED`)
 *
 * If this warning isn't wanted, it can be disabled by `-Wno-multiline-string` */
#ifndef TPP_HAVE_STRING_WARN_MULTILINE
#define TPP_HAVE_STRING_WARN_MULTILINE \
	(TPP_HAVE_STRING_ALLOW_MULTILINE && TPP_HAVE_WARNINGS)
#endif /* !TPP_HAVE_STRING_WARN_MULTILINE */

/* Enable support for automatic concatenation of adjacent string tokens.
 * This affects the behavior of `tpp_lexer_parsestring_ex()` and its
 * companion `tpp_lexer_parsestring_cb()`, such that they will only yield
 * to the next token, but not check if that next token might be another
 * string.
 *
 * This feature should always be enabled for C/C++ (since their standards
 * mandate that adjacent string tokens be joined into a single literal),
 * but other languages might not want such behavior.
 *
 * WARNING: TPP will automatically join any type of string-like-token with any
 *          other when this feature is enabled (e.g. `"FOO" 'BAR' R"AB(BAZ)AB"`
 *          becomes a single string literal `"FOOBARBAZ"`)
 */
#ifndef TPP_HAVE_STRING_AUTO_CONCAT
#define TPP_HAVE_STRING_AUTO_CONCAT ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) /* "-fstring-auto-concat" */
#endif /* !TPP_HAVE_STRING_AUTO_CONCAT */


#undef TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
#if (TPP_HAVE_TPP_TOK_SLASH_STAR_COMMENT_STAR_SLASH ||   \
     TPP_HAVE_TPP_TOK_LPAREN_STAR_COMMENT_STAR_RPAREN || \
     TPP_HAVE_TPP_TOK_LANGLE_EXCLAIM_MINUS_MINUS_COMMENT_MINUS_MINUS_RANGLE)
#define TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE
#if (TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT || \
     TPP_HAVE_TPP_TOK_POUND_COMMENT ||       \
     TPP_HAVE_TPP_TOK_SLASH_COMMENT ||       \
     TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT)
#define TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_COMMENTLIKE
#if (TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE || \
     TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE)
#define TPP_HAVE_TPP_TOK_COMMENTLIKE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_COMMENTLIKE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_C_STRINGLIKE_SQUOTE
#if (TPP_HAVE_TPP_TOK_C_CHAR ||                 \
     TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL || \
     TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL ||       \
     TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL)
#define TPP_HAVE_TPP_TOK_C_STRINGLIKE_SQUOTE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_C_STRINGLIKE_SQUOTE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_C_STRINGLIKE_DQUOTE
#if (TPP_HAVE_TPP_TOK_C_STRING ||                 \
     TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||       \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL)
#define TPP_HAVE_TPP_TOK_C_STRINGLIKE_DQUOTE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_C_STRINGLIKE_DQUOTE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_C_STRINGLIKE
#if (TPP_HAVE_TPP_TOK_C_STRINGLIKE_SQUOTE || \
     TPP_HAVE_TPP_TOK_C_STRINGLIKE_DQUOTE)
#define TPP_HAVE_TPP_TOK_C_STRINGLIKE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_C_STRINGLIKE 0
#endif /* !... */
#undef TPP_HAVE_STRING_ESCAPE
#if (TPP_HAVE_TPP_TOK_C_STRING ||                 \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
     TPP_HAVE_TPP_TOK_C_CHAR ||                   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL)
#define TPP_HAVE_STRING_ESCAPE 1
#else /* ... */
#define TPP_HAVE_STRING_ESCAPE 0
#endif /* !... */

/************************************************************************/
/* Multi-char tokens                                                    */
/************************************************************************/

/*[[[deemon
import * from deemon;
local charNames = {
	"<": "LANGLE",
	">": "RANGLE",
	"!": "EXCLAIM",
	"=": "EQUAL",
	"#": "POUND",
	"+": "PLUS",
	"-": "MINUS",
	"*": "STAR",
	"/": "SLASH",
	"%": "PERCENT",
	"&": "AMP",
	"|": "PIPE",
	"^": "HAT",
	".": "DOT",
	":": "COLON",
	"?": "QMARK",
	"@": "AT",
	"~": "TILDE",
};
@@Master controller for available multi-char tokens (add
@@stuff here and run "make" to get extra multi-char token)
local MC_TOKENS = {
	{ "<<", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ ">>", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "==", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "!=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ ">=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "<=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "...", "(TPP_CONF_IS_RT(TPP_COMMON_HAVE_TPP_TOK_C_TOKENS) ? TPP_COMMON_HAVE_TPP_TOK_C_TOKENS : (TPP_COMMON_HAVE_TPP_TOK_C_TOKENS || TPP_HAVE_VA_ARGS_IN_MACROS || TPP_HAVE_NAMED_VARARGS_IN_MACROS))", "" },
	{ "+=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "-=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "*=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "/=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "%=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "<<=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ ">>=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "&=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "|=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "^=", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "//", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "WARNING: This token conflicts with TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT" },
	{ "//=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "WARNING: This token conflicts with TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT" },
	{ "**=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "@=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "##", "(TPP_CONF_IS_RT(TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS) ? TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS : (TPP_HAVE_GLUE_MACRO_ARGUMENT || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS))", "" },
	{ "&&", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "||", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "^^", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "++", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ "--", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "WARNING: This token conflicts with TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT" },
	{ "**", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "~~", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "~=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "->", "TPP_COMMON_HAVE_TPP_TOK_C_TOKENS", "" },
	{ ":=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "::", "TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS", "" },
	{ "->*", "TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS", "" },
	{ ".*", "TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS", "" },
	{ "..", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<>", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<<<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ ">>>", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<<<=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ ">>>=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "===", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "!==", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "!!", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "??", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "?=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "><", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=+", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=-", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=*", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=**", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=/", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=//", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "WARNING: This token conflicts with TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT" },
	{ "=%", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=&", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=|", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=^", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=<<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=<<<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=>", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=>>", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=>>>", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=@", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=~", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=:", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=!", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "==!", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=?", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<-", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "*<-", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "*.", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "-<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ ">-", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<=>", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<->", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ ">=<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ ">-<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<=<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<-<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ ">=>", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ ">->", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "%%", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "%%=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=%%", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "@@", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "@@=", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "=@@", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "->>", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "->>>", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "-<<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "-<<<", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ ">>-", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ ">>>-", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<<-", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
	{ "<<<-", "TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS", "" },
};
function tokenName(tok: string): string {
	return "_".join(for (local ch: tok) charNames[ch]);
}
for (local tok, def, comment: MC_TOKENS.sorted()) {
	local name = tokenName(tok);
	print('/' '* "', tok, '"', comment ? f"  ({comment})" : "");
	print(' * @detect: #if __TPP_COUNT_TOKENS("', tok, '") == 1 *' '/');
	print('#ifndef TPP_HAVE_TPP_TOK_', name, '');
	print('#define TPP_HAVE_TPP_TOK_', name, ' ', def, ' /' '* "-ftok-', name.lower(), '" *' '/');
	print('#endif /' '* !TPP_HAVE_TPP_TOK_', name, ' *' '/');
	print;
}
local firstChars = HashSet(MC_TOKENS.each.first.first).sorted();
for (local firstChar: firstChars) {
	print("#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_", charNames[firstChar]);
	local startsWith = MC_TOKENS.filter(e -> e.first.startswith(firstChar)).sorted().each.first;
	print("#if ", " || ".join(for (local c: startsWith) f"TPP_HAVE_TPP_TOK_{tokenName(c)}"));
	print("#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_", charNames[firstChar], " 1");
	print("#else /" "* ... *" "/");
	print("#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_", charNames[firstChar], " 0");
	print("#endif /" "* !... *" "/");
}
print("#undef TPP_HAVE_TPP_TOK_MC");
print("#if ", " || ".join(for (local c: firstChars) f"TPP_HAVE_TPP_TOK_MC_STARTSWITH_{charNames[c]}"));
print("#define TPP_HAVE_TPP_TOK_MC 1");
print("#else /" "* ... *" "/");
print("#define TPP_HAVE_TPP_TOK_MC 0");
print("#endif /" "* !... *" "/");
]]]*/
/* "!!"
 * @detect: #if __TPP_COUNT_TOKENS("!!") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
#define TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-exclaim_exclaim" */
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */

/* "!="
 * @detect: #if __TPP_COUNT_TOKENS("!=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-exclaim_equal" */
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */

/* "!=="
 * @detect: #if __TPP_COUNT_TOKENS("!==") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-exclaim_equal_equal" */
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */

/* "##"
 * @detect: #if __TPP_COUNT_TOKENS("##") == 1 */
#ifndef TPP_HAVE_TPP_TOK_POUND_POUND
#define TPP_HAVE_TPP_TOK_POUND_POUND (TPP_CONF_IS_RT(TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS) ? TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS : (TPP_HAVE_GLUE_MACRO_ARGUMENT || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS)) /* "-ftok-pound_pound" */
#endif /* !TPP_HAVE_TPP_TOK_POUND_POUND */

/* "%%"
 * @detect: #if __TPP_COUNT_TOKENS("%%") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PERCENT_PERCENT
#define TPP_HAVE_TPP_TOK_PERCENT_PERCENT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-percent_percent" */
#endif /* !TPP_HAVE_TPP_TOK_PERCENT_PERCENT */

/* "%%="
 * @detect: #if __TPP_COUNT_TOKENS("%%=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL
#define TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-percent_percent_equal" */
#endif /* !TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL */

/* "%="
 * @detect: #if __TPP_COUNT_TOKENS("%=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PERCENT_EQUAL
#define TPP_HAVE_TPP_TOK_PERCENT_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-percent_equal" */
#endif /* !TPP_HAVE_TPP_TOK_PERCENT_EQUAL */

/* "&&"
 * @detect: #if __TPP_COUNT_TOKENS("&&") == 1 */
#ifndef TPP_HAVE_TPP_TOK_AMP_AMP
#define TPP_HAVE_TPP_TOK_AMP_AMP TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-amp_amp" */
#endif /* !TPP_HAVE_TPP_TOK_AMP_AMP */

/* "&="
 * @detect: #if __TPP_COUNT_TOKENS("&=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_AMP_EQUAL
#define TPP_HAVE_TPP_TOK_AMP_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-amp_equal" */
#endif /* !TPP_HAVE_TPP_TOK_AMP_EQUAL */

/* "**"
 * @detect: #if __TPP_COUNT_TOKENS("**") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_STAR
#define TPP_HAVE_TPP_TOK_STAR_STAR TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-star_star" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_STAR */

/* "**="
 * @detect: #if __TPP_COUNT_TOKENS("**=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
#define TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-star_star_equal" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */

/* "*."
 * @detect: #if __TPP_COUNT_TOKENS("*.") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_DOT
#define TPP_HAVE_TPP_TOK_STAR_DOT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-star_dot" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_DOT */

/* "*<-"
 * @detect: #if __TPP_COUNT_TOKENS("*<-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
#define TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-star_langle_minus" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */

/* "*="
 * @detect: #if __TPP_COUNT_TOKENS("*=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_EQUAL
#define TPP_HAVE_TPP_TOK_STAR_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-star_equal" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_EQUAL */

/* "++"
 * @detect: #if __TPP_COUNT_TOKENS("++") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PLUS_PLUS
#define TPP_HAVE_TPP_TOK_PLUS_PLUS TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-plus_plus" */
#endif /* !TPP_HAVE_TPP_TOK_PLUS_PLUS */

/* "+="
 * @detect: #if __TPP_COUNT_TOKENS("+=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PLUS_EQUAL
#define TPP_HAVE_TPP_TOK_PLUS_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-plus_equal" */
#endif /* !TPP_HAVE_TPP_TOK_PLUS_EQUAL */

/* "--"  (WARNING: This token conflicts with TPP_HAVE_TPP_TOK_MINUS_MINUS_COMMENT)
 * @detect: #if __TPP_COUNT_TOKENS("--") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_MINUS
#define TPP_HAVE_TPP_TOK_MINUS_MINUS TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-minus_minus" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_MINUS */

/* "-<"
 * @detect: #if __TPP_COUNT_TOKENS("-<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_LANGLE
#define TPP_HAVE_TPP_TOK_MINUS_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-minus_langle" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_LANGLE */

/* "-<<"
 * @detect: #if __TPP_COUNT_TOKENS("-<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-minus_langle_langle" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE */

/* "-<<<"
 * @detect: #if __TPP_COUNT_TOKENS("-<<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-minus_langle_langle_langle" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */

/* "-="
 * @detect: #if __TPP_COUNT_TOKENS("-=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_EQUAL
#define TPP_HAVE_TPP_TOK_MINUS_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-minus_equal" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_EQUAL */

/* "->"
 * @detect: #if __TPP_COUNT_TOKENS("->") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-minus_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE */

/* "->*"
 * @detect: #if __TPP_COUNT_TOKENS("->*") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS /* "-ftok-minus_rangle_star" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */

/* "->>"
 * @detect: #if __TPP_COUNT_TOKENS("->>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-minus_rangle_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE */

/* "->>>"
 * @detect: #if __TPP_COUNT_TOKENS("->>>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-minus_rangle_rangle_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */

/* ".*"
 * @detect: #if __TPP_COUNT_TOKENS(".*") == 1 */
#ifndef TPP_HAVE_TPP_TOK_DOT_STAR
#define TPP_HAVE_TPP_TOK_DOT_STAR TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS /* "-ftok-dot_star" */
#endif /* !TPP_HAVE_TPP_TOK_DOT_STAR */

/* ".."
 * @detect: #if __TPP_COUNT_TOKENS("..") == 1 */
#ifndef TPP_HAVE_TPP_TOK_DOT_DOT
#define TPP_HAVE_TPP_TOK_DOT_DOT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-dot_dot" */
#endif /* !TPP_HAVE_TPP_TOK_DOT_DOT */

/* "..."
 * @detect: #if __TPP_COUNT_TOKENS("...") == 1 */
#ifndef TPP_HAVE_TPP_TOK_DOT_DOT_DOT
#define TPP_HAVE_TPP_TOK_DOT_DOT_DOT (TPP_CONF_IS_RT(TPP_COMMON_HAVE_TPP_TOK_C_TOKENS) ? TPP_COMMON_HAVE_TPP_TOK_C_TOKENS : (TPP_COMMON_HAVE_TPP_TOK_C_TOKENS || TPP_HAVE_VA_ARGS_IN_MACROS || TPP_HAVE_NAMED_VARARGS_IN_MACROS)) /* "-ftok-dot_dot_dot" */
#endif /* !TPP_HAVE_TPP_TOK_DOT_DOT_DOT */

/* "//"  (WARNING: This token conflicts with TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT)
 * @detect: #if __TPP_COUNT_TOKENS("//") == 1 */
#ifndef TPP_HAVE_TPP_TOK_SLASH_SLASH
#define TPP_HAVE_TPP_TOK_SLASH_SLASH TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-slash_slash" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_SLASH */

/* "//="  (WARNING: This token conflicts with TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT)
 * @detect: #if __TPP_COUNT_TOKENS("//=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
#define TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-slash_slash_equal" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */

/* "/="
 * @detect: #if __TPP_COUNT_TOKENS("/=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_SLASH_EQUAL
#define TPP_HAVE_TPP_TOK_SLASH_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-slash_equal" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_EQUAL */

/* "::"
 * @detect: #if __TPP_COUNT_TOKENS("::") == 1 */
#ifndef TPP_HAVE_TPP_TOK_COLON_COLON
#define TPP_HAVE_TPP_TOK_COLON_COLON TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS /* "-ftok-colon_colon" */
#endif /* !TPP_HAVE_TPP_TOK_COLON_COLON */

/* ":="
 * @detect: #if __TPP_COUNT_TOKENS(":=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_COLON_EQUAL
#define TPP_HAVE_TPP_TOK_COLON_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-colon_equal" */
#endif /* !TPP_HAVE_TPP_TOK_COLON_EQUAL */

/* "<-"
 * @detect: #if __TPP_COUNT_TOKENS("<-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_MINUS
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_minus" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS */

/* "<-<"
 * @detect: #if __TPP_COUNT_TOKENS("<-<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_minus_langle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */

/* "<->"
 * @detect: #if __TPP_COUNT_TOKENS("<->") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_minus_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */

/* "<<"
 * @detect: #if __TPP_COUNT_TOKENS("<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-langle_langle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE */

/* "<<-"
 * @detect: #if __TPP_COUNT_TOKENS("<<-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_langle_minus" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS */

/* "<<<"
 * @detect: #if __TPP_COUNT_TOKENS("<<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_langle_langle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */

/* "<<<-"
 * @detect: #if __TPP_COUNT_TOKENS("<<<-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_langle_langle_minus" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS */

/* "<<<="
 * @detect: #if __TPP_COUNT_TOKENS("<<<=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_langle_langle_equal" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */

/* "<<="
 * @detect: #if __TPP_COUNT_TOKENS("<<=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-langle_langle_equal" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */

/* "<="
 * @detect: #if __TPP_COUNT_TOKENS("<=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-langle_equal" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL */

/* "<=<"
 * @detect: #if __TPP_COUNT_TOKENS("<=<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_equal_langle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */

/* "<=>"
 * @detect: #if __TPP_COUNT_TOKENS("<=>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_equal_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */

/* "<>"
 * @detect: #if __TPP_COUNT_TOKENS("<>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_RANGLE */

/* "=!"
 * @detect: #if __TPP_COUNT_TOKENS("=!") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
#define TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_exclaim" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */

/* "=%"
 * @detect: #if __TPP_COUNT_TOKENS("=%") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PERCENT
#define TPP_HAVE_TPP_TOK_EQUAL_PERCENT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_percent" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PERCENT */

/* "=%%"
 * @detect: #if __TPP_COUNT_TOKENS("=%%") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT
#define TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_percent_percent" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT */

/* "=&"
 * @detect: #if __TPP_COUNT_TOKENS("=&") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_AMP
#define TPP_HAVE_TPP_TOK_EQUAL_AMP TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_amp" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_AMP */

/* "=*"
 * @detect: #if __TPP_COUNT_TOKENS("=*") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_STAR
#define TPP_HAVE_TPP_TOK_EQUAL_STAR TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_star" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_STAR */

/* "=**"
 * @detect: #if __TPP_COUNT_TOKENS("=**") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
#define TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_star_star" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */

/* "=+"
 * @detect: #if __TPP_COUNT_TOKENS("=+") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PLUS
#define TPP_HAVE_TPP_TOK_EQUAL_PLUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_plus" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PLUS */

/* "=-"
 * @detect: #if __TPP_COUNT_TOKENS("=-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_MINUS
#define TPP_HAVE_TPP_TOK_EQUAL_MINUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_minus" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_MINUS */

/* "=/"
 * @detect: #if __TPP_COUNT_TOKENS("=/") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_SLASH
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_slash" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_SLASH */

/* "=//"  (WARNING: This token conflicts with TPP_HAVE_TPP_TOK_SLASH_SLASH_COMMENT)
 * @detect: #if __TPP_COUNT_TOKENS("=//") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_slash_slash" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */

/* "=:"
 * @detect: #if __TPP_COUNT_TOKENS("=:") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_COLON
#define TPP_HAVE_TPP_TOK_EQUAL_COLON TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_colon" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_COLON */

/* "=<"
 * @detect: #if __TPP_COUNT_TOKENS("=<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_LANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_langle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE */

/* "=<<"
 * @detect: #if __TPP_COUNT_TOKENS("=<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_langle_langle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */

/* "=<<<"
 * @detect: #if __TPP_COUNT_TOKENS("=<<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_langle_langle_langle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */

/* "=="
 * @detect: #if __TPP_COUNT_TOKENS("==") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-equal_equal" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL */

/* "==!"
 * @detect: #if __TPP_COUNT_TOKENS("==!") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_equal_exclaim" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */

/* "==="
 * @detect: #if __TPP_COUNT_TOKENS("===") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_equal_equal" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */

/* "=>"
 * @detect: #if __TPP_COUNT_TOKENS("=>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_RANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE */

/* "=>>"
 * @detect: #if __TPP_COUNT_TOKENS("=>>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_rangle_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */

/* "=>>>"
 * @detect: #if __TPP_COUNT_TOKENS("=>>>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_rangle_rangle_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */

/* "=?"
 * @detect: #if __TPP_COUNT_TOKENS("=?") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_QMARK
#define TPP_HAVE_TPP_TOK_EQUAL_QMARK TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_qmark" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_QMARK */

/* "=@"
 * @detect: #if __TPP_COUNT_TOKENS("=@") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_AT
#define TPP_HAVE_TPP_TOK_EQUAL_AT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_at" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_AT */

/* "=@@"
 * @detect: #if __TPP_COUNT_TOKENS("=@@") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_AT_AT
#define TPP_HAVE_TPP_TOK_EQUAL_AT_AT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_at_at" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_AT_AT */

/* "=^"
 * @detect: #if __TPP_COUNT_TOKENS("=^") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_HAT
#define TPP_HAVE_TPP_TOK_EQUAL_HAT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_hat" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_HAT */

/* "=|"
 * @detect: #if __TPP_COUNT_TOKENS("=|") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PIPE
#define TPP_HAVE_TPP_TOK_EQUAL_PIPE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_pipe" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PIPE */

/* "=~"
 * @detect: #if __TPP_COUNT_TOKENS("=~") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_TILDE
#define TPP_HAVE_TPP_TOK_EQUAL_TILDE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal_tilde" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_TILDE */

/* ">-"
 * @detect: #if __TPP_COUNT_TOKENS(">-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_MINUS
#define TPP_HAVE_TPP_TOK_RANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_minus" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_MINUS */

/* ">-<"
 * @detect: #if __TPP_COUNT_TOKENS(">-<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_minus_langle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */

/* ">->"
 * @detect: #if __TPP_COUNT_TOKENS(">->") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_minus_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */

/* "><"
 * @detect: #if __TPP_COUNT_TOKENS("><") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_langle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_LANGLE */

/* ">="
 * @detect: #if __TPP_COUNT_TOKENS(">=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-rangle_equal" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_EQUAL */

/* ">=<"
 * @detect: #if __TPP_COUNT_TOKENS(">=<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_equal_langle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */

/* ">=>"
 * @detect: #if __TPP_COUNT_TOKENS(">=>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_equal_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */

/* ">>"
 * @detect: #if __TPP_COUNT_TOKENS(">>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-rangle_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE */

/* ">>-"
 * @detect: #if __TPP_COUNT_TOKENS(">>-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_rangle_minus" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS */

/* ">>="
 * @detect: #if __TPP_COUNT_TOKENS(">>=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-rangle_rangle_equal" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */

/* ">>>"
 * @detect: #if __TPP_COUNT_TOKENS(">>>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_rangle_rangle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */

/* ">>>-"
 * @detect: #if __TPP_COUNT_TOKENS(">>>-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_rangle_rangle_minus" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS */

/* ">>>="
 * @detect: #if __TPP_COUNT_TOKENS(">>>=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle_rangle_rangle_equal" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */

/* "?="
 * @detect: #if __TPP_COUNT_TOKENS("?=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_QMARK_EQUAL
#define TPP_HAVE_TPP_TOK_QMARK_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-qmark_equal" */
#endif /* !TPP_HAVE_TPP_TOK_QMARK_EQUAL */

/* "??"
 * @detect: #if __TPP_COUNT_TOKENS("??") == 1 */
#ifndef TPP_HAVE_TPP_TOK_QMARK_QMARK
#define TPP_HAVE_TPP_TOK_QMARK_QMARK TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-qmark_qmark" */
#endif /* !TPP_HAVE_TPP_TOK_QMARK_QMARK */

/* "@="
 * @detect: #if __TPP_COUNT_TOKENS("@=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_AT_EQUAL
#define TPP_HAVE_TPP_TOK_AT_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-at_equal" */
#endif /* !TPP_HAVE_TPP_TOK_AT_EQUAL */

/* "@@"
 * @detect: #if __TPP_COUNT_TOKENS("@@") == 1 */
#ifndef TPP_HAVE_TPP_TOK_AT_AT
#define TPP_HAVE_TPP_TOK_AT_AT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-at_at" */
#endif /* !TPP_HAVE_TPP_TOK_AT_AT */

/* "@@="
 * @detect: #if __TPP_COUNT_TOKENS("@@=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_AT_AT_EQUAL
#define TPP_HAVE_TPP_TOK_AT_AT_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-at_at_equal" */
#endif /* !TPP_HAVE_TPP_TOK_AT_AT_EQUAL */

/* "^="
 * @detect: #if __TPP_COUNT_TOKENS("^=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_HAT_EQUAL
#define TPP_HAVE_TPP_TOK_HAT_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-hat_equal" */
#endif /* !TPP_HAVE_TPP_TOK_HAT_EQUAL */

/* "^^"
 * @detect: #if __TPP_COUNT_TOKENS("^^") == 1 */
#ifndef TPP_HAVE_TPP_TOK_HAT_HAT
#define TPP_HAVE_TPP_TOK_HAT_HAT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-hat_hat" */
#endif /* !TPP_HAVE_TPP_TOK_HAT_HAT */

/* "|="
 * @detect: #if __TPP_COUNT_TOKENS("|=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PIPE_EQUAL
#define TPP_HAVE_TPP_TOK_PIPE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-pipe_equal" */
#endif /* !TPP_HAVE_TPP_TOK_PIPE_EQUAL */

/* "||"
 * @detect: #if __TPP_COUNT_TOKENS("||") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PIPE_PIPE
#define TPP_HAVE_TPP_TOK_PIPE_PIPE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-pipe_pipe" */
#endif /* !TPP_HAVE_TPP_TOK_PIPE_PIPE */

/* "~="
 * @detect: #if __TPP_COUNT_TOKENS("~=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_TILDE_EQUAL
#define TPP_HAVE_TPP_TOK_TILDE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-tilde_equal" */
#endif /* !TPP_HAVE_TPP_TOK_TILDE_EQUAL */

/* "~~"
 * @detect: #if __TPP_COUNT_TOKENS("~~") == 1 */
#ifndef TPP_HAVE_TPP_TOK_TILDE_TILDE
#define TPP_HAVE_TPP_TOK_TILDE_TILDE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-tilde_tilde" */
#endif /* !TPP_HAVE_TPP_TOK_TILDE_TILDE */

#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL || TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND
#if TPP_HAVE_TPP_TOK_POUND_POUND
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT
#if TPP_HAVE_TPP_TOK_PERCENT_PERCENT || TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL || TPP_HAVE_TPP_TOK_PERCENT_EQUAL
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP
#if TPP_HAVE_TPP_TOK_AMP_AMP || TPP_HAVE_TPP_TOK_AMP_EQUAL
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR
#if TPP_HAVE_TPP_TOK_STAR_STAR || TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL || TPP_HAVE_TPP_TOK_STAR_DOT || TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS || TPP_HAVE_TPP_TOK_STAR_EQUAL
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS
#if TPP_HAVE_TPP_TOK_PLUS_PLUS || TPP_HAVE_TPP_TOK_PLUS_EQUAL
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS
#if TPP_HAVE_TPP_TOK_MINUS_MINUS || TPP_HAVE_TPP_TOK_MINUS_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_MINUS_EQUAL || TPP_HAVE_TPP_TOK_MINUS_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT
#if TPP_HAVE_TPP_TOK_DOT_STAR || TPP_HAVE_TPP_TOK_DOT_DOT || TPP_HAVE_TPP_TOK_DOT_DOT_DOT
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH
#if TPP_HAVE_TPP_TOK_SLASH_SLASH || TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL || TPP_HAVE_TPP_TOK_SLASH_EQUAL
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON
#if TPP_HAVE_TPP_TOK_COLON_COLON || TPP_HAVE_TPP_TOK_COLON_EQUAL
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_EQUAL || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE || TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM || TPP_HAVE_TPP_TOK_EQUAL_PERCENT || TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT || TPP_HAVE_TPP_TOK_EQUAL_AMP || TPP_HAVE_TPP_TOK_EQUAL_STAR || TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR || TPP_HAVE_TPP_TOK_EQUAL_PLUS || TPP_HAVE_TPP_TOK_EQUAL_MINUS || TPP_HAVE_TPP_TOK_EQUAL_SLASH || TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH || TPP_HAVE_TPP_TOK_EQUAL_COLON || TPP_HAVE_TPP_TOK_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE || TPP_HAVE_TPP_TOK_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM || TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL || TPP_HAVE_TPP_TOK_EQUAL_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_EQUAL_QMARK || TPP_HAVE_TPP_TOK_EQUAL_AT || TPP_HAVE_TPP_TOK_EQUAL_AT_AT || TPP_HAVE_TPP_TOK_EQUAL_HAT || TPP_HAVE_TPP_TOK_EQUAL_PIPE || TPP_HAVE_TPP_TOK_EQUAL_TILDE
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE || TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS || TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL || TPP_HAVE_TPP_TOK_QMARK_QMARK
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT
#if TPP_HAVE_TPP_TOK_AT_EQUAL || TPP_HAVE_TPP_TOK_AT_AT || TPP_HAVE_TPP_TOK_AT_AT_EQUAL
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT
#if TPP_HAVE_TPP_TOK_HAT_EQUAL || TPP_HAVE_TPP_TOK_HAT_HAT
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL || TPP_HAVE_TPP_TOK_PIPE_PIPE
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL || TPP_HAVE_TPP_TOK_TILDE_TILDE
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_MC
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM || TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND || TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT || TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP || TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR || TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS || TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS || TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT || TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH || TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON || TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE || TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL || TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE || TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK || TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT || TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT || TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE || TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE
#define TPP_HAVE_TPP_TOK_MC 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_MC 0
#endif /* !... */
/*[[[end]]]*/

/************************************************************************/
/************************************************************************/
/************************************************************************/





/************************************************************************/
/* IMPLICIT API FEATURES (PART 1)                                       */
/************************************************************************/
/* Provide a function `tpp_lexer_open_include_string()`
 * to open the file associated with an `#include`-string. */
#ifndef TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
#define TPP_HAVE_LEXER_OPEN_INCLUDE_STRING \
	(TPP_HAVE_CPP_INCLUDE ||               \
	 TPP_HAVE_CPP_INCLUDE_NEXT ||          \
	 TPP_HAVE_CPP_IMPORT ||                \
	 TPP_HAVE_CPP_EMBED ||                 \
	 TPP_HAVE_MACRO___has_include ||       \
	 TPP_HAVE_MACRO___has_include_next ||  \
	 TPP_HAVE_MACRO___has_embed ||         \
	 TPP_HAVE_MACRO___TPP_LOAD_FILE ||     \
	 TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
#endif /* !TPP_HAVE_LEXER_OPEN_INCLUDE_STRING */

/* Provide a function `tpp_lexer_decode_include_string()`
 * to decode the actual contents of an `#include`-string. */
#ifndef TPP_HAVE_LEXER_DECODE_INCLUDE_STRING
#define TPP_HAVE_LEXER_DECODE_INCLUDE_STRING TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
#endif /* !TPP_HAVE_LEXER_DECODE_INCLUDE_STRING */

/* Provide a function `tpp_lexer_yield_include_string()` to
 * do yield the next token with special handling if the next
 * token's first character is `<` or `"`in which case the
 * token is parsed as a #include-string */
#ifndef TPP_HAVE_LEXER_YIELD_INCLUDE_STRING
#define TPP_HAVE_LEXER_YIELD_INCLUDE_STRING TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
#endif /* !TPP_HAVE_LEXER_YIELD_INCLUDE_STRING */

/* Enable support for `tpp_lexer_openfile()` */
#ifndef TPP_HAVE_LEXER_OPENFILE
#if (TPP_HAVE_LEXER_OPEN_INCLUDE_STRING || \
     TPP_HAVE_CPP_INCLUDE ||               \
     TPP_HAVE_CPP_INCLUDE_NEXT ||          \
     TPP_HAVE_CPP_IMPORT ||                \
     TPP_HAVE_CPP_EMBED ||                 \
     1) /* Always enable by default */
#define TPP_HAVE_LEXER_OPENFILE 1
#else /* ... */
#define TPP_HAVE_LEXER_OPENFILE 0
#endif /* !... */
#endif /* !TPP_HAVE_LEXER_OPENFILE */

/* Enable support for `tpp_lexer_openfile_ex()` */
#ifndef TPP_HAVE_LEXER_OPENFILE_EX
#if (TPP_HAVE_LEXER_OPENFILE &&                                           \
     (TPP_HAVE_CPP_IMPORT ||                                              \
      (TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next) || \
      (TPP_HAVE_CPP_INCLUDE && TPP_HAVE_PRAGMA_ONCE)))
#define TPP_HAVE_LEXER_OPENFILE_EX 1
#else /* ... */
#define TPP_HAVE_LEXER_OPENFILE_EX 0
#endif /* !... */
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */

/* Provide a function `tpp_file_getrealfilenamekwd()` */
#ifndef TPP_HAVE_FILE_GETREALFILENAMEKWD
#define TPP_HAVE_FILE_GETREALFILENAMEKWD (TPP_HAVE_PRAGMA_ONCE)
#endif /* !TPP_HAVE_FILE_GETREALFILENAMEKWD */

/* Enable support for detecting `#ifndef`-style `#include`-guards
 * Has no effect on semantics/behavior, but tends to speed up
 * processing of files with many (repeated) includes:
 *
 * `foo.h`:
 * ```c
 * #ifndef FOO_H
 * #define FOO_H
 * ...
 * #endif // !FOO_H
 * ```
 *
 * `bar.h`:
 * ```c
 * #include "foo.h"
 * ```
 *
 * `bar.c`:
 * ```c
 * #include "bar.h" // Already includes "foo.h"
 * #include "foo.h" // This one's a no-op (won't even try to open("foo.h"))
 * #undef FOO_H
 * #include "foo.h" // This one once again opens "foo.h", behaving as expected
 * ```
 */
#ifndef TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#if ((TPP_HAVE_CPP_IMPORT ||        \
      TPP_HAVE_CPP_INCLUDE ||       \
      TPP_HAVE_CPP_INCLUDE_NEXT) && \
     TPP_HAVE_CPP_IF_ELSE_ENDIF &&  \
     TPP_HAVE_PROFILE_NOT_MINIMAL)
#define TPP_HAVE_IFNDEF_INCLUDE_GUARDS 1
#else /* ... */
#define TPP_HAVE_IFNDEF_INCLUDE_GUARDS 0
#endif /* !... */
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */

/* Enable support for TPP generating new `tpp_keyword` definitions
 * on-the-fly, as keywords are parsed (the first time any unique
 * keyword is parsed, `tpp_keywords_newkeyword()` is used to give
 * it a unique ID and `tpp_keyword` structure). This is also needed
 * for macros and a number of other features that need to store some
 * kind of state alongside keywords. */
#ifndef TPP_HAVE_USER_KEYWORDS
#if (TPP_HAVE_PROFILE_NOT_MINIMAL ||                  \
     TPP_HAVE_FILE_GETREALFILENAMEKWD ||              \
     TPP_HAVE_CPP_MACROS ||                           \
     TPP_HAVE_PRAGMA_ONCE ||                          \
     TPP_HAVE_CPP_IMPORT ||                           \
     TPP_HAVE_CLANG_MACRO___has_attribute ||          \
     TPP_HAVE_CLANG_MACRO___has_builtin ||            \
     TPP_HAVE_CLANG_MACRO___has_cpp_attribute ||      \
     TPP_HAVE_CLANG_MACRO___has_declspec_attribute || \
     TPP_HAVE_CLANG_MACRO___has_extension ||          \
     TPP_HAVE_CLANG_MACRO___has_feature ||            \
     TPP_HAVE_CLANG_MACRO___has_c_attribute ||        \
     TPP_HAVE_MACRO___is_deprecated ||                \
     TPP_HAVE_MACRO___is_poisoned ||                  \
     TPP_HAVE_PRAGMA_DEPRECATED ||                    \
     TPP_HAVE_PRAGMA_GCC_POISON ||                    \
     TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS ||         \
     TPP_HAVE_IFNDEF_INCLUDE_GUARDS ||                \
     TPP_HAVE_KEYWORD_INCLCOUNT ||                    \
     TPP_HAVE_PRAGMA_PUSH_MACRO ||                    \
     TPP_HAVE_MACRO___TPP_COUNTER ||                  \
     TPP_HAVE_KEYWORD_USERDATA)
#define TPP_HAVE_USER_KEYWORDS 1
#else /* ... */
#define TPP_HAVE_USER_KEYWORDS 0
#endif /* !... */
#endif /* !TPP_HAVE_USER_KEYWORDS */
/************************************************************************/
/************************************************************************/
/************************************************************************/






/************************************************************************/
/* LEXER HOOK CONFIGURATION                                             */
/************************************************************************/

/* Possible values for "TPP_HAVE_*_HOOK" feature macros */
#define TPP_HOOK_DISABLED      0    /* Hook is hard-disabled (same as a no-op implementation) */
#define TPP_HOOK_CONST_USER    1    /* Hook is hard-coded to call a user-supplied implementation "TPP_HOOK_FOO" (or no-op if "TPP_HOOK_FOO" isn't defined) */
#define TPP_HOOK_CONST_BUILTIN 2    /* Hook is hard-coded to call a builtin implementation (same as TPP_HOOK_DISABLED if there is no builtin) */
#define TPP_HOOK_RT_USER       (-1) /* Hook is per-lexer configurable; defaults to user-supplied implementation "TPP_HOOK_FOO" (or no-op if "TPP_HOOK_FOO" isn't defined) */
#define TPP_HOOK_RT_BUILTIN    (-2) /* Hook is per-lexer configurable; defaults to builtin implementation (or no-op if there is no builtin) */
#define TPP_HOOK_RT_NOOP       (-3) /* Hook is per-lexer configurable; defaults to no-op implementation */

#define TPP_HOOK_USESBUILTIN(x) ((x) == TPP_HOOK_CONST_BUILTIN || (x) == TPP_HOOK_RT_BUILTIN)
#define TPP_HOOK_USESUSER(x)    ((x) == TPP_HOOK_CONST_USER || (x) == TPP_HOOK_RT_USER)
#define TPP_HOOK_ISCONST(x)     ((x) > 0)
#define TPP_HOOK_ISRT(x)        ((x) < 0)

/* Default configuration specifying how required hooks should be linked. */
#ifndef TPP_HOOK_DEFAULT_BUILTIN
#define TPP_HOOK_DEFAULT_BUILTIN ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_RT_BUILTIN : TPP_HOOK_CONST_BUILTIN)
#endif /* !TPP_HOOK_DEFAULT_BUILTIN */
#ifndef TPP_HOOK_DEFAULT_USER
#define TPP_HOOK_DEFAULT_USER ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_RT_USER : TPP_HOOK_CONST_USER)
#endif /* !TPP_HOOK_DEFAULT_USER */
#ifndef TPP_HOOK_DEFAULT_NOOP
#define TPP_HOOK_DEFAULT_NOOP ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_RT_NOOP : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_DEFAULT_NOOP */


/*[[[deemon
import unPointerizeHook from ".config";

@@Defined callbacks:
@@for (local doc, name,           // Doc string and name of hook
@@     default_TPP_HAVE_FOO_HOOK, // Default value for "TPP_HAVE_FOO_HOOK"
@@     builtin_FOO_HOOK,          // Name of builtin to use when "TPP_HOOK_USESBUILTIN". If there is none, leave empty
@@     prototypePrefix,           // Prefix for function-pointer-prototype
@@     prototypeSuffix,           // Suffix for function-pointer-prototype
@@     prototypeArgs,             // names of arguments taken by hook
@@     disabled_RETURN_VALUE:     // Return value of hook when configured as "TPP_HOOK_DISABLED"
@@     HOOKS) {
@@    ...
@@}
local HOOKS = {
	{
		"Called by `tpp_lexer_warnf()` to print warning messages\n" +
		"Potentially unused if `TPP_HAVE_WARNHANDLER_HOOK` is also overwritten\n" +
		"@param: arg: The current lexer (`tpp_lexer *`)",
		"WARNPRINTER",
		"TPP_HAVE_WARNINGS",
		"_tpp_lexer_builtin_warn_or_mesg_printer",
		"tpp_formatprinter ", "", { "lexer", "text", "num_bytes" },
		"0"
	},

	{
		"Called by `tpp_lexer_warnf()` to handle warning notifications. Can be\n" +
		"overwritten to implement custom behavior in regards to handling of warnings.\n" +
		"@param: info:       Warning context location\n" +
		"@param: invokeinfo: Warning invocation method\n" +
		"@param: id:         Warning ID\n" +
		"@param: arg:        Variable arguments passed to warning\n" +
		"@return: TPP_EOK:        Success (warning was emitted)\n" +
		"@return: TPP_EWARNPRINT: Error during invocation of `TPP_HOOK_WARNPRINTER`\n" +
		"@return: TPP_ENOMEM:     A `TPP_WARNING_EX` returned with this error\n" +
		"@return: TPP_EIO:        A `TPP_WARNING_EX` returned with this error\n" +
		"@return: TPP_ELEXERROR:  A `TPP_WARNING_EX` returned with this error",
		"WARNHANDLER",
		"TPP_HAVE_WARNINGS",
		"_tpp_lexer_builtin_warnhandler",
		"tpp_errno (TPPCALL *", ")(tpp_lexer *tpp_restrict self, tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args)", { "lexer", "info", "invokeinfo", "id", "args" },
		"TPP_EOK"
	},

	{
		"Used by `#pragma message` to print messages\n" +
		"@param: arg: The current lexer (`tpp_lexer *`)",
		"MESGPRINTER",
		"TPP_HAVE_PRAGMA_MESSAGE",
		"_tpp_lexer_builtin_warn_or_mesg_printer",
		"tpp_formatprinter ", "", { "lexer", "text", "num_bytes" },
		"0"
	},

	{
		"User-defined callback for parsing `#if`-style expressions\n" +
		"- This callback is invoked in a context where `self` points\n" +
		"  before the expression's first token (meaning that this\n" +
		"  callback is responsible to do the initial yield using\n" +
		"  whatever method it wants to use).\n" +
		"- When it is known that the expression has finite length,\n" +
		"  as in: it has to end before EOF, or at the next unmatched\n" +
		"  `)`-token, the caller will have configured the lexer's\n" +
		"  current EOF accordingly (and disabled file-popping)\n" +
		"- When this function returns an error, the caller will rewind\n" +
		"  back to the start of the expression (or even further, if\n" +
		"  applicable; meaning this callback doesn't need to concern\n" +
		"  itself with rollback)\n" +
		"\n" +
		"@return: TPP_EOK:         Success (`*result` was initialized)\n" +
		"@return: TPP_ENOMEM:      Out of memory\n" +
		"@return: TPP_EIO:         Filesystem I/O operation failed\n" +
		"@return: TPP_EWOULDBLOCK: Operation would block\n" +
		"@return: TPP_ELEXERROR:   A lexer error happened\n" +
		"@return: TPP_EWARNPRINT:  Error while printing a warning",
		"PARSEEXPR",
		"(TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_MACRO___TPP_EVAL || TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed)",
		"_tpp_lexer_builtin_parseexpr",
		"tpp_errno (TPPCALL *", ")(tpp_lexer *tpp_restrict self, tpp_expr_value *tpp_restrict result)", { "lexer", "result" },
		"tpp_expr_value_init_zero(result)"
	},

	{
		"Called whenever a `#pragma` is encountered that is not recognized.\n" +
		"When called, the lexer is set-up to point at the first token after the `#pragma`.\n" +
		"@return: TPP_EOK:    Pragma has been handled\n" +
		"@return: TPP_ENOENT: Pragma is still unknown, and a warning should be emitted\n" +
		"@return: TPP_EIO:    I/O error\n" +
		"@return: TPP_ENOMEM: Out of memory",
		"UNKNOWN_PRAGMA",
		"(TPP_HAVE_PRAGMA && TPP_PROFILE == TPP_PROFILE_ALL)",
		"", // No builtin default
		"tpp_errno (TPPCALL *", ")(tpp_lexer *tpp_restrict self)", { "lexer" },
		"TPP_ENOENT"
	},

	{
		"Called whenever some file is `#include`-ed for the first time\n" +
		"@param: filename_kwd: Then `tpp_keyword` used to describe the file's name. The actual\n" +
		"                      filename can be queried as `tpp_keyword_getcstr(filename_kwd)`.",
		"NEW_DEPENDENCY",
		"(TPP_HAVE_LEXER_OPENFILE && TPP_HAVE_USER_KEYWORDS && TPP_PROFILE == TPP_PROFILE_ALL)",
		"", // No builtin default
		"tpp_errno (TPPCALL *", ")(tpp_lexer *tpp_restrict self, tpp_keyword *filename_kwd)", { "lexer", "filename_kwd" },
		"TPP_EOK"
	},

	{
		"Called to handle `#ident` and `#sccs` directives\n" +
		"@param: mode:        Either `TPP_KWD_ident` or `TPP_KWD_sccs`\n" +
		"@param: chunk:       If non-NULL a string that must be `tpp_string_incref()`d\n" +
		"                     if you want to keep `comment_str` alive. If NULL, then the\n" +
		"                     given `comment_str` is statically allocated and doesn't need\n" +
		"                     any chunk to stay alive\n" +
		"@param: comment_str: The source comment that should be inserted\n" +
		"@param: comment_len: Length of `comment_str` in bytes\n" +
		"@return: TPP_EOK:    Success\n" +
		"@return: TPP_EIO:    I/O error\n" +
		"@return: TPP_ENOMEM: Out of memory",
		"IDENT_SCCS",
		"TPP_HAVE_CPP_IDENT_SCCS",
		"", // No builtin default
		"tpp_errno (TPPCALL *", ")(tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len)", { "lexer", "mode", "chunk", "comment_str", "comment_len" },
		"TPP_EOK"
	},

	{
		"Extra callback invoked by `tpp_lexer_foreach_include_path()` at diffrent\n" +
		"points during the process of enumerating include paths. This callback is\n" +
		"then allowed to enumerate some additional include paths that may exist, but\n" +
		"for one reason or another (mainly: speed) aren't known to TPP via its system\n" +
		"include path APIs (`tpp_lexer_includes_add*`)\n" +
		"@param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`: describes the\n" +
		"              caller's position in `tpp_lexer_foreach_include_path()`.\n" +
		"@return: * :         First non-TPP_ENOENT return value of `cb`\n" +
		"@return: TPP_ENOENT: File still not found\n" +
		"@return: TPP_EIO:    I/O error\n" +
		"@return: TPP_ENOMEM: Out of memory",
		"SYSTEM_INCLUDE_PATH",
		"(TPP_HAVE_LEXER_OPEN_INCLUDE_STRING && TPP_PROFILE == TPP_PROFILE_ALL)",
		"", // No builtin default
		"tpp_errno (TPPCALL *", ")(tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg)", { "lexer", "mode", "when", "cb", "arg" },
		"TPP_ENOENT"
	},

	{
		"Called by `tpp_lexer_decodestring()` when an unknown `\\`-escape sequence is encountered\n" +
		"This hook can be used to define additional, user-defined escape sequences, or any other\n" +
		"arbitrary behavior to-be performed when specific escape-sequences are found.\n" +
		"On entry, `*p_pos` points at the first (unrecognized) character after the leading `\\`, and\n" +
		"if the hook was able to parse said escape sequence, it should update `*p_pos` to point after\n" +
		"it before returning\n" +
		"@param: p_pos: [in]  Pointer to start of unrecognized `\\`-escape sequence\n" +
		"               [out] First character no longer part of `\\`-escape sequence (if recognized)\n" +
		"               [out] Unchanged (if not recognized)\n" +
		"@param: end:   The of containing string sequence\n" +
		"@param: data_printer: Identically-named argument of `tpp_lexer_decodestring()`\n" +
		"@param: utf8_printer: *ditto*\n" +
		"@param: arg:          *ditto*\n" +
		"@return: * :   Sum of positive return values of `data_printer` and `utf8_printer`\n" +
		"@return: < 0:  First negative return value of `data_printer` or `utf8_printer`\n" +
		"@return: TPP_SSIZE_OFERR(TPP_ENOENT): Escape sequence still not recognized\n" +
		"               (please leave `*p_pos` unchanged in this case). The caller will\n" +
		"               proceed by emitting `TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE`",
		"UNKNOWN_STRING_ESCAPE",
		"(TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && TPP_PROFILE == TPP_PROFILE_ALL)",
		"", // No builtin default
		"tpp_ssize (TPPCALL *", ")(tpp_lexer *tpp_restrict self, tpp_char const **p_pos, tpp_char const *end, tpp_formatprinter data_printer, tpp_formatprinter utf8_printer, void *arg)", { "lexer", "p_pos", "end", "data_printer", "utf8_printer", "arg" },
		"TPP_SSIZE_OFERR(TPP_ENOENT)"
	},

	{
		"Called by `tpp_lexer_warnf()` just before it's about to return `TPP_ELEXERROR`\n" +
		"This hook can be used to do additional state changes that may be necessary by the\n" +
		"hosting application in order to handle the resulting `TPP_ELEXERROR`\n" +
		"@return: TPP_EOK: Have `tpp_lexer_warnf()` still return `TPP_ELEXERROR`\n" +
		"@return: * :      Make `tpp_lexer_warnf()` return this instead of `TPP_ELEXERROR`\n",
		"RAISE_LEXERROR",
		"(TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && TPP_PROFILE == TPP_PROFILE_ALL)",
		"", // No builtin default
		"tpp_errno (TPPCALL *", ")(tpp_lexer *tpp_restrict self)", { "lexer" },
		"TPP_ELEXERROR"
	},
};

for (local doc, name,
     default_TPP_HAVE_FOO_HOOK,
     builtin_FOO_HOOK,
     prototypePrefix,
     prototypeSuffix,
     prototypeArgs,
     disabled_RETURN_VALUE: HOOKS) {
	prototypePrefix, prototypeSuffix = unPointerizeHook(prototypePrefix, prototypeSuffix)...;
	print("/" "* >> ", prototypePrefix, "TPP_HOOK_", name, prototypeSuffix, ";");
	print(" * ", doc.strip().replace("\n", "\n * ").rstriplines(), " *" "/");
	print("#ifndef TPP_HAVE_", name, "_HOOK");
	print("#ifdef TPP_HOOK_", name);
	print("#define TPP_HAVE_", name, "_HOOK (", default_TPP_HAVE_FOO_HOOK, " ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)");
	print("#else /" "* TPP_HOOK_", name, " *" "/");
	print("#define TPP_HAVE_", name, "_HOOK (", default_TPP_HAVE_FOO_HOOK, " ? ",
		builtin_FOO_HOOK ? "TPP_HOOK_DEFAULT_BUILTIN" : "TPP_HOOK_DEFAULT_NOOP", " : TPP_HOOK_DISABLED)");
	print("#endif /" "* !TPP_HOOK_", name, " *" "/");
	print("#endif /" "* !TPP_HAVE_", name, "_HOOK *" "/");
	print("#if TPP_HAVE_", name, "_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_", name, ")");
	print("#if !TPP_IGNORE_INVALID_CONFIGURATION");
	print("#error \"Invalid configuration: 'TPP_HAVE_", name, "_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_", name, "' isn't defined. Configure the hook differently, or supply your definition\"");
	print("#endif /" "* !TPP_IGNORE_INVALID_CONFIGURATION *" "/");
	print("#undef TPP_HAVE_", name, "_HOOK");
	print("#define TPP_HAVE_", name, "_HOOK TPP_HOOK_DISABLED");
	print("#elif TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_", name, ")");
	print("#if !TPP_IGNORE_INVALID_CONFIGURATION");
	print("#error \"Invalid configuration: 'TPP_HAVE_", name, "_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_", name, "' isn't defined. Configure the hook differently, or supply your definition\"");
	print("#endif /" "* !TPP_IGNORE_INVALID_CONFIGURATION *" "/");
	print("#undef TPP_HAVE_", name, "_HOOK");
	print("#define TPP_HAVE_", name, "_HOOK TPP_HOOK_RT_NOOP");
	if (!builtin_FOO_HOOK) {
		print("#elif TPP_HAVE_", name, "_HOOK == TPP_HOOK_CONST_BUILTIN");
		print("#undef TPP_HAVE_", name, "_HOOK /" "* There is no builtin version *" "/");
		print("#define TPP_HAVE_", name, "_HOOK TPP_HOOK_DISABLED");
		print("#elif TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_BUILTIN");
		print("#undef TPP_HAVE_", name, "_HOOK /" "* There is no builtin version *" "/");
		print("#define TPP_HAVE_", name, "_HOOK TPP_HOOK_RT_NOOP");
	}
	print("#endif /" "* ... *" "/");
	print("#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_", name, ") && !TPP_HOOK_USESUSER(TPP_HAVE_", name, "_HOOK)");
	print("#error \"Invalid configuration: 'TPP_HOOK_", name, "' is defined, but 'TPP_HAVE_", name, "_HOOK' isn't using it\"");
	print("#endif /" "* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_", name, " && !TPP_HOOK_USESUSER(TPP_HAVE_", name, "_HOOK) *" "/");
	if (builtin_FOO_HOOK) {
		print("#ifndef TPP_HAVE_BUILTIN_", name, "_HOOK");
		print("#define TPP_HAVE_BUILTIN_", name, "_HOOK TPP_HOOK_USESBUILTIN(TPP_HAVE_", name, "_HOOK)");
		print("#endif /" "* !TPP_HAVE_BUILTIN_", name, "_HOOK *" "/");
	}
	print;
}
]]]*/
/* >> TPP_FORMATPRINTER_DEFINE(TPP_HOOK_WARNPRINTER, arg, text, num_bytes);
 * Called by `tpp_lexer_warnf()` to print warning messages
 * Potentially unused if `TPP_HAVE_WARNHANDLER_HOOK` is also overwritten
 * @param: arg: The current lexer (`tpp_lexer *`) */
#ifndef TPP_HAVE_WARNPRINTER_HOOK
#ifdef TPP_HOOK_WARNPRINTER
#define TPP_HAVE_WARNPRINTER_HOOK (TPP_HAVE_WARNINGS ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_WARNPRINTER */
#define TPP_HAVE_WARNPRINTER_HOOK (TPP_HAVE_WARNINGS ? TPP_HOOK_DEFAULT_BUILTIN : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_WARNPRINTER */
#endif /* !TPP_HAVE_WARNPRINTER_HOOK */
#if TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_WARNPRINTER)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_WARNPRINTER_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_WARNPRINTER' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_WARNPRINTER_HOOK
#define TPP_HAVE_WARNPRINTER_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_WARNPRINTER)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_WARNPRINTER_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_WARNPRINTER' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_WARNPRINTER_HOOK
#define TPP_HAVE_WARNPRINTER_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_WARNPRINTER) && !TPP_HOOK_USESUSER(TPP_HAVE_WARNPRINTER_HOOK)
#error "Invalid configuration: 'TPP_HOOK_WARNPRINTER' is defined, but 'TPP_HAVE_WARNPRINTER_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_WARNPRINTER && !TPP_HOOK_USESUSER(TPP_HAVE_WARNPRINTER_HOOK) */
#ifndef TPP_HAVE_BUILTIN_WARNPRINTER_HOOK
#define TPP_HAVE_BUILTIN_WARNPRINTER_HOOK TPP_HOOK_USESBUILTIN(TPP_HAVE_WARNPRINTER_HOOK)
#endif /* !TPP_HAVE_BUILTIN_WARNPRINTER_HOOK */

/* >> tpp_errno TPP_HOOK_WARNHANDLER(tpp_lexer *tpp_restrict self, tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args);
 * Called by `tpp_lexer_warnf()` to handle warning notifications. Can be
 * overwritten to implement custom behavior in regards to handling of warnings.
 * @param: info:       Warning context location
 * @param: invokeinfo: Warning invocation method
 * @param: id:         Warning ID
 * @param: arg:        Variable arguments passed to warning
 * @return: TPP_EOK:        Success (warning was emitted)
 * @return: TPP_EWARNPRINT: Error during invocation of `TPP_HOOK_WARNPRINTER`
 * @return: TPP_ENOMEM:     A `TPP_WARNING_EX` returned with this error
 * @return: TPP_EIO:        A `TPP_WARNING_EX` returned with this error
 * @return: TPP_ELEXERROR:  A `TPP_WARNING_EX` returned with this error */
#ifndef TPP_HAVE_WARNHANDLER_HOOK
#ifdef TPP_HOOK_WARNHANDLER
#define TPP_HAVE_WARNHANDLER_HOOK (TPP_HAVE_WARNINGS ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_WARNHANDLER */
#define TPP_HAVE_WARNHANDLER_HOOK (TPP_HAVE_WARNINGS ? TPP_HOOK_DEFAULT_BUILTIN : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_WARNHANDLER */
#endif /* !TPP_HAVE_WARNHANDLER_HOOK */
#if TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_WARNHANDLER)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_WARNHANDLER_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_WARNHANDLER' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_WARNHANDLER_HOOK
#define TPP_HAVE_WARNHANDLER_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_WARNHANDLER)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_WARNHANDLER_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_WARNHANDLER' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_WARNHANDLER_HOOK
#define TPP_HAVE_WARNHANDLER_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_WARNHANDLER) && !TPP_HOOK_USESUSER(TPP_HAVE_WARNHANDLER_HOOK)
#error "Invalid configuration: 'TPP_HOOK_WARNHANDLER' is defined, but 'TPP_HAVE_WARNHANDLER_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_WARNHANDLER && !TPP_HOOK_USESUSER(TPP_HAVE_WARNHANDLER_HOOK) */
#ifndef TPP_HAVE_BUILTIN_WARNHANDLER_HOOK
#define TPP_HAVE_BUILTIN_WARNHANDLER_HOOK TPP_HOOK_USESBUILTIN(TPP_HAVE_WARNHANDLER_HOOK)
#endif /* !TPP_HAVE_BUILTIN_WARNHANDLER_HOOK */

/* >> TPP_FORMATPRINTER_DEFINE(TPP_HOOK_MESGPRINTER, arg, text, num_bytes);
 * Used by `#pragma message` to print messages
 * @param: arg: The current lexer (`tpp_lexer *`) */
#ifndef TPP_HAVE_MESGPRINTER_HOOK
#ifdef TPP_HOOK_MESGPRINTER
#define TPP_HAVE_MESGPRINTER_HOOK (TPP_HAVE_PRAGMA_MESSAGE ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_MESGPRINTER */
#define TPP_HAVE_MESGPRINTER_HOOK (TPP_HAVE_PRAGMA_MESSAGE ? TPP_HOOK_DEFAULT_BUILTIN : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_MESGPRINTER */
#endif /* !TPP_HAVE_MESGPRINTER_HOOK */
#if TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_MESGPRINTER)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_MESGPRINTER_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_MESGPRINTER' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_MESGPRINTER_HOOK
#define TPP_HAVE_MESGPRINTER_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_MESGPRINTER)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_MESGPRINTER_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_MESGPRINTER' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_MESGPRINTER_HOOK
#define TPP_HAVE_MESGPRINTER_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_MESGPRINTER) && !TPP_HOOK_USESUSER(TPP_HAVE_MESGPRINTER_HOOK)
#error "Invalid configuration: 'TPP_HOOK_MESGPRINTER' is defined, but 'TPP_HAVE_MESGPRINTER_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_MESGPRINTER && !TPP_HOOK_USESUSER(TPP_HAVE_MESGPRINTER_HOOK) */
#ifndef TPP_HAVE_BUILTIN_MESGPRINTER_HOOK
#define TPP_HAVE_BUILTIN_MESGPRINTER_HOOK TPP_HOOK_USESBUILTIN(TPP_HAVE_MESGPRINTER_HOOK)
#endif /* !TPP_HAVE_BUILTIN_MESGPRINTER_HOOK */

/* >> tpp_errno TPP_HOOK_PARSEEXPR(tpp_lexer *tpp_restrict self, tpp_expr_value *tpp_restrict result);
 * User-defined callback for parsing `#if`-style expressions
 * - This callback is invoked in a context where `self` points
 *   before the expression's first token (meaning that this
 *   callback is responsible to do the initial yield using
 *   whatever method it wants to use).
 * - When it is known that the expression has finite length,
 *   as in: it has to end before EOF, or at the next unmatched
 *   `)`-token, the caller will have configured the lexer's
 *   current EOF accordingly (and disabled file-popping)
 * - When this function returns an error, the caller will rewind
 *   back to the start of the expression (or even further, if
 *   applicable; meaning this callback doesn't need to concern
 *   itself with rollback)
 *
 * @return: TPP_EOK:         Success (`*result` was initialized)
 * @return: TPP_ENOMEM:      Out of memory
 * @return: TPP_EIO:         Filesystem I/O operation failed
 * @return: TPP_EWOULDBLOCK: Operation would block
 * @return: TPP_ELEXERROR:   A lexer error happened
 * @return: TPP_EWARNPRINT:  Error while printing a warning */
#ifndef TPP_HAVE_PARSEEXPR_HOOK
#ifdef TPP_HOOK_PARSEEXPR
#define TPP_HAVE_PARSEEXPR_HOOK ((TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_MACRO___TPP_EVAL || TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed) ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_PARSEEXPR */
#define TPP_HAVE_PARSEEXPR_HOOK ((TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_MACRO___TPP_EVAL || TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed) ? TPP_HOOK_DEFAULT_BUILTIN : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_PARSEEXPR */
#endif /* !TPP_HAVE_PARSEEXPR_HOOK */
#if TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_PARSEEXPR)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_PARSEEXPR_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_PARSEEXPR' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_PARSEEXPR_HOOK
#define TPP_HAVE_PARSEEXPR_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_PARSEEXPR)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_PARSEEXPR_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_PARSEEXPR' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_PARSEEXPR_HOOK
#define TPP_HAVE_PARSEEXPR_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_PARSEEXPR) && !TPP_HOOK_USESUSER(TPP_HAVE_PARSEEXPR_HOOK)
#error "Invalid configuration: 'TPP_HOOK_PARSEEXPR' is defined, but 'TPP_HAVE_PARSEEXPR_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_PARSEEXPR && !TPP_HOOK_USESUSER(TPP_HAVE_PARSEEXPR_HOOK) */
#ifndef TPP_HAVE_BUILTIN_PARSEEXPR_HOOK
#define TPP_HAVE_BUILTIN_PARSEEXPR_HOOK TPP_HOOK_USESBUILTIN(TPP_HAVE_PARSEEXPR_HOOK)
#endif /* !TPP_HAVE_BUILTIN_PARSEEXPR_HOOK */

/* >> tpp_errno TPP_HOOK_UNKNOWN_PRAGMA(tpp_lexer *tpp_restrict self);
 * Called whenever a `#pragma` is encountered that is not recognized.
 * When called, the lexer is set-up to point at the first token after the `#pragma`.
 * @return: TPP_EOK:    Pragma has been handled
 * @return: TPP_ENOENT: Pragma is still unknown, and a warning should be emitted
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
#ifndef TPP_HAVE_UNKNOWN_PRAGMA_HOOK
#ifdef TPP_HOOK_UNKNOWN_PRAGMA
#define TPP_HAVE_UNKNOWN_PRAGMA_HOOK ((TPP_HAVE_PRAGMA && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_UNKNOWN_PRAGMA */
#define TPP_HAVE_UNKNOWN_PRAGMA_HOOK ((TPP_HAVE_PRAGMA && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_NOOP : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_UNKNOWN_PRAGMA */
#endif /* !TPP_HAVE_UNKNOWN_PRAGMA_HOOK */
#if TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_UNKNOWN_PRAGMA)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_UNKNOWN_PRAGMA_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_UNKNOWN_PRAGMA' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_UNKNOWN_PRAGMA_HOOK
#define TPP_HAVE_UNKNOWN_PRAGMA_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_UNKNOWN_PRAGMA)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_UNKNOWN_PRAGMA_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_UNKNOWN_PRAGMA' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_UNKNOWN_PRAGMA_HOOK
#define TPP_HAVE_UNKNOWN_PRAGMA_HOOK TPP_HOOK_RT_NOOP
#elif TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_CONST_BUILTIN
#undef TPP_HAVE_UNKNOWN_PRAGMA_HOOK /* There is no builtin version */
#define TPP_HAVE_UNKNOWN_PRAGMA_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_RT_BUILTIN
#undef TPP_HAVE_UNKNOWN_PRAGMA_HOOK /* There is no builtin version */
#define TPP_HAVE_UNKNOWN_PRAGMA_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_UNKNOWN_PRAGMA) && !TPP_HOOK_USESUSER(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
#error "Invalid configuration: 'TPP_HOOK_UNKNOWN_PRAGMA' is defined, but 'TPP_HAVE_UNKNOWN_PRAGMA_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_UNKNOWN_PRAGMA && !TPP_HOOK_USESUSER(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */

/* >> tpp_errno TPP_HOOK_NEW_DEPENDENCY(tpp_lexer *tpp_restrict self, tpp_keyword *filename_kwd);
 * Called whenever some file is `#include`-ed for the first time
 * @param: filename_kwd: Then `tpp_keyword` used to describe the file's name. The actual
 *                       filename can be queried as `tpp_keyword_getcstr(filename_kwd)`. */
#ifndef TPP_HAVE_NEW_DEPENDENCY_HOOK
#ifdef TPP_HOOK_NEW_DEPENDENCY
#define TPP_HAVE_NEW_DEPENDENCY_HOOK ((TPP_HAVE_LEXER_OPENFILE && TPP_HAVE_USER_KEYWORDS && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_NEW_DEPENDENCY */
#define TPP_HAVE_NEW_DEPENDENCY_HOOK ((TPP_HAVE_LEXER_OPENFILE && TPP_HAVE_USER_KEYWORDS && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_NOOP : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_NEW_DEPENDENCY */
#endif /* !TPP_HAVE_NEW_DEPENDENCY_HOOK */
#if TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_NEW_DEPENDENCY)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_NEW_DEPENDENCY_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_NEW_DEPENDENCY' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_NEW_DEPENDENCY_HOOK
#define TPP_HAVE_NEW_DEPENDENCY_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_NEW_DEPENDENCY)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_NEW_DEPENDENCY_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_NEW_DEPENDENCY' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_NEW_DEPENDENCY_HOOK
#define TPP_HAVE_NEW_DEPENDENCY_HOOK TPP_HOOK_RT_NOOP
#elif TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_CONST_BUILTIN
#undef TPP_HAVE_NEW_DEPENDENCY_HOOK /* There is no builtin version */
#define TPP_HAVE_NEW_DEPENDENCY_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_RT_BUILTIN
#undef TPP_HAVE_NEW_DEPENDENCY_HOOK /* There is no builtin version */
#define TPP_HAVE_NEW_DEPENDENCY_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_NEW_DEPENDENCY) && !TPP_HOOK_USESUSER(TPP_HAVE_NEW_DEPENDENCY_HOOK)
#error "Invalid configuration: 'TPP_HOOK_NEW_DEPENDENCY' is defined, but 'TPP_HAVE_NEW_DEPENDENCY_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_NEW_DEPENDENCY && !TPP_HOOK_USESUSER(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

/* >> tpp_errno TPP_HOOK_IDENT_SCCS(tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len);
 * Called to handle `#ident` and `#sccs` directives
 * @param: mode:        Either `TPP_KWD_ident` or `TPP_KWD_sccs`
 * @param: chunk:       If non-NULL a string that must be `tpp_string_incref()`d
 *                      if you want to keep `comment_str` alive. If NULL, then the
 *                      given `comment_str` is statically allocated and doesn't need
 *                      any chunk to stay alive
 * @param: comment_str: The source comment that should be inserted
 * @param: comment_len: Length of `comment_str` in bytes
 * @return: TPP_EOK:    Success
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
#ifndef TPP_HAVE_IDENT_SCCS_HOOK
#ifdef TPP_HOOK_IDENT_SCCS
#define TPP_HAVE_IDENT_SCCS_HOOK (TPP_HAVE_CPP_IDENT_SCCS ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_IDENT_SCCS */
#define TPP_HAVE_IDENT_SCCS_HOOK (TPP_HAVE_CPP_IDENT_SCCS ? TPP_HOOK_DEFAULT_NOOP : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_IDENT_SCCS */
#endif /* !TPP_HAVE_IDENT_SCCS_HOOK */
#if TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_IDENT_SCCS)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_IDENT_SCCS_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_IDENT_SCCS' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_IDENT_SCCS_HOOK
#define TPP_HAVE_IDENT_SCCS_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_IDENT_SCCS)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_IDENT_SCCS_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_IDENT_SCCS' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_IDENT_SCCS_HOOK
#define TPP_HAVE_IDENT_SCCS_HOOK TPP_HOOK_RT_NOOP
#elif TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_CONST_BUILTIN
#undef TPP_HAVE_IDENT_SCCS_HOOK /* There is no builtin version */
#define TPP_HAVE_IDENT_SCCS_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_RT_BUILTIN
#undef TPP_HAVE_IDENT_SCCS_HOOK /* There is no builtin version */
#define TPP_HAVE_IDENT_SCCS_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_IDENT_SCCS) && !TPP_HOOK_USESUSER(TPP_HAVE_IDENT_SCCS_HOOK)
#error "Invalid configuration: 'TPP_HOOK_IDENT_SCCS' is defined, but 'TPP_HAVE_IDENT_SCCS_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_IDENT_SCCS && !TPP_HOOK_USESUSER(TPP_HAVE_IDENT_SCCS_HOOK) */

/* >> tpp_errno TPP_HOOK_SYSTEM_INCLUDE_PATH(tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg);
 * Extra callback invoked by `tpp_lexer_foreach_include_path()` at diffrent
 * points during the process of enumerating include paths. This callback is
 * then allowed to enumerate some additional include paths that may exist, but
 * for one reason or another (mainly: speed) aren't known to TPP via its system
 * include path APIs (`tpp_lexer_includes_add*`)
 * @param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`: describes the
 *               caller's position in `tpp_lexer_foreach_include_path()`.
 * @return: * :         First non-TPP_ENOENT return value of `cb`
 * @return: TPP_ENOENT: File still not found
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
#ifndef TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK
#ifdef TPP_HOOK_SYSTEM_INCLUDE_PATH
#define TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK ((TPP_HAVE_LEXER_OPEN_INCLUDE_STRING && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_SYSTEM_INCLUDE_PATH */
#define TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK ((TPP_HAVE_LEXER_OPEN_INCLUDE_STRING && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_NOOP : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_SYSTEM_INCLUDE_PATH */
#endif /* !TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK */
#if TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_SYSTEM_INCLUDE_PATH)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_SYSTEM_INCLUDE_PATH' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK
#define TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_SYSTEM_INCLUDE_PATH)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_SYSTEM_INCLUDE_PATH' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK
#define TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK TPP_HOOK_RT_NOOP
#elif TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_CONST_BUILTIN
#undef TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK /* There is no builtin version */
#define TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_RT_BUILTIN
#undef TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK /* There is no builtin version */
#define TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_SYSTEM_INCLUDE_PATH) && !TPP_HOOK_USESUSER(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK)
#error "Invalid configuration: 'TPP_HOOK_SYSTEM_INCLUDE_PATH' is defined, but 'TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_SYSTEM_INCLUDE_PATH && !TPP_HOOK_USESUSER(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */

/* >> tpp_ssize TPP_HOOK_UNKNOWN_STRING_ESCAPE(tpp_lexer *tpp_restrict self, tpp_char const **p_pos, tpp_char const *end, tpp_formatprinter data_printer, tpp_formatprinter utf8_printer, void *arg);
 * Called by `tpp_lexer_decodestring()` when an unknown `\`-escape sequence is encountered
 * This hook can be used to define additional, user-defined escape sequences, or any other
 * arbitrary behavior to-be performed when specific escape-sequences are found.
 * On entry, `*p_pos` points at the first (unrecognized) character after the leading `\`, and
 * if the hook was able to parse said escape sequence, it should update `*p_pos` to point after
 * it before returning
 * @param: p_pos: [in]  Pointer to start of unrecognized `\`-escape sequence
 *                [out] First character no longer part of `\`-escape sequence (if recognized)
 *                [out] Unchanged (if not recognized)
 * @param: end:   The of containing string sequence
 * @param: data_printer: Identically-named argument of `tpp_lexer_decodestring()`
 * @param: utf8_printer: *ditto*
 * @param: arg:          *ditto*
 * @return: * :   Sum of positive return values of `data_printer` and `utf8_printer`
 * @return: < 0:  First negative return value of `data_printer` or `utf8_printer`
 * @return: TPP_SSIZE_OFERR(TPP_ENOENT): Escape sequence still not recognized
 *                (please leave `*p_pos` unchanged in this case). The caller will
 *                proceed by emitting `TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE` */
#ifndef TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK
#ifdef TPP_HOOK_UNKNOWN_STRING_ESCAPE
#define TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK ((TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_UNKNOWN_STRING_ESCAPE */
#define TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK ((TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_NOOP : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_UNKNOWN_STRING_ESCAPE */
#endif /* !TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK */
#if TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_UNKNOWN_STRING_ESCAPE)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_UNKNOWN_STRING_ESCAPE' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK
#define TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_UNKNOWN_STRING_ESCAPE)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_UNKNOWN_STRING_ESCAPE' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK
#define TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK TPP_HOOK_RT_NOOP
#elif TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_CONST_BUILTIN
#undef TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK /* There is no builtin version */
#define TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_RT_BUILTIN
#undef TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK /* There is no builtin version */
#define TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_UNKNOWN_STRING_ESCAPE) && !TPP_HOOK_USESUSER(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK)
#error "Invalid configuration: 'TPP_HOOK_UNKNOWN_STRING_ESCAPE' is defined, but 'TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_UNKNOWN_STRING_ESCAPE && !TPP_HOOK_USESUSER(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */

/* >> tpp_errno TPP_HOOK_RAISE_LEXERROR(tpp_lexer *tpp_restrict self);
 * Called by `tpp_lexer_warnf()` just before it's about to return `TPP_ELEXERROR`
 * This hook can be used to do additional state changes that may be necessary by the
 * hosting application in order to handle the resulting `TPP_ELEXERROR`
 * @return: TPP_EOK: Have `tpp_lexer_warnf()` still return `TPP_ELEXERROR`
 * @return: * :      Make `tpp_lexer_warnf()` return this instead of `TPP_ELEXERROR` */
#ifndef TPP_HAVE_RAISE_LEXERROR_HOOK
#ifdef TPP_HOOK_RAISE_LEXERROR
#define TPP_HAVE_RAISE_LEXERROR_HOOK ((TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_USER : TPP_HOOK_DISABLED)
#else /* TPP_HOOK_RAISE_LEXERROR */
#define TPP_HAVE_RAISE_LEXERROR_HOOK ((TPP_HAVE_STRING_ESCAPE && TPP_HAVE_LEXER_DECODESTRING && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_HOOK_DEFAULT_NOOP : TPP_HOOK_DISABLED)
#endif /* !TPP_HOOK_RAISE_LEXERROR */
#endif /* !TPP_HAVE_RAISE_LEXERROR_HOOK */
#if TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_CONST_USER && !defined(TPP_HOOK_RAISE_LEXERROR)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_RAISE_LEXERROR_HOOK' is configured as 'TPP_HOOK_CONST_USER', but 'TPP_HOOK_RAISE_LEXERROR' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_RAISE_LEXERROR_HOOK
#define TPP_HAVE_RAISE_LEXERROR_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_RT_USER && !defined(TPP_HOOK_RAISE_LEXERROR)
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_RAISE_LEXERROR_HOOK' is configured as 'TPP_HOOK_RT_USER', but 'TPP_HOOK_RAISE_LEXERROR' isn't defined. Configure the hook differently, or supply your definition"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#undef TPP_HAVE_RAISE_LEXERROR_HOOK
#define TPP_HAVE_RAISE_LEXERROR_HOOK TPP_HOOK_RT_NOOP
#elif TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_CONST_BUILTIN
#undef TPP_HAVE_RAISE_LEXERROR_HOOK /* There is no builtin version */
#define TPP_HAVE_RAISE_LEXERROR_HOOK TPP_HOOK_DISABLED
#elif TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_RT_BUILTIN
#undef TPP_HAVE_RAISE_LEXERROR_HOOK /* There is no builtin version */
#define TPP_HAVE_RAISE_LEXERROR_HOOK TPP_HOOK_RT_NOOP
#endif /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION && defined(TPP_HOOK_RAISE_LEXERROR) && !TPP_HOOK_USESUSER(TPP_HAVE_RAISE_LEXERROR_HOOK)
#error "Invalid configuration: 'TPP_HOOK_RAISE_LEXERROR' is defined, but 'TPP_HAVE_RAISE_LEXERROR_HOOK' isn't using it"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HOOK_RAISE_LEXERROR && !TPP_HOOK_USESUSER(TPP_HAVE_RAISE_LEXERROR_HOOK) */
/*[[[end]]]*/

/************************************************************************/
/************************************************************************/
/************************************************************************/






/************************************************************************/
/* LEXER EXPRESSIONS                                                    */
/************************************************************************/

/* Enable support for `defined(MACRO)` in builtin lexer expressions */
#ifndef TPP_HAVE_BUILTIN_EXPR_DEFINED
#define TPP_HAVE_BUILTIN_EXPR_DEFINED ((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fdefined-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_DEFINED */

/* Enable special handling in `#define foo(x) defined(x)` such that `x` is not expanded.
 * Irregardless of this feature being enabled or not, a warning `TPP_W_EXPANSION_TO_DEFINED`
 * is emitted whenever a construct `defined(<param>)` or `defined <param>` is encountered
 * within the body of a function-style macro definition, where `<param>` is the name of one
 * of the macro's parameters (see `TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED`).
 *
 * @detect: #define test(x) defined(x)
 *          #define test2   NOT_DEFINED
 *          #if test(test2) */
#ifndef TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR
#define TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR ((TPP_HAVE_BUILTIN_EXPR_DEFINED && TPP_HAVE_CPP_MACROS && TPP_HAVE_PROFILE_NOT_MINIMAL) ? TPP_CONF_EXT0 : 0) /* "-fdont-expand-defined" */
#endif /* !TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR */

/* Enable support for strings in builtin lexer expressions, as well as some new operators:
 * ```c
 * #if "FOO" != "BAR"
 * ...
 * #endif
 *
 * #if "FOO"[0] == 'F'
 * ...
 * #endif
 *
 * #if "FOO"[1:] == "OO"
 * ...
 * #endif
 *
 * #if #"FOO" == 3
 * ...
 * #endif
 * ``` */
#ifndef TPP_HAVE_BUILTIN_EXPR_STRINGS
#define TPP_HAVE_BUILTIN_EXPR_STRINGS ((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TPP_TOK_C_STRINGLIKE && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fstrings-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_STRINGS */

/* Enable support for floats in builtin lexer expressions */
#ifndef TPP_HAVE_BUILTIN_EXPR_FLOATS
#define TPP_HAVE_BUILTIN_EXPR_FLOATS ((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TPP_TOK_FLOAT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-ffloats-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_FLOATS */

/* Enable support for `foo ?: bar` in builtin lexer expressions (same as `foo ? foo : bar`) */
#ifndef TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
#define TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT ((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fif-else-optional-true" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT */

/* Enable support for `if (foo) bar else baz` in builtin
 * lexer expressions, as alias for `foo ? bar : baz`. */
#ifndef TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#define TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS ((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 0) /* "-fifelse-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */

/* XXX: Also have an extension for python's `tt if cond else ff`
 *      syntax, though TPP won't be able to guaranty that `tt`
 *      is only evaluated when `cond` evaluates to true! */

/* Enable support for `^^` in builtin lexer expressions */
#ifndef TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR
#define TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR ((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-flogical-xor-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR */

/* Enable support for `0b` literals in builtin lexer expressions */
#ifndef TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS
#define TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS (((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_PROFILE_NOT_MINIMAL) && TPP_HAVE_TPP_TOK_INT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fbinary-literals" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS */

/* Enable support for `0o` literals in builtin lexer expressions */
#ifndef TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS
#define TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS (((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_PROFILE_NOT_MINIMAL) && TPP_HAVE_TPP_TOK_INT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-foctal-literals" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS */

/* Enable support for `u`, `l`, `ul`, `ll`, `ull` integer suffixes */
#ifndef TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX
#define TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX ((TPP_HAVE_TPP_TOK_INT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) : 0) /* "-ffixed-type-integrals" */
#endif /* !TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX */

/* Enable support for `z`, `uz` integer suffixes */
#ifndef TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX
#define TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX ((TPP_HAVE_TPP_TOK_INT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) : 0) /* "-fsize-type-integrals" */
#endif /* !TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX */

/* Enable support for `i8`, `i16`, `i32`, `i64`, `ui8`, `ui16`, `ui32`, `ui64` integer suffixes */
#ifndef TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX
#define TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX ((TPP_HAVE_TPP_TOK_INT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) : 0) /* "-ffixed-length-integrals" */
#endif /* !TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX */

/* Enable support for `f`, `F`, `l`, `L` float suffixes */
#ifndef TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX
#define TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX ((TPP_HAVE_TPP_TOK_FLOAT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) : 0) /* "-ffixed-type-float" */
#endif /* !TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX */

/* Enable support for `d`, `D` float suffixes */
#ifndef TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX
#define TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX ((TPP_HAVE_TPP_TOK_FLOAT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 0) : 0) /* "-fdouble-type-float" */
#endif /* !TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX */

/* Enable support for `df`, `DF`, `dd`, `DD`, `dl`, `DL` float suffixes */
#ifndef TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX
#define TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX ((TPP_HAVE_TPP_TOK_FLOAT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) : 0) /* "-fdecimal-type-float" */
#endif /* !TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX */

/* Treat `'a'` in expressions as an integer, rather than as a string (in C/C++, this is always the case) */
#ifndef TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
#define TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS ((TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TPP_TOK_C_STRINGLIKE_SQUOTE) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_FEAT1 : (TPP_HAVE_PROFILE_C_LIKE ? 1 : 0)) : 0) /* "-fcharacter-literals" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
/************************************************************************/
/************************************************************************/
/************************************************************************/





/************************************************************************/
/* IMPLICIT API FEATURES (PART 2)                                       */
/************************************************************************/

/* Provide an API function `tpp_unicode_writeutf8()` */
#ifndef TPP_HAVE_TPP_UNICODE_WRITEUTF8
#if (TPP_HAVE_TPP_TOK_C_STRING ||                 \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
     TPP_HAVE_TPP_TOK_C_CHAR ||                   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL ||       \
     TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
#define TPP_HAVE_TPP_UNICODE_WRITEUTF8 1
#else /* ... */
#define TPP_HAVE_TPP_UNICODE_WRITEUTF8 0
#endif /* !... */
#endif /* !TPP_HAVE_TPP_UNICODE_WRITEUTF8 */

/* Provide an API for loading the current date/time */
#ifndef TPP_HAVE_TIME_API
#if (TPP_HAVE_MACRO___TIME__ ||      \
     TPP_HAVE_MACRO___DATE__ ||      \
     TPP_HAVE_MACRO___TIMESTAMP__ || \
     TPP_HAVE_NUMERIC_DATE_MACROS || \
     TPP_HAVE_NUMERIC_TIME_MACROS)
#define TPP_HAVE_TIME_API 1
#else /* ... */
#define TPP_HAVE_TIME_API 0
#endif /* !... */
#endif /* !TPP_HAVE_TIME_API */

/* Enable support for `TPP_FILE_FLAGS_SYSHDR` */
#ifndef TPP_HAVE_FILE_SYSHDR
#define TPP_HAVE_FILE_SYSHDR (TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER != 0)
#endif /* !TPP_HAVE_FILE_SYSHDR */

/* Enable support for `TPP_FILE_KIND_SUBTEXT` */
#ifndef TPP_HAVE_FILE_SUBTEXT
#define TPP_HAVE_FILE_SUBTEXT                     \
	(TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_EMBED || \
	 TPP_HAVE_MACRO__Pragma ||                    \
	 TPP_HAVE_MACRO___pragma ||                   \
	 TPP_HAVE_MACRO___TPP_IDENTIFIER ||           \
	 TPP_HAVE_MACRO___TPP_STR_DECOMPILE ||        \
	 TPP_HAVE_MACRO___TPP_STR_PACK ||             \
	 TPP_HAVE_MACRO___TPP_COUNT_TOKENS ||         \
	 TPP_HAVE_PRAGMA_TPP_EXEC ||                  \
	 TPP_HAVE_PRAGMA_TPP_TPP_EXEC)
#endif /* !TPP_HAVE_FILE_SUBTEXT */

/* Enable support for `TPP_FILE_KIND_DUMMY`, which is
 * needed to support gcc's `# <linenum>` -> `1`/`2` flags
 *
 * These flags push so-called "dummy" files onto the
 * #include-stack (without altering the actual current
 * file), with those dummy files acting as additional
 * entries for #include tracebacks. */
#ifndef TPP_HAVE_FILE_DUMMY
#define TPP_HAVE_FILE_DUMMY (TPP_HAVE_CPP_DIGIT_LINE)
#endif /* !TPP_HAVE_FILE_DUMMY */

/* Enable support for `TPP_FILE_FLAGS_EXTERN_C` */
#ifndef TPP_HAVE_FILE_EXTERN_C
#define TPP_HAVE_FILE_EXTERN_C (TPP_PROFILE == TPP_PROFILE_ALL && TPP_HAVE_CPP_DIGIT_LINE)
#endif /* !TPP_HAVE_FILE_EXTERN_C */

/* When `TPP_FILE_FLAGS_SYSHDR` is set during `#include` (i.e. *NOT* via `#pragma GCC system_header`),
 * then the `TPP_FILE_FLAGS_EXTERN_C` flag should be set alongside `TPP_FILE_FLAGS_SYSHDR`. */
#ifndef TPP_HAVE_EXTERN_C_FOR_SYSHDR
#define TPP_HAVE_EXTERN_C_FOR_SYSHDR (TPP_HAVE_FILE_SYSHDR && TPP_HAVE_FILE_EXTERN_C ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT0 : TPP_CONF_FEAT0) : 0) /* "-fextern-c-for-syshdr" */
#endif /* !TPP_HAVE_EXTERN_C_FOR_SYSHDR */

/* Enable API support for having an `#include`-stack. */
#ifndef TPP_HAVE_INCLUDE_STACK
#if (TPP_HAVE_CPP_MACROS ||       \
     TPP_HAVE_CPP_INCLUDE ||      \
     TPP_HAVE_CPP_INCLUDE_NEXT || \
     TPP_HAVE_CPP_IMPORT ||       \
     TPP_HAVE_CPP_EMBED ||        \
     TPP_HAVE_FILE_SUBTEXT ||     \
     TPP_HAVE_FILE_DUMMY)
#define TPP_HAVE_INCLUDE_STACK 1
#else /* ... */
#define TPP_HAVE_INCLUDE_STACK 0
#endif /* !... */
#endif /* !TPP_HAVE_INCLUDE_STACK */

/* Support for: custom string list describing the available
 * `-I/usr/include`-style -> `#include <foo.h>`-paths */
#ifndef TPP_HAVE_INCLUDE_PATH
#define TPP_HAVE_INCLUDE_PATH (TPP_HAVE_INCLUDE_STACK)
#endif /* !TPP_HAVE_INCLUDE_PATH */

/* `tpp_include_paths` contains a 2nd path-list that is only searched during `"`-strings */
#ifndef TPP_HAVE_INCLUDE_PATH_QUOTE
#define TPP_HAVE_INCLUDE_PATH_QUOTE \
	(TPP_HAVE_INCLUDE_STACK && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_INCLUDE_PATH_QUOTE */

/* `tpp_include_paths` contains a 3rd path-list whose files are treated as `TPP_FILE_FLAGS_SYSHDR` */
#ifndef TPP_HAVE_INCLUDE_PATH_SYSHDR
#define TPP_HAVE_INCLUDE_PATH_SYSHDR \
	(TPP_HAVE_INCLUDE_STACK && TPP_HAVE_FILE_SYSHDR && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_INCLUDE_PATH_SYSHDR */

/* `tpp_include_paths` contains a 4th path-list that is searched after all other paths */
#ifndef TPP_HAVE_INCLUDE_PATH_AFTER
#define TPP_HAVE_INCLUDE_PATH_AFTER                                 \
	(TPP_HAVE_INCLUDE_STACK && ((TPP_PROFILE == TPP_PROFILE_ALL) || \
	                            (TPP_HAVE_PROFILE_NOT_MINIMAL &&    \
	                             TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH)))
#endif /* !TPP_HAVE_INCLUDE_PATH_AFTER */

/* A preprocessor tuple describing the built-in, hard-coded, system-include path.
 * - The paths specified here are searched in order of specification.
 * - For information on the full `#include`-path resolution order, see `tpp_include_paths`
 * - Try not to include trailing slashes in paths hard-coded using this (if TPP3 needs
 *   trailing slashes in these strings, it will add those itself)
 *
 * Example:
 * ```c
 * #define TPP_CONFIG_SYSTEM_INCLUDE_PATH  2("/usr/local/include", "/usr/include")
 * ``` */
#ifndef TPP_CONFIG_SYSTEM_INCLUDE_PATH
#define TPP_CONFIG_SYSTEM_INCLUDE_PATH 0()
#endif /* !TPP_CONFIG_SYSTEM_INCLUDE_PATH */

/* Config option to skip searching `TPP_CONFIG_SYSTEM_INCLUDE_PATH`.
 * Can (and should) be used to implement `-nostdinc` */
#ifndef TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH
#define TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH (TPP_TUPLE_NONEMPTY(TPP_CONFIG_SYSTEM_INCLUDE_PATH) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0) /* "-fstdinc" */
#endif /* !TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH */

/* `"`-quoted `#include`-strings are searched relative to *every* I/O-file found on the
 * `#include`-stack; not just the most-recent one. Doing this for all files is what TPP2
 * always- and unconditionally did, but turns out that isn't actually something normally
 * done by preprocessors. As such, TPP3 turns this behavior into an extension, but has
 * it turned off by default. */
#ifndef TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE
#define TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE ((TPP_PROFILE == TPP_PROFILE_ALL && TPP_HAVE_INCLUDE_STACK) ? TPP_CONF_EXT0 : 0) /* "-finclude-relative-to-every-file" */
#endif /* !TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE */

/* Enable support to push/pop the `#include`-path state */
#ifndef TPP_HAVE_INCLUDE_PATH_PUSH_POP
#define TPP_HAVE_INCLUDE_PATH_PUSH_POP (TPP_HAVE_INCLUDE_PATH)
#endif /* !TPP_HAVE_INCLUDE_PATH_PUSH_POP */

/* Use `TPP_REF tpp_string *` instead of `char *` in `#include`-path lists.
 * Doing so greatly reduces the overhead when `#include`-path lists are
 * pushed/popped, since `tpp_string` can be incref'd, whereas `char`
 * must be hard-copied. */
#ifndef TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING
#define TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING (TPP_HAVE_INCLUDE_PATH_PUSH_POP)
#endif /* !TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */

/* Enable support for `tpp_file` keeping track of the state of active `#ifdef` directives
 * via an embedded `tpp_ifdef_stack` strcture (accessible via `tpp_file_getifdef()`) */
#ifndef TPP_HAVE_IFDEF_STACK
#define TPP_HAVE_IFDEF_STACK (TPP_HAVE_CPP_IF_ELSE_ENDIF)
#endif /* !TPP_HAVE_IFDEF_STACK */

/* Enable support for `tpp_file_setfilename()` */
#ifndef TPP_HAVE_FILE_SETFILENAME
#define TPP_HAVE_FILE_SETFILENAME (TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE)
#endif /* !TPP_HAVE_FILE_SETFILENAME */

/* Enable support for `tpp_file_setline()` */
#ifndef TPP_HAVE_FILE_SETLINE
#define TPP_HAVE_FILE_SETLINE (TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE)
#endif /* !TPP_HAVE_FILE_SETLINE */

/* Enable support for setting a special pointer in files to
 * represent a lower bound of memory that must be kept loaded. */
#ifndef TPP_HAVE_FILE_KEEPPOS
#define TPP_HAVE_FILE_KEEPPOS (TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_FILE_KEEPPOS */

/* Keep track of the original `tpp_lexer_arginfo` used during macro invocation,
 * in order to improve `tpp_file_getlcinfo_ex()`'s `tlcix_proj*` return values,
 * by making them less error-prone. */
#ifndef TPP_HAVE_FILE_MACRO_TRACKARGS
#define TPP_HAVE_FILE_MACRO_TRACKARGS \
	(TPP_HAVE_CPP_MACROS && TPP_HAVE_LEXER_SEEKPP_RPAREN && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_FILE_MACRO_TRACKARGS */

/* Provide a special `TPP_FILE_ENCODING_EMBED` file encoding
 * to convert bytes into `,`-separated decimals on-the-fly.
 *
 * NOTE: Even when this is disabled, `#embed` directives work,
 *       but will have to pre-load the entire input file into
 *       memory, rather than allowing the file to be streamed
 *       by converting it on-the-fly. */
#ifndef TPP_HAVE_FILE_ENCODING_EMBED
#define TPP_HAVE_FILE_ENCODING_EMBED (TPP_HAVE_UNICODE && TPP_HAVE_CPP_EMBED && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_FILE_ENCODING_EMBED */

/* Provide an API `tpp_lexer_rand()` that can be used to random numbers
 * using the already-parsed contents of of I/O and TEXT files as seed. */
#ifndef TPP_HAVE_LEXER_RAND
#define TPP_HAVE_LEXER_RAND ((TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_MACRO___TPP_RANDOM)
#endif /* !TPP_HAVE_LEXER_RAND */

/* Provide an API `tpp_file_gethash()` that can be used to calculate a hash
 * of all already-processed bytes within a file up to a given position. */
#ifndef TPP_HAVE_FILE_GETHASH
#define TPP_HAVE_FILE_GETHASH ((TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_LEXER_RAND)
#endif /* !TPP_HAVE_FILE_GETHASH */

/* Provide an API `tpp_file_getfullhash()` that behaves similar to `tpp_file_gethash()`,
 * but also traverses the `#include`-stack to include the hash values of all parent files
 * found therein, such that the return values describes not only the current file leading
 * up to a given position, but also all other files that came before. */
#ifndef TPP_HAVE_FILE_GETFULLHASH
#define TPP_HAVE_FILE_GETFULLHASH (TPP_HAVE_FILE_GETHASH && TPP_HAVE_INCLUDE_STACK)
#endif /* !TPP_HAVE_FILE_GETFULLHASH */

/* Provide a secondary set of keyword APIs that include support for `\`-escape sequences.
 * Needed to implement `TPP_HAVE_BSE` and `TPP_HAVE_ESCAPE_IN_IDENTIFIERS` */
#ifndef TPP_HAVE_ESCAPED_KEYWORDS
#define TPP_HAVE_ESCAPED_KEYWORDS (TPP_HAVE_BSE || TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
#endif /* !TPP_HAVE_ESCAPED_KEYWORDS */

/* Enable support for `tpp_io_compare_mtime()`.
 * This is the underlying system function needed for
 * `#pragma GCC dependency` (see `TPP_HAVE_PRAGMA_GCC_DEPENDENCY`) */
#ifndef TPP_HAVE_IO_COMPARE_MTIME
#define TPP_HAVE_IO_COMPARE_MTIME (TPP_PROFILE == TPP_PROFILE_ALL || TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
#endif /* !TPP_HAVE_IO_COMPARE_MTIME */

/* Enable support for `tpp_io_normalize_filename()`.
 * This function is needed to detect+fix (see `TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING`)
 * incorrect casing in `#include`-paths on host platforms with case-insensitive filenames
 * (i.e.: windows).
 *
 * In particular, this function is necessary such that `#pragma once` in a header file
 * `<stdio.h>` will still prevent multiple inclusion if that file is included multiple
 * times with different path casing:
 * ```c
 * #include <stdio.h>
 * #include <STDIO.H> // `TPP_HAVE_IO_NORMALIZE_FILENAME` saves the day
 * ```
 */
#ifndef TPP_HAVE_IO_NORMALIZE_FILENAME
#define TPP_HAVE_IO_NORMALIZE_FILENAME (TPP_OS_WINDOWS && (TPP_HAVE_USER_KEYWORDS || TPP_HAVE_PROFILE_NOT_MINIMAL))
#endif /* !TPP_HAVE_IO_NORMALIZE_FILENAME */

/* Enable support for `tpp_joinpath()`, a wrapper around another internal function
 * used to implement `tpp_lexer_openfile()` (see `TPP_HAVE_LEXER_OPENFILE`) and is also
 * needed to implement `#pragma GCC dependency` (see `TPP_HAVE_PRAGMA_GCC_DEPENDENCY`)
 *
 * This function takes 2 paths, resolves `.` and `..` path references between them,
 * whilst pasting them onto each other in order to form a singular, normalized path */
#ifndef TPP_HAVE_JOINPATH
#define TPP_HAVE_JOINPATH (TPP_PROFILE == TPP_PROFILE_ALL || TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
#endif /* !TPP_HAVE_JOINPATH */

/* Enable support for `tpp_lexer_initfile_io()` and `tpp_lexer_initfile_io_ex()` */
#ifndef TPP_HAVE_LEXER_INIT_IO
#define TPP_HAVE_LEXER_INIT_IO TPP_HAVE_FILE_NOKWD
#endif /* !TPP_HAVE_LEXER_INIT_IO */

/* Enable support for `tpp_lexer_initfile_open()`, a function that lets you directly
 * initialize the lexer by passing in a filename that should be opened as input.
 *
 * When `TPP_HAVE_INCLUDE_STACK` is enabled, this also enables support for `tpp_lexer_pushfile_open()` */
#ifndef TPP_HAVE_LEXER_INIT_OPEN
#define TPP_HAVE_LEXER_INIT_OPEN TPP_HAVE_LEXER_OPENFILE
#endif /* !TPP_HAVE_LEXER_INIT_OPEN */

/* Provide an API `tpp_string_builder` centered around builtin `tpp_string` */
#ifndef TPP_HAVE_STRING_BUILDER
#define TPP_HAVE_STRING_BUILDER                                                                   \
	((TPP_PROFILE == TPP_PROFILE_ALL) ||                                                          \
	 TPP_HAVE_LEXER_DECODESTRING || TPP_HAVE_LEXER_SEEKPP_RPAREN ||                               \
	 (TPP_HAVE_LEXER_DECODE_INCLUDE_STRING &&                                                     \
	  (TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS))) ||                \
	 (TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__ || TPP_HAVE_MACRO___FILE_NAME__) || \
	 TPP_HAVE_MACRO___TPP_EVAL || TPP_HAVE_MACRO___TPP_STR_PACK || TPP_HAVE_MACRO___TPP_EXEC ||   \
	 TPP_HAVE_MACRO___TPP_STR_SUBSTR || TPP_HAVE_MACRO___TPP_LOAD_FILE || TPP_HAVE_CPP_EMBED)
#endif /* !TPP_HAVE_STRING_BUILDER */

/* Provide a function `tpp_string_builder_tryalloc()` */
#ifndef TPP_HAVE_STRING_BUILDER_TRYALLOC
#define TPP_HAVE_STRING_BUILDER_TRYALLOC ((TPP_PROFILE == TPP_PROFILE_ALL) || TPP_HAVE_MACRO___TPP_LOAD_FILE)
#endif /* !TPP_HAVE_STRING_BUILDER_TRYALLOC */

/* Enable support for `tpp_lexer_skip()` and `tpp_lexer_require()` */
#ifndef TPP_HAVE_LEXER_SKIP
#if (TPP_HAVE_PROFILE_NOT_MINIMAL ||              \
     TPP_HAVE_PRAGMA_PUSH_MACRO ||                \
     TPP_HAVE_PRAGMA_DEPRECATED ||                \
     TPP_HAVE_PRAGMA_MESSAGE ||                   \
     TPP_HAVE_PRAGMA_ERROR ||                     \
     TPP_HAVE_PRAGMA_WARNING ||                   \
     TPP_HAVE_PRAGMA_EXTENSION ||                 \
     TPP_HAVE_PRAGMA_TPP_WARNING ||               \
     TPP_HAVE_PRAGMA_TPP_EXTENSION ||             \
     TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS ||     \
     TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS || \
     TPP_HAVE_PRAGMA_TPP_EXEC ||                  \
     TPP_HAVE_PRAGMA_TPP_TPP_EXEC ||              \
     TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH ||          \
     TPP_HAVE_MACRO__Pragma ||                    \
     TPP_HAVE_MACRO___TPP_EVAL ||                 \
     TPP_HAVE_MACRO___has_include ||              \
     TPP_HAVE_MACRO___has_include_next ||         \
     TPP_HAVE_MACRO___has_embed ||                \
     TPP_HAVE_MACRO___TPP_COUNT_TOKENS ||         \
     TPP_HAVE_MACRO___TPP_STR_SIZE ||             \
     TPP_HAVE_MACRO___TPP_RANDOM ||               \
     TPP_HAVE_MACRO___TPP_STR_SUBSTR ||           \
     TPP_HAVE_CPP_ASSERT ||                       \
     TPP_HAVE_CPP_EMBED ||                        \
     TPP_HAVE_BUILTIN_PARSEEXPR_HOOK)
#define TPP_HAVE_LEXER_SKIP 1
#else /* ... */
#define TPP_HAVE_LEXER_SKIP 0
#endif /* !... */
#endif /* !TPP_HAVE_LEXER_SKIP */

/* Enable support for storing a time value in `tpp_lexer` */
#ifndef TPP_HAVE_LEXER_TIME
#define TPP_HAVE_LEXER_TIME TPP_HAVE_TIME_API
#endif /* !TPP_HAVE_LEXER_TIME */

/* Enable support for `tpp_lexer_tryskip_raw()`, a function that is used-
 * and needed in order to seek- and skip-over the `(` token following a
 * macro's name (with support for searching for tokens in parent files
 * of the current one, but rolling back all changes if the next token isn't
 * as expected). It also offers a flag `TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV`
 * that will retain the `[tpp_file_getlastpos(),*)` regions of files as
 * they are scanned, allowing it to keep the previous token (which is
 * probably the name of a macro) loaded in memory. */
#ifndef TPP_HAVE_LEXER_TRYSKIP_RAW
#define TPP_HAVE_LEXER_TRYSKIP_RAW (TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_LEXER_TRYSKIP_RAW */

/* Provide a function `tpp_lexer_reprtokenid()` to
 * return the string-representation of a given token ID */
#ifndef TPP_HAVE_LEXER_REPRTOKENID
#define TPP_HAVE_LEXER_REPRTOKENID (TPP_HAVE_LEXER_SKIP)
#endif /* !TPP_HAVE_LEXER_REPRTOKENID */

/* Provide a function `tpp_reprtokenid()` that does the
 * same as `tpp_lexer_reprtokenid()` (and is also used to
 * implement that function), but fails for user-defined
 * keyword token IDs */
#ifndef TPP_HAVE_REPRTOKENID
#define TPP_HAVE_REPRTOKENID (TPP_HAVE_LEXER_REPRTOKENID)
#endif /* !TPP_HAVE_REPRTOKENID */

/* Provide a lexer state flag `TPP_LEXER_STATE_FLAG_ALLTOKENS` that forces
 * `tpp_lexer_yieldpp()` to always re-emit *all* tokens (rather than skip
 * over space/lf/comment tokens based on `TPP_HAVE_TPP_TOK_SPACE`,
 * `TPP_HAVE_TPP_TOK_LF` and `TPP_HAVE_TPP_TOK_COMMENT`)
 *
 * This flag is also needed internally when TPP needs to expand
 * the arguments supplied to a user-defined macro */
#ifndef TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
#if (TPP_HAVE_CPP_MACROS && (TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_SPACE) || \
                             TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_LF) ||    \
                             TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_COMMENT)))
#define TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS 1
#else /* ... */
#define TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS 0
#endif /* !... */
#endif /* !TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */

/* Provide a function `tpp_lexer_getkeyworddefined()` to check
 * if a given keyword is `defined()` (meaning it can be expanded
 * as a (potentially builtin) macro) */
#ifndef TPP_HAVE_LEXER_GETKEYWORDDEFINED
#if (TPP_HAVE_CPP_IF_ELSE_ENDIF || \
     TPP_HAVE_CPP_MACROS ||        \
     TPP_HAVE_BUILTIN_EXPR_DEFINED)
#define TPP_HAVE_LEXER_GETKEYWORDDEFINED 1
#else /* ... */
#define TPP_HAVE_LEXER_GETKEYWORDDEFINED 0
#endif /* !... */
#endif /* !TPP_HAVE_LEXER_GETKEYWORDDEFINED */

/* Provide a set of macros/functions `tpp_lexer_manualpopfile_*`
 * that can be used to seek through the contents of files further
 * up the #include-stack in a way that allows for rollback.
 *
 * - `tpp_lexer_manualpopfile_start()`
 * - `tpp_lexer_manualpopfile_popfile()`
 * - `tpp_lexer_manualpopfile_canpopfile()`
 * - `tpp_lexer_manualpopfile_break_rollback()`
 * - `tpp_lexer_manualpopfile_break_commit()`
 * - `tpp_lexer_manualpopfile_break()`
 * - `tpp_lexer_manualpopfile_end_rollback()`
 * - `tpp_lexer_manualpopfile_end_commit()`
 * - `tpp_lexer_manualpopfile_end()`
 */
#ifndef TPP_HAVE_LEXER_MANUALPOPFILE
#define TPP_HAVE_LEXER_MANUALPOPFILE                          \
	(TPP_HAVE_INCLUDE_STACK && (TPP_HAVE_CPP_MACROS ||        \
	                            TPP_HAVE_LEXER_TRYSKIP_RAW || \
	                            TPP_HAVE_LEXER_SKIP))
#endif /* !TPP_HAVE_LEXER_MANUALPOPFILE */

/* Provide a function `tpp_lexer_seekpp_rparen()` that can be used
 * to find the position of a matching `)`-token for the purpose
 * of macro argument lists. */
#ifndef TPP_HAVE_LEXER_SEEKPP_RPAREN
#define TPP_HAVE_LEXER_SEEKPP_RPAREN (TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN */

/* Provide a function `tpp_lexer_seekpp_rparen_ex()` that is pretty much
 * the same as `tpp_lexer_seekpp_rparen()`, but is also able to deal with
 * alternate parenthesis pairs: `[ ]` `{ }` `< >` in addition to `( )` */
#ifndef TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
#define TPP_HAVE_LEXER_SEEKPP_RPAREN_EX (TPP_HAVE_LEXER_SEEKPP_RPAREN && TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS)
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */

/* Provide a function `tpp_macro_equals()` to compare macro definitions.
 * Because the C standard specifies that re-defining a macro should only
 * be worthy of a warning if its new definition differs from its old one,
 * this function is used to only empty `TPP_W_REDEFINE_MACRO` if exactly
 * that happened.
 *
 * ```c
 * #define foo 10
 * #define foo 10  // Don't warn here (not even because of this comment)
 * #define foo 11  // But *do* warn here!
 * ``` */
#ifndef TPP_HAVE_MACRO_EQUALS
#define TPP_HAVE_MACRO_EQUALS (TPP_HAVE_TPP_W_REDEFINE_MACRO)
#endif /* !TPP_HAVE_MACRO_EQUALS */

/* Provide a function `tpp_lexer_decodestring()` to decode the data contained within strings */
#ifndef TPP_HAVE_LEXER_DECODESTRING
#define TPP_HAVE_LEXER_DECODESTRING (TPP_HAVE_TPP_TOK_C_STRINGLIKE)
#endif /* !TPP_HAVE_LEXER_DECODESTRING */

/* Provide an optional performance-optimization flag `TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS`
 * that may be passed to `tpp_lexer_parsestring_cb()` to speed up decoding of certain kinds
 * of strings (by slightly weakening what callbacks are allowed to do when this flag is set) */
#ifndef TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS
#define TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS \
	(TPP_HAVE_LEXER_DECODESTRING && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS */

/* Provide a function `tpp_expr_value_printrepr()` to construct the result
 * of `__TPP_EVAL` (see `TPP_HAVE_MACRO___TPP_EVAL`) */
#ifndef TPP_HAVE_EXPR_VALUE_PRINTREPR
#define TPP_HAVE_EXPR_VALUE_PRINTREPR (TPP_HAVE_MACRO___TPP_EVAL)
#endif /* !TPP_HAVE_EXPR_VALUE_PRINTREPR */

/* Provide a function `tpp_token_encodestring()` to perform `\`-escaping of arbitrary data */
#ifndef TPP_HAVE_TOKEN_ENCODESTRING
#define TPP_HAVE_TOKEN_ENCODESTRING                                          \
	(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT || \
	 (TPP_HAVE_EXPR_VALUE_PRINTREPR && TPP_HAVE_BUILTIN_EXPR_STRINGS) ||     \
	 TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__ ||              \
	 TPP_HAVE_MACRO___FILE_NAME__ || TPP_HAVE_MACRO___TPP_STR_PACK ||        \
	 TPP_HAVE_MACRO___TPP_STR_SUBSTR ||                                      \
	 (TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_LEXER_DUMP_DEFINITIONS))
#endif /* !TPP_HAVE_TOKEN_ENCODESTRING */

/* Provide a function `tpp_token_require_whitespace()` to check if 2 tokens,
 * when written directly adjacent to each other, *might* produce a different
 * (set of) token(s) when re-parsed.
 *
 * This function is used to implement `TPP_HAVE_MAGIC_WHITESPACE`, which in
 * turn is needed to inject additional whitespace when failure to do so could
 * result in accidental token concatenation during reparsing. */
#ifndef TPP_HAVE_TOKEN_REQUIRE_WHITESPACE
#define TPP_HAVE_TOKEN_REQUIRE_WHITESPACE TPP_HAVE_MAGIC_WHITESPACE
#endif /* !TPP_HAVE_TOKEN_REQUIRE_WHITESPACE */

/* Provide a function `tpp_lexer_decodeint_expr()` to parse an integer into a `tpp_expr_value` */
#ifndef TPP_HAVE_LEXER_DECODEINT_EXPR
#define TPP_HAVE_LEXER_DECODEINT_EXPR (TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_TPP_TOK_INT)
#endif /* !TPP_HAVE_LEXER_DECODEINT_EXPR */

/* Provide a function `tpp_lexer_decodeint()` to parse an integer */
#ifndef TPP_HAVE_LEXER_DECODEINT
#define TPP_HAVE_LEXER_DECODEINT                                 \
	(TPP_HAVE_TPP_TOK_INT && (TPP_HAVE_LEXER_DECODEINT_EXPR ||   \
	                          TPP_HAVE_CPP_LINE ||               \
	                          TPP_HAVE_CPP_DIGIT_LINE ||         \
	                          TPP_HAVE_MACRO___TPP_STR_PACK ||   \
	                          TPP_HAVE_MACRO___TPP_RANDOM ||     \
	                          TPP_HAVE_MACRO___TPP_STR_SUBSTR || \
	                          TPP_HAVE_PRAGMA_WARNING ||         \
	                          TPP_HAVE_PRAGMA_TPP_WARNING))
#endif /* !TPP_HAVE_LEXER_DECODEINT */

/* Add API support for integer type suffixes (see `tpp_integer_suffix_kind`) */
#ifndef TPP_HAVE_LEXER_DECODEINT_SUFFIX
#define TPP_HAVE_LEXER_DECODEINT_SUFFIX (TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX)
#endif /* !TPP_HAVE_LEXER_DECODEINT_SUFFIX */

/* Provide a function `tpp_lexer_decodefloat_expr()` to parse a float into a `tpp_expr_value` */
#ifndef TPP_HAVE_LEXER_DECODEFLOAT_EXPR
#define TPP_HAVE_LEXER_DECODEFLOAT_EXPR (TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_FLOATS && TPP_HAVE_TPP_TOK_FLOAT)
#endif /* !TPP_HAVE_LEXER_DECODEFLOAT_EXPR */

/* Provide a function `tpp_lexer_decodefloat()` to parse a float */
#ifndef TPP_HAVE_LEXER_DECODEFLOAT
#define TPP_HAVE_LEXER_DECODEFLOAT (TPP_HAVE_LEXER_DECODEFLOAT_EXPR)
#endif /* !TPP_HAVE_LEXER_DECODEFLOAT */

/* Add API support for float type suffixes (see `tpp_float_suffix_kind`) */
#ifndef TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX
#define TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX (TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX || TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX)
#endif /* !TPP_HAVE_LEXER_DECODEFLOAT_SUFFIX */

/* Provide a function `tpp_lexer_parsecharacter_literal()` to parse character literals */
#ifndef TPP_HAVE_LEXER_PARSECHARACTER_LITERAL
#define TPP_HAVE_LEXER_PARSECHARACTER_LITERAL (TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS)
#endif /* !TPP_HAVE_LEXER_PARSECHARACTER_LITERAL */

/* Provide a function `tpp_lexer_parsestring_expr()` to parse a string into a `tpp_expr_value` */
#ifndef TPP_HAVE_LEXER_PARSESTRING_EXPR
#define TPP_HAVE_LEXER_PARSESTRING_EXPR (TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_STRINGS && TPP_HAVE_LEXER_DECODESTRING)
#endif /* !TPP_HAVE_LEXER_PARSESTRING_EXPR */

/* Provide a function `tpp_lexer_parsecharacter_expr()` to parse a character literal into a `tpp_expr_value` */
#ifndef TPP_HAVE_LEXER_PARSECHARACTER_EXPR
#define TPP_HAVE_LEXER_PARSECHARACTER_EXPR (TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS && TPP_HAVE_LEXER_DECODESTRING)
#endif /* !TPP_HAVE_LEXER_PARSECHARACTER_EXPR */

/* Provide a builtin implementation for `tpp_expr_value` and its API */
#undef TPP_HAVE_BUILTIN_EXPR_VALUE
#if !defined(tpp_expr_value) && TPP_HAVE_PARSEEXPR_HOOK
#define TPP_HAVE_BUILTIN_EXPR_VALUE 1
#else /* !tpp_expr_value && TPP_HAVE_PARSEEXPR_HOOK */
#define TPP_HAVE_BUILTIN_EXPR_VALUE 0
#endif /* tpp_expr_value || !TPP_HAVE_PARSEEXPR_HOOK */

/* Provide a function `tpp_ftoa()` to convert a float into a string */
#ifndef TPP_HAVE_FTOA
#define TPP_HAVE_FTOA (TPP_HAVE_EXPR_VALUE_PRINTREPR)
#endif /* !TPP_HAVE_FTOA */

/* Provide a function `tpp_extension_nearest()` */
#ifndef TPP_HAVE_TPP_EXTENSION_NEAREST
#define TPP_HAVE_TPP_EXTENSION_NEAREST \
	(TPP_HAVE_TPP_W_UNKNOWN_EXTENSION && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_TPP_EXTENSION_NEAREST */

/* Provide a function `tpp_warning_group_nearest()` */
#ifndef TPP_HAVE_TPP_WARNING_GROUP_NEAREST
#define TPP_HAVE_TPP_WARNING_GROUP_NEAREST \
	(TPP_HAVE_TPP_W_UNKNOWN_WARNING && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_TPP_WARNING_GROUP_NEAREST */

/* Provide a function `tpp_fuzzy_memcmp()` to quantify the
 * *fuzziness* of how close 2 memory-blocks are to each other */
#ifndef TPP_HAVE_TPP_FUZZY_MEMCMP
#define TPP_HAVE_TPP_FUZZY_MEMCMP \
	(TPP_HAVE_TPP_EXTENSION_NEAREST || TPP_HAVE_TPP_WARNING_GROUP_NEAREST)
#endif /* !TPP_HAVE_TPP_FUZZY_MEMCMP */

/* Provide a function `tpp_lexer_define()` + `tpp_lexer_undef()`
 * that can be used to define/undef commandline-defined macros. */
#ifndef TPP_HAVE_LEXER_CLI_DEFINE
#define TPP_HAVE_LEXER_CLI_DEFINE (TPP_HAVE_PROFILE_NOT_MINIMAL && TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_LEXER_CLI_DEFINE */

/* Provide a function `tpp_lexer_assert()` + `tpp_lexer_unassert()` + `tpp_lexer_unassertall()`
 * that can be used to add/delete keyword assertions. */
#ifndef TPP_HAVE_LEXER_CLI_ASSERT
#define TPP_HAVE_LEXER_CLI_ASSERT (TPP_HAVE_PROFILE_NOT_MINIMAL && TPP_HAVE_CPP_ASSERT)
#endif /* !TPP_HAVE_LEXER_CLI_ASSERT */

/* Provide a function `tpp_keywords_undefall()` + `tpp_lexer_undefall()`
 * that can be used to quickly delete *all* macro definitions. */
#ifndef TPP_HAVE_KEYWORDS_UNDEFALL
#define TPP_HAVE_KEYWORDS_UNDEFALL ((TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_KEYWORDS_UNDEFALL */

/* Provide a function `tpp_keywords_unassertall()` + `tpp_lexer_unassertall2()`
 * that can be used to quickly delete *all* keyword assertions. */
#ifndef TPP_HAVE_KEYWORDS_UNASSERTALL
#define TPP_HAVE_KEYWORDS_UNASSERTALL ((TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_CPP_ASSERT)
#endif /* !TPP_HAVE_KEYWORDS_UNASSERTALL */

/* Provide a function `tpp_keywords_resetflags()` + `tpp_lexer_kwds_resetflags()`
 * that can be used to delete the flags of all keywords. */
#ifndef TPP_HAVE_KEYWORDS_RESETFLAGS
#define TPP_HAVE_KEYWORDS_RESETFLAGS                   \
	((TPP_PROFILE == TPP_PROFILE_ALL) &&               \
	 (TPP_HAVE_PRAGMA_ONCE ||                          \
	  TPP_HAVE_CPP_IMPORT ||                           \
	  TPP_HAVE_CLANG_MACRO___has_attribute ||          \
	  TPP_HAVE_CLANG_MACRO___has_builtin ||            \
	  TPP_HAVE_CLANG_MACRO___has_cpp_attribute ||      \
	  TPP_HAVE_CLANG_MACRO___has_declspec_attribute || \
	  TPP_HAVE_CLANG_MACRO___has_extension ||          \
	  TPP_HAVE_CLANG_MACRO___has_feature ||            \
	  TPP_HAVE_CLANG_MACRO___has_c_attribute ||        \
	  TPP_HAVE_MACRO___is_deprecated ||                \
	  TPP_HAVE_MACRO___is_poisoned ||                  \
	  TPP_HAVE_PRAGMA_DEPRECATED ||                    \
	  TPP_HAVE_PRAGMA_GCC_POISON ||                    \
	  TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS))
#endif /* !TPP_HAVE_KEYWORDS_RESETFLAGS */

/* Provide a function `tpp_keywords_resetcounters()` +
 * `tpp_lexer_kwds_resetcounters()` that can be used to
 * reset the state of all `__TPP_COUNTER()' macro expansions. */
#ifndef TPP_HAVE_KEYWORDS_RESETCOUNTERS
#define TPP_HAVE_KEYWORDS_RESETCOUNTERS \
	((TPP_PROFILE == TPP_PROFILE_ALL) && TPP_HAVE_MACRO___TPP_COUNTER)
#endif /* !TPP_HAVE_KEYWORDS_RESETCOUNTERS */

/* Filename of definitions file used by `tpp_lexer_define()` */
#ifndef TPP_CONFIG_CLI_FILENAME
#define TPP_CONFIG_CLI_FILENAME "<commandline>"
#endif /* !TPP_CONFIG_CLI_FILENAME */

/* Provide a function `tpp_lexer_dump_definitions()` that can be
 * used to re-print all user-defined macro definitions and asserts. */
#ifndef TPP_HAVE_LEXER_DUMP_DEFINITIONS
#define TPP_HAVE_LEXER_DUMP_DEFINITIONS (TPP_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_HAVE_LEXER_DUMP_DEFINITIONS */

/* Extension to `TPP_HAVE_LEXER_DUMP_DEFINITIONS`: provide an
 * additional flag `TPP_LEXER_DUMP_DEFINITIONS_SORTED` that causes
 * macro (`TPP_HAVE_CPP_MACROS`) / assert (`TPP_HAVE_CPP_ASSERT`)
 * definition to be sorted based on the ID of the associated keyword
 * (which corresponds with the first time the associated keyword was
 * seen, which also usually lines up with the order in which macros
 * were defined) */
#ifndef TPP_HAVE_LEXER_DUMP_DEFINITIONS_SORTED
#define TPP_HAVE_LEXER_DUMP_DEFINITIONS_SORTED (TPP_HAVE_LEXER_DUMP_DEFINITIONS)
#endif /* !TPP_HAVE_LEXER_DUMP_DEFINITIONS_SORTED */

/* Extension to `TPP_HAVE_LEXER_DUMP_DEFINITIONS`: provide an
 * additional flag `TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO` that causes every
 * dumped macro definition to be preceded by a comment containing (among
 * other things) the file/line/column where that definition comes from. */
#ifndef TPP_HAVE_LEXER_DUMP_DEFINITIONS_EXTRAINFO
#define TPP_HAVE_LEXER_DUMP_DEFINITIONS_EXTRAINFO (TPP_HAVE_LEXER_DUMP_DEFINITIONS)
#endif /* !TPP_HAVE_LEXER_DUMP_DEFINITIONS_EXTRAINFO */

#if TPP_HAVE_MACRO___has_embed
/* String representations of what `__has_embed()`
 * should expand to when the file wasn't found
 *
 * Also represents the expansion of the pre-defined macro `__STDC_EMBED_NOT_FOUND__`
 *
 * @detect: #ifdef __STDC_EMBED_NOT_FOUND__ */
#ifndef TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND
#define TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND "0"
#endif /* !TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND */

/* String representations of what `__has_embed()` should
 * expand to when the file was found and is non-empty
 *
 * Also represents the expansion of the pre-defined macro `__STDC_EMBED_FOUND__`
 *
 * @detect: #ifdef __STDC_EMBED_FOUND__ */
#ifndef TPP_CONFIG_VALUEOF_STDC_EMBED_FOUND
#define TPP_CONFIG_VALUEOF_STDC_EMBED_FOUND "1"
#endif /* !TPP_CONFIG_VALUEOF_STDC_EMBED_FOUND */

/* String representations of what `__has_embed()` should
 * expand to when the file was found, but is empty
 *
 * Also represents the expansion of the pre-defined macro `__STDC_EMBED_EMPTY__`
 *
 * @detect: #ifdef __STDC_EMBED_EMPTY__ */
#ifndef TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY
#define TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY "2"
#endif /* !TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY */
#endif /* TPP_HAVE_MACRO___has_embed */

/* Extra configuration for `#pragma message`: print a leading
 * `TPP_CONFIG_FILE_AND_LINE_FORMAT` using the values
 * that would also be printed by `__FILE__`, `__LINE__`, `__COLUMN__` */
#ifndef TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION
#define TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION (TPP_HAVE_PROFILE_NOT_MINIMAL ? TPP_CONF_EXT0 : 0) /* "-fpragma-message-prints-location" */
#endif /* !TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION */

/* Extra configuration for `#pragma message`: print a trailing `\n` */
#ifndef TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED
#define TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED (TPP_HAVE_PROFILE_NOT_MINIMAL ? TPP_CONF_EXT0 : 0) /* "-fpragma-message-omits-trailing-linefeed" */
#endif /* !TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED */

/************************************************************************/
/************************************************************************/
/************************************************************************/





/************************************************************************/
/* WARNINGS                                                             */
/************************************************************************/

/* Format to use for file+line+column log messages.
 * When `TPP_HAVE_RT_FILE_AND_LINE_FORMAT` is enabled, this is
 * only the *default*-format, with the actual format being overwritable
 * at runtime. */
#ifndef TPP_CONFIG_FILE_AND_LINE_FORMAT
#if defined(_MSC_VER)
#define TPP_CONFIG_FILE_AND_LINE_FORMAT "%Pf(%Pl, %Pc): "
#else /* _MSC_VER */
#define TPP_CONFIG_FILE_AND_LINE_FORMAT "%Pf:%Pl:%Pc: "
#endif /* !_MSC_VER */
#endif /* !TPP_CONFIG_FILE_AND_LINE_FORMAT */

/* Allow the file-and-line format used by warning to be overwritten on a per-lexer basis.
 *
 * When this is disabled, `TPP_CONFIG_FILE_AND_LINE_FORMAT` is always used instead.
 *
 * **Getter**: `tpp_lexer_getfileandlineformat(lexer)`<br/>
 * **Setter**: `tpp_lexer_setfileandlineformat(lexer, format)` */
#ifndef TPP_HAVE_RT_FILE_AND_LINE_FORMAT
#define TPP_HAVE_RT_FILE_AND_LINE_FORMAT \
	(TPP_PROFILE == TPP_PROFILE_ALL &&   \
	 (TPP_HAVE_WARNINGS ||               \
	  (TPP_HAVE_PRAGMA_MESSAGE &&        \
	   TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION)))
#endif /* !TPP_HAVE_RT_FILE_AND_LINE_FORMAT */

/* General config for `-Wquality` warnings. When overwritten
 * to `0`, all `-Wquality` warnings will be disabled. */
#ifndef TPP_HAVE_QUALITY_WARNINGS
#define TPP_HAVE_QUALITY_WARNINGS (TPP_HAVE_WARNINGS && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_HAVE_QUALITY_WARNINGS */

/* Configurations for individual warnings */
#ifndef TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT
#define TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT TPP_HAVE_WARNINGS
#endif /* !TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT */
#ifndef TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
#define TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED (TPP_HAVE_WARNINGS && TPP_HAVE_BSE)
#endif /* !TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
#ifndef TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
#define TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH (TPP_HAVE_WARNINGS && TPP_HAVE_TRIGRAPHS)
#endif /* !TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */
#ifndef TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
#define TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED        \
	(TPP_HAVE_WARNINGS &&                                   \
	 ((TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE) && \
	   (TPP_HAVE_TPP_TOK_C_STRING ||                        \
	    TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||         \
	    TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||         \
	    TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL ||        \
	    TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL ||        \
	    TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||              \
	    TPP_HAVE_TPP_TOK_C_CHAR ||                          \
	    TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||           \
	    TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||           \
	    TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||          \
	    TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||          \
	    TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)) ||              \
	  TPP_HAVE_LEXER_YIELD_INCLUDE_STRING))
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
#ifndef TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED
#define TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED \
	(TPP_HAVE_WARNINGS &&                              \
	 TPP_HAVE_STRING_WARN_MULTILINE &&                 \
	 (TPP_HAVE_TPP_TOK_C_STRING ||                     \
	  TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||      \
	  TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||      \
	  TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL ||     \
	  TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL ||     \
	  TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||           \
	  TPP_HAVE_TPP_TOK_C_CHAR ||                       \
	  TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||        \
	  TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||        \
	  TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||       \
	  TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||       \
	  TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL))
#endif /* !TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED */
#ifndef TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#define TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_TPP_TOK_C_STRINGLIKE || TPP_HAVE_LEXER_YIELD_INCLUDE_STRING))
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
#ifndef TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
#define TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF (TPP_HAVE_WARNINGS && TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE)
#endif /* !TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN (TPP_HAVE_WARNINGS && TPP_HAVE_LEXER_SKIP)
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK (TPP_HAVE_WARNINGS && TPP_HAVE_MACRO___TPP_STR_PACK)
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING (TPP_HAVE_WARNINGS && (TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING))
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING_AFTER_COLON
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING_AFTER_COLON (TPP_HAVE_WARNINGS && (TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING))
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING_AFTER_COLON */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_GCC_DIAGNOSTIC
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_GCC_DIAGNOSTIC (TPP_HAVE_WARNINGS && TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC)
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_GCC_DIAGNOSTIC */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_TPP_INCLUDE_PATH
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_TPP_INCLUDE_PATH (TPP_HAVE_WARNINGS && TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH)
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_TPP_INCLUDE_PATH */
#ifndef TPP_HAVE_TPP_W_ERROR
#define TPP_HAVE_TPP_W_ERROR (TPP_HAVE_WARNINGS && (TPP_HAVE_CPP_ERROR || TPP_HAVE_PRAGMA_ERROR || TPP_HAVE_PRAGMA_GCC_ERROR))
#endif /* !TPP_HAVE_TPP_W_ERROR */
#ifndef TPP_HAVE_TPP_W_WARNING
#define TPP_HAVE_TPP_W_WARNING (TPP_HAVE_WARNINGS && (TPP_HAVE_CPP_WARNING || TPP_HAVE_PRAGMA_GCC_WARNING))
#endif /* !TPP_HAVE_TPP_W_WARNING */
#ifndef TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS
#define TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS (TPP_HAVE_WARNINGS && TPP_HAVE_PRAGMA)
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
#ifndef TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE
#define TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE (TPP_HAVE_WARNINGS && TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_POUND_COMMENT) && TPP_HAVE_CPP_DIRECTIVES)
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE */
#ifndef TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
#define TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE (TPP_HAVE_WARNINGS && TPP_HAVE_PRAGMA)
#endif /* !TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE */
#ifndef TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER
#define TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER (TPP_HAVE_WARNINGS && TPP_HAVE_PRAGMA_ONCE && TPP_HAVE_INCLUDE_STACK)
#endif /* !TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER */
#ifndef TPP_HAVE_TPP_W_DEPRECATED_KEYWORD
#define TPP_HAVE_TPP_W_DEPRECATED_KEYWORD (TPP_HAVE_WARNINGS && (TPP_HAVE_PRAGMA_DEPRECATED || TPP_HAVE_PRAGMA_GCC_POISON))
#endif /* !TPP_HAVE_TPP_W_DEPRECATED_KEYWORD */
#ifndef TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK
#define TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK (TPP_HAVE_WARNINGS && TPP_HAVE_PRAGMA_PUSH_MACRO)
#endif /* !TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
#ifndef TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE
#define TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE \
	(TPP_HAVE_WARNINGS && TPP_HAVE_STRING_ESCAPE)
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE */
#ifndef TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST
#define TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST (TPP_HAVE_WARNINGS && TPP_HAVE_LEXER_SEEKPP_RPAREN)
#endif /* !TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST */
#ifndef TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS
#define TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS (TPP_HAVE_WARNINGS && TPP_HAVE_LEXER_SEEKPP_RPAREN)
#endif /* !TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS */
#ifndef TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS
#define TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS (TPP_HAVE_WARNINGS && TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */
#ifndef TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#define TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_CPP_DEFINE))
#endif /* !TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
#ifndef TPP_HAVE_TPP_W_EXPECTED_ASSERTION_KEY_IN_DIRECTIVE
#define TPP_HAVE_TPP_W_EXPECTED_ASSERTION_KEY_IN_DIRECTIVE \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_ASSERT)
#endif /* !TPP_HAVE_TPP_W_EXPECTED_ASSERTION_KEY_IN_DIRECTIVE */
#ifndef TPP_HAVE_TPP_W_EXPECTED_ASSERTION_VALUE_IN_DIRECTIVE
#define TPP_HAVE_TPP_W_EXPECTED_ASSERTION_VALUE_IN_DIRECTIVE \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_ASSERT)
#endif /* !TPP_HAVE_TPP_W_EXPECTED_ASSERTION_VALUE_IN_DIRECTIVE */
#ifndef TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
#define TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE      \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_CPP_IF_ELSE_ENDIF || \
	                       TPP_HAVE_CPP_DEFINE ||        \
	                       TPP_HAVE_CPP_INCLUDE ||       \
	                       TPP_HAVE_CPP_INCLUDE_NEXT ||  \
	                       TPP_HAVE_CPP_IMPORT ||        \
	                       TPP_HAVE_CPP_EMBED ||         \
	                       TPP_HAVE_CPP_ASSERT ||        \
	                       TPP_HAVE_CPP_DIGIT_LINE ||    \
	                       TPP_HAVE_CPP_LINE ||          \
	                       TPP_HAVE_CPP_IDENT_SCCS))
#endif /* !TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
#ifndef TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
#define TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_DEFINE)
#endif /* !TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO */
#ifndef TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO
#define TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_DEFINE)
#endif /* !TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO */
#ifndef TPP_HAVE_TPP_W_REDEFINE_MACRO
#define TPP_HAVE_TPP_W_REDEFINE_MACRO \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_DEFINE)
#endif /* !TPP_HAVE_TPP_W_REDEFINE_MACRO */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_DEFINE)
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
#ifndef TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
#define TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_DEFINE)
#endif /* !TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME */
#ifndef TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT
#define TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT \
	(TPP_HAVE_WARNINGS && TPP_HAVE_VA_OPT_IN_MACROS)
#endif /* !TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT */
#ifndef TPP_HAVE_TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT
#define TPP_HAVE_TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT \
	(TPP_HAVE_WARNINGS && TPP_HAVE_VA_OPT_IN_MACROS)
#endif /* !TPP_HAVE_TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT */
#ifndef TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED
#define TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED                  \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_EXPR_DEFINED && \
	 TPP_CONF_MAYBE_0(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR))
#endif /* !TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED */
#ifndef TPP_HAVE_TPP_W_EXPECTED_STRING
#if (TPP_HAVE_WARNINGS && TPP_HAVE_TPP_TOK_C_STRINGLIKE && \
     (TPP_HAVE_PROFILE_NOT_MINIMAL ||                    \
      TPP_HAVE_PRAGMA_PUSH_MACRO ||                      \
      TPP_HAVE_PRAGMA_DEPRECATED ||                      \
      TPP_HAVE_PRAGMA_EXTENSION ||                       \
      TPP_HAVE_PRAGMA_TPP_EXTENSION ||                   \
      TPP_HAVE_PRAGMA_MESSAGE ||                         \
      TPP_HAVE_PRAGMA_ERROR ||                           \
      TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC ||                  \
      TPP_HAVE_PRAGMA_GCC_WARNING ||                     \
      TPP_HAVE_PRAGMA_GCC_ERROR ||                       \
      TPP_HAVE_PRAGMA_TPP_EXEC ||                        \
      TPP_HAVE_PRAGMA_TPP_TPP_EXEC ||                    \
      TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS ||           \
      TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS ||       \
      TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH ||                \
      TPP_HAVE_MACRO__Pragma ||                          \
      TPP_HAVE_MACRO___TPP_IDENTIFIER ||                 \
      TPP_HAVE_MACRO___TPP_STR_DECOMPILE ||              \
      TPP_HAVE_MACRO___TPP_COUNT_TOKENS ||               \
      TPP_HAVE_MACRO___TPP_STR_SIZE ||                   \
      TPP_HAVE_MACRO___TPP_EXEC ||                       \
      TPP_HAVE_CPP_IDENT_SCCS))
#define TPP_HAVE_TPP_W_EXPECTED_STRING 1
#else /* ... */
#define TPP_HAVE_TPP_W_EXPECTED_STRING 0
#endif /* !... */
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
#ifndef TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING
#define TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING \
	(TPP_HAVE_WARNINGS && TPP_HAVE_LEXER_YIELD_INCLUDE_STRING)
#endif /* !TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
#ifndef TPP_HAVE_TPP_W_NO_SUCH_FILE
#define TPP_HAVE_TPP_W_NO_SUCH_FILE                     \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_CPP_INCLUDE ||      \
	                       TPP_HAVE_CPP_INCLUDE_NEXT || \
	                       TPP_HAVE_CPP_IMPORT ||       \
	                       TPP_HAVE_CPP_EMBED ||        \
	                       TPP_HAVE_MACRO___TPP_LOAD_FILE))
#endif /* !TPP_HAVE_TPP_W_NO_SUCH_FILE */
#ifndef TPP_HAVE_TPP_W_UNKNOWN_EMBED_PARAMETER
#define TPP_HAVE_TPP_W_UNKNOWN_EMBED_PARAMETER \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_EMBED)
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_EMBED_PARAMETER */
#ifndef TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
#define TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF \
	(TPP_HAVE_WARNINGS && TPP_HAVE_IFDEF_STACK)
#endif /* !TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */
#ifndef TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#define TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_IF_ELSE_ENDIF)
#endif /* !TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF */
#ifndef TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#define TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_IF_ELSE_ENDIF)
#endif /* !TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
#ifndef TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF
#define TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_IF_ELSE_ENDIF)
#endif /* !TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF */
#ifndef TPP_HAVE_TPP_W_ENDIF_LABELS
#define TPP_HAVE_TPP_W_ENDIF_LABELS \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_IF_ELSE_ENDIF)
#endif /* !TPP_HAVE_TPP_W_ENDIF_LABELS */
#ifndef TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF
#define TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_IF_ELSE_ENDIF)
#endif /* !TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_PARSEEXPR_HOOK)
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION */
#ifndef TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION
#define TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_PARSEEXPR_HOOK)
#endif /* !TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION */
#ifndef TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
#define TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_EXPR_DEFINED)
#endif /* !TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED */
#ifndef TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_ASSERTION
#define TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_ASSERTION \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_CPP_ASSERT))
#endif /* !TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_ASSERTION */
#ifndef TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS
#define TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && TPP_HAVE_BUILTIN_EXPR_STRINGS)
#endif /* !TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS */
#ifndef TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
#define TPP_HAVE_TPP_W_DIVIDE_BY_ZERO \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_EXPR_VALUE)
#endif /* !TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */
#ifndef TPP_HAVE_TPP_W_INVALID_INTEGER
#define TPP_HAVE_TPP_W_INVALID_INTEGER \
	(TPP_HAVE_WARNINGS && TPP_HAVE_LEXER_DECODEINT)
#endif /* !TPP_HAVE_TPP_W_INVALID_INTEGER */
#ifndef TPP_HAVE_TPP_W_MULTICHAR_LITERAL
#define TPP_HAVE_TPP_W_MULTICHAR_LITERAL \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS)
#endif /* !TPP_HAVE_TPP_W_MULTICHAR_LITERAL */
#ifndef TPP_HAVE_TPP_W_DATE_TIME
#define TPP_HAVE_TPP_W_DATE_TIME                           \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_MACRO___TIME__ ||      \
	                       TPP_HAVE_MACRO___DATE__ ||      \
	                       TPP_HAVE_MACRO___TIMESTAMP__ || \
	                       TPP_HAVE_NUMERIC_DATE_MACROS || \
	                       TPP_HAVE_NUMERIC_TIME_MACROS))
#endif /* !TPP_HAVE_TPP_W_DATE_TIME */
#ifndef TPP_HAVE_TPP_W_UNKNOWN_EXTENSION
#define TPP_HAVE_TPP_W_UNKNOWN_EXTENSION \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION))
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_EXTENSION */
#ifndef TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS
#define TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION) && TPP_HAVE_EXTENSIONS_PUSH_POP)
#endif /* !TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS */
#ifndef TPP_HAVE_TPP_W_UNKNOWN_WARNING
#define TPP_HAVE_TPP_W_UNKNOWN_WARNING \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING))
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_WARNING */
#ifndef TPP_HAVE_TPP_W_UNKNOWN_WARNING_NUMBER
#define TPP_HAVE_TPP_W_UNKNOWN_WARNING_NUMBER \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING) && TPP_HAVE_WARNING_NUMBERS)
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_WARNING_NUMBER */
#ifndef TPP_HAVE_TPP_W_CANNOT_POP_WARNINGS
#define TPP_HAVE_TPP_W_CANNOT_POP_WARNINGS \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING) && TPP_HAVE_WARNINGS_PUSH_POP)
#endif /* !TPP_HAVE_TPP_W_CANNOT_POP_WARNINGS */
#ifndef TPP_HAVE_TPP_W_EXPECTED_INT_AFTER_LINE_DIRECTIVE
#define TPP_HAVE_TPP_W_EXPECTED_INT_AFTER_LINE_DIRECTIVE \
	(TPP_HAVE_WARNINGS && TPP_HAVE_CPP_LINE)
#endif /* !TPP_HAVE_TPP_W_EXPECTED_INT_AFTER_LINE_DIRECTIVE */
#ifndef TPP_HAVE_TPP_W_DEPENDENCY_CHANGED
#define TPP_HAVE_TPP_W_DEPENDENCY_CHANGED \
	(TPP_HAVE_WARNINGS && TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
#endif /* !TPP_HAVE_TPP_W_DEPENDENCY_CHANGED */
#ifndef TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED
#define TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED \
	(TPP_HAVE_WARNINGS && TPP_MAX_INCLUDE_DEPTH != 0)
#endif /* !TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED */
#ifndef TPP_HAVE_TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED
#define TPP_HAVE_TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED \
	(TPP_HAVE_WARNINGS && TPP_MAX_RECURSIVE_MACRO_DEPTH != 0)
#endif /* !TPP_HAVE_TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED */
#ifndef TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING
#define TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING \
	(TPP_HAVE_WARNINGS && TPP_HAVE_IO_NORMALIZE_FILENAME)
#endif /* !TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING */
#ifndef TPP_HAVE_TPP_W_PAREN_AROUND_LAND
#define TPP_HAVE_TPP_W_PAREN_AROUND_LAND                                                  \
	(TPP_HAVE_WARNINGS && TPP_HAVE_QUALITY_WARNINGS && TPP_HAVE_BUILTIN_PARSEEXPR_HOOK && \
	 TPP_HAVE_TPP_TOK_PIPE_PIPE && TPP_HAVE_TPP_TOK_AMP_AMP)
#endif /* !TPP_HAVE_TPP_W_PAREN_AROUND_LAND */
#ifndef TPP_HAVE_TPP_W_CANNOT_POP_INCLUDE_PATHS
#define TPP_HAVE_TPP_W_CANNOT_POP_INCLUDE_PATHS \
	(TPP_HAVE_WARNINGS && TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_INCLUDE_PATH_PUSH_POP)
#endif /* !TPP_HAVE_TPP_W_CANNOT_POP_INCLUDE_PATHS */
/************************************************************************/
/************************************************************************/
/************************************************************************/

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CONFIG_H */
