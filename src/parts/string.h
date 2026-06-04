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
#ifndef GUARD_TPP_STRING_H
#define GUARD_TPP_STRING_H 1

#include "api.h"
#include "config.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

typedef struct tpp_string {
	tpp_refcnt ts_refcnt;              /* Reference counter */
	tpp_size   ts_len;                 /* [const] Length of the string */
	tpp_char   ts_str[TPP_FLEX_ARRAY]; /* [const][ts_len] String content */
/*	tpp_char   ts_nul;                  * [const][== 0] Trailing \0-character */
} tpp_string;

/* Helpers for interacting with TPP strings */
#define tpp_string_destroy(self)  tpp_free(self)
#define tpp_string_incref(self)   tpp_refcnt_inc(&(self)->ts_refcnt)
#define tpp_string_isshared(self) tpp_refcnt_isshared(&(self)->ts_refcnt)
#define tpp_string_decref(self) \
	(void)(tpp_refcnt_decfetch(&(self)->ts_refcnt) || (tpp_string_destroy(self), 0))

#define tpp_string_sizeof(len)     (tpp_offsetof(tpp_string, ts_str) + ((len) + 1) * sizeof(tpp_char))
#define _tpp_string_trymalloc(len) ((tpp_string *)tpp_trymalloc(tpp_string_sizeof(len)))
#define _tpp_string_malloc(len)    ((tpp_string *)tpp_malloc(tpp_string_sizeof(len)))

/* Allocate new (uninitialized) string buffers
 * @return: NULL: Propagate TPP_ENOMEM */
TPP_DECL TPP_WUNUSED tpp_string *TPPCALL tpp_string_trymalloc(tpp_size len);
TPP_DECL TPP_WUNUSED tpp_string *TPPCALL tpp_string_malloc(tpp_size len);

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_STRING_H */
