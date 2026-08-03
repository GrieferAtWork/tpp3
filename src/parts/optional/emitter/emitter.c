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
#ifndef GUARD_TPP_OPTIONAL_EMITTER_EMITTER_C
#define GUARD_TPP_OPTIONAL_EMITTER_EMITTER_C 1
#define TPP_BUILDING_OPTIONAL 1
#define TPP_EMITTER_BUILDING 1

#include "api.h"

#include "config.h"
#include "emitter.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given emitter.
 *
 * @param: output: Output printer. On error, must return one of `TPP_SSIZE_OFERR(*)`*/
TPP_IMPL TPP_NONNULL((1, 2)) void TPPCALL
tpp_emitter_init_after_lexer(tpp_emitter *tpp_restrict self,
                             tpp_formatprinter output) {
	self->te_output = output;
	tpp_emitter_state_init(&self->te_state);
#if TPP_EMITTER_HAVE_FEATURES
	tpp_emitter_features_init(&self->te_feat);
#endif /* TPP_EMITTER_HAVE_FEATURES */
#if TPP_EMITTER_MODE_HAVE_MULTIPLE
	self->te_mode = _TPP_EMITTER_MODE_DEFAULT;
#endif /* TPP_EMITTER_MODE_HAVE_MULTIPLE */
#if TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA > 0
	tpp_emitter_enable_reemit_unknown_pragma(self);
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA > 0 */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS > 0
	tpp_emitter_enable_reemit_macro_definitions(self);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS > 0 */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES > 0
	tpp_emitter_enable_reemit_include_directives(self);
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES > 0 */
}

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_emitter_fini_before_lexer(tpp_emitter *tpp_restrict self) {
	tpp_emitter_state_fini(&self->te_state);
#if TPP_EMITTER_HAVE_FEATURES
	tpp_emitter_features_fini(&self->te_feat);
#endif /* TPP_EMITTER_HAVE_FEATURES */
}



static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_emitter_printrepeat(tpp_emitter *tpp_restrict self,
                        tpp_size *tpp_restrict p_num_printed,
                        tpp_size count, tpp_char ch) {
	tpp_ssize temp, result = 0;
	tpp_char buffer[512];
	tpp_size bufmax = sizeof(buffer);
	if (bufmax > count)
		bufmax = count;
	tpp_memset(buffer, ch, bufmax * sizeof(tpp_char));
	*p_num_printed = 0;
	while (count) {
		temp = tpp_emitter_output_printraw(self, buffer, bufmax);
		if (temp < 0)
			goto err_temp;
		*p_num_printed += bufmax;
		result += temp;
		count -= bufmax;
		if (bufmax > count)
			bufmax = count;
	}
	return result;
err_temp:
	return temp;
}

/* Emit space characters (and update `self->te_state`) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_printspace(tpp_emitter *tpp_restrict self, tpp_column count) {
	tpp_size num_printed;
	tpp_ssize result = tpp_emitter_printrepeat(self, &num_printed, (tpp_size)count, ' ');
#if TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
	tpp_lcinfo_init(self->te_state.tes_curpos,
	                tpp_lcinfo_getline(self->te_state.tes_curpos),
	                tpp_lcinfo_getcol(self->te_state.tes_curpos) + num_printed);
#endif /* TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
	return result;
}

#if TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
#define tpp_emitter_getprinter(self) &_tpp_emitter_print
static TPP_FORMATPRINTER_DEFINE(_tpp_emitter_print, arg, text, num_bytes) {
	tpp_emitter *self = (tpp_emitter *)arg;
	tpp_ssize result = tpp_emitter_output_printraw(self, text, num_bytes);
	if (result >= 0) {
		self->te_state.tes_curpos = tpp_lcinfo_account_ex(self->te_state.tes_curpos, text, num_bytes,
		                                                  tpp_file_getencoding(tpp_lexer_getfile(tpp_emitter_getlexer(self))));
	}
	return result;
}
#else /* TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
#define tpp_emitter_getprinter(self) tpp_emitter_getoutput(self)
#endif /* !TPP_EMITTER_HAVE_MODE_EMIT || !TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
#define tpp_emitter_print(self, text, num_bytes) \
	tpp_formatprinter_print(tpp_emitter_getprinter(self), self, text, num_bytes)
#define tpp_emitter_print_cstr(self, cstr, num_bytes) \
	tpp_formatprinter_print_cstr(tpp_emitter_getprinter(self), self, cstr, num_bytes)
#define tpp_emitter_print_conststr(self, CONSTstr) \
	tpp_formatprinter_print_conststr(tpp_emitter_getprinter(self), self, CONSTstr)



/* Emit linefeed characters (and update `self->te_state`) */
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) || TPP_EMITTER_HAVE_NORMALIZE_LF
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_printlf(tpp_emitter *tpp_restrict self, tpp_line count) {
	tpp_size num_printed;
	tpp_ssize result = tpp_emitter_printrepeat(self, &num_printed, (tpp_size)count, '\n');
	tpp_lcinfo_init(self->te_state.tes_curpos,
	                tpp_lcinfo_getline(self->te_state.tes_curpos) + num_printed,
	                num_printed ? 0 : tpp_lcinfo_getcol(self->te_state.tes_curpos));
	return result;
}
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) || TPP_EMITTER_HAVE_NORMALIZE_LF */

#if TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
/* Emit a `#line` directive (and update `self->te_state`) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_print_line_directive(tpp_emitter *tpp_restrict self,
                                 tpp_line line, char const *filename,
                                 tpp_string *filename_str) {
	tpp_ssize temp, result = 0;
	char buffer[(sizeof("\n#line ") - sizeof(char)) + TPP_ITOA_MAXLEN + 2];
	char *ptr = buffer, *buf_temp;
	tpp_size partlen;
	tpp_column oldcol = tpp_lcinfo_getcol(self->te_state.tes_curpos);
	if (oldcol != 0)
		*ptr++ = '\n';
	*ptr++ = '#';
	*ptr++ = 'l';
	*ptr++ = 'i';
	*ptr++ = 'n';
	*ptr++ = 'e';
	*ptr++ = ' ';
	buf_temp = tpp_itoa(ptr, line + 1);
	partlen = (tpp_size)(ptr + TPP_ITOA_MAXLEN - buf_temp);
	tpp_memmovedown(ptr, buf_temp, partlen * sizeof(char));
	ptr += partlen;
	if (filename) {
		*ptr++ = ' ';
		*ptr++ = '"';
	} else {
		*ptr++ = '\n';
	}
	temp = tpp_emitter_output_printraw_cstr(self, buffer, (tpp_size)(ptr - buffer));
	if (temp < 0)
		goto err_temp;
	result += temp;
	if (filename) {
		partlen = tpp_strlen(filename);
		temp = tpp_emitter_output_printraw_cstr(self, filename, partlen);
		if (temp < 0) {
			tpp_lcinfo_init(self->te_state.tes_curpos,
			                tpp_lcinfo_getline(self->te_state.tes_curpos) + (oldcol ? 1 : 0),
			                (tpp_size)(ptr - buffer));
			goto err_temp;
		}
		result += temp;
		temp = tpp_emitter_output_printraw_conststr(self, "\"\n");
		if (temp < 0) {
			tpp_lcinfo_init(self->te_state.tes_curpos,
			                tpp_lcinfo_getline(self->te_state.tes_curpos),
			                (tpp_size)(ptr - buffer) + partlen);
			goto err_temp;
		}
		result += temp;

		/* Remember the newly assigned filename */
		self->te_state.tes_curfilename = filename;
		if (filename_str)
			tpp_string_incref(filename_str);
		if (self->te_state.tes_curfilename_str)
			tpp_string_decref(self->te_state.tes_curfilename_str);
		self->te_state.tes_curfilename_str = filename_str;
	}
	tpp_lcinfo_init(self->te_state.tes_curpos, line, 0);
	return result;
err_temp:
	return temp;
}
#endif /* TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */


#if TPP_EMITTER_HAVE_NORMALIZE_C_STRING
static TPP_FORMATPRINTER_DEFINE(tpp_emitter_print_encodestring, arg, text, num_bytes) {
	return tpp_token_encodestring(tpp_emitter_getprinter((tpp_emitter *)arg), arg, text, num_bytes);
}

#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
struct tpp_emitter_printbig_data {
	tpp_emitter *tepbd_emitter; /* Emitter */
	tpp_char     tepbd_quote;   /* Used "-character */
	bool         tepbd_after_x; /* True if after \x-sequence (meaning next regular byte mustn't be 0-9, a-f, A-F) */
};

static TPP_FORMATPRINTER_DEFINE(tpp_emitter_printbig_normal, arg, text, num_bytes) {
	tpp_ssize temp, result = 0;
	struct tpp_emitter_printbig_data *data;
	data = (struct tpp_emitter_printbig_data *)arg;
	if (!num_bytes)
		return 0;
	if (data->tepbd_after_x) {
		if (tpp_ascii_isxdigit(*text)) {
			tpp_char seq[sizeof("' '") - sizeof(char)];
			seq[0] = data->tepbd_quote;
			seq[1] = ' ';
			seq[2] = data->tepbd_quote;
			result = tpp_emitter_print(data->tepbd_emitter, seq, 3);
			if (result < 0)
				return result;
		}
		data->tepbd_after_x = false;
	}
	temp = tpp_emitter_print_encodestring(data->tepbd_emitter, text, num_bytes);
	if (temp < 0)
		return temp;
	result += temp;
	return result;
}

static tpp_ssize TPPCALL
tpp_emitter_printbig_big(void *arg, tpp_lexer *tpp_restrict lexer, tpp_uintmax value) {
#if TPP_UINTMAX_MAX <= TPP_UINTMAX_C(0xffff)
	tpp_char seq[sizeof("\\xFFFF") - sizeof(char)];
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(0xffffffff)
	tpp_char seq[sizeof("\\xFFFFFFFF") - sizeof(char)];
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(0xffffffffffffffff)
	tpp_char seq[sizeof("\\xFFFFFFFFFFFFFFFF") - sizeof(char)];
#else /* TPP_UINTMAX_MAX <= ... */
	tpp_char seq[sizeof("\\xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF") - sizeof(char)];
#endif /* TPP_UINTMAX_MAX < ... */
	tpp_char *seq_dst = seq + sizeof(seq);
	struct tpp_emitter_printbig_data *data;
	data = (struct tpp_emitter_printbig_data *)arg;
	do {
		tpp_char nibble = value & 0xf;
		*--seq_dst = tpp_ascii_ofuprxdigit(nibble);
		value >>= 4;
	} while (value);
	*--seq_dst = 'x';
	*--seq_dst = '\\';
	data->tepbd_after_x = true;
	(void)lexer;
	return tpp_emitter_print(data->tepbd_emitter, seq_dst, (tpp_size)(seq + sizeof(seq) - seq_dst));
}
#endif /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
#endif /* TPP_EMITTER_HAVE_NORMALIZE_C_STRING */


#if !TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS
#define tpp_emitter_print_keyword(self, text, num_bytes) \
	tpp_emitter_print((tpp_emitter *)(self), text, num_bytes)
#else /* TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */
static TPP_FORMATPRINTER_DEFINE(tpp_emitter_print_keyword, arg, text, num_bytes) {
	tpp_ssize temp, result = 0;
	tpp_emitter *self = (tpp_emitter *)arg;
	tpp_char const *end = text + num_bytes;
	tpp_char const *iter = text;
	bool is_first = true;
	if (iter >= end) {
		/* Special case: *empty* keyword */
		return tpp_emitter_print_conststr(self, "__TPP_IDENTIFIER(\"\")");
	}
	do {
		tpp_char const *uc_start = iter;
#if TPP_HAVE_UNICODE
		tpp_unichar uc = tpp_unicode_readutf8(&iter, end);
		if (is_first ? !tpp_unicode_issymstrt(uc)
		             : !tpp_unicode_issymcont(uc))
#else /* TPP_HAVE_UNICODE */
		tpp_char ch = *iter++;
		if (is_first ? !tpp_ascii_issymstrt(ch)
		             : !tpp_ascii_issymcont(ch))
#endif /* !TPP_HAVE_UNICODE */
		{
			/* Must escape! */
			tpp_char buf[sizeof("\\U12345678") - sizeof(char)], *dst = buf;
			temp = tpp_emitter_print(self, text, (tpp_size)(uc_start - text));
			if (temp < 0)
				goto err_temp;
			result += temp;
			*dst++ = '\\';
			*dst++ = 'U';
#if TPP_HAVE_UNICODE
			*dst++ = tpp_ascii_touprxdigit((uc & 0xf0000000) >> 28);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x0f000000) >> 24);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x00f00000) >> 20);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x000f0000) >> 16);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x0000f000) >> 12);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x00000f00) >> 8);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x000000f0) >> 4);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x0000000f));
#else /* TPP_HAVE_UNICODE */
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = tpp_ascii_touprxdigit((ch & 0xf0) >> 4);
			*dst++ = tpp_ascii_touprxdigit((ch & 0x0f));
#endif /* !TPP_HAVE_UNICODE */
			text = iter;
			temp = tpp_emitter_print(self, buf, sizeof(buf));
			if (temp < 0)
				goto err_temp;
			result += temp;
		}
		is_first = false;
	} while (iter < end);

	/* Flush remainder */
	temp = tpp_emitter_print(self, text, (tpp_size)(end - text));
	if (temp < 0)
		goto err_temp;
	result += temp;
	return result;
err_temp:
	return temp;
}
#endif /* TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */


#if !TPP_EMITTER_HAVE_NORMALIZE_BSE && !TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS
#define tpp_emitter_print_generic(self, text, num_bytes) \
	tpp_emitter_print((tpp_emitter *)(self), text, num_bytes)
#else /* !TPP_EMITTER_HAVE_NORMALIZE_BSE && !TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */
static TPP_FORMATPRINTER_DEFINE(tpp_emitter_print_generic, arg, text, num_bytes) {
	tpp_ssize temp, result = 0;
	tpp_emitter *self = (tpp_emitter *)arg;
	tpp_char const *end = text + num_bytes;
	tpp_char const *iter = text;
	while (iter < end) {
#if TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS
		if (iter[0] == '?' && (iter + 2) < end && iter[1] == '?' &&
		    tpp_emitter_has(self, NORMALIZE_TRIGRAPHS)) {
			tpp_char trich;
			switch (iter[2]) {
			case '=': trich = '#'; break;
			case '(': trich = '['; break;
			case '/': trich = '\\'; break;
			case ')': trich = ']'; break;
			case '\'': trich = '^'; break;
			case '<': trich = '{'; break;
			case '!': trich = '|'; break;
			case '>': trich = '}'; break;
			case '-': trich = '~'; break;
			default: goto not_a_trigraph;
			}
			temp = tpp_emitter_print(self, text, (tpp_size)(iter - text));
			if (temp < 0)
				goto err_temp;
			result += temp;
			temp = tpp_emitter_print(self, &trich, 1);
			if (temp < 0)
				goto err_temp;
			result += temp;
			iter += 3;
			text = iter;
		}
not_a_trigraph:
#endif /* TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */

#if TPP_EMITTER_HAVE_NORMALIZE_BSE
		if (tpp_emitter_has(self, NORMALIZE_BSE)) {
			tpp_char const *new_iter;
			new_iter = tpp_preparse_skipbse_fwd(tpp_emitter_getlexer(self), iter, end);
			tpp_assert(new_iter >= iter);
			if (new_iter > iter) {
				temp = tpp_emitter_print(self, text, (tpp_size)(iter - text));
				if (temp < 0)
					goto err_temp;
				result += temp;
				iter = text = new_iter;
			}
		}
#endif /* TPP_EMITTER_HAVE_NORMALIZE_BSE */
		++iter;
	}
	temp = tpp_emitter_print(self, text, (tpp_size)(end - text));
	if (temp < 0)
		goto err_temp;
	result += temp;
	return result;
err_temp:
	return temp;
}
#endif /* TPP_EMITTER_HAVE_NORMALIZE_BSE || TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */


/* Emit the currently loaded token (and update `self->te_state`) */
#if !TPP_EMITTER_HAVE_MODE_EMIT
#define tpp_emitter_print_current_token(self)                                      \
	tpp_emitter_print_generic(self,                                                \
	                          tpp_lexer_gettokenstart(tpp_emitter_getlexer(self)), \
	                          tpp_lexer_gettokenlen(tpp_emitter_getlexer(self)))
#else /* !TPP_EMITTER_HAVE_MODE_EMIT */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_print_current_token(tpp_emitter *tpp_restrict self) {
	tpp_lexer *const lexer = tpp_emitter_getlexer(self);
	tpp_char const *token_start = tpp_lexer_gettokenstart(lexer);
	tpp_char const *token_end = tpp_lexer_gettokenend(lexer);

	/* Configs to normalize certain tokens (see "emitter.h") */
	switch (tpp_lexer_gettok(lexer)) {

#if TPP_EMITTER_HAVE_NORMALIZE_SPACE
	case TPP_TOK_SPACE: {
		tpp_size space_count;
		if (!tpp_emitter_has(self, NORMALIZE_SPACE))
			break;
		space_count = 0;
		while (token_start < token_end) {
			tpp_char ch = *token_start++;
			(void)ch;
			++space_count;
#if TPP_HAVE_UNICODE
			if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(lexer)))
				token_start += tpp_unicode_utf8seqlen_mb_getmax(ch) - 1;
#endif /* TPP_HAVE_UNICODE */
			token_start = tpp_preparse_skipbse_fwd(lexer, token_start, token_end);
		}
		return tpp_emitter_printspace(self, space_count);
	}	break;
#endif /* TPP_EMITTER_HAVE_NORMALIZE_SPACE */

#if TPP_EMITTER_HAVE_NORMALIZE_LF
	case TPP_TOK_LF: {
		if (!tpp_emitter_has(self, NORMALIZE_LF))
			break;
		return tpp_emitter_printlf(self, 1);
	}	break;
#endif /* TPP_EMITTER_HAVE_NORMALIZE_LF */

#if TPP_EMITTER_HAVE_NORMALIZE_C_STRING
	TPP_CASE_TPP_TOK_STRING {
		tpp_lexer_decodestring_config config;
		tpp_ssize temp, result;
		tpp_char quote;
		if (!tpp_emitter_has(self, NORMALIZE_C_STRING))
			break;
#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
		if (TPP_TOK_ISSTRING_SQUOTE(tpp_lexer_gettok(lexer)) &&
		    tpp_lexer_has(lexer, BUILTIN_EXPR_CHARACTER_LITERALS)) {
			if (!tpp_lexer_has(lexer, TOK_C_CHAR))
				break;
			quote = '\'';
		} else
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
		{
			if (!tpp_lexer_has(lexer, TOK_C_STRING))
				break;
			quote = '"';
		}
		result = tpp_emitter_print(self, &quote, 1);
		if (result < 0)
			return result;

		/* Decode+re-encode string on-the-fly. */
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
		{
			struct tpp_emitter_printbig_data data;
			data.tepbd_emitter = self;
			data.tepbd_quote   = quote;
			data.tepbd_after_x = false;
			config.tldsc_arg = &data;
			config.tldsc_dataprinter = &tpp_emitter_printbig_normal;
#if TPP_HAVE_UNICODE
			config.tldsc_utf8printer = &tpp_emitter_printbig_normal;
#endif /* TPP_HAVE_UNICODE */
			config.tldsc_bigprinter = &tpp_emitter_printbig_big;
			temp = tpp_lexer_decodestring(lexer, &config);
		}
#else /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
		tpp_lexer_decodestring_config_init_simple(&config, &tpp_emitter_print_encodestring, self);
		temp = tpp_lexer_decodestring(lexer, &config);
#endif /* !TPP_HAVE_STRING_ESCAPE_BIGCHAR */
		temp = tpp_emitter_print(self, &quote, 1);
		if (temp < 0)
			return temp;
		result += temp;
		return result;
	}	break;
#endif /* TPP_EMITTER_HAVE_NORMALIZE_C_STRING */

#if TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS
	case '{':
	case '[':
	case '}':
	case ']':
	case '#': {
		tpp_char digraph_esc[1];
		if (!tpp_emitter_has(self, NORMALIZE_DIGRAPHS))
			break;
		digraph_esc[0] = (tpp_char)tpp_lexer_gettok(lexer);
		return tpp_emitter_print(self, digraph_esc, 1);
	}	break;

#if TPP_HAVE_TOK_POUND_POUND
	case TPP_TOK_POUND_POUND:
		if (!tpp_emitter_has(self, NORMALIZE_DIGRAPHS))
			break;
		return tpp_emitter_print_conststr(self, "##");
#endif /* TPP_HAVE_TOK_POUND_POUND */
#endif /* TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS */

	default: {
#if TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS
		if (TPP_TOK_ISKEYWORD(tpp_lexer_gettok(lexer))) {
			if (tpp_emitter_has(self, NORMALIZE_KEYWORDS)) {
				tpp_keyword const *const kwd = tpp_lexer_gettokenkwd(lexer);
				tpp_char const *kwd_start = tpp_keyword_getstr(kwd);
				tpp_size kwd_len = tpp_keyword_getlen(kwd);
				return tpp_emitter_print_keyword(self, kwd_start, kwd_len);
			}
		} else
#endif /* TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */
		{
		}
	}	break;
	}

	/* Do generic processing for this type of token */
	return tpp_emitter_print_generic(self, token_start, (tpp_size)(token_end - token_start));
}
#endif /* TPP_EMITTER_HAVE_MODE_EMIT */

#if TPP_EMITTER_HAVE_MODE_EMIT
static /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent_emit(tpp_emitter *tpp_restrict self) {
	tpp_ssize temp, result = 0;
	tpp_lexer const *const lexer = tpp_emitter_getlexer(self);
	tpp_token_id const tok = tpp_lexer_gettok(lexer);
	bool const require_whitespace = tpp_lexer_require_whitespace(lexer, self->te_state.tes_prevtok, tok);

	if (tpp_emitter_has(self, NOLINE)) {
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
emit_without_alignment:
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
		if (require_whitespace) {
			temp = tpp_emitter_printspace(self, 1);
			if (temp < 0)
				goto err_temp;
			result += temp;
		}
	} else {
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
		tpp_file *const lcfile = tpp_lexer_getlcfile(lexer);
		char const *const lc_filename = tpp_file_getfilename(lcfile);
		tpp_lcinfo const lcinfo = tpp_file_getstartlcinfo(lcfile);

		/* Don't align anything if there is no LC info (except to inject required whitespace) */
		if (!tpp_lcinfo_isvalid(lcinfo))
			goto emit_without_alignment;

		/* Check if the pointed-to filename changed, but its contents didn't. */
		if (self->te_state.tes_curfilename != lc_filename &&
		    self->te_state.tes_curfilename != NULL && lc_filename != NULL &&
		    tpp_strcmp(self->te_state.tes_curfilename, lc_filename) == 0) {
			tpp_string *new_filename_string = tpp_file_getfilenamestr(lcfile);
			if (new_filename_string)
				tpp_string_incref(new_filename_string);
			if (self->te_state.tes_curfilename_str)
				tpp_string_decref(self->te_state.tes_curfilename_str);
			self->te_state.tes_curfilename     = lc_filename;
			self->te_state.tes_curfilename_str = new_filename_string;
		}

		/* Check if stuff needs to be realigned (by injecting whitespace). */
		if (self->te_state.tes_curfilename != lc_filename ||
		    !tpp_lcinfo_equals(self->te_state.tes_curpos, lcinfo) ||
		    require_whitespace) {
			tpp_line oldline = tpp_lcinfo_getline(self->te_state.tes_curpos);
			tpp_line newline = tpp_lcinfo_getline(lcinfo);
			tpp_column oldcol = tpp_lcinfo_getcol(self->te_state.tes_curpos);
			tpp_column newcol = tpp_lcinfo_getcol(lcinfo);
			bool need_line_directive = false;

			/* TODO: Must also emit a directive if the system_header or
			 *       extern_c flags changed, and the `# <linenum>` is being used. */
			if (self->te_state.tes_curfilename != lc_filename) {
				need_line_directive = true;
			} else if (newline < oldline) {
				need_line_directive = true;
			} else if (newline > (oldline + 4)) { /* TODO: Config to specific this threshold (here: 4) */
				need_line_directive = true;
			} else if (newline == oldline) {
				if (newcol < oldcol) {
#if TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES
					if (lcfile != tpp_lexer_getfile(lexer) &&
					    tpp_file_ismacro(tpp_lexer_getfile(lexer)) &&
						tpp_emitter_has(self, RELAXED_MACRO_LINE_RULES)) {
						/* Inside of a macro -- so-as to prevent every token from causing
						 * another `#line`-directive being emitted, don't be too precise
						 * in terms of *all* tokens needing to have the proper column */
						if (require_whitespace)
							newcol = oldcol + 1;
					} else
#endif /* TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES */
					{
						need_line_directive = true;
					}
				} else if (newcol == oldcol && require_whitespace) {
					need_line_directive = true;
				}
			}
			if (need_line_directive) {
				char const *emit_filename = lc_filename;
				if (self->te_state.tes_curfilename == lc_filename)
					emit_filename = NULL;
				temp = tpp_emitter_print_line_directive(self, newline, emit_filename,
				                                        tpp_file_getfilenamestr(lcfile));
				if (temp < 0)
					goto err_temp;
				result += temp;
				oldline = tpp_lcinfo_getline(self->te_state.tes_curpos);
				oldcol  = tpp_lcinfo_getcol(self->te_state.tes_curpos);
			}

			/* Align with extra line-feed characters. */
			if (newline > oldline) {
				temp = tpp_emitter_printlf(self, (tpp_line)(newline - oldline));
				if (temp < 0)
					goto err_temp;
				result += temp;
				oldcol = tpp_lcinfo_getcol(self->te_state.tes_curpos);
				tpp_assert(oldcol == 0);
			}

			/* Align with extra space characters. */
			if (newcol > oldcol) {
				temp = tpp_emitter_printspace(self, (tpp_column)(newcol - oldcol));
				if (temp < 0)
					goto err_temp;
				result += temp;
				tpp_assert(tpp_lcinfo_getcol(self->te_state.tes_curpos) == newcol);
			}
		}
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
	}

	/* Actually emit the token */
	temp = tpp_emitter_print_current_token(self);
	if (temp < 0)
		goto err_temp;
	result += temp;

	/* Remember the last-emitted token */
	self->te_state.tes_prevtok = tok;
	return result;
err_temp:
	return temp;
}
#endif /* TPP_EMITTER_HAVE_MODE_EMIT */


/* Emit the token currently loaded into `tpp_emitter_getlexer(self)`,
 * and update the emitter's `te_state` accordingly
 *
 * @return: * :  Sum of return values of `tpp_emitter_getoutput(self)`
 * @return: < 0: First negative return value of `tpp_emitter_getoutput(self)` */
TPP_IMPL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent(tpp_emitter *tpp_restrict self) {
	switch (tpp_emitter_getmode(self)) {

#if TPP_EMITTER_HAVE_MODE_EMIT
	case TPP_EMITTER_MODE_EMIT:
		return tpp_emitter_emitcurrent_emit(self);
#endif /* TPP_EMITTER_HAVE_MODE_EMIT */

#if TPP_EMITTER_HAVE_MODE_DISPOSE
	case TPP_EMITTER_MODE_DISPOSE:
		/* Dispose tokens... */
		return 0;
#endif /* TPP_EMITTER_HAVE_MODE_DISPOSE */

	default: tpp_unreachable();
	}
	tpp_unreachable();
}

/* API support for (re-)emission of unknown `#pragma` directives */
#if TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_unknown_pragma(tpp_lexer *tpp_restrict lexer) {
	tpp_emitter *self = tpp_emitter_oflexer(lexer);
	tpp_ssize temp;
	tpp_token_id prev_token;
	tpp_token_id tok = tpp_lexer_gettok(lexer);
	if (tpp_lcinfo_getcol(self->te_state.tes_curpos) != 0) {
		temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			goto err_temp;
	}
	temp = tpp_emitter_print_conststr(self, "#pragma");
	if (temp < 0)
		goto err_temp;
	/* Print+yield tokens until the #pragma's end is reached */
	prev_token = TPP_KWD_pragma;
	while (tok != TPP_TOK_EOF) {
		if (!TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			/* Print token... */
			if (tpp_lexer_require_whitespace(lexer, prev_token, tok)) {
				temp = tpp_emitter_print_conststr(self, " ");
				if (temp < 0)
					goto err_temp;
			}
			temp = tpp_emitter_print_current_token(self);
			if (temp < 0)
				goto err_temp;
			prev_token = tok;
		}
		tok = tpp_lexer_yieldraw(lexer);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}
	temp = tpp_emitter_print_conststr(self, "\n");
	if (temp < 0)
		goto err_temp;
	self->te_state.tes_prevtok = TPP_TOK_EOF;
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */



/* API support for (re-)emission of `#define` and `#undef` directives */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_defined(tpp_lexer *tpp_restrict lexer,
                                tpp_keyword *tpp_restrict name,
                                tpp_macro *tpp_restrict macro) {
	tpp_emitter *self = tpp_emitter_oflexer(lexer);
	tpp_ssize temp;
	if (tpp_lcinfo_getcol(self->te_state.tes_curpos) != 0) {
		temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			goto err_temp;
	}
	temp = tpp_emitter_print_conststr(self, "#define ");
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print(self, tpp_keyword_getstr(name), tpp_keyword_getlen(name));
	if (temp < 0)
		goto err_temp;
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
	if (macro && !tpp_emitter_has(self, REEMIT_MACRO_DEFINITIONS_NAME_ONLY)) {
		/* Print function-like argument list. */
		if (tpp_macro_isfunction(macro)) {
			tpp_size i, argc = tpp_macro_getfuncargc(macro);
			tpp_char lparen = tpp_macro_getfunclparen(macro);
			tpp_char rparen = tpp_macro_getfuncrparen(macro);
			temp = tpp_emitter_print(self, &lparen, 1);
			if (temp < 0)
				goto err_temp;
			for (i = 0; i < argc; ++i) {
				tpp_token_id arg = tpp_macro_getfuncargtok(macro, i);
				tpp_keyword const *kwd;
				if (i != 0) {
					temp = tpp_emitter_print_conststr(self, ", ");
					if (temp < 0)
						goto err_temp;
				}
#if TPP_HAVE_VA_ARGS_IN_MACROS
				if ((i == argc - 1) && arg == TPP_KWD___VA_ARGS__)
					break;
#endif /* TPP_HAVE_VA_ARGS_IN_MACROS */
				kwd = tpp_lexer_kwds_getkeyword_byid(lexer, arg);
				if (kwd) {
					temp = tpp_emitter_print_keyword(self,
					                                 tpp_keyword_getstr(kwd),
					                                 tpp_keyword_getlen(kwd));
				} else { /* Should never get here */
					temp = tpp_emitter_print_conststr(self, "?");
				}
				if (temp < 0)
					goto err_temp;
			}
			if (tpp_macro_isvarargs(macro)) {
				temp = tpp_emitter_print_conststr(self, "...");
				if (temp < 0)
					goto err_temp;
			}
			temp = tpp_emitter_print(self, &rparen, 1);
			if (temp < 0)
				goto err_temp;
		}

		/* Print macro body... */
		if (tpp_macro_getbodylen(macro)) {
			temp = tpp_emitter_printspace(self, 1);
			if (temp < 0)
				goto err_temp;
			temp = tpp_emitter_print_generic(self,
			                                 tpp_macro_getbodystart(macro),
			                                 tpp_macro_getbodylen(macro));
			if (temp < 0)
				goto err_temp;
		}
	}
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */
	temp = tpp_emitter_print_conststr(self, "\n");
	if (temp < 0)
		goto err_temp;
	self->te_state.tes_prevtok = TPP_TOK_EOF;
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_undefined(tpp_lexer *tpp_restrict lexer,
                                  tpp_keyword *tpp_restrict name) {
	tpp_emitter *self = tpp_emitter_oflexer(lexer);
	tpp_ssize temp;
	if (tpp_lcinfo_getcol(self->te_state.tes_curpos) != 0) {
		temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			goto err_temp;
	}
	temp = tpp_emitter_print_conststr(self, "#undef ");
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print(self, tpp_keyword_getstr(name), tpp_keyword_getlen(name));
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print_conststr(self, "\n");
	if (temp < 0)
		goto err_temp;
	self->te_state.tes_prevtok = TPP_TOK_EOF;
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */



/* API support for (re-)emission of `#include` (and friends) directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_include_encountered(tpp_lexer *tpp_restrict lexer,
                                      tpp_hook_include_kind include_kind) {
	tpp_emitter *self = tpp_emitter_oflexer(lexer);
	tpp_ssize temp;
	if (tpp_lcinfo_getcol(self->te_state.tes_curpos) != 0) {
		temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			goto err_temp;
	}
	switch (include_kind) {
#if TPP_HAVE_CPP_INCLUDE
	case TPP_HOOK_INCLUDE_KIND_INCLUDE:
		temp = tpp_emitter_print_conststr(self, "#include ");
		break;
#endif /* TPP_HAVE_CPP_INCLUDE */
#if TPP_HAVE_CPP_INCLUDE_NEXT
	case TPP_HOOK_INCLUDE_KIND_INCLUDE_NEXT:
		temp = tpp_emitter_print_conststr(self, "#include_next ");
		break;
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
#if TPP_HAVE_CPP_IMPORT
	case TPP_HOOK_INCLUDE_KIND_IMPORT:
		temp = tpp_emitter_print_conststr(self, "#import ");
		break;
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_CPP_EMBED
	case TPP_HOOK_INCLUDE_KIND_EMBED:
		temp = tpp_emitter_print_conststr(self, "#embed ");
		break;
#endif /* TPP_HAVE_CPP_EMBED */
	default: tpp_unreachable();
	}
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print_generic(self,
	                                 tpp_lexer_gettokenstart(lexer),
	                                 tpp_lexer_gettokenlen(lexer));
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print_conststr(self, "\n");
	if (temp < 0)
		goto err_temp;
	self->te_state.tes_prevtok = TPP_TOK_EOF;
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_C */
