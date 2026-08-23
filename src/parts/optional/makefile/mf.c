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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_MF_C
#define GUARD_TPP_OPTIONAL_MAKEFILE_MF_C 1
#define TPP_BUILDING_OPTIONAL 1
#define TPP_MAKEFILE_BUILDING 1

#include "api.h"

#include "config.h"
#include "mf.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

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

#if TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO
TPP_FORMATPRINTER_IMPL(_tpp_makefile_builtin_file_output, arg, text, num_bytes) {
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
		for (i = 0; i < self->tmkf_depc; ++i) {
			tpp_keyword const *dep = self->tmkf_depv[i];
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


#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) || TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
#if TPP_MAKEFILE_HAVE_PHONY || TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_adddep(tpp_makefile *tpp_restrict self,
                    tpp_keyword const *tpp_restrict dep) {
	tpp_assert(self->tmkf_depc <= self->tmkf_depa);
	if (self->tmkf_depc >= self->tmkf_depa) {
		tpp_keyword const **new_depv;
		tpp_size new_depa = self->tmkf_depa * 2;
		if (new_depa < 8)
			new_depa = 8;
		new_depv = (tpp_keyword const **)tpp_tryrealloc(self->tmkf_depv,
		                                                new_depa *
		                                                sizeof(tpp_keyword const *));
		if tpp_unlikely(!new_depv) {
			new_depa = self->tmkf_depc + 1;
			new_depv = (tpp_keyword const **)tpp_realloc(self->tmkf_depv,
			                                             new_depa *
			                                             sizeof(tpp_keyword const *));
			if tpp_unlikely(!new_depv)
				return TPP_ENOMEM;
		}
		self->tmkf_depa = new_depa;
		self->tmkf_depv = new_depv;
	}
	self->tmkf_depv[self->tmkf_depc++] = dep;
	return TPP_EOK;
}

#if TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
static TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_makefile_hasdep(tpp_makefile *tpp_restrict self,
                    tpp_keyword const *tpp_restrict dep) {
	tpp_size i = self->tmkf_depc;
	while (i--) {
		tpp_keyword const *mydep = self->tmkf_depv[i];
		if (tpp_keyword_equals(dep, mydep))
			return true;
	}
	return false;
}
#endif /* TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */
#endif /* TPP_MAKEFILE_HAVE_PHONY || TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */


#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
TPP_FORMATPRINTER_DEFINE(tpp_makefile_strlen_printer, arg, text, num_bytes) {
	(void)arg;
	(void)text;
	return (tpp_ssize)num_bytes;
}
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

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
	if (self->tmkf_curcol > 0) {
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH < 0
		if (tpp_makefile_getmaxcol(self) >= 0)
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH < 0 */
		{
			tpp_size output_len = 1; /* +1 for leading ' '-character */
			output_len += (tpp_size)tpp_makefile_escape(tpp_formatprinter_of(tpp_makefile_strlen_printer),
			                                            NULL, filename, filename_len,
			                                            NULL);
			if ((self->tmkf_curcol + (tpp_column)output_len) >= tpp_makefile_getmaxcol(self)) {
				/* Force a line-wrap */
				output_temp = tpp_makefile_output_printraw_conststr(self, " \\\n");
				if (output_temp < 0)
					return TPP_SSIZE_ASERR(output_temp);
				self->tmkf_curcol = 0;
			}
		}
	}
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

	/* Print the dependency filename (following a preceding space) */
	output_temp = tpp_makefile_output_printraw_conststr(self, " ");
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	self->tmkf_curcol += 1;
	{
		tpp_size count;
		output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self),
		                                  self, filename, filename_len,
		                                  &count);
		if (output_temp < 0)
			return TPP_SSIZE_ASERR(output_temp);
		self->tmkf_curcol += (tpp_column)count;
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
#endif /* ... */

/* The main (mandatory) `NEW_DEPENDECY` hook that's used to
 * get notified whenever the lexer encounters a new dependency */
#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_makefile_new_dependency_hook(_tpp_makefile_new_dependency_hook_cookie cookie,
                                  tpp_keyword *filename_kwd) {
	tpp_makefile *const self = _tpp_makefile_new_dependency_hook_ofcookie(cookie);
	return tpp_makefile_new_dependency_hook_impl(self, filename_kwd);
}
#endif /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

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
_tpp_makefile_include_not_found_hook(_tpp_makefile_include_not_found_hook_cookie cookie,
                                     tpp_hook_include_kind include_kind) {
	tpp_makefile *const self = _tpp_makefile_include_not_found_hook_ofcookie(cookie);
	tpp_lexer const *const lexer = tpp_makefile_getlexer(self);
	(void)include_kind; /* Ignored -- treat all missing includes the same. */

	/* NOTE: The `TPP_EOK` return value of `tpp_lexer_decode_include_string_cb()`
	 *       also acts to suppresses the `TPP_W_NO_SUCH_FILE` that would normally
	 *       be raised by the caller */
	return tpp_lexer_decode_include_string_cb(lexer, &tpp_makefile_include_not_found_cb, self);
}
#endif /* TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_C */
