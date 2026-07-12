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
#ifndef GUARD_TPP_LEXER_C
#define GUARD_TPP_LEXER_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "token.h"
#include "features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* Assert that config macros work */
TPP_STATIC_ASSERT(TPP_CONF_1 && TPP_CONF_MAYBE_1(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_0 && !TPP_CONF_MAYBE_1(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_EXT1 && TPP_CONF_MAYBE_1(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(TPP_CONF_EXT0 && TPP_CONF_MAYBE_1(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_FEAT1 && TPP_CONF_MAYBE_1(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(TPP_CONF_FEAT0 && TPP_CONF_MAYBE_1(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(!TPP_CONF_MAYBE_0(TPP_CONF_1));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(TPP_CONF_MAYBE_0(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_0));
#if TPP_HAVE_EXTENSIONS
TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_EXT0));
#endif /* TPP_HAVE_EXTENSIONS */
TPP_STATIC_ASSERT(TPP_CONF_IS_FEAT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_FEAT(TPP_CONF_FEAT0));

#if TPP_HAVE_EXTENSIONS
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_IS_EXT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_EXT(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_FEAT0));
#endif /* TPP_HAVE_EXTENSIONS */

TPP_STATIC_ASSERT(TPP_CONF_IS_CONST(TPP_CONF_1));
TPP_STATIC_ASSERT(TPP_CONF_IS_CONST(TPP_CONF_0));
TPP_STATIC_ASSERT(!TPP_CONF_IS_CONST(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_CONST(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(!TPP_CONF_IS_CONST(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_CONST(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(!TPP_CONF_IS_RT(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_RT(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_IS_RT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_RT(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_IS_RT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_RT(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(TPP_CONF_DEFAULT(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_DEFAULT(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_DEFAULT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(!TPP_CONF_DEFAULT(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_DEFAULT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(!TPP_CONF_DEFAULT(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(TPP_CONF_MAKEFEAT(0) == TPP_CONF_FEAT0);
TPP_STATIC_ASSERT(TPP_CONF_MAKEFEAT(1) == TPP_CONF_FEAT1);
#if TPP_HAVE_EXTENSIONS
TPP_STATIC_ASSERT(TPP_CONF_MAKEEXT(0) == TPP_CONF_EXT0);
TPP_STATIC_ASSERT(TPP_CONF_MAKEEXT(1) == TPP_CONF_EXT1);
#endif /* TPP_HAVE_EXTENSIONS */


/* Assert that token <=> error conversion works */
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_ENOMEM));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_ENOMEM) == TPP_ENOMEM);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_ENOMEM) == TPP_ENOMEM);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_ENOMEM) == TPP_TOK_ENOMEM);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_ENOMEM) == TPP_TOK_ENOMEM);
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_EIO));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_EIO) == TPP_EIO);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_EIO) == TPP_EIO);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_EIO) == TPP_TOK_EIO);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_EIO) == TPP_TOK_EIO);
#if TPP_HAVE_FILE_NONBLOCK
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_EWOULDBLOCK));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_EWOULDBLOCK) == TPP_EWOULDBLOCK);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_EWOULDBLOCK) == TPP_EWOULDBLOCK);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_EWOULDBLOCK) == TPP_TOK_EWOULDBLOCK);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_EWOULDBLOCK) == TPP_TOK_EWOULDBLOCK);
#endif /* TPP_HAVE_FILE_NONBLOCK */
#if TPP_HAVE_WARNINGS
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_ELEXERROR));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_ELEXERROR) == TPP_ELEXERROR);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_ELEXERROR) == TPP_ELEXERROR);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_ELEXERROR) == TPP_TOK_ELEXERROR);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_ELEXERROR) == TPP_TOK_ELEXERROR);
TPP_STATIC_ASSERT(TPP_TOK_ISERR(TPP_TOK_EWARNPRINT));
TPP_STATIC_ASSERT(TPP_TOK_ASERR(TPP_TOK_EWARNPRINT) == TPP_EWARNPRINT);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_EWARNPRINT) == TPP_EWARNPRINT);
TPP_STATIC_ASSERT(TPP_TOK_OFERR(TPP_EWARNPRINT) == TPP_TOK_EWARNPRINT);
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_EWARNPRINT) == TPP_TOK_EWARNPRINT);
#endif /* TPP_HAVE_WARNINGS */
TPP_STATIC_ASSERT(TPP_TOK_OFERR_OR_EOF(TPP_EOK) == TPP_TOK_EOF);

TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_EOF) == TPP_EOK);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_SPACE) == TPP_EOK);
TPP_STATIC_ASSERT(TPP_TOK_ASERR_OR_EOK(TPP_TOK_AMP) == TPP_EOK);

TPP_STATIC_ASSERT(!TPP_TOK_ISCHAR(TPP_TOK_ENOMEM));
TPP_STATIC_ASSERT(!TPP_TOK_ISCHAR(TPP_TOK_EIO));
TPP_STATIC_ASSERT(TPP_TOK_ISCHAR(TPP_TOK_EOF));
TPP_STATIC_ASSERT(TPP_TOK_ISCHAR(TPP_TOK_AMP));
TPP_STATIC_ASSERT(!TPP_TOK_ISCHAR(TPP_INTERNAL(TPP_TOK_MULTICHAR_BEGIN) + 1));

TPP_STATIC_ASSERT(TPP_TOK_OFCHAR('~') == TPP_TOK_TILDE);
TPP_STATIC_ASSERT(TPP_TOK_OFCHAR('&') == TPP_TOK_AMP);


/* Make sure that offsets within "tpp_lexer" are properly aligned such that
 * the tail end of "tpp_token" correctly overlaps with the start of "tpp_file" */
TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer, tl_core.tlc_tok.tt_start) ==
                  tpp_offsetof(tpp_lexer, tl_core.tlc_input.tli_file.tf_tpos));
TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer, tl_core.tlc_tok.tt_end) ==
                  tpp_offsetof(tpp_lexer, tl_core.tlc_input.tli_file.tf_pos));
TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer, tl_core.tlc_tok.tt_chunk) ==
                  tpp_offsetof(tpp_lexer, tl_core.tlc_input.tli_file.tf_chunk));

/* Initialize/finalize everything about "self", except for the
 * currently loaded file; which the caller must still initialize
 * using one of the "tpp_lexer_initfile_*" functions below. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init(tpp_lexer *tpp_restrict self) {
#if TPP_HAVE_USER_KEYWORDS
	tpp_keywords_init(&self->tl_kwds);
#endif /* TPP_HAVE_USER_KEYWORDS */

#if TPP_HAVE_LEXER_STATE_FLAGS
	self->tl_state = TPP_LEXER_STATE_FLAG_NORMAL;
#endif /* TPP_HAVE_LEXER_STATE_FLAGS */

#if TPP_HAVE_EXTENSIONS
	tpp_extensions_init(&self->tl_exts);
#endif /* TPP_HAVE_EXTENSIONS */

#if TPP_HAVE_FEATURES
	tpp_features_init(&self->tl_feat);
#endif /* TPP_HAVE_FEATURES */

#if TPP_HAVE_INCLUDE_PATH
	tpp_include_paths_init(&self->tl_include_paths);
#endif /* TPP_HAVE_INCLUDE_PATH */

#if TPP_HAVE_HOOKS
	tpp_hooks_init(&self->tl_hooks);
#endif /* TPP_HAVE_HOOKS */

#if TPP_HAVE_WARNINGS
	tpp_warnings_init(&self->tl_warn);
#endif /* TPP_HAVE_WARNINGS */

#if TPP_HAVE_WARNING_ERROR
	self->tl_error_count = 0;
#if TPP_ERROR_LIMIT < 0
	self->tl_error_limit = (tpp_size)(-TPP_ERROR_LIMIT);
#endif /* TPP_ERROR_LIMIT < 0 */
#endif /* TPP_HAVE_WARNING_ERROR */

#if TPP_HAVE_LEXER_WARNING_COUNT
	self->tl_warning_count = 0;
#endif /* TPP_HAVE_LEXER_WARNING_COUNT */

#if TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED && TPP_MAX_INCLUDE_DEPTH < 0
	self->tl_inclusion_limit = -TPP_MAX_INCLUDE_DEPTH;
#endif /* TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED && TPP_MAX_INCLUDE_DEPTH < 0 */

#if TPP_HAVE_MACRO_RECURSION && TPP_MAX_RECURSIVE_MACRO_DEPTH < 0
	self->tl_recursive_macro_limit = -TPP_MAX_RECURSIVE_MACRO_DEPTH;
#endif /* TPP_HAVE_MACRO_RECURSION && TPP_MAX_RECURSIVE_MACRO_DEPTH < 0 */

#if TPP_HAVE_MACRO___COUNTER__
	self->tl_builtin_counter = 0;
#endif /* TPP_HAVE_MACRO___COUNTER__ */

#if TPP_HAVE_LEXER_TIME
	tpp_time_empty(&self->tl_time);
#endif /* TPP_HAVE_LEXER_TIME */

#if TPP_HAVE_RT_FILE_AND_LINE_FORMAT
	self->tl_file_and_line_format = TPP_CONFIG_FILE_AND_LINE_FORMAT;
#endif /* TPP_HAVE_RT_FILE_AND_LINE_FORMAT */
}


/* Finalize the lexer, except for the currently loaded file.
 *
 * If the caller made use of "tpp_lexer_initfile_*", then they
 * must also (either before or after this function) call
 * `tpp_lexer_finifile()' to finalize the currently loaded file. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_fini(tpp_lexer *tpp_restrict self) {
	(void)self;

	/* Finalize keywords */
#if TPP_HAVE_USER_KEYWORDS
	tpp_keywords_fini(&self->tl_kwds);
#endif /* TPP_HAVE_USER_KEYWORDS */

	/* Finalize extension states */
#if TPP_HAVE_EXTENSIONS
	tpp_extensions_fini(&self->tl_exts);
#endif /* TPP_HAVE_EXTENSIONS */

	/* Finalize feature states */
#if TPP_HAVE_FEATURES
	tpp_features_fini(&self->tl_feat);
#endif /* TPP_HAVE_FEATURES */

	/* Finalize warning states */
#if TPP_HAVE_WARNINGS
	tpp_warnings_fini(&self->tl_warn);
#endif /* TPP_HAVE_WARNINGS */

	/* Finalize include paths */
#if TPP_HAVE_INCLUDE_PATH
	tpp_include_paths_fini(&self->tl_include_paths);
#endif /* TPP_HAVE_INCLUDE_PATH */

	tpp_dbg_memset((char *)&self->tl_core + sizeof(self->tl_core),
	               sizeof(*self) - sizeof(self->tl_core));
}


#if TPP_HAVE_LEXER_COPY
/* Initialize "self" as a copy of "from". This will copy everything
 * configured in "from" (features, extensions, allocated keyword IDs,
 * macros, include paths, warnings, etc), into "self". The only thing
 * that is not copied is the #include-stack, meaning that after a call
 * to this function, the caller must still call `tpp_lexer_initfile_*'
 *
 * Additionally, the following properties are not copied:
 * - tpp_keyword_misc_getuserdata_dtor()  (only "tpp_keyword_misc_getuserdata()"
 *                                         is copied; dtors are set to "NULL")
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_copy(tpp_lexer *tpp_restrict self,
               tpp_lexer const *tpp_restrict from) {
	tpp_errno error;
#if TPP_HAVE_LEXER_TIME
	error = tpp_time_copy(&self->tl_time, &from->tl_time);
	if (TPP_ISERR(error))
		goto err;
#endif /* TPP_HAVE_LEXER_TIME */
#if TPP_HAVE_WARNINGS
	tpp_warnings_init(&self->tl_warn);
#endif /* TPP_HAVE_WARNINGS */
#if TPP_HAVE_INCLUDE_PATH
	error = tpp_include_paths_copy(&self->tl_include_paths,
	                               &from->tl_include_paths);
	if (TPP_ISERR(error))
		goto err_warn;
#endif /* TPP_HAVE_INCLUDE_PATH */
#if TPP_HAVE_EXTENSIONS
	error = tpp_extensions_copy(&self->tl_exts, &from->tl_exts);
	if (TPP_ISERR(error))
		goto err_warn_incl;
#endif /* TPP_HAVE_EXTENSIONS */
#if TPP_HAVE_USER_KEYWORDS
	error = tpp_keywords_copy(&self->tl_kwds, &from->tl_kwds);
	if (TPP_ISERR(error))
		goto err_warn_incl_exts;
#endif /* TPP_HAVE_USER_KEYWORDS */

	/* Copy stuff that can't cause errors... */
#if TPP_HAVE_LEXER_STATE_FLAGS
	self->tl_state = from->tl_state;
#endif /* TPP_HAVE_LEXER_STATE_FLAGS */

#if TPP_HAVE_FEATURES
	self->tl_feat = from->tl_feat;
#endif /* TPP_HAVE_FEATURES */

#if TPP_HAVE_HOOKS
	self->tl_hooks = from->tl_hooks;
#endif /* TPP_HAVE_HOOKS */

#if TPP_HAVE_WARNING_ERROR
	self->tl_error_count = from->tl_error_count;
#if TPP_ERROR_LIMIT < 0
	self->tl_error_count = from->tl_error_limit;
#endif /* TPP_ERROR_LIMIT < 0 */
#endif /* TPP_HAVE_WARNING_ERROR */

#if TPP_HAVE_LEXER_WARNING_COUNT
	self->tl_warning_count = from->tl_warning_count;
#endif /* TPP_HAVE_LEXER_WARNING_COUNT */

#if TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED && TPP_MAX_INCLUDE_DEPTH < 0
	self->tl_inclusion_limit = from->tl_inclusion_limit;
#endif /* TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED && TPP_MAX_INCLUDE_DEPTH < 0 */

#if TPP_HAVE_MACRO_RECURSION && TPP_MAX_RECURSIVE_MACRO_DEPTH < 0
	self->tl_recursive_macro_limit = from->tl_recursive_macro_limit;
#endif /* TPP_HAVE_MACRO_RECURSION && TPP_MAX_RECURSIVE_MACRO_DEPTH < 0 */

#if TPP_HAVE_MACRO___COUNTER__
	self->tl_builtin_counter = from->tl_builtin_counter;
#endif /* TPP_HAVE_MACRO___COUNTER__ */

#if TPP_HAVE_RT_FILE_AND_LINE_FORMAT
	self->tl_file_and_line_format = from->tl_file_and_line_format;
#endif /* TPP_HAVE_RT_FILE_AND_LINE_FORMAT */

	return TPP_EOK;
#if TPP_HAVE_USER_KEYWORDS
err_warn_incl_exts:
#endif /* TPP_HAVE_USER_KEYWORDS */
#if TPP_HAVE_EXTENSIONS
#if TPP_HAVE_USER_KEYWORDS
	tpp_extensions_fini(&self->tl_exts);
#endif /* TPP_HAVE_USER_KEYWORDS */
err_warn_incl:
#endif /* TPP_HAVE_EXTENSIONS */
#if TPP_HAVE_INCLUDE_PATH
#if TPP_HAVE_USER_KEYWORDS || TPP_HAVE_EXTENSIONS
	tpp_include_paths_fini(&self->tl_include_paths);
#endif /* TPP_HAVE_USER_KEYWORDS || TPP_HAVE_EXTENSIONS */
err_warn:
#endif /* TPP_HAVE_INCLUDE_PATH */
#if TPP_HAVE_WARNINGS
#if TPP_HAVE_USER_KEYWORDS || TPP_HAVE_EXTENSIONS || TPP_HAVE_INCLUDE_PATH
	tpp_warnings_fini(&self->tl_warn);
#endif /* TPP_HAVE_USER_KEYWORDS || TPP_HAVE_EXTENSIONS || TPP_HAVE_INCLUDE_PATH */
#endif /* TPP_HAVE_WARNINGS */
err:
	return error;
}
#endif /* TPP_HAVE_LEXER_COPY */


/* Finalize the currently loaded file (including any extra files
 * found on the #include-stack, but that hadn't been popped yet)
 *
 * This function must be called after "tpp_lexer_initfile_*" has been */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_finifile(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);

	/* Finalize whatever still remains of the #include-stack */
#if TPP_HAVE_INCLUDE_STACK
	tpp_file *iter;
	for (iter = file->tf_prev; iter;) {
		tpp_file *next = iter->tf_prev;
		tpp_file_fini(iter);
		tpp_file_free(iter);
		iter = next;
	}
#endif /* TPP_HAVE_INCLUDE_STACK */

	/* Finalize the top-most file of the #include-stack */
	tpp_file_fini(file);
	tpp_dbg_memset(&self->tl_core, sizeof(self->tl_core));
}


/* Initialize a lexer's file to read the given [text,text+text_size) blob.
 * @param: start_lc: [valid_if(chunk != NULL)] */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_initfile_text(tpp_lexer *tpp_restrict self,
                         /*utf-8*/ char const *filename,
                         /*inherit(always)*/ TPP_REF tpp_string *chunk,
                         void const *text, tpp_size text_size,
                         tpp_lcinfo start_lc
#if TPP_HAVE_FILE_FLAGS
                         , tpp_file_flags flags
#endif /* TPP_HAVE_FILE_FLAGS */
#if TPP_HAVE_UNICODE
                         , tpp_file_encoding encoding
#endif /* TPP_HAVE_UNICODE */
                         ) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_init_text_ex(file, filename, chunk, text, text_size, start_lc, flags, encoding);
}


#if TPP_HAVE_LEXER_INIT_IO
/* Initialize a lexer such that it starts reading from "handle"
 * @param: filename: [0..1] Filename to use for messages (s.a. `tpp_file_getrealfilename()')
 *                          WARNING: This filename is *NOT* copied -- it must remain
 *                                   allocated and valid until "self" is finalized.
 * @param: handle:   The I/O handle to read from in order to retrieve text data.
 * @param: ioflags:  Extra flags specifying how to interact with "handle":
 *                   - TPP_FILE_FLAGS_NONBLOCK: Do non-blocking reads (useful in case "handle" is a pipe)
 *                   - TPP_FILE_FLAGS_NOCLOSE:  A later call to `tpp_lexer_finifile()' will not close "handle"
 *                   - TPP_FILE_FLAGS_SYSHDR:   Do not emit warnings */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_initfile_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                         tpp_io_handle handle, tpp_file_flags ioflags) {
	tpp_file *const file = tpp_lexer_getfile(self);
	/* It can never be a keyword, since the lexer is only being
	 * initialized right now (and the keyword would have had to
	 * be allocated by the lexer) */
	ioflags |= TPP_FILE_FLAGS_NOKWD;
	tpp_file_init_io_ex(file, filename, handle, ioflags);
}
#endif /* TPP_HAVE_LEXER_INIT_IO */



#if TPP_HAVE_LEXER_INIT_OPEN
/* Initialize a lexer such that it starts reading from "filename"
 * @param: filename_maxlen: Max length of "filename" (in characters). You may
 *                          pass TPP_SIZE_MAX when "filename" is NUL-terminated.
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_initfile_open(tpp_lexer *tpp_restrict self,
                        /*utf-8*/ char const *tpp_restrict filename,
                        tpp_size filename_maxlen) {
	tpp_errno error;
	tpp_lexer_openfile_result ofr;
	error = tpp_lexer_openfile(self, NULL, filename, filename_maxlen, &ofr);
	if (TPP_ISERR(error)) {
		tpp_lexer_fini(self);
	} else {
		/* Initialize the lexer's I/O file */
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_file_init_io_from_ofr(file, &ofr);
	}
	return error;
}
#endif /* TPP_HAVE_LEXER_INIT_OPEN */


#if TPP_HAVE_INCLUDE_STACK
#if TPP_HAVE_LEXER_INIT_IO
/* Push another file onto the #include-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @param: filename: [0..1] Filename to use for messages (s.a. `tpp_file_getrealfilename()')
 *                          WARNING: This filename is *NOT* copied -- it must remain
 *                                   allocated and valid until "self" is finalized.
 * @param: handle:   The I/O handle to read from in order to retrieve text data.
 * @param: ioflags:  Extra flags specifying how to interact with "handle":
 *                   - TPP_FILE_FLAGS_NONBLOCK: Do non-blocking reads (useful in case "handle" is a pipe)
 *                   - TPP_FILE_FLAGS_NOCLOSE:  A later call to `tpp_lexer_finifile()' will not close "handle"
 *                   - TPP_FILE_FLAGS_SYSHDR:   Do not emit warnings
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_pushfile_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                         tpp_io_handle handle, tpp_file_flags ioflags) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		return TPP_ENOMEM;
	tpp_file_move(prev_file, file);
	ioflags |= TPP_FILE_FLAGS_NOKWD;
	tpp_file_init_io_ex(file, filename, handle, ioflags);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_INIT_IO */

#if TPP_HAVE_LEXER_INIT_OPEN
/* Push another file onto the #include-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @param: filename_maxlen: Max length of "filename" (in characters). You may
 *                          pass TPP_SIZE_MAX when "filename" is NUL-terminated.
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_pushfile_open(tpp_lexer *tpp_restrict self,
                        /*utf-8*/ char const *tpp_restrict filename,
                        tpp_size filename_maxlen) {
	tpp_errno error;
	tpp_lexer_openfile_result ofr;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		return TPP_ENOMEM;
	tpp_file_move(prev_file, file);
	error = tpp_lexer_openfile(self, NULL, filename, filename_maxlen, &ofr);
	if tpp_likely(!TPP_ISERR(error)) {
		tpp_file_init_io_from_ofr(file, &ofr);
		file->tf_prev  = prev_file;
		file->tf_tprev = prev_file;
	} else {
		tpp_file_move(file, prev_file);
		tpp_file_free(prev_file);
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_INIT_OPEN */

/* Push another file onto the #include-stack: [text,text+text_size) blob.
 * After a call to this function, the caller is responsible to yield the first token!
 * @param: start_lc: [valid_if(chunk != NULL)]
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_lexer_pushfile_text(tpp_lexer *tpp_restrict self,
                         /*utf-8*/ char const *filename,
                         /*inherit(always)*/ TPP_REF tpp_string *chunk,
                         void const *text, tpp_size text_size,
                         tpp_lcinfo start_lc
#if TPP_HAVE_FILE_FLAGS
                         , tpp_file_flags flags
#endif /* TPP_HAVE_FILE_FLAGS */
#if TPP_HAVE_UNICODE
                         , tpp_file_encoding encoding
#endif /* TPP_HAVE_UNICODE */
                         ) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file) {
		if (chunk)
			tpp_string_decref(chunk); /* Reference must *always* be inherited! */
		return TPP_ENOMEM;
	}
	tpp_file_move(prev_file, file);
	tpp_file_init_text_ex(file, filename, chunk, text, text_size, start_lc, flags, encoding);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	return TPP_EOK;
}


/* Pop the current file off the #include-stack.
 * The caller is responsible to ensure that "tpp_lexer_canpopfile(self) == true"
 * After a call to this function, the caller is responsible to yield the next token!
 * WARNING: It is the caller's responsibility to call "tpp_lexer_manualpopfile_popfile()"
 *          instead of this function if rollback of the pop should be possible.
 * NOTE: It is recommended to call "tpp_lexer_warn_nonempty_ifdef()" before calling
 *       this function in order to warn about unterminated #ifdef-blocks. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_popfile(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev = file->tf_prev;
	tpp_file_fini(file);
	tpp_file_move(file, prev);
	tpp_file_free(prev);
}
#endif /* TPP_HAVE_INCLUDE_STACK */



#if TPP_HAVE_LEXER_REPRTOKENID
/* Return the (canonical) string-representation of a given token ID */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) char const *TPPCALL
tpp_lexer_reprtokenid(tpp_lexer const *tpp_restrict self, tpp_token_id tok) {
#if TPP_HAVE_USER_KEYWORDS
	char const *result = tpp_reprtokenid(tok);
	if (result)
		return result;
	if (TPP_TOK_ISUSERKEYWORD(tok)) {
		/* Lookup keyword */
		tpp_keyword const *kwd = tpp_keywords_getkeyword_byid(&self->tl_kwds, tok);
		if (kwd)
			return (char const *)kwd->tk_kwd;
	}
	return NULL;
#else /* TPP_HAVE_USER_KEYWORDS */
	(void)self;
	return tpp_reprtokenid(tok);
#endif /* !TPP_HAVE_USER_KEYWORDS */
}
#endif /* TPP_HAVE_LEXER_REPRTOKENID */



#if TPP_HAVE_LEXER_MANUALPOPFILE
/* Example of the data-layout (also demonstrating how data is backed up)
 * when using the `tpp_lexer_manualpopfile_*()' set of functions:
 *
 * Input:
 * >> #define foo(a, b) a+b
 * >> #define bar       foo
 * >> #define baz       bar
 * >> #define exo       baz
 * >> 
 * >> bot:  exo[10, 20]
 *
 * Data-layout:
 * >> tpp_lexer_manualpopfile_start(self):
 *    tpp_lexer_getfile    ="bar"->tf_prev=@baz="baz"
 *    _tlmpf_orig_prev @baz="baz"->tf_prev=@exo="exo"
 *                     @exo="exo"->tf_prev=@bot="bot"
 *                     @bot="bot"->tf_prev=NULL
 *
 * >> tpp_lexer_manualpopfile_popfile(self):            // 1st
 *    tpp_lexer_getfile    ="baz"->tf_prev=@exo="exo"
 *    _tlmpf_orig_prev @baz="bar"->tf_prev=@exo="exo"
 *                     @exo="exo"->tf_prev=@bot="bot"
 *                     @bot="bot"->tf_prev=NULL
 *
 * >> tpp_lexer_manualpopfile_popfile(self):            // 2nd
 *    tpp_lexer_getfile    ="exo"->tf_prev=@bot="bot"
 *    _tlmpf_orig_prev @baz="bar"->tf_prev=@exo="baz"
 *                     @exo="baz"->tf_prev=@bot="bot"
 *                     @bot="bot"->tf_prev=NULL
 *
 * >> tpp_lexer_manualpopfile_popfile(self):            // 3rd
 *    tpp_lexer_getfile    ="bot"->tf_prev=NULL
 *    _tlmpf_orig_prev @baz="bar"->tf_prev=@exo="baz"
 *                     @exo="baz"->tf_prev=@bot="exo"
 *                     @bot="exo"->tf_prev=NULL
 */

TPP_INLINE TPP_NONNULL((1, 2)) void TPPCALL
tpp_swapmem(void *a, void *b, tpp_size num_bytes) {
	unsigned char *ca = (unsigned char *)a;
	unsigned char *cb = (unsigned char *)b;
	for (; num_bytes; --num_bytes, ++ca, ++cb) {
		unsigned char temp;
		temp = *ca;
		*ca  = *cb;
		*cb  = temp;
	}
}

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_manualpopfile_popfile(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev = file->TPP_INTERNAL(tf_prev);
	tpp_assert(prev != NULL && "Nowhere to pop to (caller didn't check `tpp_lexer_manualpopfile_canpopfile()')");
	file->tf_prev = prev->tf_prev;
	tpp_swapmem(file, prev, sizeof(tpp_file)); /* NOTE: This could skip "tf_prev", since that's equal in both */
}

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_manualpopfile_break_rollback(tpp_lexer *tpp_restrict self,
                                        tpp_file *tpp_restrict orig_prev) {
	tpp_file *const file = tpp_lexer_getfile(self);
	while (file->tf_prev != orig_prev) {
		tpp_file *last = orig_prev;
		tpp_file *last_prev;
		tpp_assert(last);
		while (last->tf_prev != file->tf_prev) {
			tpp_assert(last->tf_prev);
			last = last->tf_prev;
		}
		/* "tf_prev" shouldn't be swapped, so backup */
		last_prev = last->tf_prev;
		/* This could skip "tf_prev"; then wouldn't need "last_prev" */
		tpp_swapmem(file, last, sizeof(tpp_file));
		last->tf_prev = last_prev;
		file->tf_prev = last;
	}
}

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_manualpopfile_break_commit(tpp_lexer *tpp_restrict self,
                                      tpp_file *tpp_restrict orig_prev) {
	tpp_file const *const file = tpp_lexer_getfile(self);
	while (file->tf_prev != orig_prev) {
		tpp_file *prev_prev;
		tpp_assert(orig_prev);
		prev_prev = orig_prev->tf_prev;
		tpp_file_fini(orig_prev);
		tpp_file_free(orig_prev);
		orig_prev = prev_prev;
	}
}
#endif /* TPP_HAVE_LEXER_MANUALPOPFILE */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_C */
