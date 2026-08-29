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
#include "expr.h"
#include "extensions.h"
#include "features.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "macro.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_DIRECTIVES

/*[[[tpp-end]]]*/ /* --- Defined in "lexer-yieldraw.c", which was already included */
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TOK_SHELL_COMMENT || TPP_HAVE_CPP_EMBED || TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE
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
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TOK_SHELL_COMMENT || TPP_HAVE_CPP_EMBED || TPP_HAVE_CPP_DIGIT_LINE || TPP_HAVE_CPP_LINE */
/*[[[tpp-begin]]]*/


#undef TPP_HAVE_TPP_LEXER_YIELDRAW_EOL
#define TPP_HAVE_TPP_LEXER_YIELDRAW_EOL \
	(TPP_HAVE_CPP_DEFINE || TPP_HAVE_CPP_IF_ELSE_ENDIF || TPP_HAVE_CPP_ASSERT)

/************************************************************************/
#if TPP_HAVE_TPP_LEXER_YIELDRAW_EOL
/* Skip until EOL and return TPP_TOK_EOF */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_eol(tpp_lexer *tpp_restrict self) {
	tpp_token_id result = tpp_lexer_gettok(self);
	while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
		result = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(result))
			return result;
	}
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_TPP_LEXER_YIELDRAW_EOL */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_PRAGMA
/*[[[tpp-end]]]*/ /* --- Defined in "lexer-pp-pragma.c", which was already included */
/* Process a `#pragma` directive, start at the first token that comes after
 * the leading `#pragma` (i.e.: the first token of the actual directive
 * itself)
 *
 * @return: TPP_EOK:    Success (but there may still be garbage after
 *                      the directive that hasn't been parsed, yet).
 * @return: TPP_ENOENT: Unknown pragma (soft-error; caller should not emit
 *                      `TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE`)
 * @return: TPP_E*:     Error */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);
/*[[[tpp-begin]]]*/

/* Process a pragma directive, starting after the `TPP_KWD_pragma` keyword */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_pragma_directive(tpp_lexer *tpp_restrict self) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token_id tok;
	tpp_errno error;
	tpp_char const *eol_start;
	tpp_char const *eol_end;
	tpp_assert(tpp_lexer_gettok(self) == TPP_KWD_pragma);
	do {
		tok = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;
	eol_start = token->tt_start;
	eol_end   = token->tt_end;
	if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		tpp_token_id first_token_id = token->tt_id;
		struct tpp_keyword const *first_token_kwd = token->tt_kwd;
		tpp_size first_token_len = tpp_token_getlen(token);
		token->tt_end = token->tt_start;
		tpp_assert(first_token_id == tok);
		/* Seek until EOL (so we can set a parsing limit for the pragma handler) */
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, &eol_end);
			if (TPP_TOK_ISERR(tok)) {
				token->tt_end = eol_end;
				return tok;
			}
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
		/* Restore first token of #pragma directive */
		eol_start       = token->tt_start;
		token->tt_id    = first_token_id;
		token->tt_kwd   = first_token_kwd;
		token->tt_start = token->tt_end;
		token->tt_end += first_token_len;
	}

	/* Handle the pragma, but in a context where the file can't be read beyond EOL */
	tpp_file_push_eof_and_ifdef(file);
	tpp_file_seteof(file, eol_start);
	error = tpp_lexer_process_pragma(self);
	if (error == TPP_ENOENT) {
		error = TPP_EOK;
	} else
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
	if (!TPP_ISERR(error)) {
		tok = tpp_lexer_gettok(self);
		while (TPP_TOK_ISSPACE_OR_COMMENT(tok))
			tok = tpp_lexer_yieldraw(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
		} else if (tok != TPP_TOK_EOF) {
			error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE);
		}
	} else
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE */
	{
	}

	/* Cleanup files pushed by the expression */
#if TPP_HAVE_INCLUDE_STACK
	tpp_lexer_popallfiles(self);
#endif /* TPP_HAVE_INCLUDE_STACK */

	tpp_file_pop_eof_and_ifdef(file);
	file->tf_pos = eol_end; /* Continue parsing after EOL (comment) */
	return TPP_TOK_OFERR_OR_EOF(error);
}
#endif /* TPP_HAVE_PRAGMA */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_handle_error_directive(tpp_lexer *tpp_restrict self,
                                 tpp_char const *directive_iter) {
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_size rel_token_start;
	tpp_size rel_message_start;
	tpp_size rel_message_end;
	tpp_char const *message_start;
	tpp_char const *message_end;
	tpp_size message_size;
	tpp_errno error;
	rel_token_start   = tpp_file_ptr2rel(file, token->tt_start);
	rel_message_start = tpp_file_ptr2rel(file, directive_iter);
	error = tpp_lexer_seek_eol(self, &directive_iter tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
	rel_message_end = tpp_file_ptr2rel(file, directive_iter);
	token->tt_start = tpp_file_rel2ptr(file, rel_token_start);

	/* Load range of message string. */
	message_start = tpp_file_rel2ptr(file, rel_message_start);
	message_end   = tpp_file_rel2ptr(file, rel_message_end);

	/* Remember that this is where the next token should begin. */
	token->tt_end = directive_iter;
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);

	/* Strip leading/trailing whitespace */
	message_start = tpp_preparse_skipspace_fwd(self, message_start, message_end);
	message_end   = tpp_preparse_skipspace_bck(self, message_start, message_end);

	/* Emit error/warning message */
	message_size = (tpp_size)(message_end - message_start);
	switch (token->tt_id) {
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
	return TPP_TOK_OFERR_OR_EOF(error);
}
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DEFINE
/*[[[tpp-end]]]*/ /* --- Defined in "lexer-pp-define.c", which was already included */
/* Handle a `#define` directive, with `self` pointing at the macro's name-keyword
 * @return: TPP_TOK_ISERR: Error
 * @return: TPP_TOK_EOF: Success; caller should yield the next raw token */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_define_directive(tpp_lexer *tpp_restrict self);
/*[[[tpp-begin]]]*/

static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_define_directive(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
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
		return tpp_lexer_yieldraw_eol(self);
	}
	return tpp_lexer_process_define_directive(self);
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_undef_directive(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
	do {
		result = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
	if (TPP_TOK_ISERR(result))
		return result;
	if (!TPP_TOK_ISKEYWORD(result)) {
#if TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
		tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE, "undef");
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
	} else {
		/* Delete keyword definition */
		tpp_keyword const *ro_keyword = tpp_lexer_gettokenkwd(self);
		tpp_keyword *keyword = tpp_lexer_kwds_copybuiltin(self, ro_keyword);
		if tpp_unlikely(!keyword)
			return TPP_TOK_ENOMEM;

		/* Invoke #undef-hook */
#if TPP_HAVE_MACRO_UNDEFINED_HOOK
		{
			tpp_errno hook_error;
			hook_error = tpp_lexer_callhook_macro_undefined(self, keyword);
			if (TPP_ISERR(hook_error))
				return TPP_TOK_OFERR(hook_error);
		}
#endif /* TPP_HAVE_MACRO_UNDEFINED_HOOK */

		/* Actually #undef the keyword */
		tpp_keyword_undef(keyword);

		/* Seek to next token (which should be a line-feed) */
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
			return TPP_TOK_EOF;
		} else {
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, "undef");
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	}
	return tpp_lexer_yieldraw_eol(self);
}
#endif /* TPP_HAVE_CPP_DEFINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ASSERT
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_assert_directive(tpp_lexer *tpp_restrict self, tpp_token_id mode) {
	tpp_token_id result;
	tpp_keyword const *mode_kwd = tpp_lexer_gettokenkwd(self);
	tpp_assert(mode == TPP_KWD_assert || mode == TPP_KWD_unassert);
	(void)mode_kwd;
	do {
		result = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
	if (TPP_TOK_ISERR(result))
		return result;
	if (!TPP_TOK_ISKEYWORD(result)) {
#if TPP_HAVE_TPP_W_EXPECTED_ASSERTION_KEY_IN_DIRECTIVE
		tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_ASSERTION_KEY_IN_DIRECTIVE,
		                                  tpp_keyword_getcstr(mode_kwd));
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_ASSERTION_KEY_IN_DIRECTIVE */
	} else {
		/* Delete keyword definition */
		tpp_keyword const *ro_keyword = tpp_lexer_gettoken(self)->tt_kwd;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (mode == TPP_KWD_assert) {
			result = tpp_lexer_require(self, TPP_TOK_OFCHAR('('));
			if (TPP_TOK_ISERR(result))
				return result;
		}
		if (result == '(') {
			/* Assertion with specific value */
again_yield_and_handle_after_lparen:
			do {
				result = tpp_lexer_yieldraw_blocking(self);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
			if (TPP_TOK_ISERR(result))
				return result;
			if (TPP_TOK_ISKEYWORD(result)) {
				tpp_keyword const *value = tpp_lexer_gettokenkwd(self);
				tpp_keyword *keyword = tpp_keywords_copybuiltin(&self->tl_kwds, ro_keyword);
				if tpp_unlikely(!keyword)
					return TPP_TOK_ENOMEM;
				if (mode == TPP_KWD_assert) {
					tpp_errno error = tpp_keyword_addassert(keyword, value);
					if (TPP_ISERR(error))
						return TPP_TOK_OFERR(error);
				} else {
					(void)tpp_keyword_unassert(keyword, value);
				}
				result = tpp_lexer_yieldraw_blocking(self);
				if (TPP_TOK_ISERR(result))
					return result;
			} else {
#if TPP_HAVE_TPP_W_EXPECTED_ASSERTION_VALUE_IN_DIRECTIVE
				tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_ASSERTION_VALUE_IN_DIRECTIVE,
				                                  tpp_keyword_getcstr(mode_kwd),
				                                  tpp_keyword_getcstr(ro_keyword));
				if (TPP_ISERR(error))
					return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_ASSERTION_VALUE_IN_DIRECTIVE */
			}
			if (result == ',')
				goto again_yield_and_handle_after_lparen;
			result = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
			if (result == ')')
				result = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(result))
				return result;
		} else if (mode == TPP_KWD_unassert) {
#if 1 /* This is actually OK (but don't tell the public API users) */
			tpp_keyword_unassertall(ro_keyword);
#else
			tpp_keyword *keyword = tpp_keywords_copybuiltin(&self->tl_kwds, ro_keyword);
			if tpp_unlikely(!keyword)
				return TPP_TOK_ENOMEM;
			tpp_keyword_unassertall(keyword);
#endif
		}

		/* Seek to next token (which should be a line-feed) */
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		while (TPP_TOK_ISSPACE_OR_COMMENT(result))
			result = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(result))
			return result;
		if (TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
			return TPP_TOK_EOF;
		} else {
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE,
			                                  tpp_keyword_getcstr(mode_kwd));
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	}
	return tpp_lexer_yieldraw_eol(self);
}
#endif /* TPP_HAVE_CPP_ASSERT */
/************************************************************************/



/************************************************************************/
/* Delete `tpp_keyword_misc::tkm_file_guard` for the current file if appropriate
 * - Must be called just before calling `tpp_ifdef_stack_append()`, to deal with
 *   the case of a file having multiple top-level #if-blocks (in which case the
 *   file can't have a `#ifndef`-style `#include`-guard)
 * - Must also be called when returning a token from `tpp_lexer_yieldpp()`
 */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
static TPP_NONNULL((1)) void TPPCALL
tpp_file_maybe_delete_include_guard_keyword(tpp_file *tpp_restrict self) {
	if (tpp_ifdef_stack_isempty(tpp_file_getifdef(self)) &&
	    self->tf_kind == TPP_FILE_KIND_IO &&
#if TPP_HAVE_FILE_NOKWD
	    !(self->tf_flags & TPP_FILE_FLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
	    self->tf_data.td_io.tff_name != NULL) {
		tpp_keyword *kwd;
		tpp_keyword_misc *misc;
		kwd = (tpp_keyword *)((char const *)self->tf_data.td_io.tff_name -
		                      tpp_offsetof(tpp_keyword, tk_kwd));
		misc = kwd->tk_misc;
		if (misc && misc->tkm_file_guard &&
		    !(misc->tkm_flags & TPP_KEYWORD_FLAG_HDR_GUARD_VALID))
			misc->tkm_file_guard = NULL;
	}
}
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#define tpp_file_maybe_delete_include_guard_keyword(self) (void)0
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
/* Call with the current token loaded as `if` or `elif`
 * @param: p_directive_start: [out] On success (`TPP_EOK` or `TPP_ENOENT`), set
 *                                  to the start of the `if` or `elif` keyword
 * @return: TPP_EOK:    Directive evaluates to `true`
 * @return: TPP_ENOENT: Directive evaluates to `false`
 * @return: * :         Error */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_if_directive(tpp_lexer *tpp_restrict self,
                             tpp_char const **tpp_restrict p_directive_start) {
	tpp_errno result;
	tpp_token_id tok;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *trailing_lf_start;
	tpp_char const *trailing_lf_end;
	tpp_char const *directive_iter;
	tpp_expr_value expr_value;
	tpp_size directive_keyword_len;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	char const *const directive_name = tpp_lexer_gettokenkwdcstr(self);
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	directive_iter = file->tf_pos;
	file->tf_pos = file->tf_tpos; /* Retain start of `if` / `elif` keyword */
	directive_keyword_len = (tpp_size)(directive_iter - file->tf_pos);

	/* Seek end-of-line */
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
		if (TPP_TOK_ISERR(tok)) {
			tpp_assert(TPP_TOK_ASERR(tok) != TPP_ENOENT);
			return TPP_TOK_ASERR(tok);
		}
	} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));

	trailing_lf_start = file->tf_tpos;
	trailing_lf_end   = directive_iter;
	tpp_file_push_eof_and_ifdef(file);
	*p_directive_start = file->tf_pos;     /* Restore to continue pointing at effective start of expression */
	file->tf_end = trailing_lf_start;      /* Mark as EOF */
	file->tf_pos += directive_keyword_len; /* Skip over leading keyword */

	/* Parse expression */
	result = tpp_lexer_callhook_parseexpr(self, &expr_value);

	/* Evaluate expression result (and warn about trailing tokens) */
	if (!TPP_ISERR(result)) {
		result = tpp_expr_value_asbool(self, &expr_value);
		tpp_expr_value_fini(&expr_value);
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		if ((!TPP_ISERR(result) || result == TPP_ENOENT) && tpp_lexer_gettok(self) != TPP_TOK_EOF) {
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, directive_name);
			tpp_assert(error != TPP_ENOENT);
			if (TPP_ISERR(error))
				result = error;
		}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	}

	/* Cleanup files pushed by the expression */
#if TPP_HAVE_INCLUDE_STACK
	tpp_lexer_popallfiles(self);
#endif /* TPP_HAVE_INCLUDE_STACK */
	file->tf_pos = trailing_lf_end; /* Tell caller to continue parsing *after* EOL */
	tpp_file_pop_eof_and_ifdef(file);
	tpp_lexer_gettoken(self)->tt_id = TPP_TOK_LF; /* Tell caller that currently loaded token is LF */
	return result;
}

/* Call with the current token loaded as `ifdef`, `ifndef`, `elifdef` or `elifndef`
 * @param: p_directive_start: [out] On success (TPP_EOK or TPP_ENOENT), set
 *                                  to the start of the `ifdef`, ... keyword
 * @return: TPP_EOK:    Directive evaluates to *true*
 * @return: TPP_ENOENT: Directive evaluates to *false*
 * @return: * :         Error */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#define tpp_lexer_parse_ifdef_directive(self, p_directive_start) \
	tpp_lexer_parse_ifdef_directive_ex(self, p_directive_start, NULL)
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_ifdef_directive_ex(tpp_lexer *tpp_restrict self,
                                   tpp_char const **tpp_restrict p_directive_start,
                                   tpp_keyword const **p_macro_keyword)
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_ifdef_directive(tpp_lexer *tpp_restrict self,
                                tpp_char const **tpp_restrict p_directive_start)
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
{
	tpp_errno result;
	tpp_token_id tok;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *directive_iter;
	tpp_token_id const mode = tpp_lexer_gettok(self);
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	char const *const directive_name = tpp_lexer_gettokenkwdcstr(self);
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	bool is_keyword_defined;
	directive_iter = file->tf_pos;
	file->tf_pos = file->tf_tpos; /* Retain start of "ifdef" / "ifndef" keyword */

	/* Skip over space tokens to find the (presumably) keyword to test for being defined. */
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok)) {
		file->tf_pos = directive_iter;
		return TPP_TOK_ASERR(tok);
	}

	/* Check if keyword is defined */
	if (TPP_TOK_ISKEYWORD(tok)) {
		is_keyword_defined = tpp_lexer_getkeyworddefined(self, tpp_lexer_gettokenkwd(self));
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
		if (p_macro_keyword)
			*p_macro_keyword = tpp_lexer_gettokenkwd(self);
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF
		tpp_char const *saved_pos = file->tf_pos;
		file->tf_pos = directive_iter;
		result = tpp_lexer_warnf(self, TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF, directive_name);
		file->tf_pos = saved_pos;
		if (TPP_ISERR(result))
			return result;
#endif /* TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_IFDEF */
		is_keyword_defined = false;
	}

	/* Warn about extra tokens after the #ifdef-keyword */
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok)) {
			file->tf_pos = directive_iter;
			return TPP_TOK_ASERR(tok);
		}
	}
	if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		tpp_char const *saved_pos = file->tf_pos;
		file->tf_pos = directive_iter;
		result = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, directive_name);
		file->tf_pos = saved_pos;
		if (TPP_ISERR(result))
			return result;
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
			if (TPP_TOK_ISERR(tok)) {
				tpp_assert(TPP_TOK_ASERR(tok) != TPP_ENOENT);
				return TPP_TOK_ASERR(tok);
			}
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
	}
#else /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
		if (TPP_TOK_ISERR(tok)) {
			tpp_assert(TPP_TOK_ASERR(tok) != TPP_ENOENT);
			return TPP_TOK_ASERR(tok);
		}
	}
#endif /* !TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */

	/* Load "directive_iter" into file, and extract start of directive */
	*p_directive_start = file->tf_pos;
	file->tf_pos = directive_iter;

	if (mode == TPP_KWD_ifndef ||
	    mode == TPP_KWD_elifndef)
		is_keyword_defined = !is_keyword_defined;
	return is_keyword_defined ? TPP_EOK : TPP_ENOENT;
}

#if TPP_HAVE_TOK_SOL_SHELL_COMMENT
TPP_INTERN_IMPL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_char const *TPPCALL
tpp_token_sol_shell_find_after_pound(tpp_lexer const *tpp_restrict self) {
	tpp_char const *iter = tpp_lexer_gettokenstart(self);
	tpp_char const *end = tpp_lexer_gettokenend(self);
	while (iter < end) {
		tpp_char ch = *iter++;
		if (ch == '#')
			break;
#if TPP_HAVE_TRIGRAPHS
		if (ch == '?' && (iter + 1) < end && *iter == '?' && iter[1] == '=')
			return iter + 2;
#endif /* TPP_HAVE_TRIGRAPHS */
	}
	return iter;
}
#endif /* TPP_HAVE_TOK_SOL_SHELL_COMMENT */

/* Load the next `#ifdef`-like directive into `self`, and return it.
 * On entry, allowed to be pretty much anywhere (method starts out
 * by seeking the next newline, then scanning for directives from
 * there on...)
 *
 * @return: TPP_KWD_ifdef:    Found an `#ifdef`-directive    (current token is `[*]` in: `# [ifdef] foo`)
 * @return: TPP_KWD_ifndef:   Found an `#ifndef`-directive   (current token is `[*]` in: `# [ifndef] foo`)
 * @return: TPP_KWD_elif:     Found an `#elif`-directive     (current token is `[*]` in: `# [elif] foo`)
 * @return: TPP_KWD_elifdef:  Found an `#elifdef`-directive  (current token is `[*]` in: `# [elifdef] foo`)
 * @return: TPP_KWD_elifndef: Found an `#elifndef`-directive (current token is `[*]` in: `# [elifndef] foo`)
 * @return: TPP_KWD_else:     Found an `#else`-directive     (current token is `[*]` in: `# [else]`)
 * @return: TPP_KWD_endif:    Found an `#endif`-directive    (current token is `[*]` in: `# [endif]`)
 * @return: TPP_TOK_EOF:      End-of-file (no warning issued, yet)
 * @return: TPP_TOK_ISERR(*): Error
 */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seek_next_ifdef_directive_impl(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok = tpp_lexer_gettok(self);

	/* Seek until next line-feed */
seek_next_lf:
	while (!TPP_TOK_ISLF_OR_COMMENT(tok)) {
		tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR_OR_EOF(tok))
			return tok;
	}

	/* Seek next non-whitespace/comment token (i.e.: the first non-whitespace token) */
	do {
		tok = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* First non-whitespace/comment token must be '#' */
	if (tok != '#') {
		/* Deal with shell comment tokens (must be re-interpreted as directives) */
#if TPP_HAVE_TOK_SHELL_COMMENT
		if (tok == TPP_TOK_SHELL_COMMENT) {
			tpp_token *const token = tpp_lexer_gettoken(self);
			token->tt_end = token->tt_start + 1;
#if TPP_HAVE_TRIGRAPHS
			if (*token->tt_end == '?') {
				token->tt_end += 2;
			} else
#endif /* TPP_HAVE_TRIGRAPHS */
			{
			}
/*			token->tt_id = tok = TPP_TOK_OFCHAR('#'); * Not needed */
		} else
#endif /* TPP_HAVE_TOK_SHELL_COMMENT */
#if TPP_HAVE_TOK_SOL_SHELL_COMMENT
		if (tok == TPP_TOK_SOL_SHELL_COMMENT) {
			tpp_token *const token = tpp_lexer_gettoken(self);
			token->tt_end = tpp_token_sol_shell_find_after_pound(self);
/*			token->tt_id = tok = TPP_TOK_OFCHAR('#'); * Not needed */
		} else
#endif /* TPP_HAVE_TOK_SOL_SHELL_COMMENT */
		{
			goto seek_next_lf;
		}
	}

	/* Find token that comes after the leading '#'
	 * -> This (may be) what our caller is interested in. */
	do {
		tok = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	switch (tok) {

	case TPP_KWD_if:
	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef:
	case TPP_KWD_elif:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
	case TPP_KWD_else:
	case TPP_KWD_endif:
		/* Found a preprocessor directive of interest */
		return tok;

		/* Special case for #error / #warning directives: allow incomplete strings
		 * >> #if 0
		 * >> #error That's allowed
		 * >> #endif
		 *
		 * >> #if 0
		 * >> #not_error But this isn't
		 * >> #endif
		 */
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
	{
		tpp_errno error;
#if TPP_HAVE_CPP_ERROR
		if (0) {
	case TPP_KWD_error:
			if (!tpp_lexer_has(self, CPP_ERROR))
				break;
		}
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
		if (0) {
	case TPP_KWD_warning:
			if (!tpp_lexer_has(self, CPP_WARNING))
				break;
		}
#endif /* TPP_HAVE_CPP_WARNING */
		error = tpp_lexer_seek_eol(self, &tpp_lexer_gettoken(self)->tt_end
		                           tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		/* Because of the seek, we're now at an LF token */
		tok = TPP_TOK_LF;
		break;
	}
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */

	default: break;
	}
	goto seek_next_lf;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seek_next_ifdef_directive(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
	/* Don't emit warnings whilst inside of an inactive #if-block
	 * This is needed to prevent warnings in cases like this:
	 * >> #if 0
	 * >> Incomplete"string
	 * >> #endif */
	tpp_lexer_nowarnings_pushon(self);
	result = tpp_lexer_seek_next_ifdef_directive_impl(self);
	tpp_lexer_nowarnings_pop(self);
	return result;
}

/* Seek end of an inactive `#if 1 ... #else`-style block.
 * - Warn about `#elif` / `#else` directives via `TPP_W_ELIF_OR_ELSE_AFTER_ELSE`
 */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_next_unmatched_endif(tpp_lexer *tpp_restrict self,
                                           tpp_ifdef_stack_entry *ifdef_entry) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	tpp_token_id tok;
again:
	tok = tpp_lexer_seek_next_ifdef_directive(self);
	switch (tok) {
	case TPP_TOK_EOF:
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
		error = tpp_lexer_warnf_lc(self, tpp_file_getfilename(file),
		                           tpp_ifdef_stack_entry_getcreated(ifdef_entry),
		                           TPP_W_EOF_BEFORE_ENDIF);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */
		return TPP_EOK;

	case TPP_KWD_if:
	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef: {
		/* Skip over nested block */
		tpp_ifdef_stack_entry temp_entry;
		temp_entry.tidse_mode    = TPP_IFDEF_MODE_IFDEF;
		temp_entry.tidse_created = tpp_file_getlcinfo(file, tpp_lexer_gettokenstart(self));
		temp_entry.tidse_updated = temp_entry.tidse_created;
		error = tpp_lexer_seek_end_of_next_unmatched_endif(self, &temp_entry);
		if (TPP_ISERR(error))
			return error;
	}	break;

	case TPP_KWD_elif:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
	case TPP_KWD_else: {
#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE
		if (ifdef_entry->tidse_mode == TPP_IFDEF_MODE_ELSE) {
			error = tpp_lexer_warnf(self, TPP_W_ELIF_OR_ELSE_AFTER_ELSE, ifdef_entry,
			                        (char const *)tpp_lexer_gettokenkwd(self)->tk_kwd);
			if (TPP_ISERR(error))
				return error;
		}
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
		if (tok == TPP_KWD_else)
			ifdef_entry->tidse_mode = TPP_IFDEF_MODE_ELSE;
		ifdef_entry->tidse_updated = tpp_file_getlcinfo(file, tpp_lexer_gettokenstart(self));
	}	break;

	case TPP_KWD_endif:
		/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return error;
		}
		while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
#else /* TPP_HAVE_TPP_W_ENDIF_LABELS */
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
#endif /* !TPP_HAVE_TPP_W_ENDIF_LABELS */
		return TPP_EOK;

	default:
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		break;
	}
	goto again;
}

/* Seek end of an inactive `#if 0`-style block.
 * - If a `#else` or `#elif 1`-style block is found, push+create a
 *   new `#ifdef`-entry using `ifdef_location` as the created-position
 * - If a `#endif` is found, behave like `tpp_lexer_seek_end_of_next_unmatched_endif()` */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_seek_end_of_inactive_ifdef(tpp_lexer *tpp_restrict self,
                                     tpp_lcinfo ifdef_location) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	tpp_token_id tok;
again:
	tok = tpp_lexer_seek_next_ifdef_directive(self);
	switch (tok) {
	case TPP_TOK_EOF:
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
		error = tpp_lexer_warnf_lc(self, tpp_file_getfilename(file),
		                           ifdef_location, TPP_W_EOF_BEFORE_ENDIF);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */
		return TPP_EOK;

	case TPP_KWD_if:
	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef: {
		/* Skip over nested block */
		tpp_ifdef_stack_entry temp_entry;
		temp_entry.tidse_mode    = TPP_IFDEF_MODE_IFDEF;
		temp_entry.tidse_created = tpp_file_getlcinfo(file, tpp_lexer_gettokenstart(self));
		temp_entry.tidse_updated = temp_entry.tidse_created;
		error = tpp_lexer_seek_end_of_next_unmatched_endif(self, &temp_entry);
		if (TPP_ISERR(error))
			return error;
	}	break;

	{
		tpp_char const *directive_start;
		tpp_ifdef_stack_entry *ifdef_entry;
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
		error = tpp_lexer_parse_ifdef_directive(self, &directive_start);
		goto handle_pp_if_error;
	case TPP_KWD_elif:
		error = tpp_lexer_parse_if_directive(self, &directive_start);
handle_pp_if_error:
		if (error == TPP_ENOENT)
			goto again; /* *waves hand* "These aren't the #elif-s you're looking for..." */
		if (TPP_ISERR(error))
			return error;

		/* Create a new #ifdef-entry */
		tpp_file_maybe_delete_include_guard_keyword(file);
		ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
		if tpp_unlikely(!ifdef_entry)
			return TPP_ENOMEM;
		ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_IFDEF;
		ifdef_entry->tidse_created = ifdef_location;
		ifdef_entry->tidse_updated = tpp_file_getlcinfo(file, directive_start);
		return TPP_EOK;
	}	break;


	case TPP_KWD_else: {
		tpp_ifdef_stack_entry *ifdef_entry;
		tpp_lcinfo updated_at = tpp_file_getlcinfo(file, tpp_lexer_gettokenstart(self));

		/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return error;
		}
		while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
#else /* TPP_HAVE_TPP_W_ENDIF_LABELS */
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
#endif /* !TPP_HAVE_TPP_W_ENDIF_LABELS */

		/* Create a new #ifdef-entry */
		tpp_file_maybe_delete_include_guard_keyword(file);
		ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
		if tpp_unlikely(!ifdef_entry)
			return TPP_ENOMEM;
		ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_IFDEF;
		ifdef_entry->tidse_created = ifdef_location;
		ifdef_entry->tidse_updated = updated_at;
		return TPP_EOK;
	}	break;

	case TPP_KWD_endif:
		/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return error;
		}
		while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
#else /* TPP_HAVE_TPP_W_ENDIF_LABELS */
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
#endif /* !TPP_HAVE_TPP_W_ENDIF_LABELS */
		return TPP_EOK;

	default:
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		break;
	}
	goto again;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_if_directive(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	tpp_ifdef_stack_entry *ifdef_entry;
	tpp_char const *directive_start;
	tpp_assert(tpp_lexer_gettok(self) == TPP_KWD_if ||
	           tpp_lexer_gettok(self) == TPP_KWD_ifdef ||
	           tpp_lexer_gettok(self) == TPP_KWD_ifndef);
	switch (tpp_lexer_gettok(self)) {

#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
	case TPP_KWD_ifndef: {
		tpp_keyword const *ifndef_keyword = NULL;
		error = tpp_lexer_parse_ifdef_directive_ex(self, &directive_start, &ifndef_keyword);
		if (!(file->tf_flags & TPP_FILE_FLAGS_NOGUARD) &&
		    (error == TPP_EOK || error == TPP_ENOENT) && ifndef_keyword != NULL &&
		    file->tf_kind == TPP_FILE_KIND_IO && tpp_ifdef_stack_isempty(tpp_file_getifdef(file)) &&
#if TPP_HAVE_FILE_NOKWD
		    !(file->tf_flags & TPP_FILE_FLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
		    file->tf_data.td_io.tff_name != NULL) {
			tpp_keyword *kwd;
			tpp_errno setkwd_error;
			kwd = (tpp_keyword *)((char const *)file->tf_data.td_io.tff_name -
			                      tpp_offsetof(tpp_keyword, tk_kwd));
			kwd = tpp_keywords_copybuiltin(&self->tl_kwds, kwd);
			if tpp_unlikely(!kwd)
				return TPP_TOK_ENOMEM;
			setkwd_error = tpp_keyword_set_file_guard(kwd, ifndef_keyword);
			if (TPP_ISERR(setkwd_error))
				return TPP_TOK_OFERR(setkwd_error);
			if (error == TPP_ENOENT)
				goto do_seek_end_of_ifdef_block;
			/* XXX: -Wheader-guard */
			goto do_append_to_ifdef_stack;
		}
	}	break;
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	case TPP_KWD_ifndef:
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	case TPP_KWD_ifdef:
		error = tpp_lexer_parse_ifdef_directive(self, &directive_start);
		break;
	case TPP_KWD_if:
		error = tpp_lexer_parse_if_directive(self, &directive_start);
		break;
	default: tpp_unreachable();
	}
	if (error == TPP_ENOENT) {
		/* false-condition -> seek end-of-block */
		tpp_lcinfo created_at;
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
do_seek_end_of_ifdef_block:
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
		created_at = tpp_file_getlcinfo(file, directive_start);
		error = tpp_lexer_seek_end_of_inactive_ifdef(self, created_at);
		return TPP_TOK_OFERR_OR_EOF(error);
	}
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tpp_file_maybe_delete_include_guard_keyword(file);
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
do_append_to_ifdef_stack:
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
	if tpp_unlikely(!ifdef_entry)
		return TPP_TOK_ENOMEM;
	ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_IFDEF;
	ifdef_entry->tidse_created = tpp_file_getlcinfo(file, directive_start);
	ifdef_entry->tidse_updated = ifdef_entry->tidse_created;
	return TPP_TOK_EOF;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_else_directive(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_errno error;
	tpp_lcinfo lc_update;
	tpp_ifdef_stack_entry *ifdef_entry;
	lc_update = tpp_file_getlcinfo(file, token->tt_start);

	/* Check for error-case: #ifdef-stack is empty */
	if (tpp_ifdef_stack_isempty(tpp_file_getifdef(file))) {
#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF
		error = tpp_lexer_warnf(self, TPP_W_ELIF_OR_ELSE_WITHOUT_IF,
		                        (char const *)token->tt_kwd->tk_kwd);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF */
		switch (token->tt_id) {
		case TPP_KWD_elif:
			token->tt_id = TPP_KWD_if;
			return tpp_lexer_handle_if_directive(self);
		case TPP_KWD_elifdef:
			token->tt_id = TPP_KWD_ifdef;
			return tpp_lexer_handle_if_directive(self);
		case TPP_KWD_elifndef:
			token->tt_id = TPP_KWD_ifndef;
			return tpp_lexer_handle_if_directive(self);
		case TPP_KWD_else:
			break;
		default: tpp_unreachable();
		}
		tpp_file_maybe_delete_include_guard_keyword(file);
		ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
		if tpp_unlikely(!ifdef_entry)
			return TPP_TOK_ENOMEM;
		ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_ELSE;
		ifdef_entry->tidse_created = lc_update;
		ifdef_entry->tidse_updated = lc_update;
		return tpp_lexer_yieldraw_eol(self);
	}

	/* Load the most-recent #ifdef-stack entry */
	ifdef_entry = tpp_ifdef_stack_getlast(tpp_file_getifdef(file));
#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE
	if (ifdef_entry->tidse_mode == TPP_IFDEF_MODE_ELSE) {
		error = tpp_lexer_warnf(self, TPP_W_ELIF_OR_ELSE_AFTER_ELSE, ifdef_entry,
		                        (char const *)token->tt_kwd->tk_kwd);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
	if (token->tt_id == TPP_KWD_else)
		ifdef_entry->tidse_mode = TPP_IFDEF_MODE_ELSE;
	ifdef_entry->tidse_updated = lc_update;

	/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
	if (token->tt_id == TPP_KWD_else) {
		tpp_token_id result;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
			error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
	}
#endif /* TPP_HAVE_TPP_W_ENDIF_LABELS */

	/* Seek end of next unmatched #endif-directive */
	error = tpp_lexer_seek_end_of_next_unmatched_endif(self, ifdef_entry);
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);

	/* Remove #ifdef-stack entry */
	tpp_assert(!tpp_ifdef_stack_isempty(tpp_file_getifdef(file)));
	tpp_assert(ifdef_entry == tpp_ifdef_stack_getlast(tpp_file_getifdef(file)));
	tpp_ifdef_stack_remove(tpp_file_getifdef(file));
	return TPP_TOK_EOF;
}


static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_endif_directive(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	if (tpp_ifdef_stack_isempty(tpp_file_getifdef(file))) {
#if TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF
		tpp_errno error = tpp_lexer_warnf(self, TPP_W_ENDIF_WITHOUT_IF);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF */
		return tpp_lexer_yieldraw_eol(self);
	}

	/* Remove #ifdef-stack entry */
	tpp_assert(!tpp_ifdef_stack_isempty(tpp_file_getifdef(file)));
	tpp_ifdef_stack_remove(tpp_file_getifdef(file));

	/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
	{
		tpp_token_id result;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_ENDIF_LABELS);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
	}
#endif /* TPP_HAVE_TPP_W_ENDIF_LABELS */

	return tpp_lexer_yieldraw_eol(self);
}
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED

#define TPP_LEXER_PARSE_INCLUDE_DIRECTIVE_IMPL_HAS__FOR_EMBED    \
	((TPP_HAVE_CPP_EMBED && TPP_HAVE_LEXER_OPEN_EMBED_STRING) && \
	 (TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT))

/* Parse the string with the current token pointing at the `include`-keyword
 * Returns with the current token ending directly after the include-string (meaning
 * that any trailing comments, or the trailing line-feed have *NOT* been parsed, yet,
 * and that the lexer may even be inside of a nested macro right now)
 *
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOENT:  No such file or directory (a warning was already emitted),
 *                       or file was marked according to `mask_flags`
 * @return: TPP_ENOMEM:  Out of memory
 * @return: TPP_EIO:     I/O error */
#define tpp_lexer_parse_include_directive_impl(self, result, mask_flags, include_kind, for_embed) \
	tpp_lexer_parse_include_directive_impl_(self, result                                          \
		_tpp_lexer_parse_include_directive__mask_flags(mask_flags)                                \
		_tpp_lexer_parse_include_directive__include_kind(include_kind)                            \
		_tpp_lexer_parse_include_directive__for_embed(for_embed))
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_include_directive_impl_(tpp_lexer *tpp_restrict self,
                                        tpp_lexer_openfile_result *tpp_restrict result
#if TPP_HAVE_LEXER_OPENFILE_EX
                                        , tpp_lexer_openfile_flags mask_flags
#define _tpp_lexer_parse_include_directive__mask_flags(mask_flags) , mask_flags
#else /* TPP_HAVE_LEXER_OPENFILE_EX */
#define _tpp_lexer_parse_include_directive__mask_flags(mask_flags) /* nothing */
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK
                                        , tpp_hook_include_kind include_kind
#define _tpp_lexer_parse_include_directive__include_kind(include_kind) , include_kind
#if TPP_LEXER_PARSE_INCLUDE_DIRECTIVE_IMPL_HAS__FOR_EMBED
#define tpp_lexer_parse_include_directive__for_embed (include_kind == TPP_HOOK_INCLUDE_KIND_EMBED)
#endif /* TPP_LEXER_PARSE_INCLUDE_DIRECTIVE_IMPL_HAS__FOR_EMBED */
#else /* ... */
#define _tpp_lexer_parse_include_directive__include_kind(include_kind) /* nothing */
#endif /* !... */
#if (!(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) && \
     TPP_LEXER_PARSE_INCLUDE_DIRECTIVE_IMPL_HAS__FOR_EMBED)
                                        , bool for_embed
#define _tpp_lexer_parse_include_directive__for_embed(for_embed) , for_embed
#define tpp_lexer_parse_include_directive__for_embed for_embed
#else /* ... */
#define _tpp_lexer_parse_include_directive__for_embed(for_embed) /* nothing */
#endif /* !... */
                                        ) {
	tpp_errno error;
	tpp_token_id tok;
	tpp_char const *directive_iter;
	tpp_char const *token_start;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *directive_file;
	tpp_char const *directive_start;
	tpp_size directive_rel_end;
	tpp_assert(tpp_lexer_getfile(self)->tf_prev == NULL &&
	           "Caller must tpp_file_autopopfile_pushoff()");
again:
	directive_file = tpp_lexer_getfile(self);
#if TPP_HAVE_CPP_MACROS
	while (directive_file->tf_prev)
		directive_file = directive_file->tf_prev;
#endif /* TPP_HAVE_CPP_MACROS */

	/* Yield the next token (whilst keeping the start of the "include"-keyword loaded in memory) */
	directive_iter = token->tt_end;
	directive_start = directive_file->tf_tpos;
	directive_rel_end = (tpp_size)(directive_file->tf_pos - directive_start);
	directive_file->tf_pos = directive_start;
	tok = tpp_lexer_yieldraw_at_include_string_blocking(self, &directive_iter);
	token_start = token->tt_start;
	directive_start = directive_file->tf_pos;
	directive_file->tf_tpos = directive_start;
	directive_file->tf_pos += directive_rel_end;
	token->tt_end = directive_iter;

#if TPP_HAVE_CPP_MACROS
	if (TPP_TOK_ISKEYWORD(tok)) {
		/* Preserve the starting position of the current directive */
		tpp_size rel_directive_start;
		tpp_file_pushkeep(directive_file, directive_start);
		rel_directive_start = tpp_file_keep_ptr2rel(directive_file, directive_start);

		/* Do macro expansion... */
		token->tt_start = token_start;
		tok = tpp_lexer_yield_handle_keyword(self, tok);

		directive_start = tpp_file_keep_rel2ptr(directive_file, rel_directive_start);
		tpp_file_popkeep(directive_file);
		directive_file->tf_tpos = directive_start;
		if (tok == TPP_TOK_EOF)
			goto again;
	}
#endif /* TPP_HAVE_CPP_MACROS */

	/* Skip over leading whitespace and comments (but not line-feeds) */
	if (TPP_TOK_ISSPACE_OR_COMMENT(tok))
		goto again;

	/* Propagate errors */
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	token->tt_start = token_start;
	if (tok == TPP_TOK_INCPATH_DQUOTE || tok == TPP_TOK_INCPATH_LANGLE) {
		/* Invoke include-hook. Only proceed to  */
#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK
		error = tpp_lexer_callhook_include_encountered(self, include_kind);
		if (!TPP_ISERR(error))
#endif /* TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK */
		{
#if TPP_LEXER_PARSE_INCLUDE_DIRECTIVE_IMPL_HAS__FOR_EMBED
			if (tpp_lexer_parse_include_directive__for_embed)
#endif /* TPP_LEXER_PARSE_INCLUDE_DIRECTIVE_IMPL_HAS__FOR_EMBED */
			{
#if TPP_HAVE_CPP_EMBED && TPP_HAVE_LEXER_OPEN_EMBED_STRING
				error = tpp_lexer_open_embed_string(self, result);
#endif /* TPP_HAVE_CPP_EMBED && TPP_HAVE_LEXER_OPEN_EMBED_STRING */
			}
#if TPP_LEXER_PARSE_INCLUDE_DIRECTIVE_IMPL_HAS__FOR_EMBED
			else
#endif /* TPP_LEXER_PARSE_INCLUDE_DIRECTIVE_IMPL_HAS__FOR_EMBED */
			{
#if TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT
#if TPP_HAVE_LEXER_OPENFILE_EX
				error = tpp_lexer_open_include_string_ex(self, result, mask_flags);
#else  /* TPP_HAVE_LEXER_OPENFILE_EX */
				error = tpp_lexer_open_include_string(self, result);
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
#endif /* TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT */
			}

#if TPP_HAVE_TPP_W_NO_SUCH_FILE || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK
			if (error == TPP_ENOENT) {
#if TPP_HAVE_INCLUDE_NOT_FOUND_HOOK
				tpp_errno hook_error = tpp_lexer_callhook_include_not_found(self, include_kind);
				if (hook_error != TPP_ENOENT) {
					if (TPP_ISERR(hook_error))
						error = hook_error;
				} else
#endif /* TPP_HAVE_INCLUDE_NOT_FOUND_HOOK */
				{
#if TPP_HAVE_TPP_W_NO_SUCH_FILE
					tpp_errno warn_error = tpp_lexer_warnf(self, TPP_W_NO_SUCH_FILE);
					if (TPP_ISERR(warn_error))
						error = warn_error;
#endif /* TPP_HAVE_TPP_W_NO_SUCH_FILE */
				}
			} else
#endif /* TPP_HAVE_TPP_W_NO_SUCH_FILE || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK */
			{
			}
		}
#if TPP_HAVE_LEXER_OPENFILE_EX
		if (error == TPP_EMASKED)
			error = TPP_ENOENT;
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_INCLUDE_STRING);
		if (!TPP_ISERR(error))
			error = TPP_ENOENT;
#else /* TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
		error = TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
	}
	directive_file->tf_tpos = directive_start;

	/* Propagate errors. */
	if (error != TPP_EOK && error != TPP_ENOENT) {
#if TPP_HAVE_CPP_MACROS
		tpp_lexer_popallfiles(self);
#endif /* TPP_HAVE_CPP_MACROS */
		return error;
	}

	return error;
#undef tpp_lexer_parse_include_directive__for_embed
}
#endif /* TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT || TPP_HAVE_CPP_EMBED */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT

/* Parse the string with the current token pointing at the `include`-keyword
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOENT:  No such file or directory (a warning was already emitted),
 *                       or file was marked according to `mask_flags`
 * @return: TPP_ENOMEM:  Out of memory
 * @return: TPP_EIO:     I/O error */
#define tpp_lexer_parse_include_directive(self, result, mask_flags, include_kind) \
	tpp_lexer_parse_include_directive_(self, result                               \
		_tpp_lexer_parse_include_directive__mask_flags(mask_flags)                \
		_tpp_lexer_parse_include_directive__include_kind(include_kind))
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_include_directive_(tpp_lexer *tpp_restrict self,
                                   tpp_lexer_openfile_result *tpp_restrict result
#if TPP_HAVE_LEXER_OPENFILE_EX
                                   , tpp_lexer_openfile_flags mask_flags
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK
                                   , tpp_hook_include_kind include_kind
#endif /* TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK */
                                   ) {
	tpp_errno error;
	tpp_token_id tok;
	tpp_token *const token = tpp_lexer_gettoken(self);
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	bool did_warn_about_extra_tokens = false;
	char const *const directive_name = tpp_lexer_gettokenkwdcstr(self);
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */

	/* Call underlying include loader. */
	error = tpp_lexer_parse_include_directive_impl(self, result, mask_flags,
	                                               include_kind, false);
	if (error != TPP_EOK && error != TPP_ENOENT)
		return error;

	/* Seek until EOF */
	tok = tpp_lexer_gettok(self);
	while (!TPP_TOK_ISLF_OR_COMMENT(tok)
#if TPP_HAVE_CPP_MACROS
	       || tpp_lexer_getfile(self)->tf_prev
#endif /* TPP_HAVE_CPP_MACROS */
	       ) {
		tpp_size directive_rel_end;
		tpp_file *file = tpp_lexer_getfile(self);
		tpp_char const *directive_start;
		tpp_char const *directive_iter;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		tpp_char const *token_start;
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
		if (tok == TPP_TOK_EOF) {
#if TPP_HAVE_CPP_MACROS
			if (tpp_lexer_getfile(self)->tf_prev) {
				tpp_lexer_popfile(self);
			} else
#endif /* TPP_HAVE_CPP_MACROS */
			{
				break;
			}
		}

#if TPP_HAVE_CPP_MACROS
		while (file->tf_prev)
			file = file->tf_prev;
#endif /* TPP_HAVE_CPP_MACROS */
		directive_iter = token->tt_end;
		directive_start = file->tf_tpos;
		directive_rel_end = (tpp_size)(file->tf_pos - directive_start);
		file->tf_pos = directive_start;
		tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		token_start = token->tt_start;
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
		directive_start = file->tf_pos;
		file->tf_tpos = directive_start;
		file->tf_pos += directive_rel_end;
		token->tt_end = directive_iter;

		if (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT_OR_EOF(tok))
			continue;
		if (TPP_TOK_ISERR(tok)) {
			if (error == TPP_EOK)
				tpp_lexer_openfile_result_fini(result);
			return TPP_TOK_ASERR(tok);
		}
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		if (!did_warn_about_extra_tokens) {
			tpp_errno warn_error;
			tpp_char const *saved_start;
			did_warn_about_extra_tokens = true;
			saved_start = token->tt_start;
			token->tt_start = token_start;
			warn_error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, directive_name);
			token->tt_start = saved_start;
			if (TPP_ISERR(warn_error)) {
				if (error == TPP_EOK)
					tpp_lexer_openfile_result_fini(result);
				return warn_error;
			}
		}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	}

#if TPP_HAVE_CPP_MACROS
	tpp_assert(!tpp_lexer_canpopfile(self));
#endif /* TPP_HAVE_CPP_MACROS */
	return error;
}

#define tpp_lexer_handle_include_directive(self, _tfapfp_prev, mask_flags, include_kind) \
	tpp_lexer_handle_include_directive_(self, _tfapfp_prev                               \
		_tpp_lexer_parse_include_directive__mask_flags(mask_flags)                       \
		_tpp_lexer_parse_include_directive__include_kind(include_kind))
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_include_directive_(tpp_lexer *tpp_restrict self,
                                    tpp_file *const _tfapfp_prev
#if TPP_HAVE_LEXER_OPENFILE_EX
                                    , tpp_lexer_openfile_flags mask_flags
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
#if TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK
                                    , tpp_hook_include_kind include_kind
#endif /* TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK || TPP_HAVE_INCLUDE_NOT_FOUND_HOOK */
                                    ) {
	tpp_file *prev_file;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	tpp_lexer_openfile_result ofr;

	/* Parse include string... */
	error = tpp_lexer_parse_include_directive(self, &ofr, mask_flags, include_kind);

	/* Pop no-autopopfile block originally created in "tpp_lexer_process_directive()" */
	tpp_lexer_autopopfile_break(self);

	/* Check for errors... */
	if (TPP_ISERR(error)) {
		if (error == TPP_ENOENT)
			return TPP_TOK_EOF; /* Continue parsing after a masked file */
		return TPP_TOK_OFERR(error);
	}

	/* Push a new file */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file) {
		tpp_lexer_openfile_result_fini(&ofr);
		return TPP_TOK_ENOMEM;
	}
	file->tf_flags &= ~TPP_FILE_FLAGS_NODIRECTIVES; /* Allow more directives immediately upon return from file */
	tpp_file_move(prev_file, file);
	tpp_file_init_io_ex(file, tpp_keyword_getcstr(ofr.tlofr_filename_kwd),
	                    ofr.tlofr_handle, TPP_FILE_FLAGS_NORMAL);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	/* Continue parsing in newly pushed file */
	error = tpp_lexer_callhook_file_pushed(self);
	return TPP_TOK_OFERR_OR_EOF(error);
}
#endif /* TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed
typedef struct tpp_embed_builder {
	tpp_uintmax               teb_limit;     /* Limit on how many bytes to embed */
#if TPP_HAVE_CPP_EMBED_OFFSET
	tpp_uintmax               teb_offset;    /* Offset on how many bytes to embed */
#endif /* TPP_HAVE_CPP_EMBED_OFFSET */
#if TPP_HAVE_CPP_EMBED
	tpp_lexer_arginfo         teb_prefix;    /* Prefix to put before a non-empty file */
	tpp_lexer_arginfo         teb_suffix;    /* Suffix to put after a non-empty file */
	tpp_lexer_arginfo         teb_if_empty;  /* Replacement for an empty file */
	tpp_lexer_openfile_result teb_ofr;       /* [valid_if(teb_ofr_error == TPP_EOK)] The file to embed */
	tpp_errno                 teb_ofr_error; /* Error from opening `teb_ofr` (either `TPP_EOK`, or `TPP_ENOENT`) */
#endif /* TPP_HAVE_CPP_EMBED */
} tpp_embed_builder;

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_skip_ns_prefix(tpp_lexer *tpp_restrict lexer, tpp_token_id tok) {
#if TPP_HAVE_TOK_COLON_COLON
	if (tok == TPP_TOK_COLON_COLON)
		goto yield_and_skip_whitespace;
#endif /* TPP_HAVE_TOK_COLON_COLON */

#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_COLON_COLON)
	if (tok == ':') {
		tpp_char next_ch;
		tpp_char const *pos = tpp_lexer_gettokenend(lexer);
		tpp_errno read_err = tpp_lexer_readchar(lexer, &pos, &next_ch);
		if (TPP_ISERR(read_err))
			return read_err;
		if (next_ch == ':') {
			tpp_file_setpos(tpp_lexer_getfile(lexer), pos);
			goto yield_and_skip_whitespace;
		}
	}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TOK_COLON_COLON) */

	return TPP_ENOENT;
yield_and_skip_whitespace:
	do {
		tok = tpp_lexer_yield_blocking(lexer);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	return TPP_TOK_ASERR_OR_EOK(tok);
}

/* Parse trailing parameters following a `#embed` directive */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_embed_builder_handle_param(tpp_embed_builder *tpp_restrict self,
                               tpp_lexer *tpp_restrict lexer,
                               tpp_token_id param_kwd) {
	tpp_token_id tok;
	tpp_errno error;
	tpp_keyword const *function_name_kwd;
	char const *function_name;
again:
	function_name_kwd = tpp_lexer_gettokenkwd(lexer);
	function_name = tpp_keyword_getcstr(function_name_kwd);
	switch (param_kwd) {

#if TPP_HAVE_CPP_EMBED_OFFSET
	case TPP_KWD_offset:
handle_offset_param:
		if (!tpp_lexer_has(lexer, CPP_EMBED_OFFSET))
			break;
		TPP_FALLTHRU
#endif /* TPP_HAVE_CPP_EMBED_OFFSET */
	case TPP_KWD_limit: {
		tpp_expr_value int_value_expr;
		tpp_intmax int_value;
		do {
			tok = tpp_lexer_yield_blocking(lexer);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_require(lexer, TPP_TOK_OFCHAR('('));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		error = tpp_lexer_callhook_parseexpr(lexer, &int_value_expr);
		if (TPP_ISERR(error))
			return error;
		if (tpp_expr_value_isint(&int_value_expr)) {
			error = tpp_expr_value_asintmax(&int_value_expr, &int_value);
			if (error == TPP_ENOENT) {
#if TPP_HAVE_TPP_W_INTEGER_OVERFLOW
				error = tpp_lexer_warnf(lexer, TPP_W_INTEGER_OVERFLOW, "<#embed parameter>", "", "");
#else /* TPP_HAVE_TPP_W_INTEGER_OVERFLOW */
				error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_INTEGER_OVERFLOW */
				int_value = 0;
			}
		} else {
			error = tpp_expr_value_asbool(lexer, &int_value_expr);
			int_value = 1;
			if (error == TPP_ENOENT) {
				error = TPP_EOK;
				int_value = 0;
			}
		}
		tpp_expr_value_fini(&int_value_expr);
		if (TPP_ISERR(error))
			return error;
		switch (param_kwd) {
		case TPP_KWD_limit:
			self->teb_limit = int_value;
			break;
#if TPP_HAVE_CPP_EMBED_OFFSET
		case TPP_KWD_offset:
			self->teb_offset = int_value;
			break;
#endif /* TPP_HAVE_CPP_EMBED_OFFSET */
		default: tpp_unreachable();
		}
		tok = tpp_lexer_gettok(lexer);
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
			tok = tpp_lexer_yield_blocking(lexer);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_require(lexer, TPP_TOK_OFCHAR(')'));
		return TPP_TOK_ASERR_OR_EOK(tok);
	}	break;

#if TPP_HAVE_CPP_EMBED_OFFSET
	case TPP_KWD_gnu:
	case TPP_KWD_clang:
		do {
			tok = tpp_lexer_yield_blocking(lexer);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		error = tpp_lexer_skip_ns_prefix(lexer, tok);
		if (TPP_ISERR(error)) {
			if (error != TPP_ENOENT)
				return error;
			break;
		}
		param_kwd = tpp_lexer_gettok(lexer);
		if (!TPP_TOK_ISKEYWORD(param_kwd))
			break;
handle_gnu_clang_ns_prefixed_tok:
		function_name_kwd = tpp_lexer_gettokenkwd(lexer);
		function_name = tpp_keyword_getcstr(function_name_kwd);
		switch (param_kwd) {

		case TPP_KWD_offset:
			/* gnu::offset */
			/* clang::offset */
			goto handle_offset_param;

		default:
			if (*function_name == '_' || function_name[tpp_keyword_getlen(function_name_kwd) - 1] == '_') {
				tpp_size len = tpp_keyword_getlen(function_name_kwd);
				while (len && function_name[len - 1] == '_')
					--len;
				while (len && function_name[0] == '_')
					++function_name, --len;
				if (len) {
					function_name_kwd = tpp_lexer_kwds_getkeyword(lexer, (tpp_char const *)function_name, len,
					                                              tpp_hashof((tpp_char const *)function_name, len));
					if (function_name_kwd) {
						tpp_token_setkwd(tpp_lexer_gettoken(lexer), function_name_kwd);
						param_kwd = tpp_keyword_getid(function_name_kwd);
						goto handle_gnu_clang_ns_prefixed_tok;
					}
				}
			}
			break;
		}
		break;
#endif /* TPP_HAVE_CPP_EMBED_OFFSET */

	/* XXX: gnu::base64 */

	case TPP_KWD_prefix:
	case TPP_KWD_suffix:
	case TPP_KWD_if_empty: {
		tpp_lexer_arginfo arg;
#if TPP_HAVE_CPP_EMBED
		tpp_lexer_arginfo *p_dst_arg;
#endif /* TPP_HAVE_CPP_EMBED */
		unsigned int seek_flags;
		do {
			tok = tpp_lexer_yield_blocking(lexer);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_require(lexer, TPP_TOK_OFCHAR('('));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);

		/* Seek to )-token and capture arguments */
		seek_flags = TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS;
		/* Control behavior of "TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC"
		 * -- Should depend on "-fmacro-argument-whitespace" */
#if TPP_CONF_ISRT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
		if (tpp_lexer_has(lexer, MACRO_ARGUMENT_WHITESPACE))
			seek_flags |= TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC;
#endif /* TPP_CONF_ISRT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
		tok = tpp_lexer_seekpp_rparen_exact(lexer, &arg, 1, function_name, seek_flags);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
#if TPP_HAVE_CPP_EMBED
		switch (param_kwd) {
		case TPP_KWD_prefix: p_dst_arg = &self->teb_prefix; break;
		case TPP_KWD_suffix: p_dst_arg = &self->teb_suffix; break;
		case TPP_KWD_if_empty: p_dst_arg = &self->teb_if_empty; break;
		default: tpp_unreachable();
		}
		tpp_lexer_arginfo_fini(p_dst_arg);
		*p_dst_arg = arg;
#else /* TPP_HAVE_CPP_EMBED */
		tpp_lexer_arginfo_fini(&arg);
#endif /* !TPP_HAVE_CPP_EMBED */
		return TPP_EOK;
	}	break;

	default:
		/* If current keyword starts/ends with _-characters,
		 * strip those characters and try again */
		if (*function_name == '_' || function_name[tpp_keyword_getlen(function_name_kwd) - 1] == '_') {
			tpp_size len = tpp_keyword_getlen(function_name_kwd);
			while (len && function_name[len - 1] == '_')
				--len;
			while (len && function_name[0] == '_')
				++function_name, --len;
			if (len) {
				function_name_kwd = tpp_lexer_kwds_getkeyword(lexer, (tpp_char const *)function_name, len,
				                                              tpp_hashof((tpp_char const *)function_name, len));
				if (function_name_kwd) {
					tpp_token_setkwd(tpp_lexer_gettoken(lexer), function_name_kwd);
					param_kwd = tpp_keyword_getid(function_name_kwd);
					goto again;
				}
			}
		}
		break;
	}

#if TPP_HAVE_TPP_W_UNKNOWN_EMBED_PARAMETER
	error = tpp_lexer_warnf(lexer, TPP_W_UNKNOWN_EMBED_PARAMETER);
	if (TPP_ISERR(error))
		return error;
#endif /* TPP_HAVE_TPP_W_UNKNOWN_EMBED_PARAMETER */

continue_after_unknown_name:
	do {
		tok = tpp_lexer_yield_blocking(lexer);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	error = tpp_lexer_skip_ns_prefix(lexer, tok);
	if (!TPP_ISERR(error))
		goto continue_after_unknown_name;
	if (error != TPP_ENOENT)
		return error;
	tok = tpp_lexer_gettok(lexer);

	if (tok == '(') {
		tpp_lexer_arginfo arg;
		tok = tpp_lexer_seekpp_rparen_exact(lexer, &arg, 1, function_name,
		                                    TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tpp_lexer_arginfo_fini(&arg);
	} else {
		/* Re-parse current token */
		tpp_token *const token = tpp_lexer_gettoken(lexer);
		token->tt_end = token->tt_start;
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_CPP_EMBED || TPP_HAVE_MACRO___has_embed */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___has_embed
#ifndef tpp_embed_builder_handle_param_forhas_result_DEFINED
#define tpp_embed_builder_handle_param_forhas_result_DEFINED
struct tpp_embed_builder_handle_param_forhas_result {
	tpp_uintmax tebhpfhr_limit;  /* Value of `limit` parameter (or `TPP_UINTMAX_MAX`) */
#if TPP_HAVE_CPP_EMBED_OFFSET
	tpp_uintmax tebhpfhr_offset; /* Value of `offset` parameter (or `0`) */
#endif /* TPP_HAVE_CPP_EMBED_OFFSET */
};
#endif /* !tpp_embed_builder_handle_param_forhas_result_DEFINED */

/* Minimal/adjusted parameter handler for __has_embed */
TPP_INTERN_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_embed_builder_handle_param_forhas(struct tpp_embed_builder_handle_param_forhas_result *tpp_restrict res,
                                      tpp_lexer *tpp_restrict lexer, tpp_token_id param_kwd) {
	tpp_errno result;
	tpp_embed_builder self;
	self.teb_limit = res->tebhpfhr_limit;
#if TPP_HAVE_CPP_EMBED_OFFSET
	self.teb_offset = res->tebhpfhr_offset;
#endif /* TPP_HAVE_CPP_EMBED_OFFSET */
#if TPP_HAVE_CPP_EMBED
	self.teb_ofr_error = TPP_ENOENT;
	tpp_lexer_arginfo_init_empty(&self.teb_prefix);
	tpp_lexer_arginfo_init_empty(&self.teb_suffix);
	tpp_lexer_arginfo_init_empty(&self.teb_if_empty);
#endif /* TPP_HAVE_CPP_EMBED */
	result = tpp_embed_builder_handle_param(&self, lexer, param_kwd);
#if TPP_HAVE_CPP_EMBED
	tpp_lexer_arginfo_fini(&self.teb_prefix);
	tpp_lexer_arginfo_fini(&self.teb_suffix);
	tpp_lexer_arginfo_fini(&self.teb_if_empty);
#endif /* TPP_HAVE_CPP_EMBED */
	res->tebhpfhr_limit = self.teb_limit;
#if TPP_HAVE_CPP_EMBED_OFFSET
	res->tebhpfhr_offset = self.teb_offset;
#endif /* TPP_HAVE_CPP_EMBED_OFFSET */
	return result;
}
#endif /* TPP_HAVE_MACRO___has_embed */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EMBED
static TPP_NONNULL((1)) void TPPCALL
tpp_embed_builder_fini(tpp_embed_builder *tpp_restrict self) {
	if (self->teb_ofr_error == TPP_EOK)
		tpp_lexer_openfile_result_fini(&self->teb_ofr);
	tpp_lexer_arginfo_fini(&self->teb_prefix);
	tpp_lexer_arginfo_fini(&self->teb_suffix);
	tpp_lexer_arginfo_fini(&self->teb_if_empty);
}

#if !TPP_HAVE_FILE_ENCODING_EMBED
static tpp_errno TPPCALL
tpp_string_builder_append_embed(tpp_string_builder *tpp_restrict self,
                                tpp_io_handle ioh, tpp_uintmax limit) {
	unsigned char buf[512];
	tpp_size i, count;
	tpp_ssize read_status;
again:
	count = sizeof(buf);
	if ((tpp_uintmax)count > limit)
		count = (tpp_size)limit;
	read_status = tpp_io_read_blocking(ioh, buf, count);
	if (TPP_SSIZE_ISERR(read_status))
		return TPP_SSIZE_ASERR(read_status);
	if (read_status == 0)
		return TPP_EOK;
	limit -= (tpp_size)read_status;
	for (i = 0; i < (tpp_size)read_status; ++i) {
		char repr[4], *p = repr + 4;
		unsigned char b = buf[i];
		*--p = (char)tpp_ascii_ofdigit(b % 10);
		if (b >= 10)
			*--p = (char)tpp_ascii_ofdigit((b / 10) % 10);
		if (b >= 100)
			*--p = (char)tpp_ascii_ofdigit(b / 100);
		*--p = ',';
		if (tpp_string_builder_doprint(self, (tpp_char const *)p, (tpp_size)(repr + 4 - p)) < 0)
			return TPP_ENOMEM;
	}
	goto again;
}
#endif /* !TPP_HAVE_FILE_ENCODING_EMBED */


static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_embed_builder_pack_and_pushfile(tpp_embed_builder *tpp_restrict self,
                                    tpp_lexer *tpp_restrict lexer) {
	tpp_errno error;
	tpp_string_builder embed_data;
	tpp_token_id result;
	tpp_ssize ofr_read_status;
	unsigned char ofr_first_byte;
	tpp_file *const file = tpp_lexer_getfile(lexer);
	tpp_file *prev_file;
	file->tf_flags &= ~TPP_FILE_FLAGS_NODIRECTIVES; /* Allow more directives immediately upon return from file */
	if (self->teb_limit == 0)
		goto return_empty_file; /* Treat as an file... */
	if (self->teb_ofr_error != TPP_EOK)
		goto return_empty_file; /* Treat as an file... */

	/* Apply offset parameter */
#if TPP_HAVE_CPP_EMBED_OFFSET
	if (self->teb_offset != 0) {
		tpp_errno skip_error;
		tpp_uintmax skip_bytes;
		skip_error = tpp_io_skip_blocking(self->teb_ofr.tlofr_handle, self->teb_offset, &skip_bytes);
		if (TPP_ISERR(skip_error)) {
			result = TPP_TOK_OFERR(skip_error);
			goto return_result_and_fini;
		}
		if (skip_bytes < self->teb_offset)
			goto return_empty_file; /* Treat as an file... */
	}
#endif /* TPP_HAVE_CPP_EMBED_OFFSET */

#if TPP_HAVE_FILE_ENCODING_EMBED
	if ((self->teb_if_empty.tlai_start >= self->teb_if_empty.tlai_end) &&
	    (self->teb_prefix.tlai_start >= self->teb_prefix.tlai_end) &&
	    (self->teb_suffix.tlai_start >= self->teb_suffix.tlai_end)) {
		/* Simple case: simply push a "TPP_FILE_ENCODING_EMBED" file */
		prev_file = tpp_file_alloc();
		if tpp_unlikely(!prev_file)
			goto err_nomem;
		tpp_file_move(prev_file, file);
		tpp_file_init_io_from_ofr_ex(file, &self->teb_ofr, TPP_FILE_ENCODING_EMBED);
		tpp_lcstate_init_invalid(&file->tf_data.td_io.tff_start_lc);
		file->tf_data.td_io.tff_encdat.tffed_embedlimit = self->teb_limit;
		file->tf_prev  = prev_file;
		file->tf_tprev = prev_file;
		goto done_inherit_io_handle;
	}
#endif /* TPP_HAVE_FILE_ENCODING_EMBED */

	/* Read the first by of the OFR file */
	ofr_read_status = tpp_io_read_blocking(self->teb_ofr.tlofr_handle, &ofr_first_byte, 1);
	if (TPP_SSIZE_ISERR(ofr_read_status)) {
		result = TPP_TOK_OFERR(TPP_SSIZE_ASERR(ofr_read_status));
		goto return_result_and_fini;
	}

	/* Check if the file is actually empty... */
	if (ofr_read_status == 0)
		goto return_empty_file;
	--self->teb_limit;

	/* File is non-empty! */
	if (self->teb_suffix.tlai_start < self->teb_suffix.tlai_end) {
		/* Push a sub-text file for the suffix */
		prev_file = tpp_file_alloc();
		if tpp_unlikely(!prev_file)
			goto err_nomem;
		*prev_file = *file;
		file->tf_kind  = TPP_FILE_KIND_SUBTEXT
		_tpp_file_subtext_init_ifdef(file)
		_tpp_file_init_lcpos(file);
		file->tf_prev  = prev_file;
		file->tf_tprev = prev_file;
		file->tf_chunk = self->teb_suffix.tlai_chunk;
		file->tf_pos   = self->teb_suffix.tlai_start;
		file->tf_end   = self->teb_suffix.tlai_end;
		self->teb_suffix.tlai_chunk = NULL;
#if TPP_HAVE_FILE_PUSHED_HOOK
		error = tpp_lexer_callhook_file_pushed(lexer);
		if (TPP_ISERR(error)) {
			result = TPP_TOK_OFERR(error);
			goto return_result_and_fini;
		}
#endif /* TPP_HAVE_FILE_PUSHED_HOOK */
	}

	/* Construct the (possibly only head)-data for the embedded file itself. */
	tpp_string_builder_init(&embed_data);
	if (self->teb_prefix.tlai_start < self->teb_prefix.tlai_end) {
		if (tpp_string_builder_doprint(&embed_data, self->teb_prefix.tlai_start,
		                               (tpp_size)(self->teb_prefix.tlai_end - self->teb_prefix.tlai_start)) < 0)
			goto err_nomem_embed_data;
	}
	{
		char ord[3], *p = ord + 3;
		*--p = (char)tpp_ascii_ofdigit(ofr_first_byte % 10);
		if (ofr_first_byte >= 10)
			*--p = (char)tpp_ascii_ofdigit((ofr_first_byte / 10) % 10);
		if (ofr_first_byte >= 100)
			*--p = (char)tpp_ascii_ofdigit(ofr_first_byte / 100);
		if (tpp_string_builder_doprint(&embed_data, (tpp_char const *)p, (tpp_size)(ord + 3 - p)) < 0)
			goto err_nomem_embed_data;
	}

	/* Convert remainder of embedded file into a byte-sequence */
#if !TPP_HAVE_FILE_ENCODING_EMBED
	{
		error = tpp_string_builder_append_embed(&embed_data,
		                                        self->teb_ofr.tlofr_handle,
		                                        self->teb_limit);
		if (TPP_ISERR(error)) {
			result = TPP_TOK_OFERR(error);
			tpp_string_builder_fini(&embed_data);
			goto return_result_and_fini;
		}
	}
#endif /* !TPP_HAVE_FILE_ENCODING_EMBED */

	/* Push a file for the #embed-ed file itself */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem_embed_data;
	tpp_file_move(prev_file, file);
#if TPP_HAVE_FILE_ENCODING_EMBED
	tpp_file_init_io_from_ofr_ex(file, &self->teb_ofr, TPP_FILE_ENCODING_EMBED);
	tpp_lcstate_init_invalid(&file->tf_data.td_io.tff_start_lc);
	file->tf_data.td_io.tff_encdat.tffed_embedlimit = self->teb_limit;
	file->tf_chunk = tpp_string_builder_pack(&embed_data);
	file->tf_pos   = tpp_string_str(file->tf_chunk);
	file->tf_end   = tpp_string_end(file->tf_chunk);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
done_inherit_io_handle:
	tpp_lexer_arginfo_fini(&self->teb_prefix);
	tpp_lexer_arginfo_fini(&self->teb_suffix);
	tpp_lexer_arginfo_fini(&self->teb_if_empty);
#else /* TPP_HAVE_FILE_ENCODING_EMBED */
	{
		TPP_REF tpp_string *chunk = tpp_string_builder_pack(&embed_data);
		tpp_file_init_text_ascii(file, tpp_lexer_openfile_result_getfilename(&self->teb_ofr),
		                         chunk, tpp_string_str(chunk), tpp_string_len(chunk),
		                         tpp_lexer_openfile_result_getfileflags(&self->teb_ofr),
		                         TPP_LCINFO_INVALID);
	}
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	tpp_embed_builder_fini(self);
#endif /* !TPP_HAVE_FILE_ENCODING_EMBED */
	error = tpp_lexer_callhook_file_pushed(lexer);
	return TPP_TOK_OFERR_OR_EOF(error);
return_empty_file:
	if (self->teb_if_empty.tlai_start < self->teb_if_empty.tlai_end) {
		/* Return a sub-text file containing the if-empty text */
		prev_file = tpp_file_alloc();
		if tpp_unlikely(!prev_file)
			goto err_nomem;
		*prev_file = *file;
		file->tf_kind  = TPP_FILE_KIND_SUBTEXT
		_tpp_file_subtext_init_ifdef(file)
		_tpp_file_init_lcpos(file);
		file->tf_prev  = prev_file;
		file->tf_tprev = prev_file;
		file->tf_chunk = self->teb_if_empty.tlai_chunk;
		file->tf_pos   = self->teb_if_empty.tlai_start;
		file->tf_end   = self->teb_if_empty.tlai_end;
		self->teb_if_empty.tlai_chunk = NULL;
#if TPP_HAVE_FILE_PUSHED_HOOK
		error = tpp_lexer_callhook_file_pushed(lexer);
		if (TPP_ISERR(error)) {
			result = TPP_TOK_OFERR(error);
			goto return_result_and_fini;
		}
#endif /* TPP_HAVE_FILE_PUSHED_HOOK */
	}
	result = TPP_TOK_EOF;
return_result_and_fini:
	tpp_embed_builder_fini(self);
	return result;
err_nomem_embed_data:
	tpp_string_builder_fini(&embed_data);
err_nomem:
	result = TPP_TOK_ENOMEM;
	goto return_result_and_fini;
}

/* Must be called with `self` pointing at the token preceding the `#include`-string */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_embed_builder_init_parse(tpp_embed_builder *tpp_restrict self,
                             tpp_lexer *tpp_restrict lexer) {
	tpp_token_id tok;

	/* Call underlying include loader. */
	self->teb_ofr_error = tpp_lexer_parse_include_directive_impl(lexer, &self->teb_ofr,
	                                                             /* Warn about bad casing */
	                                                             TPP_LEXER_OPENFILE_FLAG_WARN_CASING,
	                                                             TPP_HOOK_INCLUDE_KIND_EMBED,
	                                                             true);
	if (self->teb_ofr_error != TPP_EOK && self->teb_ofr_error != TPP_ENOENT)
		return self->teb_ofr_error;

	/* Initialize other pieces of the directive. */
	tpp_lexer_arginfo_init_empty(&self->teb_prefix);
	tpp_lexer_arginfo_init_empty(&self->teb_suffix);
	tpp_lexer_arginfo_init_empty(&self->teb_if_empty);
	self->teb_limit = TPP_UINTMAX_MAX;
#if TPP_HAVE_CPP_EMBED_OFFSET
	self->teb_offset = 0;
#endif /* TPP_HAVE_CPP_EMBED_OFFSET */

	/* At this point, the lexer looks like this:
	 *           tf_tpos  tf_pos
	 *           v        v
	 * >> #embed "foo.dat" limit(DATA_LIMIT) if_empty(42, 10,) prefix(0xff,) suffix(,)
	 * >> content-of-next-line
	 *    ^
	 *    EOF
	 *
	 * We must now parse all those #embed parameters
	 * NOTE: We may also be inside of a macro right now! */

	for (;;) {
		tpp_errno error;
		/* Yield to the first parameter (or just straight to the trailing LF) */
		do {
			tok = tpp_lexer_yield_blocking(lexer);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			goto err_tok_builder;
		if (!TPP_TOK_ISKEYWORD(tok))
			break;
		error = tpp_embed_builder_handle_param(self, lexer, tok);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto err_tok_builder;
		}
	}
	return TPP_EOK;
err_tok_builder:
	tpp_embed_builder_fini(self);
	return TPP_TOK_ASERR(tok);
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_embed_directive(tpp_lexer *tpp_restrict self,
                                 tpp_file *const _tfapfp_prev) {
	tpp_errno error;
	tpp_embed_builder builder;
	tpp_size rel_filename;
	tpp_char const *directive_start; /* Start of "embed" keyword in source */
	tpp_char const *directive_eol;   /* End of line-feed at end of embed directive (or EOF in containing file) */
	tpp_file *const file = tpp_lexer_getfile(self);


	/* At this point, the lexer looks like this:
	 * >> #embed ["foo.dat" limit(DATA_LIMIT) if_empty(42, 10,) prefix(0xff,) suffix(,)]
	 *     ^    ^
	 * tt_start tt_end
	 *
	 * (Everything in brackets may not have been loaded yet)
	 *
	 * We must now start by pre-loading the remainder of the current line, whilst
	 * ensuring that the start of the directive remains loaded into memory! */
	tpp_assert(file->tf_prev == NULL);
	directive_eol = file->tf_pos;
	file->tf_pos  = file->tf_tpos;
	rel_filename  = (tpp_size)(directive_eol - file->tf_tpos);
	error = tpp_lexer_seek_eol(self, &directive_eol tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
	file->tf_tpos = file->tf_pos;
	file->tf_pos += rel_filename;
	if (TPP_ISERR(error)) {
		tpp_lexer_autopopfile_break(self);
		return TPP_TOK_OFERR(error);
	}

	/* At this point, the lexer looks like this:
	 * tf_tpos  tf_pos
	 *     v    v
	 * >> #embed "foo.dat" limit(DATA_LIMIT) if_empty(42, 10,) prefix(0xff,) suffix(,)
	 * >> content-of-next-line
	 *    ^
	 *    directive_eol */
	directive_start = file->tf_tpos;
	tpp_file_push_eof_and_ifdef(file);
	tpp_file_seteof(file, directive_eol);
	error = tpp_embed_builder_init_parse(&builder, self);

	/* Warn if there's anything after the directive that wasn't parsed */
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	if (!TPP_ISERR(error) && tpp_lexer_gettok(self) != TPP_TOK_EOF) {
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tpp_lexer_gettok(self))) {
			tpp_token_id tok = tpp_lexer_yield_blocking(self);
			if (TPP_TOK_ISERR(tok)) {
				error = TPP_TOK_ASERR(tok);
				break;
			}
		}
		if (!TPP_ISERR(error) && tpp_lexer_gettok(self) != TPP_TOK_EOF)
			error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, "embed");
	}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	tpp_lexer_popallfiles(self);
	tpp_file_pop_eof_and_ifdef(file);
	file->tf_tpos = directive_start;
	file->tf_pos  = directive_eol;
	tpp_lexer_autopopfile_break(self);
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);

	/* Pack the builder into an actual file */
	return tpp_embed_builder_pack_and_pushfile(&builder, self);
}
#endif /* TPP_HAVE_CPP_EMBED */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIGIT_LINE
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_token_id TPPCALL
tpp_lexer_handle_digit_directive(tpp_lexer *tpp_restrict self,
                                 tpp_file *_tfapfp_prev,
                                 tpp_char const *directive_iter) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const textfile = (file->tf_kind != TPP_FILE_KIND_IO &&
	                            file->tf_kind != TPP_FILE_KIND_TEXT)
	                           ? tpp_file_gettextfile(_tfapfp_prev)
	                           : file;
	tpp_char const *directive_start;
	tpp_char const *directive_eol;
	tpp_errno error;
	tpp_size rel_digit_loc;
	tpp_intmax new_linenumber;
	tpp_intvalue temp_intvalue;
	TPP_REF tpp_string *new_filename = NULL;
	tpp_token_id tok;
	directive_start = file->tf_pos;
	file->tf_pos = directive_iter;
	error = tpp_lexer_decodeint(self, &temp_intvalue);
	if (TPP_ISERR(error)) {
err_autopopfile_break_error:
		tpp_file_autopopfile_break(file);
		return TPP_TOK_OFERR(error);
	}
	error = tpp_intvalue_asintmax(&temp_intvalue, &new_linenumber);
	tpp_intvalue_fini(&temp_intvalue);
	if (TPP_ISERR(error)) {
#if TPP_INTVALUE_ASINTMAX_CANOVERFLOW
		if (error == TPP_ENOENT) {
			new_linenumber = -1; /* Undefined... */
		} else
#endif /* TPP_INTVALUE_ASINTMAX_CANOVERFLOW */
		{
			goto err_autopopfile_break_error;
		}
	}

	/* Load the remainder of the directive. */
	tpp_assert(file->tf_prev == NULL);
	directive_eol = file->tf_pos;
	file->tf_pos  = directive_start;
	rel_digit_loc = (tpp_size)(directive_eol - directive_start);
	error = tpp_lexer_seek_eol(self, &directive_eol tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
	file->tf_tpos = file->tf_pos;
	file->tf_pos += rel_digit_loc;
	if (TPP_ISERR(error))
		goto err_autopopfile_break_error;
	directive_start = file->tf_tpos; /* Location used by "tpp_file_pushdummy()", and used during rollback */
	tpp_file_push_eof_and_ifdef(file);
	tpp_file_seteof(file, directive_eol);
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok)) {
err_tok_rollback:
		tpp_lexer_popallfiles(self);
		tpp_file_break_eof_and_ifdef(file);
		file->tf_pos = directive_start;
		tpp_file_autopopfile_break(file);
		return tok;
	}

	/* If there's a string, use it as the new filename */
	if (TPP_TOK_ISSTRING(tok)) {
		error = tpp_lexer_parsestring(self, &new_filename, TPP_LEXER_PARSESTRING_FLAG_STOPONLF);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto err_tok_rollback;
		}
		tok = tpp_lexer_gettok(self);
	}

	/* Process flags. */
#if TPP_HAVE_FILE_SYSHDR || TPP_HAVE_FILE_EXTERN_C
	if (textfile) {
#if TPP_HAVE_FILE_SYSHDR
		textfile->tf_flags &= ~TPP_FILE_FLAGS_SYSHDR; /* Must be set anew every time */
#endif /* TPP_HAVE_FILE_SYSHDR */
#if TPP_HAVE_FILE_EXTERN_C
		textfile->tf_flags &= ~TPP_FILE_FLAGS_EXTERN_C; /* Must be set anew every time */
#endif /* TPP_HAVE_FILE_EXTERN_C */
	}
#endif /* TPP_HAVE_FILE_SYSHDR || TPP_HAVE_FILE_EXTERN_C */

	for (;;) {
		tpp_intmax user_flag;
		while (TPP_TOK_ISSPACE_OR_COMMENT(tok))
			tok = tpp_lexer_yield_blocking(self);
		if (!TPP_TOK_ISINT(tok))
			break;
		error = tpp_lexer_decodeint(self, &temp_intvalue);
		if (TPP_ISERR(error)) {
err_error_rollback_new_filename:
			tok = TPP_TOK_OFERR(error);
			goto err_tok_rollback_new_filename;
		}
		error = tpp_intvalue_asintmax(&temp_intvalue, &user_flag);
		tpp_intvalue_fini(&temp_intvalue);
		if (TPP_ISERR(error)) {
#if TPP_INTVALUE_ASINTMAX_CANOVERFLOW
			if (error == TPP_ENOENT) {
				user_flag = -1; /* Undefined... */
			} else
#endif /* TPP_INTVALUE_ASINTMAX_CANOVERFLOW */
			{
				goto err_error_rollback_new_filename;
			}
		}
		switch (user_flag) {

		case 1:
#if TPP_HAVE_FILE_DUMMY
			if (textfile) {
				file->tf_tpos = directive_start;  /* In case "textfile == file" */
				tpp_file_autopopfile_break(file); /* *ditto* */
				error = tpp_file_pushdummy(textfile, textfile->tf_tpos);
				_tfapfp_prev  = file->tf_prev; /* Undo actions from above */
				file->tf_prev = NULL;          /* *ditto* */
				if (TPP_ISERR(error))
					goto err_error_rollback_new_filename;
			}
#endif /* TPP_HAVE_FILE_DUMMY */
			break;

		case 2:
#if TPP_HAVE_FILE_DUMMY
			if (textfile) {
				tpp_file_autopopfile_break(file); /* In case "textfile == file" */
				tpp_file_popdummy(textfile);
				_tfapfp_prev  = file->tf_prev; /* Undo actions from above */
				file->tf_prev = NULL;          /* *ditto* */
			}
#endif /* TPP_HAVE_FILE_DUMMY */
			break;

		case 3:
#if TPP_HAVE_FILE_SYSHDR
			if (textfile)
				textfile->tf_flags |= TPP_FILE_FLAGS_SYSHDR;
#endif /* TPP_HAVE_FILE_SYSHDR */
			break;

		case 4:
#if TPP_HAVE_FILE_EXTERN_C
			if (textfile)
				textfile->tf_flags |= TPP_FILE_FLAGS_EXTERN_C;
#endif /* TPP_HAVE_FILE_EXTERN_C */
			break;

		default:
			/* XXX: Warning? */
			break;
		}
		tok = tpp_lexer_yield_blocking(self);
	}
	if (TPP_TOK_ISERR(tok)) {
err_tok_rollback_new_filename:
		if (new_filename)
			tpp_string_decref(new_filename);
		goto err_tok_rollback;
	}
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	if (tok != TPP_TOK_EOF) {
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			tok = tpp_lexer_yield_blocking(self);
			if (TPP_TOK_ISERR(tok))
				goto err_tok_rollback_new_filename;
		}
		if (tok != TPP_TOK_EOF) {
			error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, "<linenum>");
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				goto err_tok_rollback_new_filename;
			}
		}
	}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */

	tpp_lexer_popallfiles(self);
	tpp_file_pop_eof_and_ifdef(file);
	file->tf_pos = directive_eol;
	tpp_file_autopopfile_break(file);

#if TPP_HAVE_LEXER_USERPWD
	if (new_filename &&
	    tpp_string_len(new_filename) >= 2 &&
	    tpp_string_str(new_filename)[tpp_string_len(new_filename) - 1] == '/' &&
	    tpp_string_str(new_filename)[tpp_string_len(new_filename) - 2] == '/') {
		/* Special case: given `new_filename` is actually a new `tpp_lexer_setuserpwd()` */
		if (!tpp_string_isshared(new_filename)) {
			new_filename->ts_len -= 2;
			new_filename->ts_str[new_filename->ts_len] = '\0';
		} else {
			TPP_REF tpp_string *trimmed_filename;
			tpp_size len = tpp_string_len(new_filename) - 2;
			trimmed_filename = tpp_string_malloc(len);
			if tpp_unlikely(!trimmed_filename) {
				tpp_string_decref(new_filename);
				return TPP_TOK_ENOMEM;
			}
			tpp_memcpy(tpp_string_str(trimmed_filename),
			           tpp_string_str(new_filename),
			           len * sizeof(tpp_char));
			tpp_string_decref(new_filename);
			new_filename = trimmed_filename;
		}
		tpp_lexer_setuserpwd_inherited(self, new_filename);
		new_filename = NULL;
	}
#endif /* TPP_HAVE_LEXER_USERPWD */

	if (textfile) {
		/* Apply line number override (at directive EOL)
		 * If "textfile != file", then the file's current position should point
		 * at wherever it will parse its next token, which should be the start
		 * of the next effective line (this can happen e.g. if we get here from
		 * within a "#pragma tpp_exec()" directive) */
		--new_linenumber; /* "tpp_file_setline()" is 0-based, but user-supplied number is 1-based */
		tpp_file_setline(textfile, tpp_file_getpos(textfile), (tpp_line)new_linenumber);

		/* Apply filename override */
		if (new_filename != NULL)
			tpp_file_setfilename(textfile, new_filename);
	}
	if (new_filename)
		tpp_string_decref(new_filename);
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_CPP_DIGIT_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_LINE
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_line_directive(tpp_lexer *tpp_restrict self,
                                tpp_file *const prev_file) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const textfile = (file->tf_kind != TPP_FILE_KIND_IO &&
	                            file->tf_kind != TPP_FILE_KIND_TEXT)
	                           ? tpp_file_gettextfile(prev_file)
	                           : file;
	tpp_char const *directive_eol;
	tpp_errno error;
	tpp_intmax new_linenumber;
	TPP_REF tpp_string *new_filename = NULL;
	tpp_token_id tok;

	/* Load the remainder of the directive. */
	tpp_assert(file->tf_prev == NULL);
	directive_eol = file->tf_pos;
	error = tpp_lexer_seek_eol(self, &directive_eol tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tpp_file_push_eof_and_ifdef(file);
	tpp_file_seteof(file, directive_eol);

	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok)) {
err_tok_rollback:
		tpp_lexer_popallfiles(self);
		tpp_file_break_eof_and_ifdef(file);
		return tok;
	}

#if TPP_HAVE_TOK_INT
	if (TPP_TOK_ISINT(tok)) {
		/* Decode line number */
		tpp_intvalue new_linenumber_expr;
		error = tpp_lexer_decodeint(self, &new_linenumber_expr);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto err_tok_rollback;
		}
		error = tpp_intvalue_asintmax(&new_linenumber_expr, &new_linenumber);
		tpp_intvalue_fini(&new_linenumber_expr);
		if (TPP_ISERR(error)) {
#if TPP_INTVALUE_ASINTMAX_CANOVERFLOW
			if (error == TPP_ENOENT) {
				new_linenumber = -1; /* Undefined... */
			} else
#endif /* TPP_INTVALUE_ASINTMAX_CANOVERFLOW */
			{
				tok = TPP_TOK_OFERR(error);
				goto err_tok_rollback;
			}
		}
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			goto err_tok_rollback;
	} else
#endif /* TPP_HAVE_TOK_INT */
	{
		tpp_lcinfo lc = tpp_file_getlcinfo(file, file->tf_tpos);
		new_linenumber = tpp_lcinfo_getline(lc) - 1;
#if TPP_HAVE_TPP_W_EXPECTED_INT_AFTER_LINE_DIRECTIVE
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_INT_AFTER_LINE_DIRECTIVE);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto err_tok_rollback;
		}
#endif /* TPP_HAVE_TPP_W_EXPECTED_INT_AFTER_LINE_DIRECTIVE */
	}

	/* If there's a string, use it as the new filename */
	if (TPP_TOK_ISSTRING(tok)) {
		error = tpp_lexer_parsestring(self, &new_filename, TPP_LEXER_PARSESTRING_FLAG_STOPONLF);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto err_tok_rollback;
		}
		tok = tpp_lexer_gettok(self);
	}

	/* Warn about extra tokens after the #line-directive */
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	while (TPP_TOK_ISSPACE_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		if (TPP_TOK_ISERR(tok))
			goto err_tok_rollback;
		error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, "line");
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto err_tok_rollback;
		}
	}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */

	tpp_lexer_popallfiles(self);
	tpp_file_pop_eof_and_ifdef(file);
	file->tf_pos = directive_eol;

	if (textfile) {
		/* Apply line number override (at directive EOL)
		 * If "textfile != file", then the file's current position should point
		 * at wherever it will parse its next token, which should be the start
		 * of the next effective line (this can happen e.g. if we get here from
		 * within a "#pragma tpp_exec()" directive) */
		--new_linenumber; /* "tpp_file_setline()" is 0-based, but user-supplied number is 1-based */
		tpp_file_setline(textfile, tpp_file_getpos(textfile), (tpp_line)new_linenumber);

		/* Apply filename override */
		if (new_filename != NULL)
			tpp_file_setfilename(textfile, new_filename);
	}
	if (new_filename)
		tpp_string_decref(new_filename);
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_CPP_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IDENT_SCCS
#if TPP_HAVE_IDENT_SCCS_HOOK
struct tpp_lexer_handle_ident_sccs_directive_data {
	tpp_lexer   *tlhisdd_lexer;
	tpp_token_id tlhisdd_mode;
};
static tpp_errno TPPCALL
tpp_lexer_handle_ident_sccs_directive_cb(void *arg, tpp_string *chunk,
                                         tpp_char const *str, tpp_size length) {
	struct tpp_lexer_handle_ident_sccs_directive_data *data;
	data = (struct tpp_lexer_handle_ident_sccs_directive_data *)arg;
	tpp_assert(data->tlhisdd_mode == TPP_KWD_ident ||
	           data->tlhisdd_mode == TPP_KWD_sccs);
	(void)chunk;
	(void)str;
	(void)length;
	return tpp_lexer_callhook_ident_sccs(data->tlhisdd_lexer,
	                                     data->tlhisdd_mode,
	                                     chunk, str, length);
}
#endif /* TPP_HAVE_IDENT_SCCS_HOOK */

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_ident_sccs_directive(tpp_lexer *tpp_restrict self) {
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	char const *const directive_name = tpp_lexer_gettokenkwdcstr(self);
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	tpp_token_id tok;
	tpp_errno error;
#if TPP_HAVE_IDENT_SCCS_HOOK
	struct tpp_lexer_handle_ident_sccs_directive_data data;
	data.tlhisdd_lexer = self;
	data.tlhisdd_mode  = tpp_lexer_gettok(self);
#endif /* TPP_HAVE_IDENT_SCCS_HOOK */
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Apply filename override if one is defined */
	if (TPP_TOK_ISSTRING(tok)) {
#if TPP_HAVE_IDENT_SCCS_HOOK
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_ident_sccs_directive_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_STOPONLF);
#else /* TPP_HAVE_IDENT_SCCS_HOOK */
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok) || TPP_TOK_ISSTRING(tok));
		error = TPP_TOK_ASERR_OR_EOK(tok);
#endif /* !TPP_HAVE_IDENT_SCCS_HOOK */
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tok = tpp_lexer_gettok(self);

	/* Warn about extra tokens after the #ident/#sccs-directive */
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	while (TPP_TOK_ISSPACE_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		if (TPP_TOK_ISERR(tok))
			return tok;
		error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, directive_name);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		do {
			tok = tpp_lexer_yield_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return tok;
		} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));
	}
#else /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
		tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
	}
#endif /* !TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_CPP_IDENT_SCCS */
/************************************************************************/




/* Process a preprocessor directive, with the currently loaded token being the leading '#'
 * Upon successful return (!TPP_TOK_ISERR(return)), the caller will yield another raw token
 * @return: TPP_TOK_ISERR         : Error
 * @return: TPP_TOK_EOF           : Caller should yield the next raw token
 * @return: TPP_TOK_SHELL_COMMENT : Directive was transformed to a shell-comment which the caller should re-emit
 * @return: TPP_TOK_SOL_SHELL_COMMENT: Like `TPP_TOK_SHELL_COMMENT` */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_directive(tpp_lexer *tpp_restrict self) {
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#define tpp_lexer_process_directive_set_noguard() (file->tf_flags |= TPP_FILE_FLAGS_NOGUARD)
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#define tpp_lexer_process_directive_set_noguard() (void)0
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *directive_iter = token->tt_end;
	tpp_token_id result;
	tpp_assert(token->tt_id == '#');

	/* Make sure that the start of the directive (the #-token) remains loaded. */
	file->tf_pos = token->tt_start;

	/* Prevent the directive from continuing into another file */
	tpp_lexer_autopopfile_pushoff(self);

	/* Load token that comes after leading '#' */
again_yield_directive_iter:
	result = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
	switch (result) {

	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		/* Skip over multi-line comment tokens to allow code like:
		 * >> #(*
		 * >> comment
		 * >> *)define foo 42 */
	case TPP_TOK_SPACE:
		goto again_yield_directive_iter;

/************************************************************************/
#if TPP_HAVE_CPP_BLANK
	TPP_CASE_TPP_TOK_COMMENT_LINE
		/* line-like comment tokens must be treated as the "blank" directive,
		 * since they effective cap-off the directive via a commented line-feed */
	case TPP_TOK_EOF:
	case TPP_TOK_LF:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_BLANK)
		if (!tpp_lexer_has(self, CPP_BLANK))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_BLANK) */
		file->tf_pos = directive_iter; /* Tell caller to continue parsing *after* linefeed */
		result = TPP_TOK_EOF;
		break;
#endif /* TPP_HAVE_CPP_BLANK */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EXCLAIM
	case '!':
	TPP_CASE_TPP_TOK_MC_STARTSWITH_EXCLAIM
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_EXCLAIM)
		if (!tpp_lexer_has(self, CPP_EXCLAIM))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_EXCLAIM) */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_EXCLAIM */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIGIT_LINE
	TPP_CASE_TPP_TOK_INT
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_DIGIT_LINE)
		if (!tpp_lexer_has(self, CPP_DIGIT_LINE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_DIGIT_LINE) */
		tpp_lexer_process_directive_set_noguard();
		return tpp_lexer_handle_digit_directive(self, _tfapfp_prev, directive_iter);
#endif /* TPP_HAVE_CPP_DIGIT_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_LINE
	case TPP_KWD_line:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_LINE)
		if (!tpp_lexer_has(self, CPP_LINE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_LINE) */
		tpp_lexer_process_directive_set_noguard();
		file->tf_pos = directive_iter;
		result = tpp_lexer_handle_line_directive(self, _tfapfp_prev);
		break;
#endif /* TPP_HAVE_CPP_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_INCLUDE
	case TPP_KWD_include: {
#if TPP_HAVE_LEXER_OPENFILE_EX
		tpp_lexer_openfile_flags flags;
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_INCLUDE)
		if (!tpp_lexer_has(self, CPP_INCLUDE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_INCLUDE) */
		tpp_lexer_process_directive_set_noguard();
		file->tf_pos = directive_iter;
#if TPP_HAVE_LEXER_OPENFILE_EX
		flags = TPP_LEXER_OPENFILE_FLAG_CHECK_LIMIT | /* Check include limit */
		        TPP_LEXER_OPENFILE_FLAG_WARN_CASING;  /* Warn about bad casing */
#if TPP_HAVE_PRAGMA_ONCE
		flags |= TPP_LEXER_OPENFILE_FLAG_HDR_ONCE; /* Mask if header has "#pragma once" */
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
		flags |= TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED; /* Mask if header has an active "#ifndef" guard */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
		return tpp_lexer_handle_include_directive(self, _tfapfp_prev, flags, TPP_HOOK_INCLUDE_KIND_INCLUDE);
	}	break;
#endif /* TPP_HAVE_CPP_INCLUDE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_INCLUDE_NEXT
	case TPP_KWD_include_next: {
		tpp_lexer_openfile_flags flags;
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_INCLUDE_NEXT)
		if (!tpp_lexer_has(self, CPP_INCLUDE_NEXT))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_INCLUDE_NEXT) */
		tpp_lexer_process_directive_set_noguard();
		file->tf_pos = directive_iter;
		flags = TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT | /* Use #include_next-semantics */
		        TPP_LEXER_OPENFILE_FLAG_CHECK_LIMIT |  /* Check include limit */
		        TPP_LEXER_OPENFILE_FLAG_WARN_CASING;   /* Warn about bad casing */
#if TPP_HAVE_PRAGMA_ONCE
		flags |= TPP_LEXER_OPENFILE_FLAG_HDR_ONCE; /* Mask if header has "#pragma once" */
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
		flags |= TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED; /* Mask if header has an active "#ifndef" guard */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
		return tpp_lexer_handle_include_directive(self, _tfapfp_prev, flags, TPP_HOOK_INCLUDE_KIND_INCLUDE_NEXT);
	}	break;
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IMPORT
	case TPP_KWD_import: {
		tpp_lexer_openfile_flags flags;
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IMPORT)
		if (!tpp_lexer_has(self, CPP_IMPORT))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IMPORT) */
		tpp_lexer_process_directive_set_noguard();
		file->tf_pos = directive_iter;
		flags = TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED | /* Use #import-semantics */
		        TPP_LEXER_OPENFILE_FLAG_CHECK_LIMIT |  /* Check include limit */
		        TPP_LEXER_OPENFILE_FLAG_WARN_CASING;   /* Warn about bad casing */
#if TPP_HAVE_PRAGMA_ONCE
		flags |= TPP_LEXER_OPENFILE_FLAG_HDR_ONCE; /* Mask if header has "#pragma once" */
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
		flags |= TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED; /* Mask if header has an active "#ifndef" guard */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
		return tpp_lexer_handle_include_directive(self, _tfapfp_prev, flags, TPP_HOOK_INCLUDE_KIND_IMPORT);
	}	break;
#endif /* TPP_HAVE_CPP_IMPORT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EMBED
	case TPP_KWD_embed:
		/* #embed  (https://en.cppreference.com/c/preprocessor/embed) */
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_EMBED)
		if (!tpp_lexer_has(self, CPP_EMBED))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_EMBED) */
		tpp_lexer_process_directive_set_noguard();
		file->tf_pos = directive_iter;
		return tpp_lexer_handle_embed_directive(self, _tfapfp_prev);
#endif /* TPP_HAVE_CPP_EMBED */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
	case TPP_KWD_ifndef:
	case TPP_KWD_ifdef:
	case TPP_KWD_if:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IF_ELSE_ENDIF)
		if (!tpp_lexer_has(self, CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IF_ELSE_ENDIF) */
		file->tf_pos = directive_iter;
		result = tpp_lexer_handle_if_directive(self);
		tpp_lexer_process_directive_set_noguard(); /* Important! Set the NOGUARD flag *afterwards* */
		break;

	case TPP_KWD_elif:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
	case TPP_KWD_else:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IF_ELSE_ENDIF)
		if (!tpp_lexer_has(self, CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IF_ELSE_ENDIF) */
		tpp_lexer_process_directive_set_noguard();
		file->tf_pos = directive_iter;
		result = tpp_lexer_handle_else_directive(self);
		break;

	case TPP_KWD_endif:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IF_ELSE_ENDIF)
		if (!tpp_lexer_has(self, CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IF_ELSE_ENDIF) */
		/*tpp_lexer_process_directive_set_noguard();*/ /* Not needed... */
		file->tf_pos = directive_iter;
		result = tpp_lexer_handle_endif_directive(self);
		break;
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DEFINE
	case TPP_KWD_define:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_DEFINE)
		if (!tpp_lexer_has(self, CPP_DEFINE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_DEFINE) */
		tpp_lexer_process_directive_set_noguard();
		token->tt_end = directive_iter;
		result = tpp_lexer_handle_define_directive(self);
		break;

	case TPP_KWD_undef:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_DEFINE)
		if (!tpp_lexer_has(self, CPP_DEFINE))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_DEFINE) */
		tpp_lexer_process_directive_set_noguard();
		token->tt_end = directive_iter;
		result = tpp_lexer_handle_undef_directive(self);
		break;
#endif /* TPP_HAVE_CPP_DEFINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ASSERT
	case TPP_KWD_assert:
	case TPP_KWD_unassert:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_ASSERT)
		if (!tpp_lexer_has(self, CPP_ASSERT))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_ASSERT) */
		tpp_lexer_process_directive_set_noguard();
		token->tt_end = directive_iter;
		result = tpp_lexer_handle_assert_directive(self, result);
		break;
#endif /* TPP_HAVE_CPP_ASSERT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
#if TPP_HAVE_CPP_ERROR
	case TPP_KWD_error:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_ERROR)
		if (!tpp_lexer_has(self, CPP_ERROR))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_ERROR) */
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
		if (0) {
	case TPP_KWD_warning:;
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_WARNING)
			if (!tpp_lexer_has(self, CPP_WARNING))
				goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_WARNING) */
		}
#endif /* TPP_HAVE_CPP_WARNING */
		tpp_lexer_process_directive_set_noguard();
		result = tpp_lexer_handle_error_directive(self, directive_iter);
		break;
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IDENT_SCCS
	case TPP_KWD_ident:
	case TPP_KWD_sccs:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IDENT_SCCS)
		if (!tpp_lexer_has(self, CPP_IDENT_SCCS))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_IDENT_SCCS) */
		tpp_lexer_process_directive_set_noguard();
		token->tt_end = directive_iter;
		result = tpp_lexer_handle_ident_sccs_directive(self);
		break;
#endif /* TPP_HAVE_CPP_IDENT_SCCS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_PRAGMA
	case TPP_KWD_pragma:
#if TPP_CONF_MAYBE_0(TPP_HAVE_CPP_PRAGMA)
		if (!tpp_lexer_has(self, CPP_PRAGMA))
			goto handle_unknown_directive;
#define WANT_handle_unknown_directive
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_CPP_PRAGMA) */
		tpp_lexer_process_directive_set_noguard();
		token->tt_end = directive_iter;
		result = tpp_lexer_handle_pragma_directive(self);
		break;
#endif /* TPP_HAVE_CPP_PRAGMA */
/************************************************************************/



	default:
		if (TPP_TOK_ISERR(result))
			goto return_result;
#ifdef WANT_handle_unknown_directive
#undef WANT_handle_unknown_directive
handle_unknown_directive:
#endif /* WANT_handle_unknown_directive */
#if TPP_HAVE_TOK_SHELL_COMMENT || TPP_HAVE_TOK_SOL_SHELL_COMMENT
		if ((tpp_lexer_has(self, TOK_SHELL_COMMENT) ||
		     tpp_lexer_has(self, TOK_SOL_SHELL_COMMENT))
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
			/* "file->tf_pos" was saved as the start of the '#' (or
			 * the line itself in case of "TPP_TOK_SOL_SHELL_COMMENT") */
			token->tt_start = eol = file->tf_pos;
#if TPP_HAVE_TOK_SOL_SHELL_COMMENT
			error = tpp_lexer_seek_eol(self, &eol tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SOL_SHELL_COMMENT));
#else /* TPP_HAVE_TOK_SOL_SHELL_COMMENT */
			error = tpp_lexer_seek_eol(self, &eol tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SHELL_COMMENT));
#endif /* !TPP_HAVE_TOK_SOL_SHELL_COMMENT */
			if (TPP_ISERR(error)) {
				token->tt_start = file->tf_pos;
				token->tt_end = file->tf_pos + 1;
#if TPP_HAVE_TRIGRAPHS
				if (*file->tf_pos == '?') {
					token->tt_end += 2;
				} else
#endif /* TPP_HAVE_TRIGRAPHS */
				{
				}
				token->tt_id = TPP_TOK_OFCHAR('#');
				file->tf_flags &= ~TPP_FILE_FLAGS_NODIRECTIVES;
				result = TPP_TOK_OFERR(error);
				goto return_result;
			}

			/* Tell caller to re-emit what we thought was a directive as a shell comment */
#if TPP_HAVE_TOK_SHELL_COMMENT && TPP_HAVE_TOK_SOL_SHELL_COMMENT
			result = tpp_lexer_has(self, TOK_SOL_SHELL_COMMENT)
			         ? TPP_TOK_SOL_SHELL_COMMENT
			         : TPP_TOK_SHELL_COMMENT;
#elif TPP_HAVE_TOK_SOL_SHELL_COMMENT
			result = TPP_TOK_SOL_SHELL_COMMENT;
#else /* ... */
			result = TPP_TOK_SHELL_COMMENT;
#endif /* !... */

			token->tt_end = eol;
			token->tt_id = result;
			goto return_result;
		} else
#endif /* TPP_HAVE_TOK_SHELL_COMMENT || TPP_HAVE_TOK_SOL_SHELL_COMMENT */
		{
			tpp_lexer_process_directive_set_noguard();
#if defined(WANT_seek_end_of_line) || (TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SHELL_COMMENT) && TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SOL_SHELL_COMMENT))
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SHELL_COMMENT) && TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SOL_SHELL_COMMENT)
#if TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE
			{
				tpp_errno error;
				tpp_char const *saved_end = token->tt_end;
				token->tt_end = directive_iter;
				error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_DIRECTIVE);
				token->tt_end = saved_end;
				if (TPP_ISERR(error)) {
					result = TPP_TOK_OFERR(error);
					goto return_result;
				}
			}
#endif /* TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE */
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SHELL_COMMENT) && TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SOL_SHELL_COMMENT) */

			/* Seek until we hit LF or EOF. Caller has disabled "autopopfile",
			 * so this'll always stay within the file currently being processed. */
#ifdef WANT_seek_end_of_line
#undef WANT_seek_end_of_line
seek_end_of_line:
#endif /* WANT_seek_end_of_line */
			while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
				result = tpp_lexer_yieldraw_blocking(self);
				if (TPP_TOK_ISERR(result))
					goto return_result;
			}
#endif /* WANT_seek_end_of_line || (TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SHELL_COMMENT) && TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SOL_SHELL_COMMENT)) */
		}
		tpp_lexer_autopopfile_break(self);
		return TPP_TOK_EOF;
	}
return_result:
	tpp_lexer_autopopfile_pop(self);
	return result;
#undef tpp_lexer_process_directive_set_noguard
}
#endif /* TPP_HAVE_CPP_DIRECTIVES */



/* Wrapper around `tpp_lexer_yieldraw()` that filters certain tokens (based on
 * configured features), and implements handling for preprocessor directives,
 * like `#define`, `#include`, etc:
 * - TPP_TOK_LF:      Filtered based on `TPP_HAVE_TOK_LF` / `TPP_FEAT_TPP_TOK_LF`
 * - TPP_TOK_SPACE:   Filtered based on `TPP_HAVE_TOK_SPACE` / `TPP_FEAT_TPP_TOK_SPACE`
 * - TPP_TOK_COMMENT: Filtered based on `TPP_HAVE_TOK_COMMENT` / `TPP_FEAT_TPP_TOK_COMMENT`
 *
 * @return: * :                  The newly read token (after accounting for preprocessor directives)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses `TPP_FILE_FLAGS_NONBLOCK` and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EUSER(*):    User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
	tpp_file *const file = tpp_lexer_getfile(self);
	(void)file;
again:
	result = tpp_lexer_yieldraw(self);
	switch (result) {


/************************************************************************/
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SPACE)
	case TPP_TOK_SPACE:
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
		if (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
			break;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#if TPP_CONF_ISRT(TPP_HAVE_TOK_SPACE)
		if (tpp_lexer_has(self, TOK_SPACE))
			break; /* Enabled */
#endif /* TPP_CONF_ISRT(TPP_HAVE_TOK_SPACE) */
		goto again;
#elif TPP_HAVE_CPP_DIRECTIVES
	case TPP_TOK_SPACE:
		break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TOK_COMMENTLIKE
	_TPP_CASE_TPP_TOK_SOL_SHELL_COMMENT
	_TPP_CASE_TPP_TOK_SHELL_COMMENT
#if (TPP_HAVE_TOK_SHELL_COMMENT || TPP_HAVE_TOK_SOL_SHELL_COMMENT) && TPP_HAVE_CPP_DIRECTIVES
		if (tpp_file_getallowdirectives(file) &&
			tpp_lexer_has(self, CPP_DIRECTIVES)) {
			tpp_token *const token = tpp_lexer_gettoken(self);

			/* Must first emit leading space (if there is any) as a regular token:
			 * >>    #define foo 42
			 *   ^^^^
			 *   When "tpp_lexer_yieldraw()" emitted a TPP_TOK_SOL_SHELL_COMMENT,
			 *   then these 4 space characters will be part of that comment token
			 *   (this is a hard requirement of how SOL comments work). But if the
			 *   user also wants us to emit space tokens, and we were to process
			 *   the directive without this extra step, those 4 spaces would never
			 *   be emitted!
			 *
			 * As a downside to this, if we *don't* end up being able to process
			 * the directive, then we end up emitting the space token incorrectly...
			 *
			 * You know: I don't think this really matters -- let's just emit the
			 *           leading whitespace, if only so once we *do* get around
			 *           to parsing the directive, LC information will point at
			 *           the `#` (or `??=`) character(s), rather than the leading
			 *           whitespace of the line.
			 */
#if (TPP_HAVE_TOK_SOL_SHELL_COMMENT && (TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS || TPP_HAVE_TOK_SPACE))
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SPACE)
			if (0
#if TPP_CONF_ISRT(TPP_HAVE_TOK_SPACE)
			    || tpp_lexer_has(self, TOK_SPACE)
#endif /* TPP_CONF_ISRT(TPP_HAVE_TOK_SPACE) */
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
			    || (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
			    )
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SPACE) */
			{
				if (result == TPP_TOK_SOL_SHELL_COMMENT) {
					tpp_char const *iter = tpp_lexer_gettokenstart(self);
					tpp_char const *end = tpp_lexer_gettokenend(self);
					for (; iter < end; ++iter) {
						tpp_char ch = *iter;
						if (ch == '#')
							break;
#if TPP_HAVE_TRIGRAPHS
						if (ch == '?' && (iter + 2) < end && iter[1] == '?' && iter[2] == '=')
							break;
#endif /* TPP_HAVE_TRIGRAPHS */
					}
					if (iter > tpp_lexer_gettokenstart(self)) {
						token->tt_end = iter;
						token->tt_id = result = TPP_TOK_SPACE;
						break;
					}
				}
			}
#endif /* TPP_HAVE_TOK_SOL_SHELL_COMMENT && (TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS || TPP_HAVE_TOK_SPACE) */

			/* Must re-parse comment as a preprocessor directive instead! */
			token->tt_id = TPP_TOK_OFCHAR('#');
#if TPP_HAVE_TOK_SOL_SHELL_COMMENT
			if (result == TPP_TOK_SOL_SHELL_COMMENT) {
				token->tt_end = tpp_token_sol_shell_find_after_pound(self);
			} else
#endif /* TPP_HAVE_TOK_SOL_SHELL_COMMENT */
			{
				token->tt_end = token->tt_start + 1;
#if TPP_HAVE_TRIGRAPHS
				if (*token->tt_start == '?') {
					token->tt_end += 2;
				} else
#endif /* TPP_HAVE_TRIGRAPHS */
				{
				}
			}

			file->tf_flags |= TPP_FILE_FLAGS_NODIRECTIVES;
			result = tpp_lexer_process_directive(self);
			file->tf_flags &= ~TPP_FILE_FLAGS_NODIRECTIVES;
			if (TPP_TOK_ISERR(result))
				break;
			if (result == TPP_TOK_EOF)
				goto again;

			/* Deal with case where PP-directive wasn't recognized,
			 * and should thus be emitted as shell-comment token. */
			tpp_assert(TPP_TOK_ISSHELLCOMMENT(result));

			/* Fallthru to regular maybe-emit-comment code below... */
		}
		TPP_FALLTHRU
#endif /* TPP_HAVE_TOK_SHELL_COMMENT && TPP_HAVE_CPP_DIRECTIVES */
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_COMMENT) || TPP_HAVE_CPP_DIRECTIVES
		/* All line-style comment tokens, except for
		 * "TPP_TOK_SHELL_COMMENT" + "TPP_TOK_SOL_SHELL_COMMENT" */
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
	_TPP_CASE_TPP_TOK_AT_AT_COMMENT
	_TPP_CASE_TPP_TOK_SLASH_COMMENT
	_TPP_CASE_TPP_TOK_AT_COMMENT
	_TPP_CASE_TPP_TOK_SOL_SLASH_COMMENT
	_TPP_CASE_TPP_TOK_SOL_AT_COMMENT
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TOK_COMMENT) || TPP_HAVE_CPP_DIRECTIVES */
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_COMMENT) /* Never, or conditionally enabled */
#if TPP_HAVE_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		file->tf_flags &= ~TPP_FILE_FLAGS_NODIRECTIVES;
		TPP_FALLTHRU
#endif /* TPP_HAVE_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES */
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
		if (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
			break;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#if TPP_CONF_ISRT(TPP_HAVE_TOK_COMMENT)
		if (tpp_lexer_has(self, TOK_COMMENT))
			break; /* Comments are enabled -> emit to caller */
#endif /* TPP_CONF_ISRT(TPP_HAVE_TOK_COMMENT) */
		goto again;
#elif TPP_HAVE_CPP_DIRECTIVES
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		break;
#endif /* ... */
#endif /* TPP_HAVE_TOK_COMMENTLIKE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIRECTIVES || TPP_CONF_MAYBE_0(TPP_HAVE_TOK_LF) /* Never, or conditionally enabled */
	case TPP_TOK_LF: {
#if TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		file->tf_flags &= ~TPP_FILE_FLAGS_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_LF) /* Never, or conditionally enabled */
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
		if (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
			break;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#if TPP_CONF_ISRT(TPP_HAVE_TOK_LF)
		if (tpp_lexer_has(self, TOK_LF))
			break; /* Enabled */
#endif /* TPP_CONF_ISRT(TPP_HAVE_TOK_LF) */
		goto again;
#else /* TPP_CONF_MAYBE_0(TPP_HAVE_TOK_LF) */
		break;
#endif /* !TPP_CONF_MAYBE_0(TPP_HAVE_TOK_LF) */
	}
#endif /* TPP_HAVE_CPP_DIRECTIVES || TPP_CONF_MAYBE_0(TPP_HAVE_TOK_LF) */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIRECTIVES
	case '#':
		if (!tpp_file_getallowdirectives(file))
			break; /* Not allowed here... */
		if (!tpp_lexer_has(self, CPP_DIRECTIVES))
			break; /* Directives are disabled. */
		file->tf_flags |= TPP_FILE_FLAGS_NODIRECTIVES;
		result = tpp_lexer_process_directive(self);
		file->tf_flags &= ~TPP_FILE_FLAGS_NODIRECTIVES;
		if (TPP_TOK_ISERR(result))
			break;
#if TPP_HAVE_TOK_SHELL_COMMENT || TPP_HAVE_TOK_SOL_SHELL_COMMENT
		if (result != TPP_TOK_EOF) {
			/* Emit as a shell-comment (if enabled). Otherwise, check next raw token. */
			tpp_assert(TPP_TOK_ISSHELLCOMMENT(result));
			if (tpp_lexer_has(self, TOK_COMMENT))
				break;
		}
#endif /* TPP_HAVE_TOK_SHELL_COMMENT || TPP_HAVE_TOK_SOL_SHELL_COMMENT */
		goto again;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
/************************************************************************/



/************************************************************************/
	default:
		/* Remember that we've seen something that will prevent CPP directives */
#if TPP_HAVE_CPP_DIRECTIVES
		file->tf_flags |= TPP_FILE_FLAGS_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
		break;
/************************************************************************/
	}

#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
	/* Remember that #include-guards are no longer possible at this
	 * point (because a relevant token "result" was hit first) */
	file->tf_flags |= TPP_FILE_FLAGS_NOGUARD;
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */

	/* Delete a previously recognized #ifndef-guard keyword if
	 * we're at the top-level #ifdef-block for the current file. */
#if TPP_HAVE_CPP_DIRECTIVES
	tpp_file_maybe_delete_include_guard_keyword(file);
#endif /* TPP_HAVE_CPP_DIRECTIVES */
	return result;
}

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELDPP_C */
