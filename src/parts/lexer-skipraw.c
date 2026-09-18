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
#ifndef GUARD_TPP_LEXER_SKIPRAW_C
#define GUARD_TPP_LEXER_SKIPRAW_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "file.h"
#include "keyword.h"
#include "macro.h"
#include "lexer.h"
#include "token.h"
#include "features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_TRYSKIP_RAW

/* Check if `tok == expected`, with special handling when
 * `TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS && expected == '<'`,
 * in which case `tok` is allowed to be some other token whose
 * first character is '<'. In that last case, `self` is updated
 * to describe '<' before `true` is returned. */
#if (TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS && \
     TPP_HAVE_TOK_MC_STARTSWITH_LANGLE)
static TPP_WUNUSED TPP_NONNULL((1, 4)) bool TPPCALL
tpp_lexer_istok(tpp_lexer *tpp_restrict self,
                tpp_token_id tok,
                tpp_token_id expected,
                tpp_char const **tpp_restrict p_pos) {
	if (tok == expected)
		return true;
	if (expected == '<') {
		switch (tok) {
		TPP_CASE_TPP_TOK_MC_STARTSWITH_LANGLE
			/* Convert to "<" token */
			tpp_assert(tpp_lexer_gettoken(self)->tt_start < (*p_pos));
			tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '<');
			(*p_pos) = tpp_lexer_gettoken(self)->tt_start + 1;
			tok      = TPP_TOK_OFCHAR('<');
			tpp_lexer_gettoken(self)->tt_id = tok;
			return true;
		default: break;
		}
	}
	return false;
}
#else /* ... */
#define tpp_lexer_istok(self, tok, expected, p_pos) ((tok) == (expected))
#endif /* !... */

/* Make use of:
 * - `tpp_lexer_seek_start()`
 * - `tpp_lexer_yieldraw_at()`
 * - `tpp_lexer_manualpopfile_start(self)`
 * to seek ahead to the next token, skipping whitespace/line-feed (+resp. comments)
 * based on `flags`, check if said *next token* is equal to `expected` (with some extra-
 * extra handling when `TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS && expected == '<'`).
 * If that is the case, commit the lexer such that it points at a token equal to
 * the specified `expected`, truly disposing of any files popped in the mean-time.
 * Otherwise (the next token is `!= expected`), roll back any changes made, such
 * that `self` once again points at the same token it did upon entry. In either
 * case, return the ID of whatever token came next.
 *
 * NOTE: This function automatically handles `TPP_TOK_EWOULDBLOCK` by blocking!
 *
 * @return: * :                The next token (rollback)
 * @return: expected:          The next token (commit; iow: this is now also the current token)
 * @return: TPP_TOK_ENOMEM:    Out of memory
 * @return: TPP_TOK_EIO:       I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR: Lexer error
 * @return: TPP_TOK_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_tryskip_raw(tpp_lexer *tpp_restrict self, tpp_token_id expected,
                      unsigned int flags) {
	tpp_lexer_seek_backup backup;
	tpp_char const *pos;
	tpp_token_id tok;
	pos = tpp_lexer_seek_start(self, &backup);
again_yield_mainfile:
	tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	/* NOTE: No need to check for errors -- TPP_TOK_ISERR(*) fails all the tests
	 *       below, meaning it is implicitly handled as rollback+propagate! */
	if (TPP_TOK_ISSPACE_OR_COMMENT(tok) && !(flags & TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONSPACE))
		goto again_yield_mainfile;
	if (TPP_TOK_ISLF_OR_COMMENT(tok) && !(flags & TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONLF))
		goto again_yield_mainfile;
	if (tpp_lexer_istok(self, tok, expected, &pos)) {
		if (flags & TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV) {
			/* Include previous token, too (HINT: tpp_lexer_seek_start()
			 * saved that token's tart in "tt_end" for the sake of the
			 * lexer's file not unloading that token's data) */
			tpp_token *const token = tpp_lexer_gettoken(self);
			token->tt_start = token->tt_end;
		}
		tpp_lexer_seek_commit(self, pos);
		return expected;
	}
	tpp_lexer_seek_rollback(self, &backup);
#if TPP_HAVE_INCLUDE_STACK
	if (tok == TPP_TOK_EOF) {
		/* Check files further up the `#include`-stack */
		tpp_lexer_manualpopfile_start(self);
		while (tpp_lexer_manualpopfile_canpopfile(self) &&
		       tpp_lexer_getfilekind(self) != TPP_FILE_KIND_IO) {
			tpp_lexer_manualpopfile_popfile(self);
			pos = tpp_lexer_seek_start(self, &backup);
again_yield_nextfile:
			tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
			/* NOTE: No need to check for errors -- TPP_TOK_ISERR(*) fails all the tests
			 *       below, meaning it is implicitly handled as rollback+propagate! */
			if (TPP_TOK_ISSPACE_OR_COMMENT(tok) && !(flags & TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONSPACE))
				goto again_yield_nextfile;
			if (TPP_TOK_ISLF_OR_COMMENT(tok) && !(flags & TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONLF))
				goto again_yield_nextfile;
			if (tpp_lexer_istok(self, tok, expected, &pos)) {
				if (flags & TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV) {
					/* Include previous token, too (HINT: tpp_lexer_seek_start()
					 * saved that token's tart in "tt_end" for the sake of the
					 * lexer's file not unloading that token's data) */
					tpp_token *const token = tpp_lexer_gettoken(self);
					token->tt_start = token->tt_end;
				}
				tpp_lexer_seek_commit(self, pos);
				tpp_lexer_manualpopfile_break_commit(self);
				return expected;
			}
			tpp_lexer_seek_rollback(self, &backup);
			if (tok != TPP_TOK_EOF)
				break;
		}
		tpp_lexer_manualpopfile_end_rollback(self);
	}
#endif /* TPP_HAVE_INCLUDE_STACK */
	return tok;
}
#endif /* TPP_HAVE_LEXER_TRYSKIP_RAW */


#if TPP_HAVE_LEXER_PEEK_RAW
/* Get a peek at upcoming ("raw") tokens. This function correctly
 * handles EOF (by temporarily unwinding the #include-stack, before
 * undoing any such changes prior to returning).
 *
 * NOTE: This function also suppress warnings while peeking tokens!
 *
 * @param: flags:     Set of `TPP_LEXER_PEEK_RAW_FLAG_*`
 * @param: p_token:   When non-NULL, initialize to describe the upcoming
 *                    token (since this function always rolls back, such
 *                    information will not be in `tpp_lexer_gettoken(self)`)
 * @param: accept_cb: When non-NULL, a callback that is invoked while the
 *                    peeked token is loaded into `tpp_lexer_gettoken(self)`.
 *                    This callback can be used to:
 *                    - `return TPP_ENOENT`:   Continue peeking the next token
 *                    - `return TPP_EOK`:      Rewind and return this token
 *                    - `return TPP_ISERR(*)`: Rewind and return this error
 *                    When `NULL` is given, behaves the same as if an accept-
 *                    callback that always returns `return TPP_EOK` was used.
 *
 * @return: * :                The upcoming token (also stored in `tpp_token_getid(p_token)`)
 * @return: TPP_TOK_EOF:       No acceptable upcoming token (also stored in `tpp_token_getid(p_token)`)
 * @return: TPP_TOK_ENOMEM:    Out of memory
 * @return: TPP_TOK_EIO:       I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR: Lexer error
 * @return: TPP_TOK_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_peek_raw(tpp_lexer *tpp_restrict self, unsigned int flags, /*out*/ tpp_token *p_token,
                   tpp_errno (TPPCALL *accept_cb)(void *arg, tpp_lexer *self), void *accept_arg) {
	tpp_lexer_seek_backup backup;
	tpp_char const *pos;
	tpp_token_id tok;
	tpp_lexer_nowarnings_pushon(self);
	pos = tpp_lexer_seek_start(self, &backup);
again_yield_mainfile:
	tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	/* NOTE: No need to check for errors -- TPP_TOK_ISERR(*) fails all the tests
	 *       below, meaning it is implicitly handled as rollback+propagate! */
	if (TPP_TOK_ISSPACE_OR_COMMENT(tok) && !(flags & TPP_LEXER_PEEK_RAW_FLAG_STOPONSPACE))
		goto again_yield_mainfile;
	if (TPP_TOK_ISLF_OR_COMMENT(tok) && !(flags & TPP_LEXER_PEEK_RAW_FLAG_STOPONLF))
		goto again_yield_mainfile;
	if (!TPP_TOK_ISERR_OR_EOF(tok)) {
		tpp_errno accept_error = TPP_EOK;
		if (accept_cb != NULL) {
			tpp_char const *const saved_end = tpp_lexer_gettokenend(self);
			tpp_lexer_gettoken(self)->tt_end = pos;
			accept_error = (*accept_cb)(accept_arg, self);
			tpp_lexer_gettoken(self)->tt_end = saved_end;
		}
		if (accept_error != TPP_ENOENT) {
			if (TPP_ISERR(accept_error)) {
				tok = TPP_TOK_OFERR(accept_error);
			} else if (p_token != NULL) {
				tpp_token_copy(p_token, tpp_lexer_gettoken(self));
				if (flags & TPP_LEXER_PEEK_RAW_FLAG_INCLPREV)
					p_token->tt_start = tpp_lexer_gettokenend(self);
				p_token->tt_end = pos;
			}
			tpp_lexer_seek_rollback(self, &backup);
			goto done;
		}
	}
	tpp_lexer_seek_rollback(self, &backup);
	if (tok == TPP_TOK_EOF) {
#if TPP_HAVE_INCLUDE_STACK
		/* Check files further up the `#include`-stack */
		tpp_lexer_manualpopfile_start(self);
		while (tpp_lexer_manualpopfile_canpopfile(self) &&
		       tpp_lexer_getfilekind(self) != TPP_FILE_KIND_IO) {
			tpp_lexer_manualpopfile_popfile(self);
			pos = tpp_lexer_seek_start(self, &backup);
again_yield_nextfile:
			tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
			/* NOTE: No need to check for errors -- TPP_TOK_ISERR(*) fails all the tests
			 *       below, meaning it is implicitly handled as rollback+propagate! */
			if (TPP_TOK_ISSPACE_OR_COMMENT(tok) && !(flags & TPP_LEXER_PEEK_RAW_FLAG_STOPONSPACE))
				goto again_yield_nextfile;
			if (TPP_TOK_ISLF_OR_COMMENT(tok) && !(flags & TPP_LEXER_PEEK_RAW_FLAG_STOPONLF))
				goto again_yield_nextfile;
			if (!TPP_TOK_ISERR_OR_EOF(tok)) {
				tpp_errno accept_error = TPP_EOK;
				if (accept_cb != NULL) {
					tpp_char const *const saved_end = tpp_lexer_gettokenend(self);
					tpp_lexer_gettoken(self)->tt_end = pos;
					accept_error = (*accept_cb)(accept_arg, self);
					tpp_lexer_gettoken(self)->tt_end = saved_end;
				}
				if (accept_error != TPP_ENOENT) {
					if (TPP_ISERR(accept_error)) {
						tok = TPP_TOK_OFERR(accept_error);
					} else if (p_token != NULL) {
						tpp_token_copy(p_token, tpp_lexer_gettoken(self));
						if (flags & TPP_LEXER_PEEK_RAW_FLAG_INCLPREV)
							p_token->tt_start = tpp_lexer_gettokenend(self);
						p_token->tt_end = pos;
					}
					tpp_lexer_seek_rollback(self, &backup);
					tpp_lexer_manualpopfile_break_commit(self);
					goto done;
				}
			}
			tpp_lexer_seek_rollback(self, &backup);
			if (tok != TPP_TOK_EOF)
				break;
		}
		tpp_lexer_manualpopfile_end_rollback(self);
		if (tok == TPP_TOK_EOF)
#endif /* TPP_HAVE_INCLUDE_STACK */
		{
			/* Special case: must still return a proper token on EOF */
			if (p_token) {
				tpp_file *const file = tpp_lexer_getfile(self);

				p_token->tt_id    = TPP_TOK_EOF;
/*				p_token->tt_kwd   = NULL; */
				p_token->tt_start = tpp_file_getend(file);
				p_token->tt_end   = tpp_file_getend(file);
				if (flags & TPP_LEXER_PEEK_RAW_FLAG_INCLPREV)
					p_token->tt_start = tpp_lexer_gettokenend(self);
				p_token->tt_chunk = tpp_file_getchunk(file);
				if (p_token->tt_chunk)
					tpp_string_incref(p_token->tt_chunk);
			}
		}
	}
done:
	tpp_lexer_nowarnings_pop(self);
	return tok;
}
#endif /* TPP_HAVE_LEXER_PEEK_RAW */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_SKIPRAW_C */
