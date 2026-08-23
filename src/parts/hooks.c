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
#ifndef GUARD_TPP_HOOKS_C
#define GUARD_TPP_HOOKS_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "hooks.h"
#include "lexer.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* Ensure that `TPP_HOOK_*` helper macros work */
TPP_STATIC_ASSERT(TPP_HOOK_WITHCOOKIE(TPP_HOOK_RT_USER) == TPP_HOOK_RT_USER_C);
TPP_STATIC_ASSERT(TPP_HOOK_WITHCOOKIE(TPP_HOOK_RT_BUILTIN) == TPP_HOOK_RT_BUILTIN_C);
TPP_STATIC_ASSERT(TPP_HOOK_WITHCOOKIE(TPP_HOOK_RT_NOOP) == TPP_HOOK_RT_NOOP_C);
TPP_STATIC_ASSERT(TPP_HOOK_WITHCOOKIE(TPP_HOOK_RT_MANY) == TPP_HOOK_RT_MANY_C);
TPP_STATIC_ASSERT(TPP_HOOK_WITHCOOKIE(TPP_HOOK_RT_USER_C) == TPP_HOOK_RT_USER_C);
TPP_STATIC_ASSERT(TPP_HOOK_WITHCOOKIE(TPP_HOOK_RT_BUILTIN_C) == TPP_HOOK_RT_BUILTIN_C);
TPP_STATIC_ASSERT(TPP_HOOK_WITHCOOKIE(TPP_HOOK_RT_NOOP_C) == TPP_HOOK_RT_NOOP_C);
TPP_STATIC_ASSERT(TPP_HOOK_WITHCOOKIE(TPP_HOOK_RT_MANY_C) == TPP_HOOK_RT_MANY_C);

TPP_STATIC_ASSERT(TPP_HOOK_WITHOUTCOOKIE(TPP_HOOK_RT_USER) == TPP_HOOK_RT_USER);
TPP_STATIC_ASSERT(TPP_HOOK_WITHOUTCOOKIE(TPP_HOOK_RT_BUILTIN) == TPP_HOOK_RT_BUILTIN);
TPP_STATIC_ASSERT(TPP_HOOK_WITHOUTCOOKIE(TPP_HOOK_RT_NOOP) == TPP_HOOK_RT_NOOP);
TPP_STATIC_ASSERT(TPP_HOOK_WITHOUTCOOKIE(TPP_HOOK_RT_MANY) == TPP_HOOK_RT_MANY);
TPP_STATIC_ASSERT(TPP_HOOK_WITHOUTCOOKIE(TPP_HOOK_RT_USER_C) == TPP_HOOK_RT_USER);
TPP_STATIC_ASSERT(TPP_HOOK_WITHOUTCOOKIE(TPP_HOOK_RT_BUILTIN_C) == TPP_HOOK_RT_BUILTIN);
TPP_STATIC_ASSERT(TPP_HOOK_WITHOUTCOOKIE(TPP_HOOK_RT_NOOP_C) == TPP_HOOK_RT_NOOP);
TPP_STATIC_ASSERT(TPP_HOOK_WITHOUTCOOKIE(TPP_HOOK_RT_MANY_C) == TPP_HOOK_RT_MANY);

TPP_STATIC_ASSERT(!TPP_HOOK_USESBUILTIN(TPP_HOOK_DISABLED));
TPP_STATIC_ASSERT(!TPP_HOOK_USESBUILTIN(TPP_HOOK_CONST_USER));
TPP_STATIC_ASSERT(TPP_HOOK_USESBUILTIN(TPP_HOOK_CONST_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_USESBUILTIN(TPP_HOOK_RT_USER));
TPP_STATIC_ASSERT(TPP_HOOK_USESBUILTIN(TPP_HOOK_RT_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_USESBUILTIN(TPP_HOOK_RT_NOOP));
TPP_STATIC_ASSERT(!TPP_HOOK_USESBUILTIN(TPP_HOOK_RT_MANY));
TPP_STATIC_ASSERT(!TPP_HOOK_USESBUILTIN(TPP_HOOK_RT_USER_C));
TPP_STATIC_ASSERT(TPP_HOOK_USESBUILTIN(TPP_HOOK_RT_BUILTIN_C));
TPP_STATIC_ASSERT(!TPP_HOOK_USESBUILTIN(TPP_HOOK_RT_NOOP_C));
TPP_STATIC_ASSERT(!TPP_HOOK_USESBUILTIN(TPP_HOOK_RT_MANY_C));

TPP_STATIC_ASSERT(!TPP_HOOK_USESUSER(TPP_HOOK_DISABLED));
TPP_STATIC_ASSERT(TPP_HOOK_USESUSER(TPP_HOOK_CONST_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_USESUSER(TPP_HOOK_CONST_BUILTIN));
TPP_STATIC_ASSERT(TPP_HOOK_USESUSER(TPP_HOOK_RT_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_USESUSER(TPP_HOOK_RT_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_USESUSER(TPP_HOOK_RT_NOOP));
TPP_STATIC_ASSERT(TPP_HOOK_USESUSER(TPP_HOOK_RT_MANY));
TPP_STATIC_ASSERT(TPP_HOOK_USESUSER(TPP_HOOK_RT_USER_C));
TPP_STATIC_ASSERT(!TPP_HOOK_USESUSER(TPP_HOOK_RT_BUILTIN_C));
TPP_STATIC_ASSERT(!TPP_HOOK_USESUSER(TPP_HOOK_RT_NOOP_C));
TPP_STATIC_ASSERT(TPP_HOOK_USESUSER(TPP_HOOK_RT_MANY_C));

TPP_STATIC_ASSERT(!TPP_HOOK_ISMANY(TPP_HOOK_DISABLED));
TPP_STATIC_ASSERT(!TPP_HOOK_ISMANY(TPP_HOOK_CONST_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_ISMANY(TPP_HOOK_CONST_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_ISMANY(TPP_HOOK_RT_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_ISMANY(TPP_HOOK_RT_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_ISMANY(TPP_HOOK_RT_NOOP));
TPP_STATIC_ASSERT(TPP_HOOK_ISMANY(TPP_HOOK_RT_MANY));
TPP_STATIC_ASSERT(!TPP_HOOK_ISMANY(TPP_HOOK_RT_USER_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISMANY(TPP_HOOK_RT_BUILTIN_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISMANY(TPP_HOOK_RT_NOOP_C));
TPP_STATIC_ASSERT(TPP_HOOK_ISMANY(TPP_HOOK_RT_MANY_C));

TPP_STATIC_ASSERT(TPP_HOOK_ISCONST(TPP_HOOK_DISABLED));
TPP_STATIC_ASSERT(TPP_HOOK_ISCONST(TPP_HOOK_CONST_USER));
TPP_STATIC_ASSERT(TPP_HOOK_ISCONST(TPP_HOOK_CONST_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_ISCONST(TPP_HOOK_RT_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_ISCONST(TPP_HOOK_RT_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_ISCONST(TPP_HOOK_RT_NOOP));
TPP_STATIC_ASSERT(!TPP_HOOK_ISCONST(TPP_HOOK_RT_MANY));
TPP_STATIC_ASSERT(!TPP_HOOK_ISCONST(TPP_HOOK_RT_USER_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISCONST(TPP_HOOK_RT_BUILTIN_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISCONST(TPP_HOOK_RT_NOOP_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISCONST(TPP_HOOK_RT_MANY_C));

TPP_STATIC_ASSERT(!TPP_HOOK_ISRT(TPP_HOOK_DISABLED));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRT(TPP_HOOK_CONST_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRT(TPP_HOOK_CONST_BUILTIN));
TPP_STATIC_ASSERT(TPP_HOOK_ISRT(TPP_HOOK_RT_USER));
TPP_STATIC_ASSERT(TPP_HOOK_ISRT(TPP_HOOK_RT_BUILTIN));
TPP_STATIC_ASSERT(TPP_HOOK_ISRT(TPP_HOOK_RT_NOOP));
TPP_STATIC_ASSERT(TPP_HOOK_ISRT(TPP_HOOK_RT_MANY));
TPP_STATIC_ASSERT(TPP_HOOK_ISRT(TPP_HOOK_RT_USER_C));
TPP_STATIC_ASSERT(TPP_HOOK_ISRT(TPP_HOOK_RT_BUILTIN_C));
TPP_STATIC_ASSERT(TPP_HOOK_ISRT(TPP_HOOK_RT_NOOP_C));
TPP_STATIC_ASSERT(TPP_HOOK_ISRT(TPP_HOOK_RT_MANY_C));

TPP_STATIC_ASSERT(TPP_HOOK_ISRTUSER(TPP_HOOK_RT_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTUSER(TPP_HOOK_RT_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTUSER(TPP_HOOK_RT_NOOP));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTUSER(TPP_HOOK_RT_MANY));
TPP_STATIC_ASSERT(TPP_HOOK_ISRTUSER(TPP_HOOK_RT_USER_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTUSER(TPP_HOOK_RT_BUILTIN_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTUSER(TPP_HOOK_RT_NOOP_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTUSER(TPP_HOOK_RT_MANY_C));

TPP_STATIC_ASSERT(!TPP_HOOK_ISRTBULITIN(TPP_HOOK_RT_USER));
TPP_STATIC_ASSERT(TPP_HOOK_ISRTBULITIN(TPP_HOOK_RT_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTBULITIN(TPP_HOOK_RT_NOOP));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTBULITIN(TPP_HOOK_RT_MANY));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTBULITIN(TPP_HOOK_RT_USER_C));
TPP_STATIC_ASSERT(TPP_HOOK_ISRTBULITIN(TPP_HOOK_RT_BUILTIN_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTBULITIN(TPP_HOOK_RT_NOOP_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTBULITIN(TPP_HOOK_RT_MANY_C));

TPP_STATIC_ASSERT(!TPP_HOOK_ISRTNOOP(TPP_HOOK_RT_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTNOOP(TPP_HOOK_RT_BUILTIN));
TPP_STATIC_ASSERT(TPP_HOOK_ISRTNOOP(TPP_HOOK_RT_NOOP));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTNOOP(TPP_HOOK_RT_MANY));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTNOOP(TPP_HOOK_RT_USER_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTNOOP(TPP_HOOK_RT_BUILTIN_C));
TPP_STATIC_ASSERT(TPP_HOOK_ISRTNOOP(TPP_HOOK_RT_NOOP_C));
TPP_STATIC_ASSERT(!TPP_HOOK_ISRTNOOP(TPP_HOOK_RT_MANY_C));

TPP_STATIC_ASSERT(!TPP_HOOK_HASCOOKIE(TPP_HOOK_DISABLED));
TPP_STATIC_ASSERT(!TPP_HOOK_HASCOOKIE(TPP_HOOK_CONST_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_HASCOOKIE(TPP_HOOK_CONST_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_HASCOOKIE(TPP_HOOK_RT_USER));
TPP_STATIC_ASSERT(!TPP_HOOK_HASCOOKIE(TPP_HOOK_RT_BUILTIN));
TPP_STATIC_ASSERT(!TPP_HOOK_HASCOOKIE(TPP_HOOK_RT_NOOP));
TPP_STATIC_ASSERT(!TPP_HOOK_HASCOOKIE(TPP_HOOK_RT_MANY));
TPP_STATIC_ASSERT(TPP_HOOK_HASCOOKIE(TPP_HOOK_RT_USER_C));
TPP_STATIC_ASSERT(TPP_HOOK_HASCOOKIE(TPP_HOOK_RT_BUILTIN_C));
TPP_STATIC_ASSERT(TPP_HOOK_HASCOOKIE(TPP_HOOK_RT_NOOP_C));
TPP_STATIC_ASSERT(TPP_HOOK_HASCOOKIE(TPP_HOOK_RT_MANY_C));


#if !TPP_HOST_NO_SYSTEM_INCLUDES && (TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK)
TPP_DECL_END
#include <stdio.h>
TPP_DECL_BEGIN
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES && (TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK) */

#if TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK
TPP_FORMATPRINTER_IMPL(_tpp_lexer_builtin_warn_or_mesg_printer, arg, text, num_bytes) {
	FILE *fp = stderr;
	(void)arg;
	fwrite(text, sizeof(tpp_char), num_bytes, fp);
	return ferror(fp) ? TPP_SSIZE_OFERR(TPP_EIO) : 0;
}
#endif /* TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK */



/*[[[deemon
import HOOKS from .config;

for (local doc, name,
     default_TPP_HAVE_FOO_HOOK,
     builtin_FOO_HOOK,
     prototypePrefix,
     prototypeSuffix,
     prototypeArgs,
     disabled_RETURN_VALUE: HOOKS) {
	if (builtin_FOO_HOOK)
		continue;
	local returnType = prototypePrefix.rstrip("*").rstrip().rsstrip("TPPCALL").rstrip().rstrip("(").rstrip();
	print("#if TPP_HOOK_ISMANY(TPP_HAVE_", name, "_HOOK)");
	print("/" "* ", doc.strip().replace("\n", "\n * ").rstriplines(), " *" "/");
	print("TPP_IMPL "),;
	if (returnType != "void")
		print("TPP_WUNUSED "),;
	print("TPP_NONNULL((1)) ", returnType, " TPPCALL");
	print("_tpp_hooks_call_", name.lower(), "(struct tpp_lexer *tpp_restrict lexer"),;
	local extraParams = prototypeSuffix.lstrip(")").lstrip().lstrip("(").rstrip(")").strip()
		.rereplace(r"\bCOOKIE\s+cookie\s*,\s*", "")
		.rereplace(r"\s*,\s*COOKIE\s+cookie\b", "")
		.rereplace(r"\bCOOKIE\s+cookie\b", "");
	if (extraParams)
		print(", ", extraParams),;
	print(") {");
	print("	tpp_hook_list_", name.lower(), " const *const list = lexer->tl_hooks.th_", name.lower(), ";");
	print("	if (list) {");
	print("		tpp_size count = list->thl_size;");
	print("		while (count) {");
	print("			tpp_hook_list_entry_", name.lower(), " const *const entry = &list->thl_elem[--count];");
	local callEntryExprNormal = f"(*entry->thle_cb)({", ".join(
		for (local arg: prototypeArgs) arg != "cookie" ? arg : "lexer")})";
	local callEntryExprCookie = f"(*entry->thle_cb)({", ".join(
		for (local arg: prototypeArgs) arg != "cookie" ? arg : "entry->thle_cookie")})";
	if (returnType == "void") {
		if ("cookie" in prototypeArgs) {
			print("#if TPP_HOOK_HASCOOKIE(TPP_HAVE_", name, "_HOOK)");
			print("				", callEntryExprCookie, ";");
			print("#else /" "* TPP_HOOK_HASCOOKIE(TPP_HAVE_", name, "_HOOK) *" "/");
		}
		print("				", callEntryExprNormal, ";");
		if ("cookie" in prototypeArgs)
			print("#endif /" "* !TPP_HOOK_HASCOOKIE(TPP_HAVE_", name, "_HOOK) *" "/");
	} else {
		if ("cookie" in prototypeArgs) {
			print("#if TPP_HOOK_HASCOOKIE(TPP_HAVE_", name, "_HOOK)");
			print("				", returnType, " const result = ", callEntryExprCookie, ";");
			print("#else /" "* TPP_HOOK_HASCOOKIE(TPP_HAVE_", name, "_HOOK) *" "/");
		}
		print("				", returnType, " const result = ", callEntryExprNormal, ";");
		if ("cookie" in prototypeArgs)
			print("#endif /" "* !TPP_HOOK_HASCOOKIE(TPP_HAVE_", name, "_HOOK) *" "/");
		if (disabled_RETURN_VALUE == "TPP_EOK") {
			print("				if (TPP_ISERR(result))");
		} else {
			print("				if (result != ", disabled_RETURN_VALUE, ")");
		}
		print("					return result;");
	}
	print("		}");
	print("	}");
	if ("cookie" !in prototypeArgs)
		print("	(void)lexer;");
	print("#ifdef TPP_HOOK_", name);
	print("	return TPP_HOOK_", name, "(", ", ".join(
		for (local arg: prototypeArgs) arg != "cookie" ? arg : "lexer"), ");");
	print("#else /" "* TPP_HOOK_", name, "*" "/");
	if ("cookie" in prototypeArgs)
		print("	(void)lexer;");
	print("	return ", disabled_RETURN_VALUE, ";");
	print("#endif /" "* !TPP_HOOK_", name, "*" "/");
	print("}");
	print("#endif /" "* TPP_HOOK_ISMANY(TPP_HAVE_", name, "_HOOK) *" "/");
	print;
	print;
}
]]]*/
#if TPP_HOOK_ISMANY(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
/* Called whenever a `#pragma` is encountered that is not recognized.
 * When called, the lexer is set-up to point at the first token after the `#pragma`.
 * @return: TPP_EOK:      Pragma has been handled
 * @return: TPP_ENOENT:   Pragma is still unknown, and a warning should be emitted
 * @return: TPP_EIO:      I/O error
 * @return: TPP_ENOMEM:   Out of memory
 * @return: TPP_EUSER(*): User-defined error */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_unknown_pragma(struct tpp_lexer *tpp_restrict lexer) {
	tpp_hook_list_unknown_pragma const *const list = lexer->tl_hooks.th_unknown_pragma;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_unknown_pragma const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */
				if (result != TPP_ENOENT)
					return result;
		}
	}
#ifdef TPP_HOOK_UNKNOWN_PRAGMA
	return TPP_HOOK_UNKNOWN_PRAGMA(lexer);
#else /* TPP_HOOK_UNKNOWN_PRAGMA*/
	(void)lexer;
	return TPP_ENOENT;
#endif /* !TPP_HOOK_UNKNOWN_PRAGMA*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_NEW_DEPENDENCY_HOOK)
/* Called whenever some file is `#include`-ed for the first time
 * @param: filename_kwd: Then `tpp_keyword` used to describe the file's name. The actual
 *                       filename can be queried as `tpp_keyword_getcstr(filename_kwd)`
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_new_dependency(struct tpp_lexer *tpp_restrict lexer, tpp_keyword *filename_kwd) {
	tpp_hook_list_new_dependency const *const list = lexer->tl_hooks.th_new_dependency;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_new_dependency const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_NEW_DEPENDENCY_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie, filename_kwd);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer, filename_kwd);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
				if (TPP_ISERR(result))
					return result;
		}
	}
#ifdef TPP_HOOK_NEW_DEPENDENCY
	return TPP_HOOK_NEW_DEPENDENCY(lexer, filename_kwd);
#else /* TPP_HOOK_NEW_DEPENDENCY*/
	(void)lexer;
	return TPP_EOK;
#endif /* !TPP_HOOK_NEW_DEPENDENCY*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_NEW_DEPENDENCY_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_FILE_PUSHED_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_file_pushed(struct tpp_lexer *tpp_restrict lexer) {
	tpp_hook_list_file_pushed const *const list = lexer->tl_hooks.th_file_pushed;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_file_pushed const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_PUSHED_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_PUSHED_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_PUSHED_HOOK) */
				if (TPP_ISERR(result))
					return result;
		}
	}
#ifdef TPP_HOOK_FILE_PUSHED
	return TPP_HOOK_FILE_PUSHED(lexer);
#else /* TPP_HOOK_FILE_PUSHED*/
	(void)lexer;
	return TPP_EOK;
#endif /* !TPP_HOOK_FILE_PUSHED*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_FILE_PUSHED_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_FILE_POPPED_HOOK)
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
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
_tpp_hooks_call_file_popped(struct tpp_lexer *tpp_restrict lexer) {
	tpp_hook_list_file_popped const *const list = lexer->tl_hooks.th_file_popped;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_file_popped const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_POPPED_HOOK)
				(*entry->thle_cb)(entry->thle_cookie);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_POPPED_HOOK) */
				(*entry->thle_cb)(lexer);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_POPPED_HOOK) */
		}
	}
#ifdef TPP_HOOK_FILE_POPPED
	return TPP_HOOK_FILE_POPPED(lexer);
#else /* TPP_HOOK_FILE_POPPED*/
	(void)lexer;
	return (void)0;
#endif /* !TPP_HOOK_FILE_POPPED*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_FILE_POPPED_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_include_encountered(struct tpp_lexer *tpp_restrict lexer, tpp_hook_include_kind include_kind) {
	tpp_hook_list_include_encountered const *const list = lexer->tl_hooks.th_include_encountered;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_include_encountered const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie, include_kind);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer, include_kind);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */
				if (TPP_ISERR(result))
					return result;
		}
	}
#ifdef TPP_HOOK_INCLUDE_ENCOUNTERED
	return TPP_HOOK_INCLUDE_ENCOUNTERED(lexer, include_kind);
#else /* TPP_HOOK_INCLUDE_ENCOUNTERED*/
	(void)lexer;
	return TPP_EOK;
#endif /* !TPP_HOOK_INCLUDE_ENCOUNTERED*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_include_not_found(struct tpp_lexer *tpp_restrict lexer, tpp_hook_include_kind include_kind) {
	tpp_hook_list_include_not_found const *const list = lexer->tl_hooks.th_include_not_found;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_include_not_found const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie, include_kind);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer, include_kind);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */
				if (result != TPP_ENOENT)
					return result;
		}
	}
#ifdef TPP_HOOK_INCLUDE_NOT_FOUND
	return TPP_HOOK_INCLUDE_NOT_FOUND(lexer, include_kind);
#else /* TPP_HOOK_INCLUDE_NOT_FOUND*/
	(void)lexer;
	return TPP_ENOENT;
#endif /* !TPP_HOOK_INCLUDE_NOT_FOUND*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_MACRO_DEFINED_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_macro_defined(struct tpp_lexer *tpp_restrict lexer, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro) {
	tpp_hook_list_macro_defined const *const list = lexer->tl_hooks.th_macro_defined;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_macro_defined const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_DEFINED_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie, name, macro);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_DEFINED_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer, name, macro);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_DEFINED_HOOK) */
				if (TPP_ISERR(result))
					return result;
		}
	}
#ifdef TPP_HOOK_MACRO_DEFINED
	return TPP_HOOK_MACRO_DEFINED(lexer, name, macro);
#else /* TPP_HOOK_MACRO_DEFINED*/
	(void)lexer;
	return TPP_EOK;
#endif /* !TPP_HOOK_MACRO_DEFINED*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_MACRO_DEFINED_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_MACRO_UNDEFINED_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_macro_undefined(struct tpp_lexer *tpp_restrict lexer, tpp_keyword *tpp_restrict name) {
	tpp_hook_list_macro_undefined const *const list = lexer->tl_hooks.th_macro_undefined;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_macro_undefined const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_UNDEFINED_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie, name);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_UNDEFINED_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer, name);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_UNDEFINED_HOOK) */
				if (TPP_ISERR(result))
					return result;
		}
	}
#ifdef TPP_HOOK_MACRO_UNDEFINED
	return TPP_HOOK_MACRO_UNDEFINED(lexer, name);
#else /* TPP_HOOK_MACRO_UNDEFINED*/
	(void)lexer;
	return TPP_EOK;
#endif /* !TPP_HOOK_MACRO_UNDEFINED*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_MACRO_UNDEFINED_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_IDENT_SCCS_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_ident_sccs(struct tpp_lexer *tpp_restrict lexer, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len) {
	tpp_hook_list_ident_sccs const *const list = lexer->tl_hooks.th_ident_sccs;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_ident_sccs const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_IDENT_SCCS_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie, mode, chunk, comment_str, comment_len);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_IDENT_SCCS_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer, mode, chunk, comment_str, comment_len);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_IDENT_SCCS_HOOK) */
				if (TPP_ISERR(result))
					return result;
		}
	}
#ifdef TPP_HOOK_IDENT_SCCS
	return TPP_HOOK_IDENT_SCCS(lexer, mode, chunk, comment_str, comment_len);
#else /* TPP_HOOK_IDENT_SCCS*/
	(void)lexer;
	return TPP_EOK;
#endif /* !TPP_HOOK_IDENT_SCCS*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_IDENT_SCCS_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_system_include_path(struct tpp_lexer *tpp_restrict lexer, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg) {
	tpp_hook_list_system_include_path const *const list = lexer->tl_hooks.th_system_include_path;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_system_include_path const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie, mode, when, cb, arg);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer, mode, when, cb, arg);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */
				if (result != TPP_ENOENT)
					return result;
		}
	}
#ifdef TPP_HOOK_SYSTEM_INCLUDE_PATH
	return TPP_HOOK_SYSTEM_INCLUDE_PATH(lexer, mode, when, cb, arg);
#else /* TPP_HOOK_SYSTEM_INCLUDE_PATH*/
	(void)lexer;
	return TPP_ENOENT;
#endif /* !TPP_HOOK_SYSTEM_INCLUDE_PATH*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_system_embed_path(struct tpp_lexer *tpp_restrict lexer, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg) {
	tpp_hook_list_system_embed_path const *const list = lexer->tl_hooks.th_system_embed_path;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_system_embed_path const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie, mode, when, cb, arg);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer, mode, when, cb, arg);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */
				if (result != TPP_ENOENT)
					return result;
		}
	}
#ifdef TPP_HOOK_SYSTEM_EMBED_PATH
	return TPP_HOOK_SYSTEM_EMBED_PATH(lexer, mode, when, cb, arg);
#else /* TPP_HOOK_SYSTEM_EMBED_PATH*/
	(void)lexer;
	return TPP_ENOENT;
#endif /* !TPP_HOOK_SYSTEM_EMBED_PATH*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
_tpp_hooks_call_unknown_string_escape(struct tpp_lexer *tpp_restrict lexer, tpp_char const **tpp_restrict p_pos, tpp_char const *end, tpp_lexer_decodestring_config const *tpp_restrict config) {
	tpp_hook_list_unknown_string_escape const *const list = lexer->tl_hooks.th_unknown_string_escape;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_unknown_string_escape const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK)
				tpp_ssize const result = (*entry->thle_cb)(entry->thle_cookie, p_pos, end, config);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */
				tpp_ssize const result = (*entry->thle_cb)(lexer, p_pos, end, config);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */
				if (result != TPP_SSIZE_OFERR(TPP_ENOENT))
					return result;
		}
	}
#ifdef TPP_HOOK_UNKNOWN_STRING_ESCAPE
	return TPP_HOOK_UNKNOWN_STRING_ESCAPE(lexer, p_pos, end, config);
#else /* TPP_HOOK_UNKNOWN_STRING_ESCAPE*/
	(void)lexer;
	return TPP_SSIZE_OFERR(TPP_ENOENT);
#endif /* !TPP_HOOK_UNKNOWN_STRING_ESCAPE*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_RAISE_LEXERROR_HOOK)
/* Called by `tpp_lexer_warnf()` just before it's about to return `TPP_ELEXERROR`
 * This hook can be used to do additional state changes that may be necessary by the
 * hosting application in order to handle the resulting `TPP_ELEXERROR`
 * @return: TPP_EOK:       Have `tpp_lexer_warnf()` return `TPP_ELEXERROR`
 * @return: TPP_ELEXERROR: Same as `TPP_EOK`
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_EUSER(*):  User-defined error */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_raise_lexerror(struct tpp_lexer *tpp_restrict lexer) {
	tpp_hook_list_raise_lexerror const *const list = lexer->tl_hooks.th_raise_lexerror;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_raise_lexerror const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_RAISE_LEXERROR_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_RAISE_LEXERROR_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_RAISE_LEXERROR_HOOK) */
				if (result != TPP_ELEXERROR)
					return result;
		}
	}
#ifdef TPP_HOOK_RAISE_LEXERROR
	return TPP_HOOK_RAISE_LEXERROR(lexer);
#else /* TPP_HOOK_RAISE_LEXERROR*/
	(void)lexer;
	return TPP_ELEXERROR;
#endif /* !TPP_HOOK_RAISE_LEXERROR*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_RAISE_LEXERROR_HOOK) */


#if TPP_HOOK_ISMANY(TPP_HAVE_ISFLOATSUFFIX_HOOK)
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_hooks_call_isfloatsuffix(struct tpp_lexer *tpp_restrict lexer, tpp_char const *pos) {
	tpp_hook_list_isfloatsuffix const *const list = lexer->tl_hooks.th_isfloatsuffix;
	if (list) {
		tpp_size count = list->thl_size;
		while (count) {
			tpp_hook_list_entry_isfloatsuffix const *const entry = &list->thl_elem[--count];
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_ISFLOATSUFFIX_HOOK)
				tpp_errno const result = (*entry->thle_cb)(entry->thle_cookie, pos);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_ISFLOATSUFFIX_HOOK) */
				tpp_errno const result = (*entry->thle_cb)(lexer, pos);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_ISFLOATSUFFIX_HOOK) */
				if (result != TPP_ENOENT)
					return result;
		}
	}
#ifdef TPP_HOOK_ISFLOATSUFFIX
	return TPP_HOOK_ISFLOATSUFFIX(lexer, pos);
#else /* TPP_HOOK_ISFLOATSUFFIX*/
	(void)lexer;
	return TPP_ENOENT;
#endif /* !TPP_HOOK_ISFLOATSUFFIX*/
}
#endif /* TPP_HOOK_ISMANY(TPP_HAVE_ISFLOATSUFFIX_HOOK) */
/*[[[end]]]*/


/************************************************************************/
/* Hook-list API                                                        */
/************************************************************************/
#if TPP_HAVE_HOOKLIST2
/* @return: TPP_EOK:    Success / already registered
 * @return: TPP_ENOMEM: Out-of-memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_hooklist2_add(tpp_hooklist2 **tpp_restrict p_self,
                   void (*cb)(void), void *arg) {
	tpp_hooklist2 *new_list;
	tpp_hooklist2 *old_list = *p_self;
	tpp_size old_size = 0;
	tpp_size new_size;
	if (old_list) {
		tpp_size index = old_size = old_list->thl_size;
		while (index) {
			tpp_hooklist2_entry *ent = &old_list->thl_elem[--index];
			if (ent->thle_cb == cb && ent->thle_cookie == arg) {
				/* Already registered -> move to back (so it gets called first) */
				tpp_size num_after = (old_size - index) - 1;
				tpp_memmovedown(ent, ent + 1, num_after * sizeof(tpp_hooklist2_entry));
				ent += num_after;
				ent->thle_cb     = cb;
				ent->thle_cookie = arg;
				return TPP_EOK;
			}
		}
	}
	new_size = old_size + 1;
	new_list = (tpp_hooklist2 *)tpp_realloc(old_list,
	                                        tpp_offsetof(tpp_hooklist2, thl_elem) +
	                                        (new_size * sizeof(tpp_hooklist2_entry)));
	if tpp_unlikely(!new_list)
		return TPP_ENOMEM;
	new_list->thl_size = new_size;
	new_list->thl_elem[old_size].thle_cb     = cb;
	new_list->thl_elem[old_size].thle_cookie = arg;
	*p_self = new_list;
	return TPP_EOK;
}

TPP_IMPL TPP_PURECALL TPP_WUNUSED bool TPPCALL
_tpp_hooklist2_has(tpp_hooklist2 const *self,
                   void (*cb)(void), void *arg) {
	if (self) {
		tpp_size index = self->thl_size;
		while (index) {
			tpp_hooklist2_entry const *ent = &self->thl_elem[--index];
			if (ent->thle_cb == cb && ent->thle_cookie == arg)
				return true;
		}
	}
	return false;
}

TPP_IMPL TPP_NONNULL((1, 2)) bool TPPCALL
_tpp_hooklist2_del(tpp_hooklist2 **tpp_restrict p_self,
                   void (*cb)(void), void *arg) {
	tpp_hooklist2 *self = *p_self;
	if (self) {
		tpp_size index = self->thl_size;
		while (index) {
			tpp_hooklist2_entry *ent = &self->thl_elem[--index];
			if (ent->thle_cb == cb && ent->thle_cookie == arg) {
				/* Found it! -> remove it */
				if (self->thl_size <= 1) {
					*p_self = NULL;
					tpp_free(self);
				} else {
					tpp_size num_after = (--self->thl_size - index);
					tpp_memmovedown(ent, ent + 1, num_after * sizeof(tpp_hooklist2_entry));
					/* Release unused memory... */
					self = (tpp_hooklist2 *)tpp_tryrealloc(self,
					                                       self->thl_size *
					                                       sizeof(tpp_hooklist2_entry));
					if (self)
						*p_self = self;
				}
				return true;
			}
		}
	}
	return false;
}
#endif /* TPP_HAVE_HOOKLIST2 */

#if TPP_HAVE_HOOKLIST1
/* @return: TPP_EOK:    Success / already registered
 * @return: TPP_ENOMEM: Out-of-memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_hooklist1_add(tpp_hooklist1 **tpp_restrict p_self, void (*cb)(void)) {
	tpp_hooklist1 *new_list;
	tpp_hooklist1 *old_list = *p_self;
	tpp_size old_size = 0;
	tpp_size new_size;
	if (old_list) {
		tpp_size index = old_size = old_list->thl_size;
		while (index) {
			tpp_hooklist1_entry *ent = &old_list->thl_elem[--index];
			if (ent->thle_cb == cb) {
				/* Already registered -> move to back (so it gets called first) */
				tpp_size num_after = (old_size - index) - 1;
				tpp_memmovedown(ent, ent + 1, num_after * sizeof(tpp_hooklist1_entry));
				ent += num_after;
				ent->thle_cb = cb;
				return TPP_EOK;
			}
		}
	}
	new_size = old_size + 1;
	new_list = (tpp_hooklist1 *)tpp_realloc(old_list,
	                                        tpp_offsetof(tpp_hooklist1, thl_elem) +
	                                        (new_size * sizeof(tpp_hooklist1_entry)));
	if tpp_unlikely(!new_list)
		return TPP_ENOMEM;
	new_list->thl_size = new_size;
	new_list->thl_elem[old_size].thle_cb = cb;
	*p_self = new_list;
	return TPP_EOK;
}

TPP_IMPL TPP_PURECALL TPP_WUNUSED bool TPPCALL
_tpp_hooklist1_has(tpp_hooklist1 const *self, void (*cb)(void)) {
	if (self) {
		tpp_size index = self->thl_size;
		while (index) {
			tpp_hooklist1_entry const *ent = &self->thl_elem[--index];
			if (ent->thle_cb == cb)
				return true;
		}
	}
	return false;
}

TPP_IMPL TPP_NONNULL((1, 2)) bool TPPCALL
_tpp_hooklist1_del(tpp_hooklist1 **tpp_restrict p_self, void (*cb)(void)) {
	tpp_hooklist1 *self = *p_self;
	if (self) {
		tpp_size index = self->thl_size;
		while (index) {
			tpp_hooklist1_entry *ent = &self->thl_elem[--index];
			if (ent->thle_cb == cb) {
				/* Found it! -> remove it */
				if (self->thl_size <= 1) {
					*p_self = NULL;
					tpp_free(self);
				} else {
					tpp_size num_after = (--self->thl_size - index);
					tpp_memmovedown(ent, ent + 1, num_after * sizeof(tpp_hooklist1_entry));
					/* Release unused memory... */
					self = (tpp_hooklist1 *)tpp_tryrealloc(self,
					                                       self->thl_size *
					                                       sizeof(tpp_hooklist1_entry));
					if (self)
						*p_self = self;
				}
				return true;
			}
		}
	}
	return false;
}
#endif /* TPP_HAVE_HOOKLIST1 */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_HOOKS_C */
