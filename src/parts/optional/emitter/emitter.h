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
#ifndef GUARD_TPP_OPTIONAL_EMITTER_EMITTER_H
#define GUARD_TPP_OPTIONAL_EMITTER_EMITTER_H 1

#include "api.h"

#include "config.h"
#include "emitter-features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#undef TPP_EMITTER_HAVE_CURPOS
#if TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
#define TPP_EMITTER_HAVE_CURPOS 1
#else /* ... */
#define TPP_EMITTER_HAVE_CURPOS 0
#endif /* !... */

#if TPP_EMITTER_HAVE_CURPOS
typedef struct tpp_emitter_state_file {
	tpp_lcstate         TPP_EMITTER_INTERNAL(temsf_curpos);    /* Current line/column position in output (with respect to emitted `#line` directives) */
	char const         *TPP_EMITTER_INTERNAL(temsf_fname);     /* [0..1] The filename (tpp_file_getfilename()) that goes with `tems_curpos` (or "NULL" if unknown, or this is the first token) */
	TPP_REF tpp_string *TPP_EMITTER_INTERNAL(temsf_fname_str); /* [0..1] Same as `tems_curfilename`, but keeps a reference to `tpp_file_getfilenamestr()` so custom filename overrides aren't free'd early */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#if TPP_HAVE_FILE_SYSHDR && TPP_HAVE_FILE_EXTERN_C
#define _TPP_EMITTER_STATE_FLAGS_MASK (TPP_FILE_FLAGS_SYSHDR | TPP_FILE_FLAGS_EXTERN_C)
#elif TPP_HAVE_FILE_SYSHDR
#define _TPP_EMITTER_STATE_FLAGS_MASK TPP_FILE_FLAGS_SYSHDR
#elif TPP_HAVE_FILE_EXTERN_C
#define _TPP_EMITTER_STATE_FLAGS_MASK TPP_FILE_FLAGS_EXTERN_C
#else /* ... */
#define _TPP_EMITTER_STATE_FLAGS_MASK 0
#endif /* !... */
#if _TPP_EMITTER_STATE_FLAGS_MASK
	tpp_file_flags          TPP_EMITTER_INTERNAL(temsf_flags); /* Set of `_TPP_EMITTER_STATE_FLAGS_MASK` */
#define _TPP_EMITTER_STATE_FILE_INIT_FLAGS(self) , 0
#define _tpp_emitter_state_file_init_flags(self) , (self)->TPP_EMITTER_INTERNAL(temsf_flags) = 0
#endif /* _TPP_EMITTER_STATE_FLAGS_MASK */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#ifndef _tpp_emitter_state_file_init_flags
#define _TPP_EMITTER_STATE_FILE_INIT_FLAGS(self) /* nothing */
#define _tpp_emitter_state_file_init_flags(self) /* nothing */
#endif /* !_tpp_emitter_state_file_init_flags */
} tpp_emitter_state_file;

#define TPP_EMITTER_STATE_FILE_INIT(self)                                                                                 \
	{                                                                                                                     \
		/* .TPP_EMITTER_INTERNAL(temsf_curpos)    = */ TPP_LCSTATE_INIT((self).TPP_EMITTER_INTERNAL(temsf_curpos), 0, 0), \
		/* .TPP_EMITTER_INTERNAL(temsf_fname)     = */ NULL,                                                              \
		/* .TPP_EMITTER_INTERNAL(temsf_fname_str) = */ NULL                                                               \
		_TPP_EMITTER_STATE_FILE_INIT_FLAGS(self)                                                                          \
	}
#define tpp_emitter_state_file_init(self)                                       \
	(void)(tpp_lcstate_init(&(self)->TPP_EMITTER_INTERNAL(temsf_curpos), 0, 0), \
	       (self)->TPP_EMITTER_INTERNAL(temsf_fname)     = NULL,                \
	       (self)->TPP_EMITTER_INTERNAL(temsf_fname_str) = NULL                 \
	       _tpp_emitter_state_file_init_flags(self))

typedef struct tpp_emitter_state_files {
	tpp_emitter_state_file  TPP_EMITTER_INTERNAL(temsfs_file);    /* Most-recent file */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	tpp_size                TPP_EMITTER_INTERNAL(temsfs_filec);   /* # of dummy files pushed by `# <digit> "filename" 1` */
	tpp_emitter_state_file *TPP_EMITTER_INTERNAL(temsfs_filev);   /* [0..temsfs_filec][owned] Extra files pus */
#define _TPP_EMITTER_STATE_FILES_INIT_FILEC(self) , 0, NULL
#define _tpp_emitter_state_files_init_filec(self) , (self)->TPP_EMITTER_INTERNAL(temsfs_filec) = 0, (self)->TPP_EMITTER_INTERNAL(temsfs_filev) = NULL
#else /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#define _TPP_EMITTER_STATE_FILES_INIT_FILEC(self) /* nothing */
#define _tpp_emitter_state_files_init_filec(self) /* nothing */
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
} tpp_emitter_state_files;

#define TPP_EMITTER_STATE_FILES_INIT(self)                                                                               \
	{                                                                                                                    \
		/* .TPP_EMITTER_INTERNAL(temsfs_file) = */ TPP_EMITTER_STATE_FILE_INIT((self).TPP_EMITTER_INTERNAL(temsfs_file)) \
		_TPP_EMITTER_STATE_FILES_INIT_FILEC(self)                                                                        \
	}
#define tpp_emitter_state_files_init(self)                                         \
	(void)(tpp_emitter_state_file_init(&(self)->TPP_EMITTER_INTERNAL(temsfs_file)) \
	       _tpp_emitter_state_files_init_filec(self))
#endif /* TPP_EMITTER_HAVE_CURPOS */

#undef TPP_EMITTER_HAVE_FLAGS
#if (TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || \
     TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
#define TPP_EMITTER_HAVE_FLAGS 1
#else /* ... */
#define TPP_EMITTER_HAVE_FLAGS 0
#endif /* !... */

#if TPP_EMITTER_HAVE_FLAGS
#define tpp_emitter_flags tpp_uint_least8
#define TPP_EMITTER_FLAG_NORMAL   TPP_UINT_LEAST8_C(0x00)
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#define TPP_EMITTER_FLAG_FCHANGED TPP_UINT_LEAST8_C(0x01) /* Contents of the #include-stack may have changed */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
#define TPP_EMITTER_FLAG_HASLINE  TPP_UINT_LEAST8_C(0x02) /* At least 1 `# <linenum>`-directive was emitted */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */
#endif /* TPP_EMITTER_HAVE_FLAGS */


typedef struct tpp_emitter_state {
	/* Last token ID (preceding the token currently being emitted).
	 * When the current token is the first, this is `TPP_TOK_EOF` */
	tpp_token_id TPP_EMITTER_INTERNAL(tems_prevtok);

	/* Current file-state. */
#if TPP_EMITTER_HAVE_CURPOS
	tpp_emitter_state_files TPP_EMITTER_INTERNAL(tems_curfile);
#define _TPP_EMITTER_STATE_INIT_CURFILE(self) , TPP_EMITTER_STATE_FILES_INIT((self).TPP_EMITTER_INTERNAL(tems_curfile))
#define _tpp_emitter_state_init_curfile(self) , tpp_emitter_state_files_init(&(self)->TPP_EMITTER_INTERNAL(tems_curfile))
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	tpp_size                TPP_EMITTER_INTERNAL(tems_cached_filec); /* Size of alternate file-state buffer (`tems_cached_filev`). */
	tpp_emitter_state_file *TPP_EMITTER_INTERNAL(tems_cached_filev); /* [0..tems_cached_filec] Alternate file-state buffer (used internally) */
#define _TPP_EMITTER_STATE_INIT_CACHED_FILE(self) , 0, NULL
#define _tpp_emitter_state_init_cached_file(self) , (self)->TPP_EMITTER_INTERNAL(tems_cached_filec) = 0, (self)->TPP_EMITTER_INTERNAL(tems_cached_filev) = NULL
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#endif /* TPP_EMITTER_HAVE_CURPOS */
#ifndef _tpp_emitter_state_init_cached_file
#define _TPP_EMITTER_STATE_INIT_CACHED_FILE(self) /* nothing */
#define _tpp_emitter_state_init_cached_file(self) /* nothing */
#endif /* !_tpp_emitter_state_init_cached_file */

	/* Emitter flags */
#if TPP_EMITTER_HAVE_FLAGS
	tpp_emitter_flags TPP_EMITTER_INTERNAL(tems_flags);
#define _TPP_EMITTER_STATE_INIT_FLAGS(self) , TPP_EMITTER_FLAG_NORMAL
#define _tpp_emitter_state_init_flags(self) , (self)->TPP_EMITTER_INTERNAL(tems_flags) = TPP_EMITTER_FLAG_NORMAL
#else /* TPP_EMITTER_HAVE_FLAGS */
#define _TPP_EMITTER_STATE_INIT_FLAGS(self) /* nothing */
#define _tpp_emitter_state_init_flags(self) /* nothing */
#endif /* !TPP_EMITTER_HAVE_FLAGS */
} tpp_emitter_state;

#define TPP_EMITTER_STATE_INIT(self)                            \
	{                                                           \
		/* .TPP_EMITTER_INTERNAL(tems_prevtok) = */ TPP_TOK_EOF \
		_TPP_EMITTER_STATE_INIT_CURFILE(self)                   \
		_TPP_EMITTER_STATE_INIT_FLAGS(self)                     \
	}
#define tpp_emitter_state_init(self)                                \
	(void)((self)->TPP_EMITTER_INTERNAL(tems_prevtok) = TPP_TOK_EOF \
	       _tpp_emitter_state_init_curfile(self)                    \
	       _tpp_emitter_state_init_cached_file(self)                \
	       _tpp_emitter_state_init_flags(self))


#undef _TPP_EMITTER_MODE_DEFAULT
#undef TPP_EMITTER_MODE_HAVE_MULTIPLE
typedef enum tpp_emitter_mode {

	/* Emit tokens to the emitter output (do what e.g. `gcc -E` does) */
#if TPP_EMITTER_HAVE_MODE_EMIT
	TPP_EMITTER_MODE_EMIT,
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_EMIT
#endif /* TPP_EMITTER_HAVE_MODE_EMIT */

	/* Dispose tokens (output can only be produced by hooks or "raw" printing) */
#if TPP_EMITTER_HAVE_MODE_DISPOSE
	TPP_EMITTER_MODE_DISPOSE,
#ifndef _TPP_EMITTER_MODE_DEFAULT
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_DISPOSE
#else /* !_TPP_EMITTER_MODE_DEFAULT */
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 1
#endif /* _TPP_EMITTER_MODE_DEFAULT */
#endif /* TPP_EMITTER_HAVE_MODE_DISPOSE */

	/* Print tokens in [brackets] */
#if TPP_EMITTER_HAVE_MODE_BRACKET
	TPP_EMITTER_MODE_BRACKET,
#ifndef _TPP_EMITTER_MODE_DEFAULT
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_BRACKET
#else /* !_TPP_EMITTER_MODE_DEFAULT */
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 1
#endif /* _TPP_EMITTER_MODE_DEFAULT */
#endif /* TPP_EMITTER_HAVE_MODE_BRACKET */

	/* Print tokens as `[{TYPE}:{TOKEN}]` */
#if TPP_EMITTER_HAVE_MODE_TYPED
	TPP_EMITTER_MODE_TYPED,
#ifndef _TPP_EMITTER_MODE_DEFAULT
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_TYPED
#else /* !_TPP_EMITTER_MODE_DEFAULT */
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 1
#endif /* _TPP_EMITTER_MODE_DEFAULT */
#endif /* TPP_EMITTER_HAVE_MODE_TYPED */

	/* Print tokens as `{TOKEN}\0` */
#if TPP_EMITTER_HAVE_MODE_ZERO
	TPP_EMITTER_MODE_ZERO,
#ifndef _TPP_EMITTER_MODE_DEFAULT
#define _TPP_EMITTER_MODE_DEFAULT TPP_EMITTER_MODE_ZERO
#else /* !_TPP_EMITTER_MODE_DEFAULT */
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 1
#endif /* _TPP_EMITTER_MODE_DEFAULT */
#endif /* TPP_EMITTER_HAVE_MODE_ZERO */

} tpp_emitter_mode;

#ifndef TPP_EMITTER_MODE_HAVE_MULTIPLE
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 0
#endif /* !TPP_EMITTER_MODE_HAVE_MULTIPLE */


typedef struct tpp_emitter {
	/* [1..1][const] The lexer whose tokens are being emitted */
#ifndef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
	tpp_lexer *TPP_EMITTER_INTERNAL(tem_lexer);
#define _TPP_EMITTER_INIT_LEXER(self, lexer) (lexer),
#define _tpp_emitter_init_lexer(self, lexer) (self)->TPP_EMITTER_INTERNAL(tem_lexer) = (lexer)
#else /* TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define _TPP_EMITTER_INIT_LEXER(self, lexer) /* nothing */
#define _tpp_emitter_init_lexer(self, lexer) tpp_assert(tpp_emitter_getlexer(self) == (lexer))
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */

	/* [1..1][const] Emitter output printer (the emitter itself will be passed as argument) */
	tpp_formatprinter TPP_EMITTER_INTERNAL(tem_output);

	/* Emitter output state */
	tpp_emitter_state TPP_EMITTER_INTERNAL(tem_state);

	/* Emitter feature configuration */
#if TPP_EMITTER_HAVE_FEATURES
	tpp_emitter_features TPP_EMITTER_INTERNAL(tem_feat);
#define _TPP_EMITTER_INIT_FEAT(self) , TPP_EMITTER_FEATURES_INIT((self).TPP_EMITTER_INTERNAL(tem_feat))
#define _tpp_emitter_init_feat(self) , tpp_emitter_features_init(&(self)->TPP_EMITTER_INTERNAL(tem_feat))
#define _tpp_emitter_fini_feat(self) , tpp_emitter_features_fini(&(self)->TPP_EMITTER_INTERNAL(tem_feat))
#else /* TPP_EMITTER_HAVE_FEATURES */
#define _TPP_EMITTER_INIT_FEAT(self) /* nothing */
#define _tpp_emitter_init_feat(self) /* nothing */
#define _tpp_emitter_fini_feat(self) /* nothing */
#endif /* !TPP_EMITTER_HAVE_FEATURES */

	/* Mode in which tokens are emitted. */
#if TPP_EMITTER_MODE_HAVE_MULTIPLE
	tpp_emitter_mode TPP_EMITTER_INTERNAL(tem_mode);
#define _TPP_EMITTER_INIT_MODE(self) , _TPP_EMITTER_MODE_DEFAULT
#define _tpp_emitter_init_mode(self) , (self)->TPP_EMITTER_INTERNAL(tem_mode) = _TPP_EMITTER_MODE_DEFAULT
#define tpp_emitter_getmode(self)    ((self)->TPP_EMITTER_INTERNAL(tem_mode))
#define tpp_emitter_setmode(self, v) (void)((self)->TPP_EMITTER_INTERNAL(tem_mode) = (v))
#else /* TPP_EMITTER_MODE_HAVE_MULTIPLE */
#define _TPP_EMITTER_INIT_MODE(self) /* nothing */
#define _tpp_emitter_init_mode(self) /* nothing */
#define tpp_emitter_getmode(self)    _TPP_EMITTER_MODE_DEFAULT
#define tpp_emitter_setmode(self, v) (void)(v)
#endif /* !TPP_EMITTER_MODE_HAVE_MULTIPLE */

	/* max # of blank lines emitted for alignment purposes */
#if TPP_EMITTER_CONFIG_LINE_THRESHOLD < 0
	tpp_line TPP_EMITTER_INTERNAL(tem_linethreshold);
#define _TPP_EMITTER_INIT_LINETHRESHOLD(self)  , (-TPP_EMITTER_CONFIG_LINE_THRESHOLD)
#define _tpp_emitter_init_linethreshold(self)  , (self)->TPP_EMITTER_INTERNAL(tem_linethreshold) = (-TPP_EMITTER_CONFIG_LINE_THRESHOLD)
#define tpp_emitter_getlinethreshold(self)     ((self)->TPP_EMITTER_INTERNAL(tem_linethreshold))
#define tpp_emitter_setlinethreshold(self, v)  (void)((self)->TPP_EMITTER_INTERNAL(tem_linethreshold) = (tpp_line)(v))
#define tpp_emitter_disablelinethreshold(self) (void)((self)->TPP_EMITTER_INTERNAL(tem_linethreshold) = -1)
#else /* TPP_EMITTER_CONFIG_LINE_THRESHOLD < 0 */
#define _TPP_EMITTER_INIT_LINETHRESHOLD(self) /* nothing */
#define _tpp_emitter_init_linethreshold(self) /* nothing */
#if !TPP_EMITTER_CONFIG_LINE_THRESHOLD
#define tpp_emitter_getlinethreshold(self) (-1)
#else /* !TPP_EMITTER_CONFIG_LINE_THRESHOLD */
#define tpp_emitter_getlinethreshold(self) TPP_EMITTER_CONFIG_LINE_THRESHOLD
#endif /* TPP_EMITTER_CONFIG_LINE_THRESHOLD */
#endif /* TPP_EMITTER_CONFIG_LINE_THRESHOLD >= 0 */
} tpp_emitter;

/* Static initializer */
#define TPP_EMITTER_INIT(self, lexer, output)                                                                    \
	{                                                                                                            \
		_TPP_EMITTER_INIT_LEXER(self, lexer)                                                                     \
		/* .TPP_EMITTER_INTERNAL(tem_output) = */ (output),                                                      \
		/* .TPP_EMITTER_INTERNAL(tem_state)  = */ TPP_EMITTER_STATE_INIT((self).TPP_EMITTER_INTERNAL(tem_state)) \
		_TPP_EMITTER_INIT_FEAT(self)                                                                             \
		_TPP_EMITTER_INIT_MODE(self)                                                                             \
		_TPP_EMITTER_INIT_LINETHRESHOLD(self)                                                                    \
	}

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given emitter.
 *
 * @param: output: Output printer. On error, must return one of `TPP_SSIZE_OFERR(*)`
 * @param: lexer:  The lexer whose tokens are being emitted */
#define tpp_emitter_init(self, lexer, output)                               \
	(void)(_tpp_emitter_init_lexer(self, lexer),                            \
	       (self)->TPP_EMITTER_INTERNAL(tem_output) = (output),             \
	       tpp_emitter_state_init(&(self)->TPP_EMITTER_INTERNAL(tem_state)) \
	       _tpp_emitter_init_feat(self)                                     \
	       _tpp_emitter_init_mode(self)                                     \
	       _tpp_emitter_init_linethreshold(self))
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_emitter_fini(tpp_emitter *tpp_restrict self);


/* Register default lexer hooks. This function should be called at least
 * once before you start yielding tokens from the associated lexer. Otherwise,
 * the emitter may not be properly informed about all relevant state changes
 * that might happen within the associated lexer.
 *
 * If you're using `tpp_emitter_cli_loader`, this function is automatically
 * called by `tpp_emitter_cli_loader_flush()`
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
	/* Enable default hooks */
#if (TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) || \
     TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_TRACE_INCLUDES) ||                \
     TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS))
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_registerdefaulthooks(tpp_emitter *tpp_restrict self);
#else /* ... */
#define tpp_emitter_registerdefaulthooks(self) TPP_EOK
#endif /* !... */


/* Retrieve components of the emitter. */
#define tpp_emitter_getoutput(self) (self)->TPP_EMITTER_INTERNAL(tem_output)
#ifdef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
#define tpp_emitter_getlexer(self)  ((tpp_lexer *)((char *)(self) - TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER))
#define _tpp_emitter_oflexer(lexer) ((tpp_emitter *)((char *)(lexer) + TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER))
#else /* TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define tpp_emitter_getlexer(self)  ((self)->TPP_EMITTER_INTERNAL(tem_lexer))
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */

/* Helpers for quickly printing stuff to the emitter's output.
 * WARNING: Careless use of these functions may result in the emitter's
 *          idea of its current output file/line/column becoming invalid. */
#define tpp_emitter_output_printraw(self, text, len) \
	tpp_formatprinter_print(tpp_emitter_getoutput(self), self, text, len)
#define tpp_emitter_output_printraw_cstr(self, text, len) \
	tpp_formatprinter_print_cstr(tpp_emitter_getoutput(self), self, text, len)
#define tpp_emitter_output_printraw_conststr(self, CONSTstr) \
	tpp_formatprinter_print_conststr(tpp_emitter_getoutput(self), self, CONSTstr)

/* Check if a runtime-configurable config option `conf` in `TPP_EMITTER_HAVE_<conf>` is currently enabled.
 * When `TPP_EMITTER_HAVE_<conf>` is configured as `TPP_CONF_ISCONST()`, return that constant instead. */
#define tpp_emitter_has(self, conf) _tpp_emitter_has_##conf(self)

/* Features... */
#if TPP_EMITTER_HAVE_FEATURES
#define tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_x)          tpp_emitter_features_getid(&(self)->TPP_EMITTER_INTERNAL(tem_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_x, enabled) tpp_emitter_features_setid(&(self)->TPP_EMITTER_INTERNAL(tem_feat), TPP_EMITTER_FEAT_x, enabled)
#define tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_x)       tpp_emitter_features_enable(&(self)->TPP_EMITTER_INTERNAL(tem_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_x)      tpp_emitter_features_disable(&(self)->TPP_EMITTER_INTERNAL(tem_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_resetfeatures(self)                           tpp_emitter_features_reset(&(self)->TPP_EMITTER_INTERNAL(tem_feat))
#else /* TPP_EMITTER_HAVE_FEATURES */
#define tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_x) 0
#define tpp_emitter_resetfeatures(self)                  (void)0
#endif /* !TPP_EMITTER_HAVE_FEATURES */


/* Emit the token currently loaded into `tpp_emitter_getlexer(self)`,
 * and update the emitter's `tem_state` accordingly
 *
 * @return: * :  Sum of return values of `tpp_emitter_getoutput(self)`
 * @return: < 0: First negative return value of `tpp_emitter_getoutput(self)` */
TPP_DECL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent(tpp_emitter *tpp_restrict self);


/* API support for (re-)emission of unknown `#pragma` directives */
#if TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
#define tpp_emitter_enable_reemit_unknown_pragma(self)  tpp_lexer_addhook_unknown_pragma_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma, self)
#define tpp_emitter_disable_reemit_unknown_pragma(self) (void)tpp_lexer_delhook_unknown_pragma_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma, self)
#define tpp_emitter_get_reemit_unknown_pragma(self)     tpp_lexer_hashook_unknown_pragma_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma, self)
#define _tpp_emitter_hook_unknown_pragma_cookie         void *
#define _tpp_emitter_hook_unknown_pragma_ofcookie(x)    ((tpp_emitter *)(x))
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */
#define tpp_emitter_enable_reemit_unknown_pragma(self)  tpp_lexer_addhook_unknown_pragma(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma)
#define tpp_emitter_disable_reemit_unknown_pragma(self) (void)tpp_lexer_delhook_unknown_pragma(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma)
#define tpp_emitter_get_reemit_unknown_pragma(self)     tpp_lexer_hashook_unknown_pragma(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma)
#define _tpp_emitter_hook_unknown_pragma_cookie         tpp_lexer *
#ifdef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
#define _tpp_emitter_hook_unknown_pragma_ofcookie(x) _tpp_emitter_oflexer(x)
#else /* TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define _tpp_emitter_hook_unknown_pragma_ofcookie(x) ((tpp_emitter *)((x) + 1))
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "`TPP_HAVE_UNKNOWN_PRAGMA_HOOK` is configured without cookies, but `TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER` isn't defined"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */
#define tpp_emitter_set_reemit_unknown_pragma(self, v)    \
	((v) ? tpp_emitter_enable_reemit_unknown_pragma(self) \
	     : (tpp_emitter_disable_reemit_unknown_pragma(self), TPP_EOK))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_unknown_pragma(_tpp_emitter_hook_unknown_pragma_cookie cookie);
#else /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */
#define tpp_emitter_disable_reemit_unknown_pragma(self) (void)0
#define tpp_emitter_get_reemit_unknown_pragma(self)     false
#endif /* !TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */


/* API support for (re-)emission of `#define` and `#undef` directives */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_DEFINED_HOOK)
#define tpp_emitter_get_reemit_macro_definitions(self) tpp_lexer_hashook_macro_defined_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined, self)
#define _tpp_emitter_enable_macro_defined_hook(self)   tpp_lexer_addhook_macro_defined_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined, self)
#define _tpp_emitter_disable_macro_defined_hook(self)  tpp_lexer_delhook_macro_defined_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined, self)
#define _tpp_emitter_hook_macro_defined_cookie         void *
#define _tpp_emitter_hook_macro_defined_ofcookie(x)    ((tpp_emitter *)(x))
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_DEFINED_HOOK) */
#define tpp_emitter_get_reemit_macro_definitions(self) tpp_lexer_hashook_macro_defined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined)
#define _tpp_emitter_enable_macro_defined_hook(self)   tpp_lexer_addhook_macro_defined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined)
#define _tpp_emitter_disable_macro_defined_hook(self)  tpp_lexer_delhook_macro_defined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined)
#define _tpp_emitter_hook_macro_defined_cookie         tpp_lexer *
#ifdef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
#define _tpp_emitter_hook_macro_defined_ofcookie(x) _tpp_emitter_oflexer(x)
#else /* TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define _tpp_emitter_hook_macro_defined_ofcookie(x) ((tpp_emitter *)((x) + 1))
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "`TPP_HAVE_MACRO_DEFINED_HOOK` is configured without cookies, but `TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER` isn't defined"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_DEFINED_HOOK) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_defined(_tpp_emitter_hook_macro_defined_cookie cookie,
                                tpp_keyword *tpp_restrict name,
                                tpp_macro *tpp_restrict macro);

#if TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_UNDEFINED_HOOK)
#define _tpp_emitter_enable_macro_undefined_hook(self)  tpp_lexer_addhook_macro_undefined_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_undefined, self)
#define _tpp_emitter_disable_macro_undefined_hook(self) tpp_lexer_delhook_macro_undefined_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_undefined, self)
#define _tpp_emitter_hook_macro_undefined_cookie        void *
#define _tpp_emitter_hook_macro_undefined_ofcookie(x)   ((tpp_emitter *)(x))
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_UNDEFINED_HOOK) */
#define _tpp_emitter_enable_macro_undefined_hook(self)  tpp_lexer_addhook_macro_undefined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_undefined)
#define _tpp_emitter_disable_macro_undefined_hook(self) tpp_lexer_delhook_macro_undefined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_undefined)
#define _tpp_emitter_hook_macro_undefined_cookie        tpp_lexer *
#ifdef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
#define _tpp_emitter_hook_macro_undefined_ofcookie(x) _tpp_emitter_oflexer(x)
#else /* TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define _tpp_emitter_hook_macro_undefined_ofcookie(x) ((tpp_emitter *)((x) + 1))
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "`TPP_HAVE_MACRO_UNDEFINED_HOOK` is configured without cookies, but `TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER` isn't undefined"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_MACRO_UNDEFINED_HOOK) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_undefined(_tpp_emitter_hook_macro_undefined_cookie cookie,
                                  tpp_keyword *tpp_restrict name);

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_enable_reemit_macro_definitions(tpp_emitter *tpp_restrict self);
#define tpp_emitter_disable_reemit_macro_definitions(self) \
	(void)(_tpp_emitter_disable_macro_defined_hook(self),  \
	       _tpp_emitter_disable_macro_undefined_hook(self))
#define tpp_emitter_set_reemit_macro_definitions(self, v)    \
	((v) ? tpp_emitter_enable_reemit_macro_definitions(self) \
	     : (tpp_emitter_disable_reemit_macro_definitions(self), TPP_EOK))
#else /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */
#define tpp_emitter_disable_reemit_macro_definitions(self) (void)0
#define tpp_emitter_get_reemit_macro_definitions(self)     false
#endif /* !TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */


/* API support for (re-)emission of `#include` (and friends) directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK)
#define tpp_emitter_enable_reemit_include_directives(self)  tpp_lexer_addhook_include_encountered_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered, self)
#define tpp_emitter_disable_reemit_include_directives(self) (void)tpp_lexer_delhook_include_encountered_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered, self)
#define tpp_emitter_get_reemit_include_directives(self)     tpp_lexer_hashook_include_encountered_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered, self)
#define _tpp_emitter_hook_include_encountered_cookie        void *
#define _tpp_emitter_hook_include_encountered_ofcookie(x)   ((tpp_emitter *)(x))
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */
#define tpp_emitter_enable_reemit_include_directives(self)  tpp_lexer_addhook_include_encountered(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered)
#define tpp_emitter_disable_reemit_include_directives(self) (void)tpp_lexer_delhook_include_encountered(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered)
#define tpp_emitter_get_reemit_include_directives(self)     tpp_lexer_hashook_include_encountered(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered)
#define _tpp_emitter_hook_include_encountered_cookie        tpp_lexer *
#ifdef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
#define _tpp_emitter_hook_include_encountered_ofcookie(x) _tpp_emitter_oflexer(x)
#else /* TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define _tpp_emitter_hook_include_encountered_ofcookie(x) ((tpp_emitter *)((x) + 1))
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "`TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK` is configured without cookies, but `TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER` isn't undefined"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */
#define tpp_emitter_set_reemit_include_directives(self, v)    \
	((v) ? tpp_emitter_enable_reemit_include_directives(self) \
	     : (tpp_emitter_disable_reemit_include_directives(self), TPP_EOK))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_include_encountered(_tpp_emitter_hook_include_encountered_cookie cookie,
                                      tpp_hook_include_kind include_kind);
#else /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */
#define tpp_emitter_disable_reemit_include_directives(self) (void)0
#define tpp_emitter_get_reemit_include_directives(self)     false
#endif /* !TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */


#undef TPP_EMITTER_HAVE_HOOK_FILE_PUSHED
#if (TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || \
     TPP_EMITTER_HAVE_TRACE_INCLUDES ||                \
     TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#define TPP_EMITTER_HAVE_HOOK_FILE_PUSHED 1
#else /* ... */
#define TPP_EMITTER_HAVE_HOOK_FILE_PUSHED 0
#endif /* !... */
#if TPP_EMITTER_HAVE_HOOK_FILE_PUSHED

#if TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_PUSHED_HOOK)
#define _tpp_emitter_enable_file_pushed_hook(self)  tpp_lexer_addhook_file_pushed_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_pushed, self)
#define _tpp_emitter_disable_file_pushed_hook(self) tpp_lexer_delhook_file_pushed_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_pushed, self)
#define _tpp_emitter_hook_file_pushed_cookie        void *
#define _tpp_emitter_hook_file_pushed_ofcookie(x)   ((tpp_emitter *)(x))
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_PUSHED_HOOK) */
#define _tpp_emitter_enable_file_pushed_hook(self)  tpp_lexer_addhook_file_pushed(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_pushed)
#define _tpp_emitter_disable_file_pushed_hook(self) tpp_lexer_delhook_file_pushed(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_pushed)
#define _tpp_emitter_hook_file_pushed_cookie        tpp_lexer *
#ifdef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
#define _tpp_emitter_hook_file_pushed_ofcookie(x) _tpp_emitter_oflexer(x)
#else /* TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define _tpp_emitter_hook_file_pushed_ofcookie(x) ((tpp_emitter *)((x) + 1))
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "`TPP_HAVE_FILE_PUSHED_HOOK` is configured without cookies, but `TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER` isn't undefined"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_PUSHED_HOOK) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_file_pushed(_tpp_emitter_hook_file_pushed_cookie cookie);

#if ((!TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)) && \
     (!TPP_EMITTER_HAVE_TRACE_INCLUDES || TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)) && \
     (!TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)))
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
#define _tpp_emitter_candisable_file_pushed_hook_REEMIT_MACRO_DEFINITIONS_LAZY(self) && !tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY)
#else /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */
#define _tpp_emitter_candisable_file_pushed_hook_REEMIT_MACRO_DEFINITIONS_LAZY(self) /* nothing */
#endif /* !TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
#define _tpp_emitter_candisable_file_pushed_hook_TRACE_INCLUDES(self) && !tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES)
#else /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
#define _tpp_emitter_candisable_file_pushed_hook_TRACE_INCLUDES(self) /* nothing */
#endif /* !TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT) && TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#define _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self) && (!tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT) || !tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS))
#elif TPP_EMITTER_HAVE_USE_CPP_DIGIT
#define _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self) && !tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS)
#else /* ... */
#define _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self) /* nothing */
#endif /* !... */
#define _tpp_emitter_candisable_file_pushed_hook(self)                                 \
	(true _tpp_emitter_candisable_file_pushed_hook_REEMIT_MACRO_DEFINITIONS_LAZY(self) \
	      _tpp_emitter_candisable_file_pushed_hook_TRACE_INCLUDES(self)                \
	      _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self))
#define _tpp_emitter_maybe_disable_file_pushed_hook(self) \
	(_tpp_emitter_candisable_file_pushed_hook(self)       \
	 ? _tpp_emitter_disable_file_pushed_hook(self)        \
	 : (void)0)
#else /* ... */
#define _tpp_emitter_maybe_disable_file_pushed_hook(self) (void)0
#endif /* !... */
#else /* TPP_EMITTER_HAVE_HOOK_FILE_PUSHED */
#define _tpp_emitter_disable_file_pushed_hook(self)       (void)0
#define _tpp_emitter_maybe_disable_file_pushed_hook(self) (void)0
#define _tpp_emitter_candisable_file_pushed_hook(self)    true
#endif /* !TPP_EMITTER_HAVE_HOOK_FILE_PUSHED */


/* Extension to `TPP_EMITTER_HAVE_USE_CPP_DIGIT`: also use 1/2/3/4 flags */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_POPPED_HOOK)
#define _tpp_emitter_enable_file_popped_hook(self)  tpp_lexer_addhook_file_popped_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_popped, self)
#define _tpp_emitter_disable_file_popped_hook(self) tpp_lexer_delhook_file_popped_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_popped, self)
#define _tpp_emitter_hook_file_popped_cookie        void *
#define _tpp_emitter_hook_file_popped_ofcookie(x)   ((tpp_emitter *)(x))
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_POPPED_HOOK) */
#define _tpp_emitter_enable_file_popped_hook(self)  tpp_lexer_addhook_file_popped(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_popped)
#define _tpp_emitter_disable_file_popped_hook(self) tpp_lexer_delhook_file_popped(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_popped)
#define _tpp_emitter_hook_file_popped_cookie        tpp_lexer *
#ifdef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
#define _tpp_emitter_hook_file_popped_ofcookie(x) _tpp_emitter_oflexer(x)
#else /* TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define _tpp_emitter_hook_file_popped_ofcookie(x) ((tpp_emitter *)((x) + 1))
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "`TPP_HAVE_FILE_POPPED_HOOK` is configured without cookies, but `TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER` isn't undefined"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_FILE_POPPED_HOOK) */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_emitter_hook_file_popped(_tpp_emitter_hook_file_popped_cookie cookie);

#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_enable_use_cpp_digit_flags(tpp_emitter *tpp_restrict self);
#define tpp_emitter_disable_use_cpp_digit_flags(self)                              \
	(void)(tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS), \
	       _tpp_emitter_maybe_disable_file_pushed_hook(self),                      \
	       _tpp_emitter_disable_file_popped_hook(self))
#define tpp_emitter_get_use_cpp_digit_flags(self) \
	tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS)
#define tpp_emitter_set_use_cpp_digit_flags(self, v)    \
	((v) ? tpp_emitter_enable_use_cpp_digit_flags(self) \
	     : (tpp_emitter_disable_use_cpp_digit_flags(self), TPP_EOK))
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#else /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#define _tpp_emitter_disable_file_popped_hook(self) (void)0
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */


/* API support for *lazy* (re-)emission of `#define` and `#undef` directives */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_enable_reemit_macro_definitions_lazy(tpp_emitter *tpp_restrict self);
#define tpp_emitter_disable_reemit_macro_definitions_lazy(self)                              \
	(void)(tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY), \
	       _tpp_emitter_maybe_disable_file_pushed_hook(self))
#define tpp_emitter_get_reemit_macro_definitions_lazy(self) \
	tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY)
#define tpp_emitter_set_reemit_macro_definitions_lazy(self, v)    \
	((v) ? tpp_emitter_enable_reemit_macro_definitions_lazy(self) \
	     : (tpp_emitter_disable_reemit_macro_definitions_lazy(self), TPP_EOK))
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */


/* API support for tracing of #incude-depth and files */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_enable_trace_includes(tpp_emitter *tpp_restrict self);
#define tpp_emitter_disable_trace_includes(self)                              \
	(void)(tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES), \
	       _tpp_emitter_maybe_disable_file_pushed_hook(self))
#define tpp_emitter_get_trace_includes(self) \
	tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES)
#define tpp_emitter_set_trace_includes(self, v)    \
	((v) ? tpp_emitter_enable_trace_includes(self) \
	     : (tpp_emitter_disable_trace_includes(self), TPP_EOK))
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_H */
