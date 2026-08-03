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
#ifndef GUARD_TPP_EMITTER_AMALGAMATION_C
#define GUARD_TPP_EMITTER_AMALGAMATION_C 1

#ifndef TPP_EMITTER_AMALGAMATION_H
#define TPP_EMITTER_AMALGAMATION_H "tpp-emitter-amalgamation.h"
#endif /* !TPP_EMITTER_AMALGAMATION_H */

/* Include "tpp-emitter-amalgamation.h" header... */
#ifndef GUARD_TPP_EMITTER_AMALGAMATION_H
#undef TPP_EMITTER_BUILDING
#define TPP_EMITTER_BUILDING 1
#include TPP_EMITTER_AMALGAMATION_H
#endif /* !GUARD_TPP_EMITTER_AMALGAMATION_H */

#if !TPP_EMITTER_BUILDING
/************************************************************************/
/* File: parts/optional/emitter/expose-internals.h                      */
/************************************************************************/
/* If "tpp-emitter-amalgamation.h" was already included, re-define
 * `TPP_EMITTER_INTERNAL()` identifers to their unescaped names. */
#define tcl_emitter                             TPP_EMITTER_INTERNAL(tcl_emitter)
#define tcl_state                               TPP_EMITTER_INTERNAL(tcl_state)
#define tcl_flags                               TPP_EMITTER_INTERNAL(tcl_flags)
#define te_feat                                 TPP_EMITTER_INTERNAL(te_feat)
#define tef_flags                               TPP_EMITTER_INTERNAL(tef_flags)
#define tetf_bitset                             TPP_EMITTER_INTERNAL(tetf_bitset)
#define teff_NORMALIZE_SPACE                    TPP_EMITTER_INTERNAL(teff_NORMALIZE_SPACE)
#define teff_NORMALIZE_LF                       TPP_EMITTER_INTERNAL(teff_NORMALIZE_LF)
#define teff_NORMALIZE_C_STRING                 TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_STRING)
#define teff_NORMALIZE_KEYWORDS                 TPP_EMITTER_INTERNAL(teff_NORMALIZE_KEYWORDS)
#define teff_NORMALIZE_BSE                      TPP_EMITTER_INTERNAL(teff_NORMALIZE_BSE)
#define teff_NORMALIZE_TRIGRAPHS                TPP_EMITTER_INTERNAL(teff_NORMALIZE_TRIGRAPHS)
#define teff_NORMALIZE_DIGRAPHS                 TPP_EMITTER_INTERNAL(teff_NORMALIZE_DIGRAPHS)
#define teff_NOLINE                             TPP_EMITTER_INTERNAL(teff_NOLINE)
#define teff_RELAXED_MACRO_LINE_RULES           TPP_EMITTER_INTERNAL(teff_RELAXED_MACRO_LINE_RULES)
#define teff_REEMIT_MACRO_DEFINITIONS_LAZY      TPP_EMITTER_INTERNAL(teff_REEMIT_MACRO_DEFINITIONS_LAZY)
#define teff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY TPP_EMITTER_INTERNAL(teff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#define teff_TRACE_INCLUDES                     TPP_EMITTER_INTERNAL(teff_TRACE_INCLUDES)
#define tes_curpos                              TPP_EMITTER_INTERNAL(tes_curpos)
#define tes_curfilename                         TPP_EMITTER_INTERNAL(tes_curfilename)
#define tes_curfilename_str                     TPP_EMITTER_INTERNAL(tes_curfilename_str)
#define tes_prevtok                             TPP_EMITTER_INTERNAL(tes_prevtok)
#define te_lexer                                TPP_EMITTER_INTERNAL(te_lexer)
#define te_output                               TPP_EMITTER_INTERNAL(te_output)
#define te_state                                TPP_EMITTER_INTERNAL(te_state)
#define te_mode                                 TPP_EMITTER_INTERNAL(te_mode)

#endif /* !TPP_EMITTER_BUILDING */

/************************************************************************/
/* File: parts/optional/emitter/emitter-features.c                      */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_EMITTER_HAVE_FEATURES
TPP_CONST_IMPL tpp_emitter_features const tpp_emitter_features_default = {
	/* .tef_flags = */ {
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
		/* .teff_NORMALIZE_SPACE                    = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_SPACE),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF)
		/* .teff_NORMALIZE_LF                       = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_LF),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
		/* .teff_NORMALIZE_C_STRING                 = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
		/* .teff_NORMALIZE_KEYWORDS                 = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
		/* .teff_NORMALIZE_BSE                      = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_BSE),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
		/* .teff_NORMALIZE_TRIGRAPHS                = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
		/* .teff_NORMALIZE_DIGRAPHS                 = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
		/* .teff_NOLINE                             = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NOLINE),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES)
		/* .teff_RELAXED_MACRO_LINE_RULES           = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
		/* .teff_REEMIT_MACRO_DEFINITIONS_LAZY      = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
		/* .teff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
		/* .teff_TRACE_INCLUDES                     = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_TRACE_INCLUDES),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
	}
};
#endif /* TPP_EMITTER_HAVE_FEATURES */

/************************************************************************/
/* File: parts/optional/emitter/emitter.c                               */
/************************************************************************/

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
#if (TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) || \
     TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_TRACE_INCLUDES))
	_tpp_emitter_enable_file_pushed_hook(self); /* Must be turned on by default */
#endif /* ... */
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
		return tpp_emitter_printspace(self, (tpp_column)space_count);
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
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS || TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_dump_define(tpp_emitter *tpp_restrict self,
                        tpp_keyword const *tpp_restrict name,
                        tpp_macro const *tpp_restrict macro) {
	tpp_ssize temp;
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
				kwd = tpp_lexer_kwds_getkeyword_byid(tpp_emitter_getlexer(self), arg);
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

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_dump_undef(tpp_emitter *tpp_restrict self,
                       tpp_keyword const *tpp_restrict name) {
	tpp_ssize temp;
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
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS || TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_defined(tpp_lexer *tpp_restrict lexer,
                                tpp_keyword *tpp_restrict name,
                                tpp_macro *tpp_restrict macro) {
	tpp_ssize temp;
	tpp_emitter *self = tpp_emitter_oflexer(lexer);
	if (tpp_lcinfo_getcol(self->te_state.tes_curpos) != 0) {
		temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			goto err_temp;
	}
	return tpp_emitter_dump_define(self, name, macro);
err_temp:
	return TPP_SSIZE_ASERR(temp);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_undefined(tpp_lexer *tpp_restrict lexer,
                                  tpp_keyword *tpp_restrict name) {
	tpp_ssize temp;
	tpp_emitter *self = tpp_emitter_oflexer(lexer);
	if (tpp_lcinfo_getcol(self->te_state.tes_curpos) != 0) {
		temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			goto err_temp;
	}
	return tpp_emitter_dump_undef(self, name);
err_temp:
	return TPP_SSIZE_ASERR(temp);
}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */



/* API support for (re-)emission of `#include` (and friends) directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_include_encountered(tpp_lexer *tpp_restrict lexer,
                                      tpp_hook_include_kind include_kind) {
	tpp_emitter *const self = tpp_emitter_oflexer(lexer);
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


#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || TPP_EMITTER_HAVE_TRACE_INCLUDES
#if TPP_EMITTER_HAVE_TRACE_INCLUDES
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_trace_include_printdots(tpp_lexer *tpp_restrict lexer, tpp_size count) {
	tpp_char buffer[512];
	tpp_size bufmax = sizeof(buffer);
	if (bufmax > count)
		bufmax = count;
	tpp_memset(buffer, '.', bufmax * sizeof(tpp_char));
	while (count) {
		tpp_ssize temp = tpp_lexer_callhook_mesgprinter(lexer, buffer, bufmax);
		if (temp < 0)
			return TPP_SSIZE_ASERR(temp);
		count -= bufmax;
		if (bufmax > count)
			bufmax = count;
	}
	return TPP_EOK;
}

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_trace_include(tpp_emitter *self, tpp_file const *file) {
	tpp_ssize temp;
	tpp_errno error;
	tpp_size depth = 1;
	tpp_file const *iter = file;
	char const *filename = tpp_file_getrealfilename(file);
	tpp_size filename_len;
	while ((iter = tpp_file_getprev(iter)) != NULL) {
		if (tpp_file_getkind(iter) == TPP_FILE_KIND_IO)
			++depth;
	}
	error = tpp_emitter_trace_include_printdots(tpp_emitter_getlexer(self), depth);
	if (TPP_ISERR(error))
		return error;
	temp = tpp_lexer_callhook_mesgprinter(tpp_emitter_getlexer(self),
	                                      (tpp_char const *)" ", 1);
	if (temp < 0)
		return TPP_SSIZE_ASERR(temp);
	if (filename == NULL)
		filename = "?";
	filename_len = tpp_strlen(filename);
	temp = tpp_lexer_callhook_mesgprinter(tpp_emitter_getlexer(self),
	                                      (tpp_char const *)filename, filename_len);
	if (temp < 0)
		return TPP_SSIZE_ASERR(temp);
	temp = tpp_lexer_callhook_mesgprinter(tpp_emitter_getlexer(self),
	                                      (tpp_char const *)"\n", 1);
	if (temp < 0)
		return TPP_SSIZE_ASERR(temp);
	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_TRACE_INCLUDES */

#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
static void TPPCALL tpp_emitter_reemit_macro_used_dtor(void *ptr) {
	TPP_REF tpp_macro *macro = (TPP_REF tpp_macro *)ptr;
	tpp_macro_decref(macro);
}

/* Handle "macro" being used by being pushed onto the `#include`-stack. */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_reemit_macro_used(tpp_emitter *tpp_restrict self,
                              tpp_macro *tpp_restrict macro) {
	tpp_errno error;
	tpp_ssize temp;
	tpp_macro *last_definition = NULL;
	tpp_keyword *name = tpp_macro_getname(macro);
	if (!name)
		return TPP_EOK;
	if (tpp_keyword_getuserdata_dtor(name) == &tpp_emitter_reemit_macro_used_dtor)
		last_definition = (tpp_macro *)tpp_keyword_getuserdata_dtor(name);
	if (last_definition == macro)
		return TPP_EOK; /* Already emitted! */

	/* Must emit a new definition! */
	if (tpp_lcinfo_getcol(self->te_state.tes_curpos) != 0) {
		temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			goto err_temp;
	}
	if (last_definition) {
		error = tpp_emitter_dump_undef(self, name);
		if (TPP_ISERR(error))
			return error;
	}
	error = tpp_emitter_dump_define(self, name, macro);
	if (TPP_ISERR(error))
		return error;

	/* Remember the current definition within the keyword. */
	error = tpp_keyword_setuserdata(name, macro, &tpp_emitter_reemit_macro_used_dtor, true);
	if (TPP_ISERR(error))
		return error;
	tpp_macro_incref(macro); /* Extra reference stored in user-data of keyword "name" */
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_file_pushed(tpp_lexer *tpp_restrict lexer) {
	tpp_emitter *const self = tpp_emitter_oflexer(lexer);
	tpp_file const *const file = tpp_lexer_getfile(lexer);

	/* Deal with include tracing */
#if TPP_EMITTER_HAVE_TRACE_INCLUDES
	if (tpp_file_getkind(file) == TPP_FILE_KIND_IO) {
		if (tpp_emitter_has(self, TRACE_INCLUDES)) {
			tpp_errno error = tpp_emitter_trace_include(self, file);
			if (TPP_ISERR(error))
				return error;
		}
	}
#endif /* TPP_EMITTER_HAVE_TRACE_INCLUDES */

	/* Deal with lazy macro definitions */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
	if (tpp_file_ismacro(file)) {
		if (tpp_emitter_has(self, REEMIT_MACRO_DEFINITIONS_LAZY)) {
			tpp_macro *macro = tpp_file_getmacro(file);
			return tpp_emitter_reemit_macro_used(self, macro);
		}
	}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || TPP_EMITTER_HAVE_TRACE_INCLUDES */






/* Emit the token currently loaded into `tpp_emitter_getlexer(self)`,
 * and update the emitter's `te_state` accordingly
 *
 * @return: * :  Sum of return values of `tpp_emitter_getoutput(self)`
 * @return: < 0: First negative return value of `tpp_emitter_getoutput(self)` */
TPP_IMPL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent(tpp_emitter *tpp_restrict self) {

	/* Check if current token is a keyword with a linked macro. */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
	if (tpp_lexer_hastokenkwd(tpp_emitter_getlexer(self)) &&
	    tpp_emitter_has(self, REEMIT_MACRO_DEFINITIONS_LAZY)) {
		tpp_keyword const *keyword = tpp_lexer_gettokenkwd(tpp_emitter_getlexer(self));
		if (!tpp_keyword_hasmacro(keyword) &&
		    tpp_keyword_getuserdata_dtor(keyword) == &tpp_emitter_reemit_macro_used_dtor) {
			tpp_errno error;
			/* Must emit an #undef directive */
			if (tpp_lcinfo_getcol(self->te_state.tes_curpos) != 0) {
				tpp_ssize temp = tpp_emitter_print_conststr(self, "\n");
				if (temp < 0)
					return temp;
			}
			error = tpp_emitter_dump_undef(self, keyword);
			if (TPP_ISERR(error))
				return error;
			error = tpp_keyword_setuserdata((tpp_keyword *)keyword, NULL, NULL, true);
			if (TPP_ISERR(error))
				return TPP_SSIZE_OFERR(error);
		}
	}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

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

/************************************************************************/
/* File: parts/optional/emitter/emitter-cli.c                           */
/************************************************************************/

#if TPP_EMITTER_HAVE_CLI

#if TPP_CONF_ISEXT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_enable_TOK_SPACE(self) tpp_lexer_enableextension(self, TPP_EXT_TOK_SPACE)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_enable_TOK_SPACE(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TOK_SPACE), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TOK_SPACE(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_LF)
#define tpp_lexer_enable_TOK_LF(self) tpp_lexer_enableextension(self, TPP_EXT_TOK_LF)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_LF)
#define tpp_lexer_enable_TOK_LF(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TOK_LF), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TOK_LF(self) TPP_EOK
#endif /* !... */


/* Define a function `tpp_emitter_cli_warnf()` */
#undef TPP_HAVE_EMITTER_CLI_WARN
#define TPP_HAVE_EMITTER_CLI_WARN \
	(TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT)

#if TPP_HAVE_EMITTER_CLI_WARN
static TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_warnf(tpp_emitter *tpp_restrict self, tpp_char const *token_start,
                      tpp_size token_size, tpp_warning_id id, ...) {
	/* XXX: Use of `TPP_INTERNAL` here isn't allowed -- come up with
	 *      APIs to allow users to do this stuff without needing to
	 *      access TPP internal! */
	tpp_lexer *const lexer = tpp_emitter_getlexer(self);
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
#endif /* TPP_HAVE_EMITTER_CLI_WARN */


#if TPP_EMITTER_HAVE_CLI_NO_LINE_COMMANDS
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_no_line_commands(tpp_emitter_cli_loader *tpp_restrict self) {
	tpp_errno result;
	(void)self;
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_NOLINE)
	tpp_emitter_enablefeature(self->tcl_emitter, TPP_EMITTER_FEAT_NOLINE);
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_NOLINE) */

	/* Enable emission of SPACE/LF tokens */
	result = tpp_lexer_enable_TOK_SPACE(tpp_emitter_getlexer(self->tcl_emitter));
	if (!TPP_ISERR(result))
		result = tpp_lexer_enable_TOK_LF(tpp_emitter_getlexer(self->tcl_emitter));
	return result;
}
#endif /* TPP_EMITTER_HAVE_CLI_NO_LINE_COMMANDS */


#if TPP_EMITTER_HAVE_CLI_TRACE_INCLUDES
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_trace_includes(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
	tpp_emitter_enable_trace_includes(self->tcl_emitter);
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_NOLINE) */
	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_TRACE_INCLUDES */


#if TPP_EMITTER_HAVE_CLI_DUMP_M
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_M(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Set flag to dump definitions of builtin/predefined macros later. */
	self->tcl_flags |= _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M;

	/* Turn on re-emission of additional macros */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
	tpp_emitter_enable_reemit_macro_definitions(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

	/* Set emitter mode to "TPP_EMITTER_MODE_DISPOSE" */
#if TPP_EMITTER_HAVE_MODE_DISPOSE
	tpp_emitter_setmode(self->tcl_emitter, TPP_EMITTER_MODE_DISPOSE);
#endif /* TPP_EMITTER_HAVE_MODE_DISPOSE */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_M */


#if TPP_EMITTER_HAVE_CLI_DUMP_D
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_D(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Set flag to dump definitions of builtin/predefined macros later. */
	self->tcl_flags |= _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M;

	/* Turn on re-emission of additional macros */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
	tpp_emitter_enable_reemit_macro_definitions(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_D */


#if TPP_EMITTER_HAVE_CLI_DUMP_N
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_N(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Set flag to dump definitions of builtin/predefined macros later. */
	self->tcl_flags |= _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M;

	/* Turn on re-emission of additional macros */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
	tpp_emitter_enable_reemit_macro_definitions(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

	/* *only* print the name of macros in `#define` directives */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
	tpp_emitter_enablefeature(self->tcl_emitter, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_NAME_ONLY);
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_N */


#if TPP_EMITTER_HAVE_CLI_DUMP_I
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_I(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Turn on re-emission of #include-directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
	tpp_emitter_enable_reemit_include_directives(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_I */


#if TPP_EMITTER_HAVE_CLI_DUMP_U
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_U(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Turn on re-emission of #include-directives */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
	tpp_emitter_enable_reemit_macro_definitions_lazy(self->tcl_emitter);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_U */


#undef TPP_EMITTER_HAVE_CLI_DUMP
#define TPP_EMITTER_HAVE_CLI_DUMP   \
	(TPP_EMITTER_HAVE_CLI_DUMP_M || \
	 TPP_EMITTER_HAVE_CLI_DUMP_D || \
	 TPP_EMITTER_HAVE_CLI_DUMP_N || \
	 TPP_EMITTER_HAVE_CLI_DUMP_I || \
	 TPP_EMITTER_HAVE_CLI_DUMP_U)

#if TPP_EMITTER_HAVE_CLI_DUMP
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump(tpp_emitter_cli_loader *tpp_restrict self,
                            tpp_char what) {
	switch (what) {

#if TPP_EMITTER_HAVE_CLI_DUMP_M
	case 'M':
		return tpp_emitter_cli_enable_dump_M(self);
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_M */

#if TPP_EMITTER_HAVE_CLI_DUMP_D
	case 'D':
		return tpp_emitter_cli_enable_dump_D(self);
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_D */

#if TPP_EMITTER_HAVE_CLI_DUMP_N
	case 'N':
		return tpp_emitter_cli_enable_dump_N(self);
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_N */

#if TPP_EMITTER_HAVE_CLI_DUMP_I
	case 'I':
		return tpp_emitter_cli_enable_dump_I(self);
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_I */

#if TPP_EMITTER_HAVE_CLI_DUMP_U
	case 'U':
		return tpp_emitter_cli_enable_dump_U(self);
#endif /* TPP_EMITTER_HAVE_CLI_DUMP_U */

	default: break;
	}
	return TPP_ENOENT;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dumps(tpp_emitter_cli_loader *tpp_restrict self,
                             char const *whats) {
	tpp_errno result = TPP_EOK;
	for (;;) {
		char what = *whats++;
		if (what == '\0')
			break;
		result = tpp_emitter_cli_enable_dump(self, what);
		if (TPP_ISERR(result))
			break;
	}
	return result;
}
#endif /* TPP_EMITTER_HAVE_CLI_DUMP */


/* Feed an argument to the loader. How exactly the argument is parsed
 * depends on the loader's current state, but sufficed to say: in its
 * default/initial state, `arg` is a CLI argument as you'd expect.
 *
 * WARNING: When you call this function, you must guaranty that `arg` remains
 *          valid, allocated, and unaltered until `tpp_emitter_cli_loader_fini()` is
 *          called.
 *
 * @return: TPP_EOK:        Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:     SOFT_ERROR: Argument could not be understood (but no
 *                          warning was emitted). You must either handle it yourself
 *                          by treating it as an argument for *your* compiler's
 *                          CLI, or as an input file for the emitter, or emit a
 *                          warning informing the user that their CLI argument
 *                          was not understood. You should also probably try to
 *                          pass it to `tpp_cli_loader_parsearg()`.
 * @return: TPP_ENOMEM:     HARD_ERROR: Out of memory
 * @return: TPP_EIO:        HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR:  HARD_ERROR: A emitter error was thrown
 * @return: TPP_EWARNPRINT: HARD_ERROR: An error happened within a warning printer */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parsearg(tpp_emitter_cli_loader *tpp_restrict self, char const *arg) {
#define tpp_streq(at, CONSTstr) \
	(tpp_memcmp(at, CONSTstr, sizeof(CONSTstr) - sizeof(char)) == 0)
	switch (self->tcl_state) {

	case TPP_EMITTER_CLI_LOADER_STATE_NORMAL: {
		if (*arg++ != '-')
			break; /* All arguments must start with at least 1 "-" */
		switch (*arg++) {

		case '-':
			/* --foo argument */
			switch (*arg++) {

			case '\0':
				self->tcl_state = TPP_EMITTER_CLI_LOADER_STATE_DDASH; /* -- */
				return TPP_EOK;

			case 'n':
#if TPP_EMITTER_HAVE_CLI_NO_LINE_COMMANDS
				if (tpp_streq(arg, "o-line-commands\0")) {
					return tpp_emitter_cli_enable_no_line_commands(self);
				} else
#endif /* TPP_EMITTER_HAVE_CLI_NO_LINE_COMMANDS */
				{
				}
				break;

			case 't':
#if TPP_EMITTER_HAVE_CLI_TRACE_INCLUDES
				if (tpp_streq(arg, "race-includes\0")) {
					return tpp_emitter_cli_enable_trace_includes(self);
				} else
#endif /* TPP_EMITTER_HAVE_CLI_TRACE_INCLUDES */
				{
				}
				break;

			case 'd':
#if TPP_EMITTER_HAVE_CLI_DUMP
				if (tpp_streq(arg, "ump=")) {
					arg += sizeof("ump=") - sizeof(char);
					if (*arg)
						return tpp_emitter_cli_enable_dumps(self, arg);
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DUMP */
				{
				}
				break;

			default: break;
			}
			break;

		case 'P':
#if TPP_EMITTER_HAVE_CLI_NO_LINE_COMMANDS
			if (*arg == '\0')
				return tpp_emitter_cli_enable_no_line_commands(self);
#endif /* TPP_EMITTER_HAVE_CLI_NO_LINE_COMMANDS */
			break;

		case 'H':
#if TPP_EMITTER_HAVE_CLI_TRACE_INCLUDES
			if (*arg == '\0')
				return tpp_emitter_cli_enable_trace_includes(self);
#endif /* TPP_EMITTER_HAVE_CLI_TRACE_INCLUDES */
			break;

		case 'd':
#if TPP_EMITTER_HAVE_CLI_DUMP
			if (*arg)
				return tpp_emitter_cli_enable_dumps(self, arg);
#endif /* TPP_EMITTER_HAVE_CLI_DUMP */
			break;

		default: break;
		}
	}	break;

	case TPP_EMITTER_CLI_LOADER_STATE_DDASH:
		break; /* Don't accept any more arguments after having encountered a "--" arguments */

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
 * @return: TPP_ENOMEM:     HARD_ERROR: Out of memory
 * @return: TPP_EIO:        HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR:  HARD_ERROR: A emitter error was thrown
 * @return: TPP_EWARNPRINT: HARD_ERROR: An error happened within a warning printer */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parseflag(tpp_emitter_cli_loader *tpp_restrict self, char const **p_arg) {
	char const *arg = *p_arg;
	char flag = *arg++;
	(void)self;
	(void)flag;
	(void)arg;
	switch (flag) {

#if TPP_EMITTER_HAVE_CLI_NO_LINE_COMMANDS
	case 'P': {
		*p_arg = arg;
		return tpp_emitter_cli_enable_no_line_commands(self);
	}	break;
#endif /* TPP_EMITTER_HAVE_CLI_NO_LINE_COMMANDS */

#if TPP_EMITTER_HAVE_CLI_TRACE_INCLUDES
	case 'H': {
		*p_arg = arg;
		return tpp_emitter_cli_enable_trace_includes(self);
	}	break;
#endif /* TPP_EMITTER_HAVE_CLI_TRACE_INCLUDES */

	default: break;
	}
	return TPP_ENOENT;
}



/* Convenience wrapper around `tpp_emitter_cli_loader_parsearg()`.
 * For more information, see `tpp_cli_loader_parseargv()`.
 *
 * @return: TPP_EOK:        Success (`*p_argc` and `*p_argv` were updated such that
 *                          they contain all unrecognized arguments, as well as all
 *                          input files for the emitter).
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O Error
 * @return: TPP_ELEXERROR:  A emitter error was thrown
 * @return: TPP_EWARNPRINT: An error happened within a warning printer */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parseargv(tpp_emitter_cli_loader *tpp_restrict self,
                                 int *p_argc, char ***p_argv) {
	tpp_errno result = TPP_EOK;
	int argc    = *p_argc;
	char **argv = *p_argv;
	unsigned int unknown_count = 0;
	while (argc > 0) {
		char *arg = argv[0];
		result = tpp_emitter_cli_loader_parsearg(self, arg);
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
		if (tpp_emitter_cli_loader_hasddash(self)) {
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

/* Ensure that `self` is in a *normal* state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_emitter_cli_loader_getemitter(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the `#include`-stack.
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O Error
 * @return: TPP_ELEXERROR:  A emitter error was thrown
 * @return: TPP_EWARNPRINT: An error happened within a warning printer */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_loader_flush(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Emit a warning if the CLI loader isn't in a neutral state */
#if TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT
	if (self->tcl_state != TPP_EMITTER_CLI_LOADER_STATE_NORMAL &&
	    self->tcl_state != TPP_EMITTER_CLI_LOADER_STATE_DDASH) {
		tpp_errno error = tpp_emitter_cli_warnf(self->tcl_emitter, NULL, 0,
		                                        TPP_W_MISSING_CLI_ARGUMENT);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT */

#if (TPP_EMITTER_HAVE_CLI_DUMP_M || TPP_EMITTER_HAVE_CLI_DUMP_D || TPP_EMITTER_HAVE_CLI_DUMP_N) && TPP_HAVE_LEXER_DUMP_DEFINITIONS
	if (self->tcl_flags & _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M) {
		tpp_ssize temp;
		temp = tpp_lexer_dump_definitions(tpp_emitter_getlexer(self->tcl_emitter),
		                                  tpp_emitter_getoutput(self->tcl_emitter),
		                                  self->tcl_emitter,
		                                  TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS);
		if (temp < 0)
			return TPP_SSIZE_ASERR(temp);
	}
#endif /* (TPP_EMITTER_HAVE_CLI_DUMP_M || TPP_EMITTER_HAVE_CLI_DUMP_D || TPP_EMITTER_HAVE_CLI_DUMP_N) && TPP_HAVE_LEXER_DUMP_DEFINITIONS */

	return TPP_EOK;
}

#endif /* TPP_EMITTER_HAVE_CLI */

TPP_DECL_END

#endif /* !GUARD_TPP_EMITTER_AMALGAMATION_C */
/* clang-format on */
