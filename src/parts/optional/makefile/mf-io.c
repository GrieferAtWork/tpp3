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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_MF_IO_C
#define GUARD_TPP_OPTIONAL_MAKEFILE_MF_IO_C 1
#define TPP_BUILDING_OPTIONAL 1
#define TPP_MAKEFILE_BUILDING 1

#include "api.h"

#include "config.h"
#include "mf-io.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_MAKEFILE_HAVE_IO_HANDLE
#ifdef tpp_makefile_io_handle_IS_BUILTIN

/* Open a file for writing
 * @return: TPP_EOK:    Success (*p_result was populated and must eventually be closed by caller)
 * @return: TPP_ENOENT: Some parent-path-component of `filename` does not exist
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_io_open(/*utf-8*/ char const *tpp_restrict filename,
                     tpp_makefile_io_handle *tpp_restrict p_result) {
#ifdef tpp_makefile_io_handle_IS_HANDLE
	DWORD const dwDesiredAccess       = FILE_GENERIC_WRITE;
	DWORD const dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
	DWORD const dwFlagsAndAttributes  = FILE_ATTRIBUTE_NORMAL;
	DWORD const dwCreationDisposition = CREATE_ALWAYS;
	HANDLE hFile;

	TPP_SYSCALL({
		hFile = CreateFileA(filename, dwDesiredAccess, dwShareMode, NULL,
		                    dwCreationDisposition, dwFlagsAndAttributes, NULL);
	}, return);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE) {
		*p_result = hFile;
		return TPP_EOK;
	}

#if defined(tpp_io_handle_IS_BUILTIN) && defined(tpp_io_handle_IS_HANDLE)
	/* Convert utf-8 to wide, then pre-pend \\.\ to work around UNC limitations */
	{
		LPWSTR lpwFixedFilename;
		tpp_errno error = _tpp_nt_fix_unc_path(filename, &lpwFixedFilename);
		if (TPP_ISERR(error))
			return error;
#define tpp_io_open_return_error(err) return (tpp_free(lpwFilename), err)
		TPP_SYSCALL({
			hFile = CreateFileW(lpwFixedFilename, dwDesiredAccess, dwShareMode, NULL,
			                    dwCreationDisposition, dwFlagsAndAttributes, NULL);
		}, tpp_io_open_return_error);
#undef tpp_io_open_return_error
		tpp_free(lpwFixedFilename);
		if (hFile != NULL && hFile != INVALID_HANDLE_VALUE) {
			*p_result = hFile;
			return TPP_EOK;
		}
	}
#endif /* tpp_io_handle_IS_BUILTIN && tpp_io_handle_IS_HANDLE */

	return TPP_ENOENT;
#endif /* tpp_makefile_io_handle_IS_HANDLE */

#ifdef tpp_makefile_io_handle_IS_int
	int const mode = O_WRONLY |
	                 O_CREAT |
#ifdef O_BINARY
	                 O_BINARY |
#endif /* O_BINARY */
#ifdef O_CLOEXEC
	                 O_CLOEXEC |
#endif /* O_CLOEXEC */
	                 0;
	int fd;
	TPP_SYSCALL({
		fd = open(filename, mode, 0);
	}, return);
	if (fd == -1)
		return TPP_ENOENT;
	*p_result = fd;
	return TPP_EOK;
#endif /* tpp_makefile_io_handle_IS_int */

#ifdef tpp_makefile_io_handle_IS_FILE
	FILE *result;
	TPP_SYSCALL({
		result = fopen(filename, "wb");
	}, return);
	return result;
#endif /* tpp_makefile_io_handle_IS_FILE */
}

/* Close a file previously opened by `tpp_makefile_io_open()` */
#ifndef tpp_io_handle_IS_BUILTIN
TPP_IMPL void TPPCALL
tpp_makefile_io_close(tpp_makefile_io_handle file) {
#ifdef tpp_makefile_io_handle_IS_HANDLE
	TPP_SYSCALL_NOFAIL({
		(void)CloseHandle(file);
	});
#endif /* tpp_makefile_io_handle_IS_HANDLE */

#ifdef tpp_makefile_io_handle_IS_int
	TPP_SYSCALL_NOFAIL({
		(void)close(file);
	});
#endif /* tpp_makefile_io_handle_IS_int */

#ifdef tpp_makefile_io_handle_IS_FILE
	TPP_SYSCALL_NOFAIL({
		(void)fclose(file);
	});
#endif /* tpp_makefile_io_handle_IS_FILE */
}
#endif /* !tpp_io_handle_IS_BUILTIN */


/* Write data from `buf` into a given `file`
 * @return: * : The # of bytes written from `buf` (at most `bufsize`)
 * @return: TPP_SSIZE_OFERR(TPP_EIO):    I/O error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM): Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_makefile_io_write(tpp_makefile_io_handle file, void const *buf, tpp_size bufsize) {
#ifdef tpp_makefile_io_handle_IS_HANDLE
	BOOL bWrite;
	DWORD dwResult;
	DWORD dwBufsize = (DWORD)bufsize;
#if TPP_SIZEOF_tpp_size > 4
	if (bufsize > UINT32_C(0xffffffff))
		dwBufsize = UINT32_C(0xffffffff);
#endif /* TPP_SIZEOF_tpp_size > 4 */

	TPP_SYSCALL({
		bWrite = WriteFile(file, buf, dwBufsize, &dwResult, NULL);
	}, return TPP_SSIZE_OFERR);
	if (!bWrite)
		return TPP_SSIZE_OFERR(TPP_EIO);
	return (tpp_ssize)dwResult;
#endif /* tpp_makefile_io_handle_IS_HANDLE */

#ifdef tpp_makefile_io_handle_IS_int
	tpp_ssize result;
	TPP_SYSCALL({
		result = (tpp_ssize)write(file, buf, bufsize);
	}, return TPP_SSIZE_OFERR);
	if (result < 0)
		return TPP_SSIZE_OFERR(TPP_EIO);
	return result;
#endif /* tpp_makefile_io_handle_IS_int */

#ifdef tpp_makefile_io_handle_IS_FILE
	tpp_size result;
	TPP_SYSCALL({
		result = (tpp_size)fwrite(buf, 1, bufsize, file);
	}, return TPP_SSIZE_OFERR);
	if (result == 0 && ferror(file))
		return TPP_SSIZE_OFERR(TPP_EIO);
	return (tpp_ssize)result;
#endif /* tpp_makefile_io_handle_IS_FILE */
}

#endif /* tpp_makefile_io_handle_IS_BUILTIN */
#endif /* TPP_MAKEFILE_HAVE_IO_HANDLE */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_IO_C */
