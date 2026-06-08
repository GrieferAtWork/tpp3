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

/* API features:
 *  0: Disabled
 *  1: Enabled
 *
 * (certain features only)
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
#define TPP_ERROR_LIMIT (-16)
#endif /* !TPP_ERROR_LIMIT */

/* Enable support for non-blocking I/O
 * Configure to "-1" to only enable compile-time support if supported by OS */
#ifndef TPP_HAVE_FILE_NONBLOCK
#define TPP_HAVE_FILE_NONBLOCK (-1)
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
#define TPP_HAVE_FILE_NOCLOSE 1 /* TODO: Default should be `0' */
#endif /* !TPP_HAVE_FILE_NOCLOSE */

/* Enable support for `TPP_FILE_IOFLAGS_NOKWD' */
#ifndef TPP_HAVE_FILE_NOKWD
#define TPP_HAVE_FILE_NOKWD 1
#endif /* !TPP_HAVE_FILE_NOKWD */

/* Speed up calls to `tpp_file_lcinfo()' by caching the last-read
 * position and determining line/column information as a delta from
 * what was previously cached */
#ifndef TPP_HAVE_FILE_LC_CACHE
#define TPP_HAVE_FILE_LC_CACHE 1
#endif /* !TPP_HAVE_FILE_LC_CACHE */


/* All feature- ("TPP_FEAT_*") / extension ("-f*") -style config options can be defined as:
 *  0: Compile-time disabled
 *  1: Compile-time enabled  (always on; no #pragma extension("-f...") / TPP_FEAT_* available)
 * -1: Runtime-time enabled  (with #pragma extension("-f...") / TPP_FEAT_*, default = true)
 * -2: Runtime-time disabled (with #pragma extension("-f...") / TPP_FEAT_*, default = false)
 */

#ifndef TPP_COMMON_HAVE_TPP_TOK
#define TPP_COMMON_HAVE_TPP_TOK (-1)
#endif /* !TPP_COMMON_HAVE_TPP_TOK */
#ifndef TPP_COMMON_HAVE_TPP_TOK_1CHAR
#define TPP_COMMON_HAVE_TPP_TOK_1CHAR (TPP_COMMON_HAVE_TPP_TOK < 0 ? (-2) : TPP_COMMON_HAVE_TPP_TOK)
#endif /* !TPP_COMMON_HAVE_TPP_TOK_1CHAR */
#ifndef TPP_COMMON_HAVE_TPP_TOK_SPACE
#define TPP_COMMON_HAVE_TPP_TOK_SPACE TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_SPACE */
#ifndef TPP_COMMON_HAVE_TPP_TOK_COMMENT
#define TPP_COMMON_HAVE_TPP_TOK_COMMENT TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_COMMENT */
#ifndef TPP_COMMON_HAVE_TPP_TOK_GENERIC
#define TPP_COMMON_HAVE_TPP_TOK_GENERIC TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_GENERIC */
#ifndef TPP_COMMON_HAVE_TPP_TOK_CXX_STRING
#define TPP_COMMON_HAVE_TPP_TOK_CXX_STRING TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_CXX_STRING */
#ifndef TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING
#define TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING */
#ifndef TPP_COMMON_HAVE_TPP_TOK_C_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_C_TOKENS TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_C_TOKENS */
#ifndef TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS */
#ifndef TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS */
#ifndef TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS
#define TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS TPP_COMMON_HAVE_TPP_TOK
#endif /* !TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS */

#ifndef TPP_COMMON_HAVE_CPP_DIRECTIVES
#define TPP_COMMON_HAVE_CPP_DIRECTIVES (-1)
#endif /* !TPP_COMMON_HAVE_CPP_DIRECTIVES */
#ifndef TPP_COMMON_HAVE_PRAGMA
#define TPP_COMMON_HAVE_PRAGMA TPP_COMMON_HAVE_CPP_DIRECTIVES
#endif /* !TPP_COMMON_HAVE_PRAGMA */
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
#define TPP_HAVE_TRIGRAPHS (-1) /* "-ftrigraphs" */
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
 * "%:%:" -> "##" (TPP_TOK_POUND_POUND) */
#ifndef TPP_HAVE_DIGRAPHS
#define TPP_HAVE_DIGRAPHS (-1) /* "-fdigraphs" */
#endif /* !TPP_HAVE_DIGRAPHS */

/* Configures if line-feed tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()' */
#ifndef TPP_HAVE_TPP_TOK_LF
#define TPP_HAVE_TPP_TOK_LF TPP_COMMON_HAVE_TPP_TOK_SPACE /* "TPP_FEAT_TPP_TOK_LF" */
#endif /* !TPP_HAVE_TPP_TOK_LF */

/* Configures if whitespace tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()' */
#ifndef TPP_HAVE_TPP_TOK_SPACE
#define TPP_HAVE_TPP_TOK_SPACE TPP_COMMON_HAVE_TPP_TOK_SPACE /* "TPP_FEAT_TPP_TOK_SPACE" */
#endif /* !TPP_HAVE_TPP_TOK_SPACE */

/* Configures if comment tokens should be forwarded, or filtered by `tpp_lexer_yieldpp()' */
#ifndef TPP_HAVE_TPP_TOK_COMMENT
#define TPP_HAVE_TPP_TOK_COMMENT (-1) /* "TPP_FEAT_TPP_TOK_COMMENT" */
#endif /* !TPP_HAVE_TPP_TOK_COMMENT */

/* Enable support for recognizing c++ comments: "// like this one!" */
#ifndef TPP_HAVE_TPP_TOK_CXX_COMMENT
#define TPP_HAVE_TPP_TOK_CXX_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "TPP_FEAT_TPP_TOK_CXX_COMMENT" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_COMMENT */

/* Enable support for recognizing c comments: "/" "* like this one! *" "/" */
#ifndef TPP_HAVE_TPP_TOK_C_COMMENT
#define TPP_HAVE_TPP_TOK_C_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "TPP_FEAT_TPP_TOK_C_COMMENT" */
#endif /* !TPP_HAVE_TPP_TOK_C_COMMENT */

/* Enable support for recognizing pascal comments: "(*like this one!*)" */
#ifndef TPP_HAVE_TPP_TOK_PASCAL_COMMENT
#define TPP_HAVE_TPP_TOK_PASCAL_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "TPP_FEAT_TPP_TOK_PASCAL_COMMENT" */
#endif /* !TPP_HAVE_TPP_TOK_PASCAL_COMMENT */

/* Enable support for recognizing shell comments: "# like this one!"
 * This still works in conjunction with "TPP_HAVE_CPP_DIRECTIVES", in
 * that unknown directives will simply be re-emit as shell comments,
 * and shell comments that don't appear at the start of lines are not
 * even processed as CPP directives. */
#ifndef TPP_HAVE_TPP_TOK_SHELL_COMMENT
#define TPP_HAVE_TPP_TOK_SHELL_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "TPP_FEAT_TPP_TOK_SHELL_COMMENT" */
#endif /* !TPP_HAVE_TPP_TOK_SHELL_COMMENT */

/* Enable support for recognizing ASM comments: "/ like this one!" */
#ifndef TPP_HAVE_TPP_TOK_ASM_COMMENT
#define TPP_HAVE_TPP_TOK_ASM_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "TPP_FEAT_TPP_TOK_ASM_COMMENT" */
#endif /* !TPP_HAVE_TPP_TOK_ASM_COMMENT */

/* Enable support for recognizing SQL comments: "-- like this one!" */
#ifndef TPP_HAVE_TPP_TOK_SQL_COMMENT
#define TPP_HAVE_TPP_TOK_SQL_COMMENT TPP_COMMON_HAVE_TPP_TOK_COMMENT /* "TPP_FEAT_TPP_TOK_SQL_COMMENT" */
#endif /* !TPP_HAVE_TPP_TOK_SQL_COMMENT */

/************************************************************************/
/* Single-char tokens                                                   */
/************************************************************************/

/* "$" (treat as its own token, rather than as part of identifiers/keywords) */
#ifndef TPP_HAVE_TPP_TOK_DOLLAR
#define TPP_HAVE_TPP_TOK_DOLLAR TPP_COMMON_HAVE_TPP_TOK_1CHAR /* "TPP_FEAT_TPP_TOK_DOLLAR" */
#endif /* !TPP_HAVE_TPP_TOK_DOLLAR */

/************************************************************************/
/* Number tokens                                                        */
/************************************************************************/

/* 123 */
#ifndef TPP_HAVE_TPP_TOK_INT
#define TPP_HAVE_TPP_TOK_INT TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "TPP_FEAT_TPP_TOK_INT" */
#endif /* !TPP_HAVE_TPP_TOK_INT */

/* 123.0 */
#ifndef TPP_HAVE_TPP_TOK_FLOAT
#define TPP_HAVE_TPP_TOK_FLOAT TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "TPP_FEAT_TPP_TOK_FLOAT" */
#endif /* !TPP_HAVE_TPP_TOK_FLOAT */

/************************************************************************/
/* String tokens                                                        */
/************************************************************************/

/* 'foo' */
#ifndef TPP_HAVE_TPP_TOK_CHAR
#define TPP_HAVE_TPP_TOK_CHAR TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "TPP_FEAT_TPP_TOK_CHAR" */
#endif /* !TPP_HAVE_TPP_TOK_CHAR */

/* "foo" */
#ifndef TPP_HAVE_TPP_TOK_STRING
#define TPP_HAVE_TPP_TOK_STRING TPP_COMMON_HAVE_TPP_TOK_GENERIC /* "TPP_FEAT_TPP_TOK_STRING" */
#endif /* !TPP_HAVE_TPP_TOK_STRING */

/* R"AB(foo)AB" */
#ifndef TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING /* "TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */

/* L"foo"  (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: LR"AB(foo)AB") */
#ifndef TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING /* "TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */

/* u8"foo" (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: u8R"AB(foo)AB") */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING /* "TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */

/* u"foo"  (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: uR"AB(foo)AB") */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING /* "TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */

/* U"foo"  (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL: UR"AB(foo)AB") */
#ifndef TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_CXX_STRING /* "TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL" */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */

/* R"foo"  r"foo"  */
#ifndef TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL" */
#endif /* !TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */

/* R'bar'  r'bar' */
#ifndef TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL" */
#endif /* !TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */

/* """foo""" */
#ifndef TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
#define TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL" */
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */

/* '''foo''' */
#ifndef TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
#define TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL TPP_COMMON_HAVE_TPP_TOK_DEEMON_STRING /* "TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL" */
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */

/************************************************************************/
/* Multi-char tokens                                                    */
/************************************************************************/

/* "<<" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_LANGLE_LANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE */

/* ">>" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_RANGLE_RANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE */

/* "==" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL */

/* "!=" */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */

/* ">=" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_RANGLE_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_EQUAL */

/* "<=" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_LANGLE_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_EQUAL */

/* "..." */
#ifndef TPP_HAVE_TPP_TOK_DOT_DOT_DOT
#define TPP_HAVE_TPP_TOK_DOT_DOT_DOT ((TPP_COMMON_HAVE_TPP_TOK_C_TOKENS < 0) ? TPP_COMMON_HAVE_TPP_TOK_C_TOKENS : (TPP_COMMON_HAVE_TPP_TOK_C_TOKENS || TPP_HAVE_VA_ARGS_IN_MACROS || TPP_HAVE_NAMED_VARARGS_IN_MACROS)) /* "TPP_FEAT_TPP_TOK_DOT_DOT_DOT" */
#endif /* !TPP_HAVE_TPP_TOK_DOT_DOT_DOT */

/* "+=" */
#ifndef TPP_HAVE_TPP_TOK_PLUS_EQUAL
#define TPP_HAVE_TPP_TOK_PLUS_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_PLUS_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_PLUS_EQUAL */

/* "-=" */
#ifndef TPP_HAVE_TPP_TOK_MINUS_EQUAL
#define TPP_HAVE_TPP_TOK_MINUS_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_MINUS_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_EQUAL */

/* "*=" */
#ifndef TPP_HAVE_TPP_TOK_STAR_EQUAL
#define TPP_HAVE_TPP_TOK_STAR_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_STAR_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_EQUAL */

/* "/=" */
#ifndef TPP_HAVE_TPP_TOK_SLASH_EQUAL
#define TPP_HAVE_TPP_TOK_SLASH_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_SLASH_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_EQUAL */

/* "%=" */
#ifndef TPP_HAVE_TPP_TOK_PERCENT_EQUAL
#define TPP_HAVE_TPP_TOK_PERCENT_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_PERCENT_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_PERCENT_EQUAL */

/* "<<=" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */

/* ">>=" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */

/* "&=" */
#ifndef TPP_HAVE_TPP_TOK_AMP_EQUAL
#define TPP_HAVE_TPP_TOK_AMP_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_AMP_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_AMP_EQUAL */

/* "|=" */
#ifndef TPP_HAVE_TPP_TOK_PIPE_EQUAL
#define TPP_HAVE_TPP_TOK_PIPE_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_PIPE_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_PIPE_EQUAL */

/* "^=" */
#ifndef TPP_HAVE_TPP_TOK_HAT_EQUAL
#define TPP_HAVE_TPP_TOK_HAT_EQUAL TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_HAT_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_HAT_EQUAL */

/* "//" */
#ifndef TPP_HAVE_TPP_TOK_SLASH_SLASH
#define TPP_HAVE_TPP_TOK_SLASH_SLASH TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_SLASH_SLASH" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_SLASH */

/* "//=" */
#ifndef TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
#define TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */

/* "**=" */
#ifndef TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
#define TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */

/* "@=" */
#ifndef TPP_HAVE_TPP_TOK_AT_EQUAL
#define TPP_HAVE_TPP_TOK_AT_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_AT_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_AT_EQUAL */

/* "##" */
#ifndef TPP_HAVE_TPP_TOK_POUND_POUND
#define TPP_HAVE_TPP_TOK_POUND_POUND ((TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS < 0) ? TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS : (TPP_HAVE_GLUE_MACRO_ARGUMENT || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS)) /* "TPP_FEAT_TPP_TOK_POUND_POUND" */
#endif /* !TPP_HAVE_TPP_TOK_POUND_POUND */

/* "&&" */
#ifndef TPP_HAVE_TPP_TOK_AMP_AMP
#define TPP_HAVE_TPP_TOK_AMP_AMP TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_AMP_AMP" */
#endif /* !TPP_HAVE_TPP_TOK_AMP_AMP */

/* "||" */
#ifndef TPP_HAVE_TPP_TOK_PIPE_PIPE
#define TPP_HAVE_TPP_TOK_PIPE_PIPE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_PIPE_PIPE" */
#endif /* !TPP_HAVE_TPP_TOK_PIPE_PIPE */

/* "^^" */
#ifndef TPP_HAVE_TPP_TOK_HAT_HAT
#define TPP_HAVE_TPP_TOK_HAT_HAT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_HAT_HAT" */
#endif /* !TPP_HAVE_TPP_TOK_HAT_HAT */

/* "++" */
#ifndef TPP_HAVE_TPP_TOK_PLUS_PLUS
#define TPP_HAVE_TPP_TOK_PLUS_PLUS TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_PLUS_PLUS" */
#endif /* !TPP_HAVE_TPP_TOK_PLUS_PLUS */

/* "--" */
#ifndef TPP_HAVE_TPP_TOK_MINUS_MINUS
#define TPP_HAVE_TPP_TOK_MINUS_MINUS TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_MINUS_MINUS" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_MINUS */

/* "**" */
#ifndef TPP_HAVE_TPP_TOK_STAR_STAR
#define TPP_HAVE_TPP_TOK_STAR_STAR TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_STAR_STAR" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_STAR */

/* "~~" */
#ifndef TPP_HAVE_TPP_TOK_TILDE_TILDE
#define TPP_HAVE_TPP_TOK_TILDE_TILDE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_TILDE_TILDE" */
#endif /* !TPP_HAVE_TPP_TOK_TILDE_TILDE */

/* "~=" */
#ifndef TPP_HAVE_TPP_TOK_TILDE_EQUAL
#define TPP_HAVE_TPP_TOK_TILDE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_TILDE_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_TILDE_EQUAL */

/* "->" */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE TPP_COMMON_HAVE_TPP_TOK_C_TOKENS /* "TPP_FEAT_TPP_TOK_MINUS_RANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE */

/* ":=" */
#ifndef TPP_HAVE_TPP_TOK_COLON_EQUAL
#define TPP_HAVE_TPP_TOK_COLON_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_COLON_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_COLON_EQUAL */

/* "::" */
#ifndef TPP_HAVE_TPP_TOK_COLON_COLON
#define TPP_HAVE_TPP_TOK_COLON_COLON TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS /* "TPP_FEAT_TPP_TOK_COLON_COLON" */
#endif /* !TPP_HAVE_TPP_TOK_COLON_COLON */

/* "->*" */
#ifndef TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS /* "TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */

/* ".*" */
#ifndef TPP_HAVE_TPP_TOK_DOT_STAR
#define TPP_HAVE_TPP_TOK_DOT_STAR TPP_COMMON_HAVE_TPP_TOK_CXX_TOKENS /* "TPP_FEAT_TPP_TOK_DOT_STAR" */
#endif /* !TPP_HAVE_TPP_TOK_DOT_STAR */

/* ".." */
#ifndef TPP_HAVE_TPP_TOK_DOT_DOT
#define TPP_HAVE_TPP_TOK_DOT_DOT TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_DOT_DOT" */
#endif /* !TPP_HAVE_TPP_TOK_DOT_DOT */

/* "<>" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_LANGLE_RANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_RANGLE */

/* "<<<" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */

/* ">>>" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */

/* "<<<=" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */

/* ">>>=" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */

/* "===" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */

/* "!==" */
#ifndef TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */

/* "??" */
#ifndef TPP_HAVE_TPP_TOK_QMARK_QMARK
#define TPP_HAVE_TPP_TOK_QMARK_QMARK TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_QMARK_QMARK" */
#endif /* !TPP_HAVE_TPP_TOK_QMARK_QMARK */

/* "?=" */
#ifndef TPP_HAVE_TPP_TOK_QMARK_EQUAL
#define TPP_HAVE_TPP_TOK_QMARK_EQUAL TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_QMARK_EQUAL" */
#endif /* !TPP_HAVE_TPP_TOK_QMARK_EQUAL */

/* "><" */
#ifndef TPP_HAVE_TPP_TOK_RANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_RANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_RANGLE_LANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_RANGLE_LANGLE */

/* "=+" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PLUS
#define TPP_HAVE_TPP_TOK_EQUAL_PLUS TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_PLUS" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PLUS */

/* "=-" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_MINUS
#define TPP_HAVE_TPP_TOK_EQUAL_MINUS TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_MINUS" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_MINUS */

/* "=*" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_STAR
#define TPP_HAVE_TPP_TOK_EQUAL_STAR TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_STAR" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_STAR */

/* "=**" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
#define TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */

/* "=/" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_SLASH
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_SLASH" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_SLASH */

/* "=//" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_SLASH_SLASH" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */

/* "=%" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PERCENT
#define TPP_HAVE_TPP_TOK_EQUAL_PERCENT TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_PERCENT" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PERCENT */

/* "=&" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_AMP
#define TPP_HAVE_TPP_TOK_EQUAL_AMP TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_AMP" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_AMP */

/* "=|" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_PIPE
#define TPP_HAVE_TPP_TOK_EQUAL_PIPE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_PIPE" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_PIPE */

/* "=^" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_HAT
#define TPP_HAVE_TPP_TOK_EQUAL_HAT TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_HAT" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_HAT */

/* "=<" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_LANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_LANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE */

/* "=<<" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */

/* "=<<<" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */

/* "=>" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_RANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_RANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE */

/* "=>>" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */

/* "=>>>" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */

/* "=@" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_AT
#define TPP_HAVE_TPP_TOK_EQUAL_AT TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_AT" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_AT */

/* "=~" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_TILDE
#define TPP_HAVE_TPP_TOK_EQUAL_TILDE TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_TILDE" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_TILDE */

/* "=:" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_COLON
#define TPP_HAVE_TPP_TOK_EQUAL_COLON TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_COLON" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_COLON */

/* "=!" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
#define TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_EXCLAIM" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */

/* "==!" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */

/* "=?" */
#ifndef TPP_HAVE_TPP_TOK_EQUAL_QMARK
#define TPP_HAVE_TPP_TOK_EQUAL_QMARK TPP_COMMON_HAVE_TPP_TOK_MISC_TOKENS /* "TPP_FEAT_TPP_TOK_EQUAL_QMARK" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_QMARK */

/* "<-" */
#ifndef TPP_HAVE_TPP_TOK_LANGLE_MINUS
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_LANGLE_MINUS" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS */

/* "*<-" */
#ifndef TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
#define TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_STAR_LANGLE_MINUS" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */

/* "*." */
#ifndef TPP_HAVE_TPP_TOK_STAR_DOT
#define TPP_HAVE_TPP_TOK_STAR_DOT TPP_COMMON_HAVE_TPP_TOK_REVERSE_TOKENS /* "TPP_FEAT_TPP_TOK_STAR_DOT" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_DOT */



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
#define TPP_HAVE_BSE (-1) /* "-fbse" */
#endif /* !TPP_HAVE_BSE */

/* Extension to `TPP_HAVE_BSE': the \ backslash is allowed to be followed by extra
 * whitespace preceding the actual linefeed
 *
 * This DOES affect the line-continuation features of C++ // comments, and
 * multi-line macro definitions. */
#ifndef TPP_HAVE_BSE_WHITESPACE
#define TPP_HAVE_BSE_WHITESPACE (TPP_HAVE_BSE ? -1 : 0) /* "-fbse-whitespace" */
#endif /* !TPP_HAVE_BSE_WHITESPACE */

/* Support for \uABCD and \U01234567 in identifier names (will be
 * replaced with effective UTF-8 encodings when translated to keywords) */
#ifndef TPP_HAVE_ESCAPE_IN_IDENTIFIERS
#define TPP_HAVE_ESCAPE_IN_IDENTIFIERS (-1) /* "-fescape-in-identifiers" */
#endif /* !TPP_HAVE_ESCAPE_IN_IDENTIFIERS */

/* Support for "\e" (for U+001B) escape sequences */
#ifndef TPP_HAVE_ESCAPE_E_IN_STRINGS
#define TPP_HAVE_ESCAPE_E_IN_STRINGS (TPP_HAVE_STRING_ESCAPE ? -1 : 0) /* "-fescape-e-in-strings" */
#endif /* !TPP_HAVE_ESCAPE_E_IN_STRINGS */

/* Support for "\s" (for U+0020) escape sequences */
#ifndef TPP_HAVE_ESCAPE_S_IN_STRINGS
#define TPP_HAVE_ESCAPE_S_IN_STRINGS (TPP_HAVE_STRING_ESCAPE ? -1 : 0) /* "-fescape-s-in-strings" */
#endif /* !TPP_HAVE_ESCAPE_S_IN_STRINGS */

/* Specifies if *any* CPP directives are supported */
#ifndef TPP_HAVE_CPP_DIRECTIVES
#define TPP_HAVE_CPP_DIRECTIVES TPP_COMMON_HAVE_CPP_DIRECTIVES /* "TPP_FEAT_CPP_DIRECTIVES" */
#endif /* !TPP_HAVE_CPP_DIRECTIVES */

/* Support for C-style macros */
#ifndef TPP_HAVE_CPP_MACROS
#define TPP_HAVE_CPP_MACROS (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "TPP_FEAT_CPP_MACROS" */
#endif /* !TPP_HAVE_CPP_MACROS */

/* Support for: #!foobar  (comments) */
#ifndef TPP_HAVE_CPP_EXCLAIM
#define TPP_HAVE_CPP_EXCLAIM (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "-fshebang-directives" */
#endif /* !TPP_HAVE_CPP_EXCLAIM */

/* Support for: #  (blank line) */
#ifndef TPP_HAVE_CPP_BLANK
#define TPP_HAVE_CPP_BLANK (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "TPP_FEAT_CPP_BLANK" */
#endif /* !TPP_HAVE_CPP_BLANK */

/* Support for: # 42 ...  (similar to #line) */
#ifndef TPP_HAVE_CPP_DIGIT_LINE
#define TPP_HAVE_CPP_DIGIT_LINE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "TPP_FEAT_CPP_DIGIT_LINE" */
#endif /* !TPP_HAVE_CPP_DIGIT_LINE */

/* Support for: #line ... */
#ifndef TPP_HAVE_CPP_LINE
#define TPP_HAVE_CPP_LINE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "TPP_FEAT_CPP_LINE" */
#endif /* !TPP_HAVE_CPP_LINE */

/* Support for: #include */
#ifndef TPP_HAVE_CPP_INCLUDE
#define TPP_HAVE_CPP_INCLUDE (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "-finclude-directives" */
#endif /* !TPP_HAVE_CPP_INCLUDE */

/* Support for: #include_next */
#ifndef TPP_HAVE_CPP_INCLUDE_NEXT
#define TPP_HAVE_CPP_INCLUDE_NEXT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "-finclude-next-directives" */
#endif /* !TPP_HAVE_CPP_INCLUDE_NEXT */

/* Support for: #import */
#ifndef TPP_HAVE_CPP_IMPORT
#define TPP_HAVE_CPP_IMPORT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "-fimport-directives" */
#endif /* !TPP_HAVE_CPP_IMPORT */

/* Support for: #if, #ifdef, #ifndef, #elif, #elifdef, #elifndef, #else, #endif */
#ifndef TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_HAVE_CPP_IF_ELSE_ENDIF ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "TPP_FEAT_CPP_IF_ELSE_ENDIF" */
#endif /* !TPP_HAVE_CPP_IF_ELSE_ENDIF */

/* Support for: #define, #undef */
#ifndef TPP_HAVE_CPP_DEFINE
#define TPP_HAVE_CPP_DEFINE ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_DIRECTIVES) ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "TPP_FEAT_CPP_DEFINE" */
#endif /* !TPP_HAVE_CPP_DEFINE */

/* Support for: #assert, #unassert */
#ifndef TPP_HAVE_CPP_ASSERT
#define TPP_HAVE_CPP_ASSERT (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "-fassertions" */
#endif /* !TPP_HAVE_CPP_ASSERT */

/* Support for: #error */
#ifndef TPP_HAVE_CPP_ERROR
#define TPP_HAVE_CPP_ERROR (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "-ferror-directives" */
#endif /* !TPP_HAVE_CPP_ERROR */

/* Support for: #warning */
#ifndef TPP_HAVE_CPP_WARNING
#define TPP_HAVE_CPP_WARNING (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "-fwarning-directives" */
#endif /* !TPP_HAVE_CPP_WARNING */

/* Support for: #ident, #sccs */
#ifndef TPP_HAVE_CPP_IDENT_SCCS
#define TPP_HAVE_CPP_IDENT_SCCS (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "-fident-directives" */
#endif /* !TPP_HAVE_CPP_IDENT_SCCS */

/* Support for: #pragma */
#ifndef TPP_HAVE_CPP_PRAGMA
#define TPP_HAVE_CPP_PRAGMA (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "TPP_FEAT_CPP_PRAGMA" */
#endif /* !TPP_HAVE_CPP_PRAGMA */

/* Support for: #embed */
#ifndef TPP_HAVE_CPP_EMBED
#define TPP_HAVE_CPP_EMBED (TPP_HAVE_CPP_DIRECTIVES ? TPP_COMMON_HAVE_CPP_DIRECTIVES : 0) /* "TPP_FEAT_CPP_EMBED" */
#endif /* !TPP_HAVE_CPP_EMBED */

/* Support for: _Pragma("foo") */
#ifndef TPP_HAVE_MACRO__Pragma
#define TPP_HAVE_MACRO__Pragma TPP_COMMON_HAVE_PRAGMA /* "-f_Pragma" */
#endif /* !TPP_HAVE_MACRO__Pragma */

/* Support for: __pragma(foo) */
#ifndef TPP_HAVE_MACRO___pragma
#define TPP_HAVE_MACRO___pragma TPP_COMMON_HAVE_PRAGMA /* "-f__pragma" */
#endif /* !TPP_HAVE_MACRO___pragma */


/* Support for pragma directives */
#undef TPP_HAVE_PRAGMA
#define TPP_HAVE_PRAGMA \
	(TPP_HAVE_CPP_PRAGMA || TPP_HAVE_MACRO__Pragma || TPP_HAVE_MACRO___pragma)


/* Support for clang __has_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_attribute
#define TPP_HAVE_CLANG_MACRO___has_attribute (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fclang-__has_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_attribute */

/* Support for clang __has_builtin */
#ifndef TPP_HAVE_CLANG_MACRO___has_builtin
#define TPP_HAVE_CLANG_MACRO___has_builtin (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fclang-__has_builtin" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_builtin */

/* Support for clang __has_cpp_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_cpp_attribute
#define TPP_HAVE_CLANG_MACRO___has_cpp_attribute (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fclang-__has_cpp_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_cpp_attribute */

/* Support for clang __has_declspec_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_declspec_attribute
#define TPP_HAVE_CLANG_MACRO___has_declspec_attribute (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fclang-__has_declspec_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_declspec_attribute */

/* Support for clang __has_extension */
#ifndef TPP_HAVE_CLANG_MACRO___has_extension
#define TPP_HAVE_CLANG_MACRO___has_extension (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fclang-__has_extension" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_extension */

/* Support for clang __has_feature */
#ifndef TPP_HAVE_CLANG_MACRO___has_feature
#define TPP_HAVE_CLANG_MACRO___has_feature (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fclang-__has_feature" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_feature */

/* Support for clang __has_c_attribute */
#ifndef TPP_HAVE_CLANG_MACRO___has_c_attribute
#define TPP_HAVE_CLANG_MACRO___has_c_attribute (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fclang-__has_c_attribute" */
#endif /* !TPP_HAVE_CLANG_MACRO___has_c_attribute */

/* When enabled, clang's __has_feature() also expands to "1" when __has_extension() would. */
#ifndef TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES
#define TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES ((TPP_HAVE_CLANG_MACRO___has_extension && TPP_HAVE_CLANG_MACRO___has_feature) ? -1 : 0) /* "-fclang-extensions-are-features" */
#endif /* !TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES */

/* Support for clang __is_identifier */
#ifndef TPP_HAVE_MACRO___is_identifier
#define TPP_HAVE_MACRO___is_identifier (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-f__is_identifier" */
#endif /* !TPP_HAVE_MACRO___is_identifier */

/* Support for TPP's __is_deprecated */
#ifndef TPP_HAVE_MACRO___is_deprecated
#define TPP_HAVE_MACRO___is_deprecated (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-f__is_deprecated" */
#endif /* !TPP_HAVE_MACRO___is_deprecated */

/* Support for TPP's __is_poisoned */
#ifndef TPP_HAVE_MACRO___is_poisoned
#define TPP_HAVE_MACRO___is_poisoned (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-f__is_poisoned" */
#endif /* !TPP_HAVE_MACRO___is_poisoned */

/* Support for TPP's __has_extension */
#ifndef TPP_HAVE_MACRO___has_extension
#define TPP_HAVE_MACRO___has_extension ((TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_MACROS) ? -1 : 0) /* "-f__has_extension" */
#endif /* !TPP_HAVE_MACRO___has_extension */

/* Support for TPP's __has_known_extension */
#ifndef TPP_HAVE_MACRO___has_known_extension
#define TPP_HAVE_MACRO___has_known_extension ((TPP_HAVE_EXTENSIONS && TPP_HAVE_CPP_MACROS) ? -1 : 0) /* "-f__has_known_extension" */
#endif /* !TPP_HAVE_MACRO___has_known_extension */

/* Support for TPP's __has_warning */
#ifndef TPP_HAVE_MACRO___has_warning
#define TPP_HAVE_MACRO___has_warning ((TPP_HAVE_WARNINGS && TPP_HAVE_CPP_MACROS) ? -1 : 0) /* "-f__has_warning" */
#endif /* !TPP_HAVE_MACRO___has_warning */

/* Support for TPP's __has_known_warning */
#ifndef TPP_HAVE_MACRO___has_known_warning
#define TPP_HAVE_MACRO___has_known_warning ((TPP_HAVE_WARNINGS && TPP_HAVE_CPP_MACROS) ? -1 : 0) /* "-f__has_known_warning" */
#endif /* !TPP_HAVE_MACRO___has_known_warning */

/* Support for clang __has_include */
#ifndef TPP_HAVE_MACRO___has_include
#define TPP_HAVE_MACRO___has_include ((TPP_HAVE_CPP_MACROS && (TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_IMPORT)) ? -1 : 0) /* "-f__has_include" */
#endif /* !TPP_HAVE_MACRO___has_include */

/* Support for clang __has_include_next */
#ifndef TPP_HAVE_MACRO___has_include_next
#define TPP_HAVE_MACRO___has_include_next ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_INCLUDE_NEXT) ? -1 : 0) /* "-f__has_include_next" */
#endif /* !TPP_HAVE_MACRO___has_include_next */

/* Support for clang __has_embed */
#ifndef TPP_HAVE_MACRO___has_embed
#define TPP_HAVE_MACRO___has_embed ((TPP_HAVE_CPP_MACROS && TPP_HAVE_CPP_EMBED) ? -1 : 0) /* "-f__has_embed" */
#endif /* !TPP_HAVE_MACRO___has_embed */

/* __FILE__ */
#ifndef TPP_HAVE_MACRO___FILE__
#define TPP_HAVE_MACRO___FILE__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-f__FILE__" */
#endif /* !TPP_HAVE_MACRO___FILE__ */

/* __LINE__ */
#ifndef TPP_HAVE_MACRO___LINE__
#define TPP_HAVE_MACRO___LINE__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-f__LINE__" */
#endif /* !TPP_HAVE_MACRO___LINE__ */

/* __TIME__ */
#ifndef TPP_HAVE_MACRO___TIME__
#define TPP_HAVE_MACRO___TIME__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-f__TIME__" */
#endif /* !TPP_HAVE_MACRO___TIME__ */

/* __DATE__ */
#ifndef TPP_HAVE_MACRO___DATE__
#define TPP_HAVE_MACRO___DATE__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-f__DATE__" */
#endif /* !TPP_HAVE_MACRO___DATE__ */

/* __COLUMN__ */
#ifndef TPP_HAVE_MACRO___COLUMN__
#define TPP_HAVE_MACRO___COLUMN__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fcolumn-macro" */
#endif /* !TPP_HAVE_MACRO___COLUMN__ */

/* __BASE_FILE__ */
#ifndef TPP_HAVE_MACRO___BASE_FILE__
#define TPP_HAVE_MACRO___BASE_FILE__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fbasefile-macro" */
#endif /* !TPP_HAVE_MACRO___BASE_FILE__ */

/* __INCLUDE_LEVEL__ */
#ifndef TPP_HAVE_MACRO___INCLUDE_LEVEL__
#define TPP_HAVE_MACRO___INCLUDE_LEVEL__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-finclude-level-macro" */
#endif /* !TPP_HAVE_MACRO___INCLUDE_LEVEL__ */

/* __INCLUDE_DEPTH__ */
#ifndef TPP_HAVE_MACRO___INCLUDE_DEPTH__
#define TPP_HAVE_MACRO___INCLUDE_DEPTH__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-finclude-depth-macro" */
#endif /* !TPP_HAVE_MACRO___INCLUDE_DEPTH__ */

/* __COUNTER__ */
#ifndef TPP_HAVE_MACRO___COUNTER__
#define TPP_HAVE_MACRO___COUNTER__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fcounter-macro" */
#endif /* !TPP_HAVE_MACRO___COUNTER__ */

/* __TIMESTAMP__ */
#ifndef TPP_HAVE_MACRO___TIMESTAMP__
#define TPP_HAVE_MACRO___TIMESTAMP__ (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftimestamp-macro" */
#endif /* !TPP_HAVE_MACRO___TIMESTAMP__ */

/* __DATE_DAY__, __DATE_WDAY__, __DATE_YDAY__, __DATE_MONTH__, __DATE_YEAR__ */
#ifndef TPP_HAVE_NUMERIC_DATE_MACROS
#define TPP_HAVE_NUMERIC_DATE_MACROS (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fnumeric-date-macros" */
#endif /* !TPP_HAVE_NUMERIC_DATE_MACROS */

/* __TIME_SEC__, __TIME_MIN__, __TIME_HOUR__ */
#ifndef TPP_HAVE_NUMERIC_TIME_MACROS
#define TPP_HAVE_NUMERIC_TIME_MACROS (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fnumeric-time-macros" */
#endif /* !TPP_HAVE_NUMERIC_TIME_MACROS */

/* __TPP_EVAL */
#ifndef TPP_HAVE_MACRO___TPP_EVAL
#define TPP_HAVE_MACRO___TPP_EVAL (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-eval-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */

/* __TPP_UNIQUE */
#ifndef TPP_HAVE_MACRO___TPP_UNIQUE
#define TPP_HAVE_MACRO___TPP_UNIQUE (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-unique-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_UNIQUE */

/* __TPP_LOAD_FILE */
#ifndef TPP_HAVE_MACRO___TPP_LOAD_FILE
#define TPP_HAVE_MACRO___TPP_LOAD_FILE (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-load-file-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_LOAD_FILE */

/* __TPP_COUNTER */
#ifndef TPP_HAVE_MACRO___TPP_COUNTER
#define TPP_HAVE_MACRO___TPP_COUNTER (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-counter-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */

/* __TPP_RANDOM */
#ifndef TPP_HAVE_MACRO___TPP_RANDOM
#define TPP_HAVE_MACRO___TPP_RANDOM (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-random-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_RANDOM */

/* __TPP_STR_DECOMPILE */
#ifndef TPP_HAVE_MACRO___TPP_STR_DECOMPILE
#define TPP_HAVE_MACRO___TPP_STR_DECOMPILE (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-str-decompile-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_DECOMPILE */

/* __TPP_STR_SUBSTR */
#ifndef TPP_HAVE_MACRO___TPP_STR_SUBSTR
#define TPP_HAVE_MACRO___TPP_STR_SUBSTR (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-str-substr-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SUBSTR */

/* __TPP_STR_PACK */
#ifndef TPP_HAVE_MACRO___TPP_STR_PACK
#define TPP_HAVE_MACRO___TPP_STR_PACK (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-str-pack-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_PACK */

/* __TPP_STR_SIZE */
#ifndef TPP_HAVE_MACRO___TPP_STR_SIZE
#define TPP_HAVE_MACRO___TPP_STR_SIZE (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-str-size-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SIZE */

/* __TPP_COUNT_TOKENS */
#ifndef TPP_HAVE_MACRO___TPP_COUNT_TOKENS
#define TPP_HAVE_MACRO___TPP_COUNT_TOKENS (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-count-tokens-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_COUNT_TOKENS */

/* __TPP_IDENTIFIER */
#ifndef TPP_HAVE_MACRO___TPP_IDENTIFIER
#define TPP_HAVE_MACRO___TPP_IDENTIFIER (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-ftpp-identifier-macro" */
#endif /* !TPP_HAVE_MACRO___TPP_IDENTIFIER */


/* Support for: #define point<T> ... */
#ifndef TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-falternative-macro-parenthesis" */
#endif /* !TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

/* Support for retaining whitespace around macro arguments */
#ifndef TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
#define TPP_HAVE_MACRO_ARGUMENT_WHITESPACE (TPP_HAVE_CPP_MACROS ? -2 : 0) /* "-fmacro-argument-whitespace" */
#endif /* !TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */

/* Support for: #pragma extension("-fmacro-recursion")
 * NOTE: affects behavior of macros at the *TIME OF DEFINITION* */
#ifndef TPP_HAVE_MACRO_RECURSION
#define TPP_HAVE_MACRO_RECURSION (TPP_HAVE_CPP_MACROS ? -2 : 0) /* "-fmacro-recursion" */
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
#ifndef TPP_HAVE_TRADITIONAL_MACROS
#define TPP_HAVE_TRADITIONAL_MACROS (TPP_HAVE_CPP_MACROS ? -2 : 0) /* "-ftraditional-macro" */
#endif /* !TPP_HAVE_TRADITIONAL_MACROS */

/* Support for: #define printf(format, args...) args */
#ifndef TPP_HAVE_NAMED_VARARGS_IN_MACROS
#define TPP_HAVE_NAMED_VARARGS_IN_MACROS (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fnamed-varargs-in-macros" */
#endif /* !TPP_HAVE_NAMED_VARARGS_IN_MACROS */

/* Support for: #define printf(format, ...) __VA_ARGS__ */
#ifndef TPP_HAVE_VA_ARGS_IN_MACROS
#define TPP_HAVE_VA_ARGS_IN_MACROS (TPP_HAVE_CPP_MACROS ? -1 : 0) /* "-fva-args-in-macros" */
#endif /* !TPP_HAVE_VA_ARGS_IN_MACROS */

/* Support for: #define printf(format, ...) fprintf(stderr, format __VA_COMMA__ __VA_ARGS__) */
#ifndef TPP_HAVE_VA_COMMA_IN_MACROS
#define TPP_HAVE_VA_COMMA_IN_MACROS ((TPP_HAVE_CPP_MACROS && (TPP_HAVE_TRADITIONAL_MACROS <= 0)) ? -1 : 0) /* "-fva-comma-in-macros" */
#endif /* !TPP_HAVE_VA_COMMA_IN_MACROS */

/* Support for: #define printf(format, ...) fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__) */
#ifndef TPP_HAVE_VA_OPT_IN_MACROS
#define TPP_HAVE_VA_OPT_IN_MACROS ((TPP_HAVE_CPP_MACROS && (TPP_HAVE_TRADITIONAL_MACROS <= 0)) ? -1 : 0) /* "-fva-opt-in-macros" */
#endif /* !TPP_HAVE_VA_OPT_IN_MACROS */

/* Support for: #define min(...) min_##__VA_NARGS__(__VA_ARGS__) */
#ifndef TPP_HAVE_VA_NARGS_IN_MACROS
#define TPP_HAVE_VA_NARGS_IN_MACROS ((TPP_HAVE_CPP_MACROS && (TPP_HAVE_TRADITIONAL_MACROS <= 0)) ? -1 : 0) /* "-fva-nargs-in-macros" */
#endif /* !TPP_HAVE_VA_NARGS_IN_MACROS */

/* Support for: #define printf(format, ...) fprintf(stderr, format,##__VA_ARGS__) */
#ifndef TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
#define TPP_HAVE_VA_GLUE_COMMA_IN_MACROS ((TPP_HAVE_CPP_MACROS && (TPP_HAVE_TRADITIONAL_MACROS <= 0)) ? -1 : 0) /* "-fglue-comma-in-macros" */
#endif /* !TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */

/* Support for: #define str(x) #x */
#ifndef TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
#define TPP_HAVE_STRINGIZE_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && (TPP_HAVE_TRADITIONAL_MACROS <= 0)) ? -1 : 0) /* "-fstringize-macro-argument" */
#endif /* !TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */

/* Support for: #define chr(x) #@x */
#ifndef TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#define TPP_HAVE_CHARIZE_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && (TPP_HAVE_TRADITIONAL_MACROS <= 0)) ? -1 : 0) /* "-fcharize-macro-argument" */
#endif /* !TPP_HAVE_CHARIZE_MACRO_ARGUMENT */

/* Support for: #define noexpand(x) #!x */
#ifndef TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
#define TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && (TPP_HAVE_TRADITIONAL_MACROS <= 0)) ? -1 : 0) /* "-fdont-expand-macro-argument" */
#endif /* !TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT */

/* Support for: #define cat(a, b) a##b */
#ifndef TPP_HAVE_GLUE_MACRO_ARGUMENT
#define TPP_HAVE_GLUE_MACRO_ARGUMENT ((TPP_HAVE_CPP_MACROS && (TPP_HAVE_TRADITIONAL_MACROS <= 0)) ? -1 : 0) /* "-fglue-macro-argument" */
#endif /* !TPP_HAVE_GLUE_MACRO_ARGUMENT */

/************************************************************************/
/************************************************************************/
/************************************************************************/




/************************************************************************/
/* #PRAGMA DIRECTIVES                                                   */
/************************************************************************/

/* Support for: #pragma push_macro() / #pragma pop_macro() */
#ifndef TPP_HAVE_PRAGMA_PUSH_MACRO
#define TPP_HAVE_PRAGMA_PUSH_MACRO ((TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA) ? -1 : 0) /* "-fpragma-push-macro" */
#endif /* !TPP_HAVE_PRAGMA_PUSH_MACRO */

/* Support for: #pragma once */
#ifndef TPP_HAVE_PRAGMA_ONCE
#define TPP_HAVE_PRAGMA_ONCE (((TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT) && TPP_HAVE_PRAGMA) ? -1 : 0) /* "-fpragma-once" */
#endif /* !TPP_HAVE_PRAGMA_ONCE */

/* Support for: #pragma deprecated("foo") */
#ifndef TPP_HAVE_PRAGMA_DEPRECATED
#define TPP_HAVE_PRAGMA_DEPRECATED (TPP_HAVE_PRAGMA ? -1 : 0) /* "-fpragma-deprecated" */
#endif /* !TPP_HAVE_PRAGMA_DEPRECATED */

/* Support for: #pragma tpp_set_keyword_flags("foo", 0x7f) */
#ifndef TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS (TPP_HAVE_PRAGMA ? -1 : 0) /* "-fpragma-tpp-set-keyword-flags" */
#endif /* !TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

/* Support for: #pragma extension(...) */
#ifndef TPP_HAVE_PRAGMA_EXTENSION
#define TPP_HAVE_PRAGMA_EXTENSION ((TPP_HAVE_PRAGMA && TPP_HAVE_EXTENSIONS) ? -1 : 0) /* "-fpragma-extension" */
#endif /* !TPP_HAVE_PRAGMA_EXTENSION */

/* Support for: #pragma extension(push) */
#ifndef TPP_HAVE_PRAGMA_EXTENSION_PUSH
#define TPP_HAVE_PRAGMA_EXTENSION_PUSH ((TPP_HAVE_PRAGMA_EXTENSION && TPP_HAVE_EXTENSIONS_PUSH_POP) ? -1 : 0) /* "-fpragma-extension-push" */
#endif /* !TPP_HAVE_PRAGMA_EXTENSION_PUSH */

/* Support for: #pragma warning(...) */
#ifndef TPP_HAVE_PRAGMA_WARNING
#define TPP_HAVE_PRAGMA_WARNING ((TPP_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? -1 : 0) /* "-fpragma-warning" */
#endif /* !TPP_HAVE_PRAGMA_WARNING */

/* Support for: #pragma warning(push) */
#ifndef TPP_HAVE_PRAGMA_WARNING_PUSH
#define TPP_HAVE_PRAGMA_WARNING_PUSH ((TPP_HAVE_PRAGMA_WARNING && TPP_HAVE_WARNINGS_PUSH_POP) ? -1 : 0) /* "-fpragma-warning-push" */
#endif /* !TPP_HAVE_PRAGMA_WARNING_PUSH */

/* Support for: #pragma tpp_exec("...") */
#ifndef TPP_HAVE_PRAGMA_TPP_EXEC
#define TPP_HAVE_PRAGMA_TPP_EXEC (TPP_HAVE_PRAGMA ? -1 : 0) /* "-fpragma-tpp_exec" */
#endif /* !TPP_HAVE_PRAGMA_TPP_EXEC */

/* Support for: #pragma message("...") */
#ifndef TPP_HAVE_PRAGMA_MESSAGE
#define TPP_HAVE_PRAGMA_MESSAGE (TPP_HAVE_PRAGMA ? -1 : 0) /* "-fpragma-message" */
#endif /* !TPP_HAVE_PRAGMA_MESSAGE */

/* Support for: #pragma error("...") */
#ifndef TPP_HAVE_PRAGMA_ERROR
#define TPP_HAVE_PRAGMA_ERROR (TPP_HAVE_PRAGMA ? -1 : 0) /* "-fpragma-error" */
#endif /* !TPP_HAVE_PRAGMA_ERROR */

/* Support for: #pragma region,  #pragma endregion */
#ifndef TPP_HAVE_PRAGMA_REGION
#define TPP_HAVE_PRAGMA_REGION (TPP_HAVE_PRAGMA ? 1 : 0) /* "-fpragma-region" */
#endif /* !TPP_HAVE_PRAGMA_REGION */

/* Support for: #pragma GCC poison foo */
#ifndef TPP_HAVE_PRAGMA_GCC_POISON
#define TPP_HAVE_PRAGMA_GCC_POISON (TPP_HAVE_PRAGMA ? -1 : 0) /* "-fpragma-gcc-poison" */
#endif /* !TPP_HAVE_PRAGMA_GCC_POISON */

/* Support for: #pragma GCC system_header */
#ifndef TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#define TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER (TPP_HAVE_PRAGMA ? -1 : 0) /* "-fpragma-gcc-system-header" */
#endif /* !TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

/* Support for: #pragma TPP warning(...)  (same as TPP_HAVE_PRAGMA_WARNING) */
#ifndef TPP_HAVE_PRAGMA_TPP_WARNING
#define TPP_HAVE_PRAGMA_TPP_WARNING TPP_HAVE_PRAGMA_WARNING
#endif /* !TPP_HAVE_PRAGMA_TPP_WARNING */

/* Support for: #pragma TPP extension(...)  (same as TPP_HAVE_PRAGMA_EXTENSION) */
#ifndef TPP_HAVE_PRAGMA_TPP_EXTENSION
#define TPP_HAVE_PRAGMA_TPP_EXTENSION TPP_HAVE_PRAGMA_EXTENSION
#endif /* !TPP_HAVE_PRAGMA_TPP_EXTENSION */

/* Support for: #pragma TPP tpp_exec(...)  (same as TPP_HAVE_PRAGMA_TPP_EXEC) */
#ifndef TPP_HAVE_PRAGMA_TPP_TPP_EXEC
#define TPP_HAVE_PRAGMA_TPP_TPP_EXEC TPP_HAVE_PRAGMA_TPP_EXEC
#endif /* !TPP_HAVE_PRAGMA_TPP_TPP_EXEC */

/* Support for: #pragma TPP tpp_set_keyword_flags("foo", 0x7f)  (same as TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#ifndef TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
#define TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#endif /* !TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */

/* Support for: #pragma TPP include_path(...) */
#ifndef TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
#define TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH (TPP_HAVE_PRAGMA ? -1 : 0) /* "-fpragma-tpp-include-path" */
#endif /* !TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */
/************************************************************************/
/************************************************************************/
/************************************************************************/




/************************************************************************/
/* IMPLICIT API FEATURES                                                */
/************************************************************************/
/* Provide an API function `tpp_unicode_writeutf8()' */
#ifndef TPP_HAVE_TPP_UNICODE_WRITEUTF8
#define TPP_HAVE_TPP_UNICODE_WRITEUTF8            \
	(TPP_HAVE_TPP_TOK_CHAR ||                     \
	 TPP_HAVE_TPP_TOK_STRING ||                   \
	 TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL ||  \
	 TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL || \
	 TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL || \
	 TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL ||  \
	 TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL ||     \
	 TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL ||       \
	 TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
#endif /* !TPP_HAVE_TPP_UNICODE_WRITEUTF8 */

/* Enable support for `TPP_FILE_IOFLAGS_SYSHDR' */
#ifndef TPP_HAVE_FILE_SYSHDR
#define TPP_HAVE_FILE_SYSHDR (TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER != 0)
#endif /* !TPP_HAVE_FILE_SYSHDR */

/* Enable support for `tpp_file::tf_prev' */
#ifndef TPP_HAVE_INCLUDE_STACK
#define TPP_HAVE_INCLUDE_STACK (TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED)
#endif /* !TPP_HAVE_INCLUDE_STACK */

/* Provide a secondary set of keyword APIs that include support for \-escape sequences */
#ifndef TPP_HAVE_ESCAPED_KEYWORDS
#define TPP_HAVE_ESCAPED_KEYWORDS (TPP_HAVE_BSE || TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
#endif /* !TPP_HAVE_ESCAPED_KEYWORDS */

/* Enable support for `tpp_keywords_openfile()' */
#ifndef TPP_HAVE_KEYWORDS_OPENFILE
#define TPP_HAVE_KEYWORDS_OPENFILE (TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED || 1)
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

/* Enable support for `tpp_lexer_skip()' */
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

/* Provide a lexer state flag "TPP_LEXER_STATE_FLAG_ALLTOKENS"
 * that forces "tpp_lexer_yieldpp()" to always re-emit *all*
 * tokens (rather than skip over space/lf/comment tokens based
 * on `TPP_HAVE_TPP_TOK_*' and `TPP_FEAT_TPP_TOK_*')
 *
 * This flag is also needed internally when TPP needs to expand
 * the arguments supplied to a user-defined macro */
#ifndef TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
#define TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS                   \
	(TPP_HAVE_CPP_MACROS && ((TPP_HAVE_TPP_TOK_SPACE <= 0) || \
	                         (TPP_HAVE_TPP_TOK_LF <= 0) ||    \
	                         (TPP_HAVE_TPP_TOK_COMMENT <= 0)))
#endif /* !TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */

/* Provide a function "tpp_lexer_getkeyworddefined()" to check
 * if a given keyword is "defined()" (meaning it can be expanded
 * as a (potentially builtin) macro) */
#ifndef TPP_HAVE_LEXER_GETKEYWORDDEFINED
#define TPP_HAVE_LEXER_GETKEYWORDDEFINED \
	(TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_LEXER_GETKEYWORDDEFINED */

/* Provide a function "tpp_lexer_seek_rparen()" that can be used
 * to find the position of a matching ')'-token for the purpose
 * of macro argument lists. */
#ifndef TPP_HAVE_LEXER_SEEK_RPAREN
#define TPP_HAVE_LEXER_SEEK_RPAREN (TPP_HAVE_CPP_MACROS)
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN */

/* Same as "tpp_lexer_seek_rparen()", but also able to deal with
 * alternate parenthesis pairs: [ ] { } < > */
#ifndef TPP_HAVE_LEXER_SEEK_RPAREN_EX
#define TPP_HAVE_LEXER_SEEK_RPAREN_EX (TPP_HAVE_LEXER_SEEK_RPAREN && TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS)
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN_EX */

/* Provide a function "tpp_macro_equals()" to compare macro definitions */
#ifndef TPP_HAVE_MACRO_EQUALS
#define TPP_HAVE_MACRO_EQUALS (TPP_HAVE_TPP_W_REDEFINE_MACRO)
#endif /* !TPP_HAVE_MACRO_EQUALS */

/* Provide a function "tpp_token_encodestring()" to perform \-escaping of arbitrary data */
#ifndef TPP_HAVE_TOKEN_ENCODESTRING
#define TPP_HAVE_TOKEN_ENCODESTRING (TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT)
#endif /* !TPP_HAVE_TOKEN_ENCODESTRING */

/* Provide a function "tpp_lexer_parseexpr()" that
 * is used to implement "#if" directive expressions */
#ifndef TPP_HAVE_LEXER_PARSEEXPR
#define TPP_HAVE_LEXER_PARSEEXPR (TPP_HAVE_CPP_IF_ELSE_ENDIF  || TPP_HAVE_MACRO___TPP_EVAL)
#endif /* !TPP_HAVE_LEXER_PARSEEXPR */

/************************************************************************/
/************************************************************************/
/************************************************************************/





/************************************************************************/
/* LEXER EXPRESSIONS                                                    */
/************************************************************************/

/* Enable support for "defined(MACRO)" in lexer expressions */
#ifndef TPP_HAVE_EXPR_DEFINED
#define TPP_HAVE_EXPR_DEFINED (TPP_HAVE_LEXER_PARSEEXPR)
#endif /* !TPP_HAVE_EXPR_DEFINED */

/* Enable special handling in "#define foo(x) defined(x)" such that "x" is not expanded */
#ifndef TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR
#define TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR (TPP_HAVE_EXPR_DEFINED ? -2 : 0) /* "-fdont-expand-defined" */
#endif /* !TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR */

#if 0 /* TODO: Extensions related to lexer expression evaluation */
EXTENSION(EXT_BININTEGRAL,       "binary-literals",               TPP_CONFIG_EXTENSION_BININTEGRAL_DEFAULT)
EXTENSION(EXT_MSVC_FIXED_INT,    "fixed-length-integrals",        TPP_CONFIG_EXTENSION_MSVC_FIXED_INT_DEFAULT)
EXTENSION(EXT_GCC_IFELSE,        "if-else-optional-true",         TPP_CONFIG_EXTENSION_GCC_IFELSE_DEFAULT)
EXTENSION(EXT_IFELSE_IN_EXPR,    "ifelse-in-expressions",         TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR_DEFAULT)
EXTENSION(EXT_STRINGOPS,         "strings-in-expressions",        TPP_CONFIG_EXTENSION_STRINGOPS_DEFAULT)
EXTENSION(EXT_LXOR,              "logical-xor-in-expressions",    TPP_CONFIG_EXTENSION_LXOR_DEFAULT)
EXTENSION(EXT_MULTICHAR_CONST,   "multichar-constants",           TPP_CONFIG_EXTENSION_MULTICHAR_CONST_DEFAULT) /* TODO: Relation to -Wno-multichar? */
#endif
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
#ifndef TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE
#define TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE (TPP_HAVE_WARNINGS && (TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0) && TPP_HAVE_CPP_DIRECTIVES)
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
#ifndef TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST
#define TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST (TPP_HAVE_WARNINGS && TPP_HAVE_LEXER_SEEK_RPAREN)
#endif /* !TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST */
#ifndef TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS
#define TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS (TPP_HAVE_WARNINGS && TPP_HAVE_LEXER_SEEK_RPAREN)
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
#define TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED \
	(TPP_HAVE_WARNINGS && TPP_HAVE_EXPR_DEFINED && (TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR <= 0))
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
/* Supply a built-in printer (that uses "fwrite(stderr)")
 * when no user-defined printer was configured for a lexer. */
#ifndef TPP_HAVE_BUILTIN_WARNPRINTER
#define TPP_HAVE_BUILTIN_WARNPRINTER 1
#endif /* !TPP_HAVE_BUILTIN_WARNPRINTER */
#endif /* !TPP_CONFIG_WARNING_PRINTER */
#endif /* TPP_HAVE_WARNINGS */
/************************************************************************/
/************************************************************************/
/************************************************************************/

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CONFIG_H */
