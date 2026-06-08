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

/* Definitions for builtin keywords */
#ifndef TPP_KWD
#define TPP_KWD(id, string)
#endif /* !TPP_KWD */
#ifndef TPP_KWD_FLAGS
#define TPP_KWD_FLAGS(id, flags)
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
#define TPP_MACRO(keyword_id, if)
#endif /* !TPP_MACRO */
#ifndef TPP_BUILTIN_MACRO
/* Defines the value that a builtin macro without custom behavior
 * should expand to. Without this, the expansion is either controlled
 * via custom behavior (in the case of macros like __FILE__, __LINE__),
 * or as a fallback: the macro will simply expand to itself. */
#define TPP_BUILTIN_MACRO(name, value)
#endif /* !TPP_BUILTIN_MACRO */


/*[[[deemon
import * from deemon;
local BUILTIN_KEYWORDS: {string: string} = {
	"line": "TPP_HAVE_CPP_LINE",
	"include": "TPP_HAVE_CPP_INCLUDE",
	"include_next": "TPP_HAVE_CPP_INCLUDE_NEXT",
	"import": "TPP_HAVE_CPP_IMPORT",
	"if": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"ifdef": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"ifndef": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"elif": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"elifdef": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"elifndef": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"else": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"endif": "TPP_HAVE_CPP_IF_ELSE_ENDIF",
	"define": "TPP_HAVE_CPP_DEFINE",
	"undef": "TPP_HAVE_CPP_DEFINE",
	"embed": "TPP_HAVE_CPP_EMBED",
	"assert": "TPP_HAVE_CPP_ASSERT",
	"unassert": "TPP_HAVE_CPP_ASSERT",
	"error": "TPP_HAVE_CPP_ERROR",
	"warning": "TPP_HAVE_CPP_WARNING",
	"ident": "TPP_HAVE_CPP_IDENT_SCCS",
	"sccs": "TPP_HAVE_CPP_IDENT_SCCS",
	"pragma": "TPP_HAVE_CPP_PRAGMA",
	"_Pragma": "TPP_HAVE_MACRO__Pragma",
	"__pragma": "TPP_HAVE_MACRO___pragma",
	"push_macro": "TPP_HAVE_PRAGMA_PUSH_MACRO",
	"pop_macro": "TPP_HAVE_PRAGMA_PUSH_MACRO",
	"once": "TPP_HAVE_PRAGMA_ONCE",
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
	"defined": "TPP_HAVE_EXPR_DEFINED",
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
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_if TPP_KWD_if
TPP_KWD(TPP_KWD_if, "if")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_ifdef TPP_KWD_ifdef
TPP_KWD(TPP_KWD_ifdef, "ifdef")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_ifndef TPP_KWD_ifndef
TPP_KWD(TPP_KWD_ifndef, "ifndef")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_elif TPP_KWD_elif
TPP_KWD(TPP_KWD_elif, "elif")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_elifdef TPP_KWD_elifdef
TPP_KWD(TPP_KWD_elifdef, "elifdef")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_elifndef TPP_KWD_elifndef
TPP_KWD(TPP_KWD_elifndef, "elifndef")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_else TPP_KWD_else
TPP_KWD(TPP_KWD_else, "else")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KWD_endif TPP_KWD_endif
TPP_KWD(TPP_KWD_endif, "endif")
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#if TPP_HAVE_CPP_DEFINE
#define TPP_KWD_define TPP_KWD_define
TPP_KWD(TPP_KWD_define, "define")
#endif /* TPP_HAVE_CPP_DEFINE */
#if TPP_HAVE_CPP_DEFINE
#define TPP_KWD_undef TPP_KWD_undef
TPP_KWD(TPP_KWD_undef, "undef")
#endif /* TPP_HAVE_CPP_DEFINE */
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
#if TPP_HAVE_CPP_ERROR
#define TPP_KWD_error TPP_KWD_error
TPP_KWD(TPP_KWD_error, "error")
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
#define TPP_KWD_warning TPP_KWD_warning
TPP_KWD(TPP_KWD_warning, "warning")
#endif /* TPP_HAVE_CPP_WARNING */
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
#if TPP_HAVE_EXPR_DEFINED
#define TPP_KWD_defined TPP_KWD_defined
TPP_KWD(TPP_KWD_defined, "defined")
#endif /* TPP_HAVE_EXPR_DEFINED */
/*[[[end]]]*/


/* Pre-defined macros... */
#if TPP_HAVE_CPP_MACROS
TPP_MACRO(TPP_KWD___TPP_VERSION__, true)
TPP_BUILTIN_MACRO(TPP_KWD___TPP_VERSION__, TPP_PREPROCESSOR_VERSION_STR)
#if TPP_HAVE_MACRO__Pragma /* GCC defines _Pragma as a macro... So I'll just do that as well! */
TPP_MACRO(TPP_KWD__Pragma, tpp_lexer_getext(self, TPP_EXT_MACRO__Pragma))
#endif /* TPP_HAVE_MACRO__Pragma */
#if TPP_HAVE_MACRO___pragma /* I promised this would be defined as a macro... */
TPP_MACRO(TPP_KWD___pragma, tpp_lexer_getext(self, TPP_EXT_MACRO___pragma))
#endif /* TPP_HAVE_MACRO___pragma */

/* Declare magic, built-in macros */
#if TPP_HAVE_CLANG_MACRO___has_attribute
TPP_MACRO(TPP_KWD___has_attribute, tpp_lexer_getext(self, TPP_EXT_CLANG_MACRO___has_attribute))
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
TPP_MACRO(TPP_KWD___has_builtin, tpp_lexer_getext(self, TPP_EXT_CLANG_MACRO___has_builtin))
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
TPP_MACRO(TPP_KWD___has_cpp_attribute, tpp_lexer_getext(self, TPP_EXT_CLANG_MACRO___has_cpp_attribute))
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
TPP_MACRO(TPP_KWD___has_declspec_attribute, tpp_lexer_getext(self, TPP_EXT_CLANG_MACRO___has_declspec_attribute))
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension
TPP_MACRO(TPP_KWD___has_extension, tpp_lexer_getext(self, TPP_EXT_CLANG_MACRO___has_extension) ||
                                   tpp_lexer_getext(self, TPP_EXT_MACRO___has_extension))
#endif /* TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
TPP_MACRO(TPP_KWD___has_feature, tpp_lexer_getext(self, TPP_EXT_CLANG_MACRO___has_feature))
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
TPP_MACRO(TPP_KWD___has_c_attribute, tpp_lexer_getext(self, TPP_EXT_CLANG_MACRO___has_c_attribute))
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_MACRO___is_identifier
TPP_MACRO(TPP_KWD___is_identifier, tpp_lexer_getext(self, TPP_EXT_MACRO___is_identifier))
#endif /* TPP_HAVE_MACRO___is_identifier */
#if TPP_HAVE_MACRO___is_deprecated
TPP_MACRO(TPP_KWD___is_deprecated, tpp_lexer_getext(self, TPP_EXT_MACRO___is_deprecated))
#endif /* TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_MACRO___is_poisoned
TPP_MACRO(TPP_KWD___is_poisoned, tpp_lexer_getext(self, TPP_EXT_MACRO___is_poisoned))
#endif /* TPP_HAVE_MACRO___is_poisoned */
#if TPP_HAVE_MACRO___has_known_extension
TPP_MACRO(TPP_KWD___has_known_extension, tpp_lexer_getext(self, TPP_EXT_MACRO___has_known_extension))
#endif /* TPP_HAVE_MACRO___has_known_extension */
#if TPP_HAVE_MACRO___has_warning
TPP_MACRO(TPP_KWD___has_warning, tpp_lexer_getext(self, TPP_EXT_MACRO___has_warning))
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
TPP_MACRO(TPP_KWD___has_known_warning, tpp_lexer_getext(self, TPP_EXT_MACRO___has_known_warning))
#endif /* TPP_HAVE_MACRO___has_known_warning */
#if TPP_HAVE_MACRO___has_include
TPP_MACRO(TPP_KWD___has_include, tpp_lexer_getext(self, TPP_EXT_MACRO___has_include))
#endif /* TPP_HAVE_MACRO___has_include */
#if TPP_HAVE_MACRO___has_include_next
TPP_MACRO(TPP_KWD___has_include_next, tpp_lexer_getext(self, TPP_EXT_MACRO___has_include_next))
#endif /* TPP_HAVE_MACRO___has_include_next */
#if TPP_HAVE_MACRO___has_embed
TPP_MACRO(TPP_KWD___has_embed, tpp_lexer_getext(self, TPP_EXT_MACRO___has_embed))
#endif /* TPP_HAVE_MACRO___has_embed */
#if TPP_HAVE_MACRO___FILE__
TPP_MACRO(TPP_KWD___FILE__, tpp_lexer_getext(self, TPP_EXT_MACRO___FILE__))
#endif /* TPP_HAVE_MACRO___FILE__ */
#if TPP_HAVE_MACRO___LINE__
TPP_MACRO(TPP_KWD___LINE__, tpp_lexer_getext(self, TPP_EXT_MACRO___LINE__))
#endif /* TPP_HAVE_MACRO___LINE__ */
#if TPP_HAVE_MACRO___TIME__
TPP_MACRO(TPP_KWD___TIME__, tpp_lexer_getext(self, TPP_EXT_MACRO___TIME__))
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
TPP_MACRO(TPP_KWD___DATE__, tpp_lexer_getext(self, TPP_EXT_MACRO___DATE__))
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___COLUMN__
TPP_MACRO(TPP_KWD___COLUMN__, tpp_lexer_getext(self, TPP_EXT_MACRO___COLUMN__))
#endif /* TPP_HAVE_MACRO___COLUMN__ */
#if TPP_HAVE_MACRO___BASE_FILE__
TPP_MACRO(TPP_KWD___BASE_FILE__, tpp_lexer_getext(self, TPP_EXT_MACRO___BASE_FILE__))
#endif /* TPP_HAVE_MACRO___BASE_FILE__ */
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__
TPP_MACRO(TPP_KWD___INCLUDE_LEVEL__, tpp_lexer_getext(self, TPP_EXT_MACRO___INCLUDE_LEVEL__))
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ */
#if TPP_HAVE_MACRO___INCLUDE_DEPTH__
TPP_MACRO(TPP_KWD___INCLUDE_DEPTH__, tpp_lexer_getext(self, TPP_EXT_MACRO___INCLUDE_DEPTH__))
#endif /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
#if TPP_HAVE_MACRO___COUNTER__
TPP_MACRO(TPP_KWD___COUNTER__, tpp_lexer_getext(self, TPP_EXT_MACRO___COUNTER__))
#endif /* TPP_HAVE_MACRO___COUNTER__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
TPP_MACRO(TPP_KWD___TIMESTAMP__, tpp_lexer_getext(self, TPP_EXT_MACRO___TIMESTAMP__))
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
#if TPP_HAVE_NUMERIC_DATE_MACROS
TPP_MACRO(TPP_KWD___DATE_DAY__, tpp_lexer_getext(self, TPP_EXT_NUMERIC_DATE_MACROS))
TPP_MACRO(TPP_KWD___DATE_WDAY__, tpp_lexer_getext(self, TPP_EXT_NUMERIC_DATE_MACROS))
TPP_MACRO(TPP_KWD___DATE_YDAY__, tpp_lexer_getext(self, TPP_EXT_NUMERIC_DATE_MACROS))
TPP_MACRO(TPP_KWD___DATE_MONTH__, tpp_lexer_getext(self, TPP_EXT_NUMERIC_DATE_MACROS))
TPP_MACRO(TPP_KWD___DATE_YEAR__, tpp_lexer_getext(self, TPP_EXT_NUMERIC_DATE_MACROS))
#endif /* TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
TPP_MACRO(TPP_KWD___TIME_SEC__, tpp_lexer_getext(self, TPP_EXT_NUMERIC_TIME_MACROS))
TPP_MACRO(TPP_KWD___TIME_MIN__, tpp_lexer_getext(self, TPP_EXT_NUMERIC_TIME_MACROS))
TPP_MACRO(TPP_KWD___TIME_HOUR__, tpp_lexer_getext(self, TPP_EXT_NUMERIC_TIME_MACROS))
#endif /* TPP_HAVE_NUMERIC_TIME_MACROS */
#if TPP_HAVE_MACRO___TPP_EVAL
TPP_MACRO(TPP_KWD___TPP_EVAL, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_EVAL))
#endif /* TPP_HAVE_MACRO___TPP_EVAL */
#if TPP_HAVE_MACRO___TPP_UNIQUE
TPP_MACRO(TPP_KWD___TPP_UNIQUE, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_UNIQUE))
#endif /* TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_LOAD_FILE
TPP_MACRO(TPP_KWD___TPP_LOAD_FILE, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_LOAD_FILE))
#endif /* TPP_HAVE_MACRO___TPP_LOAD_FILE */
#if TPP_HAVE_MACRO___TPP_COUNTER
TPP_MACRO(TPP_KWD___TPP_COUNTER, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_COUNTER))
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
#if TPP_HAVE_MACRO___TPP_RANDOM
TPP_MACRO(TPP_KWD___TPP_RANDOM, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_RANDOM))
#endif /* TPP_HAVE_MACRO___TPP_RANDOM */
#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE
TPP_MACRO(TPP_KWD___TPP_STR_DECOMPILE, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_STR_DECOMPILE))
#endif /* TPP_HAVE_MACRO___TPP_STR_DECOMPILE */
#if TPP_HAVE_MACRO___TPP_STR_SUBSTR
TPP_MACRO(TPP_KWD___TPP_STR_SUBSTR, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_STR_SUBSTR))
#endif /* TPP_HAVE_MACRO___TPP_STR_SUBSTR */
#if TPP_HAVE_MACRO___TPP_STR_PACK
TPP_MACRO(TPP_KWD___TPP_STR_PACK, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_STR_PACK))
#endif /* TPP_HAVE_MACRO___TPP_STR_PACK */
#if TPP_HAVE_MACRO___TPP_STR_SIZE
TPP_MACRO(TPP_KWD___TPP_STR_SIZE, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_STR_SIZE))
#endif /* TPP_HAVE_MACRO___TPP_STR_SIZE */
#if TPP_HAVE_MACRO___TPP_COUNT_TOKENS
TPP_MACRO(TPP_KWD___TPP_COUNT_TOKENS, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_COUNT_TOKENS))
#endif /* TPP_HAVE_MACRO___TPP_COUNT_TOKENS */
#if TPP_HAVE_MACRO___TPP_IDENTIFIER
TPP_MACRO(TPP_KWD___TPP_IDENTIFIER, tpp_lexer_getext(self, TPP_EXT_MACRO___TPP_IDENTIFIER))
#endif /* TPP_HAVE_MACRO___TPP_IDENTIFIER */
#endif /* TPP_HAVE_CPP_MACROS */




/* TODO: Feature-test macros (for __has_extension()) */
#if 0
HAS_EXTENSION_IF(tpp_dollar_is_alpha,              HAVE_EXTENSION_DOLLAR_IS_ALPHA)
HAS_EXTENSION_IF(tpp_va_args,                      HAVE_EXTENSION_VA_ARGS)
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
	local TPP_EXT_FOO = "TPP_EXT_" + TPP_HAVE_FOO.lsstrip("TPP_HAVE_");
	local TPP_EXTNAME_FOO = "TPP_EXTNAME_" + TPP_HAVE_FOO.lsstrip("TPP_HAVE_");
	print("#if ", TPP_HAVE_FOO, " < 0");
	print("#define ", TPP_EXT_FOO, " ", TPP_EXT_FOO);
	print("#ifndef ", TPP_EXTNAME_FOO);
	print("#define ", TPP_EXTNAME_FOO, " ", repr(extName));
	print("#endif /" "* !", TPP_EXTNAME_FOO, " *" "/");
	print("TPP_EXTENSION(", TPP_EXT_FOO, ", ", TPP_EXTNAME_FOO, ", ", TPP_HAVE_FOO, " == -1)");
	print("#define _tpp_extensions_state_get_", TPP_EXT_FOO, "(self) (self)->tes_flags.tef_", TPP_EXT_FOO);
	print("#else /" "* ", TPP_HAVE_FOO, " < 0 *" "/");
	print("#define _tpp_extensions_state_get_", TPP_EXT_FOO, "(self) ", TPP_HAVE_FOO);
	print("#endif /" "* ", TPP_HAVE_FOO, " >= 0 *" "/");
}
]]]*/
#if TPP_HAVE_TRIGRAPHS < 0
#define TPP_EXT_TRIGRAPHS TPP_EXT_TRIGRAPHS
#ifndef TPP_EXTNAME_TRIGRAPHS
#define TPP_EXTNAME_TRIGRAPHS "trigraphs"
#endif /* !TPP_EXTNAME_TRIGRAPHS */
TPP_EXTENSION(TPP_EXT_TRIGRAPHS, TPP_EXTNAME_TRIGRAPHS, TPP_HAVE_TRIGRAPHS == -1)
#define _tpp_extensions_state_get_TPP_EXT_TRIGRAPHS(self) (self)->tes_flags.tef_TPP_EXT_TRIGRAPHS
#else /* TPP_HAVE_TRIGRAPHS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_TRIGRAPHS(self) TPP_HAVE_TRIGRAPHS
#endif /* TPP_HAVE_TRIGRAPHS >= 0 */
#if TPP_HAVE_DIGRAPHS < 0
#define TPP_EXT_DIGRAPHS TPP_EXT_DIGRAPHS
#ifndef TPP_EXTNAME_DIGRAPHS
#define TPP_EXTNAME_DIGRAPHS "digraphs"
#endif /* !TPP_EXTNAME_DIGRAPHS */
TPP_EXTENSION(TPP_EXT_DIGRAPHS, TPP_EXTNAME_DIGRAPHS, TPP_HAVE_DIGRAPHS == -1)
#define _tpp_extensions_state_get_TPP_EXT_DIGRAPHS(self) (self)->tes_flags.tef_TPP_EXT_DIGRAPHS
#else /* TPP_HAVE_DIGRAPHS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_DIGRAPHS(self) TPP_HAVE_DIGRAPHS
#endif /* TPP_HAVE_DIGRAPHS >= 0 */
#if TPP_HAVE_BSE < 0
#define TPP_EXT_BSE TPP_EXT_BSE
#ifndef TPP_EXTNAME_BSE
#define TPP_EXTNAME_BSE "bse"
#endif /* !TPP_EXTNAME_BSE */
TPP_EXTENSION(TPP_EXT_BSE, TPP_EXTNAME_BSE, TPP_HAVE_BSE == -1)
#define _tpp_extensions_state_get_TPP_EXT_BSE(self) (self)->tes_flags.tef_TPP_EXT_BSE
#else /* TPP_HAVE_BSE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_BSE(self) TPP_HAVE_BSE
#endif /* TPP_HAVE_BSE >= 0 */
#if TPP_HAVE_BSE_WHITESPACE < 0
#define TPP_EXT_BSE_WHITESPACE TPP_EXT_BSE_WHITESPACE
#ifndef TPP_EXTNAME_BSE_WHITESPACE
#define TPP_EXTNAME_BSE_WHITESPACE "bse-whitespace"
#endif /* !TPP_EXTNAME_BSE_WHITESPACE */
TPP_EXTENSION(TPP_EXT_BSE_WHITESPACE, TPP_EXTNAME_BSE_WHITESPACE, TPP_HAVE_BSE_WHITESPACE == -1)
#define _tpp_extensions_state_get_TPP_EXT_BSE_WHITESPACE(self) (self)->tes_flags.tef_TPP_EXT_BSE_WHITESPACE
#else /* TPP_HAVE_BSE_WHITESPACE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_BSE_WHITESPACE(self) TPP_HAVE_BSE_WHITESPACE
#endif /* TPP_HAVE_BSE_WHITESPACE >= 0 */
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS < 0
#define TPP_EXT_ESCAPE_IN_IDENTIFIERS TPP_EXT_ESCAPE_IN_IDENTIFIERS
#ifndef TPP_EXTNAME_ESCAPE_IN_IDENTIFIERS
#define TPP_EXTNAME_ESCAPE_IN_IDENTIFIERS "escape-in-identifiers"
#endif /* !TPP_EXTNAME_ESCAPE_IN_IDENTIFIERS */
TPP_EXTENSION(TPP_EXT_ESCAPE_IN_IDENTIFIERS, TPP_EXTNAME_ESCAPE_IN_IDENTIFIERS, TPP_HAVE_ESCAPE_IN_IDENTIFIERS == -1)
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_IN_IDENTIFIERS(self) (self)->tes_flags.tef_TPP_EXT_ESCAPE_IN_IDENTIFIERS
#else /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_IN_IDENTIFIERS(self) TPP_HAVE_ESCAPE_IN_IDENTIFIERS
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS >= 0 */
#if TPP_HAVE_ESCAPE_E_IN_STRINGS < 0
#define TPP_EXT_ESCAPE_E_IN_STRINGS TPP_EXT_ESCAPE_E_IN_STRINGS
#ifndef TPP_EXTNAME_ESCAPE_E_IN_STRINGS
#define TPP_EXTNAME_ESCAPE_E_IN_STRINGS "escape-e-in-strings"
#endif /* !TPP_EXTNAME_ESCAPE_E_IN_STRINGS */
TPP_EXTENSION(TPP_EXT_ESCAPE_E_IN_STRINGS, TPP_EXTNAME_ESCAPE_E_IN_STRINGS, TPP_HAVE_ESCAPE_E_IN_STRINGS == -1)
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_E_IN_STRINGS(self) (self)->tes_flags.tef_TPP_EXT_ESCAPE_E_IN_STRINGS
#else /* TPP_HAVE_ESCAPE_E_IN_STRINGS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_E_IN_STRINGS(self) TPP_HAVE_ESCAPE_E_IN_STRINGS
#endif /* TPP_HAVE_ESCAPE_E_IN_STRINGS >= 0 */
#if TPP_HAVE_ESCAPE_S_IN_STRINGS < 0
#define TPP_EXT_ESCAPE_S_IN_STRINGS TPP_EXT_ESCAPE_S_IN_STRINGS
#ifndef TPP_EXTNAME_ESCAPE_S_IN_STRINGS
#define TPP_EXTNAME_ESCAPE_S_IN_STRINGS "escape-s-in-strings"
#endif /* !TPP_EXTNAME_ESCAPE_S_IN_STRINGS */
TPP_EXTENSION(TPP_EXT_ESCAPE_S_IN_STRINGS, TPP_EXTNAME_ESCAPE_S_IN_STRINGS, TPP_HAVE_ESCAPE_S_IN_STRINGS == -1)
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_S_IN_STRINGS(self) (self)->tes_flags.tef_TPP_EXT_ESCAPE_S_IN_STRINGS
#else /* TPP_HAVE_ESCAPE_S_IN_STRINGS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_S_IN_STRINGS(self) TPP_HAVE_ESCAPE_S_IN_STRINGS
#endif /* TPP_HAVE_ESCAPE_S_IN_STRINGS >= 0 */
#if TPP_HAVE_CPP_EXCLAIM < 0
#define TPP_EXT_CPP_EXCLAIM TPP_EXT_CPP_EXCLAIM
#ifndef TPP_EXTNAME_CPP_EXCLAIM
#define TPP_EXTNAME_CPP_EXCLAIM "shebang-directives"
#endif /* !TPP_EXTNAME_CPP_EXCLAIM */
TPP_EXTENSION(TPP_EXT_CPP_EXCLAIM, TPP_EXTNAME_CPP_EXCLAIM, TPP_HAVE_CPP_EXCLAIM == -1)
#define _tpp_extensions_state_get_TPP_EXT_CPP_EXCLAIM(self) (self)->tes_flags.tef_TPP_EXT_CPP_EXCLAIM
#else /* TPP_HAVE_CPP_EXCLAIM < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CPP_EXCLAIM(self) TPP_HAVE_CPP_EXCLAIM
#endif /* TPP_HAVE_CPP_EXCLAIM >= 0 */
#if TPP_HAVE_CPP_INCLUDE < 0
#define TPP_EXT_CPP_INCLUDE TPP_EXT_CPP_INCLUDE
#ifndef TPP_EXTNAME_CPP_INCLUDE
#define TPP_EXTNAME_CPP_INCLUDE "include-directives"
#endif /* !TPP_EXTNAME_CPP_INCLUDE */
TPP_EXTENSION(TPP_EXT_CPP_INCLUDE, TPP_EXTNAME_CPP_INCLUDE, TPP_HAVE_CPP_INCLUDE == -1)
#define _tpp_extensions_state_get_TPP_EXT_CPP_INCLUDE(self) (self)->tes_flags.tef_TPP_EXT_CPP_INCLUDE
#else /* TPP_HAVE_CPP_INCLUDE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CPP_INCLUDE(self) TPP_HAVE_CPP_INCLUDE
#endif /* TPP_HAVE_CPP_INCLUDE >= 0 */
#if TPP_HAVE_CPP_INCLUDE_NEXT < 0
#define TPP_EXT_CPP_INCLUDE_NEXT TPP_EXT_CPP_INCLUDE_NEXT
#ifndef TPP_EXTNAME_CPP_INCLUDE_NEXT
#define TPP_EXTNAME_CPP_INCLUDE_NEXT "include-next-directives"
#endif /* !TPP_EXTNAME_CPP_INCLUDE_NEXT */
TPP_EXTENSION(TPP_EXT_CPP_INCLUDE_NEXT, TPP_EXTNAME_CPP_INCLUDE_NEXT, TPP_HAVE_CPP_INCLUDE_NEXT == -1)
#define _tpp_extensions_state_get_TPP_EXT_CPP_INCLUDE_NEXT(self) (self)->tes_flags.tef_TPP_EXT_CPP_INCLUDE_NEXT
#else /* TPP_HAVE_CPP_INCLUDE_NEXT < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CPP_INCLUDE_NEXT(self) TPP_HAVE_CPP_INCLUDE_NEXT
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT >= 0 */
#if TPP_HAVE_CPP_IMPORT < 0
#define TPP_EXT_CPP_IMPORT TPP_EXT_CPP_IMPORT
#ifndef TPP_EXTNAME_CPP_IMPORT
#define TPP_EXTNAME_CPP_IMPORT "import-directives"
#endif /* !TPP_EXTNAME_CPP_IMPORT */
TPP_EXTENSION(TPP_EXT_CPP_IMPORT, TPP_EXTNAME_CPP_IMPORT, TPP_HAVE_CPP_IMPORT == -1)
#define _tpp_extensions_state_get_TPP_EXT_CPP_IMPORT(self) (self)->tes_flags.tef_TPP_EXT_CPP_IMPORT
#else /* TPP_HAVE_CPP_IMPORT < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CPP_IMPORT(self) TPP_HAVE_CPP_IMPORT
#endif /* TPP_HAVE_CPP_IMPORT >= 0 */
#if TPP_HAVE_CPP_ASSERT < 0
#define TPP_EXT_CPP_ASSERT TPP_EXT_CPP_ASSERT
#ifndef TPP_EXTNAME_CPP_ASSERT
#define TPP_EXTNAME_CPP_ASSERT "assertions"
#endif /* !TPP_EXTNAME_CPP_ASSERT */
TPP_EXTENSION(TPP_EXT_CPP_ASSERT, TPP_EXTNAME_CPP_ASSERT, TPP_HAVE_CPP_ASSERT == -1)
#define _tpp_extensions_state_get_TPP_EXT_CPP_ASSERT(self) (self)->tes_flags.tef_TPP_EXT_CPP_ASSERT
#else /* TPP_HAVE_CPP_ASSERT < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CPP_ASSERT(self) TPP_HAVE_CPP_ASSERT
#endif /* TPP_HAVE_CPP_ASSERT >= 0 */
#if TPP_HAVE_CPP_ERROR < 0
#define TPP_EXT_CPP_ERROR TPP_EXT_CPP_ERROR
#ifndef TPP_EXTNAME_CPP_ERROR
#define TPP_EXTNAME_CPP_ERROR "error-directives"
#endif /* !TPP_EXTNAME_CPP_ERROR */
TPP_EXTENSION(TPP_EXT_CPP_ERROR, TPP_EXTNAME_CPP_ERROR, TPP_HAVE_CPP_ERROR == -1)
#define _tpp_extensions_state_get_TPP_EXT_CPP_ERROR(self) (self)->tes_flags.tef_TPP_EXT_CPP_ERROR
#else /* TPP_HAVE_CPP_ERROR < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CPP_ERROR(self) TPP_HAVE_CPP_ERROR
#endif /* TPP_HAVE_CPP_ERROR >= 0 */
#if TPP_HAVE_CPP_WARNING < 0
#define TPP_EXT_CPP_WARNING TPP_EXT_CPP_WARNING
#ifndef TPP_EXTNAME_CPP_WARNING
#define TPP_EXTNAME_CPP_WARNING "warning-directives"
#endif /* !TPP_EXTNAME_CPP_WARNING */
TPP_EXTENSION(TPP_EXT_CPP_WARNING, TPP_EXTNAME_CPP_WARNING, TPP_HAVE_CPP_WARNING == -1)
#define _tpp_extensions_state_get_TPP_EXT_CPP_WARNING(self) (self)->tes_flags.tef_TPP_EXT_CPP_WARNING
#else /* TPP_HAVE_CPP_WARNING < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CPP_WARNING(self) TPP_HAVE_CPP_WARNING
#endif /* TPP_HAVE_CPP_WARNING >= 0 */
#if TPP_HAVE_CPP_IDENT_SCCS < 0
#define TPP_EXT_CPP_IDENT_SCCS TPP_EXT_CPP_IDENT_SCCS
#ifndef TPP_EXTNAME_CPP_IDENT_SCCS
#define TPP_EXTNAME_CPP_IDENT_SCCS "ident-directives"
#endif /* !TPP_EXTNAME_CPP_IDENT_SCCS */
TPP_EXTENSION(TPP_EXT_CPP_IDENT_SCCS, TPP_EXTNAME_CPP_IDENT_SCCS, TPP_HAVE_CPP_IDENT_SCCS == -1)
#define _tpp_extensions_state_get_TPP_EXT_CPP_IDENT_SCCS(self) (self)->tes_flags.tef_TPP_EXT_CPP_IDENT_SCCS
#else /* TPP_HAVE_CPP_IDENT_SCCS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CPP_IDENT_SCCS(self) TPP_HAVE_CPP_IDENT_SCCS
#endif /* TPP_HAVE_CPP_IDENT_SCCS >= 0 */
#if TPP_HAVE_MACRO__Pragma < 0
#define TPP_EXT_MACRO__Pragma TPP_EXT_MACRO__Pragma
#ifndef TPP_EXTNAME_MACRO__Pragma
#define TPP_EXTNAME_MACRO__Pragma "_Pragma"
#endif /* !TPP_EXTNAME_MACRO__Pragma */
TPP_EXTENSION(TPP_EXT_MACRO__Pragma, TPP_EXTNAME_MACRO__Pragma, TPP_HAVE_MACRO__Pragma == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO__Pragma(self) (self)->tes_flags.tef_TPP_EXT_MACRO__Pragma
#else /* TPP_HAVE_MACRO__Pragma < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO__Pragma(self) TPP_HAVE_MACRO__Pragma
#endif /* TPP_HAVE_MACRO__Pragma >= 0 */
#if TPP_HAVE_MACRO___pragma < 0
#define TPP_EXT_MACRO___pragma TPP_EXT_MACRO___pragma
#ifndef TPP_EXTNAME_MACRO___pragma
#define TPP_EXTNAME_MACRO___pragma "__pragma"
#endif /* !TPP_EXTNAME_MACRO___pragma */
TPP_EXTENSION(TPP_EXT_MACRO___pragma, TPP_EXTNAME_MACRO___pragma, TPP_HAVE_MACRO___pragma == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___pragma(self) (self)->tes_flags.tef_TPP_EXT_MACRO___pragma
#else /* TPP_HAVE_MACRO___pragma < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___pragma(self) TPP_HAVE_MACRO___pragma
#endif /* TPP_HAVE_MACRO___pragma >= 0 */
#if TPP_HAVE_CLANG_MACRO___has_attribute < 0
#define TPP_EXT_CLANG_MACRO___has_attribute TPP_EXT_CLANG_MACRO___has_attribute
#ifndef TPP_EXTNAME_CLANG_MACRO___has_attribute
#define TPP_EXTNAME_CLANG_MACRO___has_attribute "clang-__has_attribute"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_attribute */
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_attribute, TPP_EXTNAME_CLANG_MACRO___has_attribute, TPP_HAVE_CLANG_MACRO___has_attribute == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_attribute(self) (self)->tes_flags.tef_TPP_EXT_CLANG_MACRO___has_attribute
#else /* TPP_HAVE_CLANG_MACRO___has_attribute < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_attribute(self) TPP_HAVE_CLANG_MACRO___has_attribute
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute >= 0 */
#if TPP_HAVE_CLANG_MACRO___has_builtin < 0
#define TPP_EXT_CLANG_MACRO___has_builtin TPP_EXT_CLANG_MACRO___has_builtin
#ifndef TPP_EXTNAME_CLANG_MACRO___has_builtin
#define TPP_EXTNAME_CLANG_MACRO___has_builtin "clang-__has_builtin"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_builtin */
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_builtin, TPP_EXTNAME_CLANG_MACRO___has_builtin, TPP_HAVE_CLANG_MACRO___has_builtin == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_builtin(self) (self)->tes_flags.tef_TPP_EXT_CLANG_MACRO___has_builtin
#else /* TPP_HAVE_CLANG_MACRO___has_builtin < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_builtin(self) TPP_HAVE_CLANG_MACRO___has_builtin
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin >= 0 */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute < 0
#define TPP_EXT_CLANG_MACRO___has_cpp_attribute TPP_EXT_CLANG_MACRO___has_cpp_attribute
#ifndef TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute
#define TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute "clang-__has_cpp_attribute"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute */
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_cpp_attribute, TPP_EXTNAME_CLANG_MACRO___has_cpp_attribute, TPP_HAVE_CLANG_MACRO___has_cpp_attribute == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_cpp_attribute(self) (self)->tes_flags.tef_TPP_EXT_CLANG_MACRO___has_cpp_attribute
#else /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_cpp_attribute(self) TPP_HAVE_CLANG_MACRO___has_cpp_attribute
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute >= 0 */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute < 0
#define TPP_EXT_CLANG_MACRO___has_declspec_attribute TPP_EXT_CLANG_MACRO___has_declspec_attribute
#ifndef TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute
#define TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute "clang-__has_declspec_attribute"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute */
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_declspec_attribute, TPP_EXTNAME_CLANG_MACRO___has_declspec_attribute, TPP_HAVE_CLANG_MACRO___has_declspec_attribute == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_declspec_attribute(self) (self)->tes_flags.tef_TPP_EXT_CLANG_MACRO___has_declspec_attribute
#else /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_declspec_attribute(self) TPP_HAVE_CLANG_MACRO___has_declspec_attribute
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute >= 0 */
#if TPP_HAVE_CLANG_MACRO___has_extension < 0
#define TPP_EXT_CLANG_MACRO___has_extension TPP_EXT_CLANG_MACRO___has_extension
#ifndef TPP_EXTNAME_CLANG_MACRO___has_extension
#define TPP_EXTNAME_CLANG_MACRO___has_extension "clang-__has_extension"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_extension */
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_extension, TPP_EXTNAME_CLANG_MACRO___has_extension, TPP_HAVE_CLANG_MACRO___has_extension == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_extension(self) (self)->tes_flags.tef_TPP_EXT_CLANG_MACRO___has_extension
#else /* TPP_HAVE_CLANG_MACRO___has_extension < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_extension(self) TPP_HAVE_CLANG_MACRO___has_extension
#endif /* TPP_HAVE_CLANG_MACRO___has_extension >= 0 */
#if TPP_HAVE_CLANG_MACRO___has_feature < 0
#define TPP_EXT_CLANG_MACRO___has_feature TPP_EXT_CLANG_MACRO___has_feature
#ifndef TPP_EXTNAME_CLANG_MACRO___has_feature
#define TPP_EXTNAME_CLANG_MACRO___has_feature "clang-__has_feature"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_feature */
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_feature, TPP_EXTNAME_CLANG_MACRO___has_feature, TPP_HAVE_CLANG_MACRO___has_feature == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_feature(self) (self)->tes_flags.tef_TPP_EXT_CLANG_MACRO___has_feature
#else /* TPP_HAVE_CLANG_MACRO___has_feature < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_feature(self) TPP_HAVE_CLANG_MACRO___has_feature
#endif /* TPP_HAVE_CLANG_MACRO___has_feature >= 0 */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute < 0
#define TPP_EXT_CLANG_MACRO___has_c_attribute TPP_EXT_CLANG_MACRO___has_c_attribute
#ifndef TPP_EXTNAME_CLANG_MACRO___has_c_attribute
#define TPP_EXTNAME_CLANG_MACRO___has_c_attribute "clang-__has_c_attribute"
#endif /* !TPP_EXTNAME_CLANG_MACRO___has_c_attribute */
TPP_EXTENSION(TPP_EXT_CLANG_MACRO___has_c_attribute, TPP_EXTNAME_CLANG_MACRO___has_c_attribute, TPP_HAVE_CLANG_MACRO___has_c_attribute == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_c_attribute(self) (self)->tes_flags.tef_TPP_EXT_CLANG_MACRO___has_c_attribute
#else /* TPP_HAVE_CLANG_MACRO___has_c_attribute < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_MACRO___has_c_attribute(self) TPP_HAVE_CLANG_MACRO___has_c_attribute
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute >= 0 */
#if TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES < 0
#define TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES
#ifndef TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES
#define TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES "clang-extensions-are-features"
#endif /* !TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES */
TPP_EXTENSION(TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES, TPP_EXTNAME_CLANG_EXTENSIONS_ARE_FEATURES, TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES(self) (self)->tes_flags.tef_TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES
#else /* TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES(self) TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES
#endif /* TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES >= 0 */
#if TPP_HAVE_MACRO___is_identifier < 0
#define TPP_EXT_MACRO___is_identifier TPP_EXT_MACRO___is_identifier
#ifndef TPP_EXTNAME_MACRO___is_identifier
#define TPP_EXTNAME_MACRO___is_identifier "__is_identifier"
#endif /* !TPP_EXTNAME_MACRO___is_identifier */
TPP_EXTENSION(TPP_EXT_MACRO___is_identifier, TPP_EXTNAME_MACRO___is_identifier, TPP_HAVE_MACRO___is_identifier == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___is_identifier(self) (self)->tes_flags.tef_TPP_EXT_MACRO___is_identifier
#else /* TPP_HAVE_MACRO___is_identifier < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___is_identifier(self) TPP_HAVE_MACRO___is_identifier
#endif /* TPP_HAVE_MACRO___is_identifier >= 0 */
#if TPP_HAVE_MACRO___is_deprecated < 0
#define TPP_EXT_MACRO___is_deprecated TPP_EXT_MACRO___is_deprecated
#ifndef TPP_EXTNAME_MACRO___is_deprecated
#define TPP_EXTNAME_MACRO___is_deprecated "__is_deprecated"
#endif /* !TPP_EXTNAME_MACRO___is_deprecated */
TPP_EXTENSION(TPP_EXT_MACRO___is_deprecated, TPP_EXTNAME_MACRO___is_deprecated, TPP_HAVE_MACRO___is_deprecated == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___is_deprecated(self) (self)->tes_flags.tef_TPP_EXT_MACRO___is_deprecated
#else /* TPP_HAVE_MACRO___is_deprecated < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___is_deprecated(self) TPP_HAVE_MACRO___is_deprecated
#endif /* TPP_HAVE_MACRO___is_deprecated >= 0 */
#if TPP_HAVE_MACRO___is_poisoned < 0
#define TPP_EXT_MACRO___is_poisoned TPP_EXT_MACRO___is_poisoned
#ifndef TPP_EXTNAME_MACRO___is_poisoned
#define TPP_EXTNAME_MACRO___is_poisoned "__is_poisoned"
#endif /* !TPP_EXTNAME_MACRO___is_poisoned */
TPP_EXTENSION(TPP_EXT_MACRO___is_poisoned, TPP_EXTNAME_MACRO___is_poisoned, TPP_HAVE_MACRO___is_poisoned == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___is_poisoned(self) (self)->tes_flags.tef_TPP_EXT_MACRO___is_poisoned
#else /* TPP_HAVE_MACRO___is_poisoned < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___is_poisoned(self) TPP_HAVE_MACRO___is_poisoned
#endif /* TPP_HAVE_MACRO___is_poisoned >= 0 */
#if TPP_HAVE_MACRO___has_extension < 0
#define TPP_EXT_MACRO___has_extension TPP_EXT_MACRO___has_extension
#ifndef TPP_EXTNAME_MACRO___has_extension
#define TPP_EXTNAME_MACRO___has_extension "__has_extension"
#endif /* !TPP_EXTNAME_MACRO___has_extension */
TPP_EXTENSION(TPP_EXT_MACRO___has_extension, TPP_EXTNAME_MACRO___has_extension, TPP_HAVE_MACRO___has_extension == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_extension(self) (self)->tes_flags.tef_TPP_EXT_MACRO___has_extension
#else /* TPP_HAVE_MACRO___has_extension < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_extension(self) TPP_HAVE_MACRO___has_extension
#endif /* TPP_HAVE_MACRO___has_extension >= 0 */
#if TPP_HAVE_MACRO___has_known_extension < 0
#define TPP_EXT_MACRO___has_known_extension TPP_EXT_MACRO___has_known_extension
#ifndef TPP_EXTNAME_MACRO___has_known_extension
#define TPP_EXTNAME_MACRO___has_known_extension "__has_known_extension"
#endif /* !TPP_EXTNAME_MACRO___has_known_extension */
TPP_EXTENSION(TPP_EXT_MACRO___has_known_extension, TPP_EXTNAME_MACRO___has_known_extension, TPP_HAVE_MACRO___has_known_extension == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_known_extension(self) (self)->tes_flags.tef_TPP_EXT_MACRO___has_known_extension
#else /* TPP_HAVE_MACRO___has_known_extension < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_known_extension(self) TPP_HAVE_MACRO___has_known_extension
#endif /* TPP_HAVE_MACRO___has_known_extension >= 0 */
#if TPP_HAVE_MACRO___has_warning < 0
#define TPP_EXT_MACRO___has_warning TPP_EXT_MACRO___has_warning
#ifndef TPP_EXTNAME_MACRO___has_warning
#define TPP_EXTNAME_MACRO___has_warning "__has_warning"
#endif /* !TPP_EXTNAME_MACRO___has_warning */
TPP_EXTENSION(TPP_EXT_MACRO___has_warning, TPP_EXTNAME_MACRO___has_warning, TPP_HAVE_MACRO___has_warning == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_warning(self) (self)->tes_flags.tef_TPP_EXT_MACRO___has_warning
#else /* TPP_HAVE_MACRO___has_warning < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_warning(self) TPP_HAVE_MACRO___has_warning
#endif /* TPP_HAVE_MACRO___has_warning >= 0 */
#if TPP_HAVE_MACRO___has_known_warning < 0
#define TPP_EXT_MACRO___has_known_warning TPP_EXT_MACRO___has_known_warning
#ifndef TPP_EXTNAME_MACRO___has_known_warning
#define TPP_EXTNAME_MACRO___has_known_warning "__has_known_warning"
#endif /* !TPP_EXTNAME_MACRO___has_known_warning */
TPP_EXTENSION(TPP_EXT_MACRO___has_known_warning, TPP_EXTNAME_MACRO___has_known_warning, TPP_HAVE_MACRO___has_known_warning == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_known_warning(self) (self)->tes_flags.tef_TPP_EXT_MACRO___has_known_warning
#else /* TPP_HAVE_MACRO___has_known_warning < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_known_warning(self) TPP_HAVE_MACRO___has_known_warning
#endif /* TPP_HAVE_MACRO___has_known_warning >= 0 */
#if TPP_HAVE_MACRO___has_include < 0
#define TPP_EXT_MACRO___has_include TPP_EXT_MACRO___has_include
#ifndef TPP_EXTNAME_MACRO___has_include
#define TPP_EXTNAME_MACRO___has_include "__has_include"
#endif /* !TPP_EXTNAME_MACRO___has_include */
TPP_EXTENSION(TPP_EXT_MACRO___has_include, TPP_EXTNAME_MACRO___has_include, TPP_HAVE_MACRO___has_include == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_include(self) (self)->tes_flags.tef_TPP_EXT_MACRO___has_include
#else /* TPP_HAVE_MACRO___has_include < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_include(self) TPP_HAVE_MACRO___has_include
#endif /* TPP_HAVE_MACRO___has_include >= 0 */
#if TPP_HAVE_MACRO___has_include_next < 0
#define TPP_EXT_MACRO___has_include_next TPP_EXT_MACRO___has_include_next
#ifndef TPP_EXTNAME_MACRO___has_include_next
#define TPP_EXTNAME_MACRO___has_include_next "__has_include_next"
#endif /* !TPP_EXTNAME_MACRO___has_include_next */
TPP_EXTENSION(TPP_EXT_MACRO___has_include_next, TPP_EXTNAME_MACRO___has_include_next, TPP_HAVE_MACRO___has_include_next == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_include_next(self) (self)->tes_flags.tef_TPP_EXT_MACRO___has_include_next
#else /* TPP_HAVE_MACRO___has_include_next < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_include_next(self) TPP_HAVE_MACRO___has_include_next
#endif /* TPP_HAVE_MACRO___has_include_next >= 0 */
#if TPP_HAVE_MACRO___has_embed < 0
#define TPP_EXT_MACRO___has_embed TPP_EXT_MACRO___has_embed
#ifndef TPP_EXTNAME_MACRO___has_embed
#define TPP_EXTNAME_MACRO___has_embed "__has_embed"
#endif /* !TPP_EXTNAME_MACRO___has_embed */
TPP_EXTENSION(TPP_EXT_MACRO___has_embed, TPP_EXTNAME_MACRO___has_embed, TPP_HAVE_MACRO___has_embed == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_embed(self) (self)->tes_flags.tef_TPP_EXT_MACRO___has_embed
#else /* TPP_HAVE_MACRO___has_embed < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___has_embed(self) TPP_HAVE_MACRO___has_embed
#endif /* TPP_HAVE_MACRO___has_embed >= 0 */
#if TPP_HAVE_MACRO___FILE__ < 0
#define TPP_EXT_MACRO___FILE__ TPP_EXT_MACRO___FILE__
#ifndef TPP_EXTNAME_MACRO___FILE__
#define TPP_EXTNAME_MACRO___FILE__ "__FILE__"
#endif /* !TPP_EXTNAME_MACRO___FILE__ */
TPP_EXTENSION(TPP_EXT_MACRO___FILE__, TPP_EXTNAME_MACRO___FILE__, TPP_HAVE_MACRO___FILE__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___FILE__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___FILE__
#else /* TPP_HAVE_MACRO___FILE__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___FILE__(self) TPP_HAVE_MACRO___FILE__
#endif /* TPP_HAVE_MACRO___FILE__ >= 0 */
#if TPP_HAVE_MACRO___LINE__ < 0
#define TPP_EXT_MACRO___LINE__ TPP_EXT_MACRO___LINE__
#ifndef TPP_EXTNAME_MACRO___LINE__
#define TPP_EXTNAME_MACRO___LINE__ "__LINE__"
#endif /* !TPP_EXTNAME_MACRO___LINE__ */
TPP_EXTENSION(TPP_EXT_MACRO___LINE__, TPP_EXTNAME_MACRO___LINE__, TPP_HAVE_MACRO___LINE__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___LINE__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___LINE__
#else /* TPP_HAVE_MACRO___LINE__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___LINE__(self) TPP_HAVE_MACRO___LINE__
#endif /* TPP_HAVE_MACRO___LINE__ >= 0 */
#if TPP_HAVE_MACRO___TIME__ < 0
#define TPP_EXT_MACRO___TIME__ TPP_EXT_MACRO___TIME__
#ifndef TPP_EXTNAME_MACRO___TIME__
#define TPP_EXTNAME_MACRO___TIME__ "__TIME__"
#endif /* !TPP_EXTNAME_MACRO___TIME__ */
TPP_EXTENSION(TPP_EXT_MACRO___TIME__, TPP_EXTNAME_MACRO___TIME__, TPP_HAVE_MACRO___TIME__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TIME__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TIME__
#else /* TPP_HAVE_MACRO___TIME__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TIME__(self) TPP_HAVE_MACRO___TIME__
#endif /* TPP_HAVE_MACRO___TIME__ >= 0 */
#if TPP_HAVE_MACRO___DATE__ < 0
#define TPP_EXT_MACRO___DATE__ TPP_EXT_MACRO___DATE__
#ifndef TPP_EXTNAME_MACRO___DATE__
#define TPP_EXTNAME_MACRO___DATE__ "__DATE__"
#endif /* !TPP_EXTNAME_MACRO___DATE__ */
TPP_EXTENSION(TPP_EXT_MACRO___DATE__, TPP_EXTNAME_MACRO___DATE__, TPP_HAVE_MACRO___DATE__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___DATE__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___DATE__
#else /* TPP_HAVE_MACRO___DATE__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___DATE__(self) TPP_HAVE_MACRO___DATE__
#endif /* TPP_HAVE_MACRO___DATE__ >= 0 */
#if TPP_HAVE_MACRO___COLUMN__ < 0
#define TPP_EXT_MACRO___COLUMN__ TPP_EXT_MACRO___COLUMN__
#ifndef TPP_EXTNAME_MACRO___COLUMN__
#define TPP_EXTNAME_MACRO___COLUMN__ "column-macro"
#endif /* !TPP_EXTNAME_MACRO___COLUMN__ */
TPP_EXTENSION(TPP_EXT_MACRO___COLUMN__, TPP_EXTNAME_MACRO___COLUMN__, TPP_HAVE_MACRO___COLUMN__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___COLUMN__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___COLUMN__
#else /* TPP_HAVE_MACRO___COLUMN__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___COLUMN__(self) TPP_HAVE_MACRO___COLUMN__
#endif /* TPP_HAVE_MACRO___COLUMN__ >= 0 */
#if TPP_HAVE_MACRO___BASE_FILE__ < 0
#define TPP_EXT_MACRO___BASE_FILE__ TPP_EXT_MACRO___BASE_FILE__
#ifndef TPP_EXTNAME_MACRO___BASE_FILE__
#define TPP_EXTNAME_MACRO___BASE_FILE__ "basefile-macro"
#endif /* !TPP_EXTNAME_MACRO___BASE_FILE__ */
TPP_EXTENSION(TPP_EXT_MACRO___BASE_FILE__, TPP_EXTNAME_MACRO___BASE_FILE__, TPP_HAVE_MACRO___BASE_FILE__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___BASE_FILE__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___BASE_FILE__
#else /* TPP_HAVE_MACRO___BASE_FILE__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___BASE_FILE__(self) TPP_HAVE_MACRO___BASE_FILE__
#endif /* TPP_HAVE_MACRO___BASE_FILE__ >= 0 */
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__ < 0
#define TPP_EXT_MACRO___INCLUDE_LEVEL__ TPP_EXT_MACRO___INCLUDE_LEVEL__
#ifndef TPP_EXTNAME_MACRO___INCLUDE_LEVEL__
#define TPP_EXTNAME_MACRO___INCLUDE_LEVEL__ "include-level-macro"
#endif /* !TPP_EXTNAME_MACRO___INCLUDE_LEVEL__ */
TPP_EXTENSION(TPP_EXT_MACRO___INCLUDE_LEVEL__, TPP_EXTNAME_MACRO___INCLUDE_LEVEL__, TPP_HAVE_MACRO___INCLUDE_LEVEL__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___INCLUDE_LEVEL__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___INCLUDE_LEVEL__
#else /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___INCLUDE_LEVEL__(self) TPP_HAVE_MACRO___INCLUDE_LEVEL__
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ >= 0 */
#if TPP_HAVE_MACRO___INCLUDE_DEPTH__ < 0
#define TPP_EXT_MACRO___INCLUDE_DEPTH__ TPP_EXT_MACRO___INCLUDE_DEPTH__
#ifndef TPP_EXTNAME_MACRO___INCLUDE_DEPTH__
#define TPP_EXTNAME_MACRO___INCLUDE_DEPTH__ "include-depth-macro"
#endif /* !TPP_EXTNAME_MACRO___INCLUDE_DEPTH__ */
TPP_EXTENSION(TPP_EXT_MACRO___INCLUDE_DEPTH__, TPP_EXTNAME_MACRO___INCLUDE_DEPTH__, TPP_HAVE_MACRO___INCLUDE_DEPTH__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___INCLUDE_DEPTH__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___INCLUDE_DEPTH__
#else /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___INCLUDE_DEPTH__(self) TPP_HAVE_MACRO___INCLUDE_DEPTH__
#endif /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ >= 0 */
#if TPP_HAVE_MACRO___COUNTER__ < 0
#define TPP_EXT_MACRO___COUNTER__ TPP_EXT_MACRO___COUNTER__
#ifndef TPP_EXTNAME_MACRO___COUNTER__
#define TPP_EXTNAME_MACRO___COUNTER__ "counter-macro"
#endif /* !TPP_EXTNAME_MACRO___COUNTER__ */
TPP_EXTENSION(TPP_EXT_MACRO___COUNTER__, TPP_EXTNAME_MACRO___COUNTER__, TPP_HAVE_MACRO___COUNTER__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___COUNTER__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___COUNTER__
#else /* TPP_HAVE_MACRO___COUNTER__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___COUNTER__(self) TPP_HAVE_MACRO___COUNTER__
#endif /* TPP_HAVE_MACRO___COUNTER__ >= 0 */
#if TPP_HAVE_MACRO___TIMESTAMP__ < 0
#define TPP_EXT_MACRO___TIMESTAMP__ TPP_EXT_MACRO___TIMESTAMP__
#ifndef TPP_EXTNAME_MACRO___TIMESTAMP__
#define TPP_EXTNAME_MACRO___TIMESTAMP__ "timestamp-macro"
#endif /* !TPP_EXTNAME_MACRO___TIMESTAMP__ */
TPP_EXTENSION(TPP_EXT_MACRO___TIMESTAMP__, TPP_EXTNAME_MACRO___TIMESTAMP__, TPP_HAVE_MACRO___TIMESTAMP__ == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TIMESTAMP__(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TIMESTAMP__
#else /* TPP_HAVE_MACRO___TIMESTAMP__ < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TIMESTAMP__(self) TPP_HAVE_MACRO___TIMESTAMP__
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ >= 0 */
#if TPP_HAVE_NUMERIC_DATE_MACROS < 0
#define TPP_EXT_NUMERIC_DATE_MACROS TPP_EXT_NUMERIC_DATE_MACROS
#ifndef TPP_EXTNAME_NUMERIC_DATE_MACROS
#define TPP_EXTNAME_NUMERIC_DATE_MACROS "numeric-date-macros"
#endif /* !TPP_EXTNAME_NUMERIC_DATE_MACROS */
TPP_EXTENSION(TPP_EXT_NUMERIC_DATE_MACROS, TPP_EXTNAME_NUMERIC_DATE_MACROS, TPP_HAVE_NUMERIC_DATE_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_NUMERIC_DATE_MACROS(self) (self)->tes_flags.tef_TPP_EXT_NUMERIC_DATE_MACROS
#else /* TPP_HAVE_NUMERIC_DATE_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_NUMERIC_DATE_MACROS(self) TPP_HAVE_NUMERIC_DATE_MACROS
#endif /* TPP_HAVE_NUMERIC_DATE_MACROS >= 0 */
#if TPP_HAVE_NUMERIC_TIME_MACROS < 0
#define TPP_EXT_NUMERIC_TIME_MACROS TPP_EXT_NUMERIC_TIME_MACROS
#ifndef TPP_EXTNAME_NUMERIC_TIME_MACROS
#define TPP_EXTNAME_NUMERIC_TIME_MACROS "numeric-time-macros"
#endif /* !TPP_EXTNAME_NUMERIC_TIME_MACROS */
TPP_EXTENSION(TPP_EXT_NUMERIC_TIME_MACROS, TPP_EXTNAME_NUMERIC_TIME_MACROS, TPP_HAVE_NUMERIC_TIME_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_NUMERIC_TIME_MACROS(self) (self)->tes_flags.tef_TPP_EXT_NUMERIC_TIME_MACROS
#else /* TPP_HAVE_NUMERIC_TIME_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_NUMERIC_TIME_MACROS(self) TPP_HAVE_NUMERIC_TIME_MACROS
#endif /* TPP_HAVE_NUMERIC_TIME_MACROS >= 0 */
#if TPP_HAVE_MACRO___TPP_EVAL < 0
#define TPP_EXT_MACRO___TPP_EVAL TPP_EXT_MACRO___TPP_EVAL
#ifndef TPP_EXTNAME_MACRO___TPP_EVAL
#define TPP_EXTNAME_MACRO___TPP_EVAL "tpp-eval-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_EVAL */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_EVAL, TPP_EXTNAME_MACRO___TPP_EVAL, TPP_HAVE_MACRO___TPP_EVAL == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_EVAL(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_EVAL
#else /* TPP_HAVE_MACRO___TPP_EVAL < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_EVAL(self) TPP_HAVE_MACRO___TPP_EVAL
#endif /* TPP_HAVE_MACRO___TPP_EVAL >= 0 */
#if TPP_HAVE_MACRO___TPP_UNIQUE < 0
#define TPP_EXT_MACRO___TPP_UNIQUE TPP_EXT_MACRO___TPP_UNIQUE
#ifndef TPP_EXTNAME_MACRO___TPP_UNIQUE
#define TPP_EXTNAME_MACRO___TPP_UNIQUE "tpp-unique-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_UNIQUE */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_UNIQUE, TPP_EXTNAME_MACRO___TPP_UNIQUE, TPP_HAVE_MACRO___TPP_UNIQUE == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_UNIQUE(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_UNIQUE
#else /* TPP_HAVE_MACRO___TPP_UNIQUE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_UNIQUE(self) TPP_HAVE_MACRO___TPP_UNIQUE
#endif /* TPP_HAVE_MACRO___TPP_UNIQUE >= 0 */
#if TPP_HAVE_MACRO___TPP_LOAD_FILE < 0
#define TPP_EXT_MACRO___TPP_LOAD_FILE TPP_EXT_MACRO___TPP_LOAD_FILE
#ifndef TPP_EXTNAME_MACRO___TPP_LOAD_FILE
#define TPP_EXTNAME_MACRO___TPP_LOAD_FILE "tpp-load-file-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_LOAD_FILE */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_LOAD_FILE, TPP_EXTNAME_MACRO___TPP_LOAD_FILE, TPP_HAVE_MACRO___TPP_LOAD_FILE == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_LOAD_FILE(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_LOAD_FILE
#else /* TPP_HAVE_MACRO___TPP_LOAD_FILE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_LOAD_FILE(self) TPP_HAVE_MACRO___TPP_LOAD_FILE
#endif /* TPP_HAVE_MACRO___TPP_LOAD_FILE >= 0 */
#if TPP_HAVE_MACRO___TPP_COUNTER < 0
#define TPP_EXT_MACRO___TPP_COUNTER TPP_EXT_MACRO___TPP_COUNTER
#ifndef TPP_EXTNAME_MACRO___TPP_COUNTER
#define TPP_EXTNAME_MACRO___TPP_COUNTER "tpp-counter-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_COUNTER */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_COUNTER, TPP_EXTNAME_MACRO___TPP_COUNTER, TPP_HAVE_MACRO___TPP_COUNTER == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_COUNTER(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_COUNTER
#else /* TPP_HAVE_MACRO___TPP_COUNTER < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_COUNTER(self) TPP_HAVE_MACRO___TPP_COUNTER
#endif /* TPP_HAVE_MACRO___TPP_COUNTER >= 0 */
#if TPP_HAVE_MACRO___TPP_RANDOM < 0
#define TPP_EXT_MACRO___TPP_RANDOM TPP_EXT_MACRO___TPP_RANDOM
#ifndef TPP_EXTNAME_MACRO___TPP_RANDOM
#define TPP_EXTNAME_MACRO___TPP_RANDOM "tpp-random-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_RANDOM */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_RANDOM, TPP_EXTNAME_MACRO___TPP_RANDOM, TPP_HAVE_MACRO___TPP_RANDOM == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_RANDOM(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_RANDOM
#else /* TPP_HAVE_MACRO___TPP_RANDOM < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_RANDOM(self) TPP_HAVE_MACRO___TPP_RANDOM
#endif /* TPP_HAVE_MACRO___TPP_RANDOM >= 0 */
#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE < 0
#define TPP_EXT_MACRO___TPP_STR_DECOMPILE TPP_EXT_MACRO___TPP_STR_DECOMPILE
#ifndef TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE
#define TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE "tpp-str-decompile-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_STR_DECOMPILE, TPP_EXTNAME_MACRO___TPP_STR_DECOMPILE, TPP_HAVE_MACRO___TPP_STR_DECOMPILE == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_STR_DECOMPILE(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_STR_DECOMPILE
#else /* TPP_HAVE_MACRO___TPP_STR_DECOMPILE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_STR_DECOMPILE(self) TPP_HAVE_MACRO___TPP_STR_DECOMPILE
#endif /* TPP_HAVE_MACRO___TPP_STR_DECOMPILE >= 0 */
#if TPP_HAVE_MACRO___TPP_STR_SUBSTR < 0
#define TPP_EXT_MACRO___TPP_STR_SUBSTR TPP_EXT_MACRO___TPP_STR_SUBSTR
#ifndef TPP_EXTNAME_MACRO___TPP_STR_SUBSTR
#define TPP_EXTNAME_MACRO___TPP_STR_SUBSTR "tpp-str-substr-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_STR_SUBSTR */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_STR_SUBSTR, TPP_EXTNAME_MACRO___TPP_STR_SUBSTR, TPP_HAVE_MACRO___TPP_STR_SUBSTR == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_STR_SUBSTR(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_STR_SUBSTR
#else /* TPP_HAVE_MACRO___TPP_STR_SUBSTR < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_STR_SUBSTR(self) TPP_HAVE_MACRO___TPP_STR_SUBSTR
#endif /* TPP_HAVE_MACRO___TPP_STR_SUBSTR >= 0 */
#if TPP_HAVE_MACRO___TPP_STR_PACK < 0
#define TPP_EXT_MACRO___TPP_STR_PACK TPP_EXT_MACRO___TPP_STR_PACK
#ifndef TPP_EXTNAME_MACRO___TPP_STR_PACK
#define TPP_EXTNAME_MACRO___TPP_STR_PACK "tpp-str-pack-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_STR_PACK */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_STR_PACK, TPP_EXTNAME_MACRO___TPP_STR_PACK, TPP_HAVE_MACRO___TPP_STR_PACK == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_STR_PACK(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_STR_PACK
#else /* TPP_HAVE_MACRO___TPP_STR_PACK < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_STR_PACK(self) TPP_HAVE_MACRO___TPP_STR_PACK
#endif /* TPP_HAVE_MACRO___TPP_STR_PACK >= 0 */
#if TPP_HAVE_MACRO___TPP_STR_SIZE < 0
#define TPP_EXT_MACRO___TPP_STR_SIZE TPP_EXT_MACRO___TPP_STR_SIZE
#ifndef TPP_EXTNAME_MACRO___TPP_STR_SIZE
#define TPP_EXTNAME_MACRO___TPP_STR_SIZE "tpp-str-size-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_STR_SIZE */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_STR_SIZE, TPP_EXTNAME_MACRO___TPP_STR_SIZE, TPP_HAVE_MACRO___TPP_STR_SIZE == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_STR_SIZE(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_STR_SIZE
#else /* TPP_HAVE_MACRO___TPP_STR_SIZE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_STR_SIZE(self) TPP_HAVE_MACRO___TPP_STR_SIZE
#endif /* TPP_HAVE_MACRO___TPP_STR_SIZE >= 0 */
#if TPP_HAVE_MACRO___TPP_COUNT_TOKENS < 0
#define TPP_EXT_MACRO___TPP_COUNT_TOKENS TPP_EXT_MACRO___TPP_COUNT_TOKENS
#ifndef TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS
#define TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS "tpp-count-tokens-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_COUNT_TOKENS, TPP_EXTNAME_MACRO___TPP_COUNT_TOKENS, TPP_HAVE_MACRO___TPP_COUNT_TOKENS == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_COUNT_TOKENS(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_COUNT_TOKENS
#else /* TPP_HAVE_MACRO___TPP_COUNT_TOKENS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_COUNT_TOKENS(self) TPP_HAVE_MACRO___TPP_COUNT_TOKENS
#endif /* TPP_HAVE_MACRO___TPP_COUNT_TOKENS >= 0 */
#if TPP_HAVE_MACRO___TPP_IDENTIFIER < 0
#define TPP_EXT_MACRO___TPP_IDENTIFIER TPP_EXT_MACRO___TPP_IDENTIFIER
#ifndef TPP_EXTNAME_MACRO___TPP_IDENTIFIER
#define TPP_EXTNAME_MACRO___TPP_IDENTIFIER "tpp-identifier-macro"
#endif /* !TPP_EXTNAME_MACRO___TPP_IDENTIFIER */
TPP_EXTENSION(TPP_EXT_MACRO___TPP_IDENTIFIER, TPP_EXTNAME_MACRO___TPP_IDENTIFIER, TPP_HAVE_MACRO___TPP_IDENTIFIER == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_IDENTIFIER(self) (self)->tes_flags.tef_TPP_EXT_MACRO___TPP_IDENTIFIER
#else /* TPP_HAVE_MACRO___TPP_IDENTIFIER < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO___TPP_IDENTIFIER(self) TPP_HAVE_MACRO___TPP_IDENTIFIER
#endif /* TPP_HAVE_MACRO___TPP_IDENTIFIER >= 0 */
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS < 0
#define TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
#ifndef TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS "alternative-macro-parenthesis"
#endif /* !TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS */
TPP_EXTENSION(TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS, TPP_EXTNAME_ALTERNATIVE_MACRO_PARENTHESIS, TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS == -1)
#define _tpp_extensions_state_get_TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS(self) (self)->tes_flags.tef_TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
#else /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS(self) TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS >= 0 */
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
#define TPP_EXT_MACRO_ARGUMENT_WHITESPACE TPP_EXT_MACRO_ARGUMENT_WHITESPACE
#ifndef TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE
#define TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE "macro-argument-whitespace"
#endif /* !TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE */
TPP_EXTENSION(TPP_EXT_MACRO_ARGUMENT_WHITESPACE, TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE, TPP_HAVE_MACRO_ARGUMENT_WHITESPACE == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO_ARGUMENT_WHITESPACE(self) (self)->tes_flags.tef_TPP_EXT_MACRO_ARGUMENT_WHITESPACE
#else /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO_ARGUMENT_WHITESPACE(self) TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE >= 0 */
#if TPP_HAVE_MACRO_RECURSION < 0
#define TPP_EXT_MACRO_RECURSION TPP_EXT_MACRO_RECURSION
#ifndef TPP_EXTNAME_MACRO_RECURSION
#define TPP_EXTNAME_MACRO_RECURSION "macro-recursion"
#endif /* !TPP_EXTNAME_MACRO_RECURSION */
TPP_EXTENSION(TPP_EXT_MACRO_RECURSION, TPP_EXTNAME_MACRO_RECURSION, TPP_HAVE_MACRO_RECURSION == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO_RECURSION(self) (self)->tes_flags.tef_TPP_EXT_MACRO_RECURSION
#else /* TPP_HAVE_MACRO_RECURSION < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO_RECURSION(self) TPP_HAVE_MACRO_RECURSION
#endif /* TPP_HAVE_MACRO_RECURSION >= 0 */
#if TPP_HAVE_TRADITIONAL_MACROS < 0
#define TPP_EXT_TRADITIONAL_MACROS TPP_EXT_TRADITIONAL_MACROS
#ifndef TPP_EXTNAME_TRADITIONAL_MACROS
#define TPP_EXTNAME_TRADITIONAL_MACROS "traditional-macro"
#endif /* !TPP_EXTNAME_TRADITIONAL_MACROS */
TPP_EXTENSION(TPP_EXT_TRADITIONAL_MACROS, TPP_EXTNAME_TRADITIONAL_MACROS, TPP_HAVE_TRADITIONAL_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_TRADITIONAL_MACROS(self) (self)->tes_flags.tef_TPP_EXT_TRADITIONAL_MACROS
#else /* TPP_HAVE_TRADITIONAL_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_TRADITIONAL_MACROS(self) TPP_HAVE_TRADITIONAL_MACROS
#endif /* TPP_HAVE_TRADITIONAL_MACROS >= 0 */
#if TPP_HAVE_NAMED_VARARGS_IN_MACROS < 0
#define TPP_EXT_NAMED_VARARGS_IN_MACROS TPP_EXT_NAMED_VARARGS_IN_MACROS
#ifndef TPP_EXTNAME_NAMED_VARARGS_IN_MACROS
#define TPP_EXTNAME_NAMED_VARARGS_IN_MACROS "named-varargs-in-macros"
#endif /* !TPP_EXTNAME_NAMED_VARARGS_IN_MACROS */
TPP_EXTENSION(TPP_EXT_NAMED_VARARGS_IN_MACROS, TPP_EXTNAME_NAMED_VARARGS_IN_MACROS, TPP_HAVE_NAMED_VARARGS_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_NAMED_VARARGS_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_NAMED_VARARGS_IN_MACROS
#else /* TPP_HAVE_NAMED_VARARGS_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_NAMED_VARARGS_IN_MACROS(self) TPP_HAVE_NAMED_VARARGS_IN_MACROS
#endif /* TPP_HAVE_NAMED_VARARGS_IN_MACROS >= 0 */
#if TPP_HAVE_VA_ARGS_IN_MACROS < 0
#define TPP_EXT_VA_ARGS_IN_MACROS TPP_EXT_VA_ARGS_IN_MACROS
#ifndef TPP_EXTNAME_VA_ARGS_IN_MACROS
#define TPP_EXTNAME_VA_ARGS_IN_MACROS "va-args-in-macros"
#endif /* !TPP_EXTNAME_VA_ARGS_IN_MACROS */
TPP_EXTENSION(TPP_EXT_VA_ARGS_IN_MACROS, TPP_EXTNAME_VA_ARGS_IN_MACROS, TPP_HAVE_VA_ARGS_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_VA_ARGS_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_VA_ARGS_IN_MACROS
#else /* TPP_HAVE_VA_ARGS_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_VA_ARGS_IN_MACROS(self) TPP_HAVE_VA_ARGS_IN_MACROS
#endif /* TPP_HAVE_VA_ARGS_IN_MACROS >= 0 */
#if TPP_HAVE_VA_COMMA_IN_MACROS < 0
#define TPP_EXT_VA_COMMA_IN_MACROS TPP_EXT_VA_COMMA_IN_MACROS
#ifndef TPP_EXTNAME_VA_COMMA_IN_MACROS
#define TPP_EXTNAME_VA_COMMA_IN_MACROS "va-comma-in-macros"
#endif /* !TPP_EXTNAME_VA_COMMA_IN_MACROS */
TPP_EXTENSION(TPP_EXT_VA_COMMA_IN_MACROS, TPP_EXTNAME_VA_COMMA_IN_MACROS, TPP_HAVE_VA_COMMA_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_VA_COMMA_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_VA_COMMA_IN_MACROS
#else /* TPP_HAVE_VA_COMMA_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_VA_COMMA_IN_MACROS(self) TPP_HAVE_VA_COMMA_IN_MACROS
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS >= 0 */
#if TPP_HAVE_VA_OPT_IN_MACROS < 0
#define TPP_EXT_VA_OPT_IN_MACROS TPP_EXT_VA_OPT_IN_MACROS
#ifndef TPP_EXTNAME_VA_OPT_IN_MACROS
#define TPP_EXTNAME_VA_OPT_IN_MACROS "va-opt-in-macros"
#endif /* !TPP_EXTNAME_VA_OPT_IN_MACROS */
TPP_EXTENSION(TPP_EXT_VA_OPT_IN_MACROS, TPP_EXTNAME_VA_OPT_IN_MACROS, TPP_HAVE_VA_OPT_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_VA_OPT_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_VA_OPT_IN_MACROS
#else /* TPP_HAVE_VA_OPT_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_VA_OPT_IN_MACROS(self) TPP_HAVE_VA_OPT_IN_MACROS
#endif /* TPP_HAVE_VA_OPT_IN_MACROS >= 0 */
#if TPP_HAVE_VA_NARGS_IN_MACROS < 0
#define TPP_EXT_VA_NARGS_IN_MACROS TPP_EXT_VA_NARGS_IN_MACROS
#ifndef TPP_EXTNAME_VA_NARGS_IN_MACROS
#define TPP_EXTNAME_VA_NARGS_IN_MACROS "va-nargs-in-macros"
#endif /* !TPP_EXTNAME_VA_NARGS_IN_MACROS */
TPP_EXTENSION(TPP_EXT_VA_NARGS_IN_MACROS, TPP_EXTNAME_VA_NARGS_IN_MACROS, TPP_HAVE_VA_NARGS_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_VA_NARGS_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_VA_NARGS_IN_MACROS
#else /* TPP_HAVE_VA_NARGS_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_VA_NARGS_IN_MACROS(self) TPP_HAVE_VA_NARGS_IN_MACROS
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS >= 0 */
#if TPP_HAVE_VA_GLUE_COMMA_IN_MACROS < 0
#define TPP_EXT_VA_GLUE_COMMA_IN_MACROS TPP_EXT_VA_GLUE_COMMA_IN_MACROS
#ifndef TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS
#define TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS "glue-comma-in-macros"
#endif /* !TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS */
TPP_EXTENSION(TPP_EXT_VA_GLUE_COMMA_IN_MACROS, TPP_EXTNAME_VA_GLUE_COMMA_IN_MACROS, TPP_HAVE_VA_GLUE_COMMA_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_VA_GLUE_COMMA_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_VA_GLUE_COMMA_IN_MACROS
#else /* TPP_HAVE_VA_GLUE_COMMA_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_VA_GLUE_COMMA_IN_MACROS(self) TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
#endif /* TPP_HAVE_VA_GLUE_COMMA_IN_MACROS >= 0 */
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT < 0
#define TPP_EXT_STRINGIZE_MACRO_ARGUMENT TPP_EXT_STRINGIZE_MACRO_ARGUMENT
#ifndef TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT
#define TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT "stringize-macro-argument"
#endif /* !TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT */
TPP_EXTENSION(TPP_EXT_STRINGIZE_MACRO_ARGUMENT, TPP_EXTNAME_STRINGIZE_MACRO_ARGUMENT, TPP_HAVE_STRINGIZE_MACRO_ARGUMENT == -1)
#define _tpp_extensions_state_get_TPP_EXT_STRINGIZE_MACRO_ARGUMENT(self) (self)->tes_flags.tef_TPP_EXT_STRINGIZE_MACRO_ARGUMENT
#else /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT < 0 */
#define _tpp_extensions_state_get_TPP_EXT_STRINGIZE_MACRO_ARGUMENT(self) TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT >= 0 */
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT < 0
#define TPP_EXT_CHARIZE_MACRO_ARGUMENT TPP_EXT_CHARIZE_MACRO_ARGUMENT
#ifndef TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT
#define TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT "charize-macro-argument"
#endif /* !TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT */
TPP_EXTENSION(TPP_EXT_CHARIZE_MACRO_ARGUMENT, TPP_EXTNAME_CHARIZE_MACRO_ARGUMENT, TPP_HAVE_CHARIZE_MACRO_ARGUMENT == -1)
#define _tpp_extensions_state_get_TPP_EXT_CHARIZE_MACRO_ARGUMENT(self) (self)->tes_flags.tef_TPP_EXT_CHARIZE_MACRO_ARGUMENT
#else /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CHARIZE_MACRO_ARGUMENT(self) TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT >= 0 */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT < 0
#define TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT
#ifndef TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT
#define TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT "dont-expand-macro-argument"
#endif /* !TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT */
TPP_EXTENSION(TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT, TPP_EXTNAME_DONT_EXPAND_MACRO_ARGUMENT, TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT == -1)
#define _tpp_extensions_state_get_TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT(self) (self)->tes_flags.tef_TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT
#else /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT < 0 */
#define _tpp_extensions_state_get_TPP_EXT_DONT_EXPAND_MACRO_ARGUMENT(self) TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT >= 0 */
#if TPP_HAVE_GLUE_MACRO_ARGUMENT < 0
#define TPP_EXT_GLUE_MACRO_ARGUMENT TPP_EXT_GLUE_MACRO_ARGUMENT
#ifndef TPP_EXTNAME_GLUE_MACRO_ARGUMENT
#define TPP_EXTNAME_GLUE_MACRO_ARGUMENT "glue-macro-argument"
#endif /* !TPP_EXTNAME_GLUE_MACRO_ARGUMENT */
TPP_EXTENSION(TPP_EXT_GLUE_MACRO_ARGUMENT, TPP_EXTNAME_GLUE_MACRO_ARGUMENT, TPP_HAVE_GLUE_MACRO_ARGUMENT == -1)
#define _tpp_extensions_state_get_TPP_EXT_GLUE_MACRO_ARGUMENT(self) (self)->tes_flags.tef_TPP_EXT_GLUE_MACRO_ARGUMENT
#else /* TPP_HAVE_GLUE_MACRO_ARGUMENT < 0 */
#define _tpp_extensions_state_get_TPP_EXT_GLUE_MACRO_ARGUMENT(self) TPP_HAVE_GLUE_MACRO_ARGUMENT
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT >= 0 */
#if TPP_HAVE_PRAGMA_PUSH_MACRO < 0
#define TPP_EXT_PRAGMA_PUSH_MACRO TPP_EXT_PRAGMA_PUSH_MACRO
#ifndef TPP_EXTNAME_PRAGMA_PUSH_MACRO
#define TPP_EXTNAME_PRAGMA_PUSH_MACRO "pragma-push-macro"
#endif /* !TPP_EXTNAME_PRAGMA_PUSH_MACRO */
TPP_EXTENSION(TPP_EXT_PRAGMA_PUSH_MACRO, TPP_EXTNAME_PRAGMA_PUSH_MACRO, TPP_HAVE_PRAGMA_PUSH_MACRO == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_PUSH_MACRO(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_PUSH_MACRO
#else /* TPP_HAVE_PRAGMA_PUSH_MACRO < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_PUSH_MACRO(self) TPP_HAVE_PRAGMA_PUSH_MACRO
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO >= 0 */
#if TPP_HAVE_PRAGMA_ONCE < 0
#define TPP_EXT_PRAGMA_ONCE TPP_EXT_PRAGMA_ONCE
#ifndef TPP_EXTNAME_PRAGMA_ONCE
#define TPP_EXTNAME_PRAGMA_ONCE "pragma-once"
#endif /* !TPP_EXTNAME_PRAGMA_ONCE */
TPP_EXTENSION(TPP_EXT_PRAGMA_ONCE, TPP_EXTNAME_PRAGMA_ONCE, TPP_HAVE_PRAGMA_ONCE == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_ONCE(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_ONCE
#else /* TPP_HAVE_PRAGMA_ONCE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_ONCE(self) TPP_HAVE_PRAGMA_ONCE
#endif /* TPP_HAVE_PRAGMA_ONCE >= 0 */
#if TPP_HAVE_PRAGMA_DEPRECATED < 0
#define TPP_EXT_PRAGMA_DEPRECATED TPP_EXT_PRAGMA_DEPRECATED
#ifndef TPP_EXTNAME_PRAGMA_DEPRECATED
#define TPP_EXTNAME_PRAGMA_DEPRECATED "pragma-deprecated"
#endif /* !TPP_EXTNAME_PRAGMA_DEPRECATED */
TPP_EXTENSION(TPP_EXT_PRAGMA_DEPRECATED, TPP_EXTNAME_PRAGMA_DEPRECATED, TPP_HAVE_PRAGMA_DEPRECATED == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_DEPRECATED(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_DEPRECATED
#else /* TPP_HAVE_PRAGMA_DEPRECATED < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_DEPRECATED(self) TPP_HAVE_PRAGMA_DEPRECATED
#endif /* TPP_HAVE_PRAGMA_DEPRECATED >= 0 */
#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS < 0
#define TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS
#ifndef TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS "pragma-tpp-set-keyword-flags"
#endif /* !TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS */
TPP_EXTENSION(TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS, TPP_EXTNAME_PRAGMA_TPP_SET_KEYWORD_FLAGS, TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS
#else /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS(self) TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS >= 0 */
#if TPP_HAVE_PRAGMA_EXTENSION < 0
#define TPP_EXT_PRAGMA_EXTENSION TPP_EXT_PRAGMA_EXTENSION
#ifndef TPP_EXTNAME_PRAGMA_EXTENSION
#define TPP_EXTNAME_PRAGMA_EXTENSION "pragma-extension"
#endif /* !TPP_EXTNAME_PRAGMA_EXTENSION */
TPP_EXTENSION(TPP_EXT_PRAGMA_EXTENSION, TPP_EXTNAME_PRAGMA_EXTENSION, TPP_HAVE_PRAGMA_EXTENSION == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_EXTENSION(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_EXTENSION
#else /* TPP_HAVE_PRAGMA_EXTENSION < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_EXTENSION(self) TPP_HAVE_PRAGMA_EXTENSION
#endif /* TPP_HAVE_PRAGMA_EXTENSION >= 0 */
#if TPP_HAVE_PRAGMA_EXTENSION_PUSH < 0
#define TPP_EXT_PRAGMA_EXTENSION_PUSH TPP_EXT_PRAGMA_EXTENSION_PUSH
#ifndef TPP_EXTNAME_PRAGMA_EXTENSION_PUSH
#define TPP_EXTNAME_PRAGMA_EXTENSION_PUSH "pragma-extension-push"
#endif /* !TPP_EXTNAME_PRAGMA_EXTENSION_PUSH */
TPP_EXTENSION(TPP_EXT_PRAGMA_EXTENSION_PUSH, TPP_EXTNAME_PRAGMA_EXTENSION_PUSH, TPP_HAVE_PRAGMA_EXTENSION_PUSH == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_EXTENSION_PUSH(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_EXTENSION_PUSH
#else /* TPP_HAVE_PRAGMA_EXTENSION_PUSH < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_EXTENSION_PUSH(self) TPP_HAVE_PRAGMA_EXTENSION_PUSH
#endif /* TPP_HAVE_PRAGMA_EXTENSION_PUSH >= 0 */
#if TPP_HAVE_PRAGMA_WARNING < 0
#define TPP_EXT_PRAGMA_WARNING TPP_EXT_PRAGMA_WARNING
#ifndef TPP_EXTNAME_PRAGMA_WARNING
#define TPP_EXTNAME_PRAGMA_WARNING "pragma-warning"
#endif /* !TPP_EXTNAME_PRAGMA_WARNING */
TPP_EXTENSION(TPP_EXT_PRAGMA_WARNING, TPP_EXTNAME_PRAGMA_WARNING, TPP_HAVE_PRAGMA_WARNING == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_WARNING(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_WARNING
#else /* TPP_HAVE_PRAGMA_WARNING < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_WARNING(self) TPP_HAVE_PRAGMA_WARNING
#endif /* TPP_HAVE_PRAGMA_WARNING >= 0 */
#if TPP_HAVE_PRAGMA_WARNING_PUSH < 0
#define TPP_EXT_PRAGMA_WARNING_PUSH TPP_EXT_PRAGMA_WARNING_PUSH
#ifndef TPP_EXTNAME_PRAGMA_WARNING_PUSH
#define TPP_EXTNAME_PRAGMA_WARNING_PUSH "pragma-warning-push"
#endif /* !TPP_EXTNAME_PRAGMA_WARNING_PUSH */
TPP_EXTENSION(TPP_EXT_PRAGMA_WARNING_PUSH, TPP_EXTNAME_PRAGMA_WARNING_PUSH, TPP_HAVE_PRAGMA_WARNING_PUSH == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_WARNING_PUSH(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_WARNING_PUSH
#else /* TPP_HAVE_PRAGMA_WARNING_PUSH < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_WARNING_PUSH(self) TPP_HAVE_PRAGMA_WARNING_PUSH
#endif /* TPP_HAVE_PRAGMA_WARNING_PUSH >= 0 */
#if TPP_HAVE_PRAGMA_TPP_EXEC < 0
#define TPP_EXT_PRAGMA_TPP_EXEC TPP_EXT_PRAGMA_TPP_EXEC
#ifndef TPP_EXTNAME_PRAGMA_TPP_EXEC
#define TPP_EXTNAME_PRAGMA_TPP_EXEC "pragma-tpp_exec"
#endif /* !TPP_EXTNAME_PRAGMA_TPP_EXEC */
TPP_EXTENSION(TPP_EXT_PRAGMA_TPP_EXEC, TPP_EXTNAME_PRAGMA_TPP_EXEC, TPP_HAVE_PRAGMA_TPP_EXEC == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_TPP_EXEC(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_TPP_EXEC
#else /* TPP_HAVE_PRAGMA_TPP_EXEC < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_TPP_EXEC(self) TPP_HAVE_PRAGMA_TPP_EXEC
#endif /* TPP_HAVE_PRAGMA_TPP_EXEC >= 0 */
#if TPP_HAVE_PRAGMA_MESSAGE < 0
#define TPP_EXT_PRAGMA_MESSAGE TPP_EXT_PRAGMA_MESSAGE
#ifndef TPP_EXTNAME_PRAGMA_MESSAGE
#define TPP_EXTNAME_PRAGMA_MESSAGE "pragma-message"
#endif /* !TPP_EXTNAME_PRAGMA_MESSAGE */
TPP_EXTENSION(TPP_EXT_PRAGMA_MESSAGE, TPP_EXTNAME_PRAGMA_MESSAGE, TPP_HAVE_PRAGMA_MESSAGE == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_MESSAGE(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_MESSAGE
#else /* TPP_HAVE_PRAGMA_MESSAGE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_MESSAGE(self) TPP_HAVE_PRAGMA_MESSAGE
#endif /* TPP_HAVE_PRAGMA_MESSAGE >= 0 */
#if TPP_HAVE_PRAGMA_ERROR < 0
#define TPP_EXT_PRAGMA_ERROR TPP_EXT_PRAGMA_ERROR
#ifndef TPP_EXTNAME_PRAGMA_ERROR
#define TPP_EXTNAME_PRAGMA_ERROR "pragma-error"
#endif /* !TPP_EXTNAME_PRAGMA_ERROR */
TPP_EXTENSION(TPP_EXT_PRAGMA_ERROR, TPP_EXTNAME_PRAGMA_ERROR, TPP_HAVE_PRAGMA_ERROR == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_ERROR(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_ERROR
#else /* TPP_HAVE_PRAGMA_ERROR < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_ERROR(self) TPP_HAVE_PRAGMA_ERROR
#endif /* TPP_HAVE_PRAGMA_ERROR >= 0 */
#if TPP_HAVE_PRAGMA_REGION < 0
#define TPP_EXT_PRAGMA_REGION TPP_EXT_PRAGMA_REGION
#ifndef TPP_EXTNAME_PRAGMA_REGION
#define TPP_EXTNAME_PRAGMA_REGION "pragma-region"
#endif /* !TPP_EXTNAME_PRAGMA_REGION */
TPP_EXTENSION(TPP_EXT_PRAGMA_REGION, TPP_EXTNAME_PRAGMA_REGION, TPP_HAVE_PRAGMA_REGION == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_REGION(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_REGION
#else /* TPP_HAVE_PRAGMA_REGION < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_REGION(self) TPP_HAVE_PRAGMA_REGION
#endif /* TPP_HAVE_PRAGMA_REGION >= 0 */
#if TPP_HAVE_PRAGMA_GCC_POISON < 0
#define TPP_EXT_PRAGMA_GCC_POISON TPP_EXT_PRAGMA_GCC_POISON
#ifndef TPP_EXTNAME_PRAGMA_GCC_POISON
#define TPP_EXTNAME_PRAGMA_GCC_POISON "pragma-gcc-poison"
#endif /* !TPP_EXTNAME_PRAGMA_GCC_POISON */
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_POISON, TPP_EXTNAME_PRAGMA_GCC_POISON, TPP_HAVE_PRAGMA_GCC_POISON == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_GCC_POISON(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_GCC_POISON
#else /* TPP_HAVE_PRAGMA_GCC_POISON < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_GCC_POISON(self) TPP_HAVE_PRAGMA_GCC_POISON
#endif /* TPP_HAVE_PRAGMA_GCC_POISON >= 0 */
#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER < 0
#define TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER
#ifndef TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER
#define TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER "pragma-gcc-system-header"
#endif /* !TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER */
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER, TPP_EXTNAME_PRAGMA_GCC_SYSTEM_HEADER, TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER
#else /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER(self) TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#endif /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER >= 0 */
#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH < 0
#define TPP_EXT_PRAGMA_TPP_INCLUDE_PATH TPP_EXT_PRAGMA_TPP_INCLUDE_PATH
#ifndef TPP_EXTNAME_PRAGMA_TPP_INCLUDE_PATH
#define TPP_EXTNAME_PRAGMA_TPP_INCLUDE_PATH "pragma-tpp-include-path"
#endif /* !TPP_EXTNAME_PRAGMA_TPP_INCLUDE_PATH */
TPP_EXTENSION(TPP_EXT_PRAGMA_TPP_INCLUDE_PATH, TPP_EXTNAME_PRAGMA_TPP_INCLUDE_PATH, TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_TPP_INCLUDE_PATH(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_TPP_INCLUDE_PATH
#else /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_TPP_INCLUDE_PATH(self) TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH >= 0 */
#if TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR < 0
#define TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR
#ifndef TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR
#define TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR "dont-expand-defined"
#endif /* !TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR */
TPP_EXTENSION(TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR, TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR, TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR == -1)
#define _tpp_extensions_state_get_TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR(self) (self)->tes_flags.tef_TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR
#else /* TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR < 0 */
#define _tpp_extensions_state_get_TPP_EXT_DONT_EXPAND_DEFINED_IN_EXPR(self) TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR
#endif /* TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR >= 0 */
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
	 TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE ||         \
	 TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST || \
	 TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME ||           \
	 TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT ||             \
	 TPP_HAVE_TPP_W_EXPECTED_STRING ||                          \
	 TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF)
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

#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
#define TPP_W_EOF_BEFORE_ENDIF TPP_W_EOF_BEFORE_ENDIF
TPP_WARNING(TPP_W_EOF_BEFORE_ENDIF, 1(TPP_WG_SYNTAX), 0(/*TODO*/), TPP_WSTATE_UNDEFINED,
            "unterminated %[#if%]-directive")
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */



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
#if TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR < 0
TPP_WARNING(TPP_W_EXPANSION_TO_DEFINED, 1(TPP_WG_EXPANSION_TO_DEFINED), 0(), TPP_WSTATE_UNDEFINED,
            "using %[defined%] on macro argument %Pt will not work, because macros are expanded "
            /**/ "before %[defined%] will seem them. If you want to prevent expansion in this "
            /**/ "specific case, enable %[#pragma extension(\"-f" TPP_EXTNAME_DONT_EXPAND_DEFINED_IN_EXPR "\")%]")
#else /* TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR < 0 */
TPP_WARNING(TPP_W_EXPANSION_TO_DEFINED, 1(TPP_WG_EXPANSION_TO_DEFINED), 0(), TPP_WSTATE_UNDEFINED,
            "using %[defined%] on macro argument %Pt will not work, because macros are expanded "
            /**/ "before %[defined%] will seem them.")
#endif /* TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR >= 0 */
#endif /* TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED */


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
	tpp_keyword const *keyword = va_arg(args, tpp_keyword const *);
	tpp_macro const *const old_definition = keyword->tk_macro;
	tpp_warnf("macro %[%s%] redefined\n", keyword->tk_kwd);
	if (old_definition->tm_deffile) {
		/* TODO: must use "TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT" */
		tpp_warnf("%s(%d, %d): note: see previous definition\n",
		          old_definition->tm_deffile,
		          (int)tpp_lcinfo_getline(old_definition->tm_deflc) + 1,
		          (int)tpp_lcinfo_getcol(old_definition->tm_deflc) + 1);
	}
})
#endif /* TPP_HAVE_TPP_W_REDEFINE_MACRO */



TPP_WGROUP(TPP_WG_USAGE, /*          */ 1("usage"),                TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_BOOLVALUE, /*      */ 1("boolean-value"),        TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_ENVIRON, /*        */ 1("environ"),              TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_LIMIT, /*          */ 1("limit"),                TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_UNDEF, /*          */ 1("undef"),                TPP_WSTATE_WARN)
TPP_WGROUP(TPP_WG_QUALITY, /*        */ 1("quality"),              TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_DEPENDENCY, /*     */ 1("dependency"),           TPP_WSTATE_WARN)


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
