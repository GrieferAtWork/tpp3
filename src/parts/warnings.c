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
#ifndef GUARD_TPP_WARNINGS_C
#define GUARD_TPP_WARNINGS_C 1
#define TPP_BUILDING 1

#include "api.h"
#include "config.h"
#include "error.h"
#include "tuple.h"
#include "warnings.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_WARNINGS

#if TPP_HAVE_WARNING_SUPPRESS
#define _tpp_warnings_fini_common(self) \
	tpp_warning_suppressions_fini(&(self)->tw_suppressions)
#else /* TPP_HAVE_WARNING_SUPPRESS */
#define _tpp_warnings_fini_common(self) (void)0
#endif /* !TPP_HAVE_WARNING_SUPPRESS */

/* Finalize a given warnings context "self" */
#if TPP_HAVE_WARNINGS_FINI

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_fini(tpp_warnings *tpp_restrict self) {
	/* Finalize common data */
	_tpp_warnings_fini_common(self);

	/* Finalized pushed warning states */
#if TPP_HAVE_WARNINGS_PUSH_POP
	{
		tpp_warnings *prev = self->tw_prev;
		while (prev) {
			tpp_warnings *pprev = prev->tw_prev;
			_tpp_warnings_fini_common(prev);
			tpp_free(prev);
			prev = pprev;
		}
	}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */
}
#endif /* TPP_HAVE_WARNINGS_FINI */


#if TPP_HAVE_WARNINGS_PUSH_POP
/* Pop the current warnings state (may only be called when `tpp_warnings_canpop(self)') */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_pop(tpp_warnings *tpp_restrict self) {
	tpp_assert(tpp_warnings_canpop(self));
	if (self->tw_pushcnt == 0) {
		tpp_warnings *prev = self->tw_prev;
		_tpp_warnings_fini_common(self);
		tpp_memcpy(self, prev, sizeof(tpp_warnings));
		tpp_free(prev);
		tpp_assert(self->tw_pushcnt != 0);
	}
	--self->tw_pushcnt;
}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */

/* Return the state of "ctx_id". The caller is
 * responsible to ensure that "ctx_id" is valid. */
#if TPP_HAVE_WARNING_SUPPRESS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_state TPPCALL
tpp_warnings_getctx(tpp_warnings const *tpp_restrict self,
                    tpp_warning_context_id ctx_id) {
	tpp_warning_state result;
	tpp_assert((unsigned int)ctx_id < (unsigned int)TPP_WC_COUNT);
	result = tpp_warnings_state_get(&self->tw_state, ctx_id);
	if (result == TPP_WSTATE_FATAL) {
		/* Check if this warning is currently being suppressed. */
		tpp_size lo = 0, hi = self->tw_suppressions.tws_ctxc;
		while (lo < hi) {
			tpp_warning_suppress_item const *it;
			tpp_size const mid = (lo + hi) / 2;
			it = &self->tw_suppressions.tws_ctxv[mid];
			if ((unsigned int)ctx_id < (unsigned int)it->twsi_ctx_id) {
				hi = mid;
			} else if ((unsigned int)ctx_id > (unsigned int)it->twsi_ctx_id) {
				lo = mid + 1;
			} else {
				tpp_assert(it->twsi_count != 0);
				result = TPP_WSTATE_SUPPRESS;
				break;
			}
		}
	}
	return result;
}
#endif /* TPP_HAVE_WARNING_SUPPRESS */

#if TPP_HAVE_WARNINGS_PUSH_POP
/* Create a heap-copy of "self".
 * @return: * :   The newly allocated copy
 * @return: NULL: Out of memory. */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_warnings *TPPCALL
tpp_warnings_copy(tpp_warnings const *tpp_restrict self) {
	tpp_warnings *result = (tpp_warnings *)tpp_malloc(sizeof(tpp_warnings));
	if tpp_unlikely(!result)
		goto err;

	/* Duplicate suppressions */
#if TPP_HAVE_WARNING_SUPPRESS
	if (self->tw_suppressions.tws_ctxc) {
		tpp_warning_suppress_item *suppress_copy;
		suppress_copy = (tpp_warning_suppress_item *)tpp_malloc(self->tw_suppressions.tws_ctxc *
		                                                        sizeof(tpp_warning_suppress_item));
		if tpp_unlikely(!suppress_copy)
			goto err_r;
		tpp_memcpy(suppress_copy, self->tw_suppressions.tws_ctxv,
		           self->tw_suppressions.tws_ctxc *
		           sizeof(tpp_warning_suppress_item));
		result->tw_suppressions.tws_ctxv = suppress_copy;
		result->tw_suppressions.tws_ctxc = self->tw_suppressions.tws_ctxc;
		result->tw_suppressions.tws_ctxa = self->tw_suppressions.tws_ctxc;
	} else {
		tpp_warning_suppressions_init(&result->tw_suppressions);
	}
#endif /* TPP_HAVE_WARNING_SUPPRESS */

	/* Duplicate remaining data fields... */
	result->tw_state = self->tw_state;
#if TPP_HAVE_WARNINGS_PUSH_POP
	result->tw_prev    = self->tw_prev;
	result->tw_pushcnt = self->tw_pushcnt;
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */
	return result;
#if TPP_HAVE_WARNING_SUPPRESS
err_r:
	tpp_free(result);
#endif /* TPP_HAVE_WARNING_SUPPRESS */
err:
	return NULL;
}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */


#if TPP_HAVE_WARNING_SUPPRESS
/* Ensure that an entry for "ctx_id" is allocated in `&self->tw_suppressions'
 * @return: * :   The suppression entry for "ctx_id"
 * @return: NULL: Out of memory. */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_warning_suppress_item *TPPCALL
tpp_warnings_require_suppress(tpp_warnings *tpp_restrict self,
                              tpp_warning_context_id ctx_id) {
	tpp_size lo = 0, hi = self->tw_suppressions.tws_ctxc;
	tpp_warning_suppress_item *result;
	while (lo < hi) {
		tpp_size mid = (lo + hi) / 2;
		result = &self->tw_suppressions.tws_ctxv[mid];
		if (ctx_id < result->twsi_ctx_id) {
			hi = mid;
		} else if (ctx_id > result->twsi_ctx_id) {
			lo = mid + 1;
		} else {
			/* Found existing entry! */
			return result;
		}
	}
	tpp_assert(lo == hi);
	tpp_assert(self->tw_suppressions.tws_ctxc <= self->tw_suppressions.tws_ctxa);
	if (self->tw_suppressions.tws_ctxc >= self->tw_suppressions.tws_ctxa) {
		/* Must allocate more space. */
		tpp_warning_suppress_item *new_vec;
		tpp_size new_alloc = (self->tw_suppressions.tws_ctxa * 3) / 2;
		if (new_alloc < 8)
			new_alloc = 8;
		if (new_alloc < self->tw_suppressions.tws_ctxc + 1)
			new_alloc = self->tw_suppressions.tws_ctxc + 1;
		new_vec = (tpp_warning_suppress_item *)tpp_tryrealloc(self->tw_suppressions.tws_ctxv,
		                                                      new_alloc *
		                                                      sizeof(tpp_warning_suppress_item));
		if tpp_unlikely(!new_vec) {
			new_alloc = self->tw_suppressions.tws_ctxc + 1;
			new_vec = (tpp_warning_suppress_item *)tpp_realloc(self->tw_suppressions.tws_ctxv,
			                                                   new_alloc *
			                                                   sizeof(tpp_warning_suppress_item));
			if tpp_unlikely(!new_vec)
				goto err_nomem;
		}
		self->tw_suppressions.tws_ctxv = new_vec;
		self->tw_suppressions.tws_ctxa = new_alloc;
	}
	tpp_assert(self->tw_suppressions.tws_ctxc < self->tw_suppressions.tws_ctxa);
	tpp_memmoveup(&self->tw_suppressions.tws_ctxv[lo + 1],
	              &self->tw_suppressions.tws_ctxv[lo],
	              (self->tw_suppressions.tws_ctxa - lo) *
	              sizeof(tpp_warning_suppress_item));
	result = &self->tw_suppressions.tws_ctxv[lo];
	result->twsi_count  = 0;
	result->twsi_ctx_id = ctx_id;
	return result;
err_nomem:
	return NULL;
}
#endif /* TPP_HAVE_WARNING_SUPPRESS */


/* Set the state of "ctx_id" to "state".
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_HAVE_WARNINGS_SETCTX_MAYFAIL
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_warnings_setctx(tpp_warnings *tpp_restrict self,
                    tpp_warning_context_id ctx_id,
                    tpp_warning_state state)
#else /* TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_warnings_setctx_(tpp_warnings *tpp_restrict self,
                     tpp_warning_context_id ctx_id,
                     tpp_warning_state state)
#endif /* !TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */
{
	tpp_assert((unsigned int)ctx_id < (unsigned int)TPP_WC_COUNT);

	/* Deal with `TPP_WSTATE_DEFAULT' */
#if TPP_HAVE_WARNING_DEFAULT
	if (state == TPP_WSTATE_DEFAULT)
		state = tpp_warnings_state_get(&tpp_warnings_state_default, ctx_id);
#endif /* TPP_HAVE_WARNING_DEFAULT */

	/* Duplicate the warnings state if it was pushed. */
#if TPP_HAVE_WARNINGS_PUSH_POP
	if (tpp_warnings_mustcopy(self)) {
		tpp_warnings *copy;
#ifndef __OPTIMIZE_SIZE__
		tpp_warning_state old_state;
		old_state = tpp_warnings_state_get(&self->tw_state, ctx_id);
		if (state == old_state)
			return TPP_EOK;
#endif /* !__OPTIMIZE_SIZE__ */
		copy = tpp_warnings_copy(self);
		if tpp_unlikely(!copy)
			return TPP_ENOMEM;
		self->tw_prev = copy;
		self->tw_pushcnt = 0;
	}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */

	/* Deal with `TPP_WSTATE_SUPPRESS' */
#if TPP_HAVE_WARNING_SUPPRESS
	if (state == TPP_WSTATE_SUPPRESS) {
		tpp_warning_suppress_item *item;
		item = tpp_warnings_require_suppress(self, ctx_id);
		if tpp_unlikely(!item)
			return TPP_ENOMEM;
		tpp_assert(item->twsi_ctx_id == ctx_id);
		if (item->twsi_count == 0) {
			item->twsi_restore = tpp_warnings_state_get(&self->tw_state, ctx_id);
			tpp_warnings_state_set(&self->tw_state, ctx_id, TPP_WSTATE_FATAL);
		} else {
			tpp_assert(tpp_warnings_state_get(&self->tw_state, ctx_id) == TPP_WSTATE_FATAL);
		}
		++item->twsi_count;
	} else
#endif /* TPP_HAVE_WARNING_SUPPRESS */
	{
		/* Regular case: set warning state in state-bitset. */
		tpp_assert(state == TPP_WSTATE_DISABLED ||
		           state == TPP_WSTATE_WARN ||
		           state == TPP_WSTATE_ERROR_OR_FATAL ||
		           state == TPP_WSTATE_FATAL);
		tpp_warnings_state_set(&self->tw_state, ctx_id, state);
	}

#if TPP_HAVE_WARNINGS_SETCTX_MAYFAIL
	return TPP_EOK;
#endif /* TPP_HAVE_WARNINGS_SETCTX_MAYFAIL */
}



/* Invoke "warning_id" (updating suppression counters if necessary) and
 * returning information about the context/state with which the warning
 * should be processed.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (only when "TPP_HAVE_WARNINGS_INVOKE_MAYFAIL") */
#if TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_warnings_invoke(tpp_warnings *tpp_restrict self, tpp_warning_id warning_id,
                    tpp_warning_invokeinfo *tpp_restrict result)
#else /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */
TPP_IMPL TPP_NONNULL((1, 3)) void TPPCALL
tpp_warnings_invoke_(tpp_warnings const *tpp_restrict self, tpp_warning_id warning_id,
                     tpp_warning_invokeinfo *tpp_restrict result)
#endif /* !TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */
{
	tpp_size i;
	tpp_warning_group_id const *groups;
#if TPP_HAVE_WARNING_NUMBERS
	tpp_warning_context_id number_context_id;
#endif /* TPP_HAVE_WARNING_NUMBERS */
	tpp_assert((unsigned int)warning_id < (unsigned int)TPP_W_COUNT);
	groups = tpp_warning_getgroups(warning_id);

	result->twii_ctx_id = TPP_WC_COUNT;
	result->twii_state  = TPP_WSTATE_DISABLED;
	for (i = 0; (unsigned int)groups[i] < (unsigned int)TPP_WG_COUNT; ++i) {
		tpp_warning_context_id ctx_id = tpp_warning_context_id_ofgroup(groups[i]);
		tpp_warning_state state = tpp_warnings_getctx(self, ctx_id);
		if (result->twii_ctx_id == TPP_WC_COUNT ||
		    tpp_warning_state_ismoreimportant(state, result->twii_state)) {
			/* Use this context instead! */
			result->twii_ctx_id = ctx_id;
			result->twii_state  = state;
		}
	}

#if TPP_HAVE_WARNING_NUMBERS
	/* Probe warning number context */
	number_context_id = tpp_warning_context_id_ofwarning(warning_id);
	if ((unsigned int)number_context_id < (unsigned int)TPP_WC_COUNT) {
		tpp_warning_state state = tpp_warnings_getctx(self, number_context_id);
		if (result->twii_ctx_id == TPP_WC_COUNT ||
		    tpp_warning_state_ismoreimportant(state, result->twii_state)) {
			/* Use this context instead! */
			result->twii_ctx_id = number_context_id;
			result->twii_state  = state;
		}
	}
#endif /* TPP_HAVE_WARNING_NUMBERS */

#if TPP_HAVE_WARNING_SUPPRESS
	if (result->twii_state == TPP_WSTATE_SUPPRESS) {
		tpp_size lo, hi, mid;
		tpp_warning_suppress_item *item;
		tpp_warning_context_id ctx_id = result->twii_ctx_id;

		/* Deal with special case: suppression. In this case:
		 * - must (potentially) copy "self"
		 * - must decrement the suppression-counter of the relevant context
		 * - if the suppression-counter hits "0", restore the warnings
		 *   previous state, then remove the suppression entry. */
#if TPP_HAVE_WARNINGS_PUSH_POP
		if (tpp_warnings_mustcopy(self)) {
			tpp_warnings *copy;
			copy = tpp_warnings_copy(self);
			if tpp_unlikely(!copy)
				return TPP_ENOMEM;
			self->tw_prev = copy;
			self->tw_pushcnt = 0;
		}
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */
		lo = 0;
		hi = self->tw_suppressions.tws_ctxc;
		for (;;) {
			mid = (lo + hi) / 2;
			tpp_assert(lo < hi);
			item = &self->tw_suppressions.tws_ctxv[mid];
			if (ctx_id < item->twsi_ctx_id) {
				hi = mid;
			} else if (ctx_id > item->twsi_ctx_id) {
				lo = mid + 1;
			} else {
				break; /* Found entry! */
			}
		}
		tpp_assert(item->twsi_count != 0);
		tpp_assert(item->twsi_ctx_id == ctx_id);
		--item->twsi_count;
		if (item->twsi_count == 0) {
			/* Restore old warning state */
			tpp_warning_state restore = item->twsi_restore;
			tpp_assert(restore == TPP_WSTATE_DISABLED ||
			           restore == TPP_WSTATE_WARN ||
			           restore == TPP_WSTATE_ERROR_OR_FATAL ||
			           restore == TPP_WSTATE_FATAL);
			tpp_warnings_state_set(&self->tw_state, ctx_id, restore);

			/* Delete the suppression entry */
			--self->tw_suppressions.tws_ctxc;
			tpp_memmovedown(item, item + 1,
			                (self->tw_suppressions.tws_ctxc - mid) *
			                sizeof(tpp_warning_suppress_item));
		}
	}
#endif /* TPP_HAVE_WARNING_SUPPRESS */

#if TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
	return TPP_EOK;
#endif /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */
}

#endif /* TPP_HAVE_WARNINGS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_WARNINGS_C */
