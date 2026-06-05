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
	"assert": "TPP_HAVE_CPP_ASSERT",
	"unassert": "TPP_HAVE_CPP_ASSERT",
	"error": "TPP_HAVE_CPP_ERROR",
	"warning": "TPP_HAVE_CPP_WARNING",
	"ident": "TPP_HAVE_CPP_IDENT",
	"scss": "TPP_HAVE_CPP_SCSS",
	"pragma": "TPP_HAVE_CPP_PRAGMA",
	"_Pragma": "TPP_HAVE_STDC_PRAGMA",
	"__pragma": "TPP_HAVE_MSVC_PRAGMA",
	"push_macro": "TPP_HAVE_PRAGMA_PUSH_MACRO",
	"pop_macro": "TPP_HAVE_PRAGMA_PUSH_MACRO",
	"once": "TPP_HAVE_PRAGMA_ONCE",
	"__has_attribute": "TPP_HAVE_CLANG_HAS_ATTRIBUTE",
	"__has_builtin": "TPP_HAVE_CLANG_HAS_BUILTIN",
	"__has_cpp_attribute": "TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE",
	"__has_declspec_attribute": "TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE",
	"__has_extension": "TPP_HAVE_CLANG_HAS_EXTENSION || TPP_HAVE_TPPX_HAS_EXTENSION",
	"__has_feature": "TPP_HAVE_CLANG_HAS_FEATURE",
	"__has_c_attribute": "TPP_HAVE_CLANG_HAS_C_ATTRIBUTE",
	"__is_identifier": "TPP_HAVE_CLANG_IS_IDENTIFIER",
	"__is_deprecated": "TPP_HAVE_TPPX_IS_DEPRECATED",
	"__is_poisoned": "TPP_HAVE_TPPX_IS_POISONED",
	"__has_known_extension": "TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION",
	"__has_warning": "TPP_HAVE_TPPX_HAS_WARNING",
	"__has_known_warning": "TPP_HAVE_TPPX_HAS_KNOWN_WARNING",
	"__TPP_VERSION__": "TPP_HAVE_CPP_MACROS",
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
#if TPP_HAVE_CLANG_HAS_ATTRIBUTE
#define TPP_KWD___has_attribute TPP_KWD___has_attribute
TPP_KWD(TPP_KWD___has_attribute, "__has_attribute")
#endif /* TPP_HAVE_CLANG_HAS_ATTRIBUTE */
#if TPP_HAVE_CLANG_HAS_BUILTIN
#define TPP_KWD___has_builtin TPP_KWD___has_builtin
TPP_KWD(TPP_KWD___has_builtin, "__has_builtin")
#endif /* TPP_HAVE_CLANG_HAS_BUILTIN */
#if TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE
#define TPP_KWD___has_cpp_attribute TPP_KWD___has_cpp_attribute
TPP_KWD(TPP_KWD___has_cpp_attribute, "__has_cpp_attribute")
#endif /* TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE */
#if TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE
#define TPP_KWD___has_declspec_attribute TPP_KWD___has_declspec_attribute
TPP_KWD(TPP_KWD___has_declspec_attribute, "__has_declspec_attribute")
#endif /* TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE */
#if TPP_HAVE_CLANG_HAS_EXTENSION || TPP_HAVE_TPPX_HAS_EXTENSION
#define TPP_KWD___has_extension TPP_KWD___has_extension
TPP_KWD(TPP_KWD___has_extension, "__has_extension")
#endif /* TPP_HAVE_CLANG_HAS_EXTENSION || TPP_HAVE_TPPX_HAS_EXTENSION */
#if TPP_HAVE_CLANG_HAS_FEATURE
#define TPP_KWD___has_feature TPP_KWD___has_feature
TPP_KWD(TPP_KWD___has_feature, "__has_feature")
#endif /* TPP_HAVE_CLANG_HAS_FEATURE */
#if TPP_HAVE_CLANG_HAS_C_ATTRIBUTE
#define TPP_KWD___has_c_attribute TPP_KWD___has_c_attribute
TPP_KWD(TPP_KWD___has_c_attribute, "__has_c_attribute")
#endif /* TPP_HAVE_CLANG_HAS_C_ATTRIBUTE */
#if TPP_HAVE_CLANG_IS_IDENTIFIER
#define TPP_KWD___is_identifier TPP_KWD___is_identifier
TPP_KWD(TPP_KWD___is_identifier, "__is_identifier")
#endif /* TPP_HAVE_CLANG_IS_IDENTIFIER */
#if TPP_HAVE_TPPX_IS_DEPRECATED
#define TPP_KWD___is_deprecated TPP_KWD___is_deprecated
TPP_KWD(TPP_KWD___is_deprecated, "__is_deprecated")
#endif /* TPP_HAVE_TPPX_IS_DEPRECATED */
#if TPP_HAVE_TPPX_IS_POISONED
#define TPP_KWD___is_poisoned TPP_KWD___is_poisoned
TPP_KWD(TPP_KWD___is_poisoned, "__is_poisoned")
#endif /* TPP_HAVE_TPPX_IS_POISONED */
#if TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION
#define TPP_KWD___has_known_extension TPP_KWD___has_known_extension
TPP_KWD(TPP_KWD___has_known_extension, "__has_known_extension")
#endif /* TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION */
#if TPP_HAVE_TPPX_HAS_WARNING
#define TPP_KWD___has_warning TPP_KWD___has_warning
TPP_KWD(TPP_KWD___has_warning, "__has_warning")
#endif /* TPP_HAVE_TPPX_HAS_WARNING */
#if TPP_HAVE_TPPX_HAS_KNOWN_WARNING
#define TPP_KWD___has_known_warning TPP_KWD___has_known_warning
TPP_KWD(TPP_KWD___has_known_warning, "__has_known_warning")
#endif /* TPP_HAVE_TPPX_HAS_KNOWN_WARNING */
#if TPP_HAVE_CPP_MACROS
#define TPP_KWD___TPP_VERSION__ TPP_KWD___TPP_VERSION__
TPP_KWD(TPP_KWD___TPP_VERSION__, "__TPP_VERSION__")
#endif /* TPP_HAVE_CPP_MACROS */
/*[[[end]]]*/



#if TPP_HAVE_CPP_MACROS
TPP_MACRO(TPP_KWD___TPP_VERSION__, 1)
TPP_BUILTIN_MACRO(TPP_KWD___TPP_VERSION__, "300")
#endif /* TPP_HAVE_CPP_MACROS */

/* Declare magic, built-in macros */
#if TPP_HAVE_CLANG_HAS_ATTRIBUTE
TPP_MACRO(TPP_KWD___has_attribute, tpp_lexer_getext(self, TPP_EXT_CLANG_HAS_ATTRIBUTE))
#endif /* TPP_HAVE_CLANG_HAS_ATTRIBUTE */
#if TPP_HAVE_CLANG_HAS_BUILTIN
TPP_MACRO(TPP_KWD___has_builtin, tpp_lexer_getext(self, TPP_EXT_CLANG_HAS_BUILTIN))
#endif /* TPP_HAVE_CLANG_HAS_BUILTIN */
#if TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE
TPP_MACRO(TPP_KWD___has_cpp_attribute, tpp_lexer_getext(self, TPP_EXT_CLANG_HAS_CPP_ATTRIBUTE))
#endif /* TPP_HAVE_CLANG_HAS_CPP_ATTRIBUTE */
#if TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE
TPP_MACRO(TPP_KWD___has_declspec_attribute, tpp_lexer_getext(self, TPP_EXT_CLANG_HAS_DECLSPEC_ATTRIBUTE))
#endif /* TPP_HAVE_CLANG_HAS_DECLSPEC_ATTRIBUTE */
#if TPP_HAVE_CLANG_HAS_EXTENSION || TPP_HAVE_TPPX_HAS_EXTENSION
TPP_MACRO(TPP_KWD___has_extension, tpp_lexer_getext(self, TPP_EXT_CLANG_HAS_EXTENSION) ||
                                   tpp_lexer_getext(self, TPP_EXT_TPPX_HAS_EXTENSION))
#endif /* TPP_HAVE_CLANG_HAS_EXTENSION || TPP_HAVE_TPPX_HAS_EXTENSION */
#if TPP_HAVE_CLANG_HAS_FEATURE
TPP_MACRO(TPP_KWD___has_feature, tpp_lexer_getext(self, TPP_EXT_CLANG_HAS_FEATURE))
#endif /* TPP_HAVE_CLANG_HAS_FEATURE */
#if TPP_HAVE_CLANG_HAS_C_ATTRIBUTE
TPP_MACRO(TPP_KWD___has_c_attribute, tpp_lexer_getext(self, TPP_EXT_CLANG_HAS_C_ATTRIBUTE))
#endif /* TPP_HAVE_CLANG_HAS_C_ATTRIBUTE */
#if TPP_HAVE_CLANG_IS_IDENTIFIER
TPP_MACRO(TPP_KWD___is_identifier, tpp_lexer_getext(self, TPP_EXT_CLANG_IS_IDENTIFIER))
#endif /* TPP_HAVE_CLANG_IS_IDENTIFIER */
#if TPP_HAVE_TPPX_IS_DEPRECATED
TPP_MACRO(TPP_KWD___is_deprecated, tpp_lexer_getext(self, TPP_EXT_TPPX_IS_DEPRECATED))
#endif /* TPP_HAVE_TPPX_IS_DEPRECATED */
#if TPP_HAVE_TPPX_IS_POISONED
TPP_MACRO(TPP_KWD___is_poisoned, tpp_lexer_getext(self, TPP_EXT_TPPX_IS_POISONED))
#endif /* TPP_HAVE_TPPX_IS_POISONED */
#if TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION
TPP_MACRO(TPP_KWD___has_known_extension, tpp_lexer_getext(self, TPP_EXT_TPPX_HAS_KNOWN_EXTENSION))
#endif /* TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION */
#if TPP_HAVE_TPPX_HAS_WARNING
TPP_MACRO(TPP_KWD___has_warning, tpp_lexer_getext(self, TPP_EXT_TPPX_HAS_WARNING))
#endif /* TPP_HAVE_TPPX_HAS_WARNING */
#if TPP_HAVE_TPPX_HAS_KNOWN_WARNING
TPP_MACRO(TPP_KWD___has_known_warning, tpp_lexer_getext(self, TPP_EXT_TPPX_HAS_KNOWN_WARNING))
#endif /* TPP_HAVE_TPPX_HAS_KNOWN_WARNING */


/*[[[deemon
for (local feat: {
	"TRIGRAPHS",
	"DIGRAPHS",
	"BSE",
	"BSE_WHITESPACE",
	"ESCAPE_E_IN_STRINGS",
	"ESCAPE_S_IN_STRINGS",
	"CLANG_HAS_ATTRIBUTE",
	"CLANG_HAS_BUILTIN",
	"CLANG_HAS_CPP_ATTRIBUTE",
	"CLANG_HAS_DECLSPEC_ATTRIBUTE",
	"CLANG_HAS_EXTENSION",
	"CLANG_HAS_FEATURE",
	"CLANG_HAS_C_ATTRIBUTE",
	"CLANG_IS_IDENTIFIER",
	"TPPX_IS_DEPRECATED",
	"TPPX_IS_POISONED",
	"TPPX_HAS_EXTENSION",
	"TPPX_HAS_KNOWN_EXTENSION",
	"TPPX_HAS_WARNING",
	"TPPX_HAS_KNOWN_WARNING",
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
#if TPP_HAVE_ESCAPE_E_IN_STRINGS < 0
TPP_EXTENSION(TPP_EXT_ESCAPE_E_IN_STRINGS, "escape-e-in-strings", TPP_HAVE_ESCAPE_E_IN_STRINGS == -1)
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_E_IN_STRINGS(self) (self)->tes_flags.tef_TPP_EXT_ESCAPE_E_IN_STRINGS
#else /* TPP_HAVE_ESCAPE_E_IN_STRINGS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_E_IN_STRINGS(self) TPP_HAVE_ESCAPE_E_IN_STRINGS
#endif /* TPP_HAVE_ESCAPE_E_IN_STRINGS >= 0 */
#if TPP_HAVE_ESCAPE_S_IN_STRINGS < 0
TPP_EXTENSION(TPP_EXT_ESCAPE_S_IN_STRINGS, "escape-s-in-strings", TPP_HAVE_ESCAPE_S_IN_STRINGS == -1)
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_S_IN_STRINGS(self) (self)->tes_flags.tef_TPP_EXT_ESCAPE_S_IN_STRINGS
#else /* TPP_HAVE_ESCAPE_S_IN_STRINGS < 0 */
#define _tpp_extensions_state_get_TPP_EXT_ESCAPE_S_IN_STRINGS(self) TPP_HAVE_ESCAPE_S_IN_STRINGS
#endif /* TPP_HAVE_ESCAPE_S_IN_STRINGS >= 0 */
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
#if TPP_HAVE_CLANG_HAS_C_ATTRIBUTE < 0
TPP_EXTENSION(TPP_EXT_CLANG_HAS_C_ATTRIBUTE, "clang-has-c-attribute", TPP_HAVE_CLANG_HAS_C_ATTRIBUTE == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_C_ATTRIBUTE(self) (self)->tes_flags.tef_TPP_EXT_CLANG_HAS_C_ATTRIBUTE
#else /* TPP_HAVE_CLANG_HAS_C_ATTRIBUTE < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_HAS_C_ATTRIBUTE(self) TPP_HAVE_CLANG_HAS_C_ATTRIBUTE
#endif /* TPP_HAVE_CLANG_HAS_C_ATTRIBUTE >= 0 */
#if TPP_HAVE_CLANG_IS_IDENTIFIER < 0
TPP_EXTENSION(TPP_EXT_CLANG_IS_IDENTIFIER, "clang-is-identifier", TPP_HAVE_CLANG_IS_IDENTIFIER == -1)
#define _tpp_extensions_state_get_TPP_EXT_CLANG_IS_IDENTIFIER(self) (self)->tes_flags.tef_TPP_EXT_CLANG_IS_IDENTIFIER
#else /* TPP_HAVE_CLANG_IS_IDENTIFIER < 0 */
#define _tpp_extensions_state_get_TPP_EXT_CLANG_IS_IDENTIFIER(self) TPP_HAVE_CLANG_IS_IDENTIFIER
#endif /* TPP_HAVE_CLANG_IS_IDENTIFIER >= 0 */
#if TPP_HAVE_TPPX_IS_DEPRECATED < 0
TPP_EXTENSION(TPP_EXT_TPPX_IS_DEPRECATED, "tppx-is-deprecated", TPP_HAVE_TPPX_IS_DEPRECATED == -1)
#define _tpp_extensions_state_get_TPP_EXT_TPPX_IS_DEPRECATED(self) (self)->tes_flags.tef_TPP_EXT_TPPX_IS_DEPRECATED
#else /* TPP_HAVE_TPPX_IS_DEPRECATED < 0 */
#define _tpp_extensions_state_get_TPP_EXT_TPPX_IS_DEPRECATED(self) TPP_HAVE_TPPX_IS_DEPRECATED
#endif /* TPP_HAVE_TPPX_IS_DEPRECATED >= 0 */
#if TPP_HAVE_TPPX_IS_POISONED < 0
TPP_EXTENSION(TPP_EXT_TPPX_IS_POISONED, "tppx-is-poisoned", TPP_HAVE_TPPX_IS_POISONED == -1)
#define _tpp_extensions_state_get_TPP_EXT_TPPX_IS_POISONED(self) (self)->tes_flags.tef_TPP_EXT_TPPX_IS_POISONED
#else /* TPP_HAVE_TPPX_IS_POISONED < 0 */
#define _tpp_extensions_state_get_TPP_EXT_TPPX_IS_POISONED(self) TPP_HAVE_TPPX_IS_POISONED
#endif /* TPP_HAVE_TPPX_IS_POISONED >= 0 */
#if TPP_HAVE_TPPX_HAS_EXTENSION < 0
TPP_EXTENSION(TPP_EXT_TPPX_HAS_EXTENSION, "tppx-has-extension", TPP_HAVE_TPPX_HAS_EXTENSION == -1)
#define _tpp_extensions_state_get_TPP_EXT_TPPX_HAS_EXTENSION(self) (self)->tes_flags.tef_TPP_EXT_TPPX_HAS_EXTENSION
#else /* TPP_HAVE_TPPX_HAS_EXTENSION < 0 */
#define _tpp_extensions_state_get_TPP_EXT_TPPX_HAS_EXTENSION(self) TPP_HAVE_TPPX_HAS_EXTENSION
#endif /* TPP_HAVE_TPPX_HAS_EXTENSION >= 0 */
#if TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION < 0
TPP_EXTENSION(TPP_EXT_TPPX_HAS_KNOWN_EXTENSION, "tppx-has-known-extension", TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION == -1)
#define _tpp_extensions_state_get_TPP_EXT_TPPX_HAS_KNOWN_EXTENSION(self) (self)->tes_flags.tef_TPP_EXT_TPPX_HAS_KNOWN_EXTENSION
#else /* TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION < 0 */
#define _tpp_extensions_state_get_TPP_EXT_TPPX_HAS_KNOWN_EXTENSION(self) TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION
#endif /* TPP_HAVE_TPPX_HAS_KNOWN_EXTENSION >= 0 */
#if TPP_HAVE_TPPX_HAS_WARNING < 0
TPP_EXTENSION(TPP_EXT_TPPX_HAS_WARNING, "tppx-has-warning", TPP_HAVE_TPPX_HAS_WARNING == -1)
#define _tpp_extensions_state_get_TPP_EXT_TPPX_HAS_WARNING(self) (self)->tes_flags.tef_TPP_EXT_TPPX_HAS_WARNING
#else /* TPP_HAVE_TPPX_HAS_WARNING < 0 */
#define _tpp_extensions_state_get_TPP_EXT_TPPX_HAS_WARNING(self) TPP_HAVE_TPPX_HAS_WARNING
#endif /* TPP_HAVE_TPPX_HAS_WARNING >= 0 */
#if TPP_HAVE_TPPX_HAS_KNOWN_WARNING < 0
TPP_EXTENSION(TPP_EXT_TPPX_HAS_KNOWN_WARNING, "tppx-has-known-warning", TPP_HAVE_TPPX_HAS_KNOWN_WARNING == -1)
#define _tpp_extensions_state_get_TPP_EXT_TPPX_HAS_KNOWN_WARNING(self) (self)->tes_flags.tef_TPP_EXT_TPPX_HAS_KNOWN_WARNING
#else /* TPP_HAVE_TPPX_HAS_KNOWN_WARNING < 0 */
#define _tpp_extensions_state_get_TPP_EXT_TPPX_HAS_KNOWN_WARNING(self) TPP_HAVE_TPPX_HAS_KNOWN_WARNING
#endif /* TPP_HAVE_TPPX_HAS_KNOWN_WARNING >= 0 */
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
TPP_WARNING(TPP_W_SLASHSTAR_INSIDE_OF_COMMENT, 1(TPP_WG_COMMENT), 0(), "%[/*%] repeated inside of comment")
#endif /* TPP_HAVE_TPP_W_SLASHSTAR_INSIDE_OF_COMMENT */

#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
#define TPP_W_LINE_COMMENT_CONTINUED TPP_W_LINE_COMMENT_CONTINUED
TPP_WARNING(TPP_W_LINE_COMMENT_CONTINUED, 1(TPP_WG_COMMENT), 1(4010), "line-comment continued")
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
TPP_WARNING(TPP_W_ENCOUNTERED_TRIGRAPH, 1(TPP_WG_TRIGRAPHS), 0(),
            "encountered trigraph character sequence %.3Pt")
#endif /* TPP_HAVE_TPP_W_ENCOUNTERED_TRIGRAPH */


/************************************************************************/
/* -Wsyntax                                                             */
/************************************************************************/
#ifndef TPP_HAVE_TPP_WG_SYNTAX
#define TPP_HAVE_TPP_WG_SYNTAX                       \
	(TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED || \
	 TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF ||      \
	 TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF ||     \
	 TPP_HAVE_TPP_W_UNEXPECTED_TOKEN)
#endif /* !TPP_HAVE_TPP_WG_SYNTAX */
#if TPP_HAVE_TPP_WG_SYNTAX
#define TPP_WG_SYNTAX TPP_WG_SYNTAX
TPP_WGROUP(TPP_WG_SYNTAX, 1("syntax"), TPP_WSTATE_ERROR_OR_FATAL)
#endif /* TPP_HAVE_TPP_WG_SYNTAX */

#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
#define TPP_W_STRING_TERMINATED_BY_LINEFEED TPP_W_STRING_TERMINATED_BY_LINEFEED
TPP_WARNING(TPP_W_STRING_TERMINATED_BY_LINEFEED, 1(TPP_WG_SYNTAX), 0(), "string was terminated by a linefeed")
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */

#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
#define TPP_W_STRING_TERMINATED_BY_EOF TPP_W_STRING_TERMINATED_BY_EOF
TPP_WARNING(TPP_W_STRING_TERMINATED_BY_EOF, 1(TPP_WG_SYNTAX), 0(), "string was terminated by EOF")
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */

#if TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF
#define TPP_W_COMMENT_TERMINATED_BY_EOF TPP_W_COMMENT_TERMINATED_BY_EOF
TPP_WARNING(TPP_W_COMMENT_TERMINATED_BY_EOF, 1(TPP_WG_SYNTAX), 0(), "comment was terminated by EOF")
#endif /* TPP_HAVE_TPP_W_COMMENT_TERMINATED_BY_EOF */

#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN
#define TPP_W_UNEXPECTED_TOKEN TPP_W_UNEXPECTED_TOKEN
TPP_WARNING(TPP_W_UNEXPECTED_TOKEN, 1(TPP_WG_SYNTAX), 0(), "expected %[%s%] but got %Pt")
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN */


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
TPP_WARNING(TPP_W_ERROR, 1(TPP_WG_USER), 0(), "%.*s")
#endif /* TPP_HAVE_TPP_W_ERROR */
#if TPP_HAVE_TPP_W_WARNING
#define TPP_W_WARNING TPP_W_WARNING
TPP_WARNING(TPP_W_WARNING, 2(TPP_WG_USER, TPP_WG_CPP), 0(), "%.*s")
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
TPP_WARNING(TPP_W_UNKNOWN_PRAGMAS, 1(TPP_WG_UNKNOWN_PRAGMAS), 1(4068), "unknown pragma %Pt")
#endif /* TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
#define TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
TPP_WARNING(TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE, 1(TPP_WG_UNKNOWN_PRAGMAS), 1(4083),
            "extra tokens at end of %[#pragma%] directive")
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
TPP_WARNING(TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER, 1(TPP_WG_PRAGMA_ONCE_OUTSIDE_HEADER), 0(),
            "%[#pragma%] once in main file")
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
TPP_WARNING(TPP_W_DEPRECATED_KEYWORD, 1(TPP_WG_DEPRECATED), 1(4995),
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
TPP_WARNING(TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE, 1(TPP_WG_STRING_ESCAPE), 1(4129),
            "unknown escape sequence %[\\%c%]")
#endif /* TPP_HAVE_TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE */


/************************************************************************/
/* Misc warnings...                                                     */
/************************************************************************/
#if TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK
#define TPP_W_POP_MACRO_EMPTY_STACK TPP_W_POP_MACRO_EMPTY_STACK
TPP_WARNING(TPP_W_POP_MACRO_EMPTY_STACK, 0(), 1(4602),
            "cannot %[#pragma pop_macro(\"%.*s\")%]: no preceding %[#pragma push_macro%]")
#endif /* TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */



TPP_WGROUP(TPP_WG_MACROS, /*         */ 1("macros"),               TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_USAGE, /*          */ 1("usage"),                TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_BOOLVALUE, /*      */ 1("boolean-value"),        TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_ENVIRON, /*        */ 1("environ"),              TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_LIMIT, /*          */ 1("limit"),                TPP_WSTATE_FATAL)
TPP_WGROUP(TPP_WG_UNDEF, /*          */ 1("undef"),                TPP_WSTATE_WARN)
TPP_WGROUP(TPP_WG_EXPANSION_TO_DEFINED, 1("expansion-to-defined"), TPP_WSTATE_WARN)
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
