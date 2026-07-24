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
#ifndef GUARD_TPP_LEXER_YIELD_INCLUDE_C
#define GUARD_TPP_LEXER_YIELD_INCLUDE_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "features.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "tuple.h"
#include "macro.h"
#include "sysinclude.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_YIELD_INCLUDE_STRING
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_include_string(tpp_lexer *tpp_restrict self, tpp_char const **p_pos) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_char const *pos = *p_pos;
	tpp_size const rel_start = tpp_file_ptr2rel(file, pos);
	tpp_char ch;
	tpp_errno error;
	error = tpp_lexer_readchar(self, &pos, &ch);
	if (TPP_ISERR(error))
		goto handle_error;
	if (ch == '<' || ch == '"') {
		tpp_char const start_ch = ch;
		tpp_char const end_ch = ch == '<' ? '>' : ch;
		for (;;) {
			tpp_size const rel_pos = tpp_file_ptr2rel(file, pos);
			error = tpp_lexer_readchar(self, &pos, &ch);
			if (TPP_ISERR(error))
				goto handle_error;
			if (ch == end_ch)
				break;
			if (ch == '\0' && pos >= file->tf_end) {
				/* Warn about EOF in #include-string */
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF
				error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
				                           TPP_W_STRING_TERMINATED_BY_EOF);
				if (TPP_ISERR(error))
					goto handle_error;
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_EOF */
				pos = tpp_file_rel2ptr(file, rel_pos);
				break;
			}

			if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_UNICODE
warn_linefeed:
#endif /* TPP_HAVE_UNICODE */
				/* Warn about line-feed in #include-string */
#if TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED
				error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
				                           TPP_W_STRING_TERMINATED_BY_LINEFEED);
				if (TPP_ISERR(error))
					goto handle_error;
#endif /* TPP_HAVE_TPP_W_STRING_TERMINATED_BY_LINEFEED */
				pos = tpp_file_rel2ptr(file, rel_pos);
				break;
			} else
#if TPP_HAVE_UNICODE
			if (tpp_ascii_ismb(ch) && tpp_file_isutf8(file)) {
				tpp_unichar uc;
				--pos;
				error = tpp_lexer_readunichar(self, &pos, &uc);
				if (TPP_ISERR(error))
					goto handle_error;
				if (tpp_unicode_islf(uc))
					goto warn_linefeed;
			} else
#endif /* TPP_HAVE_UNICODE */
			{
			}
		}
		/* Success -> setup token to describe the #include-string */
		token->tt_id    = TPP_TOK_OFCHAR(start_ch);
		token->tt_start = tpp_file_rel2ptr(file, rel_start);
//		token->tt_end   = pos; /* Must be done by caller if that's what they want ... */
		*p_pos = pos;          /* ... or done by this, in case "p_pos == &token->tt_end" */
		return TPP_TOK_OFCHAR(start_ch);
	}

	/* Fallback: not an #include-string -> rewind and use "tpp_lexer_yieldraw_at()" to parse next token */
	*p_pos = tpp_file_rel2ptr(file, rel_start);
	return tpp_lexer_yieldraw_at(self, p_pos);
handle_error:
	*p_pos = tpp_file_rel2ptr(file, rel_start);
	return TPP_TOK_OFERR(error);
}

#if TPP_HAVE_FILE_NONBLOCK
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_include_string_blocking(tpp_lexer *tpp_restrict self,
                                              tpp_char const **p_pos) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldraw_at_include_string(self, p_pos);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_flags & TPP_FILE_FLAGS_NONBLOCK);
		file->tf_flags &= ~TPP_FILE_FLAGS_NONBLOCK;
		tpp_file_autopopfile_pushoff(file);
		result = tpp_lexer_yieldraw_at_include_string(self, p_pos);
		tpp_file_autopopfile_pop(file);
		file->tf_flags |= TPP_FILE_FLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}

#if TPP_HAVE_CPP_MACROS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_include_string_blocking(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yield_include_string(self);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_flags & TPP_FILE_FLAGS_NONBLOCK);
		file->tf_flags &= ~TPP_FILE_FLAGS_NONBLOCK;
		tpp_file_autopopfile_pushoff(file);
		result = tpp_lexer_yield_include_string(self);
		tpp_file_autopopfile_pop(file);
		file->tf_flags |= TPP_FILE_FLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}
#endif /* TPP_HAVE_CPP_MACROS */
#endif /* TPP_HAVE_FILE_NONBLOCK */


/* (Mostly) the same as "tpp_lexer_yield()", except:
 * - Never process preprocessor directives (but macros are still expanded)
 * - If the next token starts with '"' or '<', parse it as a #include-string,
 *   with the token's start/end bounds pointing at the string's bounds. In
 *   this case, the token's ID (and return value) is:
 *   - TPP_TOK_INCPATH_DQUOTE  // For #include "foo.h"
 *   - TPP_TOK_INCPATH_LANGLE  // For #include <foo.h>
 * - WARNING: This function doesn't filter SPACE/LF/COMMENT tokens
 *   (behaves as though 'TPP_LEXER_STATE_FLAG_ALLTOKENS' was set)
 *
 * @return: * : Some other token encountered (token was parsed like tpp_lexer_yieldraw())
 * @return: TPP_TOK_INCPATH_DQUOTE: #include-string parsed: "foo.h"
 * @return: TPP_TOK_INCPATH_LANGLE: #include-string parsed: <foo.h>
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_FLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
#if TPP_HAVE_CPP_MACROS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_include_string(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldraw_include_string(self);
	if (TPP_TOK_ISKEYWORD(result)) {
		/* Do macro expansion... */
		result = tpp_lexer_yield_handle_keyword(self, result);
		if (result == TPP_TOK_EOF)
			goto again;
	}
	return result;
}
#endif /* TPP_HAVE_CPP_MACROS */
#endif /* TPP_HAVE_LEXER_YIELD_INCLUDE_STRING */


#if TPP_HAVE_LEXER_DECODE_INCLUDE_STRING
/* Decode the current token as a #include-string. The caller is responsible to
 * ensure that the current token was loaded by `tpp_lexer_yield_include_string()'
 * and is either TPP_TOK_INCPATH_LANGLE or TPP_TOK_INCPATH_DQUOTE
 *
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_decode_include_string(tpp_lexer const *tpp_restrict self,
                                tpp_formatprinter printer, void *arg) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_token_id const mode = token->tt_id;
	tpp_char const *start = token->tt_start;
	tpp_char const *end = token->tt_end;
	tpp_char const end_ch = mode == '<' ? '>' : (tpp_char)mode;
	tpp_assert(mode == '"' || mode == '<');
	if (start < end && start[0] == (tpp_char)mode)
		++start;
	if (start < end && end[-1] == (tpp_char)end_ch)
		--end;
#if (TPP_CONF_MAYBE_0(TPP_HAVE_BSE) && TPP_CONF_MAYBE_0(TPP_HAVE_TRIGRAPHS))
#if (TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS))
	if (!tpp_lexer_has(self, BSE) &&
	    !tpp_lexer_has(self, TRIGRAPHS))
#endif /* TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS) */
	{
		/* No cleanup necessary (no BSE or trigraphs) */
		return tpp_formatprinter_print(printer, arg, start, (tpp_size)(end - start));
	}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_BSE) && TPP_CONF_MAYBE_0(TPP_HAVE_TRIGRAPHS) */
#if (TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS))
	{
		tpp_ssize temp, result = 0;
		tpp_char const *iter = start;
		while (iter < end) {
			tpp_char ch = *iter++;
#if TPP_HAVE_TRIGRAPHS
			if (ch == '?' && ((iter + 1) < end && iter[0] == '?') &&
			    tpp_lexer_has(self, TRIGRAPHS)) {
				switch (iter[1]) {
				case '=': ch = '#'; break;
				case '(': ch = '['; break;
				case ')': ch = ']'; break;
				case '\'': ch = '^'; break;
				case '<': ch = '{'; break;
				case '!': ch = '|'; break;
				case '>': ch = '}'; break;
				case '-': ch = '~'; break;
				case '/':
#if TPP_HAVE_BSE
					if (tpp_lexer_has(self, BSE)) {
						tpp_char const *before_bse = iter - 1;
						tpp_char const *after_bse = tpp_skipbse_fwd(before_bse, end, tpp_lexer_getfile(self));
						if (after_bse > before_bse) {
							temp = tpp_formatprinter_print(printer, arg, start, (tpp_size)(before_bse - start));
							if (temp < 0)
								goto err_temp;
							result += temp;
							iter = start = after_bse;
							continue;
						}
					}
#endif /* TPP_HAVE_BSE */
					ch = '\\';
					break;
				default: continue; /* Not actually a trigraph escape sequence */
				}
				--iter;

				/* No need to warn about trigraph -- was already done in `tpp_lexer_yieldraw()' */

				/* Print trigraph character (but also handle case where "??/" was encoded) */
				temp = tpp_formatprinter_print(printer, arg, start, (tpp_size)(iter - start));
				if (temp < 0)
					goto err_temp;
				result += temp;
				iter += 3;
				start = iter;
				temp = tpp_formatprinter_print(printer, arg, &ch, 1);
				if (temp < 0)
					goto err_temp;
				result += temp;
				continue;
			} else
#endif /* TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_BSE
			if (ch == '\\') {
				if (tpp_lexer_has(self, BSE)) {
					tpp_char const *before_bse = iter - 1;
					tpp_char const *after_bse = tpp_skipbse_fwd(before_bse, end, tpp_lexer_getfile(self));
					if (after_bse > before_bse) {
						temp = tpp_formatprinter_print(printer, arg, start, (tpp_size)(before_bse - start));
						if (temp < 0)
							goto err_temp;
						result += temp;
						iter = start = after_bse;
						continue;
					}
				}
			} else
#endif /* TPP_HAVE_BSE */
			{
			}
		} /* while (iter < end) */

		/* Flush remainder */
		if (start < end) {
			temp = tpp_formatprinter_print(printer, arg, start, (tpp_size)(end - start));
			if tpp_unlikely(temp < 0) {
err_temp:
				return temp;
			}
			result += temp;
		}
		return result;
	}
#endif /* TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS) */
}

struct tpp_lexer_decode_include_string_cb_single_data {
	tpp_errno (TPPCALL *tldiscsd_cb)(void *arg, char const *str, tpp_size length);
	void               *tldiscsd_arg;
};

static TPP_FORMATPRINTER_DEFINE(tpp_lexer_decode_include_string_cb_single, arg, text, num_bytes) {
	tpp_errno result;
	struct tpp_lexer_decode_include_string_cb_single_data *data;
	data = (struct tpp_lexer_decode_include_string_cb_single_data *)arg;
	tpp_assert(data->tldiscsd_cb && "Multiple invocations, or no callback given");
	result = (*data->tldiscsd_cb)(data->tldiscsd_arg, (char const *)text, num_bytes);
#if TPP_DEBUG
	data->tldiscsd_cb = NULL;
#endif /* TPP_DEBUG */
	return result;
}

#if (TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS))
static TPP_FORMATPRINTER_DEFINE(tpp_lexer_decode_include_string_count_cb, arg, text, num_bytes) {
	(void)arg;
	(void)text;
	(void)num_bytes;
	return 1;
}
#endif /* TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS) */

/* Same as `tpp_lexer_decode_include_string()', but the given "cb" is only
 * invoked once whilst being passed the *entire* (decoded) #include-string.
 *
 * @return: * : Return value of the (singular) invocation of "cb"
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decode_include_string_cb(tpp_lexer const *tpp_restrict self,
                                   tpp_errno (TPPCALL *cb)(void *arg, char const *str, tpp_size length),
                                   void *arg) {
#if (TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS))
	tpp_ssize count = tpp_lexer_decode_include_string(self, &tpp_lexer_decode_include_string_count_cb, NULL);
	tpp_assert(count >= 0);
	if tpp_unlikely(count == 0)
		return (*cb)(arg, "", 0);
	if tpp_unlikely(count > 1) {
		/* Complicated case: multi-chunk #include-string */
		tpp_errno result;
		tpp_ssize status;
		tpp_string_builder builder;
		TPP_REF tpp_string *string;
		tpp_string_builder_init(&builder);
		status = tpp_lexer_decode_include_string(self, &tpp_string_builder_print, &builder);
		if tpp_unlikely(TPP_SSIZE_ISERR(status)) {
			tpp_string_builder_fini(&builder);
			return TPP_SSIZE_ASERR(status);
		}
		string = tpp_string_builder_pack(&builder);
		result = (*cb)(arg, tpp_string_cstr(string), tpp_string_len(string));
		tpp_string_decref(string);
		return result;
	} else
#endif /* TPP_CONF_MAYBE_1(TPP_HAVE_BSE) || TPP_CONF_MAYBE_1(TPP_HAVE_TRIGRAPHS) */
	{
		/* Simple (and most likely) case: single-chunk */
		tpp_ssize result;
		struct tpp_lexer_decode_include_string_cb_single_data data;
		data.tldiscsd_cb  = cb;
		data.tldiscsd_arg = arg;
		result = tpp_lexer_decode_include_string(self, &tpp_lexer_decode_include_string_cb_single, &data);
		tpp_assert(TPP_SSIZE_ISERR_OR_EOK(result));
		return TPP_SSIZE_ASERR_OR_EOK(result);
	}
}
#endif /* TPP_HAVE_LEXER_DECODE_INCLUDE_STRING */


#if TPP_HAVE_LEXER_OPEN_INCLUDE_STRING

#if TPP_HAVE_INCLUDE_PATH
/* Try to open "str" in "paths"; returns TPP_ENOENT if not found */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_foreach_include_path_in_list(tpp_include_path_list const *paths,
                                       tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to
                                                  tpp_lexer_foreach_include_path_flags__PARAM),
                                       void *arg tpp_lexer_foreach_include_path_flags__PARAM) {
	tpp_size i;
	tpp_errno result = TPP_ENOENT;
	for (i = 0; i < paths->tipl_size; ++i) {
		tpp_include_path_entry const *entry = &paths->tipl_list[i];
		char const *path = _tpp_include_path_entry_getpath(entry);
		result = (*cb)(arg, path tpp_lexer_foreach_include_path_flags__ARG(flags));
		if (result != TPP_ENOENT)
			break;
	}
	return result;
}
#endif /* TPP_HAVE_INCLUDE_PATH */

/* Enumerate #include-paths according to "mode"
 * @param: mode: #include-mode (either TPP_TOK_INCPATH_LANGLE or TPP_TOK_INCPATH_DQUOTE)
 * @param: cb:   Callback invoked for each available #include-path. The first time
 *               this callback returns something other than TPP_ENOENT, that return
 *               value is propagated.
 * @param: cb.flags: Either "TPP_FILE_FLAGS_NORMAL" or "TPP_FILE_FLAGS_SYSHDR",
 *                   possibly or'd with "TPP_FILE_FLAGS_EXTERN_C" depending on
 *                   where "relative_to" originates from, and how "self" has been
 *                   configured (see "TPP_HAVE_EXTERN_C_FOR_SYSHDR")
 * @param: arg:  Cookie for "cb"
 * @return: * :  The first non-TPP_ENOENT return value of "cb"
 * @return: TPP_ENOENT: Either "cb" was never invoked (no #include-paths), or all
 *                      invocations of "cb" returned "TPP_ENOENT". */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_lexer_foreach_include_path(tpp_lexer *tpp_restrict self, tpp_token_id mode,
                               tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to
                                                       tpp_lexer_foreach_include_path_flags__PARAM),
                               void *arg) {
#if TPP_HAVE_EXTERN_C_FOR_SYSHDR == 0
#define TPP_LEXER_FOREACH_INCLUDE_PATH_SYSHDR_FLAGS (TPP_FILE_FLAGS_SYSHDR)
#elif !TPP_CONF_MAYBE_0(TPP_HAVE_EXTERN_C_FOR_SYSHDR)
#define TPP_LEXER_FOREACH_INCLUDE_PATH_SYSHDR_FLAGS (TPP_FILE_FLAGS_SYSHDR | TPP_FILE_FLAGS_EXTERN_C)
#else /* TPP_HAVE_EXTERN_C_FOR_SYSHDR */
#define TPP_LEXER_FOREACH_INCLUDE_PATH_SYSHDR_FLAGS                                               \
	(tpp_lexer_has(self, EXTERN_C_FOR_SYSHDR) ? (TPP_FILE_FLAGS_SYSHDR | TPP_FILE_FLAGS_EXTERN_C) \
	                                          : (TPP_FILE_FLAGS_SYSHDR))
#endif /* !TPP_HAVE_EXTERN_C_FOR_SYSHDR */
#if TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK
#define tpp_lexer_foreach_include_path_hook(when)                              \
	error = tpp_lexer_callhook_system_include_path(self, mode, when, cb, arg); \
	if (error != TPP_ENOENT)                                                   \
		return error;
#else /* TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK */
#define tpp_lexer_foreach_include_path_hook(when) /* nothing */
#endif /* !TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK */
	tpp_errno error;
	tpp_assert(mode == '<' || mode == '"');
	tpp_lexer_foreach_include_path_hook(TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_FIRST);
	if (mode == '"') {
		/* Try to open files relative to the current #include-stack */
		tpp_file const *file = tpp_lexer_getfile(self);
		do {
#if TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_CPP_MACROS
			/* Must also accept TEXT-files as base:
			 * - The API user may have explicitly pushed a file using `tpp_lexer_pushfile_text_*'
			 * - We might be inside of a "tpp_file_pusheof()"-block (actually, this is *highly*
			 *   likely, since regular #if and #embed directives are usually parsed within such
			 *   a block to ensure they don't span past EOL, meaning that __has_include and the
			 *   filename taken by #embed originate from a TEXT-file at that point) */
			if (file->tf_kind == TPP_FILE_KIND_IO ||
			    file->tf_kind == TPP_FILE_KIND_TEXT)
#endif /* TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_CPP_MACROS */
			{
				char const *filename = file->tf_data.td_io.tff_name;
				if (filename) {
					error = (*cb)(arg, filename tpp_lexer_foreach_include_path_flags__ARG(TPP_FILE_FLAGS_NORMAL));
					if (error != TPP_ENOENT)
						return error;
				}
#if TPP_CONF_MAYBE_0(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE)
				if (!tpp_lexer_has(self, INCLUDE_RELATIVE_TO_EVERY_FILE))
					break;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE) */
			}
		}
#if TPP_HAVE_INCLUDE_STACK
		while ((file = file->tf_tprev) != NULL);
#else /* TPP_HAVE_INCLUDE_STACK */
		while (0);
#endif /* !TPP_HAVE_INCLUDE_STACK */

		/* Search the quote-include path */
#if TPP_HAVE_INCLUDE_PATH && TPP_HAVE_INCLUDE_PATH_QUOTE
		tpp_lexer_foreach_include_path_hook(TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_QUOTE);
		error = tpp_lexer_foreach_include_path_in_list(&self->tl_include_paths.tip_quote_list, cb, arg
		                                               tpp_lexer_foreach_include_path_flags__ARG(TPP_FILE_FLAGS_NORMAL));
		if (error != TPP_ENOENT)
			return error;
#endif /* TPP_HAVE_INCLUDE_PATH && TPP_HAVE_INCLUDE_PATH_QUOTE */
	}

	/* Try to open files relative to system include paths
	 * Yes: these paths are checked even for "foo.h"-style
	 *      include strings -- that's what the standard says. */

	/* Search the system-include path */
#if TPP_HAVE_INCLUDE_PATH
	tpp_lexer_foreach_include_path_hook(TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_SYSTEM);
	error = tpp_lexer_foreach_include_path_in_list(&self->tl_include_paths.tip_system_list, cb, arg
	                                               tpp_lexer_foreach_include_path_flags__ARG(TPP_FILE_FLAGS_NORMAL));
	if (error != TPP_ENOENT)
		return error;
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
	tpp_lexer_foreach_include_path_hook(TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_SYSHDR);
	error = tpp_lexer_foreach_include_path_in_list(&self->tl_include_paths.tip_syshdr_list, cb, arg
	                                               tpp_lexer_foreach_include_path_flags__ARG(TPP_LEXER_FOREACH_INCLUDE_PATH_SYSHDR_FLAGS));
	if (error != TPP_ENOENT)
		return error;
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#endif /* TPP_HAVE_INCLUDE_PATH */

	/* Check hard-coded system include paths... */
#if TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH
	if (tpp_lexer_has(self, SEARCH_SYSTEM_INCLUDE_PATH)) {
		tpp_lexer_foreach_include_path_hook(TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_CONFIG);
#define tpp_handle_system_include_path(_, index, value)                                                                                \
		error = (*cb)(arg, value TPP_FS_SEP_S tpp_lexer_foreach_include_path_flags__ARG(TPP_LEXER_FOREACH_INCLUDE_PATH_SYSHDR_FLAGS)); \
		if (error != TPP_ENOENT)                                                                                                       \
			return error;
		TPP_TUPLE_FOREACH(TPP_CONFIG_SYSTEM_INCLUDE_PATH, TPP_TUPLE_FOREACH_DUMMY_SEP,
		                  tpp_handle_system_include_path, ~)
#undef tpp_handle_system_include_path
	}
#endif /* TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH */


	/* Check "after" system include paths... */
#if TPP_HAVE_INCLUDE_PATH && TPP_HAVE_INCLUDE_PATH_AFTER
	tpp_lexer_foreach_include_path_hook(TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_BEFORE_AFTER);
	error = tpp_lexer_foreach_include_path_in_list(&self->tl_include_paths.tip_after_list, cb, arg
	                                               tpp_lexer_foreach_include_path_flags__ARG(TPP_LEXER_FOREACH_INCLUDE_PATH_SYSHDR_FLAGS));
	if (error != TPP_ENOENT)
		return error;
#endif /* TPP_HAVE_INCLUDE_PATH && TPP_HAVE_INCLUDE_PATH_AFTER */

	/* Invoke the final system include path hook */
	tpp_lexer_foreach_include_path_hook(TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_LAST);

	/* File not found :( */
	return TPP_ENOENT;
#undef TPP_LEXER_FOREACH_INCLUDE_PATH_SYSHDR_FLAGS
}


struct tpp_lexer_open_include_string_data {
	tpp_lexer                 *tloisd_lexer;      /* [1..1] lexer */
	tpp_lexer_openfile_result *tloisd_result;     /* [1..1] Openfile result */
#if TPP_HAVE_LEXER_OPENFILE_EX
	tpp_lexer_openfile_flags   tloisd_mask_flags; /* Mask flags */
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
	char const                *tloisd_str;        /* #include-string (used during callback) */
	tpp_size                   tloisd_length;     /* #include-string length (used during callback) */
};

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_open_include_string_path_cb(void *arg, char const *relative_to
                                      tpp_lexer_foreach_include_path_flags__PARAM) {
	tpp_errno result;
	struct tpp_lexer_open_include_string_data *data;
	data = (struct tpp_lexer_open_include_string_data *)arg;
#if TPP_HAVE_LEXER_OPENFILE_EX
	result = tpp_lexer_openfile_ex(data->tloisd_lexer, relative_to,
	                               data->tloisd_str, data->tloisd_length,
	                               data->tloisd_result, data->tloisd_mask_flags);
#else /* TPP_HAVE_LEXER_OPENFILE_EX */
	result = tpp_lexer_openfile(data->tloisd_lexer, relative_to,
	                            data->tloisd_str, data->tloisd_length,
	                            data->tloisd_result);
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
#if TPP_HAVE_FILE_SYSHDR
	data->tloisd_result->tlofr_fileflags |= flags; /* Add "TPP_FILE_FLAGS_SYSHDR" + "TPP_FILE_FLAGS_EXTERN_C" flags if necessary */
#endif /* TPP_HAVE_FILE_SYSHDR */
	return result;
}


static tpp_errno TPPCALL
tpp_lexer_open_include_string_cb(void *arg, char const *str, tpp_size length) {
	tpp_token_id mode;
	struct tpp_lexer_open_include_string_data *data;
	data = (struct tpp_lexer_open_include_string_data *)arg;
	data->tloisd_str    = str;
	data->tloisd_length = length;
	/* Check for special case: if the given filename is absolute,
	 * then we must skip all the include-path resolution! */
	if (TPP_FS_ISABS(str, length))
		return tpp_lexer_open_include_string_path_cb(data, NULL tpp_lexer_foreach_include_path_flags__ARG(TPP_FILE_FLAGS_NORMAL));
	mode = tpp_lexer_gettok(data->tloisd_lexer);
	tpp_assert(mode == '<' || mode == '"');
	return tpp_lexer_foreach_include_path(data->tloisd_lexer, mode,
	                                      &tpp_lexer_open_include_string_path_cb,
	                                      data);
}


/* Wrapper around `tpp_lexer_decode_include_string_cb()' that automatically
 * does the necessary calls to `tpp_lexer_openfile_ex()'. It also handles
 * the `TPP_ENOENT' (as far as possible) by continuing to search for other
 * matching files. The specified "mask_flags" should be set depending on
 * context like:
 * - #include ...             TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED
 * - __has_include(...)       TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED
 * - #include_next ...        TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED | TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT
 * - __has_include_next(...)  TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED | TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT
 * - #import ...              TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED | TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED
 * - #embed ...               TPP_LEXER_OPENFILE_FLAG_NORMAL
 * - __has_embed(...)         TPP_LEXER_OPENFILE_FLAG_NORMAL
 *
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOMEM:  Insufficient memory
 * @return: TPP_ENOENT:  No such file (no warning printed, yet)
 * @return: TPP_EMASKED: (tpp_lexer_open_include_string_ex only): Flags
 *                       specified by "mask_flags" were already set. */
#if TPP_HAVE_LEXER_OPENFILE_EX
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_open_include_string_ex(tpp_lexer *tpp_restrict self,
                                 /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result,
                                 tpp_lexer_openfile_flags mask_flags)
#else /* TPP_HAVE_LEXER_OPENFILE_EX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_open_include_string(tpp_lexer *tpp_restrict self,
                              /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result)
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
{
	struct tpp_lexer_open_include_string_data data;
	data.tloisd_lexer  = self;
	data.tloisd_result = result;
#if TPP_HAVE_LEXER_OPENFILE_EX
	data.tloisd_mask_flags = mask_flags;
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
	return tpp_lexer_decode_include_string_cb(self, &tpp_lexer_open_include_string_cb, &data);
}
#endif /* TPP_HAVE_LEXER_OPEN_INCLUDE_STRING */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELD_INCLUDE_C */
