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
#ifdef TPP_DEFS
/************************************************************************/
/* File: parts/defs.h                                                   */
/************************************************************************/
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
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format)
#endif /* !TPP_WARNING */
#ifndef TPP_WARNING_EX
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, expr) \
	TPP_WARNING(warning_id, wgroup_ids, numbers, ~)
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
#if TPP_HAVE_CPP_IDENT
#define TPP_KWD_ident TPP_KWD_ident
TPP_KWD(TPP_KWD_ident, "ident")
#endif /* TPP_HAVE_CPP_IDENT */
#if TPP_HAVE_CPP_SCSS
#define TPP_KWD_scss TPP_KWD_scss
TPP_KWD(TPP_KWD_scss, "scss")
#endif /* TPP_HAVE_CPP_SCSS */
#if TPP_HAVE_CPP_PRAGMA
#define TPP_KWD_pragma TPP_KWD_pragma
TPP_KWD(TPP_KWD_pragma, "pragma")
#endif /* TPP_HAVE_CPP_PRAGMA */
#if TPP_HAVE_STDC_PRAGMA
#define TPP_KWD__Pragma TPP_KWD__Pragma
TPP_KWD(TPP_KWD__Pragma, "_Pragma")
#endif /* TPP_HAVE_STDC_PRAGMA */
#if TPP_HAVE_MSVC_PRAGMA
#define TPP_KWD___pragma TPP_KWD___pragma
TPP_KWD(TPP_KWD___pragma, "__pragma")
#endif /* TPP_HAVE_MSVC_PRAGMA */
/*[[[end]]]*/



/*[[[deemon
for (local feat: {
	"TRIGRAPHS",
	"DIGRAPHS",
	"BSE",
	"BSE_WHITESPACE",
	"CLANG_HAS_ATTRIBUTE",
	"CLANG_HAS_BUILTIN",
	"CLANG_HAS_CPP_ATTRIBUTE",
	"CLANG_HAS_DECLSPEC_ATTRIBUTE",
	"CLANG_HAS_EXTENSION",
	"CLANG_HAS_FEATURE",
	"ALTERNATIVE_MACRO_PARENTHESIS",
	"MACRO_ARGUMENT_WHITESPACE",
	"MACRO_RECURSION",
	"NAMED_VARARGS_IN_MACROS",
	"VA_ARGS_IN_MACROS",
	"VA_COMMA_IN_MACROS",
	"VA_OPT_IN_MACROS",
	"VA_NARGS_IN_MACROS",
	"PRAGMA_PUSH_MACRO",
	"PRAGMA_ONCE",
	"PRAGMA_DEPRECATED",
	"PRAGMA_GCC_POISON",
	"PRAGMA_TPP_SET_KEYWORD_FLAGS",
	"PRAGMA_GCC_SYSTEM_HEADER",
	"PRAGMA_EXTENSION",
	"PRAGMA_EXTENSION_PUSH",
}) {
	print("#if TPP_HAVE_", feat, " < 0");
	local name = feat.lower().replace("_", "-");
	print("TPP_EXTENSION(TPP_EXT_", feat, ", ", repr(name), ", TPP_HAVE_", feat, " == -1)");
	print("#define _tpp_extensions_state_get_TPP_EXT_", feat, "(self) (self)->tes_flags.tef_TPP_EXT_", feat);
	print("#else /" "* TPP_HAVE_", feat, " < 0 *" "/");
	print("#define _tpp_extensions_state_get_TPP_EXT_", feat, "(self) TPP_HAVE_", feat);
	print("#endif /" "* TPP_HAVE_", feat, " >= 0 *" "/");
}
]]]*/
#if TPP_HAVE_TRIGRAPHS < 0
TPP_EXTENSION(TPP_EXT_TRIGRAPHS, "trigraphs", TPP_HAVE_TRIGRAPHS == -1)
#define _tpp_extensions_state_get_TPP_EXT_TRIGRAPHS(self) (self)->tes_flags.tef_TPP_EXT_TRIGRAPHS
#else /* TPP_HAVE_TRIGRAPHS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_TRIGRAPHS(self) TPP_HAVE_TRIGRAPHS
#endif /* TPP_HAVE_TRIGRAPHS >= 0 */
#if TPP_HAVE_DIGRAPHS < 0
TPP_EXTENSION(TPP_EXT_DIGRAPHS, "digraphs", TPP_HAVE_DIGRAPHS == -1)
#define _tpp_extensions_state_get_TPP_EXT_DIGRAPHS(self) (self)->tes_flags.tef_TPP_EXT_DIGRAPHS
#else /* TPP_HAVE_DIGRAPHS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_DIGRAPHS(self) TPP_HAVE_DIGRAPHS
#endif /* TPP_HAVE_DIGRAPHS >= 0 */
#if TPP_HAVE_BSE < 0
TPP_EXTENSION(TPP_EXT_BSE, "bse", TPP_HAVE_BSE == -1)
#define _tpp_extensions_state_get_TPP_EXT_BSE(self) (self)->tes_flags.tef_TPP_EXT_BSE
#else /* TPP_HAVE_BSE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_BSE(self) TPP_HAVE_BSE
#endif /* TPP_HAVE_BSE >= 0 */
#if TPP_HAVE_BSE_WHITESPACE < 0
TPP_EXTENSION(TPP_EXT_BSE_WHITESPACE, "bse-whitespace", TPP_HAVE_BSE_WHITESPACE == -1)
#define _tpp_extensions_state_get_TPP_EXT_BSE_WHITESPACE(self) (self)->tes_flags.tef_TPP_EXT_BSE_WHITESPACE
#else /* TPP_HAVE_BSE_WHITESPACE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_BSE_WHITESPACE(self) TPP_HAVE_BSE_WHITESPACE
#endif /* TPP_HAVE_BSE_WHITESPACE >= 0 */
#if TPP_HAVE_CLANG_HAS_ATTRIBUTE < 0
TPP_EXTENSION(TPP_EXT_CLANG_HAS_ATTRIBUTE, "clang-has-attribute", TPP_HAVE_CLANG_HAS_ATTRIBUTE == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_ATTRIBUTE(self) (self)->tes_flags.tef_TPP_EXT_CLANG_HAS_ATTRIBUTE
#else /* TPP_HAVE_CLANG_HAS_ATTRIBUTE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_ATTRIBUTE(self) TPP_HAVE_CLANG_HAS_ATTRIBUTE
#endif /* TPP_HAVE_CLANG_HAS_ATTRIBUTE >= 0 */
#if TPP_HAVE_CLANG_HAS_BUILTIN < 0
TPP_EXTENSION(TPP_EXT_CLANG_HAS_BUILTIN, "clang-has-builtin", TPP_HAVE_CLANG_HAS_BUILTIN == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_BUILTIN(self) (self)->tes_flags.tef_TPP_EXT_CLANG_HAS_BUILTIN
#else /* TPP_HAVE_CLANG_HAS_BUILTIN < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_BUILTIN(self) TPP_HAVE_CLANG_HAS_BUILTIN
#endif /* TPP_HAVE_CLANG_HAS_BUILTIN >= 0 */
#if TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE < 0
TPP_EXTENSION(TPP_EXT_CLANG_HAS_CPP_ATTRIBUTE, "clang-has-cpp-attribute", TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_CPP_ATTRIBUTE(self) (self)->tes_flags.tef_TPP_EXT_CLANG_HAS_CPP_ATTRIBUTE
#else /* TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_CPP_ATTRIBUTE(self) TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE
#endif /* TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE >= 0 */
#if TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE < 0
TPP_EXTENSION(TPP_EXT_CLANG_HAS_DECLSPEC_ATTRIBUTE, "clang-has-declspec-attribute", TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_DECLSPEC_ATTRIBUTE(self) (self)->tes_flags.tef_TPP_EXT_CLANG_HAS_DECLSPEC_ATTRIBUTE
#else /* TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_DECLSPEC_ATTRIBUTE(self) TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE
#endif /* TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE >= 0 */
#if TPP_HAVE_CLANG_HAS_EXTENSION < 0
TPP_EXTENSION(TPP_EXT_CLANG_HAS_EXTENSION, "clang-has-extension", TPP_HAVE_CLANG_HAS_EXTENSION == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_EXTENSION(self) (self)->tes_flags.tef_TPP_EXT_CLANG_HAS_EXTENSION
#else /* TPP_HAVE_CLANG_HAS_EXTENSION < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_EXTENSION(self) TPP_HAVE_CLANG_HAS_EXTENSION
#endif /* TPP_HAVE_CLANG_HAS_EXTENSION >= 0 */
#if TPP_HAVE_CLANG_HAS_FEATURE < 0
TPP_EXTENSION(TPP_EXT_CLANG_HAS_FEATURE, "clang-has-feature", TPP_HAVE_CLANG_HAS_FEATURE == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_FEATURE(self) (self)->tes_flags.tef_TPP_EXT_CLANG_HAS_FEATURE
#else /* TPP_HAVE_CLANG_HAS_FEATURE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_FEATURE(self) TPP_HAVE_CLANG_HAS_FEATURE
#endif /* TPP_HAVE_CLANG_HAS_FEATURE >= 0 */
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS < 0
TPP_EXTENSION(TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS, "alternative-macro-parenthesis", TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS == -1)
#define _tpp_extensions_state_get_TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS(self) (self)->tes_flags.tef_TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS
#else /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS(self) TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS >= 0 */
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
TPP_EXTENSION(TPP_EXT_MACRO_ARGUMENT_WHITESPACE, "macro-argument-whitespace", TPP_HAVE_MACRO_ARGUMENT_WHITESPACE == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO_ARGUMENT_WHITESPACE(self) (self)->tes_flags.tef_TPP_EXT_MACRO_ARGUMENT_WHITESPACE
#else /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO_ARGUMENT_WHITESPACE(self) TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE >= 0 */
#if TPP_HAVE_MACRO_RECURSION < 0
TPP_EXTENSION(TPP_EXT_MACRO_RECURSION, "macro-recursion", TPP_HAVE_MACRO_RECURSION == -1)
#define _tpp_extensions_state_get_TPP_EXT_MACRO_RECURSION(self) (self)->tes_flags.tef_TPP_EXT_MACRO_RECURSION
#else /* TPP_HAVE_MACRO_RECURSION < 0 */
#define _tpp_extensions_state_get_TPP_EXT_MACRO_RECURSION(self) TPP_HAVE_MACRO_RECURSION
#endif /* TPP_HAVE_MACRO_RECURSION >= 0 */
#if TPP_HAVE_NAMED_VARARGS_IN_MACROS < 0
TPP_EXTENSION(TPP_EXT_NAMED_VARARGS_IN_MACROS, "named-varargs-in-macros", TPP_HAVE_NAMED_VARARGS_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_NAMED_VARARGS_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_NAMED_VARARGS_IN_MACROS
#else /* TPP_HAVE_NAMED_VARARGS_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_NAMED_VARARGS_IN_MACROS(self) TPP_HAVE_NAMED_VARARGS_IN_MACROS
#endif /* TPP_HAVE_NAMED_VARARGS_IN_MACROS >= 0 */
#if TPP_HAVE_VA_ARGS_IN_MACROS < 0
TPP_EXTENSION(TPP_EXT_VA_ARGS_IN_MACROS, "va-args-in-macros", TPP_HAVE_VA_ARGS_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_VA_ARGS_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_VA_ARGS_IN_MACROS
#else /* TPP_HAVE_VA_ARGS_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_VA_ARGS_IN_MACROS(self) TPP_HAVE_VA_ARGS_IN_MACROS
#endif /* TPP_HAVE_VA_ARGS_IN_MACROS >= 0 */
#if TPP_HAVE_VA_COMMA_IN_MACROS < 0
TPP_EXTENSION(TPP_EXT_VA_COMMA_IN_MACROS, "va-comma-in-macros", TPP_HAVE_VA_COMMA_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_VA_COMMA_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_VA_COMMA_IN_MACROS
#else /* TPP_HAVE_VA_COMMA_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_VA_COMMA_IN_MACROS(self) TPP_HAVE_VA_COMMA_IN_MACROS
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS >= 0 */
#if TPP_HAVE_VA_OPT_IN_MACROS < 0
TPP_EXTENSION(TPP_EXT_VA_OPT_IN_MACROS, "va-opt-in-macros", TPP_HAVE_VA_OPT_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_VA_OPT_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_VA_OPT_IN_MACROS
#else /* TPP_HAVE_VA_OPT_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_VA_OPT_IN_MACROS(self) TPP_HAVE_VA_OPT_IN_MACROS
#endif /* TPP_HAVE_VA_OPT_IN_MACROS >= 0 */
#if TPP_HAVE_VA_NARGS_IN_MACROS < 0
TPP_EXTENSION(TPP_EXT_VA_NARGS_IN_MACROS, "va-nargs-in-macros", TPP_HAVE_VA_NARGS_IN_MACROS == -1)
#define _tpp_extensions_state_get_TPP_EXT_VA_NARGS_IN_MACROS(self) (self)->tes_flags.tef_TPP_EXT_VA_NARGS_IN_MACROS
#else /* TPP_HAVE_VA_NARGS_IN_MACROS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_VA_NARGS_IN_MACROS(self) TPP_HAVE_VA_NARGS_IN_MACROS
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS >= 0 */
#if TPP_HAVE_PRAGMA_PUSH_MACRO < 0
TPP_EXTENSION(TPP_EXT_PRAGMA_PUSH_MACRO, "pragma-push-macro", TPP_HAVE_PRAGMA_PUSH_MACRO == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_PUSH_MACRO(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_PUSH_MACRO
#else /* TPP_HAVE_PRAGMA_PUSH_MACRO < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_PUSH_MACRO(self) TPP_HAVE_PRAGMA_PUSH_MACRO
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO >= 0 */
#if TPP_HAVE_PRAGMA_ONCE < 0
TPP_EXTENSION(TPP_EXT_PRAGMA_ONCE, "pragma-once", TPP_HAVE_PRAGMA_ONCE == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_ONCE(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_ONCE
#else /* TPP_HAVE_PRAGMA_ONCE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_ONCE(self) TPP_HAVE_PRAGMA_ONCE
#endif /* TPP_HAVE_PRAGMA_ONCE >= 0 */
#if TPP_HAVE_PRAGMA_DEPRECATED < 0
TPP_EXTENSION(TPP_EXT_PRAGMA_DEPRECATED, "pragma-deprecated", TPP_HAVE_PRAGMA_DEPRECATED == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_DEPRECATED(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_DEPRECATED
#else /* TPP_HAVE_PRAGMA_DEPRECATED < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_DEPRECATED(self) TPP_HAVE_PRAGMA_DEPRECATED
#endif /* TPP_HAVE_PRAGMA_DEPRECATED >= 0 */
#if TPP_HAVE_PRAGMA_GCC_POISON < 0
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_POISON, "pragma-gcc-poison", TPP_HAVE_PRAGMA_GCC_POISON == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_GCC_POISON(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_GCC_POISON
#else /* TPP_HAVE_PRAGMA_GCC_POISON < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_GCC_POISON(self) TPP_HAVE_PRAGMA_GCC_POISON
#endif /* TPP_HAVE_PRAGMA_GCC_POISON >= 0 */
#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS < 0
TPP_EXTENSION(TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS, "pragma-tpp-set-keyword-flags", TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS
#else /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_TPP_SET_KEYWORD_FLAGS(self) TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS >= 0 */
#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER < 0
TPP_EXTENSION(TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER, "pragma-gcc-system-header", TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER
#else /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_GCC_SYSTEM_HEADER(self) TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#endif /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER >= 0 */
#if TPP_HAVE_PRAGMA_EXTENSION < 0
TPP_EXTENSION(TPP_EXT_PRAGMA_EXTENSION, "pragma-extension", TPP_HAVE_PRAGMA_EXTENSION == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_EXTENSION(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_EXTENSION
#else /* TPP_HAVE_PRAGMA_EXTENSION < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_EXTENSION(self) TPP_HAVE_PRAGMA_EXTENSION
#endif /* TPP_HAVE_PRAGMA_EXTENSION >= 0 */
#if TPP_HAVE_PRAGMA_EXTENSION_PUSH < 0
TPP_EXTENSION(TPP_EXT_PRAGMA_EXTENSION_PUSH, "pragma-extension-push", TPP_HAVE_PRAGMA_EXTENSION_PUSH == -1)
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_EXTENSION_PUSH(self) (self)->tes_flags.tef_TPP_EXT_PRAGMA_EXTENSION_PUSH
#else /* TPP_HAVE_PRAGMA_EXTENSION_PUSH < 0 */
#define _tpp_extensions_state_get_TPP_EXT_PRAGMA_EXTENSION_PUSH(self) TPP_HAVE_PRAGMA_EXTENSION_PUSH
#endif /* TPP_HAVE_PRAGMA_EXTENSION_PUSH >= 0 */


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
TPP_WARNING(TPP_W_SLASHSTAR_INSIDE_OF_COMMENT, 1(TPP_WG_COMMENT), 0(), "%[/*%] repeated inside of comment")
#endif /* TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT */

#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
#define TPP_W_LINE_COMMENT_CONTINUED TPP_W_LINE_COMMENT_CONTINUED
TPP_WARNING(TPP_W_LINE_COMMENT_CONTINUED, 1(TPP_WG_COMMENT), 1(4010), "Line-comment continued")
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
TPP_WARNING(TPP_W_ENCOUNTERED_TRIGRAPH, 1(TPP_WG_TRIGRAPHS), 0(), "Encountered trigraph character sequence %.3Pt")
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */


/************************************************************************/
/* -Wsyntax                                                             */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_SYNTAX
#define TPP_HAVE_TPP_WG_SYNTAX                       \
	(TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED || \
	 TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF)
#endif /* !TPP_HAVE_TPP_WG_SYNTAX */
#if TPP_HAVE_TPP_WG_SYNTAX
#define TPP_WG_SYNTAX TPP_WG_SYNTAX
TPP_WGROUP(TPP_WG_SYNTAX, 1("syntax"), TPP_WSTATE_ERROR_OR_FATAL)
#endif /* TPP_HAVE_TPP_WG_SYNTAX */

#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
#define TPP_W_STRING_TERMINATED_BY_LINEFEED TPP_W_STRING_TERMINATED_BY_LINEFEED
TPP_WARNING(TPP_W_STRING_TERMINATED_BY_LINEFEED, 1(TPP_WG_SYNTAX), 0(), "String was terminated by a linefeed")
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */

#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#define TPP_W_STRING_TERMINATED_BY_EOF TPP_W_STRING_TERMINATED_BY_EOF
TPP_WARNING(TPP_W_STRING_TERMINATED_BY_EOF, 1(TPP_WG_SYNTAX), 0(), "String was terminated by EOF")
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */

#if TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
#define TPP_W_COMMENT_TERMINATED_BY_EOF TPP_W_COMMENT_TERMINATED_BY_EOF
TPP_WARNING(TPP_W_COMMENT_TERMINATED_BY_EOF, 1(TPP_WG_SYNTAX), 0(), "Comment was terminated by EOF")
#endif /* TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */



TPP_WGROUP(TPP_WG_MACROS, /*         */ 1("macros"),               TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_USAGE, /*          */ 1("usage"),                TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_BOOLVALUE, /*      */ 1("boolean-value"),        TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_USER, /*           */ 1("user"),                 TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_ENVIRON, /*        */ 1("environ"),              TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_LIMIT, /*          */ 1("limit"),                TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_UNDEF, /*          */ 1("undef"),                TPP_WSTATE_WARN)
TPP_WGROUP(TPP_WG_EXPANSION_TO_DEFINED, 1("expansion-to-defined"), TPP_WSTATE_WARN)
TPP_WGROUP(TPP_WG_QUALITY, /*        */ 1("quality"),              TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_DEPRECATED, /*     */ 1("deprecated"),           TPP_WSTATE_FATAL)
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
/************************************************************************/

#else /* TPP_DEFS */
#ifndef GUARD_TPP_H
#define GUARD_TPP_H 1

/************************************************************************/
/* File: parts/api.h                                                    */
/************************************************************************/
#ifndef TPP_BUILDING
#define TPP_BUILDING 0
#endif /* !TPP_BUILDING */

#if TPP_BUILDING
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_OBSOLETE_NO_DEPRECATE

/* Disable some warnings */
#pragma warning(disable: 4065) /* "switch statement contains 'default' but no 'case' labels" */

/* Disable some garbage inspection warnings */
#pragma warning(disable: 26446)
#pragma warning(disable: 26482)
#pragma warning(disable: 26438)
#pragma warning(disable: 26494)
#pragma warning(disable: 26496)
#pragma warning(disable: 26485)
#pragma warning(disable: 26448)
#endif /* _MSC_VER */
#endif /* TPP_BUILDING */

#ifndef TPP_NO_SYSTEM_INCLUDES
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#endif /* !TPP_NO_SYSTEM_INCLUDES */

#ifndef __SIZEOF_POINTER__
#ifdef __has_include
#if __has_include(<hybrid/typecore.h>)
#include <hybrid/typecore.h>
#endif /* __has_include(<hybrid/typecore.h>) */
#endif /* __has_include */
#ifndef __SIZEOF_POINTER__
#ifdef SIZE_MAX
#if SIZE_MAX == UINT32_C(0xffffffff)
#define __SIZEOF_POINTER__ 4
#elif SIZE_MAX == UINT64_C(0xffffffffffffffff)
#define __SIZEOF_POINTER__ 8
#elif SIZE_MAX == UINT16_C(0xffff)
#define __SIZEOF_POINTER__ 2
#elif SIZE_MAX == UINT8_C(0xff)
#define __SIZEOF_POINTER__ 1
#endif /* ... */
#endif /* SIZE_MAX */
#ifndef __SIZEOF_POINTER__
#if defined(_WIN64) || defined(WIN64)
#define __SIZEOF_POINTER__ 8
#elif defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
#define __SIZEOF_POINTER__ 4
#endif /* !_WIN32 && __WIN32__ */
#if !defined(__SIZEOF_POINTER__) && !defined(__DEEMON__)
#error "No way to determine '__SIZEOF_POINTER__'"
#endif /* !__SIZEOF_POINTER__ */
#endif /* !__SIZEOF_POINTER__ */
#endif /* !__SIZEOF_POINTER__ */
#endif /* !__SIZEOF_POINTER__ */

#ifndef __SIZEOF_SIZE_T__
#define __SIZEOF_SIZE_T__ __SIZEOF_POINTER__
#endif /* !__SIZEOF_SIZE_T__ */

#ifndef __SIZEOF_INT__
#ifdef __has_include
#if __has_include(<hybrid/limitcore.h>)
#include <hybrid/limitcore.h>
#endif /* __has_include(<hybrid/limitcore.h>) */
#if __has_include(<limits.h>)
#include <limits.h>
#endif /* __has_include(<limits.h>) */
#endif /* __has_include */
#ifndef __SIZEOF_INT__
#ifdef INT_MAX
#if INT_MAX == 127
#define __SIZEOF_INT__ 1
#elif INT_MAX == 32767
#define __SIZEOF_INT__ 2
#elif INT_MAX == 2147483647
#define __SIZEOF_INT__ 4
#elif INT_MAX == 9223372036854775807
#define __SIZEOF_INT__ 8
#endif /* ... */
#endif /* INT_MAX */
#ifndef __SIZEOF_INT__
#define __SIZEOF_INT__ 4
#endif /* !__SIZEOF_INT__ */
#endif /* !__SIZEOF_INT__ */
#endif /* !__SIZEOF_INT__ */

/* The standard calling convention used by TPP APIs */
#ifndef TPPCALL
#define TPPCALL /* nothing */
#endif /* !TPPCALL */

/* The standard calling convention used by TPP APIs for variadic functions. */
#ifndef TPPVCALL
#define TPPVCALL /* nothing */
#endif /* !TPPVCALL */

#ifndef TPP_DECL
#define TPP_DECL extern
#endif /* !TPP_DECL */
#ifndef TPP_IMPL
#define TPP_IMPL /* nothing */
#endif /* !TPP_IMPL */
#ifndef TPP_CONST_DECL
#define TPP_CONST_DECL TPP_DECL
#endif /* !TPP_CONST_DECL */
#ifndef TPP_CONST_IMPL
#define TPP_CONST_IMPL TPP_IMPL
#endif /* !TPP_CONST_IMPL */
#ifndef TPP_INTERN_DECL
#define TPP_INTERN_DECL extern
#endif /* !TPP_INTERN_DECL */
#ifndef TPP_INTERN_IMPL
#define TPP_INTERN_IMPL /* nothing */
#endif /* !TPP_INTERN_IMPL */
#ifndef tpp_restrict
#define tpp_restrict __restrict
#endif /* !tpp_restrict */
#ifndef TPP_NONNULL
#define TPP_NONNULL(x) /* nothing */
#endif /* !TPP_NONNULL */
#ifndef TPP_WUNUSED
#define TPP_WUNUSED /* nothing */
#endif /* !TPP_WUNUSED */
#ifndef TPP_RETNONNULL
#define TPP_RETNONNULL /* nothing */
#endif /* !TPP_RETNONNULL */
#ifndef TPP_NOINLINE
#ifdef _MSC_VER
#define TPP_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define TPP_NOINLINE __attribute__((__noinline__))
#else /* ... */
#define TPP_NOINLINE /* nothing */
#endif /* !... */
#endif /* !TPP_NOINLINE */
#ifndef TPP_FLEX_ARRAY
#define TPP_FLEX_ARRAY 4096
#endif /* !TPP_FLEX_ARRAY */

#ifndef TPP_CHAR_BIT
#ifdef CHAR_BIT
#define TPP_CHAR_BIT CHAR_BIT
#elif defined(__CHAR_BIT__)
#define TPP_CHAR_BIT __CHAR_BIT__
#else /* ... */
#define TPP_CHAR_BIT 8
#endif /* !... */
#endif /* !TPP_CHAR_BIT */


#ifndef TPP_DEBUG
#ifdef NDEBUG
#define TPP_DEBUG 0
#else /* NDEBUG */
#define TPP_DEBUG 1
#endif /* !NDEBUG */
#endif /* !TPP_DEBUG */

#ifndef TPP_INLINE
#if defined(__cplusplus) || defined(inline)
#define TPP_INLINE static inline
#elif defined(_MSC_VER)
#define TPP_INLINE static __inline
#elif defined(__GNUC__) || defined(__TCC__) || defined(__DCC_VERSION__)
#define TPP_INLINE static __inline__
#else /* ... */
#define TPP_INLINE static
#endif /* !... */
#endif /* !TPP_INLINE */

#ifdef __cplusplus
#define TPP_DECL_BEGIN extern "C" {
#define TPP_DECL_END   }
#else /* __cplusplus */
#define TPP_DECL_BEGIN /* nothing */
#define TPP_DECL_END   /* nothing */
#endif /* !__cplusplus */

#ifndef tpp_offsetof
#include <stddef.h>
#define tpp_offsetof offsetof
#endif /* !tpp_offsetof */

#ifndef tpp_lengthof
#define tpp_lengthof(a) (sizeof(a) / sizeof(*(a)))
#endif /* !tpp_lengthof */

#ifndef tpp_unreachable
#ifdef _MSC_VER
#define tpp_unreachable() __assume(0)
#else /* _MSC_VER */
#define tpp_unreachable() __builtin_unreachable()
#endif /* !_MSC_VER */
#endif /* !tpp_unreachable */

#ifndef tpp_memcpy
#include <string.h>
#define tpp_strlen      strlen
#define tpp_strcmp      strcmp
#define tpp_memcmp      memcmp
#define tpp_memcpy      memcpy
#define tpp_memset      memset
#define tpp_memmove     memmove
#define tpp_memmoveup   memmove
#define tpp_memmovedown memmove
#endif /* !tpp_memcpy */

#ifndef tpp_expect
#define tpp_expect(expr, expected) expr
#define tpp_expect_IS_NOOP
#endif /* !tpp_expect */

#ifndef tpp_likely
#ifdef tpp_expect_IS_NOOP
#define tpp_likely   /* nothing */
#define tpp_unlikely /* nothing */
#else /* tpp_expect_IS_NOOP */
#define tpp_likely(expr)   tpp_expect(!!(expr), 1)
#define tpp_unlikely(expr) tpp_expect(!!(expr), 0)
#endif /* !tpp_expect_IS_NOOP */
#endif /* !tpp_unlikely */

#ifndef tpp_size
#define TPP_SIZEOF_tpp_size __SIZEOF_SIZE_T__
#define tpp_size size_t
#endif /* !tpp_size */
#ifndef tpp_ssize
#define tpp_ssize ptrdiff_t
#endif /* !tpp_ssize */
#ifndef tpp_hash
#if UINT_FAST32_MAX == UINT32_C(0xffffffff)
#define TPP_SIZEOF_tpp_hash 4
#elif UINT_FAST32_MAX == UINT64_C(0xffffffffffffffff)
#define TPP_SIZEOF_tpp_hash 8
#else /* UINT_FAST32_MAX == ... */
#error "Unrecognized 'UINT_FAST32_MAX'"
#endif /* UINT_FAST32_MAX != ... */
#define tpp_hash uint_fast32_t
#endif /* !tpp_hash */
#ifndef tpp_line
#define tpp_line   int_fast32_t
#define tpp_column int_fast32_t
#endif /* !tpp_line */
#ifndef tpp_char
#define tpp_char unsigned char
#endif /* !tpp_char */
#ifndef tpp_unichar
#define tpp_unichar uint_least32_t
#endif /* !tpp_unichar */

#ifndef tpp_intmax
#define tpp_intmax      intmax_t
#define tpp_uintmax     uintmax_t
#define TPP_UINTMAX_MAX UINTMAX_MAX
#define TPP_UINTMAX_C   UINTMAX_C
#endif /* !tpp_intmax */

#ifndef TPP_REF
#define TPP_REF       /* nothing */
#define TPP_REF_IF(c) /* nothing */
#endif /* !TPP_REF */

#ifndef TPP_STATIC_ASSERT
#define _TPP_STATIC_ASSERT_ID2(line) tpp_static_assert_##line
#define _TPP_STATIC_ASSERT_ID(line) _TPP_STATIC_ASSERT_ID2(line)
#define TPP_STATIC_ASSERT(expr) typedef int _TPP_STATIC_ASSERT_ID(__LINE__)[(expr) ? 1 : -1]
#endif /* !TPP_STATIC_ASSERT */

#ifndef tpp_refcnt
/* NOTE: Multi-threaded applications can leave this alone: a single
 *       TPP lexer can only ever be used by a single thread, meaning
 *       that reference counts don't need to be atomic, because all
 *       components are thread-local. */
#define tpp_refcnt             uint_fast32_t
#define tpp_refcnt_inc(p)      (void)(++*(p))
#define tpp_refcnt_decfetch(p) (--*(p))
#define tpp_refcnt_isshared(p) (*(p) > 1)
#endif /* !tpp_refcnt */
#ifndef tpp_refcnt_dec
#define tpp_refcnt_dec(p) (void)tpp_refcnt_decfetch(p)
#endif /* !tpp_refcnt_dec */

#ifndef tpp_once
/* WARNING: Multi-threaded applications must override this */
#define tpp_once(expr)             \
	do {                           \
		static int _to_didrun = 0; \
		if (!_to_didrun) {         \
			_to_didrun = 1;        \
			expr;                  \
		}                          \
	} while (0)

#endif /* !tpp_once */

#ifndef tpp_malloc
#include <stdlib.h>
#define tpp_trymalloc(s)     malloc(s)     /* tpp_trymalloc -- use when failure allows for re-try */
#define tpp_malloc(s)        malloc(s)     /* tpp_malloc    -- use when failure means error-propagation */
#define tpp_tryrealloc(p, s) realloc(p, s)
#define tpp_realloc(p, s)    realloc(p, s)
#define tpp_free(p)          free(p)
#endif /* !tpp_malloc */

TPP_DECL_BEGIN
#ifndef tpp_formatprinter
#define tpp_formatprinter tpp_formatprinter
#define TPP_FORMATPRINTER_CC TPPCALL
typedef tpp_ssize (TPP_FORMATPRINTER_CC *tpp_formatprinter)(void *arg, tpp_char const *text, tpp_size num_bytes);
#define tpp_formatprinter_print(printer, arg, text, num_bytes) \
	((*printer)(arg, text, num_bytes))
#endif /* !tpp_formatprinter */

#ifndef tpp_lcinfo
typedef struct {
	tpp_line   lci_line; /* Line */
	tpp_column lci_col;  /* Column */
} tpp_lcinfo;
#define tpp_lcinfo tpp_lcinfo

#define tpp_lcinfo_getline(self) ((tpp_line)(self).lci_line)
#define tpp_lcinfo_getcol(self)  ((tpp_column)(self).lci_col)
#define tpp_lcinfo_init(self, line, col) \
	(void)((self).lci_line = line, (self).lci_col = col)

TPP_INLINE TPP_WUNUSED tpp_lcinfo TPPCALL
tpp_lcinfo_of(tpp_line line, tpp_column col) {
	tpp_lcinfo result;
	result.lci_line = line;
	result.lci_col  = col;
	return result;
}
#endif /* !tpp_lcinfo */

#ifndef tpp_lcinfo_init
#define tpp_lcinfo_init(self, line, col) \
	(void)((self) = tpp_lcinfo_of(line, col))
#endif /* !tpp_lcinfo_init */

TPP_DECL_END


#ifndef tpp_assert
#include <assert.h>
#define tpp_assert assert
#endif /* !tpp_assert */
/************************************************************************/

/************************************************************************/
/* File: parts/tuple.h                                                  */
/************************************************************************/
/* Preprocessor tuples:
 * >> 0()                  // Empty tuple
 * >> 1(VALUE)             // 1-element tuple
 * >> 2(VALUE1, VALUE2)    // 2-element tuple
 * >> ...
 */

#define _TPP_TUPLE_NE_0() 0
#define _TPP_TUPLE_NE_1(a) 1
#define _TPP_TUPLE_NE_2(a, b) 1
#define _TPP_TUPLE_NE_3(a, b, c) 1
#define _TPP_TUPLE_NE_4(a, b, c, d) 1
#define _TPP_TUPLE_NE_5(a, b, c, d, e) 1
#define _TPP_TUPLE_NE_6(a, b, c, d, e, f) 1
#define _TPP_TUPLE_NE_7(a, b, c, d, e, f, g) 1
#define _TPP_TUPLE_NE_8(a, b, c, d, e, f, g, h) 1
#define _TPP_TUPLE_NE_9(a, b, c, d, e, f, g, h, i) 1
#define _TPP_TUPLE_SIZE_0() 0
#define _TPP_TUPLE_SIZE_1(a) 1
#define _TPP_TUPLE_SIZE_2(a, b) 2
#define _TPP_TUPLE_SIZE_3(a, b, c) 3
#define _TPP_TUPLE_SIZE_4(a, b, c, d) 4
#define _TPP_TUPLE_SIZE_5(a, b, c, d, e) 5
#define _TPP_TUPLE_SIZE_6(a, b, c, d, e, f) 6
#define _TPP_TUPLE_SIZE_7(a, b, c, d, e, f, g) 7
#define _TPP_TUPLE_SIZE_8(a, b, c, d, e, f, g, h) 8
#define _TPP_TUPLE_SIZE_9(a, b, c, d, e, f, g, h, i) 9
#define _TPP_TUPLE_GET_0_1(a) a
#define _TPP_TUPLE_GET_0_2(a, b) a
#define _TPP_TUPLE_GET_1_2(a, b) b
#define _TPP_TUPLE_GET_0_3(a, b, c) a
#define _TPP_TUPLE_GET_1_3(a, b, c) b
#define _TPP_TUPLE_GET_2_3(a, b, c) c
#define _TPP_TUPLE_GET_0_4(a, b, c, d) a
#define _TPP_TUPLE_GET_1_4(a, b, c, d) b
#define _TPP_TUPLE_GET_2_4(a, b, c, d) c
#define _TPP_TUPLE_GET_3_4(a, b, c, d) d
#define _TPP_TUPLE_GET_0_5(a, b, c, d, e) a
#define _TPP_TUPLE_GET_1_5(a, b, c, d, e) b
#define _TPP_TUPLE_GET_2_5(a, b, c, d, e) c
#define _TPP_TUPLE_GET_3_5(a, b, c, d, e) d
#define _TPP_TUPLE_GET_4_5(a, b, c, d, e) e
#define _TPP_TUPLE_GET_0_6(a, b, c, d, e, f) a
#define _TPP_TUPLE_GET_1_6(a, b, c, d, e, f) b
#define _TPP_TUPLE_GET_2_6(a, b, c, d, e, f) c
#define _TPP_TUPLE_GET_3_6(a, b, c, d, e, f) d
#define _TPP_TUPLE_GET_4_6(a, b, c, d, e, f) e
#define _TPP_TUPLE_GET_5_6(a, b, c, d, e, f) f
#define _TPP_TUPLE_GET_0_7(a, b, c, d, e, f, g) a
#define _TPP_TUPLE_GET_1_7(a, b, c, d, e, f, g) b
#define _TPP_TUPLE_GET_2_7(a, b, c, d, e, f, g) c
#define _TPP_TUPLE_GET_3_7(a, b, c, d, e, f, g) d
#define _TPP_TUPLE_GET_4_7(a, b, c, d, e, f, g) e
#define _TPP_TUPLE_GET_5_7(a, b, c, d, e, f, g) f
#define _TPP_TUPLE_GET_6_7(a, b, c, d, e, f, g) g
#define _TPP_TUPLE_GET_0_8(a, b, c, d, e, f, g, h) a
#define _TPP_TUPLE_GET_1_8(a, b, c, d, e, f, g, h) b
#define _TPP_TUPLE_GET_2_8(a, b, c, d, e, f, g, h) c
#define _TPP_TUPLE_GET_3_8(a, b, c, d, e, f, g, h) d
#define _TPP_TUPLE_GET_4_8(a, b, c, d, e, f, g, h) e
#define _TPP_TUPLE_GET_5_8(a, b, c, d, e, f, g, h) f
#define _TPP_TUPLE_GET_6_8(a, b, c, d, e, f, g, h) g
#define _TPP_TUPLE_GET_7_8(a, b, c, d, e, f, g, h) h
#define _TPP_TUPLE_GET_0_9(a, b, c, d, e, f, g, h, i) a
#define _TPP_TUPLE_GET_1_9(a, b, c, d, e, f, g, h, i) b
#define _TPP_TUPLE_GET_2_9(a, b, c, d, e, f, g, h, i) c
#define _TPP_TUPLE_GET_3_9(a, b, c, d, e, f, g, h, i) d
#define _TPP_TUPLE_GET_4_9(a, b, c, d, e, f, g, h, i) e
#define _TPP_TUPLE_GET_5_9(a, b, c, d, e, f, g, h, i) f
#define _TPP_TUPLE_GET_6_9(a, b, c, d, e, f, g, h, i) g
#define _TPP_TUPLE_GET_7_9(a, b, c, d, e, f, g, h, i) h
#define _TPP_TUPLE_GET_8_9(a, b, c, d, e, f, g, h, i) i
#define _TPP_TUPLE_FOREACH_0(tuple, sep, item, _)
#define _TPP_TUPLE_FOREACH_1(tuple, sep, item, _) \
	item(_, 0, TPP_TUPLE_GET(tuple, 0))
#define _TPP_TUPLE_FOREACH_2(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_1(tuple, sep, item, _) \
	sep(_, 0, TPP_TUPLE_GET(tuple, 0), 1, TPP_TUPLE_GET(tuple, 1)) \
	item(_, 1, TPP_TUPLE_GET(tuple, 1))
#define _TPP_TUPLE_FOREACH_3(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_2(tuple, sep, item, _) \
	sep(_, 1, TPP_TUPLE_GET(tuple, 1), 2, TPP_TUPLE_GET(tuple, 2)) \
	item(_, 2, TPP_TUPLE_GET(tuple, 2))
#define _TPP_TUPLE_FOREACH_4(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_3(tuple, sep, item, _) \
	sep(_, 2, TPP_TUPLE_GET(tuple, 2), 3, TPP_TUPLE_GET(tuple, 3)) \
	item(_, 3, TPP_TUPLE_GET(tuple, 3))
#define _TPP_TUPLE_FOREACH_5(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_4(tuple, sep, item, _) \
	sep(_, 3, TPP_TUPLE_GET(tuple, 3), 4, TPP_TUPLE_GET(tuple, 4)) \
	item(_, 4, TPP_TUPLE_GET(tuple, 4))
#define _TPP_TUPLE_FOREACH_6(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_5(tuple, sep, item, _) \
	sep(_, 4, TPP_TUPLE_GET(tuple, 4), 5, TPP_TUPLE_GET(tuple, 5)) \
	item(_, 5, TPP_TUPLE_GET(tuple, 5))
#define _TPP_TUPLE_FOREACH_7(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_6(tuple, sep, item, _) \
	sep(_, 5, TPP_TUPLE_GET(tuple, 5), 6, TPP_TUPLE_GET(tuple, 6)) \
	item(_, 6, TPP_TUPLE_GET(tuple, 6))
#define _TPP_TUPLE_FOREACH_8(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_7(tuple, sep, item, _) \
	sep(_, 6, TPP_TUPLE_GET(tuple, 6), 7, TPP_TUPLE_GET(tuple, 7)) \
	item(_, 7, TPP_TUPLE_GET(tuple, 7))
#define _TPP_TUPLE_FOREACH_9(tuple, sep, item, _) \
	_TPP_TUPLE_FOREACH_8(tuple, sep, item, _) \
	sep(_, 7, TPP_TUPLE_GET(tuple, 7), 8, TPP_TUPLE_GET(tuple, 8)) \
	item(_, 8, TPP_TUPLE_GET(tuple, 8))

/* >> TPP_TUPLE_NONEMPTY(tuple)
 * expands to "0" if "tuple" is empty; otherwise, expands to "1" */
#define TPP_TUPLE_NONEMPTY(tuple)  _TPP_TUPLE_NONEMPTY(tuple)
#define _TPP_TUPLE_NONEMPTY(tuple) _TPP_TUPLE_NONEMPTY_(_TPP_TUPLE_NE_##tuple)
#define _TPP_TUPLE_NONEMPTY_(x)    x

/* >> TPP_TUPLE_IF_NONEMPTY(tuple, what, _)
 * expands to "what(_)" if "tuple" is non-empty; otherwise, expands to nothing */
#define TPP_TUPLE_IF_NONEMPTY(tuple, what, _) _TPP_TUPLE_IF_NONEMPTY(TPP_TUPLE_NONEMPTY(tuple), what, _)
#define _TPP_TUPLE_IF_NONEMPTY(if, what, _)   _TPP_TUPLE_IF_NONEMPTY_(if, what, _)
#define _TPP_TUPLE_IF_NONEMPTY_(if, what, _)  _TPP_TUPLE_IF_NONEMPTY_##if (what, _)
#define _TPP_TUPLE_IF_NONEMPTY_1(what, _)     what(_)
#define _TPP_TUPLE_IF_NONEMPTY_0(what, _)

/* >> TPP_TUPLE_SIZE(tuple)
 * expands to a decimal number describing the size of "tuple" */
#define TPP_TUPLE_SIZE(tuple)  _TPP_TUPLE_SIZE(tuple)
#define _TPP_TUPLE_SIZE(tuple) _TPP_TUPLE_SIZE_(_TPP_TUPLE_SIZE_##tuple)
#define _TPP_TUPLE_SIZE_(x)    x

/* >> TPP_TUPLE_GET(tuple, i)
 * expands to the i'th element of "tuple" */
#define TPP_TUPLE_GET(tuple, i)  _TPP_TUPLE_GET(tuple, i)
#define _TPP_TUPLE_GET(tuple, i) _TPP_TUPLE_GET_(_TPP_TUPLE_GET_##i##_##tuple)
#define _TPP_TUPLE_GET_(x)       x

/* >> TPP_TUPLE_FOREACH(tuple, sep, item, _)
 *    >> sep(_, prev_index, prev_value, next_index, next_value)
 *    >> item(_, index, value)
 * expands to:
 *    - item(_, 0, TPP_TUPLE_GET(tuple, 0))
 *    - sep(_, 0, TPP_TUPLE_GET(tuple, 0), 1, TPP_TUPLE_GET(tuple, 1))
 *    - item(_, 1, TPP_TUPLE_GET(tuple, 1))
 *    - sep(_, 1, TPP_TUPLE_GET(tuple, 1), 2, TPP_TUPLE_GET(tuple, 2))
 *    - item(_, 2, TPP_TUPLE_GET(tuple, 2))
 *    - ...
 *    - item(_, N-1, TPP_TUPLE_GET(tuple, N-1))
 */
#define TPP_TUPLE_FOREACH(tuple, sep, item, _)         _TPP_TUPLE_FOREACH(tuple, _TPP_TUPLE_SIZE(tuple), sep, item, _)
#define _TPP_TUPLE_FOREACH(tuple, size, sep, item, _)  _TPP_TUPLE_FOREACH_(tuple, size, sep, item, _)
#define _TPP_TUPLE_FOREACH_(tuple, size, sep, item, _) _TPP_TUPLE_FOREACH_##size(tuple, sep, item, _)

#define TPP_TUPLE_FOREACH_DUMMY_SEP(_, prev_index, prev_value, next_index, next_value) /* nothing */
/************************************************************************/

/************************************************************************/
/* File: parts/config.h                                                 */
/************************************************************************/
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


#if (!TPP_HAVE_CPP_PRAGMA && \
     !TPP_HAVE_STDC_PRAGMA && \
     !TPP_HAVE_MSVC_PRAGMA)
#undef TPP_COMMON_HAVE_PRAGMA
#define TPP_COMMON_HAVE_PRAGMA 0
#undef TPP_COMMON_HAVE_PRAGMA_GCC
#define TPP_COMMON_HAVE_PRAGMA_GCC 0
#endif /* !... */


/* Support for clang __has_attribute */
#ifndef TPP_HAVE_CLANG_HAS_ATTRIBUTE
#define TPP_HAVE_CLANG_HAS_ATTRIBUTE (-1)
#endif /* !TPP_HAVE_CLANG_HAS_ATTRIBUTE */

/* Support for clang __has_builtin */
#ifndef TPP_HAVE_CLANG_HAS_BUILTIN
#define TPP_HAVE_CLANG_HAS_BUILTIN (-1)
#endif /* !TPP_HAVE_CLANG_HAS_BUILTIN */

/* Support for clang __has_cpp_attribute */
#ifndef TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE
#define TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE (-1)
#endif /* !TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE */

/* Support for clang __has_declspec_attribute */
#ifndef TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE
#define TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE (-1)
#endif /* !TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE */

/* Support for clang __has_extension */
#ifndef TPP_HAVE_CLANG_HAS_EXTENSION
#define TPP_HAVE_CLANG_HAS_EXTENSION (-1)
#endif /* !TPP_HAVE_CLANG_HAS_EXTENSION */

/* Support for clang __has_feature */
#ifndef TPP_HAVE_CLANG_HAS_FEATURE
#define TPP_HAVE_CLANG_HAS_FEATURE (-1)
#endif /* !TPP_HAVE_CLANG_HAS_FEATURE */

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
/* WARNINGS                                                             */
/************************************************************************/

/*  */
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
/************************************************************************/
/************************************************************************/
/************************************************************************/
/************************************************************************/

/************************************************************************/
/* File: parts/ctype.h                                                  */
/************************************************************************/
TPP_DECL_BEGIN

#undef TPP_HAVE_BUILTIN_CTYPE
#if (!defined(tpp_ascii_issymstrt) ||    \
     !defined(tpp_ascii_issymcont) ||    \
     !defined(tpp_ascii_isdigit) ||      \
     !defined(tpp_ascii_isspace) ||      \
     !defined(tpp_ascii_islf) ||         \
     !defined(tpp_ascii_isspaceornul) || \
     !defined(tpp_ascii_islfornul) ||    \
     !defined(tpp_ascii_isspace_nolf))
#define TPP_HAVE_BUILTIN_CTYPE 1
#else /* ... */
#define TPP_HAVE_BUILTIN_CTYPE 0
#endif /* !... */


#if TPP_HAVE_BUILTIN_CTYPE
#define _TPP_CTYPE_ISSYMSTRT  0x01 /* Symbol start character */
#define _TPP_CTYPE_ISSYMCONT  0x02 /* Symbol continuation character */
#define _TPP_CTYPE_ISSPACE    0x04 /* Space character */
#define _TPP_CTYPE_ISDIGIT    0x08 /* Space character */
#define _TPP_CTYPE_ISLF       0x10 /* Linefeed character (\r and \n) */
#define _TPP_CTYPE_ISNONASCII 0x20 /* 128+ */

TPP_CONST_DECL uint_least8_t const tpp_ctype[256];
#if UINT_LEAST8_MAX == 0xff
#define _tpp_ascii_mask(ch) ((uint_least8_t)(ch))
#else /* UINT_LEAST8_MAX == 0xff */
#define _tpp_ascii_mask(ch) ((ch) & 0xff)
#endif /* UINT_LEAST8_MAX != 0xff */

#ifndef tpp_ascii_issymstrt
#define tpp_ascii_issymstrt(ch)    (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISSYMSTRT)
#endif /* !tpp_ascii_issymstrt */
#ifndef tpp_ascii_issymcont
#define tpp_ascii_issymcont(ch)    (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISSYMCONT)
#endif /* !tpp_ascii_issymcont */
#ifndef tpp_ascii_isdigit
#define tpp_ascii_isdigit(ch)      (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISDIGIT)
#endif /* !tpp_ascii_isdigit */
#ifndef tpp_ascii_isspace
#define tpp_ascii_isspace(ch)      (tpp_ctype[_tpp_ascii_mask(ch)] & (_TPP_CTYPE_ISSPACE | _TPP_CTYPE_ISLF))
#endif /* !tpp_ascii_isspace */
#ifndef tpp_ascii_islf
#define tpp_ascii_islf(ch)         (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISLF)
#endif /* !tpp_ascii_islf */
#ifndef tpp_ascii_islfornascii
#define tpp_ascii_islfornascii(ch) (tpp_ctype[_tpp_ascii_mask(ch)] & (_TPP_CTYPE_ISLF | _TPP_CTYPE_ISNONASCII))
#endif /* !tpp_ascii_islfornascii */
#ifndef tpp_ascii_isspace_nolf
#define tpp_ascii_isspace_nolf(ch) (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISSPACE)
#endif /* !tpp_ascii_isspace_nolf */
#endif /* TPP_HAVE_BUILTIN_CTYPE */

#ifndef tpp_ascii_islfornascii
#define tpp_ascii_islfornascii(ch) (tpp_ascii_islf(ch) || (ch) >= 0x80)
#endif /* !tpp_ascii_islfornascii */
#ifndef tpp_ascii_isspace_nolf
#define tpp_ascii_isspace_nolf(ch) (tpp_ascii_isspace(ch) && !tpp_ascii_islf(ch))
#endif /* !tpp_ascii_isspace_nolf */


#if TPP_HAVE_UNICODE

/* Unicode character traits (all of these take "tpp_unichar ord") */
#ifndef tpp_unicode_issymstrt
#define tpp_unicode_issymstrt(ord)    ((ord) >= 0x80 || tpp_ascii_issymstrt(ord))
#endif /* !tpp_unicode_issymstrt */
#ifndef tpp_unicode_issymcont
#define tpp_unicode_issymcont(ord)    ((ord) >= 0x80 || tpp_ascii_issymcont(ord))
#endif /* !tpp_unicode_issymcont */
#ifndef tpp_unicode_isdigit
#define tpp_unicode_isdigit(ord)      ((ord) < 0x80 && tpp_ascii_isdigit(ord))
#endif /* !tpp_unicode_isdigit */
#ifndef tpp_unicode_isspace
#define tpp_unicode_isspace(ord)      ((ord) <= 0xff && tpp_ascii_isspace(ord))
#endif /* !tpp_unicode_isspace */
#ifndef tpp_unicode_islf
#define tpp_unicode_islf(ord)         ((ord) <= 0xff && tpp_ascii_islf(ord))
#endif /* !tpp_unicode_islf */
#ifndef tpp_unicode_isspace_nolf
#define tpp_unicode_isspace_nolf(ord) ((ord) <= 0xff && tpp_ascii_isspace_nolf(ord))
#endif /* !tpp_unicode_isspace_nolf */

/* Read a single unicode character from a given utf-8 blob.
 * WARNING: This function doesn't do any validity checking,
 *          allowing over-long utf-8 sequences, as well as
 *          incorrectly positioned UTF-8 continuation bytes. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8(tpp_char const **p_pos, tpp_char const *end);

/* Same as `tpp_unicode_readutf8()', but read in reverse */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8_rev(tpp_char const **p_end, tpp_char const *base);
#endif /* TPP_HAVE_UNICODE */

#ifndef TPP_UTOA_MAXLEN
#if TPP_UINTMAX_MAX <= TPP_UINTMAX_C(255)
#define TPP_UTOA_MAXLEN 3  /* "255" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(65535)
#define TPP_UTOA_MAXLEN 5  /* "65535" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(4294967295)
#define TPP_UTOA_MAXLEN 10 /* "4294967295" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(18446744073709551615)
#define TPP_UTOA_MAXLEN 20 /* "18446744073709551615" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(340282366920938463463374607431768211455)
#define TPP_UTOA_MAXLEN 39 /* "340282366920938463463374607431768211455" */
#else /* TPP_UINTMAX_MAX <= ... */
#error "Unsupported 'TPP_UINTMAX_MAX'"
#endif /* TPP_UINTMAX_MAX > ... */
#endif /* !TPP_UTOA_MAXLEN */
#ifndef TPP_ITOA_MAXLEN
#define TPP_ITOA_MAXLEN (TPP_UTOA_MAXLEN + 1) /* +1 for leading "-" */
#endif /* !TPP_ITOA_MAXLEN */

/* Convert an integer into a string */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL tpp_utoa(char buf[TPP_UTOA_MAXLEN], tpp_uintmax value);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL tpp_itoa(char buf[TPP_ITOA_MAXLEN], tpp_intmax value);

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/string.h                                                 */
/************************************************************************/
TPP_DECL_BEGIN

typedef struct tpp_string {
	tpp_refcnt ts_refcnt;              /* Reference counter */
	tpp_size   ts_len;                 /* [const] Length of the string */
	tpp_char   ts_str[TPP_FLEX_ARRAY]; /* [const][ts_len] String content */
/*	tpp_char   ts_nul;                  * [const][== 0] Trailing \0-character */
} tpp_string;

/* Helpers for interacting with TPP strings */
#define tpp_string_destroy(self)  tpp_free(self)
#define tpp_string_incref(self)   tpp_refcnt_inc(&(self)->ts_refcnt)
#define tpp_string_isshared(self) tpp_refcnt_isshared(&(self)->ts_refcnt)
#define tpp_string_decref(self) \
	(void)(tpp_refcnt_decfetch(&(self)->ts_refcnt) || (tpp_string_destroy(self), 0))

#define tpp_string_sizeof(len)     (tpp_offsetof(tpp_string, ts_str) + ((len) + 1) * sizeof(tpp_char))
#define _tpp_string_trymalloc(len) ((tpp_string *)tpp_trymalloc(tpp_string_sizeof(len)))
#define _tpp_string_malloc(len)    ((tpp_string *)tpp_malloc(tpp_string_sizeof(len)))

/* Allocate new (uninitialized) string buffers
 * @return: NULL: Propagate TPP_ENOMEM */
TPP_DECL TPP_WUNUSED tpp_string *TPPCALL tpp_string_trymalloc(tpp_size len);
TPP_DECL TPP_WUNUSED tpp_string *TPPCALL tpp_string_malloc(tpp_size len);

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/file-io.h                                                */
/************************************************************************/
#ifndef TPP_OS_WINDOWS
#if (defined(_WIN64) || defined(WIN64) || \
     defined(_WIN32) || defined(WIN32) || defined(__WIN32__))
#define TPP_OS_WINDOWS 1
#else /* ... */
#define TPP_OS_WINDOWS 0
#endif /* !... */
#endif /* !TPP_OS_WINDOWS */

#ifndef TPP_OS_UNIX
#if (defined(__unix__) || defined(__unix) || defined(unix) || \
     defined(__posix__) || defined(__posix) || defined(posix))
#define TPP_OS_UNIX 1
#else /* ... */
#define TPP_OS_UNIX 0
#endif /* !... */
#endif /* !TPP_OS_UNIX */

#ifndef TPP_FS_HAVE_DRIVES
#define TPP_FS_HAVE_DRIVES TPP_OS_WINDOWS
#endif /* !TPP_FS_HAVE_DRIVES */

#ifndef TPP_FS_HAVE_ICASE
#define TPP_FS_HAVE_ICASE TPP_OS_WINDOWS
#endif /* !TPP_FS_HAVE_ICASE */

#ifndef TPP_FS_SEP
#define TPP_FS_SEP '/'
#endif /* !TPP_FS_SEP */

#ifndef TPP_FS_ALTSEP
#if TPP_OS_WINDOWS || defined(__CYGWIN__)
#define TPP_FS_ALTSEP '\\'
#else /* TPP_OS_WINDOWS */
#define TPP_FS_ALTSEP '/'
#endif /* !TPP_OS_WINDOWS */
#endif /* !TPP_FS_ALTSEP */

#ifndef TPP_FS_ISSEP
#if TPP_FS_SEP == TPP_FS_ALTSEP
#define TPP_FS_ISSEP(x) ((x) == TPP_FS_SEP)
#else /* TPP_FS_SEP == TPP_FS_ALTSEP */
#define TPP_FS_ISSEP(x) ((x) == TPP_FS_SEP || (x) == TPP_FS_ALTSEP)
#endif /* TPP_FS_SEP != TPP_FS_ALTSEP */
#endif /* !TPP_FS_ISSEP */

#ifndef TPP_FS_ISABS
#if TPP_FS_HAVE_DRIVES
#define TPP_FS_ISABS(filename) ((filename)[0] && (filename)[1] == ':')
#else /* TPP_FS_HAVE_DRIVES */
#define TPP_FS_ISABS(filename) TPP_FS_ISSEP((filename)[0])
#endif /* !TPP_FS_HAVE_DRIVES */
#endif /* !TPP_FS_ISABS */

#ifndef tpp_io_handle
#define tpp_io_handle_IS_BUILTIN
#if TPP_OS_WINDOWS
#include <Windows.h>
#define tpp_io_handle HANDLE
#define tpp_io_handle_IS_HANDLE
#define tpp_io_handle_INVALID INVALID_HANDLE_VALUE
#elif TPP_OS_UNIX
#define tpp_io_handle int
#define tpp_io_handle_IS_int
#define tpp_io_handle_INVALID (-1)
#else /* ... */
#include <stdio.h>
#define tpp_io_handle FILE *
#define tpp_io_handle_IS_FILE
#define tpp_io_handle_INVALID NULL

#if TPP_HAVE_FILE_NONBLOCK < 0
#undef TPP_HAVE_FILE_NONBLOCK
#define TPP_HAVE_FILE_NONBLOCK 0
#elif TPP_HAVE_FILE_NONBLOCK
#error "No way to implement 'TPP_HAVE_FILE_NONBLOCK' on this OS"
#endif /* ... */
#endif /* !... */
#endif /* !tpp_io_handle */


/* Support for non-blocking I/O */
#if TPP_HAVE_FILE_NONBLOCK
#define tpp_io_nonblock__PARAM , int nonblock
#define tpp_io_nonblock__ARG   , nonblock
#else /* TPP_HAVE_FILE_NONBLOCK */
#define tpp_io_nonblock__PARAM /* nothing */
#define tpp_io_nonblock__ARG   /* nothing */
#endif /* !TPP_HAVE_FILE_NONBLOCK */


#ifdef tpp_io_handle_IS_BUILTIN
TPP_DECL_BEGIN

/* Open a file for reading
 * @return: tpp_io_handle_INVALID: No such file or directory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_io_handle TPPCALL
tpp_io_open(/*utf-8*/ char const *filename);

/* Close a file previously opened by `tpp_io_open()' */
TPP_DECL void TPPCALL tpp_io_close(tpp_io_handle file);

/* Read data from a given `file' into `buf'
 * @return: * : The # of bytes read into `buf' (at most `bufsize')
 * @return: (tpp_ssize)TPP_EIO:         I/O error
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: (tpp_ssize)TPP_EWOULDBLOCK: `nonblock' was given, but operation would block
 * #endif // TPP_HAVE_FILE_NONBLOCK */
TPP_DECL TPP_WUNUSED TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_io_read(tpp_io_handle file, void *buf, tpp_size bufsize tpp_io_nonblock__PARAM);

TPP_DECL_END
#endif /* tpp_io_handle_IS_BUILTIN */
/************************************************************************/

/************************************************************************/
/* File: parts/error.h                                                  */
/************************************************************************/
TPP_DECL_BEGIN

typedef enum tpp_errno {
#define TPP_ISERR(error) ((error) != TPP_EOK)
	TPP_EOK         = 0,  /* Success */
	TPP_ENOMEM      = -1, /* tpp_malloc() or tpp_realloc() returned NULL */
	TPP_EIO         = -2, /* Filesystem I/O operation failed */
	TPP_ENOENT      = -3, /* No such file or directory */
#if TPP_HAVE_FILE_NONBLOCK
	TPP_EWOULDBLOCK = -4, /* Operation would block, but non-blocking was requested */
#endif /* TPP_HAVE_FILE_NONBLOCK */
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
	TPP_EMASKED     = -5, /* File cannot be opened because it has been masked */
#endif /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
#if TPP_HAVE_WARNINGS
	TPP_ELEXERROR   = -6, /* Hard lexer error (usually when too many -Werror were emitted) */
	TPP_EWARNPRINT  = -7, /* Printer registered for "tpp_lexer_warnf" returned an error */
#endif /* TPP_HAVE_WARNINGS */
} tpp_errno;

#if TPP_HAVE_STRERROR
/* Return a human-readable descriptor of "error" */
TPP_DECL TPP_WUNUSED char const *TPPCALL tpp_strerror(tpp_errno error);
#endif /* TPP_HAVE_STRERROR */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/token.h                                                  */
/************************************************************************/
TPP_DECL_BEGIN


typedef enum tpp_token_id {
	/* Token IDs can also represent error */
	TPP_TOK_ENOMEM      = (int)TPP_ENOMEM,
	TPP_TOK_EIO         = (int)TPP_EIO,
	TPP_TOK_ENOENT      = (int)TPP_ENOENT,
#if TPP_HAVE_FILE_NONBLOCK
	TPP_TOK_EWOULDBLOCK = (int)TPP_EWOULDBLOCK,
#endif /* TPP_HAVE_FILE_NONBLOCK */
#if TPP_HAVE_WARNINGS
	TPP_TOK_ELEXERROR   = (int)TPP_ELEXERROR,
	TPP_TOK_EWARNPRINT  = (int)TPP_EWARNPRINT,
#endif /* TPP_HAVE_WARNINGS */
#define TPP_TOK_ISERR(id)  ((int)(id) < 0)
#define TPP_TOK_ASERR(id)  ((enum tpp_errno)(int)(id))
#define TPP_TOK_OFERR(err) ((enum tpp_token_id)(int)(err))

	TPP_TOK_EOF   = '\0', /* END-OF-FILE (will always be ZERO) */
	TPP_TOK_LF    = '\n', /* Line-feed (always generated by `tpp_lexer_yieldraw()', filtered later if disabled) */
	TPP_TOK_SPACE = ' ',  /* Whitespace (always generated by `tpp_lexer_yieldraw()', filtered later if disabled) */

	/* Single-character tokens (always equal to that character's ordinal). */
	TPP_TOK_PLUS      = '+',
	TPP_TOK_AMP       = '&',
	TPP_TOK_EQUAL     = '=',
	TPP_TOK_AT        = '@',
	TPP_TOK_BACKSLASH = '\\',
	TPP_TOK_COLON     = ':',
	TPP_TOK_COMMA     = ',',
	TPP_TOK_SLASH     = '/',
	TPP_TOK_DOT       = '.',
	TPP_TOK_POUND     = '#',
	TPP_TOK_LANGLE    = '<',
	TPP_TOK_LBRACE    = '{',
	TPP_TOK_LBRACKET  = '[',
	TPP_TOK_LPAREN    = '(',
	TPP_TOK_PERCENT   = '%',
	TPP_TOK_STAR      = '*',
	TPP_TOK_EXCLAIM   = '!',
	TPP_TOK_PIPE      = '|',
	TPP_TOK_QMARK     = '?',
	TPP_TOK_RANGLE    = '>',
	TPP_TOK_RBRACE    = '}',
	TPP_TOK_RBRACKET  = ']',
	TPP_TOK_RPAREN    = ')',
	TPP_TOK_SEMICOLON = ';',
	TPP_TOK_MINUS     = '-',
	TPP_TOK_TILDE     = '~',
	TPP_TOK_HAT       = '^',

	/* Double(or more)-character tokens. */
	_TPP_TOK_STARTICHAR_BEGIN = 255,
#if TPP_HAVE_UNICODE
	TPP_TOK_UNICHAR, /* Misc unicode character that could not be classified */
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	TPP_TOK_INT, /* 42 */
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	TPP_TOK_FLOAT, /* 42.0 */
#endif /* TPP_HAVE_TPP_TOK_FLOAT */

#if TPP_HAVE_TPP_TOK_COMMENTLIKE
	TPP_TOK_COMMENTLIKE_MIN,
	_TPP_TOK_COMMENTLIKE_MIN = TPP_TOK_COMMENTLIKE_MIN - 1,
#if TPP_HAVE_TPP_TOK_C_COMMENT
	TPP_TOK_C_COMMENT, /* like this one! */
#define _TPP_CASE_TPP_TOK_C_COMMENT case TPP_TOK_C_COMMENT:
#else /* TPP_HAVE_TPP_TOK_C_COMMENT */
#define _TPP_CASE_TPP_TOK_C_COMMENT /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_C_COMMENT */
#if TPP_HAVE_TPP_TOK_PASCAL_COMMENT
	TPP_TOK_PASCAL_COMMENT, // "(*like this one!*)"
#define _TPP_CASE_TPP_TOK_PASCAL_COMMENT case TPP_TOK_PASCAL_COMMENT:
#else /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT */
#define _TPP_CASE_TPP_TOK_PASCAL_COMMENT /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_PASCAL_COMMENT */
	_TPP_TOK_COMMENTLIKE_NOLINE_MAX,
	TPP_TOK_COMMENTLIKE_NOLINE_MAX = _TPP_TOK_COMMENTLIKE_NOLINE_MAX - 1,
#if TPP_HAVE_TPP_TOK_CXX_COMMENT
	TPP_TOK_CXX_COMMENT, // like this one!
#define _TPP_CASE_TPP_TOK_CXX_COMMENT case TPP_TOK_CXX_COMMENT:
#else /* TPP_HAVE_TPP_TOK_CXX_COMMENT */
#define _TPP_CASE_TPP_TOK_CXX_COMMENT /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
	TPP_TOK_SHELL_COMMENT, // "# like this one!"
#define _TPP_CASE_TPP_TOK_SHELL_COMMENT case TPP_TOK_SHELL_COMMENT:
#else /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#define _TPP_CASE_TPP_TOK_SHELL_COMMENT /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#if TPP_HAVE_TPP_TOK_ASM_COMMENT
	TPP_TOK_ASM_COMMENT, // "/ like this one!"
#define _TPP_CASE_TPP_TOK_ASM_COMMENT case TPP_TOK_ASM_COMMENT:
#else /* TPP_HAVE_TPP_TOK_ASM_COMMENT */
#define _TPP_CASE_TPP_TOK_ASM_COMMENT /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_ASM_COMMENT */
#if TPP_HAVE_TPP_TOK_SQL_COMMENT
	TPP_TOK_SQL_COMMENT, // "-- like this one!"
#define _TPP_CASE_TPP_TOK_SQL_COMMENT case TPP_TOK_SQL_COMMENT:
#else /* TPP_HAVE_TPP_TOK_SQL_COMMENT */
#define _TPP_CASE_TPP_TOK_SQL_COMMENT /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_SQL_COMMENT */
	_TPP_TOK_COMMENTLIKE_MAX,
	TPP_TOK_COMMENTLIKE_MAX = _TPP_TOK_COMMENTLIKE_MAX - 1,
#define TPP_TOK_ISCOMMENT(id)                     \
	((int)(id) >= (int)TPP_TOK_COMMENTLIKE_MIN && \
	 (int)(id) <= (int)TPP_TOK_COMMENTLIKE_MAX)
#define TPP_CASE_TPP_TOK_COMMENT_NOLINE \
	_TPP_CASE_TPP_TOK_C_COMMENT         \
	_TPP_CASE_TPP_TOK_PASCAL_COMMENT
#define TPP_CASE_TPP_TOK_COMMENT_LINE \
	_TPP_CASE_TPP_TOK_CXX_COMMENT     \
	_TPP_CASE_TPP_TOK_SHELL_COMMENT   \
	_TPP_CASE_TPP_TOK_ASM_COMMENT     \
	_TPP_CASE_TPP_TOK_SQL_COMMENT
#define TPP_CASE_TPP_TOK_COMMENT    \
	TPP_CASE_TPP_TOK_COMMENT_NOLINE \
	TPP_CASE_TPP_TOK_COMMENT_LINE
#else /* TPP_HAVE_TPP_TOK_COMMENTLIKE */
#define TPP_TOK_ISCOMMENT(id)           0
#define TPP_CASE_TPP_TOK_COMMENT_NOLINE /* nothing */
#define TPP_CASE_TPP_TOK_COMMENT_LINE   /* nothing */
#define TPP_CASE_TPP_TOK_COMMENT        /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_COMMENTLIKE */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
#define TPP_TOK_ISCOMMENT_NOLINE(id)              \
	((int)(id) >= (int)TPP_TOK_COMMENTLIKE_MIN && \
	 (int)(id) <= (int)TPP_TOK_COMMENTLIKE_NOLINE_MAX)
#else /* TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE */
#define TPP_TOK_ISCOMMENT_NOLINE(id) 0
#endif /* !TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE
#define TPP_TOK_ISCOMMENT_LINE(id)                      \
	((int)(id) > (int)TPP_TOK_COMMENTLIKE_NOLINE_MAX && \
	 (int)(id) <= (int)TPP_TOK_COMMENTLIKE_MAX)
#else /* TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE */
#define TPP_TOK_ISCOMMENT_LINE(id) 0
#endif /* !TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE */


#if TPP_HAVE_TPP_TOK_STRINGLIKE
	TPP_TOK_STRINGLIKE_MIN,
	_TPP_TOK_STRINGLIKE_MIN = TPP_TOK_STRINGLIKE_MIN - 1,
#if TPP_HAVE_TPP_TOK_CHAR
	TPP_TOK_CHAR, /* 'foo' */
#define _TPP_CASE_TPP_TOK_CHAR case TPP_TOK_CHAR:
#else /* TPP_HAVE_TPP_TOK_CHAR */
#define _TPP_CASE_TPP_TOK_CHAR /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CHAR */
#if TPP_HAVE_TPP_TOK_STRING
	TPP_TOK_STRING, /* "foo" */
#define _TPP_CASE_TPP_TOK_STRING case TPP_TOK_STRING:
#else /* TPP_HAVE_TPP_TOK_STRING */
#define _TPP_CASE_TPP_TOK_STRING /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_STRING */
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_TOK_CXX_RAW_STRING_LITERAL, /* R"AB(foo)AB" */
#define _TPP_CASE_TPP_TOK_CXX_RAW_STRING_LITERAL case TPP_TOK_CXX_RAW_STRING_LITERAL:
#else /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_CXX_RAW_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	TPP_TOK_CXX_WIDE_STRING_LITERAL, /* L"foo" */
#define _TPP_CASE_TPP_TOK_CXX_WIDE_STRING_LITERAL case TPP_TOK_CXX_WIDE_STRING_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL, /* LR"AB(foo)AB" */
#define _TPP_CASE_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL case TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL:
#else /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#else /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_CXX_WIDE_STRING_LITERAL     /* nothing */
#define _TPP_CASE_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	TPP_TOK_CXX_UTF8_STRING_LITERAL, /* u8"foo" */
#define _TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL case TPP_TOK_CXX_UTF8_STRING_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL, /* u8R"AB(foo)AB" */
#define _TPP_CASE_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL case TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL:
#else /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#else /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL     /* nothing */
#define _TPP_CASE_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	TPP_TOK_CXX_UTF16_STRING_LITERAL, /* u"foo" */
#define _TPP_CASE_TPP_TOK_CXX_UTF16_STRING_LITERAL case TPP_TOK_CXX_UTF16_STRING_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL, /* uR"AB(foo)AB" */
#define _TPP_CASE_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL case TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL:
#else /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#else /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_CXX_UTF16_STRING_LITERAL     /* nothing */
#define _TPP_CASE_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	TPP_TOK_CXX_UTF32_STRING_LITERAL, /* U"foo" */
#define _TPP_CASE_TPP_TOK_CXX_UTF32_STRING_LITERAL case TPP_TOK_CXX_UTF32_STRING_LITERAL:
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL, /* UR"AB(foo)AB" */
#define _TPP_CASE_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL case TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL:
#else /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#else /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_CXX_UTF32_STRING_LITERAL     /* nothing */
#define _TPP_CASE_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
	TPP_TOK_RAW_STRING_LITERAL, /* R"foo"  r"foo"  */
#define _TPP_CASE_TPP_TOK_RAW_STRING_LITERAL case TPP_TOK_RAW_STRING_LITERAL:
#else /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_RAW_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	TPP_TOK_RAW_CHAR_LITERAL, /* R'bar'  r'bar' */
#define _TPP_CASE_TPP_TOK_RAW_CHAR_LITERAL case TPP_TOK_RAW_CHAR_LITERAL:
#else /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#define _TPP_CASE_TPP_TOK_RAW_CHAR_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	TPP_TOK_BLOCK_STRING_LITERAL, /* """foo""" */
#define _TPP_CASE_TPP_TOK_BLOCK_STRING_LITERAL case TPP_TOK_BLOCK_STRING_LITERAL:
#else /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#define _TPP_CASE_TPP_TOK_BLOCK_STRING_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	TPP_TOK_BLOCK_CHAR_LITERAL, /* '''foo''' */
#define _TPP_CASE_TPP_TOK_BLOCK_CHAR_LITERAL case TPP_TOK_BLOCK_CHAR_LITERAL:
#else /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#define _TPP_CASE_TPP_TOK_BLOCK_CHAR_LITERAL /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
	_TPP_TOK_STRINGLIKE_MAX,
	TPP_TOK_STRINGLIKE_MAX = _TPP_TOK_STRINGLIKE_MAX - 1,
#define TPP_TOK_ISSTRING(id)                     \
	((int)(id) >= (int)TPP_TOK_STRINGLIKE_MIN && \
	 (int)(id) <= (int)TPP_TOK_STRINGLIKE_MAX)
#define TPP_CASE_TPP_TOK_STRING                    \
	_TPP_CASE_TPP_TOK_CHAR                         \
	_TPP_CASE_TPP_TOK_STRING                       \
	_TPP_CASE_TPP_TOK_CXX_RAW_STRING_LITERAL       \
	_TPP_CASE_TPP_TOK_CXX_WIDE_STRING_LITERAL      \
	_TPP_CASE_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL  \
	_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL      \
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL  \
	_TPP_CASE_TPP_TOK_CXX_UTF16_STRING_LITERAL     \
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL \
	_TPP_CASE_TPP_TOK_CXX_UTF32_STRING_LITERAL     \
	_TPP_CASE_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL \
	_TPP_CASE_TPP_TOK_RAW_STRING_LITERAL           \
	_TPP_CASE_TPP_TOK_RAW_CHAR_LITERAL             \
	_TPP_CASE_TPP_TOK_BLOCK_STRING_LITERAL         \
	_TPP_CASE_TPP_TOK_BLOCK_CHAR_LITERAL
#else /* TPP_HAVE_TPP_TOK_STRINGLIKE */
#define TPP_TOK_ISSTRING(id)    0
#define TPP_CASE_TPP_TOK_STRING /* nothing */
#endif /* !TPP_HAVE_TPP_TOK_STRINGLIKE */

#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	TPP_TOK_LANGLE_LANGLE, /* "<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	TPP_TOK_RANGLE_RANGLE, /* ">>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	TPP_TOK_EQUAL_EQUAL, /* "==" */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	TPP_TOK_EXCLAIM_EQUAL, /* "!=" */
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	TPP_TOK_RANGLE_EQUAL, /* ">=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	TPP_TOK_LANGLE_EQUAL, /* "<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	TPP_TOK_DOT_DOT_DOT, /* "..." */
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL
	TPP_TOK_PLUS_EQUAL, /* "+=" */
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL
	TPP_TOK_MINUS_EQUAL, /* "-=" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL
	TPP_TOK_STAR_EQUAL, /* "*=" */
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL
	TPP_TOK_SLASH_EQUAL, /* "/=" */
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	TPP_TOK_PERCENT_EQUAL, /* "%=" */
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	TPP_TOK_LANGLE_LANGLE_EQUAL, /* "<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	TPP_TOK_RANGLE_RANGLE_EQUAL, /* ">>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL
	TPP_TOK_AMP_EQUAL, /* "&=" */
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL
	TPP_TOK_PIPE_EQUAL, /* "|=" */
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL
	TPP_TOK_HAT_EQUAL, /* "^=" */
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	TPP_TOK_STAR_STAR_EQUAL, /* "**=" */
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	TPP_TOK_SLASH_SLASH_EQUAL, /* "//=" */
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_AT_EQUAL
	TPP_TOK_AT_EQUAL, /* "@=" */
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_POUND_POUND
	TPP_TOK_POUND_POUND, /* "##" */
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_AMP_AMP
	TPP_TOK_AMP_AMP, /* "&&" */
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE
	TPP_TOK_PIPE_PIPE, /* "||" */
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_HAT_HAT
	TPP_TOK_HAT_HAT, /* "^^" */
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS
	TPP_TOK_PLUS_PLUS, /* "++" */
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS
	TPP_TOK_MINUS_MINUS, /* "--" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_STAR_STAR
	TPP_TOK_STAR_STAR, /* "**" */
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH
	TPP_TOK_SLASH_SLASH, /* "//" */
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE
	TPP_TOK_TILDE_TILDE, /* "~~" */
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL
	TPP_TOK_TILDE_EQUAL, /* "~=" */
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	TPP_TOK_MINUS_RANGLE, /* "->" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL
	TPP_TOK_COLON_EQUAL, /* ":=" */
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_COLON_COLON
	TPP_TOK_COLON_COLON, /* "::" */
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	TPP_TOK_MINUS_RANGLE_STAR, /* "->*" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_DOT_STAR
	TPP_TOK_DOT_STAR, /* ".*" */
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_DOT_DOT
	TPP_TOK_DOT_DOT, /* ".." */
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	TPP_TOK_LANGLE_RANGLE, /* "<>" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	TPP_TOK_LANGLE_LANGLE_LANGLE, /* "<<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	TPP_TOK_RANGLE_RANGLE_RANGLE, /* ">>>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL, /* "<<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL, /* ">>>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	TPP_TOK_EQUAL_EQUAL_EQUAL, /* "===" */
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	TPP_TOK_EXCLAIM_EQUAL_EQUAL, /* "!==" */
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK
	TPP_TOK_QMARK_QMARK, /* "??" */
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL
	TPP_TOK_QMARK_EQUAL, /* "?=" */
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL */

	TPP_TOK_STARTICHAR_END, /* KEEP THIS THE LAST TOKEN! */
	TPP_TOK_KEYWORD_BEGIN = TPP_TOK_STARTICHAR_END, /* First builtin keyword */
	_TPP_TOK_KEYWORD_BEGIN = TPP_TOK_KEYWORD_BEGIN - 1,
#define TPP_DEFS
#define TPP_KWD(id, string) id,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	TPP_TOK_USERKEYWORD_BEGIN, /* First user-defined (non-builtin) keyword */
} tpp_token_id;

/* Helper macros to easily detect certain types of whitespace. */
#define TPP_TOK_ISSPACE_OR_COMMENT(id)       ((id) == TPP_TOK_SPACE || TPP_TOK_ISCOMMENT_NOLINE(id))
#define TPP_TOK_ISLF_OR_COMMENT(id)          ((id) == TPP_TOK_LF || TPP_TOK_ISCOMMENT_LINE(id))
#define TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(id) ((id) == TPP_TOK_SPACE || (id) == TPP_TOK_LF || TPP_TOK_ISCOMMENT(id))


/* Helper macros to determine which (and what kind of) keyword is described by a given `id' */
#define TPP_TOK_ISKEYWORD(id)        ((int)(id) >= (int)TPP_TOK_KEYWORD_BEGIN)
#define TPP_TOK_ISUSERKEYWORD(id)    ((int)(id) >= (int)TPP_TOK_USERKEYWORD_BEGIN)
#define TPP_TOK_ISBUILTINKEYWORD(id) (TPP_TOK_ISKEYWORD(id) && !TPP_TOK_ISUSERKEYWORD(id))

#if TPP_HAVE_STRTOKENID
/* Returns the "*" in "TPP_TOK_*" of "id", which must be a (non-keyword and non-error) token ID */
TPP_DECL TPP_WUNUSED char const *TPPCALL tpp_strtokenid(tpp_token_id id);
#endif /* TPP_HAVE_STRTOKENID */

struct tpp_keyword;
typedef struct tpp_token {
	tpp_token_id              tt_id;    /* Token ID (never set to one of `TPP_TOK_E*'; iow: always positive or TPP_TOK_EOF) */
	struct tpp_keyword const *tt_kwd;   /* [1..1][valid_if(TPP_TOK_ISKEYWORD(tt_id))] Keyword identified by `tt_id' */
	tpp_char const           *tt_start; /* [1..1][>= tt_chunk->ts_str && <= tt_end] Token start pointer */
	tpp_char const           *tt_end;   /* [1..1][>= tt_start && <= tt_chunk->ts_str+tt_chunk->ts_len] Token end pointer */
	TPP_REF tpp_string       *tt_chunk; /* [0..1] Text chunk containing "tt_start" and "tt_end" (or "NULL" if not needed) */
} tpp_token;

#define tpp_token_initcopy(self, other) \
	(void)(*(self) = *(other), tpp_string_incref((self)->tt_chunk))
#define tpp_token_fini(self) tpp_string_decref((self)->tt_chunk)


#if 0
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_token_encodestring(tpp_formatprinter printer, void *arg,
                       void const *data, tpp_size num_bytes);
#endif


TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/features.h                                               */
/************************************************************************/
TPP_DECL_BEGIN

#undef TPP_HAVE_FEATURES
#if ((TPP_HAVE_TPP_TOK_LF < 0) ||                         \
     (TPP_HAVE_TPP_TOK_SPACE < 0) ||                      \
     (TPP_HAVE_TPP_TOK_COMMENT < 0) ||                    \
     (TPP_HAVE_TPP_TOK_CXX_COMMENT < 0) ||                \
     (TPP_HAVE_TPP_TOK_C_COMMENT < 0) ||                  \
     (TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0) ||             \
     (TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0) ||              \
     (TPP_HAVE_TPP_TOK_ASM_COMMENT < 0) ||                \
     (TPP_HAVE_TPP_TOK_SQL_COMMENT < 0) ||                \
     (TPP_HAVE_TPP_TOK_INT < 0) ||                        \
     (TPP_HAVE_TPP_TOK_FLOAT < 0) ||                      \
     (TPP_HAVE_TPP_TOK_CHAR < 0) ||                       \
     (TPP_HAVE_TPP_TOK_STRING < 0) ||                     \
     (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0) ||     \
     (TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0) ||    \
     (TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0) ||    \
     (TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0) ||   \
     (TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0) ||   \
     (TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0) ||         \
     (TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0) ||           \
     (TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0) ||       \
     (TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0) ||         \
     (TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0) ||              \
     (TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0) ||              \
     (TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0) ||              \
     (TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0) ||               \
     (TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0) ||               \
     (TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0) ||                \
     (TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0) ||                 \
     (TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_STAR_EQUAL < 0) ||                 \
     (TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0) ||              \
     (TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0) ||        \
     (TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0) ||        \
     (TPP_HAVE_TPP_TOK_AMP_EQUAL < 0) ||                  \
     (TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0) ||                 \
     (TPP_HAVE_TPP_TOK_HAT_EQUAL < 0) ||                  \
     (TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0) ||            \
     (TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0) ||          \
     (TPP_HAVE_TPP_TOK_AT_EQUAL < 0) ||                   \
     (TPP_HAVE_TPP_TOK_POUND_POUND < 0) ||                \
     (TPP_HAVE_TPP_TOK_AMP_AMP < 0) ||                    \
     (TPP_HAVE_TPP_TOK_PIPE_PIPE < 0) ||                  \
     (TPP_HAVE_TPP_TOK_HAT_HAT < 0) ||                    \
     (TPP_HAVE_TPP_TOK_PLUS_PLUS < 0) ||                  \
     (TPP_HAVE_TPP_TOK_MINUS_MINUS < 0) ||                \
     (TPP_HAVE_TPP_TOK_STAR_STAR < 0) ||                  \
     (TPP_HAVE_TPP_TOK_SLASH_SLASH < 0) ||                \
     (TPP_HAVE_TPP_TOK_TILDE_TILDE < 0) ||                \
     (TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0) ||               \
     (TPP_HAVE_TPP_TOK_COLON_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_COLON_COLON < 0) ||                \
     (TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0) ||          \
     (TPP_HAVE_TPP_TOK_DOT_STAR < 0) ||                   \
     (TPP_HAVE_TPP_TOK_DOT_DOT < 0) ||                    \
     (TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0) ||              \
     (TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0) ||       \
     (TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0) ||       \
     (TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0) || \
     (TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0) || \
     (TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0) ||          \
     (TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0) ||        \
     (TPP_HAVE_TPP_TOK_QMARK_QMARK < 0) ||                \
     (TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0) ||                \
     (TPP_HAVE_CPP_DIRECTIVES < 0) ||                     \
     (TPP_HAVE_CPP_MACROS < 0) ||                         \
     (TPP_HAVE_CPP_EXCLAIM < 0) ||                        \
     (TPP_HAVE_CPP_BLANK < 0) ||                          \
     (TPP_HAVE_CPP_DIGIT_LINE < 0) ||                     \
     (TPP_HAVE_CPP_LINE < 0) ||                           \
     (TPP_HAVE_CPP_INCLUDE < 0) ||                        \
     (TPP_HAVE_CPP_INCLUDE_NEXT < 0) ||                   \
     (TPP_HAVE_CPP_IMPORT < 0) ||                         \
     (TPP_HAVE_CPP_IF_ELSE_ENDIF < 0) ||                  \
     (TPP_HAVE_CPP_DEFINE < 0) ||                         \
     (TPP_HAVE_CPP_ASSERT < 0) ||                         \
     (TPP_HAVE_CPP_ERROR < 0) ||                          \
     (TPP_HAVE_CPP_WARNING < 0) ||                        \
     (TPP_HAVE_CPP_IDENT < 0) ||                          \
     (TPP_HAVE_CPP_SCSS < 0) ||                           \
     (TPP_HAVE_CPP_PRAGMA < 0) ||                         \
     (TPP_HAVE_STDC_PRAGMA < 0) ||                        \
     (TPP_HAVE_MSVC_PRAGMA < 0))
#define TPP_HAVE_FEATURES 1
#else /* ... */
#define TPP_HAVE_FEATURES 0
#endif/* !... */

#if TPP_HAVE_FEATURES
enum tpp_feature {
#if TPP_HAVE_TPP_TOK_LF < 0
	TPP_FEAT_TPP_TOK_LF,
#endif /* TPP_HAVE_TPP_TOK_LF < 0 */
#if TPP_HAVE_TPP_TOK_SPACE < 0
	TPP_FEAT_TPP_TOK_SPACE,
#endif /* TPP_HAVE_TPP_TOK_SPACE < 0 */
#if TPP_HAVE_TPP_TOK_COMMENT < 0
	TPP_FEAT_TPP_TOK_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_CXX_COMMENT < 0
	TPP_FEAT_TPP_TOK_CXX_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_C_COMMENT < 0
	TPP_FEAT_TPP_TOK_C_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_C_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0
	TPP_FEAT_TPP_TOK_PASCAL_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0
	TPP_FEAT_TPP_TOK_SHELL_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_ASM_COMMENT < 0
	TPP_FEAT_TPP_TOK_ASM_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_SQL_COMMENT < 0
	TPP_FEAT_TPP_TOK_SQL_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_INT < 0
	TPP_FEAT_TPP_TOK_INT,
#endif /* TPP_HAVE_TPP_TOK_INT < 0 */
#if TPP_HAVE_TPP_TOK_FLOAT < 0
	TPP_FEAT_TPP_TOK_FLOAT,
#endif /* TPP_HAVE_TPP_TOK_FLOAT < 0 */
#if TPP_HAVE_TPP_TOK_CHAR < 0
	TPP_FEAT_TPP_TOK_CHAR,
#endif /* TPP_HAVE_TPP_TOK_CHAR < 0 */
#if TPP_HAVE_TPP_TOK_STRING < 0
	TPP_FEAT_TPP_TOK_STRING,
#endif /* TPP_HAVE_TPP_TOK_STRING < 0 */
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0
	TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0
	TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0
	TPP_FEAT_TPP_TOK_LANGLE_LANGLE,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0
	TPP_FEAT_TPP_TOK_RANGLE_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0
	TPP_FEAT_TPP_TOK_EQUAL_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0
	TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_RANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_LANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0
	TPP_FEAT_TPP_TOK_DOT_DOT_DOT,
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0 */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0
	TPP_FEAT_TPP_TOK_PLUS_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0
	TPP_FEAT_TPP_TOK_MINUS_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL < 0
	TPP_FEAT_TPP_TOK_STAR_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0
	TPP_FEAT_TPP_TOK_SLASH_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0
	TPP_FEAT_TPP_TOK_PERCENT_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL < 0
	TPP_FEAT_TPP_TOK_AMP_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0
	TPP_FEAT_TPP_TOK_PIPE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL < 0
	TPP_FEAT_TPP_TOK_HAT_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0
	TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0
	TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_AT_EQUAL < 0
	TPP_FEAT_TPP_TOK_AT_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_POUND_POUND < 0
	TPP_FEAT_TPP_TOK_POUND_POUND,
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND < 0 */
#if TPP_HAVE_TPP_TOK_AMP_AMP < 0
	TPP_FEAT_TPP_TOK_AMP_AMP,
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP < 0 */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE < 0
	TPP_FEAT_TPP_TOK_PIPE_PIPE,
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE < 0 */
#if TPP_HAVE_TPP_TOK_HAT_HAT < 0
	TPP_FEAT_TPP_TOK_HAT_HAT,
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT < 0 */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS < 0
	TPP_FEAT_TPP_TOK_PLUS_PLUS,
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS < 0
	TPP_FEAT_TPP_TOK_MINUS_MINUS,
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR < 0
	TPP_FEAT_TPP_TOK_STAR_STAR,
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH < 0
	TPP_FEAT_TPP_TOK_SLASH_SLASH,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH < 0 */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE < 0
	TPP_FEAT_TPP_TOK_TILDE_TILDE,
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE < 0 */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0
	TPP_FEAT_TPP_TOK_TILDE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0
	TPP_FEAT_TPP_TOK_MINUS_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL < 0
	TPP_FEAT_TPP_TOK_COLON_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_COLON_COLON < 0
	TPP_FEAT_TPP_TOK_COLON_COLON,
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0
	TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR,
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0 */
#if TPP_HAVE_TPP_TOK_DOT_STAR < 0
	TPP_FEAT_TPP_TOK_DOT_STAR,
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR < 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT < 0
	TPP_FEAT_TPP_TOK_DOT_DOT,
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0
	TPP_FEAT_TPP_TOK_LANGLE_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0
	TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0
	TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0
	TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0
	TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK < 0
	TPP_FEAT_TPP_TOK_QMARK_QMARK,
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK < 0 */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0
	TPP_FEAT_TPP_TOK_QMARK_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0 */
#if TPP_HAVE_CPP_DIRECTIVES < 0
	TPP_FEAT_CPP_DIRECTIVES,
#endif /* TPP_HAVE_CPP_DIRECTIVES < 0 */
#if TPP_HAVE_CPP_MACROS < 0
	TPP_FEAT_CPP_MACROS,
#endif /* TPP_HAVE_CPP_MACROS < 0 */
#if TPP_HAVE_CPP_EXCLAIM < 0
	TPP_FEAT_CPP_EXCLAIM,
#endif /* TPP_HAVE_CPP_EXCLAIM < 0 */
#if TPP_HAVE_CPP_BLANK < 0
	TPP_FEAT_CPP_BLANK,
#endif /* TPP_HAVE_CPP_BLANK < 0 */
#if TPP_HAVE_CPP_DIGIT_LINE < 0
	TPP_FEAT_CPP_DIGIT_LINE,
#endif /* TPP_HAVE_CPP_DIGIT_LINE < 0 */
#if TPP_HAVE_CPP_LINE < 0
	TPP_FEAT_CPP_LINE,
#endif /* TPP_HAVE_CPP_LINE < 0 */
#if TPP_HAVE_CPP_INCLUDE < 0
	TPP_FEAT_CPP_INCLUDE,
#endif /* TPP_HAVE_CPP_INCLUDE < 0 */
#if TPP_HAVE_CPP_INCLUDE_NEXT < 0
	TPP_FEAT_CPP_INCLUDE_NEXT,
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT < 0 */
#if TPP_HAVE_CPP_IMPORT < 0
	TPP_FEAT_CPP_IMPORT,
#endif /* TPP_HAVE_CPP_IMPORT < 0 */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF < 0
	TPP_FEAT_CPP_IF_ELSE_ENDIF,
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF < 0 */
#if TPP_HAVE_CPP_DEFINE < 0
	TPP_FEAT_CPP_DEFINE,
#endif /* TPP_HAVE_CPP_DEFINE < 0 */
#if TPP_HAVE_CPP_ASSERT < 0
	TPP_FEAT_CPP_ASSERT,
#endif /* TPP_HAVE_CPP_ASSERT < 0 */
#if TPP_HAVE_CPP_ERROR < 0
	TPP_FEAT_CPP_ERROR,
#endif /* TPP_HAVE_CPP_ERROR < 0 */
#if TPP_HAVE_CPP_WARNING < 0
	TPP_FEAT_CPP_WARNING,
#endif /* TPP_HAVE_CPP_WARNING < 0 */
#if TPP_HAVE_CPP_IDENT < 0
	TPP_FEAT_CPP_IDENT,
#endif /* TPP_HAVE_CPP_IDENT < 0 */
#if TPP_HAVE_CPP_SCSS < 0
	TPP_FEAT_CPP_SCSS,
#endif /* TPP_HAVE_CPP_SCSS < 0 */
#if TPP_HAVE_CPP_PRAGMA < 0
	TPP_FEAT_CPP_PRAGMA,
#endif /* TPP_HAVE_CPP_PRAGMA < 0 */
#if TPP_HAVE_STDC_PRAGMA < 0
	TPP_FEAT_STDC_PRAGMA,
#endif /* TPP_HAVE_STDC_PRAGMA < 0 */
#if TPP_HAVE_MSVC_PRAGMA < 0
	TPP_FEAT_MSVC_PRAGMA,
#endif /* TPP_HAVE_MSVC_PRAGMA < 0 */
	TPP_FEAT_COUNT
};

typedef union tpp_features {
	struct {
#if TPP_HAVE_TPP_TOK_LF < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LF: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LF(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LF, TPP_HAVE_TPP_TOK_LF == -1)
#else /* TPP_HAVE_TPP_TOK_LF < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LF(self) TPP_HAVE_TPP_TOK_LF
#endif /* TPP_HAVE_TPP_TOK_LF >= 0 */
#if TPP_HAVE_TPP_TOK_SPACE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SPACE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SPACE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SPACE, TPP_HAVE_TPP_TOK_SPACE == -1)
#else /* TPP_HAVE_TPP_TOK_SPACE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SPACE(self) TPP_HAVE_TPP_TOK_SPACE
#endif /* TPP_HAVE_TPP_TOK_SPACE >= 0 */
#if TPP_HAVE_TPP_TOK_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_COMMENT, TPP_HAVE_TPP_TOK_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COMMENT(self) TPP_HAVE_TPP_TOK_COMMENT
#endif /* TPP_HAVE_TPP_TOK_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_COMMENT, TPP_HAVE_TPP_TOK_CXX_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_COMMENT(self) TPP_HAVE_TPP_TOK_CXX_COMMENT
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_C_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_C_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_C_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_C_COMMENT, TPP_HAVE_TPP_TOK_C_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_C_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_C_COMMENT(self) TPP_HAVE_TPP_TOK_C_COMMENT
#endif /* TPP_HAVE_TPP_TOK_C_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PASCAL_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PASCAL_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PASCAL_COMMENT, TPP_HAVE_TPP_TOK_PASCAL_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PASCAL_COMMENT(self) TPP_HAVE_TPP_TOK_PASCAL_COMMENT
#endif /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SHELL_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SHELL_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SHELL_COMMENT, TPP_HAVE_TPP_TOK_SHELL_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SHELL_COMMENT(self) TPP_HAVE_TPP_TOK_SHELL_COMMENT
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_ASM_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_ASM_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_ASM_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_ASM_COMMENT, TPP_HAVE_TPP_TOK_ASM_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_ASM_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_ASM_COMMENT(self) TPP_HAVE_TPP_TOK_ASM_COMMENT
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_SQL_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SQL_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SQL_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SQL_COMMENT, TPP_HAVE_TPP_TOK_SQL_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_SQL_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SQL_COMMENT(self) TPP_HAVE_TPP_TOK_SQL_COMMENT
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_INT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_INT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_INT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_INT, TPP_HAVE_TPP_TOK_INT == -1)
#else /* TPP_HAVE_TPP_TOK_INT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_INT(self) TPP_HAVE_TPP_TOK_INT
#endif /* TPP_HAVE_TPP_TOK_INT >= 0 */
#if TPP_HAVE_TPP_TOK_FLOAT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_FLOAT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_FLOAT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_FLOAT, TPP_HAVE_TPP_TOK_FLOAT == -1)
#else /* TPP_HAVE_TPP_TOK_FLOAT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_FLOAT(self) TPP_HAVE_TPP_TOK_FLOAT
#endif /* TPP_HAVE_TPP_TOK_FLOAT >= 0 */
#if TPP_HAVE_TPP_TOK_CHAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CHAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CHAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CHAR, TPP_HAVE_TPP_TOK_CHAR == -1)
#else /* TPP_HAVE_TPP_TOK_CHAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CHAR(self) TPP_HAVE_TPP_TOK_CHAR
#endif /* TPP_HAVE_TPP_TOK_CHAR >= 0 */
#if TPP_HAVE_TPP_TOK_STRING < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STRING: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STRING(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STRING, TPP_HAVE_TPP_TOK_STRING == -1)
#else /* TPP_HAVE_TPP_TOK_STRING < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STRING(self) TPP_HAVE_TPP_TOK_STRING
#endif /* TPP_HAVE_TPP_TOK_STRING >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL, TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL, TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL(self) TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL, TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL, TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL(self) TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
#endif /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE, TPP_HAVE_TPP_TOK_LANGLE_LANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE(self) TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE, TPP_HAVE_TPP_TOK_RANGLE_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE(self) TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL, TPP_HAVE_TPP_TOK_EQUAL_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL(self) TPP_HAVE_TPP_TOK_EQUAL_EQUAL
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL, TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL(self) TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_EQUAL, TPP_HAVE_TPP_TOK_RANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_EQUAL, TPP_HAVE_TPP_TOK_LANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_DOT_DOT_DOT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_DOT_DOT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_DOT_DOT_DOT, TPP_HAVE_TPP_TOK_DOT_DOT_DOT == -1)
#else /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_DOT_DOT(self) TPP_HAVE_TPP_TOK_DOT_DOT_DOT
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT >= 0 */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PLUS_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PLUS_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PLUS_EQUAL, TPP_HAVE_TPP_TOK_PLUS_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PLUS_EQUAL(self) TPP_HAVE_TPP_TOK_PLUS_EQUAL
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_MINUS_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_MINUS_EQUAL, TPP_HAVE_TPP_TOK_MINUS_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_EQUAL(self) TPP_HAVE_TPP_TOK_MINUS_EQUAL
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STAR_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STAR_EQUAL, TPP_HAVE_TPP_TOK_STAR_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_STAR_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_EQUAL(self) TPP_HAVE_TPP_TOK_STAR_EQUAL
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SLASH_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SLASH_EQUAL, TPP_HAVE_TPP_TOK_SLASH_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_EQUAL(self) TPP_HAVE_TPP_TOK_SLASH_EQUAL
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PERCENT_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PERCENT_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PERCENT_EQUAL, TPP_HAVE_TPP_TOK_PERCENT_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PERCENT_EQUAL(self) TPP_HAVE_TPP_TOK_PERCENT_EQUAL
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL, TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL, TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_AMP_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AMP_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_AMP_EQUAL, TPP_HAVE_TPP_TOK_AMP_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_AMP_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AMP_EQUAL(self) TPP_HAVE_TPP_TOK_AMP_EQUAL
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PIPE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PIPE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PIPE_EQUAL, TPP_HAVE_TPP_TOK_PIPE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PIPE_EQUAL(self) TPP_HAVE_TPP_TOK_PIPE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_HAT_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_HAT_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_HAT_EQUAL, TPP_HAVE_TPP_TOK_HAT_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_HAT_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_HAT_EQUAL(self) TPP_HAVE_TPP_TOK_HAT_EQUAL
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL, TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL(self) TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL, TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL(self) TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_AT_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_AT_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AT_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_AT_EQUAL, TPP_HAVE_TPP_TOK_AT_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_AT_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AT_EQUAL(self) TPP_HAVE_TPP_TOK_AT_EQUAL
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_POUND_POUND < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_POUND_POUND: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_POUND_POUND(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_POUND_POUND, TPP_HAVE_TPP_TOK_POUND_POUND == -1)
#else /* TPP_HAVE_TPP_TOK_POUND_POUND < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_POUND_POUND(self) TPP_HAVE_TPP_TOK_POUND_POUND
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND >= 0 */
#if TPP_HAVE_TPP_TOK_AMP_AMP < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_AMP_AMP: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AMP_AMP(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_AMP_AMP, TPP_HAVE_TPP_TOK_AMP_AMP == -1)
#else /* TPP_HAVE_TPP_TOK_AMP_AMP < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AMP_AMP(self) TPP_HAVE_TPP_TOK_AMP_AMP
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP >= 0 */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PIPE_PIPE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PIPE_PIPE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PIPE_PIPE, TPP_HAVE_TPP_TOK_PIPE_PIPE == -1)
#else /* TPP_HAVE_TPP_TOK_PIPE_PIPE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PIPE_PIPE(self) TPP_HAVE_TPP_TOK_PIPE_PIPE
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE >= 0 */
#if TPP_HAVE_TPP_TOK_HAT_HAT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_HAT_HAT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_HAT_HAT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_HAT_HAT, TPP_HAVE_TPP_TOK_HAT_HAT == -1)
#else /* TPP_HAVE_TPP_TOK_HAT_HAT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_HAT_HAT(self) TPP_HAVE_TPP_TOK_HAT_HAT
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT >= 0 */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PLUS_PLUS: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PLUS_PLUS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PLUS_PLUS, TPP_HAVE_TPP_TOK_PLUS_PLUS == -1)
#else /* TPP_HAVE_TPP_TOK_PLUS_PLUS < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PLUS_PLUS(self) TPP_HAVE_TPP_TOK_PLUS_PLUS
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS >= 0 */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_MINUS_MINUS: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_MINUS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_MINUS_MINUS, TPP_HAVE_TPP_TOK_MINUS_MINUS == -1)
#else /* TPP_HAVE_TPP_TOK_MINUS_MINUS < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_MINUS(self) TPP_HAVE_TPP_TOK_MINUS_MINUS
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS >= 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STAR_STAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_STAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STAR_STAR, TPP_HAVE_TPP_TOK_STAR_STAR == -1)
#else /* TPP_HAVE_TPP_TOK_STAR_STAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_STAR(self) TPP_HAVE_TPP_TOK_STAR_STAR
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR >= 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SLASH_SLASH: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_SLASH(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SLASH_SLASH, TPP_HAVE_TPP_TOK_SLASH_SLASH == -1)
#else /* TPP_HAVE_TPP_TOK_SLASH_SLASH < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_SLASH(self) TPP_HAVE_TPP_TOK_SLASH_SLASH
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH >= 0 */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_TILDE_TILDE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_TILDE_TILDE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_TILDE_TILDE, TPP_HAVE_TPP_TOK_TILDE_TILDE == -1)
#else /* TPP_HAVE_TPP_TOK_TILDE_TILDE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_TILDE_TILDE(self) TPP_HAVE_TPP_TOK_TILDE_TILDE
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE >= 0 */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_TILDE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_TILDE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_TILDE_EQUAL, TPP_HAVE_TPP_TOK_TILDE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_TILDE_EQUAL(self) TPP_HAVE_TPP_TOK_TILDE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE, TPP_HAVE_TPP_TOK_MINUS_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_RANGLE(self) TPP_HAVE_TPP_TOK_MINUS_RANGLE
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_COLON_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COLON_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_COLON_EQUAL, TPP_HAVE_TPP_TOK_COLON_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_COLON_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COLON_EQUAL(self) TPP_HAVE_TPP_TOK_COLON_EQUAL
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_COLON_COLON < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_COLON_COLON: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COLON_COLON(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_COLON_COLON, TPP_HAVE_TPP_TOK_COLON_COLON == -1)
#else /* TPP_HAVE_TPP_TOK_COLON_COLON < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COLON_COLON(self) TPP_HAVE_TPP_TOK_COLON_COLON
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON >= 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR, TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR == -1)
#else /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR(self) TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR >= 0 */
#if TPP_HAVE_TPP_TOK_DOT_STAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_DOT_STAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_STAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_DOT_STAR, TPP_HAVE_TPP_TOK_DOT_STAR == -1)
#else /* TPP_HAVE_TPP_TOK_DOT_STAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_STAR(self) TPP_HAVE_TPP_TOK_DOT_STAR
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR >= 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_DOT_DOT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_DOT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_DOT_DOT, TPP_HAVE_TPP_TOK_DOT_DOT == -1)
#else /* TPP_HAVE_TPP_TOK_DOT_DOT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_DOT(self) TPP_HAVE_TPP_TOK_DOT_DOT
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_RANGLE, TPP_HAVE_TPP_TOK_LANGLE_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_RANGLE(self) TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE, TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE(self) TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE, TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE(self) TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL, TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL, TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL, TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL(self) TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL, TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL(self) TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_QMARK_QMARK: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_QMARK_QMARK(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_QMARK_QMARK, TPP_HAVE_TPP_TOK_QMARK_QMARK == -1)
#else /* TPP_HAVE_TPP_TOK_QMARK_QMARK < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_QMARK_QMARK(self) TPP_HAVE_TPP_TOK_QMARK_QMARK
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK >= 0 */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_QMARK_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_QMARK_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_QMARK_EQUAL, TPP_HAVE_TPP_TOK_QMARK_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_QMARK_EQUAL(self) TPP_HAVE_TPP_TOK_QMARK_EQUAL
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL >= 0 */
#if TPP_HAVE_CPP_DIRECTIVES < 0
		unsigned int tff_TPP_FEAT_CPP_DIRECTIVES: 1;
#define _tpp_features_get_TPP_FEAT_CPP_DIRECTIVES(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_DIRECTIVES, TPP_HAVE_CPP_DIRECTIVES == -1)
#else /* TPP_HAVE_CPP_DIRECTIVES < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_DIRECTIVES(self) TPP_HAVE_CPP_DIRECTIVES
#endif /* TPP_HAVE_CPP_DIRECTIVES >= 0 */
#if TPP_HAVE_CPP_MACROS < 0
		unsigned int tff_TPP_FEAT_CPP_MACROS: 1;
#define _tpp_features_get_TPP_FEAT_CPP_MACROS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_MACROS, TPP_HAVE_CPP_MACROS == -1)
#else /* TPP_HAVE_CPP_MACROS < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_MACROS(self) TPP_HAVE_CPP_MACROS
#endif /* TPP_HAVE_CPP_MACROS >= 0 */
#if TPP_HAVE_CPP_EXCLAIM < 0
		unsigned int tff_TPP_FEAT_CPP_EXCLAIM: 1;
#define _tpp_features_get_TPP_FEAT_CPP_EXCLAIM(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_EXCLAIM, TPP_HAVE_CPP_EXCLAIM == -1)
#else /* TPP_HAVE_CPP_EXCLAIM < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_EXCLAIM(self) TPP_HAVE_CPP_EXCLAIM
#endif /* TPP_HAVE_CPP_EXCLAIM >= 0 */
#if TPP_HAVE_CPP_BLANK < 0
		unsigned int tff_TPP_FEAT_CPP_BLANK: 1;
#define _tpp_features_get_TPP_FEAT_CPP_BLANK(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_BLANK, TPP_HAVE_CPP_BLANK == -1)
#else /* TPP_HAVE_CPP_BLANK < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_BLANK(self) TPP_HAVE_CPP_BLANK
#endif /* TPP_HAVE_CPP_BLANK >= 0 */
#if TPP_HAVE_CPP_DIGIT_LINE < 0
		unsigned int tff_TPP_FEAT_CPP_DIGIT_LINE: 1;
#define _tpp_features_get_TPP_FEAT_CPP_DIGIT_LINE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_DIGIT_LINE, TPP_HAVE_CPP_DIGIT_LINE == -1)
#else /* TPP_HAVE_CPP_DIGIT_LINE < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_DIGIT_LINE(self) TPP_HAVE_CPP_DIGIT_LINE
#endif /* TPP_HAVE_CPP_DIGIT_LINE >= 0 */
#if TPP_HAVE_CPP_LINE < 0
		unsigned int tff_TPP_FEAT_CPP_LINE: 1;
#define _tpp_features_get_TPP_FEAT_CPP_LINE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_LINE, TPP_HAVE_CPP_LINE == -1)
#else /* TPP_HAVE_CPP_LINE < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_LINE(self) TPP_HAVE_CPP_LINE
#endif /* TPP_HAVE_CPP_LINE >= 0 */
#if TPP_HAVE_CPP_INCLUDE < 0
		unsigned int tff_TPP_FEAT_CPP_INCLUDE: 1;
#define _tpp_features_get_TPP_FEAT_CPP_INCLUDE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_INCLUDE, TPP_HAVE_CPP_INCLUDE == -1)
#else /* TPP_HAVE_CPP_INCLUDE < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_INCLUDE(self) TPP_HAVE_CPP_INCLUDE
#endif /* TPP_HAVE_CPP_INCLUDE >= 0 */
#if TPP_HAVE_CPP_INCLUDE_NEXT < 0
		unsigned int tff_TPP_FEAT_CPP_INCLUDE_NEXT: 1;
#define _tpp_features_get_TPP_FEAT_CPP_INCLUDE_NEXT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_INCLUDE_NEXT, TPP_HAVE_CPP_INCLUDE_NEXT == -1)
#else /* TPP_HAVE_CPP_INCLUDE_NEXT < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_INCLUDE_NEXT(self) TPP_HAVE_CPP_INCLUDE_NEXT
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT >= 0 */
#if TPP_HAVE_CPP_IMPORT < 0
		unsigned int tff_TPP_FEAT_CPP_IMPORT: 1;
#define _tpp_features_get_TPP_FEAT_CPP_IMPORT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_IMPORT, TPP_HAVE_CPP_IMPORT == -1)
#else /* TPP_HAVE_CPP_IMPORT < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_IMPORT(self) TPP_HAVE_CPP_IMPORT
#endif /* TPP_HAVE_CPP_IMPORT >= 0 */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF < 0
		unsigned int tff_TPP_FEAT_CPP_IF_ELSE_ENDIF: 1;
#define _tpp_features_get_TPP_FEAT_CPP_IF_ELSE_ENDIF(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_IF_ELSE_ENDIF, TPP_HAVE_CPP_IF_ELSE_ENDIF == -1)
#else /* TPP_HAVE_CPP_IF_ELSE_ENDIF < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_IF_ELSE_ENDIF(self) TPP_HAVE_CPP_IF_ELSE_ENDIF
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF >= 0 */
#if TPP_HAVE_CPP_DEFINE < 0
		unsigned int tff_TPP_FEAT_CPP_DEFINE: 1;
#define _tpp_features_get_TPP_FEAT_CPP_DEFINE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_DEFINE, TPP_HAVE_CPP_DEFINE == -1)
#else /* TPP_HAVE_CPP_DEFINE < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_DEFINE(self) TPP_HAVE_CPP_DEFINE
#endif /* TPP_HAVE_CPP_DEFINE >= 0 */
#if TPP_HAVE_CPP_ASSERT < 0
		unsigned int tff_TPP_FEAT_CPP_ASSERT: 1;
#define _tpp_features_get_TPP_FEAT_CPP_ASSERT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_ASSERT, TPP_HAVE_CPP_ASSERT == -1)
#else /* TPP_HAVE_CPP_ASSERT < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_ASSERT(self) TPP_HAVE_CPP_ASSERT
#endif /* TPP_HAVE_CPP_ASSERT >= 0 */
#if TPP_HAVE_CPP_ERROR < 0
		unsigned int tff_TPP_FEAT_CPP_ERROR: 1;
#define _tpp_features_get_TPP_FEAT_CPP_ERROR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_ERROR, TPP_HAVE_CPP_ERROR == -1)
#else /* TPP_HAVE_CPP_ERROR < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_ERROR(self) TPP_HAVE_CPP_ERROR
#endif /* TPP_HAVE_CPP_ERROR >= 0 */
#if TPP_HAVE_CPP_WARNING < 0
		unsigned int tff_TPP_FEAT_CPP_WARNING: 1;
#define _tpp_features_get_TPP_FEAT_CPP_WARNING(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_WARNING, TPP_HAVE_CPP_WARNING == -1)
#else /* TPP_HAVE_CPP_WARNING < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_WARNING(self) TPP_HAVE_CPP_WARNING
#endif /* TPP_HAVE_CPP_WARNING >= 0 */
#if TPP_HAVE_CPP_IDENT < 0
		unsigned int tff_TPP_FEAT_CPP_IDENT: 1;
#define _tpp_features_get_TPP_FEAT_CPP_IDENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_IDENT, TPP_HAVE_CPP_IDENT == -1)
#else /* TPP_HAVE_CPP_IDENT < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_IDENT(self) TPP_HAVE_CPP_IDENT
#endif /* TPP_HAVE_CPP_IDENT >= 0 */
#if TPP_HAVE_CPP_SCSS < 0
		unsigned int tff_TPP_FEAT_CPP_SCSS: 1;
#define _tpp_features_get_TPP_FEAT_CPP_SCSS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_SCSS, TPP_HAVE_CPP_SCSS == -1)
#else /* TPP_HAVE_CPP_SCSS < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_SCSS(self) TPP_HAVE_CPP_SCSS
#endif /* TPP_HAVE_CPP_SCSS >= 0 */
#if TPP_HAVE_CPP_PRAGMA < 0
		unsigned int tff_TPP_FEAT_CPP_PRAGMA: 1;
#define _tpp_features_get_TPP_FEAT_CPP_PRAGMA(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_PRAGMA, TPP_HAVE_CPP_PRAGMA == -1)
#else /* TPP_HAVE_CPP_PRAGMA < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_PRAGMA(self) TPP_HAVE_CPP_PRAGMA
#endif /* TPP_HAVE_CPP_PRAGMA >= 0 */
#if TPP_HAVE_STDC_PRAGMA < 0
		unsigned int tff_TPP_FEAT_STDC_PRAGMA: 1;
#define _tpp_features_get_TPP_FEAT_STDC_PRAGMA(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_STDC_PRAGMA, TPP_HAVE_STDC_PRAGMA == -1)
#else /* TPP_HAVE_STDC_PRAGMA < 0 */
#define _tpp_features_get_TPP_FEAT_STDC_PRAGMA(self) TPP_HAVE_STDC_PRAGMA
#endif /* TPP_HAVE_STDC_PRAGMA >= 0 */
#if TPP_HAVE_MSVC_PRAGMA < 0
		unsigned int tff_TPP_FEAT_MSVC_PRAGMA: 1;
#define _tpp_features_get_TPP_FEAT_MSVC_PRAGMA(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_MSVC_PRAGMA, TPP_HAVE_MSVC_PRAGMA == -1)
#else /* TPP_HAVE_MSVC_PRAGMA < 0 */
#define _tpp_features_get_TPP_FEAT_MSVC_PRAGMA(self) TPP_HAVE_MSVC_PRAGMA
#endif /* TPP_HAVE_MSVC_PRAGMA >= 0 */
	} tf_flags;
	unsigned char ttf_bitset[TPP_FEAT_COUNT ? ((TPP_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_features;

TPP_CONST_DECL tpp_features const tpp_features_default;

#define tpp_features_getid(self, id) \
	((self)->ttf_bitset[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_features_enable(self, id) \
	(void)((self)->ttf_bitset[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_features_disable(self, id) \
	(void)((self)->ttf_bitset[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_features_setid(self, id, enabled) \
	((enabled) ? tpp_features_enable(self, id) : tpp_features_disable(self, id))
#endif /* TPP_HAVE_FEATURES */

#if TPP_HAVE_FEATURES
#define tpp_features_init(self) (void)(*(self) = tpp_features_default)
#define tpp_features_fini(self) (void)0
#define tpp_features_get(self, TPP_FEAT_x) _tpp_features_get_##TPP_FEAT_x(self)
#else /* TPP_HAVE_FEATURES */
#define tpp_features_get(self, TPP_FEAT_x) 1
#endif /* !TPP_HAVE_FEATURES */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/file.h                                                   */
/************************************************************************/
TPP_DECL_BEGIN

typedef enum tpp_file_kind {
	TPP_FILE_KIND_IO,    /* File found on hard-disk */
	TPP_FILE_KIND_TEXT,  /* Explicitly injected text (same as `TPP_FILE_KIND_IO', but single-chunk'd) */
#if TPP_HAVE_CPP_MACROS
	TPP_FILE_KIND_MACRO, /* Expanded macro */
#endif /* TPP_HAVE_CPP_MACROS */
} tpp_file_kind;

#if TPP_HAVE_UNICODE
typedef enum tpp_file_encoding {
	TPP_FILE_ENCODING_UTF8,       /* Assume utf-8 until an invalid byte sequence is encountered (when that happens, set to `TPP_FILE_ENCODING_ASCII') */
	TPP_FILE_ENCODING_ASCII,      /* An invalid utf-8 byte sequence is encountered */
	TPP_FILE_ENCODING_FORCE_UTF8, /* Always use utf-8 (used when a BOM was encountered at the start of the file) */

	/* The following are only used internally and (despite their names) must be treated as "TPP_FILE_ENCODING_FORCE_UTF8" */
	TPP_FILE_ENCODING_UTF16_LE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-16 (le) and auto-converted to utf-8 during reading */
	TPP_FILE_ENCODING_UTF16_BE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-16 (be) and auto-converted to utf-8 during reading */
	TPP_FILE_ENCODING_UTF32_LE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-32 (le) and auto-converted to utf-8 during reading */
	TPP_FILE_ENCODING_UTF32_BE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-32 (be) and auto-converted to utf-8 during reading */
} tpp_file_encoding;
#define TPP_FILE_ENCODING_ISUTF8(enc)  ((enc) != TPP_FILE_ENCODING_ASCII)
#define TPP_FILE_ENCODING_ISASCII(enc) ((enc) == TPP_FILE_ENCODING_ASCII)
#endif /* TPP_HAVE_UNICODE */


#undef TPP_HAVE_FILE_IOFLAGS
#if (TPP_HAVE_FILE_NONBLOCK || \
     TPP_HAVE_FILE_NOCLOSE ||  \
     TPP_HAVE_FILE_SYSHDR ||   \
     TPP_HAVE_FILE_NOKWD)
#define TPP_HAVE_FILE_IOFLAGS 1
#else /* ... */
#define TPP_HAVE_FILE_IOFLAGS 0
#endif /* !... */

#if TPP_HAVE_FILE_IOFLAGS
#define tpp_file_ioflags uint_least8_t /* Set of `TPP_FILE_IOFLAGS_*' */
#define TPP_FILE_IOFLAGS_NORMAL   UINT8_C(0x00) /* Normal flags */
#if TPP_HAVE_FILE_NONBLOCK
#define TPP_FILE_IOFLAGS_NONBLOCK UINT8_C(0x01) /* Do non-blocking I/O */
#endif /* TPP_HAVE_FILE_NONBLOCK */
#if TPP_HAVE_FILE_NOCLOSE
#define TPP_FILE_IOFLAGS_NOCLOSE  UINT8_C(0x02) /* Don't `tpp_io_close(tff_file)' on destruction */
#endif /* TPP_HAVE_FILE_NOCLOSE */
#if TPP_HAVE_FILE_SYSHDR
#define TPP_FILE_IOFLAGS_SYSHDR   UINT8_C(0x04) /* Suppress all warnings produced in the context of this file */
#endif /* TPP_HAVE_FILE_SYSHDR */
#if TPP_HAVE_FILE_NOKWD
#define TPP_FILE_IOFLAGS_NOKWD    UINT8_C(0x08) /* The file's "tff_name" field isn't actually a "tpp_keyword", but rather a raw \0-terminated C string. */
#endif /* TPP_HAVE_FILE_NOKWD */
#endif /* TPP_HAVE_FILE_IOFLAGS */


struct tpp_keyword;
#if TPP_HAVE_CPP_MACROS
struct tpp_macro;
#endif /* TPP_HAVE_CPP_MACROS */

typedef struct tpp_file {
	/* Important: "tf_pos" and "tf_chunk" must come first, so they can shadow the tail of "tpp_token" */
	tpp_char const     *tf_pos;   /* [0..1][<= tf_end] File pointer to next unread byte. */
	TPP_REF tpp_string *tf_chunk; /* [0..1][const_if(tf_kind != TPP_FILE_KIND_IO)] Currently loaded text-chunk (mutable for text-files) */
	tpp_char const     *tf_end;   /* [0..1][>= tf_chunk->ts_str && <= tf_chunk->ts_str+tf_chunk->ts_len][const_if(tf_kind != TPP_FILE_KIND_IO)] End of effective file content (mutable for text-files) */
#if TPP_HAVE_INCLUDE_STACK
	struct tpp_file    *tf_prev;  /* [0..1] Parent file in #include stack */
#endif /* TPP_HAVE_INCLUDE_STACK */
#if TPP_HAVE_FILE_LC_CACHE
	tpp_char const     *tf_lcpos; /* [0..1] Position that `tf_lcval' applies to. */
	tpp_lcinfo          tf_lcval; /* [valid_if(tf_lcpos)] Cached line/column at `tf_lcpos' */
#endif /* TPP_HAVE_FILE_LC_CACHE */
	tpp_file_kind       tf_kind;  /* [const] File kind */
#if TPP_HAVE_UNICODE
	tpp_file_encoding   tf_enc;   /* File encoding */
#endif /* TPP_HAVE_UNICODE */
	union {
		struct {
			struct tpp_keyword *tff_name;     /* [0..1][const] Filename by which this file was included (if available) */
			tpp_io_handle       tff_file;     /* [owned_if(!TPP_FILE_IOFLAGS_NOCLOSE)] Underlying I/O file (set to tpp_io_handle_INVALID after EOF) */
			tpp_lcinfo          tff_start_lc; /* [valid_if(tf_chunk != NULL)] Line/Column numbers (0-based) of `tf_chunk->ts_str' */
#if TPP_HAVE_FILE_IOFLAGS
			tpp_file_ioflags    tff_flags;    /* File flags (set of `TPP_FILE_IOFLAGS_*') */
#endif /* TPP_HAVE_FILE_IOFLAGS */
#if TPP_HAVE_UNICODE
			uint_least8_t tff_tailc;    /* [valid_if(tf_enc) == TPP_FILE_ENCODING_UTF[16|32]_[LE|BE]] Read, unaligned tail data */
			unsigned char tff_tailv[3]; /* [valid_if(tf_enc) == TPP_FILE_ENCODING_UTF[16|32]_[LE|BE]] Read, unaligned tail data */
#endif /* TPP_HAVE_UNICODE */
		} td_io; /* [tf_kind == TPP_FILE_KIND_IO] */

		struct {
			char const *tft_name; /* [0..1][const] Filename for messages (if available) */
		} td_text; /* [tf_kind == TPP_FILE_KIND_TEXT] */

#if TPP_HAVE_CPP_MACROS
		struct {
			TPP_REF struct tpp_macro *tfm_macro; /* [1..1][const] The macro definition that produced this file as its expansion */
		} td_macro; /* [tf_kind == TPP_FILE_KIND_MACRO] */
#endif /* TPP_HAVE_CPP_MACROS */
	} tf_data;
} tpp_file;


#if TPP_HAVE_INCLUDE_STACK
#define _tpp_file_init_prev(self) , (self)->tf_prev = NULL
#else /* TPP_HAVE_INCLUDE_STACK */
#define _tpp_file_init_prev(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_STACK */

#if TPP_HAVE_FILE_LC_CACHE
#define _tpp_file_init_lc(self) , (self)->tf_lcpos = NULL
#else /* TPP_HAVE_FILE_LC_CACHE */
#define _tpp_file_init_lc(self) /* nothing */
#endif /* !TPP_HAVE_FILE_LC_CACHE */

#if TPP_HAVE_UNICODE
#define _tpp_file_init_enc(self)       , (self)->tf_enc = TPP_FILE_ENCODING_UTF8
#define _tpp_file_init_enc_ex(self, v) , (self)->tf_enc = v
#else /* TPP_HAVE_UNICODE */
#define _tpp_file_init_enc(self)       /* nothing */
#define _tpp_file_init_enc_ex(self, v) /* nothing */
#endif /* !TPP_HAVE_UNICODE */

#if TPP_HAVE_FILE_IOFLAGS
#define _tpp_file_init_ioflags(self, flags) , (self)->tf_data.td_io.tff_flags = (flags)
#else /* TPP_HAVE_FILE_SYSHDR */
#define _tpp_file_init_ioflags(self, flags) /* nothing */
#endif /* !TPP_HAVE_FILE_SYSHDR */

#if TPP_HAVE_UNICODE
#define tpp_file_isutf8(self)  TPP_FILE_ENCODING_ISUTF8((self)->tf_enc)
#define tpp_file_isascii(self) TPP_FILE_ENCODING_ISASCII((self)->tf_enc)
#else /* TPP_HAVE_UNICODE */
#define tpp_file_isutf8(self)  0
#define tpp_file_isascii(self) 1
#endif /* !TPP_HAVE_UNICODE */



/* Initialize "self " as a "TPP_FILE_KIND_IO" file
 * @param: tpp_keyword     *filename: [0..1] Filename (if known)
 * @param: tpp_io_handle    fp:       File descriptor (inherited)
 * @param: tpp_file_ioflags flags:    I/O file flags (set of `TPP_FILE_IOFLAGS_*') */
#define tpp_file_init_io(self, filename, /*inherit*/ fp) \
	tpp_file_init_io_ex(self, filename, fp, TPP_FILE_IOFLAGS_NORMAL)
#define tpp_file_init_io_ex(self, filename, /*inherit*/ fp, flags)   \
	(void)((self)->tf_pos   = NULL,                                  \
	       (self)->tf_chunk = NULL,                                  \
	       (self)->tf_end   = NULL                                   \
	       _tpp_file_init_prev(self)                                 \
	       _tpp_file_init_lc(self),                                  \
	       (self)->tf_kind = TPP_FILE_KIND_IO                        \
	       _tpp_file_init_enc(self),                                 \
	       (self)->tf_data.td_io.tff_name = (filename),              \
	       (self)->tf_data.td_io.tff_file = (fp),                    \
	       tpp_lcinfo_init((self)->tf_data.td_io.tff_start_lc, 0, 0) \
	       _tpp_file_init_ioflags(self, flags))



/* Initialize "self " as a "TPP_FILE_KIND_TEXT" file
 * @param: char const       *filename:  [0..1] Filename (if known)
 * @param: void const       *text:      File data base pointer
 * @param: tpp_size          text_size: File data size
 * @param: tpp_file_encoding encoding:  File data encoding */
#define tpp_file_init_text_ascii(self, filename, text, text_size) \
	tpp_file_init_text_ex(self, filename, text, TPP_FILE_ENCODING_ASCII)
#define tpp_file_init_text_ex(self, filename, text, text_size, encoding) \
	(void)((self)->tf_pos   = (tpp_char const *)(text),                  \
	       (self)->tf_chunk = NULL,                                      \
	       (self)->tf_end   = (tpp_char const *)(text) + (text_size)     \
	       _tpp_file_init_prev(self)                                     \
	       _tpp_file_init_lc(self),                                      \
	       (self)->tf_kind = TPP_FILE_KIND_TEXT                          \
	       _tpp_file_init_enc_ex(self, encoding),                        \
	       (self)->tf_data.td_text.tft_name = (filename))



/* Finalize the given file. */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_file_fini(tpp_file *tpp_restrict self);


/* Try to expand the currently loaded `self->tf_chunk':
 * - If the file's kind isn't `TPP_FILE_KIND_IO', return `TPP_EOK'
 * - Allocate a new `struct tpp_string' suitable for holding both
 *   [tf_pos,tf_end), as well as at least 1 additional byte.
 *   HINT: When `!tpp_string_isshared(self->tf_chunk)', the old
 *         chunk string may simply be re-used (since it'd be free'd
 *         during the re-assignment below anyways). If this isn't
 *         intended (iow: you want to keep the contents of the previous
 *         chunk loaded into memory), you can simply `tpp_string_incref'
 *         it before calling this function.
 * - Copy [tf_pos,tf_end) into this new string
 * - Read from the underlying file into the tail of the new string
 * #if TPP_HAVE_UNICODE
 *   - If the underlying file's encoding is TPP_FILE_ENCODING_UTF(16|32)_(LE|BE),
 *     the read data is converted to utf-8 at this point.
 * #endif // TPP_HAVE_UNICODE
 *   - If the underlying file could not be read, return `TPP_EIO'
 *   - If nothing could be read, free the new string and return `TPP_EOK'
 *   - Else:
 *     - adjust `tf_pos' to point into the new string, and
 *       set `tf_end' to point at the end of the new string.
 *     - replace `tf_chunk' with the new string
 *     - return `TPP_EOK'
 * @return: TPP_EOK: Either the current chunk was expanded (the delta
 *                   between `tf_pos' and `tf_end' has increased), or
 *                   no further data can be read from `self'.
 * @return: TPP_EIO: I/O error
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: TPP_EWOULDBLOCK: Operation would block.
 * #endif // TPP_HAVE_FILE_NONBLOCK */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_file_expandchunk(tpp_file *tpp_restrict self);

/* Encode/decode pointer<=>text-offset such that effective positions
 * are retained across calls to `tpp_file_expandchunk()'.
 *
 * NOTE: This needs to use "tf_pos" as relative base, since the start
 *       of the currently loaded chunk can change if another chunk is
 *       allocated that doesn't include the already-read buffer area
 *       located in [tf_chunk->ts_str,tf_pos) */
#define tpp_file_ptr2rel(self, ptr) (tpp_size)((ptr) - (self)->tf_pos)
#define tpp_file_rel2ptr(self, rel) ((self)->tf_pos + (rel))



/* Return line/column information (1-based) for "pos" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_lcinfo TPPCALL
tpp_file_lcinfo(tpp_file *tpp_restrict self,
                tpp_char const *pos);

/* Returns the filename of "self", or "NULL" if unknown. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_filename(tpp_file const *tpp_restrict self);

/* Returns the filename "keyword" (which may not always be
 * available, even when "tpp_file_filename()" returns non-NULL) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) struct tpp_keyword *TPPCALL
tpp_file_filename_kwd(tpp_file const *tpp_restrict self);

/* Returns the first tf_kind=TPP_FILE_KIND_IO file in the #include-stack
 * If no such file exists, simply re-return "self". This function never
 * returns "NULL" */
#if TPP_HAVE_INCLUDE_STACK
TPP_DECL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getiofile(tpp_file const *tpp_restrict self);
#else /* TPP_HAVE_INCLUDE_STACK */
#define tpp_file_getiofile(self) (self)
#endif /* !TPP_HAVE_INCLUDE_STACK */

/* Size of \t as reported by `tpp_file_lcinfo()' */
#if TPP_TABSIZE >= 0
#define tpp_gettabsize() TPP_TABSIZE
#else /* TPP_TABSIZE >= 0 */
TPP_DECL tpp_column tpp_tabsize;
#define tpp_gettabsize()  tpp_tabsize
#define tpp_settabsize(v) (void)(tpp_tabsize = (v))
#endif /* TPP_TABSIZE < 0 */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/macro.h                                                  */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS

#define tpp_macro_kind uint_least8_t
#define TPP_MACRO_KIND_KEYWORD      UINT8_C(0)
#define TPP_MACRO_KIND_FUNC_PAREN   '('
#define TPP_MACRO_KIND_ISFUNC(kind) ((kind) != TPP_MACRO_KIND_KEYWORD)
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_MACRO_KIND_FUNC_BRACKET '['
#define TPP_MACRO_KIND_FUNC_BRACE   '{'
#define TPP_MACRO_KIND_FUNC_ANGLE   '<'
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

#undef TPP_HAVE_MACRO_FLAGS
#if (TPP_HAVE_NAMED_VARARGS_IN_MACROS || \
     TPP_HAVE_VA_ARGS_IN_MACROS ||       \
     TPP_HAVE_MACRO_RECURSION ||         \
     TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
#define TPP_HAVE_MACRO_FLAGS 1
#else /* ... */
#define TPP_HAVE_MACRO_FLAGS 0
#endif /* !... */


#if TPP_HAVE_MACRO_FLAGS
#define tpp_macro_flag uint_least8_t /* Set of `TPP_MACRO_FLAG_*' */
#define TPP_MACRO_FLAG_NORMAL     UINT8_C(0x00) /* Normal flags */
#if TPP_HAVE_NAMED_VARARGS_IN_MACROS || TPP_HAVE_VA_ARGS_IN_MACROS
#define TPP_MACRO_FLAG_VARIADIC   UINT8_C(0x01) /* The last argument of the function is variadic. */
#endif /* TPP_HAVE_NAMED_VARARGS_IN_MACROS || TPP_HAVE_VA_ARGS_IN_MACROS */
#if TPP_HAVE_MACRO_RECURSION
#define TPP_MACRO_FLAG_SELFEXPAND UINT8_C(0x02) /* After being expanded, this function is allowed to re-invoke itself and be expanded, when
                                                 * the generated text is not identical to a previous iteration. (s.a.: `-fmacro-recursion') */
#endif /* TPP_HAVE_MACRO_RECURSION */
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
#define TPP_MACRO_FLAG_KEEPARGSPC UINT8_C(0x04) /* When set, keep whitespace surrounding macro arguments during invocation.
                                                 * WARNING: Also affects recursive macro expansion. */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */
#endif /* TPP_HAVE_MACRO_FLAGS */


typedef struct tpp_macro_argument {
	tpp_token_id tma_id;      /* [const] Token ID associated with this argument name. */
	tpp_size     tma_ins;     /* [const] Amount of times the argument is inserted without expansion. */
	tpp_size     tma_ins_exp; /* [const] Amount of times the argument is inserted after expansion. */
	tpp_size     tma_ins_str; /* [const] Amount of times the argument is inserted in its escaped form. */
#if TPP_CONFIG_DEBUG
	tpp_char    *tma_name;    /* [1..1][const] Name of this macro (aliases `struct tpp_keyword::tk_kwd'). */
#endif /* TPP_CONFIG_DEBUG */
} tpp_macro_argument;

struct tpp_keyword;
typedef struct tpp_macro {
	tpp_refcnt          tm_refcnt;     /* Reference count */
	tpp_macro_kind      tm_kind;       /* [const] Macro kind (one of `TPP_MACRO_KIND_*') */
#if TPP_HAVE_MACRO_FLAGS
	tpp_macro_flag      tm_flags;      /* [const] Macro flags (set of `TPP_MACRO_FLAG_*') */
#endif /* TPP_HAVE_MACRO_FLAGS */
	TPP_REF tpp_string *tm_body_chunk; /* [0..1][const] Data-chunk containing the macro's body (or "NULL" if static or originating from file with "tf_chunk == NULL") */
	tpp_char const     *tm_body_start; /* [1..1][const] Pointer to start of macro body */
	tpp_char const     *tm_body_end;   /* [1..1][const] Pointer to end of macro body */
	tpp_size            tm_expansions; /* The amount of existing expansions of this macro.
	                                    * NOTE: Depending on the `TPP_MACRO_FLAG_SELFEXPAND' flag,
	                                    *       this value may not be allowed to exceed one(1). */
	struct tpp_keyword *tm_deffile;    /* [0..1][const] File in which this macro was defined (or "NULL" if unknown / custom definition) */
	tpp_lcinfo          tm_deflc;      /* [const][valid_if(tm_deffile != NULL)] Macro name line/column */
	tpp_lcinfo          tm_body_lc;    /* [const][valid_if(tm_deffile != NULL)] Macro body line/column */
	union {
		struct {
			tpp_size            tmf_deltotal;   /* [const][<= (:tm_body_start-:tm_body_end)] The total amount of characters removed during expansion (minus those added). */
			tpp_size            tmf_argc;       /* [const] Amount of arguments this macro-function takes */
			tpp_macro_argument *tmf_argv;       /* [const][0..f_argc][owned] Vector of argument information (used for fast calculation of the expanded macro's size) */
#if TPP_HAVE_VA_COMMA_IN_MACROS||TPP_HAVE_VA_OPT_IN_MACROS
			tpp_size            tmf_n_vacomma;  /* [const] Amount of times `__VA_COMMA__' is used in `tmf_expand' + amount of characters potentially inserted by `__VA_OPT__'. */
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS|| TPP_HAVE_VA_OPT_IN_MACROS */
#ifdef TPP_HAVE_VA_NARGS_IN_MACROS
			tpp_size            tmf_n_vanargs;  /* [const] Amount of times `__VA_NARGS__' is used in `tmf_expand'. */
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS*/
			void               *tmf_argbuf;     /* [0..1][owned] Internal preallocated cache for a required temporary buffer used during expansion.
			                                     * NOTE: Implementation-wise, this is a vector of `argcache_t' (an internal, hidden data structure). */
			unsigned char       tmf_expand[TPP_FLEX_ARRAY]; /* [const][1..1] Instruction sequence to expand this macro */
		} tmd_func; /* [TPP_MACRO_KIND_ISFUNC(tm_kind)] */
	} tm_data;
} tpp_macro;

TPP_DECL TPP_NONNULL((1)) void TPPCALL tpp_macro_destroy(tpp_macro *tpp_restrict self);
#define tpp_macro_isshared(self) tpp_refcnt_isshared(&(self)->tm_refcnt)
#define tpp_macro_incref(self)   tpp_refcnt_inc(&(self)->tm_refcnt)
#define tpp_macro_decref(self)   (void)(tpp_refcnt_decfetch(&(self)->tm_refcnt) || (tpp_macro_destroy(self), 0))


#if TPP_BUILDING
/* Figure out the line/column of "pos" in "expanded_text", as produced
 * by "self", which must be "TPP_MACRO_KIND_ISFUNC(self->tm_kind)". */
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_lcinfo TPPCALL
tpp_macro_func_lcinfo(tpp_macro const *tpp_restrict self,
                      tpp_string const *expanded_text,
                      tpp_char const *pos);
#endif /* TPP_BUILDING */

#endif /* TPP_HAVE_CPP_MACROS */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/keyword.h                                                */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_PRAGMA_PUSH_MACRO
struct tpp_macro;
typedef struct tpp_macro_pushent {
	TPP_REF struct tpp_macro *tmpe_macro; /* [0..1] The macro that was pushed, or "NULL" if not defined at the time. */
	tpp_size                  tmpe_count; /* # of times that `tmpe_macro' was pushed without the macro actually having changed */
} tpp_macro_pushent;
typedef struct tpp_macro_pushstack {
	tpp_size           tmps_cnt; /* # of elements on `tmps_vec' */
	tpp_macro_pushent *tmps_vec; /* [0..tmps_vec][owned] Vector of pushed macros (push_macro appends at the end; pop_macro takes from the end) */
} tpp_macro_pushstack;

#define tpp_macro_pushstack_init(self) \
	(void)((self)->tmps_cnt = 0, (self)->tmps_vec = NULL)
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_macro_pushstack_fini(tpp_macro_pushstack *tpp_restrict self);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#undef TPP_HAVE_KEYWORD_FLAGS
#if ((TPP_HAVE_CPP_INCLUDE && (TPP_HAVE_CPP_IF_ELSE_ENDIF || \
                               TPP_HAVE_PRAGMA_ONCE)) ||     \
     TPP_HAVE_CPP_IMPORT ||                                  \
     TPP_HAVE_CLANG_HAS_ATTRIBUTE ||                         \
     TPP_HAVE_CLANG_HAS_BUILTIN ||                           \
     TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE ||                     \
     TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE ||                \
     TPP_HAVE_CLANG_HAS_EXTENSION ||                         \
     TPP_HAVE_CLANG_HAS_FEATURE ||                           \
     TPP_HAVE_PRAGMA_DEPRECATED ||                           \
     TPP_HAVE_PRAGMA_GCC_POISON ||                           \
     TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS)
#define TPP_HAVE_KEYWORD_FLAGS 1
#else /* ... */
#define TPP_HAVE_KEYWORD_FLAGS 0
#endif /* !... */

#undef TPP_HAVE_KEYWORD_MISC
#if (TPP_HAVE_CPP_INCLUDE ||       \
     TPP_HAVE_PRAGMA_PUSH_MACRO || \
     TPP_HAVE_KEYWORD_FLAGS)
#define TPP_HAVE_KEYWORD_MISC 1
#else /* ... */
#define TPP_HAVE_KEYWORD_MISC 0
#endif /* !... */



#if TPP_HAVE_KEYWORD_MISC

/* Keyword flags... */
#if TPP_HAVE_KEYWORD_FLAGS
#define tpp_keyword_flags uint_least32_t /* Set of `TPP_KEYWORD_FLAG_*' */
#define TPP_KEYWORD_FLAG_NORMAL                 UINT32_C(0x00000000) /* Normal flags */
#if TPP_HAVE_CLANG_HAS_ATTRIBUTE
#define TPP_KEYWORD_FLAG_HAS_ATTRIBUTE          UINT32_C(0x00000001) /* `__has_attribute()' */
#endif /* TPP_HAVE_CLANG_HAS_ATTRIBUTE */
#if TPP_HAVE_CLANG_HAS_BUILTIN
#define TPP_KEYWORD_FLAG_HAS_BUILTIN            UINT32_C(0x00000002) /* `__has_builtin()' */
#endif /* TPP_HAVE_CLANG_HAS_BUILTIN */
#if TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE
#define TPP_KEYWORD_FLAG_HAS_CPP_ATTRIBUTE      UINT32_C(0x00000004) /* `__has_cpp_attribute()' */
#endif /* TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE */
#if TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE
#define TPP_KEYWORD_FLAG_HAS_DECLSPEC_ATTRIBUTE UINT32_C(0x00000008) /* `__has_declspec_attribute()' */
#endif /* TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE */
#if TPP_HAVE_CLANG_HAS_EXTENSION
#define TPP_KEYWORD_FLAG_HAS_EXTENSION          UINT32_C(0x00000010) /* `__has_extension()' */
#endif /* TPP_HAVE_CLANG_HAS_EXTENSION */
#if TPP_HAVE_CLANG_HAS_FEATURE
#define TPP_KEYWORD_FLAG_HAS_FEATURE            UINT32_C(0x00000020) /* `__has_feature()' */
#endif /* TPP_HAVE_CLANG_HAS_FEATURE */
#if TPP_HAVE_PRAGMA_DEPRECATED
#define TPP_KEYWORD_FLAG_IS_DEPRECATED          UINT32_C(0x00000040) /* Warn when the keyword appears as the result of lexical processing. */
#endif /* TPP_HAVE_PRAGMA_DEPRECATED */
#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_KEYWORD_FLAG_USERMASK               UINT32_C(0x0000007f) /* Set of flags modifiable through `#pragma tpp_set_keyword_flags()'. */
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */
#if TPP_HAVE_PRAGMA_GCC_POISON
#define TPP_KEYWORD_FLAG_IS_POISONED            UINT32_C(0x00000080) /* Extension for `TPP_KEYWORD_FLAG_IS_DEPRECATED':
                                                                      * Don't emit a warning if the keyword is used inside of a macro.
                                                                      * -> Only warn if it is used from a text file. */
#endif /* TPP_HAVE_PRAGMA_GCC_POISON */
#if TPP_HAVE_CPP_IMPORT
#define TPP_KEYWORD_FLAG_HDR_IMPORTED           UINT32_C(0x20000000) /* Set after this header was `#import'-ed */
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_CPP_INCLUDE
#if TPP_HAVE_PRAGMA_ONCE
#define TPP_KEYWORD_FLAG_HDR_ONCE               UINT32_C(0x40000000) /* Set after `#pragma once' was encountered */
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
#define TPP_KEYWORD_FLAG_HDR_NOGUARD            UINT32_C(0x80000000) /* Set after a secondary #ifdef block was detected at the top level of this file:
                                                                      * >> // File: "myfile.h"
                                                                      * >> #ifndef foo // Potential include-guard?
                                                                      * >> #endif
                                                                      * >> #ifndef bar // This #ifndef will set `tff_noguard=1'
                                                                      * >> #endif */
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
#endif /* TPP_HAVE_CPP_INCLUDE */
#endif /* TPP_HAVE_KEYWORD_FLAGS */

struct tpp_keyword;
typedef struct tpp_keyword_misc {
#if TPP_HAVE_KEYWORD_FLAGS
	tpp_keyword_flags   tkm_flags; /* Set of `TPP_KEYWORD_FLAG_*' */
#endif /* TPP_HAVE_KEYWORD_FLAGS */
#if TPP_HAVE_KEYWORD_FILE_GUARD
	struct tpp_keyword *tkm_file_guard; /* [0..1] Name of the #include guard for this file, or NULL if unknown. */
#endif /* TPP_HAVE_KEYWORD_FILE_GUARD */
#if TPP_HAVE_PRAGMA_PUSH_MACRO
	tpp_macro_pushstack tkm_macro_pushstack; /* For `#pragma push_macro()' */
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
} tpp_keyword_misc;
#endif /* TPP_HAVE_KEYWORD_MISC */


#if TPP_HAVE_CPP_MACROS
struct tpp_macro;
#endif /* TPP_HAVE_CPP_MACROS */

#undef TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
typedef struct tpp_keyword {
	tpp_token_id              tk_id;                  /* [const] Keyword ID */
	struct tpp_keyword       *tk_next;                /* [0..1] Next keyword with a similar hash */
#if TPP_HAVE_CPP_MACROS
	TPP_REF struct tpp_macro *tk_macro;               /* [0..1][const_if(IS_BUILTIN)] Macro definition */
#define TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS 1
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	tpp_keyword_misc         *tk_misc;                /* [0..1][const_if(IS_BUILTIN)][owned] Misc. keyword data (lazily allocated) */
#define TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS 1
#endif /* TPP_HAVE_KEYWORD_MISC */
	tpp_hash                  tk_hash;                /* [const] Hash for `tk_kwd' */
	tpp_refcnt                tk_refcnt;              /* Keyword reference count (for binary compatibility with "tpp_string") */
	tpp_size                  tk_len;                 /* [const] # of bytes (char-s) in `tk_kwd' (excluding trailing \0) */
	tpp_char                  tk_kwd[TPP_FLEX_ARRAY]; /* [const][tk_len] Keyword string (in input encoding; \0-terminated; never contains \-escaped linefeeds) */
/*	tpp_char                  tk_nul;                  * [const][== 0] Ensure ZERO-termination of the keyword name. */
} tpp_keyword;

#define tpp_keyword_sizeof(len) (tpp_offsetof(tpp_keyword, tk_kwd) + ((len) + 1) * sizeof(tpp_char))

/* When true, there are certain actions that require builtin keywords
 * to be copied into the current lexer's keyword table. These include
 * user-defined macros (with built-in identifiers as names), as well
 * as any other "misc"-related, custom data being assigned ot keywords */
#ifndef TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
#define TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS 0
#endif /* !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */

/* Use this macro for comparing keywords instead of doing "a == b"
 * We need to compare the IDs of keywords, since builtin keywords
 * may need to be copied into the current lexer's `tpp_keywords'
 * if `tk_macro' or `tk_misc' need to be modified */
#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
#define tpp_keyword_equals(a, b) ((a)->tk_id == (b)->tk_id)
#else /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */
#define tpp_keyword_equals(a, b) ((a) == (b))
#endif /* !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */

/* Convert back-and-forth between keywords and strings */
#define _TPP_KEYWORD_STRING_ABI_START tk_refcnt
#define tpp_keyword_asstring(self) ((tpp_string *)&(self)->_TPP_KEYWORD_STRING_ABI_START)
#define tpp_string_askeyword(self) ((tpp_keyword *)((char *)(self) - tpp_offsetof(tpp_keyword, _TPP_KEYWORD_STRING_ABI_START)))

/* Check if "self" matches the C, constant string literal "cstr" */
#define tpp_keyword_equals_cstr(self, cstr)                 \
	((self)->tk_len == (sizeof(cstr) / sizeof(char)) - 1 && \
	 tpp_memcmp((self)->tk_kwd, cstr, sizeof(cstr) - sizeof(char)) == 0)


#if TPP_HAVE_KEYWORD_MISC
/* Ensure that `self->tk_misc' has been allocated and return it.
 * If it isn't already allocated, allocate+return it lazily.
 * WARNING: Only call this function on a "writable" keyword (s.a. `tpp_keywords_copybuiltin()')
 *
 * @return: * :   The "misc" data of "self" (freshly allocated)
 * @return: NULL: OOM (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_misc *TPPCALL
tpp_keyword_requiremisc(tpp_keyword *tpp_restrict self);
#endif /* TPP_HAVE_KEYWORD_MISC */


/* Calculate the hash of a given keyword string */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof(tpp_char const *tpp_restrict kwd, tpp_size len);


#if TPP_HAVE_BSE && TPP_HAVE_UNICODE
#define TPP_HAVE_BSE_FILE_PARAM 1
#define tpp_bse_file__PARAM  , tpp_file const *tpp_restrict file
#define tpp_bse_file__ARG(x) , x
#else /* TPP_HAVE_BSE && TPP_HAVE_UNICODE */
#define TPP_HAVE_BSE_FILE_PARAM 0
#define tpp_bse_file__PARAM  /* nothing */
#define tpp_bse_file__ARG(x) /* nothing */
#endif /* !TPP_HAVE_BSE || !TPP_HAVE_UNICODE */


/* Check if "ch" may be the first byte of a \-character */
#if TPP_HAVE_TRIGRAPHS
#define _tpp_maybe_isbackslash(ch) ((ch) == '\\' || (ch) == '?') /* ?: because "??/" maps to "\" */
#else /* TPP_HAVE_TRIGRAPHS */
#define _tpp_maybe_isbackslash(ch) ((ch) == '\\')
#endif /* !TPP_HAVE_TRIGRAPHS */

/* Check if "ch" may be the first byte of a LF-style character */
#if TPP_HAVE_UNICODE
#define _tpp_maybe_islf(ch) tpp_ascii_islfornascii(ch)
#else /* TPP_HAVE_UNICODE */
#define _tpp_maybe_islf(ch) tpp_ascii_islf(ch)
#endif /* !TPP_HAVE_UNICODE */

/* Helper macros to skip over BSE when parsing already-loaded text.
 * tpp_skipbse_fwd: If "pos" points at a \-character, skip forward until end of BSE (if it is one)
 * tpp_skipbse_bck: If "pos" points after a line-feed character, skip backward until start of BSE (if it is one) */
#if TPP_HAVE_BSE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_skipbse_fwd_(tpp_char const *pos, tpp_char const *end tpp_bse_file__PARAM);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_skipbse_bck_(tpp_char const *pos, tpp_char const *start tpp_bse_file__PARAM);
#define tpp_skipbse_fwd(pos, end, file)   (((pos) >= (end) || !_tpp_maybe_isbackslash(*(pos))) ? (pos) : tpp_skipbse_fwd_(pos, end tpp_bse_file__ARG(file)))
#define tpp_skipbse_bck(pos, start, file) (((pos) <= (start) || !_tpp_maybe_islf((pos)[-1])) ? (pos) : tpp_skipbse_bck_(pos, start tpp_bse_file__ARG(file)))
#else /* TPP_HAVE_BSE */
#define tpp_skipbse_fwd(pos, end, file)   (pos)
#define tpp_skipbse_bck(pos, start, file) (pos)
#endif /* !TPP_HAVE_BSE */


#if TPP_HAVE_BSE
/* Same as `tpp_hashof()', but skip over \-escaped linefeeds when calculating the hash */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof_bse_(tpp_char const *tpp_restrict kwd, tpp_size len tpp_bse_file__PARAM);
#define tpp_hashof_bse(kwd, len, file) tpp_hashof_bse_(kwd, len tpp_bse_file__ARG(file))

/* Copy `in_text...+=len' to `out_text', whilst removing \-escaped linefeeds
 * The caller must ensure that `out_text' has space for at least `len' bytes,
 * and the actual # of used bytes of `out_text' is returned. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_without_bse_(tpp_char *tpp_restrict out_text,
                 tpp_char const *tpp_restrict in_text,
                 tpp_size len tpp_bse_file__PARAM);
#define tpp_without_bse(out_text, in_text, len, file) \
	tpp_without_bse_(out_text, in_text, len tpp_bse_file__ARG(file))

/* Compare 2 strings, one of which may contain \-escaped linefeeds that must be skipped. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) int TPPCALL
tpp_memcmp_bse_(tpp_char const *lhs_without_bse, tpp_size lhs_len,
                tpp_char const *rhs_with_bse, tpp_size rhs_len
                tpp_bse_file__PARAM);
#define tpp_memcmp_bse(lhs_without_bse, lhs_len, rhs_with_bse, rhs_len, file) \
	tpp_memcmp_bse_(lhs_without_bse, lhs_len, rhs_with_bse, rhs_len tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_BSE */


/* Lookup one of the built-in, pre-defined keywords */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword(tpp_char const *tpp_restrict kwd,
                       tpp_size len, tpp_hash hash);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_byid(enum tpp_token_id id);
#if TPP_HAVE_BSE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_bse_(tpp_char const *tpp_restrict kwd,
                            tpp_size len, tpp_hash hash
                            tpp_bse_file__PARAM);
#define tpp_builtin_getkeyword_bse(kwd, len, hash, file) \
	tpp_builtin_getkeyword_bse_(kwd, len, hash tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_BSE */



/* Custom keywords table */
typedef struct tpp_keywords {
	unsigned int          tks_kwdc; /* Amount of keyword entries stored. */
	tpp_hash              tks_bckm; /* Allocated bucket mask. */
	TPP_REF tpp_keyword **tks_bckv; /* [0..1][owned][0..tks_bckc+1][owned] Resizable keyword hash-map vector.
	                                 * NOTE: When the keyword map is destroyed, all linked keywords are, too.
	                                 *       Since this only happens when a lexer is finalized, this should
	                                 *       only happen once *all* keywords have their reference counters
	                                 *       set to "1". For this purpose, "tpp_keywords_fini" asserts that
	                                 *       no keyword has some other reference count value. */
} tpp_keywords;

TPP_DECL TPP_REF tpp_keyword *tpp_keywords_empty_map[1];

/* Initialize/finalize a given keywords table. */
#define tpp_keywords_init(self)                     \
	(void)((self)->tks_kwdc = (self)->tks_bckm = 0, \
	       (self)->tks_bckv = tpp_keywords_empty_map)
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_fini(tpp_keywords *tpp_restrict self);

/* Lookup keywords within the given keywords-table **ONLY**
 * @return: * :   The keyword in question
 * @return: NULL: No such keyword (consider using "tpp_keywords_getkeyword" to
 *                also check for builtin keywords, or "tpp_keywords_newkeyword"
 *                to do the same, but lazily create missing keywords) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword(tpp_keywords const *tpp_restrict self,
                         tpp_char const *tpp_restrict kwd,
                         tpp_size len, tpp_hash hash);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword_byid(tpp_keywords const *tpp_restrict self,
                              enum tpp_token_id id);
#if TPP_HAVE_BSE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword_bse_(tpp_keywords const *tpp_restrict self,
                              tpp_char const *tpp_restrict kwd,
                              tpp_size len, tpp_hash hash
                              tpp_bse_file__PARAM);
#define _tpp_keywords_getkeyword_bse(self, kwd, len, hash, file) \
	_tpp_keywords_getkeyword_bse_(self, kwd, len, hash tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_BSE */


/* Same as above, but also search the built-in keyword table (tpp_builtin_getkeyword) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword(tpp_keywords const *tpp_restrict self,
                        tpp_char const *tpp_restrict kwd,
                        tpp_size len, tpp_hash hash);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword_byid(tpp_keywords const *tpp_restrict self,
                             enum tpp_token_id id);
#if TPP_HAVE_BSE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword_bse_(tpp_keywords const *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM);
#define tpp_keywords_getkeyword_bse(self, kwd, len, hash, file) \
	tpp_keywords_getkeyword_bse_(self, kwd, len, hash tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_BSE */


/* Same as above, but if the keyword doesn't exist in `self' or the builtin
 * keyword table, a new keyword is allocated, given an ID, and inserted into `self'
 * @return: * :   The keyword associated with `kwd' (possibly having been just allocated)
 * @return: NULL: OOM (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_newkeyword(tpp_keywords *tpp_restrict self,
                        tpp_char const *tpp_restrict kwd,
                        tpp_size len, tpp_hash hash);
#if TPP_HAVE_BSE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_newkeyword_bse_(tpp_keywords *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM);
#define tpp_keywords_newkeyword_bse(self, kwd, len, hash, file) \
	tpp_keywords_newkeyword_bse_(self, kwd, len, hash tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_BSE */


#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
/* Check if "kwd" is contained in "self".
 * If so: do nothing and simply re-return "kwd"
 *
 * Otherwise, assume that "kwd" is a "builtin" keyword (as returned
 * by `tpp_builtin_getkeyword()'), in which the keyword is copied,
 * inserted into "self", and said copy is returned.
 *
 * This function must be used to make a keyword "writable" (which is
 * required before its `tk_macro' / `tk_misc' fields can safely be
 * written to (and in the later case: all fields of a potentially
 * pointed-to `tpp_keyword_misc', too)
 *
 * @return: * :   A writable copy of "kwd"
 * @return: NULL: OOM (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
tpp_keywords_copybuiltin(tpp_keywords *tpp_restrict self,
                         tpp_keyword const *tpp_restrict kwd);
#endif /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */


#if TPP_HAVE_KEYWORDS_OPENFILE
/* Construct the filename, open the file, and initialize "out_file" accordingly
 * @param: relative_to: The `tpp_file::tf_data.td_io.tff_name' of another file,
 *                      in case "filename" is a relative path, in which case the
 *                      filename of the file to open should be relative to the
 *                      directory of "relative_to"
 * @param: out_file:    The file that should be initialized (as `TPP_FILE_KIND_IO')
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Insufficient memory
 * @return: TPP_ENOENT: File not found (if you have additional "relative_to", try them) */
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
#define tpp_keywords_openfile(self, relative_to, filename, out_file) \
	tpp_keywords_openfile_ex(self, relative_to, filename, out_file, 0)
#else /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3, 4)) tpp_errno TPPCALL
tpp_keywords_openfile(/*1..1*/ tpp_keywords *tpp_restrict self,
                      /*0..1*/ tpp_keyword const *tpp_restrict relative_to,
                      /*1..1*/ /*utf-8*/ char const *tpp_restrict filename,
                      /*1..1*/ tpp_file *tpp_restrict out_file);
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE_EX */

/* Same as `tpp_keywords_openfile', but return `TPP_EMASKED' if the file was already
 * included before, and its keyword has any of the bits specified by `mask_flags' set.
 * @return: TPP_EMASKED: Flags specified by "mask_flags" were already set. */
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3, 4)) tpp_errno TPPCALL
tpp_keywords_openfile_ex(/*1..1*/ tpp_keywords *tpp_restrict self,
                         /*0..1*/ tpp_keyword const *tpp_restrict relative_to,
                         /*1..1*/ /*utf-8*/ char const *tpp_restrict filename,
                         /*1..1*/ tpp_file *tpp_restrict out_file,
                         tpp_keyword_flags mask_flags);
#else /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
#define tpp_keywords_openfile_ex(self, relative_to, filename, out_file, mask_flags) \
	tpp_keywords_openfile(self, relative_to, filename, out_file)
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE_EX */

/* Allocate+return a filename keyword */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
tpp_keywords_newkeyword_file(/*1..1*/ tpp_keywords *tpp_restrict self,
                             /*1..1*/ /*utf-8*/ char const *tpp_restrict filename);
#endif /* TPP_HAVE_KEYWORDS_OPENFILE */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/extensions.h                                             */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_EXTENSIONS
typedef enum tpp_extension_id {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) id,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	TPP_EXT_COUNT
} tpp_extension_id;

/* Default extension state */
typedef union tpp_extensions_state {
	struct {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) unsigned int tef_##id: 1;
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	} tes_flags;
	unsigned char tes_bitset[TPP_EXT_COUNT ? ((TPP_EXT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_extensions_state;
TPP_CONST_DECL tpp_extensions_state const tpp_extensions_state_default;

#define tpp_extensions_state_getid(self, id) \
	((self)->tes_bitset[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_extensions_state_get(self, id) _tpp_extensions_state_get_##id(self)
#define tpp_extensions_state_enable(self, id) \
	(void)((self)->tes_bitset[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_extensions_state_disable(self, id) \
	(void)((self)->tes_bitset[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_extensions_state_set(self, id, enabled) \
	((enabled) ? tpp_extensions_state_enable(self, id) : tpp_extensions_state_disable(self, id))


typedef struct tpp_extensions {
	tpp_extensions_state    te_state;   /* [const_if(te_pushcnt > 0)] Enabled-extensions state */
#if TPP_HAVE_EXTENSIONS_PUSH_POP
	tpp_size               te_pushcnt; /* # of times extensions were pushed since last modified */
	struct tpp_extensions *te_prev;    /* [0..1][owned] Old extension state. */
#endif /* TPP_HAVE_EXTENSIONS_PUSH_POP */
} tpp_extensions;

#if TPP_HAVE_EXTENSIONS_PUSH_POP
#define tpp_extensions_init(self)                             \
	(void)((self)->te_state   = tpp_extensions_state_default, \
	       (self)->te_pushcnt = 0,                            \
	       (self)->te_prev    = NULL)
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_extensions_fini(tpp_extensions *tpp_restrict self);

/* Push the current extensions state */
#define tpp_extensions_push(self) (void)(++(self)->te_pushcnt)

/* Pop the current extensions state (may only be called when `tpp_extensions_canpop(self)') */
TPP_DECL TPP_NONNULL((1)) void TPPCALL tpp_extensions_pop(tpp_extensions *tpp_restrict self);
#define tpp_extensions_canpop(self) ((self)->te_pushcnt != 0 || (self)->te_prev != NULL)

/* When true, `tpp_extensions_set()' must first copy the extension
 * state (which requires heap memory, and may thus fail) */
#define tpp_extensions_mustcopy(self) ((self)->te_pushcnt != 0)

/* @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: OOM */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_extensions_set(tpp_extensions *tpp_restrict self,
                   tpp_extension_id id, int enabled);
#else /* TPP_HAVE_EXTENSIONS_PUSH_POP */
#define tpp_extensions_init(self) (void)((self)->te_state = tpp_extensions_state_default)
#define tpp_extensions_fini(self) (void)0
#define tpp_extensions_set(self, id, enabled) \
	(tpp_extensions_state_set(&(self)->te_state, id, enabled), TPP_EOK)
#endif /* !TPP_HAVE_EXTENSIONS_PUSH_POP */
#define tpp_extensions_get(self, id) tpp_extensions_state_get(&(self)->te_state, id)


/* Convert between extension IDs and their human-readable names. */
TPP_DECL TPP_WUNUSED char const *TPPCALL
tpp_extension_getname(tpp_extension_id id);

/* @return: TPP_EXT_COUNT: No such extension */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_extension_id TPPCALL
tpp_extension_byname(char const *tpp_restrict name);

/* Returns the ID of the extension with the name that is closest to "name"
 * When no extensions are defined (at all), this will return "TPP_EXT_COUNT" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_extension_id TPPCALL
tpp_extension_nearest(char const *tpp_restrict name);
#else /* TPP_HAVE_EXTENSIONS */
#define tpp_extensions_state_get(self, id) 1
#define tpp_extensions_get(self, id)       1
#endif /* !TPP_HAVE_EXTENSIONS */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/warnings.h                                               */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_HAVE_WARNING_NUMBERS
#define TPP_WARNING_NUMBER_INVALID UINT_MAX
#endif /* TPP_HAVE_WARNING_NUMBERS */

#if TPP_HAVE_WARNINGS


/************************************************************************/
/* WARNING STATE                                                        */
/************************************************************************/

/* Configuration options for warnings.
 *
 * NOTE: When a warning has multiple groups (and/or a number), and is triggered,
 *       then the state used for emission is the one with the lowest, numerical
 *       value (e.g.: if at least one group is "TPP_WSTATE_DISABLED", then the
 *       warning is disabled). The used group/number will be printed however. */
typedef enum tpp_warning_state {
	TPP_WSTATE_DISABLED = 0, /* Warning exists, but is disabled */
	TPP_WSTATE_WARN     = 1, /* Regular warning */
#if TPP_HAVE_WARNING_ERROR
	TPP_WSTATE_ERROR    = 2, /* Error (s.a. `tpp_lexer::tl_maxerrors') */
#define TPP_WSTATE_ERROR_OR_FATAL TPP_WSTATE_ERROR
#else /* TPP_HAVE_WARNING_ERROR */
#define TPP_WSTATE_ERROR_OR_FATAL TPP_WSTATE_FATAL
#endif /* !TPP_HAVE_WARNING_ERROR */
	TPP_WSTATE_FATAL    = 3, /* Immediately cause a `TPP_ELEXERROR' error */
#if TPP_HAVE_WARNING_NUMBERS && TPP_BUILDING
	_TPP_WSTATE_UNDEFINED = 3, /* Config for numbered-warnings (causes state of linked groups to be used instead) */
#endif /* TPP_HAVE_WARNING_NUMBERS && TPP_BUILDING */

#if TPP_HAVE_WARNING_SUPPRESS
	TPP_WSTATE_SUPPRESS = -1, /* Treat as `TPP_WSTATE_DISABLED' a couple of times, then switch to old state
	                           * HINT: In "tpp_warnings_state", this is represented as "TPP_WSTATE_FATAL". */
#endif /* TPP_HAVE_WARNING_SUPPRESS */

#if TPP_HAVE_WARNING_DEFAULT
	TPP_WSTATE_DEFAULT = 99, /* Only for `tpp_warnings_setctx()': restore behavior from `tpp_warnings_state_default' */
#endif /* TPP_HAVE_WARNING_DEFAULT */
} tpp_warning_state;

/* Check if "lhs" is more important than "rhs".
 *
 * When a warning is emitted, the context that is configured to the greatest
 * importance is used to determine how that specific warning should be treated. */
#define tpp_warning_state_ismoreimportant(lhs, rhs) \
	((int)(lhs) < (int)(rhs))



/************************************************************************/
/* WARNING GROUPS                                                       */
/************************************************************************/

typedef enum tpp_warning_group_id {
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) wgroup_id,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	TPP_WG_COUNT
} tpp_warning_group_id;

/* Return a pointer to a \0\0-terminated list of strings describing the names
 * of for the given warning group "id". Returns "NULL" if "id" is "TPP_WG_COUNT"
 * or some other invalid warning group ID. */
TPP_DECL TPP_WUNUSED char const *TPPCALL
tpp_warning_group_getnames(tpp_warning_group_id id);

/* @return: TPP_WG_COUNT: No such warning_group */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_group_id TPPCALL
tpp_warning_group_byname(char const *tpp_restrict name);

/* Returns the ID of the warning group with the name that is closest to "name"
 * When no warning groups are defined (at all), this will return "TPP_WG_COUNT" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_group_id TPPCALL
tpp_warning_group_nearest(char const *tpp_restrict name);



/************************************************************************/
/* WARNING ID                                                           */
/************************************************************************/

typedef enum tpp_warning_id {
#define TPP_DEFS
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) warning_id,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	TPP_W_COUNT
} tpp_warning_id;

/* Returns a TPP_WG_COUNT-terminated list of group IDs associated with the given warning "id".
 * When the given "id" is "TPP_W_COUNT" or invalid, return a pointer to an empty warning-group-id-list. */
TPP_DECL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1))
tpp_warning_group_id const *TPPCALL tpp_warning_getgroups(tpp_warning_id id);

/* Returns the "tpp_lexer_printf_warning"-style format string assigned with "id".
 * When "id" is TPP_W_COUNT, invalid, or declared as "TPP_WARNING_EX", return "NULL" instead. */
TPP_DECL TPP_WUNUSED char const *TPPCALL
tpp_warning_getformat(tpp_warning_id id);

#if TPP_HAVE_WARNING_NUMBERS
/* Returns the warning ID linked to a given "number", or "TPP_W_COUNT" if "number" is unknown */
TPP_DECL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_ofnumber(unsigned int number);

/* Returns the TPP_WARNING_NUMBER_INVALID-terminated list of warning numbers
 * associated with "warning_id". If "warning_id" doesn't have any warning
 * numbers, return a pointer to "TPP_WARNING_NUMBER_INVALID". */
TPP_DECL TPP_RETNONNULL TPP_WUNUSED unsigned int const *TPPCALL
tpp_warning_getnumbers(tpp_warning_id warning_id);
#else /* TPP_HAVE_WARNING_NUMBERS */
#define tpp_warning_ofnumber(number) TPP_W_COUNT
#endif /* !TPP_HAVE_WARNING_NUMBERS */




/************************************************************************/
/* WARNING CONTEXT ID                                                   */
/************************************************************************/

/* Warning context ID (used internally to keep track
 * of warning states for groups & numbered warnings) */
typedef enum tpp_warning_context_id {
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) TPP_WC_##wgroup_id,
#include TPP_CONFIG_DEFS_FILENAME

#if TPP_HAVE_WARNING_NUMBERS
	TPP_WC_NUMBER_MIN,
	_TPP_WC_NUMBER_MIN = TPP_WC_NUMBER_MIN - 1,
#define TPP_DECLARE_NUMBERED_WARNING(warning_id) TPP_WC_##warning_id,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DECLARE_NUMBERED_WARNING
#endif /* TPP_HAVE_WARNING_NUMBERS */
#undef TPP_DEFS
	TPP_WC_COUNT
} tpp_warning_context_id;

/* Check what kind of context is being described by "self" */
#if TPP_HAVE_WARNING_NUMBERS
#define tpp_warning_context_id_isgroup(self) \
	(!tpp_warning_context_id_isnumber(self))
#define tpp_warning_context_id_isnumber(self) \
	((unsigned int)(self) >= (unsigned int)TPP_WC_NUMBER_MIN)
#else /* TPP_HAVE_WARNING_NUMBERS */
#define tpp_warning_context_id_isgroup(self)  1
#define tpp_warning_context_id_isnumber(self) 0
#endif /* !TPP_HAVE_WARNING_NUMBERS */

/* Return the context-id of a given "tpp_warning_group_id wgroup_id" */
#define tpp_warning_context_id_ofgroup(wgroup_id) \
	((tpp_warning_context_id)(unsigned int)(wgroup_id))

/* Return the group-id of a given "tpp_warning_context_id ctx_id".
 * Caller must ensure that `tpp_warning_context_id_isgroup(ctx_id)' */
#define tpp_warning_context_id_asgroup(ctx_id) \
	((tpp_warning_group_id)(unsigned int)(ctx_id))

#if TPP_HAVE_WARNING_NUMBERS
/* Returns the context-id of a given (should-be) "numbered" warning.
 * When "warning_id" is invalid, "TPP_W_COUNT", or not numbered, this
 * function will return "TPP_WC_COUNT" instead. */
TPP_DECL TPP_WUNUSED tpp_warning_context_id TPPCALL
tpp_warning_context_id_ofwarning(tpp_warning_id warning_id);

/* Returns the warning-id linked to "ctx_id", when `tpp_warning_context_id_isnumber(ctx_id)'.
 * When no warning is linked to "ctx_id" ("ctx_id" is either linked to a warning group, or
 * is "TPP_WC_COUNT" or some other invalid ID), return "TPP_W_COUNT" instead. */
TPP_DECL TPP_WUNUSED tpp_warning_id TPPCALL
tpp_warning_context_id_aswarning(tpp_warning_context_id ctx_id);
#else /* TPP_HAVE_WARNING_NUMBERS */
#define tpp_warning_context_id_ofwarning(warning_id) TPP_WC_COUNT
#define tpp_warning_context_id_aswarning(ctx_id)     TPP_W_COUNT
#endif /* !TPP_HAVE_WARNING_NUMBERS */




/************************************************************************/
/* WARNINGS STATE                                                       */
/************************************************************************/
typedef union tpp_warnings_state {
	struct {
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
	unsigned int twsg_##wgroup_id: 2; /* One of `tpp_warning_state' */
#include TPP_CONFIG_DEFS_FILENAME
#if TPP_HAVE_WARNING_NUMBERS
#define TPP_DECLARE_NUMBERED_WARNING(warning_id) \
	unsigned int twsn_##warning_id: 2; /* One of `tpp_warning_state' */
#define TPP_WARNING(warning_id, wgroup_ids, numbers, format) \
	TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, warning_id)
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DECLARE_NUMBERED_WARNING
#endif /* TPP_HAVE_WARNING_NUMBERS */
#undef TPP_DEFS
	} tws_state;
	unsigned char tws_bitset[TPP_WC_COUNT ? (((TPP_WC_COUNT * 2) + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_warnings_state;

TPP_DECL tpp_warnings_state const tpp_warnings_state_default;

#define _tpp_warnings_state_bitindx(ctx_id) ((unsigned int)((unsigned int)(ctx_id) / (TPP_CHAR_BIT >> 1)))
#define _tpp_warnings_state_bitshft(ctx_id) ((unsigned int)((unsigned int)(ctx_id) % (TPP_CHAR_BIT >> 1)))

/* Get/set the warning state of a given "tpp_warning_context_id ctx_id" */
#define tpp_warnings_state_get(self, ctx_id) \
	((tpp_warning_state)(((self)->tws_bitset[_tpp_warnings_state_bitindx(ctx_id)] >> _tpp_warnings_state_bitshft(ctx_id)) & 3))
#define tpp_warnings_state_set(self, ctx_id, value)                                                                  \
	(void)((self)->tws_bitset[_tpp_warnings_state_bitindx(ctx_id)] =                                                 \
	       ((self)->tws_bitset[_tpp_warnings_state_bitindx(ctx_id)] & ~(3 << _tpp_warnings_state_bitshft(ctx_id))) | \
	       (((unsigned char)(unsigned int)(value)) << _tpp_warnings_state_bitshft(ctx_id)))



#if TPP_HAVE_WARNING_SUPPRESS
typedef struct tpp_warning_suppress_item {
	tpp_warning_context_id twsi_ctx_id;  /* Context ID of this suppression item */
	tpp_warning_state      twsi_restore; /* Warning state to restore once the warning is no longer being suppressed */
	tpp_size               twsi_count;   /* # of times to suppress this warning before restoring its previous state */
} tpp_warning_suppress_item;

typedef struct tpp_warning_suppressions {
	tpp_size                   tws_ctxc; /* # of warnings that are being suppressed right now */
	tpp_size                   tws_ctxa; /* Allocated size of `tws_ctxv' */
	tpp_warning_suppress_item *tws_ctxv; /* [0..tws_ctxc|alloc(tws_ctxa)][owned] Vector of suppressions (sorted by `twsi_ctx_id') */
} tpp_warning_suppressions;
#define tpp_warning_suppressions_init(self) \
	(void)((self)->tws_ctxc = (self)->tws_ctxa = 0, (self)->tws_ctxv = NULL)
#define tpp_warning_suppressions_fini(self) \
	(void)tpp_free((self)->tws_ctxv)
#endif /* TPP_HAVE_WARNING_SUPPRESS */


/* Lexer warnings configuration */
typedef struct tpp_warnings {
	tpp_warnings_state       tw_state;        /* [const_if(tw_pushcnt > 0)] Warning state */
#if TPP_HAVE_WARNING_SUPPRESS
	tpp_warning_suppressions tw_suppressions; /* [const_if(tw_pushcnt > 0)] Information about suppressed warnings */
#define _tpp_warnings_init_suppressions(self) , tpp_warning_suppressions_init(&(self)->tw_suppressions)
#else /* TPP_HAVE_WARNING_SUPPRESS */
#define _tpp_warnings_init_suppressions(self) /* nothing */
#endif /* !TPP_HAVE_WARNING_SUPPRESS */
#if TPP_HAVE_WARNINGS_PUSH_POP
	tpp_size                 tw_pushcnt;      /* # of times warnings pushed were since last modified */
	struct tpp_warnings     *tw_prev;         /* [0..1][owned] Old warning state. */
#define _tpp_warnings_init_push(self) , (self)->tw_pushcnt = 0, (self)->tw_prev = NULL
#else /* TPP_HAVE_WARNINGS_PUSH_POP */
#define _tpp_warnings_init_push(self) /* nothing */
#endif /* !TPP_HAVE_WARNINGS_PUSH_POP */
} tpp_warnings;

/* Initialize a given warnings context "self" */
#define tpp_warnings_init(self)                          \
	(void)((self)->tw_state = tpp_warnings_state_default \
	       _tpp_warnings_init_suppressions(self)         \
	       _tpp_warnings_init_push(self))

#undef TPP_HAVE_WARNINGS_FINI
#define TPP_HAVE_WARNINGS_FINI \
	(TPP_HAVE_WARNING_SUPPRESS || TPP_HAVE_WARNINGS_PUSH_POP)

/* Finalize a given warnings context "self" */
#if TPP_HAVE_WARNINGS_FINI
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_fini(tpp_warnings *tpp_restrict self);
#else /* TPP_HAVE_WARNINGS_FINI */
#define tpp_warnings_fini(self) (void)0
#endif /* !TPP_HAVE_WARNINGS_FINI */


#if TPP_HAVE_WARNINGS_PUSH_POP
/* Push the current warnings state */
#define tpp_warnings_push(self) (void)(++(self)->tw_pushcnt)

/* Pop the current warnings state (may only be called when `tpp_warnings_canpop(self)') */
TPP_DECL TPP_NONNULL((1)) void TPPCALL tpp_warnings_pop(tpp_warnings *tpp_restrict self);
#define tpp_warnings_canpop(self) ((self)->tw_pushcnt != 0 || (self)->tw_prev != NULL)

/* When true, `tpp_warnings_setctx()' must first copy the extension
 * state (which requires heap memory, and may thus fail) */
#define tpp_warnings_mustcopy(self) ((self)->tw_pushcnt != 0)
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */

/* Return the state of "ctx_id". The caller is
 * responsible to ensure that "ctx_id" is valid. */
#if TPP_HAVE_WARNING_SUPPRESS
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_state TPPCALL
tpp_warnings_getctx(tpp_warnings const *tpp_restrict self,
                    tpp_warning_context_id ctx_id);
#else /* TPP_HAVE_WARNING_SUPPRESS */
#define tpp_warnings_getctx(self, ctx_id) \
	tpp_warnings_state_get(&(self)->tw_state, ctx_id)
#endif /* !TPP_HAVE_WARNING_SUPPRESS */


#undef TPP_HAVE_WARNINGS_SETCTX_MAYFAIL
#define TPP_HAVE_WARNINGS_SETCTX_MAYFAIL \
	(TPP_HAVE_WARNING_SUPPRESS || TPP_HAVE_WARNINGS_PUSH_POP)

/* Set the state of "ctx_id" to "state".
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HAVE_WARNINGS_SETCTX_MAYFAIL
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_warnings_setctx(tpp_warnings *tpp_restrict self,
                    tpp_warning_context_id ctx_id,
                    tpp_warning_state state);
#else /* TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */
#define tpp_warnings_setctx(self, ctx_id, state) \
	(tpp_warnings_setctx_(self, ctx_id, state), TPP_EOK)
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_setctx_(tpp_warnings *tpp_restrict self,
                     tpp_warning_context_id ctx_id,
                     tpp_warning_state state);
#endif /* !TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */



struct tpp_warning_invokeinfo {
	tpp_warning_state      twii_state;  /* State with which the warning should be invoked.
	                                     * Always one of:
	                                     * - TPP_WSTATE_DISABLED
	                                     * - TPP_WSTATE_WARN
	                                     * - TPP_WSTATE_ERROR      #if TPP_HAVE_WARNING_ERROR
	                                     * - TPP_WSTATE_FATAL */
	tpp_warning_context_id twii_ctx_id; /* Context ID that for error messages */
};

#undef TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
#define TPP_HAVE_WARNINGS_INVOKE_MAYFAIL \
	(TPP_HAVE_WARNING_SUPPRESS && TPP_HAVE_WARNINGS_PUSH_POP)

/* Invoke "warning_id" (updating suppression counters if necessary) and
 * returning information about the context/state with which the warning
 * should be processed.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (only when "TPP_HAVE_WARNINGS_INVOKE_MAYFAIL") */
#if TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_warnings_invoke(tpp_warnings *tpp_restrict self, tpp_warning_id warning_id,
                    struct tpp_warning_invokeinfo *tpp_restrict result);
#else /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */
#define tpp_warnings_invoke(self, warning_id, result) \
	(tpp_warnings_invoke_(self, warning_id, result), TPP_EOK)
TPP_DECL TPP_NONNULL((1, 3)) void TPPCALL
tpp_warnings_invoke_(tpp_warnings const *tpp_restrict self, tpp_warning_id warning_id,
                     struct tpp_warning_invokeinfo *tpp_restrict result);
#endif /* !TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */

#endif /* TPP_HAVE_WARNINGS */

TPP_DECL_END
/************************************************************************/

/************************************************************************/
/* File: parts/lexer.h                                                  */
/************************************************************************/
TPP_DECL_BEGIN

#undef TPP_HAVE_LEXER_STATE_FLAGS
#if (TPP_HAVE_CPP_DIRECTIVES || \
     TPP_HAVE_INCLUDE_STACK)
#define TPP_HAVE_LEXER_STATE_FLAGS 1
#else /* ... */
#define TPP_HAVE_LEXER_STATE_FLAGS 0
#endif /* !... */


#if TPP_HAVE_LEXER_STATE_FLAGS
#define tpp_lexer_state_flags uint_least8_t
#define TPP_LEXER_STATE_FLAG_NORMAL       UINT8_C(0x00) /* Normal state flags */
#if TPP_HAVE_CPP_DIRECTIVES
#define TPP_LEXER_STATE_FLAG_NODIRECTIVES UINT8_C(0x01) /* A non-comment/space token was encountered since the last
                                                         * TPP_TOK_LF, meaning PP-directives may not be parsed. */
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#if TPP_HAVE_INCLUDE_STACK
#define TPP_LEXER_STATE_FLAG_NOPOPFILE    UINT8_C(0x02) /* Do not pop files from the #include-stack */
#endif /* TPP_HAVE_INCLUDE_STACK */
#endif /* TPP_HAVE_LEXER_STATE_FLAGS */

typedef struct tpp_lexer {
	union {
		tpp_token      tlc_tok;  /* [valid_if(WAS_CALLED(tpp_lexer_yieldraw()))] Last-read token (never
		                          * set to one of `TPP_TOK_E*'; iow: always positive or TPP_TOK_EOF). */
		struct {
			char _tli_pad[tpp_offsetof(tpp_token, tt_end)];
			tpp_file   tli_file; /* [OVERRIDE(.tf_prev, [owned])]
			                      * The file that lies at the top of the lexer's #include/macro-stack.
			                      * this is also the file whose buffer currently contains `tl_tok' */
		} tlc_input;
	} tl_core;


	/* Custom keywords table. */
	tpp_keywords tl_kwds;


	/* Lexer extensions. */
#if TPP_HAVE_EXTENSIONS
	tpp_extensions tl_exts;
#define _tpp_lexer_init_exts(self) , tpp_extensions_init(&(self)->tl_exts)
#else /* TPP_HAVE_EXTENSIONS */
#define _tpp_lexer_init_exts(self) /* noting */
#endif /* !TPP_HAVE_EXTENSIONS */


	/* Enabled tokens */
#if TPP_HAVE_FEATURES
	tpp_features tl_feat;
#define _tpp_lexer_init_feat(self) , tpp_features_init(&(self)->tl_feat)
#else /* TPP_HAVE_FEATURES */
#define _tpp_lexer_init_feat(self) /* nothing */
#endif /* !TPP_HAVE_FEATURES */


	/* Lexer state flags */
#if TPP_HAVE_LEXER_STATE_FLAGS
	tpp_lexer_state_flags tl_state;
#define _tpp_lexer_init_state(self) , (self)->tl_state = TPP_LEXER_STATE_FLAG_NORMAL
#else /* TPP_HAVE_LEXER_STATE_FLAGS */
#define _tpp_lexer_init_state(self) /* nothing */
#endif /* !TPP_HAVE_LEXER_STATE_FLAGS */


	/* TODO: system #include paths (/usr/include, ...) */


	/* TODO: #ifdef stack */


	/* Warning configuration / printer */
#undef TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER /* Wrapper for "TPP_CONFIG_WARNPRINTER" */
#undef TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER /* Calls "fwrite(stderr)" */
#undef TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER    /* Does nothing */
#if TPP_HAVE_WARNINGS
	tpp_warnings tl_warn; /* Compiler warnings state */
#define _tpp_lexer_init_warn(self) , tpp_warnings_init(&(self)->tl_warn)
#ifdef TPP_CONFIG_WARNPRINTER
#if TPP_CONFIG_WARNPRINTER_NEEDS_ARG
	void             *tl_warnprinterarg; /* [?..?] Argument for "TPP_CONFIG_WARNPRINTER" */
#define tpp_lexer_getwarnprinter(self)    (&TPP_CONFIG_WARNPRINTER)
#define tpp_lexer_getwarnprinterarg(self) (self)->tl_warnprinterarg
#else /* TPP_CONFIG_WARNPRINTER_NEEDS_ARG */
#define TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self)    &_tpp_lexer_wrapped_warnprinter
#define tpp_lexer_getwarnprinterarg(self) NULL
#endif /* !TPP_CONFIG_WARNPRINTER_NEEDS_ARG */
#else /* TPP_CONFIG_WARNPRINTER */
	tpp_formatprinter tl_warnprinter;    /* [0..1] Warning printer (or "NULL" to use "fwrite(stderr)") */
	void             *tl_warnprinterarg; /* [valid_if(tl_warnprinter != NULL)] */
#define _tpp_lexer_init_warnprinter(self) , (self)->tl_warnprinter = NULL
#define tpp_lexer_setwarnprinter(self, printer, arg) \
	(void)((self)->tl_warnprinter    = (printer),    \
	       (self)->tl_warnprinterarg = (arg))
#if TPP_HAVE_BUILTIN_WARNPRINTER
#define TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self) ((self)->tl_warnprinter ? (self)->tl_warnprinter : &_tpp_lexer_builtin_warnprinter)
#else /* TPP_HAVE_BUILTIN_WARNPRINTER */
#define TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self) ((self)->tl_warnprinter ? (self)->tl_warnprinter : &_tpp_lexer_noop_warnprinter)
#endif /* !TPP_HAVE_BUILTIN_WARNPRINTER */
#define tpp_lexer_getwarnprinterarg(self) (self)->tl_warnprinterarg
#endif /* !TPP_CONFIG_WARNPRINTER */
#else /* TPP_HAVE_WARNINGS */
#define _tpp_lexer_init_warn(self)                        /* nothing */
#define tpp_lexer_setwarnprinter(self, printer, arg) (void)0
#endif /* !TPP_HAVE_WARNINGS */
#ifndef _tpp_lexer_init_warnprinter
#define _tpp_lexer_init_warnprinter(self) /* nothing */
#endif /* !_tpp_lexer_init_warnprinter */


	/* Lexer error limits */
#if TPP_HAVE_WARNING_ERROR
	tpp_size tl_error_count; /* # of times "TPP_WSTATE_ERROR" was emitted.
	                          * When this is non-zero by the time your compiler finishes
	                          * compiling your source file, you should NOT proceed, but
	                          * propagate an error. */
#define _tpp_lexer_initerrorcount(self)  , (self)->tl_error_count = 0
#define tpp_lexer_geterrorcount(self)    (self)->tl_error_count
#define tpp_lexer_seterrorcount(self, v) (void)((self)->tl_error_count = (v))
#if TPP_ERROR_LIMIT < 0
	tpp_size tl_error_limit; /* Once `tl_error_count >= tl_error_limit', "TPP_WSTATE_ERROR" is treated as "TPP_WSTATE_FATAL" */
#define _tpp_lexer_initerrorlimit(self)  , (self)->tl_error_limit = (tpp_size)(-TPP_ERROR_LIMIT)
#define tpp_lexer_geterrorlimit(self)    ((self)->tl_error_limit)
#define tpp_lexer_seterrorlimit(self, v) (void)((self)->tl_error_limit = (v))
#else /* TPP_ERROR_LIMIT < 0 */
#define _tpp_lexer_initerrorlimit(self)  /* nothing */
#define tpp_lexer_geterrorlimit(self)    TPP_ERROR_LIMIT
#endif /* TPP_ERROR_LIMIT >= 0 */
#else /* TPP_HAVE_WARNING_ERROR */
#define _tpp_lexer_initerrorcount(self)  /* nothing */
#define _tpp_lexer_initerrorlimit(self)  /* nothing */
#define tpp_lexer_geterrorcount(self)    0
#endif /* !TPP_HAVE_WARNING_ERROR */
} tpp_lexer;



#define tpp_lexer_gettoken(self)            (&(self)->tl_core.tlc_tok)
#define tpp_lexer_getfile(self)             (&(self)->tl_core.tlc_input.tli_file)
#define tpp_lexer_getfeat(self, TPP_FEAT_x) tpp_features_get(&(self)->tl_feat, TPP_FEAT_x)
#define tpp_lexer_getext(self, TPP_EXT_x)   tpp_extensions_get(&(self)->tl_exts, TPP_EXT_x)
#if TPP_HAVE_WARNINGS
#define tpp_lexer_getwarn(self) (&(self)->tl_warn)
#endif /* TPP_HAVE_WARNINGS */

/* Initialize/finalize everything about "self" except for "tl_core" */
#define _tpp_lexer_init_common(self)             \
	(void)(tpp_keywords_init(&(self)->tl_kwds)   \
	       _tpp_lexer_init_exts(self)            \
	       _tpp_lexer_init_feat(self)            \
	       _tpp_lexer_init_state(self)           \
	       _tpp_lexer_init_warn(self)            \
	       _tpp_lexer_init_warnprinter(self) \
	       _tpp_lexer_initerrorcount(self)       \
	       _tpp_lexer_initerrorlimit(self))
TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_fini_common(tpp_lexer *tpp_restrict self);

/* Finalize a given lexer "self" */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_fini(tpp_lexer *tpp_restrict self);


/* Initialize a lexer that simply reads the given [text,text+text_size) blob. */
#if TPP_HAVE_UNICODE
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ex(tpp_lexer *tpp_restrict self,
                       /*utf-8*/ char const *filename,
                       void const *text, tpp_size text_size,
                       tpp_file_encoding encoding);
#define tpp_lexer_init_text_ascii(self, filename, text, text_size) \
	tpp_lexer_init_text_ex(self, filename, text, text_size, TPP_FILE_ENCODING_ASCII)
#define tpp_lexer_init_text_utf8(self, filename, text, text_size) \
	tpp_lexer_init_text_ex(self, filename, text, text_size, TPP_FILE_ENCODING_FORCE_UTF8)
#else /* TPP_HAVE_UNICODE */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ascii(tpp_lexer *tpp_restrict self,
                          /*utf-8*/ char const *filename,
                          void const *text, tpp_size text_size);
#endif /* !TPP_HAVE_UNICODE */

#if TPP_HAVE_LEXER_INIT_IO
/* Initialize a lexer such that it starts reading from "handle"
 * @param: filename: [0..1] Filename to use for messages (s.a. `tpp_file_filename()')
 *                          WARNING: This filename is *NOT* copied -- it must remain
 *                                   allocated and valid until "self" is finalized.
 * @param: handle:   The I/O handle to read from in order to retrieve text data.
 * @param: ioflags:  Extra flags specifying how to interact with "handle":
 *                   - TPP_FILE_IOFLAGS_NONBLOCK: Do non-blocking reads (useful in case "handle" is a pipe)
 *                   - TPP_FILE_IOFLAGS_NOCLOSE:  A later call to `tpp_lexer_fini()' will not close "handle"
 *                   - TPP_FILE_IOFLAGS_SYSHDR:   Do not emit warnings */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                     tpp_io_handle handle, tpp_file_ioflags ioflags);
#endif /* TPP_HAVE_LEXER_INIT_IO */

#if TPP_HAVE_LEXER_INIT_FILENAME
/* Initialize a lexer such that it starts reading from "filename"
 * @return: * : TPP_ENOENT: No such file or directory
 * @return: * : TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_init_filename(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *tpp_restrict filename);
#endif /* TPP_HAVE_LEXER_INIT_FILENAME */



/* Read a single character (byte) whilst accounting for BSE (if enabled)
 * and automatically extending the current file if EOF is reached.
 * On true EOF:
 * - *p_result = '\0'
 * - *p_pos = tpp_lexer_getfile(self)->tf_end
 * - return TPP_EOK;
 *
 * @return: TPP_EOK:    Character was read
 * @return: TPP_ENOMEM: Out of memory
 * @return: TPP_EIO:    Failed to read from underlying file */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readchar(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos,
                   tpp_char *tpp_restrict p_result);

#if TPP_HAVE_UNICODE
/* Same as `tpp_lexer_readchar()', but (if the current file's encoding allows
 * it, and IN(*p_pos) points at a multi-byte character), decode a multi-byte
 * character and return it. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readunichar(tpp_lexer *tpp_restrict self,
                      tpp_char const **tpp_restrict p_pos,
                      tpp_unichar *tpp_restrict p_result);
#endif /* TPP_HAVE_UNICODE */



/* Temporarily modify the lexer state flags */
#if TPP_HAVE_LEXER_STATE_FLAGS
#define tpp_lexer_state_push(self, mask, flags)                         \
	do {                                                                \
		tpp_lexer_state_flags const _tlsp_old_flags = (self)->tl_state; \
		(self)->tl_state = ((self)->tl_state & (tpp_lexer_state_flags)(mask)) | (tpp_lexer_state_flags)(flags)
#define tpp_lexer_state_pop(self, mask, flags)                                                                      \
		(self)->tl_state = ((self)->tl_state & ((tpp_lexer_state_flags)(mask) & ~(tpp_lexer_state_flags)(flags))) | \
		                   (_tlsp_old_flags & ~((tpp_lexer_state_flags)(mask) & ~(tpp_lexer_state_flags)(flags)));  \
	} while (0)
#else /* TPP_HAVE_LEXER_STATE_FLAGS */
#define tpp_lexer_state_push(self, mask, flags) do {
#define tpp_lexer_state_pop(self, mask)         } while (0)
#endif /* !TPP_HAVE_LEXER_STATE_FLAGS */


/* Temporarily disable automatic pop-to-prev-file on EOF */
#if TPP_HAVE_INCLUDE_STACK
#define tpp_lexer_autopopfile_pushoff(self) tpp_lexer_state_push(self, 0, TPP_LEXER_STATE_FLAG_NOPOPFILE)
#define tpp_lexer_autopopfile_pop(self)     tpp_lexer_state_pop(self, 0, TPP_LEXER_STATE_FLAG_NOPOPFILE)
#else /* TPP_HAVE_INCLUDE_STACK */
#define tpp_lexer_autopopfile_pushoff(self) do {
#define tpp_lexer_autopopfile_pop(self)     } while (0)
#endif /* !TPP_HAVE_INCLUDE_STACK */

/* Do a raw yield and update `self->tl_tok' in the process, then return `tl_tok.tt_id'.
 * - On EOF, automatically pop `tl_file->tf_prev' and continue reading from there
 * - On error, return one of `TPP_TOK_E*' (e.g. `TPP_TOK_EIO').
 *   Such error codes will NOT be stored in `tl_tok.tt_id'!
 *
 * NOTE: This function does *NOT* deal with:
 * - Preprocessor directives
 * - Builtin macros
 * - User-defined macros
 * - Filtering out comment, line-feed, and whitespace tokens
 *
 * @return: * :               The newly read token
 * @return: TPP_TOK_ISERR(*): Error (s.a. `TPP_TOK_ASERR(return)' and `enum tpp_errno') */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldraw(tpp_lexer *tpp_restrict self);

/* Same as `tpp_lexer_yieldraw()', but populate the token from a custom `*p_pos',
 * and don't pop files from the current #include-stack (unless `p_pos' is the top-
 * most file's `tf_pos')
 *
 * NOTES:
 *  - This function will *NOT* populate "tpp_lexer_gettoken(self)->tt_end",
 *    however the value it would have written there is OUT(*p_pos), meaning
 *    you can just use that instead, and call this function multiple times
 *    to yield more than 1 token
 *  - This function can be used to peek future tokens, as it will also expand
 *    the current file when `*p_pos' would go beyond its end. (in this case,
 *    `*p_pos' is updated such that it always remains valid)
 *  - Unlike `tpp_lexer_yieldraw()', this function will *not* modify the
 *    currently loaded file's `tf_pos' (unless `p_pos == &file->tf_pos'),
 *    meaning that if EOF is reached, the file's chunk will only ever be
 *    expanded, but no old data (that would appear before `tf_pos') will
 *    be deallocated
 *  - This function will also not automatically move on to the next file
 *    in line when the current one has been fully exhausted (unless the
 *    given `p_pos == &file->tf_pos'), meaning that TPP_TOK_EOF will be
 *    returned when no more data can be loaded.
 *
 * This is used to implement `tpp_lexer_yieldraw()', which simply passes `' */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at(tpp_lexer *tpp_restrict self, tpp_char const **p_pos);


/* Wrapper around `tpp_lexer_yieldraw()' that filters certain tokens (based on
 * configured features), and implements handling for preprocessor directives,
 * like "#define", "#include", etc:
 * - TPP_TOK_LF:      Filtered based on `TPP_HAVE_TPP_TOK_LF' / `TPP_FEAT_TPP_TOK_LF'
 * - TPP_TOK_SPACE:   Filtered based on `TPP_HAVE_TPP_TOK_SPACE' / `TPP_FEAT_TPP_TOK_SPACE'
 * - TPP_TOK_COMMENT: Filtered based on `TPP_HAVE_TPP_TOK_COMMENT' / `TPP_FEAT_TPP_TOK_COMMENT'
 *
 * @return: * :               The newly read token (after accounting for preprocessor directives)
 * @return: TPP_TOK_ISERR(*): Error (s.a. `TPP_TOK_ASERR(return)' and `enum tpp_errno') */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp(tpp_lexer *tpp_restrict self);


/* Wrapper around `tpp_lexer_yieldpp()' that adds handling for macro expansion.
 * @return: * :               The newly read token (after accounting for macros)
 * @return: TPP_TOK_ISERR(*): Error (s.a. `TPP_TOK_ASERR(return)' and `enum tpp_errno') */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield(tpp_lexer *tpp_restrict self);




#if TPP_HAVE_KEYWORD_FLAGS
/* Return the effective set of flags for a given "kwd"
 * Since the effective flags for (certain) builtin keywords
 * can depend on active extensions/features, this can only
 * be done in the context of a specific lexer (rather than
 * stand-alone using only the "kwd")
 *
 * @return: * : Set of `TPP_KEYWORD_FLAG_*' */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_flags TPPCALL
tpp_lexer_getkeywordflags(tpp_lexer *tpp_restrict self,
                          tpp_keyword const *tpp_restrict kwd);
#endif /* TPP_HAVE_KEYWORD_FLAGS */

#if TPP_HAVE_CPP_IF_ELSE_ENDIF
/* Returns true if "kwd" should be considered to be "#if defined()"
 * Since "builtin" keywords can be considered to be "defined", even
 * when `kwd->tk_macro == NULL', this function is needed to handle
 * such macros. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_getkeyworddefined(tpp_lexer *tpp_restrict self,
                            tpp_keyword const *tpp_restrict kwd);
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */


#if TPP_HAVE_TPP_TOK_STRINGLIKE
/* Print the unescaped representation of the string-token described by "self"
 * The caller must ensure that `TPP_TOK_ISSTRING(tpp_lexer_gettoken(self)->tt_id)'
 * @param: data_printer: Printer used to fast-forward string data from token inputs, as well as \xAB
 * @param: utf8_printer: Printer used to emit explicitly utf-8 encoded data from \uABCD and \U876543210 */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_ssize TPPCALL
tpp_lexer_decodestring(tpp_lexer const *tpp_restrict self,
                       tpp_formatprinter data_printer,
                       tpp_formatprinter utf8_printer,
                       void *arg);
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE */


#if TPP_HAVE_WARNINGS
/* Interpret + print a warning-message "format" string.
 * The following %-encoded escape sequences are recognized:
 * - "%["    Start quoting text
 * - "%]"    Stop quoting text
 * - "%Pl"   1-based line of "pos" in "file"
 * - "%Pc"   1-based column of "pos" in "file"
 * - "%Pf"   Filename of given "file"
 * - "%Pt"   "%[current-token%]"
 * - "%s"    As defined by stdc, using va_arg(args, char *)
 * - "%.*s"  As defined by stdc, using va_arg(args, int) + va_arg(args, char *)
 * - "%.Ns"  As defined by stdc, using va_arg(args, char *)
 * - "%.NPt" "%[<N bytes starting at "pos">%]"
 * - "%d"    As defined by stdc, using va_arg(args, int)
 * - "%u"    As defined by stdc, using va_arg(args, unsigned int)
 * - "%c"    As defined by stdc, using va_arg(args, int)
 * - "%%"    "%" (emit a singular %-character)
 *
 * @param: pos:     Lexer position used by certain format-patterns.
 * @param: file:    The file containing "pos"
 * @param: printer: Output printer for formatted text
 * @param: arg:     Cookie argument for "printer"
 * @param: format:  Format pattern (see above)
 * @param: args:    Extra varargs-arguments for "format"
 * @return: >= 0:   Sum of return values of "printer".
 * @return: < 0:    First negative return value of "printer". The more high-level
 *                  "tpp_lexer_warnf" API returns "TPP_EWARNPRINT" in this case. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4, 6)) tpp_ssize TPPVCALL
tpp_lexer_printf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                         tpp_formatprinter printer, void *arg,
                         char const *format, ...);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4, 6)) tpp_ssize TPPCALL
tpp_lexer_vprintf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                          tpp_formatprinter printer, void *arg,
                          char const *format, va_list args);

/* Emits the specified lexer warning at the start of the current token.
 * @param: args: Format arguments specific to "id" (see '%'-sequences in warning expressions)
 * @return: TPP_EOK:        Warning was emitted, but you may proceed
 * @return: TPP_ELEXERROR:  Warning was emitted, but was configured as an error
 * @return: TPP_EWARNPRINT: Error while printing warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, va_list args);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, ...);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_vwarnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos, tpp_warning_id id, va_list args);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos, tpp_warning_id id, ...);
#endif /* TPP_HAVE_WARNINGS */




#ifndef TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER
#define TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER 0
#endif /* !TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER */
#ifndef TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
#define TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER 0
#endif /* !TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */
#ifndef TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER
#define TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER 0
#endif /* !TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER */

#if TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER
TPP_DECL tpp_ssize TPP_FORMATPRINTER_CC _tpp_lexer_wrapped_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes);
#endif /* TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER */
#if TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
TPP_DECL tpp_ssize TPP_FORMATPRINTER_CC _tpp_lexer_builtin_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes);
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */
#if TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER
TPP_DECL tpp_ssize TPP_FORMATPRINTER_CC _tpp_lexer_noop_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes);
#endif /* TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER */

TPP_DECL_END
/************************************************************************/


#endif /* !GUARD_TPP_H */
#endif /* !TPP_DEFS */
/* clang-format on */
