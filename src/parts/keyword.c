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
#include "preparse.h"
#include "string.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* Assert that `tpp_keyword` and `tpp_string` are binary-compatible. */
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
		if (_TPP_KEYWORD_MACRO_ISDEFINED(mac))
			tpp_macro_decref(mac);
	}
	tpp_free(self->tmps_vec);
	tpp_dbg_memset(self, sizeof(*self));
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
		dst->tmpe_macro = src->tmpe_macro;
		if (_TPP_KEYWORD_MACRO_ISDEFINED(dst->tmpe_macro)) {
			dst->tmpe_macro = tpp_macro_copy(src->tmpe_macro);
			if tpp_unlikely(!dst->tmpe_macro) {
				while (i--) {
					dst = &vec[i];
					if (_TPP_KEYWORD_MACRO_ISDEFINED(dst->tmpe_macro))
						tpp_macro_decref(dst->tmpe_macro);
				}
				tpp_free(vec);
				return TPP_ENOMEM;
			}
		}
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_COPY */


/* Allocate space for- and return a new (uninitialized) macro-push entry
 * @return: * :   The newly allocated macro-push entry.
 * @return: NULL: Out-of-memory (`TPP_ENOMEM`) */
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


#if TPP_HAVE_CPP_ASSERT
#define tpp_assertions_hashinit(p_hs, p_perturb, hash, HMASK) \
	(void)(*(p_hs) = (*(p_perturb) = (hash)) & (HMASK))
#define tpp_assertions_hashnext(p_hs, p_perturb, hash, HMASK) \
	(void)(*(p_hs) = (*(p_hs) << 2) + *(p_hs) + *(p_perturb) + 1, *(p_perturb) >>= 5)
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_assertions_insert(tpp_assertions *tpp_restrict self,
                      tpp_keyword const *tpp_restrict value) {
	tpp_hash const hash = value->tk_hash;
	tpp_hash hs, perturb;
	for (tpp_assertions_hashinit(&hs, &perturb, hash, self->tass_bckm);;
	     tpp_assertions_hashnext(&hs, &perturb, hash, self->tass_bckm)) {
		tpp_assertion *ent = &self->tass_bckv[hs & self->tass_bckm];
		tpp_assert(ent->tas_value != value && "Already inserted");
		if (ent->tas_value == NULL) {
			ent->tas_value = value;
			break;
		}
	}
}

#if TPP_HAVE_LEXER_COPY
static TPP_CONSTCALL TPP_WUNUSED tpp_hash TPPCALL
tpp_assertions_maskfor(tpp_size count) {
	tpp_hash result = 0;
	while (result < count)
		result = (result << 1) | 1;
	return result;
}

/* Copy the given set of assertions
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_assertions_copy(tpp_assertions *tpp_restrict self,
                    tpp_assertions const *tpp_restrict from) {
	tpp_assert(from->tass_assc <= from->tass_bckm);
	self->tass_assc = from->tass_assc;
	self->tass_bckm = 0;
	self->tass_bckv = NULL;
	if (self->tass_assc) {
		tpp_hash usemask = tpp_assertions_maskfor(self->tass_assc);
		tpp_hash mapsize = (usemask + 1) * sizeof(tpp_assertion);
		tpp_assertion *vec = (tpp_assertion *)tpp_malloc(mapsize);
		if tpp_unlikely(!vec)
			return TPP_ENOMEM;
		self->tass_bckm = usemask;
		self->tass_bckv = vec;
		if (from->tass_bckm == usemask) {
			tpp_memcpy(vec, from->tass_bckv, mapsize);
		} else {
			tpp_hash i;
			tpp_bzero(vec, mapsize);
			for (i = 0; i <= from->tass_bckm; ++i) {
				tpp_keyword const *kwd = from->tass_bckv[i].tas_value;
				if (kwd)
					tpp_assertions_insert(self, kwd);
			}
		}
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_COPY */

/* Check if a given `value` is being asserted by `self` */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_assertions_contains(tpp_assertions const *tpp_restrict self,
                        struct tpp_keyword const *tpp_restrict value) {
	tpp_hash const hash = value->tk_hash;
	tpp_hash hs, perturb;
	for (tpp_assertions_hashinit(&hs, &perturb, hash, self->tass_bckm);;
	     tpp_assertions_hashnext(&hs, &perturb, hash, self->tass_bckm)) {
		tpp_assertion const *ent = &self->tass_bckv[hs & self->tass_bckm];
		if (ent->tas_value == NULL)
			break;
		if (tpp_keyword_equals(ent->tas_value, value))
			return true;
	}
	return false;
}


/* Assert a given `value` within `self`.
 * @return: TPP_EOK:    Assertion was added
 * @return: TPP_ENOENT: Assertion was already added before (SOFT_ERROR)
 * @return: TPP_ENOMEM: Out of memory (HARD_ERROR) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_assertions_assert(tpp_assertions *tpp_restrict self,
                      struct tpp_keyword const *tpp_restrict value) {
	if (self->tass_bckv) {
		tpp_hash const hash = value->tk_hash;
		tpp_hash hs, perturb;
		for (tpp_assertions_hashinit(&hs, &perturb, hash, self->tass_bckm);;
		     tpp_assertions_hashnext(&hs, &perturb, hash, self->tass_bckm)) {
			tpp_assertion const *ent = &self->tass_bckv[hs & self->tass_bckm];
			if (ent->tas_value == NULL)
				break;
			if (tpp_keyword_equals(ent->tas_value, value))
				return TPP_ENOENT; /* Already contained */
		}
	}
	if (self->tass_assc >= self->tass_bckm) {
		/* Must rehash */
		tpp_assertion *oldmap = self->tass_bckv;
		tpp_hash oldmask = self->tass_bckm;
		tpp_hash newmask = (oldmask << 1) | 1;
		tpp_hash mapsize = (newmask + 1) * sizeof(tpp_assertion);
		tpp_assertion *newmap = (tpp_assertion *)tpp_malloc(mapsize);
		if tpp_unlikely(!newmap)
			return TPP_ENOMEM;
		tpp_bzero(newmap, mapsize);
		self->tass_bckm = newmask;
		self->tass_bckv = newmap;
		if (oldmap) {
			tpp_hash i;
			for (i = 0; i <= oldmask; ++i) {
				tpp_keyword const *v = oldmap[i].tas_value;
				if (v)
					tpp_assertions_insert(self, v);
			}
			tpp_free(oldmap);
		}
	}
	tpp_assertions_insert(self, value);
	++self->tass_assc;
	return TPP_EOK;
}

static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_assertions_fixtable(tpp_assertions *tpp_restrict self) {
	tpp_hash i;
	bool result = false;
	for (i = 0; i <= self->tass_bckm; ++i) {
		tpp_assertion *ent = &self->tass_bckv[i];
		tpp_keyword const *kwd = ent->tas_value;
		if (kwd) {
			tpp_hash const hash = kwd->tk_hash;
			tpp_hash hs, perturb;
			for (tpp_assertions_hashinit(&hs, &perturb, hash, self->tass_bckm);;
				 tpp_assertions_hashnext(&hs, &perturb, hash, self->tass_bckm)) {
				tpp_assertion *ent2 = &self->tass_bckv[hs & self->tass_bckm];
				if (ent2->tas_value == NULL) {
					ent2->tas_value = kwd;
					ent->tas_value = NULL;
					result = true;
					break;
				} else {
					tpp_assert(tpp_keyword_equals(ent2->tas_value, kwd) == (ent2 == ent));
					if (ent2 == ent)
						break;
				}
			}
		}
	}
	return result;
}

/* Unassert a given `value` within `self`.
 * @return: true:  Assertion was removed
 * @return: false: Assertion didn't exist in the first place */
TPP_IMPL TPP_NONNULL((1, 2)) bool TPPCALL
tpp_assertions_unassert(tpp_assertions *tpp_restrict self,
                        struct tpp_keyword const *tpp_restrict value) {
	tpp_assertion *ent;
	tpp_hash const hash = value->tk_hash;
	tpp_hash hs, perturb;
	if (!self->tass_assc)
		return false;
	tpp_assert(self->tass_bckv);
	for (tpp_assertions_hashinit(&hs, &perturb, hash, self->tass_bckm);;
	     tpp_assertions_hashnext(&hs, &perturb, hash, self->tass_bckm)) {
		ent = &self->tass_bckv[hs & self->tass_bckm];
		if (ent->tas_value == NULL)
			return false;
		if (tpp_keyword_equals(ent->tas_value, value))
			break; /* Found it! */
	}

	/* Down-shift all entries that come after "ent" */
	do {
		tpp_assertion *next;
		tpp_assertions_hashnext(&hs, &perturb, hash, self->tass_bckm);
		next = &self->tass_bckv[hs & self->tass_bckm];
		*ent = *next;
		ent = next;
	} while (ent->tas_value);

	/* Fix hash table errors until there are no more. */
	while (tpp_assertions_fixtable(self))
		;

	/* Update assertion counter */
	--self->tass_assc;
	return true;
}
#endif /* TPP_HAVE_CPP_ASSERT */


#if TPP_HAVE_KEYWORD_MISC
/* Ensure that `self->tk_misc` has been allocated and return it.
 * If it isn't already allocated, allocate+return it lazily.
 * WARNING: Only call this function on a *writable* keyword (s.a. `tpp_keywords_copybuiltin()`)
 *
 * @return: * :   The *misc* data of `self` (freshly allocated)
 * @return: NULL: Out of memory (TPP_ENOMEM) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_misc *TPPCALL
tpp_keyword_requiremisc(tpp_keyword *tpp_restrict self) {
	tpp_keyword_misc *result = self->tk_misc;
	if tpp_unlikely(result == NULL) {
		result = _tpp_keyword_misc_alloc();
		if tpp_likely(result) {
			_tpp_keyword_misc_init(result);
			self->tk_misc = result;
		}
	}
	return result;
}
#endif /* TPP_HAVE_KEYWORD_MISC */


#if TPP_HAVE_KEYWORD_USERDATA
/* Set the user-data pointer for `self`
 * @param: destroy_prev: When true, and `tpp_keyword_getuserdata_dtor(self) != NULL`,
 *                       as well as `tpp_keyword_getuserdata(self) != NULL` on entry,
 *                       invoke the existing destructor on the old user-data after
 *                       assigning the new `ptr` and `dtor`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_setuserdata(tpp_keyword *tpp_restrict self,
                        void *ptr, void (TPPCALL *dtor)(void *ptr),
                        bool destroy_prev) {
	void *old_ptr;
	void (TPPCALL *old_dtor)(void *ptr);
	tpp_keyword_misc *misc;
	if (!ptr && !dtor && !self->tk_misc)
		return TPP_EOK;
	misc = tpp_keyword_requiremisc(self);
	if tpp_unlikely(!misc)
		return TPP_ENOMEM;
	old_ptr  = misc->tkm_userdata_ptr;
	old_dtor = misc->tkm_userdata_dtor;
	misc->tkm_userdata_ptr  = ptr;
	misc->tkm_userdata_dtor = dtor;
	if (destroy_prev && old_dtor && old_dtor)
		(*old_dtor)(old_ptr);
	return TPP_EOK;
}
#endif /* TPP_HAVE_KEYWORD_USERDATA */



#if TPP_HAVE_PRAGMA_PUSH_MACRO
/* Push the current macro-definition of `self`
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
	if (_TPP_KEYWORD_MACRO_ISDEFINED(ent->tmpe_macro))
		tpp_macro_incref(ent->tmpe_macro);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}


/* Pop the current macro-definition of `self`
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
	if (_TPP_KEYWORD_MACRO_ISDEFINED(last->tmpe_macro))
		tpp_macro_incref(last->tmpe_macro);
	if (_TPP_KEYWORD_MACRO_ISDEFINED(self->tk_macro))
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
/* Delete the macro definition of `self`. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keyword_undef(tpp_keyword *tpp_restrict self) {
	TPP_REF tpp_macro *old_macro;
	old_macro = self->tk_macro;
	self->tk_macro = _TPP_KEYWORD_MACRO_UNDEFINED;
	if (_TPP_KEYWORD_MACRO_ISDEFINED(old_macro))
		tpp_macro_decref(old_macro);
}

/* Similar to `tpp_keyword_undef()`, but only delete user-defined macro expansions,
 * and -- if there might be a builtin/predefined macro related to `self` -- that
 * macro is re-enabled. */
#if TPP_HAVE_CPP_BUILTIN_MACROS
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keyword_undefuser(tpp_keyword *tpp_restrict self) {
	TPP_REF tpp_macro *old_macro;
	old_macro = self->tk_macro;
	self->tk_macro = tpp_keyword_isbuiltin(self)
	                 ? _TPP_KEYWORD_MACRO_BUILTIN
	                 : _TPP_KEYWORD_MACRO_UNDEFINED;
	if (_TPP_KEYWORD_MACRO_ISDEFINED(old_macro))
		tpp_macro_decref(old_macro);
}
#endif /* TPP_HAVE_CPP_BUILTIN_MACROS */

#endif /* TPP_HAVE_CPP_MACROS */



#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
/* Set the keyword registered as #ifndef-guard of
 * the given (should-be) filename-keyword `self`
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_set_file_guard(tpp_keyword *self, tpp_keyword const *guard) {
	tpp_keyword_misc *const misc = tpp_keyword_requiremisc(self);
	if tpp_unlikely(!misc)
		return TPP_ENOMEM;
	misc->tkm_file_guard = guard;
	return TPP_EOK;
}
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */



#if TPP_HAVE_MACRO___TPP_COUNTER
/* Fetch+increment the __TPP_COUNTER() value of this keyword
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_keyword_inc_builtin_counter(tpp_keyword *tpp_restrict self,
                                tpp_counter *tpp_restrict p_result) {
	tpp_keyword_misc *const misc = tpp_keyword_requiremisc(self);
	if tpp_unlikely(!misc)
		return TPP_ENOMEM;
	*p_result = misc->tkm_builtin_counter++;
	return TPP_EOK;
}
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */



#if TPP_HAVE_KEYWORD_FLAGS
/* Set the flags (set of `TPP_KEYWORD_FLAG_*`) linked to `self`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_setflags(tpp_keyword *tpp_restrict self,
                     tpp_keyword_flags flags) {
	tpp_keyword_misc *const misc = tpp_keyword_requiremisc(self);
	if tpp_unlikely(!misc)
		return TPP_ENOMEM;
	misc->tkm_flags = flags;
	return TPP_EOK;
}
#endif /* TPP_HAVE_KEYWORD_FLAGS */



#if TPP_HAVE_KEYWORD_FEATURES
/* Return the string-expansion of the feature-check `kind` when
 * given `self` as an argument. Returns `NULL` when no custom
 * expansion has been defined (caller must still check for pre-
 * defined/builtin feature-check expansions)
 *
 * You should probably call `tpp_lexer_getkeywordfeature()`
 * instead of this function, since this one doesn't handle
 * builtin expansions!
 *
 * @return: * :   The custom override for what `self` should expand to
 * @return: NULL: No custom override present. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) TPP_REF tpp_string *TPPCALL
tpp_keyword_getfeature(tpp_keyword const *tpp_restrict self,
                       tpp_keyword_feature_kind kind) {
	tpp_keyword_misc const *misc = self->tk_misc;
	if (misc) {
		tpp_keyword_feature const *feat;
		feat = _tpp_keyword_misc_featurebykind(misc, kind);
		if (feat->tkf_expansion) {
			tpp_string_incref(feat->tkf_expansion);
			return feat->tkf_expansion;
		}
	}
	return NULL;
}

/* Set the text that a feature-check of `kind` expands to when used
 * with the given keyword `self`. You may also pass `NULL` for `value`
 * to reset that specific feature back to its builtin (or `0`) state.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_setfeature(tpp_keyword *tpp_restrict self,
                       tpp_keyword_feature_kind kind,
                       tpp_string *value) {
	TPP_REF tpp_string *old_value;
	tpp_keyword_feature *feat;
	tpp_keyword_misc *misc = tpp_keyword_requiremisc(self);
	if (!misc)
		return TPP_ENOMEM;
	feat = _tpp_keyword_misc_featurebykind(misc, kind);
	old_value = feat->tkf_expansion;
	feat->tkf_expansion = value;
	if (value)
		tpp_string_incref(value);
	if (old_value)
		tpp_string_decref(old_value);
	return TPP_EOK;
}
#endif /* TPP_HAVE_KEYWORD_FEATURES */



#if TPP_HAVE_CPP_ASSERT
/* Assert a given `value` within `self`.
 * @return: TPP_EOK:    Assertion was added
 * @return: TPP_ENOENT: Assertion was already added before (SOFT_ERROR)
 * @return: TPP_ENOMEM: Out of memory (HARD_ERROR) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_keyword_addassert(tpp_keyword *self, tpp_keyword const *value) {
	tpp_keyword_misc *const misc = tpp_keyword_requiremisc(self);
	if tpp_unlikely(!misc)
		return TPP_ENOMEM;
	return tpp_assertions_assert(&misc->tkm_assertions, value);
}
#endif /* TPP_HAVE_CPP_ASSERT */




/* Calculate the hash of a given keyword string */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof(tpp_char const *tpp_restrict kwd, tpp_size len) {
	tpp_hash result = TPP_HASH_INITIAL;
	tpp_size i;
	for (i = 0; i < len; ++i) {
		tpp_char ch = kwd[i];
		result = tpp_hash_combine_char(result, ch);
	}
	return result;
}


#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
#if _TPP_HAVE_BSE_FILE_PARAM || TPP_CONF_ISRT(TPP_HAVE_TRIGRAPHS)
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_char const *TPPCALL
tpp_decode_find_unmatched_rbrace(tpp_char const **p_iter, tpp_char const *end, tpp_lexer const *lexer);
#else /* _TPP_HAVE_BSE_FILE_PARAM || TPP_CONF_ISRT(TPP_HAVE_TRIGRAPHS) */
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
_tpp_decode_find_unmatched_rbrace(tpp_char const **p_iter, tpp_char const *end)
#define tpp_decode_find_unmatched_rbrace(p_iter, end, lexer) \
	_tpp_decode_find_unmatched_rbrace(p_iter, end);
#endif /* !_TPP_HAVE_BSE_FILE_PARAM && !TPP_CONF_ISRT(TPP_HAVE_TRIGRAPHS) */
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED */



#if TPP_HAVE_ESCAPED_KEYWORDS
#if TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
#define TPP_DECODE_BSI_MAXLEN (TPP_DECODE_NAMED_ESCAPE_MAXLEN * TPP_UTF8_MAXLEN)
#else /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED */
#define TPP_DECODE_BSI_MAXLEN TPP_UTF8_MAXLEN
#endif /* !TPP_HAVE_IDENTIFIER_ESCAPE_NAMED */

#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
_tpp_decode_bsi_continue(tpp_char buf[TPP_DECODE_BSI_MAXLEN], tpp_char const **p_iter,
                         tpp_char const *end _tpp_esc_lexer__PARAM,
                         bool *tpp_restrict p_continue)
#define tpp_decode_bsi_continue(buf, p_iter, end, lexer, p_continue) \
	_tpp_decode_bsi_continue(buf, p_iter, end _tpp_esc_lexer__ARG(lexer), p_continue)
#else /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
_tpp_decode_bsi_continue(tpp_char buf[TPP_DECODE_BSI_MAXLEN], tpp_char const **p_iter,
                         tpp_char const *end _tpp_esc_lexer__PARAM)
#define tpp_decode_bsi_continue(buf, p_iter, end, lexer, p_continue) \
	_tpp_decode_bsi_continue(buf, p_iter, end _tpp_esc_lexer__ARG(lexer))
#endif /* !TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
{
	tpp_size named_index, named_count;
	tpp_unichar named_uc[TPP_DECODE_NAMED_ESCAPE_MAXLEN];
	tpp_char *buf_iter;
	tpp_char const *named_start = *p_iter;
	tpp_char const *named_end = tpp_decode_find_unmatched_rbrace(p_iter, end, lexer);

	/* Decode named sequence */
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
	named_start = tpp_preparse_skipspace_fwd(lexer, named_start, named_end);
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
	named_count = tpp_decode_named_escape(&named_start, named_end, named_uc, lexer);
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
	if (!named_count && tpp_lexer_has(lexer, IDENTIFIER_ESCAPE_NAMED_MANY)) {
		/* See if we can seek ahead to a ','-character */
		tpp_size remaining = (tpp_size)(named_end - named_start);
		tpp_char const *comma = (tpp_char const *)tpp_memchr(named_start, ',', remaining);
		if (comma)
			named_start = comma;
	}
	named_start = tpp_preparse_skipspace_fwd(lexer, named_start, named_end);
	if (named_start < named_end && *named_start == ',') {
		named_start = tpp_preparse_skipspace_fwd(lexer, named_start + 1, named_end);
		*p_continue = true;
	} else {
		*p_continue = (named_count && named_start < named_end);
	}
	if (*p_continue)
		*p_iter = named_start;
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */

	/* (re-)encode as utf-8 */
	if (named_count == 0) {
		/* Only get here when `tpp_lexer_skip_bsi()` has previously emitted a warning.
		 * For technical reasons, we can't return "0" because our caller would think that
		 * we didn't decode anything, and would proceed to append the leading \-character
		 * to the resulting keyword string (which would be even less correct). */
		named_uc[0] = 0xFFFD; /* U+FFFD: REPLACEMENT CHARACTER */
		named_count = 1;
	}
	buf_iter = buf;
	for (named_index = 0; named_index < named_count; ++named_index) {
		tpp_unichar uc = named_uc[named_index];
		buf_iter = tpp_unicode_writeutf8(buf_iter, uc);
	}
	return (tpp_size)(buf_iter - buf);
}
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED */



/* Check if `*p_iter` is a BSI sequence.
 * - If so, decode it, update `*p_iter` to point to its end,
 *   and return the # of utf-8 bytes written to `buf`
 * - Otherwise, return `0`
 *
 * NOTE: Given `*p_iter` points **AFTER** the leading `\` character
 *
 * Caller must ensure that `_tpp_maybe_isbackslash((*p_iter)[-1])` */
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
#define tpp_decode_bsi(buf, p_iter, end, lexer, p_continue) \
	tpp_decode_bsi_(buf, p_iter, end _tpp_esc_lexer__ARG(lexer), p_continue)
#else /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
#define tpp_decode_bsi(buf, p_iter, end, lexer, p_continue) \
	tpp_decode_bsi_(buf, p_iter, end _tpp_esc_lexer__ARG(lexer))
#endif /* !TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_decode_bsi_(tpp_char buf[TPP_DECODE_BSI_MAXLEN], tpp_char const **p_iter,
                tpp_char const *end _tpp_esc_lexer__PARAM
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
                , bool *tpp_restrict p_continue
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
                ) {
	tpp_char ch;
	tpp_char const *iter = *p_iter;
	tpp_assert(_tpp_maybe_isbackslash(iter[-1]));
	if (iter >= end)
		goto nope;
	iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
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
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
	*p_continue = false;
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */

#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
	if (ch == 'N') {
		iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
		if (iter >= end)
			goto nope;
		ch = *iter++;
		if (ch != '{')
			goto nope;
		iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
		if (iter >= end)
			goto nope;
		*p_iter = iter;
		return tpp_decode_bsi_continue(buf, p_iter, end, lexer, p_continue);
	} else
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED */
	{
#if TPP_HAVE_IDENTIFIER_ESCAPE_UNI
		tpp_unichar uc;
		unsigned int cur_digit;
		unsigned int max_digit;
		if (ch == 'u') {
			max_digit = 4;
		} else if (ch == 'U') {
			max_digit = 8;
		} else {
			goto nope;
		}
		iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
		if (iter >= end)
			goto nope;
		ch        = *iter++;
		uc        = 0;
		cur_digit = 0;
		for (;;) {
			uint_least8_t nibble;
			if (tpp_ascii_isdigit(ch)) {
				nibble = (uint_least8_t)tpp_ascii_asdigit(ch);
			} else if (tpp_ascii_islwrxdigit(ch)) {
				nibble = (uint_least8_t)tpp_ascii_aslwrxdigit(ch);
			} else if (tpp_ascii_isuprxdigit(ch)) {
				nibble = (uint_least8_t)tpp_ascii_asuprxdigit(ch);
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
			iter = tpp_preparse_skipbse_fwd(lexer, iter, end);
			if (iter >= end)
				break;
			ch = *iter++;
		}

		/* Indicate to caller that the BSI sequence was consumed. */
		*p_iter = iter;

		/* (re-)encode as utf-8 */
		return (tpp_size)(tpp_unicode_writeutf8(buf, uc) - buf);
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_UNI */
	}
nope:
	return 0;
}
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED */


/* Same as `tpp_hashof()`, but skip over `\`-escaped linefeeds when calculating the hash */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof_esc_(tpp_char const *tpp_restrict kwd, tpp_size len _tpp_esc_lexer__PARAM) {
	tpp_hash result = TPP_HASH_INITIAL;
	tpp_char const *end = kwd + len;
	while (kwd < end) {
		tpp_char ch = *kwd++;
#if TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
		tpp_char bsi[TPP_DECODE_BSI_MAXLEN];
		tpp_size bsi_len;
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
		bool bsi_continue;
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
		if ((_tpp_maybe_isbackslash(ch)) &&
		    (bsi_len = tpp_decode_bsi(bsi, (tpp_char const **)&kwd, end, lexer, &bsi_continue)) != 0) {
			for (;;) {
				tpp_size i = 0;
				do {
					tpp_char bsi_ch = bsi[i];
					result = tpp_hash_combine_char(result, bsi_ch);
				} while (++i < bsi_len);
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
				if (bsi_continue) {
					bsi_len = tpp_decode_bsi_continue(bsi, (tpp_char const **)&kwd, end, lexer, &bsi_continue);
					continue;
				}
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
				break;
			}
		} else
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED */
		{
			result = tpp_hash_combine_char(result, ch);
		}
		kwd = tpp_preparse_skipbse_fwd(lexer, kwd, end);
	}
	return result;
}

/* Copy `in_text...+=len` to `out_text`, whilst removing `\`-escaped linefeeds
 * The caller must ensure that `out_text` has space for at least `len` bytes,
 * and the actual # of used bytes of `out_text` is returned. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_without_esc_(tpp_char *tpp_restrict out_text,
                 tpp_char const *tpp_restrict in_text,
                 tpp_size len _tpp_esc_lexer__PARAM) {
	tpp_size result = 0;
	tpp_char const *end = in_text + len;
	while (in_text < end) {
		tpp_char ch = *in_text++;
#if TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
		tpp_size bsi_len;
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
		bool bsi_continue;
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
		if ((_tpp_maybe_isbackslash(ch)) &&
		    (bsi_len = tpp_decode_bsi(out_text + result, (tpp_char const **)&in_text, end, lexer, &bsi_continue)) != 0) {
			for (;;) {
				result += bsi_len;
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
				if (bsi_continue) {
					bsi_len = tpp_decode_bsi_continue(out_text + result, (tpp_char const **)&in_text, end, lexer, &bsi_continue);
					continue;
				}
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
				break;
			}
		} else
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED */
		{
			out_text[result++] = ch;
		}
		in_text = tpp_preparse_skipbse_fwd(lexer, in_text, end);
	}
	return result;
}

/* Compare 2 strings, one of which may contain `\`-escaped linefeeds that must be skipped. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) int TPPCALL
tpp_memcmp_esc_(tpp_char const *lhs_without_esc, tpp_size lhs_len,
                tpp_char const *rhs_with_esc, tpp_size rhs_len
                _tpp_esc_lexer__PARAM) {
	tpp_char const *lhs_end = lhs_without_esc + lhs_len;
	tpp_char const *rhs_end = rhs_with_esc + rhs_len;
	while (rhs_with_esc < rhs_end) {
		tpp_char lhs_ch, rhs_ch;
		if (lhs_without_esc >= lhs_end)
			return -1;
		lhs_ch = *lhs_without_esc++;
		rhs_ch = *rhs_with_esc++;
		if (lhs_ch != rhs_ch) {
#if TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED
			tpp_char bsi[TPP_DECODE_BSI_MAXLEN];
			tpp_size bsi_len;
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
			bool bsi_continue;
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
			if ((_tpp_maybe_isbackslash(rhs_ch)) &&
			    (bsi_len = tpp_decode_bsi(bsi, (tpp_char const **)&rhs_with_esc, rhs_end, lexer, &bsi_continue)) != 0) {
				for (;;) {
					tpp_size i = 0;
					for (;;) {
						if (lhs_ch != bsi[i])
							return lhs_ch < bsi[i] ? -1 : 1;
						if (lhs_without_esc >= lhs_end)
							return -1;
						if (++i >= bsi_len)
							break;
						lhs_ch = *lhs_without_esc++;
					}
#if TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY
					if (bsi_continue) {
						bsi_len = tpp_decode_bsi_continue(bsi, (tpp_char const **)&rhs_with_esc, rhs_end, lexer, &bsi_continue);
						continue;
					}
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_NAMED_MANY */
					break;
				}
			} else
#endif /* TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED */
			{
				return lhs_ch < rhs_ch ? -1 : 1;
			}
		}
		rhs_with_esc = tpp_preparse_skipbse_fwd(lexer, rhs_with_esc, rhs_end);
	}
	return 0;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


#if TPP_HAVE_KEYWORD_MISC
static TPP_NONNULL((1)) void TPPCALL
tpp_keyword_misc_destroy(tpp_keyword_misc *tpp_restrict self) {
#if TPP_HAVE_KEYWORD_FEATURES
	_tpp_keyword_features_fini(&self->tkm_features);
#endif /* TPP_HAVE_KEYWORD_FEATURES */
#if TPP_HAVE_CPP_ASSERT
	tpp_assertions_fini(&self->tkm_assertions);
#endif /* TPP_HAVE_CPP_ASSERT */
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

#if TPP_HAVE_USER_KEYWORDS
static TPP_NONNULL((1)) void TPPCALL
tpp_keyword_destroy(tpp_keyword *tpp_restrict self) {
#if TPP_HAVE_KEYWORD_ASSTRING
	tpp_assert(!tpp_refcnt_atomic_isshared(&self->tk_refcnt) && "Keyword still in use");
#endif /* TPP_HAVE_KEYWORD_ASSTRING */
#if TPP_HAVE_CPP_MACROS
	if (_TPP_KEYWORD_MACRO_ISDEFINED(self->tk_macro)) {
		tpp_assert(self->tk_macro->tm_expansions == 0 &&
		           "Macro still part of #include-stack?");
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


TPP_CONST_IMPL TPP_REF tpp_keyword *const _tpp_keywords_empty_map[1] = { NULL };

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_fini(tpp_keywords *tpp_restrict self) {
	TPP_REF tpp_keyword **bckv = self->tks_bckv;
	if (bckv != (tpp_keyword **)_tpp_keywords_empty_map) {
		tpp_hash i;
		for (i = 0; i <= self->tks_bckm; ++i) {
			TPP_REF tpp_keyword *chain = bckv[i];
			if (chain)
				tpp_keyword_destroychain(chain);
		}
		tpp_free(bckv);
	}
	tpp_dbg_memset(self, sizeof(*self));
}

#if TPP_HAVE_LEXER_COPY
#if TPP_HAVE_KEYWORD_MISC
static TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_misc *TPPCALL
tpp_keyword_copymisc(tpp_keyword_misc const *tpp_restrict self) {
	tpp_keyword_misc *result = _tpp_keyword_misc_alloc();
	if tpp_unlikely(!result)
		return NULL;
#if TPP_HAVE_PRAGMA_PUSH_MACRO
	{
		tpp_errno error = tpp_macro_pushstack_copy(&result->tkm_macro_pushstack,
		                                           &self->tkm_macro_pushstack);
		tpp_assert(!TPP_ISERR(error) || error == TPP_ENOMEM);
		if (TPP_ISERR(error)) {
			_tpp_keyword_misc_free(result);
			return NULL;
		}
	}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_CPP_ASSERT
	{
		tpp_errno error = tpp_assertions_copy(&result->tkm_assertions,
		                                      &self->tkm_assertions);
		tpp_assert(!TPP_ISERR(error) || error == TPP_ENOMEM);
		if (TPP_ISERR(error)) {
#if TPP_HAVE_PRAGMA_PUSH_MACRO
			tpp_macro_pushstack_fini(&result->tkm_macro_pushstack);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
			_tpp_keyword_misc_free(result);
			return NULL;
		}
	}
#endif /* TPP_HAVE_CPP_ASSERT */
#if TPP_HAVE_KEYWORD_FLAGS
	result->tkm_flags = self->tkm_flags;
#endif /* TPP_HAVE_KEYWORD_FLAGS */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
	result->tkm_file_guard = self->tkm_file_guard; /* Relocated into the new keyword-table later */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_KEYWORD_INCLCOUNT
	result->tkm_file_inclcount = 0; /* The #include-stack isn't copied, to this becomes "0" for everything */
#endif /* TPP_HAVE_KEYWORD_INCLCOUNT */
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

/* Copy+return `self`, but leave `return->tk_next` undefined */
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
	result->tk_macro = self->tk_macro;
	if (_TPP_KEYWORD_MACRO_ISDEFINED(self->tk_macro)) {
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
/* Relocate `self->tm_deffile` in case it references a keyword */
static TPP_NONNULL((1, 2, 3)) void TPPCALL
tpp_macro_relocate(tpp_macro *tpp_restrict self,
                   tpp_keywords const *tpp_restrict keywords,
                   tpp_keyword *tpp_restrict keyword) {
	tpp_char const *deffile = (tpp_char const *)self->tm_deffile;
	if (deffile) {
		tpp_size deffile_len = tpp_strlen((char const *)deffile);
		tpp_hash deffile_hash = tpp_hashof(deffile, deffile_len);
		tpp_keyword const *deffile_kwd;
		deffile_kwd = tpp_keywords_getkeyword(keywords, deffile,
		                                      deffile_len, deffile_hash);
		if (deffile_kwd)
			self->tm_deffile = tpp_keyword_getcstr(deffile_kwd);
	}
	(void)keyword;
#if TPP_HAVE_MACRO_NAME
	self->tm_name = keyword;
#endif /* TPP_HAVE_MACRO_NAME */
}
#endif /* TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_CPP_ASSERT
/* Relocate `self->tm_deffile` in case it references a keyword */
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_assertion_relocate_keyword(tpp_assertion *tpp_restrict self,
                               tpp_keywords const *tpp_restrict keywords) {
	tpp_keyword const *keyword = self->tas_value;
	if (keyword) {
		keyword = _tpp_keywords_getkeyword(keywords,
		                                   tpp_keyword_getstr(keyword),
		                                   tpp_keyword_getlen(keyword),
		                                   tpp_keyword_gethash(keyword));
		if (keyword)
			self->tas_value = keyword;
	}
}
#endif /* TPP_HAVE_CPP_ASSERT */

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_keywords_copy(tpp_keywords *tpp_restrict self,
                  tpp_keywords const *tpp_restrict from) {
	self->tks_kwdc = from->tks_kwdc;
	self->tks_bckm = from->tks_bckm;
	self->tks_bckv = from->tks_bckv;
	if (self->tks_bckv != (tpp_keyword **)_tpp_keywords_empty_map) {
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
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_CPP_ASSERT || TPP_HAVE_CPP_MACROS
		for (i = 0; i <= self->tks_bckm; ++i) {
			tpp_keyword *chain = bckv[i];
			for (; chain; chain = chain->tk_next) {
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_CPP_ASSERT || (TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO)
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
							tpp_macro_relocate(ent->tmpe_macro, self, chain);
						}
					}
#endif /* TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_CPP_ASSERT
					if (misc->tkm_assertions.tass_assc) {
						tpp_hash j;
						for (j = 0; j <= misc->tkm_assertions.tass_bckm; ++j) {
							tpp_assertion *ass = &misc->tkm_assertions.tass_bckv[j];
							tpp_assertion_relocate_keyword(ass, self);
						}
					}
#endif /* TPP_HAVE_CPP_ASSERT */
				}
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_CPP_ASSERT || (TPP_HAVE_CPP_MACROS && TPP_HAVE_PRAGMA_PUSH_MACRO) */
#if TPP_HAVE_CPP_MACROS
				if (_TPP_KEYWORD_MACRO_ISDEFINED(chain->tk_macro))
					tpp_macro_relocate(chain->tk_macro, self, chain);
#endif /* TPP_HAVE_CPP_MACROS */
			}
		}
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_CPP_ASSERT || TPP_HAVE_CPP_MACROS */
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_COPY */


/* Lookup keywords within the given keywords-table **ONLY**
 * @return: * :   The keyword in question
 * @return: NULL: No such keyword (consider using `tpp_keywords_getkeyword()` to
 *                also check for builtin keywords, or `tpp_keywords_newkeyword()`
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
                              _tpp_esc_lexer__PARAM) {
	tpp_keyword *result = self->tks_bckv[hash & self->tks_bckm];
	for (; result; result = result->tk_next) {
		if (result->tk_hash != hash)
			continue;
		if (tpp_memcmp_esc(result->tk_kwd, result->tk_len, kwd, len, lexer) == 0)
			break;
	}
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Same as above, but also search the built-in keyword table (`tpp_builtin_getkeyword()`) */
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
                             _tpp_esc_lexer__PARAM) {
	tpp_keyword const *result;
	result = _tpp_keywords_getkeyword_esc(self, kwd, len, hash, lexer);
	if (result == NULL)
		result = tpp_builtin_getkeyword_esc(kwd, len, hash, lexer);
	return result;
}
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Insert `kwd` into `self`. If necessary, resize the hash-map.
 * If resizing fails, `kwd` is destroyed and `NULL` is returned. */
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
		if (self->tks_bckv != (tpp_keyword **)_tpp_keywords_empty_map)
			tpp_free(self->tks_bckv);
		self->tks_bckv = new_table;
		self->tks_bckm = new_mask;
	}

do_insert:
	tpp_assert(self->tks_bckm != 0);
	tpp_assert(self->tks_bckv != (tpp_keyword **)_tpp_keywords_empty_map);
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

/* Same as above, but if the keyword doesn't exist in `self` or the builtin
 * keyword table, a new keyword is allocated, given an ID, and inserted into `self`
 * @return: * :   The keyword associated with `kwd` (possibly having been just allocated)
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
	result->tk_macro = _TPP_KEYWORD_MACRO_UNDEFINED;
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
                             _tpp_esc_lexer__PARAM) {
	tpp_keyword *result;
	tpp_size len_without_esc;
	result = (tpp_keyword *)tpp_keywords_getkeyword_esc(self, kwd, len, hash, lexer);
	if (result != NULL)
		goto done;

	/* Must allocate a new keyword... */
	result = _tpp_keyword_alloc(len);
	if (result == NULL)
		goto done;

	result->tk_id = (tpp_token_id)((unsigned int)TPP_TOK_USERKEYWORD_BEGIN + self->tks_kwdc);
#if TPP_HAVE_CPP_MACROS
	result->tk_macro = _TPP_KEYWORD_MACRO_UNDEFINED;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	result->tk_misc = NULL;
#endif /* TPP_HAVE_KEYWORD_MISC */
	result->tk_hash = hash;
	tpp_keyword_init_refcnt(result);
	len_without_esc = tpp_without_esc(result->tk_kwd, kwd, len, lexer);
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
/* Check if `kwd` is contained in `self`.
 * If so: do nothing and simply re-return `kwd`
 *
 * Otherwise, assume that `kwd` is a *builtin* keyword (as returned
 * by `tpp_builtin_getkeyword()`), in which the keyword is copied,
 * inserted into `self`, and said copy is returned.
 *
 * This function must be used to make a keyword *writable* (which is
 * required before its `tk_macro` / `tk_misc` fields can safely be
 * written to (and in the later case: all fields of a potentially
 * pointed-to `tpp_keyword_misc`, too)
 *
 * @return: * :   A writable copy of `kwd`
 * @return: NULL: Out of memory (`TPP_ENOMEM`) */
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
	if (_TPP_KEYWORD_MACRO_ISDEFINED(result->tk_macro))
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
#endif /* TPP_HAVE_USER_KEYWORDS */


#if TPP_HAVE_LEXER_OPENFILE || TPP_HAVE_JOINPATH
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) /*utf-8*/char *TPPCALL
tpp_fs_normalize(/*utf-8*/ char *dst_iter,  /* Output pointer destination buffer (with at least `srclen` char-s of space) */
                 /*utf-8*/ char *dst_base,  /* Base pointer of destination buffer (start of destination filename string) */
                 /*utf-8*/ char const *src, /* Filename string to append to `dst_iter` (`..`-refs don't go beyond `dst_base`) */
                 tpp_size srclen) {         /* Length of `src` (in char-s) */
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
#endif /* TPP_HAVE_LEXER_OPENFILE || TPP_HAVE_JOINPATH */


#if TPP_HAVE_LEXER_OPENFILE
#if TPP_HAVE_LEXER_OPENFILE_EX
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
#if TPP_HAVE_CPP_IMPORT
TPP_STATIC_ASSERT(TPP_LEXER_OPENFILE_FLAG_CHECK_LIMIT != TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED);
TPP_STATIC_ASSERT(TPP_LEXER_OPENFILE_FLAG_WARN_CASING != TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED);
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_PRAGMA_ONCE
TPP_STATIC_ASSERT(TPP_LEXER_OPENFILE_FLAG_CHECK_LIMIT != TPP_LEXER_OPENFILE_FLAG_HDR_ONCE);
TPP_STATIC_ASSERT(TPP_LEXER_OPENFILE_FLAG_WARN_CASING != TPP_LEXER_OPENFILE_FLAG_HDR_ONCE);
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
TPP_STATIC_ASSERT(TPP_LEXER_OPENFILE_FLAG_CHECK_LIMIT != TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED);
TPP_STATIC_ASSERT(TPP_LEXER_OPENFILE_FLAG_WARN_CASING != TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED);
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */


#if TPP_HAVE_USER_KEYWORDS && TPP_HAVE_LEXER_OPENFILE_EX
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_openfile_ex_check_mask_flags(/*1..1*/ tpp_lexer *tpp_restrict self,
                                       tpp_keyword *file_kwd,
                                       tpp_lexer_openfile_flags mask_flags) {
	tpp_keyword_misc *misc = file_kwd->tk_misc;
	if (misc != NULL) {
		tpp_keyword_flags flags_union = misc->tkm_flags & mask_flags;
		if (flags_union != 0) {
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
			if (flags_union == TPP_KEYWORD_FLAG_HDR_GUARD_VALID) {
				/* Special case when the only thing being masked is an #ifndef guard */
				tpp_keyword const *file_guard = misc->tkm_file_guard;
				tpp_assert(file_guard != NULL && "'TPP_KEYWORD_FLAG_HDR_GUARD_VALID' is "
				                                 "set, but 'tkm_file_guard == NULL'");
				if ((misc->tkm_flags & (mask_flags & ~TPP_KEYWORD_FLAG_HDR_GUARD_VALID)) != 0)
					return TPP_EMASKED; /* File is masked even if it wasn't for the header guard. */
				if (tpp_lexer_getkeyworddefined(self, file_guard))
					return TPP_EMASKED; /* File guard is still defined -> don't include */
			} else
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
			{
				return TPP_EMASKED;
			}
		} else
#if TPP_HAVE_CPP_IMPORT
		if (mask_flags & TPP_KEYWORD_FLAG_HDR_IMPORTED) {
			/* Set the "TPP_KEYWORD_FLAG_HDR_IMPORTED" flag in the keyword */
			misc->tkm_flags |= TPP_KEYWORD_FLAG_HDR_IMPORTED;
		} else
#endif /* TPP_HAVE_CPP_IMPORT */
		{
		}
	} else
#if TPP_HAVE_CPP_IMPORT
	if (mask_flags & TPP_KEYWORD_FLAG_HDR_IMPORTED) {
		/* Must allocate misc data for "result_kwd" so we can remember that this file was #import-ed */
		misc = tpp_keyword_requiremisc(file_kwd);
		if tpp_unlikely(!misc)
			return TPP_ENOMEM;
		misc->tkm_flags |= TPP_KEYWORD_FLAG_HDR_IMPORTED;
	} else
#endif /* TPP_HAVE_CPP_IMPORT */
	{
	}

#if TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next
	if (mask_flags & TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT) {
		/* Check if this file is already being #include-ed */
		tpp_file const *fp = tpp_lexer_getfile(self);
		do {
			if ((fp->tf_kind == TPP_FILE_KIND_IO ||
			     fp->tf_kind == TPP_FILE_KIND_TEXT) &&
#if TPP_HAVE_FILE_NOKWD
			    !(fp->tf_flags & TPP_FILE_FLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
			    fp->tf_data.td_io.tff_name != NULL) {
				tpp_keyword const *kwd = (tpp_keyword const *)((char const *)fp->tf_data.td_io.tff_name -
				                                               tpp_offsetof(tpp_keyword, tk_kwd));
				if (kwd == file_kwd)
					return TPP_ENOENT; /* File is already on #include-stack */
			}
		} while ((fp = fp->tf_tprev) != NULL);
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next */
#endif /* TPP_HAVE_USER_KEYWORDS && TPP_HAVE_LEXER_OPENFILE_EX */


/* Same as `tpp_lexer_openfile`, but return `TPP_EMASKED` if the file was already
 * included before, and its keyword has any of the bits specified by `mask_flags` set.
 *
 * NOTES:
 * - A special case is made when `mask_flags & TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED`,
 *   in which case, `TPP_EMASKED` is only returned if `tkm_file_guard` is a macro that
 *   is currently considered to be `#if defined()`.
 * - Another special case is made for `TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT`, which
 *   causes `TPP_EMASKED` to be returned if the file's keyword is already included
 *   somewhere on the `#include`-stack.
 * - Also: when `mask_flags & TPP_KEYWORD_FLAG_HDR_IMPORTED`, and the file's keyword
 *   doesn't already have the `TPP_KEYWORD_FLAG_HDR_IMPORTED` flag set, the open will
 *   succeed, and the `TPP_KEYWORD_FLAG_HDR_IMPORTED` flag will become set (so-as to
 *   implement the include-once semantics of `#import`)
 * - This function always sets `tlofr_fileflags = TPP_FILE_FLAGS_NORMAL`.
 *   If the given `relative_to` belongs to a system header, then it is up
 *   to the caller to set that flag. `tpp_lexer_open_include_string_ex()`
 *   will do so automatically after calling this function.
 *
 * @param: mask_flags: Set of flags describing circumstances under which `TPP_EMASKED`
 *                     should be returned:
 *                     - `TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED`
 *                     - `TPP_LEXER_OPENFILE_FLAG_HDR_ONCE`
 *                     - `TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED`
 *                     - `TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT`
 *
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOMEM:  Insufficient memory
 * @return: TPP_ENOENT:  No such file, or `TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT` was
 *                       given, and the file is already located on the `#include`-stack.
 * @return: TPP_EMASKED: Flags specified by `mask_flags` were already set */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3, 5)) tpp_errno TPPCALL
tpp_lexer_openfile_ex(/*1..1*/ tpp_lexer *tpp_restrict self,
                      /*0..1*/ char const *tpp_restrict relative_to,
                      /*1..1*/ /*utf-8*/ char const *filename, tpp_size filename_maxlen,
                      /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result,
                      tpp_lexer_openfile_flags mask_flags)
#else /* TPP_HAVE_LEXER_OPENFILE_EX */
/* Construct the filename, open the file, and initialize `result` accordingly
 * @param: relative_to: The `tpp_file::tf_data.td_io.tff_name` of another file,
 *                      in case `filename` is a relative path, in which case the
 *                      filename of the file to open should be relative to the
 *                      directory of `relative_to`
 * @param: result:      Open file information (pass along to `tpp_file_init_io()`)
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Insufficient memory
 * @return: TPP_ENOENT: File not found (if you have additional `relative_to`, try them) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3, 5)) tpp_errno TPPCALL
tpp_lexer_openfile(/*1..1*/ tpp_lexer *tpp_restrict self,
                   /*0..1*/ char const *tpp_restrict relative_to,
                   /*1..1*/ /*utf-8*/ char const *filename, tpp_size filename_maxlen,
                   /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result)
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
{
#if TPP_HAVE_USER_KEYWORDS
	bool is_known_keyword = false;
#define tpp_lexer_openfile_keyword                    tpp_keyword
#define tpp_lexer_openfile_keyword_cstr(p)            ((char *)(p)->tk_kwd)
#define tpp_lexer_openfile_keyword_setlen(p, v)       (void)((p)->tk_len = (v))
#define tpp_lexer_openfile_keyword_getlen(p)          (p)->tk_len
#define tpp_lexer_openfile_keyword_alloc(len)         _tpp_keyword_alloc(len)
#define tpp_lexer_openfile_keyword_tryrealloc(p, len) _tpp_keyword_tryrealloc(p, len)
#define tpp_lexer_openfile_keyword_realloc(p, len)    _tpp_keyword_realloc(p, len)
#define tpp_lexer_openfile_keyword_free(p)            _tpp_keyword_free(p)
#else /* TPP_HAVE_USER_KEYWORDS */
#define tpp_lexer_openfile_keyword                    char
#define tpp_lexer_openfile_keyword_cstr(p)            p
#define tpp_lexer_openfile_keyword_setlen(p, v)       (void)0
#define tpp_lexer_openfile_keyword_getlen(p)          tpp_strlen(p)
#define tpp_lexer_openfile_keyword_alloc(len)         ((char *)tpp_malloc(((len) + 1) * sizeof(char)))
#define tpp_lexer_openfile_keyword_tryrealloc(p, len) ((char *)tpp_tryrealloc(p, ((len) + 1) * sizeof(char)))
#define tpp_lexer_openfile_keyword_realloc(p, len)    ((char *)tpp_realloc(p, ((len) + 1) * sizeof(char)))
#define tpp_lexer_openfile_keyword_free(p)            tpp_free(p)
#endif /* !TPP_HAVE_USER_KEYWORDS */
	tpp_io_handle handle;
	tpp_lexer_openfile_keyword *result_kwd;
	tpp_size filename_len = tpp_strnlen(filename, filename_maxlen);
	if (TPP_FS_ISABS(filename, filename_len) || !relative_to) {
		tpp_lexer_openfile_keyword *new_result_kwd;
		char *kwd_end;
		tpp_size result_kwd_len;
without_relative_to:
		result_kwd = tpp_lexer_openfile_keyword_alloc(filename_len);
		if tpp_unlikely(!result_kwd)
			goto err_nomem;
		kwd_end = tpp_fs_normalize(tpp_lexer_openfile_keyword_cstr(result_kwd),
		                           tpp_lexer_openfile_keyword_cstr(result_kwd),
		                           filename, filename_len);
		*kwd_end = '\0';
		result_kwd_len = (tpp_size)(kwd_end - tpp_lexer_openfile_keyword_cstr(result_kwd));
		tpp_assert(result_kwd_len <= filename_len);
		new_result_kwd = tpp_lexer_openfile_keyword_tryrealloc(result_kwd, result_kwd_len);
		if tpp_likely(new_result_kwd)
			result_kwd = new_result_kwd;
		tpp_lexer_openfile_keyword_setlen(result_kwd, result_kwd_len);
	} else {
		tpp_lexer_openfile_keyword *new_result_kwd;
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
		result_kwd = tpp_lexer_openfile_keyword_alloc(whole_size);
		if tpp_unlikely(!result_kwd)
			goto err_nomem;
		dst_base = tpp_lexer_openfile_keyword_cstr(result_kwd);
		tpp_memcpy(dst_base, rel_base, rel_size * sizeof(char)); /* Including trailing '/' */
		dst_iter = dst_base + rel_size;
		dst_end = tpp_fs_normalize(dst_iter, dst_base, filename, filename_len);
		*dst_end = '\0';
		whole_size = (tpp_size)(dst_end - dst_base);
		new_result_kwd = tpp_lexer_openfile_keyword_tryrealloc(result_kwd, whole_size);
		if tpp_likely(new_result_kwd)
			result_kwd = new_result_kwd;
		tpp_lexer_openfile_keyword_setlen(result_kwd, whole_size);
	}

	/* Check if "result_kwd" is a known keyword... */
	(void)self;
#if TPP_HAVE_USER_KEYWORDS
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
			tpp_lexer_openfile_keyword_free(result_kwd);
			is_known_keyword = true;
			result_kwd = bucket;

			/* Check if the file should be marked out. */
#if TPP_HAVE_LEXER_OPENFILE_EX
			{
				tpp_errno mask_error = tpp_lexer_openfile_ex_check_mask_flags(self, result_kwd, mask_flags);
				if (TPP_ISERR(mask_error))
					return mask_error;
			}
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */

			goto got_result_kwd;
		}
		result_kwd->tk_hash = hash;
	}
got_result_kwd:
#endif /* TPP_HAVE_USER_KEYWORDS */

	/* Try to open the file */
	{
		tpp_errno error = tpp_io_open(tpp_lexer_openfile_keyword_cstr(result_kwd), &handle);
		if (TPP_ISERR(error)) {
			tpp_lexer_openfile_keyword_free(result_kwd);
			return error; /* Probably TPP_ENOENT */
		}
	}

#if TPP_HAVE_IO_NORMALIZE_FILENAME
#if TPP_HAVE_USER_KEYWORDS
	if (!is_known_keyword)
#endif /* TPP_HAVE_USER_KEYWORDS */
	{
		/* Windows has a case-insensitive filesystem, but the filename hash used here is
		 * (and has to be for the sake of allowing us to re-use the keyword table) case-
		 * sensitive.
		 *
		 * -> Solve this by essentially doing what TPP2 used to do, and validating that a
		 *    call to "FindFirstFileA()" returns with the same casing as our custom keyword
		 *    for every path segment not shared with "relative_to". Any segments where our
		 *    path contains a different casing than would be canonical must then be replaced
		 *    with its canonical equivalent.
		 * -> If at the end at least one path segment was modified:
		 *    - Emit a warning TPP_W_NONPORTABLE_FILENAME_CASING
		 * #if TPP_HAVE_USER_KEYWORDS
		 *    - Repeat the 'Check if "result_kwd" is a known keyword...' block above so
		 *      it can re-check if the file is known whilst considering the fixed casing
		 * #endif // TPP_HAVE_USER_KEYWORDS */
		bool did_fix_something = false;
#if TPP_HAVE_LEXER_OPENFILE_EX && TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING
		tpp_size relevant_base_offset;
#endif /* TPP_HAVE_LEXER_OPENFILE_EX && TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING */
		char *iter = tpp_lexer_openfile_keyword_cstr(result_kwd);
		if (relative_to) {
			char const *relative_to_iter = relative_to;
			char const *last_sep = relative_to_iter + tpp_strlen(relative_to);
			while (last_sep > relative_to_iter && last_sep[-1] != TPP_FS_SEP)
				--last_sep;
			while (*iter == *relative_to_iter && relative_to_iter < last_sep)
				++iter, ++relative_to_iter;
		}
#if TPP_FS_HAVE_DRIVES
		if (iter == tpp_lexer_openfile_keyword_cstr(result_kwd) &&
		    iter[0] != '\0' && iter[1] == ':')
			iter += 2;
#endif /* TPP_FS_HAVE_DRIVES */
#if TPP_HAVE_LEXER_OPENFILE_EX && TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING
		relevant_base_offset = (tpp_size)(iter - tpp_lexer_openfile_keyword_cstr(result_kwd));
#endif /* TPP_HAVE_LEXER_OPENFILE_EX && TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING */
		while (*iter) {
			char *next_sep;
			tpp_size partlen;
			tpp_ssize normalize_status;
			while (*iter == TPP_FS_SEP)
				++iter;
			next_sep = tpp_strchr(iter, TPP_FS_SEP);
			if (next_sep) {
				*next_sep = '\0';
				partlen   = (tpp_size)(next_sep - iter);
			} else {
				partlen = tpp_strlen(iter);
			}
			if (iter[0] == '.' && (iter[1] == '\0' || (iter[1] == '.' && iter[2] == '\0')))
				goto continue_with_next_part; /* Skip "." and ".." path references */
			normalize_status = tpp_io_normalize_filename(tpp_lexer_openfile_keyword_cstr(result_kwd),
			                                             iter, partlen);
			if (normalize_status != 0) {
				/* Something has to change here! */
				if (TPP_SSIZE_ISERR(normalize_status)) {
					if (normalize_status == TPP_SSIZE_OFERR(TPP_ENOENT))
						goto done_normalize;
err_free_close_normalize_status:
					tpp_io_close(handle);
					tpp_lexer_openfile_keyword_free(result_kwd);
					return TPP_SSIZE_ASERR(normalize_status);
				}
				if ((tpp_size)normalize_status > partlen) {
					/* Need a larger buffer! */
					tpp_lexer_openfile_keyword *new_result_kwd;
					tpp_size old_size = tpp_lexer_openfile_keyword_getlen(result_kwd);
					tpp_size delta    = (tpp_size)normalize_status - partlen;
					tpp_size new_size = old_size + delta;
					tpp_size iter_off = (tpp_size)(iter - tpp_lexer_openfile_keyword_cstr(result_kwd));
					tpp_size next_off;
					tpp_assert(delta != 0);
					if (next_sep) {
						*next_sep = TPP_FS_SEP;
					} else {
						next_sep = tpp_lexer_openfile_keyword_cstr(result_kwd) + old_size;
					}
resize_to_new_size:
					next_off = (tpp_size)(next_sep - tpp_lexer_openfile_keyword_cstr(result_kwd));
					new_result_kwd = tpp_lexer_openfile_keyword_realloc(result_kwd, new_size);
					if tpp_unlikely(!new_result_kwd) {
						normalize_status = TPP_SSIZE_OFERR(TPP_ENOMEM);
						goto err_free_close_normalize_status;
					}
					result_kwd = new_result_kwd;
					tpp_lexer_openfile_keyword_setlen(result_kwd, new_size);
					iter     = tpp_lexer_openfile_keyword_cstr(result_kwd) + iter_off;
					next_sep = tpp_lexer_openfile_keyword_cstr(result_kwd) + next_off;
					tpp_memmoveup(next_sep + delta, next_sep, (old_size - next_off) * sizeof(char));
					next_off += delta;
					*next_sep = '\0';
					next_sep += delta;
					partlen = (tpp_size)normalize_status;
					normalize_status = tpp_io_normalize_filename(tpp_lexer_openfile_keyword_cstr(result_kwd),
					                                             iter, partlen);
					if (TPP_SSIZE_ISERR(normalize_status)) {
						if (normalize_status == TPP_SSIZE_OFERR(TPP_ENOENT)) {
							normalize_status = 0; /* Shouldn't happen... */
						} else {
							goto err_free_close_normalize_status;
						}
					}
					if ((tpp_size)normalize_status == 0)
						normalize_status = (tpp_ssize)partlen; /* Shouldn't happen... */
					if ((tpp_size)normalize_status > partlen) {
						delta    = (tpp_size)normalize_status - partlen;
						new_size = old_size + delta;
						goto resize_to_new_size;
					}
					if (*next_sep == '\0')
						next_sep = NULL;
				}
				if ((tpp_size)normalize_status < partlen) {
					/* Shrink buffer */
					tpp_size old_size = tpp_lexer_openfile_keyword_getlen(result_kwd);
					if (next_sep) {
						tpp_size beyond_len = old_size - (tpp_size)(next_sep - tpp_lexer_openfile_keyword_cstr(result_kwd));
						next_sep = (char *)tpp_memmovedown(iter + (tpp_size)normalize_status, next_sep,
						                                   (beyond_len + 1) * sizeof(char));
					} else {
						iter[(tpp_size)normalize_status] = '\0';
					}
					tpp_lexer_openfile_keyword_setlen(result_kwd, old_size - partlen +
					                                              (tpp_size)normalize_status);
				}
				did_fix_something = true;
			}
continue_with_next_part:
			if (!next_sep)
				break;
			*next_sep = TPP_FS_SEP;
			iter = next_sep + 1;
		}
done_normalize:
		if (did_fix_something) {
			/* Emit a warning */
#if TPP_HAVE_LEXER_OPENFILE_EX && TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING
			if (mask_flags & TPP_LEXER_OPENFILE_FLAG_WARN_CASING) {
				tpp_errno error;
				char *correct_casing_base = tpp_lexer_openfile_keyword_cstr(result_kwd) + relevant_base_offset;
				while (*correct_casing_base == TPP_FS_SEP)
					++correct_casing_base;
				error = tpp_lexer_warnf(self, TPP_W_NONPORTABLE_FILENAME_CASING, correct_casing_base);
				if (TPP_ISERR(error)) {
					tpp_io_close(handle);
					tpp_lexer_openfile_keyword_free(result_kwd);
					return error;
				}
			}
#endif /* TPP_HAVE_LEXER_OPENFILE_EX && TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING */

			/* Do another check for the keyword (now that it's path's casing has been fixed) */
#if TPP_HAVE_USER_KEYWORDS
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
					tpp_lexer_openfile_keyword_free(result_kwd);
					is_known_keyword = true;
					result_kwd       = bucket;

					/* Check if the file should be marked out. */
#if TPP_HAVE_LEXER_OPENFILE_EX
					{
						tpp_errno mask_error = tpp_lexer_openfile_ex_check_mask_flags(self, result_kwd, mask_flags);
						if (TPP_ISERR(mask_error)) {
							tpp_io_close(handle);
							return mask_error;
						}
					}
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
					goto got_result_kwd2;
				}
				result_kwd->tk_hash = hash;
			}
got_result_kwd2:;
#endif /* TPP_HAVE_USER_KEYWORDS */
		}
	}
#endif /* TPP_HAVE_IO_NORMALIZE_FILENAME */

	/* Initialize remaining fields of "result_kwd" and insert into keyword map */
#if TPP_HAVE_USER_KEYWORDS
	if (!is_known_keyword) {
		result_kwd->tk_id = (tpp_token_id)((unsigned int)TPP_TOK_USERKEYWORD_BEGIN +
		                                   self->tl_kwds.tks_kwdc);
#if TPP_HAVE_CPP_MACROS
		result_kwd->tk_macro = _TPP_KEYWORD_MACRO_UNDEFINED;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
		result_kwd->tk_misc = NULL;
#endif /* TPP_HAVE_KEYWORD_MISC */
		tpp_keyword_init_refcnt(result_kwd);
#if TPP_HAVE_CPP_IMPORT
		if (mask_flags & TPP_KEYWORD_FLAG_HDR_IMPORTED) {
			/* Must allocate misc data for "result_kwd" so we can remember that this file was #import-ed */
			tpp_keyword_misc *misc = tpp_keyword_requiremisc(result_kwd);
			if tpp_unlikely(!misc) {
				tpp_keyword_destroy(result_kwd);
				tpp_io_close(handle);
				goto err_nomem;
			}
			misc->tkm_flags |= TPP_KEYWORD_FLAG_HDR_IMPORTED;
		}
#endif /* TPP_HAVE_CPP_IMPORT */
		result_kwd = tpp_keywords_inskeyword(&self->tl_kwds, result_kwd);
		if tpp_unlikely(!result_kwd) {
			tpp_io_close(handle);
			goto err_nomem;
		}

		/* Call a user-defined callback to keep track of dependencies (for -MF) */
#if TPP_HAVE_NEW_DEPENDENCY_HOOK
		{
			tpp_errno error = tpp_lexer_callhook_new_dependency(self, result_kwd);
			if (TPP_ISERR(error)) {
				tpp_io_close(handle);
				return error;
			}
		}
#endif /* TPP_HAVE_NEW_DEPENDENCY_HOOK */
	} else
#endif /* TPP_HAVE_USER_KEYWORDS */
	{
#if TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED
		if (!(mask_flags & TPP_LEXER_OPENFILE_FLAG_CHECK_LIMIT)) {
			tpp_size include_count;
#if TPP_HAVE_KEYWORD_INCLCOUNT
			include_count = TPP_SIZE_MAX;
			if (result_kwd->tk_misc)
				include_count = result_kwd->tk_misc->tkm_file_inclcount;
			if (include_count == TPP_SIZE_MAX)
#endif /* TPP_HAVE_KEYWORD_INCLCOUNT */
			{
				/* Manually count the # of inclusions */
				tpp_file const *fp = tpp_lexer_getfile(self);
				include_count = 0;
				do {
					if ((fp->tf_kind == TPP_FILE_KIND_IO ||
					     fp->tf_kind == TPP_FILE_KIND_TEXT) &&
#if TPP_HAVE_FILE_NOKWD
					    !(fp->tf_flags & TPP_FILE_FLAGS_NOKWD) &&
#endif /* TPP_HAVE_FILE_NOKWD */
					    fp->tf_data.td_io.tff_name != NULL) {
#if TPP_HAVE_USER_KEYWORDS
						tpp_keyword const *kwd = (tpp_keyword const *)((char const *)fp->tf_data.td_io.tff_name -
						                                               tpp_offsetof(tpp_keyword, tk_kwd));
						if (kwd == result_kwd)
#else /* TPP_HAVE_USER_KEYWORDS */
						if (tpp_strcmp(fp->tf_data.td_io.tff_name, result_kwd) == 0)
#endif /* TPP_HAVE_USER_KEYWORDS */
						{
							++include_count;
						}
					}
				} while ((fp = fp->tf_tprev) != NULL);
#if TPP_HAVE_KEYWORD_INCLCOUNT
				if (result_kwd->tk_misc == NULL) {
					/* Try to cache the result */
					tpp_keyword_misc *misc = _tpp_keyword_misc_tryalloc();
					if tpp_likely(misc) {
						_tpp_keyword_misc_init(misc);
						misc->tkm_file_inclcount = include_count;
						result_kwd->tk_misc = misc;
					}
				}
#endif /* TPP_HAVE_KEYWORD_INCLCOUNT */
			}

			if (include_count >= tpp_lexer_getinclusionlimit(self)) {
				tpp_errno error = tpp_lexer_warnf(self, TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED,
				                                  tpp_lexer_openfile_keyword_cstr(result_kwd));
				if (TPP_ISERR(error)) {
					tpp_io_close(handle);
#if !TPP_HAVE_USER_KEYWORDS
					tpp_lexer_openfile_keyword_free(result_kwd);
#endif /* !TPP_HAVE_USER_KEYWORDS */
					return error;
				}
			}
		}
#endif /* TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED */
	}

	/* Initialize "result" */
#if TPP_HAVE_USER_KEYWORDS
	result->tlofr_filename_kwd = result_kwd;
#else /* TPP_HAVE_USER_KEYWORDS */
	result->tlofr_filename = result_kwd;
#endif /* !TPP_HAVE_USER_KEYWORDS */
	result->tlofr_handle = handle;
#if TPP_HAVE_FILE_SYSHDR
	result->tlofr_fileflags = TPP_FILE_FLAGS_NORMAL; /* Overwritten by caller (if necessary) */
#endif /* !TPP_HAVE_FILE_SYSHDR */
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
#undef tpp_lexer_openfile_keyword
#undef tpp_lexer_openfile_keyword_cstr
#undef tpp_lexer_openfile_keyword_setlen
#undef tpp_lexer_openfile_keyword_getlen
#undef tpp_lexer_openfile_keyword_alloc
#undef tpp_lexer_openfile_keyword_tryrealloc
#undef tpp_lexer_openfile_keyword_realloc
#undef tpp_lexer_openfile_keyword_free
}
#endif /* TPP_HAVE_LEXER_OPENFILE */


#if TPP_HAVE_JOINPATH
/* Form an absolute filename by combining `relative_to` with `filename`
 * @return: * :   The absolute path (must be free'd by caller using `tpp_free()`)
 * @return: NULL: Out of memory. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((2)) char *TPPCALL
tpp_joinpath(/*0..1*/ char const *tpp_restrict relative_to,
             /*1..1*/ /*utf-8*/ char const *filename,
             tpp_size filename_maxlen) {
	char *result;
	tpp_size filename_len = tpp_strnlen(filename, filename_maxlen);
	if (TPP_FS_ISABS(filename, filename_len) || !relative_to) {
		char *new_result;
		char *kwd_end;
		tpp_size result_kwd_len;
without_relative_to:
		result = (char *)tpp_malloc((filename_len + 1) * sizeof(char));
		if tpp_unlikely(!result)
			return NULL;
		kwd_end = tpp_fs_normalize(result, result, filename, filename_len);
		*kwd_end = '\0';
		result_kwd_len = (tpp_size)(kwd_end - result);
		tpp_assert(result_kwd_len <= filename_len);
		new_result = (char *)tpp_tryrealloc(result, (result_kwd_len + 1) * sizeof(char));
		if tpp_likely(new_result)
			result = new_result;
	} else {
		char *new_result;
		tpp_size rel_size, whole_size;
		char const *rel_base = relative_to;
		char const *last_sep = rel_base + tpp_strlen(relative_to);
		char *dst_iter, *dst_end;
		while (last_sep > rel_base && last_sep[-1] != TPP_FS_SEP)
			--last_sep;
		if (last_sep <= rel_base)
			goto without_relative_to;
		rel_size   = (tpp_size)(last_sep - rel_base); /* Including trailing '/' */
		whole_size = rel_size + filename_len;
		result = (char *)tpp_malloc((whole_size + 1) * sizeof(char));
		if tpp_unlikely(!result)
			return NULL;
		tpp_memcpy(result, rel_base, rel_size * sizeof(char)); /* Including trailing '/' */
		dst_iter = result + rel_size;
		dst_end = tpp_fs_normalize(dst_iter, result, filename, filename_len);
		*dst_end = '\0';
		whole_size = (tpp_size)(dst_end - result);
		new_result = (char *)tpp_tryrealloc(result, (whole_size + 1) * sizeof(char));
		if tpp_likely(new_result)
			result = new_result;
	}
	return result;
}
#endif /* TPP_HAVE_JOINPATH */


#if TPP_HAVE_LEXER_CLI_ASSERT
/* Add a new keyword assertions for `key` and `value`.
 * This is the same as doing `#assert {key}({value})`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 4)) tpp_errno TPPCALL
tpp_lexer_assert(tpp_lexer *tpp_restrict self,
                 char const *key, tpp_size key_maxlen,
                 char const *value, tpp_size value_maxlen) {
	tpp_size key_len = tpp_strnlen(key, key_maxlen);
	tpp_size value_len = tpp_strnlen(value, value_maxlen);
	tpp_hash key_hash = tpp_hashof((tpp_char const *)key, key_len);
	tpp_hash value_hash = tpp_hashof((tpp_char const *)value, value_len);
	tpp_keyword const *ro_key_keyword;
	tpp_keyword const *ro_value_keyword;
	tpp_keyword *key_keyword;
	ro_key_keyword = tpp_lexer_kwds_newkeyword(self, (tpp_char const *)key, key_len, key_hash);
	if tpp_unlikely(!ro_key_keyword)
		goto err_nomem;
	ro_value_keyword = tpp_lexer_kwds_newkeyword(self, (tpp_char const *)value, value_len, value_hash);
	if tpp_unlikely(!ro_value_keyword)
		goto err_nomem;
	key_keyword = tpp_lexer_kwds_copybuiltin(self, ro_key_keyword);
	if tpp_unlikely(!key_keyword)
		goto err_nomem;
	return tpp_keyword_addassert(key_keyword, ro_value_keyword);
err_nomem:
	return TPP_ENOMEM;
}

/* Delete a new keyword assertions for `key` and `value`.
 * This is the same as doing `#unassert {key}({value})`
 * @return: true:  Success
 * @return: false: No such assertion */
TPP_IMPL TPP_NONNULL((1, 2, 4)) bool TPPCALL
tpp_lexer_unassert(tpp_lexer *tpp_restrict self,
                   char const *key, tpp_size key_maxlen,
                   char const *value, tpp_size value_maxlen) {
	tpp_size key_len = tpp_strnlen(key, key_maxlen);
	tpp_size value_len = tpp_strnlen(value, value_maxlen);
	tpp_hash key_hash = tpp_hashof((tpp_char const *)key, key_len);
	tpp_hash value_hash = tpp_hashof((tpp_char const *)value, value_len);
	tpp_keyword *key_keyword;
	tpp_keyword const *ro_value_keyword;
	key_keyword = _tpp_lexer_kwds_getkeyword(self, (tpp_char const *)key, key_len, key_hash);
	if (!key_keyword)
		return false;
	ro_value_keyword = tpp_lexer_kwds_getkeyword(self, (tpp_char const *)value, value_len, value_hash);
	if (!ro_value_keyword)
		return false;
	return tpp_keyword_unassert(key_keyword, ro_value_keyword);
}

/* Delete all keyword assertions for `key`.
 * This is the same as doing `#unassert {key}` */
TPP_IMPL TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_unassertall(tpp_lexer *tpp_restrict self,
                      char const *key, tpp_size key_maxlen) {
	tpp_size key_len = tpp_strnlen(key, key_maxlen);
	tpp_hash key_hash = tpp_hashof((tpp_char const *)key, key_len);
	tpp_keyword *key_keyword = _tpp_lexer_kwds_getkeyword(self, (tpp_char const *)key, key_len, key_hash);
	if (key_keyword)
		tpp_keyword_unassertall(key_keyword);
}
#endif /* TPP_HAVE_LEXER_CLI_ASSERT */


#if TPP_HAVE_KEYWORDS_UNDEFALL && TPP_HAVE_CPP_MACROS
/* Delete all user-defined macro definitions */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_undefalluser(tpp_keywords *tpp_restrict self) {
	tpp_hash i;
	for (i = 0; i <= self->tks_bckm; ++i) {
		tpp_keyword *bucket = self->tks_bckv[i];
		for (; bucket; bucket = bucket->tk_next)
			tpp_keyword_undefuser(bucket);
	}
}
#endif /* TPP_HAVE_KEYWORDS_UNDEFALL && TPP_HAVE_CPP_MACROS */


#if TPP_HAVE_KEYWORDS_UNASSERTALL && TPP_HAVE_CPP_ASSERT
/* Delete all user-defined keyword assertions */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_unassertall(tpp_keywords *tpp_restrict self) {
	tpp_hash i;
	for (i = 0; i <= self->tks_bckm; ++i) {
		tpp_keyword *bucket = self->tks_bckv[i];
		for (; bucket; bucket = bucket->tk_next)
			tpp_keyword_unassertall(bucket);
	}
}
#endif /* TPP_HAVE_KEYWORDS_UNASSERTALL && TPP_HAVE_CPP_ASSERT */


#if TPP_HAVE_KEYWORDS_RESETFLAGS && TPP_HAVE_KEYWORD_FLAGS
/* Modify the flags of all keywords as `flags = flags & keep_mask` */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_resetflags(tpp_keywords *tpp_restrict self,
                        tpp_keyword_flags keep_mask) {
	tpp_hash i;
	for (i = 0; i <= self->tks_bckm; ++i) {
		tpp_keyword *bucket = self->tks_bckv[i];
		for (; bucket; bucket = bucket->tk_next) {
			tpp_keyword_misc *misc = bucket->tk_misc;
			if (misc)
				misc->tkm_flags &= keep_mask;
		}
	}
}
#endif /* TPP_HAVE_KEYWORDS_RESETFLAGS && TPP_HAVE_KEYWORD_FLAGS */


#if TPP_HAVE_KEYWORDS_RESETFEATURES && TPP_HAVE_KEYWORD_FEATURES
/* Reset all uses of `tpp_keyword_setfeature()` */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_resetfeatures(tpp_keywords *tpp_restrict self) {
	tpp_hash i;
	for (i = 0; i <= self->tks_bckm; ++i) {
		tpp_keyword *bucket = self->tks_bckv[i];
		for (; bucket; bucket = bucket->tk_next)
			tpp_keyword_resetfeatures(bucket);
	}
}
#endif /* TPP_HAVE_KEYWORDS_RESETFEATURES && TPP_HAVE_KEYWORD_FEATURES */

#if TPP_HAVE_KEYWORDS_RESETCOUNTERS && TPP_HAVE_MACRO___TPP_COUNTER
/* Call `tpp_keyword_reset_builtin_counter()` on every keyword, thereby
 * resetting all side-effects of expansions of `__TPP_COUNTER` thus far. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_resetcounters(tpp_keywords *tpp_restrict self) {
	tpp_hash i;
	for (i = 0; i <= self->tks_bckm; ++i) {
		tpp_keyword *bucket = self->tks_bckv[i];
		for (; bucket; bucket = bucket->tk_next)
			tpp_keyword_reset_builtin_counter(bucket);
	}
}
#endif /* TPP_HAVE_KEYWORDS_RESETCOUNTERS && TPP_HAVE_MACRO___TPP_COUNTER */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_KEYWORD_C */
