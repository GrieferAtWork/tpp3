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
	return temp;
}

#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
static TPP_FORMATPRINTER_DEFINE(tpp_makefile_strlen_printer, arg, text, num_bytes) {
	(void)arg;
	(void)text;
	return (tpp_ssize)num_bytes;
}
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

#if TPP_MAKEFILE_HAVE_PHONY
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
#endif /* TPP_MAKEFILE_HAVE_PHONY */


#if TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO
TPP_IMPL TPP_FORMATPRINTER_DEFINE(_tpp_makefile_builtin_file_output, arg, text, num_bytes) {
	tpp_makefile const *const self = (tpp_makefile const *)arg;
	tpp_makefile_io_handle const file = tpp_makefile_getoutput_io(self);
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

	/* Re-emit all dependencies as their own, phony targets.
	 *
	 * Note that dependencies were only added if `tpp_makefile_has(self, PHONY)`,
	 * meaning that we can simply dump everything from within here, knowing that
	 * (in case the state of that config changed at some point, or might even be
	 * disabled right now), anything in here was added at a point in time when
	 * phony target tracking was enabled (and thus: should get (re-)emitted). */
#if TPP_MAKEFILE_HAVE_PHONY
	{
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



/* The main (mandatory) `NEW_DEPENDECY` hook that's used to
 * get notified whenever the lexer encounters a new dependency */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_makefile_new_dependency_hook(tpp_hook_cookie cookie, tpp_keyword *filename_kwd) {
	tpp_ssize output_temp;
	tpp_char const *const filename = tpp_keyword_getstr(filename_kwd);
	tpp_size const filename_len = tpp_keyword_getlen(filename_kwd);
	tpp_makefile *const self = tpp_makefile_ofcookie(cookie);

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

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_C */
