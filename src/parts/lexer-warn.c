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
#include "hooks.h"
#include "keyword.h"
#include "lexer.h"
#include "macro.h"
#include "token.h"
#include "warnings.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if !TPP_HOST_NO_SYSTEM_INCLUDES && (TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK)
TPP_DECL_END
#include <stdio.h>
TPP_DECL_BEGIN
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES && (TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK) */

#if TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK
TPP_IMPL TPP_FORMATPRINTER_DEFINE(_tpp_lexer_builtin_warn_or_mesg_printer, arg, text, num_bytes) {
	FILE *fp = stderr;
	(void)arg;
	fwrite(text, sizeof(tpp_char), num_bytes, fp);
	return ferror(fp) ? TPP_SSIZE_OFERR(TPP_EIO) : 0;
}
#endif /* TPP_HAVE_BUILTIN_WARNPRINTER_HOOK || TPP_HAVE_BUILTIN_MESGPRINTER_HOOK */



#if TPP_HAVE_WARNINGS
/* Interpret + print a warning-message "format" string.
 * The following %-encoded escape sequences are recognized:
 * - "%["    Start quoting text
 * - "%]"    Stop quoting text
 * - "%Pl"   1-based line described by "info"
 * - "%Pc"   1-based column described by "info"
 * - "%Pf"   Filename described by "info"
 * - "%Pt"   "%[current-token%]"   (based on tpp_lexer_gettoken(self))
 * - "%s"    As defined by stdc, using va_arg(args, char *)
 * - "%.*s"  As defined by stdc, using va_arg(args, int) + va_arg(args, char *)
 * - "%.Ns"  As defined by stdc, using va_arg(args, char *)
 * - "%.NPt" "%[<N bytes starting at "info->tlpfi_pos">%]"   (clamped if too big)
 * - "%d"    As defined by stdc, using va_arg(args, int)
 * - "%u"    As defined by stdc, using va_arg(args, unsigned int)
 * - "%c"    As defined by stdc, using va_arg(args, int)
 * - "%%"    "%" (emit a singular %-character)
 *
 * @param: info:    Information for special format descriptors
 *                  (unpopulated parts may be populated lazily)
 * @param: printer: Output printer for formatted text
 * @param: arg:     Cookie argument for "printer"
 * @param: format:  Format pattern (see above)
 * @param: args:    Extra varargs-arguments for "format"
 * @return: >= 0:   Sum of return values of "printer".
 * @return: < 0:    First negative return value of "printer". The more high-level
 *                  "tpp_lexer_warnf" API returns "TPP_EWARNPRINT" in this case. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 5)) tpp_ssize TPPVCALL
tpp_lexer_printf_warning(tpp_lexer const *self, tpp_lexer_printf_info *info,
                         tpp_formatprinter printer, void *arg,
                         char const *format, ...) {
	tpp_ssize result;
	va_list args;
	va_start(args, format);
	result = tpp_lexer_vprintf_warning(self, info, printer, arg, format, args);
	va_end(args);
	return result;
}


static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_print_uint(tpp_formatprinter printer, void *arg, tpp_uintmax value) {
	char buf[TPP_UTOA_MAXLEN];
	char const *start = tpp_utoa(buf, value);
	return tpp_formatprinter_print_cstr(printer, arg, start, (tpp_size)((buf + tpp_lengthof(buf)) - start));
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_print_int(tpp_formatprinter printer, void *arg, tpp_intmax value) {
	char buf[TPP_ITOA_MAXLEN];
	char const *start = tpp_itoa(buf, value);
	return tpp_formatprinter_print_cstr(printer, arg, start, (tpp_size)((buf + tpp_lengthof(buf)) - start));
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_quote_start(tpp_formatprinter printer, void *arg) {
	/* XXX: Do something more interesting here! */
	return tpp_formatprinter_print_conststr(printer, arg, "`");
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_quote_end(tpp_formatprinter printer, void *arg) {
	/* XXX: Do something more interesting here! */
	return tpp_formatprinter_print_conststr(printer, arg, "`");
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_format_token_data(tpp_formatprinter printer, void *arg,
                      tpp_char const *start, tpp_size length) {
	/* TODO: Escape line-feeds while printing token body */
	return tpp_formatprinter_print(printer, arg, start, length);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 5)) tpp_ssize TPPCALL
tpp_lexer_vprintf_warning(tpp_lexer const *self, tpp_lexer_printf_info *info,
                          tpp_formatprinter printer, void *arg,
                          char const *format, va_list args) {
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
			temp = tpp_formatprinter_print_cstr(printer, arg, format,
			                                    (tpp_size)(iter - format));
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			return result;
		}
		goto again;
	}
	if ((iter - 1) > format) {
		temp = tpp_formatprinter_print_cstr(printer, arg, format,
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

		case 'l': /* "%Pl"   1-based line described by "info" */
		case 'c': /* "%Pc"   1-based column described by "info" */
		{
			/* Lazily load line/column information (if not already loaded) */
			if (!tpp_lcinfo_isvalid(info->tlpfi_lc) &&
			    (info->tlpfi_file && info->tlpfi_pos)) {
				info->tlpfi_lc = tpp_file_getlcinfo(info->tlpfi_file,
				                                    info->tlpfi_pos);
			}
			if (tpp_lcinfo_isvalid(info->tlpfi_lc)) {
				temp = tpp_format_print_int(printer, arg,
				                            ch == 'l' ? (tpp_lcinfo_getline(info->tlpfi_lc) + 1)
				                                      : (tpp_lcinfo_getcol(info->tlpfi_lc) + 1));
			} else {
				temp = tpp_formatprinter_print_cstr(printer, arg, "?", 1);
			}
		}	break;

		case 'f': {
			/* "%Pf"   Filename described by "info" */
			if (info->tlpfi_filename == NULL) {
				if (info->tlpfi_file != NULL)
					info->tlpfi_filename = tpp_file_getfilename(info->tlpfi_file);
				if (info->tlpfi_filename == NULL)
					info->tlpfi_filename = "?";
			}
			temp = tpp_formatprinter_print_cstr(printer, arg, info->tlpfi_filename,
			                                    tpp_strlen(info->tlpfi_filename));
		}	break;

		case 't': {
			/* "%Pt"   "%[current-token%]" */
#if TPP_HAVE_LEXER_REPRTOKENID
			char const *token_repr;
#endif /* TPP_HAVE_LEXER_REPRTOKENID */
			tpp_token const *const token = tpp_lexer_gettoken(self);
			tpp_size length;
			temp = tpp_format_quote_start(printer, arg);
			if tpp_unlikely(temp < 0)
				goto err_temp;
			result += temp;
			length = (tpp_size)(token->tt_end - token->tt_start);
#if TPP_HAVE_LEXER_REPRTOKENID
			if ((length == 0) &&
			    (token_repr = tpp_lexer_reprtokenid(self, token->tt_id)) != NULL) {
				temp = tpp_formatprinter_print_cstr(printer, arg, token_repr, tpp_strlen(token_repr));
			} else
#endif /* TPP_HAVE_LEXER_REPRTOKENID */
			{
				temp = tpp_format_token_data(printer, arg, token->tt_start, length);
			}
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
		temp = tpp_formatprinter_print_cstr(printer, arg, s, tpp_strlen(s));
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
				temp = tpp_formatprinter_print_cstr(printer, arg, s, (tpp_size)length);
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
			unsigned int length = (unsigned int)tpp_ascii_asdigit(ch);
			for (;;) {
				ch = *iter++;
				if (tpp_ascii_isdigit(ch)) {
					length *= 10;
					length += (unsigned int)tpp_ascii_asdigit(ch);
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
				temp = tpp_formatprinter_print_cstr(printer, arg, s, (tpp_size)length);
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
					if (info->tlpfi_file == NULL ||
					    info->tlpfi_pos == NULL ||
					    info->tlpfi_pos >= info->tlpfi_file->tf_end) {
						temp = tpp_formatprinter_print_conststr(printer, arg, "?");
					} else {
						tpp_size maxlen = (tpp_size)(info->tlpfi_file->tf_end -
						                             info->tlpfi_pos);
						if ((tpp_size)length > maxlen)
							length = (unsigned int)maxlen;
						temp = tpp_format_token_data(printer, arg, info->tlpfi_pos, (tpp_size)length);
					}
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
		temp = tpp_formatprinter_print(printer, arg, &ord, 1);
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

#ifndef tpp_file_and_line
#define tpp_file_and_line tpp_file_and_line
static char const tpp_file_and_line[] = TPP_CONFIG_WARNING_FILE_AND_LINE_FORMAT;
#endif /* !tpp_file_and_line */

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_impl_custom(tpp_lexer *tpp_restrict const _self,
                             tpp_lexer_printf_info *tpp_restrict const _info,
                             tpp_formatprinter const _printer,
                             void *const _printer_arg,
                             tpp_warning_id const _id,
                             va_list _args) {
	switch (_id) {

/************************************************************************/
/* MACROS FOR USE BY "TPP_WARNING_EX"                                   */
/************************************************************************/
#define tpp_do(expr)           \
	do {                       \
		if ((expr) < 0)        \
			goto _err_printer; \
	} while (0)
#define tpp_current_va_arg(T)                va_arg(_args, T)
#define tpp_current_va_args()                _args
#define tpp_current_lexer()                  _self
#define tpp_current_info()                   _info
#define tpp_current_warning_id()             _id
#define tpp_current_printer()                _printer
#define tpp_current_printer_arg()            _printer_arg

/************************************************************************/
/* Convenience functions (100% implementable using API exposed above) */
#define tpp_warn_print_file_and_line(info)                                            \
	tpp_do(tpp_lexer_printf_warning(tpp_current_lexer(), info, tpp_current_printer(), \
	                                tpp_current_printer_arg(), tpp_file_and_line))
#define tpp_warn_print_file_and_line_at(file, pos)             \
	do {                                                       \
		tpp_lexer_printf_info _nest_info;                      \
		tpp_lexer_printf_info_init_at(&_nest_info, file, pos); \
		tpp_warn_print_file_and_line(&_nest_info);             \
	} while (0)
#define tpp_warn_print_file_and_line_lc(filename, lc)             \
	do {                                                          \
		tpp_lexer_printf_info _nest_info;                         \
		tpp_lexer_printf_info_init_lc(&_nest_info, filename, lc); \
		tpp_warn_print_file_and_line(&_nest_info);                \
	} while (0)
#define tpp_warn_print(text, num_bytes)            tpp_do(tpp_formatprinter_print(tpp_current_printer(), tpp_current_printer_arg(), text, num_bytes))
#define tpp_warn_print_cstr(text, num_bytes)       tpp_do(tpp_formatprinter_print_cstr(tpp_current_printer(), tpp_current_printer_arg(), text, num_bytes))
#define tpp_warn_print_conststr(STR)               tpp_do(tpp_formatprinter_print_conststr(tpp_current_printer(), tpp_current_printer_arg(), STR))
#define tpp_warn_printf0(info, format)             tpp_do(tpp_lexer_printf_warning(tpp_current_lexer(), info, tpp_current_printer(), tpp_current_printer_arg(), format))
#define tpp_warn_printf1(info, format, a)          tpp_do(tpp_lexer_printf_warning(tpp_current_lexer(), info, tpp_current_printer(), tpp_current_printer_arg(), format, a))
#define tpp_warn_printf2(info, format, a, b)       tpp_do(tpp_lexer_printf_warning(tpp_current_lexer(), info, tpp_current_printer(), tpp_current_printer_arg(), format, a, b))
#define tpp_warn_printf3(info, format, a, b, c)    tpp_do(tpp_lexer_printf_warning(tpp_current_lexer(), info, tpp_current_printer(), tpp_current_printer_arg(), format, a, b, c))
#define tpp_warn_printf4(info, format, a, b, c, d) tpp_do(tpp_lexer_printf_warning(tpp_current_lexer(), info, tpp_current_printer(), tpp_current_printer_arg(), format, a, b, c, d))
/************************************************************************/

#undef GUARD_TPP_AMALGAMATION_H
#define TPP_DEFS
#define TPP_WARNING_EX(warning_id, wgroup_ids, numbers, numbers_default, expr) \
		case warning_id: {                                                     \
			expr;                                                              \
		}	break;
#include TPP_CONFIG_DEFS_FILENAME
#undef TPP_DEFS

/************************************************************************/
#undef tpp_warn_print_file_and_line
#undef tpp_warn_print_file_and_line_at
#undef tpp_warn_print_file_and_line_lc
#undef tpp_warn_print
#undef tpp_warn_print_cstr
#undef tpp_warn_print_conststr
#undef tpp_warn_printf0
#undef tpp_warn_printf1
#undef tpp_warn_printf2
#undef tpp_warn_printf3
#undef tpp_warn_printf4
/************************************************************************/
#undef tpp_do
#undef tpp_current_va_arg
#undef tpp_current_va_args
#undef tpp_current_lexer
#undef tpp_current_info
#undef tpp_current_warning_id
#undef tpp_current_printer
#undef tpp_current_printer_arg
/************************************************************************/

	default:
		if (tpp_formatprinter_print_conststr(_printer, _printer_arg, "UNKNOWN WARNING\n") < 0)
			goto _err_printer;
		break;
	}
	return TPP_EOK;
_err_printer:
	return TPP_EWARNPRINT;
}

#ifndef tpp_lexer_gethook_warnprinter
#define tpp_lexer_gethook_warnprinter(self) (&tpp_dummy_printer)
#ifndef tpp_dummy_printer
#define tpp_dummy_printer tpp_dummy_printer
static TPP_FORMATPRINTER_DEFINE(tpp_dummy_printer, arg, text, num_bytes) {
	(void)arg;
	(void)text;
	(void)num_bytes;
	return 0;
}
#endif /* !tpp_dummy_printer */
#endif /* !tpp_lexer_gethook_warnprinter */

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_impl(tpp_lexer *tpp_restrict self,
                      tpp_lexer_printf_info *tpp_restrict info,
                      tpp_warning_id id, va_list args) {
	tpp_errno result;
	tpp_ssize printer_status;
	char const *warning_format;
	tpp_warning_invokeinfo invokeinfo;
	tpp_formatprinter const printer = tpp_lexer_gethook_warnprinter(self);
	void *const printer_arg = self;

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
		tpp_file const *const file = info->tlpfi_file ? info->tlpfi_file : tpp_lexer_getfile(self);
		tpp_file const *const textfile = tpp_file_gettextfile(file);
		if (textfile && tpp_file_getsystemheader(textfile))
			return TPP_EOK; /* Suppress warnings in this file */
#endif /* TPP_HAVE_FILE_SYSHDR */
#if TPP_HAVE_LEXER_WARNING_COUNT
		++self->tl_warning_count;
#endif /* TPP_HAVE_LEXER_WARNING_COUNT */
	}	break;

#if TPP_HAVE_WARNING_ERROR
	case TPP_WSTATE_ERROR: {
		tpp_size errors = tpp_lexer_geterrorcount(self);
		tpp_lexer_seterrorcount(self, errors + 1);
#if TPP_ERROR_LIMIT != 0
		if (errors >= tpp_lexer_geterrorlimit(self))
			result = TPP_ELEXERROR;
#endif /* TPP_ERROR_LIMIT != 0 */
	}	break;
#endif /* TPP_HAVE_WARNING_ERROR */

	case TPP_WSTATE_FATAL:
		result = TPP_ELEXERROR;
		break;

	default: tpp_unreachable();
	}

	/* Print file-and-line prefix */
	printer_status = tpp_lexer_printf_warning(self, info, printer, printer_arg, tpp_file_and_line);
	if (printer_status < 0)
		goto err_printer;

	/* Print what this is about... */
	if (invokeinfo.twii_state == TPP_WSTATE_WARN) {
		printer_status = tpp_formatprinter_print_conststr(printer, printer_arg, "warning[");
	} else {
		printer_status = tpp_formatprinter_print_conststr(printer, printer_arg, "error[");
	}
	if (printer_status < 0)
		goto err_printer;

	/* Print the relevant context name. */
#if TPP_HAVE_WARNING_NUMBERS
	if (tpp_warning_context_id_isnumber(invokeinfo.twii_ctx_id)) {
		tpp_warning_id ctx_wid = tpp_warning_context_id_aswarning(invokeinfo.twii_ctx_id);
		unsigned int number = tpp_warning_getnumbers(ctx_wid)[0];
		if tpp_unlikely(number == TPP_WARNING_NUMBER_INVALID) {
			printer_status = tpp_formatprinter_print_conststr(printer, printer_arg, "?");
		} else {
			printer_status = tpp_format_print_uint(printer, printer_arg, number);
		}
	} else
#endif /* TPP_HAVE_WARNING_NUMBERS */
	{
		tpp_warning_group_id group_id = tpp_warning_context_id_asgroup(invokeinfo.twii_ctx_id);
		char const *group_name = tpp_warning_group_getnames(group_id);
		if tpp_unlikely(group_name == NULL) {
			printer_status = tpp_formatprinter_print_conststr(printer, printer_arg, "?");
		} else {
			printer_status = tpp_formatprinter_print_conststr(printer, printer_arg, "-W");
			if (printer_status < 0)
				goto err_printer;
			printer_status = tpp_formatprinter_print_cstr(printer, printer_arg, group_name, tpp_strlen(group_name));
		}
	}
	if (printer_status < 0)
		goto err_printer;
	printer_status = tpp_formatprinter_print_conststr(printer, printer_arg, "]: ");
	if (printer_status < 0)
		goto err_printer;


	/* Lookup the generic warning-message format for "id" */
	warning_format = tpp_warning_getformat(id);
	if (warning_format) {
		printer_status = tpp_lexer_vprintf_warning(self, info,
		                                           printer, printer_arg,
		                                           warning_format, args);
		if (printer_status < 0)
			goto err_printer;
		printer_status = tpp_formatprinter_print_conststr(printer, printer_arg, "\n");
		if (printer_status < 0)
			goto err_printer;
	} else {
		/* In this case, "id" must be using a custom warning message expression! */
		tpp_errno error;
		error = tpp_lexer_vwarnf_impl_custom(self, info, printer,
		                                     printer_arg, id, args);
		if (TPP_ISERR(error)) {
			tpp_assert((error == TPP_ENOMEM || error == TPP_EIO ||
			            error == TPP_ELEXERROR || error == TPP_EWARNPRINT) &&
			           "Custom warning callbacks may only return one of these errors");
			return error;
		}
	}

	/* Print projection origin */
#if TPP_HAVE_CPP_MACROS
	if (info->tlpfi_file && info->tlpfi_pos) {
		tpp_lcinfo_ex lcx;
		tpp_file_getlcinfo_ex(info->tlpfi_file, info->tlpfi_pos, &lcx);
		while (lcx.tlcix_projfile) {
			tpp_lcinfo_ex nlcx;
			tpp_lexer_printf_info projection_info;
			tpp_lexer_printf_info_init_at(&projection_info, lcx.tlcix_projfile, lcx.tlcix_projpos);
			tpp_file_getlcinfo_ex(lcx.tlcix_projfile, lcx.tlcix_projpos, &nlcx);
			projection_info.tlpfi_lc = nlcx.tlcix_info;
			printer_status = tpp_lexer_printf_warning(self, &projection_info,
			                                          printer, printer_arg,
			                                          tpp_file_and_line);
			if (printer_status < 0)
				goto err_printer;
			printer_status = tpp_formatprinter_print_conststr(printer, printer_arg,
			                                                  "note: projected from here\n");
			if (printer_status < 0)
				goto err_printer;
			lcx = nlcx;
		}
	}
#endif /* TPP_HAVE_CPP_MACROS */

	/* Print origin traceback */
#if TPP_HAVE_INCLUDE_STACK
	if (info->tlpfi_file) {
		tpp_file *caller = info->tlpfi_file->tf_tprev;
		for (; caller; caller = caller->tf_tprev) {
			/* XXX: We could also display a range here:
			 * >> [caller->tf_tpos, caller->tf_pos)
			 *
			 * This range of bytes describes the "expression" that caused
			 * the include/macro-expansion to happen (it is either the range
			 * from the start of a macro's name, to the end of its parameter
			 * list, or the start of a #include-directive, to the trailing
			 * line-feed) */
			tpp_lexer_printf_info caller_info;
			tpp_lexer_printf_info_init_at(&caller_info, caller, caller->tf_tpos);
			printer_status = tpp_lexer_printf_warning(self, &caller_info,
			                                          printer, printer_arg,
			                                          tpp_file_and_line);
			if (printer_status < 0)
				goto err_printer;
			printer_status = tpp_formatprinter_print_conststr(printer, printer_arg, "note: originating from here\n");
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
		/* TODO: This should be standard hook instead of this weird #ifdef-macro! */
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
TPP_IMPL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, va_list args) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_lexer_printf_info info;
	tpp_lexer_printf_info_init_at(&info, file, tpp_file_getlastpos(file));
	return tpp_lexer_vwarnf_impl(self, &info, id, args);
}

TPP_IMPL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf(self, id, args);
	va_end(args);
	return result;
}


TPP_IMPL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_at(tpp_lexer *tpp_restrict self, tpp_file *file,
                    tpp_char const *pos, tpp_warning_id id, va_list args) {
	tpp_lexer_printf_info info;
	tpp_lexer_printf_info_init_at(&info, file, pos);
	return tpp_lexer_vwarnf_impl(self, &info, id, args);
}

TPP_IMPL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_file *file,
                   tpp_char const *pos, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf_at(self, file, pos, id, args);
	va_end(args);
	return result;
}

TPP_IMPL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_lc(tpp_lexer *tpp_restrict self, char const *filename,
                    tpp_lcinfo lc, tpp_warning_id id, va_list args) {
	tpp_lexer_printf_info info;
	tpp_lexer_printf_info_init_lc(&info, filename, lc);
	return tpp_lexer_vwarnf_impl(self, &info, id, args);
}

TPP_IMPL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf_lc(tpp_lexer *tpp_restrict self, char const *filename,
                   tpp_lcinfo lc, tpp_warning_id id, ...) {
	tpp_errno result;
	va_list args;
	va_start(args, id);
	result = tpp_lexer_vwarnf_lc(self, filename, lc, id, args);
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
		result = tpp_lexer_warnf_lc(self, tpp_file_getfilename(file),
		                            tpp_ifdef_stack_entry_getupdated(entry),
		                            TPP_W_EOF_BEFORE_ENDIF);
		if (TPP_ISERR(result))
			break;
	}
	return result;
}
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_WARN_C */
