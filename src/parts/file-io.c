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
#ifndef GUARD_TPP_FILE_IO_C
#define GUARD_TPP_FILE_IO_C 1
#define TPP_BUILDING 1

#include "api.h"
#include "config.h"
#include "error.h"
#include "file-io.h"

/*[[[tpp-begin]]]*/
#ifdef tpp_io_handle_IS_BUILTIN

#ifdef tpp_io_handle_IS_HANDLE
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <Windows.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <fcntl.h>
#ifdef _MSC_VER
#include <io.h>
#else /* _MSC_VER */
#include <unistd.h>
#if TPP_HAVE_FILE_NONBLOCK
#include <sys/select.h>
#endif /* !TPP_HAVE_FILE_NONBLOCK */
#endif /* !_MSC_VER */
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <stdio.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#endif /* tpp_io_handle_IS_FILE */

TPP_DECL_BEGIN

/* Open a file for reading
 * @return: tpp_io_handle_INVALID: No such file or directory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_io_handle TPPCALL
tpp_io_open(/*utf-8*/ char const *filename) {
#ifdef tpp_io_handle_IS_HANDLE
	DWORD const dwDesiredAccess       = FILE_GENERIC_READ;
	DWORD const dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
	DWORD const dwFlagsAndAttributes  = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS;
	DWORD const dwCreationDisposition = OPEN_EXISTING;
	HANDLE hFile;

	hFile = CreateFileA(filename, dwDesiredAccess, dwShareMode, NULL,
	                    dwCreationDisposition, dwFlagsAndAttributes, NULL);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE)
		return hFile;
	/* TODO: Convert utf-8 to wide, then pre-pend \\.\ to work around UNC limitations */
	return tpp_io_handle_INVALID;
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
	int const mode = O_RDONLY |
#ifdef O_BINARY
	                 O_BINARY |
#endif /* O_BINARY */
#ifdef O_CLOEXEC
	                 O_CLOEXEC |
#endif /* O_CLOEXEC */
	                 0;
	return open(filename, mode, 0);
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	return fopen(filename, "rb");
#endif /* tpp_io_handle_IS_FILE */
}

/* Close a file previously opened by `tpp_io_open()' */
TPP_IMPL void TPPCALL tpp_io_close(tpp_io_handle file) {
#ifdef tpp_io_handle_IS_HANDLE
	(void)CloseHandle(file);
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
	(void)close(file);
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	(void)fclose(file);
#endif /* tpp_io_handle_IS_FILE */
}

/* Read data from a given `file' into `buf'
 * @return: * : The # of bytes read into `buf' (at most `bufsize')
 * @return: (tpp_ssize)TPP_EIO:         I/O error
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: (tpp_ssize)TPP_EWOULDBLOCK: `nonblock' was given, but operation would block
 * #endif // TPP_HAVE_FILE_NONBLOCK */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_io_read(tpp_io_handle file, void *buf,
            tpp_size bufsize tpp_io_nonblock__PARAM) {
#ifdef tpp_io_handle_IS_HANDLE
	DWORD dwResult;
	DWORD dwBufsize = (DWORD)bufsize;
#if TPP_SIZEOF_tpp_size > 4
	if (bufsize > UINT32_C(0xffffffff))
		dwBufsize = UINT32_C(0xffffffff);
#endif /* TPP_SIZEOF_tpp_size > 4 */

#if TPP_HAVE_FILE_NONBLOCK
	if (nonblock) {
		DWORD dwFileType = GetFileType(file);
		if (dwFileType == FILE_TYPE_UNKNOWN)
			return (tpp_ssize)TPP_EIO;
		if (dwFileType == FILE_TYPE_PIPE) {
			BYTE temp_buffer[1];
			/* `WaitForSingleObject()' doesn't work on pipes (for some reason...) */
			dwResult = 0;
			if (PeekNamedPipe(file, temp_buffer, sizeof(temp_buffer),
			                  &dwResult, NULL, NULL) &&
			    dwResult == 0)
				return (tpp_ssize)TPP_EWOULDBLOCK;
		} else {
			dwResult = WaitForSingleObject(file, 0);
			if (dwResult == WAIT_TIMEOUT)
				return (tpp_ssize)TPP_EWOULDBLOCK;
		}
	}
#endif /* !TPP_HAVE_FILE_NONBLOCK */

	if (!ReadFile(file, buf, dwBufsize, &dwResult, NULL))
		return TPP_EIO;
	return (tpp_ssize)dwResult;
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
#if TPP_HAVE_FILE_NONBLOCK
	if (nonblock) {
		fd_set read_fds;
		struct timeval timeout = { 0, 0 };
		int result;
		FD_ZERO(&read_fds);
		FD_SET(file, &read_fds);
		result = select(file + 1, &read_fds, NULL, NULL, &timeout);
		if (result < 0)
			return (tpp_ssize)TPP_EIO;
		if (!FD_ISSET(fd, &read_fds))
			return (tpp_ssize)TPP_EWOULDBLOCK;
	}
#endif /* !TPP_HAVE_FILE_NONBLOCK */
	return (tpp_ssize)read(file, buf, bufsize);
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	tpp_size result = (tpp_size)fread(buf, 1, bufsize, file);
	if (result == 0 && ferror(file))
		return (tpp_ssize)TPP_EIO;
	return (tpp_ssize)result;
#endif /* tpp_io_handle_IS_FILE */
}

TPP_DECL_END
#endif /* tpp_io_handle_IS_BUILTIN */
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FILE_IO_C */
