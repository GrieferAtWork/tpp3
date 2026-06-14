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
#ifndef GUARD_TPP2_H
#define GUARD_TPP2_H 1

/*
 * Compatibility wrapper header for tpp2
 *
 * This obviously won't catch everything (and won't be some kind of "magic bullet"
 * that'll enable you to use tpp3 when coming from tpp2 without needing to make any
 * changes to your code), but it should make transitioning al *little* easier.
 */

/* Enable symbol aliases in the global namespace (TPP2 did this under "#define TPP(x) x") */
#ifndef TPP2_HAVE_GLOBAL_NAMESPACE
#define TPP2_HAVE_GLOBAL_NAMESPACE 1
#endif /* !TPP2_HAVE_GLOBAL_NAMESPACE */


/************************************************************************/
/* MIGRATION NOTES (use CTRL+F to search for TPP2 identifiers)          */
/************************************************************************/

/* EXT_CLANG_FEATURES: "-fhas-feature-macros"
 *   -> TPP_EXT_CLANG_MACRO___has_attribute:          "-fclang-__has_attribute"
 *   -> TPP_EXT_CLANG_MACRO___has_builtin:            "-fclang-__has_builtin"
 *   -> TPP_EXT_CLANG_MACRO___has_cpp_attribute:      "-fclang-__has_cpp_attribute"
 *   -> TPP_EXT_CLANG_MACRO___has_declspec_attribute: "-fclang-__has_declspec_attribute"
 *   -> TPP_EXT_CLANG_MACRO___has_extension:          "-fclang-__has_extension"
 *   -> TPP_EXT_CLANG_MACRO___has_feature:            "-fclang-__has_feature"
 *   -> TPP_EXT_CLANG_MACRO___has_c_attribute:        "-fclang-__has_c_attribute"
 *   -> TPP_EXT_MACRO___is_identifier:                "-f__is_identifier"
 *   -> TPP_EXT_MACRO___is_deprecated:                "-f__is_deprecated"
 *   -> TPP_EXT_MACRO___is_poisoned:                  "-f__is_poisoned"
 */

/* EXT_HAS_INCLUDE: "has-include-macros"
 *   -> TPP_EXT_MACRO___has_include:                  "-f__has_include"
 *   -> TPP_EXT_MACRO___has_include_next:             "-f__has_include_next"
 */

/* EXT_EXTENDED_IDENTS: "extended-identifiers"
 *   - TPP3 no longer includes ANSI support. Instead, TPP3 supports proper unicode
 *     and utf-8. As such, there is no extension to enable/disable ANSI identifiers.
 *     Instead, you can pre-define macros like "tpp_unicode_issymstrt()" to supply
 *     TPP with a unicode character traits database, which it will happly use.
 *   - Decoding input files from (certain) codecs into utf-8 is done automatically
 *   - TPP unicode support is enabled with "TPP_HAVE_UNICODE"
 *   - For compatibility with TPP2's (flaky) attempts at decoding (e.g.) utf-16
 *     into utf-8, this compatibility header force-enables unicode support, even
 *     though that (somewhat) conflicts with TPP2's ANSI support.
 */

/* TPPLEXER_TOKEN_NONE, TPPLEXER_TOKEN_DEFAULT:
 *   - TPP3 allows recognized tokens to be configured on a per-token basis.
 *   - Every recognized token can also be configured to be runtime configurable
 *   - When runtime configurable, tokens can be enabled/disabled using "tpp_lexer_setfeat"
 */

/* TPPLEXER_TOKEN_EQUALBINOP:
 *   - TPP3 still includes support for tokens like "=+", however unlike
 *     in TPP2, TPP3 defines separate token IDs for all mirrored variants.
 *   - As such, you will need to re-write your code as:
 *     ```diff
 *     -case TPP_TOK_SHL_EQUAL:
 *     +case TPP_TOK_LANGLE_LANGLE_EQUAL:
 *     +case TPP_TOK_EQUAL_LANGLE_LANGLE:
 *     ```
 */

/* EXT_DOLLAR_IS_ALPHA: "-fdollars-in-identifiers"
 * TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA_DEFAULT
 * TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA
 *   - TPP3 has removed this feature as an extension switch.
 *     Instead, it is a lexer "feature" that can be configured
 *     (possibly at runtime) via "TPP_HAVE_TPP_TOK_DOLLAR"
 *   - When "TPP_HAVE_TPP_TOK_DOLLAR" is enabled, '$' is treated
 *     as its own, 1-char token.
 *   - When "TPP_HAVE_TPP_TOK_DOLLAR" is disabled, '$' is treated
 *     like any other tpp_ascii_issymcont() byte, meaning it is
 *     treated as part of a keyword / identifier
 *   - HINT: #define TPP_HAVE_TPP_TOK_DOLLAR (-1)  // Runtime-configurable; default=true
 *     HINT: #define TPP_HAVE_TPP_TOK_DOLLAR (-2)  // Runtime-configurable; default=false
 */

/* EXT_CANONICAL_HEADERS: "-fcanonical-system-headers"
 * TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT
 * TPP_CONFIG_EXTENSION_CANONICAL_HEADERS
 *   - Filename normalization is no longer something that can be configured
 *     or affected using conventional lexer features / extensions. This is
 *     because the act of normalizing a filename / opening a file is now
 *     something that needs to happen at the time a lexer is initialized
 *   - Since TPP3 requires you to specify the file to-be loaded initially
 *     when initializing your lexer, that also means that there is no point
 *     in time where filename normalization could be overwritten/configured
 *   - You can however still affect how TPP3 normalizes filenames (TPP_FS_*):
 *     - TPP_FS_HAVE_DRIVES
 *     - TPP_FS_HAVE_ICASE
 *     - TPP_FS_SEP
 *     - TPP_FS_ALTSEP
 *     - TPP_FS_ISSEP
 *     - TPP_FS_ISABS
 *     Additionally, you may take a look at `tpp_fs_normalize()'
 */

/* TOK_CHAR, TPP_TOK_CHAR, TOK_STRING, TPP_TOK_STRING:
 *   - TPP3 has individual tokens for every type of string, whereas
 *     TPP2 used to have only 2 token types describing string and
 *     char tokens
 *   - Because of this, you should migrate code as follows:
 *     ```diff
 *     - case TPP_TOK_CHAR:
 *     - case TPP_TOK_STRING:
 *     +     if (tok == TPP_TOK_CHAR || tok == TPP_TOK_STRING) {
 *     + TPP_CASE_TPP_TOK_STRING
 *     +     if (TPP_TOK_ISSTRING(tok)) {
 *     ```
 */

/* TOK_COMMENT, TPP_TOK_COMMENT:
 *   - TPP3 has individual tokens for every type of comment
 *   - It also differentiates between comments that include a trailing
 *     line-feed, and comments that end in-line
 *   - Because of this, you should migrate code as follows:
 *     ```diff
 *     - case TPP_TOK_COMMENT:
 *     -     if (tok == TPP_TOK_COMMENT) {
 *     + TPP_CASE_TPP_TOK_COMMENT
 *     +     if (TPP_TOK_ISCOMMENT(tok)) {
 *     ```
 */

/* TOK_ERR:
 *   - TPP3 defines individual token IDs for different errors that can
 *     happen during lexing, whereas TPP2 only used to define a singular
 *     token ID to represent errors.
 *   - When migrating, you should adjust your error-checking code to
 *     deal with all possible types of lexing errors:
 *     ```diff
 *     - case TOK_ERR:
 *     -     if (tok == TOK_ERR) {
 *     + TPP_CASE_TPP_TOK_ERR
 *     +     if (TPP_TOK_ISERR(tok)) {
 *     ```
 *
 * Note however that you are advised to deal with the meaning of actual errors:
 * - TPP_TOK_ENOMEM, TPP_TOK_EIO, TPP_TOK_EWARNPRINT:
 *   These errors are not caused by TPP itself and indicate a problem
 *   with the underlying operating system. In all likelyhook, you can
 *   probably just propagate these errors
 * - TPP_TOK_ELEXERROR:
 *   This error is what comes closest to TPP2's "TPPLexer_SetErr()"
 *   macro. This error is returned when an error/fatal-level message
 *   was emitted in a way where the configuration of lexer warnings
 *   indicates that compilation must be aborted.
 * - TPP_TOK_EWOULDBLOCK:
 *   You will only see this error if you made use of "TPP_CONFIG_NONBLOCKING_IO"
 *   (aka. "TPP_HAVE_FILE_NONBLOCK" in TPP3). This is a temporary error
 *   that means that the next token cannot be read *right now* because
 *   reading from the underlying I/O file would block.
 *   -> You will not see this error when building with "-DTPP_HAVE_FILE_NONBLOCK=0"
 *   -> You will not see this error when not using the "TPP_FILE_IOFLAGS_NONBLOCK" flag
 */

/* TPP_CONFIG_EXTENSION_MULTICHAR_CONST_DEFAULT,
 * TPP_CONFIG_EXTENSION_MULTICHAR_CONST, EXT_MULTICHAR_CONST, "-fmultichar-constants":
 *  - TPP3 allows multi-char literals by default. Instead of having an extension to
 *    enable/disable support for this, there is now a warning "-Wmultichar" that is
 *    emitted by default when multi-char constants are used.
 *  - see: TPP_HAVE_TPP_W_MULTICHAR_LITERAL
 */
/************************************************************************/





/************************************************************************/
/* ALTER CONFIGURATION                                                  */
/************************************************************************/

#ifdef TPP_CONFIG_DEBUG
#define TPP_DEBUG TPP_CONFIG_DEBUG
#endif /* TPP_CONFIG_DEBUG */

/* TPP2 configuration macros */

#ifndef TPP_CONFIG_FEATURE_TRIGRAPHS_DEFAULT
#define TPP_CONFIG_FEATURE_TRIGRAPHS_DEFAULT 0
#endif /* !TPP_CONFIG_FEATURE_TRIGRAPHS_DEFAULT */
#ifndef TPP_CONFIG_FEATURE_DIGRAPHS_DEFAULT
#define TPP_CONFIG_FEATURE_DIGRAPHS_DEFAULT 1
#endif /* !TPP_CONFIG_FEATURE_DIGRAPHS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_GCC_VA_ARGS_DEFAULT
#define TPP_CONFIG_EXTENSION_GCC_VA_ARGS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_GCC_VA_ARGS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_GCC_VA_COMMA_DEFAULT
#define TPP_CONFIG_EXTENSION_GCC_VA_COMMA_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_GCC_VA_COMMA_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_GCC_IFELSE_DEFAULT
#define TPP_CONFIG_EXTENSION_GCC_IFELSE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_GCC_IFELSE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_VA_COMMA_DEFAULT
#define TPP_CONFIG_EXTENSION_VA_COMMA_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_VA_COMMA_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_VA_OPT_DEFAULT
#define TPP_CONFIG_EXTENSION_VA_OPT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_VA_OPT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_VA_NARGS_DEFAULT
#define TPP_CONFIG_EXTENSION_VA_NARGS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_VA_NARGS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_VA_ARGS_DEFAULT
#define TPP_CONFIG_EXTENSION_VA_ARGS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_VA_ARGS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_STR_E_DEFAULT
#define TPP_CONFIG_EXTENSION_STR_E_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_STR_E_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_ALTMAC_DEFAULT
#define TPP_CONFIG_EXTENSION_ALTMAC_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_ALTMAC_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_RECMAC_DEFAULT
#define TPP_CONFIG_EXTENSION_RECMAC_DEFAULT 0
#endif /* !TPP_CONFIG_EXTENSION_RECMAC_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_ARGSPACE_DEFAULT
#define TPP_CONFIG_EXTENSION_ARGSPACE_DEFAULT 0
#endif /* !TPP_CONFIG_EXTENSION_ARGSPACE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_BININTEGRAL_DEFAULT
#define TPP_CONFIG_EXTENSION_BININTEGRAL_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_BININTEGRAL_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_MSVC_PRAGMA_DEFAULT
#define TPP_CONFIG_EXTENSION_MSVC_PRAGMA_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_MSVC_PRAGMA_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_STRINGOPS_DEFAULT
#define TPP_CONFIG_EXTENSION_STRINGOPS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_STRINGOPS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_HASH_AT_DEFAULT
#define TPP_CONFIG_EXTENSION_HASH_AT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_HASH_AT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_HASH_XCLAIM_DEFAULT
#define TPP_CONFIG_EXTENSION_HASH_XCLAIM_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_HASH_XCLAIM_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_WARNING_DEFAULT
#define TPP_CONFIG_EXTENSION_WARNING_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_WARNING_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_SHEBANG_DEFAULT
#define TPP_CONFIG_EXTENSION_SHEBANG_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_SHEBANG_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_INCLUDE_NEXT_DEFAULT
#define TPP_CONFIG_EXTENSION_INCLUDE_NEXT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_INCLUDE_NEXT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_IMPORT_DEFAULT
#define TPP_CONFIG_EXTENSION_IMPORT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_IMPORT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_IDENT_SCCS_DEFAULT
#define TPP_CONFIG_EXTENSION_IDENT_SCCS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_IDENT_SCCS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_BASEFILE_DEFAULT
#define TPP_CONFIG_EXTENSION_BASEFILE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_BASEFILE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_INCLUDE_LEVEL_DEFAULT
#define TPP_CONFIG_EXTENSION_INCLUDE_LEVEL_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_INCLUDE_LEVEL_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_COUNTER_DEFAULT
#define TPP_CONFIG_EXTENSION_COUNTER_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_COUNTER_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_CLANG_FEATURES_DEFAULT
#define TPP_CONFIG_EXTENSION_CLANG_FEATURES_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_CLANG_FEATURES_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_HAS_INCLUDE_DEFAULT
#define TPP_CONFIG_EXTENSION_HAS_INCLUDE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_HAS_INCLUDE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_LXOR_DEFAULT
#define TPP_CONFIG_EXTENSION_LXOR_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_LXOR_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_DATEUTILS_DEFAULT
#define TPP_CONFIG_EXTENSION_DATEUTILS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_DATEUTILS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TIMEUTILS_DEFAULT
#define TPP_CONFIG_EXTENSION_TIMEUTILS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TIMEUTILS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TIMESTAMP_DEFAULT
#define TPP_CONFIG_EXTENSION_TIMESTAMP_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TIMESTAMP_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_COLUMN_DEFAULT
#define TPP_CONFIG_EXTENSION_COLUMN_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_COLUMN_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_EVAL_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_EVAL_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_EVAL_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_UNIQUE_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_UNIQUE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_UNIQUE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_LOAD_FILE_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_LOAD_FILE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_LOAD_FILE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_COUNTER_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_COUNTER_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_COUNTER_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_RANDOM_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_RANDOM_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_RANDOM_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_PACK_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_STR_PACK_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_PACK_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_SIZE_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_STR_SIZE_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_SIZE_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TPP_IDENTIFIER_DEFAULT
#define TPP_CONFIG_EXTENSION_TPP_IDENTIFIER_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_TPP_IDENTIFIER_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_ASSERTIONS_DEFAULT
#define TPP_CONFIG_EXTENSION_ASSERTIONS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_ASSERTIONS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT
#define TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES_DEFAULT
#define TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_MSVC_FIXED_INT_DEFAULT
#define TPP_CONFIG_EXTENSION_MSVC_FIXED_INT_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_MSVC_FIXED_INT_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED_DEFAULT
#define TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR_DEFAULT
#define TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_EXTENDED_IDENTS_DEFAULT
#define TPP_CONFIG_EXTENSION_EXTENDED_IDENTS_DEFAULT 1
#endif /* !TPP_CONFIG_EXTENSION_EXTENDED_IDENTS_DEFAULT */
#ifndef TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO_DEFAULT
#define TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO_DEFAULT 0
#endif /* !TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO_DEFAULT */

#ifndef TPP_CONFIG_FEATURE_TRIGRAPHS
#define TPP_CONFIG_FEATURE_TRIGRAPHS (TPP_CONFIG_FEATURE_TRIGRAPHS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_FEATURE_TRIGRAPHS */
#ifndef TPP_CONFIG_FEATURE_DIGRAPHS
#define TPP_CONFIG_FEATURE_DIGRAPHS (TPP_CONFIG_FEATURE_DIGRAPHS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_FEATURE_DIGRAPHS */
#ifndef TPP_CONFIG_EXTENSION_GCC_VA_ARGS
#define TPP_CONFIG_EXTENSION_GCC_VA_ARGS (TPP_CONFIG_EXTENSION_GCC_VA_ARGS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_GCC_VA_ARGS */
#ifndef TPP_CONFIG_EXTENSION_GCC_VA_COMMA
#define TPP_CONFIG_EXTENSION_GCC_VA_COMMA (TPP_CONFIG_EXTENSION_GCC_VA_COMMA_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_GCC_VA_COMMA */
#ifndef TPP_CONFIG_EXTENSION_GCC_IFELSE
#define TPP_CONFIG_EXTENSION_GCC_IFELSE (TPP_CONFIG_EXTENSION_GCC_IFELSE_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_GCC_IFELSE */
#ifndef TPP_CONFIG_EXTENSION_VA_COMMA
#define TPP_CONFIG_EXTENSION_VA_COMMA (TPP_CONFIG_EXTENSION_VA_COMMA_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_VA_COMMA */
#ifndef TPP_CONFIG_EXTENSION_VA_OPT
#define TPP_CONFIG_EXTENSION_VA_OPT (TPP_CONFIG_EXTENSION_VA_OPT_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_VA_OPT */
#ifndef TPP_CONFIG_EXTENSION_VA_NARGS
#define TPP_CONFIG_EXTENSION_VA_NARGS (TPP_CONFIG_EXTENSION_VA_NARGS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_VA_NARGS */
#ifndef TPP_CONFIG_EXTENSION_VA_ARGS
#define TPP_CONFIG_EXTENSION_VA_ARGS (TPP_CONFIG_EXTENSION_VA_ARGS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_VA_ARGS */
#ifndef TPP_CONFIG_EXTENSION_STR_E
#define TPP_CONFIG_EXTENSION_STR_E (TPP_CONFIG_EXTENSION_STR_E_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_STR_E */
#ifndef TPP_CONFIG_EXTENSION_ALTMAC
#define TPP_CONFIG_EXTENSION_ALTMAC (TPP_CONFIG_EXTENSION_ALTMAC_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_ALTMAC */
#ifndef TPP_CONFIG_EXTENSION_RECMAC
#define TPP_CONFIG_EXTENSION_RECMAC (TPP_CONFIG_EXTENSION_RECMAC_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_RECMAC */
#ifndef TPP_CONFIG_EXTENSION_ARGSPACE
#define TPP_CONFIG_EXTENSION_ARGSPACE (TPP_CONFIG_EXTENSION_ARGSPACE_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_ARGSPACE */
#ifndef TPP_CONFIG_EXTENSION_BININTEGRAL
#define TPP_CONFIG_EXTENSION_BININTEGRAL (TPP_CONFIG_EXTENSION_BININTEGRAL_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_BININTEGRAL */
#ifndef TPP_CONFIG_EXTENSION_MSVC_PRAGMA
#define TPP_CONFIG_EXTENSION_MSVC_PRAGMA (TPP_CONFIG_EXTENSION_MSVC_PRAGMA_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_MSVC_PRAGMA */
#ifndef TPP_CONFIG_EXTENSION_STRINGOPS
#define TPP_CONFIG_EXTENSION_STRINGOPS (TPP_CONFIG_EXTENSION_STRINGOPS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_STRINGOPS */
#ifndef TPP_CONFIG_EXTENSION_HASH_AT
#define TPP_CONFIG_EXTENSION_HASH_AT (TPP_CONFIG_EXTENSION_HASH_AT_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_HASH_AT */
#ifndef TPP_CONFIG_EXTENSION_HASH_XCLAIM
#define TPP_CONFIG_EXTENSION_HASH_XCLAIM (TPP_CONFIG_EXTENSION_HASH_XCLAIM_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_HASH_XCLAIM */
#ifndef TPP_CONFIG_EXTENSION_WARNING
#define TPP_CONFIG_EXTENSION_WARNING (TPP_CONFIG_EXTENSION_WARNING_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_WARNING */
#ifndef TPP_CONFIG_EXTENSION_SHEBANG
#define TPP_CONFIG_EXTENSION_SHEBANG (TPP_CONFIG_EXTENSION_SHEBANG_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_SHEBANG */
#ifndef TPP_CONFIG_EXTENSION_INCLUDE_NEXT
#define TPP_CONFIG_EXTENSION_INCLUDE_NEXT (TPP_CONFIG_EXTENSION_INCLUDE_NEXT_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_INCLUDE_NEXT */
#ifndef TPP_CONFIG_EXTENSION_IMPORT
#define TPP_CONFIG_EXTENSION_IMPORT (TPP_CONFIG_EXTENSION_IMPORT_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_IMPORT */
#ifndef TPP_CONFIG_EXTENSION_IDENT_SCCS
#define TPP_CONFIG_EXTENSION_IDENT_SCCS (TPP_CONFIG_EXTENSION_IDENT_SCCS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_IDENT_SCCS */
#ifndef TPP_CONFIG_EXTENSION_BASEFILE
#define TPP_CONFIG_EXTENSION_BASEFILE (TPP_CONFIG_EXTENSION_BASEFILE_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_BASEFILE */
#ifndef TPP_CONFIG_EXTENSION_INCLUDE_LEVEL
#define TPP_CONFIG_EXTENSION_INCLUDE_LEVEL (TPP_CONFIG_EXTENSION_INCLUDE_LEVEL_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_INCLUDE_LEVEL */
#ifndef TPP_CONFIG_EXTENSION_COUNTER
#define TPP_CONFIG_EXTENSION_COUNTER (TPP_CONFIG_EXTENSION_COUNTER_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_COUNTER */
#ifndef TPP_CONFIG_EXTENSION_CLANG_FEATURES
#define TPP_CONFIG_EXTENSION_CLANG_FEATURES (TPP_CONFIG_EXTENSION_CLANG_FEATURES_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_CLANG_FEATURES */
#ifndef TPP_CONFIG_EXTENSION_HAS_INCLUDE
#define TPP_CONFIG_EXTENSION_HAS_INCLUDE (TPP_CONFIG_EXTENSION_HAS_INCLUDE_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_HAS_INCLUDE */
#ifndef TPP_CONFIG_EXTENSION_LXOR
#define TPP_CONFIG_EXTENSION_LXOR (TPP_CONFIG_EXTENSION_LXOR_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_LXOR */
#ifndef TPP_CONFIG_EXTENSION_DATEUTILS
#define TPP_CONFIG_EXTENSION_DATEUTILS (TPP_CONFIG_EXTENSION_DATEUTILS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_DATEUTILS */
#ifndef TPP_CONFIG_EXTENSION_TIMEUTILS
#define TPP_CONFIG_EXTENSION_TIMEUTILS (TPP_CONFIG_EXTENSION_TIMEUTILS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TIMEUTILS */
#ifndef TPP_CONFIG_EXTENSION_TIMESTAMP
#define TPP_CONFIG_EXTENSION_TIMESTAMP (TPP_CONFIG_EXTENSION_TIMESTAMP_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TIMESTAMP */
#ifndef TPP_CONFIG_EXTENSION_COLUMN
#define TPP_CONFIG_EXTENSION_COLUMN (TPP_CONFIG_EXTENSION_COLUMN_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_COLUMN */
#ifndef TPP_CONFIG_EXTENSION_TPP_EVAL
#define TPP_CONFIG_EXTENSION_TPP_EVAL (TPP_CONFIG_EXTENSION_TPP_EVAL_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_EVAL */
#ifndef TPP_CONFIG_EXTENSION_TPP_UNIQUE
#define TPP_CONFIG_EXTENSION_TPP_UNIQUE (TPP_CONFIG_EXTENSION_TPP_UNIQUE_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_UNIQUE */
#ifndef TPP_CONFIG_EXTENSION_TPP_LOAD_FILE
#define TPP_CONFIG_EXTENSION_TPP_LOAD_FILE (TPP_CONFIG_EXTENSION_TPP_LOAD_FILE_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_LOAD_FILE */
#ifndef TPP_CONFIG_EXTENSION_TPP_COUNTER
#define TPP_CONFIG_EXTENSION_TPP_COUNTER (TPP_CONFIG_EXTENSION_TPP_COUNTER_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_COUNTER */
#ifndef TPP_CONFIG_EXTENSION_TPP_RANDOM
#define TPP_CONFIG_EXTENSION_TPP_RANDOM (TPP_CONFIG_EXTENSION_TPP_RANDOM_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_RANDOM */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE
#define TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE (TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR
#define TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR (TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_SIZE
#define TPP_CONFIG_EXTENSION_TPP_STR_SIZE (TPP_CONFIG_EXTENSION_TPP_STR_SIZE_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_SIZE */
#ifndef TPP_CONFIG_EXTENSION_TPP_STR_PACK
#define TPP_CONFIG_EXTENSION_TPP_STR_PACK (TPP_CONFIG_EXTENSION_TPP_STR_PACK_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_STR_PACK */
#ifndef TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS
#define TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS (TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS */
#ifndef TPP_CONFIG_EXTENSION_TPP_IDENTIFIER
#define TPP_CONFIG_EXTENSION_TPP_IDENTIFIER (TPP_CONFIG_EXTENSION_TPP_IDENTIFIER_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TPP_IDENTIFIER */
#ifndef TPP_CONFIG_EXTENSION_ASSERTIONS
#define TPP_CONFIG_EXTENSION_ASSERTIONS (TPP_CONFIG_EXTENSION_ASSERTIONS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_ASSERTIONS */
#ifndef TPP_CONFIG_EXTENSION_CANONICAL_HEADERS
#define TPP_CONFIG_EXTENSION_CANONICAL_HEADERS (TPP_CONFIG_EXTENSION_CANONICAL_HEADERS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_CANONICAL_HEADERS */
#ifndef TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES
#define TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES (TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES */
#ifndef TPP_CONFIG_EXTENSION_MSVC_FIXED_INT
#define TPP_CONFIG_EXTENSION_MSVC_FIXED_INT (TPP_CONFIG_EXTENSION_MSVC_FIXED_INT_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_MSVC_FIXED_INT */
#ifndef TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED
#define TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED (TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED */
#ifndef TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR
#define TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR (TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR */
#ifndef TPP_CONFIG_EXTENSION_EXTENDED_IDENTS
#define TPP_CONFIG_EXTENSION_EXTENDED_IDENTS (TPP_CONFIG_EXTENSION_EXTENDED_IDENTS_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_EXTENDED_IDENTS */
#ifndef TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO
#define TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO (TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO_DEFAULT ? -1 : -2)
#endif /* !TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO */


/* Inherit legacy tabsize configuration */
#ifndef TPPLEXER_DEFAULT_TABSIZE
#if (defined(_WIN16) || defined(WIN16) || \
     defined(_WIN32) || defined(WIN32) || \
     defined(_WIN64) || defined(WIN64) || \
     defined(__WIN32__) || defined(__TOS_WIN__))
#define TPPLEXER_DEFAULT_TABSIZE 4 /* Default tab size (used for `__COLUMN__' and in error messages). */
#else /* Windows... */
#define TPPLEXER_DEFAULT_TABSIZE 8 /* Default tab size (used for `__COLUMN__' and in error messages). */
#endif /* Unix... */
#endif /* !TPPLEXER_DEFAULT_TABSIZE */
#undef TPP_TABSIZE
#define TPP_TABSIZE TPPLEXER_DEFAULT_TABSIZE

/* Inherit legacy error limit configuration */
#ifndef TPPLEXER_DEFAULT_LIMIT_ECNT
#define TPPLEXER_DEFAULT_LIMIT_ECNT 16
#endif /* !TPPLEXER_DEFAULT_LIMIT_ECNT */
#undef TPP_ERROR_LIMIT
#define TPP_ERROR_LIMIT TPPLEXER_DEFAULT_LIMIT_ECNT

/* Inherit legacy non-blocking I/O configuration */
#undef TPP_HAVE_FILE_NONBLOCK
#ifdef TPP_CONFIG_NONBLOCKING_IO
#define TPP_HAVE_FILE_NONBLOCK (-1)
#else /* TPP_CONFIG_NONBLOCKING_IO */
#define TPP_HAVE_FILE_NONBLOCK 0
#endif /* !TPP_CONFIG_NONBLOCKING_IO */

#define TPP_BUILDING                   1 /* Not actually true, but needed to prevent internals from being escaped (TPP3 doesn't expose internals by default anymore) */
#define TPP_HAVE_UNICODE               1 /* Always enable unicode support */
#define TPP_HAVE_STRERROR              0
#define TPP_HAVE_STRTOKENID            0
#define TPP_HAVE_EXTENSIONS            1
#define TPP_HAVE_EXTENSIONS_PUSH_POP   1
#define TPP_HAVE_WARNINGS              1
#define TPP_HAVE_WARNINGS_PUSH_POP     1
#define TPP_HAVE_WARNING_NUMBERS       1 /* Enable this, even though TPP2 had different warning numbers... */
#define TPP_HAVE_WARNING_ERROR         1
#define TPP_HAVE_WARNING_SUPPRESS      1
#define TPP_HAVE_WARNING_DEFAULT       1
#define TPP_HAVE_FILE_NOCLOSE          0 /* TTP2 didn't have this */
#define TPP_HAVE_FILE_NOKWD            0 /* TTP2 didn't have this */
#define TPP_COMMON_HAVE_TPP_TOK        0 /* We want to configure tokens individually */
#define TPP_COMMON_HAVE_CPP_DIRECTIVES 1
#define TPP_COMMON_HAVE_PRAGMA         1

#ifdef TPP_CONFIG_RAW_STRING_LITERALS
#define TPP2_HAVE_RAW_STRING_LITERALS 1
#else /* TPP_CONFIG_RAW_STRING_LITERALS */
#define TPP2_HAVE_RAW_STRING_LITERALS 0
#endif /* !TPP_CONFIG_RAW_STRING_LITERALS */

#define TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL \
	TPP2_HAVE_RAW_STRING_LITERALS /* Specifically for deemon, TPP2 had (really badly integrated) deemon-style r"raw string" support */
#define TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL \
	TPP2_HAVE_RAW_STRING_LITERALS /* *ditto* */

#define TPP_HAVE_TPP_TOK_LF                         (-2) /* Configurable, default=false (TPP2 used to configure this via "TPPLEXER_FLAG_WANTLF"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LF)") */
#define TPP_HAVE_TPP_TOK_SPACE                      (-2) /* Configurable, default=false (TPP2 used to configure this via "TPPLEXER_FLAG_WANTSPACE"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_SPACE)") */
#define TPP_HAVE_TPP_TOK_COMMENT                    (-2) /* Configurable, default=false (TPP2 used to configure this via "TPPLEXER_FLAG_WANTCOMMENTS"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COMMENT)") */
#define TPP_HAVE_TPP_TOK_CXX_COMMENT                (-1) /* Configurable, default=true  (TPP2 used to configure this via "TPPLEXER_TOKEN_CPP_COMMENT"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_CXX_COMMENT)") */
#define TPP_HAVE_TPP_TOK_C_COMMENT                  (-1) /* Configurable, default=true  (TPP2 used to configure this via "TPPLEXER_TOKEN_C_COMMENT"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_C_COMMENT)") */
#define TPP_HAVE_TPP_TOK_PASCAL_COMMENT             0    /* TPP2 only recognized C/C++-like comments */
#define TPP_HAVE_TPP_TOK_SHELL_COMMENT              0    /* TPP2 only recognized C/C++-like comments */
#define TPP_HAVE_TPP_TOK_ASM_COMMENT                0    /* TPP2 only recognized C/C++-like comments */
#define TPP_HAVE_TPP_TOK_SQL_COMMENT                0    /* TPP2 only recognized C/C++-like comments */
#define TPP_HAVE_TPP_TOK_DOLLAR                     (-2) /* "$" Configurable, default=false (TPP2 used to configure this via "TPPLEXER_TOKEN_DOLLAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_DOLLAR)") */
#define TPP_HAVE_TPP_TOK_INT                        1    /* ... */
#define TPP_HAVE_TPP_TOK_FLOAT                      1    /* ... */
#define TPP_HAVE_TPP_TOK_STRING                     1    /* TPP2 only supported C character/string literals */
#define TPP_HAVE_TPP_TOK_CHAR                       1    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL     0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL    0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL    0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL   0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL   0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL       0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL       0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL      0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL      0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL     0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL     0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL         0    /* *ditto* */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE              1    /* "<<" */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE              1    /* ">>" */
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL                1    /* "==" */
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL              1    /* "!=" */
#define TPP_HAVE_TPP_TOK_RANGLE_EQUAL               1    /* ">=" */
#define TPP_HAVE_TPP_TOK_LANGLE_EQUAL               1    /* "<=" */
#define TPP_HAVE_TPP_TOK_DOT_DOT_DOT                1    /* "..." */
#define TPP_HAVE_TPP_TOK_PLUS_EQUAL                 1    /* "+=" */
#define TPP_HAVE_TPP_TOK_MINUS_EQUAL                1    /* "-=" */
#define TPP_HAVE_TPP_TOK_STAR_EQUAL                 1    /* "*=" */
#define TPP_HAVE_TPP_TOK_SLASH_EQUAL                1    /* "/=" */
#define TPP_HAVE_TPP_TOK_PERCENT_EQUAL              1    /* "%=" */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL        1    /* "<<=" */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL        1    /* ">>=" */
#define TPP_HAVE_TPP_TOK_AMP_EQUAL                  1    /* "&=" */
#define TPP_HAVE_TPP_TOK_PIPE_EQUAL                 1    /* "|=" */
#define TPP_HAVE_TPP_TOK_HAT_EQUAL                  1    /* "^=" */
#define TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL            (-1) /* "**="  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_STARSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL)") */
#define TPP_HAVE_TPP_TOK_AT_EQUAL                   (-1) /* "@="   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ATEQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_AT_EQUAL)") */
#define TPP_HAVE_TPP_TOK_POUND_POUND                1    /* "##" */
#define TPP_HAVE_TPP_TOK_AMP_AMP                    1    /* "&&" */
#define TPP_HAVE_TPP_TOK_PIPE_PIPE                  1    /* "||" */
#define TPP_HAVE_TPP_TOK_HAT_HAT                    (-1) /* "^^"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ROOFROOF"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_HAT_HAT)") */
#define TPP_HAVE_TPP_TOK_PLUS_PLUS                  1    /* "++" */
#define TPP_HAVE_TPP_TOK_MINUS_MINUS                1    /* "--" */
#define TPP_HAVE_TPP_TOK_STAR_STAR                  (-1) /* "**"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_STARSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_STAR_STAR)") */
#define TPP_HAVE_TPP_TOK_TILDE_TILDE                (-1) /* "~~"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_TILDETILDE"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_TILDE_TILDE)") */
#define TPP_HAVE_TPP_TOK_TILDE_EQUAL                1    /* "~=" */
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE               (-1) /* "->"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ARROW"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_MINUS_RANGLE)") */
#define TPP_HAVE_TPP_TOK_COLON_EQUAL                (-1) /* ":="   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_COLONASSIGN" / "TPPLEXER_TOKEN_COLLONASSIGN"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COLON_EQUAL)") */
#define TPP_HAVE_TPP_TOK_COLON_COLON                (-1) /* "::"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_COLONCOLON" / "TPPLEXER_TOKEN_COLLONCOLLON"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_COLON_COLON)") */
#define TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR          (-1) /* "->*"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ARROWSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR)") */
#define TPP_HAVE_TPP_TOK_DOT_STAR                   (-1) /* ".*"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_DOTSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_DOT_STAR)") */
#define TPP_HAVE_TPP_TOK_DOT_DOT                    (-1) /* ".."   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_DOTDOT"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_DOT_DOT)") */
#define TPP_HAVE_TPP_TOK_LANGLE_RANGLE              (-1) /* "<>"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_LOGT"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LANGLE_RANGLE)") */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE       (-1) /* "<<<"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ANGLE3"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE)") */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE       (-1) /* ">>>"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ANGLE3"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE)") */
#define TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL (-1) /* "<<<=" Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ANGLE3_EQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)") */
#define TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL (-1) /* ">>>=" Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_ANGLE3_EQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)") */
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL          (-1) /* "==="  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUAL3"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL)") */
#define TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL        (-1) /* "!=="  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUAL3"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL)") */
#define TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM            0    /* "!!"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_QMARK_QMARK                (-1) /* "??"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_QMARK_QMARK"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_SLASH_SLASH                0    /* "//"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL          0    /* "//="  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_QMARK_EQUAL                0    /* "?="   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_RANGLE_LANGLE              0    /* "><"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_PLUS                 (-1) /* "=+"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_MINUS                (-1) /* "=-"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_STAR                 (-1) /* "=*"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR            (-1) /* "=**"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP" + "TPPLEXER_TOKEN_STARSTAR"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH                (-1) /* "=/"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH          0    /* "=//"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_PERCENT              (-1) /* "=%"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_AMP                  (-1) /* "=&"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_PIPE                 (-1) /* "=|"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_HAT                  (-1) /* "=^"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE               0    /* "=<"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE        (-1) /* "=<<"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE (-1) /* "=<<<" Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP" + "TPPLEXER_TOKEN_ANGLE3_EQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE               0    /* "=>"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE        (-1) /* "=>>"  Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE (-1) /* "=>>>" Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP" + "TPPLEXER_TOKEN_ANGLE3_EQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_AT                   (-1) /* "=@"   Configurable, default=true (TPP2 used to configure this via "TPPLEXER_TOKEN_EQUALBINOP" + "TPPLEXER_TOKEN_ATEQUAL"; use "tpp_lexer_setfeat(TPP_FEAT_TPP_TOK_QMARK_QMARK)") */
#define TPP_HAVE_TPP_TOK_EQUAL_TILDE                0    /* "=~"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_COLON                0    /* "=:"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM              0    /* "=!"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM        0    /* "==!"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_EQUAL_QMARK                0    /* "=?"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_LANGLE_MINUS               0    /* "<-"   Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS          0    /* "*<-"  Completely unknown to TPP2 */
#define TPP_HAVE_TPP_TOK_STAR_DOT                   0    /* "*."   Completely unknown to TPP2 */

/* Features... */
#define TPP_HAVE_BSE                                  1 /* TPP2 didn't even let you disable this */
#define TPP_HAVE_BSE_WHITESPACE                       0 /* TPP2 didn't support whitespace between \ and LF */
#define TPP_HAVE_ESCAPE_IN_IDENTIFIERS                0 /* TPP2 didn't support \uXXXX or \UXXXXXXXX characters in identifiers */
#define TPP_HAVE_TRIGRAPHS                            TPP_CONFIG_FEATURE_TRIGRAPHS
#define TPP_HAVE_DIGRAPHS                             TPP_CONFIG_FEATURE_DIGRAPHS
#define TPP_HAVE_ESCAPE_E_IN_STRINGS                  TPP_CONFIG_EXTENSION_STR_E             /* Support for "\e" (for U+001B) escape sequences */
#define TPP_HAVE_ESCAPE_S_IN_STRINGS                  0                                      /* Support for "\s" (for U+0020) escape sequences */
#define TPP_HAVE_CPP_DIRECTIVES                       1                                      /* Specifies if *any* CPP directives are supported */
#define TPP_HAVE_CPP_MACROS                           1                                      /* Support for C-style macros */
#define TPP_HAVE_CPP_EXCLAIM                          TPP_CONFIG_EXTENSION_SHEBANG           /* Support for: #!foobar  (comments) */
#define TPP_HAVE_CPP_BLANK                            1                                      /* Support for: #  (blank line) */
#define TPP_HAVE_CPP_DIGIT_LINE                       1                                      /* Support for: # 42 ...  (similar to #line) */
#define TPP_HAVE_CPP_LINE                             1                                      /* Support for: #line ... */
#define TPP_HAVE_CPP_INCLUDE                          1                                      /* Support for: #include */
#define TPP_HAVE_CPP_INCLUDE_NEXT                     TPP_CONFIG_EXTENSION_INCLUDE_NEXT      /* Support for: #include_next */
#define TPP_HAVE_CPP_IMPORT                           TPP_CONFIG_EXTENSION_IMPORT            /* Support for: #import */
#define TPP_HAVE_CPP_IF_ELSE_ENDIF                    1                                      /* Support for: #if, #ifdef, #ifndef, #elif, #elifdef, #elifndef, #else, #endif */
#define TPP_HAVE_CPP_DEFINE                           1                                      /* Support for: #define, #undef */
#define TPP_HAVE_CPP_ASSERT                           TPP_CONFIG_EXTENSION_ASSERTIONS        /* Support for: #assert, #unassert */
#define TPP_HAVE_CPP_ERROR                            1                                      /* Support for: #error */
#define TPP_HAVE_CPP_WARNING                          TPP_CONFIG_EXTENSION_WARNING           /* Support for: #warning */
#define TPP_HAVE_CPP_IDENT_SCCS                       TPP_CONFIG_EXTENSION_IDENT_SCCS        /* Support for: #ident, #sccs */
#define TPP_HAVE_CPP_PRAGMA                           1                                      /* Support for: #pragma */
#define TPP_HAVE_CPP_EMBED                            0                                      /* Support for: #embed */
#define TPP_HAVE_MACRO__Pragma                        1                                      /* Support for: _Pragma("foo") */
#define TPP_HAVE_MACRO___pragma                       TPP_CONFIG_EXTENSION_MSVC_PRAGMA       /* Support for: __pragma(foo) */
#define TPP_HAVE_CLANG_MACRO___has_attribute          TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_attribute */
#define TPP_HAVE_CLANG_MACRO___has_builtin            TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_builtin */
#define TPP_HAVE_CLANG_MACRO___has_cpp_attribute      TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_cpp_attribute */
#define TPP_HAVE_CLANG_MACRO___has_declspec_attribute TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_declspec_attribute */
#define TPP_HAVE_CLANG_MACRO___has_extension          TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_extension */
#define TPP_HAVE_CLANG_MACRO___has_feature            TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_feature */
#define TPP_HAVE_CLANG_MACRO___has_c_attribute        TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __has_c_attribute */
#define TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES        TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES  /* When enabled, clang's __has_feature() also expands to "1" when __has_extension() would. */
#define TPP_HAVE_MACRO___is_identifier                TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for clang __is_identifier */
#define TPP_HAVE_MACRO___is_deprecated                TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __is_deprecated */
#define TPP_HAVE_MACRO___is_poisoned                  TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __is_poisoned */
#define TPP_HAVE_MACRO___has_extension                TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __has_extension */
#define TPP_HAVE_MACRO___has_known_extension          TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __has_known_extension */
#define TPP_HAVE_MACRO___has_warning                  TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __has_warning */
#define TPP_HAVE_MACRO___has_known_warning            TPP_CONFIG_EXTENSION_CLANG_FEATURES    /* Support for TPP's __has_known_warning */
#define TPP_HAVE_MACRO___has_include                  TPP_CONFIG_EXTENSION_HAS_INCLUDE       /* Support for clang __has_include */
#define TPP_HAVE_MACRO___has_include_next             TPP_CONFIG_EXTENSION_HAS_INCLUDE       /* Support for clang __has_include_next */
#define TPP_HAVE_MACRO___has_embed                    0                                      /* Support for clang __has_embed */
#define TPP_HAVE_MACRO___FILE__                       1                                      /* __FILE__ */
#define TPP_HAVE_MACRO___LINE__                       1                                      /* __LINE__ */
#define TPP_HAVE_MACRO___TIME__                       1                                      /* __TIME__ */
#define TPP_HAVE_MACRO___DATE__                       1                                      /* __DATE__ */
#define TPP_HAVE_MACRO___COLUMN__                     TPP_CONFIG_EXTENSION_COLUMN            /* __COLUMN__ */
#define TPP_HAVE_MACRO___BASE_FILE__                  TPP_CONFIG_EXTENSION_BASEFILE          /* __BASE_FILE__ */
#define TPP_HAVE_MACRO___INCLUDE_LEVEL__              TPP_CONFIG_EXTENSION_INCLUDE_LEVEL     /* __INCLUDE_LEVEL__ */
#define TPP_HAVE_MACRO___INCLUDE_DEPTH__              TPP_CONFIG_EXTENSION_INCLUDE_LEVEL     /* __INCLUDE_DEPTH__ */
#define TPP_HAVE_MACRO___COUNTER__                    TPP_CONFIG_EXTENSION_COUNTER           /* __COUNTER__ */
#define TPP_HAVE_MACRO___TIMESTAMP__                  TPP_CONFIG_EXTENSION_TIMESTAMP         /* __TIMESTAMP__ */
#define TPP_HAVE_NUMERIC_DATE_MACROS                  TPP_CONFIG_EXTENSION_DATEUTILS         /* __DATE_DAY__, __DATE_WDAY__, __DATE_YDAY__, __DATE_MONTH__, __DATE_YEAR__ */
#define TPP_HAVE_NUMERIC_TIME_MACROS                  TPP_CONFIG_EXTENSION_TIMEUTILS         /* __TIME_SEC__, __TIME_MIN__, __TIME_HOUR__ */
#define TPP_HAVE_MACRO___TPP_EVAL                     TPP_CONFIG_EXTENSION_TPP_EVAL          /* __TPP_EVAL */
#define TPP_HAVE_MACRO___TPP_UNIQUE                   TPP_CONFIG_EXTENSION_TPP_UNIQUE        /* __TPP_UNIQUE */
#define TPP_HAVE_MACRO___TPP_LOAD_FILE                TPP_CONFIG_EXTENSION_TPP_LOAD_FILE     /* __TPP_LOAD_FILE */
#define TPP_HAVE_MACRO___TPP_COUNTER                  TPP_CONFIG_EXTENSION_TPP_COUNTER       /* __TPP_COUNTER */
#define TPP_HAVE_MACRO___TPP_RANDOM                   TPP_CONFIG_EXTENSION_TPP_RANDOM        /* __TPP_RANDOM */
#define TPP_HAVE_MACRO___TPP_STR_DECOMPILE            TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE /* __TPP_STR_DECOMPILE */
#define TPP_HAVE_MACRO___TPP_STR_SUBSTR               TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR    /* __TPP_STR_SUBSTR */
#define TPP_HAVE_MACRO___TPP_STR_PACK                 TPP_CONFIG_EXTENSION_TPP_STR_PACK      /* __TPP_STR_PACK */
#define TPP_HAVE_MACRO___TPP_STR_SIZE                 TPP_CONFIG_EXTENSION_TPP_STR_SIZE      /* __TPP_STR_SIZE */
#define TPP_HAVE_MACRO___TPP_COUNT_TOKENS             TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS  /* __TPP_COUNT_TOKENS */
#define TPP_HAVE_MACRO___TPP_IDENTIFIER               TPP_CONFIG_EXTENSION_TPP_IDENTIFIER    /* __TPP_IDENTIFIER */
#define TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS        TPP_CONFIG_EXTENSION_ALTMAC            /* Support for: #define point<T> ... */
#define TPP_HAVE_MACRO_ARGUMENT_WHITESPACE            TPP_CONFIG_EXTENSION_ARGSPACE          /* Support for retaining whitespace around macro arguments */
#define TPP_HAVE_MACRO_RECURSION                      TPP_CONFIG_EXTENSION_RECMAC            /* Support for: #pragma extension("-fmacro-recursion") */
#define TPP_HAVE_TRADITIONAL_MACROS                   TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO /* Support for traditional macro expansion */
#define TPP_HAVE_NAMED_VARARGS_IN_MACROS              TPP_CONFIG_EXTENSION_GCC_VA_ARGS       /* Support for: #define printf(format, args...) args */
#define TPP_HAVE_VA_ARGS_IN_MACROS                    TPP_CONFIG_EXTENSION_VA_ARGS           /* Support for: #define printf(format, ...) __VA_ARGS__ */
#define TPP_HAVE_VA_COMMA_IN_MACROS                   TPP_CONFIG_EXTENSION_VA_COMMA          /* Support for: #define printf(format, ...) fprintf(stderr, format __VA_COMMA__ __VA_ARGS__) */
#define TPP_HAVE_VA_OPT_IN_MACROS                     TPP_CONFIG_EXTENSION_VA_OPT            /* Support for: #define printf(format, ...) fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__) */
#define TPP_HAVE_VA_NARGS_IN_MACROS                   TPP_CONFIG_EXTENSION_VA_NARGS          /* Support for: #define min(...) min_##__VA_NARGS__(__VA_ARGS__) */
#define TPP_HAVE_VA_GLUE_COMMA_IN_MACROS              TPP_CONFIG_EXTENSION_GCC_VA_COMMA      /* Support for: #define printf(format, ...) fprintf(stderr, format,##__VA_ARGS__) */
#define TPP_HAVE_STRINGIZE_MACRO_ARGUMENT             1                                      /* Support for: #define str(x) #x */
#define TPP_HAVE_CHARIZE_MACRO_ARGUMENT               TPP_CONFIG_EXTENSION_HASH_AT           /* Support for: #define chr(x) #@x */
#define TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT           TPP_CONFIG_EXTENSION_HASH_XCLAIM       /* Support for: #define noexpand(x) #!x */
#define TPP_HAVE_GLUE_MACRO_ARGUMENT                  1                                      /* Support for: #define cat(a, b) a##b */

/* #pragma directives */
#define TPP_HAVE_PRAGMA_PUSH_MACRO                1 /* Support for: #pragma push_macro() / #pragma pop_macro() */
#define TPP_HAVE_PRAGMA_ONCE                      1 /* Support for: #pragma once */
#define TPP_HAVE_PRAGMA_DEPRECATED                1 /* Support for: #pragma deprecated("foo") */
#define TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS     1 /* Support for: #pragma tpp_set_keyword_flags("foo", 0x7f) */
#define TPP_HAVE_PRAGMA_EXTENSION                 1 /* Support for: #pragma extension(...) */
#define TPP_HAVE_PRAGMA_EXTENSION_PUSH            1 /* Support for: #pragma extension(push) */
#define TPP_HAVE_PRAGMA_WARNING                   1 /* Support for: #pragma warning(...) */
#define TPP_HAVE_PRAGMA_WARNING_PUSH              1 /* Support for: #pragma warning(push) */
#define TPP_HAVE_PRAGMA_TPP_EXEC                  1 /* Support for: #pragma tpp_exec("...") */
#define TPP_HAVE_PRAGMA_MESSAGE                   1 /* Support for: #pragma message("...") */
#define TPP_HAVE_PRAGMA_ERROR                     1 /* Support for: #pragma error("...") */
#define TPP_HAVE_PRAGMA_REGION                    1 /* Support for: #pragma region,  #pragma endregion */
#define TPP_HAVE_PRAGMA_GCC_POISON                1 /* Support for: #pragma GCC poison foo */
#define TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER         1 /* Support for: #pragma GCC system_header */
#define TPP_HAVE_PRAGMA_TPP_WARNING               1 /* Support for: #pragma TPP warning(...)  (same as TPP_HAVE_PRAGMA_WARNING) */
#define TPP_HAVE_PRAGMA_TPP_EXTENSION             1 /* Support for: #pragma TPP extension(...)  (same as TPP_HAVE_PRAGMA_EXTENSION) */
#define TPP_HAVE_PRAGMA_TPP_TPP_EXEC              1 /* Support for: #pragma TPP tpp_exec(...)  (same as TPP_HAVE_PRAGMA_TPP_EXEC) */
#define TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS 1 /* Support for: #pragma TPP tpp_set_keyword_flags("foo", 0x7f)  (same as TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#define TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH          1 /* Support for: #pragma TPP include_path(...) */

/* Lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_DEFINED                1                                      /* Enable support for "defined(MACRO)" in builtin lexer expressions */
#define TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR         TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED /* Enable special handling in "#define foo(x) defined(x)" such that "x" is not expanded */
#define TPP_HAVE_BUILTIN_EXPR_STRINGS                TPP_CONFIG_EXTENSION_STRINGOPS         /* Enable support for string operations in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_FLOATS                 1                                      /* Enable support for floats in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT    TPP_CONFIG_EXTENSION_GCC_IFELSE        /* Enable support for "foo ?: bar" in builtin lexer expressions (same as "foo ? foo : bar") */
#define TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR    /* Enable support for "if (foo) bar else baz" in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR            TPP_CONFIG_EXTENSION_LXOR              /* Enable support for "^^" in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS        TPP_CONFIG_EXTENSION_BININTEGRAL       /* Enable support for "0b" literals in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS   1                                      /* Enable support for "u", "l", "ul", "ll", "ull" integer suffixes in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS TPP_CONFIG_EXTENSION_MSVC_FIXED_INT    /* Enable support for "i8", "i16", "i32", "i64", "ui8", "ui16", "ui32", "ui64" integer suffixes in builtin lexer expressions */
#define TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS     1                                      /* Treat 'a' as an integer, rather than as a string (in C, this is always the case) */

/* Force extensions to use the names they'd been using in TPP2 */
#define TPP_EXTNAME_TRIGRAPHS                           "trigraphs"
#define TPP_EXTNAME_DIGRAPHS                            "digraphs"
#define TPP_EXTNAME_NAMED_VARARGS_IN_MACROS             "named-varargs-in-macros"
#define TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS             "glue-comma-in-macros"
#define TPP_EXTNAME_VA_COMMA_IN_MACROS                  "va-comma-in-macros"
#define TPP_EXTNAME_VA_OPT_IN_MACROS                    "va-opt-in-macros"
#define TPP_EXTNAME_VA_NARGS_IN_MACROS                  "va-nargs-in-macros"
#define TPP_EXTNAME_VA_ARGS_IN_MACROS                   "va-args-in-macros"
#define TPP_EXTNAME_ESCAPE_E_IN_STRINGS                 "escape-e-in-strings"
#define TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS       "alternative-macro-parenthesis"
#define TPP_EXTNAME_MACRO_RECURSION                     "macro-recursion"
#define TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE           "macro-argument-whitespace"
#define TPP_EXTNAME_MACRO___pragma                      "msvc-pragma-support" /* TPP3 renamed "-fmsvc-pragma-support" -> "-f__pragma" */
#define TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT              "charize-macro-argument"
#define TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT          "dont-expand-macro-argument"
#define TPP_EXTNAME_CPP_WARNING                         "warning-directives"
#define TPP_EXTNAME_CPP_EXCLAIM                         "shebang-directives"
#define TPP_EXTNAME_CPP_INCLUDE_NEXT                    "include-next-directives"
#define TPP_EXTNAME_CPP_IMPORT                          "import-directives"
#define TPP_EXTNAME_CPP_IDENT_SCCS                      "ident-directives"
#define TPP_EXTNAME_MACRO___BASE_FILE__                 "basefile-macro"
#define TPP_EXTNAME_MACRO___INCLUDE_LEVEL__             "include-level-macro"
#define TPP_EXTNAME_MACRO___COUNTER__                   "counter-macro"
#define TPP_EXTNAME_MACRO___TIMESTAMP__                 "timestamp-macro"
#define TPP_EXTNAME_MACRO___COLUMN__                    "column-macro"
#define TPP_EXTNAME_NUMERIC_DATE_MACROS                 "numeric-date-macros"
#define TPP_EXTNAME_NUMERIC_TIME_MACROS                 "numeric-time-macros"
#define TPP_EXTNAME_MACRO___TPP_EVAL                    "tpp-eval-macro"
#define TPP_EXTNAME_MACRO___TPP_UNIQUE                  "tpp-unique-macro"
#define TPP_EXTNAME_MACRO___TPP_LOAD_FILE               "tpp-load-file-macro"
#define TPP_EXTNAME_MACRO___TPP_COUNTER                 "tpp-counter-macro"
#define TPP_EXTNAME_MACRO___TPP_RANDOM                  "tpp-random-macro"
#define TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE           "tpp-str-decompile-macro"
#define TPP_EXTNAME_MACRO___TPP_STR_SUBSTR              "tpp-str-substr-macro"
#define TPP_EXTNAME_MACRO___TPP_STR_PACK                "tpp-str-pack-macro"
#define TPP_EXTNAME_MACRO___TPP_STR_SIZE                "tpp-str-size-macro"
#define TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS            "tpp-count-tokens-macro"
#define TPP_EXTNAME_MACRO___TPP_IDENTIFIER              "tpp-identifier-macro"
#define TPP_EXTNAME_CPP_ASSERT                          "assertions"
#define TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR         "dont-expand-defined"
#define TPP_EXTNAME_TRADITIONAL_MACROS                  "traditional-macro"
#define TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES       "extensions-are-features" /* TPP3 renamed "-fextensions-are-features" -> "-fclang-extensions-are-features" */
#define TPP_EXTNAME_BUILTIN_EXPR_STRINGS                "strings-in-expressions"
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT    "if-else-optional-true"
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS "ifelse-in-expressions"
#define TPP_EXTNAME_BUILTIN_EXPR_LOGICAL_XOR            "logical-xor-in-expressions"
#define TPP_EXTNAME_BUILTIN_EXPR_BINARY_LITERALS        "binary-literals"
#define TPP_EXTNAME_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS "fixed-length-integrals"
/************************************************************************/





/************************************************************************/
/* CUSTOM HOOKS                                                         */
/************************************************************************/
#ifdef TPP_CONFIG_USERSTREAMS
#define tpp_io_handle         TPP_USERSTREAM_TYPE
#define tpp_io_handle_INVALID TPP_STREAM_INVALID
#endif /* TPP_CONFIG_USERSTREAMS */

/************************************************************************/





/************************************************************************/
/* PULL IN HEADER                                                       */
/************************************************************************/
#include "tpp-amalgamation.h"
/************************************************************************/





/************************************************************************/
/* PROVIDE ALIASES                                                      */
/************************************************************************/

/*[[[deemon
import * from deemon;

function alias(tpp2Name, tpp3Name, onlyIfDefined = true, condition: string = "") {
	local nsTpp2Name = "TPP_" + tpp2Name;
	if (!condition)
		condition = onlyIfDefined ? f"defined({tpp3Name})" : "1";
	local simpleCondition = condition;
	if (condition != "1") {
		try {
			simpleCondition = condition.rescanf(r"defined\((\w+)\)")...;
		} catch (...) {
			simpleCondition = condition;
		}
	}
	if (nsTpp2Name != tpp3Name || condition == "1") {
		if (condition != "1") {
			if (simpleCondition != condition) {
				print("#ifdef ", simpleCondition);
			} else {
				print("#if ", condition);
			}
		}
		if (nsTpp2Name != tpp3Name)
			print("#define ", nsTpp2Name, " ", tpp3Name);
		print("#if TPP2_HAVE_GLOBAL_NAMESPACE");
		print("#define ", tpp2Name, " ", tpp3Name);
		print("#endif /" "* TPP2_HAVE_GLOBAL_NAMESPACE *" "/");
		if (condition != "1")
			print("#endif /" "* ", simpleCondition, " *" "/");
	} else {
		print("#if TPP2_HAVE_GLOBAL_NAMESPACE && ", condition);
		print("#define ", tpp2Name, " ", tpp3Name);
		print("#endif /" "* TPP2_HAVE_GLOBAL_NAMESPACE && ", simpleCondition, " *" "/");
	}
}

// Token IDs
alias("TOK_EOF", "TPP_TOK_EOF", onlyIfDefined: false);

alias("TOK_CHAR",      "TPP_TOK_CHAR",      condition: "TPP_HAVE_TPP_TOK_CHAR");
alias("TOK_STRING",    "TPP_TOK_STRING",    condition: "TPP_HAVE_TPP_TOK_STRING");
alias("TOK_INT",       "TPP_TOK_INT",       condition: "TPP_HAVE_TPP_TOK_INT");
alias("TOK_FLOAT",     "TPP_TOK_FLOAT",     condition: "TPP_HAVE_TPP_TOK_FLOAT");
alias("TOK_LF",        "TPP_TOK_LF",        onlyIfDefined: false);
alias("TOK_SPACE",     "TPP_TOK_SPACE",     onlyIfDefined: false);
alias("TOK_ADD",       "TPP_TOK_PLUS",      onlyIfDefined: false);
alias("TOK_AND",       "TPP_TOK_AMP",       onlyIfDefined: false);
alias("TOK_ASSIGN",    "TPP_TOK_EQUAL",     onlyIfDefined: false);
alias("TOK_AT",        "TPP_TOK_AT",        onlyIfDefined: false);
alias("TOK_BACKSLASH", "TPP_TOK_BACKSLASH", onlyIfDefined: false);
alias("TOK_COLON",     "TPP_TOK_COLON",     onlyIfDefined: false);
alias("TOK_COMMA",     "TPP_TOK_COMMA",     onlyIfDefined: false);
alias("TOK_DIV",       "TPP_TOK_SLASH",     onlyIfDefined: false);
alias("TOK_DOT",       "TPP_TOK_DOT",       onlyIfDefined: false);
alias("TOK_HASH",      "TPP_TOK_POUND",     onlyIfDefined: false);
alias("TOK_LANGLE",    "TPP_TOK_LANGLE",    onlyIfDefined: false);
alias("TOK_RANGLE",    "TPP_TOK_RANGLE",    onlyIfDefined: false);
alias("TOK_LBRACKET",  "TPP_TOK_LBRACKET",  onlyIfDefined: false);
alias("TOK_RBRACKET",  "TPP_TOK_RBRACKET",  onlyIfDefined: false);
alias("TOK_LBRACE",    "TPP_TOK_LBRACE",    onlyIfDefined: false);
alias("TOK_RBRACE",    "TPP_TOK_RBRACE",    onlyIfDefined: false);
alias("TOK_LPAREN",    "TPP_TOK_LPAREN",    onlyIfDefined: false);
alias("TOK_RPAREN",    "TPP_TOK_RPAREN",    onlyIfDefined: false);
alias("TOK_MOD",       "TPP_TOK_PERCENT",   onlyIfDefined: false);
alias("TOK_MUL",       "TPP_TOK_STAR",      onlyIfDefined: false);
alias("TOK_NOT",       "TPP_TOK_EXCLAIM",   onlyIfDefined: false);
alias("TOK_OR",        "TPP_TOK_PIPE",      onlyIfDefined: false);
alias("TOK_QUESTION",  "TPP_TOK_QMARK",     onlyIfDefined: false);
alias("TOK_SEMICOLON", "TPP_TOK_SEMICOLON", onlyIfDefined: false);
alias("TOK_SUB",       "TPP_TOK_MINUS",     onlyIfDefined: false);
alias("TOK_TILDE",     "TPP_TOK_TILDE",     onlyIfDefined: false);
alias("TOK_XOR",       "TPP_TOK_HAT",       onlyIfDefined: false);

alias("TOK_SHL", "TPP_TOK_LANGLE_LANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE");
alias("TOK_SHR", "TPP_TOK_RANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE");

// Custom handling required here because "TPP_TOK_EQUAL" is a single-character token in TPP3
print("#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_EQUAL_EQUAL");
print("#define TOK_EQUAL TPP_TOK_EQUAL_EQUAL");
print("#endif /" "* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_EQUAL_EQUAL *" "/");

alias("TOK_NOT_EQUAL", "TPP_TOK_EXCLAIM_EQUAL", condition: "TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL");
alias("TOK_GREATER_EQUAL", "TPP_TOK_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_EQUAL");
alias("TOK_LOWER_EQUAL", "TPP_TOK_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_EQUAL");
alias("TOK_DOTS", "TPP_TOK_DOT_DOT_DOT", condition: "TPP_HAVE_TPP_TOK_DOT_DOT_DOT");
alias("TOK_ADD_EQUAL", "TPP_TOK_PLUS_EQUAL", condition: "TPP_HAVE_TPP_TOK_PLUS_EQUAL");
alias("TOK_SUB_EQUAL", "TPP_TOK_MINUS_EQUAL", condition: "TPP_HAVE_TPP_TOK_MINUS_EQUAL");
alias("TOK_MUL_EQUAL", "TPP_TOK_STAR_EQUAL", condition: "TPP_HAVE_TPP_TOK_STAR_EQUAL");
alias("TOK_DIV_EQUAL", "TPP_TOK_SLASH_EQUAL", condition: "TPP_HAVE_TPP_TOK_SLASH_EQUAL");
alias("TOK_MOD_EQUAL", "TPP_TOK_PERCENT_EQUAL", condition: "TPP_HAVE_TPP_TOK_PERCENT_EQUAL");
alias("TOK_SHL_EQUAL", "TPP_TOK_LANGLE_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL");
alias("TOK_SHR_EQUAL", "TPP_TOK_RANGLE_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL");
alias("TOK_AND_EQUAL", "TPP_TOK_AMP_EQUAL", condition: "TPP_HAVE_TPP_TOK_AMP_EQUAL");
alias("TOK_OR_EQUAL", "TPP_TOK_PIPE_EQUAL", condition: "TPP_HAVE_TPP_TOK_PIPE_EQUAL");
alias("TOK_XOR_EQUAL", "TPP_TOK_HAT_EQUAL", condition: "TPP_HAVE_TPP_TOK_HAT_EQUAL");
alias("TOK_POW_EQUAL", "TPP_TOK_STAR_STAR_EQUAL", condition: "TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL");
alias("TOK_AT_EQUAL", "TPP_TOK_AT_EQUAL", condition: "TPP_HAVE_TPP_TOK_AT_EQUAL");
alias("TOK_GLUE", "TPP_TOK_POUND_POUND", condition: "TPP_HAVE_TPP_TOK_POUND_POUND");
alias("TOK_LAND", "TPP_TOK_AMP_AMP", condition: "TPP_HAVE_TPP_TOK_AMP_AMP");
alias("TOK_LOR", "TPP_TOK_PIPE_PIPE", condition: "TPP_HAVE_TPP_TOK_PIPE_PIPE");
alias("TOK_LXOR", "TPP_TOK_HAT_HAT", condition: "TPP_HAVE_TPP_TOK_HAT_HAT");
alias("TOK_INC", "TPP_TOK_PLUS_PLUS", condition: "TPP_HAVE_TPP_TOK_PLUS_PLUS");
alias("TOK_DEC", "TPP_TOK_MINUS_MINUS", condition: "TPP_HAVE_TPP_TOK_MINUS_MINUS");
alias("TOK_POW", "TPP_TOK_STAR_STAR", condition: "TPP_HAVE_TPP_TOK_STAR_STAR");
alias("TOK_TILDE_TILDE", "TPP_TOK_TILDE_TILDE", condition: "TPP_HAVE_TPP_TOK_TILDE_TILDE");
alias("TOK_ARROW", "TPP_TOK_MINUS_RANGLE", condition: "TPP_HAVE_TPP_TOK_MINUS_RANGLE");
alias("TOK_COLON_EQUAL", "TPP_TOK_COLON_EQUAL", condition: "TPP_HAVE_TPP_TOK_COLON_EQUAL");
alias("TOK_NAMESPACE", "TPP_TOK_COLON_COLON", condition: "TPP_HAVE_TPP_TOK_COLON_COLON");
alias("TOK_ARROW_STAR", "TPP_TOK_MINUS_RANGLE_STAR", condition: "TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR");
alias("TOK_DOT_STAR", "TPP_TOK_DOT_STAR", condition: "TPP_HAVE_TPP_TOK_DOT_STAR");
alias("TOK_DOTDOT", "TPP_TOK_DOT_DOT", condition: "TPP_HAVE_TPP_TOK_DOT_DOT");
alias("TOK_LOGT", "TPP_TOK_LANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_RANGLE");
alias("TOK_LANGLE3", "TPP_TOK_LANGLE_LANGLE_LANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE");
alias("TOK_RANGLE3", "TPP_TOK_RANGLE_RANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE");
alias("TOK_LANGLE3_EQUAL", "TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL");
alias("TOK_RANGLE3_EQUAL", "TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL");
alias("TOK_EQUAL3", "TPP_TOK_EQUAL_EQUAL_EQUAL", condition: "TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL");
alias("TOK_NOT_EQUAL3", "TPP_TOK_EXCLAIM_EQUAL_EQUAL", condition: "TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL");
alias("TOK_QMARK_QMARK", "TPP_TOK_QMARK_QMARK", condition: "TPP_HAVE_TPP_TOK_QMARK_QMARK");
alias("TOK_KEYWORD_BEGIN", "TPP_TOK_KEYWORD_BEGIN", onlyIfDefined: false);
alias("TOK_TWOCHAR_END", "TPP_TOK_MULTICHAR_END", onlyIfDefined: false);

// Name aliases 
alias("TOK_POS",           "TPP_TOK_PLUS",      onlyIfDefined: false);
alias("TOK_NEG",           "TPP_TOK_MINUS",     onlyIfDefined: false);
alias("TOK_LOWER",         "TPP_TOK_LANGLE",    onlyIfDefined: false);
alias("TOK_GREATER",       "TPP_TOK_RANGLE",    onlyIfDefined: false);
alias("TOK_COLON_COLON",   "TPP_TOK_COLON_COLON", condition: "TPP_HAVE_TPP_TOK_COLON_COLON");
alias("TOK_LOWER_GREATER", "TPP_TOK_LANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_RANGLE");
alias("TOK_LANGLE_RANGLE", "TPP_TOK_LANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_RANGLE");
alias("TOK_LANGLE1",       "TPP_TOK_LANGLE",    onlyIfDefined: false);
alias("TOK_LANGLE2",       "TPP_TOK_LANGLE_LANGLE", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE");
alias("TOK_LANGLE_EQUAL",  "TPP_TOK_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_EQUAL");
alias("TOK_LANGLE1_EQUAL", "TPP_TOK_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_EQUAL");
alias("TOK_LANGLE2_EQUAL", "TPP_TOK_LANGLE_LANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL");
alias("TOK_RANGLE1",       "TPP_TOK_RANGLE",    onlyIfDefined: false);
alias("TOK_RANGLE2",       "TPP_TOK_RANGLE_RANGLE", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE");
alias("TOK_RANGLE_EQUAL",  "TPP_TOK_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_EQUAL");
alias("TOK_RANGLE1_EQUAL", "TPP_TOK_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_EQUAL");
alias("TOK_RANGLE2_EQUAL", "TPP_TOK_RANGLE_RANGLE_EQUAL", condition: "TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL");

// Deprecated typos 
alias("TOK_COLLON",        "TPP_TOK_COLON",     onlyIfDefined: false);
alias("TOK_COLLON_EQUAL",  "TPP_TOK_COLON_EQUAL", condition: "TPP_HAVE_TPP_TOK_COLON_EQUAL");
alias("TOK_COLLON_COLLON", "TPP_TOK_COLON_COLON", condition: "TPP_HAVE_TPP_TOK_COLON_COLON");

// Keywords
alias("KWD_if", "TPP_KWD_if");
alias("KWD_ifdef", "TPP_KWD_ifdef");
alias("KWD_ifndef", "TPP_KWD_ifndef");
alias("KWD_elif", "TPP_KWD_elif");
alias("KWD_else", "TPP_KWD_else");
alias("KWD_endif", "TPP_KWD_endif");
alias("KWD_define", "TPP_KWD_define");
alias("KWD_defined", "TPP_KWD_defined");
alias("KWD_undef", "TPP_KWD_undef");
alias("KWD_include", "TPP_KWD_include");
alias("KWD_include_next", "TPP_KWD_include_next");
alias("KWD_import", "TPP_KWD_import");
alias("KWD_line", "TPP_KWD_line");
alias("KWD_error", "TPP_KWD_error");
alias("KWD_warning", "TPP_KWD_warning");
alias("KWD_ident", "TPP_KWD_ident");
alias("KWD_sccs", "TPP_KWD_sccs");
alias("KWD_assert", "TPP_KWD_assert");
alias("KWD_unassert", "TPP_KWD_unassert");
alias("KWD_pragma", "TPP_KWD_pragma");
alias("KWD__Pragma", "TPP_KWD__Pragma");
alias("KWD___pragma", "TPP_KWD___pragma");
alias("KWD___FILE__", "TPP_KWD___FILE__");
alias("KWD___LINE__", "TPP_KWD___LINE__");
alias("KWD___TIME__", "TPP_KWD___TIME__");
alias("KWD___DATE__", "TPP_KWD___DATE__");
alias("KWD___BASE_FILE__", "TPP_KWD___BASE_FILE__");
alias("KWD___INCLUDE_LEVEL__", "TPP_KWD___INCLUDE_LEVEL__");
alias("KWD___INCLUDE_DEPTH__", "TPP_KWD___INCLUDE_DEPTH__");
alias("KWD___COUNTER__", "TPP_KWD___COUNTER__");
alias("KWD___TIMESTAMP__", "TPP_KWD___TIMESTAMP__");
alias("KWD___COLUMN__", "TPP_KWD___COLUMN__");
alias("KWD___is_identifier", "TPP_KWD___is_identifier");
alias("KWD___is_deprecated", "TPP_KWD___is_deprecated");
alias("KWD___is_poisoned", "TPP_KWD___is_poisoned");
alias("KWD___has_attribute", "TPP_KWD___has_attribute");
alias("KWD___has_builtin", "TPP_KWD___has_builtin");
alias("KWD___has_cpp_attribute", "TPP_KWD___has_cpp_attribute");
alias("KWD___has_declspec_attribute", "TPP_KWD___has_declspec_attribute");
alias("KWD___has_feature", "TPP_KWD___has_feature");
alias("KWD___has_extension", "TPP_KWD___has_extension");
alias("KWD___has_warning", "TPP_KWD___has_warning");
alias("KWD___has_known_extension", "TPP_KWD___has_known_extension");
alias("KWD___has_known_warning", "TPP_KWD___has_known_warning");
alias("KWD___has_include", "TPP_KWD___has_include");
alias("KWD___has_include_next", "TPP_KWD___has_include_next");
alias("KWD___VA_ARGS__", "TPP_KWD___VA_ARGS__");
alias("KWD___VA_COMMA__", "TPP_KWD___VA_COMMA__");
alias("KWD___VA_NARGS__", "TPP_KWD___VA_NARGS__");
alias("KWD___VA_OPT__", "TPP_KWD___VA_OPT__");
alias("KWD___TPP_EVAL", "TPP_KWD___TPP_EVAL");
alias("KWD___TPP_LOAD_FILE", "TPP_KWD___TPP_LOAD_FILE");
alias("KWD___TPP_COUNTER", "TPP_KWD___TPP_COUNTER");
alias("KWD___TPP_RANDOM", "TPP_KWD___TPP_RANDOM");
alias("KWD___TPP_STR_DECOMPILE", "TPP_KWD___TPP_STR_DECOMPILE");
alias("KWD___TPP_STR_AT", "TPP_KWD___TPP_STR_AT");
alias("KWD___TPP_STR_SUBSTR", "TPP_KWD___TPP_STR_SUBSTR");
alias("KWD___TPP_STR_PACK", "TPP_KWD___TPP_STR_PACK");
alias("KWD___TPP_STR_SIZE", "TPP_KWD___TPP_STR_SIZE");
alias("KWD___TPP_UNIQUE", "TPP_KWD___TPP_UNIQUE");
alias("KWD___TPP_COUNT_TOKENS", "TPP_KWD___TPP_COUNT_TOKENS");
alias("KWD___TPP_IDENTIFIER", "TPP_KWD___TPP_IDENTIFIER");
alias("KWD___DATE_DAY__", "TPP_KWD___DATE_DAY__");
alias("KWD___DATE_WDAY__", "TPP_KWD___DATE_WDAY__");
alias("KWD___DATE_YDAY__", "TPP_KWD___DATE_YDAY__");
alias("KWD___DATE_MONTH__", "TPP_KWD___DATE_MONTH__");
alias("KWD___DATE_YEAR__", "TPP_KWD___DATE_YEAR__");
alias("KWD___TIME_SEC__", "TPP_KWD___TIME_SEC__");
alias("KWD___TIME_MIN__", "TPP_KWD___TIME_MIN__");
alias("KWD___TIME_HOUR__", "TPP_KWD___TIME_HOUR__");
alias("KWD_tpp_dollar_is_alpha", "TPP_KWD_tpp_dollar_is_alpha");
alias("KWD_tpp_va_args", "TPP_KWD_tpp_va_args");
alias("KWD_tpp_named_va_args", "TPP_KWD_tpp_named_va_args");
alias("KWD_tpp_va_comma", "TPP_KWD_tpp_va_comma");
alias("KWD_tpp_msvc_integer_suffix", "TPP_KWD_tpp_msvc_integer_suffix");
alias("KWD_tpp_charize_operator", "TPP_KWD_tpp_charize_operator");
alias("KWD_tpp_trigraphs", "TPP_KWD_tpp_trigraphs");
alias("KWD_tpp_digraphs", "TPP_KWD_tpp_digraphs");
alias("KWD_tpp_reemit_unknown_pragmas", "TPP_KWD_tpp_reemit_unknown_pragmas");
alias("KWD_tpp_pragma_push_macro", "TPP_KWD_tpp_pragma_push_macro");
alias("KWD_tpp_pragma_pop_macro", "TPP_KWD_tpp_pragma_pop_macro");
alias("KWD_tpp_pragma_region", "TPP_KWD_tpp_pragma_region");
alias("KWD_tpp_pragma_endregion", "TPP_KWD_tpp_pragma_endregion");
alias("KWD_tpp_pragma_warning", "TPP_KWD_tpp_pragma_warning");
alias("KWD_tpp_pragma_message", "TPP_KWD_tpp_pragma_message");
alias("KWD_tpp_pragma_error", "TPP_KWD_tpp_pragma_error");
alias("KWD_tpp_pragma_once", "TPP_KWD_tpp_pragma_once");
alias("KWD_tpp_pragma_tpp_exec", "TPP_KWD_tpp_pragma_tpp_exec");
alias("KWD_tpp_pragma_deprecated", "TPP_KWD_tpp_pragma_deprecated");
alias("KWD_tpp_pragma_tpp_set_keyword_flags", "TPP_KWD_tpp_pragma_tpp_set_keyword_flags");
alias("KWD_tpp_directive_include_next", "TPP_KWD_tpp_directive_include_next");
alias("KWD_tpp_directive_import", "TPP_KWD_tpp_directive_import");
alias("KWD_tpp_directive_warning", "TPP_KWD_tpp_directive_warning");
alias("KWD_tpp_lxor", "TPP_KWD_tpp_lxor");
alias("KWD_tpp_token_tilde_tilde", "TPP_KWD_tpp_token_tilde_tilde");
alias("KWD_tpp_token_pow", "TPP_KWD_tpp_token_pow");
alias("KWD_tpp_token_lxor", "TPP_KWD_tpp_token_lxor");
alias("KWD_tpp_token_arrow", "TPP_KWD_tpp_token_arrow");
alias("KWD_tpp_token_colon_assign", "TPP_KWD_tpp_token_colon_assign");
alias("KWD_tpp_token_colon_colon", "TPP_KWD_tpp_token_colon_colon");
alias("KWD_tpp_macro_calling_conventions", "TPP_KWD_tpp_macro_calling_conventions");
alias("KWD_tpp_strict_whitespace", "TPP_KWD_tpp_strict_whitespace");
alias("KWD_tpp_strict_integer_overflow", "TPP_KWD_tpp_strict_integer_overflow");
alias("KWD_tpp_support_ansi_characters", "TPP_KWD_tpp_support_ansi_characters");
alias("KWD_tpp_emit_lf_after_directive", "TPP_KWD_tpp_emit_lf_after_directive");
alias("KWD_tpp_if_cond_expression", "TPP_KWD_tpp_if_cond_expression");
alias("KWD_tpp_debug", "TPP_KWD_tpp_debug");
alias("KWD___TPP_VERSION__", "TPP_KWD___TPP_VERSION__", onlyIfDefined: false);

// Extensions
alias("EXT_TRIGRAPHS", "TPP_EXT_TRIGRAPHS");
alias("EXT_DIGRAPHS", "TPP_EXT_DIGRAPHS");
alias("EXT_GCC_VA_ARGS", "TPP_EXT_NAMED_VARARGS_IN_MACROS");
alias("EXT_GCC_VA_COMMA", "TPP_EXT_VA_GLUE_COMMA_IN_MACROS");
alias("EXT_VA_COMMA", "TPP_EXT_VA_COMMA_IN_MACROS");
alias("EXT_VA_OPT", "TPP_EXT_VA_OPT_IN_MACROS");
alias("EXT_VA_NARGS", "TPP_EXT_VA_NARGS_IN_MACROS");
alias("EXT_VA_ARGS", "TPP_EXT_VA_ARGS_IN_MACROS");
alias("EXT_STR_E", "TPP_EXT_ESCAPE_E_IN_STRINGS");
alias("EXT_ALTMAC", "TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS");
alias("EXT_RECMAC", "TPP_EXT_MACRO_RECURSION");
alias("EXT_ARGSPACE", "TPP_EXT_MACRO_ARGUMENT_WHITESPACE");
alias("EXT_MSVC_PRAGMA", "TPP_EXT_MACRO___pragma");
alias("EXT_HASH_AT", "TPP_EXT_CHARIZE_MACRO_ARGUMENT");
alias("EXT_HASH_XCLAIM", "TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT");
alias("EXT_WARNING", "TPP_EXT_CPP_WARNING");
alias("EXT_SHEBANG", "TPP_EXT_CPP_EXCLAIM");
alias("EXT_INCLUDE_NEXT", "TPP_EXT_CPP_INCLUDE_NEXT");
alias("EXT_IMPORT", "TPP_EXT_CPP_IMPORT");
alias("EXT_IDENT_SCCS", "TPP_EXT_CPP_IDENT_SCCS");
alias("EXT_BASEFILE", "TPP_EXT_MACRO___BASE_FILE__");
alias("EXT_INCLUDE_LEVEL", "TPP_EXT_MACRO___INCLUDE_LEVEL__");
alias("EXT_COUNTER", "TPP_EXT_MACRO___COUNTER__");
alias("EXT_TIMESTAMP", "TPP_EXT_MACRO___TIMESTAMP__");
alias("EXT_COLUMN", "TPP_EXT_MACRO___COLUMN__");
alias("EXT_DATEUTILS", "TPP_EXT_NUMERIC_DATE_MACROS");
alias("EXT_TIMEUTILS", "TPP_EXT_NUMERIC_TIME_MACROS");
alias("EXT_TPP_EVAL", "TPP_EXT_MACRO_TPP_EVAL");
alias("EXT_TPP_UNIQUE", "TPP_EXT_MACRO_TPP_UNIQUE");
alias("EXT_TPP_LOAD_FILE", "TPP_EXT_MACRO_TPP_LOAD_FILE");
alias("EXT_TPP_COUNTER", "TPP_EXT_MACRO_TPP_COUNTER");
alias("EXT_TPP_RANDOM", "TPP_EXT_MACRO_TPP_RANDOM");
alias("EXT_TPP_STR_DECOMPILE", "TPP_EXT_MACRO_TPP_STR_DECOMPILE");
alias("EXT_TPP_STR_SUBSTR", "TPP_EXT_MACRO_TPP_STR_SUBSTR");
alias("EXT_TPP_STR_PACK", "TPP_EXT_MACRO_TPP_STR_PACK");
alias("EXT_TPP_STR_SIZE", "TPP_EXT_MACRO_TPP_STR_SIZE");
alias("EXT_TPP_COUNT_TOKENS", "TPP_EXT_MACRO_TPP_COUNT_TOKENS");
alias("EXT_TPP_IDENTIFIER", "TPP_EXT_MACRO_TPP_IDENTIFIER");
alias("EXT_ASSERTIONS", "TPP_EXT_CPP_ASSERT");
alias("EXT_NO_EXPAND_DEFINED", "TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR");
alias("EXT_TRADITIONAL_MACRO", "TPP_EXT_TRADITIONAL_MACROS");
alias("EXT_EXT_ARE_FEATURES", "TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES");
alias("EXT_STRINGOPS", "TPP_EXT_BUILTIN_EXPR_STRINGS");
alias("EXT_GCC_IFELSE", "TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT");
alias("EXT_IFELSE_IN_EXPR", "TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS");
alias("EXT_LXOR", "TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR");
alias("EXT_BININTEGRAL", "TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS");
alias("EXT_MSVC_FIXED_INT", "TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS");

// Warning groups
alias("WG_COMMENT",  "TPP_WG_COMMENT");
alias("WG_COMMENTS", "TPP_WG_COMMENT");
alias("WG_MACROS", "TPP_WG_MACROS");
alias("WG_SYNTAX", "TPP_WG_SYNTAX");
alias("WG_USAGE", "TPP_WG_USAGE");
alias("WG_VALUE", "TPP_WG_VALUE");
alias("WG_BOOLVALUE", "TPP_WG_BOOLVALUE");
alias("WG_USER", "TPP_WG_USER");
alias("WG_ENVIRON", "TPP_WG_ENVIRON");
alias("WG_LIMIT", "TPP_WG_LIMIT");
alias("WG_UNDEF", "TPP_WG_UNDEF");
alias("WG_TRIGRAPHS", "TPP_WG_TRIGRAPHS");
alias("WG_EXPANSION_TO_DEFINED", "TPP_WG_EXPANSION_TO_DEFINED");
alias("WG_QUALITY", "TPP_WG_QUALITY");
alias("WG_DEPRECATED", "TPP_WG_DEPRECATED");
alias("WG_DEPENDENCY", "TPP_WG_DEPENDENCY");

// Warnings
alias("W_EXPECTED_KEYWORD_AFTER_DEFINE", "TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE");
alias("W_EXPECTED_KEYWORD_AFTER_UNDEF", "TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE");
alias("W_EXPECTED_KEYWORD_AFTER_IFDEF", "TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE");
alias("W_UNKNOWN_PREPROCESSOR_DIRECTIVE", "TPP_W_UNKNOWN_DIRECTIVE");
alias("W_ERROR", "TPP_W_ERROR");
alias("W_WARNING", "TPP_W_WARNING");
alias("W_EXPECTED_MACRO_ARGUMENT_NAME", "TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST");
alias("W_EXPECTED_ARGEND_AFTER_VARARGS", "TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST");
alias("W_EXPECTED_COMMA_OR_ARGEND", "TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST");
alias("W_TOO_MANY_MACRO_ARGUMENTS", "TPP_W_TOO_MANY_ARGUMENTS");
alias("W_EOF_IN_MACRO_ARGUMENT_LIST", "TPP_W_EOF_IN_ARGUMENT_LIST");
alias("W_REDEFINING_MACRO", "TPP_W_REDEFINE_MACRO");
alias("W_CANT_UNDEF_BUILTIN_MACRO", "TPP_W_CANNOT_UNDEF_BUILTIN_MACRO");
alias("W_EXPECTED_STRING_AFTER_PUSHMACRO", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_STRD", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_LPAREN", "TPP_W_UNEXPECTED_TOKEN");
alias("W_EXPECTED_RPAREN", "TPP_W_UNEXPECTED_TOKEN");
alias("W_DEPRECATED_IDENTIFIER", "TPP_W_DEPRECATED_KEYWORD");
alias("W_ARGUMENT_NAMED_ALREADY_TAKEN", "TPP_W_DUPLICATE_MACRO_PARAMETER_NAME");
alias("W_SLASHSTAR_INSIDE_OF_COMMENT", "TPP_W_SLASHSTAR_INSIDE_OF_COMMENT");
alias("W_LINE_COMMENT_CONTINUED", "TPP_W_LINE_COMMENT_CONTINUED");
alias("W_NOT_ENGOUH_MACRO_ARGUMENTS", "TPP_W_TOO_FEW_ARGUMENTS");
alias("W_STRING_TERMINATED_BY_LINEFEED", "TPP_W_STRING_TERMINATED_BY_LINEFEED");
alias("W_STRING_TERMINATED_BY_EOF", "TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF");
alias("W_COMMENT_TERMINATED_BY_EOF", "TPP_W_COMMENT_TERMINATED_BY_EOF");
alias("W_ENCOUNTERED_TRIGRAPH", "TPP_W_ENCOUNTERED_TRIGRAPH");
alias("W_EXPECTED_STRING_IN_EXPRESSION", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_MESSAGE", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_DEPRECATED", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_EXEC", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_SETF", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_STRAT", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_CNTTOK", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_PRGERROR", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_EXTENSION", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_TPP_INCPTH", "TPP_W_EXPECTED_STRING");
alias("W_DEFINED_IN_MACRO_BODY", "TPP_W_EXPANSION_TO_DEFINED");
alias("W_EXPECTED_STRING_AFTER_IDENT", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_STRING_AFTER_GCC_DIAG", "TPP_W_EXPECTED_STRING");
alias("W_EXPECTED_LPAREN_AFTER_VA_OPT", "TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT");
alias("W_EXPECTED_RPAREN_AFTER_VA_OPT", "TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT");
alias("W_IF_WITHOUT_ENDIF", "TPP_W_EOF_BEFORE_ENDIF");
alias("W_ENDIF_WITHOUT_IF", "TPP_W_ENDIF_WITHOUT_IF");
alias("W_ELSE_WITHOUT_IF", "TPP_W_ELIF_OR_ELSE_WITHOUT_IF");
alias("W_ELSE_AFTER_ELSE", "TPP_W_ELIF_OR_ELSE_AFTER_ELSE");
alias("W_ELIF_WITHOUT_IF", "TPP_W_ELIF_OR_ELSE_WITHOUT_IF");
alias("W_ELIF_AFTER_ELSE", "TPP_W_ELIF_OR_ELSE_AFTER_ELSE");
]]]*/
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_EOF TPP_TOK_EOF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_CHAR
#define TOK_CHAR TPP_TOK_CHAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_CHAR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_STRING
#define TOK_STRING TPP_TOK_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_STRING */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_INT
#define TOK_INT TPP_TOK_INT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_INT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_FLOAT
#define TOK_FLOAT TPP_TOK_FLOAT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_FLOAT */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LF TPP_TOK_LF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SPACE TPP_TOK_SPACE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_ADD TPP_TOK_PLUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ADD TPP_TOK_PLUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_AND TPP_TOK_AMP
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_AND TPP_TOK_AMP
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_ASSIGN TPP_TOK_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ASSIGN TPP_TOK_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_AT TPP_TOK_AT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_BACKSLASH TPP_TOK_BACKSLASH
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COLON TPP_TOK_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COMMA TPP_TOK_COMMA
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_DIV TPP_TOK_SLASH
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DIV TPP_TOK_SLASH
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DOT TPP_TOK_DOT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_HASH TPP_TOK_POUND
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_HASH TPP_TOK_POUND
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE TPP_TOK_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE TPP_TOK_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LBRACKET TPP_TOK_LBRACKET
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RBRACKET TPP_TOK_RBRACKET
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LBRACE TPP_TOK_LBRACE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RBRACE TPP_TOK_RBRACE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LPAREN TPP_TOK_LPAREN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RPAREN TPP_TOK_RPAREN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_MOD TPP_TOK_PERCENT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_MOD TPP_TOK_PERCENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_MUL TPP_TOK_STAR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_MUL TPP_TOK_STAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_NOT TPP_TOK_EXCLAIM
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NOT TPP_TOK_EXCLAIM
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_OR TPP_TOK_PIPE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_OR TPP_TOK_PIPE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_QUESTION TPP_TOK_QMARK
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_QUESTION TPP_TOK_QMARK
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SEMICOLON TPP_TOK_SEMICOLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_SUB TPP_TOK_MINUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SUB TPP_TOK_MINUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_TILDE TPP_TOK_TILDE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_XOR TPP_TOK_HAT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_XOR TPP_TOK_HAT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#define TPP_TOK_SHL TPP_TOK_LANGLE_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SHL TPP_TOK_LANGLE_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_TOK_SHR TPP_TOK_RANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SHR TPP_TOK_RANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_EQUAL_EQUAL
#define TOK_EQUAL TPP_TOK_EQUAL_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#define TPP_TOK_NOT_EQUAL TPP_TOK_EXCLAIM_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NOT_EQUAL TPP_TOK_EXCLAIM_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TPP_TOK_GREATER_EQUAL TPP_TOK_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_GREATER_EQUAL TPP_TOK_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TPP_TOK_LOWER_EQUAL TPP_TOK_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOWER_EQUAL TPP_TOK_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
#define TPP_TOK_DOTS TPP_TOK_DOT_DOT_DOT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DOTS TPP_TOK_DOT_DOT_DOT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
#define TPP_TOK_ADD_EQUAL TPP_TOK_PLUS_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ADD_EQUAL TPP_TOK_PLUS_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
#define TPP_TOK_SUB_EQUAL TPP_TOK_MINUS_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SUB_EQUAL TPP_TOK_MINUS_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
#define TPP_TOK_MUL_EQUAL TPP_TOK_STAR_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_MUL_EQUAL TPP_TOK_STAR_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
#define TPP_TOK_DIV_EQUAL TPP_TOK_SLASH_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DIV_EQUAL TPP_TOK_SLASH_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
#define TPP_TOK_MOD_EQUAL TPP_TOK_PERCENT_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_MOD_EQUAL TPP_TOK_PERCENT_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#define TPP_TOK_SHL_EQUAL TPP_TOK_LANGLE_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SHL_EQUAL TPP_TOK_LANGLE_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#define TPP_TOK_SHR_EQUAL TPP_TOK_RANGLE_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_SHR_EQUAL TPP_TOK_RANGLE_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
#define TPP_TOK_AND_EQUAL TPP_TOK_AMP_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_AND_EQUAL TPP_TOK_AMP_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
#define TPP_TOK_OR_EQUAL TPP_TOK_PIPE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_OR_EQUAL TPP_TOK_PIPE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
#define TPP_TOK_XOR_EQUAL TPP_TOK_HAT_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_XOR_EQUAL TPP_TOK_HAT_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
#define TPP_TOK_POW_EQUAL TPP_TOK_STAR_STAR_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_POW_EQUAL TPP_TOK_STAR_STAR_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_AT_EQUAL
#define TOK_AT_EQUAL TPP_TOK_AT_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
#define TPP_TOK_GLUE TPP_TOK_POUND_POUND
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_GLUE TPP_TOK_POUND_POUND
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
#define TPP_TOK_LAND TPP_TOK_AMP_AMP
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LAND TPP_TOK_AMP_AMP
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
#define TPP_TOK_LOR TPP_TOK_PIPE_PIPE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOR TPP_TOK_PIPE_PIPE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
#define TPP_TOK_LXOR TPP_TOK_HAT_HAT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LXOR TPP_TOK_HAT_HAT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
#define TPP_TOK_INC TPP_TOK_PLUS_PLUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_INC TPP_TOK_PLUS_PLUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
#define TPP_TOK_DEC TPP_TOK_MINUS_MINUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DEC TPP_TOK_MINUS_MINUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
#define TPP_TOK_POW TPP_TOK_STAR_STAR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_POW TPP_TOK_STAR_STAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_TILDE_TILDE
#define TOK_TILDE_TILDE TPP_TOK_TILDE_TILDE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
#define TPP_TOK_ARROW TPP_TOK_MINUS_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ARROW TPP_TOK_MINUS_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_COLON_EQUAL
#define TOK_COLON_EQUAL TPP_TOK_COLON_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
#define TPP_TOK_NAMESPACE TPP_TOK_COLON_COLON
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NAMESPACE TPP_TOK_COLON_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
#define TPP_TOK_ARROW_STAR TPP_TOK_MINUS_RANGLE_STAR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_ARROW_STAR TPP_TOK_MINUS_RANGLE_STAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_DOT_STAR
#define TOK_DOT_STAR TPP_TOK_DOT_STAR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
#define TPP_TOK_DOTDOT TPP_TOK_DOT_DOT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_DOTDOT TPP_TOK_DOT_DOT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TPP_TOK_LOGT TPP_TOK_LANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOGT TPP_TOK_LANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
#define TPP_TOK_LANGLE3 TPP_TOK_LANGLE_LANGLE_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE3 TPP_TOK_LANGLE_LANGLE_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
#define TPP_TOK_RANGLE3 TPP_TOK_RANGLE_RANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE3 TPP_TOK_RANGLE_RANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#define TPP_TOK_LANGLE3_EQUAL TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE3_EQUAL TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#define TPP_TOK_RANGLE3_EQUAL TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE3_EQUAL TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
#define TPP_TOK_EQUAL3 TPP_TOK_EQUAL_EQUAL_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_EQUAL3 TPP_TOK_EQUAL_EQUAL_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#define TPP_TOK_NOT_EQUAL3 TPP_TOK_EXCLAIM_EQUAL_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NOT_EQUAL3 TPP_TOK_EXCLAIM_EQUAL_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_QMARK_QMARK
#define TOK_QMARK_QMARK TPP_TOK_QMARK_QMARK
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_KEYWORD_BEGIN TPP_TOK_KEYWORD_BEGIN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_TWOCHAR_END TPP_TOK_MULTICHAR_END
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_TWOCHAR_END TPP_TOK_MULTICHAR_END
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_POS TPP_TOK_PLUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_POS TPP_TOK_PLUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_NEG TPP_TOK_MINUS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_NEG TPP_TOK_MINUS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_LOWER TPP_TOK_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOWER TPP_TOK_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_TOK_GREATER TPP_TOK_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_GREATER TPP_TOK_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_COLON_COLON
#define TOK_COLON_COLON TPP_TOK_COLON_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TPP_TOK_LOWER_GREATER TPP_TOK_LANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LOWER_GREATER TPP_TOK_LANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#define TOK_LANGLE_RANGLE TPP_TOK_LANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#define TPP_TOK_LANGLE1 TPP_TOK_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE1 TPP_TOK_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#define TPP_TOK_LANGLE2 TPP_TOK_LANGLE_LANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE2 TPP_TOK_LANGLE_LANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TOK_LANGLE_EQUAL TPP_TOK_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#define TPP_TOK_LANGLE1_EQUAL TPP_TOK_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE1_EQUAL TPP_TOK_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#define TPP_TOK_LANGLE2_EQUAL TPP_TOK_LANGLE_LANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_LANGLE2_EQUAL TPP_TOK_LANGLE_LANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#define TPP_TOK_RANGLE1 TPP_TOK_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE1 TPP_TOK_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#define TPP_TOK_RANGLE2 TPP_TOK_RANGLE_RANGLE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE2 TPP_TOK_RANGLE_RANGLE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TOK_RANGLE_EQUAL TPP_TOK_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#define TPP_TOK_RANGLE1_EQUAL TPP_TOK_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE1_EQUAL TPP_TOK_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#define TPP_TOK_RANGLE2_EQUAL TPP_TOK_RANGLE_RANGLE_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_RANGLE2_EQUAL TPP_TOK_RANGLE_RANGLE_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#define TPP_TOK_COLLON TPP_TOK_COLON
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COLLON TPP_TOK_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
#define TPP_TOK_COLLON_EQUAL TPP_TOK_COLON_EQUAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COLLON_EQUAL TPP_TOK_COLON_EQUAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
#define TPP_TOK_COLLON_COLLON TPP_TOK_COLON_COLON
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define TOK_COLLON_COLLON TPP_TOK_COLON_COLON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_if)
#define KWD_if TPP_KWD_if
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_if */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_ifdef)
#define KWD_ifdef TPP_KWD_ifdef
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_ifdef */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_ifndef)
#define KWD_ifndef TPP_KWD_ifndef
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_ifndef */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_elif)
#define KWD_elif TPP_KWD_elif
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_elif */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_else)
#define KWD_else TPP_KWD_else
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_else */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_endif)
#define KWD_endif TPP_KWD_endif
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_endif */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_define)
#define KWD_define TPP_KWD_define
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_define */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_defined)
#define KWD_defined TPP_KWD_defined
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_defined */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_undef)
#define KWD_undef TPP_KWD_undef
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_undef */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_include)
#define KWD_include TPP_KWD_include
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_include */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_include_next)
#define KWD_include_next TPP_KWD_include_next
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_include_next */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_import)
#define KWD_import TPP_KWD_import
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_import */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_line)
#define KWD_line TPP_KWD_line
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_line */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_error)
#define KWD_error TPP_KWD_error
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_error */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_warning)
#define KWD_warning TPP_KWD_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_ident)
#define KWD_ident TPP_KWD_ident
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_ident */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_sccs)
#define KWD_sccs TPP_KWD_sccs
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_sccs */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_assert)
#define KWD_assert TPP_KWD_assert
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_assert */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_unassert)
#define KWD_unassert TPP_KWD_unassert
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_unassert */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_pragma)
#define KWD_pragma TPP_KWD_pragma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_pragma */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD__Pragma)
#define KWD__Pragma TPP_KWD__Pragma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD__Pragma */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___pragma)
#define KWD___pragma TPP_KWD___pragma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___pragma */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___FILE__)
#define KWD___FILE__ TPP_KWD___FILE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___FILE__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___LINE__)
#define KWD___LINE__ TPP_KWD___LINE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___LINE__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIME__)
#define KWD___TIME__ TPP_KWD___TIME__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIME__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE__)
#define KWD___DATE__ TPP_KWD___DATE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___BASE_FILE__)
#define KWD___BASE_FILE__ TPP_KWD___BASE_FILE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___BASE_FILE__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___INCLUDE_LEVEL__)
#define KWD___INCLUDE_LEVEL__ TPP_KWD___INCLUDE_LEVEL__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___INCLUDE_LEVEL__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___INCLUDE_DEPTH__)
#define KWD___INCLUDE_DEPTH__ TPP_KWD___INCLUDE_DEPTH__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___INCLUDE_DEPTH__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___COUNTER__)
#define KWD___COUNTER__ TPP_KWD___COUNTER__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___COUNTER__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIMESTAMP__)
#define KWD___TIMESTAMP__ TPP_KWD___TIMESTAMP__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIMESTAMP__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___COLUMN__)
#define KWD___COLUMN__ TPP_KWD___COLUMN__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___COLUMN__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___is_identifier)
#define KWD___is_identifier TPP_KWD___is_identifier
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___is_identifier */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___is_deprecated)
#define KWD___is_deprecated TPP_KWD___is_deprecated
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___is_deprecated */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___is_poisoned)
#define KWD___is_poisoned TPP_KWD___is_poisoned
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___is_poisoned */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_attribute)
#define KWD___has_attribute TPP_KWD___has_attribute
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_attribute */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_builtin)
#define KWD___has_builtin TPP_KWD___has_builtin
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_builtin */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_cpp_attribute)
#define KWD___has_cpp_attribute TPP_KWD___has_cpp_attribute
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_cpp_attribute */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_declspec_attribute)
#define KWD___has_declspec_attribute TPP_KWD___has_declspec_attribute
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_declspec_attribute */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_feature)
#define KWD___has_feature TPP_KWD___has_feature
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_feature */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_extension)
#define KWD___has_extension TPP_KWD___has_extension
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_extension */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_warning)
#define KWD___has_warning TPP_KWD___has_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_known_extension)
#define KWD___has_known_extension TPP_KWD___has_known_extension
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_known_extension */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_known_warning)
#define KWD___has_known_warning TPP_KWD___has_known_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_known_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_include)
#define KWD___has_include TPP_KWD___has_include
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_include */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___has_include_next)
#define KWD___has_include_next TPP_KWD___has_include_next
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___has_include_next */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___VA_ARGS__)
#define KWD___VA_ARGS__ TPP_KWD___VA_ARGS__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___VA_ARGS__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___VA_COMMA__)
#define KWD___VA_COMMA__ TPP_KWD___VA_COMMA__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___VA_COMMA__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___VA_NARGS__)
#define KWD___VA_NARGS__ TPP_KWD___VA_NARGS__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___VA_NARGS__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___VA_OPT__)
#define KWD___VA_OPT__ TPP_KWD___VA_OPT__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___VA_OPT__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_EVAL)
#define KWD___TPP_EVAL TPP_KWD___TPP_EVAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_EVAL */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_LOAD_FILE)
#define KWD___TPP_LOAD_FILE TPP_KWD___TPP_LOAD_FILE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_LOAD_FILE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_COUNTER)
#define KWD___TPP_COUNTER TPP_KWD___TPP_COUNTER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_COUNTER */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_RANDOM)
#define KWD___TPP_RANDOM TPP_KWD___TPP_RANDOM
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_RANDOM */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_DECOMPILE)
#define KWD___TPP_STR_DECOMPILE TPP_KWD___TPP_STR_DECOMPILE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_DECOMPILE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_AT)
#define KWD___TPP_STR_AT TPP_KWD___TPP_STR_AT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_AT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_SUBSTR)
#define KWD___TPP_STR_SUBSTR TPP_KWD___TPP_STR_SUBSTR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_SUBSTR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_PACK)
#define KWD___TPP_STR_PACK TPP_KWD___TPP_STR_PACK
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_PACK */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_STR_SIZE)
#define KWD___TPP_STR_SIZE TPP_KWD___TPP_STR_SIZE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_STR_SIZE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_UNIQUE)
#define KWD___TPP_UNIQUE TPP_KWD___TPP_UNIQUE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_UNIQUE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_COUNT_TOKENS)
#define KWD___TPP_COUNT_TOKENS TPP_KWD___TPP_COUNT_TOKENS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_COUNT_TOKENS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TPP_IDENTIFIER)
#define KWD___TPP_IDENTIFIER TPP_KWD___TPP_IDENTIFIER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TPP_IDENTIFIER */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_DAY__)
#define KWD___DATE_DAY__ TPP_KWD___DATE_DAY__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_DAY__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_WDAY__)
#define KWD___DATE_WDAY__ TPP_KWD___DATE_WDAY__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_WDAY__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_YDAY__)
#define KWD___DATE_YDAY__ TPP_KWD___DATE_YDAY__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_YDAY__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_MONTH__)
#define KWD___DATE_MONTH__ TPP_KWD___DATE_MONTH__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_MONTH__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___DATE_YEAR__)
#define KWD___DATE_YEAR__ TPP_KWD___DATE_YEAR__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___DATE_YEAR__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIME_SEC__)
#define KWD___TIME_SEC__ TPP_KWD___TIME_SEC__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIME_SEC__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIME_MIN__)
#define KWD___TIME_MIN__ TPP_KWD___TIME_MIN__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIME_MIN__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD___TIME_HOUR__)
#define KWD___TIME_HOUR__ TPP_KWD___TIME_HOUR__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD___TIME_HOUR__ */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_dollar_is_alpha)
#define KWD_tpp_dollar_is_alpha TPP_KWD_tpp_dollar_is_alpha
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_dollar_is_alpha */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_va_args)
#define KWD_tpp_va_args TPP_KWD_tpp_va_args
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_va_args */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_named_va_args)
#define KWD_tpp_named_va_args TPP_KWD_tpp_named_va_args
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_named_va_args */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_va_comma)
#define KWD_tpp_va_comma TPP_KWD_tpp_va_comma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_va_comma */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_msvc_integer_suffix)
#define KWD_tpp_msvc_integer_suffix TPP_KWD_tpp_msvc_integer_suffix
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_msvc_integer_suffix */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_charize_operator)
#define KWD_tpp_charize_operator TPP_KWD_tpp_charize_operator
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_charize_operator */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_trigraphs)
#define KWD_tpp_trigraphs TPP_KWD_tpp_trigraphs
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_trigraphs */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_digraphs)
#define KWD_tpp_digraphs TPP_KWD_tpp_digraphs
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_digraphs */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_reemit_unknown_pragmas)
#define KWD_tpp_reemit_unknown_pragmas TPP_KWD_tpp_reemit_unknown_pragmas
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_reemit_unknown_pragmas */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_push_macro)
#define KWD_tpp_pragma_push_macro TPP_KWD_tpp_pragma_push_macro
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_push_macro */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_pop_macro)
#define KWD_tpp_pragma_pop_macro TPP_KWD_tpp_pragma_pop_macro
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_pop_macro */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_region)
#define KWD_tpp_pragma_region TPP_KWD_tpp_pragma_region
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_region */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_endregion)
#define KWD_tpp_pragma_endregion TPP_KWD_tpp_pragma_endregion
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_endregion */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_warning)
#define KWD_tpp_pragma_warning TPP_KWD_tpp_pragma_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_message)
#define KWD_tpp_pragma_message TPP_KWD_tpp_pragma_message
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_message */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_error)
#define KWD_tpp_pragma_error TPP_KWD_tpp_pragma_error
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_error */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_once)
#define KWD_tpp_pragma_once TPP_KWD_tpp_pragma_once
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_once */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_tpp_exec)
#define KWD_tpp_pragma_tpp_exec TPP_KWD_tpp_pragma_tpp_exec
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_tpp_exec */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_deprecated)
#define KWD_tpp_pragma_deprecated TPP_KWD_tpp_pragma_deprecated
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_deprecated */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_pragma_tpp_set_keyword_flags)
#define KWD_tpp_pragma_tpp_set_keyword_flags TPP_KWD_tpp_pragma_tpp_set_keyword_flags
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_pragma_tpp_set_keyword_flags */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_directive_include_next)
#define KWD_tpp_directive_include_next TPP_KWD_tpp_directive_include_next
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_directive_include_next */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_directive_import)
#define KWD_tpp_directive_import TPP_KWD_tpp_directive_import
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_directive_import */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_directive_warning)
#define KWD_tpp_directive_warning TPP_KWD_tpp_directive_warning
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_directive_warning */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_lxor)
#define KWD_tpp_lxor TPP_KWD_tpp_lxor
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_lxor */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_tilde_tilde)
#define KWD_tpp_token_tilde_tilde TPP_KWD_tpp_token_tilde_tilde
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_tilde_tilde */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_pow)
#define KWD_tpp_token_pow TPP_KWD_tpp_token_pow
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_pow */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_lxor)
#define KWD_tpp_token_lxor TPP_KWD_tpp_token_lxor
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_lxor */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_arrow)
#define KWD_tpp_token_arrow TPP_KWD_tpp_token_arrow
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_arrow */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_colon_assign)
#define KWD_tpp_token_colon_assign TPP_KWD_tpp_token_colon_assign
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_colon_assign */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_token_colon_colon)
#define KWD_tpp_token_colon_colon TPP_KWD_tpp_token_colon_colon
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_token_colon_colon */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_macro_calling_conventions)
#define KWD_tpp_macro_calling_conventions TPP_KWD_tpp_macro_calling_conventions
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_macro_calling_conventions */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_strict_whitespace)
#define KWD_tpp_strict_whitespace TPP_KWD_tpp_strict_whitespace
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_strict_whitespace */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_strict_integer_overflow)
#define KWD_tpp_strict_integer_overflow TPP_KWD_tpp_strict_integer_overflow
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_strict_integer_overflow */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_support_ansi_characters)
#define KWD_tpp_support_ansi_characters TPP_KWD_tpp_support_ansi_characters
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_support_ansi_characters */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_emit_lf_after_directive)
#define KWD_tpp_emit_lf_after_directive TPP_KWD_tpp_emit_lf_after_directive
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_emit_lf_after_directive */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_if_cond_expression)
#define KWD_tpp_if_cond_expression TPP_KWD_tpp_if_cond_expression
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_if_cond_expression */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_KWD_tpp_debug)
#define KWD_tpp_debug TPP_KWD_tpp_debug
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_KWD_tpp_debug */
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define KWD___TPP_VERSION__ TPP_KWD___TPP_VERSION__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_EXT_TRIGRAPHS)
#define EXT_TRIGRAPHS TPP_EXT_TRIGRAPHS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_EXT_TRIGRAPHS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_EXT_DIGRAPHS)
#define EXT_DIGRAPHS TPP_EXT_DIGRAPHS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_EXT_DIGRAPHS */
#ifdef TPP_EXT_NAMED_VARARGS_IN_MACROS
#define TPP_EXT_GCC_VA_ARGS TPP_EXT_NAMED_VARARGS_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_GCC_VA_ARGS TPP_EXT_NAMED_VARARGS_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_NAMED_VARARGS_IN_MACROS */
#ifdef TPP_EXT_VA_GLUE_COMMA_IN_MACROS
#define TPP_EXT_GCC_VA_COMMA TPP_EXT_VA_GLUE_COMMA_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_GCC_VA_COMMA TPP_EXT_VA_GLUE_COMMA_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_GLUE_COMMA_IN_MACROS */
#ifdef TPP_EXT_VA_COMMA_IN_MACROS
#define TPP_EXT_VA_COMMA TPP_EXT_VA_COMMA_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_VA_COMMA TPP_EXT_VA_COMMA_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_COMMA_IN_MACROS */
#ifdef TPP_EXT_VA_OPT_IN_MACROS
#define TPP_EXT_VA_OPT TPP_EXT_VA_OPT_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_VA_OPT TPP_EXT_VA_OPT_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_OPT_IN_MACROS */
#ifdef TPP_EXT_VA_NARGS_IN_MACROS
#define TPP_EXT_VA_NARGS TPP_EXT_VA_NARGS_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_VA_NARGS TPP_EXT_VA_NARGS_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_NARGS_IN_MACROS */
#ifdef TPP_EXT_VA_ARGS_IN_MACROS
#define TPP_EXT_VA_ARGS TPP_EXT_VA_ARGS_IN_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_VA_ARGS TPP_EXT_VA_ARGS_IN_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_VA_ARGS_IN_MACROS */
#ifdef TPP_EXT_ESCAPE_E_IN_STRINGS
#define TPP_EXT_STR_E TPP_EXT_ESCAPE_E_IN_STRINGS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_STR_E TPP_EXT_ESCAPE_E_IN_STRINGS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_ESCAPE_E_IN_STRINGS */
#ifdef TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_EXT_ALTMAC TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_ALTMAC TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS */
#ifdef TPP_EXT_MACRO_RECURSION
#define TPP_EXT_RECMAC TPP_EXT_MACRO_RECURSION
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_RECMAC TPP_EXT_MACRO_RECURSION
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_RECURSION */
#ifdef TPP_EXT_MACRO_ARGUMENT_WHITESPACE
#define TPP_EXT_ARGSPACE TPP_EXT_MACRO_ARGUMENT_WHITESPACE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_ARGSPACE TPP_EXT_MACRO_ARGUMENT_WHITESPACE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_ARGUMENT_WHITESPACE */
#ifdef TPP_EXT_MACRO___pragma
#define TPP_EXT_MSVC_PRAGMA TPP_EXT_MACRO___pragma
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_MSVC_PRAGMA TPP_EXT_MACRO___pragma
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___pragma */
#ifdef TPP_EXT_CHARIZE_MACRO_ARGUMENT
#define TPP_EXT_HASH_AT TPP_EXT_CHARIZE_MACRO_ARGUMENT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_HASH_AT TPP_EXT_CHARIZE_MACRO_ARGUMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CHARIZE_MACRO_ARGUMENT */
#ifdef TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT
#define TPP_EXT_HASH_XCLAIM TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_HASH_XCLAIM TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT */
#ifdef TPP_EXT_CPP_WARNING
#define TPP_EXT_WARNING TPP_EXT_CPP_WARNING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_WARNING TPP_EXT_CPP_WARNING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_WARNING */
#ifdef TPP_EXT_CPP_EXCLAIM
#define TPP_EXT_SHEBANG TPP_EXT_CPP_EXCLAIM
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_SHEBANG TPP_EXT_CPP_EXCLAIM
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_EXCLAIM */
#ifdef TPP_EXT_CPP_INCLUDE_NEXT
#define TPP_EXT_INCLUDE_NEXT TPP_EXT_CPP_INCLUDE_NEXT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_INCLUDE_NEXT TPP_EXT_CPP_INCLUDE_NEXT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_INCLUDE_NEXT */
#ifdef TPP_EXT_CPP_IMPORT
#define TPP_EXT_IMPORT TPP_EXT_CPP_IMPORT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_IMPORT TPP_EXT_CPP_IMPORT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_IMPORT */
#ifdef TPP_EXT_CPP_IDENT_SCCS
#define TPP_EXT_IDENT_SCCS TPP_EXT_CPP_IDENT_SCCS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_IDENT_SCCS TPP_EXT_CPP_IDENT_SCCS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_IDENT_SCCS */
#ifdef TPP_EXT_MACRO___BASE_FILE__
#define TPP_EXT_BASEFILE TPP_EXT_MACRO___BASE_FILE__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_BASEFILE TPP_EXT_MACRO___BASE_FILE__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___BASE_FILE__ */
#ifdef TPP_EXT_MACRO___INCLUDE_LEVEL__
#define TPP_EXT_INCLUDE_LEVEL TPP_EXT_MACRO___INCLUDE_LEVEL__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_INCLUDE_LEVEL TPP_EXT_MACRO___INCLUDE_LEVEL__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___INCLUDE_LEVEL__ */
#ifdef TPP_EXT_MACRO___COUNTER__
#define TPP_EXT_COUNTER TPP_EXT_MACRO___COUNTER__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_COUNTER TPP_EXT_MACRO___COUNTER__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___COUNTER__ */
#ifdef TPP_EXT_MACRO___TIMESTAMP__
#define TPP_EXT_TIMESTAMP TPP_EXT_MACRO___TIMESTAMP__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TIMESTAMP TPP_EXT_MACRO___TIMESTAMP__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___TIMESTAMP__ */
#ifdef TPP_EXT_MACRO___COLUMN__
#define TPP_EXT_COLUMN TPP_EXT_MACRO___COLUMN__
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_COLUMN TPP_EXT_MACRO___COLUMN__
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO___COLUMN__ */
#ifdef TPP_EXT_NUMERIC_DATE_MACROS
#define TPP_EXT_DATEUTILS TPP_EXT_NUMERIC_DATE_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_DATEUTILS TPP_EXT_NUMERIC_DATE_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_NUMERIC_DATE_MACROS */
#ifdef TPP_EXT_NUMERIC_TIME_MACROS
#define TPP_EXT_TIMEUTILS TPP_EXT_NUMERIC_TIME_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TIMEUTILS TPP_EXT_NUMERIC_TIME_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_NUMERIC_TIME_MACROS */
#ifdef TPP_EXT_MACRO_TPP_EVAL
#define TPP_EXT_TPP_EVAL TPP_EXT_MACRO_TPP_EVAL
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_EVAL TPP_EXT_MACRO_TPP_EVAL
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_EVAL */
#ifdef TPP_EXT_MACRO_TPP_UNIQUE
#define TPP_EXT_TPP_UNIQUE TPP_EXT_MACRO_TPP_UNIQUE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_UNIQUE TPP_EXT_MACRO_TPP_UNIQUE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_UNIQUE */
#ifdef TPP_EXT_MACRO_TPP_LOAD_FILE
#define TPP_EXT_TPP_LOAD_FILE TPP_EXT_MACRO_TPP_LOAD_FILE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_LOAD_FILE TPP_EXT_MACRO_TPP_LOAD_FILE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_LOAD_FILE */
#ifdef TPP_EXT_MACRO_TPP_COUNTER
#define TPP_EXT_TPP_COUNTER TPP_EXT_MACRO_TPP_COUNTER
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_COUNTER TPP_EXT_MACRO_TPP_COUNTER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_COUNTER */
#ifdef TPP_EXT_MACRO_TPP_RANDOM
#define TPP_EXT_TPP_RANDOM TPP_EXT_MACRO_TPP_RANDOM
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_RANDOM TPP_EXT_MACRO_TPP_RANDOM
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_RANDOM */
#ifdef TPP_EXT_MACRO_TPP_STR_DECOMPILE
#define TPP_EXT_TPP_STR_DECOMPILE TPP_EXT_MACRO_TPP_STR_DECOMPILE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_STR_DECOMPILE TPP_EXT_MACRO_TPP_STR_DECOMPILE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_STR_DECOMPILE */
#ifdef TPP_EXT_MACRO_TPP_STR_SUBSTR
#define TPP_EXT_TPP_STR_SUBSTR TPP_EXT_MACRO_TPP_STR_SUBSTR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_STR_SUBSTR TPP_EXT_MACRO_TPP_STR_SUBSTR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_STR_SUBSTR */
#ifdef TPP_EXT_MACRO_TPP_STR_PACK
#define TPP_EXT_TPP_STR_PACK TPP_EXT_MACRO_TPP_STR_PACK
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_STR_PACK TPP_EXT_MACRO_TPP_STR_PACK
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_STR_PACK */
#ifdef TPP_EXT_MACRO_TPP_STR_SIZE
#define TPP_EXT_TPP_STR_SIZE TPP_EXT_MACRO_TPP_STR_SIZE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_STR_SIZE TPP_EXT_MACRO_TPP_STR_SIZE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_STR_SIZE */
#ifdef TPP_EXT_MACRO_TPP_COUNT_TOKENS
#define TPP_EXT_TPP_COUNT_TOKENS TPP_EXT_MACRO_TPP_COUNT_TOKENS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_COUNT_TOKENS TPP_EXT_MACRO_TPP_COUNT_TOKENS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_COUNT_TOKENS */
#ifdef TPP_EXT_MACRO_TPP_IDENTIFIER
#define TPP_EXT_TPP_IDENTIFIER TPP_EXT_MACRO_TPP_IDENTIFIER
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TPP_IDENTIFIER TPP_EXT_MACRO_TPP_IDENTIFIER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_MACRO_TPP_IDENTIFIER */
#ifdef TPP_EXT_CPP_ASSERT
#define TPP_EXT_ASSERTIONS TPP_EXT_CPP_ASSERT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_ASSERTIONS TPP_EXT_CPP_ASSERT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CPP_ASSERT */
#ifdef TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR
#define TPP_EXT_NO_EXPAND_DEFINED TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_NO_EXPAND_DEFINED TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR */
#ifdef TPP_EXT_TRADITIONAL_MACROS
#define TPP_EXT_TRADITIONAL_MACRO TPP_EXT_TRADITIONAL_MACROS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_TRADITIONAL_MACRO TPP_EXT_TRADITIONAL_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_TRADITIONAL_MACROS */
#ifdef TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES
#define TPP_EXT_EXT_ARE_FEATURES TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_EXT_ARE_FEATURES TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES */
#ifdef TPP_EXT_BUILTIN_EXPR_STRINGS
#define TPP_EXT_STRINGOPS TPP_EXT_BUILTIN_EXPR_STRINGS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_STRINGOPS TPP_EXT_BUILTIN_EXPR_STRINGS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_BUILTIN_EXPR_STRINGS */
#ifdef TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
#define TPP_EXT_GCC_IFELSE TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_GCC_IFELSE TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT */
#ifdef TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#define TPP_EXT_IFELSE_IN_EXPR TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define EXT_IFELSE_IN_EXPR TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_COMMENT)
#define WG_COMMENT TPP_WG_COMMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_COMMENT */
#ifdef TPP_WG_COMMENT
#define TPP_WG_COMMENTS TPP_WG_COMMENT
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define WG_COMMENTS TPP_WG_COMMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_WG_COMMENT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_MACROS)
#define WG_MACROS TPP_WG_MACROS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_MACROS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_SYNTAX)
#define WG_SYNTAX TPP_WG_SYNTAX
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_SYNTAX */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_USAGE)
#define WG_USAGE TPP_WG_USAGE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_USAGE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_VALUE)
#define WG_VALUE TPP_WG_VALUE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_VALUE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_BOOLVALUE)
#define WG_BOOLVALUE TPP_WG_BOOLVALUE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_BOOLVALUE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_USER)
#define WG_USER TPP_WG_USER
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_USER */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_ENVIRON)
#define WG_ENVIRON TPP_WG_ENVIRON
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_ENVIRON */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_LIMIT)
#define WG_LIMIT TPP_WG_LIMIT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_LIMIT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_UNDEF)
#define WG_UNDEF TPP_WG_UNDEF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_UNDEF */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_TRIGRAPHS)
#define WG_TRIGRAPHS TPP_WG_TRIGRAPHS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_TRIGRAPHS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_EXPANSION_TO_DEFINED)
#define WG_EXPANSION_TO_DEFINED TPP_WG_EXPANSION_TO_DEFINED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_EXPANSION_TO_DEFINED */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_QUALITY)
#define WG_QUALITY TPP_WG_QUALITY
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_QUALITY */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_DEPRECATED)
#define WG_DEPRECATED TPP_WG_DEPRECATED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_DEPRECATED */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_WG_DEPENDENCY)
#define WG_DEPENDENCY TPP_WG_DEPENDENCY
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_WG_DEPENDENCY */
#ifdef TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#define TPP_W_EXPECTED_KEYWORD_AFTER_DEFINE TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_KEYWORD_AFTER_DEFINE TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
#ifdef TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#define TPP_W_EXPECTED_KEYWORD_AFTER_UNDEF TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_KEYWORD_AFTER_UNDEF TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
#ifdef TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#define TPP_W_EXPECTED_KEYWORD_AFTER_IFDEF TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_KEYWORD_AFTER_IFDEF TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
#ifdef TPP_W_UNKNOWN_DIRECTIVE
#define TPP_W_UNKNOWN_PREPROCESSOR_DIRECTIVE TPP_W_UNKNOWN_DIRECTIVE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_UNKNOWN_PREPROCESSOR_DIRECTIVE TPP_W_UNKNOWN_DIRECTIVE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNKNOWN_DIRECTIVE */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_ERROR)
#define W_ERROR TPP_W_ERROR
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_ERROR */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_WARNING)
#define W_WARNING TPP_W_WARNING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_WARNING */
#ifdef TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#define TPP_W_EXPECTED_MACRO_ARGUMENT_NAME TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_MACRO_ARGUMENT_NAME TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
#ifdef TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#define TPP_W_EXPECTED_ARGEND_AFTER_VARARGS TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_ARGEND_AFTER_VARARGS TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
#ifdef TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#define TPP_W_EXPECTED_COMMA_OR_ARGEND TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_COMMA_OR_ARGEND TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
#ifdef TPP_W_TOO_MANY_ARGUMENTS
#define TPP_W_TOO_MANY_MACRO_ARGUMENTS TPP_W_TOO_MANY_ARGUMENTS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_TOO_MANY_MACRO_ARGUMENTS TPP_W_TOO_MANY_ARGUMENTS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_TOO_MANY_ARGUMENTS */
#ifdef TPP_W_EOF_IN_ARGUMENT_LIST
#define TPP_W_EOF_IN_MACRO_ARGUMENT_LIST TPP_W_EOF_IN_ARGUMENT_LIST
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EOF_IN_MACRO_ARGUMENT_LIST TPP_W_EOF_IN_ARGUMENT_LIST
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EOF_IN_ARGUMENT_LIST */
#ifdef TPP_W_REDEFINE_MACRO
#define TPP_W_REDEFINING_MACRO TPP_W_REDEFINE_MACRO
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_REDEFINING_MACRO TPP_W_REDEFINE_MACRO
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_REDEFINE_MACRO */
#ifdef TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
#define TPP_W_CANT_UNDEF_BUILTIN_MACRO TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_CANT_UNDEF_BUILTIN_MACRO TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_CANNOT_UNDEF_BUILTIN_MACRO */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_PUSHMACRO TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_PUSHMACRO TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_STRD TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_STRD TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_LPAREN TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_LPAREN TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_UNEXPECTED_TOKEN
#define TPP_W_EXPECTED_RPAREN TPP_W_UNEXPECTED_TOKEN
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_RPAREN TPP_W_UNEXPECTED_TOKEN
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_UNEXPECTED_TOKEN */
#ifdef TPP_W_DEPRECATED_KEYWORD
#define TPP_W_DEPRECATED_IDENTIFIER TPP_W_DEPRECATED_KEYWORD
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_DEPRECATED_IDENTIFIER TPP_W_DEPRECATED_KEYWORD
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_DEPRECATED_KEYWORD */
#ifdef TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
#define TPP_W_ARGUMENT_NAMED_ALREADY_TAKEN TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ARGUMENT_NAMED_ALREADY_TAKEN TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_DUPLICATE_MACRO_PARAMETER_NAME */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_SLASHSTAR_INSIDE_OF_COMMENT)
#define W_SLASHSTAR_INSIDE_OF_COMMENT TPP_W_SLASHSTAR_INSIDE_OF_COMMENT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_SLASHSTAR_INSIDE_OF_COMMENT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_LINE_COMMENT_CONTINUED)
#define W_LINE_COMMENT_CONTINUED TPP_W_LINE_COMMENT_CONTINUED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_LINE_COMMENT_CONTINUED */
#ifdef TPP_W_TOO_FEW_ARGUMENTS
#define TPP_W_NOT_ENGOUH_MACRO_ARGUMENTS TPP_W_TOO_FEW_ARGUMENTS
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_NOT_ENGOUH_MACRO_ARGUMENTS TPP_W_TOO_FEW_ARGUMENTS
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_TOO_FEW_ARGUMENTS */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_STRING_TERMINATED_BY_LINEFEED)
#define W_STRING_TERMINATED_BY_LINEFEED TPP_W_STRING_TERMINATED_BY_LINEFEED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_STRING_TERMINATED_BY_LINEFEED */
#ifdef TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#define TPP_W_STRING_TERMINATED_BY_EOF TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_STRING_TERMINATED_BY_EOF TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_COMMENT_TERMINATED_BY_EOF)
#define W_COMMENT_TERMINATED_BY_EOF TPP_W_COMMENT_TERMINATED_BY_EOF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_COMMENT_TERMINATED_BY_EOF */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_ENCOUNTERED_TRIGRAPH)
#define W_ENCOUNTERED_TRIGRAPH TPP_W_ENCOUNTERED_TRIGRAPH
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_ENCOUNTERED_TRIGRAPH */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_IN_EXPRESSION TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_IN_EXPRESSION TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_MESSAGE TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_MESSAGE TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_DEPRECATED TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_DEPRECATED TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_EXEC TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_EXEC TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_SETF TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_SETF TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_STRAT TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_STRAT TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_CNTTOK TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_CNTTOK TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_PRGERROR TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_PRGERROR TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_EXTENSION TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_EXTENSION TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_TPP_INCPTH TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_TPP_INCPTH TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPANSION_TO_DEFINED
#define TPP_W_DEFINED_IN_MACRO_BODY TPP_W_EXPANSION_TO_DEFINED
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_DEFINED_IN_MACRO_BODY TPP_W_EXPANSION_TO_DEFINED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPANSION_TO_DEFINED */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_IDENT TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_IDENT TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#ifdef TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING_AFTER_GCC_DIAG TPP_W_EXPECTED_STRING
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_EXPECTED_STRING_AFTER_GCC_DIAG TPP_W_EXPECTED_STRING
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EXPECTED_STRING */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT)
#define W_EXPECTED_LPAREN_AFTER_VA_OPT TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT)
#define W_EXPECTED_RPAREN_AFTER_VA_OPT TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT */
#ifdef TPP_W_EOF_BEFORE_ENDIF
#define TPP_W_IF_WITHOUT_ENDIF TPP_W_EOF_BEFORE_ENDIF
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_IF_WITHOUT_ENDIF TPP_W_EOF_BEFORE_ENDIF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_EOF_BEFORE_ENDIF */
#if TPP2_HAVE_GLOBAL_NAMESPACE && defined(TPP_W_ENDIF_WITHOUT_IF)
#define W_ENDIF_WITHOUT_IF TPP_W_ENDIF_WITHOUT_IF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE && TPP_W_ENDIF_WITHOUT_IF */
#ifdef TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#define TPP_W_ELSE_WITHOUT_IF TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ELSE_WITHOUT_IF TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_ELIF_OR_ELSE_WITHOUT_IF */
#ifdef TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#define TPP_W_ELSE_AFTER_ELSE TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ELSE_AFTER_ELSE TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
#ifdef TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#define TPP_W_ELIF_WITHOUT_IF TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ELIF_WITHOUT_IF TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_ELIF_OR_ELSE_WITHOUT_IF */
#ifdef TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#define TPP_W_ELIF_AFTER_ELSE TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define W_ELIF_AFTER_ELSE TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#endif /* TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
/*[[[end]]]*/


//TODO:/* 6*/ DEF_WARNING(W_EXPECTED_KWDLPAR_AFTER_DEFINED, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword or " Q("(") " after " Q("defined") ", but got " TOK_S, TOK_A)) /* OLD(TPPWarn_ExpectedKeywordOrLParenAfterDefined). */
//TODO:/* 7*/ DEF_WARNING(W_EXPECTED_KEYWORD_AFTER_DEFINED, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword after " Q("defined") ", but got " TOK_S, TOK_A))               /* OLD(TPPWarn_ExpectedKeywordAfterDefined). */
//TODO:/* 8*/ DEF_WARNING(W_EXPECTED_RPAREN_AFTER_DEFINED, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q(")") " after " Q("defined") ", but got " TOK_S, TOK_A))             /* OLD(TPPWarn_ExpectedRParenAfterDefined). */
//TODO:/*14*/ DEF_WARNING(W_EXPECTED_INCLUDE_STRING, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected #include-string, but got " TOK_S, TOK_A))                                     /* OLD(TPPWarn_ExpectedIncludeString). */
//TODO:/*15*/ DEF_WARNING(W_FILE_NOT_FOUND, (WG_ENVIRON), WSTATE_ERROR, { char *temp = ARG(char *); WARNF("File not found: " Q("%.*s"), (int)ARG(size_t), temp); })           /* [char const *,size_t] OLD(TPPWarn_IncludeFileNotFound). */
//TODO:/*35*/ DEF_WARNING(W_EXPECTED_STRING_AFTER_LINE, (WG_SYNTAX, WG_VALUE), WSTATE_ERROR, WARNF("Expected string after #line, but got " Q("%s"), CONST_STR())) /* [struct TPPConst *] OLD(TPPWarn_ExpectedStringAfterLine). */
//TODO:/*36*/ DEF_WARNING(W_MACRO_NOT_DEFINED, (WG_MACROS), WSTATE_DISABLED, WARNF("Macro " Q("%s") " is not defined", KWDNAME()))                                /* [struct TPPKeyword *] OLD(TPPWarn_MacroDoesntExist). */
//TODO:/*40*/ DEF_WARNING(W_EXPECTED_COLON_AFTER_QUESTION, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q(":") " after " Q("?")))                                /* OLD(TPPWarn_ExpectedColonAfterQuestion). */
//TODO:#define W_EXPECTED_COLLON_AFTER_QUESTION W_EXPECTED_COLON_AFTER_QUESTION
//TODO:/*41*/ DEF_WARNING(W_INVALID_INTEGER, (WG_SYNTAX), WSTATE_ERROR, WARNF("Invalid integer " TOK_S, TOK_A)) /* OLD(TPPWarn_ExpectedInteger). */
//TODO:/*42*/ DEF_WARNING(W_EXPECTED_RPAREN_IN_EXPRESSION, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q(")") " in preprocessor expression, but got " TOK_S, TOK_A))                  /* OLD(TPPWarn_ExpectedRparenAfterLParen). */
//TODO (this is actually -Wundef):/*43*/ DEF_WARNING(W_UNKNOWN_TOKEN_IN_EXPR_IS_ZERO, (WG_UNDEF, WG_SYNTAX), WSTATE_WARN, WARNF("Unrecognized token " TOK_S " is replaced with " Q("0") " in expression", TOK_A)) /* OLD(TPPWarn_UnexpectedTokenInConstExpr). */
//TODO:/*47*/ DEF_WARNING(W_EXPECTED_STRING_AFTER_PRAGMA, (WG_SYNTAX, WG_VALUE), WSTATE_ERROR, WARNF("Expected string after _Pragma, but got " Q("%s"), CONST_STR()))                  /* [struct TPPConst *] OLD(TPPWarn_ExpectedStringAfterPragma). */
//TODO:/*50*/ DEF_WARNING(W_INVALID_WARNING, (WG_VALUE), WSTATE_ERROR, {
//TODO:	/* [struct TPPConst *] OLD(TPPWarn_InvalidWarning). */
//TODO:	struct TPPConst *c = ARG(struct TPPConst *);
//TODO:	if (c->c_kind == TPP_CONST_STRING) {
//TODO:		char const *wname = c->c_data.c_string->s_text;
//TODO:		if (*wname == '-')
//TODO:			++wname;
//TODO:		if (*wname == 'W')
//TODO:			++wname;
//TODO:		if (!memcmp(wname, "no-", 3))
//TODO:			wname += 3;
//TODO:		WARNF("Invalid warning " Q("%s") " (Did you mean " Q("%s") ")", wname, find_most_likely_warning(wname));
//TODO:	} else if (c->c_kind == TPP_CONST_FLOAT) {
//TODO:		WARNF("Invalid warning " Q("%f"), (double)c->c_data.c_float);
//TODO:	} else if (c->c_kind == TPP_CONST_INTEGRAL) {
//TODO:		WARNF("Invalid warning " Q("%ld"), (long)c->c_data.c_int);
//TODO:	} else {
//TODO:		WARNF("Invalid warning");
//TODO:	}
//TODO:})
//TODO:/*51*/ DEF_WARNING(W_CANT_POP_WARNINGS, (WG_VALUE), WSTATE_ERROR, WARNF("Can't pop warnings"))                                                                       /* OLD(TPPWarn_CantPopWarnings). */
//TODO:/*61*/ DEF_WARNING(W_NONPARTABLE_FILENAME_CASING, (WG_ENVIRON), WSTATE_WARN, {
//TODO:	/* [char const *,char const *,size_t,char const *]. (path,wrong_begin,wrong_size,corrent_begin)
//TODO:	 *  OLD(TPPWarn_InvalidPathCasing). */
//TODO:	char *temp;
//TODO:	char *temp2;
//TODO:	size_t temp3;
//TODO:	temp  = ARG(char *);
//TODO:	temp2 = ARG(char *);
//TODO:	temp3 = ARG(size_t);
//TODO:	WARNF("Non-portable casing in " Q("%s") ": " Q("%.*s") " should be " Q("%s") " instead",
//TODO:	      temp, (int)temp3, temp2, ARG(char *));
//TODO:})
//TODO:/*62*/ DEF_WARNING(W_DIVIDE_BY_ZERO, (WG_VALUE), WSTATE_ERROR, WARNF("Divide by ZERO")) /* OLD(TPPWarn_DivideByZero|TPPWarn_ModuloByZero). */
//TODO:/*64*/ DEF_WARNING(W_SPECIAL_ARGUMENT_NAME, (WG_MACROS), WSTATE_WARN, WARNF("Special keyword " Q("%s") " used as argument name", KWDNAME())) /* [struct TPPKeyword *] OLD(TPPWarn_VaArgsUsedAsMacroParameter). */
//TODO:/*68*/ WARNING(W_EXPECTED_BOOL, (WG_BOOLVALUE, WG_VALUE), WSTATE_DISABLED)            /* [struct TPPConst *] OLD(TPPWarn_ExpectedBoolExpression). */
//TODO:/*69*/ WARNING(W_EXPECTED_BOOL_UNARY, (WG_BOOLVALUE, WG_VALUE), WSTATE_DISABLED)      /* [struct TPPConst *] OLD(TPPWarn_ExpectedBoolExpressionNot). */
//TODO:/*70*/ WARNING(W_EXPECTED_BOOL_BINARY_LHS, (WG_BOOLVALUE, WG_VALUE), WSTATE_DISABLED) /* [struct TPPConst *] OLD(TPPWarn_ExpectedBoolExpressionLhsOP). */
//TODO:/*71*/ WARNING(W_EXPECTED_BOOL_BINARY_RHS, (WG_BOOLVALUE, WG_VALUE), WSTATE_DISABLED) /* [struct TPPConst *] OLD(TPPWarn_ExpectedBoolExpressionRhsOP). */

/* Warnings added by the new TPP2. */
//TODO:DEF_WARNING(W_VA_KEYWORD_IN_REGULAR_MACRO, (WG_MACROS), WSTATE_WARN, WARNF("Variadic keyword " Q("%s") " used in regular macro", KWDNAME()))                    /* [struct TPPKeyword *]. */
//TODO:DEF_WARNING(W_KEYWORD_MACRO_ALREADY_ONSTACK, (WG_MACROS), WSTATE_DISABLED, WARNF("Keyword-style macro " Q("%s") " is already being expanded", FILENAME()))      /* [struct TPPFile *]. */
//TODO:DEF_WARNING(W_FUNCTION_MACRO_ALREADY_ONSTACK, (WG_MACROS), WSTATE_DISABLED, WARNF("Function-style macro " Q("%s") " is expanded to the same text", FILENAME())) /* [struct TPPFile *]. */
//TODO:DEF_WARNING(W_CHARACTER_TOO_LONG, (WG_VALUE), WSTATE_ERROR, WARNF("Character sequence is too long"))                                                            /* . */
//TODO:DEF_WARNING(W_MULTICHAR_NOT_ALLOWED, (WG_VALUE), WSTATE_ERROR, { char *temp = ARG(char *); WARNF("The multi-character sequence " Q("%.*s") " is not not allowed", (int)ARG(size_t), temp); })                                                                                             /* [char const *,size_t]. */
//TODO:DEF_WARNING(W_INDEX_OUT_OF_BOUNDS, (WG_VALUE), WSTATE_DISABLED, { struct TPPString *s = ARG(struct TPPString *); WARNF("Index %ld is out-of-bounds of 0..%lu", (unsigned long)s->s_size, (unsigned long)ARG(ptrdiff_t)); })                                                                                            /* [struct TPPString *,ptrdiff_t]. */
//TODO:DEF_WARNING(W_EXPECTED_RBRACKET_IN_EXPRESSION, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q("]") " in expression, but got " TOK_S, TOK_A))                          /* . */
//TODO:DEF_WARNING(W_EXPECTED_COLON_AFTER_WARNING, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q(":") " after #pragma warning, but got " TOK_S, TOK_A))                    /* . */
//TODO:#define W_EXPECTED_COLLON_AFTER_WARNING W_EXPECTED_COLON_AFTER_WARNING
//TODO:DEF_WARNING(W_EXPECTED_COMMA, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q(",") ", but got " TOK_S, TOK_A))                                                         /* . */
//TODO:DEF_WARNING(W_MACRO_RECURSION_LIMIT_EXCEEDED, (WG_LIMIT), WSTATE_ERROR, WARNF("Macro recursion limit exceeded when expanding " Q("%s") " (Consider passing " Q("-fno-macro-recursion") ")", FILENAME())) /* [struct TPPFile *]. */
//TODO:DEF_WARNING(W_INCLUDE_RECURSION_LIMIT_EXCEEDED, (WG_LIMIT), WSTATE_ERROR, WARNF("Include recursion limit exceeded when including " Q("%s"), FILENAME()))                                                 /* [struct TPPFile *]. */
//TODO:DEF_WARNING(W_UNKNOWN_EXTENSION, (WG_VALUE), WSTATE_ERROR, { char *temp = ARG(char *); WARNF("Unknown extension " Q("%s") " (Did you mean " Q("%s") "?)", temp,find_most_likely_extension(temp)); })                                                                                                                                          /* [char const *]. */
//TODO:DEF_WARNING(W_IDENT_SCCS_IGNORED, (WG_USAGE), WSTATE_WARN, WARNF("#ident/sccs with " Q("%s") " is ignored", CONST_STR())) /* [struct TPPConst *]. */
//TODO:DEF_WARNING(W_EXPECTED_KEYWORD_AFTER_ASSERT, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword after #assert, but got " TOK_S, TOK_A))                                        /* . */
//TODO:DEF_WARNING(W_EXPECTED_KEYWORD_AFTER_PREDICATE, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword after predicate " Q("%s") " in #assert, but got " TOK_S, KWDNAME(), TOK_A)) /* [struct TPPKeyword *]. */
//TODO:DEF_WARNING(W_EXPECTED_KEYWORD_AFTER_EXPR_HASH, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword after # in expression, but got " TOK_S, TOK_A)) /* . */
//TODO:DEF_WARNING(W_EXPECTED_KEYWORD_AFTER_EXPR_PRED, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected keyword after predicate " Q("%s") " in expression, but got " TOK_S, KWDNAME(), TOK_A)) /* . */
//TODO:DEF_WARNING(W_UNKNOWN_ASSERTION, (WG_VALUE), WSTATE_DISABLED, { char const *temp = KWDNAME(); WARNF("Assertion " Q("%s") " does not contain a predicate " Q("%s"), temp, KWDNAME()); }) /* [struct TPPKeyword *,struct TPPKeyword *]. */
//TODO:DEF_WARNING(W_CANT_POP_EXTENSIONS, (WG_VALUE), WSTATE_ERROR, WARNF("Can't pop extensions"))                                                                                     /* . */
//TODO:DEF_WARNING(W_CANT_POP_INCLUDE_PATH, (WG_VALUE), WSTATE_ERROR, WARNF("Can't pop #include paths"))                                                                               /* . */
//TODO:DEF_WARNING(W_CONSIDER_PAREN_AROUND_LAND, (WG_QUALITY), WSTATE_WARN, WARNF("Consider adding parenthesis around " Q("&&") " to prevent confusion with " Q("||")))                /* . */
//TODO:DEF_WARNING(W_INTEGRAL_OVERFLOW, (WG_VALUE), WSTATE_WARN, { tint_t old = ARG(tint_t); WARNF("Integral constant overflow from " Q("%lld") " to " Q("%lld"), (long long)old, (long long)ARG(tint_t)); }) /* [tint_t,tint_t]. */
//TODO:DEF_WARNING(W_INTEGRAL_CLAMPED, (WG_VALUE), WSTATE_WARN, WARNF("Integral constant clamped to fit")) /* [tint_t,tint_t]. */
//TODO:DEF_WARNING(W_UNKNOWN_INCLUDE_PATH, (WG_VALUE), WSTATE_WARN, { char *temp = ARG(char *); WARNF("Unknown system #include-path " Q("%.*s"), (int)ARG(size_t), temp); })                                   /* [char const *,size_t]. */
//TODO:DEF_WARNING(W_INCLUDE_PATH_ALREADY_EXISTS, (WG_VALUE), WSTATE_WARN, { char *temp = ARG(char *); WARNF("System #include-path " Q("%.*s") " already exists", (int)ARG(size_t), temp); })                            /* [char const *,size_t]. */
//TODO:DEF_WARNING(W_EXPECTED_ELSE_IN_EXPRESSION, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q("else") " in expression, but got " TOK_S, TOK_A)) /* . */
//TODO:DEF_WARNING(W_STATEMENT_IN_EXPRESSION, (WG_USAGE, WG_SYNTAX), WSTATE_WARN, WARNF("GCC-style statement " TOK_S " in expression is not understood", TOK_A))                                                  /* . */
//TODO:DEF_WARNING(W_TYPECAST_IN_EXPRESSION, (WG_USAGE, WG_SYNTAX), WSTATE_WARN, WARNF("C-style type cast " TOK_S " in expression is not understood (Consider using bit-masks to narrow integral types)", TOK_A)) /* . */
//TODO:DEF_WARNING(W_EXPECTED_RPAREN_AFTER_CAST, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q(")") " after casting type, but got " TOK_S, TOK_A))                                                               /* . */
//TODO:DEF_WARNING(W_EXPECTED_RBRACE_AFTER_STATEMENT, (WG_SYNTAX), WSTATE_ERROR, WARNF("Expected " Q("}") " after statement, but got " TOK_S, TOK_A))                                                             /* . */
//TODO:DEF_WARNING(W_EXPECTED_WARNING_NAMEORID, (WG_SYNTAX, WG_VALUE), WSTATE_ERROR, WARNF("Expected warning name or id, but got " Q("%s"), CONST_STR()))                                                         /* [struct TPPConst *]. */
//TODO:DEF_WARNING(W_CANT_DEFINE_LOCKED_KEYWORD, (WG_VALUE), WSTATE_DISABLED, WARNF("Cannot #define macro for locked keyword " Q("%s"), KWDNAME())) /* [struct TPPKeyword *]. */
//TODO:DEF_WARNING(W_CANT_UNDEF_LOCKED_KEYWORD, (WG_VALUE), WSTATE_DISABLED, WARNF("Cannot #undef macro for locked keyword " Q("%s"), KWDNAME()))   /* [struct TPPKeyword *]. */
//TODO:DEF_WARNING(W_INVALID_FLOAT_SUFFIX, (WG_SYNTAX), WSTATE_ERROR, { char *temp = ARG(char *); WARNF("Invalid floating point suffix " Q("%.*s"), (int)ARG(size_t), temp); }) /* [char const *,size_t] */
//TODO:DEF_WARNING(W_DEPENDENCY_CHANGED, (WG_DEPENDENCY), WSTATE_ERROR, {
//TODO:	char *depnam         = ARG(char *);
//TODO:	char *srcnam         = ARG(char *);
//TODO:	char *reason         = ARG(char *);
//TODO:	size_t reason_length = ARG(size_t);
//TODO:	WARNF("Dependency " Q("%s") " changed after " Q("%s"), depnam, srcnam);
//TODO:	if (reason_length)
//TODO:		WARNF(" (" Q("%.*s") ")", (unsigned int)reason_length, reason);
//TODO:})




/* API Compatibility */

#if 0 /* TODO: Not directly portable (write migration notes for this stuff) */
//struct TPPTextFile {
//	/* [owned((:f_name) = true]
//	 * HINT: `:f_name' usually is the string passed to the
//	 *        system for opening a stream for this file.
//	 *        Though this doesn't necessarily have to hold up,
//	 *        as pipe text files don't need to follow this rule. */
//	/*ref*/struct TPPFile   *f_cacheentry;  /* [0..1] Used when the copy of a file is loaded onto the #include-stack (points to the original version of the file)
//	                                         * NOTE: When non-NULL, always a textfile and this pointer also owns a reference to the associated textfile's `f_cacheinc' field. */
//	/*ref*/struct TPPString *f_usedname;    /* [0..1] When non-NULL, an override to the used filename (as set by `#line') */
//	TPP(line_t)              f_lineoff;     /* Offset of `f_begin' from the original start of the file in lines. */
//	TPP(stream_t)            f_stream;      /* Stream handle for reading more data. */
//	TPP(stream_t)            f_ownedstream; /* Usually equal to `f_stream', the stream that should be closed when it's EOF is reached (set to `TPP_STREAM_INVALID' if TPP shouldn't close the stream). */
//	/* NOTE: `:f_end' may not be equal to the end of `:f_text'.
//	 *        The difference between the two should be prefixed to the next chunk. */
//	struct TPPKeyword       *f_guard;       /* [0..1] Name of the #include guard for this file, or NULL if unknown. */
//	size_t                   f_cacheinc;    /* Used to track how often a given file is loaded onto the #include-stack.
//	                                         * (Only used for cached entires themself; aka. when `f_cacheentry == NULL'). */
//	size_t                   f_rdata;       /* (In bytes) The amount of data already read from the stream. */
//	char                     f_prefixdel;   /* The original character at `:f_end' that was overwritten with a `\0'. */
//#define TPP_TEXTFILE_FLAG_NONE         0x00 /* No special flags. */
//#define TPP_TEXTFILE_FLAG_NOGUARD      0x01 /* Set after a secondary #ifdef block was detected at the top level of this file:
//                                             * >> // File: "myfile.h"
//                                             * >> #ifndef foo // Potential include-guard?
//                                             * >> #endif
//                                             * >> #ifndef bar // This #ifndef will set `TPP_TEXTFILE_FLAG_NOGUARD'
//                                             * >> #endif */
//#define TPP_TEXTFILE_FLAG_SYSHEADER    0x02 /* This file is a system-header and all non-error warnings are suppressed. */
//#ifdef TPP_CONFIG_NONBLOCKING_IO
//#define TPP_TEXTFILE_FLAG_NONBLOCK     0x40 /* Do not block when reading file data.
//                                             * NOTE: Still block if waiting for the end of a non-escaped
//                                             *       linefeed, of the end of a comment, or string. */
//#endif /* TPP_CONFIG_NONBLOCKING_IO */
//#define TPP_TEXTFILE_FLAG_INTERNAL     0x80 /* This file is internal, meaning it shouldn't ~really~ represent a line/col number. */
//	unsigned char            f_flags;       /* A set of `TPP_TEXTFILE_FLAG_*' */
//	TPP(encoding_t)          f_encoding;    /* Encoding determined to-be used by this file. */
//	char                     f_padding[1];  /* Padding data... */
//	struct TPPKeyword       *f_newguard;    /* [0..1] The keyword of the #ifndef block that was determined to be located at
//	                                         *        the start of the file. When the file is popped from the #include-stack,
//	                                         *        this is non-NULL and `TPP_TEXTFILE_FLAG_NOGUARD' isn't set, this keyword
//	                                         *        will be copied into the `f_guard' field if not already set. */
//#ifndef TPP_CONFIG_NO_PRECACHE_TEXTLINES
//	char const              *f_lfpos;       /* [?..?] Cached line-feed counter pointer.
//	                                         * This pointer is only valid if it points
//	                                         * into the file's current text-block. */
//	size_t                   f_lfcnt;       /* [valid_if(is_valid(f_lfpos))] Number of line-feeds
//	                                         * that can be found between the file's start and `f_lfpos'. */
//#endif /* TPP_CONFIG_NO_PRECACHE_TEXTLINES */
//#ifdef TPP_USERTEXTDATA
//	TPP_USERTEXTDATA /* Optional user-defined data memory (when present, initialized to ZERO) */
//#endif /* TPP_USERTEXTDATA */
//};
//

///* Without the ident/sccs extension, disable the insert-comment callback. */
//#if (defined(TPP_CONFIG_EXTENSION_IDENT_SCCS) && !TPP_CONFIG_EXTENSION_IDENT_SCCS)
//#undef TPP_CONFIG_NO_CALLBACK_INS_COMMENT
//#undef TPP_CONFIG_CALLBACK_INS_COMMENT
//#undef TPP_CONFIG_DYN_CALLBACK_INS_COMMENT
//#define TPP_CONFIG_NO_CALLBACK_INS_COMMENT 1
//#endif
//
//
///* Figure out which callbacks should be dynamically linked at runtime. */
//#undef TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA
//#undef TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC
//#undef TPP_CONFIG_DYN_CALLBACK_INS_COMMENT
//#undef TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE
//#undef TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE
//#if (!defined(TPP_CONFIG_NO_CALLBACK_PARSE_PRAGMA) && \
//     !defined(TPP_CONFIG_CALLBACK_PARSE_PRAGMA))
//#define TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA 1
//#endif /* ... */
//#if (!defined(TPP_CONFIG_NO_CALLBACK_PARSE_PRAGMA_GCC) && \
//     !defined(TPP_CONFIG_CALLBACK_PARSE_PRAGMA_GCC))
//#define TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC 1
//#endif /* ... */
//#if (!defined(TPP_CONFIG_NO_CALLBACK_INS_COMMENT) && \
//     !defined(TPP_CONFIG_CALLBACK_INS_COMMENT))
//#define TPP_CONFIG_DYN_CALLBACK_INS_COMMENT 1
//#endif /* ... */
//#if (!defined(TPP_CONFIG_NO_CALLBACK_NEW_TEXTFILE) && \
//     !defined(TPP_CONFIG_CALLBACK_NEW_TEXTFILE))
//#define TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE 1
//#endif /* ... */
//#if (!defined(TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE) && \
//     !defined(TPP_CONFIG_CALLBACK_UNKNOWN_FILE))
//#define TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE 1
//#endif /* ... */
//#if (defined(TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA) ||     \
//     defined(TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC) || \
//     defined(TPP_CONFIG_DYN_CALLBACK_INS_COMMENT) ||      \
//     defined(TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE) ||     \
//     defined(TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE))
//#define TPP_CONFIG_DYN_CALLBACKS 1
//#endif /* ... */
//
//
//#ifdef TPP_CONFIG_DYN_CALLBACKS
//struct TPPCallbacks {
//	/* Optional user-hooks for implementing special preprocessor behavior.
//	 * NOTE: Any function pointer in here may be specified as NULL. */
//#ifdef TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA
//	/* Handle an unknown pragma.
//	 *  - The lexer currently points to the pragma's first token
//	 *    and is configured not to continue yielding tokens once
//	 *    the pragma's effective end is reached, as well as
//	 *    to ignore comment, space and LF tokens:
//	 *    >> #pragma foo bar   // [foo][bar][EOF]
//	 *    >> _Pragma("baz(2)") // [baz][(][2][)][EOF]
//	 *    >> __pragma(x*y)     // [x][*][y][EOF]
//	 * @return: 0: Unknown/errorous pragma.
//	 * @return: 1: Successfully parsed the given pragma. */
//	int (TPPCALL *c_parse_pragma)(void);
//#endif /* TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA */
//
//#ifdef TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC
//	/* Same as `c_parse_pragma', but invoked for unknown GCC-namespace pragmas
//	 * >> #pragma GCC visibility(push)
//	 *                ^^^^^^^^^^ Invoked on this token */
//	int (TPPCALL *c_parse_pragma_gcc)(void);
//#endif /* TPP_CONFIG_DYN_CALLBACK_PARSE_PRAGMA_GCC */
//
//#ifdef TPP_CONFIG_DYN_CALLBACK_INS_COMMENT
//	/* Insert the given text into the ".comment" section of the current object file.
//	 * @return: 0: Error occurred (Set a lexer error if not already set)
//	 * @return: 1: Successfully inserted the given text. */
//	int (TPPCALL *c_ins_comment)(struct TPPString *__restrict comment);
//#endif /* TPP_CONFIG_DYN_CALLBACK_INS_COMMENT */
//
//#ifdef TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE
//	/* Event-callback invoked when a textfile is included the first time.
//	 * >> Very useful for generating dependency trees.
//	 * NOTE: This function will only ever be called once
//	 *       for any given file within the same lexer.
//	 * @return: 0: Error occurred (Set a lexer error if not already set)
//	 * @return: 1: Continue parsing (same as not filling in this member). */
//	int (TPPCALL *c_new_textfile)(struct TPPFile *__restrict file, int is_system_header);
//#endif /* TPP_CONFIG_DYN_CALLBACK_NEW_TEXTFILE */
//
//#if TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE
//	/* Called when a given filename could not be found, allowing this
//	 * function to attempt more voodoo-magic in an attempt to locate it.
//	 * @param: mode:                    Set of `TPPLEXER_OPENFILE_FLAG_*' that are being used to open the file.
//	 * @return: NULL:                   Still failed to find the file (unless a lexer error was set, only emit a warning)
//	 * @return: * :                     The file we now managed to successfully open.
//	 * @return: TPP_UNKNOWN_FILE_RETRY: Instruct the file loader to try again (without invoking this function on that try)
//	 * WARNING: This callback is responsible to caching the file in a keyword entry!
//	 */
//	struct TPPFile *(TPPCALL *c_unknown_file)(int mode, char *__restrict filename,
//	                                          size_t filename_size,
//	                                          struct TPPKeyword **pkeyword_entry);
//#endif /* TPP_CONFIG_DYN_CALLBACK_UNKNOWN_FILE */
//};
//#endif /* TPP_CONFIG_DYN_CALLBACKS */
//
//#ifndef TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE
///* Return value by `c_unknown_file' to indicate that the open should be re-attempted. */
//#define TPP_UNKNOWN_FILE_RETRY ((struct TPPFile *)-1)
//#endif /* !TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE */

#endif

#define TPP_SYMARRAY_SIZE TPP_FLEX_ARRAY

#ifndef TPP_CONFIG_ONELEXER
/* Globally provide only one lexer (faster, but more restrictive).
 * 0: Have a global "struct TPPLexer *TPPLexer_Current"
 * 1: Have a global "struct TPPLexer TPPLexer_Global"
 * 2: Have a global "struct TPPLexer TPPLexer_Global" that can be backed-up/restored using "memcpy"
 * 3: Pass the current lexer to functions via arguments.*/
#define TPP_CONFIG_ONELEXER 1
#endif /* !TPP_CONFIG_ONELEXER */

#if TPP_CONFIG_ONELEXER == 3
#define TPP_LEXER_PARAM  tpp_lexer *__restrict _current
#define TPP_LEXER_PARAM_ tpp_lexer *__restrict _current,
#define TPP_LEXER__PARAM , tpp_lexer *__restrict _current
#define TPP_LEXER_ARG    _current
#define TPP_LEXER_ARG_   _current,
#define TPP_LEXER__ARG   , _current
#define TPP2_LEXER       _current
#else /* TPP_CONFIG_ONELEXER == 3 */
#define TPP_LEXER_PARAM  void
#define TPP_LEXER_PARAM_ /* nothing */
#define TPP_LEXER__PARAM /* nothing */
#define TPP_LEXER_ARG    /* nothing */
#define TPP_LEXER_ARG_   /* nothing */
#define TPP_LEXER__ARG   /* nothing */

#if TPP_CONFIG_ONELEXER == 0
#define TPP2_LEXER TPPLexer_Current
#else /* TPP_CONFIG_ONELEXER == 0 */
#define TPP2_LEXER (&TPPLexer_Global)
#endif /* TPP_CONFIG_ONELEXER != 0 */
#endif /* TPP_CONFIG_ONELEXER != 3 */



#define TPP_ISOK(id)                   (!TPP_TOK_ISERR(id))
#define TPP_ISKEYWORD(id)              TPP_TOK_ISKEYWORD(id)
#define TPP_ISUSERKEYWORD(id)          TPP_TOK_ISUSERKEYWORD(id)
#define TPP_ISBUILTINMACRO_(lexer, id) tpp_lexer_getkeyworddefined(lexer, tpp_lexer_kwds_getkeyword_byid(self, id))
#define TPP_ISBUILTINMACRO(id)         TPP_ISBUILTINMACRO_(TPP2_LEXER, id)

#define TPP_stream_t   tpp_io_handle
#define TPP_tok_t      tpp_token_id
#define TPP_hash_t     tpp_hash
#define TPP_encoding_t tpp_file_encoding
#define TPP_wgroup_t   tpp_warning_group_id
#define TPP_col_t      tpp_column
#define TPP_line_t     tpp_line
#define TPP_refcnt_t   tpp_refcnt
#define TPP_printer_t  tpp_formatprinter
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define stream_t   tpp_io_handle
#define tok_t      tpp_token_id
#define hash_t     tpp_hash
#define encoding_t tpp_file_encoding
#define wgroup_t   tpp_warning_group_id
#define col_t      tpp_column
#define line_t     tpp_line
#define refcnt_t   tpp_refcnt
#define printer_t  tpp_formatprinter
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */

#define TPP_ENCODING_UTF8     TPP_FILE_ENCODING_UTF8 /* Use TPP_FILE_ENCODING_ISUTF8() */
#define TPP_ENCODING_UTF16_BE TPP_FILE_ENCODING_UTF16_BE
#define TPP_ENCODING_UTF16_LE TPP_FILE_ENCODING_UTF16_LE
#define TPP_ENCODING_UTF32_BE TPP_FILE_ENCODING_UTF32_BE
#define TPP_ENCODING_UTF32_LE TPP_FILE_ENCODING_UTF32_LE

#define TPPKeyword tpp_keyword
#define TPPString  tpp_string
#define s_refcnt   ts_refcnt
#define s_size     ts_len
#define s_text     ts_str

#define TPPString_TEXT(x)   ((char *)tpp_string_str(x))
#define TPPString_SIZE(x)   tpp_string_len(x)
#define TPPString_Shared(x) tpp_string_isshared(x)
#define TPPString_Free(x)   tpp_string_destroy(x)
#define TPPString_Incref(x) tpp_string_incref(x)
#define TPPString_Decref(x) tpp_string_decref(x)
/*#define TPPString_Cat(lhs, rhs) tpp_string_cat(lhs, rhs)*/
/*#define TPPString_New(text, size) tpp_string_new(text, size)*/
#define TPPString_NewSized(size) tpp_string_malloc(size)
#define TPPString_NewEmpty()     tpp_string_newempty()

#define TPP_funop_t   tpp_macro_opcode
#define TPP_tint_t    tpp_intmax
#define TPP_tuint_t   tpp_uintmax
#define TPP_tfloat_t  tpp_float
#define TPP_arginfo_t tpp_macro_argument
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define funop_t   tpp_macro_opcode
#define tint_t    tpp_intmax
#define tuint_t   tpp_uintmax
#define tfloat_t  tpp_float
#define arginfo_t tpp_macro_argument
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */

#define ai_id tma_id
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
#define ai_ins tma_ins
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
#define ai_ins_exp tma_ins_exp
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#define ai_ins_str tma_ins_str
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_DEBUG
#define ai_name tma_name
/*#define ai_namesize tma_namelen*/
#endif /* TPP_DEBUG */

#define TPPLCInfo tpp_lcinfo
#define lc_line   lci_line
#define lc_col    lci_col

#define TPPMacroFile          tpp_macro
#define TPPFile               tpp_file
#define TPPFILE_KIND_TEXT     TPP_FILE_KIND_IO
#define TPPFILE_KIND_EXPLICIT TPP_FILE_KIND_TEXT
#define TPPFILE_KIND_MACRO    TPP_FILE_KIND_MACRO
#define f_kind                TPP_INTERNAL(tf_kind)
#define f_prev                TPP_INTERNAL(tf_tprev) /* Or maybe "tf_prev"... */
#define f_name                TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(tff_name) /* Should really use "tpp_file_filename()" instead! */
#define f_text                TPP_INTERNAL(tf_chunk)
#define f_begin               TPP_INTERNAL(tf_chunk)->TPP_INTERNAL(ts_str) /* Shouldn't be used */
#define f_end                 TPP_INTERNAL(tf_end)
#define f_pos                 TPP_INTERNAL(tf_pos)

#define TPPFile_LCAt(self, info, text_pointer) \
	(void)(*(info) = tpp_file_lcinfo(self, (tpp_char const *)(text_pointer)))
#define TPPFile_LineAt(self, text_pointer) \
	tpp_lcinfo_getline(tpp_file_lcinfo(self, (tpp_char const *)(text_pointer)))
#define TPPFile_ColumnAt(self, text_pointer) \
	tpp_lcinfo_getcol(tpp_file_lcinfo(self, (tpp_char const *)(text_pointer)))
#define TPPFile_Filename(self, opt_filename_length)                                         \
	((opt_filename_length)                                                                  \
	 ? (void)(*(tpp_size *)(opt_filename_length) = tpp_strlen(tpp_file_userfilename(self))) \
	 : (void)0,                                                                             \
	 tpp_file_userfilename(self))
#define TPPFile_RealFilename(self, opt_filename_length)                                 \
	((opt_filename_length)                                                              \
	 ? (void)(*(tpp_size *)(opt_filename_length) = tpp_strlen(tpp_file_filename(self))) \
	 : (void)0,                                                                         \
	 tpp_file_filename(self))


#define TPP_Itos(buf, i) tpp_itoa(buf, i)
TPP_INLINE tpp_size TPPCALL TPP_SizeofItos(tpp_intmax i) {
	char buf[TPP_ITOA_MAXLEN];
	return (tpp_size)((buf + TPP_ITOA_MAXLEN) - tpp_itoa(buf, i));
}

#define TPP_Hashof(data, size) tpp_hashof((tpp_char const *)(data), size)

#define TPP_wstate_t tpp_warning_state
#if TPP2_HAVE_GLOBAL_NAMESPACE
#define wstate_t        tpp_warning_state
#define WSTATE_DISABLED TPP_WSTATE_DISABLED
#define WSTATE_FATAL    TPP_WSTATE_FATAL
#define WSTATE_WARN     TPP_WSTATE_WARN
#define WSTATE_ERROR    TPP_WSTATE_ERROR
#define WSTATE_SUPPRESS TPP_WSTATE_SUPPRESS
#define WSTATE_DEFAULT  TPP_WSTATE_DEFAULT
#define WSTATE_DISABLE  TPP_WSTATE_DISABLED
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */
#define TPP_WSTATE_DISABLE TPP_WSTATE_DISABLED

#define TPP_WSTATE_ISENABLED(s) tpp_warning_state_willemit(s)

#if TPP2_HAVE_GLOBAL_NAMESPACE
#define WG_COUNT TPP_WG_COUNT
#define W_COUNT  TPP_W_COUNT
#endif /* TPP2_HAVE_GLOBAL_NAMESPACE */

#define TPPWarningStateEx      tpp_warning_suppress_item
#define wse_wid                TPP_INTERNAL(twsi_ctx_id)
#define wse_suppress           TPP_INTERNAL(twsi_count)
#define wse_oldstate           TPP_INTERNAL(twsi_restore)
#define TPP_WARNING_BITS       2
#define TPP_WARNING_TOTAL      TPP_WC_COUNT
#define TPP_WARNING_BITSETSIZE (((TPP_WARNING_TOTAL * TPP_WARNING_BITS + 7)) / 8)
#define TPPWarningState        tpp_warnings
#define ws_state               TPP_INTERNAL(tw_state).TPP_INTERNAL(tws_bitset)
#define ws_extendeda           TPP_INTERNAL(tw_suppressions).TPP_INTERNAL(tws_ctxc)
#define ws_extendedv           TPP_INTERNAL(tw_suppressions).TPP_INTERNAL(tws_ctxv)
#define ws_prev                TPP_INTERNAL(tw_prev)

#define TPPLexer_PushWarnings_(lexer) (tpp_lexer_pushwarnings(lexer), 1)
#define TPPLexer_PopWarnings_(lexer)  (tpp_lexer_canpopwarnings(lexer) && (tpp_lexer_pushwarnings(lexer), 1))
#define TPPLexer_PushWarnings()       TPPLexer_PushWarnings_(TPP2_LEXER)
#define TPPLexer_PopWarnings()        TPPLexer_PopWarnings_(TPP2_LEXER)


/* Set the state of a given warning number.
 * NOTE: If the given state is `WSTATE_SUPPRESS', ONE(1)
 *       will be added to the suppress recursion counter.
 * @return: 0: Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
 * @return: 1: Successfully set the given warning number.
 * @return: 2: The given warning number is unknown. */
#define TPPLexer_SetWarning(wnum, state)      TPPLexer_SetWarning_(TPP2_LEXER, wnum, state)
#define TPPLexer_SetWarningGroup(wgrp, state) TPPLexer_SetWarningGroup_(TPP2_LEXER, wgrp, state)
#define TPPLexer_GetWarning(wnum)             TPPLexer_GetWarning_(TPP2_LEXER, wnum)
#define TPPLexer_GetWarningGroup(wgrp)        TPPLexer_GetWarningGroup_(TPP2_LEXER, wgrp)
TPP_INLINE int TPPCALL TPPLexer_SetWarning_(tpp_lexer *self, int wnum, TPP_wstate_t state) {
	tpp_errno error;
	tpp_warning_context_id ctx = tpp_warning_context_id_ofwarning((tpp_warning_id)wnum);
	if ((unsigned int)ctx >= (unsigned int)TPP_WC_COUNT)
		return 2;
	error = tpp_lexer_setwarningctx(self, ctx, state);
	return error == TPP_EOK ? 0 : 1;
}
TPP_INLINE int TPPCALL TPPLexer_SetWarningGroup_(tpp_lexer *self, int wgrp, TPP_wstate_t state) {
	tpp_errno error;
	tpp_warning_context_id ctx = tpp_warning_context_id_ofgroup((tpp_warning_group_id)wgrp);
	if ((unsigned int)ctx >= (unsigned int)TPP_WC_COUNT)
		return 2;
	error = tpp_lexer_setwarningctx(self, ctx, state);
	return error == TPP_EOK ? 0 : 1;
}
TPP_INLINE TPP_wstate_t TPPCALL TPPLexer_GetWarning_(tpp_lexer *self, int wnum) {
	tpp_warning_context_id ctx = tpp_warning_context_id_ofwarning((tpp_warning_id)wnum);
	if ((unsigned int)ctx >= (unsigned int)TPP_WC_COUNT)
		return TPP_WSTATE_DISABLED;
	return tpp_lexer_getwarningctx(self, ctx);
}
TPP_INLINE TPP_wstate_t TPPCALL TPPLexer_GetWarningGroup_(tpp_lexer *self, int wgrp) {
	tpp_warning_context_id ctx = tpp_warning_context_id_ofgroup((tpp_warning_group_id)wgrp);
	if ((unsigned int)ctx >= (unsigned int)TPP_WC_COUNT)
		return TPP_WSTATE_DISABLED;
	return tpp_lexer_getwarningctx(self, ctx);
}

/* Similar to `TPPLexer_SetWarning', but set the state of all warnings from a given group.
 * NOTES:
 *   - Groups work independent of warning ids, meaning you can even
 *     specify `WSTATE_SUPPRESS' as state, with the next warning
 *     part of that group occurring simply consuming that suppression.
 *   - If you disable an entire warning group, no warning apart of it will be emit.
 *   - If a warning is invoked, that is both part of an error and a warning/disabled
 *     group it will always tend to do as little damage as possible:
 *     >> suppress >= disabled >= warning >= error
 *     With that in mind, both the warning itself, as well as all of its groups
 *     must be configured as `WSTATE_FATAL' for the warning to actually result
 *     in an error.
 * @return: 0: Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
 * @return: 1: Successfully set the given warning number.
 * @return: 2: The given group name is unknown. */
TPP_INLINE int TPPCALL
TPPLexer_SetWarnings_(tpp_lexer *self, char const *__restrict group, TPP_wstate_t state) {
	tpp_warning_group_id wgrp = tpp_warning_group_byname(group);
	return TPPLexer_SetWarningGroup_(self, (int)wgrp, state);
}
TPP_INLINE TPP_wstate_t TPPCALL
TPPLexer_GetWarnings_(tpp_lexer *self, char const *__restrict group) {
	tpp_warning_group_id wgrp = tpp_warning_group_byname(group);
	return TPPLexer_GetWarningGroup_(self, (int)wgrp);
}
#define TPPLexer_SetWarnings(group, state) TPPLexer_SetWarnings_(TPP2_LEXER, group, state)
#define TPPLexer_GetWarnings(group)        TPPLexer_GetWarnings_(TPP2_LEXER, group)

#define TPP_WARNINGMODE_FATAL  ((int)TPP_WSTATE_FATAL)
#define TPP_WARNINGMODE_ERROR  ((int)TPP_WSTATE_ERROR)
#define TPP_WARNINGMODE_WARN   ((int)TPP_WSTATE_WARN)
#define TPP_WARNINGMODE_IGNORE ((int)TPP_WSTATE_DISABLED)

/* Invoke a given warning number, returning one of `TPP_WARNINGMODE_*'.
 * NOTE: Unknown warnings will always result in `TPP_WARNINGMODE_WARN' being returned. */
#define TPPLexer_InvokeWarning(wnum) TPPLexer_InvokeWarning_(TPP2_LEXER, wnum)
TPP_INLINE int TPPCALL TPPLexer_InvokeWarning_(tpp_lexer *self, int wnum) {
	tpp_errno error;
	tpp_warning_invokeinfo info;
	if ((unsigned int)wnum >= (unsigned int)TPP_W_COUNT)
		return TPP_WARNINGMODE_IGNORE;
	error = tpp_lexer_invokewarning(self, (tpp_warning_id)wnum, &info);
	if (TPP_ISERR(error))
		return TPP_WARNINGMODE_FATAL; /* Wrong behavior... */
	return (int)info.twii_state;
}



#define TPPIfdefStackSlot tpp_ifdef_stack_entry
/*#define TPP_IFDEFMODE_FALSE 0 * Status doesn't exist in TPP3 anymore */
#define TPP_IFDEFMODE_TRUE  TPP_IFDEF_MODE_IFDEF /* The block is enabled. */
#define TPP_IFDEFMODE_ELSE  TPP_IFDEF_MODE_ELSE  /* The block follows an #else. */
#define iss_mode            TPP_INTERNAL(tidse_mode)
#define iss_line            TPP_INTERNAL(tidse_updated).TPP_INTERNAL(lci_line)
#define iss_file            tpp3_ifdef_stack_is_per_file_so_there_is_no_file_property
#define TPPIfdefStack       tpp_ifdef_stack
#define is_slotc            TPP_INTERNAL(tids_cnt)
#define is_slota            TPP_INTERNAL(tids_alc)
#define is_slotv            TPP_INTERNAL(tids_vec)

#define TPP_EXTENSIONS_BITSETSIZE (TPP_EXT_COUNT ? (TPP_EXT_COUNT + 7) / 8 : 1)
#define TPPExtState               tpp_extensions
#define es_prev                   TPP_INTERNAL(te_prev)
#define es_bitset                 TPP_INTERNAL(te_state).TPP_INTERNAL(tes_bitset)

/* Check if a given extension `ext' is currently enabled.
 * @return: 0: The extension is disabled.
 * @return: !0: The extension is enabled. */
#define TPPLexer_HasExtension_(_current, ext) tpp_lexer_getextension(_current, expt)
#define TPPLexer_HasExtension(ext)            TPPLexer_HasExtension_(TPP2_LEXER, ext)

/* Set the state of a given extension `ext'. */
#define TPPLexer_EnableExtension_(_current, ext)  tpp_lexer_enableextension(_current, ext)
#define TPPLexer_DisableExtension_(_current, ext) tpp_lexer_disableextension(_current, ext)
#define TPPLexer_EnableExtension(ext)             TPPLexer_EnableExtension_(TPP2_LEXER, ext)
#define TPPLexer_DisableExtension(ext)            TPPLexer_DisableExtension_(TPP2_LEXER, ext)


#if 0 /* TODO */
//TPPFUN int TPPCALL TPPFile_Copyname(struct TPPFile *__restrict self);
//TPPFUN /*ref*/struct TPPFile *TPPCALL TPPFile_CopyForInclude(struct TPPFile *__restrict self);
//TPPFUN /*ref*/struct TPPFile *TPPCALL TPPFile_Open(char const *__restrict filename);
//TPPFUN /*ref*/struct TPPFile *TPPCALL TPPFile_OpenStream(TPP(stream_t) stream, char const *__restrict name);
//TPPFUN struct TPPFile *TPPCALL TPPFile_NewDefine_(TPP_LEXER_PARAM);
//TPPFUN int TPPCALL TPPFile_NextChunk_(TPP_LEXER_PARAM_ struct TPPFile *__restrict self, unsigned int flags);
//TPPFUN char *TPPCALL TPP_Unescape_(TPP_LEXER_PARAM_ char *__restrict buf, char const *__restrict data, size_t size, size_t charsize);
//TPPFUN size_t TPPCALL TPP_SizeofUnescape_(TPP_LEXER_PARAM_ char const *__restrict data, size_t size, size_t charsize);
//TPPFUN char *TPPCALL TPP_UnescapeRaw(char *__restrict buf, char const *__restrict data, size_t size);
//TPPFUN size_t TPPCALL TPP_SizeofUnescapeRaw(char const *__restrict data, size_t size);
//TPPFUN char *TPPCALL TPP_Escape_(TPP_LEXER_PARAM_ char *__restrict buf, char const *__restrict data, size_t size);
//TPPFUN size_t TPPCALL TPP_SizeofEscape_(TPP_LEXER_PARAM_ char const *__restrict data, size_t size);
//TPPFUN char *TPPCALL TPP_Ftos(char *__restrict buf, TPP(tfloat_t) f);
//TPPFUN size_t TPPCALL TPP_SizeofFtos(TPP(tfloat_t) f);


//struct TPPIncludeList {
//	/* List of sanitized #include paths. */
//	struct TPPIncludeList    *il_prev;  /* [0..1][owned] Pointer to another m-allocated list of system #include-paths.
//	                                     *                This field is used to implement system #include-path push/pop. */
//	size_t                    il_pathc; /* Amount of elements in the vector below. */
//	/*ref*/struct TPPString **il_pathv; /* [1..1][0..il_pathc][owned] Vector of sanitized #include path. */
//};
//
///* Push/Pop the current system #include-path list.
// * @return: 0: [TPPLexer_PushInclude] Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
// * @return: 0: [TPPLexer_PopInclude] No older #include-path list was available to restore.
// * @return: 1: Successfully pushed/popped the system #include-path list. */
//TPPFUN int TPPCALL TPPLexer_PushInclude_(TPP_LEXER_PARAM);
//TPPFUN int TPPCALL TPPLexer_PopInclude_(TPP_LEXER_PARAM);
//#define TPPLexer_PushInclude() TPPLexer_PushInclude_(TPP_LEXER_ARG)
//#define TPPLexer_PopInclude()  TPPLexer_PopInclude_(TPP_LEXER_ARG)
//
///* Add/delete the given path from the list of system #include paths.
// * WARNING: This function will modify the given path.
// * WARNING: Be careful with absolute vs. relative paths!
// *          TPP can not tell that they're the same and
// *          `#pragma once' might break as a consequence!
// *       >> As a solution, _always_ use either absolute
// *          or relative paths for the same file/path.
// *         (This also goes for #include directives)
// * @return: 0: [TPPLexer_AddIncludePath] Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
// * @return: 1: [TPPLexer_AddIncludePath] The given path was successfully added.
// * @return: 2: [TPPLexer_AddIncludePath] The given path had already been added before.
// * @return: 0: [TPPLexer_DelIncludePath] The given path was not found.
// * @return: 1: [TPPLexer_DelIncludePath] The given path was successfully removed. */
//TPPFUN int TPPCALL TPPLexer_AddIncludePath_(TPP_LEXER_PARAM_ char *__restrict path, size_t pathsize);
//TPPFUN int TPPCALL TPPLexer_DelIncludePath_(TPP_LEXER_PARAM_ char *__restrict path, size_t pathsize);
//#define TPPLexer_AddIncludePath(path, pathsize) TPPLexer_AddIncludePath_(TPP_LEXER_ARG_ path, pathsize)
//#define TPPLexer_DelIncludePath(path, pathsize) TPPLexer_DelIncludePath_(TPP_LEXER_ARG_ path, pathsize)
//
//
//#ifndef TPP_CONFIG_NO_ASSERTIONS
//struct TPPAssertion {
//	struct TPPAssertion *as_next; /* [0..1][owned] Next assertion. */
//	struct TPPKeyword   *as_kwd;  /* [1..1][const] Keyword associated with this assertion. */
//};
//struct TPPAssertions {
//	/* s.a.: `https://gcc.gnu.org/onlinedocs/cpp/Obsolete-Features.html' */
//	size_t                as_assc; /* Amount of defined assertions. */
//	size_t                as_assa; /* Allocated amount of assertions. */
//	struct TPPAssertion **as_assv; /* [0..1][owned][0..as_alloc][owned] Hash-map of existing assertions. */
//};
//#endif /* !TPP_CONFIG_NO_ASSERTIONS */
//
//struct TPPRareKeyword {
//	/* Keyword-specific data that is only rarely used. */
//	/*ref*/struct TPPFile    *kr_file;     /* [0..1] Set if this keyword is actually the name of a file, when that file was already included.
//	                                        *        Used to track recursive file-inclusion, as well as quickly dismiss guarded files and
//	                                        *        speed up determining the correct filename for `include_next'.
//	                                        *  NOTE: Sadly, this cannot simply be made into a union with `k_macro', as a filename could
//	                                        *        potentially be equal to a keyword (e.g.: `#include "header"' vs. `#define header 42') */
//	/*ref*/struct TPPFile    *kr_oldmacro; /* [0..1][linked_list(->f_hashnext...)] Linked list of old (aka. pushed) version of this macro. */
//	/*ref*/struct TPPFile    *kr_defmacro; /* [0..1] Default macro definition (backup of the original, builtin macro when re-defined by the
//	                                        *        user). This macro file is restored as the active macro when lexer macros are reset and
//	                                        *        the `TPP_KEYWORDFLAG_BUILTINMACRO' keyword flag is set below. */
//#define TPP_KEYWORDFLAG_NONE                   0x00000000 /* No special flags (default) */
//#define TPP_KEYWORDFLAG_BUILTINMACRO           0x20000000 /* An explicitly defined builtin macro-definition, that can't be #undef'ed */
//#define TPP_KEYWORDFLAG_NO_UNDERSCORES         0x40000000 /* When looking up keyword flags, don't allow this keyword to alias another with additional underscores at the front and back:
//                                                           * >> __has_feature(__tpp_dollar_is_alpha__) // If `tpp_dollar_is_alpha' doesn't have this flag set, it can alias `__tpp_dollar_is_alpha__' */
//#define TPP_KEYWORDFLAG_IMPORTED               0x80000000 /* Set for for files after they've been #import-ed. */
//#if !defined(TPP_CONFIG_LOCKED_KEYWORDS) || TPP_CONFIG_LOCKED_KEYWORDS
//#define TPP_KEYWORDFLAG_LOCKED                 0x00000200 /* Any attempts at defining or deleting a macro for this keyword using #define or #undef are denied. */
//#endif /* TPP_CONFIG_LOCKED_KEYWORDS */
//	/* NOTE: These flags share their values with those
//	 *       from the old TPP for backwards compatibility. */
//#define TPP_KEYWORDFLAG_HAS_ATTRIBUTE          0x00000001 /* `__has_attribute()' */
//#define TPP_KEYWORDFLAG_HAS_BUILTIN            0x00000002 /* `__has_builtin()' */
//#define TPP_KEYWORDFLAG_HAS_CPP_ATTRIBUTE      0x00000004 /* `__has_cpp_attribute()' */
//#define TPP_KEYWORDFLAG_HAS_DECLSPEC_ATTRIBUTE 0x00000008 /* `__has_declspec_attribute()' */
//#define TPP_KEYWORDFLAG_HAS_EXTENSION          0x00000010 /* `__has_extension()' */
//#define TPP_KEYWORDFLAG_HAS_FEATURE            0x00000020 /* `__has_feature()' */
//#define TPP_KEYWORDFLAG_IS_DEPRECATED          0x00000040 /* Warn when the keyword appears as the result of lexical processing. */
//#define TPP_KEYWORDFLAG_IS_POISONED            0x00000080 /* Extension for `TPP_KEYWORDFLAG_IS_DEPRECATED':
//                                                           * Don't emit a warning if the keyword is used inside of a macro.
//                                                           * -> Only warn if it is used from a text file. */
//#define TPP_KEYWORDFLAG_HAS_TPP_BUILTIN        0x00000100 /* The keyword is a __builtin-function available for use in preprocessor expressions. */
//#define TPP_KEYWORDFLAG_USERMASK               0x0000007f /* Set of flags modifiable through `#pragma tpp_set_keyword_flags()'. */
//	uint32_t                  kr_flags;    /* A set of `TPP_KEYWORDFLAG_*'. */
//	TPP(tint_t)               kr_counter;  /* Counter value used by `__TPP_COUNTER()' */
//#ifndef TPP_CONFIG_NO_ASSERTIONS
//	struct TPPAssertions      kr_asserts;  /* Assertions (aka. #assert/#unassert associated with this keyword) */
//#endif /* !TPP_CONFIG_NO_ASSERTIONS */
//	void                     *kr_user;     /* User-defined meaning; initialized to NULL. */
//};
//
//struct TPPKeyword {
//	       struct TPPKeyword *k_next;     /* [0..1][owned] Pointer to another keyword entry with the same hash. */
//	/*ref*/struct TPPFile    *k_macro;    /* [0..1] Macro currently associated with this keyword. */
//	struct TPPRareKeyword    *k_rare;     /* [0..1][owned] Rare keyword data. */
//#ifdef __INTELLISENSE__
//	    tok_t                 k_id;       /* [const] Unique token ID associated with this keyword. */
//#else /* __INTELLISENSE__ */
//	TPP(tok_t)                k_id;       /* [const] Unique token ID associated with this keyword. */
//#endif /* !__INTELLISENSE__ */
//#if __SIZEOF_POINTER__ > __SIZEOF_INT__
//	uint8_t                   k_pad[sizeof(void *) - sizeof(int)];
//#endif /* __SIZEOF_POINTER__ > __SIZEOF_INT__ */
//	size_t                    k_size;     /* [const] Size of the keyword (in characters). */
//	TPP(hash_t)               k_hash;     /* [const] The hash-value of the keyword. */
//	char                      k_name[TPP_SYMARRAY_SIZE]; /* [const][k_size] name of this keyword (HINT: doesn't contain unescaped linefeeds). */
///*	char                      k_zero;     /* [const][== 0] Ensure ZERO-termination of the keyword name. */
//};
//#define TPPKeyword_ISDEFINED(self) ((self)->k_macro != NULL || TPP_ISBUILTINMACRO((self)->k_id))
//#define TPPKeyword_MAKERARE(self)                                  \
//	((self)->k_rare || ((self)->k_rare = (struct TPPRareKeyword *) \
//	                    calloc(1, sizeof(struct TPPRareKeyword))) != NULL)
//#ifdef TPP_HASHOF
//#define TPPKeyword_EQUALS(self, constant_string)                       \
//	((self)->k_hash == TPP_HASHOF(constant_string) &&                  \
//	 (self)->k_size == (sizeof(constant_string) / sizeof(char)) - 1 && \
//	 memcmp((self)->k_name, constant_string, sizeof(constant_string) - sizeof(char)) == 0)
//#else /* TPP_HASHOF */
//#define TPPKeyword_EQUALS(self, constant_string)                       \
//	((self)->k_size == (sizeof(constant_string) / sizeof(char)) - 1 && \
//	 memcmp((self)->k_name, constant_string, sizeof(constant_string) - sizeof(char)) == 0)
//#endif /* !TPP_HASHOF */
//
//
///* Returns the effective keyword flags of `self'.
// * @return: A set of `TPP_KEYWORDFLAG_*' */
//TPPFUN uint32_t TPPCALL
//TPPKeyword_GetFlags_(TPP_LEXER_PARAM_
//                     struct TPPKeyword const *__restrict self,
//                     int check_without_underscores);
//#define TPPKeyword_GetFlags(self, check_without_underscores) \
//	TPPKeyword_GetFlags_(TPP_LEXER_ARG_ self, check_without_underscores)
//
//
//struct TPPKeywordMap {
//	size_t              km_entryc;  /* Amount of keyword entries stored. */
//	size_t              km_bucketc; /* Used amount of buckets. */
//	struct TPPKeyword **km_bucketv; /* [0..1][owned][0..km_bucketc][owned]
//	                                 * Resizable keyword hash-map vector. */
//};
///* When this evaluates to true, TPP attempts to rehash the keyword map to `km_entryc' entries. */
//#define TPPKeywordMap_SHOULDHASH(self) \
//	((self)->km_entryc >= (self)->km_bucketc * 2)
//
//struct TPPToken {
//	TPP(tok_t)              t_id;    /* The symbol/keyword ID of this token. */
//	unsigned long           t_num;   /* The token number (incremented every time a new token is yielded). */
//	/*ref*/struct TPPFile  *t_file;  /* [1..1] File associated with this token. */
//	char                   *t_begin; /* [1..1][<= t_end] Token text start pointer. */
//	char                   *t_end;   /* [1..1][>= t_begin] Token text end pointer. */
//	struct TPPKeyword      *t_kwd;   /* [0..1] Set when `t_id' is a keyword (WARNING: Not always updated during yield; check `TPP_ISKEYWORD(t_id)' before using). */
//};
//
///* Returns the top-level source locations (in-macro & everything)
// * NOTE: These are _not_ what you're probably looking for.
// *       You probably expect these to act like __FILE__ and __LINE__,
// *       but instead they will show the true source locations where the
// *       current token originates from (e.g.: from a macro definition.)
// *    >> To get information about the source file you must walk the
// *       current token's t_file->f_prev->... chain until you reach
// *       a text file. Then, taking that file, use it and its f_pos
// *       pointers to figure out what you actually want to know. */
//#define TPPLexer_TRUE_FILE(plength) TPPFile_Filename(TPPLexer_Current->l_token.t_file, plength)
//#define TPPLexer_TRUE_LC(info)      TPPFile_LCAt(TPPLexer_Current->l_token.t_file, info, TPPLexer_Current->l_token.t_begin)
//#define TPPLexer_TRUE_LINE()        TPPFile_LineAt(TPPLexer_Current->l_token.t_file, TPPLexer_Current->l_token.t_begin)
//#define TPPLexer_TRUE_COLUMN()      TPPFile_ColumnAt(TPPLexer_Current->l_token.t_file, TPPLexer_Current->l_token.t_begin)
//
///* Returns the top-most text file associated with the current lexer.
// * NOTE: These functions never returns NULL. */
//TPPFUN struct TPPFile *TPPCALL TPPLexer_Textfile_(TPP_LEXER_PARAM);
//TPPFUN struct TPPFile *TPPCALL TPPLexer_Basefile_(TPP_LEXER_PARAM);
//#define TPPLexer_Textfile() TPPLexer_Textfile_(TPP_LEXER_ARG)
//#define TPPLexer_Basefile() TPPLexer_Basefile_(TPP_LEXER_ARG)
//
//#define TPPLexer_FILE(plength)     TPPFile_Filename(TPPLexer_Textfile(), plength)
//#define TPPLexer_BASEFILE(plength) TPPFile_Filename(TPPLexer_Basefile(), plength)
//#define TPPLexer_LC(info)          TPPLexer_LC_(TPP_LEXER_ARG_ info)
//#define TPPLexer_LINE()            TPPLexer_LINE_(TPP_LEXER_ARG)
//#define TPPLexer_COLUMN()          TPPLexer_COLUMN_(TPP_LEXER_ARG)
//TPP_LOCAL void TPPCALL TPPLexer_LC_(TPP_LEXER_PARAM_ struct TPPLCInfo *__restrict info) {
//	struct TPPFile *f = TPPLexer_Textfile();
//	TPPFile_LCAt(f, info, f->f_pos);
//}
//TPP_LOCAL TPP(line_t) TPPCALL TPPLexer_LINE_(TPP_LEXER_PARAM) {
//	struct TPPFile *f = TPPLexer_Textfile();
//	return TPPFile_LineAt(f, f->f_pos);
//}
//TPP_LOCAL TPP(col_t) TPPCALL TPPLexer_COLUMN_(TPP_LEXER_PARAM) {
//	struct TPPFile *f = TPPLexer_Textfile();
//	return TPPFile_ColumnAt(f, f->f_pos);
//}
//
//
///* Saved lexer position (can be restore to rewind the lexer)
// * When wanting to rewind the lexer, flags must be set as follows (+: set; -: clear)
// *  [+] TPPLEXER_FLAG_NO_DIRECTIVES     (Disallow directives since rewinding them isn't possible
// *                                      If a macro were to be deleted or defined, we'd not be
// *                                      able to undo that)
// *  [+] TPPLEXER_FLAG_EXTENDFILE        (Don't discard already-read data)
// *  [-] TPPLEXER_FLAG_WERROR            (You probably don't want errors)
// *  [+] TPPLEXER_FLAG_NO_WARNINGS       (You probably don't want warnings enabled...)
// *  [+] TPPLEXER_FLAG_WILLRESTORE       (Prevent some internal events from firing)
// * WARNING:
// *  - __TPP_RANDOM will yield different results after rewinding!
// *  - __TPP_COUNTER and __COUNTER__ will not be reset during rewinding!
// */
//struct TPPLexerFilePosition {
//	/*ref*/struct TPPFile *tlfp_file; /* [1..1] The file to restore. */
//	ptrdiff_t              tlfp_pos;  /* File position to restore (relative to `tlfp_file->f_begin'). */
//};
//struct TPPLexerPosition {
//	TPP(tok_t)                   tlp_tok_id;    /* The symbol/keyword ID of this token. */
//	unsigned long                tlp_tok_num;   /* The token number (incremented every time a new token is yielded). */
//	ptrdiff_t                    tlp_tok_begin; /* [<= tlp_tok_end] Token text start pointer (relative to `tlp_filev[tlp_filec-1].tlfp_file->f_begin'). */
//	ptrdiff_t                    tlp_tok_end;   /* [>= tlp_tok_begin] Token text end pointer. */
//	struct TPPKeyword           *tlp_tok_kwd;   /* [0..1] Set when `t_id' is a keyword (WARNING: Not always updated during yield; check `TPP_ISKEYWORD(t_id)' before using). */
//	size_t                       tlp_filec;     /* # of files to restore from the #include-stack. */
//	struct TPPLexerFilePosition *tlp_filev;     /* [0..tlp_filec] Vector of files to restore. */
//	uint32_t                     tlp_flags;     /* Old lexer flags. */
//};
//
///* Save/restore the lexer position (during save: lexer flags are altered as documented above)
// * @return: 1 : Success
// * @return: 0 : Error */
//TPPFUN int TPPCALL TPPLexer_SavePosition_(TPP_LEXER_PARAM_ struct TPPLexerPosition *__restrict self);
//TPPFUN void TPPCALL TPPLexer_LoadPosition_(TPP_LEXER_PARAM_ struct TPPLexerPosition *__restrict self);
//#define TPPLexer_SavePosition(self) TPPLexer_SavePosition_(TPP_LEXER_ARG_ self)
//#define TPPLexer_LoadPosition(self) TPPLexer_LoadPosition_(TPP_LEXER_ARG_ self)
//
//
//
///* Lexer state flags. */
//#define TPPLEXER_FLAG_NONE                   0x00000000
//#define TPPLEXER_FLAG_WANTCOMMENTS           0x00000001 /* Emit COMMENT tokens. */
//#define TPPLEXER_FLAG_WANTSPACE              0x00000002 /* Emit SPACE tokens. */
//#define TPPLEXER_FLAG_WANTLF                 0x00000004 /* Emit LF tokens. */
//#define TPPLEXER_FLAG_NO_SEEK_ON_EOB         0x00000008 /* Don't seek the next chunk when the current one ends (instead, signal EOF). */
//#define TPPLEXER_FLAG_NO_POP_ON_EOF          0x00000010 /* Don't pop the top file when an EOF occurs. */
//#define TPPLEXER_FLAG_KEEP_MACRO_WHITESPACE  0x00000020 /* Keep whitespace tokens around the front and back of macro texts. */
//#ifdef TPP_CONFIG_NONBLOCKING_IO
//#define TPPLEXER_FLAG_NONBLOCKING            0x00000040 /* Enable non-blocking I/O in files that support that flag.
//                                                         * When enabled, the lexer will produce EOF tokens in text files
//                                                         * with the `TPP_TEXTFILE_FLAG_NONBLOCK' flag set, if the current
//                                                         * text location is desirable for such behavior (i.e. no trailing
//                                                         * unescaped linefeeds, or unfinished comments or strings), if
//                                                         * reading data from the source stream would block.
//                                                         * For convenience, you may use `TPPLexer_Yield*NB()' to set this
//                                                         * flag for a single call. */
//#endif /* TPP_CONFIG_NONBLOCKING_IO */
//#define TPPLEXER_FLAG_TERMINATE_STRING_LF    0x00000080 /* Terminate character/string sequences when a linefeed is detected (also emit a warning in that case). */
//#define TPPLEXER_FLAG_NO_DIRECTIVES          0x00000100 /* Disable evaluation of preprocessor directives. */
//#define TPPLEXER_FLAG_NO_MACROS              0x00000200 /* Disable expansion of macros (user defined only; builtin must be disabled explicitly with `TPPLEXER_FLAG_NO_BUILTIN_MACROS'). */
//#define TPPLEXER_FLAG_NO_BUILTIN_MACROS      0x00000400 /* When set, don't expand _any_ builtin macros (such as __FILE__ and __LINE__). */
//#define TPPLEXER_FLAG_ASM_COMMENTS           0x00000800 /* Suppress warnings for unknown/invalid preprocessor directives, instead either emitting them as `TOK_COMMENT' or ignoring them based on `TPPLEXER_FLAG_WANTCOMMENTS'. */
//#define TPPLEXER_FLAG_DIRECTIVE_NOOWN_LF     0x00001000 /* Linefeeds terminating preprocessor directives are not part of those directives and are instead re-emit (Meaningless without `TPPLEXER_FLAG_WANTLF').
//                                                         * WARNING: Using this flag is not recommended, as a freshly defined macro will modify
//                                                         *          text from the file and set the first character of that linefeed to `\0'. */
//#define TPPLEXER_FLAG_COMMENT_NOOWN_LF       0x00002000 /* Linefeeds terminating a `// foo'-style comment are not owned by that comment, but are re-emit (Meaningless without `TPPLEXER_FLAG_WANTLF'). */
//#define TPPLEXER_FLAG_MESSAGE_LOCATION       0x00004000 /* Print the file+line location in messages from `#pragma message'. */
//#define TPPLEXER_FLAG_MESSAGE_NOLINEFEED     0x00008000 /* Don't print a linefeed following the user-provided message in `#pragma message'. */
//#define TPPLEXER_FLAG_INCLUDESTRING          0x00010000 /* Parse strings as #include strings (without \-escape sequences). (WARNING: system-style (<...>) strings must be handled manually by the caller) */
//#define TPPLEXER_FLAG_EXTENDFILE             0x00020000 /* Set the `TPPFILE_NEXTCHUNK_FLAG_EXTEND' flag when loading more file data. */
//#define TPPLEXER_FLAG_NO_LEGACY_GUARDS       0x00040000 /* Don't recognize legacy #include-guards
//                                                         * WARNING: Not setting this option may lead to whitespace and comments at the
//                                                         *          start and end of a guarded file to not be emit on a second pass.
//                                                         *       >> Enable this option when your compiler is sensitive to whitespace
//                                                         *          in situations such as these. */
//#define TPPLEXER_FLAG_WERROR                 0x00080000 /* All warnings are turned into errors (NOTE: less powerful than `TPPLEXER_FLAG_WSYSTEMHEADERS'). */
//#define TPPLEXER_FLAG_WSYSTEMHEADERS         0x00100000 /* Still emit warnings in system headers (alongside errors). */
//#define TPPLEXER_FLAG_NO_DEPRECATED          0x00200000 /* Don't warn about deprecated or poisoned keywords. */
//#define TPPLEXER_FLAG_MSVC_MESSAGEFORMAT     0x00400000 /* Use msvc's file+line format `%s(%d,%d) : ' instead of GCC's `%s:%d:%d: '. */
//#define TPPLEXER_FLAG_NO_WARNINGS            0x00800000 /* Don't emit warnings. */
//#define TPPLEXER_FLAG_NO_ENCODING            0x01000000 /* Don't try to detect file encodings (Everything is UTF-8 without BOM; aka. raw text). */
//#define TPPLEXER_FLAG_REEMIT_UNKNOWN_PRAGMA  0x02000000 /* Re-emit unknown pragmas. */
//#define TPPLEXER_FLAG_CHAR_UNSIGNED          0x04000000 /* When set, character-constants are unsigned. */
//#define TPPLEXER_FLAG_EOF_ON_PAREN           0x08000000 /* When set, recursively track `(`...`)' pairs and yield EOF when `l_eof_paren' reaches ZERO(0). */
//#define TPPLEXER_FLAG_WILLRESTORE            0x10000000 /* A preceding lexer position will be restored (don't fire internal events that could pose problems with this) */
///*                                           0x20000000 /* ... */
//#define TPPLEXER_FLAG_RANDOM_INITIALIZED     0x40000000 /* Set when rand() has been initialized. */
//#define TPPLEXER_FLAG_ERROR                  0x80000000 /* When set, the lexer is in an error-state in which calls to yield() will return TOK_ERR. */
//#define TPPLEXER_FLAG_MERGEMASK              0xe0000000 /* A mask of flags that are merged (or'd together) during popf(). */
//#define TPPLEXER_FLAG_DEFAULT                0x00000000 /* Default set of flags (suitable for use with most token-based compilers). */
///* A mask of flags that are preserved when modified by #pragma directives.
// * WARNING: `TPPLEXER_FLAG_MERGEMASK' must not be part of this mask.
// * WARNING: `TPPLEXER_FLAG_NO_MACROS|TPPLEXER_FLAG_NO_DIRECTIVES|TPPLEXER_FLAG_NO_BUILTIN_MACROS'
// *           must not be part of this mask, as these flags may
// *           contain custom values while pragmas are executed. */
//#define TPPLEXER_FLAG_PRAGMA_KEEPMASK                                          \
//	(TPPLEXER_FLAG_KEEP_MACRO_WHITESPACE | TPPLEXER_FLAG_TERMINATE_STRING_LF | \
//	 TPPLEXER_FLAG_ASM_COMMENTS | TPPLEXER_FLAG_MESSAGE_LOCATION |             \
//	 TPPLEXER_FLAG_MESSAGE_NOLINEFEED | TPPLEXER_FLAG_EXTENDFILE |             \
//	 TPPLEXER_FLAG_NO_LEGACY_GUARDS | TPPLEXER_FLAG_WERROR |                   \
//	 TPPLEXER_FLAG_WSYSTEMHEADERS | TPPLEXER_FLAG_NO_DEPRECATED |              \
//	 TPPLEXER_FLAG_MSVC_MESSAGEFORMAT | TPPLEXER_FLAG_NO_WARNINGS |            \
//	 TPPLEXER_FLAG_NO_ENCODING | TPPLEXER_FLAG_REEMIT_UNKNOWN_PRAGMA |         \
//	 TPPLEXER_FLAG_CHAR_UNSIGNED | TPPLEXER_FLAG_WILLRESTORE)
//
//
//
///* Recognized extension token flags. */
//#define TPPLEXER_TOKEN_NONE                  0x00000000
//#define TPPLEXER_TOKEN_TILDETILDE            0x00000001 /* Enable recognition of `~~' tokens. */
//#define TPPLEXER_TOKEN_ROOFROOF              0x00000002 /* Enable recognition of `^^' tokens. */
//#define TPPLEXER_TOKEN_COLONCOLON            0x00000004 /* Enable recognition of `::' tokens. */
//#define TPPLEXER_TOKEN_COLONASSIGN           0x00000008 /* Enable recognition of `:=' tokens. */
//#define TPPLEXER_TOKEN_STARSTAR              0x00000010 /* Enable recognition of `**' and `**=' tokens. */
//#define TPPLEXER_TOKEN_ARROW                 0x00000020 /* Enable recognition of `->' tokens. */
//#define TPPLEXER_TOKEN_ARROWSTAR             0x00000040 /* Enable recognition of `->*' tokens. */
//#define TPPLEXER_TOKEN_DOTSTAR               0x00000080 /* Enable recognition of `.*' tokens. */
//#define TPPLEXER_TOKEN_DOTDOT                0x00000100 /* Enable recognition of `..' tokens. */
//#define TPPLEXER_TOKEN_ATEQUAL               0x00000200 /* Enable recognition of `@=' tokens. */
//#define TPPLEXER_TOKEN_C_COMMENT             0x00000400 /* Enable recognition of `/[]* comment *[]/' tokens. */
//#define TPPLEXER_TOKEN_CPP_COMMENT           0x00000800 /* Enable recognition of `// comment' tokens. */
//#define TPPLEXER_TOKEN_ANGLE3                0x00001000 /* Enable recognition of `<<<' and `>>>' tokens. */
//#define TPPLEXER_TOKEN_ANGLE3_EQUAL          0x00002000 /* Enable recognition of `<<<=' and `>>>=' tokens. */
//#define TPPLEXER_TOKEN_LOGT                  0x00004000 /* Enable recognition of `<>' tokens. */
//#define TPPLEXER_TOKEN_EQUALBINOP            0x00008000 /* Enable recognition of `=+', `=-', `=*', `=/', `=%', `=&', `=|', `=^', `=<<', `=>>', `=>>>', `=<<<', `=@' and `=**' tokens (NOTE: These are all aliasing the regular inplace versions).
//                                                         * NOTE: Special token such as `=@' or `=<<<' are only available when other token extensions are enabled as well! */
//#define TPPLEXER_TOKEN_EQUAL3                0x00010000 /* Enable recognition of `===' and `!==' tokens. */
//#define TPPLEXER_TOKEN_QMARK_QMARK           0x00020000 /* Enable recognition of `??' tokens. */
//#define TPPLEXER_TOKEN_DOLLAR                0x80000000 /* Recognize `$' as its own token (Supersedes `EXT_DOLLAR_IS_ALPHA'). */
//#define TPPLEXER_TOKEN_DEFAULT               0x0fffffff /* Default set of extension tokens (enable all). */
//
///* Deprecated typos */
//#define TPPLEXER_TOKEN_COLLONCOLLON TPPLEXER_TOKEN_COLONCOLON
//#define TPPLEXER_TOKEN_COLLONASSIGN TPPLEXER_TOKEN_COLONASSIGN
//
//
///* Predefined set of extension tokens for some languages.
// * WARNING: Most of these languages will also need additional tweaks to other flags. */
//#define TPPLEXER_TOKEN_LANG_C                          \
//	(TPPLEXER_TOKEN_ARROW | TPPLEXER_TOKEN_C_COMMENT | \
//	 TPPLEXER_TOKEN_CPP_COMMENT)
//#define TPPLEXER_TOKEN_LANG_ASM                    \
//	(TPPLEXER_TOKEN_DOLLAR | TPPLEXER_TOKEN_LOGT | \
//	 TPPLEXER_TOKEN_C_COMMENT | TPPLEXER_TOKEN_CPP_COMMENT)
//#define TPPLEXER_TOKEN_LANG_CPP                          \
//	(TPPLEXER_TOKEN_COLONCOLON | TPPLEXER_TOKEN_ARROW |  \
//	 TPPLEXER_TOKEN_ARROWSTAR | TPPLEXER_TOKEN_DOTSTAR | \
//	 TPPLEXER_TOKEN_C_COMMENT | TPPLEXER_TOKEN_CPP_COMMENT)
//#define TPPLEXER_TOKEN_LANG_JAVA \
//	(TPPLEXER_TOKEN_C_COMMENT | TPPLEXER_TOKEN_CPP_COMMENT)
//#define TPPLEXER_TOKEN_LANG_DEEMON                            \
//	(TPPLEXER_TOKEN_COLONCOLON | TPPLEXER_TOKEN_COLONASSIGN | \
//	 TPPLEXER_TOKEN_STARSTAR | TPPLEXER_TOKEN_ARROW |         \
//	 TPPLEXER_TOKEN_C_COMMENT | TPPLEXER_TOKEN_CPP_COMMENT |  \
//	 TPPLEXER_TOKEN_EQUAL3 | TPPLEXER_TOKEN_QMARK_QMARK)
//
//
//struct TPPLexer {
//	struct TPPToken       l_token;      /* The current token. */
//	struct TPPFile       *l_eob_file;   /* [0..1] When non-NULL prevent seek_on_eob when this file is atop the stack.
//	                                     * >> NOTE: This does the same as `TPPLEXER_FLAG_NO_SEEK_ON_EOB', but only for a specific file.
//	                                     * >> Using this, you can restrict the lexer to a sub-space of a file, allowing
//	                                     *    you to safely parse data until the current chunk of a given file ends. */
//	struct TPPFile       *l_eof_file;   /* [0..1] Similar to `l_eob_file', but used for end-of-file instead. */
//	uint32_t              l_flags;      /* A set of `TPPLEXER_FLAG_*' */
//	uint32_t              l_extokens;   /* A set of `TPPLEXER_TOKEN_*' */
//	struct TPPExtState    l_extensions; /* Enabled preprocessor features/extensions. */
//	struct TPPKeywordMap  l_keywords;   /* Hash-map used to map keyword strings to their ids. */
//	struct TPPIncludeList l_syspaths;   /* List of paths searched when looking for system #include files. */
//	size_t                l_limit_mrec; /* Limit for how often a macro may recursively expand into itself. */
//	size_t                l_limit_incl; /* Limit for how often the same text file may exist on the #include stack. */
//	size_t                l_eof_paren;  /* Recursion counter used by the `TPPLEXER_FLAG_EOF_ON_PAREN' flag. */
//	size_t                l_warncount;  /* Amount of warnings that were invoked (including those that were dismissed). */
//	size_t                l_errorcount; /* Amount of errors that were invoked when compilation still continued. */
//	size_t                l_maxerrors;  /* Max amount of errors (`l_errorcount') that should be ignored before they become fatal.
//	                                     * NOTE: Defaults to `TPPLEXER_DEFAULT_LIMIT_ECNT'. */
//	size_t                l_tabsize;    /* Width of `\t' tab characters (used for __COLUMN__ and in error messages).
//	                                     * NOTE: Defaults to `TPPLEXER_DEFAULT_TABSIZE' */
//	struct TPPIfdefStack  l_ifdef;      /* #ifdef stack. */
//	struct TPPWarnings    l_warnings;   /* Current user-configured warnings state. */
//#ifdef TPP_CONFIG_DYN_CALLBACKS
//	struct TPPCallbacks   l_callbacks;  /* User-defined lexer callbacks. */
//#endif /* TPP_CONFIG_DYN_CALLBACKS */
//	TPP(tok_t)            l_noerror;    /* Old token ID before `TPPLEXER_FLAG_ERROR' was set. */
//	TPP(tint_t)           l_counter;    /* Value returned the next time `__COUNTER__' is expanded (Initialized to ZERO(0)). */
//};
//
//#ifndef TPPLEXER_DEFAULT_LIMIT_ECNT
//#define TPPLEXER_DEFAULT_LIMIT_ECNT 16  /* Default value of `l_errorcount' */
//#endif /* !TPPLEXER_DEFAULT_LIMIT_ECNT */
//#ifndef TPPLEXER_DEFAULT_LIMIT_MREC
//#define TPPLEXER_DEFAULT_LIMIT_MREC 512 /* Even when generated text differs from previous version, don't allow more self-recursion per macro than this. */
//#endif /* !TPPLEXER_DEFAULT_LIMIT_MREC */
//#ifndef TPPLEXER_DEFAULT_LIMIT_INCL
//#define TPPLEXER_DEFAULT_LIMIT_INCL 64  /* User attempts to #include a file more often that file will fail with an error message. */
//#endif /* !TPPLEXER_DEFAULT_LIMIT_INCL */
//#ifndef TPPLEXER_DEFAULT_TABSIZE
//#if (defined(_WIN16) || defined(WIN16) || \
//     defined(_WIN32) || defined(WIN32) || \
//     defined(_WIN64) || defined(WIN64) || \
//     defined(__WIN32__) || defined(__TOS_WIN__))
//#define TPPLEXER_DEFAULT_TABSIZE    4   /* Default tab size (used for `__COLUMN__' and in error messages). */
//#else /* Windows... */
//#define TPPLEXER_DEFAULT_TABSIZE    8   /* Default tab size (used for `__COLUMN__' and in error messages). */
//#endif /* Unix... */
//#endif /* !TPPLEXER_DEFAULT_TABSIZE */
//
//#if TPP_CONFIG_ONELEXER == 3
//#define TPPLexer_Current _current
//#elif TPP_CONFIG_ONELEXER
//#define TPPLexer_Current  (&TPPLexer_Global)
//TPPFUN struct TPPLexer TPPLexer_Global;
//#else /* TPP_CONFIG_ONELEXER */
///* [1..1] The currently selected lexer
// * >> When NULL, only certain parts of TPP can work without problems. */
//TPPFUN struct TPPLexer *TPPLexer_Current;
//#endif /* !TPP_CONFIG_ONELEXER */
//
///* Initialize/Finalize the given TPP Lexer object.
// * NOTE: These functions can (obviously) be called when
// *      `TPPLexer_Current' is NULL, or not initialized.
// * @return: 1: Successfully initialized the given lexer.
// * @return: 0: Not enough available memory to setup builtin keywords. (TPP_CONFIG_SET_API_ERROR) */
//TPPFUN int  TPPCALL TPPLexer_Init(struct TPPLexer *__restrict self);
//TPPFUN void TPPCALL TPPLexer_Quit(struct TPPLexer *__restrict self);
//
///* Clear the current ifdef-stack and warn about each entry.
// * @return: 1: Everything was ok, or no critical warning happened.
// * @return: 0: A critical warning happened. */
//TPPFUN int TPPCALL TPPLexer_ClearIfdefStack_(TPP_LEXER_PARAM);
//#define TPPLexer_ClearIfdefStack() TPPLexer_ClearIfdefStack_(TPP_LEXER_ARG)
//
///* Reset certain parts of the lexer.
// * NOTE: This function can be called when `TPPLexer_Current' is NULL, or not initialized.
// * @param: flags: Set of `TPPLEXER_RESET_*' */
//TPPFUN void TPPCALL TPPLexer_Reset(struct TPPLexer *__restrict self, uint32_t flags);
//#define TPPLEXER_RESET_NONE       0x00000000
//#define TPPLEXER_RESET_INCLUDE    0x00000001 /* Reset the #include/#ifdef-stack and set the current token to EOF.
//                                              * NOTE: Also resets the `l_eob_file' and `l_eof_file' special
//                                              *       file pointers, as well as setting `l_noerror' to EOF
//                                              *       and `l_warncount' and `l_errorcount' to ZERO(0). */
//#define TPPLEXER_RESET_ESTATE     0x00000002 /* Reset the current extensions state to mirror the default. */
//#define TPPLEXER_RESET_ESTACK     0x00000004 /* Clear all previously pushed extension states. */
//#define TPPLEXER_RESET_WSTATE     0x00000008 /* Reset the current warning state to mirror the default. */
//#define TPPLEXER_RESET_WSTACK     0x00000010 /* Clear all previously pushed warning states. */
//#define TPPLEXER_RESET_SYSPATHS   0x00000020 /* Clears all system #include-paths. */
//#define TPPLEXER_RESET_MACRO      0x00000040 /* Reset user-defined macros.
//                                              * The original definitions of runtime builtin macros are restored,
//                                              * unless the `TPPLEXER_RESET_NORESTOREMACROS' flag is set. */
//#define TPPLEXER_RESET_ASSERT     0x00000080 /* Reset user-defined assertions. */
//#define TPPLEXER_RESET_KWDFLAGS   0x00000100 /* Reset user-defined keyword flags. */
//#define TPPLEXER_RESET_COUNTER    0x00000200 /* Reset __COUNTER__ and __TPP_COUNTER for all keywords. */
//#define TPPLEXER_RESET_FONCE      0x00000400 /* Clear all `#pragma once' descriptors. */
//#define TPPLEXER_RESET_KEYWORDS   0x00000800 /* Clear all keywords, but keep all predefined.
//                                              * NOTE: When set, this flag implies `TPPLEXER_RESET_MACRO',
//                                              *       `TPPLEXER_RESET_ASSERT', `TPPLEXER_RESET_KWDFLAGS',
//                                              *       `TPPLEXER_RESET_COUNTER' and `TPPLEXER_RESET_FONCE'.
//                                              * NOTE: It also implies `TPPLEXER_RESET_NORESTOREMACROS' */
//#define TPPLEXER_RESET_NORESTOREMACROS 0x00001000 /* When used with `TPPLEXER_RESET_MACRO': Don't restore builtin macro definitions. */
//
//#define TPPLEXER_RESET_EXTENSIONS (TPPLEXER_RESET_ESTATE|TPPLEXER_RESET_ESTACK)
//#define TPPLEXER_RESET_WARNINGS   (TPPLEXER_RESET_WSTATE|TPPLEXER_RESET_WSTACK)
//
///* Push/Pop the current extension state.
// * @return: 0: [TPPLexer_PushExtensions] Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
// * @return: 0: [TPPLexer_PopExtensions] No older extension state was available to restore.
// * @return: 1: Successfully pushed/popped active extensions. */
//TPPFUN int TPPCALL TPPLexer_PushExtensions_(TPP_LEXER_PARAM);
//TPPFUN int TPPCALL TPPLexer_PopExtensions_(TPP_LEXER_PARAM);
//#define TPPLexer_PushExtensions() TPPLexer_PushExtensions_(TPP_LEXER_ARG)
//#define TPPLexer_PopExtensions()  TPPLexer_PopExtensions_(TPP_LEXER_ARG)
//
///* Set the state of a given extension `name'.
// * Extension names attempt to follow gcc names of the same extension.
// * The name of an extension can be found above.
// * @return: 0: Unknown extension.
// * @return: 1: Successfully configured the given extension. */
//TPPFUN int TPPCALL TPPLexer_SetExtension_(TPP_LEXER_PARAM_ char const *__restrict name, int enable);
//#define TPPLexer_SetExtension(name, enable) TPPLexer_SetExtension_(TPP_LEXER_ARG_ name, enable)
//
///* Returns the state of a given extension.
// * @return: -1: Unknown extension.
// * @return:  0: Disabled extension.
// * @return:  1: Enabled extension. */
//TPPFUN int TPPCALL TPPLexer_GetExtension_(TPP_LEXER_PARAM_ char const *__restrict name);
//#define TPPLexer_GetExtension(name) TPPLexer_GetExtension_(TPP_LEXER_ARG_ name)
//
///* Searches the cache and opens a new file if not found.
// * WARNING: If the caller intends to push the file onto the #include-stack,
// *          additional steps must be taken when the file was already
// *          located on the stack (in which case another stream must be opened,
// *          and a file that is not cached must be pushed onto the #include-stack).
// * WARNING: This function may modify the given `filename..filename_size+1' area of memory.
// * @param: pkeyword_entry: When non-NULL, the keyword entry associated with the filename is stored here.
// * @return: * :   A pointer to the already-chached file (WARNING: This is not a reference)
// * @return: NULL: File not found. */
//TPPFUN struct TPPFile *TPPCALL
//TPPLexer_OpenFile_(TPP_LEXER_PARAM_
//                   int mode, char *__restrict filename, size_t filename_size,
//                   struct TPPKeyword **pkeyword_entry);
//#define TPPLexer_OpenFile(mode, filename, filename_size, pkeyword_entry) \
//	TPPLexer_OpenFile_(TPP_LEXER_ARG_ mode, filename, filename_size, pkeyword_entry)
//#define TPPLEXER_OPENFILE_MODE_NORMAL     0x00 /* Normal open (simply pass the given filename to TPPFile_Open, but still sanitize and cache the filename) */
//#define TPPLEXER_OPENFILE_MODE_RELATIVE   0x01 /* #include "foo.h" (Search for the file relative to the path of every text file on the #include-stack in reverse. - If this fails, search in system folders). */
//#define TPPLEXER_OPENFILE_MODE_SYSTEM     0x02 /* #include <stdlib.h> (Search through system folders usually specified with `-I' on the commandline). */
//#define TPPLEXER_OPENFILE_FLAG_NEXT       0x04 /* FLAG: Only open a file not already part of the #include-stack
//                                                * WARNING: May not be used for `TPPLEXER_OPENFILE_MODE_NORMAL'! */
//#define TPPLEXER_OPENFILE_FLAG_NOCASEWARN 0x08 /* FLAG: Don't warn about filename casing on windows. */
//#ifndef TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE
//#define TPPLEXER_OPENFILE_FLAG_NOCALLBACK 0x10 /* FLAG: Don't invoke the unknown-file callback when set. */
//#endif /* !TPP_CONFIG_NO_CALLBACK_UNKNOWN_FILE */
//#define TPPLEXER_OPENFILE_FLAG_CONSTNAME  0x20 /* FLAG: The given `filename' may not be modified, but is guarantied to be '\0'-terminated. */
//
///* Push a given file into the #include-stack of the current lexer.
// * NOTE: These functions never fail and return void.
// * HINT: Call `TPPLexer_PushFileInherited' if you want the lexer to inherit the file.
// * WARNING: The file argument may be evaluated more than once! */
//#define TPPLexer_PushFileInherited(f) TPPLexer_PushFileInherited_(TPPLexer_Current, f)
//#define TPPLexer_PushFile(f)          TPPLexer_PushFile_(TPPLexer_Current, f)
//#define TPPLexer_PushFileInherited_(_current, f)   \
//	(void)((f)->f_prev = _current->l_token.t_file, \
//	       _current->l_token.t_file = (f))
//#define TPPLexer_PushFile_(_current, f) \
//	(TPPFile_Incref(f), TPPLexer_PushFileInherited_(_current, f))
//
///* Returns the currently active #include-file. */
//#define TPPLexer_GetFile() TPPLexer_GetFile_(TPPLexer_Current)
//#define TPPLexer_GetFile_(_current) _current->l_token.t_file
//
///* Pop the current file off of the #include-stack.
// * HINT: This function is save to call, even when the current
// *       file is `TPPFile_Empty' (aka. no files are loaded)
// * WARNING: The caller is responsible never to call this
// *          function when `TPPLexer_GetFile()' has been
// *          configured as either the EOB or EOF lexer-file.
// * NOTE: This function is usually called in a context like:
// *    >> while (TPPLexer_GetFile() != my_file) TPPLexer_PopFile(); */
//TPPFUN void TPPCALL TPPLexer_PopFile_(TPP_LEXER_PARAM);
//#define TPPLexer_PopFile() TPPLexer_PopFile_(TPP_LEXER_ARG)
//
///* Lookup or create a keyword entry for the given name.
// * HINT: TPP also caches files inside the keyword hashmap.
// * @return: NULL: [create_missing]  Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
// * @return: NULL: [!create_missing] No keyword with the given name.
// * @return: * :    The keyword entry associated with the given name. */
//TPPFUN struct TPPKeyword *TPPCALL
//TPPLexer_LookupKeyword_(TPP_LEXER_PARAM_ char const *__restrict name,
//                        size_t namelen, int create_missing);
//TPPFUN struct TPPKeyword *TPPCALL
//TPPLexer_LookupEscapedKeyword_(TPP_LEXER_PARAM_ char const *__restrict name,
//                               size_t namelen, int create_missing);
//#define TPPLexer_LookupKeyword(name, namelen, create_missing) \
//	TPPLexer_LookupKeyword_(TPP_LEXER_ARG_ name, namelen, create_missing)
//#define TPPLexer_LookupEscapedKeyword(name, namelen, create_missing) \
//	TPPLexer_LookupEscapedKeyword_(TPP_LEXER_ARG_ name, namelen, create_missing)
//
///* Looks up a keyword, given its ID
// * WARNING: This function is _extremely_ slow and should only
// *          be used if there is absolutely no other choice. */
//TPPFUN struct TPPKeyword *TPPCALL TPPLexer_LookupKeywordID_(TPP_LEXER_PARAM_ TPP(tok_t) id);
//#define TPPLexer_LookupKeywordID(id) TPPLexer_LookupKeywordID_(TPP_LEXER_ARG_ id)
//
///* Define a regular, keyword-style macro `name' as `value'.
// * @param: flags: A set of `TPPLEXER_DEFINE_FLAG_*'
// * @return: 0: Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
// * @return: 1: Successfully defined the given macro.
// * @return: 2: A macro named `name' was already defined, and was overwritten. */
//TPPFUN int TPPCALL
//TPPLexer_Define_(TPP_LEXER_PARAM_
//                 char const *__restrict name, size_t name_size,
//                 char const *__restrict value, size_t value_size,
//                 uint32_t flags);
//#define TPPLexer_Define(name, name_size, value, value_size, flags) \
//	TPPLexer_Define_(TPP_LEXER_ARG_ name, name_size, value, value_size, flags)
//#define TPPLEXER_DEFINE_FLAG_NONE    0x00000000
//#define TPPLEXER_DEFINE_FLAG_BUILTIN TPP_KEYWORDFLAG_BUILTINMACRO /* Define the macro as builtin, meaning the definition
//                                                                   * set by `value' will be restored when `TPPLexer_Reset()'
//                                                                   * is called with `TPPLEXER_RESET_MACRO'. */
//
///* Undefine the macro associated with a given name.
// * @return: 0: No macro was associated with the given name.
// * @return: 1: Successfully undefined a macro. */
//TPPFUN int TPPCALL TPPLexer_Undef_(TPP_LEXER_PARAM_ char const *__restrict name, size_t name_size);
//#define TPPLexer_Undef(name, name_size) TPPLexer_Undef_(TPP_LEXER_ARG_ name, name_size)
//
//#ifndef TPP_CONFIG_NO_ASSERTIONS
///* Add/Delete a given assertion for a given predicate.
// * @param: answer: [TPPLexer_DelAssert] When NULL, clear all assertions.
// * @return: 0: [TPPLexer_AddAssert] Not enough available memory. (TPP_CONFIG_SET_API_ERROR)
// * @return: 0: [TPPLexer_DelAssert] Unknown/no answer(s)
// * @return: 1: Successfully added/deleted any assertion(s) */
//TPPFUN int TPPCALL
//TPPLexer_AddAssert_(TPP_LEXER_PARAM_
//                    char const *__restrict predicate, size_t predicate_size,
//                    char const *__restrict answer, size_t answer_size);
//TPPFUN int TPPCALL
//TPPLexer_DelAssert_(TPP_LEXER_PARAM_
//                    char const *__restrict predicate, size_t predicate_size,
//                    char const *answer, size_t answer_size);
//#define TPPLexer_AddAssert(predicate, predicate_size, answer, answer_size) \
//	TPPLexer_AddAssert_(TPP_LEXER_ARG_ predicate, predicate_size, answer, answer_size)
//#define TPPLexer_DelAssert(predicate, predicate_size, answer, answer_size) \
//	TPPLexer_DelAssert_(TPP_LEXER_ARG_ predicate, predicate_size, answer, answer_size)
//#endif /* !TPP_CONFIG_NO_ASSERTIONS */
//
///* Similar to `TPPLexer_Yield' and used to implement it, but
// * doesn't expand macros or execute preprocessor directives. */
//TPPFUN TPP(tok_t) TPPCALL TPPLexer_YieldRaw_(TPP_LEXER_PARAM);
//#define TPPLexer_YieldRaw() TPPLexer_YieldRaw_(TPP_LEXER_ARG)
//
///* Similar to `TPPLexer_Yield' and used to
// * implement it, but doesn't expand macros. */
//TPPFUN TPP(tok_t) TPPCALL TPPLexer_YieldPP_(TPP_LEXER_PARAM);
//#define TPPLexer_YieldPP() TPPLexer_YieldPP_(TPP_LEXER_ARG)
//
///* Advance the selected lexer by one token and return the id of the new one.
// * HINT: Returns ZERO(0) if EOF was reached. */
//TPPFUN TPP(tok_t) TPPCALL TPPLexer_Yield_(TPP_LEXER_PARAM);
//#define TPPLexer_Yield() TPPLexer_Yield_(TPP_LEXER_ARG)
//
///* Return non-ZERO if the current token is the first of the current input line.
// * Return ZERO otherwise. */
//TPPFUN int TPPCALL TPPLexer_AtStartOfLine_(TPP_LEXER_PARAM);
//#define TPPLexer_AtStartOfLine() TPPLexer_AtStartOfLine_(TPP_LEXER_ARG)
//
//#ifdef TPP_CONFIG_NONBLOCKING_IO
///* Yield the next token while trying not
// * to block in a non-block enabled file. */
//TPP_LOCAL TPP(tok_t) TPPCALL TPPLexer_YieldRawNB_(TPP_LEXER_PARAM) {
//	TPP(tok_t)
//	result;
//	uint32_t old_flags = TPPLexer_Current->l_flags;
//	TPPLexer_Current->l_flags |= TPPLEXER_FLAG_NONBLOCKING;
//	result = TPPLexer_YieldRaw();
//	TPPLexer_Current->l_flags &= TPPLEXER_FLAG_MERGEMASK;
//	TPPLexer_Current->l_flags |= old_flags & ~TPPLEXER_FLAG_MERGEMASK;
//	return result;
//}
//
//TPP_LOCAL TPP(tok_t) TPPCALL TPPLexer_YieldPPNB_(TPP_LEXER_PARAM) {
//	TPP(tok_t)
//	result;
//	uint32_t old_flags = TPPLexer_Current->l_flags;
//	TPPLexer_Current->l_flags |= TPPLEXER_FLAG_NONBLOCKING;
//	result = TPPLexer_YieldPP();
//	TPPLexer_Current->l_flags &= TPPLEXER_FLAG_MERGEMASK;
//	TPPLexer_Current->l_flags |= old_flags & ~TPPLEXER_FLAG_MERGEMASK;
//	return result;
//}
//
//TPP_LOCAL TPP(tok_t) TPPCALL TPPLexer_YieldNB_(TPP_LEXER_PARAM) {
//	TPP(tok_t)
//	result;
//	uint32_t old_flags = TPPLexer_Current->l_flags;
//	TPPLexer_Current->l_flags |= TPPLEXER_FLAG_NONBLOCKING;
//	result = TPPLexer_Yield();
//	TPPLexer_Current->l_flags &= TPPLEXER_FLAG_MERGEMASK;
//	TPPLexer_Current->l_flags |= old_flags & ~TPPLEXER_FLAG_MERGEMASK;
//	return result;
//}
//#define TPPLexer_YieldRawNB() TPPLexer_YieldRawNB_(TPP_LEXER_ARG)
//#define TPPLexer_YieldPPNB()  TPPLexer_YieldPPNB_(TPP_LEXER_ARG)
//#define TPPLexer_YieldNB()    TPPLexer_YieldNB_(TPP_LEXER_ARG)
//#endif /* TPP_CONFIG_NONBLOCKING_IO */
//
//
//#ifndef TPP_CONFIG_CALLBACK_WARNING
///* Emit a given warning.
// * @return: 0: The warning was critical (TPPLexer_SetErr() was called and you should try to abort)
// * @return: 1: The warning was ignored, suppressed or simply non-fatal. */
//TPPFUN int TPPVCALL TPPLexer_Warn_(TPP_LEXER_PARAM_ int wnum, ...); /* TODO: __attribute__((cold)) */
//#if TPP_CONFIG_ONELEXER == 3
//#define TPPLexer_Warn(...) TPPLexer_Warn_(TPP_LEXER_ARG_ __VA_ARGS__)
//#else /* TPP_CONFIG_ONELEXER == 2 */
//#define TPPLexer_Warn TPPLexer_Warn_
//#endif /* TPP_CONFIG_ONELEXER != 2 */
//#endif /* !TPP_CONFIG_CALLBACK_WARNING */
//
//#undef TPPLexer_SetErr
//#undef TPPLexer_UnsetErr
//
//#define TPPLexer_SetErr_inline()   TPPLexer_SetErr_inline_(TPPLexer_Current)
//#define TPPLexer_UnsetErr_inline() TPPLexer_UnsetErr_inline_(TPPLexer_Current)
//#define TPPLexer_SetErr_inline_(_current)                \
//	((_current->l_flags & TPPLEXER_FLAG_ERROR)           \
//	 ? 0                                                 \
//	 : (_current->l_flags |= TPPLEXER_FLAG_ERROR,        \
//	    _current->l_noerror    = _current->l_token.t_id, \
//	    _current->l_token.t_id = TPP(TOK_ERR), 1))
//#define TPPLexer_UnsetErr_inline_(_current)              \
//	((_current->l_flags & TPPLEXER_FLAG_ERROR)           \
//	 ? (_current->l_flags &= ~TPPLEXER_FLAG_ERROR,       \
//	    _current->l_token.t_id = _current->l_noerror, 1) \
//	 : 0)
//
//
///* Set the lexer into an error-state in which
// * calls to to any yield function return TOK_ERR.
// * >> Called when an unrecoverable error occurrs.
// * HINT: To recover after such an event, `TPPLexer_UnsetErr()' should be called.
// * @return: 0: [TPPLexer_SetErr]   A lexer error was already set.
// *             [TPPLexer_UnsetErr] No lexer error was set.
// * @return: 1: [TPPLexer_SetErr]   Successfully set a lexer error.
// *             [TPPLexer_UnsetErr] Successfully cleared a lexer error. */
//TPPFUN int TPPCALL TPPLexer_SetErr_(TPP_LEXER_PARAM);
//TPPFUN int TPPCALL TPPLexer_UnsetErr_(TPP_LEXER_PARAM);
//#define TPPLexer_SetErr()   TPPLexer_SetErr_(TPP_LEXER_ARG)
//#define TPPLexer_UnsetErr() TPPLexer_UnsetErr_(TPP_LEXER_ARG)
//
//#ifdef TPP_CONFIG_INLINE_SETERR
//#undef TPPLexer_SetErr
//#undef TPPLexer_UnsetErr
//#define TPPLexer_SetErr    TPPLexer_SetErr_inline
//#define TPPLexer_UnsetErr  TPPLexer_UnsetErr_inline
//#endif /* TPP_CONFIG_INLINE_SETERR */
//
///* Called after a given macro was referenced and
// * the associated parenthesis was located.
// * Expected to be called when the current token is the macro's name,
// * this function will parse the macro's argument list remainder of the macro's
// * argument list (including the terminating parenthesis), before 
// * pushing a new file describing the expanded macro onto the include stack.
// * @return: 0: A hard error occurred (such as not enough memory; (TPP_CONFIG_SET_API_ERROR))
// * @return: 1: Successfully expanded the macro.
// * @return: 2: Missing argument list or illegal recursive expansion. */
//TPPFUN int TPPCALL TPPLexer_ExpandFunctionMacro_(TPP_LEXER_PARAM_ struct TPPFile *__restrict macro);
//#define TPPLexer_ExpandFunctionMacro(macro) TPPLexer_ExpandFunctionMacro_(TPP_LEXER_ARG_ macro)
//
//
//struct TPPConst {
//#define TPP_CONST_INTEGRAL 0
//#define TPP_CONST_FLOAT    1
//#define TPP_CONST_STRING   2
//	unsigned int c_kind; /* Constant kind (One of `TPP_CONST_*'). */
//	union {
//		TPP(tint_t)              c_int;    /* [TPP_CONST_INTEGRAL] Integral. */
//		TPP(tfloat_t)            c_float;  /* [TPP_CONST_FLOAT] Floating point. */
//		/*ref*/struct TPPString *c_string; /* [TPP_CONST_STRING][1..1] String. */
//	} c_data;
//};
//#define TPPConst_IsTrue(self)                         \
//	((self)->c_kind == TPP_CONST_STRING               \
//	 ? (TPPString_SIZE((self)->c_data.c_string) != 0) \
//	 : (self)->c_kind == TPP_CONST_FLOAT              \
//	   ? ((self)->c_data.c_float != 0.0L)             \
//	   : ((self)->c_data.c_int != 0))
//#define TPPConst_IsBool(self)                \
//	((self)->c_kind == TPP_CONST_INTEGRAL && \
//	 !((self)->c_data.c_int & ~(TPP(tint_t))1))
//#define TPPConst_AsInt(self)                 \
//	((self)->c_kind == TPP_CONST_INTEGRAL    \
//	 ? (self)->c_data.c_int                  \
//	 : (self)->c_kind == TPP_CONST_FLOAT     \
//	   ? (TPP(tint_t))(self)->c_data.c_float \
//	   : TPPString_SIZE((self)->c_data.c_string) != 0)
//#define TPPConst_AsFloat(self)               \
//	((self)->c_kind == TPP_CONST_FLOAT       \
//	 ? (self)->c_data.c_float                \
//	 : (self)->c_kind == TPP_CONST_INTEGRAL  \
//	   ? (TPP(tfloat_t))(self)->c_data.c_int \
//	   : (TPP(tfloat_t))(TPPString_SIZE((self)->c_data.c_string) != 0))
//#define TPPConst_InitCopy(self, right)                 \
//	do {                                               \
//		*(self) = *(right);                            \
//		if ((self)->c_kind == TPP_CONST_STRING)        \
//			TPPString_Incref((self)->c_data.c_string); \
//	} while (TPP_MACRO_FALSE)
//
//#define TPPConst_ToBool(self)                                            \
//	do {                                                                 \
//		if ((self)->c_kind == TPP_CONST_STRING) {                        \
//			int c_newval = TPPString_SIZE((self)->c_data.c_string) != 0; \
//			TPPString_Decref((self)->c_data.c_string);                   \
//			(self)->c_data.c_int = (TPP(tint_t))c_newval;                \
//			(self)->c_kind       = TPP_CONST_INTEGRAL;                   \
//		} else if ((self)->c_kind == TPP_CONST_FLOAT) {                  \
//			(self)->c_data.c_int = (self)->c_data.c_float != 0.0L;       \
//			(self)->c_kind       = TPP_CONST_INTEGRAL;                   \
//		} else {                                                         \
//			(self)->c_data.c_int = !!(self)->c_data.c_int;               \
//		}                                                                \
//	} while (TPP_MACRO_FALSE)
//#define TPPConst_ToInt(self)                                             \
//	do {                                                                 \
//		if ((self)->c_kind == TPP_CONST_STRING) {                        \
//			int c_newval = TPPString_SIZE((self)->c_data.c_string) != 0; \
//			TPPString_Decref((self)->c_data.c_string);                   \
//			(self)->c_data.c_int = (TPP(tint_t))c_newval;                \
//			(self)->c_kind       = TPP_CONST_INTEGRAL;                   \
//		} else if ((self)->c_kind == TPP_CONST_FLOAT) {                  \
//			(self)->c_data.c_int = (TPP(tint_t))(self)->c_data.c_float;  \
//			(self)->c_kind       = TPP_CONST_INTEGRAL;                   \
//		}                                                                \
//	} while (TPP_MACRO_FALSE)
//
//#define TPPConst_ZERO(self)                           \
//	(void)((self)->c_kind       = TPP_CONST_INTEGRAL, \
//	       (self)->c_data.c_int = 0)
//#define TPPConst_Quit(self)                      \
//	(void)((self)->c_kind != TPP_CONST_STRING || \
//	       (TPPString_Decref((self)->c_data.c_string), 1))
//
///* Convert a given preprocessor constant into a string:
// * >> The returned string can be used to create a file
// *    that represents the constant's value as loaded
// *    by the `__TPP_EVAL' extension.
// * NOTE: If `self' is a string, it will be escaped.
// * @return: NULL: Not enough available memory. */
//TPPFUN /*ref*/struct TPPString *TPPCALL
//TPPConst_ToString_(TPP_LEXER_PARAM_ struct TPPConst const *__restrict self);
//#define TPPConst_ToString(self) TPPConst_ToString_(TPP_LEXER_ARG_ self)
//
///* Evaluate a constant expression as found after `#if' or in `__TPP_EVAL(...)'
// * NOTE: If `result' is NULL, the expression's is
// *       parsed, yet warnings will not be emit.
// * NOTE: Expects the current token to point to the first one part of the expression.
// *       Upon exit, that token will point to the first one past the expression.
// * NOTE: Evaluation is compatible with standard c rules, but
// *       `,' operators are not parsed at the highest level.
// * @return: 1: Successfully parsed an expression
// * @return: 0: An error occurred. */
//TPPFUN int TPPCALL TPPLexer_Eval_(TPP_LEXER_PARAM_ struct TPPConst *result);
//#define TPPLexer_Eval(result) TPPLexer_Eval_(TPP_LEXER_ARG_ result)
//
///* Parse the data block of a pragma.
// * NOTE: `TPPLexer_ParseBuiltinPragma' behaves similar to
// *       `TPPLexer_ParsePragma', but will not invoke a
// *       user-provided pragma handler in the even of an
// *       unknown one.
// * @return: 0: Unknown/errorous pragma.
// * @return: 1: Successfully parsed the given pragma. */
//TPPFUN int TPPCALL TPPLexer_ParsePragma_(TPP_LEXER_PARAM);
//TPPFUN int TPPCALL TPPLexer_ParseBuiltinPragma_(TPP_LEXER_PARAM);
//#define TPPLexer_ParsePragma()        TPPLexer_ParsePragma_(TPP_LEXER_ARG)
//#define TPPLexer_ParseBuiltinPragma() TPPLexer_ParseBuiltinPragma_(TPP_LEXER_ARG)
//
///* Parse an evaluate a string from the current lexer.
// * NOTE: This functions expects the current token to be a string token
// *       and will continue parsing and concat-ing strings until the
// *       next non-string token.
// * @return: * :   A reference to the unescaped string that was parsed.
// * @return: NULL: A lexer error occurred (TPPLexer_SetErr() was set; (TPP_CONFIG_SET_API_ERROR)). */
//#if TPP_UNESCAPE_MAXCHAR == 1
//TPPFUN /*ref*/struct TPPString *TPPCALL TPPLexer_ParseString_(TPP_LEXER_PARAM);
//#define TPPLexer_ParseString() TPPLexer_ParseString_(TPP_LEXER_ARG)
//#else /* TPP_UNESCAPE_MAXCHAR == 1 */
//#define TPPLexer_ParseString() TPPLexer_ParseStringEx_(TPP_LEXER_ARG_ sizeof(char))
//TPPFUN /*ref*/struct TPPString *TPPCALL TPPLexer_ParseStringEx_(TPP_LEXER_PARAM_ size_t sizeof_char);
//#endif /* TPP_UNESCAPE_MAXCHAR != 1 */
//
///* Transform the current token (which must either be `TOK_INT' or `TOK_CHAR')
// * into an integral value, storing that value in `*pint' and returning
// * a set of `TPP_ATOI_*' flags, indicating typing and success.
// * NOTE: This function does _NOT_ yield the current token once finished.
// *       If intended, the caller is responsible for advancing it upon success.
// * @return: TPP_ATOI_ERR: Emiting a warning caused the lexer to error out (TPPLexer_SetErr() was set).
// * @return: * :           A set of `TPP_ATOI_*' (see below) */
//TPPFUN int TPPCALL TPP_Atoi_(TPP_LEXER_PARAM_ TPP(tint_t) *__restrict pint);
//#define TPP_Atoi(pint) TPP_Atoi_(TPP_LEXER_ARG_ pint)
//#define TPP_ATOI_ERR           0x00 /* NOTE: Never used with any flags (indicates failure). */
//#define TPP_ATOI_OK            0x01 /* Always set on success. */
//#define TPP_ATOI_UNSIGNED      0x02 /* Unless set, the integral is signed. */
//#define TPP_ATOI_TYPE_MASK     0xf0 /* Mask of the integral's typing (NOTE: The function already clamped the resulting value with this type's range). */
//#define TPP_ATOI_TYPE_INT      0x00 /* `int' (default typing without suffix/for chars). */
//#define TPP_ATOI_TYPE_LONG     0x10 /* `long'. */
//#define TPP_ATOI_TYPE_LONGLONG 0x20 /* `long long'. */
//#define TPP_ATOI_TYPE_INT8     0x30 /* `__int8' (msvc-extension). */
//#define TPP_ATOI_TYPE_INT16    0x40 /* `__int16' (msvc-extension). */
//#define TPP_ATOI_TYPE_INT32    0x50 /* `__int32' (msvc-extension). */
//#define TPP_ATOI_TYPE_INT64    0x60 /* `__int64' (msvc-extension). */
//
///* Transform the current token (which must be `TOK_FLOAT') into a
// * floating point value, storing that value in `*pfloat' and returning
// * a set of `TPP_ATOF_*' flags, indicating typing and success.
// * NOTE: This function does _NOT_ yield the current token once finished.
// *       If intended, the caller is responsible for advancing it upon success.
// * @return: TPP_ATOF_ERR: Emiting a warning caused the lexer to error out (TPPLexer_SetErr() was set).
// * @return: * :           A set of `TPP_ATOF_*' (see below) */
//TPPFUN int TPPCALL TPP_Atof_(TPP_LEXER_PARAM_ TPP(tfloat_t) *__restrict pfloat);
//#define TPP_Atof(pfloat) TPP_Atof_(TPP_LEXER_ARG_ pfloat)
//#define TPP_ATOF_ERR             0x00 /* NOTE: Never used with any flags (indicates failure). */
//#define TPP_ATOF_OK              0x01 /* Always set on success. */
//#define TPP_ATOF_TYPE_MASK       0xf0 /* Mask of the float's typing. */
//#define TPP_ATOF_TYPE_DOUBLE     0x00 /* `double' (default typing without suffix). */
//#define TPP_ATOF_TYPE_FLOAT      0x10 /* `float' (float-suffix `f') */
//#define TPP_ATOF_TYPE_LONGDOUBLE 0x20 /* `long double' (long-double-suffix `L'). */
//
///* Prints the text contained within the current token, automatically
// * skipping escaped linefeeds and converting di/trigraphs.
// * NOTE: `TPP_PrintComment' behaves similar, but will
// *        instead handle any kind of comment token,
// *        printing the comment text within.
// * @return: >= 0: Sum of all return values from `printer'.
// * @return: <  0: The first negative value returned by `printer' */
//TPPFUN ptrdiff_t TPPCALL TPP_PrintToken_(TPP_LEXER_PARAM_ TPP(printer_t) printer, void *closure);
//TPPFUN ptrdiff_t TPPCALL TPP_PrintComment_(TPP_LEXER_PARAM_ TPP(printer_t) printer, void *closure);
//#define TPP_PrintToken(printer, closure)   TPP_PrintToken_(TPP_LEXER_ARG_ printer, closure)
//#define TPP_PrintComment(printer, closure) TPP_PrintComment_(TPP_LEXER_ARG_ printer, closure)
//
//
///* Helper macros to initialize/finalize the global TPP context.
// * NOTE: These macros can (obviously) be called when
// *      `TPPLexer_Current' is NULL, or not initialized. */
//#if TPP_CONFIG_ONELEXER == 3
///* You have to allocate+TPPLexer_Init()+TPPLexer_Quit() your own lexer(s) */
//#elif TPP_CONFIG_ONELEXER
//#define TPP_INITIALIZE() TPPLexer_Init(&TPPLexer_Global)
//#define TPP_FINALIZE()   TPPLexer_Quit(&TPPLexer_Global)
//#else /* TPP_CONFIG_ONELEXER */
//#define TPP_INITIALIZE()                                                    \
//	(TPPLexer_Current = (struct TPPLexer *)malloc(sizeof(struct TPPLexer)), \
//	 TPPLexer_Current ? (TPPLexer_Init(TPPLexer_Current) ? 1 : (free(TPPLexer_Current), 0)) : 0)
//#define TPP_FINALIZE() (TPPLexer_Quit(TPPLexer_Current), free(TPPLexer_Current))
//#endif /* !TPP_CONFIG_ONELEXER */
//
//
//#ifdef TPP_NAMESPACE_DEFINED
//#undef TPP_NAMESPACE_DEFINED
//#undef TPP
//#endif /* TPP_NAMESPACE_DEFINED */
//
//
///* Fix unnamed union/struct members. */
//#if !TPP_HAVE_UNNAMED_UNION
//#define m_function     m_specific.m_function
//#define m_expand       m_specific.m_expand
//#define f_textfile     f_specific.f_textfile
//#define f_macro        f_specific.f_macro
//#endif /* !TPP_HAVE_UNNAMED_UNION */
#endif

#endif /* !GUARD_TPP2_H */
