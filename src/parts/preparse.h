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
#ifndef GUARD_TPP_PREPARSE_H
#define GUARD_TPP_PREPARSE_H 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "file.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#undef _TPP_HAVE_BSE_FILE_PARAM
#if (TPP_HAVE_BSE && (TPP_HAVE_UNICODE ||                        \
                      TPP_CONF_IS_RT(TPP_HAVE_BSE_WHITESPACE) || \
                      TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS)))
#define _TPP_HAVE_BSE_FILE_PARAM 1
struct tpp_lexer;
#define _tpp_preparse_skipbse_lexer__PARAM  , struct tpp_lexer const *tpp_restrict lexer
#define _tpp_preparse_skipbse_lexer__ARG(x) , x
#else /* TPP_HAVE_BSE && TPP_HAVE_UNICODE */
#define _TPP_HAVE_BSE_FILE_PARAM 0
#define _tpp_preparse_skipbse_lexer__PARAM  /* nothing */
#define _tpp_preparse_skipbse_lexer__ARG(x) /* nothing */
#endif /* !TPP_HAVE_BSE || !TPP_HAVE_UNICODE */

/* Check if `ch` may be the first byte of a `\`-character */
#if TPP_HAVE_TRIGRAPHS
#define _tpp_maybe_isbackslash(ch) ((ch) == '\\' || (ch) == '?') /* `?`: because `??/` maps to `\` */
#else /* TPP_HAVE_TRIGRAPHS */
#define _tpp_maybe_isbackslash(ch) ((ch) == '\\')
#endif /* !TPP_HAVE_TRIGRAPHS */

/* Check if `ch` may be the first byte of a LF-style character */
#if TPP_HAVE_UNICODE
#define _tpp_maybe_islf(ch) tpp_ascii_islf_or_mblf(ch)
#else /* TPP_HAVE_UNICODE */
#define _tpp_maybe_islf(ch) tpp_ascii_islf(ch)
#endif /* !TPP_HAVE_UNICODE */

/* Helper macros to skip over BSE when parsing already-loaded text.
 * tpp_preparse_skipbse_fwd: If `pos` points at a `\`-character, skip forward until end of BSE (if it is one)
 * tpp_preparse_skipbse_bck: If `pos` points after a line-feed character, skip backward until start of BSE (if it is one) */
#if TPP_HAVE_BSE
TPP_DECL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_preparse_skipbse_fwd(tpp_char const *pos, tpp_char const *end
                          _tpp_preparse_skipbse_lexer__PARAM);
TPP_DECL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_preparse_skipbse_bck(tpp_char const *start, tpp_char const *pos
                          _tpp_preparse_skipbse_lexer__PARAM);
#define tpp_preparse_skipbse_fwd(lexer, pos, end)   (((pos) >= (end) || tpp_likely(!_tpp_maybe_isbackslash(*(pos)))) ? (pos) : _tpp_preparse_skipbse_fwd(pos, end _tpp_preparse_skipbse_lexer__ARG(lexer)))
#define tpp_preparse_skipbse_bck(lexer, start, pos) (((pos) <= (start) || tpp_likely(!_tpp_maybe_islf((pos)[-1]))) ? (pos) : _tpp_preparse_skipbse_bck(start, pos _tpp_preparse_skipbse_lexer__ARG(lexer)))
#else /* TPP_HAVE_BSE */
#define tpp_preparse_skipbse_fwd(lexer, pos, end)   (pos)
#define tpp_preparse_skipbse_bck(lexer, start, pos) (pos)
#endif /* !TPP_HAVE_BSE */


#if TPP_HAVE_PREPARSE_SKIPSPACE_FWD
/* Skip over all whitespace and BSE sequences, starting at `pos` and
 * going no further than `end` (such that `*end` is never dereferenced)
 *
 * @return: * :  Pointer to the first non-whitespace (and not-part-of-BSE)
 *               character that is `>= pos`.
 * @return: end: Nothing but whitespace (or BSE) found before `end` was reached. */
#define tpp_preparse_skipspace_fwd(lexer, pos, end) \
	_tpp_preparse_skipspace_fwd(pos, end _tpp_preparse_skipbse_lexer__ARG(lexer))
TPP_DECL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_preparse_skipspace_fwd(tpp_char const *pos, tpp_char const *end
                            _tpp_preparse_skipbse_lexer__PARAM);
#endif /* TPP_HAVE_PREPARSE_SKIPSPACE_FWD */

#if TPP_HAVE_PREPARSE_SKIPSPACE_BCK
/* Skip over all whitespace and BSE sequences, starting at `pos-1` and
 * going no further than `start` (such that `start[-1]` is never dereferenced)
 *
 * @return: * :    Pointer after the first non-whitespace (and not-part-of-BSE)
 *                 character that is `<= pos`.
 * @return: start: Nothing but whitespace (or BSE) found before `start` was reached. */
#define tpp_preparse_skipspace_bck(lexer, start, pos) \
	_tpp_preparse_skipspace_bck(start, pos _tpp_preparse_skipbse_lexer__ARG(lexer))
TPP_DECL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_preparse_skipspace_bck(tpp_char const *start, tpp_char const *pos
                            _tpp_preparse_skipbse_lexer__PARAM);
#endif /* TPP_HAVE_PREPARSE_SKIPSPACE_BCK */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_PREPARSE_H */
