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
#ifndef GUARD_TPP_TIME_H
#define GUARD_TPP_TIME_H 1

#include "api.h"
#include "config.h"
#include "error.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_TIME_API
#ifndef tpp_time
#if !TPP_HOST_NO_SYSTEM_INCLUDES
TPP_DECL_END
#include <time.h>
TPP_DECL_BEGIN
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */

/* Time API */
#define tpp_time                      time_t
#define tpp_time_now(p_time)          (time(p_time), TPP_EOK)
#define tpp_time_empty(p_time)        (*(p_time) = 0)
#define tpp_time_isempty(p_time)      (*(p_time) == 0)
#if TPP_HAVE_LEXER_COPY
#define tpp_time_copy(p_self, p_from) (*(p_self) = *(p_from), TPP_EOK)
#endif /* TPP_HAVE_LEXER_COPY */

/* Time -> tm conversion (splitting time into its individual components) */
typedef struct tm tpp_tm;
#ifdef _MSC_VER
#define tpp_tm_fromtime(self, p_time) (localtime_s(self, p_time) == 0 ? TPP_EOK : TPP_EIO)
#else /* _MSC_VER */
#ifndef TPP_CONFIG_HAVE_LOCALTIME_R
#define TPP_CONFIG_HAVE_LOCALTIME_R 1 /* Override if your platform doesn't have this... */
#endif /* !TPP_CONFIG_HAVE_LOCALTIME_R */

#if TPP_CONFIG_HAVE_LOCALTIME_R
#define tpp_tm_fromtime(self, p_time) (localtime_r(p_time, self) ? TPP_EOK : TPP_EIO)
#else /* TPP_CONFIG_HAVE_LOCALTIME_R */
#define tpp_tm_fromtime tpp_tm_fromtime
TPP_INLINE tpp_errno TPPCALL tpp_tm_fromtime(tpp_tm *self, tpp_time *p_time) {
	struct tm *tmp = localtime(p_time);
	if (!tmp)
		return TPP_EIO;
	*self = *tmp;
	return TPP_EOK;
}
#endif /* !TPP_CONFIG_HAVE_LOCALTIME_R */
#endif /* !_MSC_VER */

#define tpp_tm_getsec(self)  ((self)->tm_sec)         /* [0, 60] */
#define tpp_tm_getmin(self)  ((self)->tm_min)         /* [0, 59] */
#define tpp_tm_gethour(self) ((self)->tm_hour)        /* [0, 23] */
#define tpp_tm_getmday(self) ((self)->tm_mday)        /* [1, 31] */
#define tpp_tm_getmon(self)  ((self)->tm_mon + 1)     /* [1, 12] */
#define tpp_tm_getyear(self) ((self)->tm_year + 1900) /* ... */
#define tpp_tm_getwday(self) ((self)->tm_wday)        /* [0, 6] */
#define tpp_tm_getyday(self) ((self)->tm_yday)        /* [0, 365] */

#endif /* !tpp_time */
#endif /* TPP_HAVE_TIME_API */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_TIME_H */
