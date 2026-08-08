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
/*!depends config.h*/
#ifndef GUARD_TPP_HOOKS_H
#define GUARD_TPP_HOOKS_H 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "expr.h"
#include "keyword.h"
#include "macro.h"
#include "warnings.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK)
#ifndef tpp_lexer_foreach_include_path_flags__PARAM
#if TPP_HAVE_FILE_SYSHDR
#define tpp_lexer_foreach_include_path_flags__PARAM  , tpp_file_flags flags
#define tpp_lexer_foreach_include_path_flags__ARG(x) , x
#else /* TPP_HAVE_FILE_SYSHDR */
#define tpp_lexer_foreach_include_path_flags__PARAM  /* nothing */
#define tpp_lexer_foreach_include_path_flags__ARG(x) /* nothing */
#endif /* !TPP_HAVE_FILE_SYSHDR */
#endif /* !tpp_lexer_foreach_include_path_flags__PARAM */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */

/* Possible values for `tpp_hooks_call_system_include_path(when)` */
#if TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK
typedef enum tpp_hook_system_include_path_when {
	TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_FIRST,         /* Called at the very start */
#if TPP_HAVE_INCLUDE_PATH
#if TPP_HAVE_INCLUDE_PATH_QUOTE
	TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_QUOTE,  /* Called for `"`-paths after trying to import relative to current file, but before `tip_quote_list` is checked */
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
	TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_SYSTEM, /* Called before `tip_system_list` is checked */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
	TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_SYSHDR, /* Called before `tip_syshdr_list` is checked */
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#endif /* TPP_HAVE_INCLUDE_PATH */
#if TPP_HAVE_INCLUDE_SYSTEM_INCLUDE_PATH
	TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_CONFIG, /* Called before `TPP_CONFIG_SYSTEM_INCLUDE_PATH` is checked */
#endif /* TPP_HAVE_INCLUDE_SYSTEM_INCLUDE_PATH */
#if TPP_HAVE_INCLUDE_PATH && TPP_HAVE_INCLUDE_PATH_AFTER
	TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_AFTER,  /* Called before `tip_after_list` is checked */
#endif /* TPP_HAVE_INCLUDE_PATH && TPP_HAVE_INCLUDE_PATH_AFTER */
	TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_LAST,          /* Called at the very end */
} tpp_hook_system_include_path_when;
#endif /* TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK */


#if TPP_HAVE_SYSTEM_EMBED_PATH_HOOK
typedef enum tpp_hook_system_embed_path_when {
	TPP_HOOK_SYSTEM_EMBED_PATH_WHEN_FIRST,         /* Called at the very start */
#if TPP_HAVE_INCLUDE_PATH_EMBED
	TPP_HOOK_SYSTEM_EMBED_PATH_WHEN_BEFORE_SYSTEM, /* Called before `tip_embed_list` is checked */
#endif /* TPP_HAVE_INCLUDE_PATH_EMBED */
	TPP_HOOK_SYSTEM_EMBED_PATH_WHEN_LAST,          /* Called at the very end */
} tpp_hook_system_embed_path_when;
#endif /* TPP_HAVE_SYSTEM_EMBED_PATH_HOOK */


#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK
typedef enum tpp_hook_include_kind {
#if TPP_HAVE_CPP_INCLUDE
	TPP_HOOK_INCLUDE_KIND_INCLUDE, /* `#include "foo"` or `#include <foo>` */
#endif /* TPP_HAVE_CPP_INCLUDE */
#if TPP_HAVE_CPP_INCLUDE_NEXT
	TPP_HOOK_INCLUDE_KIND_INCLUDE_NEXT, /* `#include_next "foo"` or `#include_next <foo>` */
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
#if TPP_HAVE_CPP_IMPORT
	TPP_HOOK_INCLUDE_KIND_IMPORT, /* `#import "foo"` or `#import <foo>` */
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_CPP_EMBED
	TPP_HOOK_INCLUDE_KIND_EMBED, /* `#embed "foo"` or `#embed <foo>` */
#endif /* TPP_HAVE_CPP_EMBED */
} tpp_hook_include_kind;
#endif /* TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK */




/*[[[deemon
import HOOKS from .config;

print("#undef TPP_HAVE_HOOKS");
local longestNameLen = HOOKS.each[1].length > ...;
print("#if (", "\\\n     ".join(
	for (local h: HOOKS)
		f"TPP_HOOK_ISRT(TPP_HAVE_{h[1]}_HOOK) || " +
			" " * (longestNameLen - #h[1])
).rstrip(" |"), ")");
print("#define TPP_HAVE_HOOKS 1");
print("#else /" "* ... *" "/");
print("#define TPP_HAVE_HOOKS 0");
print("#endif /" "* !... *" "/");
print;
print("#if TPP_HAVE_HOOKS");
print("#if TPP_HAVE_HOOK_COOKIES");
print("#define tpp_hook_cookie void *");
print("#else /" "* TPP_HAVE_HOOK_COOKIES *" "/");
print("struct tpp_lexer;");
print("#define tpp_hook_cookie struct tpp_lexer *");
print("#endif /" "* !TPP_HAVE_HOOK_COOKIES *" "/");
print("struct tpp_lexer_decodestring_config;");
print("#if TPP_HAVE_WARNINGS");
print("struct tpp_lexer_printf_info;");
print("#endif /" "* TPP_HAVE_WARNINGS *" "/");
print("typedef struct tpp_hooks {");
function fixProto(x) -> x
	.rereplace(r"\btpp_lexer_printf_info\b", "struct tpp_lexer_printf_info")
	.rereplace(r"\btpp_lexer_decodestring_config\b", "struct tpp_lexer_decodestring_config")
	;
local isFirst = true;
for (local doc, name,
     default_TPP_HAVE_FOO_HOOK,
     builtin_FOO_HOOK,
     prototypePrefix,
     prototypeSuffix,
     prototypeArgs,
     disabled_RETURN_VALUE: HOOKS) {
	if (!isFirst)
		print;
	isFirst = false;
	print("	/" "* >> ", fixProto(prototypePrefix), "th_", name.lower(), fixProto(prototypeSuffix), ";");
	print("	 * ", doc.strip().replace("\n", "\n\t * ").rstriplines(), " *" "/");
	print("#if TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK)");
	if (builtin_FOO_HOOK) {
		print("#if TPP_HAVE_", name, "_HOOK != TPP_HOOK_RT_NOOP");
		print("	", fixProto(prototypePrefix), "TPP_INTERNAL(th_", name.lower(), ")", fixProto(prototypeSuffix), "; /" "* [1..1] *" "/");
		print("#else /" "* TPP_HAVE_", name, "_HOOK != TPP_HOOK_RT_NOOP *" "/");
	}
	print("	", fixProto(prototypePrefix), "TPP_INTERNAL(th_", name.lower(), ")", fixProto(prototypeSuffix), "; /" "* [0..1] *" "/");
	if (builtin_FOO_HOOK)
		print("#endif /" "* TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_NOOP *" "/");
	if ("cookie" in prototypeArgs)
		print("	tpp_hook_cookie TPP_INTERNAL(th_", name.lower(), "_cookie); /" "* [?..?] Cookie argument for `th_", name.lower(), "` *" "/");
	print("#endif /" "* TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK) *" "/");
}
print("} tpp_hooks;");
print("#endif /" "* TPP_HAVE_HOOKS *" "/");
print;
for (local doc, name,
     default_TPP_HAVE_FOO_HOOK,
     builtin_FOO_HOOK,
     prototypePrefix,
     prototypeSuffix,
     prototypeArgs,
     disabled_RETURN_VALUE: HOOKS) {
	local builtinNeedsCookieAsLexer = builtin_FOO_HOOK &&
		builtin_FOO_HOOK !in ["_tpp_lexer_builtin_warn_or_mesg_printer"];

	print("/" "* ", doc.strip().replace("\n", "\n * ").rstriplines(), " *" "/");
	print("#if TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK)");
	local isFormatPrinter = prototypePrefix.strip() == "tpp_formatprinter" && !prototypeSuffix;
	local hookMustBeFunctionPointer = isFormatPrinter;
	local formattedParams = "self, lexer" + "".join(for (local x: prototypeArgs)
		if (x !in ["cookie", "lexer"]) f", {x}");
	local formattedArgs = ", ".join(for (local arg: prototypeArgs)
		arg != "cookie" ? arg : f"tpp_hooks_getcookie_{name.lower()}(self, lexer)");
	if (builtin_FOO_HOOK) {
		print("#if TPP_HAVE_", name, "_HOOK != TPP_HOOK_RT_NOOP");
		print("#define tpp_hooks_call_", name.lower(), "(", formattedParams, ") \\");
		if (isFormatPrinter) {
			print("	tpp_formatprinter_print((self)->TPP_INTERNAL(th_", name.lower(), "), ", formattedArgs, ")");
		} else {
			print("	(*(self)->TPP_INTERNAL(th_", name.lower(), "))(", formattedArgs, ")");
		}
		print("#else /" "* TPP_HAVE_", name, "_HOOK != TPP_HOOK_RT_NOOP *" "/");
	}
	print("#define tpp_hooks_call_", name.lower(), "(", formattedParams, ") \\");
	print("	((self)->TPP_INTERNAL(th_", name.lower(), ") "),;
	if (isFormatPrinter) {
		print("? tpp_formatprinter_print((self)->TPP_INTERNAL(th_", name.lower(), "), ", formattedArgs, ") "),;
	} else {
		print("? (*(self)->TPP_INTERNAL(th_", name.lower(), "))(", formattedArgs, ") "),;
	}
	print(": ", disabled_RETURN_VALUE, ")");
	if (builtin_FOO_HOOK)
		print("#endif /" "* TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_NOOP *" "/");
	print("#if TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_", name, ")");
	print("#define _TPP_HOOKS_DEFAULT_", name, " (&TPP_HOOK_", name, ")");
	if (builtin_FOO_HOOK) {
		print("#elif TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_BUILTIN");
		print("#define _TPP_HOOKS_DEFAULT_", name, " (&", builtin_FOO_HOOK, ")");
	}
	print("#else /" "* ... *" "/");
	print("#define _TPP_HOOKS_DEFAULT_", name, " NULL");
	print("#endif /" "* !... *" "/");
	if ("cookie" in prototypeArgs) {
		print("#if TPP_HAVE_HOOK_COOKIES");
		print("#define tpp_hooks_getcookie_", name.lower(), "(self, lexer)  (self)->TPP_INTERNAL(th_", name.lower(), "_cookie)");
		print("#define tpp_hooks_set_", name.lower(), "(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_", name.lower(), ") = (v), (self)->TPP_INTERNAL(th_", name.lower(), "_cookie) = (lexer))");
		print("#define tpp_hooks_set_", name.lower(), "_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_", name.lower(), ") = (v), (self)->TPP_INTERNAL(th_", name.lower(), "_cookie) = (cookie))");
		if (builtinNeedsCookieAsLexer) {
			print("#define tpp_hooks_reset_", name.lower(), "(self, lexer)      (void)((self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name, ", (self)->TPP_INTERNAL(th_", name.lower(), "_cookie) = (lexer))");
			print("#define _tpp_hooks_init_", name.lower(), "(self, lexer)      , (self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name, ", (self)->TPP_INTERNAL(th_", name.lower(), "_cookie) = (lexer)");
		} else {
			print("#if TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_", name, ")");
			print("#define tpp_hooks_reset_", name.lower(), "(self, lexer) (void)((self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name, ", (self)->TPP_INTERNAL(th_", name.lower(), "_cookie) = (lexer))");
			print("#define _tpp_hooks_init_", name.lower(), "(self, lexer) , (self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name, ", (self)->TPP_INTERNAL(th_", name.lower(), "_cookie) = (lexer)");
			print("#else /" "* TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_", name, " *" "/");
			print("#define tpp_hooks_reset_", name.lower(), "(self, lexer) (void)((self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name, ")");
			print("#define _tpp_hooks_init_", name.lower(), "(self, lexer) , (self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name);
			print("#endif /" "* TPP_HAVE_", name, "_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_", name, " *" "/");
		}
		print("#else /" "* TPP_HAVE_HOOK_COOKIES *" "/");
		print("#define tpp_hooks_getcookie_", name.lower(), "(self, lexer) (lexer)");
	}
	print("#define tpp_hooks_set_", name.lower(), "(self, lexer, v) (void)((self)->TPP_INTERNAL(th_", name.lower(), ") = (v))");
	print("#define tpp_hooks_reset_", name.lower(), "(self, lexer)  (void)((self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name, ")");
	print("#define _tpp_hooks_init_", name.lower(), "(self, lexer)  , (self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name);
	if ("cookie" !in prototypeArgs) {
		print("#define tpp_hooks_get_", name.lower(), "(self)           (self)->TPP_INTERNAL(th_", name.lower(), ")");
	} else {
		print("#endif /" "* !TPP_HAVE_HOOK_COOKIES *" "/");
		print("#define tpp_hooks_get_", name.lower(), "(self) (self)->TPP_INTERNAL(th_", name.lower(), ")");
	}
	print("#else /" "* TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK) *" "/");
	print("#if TPP_HAVE_", name, "_HOOK == TPP_HOOK_CONST_USER");
	if (hookMustBeFunctionPointer) {
		print("#define tpp_hooks_get_", name.lower(), "(self) (&TPP_HOOK_", name, ")");
		if ("cookie" in prototypeArgs)
			print("#define tpp_hooks_getcookie_", name.lower(), "(self, lexer) (lexer)");
	}
	print("#define tpp_hooks_call_", name.lower(), "(self", "".join(for (local x: prototypeArgs) f", {x}"), ") \\");
	print("	TPP_HOOK_", name, "(", ", ".join(prototypeArgs), ")");
	if (builtin_FOO_HOOK) {
		print("#elif TPP_HAVE_", name, "_HOOK == TPP_HOOK_CONST_BUILTIN");
		if (hookMustBeFunctionPointer) {
			print("#define tpp_hooks_get_", name.lower(), "(self) (&", builtin_FOO_HOOK, ")");
			if ("cookie" in prototypeArgs)
				print("#define tpp_hooks_getcookie_", name.lower(), "(self, lexer) (lexer)");
		}
		print("#define tpp_hooks_call_", name.lower(), "(self", "".join(for (local x: prototypeArgs) f", {x}"), ") \\");
		print("	", builtin_FOO_HOOK, "(", ", ".join(prototypeArgs), ")");
	}
	print("#else /" "*  *" "/");
	print("#define tpp_hooks_call_", name.lower(), "(self", "".join(for (local x: prototypeArgs) f", {x}"), ") ", disabled_RETURN_VALUE);
	print("#endif /" "* ... *" "/");
	print("#define _tpp_hooks_init_", name.lower(), "(self, lexer) /" "* nothing *" "/");
	print("#endif /" "* !TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK) *" "/");
	print;
}
print("/" "* Initialize lexer hooks *" "/");
print("#define tpp_hooks_init(self, lexer) \\");
print("	(void)((void)0 "),;
local isFirst = true;
for (local doc, name,
     default_TPP_HAVE_FOO_HOOK,
     builtin_FOO_HOOK,
     prototypePrefix,
     prototypeSuffix,
     prototypeArgs,
     disabled_RETURN_VALUE: HOOKS) {
	if (!isFirst)
		print(" \\\n\t       "),;
	isFirst = false;
	print("_tpp_hooks_init_", name.lower(), "(self, lexer)"),;
}
print(")");
]]]*/
#undef TPP_HAVE_HOOKS
#if (TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) ||           \
     TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK) ||           \
     TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK) ||           \
     TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) ||             \
     TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) ||        \
     TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) ||        \
     TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) ||           \
     TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK) ||           \
     TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) ||   \
     TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) ||     \
     TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK) ||         \
     TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK) ||       \
     TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK) ||            \
     TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) ||   \
     TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) ||     \
     TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) || \
     TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK) ||        \
     TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK))
#define TPP_HAVE_HOOKS 1
#else /* ... */
#define TPP_HAVE_HOOKS 0
#endif /* !... */

#if TPP_HAVE_HOOKS
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hook_cookie void *
#else /* TPP_HAVE_HOOK_COOKIES */
struct tpp_lexer;
#define tpp_hook_cookie struct tpp_lexer *
#endif /* !TPP_HAVE_HOOK_COOKIES */
struct tpp_lexer_decodestring_config;
#if TPP_HAVE_WARNINGS
struct tpp_lexer_printf_info;
#endif /* TPP_HAVE_WARNINGS */
typedef struct tpp_hooks {
	/* >> tpp_formatprinter th_warnprinter;
	 * Called by `tpp_lexer_warnf()` to print warning messages.
	 * Potentially unused if `TPP_HAVE_WARNHANDLER_HOOK` is also overwritten
	 * @return: >= 0: Success
	 * @return: TPP_SSIZE_OFERR(TPP_EIO):       I/O Error
	 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
	 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): Hard lexer error
	 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK)
#if TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP
	tpp_formatprinter TPP_INTERNAL(th_warnprinter); /* [1..1] */
#else /* TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP */
	tpp_formatprinter TPP_INTERNAL(th_warnprinter); /* [0..1] */
#endif /* TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_NOOP */
	tpp_hook_cookie TPP_INTERNAL(th_warnprinter_cookie); /* [?..?] Cookie argument for `th_warnprinter` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */

	/* >> tpp_errno (TPPCALL *th_warnhandler)(tpp_hook_cookie cookie, struct tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args);
	 * Called by `tpp_lexer_warnf()` to handle warning notifications. Can be
	 * overwritten to implement custom behavior in regards to handling of warnings.
	 * @param: info:       Warning context location
	 * @param: invokeinfo: Warning invocation method
	 * @param: id:         Warning ID
	 * @param: args:       Variable arguments passed to warning
	 * @return: TPP_EOK:       Success (warning was emitted)
	 * @return: TPP_EIO:       I/O Error
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_ELEXERROR: Hard lexer error
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK)
#if TPP_HAVE_WARNHANDLER_HOOK != TPP_HOOK_RT_NOOP
	tpp_errno (TPPCALL *TPP_INTERNAL(th_warnhandler))(tpp_hook_cookie cookie, struct tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args); /* [1..1] */
#else /* TPP_HAVE_WARNHANDLER_HOOK != TPP_HOOK_RT_NOOP */
	tpp_errno (TPPCALL *TPP_INTERNAL(th_warnhandler))(tpp_hook_cookie cookie, struct tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args); /* [0..1] */
#endif /* TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_RT_NOOP */
	tpp_hook_cookie TPP_INTERNAL(th_warnhandler_cookie); /* [?..?] Cookie argument for `th_warnhandler` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK) */

	/* >> tpp_formatprinter th_mesgprinter;
	 * Used by `#pragma message` to print messages (see `TPP_HAVE_PRAGMA_MESSAGE`)
	 * @return: >= 0: Success
	 * @return: TPP_SSIZE_OFERR(TPP_EIO):       I/O Error
	 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
	 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): Hard lexer error
	 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK)
#if TPP_HAVE_MESGPRINTER_HOOK != TPP_HOOK_RT_NOOP
	tpp_formatprinter TPP_INTERNAL(th_mesgprinter); /* [1..1] */
#else /* TPP_HAVE_MESGPRINTER_HOOK != TPP_HOOK_RT_NOOP */
	tpp_formatprinter TPP_INTERNAL(th_mesgprinter); /* [0..1] */
#endif /* TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_NOOP */
	tpp_hook_cookie TPP_INTERNAL(th_mesgprinter_cookie); /* [?..?] Cookie argument for `th_mesgprinter` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK) */

	/* >> tpp_errno (TPPCALL *th_parseexpr)(tpp_hook_cookie cookie, tpp_expr_value *tpp_restrict result);
	 * User-defined callback for parsing `#if`-style expressions
	 * - This callback is invoked in a context where the lexer
	 *   points before the expression's first token (meaning that
	 *   this callback is responsible to do the initial yield
	 *   using whatever method it wants to use).
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
	 * @return: TPP_EUSER(*):    User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK)
#if TPP_HAVE_PARSEEXPR_HOOK != TPP_HOOK_RT_NOOP
	tpp_errno (TPPCALL *TPP_INTERNAL(th_parseexpr))(tpp_hook_cookie cookie, tpp_expr_value *tpp_restrict result); /* [1..1] */
#else /* TPP_HAVE_PARSEEXPR_HOOK != TPP_HOOK_RT_NOOP */
	tpp_errno (TPPCALL *TPP_INTERNAL(th_parseexpr))(tpp_hook_cookie cookie, tpp_expr_value *tpp_restrict result); /* [0..1] */
#endif /* TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_RT_NOOP */
	tpp_hook_cookie TPP_INTERNAL(th_parseexpr_cookie); /* [?..?] Cookie argument for `th_parseexpr` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) */

	/* >> tpp_errno (TPPCALL *th_unknown_pragma)(tpp_hook_cookie cookie);
	 * Called whenever a `#pragma` is encountered that is not recognized.
	 * When called, the lexer is set-up to point at the first token after the `#pragma`.
	 * @return: TPP_EOK:      Pragma has been handled
	 * @return: TPP_ENOENT:   Pragma is still unknown, and a warning should be emitted
	 * @return: TPP_EIO:      I/O error
	 * @return: TPP_ENOMEM:   Out of memory
	 * @return: TPP_EUSER(*): User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_unknown_pragma))(tpp_hook_cookie cookie); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_unknown_pragma_cookie); /* [?..?] Cookie argument for `th_unknown_pragma` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */

	/* >> tpp_errno (TPPCALL *th_new_dependency)(tpp_hook_cookie cookie, tpp_keyword *filename_kwd);
	 * Called whenever some file is `#include`-ed for the first time
	 * @param: filename_kwd: Then `tpp_keyword` used to describe the file's name. The actual
	 *                       filename can be queried as `tpp_keyword_getcstr(filename_kwd)`
	 * @return: TPP_EOK:       Success (keep going)
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_new_dependency))(tpp_hook_cookie cookie, tpp_keyword *filename_kwd); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_new_dependency_cookie); /* [?..?] Cookie argument for `th_new_dependency` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

	/* >> tpp_errno (TPPCALL *th_file_pushed)(tpp_hook_cookie cookie);
	 * Called whenever a file was just pushed onto the `#include`-stack. Information
	 * about the just-pushed file can be retrieved by examining `tpp_lexer_getfile(LEXER)`.
	 *
	 * Notes:
	 * - This hook can be used by a frontend to implement stuff like GCC's `--trace-includes`.
	 * - This hook is *NOT* called for `tpp_file_subtext_push()` or `tpp_file_pushdummy()`
	 * @return: TPP_EOK:       Success (keep going)
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_file_pushed))(tpp_hook_cookie cookie); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_file_pushed_cookie); /* [?..?] Cookie argument for `th_file_pushed` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) */

	/* >> void (TPPCALL *th_file_popped)(tpp_hook_cookie cookie);
	 * Called whenever a file is about to be popped off the `#include`-stack.
	 * Information about the file that's about-to-be popped can be retrieved
	 * by examining `tpp_lexer_getfile(LEXER)`.
	 *
	 * Notes:
	 * - When files are popped by `tpp_lexer_manualpopfile_popfile()` within a
	 *   `tpp_lexer_manualpopfile_start()`-region, this hook is called during the
	 *   *commit* phase (i.e.: by `tpp_lexer_manualpopfile_break_commit()`),
	 *   rather than `tpp_lexer_manualpopfile_popfile()` as one might suspect at first.
	 * - This hook is *NOT* called by `tpp_file_subtext_pop()` or `tpp_file_popdummy()` */
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK)
	void (TPPCALL *TPP_INTERNAL(th_file_popped))(tpp_hook_cookie cookie); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_file_popped_cookie); /* [?..?] Cookie argument for `th_file_popped` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK) */

	/* >> tpp_errno (TPPCALL *th_include_encountered)(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind);
	 * Called when a `#include` (or `#include_next`, `#import` or `#embed`)-directive
	 * is encountered, at the point in time when the lexer's current token has already
	 * been populated by `tpp_lexer_yieldraw_at_include_string_blocking()` (and macros
	 * were also already expanded), and the current token is `TPP_TOK_INCPATH_DQUOTE` or
	 * `TPP_TOK_INCPATH_LANGLE`.
	 *
	 * This hook is primarily here for the purpose of implementing GCC's `-dI` switch, but
	 * it could also be used for other purposes, such as intentionally skipping certain
	 * `#include`-directives.
	 *
	 * To gain access to the `#include`-string, you must use `tpp_lexer_decode_include_string_cb()`
	 *
	 * @param: include_kind: The kind of directive that this is (one of `TPP_HOOK_INCLUDE_KIND_*`)
	 * @return: TPP_EOK:       Continue handling like usual
	 * @return: TPP_ENOENT:    Don't attempt to find/open a file. Instead, continue processing
	 *                         the file containing the `#include`-directive as though the file
	 *                         could not be found, and the `TPP_W_NO_SUCH_FILE` error was being
	 *                         suppressed.
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_include_encountered))(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_include_encountered_cookie); /* [?..?] Cookie argument for `th_include_encountered` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */

	/* >> tpp_errno (TPPCALL *th_include_not_found)(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind);
	 * Called when the file specified by a `#include` (or `#include_next`, `#import` or
	 * `#embed`)-directive could not be found. This hook may be used to either suppress
	 * the error (by returning something other than `TPP_ENOENT`), or log the error to
	 * implement something like GCC's `-MG` commandline switch.
	 *
	 * This hook is called just before `TPP_W_NO_SUCH_FILE` would be emitted, with the lexer's
	 * current token still being the `<stdio.h>` or `"file.h"` string, meaning if you want
	 * to know what that string says, you can use `tpp_lexer_decode_include_string_cb()` to decode it.
	 *
	 * @param: include_kind: The kind of directive that this is (one of `TPP_HOOK_INCLUDE_KIND_*`)
	 * @return: TPP_EOK:       Suppress the accompanying `TPP_W_NO_SUCH_FILE` error, but continue acting like
	 *                         the file could not be found (*DONT* use this hook to manually push a file or
	 *                         something like that)
	 * @return: TPP_ENOENT:    Emit the `TPP_W_NO_SUCH_FILE` error
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_include_not_found))(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_include_not_found_cookie); /* [?..?] Cookie argument for `th_include_not_found` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */

	/* >> tpp_errno (TPPCALL *th_macro_defined)(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro);
	 * Called whenever a `#define` directive has just been fully
	 * parsed (macro was has not yet been registered with keyword).
	 *
	 * - This hook is *ONLY* invoked when `#define` is encountered, or
	 *   `#pragma pop_macro("foo")` was used to restore a macro's previous
	 *   definition.
	 * - Calls to `tpp_lexer_define()` or other related functions will
	 *   *NOT* invoke this hook.
	 *
	 * @return: TPP_EOK:       Success (keep going)
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_macro_defined))(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_macro_defined_cookie); /* [?..?] Cookie argument for `th_macro_defined` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK) */

	/* >> tpp_errno (TPPCALL *th_macro_undefined)(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name);
	 * Called whenever a `#undef` directive has just been fully
	 * parsed (macro hasn't been deleted from keyword, yet). Note
	 * that this hook is still called, even if the keyword doesn't
	 * have a macro (and might have even already been marked as having
	 * no predefined definition: `_TPP_KEYWORD_MACRO_UNDEFINED`). This
	 * hook is imply called as part of the process of evaluating `#undef`
	 *
	 * This hook is *ONLY* invoked when `#undef` is encountered.
	 * Calls to `tpp_lexer_undef()`, `tpp_keyword_undef()`, or other
	 * related functions will *NOT* invoke this hook.
	 *
	 * NOTE: this hook *will* actually also be called by `#pragma push_macro(undef, "foo")`
	 *
	 * @return: TPP_EOK:       Success (keep going)
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_macro_undefined))(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_macro_undefined_cookie); /* [?..?] Cookie argument for `th_macro_undefined` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK) */

	/* >> tpp_errno (TPPCALL *th_ident_sccs)(tpp_hook_cookie cookie, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len);
	 * Called to handle `#ident` and `#sccs` directives
	 * @param: mode:        Either `TPP_KWD_ident` or `TPP_KWD_sccs`
	 * @param: chunk:       If non-NULL a string that must be `tpp_string_incref()`d
	 *                      if you want to keep `comment_str` alive. If NULL, then the
	 *                      given `comment_str` is statically allocated and doesn't need
	 *                      any chunk to stay alive
	 * @param: comment_str: The source comment that should be inserted
	 * @param: comment_len: Length of `comment_str` in bytes
	 * @return: TPP_EOK:       Success (keep going)
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_ident_sccs))(tpp_hook_cookie cookie, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_ident_sccs_cookie); /* [?..?] Cookie argument for `th_ident_sccs` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK) */

	/* >> tpp_errno (TPPCALL *th_system_include_path)(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg);
	 * Extra callback invoked by `tpp_lexer_foreach_include_path()` at different
	 * points during the process of enumerating include paths. This callback is
	 * then allowed to enumerate some additional include paths that may exist, but
	 * for one reason or another (mainly: speed) aren't known to TPP via its system
	 * include path APIs (`tpp_lexer_includes_add*`)
	 * @param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`, describing the
	 *               caller's position in `tpp_lexer_foreach_include_path()`.
	 * @return: * :            First non-TPP_ENOENT return value of `cb`
	 * @return: TPP_ENOENT:    File still not found
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_system_include_path))(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_system_include_path_cookie); /* [?..?] Cookie argument for `th_system_include_path` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */

	/* >> tpp_errno (TPPCALL *th_system_embed_path)(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg);
	 * Extra callback invoked by `tpp_lexer_foreach_embed_path()` at different points
	 * during the process of enumerating embed paths. (s.a. `TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK`)
	 * @param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`, describing the
	 *               caller's position in `tpp_lexer_foreach_embed_path()`.
	 * @return: * :            First non-TPP_ENOENT return value of `cb`
	 * @return: TPP_ENOENT:    File still not found
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_system_embed_path))(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_system_embed_path_cookie); /* [?..?] Cookie argument for `th_system_embed_path` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */

	/* >> tpp_ssize (TPPCALL *th_unknown_string_escape)(tpp_hook_cookie cookie, tpp_char const **p_pos, tpp_char const *end, struct tpp_lexer_decodestring_config const *tpp_restrict config);
	 * Called by `tpp_lexer_decodestring()` when an unknown `\`-escape sequence is encountered.
	 * This hook can be used to define additional, user-defined escape sequences, or any other
	 * arbitrary behavior to-be performed when specific escape-sequences are found.
	 * On entry, `*p_pos` points at the first (unrecognized) character after the leading `\`, and
	 * if the hook was able to parse said escape sequence, it should update `*p_pos` to point after
	 * it before returning.
	 * @param: p_pos:  [in]  Pointer to start of unrecognized `\`-escape sequence
	 *                 [out] First character no longer part of `\`-escape sequence (if recognized)
	 *                 [out] Unchanged (if not recognized)
	 * @param: end:    The of containing string sequence
	 * @param: config: Identically-named argument of `tpp_lexer_decodestring()`
	 * @return: * :    Sum of positive return values of `data_printer` and `utf8_printer`
	 * @return: < 0:   First negative return value of `data_printer` or `utf8_printer`
	 * @return: TPP_SSIZE_OFERR(TPP_ENOENT):    Escape sequence still not recognized
	 *                 (please leave `*p_pos` unchanged in this case). The caller will
	 *                 proceed by emitting `TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE`
	 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
	 * @return: TPP_SSIZE_OFERR(TPP_EIO):       Filesystem I/O operation failed
	 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): A lexer error happened
	 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK)
	tpp_ssize (TPPCALL *TPP_INTERNAL(th_unknown_string_escape))(tpp_hook_cookie cookie, tpp_char const **p_pos, tpp_char const *end, struct tpp_lexer_decodestring_config const *tpp_restrict config); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_unknown_string_escape_cookie); /* [?..?] Cookie argument for `th_unknown_string_escape` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */

	/* >> tpp_errno (TPPCALL *th_raise_lexerror)(tpp_hook_cookie cookie);
	 * Called by `tpp_lexer_warnf()` just before it's about to return `TPP_ELEXERROR`
	 * This hook can be used to do additional state changes that may be necessary by the
	 * hosting application in order to handle the resulting `TPP_ELEXERROR`
	 * @return: TPP_EOK:       Have `tpp_lexer_warnf()` return `TPP_ELEXERROR`
	 * @return: TPP_ELEXERROR: Same as `TPP_EOK`
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_raise_lexerror))(tpp_hook_cookie cookie); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_raise_lexerror_cookie); /* [?..?] Cookie argument for `th_raise_lexerror` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK) */

	/* >> tpp_errno (TPPCALL *th_isfloatsuffix)(tpp_hook_cookie cookie, tpp_char const *pos);
	 * Called by `tpp_lexer_yieldraw()` when `TPP_HAVE_SMART_FLOAT_TOKENS` is enabled and
	 * a sequence like `1.f` is encountered where the lexer is unsure if the `f` should be
	 * part of the float-token (in the form of a float-suffix), or if this should actually be
	 * parsed as 3 tokens: `[C_INT:1][DOT:.][f:f]`. For this purpose, this hook is called
	 * with `pos` pointing at the `f` (additional characters thereafter may not be loaded
	 * yet, though can be loaded using `tpp_lexer_readchar()` and `tpp_lexer_readunichar()`)
	 * @return: TPP_EOK:       Pointed-to location actually *does* refer to a float suffix
	 * @return: TPP_ENOENT:    It's not a float suffix
	 * @return: TPP_ENOMEM:    Out of memory
	 * @return: TPP_EIO:       Filesystem I/O operation failed
	 * @return: TPP_ELEXERROR: A lexer error happened
	 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_isfloatsuffix))(tpp_hook_cookie cookie, tpp_char const *pos); /* [0..1] */
	tpp_hook_cookie TPP_INTERNAL(th_isfloatsuffix_cookie); /* [?..?] Cookie argument for `th_isfloatsuffix` */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK) */
} tpp_hooks;
#endif /* TPP_HAVE_HOOKS */

/* Called by `tpp_lexer_warnf()` to print warning messages.
 * Potentially unused if `TPP_HAVE_WARNHANDLER_HOOK` is also overwritten
 * @return: >= 0: Success
 * @return: TPP_SSIZE_OFERR(TPP_EIO):       I/O Error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): Hard lexer error
 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK)
#if TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) \
	tpp_formatprinter_print((self)->TPP_INTERNAL(th_warnprinter), tpp_hooks_getcookie_warnprinter(self, lexer), text, num_bytes)
#else /* TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP */
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) \
	((self)->TPP_INTERNAL(th_warnprinter) ? tpp_formatprinter_print((self)->TPP_INTERNAL(th_warnprinter), tpp_hooks_getcookie_warnprinter(self, lexer), text, num_bytes) : 0)
#endif /* TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_NOOP */
#if TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_WARNPRINTER)
#define _TPP_HOOKS_DEFAULT_WARNPRINTER (&TPP_HOOK_WARNPRINTER)
#elif TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_BUILTIN
#define _TPP_HOOKS_DEFAULT_WARNPRINTER (&_tpp_lexer_builtin_warn_or_mesg_printer)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_WARNPRINTER NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_warnprinter(self, lexer)  (self)->TPP_INTERNAL(th_warnprinter_cookie)
#define tpp_hooks_set_warnprinter(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_warnprinter) = (v), (self)->TPP_INTERNAL(th_warnprinter_cookie) = (lexer))
#define tpp_hooks_set_warnprinter_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_warnprinter) = (v), (self)->TPP_INTERNAL(th_warnprinter_cookie) = (cookie))
#if TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_WARNPRINTER)
#define tpp_hooks_reset_warnprinter(self, lexer) (void)((self)->TPP_INTERNAL(th_warnprinter) = _TPP_HOOKS_DEFAULT_WARNPRINTER, (self)->TPP_INTERNAL(th_warnprinter_cookie) = (lexer))
#define _tpp_hooks_init_warnprinter(self, lexer) , (self)->TPP_INTERNAL(th_warnprinter) = _TPP_HOOKS_DEFAULT_WARNPRINTER, (self)->TPP_INTERNAL(th_warnprinter_cookie) = (lexer)
#else /* TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_WARNPRINTER */
#define tpp_hooks_reset_warnprinter(self, lexer) (void)((self)->TPP_INTERNAL(th_warnprinter) = _TPP_HOOKS_DEFAULT_WARNPRINTER)
#define _tpp_hooks_init_warnprinter(self, lexer) , (self)->TPP_INTERNAL(th_warnprinter) = _TPP_HOOKS_DEFAULT_WARNPRINTER
#endif /* TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_WARNPRINTER */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_warnprinter(self, lexer) (lexer)
#define tpp_hooks_set_warnprinter(self, lexer, v) (void)((self)->TPP_INTERNAL(th_warnprinter) = (v))
#define tpp_hooks_reset_warnprinter(self, lexer)  (void)((self)->TPP_INTERNAL(th_warnprinter) = _TPP_HOOKS_DEFAULT_WARNPRINTER)
#define _tpp_hooks_init_warnprinter(self, lexer)  , (self)->TPP_INTERNAL(th_warnprinter) = _TPP_HOOKS_DEFAULT_WARNPRINTER
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_warnprinter(self) (self)->TPP_INTERNAL(th_warnprinter)
#else /* TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */
#if TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_get_warnprinter(self) (&TPP_HOOK_WARNPRINTER)
#define tpp_hooks_getcookie_warnprinter(self, lexer) (lexer)
#define tpp_hooks_call_warnprinter(self, cookie, text, num_bytes) \
	TPP_HOOK_WARNPRINTER(cookie, text, num_bytes)
#elif TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_CONST_BUILTIN
#define tpp_hooks_get_warnprinter(self) (&_tpp_lexer_builtin_warn_or_mesg_printer)
#define tpp_hooks_getcookie_warnprinter(self, lexer) (lexer)
#define tpp_hooks_call_warnprinter(self, cookie, text, num_bytes) \
	_tpp_lexer_builtin_warn_or_mesg_printer(cookie, text, num_bytes)
#else /*  */
#define tpp_hooks_call_warnprinter(self, cookie, text, num_bytes) 0
#endif /* ... */
#define _tpp_hooks_init_warnprinter(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */

/* Called by `tpp_lexer_warnf()` to handle warning notifications. Can be
 * overwritten to implement custom behavior in regards to handling of warnings.
 * @param: info:       Warning context location
 * @param: invokeinfo: Warning invocation method
 * @param: id:         Warning ID
 * @param: args:       Variable arguments passed to warning
 * @return: TPP_EOK:       Success (warning was emitted)
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_ELEXERROR: Hard lexer error
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK)
#if TPP_HAVE_WARNHANDLER_HOOK != TPP_HOOK_RT_NOOP
#define tpp_hooks_call_warnhandler(self, lexer, info, invokeinfo, id, args) \
	(*(self)->TPP_INTERNAL(th_warnhandler))(tpp_hooks_getcookie_warnhandler(self, lexer), info, invokeinfo, id, args)
#else /* TPP_HAVE_WARNHANDLER_HOOK != TPP_HOOK_RT_NOOP */
#define tpp_hooks_call_warnhandler(self, lexer, info, invokeinfo, id, args) \
	((self)->TPP_INTERNAL(th_warnhandler) ? (*(self)->TPP_INTERNAL(th_warnhandler))(tpp_hooks_getcookie_warnhandler(self, lexer), info, invokeinfo, id, args) : TPP_EOK)
#endif /* TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_RT_NOOP */
#if TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_WARNHANDLER)
#define _TPP_HOOKS_DEFAULT_WARNHANDLER (&TPP_HOOK_WARNHANDLER)
#elif TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_RT_BUILTIN
#define _TPP_HOOKS_DEFAULT_WARNHANDLER (&_tpp_lexer_builtin_warnhandler)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_WARNHANDLER NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_warnhandler(self, lexer)  (self)->TPP_INTERNAL(th_warnhandler_cookie)
#define tpp_hooks_set_warnhandler(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_warnhandler) = (v), (self)->TPP_INTERNAL(th_warnhandler_cookie) = (lexer))
#define tpp_hooks_set_warnhandler_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_warnhandler) = (v), (self)->TPP_INTERNAL(th_warnhandler_cookie) = (cookie))
#define tpp_hooks_reset_warnhandler(self, lexer)      (void)((self)->TPP_INTERNAL(th_warnhandler) = _TPP_HOOKS_DEFAULT_WARNHANDLER, (self)->TPP_INTERNAL(th_warnhandler_cookie) = (lexer))
#define _tpp_hooks_init_warnhandler(self, lexer)      , (self)->TPP_INTERNAL(th_warnhandler) = _TPP_HOOKS_DEFAULT_WARNHANDLER, (self)->TPP_INTERNAL(th_warnhandler_cookie) = (lexer)
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_warnhandler(self, lexer) (lexer)
#define tpp_hooks_set_warnhandler(self, lexer, v) (void)((self)->TPP_INTERNAL(th_warnhandler) = (v))
#define tpp_hooks_reset_warnhandler(self, lexer)  (void)((self)->TPP_INTERNAL(th_warnhandler) = _TPP_HOOKS_DEFAULT_WARNHANDLER)
#define _tpp_hooks_init_warnhandler(self, lexer)  , (self)->TPP_INTERNAL(th_warnhandler) = _TPP_HOOKS_DEFAULT_WARNHANDLER
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_warnhandler(self) (self)->TPP_INTERNAL(th_warnhandler)
#else /* TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK) */
#if TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_warnhandler(self, cookie, info, invokeinfo, id, args) \
	TPP_HOOK_WARNHANDLER(cookie, info, invokeinfo, id, args)
#elif TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_CONST_BUILTIN
#define tpp_hooks_call_warnhandler(self, cookie, info, invokeinfo, id, args) \
	_tpp_lexer_builtin_warnhandler(cookie, info, invokeinfo, id, args)
#else /*  */
#define tpp_hooks_call_warnhandler(self, cookie, info, invokeinfo, id, args) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_warnhandler(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK) */

/* Used by `#pragma message` to print messages (see `TPP_HAVE_PRAGMA_MESSAGE`)
 * @return: >= 0: Success
 * @return: TPP_SSIZE_OFERR(TPP_EIO):       I/O Error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): Hard lexer error
 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK)
#if TPP_HAVE_MESGPRINTER_HOOK != TPP_HOOK_RT_NOOP
#define tpp_hooks_call_mesgprinter(self, lexer, text, num_bytes) \
	tpp_formatprinter_print((self)->TPP_INTERNAL(th_mesgprinter), tpp_hooks_getcookie_mesgprinter(self, lexer), text, num_bytes)
#else /* TPP_HAVE_MESGPRINTER_HOOK != TPP_HOOK_RT_NOOP */
#define tpp_hooks_call_mesgprinter(self, lexer, text, num_bytes) \
	((self)->TPP_INTERNAL(th_mesgprinter) ? tpp_formatprinter_print((self)->TPP_INTERNAL(th_mesgprinter), tpp_hooks_getcookie_mesgprinter(self, lexer), text, num_bytes) : 0)
#endif /* TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_NOOP */
#if TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_MESGPRINTER)
#define _TPP_HOOKS_DEFAULT_MESGPRINTER (&TPP_HOOK_MESGPRINTER)
#elif TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_BUILTIN
#define _TPP_HOOKS_DEFAULT_MESGPRINTER (&_tpp_lexer_builtin_warn_or_mesg_printer)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_MESGPRINTER NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_mesgprinter(self, lexer)  (self)->TPP_INTERNAL(th_mesgprinter_cookie)
#define tpp_hooks_set_mesgprinter(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_mesgprinter) = (v), (self)->TPP_INTERNAL(th_mesgprinter_cookie) = (lexer))
#define tpp_hooks_set_mesgprinter_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_mesgprinter) = (v), (self)->TPP_INTERNAL(th_mesgprinter_cookie) = (cookie))
#if TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_MESGPRINTER)
#define tpp_hooks_reset_mesgprinter(self, lexer) (void)((self)->TPP_INTERNAL(th_mesgprinter) = _TPP_HOOKS_DEFAULT_MESGPRINTER, (self)->TPP_INTERNAL(th_mesgprinter_cookie) = (lexer))
#define _tpp_hooks_init_mesgprinter(self, lexer) , (self)->TPP_INTERNAL(th_mesgprinter) = _TPP_HOOKS_DEFAULT_MESGPRINTER, (self)->TPP_INTERNAL(th_mesgprinter_cookie) = (lexer)
#else /* TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_MESGPRINTER */
#define tpp_hooks_reset_mesgprinter(self, lexer) (void)((self)->TPP_INTERNAL(th_mesgprinter) = _TPP_HOOKS_DEFAULT_MESGPRINTER)
#define _tpp_hooks_init_mesgprinter(self, lexer) , (self)->TPP_INTERNAL(th_mesgprinter) = _TPP_HOOKS_DEFAULT_MESGPRINTER
#endif /* TPP_HAVE_MESGPRINTER_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_MESGPRINTER */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_mesgprinter(self, lexer) (lexer)
#define tpp_hooks_set_mesgprinter(self, lexer, v) (void)((self)->TPP_INTERNAL(th_mesgprinter) = (v))
#define tpp_hooks_reset_mesgprinter(self, lexer)  (void)((self)->TPP_INTERNAL(th_mesgprinter) = _TPP_HOOKS_DEFAULT_MESGPRINTER)
#define _tpp_hooks_init_mesgprinter(self, lexer)  , (self)->TPP_INTERNAL(th_mesgprinter) = _TPP_HOOKS_DEFAULT_MESGPRINTER
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_mesgprinter(self) (self)->TPP_INTERNAL(th_mesgprinter)
#else /* TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK) */
#if TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_get_mesgprinter(self) (&TPP_HOOK_MESGPRINTER)
#define tpp_hooks_getcookie_mesgprinter(self, lexer) (lexer)
#define tpp_hooks_call_mesgprinter(self, cookie, text, num_bytes) \
	TPP_HOOK_MESGPRINTER(cookie, text, num_bytes)
#elif TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_CONST_BUILTIN
#define tpp_hooks_get_mesgprinter(self) (&_tpp_lexer_builtin_warn_or_mesg_printer)
#define tpp_hooks_getcookie_mesgprinter(self, lexer) (lexer)
#define tpp_hooks_call_mesgprinter(self, cookie, text, num_bytes) \
	_tpp_lexer_builtin_warn_or_mesg_printer(cookie, text, num_bytes)
#else /*  */
#define tpp_hooks_call_mesgprinter(self, cookie, text, num_bytes) 0
#endif /* ... */
#define _tpp_hooks_init_mesgprinter(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK) */

/* User-defined callback for parsing `#if`-style expressions
 * - This callback is invoked in a context where the lexer
 *   points before the expression's first token (meaning that
 *   this callback is responsible to do the initial yield
 *   using whatever method it wants to use).
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
 * @return: TPP_EUSER(*):    User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK)
#if TPP_HAVE_PARSEEXPR_HOOK != TPP_HOOK_RT_NOOP
#define tpp_hooks_call_parseexpr(self, lexer, result) \
	(*(self)->TPP_INTERNAL(th_parseexpr))(tpp_hooks_getcookie_parseexpr(self, lexer), result)
#else /* TPP_HAVE_PARSEEXPR_HOOK != TPP_HOOK_RT_NOOP */
#define tpp_hooks_call_parseexpr(self, lexer, result) \
	((self)->TPP_INTERNAL(th_parseexpr) ? (*(self)->TPP_INTERNAL(th_parseexpr))(tpp_hooks_getcookie_parseexpr(self, lexer), result) : tpp_expr_value_init_zero(result))
#endif /* TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_RT_NOOP */
#if TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_PARSEEXPR)
#define _TPP_HOOKS_DEFAULT_PARSEEXPR (&TPP_HOOK_PARSEEXPR)
#elif TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_RT_BUILTIN
#define _TPP_HOOKS_DEFAULT_PARSEEXPR (&_tpp_lexer_builtin_parseexpr)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_PARSEEXPR NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_parseexpr(self, lexer)  (self)->TPP_INTERNAL(th_parseexpr_cookie)
#define tpp_hooks_set_parseexpr(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_parseexpr) = (v), (self)->TPP_INTERNAL(th_parseexpr_cookie) = (lexer))
#define tpp_hooks_set_parseexpr_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_parseexpr) = (v), (self)->TPP_INTERNAL(th_parseexpr_cookie) = (cookie))
#define tpp_hooks_reset_parseexpr(self, lexer)      (void)((self)->TPP_INTERNAL(th_parseexpr) = _TPP_HOOKS_DEFAULT_PARSEEXPR, (self)->TPP_INTERNAL(th_parseexpr_cookie) = (lexer))
#define _tpp_hooks_init_parseexpr(self, lexer)      , (self)->TPP_INTERNAL(th_parseexpr) = _TPP_HOOKS_DEFAULT_PARSEEXPR, (self)->TPP_INTERNAL(th_parseexpr_cookie) = (lexer)
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_parseexpr(self, lexer) (lexer)
#define tpp_hooks_set_parseexpr(self, lexer, v) (void)((self)->TPP_INTERNAL(th_parseexpr) = (v))
#define tpp_hooks_reset_parseexpr(self, lexer)  (void)((self)->TPP_INTERNAL(th_parseexpr) = _TPP_HOOKS_DEFAULT_PARSEEXPR)
#define _tpp_hooks_init_parseexpr(self, lexer)  , (self)->TPP_INTERNAL(th_parseexpr) = _TPP_HOOKS_DEFAULT_PARSEEXPR
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_parseexpr(self) (self)->TPP_INTERNAL(th_parseexpr)
#else /* TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) */
#if TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_parseexpr(self, cookie, result) \
	TPP_HOOK_PARSEEXPR(cookie, result)
#elif TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_CONST_BUILTIN
#define tpp_hooks_call_parseexpr(self, cookie, result) \
	_tpp_lexer_builtin_parseexpr(cookie, result)
#else /*  */
#define tpp_hooks_call_parseexpr(self, cookie, result) tpp_expr_value_init_zero(result)
#endif /* ... */
#define _tpp_hooks_init_parseexpr(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) */

/* Called whenever a `#pragma` is encountered that is not recognized.
 * When called, the lexer is set-up to point at the first token after the `#pragma`.
 * @return: TPP_EOK:      Pragma has been handled
 * @return: TPP_ENOENT:   Pragma is still unknown, and a warning should be emitted
 * @return: TPP_EIO:      I/O error
 * @return: TPP_ENOMEM:   Out of memory
 * @return: TPP_EUSER(*): User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
#define tpp_hooks_call_unknown_pragma(self, lexer) \
	((self)->TPP_INTERNAL(th_unknown_pragma) ? (*(self)->TPP_INTERNAL(th_unknown_pragma))(tpp_hooks_getcookie_unknown_pragma(self, lexer)) : TPP_ENOENT)
#if TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_UNKNOWN_PRAGMA)
#define _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA (&TPP_HOOK_UNKNOWN_PRAGMA)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_unknown_pragma(self, lexer)  (self)->TPP_INTERNAL(th_unknown_pragma_cookie)
#define tpp_hooks_set_unknown_pragma(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_unknown_pragma) = (v), (self)->TPP_INTERNAL(th_unknown_pragma_cookie) = (lexer))
#define tpp_hooks_set_unknown_pragma_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_unknown_pragma) = (v), (self)->TPP_INTERNAL(th_unknown_pragma_cookie) = (cookie))
#if TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_UNKNOWN_PRAGMA)
#define tpp_hooks_reset_unknown_pragma(self, lexer) (void)((self)->TPP_INTERNAL(th_unknown_pragma) = _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA, (self)->TPP_INTERNAL(th_unknown_pragma_cookie) = (lexer))
#define _tpp_hooks_init_unknown_pragma(self, lexer) , (self)->TPP_INTERNAL(th_unknown_pragma) = _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA, (self)->TPP_INTERNAL(th_unknown_pragma_cookie) = (lexer)
#else /* TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_UNKNOWN_PRAGMA */
#define tpp_hooks_reset_unknown_pragma(self, lexer) (void)((self)->TPP_INTERNAL(th_unknown_pragma) = _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA)
#define _tpp_hooks_init_unknown_pragma(self, lexer) , (self)->TPP_INTERNAL(th_unknown_pragma) = _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA
#endif /* TPP_HAVE_UNKNOWN_PRAGMA_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_UNKNOWN_PRAGMA */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_unknown_pragma(self, lexer) (lexer)
#define tpp_hooks_set_unknown_pragma(self, lexer, v) (void)((self)->TPP_INTERNAL(th_unknown_pragma) = (v))
#define tpp_hooks_reset_unknown_pragma(self, lexer)  (void)((self)->TPP_INTERNAL(th_unknown_pragma) = _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA)
#define _tpp_hooks_init_unknown_pragma(self, lexer)  , (self)->TPP_INTERNAL(th_unknown_pragma) = _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_unknown_pragma(self) (self)->TPP_INTERNAL(th_unknown_pragma)
#else /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */
#if TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_unknown_pragma(self, cookie) \
	TPP_HOOK_UNKNOWN_PRAGMA(cookie)
#else /*  */
#define tpp_hooks_call_unknown_pragma(self, cookie) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_unknown_pragma(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */

/* Called whenever some file is `#include`-ed for the first time
 * @param: filename_kwd: Then `tpp_keyword` used to describe the file's name. The actual
 *                       filename can be queried as `tpp_keyword_getcstr(filename_kwd)`
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
#define tpp_hooks_call_new_dependency(self, lexer, filename_kwd) \
	((self)->TPP_INTERNAL(th_new_dependency) ? (*(self)->TPP_INTERNAL(th_new_dependency))(tpp_hooks_getcookie_new_dependency(self, lexer), filename_kwd) : TPP_EOK)
#if TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_NEW_DEPENDENCY)
#define _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY (&TPP_HOOK_NEW_DEPENDENCY)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_new_dependency(self, lexer)  (self)->TPP_INTERNAL(th_new_dependency_cookie)
#define tpp_hooks_set_new_dependency(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_new_dependency) = (v), (self)->TPP_INTERNAL(th_new_dependency_cookie) = (lexer))
#define tpp_hooks_set_new_dependency_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_new_dependency) = (v), (self)->TPP_INTERNAL(th_new_dependency_cookie) = (cookie))
#if TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_NEW_DEPENDENCY)
#define tpp_hooks_reset_new_dependency(self, lexer) (void)((self)->TPP_INTERNAL(th_new_dependency) = _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY, (self)->TPP_INTERNAL(th_new_dependency_cookie) = (lexer))
#define _tpp_hooks_init_new_dependency(self, lexer) , (self)->TPP_INTERNAL(th_new_dependency) = _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY, (self)->TPP_INTERNAL(th_new_dependency_cookie) = (lexer)
#else /* TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_NEW_DEPENDENCY */
#define tpp_hooks_reset_new_dependency(self, lexer) (void)((self)->TPP_INTERNAL(th_new_dependency) = _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY)
#define _tpp_hooks_init_new_dependency(self, lexer) , (self)->TPP_INTERNAL(th_new_dependency) = _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY
#endif /* TPP_HAVE_NEW_DEPENDENCY_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_NEW_DEPENDENCY */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_new_dependency(self, lexer) (lexer)
#define tpp_hooks_set_new_dependency(self, lexer, v) (void)((self)->TPP_INTERNAL(th_new_dependency) = (v))
#define tpp_hooks_reset_new_dependency(self, lexer)  (void)((self)->TPP_INTERNAL(th_new_dependency) = _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY)
#define _tpp_hooks_init_new_dependency(self, lexer)  , (self)->TPP_INTERNAL(th_new_dependency) = _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_new_dependency(self) (self)->TPP_INTERNAL(th_new_dependency)
#else /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
#if TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_new_dependency(self, cookie, filename_kwd) \
	TPP_HOOK_NEW_DEPENDENCY(cookie, filename_kwd)
#else /*  */
#define tpp_hooks_call_new_dependency(self, cookie, filename_kwd) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_new_dependency(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

/* Called whenever a file was just pushed onto the `#include`-stack. Information
 * about the just-pushed file can be retrieved by examining `tpp_lexer_getfile(LEXER)`.
 *
 * Notes:
 * - This hook can be used by a frontend to implement stuff like GCC's `--trace-includes`.
 * - This hook is *NOT* called for `tpp_file_subtext_push()` or `tpp_file_pushdummy()`
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK)
#define tpp_hooks_call_file_pushed(self, lexer) \
	((self)->TPP_INTERNAL(th_file_pushed) ? (*(self)->TPP_INTERNAL(th_file_pushed))(tpp_hooks_getcookie_file_pushed(self, lexer)) : TPP_EOK)
#if TPP_HAVE_FILE_PUSHED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_FILE_PUSHED)
#define _TPP_HOOKS_DEFAULT_FILE_PUSHED (&TPP_HOOK_FILE_PUSHED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_FILE_PUSHED NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_file_pushed(self, lexer)  (self)->TPP_INTERNAL(th_file_pushed_cookie)
#define tpp_hooks_set_file_pushed(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_file_pushed) = (v), (self)->TPP_INTERNAL(th_file_pushed_cookie) = (lexer))
#define tpp_hooks_set_file_pushed_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_file_pushed) = (v), (self)->TPP_INTERNAL(th_file_pushed_cookie) = (cookie))
#if TPP_HAVE_FILE_PUSHED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_FILE_PUSHED)
#define tpp_hooks_reset_file_pushed(self, lexer) (void)((self)->TPP_INTERNAL(th_file_pushed) = _TPP_HOOKS_DEFAULT_FILE_PUSHED, (self)->TPP_INTERNAL(th_file_pushed_cookie) = (lexer))
#define _tpp_hooks_init_file_pushed(self, lexer) , (self)->TPP_INTERNAL(th_file_pushed) = _TPP_HOOKS_DEFAULT_FILE_PUSHED, (self)->TPP_INTERNAL(th_file_pushed_cookie) = (lexer)
#else /* TPP_HAVE_FILE_PUSHED_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_FILE_PUSHED */
#define tpp_hooks_reset_file_pushed(self, lexer) (void)((self)->TPP_INTERNAL(th_file_pushed) = _TPP_HOOKS_DEFAULT_FILE_PUSHED)
#define _tpp_hooks_init_file_pushed(self, lexer) , (self)->TPP_INTERNAL(th_file_pushed) = _TPP_HOOKS_DEFAULT_FILE_PUSHED
#endif /* TPP_HAVE_FILE_PUSHED_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_FILE_PUSHED */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_file_pushed(self, lexer) (lexer)
#define tpp_hooks_set_file_pushed(self, lexer, v) (void)((self)->TPP_INTERNAL(th_file_pushed) = (v))
#define tpp_hooks_reset_file_pushed(self, lexer)  (void)((self)->TPP_INTERNAL(th_file_pushed) = _TPP_HOOKS_DEFAULT_FILE_PUSHED)
#define _tpp_hooks_init_file_pushed(self, lexer)  , (self)->TPP_INTERNAL(th_file_pushed) = _TPP_HOOKS_DEFAULT_FILE_PUSHED
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_file_pushed(self) (self)->TPP_INTERNAL(th_file_pushed)
#else /* TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) */
#if TPP_HAVE_FILE_PUSHED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_file_pushed(self, cookie) \
	TPP_HOOK_FILE_PUSHED(cookie)
#else /*  */
#define tpp_hooks_call_file_pushed(self, cookie) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_file_pushed(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) */

/* Called whenever a file is about to be popped off the `#include`-stack.
 * Information about the file that's about-to-be popped can be retrieved
 * by examining `tpp_lexer_getfile(LEXER)`.
 *
 * Notes:
 * - When files are popped by `tpp_lexer_manualpopfile_popfile()` within a
 *   `tpp_lexer_manualpopfile_start()`-region, this hook is called during the
 *   *commit* phase (i.e.: by `tpp_lexer_manualpopfile_break_commit()`),
 *   rather than `tpp_lexer_manualpopfile_popfile()` as one might suspect at first.
 * - This hook is *NOT* called by `tpp_file_subtext_pop()` or `tpp_file_popdummy()` */
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK)
#define tpp_hooks_call_file_popped(self, lexer) \
	((self)->TPP_INTERNAL(th_file_popped) ? (*(self)->TPP_INTERNAL(th_file_popped))(tpp_hooks_getcookie_file_popped(self, lexer)) : (void)0)
#if TPP_HAVE_FILE_POPPED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_FILE_POPPED)
#define _TPP_HOOKS_DEFAULT_FILE_POPPED (&TPP_HOOK_FILE_POPPED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_FILE_POPPED NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_file_popped(self, lexer)  (self)->TPP_INTERNAL(th_file_popped_cookie)
#define tpp_hooks_set_file_popped(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_file_popped) = (v), (self)->TPP_INTERNAL(th_file_popped_cookie) = (lexer))
#define tpp_hooks_set_file_popped_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_file_popped) = (v), (self)->TPP_INTERNAL(th_file_popped_cookie) = (cookie))
#if TPP_HAVE_FILE_POPPED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_FILE_POPPED)
#define tpp_hooks_reset_file_popped(self, lexer) (void)((self)->TPP_INTERNAL(th_file_popped) = _TPP_HOOKS_DEFAULT_FILE_POPPED, (self)->TPP_INTERNAL(th_file_popped_cookie) = (lexer))
#define _tpp_hooks_init_file_popped(self, lexer) , (self)->TPP_INTERNAL(th_file_popped) = _TPP_HOOKS_DEFAULT_FILE_POPPED, (self)->TPP_INTERNAL(th_file_popped_cookie) = (lexer)
#else /* TPP_HAVE_FILE_POPPED_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_FILE_POPPED */
#define tpp_hooks_reset_file_popped(self, lexer) (void)((self)->TPP_INTERNAL(th_file_popped) = _TPP_HOOKS_DEFAULT_FILE_POPPED)
#define _tpp_hooks_init_file_popped(self, lexer) , (self)->TPP_INTERNAL(th_file_popped) = _TPP_HOOKS_DEFAULT_FILE_POPPED
#endif /* TPP_HAVE_FILE_POPPED_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_FILE_POPPED */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_file_popped(self, lexer) (lexer)
#define tpp_hooks_set_file_popped(self, lexer, v) (void)((self)->TPP_INTERNAL(th_file_popped) = (v))
#define tpp_hooks_reset_file_popped(self, lexer)  (void)((self)->TPP_INTERNAL(th_file_popped) = _TPP_HOOKS_DEFAULT_FILE_POPPED)
#define _tpp_hooks_init_file_popped(self, lexer)  , (self)->TPP_INTERNAL(th_file_popped) = _TPP_HOOKS_DEFAULT_FILE_POPPED
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_file_popped(self) (self)->TPP_INTERNAL(th_file_popped)
#else /* TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK) */
#if TPP_HAVE_FILE_POPPED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_file_popped(self, cookie) \
	TPP_HOOK_FILE_POPPED(cookie)
#else /*  */
#define tpp_hooks_call_file_popped(self, cookie) (void)0
#endif /* ... */
#define _tpp_hooks_init_file_popped(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK) */

/* Called when a `#include` (or `#include_next`, `#import` or `#embed`)-directive
 * is encountered, at the point in time when the lexer's current token has already
 * been populated by `tpp_lexer_yieldraw_at_include_string_blocking()` (and macros
 * were also already expanded), and the current token is `TPP_TOK_INCPATH_DQUOTE` or
 * `TPP_TOK_INCPATH_LANGLE`.
 *
 * This hook is primarily here for the purpose of implementing GCC's `-dI` switch, but
 * it could also be used for other purposes, such as intentionally skipping certain
 * `#include`-directives.
 *
 * To gain access to the `#include`-string, you must use `tpp_lexer_decode_include_string_cb()`
 *
 * @param: include_kind: The kind of directive that this is (one of `TPP_HOOK_INCLUDE_KIND_*`)
 * @return: TPP_EOK:       Continue handling like usual
 * @return: TPP_ENOENT:    Don't attempt to find/open a file. Instead, continue processing
 *                         the file containing the `#include`-directive as though the file
 *                         could not be found, and the `TPP_W_NO_SUCH_FILE` error was being
 *                         suppressed.
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK)
#define tpp_hooks_call_include_encountered(self, lexer, include_kind) \
	((self)->TPP_INTERNAL(th_include_encountered) ? (*(self)->TPP_INTERNAL(th_include_encountered))(tpp_hooks_getcookie_include_encountered(self, lexer), include_kind) : TPP_EOK)
#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_INCLUDE_ENCOUNTERED)
#define _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED (&TPP_HOOK_INCLUDE_ENCOUNTERED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_include_encountered(self, lexer)  (self)->TPP_INTERNAL(th_include_encountered_cookie)
#define tpp_hooks_set_include_encountered(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_include_encountered) = (v), (self)->TPP_INTERNAL(th_include_encountered_cookie) = (lexer))
#define tpp_hooks_set_include_encountered_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_include_encountered) = (v), (self)->TPP_INTERNAL(th_include_encountered_cookie) = (cookie))
#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_INCLUDE_ENCOUNTERED)
#define tpp_hooks_reset_include_encountered(self, lexer) (void)((self)->TPP_INTERNAL(th_include_encountered) = _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED, (self)->TPP_INTERNAL(th_include_encountered_cookie) = (lexer))
#define _tpp_hooks_init_include_encountered(self, lexer) , (self)->TPP_INTERNAL(th_include_encountered) = _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED, (self)->TPP_INTERNAL(th_include_encountered_cookie) = (lexer)
#else /* TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_INCLUDE_ENCOUNTERED */
#define tpp_hooks_reset_include_encountered(self, lexer) (void)((self)->TPP_INTERNAL(th_include_encountered) = _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED)
#define _tpp_hooks_init_include_encountered(self, lexer) , (self)->TPP_INTERNAL(th_include_encountered) = _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED
#endif /* TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_INCLUDE_ENCOUNTERED */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_include_encountered(self, lexer) (lexer)
#define tpp_hooks_set_include_encountered(self, lexer, v) (void)((self)->TPP_INTERNAL(th_include_encountered) = (v))
#define tpp_hooks_reset_include_encountered(self, lexer)  (void)((self)->TPP_INTERNAL(th_include_encountered) = _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED)
#define _tpp_hooks_init_include_encountered(self, lexer)  , (self)->TPP_INTERNAL(th_include_encountered) = _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_include_encountered(self) (self)->TPP_INTERNAL(th_include_encountered)
#else /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */
#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_include_encountered(self, cookie, include_kind) \
	TPP_HOOK_INCLUDE_ENCOUNTERED(cookie, include_kind)
#else /*  */
#define tpp_hooks_call_include_encountered(self, cookie, include_kind) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_include_encountered(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */

/* Called when the file specified by a `#include` (or `#include_next`, `#import` or
 * `#embed`)-directive could not be found. This hook may be used to either suppress
 * the error (by returning something other than `TPP_ENOENT`), or log the error to
 * implement something like GCC's `-MG` commandline switch.
 *
 * This hook is called just before `TPP_W_NO_SUCH_FILE` would be emitted, with the lexer's
 * current token still being the `<stdio.h>` or `"file.h"` string, meaning if you want
 * to know what that string says, you can use `tpp_lexer_decode_include_string_cb()` to decode it.
 *
 * @param: include_kind: The kind of directive that this is (one of `TPP_HOOK_INCLUDE_KIND_*`)
 * @return: TPP_EOK:       Suppress the accompanying `TPP_W_NO_SUCH_FILE` error, but continue acting like
 *                         the file could not be found (*DONT* use this hook to manually push a file or
 *                         something like that)
 * @return: TPP_ENOENT:    Emit the `TPP_W_NO_SUCH_FILE` error
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
#define tpp_hooks_call_include_not_found(self, lexer, include_kind) \
	((self)->TPP_INTERNAL(th_include_not_found) ? (*(self)->TPP_INTERNAL(th_include_not_found))(tpp_hooks_getcookie_include_not_found(self, lexer), include_kind) : TPP_ENOENT)
#if TPP_HAVE_INCLUDE_NOT_FOUND_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_INCLUDE_NOT_FOUND)
#define _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND (&TPP_HOOK_INCLUDE_NOT_FOUND)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_include_not_found(self, lexer)  (self)->TPP_INTERNAL(th_include_not_found_cookie)
#define tpp_hooks_set_include_not_found(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_include_not_found) = (v), (self)->TPP_INTERNAL(th_include_not_found_cookie) = (lexer))
#define tpp_hooks_set_include_not_found_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_include_not_found) = (v), (self)->TPP_INTERNAL(th_include_not_found_cookie) = (cookie))
#if TPP_HAVE_INCLUDE_NOT_FOUND_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_INCLUDE_NOT_FOUND)
#define tpp_hooks_reset_include_not_found(self, lexer) (void)((self)->TPP_INTERNAL(th_include_not_found) = _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND, (self)->TPP_INTERNAL(th_include_not_found_cookie) = (lexer))
#define _tpp_hooks_init_include_not_found(self, lexer) , (self)->TPP_INTERNAL(th_include_not_found) = _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND, (self)->TPP_INTERNAL(th_include_not_found_cookie) = (lexer)
#else /* TPP_HAVE_INCLUDE_NOT_FOUND_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_INCLUDE_NOT_FOUND */
#define tpp_hooks_reset_include_not_found(self, lexer) (void)((self)->TPP_INTERNAL(th_include_not_found) = _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND)
#define _tpp_hooks_init_include_not_found(self, lexer) , (self)->TPP_INTERNAL(th_include_not_found) = _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND
#endif /* TPP_HAVE_INCLUDE_NOT_FOUND_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_INCLUDE_NOT_FOUND */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_include_not_found(self, lexer) (lexer)
#define tpp_hooks_set_include_not_found(self, lexer, v) (void)((self)->TPP_INTERNAL(th_include_not_found) = (v))
#define tpp_hooks_reset_include_not_found(self, lexer)  (void)((self)->TPP_INTERNAL(th_include_not_found) = _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND)
#define _tpp_hooks_init_include_not_found(self, lexer)  , (self)->TPP_INTERNAL(th_include_not_found) = _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_include_not_found(self) (self)->TPP_INTERNAL(th_include_not_found)
#else /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */
#if TPP_HAVE_INCLUDE_NOT_FOUND_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_include_not_found(self, cookie, include_kind) \
	TPP_HOOK_INCLUDE_NOT_FOUND(cookie, include_kind)
#else /*  */
#define tpp_hooks_call_include_not_found(self, cookie, include_kind) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_include_not_found(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */

/* Called whenever a `#define` directive has just been fully
 * parsed (macro was has not yet been registered with keyword).
 *
 * - This hook is *ONLY* invoked when `#define` is encountered, or
 *   `#pragma pop_macro("foo")` was used to restore a macro's previous
 *   definition.
 * - Calls to `tpp_lexer_define()` or other related functions will
 *   *NOT* invoke this hook.
 *
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK)
#define tpp_hooks_call_macro_defined(self, lexer, name, macro) \
	((self)->TPP_INTERNAL(th_macro_defined) ? (*(self)->TPP_INTERNAL(th_macro_defined))(tpp_hooks_getcookie_macro_defined(self, lexer), name, macro) : TPP_EOK)
#if TPP_HAVE_MACRO_DEFINED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_MACRO_DEFINED)
#define _TPP_HOOKS_DEFAULT_MACRO_DEFINED (&TPP_HOOK_MACRO_DEFINED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_MACRO_DEFINED NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_macro_defined(self, lexer)  (self)->TPP_INTERNAL(th_macro_defined_cookie)
#define tpp_hooks_set_macro_defined(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_macro_defined) = (v), (self)->TPP_INTERNAL(th_macro_defined_cookie) = (lexer))
#define tpp_hooks_set_macro_defined_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_macro_defined) = (v), (self)->TPP_INTERNAL(th_macro_defined_cookie) = (cookie))
#if TPP_HAVE_MACRO_DEFINED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_MACRO_DEFINED)
#define tpp_hooks_reset_macro_defined(self, lexer) (void)((self)->TPP_INTERNAL(th_macro_defined) = _TPP_HOOKS_DEFAULT_MACRO_DEFINED, (self)->TPP_INTERNAL(th_macro_defined_cookie) = (lexer))
#define _tpp_hooks_init_macro_defined(self, lexer) , (self)->TPP_INTERNAL(th_macro_defined) = _TPP_HOOKS_DEFAULT_MACRO_DEFINED, (self)->TPP_INTERNAL(th_macro_defined_cookie) = (lexer)
#else /* TPP_HAVE_MACRO_DEFINED_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_MACRO_DEFINED */
#define tpp_hooks_reset_macro_defined(self, lexer) (void)((self)->TPP_INTERNAL(th_macro_defined) = _TPP_HOOKS_DEFAULT_MACRO_DEFINED)
#define _tpp_hooks_init_macro_defined(self, lexer) , (self)->TPP_INTERNAL(th_macro_defined) = _TPP_HOOKS_DEFAULT_MACRO_DEFINED
#endif /* TPP_HAVE_MACRO_DEFINED_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_MACRO_DEFINED */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_macro_defined(self, lexer) (lexer)
#define tpp_hooks_set_macro_defined(self, lexer, v) (void)((self)->TPP_INTERNAL(th_macro_defined) = (v))
#define tpp_hooks_reset_macro_defined(self, lexer)  (void)((self)->TPP_INTERNAL(th_macro_defined) = _TPP_HOOKS_DEFAULT_MACRO_DEFINED)
#define _tpp_hooks_init_macro_defined(self, lexer)  , (self)->TPP_INTERNAL(th_macro_defined) = _TPP_HOOKS_DEFAULT_MACRO_DEFINED
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_macro_defined(self) (self)->TPP_INTERNAL(th_macro_defined)
#else /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK) */
#if TPP_HAVE_MACRO_DEFINED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_macro_defined(self, cookie, name, macro) \
	TPP_HOOK_MACRO_DEFINED(cookie, name, macro)
#else /*  */
#define tpp_hooks_call_macro_defined(self, cookie, name, macro) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_macro_defined(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK) */

/* Called whenever a `#undef` directive has just been fully
 * parsed (macro hasn't been deleted from keyword, yet). Note
 * that this hook is still called, even if the keyword doesn't
 * have a macro (and might have even already been marked as having
 * no predefined definition: `_TPP_KEYWORD_MACRO_UNDEFINED`). This
 * hook is imply called as part of the process of evaluating `#undef`
 *
 * This hook is *ONLY* invoked when `#undef` is encountered.
 * Calls to `tpp_lexer_undef()`, `tpp_keyword_undef()`, or other
 * related functions will *NOT* invoke this hook.
 *
 * NOTE: this hook *will* actually also be called by `#pragma push_macro(undef, "foo")`
 *
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK)
#define tpp_hooks_call_macro_undefined(self, lexer, name) \
	((self)->TPP_INTERNAL(th_macro_undefined) ? (*(self)->TPP_INTERNAL(th_macro_undefined))(tpp_hooks_getcookie_macro_undefined(self, lexer), name) : TPP_EOK)
#if TPP_HAVE_MACRO_UNDEFINED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_MACRO_UNDEFINED)
#define _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED (&TPP_HOOK_MACRO_UNDEFINED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_macro_undefined(self, lexer)  (self)->TPP_INTERNAL(th_macro_undefined_cookie)
#define tpp_hooks_set_macro_undefined(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_macro_undefined) = (v), (self)->TPP_INTERNAL(th_macro_undefined_cookie) = (lexer))
#define tpp_hooks_set_macro_undefined_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_macro_undefined) = (v), (self)->TPP_INTERNAL(th_macro_undefined_cookie) = (cookie))
#if TPP_HAVE_MACRO_UNDEFINED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_MACRO_UNDEFINED)
#define tpp_hooks_reset_macro_undefined(self, lexer) (void)((self)->TPP_INTERNAL(th_macro_undefined) = _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED, (self)->TPP_INTERNAL(th_macro_undefined_cookie) = (lexer))
#define _tpp_hooks_init_macro_undefined(self, lexer) , (self)->TPP_INTERNAL(th_macro_undefined) = _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED, (self)->TPP_INTERNAL(th_macro_undefined_cookie) = (lexer)
#else /* TPP_HAVE_MACRO_UNDEFINED_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_MACRO_UNDEFINED */
#define tpp_hooks_reset_macro_undefined(self, lexer) (void)((self)->TPP_INTERNAL(th_macro_undefined) = _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED)
#define _tpp_hooks_init_macro_undefined(self, lexer) , (self)->TPP_INTERNAL(th_macro_undefined) = _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED
#endif /* TPP_HAVE_MACRO_UNDEFINED_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_MACRO_UNDEFINED */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_macro_undefined(self, lexer) (lexer)
#define tpp_hooks_set_macro_undefined(self, lexer, v) (void)((self)->TPP_INTERNAL(th_macro_undefined) = (v))
#define tpp_hooks_reset_macro_undefined(self, lexer)  (void)((self)->TPP_INTERNAL(th_macro_undefined) = _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED)
#define _tpp_hooks_init_macro_undefined(self, lexer)  , (self)->TPP_INTERNAL(th_macro_undefined) = _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_macro_undefined(self) (self)->TPP_INTERNAL(th_macro_undefined)
#else /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK) */
#if TPP_HAVE_MACRO_UNDEFINED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_macro_undefined(self, cookie, name) \
	TPP_HOOK_MACRO_UNDEFINED(cookie, name)
#else /*  */
#define tpp_hooks_call_macro_undefined(self, cookie, name) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_macro_undefined(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK) */

/* Called to handle `#ident` and `#sccs` directives
 * @param: mode:        Either `TPP_KWD_ident` or `TPP_KWD_sccs`
 * @param: chunk:       If non-NULL a string that must be `tpp_string_incref()`d
 *                      if you want to keep `comment_str` alive. If NULL, then the
 *                      given `comment_str` is statically allocated and doesn't need
 *                      any chunk to stay alive
 * @param: comment_str: The source comment that should be inserted
 * @param: comment_len: Length of `comment_str` in bytes
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK)
#define tpp_hooks_call_ident_sccs(self, lexer, mode, chunk, comment_str, comment_len) \
	((self)->TPP_INTERNAL(th_ident_sccs) ? (*(self)->TPP_INTERNAL(th_ident_sccs))(tpp_hooks_getcookie_ident_sccs(self, lexer), mode, chunk, comment_str, comment_len) : TPP_EOK)
#if TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_IDENT_SCCS)
#define _TPP_HOOKS_DEFAULT_IDENT_SCCS (&TPP_HOOK_IDENT_SCCS)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_IDENT_SCCS NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_ident_sccs(self, lexer)  (self)->TPP_INTERNAL(th_ident_sccs_cookie)
#define tpp_hooks_set_ident_sccs(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_ident_sccs) = (v), (self)->TPP_INTERNAL(th_ident_sccs_cookie) = (lexer))
#define tpp_hooks_set_ident_sccs_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_ident_sccs) = (v), (self)->TPP_INTERNAL(th_ident_sccs_cookie) = (cookie))
#if TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_IDENT_SCCS)
#define tpp_hooks_reset_ident_sccs(self, lexer) (void)((self)->TPP_INTERNAL(th_ident_sccs) = _TPP_HOOKS_DEFAULT_IDENT_SCCS, (self)->TPP_INTERNAL(th_ident_sccs_cookie) = (lexer))
#define _tpp_hooks_init_ident_sccs(self, lexer) , (self)->TPP_INTERNAL(th_ident_sccs) = _TPP_HOOKS_DEFAULT_IDENT_SCCS, (self)->TPP_INTERNAL(th_ident_sccs_cookie) = (lexer)
#else /* TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_IDENT_SCCS */
#define tpp_hooks_reset_ident_sccs(self, lexer) (void)((self)->TPP_INTERNAL(th_ident_sccs) = _TPP_HOOKS_DEFAULT_IDENT_SCCS)
#define _tpp_hooks_init_ident_sccs(self, lexer) , (self)->TPP_INTERNAL(th_ident_sccs) = _TPP_HOOKS_DEFAULT_IDENT_SCCS
#endif /* TPP_HAVE_IDENT_SCCS_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_IDENT_SCCS */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_ident_sccs(self, lexer) (lexer)
#define tpp_hooks_set_ident_sccs(self, lexer, v) (void)((self)->TPP_INTERNAL(th_ident_sccs) = (v))
#define tpp_hooks_reset_ident_sccs(self, lexer)  (void)((self)->TPP_INTERNAL(th_ident_sccs) = _TPP_HOOKS_DEFAULT_IDENT_SCCS)
#define _tpp_hooks_init_ident_sccs(self, lexer)  , (self)->TPP_INTERNAL(th_ident_sccs) = _TPP_HOOKS_DEFAULT_IDENT_SCCS
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_ident_sccs(self) (self)->TPP_INTERNAL(th_ident_sccs)
#else /* TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK) */
#if TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_ident_sccs(self, cookie, mode, chunk, comment_str, comment_len) \
	TPP_HOOK_IDENT_SCCS(cookie, mode, chunk, comment_str, comment_len)
#else /*  */
#define tpp_hooks_call_ident_sccs(self, cookie, mode, chunk, comment_str, comment_len) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_ident_sccs(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK) */

/* Extra callback invoked by `tpp_lexer_foreach_include_path()` at different
 * points during the process of enumerating include paths. This callback is
 * then allowed to enumerate some additional include paths that may exist, but
 * for one reason or another (mainly: speed) aren't known to TPP via its system
 * include path APIs (`tpp_lexer_includes_add*`)
 * @param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`, describing the
 *               caller's position in `tpp_lexer_foreach_include_path()`.
 * @return: * :            First non-TPP_ENOENT return value of `cb`
 * @return: TPP_ENOENT:    File still not found
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK)
#define tpp_hooks_call_system_include_path(self, lexer, mode, when, cb, arg) \
	((self)->TPP_INTERNAL(th_system_include_path) ? (*(self)->TPP_INTERNAL(th_system_include_path))(tpp_hooks_getcookie_system_include_path(self, lexer), mode, when, cb, arg) : TPP_ENOENT)
#if TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_SYSTEM_INCLUDE_PATH)
#define _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH (&TPP_HOOK_SYSTEM_INCLUDE_PATH)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_system_include_path(self, lexer)  (self)->TPP_INTERNAL(th_system_include_path_cookie)
#define tpp_hooks_set_system_include_path(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_system_include_path) = (v), (self)->TPP_INTERNAL(th_system_include_path_cookie) = (lexer))
#define tpp_hooks_set_system_include_path_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_system_include_path) = (v), (self)->TPP_INTERNAL(th_system_include_path_cookie) = (cookie))
#if TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_SYSTEM_INCLUDE_PATH)
#define tpp_hooks_reset_system_include_path(self, lexer) (void)((self)->TPP_INTERNAL(th_system_include_path) = _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH, (self)->TPP_INTERNAL(th_system_include_path_cookie) = (lexer))
#define _tpp_hooks_init_system_include_path(self, lexer) , (self)->TPP_INTERNAL(th_system_include_path) = _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH, (self)->TPP_INTERNAL(th_system_include_path_cookie) = (lexer)
#else /* TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_SYSTEM_INCLUDE_PATH */
#define tpp_hooks_reset_system_include_path(self, lexer) (void)((self)->TPP_INTERNAL(th_system_include_path) = _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH)
#define _tpp_hooks_init_system_include_path(self, lexer) , (self)->TPP_INTERNAL(th_system_include_path) = _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH
#endif /* TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_SYSTEM_INCLUDE_PATH */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_system_include_path(self, lexer) (lexer)
#define tpp_hooks_set_system_include_path(self, lexer, v) (void)((self)->TPP_INTERNAL(th_system_include_path) = (v))
#define tpp_hooks_reset_system_include_path(self, lexer)  (void)((self)->TPP_INTERNAL(th_system_include_path) = _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH)
#define _tpp_hooks_init_system_include_path(self, lexer)  , (self)->TPP_INTERNAL(th_system_include_path) = _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_system_include_path(self) (self)->TPP_INTERNAL(th_system_include_path)
#else /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */
#if TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_system_include_path(self, cookie, mode, when, cb, arg) \
	TPP_HOOK_SYSTEM_INCLUDE_PATH(cookie, mode, when, cb, arg)
#else /*  */
#define tpp_hooks_call_system_include_path(self, cookie, mode, when, cb, arg) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_system_include_path(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */

/* Extra callback invoked by `tpp_lexer_foreach_embed_path()` at different points
 * during the process of enumerating embed paths. (s.a. `TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK`)
 * @param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`, describing the
 *               caller's position in `tpp_lexer_foreach_embed_path()`.
 * @return: * :            First non-TPP_ENOENT return value of `cb`
 * @return: TPP_ENOENT:    File still not found
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK)
#define tpp_hooks_call_system_embed_path(self, lexer, mode, when, cb, arg) \
	((self)->TPP_INTERNAL(th_system_embed_path) ? (*(self)->TPP_INTERNAL(th_system_embed_path))(tpp_hooks_getcookie_system_embed_path(self, lexer), mode, when, cb, arg) : TPP_ENOENT)
#if TPP_HAVE_SYSTEM_EMBED_PATH_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_SYSTEM_EMBED_PATH)
#define _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH (&TPP_HOOK_SYSTEM_EMBED_PATH)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_system_embed_path(self, lexer)  (self)->TPP_INTERNAL(th_system_embed_path_cookie)
#define tpp_hooks_set_system_embed_path(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_system_embed_path) = (v), (self)->TPP_INTERNAL(th_system_embed_path_cookie) = (lexer))
#define tpp_hooks_set_system_embed_path_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_system_embed_path) = (v), (self)->TPP_INTERNAL(th_system_embed_path_cookie) = (cookie))
#if TPP_HAVE_SYSTEM_EMBED_PATH_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_SYSTEM_EMBED_PATH)
#define tpp_hooks_reset_system_embed_path(self, lexer) (void)((self)->TPP_INTERNAL(th_system_embed_path) = _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH, (self)->TPP_INTERNAL(th_system_embed_path_cookie) = (lexer))
#define _tpp_hooks_init_system_embed_path(self, lexer) , (self)->TPP_INTERNAL(th_system_embed_path) = _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH, (self)->TPP_INTERNAL(th_system_embed_path_cookie) = (lexer)
#else /* TPP_HAVE_SYSTEM_EMBED_PATH_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_SYSTEM_EMBED_PATH */
#define tpp_hooks_reset_system_embed_path(self, lexer) (void)((self)->TPP_INTERNAL(th_system_embed_path) = _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH)
#define _tpp_hooks_init_system_embed_path(self, lexer) , (self)->TPP_INTERNAL(th_system_embed_path) = _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH
#endif /* TPP_HAVE_SYSTEM_EMBED_PATH_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_SYSTEM_EMBED_PATH */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_system_embed_path(self, lexer) (lexer)
#define tpp_hooks_set_system_embed_path(self, lexer, v) (void)((self)->TPP_INTERNAL(th_system_embed_path) = (v))
#define tpp_hooks_reset_system_embed_path(self, lexer)  (void)((self)->TPP_INTERNAL(th_system_embed_path) = _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH)
#define _tpp_hooks_init_system_embed_path(self, lexer)  , (self)->TPP_INTERNAL(th_system_embed_path) = _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_system_embed_path(self) (self)->TPP_INTERNAL(th_system_embed_path)
#else /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */
#if TPP_HAVE_SYSTEM_EMBED_PATH_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_system_embed_path(self, cookie, mode, when, cb, arg) \
	TPP_HOOK_SYSTEM_EMBED_PATH(cookie, mode, when, cb, arg)
#else /*  */
#define tpp_hooks_call_system_embed_path(self, cookie, mode, when, cb, arg) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_system_embed_path(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */

/* Called by `tpp_lexer_decodestring()` when an unknown `\`-escape sequence is encountered.
 * This hook can be used to define additional, user-defined escape sequences, or any other
 * arbitrary behavior to-be performed when specific escape-sequences are found.
 * On entry, `*p_pos` points at the first (unrecognized) character after the leading `\`, and
 * if the hook was able to parse said escape sequence, it should update `*p_pos` to point after
 * it before returning.
 * @param: p_pos:  [in]  Pointer to start of unrecognized `\`-escape sequence
 *                 [out] First character no longer part of `\`-escape sequence (if recognized)
 *                 [out] Unchanged (if not recognized)
 * @param: end:    The of containing string sequence
 * @param: config: Identically-named argument of `tpp_lexer_decodestring()`
 * @return: * :    Sum of positive return values of `data_printer` and `utf8_printer`
 * @return: < 0:   First negative return value of `data_printer` or `utf8_printer`
 * @return: TPP_SSIZE_OFERR(TPP_ENOENT):    Escape sequence still not recognized
 *                 (please leave `*p_pos` unchanged in this case). The caller will
 *                 proceed by emitting `TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE`
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
 * @return: TPP_SSIZE_OFERR(TPP_EIO):       Filesystem I/O operation failed
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): A lexer error happened
 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK)
#define tpp_hooks_call_unknown_string_escape(self, lexer, p_pos, end, config) \
	((self)->TPP_INTERNAL(th_unknown_string_escape) ? (*(self)->TPP_INTERNAL(th_unknown_string_escape))(tpp_hooks_getcookie_unknown_string_escape(self, lexer), p_pos, end, config) : TPP_SSIZE_OFERR(TPP_ENOENT))
#if TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_UNKNOWN_STRING_ESCAPE)
#define _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE (&TPP_HOOK_UNKNOWN_STRING_ESCAPE)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_unknown_string_escape(self, lexer)  (self)->TPP_INTERNAL(th_unknown_string_escape_cookie)
#define tpp_hooks_set_unknown_string_escape(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_unknown_string_escape) = (v), (self)->TPP_INTERNAL(th_unknown_string_escape_cookie) = (lexer))
#define tpp_hooks_set_unknown_string_escape_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_unknown_string_escape) = (v), (self)->TPP_INTERNAL(th_unknown_string_escape_cookie) = (cookie))
#if TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_UNKNOWN_STRING_ESCAPE)
#define tpp_hooks_reset_unknown_string_escape(self, lexer) (void)((self)->TPP_INTERNAL(th_unknown_string_escape) = _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE, (self)->TPP_INTERNAL(th_unknown_string_escape_cookie) = (lexer))
#define _tpp_hooks_init_unknown_string_escape(self, lexer) , (self)->TPP_INTERNAL(th_unknown_string_escape) = _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE, (self)->TPP_INTERNAL(th_unknown_string_escape_cookie) = (lexer)
#else /* TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_UNKNOWN_STRING_ESCAPE */
#define tpp_hooks_reset_unknown_string_escape(self, lexer) (void)((self)->TPP_INTERNAL(th_unknown_string_escape) = _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE)
#define _tpp_hooks_init_unknown_string_escape(self, lexer) , (self)->TPP_INTERNAL(th_unknown_string_escape) = _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE
#endif /* TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_UNKNOWN_STRING_ESCAPE */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_unknown_string_escape(self, lexer) (lexer)
#define tpp_hooks_set_unknown_string_escape(self, lexer, v) (void)((self)->TPP_INTERNAL(th_unknown_string_escape) = (v))
#define tpp_hooks_reset_unknown_string_escape(self, lexer)  (void)((self)->TPP_INTERNAL(th_unknown_string_escape) = _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE)
#define _tpp_hooks_init_unknown_string_escape(self, lexer)  , (self)->TPP_INTERNAL(th_unknown_string_escape) = _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_unknown_string_escape(self) (self)->TPP_INTERNAL(th_unknown_string_escape)
#else /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */
#if TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_unknown_string_escape(self, cookie, p_pos, end, config) \
	TPP_HOOK_UNKNOWN_STRING_ESCAPE(cookie, p_pos, end, config)
#else /*  */
#define tpp_hooks_call_unknown_string_escape(self, cookie, p_pos, end, config) TPP_SSIZE_OFERR(TPP_ENOENT)
#endif /* ... */
#define _tpp_hooks_init_unknown_string_escape(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */

/* Called by `tpp_lexer_warnf()` just before it's about to return `TPP_ELEXERROR`
 * This hook can be used to do additional state changes that may be necessary by the
 * hosting application in order to handle the resulting `TPP_ELEXERROR`
 * @return: TPP_EOK:       Have `tpp_lexer_warnf()` return `TPP_ELEXERROR`
 * @return: TPP_ELEXERROR: Same as `TPP_EOK`
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK)
#define tpp_hooks_call_raise_lexerror(self, lexer) \
	((self)->TPP_INTERNAL(th_raise_lexerror) ? (*(self)->TPP_INTERNAL(th_raise_lexerror))(tpp_hooks_getcookie_raise_lexerror(self, lexer)) : TPP_ELEXERROR)
#if TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_RAISE_LEXERROR)
#define _TPP_HOOKS_DEFAULT_RAISE_LEXERROR (&TPP_HOOK_RAISE_LEXERROR)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_RAISE_LEXERROR NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_raise_lexerror(self, lexer)  (self)->TPP_INTERNAL(th_raise_lexerror_cookie)
#define tpp_hooks_set_raise_lexerror(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_raise_lexerror) = (v), (self)->TPP_INTERNAL(th_raise_lexerror_cookie) = (lexer))
#define tpp_hooks_set_raise_lexerror_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_raise_lexerror) = (v), (self)->TPP_INTERNAL(th_raise_lexerror_cookie) = (cookie))
#if TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_RAISE_LEXERROR)
#define tpp_hooks_reset_raise_lexerror(self, lexer) (void)((self)->TPP_INTERNAL(th_raise_lexerror) = _TPP_HOOKS_DEFAULT_RAISE_LEXERROR, (self)->TPP_INTERNAL(th_raise_lexerror_cookie) = (lexer))
#define _tpp_hooks_init_raise_lexerror(self, lexer) , (self)->TPP_INTERNAL(th_raise_lexerror) = _TPP_HOOKS_DEFAULT_RAISE_LEXERROR, (self)->TPP_INTERNAL(th_raise_lexerror_cookie) = (lexer)
#else /* TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_RAISE_LEXERROR */
#define tpp_hooks_reset_raise_lexerror(self, lexer) (void)((self)->TPP_INTERNAL(th_raise_lexerror) = _TPP_HOOKS_DEFAULT_RAISE_LEXERROR)
#define _tpp_hooks_init_raise_lexerror(self, lexer) , (self)->TPP_INTERNAL(th_raise_lexerror) = _TPP_HOOKS_DEFAULT_RAISE_LEXERROR
#endif /* TPP_HAVE_RAISE_LEXERROR_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_RAISE_LEXERROR */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_raise_lexerror(self, lexer) (lexer)
#define tpp_hooks_set_raise_lexerror(self, lexer, v) (void)((self)->TPP_INTERNAL(th_raise_lexerror) = (v))
#define tpp_hooks_reset_raise_lexerror(self, lexer)  (void)((self)->TPP_INTERNAL(th_raise_lexerror) = _TPP_HOOKS_DEFAULT_RAISE_LEXERROR)
#define _tpp_hooks_init_raise_lexerror(self, lexer)  , (self)->TPP_INTERNAL(th_raise_lexerror) = _TPP_HOOKS_DEFAULT_RAISE_LEXERROR
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_raise_lexerror(self) (self)->TPP_INTERNAL(th_raise_lexerror)
#else /* TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK) */
#if TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_raise_lexerror(self, cookie) \
	TPP_HOOK_RAISE_LEXERROR(cookie)
#else /*  */
#define tpp_hooks_call_raise_lexerror(self, cookie) TPP_ELEXERROR
#endif /* ... */
#define _tpp_hooks_init_raise_lexerror(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK) */

/* Called by `tpp_lexer_yieldraw()` when `TPP_HAVE_SMART_FLOAT_TOKENS` is enabled and
 * a sequence like `1.f` is encountered where the lexer is unsure if the `f` should be
 * part of the float-token (in the form of a float-suffix), or if this should actually be
 * parsed as 3 tokens: `[C_INT:1][DOT:.][f:f]`. For this purpose, this hook is called
 * with `pos` pointing at the `f` (additional characters thereafter may not be loaded
 * yet, though can be loaded using `tpp_lexer_readchar()` and `tpp_lexer_readunichar()`)
 * @return: TPP_EOK:       Pointed-to location actually *does* refer to a float suffix
 * @return: TPP_ENOENT:    It's not a float suffix
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#if TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK)
#define tpp_hooks_call_isfloatsuffix(self, lexer, pos) \
	((self)->TPP_INTERNAL(th_isfloatsuffix) ? (*(self)->TPP_INTERNAL(th_isfloatsuffix))(tpp_hooks_getcookie_isfloatsuffix(self, lexer), pos) : TPP_ENOENT)
#if TPP_HAVE_ISFLOATSUFFIX_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_ISFLOATSUFFIX)
#define _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX (&TPP_HOOK_ISFLOATSUFFIX)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX NULL
#endif /* !... */
#if TPP_HAVE_HOOK_COOKIES
#define tpp_hooks_getcookie_isfloatsuffix(self, lexer)  (self)->TPP_INTERNAL(th_isfloatsuffix_cookie)
#define tpp_hooks_set_isfloatsuffix(self, lexer, v)     (void)((self)->TPP_INTERNAL(th_isfloatsuffix) = (v), (self)->TPP_INTERNAL(th_isfloatsuffix_cookie) = (lexer))
#define tpp_hooks_set_isfloatsuffix_ex(self, v, cookie) (void)((self)->TPP_INTERNAL(th_isfloatsuffix) = (v), (self)->TPP_INTERNAL(th_isfloatsuffix_cookie) = (cookie))
#if TPP_HAVE_ISFLOATSUFFIX_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_ISFLOATSUFFIX)
#define tpp_hooks_reset_isfloatsuffix(self, lexer) (void)((self)->TPP_INTERNAL(th_isfloatsuffix) = _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX, (self)->TPP_INTERNAL(th_isfloatsuffix_cookie) = (lexer))
#define _tpp_hooks_init_isfloatsuffix(self, lexer) , (self)->TPP_INTERNAL(th_isfloatsuffix) = _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX, (self)->TPP_INTERNAL(th_isfloatsuffix_cookie) = (lexer)
#else /* TPP_HAVE_ISFLOATSUFFIX_HOOK == TPP_HOOK_RT_USER && TPP_HOOK_ISFLOATSUFFIX */
#define tpp_hooks_reset_isfloatsuffix(self, lexer) (void)((self)->TPP_INTERNAL(th_isfloatsuffix) = _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX)
#define _tpp_hooks_init_isfloatsuffix(self, lexer) , (self)->TPP_INTERNAL(th_isfloatsuffix) = _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX
#endif /* TPP_HAVE_ISFLOATSUFFIX_HOOK != TPP_HOOK_RT_USER || !TPP_HOOK_ISFLOATSUFFIX */
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_getcookie_isfloatsuffix(self, lexer) (lexer)
#define tpp_hooks_set_isfloatsuffix(self, lexer, v) (void)((self)->TPP_INTERNAL(th_isfloatsuffix) = (v))
#define tpp_hooks_reset_isfloatsuffix(self, lexer)  (void)((self)->TPP_INTERNAL(th_isfloatsuffix) = _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX)
#define _tpp_hooks_init_isfloatsuffix(self, lexer)  , (self)->TPP_INTERNAL(th_isfloatsuffix) = _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_hooks_get_isfloatsuffix(self) (self)->TPP_INTERNAL(th_isfloatsuffix)
#else /* TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK) */
#if TPP_HAVE_ISFLOATSUFFIX_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_isfloatsuffix(self, cookie, pos) \
	TPP_HOOK_ISFLOATSUFFIX(cookie, pos)
#else /*  */
#define tpp_hooks_call_isfloatsuffix(self, cookie, pos) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_isfloatsuffix(self, lexer) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK) */

/* Initialize lexer hooks */
#define tpp_hooks_init(self, lexer) \
	(void)((void)0 _tpp_hooks_init_warnprinter(self, lexer) \
	       _tpp_hooks_init_warnhandler(self, lexer) \
	       _tpp_hooks_init_mesgprinter(self, lexer) \
	       _tpp_hooks_init_parseexpr(self, lexer) \
	       _tpp_hooks_init_unknown_pragma(self, lexer) \
	       _tpp_hooks_init_new_dependency(self, lexer) \
	       _tpp_hooks_init_file_pushed(self, lexer) \
	       _tpp_hooks_init_file_popped(self, lexer) \
	       _tpp_hooks_init_include_encountered(self, lexer) \
	       _tpp_hooks_init_include_not_found(self, lexer) \
	       _tpp_hooks_init_macro_defined(self, lexer) \
	       _tpp_hooks_init_macro_undefined(self, lexer) \
	       _tpp_hooks_init_ident_sccs(self, lexer) \
	       _tpp_hooks_init_system_include_path(self, lexer) \
	       _tpp_hooks_init_system_embed_path(self, lexer) \
	       _tpp_hooks_init_unknown_string_escape(self, lexer) \
	       _tpp_hooks_init_raise_lexerror(self, lexer) \
	       _tpp_hooks_init_isfloatsuffix(self, lexer))
/*[[[end]]]*/


/************************************************************************/
/* Builtin hooks...                                                     */
/************************************************************************/
#if TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK
TPP_DECL TPP_FORMATPRINTER_DEFINE(_tpp_lexer_builtin_warn_or_mesg_printer, arg, text, num_bytes);
#endif /* TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK */

#if TPP_HAVE_BUILTIN_WARNHANDLER_HOOK
struct tpp_lexer_printf_info;
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
_tpp_lexer_builtin_warnhandler(tpp_hook_cookie lexer_cookie,
                               struct tpp_lexer_printf_info *tpp_restrict info,
                               tpp_warning_invokeinfo const *tpp_restrict invokeinfo,
                               tpp_warning_id id, va_list args);
#endif /* TPP_HAVE_BUILTIN_WARNHANDLER_HOOK */

#if TPP_HAVE_BUILTIN_PARSEEXPR_HOOK
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_lexer_builtin_parseexpr(tpp_hook_cookie lexer_cookie, tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE_BUILTIN_PARSEEXPR_HOOK */
/************************************************************************/


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_HOOKS_H */
