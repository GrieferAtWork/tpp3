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
#ifndef GUARD_TPP_KEYWORD_C
#define GUARD_TPP_KEYWORD_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "keyword.h"
#include "lexer.h"
#include "macro.h"
#include "string.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* Assert that "tpp_keyword" and "tpp_string" are binary-compatible. */
#if TPP_HAVE_KEYWORD_ASSTRING
TPP_STATIC_ASSERT((tpp_offsetof(tpp_keyword, tk_refcnt) -
                   tpp_offsetof(tpp_keyword, _TPP_KEYWORD_STRING_ABI_START)) ==
                  (tpp_offsetof(tpp_string, ts_refcnt)));
TPP_STATIC_ASSERT((tpp_offsetof(tpp_keyword, tk_len) -
                   tpp_offsetof(tpp_keyword, _TPP_KEYWORD_STRING_ABI_START)) ==
                  (tpp_offsetof(tpp_string, ts_len)));
TPP_STATIC_ASSERT((tpp_offsetof(tpp_keyword, tk_kwd) -
                   tpp_offsetof(tpp_keyword, _TPP_KEYWORD_STRING_ABI_START)) ==
                  (tpp_offsetof(tpp_string, ts_str)));
#endif /* TPP_HAVE_KEYWORD_ASSTRING */

#if TPP_HAVE_KEYWORD_ASSTRING
#define tpp_keyword_init_refcnt(self) tpp_refcnt_atomic_init(&(self)->tk_refcnt, 1)
#else /* TPP_HAVE_KEYWORD_ASSTRING */
#define tpp_keyword_init_refcnt(self) (void)0
#endif /* !TPP_HAVE_KEYWORD_ASSTRING */

#if TPP_HAVE_PRAGMA_PUSH_MACRO
/* Initialize/finalize a given macro-push stack */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_macro_pushstack_fini(tpp_macro_pushstack *tpp_restrict self) {
	tpp_size i;
	for (i = 0; i < self->tmps_cnt; ++i) {
		TPP_REF tpp_macro *mac;
		mac = self->tmps_vec[i].tmpe_macro;
		if (mac)
			tpp_macro_decref(mac);
	}
	tpp_free(self->tmps_vec);
}

#if TPP_HAVE_LEXER_COPY
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_macro_pushstack_copy(tpp_macro_pushstack *tpp_restrict self,
                         tpp_macro_pushstack const *tpp_restrict from) {
	tpp_size i;
	tpp_macro_pushent *vec;
	self->tmps_cnt = from->tmps_cnt;
	self->tmps_vec = NULL;
	if (self->tmps_cnt == 0)
		return TPP_EOK;
	vec = (tpp_macro_pushent *)tpp_malloc(self->tmps_cnt * sizeof(tpp_macro_pushent));
	if tpp_unlikely(!vec)
		return TPP_ENOMEM;
	self->tmps_vec = vec;
	for (i = 0; i < self->tmps_cnt; ++i) {
		tpp_macro_pushent const *src = &from->tmps_vec[i];
		tpp_macro_pushent *dst = &vec[i];
		dst->tmpe_count = src->tmpe_count;
		dst->tmpe_macro = tpp_macro_copy(src->tmpe_macro);
		if tpp_unlikely(!dst->tmpe_macro) {
			while (i--) {
				dst = &vec[i];
				tpp_macro_decref(dst->tmpe_macro);
			}
			return TPP_ENOMEM;
		}
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_COPY */


/* Allocate space for- and return a new (uninitialized) macro-push entry
 * @return: * :   The newly allocated macro-push entry.
 * @return: NULL: Out-of-memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_pushent *TPPCALL
tpp_macro_pushstack_append(tpp_macro_pushstack *tpp_restrict self) {
	tpp_macro_pushent *new_vec;
	tpp_size new_cnt = self->tmps_cnt + 1;
	new_vec = (tpp_macro_pushent *)tpp_realloc(self->tmps_vec, new_cnt * sizeof(tpp_macro_pushent));
	if tpp_likely(new_vec) {
		self->tmps_vec = new_vec;
		self->tmps_cnt = new_cnt;
		new_vec += new_cnt - 1; /* Return pointer to last (newly allocated / uninitialized) element. */
	}
	return new_vec;
}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_KEYWORD_MISC
/* Ensure that `self->tk_misc' has been allocated and return it.
 * If it isn't already allocated, allocate+return it lazily.
 * WARNING: Only call this function on a "writable" keyword (s.a. `tpp_keywords_copybuiltin()')
 *
 * @return: * :   The "misc" data of "self" (freshly allocated)
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_misc *TPPCALL
tpp_keyword_requiremisc(tpp_keyword *tpp_restrict self) {
	tpp_keyword_misc *result = self->tk_misc;
	if tpp_unlikely(result == NULL) {
		result = _tpp_keyword_misc_alloc();
		if tpp_likely(result) {
#if TPP_HAVE_KEYWORD_FLAGS
			result->tkm_flags = TPP_KEYWORD_FLAG_NORMAL;
#endif /* TPP_HAVE_KEYWORD_FLAGS */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
			result->tkm_file_guard = NULL;
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_PRAGMA_PUSH_MACRO
			tpp_macro_pushstack_init(&result->tkm_macro_pushstack);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_MACRO___TPP_COUNTER
			result->tkm_builtin_counter = 0;
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
#if TPP_HAVE_KEYWORD_USERDATA
			result->tkm_userdata_ptr  = NULL;
			result->tkm_userdata_dtor = NULL;
#endif /* TPP_HAVE_KEYWORD_USERDATA */
			self->tk_misc = result;
		}
	}
	return result;
}
#endif /* TPP_HAVE_KEYWORD_MISC */


#if TPP_HAVE_KEYWORD_USERDATA
/* Get the user-data pointer for "self"
 * @return: NULL: No pointer set, or set pointer is "NULL" */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) void *TPPCALL
tpp_keyword_getuserdata(tpp_keyword const *tpp_restrict self) {
	tpp_keyword_misc const *misc = self->tk_misc;
	return misc ? tpp_keyword_misc_getuserdata(misc) : NULL;
}

/* Set the user-data pointer for "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_setuserdata(tpp_keyword *tpp_restrict self,
                        void *ptr, void (TPPCALL *dtor)(void *ptr)) {
	tpp_keyword_misc *misc;
	if (!ptr && !dtor && !self->tk_misc)
		return TPP_EOK;
	misc = tpp_keyword_requiremisc(self);
	if tpp_unlikely(!misc)
		return TPP_ENOMEM;
	tpp_keyword_misc_setuserdata(misc, ptr, dtor);
	return TPP_EOK;
}
#endif /* TPP_HAVE_KEYWORD_USERDATA */



#if TPP_HAVE_PRAGMA_PUSH_MACRO
/* Push the current macro-definition of "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_pushmacro(tpp_keyword *tpp_restrict self) {
	tpp_keyword_misc *const misc = tpp_keyword_requiremisc(self);
	tpp_macro_pushent *ent;
	if tpp_unlikely(!misc)
		goto err_nomem;

	/* Check if the last-pushed entry still correctly describes the state. */
	if (misc->tkm_macro_pushstack.tmps_cnt) {
		ent = &misc->tkm_macro_pushstack.tmps_vec[misc->tkm_macro_pushstack.tmps_cnt - 1];
		tpp_assert(ent->tmpe_count != 0);
		if (ent->tmpe_macro == self->tk_macro) {
			++ent->tmpe_count;
			return TPP_EOK;
		}
	}

	/* Must allocate a new push-entry. */
	ent = tpp_macro_pushstack_append(&misc->tkm_macro_pushstack);
	if tpp_unlikely(!ent)
		goto err_nomem;

	/* Initialize the new push-entry */
	ent->tmpe_count = 1;              /* First time! */
	ent->tmpe_macro = self->tk_macro; /* Current definition */
	if (ent->tmpe_macro)
		tpp_macro_incref(ent->tmpe_macro);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}


/* Pop the current macro-definition of "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: Macro-push-stack was already empty (soft-error) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_popmacro(tpp_keyword *tpp_restrict self) {
	tpp_keyword_misc *misc = self->tk_misc;
	tpp_macro_pushent *last;
	if (misc == NULL)
		goto err_empty;
	if (misc->tkm_macro_pushstack.tmps_cnt == 0)
		goto err_empty;
	last = &misc->tkm_macro_pushstack.tmps_vec[misc->tkm_macro_pushstack.tmps_cnt - 1];
	tpp_assert(last->tmpe_count != 0);

	/* Restore macro definition */
	if (last->tmpe_macro)
		tpp_macro_incref(last->tmpe_macro);
	if (self->tk_macro)
		tpp_macro_decref(self->tk_macro);
	self->tk_macro = last->tmpe_macro;

	/* Update stack-element counter. */
	--last->tmpe_count;
	if (last->tmpe_count == 0) {
		/* Remove stack element. */
		if (last->tmpe_macro)
			tpp_refcnt_dec(&last->tmpe_macro->tm_refcnt);
		--misc->tkm_macro_pushstack.tmps_cnt;
#ifndef __OPTIMIZE_SIZE__
		if (misc->tkm_macro_pushstack.tmps_cnt == 0) {
			/* Free push-stack */
			tpp_free(misc->tkm_macro_pushstack.tmps_vec);
			misc->tkm_macro_pushstack.tmps_vec = NULL;
		} else {
			/* Try to truncate push-stack */
			tpp_macro_pushent *new_vec;
			new_vec = (tpp_macro_pushent *)tpp_tryrealloc(misc->tkm_macro_pushstack.tmps_vec,
			                                              misc->tkm_macro_pushstack.tmps_cnt *
			                                              sizeof(tpp_macro_pushent));
			if tpp_likely(new_vec)
				misc->tkm_macro_pushstack.tmps_vec = new_vec;
		}
#endif /* !__OPTIMIZE_SIZE__ */
	}
	return TPP_EOK;
err_empty:
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_CPP_MACROS
/* Delete the macro definition of `self'.
 * The caller must ensure that `tpp_keyword_canundef(self)' */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keyword_undef(tpp_keyword *tpp_restrict self) {
	TPP_REF tpp_macro *old_macro;
	tpp_assert(tpp_keyword_canundef(self));
	old_macro = self->tk_macro;
	self->tk_macro = NULL;
	tpp_macro_decref(old_macro);
}
#endif /* TPP_HAVE_CPP_MACROS */



/* Calculate the hash of a given keyword string */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof(tpp_char const *tpp_restrict kwd, tpp_size len) {
	tpp_hash result = 1;
	tpp_size i;
	for (i = 0; i < len; ++i) {
		tpp_char ch = kwd[i];
		result = result * 263 + ch;
	}
	return result;
}



/* Helper macros to skip over BSE when parsing already-loaded text.
 * tpp_skipbse_fwd: If "pos" points at a \-character, skip forward until end of BSE (if it is one)
 * tpp_skipbse_bck: If "pos" points after a line-feed character, skip backward until start of BSE (if it is one) */
#if TPP_HAVE_BSE
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_skipbse_fwd_(tpp_char const *pos, tpp_char const *end tpp_bse_file__PARAM) {
	tpp_char const *iter = pos;
again:
	tpp_assert(iter < end);
	tpp_assert(_tpp_maybe_isbackslash(*iter));
#if TPP_HAVE_TRIGRAPHS
	if (*iter == '?') {
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
	if (tpp_file_isutf8(file)) {
		tpp_unichar uc;
#if TPP_HAVE_BSE_WHITESPACE
		for (;;) {
			uc = tpp_unicode_readutf8(&iter, end);
			if (!uc && iter >= end)
				goto not_bse;
			if (!tpp_unicode_isspace_nolf(uc)) {
				if (!tpp_unicode_islf(uc))
					goto not_bse;
				break;
			}
		}
#else /* TPP_HAVE_BSE_WHITESPACE */
		uc = tpp_unicode_readutf8(&iter, end);
		if (!tpp_unicode_islf(*iter))
			goto not_bse;
#endif /* !TPP_HAVE_BSE_WHITESPACE */
		if (uc == '\r' && iter < end && *iter == '\n')
			++iter;
	} else
#endif /* TPP_HAVE_UNICODE */
	{
#if TPP_HAVE_BSE_WHITESPACE
		for (;;) {
			if (iter >= end)
				goto not_bse;
			if (!tpp_ascii_isspace_nolf(*iter))
				break;
			++iter;
		}
#endif /* TPP_HAVE_BSE_WHITESPACE */
		if (!tpp_ascii_islf(*iter))
			goto not_bse;
		if (*iter == '\r') {
			++iter;
			if (iter < end && *iter == '\n')
				++iter;
		} else {
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

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_skipbse_bck_(tpp_char const *pos, tpp_char const *start tpp_bse_file__PARAM) {
	tpp_char const *iter = pos;
again:
	tpp_assert(iter > start);
#if TPP_HAVE_UNICODE
	tpp_assert(tpp_ascii_islfornascii(iter[-1]));
	if (tpp_file_isutf8(file)) {
		tpp_unichar uc;
		uc = tpp_unicode_readutf8_rev(&iter, start);
		if (!tpp_unicode_islf(uc))
			goto not_bse;
		if (uc == '\n' && iter > start && iter[-1] == '\r')
			--iter;
#if TPP_HAVE_BSE_WHITESPACE
		for (;;) {
			uc = tpp_unicode_readutf8_rev(&iter, start);
			if (!uc && iter <= start)
				goto not_bse;
			if (!tpp_unicode_isspace_nolf(uc)) {
				if (uc == '\\')
					return iter;
#if TPP_HAVE_TRIGRAPHS
				if (uc == '/') {
					uc = tpp_unicode_readutf8_rev(&iter, start);
					if (uc != '?')
						goto not_bse;
					uc = tpp_unicode_readutf8_rev(&iter, start);
					if (uc != '?')
						goto not_bse;
					return iter;
				}
#endif /* TPP_HAVE_TRIGRAPHS */
				goto not_bse;
			}
		}
#else /* TPP_HAVE_BSE_WHITESPACE */
		uc = tpp_unicode_readutf8_rev(&iter, start);
		if (uc == '\\')
			return iter;
#if TPP_HAVE_TRIGRAPHS
		if (uc == '/') {
			uc = tpp_unicode_readutf8_rev(&iter, start);
			if (uc != '?')
				goto not_bse;
			uc = tpp_unicode_readutf8_rev(&iter, start);
			if (uc != '?')
				goto not_bse;
			return iter;
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
	if (iter[-1] == '\r' && *iter == '\n') {
		--iter;
		if (iter <= start)
			goto not_bse;
	}
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
	if (iter > start && _tpp_maybe_islf(iter[-1])) {
		pos = iter;
		goto again;
	}
	return iter;
not_bse:
	return pos;
}
#endif /* TPP_HAVE_BSE */


#if TPP_HAVE_ESCAPED_KEYWORDS
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS

/* Check if "*p_iter" is a BSI sequence.
 * - If so, decode it, update `*p_iter' to point to its end,
 *   and return the # of utf-8 bytes written to "buf"
 * - Otherwise, return "0"
 *
 * NOTE: Given *p_iter points **AFTER** the leading \ character
 *
 * Caller must ensure that _tpp_maybe_isbackslash((*p_iter)[-1]) */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_decode_bsi(tpp_char buf[TPP_UTF8_MAXLEN], tpp_char const **p_iter, tpp_char const *end) {
	tpp_char ch;
	tpp_char const *iter = *p_iter;
	tpp_unichar uc;
	unsigned int cur_digit;
	unsigned int max_digit;
	tpp_assert(_tpp_maybe_isbackslash(iter[-1]));
	if (iter >= end)
		goto nope;
	ch = *iter++;
#if TPP_HAVE_TRIGRAPHS
	if (ch == '?') {
		if (iter[-2] != '?')
			goto nope;
		if (iter >= end)
			goto nope;
		if (*iter != '/')
			goto nope;
		++iter;
		ch = *iter++;
	}
#endif /* TPP_HAVE_TRIGRAPHS */

	if (ch == 'u') {
		max_digit = 4;
	} else if (ch == 'U') {
		max_digit = 8;
	} else {
		goto nope;
	}
	if (iter >= end)
		goto nope;
	ch = *iter++;
	uc = 0;
	cur_digit = 0;
	for (;;) {
		uint_least8_t nibble;
		if (ch >= '0' && ch <= '9') {
			nibble = (uint_least8_t)(ch - '0');
		} else if (ch >= 'a' && ch <= 'f') {
			nibble = 10 + (uint_least8_t)(ch - 'a');
		} else if (ch >= 'A' && ch <= 'F') {
			nibble = 10 + (uint_least8_t)(ch - 'A');
		} else {
			if (cur_digit == 0)
				goto nope;
			--iter; /* Don't consume this one! */
			break;
		}
		uc <<= 4;
		uc |= nibble;
		++cur_digit;
		if (cur_digit >= max_digit)
			break;
		if (iter >= end)
			break;
		ch = *iter++;
	}

	/* Indicate to caller that the BSI sequence was consumed. */
	*p_iter = iter;

	/* (re-)encode as utf-8 */
	return (tpp_size)(tpp_unicode_writeutf8(buf, uc) - buf);
nope:
	return 0;
}
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */


/* Same as `tpp_hashof()', but skip over \-escaped linefeeds when calculating the hash */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof_esc_(tpp_char const *tpp_restrict kwd, tpp_size len tpp_bse_file__PARAM) {
	tpp_hash result = 1;
	tpp_char const *end = kwd + len;
	while (kwd < end) {
		tpp_char ch = *kwd++;
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
		tpp_char bsi[TPP_UTF8_MAXLEN];
		tpp_size bsi_len;
		if ((_tpp_maybe_isbackslash(ch)) &&
		    (bsi_len = tpp_decode_bsi(bsi, (tpp_char const **)&kwd, end)) != 0) {
			tpp_size i = 0;
			do {
				result = result * 263 + bsi[i];
			} while (++i < bsi_len);
		} else
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
		{
			result = result * 263 + ch;
		}
		kwd = tpp_skipbse_fwd(kwd, end, file);
	}
	return result;
}

/* Copy `in_text...+=len' to `out_text', whilst removing \-escaped linefeeds
 * The caller must ensure that `out_text' has space for at least `len' bytes,
 * and the actual # of used bytes of `out_text' is returned. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_without_esc_(tpp_char *tpp_restrict out_text,
                 tpp_char const *tpp_restrict in_text,
                 tpp_size len tpp_bse_file__PARAM) {
	tpp_size result = 0;
	tpp_char const *end = in_text + len;
	while (in_text < end) {
		tpp_char ch = *in_text++;
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
		tpp_size bsi_len;
		if ((_tpp_maybe_isbackslash(ch)) &&
		    (bsi_len = tpp_decode_bsi(out_text + result, (tpp_char const **)&in_text, end)) != 0) {
			result += bsi_len;
		} else
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
		{
			out_text[result++] = ch;
		}
		in_text = tpp_skipbse_fwd(in_text, end, file);
	}
	return result;
}

/* Compare 2 strings, one of which may contain \-escaped linefeeds that must be skipped. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) int TPPCALL
tpp_memcmp_esc_(tpp_char const *lhs_without_esc, tpp_size lhs_len,
                tpp_char const *rhs_with_esc, tpp_size rhs_len
                tpp_bse_file__PARAM) {
	tpp_char const *lhs_end = lhs_without_esc + lhs_len;
	tpp_char const *rhs_end = rhs_with_esc + rhs_len;
	while (rhs_with_esc < rhs_end) {
		tpp_char lhs_ch, rhs_ch;
		if (lhs_without_esc >= lhs_end)
			return -1;
		lhs_ch = *lhs_without_esc++;
		rhs_ch = *rhs_with_esc++;
		if (lhs_ch != rhs_ch) {
#if TPP_HAVE_ESCAPE_IN_IDENTIFIERS
			tpp_char bsi[TPP_UTF8_MAXLEN];
			tpp_size bsi_len;
			if ((_tpp_maybe_isbackslash(rhs_ch)) &&
			    (bsi_len = tpp_decode_bsi(bsi, (tpp_char const **)&rhs_with_esc, rhs_end)) != 0) {
				tpp_size i = 0;
				do {
					if (lhs_ch != bsi[i])
						return lhs_ch < bsi[i] ? -1 : 1;
					if (lhs_without_esc >= lhs_end)
						return -1;
					lhs_ch = *lhs_without_esc++;
				} while (++i < bsi_len);
			} else
#endif /* TPP_HAVE_ESCAPE_IN_IDENTIFIERS */
			{
				return lhs_ch < rhs_ch ? -1 : 1;
			}
		}
		rhs_with_esc = tpp_skipbse_fwd(rhs_with_esc, rhs_end, file);
	}
	return 0;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


#if TPP_HAVE_KEYWORD_MISC
static TPP_NONNULL((1)) void TPPCALL
tpp_keyword_misc_destroy(tpp_keyword_misc *tpp_restrict self) {
#if TPP_HAVE_PRAGMA_PUSH_MACRO
	tpp_macro_pushstack_fini(&self->tkm_macro_pushstack);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_KEYWORD_USERDATA
	if (self->tkm_userdata_dtor)
		(*self->tkm_userdata_dtor)(self->tkm_userdata_ptr);
#endif /* TPP_HAVE_KEYWORD_USERDATA */
	_tpp_keyword_misc_free(self);
}
#endif /* TPP_HAVE_KEYWORD_MISC */

static TPP_NONNULL((1)) void TPPCALL
tpp_keyword_destroy(tpp_keyword *tpp_restrict self) {
#if TPP_HAVE_KEYWORD_ASSTRING
	tpp_assert(!tpp_refcnt_atomic_isshared(&self->tk_refcnt) && "Keyword still in use");
#endif /* TPP_HAVE_KEYWORD_ASSTRING */
#if TPP_HAVE_CPP_MACROS
	if (self->tk_macro) {
		tpp_assert(self->tk_macro->tm_expansions == 0 && "Macro still part of #include-stack?");
		tpp_macro_decref(self->tk_macro);
	}
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	if (self->tk_misc)
		tpp_keyword_misc_destroy(self->tk_misc);
#endif /* TPP_HAVE_KEYWORD_MISC */
	_tpp_keyword_free(self);
}

static TPP_NONNULL((1)) void TPPCALL
tpp_keyword_destroychain(tpp_keyword *tpp_restrict chain) {
	do {
		TPP_REF tpp_keyword *next = chain->tk_next;
		tpp_keyword_destroy(chain);
		chain = next;
	} while (chain);
}


TPP_IMPL TPP_REF tpp_keyword *tpp_keywords_empty_map[1] = { NULL };

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_fini(tpp_keywords *tpp_restrict self) {
	TPP_REF tpp_keyword **bckv = self->tks_bckv;
	if (bckv != tpp_keywords_empty_map) {
		tpp_hash i;
		for (i = 0; i <= self->tks_bckm; ++i) {
			TPP_REF tpp_keyword *chain = bckv[i];
			if (chain)
				tpp_keyword_destroychain(chain);
		}
		tpp_free(bckv);
	}
}

#if TPP_HAVE_LEXER_COPY
#if TPP_HAVE_KEYWORD_MISC
static TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_misc *TPPCALL
tpp_keyword_copymisc(tpp_keyword_misc const *tpp_restrict self) {
	tpp_keyword_misc *result = _tpp_keyword_misc_alloc();
	if tpp_unlikely(!result)
		return NULL;
#if TPP_HAVE_KEYWORD_FLAGS
	result->tkm_flags = self->tkm_flags;
#endif /* TPP_HAVE_KEYWORD_FLAGS */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
	result->tkm_file_guard = self->tkm_file_guard; /* Relocated into the new keyword-table later */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_PRAGMA_PUSH_MACRO
	{
		tpp_errno error = tpp_macro_pushstack_copy(&result->tkm_macro_pushstack,
		                                           &self->tkm_macro_pushstack);
		tpp_assert(!TPP_ISERR(error) || error == TPP_ENOMEM);
		if (TPP_ISERR(error)) {
			_tpp_keyword_free(result);
			return NULL;
		}
	}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_MACRO___TPP_COUNTER
	result->tkm_builtin_counter = self->tkm_builtin_counter;
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
#if TPP_HAVE_KEYWORD_USERDATA
	result->tkm_userdata_ptr  = self->tkm_userdata_ptr;
	result->tkm_userdata_dtor = NULL; /* Intentionally set to "NULL"! */
#endif /* TPP_HAVE_KEYWORD_USERDATA */
	return result;
}
#endif /* TPP_HAVE_KEYWORD_MISC */

/* Copy+return "self", but leave "return->tk_next" undefined */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword *TPPCALL
tpp_keyword_copy(tpp_keyword const *tpp_restrict self) {
	tpp_keyword *result;
	result = _tpp_keyword_alloc(self->tk_len);
	if (result == NULL)
		return NULL;
#if TPP_HAVE_KEYWORD_MISC
	result->tk_misc = NULL;
	if (self->tk_misc) {
		result->tk_misc = tpp_keyword_copymisc(self->tk_misc);
		if tpp_unlikely(!result->tk_misc) {
			_tpp_keyword_free(result);
			return NULL;
		}
	}
#endif /* TPP_HAVE_KEYWORD_MISC */
	result->tk_id = self->tk_id;
#if TPP_HAVE_CPP_MACROS
	result->tk_macro = NULL;
	if (self->tk_macro) {
		result->tk_macro = tpp_macro_copy(self->tk_macro);
		if tpp_unlikely(!result->tk_macro) {
#if TPP_HAVE_KEYWORD_MISC
			if (result->tk_misc)
				tpp_keyword_misc_destroy(result->tk_misc);
#endif /* TPP_HAVE_KEYWORD_MISC */
			_tpp_keyword_free(result);
			return NULL;
		}
	}
#endif /* TPP_HAVE_CPP_MACROS */
	result->tk_hash = self->tk_hash;
	tpp_keyword_init_refcnt(result);
	result->tk_len = self->tk_len;
	tpp_memcpy(result->tk_kwd, self->tk_kwd, (self->tk_len + 1) * sizeof(tpp_char));
	return result;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword *TPPCALL
tpp_keyword_copychain(tpp_keyword const *tpp_restrict self) {
	tpp_keyword *result = tpp_keyword_copy(self);
	tpp_keyword *last = result;
	while (self->tk_next) {
		tpp_keyword *next;
		self = self->tk_next;
		next = tpp_keyword_copy(self);
		if tpp_unlikely(!next)
			goto err_r;
		last->tk_next = next;
		last = next;
	}
	last->tk_next = NULL;
	return result;
err_r:
	last->tk_next = NULL;
	tpp_keyword_destroychain(result);
	return NULL;
}

#if TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO
/* Relocate "self->tm_deffile" in case it references a keyword */
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_macro_relocate_deffile(tpp_macro *tpp_restrict self,
                           tpp_keywords const *tpp_restrict keywords) {
	tpp_char const *deffile = (tpp_char const *)self->tm_deffile;
	if (deffile) {
		tpp_size deffile_len = tpp_strlen((char const *)deffile);
		tpp_hash deffile_hash = tpp_hashof(deffile, deffile_len);
		tpp_keyword const *deffile_kwd;
		deffile_kwd = tpp_keywords_getkeyword(keywords, deffile,
		                                      deffile_len, deffile_hash);
		if (deffile_kwd)
			self->tm_deffile = tpp_keyword_getkwdcstr(deffile_kwd);
	}
}
#endif /* TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO */

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_keywords_copy(tpp_keywords *tpp_restrict self,
                  tpp_keywords const *tpp_restrict from) {
	self->tks_kwdc = from->tks_kwdc;
	self->tks_bckm = from->tks_bckm;
	self->tks_bckv = from->tks_bckv;
	if (self->tks_bckv != tpp_keywords_empty_map) {
		TPP_REF tpp_keyword **bckv;
		tpp_keyword const *const *src;
		tpp_hash i;
		bckv = (TPP_REF tpp_keyword **)tpp_malloc((self->tks_bckm + 1) *
		                                          sizeof(TPP_REF tpp_keyword *));
		if tpp_unlikely(!bckv)
			return TPP_ENOMEM;
		self->tks_bckv = bckv;
		src = (tpp_keyword const *const *)from->tks_bckv;
		for (i = 0; i <= self->tks_bckm; ++i) {
			tpp_keyword const *chain = src[i];
			bckv[i] = NULL;
			if (chain) {
				tpp_keyword *copy;
				copy = tpp_keyword_copychain(chain);
				if tpp_unlikely(!copy) {
					while (i--) {
						copy = bckv[i];
						if (copy)
							tpp_keyword_destroychain(copy);
					}
					tpp_free(bckv);
					return TPP_ENOMEM;
				}
				bckv[i] = copy;
			}
		}
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_CPP_MACROS
		for (i = 0; i <= self->tks_bckm; ++i) {
			tpp_keyword *chain = bckv[i];
			for (; chain; chain = chain->tk_next) {
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS || (TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO)
				tpp_keyword_misc *misc = chain->tk_misc;
				if (misc) {
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
					tpp_keyword const *guard = misc->tkm_file_guard;
					if (guard && !tpp_keyword_isbuiltin(guard)) {
						guard = tpp_keywords_getkeyword(self,
						                                guard->tk_kwd,
						                                guard->tk_len,
						                                guard->tk_hash);
						tpp_assert(guard && "File guard keyword not found even "
						                    "though it should have been copied");
						misc->tkm_file_guard = guard;
					}
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO
					{
						tpp_size j;
						for (j = 0; j < misc->tkm_macro_pushstack.tmps_cnt; ++j) {
							tpp_macro_pushent *ent = &misc->tkm_macro_pushstack.tmps_vec[j];
							tpp_macro_relocate_deffile(ent->tmpe_macro, self);
						}
					}
#endif /* TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO */
				}
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS || (TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO) */
#if TPP_HAVE_CPP_MACROS
				if (chain->tk_macro)
					tpp_macro_relocate_deffile(chain->tk_macro, self);
#endif /* TPP_HAVE_CPP_MACROS */
			}
		}
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_CPP_MACROS */
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_COPY */


/* Lookup keywords within the given keywords-table **ONLY**
 * @return: * :   The keyword in question
 * @return: NULL: No such keyword (consider using "tpp_keywords_getkeyword" to
 *                also check for builtin keywords, or "tpp_keywords_newkeyword"
 *                to do the same, but lazily create missing keywords) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword(tpp_keywords const *tpp_restrict self,
                         tpp_char const *tpp_restrict kwd,
                         tpp_size len, tpp_hash hash) {
	tpp_keyword *result = self->tks_bckv[hash & self->tks_bckm];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (result->tk_len != len)
			continue;
		if (tpp_memcmp(result->tk_kwd, kwd, len) == 0)
			break;
	}
	return result;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword_byid(tpp_keywords const *tpp_restrict self,
                              enum tpp_token_id id) {
	tpp_hash i;
#if !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
	if (!TPP_TOK_ISUSERKEYWORD(id))
		return NULL;
#endif /* !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */

	/* XXX: This is slow... */
	for (i = 0; i <= self->tks_bckm; ++i) {
		tpp_keyword *chain = self->tks_bckv[i];
		for (; chain; chain = chain->tk_next) {
			if (chain->tk_id == id)
				return chain;
		}
	}
	return NULL;
}

#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword_esc_(tpp_keywords const *tpp_restrict self,
                              tpp_char const *tpp_restrict kwd,
                              tpp_size len, tpp_hash hash
                              tpp_bse_file__PARAM) {
	tpp_keyword *result = self->tks_bckv[hash & self->tks_bckm];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (tpp_memcmp_esc(result->tk_kwd, result->tk_len, kwd, len, file) == 0)
			break;
	}
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Same as above, but also search the built-in keyword table (tpp_builtin_getkeyword) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword(tpp_keywords const *tpp_restrict self,
                        tpp_char const *tpp_restrict kwd,
                        tpp_size len, tpp_hash hash) {
	tpp_keyword const *result;
	result = _tpp_keywords_getkeyword(self, kwd, len, hash);
	if (result == NULL)
		result = tpp_builtin_getkeyword(kwd, len, hash);
	return result;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword_byid(tpp_keywords const *tpp_restrict self,
                             enum tpp_token_id id) {
	tpp_keyword const *result;
	result = _tpp_keywords_getkeyword_byid(self, id);
	if (result == NULL)
		result = tpp_builtin_getkeyword_byid(id);
	return result;
}

#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword_esc_(tpp_keywords const *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM) {
	tpp_keyword const *result;
	result = _tpp_keywords_getkeyword_esc(self, kwd, len, hash, file);
	if (result == NULL)
		result = tpp_builtin_getkeyword_esc(kwd, len, hash, file);
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Insert "kwd" into "self". If necessary, resize the hash-map.
 * If resizing fails, "kwd" is destroyed and "NULL" is returned. */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
tpp_keywords_inskeyword(tpp_keywords *tpp_restrict self,
                        /*inherit(always)*/ tpp_keyword *kwd) {
	if (self->tks_kwdc >= self->tks_bckm) {
		/* Try to resize table. */
		TPP_REF tpp_keyword **new_table;
		tpp_hash i, new_mask = (self->tks_bckm << 1) | 1;
		if (new_mask < 15)
			new_mask = 15;
		new_table = (TPP_REF tpp_keyword **)tpp_trymalloc((new_mask + 1) * sizeof(TPP_REF tpp_keyword *));
		if tpp_unlikely(!new_table) {
			new_mask = (self->tks_bckm << 1) | 1;
			new_table = (TPP_REF tpp_keyword **)tpp_trymalloc((new_mask + 1) * sizeof(TPP_REF tpp_keyword *));
			if tpp_unlikely(!new_table) {
				if (self->tks_bckm != 0)
					goto do_insert;
				new_table = (TPP_REF tpp_keyword **)tpp_malloc((new_mask + 1) * sizeof(TPP_REF tpp_keyword *));
				if tpp_unlikely(!new_table)
					goto err_oom;
			}
		}
		tpp_memset(new_table, 0, (new_mask + 1) * sizeof(TPP_REF tpp_keyword *));

		/* Transfer "self->tks_bckv" (old table) into "new_table" */
		for (i = 0; i <= self->tks_bckm; ++i) {
			TPP_REF tpp_keyword *chain = self->tks_bckv[i];
			while (chain) {
				TPP_REF tpp_keyword *next = chain->tk_next;
				TPP_REF tpp_keyword **bucket = &new_table[chain->tk_hash & new_mask];
				chain->tk_next = *bucket;
				*bucket = chain;
				chain = next;
			}
		}

		/* Free old table and assume new one. */
		if (self->tks_bckv != tpp_keywords_empty_map)
			tpp_free(self->tks_bckv);
		self->tks_bckv = new_table;
		self->tks_bckm = new_mask;
	}

do_insert:
	tpp_assert(self->tks_bckm != 0);
	tpp_assert(self->tks_bckv != tpp_keywords_empty_map);
	{
		TPP_REF tpp_keyword **bucket;
		bucket = &self->tks_bckv[kwd->tk_hash & self->tks_bckm];
		kwd->tk_next = *bucket;
		*bucket = kwd;
	}
	++self->tks_kwdc;
	return kwd;
err_oom:
	tpp_keyword_destroy(kwd);
	return NULL;
}

/* Same as above, but if the keyword doesn't exist in `self' or the builtin
 * keyword table, a new keyword is allocated, given an ID, and inserted into `self'
 * @return: * :   The keyword associated with `kwd' (possibly having been just allocated)
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_newkeyword(tpp_keywords *tpp_restrict self,
                        tpp_char const *tpp_restrict kwd,
                        tpp_size len, tpp_hash hash) {
	tpp_keyword *result;
	result = (tpp_keyword *)tpp_keywords_getkeyword(self, kwd, len, hash);
	if (result != NULL)
		goto done;

	/* Must allocate a new keyword... */
	result = _tpp_keyword_alloc(len);
	if (result == NULL)
		goto done;

	result->tk_id = (tpp_token_id)((unsigned int)TPP_TOK_USERKEYWORD_BEGIN + self->tks_kwdc);
#if TPP_HAVE_CPP_MACROS
	result->tk_macro = NULL;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	result->tk_misc = NULL;
#endif /* TPP_HAVE_KEYWORD_MISC */
	result->tk_hash = hash;
	tpp_keyword_init_refcnt(result);
	result->tk_len = len;
	tpp_memcpy(result->tk_kwd, kwd, len * sizeof(tpp_char));
	result->tk_kwd[len] = (tpp_char)'\0';
	result = tpp_keywords_inskeyword(self, result);
done:
	return result;
}

#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_newkeyword_esc_(tpp_keywords *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM) {
	tpp_keyword *result;
	tpp_size len_without_esc;
	result = (tpp_keyword *)tpp_keywords_getkeyword_esc(self, kwd, len, hash, file);
	if (result != NULL)
		goto done;

	/* Must allocate a new keyword... */
	result = _tpp_keyword_alloc(len);
	if (result == NULL)
		goto done;

	result->tk_id = (tpp_token_id)((unsigned int)TPP_TOK_USERKEYWORD_BEGIN + self->tks_kwdc);
#if TPP_HAVE_CPP_MACROS
	result->tk_macro = NULL;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	result->tk_misc = NULL;
#endif /* TPP_HAVE_KEYWORD_MISC */
	result->tk_hash = hash;
	tpp_keyword_init_refcnt(result);
	len_without_esc = tpp_without_esc(result->tk_kwd, kwd, len, file);
	tpp_assert(len_without_esc <= len);
	result->tk_len = len_without_esc;
	if (len_without_esc < len) {
		tpp_keyword *new_result;
		new_result = _tpp_keyword_tryrealloc(result, len_without_esc);
		if tpp_likely(new_result)
			result = new_result;
	}
	result->tk_kwd[len_without_esc] = (tpp_char)'\0';
	result = tpp_keywords_inskeyword(self, result);
done:
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
/* Check if "kwd" is contained in "self".
 * If so: do nothing and simply re-return "kwd"
 *
 * Otherwise, assume that "kwd" is a "builtin" keyword (as returned
 * by `tpp_builtin_getkeyword()'), in which the keyword is copied,
 * inserted into "self", and said copy is returned.
 *
 * This function must be used to make a keyword "writable" (which is
 * required before its `tk_macro' / `tk_misc' fields can safely be
 * written to (and in the later case: all fields of a potentially
 * pointed-to `tpp_keyword_misc', too)
 *
 * @return: * :   A writable copy of "kwd"
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
tpp_keywords_copybuiltin(tpp_keywords *tpp_restrict self,
                         tpp_keyword const *tpp_restrict kwd) {
	TPP_REF tpp_keyword *result;

	/* Check for simple case: if it's a user-defined keyword,
	 * it can't exist in built-ins, meaning it never needs to
	 * be copied. */
	if (TPP_TOK_ISUSERKEYWORD(kwd->tk_id))
		return (tpp_keyword *)kwd;

	/* Check if "kwd" is already contained with out map. */
	result = self->tks_bckv[kwd->tk_hash & self->tks_bckm];
	for (; result; result = result->tk_next) {
		if (result == kwd)
			return result;
	}

	/* Yes: must copy "kwd" */
	result = _tpp_keyword_alloc(kwd->tk_len);
	if (result == NULL)
		goto done;

#if TPP_HAVE_KEYWORD_MISC
	result->tk_misc = NULL; /* Builtin keywords never have "misc" data (flags are dynamically calculated) */
#endif /* TPP_HAVE_KEYWORD_MISC */
	result->tk_id = kwd->tk_id;
#if TPP_HAVE_CPP_MACROS
	result->tk_macro = kwd->tk_macro;
	if (result->tk_macro)
		tpp_macro_incref(result->tk_macro);
#endif /* TPP_HAVE_CPP_MACROS */
	result->tk_hash = kwd->tk_hash;
	tpp_keyword_init_refcnt(result);
	result->tk_len = kwd->tk_len;
	tpp_memcpy(result->tk_kwd, kwd->tk_kwd, (kwd->tk_len + 1) * sizeof(tpp_char));
	result = tpp_keywords_inskeyword(self, result);
done:
	return result;
}
#endif /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */



#if TPP_HAVE_KEYWORDS_OPENFILE
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) /*utf-8*/char *TPPCALL
tpp_fs_normalize(/*utf-8*/ char *dst_iter,  /* Output pointer destination buffer (with at least "srclen" char-s of space) */
                 /*utf-8*/ char *dst_base,  /* Base pointer of destination buffer (start of destination filename string) */
                 /*utf-8*/ char const *src, /* Filename string to append to "dst_iter" (".."-refs don't go beyond "dst_base") */
                 tpp_size srclen) {         /* Length of "src" (in char-s) */
	char const *src_end = src + srclen;

#if TPP_FS_HAVE_DRIVES
	if (src >= src_end)
		goto done;
	if (TPP_FS_ISABS(src, srclen)) {
		dst_iter = dst_base;
		*dst_iter++ = *src++;
		*dst_iter++ = *src++;
	}
#endif /* TPP_FS_HAVE_DRIVES */

	/* Deal with leading slashes. */
	for (;;) {
		if (src >= src_end)
			goto done;
		if (!TPP_FS_ISSEP(*src))
			break;
		++src;
		if (dst_iter <= dst_base || dst_iter[-1] != TPP_FS_SEP)
			*dst_iter++ = TPP_FS_SEP;
	}

	for (;;) {
		char const *next_sep;
		tpp_size segment_len;
		if (src >= src_end)
			goto done;

		/* At this point, "src" is at the start of some path-component,
		 * and "dst_iter" points at:
		 * - The start (dst_base)
		 * - After a TPP_FS_SEP
		 * - Just after the drive base (in case of TPP_FS_HAVE_DRIVES) */
		next_sep = src;
		while (next_sep < src_end && !TPP_FS_ISSEP(*next_sep))
			++next_sep;
		segment_len = (tpp_size)(next_sep - src);
		while (next_sep < src_end && TPP_FS_ISSEP(*next_sep))
			++next_sep; /* Skip over trailing SEP of segment */

		/* Deal with special segments. */
		switch (segment_len) {
		case 0:
			goto continue_with_next_sep; /* Empty segment -> ignore */
		case 1:
			if (src[0] == '.')
				goto continue_with_next_sep; /* Current-directory-segment -> ignore */
			break;
		case 2:
			if (src[0] == '.' && src[1] == '.' && dst_iter > dst_base) {
				/* Parent-directory-segment -> delete 1 up-ref in "dst", but
				 * only if that up-ref isn't another ".." (or ".") sequence. */
				char *dst_seq_start;
				char *dst_seq_end = dst_iter;
				tpp_size dst_seq_len;
				while (dst_seq_end > dst_base && dst_seq_end[-1] == TPP_FS_SEP)
					--dst_seq_end;
				dst_seq_start = dst_seq_end;
				while (dst_seq_start > dst_base && dst_seq_start[-1] != TPP_FS_SEP)
					--dst_seq_start;
				dst_seq_len = (tpp_size)(dst_seq_end - dst_seq_start);
				switch (dst_seq_len) {
				case 1:
					if (dst_seq_start[0] == '.')
						goto append_to_dst_iter; /* Can't delete "." */
					break;
				case 2:
					if (dst_seq_start[0] == '.' && dst_seq_start[1] == '.')
						goto append_to_dst_iter; /* Can't delete ".." */
					break;
				default: break;
				}
				dst_iter = dst_seq_start;
				goto continue_with_next_sep;
			}
			break;
		default: break;
		}

		/* Copy segment into "dst_iter" */
append_to_dst_iter:
		tpp_memcpy(dst_iter, src, segment_len * sizeof(char));
		dst_iter += segment_len;
		if (next_sep >= src_end)
			goto done;

		/* Append SEP to "dst_iter" */
		*dst_iter++ = TPP_FS_SEP;

continue_with_next_sep:
		src = next_sep;
	}


done:
	/* Remove trailing SEP */
	if (dst_iter > dst_base && dst_iter[-1] == TPP_FS_SEP)
		--dst_iter;
	return dst_iter;
}

#if TPP_HAVE_KEYWORDS_OPENFILE_EX
#if TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next
#if TPP_HAVE_CPP_IMPORT
TPP_STATIC_ASSERT(TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT != TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED);
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_PRAGMA_ONCE
TPP_STATIC_ASSERT(TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT != TPP_LEXER_OPENFILE_FLAG_HDR_ONCE);
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
TPP_STATIC_ASSERT(TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT != TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED);
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next */

/* Same as `tpp_lexer_openfile', but return `TPP_EMASKED' if the file was already
 * included before, and its keyword has any of the bits specified by `mask_flags' set.
 *
 * A special case is made when "mask_flags & TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED",
 * in which case, "TPP_EMASKED" is only returned if "tkm_file_guard" is a macro that
 * is currently considered to be `#if defined()'.
 *
 * Another special case is made for "TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT", which
 * causes "TPP_EMASKED" to be returned if the file's keyword is already included
 * somewhere on the #include-stack.
 *
 * @param: mask_flags: Set of flags describing circumstances under which TPP_EMASKED
 *                     should be returned:
 *                     - TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED
 *                     - TPP_LEXER_OPENFILE_FLAG_HDR_ONCE
 *                     - TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED
 *                     - TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT
 *
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOMEM:  Insufficient memory
 * @return: TPP_ENOENT:  No such file, or TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT was
 *                       given, and the file is already located on the #include-stack.
 * @return: TPP_EMASKED: Flags specified by "mask_flags" were already set */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3, 5)) tpp_errno TPPCALL
tpp_lexer_openfile_ex(/*1..1*/ tpp_lexer *tpp_restrict self,
                      /*0..1*/ char const *tpp_restrict relative_to,
                      /*1..1*/ /*utf-8*/ char const *filename, tpp_size filename_maxlen,
                      /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result,
                      tpp_lexer_openfile_flags mask_flags)
#else /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
/* Construct the filename, open the file, and initialize "result" accordingly
 * @param: relative_to: The `tpp_file::tf_data.td_io.tff_name' of another file,
 *                      in case "filename" is a relative path, in which case the
 *                      filename of the file to open should be relative to the
 *                      directory of "relative_to"
 * @param: result:      Open file information (pass along to "tpp_file_init_io()")
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Insufficient memory
 * @return: TPP_ENOENT: File not found (if you have additional "relative_to", try them) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3, 5)) tpp_errno TPPCALL
tpp_lexer_openfile(/*1..1*/ tpp_lexer *tpp_restrict self,
                   /*0..1*/ char const *tpp_restrict relative_to,
                   /*1..1*/ /*utf-8*/ char const *filename, tpp_size filename_maxlen,
                   /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result)
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE_EX */
{
	bool is_known_keyword = false;
	tpp_io_handle handle;
	tpp_keyword *result_kwd;
	tpp_size filename_len = tpp_strnlen(filename, filename_maxlen);
	if (TPP_FS_ISABS(filename, filename_len) || !relative_to) {
		tpp_keyword *new_result_kwd;
		tpp_char *kwd_end;
		tpp_size result_kwd_len;
without_relative_to:
		result_kwd = _tpp_keyword_alloc(filename_len);
		if tpp_unlikely(!result_kwd)
			goto err_nomem;
		kwd_end = (tpp_char *)tpp_fs_normalize((char *)result_kwd->tk_kwd,
		                                       (char *)result_kwd->tk_kwd,
		                                       filename, filename_len);
		*kwd_end = (tpp_char)'\0';
		result_kwd_len = (tpp_size)(kwd_end - result_kwd->tk_kwd);
		tpp_assert(result_kwd_len <= filename_len);
		new_result_kwd = _tpp_keyword_tryrealloc(result_kwd, result_kwd_len);
		if tpp_likely(new_result_kwd)
			result_kwd = new_result_kwd;
		result_kwd->tk_len = result_kwd_len;
	} else {
		tpp_keyword *new_result_kwd;
		tpp_size rel_size, whole_size;
		char const *rel_base = relative_to;
		char const *last_sep = rel_base + tpp_strlen(relative_to);
		char *dst_base, *dst_iter, *dst_end;
		while (last_sep > rel_base && last_sep[-1] != TPP_FS_SEP)
			--last_sep;
		if (last_sep <= rel_base)
			goto without_relative_to;
		rel_size   = (tpp_size)(last_sep - rel_base); /* Including trailing '/' */
		whole_size = rel_size + filename_len;
		result_kwd = _tpp_keyword_alloc(whole_size);
		if tpp_unlikely(!result_kwd)
			goto err_nomem;
		dst_base = (char *)result_kwd->tk_kwd;
		tpp_memcpy(dst_base, rel_base, rel_size * sizeof(char)); /* Including trailing '/' */
		dst_iter = dst_base + rel_size;
		dst_end = tpp_fs_normalize(dst_iter, dst_base, filename, filename_len);
		*dst_end = '\0';
		whole_size = (tpp_size)(dst_end - dst_base);
		new_result_kwd = _tpp_keyword_tryrealloc(result_kwd, whole_size);
		if tpp_likely(new_result_kwd)
			result_kwd = new_result_kwd;
		result_kwd->tk_len = whole_size;
	}

	/* FIXME: Windows has a case-insensitive filesystem, but the filename hash used here
	 *        is (and has to be for the sake of allowing us to re-use the keyword table)
	 *        case-sensitive. */

	/* Check if "result_kwd" is a known keyword... */
	{
		tpp_hash hash = tpp_hashof(result_kwd->tk_kwd, result_kwd->tk_len);
		tpp_keyword *bucket = self->tl_kwds.tks_bckv[hash & self->tl_kwds.tks_bckm];
		for (; bucket; bucket = bucket->tk_next) {
			if (bucket->tk_hash != hash)
				continue;
			if (bucket->tk_len != result_kwd->tk_len)
				continue;
			if (tpp_memcmp(bucket->tk_kwd, result_kwd->tk_kwd,
			               result_kwd->tk_len * sizeof(tpp_char)) != 0)
				continue;

			/* Keyword already exists */
			_tpp_keyword_free(result_kwd);
			is_known_keyword = true;
			result_kwd = bucket;

			/* Check if the file should be marked out. */
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
			if ((result_kwd->tk_misc) != NULL &&
			    (result_kwd->tk_misc->tkm_flags & mask_flags) != 0) {
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
				if (mask_flags & TPP_KEYWORD_FLAG_HDR_GUARD_VALID) {
					tpp_keyword const *file_guard = result_kwd->tk_misc->tkm_file_guard;
					tpp_assert(file_guard != NULL && "'TPP_KEYWORD_FLAG_HDR_GUARD_VALID' is "
					                                 "set, but 'tkm_file_guard == NULL'");
					if ((result_kwd->tk_misc->tkm_flags & (mask_flags & ~TPP_KEYWORD_FLAG_HDR_GUARD_VALID)) != 0)
						return TPP_EMASKED; /* File is masked even if it wasn't for the header guard. */
					if (tpp_lexer_getkeyworddefined(self, file_guard))
						return TPP_EMASKED; /* File guard is still defined -> don't include */
				} else
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
				{
					return TPP_EMASKED;
				}
			}
#if TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next
			if (mask_flags & TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT) {
				/* Check if this file is already being #include-ed */
				tpp_file const *fp = tpp_lexer_getfile(self);
				do {
					if (fp->tf_kind == TPP_FILE_KIND_IO &&
#if TPP_HAVE_FILE_NOKWD
					    !(fp->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
					    fp->tf_data.td_io.tff_name != NULL) {
						tpp_keyword const *kwd = (tpp_keyword const *)((char const *)fp->tf_data.td_io.tff_name -
						                                               tpp_offsetof(tpp_keyword, tk_kwd));
						if (kwd == result_kwd)
							return TPP_ENOENT; /* File is already on #include-stack */
					}
				} while ((fp = fp->tf_tprev) != NULL);
			}
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next */
#endif /* TPP_HAVE_KEYWORDS_OPENFILE_EX */

			goto got_result_kwd;
		}
		result_kwd->tk_hash = hash;
	}
got_result_kwd:

	/* Try to open the file */
	handle = tpp_io_open((char const *)result_kwd->tk_kwd);
	if (handle == tpp_io_handle_INVALID) {
		_tpp_keyword_free(result_kwd);
		return TPP_ENOENT;
	}

	/* Initialize remaining fields of "result_kwd" and insert into keyword map */
	if (!is_known_keyword) {
		result_kwd->tk_id = (tpp_token_id)((unsigned int)TPP_TOK_USERKEYWORD_BEGIN +
		                                   self->tl_kwds.tks_kwdc);
#if TPP_HAVE_CPP_MACROS
		result_kwd->tk_macro = NULL;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
		result_kwd->tk_misc = NULL;
#endif /* TPP_HAVE_KEYWORD_MISC */
		tpp_keyword_init_refcnt(result_kwd);
		result_kwd = tpp_keywords_inskeyword(&self->tl_kwds, result_kwd);
		if tpp_unlikely(!result_kwd) {
			tpp_io_close(handle);
			goto err_nomem;
		}
	}

	/* Initialize "result" */
	result->tlofr_filename = result_kwd;
	result->tlofr_handle   = handle;
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_HAVE_KEYWORDS_OPENFILE */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_KEYWORD_C */
