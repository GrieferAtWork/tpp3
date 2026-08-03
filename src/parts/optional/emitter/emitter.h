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

typedef struct tpp_emitter_state {
#if TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
	tpp_lcinfo          TPP_EMITTER_INTERNAL(tes_curpos);          /* Current line/column position in output (with respect to emitted `#line` directives) */
	char const         *TPP_EMITTER_INTERNAL(tes_curfilename);     /* [0..1] The filename (tpp_file_getfilename()) that goes with `tes_curpos` (or "NULL" if unknown, or this is the first token) */
	TPP_REF tpp_string *TPP_EMITTER_INTERNAL(tes_curfilename_str); /* [0..1] Same as `tes_curfilename`, but keeps a reference to `tpp_file_getfilenamestr()` so custom filename overrides aren't free'd early */
#define _tpp_emitter_state_init_cur(self)                             \
	, tpp_lcinfo_init((self)->TPP_EMITTER_INTERNAL(tes_curpos), 0, 0) \
	, (self)->TPP_EMITTER_INTERNAL(tes_curfilename)     = NULL        \
	, (self)->TPP_EMITTER_INTERNAL(tes_curfilename_str) = NULL
#define _tpp_emitter_state_fini_cur(self)                                          \
	, (self)->TPP_EMITTER_INTERNAL(tes_curfilename_str)                            \
	  ? (void)tpp_string_decref((self)->TPP_EMITTER_INTERNAL(tes_curfilename_str)) \
	  : (void)0
#else /* TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
#define _tpp_emitter_state_init_cur(self) /* nothing */
#define _tpp_emitter_state_fini_cur(self) /* nothing */
#endif /* TPP_EMITTER_HAVE_MODE_EMIT && !TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
	tpp_token_id        TPP_EMITTER_INTERNAL(tes_prevtok);         /* Last token ID (preceding the token currently being emitted).
	                                                                * When the current token is the first, this is `TPP_TOK_EOF` */
} tpp_emitter_state;

#define tpp_emitter_state_init(self)                               \
	(void)((self)->TPP_EMITTER_INTERNAL(tes_prevtok) = TPP_TOK_EOF \
	       _tpp_emitter_state_init_cur(self))
#define tpp_emitter_state_fini(self) \
	(void)((void)0 _tpp_emitter_state_fini_cur(self))

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

} tpp_emitter_mode;

#ifndef TPP_EMITTER_MODE_HAVE_MULTIPLE
#define TPP_EMITTER_MODE_HAVE_MULTIPLE 0
#endif /* !TPP_EMITTER_MODE_HAVE_MULTIPLE */


typedef struct tpp_emitter {
	/* NOTE: The lexer will *always* be at offset=0 here, meaning you're allowed to do:
	 * >> typedef struct my_compiler {
	 * >>     ...;
	 * >>     union {
	 * >>         tpp_lexer   l_lexer;     // Common lexer object
	 * >>         tpp_emitter l_emitter;   // For implementing a -E switch
	 * >>         struct {
	 * >>             tpp_lexer lmy_lexer; // Lexer for my compiler
	 * >>             ...                  // Stuff not needed when -E is active
	 * >>         } l_my_lexer;
	 * >>     } mc_lexer;
	 * >> } my_compiler; */
	tpp_lexer            TPP_EMITTER_INTERNAL(te_lexer);  /* Lexer supplying the emitter's input token stream */
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
} tpp_emitter;

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given emitter.
 *
 * @param: output: Output printer. On error, must return one of `TPP_SSIZE_OFERR(*)`*/
TPP_DECL TPP_NONNULL((1, 2)) void TPPCALL
tpp_emitter_init_after_lexer(tpp_emitter *tpp_restrict self,
                             tpp_formatprinter output);
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_emitter_fini_before_lexer(tpp_emitter *tpp_restrict self);

/* Initialize/finalize a given emitter, as well as the underlying lexer.
 *
 * The caller is still responsible to initialize/finalize the lexer's file:
 * - `tpp_lexer_initfile_*(tpp_emitter_getlexer(self))`
 * - `tpp_lexer_finifile(tpp_emitter_getlexer(self))`
 *
 * @param: output: Output printer. On error, must return one of `TPP_SSIZE_OFERR(*)` */
#define tpp_emitter_init(self, output)                        \
	(tpp_lexer_init(&(self)->TPP_EMITTER_INTERNAL(te_lexer)), \
	 tpp_emitter_init_after_lexer(self, output))
#define tpp_emitter_fini(self)            \
	(tpp_emitter_fini_before_lexer(self), \
	 tpp_lexer_fini(&(self)->TPP_EMITTER_INTERNAL(te_lexer)))

/* Retrieve components of the emitter. */
#define tpp_emitter_getlexer(self)  (&(self)->TPP_EMITTER_INTERNAL(te_lexer))
#define tpp_emitter_getoutput(self) (self)->TPP_EMITTER_INTERNAL(te_output)

/* Reverse-engineer the emitter of the lexer returned by `tpp_emitter_getlexer()` */
#define tpp_emitter_oflexer(lexer) ((tpp_emitter *)(lexer))

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
#define tpp_emitter_enable_reemit_unknown_pragma(self) \
	tpp_lexer_sethook_unknown_pragma(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma)
#define tpp_emitter_disable_reemit_unknown_pragma(self) \
	tpp_lexer_resethook_unknown_pragma(tpp_emitter_getlexer(self))
#define tpp_emitter_get_reemit_unknown_pragma(self) \
	(tpp_lexer_gethook_unknown_pragma(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_unknown_pragma)
#define tpp_emitter_set_reemit_unknown_pragma(self, v)    \
	((v) ? tpp_emitter_enable_reemit_unknown_pragma(self) \
	     : tpp_emitter_disable_reemit_unknown_pragma(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_unknown_pragma(tpp_lexer *tpp_restrict self);
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */

/* API support for (re-)emission of `#define` and `#undef` directives */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
#define tpp_emitter_enable_reemit_macro_definitions(self)                                           \
	(tpp_lexer_sethook_macro_defined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_defined), \
	 tpp_lexer_sethook_macro_undefined(tpp_emitter_getlexer(self), &_tpp_emitter_hook_macro_undefined))
#define tpp_emitter_disable_reemit_macro_definitions(self)          \
	(tpp_lexer_resethook_macro_defined(tpp_emitter_getlexer(self)), \
	 tpp_lexer_resethook_macro_undefined(tpp_emitter_getlexer(self)))
#define tpp_emitter_get_reemit_macro_definitions(self) \
	(tpp_lexer_gethook_macro_defined(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_macro_defined)
#define tpp_emitter_set_reemit_macro_definitions(self, v)    \
	((v) ? tpp_emitter_enable_reemit_macro_definitions(self) \
	     : tpp_emitter_disable_reemit_macro_definitions(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_defined(tpp_lexer *tpp_restrict self,
                                tpp_keyword *tpp_restrict name,
                                tpp_macro *tpp_restrict macro);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_undefined(tpp_lexer *tpp_restrict self,
                                  tpp_keyword *tpp_restrict name);
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

/* API support for (re-)emission of `#include` (and friends) directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
#define tpp_emitter_enable_reemit_include_directives(self) \
	tpp_lexer_sethook_include_encountered(tpp_emitter_getlexer(self), &_tpp_emitter_hook_include_encountered)
#define tpp_emitter_disable_reemit_include_directives(self) \
	tpp_lexer_resethook_include_encountered(tpp_emitter_getlexer(self))
#define tpp_emitter_get_reemit_include_directives(self) \
	(tpp_lexer_gethook_include_encountered(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_include_encountered)
#define tpp_emitter_set_reemit_include_directives(self, v)    \
	((v) ? tpp_emitter_enable_reemit_include_directives(self) \
	     : tpp_emitter_disable_reemit_include_directives(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_include_encountered(tpp_lexer *tpp_restrict self,
                                      tpp_hook_include_kind include_kind);
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */

#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || TPP_EMITTER_HAVE_TRACE_INCLUDES
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_file_pushed(tpp_lexer *tpp_restrict self);
#define _tpp_emitter_enable_file_pushed_hook(self) \
	tpp_lexer_sethook_file_pushed(tpp_emitter_getlexer(self), &_tpp_emitter_hook_file_pushed)
#if ((!TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)) && \
     (!TPP_EMITTER_HAVE_TRACE_INCLUDES || TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)))
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) && TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
#define _tpp_emitter_candisable_file_pushed_hook(self)                                \
	(!tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY) && \
	 !tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES))
#elif TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
#define _tpp_emitter_candisable_file_pushed_hook(self) (!tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY))
#else /* ... */
#define _tpp_emitter_candisable_file_pushed_hook(self) (!tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES))
#endif /* !... */
#define _tpp_emitter_disable_file_pushed_hook(self)                \
	(_tpp_emitter_candisable_file_pushed_hook(self)                \
	 ? tpp_lexer_resethook_file_pushed(tpp_emitter_getlexer(self)) \
	 : (void)0)
#else /* ... */
#define _tpp_emitter_disable_file_pushed_hook(self) (void)0
#endif /* !... */
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY || TPP_EMITTER_HAVE_TRACE_INCLUDES */

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
