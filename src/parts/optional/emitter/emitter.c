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
	(void)0 _tpp_emitter_init_feat(self);
#if TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA > 0
	tpp_emitter_enable_reemit_unknown_pragma(self);
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA > 0 */
}

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_emitter_fini_before_lexer(tpp_emitter *tpp_restrict self) {
	tpp_emitter_state_fini(&self->te_state);
	(void)0 _tpp_emitter_fini_feat(self);
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

/* Emit linefeed characters (and update `self->te_state`) */
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_printlf(tpp_emitter *tpp_restrict self, tpp_line count) {
	tpp_size num_printed;
	tpp_ssize result = tpp_emitter_printrepeat(self, &num_printed, (tpp_size)count, '\n');
	tpp_lcinfo_init(self->te_state.tes_curpos,
	                tpp_lcinfo_getline(self->te_state.tes_curpos) + num_printed,
	                num_printed ? 0 : tpp_lcinfo_getcol(self->te_state.tes_curpos));
	return result;
}
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */

/* Emit space characters (and update `self->te_state`) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_printspace(tpp_emitter *tpp_restrict self, tpp_column count) {
	tpp_size num_printed;
	tpp_ssize result = tpp_emitter_printrepeat(self, &num_printed, (tpp_size)count, ' ');
	tpp_lcinfo_init(self->te_state.tes_curpos,
	                tpp_lcinfo_getline(self->te_state.tes_curpos),
	                tpp_lcinfo_getcol(self->te_state.tes_curpos) + num_printed);
	return result;
}

#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
/* Emit a `#line` directive (and update `self->te_state`) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_printline(tpp_emitter *tpp_restrict self,
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
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */

static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_print(tpp_emitter *tpp_restrict self,
                  tpp_char const *text, tpp_size len) {
	tpp_ssize result = tpp_emitter_output_printraw(self, text, len);
	if (result >= 0) {
		self->te_state.tes_curpos = tpp_lcinfo_account_ex(self->te_state.tes_curpos, text, len,
		                                                  tpp_file_getencoding(tpp_lexer_getfile(tpp_emitter_getlexer(self))));
	}
	return result;
}

/* Emit a `#line` directive (and update `self->te_state`) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_printtoken(tpp_emitter *tpp_restrict self,
                       tpp_lexer const *tpp_restrict lexer) {
	tpp_char const *token_start = tpp_lexer_gettokenstart(lexer);
	tpp_size token_len = tpp_lexer_gettokenlen(lexer);
	/* TODO: Config to normalize certain tokens (see "emitter.h") */
	return tpp_emitter_print(self, token_start, token_len);
}

/* Emit the token currently loaded into `tpp_emitter_getlexer(self)`,
 * and update the emitter's `te_state` accordingly
 *
 * @return: * :  Sum of return values of `tpp_emitter_getoutput(self)`
 * @return: < 0: First negative return value of `tpp_emitter_getoutput(self)` */
TPP_IMPL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent(tpp_emitter *tpp_restrict self) {
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
				temp = tpp_emitter_printline(self, newline, emit_filename,
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
	temp = tpp_emitter_printtoken(self, lexer);
	if (temp < 0)
		goto err_temp;
	result += temp;

	/* Remember the last-emitted token */
	self->te_state.tes_prevtok = tok;
	return result;
err_temp:
	return temp;
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
		temp = tpp_emitter_print(self, (tpp_char const *)"\n", 1);
		if (temp < 0)
			goto err_temp;
	}
	temp = tpp_emitter_print(self, (tpp_char const *)"#pragma", 7);
	if (temp < 0)
		goto err_temp;
	/* Print+yield tokens until the #pragma's end is reached */
	prev_token = TPP_KWD_pragma;
	while (tok != TPP_TOK_EOF) {
		if (!TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			/* Print token... */
			if (tpp_lexer_require_whitespace(lexer, prev_token, tok)) {
				temp = tpp_emitter_print(self, (tpp_char const *)" ", 1);
				if (temp < 0)
					goto err_temp;
			}
			temp = tpp_emitter_printtoken(self, lexer);
			if (temp < 0)
				goto err_temp;
			prev_token = tok;
		}
		tok = tpp_lexer_yieldraw(lexer);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}
	temp = tpp_emitter_print(self, (tpp_char const *)"\n", 1);
	if (temp < 0)
		goto err_temp;
	self->te_state.tes_prevtok = TPP_TOK_EOF;
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_C */
