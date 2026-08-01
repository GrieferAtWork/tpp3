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
print("struct tpp_lexer;");
print("struct tpp_lexer_decodestring_config;");
print("#if TPP_HAVE_WARNINGS");
print("struct tpp_lexer_printf_info;");
print("#endif /" "* TPP_HAVE_WARNINGS *" "/");
print("typedef struct tpp_hooks {");
function fixProto(x) -> x
	.rereplace(r"\btpp_lexer\b", "struct tpp_lexer")
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
	print("/" "* ", doc.strip().replace("\n", "\n * ").rstriplines(), " *" "/");
	print("#if TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK)");
	local isFormatPrinter = prototypePrefix.strip() == "tpp_formatprinter" && !prototypeSuffix;
	local hookMustBeFunctionPointer = isFormatPrinter;
	if (builtin_FOO_HOOK) {
		print("#if TPP_HAVE_", name, "_HOOK != TPP_HOOK_RT_NOOP");
		print("#define tpp_hooks_call_", name.lower(), "(self", "".join(for (local x: prototypeArgs) f", {x}"), ") \\");
		if (isFormatPrinter) {
			print("	tpp_formatprinter_print((self)->TPP_INTERNAL(th_", name.lower(), "), ", ", ".join(prototypeArgs), ")");
		} else {
			print("	(*(self)->TPP_INTERNAL(th_", name.lower(), "))(", ", ".join(prototypeArgs), ")");
		}
		print("#else /" "* TPP_HAVE_", name, "_HOOK != TPP_HOOK_RT_NOOP *" "/");
	}
	print("#define tpp_hooks_call_", name.lower(), "(self", "".join(for (local x: prototypeArgs) f", {x}"), ") \\");
	print("	((self)->TPP_INTERNAL(th_", name.lower(), ") "),;
	if (isFormatPrinter) {
		print("? tpp_formatprinter_print((self)->TPP_INTERNAL(th_", name.lower(), "), ", ", ".join(prototypeArgs), ") "),;
	} else {
		print("? (*(self)->TPP_INTERNAL(th_", name.lower(), "))(", ", ".join(prototypeArgs), ") "),;
	}
	print(": ", disabled_RETURN_VALUE, ")");
	if (builtin_FOO_HOOK)
		print("#endif /" "* TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_NOOP *" "/");
	print("#define tpp_hooks_get_", name.lower(), "(self)    (self)->TPP_INTERNAL(th_", name.lower(), ")");
	print("#define tpp_hooks_set_", name.lower(), "(self, v) (void)((self)->TPP_INTERNAL(th_", name.lower(), ") = (v))");
	print("#define tpp_hooks_reset_", name.lower(), "(self)  (void)((self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name, ")");
	print("#define _tpp_hooks_init_", name.lower(), "(self)  , (self)->TPP_INTERNAL(th_", name.lower(), ") = _TPP_HOOKS_DEFAULT_", name);
	print("#if TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_", name, ")");
	print("#define _TPP_HOOKS_DEFAULT_", name, " (&TPP_HOOK_", name, ")");
	if (builtin_FOO_HOOK) {
		print("#elif TPP_HAVE_", name, "_HOOK == TPP_HOOK_RT_BUILTIN");
		print("#define _TPP_HOOKS_DEFAULT_", name, " (&", builtin_FOO_HOOK, ")");
	}
	print("#else /" "* ... *" "/");
	print("#define _TPP_HOOKS_DEFAULT_", name, " NULL");
	print("#endif /" "* !... *" "/");
	print("#else /" "* TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK) *" "/");
	print("#if TPP_HAVE_", name, "_HOOK == TPP_HOOK_CONST_USER");
	if (hookMustBeFunctionPointer)
		print("#define tpp_hooks_get_", name.lower(), "(self) (&TPP_HOOK_", name, ")");
	print("#define tpp_hooks_call_", name.lower(), "(self", "".join(for (local x: prototypeArgs) f", {x}"), ") \\");
	print("	TPP_HOOK_", name, "(", ", ".join(prototypeArgs), ")");
	if (builtin_FOO_HOOK) {
		print("#elif TPP_HAVE_", name, "_HOOK == TPP_HOOK_CONST_BUILTIN");
		if (hookMustBeFunctionPointer)
			print("#define tpp_hooks_get_", name.lower(), "(self) (&", builtin_FOO_HOOK, ")");
		print("#define tpp_hooks_call_", name.lower(), "(self", "".join(for (local x: prototypeArgs) f", {x}"), ") \\");
		print("	", builtin_FOO_HOOK, "(", ", ".join(prototypeArgs), ")");
	}
	print("#else /" "*  *" "/");
	print("#define tpp_hooks_call_", name.lower(), "(self", "".join(for (local x: prototypeArgs) f", {x}"), ") ", disabled_RETURN_VALUE);
	print("#endif /" "* ... *" "/");
	print("#define _tpp_hooks_init_", name.lower(), "(self) /" "* nothing *" "/");
	print("#endif /" "* !TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK) *" "/");
	print;
}
print("/" "* Initialize lexer hooks *" "/");
print("#define tpp_hooks_init(self) \\");
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
	print("_tpp_hooks_init_", name.lower(), "(self)"),;
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
struct tpp_lexer;
struct tpp_lexer_decodestring_config;
#if TPP_HAVE_WARNINGS
struct tpp_lexer_printf_info;
#endif /* TPP_HAVE_WARNINGS */
typedef struct tpp_hooks {
	/* >> tpp_formatprinter th_warnprinter;
	 * Called by `tpp_lexer_warnf()` to print warning messages
	 * Potentially unused if `TPP_HAVE_WARNHANDLER_HOOK` is also overwritten
	 * @param: arg: The current lexer (`tpp_lexer *`) */
#if TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK)
#if TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP
	tpp_formatprinter TPP_INTERNAL(th_warnprinter); /* [1..1] */
#else /* TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP */
	tpp_formatprinter TPP_INTERNAL(th_warnprinter); /* [0..1] */
#endif /* TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_NOOP */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */

	/* >> tpp_errno (TPPCALL *th_warnhandler)(struct tpp_lexer *tpp_restrict self, struct tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args);
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
#if TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK)
#if TPP_HAVE_WARNHANDLER_HOOK != TPP_HOOK_RT_NOOP
	tpp_errno (TPPCALL *TPP_INTERNAL(th_warnhandler))(struct tpp_lexer *tpp_restrict self, struct tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args); /* [1..1] */
#else /* TPP_HAVE_WARNHANDLER_HOOK != TPP_HOOK_RT_NOOP */
	tpp_errno (TPPCALL *TPP_INTERNAL(th_warnhandler))(struct tpp_lexer *tpp_restrict self, struct tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args); /* [0..1] */
#endif /* TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_RT_NOOP */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK) */

	/* >> tpp_formatprinter th_mesgprinter;
	 * Used by `#pragma message` to print messages
	 * @param: arg: The current lexer (`tpp_lexer *`) */
#if TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK)
#if TPP_HAVE_MESGPRINTER_HOOK != TPP_HOOK_RT_NOOP
	tpp_formatprinter TPP_INTERNAL(th_mesgprinter); /* [1..1] */
#else /* TPP_HAVE_MESGPRINTER_HOOK != TPP_HOOK_RT_NOOP */
	tpp_formatprinter TPP_INTERNAL(th_mesgprinter); /* [0..1] */
#endif /* TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_NOOP */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK) */

	/* >> tpp_errno (TPPCALL *th_parseexpr)(struct tpp_lexer *tpp_restrict self, tpp_expr_value *tpp_restrict result);
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
#if TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK)
#if TPP_HAVE_PARSEEXPR_HOOK != TPP_HOOK_RT_NOOP
	tpp_errno (TPPCALL *TPP_INTERNAL(th_parseexpr))(struct tpp_lexer *tpp_restrict self, tpp_expr_value *tpp_restrict result); /* [1..1] */
#else /* TPP_HAVE_PARSEEXPR_HOOK != TPP_HOOK_RT_NOOP */
	tpp_errno (TPPCALL *TPP_INTERNAL(th_parseexpr))(struct tpp_lexer *tpp_restrict self, tpp_expr_value *tpp_restrict result); /* [0..1] */
#endif /* TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_RT_NOOP */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) */

	/* >> tpp_errno (TPPCALL *th_unknown_pragma)(struct tpp_lexer *tpp_restrict self);
	 * Called whenever a `#pragma` is encountered that is not recognized.
	 * When called, the lexer is set-up to point at the first token after the `#pragma`.
	 * @return: TPP_EOK:    Pragma has been handled
	 * @return: TPP_ENOENT: Pragma is still unknown, and a warning should be emitted
	 * @return: TPP_EIO:    I/O error
	 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_unknown_pragma))(struct tpp_lexer *tpp_restrict self); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */

	/* >> tpp_errno (TPPCALL *th_new_dependency)(struct tpp_lexer *tpp_restrict self, tpp_keyword *filename_kwd);
	 * Called whenever some file is `#include`-ed for the first time
	 * @param: filename_kwd: Then `tpp_keyword` used to describe the file's name. The actual
	 *                       filename can be queried as `tpp_keyword_getcstr(filename_kwd)`. */
#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_new_dependency))(struct tpp_lexer *tpp_restrict self, tpp_keyword *filename_kwd); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

	/* >> tpp_errno (TPPCALL *th_file_pushed)(struct tpp_lexer *tpp_restrict self);
	 * Called whenever a file was just pushed onto the `#include`-stack
	 * Information about the just-pushed file can be retrieved by examining `tpp_lexer_getfile(self)`
	 * This hook can be used by a frontend to implement stuff like GCC's `--trace-includes`.
	 * WARNING: *NOT* Called for `tpp_file_subtext_push()` or `tpp_file_pushdummy()` */
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_file_pushed))(struct tpp_lexer *tpp_restrict self); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) */

	/* >> void (TPPCALL *th_file_popped)(struct tpp_lexer *tpp_restrict self);
	 * Called whenever a file is about to be popped off the `#include`-stack
	 * Information about the file that's about-to-be popped can be retrieved
	 * by examining `tpp_lexer_getfile(self)`. Note that this hook is called
	 * during the file-pop *commit* phase (`tpp_lexer_manualpopfile_break_commit()`)
	 * but is *NOT* called by `tpp_lexer_manualpopfile_popfile()`.
	 * WARNING: *NOT* Called for `tpp_file_subtext_pop()` or `tpp_file_popdummy()` */
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK)
	void (TPPCALL *TPP_INTERNAL(th_file_popped))(struct tpp_lexer *tpp_restrict self); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK) */

	/* >> tpp_errno (TPPCALL *th_include_encountered)(struct tpp_lexer *tpp_restrict self, tpp_hook_include_kind include_kind);
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
	 * @return: TPP_EOK:    Continue handling like usual
	 * @return: TPP_ENOENT: Don't attempt to find/open a file. Instead, continue processing
	 *                      the file containing the `#include`-directive as though the file
	 *                      could not be found, and the `TPP_W_NO_SUCH_FILE` error was being
	 *                      suppressed.
	 * @return: TPP_E*:     Some other error -- propagate immdediately */
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_include_encountered))(struct tpp_lexer *tpp_restrict self, tpp_hook_include_kind include_kind); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */

	/* >> tpp_errno (TPPCALL *th_include_not_found)(struct tpp_lexer *tpp_restrict self, tpp_hook_include_kind include_kind);
	 * Called when the file specified by a `#include` (or `#include_next`, `#import` or `#embed`)-
	 * directive could not be found, this hook may be used to either suppress the error (by returning
	 * something other than `TPP_ENOENT`), or log the error to implement something like GCC's `-MG`
	 * commandline switch.
	 *
	 * This hook is called just before `TPP_W_NO_SUCH_FILE` would be emitted, with the lexer's
	 * current token still being the `<stdio.h>` or `"file.h"` string, meaning if you want
	 * to know what that string says, you can use `tpp_lexer_decode_include_string_cb()` to decode it.
	 *
	 * @param: include_kind: The kind of directive that this is (one of `TPP_HOOK_INCLUDE_KIND_*`)
	 * @return: TPP_EOK:    Suppress the accompanying `TPP_W_NO_SUCH_FILE` error, but continue acting like
	 *                      the file could not be found (*DONT* use this hook to manually push a file or
	 *                      something like that)
	 * @return: TPP_ENOENT: Emit the `TPP_W_NO_SUCH_FILE` error
	 * @return: TPP_E*:     Some other error -- propagate immdediately */
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_include_not_found))(struct tpp_lexer *tpp_restrict self, tpp_hook_include_kind include_kind); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */

	/* >> tpp_errno (TPPCALL *th_macro_defined)(struct tpp_lexer *tpp_restrict self, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro);
	 * Called whenever a `#define` directive has just been fully
	 * parsed (macro was has not yet been registered with keyword).
	 *
	 * This hook is *ONLY* invoked when `#define` is encountered, or
	 * `#pragma pop_macro("foo")` was used to restore a macro's previous
	 * definition.
	 *
	 * Calls to `tpp_lexer_define()` or other related functions will
	 * *NOT* invoke this hook. */
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_macro_defined))(struct tpp_lexer *tpp_restrict self, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK) */

	/* >> tpp_errno (TPPCALL *th_macro_undefined)(struct tpp_lexer *tpp_restrict self, tpp_keyword *tpp_restrict name);
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
	 * NOTE: this hook *will* actually also be called by `#pragma push_macro(undef, "foo")` */
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_macro_undefined))(struct tpp_lexer *tpp_restrict self, tpp_keyword *tpp_restrict name); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK) */

	/* >> tpp_errno (TPPCALL *th_ident_sccs)(struct tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len);
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
#if TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_ident_sccs))(struct tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK) */

	/* >> tpp_errno (TPPCALL *th_system_include_path)(struct tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg);
	 * Extra callback invoked by `tpp_lexer_foreach_include_path()` at different
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
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_system_include_path))(struct tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */

	/* >> tpp_errno (TPPCALL *th_system_embed_path)(struct tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg);
	 * Extra callback invoked by `tpp_lexer_foreach_embed_path()` at different points
	 * during the process of enumerating embed paths. (s.a. `TPP_HOOK_SYSTEM_INCLUDE_PATH`)
	 * @param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`: describes the
	 *               caller's position in `tpp_lexer_foreach_include_path()`.
	 * @return: * :         First non-TPP_ENOENT return value of `cb`
	 * @return: TPP_ENOENT: File still not found
	 * @return: TPP_EIO:    I/O error
	 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_system_embed_path))(struct tpp_lexer *tpp_restrict self, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */

	/* >> tpp_ssize (TPPCALL *th_unknown_string_escape)(struct tpp_lexer *tpp_restrict self, tpp_char const **p_pos, tpp_char const *end, struct tpp_lexer_decodestring_config const *tpp_restrict config);
	 * Called by `tpp_lexer_decodestring()` when an unknown `\`-escape sequence is encountered
	 * This hook can be used to define additional, user-defined escape sequences, or any other
	 * arbitrary behavior to-be performed when specific escape-sequences are found.
	 * On entry, `*p_pos` points at the first (unrecognized) character after the leading `\`, and
	 * if the hook was able to parse said escape sequence, it should update `*p_pos` to point after
	 * it before returning
	 * @param: p_pos:  [in]  Pointer to start of unrecognized `\`-escape sequence
	 *                 [out] First character no longer part of `\`-escape sequence (if recognized)
	 *                 [out] Unchanged (if not recognized)
	 * @param: end:    The of containing string sequence
	 * @param: config: Identically-named argument of `tpp_lexer_decodestring()`
	 * @return: * :    Sum of positive return values of `data_printer` and `utf8_printer`
	 * @return: < 0:   First negative return value of `data_printer` or `utf8_printer`
	 * @return: TPP_SSIZE_OFERR(TPP_ENOENT): Escape sequence still not recognized
	 *                 (please leave `*p_pos` unchanged in this case). The caller will
	 *                 proceed by emitting `TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE` */
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK)
	tpp_ssize (TPPCALL *TPP_INTERNAL(th_unknown_string_escape))(struct tpp_lexer *tpp_restrict self, tpp_char const **p_pos, tpp_char const *end, struct tpp_lexer_decodestring_config const *tpp_restrict config); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */

	/* >> tpp_errno (TPPCALL *th_raise_lexerror)(struct tpp_lexer *tpp_restrict self);
	 * Called by `tpp_lexer_warnf()` just before it's about to return `TPP_ELEXERROR`
	 * This hook can be used to do additional state changes that may be necessary by the
	 * hosting application in order to handle the resulting `TPP_ELEXERROR`
	 * @return: TPP_EOK: Have `tpp_lexer_warnf()` still return `TPP_ELEXERROR`
	 * @return: * :      Make `tpp_lexer_warnf()` return this instead of `TPP_ELEXERROR` */
#if TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_raise_lexerror))(struct tpp_lexer *tpp_restrict self); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK) */

	/* >> tpp_errno (TPPCALL *th_isfloatsuffix)(struct tpp_lexer *tpp_restrict self, tpp_char const *pos);
	 * Called by `tpp_lexer_yieldraw()` when `TPP_HAVE_SMART_FLOAT_TOKENS` is enabled and
	 * a sequence like `1.f` is encountered where the lexer is unsure if the `f` should be
	 * part of the float-token (in the form of a float-suffix), or if this is actually be
	 * parsed as 3 tokens: `[C_INT:1][DOT:.][f:f]`. For this purpose, this hook is called
	 * with `pos` pointing at the `f` (though additional characters thereafter may not be
	 * loaded yet, though can be loaded using `tpp_lexer_readchar()`)
	 * @return: TPP_EOK:    Pointed-to location actually *does* refer to a float suffix
	 * @return: TPP_ENOENT: It's not a float suffix
	 * @return: TPP_EIO:    I/O error
	 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_isfloatsuffix))(struct tpp_lexer *tpp_restrict self, tpp_char const *pos); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK) */
} tpp_hooks;
#endif /* TPP_HAVE_HOOKS */

/* Called by `tpp_lexer_warnf()` to print warning messages
 * Potentially unused if `TPP_HAVE_WARNHANDLER_HOOK` is also overwritten
 * @param: arg: The current lexer (`tpp_lexer *`) */
#if TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK)
#if TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) \
	tpp_formatprinter_print((self)->TPP_INTERNAL(th_warnprinter), lexer, text, num_bytes)
#else /* TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP */
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) \
	((self)->TPP_INTERNAL(th_warnprinter) ? tpp_formatprinter_print((self)->TPP_INTERNAL(th_warnprinter), lexer, text, num_bytes) : 0)
#endif /* TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_NOOP */
#define tpp_hooks_get_warnprinter(self)    (self)->TPP_INTERNAL(th_warnprinter)
#define tpp_hooks_set_warnprinter(self, v) (void)((self)->TPP_INTERNAL(th_warnprinter) = (v))
#define tpp_hooks_reset_warnprinter(self)  (void)((self)->TPP_INTERNAL(th_warnprinter) = _TPP_HOOKS_DEFAULT_WARNPRINTER)
#define _tpp_hooks_init_warnprinter(self)  , (self)->TPP_INTERNAL(th_warnprinter) = _TPP_HOOKS_DEFAULT_WARNPRINTER
#if TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_WARNPRINTER)
#define _TPP_HOOKS_DEFAULT_WARNPRINTER (&TPP_HOOK_WARNPRINTER)
#elif TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_BUILTIN
#define _TPP_HOOKS_DEFAULT_WARNPRINTER (&_tpp_lexer_builtin_warn_or_mesg_printer)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_WARNPRINTER NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */
#if TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_get_warnprinter(self) (&TPP_HOOK_WARNPRINTER)
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) \
	TPP_HOOK_WARNPRINTER(lexer, text, num_bytes)
#elif TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_CONST_BUILTIN
#define tpp_hooks_get_warnprinter(self) (&_tpp_lexer_builtin_warn_or_mesg_printer)
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) \
	_tpp_lexer_builtin_warn_or_mesg_printer(lexer, text, num_bytes)
#else /*  */
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) 0
#endif /* ... */
#define _tpp_hooks_init_warnprinter(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */

/* Called by `tpp_lexer_warnf()` to handle warning notifications. Can be
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
#if TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK)
#if TPP_HAVE_WARNHANDLER_HOOK != TPP_HOOK_RT_NOOP
#define tpp_hooks_call_warnhandler(self, lexer, info, invokeinfo, id, args) \
	(*(self)->TPP_INTERNAL(th_warnhandler))(lexer, info, invokeinfo, id, args)
#else /* TPP_HAVE_WARNHANDLER_HOOK != TPP_HOOK_RT_NOOP */
#define tpp_hooks_call_warnhandler(self, lexer, info, invokeinfo, id, args) \
	((self)->TPP_INTERNAL(th_warnhandler) ? (*(self)->TPP_INTERNAL(th_warnhandler))(lexer, info, invokeinfo, id, args) : TPP_EOK)
#endif /* TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_RT_NOOP */
#define tpp_hooks_get_warnhandler(self)    (self)->TPP_INTERNAL(th_warnhandler)
#define tpp_hooks_set_warnhandler(self, v) (void)((self)->TPP_INTERNAL(th_warnhandler) = (v))
#define tpp_hooks_reset_warnhandler(self)  (void)((self)->TPP_INTERNAL(th_warnhandler) = _TPP_HOOKS_DEFAULT_WARNHANDLER)
#define _tpp_hooks_init_warnhandler(self)  , (self)->TPP_INTERNAL(th_warnhandler) = _TPP_HOOKS_DEFAULT_WARNHANDLER
#if TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_WARNHANDLER)
#define _TPP_HOOKS_DEFAULT_WARNHANDLER (&TPP_HOOK_WARNHANDLER)
#elif TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_RT_BUILTIN
#define _TPP_HOOKS_DEFAULT_WARNHANDLER (&_tpp_lexer_builtin_warnhandler)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_WARNHANDLER NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK) */
#if TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_warnhandler(self, lexer, info, invokeinfo, id, args) \
	TPP_HOOK_WARNHANDLER(lexer, info, invokeinfo, id, args)
#elif TPP_HAVE_WARNHANDLER_HOOK == TPP_HOOK_CONST_BUILTIN
#define tpp_hooks_call_warnhandler(self, lexer, info, invokeinfo, id, args) \
	_tpp_lexer_builtin_warnhandler(lexer, info, invokeinfo, id, args)
#else /*  */
#define tpp_hooks_call_warnhandler(self, lexer, info, invokeinfo, id, args) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_warnhandler(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK) */

/* Used by `#pragma message` to print messages
 * @param: arg: The current lexer (`tpp_lexer *`) */
#if TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK)
#if TPP_HAVE_MESGPRINTER_HOOK != TPP_HOOK_RT_NOOP
#define tpp_hooks_call_mesgprinter(self, lexer, text, num_bytes) \
	tpp_formatprinter_print((self)->TPP_INTERNAL(th_mesgprinter), lexer, text, num_bytes)
#else /* TPP_HAVE_MESGPRINTER_HOOK != TPP_HOOK_RT_NOOP */
#define tpp_hooks_call_mesgprinter(self, lexer, text, num_bytes) \
	((self)->TPP_INTERNAL(th_mesgprinter) ? tpp_formatprinter_print((self)->TPP_INTERNAL(th_mesgprinter), lexer, text, num_bytes) : 0)
#endif /* TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_NOOP */
#define tpp_hooks_get_mesgprinter(self)    (self)->TPP_INTERNAL(th_mesgprinter)
#define tpp_hooks_set_mesgprinter(self, v) (void)((self)->TPP_INTERNAL(th_mesgprinter) = (v))
#define tpp_hooks_reset_mesgprinter(self)  (void)((self)->TPP_INTERNAL(th_mesgprinter) = _TPP_HOOKS_DEFAULT_MESGPRINTER)
#define _tpp_hooks_init_mesgprinter(self)  , (self)->TPP_INTERNAL(th_mesgprinter) = _TPP_HOOKS_DEFAULT_MESGPRINTER
#if TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_MESGPRINTER)
#define _TPP_HOOKS_DEFAULT_MESGPRINTER (&TPP_HOOK_MESGPRINTER)
#elif TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_RT_BUILTIN
#define _TPP_HOOKS_DEFAULT_MESGPRINTER (&_tpp_lexer_builtin_warn_or_mesg_printer)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_MESGPRINTER NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK) */
#if TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_get_mesgprinter(self) (&TPP_HOOK_MESGPRINTER)
#define tpp_hooks_call_mesgprinter(self, lexer, text, num_bytes) \
	TPP_HOOK_MESGPRINTER(lexer, text, num_bytes)
#elif TPP_HAVE_MESGPRINTER_HOOK == TPP_HOOK_CONST_BUILTIN
#define tpp_hooks_get_mesgprinter(self) (&_tpp_lexer_builtin_warn_or_mesg_printer)
#define tpp_hooks_call_mesgprinter(self, lexer, text, num_bytes) \
	_tpp_lexer_builtin_warn_or_mesg_printer(lexer, text, num_bytes)
#else /*  */
#define tpp_hooks_call_mesgprinter(self, lexer, text, num_bytes) 0
#endif /* ... */
#define _tpp_hooks_init_mesgprinter(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK) */

/* User-defined callback for parsing `#if`-style expressions
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
#if TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK)
#if TPP_HAVE_PARSEEXPR_HOOK != TPP_HOOK_RT_NOOP
#define tpp_hooks_call_parseexpr(self, lexer, result) \
	(*(self)->TPP_INTERNAL(th_parseexpr))(lexer, result)
#else /* TPP_HAVE_PARSEEXPR_HOOK != TPP_HOOK_RT_NOOP */
#define tpp_hooks_call_parseexpr(self, lexer, result) \
	((self)->TPP_INTERNAL(th_parseexpr) ? (*(self)->TPP_INTERNAL(th_parseexpr))(lexer, result) : tpp_expr_value_init_zero(result))
#endif /* TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_RT_NOOP */
#define tpp_hooks_get_parseexpr(self)    (self)->TPP_INTERNAL(th_parseexpr)
#define tpp_hooks_set_parseexpr(self, v) (void)((self)->TPP_INTERNAL(th_parseexpr) = (v))
#define tpp_hooks_reset_parseexpr(self)  (void)((self)->TPP_INTERNAL(th_parseexpr) = _TPP_HOOKS_DEFAULT_PARSEEXPR)
#define _tpp_hooks_init_parseexpr(self)  , (self)->TPP_INTERNAL(th_parseexpr) = _TPP_HOOKS_DEFAULT_PARSEEXPR
#if TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_PARSEEXPR)
#define _TPP_HOOKS_DEFAULT_PARSEEXPR (&TPP_HOOK_PARSEEXPR)
#elif TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_RT_BUILTIN
#define _TPP_HOOKS_DEFAULT_PARSEEXPR (&_tpp_lexer_builtin_parseexpr)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_PARSEEXPR NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) */
#if TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_parseexpr(self, lexer, result) \
	TPP_HOOK_PARSEEXPR(lexer, result)
#elif TPP_HAVE_PARSEEXPR_HOOK == TPP_HOOK_CONST_BUILTIN
#define tpp_hooks_call_parseexpr(self, lexer, result) \
	_tpp_lexer_builtin_parseexpr(lexer, result)
#else /*  */
#define tpp_hooks_call_parseexpr(self, lexer, result) tpp_expr_value_init_zero(result)
#endif /* ... */
#define _tpp_hooks_init_parseexpr(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) */

/* Called whenever a `#pragma` is encountered that is not recognized.
 * When called, the lexer is set-up to point at the first token after the `#pragma`.
 * @return: TPP_EOK:    Pragma has been handled
 * @return: TPP_ENOENT: Pragma is still unknown, and a warning should be emitted
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
#define tpp_hooks_call_unknown_pragma(self, lexer) \
	((self)->TPP_INTERNAL(th_unknown_pragma) ? (*(self)->TPP_INTERNAL(th_unknown_pragma))(lexer) : TPP_ENOENT)
#define tpp_hooks_get_unknown_pragma(self)    (self)->TPP_INTERNAL(th_unknown_pragma)
#define tpp_hooks_set_unknown_pragma(self, v) (void)((self)->TPP_INTERNAL(th_unknown_pragma) = (v))
#define tpp_hooks_reset_unknown_pragma(self)  (void)((self)->TPP_INTERNAL(th_unknown_pragma) = _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA)
#define _tpp_hooks_init_unknown_pragma(self)  , (self)->TPP_INTERNAL(th_unknown_pragma) = _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA
#if TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_UNKNOWN_PRAGMA)
#define _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA (&TPP_HOOK_UNKNOWN_PRAGMA)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_UNKNOWN_PRAGMA NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */
#if TPP_HAVE_UNKNOWN_PRAGMA_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_unknown_pragma(self, lexer) \
	TPP_HOOK_UNKNOWN_PRAGMA(lexer)
#else /*  */
#define tpp_hooks_call_unknown_pragma(self, lexer) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_unknown_pragma(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */

/* Called whenever some file is `#include`-ed for the first time
 * @param: filename_kwd: Then `tpp_keyword` used to describe the file's name. The actual
 *                       filename can be queried as `tpp_keyword_getcstr(filename_kwd)`. */
#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
#define tpp_hooks_call_new_dependency(self, lexer, filename_kwd) \
	((self)->TPP_INTERNAL(th_new_dependency) ? (*(self)->TPP_INTERNAL(th_new_dependency))(lexer, filename_kwd) : TPP_EOK)
#define tpp_hooks_get_new_dependency(self)    (self)->TPP_INTERNAL(th_new_dependency)
#define tpp_hooks_set_new_dependency(self, v) (void)((self)->TPP_INTERNAL(th_new_dependency) = (v))
#define tpp_hooks_reset_new_dependency(self)  (void)((self)->TPP_INTERNAL(th_new_dependency) = _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY)
#define _tpp_hooks_init_new_dependency(self)  , (self)->TPP_INTERNAL(th_new_dependency) = _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY
#if TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_NEW_DEPENDENCY)
#define _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY (&TPP_HOOK_NEW_DEPENDENCY)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_NEW_DEPENDENCY NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
#if TPP_HAVE_NEW_DEPENDENCY_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_new_dependency(self, lexer, filename_kwd) \
	TPP_HOOK_NEW_DEPENDENCY(lexer, filename_kwd)
#else /*  */
#define tpp_hooks_call_new_dependency(self, lexer, filename_kwd) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_new_dependency(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

/* Called whenever a file was just pushed onto the `#include`-stack
 * Information about the just-pushed file can be retrieved by examining `tpp_lexer_getfile(self)`
 * This hook can be used by a frontend to implement stuff like GCC's `--trace-includes`.
 * WARNING: *NOT* Called for `tpp_file_subtext_push()` or `tpp_file_pushdummy()` */
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK)
#define tpp_hooks_call_file_pushed(self, lexer) \
	((self)->TPP_INTERNAL(th_file_pushed) ? (*(self)->TPP_INTERNAL(th_file_pushed))(lexer) : TPP_EOK)
#define tpp_hooks_get_file_pushed(self)    (self)->TPP_INTERNAL(th_file_pushed)
#define tpp_hooks_set_file_pushed(self, v) (void)((self)->TPP_INTERNAL(th_file_pushed) = (v))
#define tpp_hooks_reset_file_pushed(self)  (void)((self)->TPP_INTERNAL(th_file_pushed) = _TPP_HOOKS_DEFAULT_FILE_PUSHED)
#define _tpp_hooks_init_file_pushed(self)  , (self)->TPP_INTERNAL(th_file_pushed) = _TPP_HOOKS_DEFAULT_FILE_PUSHED
#if TPP_HAVE_FILE_PUSHED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_FILE_PUSHED)
#define _TPP_HOOKS_DEFAULT_FILE_PUSHED (&TPP_HOOK_FILE_PUSHED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_FILE_PUSHED NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) */
#if TPP_HAVE_FILE_PUSHED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_file_pushed(self, lexer) \
	TPP_HOOK_FILE_PUSHED(lexer)
#else /*  */
#define tpp_hooks_call_file_pushed(self, lexer) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_file_pushed(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) */

/* Called whenever a file is about to be popped off the `#include`-stack
 * Information about the file that's about-to-be popped can be retrieved
 * by examining `tpp_lexer_getfile(self)`. Note that this hook is called
 * during the file-pop *commit* phase (`tpp_lexer_manualpopfile_break_commit()`)
 * but is *NOT* called by `tpp_lexer_manualpopfile_popfile()`.
 * WARNING: *NOT* Called for `tpp_file_subtext_pop()` or `tpp_file_popdummy()` */
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK)
#define tpp_hooks_call_file_popped(self, lexer) \
	((self)->TPP_INTERNAL(th_file_popped) ? (*(self)->TPP_INTERNAL(th_file_popped))(lexer) : (void)0)
#define tpp_hooks_get_file_popped(self)    (self)->TPP_INTERNAL(th_file_popped)
#define tpp_hooks_set_file_popped(self, v) (void)((self)->TPP_INTERNAL(th_file_popped) = (v))
#define tpp_hooks_reset_file_popped(self)  (void)((self)->TPP_INTERNAL(th_file_popped) = _TPP_HOOKS_DEFAULT_FILE_POPPED)
#define _tpp_hooks_init_file_popped(self)  , (self)->TPP_INTERNAL(th_file_popped) = _TPP_HOOKS_DEFAULT_FILE_POPPED
#if TPP_HAVE_FILE_POPPED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_FILE_POPPED)
#define _TPP_HOOKS_DEFAULT_FILE_POPPED (&TPP_HOOK_FILE_POPPED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_FILE_POPPED NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK) */
#if TPP_HAVE_FILE_POPPED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_file_popped(self, lexer) \
	TPP_HOOK_FILE_POPPED(lexer)
#else /*  */
#define tpp_hooks_call_file_popped(self, lexer) (void)0
#endif /* ... */
#define _tpp_hooks_init_file_popped(self) /* nothing */
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
 * @return: TPP_EOK:    Continue handling like usual
 * @return: TPP_ENOENT: Don't attempt to find/open a file. Instead, continue processing
 *                      the file containing the `#include`-directive as though the file
 *                      could not be found, and the `TPP_W_NO_SUCH_FILE` error was being
 *                      suppressed.
 * @return: TPP_E*:     Some other error -- propagate immdediately */
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK)
#define tpp_hooks_call_include_encountered(self, lexer, include_kind) \
	((self)->TPP_INTERNAL(th_include_encountered) ? (*(self)->TPP_INTERNAL(th_include_encountered))(lexer, include_kind) : TPP_EOK)
#define tpp_hooks_get_include_encountered(self)    (self)->TPP_INTERNAL(th_include_encountered)
#define tpp_hooks_set_include_encountered(self, v) (void)((self)->TPP_INTERNAL(th_include_encountered) = (v))
#define tpp_hooks_reset_include_encountered(self)  (void)((self)->TPP_INTERNAL(th_include_encountered) = _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED)
#define _tpp_hooks_init_include_encountered(self)  , (self)->TPP_INTERNAL(th_include_encountered) = _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED
#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_INCLUDE_ENCOUNTERED)
#define _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED (&TPP_HOOK_INCLUDE_ENCOUNTERED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_INCLUDE_ENCOUNTERED NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */
#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_include_encountered(self, lexer, include_kind) \
	TPP_HOOK_INCLUDE_ENCOUNTERED(lexer, include_kind)
#else /*  */
#define tpp_hooks_call_include_encountered(self, lexer, include_kind) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_include_encountered(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */

/* Called when the file specified by a `#include` (or `#include_next`, `#import` or `#embed`)-
 * directive could not be found, this hook may be used to either suppress the error (by returning
 * something other than `TPP_ENOENT`), or log the error to implement something like GCC's `-MG`
 * commandline switch.
 *
 * This hook is called just before `TPP_W_NO_SUCH_FILE` would be emitted, with the lexer's
 * current token still being the `<stdio.h>` or `"file.h"` string, meaning if you want
 * to know what that string says, you can use `tpp_lexer_decode_include_string_cb()` to decode it.
 *
 * @param: include_kind: The kind of directive that this is (one of `TPP_HOOK_INCLUDE_KIND_*`)
 * @return: TPP_EOK:    Suppress the accompanying `TPP_W_NO_SUCH_FILE` error, but continue acting like
 *                      the file could not be found (*DONT* use this hook to manually push a file or
 *                      something like that)
 * @return: TPP_ENOENT: Emit the `TPP_W_NO_SUCH_FILE` error
 * @return: TPP_E*:     Some other error -- propagate immdediately */
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
#define tpp_hooks_call_include_not_found(self, lexer, include_kind) \
	((self)->TPP_INTERNAL(th_include_not_found) ? (*(self)->TPP_INTERNAL(th_include_not_found))(lexer, include_kind) : TPP_ENOENT)
#define tpp_hooks_get_include_not_found(self)    (self)->TPP_INTERNAL(th_include_not_found)
#define tpp_hooks_set_include_not_found(self, v) (void)((self)->TPP_INTERNAL(th_include_not_found) = (v))
#define tpp_hooks_reset_include_not_found(self)  (void)((self)->TPP_INTERNAL(th_include_not_found) = _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND)
#define _tpp_hooks_init_include_not_found(self)  , (self)->TPP_INTERNAL(th_include_not_found) = _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND
#if TPP_HAVE_INCLUDE_NOT_FOUND_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_INCLUDE_NOT_FOUND)
#define _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND (&TPP_HOOK_INCLUDE_NOT_FOUND)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_INCLUDE_NOT_FOUND NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */
#if TPP_HAVE_INCLUDE_NOT_FOUND_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_include_not_found(self, lexer, include_kind) \
	TPP_HOOK_INCLUDE_NOT_FOUND(lexer, include_kind)
#else /*  */
#define tpp_hooks_call_include_not_found(self, lexer, include_kind) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_include_not_found(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */

/* Called whenever a `#define` directive has just been fully
 * parsed (macro was has not yet been registered with keyword).
 *
 * This hook is *ONLY* invoked when `#define` is encountered, or
 * `#pragma pop_macro("foo")` was used to restore a macro's previous
 * definition.
 *
 * Calls to `tpp_lexer_define()` or other related functions will
 * *NOT* invoke this hook. */
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK)
#define tpp_hooks_call_macro_defined(self, lexer, name, macro) \
	((self)->TPP_INTERNAL(th_macro_defined) ? (*(self)->TPP_INTERNAL(th_macro_defined))(lexer, name, macro) : TPP_EOK)
#define tpp_hooks_get_macro_defined(self)    (self)->TPP_INTERNAL(th_macro_defined)
#define tpp_hooks_set_macro_defined(self, v) (void)((self)->TPP_INTERNAL(th_macro_defined) = (v))
#define tpp_hooks_reset_macro_defined(self)  (void)((self)->TPP_INTERNAL(th_macro_defined) = _TPP_HOOKS_DEFAULT_MACRO_DEFINED)
#define _tpp_hooks_init_macro_defined(self)  , (self)->TPP_INTERNAL(th_macro_defined) = _TPP_HOOKS_DEFAULT_MACRO_DEFINED
#if TPP_HAVE_MACRO_DEFINED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_MACRO_DEFINED)
#define _TPP_HOOKS_DEFAULT_MACRO_DEFINED (&TPP_HOOK_MACRO_DEFINED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_MACRO_DEFINED NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK) */
#if TPP_HAVE_MACRO_DEFINED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_macro_defined(self, lexer, name, macro) \
	TPP_HOOK_MACRO_DEFINED(lexer, name, macro)
#else /*  */
#define tpp_hooks_call_macro_defined(self, lexer, name, macro) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_macro_defined(self) /* nothing */
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
 * NOTE: this hook *will* actually also be called by `#pragma push_macro(undef, "foo")` */
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK)
#define tpp_hooks_call_macro_undefined(self, lexer, name) \
	((self)->TPP_INTERNAL(th_macro_undefined) ? (*(self)->TPP_INTERNAL(th_macro_undefined))(lexer, name) : TPP_EOK)
#define tpp_hooks_get_macro_undefined(self)    (self)->TPP_INTERNAL(th_macro_undefined)
#define tpp_hooks_set_macro_undefined(self, v) (void)((self)->TPP_INTERNAL(th_macro_undefined) = (v))
#define tpp_hooks_reset_macro_undefined(self)  (void)((self)->TPP_INTERNAL(th_macro_undefined) = _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED)
#define _tpp_hooks_init_macro_undefined(self)  , (self)->TPP_INTERNAL(th_macro_undefined) = _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED
#if TPP_HAVE_MACRO_UNDEFINED_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_MACRO_UNDEFINED)
#define _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED (&TPP_HOOK_MACRO_UNDEFINED)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_MACRO_UNDEFINED NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK) */
#if TPP_HAVE_MACRO_UNDEFINED_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_macro_undefined(self, lexer, name) \
	TPP_HOOK_MACRO_UNDEFINED(lexer, name)
#else /*  */
#define tpp_hooks_call_macro_undefined(self, lexer, name) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_macro_undefined(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK) */

/* Called to handle `#ident` and `#sccs` directives
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
#if TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK)
#define tpp_hooks_call_ident_sccs(self, lexer, mode, chunk, comment_str, comment_len) \
	((self)->TPP_INTERNAL(th_ident_sccs) ? (*(self)->TPP_INTERNAL(th_ident_sccs))(lexer, mode, chunk, comment_str, comment_len) : TPP_EOK)
#define tpp_hooks_get_ident_sccs(self)    (self)->TPP_INTERNAL(th_ident_sccs)
#define tpp_hooks_set_ident_sccs(self, v) (void)((self)->TPP_INTERNAL(th_ident_sccs) = (v))
#define tpp_hooks_reset_ident_sccs(self)  (void)((self)->TPP_INTERNAL(th_ident_sccs) = _TPP_HOOKS_DEFAULT_IDENT_SCCS)
#define _tpp_hooks_init_ident_sccs(self)  , (self)->TPP_INTERNAL(th_ident_sccs) = _TPP_HOOKS_DEFAULT_IDENT_SCCS
#if TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_IDENT_SCCS)
#define _TPP_HOOKS_DEFAULT_IDENT_SCCS (&TPP_HOOK_IDENT_SCCS)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_IDENT_SCCS NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK) */
#if TPP_HAVE_IDENT_SCCS_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_ident_sccs(self, lexer, mode, chunk, comment_str, comment_len) \
	TPP_HOOK_IDENT_SCCS(lexer, mode, chunk, comment_str, comment_len)
#else /*  */
#define tpp_hooks_call_ident_sccs(self, lexer, mode, chunk, comment_str, comment_len) TPP_EOK
#endif /* ... */
#define _tpp_hooks_init_ident_sccs(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK) */

/* Extra callback invoked by `tpp_lexer_foreach_include_path()` at different
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
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK)
#define tpp_hooks_call_system_include_path(self, lexer, mode, when, cb, arg) \
	((self)->TPP_INTERNAL(th_system_include_path) ? (*(self)->TPP_INTERNAL(th_system_include_path))(lexer, mode, when, cb, arg) : TPP_ENOENT)
#define tpp_hooks_get_system_include_path(self)    (self)->TPP_INTERNAL(th_system_include_path)
#define tpp_hooks_set_system_include_path(self, v) (void)((self)->TPP_INTERNAL(th_system_include_path) = (v))
#define tpp_hooks_reset_system_include_path(self)  (void)((self)->TPP_INTERNAL(th_system_include_path) = _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH)
#define _tpp_hooks_init_system_include_path(self)  , (self)->TPP_INTERNAL(th_system_include_path) = _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH
#if TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_SYSTEM_INCLUDE_PATH)
#define _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH (&TPP_HOOK_SYSTEM_INCLUDE_PATH)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_SYSTEM_INCLUDE_PATH NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */
#if TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_system_include_path(self, lexer, mode, when, cb, arg) \
	TPP_HOOK_SYSTEM_INCLUDE_PATH(lexer, mode, when, cb, arg)
#else /*  */
#define tpp_hooks_call_system_include_path(self, lexer, mode, when, cb, arg) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_system_include_path(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */

/* Extra callback invoked by `tpp_lexer_foreach_embed_path()` at different points
 * during the process of enumerating embed paths. (s.a. `TPP_HOOK_SYSTEM_INCLUDE_PATH`)
 * @param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`: describes the
 *               caller's position in `tpp_lexer_foreach_include_path()`.
 * @return: * :         First non-TPP_ENOENT return value of `cb`
 * @return: TPP_ENOENT: File still not found
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK)
#define tpp_hooks_call_system_embed_path(self, lexer, mode, when, cb, arg) \
	((self)->TPP_INTERNAL(th_system_embed_path) ? (*(self)->TPP_INTERNAL(th_system_embed_path))(lexer, mode, when, cb, arg) : TPP_ENOENT)
#define tpp_hooks_get_system_embed_path(self)    (self)->TPP_INTERNAL(th_system_embed_path)
#define tpp_hooks_set_system_embed_path(self, v) (void)((self)->TPP_INTERNAL(th_system_embed_path) = (v))
#define tpp_hooks_reset_system_embed_path(self)  (void)((self)->TPP_INTERNAL(th_system_embed_path) = _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH)
#define _tpp_hooks_init_system_embed_path(self)  , (self)->TPP_INTERNAL(th_system_embed_path) = _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH
#if TPP_HAVE_SYSTEM_EMBED_PATH_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_SYSTEM_EMBED_PATH)
#define _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH (&TPP_HOOK_SYSTEM_EMBED_PATH)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_SYSTEM_EMBED_PATH NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */
#if TPP_HAVE_SYSTEM_EMBED_PATH_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_system_embed_path(self, lexer, mode, when, cb, arg) \
	TPP_HOOK_SYSTEM_EMBED_PATH(lexer, mode, when, cb, arg)
#else /*  */
#define tpp_hooks_call_system_embed_path(self, lexer, mode, when, cb, arg) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_system_embed_path(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */

/* Called by `tpp_lexer_decodestring()` when an unknown `\`-escape sequence is encountered
 * This hook can be used to define additional, user-defined escape sequences, or any other
 * arbitrary behavior to-be performed when specific escape-sequences are found.
 * On entry, `*p_pos` points at the first (unrecognized) character after the leading `\`, and
 * if the hook was able to parse said escape sequence, it should update `*p_pos` to point after
 * it before returning
 * @param: p_pos:  [in]  Pointer to start of unrecognized `\`-escape sequence
 *                 [out] First character no longer part of `\`-escape sequence (if recognized)
 *                 [out] Unchanged (if not recognized)
 * @param: end:    The of containing string sequence
 * @param: config: Identically-named argument of `tpp_lexer_decodestring()`
 * @return: * :    Sum of positive return values of `data_printer` and `utf8_printer`
 * @return: < 0:   First negative return value of `data_printer` or `utf8_printer`
 * @return: TPP_SSIZE_OFERR(TPP_ENOENT): Escape sequence still not recognized
 *                 (please leave `*p_pos` unchanged in this case). The caller will
 *                 proceed by emitting `TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE` */
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK)
#define tpp_hooks_call_unknown_string_escape(self, lexer, p_pos, end, config) \
	((self)->TPP_INTERNAL(th_unknown_string_escape) ? (*(self)->TPP_INTERNAL(th_unknown_string_escape))(lexer, p_pos, end, config) : TPP_SSIZE_OFERR(TPP_ENOENT))
#define tpp_hooks_get_unknown_string_escape(self)    (self)->TPP_INTERNAL(th_unknown_string_escape)
#define tpp_hooks_set_unknown_string_escape(self, v) (void)((self)->TPP_INTERNAL(th_unknown_string_escape) = (v))
#define tpp_hooks_reset_unknown_string_escape(self)  (void)((self)->TPP_INTERNAL(th_unknown_string_escape) = _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE)
#define _tpp_hooks_init_unknown_string_escape(self)  , (self)->TPP_INTERNAL(th_unknown_string_escape) = _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE
#if TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_UNKNOWN_STRING_ESCAPE)
#define _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE (&TPP_HOOK_UNKNOWN_STRING_ESCAPE)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_UNKNOWN_STRING_ESCAPE NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */
#if TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_unknown_string_escape(self, lexer, p_pos, end, config) \
	TPP_HOOK_UNKNOWN_STRING_ESCAPE(lexer, p_pos, end, config)
#else /*  */
#define tpp_hooks_call_unknown_string_escape(self, lexer, p_pos, end, config) TPP_SSIZE_OFERR(TPP_ENOENT)
#endif /* ... */
#define _tpp_hooks_init_unknown_string_escape(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */

/* Called by `tpp_lexer_warnf()` just before it's about to return `TPP_ELEXERROR`
 * This hook can be used to do additional state changes that may be necessary by the
 * hosting application in order to handle the resulting `TPP_ELEXERROR`
 * @return: TPP_EOK: Have `tpp_lexer_warnf()` still return `TPP_ELEXERROR`
 * @return: * :      Make `tpp_lexer_warnf()` return this instead of `TPP_ELEXERROR` */
#if TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK)
#define tpp_hooks_call_raise_lexerror(self, lexer) \
	((self)->TPP_INTERNAL(th_raise_lexerror) ? (*(self)->TPP_INTERNAL(th_raise_lexerror))(lexer) : TPP_ELEXERROR)
#define tpp_hooks_get_raise_lexerror(self)    (self)->TPP_INTERNAL(th_raise_lexerror)
#define tpp_hooks_set_raise_lexerror(self, v) (void)((self)->TPP_INTERNAL(th_raise_lexerror) = (v))
#define tpp_hooks_reset_raise_lexerror(self)  (void)((self)->TPP_INTERNAL(th_raise_lexerror) = _TPP_HOOKS_DEFAULT_RAISE_LEXERROR)
#define _tpp_hooks_init_raise_lexerror(self)  , (self)->TPP_INTERNAL(th_raise_lexerror) = _TPP_HOOKS_DEFAULT_RAISE_LEXERROR
#if TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_RAISE_LEXERROR)
#define _TPP_HOOKS_DEFAULT_RAISE_LEXERROR (&TPP_HOOK_RAISE_LEXERROR)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_RAISE_LEXERROR NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK) */
#if TPP_HAVE_RAISE_LEXERROR_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_raise_lexerror(self, lexer) \
	TPP_HOOK_RAISE_LEXERROR(lexer)
#else /*  */
#define tpp_hooks_call_raise_lexerror(self, lexer) TPP_ELEXERROR
#endif /* ... */
#define _tpp_hooks_init_raise_lexerror(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK) */

/* Called by `tpp_lexer_yieldraw()` when `TPP_HAVE_SMART_FLOAT_TOKENS` is enabled and
 * a sequence like `1.f` is encountered where the lexer is unsure if the `f` should be
 * part of the float-token (in the form of a float-suffix), or if this is actually be
 * parsed as 3 tokens: `[C_INT:1][DOT:.][f:f]`. For this purpose, this hook is called
 * with `pos` pointing at the `f` (though additional characters thereafter may not be
 * loaded yet, though can be loaded using `tpp_lexer_readchar()`)
 * @return: TPP_EOK:    Pointed-to location actually *does* refer to a float suffix
 * @return: TPP_ENOENT: It's not a float suffix
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK)
#define tpp_hooks_call_isfloatsuffix(self, lexer, pos) \
	((self)->TPP_INTERNAL(th_isfloatsuffix) ? (*(self)->TPP_INTERNAL(th_isfloatsuffix))(lexer, pos) : TPP_ENOENT)
#define tpp_hooks_get_isfloatsuffix(self)    (self)->TPP_INTERNAL(th_isfloatsuffix)
#define tpp_hooks_set_isfloatsuffix(self, v) (void)((self)->TPP_INTERNAL(th_isfloatsuffix) = (v))
#define tpp_hooks_reset_isfloatsuffix(self)  (void)((self)->TPP_INTERNAL(th_isfloatsuffix) = _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX)
#define _tpp_hooks_init_isfloatsuffix(self)  , (self)->TPP_INTERNAL(th_isfloatsuffix) = _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX
#if TPP_HAVE_ISFLOATSUFFIX_HOOK == TPP_HOOK_RT_USER && defined(TPP_HOOK_ISFLOATSUFFIX)
#define _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX (&TPP_HOOK_ISFLOATSUFFIX)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_ISFLOATSUFFIX NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK) */
#if TPP_HAVE_ISFLOATSUFFIX_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_call_isfloatsuffix(self, lexer, pos) \
	TPP_HOOK_ISFLOATSUFFIX(lexer, pos)
#else /*  */
#define tpp_hooks_call_isfloatsuffix(self, lexer, pos) TPP_ENOENT
#endif /* ... */
#define _tpp_hooks_init_isfloatsuffix(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK) */

/* Initialize lexer hooks */
#define tpp_hooks_init(self) \
	(void)((void)0 _tpp_hooks_init_warnprinter(self) \
	       _tpp_hooks_init_warnhandler(self) \
	       _tpp_hooks_init_mesgprinter(self) \
	       _tpp_hooks_init_parseexpr(self) \
	       _tpp_hooks_init_unknown_pragma(self) \
	       _tpp_hooks_init_new_dependency(self) \
	       _tpp_hooks_init_file_pushed(self) \
	       _tpp_hooks_init_file_popped(self) \
	       _tpp_hooks_init_include_encountered(self) \
	       _tpp_hooks_init_include_not_found(self) \
	       _tpp_hooks_init_macro_defined(self) \
	       _tpp_hooks_init_macro_undefined(self) \
	       _tpp_hooks_init_ident_sccs(self) \
	       _tpp_hooks_init_system_include_path(self) \
	       _tpp_hooks_init_system_embed_path(self) \
	       _tpp_hooks_init_unknown_string_escape(self) \
	       _tpp_hooks_init_raise_lexerror(self) \
	       _tpp_hooks_init_isfloatsuffix(self))
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
_tpp_lexer_builtin_warnhandler(struct tpp_lexer *tpp_restrict self,
                               struct tpp_lexer_printf_info *tpp_restrict info,
                               tpp_warning_invokeinfo const *tpp_restrict invokeinfo,
                               tpp_warning_id id, va_list args);
#endif /* TPP_HAVE_BUILTIN_WARNHANDLER_HOOK */

#if TPP_HAVE_BUILTIN_MESGPRINTER_HOOK
TPP_DECL TPP_FORMATPRINTER_DEFINE(_tpp_lexer_builtin_mesgprinter, arg, text, num_bytes);
#endif /* TPP_HAVE_BUILTIN_MESGPRINTER_HOOK */

#if TPP_HAVE_BUILTIN_PARSEEXPR_HOOK
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_lexer_builtin_parseexpr(struct tpp_lexer *tpp_restrict self,
                             tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE_BUILTIN_PARSEEXPR_HOOK */
/************************************************************************/


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_HOOKS_H */
