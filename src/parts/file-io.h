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
#ifndef GUARD_TPP_FILE_IO_H
#define GUARD_TPP_FILE_IO_H 1

#include "api.h"
#include "config.h"

/*[[[tpp-begin]]]*/
#ifndef TPP_FS_HAVE_DRIVES
#define TPP_FS_HAVE_DRIVES TPP_OS_WINDOWS
#endif /* !TPP_FS_HAVE_DRIVES */

#ifndef TPP_FS_HAVE_ICASE
#define TPP_FS_HAVE_ICASE TPP_OS_WINDOWS
#endif /* !TPP_FS_HAVE_ICASE */

#ifndef TPP_FS_SEP
#define TPP_FS_SEP   '/'
#define TPP_FS_SEP_S "/"
#endif /* !TPP_FS_SEP */

#ifndef TPP_FS_ALTSEP
#if TPP_OS_WINDOWS || defined(__CYGWIN__)
#define TPP_FS_ALTSEP '\\'
#else /* TPP_OS_WINDOWS */
#define TPP_FS_ALTSEP '/'
#endif /* !TPP_OS_WINDOWS */
#endif /* !TPP_FS_ALTSEP */

#ifndef TPP_FS_ISSEP
#if TPP_FS_SEP == TPP_FS_ALTSEP
#define TPP_FS_ISSEP(x) ((x) == TPP_FS_SEP)
#else /* TPP_FS_SEP == TPP_FS_ALTSEP */
#define TPP_FS_ISSEP(x) ((x) == TPP_FS_SEP || (x) == TPP_FS_ALTSEP)
#endif /* TPP_FS_SEP != TPP_FS_ALTSEP */
#endif /* !TPP_FS_ISSEP */

#ifndef TPP_FS_ISABS
#if TPP_FS_HAVE_DRIVES
#define TPP_FS_ISABS(filename, filename_len) ((filename_len) >= 2 && (filename)[0] && (filename)[1] == ':')
#else /* TPP_FS_HAVE_DRIVES */
#define TPP_FS_ISABS(filename, filename_len) ((filename_len) >= 1 && TPP_FS_ISSEP((filename)[0]))
#endif /* !TPP_FS_HAVE_DRIVES */
#endif /* !TPP_FS_ISABS */

#undef tpp_io_handle_IS_BUILTIN
#ifndef tpp_io_handle
#define tpp_io_handle_IS_BUILTIN
#if TPP_OS_WINDOWS
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <Windows.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_io_handle HANDLE
#define tpp_io_handle_IS_HANDLE
#elif TPP_OS_UNIX
#define tpp_io_handle int
#define tpp_io_handle_IS_int
#else /* ... */
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <stdio.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_io_handle FILE *
#define tpp_io_handle_IS_FILE
#if !defined(TPP_IGNORE_INVALID_CONFIGURATION) && TPP_HAVE_FILE_NONBLOCK
#error "No way to implement 'TPP_HAVE_FILE_NONBLOCK' on this OS"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && TPP_HAVE_FILE_NONBLOCK */
#endif /* !... */
#endif /* !tpp_io_handle */


/* Support for non-blocking I/O */
#if TPP_HAVE_FILE_NONBLOCK
#define tpp_io_nonblock__PARAM , int nonblock
#define tpp_io_nonblock__ARG   , nonblock
#else /* TPP_HAVE_FILE_NONBLOCK */
#define tpp_io_nonblock__PARAM /* nothing */
#define tpp_io_nonblock__ARG   /* nothing */
#endif /* !TPP_HAVE_FILE_NONBLOCK */


#ifdef tpp_io_handle_IS_BUILTIN
TPP_DECL_BEGIN

/* Open a file for reading
 * @return: TPP_EOK:    Success (*p_result was populated and must eventually be closed by caller)
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_io_open(/*utf-8*/ char const *tpp_restrict filename,
            tpp_io_handle *tpp_restrict p_result);

/* Close a file previously opened by `tpp_io_open()' */
TPP_DECL void TPPCALL tpp_io_close(tpp_io_handle file);

/* Read data from a given `file' into `buf'
 * @return: * : The # of bytes read into `buf' (at most `bufsize')
 *              NOTE: Use "TPP_SSIZE_ISERR()" to detect error conditions!
 * @return: TPP_SSIZE_OFERR(TPP_EIO):         I/O error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):      Out of memory
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: TPP_SSIZE_OFERR(TPP_EWOULDBLOCK): `nonblock' was given, but operation would block
 * #endif // TPP_HAVE_FILE_NONBLOCK */
TPP_DECL TPP_WUNUSED TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_io_read(tpp_io_handle file, void *buf, tpp_size bufsize tpp_io_nonblock__PARAM);

#if TPP_HAVE_IO_COMPARE_MTIME
/* Compare the last-modified timestamp of "lhs_handle" with the last-
 * modified timestamp of "rhs_filename". If available, "lhs_filename"
 * specifies the filename linked to "lhs_handle", though this may be
 * "NULL" if unknown.
 *
 * This function is used to implement "#pragma GCC dependency"
 *
 * @param: lhs_filename: Filename of "lhs_handle" (or "NULL" if unknown)
 * @param: lhs_handle:   File handle for left file (only valid if "lhs_handle_valid")
 * @param: rhs_filename: Filename of right file (always non-NULL)
 * @param: p_cmp_result: Set according to compare result on TPP_EOK:
 *                        *p_cmp_result <  0  <=>  fstat(lhs_handle).st_mtime <  stat(rhs_filename).st_mtime
 *                        *p_cmp_result == 0  <=>  fstat(lhs_handle).st_mtime == stat(rhs_filename).st_mtime
 *                        *p_cmp_result >  0  <=>  fstat(lhs_handle).st_mtime >  stat(rhs_filename).st_mtime
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOENT:  No such file "rhs_filename" (SOFT_ERROR)
 * @return: TPP_EIO:     I/O error (HARD_ERROR)
 * @return: TPP_ENOMEM:  Out of memory (HARD_ERROR)
 * @return: TPP_ELAST:   Unable to fstat(lhs_handle) / stat(lhs_filename)
 *                       [os-specific: or "lhs_filename == NULL" or doesn't exist], or unable
 *                       to implement function and "TPP_IGNORE_INVALID_CONFIGURATION" is enabled. */
#ifndef tpp_io_compare_mtime
TPP_DECL TPP_WUNUSED TPP_NONNULL((4, 5)) tpp_errno TPPCALL
tpp_io_compare_mtime(char const *lhs_filename, tpp_io_handle lhs_handle, bool lhs_handle_valid,
                     char const *rhs_filename, int *tpp_restrict p_cmp_result);
#endif /* !tpp_io_compare_mtime */
#endif /* TPP_HAVE_IO_COMPARE_MTIME */

TPP_DECL_END
#endif /* tpp_io_handle_IS_BUILTIN */
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FILE_IO_H */
