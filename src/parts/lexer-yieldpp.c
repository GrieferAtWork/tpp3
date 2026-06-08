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
#ifndef GUARD_TPP_LEXER_YIELDPP_C
#define GUARD_TPP_LEXER_YIELDPP_C 1
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

#if TPP_HAVE_TRIGRAPHS && TPP_HAVE_DIGRAPHS
#define tpp_is_start_of_hash(ch) ((ch) == '#' || (ch) == '?' || (ch) == '%')
#elif TPP_HAVE_DIGRAPHS
#define tpp_is_start_of_hash(ch) ((ch) == '#' || (ch) == '%')
#elif TPP_HAVE_TRIGRAPHS
#define tpp_is_start_of_hash(ch) ((ch) == '#' || (ch) == '?')
#else /* ... */
#define tpp_is_start_of_hash(ch) ((ch) == '#')
#endif /* !... */

#if TPP_HAVE_CPP_DIRECTIVES

#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT
#undef tpp_lexer_seek_eol__STYLE_PARAM
#undef tpp_lexer_seek_eol__STYLE_ARG
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
#define tpp_lexer_seek_eol__STYLE_PARAM  , tpp_token_id comment_style
#define tpp_lexer_seek_eol__STYLE_ARG(x) , x
#else /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
#define tpp_lexer_seek_eol__STYLE_PARAM  /* nothing */
#define tpp_lexer_seek_eol__STYLE_ARG(x) /* nothing */
#endif /* !TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */

/* From "./lexer-yieldraw.c" */
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_eol(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos
                   tpp_lexer_seek_eol__STYLE_PARAM);
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT */


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
tpp_lexer_handle_pushpopmacro_cb(void *arg, tpp_char const *str, tpp_size length) {
	tpp_errno result;
	struct tpp_lexer_handle_pushpopmacro_data *data;
	tpp_keyword const *ro_keyword;
	tpp_keyword *keyword;
	tpp_hash hash = tpp_hashof(str, length);
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

/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_pragma_directive(tpp_lexer *tpp_restrict self) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_errno error = tpp_lexer_process_pragma(self);
	if (TPP_ISERR(error)) {
		if (error == TPP_ENOENT)
			goto skip_garbage_without_warning;
		return TPP_TOK_OFERR(error);
	}
	while (TPP_TOK_ISSPACE_OR_COMMENT(token->tt_id)) {
		tpp_token_id tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
	}
	if (TPP_TOK_ISLF_OR_COMMENT(token->tt_id))
		return TPP_TOK_EOF;
	if (token->tt_id == TPP_TOK_EOF)
		return TPP_TOK_EOF;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
	error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE);
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE */
skip_garbage_without_warning:
	for (;;) {
		tpp_token_id tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
		if (TPP_TOK_ISLF_OR_COMMENT(tok))
			break;
	}
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_PRAGMA */

#if TPP_HAVE_CPP_DEFINE
/* Handle a "#define" directive, with "self" pointing at the macro's name-keyword
 * @return: TPP_TOK_ISERR: Error
 * @return: TPP_TOK_EOF: Success; caller should yield the next raw token */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_define_directive(tpp_lexer *tpp_restrict self);
#endif /* TPP_HAVE_CPP_DEFINE */


/* Process a preprocessor directive, with the currently loaded token being the leading '#'
 * Upon successful return (!TPP_TOK_ISERR(return)), the caller will yield another raw token
 * @return: TPP_TOK_ISERR         : Error
 * @return: TPP_TOK_EOF           : Caller should yield the next raw token
 * @return: TPP_TOK_SHELL_COMMENT : Directive was transformed to a shell-comment which the caller should re-emit */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_directive(tpp_lexer *tpp_restrict self) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token_id result;
	tpp_char const *directive_iter;
	tpp_assert(token->tt_id == '#');
	directive_iter = token->tt_end;
	file->tf_pos = token->tt_start; /* Make sure that the start of the directive remains loaded. */

	/* Load token that comes after leading '#' */
again_yield_directive_iter:
	result = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
	switch (result) {

#if TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		/* Skip over multi-line comment tokens to allow code like:
		 * >> #(*
		 * >> comment
		 * >> *)define foo 42 */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE */
	case TPP_TOK_SPACE:
		goto again_yield_directive_iter;

/************************************************************************/
#if TPP_HAVE_CPP_BLANK
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE
	TPP_CASE_TPP_TOK_COMMENT_LINE
		/* line-like comment tokens must be treated as the "blank" directive,
		 * since they effective cap-off the directive via a commented line-feed */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE */
	case TPP_TOK_EOF:
	case TPP_TOK_LF:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_BLANK))
			goto handle_unknown_directive;
		break;
#endif /* TPP_HAVE_CPP_BLANK */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EXCLAIM
	case '!':
		if (!tpp_lexer_getext(self, TPP_EXT_CPP_EXCLAIM))
			goto handle_unknown_directive;
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_EXCLAIM */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIGIT_LINE
	case TPP_TOK_INT:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DIGIT_LINE))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_DIGIT_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_LINE
	case TPP_KWD_line:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_LINE))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT
	{
#if TPP_HAVE_CPP_INCLUDE
		if (0) {
	case TPP_KWD_include:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_INCLUDE))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_INCLUDE */
#if TPP_HAVE_CPP_INCLUDE_NEXT
		if (0) {
	case TPP_KWD_include_next:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_INCLUDE_NEXT))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
#if TPP_HAVE_CPP_IMPORT
		if (0) {
	case TPP_KWD_import:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_IMPORT))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_IMPORT */

		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}
#endif /* TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
	case TPP_KWD_if:
	case TPP_KWD_elif:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO: -Wundef */
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line

	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO: -Wheader-guard */
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line

	case TPP_KWD_else:
	case TPP_KWD_endif:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO: -Wno-endif-labels */
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DEFINE
	case TPP_KWD_define: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DEFINE))
			goto handle_unknown_directive;
		token->tt_end = directive_iter;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (!TPP_TOK_ISKEYWORD(result)) {
#if TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE, "define");
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
			goto seek_end_of_line;
#define WANT_seek_end_of_line
		}
		return tpp_lexer_process_define_directive(self);
	}	break;

	case TPP_KWD_undef: {
		tpp_keyword const *ro_keyword;
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DEFINE))
			goto handle_unknown_directive;
		token->tt_end = directive_iter;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (!TPP_TOK_ISKEYWORD(result)) {
#if TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE, "undef");
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
			goto seek_end_of_line;
#define WANT_seek_end_of_line
		}

		/* Delete keyword definition */
		ro_keyword = tpp_lexer_gettoken(self)->tt_kwd;
		if (ro_keyword->tk_macro) {
			tpp_keyword *keyword = tpp_keywords_copybuiltin(&self->tl_kwds, ro_keyword);
			if tpp_unlikely(!keyword)
				return TPP_TOK_ENOMEM;
			tpp_assert(keyword->tk_macro);
			tpp_macro_decref(keyword->tk_macro);
			keyword->tk_macro = NULL;
		} else
#if TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
		if (tpp_lexer_getkeyworddefined(self, ro_keyword)) {
			/* Builtin keyword... */
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_CANNOT_UNDEF_BUILTIN_MACRO);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		} else
#endif /* TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO */
		{
		}

		/* Seek to next token (which should be a line-feed) */
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (TPP_TOK_ISLF_OR_COMMENT(result))
			break;
		if (result == TPP_TOK_EOF)
			break;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		{
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, "undef");
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}	break;
#endif /* TPP_HAVE_CPP_DEFINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ASSERT
	case TPP_KWD_assert:
	case TPP_KWD_unassert:
		if (!tpp_lexer_getext(self, TPP_EXT_CPP_ASSERT))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_ASSERT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
	{
		tpp_size rel_token_start;
		tpp_size rel_message_start;
		tpp_size rel_message_end;
		tpp_char const *message_start;
		tpp_char const *message_end;
		tpp_size message_size;
		tpp_errno error;
#if TPP_HAVE_CPP_ERROR
		if (0) {
	case TPP_KWD_error:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_ERROR))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
		if (0) {
	case TPP_KWD_warning:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_WARNING))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_WARNING */
		rel_token_start   = tpp_file_ptr2rel(file, token->tt_start);
		rel_message_start = tpp_file_ptr2rel(file, directive_iter);
		error = tpp_lexer_seek_eol(self, &directive_iter tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
		rel_message_end = tpp_file_ptr2rel(file, directive_iter);
		token->tt_start = tpp_file_rel2ptr(file, rel_token_start);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);

		/* Load range of message string. */
		message_start = tpp_file_rel2ptr(file, rel_message_start);
		message_end   = tpp_file_rel2ptr(file, rel_message_end);

		/* Remember that this is where the next token should begin. */
		token->tt_end = directive_iter;

		/* Strip leading/trailing whitespace */
#if TPP_HAVE_UNICODE
		if (tpp_file_isutf8(tpp_lexer_getfile(self))) {
			while (message_start < message_end) {
				tpp_char const *nstart = message_start;
				tpp_unichar uc = tpp_unicode_readutf8(&nstart, message_end);
				if (!tpp_unicode_isspace(uc))
					break;
				message_start = nstart;
			}
			while (message_start < message_end) {
				tpp_char const *nend = message_end;
				tpp_unichar uc = tpp_unicode_readutf8_rev(&nend, message_start);
				if (!tpp_unicode_isspace(uc))
					break;
				message_end = nend;
			}
		} else
#endif /* TPP_HAVE_UNICODE */
		{
			while (message_start < message_end && tpp_ascii_isspace(*message_start))
				++message_start;
			while (message_start < message_end && tpp_ascii_isspace(message_end[-1]))
				--message_end;
		}

		/* Emit error/warning message */
		message_size = (tpp_size)(message_end - message_start);
		switch (result) {
		case TPP_KWD_error:
#if TPP_HAVE_TPP_W_ERROR
			error = tpp_lexer_warnf(self, TPP_W_ERROR, (unsigned int)message_size, message_start);
#endif /* TPP_HAVE_TPP_W_ERROR */
			break;
		case TPP_KWD_warning:
#if TPP_HAVE_TPP_W_WARNING
			error = tpp_lexer_warnf(self, TPP_W_WARNING, (unsigned int)message_size, message_start);
#endif /* TPP_HAVE_TPP_W_WARNING */
			break;
		default: tpp_unreachable();
		}
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		break;
	}
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IDENT_SCCS
	case TPP_KWD_ident:
	case TPP_KWD_sccs:
		if (!tpp_lexer_getext(self, TPP_EXT_CPP_IDENT_SCCS))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_IDENT_SCCS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_PRAGMA
	case TPP_KWD_pragma: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_PRAGMA))
			goto handle_unknown_directive;
		token->tt_end = directive_iter;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		return tpp_lexer_process_pragma_directive(self);
	}	break;
#endif /* TPP_HAVE_CPP_PRAGMA */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EMBED
	case TPP_KWD_embed: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_EMBED))
			goto handle_unknown_directive;
		/* TODO: #embed  (https://en.cppreference.com/c/preprocessor/embed) */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}	break;
#endif /* TPP_HAVE_CPP_EMBED */
/************************************************************************/



	default:
		if (TPP_TOK_ISERR(result))
			return result;
handle_unknown_directive:
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SHELL_COMMENT)
#if TPP_HAVE_DIGRAPHS
		    && *file->tf_pos != '%' /* Digraph "%:" must never become a shell comment (since
		                             * "%:" is a **TOKEN** substitution, but not a **CHARACTER**
		                             * substitution like "??=" is) */
#endif /* TPP_HAVE_DIGRAPHS */
		    ) {
			/* Seek next end-of-line character (rather than next end-of-line token)
			 * The difference here comes into play when considering that multi-line
			 * comment tokens can disguise line-feed tokens, but since we're to
			 * treat the #-line as a comment on its own, that means that we're
			 * already *within* a comment (semantically speaking), and comment
			 * nesting isn't something that's allowed! */
			tpp_errno error;
			tpp_char const *eol;
			/* "file->tf_pos" was saved as the start of the '#' */
			tpp_assert(tpp_is_start_of_hash(*file->tf_pos));
			token->tt_start = eol = file->tf_pos;
			error = tpp_lexer_seek_eol(self, &eol tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SHELL_COMMENT));
			if (TPP_ISERR(error)) {
				token->tt_start = file->tf_pos;
				token->tt_end = file->tf_pos + 1;
#if TPP_HAVE_TRIGRAPHS
				if (*file->tf_pos == '?') {
					token->tt_end += 2;
				} else
#endif /* TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_DIGRAPHS
				if (*file->tf_pos == '%') {
					token->tt_end += 1;
				} else
#endif /* TPP_HAVE_DIGRAPHS */
				{
				}
				token->tt_id = (tpp_token_id)'#';
				self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
				return TPP_TOK_OFERR(error);
			}
			token->tt_end = eol;
			token->tt_id = TPP_TOK_SHELL_COMMENT;

			/* Tell caller to re-emit what we thought was a directive as a shell comment */
			return TPP_TOK_SHELL_COMMENT;
		} else
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		{
#if defined(WANT_seek_end_of_line) || TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0
#if TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_DIRECTIVE);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE */
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0 */
	
			/* Seek until we hit LF or EOF. Caller has disabled "autopopfile",
			 * so this'll always stay within the file currently being processed. */
#ifdef WANT_seek_end_of_line
#undef WANT_seek_end_of_line
seek_end_of_line:
#endif /* WANT_seek_end_of_line */
			while (!TPP_TOK_ISLF_OR_COMMENT(result) && result != TPP_TOK_EOF) {
				result = tpp_lexer_yieldraw_blocking(self);
				if (TPP_TOK_ISERR(result))
					break;
			}
#endif /* WANT_seek_end_of_line || TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0 */
		}
		break;
	}
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_CPP_DIRECTIVES */



/* Wrapper around `tpp_lexer_yieldraw()' that filters certain tokens (based on
 * configured features), and implements handling for preprocessor directives,
 * like "#define", "#include", etc:
 * - TPP_TOK_LF:      Filtered based on `TPP_HAVE_TPP_TOK_LF' / `TPP_FEAT_TPP_TOK_LF'
 * - TPP_TOK_SPACE:   Filtered based on `TPP_HAVE_TPP_TOK_SPACE' / `TPP_FEAT_TPP_TOK_SPACE'
 * - TPP_TOK_COMMENT: Filtered based on `TPP_HAVE_TPP_TOK_COMMENT' / `TPP_FEAT_TPP_TOK_COMMENT'
 *
 * @return: * :                  The newly read token (after accounting for preprocessor directives)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldraw(self);
	switch (result) {


/************************************************************************/
#if TPP_HAVE_TPP_TOK_SPACE <= 0 /* Always, or conditionally disabled */
	case TPP_TOK_SPACE:
#if TPP_HAVE_TPP_TOK_SPACE < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SPACE))
			break; /* Enabled */
#endif /* TPP_HAVE_TPP_TOK_SPACE < 0 */
		goto again;
#elif TPP_HAVE_CPP_DIRECTIVES
	case TPP_TOK_SPACE:
		break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_COMMENTLIKE
	_TPP_CASE_TPP_TOK_SHELL_COMMENT
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT && TPP_HAVE_CPP_DIRECTIVES
		if (!(self->tl_state & TPP_LEXER_STATE_FLAG_NODIRECTIVES) &&
			tpp_lexer_getfeat(self, TPP_FEAT_CPP_DIRECTIVES)) {
			tpp_token *const token = tpp_lexer_gettoken(self);

			/* Must re-parse comment as a preprocessor directive instead! */
			token->tt_id  = (tpp_token_id)'#';
			tpp_assert(tpp_is_start_of_hash(*token->tt_start));
			token->tt_end = token->tt_start + 1;
#if TPP_HAVE_TRIGRAPHS
			if (*token->tt_start == '?') {
				token->tt_end += 2;
			} else
#endif /* TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_DIGRAPHS
			if (*token->tt_start == '%') {
				token->tt_end += 1;
			} else
#endif /* TPP_HAVE_DIGRAPHS */
			{
			}

			tpp_lexer_autopopfile_pushoff(self);
			result = tpp_lexer_process_directive(self);
			tpp_lexer_autopopfile_pop(self);
			if (TPP_TOK_ISERR(result))
				break;
			if (result == TPP_TOK_EOF)
				goto again;

			/* Deal with case where PP-directive wasn't recognized,
			 * and should thus be emitted as shell-comment token. */
			tpp_assert(result == TPP_TOK_SHELL_COMMENT);

			/* Fallthru to regular maybe-emit-comment code below... */
		}
		TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT && TPP_HAVE_CPP_DIRECTIVES */
#if TPP_HAVE_TPP_TOK_COMMENT <= 0 /* Always, or conditionally disabled */
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_ASM_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
		TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES */
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
#if TPP_HAVE_TPP_TOK_COMMENT < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COMMENT))
			break; /* Comments are enabled -> emit to caller */
#endif /* TPP_HAVE_TPP_TOK_COMMENT < 0 */
		goto again;
#elif TPP_HAVE_CPP_DIRECTIVES
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_ASM_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		break;
#endif /* ... */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIRECTIVES || TPP_HAVE_TPP_TOK_LF <= 0 /* Always, or conditionally disabled */
	case TPP_TOK_LF: {
#if TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#if TPP_HAVE_TPP_TOK_LF <= 0 /* Always, or conditionally disabled */
#if TPP_HAVE_TPP_TOK_LF < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LF))
			break; /* Enabled */
#endif /* TPP_HAVE_TPP_TOK_LF < 0 */
		goto again;
#else /* TPP_HAVE_TPP_TOK_LF <= 0 */
		break;
#endif /* TPP_HAVE_TPP_TOK_LF > 0 */
	}
#endif /* TPP_HAVE_CPP_DIRECTIVES || TPP_HAVE_TPP_TOK_LF <= 0 */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIRECTIVES
	case '#':
		if (self->tl_state & TPP_LEXER_STATE_FLAG_NODIRECTIVES)
			break; /* Not allowed here... */
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DIRECTIVES))
			break; /* Directives are disabled. */
		tpp_lexer_autopopfile_pushoff(self);
		result = tpp_lexer_process_directive(self);
		tpp_lexer_autopopfile_pop(self);
		if (TPP_TOK_ISERR(result))
			break;
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (result != TPP_TOK_EOF) {
			/* Emit as a shell-comment (if enabled). Otherwise, check next raw token. */
			tpp_assert(result == TPP_TOK_SHELL_COMMENT);
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COMMENT))
				break;
		}
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		goto again;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
/************************************************************************/



/************************************************************************/
	default:
#if TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen something that will prevent CPP directives */
		self->tl_state |= TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
		break;
/************************************************************************/
	}
	return result;
}


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELDPP_C */
