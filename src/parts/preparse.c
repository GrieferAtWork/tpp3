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
#ifndef GUARD_TPP_PREPARSE_C
#define GUARD_TPP_PREPARSE_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "lexer.h"
#include "preparse.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* Helper macros to skip over BSE when parsing already-loaded text.
 * tpp_preparse_skipbse_fwd: If "pos" points at a \-character, skip forward until end of BSE (if it is one)
 * tpp_preparse_skipbse_bck: If "pos" points after a line-feed character, skip backward until start of BSE (if it is one) */
#if TPP_HAVE_BSE
TPP_IMPL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_preparse_skipbse_fwd(tpp_char const *pos, tpp_char const *end _tpp_preparse_skipbse_lexer__PARAM) {
	tpp_char const *iter = pos;
#if TPP_CONF_MAYBE_0(TPP_HAVE_BSE)
	if (!tpp_lexer_has(lexer, BSE))
		return pos;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_BSE) */
again:
	tpp_assert(iter < end);
	tpp_assert(_tpp_maybe_isbackslash(*iter));
#if TPP_HAVE_TRIGRAPHS
	if (*iter == '?' && tpp_lexer_has(lexer, TRIGRAPHS)) {
		if ((iter + 2) > end)
			goto not_bse;
		if (iter[1] != '?')
			goto not_bse;
		if (iter[2] != '/')
			goto not_bse;
		iter += 3;
	} else {
		iter += 1;
	}
#else /* TPP_HAVE_TRIGRAPHS */
	++iter;
#endif /* !TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(tpp_lexer_getfile(lexer))) {
		tpp_unichar uc;
#if TPP_HAVE_BSE_WHITESPACE
		for (;;) {
			uc = tpp_unicode_readutf8(&iter, end);
			if (!uc && iter >= end)
				goto not_bse;
			if (tpp_unicode_isspace_nolf(uc)) {
				if (!tpp_lexer_has(lexer, BSE_WHITESPACE))
					goto not_bse;
			} else if (tpp_unicode_islf(uc)) {
				break;
			} else {
				goto not_bse;
			}
		}
#else /* TPP_HAVE_BSE_WHITESPACE */
		uc = tpp_unicode_readutf8(&iter, end);
		if (!tpp_unicode_islf(*iter))
			goto not_bse;
#endif /* !TPP_HAVE_BSE_WHITESPACE */
#if TPP_HAVE_CR_LF_DETECTION
		if (uc == TPP_ASCII_CR && iter < end && *iter == TPP_ASCII_LF)
			++iter;
#endif /* TPP_HAVE_CR_LF_DETECTION */
	} else
#endif /* TPP_HAVE_UNICODE */
	{
#if TPP_HAVE_BSE_WHITESPACE
		if (tpp_lexer_has(lexer, BSE_WHITESPACE)) {
			for (;;) {
				if (iter >= end)
					goto not_bse;
				if (!tpp_ascii_isspace_nolf(*iter))
					break;
				++iter;
			}
		}
#endif /* TPP_HAVE_BSE_WHITESPACE */
		if (!tpp_ascii_islf(*iter))
			goto not_bse;
#if TPP_HAVE_CR_LF_DETECTION
		if (*iter == TPP_ASCII_CR) {
			++iter;
			if (iter < end && *iter == TPP_ASCII_LF)
				++iter;
		} else
#endif /* TPP_HAVE_CR_LF_DETECTION */
		{
			++iter;
		}
	}
	/* Check if there may be multiple, consecutive BSE sequences... */
	if (iter < end && _tpp_maybe_isbackslash(*iter)) {
		pos = iter;
		goto again;
	}
	return iter;
not_bse:
	return pos;
}

TPP_IMPL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_preparse_skipbse_bck(tpp_char const *pos, tpp_char const *start _tpp_preparse_skipbse_lexer__PARAM) {
	tpp_char const *iter = pos;
#if TPP_CONF_MAYBE_0(TPP_HAVE_BSE)
	if (!tpp_lexer_has(lexer, BSE))
		return pos;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_BSE) */
again:
	tpp_assert(iter > start);
#if TPP_HAVE_UNICODE
	tpp_assert(tpp_ascii_islf_or_mblf(iter[-1]));
	if (tpp_file_isutf8(tpp_lexer_getfile(lexer))) {
		tpp_unichar uc;
		uc = tpp_unicode_readutf8_rev(&iter, start);
		if (!tpp_unicode_islf(uc))
			goto not_bse;
#if TPP_HAVE_CR_LF_DETECTION
		if (uc == TPP_ASCII_LF && iter > start && iter[-1] == TPP_ASCII_CR)
			--iter;
#endif /* TPP_HAVE_CR_LF_DETECTION */
#if TPP_HAVE_BSE_WHITESPACE
		for (;;) {
			uc = tpp_unicode_readutf8_rev(&iter, start);
			if (!uc && iter <= start)
				goto not_bse;
			if (tpp_unicode_isspace_nolf(uc)) {
				if (!tpp_lexer_has(lexer, BSE_WHITESPACE))
					goto not_bse;
				continue;
			}
			if (uc == '\\')
				goto return_iter;
#if TPP_HAVE_TRIGRAPHS
			if (uc == '/' && tpp_lexer_has(lexer, TRIGRAPHS)) {
				uc = tpp_unicode_readutf8_rev(&iter, start);
				if (uc != '?')
					goto not_bse;
				uc = tpp_unicode_readutf8_rev(&iter, start);
				if (uc != '?')
					goto not_bse;
				goto return_iter;
			}
#endif /* TPP_HAVE_TRIGRAPHS */
			goto not_bse;
		}
#else /* TPP_HAVE_BSE_WHITESPACE */
		uc = tpp_unicode_readutf8_rev(&iter, start);
		if (uc == '\\')
			goto return_iter;
#if TPP_HAVE_TRIGRAPHS
		if (uc == '/' && tpp_lexer_has(lexer, TRIGRAPHS)) {
			uc = tpp_unicode_readutf8_rev(&iter, start);
			if (uc != '?')
				goto not_bse;
			uc = tpp_unicode_readutf8_rev(&iter, start);
			if (uc != '?')
				goto not_bse;
			goto return_iter;
		}
#endif /* TPP_HAVE_TRIGRAPHS */
		goto not_bse;
#endif /* !TPP_HAVE_BSE_WHITESPACE */
	}
	if (!tpp_ascii_islf(*iter))
		goto not_bse;
#else /* TPP_HAVE_UNICODE */
	tpp_assert(tpp_ascii_islf(iter[-1]));
#endif /* !TPP_HAVE_UNICODE */
	--iter;
	if (iter <= start)
		goto not_bse;
#if TPP_HAVE_CR_LF_DETECTION
	if (iter[-1] == TPP_ASCII_CR && *iter == TPP_ASCII_LF) {
		--iter;
		if (iter <= start)
			goto not_bse;
	}
#endif /* TPP_HAVE_CR_LF_DETECTION */
#if TPP_HAVE_BSE_WHITESPACE
	for (;;) {
		if (iter <= start)
			goto not_bse;
		--iter;
		if (!tpp_ascii_isspace_nolf(*iter))
			break;
	}
#endif /* TPP_HAVE_BSE_WHITESPACE */
	if (*iter == '\\') {
		/* Yes! */
	} else
#if TPP_HAVE_TRIGRAPHS
	if (*iter == '/') {
		if ((iter - 2) < start)
			goto not_bse;
		if (iter[-1] != '?')
			goto not_bse;
		if (iter[-2] != '?')
			goto not_bse;
		iter -= 2;
	} else
#endif /* TPP_HAVE_TRIGRAPHS */
	{
		goto not_bse;
	}

	/* Check if there may be multiple, consecutive BSE sequences... */
#if TPP_HAVE_UNICODE
return_iter:
#endif /* TPP_HAVE_UNICODE */
	if (iter > start && _tpp_maybe_islf(iter[-1])) {
		pos = iter;
		goto again;
	}
	return iter;
not_bse:
	return pos;
}
#endif /* TPP_HAVE_BSE */

#if TPP_HAVE_PREPARSE_SKIPSPACE_FWD
/* Skip over all whitespace and BSE sequences, starting at `pos` and
 * going no further than `end` (such that `*end` is never dereferenced)
 *
 * @return: * :  Pointer to the first non-whitespace (and not-part-of-BSE)
 *               character that is `>= pos`.
 * @return: end: Nothing but whitespace (or BSE) found before `end` was reached. */
TPP_IMPL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_preparse_skipspace_fwd(tpp_char const *pos, tpp_char const *end
                            _tpp_preparse_skipbse_lexer__PARAM) {
	for (;;) {
		tpp_char ch;
		pos = tpp_preparse_skipbse_fwd(lexer, pos, end);
		if (pos >= end)
			break;
		ch = *pos;
		if (tpp_ascii_isspace(ch)) {
			++pos;
		} else
#if TPP_HAVE_UNICODE
		if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(lexer))) {
			tpp_char const *uc_pos = pos;
			tpp_unichar uc = tpp_unicode_readutf8(&uc_pos, end);
			if (!tpp_unicode_isspace(uc))
				break;
			pos = uc_pos;
		} else
#endif /* TPP_HAVE_UNICODE */
		{
			break;
		}
	}
	return pos;
}
#endif /* TPP_HAVE_PREPARSE_SKIPSPACE_FWD */

#if TPP_HAVE_PREPARSE_SKIPSPACE_BCK
/* Skip over all whitespace and BSE sequences, starting at `pos-1` and
 * going no further than `start` (such that `start[-1]` is never dereferenced)
 *
 * @return: * :    Pointer after the first non-whitespace (and not-part-of-BSE)
 *                 character that is `<= pos`.
 * @return: start: Nothing but whitespace (or BSE) found before `start` was reached. */
TPP_IMPL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_preparse_skipspace_bck(tpp_char const *pos, tpp_char const *start
                            _tpp_preparse_skipbse_lexer__PARAM) {
	for (;;) {
		tpp_char ch;
		pos = tpp_preparse_skipbse_bck(lexer, pos, start);
		if (pos <= start)
			break;
		ch = pos[-1];
		if (tpp_ascii_isspace(ch)) {
			--pos;
		} else
#if TPP_HAVE_UNICODE
		if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(lexer))) {
			tpp_char const *uc_pos = pos;
			tpp_unichar uc = tpp_unicode_readutf8_rev(&uc_pos, start);
			if (!tpp_unicode_isspace(uc))
				break;
			pos = uc_pos;
		} else
#endif /* TPP_HAVE_UNICODE */
		{
			break;
		}
	}
	return pos;
}
#endif /* TPP_HAVE_PREPARSE_SKIPSPACE_BCK */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_PREPARSE_C */
