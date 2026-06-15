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
#ifndef GUARD_TPP_EXTENSIONS_C
#define GUARD_TPP_EXTENSIONS_C 1
#define TPP_BUILDING 1

#include "api.h"
#include "config.h"
#include "extensions.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_EXTENSIONS

/* Default extension state */
TPP_CONST_IMPL tpp_extensions_state const tpp_extensions_state_default = {
	/* .tes_flags = */ {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) /* .tef_##id = */ default,
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS
	}
};


#if TPP_HAVE_EXTENSIONS_PUSH_POP
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_extensions_fini(tpp_extensions *tpp_restrict self) {
	tpp_extensions *iter = self->te_prev;
	while (iter != NULL) {
		tpp_extensions *prev = iter->te_prev;
		tpp_free(iter);
		iter = prev;
	}
}

/* Pop the current extensions state (may only be called when `tpp_extensions_canpop(self)') */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_extensions_pop(tpp_extensions *tpp_restrict self) {
	tpp_assert(tpp_extensions_canpop(self));
	if (self->te_pushcnt == 0) {
		tpp_extensions *prev = self->te_prev;
		tpp_memcpy(self, prev, sizeof(tpp_extensions));
		tpp_free(prev);
		tpp_assert(self->te_pushcnt != 0);
	}
	--self->te_pushcnt;
}

/* @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: OOM */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_extensions_setid(tpp_extensions *tpp_restrict self,
                     tpp_extension_id id, bool enabled) {
	if (tpp_extensions_mustcopy(self)) {
		tpp_extensions *copy;
		if (!!tpp_extensions_state_getid(&self->te_state, id) == !!enabled)
			return TPP_EOK; /* Unchanged -> no need to actually copy! */
		copy = (tpp_extensions *)tpp_malloc(sizeof(tpp_extensions));
		if tpp_unlikely(!copy)
			goto err_nomem;
		tpp_memcpy(copy, self, sizeof(tpp_extensions));
		self->te_prev    = copy;
		self->te_pushcnt = 0;
	}
	tpp_extensions_state_setid(&self->te_state, id, enabled);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_HAVE_EXTENSIONS_PUSH_POP */
#endif /* TPP_HAVE_EXTENSIONS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_EXTENSIONS_C */
