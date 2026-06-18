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
#ifndef GUARD_TPP_LEXER_SEEK_RPAREN_C
#define GUARD_TPP_LEXER_SEEK_RPAREN_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "error.h"
#include "string.h"
#include "lexer.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_SEEKPP_RPAREN

#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
#define TPP_RECURSION_CC_PAREN   0
#define TPP_RECURSION_CC_BRACKET 1
#define TPP_RECURSION_CC_BRACE   2
#define TPP_RECURSION_CC_ANGLE   3

static TPP_CONSTCALL TPP_WUNUSED unsigned int TPPCALL
tpp_get_recursion_cc(tpp_token_id lparen_kind) {
	switch (lparen_kind) {
	case TPP_TOK_OFCHAR('('):
		return TPP_RECURSION_CC_PAREN;
	case TPP_TOK_OFCHAR('['):
		return TPP_RECURSION_CC_BRACKET;
	case TPP_TOK_OFCHAR('{'):
		return TPP_RECURSION_CC_BRACE;
	case TPP_TOK_OFCHAR('<'):
		return TPP_RECURSION_CC_ANGLE;
	default: tpp_unreachable();
	}
}
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
#define TPP_RECURSION_CC_PAREN 0
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */


typedef struct tpp_seek_rparen_state {
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	/* Recursively nested ( ), [ ], { }, < > -pairs (in that order)
	 * The recursion level at any index can only be altered if all
	 * recursion levels with lower indices are set to 0:
	 * >> #define ARR[x] #x
	 * >> ARR[foo(])bar([)baz]
	 * Expands to 1 token:  "foo(])bar([)baz"
	 * rather than:         "foo("   )   bar   (   [   )   baz   ]   ) */
	tpp_ssize    tsrps_recursion[4];
	unsigned int tsrps_cc;
#define tpp_seek_rparen_state_init(self, lparen_kind) \
	(void)((self)->tsrps_recursion[0] = 0,            \
	       (self)->tsrps_recursion[1] = 0,            \
	       (self)->tsrps_recursion[2] = 0,            \
	       (self)->tsrps_recursion[3] = 0,            \
	       (self)->tsrps_cc = tpp_get_recursion_cc(lparen_kind))
#define tpp_seek_rparen_state_getcc(self) ((self)->tsrps_cc)
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	tpp_ssize tsrps_recursion[1]; /* Recursively nested ( )-pairs */
#define tpp_seek_rparen_state_init(self, lparen_kind) \
	(void)((self)->tsrps_recursion[0] = 0)
#define tpp_seek_rparen_state_getcc(self) TPP_RECURSION_CC_PAREN
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */

#if TPP_HAVE_INCLUDE_STACK
	tpp_file       *tsrps_curfile_saved_prev;     /* [0..1] Saved "tf_prev" of current file */
#endif /* TPP_HAVE_INCLUDE_STACK */
	tpp_size        tsrps_curfile_saved_keep;     /* Saved "ttf_keep" of current file, relative to current "ttf_keep", or (tpp_size)-1 if there was no keep-pointer */
	tpp_size        tsrps_curfile_saved_tpos_rel; /* Saved "tf_tpos" of current file, relative to current "ttf_keep" */
	tpp_size        tsrps_curfile_saved_pos_rel;  /* Saved "tf_pos" of current file, relative to current "ttf_keep" */

	/* Already-parsed text that must be prepended before the current argument. */

	/* TODO: Come up with a smart way of tracking debug info for custom printed arguments
	 *       -> need to be able to track lcinfo for custom char ranges (any range of chars
	 *          from this string must be able to map to its own file/line/col triple)
	 *       -> also must adjust tpp_file_lcinfo() to support this, and somehow also
	 *          incorporate tpp_file_filename()/tpp_file_userfilename() to support
	 *          different filenames based on char position
	 * where this is necessary:
	 * >> #define foo(a) a a
	 * >> foo(
	 * >> #include "file1.txt"   // Contains 10
	 * >> #include "file2.txt"   // Contains 20
	 * >> )
	 *
	 * Must result in 4 tokens (not accounting for whitespace/linefeed tokens):
	 * - file1.txt:1:1: 10
	 * - file2.txt:1:1: 20
	 * - file1.txt:1:1: 10
	 * - file2.txt:1:1: 20
	 */
	tpp_string_builder tsrps_curarg_prefix;
#define tpp_seek_rparen_state_init_curarg(self) \
	tpp_string_builder_init(&(self)->tsrps_curarg_prefix)
#define tpp_seek_rparen_state_fini_curarg(self)               \
	tpp_string_builder_fini(&(self)->tsrps_curarg_prefix)
#define tpp_seek_rparen_state_curarg_append(self, data, size) \
	(tpp_string_builder_print(&(self)->tsrps_curarg_prefix, data, size) >= 0)
} tpp_seek_rparen_state;


TPP_INLINE TPP_NONNULL((1, 2)) void TPPCALL
tpp_seek_rparen_state_save_curfile(tpp_seek_rparen_state *tpp_restrict self,
                                   tpp_lexer *tpp_restrict lexer) {
	tpp_file *const file = tpp_lexer_getfile(lexer);
	tpp_char const *keep = file->tf_data.td_io.ttf_keep;
	tpp_char const *tpos = file->tf_tpos;
	if (!keep) {
		self->tsrps_curfile_saved_keep = (tpp_size)-1;
		file->tf_data.td_io.ttf_keep   = tpos;
		keep = tpos;
	} else if (keep <= tpos) {
		self->tsrps_curfile_saved_keep = 0;
	} else {
		self->tsrps_curfile_saved_keep = (tpp_size)(keep - tpos);
		file->tf_data.td_io.ttf_keep   = tpos;
		keep = tpos;
	}
	self->tsrps_curfile_saved_tpos_rel = (tpp_size)(tpos - keep);
	self->tsrps_curfile_saved_pos_rel = (tpp_size)(file->tf_pos - keep);
#if TPP_HAVE_INCLUDE_STACK
	self->tsrps_curfile_saved_prev = file->tf_prev;
	file->tf_prev = NULL;
#endif /* TPP_HAVE_INCLUDE_STACK */
}

TPP_INLINE TPP_NONNULL((1, 2)) void TPPCALL
tpp_seek_rparen_state_rstr_curfile(tpp_seek_rparen_state *tpp_restrict self,
                                   tpp_lexer *tpp_restrict lexer,
                                   bool restore_pos) {
	tpp_file *const file = tpp_lexer_getfile(lexer);
#if TPP_HAVE_INCLUDE_STACK
	file->tf_prev = self->tsrps_curfile_saved_prev;
#endif /* TPP_HAVE_INCLUDE_STACK */
	file->tf_tpos = tpp_file_keep_rel2ptr(file, self->tsrps_curfile_saved_tpos_rel);
	if (restore_pos)
		file->tf_pos = tpp_file_keep_rel2ptr(file, self->tsrps_curfile_saved_pos_rel);
	if (self->tsrps_curfile_saved_keep != (tpp_size)-1) {
		file->tf_data.td_io.ttf_keep += self->tsrps_curfile_saved_keep;
	} else {
		file->tf_data.td_io.ttf_keep = NULL;
	}
}

#define tpp_lexer_arginfo_isrelative(self) \
	((self)->tlai_chunk == (TPP_REF tpp_string *)-1)
#define tpp_lexer_arginfo_rel2ptr(self, file)                                              \
	(void)((self)->tlai_start = tpp_file_keep_rel2ptr(file, (tpp_size)(self)->tlai_start), \
	       (self)->tlai_end   = tpp_file_keep_rel2ptr(file, (tpp_size)(self)->tlai_end))
#define tpp_lexer_arginfo_ptr2rel(self, file)                                                      \
	(void)((self)->tlai_start = (tpp_char const *)tpp_file_keep_ptr2rel(file, (self)->tlai_start), \
	       (self)->tlai_end   = (tpp_char const *)tpp_file_keep_ptr2rel(file, (self)->tlai_end))


/* Seek the first unmatched ')'-token, whilst collecting information
 * about every ','-separated text-area encountered until then.
 *
 * NOTES:
 *  - This function is used to parse the argument list for user-defined
 *    macros, as well as a couple of built-in macros.
 *  - This function preserves the effective "tf_tpos" (aka. tpp_token_getstart())
 *    of the final output file (and when using "tpp_lexer_manualpopfile_start":
 *    all intermediate popped files also)
 *
 * @param: p_argv: [out]    Output buffer for the bounds of macro
 *                          arguments encountered along the way.
 *                          The size of this buffer is IN(*p_argc)
 * @param: p_argc: [in/out] In:  Size of provided "p_argv" buffer (in elements)
 *                          Out: Number of arguments actually encountered. May
 *                               be set to a number greater tha IN(*p_argc), in
 *                               which case the last argument (IN(*p_argc) - 1)
 *                               is treated as a varargs argument. Unless the
 *                               `TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS' flag is
 *                               given in this case, this also causes a warning
 *                               to be emitted.
 * @param: p_rollback_pos:  [out] Set to the position that the current file should
 *                                be rewound to for the sake of performing a rollback
 * @param: opt_function_name_for_messages:
 *                          Function name for warning messages
 * @param: flags:           Set of `TPP_LEXER_SEEK_RPAREN_FLAG_*'
 *
 * @return: TPP_TOK_EOF:         EOF was encountered before an unmatched ')' was found
 * @return: TPP_TOK_RPAREN:      Unmatched closing ')' was encountered
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen_ex(tpp_lexer *tpp_restrict self,
                           tpp_lexer_arginfo *tpp_restrict p_argv,
                           tpp_size *tpp_restrict p_argc,
                           tpp_char const **p_rollback_pos,
                           char const *opt_function_name_for_messages,
                           unsigned int flags, tpp_token_id lparen_kind)
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen(tpp_lexer *tpp_restrict self,
                        tpp_lexer_arginfo *tpp_restrict p_argv,
                        tpp_size *tpp_restrict p_argc,
                        tpp_char const **p_rollback_pos,
                        char const *opt_function_name_for_messages,
                        unsigned int flags)
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
{
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
#define tpp_lexer_seekpp_rparen_keepspace() (flags & TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC)
#else /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */
#define tpp_lexer_seekpp_rparen_keepspace() (TPP_HAVE_MACRO_ARGUMENT_WHITESPACE != 0)
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE >= 0 */
	tpp_size const argv_bufsize = *p_argc;
	tpp_size argc = 0;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_token_id result;
	tpp_seek_rparen_state state;
	tpp_lexer_state_flags saved_lexer_state;
	tpp_size curarg_rel_start; /* Start of current argument (relative to current file's KEEP) */
	tpp_size curarg_rel_end;   /* End of current argument (relative to current file's KEEP) */
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0
	tpp_size curarg_rel_rend;  /* End of current argument without trailing whitespace */
#define tpp_set_curarg_rel_rend(v) (curarg_rel_end = curarg_rel_rend = (v))
#define tpp_get_curarg_rel_rend()  curarg_rel_rend
#else /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0 */
#define tpp_set_curarg_rel_rend(v) (curarg_rel_end = (v))
#define tpp_get_curarg_rel_rend()   curarg_rel_end
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE > 0 */
	saved_lexer_state = self->tl_state;
	self->tl_state |= TPP_LEXER_STATE_FLAG_ALLTOKENS;
	tpp_seek_rparen_state_init(&state, lparen_kind);
	tpp_seek_rparen_state_save_curfile(&state, self);
	tpp_seek_rparen_state_init_curarg(&state);

	/* NOTES:
	 * - Elements of "p_argv" are:
	 *   - Only initialized in the range [0,min(argv_bufsize-1, argc))
	 *   - When tpp_lexer_arginfo_isrelative() is true, "tlai_chunk",
	 *     "tlai_enc" and "tlai_lcinfo" are all undefined, and the
	 *     "tlai_start"+"tlai_end" fields are actually relative offsets
	 *     to the current file's keep-pointer
	 *   - When tpp_lexer_arginfo_isrelative() is false, the argument
	 *     is initialized as per the specifications from "lexer.h"
	 *
	 * - "curarg_rel_start" / "curarg_rel_end" specify additional text
	 *   that needs to be appended to the current argument
	 */

	result = token->tt_id;
	curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_end);
	tpp_set_curarg_rel_rend(curarg_rel_start);
again_yield_and_switch_tok:
	result = tpp_lexer_yieldpp_blocking(self);
again_switch_tok:
	if (file->tf_prev != NULL && !TPP_TOK_ISERR(result)) {
		/* Extra file was pushed -- include all tokens as-is and don't scan for arguments */
		if (curarg_rel_start < tpp_get_curarg_rel_rend()) { /* Save argument text */
			tpp_char const *data;
			tpp_size num_bytes = (tpp_size)(tpp_get_curarg_rel_rend() - curarg_rel_start);
			tpp_file *curfile = file->tf_prev;
			while (curfile->tf_prev)
				curfile = curfile->tf_prev;
			data = tpp_file_keep_rel2ptr(curfile, curarg_rel_start);
			if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
				goto err_nomem;
		}
		for (;;) {
			if (!tpp_seek_rparen_state_curarg_append(&state,
			                                         tpp_token_getstart(token),
			                                         tpp_token_getlen(token)))
				goto err_nomem;
			result = tpp_lexer_yieldpp_blocking(self);
			if (TPP_TOK_ISERR(result))
				break;
		} while (file->tf_prev != NULL);
		curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_start);
		tpp_set_curarg_rel_rend(curarg_rel_start);
	}

	switch (result) {

	case TPP_TOK_SPACE:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT {
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
		if (tpp_lexer_seekpp_rparen_keepspace())
			break; /* When whitespace should be kept: treat it like a regular token */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0
		if (state.tsrps_curarg_prefix.tsb_len == 0 &&
		    curarg_rel_start == curarg_rel_rend) {
			/* Skip leading whitespace... */
			curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_end);
			tpp_set_curarg_rel_rend(curarg_rel_start);
			goto again_yield_and_switch_tok;
		}
		if (curarg_rel_end != tpp_file_keep_ptr2rel(file, token->tt_start)) {
			tpp_assert(curarg_rel_rend >= curarg_rel_start);
			if (curarg_rel_rend > curarg_rel_start) {
				tpp_size num_bytes = (tpp_size)(curarg_rel_rend - curarg_rel_start);
				tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
				if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
					goto err_nomem;
			}
			curarg_rel_start = curarg_rel_rend;
		}
		curarg_rel_end = tpp_file_keep_ptr2rel(file, token->tt_end);
		goto again_yield_and_switch_tok;
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0 */
	}	break;

	case TPP_TOK_EOF:
#if TPP_HAVE_INCLUDE_STACK
		if (state.tsrps_curfile_saved_prev && file->tf_kind != TPP_FILE_KIND_IO) {
			/* Make already-encoded arguments absolute */
			tpp_size i, written_argc = argc;
			if (written_argc >= argv_bufsize) {
				written_argc = argv_bufsize;
				if (written_argc)
					--written_argc;
			}
			for (i = 0; i < written_argc; ++i) {
				tpp_lexer_arginfo *arg = &p_argv[i];
				if (tpp_lexer_arginfo_isrelative(arg)) {
					tpp_lexer_arginfo_rel2ptr(arg, file);
					arg->tlai_chunk = file->tf_chunk;
					if (arg->tlai_chunk)
						tpp_string_incref(arg->tlai_chunk);
				}
			}

#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
			if (tpp_lexer_seekpp_rparen_keepspace()) {
				curarg_rel_rend = curarg_rel_end;
			} else
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */
			{
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0
				/* FIXME: Have to print text until "curarg_rel_end" (iow: including whitespace)
				 *        if follow-up files contain tokens that also have to be appended (the
				 *        trailing whitespace of the current (old) file must only be trimmed if
				 *        the argument at the very start of the next file (possibly after being
				 *        preceded by some more whitespace)) */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0 */
			}

			if (curarg_rel_start < tpp_get_curarg_rel_rend()) { /* Save argument text */
				tpp_size num_bytes   = (tpp_size)(tpp_get_curarg_rel_rend() - curarg_rel_start);
				tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
				if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
					goto err_nomem;
			}

			/* Continue with next file */
			tpp_seek_rparen_state_rstr_curfile(&state, self, true);
			tpp_assert(file->tf_prev == state.tsrps_curfile_saved_prev);
			tpp_assert(file->tf_prev);
#if TPP_HAVE_LEXER_MANUALPOPFILE
			if (flags & TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK) {
				/* Special case: use an alternate (rollback-capable) mechanism to pop files. */
				tpp_lexer_manualpopfile_popfile(self);
			} else
#endif /* TPP_HAVE_LEXER_MANUALPOPFILE */
			{
				tpp_lexer_popfile(self);
			}
			tpp_seek_rparen_state_save_curfile(&state, self);
			result = tpp_lexer_yieldpp_blocking(self);
			curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_start);
			tpp_set_curarg_rel_rend(curarg_rel_start);
			goto again_switch_tok;
		} else
#endif /* TPP_HAVE_INCLUDE_STACK */
#if TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST
		if (!(flags & TPP_LEXER_SEEK_RPAREN_FLAG_NOWARNEOF)) {
			tpp_errno error;
			tpp_char const *pos = file->tf_data.td_io.ttf_keep + state.tsrps_curfile_saved_tpos_rel;
			error = tpp_lexer_warnf_at(self, pos, TPP_W_EOF_IN_ARGUMENT_LIST,
			                           opt_function_name_for_messages);
			if (TPP_ISERR(error)) {
				result = TPP_TOK_OFERR(error);
				goto err_result;
			}
		} else
#endif /* TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST */
		{
		}
		goto done;

	case '(':
		++state.tsrps_recursion[0];
		break;

	case ')':
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] == 0) {
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
			if (tpp_seek_rparen_state_getcc(&state) == TPP_RECURSION_CC_PAREN)
#endif /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
			{
				goto done;
			}
		}
		--state.tsrps_recursion[TPP_RECURSION_CC_PAREN];
		break;

#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	case '[':
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_BRACKET)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		++state.tsrps_recursion[TPP_RECURSION_CC_BRACKET];
		break;
	case ']':
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_BRACKET)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] == 0)
			goto done;
		--state.tsrps_recursion[TPP_RECURSION_CC_BRACKET];
		break;

	case '{':
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_BRACE)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		++state.tsrps_recursion[TPP_RECURSION_CC_BRACE];
		break;
	case '}':
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_BRACE)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACE] == 0)
			goto done;
		--state.tsrps_recursion[TPP_RECURSION_CC_BRACE];
		break;

#if (TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE ||              \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE ||       \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS ||               \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE ||        \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE ||        \
     TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE ||        \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE)
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	case TPP_TOK_LANGLE_EQUAL: /* "<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE: /* "<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_EQUAL: /* "<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE_LANGLE: /* "<<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: /* "<<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
	case TPP_TOK_LANGLE_MINUS: /* "<-" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	case TPP_TOK_LANGLE_EQUAL_RANGLE: /* "<=>" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	case TPP_TOK_LANGLE_MINUS_RANGLE: /* "<->" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	case TPP_TOK_LANGLE_EQUAL_LANGLE: /* "<=<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	case TPP_TOK_LANGLE_MINUS_LANGLE: /* "<-<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
		/* Convert to "<" token */
		tpp_assert(token->tt_start < token->tt_end);
		tpp_assert(token->tt_start[0] == '<');
		token->tt_end = token->tt_start + 1;
/*		result = TPP_TOK_OFCHAR('<');  * Not necessary */
/*		token->tt_id = result;         * Not necessary */
		goto handle_langle;
#define WANT_handle_langle
		break;
#endif /* ... */



#if (TPP_HAVE_TPP_TOK_RANGLE_LANGLE ||              \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||              \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE ||       \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS ||               \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE)
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	case TPP_TOK_RANGLE_LANGLE: /* "><" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE: /* ">>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	case TPP_TOK_RANGLE_EQUAL: /* ">=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_EQUAL: /* ">>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE_RANGLE: /* ">>>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: /* ">>>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS
	case TPP_TOK_RANGLE_MINUS: /* ">-" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	case TPP_TOK_RANGLE_EQUAL_LANGLE: /* ">=<" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	case TPP_TOK_RANGLE_MINUS_LANGLE: /* ">-<" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	case TPP_TOK_RANGLE_EQUAL_RANGLE: /* ">=>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	case TPP_TOK_RANGLE_MINUS_RANGLE: /* ">->" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
		/* Convert to ">" token */
		tpp_assert(token->tt_start < token->tt_end);
		tpp_assert(token->tt_start[0] == '>');
		token->tt_end = token->tt_start + 1;
		result = TPP_TOK_OFCHAR('>');
		token->tt_id = result;
		goto handle_rangle;
#define WANT_handle_rangle
		break;
#endif /* ... */

	/* Tokens where the first character is irrelevant, and need to be split into 2 */
#if (TPP_HAVE_TPP_TOK_MINUS_RANGLE ||               \
     TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR ||          \
     TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS ||          \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE ||               \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE || \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE ||               \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE || \
     TPP_HAVE_TPP_TOK_MINUS_LANGLE)
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	case TPP_TOK_MINUS_RANGLE: /* "->" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	case TPP_TOK_MINUS_RANGLE_STAR: /* "->*" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	case TPP_TOK_STAR_LANGLE_MINUS: /* "*<-" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	case TPP_TOK_EQUAL_LANGLE: /* "=<" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE: /* "=<<" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE: /* "=<<<" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	case TPP_TOK_EQUAL_RANGLE: /* "=>" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE: /* "=>>" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE: /* "=>>>" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_LANGLE
	case TPP_TOK_MINUS_LANGLE: /* "-<" */
#endif /* !TPP_HAVE_TPP_TOK_MINUS_LANGLE */
		/* Convert to 1-char token */
		tpp_assert(token->tt_start < token->tt_end);
		token->tt_end = token->tt_start + 1;
/*		result = TPP_TOK_OFCHAR(token->tt_start[0]); * Not necessary */
/*		token->tt_id = result;                       * Not necessary */
		break;
#endif /* ... */

	case '<':
#ifdef WANT_handle_langle
#undef WANT_handle_langle
handle_langle:
#endif /* WANT_handle_langle */
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_ANGLE)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		++state.tsrps_recursion[TPP_RECURSION_CC_ANGLE];
		break;

	case '>':
#ifdef WANT_handle_rangle
#undef WANT_handle_rangle
handle_rangle:
#endif /* WANT_handle_rangle */
		if (tpp_seek_rparen_state_getcc(&state) < TPP_RECURSION_CC_ANGLE)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_ANGLE] == 0)
			goto done;
		--state.tsrps_recursion[TPP_RECURSION_CC_ANGLE];
		break;

#endif /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */


	case ',': {
		/* Make sure that we're not inside of a nested ( )-pair */
		if (state.tsrps_recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		if (state.tsrps_recursion[TPP_RECURSION_CC_ANGLE] != 0)
			break;
#endif /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */

#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
		if (tpp_lexer_seekpp_rparen_keepspace())
			curarg_rel_rend = curarg_rel_end; /* Argument must includes whitespace preceding the ","-token */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */

		/* Write next argument */
		if (argc < (argv_bufsize - 1)) {
			tpp_lexer_arginfo *arg = &p_argv[argc];
			if (state.tsrps_curarg_prefix.tsb_len) {
				if (curarg_rel_start < tpp_get_curarg_rel_rend()) { /* Save argument text */
					tpp_size num_bytes   = (tpp_size)(tpp_get_curarg_rel_rend() - curarg_rel_start);
					tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
					if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
						goto err_nomem;
				}
				arg->tlai_chunk = tpp_string_builder_pack(&state.tsrps_curarg_prefix);
				arg->tlai_start = tpp_string_str(arg->tlai_chunk);
				arg->tlai_end   = tpp_string_end(arg->tlai_chunk);
				tpp_seek_rparen_state_init_curarg(&state);
			} else {
				arg->tlai_chunk = (TPP_REF tpp_string *)-1; /* Indicate that this is a relative argument (fixed later) */
				arg->tlai_start = (tpp_char const *)curarg_rel_start;
				arg->tlai_end   = (tpp_char const *)tpp_get_curarg_rel_rend();
			}
			curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_end);
			tpp_set_curarg_rel_rend(curarg_rel_start);
			++argc;
			goto again_yield_and_switch_tok;
		}
		++argc;
	}	break;

	default:
		if (TPP_TOK_ISERR(result))
			goto err_result;
		break;
	}
	if (curarg_rel_end != tpp_file_keep_ptr2rel(file, token->tt_start)) {
		tpp_assert(curarg_rel_end >= curarg_rel_start);
		if (curarg_rel_end > curarg_rel_start) {
			tpp_size num_bytes = (tpp_size)(curarg_rel_end - curarg_rel_start);
			tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
			if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
				goto err_nomem;
		}
		curarg_rel_start = tpp_file_keep_ptr2rel(file, token->tt_start);
	}
	tpp_set_curarg_rel_rend(tpp_file_keep_ptr2rel(file, token->tt_end));
	goto again_yield_and_switch_tok;


done:
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
	if (tpp_lexer_seekpp_rparen_keepspace())
		curarg_rel_rend = curarg_rel_end; /* Argument must includes trailing whitespace */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */
	if (argc || (state.tsrps_curarg_prefix.tsb_len ||
	             tpp_get_curarg_rel_rend() > curarg_rel_start)) {
		/* Write last argument */
		tpp_lexer_arginfo *arg;
		if (argc < argv_bufsize) {
			arg = &p_argv[argc];
		} else if (argv_bufsize) {
			/* Update end-pointer of last argument (for varargs) */
			arg = &p_argv[argv_bufsize - 1];
		} else {
			++argc;
			goto done_after_last_arg;
		}
		++argc;
		if (state.tsrps_curarg_prefix.tsb_len) {
			if (tpp_get_curarg_rel_rend() > curarg_rel_start) {
				tpp_size num_bytes   = (tpp_size)(tpp_get_curarg_rel_rend() - curarg_rel_start);
				tpp_char const *data = tpp_file_keep_rel2ptr(file, curarg_rel_start);
				if (!tpp_seek_rparen_state_curarg_append(&state, data, num_bytes))
					goto err_nomem;
			}
			arg->tlai_chunk = tpp_string_builder_pack(&state.tsrps_curarg_prefix);
			arg->tlai_start = tpp_string_str(arg->tlai_chunk);
			arg->tlai_end   = tpp_string_end(arg->tlai_chunk);
			tpp_seek_rparen_state_init_curarg(&state);
		} else {
			arg->tlai_chunk = (TPP_REF tpp_string *)-1; /* Indicate that this is a relative argument (fixed later) */
			arg->tlai_start = (tpp_char const *)curarg_rel_start;
			arg->tlai_end   = (tpp_char const *)tpp_get_curarg_rel_rend();
		}
	}
done_after_last_arg:
#if TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS
	if (argc > argv_bufsize && !(flags & TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS)) {
		tpp_errno error;
		tpp_char const *pos = file->tf_data.td_io.ttf_keep + state.tsrps_curfile_saved_tpos_rel;
		error = tpp_lexer_warnf_at(self, pos, TPP_W_TOO_MANY_ARGUMENTS,
		                           opt_function_name_for_messages,
		                           (unsigned int)argv_bufsize,
		                           (unsigned int)argc);
		if (TPP_ISERR(error))
			result = TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS */
err_result:
	*p_argc = argc;
	if (argc > argv_bufsize)
		argc = argv_bufsize;
	if tpp_unlikely(TPP_TOK_ISERR(result)) {
		tpp_size i;
		if (argc)
			--argc; /* Last argument is initialized at the very end */
		for (i = 0; i < argc; ++i) {
			tpp_lexer_arginfo *arg = &p_argv[i];
			if (!tpp_lexer_arginfo_isrelative(arg))
				tpp_lexer_arginfo_fini(arg);
		}
	} else {
		tpp_size i;
		for (i = 0; i < argc; ++i) {
			tpp_lexer_arginfo *arg = &p_argv[i];
			if (tpp_lexer_arginfo_isrelative(arg)) {
				tpp_lexer_arginfo_rel2ptr(arg, file);
				arg->tlai_chunk = file->tf_chunk;
				if (arg->tlai_chunk)
					tpp_string_incref(arg->tlai_chunk);
			}
		}
	}
	*p_rollback_pos = tpp_file_keep_rel2ptr(file, state.tsrps_curfile_saved_pos_rel);
	tpp_seek_rparen_state_rstr_curfile(&state, self, false);
	tpp_seek_rparen_state_fini_curarg(&state);
	self->tl_state = saved_lexer_state;
	return result;
err_nomem:
	result = TPP_TOK_ENOMEM;
	goto err_result;
#undef tpp_set_curarg_rel_rend
#undef tpp_lexer_seekpp_rparen_keepspace
}



/* Same as above, but always initializes *exactly* "argc" arguments,
 * and automatically emits "TPP_W_TOO_FEW_ARGUMENTS" when fewer were
 * parsed. */
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen_exact_ex(tpp_lexer *tpp_restrict self,
                                 tpp_lexer_arginfo *tpp_restrict p_argv, tpp_size argc,
                                 char const *opt_function_name_for_messages,
                                 unsigned int flags, tpp_token_id lparen_kind)
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen_exact(tpp_lexer *tpp_restrict self,
                              tpp_lexer_arginfo *tpp_restrict p_argv, tpp_size argc,
                              char const *opt_function_name_for_messages,
                              unsigned int flags)
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
{
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_token_id result;
	tpp_size argc_actual = argc;
	tpp_char const *rollback_pos;
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	result = tpp_lexer_seekpp_rparen_ex(self, p_argv, &argc_actual, &rollback_pos,
	                                    opt_function_name_for_messages, flags,
	                                    lparen_kind);
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	result = tpp_lexer_seekpp_rparen(self, p_pos, p_argv, &argc_actual, &rollback_pos,
	                                 opt_function_name_for_messages, flags);
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	if (!TPP_TOK_ISERR(result) && argc_actual < argc) {
		tpp_size i;
#if TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS
		tpp_errno error;
		error = tpp_lexer_warnf(self, TPP_W_TOO_FEW_ARGUMENTS,
		                        opt_function_name_for_messages,
		                        (unsigned int)argc,
		                        (unsigned int)argc_actual);
		if (TPP_ISERR(error))
			result = TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */
		for (i = argc_actual; i < argc; ++i) {
			p_argv[i].tlai_start = rollback_pos;
			p_argv[i].tlai_end   = rollback_pos;
			p_argv[i].tlai_chunk = file->tf_chunk;
			if (file->tf_chunk)
				tpp_string_incref(file->tf_chunk);
		}
	}
	return result;
}

#endif /* TPP_HAVE_LEXER_SEEKPP_RPAREN */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_SEEK_RPAREN_C */
