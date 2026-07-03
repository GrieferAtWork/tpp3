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
#if !defined(tpp_io_compare_mtime) && TPP_HAVE_IO_COMPARE_MTIME
#include <sys/stat.h>
#endif /* !tpp_io_compare_mtime && TPP_HAVE_IO_COMPARE_MTIME */
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <stdio.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#endif /* tpp_io_handle_IS_FILE */

TPP_DECL_BEGIN

/* Open a file for reading
 * @return: TPP_EOK:    Success (*p_result was populated and must eventually be closed by caller)
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_io_open(/*utf-8*/ char const *tpp_restrict filename,
            tpp_io_handle *tpp_restrict p_result) {
#ifdef tpp_io_handle_IS_HANDLE
	DWORD const dwDesiredAccess       = FILE_GENERIC_READ;
	DWORD const dwShareMode           = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
	DWORD const dwFlagsAndAttributes  = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS;
	DWORD const dwCreationDisposition = OPEN_EXISTING;
	HANDLE hFile;

	hFile = CreateFileA(filename, dwDesiredAccess, dwShareMode, NULL,
	                    dwCreationDisposition, dwFlagsAndAttributes, NULL);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE) {
		*p_result = hFile;
		return TPP_EOK;
	}

	/* TODO: Convert utf-8 to wide, then pre-pend \\.\ to work around UNC limitations */
	return TPP_ENOENT;
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
	int fd = open(filename, mode, 0);
	if (fd == -1)
		return TPP_ENOENT;
	*p_result = fd;
	return TPP_EOK;
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
 *              NOTE: Use "TPP_SSIZE_ISERR()" to detect error conditions!
 * @return: TPP_SSIZE_OFERR(TPP_EIO):         I/O error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):      Out of memory
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: TPP_SSIZE_OFERR(TPP_EWOULDBLOCK): `nonblock' was given, but operation would block
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
			return TPP_SSIZE_OFERR(TPP_EIO);
		if (dwFileType == FILE_TYPE_PIPE) {
			BYTE temp_buffer[1];
			/* `WaitForSingleObject()' doesn't work on pipes (for some reason...) */
			dwResult = 0;
			if (PeekNamedPipe(file, temp_buffer, sizeof(temp_buffer),
			                  &dwResult, NULL, NULL) &&
			    dwResult == 0)
				return TPP_SSIZE_OFERR(TPP_EWOULDBLOCK);
		} else {
			dwResult = WaitForSingleObject(file, 0);
			if (dwResult == WAIT_TIMEOUT)
				return TPP_SSIZE_OFERR(TPP_EWOULDBLOCK);
		}
	}
#endif /* !TPP_HAVE_FILE_NONBLOCK */

	if (!ReadFile(file, buf, dwBufsize, &dwResult, NULL))
		return TPP_SSIZE_OFERR(TPP_EIO);
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
			return TPP_SSIZE_OFERR(TPP_EIO);
		if (!FD_ISSET(fd, &read_fds))
			return TPP_SSIZE_OFERR(TPP_EWOULDBLOCK);
	}
#endif /* !TPP_HAVE_FILE_NONBLOCK */
	return (tpp_ssize)read(file, buf, bufsize);
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	tpp_size result = (tpp_size)fread(buf, 1, bufsize, file);
	if (result == 0 && ferror(file))
		return TPP_SSIZE_OFERR(TPP_EIO);
	return (tpp_ssize)result;
#endif /* tpp_io_handle_IS_FILE */
}


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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((4, 5)) tpp_errno TPPCALL
tpp_io_compare_mtime(char const *lhs_filename, tpp_io_handle lhs_handle, bool lhs_handle_valid,
                     char const *rhs_filename, int *tpp_restrict p_cmp_result) {
#ifdef tpp_io_handle_IS_HANDLE
	BY_HANDLE_FILE_INFORMATION lhsInfo;
	BY_HANDLE_FILE_INFORMATION rhsInfo;
	WIN32_FILE_ATTRIBUTE_DATA temp;
	FILETIME ftLastAccessed;
	tpp_io_handle temp_handle;
	(void)lhs_filename;
	if (lhs_handle_valid && GetFileInformationByHandle(lhs_handle, &lhsInfo)) {
		/* Got lhs info! */
	} else if (lhs_filename) {
		if (GetFileAttributesExA(lhs_filename, GetFileExInfoStandard, &temp)) {
			lhsInfo.ftLastWriteTime = temp.ftLastWriteTime;
		} else {
			/* Try to properly open to deal with UNC path errors */
			BOOL bHasLhsInfo;
			tpp_errno error = tpp_io_open(rhs_filename, &temp_handle);
			if (TPP_ISERR(error))
				return error; /* Probably TPP_ENOENT */
			/* (try to) prevent this open from counting towards rhs_filename's last-accessed timestamp */
			ftLastAccessed.dwLowDateTime  = (DWORD)UINT32_C(0xffffffff);
			ftLastAccessed.dwHighDateTime = (DWORD)UINT32_C(0xffffffff);
			(void)SetFileTime(temp_handle, NULL, &ftLastAccessed, NULL);

			/* Query info on "temp_handle" */
			bHasLhsInfo = GetFileInformationByHandle(temp_handle, &lhsInfo);
			tpp_io_close(temp_handle);
			if (!bHasLhsInfo)
				return TPP_ELAST; /* Cannot compare */
		}
	} else {
		return TPP_ENOENT; /* Cannot compare */
	}

	if (GetFileAttributesExA(rhs_filename, GetFileExInfoStandard, &temp)) {
		rhsInfo.ftLastWriteTime = temp.ftLastWriteTime;
	} else {
		/* Try to properly open to deal with UNC path errors */
		BOOL bHasRhsInfo2;
		tpp_errno error = tpp_io_open(rhs_filename, &temp_handle);
		if (TPP_ISERR(error))
			return error; /* Probably TPP_ENOENT */

		/* (try to) prevent this open from counting towards rhs_filename's last-accessed timestamp */
		ftLastAccessed.dwLowDateTime  = (DWORD)UINT32_C(0xffffffff);
		ftLastAccessed.dwHighDateTime = (DWORD)UINT32_C(0xffffffff);
		(void)SetFileTime(temp_handle, NULL, &ftLastAccessed, NULL);

		/* Query info on "rhs_handle" */
		bHasRhsInfo2 = GetFileInformationByHandle(temp_handle, &rhsInfo);
		tpp_io_close(temp_handle);
		if (!bHasRhsInfo2)
			return TPP_ELAST; /* Cannot compare */
	}

	/* Compare timestamps */
	if (lhsInfo.ftLastWriteTime.dwHighDateTime < rhsInfo.ftLastWriteTime.dwHighDateTime) {
		*p_cmp_result = -1;
	} else if (lhsInfo.ftLastWriteTime.dwHighDateTime > rhsInfo.ftLastWriteTime.dwHighDateTime) {
		*p_cmp_result = 1;
	} else if (lhsInfo.ftLastWriteTime.dwLowDateTime < rhsInfo.ftLastWriteTime.dwLowDateTime) {
		*p_cmp_result = -1;
	} else if (lhsInfo.ftLastWriteTime.dwLowDateTime > rhsInfo.ftLastWriteTime.dwLowDateTime) {
		*p_cmp_result = 1;
	} else {
		*p_cmp_result = 0;
	}
	return TPP_EOK;
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
	struct stat lhs_st;
	struct stat rhs_st;
	if (lhs_handle_valid) {
		if (fstat(lhs_handle, &lhs_st) != 0)
			return TPP_ELAST; /* Cannot compare */
	} else {
		if (stat(lhs_filename, &lhs_st) != 0)
			return TPP_ELAST; /* Cannot compare */
	}
	if (stat(rhs_filename, &lhs_st) != 0)
		return TPP_ENOENT;
	if (lhs_st.st_mtime < rhs_st.st_mtime) {
		*p_cmp_result = -1;
	} else if (lhs_st.st_mtime > rhs_st.st_mtime) {
		*p_cmp_result = 1;
	} else {
		/* XXX: Look at "st_mtim" / "st_mtimes" / "st_mtimensec" */
		*p_cmp_result = 0;
	}
	return TPP_EOK;
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
#ifndef TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_IO_COMPARE_MTIME' is enabled, but no way to implement on this OS. Supply your own definition, or build with '-DTPP_HAVE_IO_COMPARE_MTIME=0' + '-DTPP_HAVE_PRAGMA_GCC_DEPENDENCY=0'"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
	(void)lhs_filename;
	(void)lhs_handle;
	(void)lhs_handle_valid;
	(void)rhs_filename;
	(void)p_cmp_result;
	return TPP_ELAST;
#endif /* tpp_io_handle_IS_FILE */
}
#endif /* !tpp_io_compare_mtime */
#endif /* TPP_HAVE_IO_COMPARE_MTIME */


TPP_DECL_END
#endif /* tpp_io_handle_IS_BUILTIN */
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FILE_IO_C */
