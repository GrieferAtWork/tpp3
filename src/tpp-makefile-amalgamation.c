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
/* clang-format off */
#ifndef GUARD_TPP_MAKEFILE_AMALGAMATION_C
#define GUARD_TPP_MAKEFILE_AMALGAMATION_C 1

#ifndef TPP_MAKEFILE_AMALGAMATION_H
#define TPP_MAKEFILE_AMALGAMATION_H "tpp-makefile-amalgamation.h"
#endif /* !TPP_MAKEFILE_AMALGAMATION_H */

/* Include "tpp-makefile-amalgamation.h" header... */
#ifndef GUARD_TPP_MAKEFILE_AMALGAMATION_H
#undef TPP_MAKEFILE_BUILDING
#define TPP_MAKEFILE_BUILDING 1
#include TPP_MAKEFILE_AMALGAMATION_H
#endif /* !GUARD_TPP_MAKEFILE_AMALGAMATION_H */

#if !TPP_MAKEFILE_BUILDING
/************************************************************************/
/* File: parts/optional/makefile/expose-internals.h                     */
/************************************************************************/
/* If "tpp-emitter-amalgamation.h" was already included, re-define
 * `TPP_MAKEFILE_INTERNAL()` identifers to their unescaped names. */
#define tmfcl_mf               TPP_MAKEFILE_INTERNAL(tmfcl_mf)
#define tmfcl_state            TPP_MAKEFILE_INTERNAL(tmfcl_state)
#define tmfcl_target           TPP_MAKEFILE_INTERNAL(tmfcl_target)
#define tmfcl_outfile          TPP_MAKEFILE_INTERNAL(tmfcl_outfile)
#define tmfcl_flags            TPP_MAKEFILE_INTERNAL(tmfcl_flags)
#define tmf_feat               TPP_MAKEFILE_INTERNAL(tmf_feat)
#define tmf_flags              TPP_MAKEFILE_INTERNAL(tmf_flags)
#define tmf_bitset             TPP_MAKEFILE_INTERNAL(tmf_bitset)
#define tmff_USER_DEPENDENCIES TPP_MAKEFILE_INTERNAL(tmff_USER_DEPENDENCIES)
#define tmff_PHONY             TPP_MAKEFILE_INTERNAL(tmff_PHONY)
#define tmf_lexer              TPP_MAKEFILE_INTERNAL(tmf_lexer)
#define tmf_output             TPP_MAKEFILE_INTERNAL(tmf_output)
#define tmf_output_file        TPP_MAKEFILE_INTERNAL(tmf_output_file)
#define tmf_depc               TPP_MAKEFILE_INTERNAL(tmf_depc)
#define tmf_depa               TPP_MAKEFILE_INTERNAL(tmf_depa)
#define tmf_depv               TPP_MAKEFILE_INTERNAL(tmf_depv)
#define tmf_curcol             TPP_MAKEFILE_INTERNAL(tmf_curcol)
#define tmf_maxcol             TPP_MAKEFILE_INTERNAL(tmf_maxcol)

#endif /* !TPP_MAKEFILE_BUILDING */

/************************************************************************/
/* File: parts/optional/makefile/mf-io.c                                */
/************************************************************************/
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

/************************************************************************/
/* File: parts/optional/makefile/mf-features.c                          */
/************************************************************************/

#if TPP_MAKEFILE_HAVE_FEATURES
TPP_CONST_IMPL tpp_makefile_features const tpp_makefile_features_default = {
	/* .tmf_flags = */ {
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
		/* .tmff_USER_DEPENDENCIES = */ TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES),
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY)
		/* .tmff_PHONY             = */ TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_PHONY),
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY) */
	}
};
#endif /* TPP_MAKEFILE_HAVE_FEATURES */

/************************************************************************/
/* File: parts/optional/makefile/mf.c                                   */
/************************************************************************/

/* Apply Makefile-escape rules to `data...+=num_bytes` and print the result to `printer` */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_ssize TPPCALL
tpp_makefile_escape(tpp_formatprinter printer, void *arg,
                    tpp_char const *data, tpp_size num_bytes,
                    tpp_size *p_num_bytes_printed) {
	tpp_size count, num_bytes_printed = 0;
	tpp_ssize temp, result = 0;
	tpp_char const *iter = data;
	tpp_char const *end = data + num_bytes;
	while (iter < end) {
		tpp_char ch = *iter++;
		switch (ch) {
		case ' ':
		case '\t':
		case '#': {
			/* Must escape as \CHAR */
			tpp_char const *flush_end;
escape_space_or_tab:
			flush_end = iter - 1;
			count = (tpp_size)(flush_end - data);
			num_bytes_printed += count;
			temp = tpp_formatprinter_print(printer, arg, data, count);
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			num_bytes_printed += 1;
			temp = tpp_formatprinter_print_conststr(printer, arg, "\\");
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			data = flush_end;
		}	break;

		case '$': {
			/* Must escape as $$ */
			count = (tpp_size)(iter - data);
			num_bytes_printed += count;
			temp = tpp_formatprinter_print(printer, arg, data, count);
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			data = iter - 1; /* Print the $ a second time */
		}	break;

		case '\\': {
			/* This one gets complicated: count the # of consecutive \-characters: N.
			 * If what comes after is a ' ' or '\t' character, then we need to emit
			 * another N+1 \-characters. Otherwise, \-characters must not be escaped. */
			tpp_char const *bk = iter - 1;
			while (iter < end && *iter == '\\')
				++iter;
			if (iter < end) {
				ch = *iter;
				if (ch == ' ' || ch == '\t') {
					/* Print the \-sequence for the first time (excluding the trailing SPACE/TAB) */
					count = (tpp_size)(iter - data);
					num_bytes_printed += count;
					temp = tpp_formatprinter_print(printer, arg, data, count);
					if tpp_unlikely(temp < 0)
						goto err_temp;
					result += temp;
					data = bk; /* Print the \-sequence a second time (for a total of 2N) */
					++iter;    /* Consume the trailing SPACE/TAB */
					goto escape_space_or_tab; /* This will go on to print the final \ (for a total of 2N+1) */
				}
			}
		}	break;

		default: break;
		}
	}
	/* Flush remainder */
	count = (tpp_size)(end - data);
	num_bytes_printed += count;
	temp = tpp_formatprinter_print(printer, arg, data, (tpp_size)(end - data));
	if tpp_unlikely(temp < 0)
		goto err_temp;
	if (p_num_bytes_printed)
		*p_num_bytes_printed = num_bytes_printed;
	result += temp;
	return result;
err_temp:
	if (p_num_bytes_printed) {
		/* Doesn't really matter in this case... */
		*p_num_bytes_printed = num_bytes_printed;
	}
	return temp;
}

#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
static TPP_FORMATPRINTER_DEFINE(tpp_makefile_strlen_printer, arg, text, num_bytes) {
	(void)arg;
	(void)text;
	return (tpp_ssize)num_bytes;
}
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

#if TPP_MAKEFILE_HAVE_PHONY || TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_adddep(tpp_makefile *tpp_restrict self,
                    tpp_keyword const *tpp_restrict dep) {
	tpp_assert(self->tmf_depc <= self->tmf_depa);
	if (self->tmf_depc >= self->tmf_depa) {
		tpp_keyword const **new_depv;
		tpp_size new_depa = self->tmf_depa * 2;
		if (new_depa < 8)
			new_depa = 8;
		new_depv = (tpp_keyword const **)tpp_tryrealloc(self->tmf_depv,
		                                                new_depa *
		                                                sizeof(tpp_keyword const *));
		if tpp_unlikely(!new_depv) {
			new_depa = self->tmf_depc + 1;
			new_depv = (tpp_keyword const **)tpp_realloc(self->tmf_depv,
			                                             new_depa *
			                                             sizeof(tpp_keyword const *));
			if tpp_unlikely(!new_depv)
				return TPP_ENOMEM;
		}
		self->tmf_depa = new_depa;
		self->tmf_depv = new_depv;
	}
	self->tmf_depv[self->tmf_depc++] = dep;
	return TPP_EOK;
}

#if TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
static TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_makefile_hasdep(tpp_makefile *tpp_restrict self,
                    tpp_keyword const *tpp_restrict dep) {
	tpp_size i = self->tmf_depc;
	while (i--) {
		tpp_keyword const *mydep = self->tmf_depv[i];
		if (tpp_keyword_equals(dep, mydep))
			return true;
	}
	return false;
}
#endif /* TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */
#endif /* TPP_MAKEFILE_HAVE_PHONY || TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */


#if TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO
TPP_IMPL TPP_FORMATPRINTER_DEFINE(_tpp_makefile_builtin_file_output, arg, text, num_bytes) {
	tpp_makefile const *const self = (tpp_makefile const *)arg;
	tpp_makefile_io_handle const file = tpp_makefile_getoutput_io(self);
#ifndef __OPTIMIZE_SIZE__
	if (num_bytes == 0)
		return 0; /* Reduce # of system calls... */
#endif /* !__OPTIMIZE_SIZE__ */
	return tpp_makefile_io_write(file, text, num_bytes);
}
#endif /* TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO */


/* Same as `tpp_makefile_setoutput_io()`, but can be used to *directly*
 * initialize output by opening (or creating) a file on-disk.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: Some parent-path-component of `filename` does not exist
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_MAKEFILE_HAVE_OUTPUT_FILE
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_setoutput_file(tpp_makefile *tpp_restrict self,
                            char const *tpp_restrict filename) {
	tpp_makefile_io_handle file;
	tpp_errno error = tpp_makefile_io_open(filename, &file);
	if (TPP_ISERR(error))
		return error;

	/* Assign new output (also closing any potential previous output) */
	tpp_makefile_setoutput_io(self, file);
	return TPP_EOK;
}
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE */




/* Must be called *ONCE* after all input has been processed.
 * - This function will print a final, trailing LF, and (if enabled) re-
 *   emit dependencies as dummy targets (see `TPP_MAKEFILE_HAVE_PHONY`).
 * - This function is a no-op if `tpp_makefile_getenabled(self)` is `false`.
 *
 * @return: TPP_EOK: Success (or no-op)
 * @return: * : Error (as returned by `tpp_makefile_getoutput(self)`) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_flush(tpp_makefile *tpp_restrict self) {
	tpp_ssize output_temp;
	if (!tpp_makefile_getenabled(self))
		return TPP_EOK; /* Makefile not enabled... */

	/* Print trailing linefeed */
	output_temp = tpp_makefile_output_printraw_conststr(self, "\n");
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);

	/* Re-emit all dependencies as their own, phony targets. */
#if TPP_MAKEFILE_HAVE_PHONY
	if (tpp_makefile_has(self, PHONY)) {
		tpp_size i;
		for (i = 0; i < self->tmf_depc; ++i) {
			tpp_keyword const *dep = self->tmf_depv[i];
			tpp_char const *const filename = tpp_keyword_getstr(dep);
			tpp_size const filename_len    = tpp_keyword_getlen(dep);
			output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self),
			                                  self, filename, filename_len,
			                                  NULL);
			if (output_temp < 0)
				return TPP_SSIZE_ASERR(output_temp);
			output_temp = tpp_makefile_output_printraw_conststr(self, ":\n");
			if (output_temp < 0)
				return TPP_SSIZE_ASERR(output_temp);
		}
	}
#endif /* TPP_MAKEFILE_HAVE_PHONY */

	return TPP_EOK;
}


static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_new_dependency_hook_impl(tpp_makefile *tpp_restrict self,
                                      tpp_keyword const *tpp_restrict filename_kwd) {
	tpp_ssize output_temp;
	tpp_char const *const filename = tpp_keyword_getstr(filename_kwd);
	tpp_size const filename_len = tpp_keyword_getlen(filename_kwd);

	/* Ignore if #include-stack contains syshdr-files */
#if TPP_MAKEFILE_HAVE_USER_DEPENDENCIES
	tpp_lexer const *const lexer = tpp_makefile_getlexer(self);
	if (tpp_makefile_has(self, USER_DEPENDENCIES)) {
		tpp_file const *iter = tpp_lexer_getfile(lexer);
		do {
			if (tpp_file_getsystemheader(iter))
				return TPP_EOK;
		} while ((iter = tpp_file_getprev(iter)) != NULL);
	}
#endif /* TPP_MAKEFILE_HAVE_USER_DEPENDENCIES */

	/* Remember dependency in case we need to re-emit them as phonies later. */
#if TPP_MAKEFILE_HAVE_PHONY
	if (tpp_makefile_has(self, PHONY)) {
		tpp_errno error = tpp_makefile_adddep(self, filename_kwd);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_MAKEFILE_HAVE_PHONY */

	/* Check if we need to wrap the filename before printing it. */
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	if (self->tmf_curcol > 0) {
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH < 0
		if (tpp_makefile_getmaxcol(self) >= 0)
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH < 0 */
		{
			tpp_size output_len = 1; /* +1 for leading ' '-character */
			output_len += (tpp_size)tpp_makefile_escape(&tpp_makefile_strlen_printer,
			                                            NULL, filename, filename_len,
			                                            NULL);
			if ((self->tmf_curcol + (tpp_column)output_len) >= tpp_makefile_getmaxcol(self)) {
				/* Force a line-wrap */
				output_temp = tpp_makefile_output_printraw_conststr(self, " \\\n");
				if (output_temp < 0)
					return TPP_SSIZE_ASERR(output_temp);
				self->tmf_curcol = 0;
			}
		}
	}
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

	/* Print the dependency filename (following a preceding space) */
	output_temp = tpp_makefile_output_printraw_conststr(self, " ");
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	self->tmf_curcol += 1;
	{
		tpp_size count;
		output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self),
		                                  self, filename, filename_len,
		                                  &count);
		if (output_temp < 0)
			return TPP_SSIZE_ASERR(output_temp);
		self->tmf_curcol += (tpp_column)count;
	}
#else /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */
	output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self),
	                                  self, filename, filename_len,
	                                  NULL);
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);
#endif /* !TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */
	return TPP_EOK;
}

/* The main (mandatory) `NEW_DEPENDECY` hook that's used to
 * get notified whenever the lexer encounters a new dependency */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_makefile_new_dependency_hook(tpp_hook_cookie cookie, tpp_keyword *filename_kwd) {
	tpp_makefile *const self = tpp_makefile_ofcookie(cookie);
	return tpp_makefile_new_dependency_hook_impl(self, filename_kwd);
}

/* Handle missing file dependencies by (blindly) emitting them to the makefile */
#if TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_include_not_found_cb(void *arg, char const *str, tpp_size length) {
	/* Turn the filename into a keyword (so we can re-use the regular dependency-mechanism) */
	tpp_makefile *const self = (tpp_makefile *)arg;
	tpp_lexer *const lexer = tpp_makefile_getlexer(self);
	tpp_hash const hash = tpp_hashof((tpp_char const *)str, length);
	tpp_keyword const *ro_keyword = tpp_lexer_kwds_newkeyword(lexer, (tpp_char const *)str, length, hash);
	if tpp_unlikely(!ro_keyword)
		return TPP_ENOMEM;

	/* Check if this "dependency" is already known */
	if (tpp_makefile_hasdep(self, ro_keyword))
		return TPP_EOK; /* Already emitted -> don't do so again! */

#if TPP_MAKEFILE_HAVE_PHONY
	if (!tpp_makefile_has(self, PHONY))
#endif /* TPP_MAKEFILE_HAVE_PHONY */
	{
		tpp_errno error = tpp_makefile_adddep(self, ro_keyword);
		if (TPP_ISERR(error))
			return error;
	}
	return tpp_makefile_new_dependency_hook_impl(self, ro_keyword);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno
_tpp_makefile_include_not_found_hook(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind) {
	tpp_makefile *const self = tpp_makefile_ofcookie(cookie);
	tpp_lexer const *const lexer = tpp_makefile_getlexer(self);
	(void)include_kind; /* Ignored -- treat all missing includes the same. */

	/* NOTE: The `TPP_EOK` return value of `tpp_lexer_decode_include_string_cb()`
	 *       also acts to suppresses the `TPP_W_NO_SUCH_FILE` that would normally
	 *       be raised by the caller */
	return tpp_lexer_decode_include_string_cb(lexer, &tpp_makefile_include_not_found_cb, self);
}
#endif /* TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */


/************************************************************************/
/* File: parts/optional/makefile/mf-cli.c                               */
/************************************************************************/

#if TPP_MAKEFILE_HAVE_CLI

/* Define a function `tpp_makefile_cli_warnf()` */
#undef TPP_HAVE_MAKEFILE_CLI_WARN
#define TPP_HAVE_MAKEFILE_CLI_WARN \
	(TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT)

#if TPP_HAVE_MAKEFILE_CLI_WARN
static TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_cli_warnf(tpp_makefile *tpp_restrict self, tpp_char const *token_start,
                       tpp_size token_size, tpp_warning_id id, ...) {
	/* XXX: Use of `TPP_INTERNAL` here isn't allowed -- come up with
	 *      APIs to allow users to do this stuff without needing to
	 *      access TPP internal! */
	tpp_lexer *const lexer = tpp_makefile_getlexer(self);
	tpp_errno result;
	va_list args;
	union TPP_INTERNAL(tpp_lexer_core) saved_core = lexer->TPP_INTERNAL(tl_core);
	tpp_file *const file = tpp_lexer_getfile(lexer);
	tpp_file_init_text_utf8(file, TPP_CONFIG_CLI_FILENAME,
	                        NULL, token_start, token_size,
	                        TPP_LCINFO_INVALID, TPP_FILE_FLAGS_NORMAL);
	file->TPP_INTERNAL(tf_tpos) = token_start;
	file->TPP_INTERNAL(tf_pos)  = token_start + token_size;
	va_start(args, id);
	result = tpp_lexer_vwarnf(lexer, id, args);
	va_end(args);
	lexer->TPP_INTERNAL(tl_core) = saved_core;
	return result;
}
#endif /* TPP_HAVE_MAKEFILE_CLI_WARN */


enum {
	_TPP_MAKEFILE_CLI_LOADER_STATE_FIRST_INTERNAL = TPP_MAKEFILE_CLI_LOADER_STATE_DDASH,
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
	TPP_MAKEFILE_CLI_LOADER_STATE_MF, /* -MF FILE */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
	TPP_MAKEFILE_CLI_LOADER_STATE_MT, /* -MT TARGET */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
	TPP_MAKEFILE_CLI_LOADER_STATE_MQ, /* -MQ TARGET */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
};


/* Make makefile for being turned on (during `tpp_makefile_cli_loader_flush()`) */
#if TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM || TPP_MAKEFILE_HAVE_CLI_DASH_MG
static TPP_NONNULL((1)) void TPPCALL
tpp_makefile_cli_loader_enable_with_makefile_only(tpp_makefile_cli_loader *tpp_restrict self) {
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
	tpp_makefile_cli_loader_enableonlymakefile(self);
#endif /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
	tpp_makefile_cli_loader_enablemakefile(self);
}
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM || TPP_MAKEFILE_HAVE_CLI_DASH_MG */

/* Feed an argument to the loader. How exactly the argument is parsed
 * depends on the loader's current state, but sufficed to say: in its
 * default/initial state, `arg` is a CLI argument as you'd expect.
 *
 * WARNING: When you call this function, you must guaranty that `arg` remains
 *          valid, allocated, and unaltered until `tpp_makefile_cli_loader_fini()` is
 *          called.
 *
 * @return: TPP_EOK:       Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:    SOFT_ERROR: Argument could not be understood (but no
 *                         warning was emitted). You must either handle it yourself
 *                         by treating it as an argument for *your* compiler's
 *                         CLI, or as an input file for the lexer, or emit a
 *                         warning informing the user that their CLI argument
 *                         was not understood. You should also probably try to
 *                         pass it to `tpp_cli_loader_parsearg()`.
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_cli_loader_parsearg(tpp_makefile_cli_loader *tpp_restrict self, char const *arg) {
#define tpp_streq(at, CONSTstr) \
	(tpp_memcmp(at, CONSTstr, sizeof(CONSTstr) - sizeof(char)) == 0)
	switch (self->tmfcl_state) {

	case TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL: {
		if (*arg++ != '-')
			break; /* All arguments must start with at least 1 "-" */
		switch (*arg++) {

		case '-':
			/* --foo argument */
			switch (*arg++) {

			case '\0':
				self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_DDASH; /* -- */
				return TPP_EOK;

			case 'd':
#if TPP_MAKEFILE_HAVE_CLI_DASH_M
				if (tpp_streq(arg, "ependencies\0")) { /* --dependencies */
					tpp_makefile_cli_loader_enable_with_makefile_only(self);
					return TPP_EOK;
				} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M */
				{
				}
				break;

			case 'u':
#if TPP_MAKEFILE_HAVE_CLI_DASH_MM
				if (tpp_streq(arg, "ser-dependencies\0")) { /* --user-dependencies */
					tpp_makefile_enablefeature(self->tmfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
					tpp_makefile_cli_loader_enable_with_makefile_only(self);
					return TPP_EOK;
				} else
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MM */
				{
				}
				break;

			case 'p':
#if TPP_MAKEFILE_HAVE_CLI_DASH_MG
				if (tpp_streq(arg, "rint-missing-file-dependencies\0")) { /* --print-missing-file-dependencies */
					tpp_makefile_cli_loader_enable_with_makefile_only(self);
					return tpp_makefile_enable_missing_file_dependencies(self->tmfcl_mf);
				} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MG */
				{
				}
				break;

			default: break;
			}
			break;

		case 'M':
#if TPP_MAKEFILE_HAVE_CLI_DASH_MMD
			if (tpp_streq(arg, "MD\0")) {
				self->tmfcl_flags |= _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT;
				tpp_makefile_enablefeature(self->tmfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
				tpp_makefile_cli_loader_enablemakefile(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MMD */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MD
			if (tpp_streq(arg, "D\0")) {
				self->tmfcl_flags |= _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT;
				tpp_makefile_cli_loader_enablemakefile(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MD */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
			if (*arg == 'F') {
				if (*++arg) {
					self->tmfcl_outfile = arg; /* -MFfoo.m */
				} else {
					self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_MF; /* -MF foo.m */
				}
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MP
			if (tpp_streq(arg, "P\0")) {
				tpp_makefile_enablefeature(self->tmfcl_mf, TPP_MAKEFILE_FEAT_PHONY);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MP */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
			if (*arg == 'T') {
				if (*++arg) {
					tpp_makefile_cli_loader_settarget_mt(self, arg); /* -MTmain.o */
				} else {
					self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_MT; /* -MT main.o */
				}
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
			if (*arg == 'Q') {
				if (*++arg) {
					tpp_makefile_cli_loader_settarget_mq(self, arg); /* -MQmain.o */
				} else {
					self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_MQ; /* -MQ main.o */
				}
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MG
			if (tpp_streq(arg, "G\0")) {
				tpp_makefile_cli_loader_enable_with_makefile_only(self);
				return tpp_makefile_enable_missing_file_dependencies(self->tmfcl_mf);
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MG */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MM
			if (tpp_streq(arg, "M\0")) {
				tpp_makefile_enablefeature(self->tmfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
				tpp_makefile_cli_loader_enable_with_makefile_only(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MM */
#if TPP_MAKEFILE_HAVE_CLI_DASH_M
			if (tpp_streq(arg, "\0")) {
				tpp_makefile_cli_loader_enable_with_makefile_only(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M */
			{
			}
			break;

		default: break;
		}
	}	break;

	case TPP_MAKEFILE_CLI_LOADER_STATE_DDASH:
		break; /* Don't accept any more arguments after having encountered a "--" arguments */

#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
	case TPP_MAKEFILE_CLI_LOADER_STATE_MF:
		self->tmfcl_state   = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL;
		self->tmfcl_outfile = arg;
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */

#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
	case TPP_MAKEFILE_CLI_LOADER_STATE_MT:
		self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL;
		tpp_makefile_cli_loader_settarget_mt(self, arg);
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */

#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
	case TPP_MAKEFILE_CLI_LOADER_STATE_MQ:
		self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL;
		tpp_makefile_cli_loader_settarget_mq(self, arg);
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MQ */

	default: tpp_unreachable();
	}
	return TPP_ENOENT;
#undef tpp_streq
}


/* Try to parse a *flag*-style parameter, that is: an argument that actually consists
 * of multiple, tightly packed parameters, whilst having a singular, leading `-` (that
 * was already skipped by the caller).
 *
 * Example: `-PH` or `-HP`
 * - This argument consists of 2 flags `-H` and `-P`, which are simply concatenated
 *   into a single argument here. This function will then parse one of those flags
 *   from `**p_arg` (iow: `**p_arg` must be one of `H` or `P`), and advance `*p_arg`
 *   to either the end of the argument, or the next *flag*-style parameter.
 *
 * @return: TPP_EOK:    Success (`*p_arg` was updated to point to the next *flag*-style
 *                      parameter, or the argument string's end)
 * @return: TPP_ENOENT: Did not recognize the flag in `**p_arg` (caller should try to
 *                      handle the flag in a different context).
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_cli_loader_parseflag(tpp_makefile_cli_loader *tpp_restrict self, char const **p_arg) {
	char const *arg = *p_arg;
	char flag = *arg++;
	(void)self;
	(void)flag;
	(void)arg;
	switch (flag) {

		/* No flags defined... */

	default: break;
	}
	return TPP_ENOENT;
}



/* Convenience wrapper around `tpp_makefile_cli_loader_parsearg()`.
 * For more information, see `tpp_cli_loader_parseargv()`.
 *
 * @return: TPP_EOK:       Success (`*p_argc` and `*p_argv` were updated such that
 *                         they contain all unrecognized arguments, as well as all
 *                         input files for the lexer).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_makefile_cli_loader_parseargv(tpp_makefile_cli_loader *tpp_restrict self,
                                 int *p_argc, char ***p_argv) {
	tpp_errno result = TPP_EOK;
	int argc    = *p_argc;
	char **argv = *p_argv;
	unsigned int unknown_count = 0;
	while (argc > 0) {
		char *arg = argv[0];
		result = tpp_makefile_cli_loader_parsearg(self, arg);
		if (TPP_ISERR(result)) {
			if (result != TPP_ENOENT)
				break;
			/* Add "arg" to trailing list of unknown arguments */
			--argc;
			tpp_memmovedown(&argv[0], &argv[1],
			                (argc + unknown_count) *
			                sizeof(char *));
			argv[argc + unknown_count] = arg;
			++unknown_count;
			result = TPP_EOK;
			continue;
		}
		if (tpp_makefile_cli_loader_hasddash(self)) {
			if (unknown_count) {
				/* Right now, "argv" looks like this:
				 * >> argv = { "--", "file1.c", "file2.c", "-unknown-arg", "file0.c" }
				 * >> argc = 3
				 * >> unknown_count = 2
				 *
				 * Our job now is to make `argv` look like this:
				 * >> argv = { "-unknown-arg", "file0.c", "--", "file1.c", "file2.c" } */
				unsigned int shift_count = unknown_count;
				unsigned int total_count_minus_1 = argc + unknown_count - 1;
				while (shift_count--) {
					arg = argv[0];
					tpp_memmovedown(&argv[0], &argv[1], total_count_minus_1 * sizeof(char *));
					argv[total_count_minus_1] = arg;
				}
			}
			break;
		}
		++argv;
		--argc;
	}
	*p_argc = argc + unknown_count;
	*p_argv = argv;
	return result;
}

struct tpp_makefile_cli_default_target_data {
	tpp_makefile *tmfcdtd_mf;    /* [1..1][const] Makefile */
	tpp_size      tmfcdtd_count; /* # of printed bytes */
};

static TPP_FORMATPRINTER_DEFINE(tpp_makefile_cli_default_target_printer, arg, text, num_bytes) {
	tpp_size count;
	tpp_ssize result;
	struct tpp_makefile_cli_default_target_data *data;
	data   = (struct tpp_makefile_cli_default_target_data *)arg;
	result = tpp_makefile_escape(tpp_makefile_getoutput(data->tmfcdtd_mf),
	                             data->tmfcdtd_mf, text, num_bytes, &count);
	data->tmfcdtd_count += count;
	return result;
}


/* Ensure that `self` is in a *normal* state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_makefile_cli_loader_getmakefile(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the `#include`-stack.
 *
 * When `tpp_makefile_cli_loader_getonlymakefile(self)` returns true, a call
 * to this function will cause *all* tokens to be consumed (and discarded)
 * from the linked lexer, which is needed to implement the conventional
 * behavior of the `-M` CLI flag. This behavior can however be hard-suppressed
 * by `#define TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE 0`.
 *
 * @param: output_filename: [0..1] Name of the compiler output file (used to generate
 *                                 a fallback dependency filename for `-MD` / `-MMD`)
 *                                 If present, also used as the default name of the
 *                                 target that's written in the makefile (otherwise,
 *                                 that target is derived from `__BASE_FILE__`)
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_cli_loader_flush(tpp_makefile_cli_loader *tpp_restrict self,
                              char const *output_filename) {
	tpp_errno error;
	tpp_ssize output_temp;
	tpp_size output_count;

	/* Emit a warning if the CLI loader isn't in a neutral state */
#if TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT
	if (self->tmfcl_state != TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL &&
	    self->tmfcl_state != TPP_MAKEFILE_CLI_LOADER_STATE_DDASH) {
		error = tpp_makefile_cli_warnf(self->tmfcl_mf, NULL, 0,
		                               TPP_W_MISSING_CLI_ARGUMENT);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT */

	/* Check if Makefile generation is enabled (if it isn't don't do anything else) */
#if TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED
	if (!tpp_makefile_cli_loader_getmakefileenabled(self))
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED */

	/* Turn on the linked makefile */
	error = tpp_makefile_enable(self->tmfcl_mf);
	if (TPP_ISERR(error))
		return error;

	/* Redirect output to a custom file */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
	if (self->tmfcl_outfile) {
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH
		if (tpp_strcmp(self->tmfcl_outfile, "-") == 0) {
#ifdef tpp_makefile_io_getstdout
			tpp_makefile_io_handle handle = tpp_makefile_io_getstdout();
			tpp_makefile_setoutput_io_ex(self->tmfcl_mf, handle, true);
#else /* tpp_makefile_io_getstdout */
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH' is enabled, but no way to retrieve STDOUT handle"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !tpp_makefile_io_getstdout */
		} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH */
		{
			error = tpp_makefile_setoutput_file(self->tmfcl_mf,
			                                    self->tmfcl_outfile);
			if (TPP_ISERR(error))
				return error;
		}
	} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
	{
#if TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD
		if (self->tmfcl_flags & _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT) {
			char const *filename = output_filename;
			char const *filename_end;
			tpp_size filename_len;
			char *output_buf, *ptr;
			/* Auto-determine output filename when -MD or -MMD was used,
			 * based on `output_filename`, or if not given: __BASE_FILE__ */
			if (filename == NULL) {
				/* TODO: This shouldn't be __BASE_FILE__, but should be __FILE__
				 *       (i.e.: the *first* file to get compiled) */
				tpp_file const *bf = tpp_lexer_getbasefile(tpp_makefile_getlexer(self->tmfcl_mf));
				filename = tpp_file_getrealfilename(bf);
				if (filename == NULL)
					filename = ""; /* Unknown filename :( */
			}
			filename_end = filename;
			filename_end += tpp_strlen(filename);
			for (;;) {
				char ch;
				if (filename_end <= filename) {
use_full_filename:
					filename_end += tpp_strlen(filename_end);
					break;
				}
				ch = *--filename_end;
				if (TPP_FS_ISSEP(ch))
					goto use_full_filename;
				if (ch == '.')
					break;
			}
			filename_len = (tpp_size)(filename_end - filename);
			output_buf = (char *)tpp_malloc((filename_len * sizeof(char)) +
			                                sizeof(TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION));
			if tpp_unlikely(!output_buf)
				return TPP_ENOMEM;
			ptr = (char *)tpp_mempcpy(output_buf, filename, filename_len * sizeof(char));
			tpp_memcpy(ptr, TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION,
			           sizeof(TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION));
			/* Use this one as filename */
			error = tpp_makefile_setoutput_file(self->tmfcl_mf, output_buf);
			tpp_free(output_buf);
			if (TPP_ISERR(error))
				return error;
		}
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD */
	}

	/* Print makefile target */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ
	if (self->tmfcl_target) {
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT && TPP_MAKEFILE_HAVE_CLI_DASH_MQ
		if (self->tmfcl_flags & _TPP_MAKEFILE_CLI_LOADER_FLAG_TARGETESCAPE) {
			output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self->tmfcl_mf), self->tmfcl_mf,
			                                  (tpp_char const *)self->tmfcl_target,
			                                  tpp_strlen(self->tmfcl_target),
			                                  &output_count);
		} else {
			output_count = tpp_strlen(self->tmfcl_target);
			output_temp = tpp_makefile_output_printraw_cstr(self->tmfcl_mf, self->tmfcl_target, output_count);
		}
#elif TPP_MAKEFILE_HAVE_CLI_DASH_MT
		output_count = tpp_strlen(self->tmfcl_target);
		output_temp = tpp_makefile_output_printraw_cstr(self->tmfcl_mf, self->tmfcl_target, output_count);
#else /* ... */
		output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self->tmfcl_mf), self->tmfcl_mf,
		                                  (tpp_char const *)self->tmfcl_target,
		                                  tpp_strlen(self->tmfcl_target), &output_count);
#endif /* !... */
	} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
	if (output_filename) {
		output_count = tpp_strlen(output_filename);
		output_temp = tpp_makefile_output_printraw_cstr(self->tmfcl_mf, output_filename, output_count);
	} else {
		/* Auto-determine target name based on __BASE_FILE__ */
		/* TODO: This shouldn't be __BASE_FILE__, but should be __FILE__
		 *       (i.e.: the *first* file to get compiled) */
		tpp_file const *bf = tpp_lexer_getbasefile(tpp_makefile_getlexer(self->tmfcl_mf));
		char const *bf_filename = tpp_file_getrealfilename(bf);
		if tpp_unlikely(bf_filename == NULL) {
			output_count = 0;
			output_temp  = 0;
		} else {
			struct tpp_makefile_cli_default_target_data data;
			data.tmfcdtd_count = 0;
			data.tmfcdtd_mf    = self->tmfcl_mf;
			output_temp = tpp_makefile_cli_print_default_target(self, &tpp_makefile_cli_default_target_printer,
			                                                    &data, bf_filename);
			output_count = data.tmfcdtd_count;
		}
	}
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);
	(void)output_count;
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	self->tmfcl_mf->tmf_curcol = (tpp_column)output_count;
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

	/* Print the trailing `:` following the target name */
	output_temp = tpp_makefile_output_printraw_conststr(self->tmfcl_mf, ":");
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	self->tmfcl_mf->tmf_curcol += 1;
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

	/* TODO: Emit dependencies for every (distinct) file currently
	 *       on the #include-stack (since those won't appear as
	 *       "new" dependencies anymore after this point!) */

	/* Consume all tokens from the lexer (if enabled) */
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
	if (tpp_makefile_cli_loader_getonlymakefile(self)) {
		tpp_token_id tok;
		tpp_lexer *const lexer = tpp_makefile_getlexer(self->tmfcl_mf);
		do {
			tok = tpp_lexer_yield(lexer);
		} while (!TPP_TOK_ISERR_OR_EOF(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}
#endif /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */

	return TPP_EOK;
}

#if TPP_MAKEFILE_HAVE_CLI_HELP
#undef TPP_CLI_HELP1
#undef TPP_CLI_HELP2
#define TPP_CLI_HELP1(spelling, description) \
	spelling "\0\0" description "\0"
#if TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS
#define TPP_CLI_HELP2(spelling1, spelling2, description) \
	spelling1 "\0" spelling2 "\0\0" description "\0"
#else /* TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS */
#define TPP_CLI_HELP2(spelling1, spelling2, description) \
	TPP_CLI_HELP1(spelling1, description)
#endif /* !TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS */

/* Returns supported CLI parameters, and human-readable information
 * for them. Same format as `tpp_cli_loader_help` (see for more info) */
TPP_CONST_IMPL char const tpp_makefile_cli_loader_help[] =
#if TPP_MAKEFILE_HAVE_CLI_DASH_M
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
TPP_CLI_HELP2("-M", "--dependencies", "Discard preprocessor output and generate a Makefile")
#else /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
TPP_CLI_HELP2("-M", "--dependencies", "Generate a Makefile")
#endif /* !TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MM
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
TPP_CLI_HELP2("-MM", "--user-dependencies",
              "Discard preprocessor output and generate a Makefile\n"
              "Omit dependencies from #pragma GCC system_header files")
#else /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
TPP_CLI_HELP2("-MM", "--user-dependencies",
              "Generate a Makefile\n"
              "Omit dependencies from #pragma GCC system_header files")
#endif /* !TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MM */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
TPP_CLI_HELP1("-MF FILE", "Write Makefile output to FILE")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
TPP_CLI_HELP1("-MT TARGET", "Use TARGET in Makefile output")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
TPP_CLI_HELP1("-MQ TARGET", "Use TARGET in Makefile output, but escape it first")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MG
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
TPP_CLI_HELP2("-MG", "--print-missing-file-dependencies",
              "Discard preprocessor output and generate a Makefile\n"
              "Also include missing #include-s in the Makefile")
#else /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
TPP_CLI_HELP2("-MG", "--print-missing-file-dependencies",
              "Generate a Makefile\n"
              "Also include missing #include-s in the Makefile")
#endif /* !TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MG */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MP
TPP_CLI_HELP1("-MP", "All dependencies are repeated as phony/dummy targets")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MP */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MD
TPP_CLI_HELP1("-MD", "Generate a Makefile\n"
                     "The output filename is determined automatically")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MD */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MMD
TPP_CLI_HELP1("-MMD", "Generate a Makefile\n"
                      "The output filename is determined automatically\n"
                      "Omit dependencies from #pragma GCC system_header files")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MMD */
"";

#undef TPP_CLI_HELP1
#undef TPP_CLI_HELP2
#endif /* TPP_MAKEFILE_HAVE_CLI_HELP */


/* Print the default target name of `input_filename`, when no explicit output is known.
 * The default implementation of this function is affected by the following configs:
 * - `TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX`
 * - `TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY`
 * - `TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX`
 * - `TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY`
 * - `TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION`
 *
 * If the desired behavior cannot be achieved using only those function, it is recommended
 * to fully override this function (which can be done by pre-defining a macro with the same
 * name as this function before `#include`-ing TPP's **MAKEFILE** source extension), and
 * then implementing that function such that it behaves as desired.
 *
 * @param: self:           The CLI loader (can be used to gain access to the linked makefile/lexer)
 * @param: printer:        Output printer that the (unescaped) object filename should be printed to
 * @param: input_filename: The `__BASE_FILE__` filename of the linked lexer
 * @return: * : Sum of return values of `printer` */
#ifndef tpp_makefile_cli_print_default_target
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 4)) tpp_ssize TPPCALL
tpp_makefile_cli_print_default_target(tpp_makefile_cli_loader *tpp_restrict self,
                                      tpp_formatprinter printer, void *arg,
                                      char const *input_filename) {
	/* Auto-determine target name based on __BASE_FILE__ */
	tpp_ssize temp, result = 0;
	char const *iter, *after_last_sep, *before_last_dot;
	(void)self;

	iter = after_last_sep = input_filename;
	for (;;) {
		char ch = *iter++;
		if (!ch)
			break;
		if (TPP_FS_ISSEP(ch))
			after_last_sep = iter;
	}
	before_last_dot = iter - 1;
	tpp_assert(*before_last_dot == '\0');
	iter = after_last_sep;
	for (; *iter; ++iter) {
		if (*iter == '.')
			before_last_dot = iter;
	}

	/* Print leading path prefix */
#if !TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY
	temp = tpp_formatprinter_print_conststr(printer, arg, TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX);
	if (temp < 0)
		goto err_temp;
	result += temp;
#endif /* !TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY */

	/* Print directory-heading, plus everything up to (but excluding)
	 * the final '.' preceding the extension. When configured as such,
	 * also print `TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX`. */
#if !TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY
	temp = tpp_formatprinter_print_cstr(printer, arg, input_filename,
	                                    (tpp_size)(after_last_sep - input_filename));
	if (temp < 0)
		goto err_temp;
	result += temp;
	temp = tpp_formatprinter_print_conststr(printer, arg, TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX);
	if (temp < 0)
		goto err_temp;
	result += temp;
	temp = tpp_formatprinter_print_cstr(printer, arg, after_last_sep,
	                                    (tpp_size)(before_last_dot - after_last_sep));
	if (temp < 0)
		goto err_temp;
	result += temp;
#else /* !TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY */
	temp = tpp_formatprinter_print_cstr(printer, arg, input_filename,
	                                    (tpp_size)(before_last_dot - input_filename));
	if (temp < 0)
		goto err_temp;
	result += temp;
#endif /* !TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY */

	/* Print the target's default object-file extension. */
	temp = tpp_formatprinter_print_conststr(printer, arg, TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION);
	if (temp < 0)
		goto err_temp;
	result += temp;
	return result;
err_temp:
	return temp;
}
#endif /* !tpp_makefile_cli_print_default_target */

#endif /* TPP_MAKEFILE_HAVE_CLI */

TPP_DECL_END

#endif /* !GUARD_TPP_MAKEFILE_AMALGAMATION_C */
/* clang-format on */
