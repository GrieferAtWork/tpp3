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
#include "extension.h"
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


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELD_C */
