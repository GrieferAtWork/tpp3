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
#ifndef GUARD_TPP_MACRO_C
#define GUARD_TPP_MACRO_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "macro.h"
#include "string.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_macro_destroy(tpp_macro *tpp_restrict self) {
	if (self->tm_body_chunk)
		tpp_string_decref(self->tm_body_chunk);
	if (TPP_MACRO_KIND_ISFUNC(self->tm_kind)) {
		tpp_free(self->tm_data.tmd_func.tmf_argv);
		tpp_free(self->tm_data.tmd_func.tmf_argbuf);
	}
}

/* Figure out the line/column of "pos" in "expanded_text", as produced
 * by "self", which must be "TPP_MACRO_KIND_ISFUNC(self->tm_kind)". */
TPP_INTERN_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_lcinfo TPPCALL
tpp_macro_func_lcinfo(tpp_macro const *tpp_restrict self,
                      tpp_string const *expanded_text,
                      tpp_char const *pos) {
	(void)self;
	(void)expanded_text;
	(void)pos;
	/* TODO */
	return tpp_lcinfo_of(0, 0);
}

#endif /* TPP_HAVE_CPP_MACROS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_MACRO_C */
