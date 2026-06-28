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
#ifndef TPP_DEFS
#error "Must only #include this file while defining 'TPP_DEFS'"
#endif /* !TPP_DEFS */

/*[[[tpp-begin]]]*/


/********************************************************************************
 * Supplying your own keywords/warnings/extensions/etc. to TPP3:
 * >> #define TPP_CONFIG_USERDEFS_FILENAME "../../relative/path/to/your/defs.h"
 *
 * If defined, this is the (relative to "tpp-amalgamation.h") name of a file that
 * should be included in various places to define additional keywords/warnings/
 * extensions/etc. for integration into TPP3.
 ********************************************************************************
 *
 * While inside of this file, the following macros are pre-defined:
 *
 *
 * >> #undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
 *    Defined, but no explicit meaning (for use in "#ifdef TPP_DEFS" to detect context)
 *    If you #include other files for "your/defs.h", you can use this macro prevent
 *    unexpected tokens from appearing within the definitions file.
 *
 *
 * >> #define TPP_KWD(id, string)  <magic>
 *    Define an additional keyword "tpp_token_id" by the name "id".
 *    This keyword token is returned when an identifier equal to "string" was parsed.
 *    Example:
 *       >> TPP_KWD(KWD_function, "function")
 *    Your compiler can then check for the "function" keyword token like:
 *       >> if (tpp_lexer_gettok(LEXER) == KWD_function) {
 *       >>     ...
 *       >> }
 *
 *
 * >> #define TPP_KWD_FLAGS(id, flags_expr)  <magic>
 *    For use with "TPP_KWD": defines flags that should be associated with the keyword
 *    token "id". The given "flags_expr" parameter is allowed to be a runtime expression,
 *    and should evaluate to a set of `TPP_KEYWORD_FLAG_*', and can thus be used to encode
 *    the intended expansion values of builtins like `__has_extension()'.
 *    The specified "flags_expr" has access to the following pre-defined function:
 *       >> tpp_lexer *tpp_current_lexer(void);            // The current lexer
 *       >> tpp_keyword const *tpp_current_keyword(void);  // The current keyword
 *       >> tpp_token_id tpp_current_keyword_id(void);     // The current keyword ID
 *       WARNING: The state of the current lexer is undefined inside of "if_expr"!
 *                Do not appear to yield tokens, or modify "tpp_current_lexer()"
 *                in any way.
 *    Example:
 *       >> // Allow user-code to query support for "#pragma once" using "__has_extension(pragma_once)"
 *       >> TPP_KWD(pragma_once, "pragma_once")
 *       >> TPP_KWD_FLAGS(pragma_once, tpp_lexer_has(tpp_current_lexer(), PRAGMA_ONCE)
 *       >>                            ? TPP_KEYWORD_FLAG_HAS_EXTENSION
 *       >>                            ? 0)
 *    User-code can how evaluate the specified "flags_expr" like so:
 *       >> #if __has_extension(pragma_once)
 *       >> #pragma once
 *       >> #endif
 *
 *
 * >> #define TPP_EXTENSION(id, name, default)  <magic>
 *    Define an extension that can be tested-for using `tpp_lexer_getextension(LEXER, id)',
 *    and can be configured by user-code as `#pragma extension("-f<name>")'. The "default"
 *    argument is a compile-time constant expression specifying if the extension should be
 *    enabled (!= 0) or disabled (== 0) by default.
 *    Example:
 *       >> // "-fmy-cool-expresion" is enabled by default
 *       >> TPP_EXTENSION(EXT_MY_COOL_EXTENSION, "my-cool-expresion", 1)
 *    User-code can now control this extension like so:
 *       >> #pragma extension("-fmy-cool-expresion")    // Enable
 *       >> #pragma extension("-fno-my-cool-expresion") // Disable
 *    During compilation, you can check if the extension is currently enabled:
 *       >> if (tpp_lexer_getextension(LEXER, EXT_MY_COOL_EXTENSION)) {
 *       >>     ...
 *       >> }
 *    When "TPP_HAVE_EXTENSIONS" is disabled, uses of this macro are ignored
 *
 *
 * >> #define TPP_WGROUP(wgroup_id, names, default)  <magic>
 *    Define a warning group identified by "wgroup_id" and addressable by the
 *    specified "names" (which must be a "preprocessor tuple"). The "default"
 *    argument is a compile-time constant expression specifying the warning
 *    group's default "tpp_warning_state" and must evaluate to one of:
 *       - TPP_WSTATE_DISABLED
 *       - TPP_WSTATE_WARN
 *       - TPP_WSTATE_ERROR     (only #if TPP_HAVE_WARNING_ERROR)
 *       - TPP_WSTATE_FATAL
 *       Other values (particularly 'TPP_WSTATE_SUPPRESS') are NOT allowed
 *    Example:
 *       >> // "-fmy-cool-expresion" is enabled by default
 *       >> TPP_WGROUP(WG_BAD_THING_HAPPEND, 2("bad-thing-happened", "same-bad-thing-happened"), TPP_WSTATE_ERROR)
 *    Usercode can then control this warning like:
 *       >> #pragma warning("-Wno-bad-thing-happened")
 *       >> #pragma warning("-Wno-same-bad-thing-happened") // Same as previous line
 *    When "TPP_HAVE_WARNINGS" is disabled, uses of this macro are ignored
 *
 *
 * >> TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)
 * >> TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr)
 *    Defines a warning that can be emitted as "tpp_lexer_warnf(LEXER, warning_id, ...)",
 *    and belongs to the specified "wgroup_ids" (which must be a "preprocessor tuple").
 *    - When "TPP_HAVE_WARNING_NUMBERS" is enabled, "numbers" is another preprocessor
 *      tuple, expanding to the set of "numbers" that can be used to identify this warning
 *      specifically, in which case "numbers_default" is the default state of that warning
 *      specifically.
 *    - When "TPP_HAVE_WARNING_NUMBERS" is disabled, or "numbers" is "0()" (an empty tuple),
 *      the the "numbers_default" parameter is ignored and never used.
 *    - When "TPP_WARNING" is used, "format" is the format string that is returned by
 *      the TPP API function `tpp_warning_getformat()', and is also the string passed to
 *      `tpp_lexer_printf_warning()' when the warning is emitted by `tpp_lexer_warnf()'.
 *      Operands like '%s' that may appear in this format-string are taken from varargs
 *      passed to `tpp_lexer_warnf()' at the time the warning is emitted.
 *    The alternate macro "TPP_WARNING_EX()" behaves the same as "TPP_WARNING()", except
 *    that instead of directly specifying a "format" string literal, an "expr" is supplied,
 *    that is executed (as a block-statement) when the warning is emitted. When this happens,
 *    "expr" has access to the following functions:
 *       >> tpp_lexer *tpp_current_lexer(void);          // The current lext
 *       >> tpp_warning_id tpp_current_warning_id(void); // The warning that is being invoked
 *       >> va_list tpp_current_va_args(void);                   // Variable arguments supplied to warning
 *       >> T tpp_current_va_arg(T);                             // Convenience wrapper for "va_arg(tpp_current_va_arg(), T)"
 *       >> void tpp_print_file_and_line(tpp_file *at_file, tpp_char const *at_pos, tpp_lcinfo at_lc);
 *       >> void tpp_print_file_and_line_at(tpp_file *at_file, tpp_char const *at_pos);
 *       >> void tpp_print_file_and_line_lc(tpp_file *at_file, tpp_lcinfo at_lc);
 *       >> void tpp_warnf0(char const *format);
 *       >> void tpp_warnf1(char const *format, A a);
 *       >> void tpp_warnf2(char const *format, A a, B b);
 *       >> void tpp_warnf3(char const *format, A a, B b, C c);
 *       >> void tpp_warnf4(char const *format, A a, B b, C c, D d);
 *       - Any "return" statement executed must specify some "tpp_errno", and causes
 *         the associated `tpp_lexer_warnf()' to immediately return with that value.
 *    Example:
 *       >> TPP_WARNING(W_BAD_THING_HAPPEND, 1(WG_BAD_THING_HAPPEND), 2(100, 101), "bad thing happened: %s")
 *       >> TPP_WARNING_EX(W_BAD_THING_HAPPEND, 1(WG_BAD_THING_HAPPEND), 2(100, 101), {
 *       >>     char const *message = tpp_current_va_arg(char const *);
 *       >>     tpp_warnf1("same bad thing happened: %s", message);
 *       >> });
 *    During compilation, you can emit this warning like:
 *       >> tpp_errno error = tpp_lexer_warnf(LEXER, W_BAD_THING_HAPPEND, "uh'oh");
 *       >> if (TPP_ISERR(error))
 *       >>     ...;
 *
 *
 * >> #define TPP_MACRO(keyword_id, if_expr)
 *    For use with "TPP_KWD": specifies if the associated keyword should be considered to
 *    be "#if defined(<keyword_id>)" for the purposes of #ifdef-checks, as well as builtin
 *    macro expansions. For this purpose, "if_expr" should be a "bool"-expression.
 *    The specified "if_expr" has access to the following pre-defined function:
 *       >> tpp_lexer *tpp_current_lexer(void);            // The current lexer
 *       >> tpp_keyword const *tpp_current_keyword(void);  // The current keyword
 *       >> tpp_token_id tpp_current_keyword_id(void);     // The current keyword ID
 *       WARNING: The state of the current lexer is undefined inside of "if_expr"!
 *                Do not appear to yield tokens, or modify "tpp_current_lexer()"
 *                in any way.
 *    Example:
 *       >> // Allow user-code to query support for "#pragma once" using "__has_extension(pragma_once)"
 *       >> TPP_KWD(KWD___MY_COOL_EXTENSION_ENABLED__, "__MY_COOL_EXTENSION_ENABLED__")
 *       >> TPP_MACRO(KWD___MY_COOL_EXTENSION_ENABLED__, tpp_lexer_getextension(tpp_current_lexer(), EXT_MY_COOL_EXTENSION))
 *    User-code can now test the state of "EXT_MY_COOL_EXTENSION" like:
 *       >> #ifdef __MY_COOL_EXTENSION_ENABLED__
 *    Hint:
 *       - You can use "container_of" to access structure surrounding TPP's lexer,
 *         allowing you to (safely) access other components of your compiler, thus
 *         enabling user-code to test feature flags you implemented without the use
 *         of TPP's extension system
 *
 ********************************************************************************/



/* Definitions for builtin keywords */
#ifndef TPP_KWD
#define TPP_KWD(id, string)
#endif /* !TPP_KWD */
#ifndef TPP_KWD_FLAGS
#define TPP_KWD_FLAGS(id, flags_expr)
#endif /* !TPP_KWD_FLAGS */

/* Definitions for compiler extensions */
#ifndef TPP_EXTENSION
#define TPP_EXTENSION(id, name, default)
#endif /* !TPP_EXTENSION */

/* Definitions for compiler warnings */
#ifndef TPP_WGROUP
#define TPP_WGROUP(wgroup_id, names, default)
#endif /* !TPP_WGROUP */
#ifndef TPP_WARNING
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format)
#endif /* !TPP_WARNING */
#ifndef TPP_WARNING_EX
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr) \
	TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, ~)
#endif /* !TPP_WARNING_EX */

/* Definitions for builtin macros*/
#ifndef TPP_MACRO
/* Reference a keyword that would behave as a predefined
 * macro only defined when `if' evaluates to true at runtime.
 * NOTE: Such a macro can still be re-defined by the user, but may
 *       later be #undef'd again to restore its original meaning. */
#define TPP_MACRO(keyword_id, if_expr)
#endif /* !TPP_MACRO */
#ifndef TPP_BUILTIN_MACRO
/* Defines the value that a builtin macro without custom behavior
 * should expand to. Without this, the expansion is either controlled
 * via custom behavior (in the case of macros like __FILE__, __LINE__),
 * or as a fallback: the macro will simply expand to itself. */
#define TPP_BUILTIN_MACRO(keyword_id, value)
#endif /* !TPP_BUILTIN_MACRO */


/*[[[deemon
import * from deemon;
local BUILTIN_KEYWORDS: {string: string} = {
	"line": "TPP_HAVE_CPP_LINE",
	"include": "TPP_HAVE_CPP_INCLUDE",
	"include_next": "TPP_HAVE_CPP_INCLUDE_NEXT",
	"import": "TPP_HAVE_CPP_IMPORT",
	"if": "TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS",
	"ifdef": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"ifndef": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"elif": "TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS",
	"elifdef": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"elifndef": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"else": "TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS",
	"endif": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"define": "TPP_HAVE_CPP_DEFINE",
	"undef": "TPP_HAVE_CPP_DEFINE || TPP_HAVE_PRAGMA_PUSH_MACRO",
	"embed": "TPP_HAVE_CPP_EMBED",
	"assert": "TPP_HAVE_CPP_ASSERT",
	"unassert": "TPP_HAVE_CPP_ASSERT",
	"error": "TPP_HAVE_CPP_ERROR || TPP_HAVE_PRAGMA_ERROR || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_GCC_ERROR",
	"warning": "TPP_HAVE_CPP_WARNING || TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_GCC_WARNING",
	"ident": "TPP_HAVE_CPP_IDENT_SCCS",
	"sccs": "TPP_HAVE_CPP_IDENT_SCCS",
	"pragma": "TPP_HAVE_CPP_PRAGMA",
	"_Pragma": "TPP_HAVE_MACRO__Pragma",
	"__pragma": "TPP_HAVE_MACRO___pragma",
	"__has_attribute": "TPP_HAVE_CLANG_MACRO___has_attribute",
	"__has_builtin": "TPP_HAVE_CLANG_MACRO___has_builtin",
	"__has_cpp_attribute": "TPP_HAVE_CLANG_MACRO___has_cpp_attribute",
	"__has_declspec_attribute": "TPP_HAVE_CLANG_MACRO___has_declspec_attribute",
	"__has_extension": "TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension",
	"__has_feature": "TPP_HAVE_CLANG_MACRO___has_feature",
	"__has_c_attribute": "TPP_HAVE_CLANG_MACRO___has_c_attribute",
	"__is_identifier": "TPP_HAVE_MACRO___is_identifier",
	"__is_deprecated": "TPP_HAVE_MACRO___is_deprecated",
	"__is_poisoned": "TPP_HAVE_MACRO___is_poisoned",
	"__has_known_extension": "TPP_HAVE_MACRO___has_known_extension",
	"__has_warning": "TPP_HAVE_MACRO___has_warning",
	"__has_known_warning": "TPP_HAVE_MACRO___has_known_warning",
	"__TPP_VERSION__": "TPP_HAVE_CPP_MACROS",
	"__has_include": "TPP_HAVE_MACRO___has_include",
	"__has_include_next": "TPP_HAVE_MACRO___has_include_next",
	"__has_embed": "TPP_HAVE_MACRO___has_embed",
	"__FILE__": "TPP_HAVE_MACRO___FILE__",
	"__LINE__": "TPP_HAVE_MACRO___LINE__",
	"__TIME__": "TPP_HAVE_MACRO___TIME__",
	"__DATE__": "TPP_HAVE_MACRO___DATE__",
	"__COLUMN__": "TPP_HAVE_MACRO___COLUMN__",
	"__BASE_FILE__": "TPP_HAVE_MACRO___BASE_FILE__",
	"__FILE_NAME__": "TPP_HAVE_MACRO___FILE_NAME__",
	"__INCLUDE_LEVEL__": "TPP_HAVE_MACRO___INCLUDE_LEVEL__",
	"__INCLUDE_DEPTH__": "TPP_HAVE_MACRO___INCLUDE_DEPTH__",
	"__COUNTER__": "TPP_HAVE_MACRO___COUNTER__",
	"__TIMESTAMP__": "TPP_HAVE_MACRO___TIMESTAMP__",
	"__DATE_DAY__": "TPP_HAVE_NUMERIC_DATE_MACROS",
	"__DATE_WDAY__": "TPP_HAVE_NUMERIC_DATE_MACROS",
	"__DATE_YDAY__": "TPP_HAVE_NUMERIC_DATE_MACROS",
	"__DATE_MONTH__": "TPP_HAVE_NUMERIC_DATE_MACROS",
	"__DATE_YEAR__": "TPP_HAVE_NUMERIC_DATE_MACROS",
	"__TIME_SEC__": "TPP_HAVE_NUMERIC_TIME_MACROS",
	"__TIME_MIN__": "TPP_HAVE_NUMERIC_TIME_MACROS",
	"__TIME_HOUR__": "TPP_HAVE_NUMERIC_TIME_MACROS",
	"__TPP_EVAL": "TPP_HAVE_MACRO___TPP_EVAL",
	"__TPP_UNIQUE": "TPP_HAVE_MACRO___TPP_UNIQUE",
	"__TPP_LOAD_FILE": "TPP_HAVE_MACRO___TPP_LOAD_FILE",
	"__TPP_COUNTER": "TPP_HAVE_MACRO___TPP_COUNTER",
	"__TPP_RANDOM": "TPP_HAVE_MACRO___TPP_RANDOM",
	"__TPP_STR_DECOMPILE": "TPP_HAVE_MACRO___TPP_STR_DECOMPILE",
	"__TPP_STR_SUBSTR": "TPP_HAVE_MACRO___TPP_STR_SUBSTR",
	"__TPP_STR_PACK": "TPP_HAVE_MACRO___TPP_STR_PACK",
	"__TPP_STR_SIZE": "TPP_HAVE_MACRO___TPP_STR_SIZE",
	"__TPP_COUNT_TOKENS": "TPP_HAVE_MACRO___TPP_COUNT_TOKENS",
	"__TPP_IDENTIFIER": "TPP_HAVE_MACRO___TPP_IDENTIFIER",
	"__VA_ARGS__": "TPP_HAVE_VA_ARGS_IN_MACROS",
	"__VA_COMMA__": "TPP_HAVE_VA_COMMA_IN_MACROS",
	"__VA_NARGS__": "TPP_HAVE_VA_NARGS_IN_MACROS",
	"__VA_OPT__": "TPP_HAVE_VA_OPT_IN_MACROS",
	"defined": "TPP_HAVE_BUILTIN_EXPR_DEFINED",

	// For #embed and __has_embed
	"limit": "TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed",
	"prefix": "TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed",
	"suffix": "TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed",
	"if_empty": "TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed",
	"__STDC_EMBED_NOT_FOUND__": "TPP_HAVE_MACRO___has_embed",
	"__STDC_EMBED_FOUND__": "TPP_HAVE_MACRO___has_embed",
	"__STDC_EMBED_EMPTY__": "TPP_HAVE_MACRO___has_embed",

	// Pragmas....
	"push_macro": "TPP_HAVE_PRAGMA_PUSH_MACRO",
	"pop_macro": "TPP_HAVE_PRAGMA_PUSH_MACRO",
	"once": "TPP_HAVE_PRAGMA_ONCE",
	"TPP": "TPP_HAVE_PRAGMA_TPP",
	"region": "TPP_HAVE_PRAGMA_REGION",
	"endregion": "TPP_HAVE_PRAGMA_REGION",
	"deprecated": "TPP_HAVE_PRAGMA_DEPRECATED",
	"tpp_exec": "TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC",
	"tpp_set_keyword_flags": "TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS",
	"extension": "TPP_HAVE_PRAGMA_EXTENSION",
	"push": "((TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION) && TPP_HAVE_EXTENSIONS_PUSH_POP) || "
	        "((TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_TPP_WARNING) && TPP_HAVE_WARNINGS_PUSH_POP) || "
	        "(TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_WARNINGS_PUSH_POP)",
	"pop":  "((TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION) && TPP_HAVE_EXTENSIONS_PUSH_POP) || "
	        "((TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_TPP_WARNING) && TPP_HAVE_WARNINGS_PUSH_POP) || "
	        "(TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_WARNINGS_PUSH_POP)",
	"message": "TPP_HAVE_PRAGMA_MESSAGE",
	"GCC": "TPP_HAVE_PRAGMA_GCC",
	"poison": "TPP_HAVE_PRAGMA_GCC_POISON",
	"system_header": "TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER",
	"diagnostic": "TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC",
	"dependency": "TPP_HAVE_PRAGMA_GCC_DEPENDENCY",
	"ignored": "TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC",
	"include_path": "TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH",
	"clear": "TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH",
};
for (local kwd, cond: BUILTIN_KEYWORDS) {
	print("#if ", cond);
	print("#define TPP_KWD_", kwd, " TPP_KWD_", kwd);
	print("TPP_KWD(TPP_KWD_", kwd, ", ", repr(kwd), ")");
	print("#endif /" "* ", cond, " *" "/");
}
]]]*/
#if TPP_HAVE_CPP_LINE
#define TPP_KWD_line TPP_KWD_line
TPP_KWD(TPP_KWD_line, "line")
#endif /* TPP_HAVE_CPP_LINE */
#if TPP_HAVE_CPP_INCLUDE
#define TPP_KWD_include TPP_KWD_include
TPP_KWD(TPP_KWD_include, "include")
#endif /* TPP_HAVE_CPP_INCLUDE */
#if TPP_HAVE_CPP_INCLUDE_NEXT
#define TPP_KWD_include_next TPP_KWD_include_next
TPP_KWD(TPP_KWD_include_next, "include_next")
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
#if TPP_HAVE_CPP_IMPORT
#define TPP_KWD_import TPP_KWD_import
TPP_KWD(TPP_KWD_import, "import")
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#define TPP_KWD_if TPP_KWD_if
TPP_KWD(TPP_KWD_if, "if")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_ifdef TPP_KWD_ifdef
TPP_KWD(TPP_KWD_ifdef, "ifdef")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_ifndef TPP_KWD_ifndef
TPP_KWD(TPP_KWD_ifndef, "ifndef")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#define TPP_KWD_elif TPP_KWD_elif
TPP_KWD(TPP_KWD_elif, "elif")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_elifdef TPP_KWD_elifdef
TPP_KWD(TPP_KWD_elifdef, "elifdef")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_elifndef TPP_KWD_elifndef
TPP_KWD(TPP_KWD_elifndef, "elifndef")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#define TPP_KWD_else TPP_KWD_else
TPP_KWD(TPP_KWD_else, "else")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_endif TPP_KWD_endif
TPP_KWD(TPP_KWD_endif, "endif")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_DEFINE
#define TPP_KWD_define TPP_KWD_define
TPP_KWD(TPP_KWD_define, "define")
#endif /* TPP_HAVE_CPP_DEFINE */
#if TPP_HAVE_CPP_DEFINE || TPP_HAVE_PRAGMA_PUSH_MACRO
#define TPP_KWD_undef TPP_KWD_undef
TPP_KWD(TPP_KWD_undef, "undef")
#endif /* TPP_HAVE_CPP_DEFINE || TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_CPP_EMBED
#define TPP_KWD_embed TPP_KWD_embed
TPP_KWD(TPP_KWD_embed, "embed")
#endif /* TPP_HAVE_CPP_EMBED */
#if TPP_HAVE_CPP_ASSERT
#define TPP_KWD_assert TPP_KWD_assert
TPP_KWD(TPP_KWD_assert, "assert")
#endif /* TPP_HAVE_CPP_ASSERT */
#if TPP_HAVE_CPP_ASSERT
#define TPP_KWD_unassert TPP_KWD_unassert
TPP_KWD(TPP_KWD_unassert, "unassert")
#endif /* TPP_HAVE_CPP_ASSERT */
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_PRAGMA_ERROR || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_GCC_ERROR
#define TPP_KWD_error TPP_KWD_error
TPP_KWD(TPP_KWD_error, "error")
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_PRAGMA_ERROR || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_GCC_ERROR */
#if TPP_HAVE_CPP_WARNING || TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_GCC_WARNING
#define TPP_KWD_warning TPP_KWD_warning
TPP_KWD(TPP_KWD_warning, "warning")
#endif /* TPP_HAVE_CPP_WARNING || TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_GCC_WARNING */
#if TPP_HAVE_CPP_IDENT_SCCS
#define TPP_KWD_ident TPP_KWD_ident
TPP_KWD(TPP_KWD_ident, "ident")
#endif /* TPP_HAVE_CPP_IDENT_SCCS */
#if TPP_HAVE_CPP_IDENT_SCCS
#define TPP_KWD_sccs TPP_KWD_sccs
TPP_KWD(TPP_KWD_sccs, "sccs")
#endif /* TPP_HAVE_CPP_IDENT_SCCS */
#if TPP_HAVE_CPP_PRAGMA
#define TPP_KWD_pragma TPP_KWD_pragma
TPP_KWD(TPP_KWD_pragma, "pragma")
#endif /* TPP_HAVE_CPP_PRAGMA */
#if TPP_HAVE_MACRO__Pragma
#define TPP_KWD__Pragma TPP_KWD__Pragma
TPP_KWD(TPP_KWD__Pragma, "_Pragma")
#endif /* TPP_HAVE_MACRO__Pragma */
#if TPP_HAVE_MACRO___pragma
#define TPP_KWD___pragma TPP_KWD___pragma
TPP_KWD(TPP_KWD___pragma, "__pragma")
#endif /* TPP_HAVE_MACRO___pragma */
#if TPP_HAVE_CLANG_MACRO___has_attribute
#define TPP_KWD___has_attribute TPP_KWD___has_attribute
TPP_KWD(TPP_KWD___has_attribute, "__has_attribute")
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
#define TPP_KWD___has_builtin TPP_KWD___has_builtin
TPP_KWD(TPP_KWD___has_builtin, "__has_builtin")
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
#define TPP_KWD___has_cpp_attribute TPP_KWD___has_cpp_attribute
TPP_KWD(TPP_KWD___has_cpp_attribute, "__has_cpp_attribute")
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
#define TPP_KWD___has_declspec_attribute TPP_KWD___has_declspec_attribute
TPP_KWD(TPP_KWD___has_declspec_attribute, "__has_declspec_attribute")
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension
#define TPP_KWD___has_extension TPP_KWD___has_extension
TPP_KWD(TPP_KWD___has_extension, "__has_extension")
#endif /* TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
#define TPP_KWD___has_feature TPP_KWD___has_feature
TPP_KWD(TPP_KWD___has_feature, "__has_feature")
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
#define TPP_KWD___has_c_attribute TPP_KWD___has_c_attribute
TPP_KWD(TPP_KWD___has_c_attribute, "__has_c_attribute")
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_MACRO___is_identifier
#define TPP_KWD___is_identifier TPP_KWD___is_identifier
TPP_KWD(TPP_KWD___is_identifier, "__is_identifier")
#endif /* TPP_HAVE_MACRO___is_identifier */
#if TPP_HAVE_MACRO___is_deprecated
#define TPP_KWD___is_deprecated TPP_KWD___is_deprecated
TPP_KWD(TPP_KWD___is_deprecated, "__is_deprecated")
#endif /* TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_MACRO___is_poisoned
#define TPP_KWD___is_poisoned TPP_KWD___is_poisoned
TPP_KWD(TPP_KWD___is_poisoned, "__is_poisoned")
#endif /* TPP_HAVE_MACRO___is_poisoned */
#if TPP_HAVE_MACRO___has_known_extension
#define TPP_KWD___has_known_extension TPP_KWD___has_known_extension
TPP_KWD(TPP_KWD___has_known_extension, "__has_known_extension")
#endif /* TPP_HAVE_MACRO___has_known_extension */
#if TPP_HAVE_MACRO___has_warning
#define TPP_KWD___has_warning TPP_KWD___has_warning
TPP_KWD(TPP_KWD___has_warning, "__has_warning")
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
#define TPP_KWD___has_known_warning TPP_KWD___has_known_warning
TPP_KWD(TPP_KWD___has_known_warning, "__has_known_warning")
#endif /* TPP_HAVE_MACRO___has_known_warning */
#if TPP_HAVE_CPP_MACROS
#define TPP_KWD___TPP_VERSION__ TPP_KWD___TPP_VERSION__
TPP_KWD(TPP_KWD___TPP_VERSION__, "__TPP_VERSION__")
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_MACRO___has_include
#define TPP_KWD___has_include TPP_KWD___has_include
TPP_KWD(TPP_KWD___has_include, "__has_include")
#endif /* TPP_HAVE_MACRO___has_include */
#if TPP_HAVE_MACRO___has_include_next
#define TPP_KWD___has_include_next TPP_KWD___has_include_next
TPP_KWD(TPP_KWD___has_include_next, "__has_include_next")
#endif /* TPP_HAVE_MACRO___has_include_next */
#if TPP_HAVE_MACRO___has_embed
#define TPP_KWD___has_embed TPP_KWD___has_embed
TPP_KWD(TPP_KWD___has_embed, "__has_embed")
#endif /* TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_MACRO___FILE__
#define TPP_KWD___FILE__ TPP_KWD___FILE__
TPP_KWD(TPP_KWD___FILE__, "__FILE__")
#endif /* TPP_HAVE_MACRO___FILE__ */
#if TPP_HAVE_MACRO___LINE__
#define TPP_KWD___LINE__ TPP_KWD___LINE__
TPP_KWD(TPP_KWD___LINE__, "__LINE__")
#endif /* TPP_HAVE_MACRO___LINE__ */
#if TPP_HAVE_MACRO___TIME__
#define TPP_KWD___TIME__ TPP_KWD___TIME__
TPP_KWD(TPP_KWD___TIME__, "__TIME__")
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
#define TPP_KWD___DATE__ TPP_KWD___DATE__
TPP_KWD(TPP_KWD___DATE__, "__DATE__")
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___COLUMN__
#define TPP_KWD___COLUMN__ TPP_KWD___COLUMN__
TPP_KWD(TPP_KWD___COLUMN__, "__COLUMN__")
#endif /* TPP_HAVE_MACRO___COLUMN__ */
#if TPP_HAVE_MACRO___BASE_FILE__
#define TPP_KWD___BASE_FILE__ TPP_KWD___BASE_FILE__
TPP_KWD(TPP_KWD___BASE_FILE__, "__BASE_FILE__")
#endif /* TPP_HAVE_MACRO___BASE_FILE__ */
#if TPP_HAVE_MACRO___FILE_NAME__
#define TPP_KWD___FILE_NAME__ TPP_KWD___FILE_NAME__
TPP_KWD(TPP_KWD___FILE_NAME__, "__FILE_NAME__")
#endif /* TPP_HAVE_MACRO___FILE_NAME__ */
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__
#define TPP_KWD___INCLUDE_LEVEL__ TPP_KWD___INCLUDE_LEVEL__
TPP_KWD(TPP_KWD___INCLUDE_LEVEL__, "__INCLUDE_LEVEL__")
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ */
#if TPP_HAVE_MACRO___INCLUDE_DEPTH__
#define TPP_KWD___INCLUDE_DEPTH__ TPP_KWD___INCLUDE_DEPTH__
TPP_KWD(TPP_KWD___INCLUDE_DEPTH__, "__INCLUDE_DEPTH__")
#endif /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
#if TPP_HAVE_MACRO___COUNTER__
#define TPP_KWD___COUNTER__ TPP_KWD___COUNTER__
TPP_KWD(TPP_KWD___COUNTER__, "__COUNTER__")
#endif /* TPP_HAVE_MACRO___COUNTER__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
#define TPP_KWD___TIMESTAMP__ TPP_KWD___TIMESTAMP__
TPP_KWD(TPP_KWD___TIMESTAMP__, "__TIMESTAMP__")
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
#if TPP_HAVE_NUMERIC_DATE_MACROS
#define TPP_KWD___DATE_DAY__ TPP_KWD___DATE_DAY__
TPP_KWD(TPP_KWD___DATE_DAY__, "__DATE_DAY__")
#endif /* TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_DATE_MACROS
#define TPP_KWD___DATE_WDAY__ TPP_KWD___DATE_WDAY__
TPP_KWD(TPP_KWD___DATE_WDAY__, "__DATE_WDAY__")
#endif /* TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_DATE_MACROS
#define TPP_KWD___DATE_YDAY__ TPP_KWD___DATE_YDAY__
TPP_KWD(TPP_KWD___DATE_YDAY__, "__DATE_YDAY__")
#endif /* TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_DATE_MACROS
#define TPP_KWD___DATE_MONTH__ TPP_KWD___DATE_MONTH__
TPP_KWD(TPP_KWD___DATE_MONTH__, "__DATE_MONTH__")
#endif /* TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_DATE_MACROS
#define TPP_KWD___DATE_YEAR__ TPP_KWD___DATE_YEAR__
TPP_KWD(TPP_KWD___DATE_YEAR__, "__DATE_YEAR__")
#endif /* TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
#define TPP_KWD___TIME_SEC__ TPP_KWD___TIME_SEC__
TPP_KWD(TPP_KWD___TIME_SEC__, "__TIME_SEC__")
#endif /* TPP_HAVE_NUMERIC_TIME_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
#define TPP_KWD___TIME_MIN__ TPP_KWD___TIME_MIN__
TPP_KWD(TPP_KWD___TIME_MIN__, "__TIME_MIN__")
#endif /* TPP_HAVE_NUMERIC_TIME_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
#define TPP_KWD___TIME_HOUR__ TPP_KWD___TIME_HOUR__
TPP_KWD(TPP_KWD___TIME_HOUR__, "__TIME_HOUR__")
#endif /* TPP_HAVE_NUMERIC_TIME_MACROS */
#if TPP_HAVE_MACRO___TPP_EVAL
#define TPP_KWD___TPP_EVAL TPP_KWD___TPP_EVAL
TPP_KWD(TPP_KWD___TPP_EVAL, "__TPP_EVAL")
#endif /* TPP_HAVE_MACRO___TPP_EVAL */
#if TPP_HAVE_MACRO___TPP_UNIQUE
#define TPP_KWD___TPP_UNIQUE TPP_KWD___TPP_UNIQUE
TPP_KWD(TPP_KWD___TPP_UNIQUE, "__TPP_UNIQUE")
#endif /* TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_LOAD_FILE
#define TPP_KWD___TPP_LOAD_FILE TPP_KWD___TPP_LOAD_FILE
TPP_KWD(TPP_KWD___TPP_LOAD_FILE, "__TPP_LOAD_FILE")
#endif /* TPP_HAVE_MACRO___TPP_LOAD_FILE */
#if TPP_HAVE_MACRO___TPP_COUNTER
#define TPP_KWD___TPP_COUNTER TPP_KWD___TPP_COUNTER
TPP_KWD(TPP_KWD___TPP_COUNTER, "__TPP_COUNTER")
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
#if TPP_HAVE_MACRO___TPP_RANDOM
#define TPP_KWD___TPP_RANDOM TPP_KWD___TPP_RANDOM
TPP_KWD(TPP_KWD___TPP_RANDOM, "__TPP_RANDOM")
#endif /* TPP_HAVE_MACRO___TPP_RANDOM */
#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE
#define TPP_KWD___TPP_STR_DECOMPILE TPP_KWD___TPP_STR_DECOMPILE
TPP_KWD(TPP_KWD___TPP_STR_DECOMPILE, "__TPP_STR_DECOMPILE")
#endif /* TPP_HAVE_MACRO___TPP_STR_DECOMPILE */
#if TPP_HAVE_MACRO___TPP_STR_SUBSTR
#define TPP_KWD___TPP_STR_SUBSTR TPP_KWD___TPP_STR_SUBSTR
TPP_KWD(TPP_KWD___TPP_STR_SUBSTR, "__TPP_STR_SUBSTR")
#endif /* TPP_HAVE_MACRO___TPP_STR_SUBSTR */
#if TPP_HAVE_MACRO___TPP_STR_PACK
#define TPP_KWD___TPP_STR_PACK TPP_KWD___TPP_STR_PACK
TPP_KWD(TPP_KWD___TPP_STR_PACK, "__TPP_STR_PACK")
#endif /* TPP_HAVE_MACRO___TPP_STR_PACK */
#if TPP_HAVE_MACRO___TPP_STR_SIZE
#define TPP_KWD___TPP_STR_SIZE TPP_KWD___TPP_STR_SIZE
TPP_KWD(TPP_KWD___TPP_STR_SIZE, "__TPP_STR_SIZE")
#endif /* TPP_HAVE_MACRO___TPP_STR_SIZE */
#if TPP_HAVE_MACRO___TPP_COUNT_TOKENS
#define TPP_KWD___TPP_COUNT_TOKENS TPP_KWD___TPP_COUNT_TOKENS
TPP_KWD(TPP_KWD___TPP_COUNT_TOKENS, "__TPP_COUNT_TOKENS")
#endif /* TPP_HAVE_MACRO___TPP_COUNT_TOKENS */
#if TPP_HAVE_MACRO___TPP_IDENTIFIER
#define TPP_KWD___TPP_IDENTIFIER TPP_KWD___TPP_IDENTIFIER
TPP_KWD(TPP_KWD___TPP_IDENTIFIER, "__TPP_IDENTIFIER")
#endif /* TPP_HAVE_MACRO___TPP_IDENTIFIER */
#if TPP_HAVE_VA_ARGS_IN_MACROS
#define TPP_KWD___VA_ARGS__ TPP_KWD___VA_ARGS__
TPP_KWD(TPP_KWD___VA_ARGS__, "__VA_ARGS__")
#endif /* TPP_HAVE_VA_ARGS_IN_MACROS */
#if TPP_HAVE_VA_COMMA_IN_MACROS
#define TPP_KWD___VA_COMMA__ TPP_KWD___VA_COMMA__
TPP_KWD(TPP_KWD___VA_COMMA__, "__VA_COMMA__")
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS */
#if TPP_HAVE_VA_NARGS_IN_MACROS
#define TPP_KWD___VA_NARGS__ TPP_KWD___VA_NARGS__
TPP_KWD(TPP_KWD___VA_NARGS__, "__VA_NARGS__")
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */
#if TPP_HAVE_VA_OPT_IN_MACROS
#define TPP_KWD___VA_OPT__ TPP_KWD___VA_OPT__
TPP_KWD(TPP_KWD___VA_OPT__, "__VA_OPT__")
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */
#if TPP_HAVE_BUILTIN_EXPR_DEFINED
#define TPP_KWD_defined TPP_KWD_defined
TPP_KWD(TPP_KWD_defined, "defined")
#endif /* TPP_HAVE_BUILTIN_EXPR_DEFINED */
#if TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed
#define TPP_KWD_limit TPP_KWD_limit
TPP_KWD(TPP_KWD_limit, "limit")
#endif /* TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed
#define TPP_KWD_prefix TPP_KWD_prefix
TPP_KWD(TPP_KWD_prefix, "prefix")
#endif /* TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed
#define TPP_KWD_suffix TPP_KWD_suffix
TPP_KWD(TPP_KWD_suffix, "suffix")
#endif /* TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed
#define TPP_KWD_if_empty TPP_KWD_if_empty
TPP_KWD(TPP_KWD_if_empty, "if_empty")
#endif /* TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_MACRO___has_embed
#define TPP_KWD___STDC_EMBED_NOT_FOUND__ TPP_KWD___STDC_EMBED_NOT_FOUND__
TPP_KWD(TPP_KWD___STDC_EMBED_NOT_FOUND__, "__STDC_EMBED_NOT_FOUND__")
#endif /* TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_MACRO___has_embed
#define TPP_KWD___STDC_EMBED_FOUND__ TPP_KWD___STDC_EMBED_FOUND__
TPP_KWD(TPP_KWD___STDC_EMBED_FOUND__, "__STDC_EMBED_FOUND__")
#endif /* TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_MACRO___has_embed
#define TPP_KWD___STDC_EMBED_EMPTY__ TPP_KWD___STDC_EMBED_EMPTY__
TPP_KWD(TPP_KWD___STDC_EMBED_EMPTY__, "__STDC_EMBED_EMPTY__")
#endif /* TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_PRAGMA_PUSH_MACRO
#define TPP_KWD_push_macro TPP_KWD_push_macro
TPP_KWD(TPP_KWD_push_macro, "push_macro")
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_PRAGMA_PUSH_MACRO
#define TPP_KWD_pop_macro TPP_KWD_pop_macro
TPP_KWD(TPP_KWD_pop_macro, "pop_macro")
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_PRAGMA_ONCE
#define TPP_KWD_once TPP_KWD_once
TPP_KWD(TPP_KWD_once, "once")
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_PRAGMA_TPP
#define TPP_KWD_TPP TPP_KWD_TPP
TPP_KWD(TPP_KWD_TPP, "TPP")
#endif /* TPP_HAVE_PRAGMA_TPP */
#if TPP_HAVE_PRAGMA_REGION
#define TPP_KWD_region TPP_KWD_region
TPP_KWD(TPP_KWD_region, "region")
#endif /* TPP_HAVE_PRAGMA_REGION */
#if TPP_HAVE_PRAGMA_REGION
#define TPP_KWD_endregion TPP_KWD_endregion
TPP_KWD(TPP_KWD_endregion, "endregion")
#endif /* TPP_HAVE_PRAGMA_REGION */
#if TPP_HAVE_PRAGMA_DEPRECATED
#define TPP_KWD_deprecated TPP_KWD_deprecated
TPP_KWD(TPP_KWD_deprecated, "deprecated")
#endif /* TPP_HAVE_PRAGMA_DEPRECATED */
#if TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC
#define TPP_KWD_tpp_exec TPP_KWD_tpp_exec
TPP_KWD(TPP_KWD_tpp_exec, "tpp_exec")
#endif /* TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC */
#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
#define TPP_KWD_tpp_set_keyword_flags TPP_KWD_tpp_set_keyword_flags
TPP_KWD(TPP_KWD_tpp_set_keyword_flags, "tpp_set_keyword_flags")
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */
#if TPP_HAVE_PRAGMA_EXTENSION
#define TPP_KWD_extension TPP_KWD_extension
TPP_KWD(TPP_KWD_extension, "extension")
#endif /* TPP_HAVE_PRAGMA_EXTENSION */
#if ((TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION) && TPP_HAVE_EXTENSIONS_PUSH_POP) || ((TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_TPP_WARNING) && TPP_HAVE_WARNINGS_PUSH_POP) || (TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_WARNINGS_PUSH_POP)
#define TPP_KWD_push TPP_KWD_push
TPP_KWD(TPP_KWD_push, "push")
#endif /* ((TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION) && TPP_HAVE_EXTENSIONS_PUSH_POP) || ((TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_TPP_WARNING) && TPP_HAVE_WARNINGS_PUSH_POP) || (TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_WARNINGS_PUSH_POP) */
#if ((TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION) && TPP_HAVE_EXTENSIONS_PUSH_POP) || ((TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_TPP_WARNING) && TPP_HAVE_WARNINGS_PUSH_POP) || (TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_WARNINGS_PUSH_POP)
#define TPP_KWD_pop TPP_KWD_pop
TPP_KWD(TPP_KWD_pop, "pop")
#endif /* ((TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION) && TPP_HAVE_EXTENSIONS_PUSH_POP) || ((TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC || TPP_HAVE_PRAGMA_TPP_WARNING) && TPP_HAVE_WARNINGS_PUSH_POP) || (TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH && TPP_HAVE_WARNINGS_PUSH_POP) */
#if TPP_HAVE_PRAGMA_MESSAGE
#define TPP_KWD_message TPP_KWD_message
TPP_KWD(TPP_KWD_message, "message")
#endif /* TPP_HAVE_PRAGMA_MESSAGE */
#if TPP_HAVE_PRAGMA_GCC
#define TPP_KWD_GCC TPP_KWD_GCC
TPP_KWD(TPP_KWD_GCC, "GCC")
#endif /* TPP_HAVE_PRAGMA_GCC */
#if TPP_HAVE_PRAGMA_GCC_POISON
#define TPP_KWD_poison TPP_KWD_poison
TPP_KWD(TPP_KWD_poison, "poison")
#endif /* TPP_HAVE_PRAGMA_GCC_POISON */
#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#define TPP_KWD_system_header TPP_KWD_system_header
TPP_KWD(TPP_KWD_system_header, "system_header")
#endif /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */
#if TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC
#define TPP_KWD_diagnostic TPP_KWD_diagnostic
TPP_KWD(TPP_KWD_diagnostic, "diagnostic")
#endif /* TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */
#if TPP_HAVE_PRAGMA_GCC_DEPENDENCY
#define TPP_KWD_dependency TPP_KWD_dependency
TPP_KWD(TPP_KWD_dependency, "dependency")
#endif /* TPP_HAVE_PRAGMA_GCC_DEPENDENCY */
#if TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC
#define TPP_KWD_ignored TPP_KWD_ignored
TPP_KWD(TPP_KWD_ignored, "ignored")
#endif /* TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */
#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
#define TPP_KWD_include_path TPP_KWD_include_path
TPP_KWD(TPP_KWD_include_path, "include_path")
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */
#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
#define TPP_KWD_clear TPP_KWD_clear
TPP_KWD(TPP_KWD_clear, "clear")
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */
/*[[[end]]]*/


/* Pre-defined macros... */
#if TPP_HAVE_CPP_MACROS
TPP_MACRO(TPP_KWD___TPP_VERSION__, true)
TPP_BUILTIN_MACRO(TPP_KWD___TPP_VERSION__, TPP_PREPROCESSOR_VERSION_STR)
#if TPP_HAVE_MACRO__Pragma /* GCC defines _Pragma as a macro... So I'll just do that as well! */
TPP_MACRO(TPP_KWD__Pragma, tpp_lexer_has(tpp_current_lexer(), MACRO__Pragma))
#endif /* TPP_HAVE_MACRO__Pragma */
#if TPP_HAVE_MACRO___pragma /* I promised this would be defined as a macro... */
TPP_MACRO(TPP_KWD___pragma, tpp_lexer_has(tpp_current_lexer(), MACRO___pragma))
#endif /* TPP_HAVE_MACRO___pragma */

/* Declare magic, built-in macros */
#if TPP_HAVE_CLANG_MACRO___has_attribute
TPP_MACRO(TPP_KWD___has_attribute, tpp_lexer_has(tpp_current_lexer(), CLANG_MACRO___has_attribute))
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
TPP_MACRO(TPP_KWD___has_builtin, tpp_lexer_has(tpp_current_lexer(), CLANG_MACRO___has_builtin))
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
TPP_MACRO(TPP_KWD___has_cpp_attribute, tpp_lexer_has(tpp_current_lexer(), CLANG_MACRO___has_cpp_attribute))
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
TPP_MACRO(TPP_KWD___has_declspec_attribute, tpp_lexer_has(tpp_current_lexer(), CLANG_MACRO___has_declspec_attribute))
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension
TPP_MACRO(TPP_KWD___has_extension, tpp_lexer_has(tpp_current_lexer(), CLANG_MACRO___has_extension) ||
                                   tpp_lexer_has(tpp_current_lexer(), MACRO___has_extension))
#endif /* TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
TPP_MACRO(TPP_KWD___has_feature, tpp_lexer_has(tpp_current_lexer(), CLANG_MACRO___has_feature))
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
TPP_MACRO(TPP_KWD___has_c_attribute, tpp_lexer_has(tpp_current_lexer(), CLANG_MACRO___has_c_attribute))
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_MACRO___is_identifier
TPP_MACRO(TPP_KWD___is_identifier, tpp_lexer_has(tpp_current_lexer(), MACRO___is_identifier))
#endif /* TPP_HAVE_MACRO___is_identifier */
#if TPP_HAVE_MACRO___is_deprecated
TPP_MACRO(TPP_KWD___is_deprecated, tpp_lexer_has(tpp_current_lexer(), MACRO___is_deprecated))
#endif /* TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_MACRO___is_poisoned
TPP_MACRO(TPP_KWD___is_poisoned, tpp_lexer_has(tpp_current_lexer(), MACRO___is_poisoned))
#endif /* TPP_HAVE_MACRO___is_poisoned */
#if TPP_HAVE_MACRO___has_known_extension
TPP_MACRO(TPP_KWD___has_known_extension, tpp_lexer_has(tpp_current_lexer(), MACRO___has_known_extension))
#endif /* TPP_HAVE_MACRO___has_known_extension */
#if TPP_HAVE_MACRO___has_warning
TPP_MACRO(TPP_KWD___has_warning, tpp_lexer_has(tpp_current_lexer(), MACRO___has_warning))
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
TPP_MACRO(TPP_KWD___has_known_warning, tpp_lexer_has(tpp_current_lexer(), MACRO___has_known_warning))
#endif /* TPP_HAVE_MACRO___has_known_warning */
#if TPP_HAVE_MACRO___has_include
TPP_MACRO(TPP_KWD___has_include, tpp_lexer_has(tpp_current_lexer(), MACRO___has_include))
#endif /* TPP_HAVE_MACRO___has_include */
#if TPP_HAVE_MACRO___has_include_next
TPP_MACRO(TPP_KWD___has_include_next, tpp_lexer_has(tpp_current_lexer(), MACRO___has_include_next))
#endif /* TPP_HAVE_MACRO___has_include_next */
#if TPP_HAVE_MACRO___has_embed
TPP_MACRO(TPP_KWD___has_embed, tpp_lexer_has(tpp_current_lexer(), MACRO___has_embed))
TPP_MACRO(TPP_KWD___STDC_EMBED_NOT_FOUND__, tpp_lexer_has(tpp_current_lexer(), MACRO___has_embed))
TPP_MACRO(TPP_KWD___STDC_EMBED_FOUND__, tpp_lexer_has(tpp_current_lexer(), MACRO___has_embed))
TPP_MACRO(TPP_KWD___STDC_EMBED_EMPTY__, tpp_lexer_has(tpp_current_lexer(), MACRO___has_embed))
TPP_BUILTIN_MACRO(TPP_KWD___STDC_EMBED_NOT_FOUND__, TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND)
TPP_BUILTIN_MACRO(TPP_KWD___STDC_EMBED_FOUND__, TPP_CONFIG_VALUEOF_STDC_EMBED_FOUND)
TPP_BUILTIN_MACRO(TPP_KWD___STDC_EMBED_EMPTY__, TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY)
#endif /* TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_MACRO___FILE__
TPP_MACRO(TPP_KWD___FILE__, tpp_lexer_has(tpp_current_lexer(), MACRO___FILE__))
#endif /* TPP_HAVE_MACRO___FILE__ */
#if TPP_HAVE_MACRO___LINE__
TPP_MACRO(TPP_KWD___LINE__, tpp_lexer_has(tpp_current_lexer(), MACRO___LINE__))
#endif /* TPP_HAVE_MACRO___LINE__ */
#if TPP_HAVE_MACRO___TIME__
TPP_MACRO(TPP_KWD___TIME__, tpp_lexer_has(tpp_current_lexer(), MACRO___TIME__))
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
TPP_MACRO(TPP_KWD___DATE__, tpp_lexer_has(tpp_current_lexer(), MACRO___DATE__))
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___COLUMN__
TPP_MACRO(TPP_KWD___COLUMN__, tpp_lexer_has(tpp_current_lexer(), MACRO___COLUMN__))
#endif /* TPP_HAVE_MACRO___COLUMN__ */
#if TPP_HAVE_MACRO___BASE_FILE__
TPP_MACRO(TPP_KWD___BASE_FILE__, tpp_lexer_has(tpp_current_lexer(), MACRO___BASE_FILE__))
#endif /* TPP_HAVE_MACRO___BASE_FILE__ */
#if TPP_HAVE_MACRO___FILE_NAME__
TPP_MACRO(TPP_KWD___FILE_NAME__, tpp_lexer_has(tpp_current_lexer(), MACRO___FILE_NAME__))
#endif /* TPP_HAVE_MACRO___FILE_NAME__ */
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__
TPP_MACRO(TPP_KWD___INCLUDE_LEVEL__, tpp_lexer_has(tpp_current_lexer(), MACRO___INCLUDE_LEVEL__))
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ */
#if TPP_HAVE_MACRO___INCLUDE_DEPTH__
TPP_MACRO(TPP_KWD___INCLUDE_DEPTH__, tpp_lexer_has(tpp_current_lexer(), MACRO___INCLUDE_DEPTH__))
#endif /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
#if TPP_HAVE_MACRO___COUNTER__
TPP_MACRO(TPP_KWD___COUNTER__, tpp_lexer_has(tpp_current_lexer(), MACRO___COUNTER__))
#endif /* TPP_HAVE_MACRO___COUNTER__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
TPP_MACRO(TPP_KWD___TIMESTAMP__, tpp_lexer_has(tpp_current_lexer(), MACRO___TIMESTAMP__))
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
#if TPP_HAVE_NUMERIC_DATE_MACROS
TPP_MACRO(TPP_KWD___DATE_DAY__, tpp_lexer_has(tpp_current_lexer(), NUMERIC_DATE_MACROS))
TPP_MACRO(TPP_KWD___DATE_WDAY__, tpp_lexer_has(tpp_current_lexer(), NUMERIC_DATE_MACROS))
TPP_MACRO(TPP_KWD___DATE_YDAY__, tpp_lexer_has(tpp_current_lexer(), NUMERIC_DATE_MACROS))
TPP_MACRO(TPP_KWD___DATE_MONTH__, tpp_lexer_has(tpp_current_lexer(), NUMERIC_DATE_MACROS))
TPP_MACRO(TPP_KWD___DATE_YEAR__, tpp_lexer_has(tpp_current_lexer(), NUMERIC_DATE_MACROS))
#endif /* TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
TPP_MACRO(TPP_KWD___TIME_SEC__, tpp_lexer_has(tpp_current_lexer(), NUMERIC_TIME_MACROS))
TPP_MACRO(TPP_KWD___TIME_MIN__, tpp_lexer_has(tpp_current_lexer(), NUMERIC_TIME_MACROS))
TPP_MACRO(TPP_KWD___TIME_HOUR__, tpp_lexer_has(tpp_current_lexer(), NUMERIC_TIME_MACROS))
#endif /* TPP_HAVE_NUMERIC_TIME_MACROS */
#if TPP_HAVE_MACRO___TPP_EVAL
TPP_MACRO(TPP_KWD___TPP_EVAL, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_EVAL))
#endif /* TPP_HAVE_MACRO___TPP_EVAL */
#if TPP_HAVE_MACRO___TPP_UNIQUE
TPP_MACRO(TPP_KWD___TPP_UNIQUE, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_UNIQUE))
#endif /* TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_LOAD_FILE
TPP_MACRO(TPP_KWD___TPP_LOAD_FILE, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_LOAD_FILE))
#endif /* TPP_HAVE_MACRO___TPP_LOAD_FILE */
#if TPP_HAVE_MACRO___TPP_COUNTER
TPP_MACRO(TPP_KWD___TPP_COUNTER, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_COUNTER))
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
#if TPP_HAVE_MACRO___TPP_RANDOM
TPP_MACRO(TPP_KWD___TPP_RANDOM, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_RANDOM))
#endif /* TPP_HAVE_MACRO___TPP_RANDOM */
#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE
TPP_MACRO(TPP_KWD___TPP_STR_DECOMPILE, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_STR_DECOMPILE))
#endif /* TPP_HAVE_MACRO___TPP_STR_DECOMPILE */
#if TPP_HAVE_MACRO___TPP_STR_SUBSTR
TPP_MACRO(TPP_KWD___TPP_STR_SUBSTR, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_STR_SUBSTR))
#endif /* TPP_HAVE_MACRO___TPP_STR_SUBSTR */
#if TPP_HAVE_MACRO___TPP_STR_PACK
TPP_MACRO(TPP_KWD___TPP_STR_PACK, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_STR_PACK))
#endif /* TPP_HAVE_MACRO___TPP_STR_PACK */
#if TPP_HAVE_MACRO___TPP_STR_SIZE
TPP_MACRO(TPP_KWD___TPP_STR_SIZE, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_STR_SIZE))
#endif /* TPP_HAVE_MACRO___TPP_STR_SIZE */
#if TPP_HAVE_MACRO___TPP_COUNT_TOKENS
TPP_MACRO(TPP_KWD___TPP_COUNT_TOKENS, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_COUNT_TOKENS))
#endif /* TPP_HAVE_MACRO___TPP_COUNT_TOKENS */
#if TPP_HAVE_MACRO___TPP_IDENTIFIER
TPP_MACRO(TPP_KWD___TPP_IDENTIFIER, tpp_lexer_has(tpp_current_lexer(), MACRO___TPP_IDENTIFIER))
#endif /* TPP_HAVE_MACRO___TPP_IDENTIFIER */
#endif /* TPP_HAVE_CPP_MACROS */




/* TODO: Feature-test macros (for __has_extension()) */
#if 0
HAS_EXTENSION_IF(tpp_dollar_is_alpha,              HAVE_EXTENSION_DOLLAR_IS_ALPHA)
HAS_EXTENSION_IF(tpp_current_va_args,                      HAVE_EXTENSION_VA_ARGS)
HAS_EXTENSION_IF(tpp_named_va_args,                HAVE_EXTENSION_GCC_VA_ARGS)
HAS_EXTENSION_IF(tpp_va_comma,                     HAVE_EXTENSION_VA_COMMA)
HAS_EXTENSION_IF(tpp_msvc_integer_suffix,          HAVE_EXTENSION_MSVC_FIXED_INT)
HAS_EXTENSION_IF(tpp_charize_operator,             HAVE_EXTENSION_HASH_AT)
HAS_EXTENSION_IF(tpp_trigraphs,                    HAVE_FEATURE_TRIGRAPHS)
HAS_EXTENSION_IF(tpp_digraphs,                     HAVE_FEATURE_DIGRAPHS)
HAS_EXTENSION_IF(tpp_reemit_unknown_pragmas,       (TPPLexer_Current->l_flags & TPPLEXER_FLAG_REEMIT_UNKNOWN_PRAGMA))
HAS_EXTENSION_IF(tpp_pragma_push_macro,            TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_pop_macro,             TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_region,                TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_endregion,             TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_warning,               TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_message,               TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_error,                 TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_once,                  TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_tpp_exec,              TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_deprecated,            TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_pragma_tpp_set_keyword_flags, TPP_PREPROCESSOR_VERSION >= 200)
HAS_EXTENSION_IF(tpp_directive_include_next,       HAVE_EXTENSION_INCLUDE_NEXT)
HAS_EXTENSION_IF(tpp_directive_import,             HAVE_EXTENSION_IMPORT)
HAS_EXTENSION_IF(tpp_directive_warning,            HAVE_EXTENSION_WARNING)
HAS_EXTENSION_IF(tpp_lxor,                         HAVE_EXTENSION_LXOR)
HAS_EXTENSION_IF(tpp_token_tilde_tilde,            TPPLexer_Current->l_extokens & TPPLEXER_TOKEN_TILDETILDE)
HAS_EXTENSION_IF(tpp_token_pow,                    TPPLexer_Current->l_extokens & TPPLEXER_TOKEN_STARSTAR)
HAS_EXTENSION_IF(tpp_token_lxor,                   TPPLexer_Current->l_extokens & TPPLEXER_TOKEN_ROOFROOF)
HAS_EXTENSION_IF(tpp_token_arrow,                  TPPLexer_Current->l_extokens & TPPLEXER_TOKEN_ARROW)
HAS_EXTENSION_IF(tpp_token_colon_assign,           TPPLexer_Current->l_extokens & TPPLEXER_TOKEN_COLONASSIGN)
HAS_EXTENSION_IF(tpp_token_colon_colon,            TPPLexer_Current->l_extokens & TPPLEXER_TOKEN_COLONCOLON)
HAS_EXTENSION_IF(tpp_macro_calling_conventions,    HAVE_EXTENSION_ALTMAC)
HAS_EXTENSION_IF(tpp_strict_whitespace,            HAVE_EXTENSION_ARGSPACE)
HAS_EXTENSION_IF(tpp_strict_integer_overflow,      TPP_WSTATE_ISENABLED(TPPLexer_GetWarning(W_INTEGRAL_OVERFLOW)) || TPP_WSTATE_ISENABLED(TPPLexer_GetWarning(W_INTEGRAL_CLAMPED)))
HAS_EXTENSION_IF(tpp_support_ansi_characters,      0) /* TODO: (Re-)add support for this. */
HAS_EXTENSION_IF(tpp_emit_lf_after_directive,      TPPLexer_Current->l_flags & TPPLEXER_FLAG_DIRECTIVE_NOOWN_LF)
HAS_EXTENSION_IF(tpp_if_cond_expression,           HAVE_EXTENSION_IFELSE_IN_EXPR)
HAS_EXTENSION_IF(tpp_debug,                        TPP_CONFIG_DEBUG)
#endif

/*[[[deemon
import * from deemon;
for (local line: File.open("config.h", "rb").read().decode("utf-8").splitlines(false)) {
	local TPP_HAVE_FOO, extName;
	try {
		TPP_HAVE_FOO, extName = line.rescanf(r'#\s*define\s+(\w+)\s*[^/]+/\*\s*"-f([^"]+)"')...;
	} catch (...) {
		continue;
	}
	local CONF = TPP_HAVE_FOO.lsstrip("TPP_HAVE_");
	local TPP_EXT_FOO = "TPP_EXT_" + CONF;
	local TPP_EXTNAME_FOO = "TPP_EXTNAME_" + CONF;
	print("#if TPP_CONF_IS_EXT(", TPP_HAVE_FOO, ")");
	print("#ifndef ", TPP_EXTNAME_FOO);
	print("#define ", TPP_EXTNAME_FOO, " ", repr(extName));
	print("#endif /" "* !", TPP_EXTNAME_FOO, " *" "/");
	print("#define ", TPP_EXT_FOO, " ", TPP_EXT_FOO);
	print("TPP_EXTENSION(", TPP_EXT_FOO, ", ", TPP_EXTNAME_FOO, ", TPP_CONF_DEFAULT(", TPP_HAVE_FOO, "))");
	print("#define _tpp_lexer_has_", CONF, "(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_", TPP_EXT_FOO, ")");
	print("#endif /" "* TPP_CONF_IS_EXT(", TPP_HAVE_FOO, ") *" "/");
}
]]]*/
#if TPP_CONF_IS_EXT(TPP_HAVE_TRIGRAPHS)
#ifndef TPP_EXTNAME_TRIGRAPHS
#define TPP_EXTNAME_TRIGRAPHS "trigraphs"
#endif /* !TPP_EXTNAME_TRIGRAPHS */
#define TPP_EXT_TRIGRAPHS TPP_EXT_TRIGRAPHS
TPP_EXTENSION(TPP_EXT_TRIGRAPHS, TPP_EXTNAME_TRIGRAPHS, TPP_CONF_DEFAULT(TPP_HAVE_TRIGRAPHS))
#define _tpp_lexer_has_TRIGRAPHS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TRIGRAPHS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TRIGRAPHS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_DIGRAPHS)
#ifndef TPP_EXTNAME_DIGRAPHS
#define TPP_EXTNAME_DIGRAPHS "digraphs"
#endif /* !TPP_EXTNAME_DIGRAPHS */
#define TPP_EXT_DIGRAPHS TPP_EXT_DIGRAPHS
TPP_EXTENSION(TPP_EXT_DIGRAPHS, TPP_EXTNAME_DIGRAPHS, TPP_CONF_DEFAULT(TPP_HAVE_DIGRAPHS))
#define _tpp_lexer_has_DIGRAPHS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_DIGRAPHS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_DIGRAPHS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LF)
#ifndef TPP_EXTNAME_TPP_TOK_LF
#define TPP_EXTNAME_TPP_TOK_LF "tok-lf"
#endif /* !TPP_EXTNAME_TPP_TOK_LF */
#define TPP_EXT_TPP_TOK_LF TPP_EXT_TPP_TOK_LF
TPP_EXTENSION(TPP_EXT_TPP_TOK_LF, TPP_EXTNAME_TPP_TOK_LF, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LF))
#define _tpp_lexer_has_TPP_TOK_LF(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LF)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LF) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SPACE)
#ifndef TPP_EXTNAME_TPP_TOK_SPACE
#define TPP_EXTNAME_TPP_TOK_SPACE "tok-space"
#endif /* !TPP_EXTNAME_TPP_TOK_SPACE */
#define TPP_EXT_TPP_TOK_SPACE TPP_EXT_TPP_TOK_SPACE
TPP_EXTENSION(TPP_EXT_TPP_TOK_SPACE, TPP_EXTNAME_TPP_TOK_SPACE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SPACE))
#define _tpp_lexer_has_TPP_TOK_SPACE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_SPACE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SPACE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_COMMENT)
#ifndef TPP_EXTNAME_TPP_TOK_COMMENT
#define TPP_EXTNAME_TPP_TOK_COMMENT "tok-comment"
#endif /* !TPP_EXTNAME_TPP_TOK_COMMENT */
#define TPP_EXT_TPP_TOK_COMMENT TPP_EXT_TPP_TOK_COMMENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_COMMENT, TPP_EXTNAME_TPP_TOK_COMMENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_COMMENT))
#define _tpp_lexer_has_TPP_TOK_COMMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_COMMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_COMMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_COMMENT)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_COMMENT
#define TPP_EXTNAME_TPP_TOK_CXX_COMMENT "tok-cxx-comment"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_COMMENT */
#define TPP_EXT_TPP_TOK_CXX_COMMENT TPP_EXT_TPP_TOK_CXX_COMMENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_COMMENT, TPP_EXTNAME_TPP_TOK_CXX_COMMENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_COMMENT))
#define _tpp_lexer_has_TPP_TOK_CXX_COMMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_COMMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_COMMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_C_COMMENT)
#ifndef TPP_EXTNAME_TPP_TOK_C_COMMENT
#define TPP_EXTNAME_TPP_TOK_C_COMMENT "tok-c-comment"
#endif /* !TPP_EXTNAME_TPP_TOK_C_COMMENT */
#define TPP_EXT_TPP_TOK_C_COMMENT TPP_EXT_TPP_TOK_C_COMMENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_C_COMMENT, TPP_EXTNAME_TPP_TOK_C_COMMENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_C_COMMENT))
#define _tpp_lexer_has_TPP_TOK_C_COMMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_C_COMMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_C_COMMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PASCAL_COMMENT)
#ifndef TPP_EXTNAME_TPP_TOK_PASCAL_COMMENT
#define TPP_EXTNAME_TPP_TOK_PASCAL_COMMENT "tok-pascal-comment"
#endif /* !TPP_EXTNAME_TPP_TOK_PASCAL_COMMENT */
#define TPP_EXT_TPP_TOK_PASCAL_COMMENT TPP_EXT_TPP_TOK_PASCAL_COMMENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_PASCAL_COMMENT, TPP_EXTNAME_TPP_TOK_PASCAL_COMMENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PASCAL_COMMENT))
#define _tpp_lexer_has_TPP_TOK_PASCAL_COMMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_PASCAL_COMMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PASCAL_COMMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SHELL_COMMENT)
#ifndef TPP_EXTNAME_TPP_TOK_SHELL_COMMENT
#define TPP_EXTNAME_TPP_TOK_SHELL_COMMENT "tok-shell-comment"
#endif /* !TPP_EXTNAME_TPP_TOK_SHELL_COMMENT */
#define TPP_EXT_TPP_TOK_SHELL_COMMENT TPP_EXT_TPP_TOK_SHELL_COMMENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_SHELL_COMMENT, TPP_EXTNAME_TPP_TOK_SHELL_COMMENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SHELL_COMMENT))
#define _tpp_lexer_has_TPP_TOK_SHELL_COMMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_SHELL_COMMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SHELL_COMMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_ASM_COMMENT)
#ifndef TPP_EXTNAME_TPP_TOK_ASM_COMMENT
#define TPP_EXTNAME_TPP_TOK_ASM_COMMENT "tok-asm-comment"
#endif /* !TPP_EXTNAME_TPP_TOK_ASM_COMMENT */
#define TPP_EXT_TPP_TOK_ASM_COMMENT TPP_EXT_TPP_TOK_ASM_COMMENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_ASM_COMMENT, TPP_EXTNAME_TPP_TOK_ASM_COMMENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_ASM_COMMENT))
#define _tpp_lexer_has_TPP_TOK_ASM_COMMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_ASM_COMMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_ASM_COMMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SQL_COMMENT)
#ifndef TPP_EXTNAME_TPP_TOK_SQL_COMMENT
#define TPP_EXTNAME_TPP_TOK_SQL_COMMENT "tok-sql-comment"
#endif /* !TPP_EXTNAME_TPP_TOK_SQL_COMMENT */
#define TPP_EXT_TPP_TOK_SQL_COMMENT TPP_EXT_TPP_TOK_SQL_COMMENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_SQL_COMMENT, TPP_EXTNAME_TPP_TOK_SQL_COMMENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SQL_COMMENT))
#define _tpp_lexer_has_TPP_TOK_SQL_COMMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_SQL_COMMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SQL_COMMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_DOLLAR)
#ifndef TPP_EXTNAME_TPP_TOK_DOLLAR
#define TPP_EXTNAME_TPP_TOK_DOLLAR "tok-dollar"
#endif /* !TPP_EXTNAME_TPP_TOK_DOLLAR */
#define TPP_EXT_TPP_TOK_DOLLAR TPP_EXT_TPP_TOK_DOLLAR
TPP_EXTENSION(TPP_EXT_TPP_TOK_DOLLAR, TPP_EXTNAME_TPP_TOK_DOLLAR, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_DOLLAR))
#define _tpp_lexer_has_TPP_TOK_DOLLAR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_DOLLAR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_DOLLAR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_INT)
#ifndef TPP_EXTNAME_TPP_TOK_INT
#define TPP_EXTNAME_TPP_TOK_INT "tok-int"
#endif /* !TPP_EXTNAME_TPP_TOK_INT */
#define TPP_EXT_TPP_TOK_INT TPP_EXT_TPP_TOK_INT
TPP_EXTENSION(TPP_EXT_TPP_TOK_INT, TPP_EXTNAME_TPP_TOK_INT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_INT))
#define _tpp_lexer_has_TPP_TOK_INT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_INT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_INT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_FLOAT)
#ifndef TPP_EXTNAME_TPP_TOK_FLOAT
#define TPP_EXTNAME_TPP_TOK_FLOAT "tok-float"
#endif /* !TPP_EXTNAME_TPP_TOK_FLOAT */
#define TPP_EXT_TPP_TOK_FLOAT TPP_EXT_TPP_TOK_FLOAT
TPP_EXTENSION(TPP_EXT_TPP_TOK_FLOAT, TPP_EXTNAME_TPP_TOK_FLOAT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_FLOAT))
#define _tpp_lexer_has_TPP_TOK_FLOAT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_FLOAT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_FLOAT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CHAR)
#ifndef TPP_EXTNAME_TPP_TOK_CHAR
#define TPP_EXTNAME_TPP_TOK_CHAR "tok-char"
#endif /* !TPP_EXTNAME_TPP_TOK_CHAR */
#define TPP_EXT_TPP_TOK_CHAR TPP_EXT_TPP_TOK_CHAR
TPP_EXTENSION(TPP_EXT_TPP_TOK_CHAR, TPP_EXTNAME_TPP_TOK_CHAR, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CHAR))
#define _tpp_lexer_has_TPP_TOK_CHAR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CHAR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CHAR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STRING)
#ifndef TPP_EXTNAME_TPP_TOK_STRING
#define TPP_EXTNAME_TPP_TOK_STRING "tok-string"
#endif /* !TPP_EXTNAME_TPP_TOK_STRING */
#define TPP_EXT_TPP_TOK_STRING TPP_EXT_TPP_TOK_STRING
TPP_EXTENSION(TPP_EXT_TPP_TOK_STRING, TPP_EXTNAME_TPP_TOK_STRING, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STRING))
#define _tpp_lexer_has_TPP_TOK_STRING(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_STRING)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STRING) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_RAW_STRING_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_RAW_STRING_LITERAL "tok-cxx-raw-string-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_RAW_STRING_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_RAW_STRING_LITERAL TPP_EXT_TPP_TOK_CXX_RAW_STRING_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_RAW_STRING_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_RAW_STRING_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_RAW_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_RAW_STRING_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_WIDE_STRING_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_WIDE_STRING_LITERAL "tok-cxx-wide-string-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_WIDE_STRING_LITERAL TPP_EXT_TPP_TOK_CXX_WIDE_STRING_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_WIDE_STRING_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_WIDE_STRING_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_WIDE_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_WIDE_STRING_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_UTF8_STRING_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_UTF8_STRING_LITERAL "tok-cxx-utf8-string-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_UTF8_STRING_LITERAL TPP_EXT_TPP_TOK_CXX_UTF8_STRING_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_UTF8_STRING_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_UTF8_STRING_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_UTF8_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_UTF8_STRING_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_UTF16_STRING_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_UTF16_STRING_LITERAL "tok-cxx-utf16-string-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_UTF16_STRING_LITERAL TPP_EXT_TPP_TOK_CXX_UTF16_STRING_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_UTF16_STRING_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_UTF16_STRING_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_UTF16_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_UTF16_STRING_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_UTF32_STRING_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_UTF32_STRING_LITERAL "tok-cxx-utf32-string-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_UTF32_STRING_LITERAL TPP_EXT_TPP_TOK_CXX_UTF32_STRING_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_UTF32_STRING_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_UTF32_STRING_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_UTF32_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_UTF32_STRING_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_RAW_CHAR_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_RAW_CHAR_LITERAL "tok-cxx-raw-char-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_RAW_CHAR_LITERAL TPP_EXT_TPP_TOK_CXX_RAW_CHAR_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_RAW_CHAR_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_RAW_CHAR_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_RAW_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_RAW_CHAR_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_WIDE_CHAR_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_WIDE_CHAR_LITERAL "tok-cxx-wide-char-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_WIDE_CHAR_LITERAL TPP_EXT_TPP_TOK_CXX_WIDE_CHAR_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_WIDE_CHAR_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_WIDE_CHAR_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_WIDE_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_WIDE_CHAR_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_UTF8_CHAR_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_UTF8_CHAR_LITERAL "tok-cxx-utf8-char-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_UTF8_CHAR_LITERAL TPP_EXT_TPP_TOK_CXX_UTF8_CHAR_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_UTF8_CHAR_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_UTF8_CHAR_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_UTF8_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_UTF8_CHAR_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_UTF16_CHAR_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_UTF16_CHAR_LITERAL "tok-cxx-utf16-char-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_UTF16_CHAR_LITERAL TPP_EXT_TPP_TOK_CXX_UTF16_CHAR_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_UTF16_CHAR_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_UTF16_CHAR_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_UTF16_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_UTF16_CHAR_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_CXX_UTF32_CHAR_LITERAL
#define TPP_EXTNAME_TPP_TOK_CXX_UTF32_CHAR_LITERAL "tok-cxx-utf32-char-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
#define TPP_EXT_TPP_TOK_CXX_UTF32_CHAR_LITERAL TPP_EXT_TPP_TOK_CXX_UTF32_CHAR_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_CXX_UTF32_CHAR_LITERAL, TPP_EXTNAME_TPP_TOK_CXX_UTF32_CHAR_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL))
#define _tpp_lexer_has_TPP_TOK_CXX_UTF32_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_CXX_UTF32_CHAR_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_RAW_STRING_LITERAL
#define TPP_EXTNAME_TPP_TOK_RAW_STRING_LITERAL "tok-raw-string-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_RAW_STRING_LITERAL */
#define TPP_EXT_TPP_TOK_RAW_STRING_LITERAL TPP_EXT_TPP_TOK_RAW_STRING_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_RAW_STRING_LITERAL, TPP_EXTNAME_TPP_TOK_RAW_STRING_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL))
#define _tpp_lexer_has_TPP_TOK_RAW_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RAW_STRING_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_RAW_CHAR_LITERAL
#define TPP_EXTNAME_TPP_TOK_RAW_CHAR_LITERAL "tok-raw-char-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_RAW_CHAR_LITERAL */
#define TPP_EXT_TPP_TOK_RAW_CHAR_LITERAL TPP_EXT_TPP_TOK_RAW_CHAR_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_RAW_CHAR_LITERAL, TPP_EXTNAME_TPP_TOK_RAW_CHAR_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL))
#define _tpp_lexer_has_TPP_TOK_RAW_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RAW_CHAR_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_BLOCK_STRING_LITERAL
#define TPP_EXTNAME_TPP_TOK_BLOCK_STRING_LITERAL "tok-block-string-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_BLOCK_STRING_LITERAL */
#define TPP_EXT_TPP_TOK_BLOCK_STRING_LITERAL TPP_EXT_TPP_TOK_BLOCK_STRING_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_BLOCK_STRING_LITERAL, TPP_EXTNAME_TPP_TOK_BLOCK_STRING_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL))
#define _tpp_lexer_has_TPP_TOK_BLOCK_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_BLOCK_STRING_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL)
#ifndef TPP_EXTNAME_TPP_TOK_BLOCK_CHAR_LITERAL
#define TPP_EXTNAME_TPP_TOK_BLOCK_CHAR_LITERAL "tok-block-char-literal"
#endif /* !TPP_EXTNAME_TPP_TOK_BLOCK_CHAR_LITERAL */
#define TPP_EXT_TPP_TOK_BLOCK_CHAR_LITERAL TPP_EXT_TPP_TOK_BLOCK_CHAR_LITERAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_BLOCK_CHAR_LITERAL, TPP_EXTNAME_TPP_TOK_BLOCK_CHAR_LITERAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL))
#define _tpp_lexer_has_TPP_TOK_BLOCK_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_BLOCK_CHAR_LITERAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_STRING_ALLOW_MULTILINE)
#ifndef TPP_EXTNAME_STRING_ALLOW_MULTILINE
#define TPP_EXTNAME_STRING_ALLOW_MULTILINE "string-allow-multiline"
#endif /* !TPP_EXTNAME_STRING_ALLOW_MULTILINE */
#define TPP_EXT_STRING_ALLOW_MULTILINE TPP_EXT_STRING_ALLOW_MULTILINE
TPP_EXTENSION(TPP_EXT_STRING_ALLOW_MULTILINE, TPP_EXTNAME_STRING_ALLOW_MULTILINE, TPP_CONF_DEFAULT(TPP_HAVE_STRING_ALLOW_MULTILINE))
#define _tpp_lexer_has_STRING_ALLOW_MULTILINE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_STRING_ALLOW_MULTILINE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_STRING_ALLOW_MULTILINE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_STRING_AUTO_CONCAT)
#ifndef TPP_EXTNAME_STRING_AUTO_CONCAT
#define TPP_EXTNAME_STRING_AUTO_CONCAT "string-auto-concat"
#endif /* !TPP_EXTNAME_STRING_AUTO_CONCAT */
#define TPP_EXT_STRING_AUTO_CONCAT TPP_EXT_STRING_AUTO_CONCAT
TPP_EXTENSION(TPP_EXT_STRING_AUTO_CONCAT, TPP_EXTNAME_STRING_AUTO_CONCAT, TPP_CONF_DEFAULT(TPP_HAVE_STRING_AUTO_CONCAT))
#define _tpp_lexer_has_STRING_AUTO_CONCAT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_STRING_AUTO_CONCAT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_STRING_AUTO_CONCAT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM)
#ifndef TPP_EXTNAME_TPP_TOK_EXCLAIM_EXCLAIM
#define TPP_EXTNAME_TPP_TOK_EXCLAIM_EXCLAIM "tok-exclaim_exclaim"
#endif /* !TPP_EXTNAME_TPP_TOK_EXCLAIM_EXCLAIM */
#define TPP_EXT_TPP_TOK_EXCLAIM_EXCLAIM TPP_EXT_TPP_TOK_EXCLAIM_EXCLAIM
TPP_EXTENSION(TPP_EXT_TPP_TOK_EXCLAIM_EXCLAIM, TPP_EXTNAME_TPP_TOK_EXCLAIM_EXCLAIM, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM))
#define _tpp_lexer_has_TPP_TOK_EXCLAIM_EXCLAIM(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EXCLAIM_EXCLAIM)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_EXCLAIM_EQUAL
#define TPP_EXTNAME_TPP_TOK_EXCLAIM_EQUAL "tok-exclaim_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_EXCLAIM_EQUAL */
#define TPP_EXT_TPP_TOK_EXCLAIM_EQUAL TPP_EXT_TPP_TOK_EXCLAIM_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_EXCLAIM_EQUAL, TPP_EXTNAME_TPP_TOK_EXCLAIM_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL))
#define _tpp_lexer_has_TPP_TOK_EXCLAIM_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EXCLAIM_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#define TPP_EXTNAME_TPP_TOK_EXCLAIM_EQUAL_EQUAL "tok-exclaim_equal_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#define TPP_EXT_TPP_TOK_EXCLAIM_EQUAL_EQUAL TPP_EXT_TPP_TOK_EXCLAIM_EQUAL_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_EXCLAIM_EQUAL_EQUAL, TPP_EXTNAME_TPP_TOK_EXCLAIM_EQUAL_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL))
#define _tpp_lexer_has_TPP_TOK_EXCLAIM_EQUAL_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EXCLAIM_EQUAL_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_POUND_POUND)
#ifndef TPP_EXTNAME_TPP_TOK_POUND_POUND
#define TPP_EXTNAME_TPP_TOK_POUND_POUND "tok-pound_pound"
#endif /* !TPP_EXTNAME_TPP_TOK_POUND_POUND */
#define TPP_EXT_TPP_TOK_POUND_POUND TPP_EXT_TPP_TOK_POUND_POUND
TPP_EXTENSION(TPP_EXT_TPP_TOK_POUND_POUND, TPP_EXTNAME_TPP_TOK_POUND_POUND, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_POUND_POUND))
#define _tpp_lexer_has_TPP_TOK_POUND_POUND(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_POUND_POUND)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_POUND_POUND) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PERCENT_PERCENT)
#ifndef TPP_EXTNAME_TPP_TOK_PERCENT_PERCENT
#define TPP_EXTNAME_TPP_TOK_PERCENT_PERCENT "tok-percent_percent"
#endif /* !TPP_EXTNAME_TPP_TOK_PERCENT_PERCENT */
#define TPP_EXT_TPP_TOK_PERCENT_PERCENT TPP_EXT_TPP_TOK_PERCENT_PERCENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_PERCENT_PERCENT, TPP_EXTNAME_TPP_TOK_PERCENT_PERCENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PERCENT_PERCENT))
#define _tpp_lexer_has_TPP_TOK_PERCENT_PERCENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_PERCENT_PERCENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PERCENT_PERCENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_PERCENT_PERCENT_EQUAL
#define TPP_EXTNAME_TPP_TOK_PERCENT_PERCENT_EQUAL "tok-percent_percent_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_PERCENT_PERCENT_EQUAL */
#define TPP_EXT_TPP_TOK_PERCENT_PERCENT_EQUAL TPP_EXT_TPP_TOK_PERCENT_PERCENT_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_PERCENT_PERCENT_EQUAL, TPP_EXTNAME_TPP_TOK_PERCENT_PERCENT_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL))
#define _tpp_lexer_has_TPP_TOK_PERCENT_PERCENT_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_PERCENT_PERCENT_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PERCENT_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_PERCENT_EQUAL
#define TPP_EXTNAME_TPP_TOK_PERCENT_EQUAL "tok-percent_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_PERCENT_EQUAL */
#define TPP_EXT_TPP_TOK_PERCENT_EQUAL TPP_EXT_TPP_TOK_PERCENT_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_PERCENT_EQUAL, TPP_EXTNAME_TPP_TOK_PERCENT_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PERCENT_EQUAL))
#define _tpp_lexer_has_TPP_TOK_PERCENT_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_PERCENT_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PERCENT_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AMP_AMP)
#ifndef TPP_EXTNAME_TPP_TOK_AMP_AMP
#define TPP_EXTNAME_TPP_TOK_AMP_AMP "tok-amp_amp"
#endif /* !TPP_EXTNAME_TPP_TOK_AMP_AMP */
#define TPP_EXT_TPP_TOK_AMP_AMP TPP_EXT_TPP_TOK_AMP_AMP
TPP_EXTENSION(TPP_EXT_TPP_TOK_AMP_AMP, TPP_EXTNAME_TPP_TOK_AMP_AMP, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_AMP_AMP))
#define _tpp_lexer_has_TPP_TOK_AMP_AMP(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_AMP_AMP)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AMP_AMP) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AMP_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_AMP_EQUAL
#define TPP_EXTNAME_TPP_TOK_AMP_EQUAL "tok-amp_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_AMP_EQUAL */
#define TPP_EXT_TPP_TOK_AMP_EQUAL TPP_EXT_TPP_TOK_AMP_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_AMP_EQUAL, TPP_EXTNAME_TPP_TOK_AMP_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_AMP_EQUAL))
#define _tpp_lexer_has_TPP_TOK_AMP_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_AMP_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AMP_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_STAR)
#ifndef TPP_EXTNAME_TPP_TOK_STAR_STAR
#define TPP_EXTNAME_TPP_TOK_STAR_STAR "tok-star_star"
#endif /* !TPP_EXTNAME_TPP_TOK_STAR_STAR */
#define TPP_EXT_TPP_TOK_STAR_STAR TPP_EXT_TPP_TOK_STAR_STAR
TPP_EXTENSION(TPP_EXT_TPP_TOK_STAR_STAR, TPP_EXTNAME_TPP_TOK_STAR_STAR, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_STAR))
#define _tpp_lexer_has_TPP_TOK_STAR_STAR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_STAR_STAR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_STAR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_STAR_STAR_EQUAL
#define TPP_EXTNAME_TPP_TOK_STAR_STAR_EQUAL "tok-star_star_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_STAR_STAR_EQUAL */
#define TPP_EXT_TPP_TOK_STAR_STAR_EQUAL TPP_EXT_TPP_TOK_STAR_STAR_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_STAR_STAR_EQUAL, TPP_EXTNAME_TPP_TOK_STAR_STAR_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL))
#define _tpp_lexer_has_TPP_TOK_STAR_STAR_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_STAR_STAR_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_DOT)
#ifndef TPP_EXTNAME_TPP_TOK_STAR_DOT
#define TPP_EXTNAME_TPP_TOK_STAR_DOT "tok-star_dot"
#endif /* !TPP_EXTNAME_TPP_TOK_STAR_DOT */
#define TPP_EXT_TPP_TOK_STAR_DOT TPP_EXT_TPP_TOK_STAR_DOT
TPP_EXTENSION(TPP_EXT_TPP_TOK_STAR_DOT, TPP_EXTNAME_TPP_TOK_STAR_DOT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_DOT))
#define _tpp_lexer_has_TPP_TOK_STAR_DOT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_STAR_DOT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_DOT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS)
#ifndef TPP_EXTNAME_TPP_TOK_STAR_LANGLE_MINUS
#define TPP_EXTNAME_TPP_TOK_STAR_LANGLE_MINUS "tok-star_langle_minus"
#endif /* !TPP_EXTNAME_TPP_TOK_STAR_LANGLE_MINUS */
#define TPP_EXT_TPP_TOK_STAR_LANGLE_MINUS TPP_EXT_TPP_TOK_STAR_LANGLE_MINUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_STAR_LANGLE_MINUS, TPP_EXTNAME_TPP_TOK_STAR_LANGLE_MINUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS))
#define _tpp_lexer_has_TPP_TOK_STAR_LANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_STAR_LANGLE_MINUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_STAR_EQUAL
#define TPP_EXTNAME_TPP_TOK_STAR_EQUAL "tok-star_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_STAR_EQUAL */
#define TPP_EXT_TPP_TOK_STAR_EQUAL TPP_EXT_TPP_TOK_STAR_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_STAR_EQUAL, TPP_EXTNAME_TPP_TOK_STAR_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_STAR_EQUAL))
#define _tpp_lexer_has_TPP_TOK_STAR_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_STAR_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_STAR_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PLUS_PLUS)
#ifndef TPP_EXTNAME_TPP_TOK_PLUS_PLUS
#define TPP_EXTNAME_TPP_TOK_PLUS_PLUS "tok-plus_plus"
#endif /* !TPP_EXTNAME_TPP_TOK_PLUS_PLUS */
#define TPP_EXT_TPP_TOK_PLUS_PLUS TPP_EXT_TPP_TOK_PLUS_PLUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_PLUS_PLUS, TPP_EXTNAME_TPP_TOK_PLUS_PLUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PLUS_PLUS))
#define _tpp_lexer_has_TPP_TOK_PLUS_PLUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_PLUS_PLUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PLUS_PLUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PLUS_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_PLUS_EQUAL
#define TPP_EXTNAME_TPP_TOK_PLUS_EQUAL "tok-plus_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_PLUS_EQUAL */
#define TPP_EXT_TPP_TOK_PLUS_EQUAL TPP_EXT_TPP_TOK_PLUS_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_PLUS_EQUAL, TPP_EXTNAME_TPP_TOK_PLUS_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PLUS_EQUAL))
#define _tpp_lexer_has_TPP_TOK_PLUS_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_PLUS_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PLUS_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_MINUS)
#ifndef TPP_EXTNAME_TPP_TOK_MINUS_MINUS
#define TPP_EXTNAME_TPP_TOK_MINUS_MINUS "tok-minus_minus"
#endif /* !TPP_EXTNAME_TPP_TOK_MINUS_MINUS */
#define TPP_EXT_TPP_TOK_MINUS_MINUS TPP_EXT_TPP_TOK_MINUS_MINUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_MINUS_MINUS, TPP_EXTNAME_TPP_TOK_MINUS_MINUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_MINUS))
#define _tpp_lexer_has_TPP_TOK_MINUS_MINUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_MINUS_MINUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_MINUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_MINUS_LANGLE
#define TPP_EXTNAME_TPP_TOK_MINUS_LANGLE "tok-minus_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_MINUS_LANGLE */
#define TPP_EXT_TPP_TOK_MINUS_LANGLE TPP_EXT_TPP_TOK_MINUS_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_MINUS_LANGLE, TPP_EXTNAME_TPP_TOK_MINUS_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_LANGLE))
#define _tpp_lexer_has_TPP_TOK_MINUS_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_MINUS_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_MINUS_LANGLE_LANGLE
#define TPP_EXTNAME_TPP_TOK_MINUS_LANGLE_LANGLE "tok-minus_langle_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_MINUS_LANGLE_LANGLE */
#define TPP_EXT_TPP_TOK_MINUS_LANGLE_LANGLE TPP_EXT_TPP_TOK_MINUS_LANGLE_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_MINUS_LANGLE_LANGLE, TPP_EXTNAME_TPP_TOK_MINUS_LANGLE_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE))
#define _tpp_lexer_has_TPP_TOK_MINUS_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_MINUS_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
#define TPP_EXTNAME_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE "tok-minus_langle_langle_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */
#define TPP_EXT_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE TPP_EXT_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE, TPP_EXTNAME_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE))
#define _tpp_lexer_has_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_MINUS_EQUAL
#define TPP_EXTNAME_TPP_TOK_MINUS_EQUAL "tok-minus_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_MINUS_EQUAL */
#define TPP_EXT_TPP_TOK_MINUS_EQUAL TPP_EXT_TPP_TOK_MINUS_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_MINUS_EQUAL, TPP_EXTNAME_TPP_TOK_MINUS_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_EQUAL))
#define _tpp_lexer_has_TPP_TOK_MINUS_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_MINUS_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_MINUS_RANGLE
#define TPP_EXTNAME_TPP_TOK_MINUS_RANGLE "tok-minus_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_MINUS_RANGLE */
#define TPP_EXT_TPP_TOK_MINUS_RANGLE TPP_EXT_TPP_TOK_MINUS_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_MINUS_RANGLE, TPP_EXTNAME_TPP_TOK_MINUS_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_RANGLE))
#define _tpp_lexer_has_TPP_TOK_MINUS_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_MINUS_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR)
#ifndef TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_STAR
#define TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_STAR "tok-minus_rangle_star"
#endif /* !TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_STAR */
#define TPP_EXT_TPP_TOK_MINUS_RANGLE_STAR TPP_EXT_TPP_TOK_MINUS_RANGLE_STAR
TPP_EXTENSION(TPP_EXT_TPP_TOK_MINUS_RANGLE_STAR, TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_STAR, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR))
#define _tpp_lexer_has_TPP_TOK_MINUS_RANGLE_STAR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_MINUS_RANGLE_STAR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_RANGLE
#define TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_RANGLE "tok-minus_rangle_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_RANGLE */
#define TPP_EXT_TPP_TOK_MINUS_RANGLE_RANGLE TPP_EXT_TPP_TOK_MINUS_RANGLE_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_MINUS_RANGLE_RANGLE, TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE))
#define _tpp_lexer_has_TPP_TOK_MINUS_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_MINUS_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
#define TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE "tok-minus_rangle_rangle_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */
#define TPP_EXT_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE TPP_EXT_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE, TPP_EXTNAME_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE))
#define _tpp_lexer_has_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_DOT_STAR)
#ifndef TPP_EXTNAME_TPP_TOK_DOT_STAR
#define TPP_EXTNAME_TPP_TOK_DOT_STAR "tok-dot_star"
#endif /* !TPP_EXTNAME_TPP_TOK_DOT_STAR */
#define TPP_EXT_TPP_TOK_DOT_STAR TPP_EXT_TPP_TOK_DOT_STAR
TPP_EXTENSION(TPP_EXT_TPP_TOK_DOT_STAR, TPP_EXTNAME_TPP_TOK_DOT_STAR, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_DOT_STAR))
#define _tpp_lexer_has_TPP_TOK_DOT_STAR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_DOT_STAR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_DOT_STAR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_DOT_DOT)
#ifndef TPP_EXTNAME_TPP_TOK_DOT_DOT
#define TPP_EXTNAME_TPP_TOK_DOT_DOT "tok-dot_dot"
#endif /* !TPP_EXTNAME_TPP_TOK_DOT_DOT */
#define TPP_EXT_TPP_TOK_DOT_DOT TPP_EXT_TPP_TOK_DOT_DOT
TPP_EXTENSION(TPP_EXT_TPP_TOK_DOT_DOT, TPP_EXTNAME_TPP_TOK_DOT_DOT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_DOT_DOT))
#define _tpp_lexer_has_TPP_TOK_DOT_DOT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_DOT_DOT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_DOT_DOT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_DOT_DOT_DOT)
#ifndef TPP_EXTNAME_TPP_TOK_DOT_DOT_DOT
#define TPP_EXTNAME_TPP_TOK_DOT_DOT_DOT "tok-dot_dot_dot"
#endif /* !TPP_EXTNAME_TPP_TOK_DOT_DOT_DOT */
#define TPP_EXT_TPP_TOK_DOT_DOT_DOT TPP_EXT_TPP_TOK_DOT_DOT_DOT
TPP_EXTENSION(TPP_EXT_TPP_TOK_DOT_DOT_DOT, TPP_EXTNAME_TPP_TOK_DOT_DOT_DOT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_DOT_DOT_DOT))
#define _tpp_lexer_has_TPP_TOK_DOT_DOT_DOT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_DOT_DOT_DOT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_DOT_DOT_DOT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SLASH_SLASH)
#ifndef TPP_EXTNAME_TPP_TOK_SLASH_SLASH
#define TPP_EXTNAME_TPP_TOK_SLASH_SLASH "tok-slash_slash"
#endif /* !TPP_EXTNAME_TPP_TOK_SLASH_SLASH */
#define TPP_EXT_TPP_TOK_SLASH_SLASH TPP_EXT_TPP_TOK_SLASH_SLASH
TPP_EXTENSION(TPP_EXT_TPP_TOK_SLASH_SLASH, TPP_EXTNAME_TPP_TOK_SLASH_SLASH, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SLASH_SLASH))
#define _tpp_lexer_has_TPP_TOK_SLASH_SLASH(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_SLASH_SLASH)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SLASH_SLASH) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_SLASH_SLASH_EQUAL
#define TPP_EXTNAME_TPP_TOK_SLASH_SLASH_EQUAL "tok-slash_slash_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_SLASH_SLASH_EQUAL */
#define TPP_EXT_TPP_TOK_SLASH_SLASH_EQUAL TPP_EXT_TPP_TOK_SLASH_SLASH_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_SLASH_SLASH_EQUAL, TPP_EXTNAME_TPP_TOK_SLASH_SLASH_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL))
#define _tpp_lexer_has_TPP_TOK_SLASH_SLASH_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_SLASH_SLASH_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SLASH_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_SLASH_EQUAL
#define TPP_EXTNAME_TPP_TOK_SLASH_EQUAL "tok-slash_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_SLASH_EQUAL */
#define TPP_EXT_TPP_TOK_SLASH_EQUAL TPP_EXT_TPP_TOK_SLASH_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_SLASH_EQUAL, TPP_EXTNAME_TPP_TOK_SLASH_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_SLASH_EQUAL))
#define _tpp_lexer_has_TPP_TOK_SLASH_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_SLASH_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_SLASH_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_COLON_COLON)
#ifndef TPP_EXTNAME_TPP_TOK_COLON_COLON
#define TPP_EXTNAME_TPP_TOK_COLON_COLON "tok-colon_colon"
#endif /* !TPP_EXTNAME_TPP_TOK_COLON_COLON */
#define TPP_EXT_TPP_TOK_COLON_COLON TPP_EXT_TPP_TOK_COLON_COLON
TPP_EXTENSION(TPP_EXT_TPP_TOK_COLON_COLON, TPP_EXTNAME_TPP_TOK_COLON_COLON, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_COLON_COLON))
#define _tpp_lexer_has_TPP_TOK_COLON_COLON(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_COLON_COLON)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_COLON_COLON) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_COLON_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_COLON_EQUAL
#define TPP_EXTNAME_TPP_TOK_COLON_EQUAL "tok-colon_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_COLON_EQUAL */
#define TPP_EXT_TPP_TOK_COLON_EQUAL TPP_EXT_TPP_TOK_COLON_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_COLON_EQUAL, TPP_EXTNAME_TPP_TOK_COLON_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_COLON_EQUAL))
#define _tpp_lexer_has_TPP_TOK_COLON_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_COLON_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_COLON_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_MINUS)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_MINUS
#define TPP_EXTNAME_TPP_TOK_LANGLE_MINUS "tok-langle_minus"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_MINUS */
#define TPP_EXT_TPP_TOK_LANGLE_MINUS TPP_EXT_TPP_TOK_LANGLE_MINUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_MINUS, TPP_EXTNAME_TPP_TOK_LANGLE_MINUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_MINUS))
#define _tpp_lexer_has_TPP_TOK_LANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_MINUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_MINUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_MINUS_LANGLE
#define TPP_EXTNAME_TPP_TOK_LANGLE_MINUS_LANGLE "tok-langle_minus_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_MINUS_LANGLE */
#define TPP_EXT_TPP_TOK_LANGLE_MINUS_LANGLE TPP_EXT_TPP_TOK_LANGLE_MINUS_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_MINUS_LANGLE, TPP_EXTNAME_TPP_TOK_LANGLE_MINUS_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE))
#define _tpp_lexer_has_TPP_TOK_LANGLE_MINUS_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_MINUS_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_MINUS_RANGLE
#define TPP_EXTNAME_TPP_TOK_LANGLE_MINUS_RANGLE "tok-langle_minus_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_MINUS_RANGLE */
#define TPP_EXT_TPP_TOK_LANGLE_MINUS_RANGLE TPP_EXT_TPP_TOK_LANGLE_MINUS_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_MINUS_RANGLE, TPP_EXTNAME_TPP_TOK_LANGLE_MINUS_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE))
#define _tpp_lexer_has_TPP_TOK_LANGLE_MINUS_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_MINUS_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE
#define TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE "tok-langle_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE */
#define TPP_EXT_TPP_TOK_LANGLE_LANGLE TPP_EXT_TPP_TOK_LANGLE_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_LANGLE, TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE))
#define _tpp_lexer_has_TPP_TOK_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_MINUS
#define TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_MINUS "tok-langle_langle_minus"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_MINUS */
#define TPP_EXT_TPP_TOK_LANGLE_LANGLE_MINUS TPP_EXT_TPP_TOK_LANGLE_LANGLE_MINUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_LANGLE_MINUS, TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_MINUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS))
#define _tpp_lexer_has_TPP_TOK_LANGLE_LANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_LANGLE_MINUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE
#define TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE "tok-langle_langle_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE */
#define TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE, TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE))
#define _tpp_lexer_has_TPP_TOK_LANGLE_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS
#define TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS "tok-langle_langle_langle_minus"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS */
#define TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS, TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS))
#define _tpp_lexer_has_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#define TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL "tok-langle_langle_langle_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#define TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL, TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL))
#define _tpp_lexer_has_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_EQUAL
#define TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_EQUAL "tok-langle_langle_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_EQUAL */
#define TPP_EXT_TPP_TOK_LANGLE_LANGLE_EQUAL TPP_EXT_TPP_TOK_LANGLE_LANGLE_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_LANGLE_EQUAL, TPP_EXTNAME_TPP_TOK_LANGLE_LANGLE_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL))
#define _tpp_lexer_has_TPP_TOK_LANGLE_LANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_LANGLE_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL
#define TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL "tok-langle_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL */
#define TPP_EXT_TPP_TOK_LANGLE_EQUAL TPP_EXT_TPP_TOK_LANGLE_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_EQUAL, TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL))
#define _tpp_lexer_has_TPP_TOK_LANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL_LANGLE
#define TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL_LANGLE "tok-langle_equal_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL_LANGLE */
#define TPP_EXT_TPP_TOK_LANGLE_EQUAL_LANGLE TPP_EXT_TPP_TOK_LANGLE_EQUAL_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_EQUAL_LANGLE, TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE))
#define _tpp_lexer_has_TPP_TOK_LANGLE_EQUAL_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_EQUAL_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL_RANGLE
#define TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL_RANGLE "tok-langle_equal_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL_RANGLE */
#define TPP_EXT_TPP_TOK_LANGLE_EQUAL_RANGLE TPP_EXT_TPP_TOK_LANGLE_EQUAL_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_EQUAL_RANGLE, TPP_EXTNAME_TPP_TOK_LANGLE_EQUAL_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE))
#define _tpp_lexer_has_TPP_TOK_LANGLE_EQUAL_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_EQUAL_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_LANGLE_RANGLE
#define TPP_EXTNAME_TPP_TOK_LANGLE_RANGLE "tok-langle_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_LANGLE_RANGLE */
#define TPP_EXT_TPP_TOK_LANGLE_RANGLE TPP_EXT_TPP_TOK_LANGLE_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_LANGLE_RANGLE, TPP_EXTNAME_TPP_TOK_LANGLE_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_LANGLE_RANGLE))
#define _tpp_lexer_has_TPP_TOK_LANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_LANGLE_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_LANGLE_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_EXCLAIM
#define TPP_EXTNAME_TPP_TOK_EQUAL_EXCLAIM "tok-equal_exclaim"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_EXCLAIM */
#define TPP_EXT_TPP_TOK_EQUAL_EXCLAIM TPP_EXT_TPP_TOK_EQUAL_EXCLAIM
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_EXCLAIM, TPP_EXTNAME_TPP_TOK_EQUAL_EXCLAIM, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM))
#define _tpp_lexer_has_TPP_TOK_EQUAL_EXCLAIM(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_EXCLAIM)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_PERCENT)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_PERCENT
#define TPP_EXTNAME_TPP_TOK_EQUAL_PERCENT "tok-equal_percent"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_PERCENT */
#define TPP_EXT_TPP_TOK_EQUAL_PERCENT TPP_EXT_TPP_TOK_EQUAL_PERCENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_PERCENT, TPP_EXTNAME_TPP_TOK_EQUAL_PERCENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_PERCENT))
#define _tpp_lexer_has_TPP_TOK_EQUAL_PERCENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_PERCENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_PERCENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_PERCENT_PERCENT
#define TPP_EXTNAME_TPP_TOK_EQUAL_PERCENT_PERCENT "tok-equal_percent_percent"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_PERCENT_PERCENT */
#define TPP_EXT_TPP_TOK_EQUAL_PERCENT_PERCENT TPP_EXT_TPP_TOK_EQUAL_PERCENT_PERCENT
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_PERCENT_PERCENT, TPP_EXTNAME_TPP_TOK_EQUAL_PERCENT_PERCENT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT))
#define _tpp_lexer_has_TPP_TOK_EQUAL_PERCENT_PERCENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_PERCENT_PERCENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_AMP)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_AMP
#define TPP_EXTNAME_TPP_TOK_EQUAL_AMP "tok-equal_amp"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_AMP */
#define TPP_EXT_TPP_TOK_EQUAL_AMP TPP_EXT_TPP_TOK_EQUAL_AMP
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_AMP, TPP_EXTNAME_TPP_TOK_EQUAL_AMP, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_AMP))
#define _tpp_lexer_has_TPP_TOK_EQUAL_AMP(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_AMP)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_AMP) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_STAR)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_STAR
#define TPP_EXTNAME_TPP_TOK_EQUAL_STAR "tok-equal_star"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_STAR */
#define TPP_EXT_TPP_TOK_EQUAL_STAR TPP_EXT_TPP_TOK_EQUAL_STAR
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_STAR, TPP_EXTNAME_TPP_TOK_EQUAL_STAR, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_STAR))
#define _tpp_lexer_has_TPP_TOK_EQUAL_STAR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_STAR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_STAR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_STAR_STAR
#define TPP_EXTNAME_TPP_TOK_EQUAL_STAR_STAR "tok-equal_star_star"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_STAR_STAR */
#define TPP_EXT_TPP_TOK_EQUAL_STAR_STAR TPP_EXT_TPP_TOK_EQUAL_STAR_STAR
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_STAR_STAR, TPP_EXTNAME_TPP_TOK_EQUAL_STAR_STAR, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR))
#define _tpp_lexer_has_TPP_TOK_EQUAL_STAR_STAR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_STAR_STAR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_PLUS)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_PLUS
#define TPP_EXTNAME_TPP_TOK_EQUAL_PLUS "tok-equal_plus"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_PLUS */
#define TPP_EXT_TPP_TOK_EQUAL_PLUS TPP_EXT_TPP_TOK_EQUAL_PLUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_PLUS, TPP_EXTNAME_TPP_TOK_EQUAL_PLUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_PLUS))
#define _tpp_lexer_has_TPP_TOK_EQUAL_PLUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_PLUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_PLUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_MINUS)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_MINUS
#define TPP_EXTNAME_TPP_TOK_EQUAL_MINUS "tok-equal_minus"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_MINUS */
#define TPP_EXT_TPP_TOK_EQUAL_MINUS TPP_EXT_TPP_TOK_EQUAL_MINUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_MINUS, TPP_EXTNAME_TPP_TOK_EQUAL_MINUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_MINUS))
#define _tpp_lexer_has_TPP_TOK_EQUAL_MINUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_MINUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_MINUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_SLASH)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_SLASH
#define TPP_EXTNAME_TPP_TOK_EQUAL_SLASH "tok-equal_slash"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_SLASH */
#define TPP_EXT_TPP_TOK_EQUAL_SLASH TPP_EXT_TPP_TOK_EQUAL_SLASH
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_SLASH, TPP_EXTNAME_TPP_TOK_EQUAL_SLASH, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_SLASH))
#define _tpp_lexer_has_TPP_TOK_EQUAL_SLASH(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_SLASH)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_SLASH) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_SLASH_SLASH
#define TPP_EXTNAME_TPP_TOK_EQUAL_SLASH_SLASH "tok-equal_slash_slash"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_SLASH_SLASH */
#define TPP_EXT_TPP_TOK_EQUAL_SLASH_SLASH TPP_EXT_TPP_TOK_EQUAL_SLASH_SLASH
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_SLASH_SLASH, TPP_EXTNAME_TPP_TOK_EQUAL_SLASH_SLASH, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH))
#define _tpp_lexer_has_TPP_TOK_EQUAL_SLASH_SLASH(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_SLASH_SLASH)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_COLON)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_COLON
#define TPP_EXTNAME_TPP_TOK_EQUAL_COLON "tok-equal_colon"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_COLON */
#define TPP_EXT_TPP_TOK_EQUAL_COLON TPP_EXT_TPP_TOK_EQUAL_COLON
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_COLON, TPP_EXTNAME_TPP_TOK_EQUAL_COLON, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_COLON))
#define _tpp_lexer_has_TPP_TOK_EQUAL_COLON(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_COLON)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_COLON) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE
#define TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE "tok-equal_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE */
#define TPP_EXT_TPP_TOK_EQUAL_LANGLE TPP_EXT_TPP_TOK_EQUAL_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_LANGLE, TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE))
#define _tpp_lexer_has_TPP_TOK_EQUAL_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE_LANGLE
#define TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE_LANGLE "tok-equal_langle_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE_LANGLE */
#define TPP_EXT_TPP_TOK_EQUAL_LANGLE_LANGLE TPP_EXT_TPP_TOK_EQUAL_LANGLE_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_LANGLE_LANGLE, TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE))
#define _tpp_lexer_has_TPP_TOK_EQUAL_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
#define TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE "tok-equal_langle_langle_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#define TPP_EXT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE TPP_EXT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE, TPP_EXTNAME_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE))
#define _tpp_lexer_has_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL
#define TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL "tok-equal_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL */
#define TPP_EXT_TPP_TOK_EQUAL_EQUAL TPP_EXT_TPP_TOK_EQUAL_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_EQUAL, TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL))
#define _tpp_lexer_has_TPP_TOK_EQUAL_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL_EXCLAIM
#define TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL_EXCLAIM "tok-equal_equal_exclaim"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#define TPP_EXT_TPP_TOK_EQUAL_EQUAL_EXCLAIM TPP_EXT_TPP_TOK_EQUAL_EQUAL_EXCLAIM
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_EQUAL_EXCLAIM, TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL_EXCLAIM, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM))
#define _tpp_lexer_has_TPP_TOK_EQUAL_EQUAL_EXCLAIM(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_EQUAL_EXCLAIM)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL_EQUAL
#define TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL_EQUAL "tok-equal_equal_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL_EQUAL */
#define TPP_EXT_TPP_TOK_EQUAL_EQUAL_EQUAL TPP_EXT_TPP_TOK_EQUAL_EQUAL_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_EQUAL_EQUAL, TPP_EXTNAME_TPP_TOK_EQUAL_EQUAL_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL))
#define _tpp_lexer_has_TPP_TOK_EQUAL_EQUAL_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_EQUAL_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE
#define TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE "tok-equal_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE */
#define TPP_EXT_TPP_TOK_EQUAL_RANGLE TPP_EXT_TPP_TOK_EQUAL_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_RANGLE, TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE))
#define _tpp_lexer_has_TPP_TOK_EQUAL_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE_RANGLE
#define TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE_RANGLE "tok-equal_rangle_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE_RANGLE */
#define TPP_EXT_TPP_TOK_EQUAL_RANGLE_RANGLE TPP_EXT_TPP_TOK_EQUAL_RANGLE_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_RANGLE_RANGLE, TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE))
#define _tpp_lexer_has_TPP_TOK_EQUAL_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
#define TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE "tok-equal_rangle_rangle_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#define TPP_EXT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE TPP_EXT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE, TPP_EXTNAME_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE))
#define _tpp_lexer_has_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_QMARK)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_QMARK
#define TPP_EXTNAME_TPP_TOK_EQUAL_QMARK "tok-equal_qmark"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_QMARK */
#define TPP_EXT_TPP_TOK_EQUAL_QMARK TPP_EXT_TPP_TOK_EQUAL_QMARK
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_QMARK, TPP_EXTNAME_TPP_TOK_EQUAL_QMARK, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_QMARK))
#define _tpp_lexer_has_TPP_TOK_EQUAL_QMARK(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_QMARK)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_QMARK) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_AT)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_AT
#define TPP_EXTNAME_TPP_TOK_EQUAL_AT "tok-equal_at"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_AT */
#define TPP_EXT_TPP_TOK_EQUAL_AT TPP_EXT_TPP_TOK_EQUAL_AT
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_AT, TPP_EXTNAME_TPP_TOK_EQUAL_AT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_AT))
#define _tpp_lexer_has_TPP_TOK_EQUAL_AT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_AT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_AT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_AT_AT)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_AT_AT
#define TPP_EXTNAME_TPP_TOK_EQUAL_AT_AT "tok-equal_at_at"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_AT_AT */
#define TPP_EXT_TPP_TOK_EQUAL_AT_AT TPP_EXT_TPP_TOK_EQUAL_AT_AT
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_AT_AT, TPP_EXTNAME_TPP_TOK_EQUAL_AT_AT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_AT_AT))
#define _tpp_lexer_has_TPP_TOK_EQUAL_AT_AT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_AT_AT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_AT_AT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_HAT)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_HAT
#define TPP_EXTNAME_TPP_TOK_EQUAL_HAT "tok-equal_hat"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_HAT */
#define TPP_EXT_TPP_TOK_EQUAL_HAT TPP_EXT_TPP_TOK_EQUAL_HAT
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_HAT, TPP_EXTNAME_TPP_TOK_EQUAL_HAT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_HAT))
#define _tpp_lexer_has_TPP_TOK_EQUAL_HAT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_HAT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_HAT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_PIPE)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_PIPE
#define TPP_EXTNAME_TPP_TOK_EQUAL_PIPE "tok-equal_pipe"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_PIPE */
#define TPP_EXT_TPP_TOK_EQUAL_PIPE TPP_EXT_TPP_TOK_EQUAL_PIPE
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_PIPE, TPP_EXTNAME_TPP_TOK_EQUAL_PIPE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_PIPE))
#define _tpp_lexer_has_TPP_TOK_EQUAL_PIPE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_PIPE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_PIPE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_TILDE)
#ifndef TPP_EXTNAME_TPP_TOK_EQUAL_TILDE
#define TPP_EXTNAME_TPP_TOK_EQUAL_TILDE "tok-equal_tilde"
#endif /* !TPP_EXTNAME_TPP_TOK_EQUAL_TILDE */
#define TPP_EXT_TPP_TOK_EQUAL_TILDE TPP_EXT_TPP_TOK_EQUAL_TILDE
TPP_EXTENSION(TPP_EXT_TPP_TOK_EQUAL_TILDE, TPP_EXTNAME_TPP_TOK_EQUAL_TILDE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_EQUAL_TILDE))
#define _tpp_lexer_has_TPP_TOK_EQUAL_TILDE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_EQUAL_TILDE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_EQUAL_TILDE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_MINUS)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_MINUS
#define TPP_EXTNAME_TPP_TOK_RANGLE_MINUS "tok-rangle_minus"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_MINUS */
#define TPP_EXT_TPP_TOK_RANGLE_MINUS TPP_EXT_TPP_TOK_RANGLE_MINUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_MINUS, TPP_EXTNAME_TPP_TOK_RANGLE_MINUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_MINUS))
#define _tpp_lexer_has_TPP_TOK_RANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_MINUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_MINUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_MINUS_LANGLE
#define TPP_EXTNAME_TPP_TOK_RANGLE_MINUS_LANGLE "tok-rangle_minus_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_MINUS_LANGLE */
#define TPP_EXT_TPP_TOK_RANGLE_MINUS_LANGLE TPP_EXT_TPP_TOK_RANGLE_MINUS_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_MINUS_LANGLE, TPP_EXTNAME_TPP_TOK_RANGLE_MINUS_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE))
#define _tpp_lexer_has_TPP_TOK_RANGLE_MINUS_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_MINUS_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_MINUS_RANGLE
#define TPP_EXTNAME_TPP_TOK_RANGLE_MINUS_RANGLE "tok-rangle_minus_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_MINUS_RANGLE */
#define TPP_EXT_TPP_TOK_RANGLE_MINUS_RANGLE TPP_EXT_TPP_TOK_RANGLE_MINUS_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_MINUS_RANGLE, TPP_EXTNAME_TPP_TOK_RANGLE_MINUS_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE))
#define _tpp_lexer_has_TPP_TOK_RANGLE_MINUS_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_MINUS_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_LANGLE
#define TPP_EXTNAME_TPP_TOK_RANGLE_LANGLE "tok-rangle_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_LANGLE */
#define TPP_EXT_TPP_TOK_RANGLE_LANGLE TPP_EXT_TPP_TOK_RANGLE_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_LANGLE, TPP_EXTNAME_TPP_TOK_RANGLE_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_LANGLE))
#define _tpp_lexer_has_TPP_TOK_RANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL
#define TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL "tok-rangle_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL */
#define TPP_EXT_TPP_TOK_RANGLE_EQUAL TPP_EXT_TPP_TOK_RANGLE_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_EQUAL, TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL))
#define _tpp_lexer_has_TPP_TOK_RANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL_LANGLE
#define TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL_LANGLE "tok-rangle_equal_langle"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL_LANGLE */
#define TPP_EXT_TPP_TOK_RANGLE_EQUAL_LANGLE TPP_EXT_TPP_TOK_RANGLE_EQUAL_LANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_EQUAL_LANGLE, TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL_LANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE))
#define _tpp_lexer_has_TPP_TOK_RANGLE_EQUAL_LANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_EQUAL_LANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL_RANGLE
#define TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL_RANGLE "tok-rangle_equal_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL_RANGLE */
#define TPP_EXT_TPP_TOK_RANGLE_EQUAL_RANGLE TPP_EXT_TPP_TOK_RANGLE_EQUAL_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_EQUAL_RANGLE, TPP_EXTNAME_TPP_TOK_RANGLE_EQUAL_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE))
#define _tpp_lexer_has_TPP_TOK_RANGLE_EQUAL_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_EQUAL_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE
#define TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE "tok-rangle_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE */
#define TPP_EXT_TPP_TOK_RANGLE_RANGLE TPP_EXT_TPP_TOK_RANGLE_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_RANGLE, TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE))
#define _tpp_lexer_has_TPP_TOK_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_MINUS
#define TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_MINUS "tok-rangle_rangle_minus"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_MINUS */
#define TPP_EXT_TPP_TOK_RANGLE_RANGLE_MINUS TPP_EXT_TPP_TOK_RANGLE_RANGLE_MINUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_RANGLE_MINUS, TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_MINUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS))
#define _tpp_lexer_has_TPP_TOK_RANGLE_RANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_RANGLE_MINUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_EQUAL
#define TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_EQUAL "tok-rangle_rangle_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_EQUAL */
#define TPP_EXT_TPP_TOK_RANGLE_RANGLE_EQUAL TPP_EXT_TPP_TOK_RANGLE_RANGLE_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_RANGLE_EQUAL, TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL))
#define _tpp_lexer_has_TPP_TOK_RANGLE_RANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_RANGLE_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE
#define TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE "tok-rangle_rangle_rangle"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE */
#define TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE, TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE))
#define _tpp_lexer_has_TPP_TOK_RANGLE_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS
#define TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS "tok-rangle_rangle_rangle_minus"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS */
#define TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS, TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS))
#define _tpp_lexer_has_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#define TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL "tok-rangle_rangle_rangle_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#define TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL, TPP_EXTNAME_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL))
#define _tpp_lexer_has_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_QMARK_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_QMARK_EQUAL
#define TPP_EXTNAME_TPP_TOK_QMARK_EQUAL "tok-qmark_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_QMARK_EQUAL */
#define TPP_EXT_TPP_TOK_QMARK_EQUAL TPP_EXT_TPP_TOK_QMARK_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_QMARK_EQUAL, TPP_EXTNAME_TPP_TOK_QMARK_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_QMARK_EQUAL))
#define _tpp_lexer_has_TPP_TOK_QMARK_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_QMARK_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_QMARK_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_QMARK_QMARK)
#ifndef TPP_EXTNAME_TPP_TOK_QMARK_QMARK
#define TPP_EXTNAME_TPP_TOK_QMARK_QMARK "tok-qmark_qmark"
#endif /* !TPP_EXTNAME_TPP_TOK_QMARK_QMARK */
#define TPP_EXT_TPP_TOK_QMARK_QMARK TPP_EXT_TPP_TOK_QMARK_QMARK
TPP_EXTENSION(TPP_EXT_TPP_TOK_QMARK_QMARK, TPP_EXTNAME_TPP_TOK_QMARK_QMARK, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_QMARK_QMARK))
#define _tpp_lexer_has_TPP_TOK_QMARK_QMARK(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_QMARK_QMARK)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_QMARK_QMARK) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AT_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_AT_EQUAL
#define TPP_EXTNAME_TPP_TOK_AT_EQUAL "tok-at_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_AT_EQUAL */
#define TPP_EXT_TPP_TOK_AT_EQUAL TPP_EXT_TPP_TOK_AT_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_AT_EQUAL, TPP_EXTNAME_TPP_TOK_AT_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_AT_EQUAL))
#define _tpp_lexer_has_TPP_TOK_AT_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_AT_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AT_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AT_AT)
#ifndef TPP_EXTNAME_TPP_TOK_AT_AT
#define TPP_EXTNAME_TPP_TOK_AT_AT "tok-at_at"
#endif /* !TPP_EXTNAME_TPP_TOK_AT_AT */
#define TPP_EXT_TPP_TOK_AT_AT TPP_EXT_TPP_TOK_AT_AT
TPP_EXTENSION(TPP_EXT_TPP_TOK_AT_AT, TPP_EXTNAME_TPP_TOK_AT_AT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_AT_AT))
#define _tpp_lexer_has_TPP_TOK_AT_AT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_AT_AT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AT_AT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AT_AT_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_AT_AT_EQUAL
#define TPP_EXTNAME_TPP_TOK_AT_AT_EQUAL "tok-at_at_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_AT_AT_EQUAL */
#define TPP_EXT_TPP_TOK_AT_AT_EQUAL TPP_EXT_TPP_TOK_AT_AT_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_AT_AT_EQUAL, TPP_EXTNAME_TPP_TOK_AT_AT_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_AT_AT_EQUAL))
#define _tpp_lexer_has_TPP_TOK_AT_AT_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_AT_AT_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_AT_AT_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_HAT_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_HAT_EQUAL
#define TPP_EXTNAME_TPP_TOK_HAT_EQUAL "tok-hat_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_HAT_EQUAL */
#define TPP_EXT_TPP_TOK_HAT_EQUAL TPP_EXT_TPP_TOK_HAT_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_HAT_EQUAL, TPP_EXTNAME_TPP_TOK_HAT_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_HAT_EQUAL))
#define _tpp_lexer_has_TPP_TOK_HAT_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_HAT_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_HAT_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_HAT_HAT)
#ifndef TPP_EXTNAME_TPP_TOK_HAT_HAT
#define TPP_EXTNAME_TPP_TOK_HAT_HAT "tok-hat_hat"
#endif /* !TPP_EXTNAME_TPP_TOK_HAT_HAT */
#define TPP_EXT_TPP_TOK_HAT_HAT TPP_EXT_TPP_TOK_HAT_HAT
TPP_EXTENSION(TPP_EXT_TPP_TOK_HAT_HAT, TPP_EXTNAME_TPP_TOK_HAT_HAT, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_HAT_HAT))
#define _tpp_lexer_has_TPP_TOK_HAT_HAT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_HAT_HAT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_HAT_HAT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PIPE_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_PIPE_EQUAL
#define TPP_EXTNAME_TPP_TOK_PIPE_EQUAL "tok-pipe_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_PIPE_EQUAL */
#define TPP_EXT_TPP_TOK_PIPE_EQUAL TPP_EXT_TPP_TOK_PIPE_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_PIPE_EQUAL, TPP_EXTNAME_TPP_TOK_PIPE_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PIPE_EQUAL))
#define _tpp_lexer_has_TPP_TOK_PIPE_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_PIPE_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PIPE_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PIPE_PIPE)
#ifndef TPP_EXTNAME_TPP_TOK_PIPE_PIPE
#define TPP_EXTNAME_TPP_TOK_PIPE_PIPE "tok-pipe_pipe"
#endif /* !TPP_EXTNAME_TPP_TOK_PIPE_PIPE */
#define TPP_EXT_TPP_TOK_PIPE_PIPE TPP_EXT_TPP_TOK_PIPE_PIPE
TPP_EXTENSION(TPP_EXT_TPP_TOK_PIPE_PIPE, TPP_EXTNAME_TPP_TOK_PIPE_PIPE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_PIPE_PIPE))
#define _tpp_lexer_has_TPP_TOK_PIPE_PIPE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_PIPE_PIPE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_PIPE_PIPE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_TILDE_EQUAL)
#ifndef TPP_EXTNAME_TPP_TOK_TILDE_EQUAL
#define TPP_EXTNAME_TPP_TOK_TILDE_EQUAL "tok-tilde_equal"
#endif /* !TPP_EXTNAME_TPP_TOK_TILDE_EQUAL */
#define TPP_EXT_TPP_TOK_TILDE_EQUAL TPP_EXT_TPP_TOK_TILDE_EQUAL
TPP_EXTENSION(TPP_EXT_TPP_TOK_TILDE_EQUAL, TPP_EXTNAME_TPP_TOK_TILDE_EQUAL, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_TILDE_EQUAL))
#define _tpp_lexer_has_TPP_TOK_TILDE_EQUAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_TILDE_EQUAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_TILDE_EQUAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_TILDE_TILDE)
#ifndef TPP_EXTNAME_TPP_TOK_TILDE_TILDE
#define TPP_EXTNAME_TPP_TOK_TILDE_TILDE "tok-tilde_tilde"
#endif /* !TPP_EXTNAME_TPP_TOK_TILDE_TILDE */
#define TPP_EXT_TPP_TOK_TILDE_TILDE TPP_EXT_TPP_TOK_TILDE_TILDE
TPP_EXTENSION(TPP_EXT_TPP_TOK_TILDE_TILDE, TPP_EXTNAME_TPP_TOK_TILDE_TILDE, TPP_CONF_DEFAULT(TPP_HAVE_TPP_TOK_TILDE_TILDE))
#define _tpp_lexer_has_TPP_TOK_TILDE_TILDE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TPP_TOK_TILDE_TILDE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TPP_TOK_TILDE_TILDE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BSE)
#ifndef TPP_EXTNAME_BSE
#define TPP_EXTNAME_BSE "bse"
#endif /* !TPP_EXTNAME_BSE */
#define TPP_EXT_BSE TPP_EXT_BSE
TPP_EXTENSION(TPP_EXT_BSE, TPP_EXTNAME_BSE, TPP_CONF_DEFAULT(TPP_HAVE_BSE))
#define _tpp_lexer_has_BSE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BSE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BSE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BSE_WHITESPACE)
#ifndef TPP_EXTNAME_BSE_WHITESPACE
#define TPP_EXTNAME_BSE_WHITESPACE "bse-whitespace"
#endif /* !TPP_EXTNAME_BSE_WHITESPACE */
#define TPP_EXT_BSE_WHITESPACE TPP_EXT_BSE_WHITESPACE
TPP_EXTENSION(TPP_EXT_BSE_WHITESPACE, TPP_EXTNAME_BSE_WHITESPACE, TPP_CONF_DEFAULT(TPP_HAVE_BSE_WHITESPACE))
#define _tpp_lexer_has_BSE_WHITESPACE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BSE_WHITESPACE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BSE_WHITESPACE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
#ifndef TPP_EXTNAME_ESCAPE_IN_IDENTIFIERS
#define TPP_EXTNAME_ESCAPE_IN_IDENTIFIERS "escape-in-identifiers"
#endif /* !TPP_EXTNAME_ESCAPE_IN_IDENTIFIERS */
#define TPP_EXT_ESCAPE_IN_IDENTIFIERS TPP_EXT_ESCAPE_IN_IDENTIFIERS
TPP_EXTENSION(TPP_EXT_ESCAPE_IN_IDENTIFIERS, TPP_EXTNAME_ESCAPE_IN_IDENTIFIERS, TPP_CONF_DEFAULT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS))
#define _tpp_lexer_has_ESCAPE_IN_IDENTIFIERS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_ESCAPE_IN_IDENTIFIERS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_ESCAPE_E_IN_STRINGS)
#ifndef TPP_EXTNAME_ESCAPE_E_IN_STRINGS
#define TPP_EXTNAME_ESCAPE_E_IN_STRINGS "escape-e-in-strings"
#endif /* !TPP_EXTNAME_ESCAPE_E_IN_STRINGS */
#define TPP_EXT_ESCAPE_E_IN_STRINGS TPP_EXT_ESCAPE_E_IN_STRINGS
TPP_EXTENSION(TPP_EXT_ESCAPE_E_IN_STRINGS, TPP_EXTNAME_ESCAPE_E_IN_STRINGS, TPP_CONF_DEFAULT(TPP_HAVE_ESCAPE_E_IN_STRINGS))
#define _tpp_lexer_has_ESCAPE_E_IN_STRINGS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_ESCAPE_E_IN_STRINGS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_ESCAPE_E_IN_STRINGS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_ESCAPE_S_IN_STRINGS)
#ifndef TPP_EXTNAME_ESCAPE_S_IN_STRINGS
#define TPP_EXTNAME_ESCAPE_S_IN_STRINGS "escape-s-in-strings"
#endif /* !TPP_EXTNAME_ESCAPE_S_IN_STRINGS */
#define TPP_EXT_ESCAPE_S_IN_STRINGS TPP_EXT_ESCAPE_S_IN_STRINGS
TPP_EXTENSION(TPP_EXT_ESCAPE_S_IN_STRINGS, TPP_EXTNAME_ESCAPE_S_IN_STRINGS, TPP_CONF_DEFAULT(TPP_HAVE_ESCAPE_S_IN_STRINGS))
#define _tpp_lexer_has_ESCAPE_S_IN_STRINGS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_ESCAPE_S_IN_STRINGS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_ESCAPE_S_IN_STRINGS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_DIRECTIVES)
#ifndef TPP_EXTNAME_CPP_DIRECTIVES
#define TPP_EXTNAME_CPP_DIRECTIVES "cpp-directives"
#endif /* !TPP_EXTNAME_CPP_DIRECTIVES */
#define TPP_EXT_CPP_DIRECTIVES TPP_EXT_CPP_DIRECTIVES
TPP_EXTENSION(TPP_EXT_CPP_DIRECTIVES, TPP_EXTNAME_CPP_DIRECTIVES, TPP_CONF_DEFAULT(TPP_HAVE_CPP_DIRECTIVES))
#define _tpp_lexer_has_CPP_DIRECTIVES(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_DIRECTIVES)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_DIRECTIVES) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_MACROS)
#ifndef TPP_EXTNAME_CPP_MACROS
#define TPP_EXTNAME_CPP_MACROS "cpp-macros"
#endif /* !TPP_EXTNAME_CPP_MACROS */
#define TPP_EXT_CPP_MACROS TPP_EXT_CPP_MACROS
TPP_EXTENSION(TPP_EXT_CPP_MACROS, TPP_EXTNAME_CPP_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_CPP_MACROS))
#define _tpp_lexer_has_CPP_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_BUILTIN_MACROS)
#ifndef TPP_EXTNAME_CPP_BUILTIN_MACROS
#define TPP_EXTNAME_CPP_BUILTIN_MACROS "cpp-builtin-macros"
#endif /* !TPP_EXTNAME_CPP_BUILTIN_MACROS */
#define TPP_EXT_CPP_BUILTIN_MACROS TPP_EXT_CPP_BUILTIN_MACROS
TPP_EXTENSION(TPP_EXT_CPP_BUILTIN_MACROS, TPP_EXTNAME_CPP_BUILTIN_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_CPP_BUILTIN_MACROS))
#define _tpp_lexer_has_CPP_BUILTIN_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_BUILTIN_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_BUILTIN_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_EXCLAIM)
#ifndef TPP_EXTNAME_CPP_EXCLAIM
#define TPP_EXTNAME_CPP_EXCLAIM "shebang-directives"
#endif /* !TPP_EXTNAME_CPP_EXCLAIM */
#define TPP_EXT_CPP_EXCLAIM TPP_EXT_CPP_EXCLAIM
TPP_EXTENSION(TPP_EXT_CPP_EXCLAIM, TPP_EXTNAME_CPP_EXCLAIM, TPP_CONF_DEFAULT(TPP_HAVE_CPP_EXCLAIM))
#define _tpp_lexer_has_CPP_EXCLAIM(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_EXCLAIM)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_EXCLAIM) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_BLANK)
#ifndef TPP_EXTNAME_CPP_BLANK
#define TPP_EXTNAME_CPP_BLANK "blank-directives"
#endif /* !TPP_EXTNAME_CPP_BLANK */
#define TPP_EXT_CPP_BLANK TPP_EXT_CPP_BLANK
TPP_EXTENSION(TPP_EXT_CPP_BLANK, TPP_EXTNAME_CPP_BLANK, TPP_CONF_DEFAULT(TPP_HAVE_CPP_BLANK))
#define _tpp_lexer_has_CPP_BLANK(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_BLANK)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_BLANK) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_DIGIT_LINE)
#ifndef TPP_EXTNAME_CPP_DIGIT_LINE
#define TPP_EXTNAME_CPP_DIGIT_LINE "digit-directives"
#endif /* !TPP_EXTNAME_CPP_DIGIT_LINE */
#define TPP_EXT_CPP_DIGIT_LINE TPP_EXT_CPP_DIGIT_LINE
TPP_EXTENSION(TPP_EXT_CPP_DIGIT_LINE, TPP_EXTNAME_CPP_DIGIT_LINE, TPP_CONF_DEFAULT(TPP_HAVE_CPP_DIGIT_LINE))
#define _tpp_lexer_has_CPP_DIGIT_LINE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_DIGIT_LINE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_DIGIT_LINE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_LINE)
#ifndef TPP_EXTNAME_CPP_LINE
#define TPP_EXTNAME_CPP_LINE "line-directives"
#endif /* !TPP_EXTNAME_CPP_LINE */
#define TPP_EXT_CPP_LINE TPP_EXT_CPP_LINE
TPP_EXTENSION(TPP_EXT_CPP_LINE, TPP_EXTNAME_CPP_LINE, TPP_CONF_DEFAULT(TPP_HAVE_CPP_LINE))
#define _tpp_lexer_has_CPP_LINE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_LINE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_LINE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_INCLUDE)
#ifndef TPP_EXTNAME_CPP_INCLUDE
#define TPP_EXTNAME_CPP_INCLUDE "include-directives"
#endif /* !TPP_EXTNAME_CPP_INCLUDE */
#define TPP_EXT_CPP_INCLUDE TPP_EXT_CPP_INCLUDE
TPP_EXTENSION(TPP_EXT_CPP_INCLUDE, TPP_EXTNAME_CPP_INCLUDE, TPP_CONF_DEFAULT(TPP_HAVE_CPP_INCLUDE))
#define _tpp_lexer_has_CPP_INCLUDE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_INCLUDE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_INCLUDE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_INCLUDE_NEXT)
#ifndef TPP_EXTNAME_CPP_INCLUDE_NEXT
#define TPP_EXTNAME_CPP_INCLUDE_NEXT "include-next-directives"
#endif /* !TPP_EXTNAME_CPP_INCLUDE_NEXT */
#define TPP_EXT_CPP_INCLUDE_NEXT TPP_EXT_CPP_INCLUDE_NEXT
TPP_EXTENSION(TPP_EXT_CPP_INCLUDE_NEXT, TPP_EXTNAME_CPP_INCLUDE_NEXT, TPP_CONF_DEFAULT(TPP_HAVE_CPP_INCLUDE_NEXT))
#define _tpp_lexer_has_CPP_INCLUDE_NEXT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_INCLUDE_NEXT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_INCLUDE_NEXT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_IMPORT)
#ifndef TPP_EXTNAME_CPP_IMPORT
#define TPP_EXTNAME_CPP_IMPORT "import-directives"
#endif /* !TPP_EXTNAME_CPP_IMPORT */
#define TPP_EXT_CPP_IMPORT TPP_EXT_CPP_IMPORT
TPP_EXTENSION(TPP_EXT_CPP_IMPORT, TPP_EXTNAME_CPP_IMPORT, TPP_CONF_DEFAULT(TPP_HAVE_CPP_IMPORT))
#define _tpp_lexer_has_CPP_IMPORT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_IMPORT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_IMPORT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_IF_ELSE_ENDIF)
#ifndef TPP_EXTNAME_CPP_IF_ELSE_ENDIF
#define TPP_EXTNAME_CPP_IF_ELSE_ENDIF "if-directives"
#endif /* !TPP_EXTNAME_CPP_IF_ELSE_ENDIF */
#define TPP_EXT_CPP_IF_ELSE_ENDIF TPP_EXT_CPP_IF_ELSE_ENDIF
TPP_EXTENSION(TPP_EXT_CPP_IF_ELSE_ENDIF, TPP_EXTNAME_CPP_IF_ELSE_ENDIF, TPP_CONF_DEFAULT(TPP_HAVE_CPP_IF_ELSE_ENDIF))
#define _tpp_lexer_has_CPP_IF_ELSE_ENDIF(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_IF_ELSE_ENDIF)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_IF_ELSE_ENDIF) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_DEFINE)
#ifndef TPP_EXTNAME_CPP_DEFINE
#define TPP_EXTNAME_CPP_DEFINE "define-directives"
#endif /* !TPP_EXTNAME_CPP_DEFINE */
#define TPP_EXT_CPP_DEFINE TPP_EXT_CPP_DEFINE
TPP_EXTENSION(TPP_EXT_CPP_DEFINE, TPP_EXTNAME_CPP_DEFINE, TPP_CONF_DEFAULT(TPP_HAVE_CPP_DEFINE))
#define _tpp_lexer_has_CPP_DEFINE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_DEFINE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_DEFINE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_ASSERT)
#ifndef TPP_EXTNAME_CPP_ASSERT
#define TPP_EXTNAME_CPP_ASSERT "assertions"
#endif /* !TPP_EXTNAME_CPP_ASSERT */
#define TPP_EXT_CPP_ASSERT TPP_EXT_CPP_ASSERT
TPP_EXTENSION(TPP_EXT_CPP_ASSERT, TPP_EXTNAME_CPP_ASSERT, TPP_CONF_DEFAULT(TPP_HAVE_CPP_ASSERT))
#define _tpp_lexer_has_CPP_ASSERT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_ASSERT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_ASSERT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_ERROR)
#ifndef TPP_EXTNAME_CPP_ERROR
#define TPP_EXTNAME_CPP_ERROR "error-directives"
#endif /* !TPP_EXTNAME_CPP_ERROR */
#define TPP_EXT_CPP_ERROR TPP_EXT_CPP_ERROR
TPP_EXTENSION(TPP_EXT_CPP_ERROR, TPP_EXTNAME_CPP_ERROR, TPP_CONF_DEFAULT(TPP_HAVE_CPP_ERROR))
#define _tpp_lexer_has_CPP_ERROR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_ERROR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_ERROR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_WARNING)
#ifndef TPP_EXTNAME_CPP_WARNING
#define TPP_EXTNAME_CPP_WARNING "warning-directives"
#endif /* !TPP_EXTNAME_CPP_WARNING */
#define TPP_EXT_CPP_WARNING TPP_EXT_CPP_WARNING
TPP_EXTENSION(TPP_EXT_CPP_WARNING, TPP_EXTNAME_CPP_WARNING, TPP_CONF_DEFAULT(TPP_HAVE_CPP_WARNING))
#define _tpp_lexer_has_CPP_WARNING(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_WARNING)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_WARNING) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_IDENT_SCCS)
#ifndef TPP_EXTNAME_CPP_IDENT_SCCS
#define TPP_EXTNAME_CPP_IDENT_SCCS "ident-directives"
#endif /* !TPP_EXTNAME_CPP_IDENT_SCCS */
#define TPP_EXT_CPP_IDENT_SCCS TPP_EXT_CPP_IDENT_SCCS
TPP_EXTENSION(TPP_EXT_CPP_IDENT_SCCS, TPP_EXTNAME_CPP_IDENT_SCCS, TPP_CONF_DEFAULT(TPP_HAVE_CPP_IDENT_SCCS))
#define _tpp_lexer_has_CPP_IDENT_SCCS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_IDENT_SCCS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_IDENT_SCCS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_PRAGMA)
#ifndef TPP_EXTNAME_CPP_PRAGMA
#define TPP_EXTNAME_CPP_PRAGMA "pragma-directives"
#endif /* !TPP_EXTNAME_CPP_PRAGMA */
#define TPP_EXT_CPP_PRAGMA TPP_EXT_CPP_PRAGMA
TPP_EXTENSION(TPP_EXT_CPP_PRAGMA, TPP_EXTNAME_CPP_PRAGMA, TPP_CONF_DEFAULT(TPP_HAVE_CPP_PRAGMA))
#define _tpp_lexer_has_CPP_PRAGMA(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_PRAGMA)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_PRAGMA) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CPP_EMBED)
#ifndef TPP_EXTNAME_CPP_EMBED
#define TPP_EXTNAME_CPP_EMBED "embed-directives"
#endif /* !TPP_EXTNAME_CPP_EMBED */
#define TPP_EXT_CPP_EMBED TPP_EXT_CPP_EMBED
TPP_EXTENSION(TPP_EXT_CPP_EMBED, TPP_EXTNAME_CPP_EMBED, TPP_CONF_DEFAULT(TPP_HAVE_CPP_EMBED))
#define _tpp_lexer_has_CPP_EMBED(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CPP_EMBED)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CPP_EMBED) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO__Pragma)
#ifndef TPP_EXTNAME_MACRO__Pragma
#define TPP_EXTNAME_MACRO__Pragma "_Pragma"
#endif /* !TPP_EXTNAME_MACRO__Pragma */
#define TPP_EXT_MACRO__Pragma TPP_EXT_MACRO__Pragma
TPP_EXTENSION(TPP_EXT_MACRO__Pragma, TPP_EXTNAME_MACRO__Pragma, TPP_CONF_DEFAULT(TPP_HAVE_MACRO__Pragma))
#define _tpp_lexer_has_MACRO__Pragma(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO__Pragma)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO__Pragma) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___pragma)
#ifndef TPP_EXTNAME_MACRO___pragma
#define TPP_EXTNAME_MACRO___pragma "__pragma"
#endif /* !TPP_EXTNAME_MACRO___pragma */
#define TPP_EXT_MACRO___pragma TPP_EXT_MACRO___pragma
TPP_EXTENSION(TPP_EXT_MACRO___pragma, TPP_EXTNAME_MACRO___pragma, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___pragma))
#define _tpp_lexer_has_MACRO___pragma(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___pragma)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___pragma) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_attribute)
#ifndef TPP_EXTNAME_CLANG_MACRO___has_attribute
#define TPP_EXTNAME_CLANG_MACRO___has_attribute "clang-__has_attribute"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_attribute */
#define TPP_EXT_CLANG_MACRO___has_attribute TPP_EXT_CLANG_MACRO___has_attribute
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_attribute, TPP_EXTNAME_CLANG_MACRO___has_attribute, TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_attribute))
#define _tpp_lexer_has_CLANG_MACRO___has_attribute(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CLANG_MACRO___has_attribute)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_attribute) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_builtin)
#ifndef TPP_EXTNAME_CLANG_MACRO___has_builtin
#define TPP_EXTNAME_CLANG_MACRO___has_builtin "clang-__has_builtin"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_builtin */
#define TPP_EXT_CLANG_MACRO___has_builtin TPP_EXT_CLANG_MACRO___has_builtin
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_builtin, TPP_EXTNAME_CLANG_MACRO___has_builtin, TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_builtin))
#define _tpp_lexer_has_CLANG_MACRO___has_builtin(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CLANG_MACRO___has_builtin)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_builtin) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute)
#ifndef TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute
#define TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute "clang-__has_cpp_attribute"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute */
#define TPP_EXT_CLANG_MACRO___has_cpp_attribute TPP_EXT_CLANG_MACRO___has_cpp_attribute
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_cpp_attribute, TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute, TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute))
#define _tpp_lexer_has_CLANG_MACRO___has_cpp_attribute(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CLANG_MACRO___has_cpp_attribute)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute)
#ifndef TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute
#define TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute "clang-__has_declspec_attribute"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute */
#define TPP_EXT_CLANG_MACRO___has_declspec_attribute TPP_EXT_CLANG_MACRO___has_declspec_attribute
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_declspec_attribute, TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute, TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute))
#define _tpp_lexer_has_CLANG_MACRO___has_declspec_attribute(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CLANG_MACRO___has_declspec_attribute)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_extension)
#ifndef TPP_EXTNAME_CLANG_MACRO___has_extension
#define TPP_EXTNAME_CLANG_MACRO___has_extension "clang-__has_extension"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_extension */
#define TPP_EXT_CLANG_MACRO___has_extension TPP_EXT_CLANG_MACRO___has_extension
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_extension, TPP_EXTNAME_CLANG_MACRO___has_extension, TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_extension))
#define _tpp_lexer_has_CLANG_MACRO___has_extension(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CLANG_MACRO___has_extension)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_extension) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_feature)
#ifndef TPP_EXTNAME_CLANG_MACRO___has_feature
#define TPP_EXTNAME_CLANG_MACRO___has_feature "clang-__has_feature"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_feature */
#define TPP_EXT_CLANG_MACRO___has_feature TPP_EXT_CLANG_MACRO___has_feature
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_feature, TPP_EXTNAME_CLANG_MACRO___has_feature, TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_feature))
#define _tpp_lexer_has_CLANG_MACRO___has_feature(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CLANG_MACRO___has_feature)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_feature) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_c_attribute)
#ifndef TPP_EXTNAME_CLANG_MACRO___has_c_attribute
#define TPP_EXTNAME_CLANG_MACRO___has_c_attribute "clang-__has_c_attribute"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_c_attribute */
#define TPP_EXT_CLANG_MACRO___has_c_attribute TPP_EXT_CLANG_MACRO___has_c_attribute
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_c_attribute, TPP_EXTNAME_CLANG_MACRO___has_c_attribute, TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_c_attribute))
#define _tpp_lexer_has_CLANG_MACRO___has_c_attribute(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CLANG_MACRO___has_c_attribute)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CLANG_MACRO___has_c_attribute) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES)
#ifndef TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES
#define TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES "clang-extensions-are-features"
#endif /* !TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES */
#define TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES
TPP_EXTENSION(TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES, TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES, TPP_CONF_DEFAULT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES))
#define _tpp_lexer_has_CLANG_EXTENSIONS_ARE_FEATURES(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___is_identifier)
#ifndef TPP_EXTNAME_MACRO___is_identifier
#define TPP_EXTNAME_MACRO___is_identifier "__is_identifier"
#endif /* !TPP_EXTNAME_MACRO___is_identifier */
#define TPP_EXT_MACRO___is_identifier TPP_EXT_MACRO___is_identifier
TPP_EXTENSION(TPP_EXT_MACRO___is_identifier, TPP_EXTNAME_MACRO___is_identifier, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___is_identifier))
#define _tpp_lexer_has_MACRO___is_identifier(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___is_identifier)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___is_identifier) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___is_deprecated)
#ifndef TPP_EXTNAME_MACRO___is_deprecated
#define TPP_EXTNAME_MACRO___is_deprecated "__is_deprecated"
#endif /* !TPP_EXTNAME_MACRO___is_deprecated */
#define TPP_EXT_MACRO___is_deprecated TPP_EXT_MACRO___is_deprecated
TPP_EXTENSION(TPP_EXT_MACRO___is_deprecated, TPP_EXTNAME_MACRO___is_deprecated, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___is_deprecated))
#define _tpp_lexer_has_MACRO___is_deprecated(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___is_deprecated)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___is_deprecated) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___is_poisoned)
#ifndef TPP_EXTNAME_MACRO___is_poisoned
#define TPP_EXTNAME_MACRO___is_poisoned "__is_poisoned"
#endif /* !TPP_EXTNAME_MACRO___is_poisoned */
#define TPP_EXT_MACRO___is_poisoned TPP_EXT_MACRO___is_poisoned
TPP_EXTENSION(TPP_EXT_MACRO___is_poisoned, TPP_EXTNAME_MACRO___is_poisoned, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___is_poisoned))
#define _tpp_lexer_has_MACRO___is_poisoned(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___is_poisoned)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___is_poisoned) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_extension)
#ifndef TPP_EXTNAME_MACRO___has_extension
#define TPP_EXTNAME_MACRO___has_extension "__has_extension"
#endif /* !TPP_EXTNAME_MACRO___has_extension */
#define TPP_EXT_MACRO___has_extension TPP_EXT_MACRO___has_extension
TPP_EXTENSION(TPP_EXT_MACRO___has_extension, TPP_EXTNAME_MACRO___has_extension, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_extension))
#define _tpp_lexer_has_MACRO___has_extension(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___has_extension)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_extension) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_known_extension)
#ifndef TPP_EXTNAME_MACRO___has_known_extension
#define TPP_EXTNAME_MACRO___has_known_extension "__has_known_extension"
#endif /* !TPP_EXTNAME_MACRO___has_known_extension */
#define TPP_EXT_MACRO___has_known_extension TPP_EXT_MACRO___has_known_extension
TPP_EXTENSION(TPP_EXT_MACRO___has_known_extension, TPP_EXTNAME_MACRO___has_known_extension, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_known_extension))
#define _tpp_lexer_has_MACRO___has_known_extension(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___has_known_extension)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_known_extension) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_warning)
#ifndef TPP_EXTNAME_MACRO___has_warning
#define TPP_EXTNAME_MACRO___has_warning "__has_warning"
#endif /* !TPP_EXTNAME_MACRO___has_warning */
#define TPP_EXT_MACRO___has_warning TPP_EXT_MACRO___has_warning
TPP_EXTENSION(TPP_EXT_MACRO___has_warning, TPP_EXTNAME_MACRO___has_warning, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_warning))
#define _tpp_lexer_has_MACRO___has_warning(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___has_warning)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_warning) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_known_warning)
#ifndef TPP_EXTNAME_MACRO___has_known_warning
#define TPP_EXTNAME_MACRO___has_known_warning "__has_known_warning"
#endif /* !TPP_EXTNAME_MACRO___has_known_warning */
#define TPP_EXT_MACRO___has_known_warning TPP_EXT_MACRO___has_known_warning
TPP_EXTENSION(TPP_EXT_MACRO___has_known_warning, TPP_EXTNAME_MACRO___has_known_warning, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_known_warning))
#define _tpp_lexer_has_MACRO___has_known_warning(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___has_known_warning)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_known_warning) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_include)
#ifndef TPP_EXTNAME_MACRO___has_include
#define TPP_EXTNAME_MACRO___has_include "__has_include"
#endif /* !TPP_EXTNAME_MACRO___has_include */
#define TPP_EXT_MACRO___has_include TPP_EXT_MACRO___has_include
TPP_EXTENSION(TPP_EXT_MACRO___has_include, TPP_EXTNAME_MACRO___has_include, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_include))
#define _tpp_lexer_has_MACRO___has_include(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___has_include)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_include) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_include_next)
#ifndef TPP_EXTNAME_MACRO___has_include_next
#define TPP_EXTNAME_MACRO___has_include_next "__has_include_next"
#endif /* !TPP_EXTNAME_MACRO___has_include_next */
#define TPP_EXT_MACRO___has_include_next TPP_EXT_MACRO___has_include_next
TPP_EXTENSION(TPP_EXT_MACRO___has_include_next, TPP_EXTNAME_MACRO___has_include_next, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_include_next))
#define _tpp_lexer_has_MACRO___has_include_next(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___has_include_next)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_include_next) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_embed)
#ifndef TPP_EXTNAME_MACRO___has_embed
#define TPP_EXTNAME_MACRO___has_embed "__has_embed"
#endif /* !TPP_EXTNAME_MACRO___has_embed */
#define TPP_EXT_MACRO___has_embed TPP_EXT_MACRO___has_embed
TPP_EXTENSION(TPP_EXT_MACRO___has_embed, TPP_EXTNAME_MACRO___has_embed, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_embed))
#define _tpp_lexer_has_MACRO___has_embed(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___has_embed)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___has_embed) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___FILE__)
#ifndef TPP_EXTNAME_MACRO___FILE__
#define TPP_EXTNAME_MACRO___FILE__ "__FILE__"
#endif /* !TPP_EXTNAME_MACRO___FILE__ */
#define TPP_EXT_MACRO___FILE__ TPP_EXT_MACRO___FILE__
TPP_EXTENSION(TPP_EXT_MACRO___FILE__, TPP_EXTNAME_MACRO___FILE__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___FILE__))
#define _tpp_lexer_has_MACRO___FILE__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___FILE__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___FILE__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___LINE__)
#ifndef TPP_EXTNAME_MACRO___LINE__
#define TPP_EXTNAME_MACRO___LINE__ "__LINE__"
#endif /* !TPP_EXTNAME_MACRO___LINE__ */
#define TPP_EXT_MACRO___LINE__ TPP_EXT_MACRO___LINE__
TPP_EXTENSION(TPP_EXT_MACRO___LINE__, TPP_EXTNAME_MACRO___LINE__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___LINE__))
#define _tpp_lexer_has_MACRO___LINE__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___LINE__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___LINE__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TIME__)
#ifndef TPP_EXTNAME_MACRO___TIME__
#define TPP_EXTNAME_MACRO___TIME__ "__TIME__"
#endif /* !TPP_EXTNAME_MACRO___TIME__ */
#define TPP_EXT_MACRO___TIME__ TPP_EXT_MACRO___TIME__
TPP_EXTENSION(TPP_EXT_MACRO___TIME__, TPP_EXTNAME_MACRO___TIME__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TIME__))
#define _tpp_lexer_has_MACRO___TIME__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TIME__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TIME__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___DATE__)
#ifndef TPP_EXTNAME_MACRO___DATE__
#define TPP_EXTNAME_MACRO___DATE__ "__DATE__"
#endif /* !TPP_EXTNAME_MACRO___DATE__ */
#define TPP_EXT_MACRO___DATE__ TPP_EXT_MACRO___DATE__
TPP_EXTENSION(TPP_EXT_MACRO___DATE__, TPP_EXTNAME_MACRO___DATE__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___DATE__))
#define _tpp_lexer_has_MACRO___DATE__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___DATE__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___DATE__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___COLUMN__)
#ifndef TPP_EXTNAME_MACRO___COLUMN__
#define TPP_EXTNAME_MACRO___COLUMN__ "column-macro"
#endif /* !TPP_EXTNAME_MACRO___COLUMN__ */
#define TPP_EXT_MACRO___COLUMN__ TPP_EXT_MACRO___COLUMN__
TPP_EXTENSION(TPP_EXT_MACRO___COLUMN__, TPP_EXTNAME_MACRO___COLUMN__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___COLUMN__))
#define _tpp_lexer_has_MACRO___COLUMN__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___COLUMN__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___COLUMN__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___BASE_FILE__)
#ifndef TPP_EXTNAME_MACRO___BASE_FILE__
#define TPP_EXTNAME_MACRO___BASE_FILE__ "basefile-macro"
#endif /* !TPP_EXTNAME_MACRO___BASE_FILE__ */
#define TPP_EXT_MACRO___BASE_FILE__ TPP_EXT_MACRO___BASE_FILE__
TPP_EXTENSION(TPP_EXT_MACRO___BASE_FILE__, TPP_EXTNAME_MACRO___BASE_FILE__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___BASE_FILE__))
#define _tpp_lexer_has_MACRO___BASE_FILE__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___BASE_FILE__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___BASE_FILE__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___FILE_NAME__)
#ifndef TPP_EXTNAME_MACRO___FILE_NAME__
#define TPP_EXTNAME_MACRO___FILE_NAME__ "__FILE_NAME__"
#endif /* !TPP_EXTNAME_MACRO___FILE_NAME__ */
#define TPP_EXT_MACRO___FILE_NAME__ TPP_EXT_MACRO___FILE_NAME__
TPP_EXTENSION(TPP_EXT_MACRO___FILE_NAME__, TPP_EXTNAME_MACRO___FILE_NAME__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___FILE_NAME__))
#define _tpp_lexer_has_MACRO___FILE_NAME__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___FILE_NAME__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___FILE_NAME__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___INCLUDE_LEVEL__)
#ifndef TPP_EXTNAME_MACRO___INCLUDE_LEVEL__
#define TPP_EXTNAME_MACRO___INCLUDE_LEVEL__ "include-level-macro"
#endif /* !TPP_EXTNAME_MACRO___INCLUDE_LEVEL__ */
#define TPP_EXT_MACRO___INCLUDE_LEVEL__ TPP_EXT_MACRO___INCLUDE_LEVEL__
TPP_EXTENSION(TPP_EXT_MACRO___INCLUDE_LEVEL__, TPP_EXTNAME_MACRO___INCLUDE_LEVEL__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___INCLUDE_LEVEL__))
#define _tpp_lexer_has_MACRO___INCLUDE_LEVEL__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___INCLUDE_LEVEL__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___INCLUDE_LEVEL__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___INCLUDE_DEPTH__)
#ifndef TPP_EXTNAME_MACRO___INCLUDE_DEPTH__
#define TPP_EXTNAME_MACRO___INCLUDE_DEPTH__ "include-depth-macro"
#endif /* !TPP_EXTNAME_MACRO___INCLUDE_DEPTH__ */
#define TPP_EXT_MACRO___INCLUDE_DEPTH__ TPP_EXT_MACRO___INCLUDE_DEPTH__
TPP_EXTENSION(TPP_EXT_MACRO___INCLUDE_DEPTH__, TPP_EXTNAME_MACRO___INCLUDE_DEPTH__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___INCLUDE_DEPTH__))
#define _tpp_lexer_has_MACRO___INCLUDE_DEPTH__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___INCLUDE_DEPTH__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___INCLUDE_DEPTH__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___COUNTER__)
#ifndef TPP_EXTNAME_MACRO___COUNTER__
#define TPP_EXTNAME_MACRO___COUNTER__ "counter-macro"
#endif /* !TPP_EXTNAME_MACRO___COUNTER__ */
#define TPP_EXT_MACRO___COUNTER__ TPP_EXT_MACRO___COUNTER__
TPP_EXTENSION(TPP_EXT_MACRO___COUNTER__, TPP_EXTNAME_MACRO___COUNTER__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___COUNTER__))
#define _tpp_lexer_has_MACRO___COUNTER__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___COUNTER__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___COUNTER__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TIMESTAMP__)
#ifndef TPP_EXTNAME_MACRO___TIMESTAMP__
#define TPP_EXTNAME_MACRO___TIMESTAMP__ "timestamp-macro"
#endif /* !TPP_EXTNAME_MACRO___TIMESTAMP__ */
#define TPP_EXT_MACRO___TIMESTAMP__ TPP_EXT_MACRO___TIMESTAMP__
TPP_EXTENSION(TPP_EXT_MACRO___TIMESTAMP__, TPP_EXTNAME_MACRO___TIMESTAMP__, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TIMESTAMP__))
#define _tpp_lexer_has_MACRO___TIMESTAMP__(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TIMESTAMP__)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TIMESTAMP__) */
#if TPP_CONF_IS_EXT(TPP_HAVE_NUMERIC_DATE_MACROS)
#ifndef TPP_EXTNAME_NUMERIC_DATE_MACROS
#define TPP_EXTNAME_NUMERIC_DATE_MACROS "numeric-date-macros"
#endif /* !TPP_EXTNAME_NUMERIC_DATE_MACROS */
#define TPP_EXT_NUMERIC_DATE_MACROS TPP_EXT_NUMERIC_DATE_MACROS
TPP_EXTENSION(TPP_EXT_NUMERIC_DATE_MACROS, TPP_EXTNAME_NUMERIC_DATE_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_NUMERIC_DATE_MACROS))
#define _tpp_lexer_has_NUMERIC_DATE_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_NUMERIC_DATE_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_NUMERIC_DATE_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_NUMERIC_TIME_MACROS)
#ifndef TPP_EXTNAME_NUMERIC_TIME_MACROS
#define TPP_EXTNAME_NUMERIC_TIME_MACROS "numeric-time-macros"
#endif /* !TPP_EXTNAME_NUMERIC_TIME_MACROS */
#define TPP_EXT_NUMERIC_TIME_MACROS TPP_EXT_NUMERIC_TIME_MACROS
TPP_EXTENSION(TPP_EXT_NUMERIC_TIME_MACROS, TPP_EXTNAME_NUMERIC_TIME_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_NUMERIC_TIME_MACROS))
#define _tpp_lexer_has_NUMERIC_TIME_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_NUMERIC_TIME_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_NUMERIC_TIME_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_EVAL)
#ifndef TPP_EXTNAME_MACRO___TPP_EVAL
#define TPP_EXTNAME_MACRO___TPP_EVAL "tpp-eval-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_EVAL */
#define TPP_EXT_MACRO___TPP_EVAL TPP_EXT_MACRO___TPP_EVAL
TPP_EXTENSION(TPP_EXT_MACRO___TPP_EVAL, TPP_EXTNAME_MACRO___TPP_EVAL, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_EVAL))
#define _tpp_lexer_has_MACRO___TPP_EVAL(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_EVAL)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_EVAL) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_UNIQUE)
#ifndef TPP_EXTNAME_MACRO___TPP_UNIQUE
#define TPP_EXTNAME_MACRO___TPP_UNIQUE "tpp-unique-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_UNIQUE */
#define TPP_EXT_MACRO___TPP_UNIQUE TPP_EXT_MACRO___TPP_UNIQUE
TPP_EXTENSION(TPP_EXT_MACRO___TPP_UNIQUE, TPP_EXTNAME_MACRO___TPP_UNIQUE, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_UNIQUE))
#define _tpp_lexer_has_MACRO___TPP_UNIQUE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_UNIQUE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_UNIQUE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_LOAD_FILE)
#ifndef TPP_EXTNAME_MACRO___TPP_LOAD_FILE
#define TPP_EXTNAME_MACRO___TPP_LOAD_FILE "tpp-load-file-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_LOAD_FILE */
#define TPP_EXT_MACRO___TPP_LOAD_FILE TPP_EXT_MACRO___TPP_LOAD_FILE
TPP_EXTENSION(TPP_EXT_MACRO___TPP_LOAD_FILE, TPP_EXTNAME_MACRO___TPP_LOAD_FILE, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_LOAD_FILE))
#define _tpp_lexer_has_MACRO___TPP_LOAD_FILE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_LOAD_FILE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_LOAD_FILE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_COUNTER)
#ifndef TPP_EXTNAME_MACRO___TPP_COUNTER
#define TPP_EXTNAME_MACRO___TPP_COUNTER "tpp-counter-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_COUNTER */
#define TPP_EXT_MACRO___TPP_COUNTER TPP_EXT_MACRO___TPP_COUNTER
TPP_EXTENSION(TPP_EXT_MACRO___TPP_COUNTER, TPP_EXTNAME_MACRO___TPP_COUNTER, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_COUNTER))
#define _tpp_lexer_has_MACRO___TPP_COUNTER(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_COUNTER)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_COUNTER) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_RANDOM)
#ifndef TPP_EXTNAME_MACRO___TPP_RANDOM
#define TPP_EXTNAME_MACRO___TPP_RANDOM "tpp-random-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_RANDOM */
#define TPP_EXT_MACRO___TPP_RANDOM TPP_EXT_MACRO___TPP_RANDOM
TPP_EXTENSION(TPP_EXT_MACRO___TPP_RANDOM, TPP_EXTNAME_MACRO___TPP_RANDOM, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_RANDOM))
#define _tpp_lexer_has_MACRO___TPP_RANDOM(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_RANDOM)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_RANDOM) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE)
#ifndef TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE
#define TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE "tpp-str-decompile-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE */
#define TPP_EXT_MACRO___TPP_STR_DECOMPILE TPP_EXT_MACRO___TPP_STR_DECOMPILE
TPP_EXTENSION(TPP_EXT_MACRO___TPP_STR_DECOMPILE, TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE))
#define _tpp_lexer_has_MACRO___TPP_STR_DECOMPILE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_STR_DECOMPILE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_STR_PACK)
#ifndef TPP_EXTNAME_MACRO___TPP_STR_PACK
#define TPP_EXTNAME_MACRO___TPP_STR_PACK "tpp-str-pack-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_STR_PACK */
#define TPP_EXT_MACRO___TPP_STR_PACK TPP_EXT_MACRO___TPP_STR_PACK
TPP_EXTENSION(TPP_EXT_MACRO___TPP_STR_PACK, TPP_EXTNAME_MACRO___TPP_STR_PACK, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_PACK))
#define _tpp_lexer_has_MACRO___TPP_STR_PACK(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_STR_PACK)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_STR_PACK) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_STR_SUBSTR)
#ifndef TPP_EXTNAME_MACRO___TPP_STR_SUBSTR
#define TPP_EXTNAME_MACRO___TPP_STR_SUBSTR "tpp-str-substr-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_STR_SUBSTR */
#define TPP_EXT_MACRO___TPP_STR_SUBSTR TPP_EXT_MACRO___TPP_STR_SUBSTR
TPP_EXTENSION(TPP_EXT_MACRO___TPP_STR_SUBSTR, TPP_EXTNAME_MACRO___TPP_STR_SUBSTR, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_SUBSTR))
#define _tpp_lexer_has_MACRO___TPP_STR_SUBSTR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_STR_SUBSTR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_STR_SUBSTR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_STR_SIZE)
#ifndef TPP_EXTNAME_MACRO___TPP_STR_SIZE
#define TPP_EXTNAME_MACRO___TPP_STR_SIZE "tpp-str-size-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_STR_SIZE */
#define TPP_EXT_MACRO___TPP_STR_SIZE TPP_EXT_MACRO___TPP_STR_SIZE
TPP_EXTENSION(TPP_EXT_MACRO___TPP_STR_SIZE, TPP_EXTNAME_MACRO___TPP_STR_SIZE, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_SIZE))
#define _tpp_lexer_has_MACRO___TPP_STR_SIZE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_STR_SIZE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_STR_SIZE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS)
#ifndef TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS
#define TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS "tpp-count-tokens-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS */
#define TPP_EXT_MACRO___TPP_COUNT_TOKENS TPP_EXT_MACRO___TPP_COUNT_TOKENS
TPP_EXTENSION(TPP_EXT_MACRO___TPP_COUNT_TOKENS, TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS))
#define _tpp_lexer_has_MACRO___TPP_COUNT_TOKENS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_COUNT_TOKENS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_IDENTIFIER)
#ifndef TPP_EXTNAME_MACRO___TPP_IDENTIFIER
#define TPP_EXTNAME_MACRO___TPP_IDENTIFIER "tpp-identifier-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_IDENTIFIER */
#define TPP_EXT_MACRO___TPP_IDENTIFIER TPP_EXT_MACRO___TPP_IDENTIFIER
TPP_EXTENSION(TPP_EXT_MACRO___TPP_IDENTIFIER, TPP_EXTNAME_MACRO___TPP_IDENTIFIER, TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_IDENTIFIER))
#define _tpp_lexer_has_MACRO___TPP_IDENTIFIER(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO___TPP_IDENTIFIER)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO___TPP_IDENTIFIER) */
#if TPP_CONF_IS_EXT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS)
#ifndef TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS "alternative-macro-parenthesis"
#endif /* !TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS */
#define TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
TPP_EXTENSION(TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS, TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS, TPP_CONF_DEFAULT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS))
#define _tpp_lexer_has_ALTERNATIVE_MACRO_PARENTHESIS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
#ifndef TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE
#define TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE "macro-argument-whitespace"
#endif /* !TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE */
#define TPP_EXT_MACRO_ARGUMENT_WHITESPACE TPP_EXT_MACRO_ARGUMENT_WHITESPACE
TPP_EXTENSION(TPP_EXT_MACRO_ARGUMENT_WHITESPACE, TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE, TPP_CONF_DEFAULT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE))
#define _tpp_lexer_has_MACRO_ARGUMENT_WHITESPACE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO_ARGUMENT_WHITESPACE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION)
#ifndef TPP_EXTNAME_MACRO_RECURSION
#define TPP_EXTNAME_MACRO_RECURSION "macro-recursion"
#endif /* !TPP_EXTNAME_MACRO_RECURSION */
#define TPP_EXT_MACRO_RECURSION TPP_EXT_MACRO_RECURSION
TPP_EXTENSION(TPP_EXT_MACRO_RECURSION, TPP_EXTNAME_MACRO_RECURSION, TPP_CONF_DEFAULT(TPP_HAVE_MACRO_RECURSION))
#define _tpp_lexer_has_MACRO_RECURSION(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_MACRO_RECURSION)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION) */
#if TPP_CONF_IS_EXT(TPP_HAVE_TRADITIONAL_MACROS)
#ifndef TPP_EXTNAME_TRADITIONAL_MACROS
#define TPP_EXTNAME_TRADITIONAL_MACROS "traditional-macro"
#endif /* !TPP_EXTNAME_TRADITIONAL_MACROS */
#define TPP_EXT_TRADITIONAL_MACROS TPP_EXT_TRADITIONAL_MACROS
TPP_EXTENSION(TPP_EXT_TRADITIONAL_MACROS, TPP_EXTNAME_TRADITIONAL_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_TRADITIONAL_MACROS))
#define _tpp_lexer_has_TRADITIONAL_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_TRADITIONAL_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_TRADITIONAL_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_NAMED_VARARGS_IN_MACROS)
#ifndef TPP_EXTNAME_NAMED_VARARGS_IN_MACROS
#define TPP_EXTNAME_NAMED_VARARGS_IN_MACROS "named-varargs-in-macros"
#endif /* !TPP_EXTNAME_NAMED_VARARGS_IN_MACROS */
#define TPP_EXT_NAMED_VARARGS_IN_MACROS TPP_EXT_NAMED_VARARGS_IN_MACROS
TPP_EXTENSION(TPP_EXT_NAMED_VARARGS_IN_MACROS, TPP_EXTNAME_NAMED_VARARGS_IN_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_NAMED_VARARGS_IN_MACROS))
#define _tpp_lexer_has_NAMED_VARARGS_IN_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_NAMED_VARARGS_IN_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_NAMED_VARARGS_IN_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_VA_ARGS_IN_MACROS)
#ifndef TPP_EXTNAME_VA_ARGS_IN_MACROS
#define TPP_EXTNAME_VA_ARGS_IN_MACROS "va-args-in-macros"
#endif /* !TPP_EXTNAME_VA_ARGS_IN_MACROS */
#define TPP_EXT_VA_ARGS_IN_MACROS TPP_EXT_VA_ARGS_IN_MACROS
TPP_EXTENSION(TPP_EXT_VA_ARGS_IN_MACROS, TPP_EXTNAME_VA_ARGS_IN_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_VA_ARGS_IN_MACROS))
#define _tpp_lexer_has_VA_ARGS_IN_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_VA_ARGS_IN_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_VA_ARGS_IN_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_VA_COMMA_IN_MACROS)
#ifndef TPP_EXTNAME_VA_COMMA_IN_MACROS
#define TPP_EXTNAME_VA_COMMA_IN_MACROS "va-comma-in-macros"
#endif /* !TPP_EXTNAME_VA_COMMA_IN_MACROS */
#define TPP_EXT_VA_COMMA_IN_MACROS TPP_EXT_VA_COMMA_IN_MACROS
TPP_EXTENSION(TPP_EXT_VA_COMMA_IN_MACROS, TPP_EXTNAME_VA_COMMA_IN_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_VA_COMMA_IN_MACROS))
#define _tpp_lexer_has_VA_COMMA_IN_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_VA_COMMA_IN_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_VA_COMMA_IN_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_VA_OPT_IN_MACROS)
#ifndef TPP_EXTNAME_VA_OPT_IN_MACROS
#define TPP_EXTNAME_VA_OPT_IN_MACROS "va-opt-in-macros"
#endif /* !TPP_EXTNAME_VA_OPT_IN_MACROS */
#define TPP_EXT_VA_OPT_IN_MACROS TPP_EXT_VA_OPT_IN_MACROS
TPP_EXTENSION(TPP_EXT_VA_OPT_IN_MACROS, TPP_EXTNAME_VA_OPT_IN_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_VA_OPT_IN_MACROS))
#define _tpp_lexer_has_VA_OPT_IN_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_VA_OPT_IN_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_VA_OPT_IN_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_VA_NARGS_IN_MACROS)
#ifndef TPP_EXTNAME_VA_NARGS_IN_MACROS
#define TPP_EXTNAME_VA_NARGS_IN_MACROS "va-nargs-in-macros"
#endif /* !TPP_EXTNAME_VA_NARGS_IN_MACROS */
#define TPP_EXT_VA_NARGS_IN_MACROS TPP_EXT_VA_NARGS_IN_MACROS
TPP_EXTENSION(TPP_EXT_VA_NARGS_IN_MACROS, TPP_EXTNAME_VA_NARGS_IN_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_VA_NARGS_IN_MACROS))
#define _tpp_lexer_has_VA_NARGS_IN_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_VA_NARGS_IN_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_VA_NARGS_IN_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS)
#ifndef TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS
#define TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS "glue-comma-in-macros"
#endif /* !TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS */
#define TPP_EXT_VA_GLUE_COMMA_IN_MACROS TPP_EXT_VA_GLUE_COMMA_IN_MACROS
TPP_EXTENSION(TPP_EXT_VA_GLUE_COMMA_IN_MACROS, TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS, TPP_CONF_DEFAULT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS))
#define _tpp_lexer_has_VA_GLUE_COMMA_IN_MACROS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_VA_GLUE_COMMA_IN_MACROS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT)
#ifndef TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT
#define TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT "stringize-macro-argument"
#endif /* !TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT */
#define TPP_EXT_STRINGIZE_MACRO_ARGUMENT TPP_EXT_STRINGIZE_MACRO_ARGUMENT
TPP_EXTENSION(TPP_EXT_STRINGIZE_MACRO_ARGUMENT, TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT, TPP_CONF_DEFAULT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT))
#define _tpp_lexer_has_STRINGIZE_MACRO_ARGUMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_STRINGIZE_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT)
#ifndef TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT
#define TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT "charize-macro-argument"
#endif /* !TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT */
#define TPP_EXT_CHARIZE_MACRO_ARGUMENT TPP_EXT_CHARIZE_MACRO_ARGUMENT
TPP_EXTENSION(TPP_EXT_CHARIZE_MACRO_ARGUMENT, TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT, TPP_CONF_DEFAULT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT))
#define _tpp_lexer_has_CHARIZE_MACRO_ARGUMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_CHARIZE_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT)
#ifndef TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT
#define TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT "dont-expand-macro-argument"
#endif /* !TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT */
#define TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT
TPP_EXTENSION(TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT, TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT, TPP_CONF_DEFAULT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT))
#define _tpp_lexer_has_DONT_EXPAND_MACRO_ARGUMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_GLUE_MACRO_ARGUMENT)
#ifndef TPP_EXTNAME_GLUE_MACRO_ARGUMENT
#define TPP_EXTNAME_GLUE_MACRO_ARGUMENT "glue-macro-argument"
#endif /* !TPP_EXTNAME_GLUE_MACRO_ARGUMENT */
#define TPP_EXT_GLUE_MACRO_ARGUMENT TPP_EXT_GLUE_MACRO_ARGUMENT
TPP_EXTENSION(TPP_EXT_GLUE_MACRO_ARGUMENT, TPP_EXTNAME_GLUE_MACRO_ARGUMENT, TPP_CONF_DEFAULT(TPP_HAVE_GLUE_MACRO_ARGUMENT))
#define _tpp_lexer_has_GLUE_MACRO_ARGUMENT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_GLUE_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_GLUE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_PUSH_MACRO)
#ifndef TPP_EXTNAME_PRAGMA_PUSH_MACRO
#define TPP_EXTNAME_PRAGMA_PUSH_MACRO "pragma-push-macro"
#endif /* !TPP_EXTNAME_PRAGMA_PUSH_MACRO */
#define TPP_EXT_PRAGMA_PUSH_MACRO TPP_EXT_PRAGMA_PUSH_MACRO
TPP_EXTENSION(TPP_EXT_PRAGMA_PUSH_MACRO, TPP_EXTNAME_PRAGMA_PUSH_MACRO, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_PUSH_MACRO))
#define _tpp_lexer_has_PRAGMA_PUSH_MACRO(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_PUSH_MACRO)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_PUSH_MACRO) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_ONCE)
#ifndef TPP_EXTNAME_PRAGMA_ONCE
#define TPP_EXTNAME_PRAGMA_ONCE "pragma-once"
#endif /* !TPP_EXTNAME_PRAGMA_ONCE */
#define TPP_EXT_PRAGMA_ONCE TPP_EXT_PRAGMA_ONCE
TPP_EXTENSION(TPP_EXT_PRAGMA_ONCE, TPP_EXTNAME_PRAGMA_ONCE, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_ONCE))
#define _tpp_lexer_has_PRAGMA_ONCE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_ONCE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_ONCE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_DEPRECATED)
#ifndef TPP_EXTNAME_PRAGMA_DEPRECATED
#define TPP_EXTNAME_PRAGMA_DEPRECATED "pragma-deprecated"
#endif /* !TPP_EXTNAME_PRAGMA_DEPRECATED */
#define TPP_EXT_PRAGMA_DEPRECATED TPP_EXT_PRAGMA_DEPRECATED
TPP_EXTENSION(TPP_EXT_PRAGMA_DEPRECATED, TPP_EXTNAME_PRAGMA_DEPRECATED, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_DEPRECATED))
#define _tpp_lexer_has_PRAGMA_DEPRECATED(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_DEPRECATED)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_DEPRECATED) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_EXTENSION)
#ifndef TPP_EXTNAME_PRAGMA_EXTENSION
#define TPP_EXTNAME_PRAGMA_EXTENSION "pragma-extension"
#endif /* !TPP_EXTNAME_PRAGMA_EXTENSION */
#define TPP_EXT_PRAGMA_EXTENSION TPP_EXT_PRAGMA_EXTENSION
TPP_EXTENSION(TPP_EXT_PRAGMA_EXTENSION, TPP_EXTNAME_PRAGMA_EXTENSION, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_EXTENSION))
#define _tpp_lexer_has_PRAGMA_EXTENSION(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_EXTENSION)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_EXTENSION) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_WARNING)
#ifndef TPP_EXTNAME_PRAGMA_WARNING
#define TPP_EXTNAME_PRAGMA_WARNING "pragma-warning"
#endif /* !TPP_EXTNAME_PRAGMA_WARNING */
#define TPP_EXT_PRAGMA_WARNING TPP_EXT_PRAGMA_WARNING
TPP_EXTENSION(TPP_EXT_PRAGMA_WARNING, TPP_EXTNAME_PRAGMA_WARNING, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_WARNING))
#define _tpp_lexer_has_PRAGMA_WARNING(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_WARNING)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_WARNING) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_MESSAGE)
#ifndef TPP_EXTNAME_PRAGMA_MESSAGE
#define TPP_EXTNAME_PRAGMA_MESSAGE "pragma-message"
#endif /* !TPP_EXTNAME_PRAGMA_MESSAGE */
#define TPP_EXT_PRAGMA_MESSAGE TPP_EXT_PRAGMA_MESSAGE
TPP_EXTENSION(TPP_EXT_PRAGMA_MESSAGE, TPP_EXTNAME_PRAGMA_MESSAGE, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_MESSAGE))
#define _tpp_lexer_has_PRAGMA_MESSAGE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_MESSAGE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_MESSAGE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_ERROR)
#ifndef TPP_EXTNAME_PRAGMA_ERROR
#define TPP_EXTNAME_PRAGMA_ERROR "pragma-error"
#endif /* !TPP_EXTNAME_PRAGMA_ERROR */
#define TPP_EXT_PRAGMA_ERROR TPP_EXT_PRAGMA_ERROR
TPP_EXTENSION(TPP_EXT_PRAGMA_ERROR, TPP_EXTNAME_PRAGMA_ERROR, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_ERROR))
#define _tpp_lexer_has_PRAGMA_ERROR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_ERROR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_ERROR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_REGION)
#ifndef TPP_EXTNAME_PRAGMA_REGION
#define TPP_EXTNAME_PRAGMA_REGION "pragma-region"
#endif /* !TPP_EXTNAME_PRAGMA_REGION */
#define TPP_EXT_PRAGMA_REGION TPP_EXT_PRAGMA_REGION
TPP_EXTENSION(TPP_EXT_PRAGMA_REGION, TPP_EXTNAME_PRAGMA_REGION, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_REGION))
#define _tpp_lexer_has_PRAGMA_REGION(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_REGION)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_REGION) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_TPP_EXEC)
#ifndef TPP_EXTNAME_PRAGMA_TPP_EXEC
#define TPP_EXTNAME_PRAGMA_TPP_EXEC "pragma-tpp-exec"
#endif /* !TPP_EXTNAME_PRAGMA_TPP_EXEC */
#define TPP_EXT_PRAGMA_TPP_EXEC TPP_EXT_PRAGMA_TPP_EXEC
TPP_EXTENSION(TPP_EXT_PRAGMA_TPP_EXEC, TPP_EXTNAME_PRAGMA_TPP_EXEC, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_TPP_EXEC))
#define _tpp_lexer_has_PRAGMA_TPP_EXEC(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_TPP_EXEC)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_TPP_EXEC) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS)
#ifndef TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS "pragma-tpp-set-keyword-flags"
#endif /* !TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS */
#define TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS
TPP_EXTENSION(TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS, TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS))
#define _tpp_lexer_has_PRAGMA_TPP_SET_KEYWORD_FLAGS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_POISON)
#ifndef TPP_EXTNAME_PRAGMA_GCC_POISON
#define TPP_EXTNAME_PRAGMA_GCC_POISON "pragma-gcc-poison"
#endif /* !TPP_EXTNAME_PRAGMA_GCC_POISON */
#define TPP_EXT_PRAGMA_GCC_POISON TPP_EXT_PRAGMA_GCC_POISON
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_POISON, TPP_EXTNAME_PRAGMA_GCC_POISON, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_POISON))
#define _tpp_lexer_has_PRAGMA_GCC_POISON(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_GCC_POISON)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_POISON) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_WARNING)
#ifndef TPP_EXTNAME_PRAGMA_GCC_WARNING
#define TPP_EXTNAME_PRAGMA_GCC_WARNING "pragma-gcc-warning"
#endif /* !TPP_EXTNAME_PRAGMA_GCC_WARNING */
#define TPP_EXT_PRAGMA_GCC_WARNING TPP_EXT_PRAGMA_GCC_WARNING
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_WARNING, TPP_EXTNAME_PRAGMA_GCC_WARNING, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_WARNING))
#define _tpp_lexer_has_PRAGMA_GCC_WARNING(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_GCC_WARNING)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_WARNING) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_ERROR)
#ifndef TPP_EXTNAME_PRAGMA_GCC_ERROR
#define TPP_EXTNAME_PRAGMA_GCC_ERROR "pragma-gcc-error"
#endif /* !TPP_EXTNAME_PRAGMA_GCC_ERROR */
#define TPP_EXT_PRAGMA_GCC_ERROR TPP_EXT_PRAGMA_GCC_ERROR
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_ERROR, TPP_EXTNAME_PRAGMA_GCC_ERROR, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_ERROR))
#define _tpp_lexer_has_PRAGMA_GCC_ERROR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_GCC_ERROR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_ERROR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER)
#ifndef TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER
#define TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER "pragma-gcc-system-header"
#endif /* !TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER */
#define TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER, TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER))
#define _tpp_lexer_has_PRAGMA_GCC_SYSTEM_HEADER(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC)
#ifndef TPP_EXTNAME_PRAGMA_GCC_DIAGNOSTIC
#define TPP_EXTNAME_PRAGMA_GCC_DIAGNOSTIC "pragma-gcc-diagnostic"
#endif /* !TPP_EXTNAME_PRAGMA_GCC_DIAGNOSTIC */
#define TPP_EXT_PRAGMA_GCC_DIAGNOSTIC TPP_EXT_PRAGMA_GCC_DIAGNOSTIC
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_DIAGNOSTIC, TPP_EXTNAME_PRAGMA_GCC_DIAGNOSTIC, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC))
#define _tpp_lexer_has_PRAGMA_GCC_DIAGNOSTIC(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_GCC_DIAGNOSTIC)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC) */
#if TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
#ifndef TPP_EXTNAME_PRAGMA_GCC_DEPENDENCY
#define TPP_EXTNAME_PRAGMA_GCC_DEPENDENCY "pragma-gcc-dependency"
#endif /* !TPP_EXTNAME_PRAGMA_GCC_DEPENDENCY */
#define TPP_EXT_PRAGMA_GCC_DEPENDENCY TPP_EXT_PRAGMA_GCC_DEPENDENCY
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_DEPENDENCY, TPP_EXTNAME_PRAGMA_GCC_DEPENDENCY, TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY))
#define _tpp_lexer_has_PRAGMA_GCC_DEPENDENCY(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_PRAGMA_GCC_DEPENDENCY)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_DEFINED)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_DEFINED
#define TPP_EXTNAME_BUILTIN_EXPR_DEFINED "defined-in-expressions"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_DEFINED */
#define TPP_EXT_BUILTIN_EXPR_DEFINED TPP_EXT_BUILTIN_EXPR_DEFINED
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_DEFINED, TPP_EXTNAME_BUILTIN_EXPR_DEFINED, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_DEFINED))
#define _tpp_lexer_has_BUILTIN_EXPR_DEFINED(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_DEFINED)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_DEFINED) */
#if TPP_CONF_IS_EXT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR)
#ifndef TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR
#define TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR "dont-expand-defined"
#endif /* !TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR */
#define TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR
TPP_EXTENSION(TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR, TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR, TPP_CONF_DEFAULT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR))
#define _tpp_lexer_has_DONT_EXPAND_DEFINED_IN_EXPR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_STRINGS)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_STRINGS
#define TPP_EXTNAME_BUILTIN_EXPR_STRINGS "strings-in-expressions"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_STRINGS */
#define TPP_EXT_BUILTIN_EXPR_STRINGS TPP_EXT_BUILTIN_EXPR_STRINGS
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_STRINGS, TPP_EXTNAME_BUILTIN_EXPR_STRINGS, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_STRINGS))
#define _tpp_lexer_has_BUILTIN_EXPR_STRINGS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_STRINGS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_STRINGS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_FLOATS)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_FLOATS
#define TPP_EXTNAME_BUILTIN_EXPR_FLOATS "floats-in-expressions"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_FLOATS */
#define TPP_EXT_BUILTIN_EXPR_FLOATS TPP_EXT_BUILTIN_EXPR_FLOATS
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_FLOATS, TPP_EXTNAME_BUILTIN_EXPR_FLOATS, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_FLOATS))
#define _tpp_lexer_has_BUILTIN_EXPR_FLOATS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_FLOATS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_FLOATS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT "if-else-optional-true"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT */
#define TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT, TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT))
#define _tpp_lexer_has_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
#define TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS "ifelse-in-expressions"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */
#define TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS, TPP_EXTNAME_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS))
#define _tpp_lexer_has_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_LOGICAL_XOR
#define TPP_EXTNAME_BUILTIN_EXPR_LOGICAL_XOR "logical-xor-in-expressions"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_LOGICAL_XOR */
#define TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR, TPP_EXTNAME_BUILTIN_EXPR_LOGICAL_XOR, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR))
#define _tpp_lexer_has_BUILTIN_EXPR_LOGICAL_XOR(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_LOGICAL_XOR)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_BINARY_LITERALS
#define TPP_EXTNAME_BUILTIN_EXPR_BINARY_LITERALS "binary-literals"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_BINARY_LITERALS */
#define TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS, TPP_EXTNAME_BUILTIN_EXPR_BINARY_LITERALS, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS))
#define _tpp_lexer_has_BUILTIN_EXPR_BINARY_LITERALS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_BINARY_LITERALS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_OCTAL_LITERALS
#define TPP_EXTNAME_BUILTIN_EXPR_OCTAL_LITERALS "octal-literals"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_OCTAL_LITERALS */
#define TPP_EXT_BUILTIN_EXPR_OCTAL_LITERALS TPP_EXT_BUILTIN_EXPR_OCTAL_LITERALS
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_OCTAL_LITERALS, TPP_EXTNAME_BUILTIN_EXPR_OCTAL_LITERALS, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS))
#define _tpp_lexer_has_BUILTIN_EXPR_OCTAL_LITERALS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_OCTAL_LITERALS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS
#define TPP_EXTNAME_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS "fixed-type-integrals"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */
#define TPP_EXT_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS TPP_EXT_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS, TPP_EXTNAME_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS))
#define _tpp_lexer_has_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
#define TPP_EXTNAME_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS "fixed-length-integrals"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */
#define TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS, TPP_EXTNAME_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS))
#define _tpp_lexer_has_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS)
#ifndef TPP_EXTNAME_BUILTIN_EXPR_CHARACTER_LITERALS
#define TPP_EXTNAME_BUILTIN_EXPR_CHARACTER_LITERALS "character-literals"
#endif /* !TPP_EXTNAME_BUILTIN_EXPR_CHARACTER_LITERALS */
#define TPP_EXT_BUILTIN_EXPR_CHARACTER_LITERALS TPP_EXT_BUILTIN_EXPR_CHARACTER_LITERALS
TPP_EXTENSION(TPP_EXT_BUILTIN_EXPR_CHARACTER_LITERALS, TPP_EXTNAME_BUILTIN_EXPR_CHARACTER_LITERALS, TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS))
#define _tpp_lexer_has_BUILTIN_EXPR_CHARACTER_LITERALS(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_BUILTIN_EXPR_CHARACTER_LITERALS)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS) */
#if TPP_CONF_IS_EXT(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE)
#ifndef TPP_EXTNAME_INCLUDE_RELATIVE_TO_EVERY_FILE
#define TPP_EXTNAME_INCLUDE_RELATIVE_TO_EVERY_FILE "include-relative-to-every-file"
#endif /* !TPP_EXTNAME_INCLUDE_RELATIVE_TO_EVERY_FILE */
#define TPP_EXT_INCLUDE_RELATIVE_TO_EVERY_FILE TPP_EXT_INCLUDE_RELATIVE_TO_EVERY_FILE
TPP_EXTENSION(TPP_EXT_INCLUDE_RELATIVE_TO_EVERY_FILE, TPP_EXTNAME_INCLUDE_RELATIVE_TO_EVERY_FILE, TPP_CONF_DEFAULT(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE))
#define _tpp_lexer_has_INCLUDE_RELATIVE_TO_EVERY_FILE(self) (self)->TPP_INTERNAL(tl_exts).TPP_INTERNAL(te_state).TPP_INTERNAL(tes_flags).TPP_INTERNAL(tef_TPP_EXT_INCLUDE_RELATIVE_TO_EVERY_FILE)
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE) */
/*[[[end]]]*/






/* Declare builtin warnings. */

/************************************************************************/
/* -Wcomment                                                            */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_COMMENT
#define TPP_HAVE_TPP_WG_COMMENT \
	(TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT || TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED)
#endif /* !TPP_HAVE_TPP_WG_COMMENT */
#if TPP_HAVE_TPP_WG_COMMENT
#define TPP_WG_COMMENT TPP_WG_COMMENT
TPP_WGROUP(TPP_WG_COMMENT, 2("comment", "comments"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_COMMENT */

#if TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT
#define TPP_W_SLASHSTAR_INSIDE_OF_COMMENT TPP_W_SLASHSTAR_INSIDE_OF_COMMENT
TPP_WARNING(TPP_W_SLASHSTAR_INSIDE_OF_COMMENT, 1(TPP_WG_COMMENT), 0(), ~,
            "%[/*%] repeated inside of comment")
#endif /* TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT */

#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
#define TPP_W_LINE_COMMENT_CONTINUED TPP_W_LINE_COMMENT_CONTINUED
TPP_WARNING(TPP_W_LINE_COMMENT_CONTINUED, 1(TPP_WG_COMMENT), 1(4010), TPP_WSTATE_UNDEFINED,
            "line-comment continued")
#endif /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */


/************************************************************************/
/* -Wtrigraphs                                                          */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_TRIGRAPHS
#define TPP_HAVE_TPP_WG_TRIGRAPHS \
	(TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH)
#endif /* !TPP_HAVE_TPP_WG_TRIGRAPHS */
#if TPP_HAVE_TPP_WG_TRIGRAPHS
#define TPP_WG_TRIGRAPHS TPP_WG_TRIGRAPHS
TPP_WGROUP(TPP_WG_TRIGRAPHS, 1("trigraphs"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_TRIGRAPHS */

#if TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH
#define TPP_W_ENCOUNTERED_TRIGRAPH TPP_W_ENCOUNTERED_TRIGRAPH
TPP_WARNING(TPP_W_ENCOUNTERED_TRIGRAPH, 1(TPP_WG_TRIGRAPHS), 0(), ~,
            "encountered trigraph character sequence %.3Pt")
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */


/************************************************************************/
/* -Wsyntax                                                             */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_SYNTAX
#define TPP_HAVE_TPP_WG_SYNTAX                                  \
	(TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED ||            \
	 TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF ||                 \
	 TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF ||                \
	 TPP_HAVE_TPP_W_UNEXPECTED_TOKEN ||                         \
	 TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE ||                        \
	 TPP_HAVE_TPP_W_UNKNOWN_EMBED_PARAMETER ||                  \
	 TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE ||         \
	 TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST || \
	 TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME ||           \
	 TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT ||             \
	 TPP_HAVE_TPP_W_EXPECTED_STRING ||                          \
	 TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING ||                  \
	 TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF ||                         \
	 TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE ||                  \
	 TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF ||                  \
	 TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF ||                         \
	 TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION ||           \
	 TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF ||          \
	 TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED ||        \
	 TPP_HAVE_TPP_W_INVALID_INTEGER ||                          \
	 TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK)
#endif /* !TPP_HAVE_TPP_WG_SYNTAX */
#if TPP_HAVE_TPP_WG_SYNTAX
#define TPP_WG_SYNTAX TPP_WG_SYNTAX
TPP_WGROUP(TPP_WG_SYNTAX, 1("syntax"), TPP_WSTATE_ERROR_OR_FATAL)
#endif /* TPP_HAVE_TPP_WG_SYNTAX */

#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
#define TPP_W_STRING_TERMINATED_BY_LINEFEED TPP_W_STRING_TERMINATED_BY_LINEFEED
TPP_WARNING(TPP_W_STRING_TERMINATED_BY_LINEFEED, 1(TPP_WG_SYNTAX), 0(), ~,
            "string was terminated by a linefeed")
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */

#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#define TPP_W_STRING_TERMINATED_BY_EOF TPP_W_STRING_TERMINATED_BY_EOF
TPP_WARNING(TPP_W_STRING_TERMINATED_BY_EOF, 1(TPP_WG_SYNTAX), 0(), ~,
            "string was terminated by EOF")
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */

#if TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
#define TPP_W_COMMENT_TERMINATED_BY_EOF TPP_W_COMMENT_TERMINATED_BY_EOF
TPP_WARNING(TPP_W_COMMENT_TERMINATED_BY_EOF, 1(TPP_WG_SYNTAX), 0(), ~,
            "comment was terminated by EOF")
#endif /* TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */

#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN
#define TPP_W_UNEXPECTED_TOKEN TPP_W_UNEXPECTED_TOKEN
TPP_WARNING(TPP_W_UNEXPECTED_TOKEN, 1(TPP_WG_SYNTAX), 0(), ~,
            "expected %[%s%] but got %Pt")
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN */

#if TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE
#define TPP_W_UNKNOWN_DIRECTIVE TPP_W_UNKNOWN_DIRECTIVE
TPP_WARNING(TPP_W_UNKNOWN_DIRECTIVE, 1(TPP_WG_SYNTAX), 1(1021), TPP_WSTATE_UNDEFINED,
            "unknown preprocessor directive: %Pt")
#endif /* TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE */

#if TPP_HAVE_TPP_W_UNKNOWN_EMBED_PARAMETER
#define TPP_W_UNKNOWN_EMBED_PARAMETER TPP_W_UNKNOWN_EMBED_PARAMETER
TPP_WARNING(TPP_W_UNKNOWN_EMBED_PARAMETER, 1(TPP_WG_SYNTAX), 0(), TPP_WSTATE_UNDEFINED,
            "unknown %[#embed%] parameter: %Pt")
#endif /* TPP_HAVE_TPP_W_UNKNOWN_EMBED_PARAMETER */

#if TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
#define TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
TPP_WARNING(TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE, 1(TPP_WG_SYNTAX), 1(4006), TPP_WSTATE_UNDEFINED,
            "no macro name given in %[#%s%] directive")
#endif /* TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */

#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
#define TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
TPP_WARNING(TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST, 1(TPP_WG_SYNTAX), 1(2010), TPP_WSTATE_UNDEFINED,
            "unexpected token %Pt in macro parameter list")
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */

#if TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
#define TPP_W_DUPLICATE_MACRO_PARAMETER_NAME TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
TPP_WARNING(TPP_W_DUPLICATE_MACRO_PARAMETER_NAME, 1(TPP_WG_SYNTAX), 1(2009), TPP_WSTATE_UNDEFINED,
            "duplicate macro parameter name %Pt")
#endif /* TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME */

#if TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT
#define TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT
TPP_WARNING(TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT, 1(TPP_WG_SYNTAX), 1(7514), TPP_WSTATE_UNDEFINED,
            "expected %[(%] after %[__VA_OPT__%] in macro body")
#endif /* TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT */

#if TPP_HAVE_TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT
#define TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT
TPP_WARNING(TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT, 1(TPP_WG_SYNTAX), 1(7615), TPP_WSTATE_UNDEFINED,
            "expected %[)%] after %[__VA_OPT__%] in macro body")
#endif /* TPP_HAVE_TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT */

#if TPP_HAVE_TPP_W_EXPECTED_STRING
#define TPP_W_EXPECTED_STRING TPP_W_EXPECTED_STRING
TPP_WARNING(TPP_W_EXPECTED_STRING, 1(TPP_WG_SYNTAX), 1(4081), TPP_WSTATE_UNDEFINED,
            "expected <string>, but got %Pt")
#endif /* TPP_HAVE_TPP_W_EXPECTED_STRING */

#if TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING
#define TPP_W_EXPECTED_INCLUDE_STRING TPP_W_EXPECTED_INCLUDE_STRING
TPP_WARNING(TPP_W_EXPECTED_INCLUDE_STRING, 1(TPP_WG_SYNTAX), 1(2006), TPP_WSTATE_UNDEFINED,
            "expected <include-string>, but got %Pt")
#endif /* TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */

#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
#define TPP_W_EOF_BEFORE_ENDIF TPP_W_EOF_BEFORE_ENDIF
TPP_WARNING(TPP_W_EOF_BEFORE_ENDIF, 1(TPP_WG_SYNTAX), 1(1070), TPP_WSTATE_UNDEFINED,
            "unterminated %[#if%]-directive")
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */

#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE
#define TPP_W_ELIF_OR_ELSE_AFTER_ELSE TPP_W_ELIF_OR_ELSE_AFTER_ELSE
TPP_WARNING_EX(TPP_W_ELIF_OR_ELSE_AFTER_ELSE, 1(TPP_WG_SYNTAX), 1(1022), TPP_WSTATE_UNDEFINED, {
	tpp_ifdef_stack_entry const *const entry = tpp_current_va_arg(tpp_ifdef_stack_entry const *);
	tpp_file *const current_file = tpp_lexer_getfile(self);
	char const *directive_name = tpp_current_va_arg(char const *);
	tpp_warnf1("%[%s%]-directive after %[#else%]\n", directive_name);
	tpp_print_file_and_line_lc(current_file, tpp_ifdef_stack_entry_getupdated(entry));
	tpp_warnf0("note: see associated %[#else%]\n");
	tpp_print_file_and_line_lc(current_file, tpp_ifdef_stack_entry_getcreated(entry));
	tpp_warnf0("note: see associated %[#if%], %[#ifdef%] or %[#ifndef%]\n");
})
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE */

#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF
#define TPP_W_ELIF_OR_ELSE_WITHOUT_IF TPP_W_ELIF_OR_ELSE_WITHOUT_IF
TPP_WARNING(TPP_W_ELIF_OR_ELSE_WITHOUT_IF, 1(TPP_WG_SYNTAX), 2(1018, 1019), TPP_WSTATE_UNDEFINED,
            "%[#%s%]-directive without preceding %[#if%], %[#ifdef%] or %[#ifndef%]")
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF */

#if TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF
#define TPP_W_ENDIF_WITHOUT_IF TPP_W_ENDIF_WITHOUT_IF
TPP_WARNING(TPP_W_ENDIF_WITHOUT_IF, 1(TPP_WG_SYNTAX), 1(1020), TPP_WSTATE_UNDEFINED,
            "%[#endif%]-directive without preceding %[#if%], %[#ifdef%] or %[#ifndef%]")
#endif /* TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF */

#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION
#define TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION
TPP_WARNING(TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION, 1(TPP_WG_SYNTAX), 1(1017), TPP_WSTATE_UNDEFINED,
            "unexpected token %Pt in preprocessor expression")
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION */

#if TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF
#define TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF
TPP_WARNING(TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF, 1(TPP_WG_SYNTAX), 1(1016), TPP_WSTATE_UNDEFINED,
            "expected <keyword> after %[#%s%], but got %Pt")
#endif /* TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF */

#if TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
#define TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
TPP_WARNING(TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED, 1(TPP_WG_SYNTAX), 1(2003), TPP_WSTATE_UNDEFINED,
            "expected <keyword> after %[defined%] in expression, but got %Pt")
#endif /* TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED */

#if TPP_HAVE_TPP_W_INVALID_INTEGER
#define TPP_W_INVALID_INTEGER TPP_W_INVALID_INTEGER
TPP_WARNING(TPP_W_INVALID_INTEGER, 1(TPP_WG_SYNTAX), 1(3688), TPP_WSTATE_UNDEFINED,
            "invalid integer literal: %Pt")
#endif /* TPP_HAVE_TPP_W_INVALID_INTEGER */

#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK
#define TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK
#if TPP_HAVE_TPP_TOK_INT
TPP_WARNING(TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK, 1(TPP_WG_SYNTAX), 0(), ~,
            "expected %[<integer>%], %[<string>%] or %[,%] in %[__TPP_STR_PACK%] but got %Pt")
#else /* TPP_HAVE_TPP_TOK_INT */
TPP_WARNING(TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK, 1(TPP_WG_SYNTAX), 0(), ~,
            "expected %[<integer>%] or %[,%] in %[__TPP_STR_PACK%] but got %Pt")
#endif /* !TPP_HAVE_TPP_TOK_INT */
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK */




/************************************************************************/
/* -Wuser, -Wcpp                                                        */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_USER
#define TPP_HAVE_TPP_WG_USER (TPP_HAVE_TPP_W_ERROR || TPP_HAVE_TPP_W_WARNING)
#endif /* !TPP_HAVE_TPP_WG_USER */
#ifndef TPP_HAVE_TPP_WG_CPP
#define TPP_HAVE_TPP_WG_CPP (TPP_HAVE_TPP_W_WARNING)
#endif /* !TPP_HAVE_TPP_WG_CPP */
#if TPP_HAVE_TPP_WG_USER
#define TPP_WG_USER TPP_WG_USER
TPP_WGROUP(TPP_WG_USER, 1("user"), TPP_WSTATE_ERROR_OR_FATAL)
#endif /* TPP_HAVE_TPP_WG_USER */

#if TPP_HAVE_TPP_WG_CPP
#define TPP_WG_CPP TPP_WG_CPP
TPP_WGROUP(TPP_WG_CPP, 1("cpp"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_CPP */

#if TPP_HAVE_TPP_W_ERROR
#define TPP_W_ERROR TPP_W_ERROR
TPP_WARNING(TPP_W_ERROR, 1(TPP_WG_USER), 0(), ~, "%.*s")
#endif /* TPP_HAVE_TPP_W_ERROR */

#if TPP_HAVE_TPP_W_WARNING
#define TPP_W_WARNING TPP_W_WARNING
TPP_WARNING(TPP_W_WARNING, 2(TPP_WG_USER, TPP_WG_CPP), 0(), ~, "%.*s")
#endif /* TPP_HAVE_TPP_W_WARNING */


/************************************************************************/
/* -Wunknown-pragmas                                                    */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_UNKNOWN_PRAGMAS
#define TPP_HAVE_TPP_WG_UNKNOWN_PRAGMAS (TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS)
#endif /* !TPP_HAVE_TPP_WG_UNKNOWN_PRAGMAS */
#if TPP_HAVE_TPP_WG_UNKNOWN_PRAGMAS
#define TPP_WG_UNKNOWN_PRAGMAS TPP_WG_UNKNOWN_PRAGMAS
TPP_WGROUP(TPP_WG_UNKNOWN_PRAGMAS, 1("unknown-pragmas"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_UNKNOWN_PRAGMAS */

#if TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS
#define TPP_W_UNKNOWN_PRAGMAS TPP_W_UNKNOWN_PRAGMAS
TPP_WARNING(TPP_W_UNKNOWN_PRAGMAS, 1(TPP_WG_UNKNOWN_PRAGMAS), 1(4068), TPP_WSTATE_UNDEFINED,
            "unknown pragma %Pt")
#endif /* TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */

#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
#define TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
TPP_WARNING(TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE, 1(TPP_WG_UNKNOWN_PRAGMAS), 1(4083), TPP_WSTATE_UNDEFINED,
            "extra tokens at end of %[#pragma%] directive: %Pt")
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE */


/************************************************************************/
/* -Wpragma-once-outside-header                                         */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER
#define TPP_HAVE_TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER (TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER)
#endif /* !TPP_HAVE_TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER */
#if TPP_HAVE_TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER
#define TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER
TPP_WGROUP(TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER, 1("pragma-once-outside-header"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER */

#if TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER
#define TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER
TPP_WARNING(TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER, 1(TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER), 0(), ~,
            "%[#pragma once%] in main file")
#endif /* TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER */


/************************************************************************/
/* -Wundef                                                              */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_UNDEF
#define TPP_HAVE_TPP_WG_UNDEF (TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION)
#endif /* !TPP_HAVE_TPP_WG_UNDEF */
#if TPP_HAVE_TPP_WG_UNDEF
#define TPP_WG_UNDEF TPP_WG_UNDEF
TPP_WGROUP(TPP_WG_UNDEF, 1("undef"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_UNDEF */

#if TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION
#define TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION
TPP_WARNING(TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION, 1(TPP_WG_UNDEF), 0(), ~,
            "undefined keyword %Pt is replaced with %[0%] in expression")
#endif /* TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION */


/************************************************************************/
/* -Wendif-labels                                                              */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_ENDIF_LABELS
#define TPP_HAVE_TPP_WG_ENDIF_LABELS (TPP_HAVE_TPP_W_ENDIF_LABELS)
#endif /* !TPP_HAVE_TPP_WG_ENDIF_LABELS */
#if TPP_HAVE_TPP_WG_ENDIF_LABELS
#define TPP_WG_ENDIF_LABELS TPP_WG_ENDIF_LABELS
TPP_WGROUP(TPP_WG_ENDIF_LABELS, 1("endif-labels"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_ENDIF_LABELS */

#if TPP_HAVE_TPP_W_ENDIF_LABELS
#define TPP_W_ENDIF_LABELS TPP_W_ENDIF_LABELS
TPP_WARNING(TPP_W_ENDIF_LABELS, 1(TPP_WG_ENDIF_LABELS), 0(), ~,
            "endif-label %Pt used")
#endif /* TPP_HAVE_TPP_W_ENDIF_LABELS */


/************************************************************************/
/* -Wdeprecated                                                         */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_DEPRECATED
#define TPP_HAVE_TPP_WG_DEPRECATED (TPP_HAVE_TPP_W_DEPRECATED_KEYWORD)
#endif /* !TPP_HAVE_TPP_WG_DEPRECATED */
#if TPP_HAVE_TPP_WG_DEPRECATED
#define TPP_WG_DEPRECATED TPP_WG_DEPRECATED
TPP_WGROUP(TPP_WG_DEPRECATED, 1("deprecated"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_DEPRECATED */

#if TPP_HAVE_TPP_W_DEPRECATED_KEYWORD
#define TPP_W_DEPRECATED_KEYWORD TPP_W_DEPRECATED_KEYWORD
TPP_WARNING(TPP_W_DEPRECATED_KEYWORD, 1(TPP_WG_DEPRECATED), 1(4995), TPP_WSTATE_UNDEFINED,
            "deprecated keyword %Pt used")
#endif /* TPP_HAVE_TPP_W_DEPRECATED_KEYWORD */


/************************************************************************/
/* -Wstring-escape                                                      */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_STRING_ESCAPE
#define TPP_HAVE_TPP_WG_STRING_ESCAPE (TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE)
#endif /* !TPP_HAVE_TPP_WG_STRING_ESCAPE */
#if TPP_HAVE_TPP_WG_STRING_ESCAPE
#define TPP_WG_STRING_ESCAPE TPP_WG_STRING_ESCAPE
TPP_WGROUP(TPP_WG_STRING_ESCAPE, 1("string-escape"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_STRING_ESCAPE */

#if TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE
#define TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE
TPP_WARNING(TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE, 1(TPP_WG_STRING_ESCAPE), 1(4129), TPP_WSTATE_UNDEFINED,
            "unknown escape sequence %[\\%c%]")
#endif /* TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE */


/************************************************************************/
/* -Wmultichar                                                          */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_MULTICHAR_LITERAL
#define TPP_HAVE_TPP_WG_MULTICHAR_LITERAL (TPP_HAVE_TPP_W_MULTICHAR_LITERAL)
#endif /* !TPP_HAVE_TPP_WG_MULTICHAR_LITERAL */
#if TPP_HAVE_TPP_WG_MULTICHAR_LITERAL
#define TPP_WG_MULTICHAR_LITERAL TPP_WG_MULTICHAR_LITERAL
TPP_WGROUP(TPP_WG_MULTICHAR_LITERAL, 1("multichar"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_MULTICHAR_LITERAL */

#if TPP_HAVE_TPP_W_MULTICHAR_LITERAL
#define TPP_W_MULTICHAR_LITERAL TPP_W_MULTICHAR_LITERAL
TPP_WARNING(TPP_W_MULTICHAR_LITERAL, 1(TPP_WG_MULTICHAR_LITERAL), 0(), TPP_WSTATE_UNDEFINED,
            "multichar literal %Pt used")
#endif /* TPP_HAVE_TPP_W_MULTICHAR_LITERAL */


/************************************************************************/
/* -Wmultiline-string                                                   */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_MULTILINE_STRING
#define TPP_HAVE_TPP_WG_MULTILINE_STRING (TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED)
#endif /* !TPP_HAVE_TPP_WG_MULTILINE_STRING */
#if TPP_HAVE_TPP_WG_MULTILINE_STRING
#define TPP_WG_MULTILINE_STRING TPP_WG_MULTILINE_STRING
TPP_WGROUP(TPP_WG_MULTILINE_STRING, 1("multiline-string"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_MULTILINE_STRING */

#if TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED
#define TPP_W_STRING_CONTINUED_AFTER_LINEFEED TPP_W_STRING_CONTINUED_AFTER_LINEFEED
TPP_WARNING(TPP_W_STRING_CONTINUED_AFTER_LINEFEED, 1(TPP_WG_MULTILINE_STRING), 0(), ~,
            "string continues after linefeed")
#endif /* TPP_HAVE_TPP_W_STRING_CONTINUED_AFTER_LINEFEED */



/************************************************************************/
/* -Wdate-time                                                          */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_DATE_TIME
#define TPP_HAVE_TPP_WG_DATE_TIME (TPP_HAVE_TPP_W_DATE_TIME)
#endif /* !TPP_HAVE_TPP_WG_DATE_TIME */
#if TPP_HAVE_TPP_WG_DATE_TIME
#define TPP_WG_DATE_TIME TPP_WG_DATE_TIME
TPP_WGROUP(TPP_WG_DATE_TIME, 1("date-time"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_DATE_TIME */

#if TPP_HAVE_TPP_W_DATE_TIME
#define TPP_W_DATE_TIME TPP_W_DATE_TIME
TPP_WARNING(TPP_W_DATE_TIME, 1(TPP_WG_DATE_TIME), 0(), TPP_WSTATE_UNDEFINED,
            "date-time macro used")
#endif /* TPP_HAVE_TPP_W_DATE_TIME */


/************************************************************************/
/* -Wmacros                                                             */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_MACROS
#define TPP_HAVE_TPP_WG_MACROS              \
	(TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST || \
	 TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS)
#endif /* !TPP_HAVE_TPP_WG_MACROS */
#if TPP_HAVE_TPP_WG_MACROS
#define TPP_WG_MACROS TPP_WG_MACROS
TPP_WGROUP(TPP_WG_MACROS, 1("macros"), TPP_WSTATE_ERROR_OR_FATAL)
#endif /* TPP_HAVE_TPP_WG_MACROS */

#if TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST
#define TPP_W_EOF_IN_ARGUMENT_LIST TPP_W_EOF_IN_ARGUMENT_LIST
TPP_WARNING(TPP_W_EOF_IN_ARGUMENT_LIST, 1(TPP_WG_MACROS), 1(1057), TPP_WSTATE_UNDEFINED,
            "eof in argument list for %[%s%]")
#endif /* TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST */

#if TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS
#define TPP_W_TOO_MANY_ARGUMENTS TPP_W_TOO_MANY_ARGUMENTS
TPP_WARNING(TPP_W_TOO_MANY_ARGUMENTS, 1(TPP_WG_MACROS), 1(4002), TPP_WSTATE_UNDEFINED,
            "too many arguments for %[%s%] which takes %u when %u were given")
#endif /* TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS */

#if TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS
#define TPP_W_TOO_FEW_ARGUMENTS TPP_W_TOO_FEW_ARGUMENTS
TPP_WARNING(TPP_W_TOO_FEW_ARGUMENTS, 1(TPP_WG_MACROS), 1(4003), TPP_WSTATE_UNDEFINED,
            "too few arguments for %[%s%] which takes %u when %u were given")
#endif /* TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */


/************************************************************************/
/* -Wmacros                                                             */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_EXPANSION_TO_DEFINED
#define TPP_HAVE_TPP_WG_EXPANSION_TO_DEFINED \
	(TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED)
#endif /* !TPP_HAVE_TPP_WG_EXPANSION_TO_DEFINED */
#if TPP_HAVE_TPP_WG_EXPANSION_TO_DEFINED
#define TPP_WG_EXPANSION_TO_DEFINED TPP_WG_EXPANSION_TO_DEFINED
TPP_WGROUP(TPP_WG_EXPANSION_TO_DEFINED, 1("expansion-to-defined"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_EXPANSION_TO_DEFINED */

#if TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED
#define TPP_W_EXPANSION_TO_DEFINED TPP_W_EXPANSION_TO_DEFINED
#if TPP_CONF_IS_EXT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR)
TPP_WARNING(TPP_W_EXPANSION_TO_DEFINED, 1(TPP_WG_EXPANSION_TO_DEFINED), 0(), TPP_WSTATE_UNDEFINED,
            "using %[defined%] on macro argument %Pt will not work, because macros are expanded "
            /**/ "before %[defined%] will seem them. If you want to prevent expansion in this "
            /**/ "specific case, enable %[#pragma extension(\"-f" TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR "\")%]")
#else /* TPP_CONF_IS_EXT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR) */
TPP_WARNING(TPP_W_EXPANSION_TO_DEFINED, 1(TPP_WG_EXPANSION_TO_DEFINED), 0(), TPP_WSTATE_UNDEFINED,
            "using %[defined%] on macro argument %Pt will not work, because macros are expanded "
            /**/ "before %[defined%] will seem them.")
#endif /* !TPP_CONF_IS_EXT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR) */
#endif /* TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED */


/************************************************************************/
/* -Wextension                                                          */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_EXTENSION
#define TPP_HAVE_TPP_WG_EXTENSION        \
	(TPP_HAVE_TPP_W_UNKNOWN_EXTENSION || \
	 TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS)
#endif /* !TPP_HAVE_TPP_WG_EXTENSION */
#if TPP_HAVE_TPP_WG_EXTENSION
#define TPP_WG_EXTENSION TPP_WG_EXTENSION
TPP_WGROUP(TPP_WG_EXTENSION, 1("extension"), TPP_WSTATE_WARN)
#endif /* TPP_HAVE_TPP_WG_EXTENSION */

#if TPP_HAVE_TPP_W_UNKNOWN_EXTENSION
#define TPP_W_UNKNOWN_EXTENSION TPP_W_UNKNOWN_EXTENSION
#if TPP_HAVE_TPP_EXTENSION_NEAREST
TPP_WARNING(TPP_W_UNKNOWN_EXTENSION, 1(TPP_WG_EXTENSION), 0(), TPP_WSTATE_UNDEFINED,
            "unknown extension %[-f%.*s%], did you mean %[-f%s%]")
#else /* TPP_HAVE_TPP_EXTENSION_NEAREST */
TPP_WARNING(TPP_W_UNKNOWN_EXTENSION, 1(TPP_WG_EXTENSION), 0(), TPP_WSTATE_UNDEFINED,
            "unknown extension %[-f%.*s%]")
#endif /* !TPP_HAVE_TPP_EXTENSION_NEAREST */
#endif /* TPP_HAVE_TPP_W_UNKNOWN_EXTENSION */

#if TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS
#define TPP_W_CANNOT_POP_EXTENSIONS TPP_W_CANNOT_POP_EXTENSIONS
TPP_WARNING(TPP_W_CANNOT_POP_EXTENSIONS, 1(TPP_WG_EXTENSION), 0(), TPP_WSTATE_UNDEFINED,
            "cannot %[#pragma extension(pop)%] no preceding %[push%]")
#endif /* TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS */


/************************************************************************/
/* -Wenviron                                                            */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_ENVIRON
#define TPP_HAVE_TPP_WG_ENVIRON \
	(TPP_HAVE_TPP_W_NO_SUCH_FILE)
#endif /* !TPP_HAVE_TPP_WG_ENVIRON */
#if TPP_HAVE_TPP_WG_ENVIRON
#define TPP_WG_ENVIRON TPP_WG_ENVIRON
TPP_WGROUP(TPP_WG_ENVIRON, 1("environ"), TPP_WSTATE_ERROR_OR_FATAL) /* XXX: Some other warning name? */
#endif /* TPP_HAVE_TPP_WG_ENVIRON */

#if TPP_HAVE_TPP_W_NO_SUCH_FILE
#define TPP_W_NO_SUCH_FILE TPP_W_NO_SUCH_FILE
TPP_WARNING(TPP_W_NO_SUCH_FILE, 1(TPP_WG_ENVIRON), 1(1083), TPP_WSTATE_UNDEFINED,
            "no such file: %Pt")
#endif /* TPP_HAVE_TPP_W_NO_SUCH_FILE */


/************************************************************************/
/* Misc warnings...                                                     */
/************************************************************************/
#if TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK
#define TPP_W_POP_MACRO_EMPTY_STACK TPP_W_POP_MACRO_EMPTY_STACK
TPP_WARNING(TPP_W_POP_MACRO_EMPTY_STACK, 0(), 1(4602), TPP_WSTATE_WARN,
            "cannot %[#pragma pop_macro(\"%.*s\")%]: no preceding %[#pragma push_macro%]")
#endif /* TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */

#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
#define TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
TPP_WARNING(TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, 0(), 1(4067), TPP_WSTATE_WARN,
            "extra token %Pt after %[#%s%] directive")
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */

#if TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
#define TPP_W_CANNOT_UNDEF_BUILTIN_MACRO TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
TPP_WARNING(TPP_W_CANNOT_UNDEF_BUILTIN_MACRO, 0(), 1(4117), TPP_WSTATE_WARN,
            "cannot %[#undef%] builtin keyword %[%s%]")
#endif /* TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO */

#if TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO
#define TPP_W_DEFINE_BUILTIN_MACRO TPP_W_DEFINE_BUILTIN_MACRO
TPP_WARNING(TPP_W_DEFINE_BUILTIN_MACRO, 0(), 1(4118), TPP_WSTATE_WARN,
            "builtin macro %[%s%] redefined")
#endif /* TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO */

#if TPP_HAVE_TPP_W_REDEFINE_MACRO
#define TPP_W_REDEFINE_MACRO TPP_W_REDEFINE_MACRO
TPP_WARNING_EX(TPP_W_REDEFINE_MACRO, 0(), 1(4005), TPP_WSTATE_WARN, {
	tpp_keyword const *keyword = tpp_current_va_arg(tpp_keyword const *);
	tpp_macro const *const old_definition = tpp_keyword_getmacro(keyword);
	tpp_warnf1("macro %[%s%] redefined\n", tpp_keyword_getkwdcstr(keyword));
	if (tpp_macro_getdeffilename(old_definition)) {
		/* TODO: must use "TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT" */
		tpp_warnf3("%s(%d, %d): note: see previous definition\n",
		           tpp_macro_getdeffilename(old_definition),
		           (int)tpp_lcinfo_getline(tpp_macro_getdeflcinfo(old_definition)) + 1,
		           (int)tpp_lcinfo_getcol(tpp_macro_getdeflcinfo(old_definition)) + 1);
	}
})
#endif /* TPP_HAVE_TPP_W_REDEFINE_MACRO */

#if TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS
#define TPP_W_BAD_EXPRESSION_OPERANDS TPP_W_BAD_EXPRESSION_OPERANDS
TPP_WARNING(TPP_W_BAD_EXPRESSION_OPERANDS, 0(), 0(), TPP_WSTATE_ERROR_OR_FATAL,
            "bad operand types in %[%s%s%s%s%s%s%]")
#endif /* TPP_HAVE_TPP_W_BAD_EXPRESSION_OPERANDS */

#if TPP_HAVE_TPP_W_DIVIDE_BY_ZERO
#define TPP_W_DIVIDE_BY_ZERO TPP_W_DIVIDE_BY_ZERO
TPP_WARNING(TPP_W_DIVIDE_BY_ZERO, 0(), 0(), TPP_WSTATE_ERROR_OR_FATAL,
            "division by zero")
#endif /* TPP_HAVE_TPP_W_DIVIDE_BY_ZERO */



//TODO:TPP_WGROUP(TPP_WG_USAGE, /*          */ 1("usage"),                TPP_WSTATE_FATAL)
//TODO:TPP_WGROUP(TPP_WG_BOOLVALUE, /*      */ 1("boolean-value"),        TPP_WSTATE_FATAL)
//TODO:TPP_WGROUP(TPP_WG_LIMIT, /*          */ 1("limit"),                TPP_WSTATE_FATAL)
//TODO:TPP_WGROUP(TPP_WG_QUALITY, /*        */ 1("quality"),              TPP_WSTATE_FATAL)
//TODO:TPP_WGROUP(TPP_WG_DEPENDENCY, /*     */ 1("dependency"),           TPP_WSTATE_WARN)


/* Pull in user definitions (if defined) */
#ifdef TPP_CONFIG_USERDEFS_FILENAME
#include TPP_CONFIG_USERDEFS_FILENAME
#endif /* TPP_CONFIG_USERDEFS_FILENAME */


#undef TPP_KWD
#undef TPP_KWD_FLAGS
#undef TPP_EXTENSION
#undef TPP_WGROUP
#undef TPP_WARNING
#undef TPP_WARNING_EX
#undef TPP_WARNING_WITH_NUMBER
#undef TPP_WARNING_WITH_NUMBER_EX
#undef TPP_MACRO
#undef TPP_BUILTIN_MACRO
/*[[[tpp-end]]]*/
