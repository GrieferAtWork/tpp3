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

/* Multi-file TPP needs to use its own "defs.h" for dynamic definitions
 * -- The amalgamation hard-overrides this macro with its own filename. */
#undef TPP_CONFIG_DEFS_FILENAME
#define TPP_CONFIG_DEFS_FILENAME "defs.h"

/* Multi-file TPP should be built using the "all" profile
 * -- Use the amalgamation if you want to use other profiles. */
#undef TPP_PROFILE
#define TPP_PROFILE TPP_PROFILE_ALL

/*[[[tpp-begin]]]*/

/************************************************************************/
/*                                                                      */
/* To learn how to supply your own keywords/warnings/extensions/etc.    */
/* definitions to TPP3, see comment at start of "File: parts/defs.h"    */
/*                                                                      */
/************************************************************************/


/************************************************************************/
/* API limits:
 * - Positive: compile-time hard-code
 * - Negative: runtime configurable (with absolute value used as default)
 */
/************************************************************************/



/* TPP tab size.
 * when positive: compile-time hard-code
 * when negative: runtime-configurable, with absolute value being used as default */
#ifndef TPP_TABSIZE
#define TPP_TABSIZE (-4)
#endif /* !TPP_TABSIZE */

/* Max # of "TPP_WSTATE_ERROR" warnings that can be emitted
 * before the next one will be treated as "TPP_WSTATE_FATAL"
 *
 * - 0:  Set limit to "0" (when not also overwritten, causes TPP_HAVE_WARNING_ERROR=0)
 * - N:  Limit is hard-coded to "N" and cannot be overwritten at runtime
 * - -N: Limit can be overwritten at runtime, with "N" being used as the default
 */
#ifndef TPP_ERROR_LIMIT
#define TPP_ERROR_LIMIT (-16)
#endif /* !TPP_ERROR_LIMIT */



/************************************************************************/
/* CONFIGURATION PROFILE                                                */
/************************************************************************/
#define TPP_PROFILE_MINIMAL 0 /* Disable everything, except dependencies of explicitly enabled features */
#define TPP_PROFILE_DEFAULT 1 /* Like "TPP_PROFILE_ALL", but only enable minimal multi-char tokens */
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
 *  1: Enabled
 *
 * (certain features only)
 * -1: Enable if possible (re-defined to `0' if unsupported) */
/************************************************************************/

/* Enable support for non-blocking I/O
 * Configure to "-1" to only enable compile-time support if supported by OS */
#ifndef TPP_HAVE_FILE_NONBLOCK
#define TPP_HAVE_FILE_NONBLOCK (TPP_PROFILE ? -1 : 0)
#endif /* !TPP_HAVE_FILE_NONBLOCK */

/* Enable support for:
 * - External hooks to inject unicode ctype information
 *   By default, only ASCII ctype information is built-in
 * - Automatic detection of utf-8, utf-8-bom, utf-16[le/be], utf-32[le/be] in input files
 *   NOTE: The Lexer assumes "utf-8" by default, unless it encounters an invalid utf-8
 *         byte sequence, at which point it will automatically downgrade to ASCII-only
 *         for the remainder of the relevant file */
#ifndef TPP_HAVE_UNICODE
#define TPP_HAVE_UNICODE 1
#endif /* !TPP_HAVE_UNICODE */

/* Provide a function "tpp_strerror()" to get a description of a given "tpp_errno" error code. */
#ifndef TPP_HAVE_STRERROR
#define TPP_HAVE_STRERROR TPP_PROFILE
#endif /* !TPP_HAVE_STRERROR */

/* Provide a function "tpp_strtokenid()" to get the API name of a (non-keyword) token ID */
#ifndef TPP_HAVE_STRTOKENID
#define TPP_HAVE_STRTOKENID TPP_PROFILE
#endif /* !TPP_HAVE_STRTOKENID */

/* Enable support for storing custom user-data in keywords. */
#ifndef TPP_HAVE_KEYWORD_USERDATA
#define TPP_HAVE_KEYWORD_USERDATA (TPP_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_HAVE_KEYWORD_USERDATA */

/* Enable support for runtime-configurable extensions */
#ifndef TPP_HAVE_EXTENSIONS
#define TPP_HAVE_EXTENSIONS TPP_PROFILE
#endif /* !TPP_HAVE_EXTENSIONS */

/* Enable support to push/pop the extension state */
#ifndef TPP_HAVE_EXTENSIONS_PUSH_POP
#define TPP_HAVE_EXTENSIONS_PUSH_POP TPP_HAVE_EXTENSIONS
#endif /* !TPP_HAVE_EXTENSIONS_PUSH_POP */

/* Support for: compiler warnings (else: behave as though all warnings were being suppressed) */
#ifndef TPP_HAVE_WARNINGS
#define TPP_HAVE_WARNINGS TPP_PROFILE
#endif /* !TPP_HAVE_WARNINGS */

/* Enable support to push/pop the warning state */
#ifndef TPP_HAVE_WARNINGS_PUSH_POP
#define TPP_HAVE_WARNINGS_PUSH_POP TPP_HAVE_WARNINGS
#endif /* !TPP_HAVE_WARNINGS_PUSH_POP */

/* Support for: tpp_warning_id_fromnumber() */
#ifndef TPP_HAVE_WARNING_NUMBERS
#define TPP_HAVE_WARNING_NUMBERS TPP_HAVE_WARNINGS
#endif /* !TPP_HAVE_WARNING_NUMBERS */

/* Support for: TPP_WSTATE_ERROR (else: only "TPP_WSTATE_FATAL" is available) */
#ifndef TPP_HAVE_WARNING_ERROR
#define TPP_HAVE_WARNING_ERROR (TPP_HAVE_WARNINGS && TPP_ERROR_LIMIT != 0)
#endif /* !TPP_HAVE_WARNING_ERROR */

/* Support for: TPP_WSTATE_SUPPRESS */
#ifndef TPP_HAVE_WARNING_SUPPRESS
#define TPP_HAVE_WARNING_SUPPRESS TPP_HAVE_WARNINGS
#endif /* !TPP_HAVE_WARNING_SUPPRESS */

/* Support for: TPP_WSTATE_DEFAULT */
#ifndef TPP_HAVE_WARNING_DEFAULT
#define TPP_HAVE_WARNING_DEFAULT TPP_HAVE_WARNINGS
#endif /* !TPP_HAVE_WARNING_DEFAULT */

/* Enable support for `TPP_FILE_IOFLAGS_NOCLOSE' */
#ifndef TPP_HAVE_FILE_NOCLOSE
#define TPP_HAVE_FILE_NOCLOSE (TPP_PROFILE == TPP_PROFILE_ALL)
#endif /* !TPP_HAVE_FILE_NOCLOSE */

/* Enable support for `TPP_FILE_IOFLAGS_NOKWD' */
#ifndef TPP_HAVE_FILE_NOKWD
#define TPP_HAVE_FILE_NOKWD TPP_PROFILE
#endif /* !TPP_HAVE_FILE_NOKWD */

/* Speed up calls to `tpp_file_lcinfo()' by caching the last-read
 * position and determining line/column information as a delta from
 * what was previously cached */
#ifndef TPP_HAVE_FILE_LC_CACHE
#define TPP_HAVE_FILE_LC_CACHE TPP_PROFILE
#endif /* !TPP_HAVE_FILE_LC_CACHE */



/* All TPP_HAVE_* options (those with "-f*"-style comments) can be configured as:
 * - TPP_CONF_1     : Compile-time enabled  (always on; no #pragma extension("-f...") / TPP_FEAT_* available)
 * - TPP_CONF_0     : Compile-time disabled
 * - TPP_CONF_EXT1  : Runtime-configurable (via #pragma extension("-f...") / TPP_EXT_*, default = true)
 * - TPP_CONF_EXT0  : Runtime-configurable (via #pragma extension("-f...") / TPP_EXT_*, default = false)
 * - TPP_CONF_FEAT1 : Runtime-configurable (via TPP_FEAT_*, default = true)
 * - TPP_CONF_FEAT0 : Runtime-configurable (via TPP_FEAT_*, default = false)
 *
 * In the case of "TPP_CONF_EXT1" / "TPP_CONF_EXT0", the extension's name will be the
 * "-f..." comment, but can be overwritten via #define TPP_EXTNAME_<name> "my-name":
 * >> #define TPP_HAVE_TRIGRAPHS    TPP_CONF_EXT1
 * >> #define TPP_EXTNAME_TRIGRAPHS "the-cool-trigraphs"
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

#ifndef TPP_COMMON_HAVE_TPP_TOK
#define TPP_COMMON_HAVE_TPP_TOK ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_COMMON_HAVE_TPP_TOK */
#ifndef TPP_COMMON_HAVE_TPP_TOK_1CHAR
#define TPP_COMMON_HAVE_TPP_TOK_1CHAR (TPP_HAVE_PROFILE_NOT_MINIMAL ? TPP_CONF_EXT0 : 0)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_1CHAR */
#ifndef TPP_COMMON_HAVE_TPP_TOK_SPACE
#define TPP_COMMON_HAVE_TPP_TOK_SPACE (TPP_HAVE_PROFILE_NOT_MINIMAL ? TPP_CONF_FEAT1 : 1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_SPACE */
#ifndef TPP_COMMON_HAVE_TPP_TOK_COMMENT
#define TPP_COMMON_HAVE_TPP_TOK_COMMENT ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_COMMON_HAVE_TPP_TOK : 0)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_COMMENT */
#ifndef TPP_COMMON_HAVE_TPP_TOK_GENERIC
#define TPP_COMMON_HAVE_TPP_TOK_GENERIC TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_GENERIC */
#ifndef TPP_COMMON_HAVE_TPP_TOK_C_STRING
#define TPP_COMMON_HAVE_TPP_TOK_C_STRING (TPP_HAVE_PROFILE_DEFAULT ? TPP_COMMON_HAVE_TPP_TOK : TPP_HAVE_PROFILE_C_LIKE)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_C_STRING */
#ifndef TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
#define TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_COMMON_HAVE_TPP_TOK : 0)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING */
#ifndef TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_C_TOKENS (TPP_HAVE_PROFILE_DEFAULT ? TPP_COMMON_HAVE_TPP_TOK : TPP_HAVE_PROFILE_C_LIKE)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_C_TOKENS */
#ifndef TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_COMMON_HAVE_TPP_TOK : (TPP_PROFILE == TPP_PROFILE_CXX))
#endif /* !TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS */
#ifndef TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_COMMON_HAVE_TPP_TOK : 0)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS */
#ifndef TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_COMMON_HAVE_TPP_TOK : 0)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS */

#ifndef TPP_COMMON_HAVE_CPP_DIRECTIVES_STD
#define TPP_COMMON_HAVE_CPP_DIRECTIVES_STD ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_HAVE_PROFILE_DEFAULT || TPP_HAVE_PROFILE_C_LIKE))
#endif /* !TPP_COMMON_HAVE_CPP_DIRECTIVES_STD */
#ifndef TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT
#define TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_HAVE_PROFILE_DEFAULT || TPP_HAVE_PROFILE_C_LIKE))
#endif /* !TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT */
/************************************************************************/
/************************************************************************/
/************************************************************************/




/************************************************************************/
/* TOKENS                                                               */
/************************************************************************/

/* Support for character aliases:
 * "??=" -> "#"
 * "??(" -> "["
 * "??/" -> "\"
 * "??)" -> "]"
 * "??'" -> "^"
 * "??<" -> "{"
 * "??!" -> "|"
 * "??>" -> "}"
 * "??-" -> "~"
 * "???" -> "?" */
#ifndef TPP_HAVE_TRIGRAPHS
#define TPP_HAVE_TRIGRAPHS (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) /* "-ftrigraphs" */
#endif /* !TPP_HAVE_TRIGRAPHS */

/* Support for token aliases:
 * "<%"   -> "{"
 * "<:"   -> "["
 * "%>"   -> "}"
 * ":>"   -> "]"
 * "%:"   -> "#"
 *
 * Extra, special exceptions/extensions:
 * "<::"  -> "<", "::"
 * "%:%:" -> "##" (TPP_TOK_POUND_POUND)
 *
 * @detect: #if __TPP_COUNT_TOKENS("%:") == 1 */
#ifndef TPP_HAVE_DIGRAPHS
#define TPP_HAVE_DIGRAPHS (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) /* "-fdigraphs" */
#endif /* !TPP_HAVE_DIGRAPHS */

/* Configures if line-feed tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()'
 * @detect: #if __TPP_COUNT_TOKENS("\n") != 0 */
#ifndef TPP_HAVE_TPP_TOK_LF
#define TPP_HAVE_TPP_TOK_LF TPP_COMMON_HAVE_TPP_TOK_SPACE /* "-ftok-lf" */
#endif /* !TPP_HAVE_TPP_TOK_LF */

/* Configures if whitespace tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()'
 * @detect: #if __TPP_COUNT_TOKENS(" ") != 0 */
#ifndef TPP_HAVE_TPP_TOK_SPACE
#define TPP_HAVE_TPP_TOK_SPACE TPP_COMMON_HAVE_TPP_TOK_SPACE /* "-ftok-space" */
#endif /* !TPP_HAVE_TPP_TOK_SPACE */

/* Configures if comment tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()'
 * @detect: #if __TPP_COUNT_TOKENS("// a b c") == 1 */
#ifndef TPP_HAVE_TPP_TOK_COMMENT
#define TPP_HAVE_TPP_TOK_COMMENT TPP_COMMON_HAVE_TPP_TOK_SPACE /* "-ftok-comment" */
#endif /* !TPP_HAVE_TPP_TOK_COMMENT */

/* Enable support for recognizing c++ comments: "// like this one!"
 * @detect: #if __TPP_COUNT_TOKENS("// a b c") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_COMMENT
#define TPP_HAVE_TPP_TOK_CXX_COMMENT ((TPP_PROFILE == TPP_PROFILE_DEFAULT || TPP_HAVE_PROFILE_C_LIKE) ? 1 : TPP_COMMON_HAVE_TPP_TOK_COMMENT) /* "-ftok-cxx-comment" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_COMMENT */

// Enable support for recognizing c comments: "/* like this one! */"
// @detect: #if __TPP_COUNT_TOKENS("/* a b c */") <= 1
#ifndef TPP_HAVE_TPP_TOK_C_COMMENT
#define TPP_HAVE_TPP_TOK_C_COMMENT ((TPP_PROFILE == TPP_PROFILE_DEFAULT || TPP_HAVE_PROFILE_C_LIKE) ? 1 : TPP_COMMON_HAVE_TPP_TOK_COMMENT) /* "-ftok-c-comment" */
#endif /* !TPP_HAVE_TPP_TOK_C_COMMENT */

/* Enable support for recognizing pascal comments: "(* like this one! *)"
 * @detect: #if __TPP_COUNT_TOKENS("(* a b c *)") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_PASCAL_COMMENT
#define TPP_HAVE_TPP_TOK_PASCAL_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-pascal-comment" */
#endif /* !TPP_HAVE_TPP_TOK_PASCAL_COMMENT */

/* Enable support for recognizing shell comments: "# like this one!"
 *
 * This still works in conjunction with "TPP_HAVE_CPP_DIRECTIVES", in
 * that unknown directives will simply be re-emit as shell comments,
 * and shell comments that don't appear at the start of lines are not
 * even processed as CPP directives.
 * @detect: #if __TPP_COUNT_TOKENS("# a b c") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_SHELL_COMMENT
#define TPP_HAVE_TPP_TOK_SHELL_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-shell-comment" */
#endif /* !TPP_HAVE_TPP_TOK_SHELL_COMMENT */

/* Enable support for recognizing ASM comments: "/ like this one!"
 * @detect: #if __TPP_COUNT_TOKENS("/ a b c") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_ASM_COMMENT
#define TPP_HAVE_TPP_TOK_ASM_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-asm-comment" */
#endif /* !TPP_HAVE_TPP_TOK_ASM_COMMENT */

/* Enable support for recognizing SQL comments: "-- like this one!"
 * @detect: #if __TPP_COUNT_TOKENS("-- a b c") <= 1 */
#ifndef TPP_HAVE_TPP_TOK_SQL_COMMENT
#define TPP_HAVE_TPP_TOK_SQL_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "-ftok-sql-comment" */
#endif /* !TPP_HAVE_TPP_TOK_SQL_COMMENT */

/* TODO: Support for deemon-style "@@doc-string" comments */

/************************************************************************/
/* Single-char tokens                                                   */
/************************************************************************/

/* "$" (treat as its own token, rather than as part of identifiers/keywords)
 * @detect: #if __TPP_COUNT_TOKENS("a$b") == 1 */
#ifndef TPP_HAVE_TPP_TOK_DOLLAR
#define TPP_HAVE_TPP_TOK_DOLLAR TPP_COMMON_HAVE_TPP_TOK_1CHAR /* "-ftok-dollar" */
#endif /* !TPP_HAVE_TPP_TOK_DOLLAR */

/************************************************************************/
/* Number tokens                                                        */
/************************************************************************/

/* 123
 * @detect: #if __TPP_COUNT_TOKENS("123") == 1 */
#ifndef TPP_HAVE_TPP_TOK_INT
#define TPP_HAVE_TPP_TOK_INT TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "-ftok-int" */
#endif /* !TPP_HAVE_TPP_TOK_INT */

/* 123.0
 * @detect: #if __TPP_COUNT_TOKENS("123.0") == 1 */
#ifndef TPP_HAVE_TPP_TOK_FLOAT
#define TPP_HAVE_TPP_TOK_FLOAT TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "-ftok-float" */
#endif /* !TPP_HAVE_TPP_TOK_FLOAT */

/************************************************************************/
/* String tokens                                                        */
/************************************************************************/

/* TODO: Support for sql-style '-string literals ('' is escape for ', and line-feeds are allowed) */
/* TODO: Support for sql-style "-string literals ("" is escape for ", and line-feeds are allowed) */
/* TODO: Support for sql-style E'foo'-string literals (line-feeds are allowed, and \-escape sequences are handled) */
/* TODO: Support for sql-style E"foo"-string literals (line-feeds are allowed, and \-escape sequences are handled) */

/* 'foo'
 * @detect: #if __TPP_COUNT_TOKENS("'foo'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_CHAR
#define TPP_HAVE_TPP_TOK_CHAR TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "-ftok-char" */
#endif /* !TPP_HAVE_TPP_TOK_CHAR */

/* "foo"
 * @detect: #if __TPP_COUNT_TOKENS('"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_STRING
#define TPP_HAVE_TPP_TOK_STRING TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "-ftok-string" */
#endif /* !TPP_HAVE_TPP_TOK_STRING */

/* R"AB(foo)AB"
 * @detect: #if __TPP_COUNT_TOKENS('R"AB(foo)AB"') == 1 && __TPP_STR_SIZE(R"AB(foo)AB") == 3 */
#ifndef TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-raw-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */

/* L"foo"  (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: LR"AB(foo)AB")
 * @detect: #if __TPP_COUNT_TOKENS('L"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-wide-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */

/* u8"foo" (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: u8R"AB(foo)AB")
 * @detect: #if __TPP_COUNT_TOKENS('u8"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf8-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */

/* u"foo"  (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: uR"AB(foo)AB")
 * @detect: #if __TPP_COUNT_TOKENS('u"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf16-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */

/* U"foo"  (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: UR"AB(foo)AB")
 * @detect: #if __TPP_COUNT_TOKENS('U"foo"') == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf32-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */

/* R'AB(f)AB'
 * @detect: #if __TPP_COUNT_TOKENS("R'AB(foo)AB'") == 1 && __TPP_STR_SIZE(R'AB(foo)AB') == 3 */
#ifndef TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-raw-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */

/* L'f'  (TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL: LR'AB(f)AB')
 * @detect: #if __TPP_COUNT_TOKENS("L'f'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-wide-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */

/* u8'f' (TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL: u8R'AB(f)AB')
 * @detect: #if __TPP_COUNT_TOKENS("u8'f'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf8-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */

/* u'f'  (TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL: uR'AB(f)AB')
 * @detect: #if __TPP_COUNT_TOKENS("u'f'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf16-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */

/* U'f'  (TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL: UR'AB(f)AB')
 * @detect: #if __TPP_COUNT_TOKENS("U'f'") == 1 */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_C_STRING /* "-ftok-cxx-utf32-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */

/* R"foo"  r"foo"
 * @detect: #if __TPP_COUNT_TOKENS('R"foo"') == 1 && __TPP_STR_SIZE(R"AB(foo)AB") == 9 */
#ifndef TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "-ftok-raw-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */

/* R'bar'  r'bar'
 * @detect: #if __TPP_COUNT_TOKENS('R"foo"') == 1 && __TPP_STR_SIZE(R'AB(foo)AB') == 9 */
#ifndef TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "-ftok-raw-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */

/* """foo"""
 * @detect: #if __TPP_COUNT_TOKENS('"""\n a\n b"""') == 1 */
#ifndef TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "-ftok-block-string-literal" */
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */

/* '''foo'''
 * @detect: #if __TPP_COUNT_TOKENS("'''\n a\n b'''") == 1 */
#ifndef TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "-ftok-block-char-literal" */
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */

/* Feature-flag: treat line-feeds like any regular character in string tokens:
 * - TPP_HAVE_TPP_TOK_STRING
 * - TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
 * - TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
 * - TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
 * - TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
 * - TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
 * - TPP_HAVE_TPP_TOK_CHAR
 * - TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
 * - TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
 * - TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
 * - TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
 * - TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
 *
 * When this flag is disabled, line-feeds in such string tokens will instead
 * terminate the string, and cause a "TPP_W_STRING_TERMINATED_BY_LINEFEED"
 * warning to be emitted.
 *
 * @detect: #if __TPP_COUNT_TOKENS("\"\n\"") == 1 */
#ifndef TPP_HAVE_STRING_ALLOW_MULTILINE
#define TPP_HAVE_STRING_ALLOW_MULTILINE (TPP_HAVE_PROFILE_NOT_MINIMAL ? TPP_CONF_FEAT0 : 0) /* "-fstring-allow-multiline" */
#endif /* !TPP_HAVE_STRING_ALLOW_MULTILINE */

/* Alter behavior of "TPP_HAVE_STRING_ALLOW_MULTILINE"
 * (that only takes effect when that feature is enabled):
 * - Multi-line strings continue to be allowed (does not affect behavior)
 * - When a multi-line string is encountered, emit a
 *   warning "TPP_W_STRING_CONTINUED_AFTER_LINEFEED"
 *
 * If this warning isn't wanted, it can be disabled by "-Wno-multiline-string" */
#ifndef TPP_HAVE_STRING_WARN_MULTILINE
#define TPP_HAVE_STRING_WARN_MULTILINE \
	(TPP_HAVE_STRING_ALLOW_MULTILINE && TPP_HAVE_WARNINGS)
#endif /* !TPP_HAVE_STRING_WARN_MULTILINE */

/************************************************************************/
/* Multi-char tokens                                                    */
/************************************************************************/

/* "<<"
 * @detect: #if __TPP_COUNT_TOKENS("<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-langle-langle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE */

/* ">>"
 * @detect: #if __TPP_COUNT_TOKENS(">>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-rangle-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE */

/* "=="
 * @detect: #if __TPP_COUNT_TOKENS("==") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-equal-equal" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL */

/* "!="
 * @detect: #if __TPP_COUNT_TOKENS("!=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-exclaim-equal" */
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */

/* ">="
 * @detect: #if __TPP_COUNT_TOKENS(">=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-rangle-equal" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_EQUAL */

/* "<="
 * @detect: #if __TPP_COUNT_TOKENS("<=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-langle-equal" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL */

/* "..."
 * @detect: #if __TPP_COUNT_TOKENS("...") == 1 */
#ifndef TPP_HAVE_TPP_TOK_DOT_DOT_DOT
#define TPP_HAVE_TPP_TOK_DOT_DOT_DOT (TPP_CONF_IS_RT(TPP_COMMON_HAVE_TPP_TOK_C_TOKENS) ? TPP_COMMON_HAVE_TPP_TOK_C_TOKENS : (TPP_COMMON_HAVE_TPP_TOK_C_TOKENS || TPP_HAVE_VA_ARGS_IN_MACROS || TPP_HAVE_NAMED_VARARGS_IN_MACROS)) /* "-ftok-dot-dot-dot" */
#endif /* !TPP_HAVE_TPP_TOK_DOT_DOT_DOT */

/* "+="
 * @detect: #if __TPP_COUNT_TOKENS("+=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PLUS_EQUAL
#define TPP_HAVE_TPP_TOK_PLUS_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-plus-equal" */
#endif /* !TPP_HAVE_TPP_TOK_PLUS_EQUAL */

/* "-="
 * @detect: #if __TPP_COUNT_TOKENS("-=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_EQUAL
#define TPP_HAVE_TPP_TOK_MINUS_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-minus-equal" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_EQUAL */

/* "*="
 * @detect: #if __TPP_COUNT_TOKENS("*=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_EQUAL
#define TPP_HAVE_TPP_TOK_STAR_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-star-equal" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_EQUAL */

/* "/="
 * @detect: #if __TPP_COUNT_TOKENS("/=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_SLASH_EQUAL
#define TPP_HAVE_TPP_TOK_SLASH_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-slash-equal" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_EQUAL */

/* "%="
 * @detect: #if __TPP_COUNT_TOKENS("%=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PERCENT_EQUAL
#define TPP_HAVE_TPP_TOK_PERCENT_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-percent-equal" */
#endif /* !TPP_HAVE_TPP_TOK_PERCENT_EQUAL */

/* "<<="
 * @detect: #if __TPP_COUNT_TOKENS("<<=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-langle-langle-equal" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */

/* ">>="
 * @detect: #if __TPP_COUNT_TOKENS(">>=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-rangle-rangle-equal" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */

/* "&="
 * @detect: #if __TPP_COUNT_TOKENS("&=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_AMP_EQUAL
#define TPP_HAVE_TPP_TOK_AMP_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-amp-equal" */
#endif /* !TPP_HAVE_TPP_TOK_AMP_EQUAL */

/* "|="
 * @detect: #if __TPP_COUNT_TOKENS("|=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PIPE_EQUAL
#define TPP_HAVE_TPP_TOK_PIPE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-pipe-equal" */
#endif /* !TPP_HAVE_TPP_TOK_PIPE_EQUAL */

/* "^="
 * @detect: #if __TPP_COUNT_TOKENS("^=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_HAT_EQUAL
#define TPP_HAVE_TPP_TOK_HAT_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-hat-equal" */
#endif /* !TPP_HAVE_TPP_TOK_HAT_EQUAL */

/* "//"  (WARNING: This token conflicts with TPP_HAVE_TPP_TOK_CXX_COMMENT)
 * @detect: #if __TPP_COUNT_TOKENS("//") == 1 */
#ifndef TPP_HAVE_TPP_TOK_SLASH_SLASH
#define TPP_HAVE_TPP_TOK_SLASH_SLASH TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-slash-slash" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_SLASH */

/* "//="  (WARNING: This token conflicts with TPP_HAVE_TPP_TOK_CXX_COMMENT)
 * @detect: #if __TPP_COUNT_TOKENS("//=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
#define TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-slash-slash-equal" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */

/* "**="
 * @detect: #if __TPP_COUNT_TOKENS("**=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
#define TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-star-star-equal" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */

/* "@="
 * @detect: #if __TPP_COUNT_TOKENS("@=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_AT_EQUAL
#define TPP_HAVE_TPP_TOK_AT_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-at-equal" */
#endif /* !TPP_HAVE_TPP_TOK_AT_EQUAL */

/* "##"
 * @detect: #if __TPP_COUNT_TOKENS("##") == 1 */
#ifndef TPP_HAVE_TPP_TOK_POUND_POUND
#define TPP_HAVE_TPP_TOK_POUND_POUND (TPP_CONF_IS_RT(TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS) ? TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS : (TPP_HAVE_GLUE_MACRO_ARGUMENT || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS)) /* "-ftok-pound-pound" */
#endif /* !TPP_HAVE_TPP_TOK_POUND_POUND */

/* "&&"
 * @detect: #if __TPP_COUNT_TOKENS("&&") == 1 */
#ifndef TPP_HAVE_TPP_TOK_AMP_AMP
#define TPP_HAVE_TPP_TOK_AMP_AMP TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-amp-amp" */
#endif /* !TPP_HAVE_TPP_TOK_AMP_AMP */

/* "||"
 * @detect: #if __TPP_COUNT_TOKENS("||") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PIPE_PIPE
#define TPP_HAVE_TPP_TOK_PIPE_PIPE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-pipe-pipe" */
#endif /* !TPP_HAVE_TPP_TOK_PIPE_PIPE */

/* "^^"
 * @detect: #if __TPP_COUNT_TOKENS("^^") == 1 */
#ifndef TPP_HAVE_TPP_TOK_HAT_HAT
#define TPP_HAVE_TPP_TOK_HAT_HAT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-hat-hat" */
#endif /* !TPP_HAVE_TPP_TOK_HAT_HAT */

/* "++"
 * @detect: #if __TPP_COUNT_TOKENS("++") == 1 */
#ifndef TPP_HAVE_TPP_TOK_PLUS_PLUS
#define TPP_HAVE_TPP_TOK_PLUS_PLUS TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-plus-plus" */
#endif /* !TPP_HAVE_TPP_TOK_PLUS_PLUS */

/* "--"  (WARNING: This token conflicts with TPP_HAVE_TPP_TOK_SQL_COMMENT)
 * @detect: #if __TPP_COUNT_TOKENS("--") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_MINUS
#define TPP_HAVE_TPP_TOK_MINUS_MINUS TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-minus-minus" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_MINUS */

/* "**"
 * @detect: #if __TPP_COUNT_TOKENS("**") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_STAR
#define TPP_HAVE_TPP_TOK_STAR_STAR TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-star-star" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_STAR */

/* "~~"
 * @detect: #if __TPP_COUNT_TOKENS("~~") == 1 */
#ifndef TPP_HAVE_TPP_TOK_TILDE_TILDE
#define TPP_HAVE_TPP_TOK_TILDE_TILDE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-tilde-tilde" */
#endif /* !TPP_HAVE_TPP_TOK_TILDE_TILDE */

/* "~="
 * @detect: #if __TPP_COUNT_TOKENS("~=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_TILDE_EQUAL
#define TPP_HAVE_TPP_TOK_TILDE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-tilde-equal" */
#endif /* !TPP_HAVE_TPP_TOK_TILDE_EQUAL */

/* "->"
 * @detect: #if __TPP_COUNT_TOKENS("->") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "-ftok-minus-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE */

/* ":="
 * @detect: #if __TPP_COUNT_TOKENS(":=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_COLON_EQUAL
#define TPP_HAVE_TPP_TOK_COLON_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-colon-equal" */
#endif /* !TPP_HAVE_TPP_TOK_COLON_EQUAL */

/* "::"
 * @detect: #if __TPP_COUNT_TOKENS("::") == 1 */
#ifndef TPP_HAVE_TPP_TOK_COLON_COLON
#define TPP_HAVE_TPP_TOK_COLON_COLON TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS /* "-ftok-colon-colon" */
#endif /* !TPP_HAVE_TPP_TOK_COLON_COLON */

/* "->*"
 * @detect: #if __TPP_COUNT_TOKENS("->*") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS /* "-ftok-minus-rangle-star" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */

/* ".*"
 * @detect: #if __TPP_COUNT_TOKENS(".*") == 1 */
#ifndef TPP_HAVE_TPP_TOK_DOT_STAR
#define TPP_HAVE_TPP_TOK_DOT_STAR TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS /* "-ftok-dot-star" */
#endif /* !TPP_HAVE_TPP_TOK_DOT_STAR */

/* ".."
 * @detect: #if __TPP_COUNT_TOKENS("..") == 1 */
#ifndef TPP_HAVE_TPP_TOK_DOT_DOT
#define TPP_HAVE_TPP_TOK_DOT_DOT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-dot-dot" */
#endif /* !TPP_HAVE_TPP_TOK_DOT_DOT */

/* "<>"
 * @detect: #if __TPP_COUNT_TOKENS("<>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_RANGLE */

/* "<<<"
 * @detect: #if __TPP_COUNT_TOKENS("<<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle-langle-langle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */

/* ">>>"
 * @detect: #if __TPP_COUNT_TOKENS(">>>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle-rangle-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */

/* "<<<="
 * @detect: #if __TPP_COUNT_TOKENS("<<<=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle-langle-langle-equal" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */

/* ">>>="
 * @detect: #if __TPP_COUNT_TOKENS(">>>=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle-rangle-rangle-equal" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */

/* "==="
 * @detect: #if __TPP_COUNT_TOKENS("===") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal-equal-equal" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */

/* "!=="
 * @detect: #if __TPP_COUNT_TOKENS("!==") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-exclaim-equal-equal" */
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */

/* "!!"
 * @detect: #if __TPP_COUNT_TOKENS("!!") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
#define TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-exclaim-exclaim" */
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */

/* "??"
 * @detect: #if __TPP_COUNT_TOKENS("??") == 1 */
#ifndef TPP_HAVE_TPP_TOK_QMARK_QMARK
#define TPP_HAVE_TPP_TOK_QMARK_QMARK TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-qmark-qmark" */
#endif /* !TPP_HAVE_TPP_TOK_QMARK_QMARK */

/* "?="
 * @detect: #if __TPP_COUNT_TOKENS("?=") == 1 */
#ifndef TPP_HAVE_TPP_TOK_QMARK_EQUAL
#define TPP_HAVE_TPP_TOK_QMARK_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-qmark-equal" */
#endif /* !TPP_HAVE_TPP_TOK_QMARK_EQUAL */

/* "><"
 * @detect: #if __TPP_COUNT_TOKENS("><") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle-langle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_LANGLE */

/* "=+"
 * @detect: #if __TPP_COUNT_TOKENS("=+") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PLUS
#define TPP_HAVE_TPP_TOK_EQUAL_PLUS TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-plus" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PLUS */

/* "=-"
 * @detect: #if __TPP_COUNT_TOKENS("=-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_MINUS
#define TPP_HAVE_TPP_TOK_EQUAL_MINUS TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-minus" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_MINUS */

/* "=*"
 * @detect: #if __TPP_COUNT_TOKENS("=*") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_STAR
#define TPP_HAVE_TPP_TOK_EQUAL_STAR TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-star" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_STAR */

/* "=**"
 * @detect: #if __TPP_COUNT_TOKENS("=**") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
#define TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-star-star" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */

/* "=/"
 * @detect: #if __TPP_COUNT_TOKENS("=/") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_SLASH
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-slash" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_SLASH */

/* "=//"  (WARNING: This token conflicts with TPP_HAVE_TPP_TOK_CXX_COMMENT)
 * @detect: #if __TPP_COUNT_TOKENS("=//") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-slash-slash" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */

/* "=%"
 * @detect: #if __TPP_COUNT_TOKENS("=%") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PERCENT
#define TPP_HAVE_TPP_TOK_EQUAL_PERCENT TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-percent" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PERCENT */

/* "=&"
 * @detect: #if __TPP_COUNT_TOKENS("=&") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_AMP
#define TPP_HAVE_TPP_TOK_EQUAL_AMP TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-amp" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_AMP */

/* "=|"
 * @detect: #if __TPP_COUNT_TOKENS("=|") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PIPE
#define TPP_HAVE_TPP_TOK_EQUAL_PIPE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-pipe" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PIPE */

/* "=^"
 * @detect: #if __TPP_COUNT_TOKENS("=^") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_HAT
#define TPP_HAVE_TPP_TOK_EQUAL_HAT TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-hat" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_HAT */

/* "=<"
 * @detect: #if __TPP_COUNT_TOKENS("=<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_LANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-langle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE */

/* "=<<"
 * @detect: #if __TPP_COUNT_TOKENS("=<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-langle-langle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */

/* "=<<<"
 * @detect: #if __TPP_COUNT_TOKENS("=<<<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-langle-langle-langle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */

/* "=>"
 * @detect: #if __TPP_COUNT_TOKENS("=>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_RANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE */

/* "=>>"
 * @detect: #if __TPP_COUNT_TOKENS("=>>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-rangle-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */

/* "=>>>"
 * @detect: #if __TPP_COUNT_TOKENS("=>>>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-rangle-rangle-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */

/* "=@"
 * @detect: #if __TPP_COUNT_TOKENS("=@") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_AT
#define TPP_HAVE_TPP_TOK_EQUAL_AT TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-at" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_AT */

/* "=~"
 * @detect: #if __TPP_COUNT_TOKENS("=~") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_TILDE
#define TPP_HAVE_TPP_TOK_EQUAL_TILDE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-tilde" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_TILDE */

/* "=:"
 * @detect: #if __TPP_COUNT_TOKENS("=:") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_COLON
#define TPP_HAVE_TPP_TOK_EQUAL_COLON TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-colon" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_COLON */

/* "=!"
 * @detect: #if __TPP_COUNT_TOKENS("=!") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
#define TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-exclaim" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */

/* "==!"
 * @detect: #if __TPP_COUNT_TOKENS("==!") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-equal-equal-exclaim" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */

/* "=?"
 * @detect: #if __TPP_COUNT_TOKENS("=?") == 1 */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_QMARK
#define TPP_HAVE_TPP_TOK_EQUAL_QMARK TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-equal-qmark" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_QMARK */

/* "<-"
 * @detect: #if __TPP_COUNT_TOKENS("<-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_MINUS
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-langle-minus" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS */

/* "*<-"
 * @detect: #if __TPP_COUNT_TOKENS("*<-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
#define TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-star-langle-minus" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */

/* "*."
 * @detect: #if __TPP_COUNT_TOKENS("*.") == 1 */
#ifndef TPP_HAVE_TPP_TOK_STAR_DOT
#define TPP_HAVE_TPP_TOK_STAR_DOT TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "-ftok-star-dot" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_DOT */

/* "-<"
 * @detect: #if __TPP_COUNT_TOKENS("-<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_MINUS_LANGLE
#define TPP_HAVE_TPP_TOK_MINUS_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-minus-langle" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_LANGLE */

/* ">-"
 * @detect: #if __TPP_COUNT_TOKENS(">-") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_MINUS
#define TPP_HAVE_TPP_TOK_RANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle-minus" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_MINUS */

/* "<=>"
 * @detect: #if __TPP_COUNT_TOKENS("<=>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle-equal-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */

/* "<->"
 * @detect: #if __TPP_COUNT_TOKENS("<->") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle-minus-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */

/* ">=<"
 * @detect: #if __TPP_COUNT_TOKENS(">=<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle-equal-langle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */

/* ">-<"
 * @detect: #if __TPP_COUNT_TOKENS(">-<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle-minus-langle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */

/* "<=<"
 * @detect: #if __TPP_COUNT_TOKENS("<=<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle-equal-langle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */

/* "<-<"
 * @detect: #if __TPP_COUNT_TOKENS("<-<") == 1 */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-langle-minus-langle" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */

/* ">=>"
 * @detect: #if __TPP_COUNT_TOKENS(">=>") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle-equal-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */

/* ">->"
 * @detect: #if __TPP_COUNT_TOKENS(">->") == 1 */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "-ftok-rangle-minus-rangle" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */


#undef TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
#if (TPP_HAVE_TPP_TOK_C_COMMENT || \
     TPP_HAVE_TPP_TOK_PASCAL_COMMENT)
#define TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE
#if (TPP_HAVE_TPP_TOK_CXX_COMMENT ||   \
     TPP_HAVE_TPP_TOK_SHELL_COMMENT || \
     TPP_HAVE_TPP_TOK_ASM_COMMENT ||   \
     TPP_HAVE_TPP_TOK_SQL_COMMENT)
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
#undef TPP_HAVE_TPP_TOK_STRINGLIKE_SQUOTE
#if (TPP_HAVE_TPP_TOK_CHAR ||                     \
     TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL ||     \
     TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||   \
     TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL ||         \
     TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL)
#define TPP_HAVE_TPP_TOK_STRINGLIKE_SQUOTE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_STRINGLIKE_SQUOTE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_STRINGLIKE_DQUOTE
#if (TPP_HAVE_TPP_TOK_STRING ||                     \
     TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL ||     \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||    \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL ||   \
     TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||         \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL)
#define TPP_HAVE_TPP_TOK_STRINGLIKE_DQUOTE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_STRINGLIKE_DQUOTE 0
#endif /* !... */
#undef TPP_HAVE_TPP_TOK_STRINGLIKE
#if (TPP_HAVE_TPP_TOK_STRINGLIKE_SQUOTE || \
     TPP_HAVE_TPP_TOK_STRINGLIKE_DQUOTE)
#define TPP_HAVE_TPP_TOK_STRINGLIKE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_STRINGLIKE 0
#endif /* !... */
#undef TPP_HAVE_STRING_ESCAPE
#if (TPP_HAVE_TPP_TOK_STRING ||                   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
     TPP_HAVE_TPP_TOK_CHAR ||                     \
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
/************************************************************************/
/************************************************************************/



/************************************************************************/
/* PREPROCESSOR FEATURES                                                */
/************************************************************************/

/* Support for \-escaped line continuation: when a line ends with a \ character
 * that is immediately (but see TPP_HAVE_BSE_WHITESPACE) followed by a linefeed,
 * that that linefeed is never yielded, and a potential multi-character token is
 * continued:
 * >> "foo\\\nbar" -- Produces a single token "foobar"
 * >> "+\\\n=" -- Produces a single token "+="
 *
 * This does NOT affect the line-continuation features of C++ // comments,
 * and multi-line macro definitions.
 * @detect: #if __TPP_COUNT_TOKENS("a\\\nb") == 1 */
#ifndef TPP_HAVE_BSE
#define TPP_HAVE_BSE (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) /* "-fbse" */
#endif /* !TPP_HAVE_BSE */

/* Extension to `TPP_HAVE_BSE': the \ backslash is allowed to be followed by extra
 * whitespace preceding the actual linefeed
 *
 * This DOES affect the line-continuation features of C++ // comments, and
 * multi-line macro definitions.
 * @detect: #if __TPP_COUNT_TOKENS("a\\ \nb") == 1 */
#ifndef TPP_HAVE_BSE_WHITESPACE
#define TPP_HAVE_BSE_WHITESPACE (TPP_HAVE_BSE ? TPP_CONF_EXT1 : 0) /* "-fbse-whitespace" */
#endif /* !TPP_HAVE_BSE_WHITESPACE */

/* Support for \uABCD and \U01234567 in identifier names (will be
 * replaced with effective UTF-8 encodings when translated to keywords)
 * @detect: #if __TPP_COUNT_TOKENS("a\\u1234b") == 1 */
#ifndef TPP_HAVE_ESCAPE_IN_IDENTIFIERS
#define TPP_HAVE_ESCAPE_IN_IDENTIFIERS (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_C_LIKE) /* "-fescape-in-identifiers" */
#endif /* !TPP_HAVE_ESCAPE_IN_IDENTIFIERS */

/* Support for "\e" (for U+001B) escape sequences
 * @detect: TODO */
#ifndef TPP_HAVE_ESCAPE_E_IN_STRINGS
#define TPP_HAVE_ESCAPE_E_IN_STRINGS (TPP_HAVE_STRING_ESCAPE ? TPP_CONF_EXT1: 0) /* "-fescape-e-in-strings" */
#endif /* !TPP_HAVE_ESCAPE_E_IN_STRINGS */

/* Support for "\s" (for U+0020) escape sequences
 * @detect: TODO */
#ifndef TPP_HAVE_ESCAPE_S_IN_STRINGS
#define TPP_HAVE_ESCAPE_S_IN_STRINGS (TPP_HAVE_STRING_ESCAPE ? TPP_CONF_EXT1 : 0) /* "-fescape-s-in-strings" */
#endif /* !TPP_HAVE_ESCAPE_S_IN_STRINGS */

/* Specifies if *any* CPP directives are supported
 * @detect: N/A */
#ifndef TPP_HAVE_CPP_DIRECTIVES
#define TPP_HAVE_CPP_DIRECTIVES ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : (TPP_HAVE_PROFILE_DEFAULT ? TPP_CONF_FEAT1 : TPP_HAVE_PROFILE_NOT_MINIMAL)) /* "-fcpp-directives" */
#endif /* !TPP_HAVE_CPP_DIRECTIVES */

/* Support for C-style macros
 * @detect: N/A */
#ifndef TPP_HAVE_CPP_MACROS
#define TPP_HAVE_CPP_MACROS (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fcpp-macros" */
#endif /* !TPP_HAVE_CPP_MACROS */

/* Support for builtin C-style macros (require "TPP_HAVE_CPP_MACROS" to be enabled, too)
 * @detect: N/A */
#ifndef TPP_HAVE_CPP_BUILTIN_MACROS
#define TPP_HAVE_CPP_BUILTIN_MACROS (TPP_HAVE_CPP_MACROS ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fcpp-builtin-macros" */
#endif /* !TPP_HAVE_CPP_BUILTIN_MACROS */

/* Support for: #!foobar  (comments)
 * @detect: #if __has_known_extension("-fshebang-directives") */
#ifndef TPP_HAVE_CPP_EXCLAIM
#define TPP_HAVE_CPP_EXCLAIM (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fshebang-directives" */
#endif /* !TPP_HAVE_CPP_EXCLAIM */

/* Support for: #  (blank line)
 * @detect: TODO */
#ifndef TPP_HAVE_CPP_BLANK
#define TPP_HAVE_CPP_BLANK (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fblank-directives" */
#endif /* !TPP_HAVE_CPP_BLANK */

/* Support for: # 42 ...  (similar to #line)
 * @detect: TODO */
#ifndef TPP_HAVE_CPP_DIGIT_LINE
#define TPP_HAVE_CPP_DIGIT_LINE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fdigit-directives" */
#endif /* !TPP_HAVE_CPP_DIGIT_LINE */

/* Support for: #line ...
 * @detect: TODO */
#ifndef TPP_HAVE_CPP_LINE
#define TPP_HAVE_CPP_LINE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fline-directives" */
#endif /* !TPP_HAVE_CPP_LINE */

/* Support for: #include
 * @detect: #if __has_known_extension("-finclude-directives") */
#ifndef TPP_HAVE_CPP_INCLUDE
#define TPP_HAVE_CPP_INCLUDE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-finclude-directives" */
#endif /* !TPP_HAVE_CPP_INCLUDE */

/* Support for: #include_next
 * @detect: #if __has_known_extension("-finclude-next-directives") */
#ifndef TPP_HAVE_CPP_INCLUDE_NEXT
#define TPP_HAVE_CPP_INCLUDE_NEXT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-finclude-next-directives" */
#endif /* !TPP_HAVE_CPP_INCLUDE_NEXT */

/* Support for: #import
 * @detect: #if __has_known_extension("-fimport-directives") */
#ifndef TPP_HAVE_CPP_IMPORT
#define TPP_HAVE_CPP_IMPORT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fimport-directives" */
#endif /* !TPP_HAVE_CPP_IMPORT */

/* Support for: #if, #ifdef, #ifndef, #elif, #elifdef, #elifndef, #else, #endif
 * @detect: N/A */
#ifndef TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_HAVE_CPP_IF_ELSE_ENDIF ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fif-directives" */
#endif /* !TPP_HAVE_CPP_IF_ELSE_ENDIF */

/* Support for: #define, #undef
 * @detect: N/A */
#ifndef TPP_HAVE_CPP_DEFINE
#define TPP_HAVE_CPP_DEFINE ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fdefine-directives" */
#endif /* !TPP_HAVE_CPP_DEFINE */

/* Support for: #assert, #unassert
 * @detect: #if __has_known_extension("-fassertions") */
#ifndef TPP_HAVE_CPP_ASSERT
#define TPP_HAVE_CPP_ASSERT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fassertions" */
#endif /* !TPP_HAVE_CPP_ASSERT */

/* Support for: #error
 * @detect: #if __has_known_extension("-ferror-directives") */
#ifndef TPP_HAVE_CPP_ERROR
#define TPP_HAVE_CPP_ERROR (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-ferror-directives" */
#endif /* !TPP_HAVE_CPP_ERROR */

/* Support for: #warning
 * @detect: #if __has_known_extension("-fwarning-directives") */
#ifndef TPP_HAVE_CPP_WARNING
#define TPP_HAVE_CPP_WARNING (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fwarning-directives" */
#endif /* !TPP_HAVE_CPP_WARNING */

/* Support for: #ident, #sccs
 * @detect: #if __has_known_extension("-fident-directives") */
#ifndef TPP_HAVE_CPP_IDENT_SCCS
#define TPP_HAVE_CPP_IDENT_SCCS (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_EXT : 0) /* "-fident-directives" */
#endif /* !TPP_HAVE_CPP_IDENT_SCCS */

/* Support for: #pragma
 * @detect: TODO */
#ifndef TPP_HAVE_CPP_PRAGMA
#define TPP_HAVE_CPP_PRAGMA (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fpragma-directives" */
#endif /* !TPP_HAVE_CPP_PRAGMA */

/* Support for: #embed
 * @detect: TODO */
#ifndef TPP_HAVE_CPP_EMBED
#define TPP_HAVE_CPP_EMBED (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-fembed-directives" */
#endif /* !TPP_HAVE_CPP_EMBED */

/* Support for: _Pragma("foo")
 * @detect: #ifdef _Pragma */
#ifndef TPP_HAVE_MACRO__Pragma
#define TPP_HAVE_MACRO__Pragma (TPP_HAVE_CPP_BUILTIN_MACROS ? TPP_COMMON_HAVE_CPP_DIRECTIVES_STD : 0) /* "-f_Pragma" */
#endif /* !TPP_HAVE_MACRO__Pragma */

/* Support for: __pragma(foo)
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


/* Support for clang __has_attribute
 * @detect: #ifdef __has_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_attribute
#define TPP_HAVE_CLANG_MACRO___has_attribute (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_attribute */

/* Support for clang __has_builtin
 * @detect: #ifdef __has_builtin */
#ifndef TPP_HAVE_CLANG_MACRO___has_builtin
#define TPP_HAVE_CLANG_MACRO___has_builtin (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_builtin" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_builtin */

/* Support for clang __has_cpp_attribute
 * @detect: #ifdef __has_cpp_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_cpp_attribute
#define TPP_HAVE_CLANG_MACRO___has_cpp_attribute (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_cpp_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_cpp_attribute */

/* Support for clang __has_declspec_attribute
 * @detect: #ifdef __has_declspec_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_declspec_attribute
#define TPP_HAVE_CLANG_MACRO___has_declspec_attribute (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_declspec_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_declspec_attribute */

/* Support for clang __has_extension
 * @detect: #ifdef __has_extension */
#ifndef TPP_HAVE_CLANG_MACRO___has_extension
#define TPP_HAVE_CLANG_MACRO___has_extension (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_extension" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_extension */

/* Support for clang __has_feature
 * @detect: #ifdef __has_feature */
#ifndef TPP_HAVE_CLANG_MACRO___has_feature
#define TPP_HAVE_CLANG_MACRO___has_feature (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_feature" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_feature */

/* Support for clang __has_c_attribute
 * @detect: #ifdef __has_c_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_c_attribute
#define TPP_HAVE_CLANG_MACRO___has_c_attribute (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fclang-__has_c_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_c_attribute */

/* When enabled, clang's __has_feature() also expands to "1" when __has_extension() would.
 * @detect: #if __has_known_extension("-fclang-extensions-are-features") */
#ifndef TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES
#define TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES ((TPP_HAVE_CLANG_MACRO___has_extension && TPP_HAVE_CLANG_MACRO___has_feature) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : 1) : 0) /* "-fclang-extensions-are-features" */
#endif /* !TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES */

/* Support for clang __is_identifier
 * @detect: #ifdef __is_identifier */
#ifndef TPP_HAVE_MACRO___is_identifier
#define TPP_HAVE_MACRO___is_identifier (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__is_identifier" */
#endif /* !TPP_HAVE_MACRO___is_identifier */

/* Support for TPP's __is_deprecated
 * @detect: #ifdef __is_deprecated */
#ifndef TPP_HAVE_MACRO___is_deprecated
#define TPP_HAVE_MACRO___is_deprecated (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__is_deprecated" */
#endif /* !TPP_HAVE_MACRO___is_deprecated */

/* Support for TPP's __is_poisoned
 * @detect: #ifdef __is_poisoned */
#ifndef TPP_HAVE_MACRO___is_poisoned
#define TPP_HAVE_MACRO___is_poisoned (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__is_poisoned" */
#endif /* !TPP_HAVE_MACRO___is_poisoned */

/* Support for TPP's __has_extension
 * @detect: #ifdef __has_extension */
#ifndef TPP_HAVE_MACRO___has_extension
#define TPP_HAVE_MACRO___has_extension ((TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_extension" */
#endif /* !TPP_HAVE_MACRO___has_extension */

/* Support for TPP's __has_known_extension
 * @detect: #ifdef __has_known_extension */
#ifndef TPP_HAVE_MACRO___has_known_extension
#define TPP_HAVE_MACRO___has_known_extension ((TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_known_extension" */
#endif /* !TPP_HAVE_MACRO___has_known_extension */

/* Support for TPP's __has_warning
 * @detect: #ifdef __has_warning */
#ifndef TPP_HAVE_MACRO___has_warning
#define TPP_HAVE_MACRO___has_warning ((TPP_HAVE_WARNINGS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_warning" */
#endif /* !TPP_HAVE_MACRO___has_warning */

/* Support for TPP's __has_known_warning
 * @detect: #ifdef __has_known_warning */
#ifndef TPP_HAVE_MACRO___has_known_warning
#define TPP_HAVE_MACRO___has_known_warning ((TPP_HAVE_WARNINGS && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_known_warning" */
#endif /* !TPP_HAVE_MACRO___has_known_warning */

/* Support for clang __has_include
 * @detect: #ifdef __has_include */
#ifndef TPP_HAVE_MACRO___has_include
#define TPP_HAVE_MACRO___has_include (((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_IMPORT) && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_include" */
#endif /* !TPP_HAVE_MACRO___has_include */

/* Support for clang __has_include_next
 * @detect: #ifdef __has_include_next */
#ifndef TPP_HAVE_MACRO___has_include_next
#define TPP_HAVE_MACRO___has_include_next ((TPP_HAVE_CPP_INCLUDE_NEXT && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_include_next" */
#endif /* !TPP_HAVE_MACRO___has_include_next */

/* Support for clang __has_embed
 * @detect: #ifdef __has_embed */
#ifndef TPP_HAVE_MACRO___has_embed
#define TPP_HAVE_MACRO___has_embed ((TPP_HAVE_CPP_EMBED && TPP_HAVE_CPP_BUILTIN_MACROS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__has_embed" */
#endif /* !TPP_HAVE_MACRO___has_embed */

/* __FILE__
 * @detect: #ifdef __FILE__ */
#ifndef TPP_HAVE_MACRO___FILE__
#define TPP_HAVE_MACRO___FILE__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__FILE__" */
#endif /* !TPP_HAVE_MACRO___FILE__ */

/* __LINE__
 * @detect: #ifdef __LINE__ */
#ifndef TPP_HAVE_MACRO___LINE__
#define TPP_HAVE_MACRO___LINE__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__LINE__" */
#endif /* !TPP_HAVE_MACRO___LINE__ */

/* __TIME__
 * @detect: #ifdef __TIME__ */
#ifndef TPP_HAVE_MACRO___TIME__
#define TPP_HAVE_MACRO___TIME__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__TIME__" */
#endif /* !TPP_HAVE_MACRO___TIME__ */

/* __DATE__
 * @detect: #ifdef __DATE__ */
#ifndef TPP_HAVE_MACRO___DATE__
#define TPP_HAVE_MACRO___DATE__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-f__DATE__" */
#endif /* !TPP_HAVE_MACRO___DATE__ */

/* __COLUMN__
 * @detect: #ifdef __COLUMN__ */
#ifndef TPP_HAVE_MACRO___COLUMN__
#define TPP_HAVE_MACRO___COLUMN__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fcolumn-macro" */
#endif /* !TPP_HAVE_MACRO___COLUMN__ */

/* __BASE_FILE__
 * @detect: #ifdef __BASE_FILE__ */
#ifndef TPP_HAVE_MACRO___BASE_FILE__
#define TPP_HAVE_MACRO___BASE_FILE__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fbasefile-macro" */
#endif /* !TPP_HAVE_MACRO___BASE_FILE__ */

/* __INCLUDE_LEVEL__
 * @detect: #ifdef __INCLUDE_LEVEL__ */
#ifndef TPP_HAVE_MACRO___INCLUDE_LEVEL__
#define TPP_HAVE_MACRO___INCLUDE_LEVEL__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-finclude-level-macro" */
#endif /* !TPP_HAVE_MACRO___INCLUDE_LEVEL__ */

/* __INCLUDE_DEPTH__
 * @detect: #ifdef __INCLUDE_DEPTH__ */
#ifndef TPP_HAVE_MACRO___INCLUDE_DEPTH__
#define TPP_HAVE_MACRO___INCLUDE_DEPTH__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-finclude-depth-macro" */
#endif /* !TPP_HAVE_MACRO___INCLUDE_DEPTH__ */

/* __COUNTER__
 * @detect: #ifdef __COUNTER__ */
#ifndef TPP_HAVE_MACRO___COUNTER__
#define TPP_HAVE_MACRO___COUNTER__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fcounter-macro" */
#endif /* !TPP_HAVE_MACRO___COUNTER__ */

/* __TIMESTAMP__
 * @detect: #ifdef __TIMESTAMP__ */
#ifndef TPP_HAVE_MACRO___TIMESTAMP__
#define TPP_HAVE_MACRO___TIMESTAMP__ (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftimestamp-macro" */
#endif /* !TPP_HAVE_MACRO___TIMESTAMP__ */

/* __DATE_DAY__, __DATE_WDAY__, __DATE_YDAY__, __DATE_MONTH__, __DATE_YEAR__
 * @detect: #ifdef __DATE_DAY__, ... */
#ifndef TPP_HAVE_NUMERIC_DATE_MACROS
#define TPP_HAVE_NUMERIC_DATE_MACROS (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fnumeric-date-macros" */
#endif /* !TPP_HAVE_NUMERIC_DATE_MACROS */

/* __TIME_SEC__, __TIME_MIN__, __TIME_HOUR__
 * @detect: #ifdef __TIME_SEC__, ... */
#ifndef TPP_HAVE_NUMERIC_TIME_MACROS
#define TPP_HAVE_NUMERIC_TIME_MACROS (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fnumeric-time-macros" */
#endif /* !TPP_HAVE_NUMERIC_TIME_MACROS */

/* __TPP_EVAL
 * @detect: #ifdef __TPP_EVAL */
#ifndef TPP_HAVE_MACRO___TPP_EVAL
#define TPP_HAVE_MACRO___TPP_EVAL (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-eval-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */

/* __TPP_UNIQUE
 * @detect: #ifdef __TPP_UNIQUE */
#ifndef TPP_HAVE_MACRO___TPP_UNIQUE
#define TPP_HAVE_MACRO___TPP_UNIQUE (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-unique-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_UNIQUE */

/* __TPP_LOAD_FILE
 * @detect: #ifdef __TPP_LOAD_FILE */
#ifndef TPP_HAVE_MACRO___TPP_LOAD_FILE
#define TPP_HAVE_MACRO___TPP_LOAD_FILE (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-load-file-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_LOAD_FILE */

/* __TPP_COUNTER
 * @detect: #ifdef __TPP_COUNTER */
#ifndef TPP_HAVE_MACRO___TPP_COUNTER
#define TPP_HAVE_MACRO___TPP_COUNTER (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-counter-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */

/* __TPP_RANDOM
 * @detect: #ifdef __TPP_RANDOM */
#ifndef TPP_HAVE_MACRO___TPP_RANDOM
#define TPP_HAVE_MACRO___TPP_RANDOM (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-random-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_RANDOM */

/* __TPP_STR_DECOMPILE
 * @detect: #ifdef __TPP_STR_DECOMPILE */
#ifndef TPP_HAVE_MACRO___TPP_STR_DECOMPILE
#define TPP_HAVE_MACRO___TPP_STR_DECOMPILE (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-str-decompile-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_DECOMPILE */

/* __TPP_STR_SUBSTR
 * @detect: #ifdef __TPP_STR_SUBSTR */
#ifndef TPP_HAVE_MACRO___TPP_STR_SUBSTR
#define TPP_HAVE_MACRO___TPP_STR_SUBSTR (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-str-substr-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SUBSTR */

/* __TPP_STR_PACK
 * @detect: #ifdef __TPP_STR_PACK */
#ifndef TPP_HAVE_MACRO___TPP_STR_PACK
#define TPP_HAVE_MACRO___TPP_STR_PACK (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-str-pack-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_PACK */

/* __TPP_STR_SIZE
 * @detect: #ifdef __TPP_STR_SIZE */
#ifndef TPP_HAVE_MACRO___TPP_STR_SIZE
#define TPP_HAVE_MACRO___TPP_STR_SIZE (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-str-size-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SIZE */

/* __TPP_COUNT_TOKENS
 * @detect: #ifdef __TPP_COUNT_TOKENS */
#ifndef TPP_HAVE_MACRO___TPP_COUNT_TOKENS
#define TPP_HAVE_MACRO___TPP_COUNT_TOKENS (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-count-tokens-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_COUNT_TOKENS */

/* __TPP_IDENTIFIER
 * @detect: #ifdef __TPP_IDENTIFIER */
#ifndef TPP_HAVE_MACRO___TPP_IDENTIFIER
#define TPP_HAVE_MACRO___TPP_IDENTIFIER (TPP_HAVE_CPP_BUILTIN_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-ftpp-identifier-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_IDENTIFIER */


/* Support for: #define point<T> ...
 * @detect: #if __has_known_extension("-falternative-macro-parenthesis") */
#ifndef TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS (TPP_HAVE_CPP_MACROS ? TPP_CONF_EXT1 : 0) /* "-falternative-macro-parenthesis" */
#endif /* !TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

/* Support for retaining whitespace around macro arguments
 * @detect: #if __has_known_extension("-fmacro-argument-whitespace") */
#ifndef TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
#define TPP_HAVE_MACRO_ARGUMENT_WHITESPACE ((TPP_HAVE_CPP_MACROS && TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT0 : 0) /* "-fmacro-argument-whitespace" */
#endif /* !TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */

/* Support for: #pragma extension("-fmacro-recursion")
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION*
 * @detect: #if __has_known_extension("-fmacro-recursion") */
#ifndef TPP_HAVE_MACRO_RECURSION
#define TPP_HAVE_MACRO_RECURSION ((TPP_HAVE_CPP_MACROS && TPP_HAVE_PROFILE_NOT_MINIMAL) ? TPP_CONF_EXT0 : 0) /* "-fmacro-recursion" */
#endif /* !TPP_HAVE_MACRO_RECURSION */

// Support for traditional macro expansion:
// #if __has_extension("-ftraditional-macro")
// #define STR(x)    "x"
// #define CAT(a, b) a/**/b
// #else
// #define STR(x)    #x
// #define CAT(a, b) a##b
// #endif
// NOTE: affects behavior of macros at the *TIME OF DEFINITION*
// @detect: #if __has_known_extension("-ftraditional-macro")
#ifndef TPP_HAVE_TRADITIONAL_MACROS
#define TPP_HAVE_TRADITIONAL_MACROS ((TPP_HAVE_CPP_MACROS && TPP_HAVE_PROFILE_NOT_MINIMAL) ? TPP_CONF_EXT0 : 0) /* "-ftraditional-macro" */
#endif /* !TPP_HAVE_TRADITIONAL_MACROS */

/* Support for: #define printf(format, args...) args
 * @detect: #if __has_known_extension("-fnamed-varargs-in-macros") */
#ifndef TPP_HAVE_NAMED_VARARGS_IN_MACROS
#define TPP_HAVE_NAMED_VARARGS_IN_MACROS (TPP_HAVE_CPP_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fnamed-varargs-in-macros" */
#endif /* !TPP_HAVE_NAMED_VARARGS_IN_MACROS */

/* Support for: #define printf(format, ...) __VA_ARGS__
 * @detect: #if __has_known_extension("-fva-args-in-macros") */
#ifndef TPP_HAVE_VA_ARGS_IN_MACROS
#define TPP_HAVE_VA_ARGS_IN_MACROS (TPP_HAVE_CPP_MACROS ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fva-args-in-macros" */
#endif /* !TPP_HAVE_VA_ARGS_IN_MACROS */

/* Support for: #define printf(format, ...) fprintf(stderr, format __VA_COMMA__ __VA_ARGS__)
 * @detect: #define test1(a, b, ...) __VA_ARGS__+0
 *          #define test2(...) test1(__VA_COMMA__ 0, 1)
 *          #if test2(~) */
#ifndef TPP_HAVE_VA_COMMA_IN_MACROS
#define TPP_HAVE_VA_COMMA_IN_MACROS ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fva-comma-in-macros" */
#endif /* !TPP_HAVE_VA_COMMA_IN_MACROS */

/* Support for: #define printf(format, ...) fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__)
 * @detect: #define test1(a, b, ...) __VA_ARGS__+0
 *          #define test2(...) test1(__VA_OPT__(,) 0, 1)
 *          #if test2(~) */
#ifndef TPP_HAVE_VA_OPT_IN_MACROS
#define TPP_HAVE_VA_OPT_IN_MACROS ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fva-opt-in-macros" */
#endif /* !TPP_HAVE_VA_OPT_IN_MACROS */

/* Support for: #define min(...) min_##__VA_NARGS__(__VA_ARGS__)
 * @detect: #define test___VA_COMMA__ 0
 *          #define test_1            1
 *          #define test(...) test_##__VA_NARGS__
 *          #if test2(~) */
#ifndef TPP_HAVE_VA_NARGS_IN_MACROS
#define TPP_HAVE_VA_NARGS_IN_MACROS ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fva-nargs-in-macros" */
#endif /* !TPP_HAVE_VA_NARGS_IN_MACROS */

/* Support for: #define printf(format, ...) fprintf(stderr, format,##__VA_ARGS__)
 * @detect: #define test1(a, b, ...) __VA_ARGS__+0
 *          #define test2(...) test1(,##__VA_ARGS__, 1)
 *          #if test2() == 0 */
#ifndef TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
#define TPP_HAVE_VA_GLUE_COMMA_IN_MACROS ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fglue-comma-in-macros" */
#endif /* !TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */

/* Support for: #define str(x) #x
 * @detect: #define str(x) #x
 *          #if __TPP_COUNT_TOKENS(str(a b)) == 1 */
#ifndef TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
#define TPP_HAVE_STRINGIZE_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fstringize-macro-argument" */
#endif /* !TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */

/* Support for: #define chr(x) #@x
 * @detect: #define str(x) #@x
 *          #if __TPP_COUNT_TOKENS(str(a b)) == 1 */
#ifndef TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#define TPP_HAVE_CHARIZE_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fcharize-macro-argument" */
#endif /* !TPP_HAVE_CHARIZE_MACRO_ARGUMENT */

/* Support for: #define noexpand(x) #!x
 * @detect: #define test1(x) #x
 *          #define test2(x) test1(#!x)
 *          #define test3    42
 *          #if __TPP_STR_SIZE(__TPP_COUNT_TOKENS(test3)) == 5 */
#ifndef TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
#define TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fdont-expand-macro-argument" */
#endif /* !TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT */

/* Support for: #define cat(a, b) a##b
 * @detect: #define test(a, b) a##b
 *          #if __TPP_COUNT_TOKENS(test(10, 20)) == 1 */
#ifndef TPP_HAVE_GLUE_MACRO_ARGUMENT
#define TPP_HAVE_GLUE_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fglue-macro-argument" */
#endif /* !TPP_HAVE_GLUE_MACRO_ARGUMENT */

/************************************************************************/
/************************************************************************/
/************************************************************************/




/************************************************************************/
/* #PRAGMA DIRECTIVES                                                   */
/************************************************************************/

/* TODO: Builtin support for STDC pragmas:
 * >> #pragma STDC FENV_ACCESS ON/OFF/DEFAULT
 * >> #pragma STDC FP_CONTRACT ON/OFF/DEFAULT
 * >> #pragma STDC CX_LIMITED_RANGE ON/OFF/DEFAULT
 */

/* TODO: Builtin support #pragma pack(...) */
/* TODO: Builtin support #pragma GCC visibility ... */

/* Support for: #pragma push_macro() / #pragma pop_macro()
 * @detect: #define FOO 42
 *          #pragma push_macro(undef, "FOO")
 *          #ifndef FOO */
#ifndef TPP_HAVE_PRAGMA_PUSH_MACRO
#define TPP_HAVE_PRAGMA_PUSH_MACRO ((TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-push-macro" */
#endif /* !TPP_HAVE_PRAGMA_PUSH_MACRO */

/* Support for: #pragma once
 * @detect: #if __has_known_extension("-fpragma-once") */
#ifndef TPP_HAVE_PRAGMA_ONCE
#define TPP_HAVE_PRAGMA_ONCE (((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT) && TPP_HAVE_PRAGMA) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-once" */
#endif /* !TPP_HAVE_PRAGMA_ONCE */

/* Support for: #pragma deprecated("foo")
 * @detect: #if __has_known_extension("-fpragma-deprecated") */
#ifndef TPP_HAVE_PRAGMA_DEPRECATED
#define TPP_HAVE_PRAGMA_DEPRECATED (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-deprecated" */
#endif /* !TPP_HAVE_PRAGMA_DEPRECATED */

/* Support for: #pragma extension(...)
 * @detect: #if __has_known_extension("-fpragma-extension") */
#ifndef TPP_HAVE_PRAGMA_EXTENSION
#define TPP_HAVE_PRAGMA_EXTENSION ((TPP_HAVE_PRAGMA && TPP_HAVE_EXTENSIONS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-extension" */
#endif /* !TPP_HAVE_PRAGMA_EXTENSION */

/* Support for: #pragma warning(...)
 * @detect: #if __has_known_extension("-fpragma-warning") */
#ifndef TPP_HAVE_PRAGMA_WARNING
#define TPP_HAVE_PRAGMA_WARNING ((TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-warning" */
#endif /* !TPP_HAVE_PRAGMA_WARNING */

/* Support for: #pragma message("...")
 * @detect: #if __has_known_extension("-fpragma-message") */
#ifndef TPP_HAVE_PRAGMA_MESSAGE
#define TPP_HAVE_PRAGMA_MESSAGE (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-message" */
#endif /* !TPP_HAVE_PRAGMA_MESSAGE */

/* Support for: #pragma error("...")
 * @detect: #if __has_known_extension("-fpragma-error") */
#ifndef TPP_HAVE_PRAGMA_ERROR
#define TPP_HAVE_PRAGMA_ERROR (TPP_HAVE_PRAGMA ? ((TPP_PROFILE == TPP_PROFILE_ALL) ? TPP_CONF_EXT1 : TPP_HAVE_PROFILE_NOT_MINIMAL) : 0) /* "-fpragma-error" */
#endif /* !TPP_HAVE_PRAGMA_ERROR */

/* Support for: #pragma region,  #pragma endregion
 * @detect: #if __has_known_extension("-fpragma-region") */
#ifndef TPP_HAVE_PRAGMA_REGION
#define TPP_HAVE_PRAGMA_REGION (TPP_HAVE_PRAGMA ? TPP_HAVE_PROFILE_NOT_MINIMAL : 0) /* "-fpragma-region" */
#endif /* !TPP_HAVE_PRAGMA_REGION */

/* Support for: #pragma tpp_exec("...")
 * @detect: #if __has_known_extension("-fpragma-tpp-exec") */
#ifndef TPP_HAVE_PRAGMA_TPP_EXEC
#define TPP_HAVE_PRAGMA_TPP_EXEC (TPP_HAVE_PRAGMA ? TPP_HAVE_PROFILE_NOT_MINIMAL : 0) /* "-fpragma-tpp-exec" */
#endif /* !TPP_HAVE_PRAGMA_TPP_EXEC */

/* Support for: #pragma tpp_set_keyword_flags("foo", 0x7f)
 * @detect: #if __has_known_extension("-fpragma-tpp-set-keyword-flags") */
#ifndef TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS (TPP_HAVE_PRAGMA ? TPP_HAVE_PROFILE_NOT_MINIMAL : 0) /* "-fpragma-tpp-set-keyword-flags" */
#endif /* !TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

/* Support for: #pragma GCC poison foo
 * @detect: #if __has_known_extension("-fpragma-gcc-poison") */
#ifndef TPP_HAVE_PRAGMA_GCC_POISON
#define TPP_HAVE_PRAGMA_GCC_POISON (TPP_HAVE_PRAGMA ? TPP_HAVE_PROFILE_NOT_MINIMAL : 0) /* "-fpragma-gcc-poison" */
#endif /* !TPP_HAVE_PRAGMA_GCC_POISON */

/* Support for: #pragma GCC warning "message"
 * @detect: #if __has_known_extension("-fpragma-gcc-warning") */
#ifndef TPP_HAVE_PRAGMA_GCC_WARNING
#define TPP_HAVE_PRAGMA_GCC_WARNING ((TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? TPP_HAVE_PROFILE_NOT_MINIMAL : 0) /* "-fpragma-gcc-warning" */
#endif /* !TPP_HAVE_PRAGMA_GCC_WARNING */

/* Support for: #pragma GCC error "message"
 * @detect: #if __has_known_extension("-fpragma-gcc-error") */
#ifndef TPP_HAVE_PRAGMA_GCC_ERROR
#define TPP_HAVE_PRAGMA_GCC_ERROR ((TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? TPP_HAVE_PROFILE_NOT_MINIMAL : 0) /* "-fpragma-gcc-error" */
#endif /* !TPP_HAVE_PRAGMA_GCC_ERROR */

/* Support for: #pragma GCC system_header
 * @detect: #if __has_known_extension("-fpragma-gcc-system-header") */
#ifndef TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#define TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER (TPP_HAVE_PRAGMA ? TPP_HAVE_PROFILE_NOT_MINIMAL : 0) /* "-fpragma-gcc-system-header" */
#endif /* !TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

/* Support for: #pragma GCC diagnostic
 * @detect: #if __has_known_extension("-fpragma-gcc-diagnostic") */
#ifndef TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC
#define TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC ((TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? TPP_HAVE_PROFILE_NOT_MINIMAL : 0) /* "-fpragma-gcc-diagnostic" */
#endif /* !TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */

/* Support for: #pragma GCC dependency
 * @detect: #if __has_known_extension("-fpragma-gcc-dependency") */
#ifndef TPP_HAVE_PRAGMA_GCC_DEPENDENCY
#define TPP_HAVE_PRAGMA_GCC_DEPENDENCY (TPP_HAVE_PRAGMA ? TPP_HAVE_PROFILE_NOT_MINIMAL : 0) /* "-fpragma-gcc-dependency" */
#endif /* !TPP_HAVE_PRAGMA_GCC_DEPENDENCY */

/* Support for: #pragma TPP warning(...)  (same as TPP_HAVE_PRAGMA_WARNING, but doesn't require "-fpragma-warning")
 * @detect: #if __has_known_extension("-fpragma-warning") */
#ifndef TPP_HAVE_PRAGMA_TPP_WARNING
#define TPP_HAVE_PRAGMA_TPP_WARNING TPP_HAVE_PRAGMA_WARNING
#endif /* !TPP_HAVE_PRAGMA_TPP_WARNING */

/* Support for: #pragma TPP extension(...)  (same as TPP_HAVE_PRAGMA_EXTENSION, but doesn't require "-fpragma-extension")
 * @detect: #if __has_known_extension("-fpragma-extension") */
#ifndef TPP_HAVE_PRAGMA_TPP_EXTENSION
#define TPP_HAVE_PRAGMA_TPP_EXTENSION TPP_HAVE_PRAGMA_EXTENSION
#endif /* !TPP_HAVE_PRAGMA_TPP_EXTENSION */

/* Support for: #pragma TPP tpp_exec(...)  (same as TPP_HAVE_PRAGMA_TPP_EXEC, but doesn't require "-fpragma-tpp-exec")
 * @detect: #if __has_known_extension("-fpragma-tpp-exec") */
#ifndef TPP_HAVE_PRAGMA_TPP_TPP_EXEC
#define TPP_HAVE_PRAGMA_TPP_TPP_EXEC TPP_HAVE_PRAGMA_TPP_EXEC
#endif /* !TPP_HAVE_PRAGMA_TPP_TPP_EXEC */

/* Support for: #pragma TPP tpp_set_keyword_flags("foo", 0x7f)  (same as TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS, but doesn't require "-fpragma-tpp-set-keyword-flags")
 * @detect: #if __has_known_extension("-fpragma-tpp-set-keyword-flags") */
#ifndef TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
#define TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#endif /* !TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */

/* Support for: #pragma TPP include_path(...) */
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
/* LEXER EXPRESSIONS                                                    */
/************************************************************************/

/* Provide a function "tpp_lexer_parseexpr()" that
 * is used to implement "#if" directive expressions
 * @detect: N/A */
#ifndef TPP_HAVE_LEXER_PARSEEXPR
#define TPP_HAVE_LEXER_PARSEEXPR (TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_MACRO___TPP_EVAL)
#endif /* !TPP_HAVE_LEXER_PARSEEXPR */

/* Expression parser configuration */
#if TPP_HAVE_LEXER_PARSEEXPR
#ifdef TPP_CONFIG_EXPRPARSER
#ifndef TPP_CONFIG_EXPRPARSER_NEEDS_ARG
#define TPP_CONFIG_EXPRPARSER_NEEDS_ARG 1
#endif /* !TPP_CONFIG_EXPRPARSER_NEEDS_ARG */

/* >> #define TPP_CONFIG_EXPRPARSER my_expr_parser
 * >> #if TPP_CONFIG_EXPRPARSER_NEEDS_ARG
 * >> static tpp_errno TPP_FORMATPRINTER_CC
 * >> my_expr_parser(void *arg, tpp_lexer *self, tpp_expr_value *result)
 * >> #else // TPP_CONFIG_EXPRPARSER_NEEDS_ARG
 * >> static tpp_errno TPP_FORMATPRINTER_CC
 * >> my_expr_parser(tpp_char const *self, tpp_expr_value *result)
 * >> #endif // !TPP_CONFIG_EXPRPARSER_NEEDS_ARG
 * >> {
 * >>    ...
 * >>    return TPP_EOK;
 * >> } */
#else /* TPP_CONFIG_EXPRPARSER */
/* Supply a built-in expression parser.
 *
 * -1: Provide builtin, but allow users to override
 *  1: Provide+hard-wire builtin
 *  0: Don't provide builtin, but allow users to override
 */
#ifndef TPP_HAVE_BUILTIN_EXPRPARSER
#define TPP_HAVE_BUILTIN_EXPRPARSER (-1)
#endif /* !TPP_HAVE_BUILTIN_EXPRPARSER */
#endif /* !TPP_CONFIG_EXPRPARSER */
#endif /* TPP_HAVE_LEXER_PARSEEXPR */

/* Enable support for "defined(MACRO)" in builtin lexer expressions
 * @detect: N/A */
#ifndef TPP_HAVE_BUILTIN_EXPR_DEFINED
#define TPP_HAVE_BUILTIN_EXPR_DEFINED ((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fdefined-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_DEFINED */

/* Enable special handling in "#define foo(x) defined(x)" such that "x" is not expanded
 * @detect: #define test(x) defined(x)
 *          #define test2   test1
 *          #if test(test2) */
#ifndef TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR
#define TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR ((TPP_HAVE_BUILTIN_EXPR_DEFINED && TPP_HAVE_CPP_MACROS && TPP_HAVE_PROFILE_NOT_MINIMAL) ? TPP_CONF_EXT0 : 0) /* "-fdont-expand-defined" */
#endif /* !TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR */

/* Enable support for string operations in builtin lexer expressions
 * @detect: TODO */
#ifndef TPP_HAVE_BUILTIN_EXPR_STRINGS
#define TPP_HAVE_BUILTIN_EXPR_STRINGS ((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fstrings-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_STRINGS */

/* Enable support for floats in builtin lexer expressions
 * @detect: TODO */
#ifndef TPP_HAVE_BUILTIN_EXPR_FLOATS
#define TPP_HAVE_BUILTIN_EXPR_FLOATS ((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_TPP_TOK_FLOAT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-ffloats-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_FLOATS */

/* Enable support for "foo ?: bar" in builtin lexer expressions (same as "foo ? foo : bar")
 * @detect: TODO */
#ifndef TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
#define TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT ((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fif-else-optional-true" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT */

/* Enable support for "if (foo) bar else baz" in builtin lexer expressions
 * @detect: TODO */
#ifndef TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#define TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS ((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fifelse-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */

/* Enable support for "^^" in builtin lexer expressions
 * @detect: TODO */
#ifndef TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR
#define TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR ((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-flogical-xor-in-expressions" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR */

/* Enable support for "0b" literals in builtin lexer expressions
 * @detect: TODO */
#ifndef TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS
#define TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS (((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_PROFILE_NOT_MINIMAL) && TPP_HAVE_TPP_TOK_INT) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-fbinary-literals" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS */

/* Enable support for "u", "l", "ul", "ll", "ull" integer suffixes in builtin lexer expressions
 * @detect: TODO */
#ifndef TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS
#define TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS ((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_TPP_TOK_INT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-ffixed-type-integrals" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */

/* Enable support for "i8", "i16", "i32", "i64", "ui8", "ui16", "ui32", "ui64" integer suffixes in builtin lexer expressions
 * @detect: TODO */
#ifndef TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
#define TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS ((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_TPP_TOK_INT && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_EXT1 : 1) : 0) /* "-ffixed-length-integrals" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */

/* Treat 'a' as an integer, rather than as a string (in C, this is always the case)
 * @detect: TODO */
#ifndef TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
#define TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS ((TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_TPP_TOK_STRINGLIKE_SQUOTE && TPP_HAVE_PROFILE_NOT_MINIMAL) ? (TPP_PROFILE == TPP_PROFILE_ALL ? TPP_CONF_FEAT1 : 1) : 0) /* "-fcharacter-literals" */
#endif /* !TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
/************************************************************************/
/************************************************************************/
/************************************************************************/





/************************************************************************/
/* IMPLICIT API FEATURES                                                */
/************************************************************************/
/* Provide an API function `tpp_unicode_writeutf8()' */
#ifndef TPP_HAVE_TPP_UNICODE_WRITEUTF8
#if (TPP_HAVE_TPP_TOK_STRING ||                   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
     TPP_HAVE_TPP_TOK_CHAR ||                     \
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

/* Enable support for `TPP_FILE_IOFLAGS_SYSHDR' */
#ifndef TPP_HAVE_FILE_SYSHDR
#define TPP_HAVE_FILE_SYSHDR (TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER != 0)
#endif /* !TPP_HAVE_FILE_SYSHDR */

/* Enable support for `tpp_file::tf_prev' */
#ifndef TPP_HAVE_INCLUDE_STACK
#if (TPP_HAVE_CPP_MACROS ||       \
     TPP_HAVE_CPP_INCLUDE ||      \
     TPP_HAVE_CPP_INCLUDE_NEXT || \
     TPP_HAVE_CPP_IMPORT ||       \
     TPP_HAVE_CPP_EMBED)
#define TPP_HAVE_INCLUDE_STACK 1
#else /* ... */
#define TPP_HAVE_INCLUDE_STACK 0
#endif /* !... */
#endif /* !TPP_HAVE_INCLUDE_STACK */

/* Support for: custom string list describing the available
 * "-I/usr/include-style" -> "#include <foo.h>" -paths */
#ifndef TPP_HAVE_INCLUDE_PATH
#define TPP_HAVE_INCLUDE_PATH TPP_HAVE_INCLUDE_STACK
#endif /* !TPP_HAVE_INCLUDE_PATH */

/* Enable support to push/pop the include-path state */
#ifndef TPP_HAVE_INCLUDE_PATH_PUSH_POP
#define TPP_HAVE_INCLUDE_PATH_PUSH_POP TPP_HAVE_INCLUDE_PATH
#endif /* !TPP_HAVE_INCLUDE_PATH_PUSH_POP */

/* Enable support for `tpp_file::tf_ifdef' */
#ifndef TPP_HAVE_IFDEF_STACK
#define TPP_HAVE_IFDEF_STACK (TPP_HAVE_CPP_IF_ELSE_ENDIF)
#endif /* !TPP_HAVE_IFDEF_STACK */

/* Enable support for `tpp_file::tf_data.td_io.tff_user_filename' */
#ifndef TPP_HAVE_FILE_USER_FILENAME
#define TPP_HAVE_FILE_USER_FILENAME (TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE)
#endif /* !TPP_HAVE_FILE_USER_FILENAME */

/* Enable support for `tpp_file_setline()' */
#ifndef TPP_HAVE_FILE_SETLINE
#define TPP_HAVE_FILE_SETLINE (TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE)
#endif /* !TPP_HAVE_FILE_SETLINE */

/* Enable support for setting a special pointer in files to
 * represent a lower bound of memory that must be kept loaded. */
#ifndef TPP_HAVE_FILE_KEEPPOS
#define TPP_HAVE_FILE_KEEPPOS (TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_FILE_KEEPPOS */

/* Provide a secondary set of keyword APIs that include support for \-escape sequences */
#ifndef TPP_HAVE_ESCAPED_KEYWORDS
#define TPP_HAVE_ESCAPED_KEYWORDS (TPP_HAVE_BSE || TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
#endif /* !TPP_HAVE_ESCAPED_KEYWORDS */

/* Enable support for `tpp_keywords_openfile()' */
#ifndef TPP_HAVE_KEYWORDS_OPENFILE
#if (TPP_HAVE_CPP_INCLUDE ||      \
     TPP_HAVE_CPP_INCLUDE_NEXT || \
     TPP_HAVE_CPP_IMPORT ||       \
     TPP_HAVE_CPP_EMBED ||        \
     1) /* Always enable by default */
#define TPP_HAVE_KEYWORDS_OPENFILE 1
#else /* ... */
#define TPP_HAVE_KEYWORDS_OPENFILE 0
#endif /* !... */
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE */

/* Enable support for `tpp_keywords_openfile_ex()' */
#ifndef TPP_HAVE_KEYWORDS_OPENFILE_EX
#if (TPP_HAVE_KEYWORDS_OPENFILE &&      \
     (TPP_HAVE_CPP_IMPORT ||          \
      ((TPP_HAVE_CPP_INCLUDE ||       \
        TPP_HAVE_CPP_INCLUDE_NEXT) && \
       TPP_HAVE_PRAGMA_ONCE)))
#define TPP_HAVE_KEYWORDS_OPENFILE_EX 1
#else /* ... */
#define TPP_HAVE_KEYWORDS_OPENFILE_EX 0
#endif /* !... */
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE_EX */

/* Enable support for `tpp_lexer_init_io_ex()' */
#ifndef TPP_HAVE_LEXER_INIT_IO
#define TPP_HAVE_LEXER_INIT_IO TPP_HAVE_FILE_NOKWD
#endif /* !TPP_HAVE_LEXER_INIT_IO */

/* Enable support for `tpp_lexer_init_filename()' */
#ifndef TPP_HAVE_LEXER_INIT_FILENAME
#define TPP_HAVE_LEXER_INIT_FILENAME TPP_HAVE_KEYWORDS_OPENFILE
#endif /* !TPP_HAVE_LEXER_INIT_FILENAME */

/* Enable support for detecting #ifndef-style #include-guards */
#ifndef TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#if ((TPP_HAVE_CPP_IMPORT ||        \
      TPP_HAVE_CPP_INCLUDE ||       \
      TPP_HAVE_CPP_INCLUDE_NEXT) && \
     TPP_HAVE_CPP_IF_ELSE_ENDIF)
#define TPP_HAVE_IFNDEF_INCLUDE_GUARDS 1
#else /* ... */
#define TPP_HAVE_IFNDEF_INCLUDE_GUARDS 0
#endif /* !... */
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */

/* Enable support for `tpp_lexer_skip()' */
#ifndef TPP_HAVE_LEXER_SKIP
#if (TPP_HAVE_PRAGMA_PUSH_MACRO || 1) /* TODO: List all features that use this function */
#define TPP_HAVE_LEXER_SKIP 1
#else /* ... */
#define TPP_HAVE_LEXER_SKIP 0
#endif /* !... */
#endif /* !TPP_HAVE_LEXER_SKIP */

/* Enable support for storing a time value in "tpp_lexer" */
#ifndef TPP_HAVE_LEXER_TIME
#define TPP_HAVE_LEXER_TIME TPP_HAVE_TIME_API
#endif /* !TPP_HAVE_LEXER_TIME */

/* Enable support for `tpp_lexer_rawskip_raw()', a function that is used-
 * and needed in order to seek- and skip-over the '(' token following a
 * macro's name (with support for searching for tokens in parent files
 * of the current one, but rolling back all changes if the next token
 * isn't '('). It also includes some additional functionality that will
 * retain the [tf_tpos,*) regions of files as they are scanned. */
#ifndef TPP_HAVE_LEXER_TRYSKIP_RAW
#define TPP_HAVE_LEXER_TRYSKIP_RAW (TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_LEXER_TRYSKIP_RAW */

 /* Provide a function "tpp_lexer_reprtokenid()" to
 * return the string-representation of a given token ID */
#ifndef TPP_HAVE_LEXER_REPRTOKENID
#define TPP_HAVE_LEXER_REPRTOKENID (TPP_HAVE_LEXER_SKIP)
#endif /* !TPP_HAVE_LEXER_REPRTOKENID */

/* Provide a function "tpp_reprtokenid()" that does the
 * same as "tpp_lexer_reprtokenid()" (and is also used to
 * implement that function), but fails for user-defined
 * keyword token IDs */
#ifndef TPP_HAVE_REPRTOKENID
#define TPP_HAVE_REPRTOKENID (TPP_HAVE_LEXER_REPRTOKENID)
#endif /* !TPP_HAVE_REPRTOKENID */

/* Provide a lexer state flag "TPP_LEXER_STATE_FLAG_ALLTOKENS"
 * that forces "tpp_lexer_yieldpp()" to always re-emit *all*
 * tokens (rather than skip over space/lf/comment tokens based
 * on `TPP_HAVE_TPP_TOK_*' and `TPP_FEAT_TPP_TOK_*')
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

/* Provide a function "tpp_lexer_getkeyworddefined()" to check
 * if a given keyword is "defined()" (meaning it can be expanded
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

/* Provide a set of macros/functions `tpp_lexer_manualpopfile_*'
 * that can be used to seek through the contents of files further
 * up the #include-stack in a way that allows for rollback. */
#ifndef TPP_HAVE_LEXER_MANUALPOPFILE
#define TPP_HAVE_LEXER_MANUALPOPFILE (TPP_HAVE_CPP_MACROS/* && TPP_HAVE_INCLUDE_STACK*/)
#endif /* !TPP_HAVE_LEXER_MANUALPOPFILE */

/* Provide a function "tpp_lexer_seek_rparen()" that can be used
 * to find the position of a matching ')'-token for the purpose
 * of macro argument lists. */
#ifndef TPP_HAVE_LEXER_SEEKPP_RPAREN
#define TPP_HAVE_LEXER_SEEKPP_RPAREN (TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN */

/* Same as "tpp_lexer_seek_rparen()", but also able to deal with
 * alternate parenthesis pairs: [ ] { } < > */
#ifndef TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
#define TPP_HAVE_LEXER_SEEKPP_RPAREN_EX (TPP_HAVE_LEXER_SEEKPP_RPAREN && TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS)
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */

 /* Provide a function "tpp_macro_equals()" to compare macro definitions */
#ifndef TPP_HAVE_MACRO_EQUALS
#define TPP_HAVE_MACRO_EQUALS (TPP_HAVE_TPP_W_REDEFINE_MACRO)
#endif /* !TPP_HAVE_MACRO_EQUALS */

/* Provide a function "TPP_HAVE_TPP_TOK_STRINGLIKE()" to perform \-escaping of arbitrary data */
#ifndef TPP_HAVE_LEXER_DECODESTRING
#define TPP_HAVE_LEXER_DECODESTRING (TPP_HAVE_TPP_TOK_STRINGLIKE)
#endif /* !TPP_HAVE_LEXER_DECODESTRING */

/* Provide a function "tpp_expr_value_printrepr()" to construct the result of __TPP_EVAL */
#ifndef TPP_HAVE_EXPR_VALUE_PRINTREPR
#define TPP_HAVE_EXPR_VALUE_PRINTREPR (TPP_HAVE_MACRO___TPP_EVAL)
#endif /* !TPP_HAVE_EXPR_VALUE_PRINTREPR */

/* Provide a function "tpp_token_encodestring()" to perform \-escaping of arbitrary data */
#ifndef TPP_HAVE_TOKEN_ENCODESTRING
#define TPP_HAVE_TOKEN_ENCODESTRING                                          \
	(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT || \
	 (TPP_HAVE_EXPR_VALUE_PRINTREPR && TPP_HAVE_BUILTIN_EXPR_STRINGS))
#endif /* !TPP_HAVE_TOKEN_ENCODESTRING */

/* Provide a function "tpp_lexer_decodeint_expr()" to parse an integer */
#ifndef TPP_HAVE_LEXER_DECODEINT_EXPR
#define TPP_HAVE_LEXER_DECODEINT_EXPR (TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_TPP_TOK_INT)
#endif /* !TPP_HAVE_LEXER_DECODEINT_EXPR */

/* Provide a function "tpp_lexer_decodeint()" to parse an integer */
#ifndef TPP_HAVE_LEXER_DECODEINT
#define TPP_HAVE_LEXER_DECODEINT ((TPP_HAVE_LEXER_DECODEINT_EXPR || TPP_HAVE_CPP_LINE || TPP_HAVE_CPP_DIGIT_LINE) && TPP_HAVE_TPP_TOK_INT)
#endif /* !TPP_HAVE_LEXER_DECODEINT */

/* Provide a function "tpp_lexer_decodefloat_expr()" to parse a float */
#ifndef TPP_HAVE_LEXER_DECODEFLOAT_EXPR
#define TPP_HAVE_LEXER_DECODEFLOAT_EXPR (TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_BUILTIN_EXPR_FLOATS && TPP_HAVE_TPP_TOK_FLOAT)
#endif /* !TPP_HAVE_LEXER_DECODEFLOAT_EXPR */

/* Provide a function "tpp_lexer_decodefloat()" to parse a float */
#ifndef TPP_HAVE_LEXER_DECODEFLOAT
#define TPP_HAVE_LEXER_DECODEFLOAT (TPP_HAVE_LEXER_DECODEFLOAT_EXPR)
#endif /* !TPP_HAVE_LEXER_DECODEFLOAT */

/* Provide a function "tpp_lexer_parsestring_expr()" to parse a string */
#ifndef TPP_HAVE_LEXER_PARSESTRING_EXPR
#define TPP_HAVE_LEXER_PARSESTRING_EXPR (TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_BUILTIN_EXPR_STRINGS && TPP_HAVE_LEXER_DECODESTRING)
#endif /* !TPP_HAVE_LEXER_PARSESTRING_EXPR */

/* Provide a function "tpp_lexer_parsecharacter_expr()" to parse a character literal */
#ifndef TPP_HAVE_LEXER_PARSECHARACTER_EXPR
#define TPP_HAVE_LEXER_PARSECHARACTER_EXPR (TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS && TPP_HAVE_LEXER_DECODESTRING)
#endif /* !TPP_HAVE_LEXER_PARSECHARACTER_EXPR */

/* Provide a builtin implementation for "tpp_expr_value" and its API */
#undef TPP_HAVE_BUILTIN_EXPR_VALUE
#if !defined(tpp_expr_value) && TPP_HAVE_LEXER_PARSEEXPR
#define TPP_HAVE_BUILTIN_EXPR_VALUE 1
#endif /* !tpp_expr_value && TPP_HAVE_LEXER_PARSEEXPR */
#ifndef TPP_HAVE_BUILTIN_EXPR_VALUE
#define TPP_HAVE_BUILTIN_EXPR_VALUE 0
#endif /* !TPP_HAVE_BUILTIN_EXPR_VALUE */

/* Provide a function "tpp_ftoa()" to convert a float into a string */
#ifndef TPP_HAVE_FTOA
#define TPP_HAVE_FTOA (TPP_HAVE_EXPR_VALUE_PRINTREPR)
#endif /* !TPP_HAVE_FTOA */

/************************************************************************/
/************************************************************************/
/************************************************************************/





/************************************************************************/
/* WARNINGS                                                             */
/************************************************************************/

/* Format to use for file+line+column log messages */
#ifndef TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT
#if defined(TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV) && TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV
#undef TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_GCC
#elif defined(TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_GCC) && TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_GCC
#undef TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV
#else /* !... */
#undef TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV
#undef TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_GCC
#ifdef _MSC_VER
#define TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV 1
#define TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_GCC  0
#else /* _MSC_VER */
#define TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_GCC  1
#define TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV 0
#endif /* !_MSC_VER */
#endif /* ... */
#if TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV
#define TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT "%Pf(%Pl, %Pc): "
#elif TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_GCC
#define TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT "%Pf:%Pl:%Pc: "
#else /* ... */
#error "Invalid configuration of 'TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT'"
#endif /* !... */
#else /* !TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT */
#undef TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_GCC
#undef TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV
#define TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV 0
#define TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_GCC  0
#endif /* TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT */

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
#define TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED                \
	(TPP_HAVE_WARNINGS &&                                           \
	 TPP_CONF_MAYBE_0(TPP_HAVE_STRING_ALLOW_MULTILINE) && \
	 (TPP_HAVE_TPP_TOK_STRING ||                                  \
	  TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||                 \
	  TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||                 \
	  TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL ||                \
	  TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL ||                \
	  TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||                      \
	  TPP_HAVE_TPP_TOK_CHAR ||                                    \
	  TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||                   \
	  TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||                   \
	  TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||                  \
	  TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||                  \
	  TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL))
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
#ifndef TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED
#define TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED \
	(TPP_HAVE_WARNINGS &&                              \
	 TPP_HAVE_STRING_WARN_MULTILINE &&         \
	 (TPP_HAVE_TPP_TOK_STRING ||                     \
	  TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||    \
	  TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||    \
	  TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL ||   \
	  TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL ||   \
	  TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||         \
	  TPP_HAVE_TPP_TOK_CHAR ||                       \
	  TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||      \
	  TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||      \
	  TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||     \
	  TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||     \
	  TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL))
#endif /* !TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED */
#ifndef TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#define TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF (TPP_HAVE_WARNINGS && TPP_HAVE_TPP_TOK_STRINGLIKE)
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
#ifndef TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
#define TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF (TPP_HAVE_WARNINGS && TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE)
#endif /* !TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN (TPP_HAVE_WARNINGS && TPP_HAVE_LEXER_SKIP)
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN */
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
#define TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE (TPP_HAVE_WARNINGS && TPP_CONF_MAYBE_0(TPP_HAVE_TPP_TOK_SHELL_COMMENT) && TPP_HAVE_CPP_DIRECTIVES)
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
#define TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE                     \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_TPP_TOK_STRING ||                   \
	                       TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
	                       TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
	                       TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
	                       TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
	                       TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
	                       TPP_HAVE_TPP_TOK_CHAR ||                     \
	                       TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL ||    \
	                       TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL ||    \
	                       TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL ||   \
	                       TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL ||   \
	                       TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL))
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
#ifndef TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
#define TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_CPP_DEFINE))
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
#if 1
#define TPP_HAVE_TPP_W_EXPECTED_STRING \
	(TPP_HAVE_WARNINGS && TPP_HAVE_TPP_TOK_STRINGLIKE)
#else
#define TPP_HAVE_TPP_W_EXPECTED_STRING \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_PRAGMA_PUSH_MACRO || ...))
#endif
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
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
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_EXPRPARSER)
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION */
#ifndef TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION
#define TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_EXPRPARSER)
#endif /* !TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION */
#ifndef TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
#define TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_EXPR_DEFINED)
#endif /* !TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED */
#ifndef TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS
#define TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS \
	(TPP_HAVE_WARNINGS && TPP_HAVE_BUILTIN_EXPRPARSER && TPP_HAVE_BUILTIN_EXPR_STRINGS)
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
#define TPP_HAVE_TPP_W_DATE_TIME                             \
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


/* Warning printer configuration */
#if TPP_HAVE_WARNINGS
#ifdef TPP_CONFIG_WARNPRINTER
#ifndef TPP_CONFIG_WARNPRINTER_NEEDS_ARG
#define TPP_CONFIG_WARNPRINTER_NEEDS_ARG 1
#endif /* !TPP_CONFIG_WARNPRINTER_NEEDS_ARG */

/* >> #define TPP_CONFIG_WARNPRINTER my_warning_printer
 * >> #if TPP_CONFIG_WARNPRINTER_NEEDS_ARG
 * >> static tpp_ssize TPP_FORMATPRINTER_CC
 * >> my_warning_printer(void *arg, tpp_char const *text, tpp_size num_bytes)
 * >> #else // TPP_CONFIG_WARNPRINTER_NEEDS_ARG
 * >> static tpp_ssize TPP_FORMATPRINTER_CC
 * >> my_warning_printer(tpp_char const *text, tpp_size num_bytes)
 * >> #endif // !TPP_CONFIG_WARNPRINTER_NEEDS_ARG
 * >> {
 * >>    ...
 * >> } */
#else /* TPP_CONFIG_WARNPRINTER */
/* Supply a built-in printer (that uses "fwrite(stderr)")
 * when no user-defined printer was configured for a lexer.
 *
 * -1: Provide builtin, but allow users to override
 *  1: Provide+hard-wire builtin
 *  0: Don't provide builtin, but allow users to override
 */
#ifndef TPP_HAVE_BUILTIN_WARNPRINTER
#define TPP_HAVE_BUILTIN_WARNPRINTER (-1)
#endif /* !TPP_HAVE_BUILTIN_WARNPRINTER */
#endif /* !TPP_CONFIG_WARNPRINTER */
#endif /* TPP_HAVE_WARNINGS */
/************************************************************************/
/************************************************************************/
/************************************************************************/

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CONFIG_H */
