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
#ifndef GUARD_TPP_LEXER_SKIP_C
#define GUARD_TPP_LEXER_SKIP_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "extensions.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "preparse.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_SKIP

static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_token_matches(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token *const token = tpp_lexer_gettoken(self);

	/* Check for simple (expected) case: the current token is correct */
	if tpp_likely(token->tt_id == tok)
		return true;

	/* If "tok" is a single-char token, see if the currently
	 * loaded token is a multi-char token that starts with
	 * the same value. */
	if ((TPP_TOK_ISCHAR(tok)) &&
	    (token->tt_start < token->tt_end) &&
	    (*token->tt_start == (tpp_char)(unsigned int)tok)) {
		token->tt_end = token->tt_start + 1;
		token->tt_id  = tok;
		return true;
	}

	/* Handle stuff like "tok == '>>' && CURRENT_TOKEN == '>>>'", etc. */
	/* XXX: The code here should be generated! */
	switch (token->tt_id) {

#if TPP_HAVE_TOK_DOT_DOT_DOT && TPP_HAVE_TOK_DOT_DOT
	case TPP_TOK_DOT_DOT_DOT: /* "..." */
		if (tok == TPP_TOK_DOT_DOT)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TOK_STAR_STAR_EQUAL && TPP_HAVE_TOK_STAR_STAR
	case TPP_TOK_STAR_STAR_EQUAL: /* "**=" */
		if (tok == TPP_TOK_STAR_STAR)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TOK_SLASH_SLASH_EQUAL && TPP_HAVE_TOK_SLASH_SLASH
	case TPP_TOK_SLASH_SLASH_EQUAL: /* "//=" */
		if (tok == TPP_TOK_SLASH_SLASH)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL && TPP_HAVE_TOK_EXCLAIM_EQUAL
	case TPP_TOK_EXCLAIM_EQUAL_EQUAL: /* "!==" */
		if (tok == TPP_TOK_EXCLAIM_EQUAL)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TOK_EQUAL_STAR_STAR && TPP_HAVE_TOK_EQUAL_STAR
	case TPP_TOK_EQUAL_STAR_STAR: /* "=**" */
		if (tok == TPP_TOK_EQUAL_STAR)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TOK_EQUAL_SLASH_SLASH && TPP_HAVE_TOK_EQUAL_SLASH
	case TPP_TOK_EQUAL_SLASH_SLASH: /* "=//" */
		if (tok == TPP_TOK_EQUAL_SLASH)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if TPP_HAVE_TOK_MINUS_RANGLE_STAR && TPP_HAVE_TOK_MINUS_RANGLE
	case TPP_TOK_MINUS_RANGLE_STAR: /* "->*" */
		if (tok == TPP_TOK_MINUS_RANGLE)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if ((TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL ||    \
      TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM) && \
     TPP_HAVE_TOK_EQUAL_EQUAL)
#if TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL
	case TPP_TOK_EQUAL_EQUAL_EQUAL: /* "===" */
#endif /* TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM
	case TPP_TOK_EQUAL_EQUAL_EXCLAIM: /* "==!" */
#endif /* TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM */
		if (tok == TPP_TOK_EQUAL_EQUAL)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#if ((TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL || \
      TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE ||       \
      TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL) &&       \
     TPP_HAVE_TOK_LANGLE_LANGLE)
#if TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: /* "<<<=" */
#if TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE
		if (tok == TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE)
			goto set_threechar;
#define WANT_set_threechar
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE */
		TPP_FALLTHRU
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE_LANGLE: /* "<<<" */
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_EQUAL: /* "<<=" */
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TOK_LANGLE_LANGLE
		if (tok == TPP_HAVE_TOK_LANGLE_LANGLE)
			goto set_twochar;
#define WANT_set_twochar
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE */
		break;
#elif TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL && TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: /* "<<<=" */
		if (tok == TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE)
			goto set_threechar;
#define WANT_set_threechar
		break;
#endif /* ... */

#if ((TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL || \
      TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE ||       \
      TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL) &&       \
     TPP_HAVE_TOK_RANGLE_RANGLE)
#if TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: /* ">>>=" */
#if TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE
		if (tok == TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE)
			goto set_threechar;
#define WANT_set_threechar
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE */
		TPP_FALLTHRU
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE_RANGLE: /* ">>>" */
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_EQUAL: /* ">>=" */
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_RANGLE
		if (tok == TPP_HAVE_TOK_RANGLE_RANGLE)
			goto set_twochar;
#define WANT_set_twochar
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE */
		break;
#elif TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL && TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: /* ">>>=" */
		if (tok == TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE)
			goto set_threechar;
#define WANT_set_threechar
		break;
#endif /* ... */

#if ((TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE || \
      TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE) &&       \
     TPP_HAVE_TOK_EQUAL_LANGLE)
#if TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE: /* "=<<<" */
#endif /* TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE: /* "=<<" */
#endif /* TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE */
		if (tok == TPP_TOK_EQUAL_LANGLE)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE || \
      TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE) &&       \
     TPP_HAVE_TOK_EQUAL_RANGLE)
#if TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE: /* "=>>>" */
#endif /* TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE: /* "=>>" */
#endif /* TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE */
		if (tok == TPP_TOK_EQUAL_RANGLE)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE ||  \
      TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE) && \
     TPP_HAVE_TOK_LANGLE_EQUAL)
#if TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE
	case TPP_TOK_LANGLE_EQUAL_RANGLE: /* "<=>" */
#endif /* TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE
	case TPP_TOK_LANGLE_EQUAL_LANGLE: /* "<=<" */
#endif /* TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE */
		if (tok == TPP_TOK_LANGLE_EQUAL)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE ||  \
      TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE) && \
     TPP_HAVE_TOK_RANGLE_EQUAL)
#if TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE
	case TPP_TOK_RANGLE_EQUAL_LANGLE: /* ">=<" */
#endif /* TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE
	case TPP_TOK_RANGLE_EQUAL_RANGLE: /* ">=>" */
#endif /* TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE */
		if (tok == TPP_TOK_RANGLE_EQUAL)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TOK_LANGLE_MINUS_RANGLE ||  \
      TPP_HAVE_TOK_LANGLE_MINUS_LANGLE) && \
     TPP_HAVE_TOK_LANGLE_MINUS)
#if TPP_HAVE_TOK_LANGLE_MINUS_RANGLE
	case TPP_TOK_LANGLE_MINUS_RANGLE: /* "<->" */
#endif /* TPP_HAVE_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TOK_LANGLE_MINUS_LANGLE
	case TPP_TOK_LANGLE_MINUS_LANGLE: /* "<-<" */
#endif /* TPP_HAVE_TOK_LANGLE_MINUS_LANGLE */
		if (tok == TPP_TOK_LANGLE_MINUS)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */


#if ((TPP_HAVE_TOK_RANGLE_MINUS_LANGLE ||  \
      TPP_HAVE_TOK_RANGLE_MINUS_RANGLE) && \
     TPP_HAVE_TOK_RANGLE_MINUS)
#if TPP_HAVE_TOK_RANGLE_MINUS_LANGLE
	case TPP_TOK_RANGLE_MINUS_LANGLE: /* ">-<" */
#endif /* TPP_HAVE_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TOK_RANGLE_MINUS_RANGLE
	case TPP_TOK_RANGLE_MINUS_RANGLE: /* ">->" */
#endif /* TPP_HAVE_TOK_RANGLE_MINUS_RANGLE */
		if (tok == TPP_TOK_RANGLE_MINUS)
			goto set_twochar;
#define WANT_set_twochar
		break;
#endif /* ... */

#ifdef WANT_set_twochar
#undef WANT_set_twochar
		{
			tpp_char const *iter;
set_twochar:
			iter = token->tt_start + 1;
			iter = tpp_preparse_skipbse_fwd(self, iter, token->tt_end);
			token->tt_end = iter + 1;
			token->tt_id  = tok;
			return true;
		}
#endif /* WANT_set_twochar */

#ifdef WANT_set_threechar
#undef WANT_set_threechar
		{
			tpp_char const *iter;
set_threechar:
			iter = token->tt_start + 1;
			iter = tpp_preparse_skipbse_fwd(self, iter, token->tt_end);
			iter = iter + 1;
			iter = tpp_preparse_skipbse_fwd(self, iter, token->tt_end);
			token->tt_end = iter + 1;
			token->tt_id  = tok;
			return true;
		}
#endif /* WANT_set_threechar */

	default: break;
	}

	return false;
}

/* Same as `tpp_lexer_skip()`, but don't advance to the next token,
 * except in those cases where the requested `tok` could be found
 * a little further up ahead, and the implementation decided that
 * the tokens that lay in-between should be skipped.
 *
 * @return: * :                 The currently loaded token
 * @return: tok:                Success
 * @return: TPP_TOK_ENOMEM:     Out of memory
 * @return: TPP_TOK_EIO:        I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:  Lexer error
 * @return: TPP_TOK_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_require(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_char const *pos;
	tpp_token_id result;
	tpp_lexer_seek_backup backup;

	/* Check if token matches */
	if (tpp_lexer_token_matches(self, tok))
		return tok;

	/* Emit warning */
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN
	{
		tpp_errno error;
		char const *expected = tpp_lexer_reprtokenid(self, tok);
		if tpp_unlikely(!expected)
			expected = "?";
		error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN, expected);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN */

	/* Start seeking ahead... */
	pos = tpp_lexer_seek_start(self, &backup);

	/* Skip over whitespace */
	result = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(result))
		result = tpp_lexer_yieldraw_at_blocking(self, &pos);
	if (TPP_TOK_ISERR(result))
		goto err_result_rollback;

	/* Check if token matches after whitespace... */
	if (tpp_lexer_token_matches(self, tok)) {
		tpp_lexer_seek_commit(self, pos);
		return tok;
	}

	/* Try to seek ahead to find "tok" when it's (e.g.) a '(' (to
	 * deal with cases where the user added some extra, unrelated
	 * tokens before the one we're expecting)
	 *
	 * - If "tok == ')", find next unmatched ')'
	 * - If "tok == ']", find next unmatched ']'
	 * - If "tok == '}", find next unmatched '}'
	 * - If "tok == '>", find next unmatched '>' */
#if TPP_HAVE_LEXER_MANUALPOPFILE
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	if (tok == ')' || tok == ']' || tok == '}' || tok == '>')
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	if (tok == ')')
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	{
		tpp_token *const token = tpp_lexer_gettoken(self);
		tpp_lexer_arginfo argv[1];
		tpp_size argc = 1;
		token->tt_start = token->tt_end;
		token->tt_end   = pos;
		tpp_lexer_manualpopfile_start(self);
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
		{
			tpp_token_id lparen_kind;
			switch (tok) {
			case ')': lparen_kind = TPP_TOK_OFCHAR('('); break;
			case ']': lparen_kind = TPP_TOK_OFCHAR('['); break;
			case '}': lparen_kind = TPP_TOK_OFCHAR('{'); break;
			case '>': lparen_kind = TPP_TOK_OFCHAR('<'); break;
			default: tpp_unreachable();
			}
			result = tpp_lexer_seekpp_rparen_ex(self, argv, &argc, &pos, NULL,
			                                    TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS |
			                                    TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK |
			                                    TPP_LEXER_SEEK_RPAREN_FLAG_NOWARNEOF,
			                                    lparen_kind);
		}
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
		result = tpp_lexer_seekpp_rparen(self, argv, &argc, &pos, NULL,
		                                 TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS |
		                                 TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK |
		                                 TPP_LEXER_SEEK_RPAREN_FLAG_NOWARNEOF);
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
		if (!TPP_TOK_ISERR(result) && argc)
			tpp_lexer_arginfo_fini(&argv[0]);
		if (result == tok) {
			/* Found it! */
			tpp_lexer_manualpopfile_break_commit(self);
			token->tt_start = token->tt_end - 1;
			return result;
		}
		tpp_lexer_getfile(self)->tf_pos = pos;
		tpp_lexer_manualpopfile_end_rollback(self);
		token->tt_end   = token->tt_start + backup.tlsb_len;
		token->tt_id    = backup.tlsb_id;
		token->tt_kwd   = backup.tlsb_kwd;
		if (!TPP_TOK_ISERR(result))
			result = backup.tlsb_id;
		return result;
	}
#endif /* TPP_HAVE_LEXER_MANUALPOPFILE */

	if (tok == ',') {
		/* XXX: Seek to the next ','-token, so-long as no unmatched ) ] } or > is found first */
	}

err_result_rollback:
	tpp_lexer_seek_rollback(self, &backup);
	return result;
}

/* Check that the currently loaded token is `tok`. If so, `tpp_lexer_yield_blocking()` to
 * the next token (which is also returned). Otherwise, trigger 'TPP_W_UNEXPECTED_TOKEN'
 * and (if that warning wasn't fatal), try to seek ahead to see if `tok` can be found
 * somewhere close by (depending on what `tok` and what was actually loaded on entry)
 *
 * NOTE: This function automatically handles `TPP_TOK_EWOULDBLOCK`
 *
 * @return: * :                 The token that comes after the one that was just skipped
 * @return: TPP_TOK_ENOMEM:     Out of memory
 * @return: TPP_TOK_EIO:        I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:  Lexer error
 * @return: TPP_TOK_EWARNPRINT: Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token_id result = tpp_lexer_require(self, tok);
	if (result == tok)
		result = tpp_lexer_yield_blocking(self);
	return result;
}

#endif /* TPP_HAVE_LEXER_SKIP */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_SKIP_C */
