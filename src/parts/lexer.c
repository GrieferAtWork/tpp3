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
TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_FEAT(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(TPP_CONF_IS_FEAT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_FEAT(TPP_CONF_FEAT0));

TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_0));
TPP_STATIC_ASSERT(TPP_CONF_IS_EXT(TPP_CONF_EXT1));
TPP_STATIC_ASSERT(TPP_CONF_IS_EXT(TPP_CONF_EXT0));
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_FEAT1));
TPP_STATIC_ASSERT(!TPP_CONF_IS_EXT(TPP_CONF_FEAT0));

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
TPP_STATIC_ASSERT(TPP_CONF_MAKEEXT(0) == TPP_CONF_EXT0);
TPP_STATIC_ASSERT(TPP_CONF_MAKEEXT(1) == TPP_CONF_EXT1);


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

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_fini_common(tpp_lexer *tpp_restrict self) {
	/* Finalize keywords */
	tpp_keywords_fini(&self->tl_kwds);

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
}

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_fini(tpp_lexer *tpp_restrict self) {
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

	/* Finalize common data... */
	_tpp_lexer_fini_common(self);
}


/* Initialize a lexer that simply reads the given [text,text+text_size) blob.
 * @param: start_lc: [valid_if(chunk != NULL)] */
#if TPP_HAVE_UNICODE
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ex(tpp_lexer *tpp_restrict self,
                       /*utf-8*/ char const *filename,
                       /*inherit(always)*/ TPP_REF tpp_string *chunk,
                       void const *text, tpp_size text_size,
                       tpp_lcinfo start_lc, tpp_file_encoding encoding)
#else /* TPP_HAVE_UNICODE */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ascii(tpp_lexer *tpp_restrict self,
                          /*utf-8*/ char const *filename,
                          /*inherit(always)*/ TPP_REF tpp_string *chunk,
                          void const *text, tpp_size text_size,
                          tpp_lcinfo start_lc)
#endif /* !TPP_HAVE_UNICODE */
{
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_init_text_ex(file, filename, chunk, text, text_size, start_lc, encoding);
	_tpp_lexer_init_common(self);
}


#if TPP_HAVE_LEXER_INIT_IO
/* Initialize a lexer such that it starts reading from "handle"
 * @param: filename: [0..1] Filename to use for messages (s.a. `tpp_file_filename()')
 *                          WARNING: This filename is *NOT* copied -- it must remain
 *                                   allocated and valid until "self" is finalized.
 * @param: handle:   The I/O handle to read from in order to retrieve text data.
 * @param: ioflags:  Extra flags specifying how to interact with "handle":
 *                   - TPP_FILE_IOFLAGS_NONBLOCK: Do non-blocking reads (useful in case "handle" is a pipe)
 *                   - TPP_FILE_IOFLAGS_NOCLOSE:  A later call to `tpp_lexer_fini()' will not close "handle"
 *                   - TPP_FILE_IOFLAGS_SYSHDR:   Do not emit warnings */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                     tpp_io_handle handle, tpp_file_ioflags ioflags) {
	tpp_file *const file = tpp_lexer_getfile(self);
	/* It can never be a keyword, since the lexer is only being
	 * initialized right now (and the keyword would have had to
	 * be allocated by the lexer) */
	ioflags |= TPP_FILE_IOFLAGS_NOKWD;
	tpp_file_init_io_ex(file, filename, handle, ioflags);
	_tpp_lexer_init_common(self);
}
#endif /* TPP_HAVE_LEXER_INIT_IO */



#if TPP_HAVE_LEXER_INIT_FILENAME
/* Initialize a lexer such that it starts reading from "filename"
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_init_filename(tpp_lexer *tpp_restrict self,
                        /*utf-8*/ char const *tpp_restrict filename) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	_tpp_lexer_init_common(self);
	error = tpp_keywords_openfile(&self->tl_kwds, NULL, filename, file);
	if tpp_unlikely(TPP_ISERR(error))
		_tpp_lexer_fini_common(self);
	return error;
}
#endif /* TPP_HAVE_LEXER_INIT_FILENAME */


#if TPP_HAVE_INCLUDE_STACK
#if TPP_HAVE_LEXER_INIT_IO
/* Push another file onto the #include-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @param: filename: [0..1] Filename to use for messages (s.a. `tpp_file_filename()')
 *                          WARNING: This filename is *NOT* copied -- it must remain
 *                                   allocated and valid until "self" is finalized.
 * @param: handle:   The I/O handle to read from in order to retrieve text data.
 * @param: ioflags:  Extra flags specifying how to interact with "handle":
 *                   - TPP_FILE_IOFLAGS_NONBLOCK: Do non-blocking reads (useful in case "handle" is a pipe)
 *                   - TPP_FILE_IOFLAGS_NOCLOSE:  A later call to `tpp_lexer_fini()' will not close "handle"
 *                   - TPP_FILE_IOFLAGS_SYSHDR:   Do not emit warnings
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_pushfile_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                         tpp_io_handle handle, tpp_file_ioflags ioflags) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		return TPP_ENOMEM;
	*prev_file = *file;
	ioflags |= TPP_FILE_IOFLAGS_NOKWD;
	tpp_file_init_io_ex(file, filename, handle, ioflags);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_INIT_IO */

#if TPP_HAVE_LEXER_INIT_FILENAME
/* Push another file onto the #include-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_pushfile_filename(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *tpp_restrict filename) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		return TPP_ENOMEM;
	*prev_file = *file;
	error = tpp_keywords_openfile(&self->tl_kwds, NULL, filename, file);
	if tpp_likely(!TPP_ISERR(error)) {
		file->tf_prev  = prev_file;
		file->tf_tprev = prev_file;
	} else {
		*file = *prev_file;
		tpp_file_free(prev_file);
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_LEXER_INIT_FILENAME */

/* Check if the current file can be popped. */
#define tpp_lexer_canpopfile(self) \
	(tpp_lexer_getfile(self)->TPP_INTERNAL(tf_prev) != NULL)

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
	*file = *prev;
	tpp_file_free(prev);
}

#endif /* TPP_HAVE_INCLUDE_STACK */


#if TPP_HAVE_LEXER_REPRTOKENID
/* Return the (canonical) string-representation of a given token ID */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) char const *TPPCALL
tpp_lexer_reprtokenid(tpp_lexer const *tpp_restrict self, tpp_token_id tok) {
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

TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_manualpopfile_popfile(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *const prev = file->TPP_INTERNAL(tf_prev);
	tpp_assert(prev != NULL && "Nowhere to pop to (caller didn't check `tpp_lexer_manualpopfile_canpopfile()')");
	file->tf_prev = prev->tf_prev;
	tpp_swapmem(file, prev, sizeof(tpp_file)); /* NOTE: This could skip "tf_prev", since that's equal in both */
}

TPP_DECL TPP_NONNULL((1)) void TPPCALL
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

TPP_DECL TPP_NONNULL((1)) void TPPCALL
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
