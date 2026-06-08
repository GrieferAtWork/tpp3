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
#ifndef GUARD_TPP_LEXER_H
#define GUARD_TPP_LEXER_H 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "file.h"
#include "keyword.h"
#include "token.h"
#include "features.h"
#include "warnings.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#undef TPP_HAVE_LEXER_STATE_FLAGS
#if (TPP_HAVE_CPP_DIRECTIVES || TPP_HAVE_WARNINGS)
#define TPP_HAVE_LEXER_STATE_FLAGS 1
#else /* ... */
#define TPP_HAVE_LEXER_STATE_FLAGS 0
#endif /* !... */


#if TPP_HAVE_LEXER_STATE_FLAGS
#define tpp_lexer_state_flags uint_least8_t
#define TPP_LEXER_STATE_FLAG_NORMAL       UINT8_C(0x00) /* Normal state flags */
#if TPP_HAVE_CPP_DIRECTIVES
#define TPP_LEXER_STATE_FLAG_NODIRECTIVES UINT8_C(0x01) /* A non-comment/space token was encountered since the last
                                                         * TPP_TOK_LF, meaning PP-directives may not be parsed. */
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#if TPP_HAVE_WARNINGS
#define TPP_LEXER_STATE_FLAG_NOWARNINGS   UINT8_C(0x02) /* Do not emit any warnings/errors (don't even trigger them) -- should be used during seek-ahead yields. */
#endif /* TPP_HAVE_WARNINGS */
#endif /* TPP_HAVE_LEXER_STATE_FLAGS */


#if TPP_HAVE_INCLUDE_STACK
#define TPP_TOKEN_START_OF_FILE_FIELD tt_start /* tt_start == tf_tpos */
#else /* TPP_HAVE_INCLUDE_STACK */
#define TPP_TOKEN_START_OF_FILE_FIELD tt_end   /* tt_end == tf_pos */
#endif /* !TPP_HAVE_INCLUDE_STACK */

typedef struct tpp_lexer {
	union {
		tpp_token      tlc_tok;  /* [valid_if(WAS_CALLED(tpp_lexer_yieldraw()))] Last-read token (never
		                          * set to one of `TPP_TOK_E*'; iow: always positive or TPP_TOK_EOF). */
		struct {
			char _tli_pad[tpp_offsetof(tpp_token, TPP_TOKEN_START_OF_FILE_FIELD)];
			tpp_file   tli_file; /* [OVERRIDE(.tf_prev, [owned])]
			                      * The file that lies at the top of the lexer's #include/macro-stack.
			                      * this is also the file whose buffer currently contains `tl_tok' */
		} tlc_input;
	} tl_core;


	/* Custom keywords table. */
	tpp_keywords tl_kwds;


	/* Lexer extensions. */
#if TPP_HAVE_EXTENSIONS
	tpp_extensions tl_exts;
#define _tpp_lexer_init_exts(self) , tpp_extensions_init(&(self)->tl_exts)
#else /* TPP_HAVE_EXTENSIONS */
#define _tpp_lexer_init_exts(self) /* noting */
#endif /* !TPP_HAVE_EXTENSIONS */


	/* Enabled tokens */
#if TPP_HAVE_FEATURES
	tpp_features tl_feat;
#define _tpp_lexer_init_feat(self) , tpp_features_init(&(self)->tl_feat)
#else /* TPP_HAVE_FEATURES */
#define _tpp_lexer_init_feat(self) /* nothing */
#endif /* !TPP_HAVE_FEATURES */


	/* Lexer state flags */
#if TPP_HAVE_LEXER_STATE_FLAGS
	tpp_lexer_state_flags tl_state;
#define _tpp_lexer_init_state(self) , (self)->tl_state = TPP_LEXER_STATE_FLAG_NORMAL
#else /* TPP_HAVE_LEXER_STATE_FLAGS */
#define _tpp_lexer_init_state(self) /* nothing */
#endif /* !TPP_HAVE_LEXER_STATE_FLAGS */


	/* TODO: system #include paths (/usr/include, ...) */


	/* Warning configuration / printer */
#undef TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER /* Wrapper for "TPP_CONFIG_WARNPRINTER" */
#undef TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER /* Calls "fwrite(stderr)" */
#undef TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER    /* Does nothing */
#if TPP_HAVE_WARNINGS
	tpp_warnings tl_warn; /* Compiler warnings state */
#define _tpp_lexer_init_warn(self) , tpp_warnings_init(&(self)->tl_warn)
#ifdef TPP_CONFIG_WARNPRINTER
#if TPP_CONFIG_WARNPRINTER_NEEDS_ARG
	void             *tl_warnprinterarg; /* [?..?] Argument for "TPP_CONFIG_WARNPRINTER" */
#define tpp_lexer_getwarnprinter(self)    (&TPP_CONFIG_WARNPRINTER)
#define tpp_lexer_getwarnprinterarg(self) (self)->tl_warnprinterarg
#else /* TPP_CONFIG_WARNPRINTER_NEEDS_ARG */
#define TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self)    &_tpp_lexer_wrapped_warnprinter
#define tpp_lexer_getwarnprinterarg(self) NULL
#endif /* !TPP_CONFIG_WARNPRINTER_NEEDS_ARG */
#else /* TPP_CONFIG_WARNPRINTER */
	tpp_formatprinter tl_warnprinter;    /* [0..1] Warning printer (or "NULL" to use "fwrite(stderr)") */
	void             *tl_warnprinterarg; /* [valid_if(tl_warnprinter != NULL)] */
#define _tpp_lexer_init_warnprinter(self) , (self)->tl_warnprinter = NULL
#define tpp_lexer_setwarnprinter(self, printer, arg) \
	(void)((self)->tl_warnprinter    = (printer),    \
	       (self)->tl_warnprinterarg = (arg))
#if TPP_HAVE_BUILTIN_WARNPRINTER
#define TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self) ((self)->tl_warnprinter ? (self)->tl_warnprinter : &_tpp_lexer_builtin_warnprinter)
#else /* TPP_HAVE_BUILTIN_WARNPRINTER */
#define TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self) ((self)->tl_warnprinter ? (self)->tl_warnprinter : &_tpp_lexer_noop_warnprinter)
#endif /* !TPP_HAVE_BUILTIN_WARNPRINTER */
#define tpp_lexer_getwarnprinterarg(self) (self)->tl_warnprinterarg
#endif /* !TPP_CONFIG_WARNPRINTER */
#else /* TPP_HAVE_WARNINGS */
#define _tpp_lexer_init_warn(self)                        /* nothing */
#define tpp_lexer_setwarnprinter(self, printer, arg) (void)0
#endif /* !TPP_HAVE_WARNINGS */
#ifndef _tpp_lexer_init_warnprinter
#define _tpp_lexer_init_warnprinter(self) /* nothing */
#endif /* !_tpp_lexer_init_warnprinter */


	/* Lexer error limits */
#if TPP_HAVE_WARNING_ERROR
	tpp_size tl_error_count; /* # of times "TPP_WSTATE_ERROR" was emitted.
	                          * When this is non-zero by the time your compiler finishes
	                          * compiling your source file, you should NOT proceed, but
	                          * propagate an error. */
#define _tpp_lexer_initerrorcount(self)  , (self)->tl_error_count = 0
#define tpp_lexer_geterrorcount(self)    (self)->tl_error_count
#define tpp_lexer_seterrorcount(self, v) (void)((self)->tl_error_count = (v))
#if TPP_ERROR_LIMIT < 0
	tpp_size tl_error_limit; /* Once `tl_error_count >= tl_error_limit', "TPP_WSTATE_ERROR" is treated as "TPP_WSTATE_FATAL" */
#define _tpp_lexer_initerrorlimit(self)  , (self)->tl_error_limit = (tpp_size)(-TPP_ERROR_LIMIT)
#define tpp_lexer_geterrorlimit(self)    ((self)->tl_error_limit)
#define tpp_lexer_seterrorlimit(self, v) (void)((self)->tl_error_limit = (v))
#else /* TPP_ERROR_LIMIT < 0 */
#define _tpp_lexer_initerrorlimit(self)  /* nothing */
#define tpp_lexer_geterrorlimit(self)    TPP_ERROR_LIMIT
#endif /* TPP_ERROR_LIMIT >= 0 */
#else /* TPP_HAVE_WARNING_ERROR */
#define _tpp_lexer_initerrorcount(self)  /* nothing */
#define _tpp_lexer_initerrorlimit(self)  /* nothing */
#define tpp_lexer_geterrorcount(self)    0
#endif /* !TPP_HAVE_WARNING_ERROR */
} tpp_lexer;



#define tpp_lexer_gettoken(self)            (&(self)->tl_core.tlc_tok)
#define tpp_lexer_getfile(self)             (&(self)->tl_core.tlc_input.tli_file)
#define tpp_lexer_getfeat(self, TPP_FEAT_x) tpp_features_get(&(self)->tl_feat, TPP_FEAT_x)
#define tpp_lexer_getext(self, TPP_EXT_x)   tpp_extensions_get(&(self)->tl_exts, TPP_EXT_x)
#if TPP_HAVE_WARNINGS
#define tpp_lexer_getwarn(self) (&(self)->tl_warn)
#endif /* TPP_HAVE_WARNINGS */
#define tpp_lexer_setfeat(self, TPP_FEAT_x, enabled) tpp_features_setid(&(self)->tl_feat, TPP_FEAT_x, enabled)
#define tpp_lexer_enablefeat(self, TPP_FEAT_x)       tpp_features_enable(&(self)->tl_feat, TPP_FEAT_x)
#define tpp_lexer_disablefeat(self, TPP_FEAT_x)      tpp_features_disable(&(self)->tl_feat, TPP_FEAT_x)


/* Wrappers for keywords API */
#define _tpp_lexer_kwds_getkeyword(self, kwd, len, hash) _tpp_keywords_getkeyword(&(self)->tl_kwds, kwd, len, hash)
#define _tpp_lexer_kwds_getkeyword_byid(self, id)        _tpp_keywords_getkeyword_byid(&(self)->tl_kwds, id)
#define tpp_lexer_kwds_getkeyword(self, kwd, len, hash)  tpp_keywords_getkeyword(&(self)->tl_kwds, kwd, len, hash)
#define tpp_lexer_kwds_getkeyword_byid(self, id)         tpp_keywords_getkeyword_byid(&(self)->tl_kwds, id)
#define tpp_lexer_kwds_newkeyword(self, kwd, len, hash)  tpp_keywords_newkeyword(&(self)->tl_kwds, kwd, len, hash)
#if TPP_HAVE_BSE
#define _tpp_lexer_kwds_getkeyword_bse(self, kwd, len, hash, file) _tpp_keywords_getkeyword_esc(&(self)->tl_kwds, kwd, len, hash, file)
#define tpp_lexer_kwds_getkeyword_bse(self, kwd, len, hash, file) tpp_keywords_getkeyword_esc(&(self)->tl_kwds, kwd, len, hash, file)
#define tpp_lexer_kwds_newkeyword_bse(self, kwd, len, hash, file) tpp_keywords_newkeyword_esc(&(self)->tl_kwds, kwd, len, hash, file)
#endif /* TPP_HAVE_BSE */
#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
#define tpp_lexer_kwds_copybuiltin(self, kwd) tpp_keywords_copybuiltin(&(self)->tl_kwds, kwd)
#endif /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */



/* Initialize/finalize everything about "self" except for "tl_core" */
#define _tpp_lexer_init_common(self)           \
	(void)(tpp_keywords_init(&(self)->tl_kwds) \
	       _tpp_lexer_init_exts(self)          \
	       _tpp_lexer_init_feat(self)          \
	       _tpp_lexer_init_state(self)         \
	       _tpp_lexer_init_warn(self)          \
	       _tpp_lexer_init_warnprinter(self)   \
	       _tpp_lexer_initerrorcount(self)     \
	       _tpp_lexer_initerrorlimit(self))
TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_fini_common(tpp_lexer *tpp_restrict self);

/* Finalize a given lexer "self" */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_fini(tpp_lexer *tpp_restrict self);


/* Initialize a lexer that simply reads the given [text,text+text_size) blob. */
#if TPP_HAVE_UNICODE
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ex(tpp_lexer *tpp_restrict self,
                       /*utf-8*/ char const *filename,
                       void const *text, tpp_size text_size,
                       tpp_lcinfo start_lc, tpp_file_encoding encoding);
#define tpp_lexer_init_text_ascii(self, filename, text, text_size, start_lc) \
	tpp_lexer_init_text_ex(self, filename, text, text_size, start_lc, TPP_FILE_ENCODING_ASCII)
#define tpp_lexer_init_text_utf8(self, filename, text, text_size, start_lc) \
	tpp_lexer_init_text_ex(self, filename, text, text_size, start_lc, TPP_FILE_ENCODING_FORCE_UTF8)
#else /* TPP_HAVE_UNICODE */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ascii(tpp_lexer *tpp_restrict self,
                          /*utf-8*/ char const *filename,
                          void const *text, tpp_size text_size,
                       tpp_lcinfo start_lc);
#endif /* !TPP_HAVE_UNICODE */

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
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                     tpp_io_handle handle, tpp_file_ioflags ioflags);
#endif /* TPP_HAVE_LEXER_INIT_IO */

#if TPP_HAVE_LEXER_INIT_FILENAME
/* Initialize a lexer such that it starts reading from "filename"
 * @return: * : TPP_ENOENT: No such file or directory
 * @return: * : TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_init_filename(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *tpp_restrict filename);
#endif /* TPP_HAVE_LEXER_INIT_FILENAME */



/* Read a single character (byte) whilst accounting for BSE (if enabled)
 * and automatically extending the current file if EOF is reached.
 * On true EOF:
 * - *p_result = '\0'
 * - *p_pos = tpp_lexer_getfile(self)->tf_end
 * - return TPP_EOK;
 *
 * @return: TPP_EOK:    Character was read
 * @return: TPP_ENOMEM: Out of memory
 * @return: TPP_EIO:    Failed to read from underlying file */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readchar(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos,
                   tpp_char *tpp_restrict p_result);

#if TPP_HAVE_UNICODE
/* Same as `tpp_lexer_readchar()', but (if the current file's encoding allows
 * it, and IN(*p_pos) points at a multi-byte character), decode a multi-byte
 * character and return it. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_readunichar(tpp_lexer *tpp_restrict self,
                      tpp_char const **tpp_restrict p_pos,
                      tpp_unichar *tpp_restrict p_result);
#endif /* TPP_HAVE_UNICODE */



/* Temporarily modify the lexer state flags */
#if TPP_HAVE_LEXER_STATE_FLAGS
#define tpp_lexer_state_push(self, mask, flags)                         \
	do {                                                                \
		tpp_lexer_state_flags const _tlsp_old_flags = (self)->tl_state; \
		(self)->tl_state = ((self)->tl_state & (tpp_lexer_state_flags)(mask)) | (tpp_lexer_state_flags)(flags)
#define tpp_lexer_state_pop(self, mask, flags)                                                                      \
		(self)->tl_state = ((self)->tl_state & ((tpp_lexer_state_flags)(mask) & ~(tpp_lexer_state_flags)(flags))) | \
		                   (_tlsp_old_flags & ~((tpp_lexer_state_flags)(mask) & ~(tpp_lexer_state_flags)(flags)));  \
	} while (0)
#else /* TPP_HAVE_LEXER_STATE_FLAGS */
#define tpp_lexer_state_push(self, mask, flags) do {
#define tpp_lexer_state_pop(self, mask)         } while (0)
#endif /* !TPP_HAVE_LEXER_STATE_FLAGS */


/* Temporarily disable automatic pop-to-prev-file on EOF */
#define tpp_lexer_autopopfile_pushoff(self) tpp_file_autopopfile_pushoff(tpp_lexer_getfile(self))
#define tpp_lexer_autopopfile_pop(self)     tpp_file_autopopfile_pop(tpp_lexer_getfile(self))


/* Do a raw yield and update `self->tl_tok' in the process, then return `tl_tok.tt_id'.
 * - On EOF, automatically pop `tl_file->tf_prev' and continue reading from there
 * - On error, return one of `TPP_TOK_E*' (e.g. `TPP_TOK_EIO').
 *   Such error codes will NOT be stored in `tl_tok.tt_id'!
 *
 * NOTE: This function does *NOT* deal with:
 * - Preprocessor directives
 * - Builtin macros
 * - User-defined macros
 * - Filtering out comment, line-feed, and whitespace tokens
 *
 * @return: * :                  The newly read token
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldraw(tpp_lexer *tpp_restrict self);

/* Same as `tpp_lexer_yieldraw()', but populate the token from a custom `*p_pos',
 * and don't pop files from the current #include-stack (unless `p_pos' is the top-
 * most file's `tf_pos')
 *
 * NOTES:
 *  - This function will *NOT* populate "tpp_lexer_gettoken(self)->tt_end",
 *    however the value it would have written there is OUT(*p_pos), meaning
 *    you can just use that instead, and call this function multiple times
 *    to yield more than 1 token
 *  - This function can be used to peek future tokens, as it will also expand
 *    the current file when `*p_pos' would go beyond its end. (in this case,
 *    `*p_pos' is updated such that it always remains valid)
 *  - Unlike `tpp_lexer_yieldraw()', this function will *not* modify the
 *    currently loaded file's `tf_pos' (unless `p_pos == &file->tf_pos'),
 *    meaning that if EOF is reached, the file's chunk will only ever be
 *    expanded, but no old data (that would appear before `tf_pos') will
 *    be deallocated
 *  - This function will also not automatically move on to the next file
 *    in line when the current one has been fully exhausted (unless the
 *    given `p_pos == &file->tf_pos'), meaning that TPP_TOK_EOF will be
 *    returned when no more data can be loaded.
 *
 * This is used to implement `tpp_lexer_yieldraw()', which simply
 * passes `p_pos = &tpp_lexer_gettoken(self)->tt_end'
 *
 * @return: * : See `tpp_lexer_yieldraw()' */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at(tpp_lexer *tpp_restrict self, tpp_char const **p_pos);


#undef TPP_HAVE_LEXER_SEEK_BACKUP_FLAGS
#if TPP_HAVE_WARNINGS
#define TPP_HAVE_LEXER_SEEK_BACKUP_FLAGS 1
#else /* ... */
#define TPP_HAVE_LEXER_SEEK_BACKUP_FLAGS 0
#endif /* !... */

typedef struct tpp_lexer_seek_backup {
	tpp_token_id              tlsb_id;    /* Saved token id */
	struct tpp_keyword const *tlsb_kwd;   /* [1..1][valid_if(TPP_TOK_ISKEYWORD(tlsb_id))] Saved token keyword */
	tpp_size                  tlsb_len;   /* Relative length of token */
#if TPP_HAVE_LEXER_SEEK_BACKUP_FLAGS
	tpp_lexer_state_flags     tlsb_state; /* Saved lexer state flags. */
#define _tpp_lexer_seek_backup_restore_state(self, lexer) , (lexer)->tl_state = (self)->tlsb_state
#else /* TPP_HAVE_LEXER_SEEK_BACKUP_FLAGS */
#define _tpp_lexer_seek_backup_restore_state(self, lexer) /* nothing */
#endif /* !TPP_HAVE_LEXER_SEEK_BACKUP_FLAGS */
} tpp_lexer_seek_backup;

/* Save/restore the currently loaded token. This must be done before/after
 * making use of `tpp_lexer_yieldraw_at()' with a custom text pointer:
 * >> tpp_token_id seek_next_token(tpp_lexer *self) {
 * >>     tpp_token_id result;
 * >>     tpp_lexer_seek_backup backup;
 * >>     tpp_char const *pos;
 * >>     pos    = tpp_lexer_seek_begin(self, &backup);
 * >>     result = tpp_lexer_yieldraw_at(self, &pos);
 * >>     if (SHOULD_COMMIT(result)) {
 * >>         tpp_lexer_seek_commit(self, pos);
 * >>     } else {
 * >>         tpp_lexer_seek_rollback(self, &backup);
 * >>     }
 * >>     return result;
 * >> } */
TPP_INLINE TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_lexer_seek_begin(tpp_lexer *tpp_restrict self,
                     tpp_lexer_seek_backup *tpp_restrict backup,
                     bool disable_warnings) {
	tpp_char const *result;
	tpp_token *const token = tpp_lexer_gettoken(self);
	backup->tlsb_id  = token->tt_id;
	backup->tlsb_kwd = token->tt_kwd;
	backup->tlsb_len = (tpp_size)(token->tt_end - token->tt_start);
#if TPP_HAVE_LEXER_SEEK_BACKUP_FLAGS
	backup->tlsb_state = self->tl_state;
#endif /* TPP_HAVE_LEXER_SEEK_BACKUP_FLAGS */
#if TPP_HAVE_WARNINGS
	if (disable_warnings)
		self->tl_state |= TPP_LEXER_STATE_FLAG_NOWARNINGS;
#else /* TPP_HAVE_WARNINGS */
	(void)disable_warnings;
#endif /* !TPP_HAVE_WARNINGS */
	result        = token->tt_end;
	token->tt_end = token->tt_start;
	return result;
}
#define tpp_lexer_seek_commit(self, pos) \
	(void)(tpp_lexer_gettoken(self)->tt_end = (pos))
#define tpp_lexer_seek_rollback(self, backup)                                                   \
	(tpp_lexer_gettoken(self)->tt_kwd = (backup)->tlsb_kwd,                                     \
	 tpp_lexer_gettoken(self)->tt_start = tpp_lexer_gettoken(self)->tt_end,                     \
	 tpp_lexer_gettoken(self)->tt_end = tpp_lexer_gettoken(self)->tt_start + (backup)->tlsb_len \
	 _tpp_lexer_seek_backup_restore_state(backup, self),                                        \
	 tpp_lexer_gettoken(self)->tt_id  = (backup)->tlsb_id)


/* Wrapper around `tpp_lexer_yieldraw()' that filters certain tokens (based on
 * configured features), and implements handling for preprocessor directives,
 * like "#define", "#include", etc:
 * - TPP_TOK_LF:      Filtered based on `TPP_HAVE_TPP_TOK_LF' / `TPP_FEAT_TPP_TOK_LF'
 * - TPP_TOK_SPACE:   Filtered based on `TPP_HAVE_TPP_TOK_SPACE' / `TPP_FEAT_TPP_TOK_SPACE'
 * - TPP_TOK_COMMENT: Filtered based on `TPP_HAVE_TPP_TOK_COMMENT' / `TPP_FEAT_TPP_TOK_COMMENT'
 *
 * @return: * :                  The newly read token (after accounting for preprocessor directives)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp(tpp_lexer *tpp_restrict self);


/* Wrapper around `tpp_lexer_yieldpp()' that adds handling for macro expansion.
 * @return: * :                  The newly read token (after accounting for macros)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield(tpp_lexer *tpp_restrict self);



#if TPP_HAVE_FILE_NONBLOCK
/* Same as `tpp_lexer_yield()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_blocking(tpp_lexer *tpp_restrict self);

/* Same as `tpp_lexer_yieldraw()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
#define tpp_lexer_yieldraw_blocking(self) \
	tpp_lexer_yieldraw_at_blocking(self, &tpp_lexer_gettoken(self)->tt_end)

/* Same as `tpp_lexer_yieldraw_at()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_blocking(tpp_lexer *tpp_restrict self, tpp_char const **p_pos);
#else /* TPP_HAVE_FILE_NONBLOCK */
#define tpp_lexer_yield_blocking(self)              tpp_lexer_yield(self)
#define tpp_lexer_yieldraw_blocking(self)           tpp_lexer_yieldraw(self)
#define tpp_lexer_yieldraw_at_blocking(self, p_pos) tpp_lexer_yieldraw_at(self, p_pos)
#endif /* !TPP_HAVE_FILE_NONBLOCK */




#if TPP_HAVE_LEXER_SKIP
/* Check that the currently loaded token is 'tok'. If so, "tpp_lexer_yield_blocking()" to
 * the next token (which is also returned). Otherwise, trigger 'TPP_W_UNEXPECTED_TOKEN'
 * and (if that warning wasn't fatal), try to seek ahead to see if "tok" can be found
 * somewhere close by (depending on what 'tok' and what was actually loaded on entry)
 *
 * @return: * :                  The token that comes after the one that was just skipped
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip(tpp_lexer *tpp_restrict self, tpp_token_id tok);

#if TPP_HAVE_FILE_NONBLOCK
/* Same as `tpp_lexer_skip()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip_blocking(tpp_lexer *tpp_restrict self, tpp_token_id tok);
#else /* TPP_HAVE_FILE_NONBLOCK */
#define tpp_lexer_skip_blocking(self, tok) tpp_lexer_skip(self, tok)
#endif /* !TPP_HAVE_FILE_NONBLOCK */
#endif /* TPP_HAVE_LEXER_SKIP */


#if TPP_HAVE_LEXER_SEEK_RPAREN
typedef struct tpp_lexer_arginfo {
	/* NOTE: Leading/trailing whitespace in arguments is controlled by "TPP_HAVE_MACRO_ARGUMENT_WHITESPACE" */
	tpp_char const *tlai_start; /* [1..1][<= tlai_end] Pointer to argument start text data */
	tpp_char const *tlai_end;   /* [1..1][>= tlai_start] Pointer to argument end text data */
} tpp_lexer_arginfo;


#define TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL     0x0000
#define TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS    0x0001 /* Store varargs info in p_argv[IN(*p_argc) - 1] */
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
#define TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC 0x0002 /* Do not strip whitespace/comments around arguments */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */

/* Find the position of the next unmatched ')'-token, gathering information
 * about ranges of ','-token-separated text-sequences along the way.
 *
 * NOTE: This function automatically handles "TPP_TOK_EWOULDBLOCK" (meaning it always blocks)
 *
 * @param: p_pos: [in/out]  In:  Pointer to first character to start checking for ')'
 *                          Out: Pointer to the trailing ')', or EOF
 *                               if end-of-file was reached first, or
 *                               in case of TPP_TOK_EWOULDBLOCK.
 *                          WARNING: must DIFFER from "&tpp_lexer_gettoken(self)->tt_end"!
 * @param: p_argv: [out]    Output buffer for the bounds of macro
 *                          arguments encountered along the way.
 *                          The size of this buffer is IN(*p_argc)
 * @param: p_argc: [in/out] In:  Size of provided "p_argv" buffer (in elements)
 *                          Out: Number of arguments actually encountered. May
 *                               be set to a number greater tha IN(*p_argc), in
 *                               which case only info about the first IN(*p_argc)
 *                               arguments is actually returned. But note the
 *                               `TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS' flag, which
 *                               causes the last element of `p_argv' to span up
 *                               to the closing ')'
 * @param: opt_function_name_for_messages:
 *                          Function name for too-many-arguments/end-of-params message
 * @param: flags:           Set of `TPP_LEXER_SEEK_RPAREN_FLAG_*'
 *
 * @return: TPP_TOK_EOF:         EOF was encountered before an unmatched ')' was found
 * @return: TPP_TOK_RPAREN:      Unmatched closing ')' was encountered
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
#define tpp_lexer_seek_rparen(self, p_pos, p_argv, p_argc, opt_function_name_for_messages, flags) \
	tpp_lexer_seek_rparen_ex(self, p_pos, p_argv, p_argc, opt_function_name_for_messages, flags, TPP_TOK_LPAREN)
#else /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seek_rparen(tpp_lexer *tpp_restrict self,
                      tpp_char const **tpp_restrict p_pos,
                      tpp_lexer_arginfo *tpp_restrict p_argv,
                      tpp_size *tpp_restrict p_argc,
                      char const *opt_function_name_for_messages,
                      unsigned int flags);
#if TPP_BUILDING
#define tpp_lexer_seek_rparen_ex(self, p_pos, p_argv, p_argc, opt_function_name_for_messages, flags, lparen_kind) \
	tpp_lexer_seek_rparen(self, p_pos, p_argv, p_argc, opt_function_name_for_messages, flags)
#endif /* TPP_BUILDING */
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN_EX */

/* Same as `tpp_lexer_seek_rparen()', but also able to accept alternate
 * parenthesis, depending on "lparen_kind"
 * @param: lparen_kind: One of 'TPP_TOK_LPAREN', 'TPP_TOK_LBRACKET',
 *                      'TPP_TOK_LBRACE' or 'TPP_TOK_LANGLE'.
 * @return: * : See `tpp_lexer_seek_rparen()' */
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seek_rparen_ex(tpp_lexer *tpp_restrict self,
                         tpp_char const **tpp_restrict p_pos,
                         tpp_lexer_arginfo *tpp_restrict p_argv,
                         tpp_size *tpp_restrict p_argc,
                         char const *opt_function_name_for_messages,
                         unsigned int flags,
                         tpp_token_id lparen_kind);
#endif /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
#endif /* TPP_HAVE_LEXER_SEEK_RPAREN */





#if TPP_HAVE_KEYWORD_FLAGS
/* Return the effective set of flags for a given "kwd"
 * Since the effective flags for (certain) builtin keywords
 * can depend on active extensions/features, this can only
 * be done in the context of a specific lexer (rather than
 * stand-alone using only the "kwd")
 *
 * @return: * : Set of `TPP_KEYWORD_FLAG_*' */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_flags TPPCALL
tpp_lexer_getkeywordflags(tpp_lexer *tpp_restrict self,
                          tpp_keyword const *tpp_restrict kwd);
#endif /* TPP_HAVE_KEYWORD_FLAGS */

#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
/* Returns true if "kwd" should be considered to be "#if defined()"
 * Since "builtin" keywords can be considered to be "defined", even
 * when `kwd->tk_macro == NULL', this function is needed to handle
 * such macros. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_getkeyworddefined(tpp_lexer *tpp_restrict self,
                            tpp_keyword const *tpp_restrict kwd);
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */


#if TPP_HAVE_TPP_TOK_STRINGLIKE

/* Print the unescaped representation of the string-token described by "self"
 * The caller must ensure that `TPP_TOK_ISSTRING(tpp_lexer_gettoken(self)->tt_id)'
 *
 * @param: data_printer: Printer used to fast-forward string data from token inputs, as well as \xAB
 * @param: utf8_printer: Printer used to emit explicitly utf-8 encoded data from \uABCD and \U876543210,
 *                       as well as regular text-data when the "tpp_file_isutf8(tpp_lexer_getfile(self))"
 *
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers
 * @return: (tpp_ssize)TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                                     a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: (tpp_ssize)TPP_ENOMEM:     Out of memory  (can only happen inside of `tpp_lexer_warnf()')
 * @return: (tpp_ssize)TPP_EWARNPRINT: Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_ssize TPPCALL
tpp_lexer_decodestring(tpp_lexer *tpp_restrict self,
                       tpp_formatprinter data_printer,
                       tpp_formatprinter utf8_printer,
                       void *arg);

/* Flags for `tpp_lexer_parsestring()' & friends. */
#define TPP_LEXER_PARSESTRING_FLAG_NORMAL      0x0000 /* Normal flags */
#define TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE 0x0001 /* Stop if a NOLINE-COMMENT or SPACE token is hit */
#define TPP_LEXER_PARSESTRING_FLAG_STOPONLF    0x0002 /* Stop if a LINE-COMMENT or LF token is hit */

/* Same as "tpp_lexer_decodestring()", but also "tpp_lexer_yield()" to the next token.
 * Then, if that token is also string-like (TPP_TOK_ISSTRING()), decode it also,
 * then yield again, and so on, until a non-string-like token is encountered, an
 * error happens, or one of the printers returned a negative value.
 *
 * HINT: This function automatically handles "TPP_EWOULDBLOCK" during
 *       yield by trying again with TPP_FILE_IOFLAGS_NONBLOCK disabled.
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers
 * @return: (tpp_ssize)TPP_ELEXERROR:   Either one of the printers returned this value, or
 *                                      a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: (tpp_ssize)TPP_ENOMEM:      Out of memory
 * @return: (tpp_ssize)TPP_EIO:         I/O error while yielding to next token
 * @return: (tpp_ssize)TPP_EWARNPRINT:  Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_ssize TPPCALL
tpp_lexer_parsestring_ex(tpp_lexer *tpp_restrict self,
                         tpp_formatprinter data_printer,
                         tpp_formatprinter utf8_printer,
                         void *arg, unsigned int flags);

/* Convenience wrapper around `tpp_lexer_parsestring_ex()'
 * On success (!TPP_ISERR(return)), caller must "tpp_string_decref(*p_result)"
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring(tpp_lexer *tpp_restrict self,
                      /*out*/ TPP_REF tpp_string **tpp_restrict p_result,
                      unsigned int flags);

/* Wrapper around `tpp_lexer_parsestring()' that passes the actual string data
 * to a given callback. This function also enables some (optional) optimizations
 * for the most common case where the string token in "self" isn't followed by
 * another string token, and can be printed as a singular chunk. When this is
 * the case, no intermediate heap-buffer needs to be created, as "cb" can just
 * be invoked using the currently loaded file's content-buffer.
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning
 * @return: * :             Return value of given "cb" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring_cb(tpp_lexer *tpp_restrict self,
                         tpp_errno (TPPCALL *cb)(void *arg, tpp_char const *str, tpp_size length),
                         void *arg, unsigned int flags);
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE */


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
 * @param: pos:     Lexer position used by certain format-patterns.
 * @param: file:    The file containing "pos"
 * @param: printer: Output printer for formatted text
 * @param: arg:     Cookie argument for "printer"
 * @param: format:  Format pattern (see above)
 * @param: args:    Extra varargs-arguments for "format"
 * @return: >= 0:   Sum of return values of "printer".
 * @return: < 0:    First negative return value of "printer". The more high-level
 *                  "tpp_lexer_warnf" API returns "TPP_EWARNPRINT" in this case. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4, 6)) tpp_ssize TPPVCALL
tpp_lexer_printf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                         tpp_formatprinter printer, void *arg,
                         char const *format, ...);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4, 6)) tpp_ssize TPPCALL
tpp_lexer_vprintf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                          tpp_formatprinter printer, void *arg,
                          char const *format, va_list args);

/* Emits the specified lexer warning at the start of the current token.
 * @param: args: Format arguments specific to "id" (see '%'-sequences in warning expressions)
 * @return: TPP_EOK:        Warning was emitted, but you may proceed
 * @return: TPP_ELEXERROR:  Warning was emitted, but was configured as an error
 * @return: TPP_EWARNPRINT: Error while printing warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, va_list args);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, ...);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_vwarnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos, tpp_warning_id id, va_list args);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos, tpp_warning_id id, ...);
#else /* TPP_HAVE_WARNINGS */
#define tpp_lexer_vwarnf(self, id, args)         TPP_EOK
#define tpp_lexer_vwarnf_at(self, pos, id, args) TPP_EOK
#if TPP_HOST_HAVE_PP_VARARGS
#define tpp_lexer_warnf(self, id, ...)           TPP_EOK
#define tpp_lexer_warnf_at(self, pos, id, ...)   TPP_EOK
#else /* TPP_HOST_HAVE_PP_VARARGS */
TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, ...) {
	(void)self;
	(void)id;
	return TPP_EOK;
}

TPP_INLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos, tpp_warning_id id, ...) {
	(void)self;
	(void)pos;
	(void)id;
	return TPP_EOK;
}
#endif /* !TPP_HOST_HAVE_PP_VARARGS */
#endif /* !TPP_HAVE_WARNINGS */


#if TPP_HAVE_LEXER_REPRTOKENID
/* Return the (canonical) string-representation of a given token ID */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) char const *TPPCALL
tpp_lexer_reprtokenid(tpp_lexer const *tpp_restrict self, tpp_token_id tok);
#endif /* TPP_HAVE_LEXER_REPRTOKENID */




#ifndef TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER
#define TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER 0
#endif /* !TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER */
#ifndef TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
#define TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER 0
#endif /* !TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */
#ifndef TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER
#define TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER 0
#endif /* !TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER */

#if TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER
TPP_DECL tpp_ssize TPP_FORMATPRINTER_CC _tpp_lexer_wrapped_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes);
#endif /* TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER */
#if TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
TPP_DECL tpp_ssize TPP_FORMATPRINTER_CC _tpp_lexer_builtin_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes);
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */
#if TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER
TPP_DECL tpp_ssize TPP_FORMATPRINTER_CC _tpp_lexer_noop_warnprinter(void *arg, tpp_char const *text, tpp_size num_bytes);
#endif /* TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_H */
