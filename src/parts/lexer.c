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

/* Make sure that offsets within "tpp_lexer" are properly aligned such that
 * the tail end of "tpp_token" correctly overlaps with the start of "tpp_file" */
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
		tpp_free(iter);
		iter = next;
	}
#endif /* TPP_HAVE_INCLUDE_STACK */

	/* Finalize the top-most file of the #include-stack */
	tpp_file_fini(file);

	/* Finalize common data... */
	_tpp_lexer_fini_common(self);
}


/* Initialize a lexer that simply reads the given [text,text+text_size) blob. */
#if TPP_HAVE_UNICODE
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ex(tpp_lexer *tpp_restrict self,
                       /*utf-8*/ char const *filename,
                       void const *text, tpp_size text_size,
                       tpp_file_encoding encoding)
#else /* TPP_HAVE_UNICODE */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ascii(tpp_lexer *tpp_restrict self,
                          /*utf-8*/ char const *filename,
                          void const *text, tpp_size text_size)
#endif /* !TPP_HAVE_UNICODE */
{
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_init_text_ex(file, filename, text, text_size, encoding);
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
	tpp_keyword *filename_kwd = NULL;
	if (filename != NULL) {
		filename_kwd = (tpp_keyword *)((char *)filename - offsetof(tpp_keyword, tk_kwd));
		ioflags |= TPP_HAVE_FILE_NOKWD;
	}
	tpp_file_init_io_ex(file, filename_kwd, handle, ioflags);
	_tpp_lexer_init_common(self);
}
#endif /* TPP_HAVE_LEXER_INIT_IO */



#if TPP_HAVE_LEXER_INIT_FILENAME
/* Initialize a lexer such that it starts reading from "filename"
 * @return: * : TPP_ENOENT: No such file or directory
 * @return: * : TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_init_filename(tpp_lexer *tpp_restrict self,
                        /*utf-8*/ char const *tpp_restrict filename) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_errno error;
	_tpp_lexer_init_common(self);
	error = tpp_keywords_openfile(&self->tl_kwds, NULL, filename, file);
	if tpp_unlikely(error != TPP_EOK)
		_tpp_lexer_fini_common(self);
	return error;
}
#endif /* TPP_HAVE_LEXER_INIT_FILENAME */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_C */
