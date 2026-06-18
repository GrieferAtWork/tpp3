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
/* Process a #pragma directive, start at the first token that comes after
 * the leading "#pragma" (i.e.: the first token of the actual directive
 * itself)
 *
 * @return: TPP_EOK:    Success (but there may still be garbage after
 *                      the directive that hasn't been parsed, yet).
 * @return: TPP_ENOENT: Unknown pragma (soft-error; caller should not emit
 *                      "TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE")
 * @return: TPP_E*:     Error */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);

/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_pragma_directive(tpp_lexer *tpp_restrict self) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token_id tok;
	tpp_errno error;
	tpp_char const *eol_start;
	tpp_char const *eol_end;
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
	tpp_file_pusheof(file);
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
#if TPP_HAVE_INCLUDE_STACK
	/* Cleanup files pushed by the expression */
	while (file->tf_prev)
		tpp_lexer_popfile(self);
#endif /* TPP_HAVE_INCLUDE_STACK */

	tpp_file_popeof(file);
	file->tf_pos = eol_end; /* Continue parsing after EOL (comment) */
	return TPP_TOK_OFERR_OR_EOF(error);
}
#endif /* TPP_HAVE_PRAGMA */

#if TPP_HAVE_CPP_DEFINE
/* Handle a "#define" directive, with "self" pointing at the macro's name-keyword
 * @return: TPP_TOK_ISERR: Error
 * @return: TPP_TOK_EOF: Success; caller should yield the next raw token */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_define_directive(tpp_lexer *tpp_restrict self);
#endif /* TPP_HAVE_CPP_DEFINE */


/* Delete "tpp_keyword_misc::tkm_file_guard" for the current file if appropriate
 * - Must be called just before calling "tpp_ifdef_stack_append()", to deal with
 *   the case of a file having multiple top-level #if-blocks (in which case the
 *   file can't have a #ifndef-style #include-guard)
 * - Must also be called when returning a token from tpp_lexer_yieldpp()
 */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
static TPP_NONNULL((1)) void TPPCALL
tpp_file_maybe_delete_include_guard_keyword(tpp_file *tpp_restrict self) {
	if (tpp_ifdef_stack_isempty(tpp_file_getifdef(self)) &&
	    self->tf_kind == TPP_FILE_KIND_IO &&
#if TPP_HAVE_FILE_NOKWD
	    !(self->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
	    self->tf_data.td_io.tff_name != NULL) {
		tpp_keyword *kwd;
		tpp_keyword_misc *misc;
		kwd = (tpp_keyword *)((char const *)self->tf_data.td_io.tff_name -
		                      tpp_offsetof(tpp_keyword, tk_kwd));
		misc = tpp_keyword_getmisc(kwd);
		if (misc && misc->tkm_file_guard &&
		    !(misc->tkm_flags & TPP_KEYWORD_FLAG_HDR_GUARD_VALID))
			misc->tkm_file_guard = NULL;
	}
}
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#define tpp_file_maybe_delete_include_guard_keyword(self) (void)0
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */

#if TPP_HAVE_CPP_IF_ELSE_ENDIF
/* Call with the current token loaded as "if" or "elif"
 * @param: p_directive_start: [out] On success (TPP_EOK or TPP_ENOENT), set
 *                                  to the start of the "if" or "elif" keyword
 * @return: TPP_EOK:    Directive evaluates to "true"
 * @return: TPP_ENOENT: Directive evaluates to "false"
 * @return: * :         Error */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_if_directive(tpp_lexer *tpp_restrict self,
                             tpp_char const **p_directive_start) {
	tpp_errno result;
	tpp_token_id tok;
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *trailing_lf_start;
	tpp_char const *trailing_lf_end;
	tpp_char const *directive_iter;
	tpp_expr_value expr_value;
	tpp_size directive_keyword_len;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	char const *directive_name = (char const *)token->tt_kwd->tk_kwd;
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	directive_iter = file->tf_pos;
	file->tf_pos = token->tt_start; /* Retain start of "if" / "elif" keyword */
	directive_keyword_len = (tpp_size)(directive_iter - file->tf_pos);

	/* Seek end-of-line */
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
		if (TPP_TOK_ISERR(tok)) {
			tpp_assert(TPP_TOK_ASERR(tok) != TPP_ENOENT);
			return TPP_TOK_ASERR(tok);
		}
	} while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok));

	trailing_lf_start = token->tt_start;
	trailing_lf_end   = directive_iter;
	tpp_file_pushifdef(file);
	tpp_file_pusheof(file);
	*p_directive_start = file->tf_pos;     /* Restore to continue pointing at effective start of expression */
	file->tf_end = trailing_lf_start;      /* Mark as EOF */
	file->tf_pos += directive_keyword_len; /* Skip over leading keyword */

	/* Parse expression */
	result = tpp_lexer_parseexpr(self, &expr_value);

	/* Evaluate expression result (and warn about trailing tokens) */
	if (!TPP_ISERR(result)) {
		bool b_expr_value;
		result = tpp_expr_value_asbool(self, &expr_value, &b_expr_value);
		tpp_expr_value_fini(&expr_value);
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		if (!TPP_ISERR(result) && tpp_lexer_gettok(self) != TPP_TOK_EOF)
			result = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, directive_name);
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
		if (!TPP_ISERR(result))
			result = b_expr_value ? TPP_EOK : TPP_ENOENT;
	}

#if TPP_HAVE_INCLUDE_STACK
	/* Cleanup files pushed by the expression */
	while (file->tf_prev)
		tpp_lexer_popfile(self);
#endif /* TPP_HAVE_INCLUDE_STACK */
	file->tf_pos = trailing_lf_end; /* Tell caller to continue parsing *after* EOL */
	tpp_file_popeof(file);
	tpp_file_popifdef(file);
	return result;
}

/* Call with the current token loaded as "ifdef", "ifndef", "elifdef" or "elifndef"
 * @param: p_directive_start: [out] On success (TPP_EOK or TPP_ENOENT), set
 *                                  to the start of the "ifdef", ... keyword
 * @return: TPP_EOK:    Directive evaluates to "true"
 * @return: TPP_ENOENT: Directive evaluates to "false"
 * @return: * :         Error */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_ifdef_directive_ex(tpp_lexer *tpp_restrict self,
                                   tpp_char const **p_directive_start,
                                   tpp_keyword const **p_macro_keyword)
#define tpp_lexer_parse_ifdef_directive(self, p_directive_start) \
	tpp_lexer_parse_ifdef_directive_ex(self, p_directive_start, NULL)
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parse_ifdef_directive(tpp_lexer *tpp_restrict self,
                                tpp_char const **p_directive_start)
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
{
	tpp_errno result;
	tpp_token_id tok;
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_char const *directive_iter;
	tpp_token_id const mode = token->tt_id;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
	char const *directive_name = (char const *)token->tt_kwd->tk_kwd;
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
	bool is_keyword_defined;
	directive_iter = file->tf_pos;
	file->tf_pos = token->tt_start; /* Retain start of "ifdef" / "ifndef" keyword */

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

	if (mode == TPP_KWD_ifndef) {
		/* TODO: Try to register as include-guard for current file */
		/* TODO: -Wheader-guard */
	}

	if (mode == TPP_KWD_ifndef ||
	    mode == TPP_KWD_elifndef)
		is_keyword_defined = !is_keyword_defined;
	return is_keyword_defined ? TPP_EOK : TPP_ENOENT;
}

/* Load the next #ifdef-like directive into "self", and return it.
 * On entry, allowed to be pretty much anywhere (method starts out
 * by seeking the next newline, then scanning for directives from
 * there on...)
 *
 * @return: TPP_KWD_ifdef:    Found an #ifdef-directive (current token points at like "# [ifdef] foo")
 * @return: TPP_KWD_ifndef:   Found an #ifdef-directive (current token points at like "# [ifndef] foo")
 * @return: TPP_KWD_elif:     Found an #ifdef-directive (current token points at like "# [elif] foo")
 * @return: TPP_KWD_elifdef:  Found an #ifdef-directive (current token points at like "# [elifdef] foo")
 * @return: TPP_KWD_elifndef: Found an #ifdef-directive (current token points at like "# [elifndef] foo")
 * @return: TPP_KWD_else:     Found an #ifdef-directive (current token points at like "# [else]")
 * @return: TPP_KWD_endif:    Found an #ifdef-directive (current token points at like "# [endif]")
 * @return: TPP_TOK_EOF:      End-of-file (no warning issued, yet)
 * @return: TPP_TOK_ISERR(*): Error
 */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seek_next_ifdef_directive(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok = tpp_lexer_gettok(self);

	/* Seek until next line-feed */
seek_next_lf:
	while (!TPP_TOK_ISLF_OR_COMMENT(tok)) {
		tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok) || tok == TPP_TOK_EOF)
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
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (tok == TPP_TOK_SHELL_COMMENT) {
			tpp_token *const token = tpp_lexer_gettoken(self);
			token->tt_end = token->tt_start + 1;
/*			token->tt_id = tok = TPP_TOK_OFCHAR('#'); * Not needed */
		} else
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		{
			goto seek_next_lf;
		}
	}

	/* Find token that comes after the leading '#'
	 * -> This (may be) the that our caller is interested in. */
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
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_ERROR))
				break;
		}
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
		if (0) {
	case TPP_KWD_warning:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_WARNING))
				break;
		}
#endif /* TPP_HAVE_CPP_WARNING */
		error = tpp_lexer_seek_eol(self, &tpp_lexer_gettoken(self)->tt_end
		                           tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		break;
	}
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */

	default: break;
	}
	goto seek_next_lf;
}

/* Seek end of an inactive "#if 1 ... #else"-style block.
 * - Warn about "#elif" / "#else" directives via "TPP_W_ELIF_OR_ELSE_AFTER_ELSE"
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
		error = tpp_lexer_warnf_lc(self, ifdef_entry->tidse_created, TPP_W_EOF_BEFORE_ENDIF);
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
		temp_entry.tidse_created = tpp_file_lcinfo(file, tpp_lexer_gettokenstart(self));
		temp_entry.tidse_updated = temp_entry.tidse_created;
		error = tpp_lexer_seek_end_of_next_unmatched_endif(self, &temp_entry);
		if (TPP_ISERR(error))
			return error;
	}	break;

	case TPP_KWD_elif:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
	case TPP_KWD_else: {
		tpp_lcinfo ifdef_location = tpp_file_lcinfo(file, tpp_lexer_gettokenstart(self));
#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE
		error = tpp_lexer_warnf(self, TPP_W_ELIF_OR_ELSE_AFTER_ELSE, ifdef_entry,
		                        (char const *)tpp_lexer_gettokenkwd(self)->tk_kwd);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
		ifdef_entry->tidse_updated = ifdef_location;
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

/* Seek end of an inactive "#if 0"-style block.
 * - If a "#else" or "#elif 1"-style block is found, push+create a
 *   new #ifdef-entry using "ifdef_location" as the created-position
 * - If a "#endif" is found, behave like "tpp_lexer_seek_end_of_next_unmatched_endif" */
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
		error = tpp_lexer_warnf_lc(self, ifdef_location, TPP_W_EOF_BEFORE_ENDIF);
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
		temp_entry.tidse_created = tpp_file_lcinfo(file, tpp_lexer_gettokenstart(self));
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
		ifdef_entry->tidse_updated = tpp_file_lcinfo(file, directive_start);
		return TPP_EOK;
	}	break;


	case TPP_KWD_else: {
		tpp_ifdef_stack_entry *ifdef_entry;
		tpp_lcinfo updated_at = tpp_file_lcinfo(file, tpp_lexer_gettokenstart(self));

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
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */


/* Process a preprocessor directive, with the currently loaded token being the leading '#'
 * Upon successful return (!TPP_TOK_ISERR(return)), the caller will yield another raw token
 * @return: TPP_TOK_ISERR         : Error
 * @return: TPP_TOK_EOF           : Caller should yield the next raw token
 * @return: TPP_TOK_SHELL_COMMENT : Directive was transformed to a shell-comment which the caller should re-emit */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_directive(tpp_lexer *tpp_restrict self) {
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#define tpp_lexer_process_directive_set_noguard() (file->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_NOGUARD)
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#define tpp_lexer_process_directive_set_noguard() (void)0
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
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
#if TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	case TPP_TOK_EXCLAIM_EXCLAIM:
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
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
		tpp_lexer_process_directive_set_noguard();

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
		tpp_lexer_process_directive_set_noguard();

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
		tpp_lexer_process_directive_set_noguard();

		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}
#endif /* TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
	{
		tpp_errno error;
		tpp_ifdef_stack_entry *ifdef_entry;
		tpp_char const *directive_start;
	case TPP_KWD_ifndef:
	case TPP_KWD_ifdef:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
handle_pp_ifdef:
		file->tf_pos = directive_iter;
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
		if (token->tt_id == TPP_KWD_ifndef) {
			tpp_keyword const *ifndef_keyword = NULL;
			error = tpp_lexer_parse_ifdef_directive_ex(self, &directive_start, &ifndef_keyword);
			if (!(file->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOGUARD) &&
			    (error == TPP_EOK || error == TPP_ENOENT) && ifndef_keyword != NULL &&
			    file->tf_kind == TPP_FILE_KIND_IO && tpp_ifdef_stack_isempty(tpp_file_getifdef(file)) &&
#if TPP_HAVE_FILE_NOKWD
			    !(file->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
			    file->tf_data.td_io.tff_name != NULL) {
				tpp_keyword *kwd;
				tpp_keyword_misc *misc;
				kwd = (tpp_keyword *)((char const *)file->tf_data.td_io.tff_name -
				                      tpp_offsetof(tpp_keyword, tk_kwd));
				kwd = tpp_keywords_copybuiltin(&self->tl_kwds, kwd);
				if tpp_unlikely(!kwd)
					return TPP_TOK_ENOMEM;
				misc = tpp_keyword_requiremisc(kwd);
				if tpp_unlikely(!misc)
					return TPP_TOK_ENOMEM;
				misc->tkm_file_guard = ifndef_keyword;
				if (error == TPP_ENOENT) {
					/* false-condition -> seek end-of-block */
					tpp_lcinfo created_at = tpp_file_lcinfo(file, directive_start);
					error = tpp_lexer_seek_end_of_inactive_ifdef(self, created_at);
					return TPP_TOK_OFERR_OR_EOF(error);
				}
				ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
				if tpp_unlikely(!ifdef_entry)
					return TPP_TOK_ENOMEM;
				ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_IFDEF;
				ifdef_entry->tidse_created = tpp_file_lcinfo(file, directive_start);
				ifdef_entry->tidse_updated = ifdef_entry->tidse_created;
				return TPP_TOK_EOF;
			}
		} else
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
		{
			error = tpp_lexer_parse_ifdef_directive(self, &directive_start);
		}
		tpp_lexer_process_directive_set_noguard();
		goto handle_pp_if_error;

	case TPP_KWD_if:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		tpp_lexer_process_directive_set_noguard(); /* XXX: Also allow guards for "#if !defined(FOO)" ? */
handle_pp_if:
		/* Evaluate expression */
		file->tf_pos = directive_iter;
		error = tpp_lexer_parse_if_directive(self, &directive_start);
handle_pp_if_error:
		if (error == TPP_ENOENT) {
			/* false-condition -> seek end-of-block */
			tpp_lcinfo created_at = tpp_file_lcinfo(file, directive_start);
			error = tpp_lexer_seek_end_of_inactive_ifdef(self, created_at);
			return TPP_TOK_OFERR_OR_EOF(error);
		}
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		tpp_file_maybe_delete_include_guard_keyword(file);
		ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
		if tpp_unlikely(!ifdef_entry)
			return TPP_TOK_ENOMEM;
		ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_IFDEF;
		ifdef_entry->tidse_created = tpp_file_lcinfo(file, directive_start);
		ifdef_entry->tidse_updated = ifdef_entry->tidse_created;
		return TPP_TOK_EOF;
	}

	case TPP_KWD_elif:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
	case TPP_KWD_else: {
		tpp_errno error;
		tpp_lcinfo lc_update;
		tpp_ifdef_stack_entry *ifdef_entry;
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		tpp_lexer_process_directive_set_noguard();

		/* Check for error-case: #ifdef-stack is empty */
		if (tpp_ifdef_stack_isempty(tpp_file_getifdef(file))) {
#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF
			file->tf_pos = directive_iter;
			error = tpp_lexer_warnf(self, TPP_W_ELIF_OR_ELSE_WITHOUT_IF,
			                        (char const *)token->tt_kwd->tk_kwd);
			file->tf_pos = token->tt_start;
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_WITHOUT_IF */
			if (result == TPP_KWD_elif)
				goto handle_pp_if;
			if (result == TPP_KWD_elifdef)
				goto handle_pp_ifdef;
			if (result == TPP_KWD_elifndef)
				goto handle_pp_ifdef;
			tpp_file_maybe_delete_include_guard_keyword(file);
			ifdef_entry = tpp_ifdef_stack_append(tpp_file_getifdef(file));
			if tpp_unlikely(!ifdef_entry)
				return TPP_TOK_ENOMEM;
			ifdef_entry->tidse_mode    = TPP_IFDEF_MODE_ELSE;
			ifdef_entry->tidse_created = tpp_file_lcinfo(file, file->tf_pos);
			ifdef_entry->tidse_updated = ifdef_entry->tidse_created;
			file->tf_pos = directive_iter;
			goto seek_end_of_line;
#define WANT_seek_end_of_line
		}

		/* Load the most-recent #ifdef-stack entry */
		ifdef_entry = tpp_ifdef_stack_getlast(tpp_file_getifdef(file));
		lc_update = tpp_file_lcinfo(file, file->tf_pos);
#if TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE
		if (ifdef_entry->tidse_mode == TPP_IFDEF_MODE_ELSE) {
			file->tf_pos = directive_iter;
			error = tpp_lexer_warnf_lc(self, lc_update, TPP_W_ELIF_OR_ELSE_AFTER_ELSE, ifdef_entry,
			                           (char const *)token->tt_kwd->tk_kwd);
			file->tf_pos = token->tt_start;
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_ELIF_OR_ELSE_AFTER_ELSE */
		ifdef_entry->tidse_updated = lc_update;

		/* Continue parsing after directive */
		file->tf_pos = directive_iter;

		/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
		if (result == TPP_KWD_else) {
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
	}	break;

	case TPP_KWD_endif: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/*tpp_lexer_process_directive_set_noguard();*/ /* Not needed... */
		file->tf_pos = directive_iter;

		if (tpp_ifdef_stack_isempty(tpp_file_getifdef(file))) {
#if TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_ENDIF_WITHOUT_IF);
			if (TPP_ISERR(error))
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_ENDIF_WITHOUT_IF */
			goto seek_end_of_line;
#define WANT_seek_end_of_line
		}

		/* Remove #ifdef-stack entry */
		tpp_assert(!tpp_ifdef_stack_isempty(tpp_file_getifdef(file)));
		tpp_ifdef_stack_remove(tpp_file_getifdef(file));

		/* Check for -Wendif-labels */
#if TPP_HAVE_TPP_W_ENDIF_LABELS
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
#endif /* TPP_HAVE_TPP_W_ENDIF_LABELS */

		/* Seek end-of-line */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}	break;
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DEFINE
	case TPP_KWD_define: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DEFINE))
			goto handle_unknown_directive;
		tpp_lexer_process_directive_set_noguard();
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
		tpp_lexer_process_directive_set_noguard();
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
		if (tpp_keyword_canundef(ro_keyword)) {
			tpp_keyword *keyword = tpp_keywords_copybuiltin(&self->tl_kwds, ro_keyword);
			if tpp_unlikely(!keyword)
				return TPP_TOK_ENOMEM;
			tpp_assert(tpp_keyword_canundef(keyword));
			tpp_keyword_undef(keyword);
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
		tpp_lexer_process_directive_set_noguard();
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
		tpp_lexer_process_directive_set_noguard();
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
		tpp_lexer_process_directive_set_noguard();
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
		tpp_lexer_process_directive_set_noguard();
		token->tt_end = directive_iter;
		return tpp_lexer_process_pragma_directive(self);
	}	break;
#endif /* TPP_HAVE_CPP_PRAGMA */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EMBED
	case TPP_KWD_embed: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_EMBED))
			goto handle_unknown_directive;
		tpp_lexer_process_directive_set_noguard();
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
			tpp_lexer_process_directive_set_noguard();
#if defined(WANT_seek_end_of_line) || TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0
#if TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE
			{
				tpp_errno error;
				error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_DIRECTIVE);
				if (TPP_ISERR(error))
					return TPP_TOK_OFERR(error);
			}
#endif /* TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE */
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0 */
	
			/* Seek until we hit LF or EOF. Caller has disabled "autopopfile",
			 * so this'll always stay within the file currently being processed. */
#ifdef WANT_seek_end_of_line
#undef WANT_seek_end_of_line
seek_end_of_line:
#endif /* WANT_seek_end_of_line */
			while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(result)) {
				result = tpp_lexer_yieldraw_blocking(self);
				if (TPP_TOK_ISERR(result))
					break;
			}
#endif /* WANT_seek_end_of_line || TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0 */
		}
		break;
	}
	return TPP_TOK_EOF;
#undef tpp_lexer_process_directive_set_noguard
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
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
		if (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
			break;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
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

			tpp_lexer_autopopfile_pushoff(self); /* TODO: #include-directives need to skip this step */
			self->tl_state |= TPP_LEXER_STATE_FLAG_NODIRECTIVES;
			result = tpp_lexer_process_directive(self);
			self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
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
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
		if (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
			break;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
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
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
		if (self->tl_state & TPP_LEXER_STATE_FLAG_ALLTOKENS)
			break;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
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
		tpp_lexer_autopopfile_pushoff(self); /* TODO: #include-directives need to skip this step */
		self->tl_state |= TPP_LEXER_STATE_FLAG_NODIRECTIVES;
		result = tpp_lexer_process_directive(self);
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
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
		/* Remember that we've seen something that will prevent CPP directives */
#if TPP_HAVE_CPP_DIRECTIVES
		self->tl_state |= TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
		break;
/************************************************************************/
	}

#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
	/* Remember that #include-guards are no longer possible at this
	 * point (because a relevant token "result" was hit first) */
	tpp_lexer_getfile(self)->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_NOGUARD;
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */

	/* Delete a previously recognized #ifndef-guard keyword if
	 * we're at the top-level #ifdef-block for the current file. */
	tpp_file_maybe_delete_include_guard_keyword(tpp_lexer_getfile(self));
	return result;
}


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELDPP_C */
