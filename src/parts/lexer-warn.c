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
#ifndef GUARD_TPP_LEXER_WARN_C
#define GUARD_TPP_LEXER_WARN_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "features.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "macro.h"
#include "token.h"
#include "warnings.h"

/*[[[tpp-begin]]]*/
#ifndef TPP_NO_SYSTEM_INCLUDES
#if TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
#include <stdio.h>
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */
#endif /* !TPP_NO_SYSTEM_INCLUDES */

TPP_DECL_BEGIN

#if TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER
TPP_IMPL tpp_ssize TPP_FORMATPRINTER_CC
_tpp_lexer_wrapped_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes) {
	(void)arg;
	return TPP_CONFIG_WARNPRINTER(text, num_bytes);
}
#endif /* TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER */

#if TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
TPP_IMPL tpp_ssize TPP_FORMATPRINTER_CC
_tpp_lexer_builtin_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes) {
	FILE *fp = stderr;
	(void)arg;
	fwrite(text, sizeof(tpp_char), num_bytes, fp);
	return ferror(fp) ? -1 : 0;
}
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */

#if TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER
TPP_IMPL tpp_ssize TPP_FORMATPRINTER_CC
_tpp_lexer_noop_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes) {
	(void)arg;
	(void)text;
	(void)num_bytes;
	return 0;
}
#endif /* TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER */



#if TPP_HAVE_WARNINGS
/* Interpret + print a warning-message "format" string.
 * The following %-encoded escape sequences are recognized:
 * - "%["    Start quoting text
 * - "%]"    Stop quoting text
 * - "%Pl"   1-based line of "pos" in "file"
 * - "%Pc"   1-based column of "pos" in "file"
 * - "%Pf"   Filename of given "file"
 * - "%Pt"   "%[current-token%]"
 * - "%s"    As defined by stdc, using va_arg(args, char *)
 * - "%.*s"  As defined by stdc, using va_arg(args, int) + va_arg(args, char *)
 * - "%.Ns"  As defined by stdc, using va_arg(args, char *)
 * - "%.NPt" "%[<N bytes starting at "pos">%]"
 * - "%d"    As defined by stdc, using va_arg(args, int)
 * - "%u"    As defined by stdc, using va_arg(args, unsigned int)
 * - "%c"    As defined by stdc, using va_arg(args, int)
 * - "%%"    "%" (emit a singular %-character)
 *
 * @param: pos:        [0..1] Lexer position used by certain format-patterns.
 * @param: pos_lcinfo: Lexer position used when "pos == NULL"
 * @param: file:       The file containing "pos"
 * @param: printer:    Output printer for formatted text
 * @param: arg:        Cookie argument for "printer"
 * @param: format:     Format pattern (see above)
 * @param: args:       Extra varargs-arguments for "format"
 * @return: >= 0:      Sum of return values of "printer".
 * @return: < 0:       First negative return value of "printer". The more high-level
 *                     "tpp_lexer_warnf" API returns "TPP_EWARNPRINT" in this case. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 5, 7)) tpp_ssize TPPVCALL
tpp_lexer_printf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                         tpp_lcinfo pos_lcinfo, tpp_formatprinter printer, void *arg,
                         char const *format, ...) {
	tpp_ssize result;
	va_list args;
	va_start(args, format);
	result = tpp_lexer_vprintf_warning(self, file, pos, pos_lcinfo,
	                                   printer, arg, format, args);
	va_end(args);
	return result;
}


static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_print_uint(tpp_formatprinter printer, void *arg, tpp_uintmax value) {
	char buf[TPP_UTOA_MAXLEN];
	char const *start = tpp_utoa(buf, value);
	return (*printer)(arg, (tpp_char const *)start, (tpp_size)((buf + tpp_lengthof(buf)) - start));
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_print_int(tpp_formatprinter printer, void *arg, tpp_intmax value) {
	char buf[TPP_ITOA_MAXLEN];
	char const *start = tpp_itoa(buf, value);
	return (*printer)(arg, (tpp_char const *)start, (tpp_size)((buf + tpp_lengthof(buf)) - start));
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_quote_start(tpp_formatprinter printer, void *arg) {
	/* TODO: Do something more interesting here! */
	return (*printer)(arg, (tpp_char const *)"`", 1);
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_quote_end(tpp_formatprinter printer, void *arg) {
	/* TODO: Do something more interesting here! */
	return (*printer)(arg, (tpp_char const *)"`", 1);
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_token_data(tpp_formatprinter printer, void *arg,
                      tpp_char const *start, tpp_size length) {
	/* TODO: Escape line-feeds while printing token body */
	return (*printer)(arg, start, length);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 5, 7)) tpp_ssize TPPCALL
tpp_lexer_vprintf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                          tpp_lcinfo pos_lcinfo, tpp_formatprinter printer, void *arg,
                          char const *format, va_list args) {
	bool lcinfo_loaded = pos == NULL;
	static char const null_str[] = "(null)";
	tpp_ssize temp, result = 0;
	char const *iter = format;
	char ch;
again:
	ch = *iter++;
	if (ch != '%') {
		if (ch == '\0') {
handle_eof:
			--iter;
			temp = (*printer)(arg, (tpp_char const *)format,
			                  (tpp_size)(iter - format));
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			return result;
		}
		goto again;
	}
	if ((iter - 1) > format) {
		temp = (*printer)(arg, (tpp_char const *)format,
		                  (tpp_size)((iter - 1) - format));
		if tpp_unlikely(temp < 0)
			goto err_temp;
		result += temp;
	}
	ch = *iter++;
	switch (ch) {
	case '\0':
		goto handle_eof;
	case '%':
		format = iter - 1;
		goto again;

	case '[': /* "%["    Start quoting text */
		temp = tpp_format_quote_start(printer, arg);
		break;

	case ']': /* "%]"    Stop quoting text */
		temp = tpp_format_quote_end(printer, arg);
		break;

	case 'P':
		ch = *iter++;
		switch (ch) {

		case 'l': /* "%Pl"   1-based line of "pos" in "file" */
		case 'c': /* "%Pc"   1-based column of "pos" in "file" */
		{
			/* Lazily load line/column information (if not already loaded) */
			if (!lcinfo_loaded) {
				pos_lcinfo = tpp_file_lcinfo(file, pos);
				lcinfo_loaded = true;
			}
			temp = tpp_format_print_int(printer, arg,
			                            ch == 'l' ? (tpp_lcinfo_getline(pos_lcinfo) + 1)
			                                      : (tpp_lcinfo_getcol(pos_lcinfo) + 1));
		}	break;

		case 'f': {
			/* "%Pf"   Filename of given "file" */
			char const *filename = tpp_file_userfilename(file);
			if (filename == NULL)
				filename = "?";
			temp = (*printer)(arg, (tpp_char const *)filename, tpp_strlen(filename));
		}	break;

		case 't': {
			/* "%Pt"   "%[current-token%]" */
			tpp_token const *const token = tpp_lexer_gettoken(self);
			temp = tpp_format_quote_start(printer, arg);
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			temp = tpp_format_token_data(printer, arg, token->tt_start,
			                             (tpp_size)(token->tt_end - token->tt_start));
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			temp = tpp_format_quote_end(printer, arg);
		}	break;

		default:
			format = iter - 3;
			goto again;
		}
		break;

	case 's': {
		/* "%s"    As defined by stdc, using va_arg(args, char *) */
		char const *s = va_arg(args, char const *);
		if (s == NULL)
			s = null_str;
		temp = (*printer)(arg, (tpp_char const *)s, tpp_strlen(s));
	}	break;

	case '.': {
		ch = *iter++;
		switch (ch) {

		case '*': {
			unsigned int length = va_arg(args, unsigned int);
			ch = *iter++;
			switch (ch) {

			case 's': {
				/* "%.*s"  As defined by stdc, using va_arg(args, int) + va_arg(args, char *) */
				char const *s = va_arg(args, char const *);
				if (s == NULL)
					s = null_str;
				temp = (*printer)(arg, (tpp_char const *)s, (tpp_size)length);
			}	break;

			default:
				format = iter - 4;
				goto again;
			}
		}	break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			char const *block_start = iter - 3;
			unsigned int length = (unsigned int)(ch - '0');
			for (;;) {
				ch = *iter++;
				if (ch >= '0' && ch <= '9') {
					length *= 10;
					length += (unsigned int)(ch - '0');
				} else {
					break;
				}
			}
			switch (ch) {
			case 's': {
				/* "%.Ns"  As defined by stdc, using va_arg(args, char *) */
				char const *s = va_arg(args, char const *);
				if (s == NULL)
					s = null_str;
				temp = (*printer)(arg, (tpp_char const *)s, (tpp_size)length);
			}	break;

			case 'P': {
				ch = *iter++;
				switch (ch) {

				case 't': {
					/* "%.NPt" "%[<N bytes starting at "pos">%]" */
					temp = tpp_format_quote_start(printer, arg);
					if tpp_unlikely(temp < 0)
						goto err_temp;
					result += temp;
					if (pos >= file->tf_end) {
						length = 0;
					} else {
						tpp_size maxlen = (tpp_size)(file->tf_end - pos);
						if ((tpp_size)length > maxlen)
							length = (unsigned int)maxlen;
					}
					temp = tpp_format_token_data(printer, arg, pos, (tpp_size)length);
					if tpp_unlikely(temp < 0)
						goto err_temp;
					result += temp;
					temp = tpp_format_quote_end(printer, arg);
				}	break;

				default:
					format = block_start;
					goto again;
				}
			}	break;

			default:
				format = block_start;
				goto again;
			}
		}	break;

		default:
			format = iter - 3;
			goto again;
		}

	}	break;

	case 'd': {
		/* "%d"    As defined by stdc, using va_arg(args, int) */
		int value = va_arg(args, int);
		temp = tpp_format_print_int(printer, arg, value);
	}	break;

	case 'u': {
		/* "%u"    As defined by stdc, using va_arg(args, unsigned int) */
		unsigned int value = va_arg(args, unsigned int);
		temp = tpp_format_print_uint(printer, arg, value);
	}	break;

	case 'c': {
		/* "%c"    As defined by stdc, using va_arg(args, int) */
		tpp_char ord = (tpp_char)va_arg(args, unsigned int);
		temp = (*printer)(arg, &ord, 1);
	}	break;

	default:
		format = iter - 2;
		goto again;
	}
	if tpp_unlikely(temp < 0)
		goto err_temp;
	result += temp;
	format = iter;
	goto again;
err_temp:
	return temp;
}

static char const tpp_file_and_line[] = TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT;

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_impl(tpp_lexer *tpp_restrict self, tpp_char const *pos,
                      tpp_lcinfo pos_lcinfo, tpp_warning_id id, va_list args) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno result;
	tpp_ssize printer_status;
	char const *warning_format;
	tpp_warning_invokeinfo invokeinfo;
	tpp_formatprinter printer;
	void *printer_arg;

	/* Quick check: are warnings disabled? */
	if (self->tl_state & TPP_LEXER_STATE_FLAG_NOWARNINGS)
		return TPP_EOK;

	/* Ask warning configuration how we should have this one */
	result = tpp_lexer_invokewarning(self, id, &invokeinfo);
#if TPP_HAVE_WARNINGS_INVOKE_MAYFAIL
	if (TPP_ISERR(result))
		goto done;
#endif /* TPP_HAVE_WARNINGS_INVOKE_MAYFAIL */

	/* Deal with certain warning states. */
	switch (invokeinfo.twii_state) {

	case TPP_WSTATE_DISABLED:
		goto done; /* Nothing to do here */

	case TPP_WSTATE_WARN: {
		/* Display as a warning */
#if TPP_HAVE_FILE_SYSHDR
		tpp_file const *const iofile = tpp_file_getiofile(file);
		if (iofile->tf_kind == TPP_FILE_KIND_IO &&
		    iofile->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_SYSHDR)
			return TPP_EOK; /* Suppress warnings in this file */
#endif /* TPP_HAVE_FILE_SYSHDR */
	}	break;

#if TPP_HAVE_WARNING_ERROR
	case TPP_WSTATE_ERROR: {
		tpp_size errors = tpp_lexer_geterrorcount(self);
		tpp_lexer_seterrorcount(self, errors + 1);
		if (errors >= tpp_lexer_geterrorlimit(self))
			result = TPP_ELEXERROR;
	}	break;
#endif /* TPP_HAVE_WARNING_ERROR */

	case TPP_WSTATE_FATAL:
		result = TPP_ELEXERROR;
		break;

	default: tpp_unreachable();
	}

	printer     = tpp_lexer_getwarnprinter(self);
	printer_arg = tpp_lexer_getwarnprinterarg(self);

	/* Print file-and-line prefix */
	printer_status = tpp_lexer_printf_warning(self, file, pos, pos_lcinfo,
	                                          printer, printer_arg,
	                                          tpp_file_and_line);
	if (printer_status < 0)
		goto err_printer;

	/* Print what this is about... */
	if (invokeinfo.twii_state == TPP_WSTATE_WARN) {
		printer_status = (*printer)(printer_arg, (tpp_char const *)"warning[", 8);
	} else {
		printer_status = (*printer)(printer_arg, (tpp_char const *)"error[", 6);
	}
	if (printer_status < 0)
		goto err_printer;

	/* Print the relevant context name. */
#if TPP_HAVE_WARNING_NUMBERS
	if (tpp_warning_context_id_isnumber(invokeinfo.twii_ctx_id)) {
		tpp_warning_id ctx_wid = tpp_warning_context_id_aswarning(invokeinfo.twii_ctx_id);
		unsigned int number = tpp_warning_getnumbers(ctx_wid)[0];
		if tpp_unlikely(number == TPP_WARNING_NUMBER_INVALID) {
			printer_status = (*printer)(printer_arg, (tpp_char const *)"?", 1);
		} else {
			printer_status = tpp_format_print_uint(printer, printer_arg, number);
		}
	} else
#endif /* TPP_HAVE_WARNING_NUMBERS */
	{
		tpp_warning_group_id group_id = tpp_warning_context_id_asgroup(invokeinfo.twii_ctx_id);
		char const *group_name = tpp_warning_group_getnames(group_id);
		if tpp_unlikely(group_name == NULL) {
			printer_status = (*printer)(printer_arg, (tpp_char const *)"?", 1);
		} else {
			printer_status = (*printer)(printer_arg, (tpp_char const *)"-W", 2);
			if (printer_status < 0)
				goto err_printer;
			printer_status = (*printer)(printer_arg, (tpp_char const *)group_name, tpp_strlen(group_name));
		}
	}
	if (printer_status < 0)
		goto err_printer;
	printer_status = (*printer)(printer_arg, (tpp_char const *)"]: ", 3);
	if (printer_status < 0)
		goto err_printer;


	/* Lookup the generic warning-message format for "id" */
	warning_format = tpp_warning_getformat(id);
	if (warning_format) {
		printer_status = tpp_lexer_vprintf_warning(self, file, pos, pos_lcinfo,
		                                           printer, printer_arg,
		                                           warning_format, args);
		if (printer_status < 0)
			goto err_printer;
		printer_status = (*printer)(printer_arg, (tpp_char const *)"\n", 1);
		if (printer_status < 0)
			goto err_printer;
	} else {
		/* In this case, "id" must be using a custom warning message expression! */
		switch (id) {

/************************************************************************/
/* MACROS FOR USE BY "TPP_WARNING_EX"                                   */
/************************************************************************/
#define tpp_do(expr)                       \
	do {                                   \
		if ((printer_status = (expr)) < 0) \
			goto err_printer;              \
	} while (0)
#define tpp_print_file_and_line(at_file, at_pos, at_lc)           \
	tpp_do(tpp_lexer_printf_warning(self, at_file, at_pos, at_lc, \
	                                printer, printer_arg,         \
	                                tpp_file_and_line))
#define tpp_print_file_and_line_at(at_file, at_pos) tpp_print_file_and_line(at_file, at_pos, tpp_lcinfo_of(-1, -1))
#define tpp_print_file_and_line_lc(at_file, at_lc)  tpp_print_file_and_line(at_file, NULL, at_lc)
#define tpp_warnf0(format)                 tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format))
#define tpp_warnf1(format, a)              tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format, a))
#define tpp_warnf2(format, a, b)           tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format, a, b))
#define tpp_warnf3(format, a, b, c)        tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format, a, b, c))
#define tpp_warnf4(format, a, b, c, d)     tpp_do(tpp_lexer_printf_warning(self, file, pos, pos_lcinfo, printer, printer_arg, format, a, b, c, d))
#define tpp_va_arg(T) va_arg(args, T)
/* ... */
/************************************************************************/

#define TPP_DEFS
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr) \
		case warning_id: {                                                     \
			expr;                                                              \
		}	break;
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_WARNING_NUMBER_CASE
#undef TPP_DEFS

/************************************************************************/
#undef tpp_do
#undef tpp_print_file_and_line
#undef tpp_print_file_and_line_at
#undef tpp_print_file_and_line_lc
#undef tpp_warnf0
#undef tpp_warnf1
#undef tpp_warnf2
#undef tpp_warnf3
#undef tpp_warnf4
/************************************************************************/
		default:
			printer_status = (*printer)(printer_arg, (tpp_char const *)"UNKNOWN WARNING\n", 16);
			if (printer_status < 0)
				goto err_printer;
			break;
		}
	}

	/* Print origin traceback */
#if TPP_HAVE_INCLUDE_STACK
	{
		tpp_file *caller = file->tf_tprev;
		for (; caller; caller = caller->tf_tprev) {
			/* XXX: We could also display a range here:
			 * >> [caller->tf_tpos, caller->tf_pos)
			 *
			 * This range of bytes describes the "expression" that caused
			 * the include/macro-expansion to happen (it is either the range
			 * from the start of a macro's name, to the end of its parameter
			 * list, or the start of a #include-directive, to the trailing
			 * line-feed) */
			printer_status = tpp_lexer_printf_warning(self, caller, caller->tf_tpos,
			                                          tpp_lcinfo_of(-1, -1),
			                                          printer, printer_arg,
			                                          tpp_file_and_line);
			if (printer_status < 0)
				goto err_printer;
			printer_status = (*printer)(printer_arg, (tpp_char const *)"note: originating from here\n", 28);
			if (printer_status < 0)
				goto err_printer;
		}
	}
#endif /* TPP_HAVE_INCLUDE_STACK */

done:
#ifdef TPP_CONFIG_RAISE_LEXERROR
	if (result == TPP_ELEXERROR) {
		/* Allow users to define a hook to do additional
		 * processing when a lexer error should be raised */
		TPP_CONFIG_RAISE_LEXERROR;
	}
#endif /* TPP_CONFIG_RAISE_LEXERROR */
	return result;
err_printer:
	return TPP_EWARNPRINT;
}



/* Emits the specified lexer warning at the start of the current token.
 * @param: args: Format arguments specific to "id" (see '%'-sequences in warning expressions)
 * @return: TPP_EOK:        Warning was emitted, but you may proceed
 * @return: TPP_ELEXERROR:  Warning was emitted, but was configured as an error
 * @return: TPP_EWARNPRINT: Error while printing warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, va_list args) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	return tpp_lexer_vwarnf_at(self, token->tt_start, id, args);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf(self, id, args);
	va_end(args);
	return result;
}


TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_vwarnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos,
                    tpp_warning_id id, va_list args) {
	return tpp_lexer_vwarnf_impl(self, pos, tpp_lcinfo_of(-1, -1), id, args);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf_impl(self, pos, tpp_lcinfo_of(-1, -1), id, args);
	va_end(args);
	return result;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_lc(tpp_lexer *tpp_restrict self, tpp_lcinfo lc,
                    tpp_warning_id id, va_list args) {
	return tpp_lexer_vwarnf_impl(self, NULL, lc, id, args);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf_lc(tpp_lexer *tpp_restrict self, tpp_lcinfo lc, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf_impl(self, NULL, lc, id, args);
	va_end(args);
	return result;
}
#endif /* TPP_HAVE_WARNINGS */


/* Warn if the current file's #ifdef-stack is non-empty.
 * @return: * : See `tpp_lexer_warnf()' */
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_warn_nonempty_ifdef(tpp_lexer *tpp_restrict self) {
	tpp_errno result = TPP_EOK;
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_size count = file->tf_ifdef.tids_cnt;
	while (count) {
		tpp_ifdef_stack_entry const *entry;
		--count;
		entry = &file->tf_ifdef.tids_vec[count];
		result = tpp_lexer_warnf_lc(self, entry->tidse_updated, TPP_W_EOF_BEFORE_ENDIF);
		if (TPP_ISERR(result))
			break;
	}
	return result;
}
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_WARN_C */
