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
TPP_DECL_BEGIN

#ifdef tpp_io_handle_IS_BUILTIN
#if !TPP_HOST_NO_SYSTEM_INCLUDES
TPP_DECL_END

#ifdef tpp_io_handle_IS_HANDLE
#include <Windows.h>
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
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
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
#include <stdio.h>
#endif /* tpp_io_handle_IS_FILE */

#if TPP_HAVE_IO_NORMALIZE_FILENAME && TPP_OS_WINDOWS && !defined(tpp_io_normalize_filename)
#include <Windows.h>
#endif /* TPP_HAVE_IO_NORMALIZE_FILENAME && TPP_OS_WINDOWS && !tpp_io_normalize_filename */

TPP_DECL_BEGIN
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */

#ifdef tpp_io_handle_IS_HANDLE
/* Semi-public API to fix a UNC path on windows */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_nt_fix_unc_path(/*utf-8*/ char const *tpp_restrict sFilename,
                     LPWSTR *tpp_restrict plpwFixedFilename) {
	/* TODO */
	(void)sFilename;
	(void)plpwFixedFilename;
	return TPP_ENOENT;
}
#endif /* tpp_io_handle_IS_HANDLE */


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
	DWORD const dwFlagsAndAttributes  = FILE_ATTRIBUTE_NORMAL;
	DWORD const dwCreationDisposition = OPEN_EXISTING;
	HANDLE hFile;
	LPWSTR lpwFixedFilename;
	tpp_errno error;

	TPP_SYSCALL({
		hFile = CreateFileA(filename, dwDesiredAccess, dwShareMode, NULL,
		                    dwCreationDisposition, dwFlagsAndAttributes, NULL);
	}, return);
	if (hFile != NULL && hFile != INVALID_HANDLE_VALUE) {
		*p_result = hFile;
		return TPP_EOK;
	}

	/* Convert utf-8 to wide, then pre-pend \\.\ to work around UNC limitations */
	error = _tpp_nt_fix_unc_path(filename, &lpwFixedFilename);
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
	int fd;
	TPP_SYSCALL({
		fd = open(filename, mode, 0);
	}, return);
	if (fd == -1)
		return TPP_ENOENT;
	*p_result = fd;
	return TPP_EOK;
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	FILE *result;
	TPP_SYSCALL({
		result = fopen(filename, "rb");
	}, return);
	return result;
#endif /* tpp_io_handle_IS_FILE */
}

/* Close a file previously opened by `tpp_io_open()` */
TPP_IMPL void TPPCALL tpp_io_close(tpp_io_handle file) {
#ifdef tpp_io_handle_IS_HANDLE
	TPP_SYSCALL_NOFAIL({
		(void)CloseHandle(file);
	});
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
	TPP_SYSCALL_NOFAIL({
		(void)close(file);
	});
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	TPP_SYSCALL_NOFAIL({
		(void)fclose(file);
	});
#endif /* tpp_io_handle_IS_FILE */
}

/* Read data from a given `file` into `buf`
 * @return: * : The # of bytes read into `buf` (at most `bufsize`)
 *              NOTE: Use `TPP_SSIZE_ISERR()` to detect error conditions!
 * @return: TPP_SSIZE_OFERR(TPP_EIO):         I/O error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):      Out of memory
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: TPP_SSIZE_OFERR(TPP_EWOULDBLOCK): `nonblock` was given, but operation would block
 * #endif // TPP_HAVE_FILE_NONBLOCK */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_io_read(tpp_io_handle file, void *buf,
            tpp_size bufsize tpp_io_nonblock__PARAM) {
#ifdef tpp_io_handle_IS_HANDLE
	BOOL bRead;
	DWORD dwResult;
	DWORD dwBufsize = (DWORD)bufsize;
#if TPP_SIZEOF_tpp_size > 4
	if (bufsize > UINT32_C(0xffffffff))
		dwBufsize = UINT32_C(0xffffffff);
#endif /* TPP_SIZEOF_tpp_size > 4 */

#if TPP_HAVE_FILE_NONBLOCK
	if (nonblock) {
		DWORD dwFileType;
		TPP_SYSCALL({
			dwFileType = GetFileType(file);
		}, return TPP_SSIZE_OFERR);
		if (dwFileType == FILE_TYPE_UNKNOWN)
			return TPP_SSIZE_OFERR(TPP_EIO);
		if (dwFileType == FILE_TYPE_PIPE) {
			BYTE temp_buffer[1];
			BOOL bPeek;
			/* "WaitForSingleObject()" doesn't work on pipes (for some reason...) */
			dwResult = 0;
			TPP_SYSCALL({
				bPeek = PeekNamedPipe(file, temp_buffer, sizeof(temp_buffer), &dwResult, NULL, NULL);
			}, return TPP_SSIZE_OFERR);
			if (bPeek && dwResult == 0)
				return TPP_SSIZE_OFERR(TPP_EWOULDBLOCK);
		} else {
			TPP_SYSCALL({
				dwResult = WaitForSingleObject(file, 0);
			}, return TPP_SSIZE_OFERR);
			if (dwResult == WAIT_TIMEOUT)
				return TPP_SSIZE_OFERR(TPP_EWOULDBLOCK);
		}
	}
#endif /* !TPP_HAVE_FILE_NONBLOCK */

	TPP_SYSCALL({
		bRead = ReadFile(file, buf, dwBufsize, &dwResult, NULL);
	}, return TPP_SSIZE_OFERR);
	if (!bRead)
		return TPP_SSIZE_OFERR(TPP_EIO);
	return (tpp_ssize)dwResult;
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
	tpp_ssize result;
#if TPP_HAVE_FILE_NONBLOCK
	if (nonblock) {
		fd_set read_fds;
		struct timeval timeout = { 0, 0 };
		int result;
		FD_ZERO(&read_fds);
		FD_SET(file, &read_fds);
		TPP_SYSCALL({
			result = select(file + 1, &read_fds, NULL, NULL, &timeout);
		}, return TPP_SSIZE_OFERR);
		if (result < 0)
			return TPP_SSIZE_OFERR(TPP_EIO);
		if (!FD_ISSET(file, &read_fds))
			return TPP_SSIZE_OFERR(TPP_EWOULDBLOCK);
	}
#endif /* !TPP_HAVE_FILE_NONBLOCK */
	TPP_SYSCALL({
		result = (tpp_ssize)read(file, buf, bufsize);
	}, return TPP_SSIZE_OFERR);
	if (result < 0)
		return TPP_SSIZE_OFERR(TPP_EIO);
	return result;
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
	tpp_size result;
	TPP_SYSCALL({
		result = (tpp_size)fread(buf, 1, bufsize, file);
	}, return TPP_SSIZE_OFERR);
	if (result == 0 && ferror(file))
		return TPP_SSIZE_OFERR(TPP_EIO);
	return (tpp_ssize)result;
#endif /* tpp_io_handle_IS_FILE */
}


#if TPP_HAVE_IO_COMPARE_MTIME
/* Compare the last-modified timestamp of `lhs_handle` with the last-
 * modified timestamp of `rhs_filename`. If available, `lhs_filename`
 * specifies the filename linked to `lhs_handle`, though this may be
 * `NULL` if unknown.
 *
 * This function is used to implement `#pragma GCC dependency`
 *
 * @param: lhs_filename: Filename of `lhs_handle` (or `NULL` if unknown)
 * @param: lhs_handle:   File handle for left file (only valid if `lhs_handle_valid`)
 * @param: rhs_filename: Filename of right file (always non-NULL)
 * @param: p_cmp_result: Set according to compare result on TPP_EOK:
 *                        *p_cmp_result <  0  <=>  fstat(lhs_handle).st_mtime <  stat(rhs_filename).st_mtime
 *                        *p_cmp_result == 0  <=>  fstat(lhs_handle).st_mtime == stat(rhs_filename).st_mtime
 *                        *p_cmp_result >  0  <=>  fstat(lhs_handle).st_mtime >  stat(rhs_filename).st_mtime
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOENT:  No such file `rhs_filename` (SOFT_ERROR)
 * @return: TPP_EIO:     I/O error (HARD_ERROR)
 * @return: TPP_ENOMEM:  Out of memory (HARD_ERROR)
 * @return: TPP_ELAST:   Unable to fstat(lhs_handle) / stat(lhs_filename)
 *                       [os-specific: or `lhs_filename == NULL` or doesn't exist], or unable
 *                       to implement function and `TPP_IGNORE_INVALID_CONFIGURATION` is enabled. */
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
	BOOL bOK;
	(void)lhs_filename;
	if (lhs_handle_valid) {
		TPP_SYSCALL({
			bOK = GetFileInformationByHandle(lhs_handle, &lhsInfo);
		}, return);
		if (bOK)
			goto got_lhs_info; /* Got lhs info! */
	}
	if (lhs_filename) {
		TPP_SYSCALL({
			bOK = GetFileAttributesExA(lhs_filename, GetFileExInfoStandard, &temp);
		}, return);
		if (bOK) {
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
#define tpp_close_temp_handle_and_return(error) return (tpp_io_close(temp_handle), error)
			TPP_SYSCALL({
				(void)SetFileTime(temp_handle, NULL, &ftLastAccessed, NULL);
			}, tpp_close_temp_handle_and_return);

			/* Query info on "temp_handle" */
			TPP_SYSCALL({
				bHasLhsInfo = GetFileInformationByHandle(temp_handle, &lhsInfo);
			}, tpp_close_temp_handle_and_return);
#undef tpp_close_temp_handle_and_return
			tpp_io_close(temp_handle);
			if (!bHasLhsInfo)
				return TPP_ELAST; /* Cannot compare */
		}
	} else {
		return TPP_ENOENT; /* Cannot compare */
	}
got_lhs_info:
	TPP_SYSCALL({
		bOK = GetFileAttributesExA(rhs_filename, GetFileExInfoStandard, &temp);
	}, return);
	if (bOK) {
		rhsInfo.ftLastWriteTime = temp.ftLastWriteTime;
	} else {
		/* Try to properly open to deal with UNC path errors */
		BOOL bHasRhsInfo2;
		tpp_errno error = tpp_io_open(rhs_filename, &temp_handle);
		if (TPP_ISERR(error))
			return error; /* Probably TPP_ENOENT */

		/* (try to) prevent this open from counting towards rhs_filename's last-accessed timestamp */
#define tpp_close_temp_handle_and_return(error) return (tpp_io_close(temp_handle), error)
		ftLastAccessed.dwLowDateTime  = (DWORD)UINT32_C(0xffffffff);
		ftLastAccessed.dwHighDateTime = (DWORD)UINT32_C(0xffffffff);
		TPP_SYSCALL({
			(void)SetFileTime(temp_handle, NULL, &ftLastAccessed, NULL);
		}, tpp_close_temp_handle_and_return);

		/* Query info on "rhs_handle" */
		TPP_SYSCALL({
			bHasRhsInfo2 = GetFileInformationByHandle(temp_handle, &rhsInfo);
		}, tpp_close_temp_handle_and_return);
#undef tpp_close_temp_handle_and_return
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
	int status;
	if (lhs_handle_valid) {
		TPP_SYSCALL({
			status = fstat(lhs_handle, &lhs_st);
		}, return);
		if (status != 0)
			return TPP_ELAST; /* Cannot compare */
	} else {
		TPP_SYSCALL({
			status = stat(lhs_filename, &lhs_st);
		}, return);
		if (status != 0)
			return TPP_ELAST; /* Cannot compare */
	}
	TPP_SYSCALL({
		status = stat(rhs_filename, &rhs_st);
	}, return);
	if (status != 0)
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
#if !TPP_IGNORE_INVALID_CONFIGURATION
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


#if TPP_HAVE_IO_NORMALIZE_FILENAME
#ifndef tpp_io_normalize_filename
/* Given pointers to a string like this:
 * >> r"C:\Users\me\Desktop\0[unused-buffer-space]"
 *      ^           ^                             ^
 *      filename    after_last_sep                after_last_sep+after_last_sep_bufsize
 *
 * Check that the casing of the last part of the filename (here: `Desktop`)
 * is correct. If it is, do nothing and return `0`. If it isn't, check the
 * length of the correctly cased filename. If it's `<= after_last_sep_bufsize`,
 * copy it to `after_last_sep` (without a trailing `\0`-character) and return
 * the number of copied bytes (here: return <= after_last_sep_bufsize). If
 * it's `> after_last_sep_bufsize`, don't copy anything to `after_last_sep`
 * and return the required buffer size (here: return > after_last_sep_bufsize)
 *
 * @return: 0 :                          Casing is correct
 * @return: <= after_last_sep_bufsize:   Casing was fixed by copying `return` bytes to `after_last_sep`
 * @return: > after_last_sep_bufsize:    Casing is incorrect, and you must supply a larger buffer
 * @return: TPP_SSIZE_OFERR(TPP_ENOENT): [SOFT_ERROR] No such file or directory (you can stop checking casing)
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM): [HARD_ERROR] Out of memory
 * @return: TPP_SSIZE_OFERR(TPP_ENOIO):  [HARD_ERROR] I/O error */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_io_normalize_filename(char *filename, char *after_last_sep,
                          tpp_size after_last_sep_bufsize) {
#define tpp_io_normalize_filename_return_error(error) return TPP_SSIZE_OFERR(error)
#if TPP_OS_WINDOWS
	HANDLE hFind;
	WIN32_FIND_DATAA fData;
	tpp_size szCorrectSize;
	TPP_SYSCALL({
		hFind = FindFirstFileA(filename, &fData);
	}, tpp_io_normalize_filename_return_error);
	if (!hFind || hFind == INVALID_HANDLE_VALUE)
		return TPP_SSIZE_OFERR(TPP_ENOENT);
	TPP_SYSCALL({
		(void)FindClose(hFind);
	}, tpp_io_normalize_filename_return_error);
	if (tpp_strcmp(fData.cFileName, after_last_sep) == 0)
		return 0; /* Nothing changed! */
	szCorrectSize = tpp_strlen(fData.cFileName);
	if (szCorrectSize <= after_last_sep_bufsize)
		tpp_memcpy(after_last_sep, fData.cFileName, szCorrectSize * sizeof(char));
	return (tpp_ssize)szCorrectSize;
#else /* TPP_OS_WINDOWS */
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_HAVE_IO_NORMALIZE_FILENAME' is enabled, but no way to implement on this OS. Supply your own definition, or build with '-DTPP_HAVE_IO_NORMALIZE_FILENAME=0'"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
	(void)filename;
	(void)after_last_sep;
	(void)after_last_sep_bufsize;
	return 0;
#endif /* !TPP_OS_WINDOWS */
#undef tpp_io_normalize_filename_return_error
}
#endif /* !tpp_io_normalize_filename */
#endif /* TPP_HAVE_IO_NORMALIZE_FILENAME */
#endif /* tpp_io_handle_IS_BUILTIN */


#if TPP_HAVE_IO_SKIP_BLOCKING
#ifndef tpp_io_skip_blocking
/* Skip up to `max_bytes` of input from `file` whilst blocking,
 * storing the actual number of skipped bytes in `*p_skipped_bytes`
 * before returning `TPP_EOK`.
 *
 * @return: TPP_EOK: Success
 * @return: TPP_EIO: I/O error (HARD_ERROR) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((3)) tpp_errno TPPCALL
tpp_io_skip_blocking(tpp_io_handle file, tpp_uintmax max_bytes,
                     tpp_uintmax *tpp_restrict p_skipped_bytes) {
	tpp_uintmax skipped_bytes = 0;
#if defined(tpp_io_handle_IS_BUILTIN) && defined(tpp_io_handle_IS_HANDLE)
	/* XXX: Fast-pass when input isn't a pipe */
#elif defined(tpp_io_handle_IS_BUILTIN) && defined(tpp_io_handle_IS_int)
	/* XXX: Fast-pass when input isn't a pipe */
#endif
	while (skipped_bytes < max_bytes) {
		char buf[256];
		tpp_uintmax max_skip1 = max_bytes - skipped_bytes;
		tpp_size max_skip2 = max_skip1 < sizeof(buf) ? (tpp_size)max_skip1 : sizeof(buf);
		tpp_ssize read_status = tpp_io_read_blocking(file, buf, max_skip2);
		if (TPP_SSIZE_ISERR(read_status))
			return TPP_SSIZE_ASERR(read_status);
		skipped_bytes += (tpp_size)read_status;
		if ((tpp_size)read_status < max_skip2)
			break;
	}
	*p_skipped_bytes = skipped_bytes;
	return TPP_EOK;
}
#endif /* !tpp_io_skip_blocking */
#endif /* TPP_HAVE_IO_SKIP_BLOCKING */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FILE_IO_C */
