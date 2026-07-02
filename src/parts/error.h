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

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

enum {
	_TPP_ERRCODE_OK,
	_TPP_ERRCODE_NOMEM,
	_TPP_ERRCODE_IO,
	_TPP_ERRCODE_NOENT,
#if TPP_HAVE_FILE_NONBLOCK
	_TPP_ERRCODE_WOULDBLOCK,
#endif /* TPP_HAVE_FILE_NONBLOCK */
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
	_TPP_ERRCODE_MASKED,
#endif /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
#if TPP_HAVE_WARNINGS
	_TPP_ERRCODE_LEXERROR,
	_TPP_ERRCODE_WARNPRINT,
#endif /* TPP_HAVE_WARNINGS */
	_TPP_ERRCODE_COUNT,
};


/* NOTE: "[SOFT_ERROR]" are "temporary" errors that are intended to-be recovered from.
 *       These errors should be caught & dealt with at appropriate points in the code. */
typedef enum tpp_errno {
#define TPP_ISERR(error)     ((error) != TPP_EOK)

	/* --------------------------------------------------------------------
	 * NO_ERROR: TPP_EOK
	 * --------------------------------------------------------------------
	 *
	 * Success (operation completed without errors) */
	TPP_EOK = -_TPP_ERRCODE_OK,



	/* --------------------------------------------------------------------
	 * HARD_ERROR: TPP_ENOMEM
	 * --------------------------------------------------------------------
	 *
	 * Out of memory
	 *
	 * A call to `tpp_malloc()' or `tpp_realloc()' returned NULL, indicating
	 * that the system is out of heap memory. */
	TPP_ENOMEM = -_TPP_ERRCODE_NOMEM,



	/* --------------------------------------------------------------------
	 * HARD_ERROR: TPP_EIO
	 * --------------------------------------------------------------------
	 *
	 * Filesystem I/O operation failed */
	TPP_EIO = -_TPP_ERRCODE_IO,



	/* --------------------------------------------------------------------
	 * SOFT_ERROR: TPP_ENOENT
	 * --------------------------------------------------------------------
	 *
	 * No such file or directory
	 *
	 * WARNING: This error is _NOT_ returned when a #include-directive fails!
	 *          As a matter of fact, this error is _NEVER_ returned by APIs
	 *          such as `tpp_lexer_yield()' (there is no TPP_TOK_ENOENT).
	 * Instead, this error is used internally to indicate that a file could
	 * not be opened, however TPP may inherently handle this by trying to
	 * use some other filename in order to open a file. e.g.: when you have
	 * specified multiple system include paths, and TPP is parsing a line
	 * such as:
	 * >> #include <stdio.h>
	 *
	 * When it will try to open the following files (in order) and stop the
	 * first time one of these open calls succeeds:
	 * - tpp_io_open("/home/me/projects/tpp/stdio.h")
	 * - tpp_io_open("/usr/local/include/stdio.h")
	 * - tpp_io_open("/usr/include/stdio.h")
	 *
	 * Even if all of those calls fail, you will NOT see this error escape
	 * out of the handler for #include-directives. At that point, TPP will
	 * instead trigger the appropriate tpp_lexer_warnf() warning, which
	 * might then return "TPP_ELEXERROR". And it will be **that** error
	 * that will be propagated out of `tpp_lexer_yield()'; not TPP_ENOENT */
	TPP_ENOENT = -_TPP_ERRCODE_NOENT,



#if TPP_HAVE_FILE_NONBLOCK
	/* --------------------------------------------------------------------
	 * SOFT_ERROR: TPP_EWOULDBLOCK
	 * --------------------------------------------------------------------
	 *
	 * Operation would block, but non-blocking was requested
	 *
	 * You will only see this error if you made use of "TPP_HAVE_FILE_NONBLOCK"
	 * This is a temporary error that means that the next token cannot be read
	 * *right now* because reading from the underlying I/O file would block.
	 *
	 * -> You will not see this error when building with "-DTPP_HAVE_FILE_NONBLOCK=0"
	 * -> You will not see this error when not using the "TPP_FILE_IOFLAGS_NONBLOCK" flag */
	TPP_EWOULDBLOCK = -_TPP_ERRCODE_WOULDBLOCK,
#endif /* TPP_HAVE_FILE_NONBLOCK */



#if TPP_HAVE_KEYWORDS_OPENFILE_EX
	/* --------------------------------------------------------------------
	 * SOFT_ERROR: TPP_EMASKED
	 * --------------------------------------------------------------------
	 *
	 * File cannot be opened because it has been masked. Used internally to
	 * describe a file that exists, but should not be included (again) due
	 * to a `#pragma once', or because `#import' is being used (and had
	 * already been used once before) */
	TPP_EMASKED = -_TPP_ERRCODE_MASKED,
#endif /* TPP_HAVE_KEYWORDS_OPENFILE_EX */



#if TPP_HAVE_WARNINGS
	/* --------------------------------------------------------------------
	 * SOFT_ERROR / HARD_ERROR: TPP_ELEXERROR
	 * --------------------------------------------------------------------
	 *
	 * Hard lexer error (usually when too many -Werror were emitted)
	 *
	 * This error should be treated as a HARD_ERROR in most cases, in that
	 * it should be propagated. However, since it is always generated by
	 * TPP itself, you *could* in theory also change lexer configuration
	 * and re-try the failing operation (though if you do that, you will
	 * probably run into the same error again) */
	TPP_ELEXERROR = -_TPP_ERRCODE_LEXERROR,

	/* --------------------------------------------------------------------
	 * HARD_ERROR: TPP_EWARNPRINT
	 * --------------------------------------------------------------------
	 *
	 * Printer registered for "tpp_lexer_warnf" returned an error.
	 * Since this error is not related to TPP itself, this error should
	 * be propagated. */
	TPP_EWARNPRINT = -_TPP_ERRCODE_WARNPRINT,
#endif /* TPP_HAVE_WARNINGS */


	TPP_ELAST = (-_TPP_ERRCODE_COUNT) + 1, /* Last defined error number */
} tpp_errno;


/* Helper macros for embedding error codes in "tpp_ssize" values. */
#define /*tpp_ssize*/ TPP_SSIZE_OFERR(/*tpp_errno*/ e) ((tpp_ssize)(int)(e))
#define /*tpp_errno*/ TPP_SSIZE_ASERR(/*tpp_ssize*/ v) ((tpp_errno)(int)(v))
#if 1
#define /*tpp_errno*/ TPP_SSIZE_ASERR_OR_EOK(/*tpp_ssize*/ v) ((tpp_errno)(int)(v))
#else
#define /*tpp_errno*/ TPP_SSIZE_ASERR_OR_EOK(/*tpp_ssize*/ v) ((v) == 0 ? TPP_EOK : (tpp_errno)(int)(v))
#endif
#if TPP_DEBUG
#define /*bool*/ TPP_SSIZE_ISERR(/*tpp_ssize*/ v) \
	(((v) < 0) ? (tpp_assert((tpp_size)(v) >= (tpp_size)TPP_SSIZE_OFERR(TPP_ELAST)), 1) : 0)
#define /*bool*/ TPP_SSIZE_ISERR_OR_EOK(/*tpp_ssize*/ v) \
	((v) <= 0 ? (tpp_assert((v) >= TPP_SSIZE_OFERR(TPP_ELAST)), 1) : 0)
#else /* TPP_DEBUG */
#define /*bool*/ TPP_SSIZE_ISERR(/*tpp_ssize*/ v) ((v) < 0)
#define /*bool*/ TPP_SSIZE_ISERR_OR_EOK(/*tpp_ssize*/ v) ((v) <= 0)
#endif /* !TPP_DEBUG */



#if TPP_HAVE_STRERROR
/* Return a human-readable descriptor of "error" */
TPP_DECL TPP_CONSTCALL TPP_RETNONNULL TPP_WUNUSED
char const *TPPCALL tpp_strerror(tpp_errno error);
#endif /* TPP_HAVE_STRERROR */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_ERROR_H */
