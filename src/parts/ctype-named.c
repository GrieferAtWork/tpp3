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
#ifndef GUARD_TPP_CTYPE_NAMED_C
#define GUARD_TPP_CTYPE_NAMED_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "lexer.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_DECODE_NAMED_ESCAPE

/* Decode a named \N{...} sequence and update `*p_iter` to point to
 * its end. This function implement the routing between the different
 * (currently enabled) named escape decoders, as configured by:
 * - `TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES`
 * - `TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD`
 * - `TPP_HAVE_ESCAPE_NAMED_XML`
 *
 * @return: 0 : Unknown named sequence (`*p_iter` is unchanged) 
 * @return: * : The # of characters written to `result` (always `<= TPP_DECODE_NAMED_ESCAPE_MAXLEN`)*/
#if TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_size TPPCALL
tpp_decode_named_escape(tpp_char const **p_iter, tpp_char const *end,
                        tpp_unichar result[TPP_DECODE_NAMED_ESCAPE_MAXLEN],
                        struct tpp_lexer const *tpp_restrict lexer)
#else /* TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
_tpp_decode_named_escape(tpp_char const **p_iter, tpp_char const *end,
                         tpp_unichar result[TPP_DECODE_NAMED_ESCAPE_MAXLEN])
#endif /* !TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER */
{
	tpp_char const *iter = *p_iter;
	/* TODO */
	(void)iter;
	(void)end;
	(void)result;
#if TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER
	(void)lexer;
#endif /* TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER */
	return 0;
}
#endif /* TPP_HAVE_DECODE_NAMED_ESCAPE */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CTYPE_NAMED_C */
