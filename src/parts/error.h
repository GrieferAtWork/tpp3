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
#ifndef GUARD_TPP_ERROR_H
#define GUARD_TPP_ERROR_H 1

#include "api.h"
#include "config.h"
#include "file-io.h" /* Needed for "TPP_HAVE_FILE_NONBLOCK == -1" override */

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

typedef enum tpp_errno {
#define TPP_ISERR(error) ((error) != TPP_EOK)
	TPP_EOK         = 0,  /* Success */
	TPP_ENOMEM      = -1, /* tpp_malloc() or tpp_realloc() returned NULL */
	TPP_EIO         = -2, /* Filesystem I/O operation failed */
	TPP_ENOENT      = -3, /* No such file or directory */
#if TPP_HAVE_FILE_NONBLOCK
	TPP_EWOULDBLOCK = -4, /* Operation would block, but non-blocking was requested */
#endif /* TPP_HAVE_FILE_NONBLOCK */
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
	TPP_EMASKED     = -5, /* File has been masked */
#endif /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
#if TPP_HAVE_WARNINGS
	TPP_ELEXERROR   = -6, /* Hard lexer error (usually when too many -Werror were emitted) */
	TPP_EWARNPRINT  = -7, /* Printer registered for "tpp_lexer_warnf" returned an error */
#endif /* TPP_HAVE_WARNINGS */
} tpp_errno;

#if TPP_HAVE_STRERROR
/* Return a human-readable descriptor of "error" */
TPP_DECL TPP_WUNUSED char const *TPPCALL tpp_strerror(tpp_errno error);
#endif /* TPP_HAVE_STRERROR */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_ERROR_H */
