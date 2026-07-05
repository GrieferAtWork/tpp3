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
#ifndef GUARD_TPP_HOOKS_H
#define GUARD_TPP_HOOKS_H 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "expr.h"
#include "keyword.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/*[[[deemon
import HOOKS from .config;

print("#undef TPP_HAVE_HOOKS");
print("#if (", " || \\\n     ".join(
	for (local h: HOOKS)
		f"TPP_HOOK_ISRT(TPP_HAVE_{h[1]}_HOOK)"
), ")");
print("#define TPP_HAVE_HOOKS 1");
print("#else /" "* ... *" "/");
print("#define TPP_HAVE_HOOKS 0");
print("#endif /" "* !... *" "/");
print;
print("#if TPP_HAVE_HOOKS");
print("struct tpp_lexer;");
print("typedef struct tpp_hooks {");
function fixProto(x) -> x
	.rereplace(r"\btpp_lexer\b", "struct tpp_lexer")
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
	print("	 * ", doc.strip().replace("\n", "\n\t * "), " *" "/");
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
	print("/" "* ", doc.strip().replace("\n", "\n * "), " *" "/");
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
print("	(void)(0 "),;
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
#if (TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) || \
     TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) || \
     TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) || \
     TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK))
#define TPP_HAVE_HOOKS 1
#else /* ... */
#define TPP_HAVE_HOOKS 0
#endif /* !... */

#if TPP_HAVE_HOOKS
struct tpp_lexer;
typedef struct tpp_hooks {
	/* >> tpp_formatprinter th_warnprinter;
	 * Called by `tpp_lexer_warnf()' to print warning messages
	 * @param: arg: The current lexer (tpp_lexer *) */
#if TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK)
#if TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP
	tpp_formatprinter TPP_INTERNAL(th_warnprinter); /* [1..1] */
#else /* TPP_HAVE_WARNPRINTER_HOOK != TPP_HOOK_RT_NOOP */
	tpp_formatprinter TPP_INTERNAL(th_warnprinter); /* [0..1] */
#endif /* TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_RT_NOOP */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */

	/* >> tpp_errno (TPPCALL *th_parseexpr)(struct tpp_lexer *tpp_restrict self, tpp_expr_value *tpp_restrict result);
	 * User-defined callback for parsing "#if"-style expressions
	 * - This callback is invoked in a context where "self" points
	 *   before the expression's first token (meaning that this
	 *   callback is responsible to do the initial yield using
	 *   whatever method it wants to use).
	 * - When it is known that the expression has finite length,
	 *   as in: it has to end before EOF, or at the next unmatched
	 *   ')'-token, the caller will have configured the lexer's
	 *   current EOF accordingly (and disabled file-popping)
	 * - When this function returns an error, the caller will rewind
	 *   back to the start of the expression (or even further, if
	 *   applicable; meaning this callback doesn't need to concern
	 *   itself with rollback)
	 * 
	 * @return: TPP_EOK:         Success (*result was initialized)
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
	 * Called whenever a #pragma is encountered that is not recognized.
	 * When called, the lexer is set-up to point at the first token after the #pragma.
	 * @return: TPP_EOK:    Pragma has been handled
	 * @return: TPP_ENOENT: Pragma is still unknown, and a warning should be emitted
	 * @return: TPP_EIO:    I/O error
	 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_unknown_pragma))(struct tpp_lexer *tpp_restrict self); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */

	/* >> tpp_errno (TPPCALL *th_new_dependency)(struct tpp_lexer *tpp_restrict self, tpp_keyword *filename_kwd);
	 * Called whenever some file is #include-ed for the first time
	 * @param: filename_kwd: Then 'tpp_keyword' used to describe the file's name. The actual
	 *                       filename can be queried as `tpp_keyword_getkwdcstr(filename_kwd)'. */
#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
	tpp_errno (TPPCALL *TPP_INTERNAL(th_new_dependency))(struct tpp_lexer *tpp_restrict self, tpp_keyword *filename_kwd); /* [0..1] */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
} tpp_hooks;
#endif /* TPP_HAVE_HOOKS */

/* Called by `tpp_lexer_warnf()' to print warning messages
 * @param: arg: The current lexer (tpp_lexer *) */
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
#define _TPP_HOOKS_DEFAULT_WARNPRINTER (&_tpp_lexer_builtin_warnprinter)
#else /* ... */
#define _TPP_HOOKS_DEFAULT_WARNPRINTER NULL
#endif /* !... */
#else /* TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */
#if TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_CONST_USER
#define tpp_hooks_get_warnprinter(self) (&TPP_HOOK_WARNPRINTER)
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) \
	TPP_HOOK_WARNPRINTER(lexer, text, num_bytes)
#elif TPP_HAVE_WARNPRINTER_HOOK == TPP_HOOK_CONST_BUILTIN
#define tpp_hooks_get_warnprinter(self) (&_tpp_lexer_builtin_warnprinter)
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) \
	_tpp_lexer_builtin_warnprinter(lexer, text, num_bytes)
#else /*  */
#define tpp_hooks_call_warnprinter(self, lexer, text, num_bytes) 0
#endif /* ... */
#define _tpp_hooks_init_warnprinter(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */

/* User-defined callback for parsing "#if"-style expressions
 * - This callback is invoked in a context where "self" points
 *   before the expression's first token (meaning that this
 *   callback is responsible to do the initial yield using
 *   whatever method it wants to use).
 * - When it is known that the expression has finite length,
 *   as in: it has to end before EOF, or at the next unmatched
 *   ')'-token, the caller will have configured the lexer's
 *   current EOF accordingly (and disabled file-popping)
 * - When this function returns an error, the caller will rewind
 *   back to the start of the expression (or even further, if
 *   applicable; meaning this callback doesn't need to concern
 *   itself with rollback)
 * 
 * @return: TPP_EOK:         Success (*result was initialized)
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
	((self)->TPP_INTERNAL(th_parseexpr) ? (*(self)->TPP_INTERNAL(th_parseexpr))(lexer, result) : 0)
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
#define tpp_hooks_call_parseexpr(self, lexer, result) 0
#endif /* ... */
#define _tpp_hooks_init_parseexpr(self) /* nothing */
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) */

/* Called whenever a #pragma is encountered that is not recognized.
 * When called, the lexer is set-up to point at the first token after the #pragma.
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

/* Called whenever some file is #include-ed for the first time
 * @param: filename_kwd: Then 'tpp_keyword' used to describe the file's name. The actual
 *                       filename can be queried as `tpp_keyword_getkwdcstr(filename_kwd)'. */
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

/* Initialize lexer hooks */
#define tpp_hooks_init(self) \
	(void)(0 _tpp_hooks_init_warnprinter(self) \
	       _tpp_hooks_init_parseexpr(self) \
	       _tpp_hooks_init_unknown_pragma(self) \
	       _tpp_hooks_init_new_dependency(self))
/*[[[end]]]*/



/************************************************************************/
/* Builtin hooks...                                                     */
/************************************************************************/
#if TPP_HAVE_BUILTIN_WARNPRINTER_HOOK
TPP_DECL TPP_FORMATPRINTER_DEFINE(_tpp_lexer_builtin_warnprinter, arg, text, num_bytes);
#endif /* TPP_HAVE_BUILTIN_WARNPRINTER_HOOK */

#if TPP_HAVE_BUILTIN_PARSEEXPR_HOOK
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_lexer_builtin_parseexpr(struct tpp_lexer *tpp_restrict self,
                             tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE_BUILTIN_PARSEEXPR_HOOK */
/************************************************************************/


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_HOOKS_H */
