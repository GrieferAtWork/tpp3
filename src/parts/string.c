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
#ifndef GUARD_TPP_STRING_C
#define GUARD_TPP_STRING_C 1
#define TPP_BUILDING 1

#include "api.h"
#include "config.h"
#include "string.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

TPP_IMPL TPP_WUNUSED tpp_string *TPPCALL
tpp_string_trymalloc(tpp_size len) {
	tpp_string *result = _tpp_string_trymalloc(len);
	if tpp_likely(result) {
		result->ts_refcnt   = 1;
		result->ts_len      = len;
		result->ts_str[len] = '\0';
	}
	return result;
}

TPP_IMPL TPP_WUNUSED tpp_string *TPPCALL
tpp_string_malloc(tpp_size len) {
	tpp_string *result = _tpp_string_malloc(len);
	if tpp_likely(result) {
		result->ts_refcnt   = 1;
		result->ts_len      = len;
		result->ts_str[len] = '\0';
	}
	return result;
}

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_STRING_C */
