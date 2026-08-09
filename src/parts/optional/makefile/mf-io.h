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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_MF_IO_H
#define GUARD_TPP_OPTIONAL_MAKEFILE_MF_IO_H 1

#include "api.h"

#include "config.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_MAKEFILE_HAVE_IO_HANDLE

#ifndef tpp_makefile_io_handle
#define tpp_makefile_io_handle_IS_BUILTIN
#ifdef tpp_io_handle_IS_BUILTIN
#define tpp_makefile_io_handle tpp_io_handle
#ifdef tpp_io_handle_IS_HANDLE
#define tpp_makefile_io_handle_IS_HANDLE
#endif /* !tpp_io_handle_IS_HANDLE */
#ifdef tpp_io_handle_IS_int
#define tpp_makefile_io_handle_IS_int
#endif /* !tpp_io_handle_IS_int */
#ifdef tpp_io_handle_IS_FILE
#define tpp_makefile_io_handle_IS_FILE
#endif /* !tpp_io_handle_IS_FILE */
#elif TPP_OS_WINDOWS
#if !TPP_HOST_NO_SYSTEM_INCLUDES
TPP_DECL_END
#include <Windows.h>
TPP_DECL_BEGIN
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_makefile_io_handle HANDLE
#define tpp_makefile_io_handle_IS_HANDLE
#elif TPP_OS_UNIX
#define tpp_makefile_io_handle int
#define tpp_makefile_io_handle_IS_int
#else /* ... */
#if !TPP_HOST_NO_SYSTEM_INCLUDES
TPP_DECL_END
#include <stdio.h>
TPP_DECL_BEGIN
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_makefile_io_handle FILE *
#define tpp_makefile_io_handle_IS_FILE
#endif /* !... */
#endif /* !tpp_makefile_io_handle */


#ifdef tpp_makefile_io_handle_IS_BUILTIN

/* Return a handle for the hosting process's STDOUT stream */
#ifdef tpp_makefile_io_handle_IS_HANDLE
#define tpp_makefile_io_getstdout() GetStdHandle(STD_OUTPUT_HANDLE)
#elif defined(tpp_makefile_io_handle_IS_int)
#ifdef STDOUT_FILENO
#define tpp_makefile_io_getstdout() STDOUT_FILENO
#else /* STDOUT_FILENO */
#define tpp_makefile_io_getstdout() 1
#endif /* !STDOUT_FILENO */
#elif defined(tpp_makefile_io_handle_IS_FILE)
#define tpp_makefile_io_getstdout() stdout
#endif /* ... */

/* Open a file for writing
 * @return: TPP_EOK:    Success (*p_result was populated and must eventually be closed by caller)
 * @return: TPP_ENOENT: Some parent-path-component of `filename` does not exist
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_io_open(/*utf-8*/ char const *tpp_restrict filename,
                     tpp_makefile_io_handle *tpp_restrict p_result);

/* Close a file previously opened by `tpp_makefile_io_open()` */
#ifdef tpp_io_handle_IS_BUILTIN
#define tpp_makefile_io_close(file) tpp_io_close(file)
#else /* tpp_io_handle_IS_BUILTIN */
TPP_DECL void TPPCALL tpp_makefile_io_close(tpp_makefile_io_handle file);
#endif /* !tpp_io_handle_IS_BUILTIN */

/* Write data from `buf` into a given `file`
 * @return: * : The # of bytes written from `buf` (at most `bufsize`)
 * @return: TPP_SSIZE_OFERR(TPP_EIO):    I/O error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM): Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_makefile_io_write(tpp_makefile_io_handle file, void const *buf, tpp_size bufsize);

#endif /* tpp_makefile_io_handle_IS_BUILTIN */
#endif /* TPP_MAKEFILE_HAVE_IO_HANDLE */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_IO_H */
