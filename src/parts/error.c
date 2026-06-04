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
#ifndef GUARD_TPP_ERROR_C
#define GUARD_TPP_ERROR_C 1
#define TPP_BUILDING 1

#include "api.h"
#include "config.h"
#include "error.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_STRERROR
/* Return a human-readable descriptor of "error" */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_strerror(tpp_errno error) {
	switch (error) {
	case TPP_EOK:
		return "Success";
	case TPP_ENOMEM:
		return "Out of memory";
	case TPP_EIO:
		return "I/O operation failed";
#if TPP_HAVE_FILE_NONBLOCK
	case TPP_EWOULDBLOCK:
		return "Operation would have blocked";
#endif /* TPP_HAVE_FILE_NONBLOCK */
	case TPP_ENOENT:
		return "No such file or directory";
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
	case TPP_EMASKED:
		return "File has been masked";
#endif /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
#if TPP_HAVE_WARNINGS
	case TPP_EWARNPRINT:
		return "Error while printing warning";
#endif /* TPP_HAVE_WARNINGS */
	default:
		return "Unknown error";
	}
}
#endif /* TPP_HAVE_STRERROR */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_ERROR_C */
