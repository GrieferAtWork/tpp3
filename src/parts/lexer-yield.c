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
#ifndef GUARD_TPP_LEXER_YIELD_C
#define GUARD_TPP_LEXER_YIELD_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "token.h"
#include "features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* Wrapper around `tpp_lexer_yieldpp()' that adds handling for macro expansion.
 * @return: * :               The newly read token (after accounting for macros)
 * @return: TPP_TOK_ISERR(*): Error (s.a. `TPP_TOK_ASERR(return)' and `enum tpp_errno') */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield(tpp_lexer *tpp_restrict self) {
	tpp_token_id result = tpp_lexer_yieldpp(self);
	switch (result) {

	/* TODO */

	default: break;
	}
	return result;
}



#if TPP_HAVE_LEXER_SKIP
/* Check that the currently loaded token is 'tok'. If so, "tpp_lexer_yield" to the
 * next token (which is also returned). Otherwise, trigger 'TPP_W_UNEXPECTED_TOKEN'
 * and (if that warning wasn't fatal), try to seek ahead to see if "tok" can be found
 * somewhere close by (depending on what 'tok' and what was actually loaded on entry) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	if tpp_likely(token->tt_id == tok)
		return tpp_lexer_yield(self);
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN
	{
		tpp_errno error;
		char const *expected = tpp_lexer_reprtokenid(self, tok);
		if tpp_unlikely(!expected)
			expected = "?";
		error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN, expected);
		if (error != TPP_EOK)
			return TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN */
	/* TODO: Try to seek ahead (within the current line) to
	 *       find "tok" when it's (e.g.) a '(' (to deal with
	 *       cases where the user added some extra, unrelated
	 *       tokens before the one we're expecting) */

	return token->tt_id;
}
#endif /* TPP_HAVE_LEXER_SKIP */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELD_C */
