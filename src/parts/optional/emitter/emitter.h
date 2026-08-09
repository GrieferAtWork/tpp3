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
	tpp_lcinfo          TPP_EMITTER_INTERNAL(tesf_curpos);    /* Current line/column position in output (with respect to emitted `#line` directives) */
	char const         *TPP_EMITTER_INTERNAL(tesf_fname);     /* [0..1] The filename (tpp_file_getfilename()) that goes with `tes_curpos` (or "NULL" if unknown, or this is the first token) */
	TPP_REF tpp_string *TPP_EMITTER_INTERNAL(tesf_fname_str); /* [0..1] Same as `tes_curfilename`, but keeps a reference to `tpp_file_getfilenamestr()` so custom filename overrides aren't free'd early */
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
	tpp_file_flags          TPP_EMITTER_INTERNAL(tesf_flags); /* Set of `_TPP_EMITTER_STATE_FLAGS_MASK` */
#endif /* _TPP_EMITTER_STATE_FLAGS_MASK */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
} tpp_emitter_state_file;

typedef struct tpp_emitter_state_files {
	tpp_emitter_state_file  TPP_EMITTER_INTERNAL(tesfs_file);    /* Most-recent file */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	tpp_size                TPP_EMITTER_INTERNAL(tesfs_filec);   /* # of dummy files pushed by `# <digit> "filename" 1` */
	tpp_emitter_state_file *TPP_EMITTER_INTERNAL(tesfs_filev);   /* [0..tesfs_filec][owned] Extra files pus */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
} tpp_emitter_state_files;
#endif /* TPP_EMITTER_HAVE_CURPOS */

#undef TPP_EMITTER_HAVE_FLAGS
#if (TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || \
     TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
#define TPP_EMITTER_HAVE_FLAGS 1
#else /* ... */
#define TPP_EMITTER_HAVE_FLAGS 0
#endif /* !... */

#if TPP_EMITTER_HAVE_FLAGS
#define tpp_emitter_flags uint_least8_t
#define TPP_EMITTER_FLAG_NORMAL   UINT8_C(0x00)
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#define TPP_EMITTER_FLAG_FCHANGED UINT8_C(0x01) /* Contents of the #include-stack may have changed */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
#define TPP_EMITTER_FLAG_HASLINE  UINT8_C(0x02) /* At least 1 `# <linenum>`-directive was emitted */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */
#endif /* TPP_EMITTER_HAVE_FLAGS */


typedef struct tpp_emitter_state {
#if TPP_EMITTER_HAVE_CURPOS
	tpp_emitter_state_files TPP_EMITTER_INTERNAL(tes_curfile); /* Current file-state. */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	tpp_size                TPP_EMITTER_INTERNAL(tes_cached_filec); /* Size of alternate file-state buffer (`tes_cached_filev`). */
	tpp_emitter_state_file *TPP_EMITTER_INTERNAL(tes_cached_filev); /* [0..tes_cached_filec] Alternate file-state buffer (used internally) */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#endif /* TPP_EMITTER_HAVE_CURPOS */
#if TPP_EMITTER_HAVE_FLAGS
	tpp_emitter_flags       TPP_EMITTER_INTERNAL(tes_flags);   /* Emitter flags */
#endif /* TPP_EMITTER_HAVE_FLAGS */
	tpp_token_id            TPP_EMITTER_INTERNAL(tes_prevtok); /* Last token ID (preceding the token currently being emitted).
	                                                            * When the current token is the first, this is `TPP_TOK_EOF` */
} tpp_emitter_state;


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
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER)
	tpp_lexer           *TPP_EMITTER_INTERNAL(te_lexer);  /* [1..1][const] The lexer whose tokens are being emitted */
#endif /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
	tpp_formatprinter    TPP_EMITTER_INTERNAL(te_output); /* [1..1][const] Emitter output printer (the emitter itself will be passed as argument) */
	tpp_emitter_state    TPP_EMITTER_INTERNAL(te_state);  /* Emitter output state */
#if TPP_EMITTER_HAVE_FEATURES
	tpp_emitter_features TPP_EMITTER_INTERNAL(te_feat);   /* Emitter feature configuration */
#endif /* TPP_EMITTER_HAVE_FEATURES */
#if TPP_EMITTER_MODE_HAVE_MULTIPLE
	tpp_emitter_mode     TPP_EMITTER_INTERNAL(te_mode);   /* Mode in which tokens are emitted. */
#define tpp_emitter_getmode(self)    ((self)->TPP_EMITTER_INTERNAL(te_mode))
#define tpp_emitter_setmode(self, v) (void)((self)->TPP_EMITTER_INTERNAL(te_mode) = (v))
#else /* TPP_EMITTER_MODE_HAVE_MULTIPLE */
#define tpp_emitter_getmode(self)    _TPP_EMITTER_MODE_DEFAULT
#define tpp_emitter_setmode(self, v) (void)(v)
#endif /* !TPP_EMITTER_MODE_HAVE_MULTIPLE */
#if TPP_EMITTER_CONFIG_LINE_THRESHOLD < 0
	tpp_line             TPP_EMITTER_INTERNAL(te_linethreshold); /* max # of blank lines emitted for alignment purposes */
#define tpp_emitter_getlinethreshold(self)     ((self)->TPP_EMITTER_INTERNAL(te_linethreshold))
#define tpp_emitter_setlinethreshold(self, v)  (void)((self)->TPP_EMITTER_INTERNAL(te_linethreshold) = (tpp_line)(v))
#define tpp_emitter_disablelinethreshold(self) (void)((self)->TPP_EMITTER_INTERNAL(te_linethreshold) = -1)
#elif !TPP_EMITTER_CONFIG_LINE_THRESHOLD
#define tpp_emitter_getlinethreshold(self) (-1)
#else /* ... */
#define tpp_emitter_getlinethreshold(self) TPP_EMITTER_CONFIG_LINE_THRESHOLD
#endif /* !... */
} tpp_emitter;

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given emitter.
 *
 * @param: output: Output printer. On error, must return one of `TPP_SSIZE_OFERR(*)`
 * @param: lexer:  The lexer whose tokens are being emitted */
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER)
TPP_DECL TPP_NONNULL((1, 2, 3)) void TPPCALL
tpp_emitter_init(tpp_emitter *tpp_restrict self,
                 tpp_lexer *tpp_restrict lexer,
                 tpp_formatprinter output);
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
TPP_DECL TPP_NONNULL((1, 2)) void TPPCALL
_tpp_emitter_init(tpp_emitter *tpp_restrict self,
                  tpp_formatprinter output);
#define tpp_emitter_init(self, lexer, output)           \
	(tpp_assert(tpp_emitter_getlexer(self) == (lexer)), \
	 _tpp_emitter_init(self, output))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_emitter_fini(tpp_emitter *tpp_restrict self);

/* Retrieve components of the emitter. */
#define tpp_emitter_getoutput(self) (self)->TPP_EMITTER_INTERNAL(te_output)
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER)
#define tpp_emitter_getlexer(self)   ((self)->TPP_EMITTER_INTERNAL(te_lexer))
#define tpp_emitter_ofcookie(cookie) ((tpp_emitter *)(cookie))
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#ifndef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
#error "Invalid configuration: under '-DTPP_HAVE_HOOK_COOKIES=0' you must specify a macro '#define TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER (offsetof(MY_CONTAINER, emitter) - offsetof(MY_CONTAINER, lexer))' to specify how to retrieve the emitter from a lexer"
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
#define tpp_emitter_getlexer(self)   ((tpp_lexer *)((char *)(self) - TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER))
#define tpp_emitter_ofcookie(cookie) ((tpp_emitter *)((char *)(self) + TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */

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
#define tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_x)          tpp_emitter_features_getid(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_x, enabled) tpp_emitter_features_setid(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x, enabled)
#define tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_x)       tpp_emitter_features_enable(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_x)      tpp_emitter_features_disable(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_resetfeatures(self)                           tpp_emitter_features_reset(&(self)->TPP_EMITTER_INTERNAL(te_feat))
#else /* TPP_EMITTER_HAVE_FEATURES */
#define tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_x) 0
#define tpp_emitter_resetfeatures(self)                  (void)0
#endif /* !TPP_EMITTER_HAVE_FEATURES */


/* Emit the token currently loaded into `tpp_emitter_getlexer(self)`,
 * and update the emitter's `te_state` accordingly
 *
 * @return: * :  Sum of return values of `tpp_emitter_getoutput(self)`
 * @return: < 0: First negative return value of `tpp_emitter_getoutput(self)` */
TPP_DECL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent(tpp_emitter *tpp_restrict self);


/* API support for (re-)emission of unknown `#pragma` directives */
#if TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
#if TPP_HAVE_HOOK_COOKIES
#define tpp_emitter_enable_reemit_unknown_pragma(self) \
	tpp_lexer_sethook_unknown_pragma_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma, self)
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_enable_reemit_unknown_pragma(self) \
	tpp_lexer_sethook_unknown_pragma(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma)
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_disable_reemit_unknown_pragma(self) \
	tpp_lexer_resethook_unknown_pragma(tpp_emitter_getlexer(self))
#define tpp_emitter_get_reemit_unknown_pragma(self) \
	(tpp_lexer_gethook_unknown_pragma(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_unknown_pragma)
#define tpp_emitter_set_reemit_unknown_pragma(self, v)    \
	((v) ? tpp_emitter_enable_reemit_unknown_pragma(self) \
	     : tpp_emitter_disable_reemit_unknown_pragma(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_unknown_pragma(tpp_hook_cookie cookie);
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */

/* API support for (re-)emission of `#define` and `#undef` directives */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
#if TPP_HAVE_HOOK_COOKIES
#define tpp_emitter_enable_reemit_macro_definitions(self)                                                    \
	(tpp_lexer_sethook_macro_defined_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined, self), \
	 tpp_lexer_sethook_macro_undefined_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_undefined, self))
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_enable_reemit_macro_definitions(self)                                           \
	(tpp_lexer_sethook_macro_defined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined), \
	 tpp_lexer_sethook_macro_undefined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_undefined))
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_disable_reemit_macro_definitions(self)          \
	(tpp_lexer_resethook_macro_defined(tpp_emitter_getlexer(self)), \
	 tpp_lexer_resethook_macro_undefined(tpp_emitter_getlexer(self)))
#define tpp_emitter_get_reemit_macro_definitions(self) \
	(tpp_lexer_gethook_macro_defined(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_macro_defined)
#define tpp_emitter_set_reemit_macro_definitions(self, v)    \
	((v) ? tpp_emitter_enable_reemit_macro_definitions(self) \
	     : tpp_emitter_disable_reemit_macro_definitions(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_defined(tpp_hook_cookie cookie,
                                tpp_keyword *tpp_restrict name,
                                tpp_macro *tpp_restrict macro);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_undefined(tpp_hook_cookie cookie,
                                  tpp_keyword *tpp_restrict name);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

/* API support for (re-)emission of `#include` (and friends) directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
#if TPP_HAVE_HOOK_COOKIES
#define tpp_emitter_enable_reemit_include_directives(self) \
	tpp_lexer_sethook_include_encountered_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered, self)
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_enable_reemit_include_directives(self) \
	tpp_lexer_sethook_include_encountered(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered)
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_disable_reemit_include_directives(self) \
	tpp_lexer_resethook_include_encountered(tpp_emitter_getlexer(self))
#define tpp_emitter_get_reemit_include_directives(self) \
	(tpp_lexer_gethook_include_encountered(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_include_encountered)
#define tpp_emitter_set_reemit_include_directives(self, v)    \
	((v) ? tpp_emitter_enable_reemit_include_directives(self) \
	     : tpp_emitter_disable_reemit_include_directives(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_include_encountered(tpp_hook_cookie cookie,
                                      tpp_hook_include_kind include_kind);
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */

#undef _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED
#if (TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || \
     TPP_EMITTER_HAVE_TRACE_INCLUDES ||                \
     TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#define _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED 1
#else /* ... */
#define _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED 0
#endif /* !... */
#if _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_file_pushed(tpp_hook_cookie cookie);
#if TPP_HAVE_HOOK_COOKIES
#define _tpp_emitter_enable_file_pushed_hook(self) \
	tpp_lexer_sethook_file_pushed_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_pushed, self)
#else /* TPP_HAVE_HOOK_COOKIES */
#define _tpp_emitter_enable_file_pushed_hook(self) \
	tpp_lexer_sethook_file_pushed(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_pushed)
#endif /* !TPP_HAVE_HOOK_COOKIES */
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
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#define _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self) && !tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS)
#else /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#define _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self) /* nothing */
#endif /* !TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#define _tpp_emitter_candisable_file_pushed_hook(self)                              \
	(1 _tpp_emitter_candisable_file_pushed_hook_REEMIT_MACRO_DEFINITIONS_LAZY(self) \
	   _tpp_emitter_candisable_file_pushed_hook_TRACE_INCLUDES(self)                \
	   _tpp_emitter_candisable_file_pushed_hook_USE_CPP_DIGIT_FLAGS(self))
#define _tpp_emitter_disable_file_pushed_hook(self)                \
	(_tpp_emitter_candisable_file_pushed_hook(self)                \
	 ? tpp_lexer_resethook_file_pushed(tpp_emitter_getlexer(self)) \
	 : (void)0)
#else /* ... */
#define _tpp_emitter_disable_file_pushed_hook(self) (void)0
#endif /* !... */
#endif /* _TPP_EMITTER_HAVE_HOOK_FILE_PUSHED */

/* Extension to `TPP_EMITTER_HAVE_USE_CPP_DIGIT`: also use 1/2/3/4 flags */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_emitter_hook_file_popped(tpp_hook_cookie cookie);
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
#if TPP_HAVE_HOOK_COOKIES
#define _tpp_emitter_enable_file_popped_hook(self) \
	tpp_lexer_sethook_file_popped_ex(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_popped, self)
#else /* TPP_HAVE_HOOK_COOKIES */
#define _tpp_emitter_enable_file_popped_hook(self) \
	 tpp_lexer_sethook_file_popped(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_popped))
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_emitter_enable_use_cpp_digit_flags(self)                        \
	(tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS), \
	 _tpp_emitter_enable_file_pushed_hook(self),                            \
	 _tpp_emitter_enable_file_popped_hook(self))
#define tpp_emitter_disable_use_cpp_digit_flags(self)                        \
	(tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS), \
	 _tpp_emitter_disable_file_pushed_hook(self),                            \
	 tpp_lexer_resethook_file_popped(tpp_emitter_getlexer(self)))
#define tpp_emitter_get_use_cpp_digit_flags(self) \
	tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS)
#define tpp_emitter_set_use_cpp_digit_flags(self, v)    \
	((v) ? tpp_emitter_enable_use_cpp_digit_flags(self) \
	     : tpp_emitter_disable_use_cpp_digit_flags(self))
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */

/* API support for *lazy* (re-)emission of `#define` and `#undef` directives */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
#define tpp_emitter_enable_reemit_macro_definitions_lazy(self)                        \
	(tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY), \
	 _tpp_emitter_enable_file_pushed_hook(self))
#define tpp_emitter_disable_reemit_macro_definitions_lazy(self)                        \
	(tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY), \
	 _tpp_emitter_disable_file_pushed_hook(self))
#define tpp_emitter_get_reemit_macro_definitions_lazy(self) \
	tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY)
#define tpp_emitter_set_reemit_macro_definitions_lazy(self, v)    \
	((v) ? tpp_emitter_enable_reemit_macro_definitions_lazy(self) \
	     : tpp_emitter_disable_reemit_macro_definitions_lazy(self))
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */

/* API support for tracing of #incude-depth and files */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
#define tpp_emitter_enable_trace_includes(self)                        \
	(tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES), \
	 _tpp_emitter_enable_file_pushed_hook(self))
#define tpp_emitter_disable_trace_includes(self)                        \
	(tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES), \
	 _tpp_emitter_disable_file_pushed_hook(self))
#define tpp_emitter_get_trace_includes(self) \
	tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES)
#define tpp_emitter_set_trace_includes(self, v)    \
	((v) ? tpp_emitter_enable_trace_includes(self) \
	     : tpp_emitter_disable_trace_includes(self))
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_H */
