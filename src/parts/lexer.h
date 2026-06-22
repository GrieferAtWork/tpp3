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
#include "expr.h"
#include "extensions.h"
#include "features.h"
#include "file.h"
#include "keyword.h"
#include "sysinclude.h"
#include "time.h"
#include "token.h"
#include "warnings.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#undef TPP_HAVE_LEXER_STATE_FLAGS
#if (TPP_HAVE_CPP_DIRECTIVES || \
     TPP_HAVE_WARNINGS ||       \
     TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS)
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
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
#define TPP_LEXER_STATE_FLAG_ALLTOKENS    UINT8_C(0x04) /* Prevent `tpp_lexer_yieldpp()' from (possibly) skipp SPACE/LF/COMMENT tokens */
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#endif /* TPP_HAVE_LEXER_STATE_FLAGS */


typedef struct tpp_lexer {
	union {
		tpp_token      TPP_INTERNAL(tlc_tok);  /* [valid_if(WAS_CALLED(tpp_lexer_yieldraw()))] Last-read token (never
		                                        * set to one of `TPP_TOK_E*'; iow: always positive or TPP_TOK_EOF). */
		struct {
			char _tli_pad[tpp_offsetof(tpp_token, TPP_INTERNAL(tt_start))];
			tpp_file   TPP_INTERNAL(tli_file); /* [OVERRIDE(.tf_prev, [owned])]
			                                    * The file that lies at the top of the lexer's #include/macro-stack.
			                                    * this is also the file whose buffer currently contains `tl_tok' */
		} TPP_INTERNAL(tlc_input);
	} TPP_INTERNAL(tl_core);


	/* Custom keywords table. */
	tpp_keywords TPP_INTERNAL(tl_kwds);


	/* Lexer extensions. */
#if TPP_HAVE_EXTENSIONS
	tpp_extensions TPP_INTERNAL(tl_exts);
#define _tpp_lexer_init_exts(self) , tpp_extensions_init(&(self)->TPP_INTERNAL(tl_exts))
#else /* TPP_HAVE_EXTENSIONS */
#define _tpp_lexer_init_exts(self) /* noting */
#endif /* !TPP_HAVE_EXTENSIONS */


	/* Enabled tokens */
#if TPP_HAVE_FEATURES
	tpp_features TPP_INTERNAL(tl_feat);
#define _tpp_lexer_init_feat(self) , tpp_features_init(&(self)->TPP_INTERNAL(tl_feat))
#else /* TPP_HAVE_FEATURES */
#define _tpp_lexer_init_feat(self) /* nothing */
#endif /* !TPP_HAVE_FEATURES */


	/* Lexer state flags */
#if TPP_HAVE_LEXER_STATE_FLAGS
	tpp_lexer_state_flags TPP_INTERNAL(tl_state);
#define _tpp_lexer_init_state(self) , (self)->TPP_INTERNAL(tl_state) = TPP_LEXER_STATE_FLAG_NORMAL
#else /* TPP_HAVE_LEXER_STATE_FLAGS */
#define _tpp_lexer_init_state(self) /* nothing */
#endif /* !TPP_HAVE_LEXER_STATE_FLAGS */


	/* system #include paths (/usr/include, ...) */
#if TPP_HAVE_INCLUDE_PATH
	tpp_include_paths TPP_INTERNAL(tl_include_paths);
#define _tpp_lexer_init_incpath(self) , tpp_include_paths_init(&(self)->TPP_INTERNAL(tl_include_paths))
#else /* TPP_HAVE_INCLUDE_PATH */
#define _tpp_lexer_init_incpath(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_PATH */


	/* TODO: User-defined callback hook to parse pragmas not known to TPP itself
	 * -> Should come in 3 flavors:
	 *    - Compile-time disabled
	 *    - Compile-time enabled (with hardcoded function call to user-defined macro)
	 *    - Runtime enabled (with nullable function pointer in lexer)
	 *
	 * TODO: All these different runtime hooks (tl_warnprinter, tl_expr_parser_cb, and
	 *       now this new "unknown_pragma_handler") shouldn't each do their own thing!
	 *       Instead, these should be a generic runtime-hooks component to "tpp_lexer",
	 *       similar to TPP2's "struct TPPCallbacks" */

	/* Warning configuration / printer */
#undef TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER /* Wrapper for "TPP_CONFIG_WARNPRINTER" */
#undef TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER /* Calls "fwrite(stderr)" */
#undef TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER    /* Does nothing */
#if TPP_HAVE_WARNINGS
	tpp_warnings TPP_INTERNAL(tl_warn); /* Compiler warnings state */
#define _tpp_lexer_init_warn(self) , tpp_warnings_init(&(self)->TPP_INTERNAL(tl_warn))
#ifdef TPP_CONFIG_WARNPRINTER
#if TPP_CONFIG_WARNPRINTER_NEEDS_ARG
	void             *TPP_INTERNAL(tl_warnprinterarg); /* [?..?] Argument for "TPP_CONFIG_WARNPRINTER" */
#define tpp_lexer_getwarnprinter(self)       (&TPP_CONFIG_WARNPRINTER)
#define tpp_lexer_getwarnprinterarg(self)    (self)->TPP_INTERNAL(tl_warnprinterarg)
#define tpp_lexer_setwarnprinterarg(self, v) (void)((self)->TPP_INTERNAL(tl_warnprinterarg) = (v))
#else /* TPP_CONFIG_WARNPRINTER_NEEDS_ARG */
#define TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self)    &_tpp_lexer_wrapped_warnprinter
#define tpp_lexer_getwarnprinterarg(self) NULL
#endif /* !TPP_CONFIG_WARNPRINTER_NEEDS_ARG */
#elif TPP_HAVE_BUILTIN_WARNPRINTER > 0
#define TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self)    (&_tpp_lexer_builtin_warnprinter)
#define tpp_lexer_getwarnprinterarg(self) NULL
#else /* TPP_CONFIG_WARNPRINTER */
	tpp_formatprinter TPP_INTERNAL(tl_warnprinter);    /* [0..1] Warning printer (or "NULL" to use "fwrite(stderr)") */
	void             *TPP_INTERNAL(tl_warnprinterarg); /* [valid_if(TPP_INTERNAL(tl_warnprinter) != NULL)] */
#define _tpp_lexer_init_warnprinter(self) , (self)->TPP_INTERNAL(tl_warnprinter) = NULL
#define tpp_lexer_setwarnprinter(self, printer, arg)            \
	(void)((self)->TPP_INTERNAL(tl_warnprinter)    = (printer), \
	       (self)->TPP_INTERNAL(tl_warnprinterarg) = (arg))
#define tpp_lexer_setwarnprinterarg(self, v) \
	(void)((self)->TPP_INTERNAL(tl_warnprinterarg) = (v))
#if TPP_HAVE_BUILTIN_WARNPRINTER
#define TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self) ((self)->TPP_INTERNAL(tl_warnprinter) ? (self)->TPP_INTERNAL(tl_warnprinter) : &_tpp_lexer_builtin_warnprinter)
#else /* TPP_HAVE_BUILTIN_WARNPRINTER */
#define TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER 1
#define tpp_lexer_getwarnprinter(self) ((self)->TPP_INTERNAL(tl_warnprinter) ? (self)->TPP_INTERNAL(tl_warnprinter) : &_tpp_lexer_noop_warnprinter)
#endif /* !TPP_HAVE_BUILTIN_WARNPRINTER */
#define tpp_lexer_getwarnprinterarg(self) (self)->TPP_INTERNAL(tl_warnprinterarg)
#endif /* !TPP_CONFIG_WARNPRINTER */
#else /* TPP_HAVE_WARNINGS */
#define _tpp_lexer_init_warn(self) /* nothing */
#endif /* !TPP_HAVE_WARNINGS */
#ifndef _tpp_lexer_init_warnprinter
#define _tpp_lexer_init_warnprinter(self) /* nothing */
#endif /* !_tpp_lexer_init_warnprinter */


	/* Expression parser configuration */
#undef TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR
#undef TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR_WITH_ARG
#if TPP_HAVE_LEXER_PARSEEXPR
	/* User-defined callback for parsing "#if"-style expressions
	 * - This callback is invoked in a context where "self" points
	 *   before the expression's first token (meaning that this
	 *   callback is responsible to do the initial yield using
	 *   whatever method it wants to use).
	 * - When it is known that the expression has finite length,
	 *   as in: it has to end before EOF, or at the next unmatched
	 *   ')'-token, the caller will have configured the lexer's
	 *   current EOF accordingly (and disabled file-popping)
	 * - When this function returns an error, the caller will rewind
	 *   back to the start of the expression (or even further, if
	 *   applicable; meaning this callback doesn't need to concern
	 *   itself with rollback)
	 *
	 * @return: TPP_EOK:         Success (*result was initialized)
	 * @return: TPP_ENOMEM:      Out of memory
	 * @return: TPP_EIO:         Filesystem I/O operation failed
	 * @return: TPP_EWOULDBLOCK: Operation would block
	 * @return: TPP_ELEXERROR:   A lexer error happened
	 * @return: TPP_EWARNPRINT:  Error while printing a warning */
#ifdef TPP_CONFIG_EXPRPARSER
#if TPP_CONFIG_EXPRPARSER_NEEDS_ARG
	void *TPP_INTERNAL(tl_expr_parser_arg); /* [?..?][valid_if(tl_expr_parser_cb)] Cookie for "tl_expr_parser_cb" */
#define tpp_lexer_getparseexprarg(self)    (self)->TPP_INTERNAL(tl_expr_parser_arg)
#define tpp_lexer_setparseexprarg(self, v) (void)((self)->TPP_INTERNAL(tl_expr_parser_arg) = (v))
#define tpp_lexer_parseexpr(self, result) TPP_CONFIG_EXPRPARSER(tpp_lexer_getparseexprarg(self), self, result)
#else /* TPP_CONFIG_EXPRPARSER_NEEDS_ARG */
#define tpp_lexer_parseexpr(self, result) TPP_CONFIG_EXPRPARSER(self, result)
#endif /* !TPP_CONFIG_EXPRPARSER_NEEDS_ARG */
#elif TPP_HAVE_BUILTIN_EXPRPARSER > 0
#define TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR 1
#define tpp_lexer_parseexpr(self, result) _tpp_lexer_builtin_parseexpr(self, result)
#else /* TPP_CONFIG_EXPRPARSER */
	/* User-defined override for parsing preprocessor expressions */
	tpp_errno (TPPCALL *TPP_INTERNAL(tl_expr_parser_cb))(void *arg, struct tpp_lexer *tpp_restrict self,
	                                                     tpp_expr_value *tpp_restrict result);
	void *TPP_INTERNAL(tl_expr_parser_arg); /* [?..?][valid_if(tl_expr_parser_cb)] Cookie for "tl_expr_parser_cb" */
#define _tpp_lexer_init_parseexpr(self) , (self)->TPP_INTERNAL(tl_expr_parser_cb) = NULL
#define tpp_lexer_setparseexpr(self, cb, arg)               \
	(void)((self)->TPP_INTERNAL(tl_expr_parser_cb)  = (cb), \
	       (self)->TPP_INTERNAL(tl_expr_parser_arg) = (arg))
#define tpp_lexer_getparseexprarg(self)    (self)->TPP_INTERNAL(tl_expr_parser_arg)
#define tpp_lexer_setparseexprarg(self, v) (void)((self)->TPP_INTERNAL(tl_expr_parser_arg) = (v))
#if TPP_HAVE_BUILTIN_EXPRPARSER
#define TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR_WITH_ARG 1
#define tpp_lexer_getparseexpr(self)           \
	((self)->TPP_INTERNAL(tl_expr_parser_cb)   \
	 ? (self)->TPP_INTERNAL(tl_expr_parser_cb) \
	 : &_tpp_lexer_builtin_parseexpr_with_arg)
#define tpp_lexer_parseexpr(self, result) \
	((*tpp_lexer_getparseexpr(self))(tpp_lexer_getparseexprarg(self), self, result))
#else /* TPP_HAVE_BUILTIN_EXPRPARSER */
#define tpp_lexer_parseexpr(self, result)                                               \
	(tpp_lexer_getparseexpr(self)                                                       \
	 ? ((*tpp_lexer_getparseexpr(self))(tpp_lexer_getparseexprarg(self), self, result)) \
	 : TPP_EOK)
#endif /* !TPP_HAVE_BUILTIN_EXPRPARSER */
#endif /* !TPP_CONFIG_EXPRPARSER */
#endif /* TPP_HAVE_LEXER_PARSEEXPR */
#ifndef _tpp_lexer_init_parseexpr
#define _tpp_lexer_init_parseexpr(self) /* nothing */
#endif /* !_tpp_lexer_init_parseexpr */


	/* Lexer error limits */
#if TPP_HAVE_WARNING_ERROR
	tpp_size TPP_INTERNAL(tl_error_count); /* # of times "TPP_WSTATE_ERROR" was emitted.
	                                        * When this is non-zero by the time your compiler finishes
	                                        * compiling your source file, you should NOT proceed, but
	                                        * propagate an error. */
#define _tpp_lexer_initerrorcount(self)  , (self)->TPP_INTERNAL(tl_error_count) = 0
#define tpp_lexer_geterrorcount(self)    (self)->TPP_INTERNAL(tl_error_count)
#define tpp_lexer_seterrorcount(self, v) (void)((self)->TPP_INTERNAL(tl_error_count) = (v))
#if TPP_ERROR_LIMIT < 0
	tpp_size TPP_INTERNAL(tl_error_limit); /* Once `tl_error_count >= tl_error_limit', "TPP_WSTATE_ERROR" is treated as "TPP_WSTATE_FATAL" */
#define _tpp_lexer_initerrorlimit(self)  , (self)->TPP_INTERNAL(tl_error_limit) = (tpp_size)(-TPP_ERROR_LIMIT)
#define tpp_lexer_geterrorlimit(self)    ((self)->TPP_INTERNAL(tl_error_limit))
#define tpp_lexer_seterrorlimit(self, v) (void)((self)->TPP_INTERNAL(tl_error_limit) = (v))
#else /* TPP_ERROR_LIMIT < 0 */
#define _tpp_lexer_initerrorlimit(self)  /* nothing */
#define tpp_lexer_geterrorlimit(self)    TPP_ERROR_LIMIT
#endif /* TPP_ERROR_LIMIT >= 0 */
#else /* TPP_HAVE_WARNING_ERROR */
#define _tpp_lexer_initerrorcount(self)  /* nothing */
#define _tpp_lexer_initerrorlimit(self)  /* nothing */
#define tpp_lexer_geterrorcount(self)    0
#endif /* !TPP_HAVE_WARNING_ERROR */


	/* Next value for __COUNTER__ */
#if TPP_HAVE_MACRO___COUNTER__
	tpp_size TPP_INTERNAL(tl_builtin_counter); /* Next value for __COUNTER__ */
#define _tpp_lexer_initcounter(self) , (self)->TPP_INTERNAL(tl_builtin_counter) = 0
#else /* TPP_HAVE_MACRO___COUNTER__ */
#define _tpp_lexer_initcounter(self) /* nothing */
#endif /* !TPP_HAVE_MACRO___COUNTER__ */


	/* Next value for __COUNTER__ */
#if TPP_HAVE_LEXER_TIME
	tpp_time TPP_INTERNAL(tl_time); /* Current time, or empty if not yet loaded */
#define _tpp_lexer_inittime(self) , tpp_time_empty(&(self)->TPP_INTERNAL(tl_time))
#define tpp_lexer_gettimeptr(self) (&(self)->TPP_INTERNAL(tl_time))
#define tpp_lexer_gettime(self)    (self)->TPP_INTERNAL(tl_time)
#define tpp_lexer_settime(self, v) (void)((self)->TPP_INTERNAL(tl_time) = (v))
#else /* TPP_HAVE_LEXER_TIME */
#define _tpp_lexer_inittime(self) /* nothing */
#endif /* !TPP_HAVE_LEXER_TIME */
} tpp_lexer;

/* Check if a runtime-configurable config option "conf" in "TPP_HAVE_conf" is currently enabled.
 * When "TPP_HAVE_conf" is configured as "TPP_CONF_IS_CONST()", return that constant instead. */
#define tpp_lexer_has(self, conf) _tpp_lexer_has_##conf(self)

/* Current token */
#define tpp_lexer_gettok(self)        ((self)->TPP_INTERNAL(tl_core).TPP_INTERNAL(tlc_tok).TPP_INTERNAL(tt_id))
#define tpp_lexer_gettoken(self)      (&(self)->TPP_INTERNAL(tl_core).TPP_INTERNAL(tlc_tok))
#define tpp_lexer_gettokenkwd(self)   tpp_token_getkwd(tpp_lexer_gettoken(self))
#define tpp_lexer_gettokenstart(self) tpp_token_getstart(tpp_lexer_gettoken(self))
#define tpp_lexer_gettokenend(self)   tpp_token_getend(tpp_lexer_gettoken(self))
#define tpp_lexer_gettokenlen(self)   tpp_token_getlen(tpp_lexer_gettoken(self))

/* Current file */
#define tpp_lexer_getfile(self)     (&(self)->TPP_INTERNAL(tl_core).TPP_INTERNAL(tlc_input).TPP_INTERNAL(tli_file))
#define tpp_lexer_getfilekind(self) tpp_file_getkind(tpp_lexer_getfile(self))

/* Warnings... */
#if TPP_HAVE_WARNINGS
#if TPP_HAVE_WARNINGS_PUSH_POP
#define tpp_lexer_pushwarnings(self)   tpp_warnings_push(&(self)->TPP_INTERNAL(tl_warn))
#define tpp_lexer_popwarnings(self)    tpp_warnings_pop(&(self)->TPP_INTERNAL(tl_warn))
#define tpp_lexer_canpopwarnings(self) tpp_warnings_canpop(&(self)->TPP_INTERNAL(tl_warn))
#else /* TPP_HAVE_WARNINGS_PUSH_POP */
#define tpp_lexer_canpopwarnings(self) 0
#endif /* !TPP_HAVE_WARNINGS_PUSH_POP */
#define tpp_lexer_getwarningctx(self, ctx_id)             tpp_warnings_getctx(&(self)->TPP_INTERNAL(tl_warn), ctx_id)
#define tpp_lexer_setwarningctx(self, ctx_id, state)      tpp_warnings_setctx(&(self)->TPP_INTERNAL(tl_warn), ctx_id, state)
#define tpp_lexer_invokewarning(self, warning_id, result) tpp_warnings_invoke(&(self)->TPP_INTERNAL(tl_warn), warning_id, result)
#endif /* TPP_HAVE_WARNINGS */

/* Extensions... */
#if TPP_HAVE_EXTENSIONS
#define tpp_lexer_getextension(self, TPP_EXT_x)          tpp_extensions_getid(&(self)->TPP_INTERNAL(tl_exts), TPP_EXT_x)
#define tpp_lexer_setextension(self, TPP_EXT_x, enabled) tpp_extensions_setid(&(self)->TPP_INTERNAL(tl_exts), TPP_EXT_x, enabled)
#define tpp_lexer_enableextension(self, TPP_EXT_x)       tpp_lexer_setextension(self, TPP_EXT_x, true)
#define tpp_lexer_disableextension(self, TPP_EXT_x)      tpp_lexer_setextension(self, TPP_EXT_x, false)
#if TPP_HAVE_EXTENSIONS_PUSH_POP
#define tpp_lexer_pushextensions(self)   tpp_extensions_push(&(self)->TPP_INTERNAL(tl_exts))
#define tpp_lexer_popextensions(self)    tpp_extensions_pop(&(self)->TPP_INTERNAL(tl_exts))
#define tpp_lexer_canpopextensions(self) tpp_extensions_canpop(&(self)->TPP_INTERNAL(tl_exts))
#endif /* TPP_HAVE_EXTENSIONS_PUSH_POP */
#endif /* TPP_HAVE_EXTENSIONS */

/* Features... */
#if TPP_HAVE_FEATURES
#define tpp_lexer_getfeature(self, TPP_FEAT_x)          tpp_features_getid(&(self)->TPP_INTERNAL(tl_feat), TPP_FEAT_x)
#define tpp_lexer_setfeature(self, TPP_FEAT_x, enabled) tpp_features_setid(&(self)->TPP_INTERNAL(tl_feat), TPP_FEAT_x, enabled)
#define tpp_lexer_enablefeature(self, TPP_FEAT_x)       tpp_features_enable(&(self)->TPP_INTERNAL(tl_feat), TPP_FEAT_x)
#define tpp_lexer_disablefeature(self, TPP_FEAT_x)      tpp_features_disable(&(self)->TPP_INTERNAL(tl_feat), TPP_FEAT_x)
#define tpp_lexer_resetfeatures(self)                   tpp_features_reset(&(self)->TPP_INTERNAL(tl_feat))
#endif /* TPP_HAVE_FEATURES */


/* Include path... */
#if TPP_HAVE_INCLUDE_PATH
#define tpp_lexer_includes_addsystem(self, path, path_maxlen)      tpp_include_paths_addsystem(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_addsystem_head(self, path, path_maxlen) tpp_include_paths_addsystem_head(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_delsystem(self, path, path_maxlen)      tpp_include_paths_delsystem(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_getsystem(self, i)                      tpp_include_paths_getsystem(&(self)->TPP_INTERNAL(tl_include_paths), i)
#define tpp_lexer_includes_numsystem(self)                         tpp_include_paths_numsystem(&(self)->TPP_INTERNAL(tl_include_paths))
#if TPP_HAVE_INCLUDE_PATH_QUOTE
#define tpp_lexer_includes_addquote(self, path, path_maxlen)      tpp_include_paths_addquote(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_addquote_head(self, path, path_maxlen) tpp_include_paths_addquote_head(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_delquote(self, path, path_maxlen)      tpp_include_paths_delquote(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_getquote(self, i)                      tpp_include_paths_getquote(&(self)->TPP_INTERNAL(tl_include_paths), i)
#define tpp_lexer_includes_numquote(self)                         tpp_include_paths_numquote(&(self)->TPP_INTERNAL(tl_include_paths))
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
#if TPP_HAVE_INCLUDE_PATH_AFTER
#define tpp_lexer_includes_addafter(self, path, path_maxlen)      tpp_include_paths_addafter(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_addafter_head(self, path, path_maxlen) tpp_include_paths_addafter_head(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_delafter(self, path, path_maxlen)      tpp_include_paths_delafter(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_getafter(self, i)                      tpp_include_paths_getafter(&(self)->TPP_INTERNAL(tl_include_paths), i)
#define tpp_lexer_includes_numafter(self)                         tpp_include_paths_numafter(&(self)->TPP_INTERNAL(tl_include_paths))
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */
#if TPP_HAVE_INCLUDE_PATH_PUSH_POP
#define tpp_lexer_pushincludes(self)   tpp_include_paths_push(&(self)->TPP_INTERNAL(tl_include_paths))
#define tpp_lexer_popincludes(self)    tpp_include_paths_pop(&(self)->TPP_INTERNAL(tl_include_paths))
#define tpp_lexer_canpopincludes(self) tpp_include_paths_canpop(&(self)->TPP_INTERNAL(tl_include_paths))
#endif /* TPP_HAVE_INCLUDE_PATH_PUSH_POP */
#endif /* TPP_HAVE_INCLUDE_PATH */



/* Wrappers for keywords API */
#define _tpp_lexer_kwds_getkeyword(self, kwd, len, hash) _tpp_keywords_getkeyword(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash)
#define _tpp_lexer_kwds_getkeyword_byid(self, id)        _tpp_keywords_getkeyword_byid(&(self)->TPP_INTERNAL(tl_kwds), id)
#define tpp_lexer_kwds_getkeyword(self, kwd, len, hash)  tpp_keywords_getkeyword(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash)
#define tpp_lexer_kwds_getkeyword_byid(self, id)         tpp_keywords_getkeyword_byid(&(self)->TPP_INTERNAL(tl_kwds), id)
#define tpp_lexer_kwds_newkeyword(self, kwd, len, hash)  tpp_keywords_newkeyword(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash)
#if TPP_HAVE_BSE
#define _tpp_lexer_kwds_getkeyword_bse(self, kwd, len, hash, file) _tpp_keywords_getkeyword_esc(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash, file)
#define tpp_lexer_kwds_getkeyword_bse(self, kwd, len, hash, file) tpp_keywords_getkeyword_esc(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash, file)
#define tpp_lexer_kwds_newkeyword_bse(self, kwd, len, hash, file) tpp_keywords_newkeyword_esc(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash, file)
#endif /* TPP_HAVE_BSE */
#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
#define tpp_lexer_kwds_copybuiltin(self, kwd) tpp_keywords_copybuiltin(&(self)->TPP_INTERNAL(tl_kwds), kwd)
#endif /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */



/* Initialize/finalize everything about "self" except for "tl_core" */
#define _tpp_lexer_init_common(self)                         \
	(void)(tpp_keywords_init(&(self)->TPP_INTERNAL(tl_kwds)) \
	       _tpp_lexer_init_exts(self)                        \
	       _tpp_lexer_init_feat(self)                        \
	       _tpp_lexer_init_state(self)                       \
	       _tpp_lexer_init_incpath(self)                     \
	       _tpp_lexer_init_warn(self)                        \
	       _tpp_lexer_init_warnprinter(self)                 \
	       _tpp_lexer_init_parseexpr(self)                   \
	       _tpp_lexer_initerrorcount(self)                   \
	       _tpp_lexer_initerrorlimit(self)                   \
	       _tpp_lexer_initcounter(self)                      \
	       _tpp_lexer_inittime(self))
TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_lexer_fini_common(tpp_lexer *tpp_restrict self);

/* Finalize a given lexer "self" */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_fini(tpp_lexer *tpp_restrict self);


/* Initialize a lexer that simply reads the given [text,text+text_size) blob.
 * @param: start_lc: [valid_if(chunk != NULL)] */
#if TPP_HAVE_UNICODE
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ex(tpp_lexer *tpp_restrict self,
                       /*utf-8*/ char const *filename,
                       /*inherit(always)*/ TPP_REF tpp_string *chunk,
                       void const *text, tpp_size text_size,
                       tpp_lcinfo start_lc, tpp_file_encoding encoding);
#define tpp_lexer_init_text_ascii(self, filename, chunk, text, text_size, start_lc) \
	tpp_lexer_init_text_ex(self, filename, chunk, text, text_size, start_lc, TPP_FILE_ENCODING_ASCII)
#define tpp_lexer_init_text_utf8(self, filename, chunk, text, text_size, start_lc) \
	tpp_lexer_init_text_ex(self, filename, chunk, text, text_size, start_lc, TPP_FILE_ENCODING_FORCE_UTF8)
#else /* TPP_HAVE_UNICODE */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init_text_ascii(tpp_lexer *tpp_restrict self,
                          /*utf-8*/ char const *filename,
                          /*inherit(always)*/ TPP_REF tpp_string *chunk,
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
 * @param: filename_maxlen: Max length of "filename" (in characters). You may
 *                          pass TPP_SIZE_MAX when "filename" is NUL-terminated.
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_init_filename(tpp_lexer *tpp_restrict self,
                        /*utf-8*/ char const *tpp_restrict filename,
                        tpp_size filename_maxlen);
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
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_pushfile_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                         tpp_io_handle handle, tpp_file_ioflags ioflags);
#endif /* TPP_HAVE_LEXER_INIT_IO */

#if TPP_HAVE_LEXER_INIT_FILENAME
/* Push another file onto the #include-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @param: filename_maxlen: Max length of "filename" (in characters). You may
 *                          pass TPP_SIZE_MAX when "filename" is NUL-terminated.
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_pushfile_filename(tpp_lexer *tpp_restrict self,
                            /*utf-8*/ char const *tpp_restrict filename,
                            tpp_size filename_maxlen);
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
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_popfile(tpp_lexer *tpp_restrict self);
#endif /* TPP_HAVE_INCLUDE_STACK */



#if TPP_HAVE_KEYWORDS_OPENFILE
typedef struct tpp_lexer_openfile_result {
	tpp_keyword  *tlofr_filename; /* [1..1] Keyword for filename */
	tpp_io_handle tlofr_handle;   /* [1..1] I/O handle for requested file (must be inherited by caller) */
} tpp_lexer_openfile_result;

#if TPP_HAVE_KEYWORDS_OPENFILE_EX
#define TPP_LEXER_OPENFILE_FLAG_NORMAL 0 /* Normal flags */
#ifdef tpp_keyword_flags
#define tpp_lexer_openfile_flags tpp_keyword_flags /* Set of `TPP_LEXER_OPENFILE_FLAG_*' */
#if TPP_HAVE_CPP_IMPORT
#define TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED TPP_KEYWORD_FLAG_HDR_IMPORTED    /* Filter out files that were already #import-ed */
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_PRAGMA_ONCE
#define TPP_LEXER_OPENFILE_FLAG_HDR_ONCE     TPP_KEYWORD_FLAG_HDR_ONCE        /* Filter out files with "#pragma once" */
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#define TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED  TPP_KEYWORD_FLAG_HDR_GUARD_VALID /* Filter out files with a confirmed "#ifndef"-block of a macro that is current defined */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#else /* tpp_keyword_flags */
#define tpp_lexer_openfile_flags uint_least32_t /* Set of `TPP_LEXER_OPENFILE_FLAG_*' */
#endif /* !tpp_keyword_flags */
#if TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next
#define TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT UINT32_C(0x10000000) /* Reject files that are already on the #include-stack */
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next */

/* Same as `tpp_lexer_openfile', but return `TPP_EMASKED' if the file was already
 * included before, and its keyword has any of the bits specified by `mask_flags' set.
 *
 * A special case is made when "mask_flags & TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED",
 * in which case, "TPP_EMASKED" is only returned if "tkm_file_guard" is a macro that
 * is currently considered to be `#if defined()'.
 *
 * Another special case is made for "TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT", which
 * causes "TPP_EMASKED" to be returned if the file's keyword is already included
 * somewhere on the #include-stack.
 *
 * @param: mask_flags: Set of flags describing circumstances under which TPP_EMASKED
 *                     should be returned:
 *                     - TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED
 *                     - TPP_LEXER_OPENFILE_FLAG_HDR_ONCE
 *                     - TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED
 *                     - TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT
 *
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOMEM:  Insufficient memory
 * @return: TPP_ENOENT:  No such file, or TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT was
 *                       given, and the file is already located on the #include-stack.
 * @return: TPP_EMASKED: Flags specified by "mask_flags" were already set */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3, 5)) tpp_errno TPPCALL
tpp_lexer_openfile_ex(/*1..1*/ tpp_lexer *tpp_restrict self,
                      /*0..1*/ char const *tpp_restrict relative_to,
                      /*1..1*/ /*utf-8*/ char const *filename, tpp_size filename_maxlen,
                      /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result,
                      tpp_lexer_openfile_flags mask_flags);
#define tpp_lexer_openfile(self, relative_to, filename, filename_maxlen, result) \
	tpp_lexer_openfile_ex(self, relative_to, filename, filename_maxlen, result, TPP_LEXER_OPENFILE_FLAG_NORMAL)
#else /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
/* Construct the filename, open the file, and initialize "result" accordingly
 * @param: relative_to: The `tpp_file::tf_data.td_io.tff_name' of another file,
 *                      in case "filename" is a relative path, in which case the
 *                      filename of the file to open should be relative to the
 *                      directory of "relative_to"
 * @param: result:      Open file information (pass along to "tpp_file_init_io()")
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Insufficient memory
 * @return: TPP_ENOENT: File not found (if you have additional "relative_to", try them) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3, 5)) tpp_errno TPPCALL
tpp_lexer_openfile(/*1..1*/ tpp_lexer *tpp_restrict self,
                   /*0..1*/ char const *tpp_restrict relative_to,
                   /*1..1*/ /*utf-8*/ char const *filename, tpp_size filename_maxlen,
                   /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result);
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE_EX */
#endif /* TPP_HAVE_KEYWORDS_OPENFILE */



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



/* Temporarily modify lexer state flags */
#if TPP_HAVE_LEXER_STATE_FLAGS
#define _tpp_lexer_pushstate(self, mask, flags)                                       \
	do {                                                                              \
		tpp_lexer_state_flags const _tlsp_old_flags = (self)->TPP_INTERNAL(tl_state); \
		(self)->TPP_INTERNAL(tl_state) = ((self)->TPP_INTERNAL(tl_state) & (tpp_lexer_state_flags)(mask)) | (tpp_lexer_state_flags)(flags)
#define _tpp_lexer_enablestate(self, state)  (void)((self)->TPP_INTERNAL(tl_state) |= (state))
#define _tpp_lexer_disablestate(self, state) (void)((self)->TPP_INTERNAL(tl_state) &= ~(state))
#define _tpp_lexer_breakstate(self) \
		(void)((self)->TPP_INTERNAL(tl_state) = _tlsp_old_flags)
#define _tpp_lexer_popstate(self)    \
		_tpp_lexer_breakstate(self); \
	} while (0)
#else /* TPP_HAVE_LEXER_STATE_FLAGS */
#define _tpp_lexer_pushstate(self, mask, flags) do {
#define _tpp_lexer_enablestate(self, state)     (void)0
#define _tpp_lexer_disablestate(self, state)    (void)0
#define _tpp_lexer_breakstate(self)             (void)0
#define _tpp_lexer_popstate(self)               } while (0)
#endif /* !TPP_HAVE_LEXER_STATE_FLAGS */
#define _tpp_lexer_pushstate_on(self, flags)  _tpp_lexer_pushstate(self, ~0, flags)
#define _tpp_lexer_pushstate_off(self, flags) _tpp_lexer_pushstate(self, ~(flags), 0)

/* Alter the lexer state such that no warning messages are produces. */
#if TPP_HAVE_WARNINGS
#define tpp_lexer_nowarnings_pushon(self) _tpp_lexer_pushstate_on(self, TPP_LEXER_STATE_FLAG_NOWARNINGS)
#define tpp_lexer_nowarnings_break(self)  _tpp_lexer_breakstate(self)
#define tpp_lexer_nowarnings_pop(self)    _tpp_lexer_popstate(self)
#else /* TPP_HAVE_WARNINGS */
#define tpp_lexer_nowarnings_pushon(self) do {
#define tpp_lexer_nowarnings_break(self)  (void)0
#define tpp_lexer_nowarnings_pop(self)    } while (0)
#endif /* !TPP_HAVE_WARNINGS */

/* Alter the lexer state such that `tpp_lexer_yieldpp()' does not filter tokens. */
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
#define tpp_lexer_alltokens_pushon(self) _tpp_lexer_pushstate_on(self, TPP_LEXER_STATE_FLAG_ALLTOKENS)
#define tpp_lexer_alltokens_break(self)  _tpp_lexer_breakstate(self)
#define tpp_lexer_alltokens_pop(self)    _tpp_lexer_popstate(self)
#else /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#define tpp_lexer_alltokens_pushon(self) do {
#define tpp_lexer_alltokens_break(self)  (void)0
#define tpp_lexer_alltokens_pop(self)    } while (0)
#endif /* !TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */

#if TPP_HAVE_LEXER_MANUALPOPFILE
#define tpp_lexer_popfilerlbk_pushon(self) _tpp_lexer_pushstate_on(self, TPP_LEXER_STATE_FLAG_POPFILERLBK)
#define tpp_lexer_popfilerlbk_break(self)  _tpp_lexer_breakstate(self)
#define tpp_lexer_popfilerlbk_pop(self)    _tpp_lexer_popstate(self)
#else /* TPP_HAVE_LEXER_MANUALPOPFILE */
#define tpp_lexer_popfilerlbk_pushon(self) do {
#define tpp_lexer_popfilerlbk_break(self)  (void)0
#define tpp_lexer_popfilerlbk_pop(self)    } while (0)
#endif /* !TPP_HAVE_LEXER_MANUALPOPFILE */


/* Temporarily disable automatic pop-to-prev-file on EOF */
#define tpp_lexer_autopopfile_pushoff(self) tpp_file_autopopfile_pushoff(tpp_lexer_getfile(self))
#define tpp_lexer_autopopfile_break(self)   tpp_file_autopopfile_break(tpp_lexer_getfile(self))
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


typedef struct tpp_lexer_seek_backup {
	tpp_token_id              TPP_INTERNAL(tlsb_id);    /* Saved token id */
	struct tpp_keyword const *TPP_INTERNAL(tlsb_kwd);   /* [1..1][valid_if(TPP_TOK_ISKEYWORD(tlsb_id))] Saved token keyword */
	tpp_size                  TPP_INTERNAL(tlsb_len);   /* Relative length of token */
} tpp_lexer_seek_backup;

/* Save/restore the currently loaded token. This must be done before/after
 * making use of `tpp_lexer_yieldraw_at()' with a custom text pointer:
 * >> tpp_token_id seek_next_token(tpp_lexer *self) {
 * >>     tpp_token_id result;
 * >>     tpp_lexer_seek_backup backup;
 * >>     tpp_char const *pos;
 * >>     pos    = tpp_lexer_seek_start(self, &backup);
 * >>     result = tpp_lexer_yieldraw_at(self, &pos);
 * >>     if (SHOULD_COMMIT(result)) {
 * >>         tpp_lexer_seek_commit(self, pos);
 * >>     } else {
 * >>         tpp_lexer_seek_rollback(self, &backup);
 * >>     }
 * >>     return result;
 * >> } */
TPP_INLINE TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_lexer_seek_start(tpp_lexer *tpp_restrict self,
                     tpp_lexer_seek_backup *tpp_restrict backup) {
	tpp_char const *result;
	tpp_token *const token = tpp_lexer_gettoken(self);
	backup->TPP_INTERNAL(tlsb_id)  = tpp_token_getid(token);
	backup->TPP_INTERNAL(tlsb_kwd) = tpp_token_getkwd(token);
	backup->TPP_INTERNAL(tlsb_len) = tpp_token_getlen(token);
	result                         = tpp_token_getend(token);
	token->TPP_INTERNAL(tt_end)    = tpp_token_getstart(token);
	return result;
}
#define tpp_lexer_seek_commit(self, pos) \
	(void)(tpp_lexer_gettoken(self)->TPP_INTERNAL(tt_end) = (pos))
#define tpp_lexer_seek_rollback(self, backup)                                                                             \
	(tpp_lexer_gettoken(self)->TPP_INTERNAL(tt_kwd)   = (backup)->TPP_INTERNAL(tlsb_kwd),                                 \
	 tpp_lexer_gettoken(self)->TPP_INTERNAL(tt_start) = tpp_lexer_gettokenend(self),                                      \
	 tpp_lexer_gettoken(self)->TPP_INTERNAL(tt_end)   = tpp_lexer_gettokenstart(self) + (backup)->TPP_INTERNAL(tlsb_len), \
	 tpp_lexer_gettoken(self)->TPP_INTERNAL(tt_id)    = (backup)->TPP_INTERNAL(tlsb_id))



#if TPP_HAVE_LEXER_MANUALPOPFILE
/* Enter a region of code where it is possible to manually (and possibly temporarily)
 * pop the currently loaded lexer file such that the file next-up in the #include-
 * stack can be yielded from instead.
 *
 * NOTES:
 *  - These macros respect "tpp_lexer_autopopfile_pushoff" such that
 *    "tpp_lexer_manualpopfile_canpopfile(self)" returns "false" if
 *    the then top-most file had automatic popping disabled.
 *  - Changes made to the effective "tf_pos" of files must be restored
 *    by the user before calling "tpp_lexer_manualpopfile_popfile()".
 *
 * This functionality is needed/used to scan for the opening '(' token in macro calls
 * in those cases where the file containing the original macro invocation ends with
 * EOF before such a token can be located:
 * >> #define foo(a, b) a+b
 * >> #define bar       foo
 * >> bar(10, 20)  // Here, "foo" (macro name) and "(" are located in different files, but macro expansion must still happen
 * >> bar[10, 20]  // But for this case, a rollback behavior is still needed to still yield "foo" as an identifer
 *
 * Use these macros as follows:
 * >> // Check if the next "raw", non-space token will be "(".
 * >> // If so, yield to it (such that the current token becomes '(') and return "true"
 * >> // Otherwise, don't change anything and return "false"
 * >> // -- Error handling omitted for brevity
 * >> bool search_for_lparen(tpp_lexer *self) {
 * >>     tpp_lexer_seek_backup backup;
 * >>     tpp_char const *pos = tpp_lexer_seek_start(self, &backup);
 * >>     tpp_token_id tok;
 * >>     do {
 * >>         tok = tpp_lexer_yieldraw_at(self, &pos);
 * >>     } while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
 * >>     if (tok == '(') { // Found in same file
 * >>         tpp_lexer_seek_commit(self, pos);
 * >>         return true;
 * >>     }
 * >>     tpp_lexer_seek_rollback(self, &backup);
 * >>     if (tok == TPP_TOK_EOF) {
 * >>         // Check files further up the #include-stack
 * >>         tpp_lexer_manualpopfile_start(self);
 * >>         while (tpp_lexer_manualpopfile_canpopfile(self)) {
 * >>             tpp_lexer_manualpopfile_popfile(self);
 * >>             pos = tpp_lexer_seek_start(self, &backup);
 * >>             do {
 * >>                 tok = tpp_lexer_yieldraw_at(self, &pos);
 * >>             } while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
 * >>             if (tok == '(') {
 * >>                 tpp_lexer_seek_commit(self, pos);
 * >>                 tpp_lexer_manualpopfile_break_commit(self);
 * >>                 return true;
 * >>             }
 * >>             tpp_lexer_seek_rollback(self, &backup);
 * >>             if (tok != TPP_TOK_EOF)
 * >>                 break;
 * >>         }
 * >>         tpp_lexer_manualpopfile_end_rollback(self);
 * >>     }
 * >>     return false;
 * >> }
 *
 * Another use case is to:
 * - Setup
 *   - Use "tpp_file_autopopfile_pushoff()" to set a limit on how far files can be popped
 *   - Use "tpp_file_setkeep()" on all popable files to prevent old data from being free'd
 *   - Save "tpp_file_getpos()" of popable files relative to their "tpp_file_getkeep()"
 *   - Use "tpp_lexer_manualpopfile_start()"
 *   - Set the "TPP_LEXER_STATE_FLAG_POPFILERLBK" state flag
 * - At this, functions like "tpp_lexer_yieldpp()" or "tpp_lexer_yield()"
 *   can be called like normal.
 * - Rollback can then Implement rollback as:
 *   - Restore the "TPP_LEXER_STATE_FLAG_POPFILERLBK" flag
 *   - "tpp_lexer_manualpopfile_end_rollback(self)"
 *   - Restore positions of popable files by doing "tpp_file_getkeep() + REL_POS"
 *   - Restore the old keep-positions of popable files
 *   - Calling "tpp_file_autopopfile_pop()" on popable files
 * - But be careful: side-effects like macro definitions or #pragma directives aren't rolled back"
 *
 * HINT: Something similar to this is done to parse the argument lists of macros when
 *       rollback needs to be possible due to the "TPP_MACRO_FLAG_SELFEXPAND" extension.
 *
 * WARNING: All additional files pushed onto the #include-stack at any point while
 *          inside of a tpp_lexer_manualpopfile_start()-block must be popped again
 *          before the block may be committed or rolled back! */
#define tpp_lexer_manualpopfile_start(self) \
	do{ tpp_file *const _tlmpf_orig_prev = tpp_lexer_getfile(self)->TPP_INTERNAL(tf_prev)
TPP_DECL TPP_NONNULL((1)) void TPPCALL tpp_lexer_manualpopfile_popfile(tpp_lexer *tpp_restrict self);
#define tpp_lexer_manualpopfile_canpopfile(self) (tpp_lexer_getfile(self)->TPP_INTERNAL(tf_prev) != NULL)
TPP_DECL TPP_NONNULL((1)) void TPPCALL _tpp_lexer_manualpopfile_break_rollback(tpp_lexer *tpp_restrict self, tpp_file *tpp_restrict orig_prev);
TPP_DECL TPP_NONNULL((1)) void TPPCALL _tpp_lexer_manualpopfile_break_commit(tpp_lexer *tpp_restrict self, tpp_file *tpp_restrict orig_prev);
#define tpp_lexer_manualpopfile_break_rollback(self) _tpp_lexer_manualpopfile_break_rollback(self, _tlmpf_orig_prev)
#define tpp_lexer_manualpopfile_break_commit(self)   _tpp_lexer_manualpopfile_break_commit(self, _tlmpf_orig_prev)
#define tpp_lexer_manualpopfile_break(self, rollback)   \
		((rollback)                                     \
		 ? tpp_lexer_manualpopfile_break_rollback(self) \
		 : tpp_lexer_manualpopfile_break_commit(self))
#define tpp_lexer_manualpopfile_end_rollback(self)    \
		tpp_lexer_manualpopfile_break_rollback(self); \
	} while (0)
#define tpp_lexer_manualpopfile_end_commit(self)    \
		tpp_lexer_manualpopfile_break_commit(self); \
	} while (0)
#define tpp_lexer_manualpopfile_end(self, rollback)    \
		tpp_lexer_manualpopfile_break(self, rollback); \
	} while (0)
#endif /* TPP_HAVE_LEXER_MANUALPOPFILE */



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

/* Handle a keyword-style macro (used to implement "tpp_lexer_yield()").
 * @return: TPP_TOK_EOF: Caller should yield again.
 * @return: * : The new expansion token after keywords were handled */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_keyword(tpp_lexer *tpp_restrict self, tpp_token_id tok);



#if TPP_HAVE_FILE_NONBLOCK
/* Same as `tpp_lexer_yield()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_blocking(tpp_lexer *tpp_restrict self);

/* Same as `tpp_lexer_yieldpp()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp_blocking(tpp_lexer *tpp_restrict self);

/* Same as `tpp_lexer_yieldraw()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
#define tpp_lexer_yieldraw_blocking(self) \
	tpp_lexer_yieldraw_at_blocking(self, &tpp_lexer_gettoken(self)->TPP_INTERNAL(tt_end))

/* Same as `tpp_lexer_yieldraw_at()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_blocking(tpp_lexer *tpp_restrict self, tpp_char const **p_pos);
#else /* TPP_HAVE_FILE_NONBLOCK */
#define tpp_lexer_yield_blocking(self)              tpp_lexer_yield(self)
#define tpp_lexer_yieldpp_blocking(self)            tpp_lexer_yieldpp(self)
#define tpp_lexer_yieldraw_blocking(self)           tpp_lexer_yieldraw(self)
#define tpp_lexer_yieldraw_at_blocking(self, p_pos) tpp_lexer_yieldraw_at(self, p_pos)
#endif /* !TPP_HAVE_FILE_NONBLOCK */



#if TPP_HAVE_LEXER_YIELD_INCLUDE_STRING
/* (Mostly) the same as "tpp_lexer_yield()", except:
 * - Never process preprocessor directives (but macros are still expanded)
 * - If the next token starts with '"' or '<', parse it as a #include-string,
 *   with the token's start/end bounds pointing at the string's bounds. In
 *   this case, the token's ID (and return value) is:
 *   - TPP_TOK_OFCHAR('"')  // For #include "foo.h"
 *   - TPP_TOK_OFCHAR('<')  // For #include <foo.h>
 * - WARNING: This function doesn't filter SPACE/LF/COMMENT tokens
 *   (behaves as though 'TPP_LEXER_STATE_FLAG_ALLTOKENS' was set)
 *
 * @return: * : Some other token encountered (token was parsed like tpp_lexer_yieldraw())
 * @return: TPP_TOK_OFCHAR('"'): #include-string parsed: "foo.h"
 * @return: TPP_TOK_OFCHAR('<'): #include-string parsed: <foo.h>
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
#if TPP_HAVE_CPP_MACROS
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_include_string(tpp_lexer *tpp_restrict self);
#else /* TPP_HAVE_CPP_MACROS */
#define tpp_lexer_yield_include_string(self) tpp_lexer_yieldraw_include_string(self)
#endif /* !TPP_HAVE_CPP_MACROS */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_include_string(tpp_lexer *tpp_restrict self, tpp_char const **p_pos);
#if TPP_HAVE_FILE_NONBLOCK
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_include_string_blocking(tpp_lexer *tpp_restrict self, tpp_char const **p_pos);
#if TPP_HAVE_CPP_MACROS
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_include_string_blocking(tpp_lexer *tpp_restrict self);
#else /* TPP_HAVE_CPP_MACROS */
#define tpp_lexer_yield_include_string_blocking(self) tpp_lexer_yieldraw_include_string_blocking(self)
#endif /* !TPP_HAVE_CPP_MACROS */
#else /* TPP_HAVE_FILE_NONBLOCK */
#define tpp_lexer_yieldraw_at_include_string_blocking(self, p_pos) tpp_lexer_yieldraw_at_include_string(self, p_pos)
#define tpp_lexer_yield_include_string_blocking(self)              tpp_lexer_yield_include_string(self)
#endif /* !TPP_HAVE_FILE_NONBLOCK */
#define tpp_lexer_yieldraw_include_string_blocking(self, p_pos) \
	tpp_lexer_yieldraw_at_include_string_blocking(self, &tpp_lexer_gettoken(self)->TPP_INTERNAL(tt_end))
#define tpp_lexer_yieldraw_include_string(self) \
	tpp_lexer_yieldraw_at_include_string(self, &tpp_lexer_gettoken(self)->TPP_INTERNAL(tt_end))
#endif /* TPP_HAVE_LEXER_YIELD_INCLUDE_STRING */

#if TPP_HAVE_LEXER_DECODE_INCLUDE_STRING
/* Decode the current token as a #include-string. The caller is responsible to
 * ensure that the current token was loaded by `tpp_lexer_yield_include_string()'
 * and is either TPP_TOK_OFCHAR('<') or TPP_TOK_OFCHAR('"')
 *
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_decode_include_string(tpp_lexer const *tpp_restrict self,
                                tpp_formatprinter printer, void *arg);

/* Same as `tpp_lexer_decode_include_string()', but the given "cb" is only
 * invoked once whilst being passed the *entire* (decoded) #include-string.
 *
 * @return: * : Return value of the (singular) invocation of "cb"
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decode_include_string_cb(tpp_lexer const *tpp_restrict self,
                                   tpp_errno (TPPCALL *cb)(void *arg, tpp_char const *str, tpp_size length),
                                   void *arg);
#endif /* TPP_HAVE_LEXER_DECODE_INCLUDE_STRING */


#if TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
/* Wrapper around `tpp_lexer_decode_include_string_cb()' that automatically
 * does the necessary calls to `tpp_lexer_openfile_ex()'. It also handles
 * the `TPP_ENOENT' (as far as possible) by continuing to search for other
 * matching files. The specified "mask_flags" should be set depending on
 * context like:
 * - #include ...             TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED
 * - __has_include(...)       TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED
 * - #include_next ...        TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED | TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT
 * - __has_include_next(...)  TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED | TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT
 * - #import ...              TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED | TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED
 * - #embed ...               TPP_LEXER_OPENFILE_FLAG_NORMAL
 * - __has_embed(...)         TPP_LEXER_OPENFILE_FLAG_NORMAL
 *
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOMEM:  Insufficient memory
 * @return: TPP_ENOENT:  No such file (no warning printed, yet)
 * @return: TPP_EMASKED: (tpp_lexer_open_include_string_ex only): Flags
 *                       specified by "mask_flags" were already set. */
#if TPP_HAVE_KEYWORDS_OPENFILE_EX
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_open_include_string_ex(tpp_lexer *tpp_restrict self,
                                 /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result,
                                 tpp_lexer_openfile_flags mask_flags);
#define tpp_lexer_open_include_string(self, result) \
	tpp_lexer_open_include_string_ex(self, result, 0)
#else /* TPP_HAVE_KEYWORDS_OPENFILE_EX */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_open_include_string(tpp_lexer *tpp_restrict self,
                              /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result);
#endif /* !TPP_HAVE_KEYWORDS_OPENFILE_EX */
#endif /* TPP_HAVE_LEXER_OPEN_INCLUDE_STRING */




#if TPP_HAVE_LEXER_SKIP
/* Check that the currently loaded token is 'tok'. If so, "tpp_lexer_yield_blocking()" to
 * the next token (which is also returned). Otherwise, trigger 'TPP_W_UNEXPECTED_TOKEN'
 * and (if that warning wasn't fatal), try to seek ahead to see if "tok" can be found
 * somewhere close by (depending on what 'tok' and what was actually loaded on entry)
 *
 * NOTE: This function automatically handles "TPP_TOK_EWOULDBLOCK"
 *
 * @return: * :                 The token that comes after the one that was just skipped
 * @return: TPP_TOK_ENOMEM:     Out of memory
 * @return: TPP_TOK_EIO:        I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:  Lexer error
 * @return: TPP_TOK_EWARNPRINT: Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip(tpp_lexer *tpp_restrict self, tpp_token_id tok);

/* Same as "tpp_lexer_skip()", but don't advance to the next token,
 * except in those cases where the requested "tok" could be found
 * a little further up ahead, and the implementation decided that
 * the tokens that lay in-between should be skipped.
 *
 * @return: * :                 The currently loaded token
 * @return: tok:                Success
 * @return: TPP_TOK_ENOMEM:     Out of memory
 * @return: TPP_TOK_EIO:        I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:  Lexer error
 * @return: TPP_TOK_EWARNPRINT: Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_require(tpp_lexer *tpp_restrict self, tpp_token_id tok);
#endif /* TPP_HAVE_LEXER_SKIP */



#if TPP_HAVE_LEXER_TRYSKIP_RAW
#define TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL      0x0000 /* Normal flags */
#define TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONSPACE 0x0001 /* Rollback if a NOLINE-COMMENT or SPACE token is hit */
#define TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONLF    0x0002 /* Rollback if a LINE-COMMENT or LF token is hit */
#define TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV    0x0004 /* On success, include the previous token in the selected text-area, too */

/* Make use of:
 * - tpp_lexer_seek_start()
 * - tpp_lexer_yieldraw_at()
 * - tpp_lexer_manualpopfile_start(self)
 * to seek ahead to the next token, skipping whitespace/line-feed (+resp. comments)
 * based on "flags", check if said "next token" is equal to "expected" (with some extra-
 * extra handling when "TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS && expected == '<'").
 * If that is the case, commit the lexer such that it points at a token equal to
 * the specified "expected", truly disposing of any files popped in the mean-time.
 * Otherwise (the next token is "!= expected"), roll back any changes made, such
 * that "self" once again points at the same token it did upon entry. In either
 * case, return the ID of whatever token came next.
 *
 * NOTE: This function automatically handles "TPP_TOK_EWOULDBLOCK" by blocking!
 *
 * @return: * :                 The next token (rollback)
 * @return: expected:           The next token (commit)
 *                              When "TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV" is set,
 *                              the text-range of "self" will actually include the
 *                              previous token as well.
 * @return: TPP_TOK_ENOMEM:     Out of memory
 * @return: TPP_TOK_EIO:        I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:  Lexer error
 * @return: TPP_TOK_EWARNPRINT: Error while printing a warning */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_tryskip_raw(tpp_lexer *tpp_restrict self, tpp_token_id expected,
                      unsigned int flags);
#endif /* TPP_HAVE_LEXER_TRYSKIP_RAW */



#if TPP_HAVE_LEXER_SEEKPP_RPAREN

#define TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL     0x0000
#define TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS    0x0001 /* Store varargs info in p_argv[IN(*p_argc) - 1] */
#if TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
#define TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC 0x0002 /* Do not strip whitespace/comments around arguments */
#endif /* TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_HAVE_LEXER_MANUALPOPFILE
#define TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK    0x0004 /* Use "tpp_lexer_manualpopfile_popfile()" to pop files */
#endif /* TPP_HAVE_LEXER_MANUALPOPFILE */
#define TPP_LEXER_SEEK_RPAREN_FLAG_NOWARNEOF  0x0008 /* Do not emit "TPP_W_EOF_IN_ARGUMENT_LIST" warnings */

typedef struct tpp_lexer_arginfo {
	/* NOTE: Leading/trailing whitespace in arguments is controlled by "TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC" */
	tpp_char const     *tlai_start;  /* [1..1][<= tlai_end] Pointer to argument start text data */
	tpp_char const     *tlai_end;    /* [1..1][>= tlai_start] Pointer to argument end text data */
	TPP_REF tpp_string *tlai_chunk;  /* [0..1] Chunk of text containing [tlai_start,tlai_end), or "NULL" if statically allocated */
} tpp_lexer_arginfo;

#define tpp_lexer_arginfo_init_empty(self) \
	(void)((self)->tlai_chunk = NULL, (self)->tlai_start = (self)->tlai_end = NULL)
#define tpp_lexer_arginfo_fini(self) \
	(void)(!(self)->tlai_chunk || (tpp_string_decref((self)->tlai_chunk), 0))
#define tpp_lexer_arginfo_copy(dst, src) \
	(void)(*(dst) = *(src), (!(self)->tlai_chunk || (tpp_string_incref((self)->tlai_chunk), 0)))

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
#define tpp_lexer_seekpp_rparen(self, p_argv, p_argc, p_rollback_pos, opt_function_name_for_messages, flags) \
	tpp_lexer_seekpp_rparen_ex(self, p_argv, p_argc, p_rollback_pos, opt_function_name_for_messages, flags, TPP_TOK_LPAREN)
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen_ex(tpp_lexer *tpp_restrict self,
                           tpp_lexer_arginfo *tpp_restrict p_argv,
                           tpp_size *tpp_restrict p_argc,
                           tpp_char const **p_rollback_pos,
                           char const *opt_function_name_for_messages,
                           unsigned int flags,
                           tpp_token_id lparen_kind);
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen(tpp_lexer *tpp_restrict self,
                        tpp_lexer_arginfo *tpp_restrict p_argv,
                        tpp_size *tpp_restrict p_argc,
                        tpp_char const **p_rollback_pos,
                        char const *opt_function_name_for_messages,
                        unsigned int flags);
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */

/* Same as above, but always initializes *exactly* "argc" arguments,
 * and automatically emits "TPP_W_TOO_FEW_ARGUMENTS" when fewer were
 * parsed. */
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
#define tpp_lexer_seekpp_rparen_exact(self, p_argv, argc, opt_function_name_for_messages, flags) \
	tpp_lexer_seekpp_rparen_exact_ex(self, p_argv, argc, opt_function_name_for_messages, flags, TPP_TOK_LPAREN)
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen_exact_ex(tpp_lexer *tpp_restrict self,
                                 tpp_lexer_arginfo *tpp_restrict p_argv, tpp_size argc,
                                 char const *opt_function_name_for_messages,
                                 unsigned int flags, tpp_token_id lparen_kind);
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_seekpp_rparen_exact(tpp_lexer *tpp_restrict self,
                              tpp_lexer_arginfo *tpp_restrict p_argv, tpp_size argc,
                              char const *opt_function_name_for_messages,
                              unsigned int flags);
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
#endif /* TPP_HAVE_LEXER_SEEKPP_RPAREN */





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
#elif TPP_HAVE_CPP_MACROS
#define tpp_lexer_getkeyworddefined(self, kwd) tpp_keyword_canundef(kwd)
#else /* ... */
#define tpp_lexer_getkeyworddefined(self, kwd) 0
#endif /* !... */


#if TPP_HAVE_LEXER_DECODEINT
#undef TPP_HAVE_INTEGER_SUFFIX_KIND
#if TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS || TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
#define TPP_HAVE_INTEGER_SUFFIX_KIND 1
typedef enum tpp_integer_suffix_kind {
	TPP_INTEGER_SUFFIX_KIND_INT, /* "" (default) */
#if TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS
	TPP_INTEGER_SUFFIX_KIND_UNSIGNED,           /* "u" */
	TPP_INTEGER_SUFFIX_KIND_LONG,               /* "l" */
	TPP_INTEGER_SUFFIX_KIND_UNSIGNED_LONG,      /* "ul" */
	TPP_INTEGER_SUFFIX_KIND_LONG_LONG,          /* "ll" */
	TPP_INTEGER_SUFFIX_KIND_UNSIGNED_LONG_LONG, /* "ull" */
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS */
#if TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS
	TPP_INTEGER_SUFFIX_KIND_INT8,   /* "i8" */
	TPP_INTEGER_SUFFIX_KIND_INT16,  /* "i16" */
	TPP_INTEGER_SUFFIX_KIND_INT32,  /* "i32" */
	TPP_INTEGER_SUFFIX_KIND_INT64,  /* "i64" */
	TPP_INTEGER_SUFFIX_KIND_UINT8,  /* "ui8" */
	TPP_INTEGER_SUFFIX_KIND_UINT16, /* "ui16" */
	TPP_INTEGER_SUFFIX_KIND_UINT32, /* "ui32" */
	TPP_INTEGER_SUFFIX_KIND_UINT64, /* "ui64" */
#endif /* TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */
} tpp_integer_suffix_kind;
#else /* TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS || TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */
#define TPP_HAVE_INTEGER_SUFFIX_KIND 0
#endif /* !TPP_HAVE_BUILTIN_EXPR_FIXED_TYPE_INTEGRALS && !TPP_HAVE_BUILTIN_EXPR_FIXED_LENGTH_INTEGRALS */


/* Decode the current token (which should be TPP_TOK_INT) into an integer
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_INTEGER_SUFFIX_KIND
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_ex(tpp_lexer *tpp_restrict self,
                       tpp_intmax *tpp_restrict result,
                       tpp_integer_suffix_kind *p_suffix_kind);
#define tpp_lexer_decodeint(self, result) \
	tpp_lexer_decodeint_ex(self, result, NULL)
#else /* TPP_HAVE_INTEGER_SUFFIX_KIND */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint(tpp_lexer *tpp_restrict self,
                    tpp_intmax *tpp_restrict result);
#endif /* !TPP_HAVE_INTEGER_SUFFIX_KIND */
#endif /* TPP_HAVE_LEXER_DECODEINT */

/* Decode the current token (which should be TPP_TOK_INT) into an integer
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEINT_EXPR
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_expr(tpp_lexer *tpp_restrict self,
                         tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE_LEXER_DECODEINT_EXPR */


/* Decode the current token (which should be TPP_TOK_FLOAT) into a float
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEFLOAT
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat(tpp_lexer *tpp_restrict self,
                      tpp_float *tpp_restrict result);
#endif /* TPP_HAVE_LEXER_DECODEFLOAT */

/* Decode the current token (which should be TPP_TOK_FLOAT) into a float
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Lexer error happened
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#if TPP_HAVE_LEXER_DECODEFLOAT_EXPR
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat_expr(tpp_lexer *tpp_restrict self,
                           tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE_LEXER_DECODEFLOAT_EXPR */





#if TPP_HAVE_LEXER_DECODESTRING
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
 * @param: cb.arg:   Cookie argument (s.a. `arg')
 * @param: cb.chunk: The string-chunk containing "str" (or "NULL" if "str" is statically allocated)
 *                   NOTE: May be non-NULL, even if "str" is statically allocated!
 * @param: flags:    Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning
 * @return: * :             Return value of given "cb" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring_cb(tpp_lexer *self,
                         tpp_errno (TPPCALL *cb)(void *arg, tpp_string *chunk,
                                                 tpp_char const *str, tpp_size length),
                         void *arg, unsigned int flags);

#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
/* Convenience wrapper to parse a character integer literal
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
tpp_lexer_parsecharacter_literal(tpp_lexer *tpp_restrict self,
                                 /*out*/ tpp_intmax *tpp_restrict p_result,
                                 unsigned int flags);
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
#endif /* TPP_HAVE_LEXER_DECODESTRING */

#undef TPP_HAVE_BUILTIN_LEXER_PARSESTRING_EXPR
#if TPP_HAVE_LEXER_PARSESTRING_EXPR
/* Convenience wrapper around `tpp_lexer_parsestring()'
 * On success (!TPP_ISERR(return)), caller must "tpp_expr_value_fini(result)"
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#ifndef tpp_lexer_parsestring_expr
#define tpp_lexer_parsestring_expr tpp_lexer_parsestring_expr
#define TPP_HAVE_BUILTIN_LEXER_PARSESTRING_EXPR 1
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring_expr(tpp_lexer *tpp_restrict self,
                           /*out*/ tpp_expr_value *tpp_restrict result,
                           unsigned int flags);
#endif /* !tpp_lexer_parsestring_expr */
#endif /* TPP_HAVE_LEXER_PARSESTRING_EXPR */
#ifndef TPP_HAVE_BUILTIN_LEXER_PARSESTRING_EXPR
#define TPP_HAVE_BUILTIN_LEXER_PARSESTRING_EXPR 0
#endif /* !TPP_HAVE_BUILTIN_LEXER_PARSESTRING_EXPR */



#undef TPP_HAVE_BUILTIN_LEXER_PARSECHARACTER_EXPR
#if TPP_HAVE_LEXER_PARSECHARACTER_EXPR
/* Convenience wrapper around `tpp_lexer_parsecharacter()'
 * On success (!TPP_ISERR(return)), caller must "tpp_expr_value_fini(result)"
 *
 * @param: flags: Set of `TPP_LEXER_PARSECHARACTER_FLAG_*'
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ELEXERROR:  Either one of the printers returned this value, or
 *                          a lexer error happened (s.a. `tpp_lexer_warnf()').
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O error while yielding to next token
 * @return: TPP_EWARNPRINT: Error while printing a warning */
#ifndef tpp_lexer_parsecharacter_expr
#define tpp_lexer_parsecharacter_expr tpp_lexer_parsecharacter_expr
#define TPP_HAVE_BUILTIN_LEXER_PARSECHARACTER_EXPR 1
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsecharacter_expr(tpp_lexer *tpp_restrict self,
                              /*out*/ tpp_expr_value *tpp_restrict result,
                              unsigned int flags);
#endif /* !tpp_lexer_parsecharacter_expr */
#endif /* TPP_HAVE_LEXER_PARSECHARACTER_EXPR */
#ifndef TPP_HAVE_BUILTIN_LEXER_PARSECHARACTER_EXPR
#define TPP_HAVE_BUILTIN_LEXER_PARSECHARACTER_EXPR 0
#endif /* !TPP_HAVE_BUILTIN_LEXER_PARSECHARACTER_EXPR */



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
 * @param: pos:        [0..1] Lexer position used by certain format-patterns.
 * @param: pos_lcinfo: Lexer position used when "pos == NULL"
 * @param: file:       The file containing "pos"
 * @param: printer:    Output printer for formatted text
 * @param: arg:        Cookie argument for "printer"
 * @param: format:     Format pattern (see above)
 * @param: args:       Extra varargs-arguments for "format"
 * @return: >= 0:      Sum of return values of "printer".
 * @return: < 0:       First negative return value of "printer". The more high-level
 *                     "tpp_lexer_warnf" API returns "TPP_EWARNPRINT" in this case. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 5, 7)) tpp_ssize TPPVCALL
tpp_lexer_printf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                         tpp_lcinfo pos_lcinfo, tpp_formatprinter printer, void *arg,
                         char const *format, ...);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 5, 7)) tpp_ssize TPPCALL
tpp_lexer_vprintf_warning(tpp_lexer const *self, tpp_file *file, tpp_char const *pos,
                          tpp_lcinfo pos_lcinfo, tpp_formatprinter printer, void *arg,
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
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_lc(tpp_lexer *tpp_restrict self, tpp_lcinfo lc, tpp_warning_id id, va_list args);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf_lc(tpp_lexer *tpp_restrict self, tpp_lcinfo lc, tpp_warning_id id, ...);
#else /* TPP_HAVE_WARNINGS */
#define tpp_lexer_vwarnf(self, id, args)         TPP_EOK
#define tpp_lexer_vwarnf_at(self, pos, id, args) TPP_EOK
#define tpp_lexer_vwarnf_lc(self, lc, id, args)  TPP_EOK
#if TPP_HOST_HAVE_PP_VARARGS
#define tpp_lexer_warnf(self, id, ...)           TPP_EOK
#define tpp_lexer_warnf_at(self, pos, id, ...)   TPP_EOK
#define tpp_lexer_warnf_lc(self, lc, id, ...)    TPP_EOK
#else /* TPP_HOST_HAVE_PP_VARARGS */
TPP_INLINE tpp_errno TPPVCALL
tpp_lexer_warnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, ...) {
	(void)self;
	(void)id;
	return TPP_EOK;
}

TPP_INLINE tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_char const *pos, tpp_warning_id id, ...) {
	(void)self;
	(void)pos;
	(void)id;
	return TPP_EOK;
}

TPP_INLINE tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_lcinfo lc, tpp_warning_id id, ...) {
	(void)self;
	(void)lc;
	(void)id;
	return TPP_EOK;
}
#endif /* !TPP_HOST_HAVE_PP_VARARGS */
#endif /* !TPP_HAVE_WARNINGS */


/* Warn if the current file's #ifdef-stack is non-empty.
 * @return: * : See `tpp_lexer_warnf()' */
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_warn_nonempty_ifdef(tpp_lexer *tpp_restrict self);
#else /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */
#define tpp_lexer_warn_nonempty_ifdef(self) TPP_EOK
#endif /* !TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */


/* Return the (canonical) string-representation of a given token ID */
#if TPP_HAVE_LEXER_REPRTOKENID
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) char const *TPPCALL
tpp_lexer_reprtokenid(tpp_lexer const *tpp_restrict self, tpp_token_id tok);
#endif /* TPP_HAVE_LEXER_REPRTOKENID */



/* Builtin warning printers... */
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
TPP_DECL TPP_FORMATPRINTER_DEFINE(_tpp_lexer_wrapped_warnprinter, arg, text, num_bytes);
#endif /* TPP_HAVE__TPP_LEXER_WRAPPED_WARNPRINTER */
#if TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER
TPP_DECL TPP_FORMATPRINTER_DEFINE(_tpp_lexer_builtin_warnprinter, arg, text, num_bytes);
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_WARNPRINTER */
#if TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER
TPP_DECL TPP_FORMATPRINTER_DEFINE(_tpp_lexer_noop_warnprinter, arg, text, num_bytes);
#endif /* TPP_HAVE__TPP_LEXER_NOOP_WARNPRINTER */


/* Builtin expression parsers... */
#ifndef TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR
#define TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR 0
#endif /* !TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR */
#ifndef TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR_WITH_ARG
#define TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR_WITH_ARG 0
#endif /* !TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR_WITH_ARG */
#if TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_lexer_builtin_parseexpr(tpp_lexer *tpp_restrict self,
                             tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR */
#if TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR_WITH_ARG
TPP_DECL TPP_WUNUSED TPP_NONNULL((2, 3)) tpp_errno TPPCALL
_tpp_lexer_builtin_parseexpr_with_arg(void *arg, tpp_lexer *tpp_restrict self,
                                      tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE__TPP_LEXER_BUILTIN_PARSEEXPR_WITH_ARG */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_H */
