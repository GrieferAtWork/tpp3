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
#ifndef GUARD_TPP_TIME_C
#define GUARD_TPP_TIME_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "error.h"
#include "tuple.h"
#include "ctype.h"
#include "file-io.h"
#include "time.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_TIME_API
#if TPP_HAVE_TIME_ENVIRON

#if TPP_TUPLE_SIZE(TPP_CONFIG_TIME_ENVIRON)
static TPP_WUNUSED tpp_errno TPPCALL
tpp_time_fromenv_parse(void *arg, char const *envvalue) {
	tpp_time *const me = (tpp_time *)arg;
	tpp_uintmax value = 0;
	for (;; ++envvalue) {
		char ch = *envvalue;
		if (!tpp_ascii_isdigit(ch))
			break;
		value *= 10;
		value += tpp_ascii_asdigit(ch);
	}
	return tpp_time_ofepoch(me, value);
}
#elif !TPP_IGNORE_INVALID_CONFIGURATION
#error "`TPP_HAVE_TIME_ENVIRON` is enabled, but `TPP_CONFIG_TIME_ENVIRON` doesn't define any variables"
#endif /* ... */

/* Initialize `*p_time` from environment variables, as configured
 * by `TPP_CONFIG_TIME_ENVIRON`. If none of those variables are
 * defined or non-empty, return `TPP_ENOENT`.
 *
 * @return: TPP_EOK:    Success (`*p_time` has been initialized)
 * @return: TPP_ENOENT: SOFT_ERROR (none of the `TPP_CONFIG_TIME_ENVIRON`-variables are defined)
 * @return: * :         HARD_ERROR (`*p_time` is undefined) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_time_fromenviron(tpp_time *tpp_restrict p_time) {
#if TPP_TUPLE_SIZE(TPP_CONFIG_TIME_ENVIRON)
	tpp_errno result;
#define tpp_time_fromenv_tuple_item(_, index, value) \
	result = tpp_io_withenv(value, &tpp_time_fromenv_parse, p_time);
#define tpp_time_fromenv_tuple_sep(_, prev_index, prev_value, next_index, next_value) \
	if (result != TPP_ENOENT)                                                         \
		return result;
	TPP_TUPLE_FOREACH(TPP_CONFIG_TIME_ENVIRON,
	                  tpp_time_fromenv_tuple_sep,
	                  tpp_time_fromenv_tuple_item,
	                  ~)
#undef tpp_time_fromenv_tuple_sep
#undef tpp_time_fromenv_tuple_item
	return result;
#else /* TPP_TUPLE_SIZE(TPP_CONFIG_TIME_ENVIRON) */
	(void)p_time;
	return TPP_ENOENT;
#endif /* !TPP_TUPLE_SIZE(TPP_CONFIG_TIME_ENVIRON) */
}
#endif /* TPP_HAVE_TIME_ENVIRON */
#endif /* TPP_HAVE_TIME_API */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_TIME_C */
