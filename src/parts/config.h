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

#ifndef TPP_CONFIG_DEFS_FILENAME
#define TPP_CONFIG_DEFS_FILENAME "defs.h"
#endif /* !TPP_CONFIG_DEFS_FILENAME */

/*[[[tpp-begin]]]*/

/* Name of the file to #include for dynamic definitions */
#ifndef TPP_CONFIG_DEFS_FILENAME
#define TPP_CONFIG_DEFS_FILENAME "tpp-amalgamation.h"
#endif /* !TPP_CONFIG_DEFS_FILENAME */

#ifndef TPP_CONFIG_DEBUG
#ifdef NDEBUG
#define TPP_CONFIG_DEBUG 0
#else /* NDEBUG */
#define TPP_CONFIG_DEBUG 1
#endif /* !NDEBUG */
#endif /* !TPP_CONFIG_DEBUG */

/* API features:
 *  0: Disabled
 *  1: Enabled
 * -1: Enable if possible (re-defined to `0' if unsupported)
 */

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
#define TPP_ERROR_LIMIT (-64)
#endif /* !TPP_ERROR_LIMIT */

/* Enable support for non-blocking I/O */
#ifndef TPP_HAVE_FILE_NONBLOCK
#define TPP_HAVE_FILE_NONBLOCK (-1)
#endif /* !TPP_HAVE_FILE_NONBLOCK */

/* Enable support for:
 * - External hooks to inject unicode ctype information
 *   By default, only ASCII ctype information is built-in
 * - Automatic detection of utf-8, utf-8-bom, utf-16[le/be], utf-32[le/be] in input files
 *   NOTE: The Lexer assumes "utf-8" by default, unless it encounters an invalid utf-8
 *         byte sequence, at which point it will automatically downgrade to ASCII-only
 *         for the remainder of the relevant file
 */
#ifndef TPP_HAVE_UNICODE
#define TPP_HAVE_UNICODE (-1)
#endif /* !TPP_HAVE_UNICODE */

/* Provide a function "tpp_strerror()" to get a description of a given "tpp_errno" error code. */
#ifndef TPP_HAVE_STRERROR
#define TPP_HAVE_STRERROR 1 /* TODO: Default should be `0' */
#endif /* !TPP_HAVE_STRERROR */

/* Provide a function "tpp_strtokenid()" to get the API name of a (non-keyword) token ID */
#ifndef TPP_HAVE_STRTOKENID
#define TPP_HAVE_STRTOKENID 1 /* TODO: Default should be `0' */
#endif /* !TPP_HAVE_STRTOKENID */

/* Enable support for runtime-configurable extensions */
#ifndef TPP_HAVE_EXTENSIONS
#define TPP_HAVE_EXTENSIONS 1
#endif /* !TPP_HAVE_EXTENSIONS */

/* Enable support to push/pop the extension state */
#ifndef TPP_HAVE_EXTENSIONS_PUSH_POP
#define TPP_HAVE_EXTENSIONS_PUSH_POP TPP_HAVE_EXTENSIONS
#endif /* !TPP_HAVE_EXTENSIONS_PUSH_POP */

/* Support for: compiler warnings (else: behave as though all warnings were being suppressed) */
#ifndef TPP_HAVE_WARNINGS
#define TPP_HAVE_WARNINGS 1
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
#define TPP_HAVE_FILE_NOCLOSE (-1) /* TODO: Default should be `0' */
#endif /* !TPP_HAVE_FILE_NOCLOSE */

/* Enable support for `TPP_FILE_IOFLAGS_NOKWD' */
#ifndef TPP_HAVE_FILE_NOKWD
#define TPP_HAVE_FILE_NOKWD (-1)
#endif /* !TPP_HAVE_FILE_NOKWD */

/* Speed up calls to `tpp_file_lcinfo()' by caching the last-read
 * position and determining line/column information as a delta from
 * what was previously cached */
#ifndef TPP_HAVE_FILE_LC_CACHE
#define TPP_HAVE_FILE_LC_CACHE 1
#endif /* !TPP_HAVE_FILE_LC_CACHE */


/* All config options can be defined as:
 *  0: Compile-time disabled
 *  1: Compile-time enabled (without #pragma extension("-f..."))
 * -1: Compile-time enabled (with #pragma extension("-f..."), default = true)
 * -2: Compile-time enabled (with #pragma extension("-f..."), default = false)
 */

#ifndef TPP_COMMON_HAVE_TPP_TOK_SPACE
#define TPP_COMMON_HAVE_TPP_TOK_SPACE (-1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_SPACE */
#ifndef TPP_COMMON_HAVE_TPP_TOK_COMMENT
#define TPP_COMMON_HAVE_TPP_TOK_COMMENT (-1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_COMMENT */
#ifndef TPP_COMMON_HAVE_TPP_TOK_GENERIC
#define TPP_COMMON_HAVE_TPP_TOK_GENERIC (-1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_GENERIC */
#ifndef TPP_COMMON_HAVE_TPP_TOK_CXX_STRING
#define TPP_COMMON_HAVE_TPP_TOK_CXX_STRING (-1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_CXX_STRING */
#ifndef TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
#define TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING (-1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING */
#ifndef TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_C_TOKENS (-1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_C_TOKENS */
#ifndef TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS (-1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS */
#ifndef TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS (-1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS */
#ifndef TPP_COMMON_HAVE_CPP_DIRECTIVES
#define TPP_COMMON_HAVE_CPP_DIRECTIVES (-1)
#endif /* !TPP_COMMON_HAVE_CPP_DIRECTIVES */
#ifndef TPP_COMMON_HAVE_PRAGMA
#define TPP_COMMON_HAVE_PRAGMA TPP_COMMON_HAVE_CPP_DIRECTIVES
#endif /* !TPP_COMMON_HAVE_PRAGMA */
#ifndef TPP_COMMON_HAVE_PRAGMA_GCC
#define TPP_COMMON_HAVE_PRAGMA_GCC TPP_COMMON_HAVE_PRAGMA
#endif /* !TPP_COMMON_HAVE_PRAGMA_GCC */
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
#define TPP_HAVE_TRIGRAPHS (-1)
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
 * "%:%:" -> "##" */
#ifndef TPP_HAVE_DIGRAPHS
#define TPP_HAVE_DIGRAPHS (-1)
#endif /* !TPP_HAVE_DIGRAPHS */

/* Configures if line-feed tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()' */
#ifndef TPP_HAVE_TPP_TOK_LF
#define TPP_HAVE_TPP_TOK_LF TPP_COMMON_HAVE_TPP_TOK_SPACE
#endif /* !TPP_HAVE_TPP_TOK_LF */

/* Configures if whitespace tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()' */
#ifndef TPP_HAVE_TPP_TOK_SPACE
#define TPP_HAVE_TPP_TOK_SPACE TPP_COMMON_HAVE_TPP_TOK_SPACE
#endif /* !TPP_HAVE_TPP_TOK_SPACE */

/* Configures if comment tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()' */
#ifndef TPP_HAVE_TPP_TOK_COMMENT
#define TPP_HAVE_TPP_TOK_COMMENT (-1)
#endif /* !TPP_HAVE_TPP_TOK_COMMENT */

/* Enable support for c++ comments: "// like this one!" */
#ifndef TPP_HAVE_TPP_TOK_CXX_COMMENT
#define TPP_HAVE_TPP_TOK_CXX_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT
#endif /* !TPP_HAVE_TPP_TOK_CXX_COMMENT */

/* Enable support for c comments: "/" "* like this one! *" "/" */
#ifndef TPP_HAVE_TPP_TOK_C_COMMENT
#define TPP_HAVE_TPP_TOK_C_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT
#endif /* !TPP_HAVE_TPP_TOK_C_COMMENT */

/* Enable support for pascal comments: "(*like this one!*)" */
#ifndef TPP_HAVE_TPP_TOK_PASCAL_COMMENT
#define TPP_HAVE_TPP_TOK_PASCAL_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT
#endif /* !TPP_HAVE_TPP_TOK_PASCAL_COMMENT */

/* Enable support for shell comments: "# like this one!" */
#ifndef TPP_HAVE_TPP_TOK_SHELL_COMMENT
#define TPP_HAVE_TPP_TOK_SHELL_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT
#endif /* !TPP_HAVE_TPP_TOK_SHELL_COMMENT */

/* Enable support for ASM comments: "/ like this one!" */
#ifndef TPP_HAVE_TPP_TOK_ASM_COMMENT
#define TPP_HAVE_TPP_TOK_ASM_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT
#endif /* !TPP_HAVE_TPP_TOK_ASM_COMMENT */

/* Enable support for SQL comments: "-- like this one!" */
#ifndef TPP_HAVE_TPP_TOK_SQL_COMMENT
#define TPP_HAVE_TPP_TOK_SQL_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT
#endif /* !TPP_HAVE_TPP_TOK_SQL_COMMENT */


/* 123 */
#ifndef TPP_HAVE_TPP_TOK_INT
#define TPP_HAVE_TPP_TOK_INT TPP_COMMON_HAVE_TPP_TOK_GENERIC
#endif /* !TPP_HAVE_TPP_TOK_INT */

/* 123.0 */
#ifndef TPP_HAVE_TPP_TOK_FLOAT
#define TPP_HAVE_TPP_TOK_FLOAT TPP_COMMON_HAVE_TPP_TOK_GENERIC
#endif /* !TPP_HAVE_TPP_TOK_FLOAT */

/* 'foo' */
#ifndef TPP_HAVE_TPP_TOK_CHAR
#define TPP_HAVE_TPP_TOK_CHAR TPP_COMMON_HAVE_TPP_TOK_GENERIC
#endif /* !TPP_HAVE_TPP_TOK_CHAR */

/* "foo" */
#ifndef TPP_HAVE_TPP_TOK_STRING
#define TPP_HAVE_TPP_TOK_STRING TPP_COMMON_HAVE_TPP_TOK_GENERIC
#endif /* !TPP_HAVE_TPP_TOK_STRING */

/* R"AB(foo)AB" */
#ifndef TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */

/* L"foo"  (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: LR"AB(foo)AB") */
#ifndef TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING
#endif /* !TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */

/* u8"foo" (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: u8R"AB(foo)AB") */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */

/* u"foo"  (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: uR"AB(foo)AB") */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */

/* U"foo"  (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: UR"AB(foo)AB") */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */

/* R"foo"  r"foo"  */
#ifndef TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
#endif /* !TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */

/* R'bar'  r'bar' */
#ifndef TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
#endif /* !TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */

/* """foo""" */
#ifndef TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */

/* '''foo''' */
#ifndef TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */

/* "<<" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE */

/* ">>" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE */

/* "==" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL */

/* "!=" */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */

/* ">=" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_EQUAL */

/* "<=" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL */

/* "..." */
#ifndef TPP_HAVE_TPP_TOK_DOT_DOT_DOT
#define TPP_HAVE_TPP_TOK_DOT_DOT_DOT TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_DOT_DOT_DOT */

/* "+=" */
#ifndef TPP_HAVE_TPP_TOK_PLUS_EQUAL
#define TPP_HAVE_TPP_TOK_PLUS_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_PLUS_EQUAL */

/* "-=" */
#ifndef TPP_HAVE_TPP_TOK_MINUS_EQUAL
#define TPP_HAVE_TPP_TOK_MINUS_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_MINUS_EQUAL */

/* "*=" */
#ifndef TPP_HAVE_TPP_TOK_STAR_EQUAL
#define TPP_HAVE_TPP_TOK_STAR_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_STAR_EQUAL */

/* "/=" */
#ifndef TPP_HAVE_TPP_TOK_SLASH_EQUAL
#define TPP_HAVE_TPP_TOK_SLASH_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_SLASH_EQUAL */

/* "%=" */
#ifndef TPP_HAVE_TPP_TOK_PERCENT_EQUAL
#define TPP_HAVE_TPP_TOK_PERCENT_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_PERCENT_EQUAL */

/* "<<=" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */

/* ">>=" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */

/* "&=" */
#ifndef TPP_HAVE_TPP_TOK_AMP_EQUAL
#define TPP_HAVE_TPP_TOK_AMP_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_AMP_EQUAL */

/* "|=" */
#ifndef TPP_HAVE_TPP_TOK_PIPE_EQUAL
#define TPP_HAVE_TPP_TOK_PIPE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_PIPE_EQUAL */

/* "^=" */
#ifndef TPP_HAVE_TPP_TOK_HAT_EQUAL
#define TPP_HAVE_TPP_TOK_HAT_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_HAT_EQUAL */

/* "//" */
#ifndef TPP_HAVE_TPP_TOK_SLASH_SLASH
#define TPP_HAVE_TPP_TOK_SLASH_SLASH TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_SLASH_SLASH */

/* "//=" */
#ifndef TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
#define TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */

/* "**=" */
#ifndef TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
#define TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */

/* "@=" */
#ifndef TPP_HAVE_TPP_TOK_AT_EQUAL
#define TPP_HAVE_TPP_TOK_AT_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_AT_EQUAL */

/* "##" */
#ifndef TPP_HAVE_TPP_TOK_POUND_POUND
#define TPP_HAVE_TPP_TOK_POUND_POUND TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_POUND_POUND */

/* "&&" */
#ifndef TPP_HAVE_TPP_TOK_AMP_AMP
#define TPP_HAVE_TPP_TOK_AMP_AMP TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_AMP_AMP */

/* "||" */
#ifndef TPP_HAVE_TPP_TOK_PIPE_PIPE
#define TPP_HAVE_TPP_TOK_PIPE_PIPE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_PIPE_PIPE */

/* "^^" */
#ifndef TPP_HAVE_TPP_TOK_HAT_HAT
#define TPP_HAVE_TPP_TOK_HAT_HAT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_HAT_HAT */

/* "++" */
#ifndef TPP_HAVE_TPP_TOK_PLUS_PLUS
#define TPP_HAVE_TPP_TOK_PLUS_PLUS TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_PLUS_PLUS */

/* "--" */
#ifndef TPP_HAVE_TPP_TOK_MINUS_MINUS
#define TPP_HAVE_TPP_TOK_MINUS_MINUS TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_MINUS_MINUS */

/* "**" */
#ifndef TPP_HAVE_TPP_TOK_STAR_STAR
#define TPP_HAVE_TPP_TOK_STAR_STAR TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_STAR_STAR */

/* "~~" */
#ifndef TPP_HAVE_TPP_TOK_TILDE_TILDE
#define TPP_HAVE_TPP_TOK_TILDE_TILDE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_TILDE_TILDE */

/* "~=" */
#ifndef TPP_HAVE_TPP_TOK_TILDE_EQUAL
#define TPP_HAVE_TPP_TOK_TILDE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_TILDE_EQUAL */

/* "->" */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE */

/* ":=" */
#ifndef TPP_HAVE_TPP_TOK_COLON_EQUAL
#define TPP_HAVE_TPP_TOK_COLON_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_COLON_EQUAL */

/* "::" */
#ifndef TPP_HAVE_TPP_TOK_COLON_COLON
#define TPP_HAVE_TPP_TOK_COLON_COLON TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_COLON_COLON */

/* "->*" */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */

/* ".*" */
#ifndef TPP_HAVE_TPP_TOK_DOT_STAR
#define TPP_HAVE_TPP_TOK_DOT_STAR TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_DOT_STAR */

/* ".." */
#ifndef TPP_HAVE_TPP_TOK_DOT_DOT
#define TPP_HAVE_TPP_TOK_DOT_DOT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_DOT_DOT */

/* "<>" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_RANGLE */

/* "<<<" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */

/* ">>>" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */

/* "<<<=" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */

/* ">>>=" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */

/* "===" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */

/* "!==" */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */

/* "??" */
#ifndef TPP_HAVE_TPP_TOK_QMARK_QMARK
#define TPP_HAVE_TPP_TOK_QMARK_QMARK TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_QMARK_QMARK */

/* "?=" */
#ifndef TPP_HAVE_TPP_TOK_QMARK_EQUAL
#define TPP_HAVE_TPP_TOK_QMARK_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#endif /* !TPP_HAVE_TPP_TOK_QMARK_EQUAL */


#undef TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
#define TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE \
	(TPP_HAVE_TPP_TOK_C_COMMENT || TPP_HAVE_TPP_TOK_PASCAL_COMMENT)
#undef TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE
#define TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE                              \
	(TPP_HAVE_TPP_TOK_CXX_COMMENT || TPP_HAVE_TPP_TOK_SHELL_COMMENT || \
	 TPP_HAVE_TPP_TOK_ASM_COMMENT || TPP_HAVE_TPP_TOK_SQL_COMMENT)
#undef TPP_HAVE_TPP_TOK_COMMENTLIKE
#define TPP_HAVE_TPP_TOK_COMMENTLIKE \
	(TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE || TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE)
#undef TPP_HAVE_TPP_TOK_STRINGLIKE
#if (TPP_HAVE_TPP_TOK_CHAR ||                     \
     TPP_HAVE_TPP_TOK_STRING ||                   \
     TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL ||   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||       \
     TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL ||         \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
     TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL)
#define TPP_HAVE_TPP_TOK_STRINGLIKE 1
#else /* ... */
#define TPP_HAVE_TPP_TOK_STRINGLIKE 0
#endif /* !... */
#undef TPP_HAVE_STRING_ESCAPE
#if (TPP_HAVE_TPP_TOK_CHAR ||                     \
     TPP_HAVE_TPP_TOK_STRING ||                   \
     TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
     TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
     TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
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
 * and multi-line macro definitions. */
#ifndef TPP_HAVE_BSE
#define TPP_HAVE_BSE (-1)
#endif /* !TPP_HAVE_BSE */

/* Extension to `TPP_HAVE_BSE': the \ backslash is allowed to be followed by extra
 * whitespace preceding the actual linefeed
 *
 * This DOES affect the line-continuation features of C++ // comments, and
 * multi-line macro definitions. */
#ifndef TPP_HAVE_BSE_WHITESPACE
#define TPP_HAVE_BSE_WHITESPACE (TPP_HAVE_BSE ? -1 : 0)
#endif /* !TPP_HAVE_BSE_WHITESPACE */

/* Support for "\e" (for U+001B) escape sequences */
#ifndef TPP_HAVE_ESCAPE_E_IN_STRINGS
#define TPP_HAVE_ESCAPE_E_IN_STRINGS (TPP_HAVE_STRING_ESCAPE ? -1 : 0)
#endif /* !TPP_HAVE_ESCAPE_E_IN_STRINGS */

/* Support for "\s" (for U+0020) escape sequences */
#ifndef TPP_HAVE_ESCAPE_S_IN_STRINGS
#define TPP_HAVE_ESCAPE_S_IN_STRINGS (TPP_HAVE_STRING_ESCAPE ? -1 : 0)
#endif /* !TPP_HAVE_ESCAPE_S_IN_STRINGS */

/* Specifies if *any* CPP directives are supported */
#ifndef TPP_HAVE_CPP_DIRECTIVES
#define TPP_HAVE_CPP_DIRECTIVES TPP_COMMON_HAVE_CPP_DIRECTIVES
#endif /* !TPP_HAVE_CPP_DIRECTIVES */

/* Support for C-style macros */
#ifndef TPP_HAVE_CPP_MACROS
#define TPP_HAVE_CPP_MACROS (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_MACROS */

/* Support for: #!foobar  (comments) */
#ifndef TPP_HAVE_CPP_EXCLAIM
#define TPP_HAVE_CPP_EXCLAIM (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_EXCLAIM */

/* Support for: #  (blank line) */
#ifndef TPP_HAVE_CPP_BLANK
#define TPP_HAVE_CPP_BLANK (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_BLANK */

/* Support for: # 42 ...  (similar to #line) */
#ifndef TPP_HAVE_CPP_DIGIT_LINE
#define TPP_HAVE_CPP_DIGIT_LINE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_DIGIT_LINE */

/* Support for: #line ... */
#ifndef TPP_HAVE_CPP_LINE
#define TPP_HAVE_CPP_LINE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_LINE */

/* Support for: #include */
#ifndef TPP_HAVE_CPP_INCLUDE
#define TPP_HAVE_CPP_INCLUDE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_INCLUDE */

/* Support for: #include_next */
#ifndef TPP_HAVE_CPP_INCLUDE_NEXT
#define TPP_HAVE_CPP_INCLUDE_NEXT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_INCLUDE_NEXT */

/* Support for: #import */
#ifndef TPP_HAVE_CPP_IMPORT
#define TPP_HAVE_CPP_IMPORT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_IMPORT */

/* Support for: #if, #ifdef, #ifndef, #elif, #elifdef, #elifndef, #else, #endif */
#ifndef TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_HAVE_CPP_IF_ELSE_ENDIF ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_IF_ELSE_ENDIF */

/* Support for: #define, #undef */
#ifndef TPP_HAVE_CPP_DEFINE
#define TPP_HAVE_CPP_DEFINE ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_DEFINE */

/* Support for: #assert, #unassert */
#ifndef TPP_HAVE_CPP_ASSERT
#define TPP_HAVE_CPP_ASSERT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_ASSERT */

/* Support for: #error */
#ifndef TPP_HAVE_CPP_ERROR
#define TPP_HAVE_CPP_ERROR (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_ERROR */

/* Support for: #warning */
#ifndef TPP_HAVE_CPP_WARNING
#define TPP_HAVE_CPP_WARNING (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_WARNING */

/* Support for: #ident */
#ifndef TPP_HAVE_CPP_IDENT
#define TPP_HAVE_CPP_IDENT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_IDENT */

/* Support for: #scss */
#ifndef TPP_HAVE_CPP_SCSS
#define TPP_HAVE_CPP_SCSS (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_SCSS */

/* Support for: #pragma */
#ifndef TPP_HAVE_CPP_PRAGMA
#define TPP_HAVE_CPP_PRAGMA (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0)
#endif /* !TPP_HAVE_CPP_PRAGMA */

/* Support for: _Pragma("foo") */
#ifndef TPP_HAVE_STDC_PRAGMA
#define TPP_HAVE_STDC_PRAGMA TPP_COMMON_HAVE_PRAGMA
#endif /* !TPP_HAVE_STDC_PRAGMA */

/* Support for: __pragma(foo) */
#ifndef TPP_HAVE_MSVC_PRAGMA
#define TPP_HAVE_MSVC_PRAGMA TPP_COMMON_HAVE_PRAGMA
#endif /* !TPP_HAVE_MSVC_PRAGMA */


/* Support for: #pragma */
#ifndef TPP_HAVE_PRAGMA
#define TPP_HAVE_PRAGMA (TPP_HAVE_CPP_PRAGMA || TPP_HAVE_STDC_PRAGMA || TPP_HAVE_MSVC_PRAGMA)
#endif /* !TPP_HAVE_PRAGMA */


/* Support for clang __has_attribute */
#ifndef TPP_HAVE_CLANG_HAS_ATTRIBUTE
#define TPP_HAVE_CLANG_HAS_ATTRIBUTE (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_CLANG_HAS_ATTRIBUTE */

/* Support for clang __has_builtin */
#ifndef TPP_HAVE_CLANG_HAS_BUILTIN
#define TPP_HAVE_CLANG_HAS_BUILTIN (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_CLANG_HAS_BUILTIN */

/* Support for clang __has_cpp_attribute */
#ifndef TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE
#define TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE */

/* Support for clang __has_declspec_attribute */
#ifndef TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE
#define TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE */

/* Support for clang __has_extension */
#ifndef TPP_HAVE_CLANG_HAS_EXTENSION
#define TPP_HAVE_CLANG_HAS_EXTENSION (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_CLANG_HAS_EXTENSION */

/* Support for clang __has_feature */
#ifndef TPP_HAVE_CLANG_HAS_FEATURE
#define TPP_HAVE_CLANG_HAS_FEATURE (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_CLANG_HAS_FEATURE */

/* Support for clang __has_c_attribute */
#ifndef TPP_HAVE_CLANG_HAS_C_ATTRIBUTE
#define TPP_HAVE_CLANG_HAS_C_ATTRIBUTE (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_CLANG_HAS_C_ATTRIBUTE */

/* Support for clang __is_identifier */
#ifndef TPP_HAVE_CLANG_IS_IDENTIFIER
#define TPP_HAVE_CLANG_IS_IDENTIFIER (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_CLANG_IS_IDENTIFIER */

/* Support for TPP's __is_deprecated */
#ifndef TPP_HAVE_TPPX_IS_DEPRECATED
#define TPP_HAVE_TPPX_IS_DEPRECATED (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_TPPX_IS_DEPRECATED */

/* Support for TPP's __is_poisoned */
#ifndef TPP_HAVE_TPPX_IS_POISONED
#define TPP_HAVE_TPPX_IS_POISONED (TPP_HAVE_CPP_MACROS ? (-1) : 0)
#endif /* !TPP_HAVE_TPPX_IS_POISONED */

/* Support for TPP's __has_extension */
#ifndef TPP_HAVE_TPPX_HAS_EXTENSION
#define TPP_HAVE_TPPX_HAS_EXTENSION ((TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_MACROS) ? (-1) : 0)
#endif /* !TPP_HAVE_TPPX_HAS_EXTENSION */

/* Support for TPP's __has_known_extension */
#ifndef TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION
#define TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION ((TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_MACROS) ? (-1) : 0)
#endif /* !TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION */

/* Support for TPP's __has_warning */
#ifndef TPP_HAVE_TPPX_HAS_WARNING
#define TPP_HAVE_TPPX_HAS_WARNING ((TPP_HAVE_WARNINGS && TPP_HAVE_CPP_MACROS) ? (-1) : 0)
#endif /* !TPP_HAVE_TPPX_HAS_WARNING */

/* Support for TPP's __has_known_warning */
#ifndef TPP_HAVE_TPPX_HAS_KNOWN_WARNING
#define TPP_HAVE_TPPX_HAS_KNOWN_WARNING ((TPP_HAVE_WARNINGS && TPP_HAVE_CPP_MACROS) ? (-1) : 0)
#endif /* !TPP_HAVE_TPPX_HAS_KNOWN_WARNING */

/* Support for: #define point<T> ... */
#ifndef TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS (TPP_HAVE_CPP_MACROS ? -1 : 0)
#endif /* !TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

/* Support for retaining whitespace around macro arguments */
#ifndef TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
#define TPP_HAVE_MACRO_ARGUMENT_WHITESPACE (TPP_HAVE_CPP_MACROS ? -1 : 0)
#endif /* !TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */

/* Support for: #pragma extension("-fmacro-recursion") */
#ifndef TPP_HAVE_MACRO_RECURSION
#define TPP_HAVE_MACRO_RECURSION (TPP_HAVE_CPP_MACROS ? -2 : 0)
#endif /* !TPP_HAVE_MACRO_RECURSION */

/* Support for: #define printf(format, args...) args */
#ifndef TPP_HAVE_NAMED_VARARGS_IN_MACROS
#define TPP_HAVE_NAMED_VARARGS_IN_MACROS (TPP_HAVE_CPP_MACROS ? -1 : 0)
#endif /* !TPP_HAVE_NAMED_VARARGS_IN_MACROS */

/* Support for: #define printf(format, ...) __VA_ARGS__ */
#ifndef TPP_HAVE_VA_ARGS_IN_MACROS
#define TPP_HAVE_VA_ARGS_IN_MACROS (TPP_HAVE_CPP_MACROS ? -1 : 0)
#endif /* !TPP_HAVE_VA_ARGS_IN_MACROS */

/* Support for: #define printf(format, ...) fprintf(stderr, format __VA_COMMA__ __VA_ARGS__) */
#ifndef TPP_HAVE_VA_COMMA_IN_MACROS
#define TPP_HAVE_VA_COMMA_IN_MACROS (TPP_HAVE_CPP_MACROS ? -1 : 0)
#endif /* !TPP_HAVE_VA_COMMA_IN_MACROS */

/* Support for: #define printf(format, ...) fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__) */
#ifndef TPP_HAVE_VA_OPT_IN_MACROS
#define TPP_HAVE_VA_OPT_IN_MACROS (TPP_HAVE_CPP_MACROS ? -1 : 0)
#endif /* !TPP_HAVE_VA_OPT_IN_MACROS */

/* Support for: #define min(...) min_##__VA_NARGS__(__VA_ARGS__) */
#ifndef TPP_HAVE_VA_NARGS_IN_MACROS
#define TPP_HAVE_VA_NARGS_IN_MACROS (TPP_HAVE_CPP_MACROS ? -1 : 0)
#endif /* !TPP_HAVE_VA_NARGS_IN_MACROS */
/************************************************************************/
/************************************************************************/
/************************************************************************/




/************************************************************************/
/* #PRAGMA DIRECTIVES                                                   */
/************************************************************************/

/* Support for: #pragma push_macro() / #pragma pop_macro() */
#ifndef TPP_HAVE_PRAGMA_PUSH_MACRO
#define TPP_HAVE_PRAGMA_PUSH_MACRO ((TPP_HAVE_CPP_MACROS && TPP_COMMON_HAVE_PRAGMA) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_PUSH_MACRO */

/* Support for: #pragma once */
#ifndef TPP_HAVE_PRAGMA_ONCE
#define TPP_HAVE_PRAGMA_ONCE (((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT) && TPP_COMMON_HAVE_PRAGMA) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_ONCE */

/* Support for: #pragma deprecated("foo") */
#ifndef TPP_HAVE_PRAGMA_DEPRECATED
#define TPP_HAVE_PRAGMA_DEPRECATED (TPP_COMMON_HAVE_PRAGMA ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_DEPRECATED */

/* Support for: #pragma GCC poison foo */
#ifndef TPP_HAVE_PRAGMA_GCC_POISON
#define TPP_HAVE_PRAGMA_GCC_POISON (TPP_COMMON_HAVE_PRAGMA_GCC ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_GCC_POISON */

/* Support for: #pragma tpp_set_keyword_flags */
#ifndef TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS (TPP_COMMON_HAVE_PRAGMA ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

/* Support for: #pragma GCC system_header */
#ifndef TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#define TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER (TPP_COMMON_HAVE_PRAGMA_GCC ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

/* Support for: #pragma extension(...) */
#ifndef TPP_HAVE_PRAGMA_EXTENSION
#define TPP_HAVE_PRAGMA_EXTENSION ((TPP_COMMON_HAVE_PRAGMA && TPP_HAVE_EXTENSIONS) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_EXTENSION */

/* Support for: #pragma extension(push) */
#ifndef TPP_HAVE_PRAGMA_EXTENSION_PUSH
#define TPP_HAVE_PRAGMA_EXTENSION_PUSH ((TPP_HAVE_PRAGMA_EXTENSION && TPP_HAVE_EXTENSIONS_PUSH_POP) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_EXTENSION_PUSH */

/* Support for: #pragma warning(...) */
#ifndef TPP_HAVE_PRAGMA_WARNING
#define TPP_HAVE_PRAGMA_WARNING ((TPP_COMMON_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_WARNING */

/* Support for: #pragma warning(push) */
#ifndef TPP_HAVE_PRAGMA_WARNING_PUSH
#define TPP_HAVE_PRAGMA_WARNING_PUSH ((TPP_HAVE_PRAGMA_WARNING && TPP_HAVE_WARNINGS_PUSH_POP) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_WARNING_PUSH */
/************************************************************************/
/************************************************************************/
/************************************************************************/





/************************************************************************/
/* WARNINGS                                                             */
/************************************************************************/

/* Format to use for file+line+column log messages */
#ifndef TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT
#if TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV
#endif /* !TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT_IS_MSCV */

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
#define TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED                    \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_TPP_TOK_CHAR ||                     \
	                       TPP_HAVE_TPP_TOK_STRING ||                   \
	                       TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
	                       TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
	                       TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
	                       TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
	                       TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL ||       \
	                       TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL))
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
#ifndef TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#define TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF (TPP_HAVE_WARNINGS && TPP_HAVE_TPP_TOK_STRINGLIKE)
#endif /* !TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
#ifndef TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
#define TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF (TPP_HAVE_WARNINGS && TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE)
#endif /* !TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */
#ifndef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN
#define TPP_HAVE_TPP_W_UNEXPECTED_TOKEN (TPP_HAVE_WARNINGS)
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN */
#ifndef TPP_HAVE_TPP_W_ERROR
#define TPP_HAVE_TPP_W_ERROR (TPP_HAVE_WARNINGS && TPP_HAVE_CPP_ERROR)
#endif /* !TPP_HAVE_TPP_W_ERROR */
#ifndef TPP_HAVE_TPP_W_WARNING
#define TPP_HAVE_TPP_W_WARNING (TPP_HAVE_WARNINGS && TPP_HAVE_CPP_WARNING)
#endif /* !TPP_HAVE_TPP_W_WARNING */
#ifndef TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS
#define TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS (TPP_HAVE_WARNINGS && TPP_HAVE_PRAGMA)
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
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
#define TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE                   \
	(TPP_HAVE_WARNINGS && (TPP_HAVE_TPP_TOK_CHAR ||                     \
	                       TPP_HAVE_TPP_TOK_STRING ||                   \
	                       TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
	                       TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
	                       TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
	                       TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
	                       TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
	                       TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL))
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE */


/* Warning printer configuration */
#if TPP_HAVE_WARNINGS
#ifdef TPP_CONFIG_WARNING_PRINTER
#ifndef TPP_CONFIG_WARNING_PRINTER_NEEDS_ARG
#define TPP_CONFIG_WARNING_PRINTER_NEEDS_ARG 1
#endif /* !TPP_CONFIG_WARNING_PRINTER_NEEDS_ARG */

/* >> #define TPP_CONFIG_WARNING_PRINTER my_warning_printer
 * >> #if TPP_CONFIG_WARNING_PRINTER_NEEDS_ARG
 * >> static tpp_ssize TPP_FORMATPRINTER_CC
 * >> my_warning_printer(void *arg, tpp_char const *text, tpp_size num_bytes)
 * >> #else // TPP_CONFIG_WARNING_PRINTER_NEEDS_ARG
 * >> static tpp_ssize TPP_FORMATPRINTER_CC
 * >> my_warning_printer(tpp_char const *text, tpp_size num_bytes)
 * >> #endif // !TPP_CONFIG_WARNING_PRINTER_NEEDS_ARG
 * >> {
 * >>    ...
 * >> } */
#else /* TPP_CONFIG_WARNING_PRINTER */
/* Supply a built-in printer (that uses "fwrite(stderr)") when  */
#ifndef TPP_HAVE_BUILTIN_WARNPRINTER
#define TPP_HAVE_BUILTIN_WARNPRINTER 1
#endif /* !TPP_HAVE_BUILTIN_WARNPRINTER */
#endif /* !TPP_CONFIG_WARNING_PRINTER */
#endif /* TPP_HAVE_WARNINGS */
/************************************************************************/
/************************************************************************/
/************************************************************************/




/************************************************************************/
/* IMPLICIT API FEATURES                                                */
/************************************************************************/
/* Enable support for `TPP_FILE_IOFLAGS_SYSHDR' */
#ifndef TPP_HAVE_FILE_SYSHDR
#define TPP_HAVE_FILE_SYSHDR (TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER != 0)
#endif /* !TPP_HAVE_FILE_SYSHDR */

/* Enable support for `tpp_file::tf_prev' */
#ifndef TPP_HAVE_INCLUDE_STACK
#define TPP_HAVE_INCLUDE_STACK (TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT)
#endif /* !TPP_HAVE_INCLUDE_STACK */

/* Enable support for `tpp_keywords_openfile()' */
#ifndef TPP_HAVE_KEYWORDS_OPENFILE
#define TPP_HAVE_KEYWORDS_OPENFILE (TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || 1)
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE */

/* Enable support for `tpp_keywords_openfile_ex()' */
#ifndef TPP_HAVE_KEYWORDS_OPENFILE_EX
#define TPP_HAVE_KEYWORDS_OPENFILE_EX \
	(TPP_HAVE_KEYWORDS_OPENFILE && (TPP_HAVE_CPP_IMPORT || ((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT) && TPP_HAVE_PRAGMA_ONCE)))
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE_EX */

/* Enable support for `tpp_lexer_init_io_ex()' */
#ifndef TPP_HAVE_LEXER_INIT_IO
#define TPP_HAVE_LEXER_INIT_IO TPP_HAVE_FILE_NOKWD
#endif /* !TPP_HAVE_LEXER_INIT_IO */

/* Enable support for `tpp_lexer_init_filename()' */
#ifndef TPP_HAVE_LEXER_INIT_FILENAME
#define TPP_HAVE_LEXER_INIT_FILENAME TPP_HAVE_KEYWORDS_OPENFILE
#endif /* !TPP_HAVE_LEXER_INIT_FILENAME */

/* Enable support for `tpp_keyword_misc::tkm_file_guard' */
#ifndef TPP_HAVE_KEYWORD_FILE_GUARD
#define TPP_HAVE_KEYWORD_FILE_GUARD ((TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT) && TPP_HAVE_CPP_IF_ELSE_ENDIF)
#endif /* !TPP_HAVE_KEYWORD_FILE_GUARD */

/* Enable support for `tpp_lexer_skip_blocking()' */
#ifndef TPP_HAVE_LEXER_SKIP
#define TPP_HAVE_LEXER_SKIP (TPP_HAVE_PRAGMA_PUSH_MACRO || 1)
#endif /* !TPP_HAVE_LEXER_SKIP */

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

/* Provide a function "tpp_lexer_getkeyworddefined()" to check
 * if a given keyword is "defined()" (meaning it can be expanded
 * as a (potentially builtin) macro) */
#ifndef TPP_HAVE_LEXER_GETKEYWORDDEFINED
#define TPP_HAVE_LEXER_GETKEYWORDDEFINED \
	(TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_LEXER_GETKEYWORDDEFINED */

/************************************************************************/
/************************************************************************/
/************************************************************************/

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CONFIG_H */
