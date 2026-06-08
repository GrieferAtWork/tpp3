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
#ifndef GUARD_TPP_LEXER_PP_PRAGMA_C
#define GUARD_TPP_LEXER_PP_PRAGMA_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "features.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "macro.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_PRAGMA
/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword
 * @return: TPP_ENOENT: Unknown pragma (warning was already emitted; caller
 *                      should seek until after macro) */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);

#if TPP_HAVE_PRAGMA_PUSH_MACRO
struct tpp_lexer_handle_pushpopmacro_data {
	tpp_lexer *tlhppmd_lexer; /* [1..1] Lexer */
	bool       tlhppmd_push;  /* True if "push_macro", false if "pop_macro" */
};

static tpp_errno TPPCALL
tpp_lexer_handle_pushpopmacro_cb(void *arg, tpp_string *chunk,
                                 tpp_char const *str, tpp_size length) {
	tpp_errno result;
	struct tpp_lexer_handle_pushpopmacro_data *data;
	tpp_keyword const *ro_keyword;
	tpp_keyword *keyword;
	tpp_hash hash = tpp_hashof(str, length);
	(void)chunk;
	data = (struct tpp_lexer_handle_pushpopmacro_data *)arg;

	/* Load keyword */
	ro_keyword = tpp_keywords_newkeyword(&data->tlhppmd_lexer->tl_kwds, str, length, hash);
	if tpp_unlikely(!ro_keyword)
		goto err_nomem;

	/* Make keyword writable */
	keyword = tpp_keywords_copybuiltin(&data->tlhppmd_lexer->tl_kwds, ro_keyword);
	if tpp_unlikely(!keyword)
		goto err_nomem;

	/* Push/pop the macro linked to this keyword. */
	if (data->tlhppmd_push)
		return tpp_keyword_pushmacro(keyword);

	result = tpp_keyword_popmacro(keyword);
	tpp_assert(!TPP_ISERR(result) ||
	           result == TPP_ENOENT);
	if (result == TPP_ENOENT) {
		/* Emit a warning */
#if TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK
		result = tpp_lexer_warnf(data->tlhppmd_lexer, TPP_W_POP_MACRO_EMPTY_STACK,
		                         (unsigned int)length, str);
#else /* TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
		result = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
	}
	return result;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

/* Process a #pragma directive, start at the first token that comes after
 * the leading "#pragma" (i.e.: the first token of the actual directive
 * itself)
 *
 * @return: TPP_EOK:    Success (but there may still be garbage after
 *                      the directive that hasn't been parsed, yet).
 * @return: TPP_ENOENT: Unknown pragma (soft-error; must be handled by caller)
 * @return: TPP_E*:     Error */
TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_token_id tok = token->tt_id;
	switch (tok) {

#if TPP_HAVE_PRAGMA_PUSH_MACRO
	case TPP_KWD_push_macro:
	case TPP_KWD_pop_macro: {
		tpp_errno error;
		struct tpp_lexer_handle_pushpopmacro_data data;
		data.tlhppmd_lexer = self;
		data.tlhppmd_push  = tok == TPP_KWD_push_macro;
		if (!tpp_lexer_getext(self, TPP_EXT_PRAGMA_PUSH_MACRO))
			goto unknown_pragma;
		tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);

		/* Skip leading '(' */
		tok = tpp_lexer_skip_blocking(self, TPP_TOK_OFCHAR('('));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (TPP_TOK_ISSTRING(tok)) {
			/* Parse+process string (using "tpp_lexer_parsestring_cb()") */
			error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_pushpopmacro_cb,
			                                 &data, TPP_LEXER_PARSESTRING_FLAG_STOPONLF);
		} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			error = TPP_EOK;;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
		}
		if (TPP_ISERR(error))
			return error;

		/* Skip trailing ')' */
		tok = tpp_lexer_skip_blocking(self, TPP_TOK_OFCHAR(')'));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}	break;
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_PRAGMA_ONCE
	case TPP_KWD_once: {
		tpp_file const *iofile;
		tpp_keyword *iofile_kwd;
		if (!tpp_lexer_getext(self, TPP_EXT_PRAGMA_ONCE))
			goto unknown_pragma;
		iofile     = tpp_file_getiofile(tpp_lexer_getfile(self));
		iofile_kwd = tpp_file_filename_kwd(iofile);
		if (iofile_kwd) {
			tpp_keyword_misc *misc;
			misc = tpp_keyword_requiremisc(iofile_kwd);
			if tpp_unlikely(!misc)
				return TPP_ENOMEM;
			misc->tkm_flags |= TPP_KEYWORD_FLAG_HDR_ONCE;
		}
#if TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER
#if TPP_HAVE_INCLUDE_STACK
		if (iofile->tf_prev == NULL)
#endif /* TPP_HAVE_INCLUDE_STACK */
		{
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER);
			if (TPP_ISERR(error))
				return error;
		}
#endif /* TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER */
		tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}	break;
#endif /* TPP_HAVE_PRAGMA_ONCE */

#if TPP_HAVE_PRAGMA_DEPRECATED
	/* TODO: #pragma deprecated("foo") */
#endif /* TPP_HAVE_PRAGMA_DEPRECATED */

#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
	/* TODO: #pragma tpp_set_keyword_flags("foo", 0x7f) */
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

#if TPP_HAVE_PRAGMA_EXTENSION
	/* TODO: #pragma extension(...) */
#endif /* TPP_HAVE_PRAGMA_EXTENSION */

#if TPP_HAVE_PRAGMA_EXTENSION_PUSH
	/* TODO: #pragma extension(push) */
#endif /* TPP_HAVE_PRAGMA_EXTENSION_PUSH */

#if TPP_HAVE_PRAGMA_WARNING
	/* TODO: #pragma warning(...) */
#endif /* TPP_HAVE_PRAGMA_WARNING */

#if TPP_HAVE_PRAGMA_WARNING_PUSH
	/* TODO: #pragma warning(push) */
#endif /* TPP_HAVE_PRAGMA_WARNING_PUSH */

#if TPP_HAVE_PRAGMA_TPP_EXEC
	/* TODO: #pragma tpp_exec("...") */
#endif /* TPP_HAVE_PRAGMA_TPP_EXEC */

#if TPP_HAVE_PRAGMA_MESSAGE
	/* TODO: #pragma message("...") */
#endif /* TPP_HAVE_PRAGMA_MESSAGE */

#if TPP_HAVE_PRAGMA_ERROR
	/* TODO: #pragma error("...") */
#endif /* TPP_HAVE_PRAGMA_ERROR */

#if TPP_HAVE_PRAGMA_REGION
	/* TODO: #pragma region,  #pragma endregion */
#endif /* TPP_HAVE_PRAGMA_REGION */

#if TPP_HAVE_PRAGMA_GCC_POISON
	/* TODO: #pragma GCC poison foo */
#endif /* TPP_HAVE_PRAGMA_GCC_POISON */

#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
	/* TODO: #pragma GCC system_header */
#endif /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

#if TPP_HAVE_PRAGMA_TPP_WARNING
	/* TODO: #pragma TPP warning(...)  (same as TPP_HAVE_PRAGMA_WARNING) */
#endif /* TPP_HAVE_PRAGMA_TPP_WARNING */

#if TPP_HAVE_PRAGMA_TPP_EXTENSION
	/* TODO: #pragma TPP extension(...)  (same as TPP_HAVE_PRAGMA_EXTENSION) */
#endif /* TPP_HAVE_PRAGMA_TPP_EXTENSION */

#if TPP_HAVE_PRAGMA_TPP_TPP_EXEC
	/* TODO: #pragma TPP tpp_exec(...)  (same as TPP_HAVE_PRAGMA_TPP_EXEC) */
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_EXEC */

#if TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
	/* TODO: #pragma TPP tpp_set_keyword_flags("foo", 0x7f)  (same as TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */

#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
	/* TODO: #pragma TPP include_path(...) */
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */

	default: goto unknown_pragma;
	}
	return TPP_EOK;
#if TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS
	{
		tpp_errno error;
unknown_pragma:
		error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_PRAGMAS);
		if (!TPP_ISERR(error))
			error = TPP_ENOENT;
		return error;
	}
#else /* TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
unknown_pragma:
	return TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
}
#endif /* TPP_HAVE_PRAGMA */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_PP_PRAGMA_C */
