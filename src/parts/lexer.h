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
/*!depends config.h*/
#ifndef GUARD_TPP_LEXER_H
#define GUARD_TPP_LEXER_H 1

#include "api.h"

#include "config.h"
#include "expr.h"
#include "extensions.h"
#include "features.h"
#include "file.h"
#include "hooks.h"
#include "keyword.h"
#include "sysinclude.h"
#include "time.h"
#include "token.h"
#include "warnings.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#undef TPP_HAVE_LEXER_STATE_FLAGS
#if (TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS || \
     TPP_HAVE_WARNINGS)
#define TPP_HAVE_LEXER_STATE_FLAGS 1
#else /* ... */
#define TPP_HAVE_LEXER_STATE_FLAGS 0
#endif /* !... */


#if TPP_HAVE_LEXER_STATE_FLAGS
#define tpp_lexer_state_flags uint_least8_t
#define TPP_LEXER_STATE_FLAG_NORMAL       UINT8_C(0x00) /* Normal state flags */
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
#define TPP_LEXER_STATE_FLAG_ALLTOKENS    UINT8_C(0x01) /* Prevent `tpp_lexer_yieldpp()` from (possibly) filtering SPACE/LF/COMMENT tokens */
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#if TPP_HAVE_WARNINGS
#define TPP_LEXER_STATE_FLAG_NOWARNINGS   UINT8_C(0x02) /* Do not emit any warnings/errors (don't even trigger them) -- should be used during seek-ahead yields. */
#endif /* TPP_HAVE_WARNINGS */
#endif /* TPP_HAVE_LEXER_STATE_FLAGS */


union TPP_INTERNAL(tpp_lexer_core) {
	tpp_token      TPP_INTERNAL(tlc_tok);  /* [valid_if(WAS_CALLED(tpp_lexer_yieldraw()))] Last-read token (never
	                                        * set to one of `TPP_TOK_E*`; iow: always positive or TPP_TOK_EOF). */
	struct {
		char _tli_pad[tpp_offsetof(tpp_token, TPP_INTERNAL(tt_start))];
		tpp_file   TPP_INTERNAL(tli_file); /* [OVERRIDE(.tf_prev, [owned])]
		                                    * The file that lies at the top of the lexer's #include/macro-stack.
		                                    * this is also the file whose buffer currently contains `tl_tok` */
	} TPP_INTERNAL(tlc_input);
};

typedef struct tpp_lexer {
	union TPP_INTERNAL(tpp_lexer_core) TPP_INTERNAL(tl_core); /* Lexer core */

	/* Lexer state flags */
#if TPP_HAVE_LEXER_STATE_FLAGS
	tpp_lexer_state_flags TPP_INTERNAL(tl_state);
#endif /* TPP_HAVE_LEXER_STATE_FLAGS */


	/* Custom keywords table. */
#if TPP_HAVE_USER_KEYWORDS
	tpp_keywords TPP_INTERNAL(tl_kwds);
#endif /* TPP_HAVE_USER_KEYWORDS */


	/* Lexer extensions. */
#if TPP_HAVE_EXTENSIONS
	tpp_extensions TPP_INTERNAL(tl_exts);
#endif /* TPP_HAVE_EXTENSIONS */


	/* Enabled tokens */
#if TPP_HAVE_FEATURES
	tpp_features TPP_INTERNAL(tl_feat);
#endif /* TPP_HAVE_FEATURES */


	/* system #include paths (/usr/include, ...) */
#if TPP_HAVE_INCLUDE_PATH
	tpp_include_paths TPP_INTERNAL(tl_include_paths);
#endif /* TPP_HAVE_INCLUDE_PATH */


	/* User-overwritable function pointer hooks */
#if TPP_HAVE_HOOKS
	tpp_hooks TPP_INTERNAL(tl_hooks);
#endif /* TPP_HAVE_HOOKS */


	/* Compiler warnings configuration */
#if TPP_HAVE_WARNINGS
	tpp_warnings TPP_INTERNAL(tl_warn);
#endif /* TPP_HAVE_WARNINGS */


	/* Lexer error limits */
#if TPP_HAVE_WARNING_ERROR
	tpp_size TPP_INTERNAL(tl_error_count); /* # of times `TPP_WSTATE_ERROR` was emitted.
	                                        * When this is non-zero by the time your compiler finishes
	                                        * compiling your source file, you should NOT proceed, but
	                                        * propagate an error. */
#define tpp_lexer_geterrorcount(self)    (self)->TPP_INTERNAL(tl_error_count)
#define tpp_lexer_seterrorcount(self, v) (void)((self)->TPP_INTERNAL(tl_error_count) = (v))
#if TPP_ERROR_LIMIT < 0
	tpp_size TPP_INTERNAL(tl_error_limit); /* Once `tl_error_count >= tl_error_limit`, `TPP_WSTATE_ERROR` is treated as `TPP_WSTATE_FATAL` */
#define tpp_lexer_geterrorlimit(self)    ((self)->TPP_INTERNAL(tl_error_limit))
#define tpp_lexer_seterrorlimit(self, v) (void)((self)->TPP_INTERNAL(tl_error_limit) = (v))
#else /* TPP_ERROR_LIMIT < 0 */
#define tpp_lexer_geterrorlimit(self) TPP_ERROR_LIMIT
#endif /* TPP_ERROR_LIMIT >= 0 */
#else /* TPP_HAVE_WARNING_ERROR */
#define tpp_lexer_geterrorcount(self) 0
#endif /* !TPP_HAVE_WARNING_ERROR */


	/* Lexer warning counter */
#if TPP_HAVE_LEXER_WARNING_COUNT
	tpp_size TPP_INTERNAL(tl_warning_count); /* # of times `TPP_WSTATE_WARN` was emitted. */
#define tpp_lexer_getwarningcount(self)    (self)->TPP_INTERNAL(tl_warning_count)
#define tpp_lexer_setwarningcount(self, v) (void)((self)->TPP_INTERNAL(tl_warning_count) = (v))
#endif /* TPP_HAVE_LEXER_WARNING_COUNT */


	/* Lexer inclusion limit */
#if TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED
#if TPP_MAX_INCLUDE_DEPTH < 0
	tpp_size TPP_INTERNAL(tl_inclusion_limit); /* How many times the same file can be `#include`-ed before `TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED` is emitted */
#define tpp_lexer_getinclusionlimit(self)    ((self)->TPP_INTERNAL(tl_inclusion_limit))
#define tpp_lexer_setinclusionlimit(self, v) (void)((self)->TPP_INTERNAL(tl_inclusion_limit) = (v))
#else /* TPP_MAX_INCLUDE_DEPTH < 0 */
#define tpp_lexer_getinclusionlimit(self) TPP_MAX_INCLUDE_DEPTH
#endif /* TPP_MAX_INCLUDE_DEPTH >= 0 */
#elif TPP_MAX_INCLUDE_DEPTH < 0
#define tpp_lexer_getinclusionlimit(self) (-TPP_MAX_INCLUDE_DEPTH)
#else /* ... */
#define tpp_lexer_getinclusionlimit(self) TPP_MAX_INCLUDE_DEPTH
#endif /* !... */


	/* Lexer recursive macro limit */
#if TPP_HAVE_MACRO_RECURSION
#if TPP_MAX_RECURSIVE_MACRO_DEPTH < 0
	tpp_size TPP_INTERNAL(tl_recursive_macro_limit); /* How many times the same recursive macro can exists on the `#include`-stack */
#define tpp_lexer_getrecursivemacrolimit(self)    ((self)->TPP_INTERNAL(tl_recursive_macro_limit))
#define tpp_lexer_setrecursivemacrolimit(self, v) (void)((self)->TPP_INTERNAL(tl_recursive_macro_limit) = (v))
#else /* TPP_MAX_RECURSIVE_MACRO_DEPTH < 0 */
#define tpp_lexer_getrecursivemacrolimit(self) TPP_MAX_RECURSIVE_MACRO_DEPTH
#endif /* TPP_MAX_RECURSIVE_MACRO_DEPTH >= 0 */
#elif TPP_MAX_RECURSIVE_MACRO_DEPTH < 0
#define tpp_lexer_getrecursivemacrolimit(self) (-TPP_MAX_RECURSIVE_MACRO_DEPTH)
#else /* ... */
#define tpp_lexer_getrecursivemacrolimit(self) TPP_MAX_RECURSIVE_MACRO_DEPTH
#endif /* !... */
#if !TPP_IGNORE_INVALID_CONFIGURATION
#if TPP_MAX_RECURSIVE_MACRO_DEPTH != 0 && !TPP_HAVE_MACRO_RECURSION
#error "Invalid configuration: `TPP_MAX_RECURSIVE_MACRO_DEPTH` can only take effect when `TPP_HAVE_MACRO_RECURSION` is enabled"
#elif TPP_MAX_RECURSIVE_MACRO_DEPTH == 1 && TPP_HAVE_MACRO_RECURSION
#error "Invalid configuration: when `TPP_MAX_RECURSIVE_MACRO_DEPTH=1` is hardcoded, `TPP_HAVE_MACRO_RECURSION` being on/off makes no difference"
#endif /* ... */
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */


	/* Next value for `__COUNTER__` */
#if TPP_HAVE_MACRO___COUNTER__
	tpp_counter TPP_INTERNAL(tl_builtin_counter); /* Next value for `__COUNTER__` */
#define tpp_lexer_getnextcounter(self)    ((self)->TPP_INTERNAL(tl_builtin_counter))
#define tpp_lexer_setnextcounter(self, v) (void)((self)->TPP_INTERNAL(tl_builtin_counter) = (v))
#endif /* TPP_HAVE_MACRO___COUNTER__ */


	/* Value for current time (expansion of `__TIME__` can't change between multiple expansions) */
#if TPP_HAVE_LEXER_TIME
	tpp_time TPP_INTERNAL(tl_time); /* Current time, or empty if not yet loaded */
#define tpp_lexer_gettimeptr(self) (&(self)->TPP_INTERNAL(tl_time))
#define tpp_lexer_gettime(self)    (self)->TPP_INTERNAL(tl_time)
#define tpp_lexer_settime(self, v) (void)((self)->TPP_INTERNAL(tl_time) = (v))
#endif /* TPP_HAVE_LEXER_TIME */


	/* Seed for random-number generation (combined with `tpp_lexer_getinputhash()` before use).
	 * Also combined with the final hash of I/O and TEXT files as those files are popped. */
#if TPP_HAVE_LEXER_RAND
	tpp_hash TPP_INTERNAL(tl_rngseed); /* Next RandomNumberGenerationSEED */
#define tpp_lexer_getrngseed(self)    (self)->TPP_INTERNAL(tl_rngseed)
#define tpp_lexer_setrngseed(self, v) (void)((self)->TPP_INTERNAL(tl_rngseed) = (v))
#define tpp_lexer_resetrngseed(self)  (void)((self)->TPP_INTERNAL(tl_rngseed) = TPP_HASH_INITIAL)
#define _tpp_lexer_addrngseed(self, hash) \
	tpp_lexer_setrngseed(self, tpp_hash_combine_hash(tpp_lexer_getrngseed(self), hash))
#define _tpp_lexer_addrngseed_from_file(self, file)                                             \
	(tpp_file_getkind(file) == TPP_FILE_KIND_IO || tpp_file_getkind(file) == TPP_FILE_KIND_TEXT \
	 ? _tpp_lexer_addrngseed(self, tpp_file_gethash(file, (file)->TPP_INTERNAL(tf_end)))        \
	 : (void)0)
#else /* TPP_HAVE_LEXER_RAND */
#define tpp_lexer_resetrngseed(self)                (void)0
#define _tpp_lexer_addrngseed(self, hash)           (void)0
#define _tpp_lexer_addrngseed_from_file(self, file) (void)0
#endif /* !TPP_HAVE_LEXER_RAND */


	/* Format to use for file-and-line prefixes in messages. */
#if TPP_HAVE_RT_FILE_AND_LINE_FORMAT
	char const *TPP_INTERNAL(tl_file_and_line_format); /* [1..1] format for file-and-line prefixes in messages */
#define tpp_lexer_getfileandlineformat(self)         (self)->TPP_INTERNAL(tl_file_and_line_format)
#define tpp_lexer_setfileandlineformat(self, format) (void)((self)->TPP_INTERNAL(tl_file_and_line_format) = (format))
#define tpp_lexer_resetfileandlineformat(self)       (void)((self)->TPP_INTERNAL(tl_file_and_line_format) = TPP_CONFIG_FILE_AND_LINE_FORMAT)
#else /* TPP_HAVE_RT_FILE_AND_LINE_FORMAT */
#define tpp_lexer_getfileandlineformat(self)   TPP_CONFIG_FILE_AND_LINE_FORMAT
#define tpp_lexer_resetfileandlineformat(self) (void)0
#endif /* !TPP_HAVE_RT_FILE_AND_LINE_FORMAT */
} tpp_lexer;

/* Check if a runtime-configurable config option `conf` in `TPP_HAVE_<conf>` is currently enabled.
 * When `TPP_HAVE_<conf>` is configured as `TPP_CONF_ISCONST()`, return that constant instead. */
#define tpp_lexer_has(self, conf) _tpp_lexer_has_##conf(self)

/* Current token */
#define tpp_lexer_gettok(self)          ((self)->TPP_INTERNAL(tl_core).TPP_INTERNAL(tlc_tok).TPP_INTERNAL(tt_id))
#define tpp_lexer_gettoken(self)        (&(self)->TPP_INTERNAL(tl_core).TPP_INTERNAL(tlc_tok))
#define tpp_lexer_hastokenkwd(self)     tpp_token_haskwd(tpp_lexer_gettoken(self))
#define tpp_lexer_gettokenkwd(self)     tpp_token_getkwd(tpp_lexer_gettoken(self))
#define tpp_lexer_gettokenkwdcstr(self) tpp_token_getkwdcstr(tpp_lexer_gettoken(self))
#define tpp_lexer_gettokenstart(self)   tpp_token_getstart(tpp_lexer_gettoken(self))
#define tpp_lexer_gettokenend(self)     tpp_token_getend(tpp_lexer_gettoken(self))
#define tpp_lexer_gettokenlen(self)     tpp_token_getlen(tpp_lexer_gettoken(self))

/* Current file */
#define tpp_lexer_getfile(self)     (&(self)->TPP_INTERNAL(tl_core).TPP_INTERNAL(tlc_input).TPP_INTERNAL(tli_file))
#define tpp_lexer_getfilekind(self) tpp_file_getkind(tpp_lexer_getfile(self))
#define tpp_lexer_getlcfile(self)   tpp_file_getlcfile(tpp_lexer_getfile(self))   /* [1..1] Returns the file that is used to determine `__LINE__` and `__COLUMN__` */
#define tpp_lexer_getbasefile(self) tpp_file_getbasefile(tpp_lexer_getfile(self)) /* [1..1] Return the *base* file (that is: the last one in the `#include`-stack) */
#define tpp_lexer_gettextfile(self) tpp_file_gettextfile(tpp_lexer_getfile(self)) /* [0..1] Return the last I/O or TEXT file */

/* L/C information helpers */
#define tpp_lexer_getlcinfoat(self, pos)                 tpp_file_getlcinfo(tpp_lexer_getfile(self), pos)
#define tpp_lexer_getlcinfoat_ex(self, pos, result)      tpp_file_getlcinfo_ex(tpp_lexer_getfile(self), pos, result)
#define tpp_lexer_getlcinfoattokenstart(self)            tpp_lexer_getlcinfoat(self, tpp_lexer_gettokenstart(self))
#define tpp_lexer_getlcinfoattokenstart_ex(self, result) tpp_lexer_getlcinfoat_ex(self, tpp_lexer_gettokenstart(self), result)
#define tpp_lexer_getlcinfoattokenend(self)              tpp_lexer_getlcinfoat(self, tpp_lexer_gettokenend(self))
#define tpp_lexer_getlcinfoattokenend_ex(self, result)   tpp_lexer_getlcinfoat_ex(self, tpp_lexer_gettokenend(self), result)

/* Lexer input hash API */
#if TPP_HAVE_FILE_GETFULLHASH
#define tpp_lexer_getinputhashat(self, pos)      tpp_file_getfullhash(tpp_lexer_getfile(self), pos)
#define tpp_lexer_getinputhashattokenstart(self) tpp_lexer_getinputhashat(self, tpp_lexer_gettokenstart(self))
#define tpp_lexer_getinputhashattokenend(self)   tpp_lexer_getinputhashat(self, tpp_lexer_gettokenend(self))
#define tpp_lexer_getinputhash(self)             tpp_lexer_getinputhashattokenstart(self)
#endif /* TPP_HAVE_FILE_GETFULLHASH */

/* Convenience L/C information helpers.
 * If you don't want to bother learning what all the above does, then it's these that
 * you want to use -- these return the values as reported by __LINE__ and __COLUMN__. */
#define tpp_lexer_getlcfilename(self)  tpp_file_getfilename(tpp_lexer_getlcfile(self))    /* [0..1] Value of `__FILE__` */
#define tpp_lexer_getstartlcinfo(self) tpp_file_getstartlcinfo(tpp_lexer_getlcfile(self)) /* Value of `__LINE__` / `__COLUMN__` */
#define tpp_lexer_getendlcinfo(self)   tpp_file_getendlcinfo(tpp_lexer_getlcfile(self))   /* Theoretical value of `__LINE__` / `__COLUMN__` at end of current token */
#define tpp_lexer_getlcinfo(self)      tpp_lexer_getstartlcinfo(self)                     /* Convenience alias to make it clear what you want to use */

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
#define tpp_lexer_resetwarnings(self)                     tpp_warnings_reset(&(self)->TPP_INTERNAL(tl_warn))
#define tpp_lexer_getwarninggrp(self, grp_id)             tpp_lexer_getwarningctx(self, tpp_warning_context_id_ofgroup(grp_id))
#define tpp_lexer_setwarninggrp(self, grp_id, state)      tpp_lexer_setwarningctx(self, tpp_warning_context_id_ofgroup(grp_id), state)
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
#define tpp_lexer_resetextensions(self) tpp_extensions_reset(&(self)->TPP_INTERNAL(tl_exts))
#else /* TPP_HAVE_EXTENSIONS */
#define tpp_lexer_getextension(self, TPP_EXT_x) 0
#define tpp_lexer_resetextensions(self)         (void)0
#endif /* !TPP_HAVE_EXTENSIONS */

/* Features... */
#if TPP_HAVE_FEATURES
#define tpp_lexer_getfeature(self, TPP_FEAT_x)          tpp_features_getid(&(self)->TPP_INTERNAL(tl_feat), TPP_FEAT_x)
#define tpp_lexer_setfeature(self, TPP_FEAT_x, enabled) tpp_features_setid(&(self)->TPP_INTERNAL(tl_feat), TPP_FEAT_x, enabled)
#define tpp_lexer_enablefeature(self, TPP_FEAT_x)       tpp_features_enable(&(self)->TPP_INTERNAL(tl_feat), TPP_FEAT_x)
#define tpp_lexer_disablefeature(self, TPP_FEAT_x)      tpp_features_disable(&(self)->TPP_INTERNAL(tl_feat), TPP_FEAT_x)
#define tpp_lexer_resetfeatures(self)                   tpp_features_reset(&(self)->TPP_INTERNAL(tl_feat))
#else /* TPP_HAVE_FEATURES */
#define tpp_lexer_getfeature(self, TPP_FEAT_x) 0
#define tpp_lexer_resetfeatures(self)          (void)0
#endif /* !TPP_HAVE_FEATURES */

/* Check if `tpp_lexer_yieldpp()` might parse directives right now.
 * Since directives are only allowed to appear directly following a
 * line-feed within the same file, or at the start of some file, this
 * function allows you to check if the lexer is in a scenario where
 * it allows directives to be parsed. */
#define tpp_lexer_yieldpp_getallowdirectives(self) \
	tpp_file_getallowdirectives(tpp_lexer_getfile(self))



/* Include path... */
#if TPP_HAVE_INCLUDE_PATH
#define tpp_lexer_includes_addbykind(self, kind, path, path_maxlen)      tpp_include_paths_addbykind(&(self)->TPP_INTERNAL(tl_include_paths), kind, path, path_maxlen)
#define tpp_lexer_includes_addbykind_head(self, kind, path, path_maxlen) tpp_include_paths_addbykind_head(&(self)->TPP_INTERNAL(tl_include_paths), kind, path, path_maxlen)
#define tpp_lexer_includes_delbykind(self, kind, path, path_maxlen)      tpp_include_paths_delbykind(&(self)->TPP_INTERNAL(tl_include_paths), kind, path, path_maxlen)
#define tpp_lexer_includes_clearbykind(self, kind)                       tpp_include_paths_clearbykind(&(self)->TPP_INTERNAL(tl_include_paths), kind)
#define tpp_lexer_includes_getbykind(self, kind, i)                      tpp_include_paths_getbykind(&(self)->TPP_INTERNAL(tl_include_paths), kind, i)
#define tpp_lexer_includes_numbykind(self, kind)                         tpp_include_paths_numbykind(&(self)->TPP_INTERNAL(tl_include_paths), kind)

#define tpp_lexer_includes_addsystem(self, path, path_maxlen)      tpp_include_paths_addsystem(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_addsystem_head(self, path, path_maxlen) tpp_include_paths_addsystem_head(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_delsystem(self, path, path_maxlen)      tpp_include_paths_delsystem(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_clearsystem(self)                       tpp_include_paths_clearsystem(&(self)->TPP_INTERNAL(tl_include_paths))
#define tpp_lexer_includes_getsystem(self, i)                      tpp_include_paths_getsystem(&(self)->TPP_INTERNAL(tl_include_paths), i)
#define tpp_lexer_includes_numsystem(self)                         tpp_include_paths_numsystem(&(self)->TPP_INTERNAL(tl_include_paths))
#if TPP_HAVE_INCLUDE_PATH_QUOTE
#define tpp_lexer_includes_addquote(self, path, path_maxlen)      tpp_include_paths_addquote(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_addquote_head(self, path, path_maxlen) tpp_include_paths_addquote_head(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_delquote(self, path, path_maxlen)      tpp_include_paths_delquote(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_clearquote(self)                       tpp_include_paths_clearquote(&(self)->TPP_INTERNAL(tl_include_paths))
#define tpp_lexer_includes_getquote(self, i)                      tpp_include_paths_getquote(&(self)->TPP_INTERNAL(tl_include_paths), i)
#define tpp_lexer_includes_numquote(self)                         tpp_include_paths_numquote(&(self)->TPP_INTERNAL(tl_include_paths))
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
#define tpp_lexer_includes_addsyshdr(self, path, path_maxlen)      tpp_include_paths_addsyshdr(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_addsyshdr_head(self, path, path_maxlen) tpp_include_paths_addsyshdr_head(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_delsyshdr(self, path, path_maxlen)      tpp_include_paths_delsyshdr(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_clearsyshdr(self)                       tpp_include_paths_clearsyshdr(&(self)->TPP_INTERNAL(tl_include_paths))
#define tpp_lexer_includes_getsyshdr(self, i)                      tpp_include_paths_getsyshdr(&(self)->TPP_INTERNAL(tl_include_paths), i)
#define tpp_lexer_includes_numsyshdr(self)                         tpp_include_paths_numsyshdr(&(self)->TPP_INTERNAL(tl_include_paths))
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#if TPP_HAVE_INCLUDE_PATH_AFTER
#define tpp_lexer_includes_addafter(self, path, path_maxlen)      tpp_include_paths_addafter(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_addafter_head(self, path, path_maxlen) tpp_include_paths_addafter_head(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_delafter(self, path, path_maxlen)      tpp_include_paths_delafter(&(self)->TPP_INTERNAL(tl_include_paths), path, path_maxlen)
#define tpp_lexer_includes_clearafter(self)                       tpp_include_paths_clearafter(&(self)->TPP_INTERNAL(tl_include_paths))
#define tpp_lexer_includes_getafter(self, i)                      tpp_include_paths_getafter(&(self)->TPP_INTERNAL(tl_include_paths), i)
#define tpp_lexer_includes_numafter(self)                         tpp_include_paths_numafter(&(self)->TPP_INTERNAL(tl_include_paths))
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */
#if TPP_HAVE_INCLUDE_PATH_PUSH_POP
#define tpp_lexer_pushincludes(self)   tpp_include_paths_push(&(self)->TPP_INTERNAL(tl_include_paths))
#define tpp_lexer_popincludes(self)    tpp_include_paths_pop(&(self)->TPP_INTERNAL(tl_include_paths))
#define tpp_lexer_canpopincludes(self) tpp_include_paths_canpop(&(self)->TPP_INTERNAL(tl_include_paths))
#endif /* TPP_HAVE_INCLUDE_PATH_PUSH_POP */
#define tpp_lexer_resetincludes(self)  tpp_include_paths_reset(&(self)->TPP_INTERNAL(tl_include_paths))
#endif /* TPP_HAVE_INCLUDE_PATH */



/* Wrappers for keywords API */
#if TPP_HAVE_USER_KEYWORDS
#define _tpp_lexer_kwds_getkeyword(self, kwd, len, hash) _tpp_keywords_getkeyword(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash)
#define _tpp_lexer_kwds_getkeyword_byid(self, id)        _tpp_keywords_getkeyword_byid(&(self)->TPP_INTERNAL(tl_kwds), id)
#define tpp_lexer_kwds_getkeyword(self, kwd, len, hash)  tpp_keywords_getkeyword(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash)
#define tpp_lexer_kwds_getkeyword_byid(self, id)         tpp_keywords_getkeyword_byid(&(self)->TPP_INTERNAL(tl_kwds), id)
#define tpp_lexer_kwds_newkeyword(self, kwd, len, hash)  tpp_keywords_newkeyword(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash)
#if TPP_HAVE_ESCAPED_KEYWORDS
#define _tpp_lexer_kwds_getkeyword_esc(self, kwd, len, hash) _tpp_keywords_getkeyword_esc(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash, self)
#define tpp_lexer_kwds_getkeyword_esc(self, kwd, len, hash) tpp_keywords_getkeyword_esc(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash, self)
#define tpp_lexer_kwds_newkeyword_esc(self, kwd, len, hash) tpp_keywords_newkeyword_esc(&(self)->TPP_INTERNAL(tl_kwds), kwd, len, hash, self)
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */
#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
#define tpp_lexer_kwds_copybuiltin(self, kwd) tpp_keywords_copybuiltin(&(self)->TPP_INTERNAL(tl_kwds), kwd)
#endif /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */

#if TPP_HAVE_KEYWORDS_RESETFLAGS
/* Modify the flags of all keywords as `flags = flags & keep_mask` */
#define tpp_lexer_kwds_resetflags(self, keep_mask) tpp_keywords_resetflags(&(self)->TPP_INTERNAL(tl_kwds), keep_mask)
#endif /* TPP_HAVE_KEYWORDS_RESETFLAGS */

#if TPP_HAVE_KEYWORDS_RESETFEATURES
/* Reset all uses of `tpp_keyword_setfeature()` */
#define tpp_lexer_kwds_resetfeatures(self) tpp_keywords_resetfeatures(&(self)->TPP_INTERNAL(tl_kwds))
#endif /* TPP_HAVE_KEYWORDS_RESETFEATURES */

#if TPP_HAVE_KEYWORDS_RESETCOUNTERS
/* Call `tpp_keyword_reset_builtin_counter()` on every keyword, thereby
 * resetting all side-effects of expansions of `__TPP_COUNTER` thus far. */
#define tpp_lexer_kwds_resetcounters(self) tpp_keywords_resetcounters(&(self)->TPP_INTERNAL(tl_kwds))
#endif /* TPP_HAVE_KEYWORDS_RESETCOUNTERS */

/* Reset (re-initialize) all user-defined keywords
 * WARNING: To use this function, you should first finalize the `#include`-stack (i.e.:
 *          call `tpp_lexer_finifile()`), since the `#include`-stack usually contains
 *          references to certain keywords that will become dangling after a call to
 *          this function */
#define tpp_lexer_kwds_reset(self) tpp_keywords_reset(&(self)->TPP_INTERNAL(tl_kwds))
#else /* TPP_HAVE_USER_KEYWORDS */
#define tpp_lexer_kwds_getkeyword(self, kwd, len, hash)  tpp_builtin_getkeyword(kwd, len, hash)
#define tpp_lexer_kwds_getkeyword_byid(self, id)         tpp_builtin_getkeyword_byid(id)
#if TPP_HAVE_ESCAPED_KEYWORDS
#define tpp_lexer_kwds_getkeyword_esc(self, kwd, len, hash, file) tpp_builtin_getkeyword_esc(kwd, len, hash, file)
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */
#define tpp_lexer_kwds_reset(self) (void)0
#endif /* !TPP_HAVE_USER_KEYWORDS */


/* Invocation of hooks */
/*[[[deemon
import HOOKS, unPointerizeHook from .config;

for (local doc, name,
     default_TPP_HAVE_FOO_HOOK,
     builtin_FOO_HOOK,
     prototypePrefix,
     prototypeSuffix,
     prototypeArgs,
     disabled_RETURN_VALUE: HOOKS) {
	local isFormatPrinter = prototypePrefix.strip() == "tpp_formatprinter" && !prototypeSuffix;
	local hookMustBeFunctionPointer = isFormatPrinter;
	prototypePrefix, prototypeSuffix = unPointerizeHook(prototypePrefix, prototypeSuffix)...;
	print("/" "* >> ", prototypePrefix, "tpp_lexer_callhook_", name.lower(), prototypeSuffix, ";");
	print(" * ", doc.strip().replace("\n", "\n * ").rstriplines(), " *" "/");
	print("#define tpp_lexer_callhook_", name.lower(), "(self",
		"".join(for (local x: prototypeArgs)
			if (x !in ["lexer", "cookie"]) f", {x}"), ") \\");
	print("	tpp_hooks_call_", name.lower(), "(&(self)->TPP_INTERNAL(tl_hooks), self",
		"".join(for (local x: prototypeArgs) if (x !in ["cookie", "lexer"]) f", {x}"), ")");
	if (hookMustBeFunctionPointer) {
		print("#ifdef tpp_hooks_get_", name.lower());
		if ("cookie" in prototypeArgs) {
			print("#define tpp_lexer_gethookcookie_", name.lower(), "(self) tpp_hooks_getcookie_", name.lower(), "(&(self)->TPP_INTERNAL(tl_hooks), self)");
			print("#define tpp_lexer_gethook_", name.lower(), "(self)       tpp_hooks_get_", name.lower(), "(&(self)->TPP_INTERNAL(tl_hooks))");
		} else {
			print("#define tpp_lexer_gethook_", name.lower(), "(self) tpp_hooks_get_", name.lower(), "(&(self)->TPP_INTERNAL(tl_hooks))");
		}
		print("#endif /" "* tpp_hooks_get_", name.lower(), " *" "/");
	}
	print("#if TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK)");
	if (!hookMustBeFunctionPointer) {
		if ("cookie" in prototypeArgs)
			print("#define tpp_lexer_gethookcookie_", name.lower(), "(self) tpp_hooks_getcookie_", name.lower(), "(&(self)->TPP_INTERNAL(tl_hooks), self)");
		print("#define tpp_lexer_gethook_", name.lower(), "(self)    tpp_hooks_get_", name.lower(), "(&(self)->TPP_INTERNAL(tl_hooks))");
	}
	print("#define tpp_lexer_resethook_", name.lower(), "(self)  tpp_hooks_reset_", name.lower(), "(&(self)->TPP_INTERNAL(tl_hooks), self)");
	print("#define tpp_lexer_sethook_", name.lower(), "(self, v) tpp_hooks_set_", name.lower(), "(&(self)->TPP_INTERNAL(tl_hooks), self, v)");
	if ("cookie" in prototypeArgs) {
		print("#if TPP_HAVE_HOOK_COOKIES");
		print("#define tpp_lexer_sethook_", name.lower(), "_ex(self, v, cookie) tpp_hooks_set_", name.lower(), "_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)");
		print("#endif /" "* TPP_HAVE_HOOK_COOKIES *" "/");
	}
	print("#endif /" "* TPP_HOOK_ISRT(TPP_HAVE_", name, "_HOOK) *" "/");
	print;
}
]]]*/
/* >> TPP_FORMATPRINTER_DEFINE(tpp_lexer_callhook_warnprinter, arg, text, num_bytes);
 * Called by `tpp_lexer_warnf()` to print warning messages.
 * Potentially unused if `TPP_HAVE_WARNHANDLER_HOOK` is also overwritten
 * @return: >= 0: Success
 * @return: TPP_SSIZE_OFERR(TPP_EIO):       I/O Error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): Hard lexer error
 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error */
#define tpp_lexer_callhook_warnprinter(self, text, num_bytes) \
	tpp_hooks_call_warnprinter(&(self)->TPP_INTERNAL(tl_hooks), self, text, num_bytes)
#ifdef tpp_hooks_get_warnprinter
#define tpp_lexer_gethookcookie_warnprinter(self) tpp_hooks_getcookie_warnprinter(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_warnprinter(self)       tpp_hooks_get_warnprinter(&(self)->TPP_INTERNAL(tl_hooks))
#endif /* tpp_hooks_get_warnprinter */
#if TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK)
#define tpp_lexer_resethook_warnprinter(self)  tpp_hooks_reset_warnprinter(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_warnprinter(self, v) tpp_hooks_set_warnprinter(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_warnprinter_ex(self, v, cookie) tpp_hooks_set_warnprinter_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_WARNPRINTER_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_warnhandler(tpp_hook_cookie cookie, tpp_lexer_printf_info *tpp_restrict info, tpp_warning_invokeinfo const *tpp_restrict invokeinfo, tpp_warning_id id, va_list args);
 * Called by `tpp_lexer_warnf()` to handle warning notifications. Can be
 * overwritten to implement custom behavior in regards to handling of warnings.
 * @param: info:       Warning context location
 * @param: invokeinfo: Warning invocation method
 * @param: id:         Warning ID
 * @param: args:       Variable arguments passed to warning
 * @return: TPP_EOK:       Success (warning was emitted)
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_ELEXERROR: Hard lexer error
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_warnhandler(self, info, invokeinfo, id, args) \
	tpp_hooks_call_warnhandler(&(self)->TPP_INTERNAL(tl_hooks), self, info, invokeinfo, id, args)
#if TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK)
#define tpp_lexer_gethookcookie_warnhandler(self) tpp_hooks_getcookie_warnhandler(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_warnhandler(self)    tpp_hooks_get_warnhandler(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_warnhandler(self)  tpp_hooks_reset_warnhandler(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_warnhandler(self, v) tpp_hooks_set_warnhandler(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_warnhandler_ex(self, v, cookie) tpp_hooks_set_warnhandler_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_WARNHANDLER_HOOK) */

/* >> TPP_FORMATPRINTER_DEFINE(tpp_lexer_callhook_mesgprinter, arg, text, num_bytes);
 * Used by `#pragma message` to print messages (see `TPP_HAVE_PRAGMA_MESSAGE`)
 * @return: >= 0: Success
 * @return: TPP_SSIZE_OFERR(TPP_EIO):       I/O Error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): Hard lexer error
 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error */
#define tpp_lexer_callhook_mesgprinter(self, text, num_bytes) \
	tpp_hooks_call_mesgprinter(&(self)->TPP_INTERNAL(tl_hooks), self, text, num_bytes)
#ifdef tpp_hooks_get_mesgprinter
#define tpp_lexer_gethookcookie_mesgprinter(self) tpp_hooks_getcookie_mesgprinter(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_mesgprinter(self)       tpp_hooks_get_mesgprinter(&(self)->TPP_INTERNAL(tl_hooks))
#endif /* tpp_hooks_get_mesgprinter */
#if TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK)
#define tpp_lexer_resethook_mesgprinter(self)  tpp_hooks_reset_mesgprinter(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_mesgprinter(self, v) tpp_hooks_set_mesgprinter(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_mesgprinter_ex(self, v, cookie) tpp_hooks_set_mesgprinter_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_MESGPRINTER_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_parseexpr(tpp_hook_cookie cookie, tpp_expr_value *tpp_restrict result);
 * User-defined callback for parsing `#if`-style expressions
 * - This callback is invoked in a context where the lexer
 *   points before the expression's first token (meaning that
 *   this callback is responsible to do the initial yield
 *   using whatever method it wants to use).
 * - When it is known that the expression has finite length,
 *   as in: it has to end before EOF, or at the next unmatched
 *   `)`-token, the caller will have configured the lexer's
 *   current EOF accordingly (and disabled file-popping)
 * - When this function returns an error, the caller will rewind
 *   back to the start of the expression (or even further, if
 *   applicable; meaning this callback doesn't need to concern
 *   itself with rollback)
 *
 * @return: TPP_EOK:         Success (`*result` was initialized)
 * @return: TPP_ENOMEM:      Out of memory
 * @return: TPP_EIO:         Filesystem I/O operation failed
 * @return: TPP_EWOULDBLOCK: Operation would block
 * @return: TPP_ELEXERROR:   A lexer error happened
 * @return: TPP_EUSER(*):    User-defined error */
#define tpp_lexer_callhook_parseexpr(self, result) \
	tpp_hooks_call_parseexpr(&(self)->TPP_INTERNAL(tl_hooks), self, result)
#if TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK)
#define tpp_lexer_gethookcookie_parseexpr(self) tpp_hooks_getcookie_parseexpr(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_parseexpr(self)    tpp_hooks_get_parseexpr(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_parseexpr(self)  tpp_hooks_reset_parseexpr(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_parseexpr(self, v) tpp_hooks_set_parseexpr(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_parseexpr_ex(self, v, cookie) tpp_hooks_set_parseexpr_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_PARSEEXPR_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_unknown_pragma(tpp_hook_cookie cookie);
 * Called whenever a `#pragma` is encountered that is not recognized.
 * When called, the lexer is set-up to point at the first token after the `#pragma`.
 * @return: TPP_EOK:      Pragma has been handled
 * @return: TPP_ENOENT:   Pragma is still unknown, and a warning should be emitted
 * @return: TPP_EIO:      I/O error
 * @return: TPP_ENOMEM:   Out of memory
 * @return: TPP_EUSER(*): User-defined error */
#define tpp_lexer_callhook_unknown_pragma(self) \
	tpp_hooks_call_unknown_pragma(&(self)->TPP_INTERNAL(tl_hooks), self)
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK)
#define tpp_lexer_gethookcookie_unknown_pragma(self) tpp_hooks_getcookie_unknown_pragma(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_unknown_pragma(self)    tpp_hooks_get_unknown_pragma(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_unknown_pragma(self)  tpp_hooks_reset_unknown_pragma(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_unknown_pragma(self, v) tpp_hooks_set_unknown_pragma(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_unknown_pragma_ex(self, v, cookie) tpp_hooks_set_unknown_pragma_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_new_dependency(tpp_hook_cookie cookie, tpp_keyword *filename_kwd);
 * Called whenever some file is `#include`-ed for the first time
 * @param: filename_kwd: Then `tpp_keyword` used to describe the file's name. The actual
 *                       filename can be queried as `tpp_keyword_getcstr(filename_kwd)`
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_new_dependency(self, filename_kwd) \
	tpp_hooks_call_new_dependency(&(self)->TPP_INTERNAL(tl_hooks), self, filename_kwd)
#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
#define tpp_lexer_gethookcookie_new_dependency(self) tpp_hooks_getcookie_new_dependency(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_new_dependency(self)    tpp_hooks_get_new_dependency(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_new_dependency(self)  tpp_hooks_reset_new_dependency(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_new_dependency(self, v) tpp_hooks_set_new_dependency(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_new_dependency_ex(self, v, cookie) tpp_hooks_set_new_dependency_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_file_pushed(tpp_hook_cookie cookie);
 * Called whenever a file was just pushed onto the `#include`-stack. Information
 * about the just-pushed file can be retrieved by examining `tpp_lexer_getfile(LEXER)`.
 *
 * Notes:
 * - This hook can be used by a frontend to implement stuff like GCC's `--trace-includes`.
 * - This hook is *NOT* called for `tpp_file_subtext_push()` or `tpp_file_pushdummy()`
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_file_pushed(self) \
	tpp_hooks_call_file_pushed(&(self)->TPP_INTERNAL(tl_hooks), self)
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK)
#define tpp_lexer_gethookcookie_file_pushed(self) tpp_hooks_getcookie_file_pushed(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_file_pushed(self)    tpp_hooks_get_file_pushed(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_file_pushed(self)  tpp_hooks_reset_file_pushed(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_file_pushed(self, v) tpp_hooks_set_file_pushed(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_file_pushed_ex(self, v, cookie) tpp_hooks_set_file_pushed_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_FILE_PUSHED_HOOK) */

/* >> void tpp_lexer_callhook_file_popped(tpp_hook_cookie cookie);
 * Called whenever a file is about to be popped off the `#include`-stack.
 * Information about the file that's about-to-be popped can be retrieved
 * by examining `tpp_lexer_getfile(LEXER)`.
 *
 * Notes:
 * - When files are popped by `tpp_lexer_manualpopfile_popfile()` within a
 *   `tpp_lexer_manualpopfile_start()`-region, this hook is called during the
 *   *commit* phase (i.e.: by `tpp_lexer_manualpopfile_break_commit()`),
 *   rather than `tpp_lexer_manualpopfile_popfile()` as one might suspect at first.
 * - This hook is *NOT* called by `tpp_file_subtext_pop()` or `tpp_file_popdummy()` */
#define tpp_lexer_callhook_file_popped(self) \
	tpp_hooks_call_file_popped(&(self)->TPP_INTERNAL(tl_hooks), self)
#if TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK)
#define tpp_lexer_gethookcookie_file_popped(self) tpp_hooks_getcookie_file_popped(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_file_popped(self)    tpp_hooks_get_file_popped(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_file_popped(self)  tpp_hooks_reset_file_popped(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_file_popped(self, v) tpp_hooks_set_file_popped(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_file_popped_ex(self, v, cookie) tpp_hooks_set_file_popped_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_FILE_POPPED_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_include_encountered(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind);
 * Called when a `#include` (or `#include_next`, `#import` or `#embed`)-directive
 * is encountered, at the point in time when the lexer's current token has already
 * been populated by `tpp_lexer_yieldraw_at_include_string_blocking()` (and macros
 * were also already expanded), and the current token is `TPP_TOK_INCPATH_DQUOTE` or
 * `TPP_TOK_INCPATH_LANGLE`.
 *
 * This hook is primarily here for the purpose of implementing GCC's `-dI` switch, but
 * it could also be used for other purposes, such as intentionally skipping certain
 * `#include`-directives.
 *
 * To gain access to the `#include`-string, you must use `tpp_lexer_decode_include_string_cb()`
 *
 * @param: include_kind: The kind of directive that this is (one of `TPP_HOOK_INCLUDE_KIND_*`)
 * @return: TPP_EOK:       Continue handling like usual
 * @return: TPP_ENOENT:    Don't attempt to find/open a file. Instead, continue processing
 *                         the file containing the `#include`-directive as though the file
 *                         could not be found, and the `TPP_W_NO_SUCH_FILE` error was being
 *                         suppressed.
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_include_encountered(self, include_kind) \
	tpp_hooks_call_include_encountered(&(self)->TPP_INTERNAL(tl_hooks), self, include_kind)
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK)
#define tpp_lexer_gethookcookie_include_encountered(self) tpp_hooks_getcookie_include_encountered(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_include_encountered(self)    tpp_hooks_get_include_encountered(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_include_encountered(self)  tpp_hooks_reset_include_encountered(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_include_encountered(self, v) tpp_hooks_set_include_encountered(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_include_encountered_ex(self, v, cookie) tpp_hooks_set_include_encountered_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_include_not_found(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind);
 * Called when the file specified by a `#include` (or `#include_next`, `#import` or
 * `#embed`)-directive could not be found. This hook may be used to either suppress
 * the error (by returning something other than `TPP_ENOENT`), or log the error to
 * implement something like GCC's `-MG` commandline switch.
 *
 * This hook is called just before `TPP_W_NO_SUCH_FILE` would be emitted, with the lexer's
 * current token still being the `<stdio.h>` or `"file.h"` string, meaning if you want
 * to know what that string says, you can use `tpp_lexer_decode_include_string_cb()` to decode it.
 *
 * @param: include_kind: The kind of directive that this is (one of `TPP_HOOK_INCLUDE_KIND_*`)
 * @return: TPP_EOK:       Suppress the accompanying `TPP_W_NO_SUCH_FILE` error, but continue acting like
 *                         the file could not be found (*DONT* use this hook to manually push a file or
 *                         something like that)
 * @return: TPP_ENOENT:    Emit the `TPP_W_NO_SUCH_FILE` error
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_include_not_found(self, include_kind) \
	tpp_hooks_call_include_not_found(&(self)->TPP_INTERNAL(tl_hooks), self, include_kind)
#if TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
#define tpp_lexer_gethookcookie_include_not_found(self) tpp_hooks_getcookie_include_not_found(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_include_not_found(self)    tpp_hooks_get_include_not_found(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_include_not_found(self)  tpp_hooks_reset_include_not_found(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_include_not_found(self, v) tpp_hooks_set_include_not_found(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_include_not_found_ex(self, v, cookie) tpp_hooks_set_include_not_found_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_macro_defined(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name, tpp_macro *tpp_restrict macro);
 * Called whenever a `#define` directive has just been fully
 * parsed (macro was has not yet been registered with keyword).
 *
 * - This hook is *ONLY* invoked when `#define` is encountered, or
 *   `#pragma pop_macro("foo")` was used to restore a macro's previous
 *   definition.
 * - Calls to `tpp_lexer_define()` or other related functions will
 *   *NOT* invoke this hook.
 *
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_macro_defined(self, name, macro) \
	tpp_hooks_call_macro_defined(&(self)->TPP_INTERNAL(tl_hooks), self, name, macro)
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK)
#define tpp_lexer_gethookcookie_macro_defined(self) tpp_hooks_getcookie_macro_defined(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_macro_defined(self)    tpp_hooks_get_macro_defined(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_macro_defined(self)  tpp_hooks_reset_macro_defined(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_macro_defined(self, v) tpp_hooks_set_macro_defined(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_macro_defined_ex(self, v, cookie) tpp_hooks_set_macro_defined_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_DEFINED_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_macro_undefined(tpp_hook_cookie cookie, tpp_keyword *tpp_restrict name);
 * Called whenever a `#undef` directive has just been fully
 * parsed (macro hasn't been deleted from keyword, yet). Note
 * that this hook is still called, even if the keyword doesn't
 * have a macro (and might have even already been marked as having
 * no predefined definition: `_TPP_KEYWORD_MACRO_UNDEFINED`). This
 * hook is imply called as part of the process of evaluating `#undef`
 *
 * This hook is *ONLY* invoked when `#undef` is encountered.
 * Calls to `tpp_lexer_undef()`, `tpp_keyword_undef()`, or other
 * related functions will *NOT* invoke this hook.
 *
 * NOTE: this hook *will* actually also be called by `#pragma push_macro(undef, "foo")`
 *
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_macro_undefined(self, name) \
	tpp_hooks_call_macro_undefined(&(self)->TPP_INTERNAL(tl_hooks), self, name)
#if TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK)
#define tpp_lexer_gethookcookie_macro_undefined(self) tpp_hooks_getcookie_macro_undefined(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_macro_undefined(self)    tpp_hooks_get_macro_undefined(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_macro_undefined(self)  tpp_hooks_reset_macro_undefined(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_macro_undefined(self, v) tpp_hooks_set_macro_undefined(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_macro_undefined_ex(self, v, cookie) tpp_hooks_set_macro_undefined_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_MACRO_UNDEFINED_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_ident_sccs(tpp_hook_cookie cookie, tpp_token_id mode, tpp_string *chunk, tpp_char const *comment_str, tpp_size comment_len);
 * Called to handle `#ident` and `#sccs` directives
 * @param: mode:        Either `TPP_KWD_ident` or `TPP_KWD_sccs`
 * @param: chunk:       If non-NULL a string that must be `tpp_string_incref()`d
 *                      if you want to keep `comment_str` alive. If NULL, then the
 *                      given `comment_str` is statically allocated and doesn't need
 *                      any chunk to stay alive
 * @param: comment_str: The source comment that should be inserted
 * @param: comment_len: Length of `comment_str` in bytes
 * @return: TPP_EOK:       Success (keep going)
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_ident_sccs(self, mode, chunk, comment_str, comment_len) \
	tpp_hooks_call_ident_sccs(&(self)->TPP_INTERNAL(tl_hooks), self, mode, chunk, comment_str, comment_len)
#if TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK)
#define tpp_lexer_gethookcookie_ident_sccs(self) tpp_hooks_getcookie_ident_sccs(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_ident_sccs(self)    tpp_hooks_get_ident_sccs(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_ident_sccs(self)  tpp_hooks_reset_ident_sccs(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_ident_sccs(self, v) tpp_hooks_set_ident_sccs(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_ident_sccs_ex(self, v, cookie) tpp_hooks_set_ident_sccs_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_IDENT_SCCS_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_system_include_path(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_include_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to tpp_lexer_foreach_include_path_flags__PARAM), void *arg);
 * Extra callback invoked by `tpp_lexer_foreach_include_path()` at different
 * points during the process of enumerating include paths. This callback is
 * then allowed to enumerate some additional include paths that may exist, but
 * for one reason or another (mainly: speed) aren't known to TPP via its system
 * include path APIs (`tpp_lexer_includes_add*`)
 * @param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`, describing the
 *               caller's position in `tpp_lexer_foreach_include_path()`.
 * @return: * :            First non-TPP_ENOENT return value of `cb`
 * @return: TPP_ENOENT:    File still not found
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_system_include_path(self, mode, when, cb, arg) \
	tpp_hooks_call_system_include_path(&(self)->TPP_INTERNAL(tl_hooks), self, mode, when, cb, arg)
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK)
#define tpp_lexer_gethookcookie_system_include_path(self) tpp_hooks_getcookie_system_include_path(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_system_include_path(self)    tpp_hooks_get_system_include_path(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_system_include_path(self)  tpp_hooks_reset_system_include_path(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_system_include_path(self, v) tpp_hooks_set_system_include_path(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_system_include_path_ex(self, v, cookie) tpp_hooks_set_system_include_path_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_system_embed_path(tpp_hook_cookie cookie, tpp_token_id mode, tpp_hook_system_embed_path_when when, tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to), void *arg);
 * Extra callback invoked by `tpp_lexer_foreach_embed_path()` at different points
 * during the process of enumerating embed paths. (s.a. `TPP_HAVE_SYSTEM_INCLUDE_PATH_HOOK`)
 * @param: when: One of `TPP_HOOK_SYSTEM_INCLUDE_PATH_WHEN_*`, describing the
 *               caller's position in `tpp_lexer_foreach_embed_path()`.
 * @return: * :            First non-TPP_ENOENT return value of `cb`
 * @return: TPP_ENOENT:    File still not found
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_system_embed_path(self, mode, when, cb, arg) \
	tpp_hooks_call_system_embed_path(&(self)->TPP_INTERNAL(tl_hooks), self, mode, when, cb, arg)
#if TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK)
#define tpp_lexer_gethookcookie_system_embed_path(self) tpp_hooks_getcookie_system_embed_path(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_system_embed_path(self)    tpp_hooks_get_system_embed_path(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_system_embed_path(self)  tpp_hooks_reset_system_embed_path(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_system_embed_path(self, v) tpp_hooks_set_system_embed_path(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_system_embed_path_ex(self, v, cookie) tpp_hooks_set_system_embed_path_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_SYSTEM_EMBED_PATH_HOOK) */

/* >> tpp_ssize tpp_lexer_callhook_unknown_string_escape(tpp_hook_cookie cookie, tpp_char const **p_pos, tpp_char const *end, tpp_lexer_decodestring_config const *tpp_restrict config);
 * Called by `tpp_lexer_decodestring()` when an unknown `\`-escape sequence is encountered.
 * This hook can be used to define additional, user-defined escape sequences, or any other
 * arbitrary behavior to-be performed when specific escape-sequences are found.
 * On entry, `*p_pos` points at the first (unrecognized) character after the leading `\`, and
 * if the hook was able to parse said escape sequence, it should update `*p_pos` to point after
 * it before returning.
 * @param: p_pos:  [in]  Pointer to start of unrecognized `\`-escape sequence
 *                 [out] First character no longer part of `\`-escape sequence (if recognized)
 *                 [out] Unchanged (if not recognized)
 * @param: end:    The of containing string sequence
 * @param: config: Identically-named argument of `tpp_lexer_decodestring()`
 * @return: * :    Sum of positive return values of `data_printer` and `utf8_printer`
 * @return: < 0:   First negative return value of `data_printer` or `utf8_printer`
 * @return: TPP_SSIZE_OFERR(TPP_ENOENT):    Escape sequence still not recognized
 *                 (please leave `*p_pos` unchanged in this case). The caller will
 *                 proceed by emitting `TPP_W_UNKNOWN_STRING_ESCAPE_SEQUENCE`
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
 * @return: TPP_SSIZE_OFERR(TPP_EIO):       Filesystem I/O operation failed
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): A lexer error happened
 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error */
#define tpp_lexer_callhook_unknown_string_escape(self, p_pos, end, config) \
	tpp_hooks_call_unknown_string_escape(&(self)->TPP_INTERNAL(tl_hooks), self, p_pos, end, config)
#if TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK)
#define tpp_lexer_gethookcookie_unknown_string_escape(self) tpp_hooks_getcookie_unknown_string_escape(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_unknown_string_escape(self)    tpp_hooks_get_unknown_string_escape(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_unknown_string_escape(self)  tpp_hooks_reset_unknown_string_escape(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_unknown_string_escape(self, v) tpp_hooks_set_unknown_string_escape(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_unknown_string_escape_ex(self, v, cookie) tpp_hooks_set_unknown_string_escape_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_STRING_ESCAPE_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_raise_lexerror(tpp_hook_cookie cookie);
 * Called by `tpp_lexer_warnf()` just before it's about to return `TPP_ELEXERROR`
 * This hook can be used to do additional state changes that may be necessary by the
 * hosting application in order to handle the resulting `TPP_ELEXERROR`
 * @return: TPP_EOK:       Have `tpp_lexer_warnf()` return `TPP_ELEXERROR`
 * @return: TPP_ELEXERROR: Same as `TPP_EOK`
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_raise_lexerror(self) \
	tpp_hooks_call_raise_lexerror(&(self)->TPP_INTERNAL(tl_hooks), self)
#if TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK)
#define tpp_lexer_gethookcookie_raise_lexerror(self) tpp_hooks_getcookie_raise_lexerror(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_raise_lexerror(self)    tpp_hooks_get_raise_lexerror(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_raise_lexerror(self)  tpp_hooks_reset_raise_lexerror(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_raise_lexerror(self, v) tpp_hooks_set_raise_lexerror(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_raise_lexerror_ex(self, v, cookie) tpp_hooks_set_raise_lexerror_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_RAISE_LEXERROR_HOOK) */

/* >> tpp_errno tpp_lexer_callhook_isfloatsuffix(tpp_hook_cookie cookie, tpp_char const *pos);
 * Called by `tpp_lexer_yieldraw()` when `TPP_HAVE_SMART_FLOAT_TOKENS` is enabled and
 * a sequence like `1.f` is encountered where the lexer is unsure if the `f` should be
 * part of the float-token (in the form of a float-suffix), or if this should actually be
 * parsed as 3 tokens: `[C_INT:1][DOT:.][f:f]`. For this purpose, this hook is called
 * with `pos` pointing at the `f` (additional characters thereafter may not be loaded
 * yet, though can be loaded using `tpp_lexer_readchar()` and `tpp_lexer_readunichar()`)
 * @return: TPP_EOK:       Pointed-to location actually *does* refer to a float suffix
 * @return: TPP_ENOENT:    It's not a float suffix
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       Filesystem I/O operation failed
 * @return: TPP_ELEXERROR: A lexer error happened
 * @return: TPP_EUSER(*):  User-defined error */
#define tpp_lexer_callhook_isfloatsuffix(self, pos) \
	tpp_hooks_call_isfloatsuffix(&(self)->TPP_INTERNAL(tl_hooks), self, pos)
#if TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK)
#define tpp_lexer_gethookcookie_isfloatsuffix(self) tpp_hooks_getcookie_isfloatsuffix(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_gethook_isfloatsuffix(self)    tpp_hooks_get_isfloatsuffix(&(self)->TPP_INTERNAL(tl_hooks))
#define tpp_lexer_resethook_isfloatsuffix(self)  tpp_hooks_reset_isfloatsuffix(&(self)->TPP_INTERNAL(tl_hooks), self)
#define tpp_lexer_sethook_isfloatsuffix(self, v) tpp_hooks_set_isfloatsuffix(&(self)->TPP_INTERNAL(tl_hooks), self, v)
#if TPP_HAVE_HOOK_COOKIES
#define tpp_lexer_sethook_isfloatsuffix_ex(self, v, cookie) tpp_hooks_set_isfloatsuffix_ex(&(self)->TPP_INTERNAL(tl_hooks), v, cookie)
#endif /* TPP_HAVE_HOOK_COOKIES */
#endif /* TPP_HOOK_ISRT(TPP_HAVE_ISFLOATSUFFIX_HOOK) */
/*[[[end]]]*/



/* Initialize/finalize everything about `self`, except for the
 * currently loaded file; which the caller must still initialize
 * using one of the `tpp_lexer_initfile_*` functions below. */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_init(tpp_lexer *tpp_restrict self);

/* Finalize the lexer, except for the currently loaded file.
 *
 * If the caller made use of `tpp_lexer_initfile_*`, then they
 * must also (either before or after this function) call
 * `tpp_lexer_finifile()` to finalize the currently loaded file. */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_fini(tpp_lexer *tpp_restrict self);

/* Move-construct `self` out of `from`
 * Use this function instead of directly assigning one lexer on-to
 * another: even though the later would currently work to do so,
 * that might change in the future so use of this macro gives you
 * that forward-compatibility. */
#define tpp_lexer_move(self, from) \
	(void)(*(self) = *(from), tpp_dbg_memset(from, sizeof(tpp_lexer)))

#if TPP_HAVE_LEXER_COPY
/* Initialize `self` as a copy of `from`. This will copy everything
 * configured in `from` (features, extensions, allocated keyword IDs,
 * macros, include paths, warnings, etc), into `self`. The only thing
 * that is not copied is the `#include`-stack, meaning that after a call
 * to this function, the caller must still call `tpp_lexer_initfile_*`
 *
 * Additionally, the following properties are not copied:
 * - `tpp_keyword_getuserdata_dtor()`  (only `tpp_keyword_getuserdata()`
 *                                      is copied; dtors are set to `NULL`)
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_copy(tpp_lexer *tpp_restrict self,
               tpp_lexer const *tpp_restrict from);
#endif /* TPP_HAVE_LEXER_COPY */

#if TPP_HAVE_LEXER_OPENFILE
typedef struct tpp_lexer_openfile_result {
	tpp_io_handle  tlofr_handle;       /* [1..1][owned] I/O handle for requested file (must be inherited by caller) */
#if TPP_HAVE_USER_KEYWORDS
	tpp_keyword   *tlofr_filename_kwd; /* [1..1] Keyword for filename */
#define tpp_lexer_openfile_result_getfilename(self) tpp_keyword_getcstr((self)->tlofr_filename_kwd)
#else /* TPP_HAVE_USER_KEYWORDS */
	char          *tlofr_filename;     /* [1..1][owned] Filename string */
#define tpp_lexer_openfile_result_getfilename(self) ((char const *)(self)->tlofr_filename)
#endif /* !TPP_HAVE_USER_KEYWORDS */
#if TPP_HAVE_FILE_SYSHDR
	tpp_file_flags tlofr_fileflags;    /* Either `TPP_FILE_FLAGS_NORMAL` or `TPP_FILE_FLAGS_SYSHDR` or `TPP_FILE_FLAGS_EXTERN_C` */
#define _tpp_lexer_openfile_result_getfileflags(self) (self)->tlofr_fileflags
#else /* TPP_HAVE_FILE_SYSHDR */
#define _tpp_lexer_openfile_result_getfileflags(self) TPP_FILE_FLAGS_NORMAL
#endif /* !TPP_HAVE_FILE_SYSHDR */
#if TPP_HAVE_USER_KEYWORDS
#define tpp_lexer_openfile_result_getfileflags(self) _tpp_lexer_openfile_result_getfileflags(self)
#elif TPP_HAVE_FILE_NOKWD
#define tpp_lexer_openfile_result_getfileflags(self) (_tpp_lexer_openfile_result_getfileflags(self) | TPP_FILE_FLAGS_FREENAME | TPP_FILE_FLAGS_NOKWD)
#else /* ... */
#define tpp_lexer_openfile_result_getfileflags(self) (_tpp_lexer_openfile_result_getfileflags(self) | TPP_FILE_FLAGS_FREENAME)
#endif /* !... */
} tpp_lexer_openfile_result;


#if TPP_HAVE_USER_KEYWORDS
#define tpp_lexer_openfile_result_fini(self) \
	(tpp_io_close((self)->tlofr_handle),     \
	 tpp_dbg_memset(self, sizeof(*(self))))
#else /* TPP_HAVE_USER_KEYWORDS */
#define tpp_lexer_openfile_result_fini(self) \
	(tpp_io_close((self)->tlofr_handle),     \
	 tpp_free((self)->tlofr_filename),       \
	 tpp_dbg_memset(self, sizeof(*(self))))
#endif /* !TPP_HAVE_USER_KEYWORDS */

#if TPP_HAVE_LEXER_OPENFILE_EX
#define TPP_LEXER_OPENFILE_FLAG_NORMAL 0 /* Normal flags */
#ifdef tpp_keyword_flags
#define tpp_lexer_openfile_flags tpp_keyword_flags /* Set of `TPP_LEXER_OPENFILE_FLAG_*` */
#if TPP_HAVE_CPP_IMPORT
#define TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED TPP_KEYWORD_FLAG_HDR_IMPORTED    /* Filter out files that were already `#import`-ed */
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_PRAGMA_ONCE
#define TPP_LEXER_OPENFILE_FLAG_HDR_ONCE     TPP_KEYWORD_FLAG_HDR_ONCE        /* Filter out files with `#pragma once` */
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#define TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED  TPP_KEYWORD_FLAG_HDR_GUARD_VALID /* Filter out files with a confirmed `#ifndef`-block of a macro that is current defined */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#else /* tpp_keyword_flags */
#define tpp_lexer_openfile_flags uint_least32_t /* Set of `TPP_LEXER_OPENFILE_FLAG_*` */
#endif /* !tpp_keyword_flags */
#if TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next
#define TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT UINT16_C(0x0100) /* Reject files that are already on the `#include`-stack */
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_MACRO___has_include_next */
#if TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED
#define TPP_LEXER_OPENFILE_FLAG_CHECK_LIMIT  UINT16_C(0x0200) /* Emit a warning if the file already appears too often on the `#include`-stack */
#else /* TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED */
#define TPP_LEXER_OPENFILE_FLAG_CHECK_LIMIT  UINT16_C(0x0000) /* no-op */
#endif /* !TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED */
#if TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING
#define TPP_LEXER_OPENFILE_FLAG_WARN_CASING  UINT16_C(0x0400) /* Emit a warning `TPP_W_NONPORTABLE_FILENAME_CASING` if the file's casing is bad */
#else /* TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING */
#define TPP_LEXER_OPENFILE_FLAG_WARN_CASING  UINT16_C(0x0000) /* no-op */
#endif /* !TPP_HAVE_TPP_W_NONPORTABLE_FILENAME_CASING */

/* Same as `tpp_lexer_openfile`, but return `TPP_EMASKED` if the file was already
 * included before, and its keyword has any of the bits specified by `mask_flags` set.
 *
 * NOTES:
 * - A special case is made when `mask_flags & TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED`,
 *   in which case, `TPP_EMASKED` is only returned if `tkm_file_guard` is a macro that
 *   is currently considered to be `#if defined()`.
 * - Another special case is made for `TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT`, which
 *   causes `TPP_EMASKED` to be returned if the file's keyword is already included
 *   somewhere on the `#include`-stack.
 * - Also: when `mask_flags & TPP_KEYWORD_FLAG_HDR_IMPORTED`, and the file's keyword
 *   doesn't already have the `TPP_KEYWORD_FLAG_HDR_IMPORTED` flag set, the open will
 *   succeed, and the `TPP_KEYWORD_FLAG_HDR_IMPORTED` flag will become set (so-as to
 *   implement the include-once semantics of `#import`)
 * - This function always sets `tlofr_fileflags = TPP_FILE_FLAGS_NORMAL`.
 *   If the given `relative_to` belongs to a system header, then it is up
 *   to the caller to set that flag. `tpp_lexer_open_include_string_ex()`
 *   will do so automatically after calling this function.
 *
 * @param: mask_flags: Set of flags describing circumstances under which `TPP_EMASKED`
 *                     should be returned:
 *                     - `TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED`
 *                     - `TPP_LEXER_OPENFILE_FLAG_HDR_ONCE`
 *                     - `TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED`
 *                     - `TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT`
 *
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOMEM:  Insufficient memory
 * @return: TPP_ENOENT:  No such file, or `TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT` was
 *                       given, and the file is already located on the `#include`-stack.
 * @return: TPP_EMASKED: Flags specified by `mask_flags` were already set */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3, 5)) tpp_errno TPPCALL
tpp_lexer_openfile_ex(/*1..1*/ tpp_lexer *tpp_restrict self,
                      /*0..1*/ char const *tpp_restrict relative_to,
                      /*1..1*/ /*utf-8*/ char const *filename, tpp_size filename_maxlen,
                      /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result,
                      tpp_lexer_openfile_flags mask_flags);
#define tpp_lexer_openfile(self, relative_to, filename, filename_maxlen, result) \
	tpp_lexer_openfile_ex(self, relative_to, filename, filename_maxlen, result, TPP_LEXER_OPENFILE_FLAG_NORMAL)
#else /* TPP_HAVE_LEXER_OPENFILE_EX */
/* Construct the filename, open the file, and initialize `result` accordingly
 * @param: relative_to: The `tpp_file::tf_data.td_io.tff_name` of another file,
 *                      in case `filename` is a relative path, in which case the
 *                      filename of the file to open should be relative to the
 *                      directory of `relative_to`
 * @param: result:      Open file information (pass along to `tpp_file_init_io()`)
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Insufficient memory
 * @return: TPP_ENOENT: File not found (if you have additional `relative_to`, try them) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3, 5)) tpp_errno TPPCALL
tpp_lexer_openfile(/*1..1*/ tpp_lexer *tpp_restrict self,
                   /*0..1*/ char const *tpp_restrict relative_to,
                   /*1..1*/ /*utf-8*/ char const *filename, tpp_size filename_maxlen,
                   /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result);
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
#endif /* TPP_HAVE_LEXER_OPENFILE */


/* Finalize the currently loaded file (including any extra files
 * found on the `#include`-stack, but that hadn't been popped yet)
 *
 * This function must be called after `tpp_lexer_initfile_*` has been */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_finifile(tpp_lexer *tpp_restrict self);


/* Initialize a lexer's file to read the given [text,text+text_size) blob.
 * @param: start_lc: [valid_if(chunk != NULL)] */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
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
                         );
#if TPP_HAVE_FILE_FLAGS && TPP_HAVE_UNICODE
#define tpp_lexer_initfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, encoding) \
	_tpp_lexer_initfile_text(self, filename, chunk, text, text_size, start_lc, flags, encoding)
#elif TPP_HAVE_FILE_FLAGS
#define tpp_lexer_initfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, encoding) \
	_tpp_lexer_initfile_text(self, filename, chunk, text, text_size, start_lc, flags)
#elif TPP_HAVE_UNICODE
#define tpp_lexer_initfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, encoding) \
	_tpp_lexer_initfile_text(self, filename, chunk, text, text_size, start_lc, encoding)
#else /* ... */
#define tpp_lexer_initfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, encoding) \
	_tpp_lexer_initfile_text(self, filename, chunk, text, text_size, start_lc)
#endif /* !... */
#define tpp_lexer_initfile_text(self, filename, chunk, text, text_size, start_lc, flags) \
	tpp_lexer_initfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, TPP_FILE_ENCODING_UTF8)
#define tpp_lexer_initfile_text_ascii(self, filename, chunk, text, text_size, start_lc, flags) \
	tpp_lexer_initfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, TPP_FILE_ENCODING_ASCII)
#if TPP_HAVE_UNICODE
#define tpp_lexer_initfile_text_utf8(self, filename, chunk, text, text_size, start_lc, flags) \
	tpp_lexer_initfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, TPP_FILE_ENCODING_FORCE_UTF8)
#endif /* TPP_HAVE_UNICODE */


#if TPP_HAVE_LEXER_INIT_IO
/* Initialize a lexer such that it starts reading from `handle`
 * @param: filename: [0..1] Filename to use for messages (s.a. `tpp_file_getrealfilename()`)
 *                          WARNING: This filename is *NOT* copied -- it must remain
 *                                   allocated and valid until `self` is finalized.
 * @param: handle:   The I/O handle to read from in order to retrieve text data.
 * @param: ioflags:  Extra flags specifying how to interact with `handle`:
 *                   - `TPP_FILE_FLAGS_NONBLOCK`: Do non-blocking reads (useful in case `handle` is a pipe)
 *                   - `TPP_FILE_FLAGS_NOCLOSE`:  A later call to `tpp_lexer_finifile()` will not close `handle`
 *                   - `TPP_FILE_FLAGS_SYSHDR`:   Do not emit warnings */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_initfile_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                         tpp_io_handle handle, tpp_file_flags ioflags);
#define tpp_lexer_initfile_io(self, filename, handle) \
	tpp_lexer_initfile_io_ex(self, filename, handle, TPP_FILE_FLAGS_NORMAL)
#endif /* TPP_HAVE_LEXER_INIT_IO */

#if TPP_HAVE_LEXER_INIT_OPEN
/* Initialize a lexer such that it starts reading from `filename`
 * @param: filename_maxlen: Max length of `filename` (in characters). You may
 *                          pass `TPP_SIZE_MAX` when `filename` is NUL-terminated.
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_initfile_open(tpp_lexer *tpp_restrict self,
                        /*utf-8*/ char const *tpp_restrict filename,
                        tpp_size filename_maxlen);

/* Initialize a lexer such that it starts reading
 * from `tpp_lexer_openfile_result *ofr` (never fails) */
#define tpp_lexer_initfile_ofr(self, /*inherit*/ /*tpp_lexer_openfile_result **/ofr) \
	tpp_file_init_io_from_ofr(tpp_lexer_getfile(file), ofr)
#endif /* TPP_HAVE_LEXER_INIT_OPEN */


#if TPP_HAVE_INCLUDE_STACK
#if TPP_HAVE_LEXER_INIT_IO
/* Push another file onto the `#include`-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @param: filename: [0..1] Filename to use for messages (s.a. `tpp_file_getrealfilename()`)
 *                          WARNING: This filename is *NOT* copied -- it must remain
 *                                   allocated and valid until `self` is finalized.
 * @param: handle:   The I/O handle to read from in order to retrieve text data.
 * @param: ioflags:  Extra flags specifying how to interact with `handle`:
 *                   - `TPP_FILE_FLAGS_NONBLOCK`: Do non-blocking reads (useful in case `handle` is a pipe)
 *                   - `TPP_FILE_FLAGS_NOCLOSE`:  A later call to `tpp_lexer_finifile()` will not close `handle`
 *                   - `TPP_FILE_FLAGS_SYSHDR`:   Do not emit warnings
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_pushfile_io_ex(tpp_lexer *tpp_restrict self, /*utf-8*/ char const *filename,
                         tpp_io_handle handle, tpp_file_flags ioflags);
#define tpp_lexer_pushfile_io(self, filename, handle) \
	tpp_lexer_pushfile_io_ex(self, filename, handle, TPP_FILE_FLAGS_NORMAL)
#endif /* TPP_HAVE_LEXER_INIT_IO */

#if TPP_HAVE_LEXER_INIT_OPEN
/* Push another file onto the `#include`-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @param: filename_maxlen: Max length of `filename` (in characters). You may
 *                          pass `TPP_SIZE_MAX` when `filename` is NUL-terminated.
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: No such file or directory
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_pushfile_open(tpp_lexer *tpp_restrict self,
                        /*utf-8*/ char const *tpp_restrict filename,
                        tpp_size filename_maxlen);

/* Push another file onto the `#include`-stack:
 * After a call to this function, the caller is responsible to yield the first token!
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (given `ofr` was *NOT* inherited) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_pushfile_ofr(tpp_lexer *tpp_restrict self,
                       /*inherit(on_success)*/ tpp_lexer_openfile_result *tpp_restrict ofr);
#endif /* TPP_HAVE_LEXER_INIT_OPEN */

/* Push another file onto the `#include`-stack: [text,text+text_size) blob.
 * After a call to this function, the caller is responsible to yield the first token!
 * @param: start_lc: [valid_if(chunk != NULL)]
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
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
                         );
#if TPP_HAVE_FILE_FLAGS && TPP_HAVE_UNICODE
#define tpp_lexer_pushfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, encoding) \
	_tpp_lexer_pushfile_text(self, filename, chunk, text, text_size, start_lc, flags, encoding)
#elif TPP_HAVE_FILE_FLAGS
#define tpp_lexer_pushfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, encoding) \
	_tpp_lexer_pushfile_text(self, filename, chunk, text, text_size, start_lc, flags)
#elif TPP_HAVE_UNICODE
#define tpp_lexer_pushfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, encoding) \
	_tpp_lexer_pushfile_text(self, filename, chunk, text, text_size, start_lc, encoding)
#else /* ... */
#define tpp_lexer_pushfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, encoding) \
	_tpp_lexer_pushfile_text(self, filename, chunk, text, text_size, start_lc)
#endif /* !... */
#define tpp_lexer_pushfile_text(self, filename, chunk, text, text_size, start_lc, flags) \
	tpp_lexer_pushfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, TPP_FILE_ENCODING_UTF8)
#define tpp_lexer_pushfile_text_ascii(self, filename, chunk, text, text_size, start_lc, flags) \
	tpp_lexer_pushfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, TPP_FILE_ENCODING_ASCII)
#if TPP_HAVE_UNICODE
#define tpp_lexer_pushfile_text_utf8(self, filename, chunk, text, text_size, start_lc, flags) \
	tpp_lexer_pushfile_text_ex(self, filename, chunk, text, text_size, start_lc, flags, TPP_FILE_ENCODING_FORCE_UTF8)
#endif /* TPP_HAVE_UNICODE */


/* Check if the current file can be popped. */
#define tpp_lexer_canpopfile(self) \
	(tpp_lexer_getfile(self)->TPP_INTERNAL(tf_prev) != NULL)

/* Pop the current file off the `#include`-stack.
 * The caller is responsible to ensure that `tpp_lexer_canpopfile(self) == true`
 * After a call to this function, the caller is responsible to yield the next token!
 * WARNING: It is the caller's responsibility to call `tpp_lexer_manualpopfile_popfile()`
 *          instead of this function if rollback of the pop should be possible.
 * NOTE: It is recommended to call `tpp_lexer_warn_nonempty_ifdef()` before calling
 *       this function in order to warn about unterminated `#ifdef`-blocks. */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_lexer_popfile(tpp_lexer *tpp_restrict self);
#define tpp_lexer_popallfiles(self)        \
	do {                                   \
		while (tpp_lexer_canpopfile(self)) \
			tpp_lexer_popfile(self);       \
	} while (0)
#else /* TPP_HAVE_INCLUDE_STACK */
#define tpp_lexer_popallfiles(self) (void)0
#endif /* !TPP_HAVE_INCLUDE_STACK */



#if TPP_HAVE_JOINPATH
/* Form an absolute filename by combining `relative_to` with `filename`
 * @return: * :   The absolute path (must be free'd by caller using `tpp_free()`)
 * @return: NULL: Out of memory. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((2)) char *TPPCALL
tpp_joinpath(/*0..1*/ char const *tpp_restrict relative_to,
             /*1..1*/ /*utf-8*/ char const *filename,
             tpp_size filename_maxlen);
#endif /* TPP_HAVE_JOINPATH */


#if TPP_HAVE_LEXER_CLI_DEFINE
/* Define (or override) a macro `macro_name` with a body definition `macro_body`
 * When `macro_name` contains an opening `(` character, it, as well as `macro_body`
 * are parsed as a function-like macro. The same also goes for `{`, `[` and `<`
 * when `TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS` is enabled.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 4)) tpp_errno TPPCALL
tpp_lexer_define(tpp_lexer *tpp_restrict self,
                 char const *macro_name, tpp_size macro_name_maxlen,
                 char const *macro_body, tpp_size macro_body_maxlen);

/* Delete a macro definition
 * @return: true:  Success
 * @return: false: No such macro */
TPP_DECL TPP_NONNULL((1, 2)) bool TPPCALL
tpp_lexer_undef(tpp_lexer *tpp_restrict self,
                char const *macro_name, tpp_size macro_name_maxlen);
#endif /* TPP_HAVE_LEXER_CLI_DEFINE */

#if TPP_HAVE_KEYWORDS_UNDEFALL
/* Delete all user-defined macro definitions */
#define tpp_lexer_undefalluser(self) tpp_keywords_undefalluser(&(self)->TPP_INTERNAL(tl_kwds))
#endif /* TPP_HAVE_KEYWORDS_UNDEFALL */


#if TPP_HAVE_LEXER_CLI_ASSERT
/* Add a new keyword assertions for `key` and `value`.
 * This is the same as doing `#assert {key}({value})`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 4)) tpp_errno TPPCALL
tpp_lexer_assert(tpp_lexer *tpp_restrict self,
                 char const *key, tpp_size key_maxlen,
                 char const *value, tpp_size value_maxlen);

/* Delete a new keyword assertions for `key` and `value`.
 * This is the same as doing `#unassert {key}({value})`
 * @return: true:  Success
 * @return: false: No such assertion */
TPP_DECL TPP_NONNULL((1, 2, 4)) bool TPPCALL
tpp_lexer_unassert(tpp_lexer *tpp_restrict self,
                   char const *key, tpp_size key_maxlen,
                   char const *value, tpp_size value_maxlen);

/* Delete all keyword assertions for `key`.
 * This is the same as doing `#unassert {key}` */
TPP_DECL TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_unassertall(tpp_lexer *tpp_restrict self,
                      char const *key, tpp_size key_maxlen);
#endif /* TPP_HAVE_LEXER_CLI_ASSERT */

#if TPP_HAVE_KEYWORDS_UNASSERTALL
/* Delete all user-defined keyword assertions */
#define tpp_lexer_unassertall2(self) tpp_keywords_unassertall(&(self)->TPP_INTERNAL(tl_kwds))
#endif /* TPP_HAVE_KEYWORDS_UNASSERTALL */


#if TPP_HAVE_LEXER_RAND
/* Produce a random number based on:
 * - `tpp_lexer_getrngseed()` (affected by `tpp_lexer_popfile()` + `tpp_lexer_manualpopfile_break_commit()`)
 * - `tpp_lexer_getinputhash()` (affected by everything read from files currently on the `#include`-stack)
 *
 * The result of the combination of those 2 values is then put
 * through a PRNG, before the result of the PRNG is then returned
 * by this function.
 *
 * This function is used to implement the builtin macro `__TPP_RANDOM()` */
TPP_DECL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_lexer_getrand(tpp_lexer const *tpp_restrict self);

TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_lexer_nextrand(tpp_lexer *tpp_restrict self) {
	tpp_hash const result = tpp_lexer_getrand(self);
	tpp_lexer_setrngseed(self, result);
	return result;
}
#endif /* TPP_HAVE_LEXER_RAND */



#if TPP_HAVE_LEXER_REQUIRE_WHITESPACE
/* Check if 2 tokens, when written directly adjacent to each other,
 * *might* (though not necessarily) result in a different set of
 * tokens when re-parsed. */
TPP_DECL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_require_whitespace(tpp_lexer const *tpp_restrict self,
                             tpp_token_id lhs, tpp_token_id rhs);
#endif /* TPP_HAVE_LEXER_REQUIRE_WHITESPACE */



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
/* Same as `tpp_lexer_readchar()`, but (if the current file's encoding allows
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

/* Alter the lexer state such that `tpp_lexer_yieldpp()` does not filter tokens. */
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


/* Do a raw yield and update `self->tl_tok` in the process, then return `tl_tok.tt_id`.
 * - On EOF, automatically pop `tl_file->tf_prev` and continue reading from there
 * - On error, return one of `TPP_TOK_E*` (e.g. `TPP_TOK_EIO`).
 *   Such error codes will NOT be stored in `tl_tok.tt_id`!
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
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses `TPP_FILE_FLAGS_NONBLOCK` and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EUSER(*):    User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldraw(tpp_lexer *tpp_restrict self);

/* Same as `tpp_lexer_yieldraw()`, but populate the token from a custom `*p_pos`,
 * and don't pop files from the current `#include`-stack (unless `p_pos` is the top-
 * most file's `tf_pos`)
 *
 * NOTES:
 *  - This function will *NOT* update `tpp_lexer_gettokenend(self)`, however
 *    the value it would have written there is `OUT(*p_pos)`, meaning you
 *    can just use that instead, and call this function multiple times
 *    to yield more than 1 token
 *  - This function can be used to peek future tokens, as it will also expand
 *    the current file when `*p_pos` would go beyond its end. (in this case,
 *    `*p_pos` is updated such that it always remains valid)
 *  - Unlike `tpp_lexer_yieldraw()`, this function will *not* modify the
 *    currently loaded file's `tf_pos` (unless `p_pos == &file->tf_pos`),
 *    meaning that if EOF is reached, the file's chunk will only ever be
 *    expanded, but no old data (that would appear before `tf_pos`) will
 *    be deallocated
 *  - This function will also not automatically move on to the next file
 *    in line when the current one has been fully exhausted (unless the
 *    given `p_pos == &file->tf_pos`), meaning that `TPP_TOK_EOF` will be
 *    returned when no more data can be loaded.
 *
 * This is used to implement `tpp_lexer_yieldraw()`, which simply
 * passes `p_pos = &tpp_lexer_gettoken(self)->tt_end`
 *
 * @return: * : See `tpp_lexer_yieldraw()` */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at(tpp_lexer *tpp_restrict self, tpp_char const **p_pos);


typedef struct tpp_lexer_seek_backup {
	tpp_token_id              TPP_INTERNAL(tlsb_id);    /* Saved token id */
	struct tpp_keyword const *TPP_INTERNAL(tlsb_kwd);   /* [1..1][valid_if(TPP_TOK_ISKEYWORD(tlsb_id))] Saved token keyword */
	tpp_size                  TPP_INTERNAL(tlsb_len);   /* Relative length of token */
} tpp_lexer_seek_backup;

/* Save/restore the currently loaded token. This must be done before/after
 * making use of `tpp_lexer_yieldraw_at()` with a custom text pointer:
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
 * pop the currently loaded lexer file such that the file next-up in the `#include`-
 * stack can be yielded from instead.
 *
 * NOTES:
 *  - These macros respect `tpp_lexer_autopopfile_pushoff()` such that
 *    `tpp_lexer_manualpopfile_canpopfile(self)` returns `false` if
 *    the then top-most file had automatic popping disabled.
 *  - Changes made to the effective `tf_pos` of files must be restored
 *    by the user before calling `tpp_lexer_manualpopfile_popfile()`.
 *
 * This functionality is needed/used to scan for the opening '(' token in macro calls
 * in those cases where the file containing the original macro invocation ends with
 * EOF before such a token can be located:
 * >> #define foo(a, b) a+b
 * >> #define bar       foo
 * >> bar(10, 20)  // Here, `foo` (macro name) and `(` are located in different files, but macro expansion must still happen
 * >> bar[10, 20]  // But for this case, a rollback behavior is still needed to still yield `foo` as an identifer
 *
 * Use these macros as follows:
 * >> // Check if the next `raw`, non-space token will be `(`.
 * >> // If so, yield to it (such that the current token becomes '(') and return *true*
 * >> // Otherwise, don't change anything and return *false*
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
 * >>         // Check files further up the `#include`-stack
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
 *   - Use `tpp_file_autopopfile_pushoff()` to set a limit on how far files can be popped
 *   - Use `tpp_file_setkeep()` on all popable files to prevent old data from being free'd
 *   - Save `tpp_file_getpos()` of popable files relative to their `tpp_file_getkeep()`
 *   - Use `tpp_lexer_manualpopfile_start()`
 *   - Set the `TPP_LEXER_STATE_FLAG_POPFILERLBK` state flag
 * - At this, functions like `tpp_lexer_yieldpp()` or `tpp_lexer_yield()`
 *   can be called like normal.
 * - Rollback can then Implement rollback as:
 *   - Restore the `TPP_LEXER_STATE_FLAG_POPFILERLBK` flag
 *   - `tpp_lexer_manualpopfile_end_rollback(self)`
 *   - Restore positions of popable files by doing `tpp_file_getkeep() + REL_POS`
 *   - Restore the old keep-positions of popable files
 *   - Calling `tpp_file_autopopfile_pop()` on popable files
 * - But be careful: side-effects like macro definitions or `#pragma` directives aren't rolled back
 *
 * HINT: Something similar to this is done to parse the argument lists of macros when
 *       rollback needs to be possible due to the `TPP_MACRO_FLAG_SELFEXPAND` extension.
 *
 * WARNING: All additional files pushed onto the `#include`-stack at any point while
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



/* Wrapper around `tpp_lexer_yieldraw()` that filters certain tokens (based on
 * configured features), and implements handling for preprocessor directives,
 * like `#define`, `#include`, etc:
 * - TPP_TOK_LF:      Filtered based on `TPP_HAVE_TOK_LF` / `TPP_FEAT_TPP_TOK_LF`
 * - TPP_TOK_SPACE:   Filtered based on `TPP_HAVE_TOK_SPACE` / `TPP_FEAT_TPP_TOK_SPACE`
 * - TPP_TOK_COMMENT: Filtered based on `TPP_HAVE_TOK_COMMENT` / `TPP_FEAT_TPP_TOK_COMMENT`
 *
 * @return: * :                  The newly read token (after accounting for preprocessor directives)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses `TPP_FILE_FLAGS_NONBLOCK` and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EUSER(*):    User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp(tpp_lexer *tpp_restrict self);


/* Wrapper around `tpp_lexer_yieldpp()` that adds handling for macro expansion.
 * @return: * :                  The newly read token (after accounting for macros)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses `TPP_FILE_FLAGS_NONBLOCK` and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EUSER(*):    User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield(tpp_lexer *tpp_restrict self);

/* Handle a keyword-style macro (used to implement `tpp_lexer_yield()`).
 * @return: TPP_TOK_EOF: Caller should yield again.
 * @return: * : The new expansion token after keywords were handled */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_keyword(tpp_lexer *tpp_restrict self, tpp_token_id tok);



#if TPP_HAVE_FILE_NONBLOCK
/* Same as `tpp_lexer_yield()`, but handle `TPP_TOK_EWOULDBLOCK` by temporarily
 * clearing the `TPP_FILE_FLAGS_NONBLOCK` flag, and re-attempting the yield. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_blocking(tpp_lexer *tpp_restrict self);

/* Same as `tpp_lexer_yieldpp()`, but handle `TPP_TOK_EWOULDBLOCK` by temporarily
 * clearing the `TPP_FILE_FLAGS_NONBLOCK` flag, and re-attempting the yield. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp_blocking(tpp_lexer *tpp_restrict self);

/* Same as `tpp_lexer_yieldraw()`, but handle `TPP_TOK_EWOULDBLOCK` by temporarily
 * clearing the `TPP_FILE_FLAGS_NONBLOCK` flag, and re-attempting the yield. */
#define tpp_lexer_yieldraw_blocking(self) \
	tpp_lexer_yieldraw_at_blocking(self, &tpp_lexer_gettoken(self)->TPP_INTERNAL(tt_end))

/* Same as `tpp_lexer_yieldraw_at()`, but handle `TPP_TOK_EWOULDBLOCK` by temporarily
 * clearing the `TPP_FILE_FLAGS_NONBLOCK` flag, and re-attempting the yield. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_blocking(tpp_lexer *tpp_restrict self, tpp_char const **p_pos);
#else /* TPP_HAVE_FILE_NONBLOCK */
#define tpp_lexer_yield_blocking(self)              tpp_lexer_yield(self)
#define tpp_lexer_yieldpp_blocking(self)            tpp_lexer_yieldpp(self)
#define tpp_lexer_yieldraw_blocking(self)           tpp_lexer_yieldraw(self)
#define tpp_lexer_yieldraw_at_blocking(self, p_pos) tpp_lexer_yieldraw_at(self, p_pos)
#endif /* !TPP_HAVE_FILE_NONBLOCK */


#if TPP_HAVE_LEXER_PARSEEMBED
/* Quickly parse a `,`-separated sequence of `TPP_TOK_ISINT()`-like
 * tokens in range of `[0,0xff]` into a sequence of bytes, which are
 * then fed to `printer`.
 *
 * If enabled and used, this function has special optimizations for
 * input files pushed by `#embed` and `TPP_HAVE_FILE_ENCODING_EMBED`,
 * such that in this case, anything not already read from the file
 * will be streamed directly into `printer` (without the need of
 * `tpp_file` converting file bytes into decimals first).
 *
 * NOTES:
 * - This function must be called when the current token is an int
 *   This is asserted: `tpp_assert(TPP_TOK_ISINT(tpp_lexer_gettok(self)))`
 * - This function returns in the following situations:
 *   - After skipping whitespace and comments, the token that follows
 *     a `TPP_TOK_ISINT()`-like token isn't `TPP_TOK_COMMA`. In this
 *     case, the return value is the sum of calls to `printer`, and
 *     the currently loaded token is whatever that non-`TPP_TOK_ISINT()`
 *     turned out to be.
 *     In this case `*p_final_state = TPP_LEXER_PARSEEMBED_STATE_COMMA`
 *   - After skipping a `TPP_TOK_COMMA` (and any whitespace+comments
 *     thereafter), what follows wasn't a `TPP_TOK_ISINT()`-like token.
 *     Like with the prior case, the return value is the sum of calls
 *     to `printer`, and the currently loaded token is whatever that
 *     non-`TPP_TOK_ISINT()` turned out to be.
 *     In this case `*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER`
 *   - A `TPP_TOK_ISINT()`-like token is encountered whose decoded value
 *     falls outside the range of `[0,0xff]`.
 *     Once again, the return value is the sum of calls to `printer`,
 *     and the currently loaded token is whatever that `TPP_TOK_ISINT()`
 *     token whose value (as per `tpp_lexer_decodeint()`) is bad.
 *     In this case `*p_final_state = TPP_LEXER_PARSEEMBED_STATE_INTEGER`
 *   - A call to `printer` returned a negative value, which is propagated
 *     immediately. In this case `*p_final_state` is undefined, and the
 *     currently loaded token is weakly undefined. It may point to some
 *     arbitrary, or unrelated token.
 *   - A call to `tpp_lexer_yield_blocking()` returned an error, which is
 *     propagated by being wrapped as `TPP_SSIZE_OFERR()`. In this case
 *     `*p_final_state` is undefined, and the currently loaded token is
 *     left in whatever state `tpp_lexer_yield_blocking()` left it in.
 *
 * @param: p_final_state: [0..1] Set to a description of the final parser state
 * @return: * : Success:  return value is sum of return value of `printer`
 *                        current token is whatever caused parsing to stop
 *                        parsing stop position is described by `*p_final_state`
 * @return: < 0: Failure: Either `printer` returned this value, or trying to
 *                        yield to the next token resulted in an error. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_parseembed(tpp_lexer *tpp_restrict self,
                     tpp_formatprinter printer, void *arg,
                     unsigned int *p_final_state);
#define TPP_LEXER_PARSEEMBED_STATE_INTEGER 0 /* Expected an <int> but found some other token (or <int> was not in range [0,0xff]) */
#define TPP_LEXER_PARSEEMBED_STATE_COMMA   1 /* Expected a ',' but found some other token */
#endif /* TPP_HAVE_LEXER_PARSEEMBED */


#if TPP_HAVE_LEXER_YIELD_INCLUDE_STRING
/* Special token IDs for include paths */
#define TPP_TOK_INCPATH_DQUOTE TPP_TOK_OFCHAR('"') /* `#include "foo.h"` */
#define TPP_TOK_INCPATH_LANGLE TPP_TOK_OFCHAR('<') /* `#include <foo.h>` */

/* (Mostly) the same as `tpp_lexer_yield()`, except:
 * - Never process preprocessor directives (but macros are still expanded)
 * - If the next token starts with `"` or `<`, parse it as a `#include`-string,
 *   with the token's start/end bounds pointing at the string's bounds. In
 *   this case, the token's ID (and return value) is:
 *   - `TPP_TOK_INCPATH_DQUOTE` (For `#include "foo.h"`)
 *   - `TPP_TOK_INCPATH_LANGLE` (For `#include <foo.h>`)
 * - WARNING: This function doesn't filter SPACE/LF/COMMENT tokens
 *   (behaves as though `TPP_LEXER_STATE_FLAG_ALLTOKENS` was set)
 *
 * @return: * : Some other token encountered (token was parsed like tpp_lexer_yieldraw())
 * @return: TPP_TOK_INCPATH_DQUOTE: `#include`-string parsed: `"foo.h"`
 * @return: TPP_TOK_INCPATH_LANGLE: `#include`-string parsed: `<foo.h>`
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses `TPP_FILE_FLAGS_NONBLOCK` and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EUSER(*):    User-defined error from hook */
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
/* Decode the current token as a `#include`-string. The caller is responsible to
 * ensure that the current token was loaded by `tpp_lexer_yield_include_string()`
 * and is either `TPP_TOK_INCPATH_LANGLE` or `TPP_TOK_INCPATH_DQUOTE`.
 *
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_decode_include_string(tpp_lexer const *tpp_restrict self,
                                tpp_formatprinter printer, void *arg);

/* Same as `tpp_lexer_decode_include_string()`, but the given `cb` is only
 * invoked once whilst being passed the *entire* (decoded) `#include`-string.
 *
 * @return: * : Return value of the (singular) invocation of `cb`
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decode_include_string_cb(tpp_lexer const *tpp_restrict self,
                                   tpp_errno (TPPCALL *cb)(void *arg, char const *str, tpp_size length),
                                   void *arg);
#endif /* TPP_HAVE_LEXER_DECODE_INCLUDE_STRING */


#if TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
#ifndef tpp_lexer_foreach_include_path_flags__PARAM
#if TPP_HAVE_FILE_SYSHDR
#define tpp_lexer_foreach_include_path_flags__PARAM  , tpp_file_flags flags
#define tpp_lexer_foreach_include_path_flags__ARG(x) , x
#else /* TPP_HAVE_FILE_SYSHDR */
#define tpp_lexer_foreach_include_path_flags__PARAM  /* nothing */
#define tpp_lexer_foreach_include_path_flags__ARG(x) /* nothing */
#endif /* !TPP_HAVE_FILE_SYSHDR */
#endif /* !tpp_lexer_foreach_include_path_flags__PARAM */

/* Enumerate `#include`-paths according to `mode`
 * @param: mode: `#include`-mode (either `TPP_TOK_INCPATH_LANGLE` or `TPP_TOK_INCPATH_DQUOTE`)
 * @param: cb:   Callback invoked for each available `#include`-path. The first time
 *               this callback returns something other than TPP_ENOENT, that return
 *               value is propagated.
 * @param: cb.flags: Either `TPP_FILE_FLAGS_NORMAL` or `TPP_FILE_FLAGS_SYSHDR`,
 *                   possibly or'd with `TPP_FILE_FLAGS_EXTERN_C` depending on
 *                   where `relative_to` originates from, and how `self` has been
 *                   configured (see `TPP_HAVE_EXTERN_C_FOR_SYSHDR`)
 * @param: arg:  Cookie for `cb`
 * @return: * :  The first non-TPP_ENOENT return value of `cb`
 * @return: TPP_ENOENT: Either `cb` was never invoked (no `#include`-paths),
 *                      or all invocations of `cb` returned `TPP_ENOENT`. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_lexer_foreach_include_path(tpp_lexer *tpp_restrict self, tpp_token_id mode,
                               tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to
                                                       tpp_lexer_foreach_include_path_flags__PARAM),
                               void *arg);

/* Wrapper around `tpp_lexer_decode_include_string_cb()` that automatically
 * does the necessary calls to `tpp_lexer_openfile_ex()`. It also handles
 * the `TPP_ENOENT` (as far as possible) by continuing to search for other
 * matching files. The specified `mask_flags` should be set depending on
 * context like:
 * - `#include ...`:             `TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED`
 * - `__has_include(...)`:       `TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED`
 * - `#include_next ...`:        `TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED | TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT`
 * - `__has_include_next(...)`:  `TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED | TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT`
 * - `#import ...`:              `TPP_LEXER_OPENFILE_FLAG_HDR_ONCE | TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED | TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED`
 * - `#embed ...`:               `TPP_LEXER_OPENFILE_FLAG_NORMAL`
 * - `__has_embed(...)`:         `TPP_LEXER_OPENFILE_FLAG_NORMAL`
 *
 * @return: TPP_EOK:     Success
 * @return: TPP_ENOMEM:  Insufficient memory
 * @return: TPP_ENOENT:  No such file (no warning printed, yet)
 * @return: TPP_EMASKED: (tpp_lexer_open_include_string_ex only): Flags
 *                       specified by `mask_flags` were already set. */
#if TPP_HAVE_LEXER_OPENFILE_EX
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_open_include_string_ex(tpp_lexer *tpp_restrict self,
                                 /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result,
                                 tpp_lexer_openfile_flags mask_flags);
#define tpp_lexer_open_include_string(self, result) \
	tpp_lexer_open_include_string_ex(self, result, 0)
#else /* TPP_HAVE_LEXER_OPENFILE_EX */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_open_include_string(tpp_lexer *tpp_restrict self,
                              /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result);
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
#endif /* TPP_HAVE_LEXER_OPEN_INCLUDE_STRING */



#if TPP_HAVE_LEXER_OPEN_EMBED_STRING
/* Enumerate `#embed`-paths according to `mode` (s.a. `tpp_lexer_foreach_include_path()`) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_errno TPPCALL
tpp_lexer_foreach_embed_path(tpp_lexer *tpp_restrict self, tpp_token_id mode,
                             tpp_errno (TPPCALL *cb)(void *arg, char const *relative_to),
                             void *arg);

/* Open an `#embed`-file (s.a. `tpp_lexer_open_include_string()`) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_open_embed_string(tpp_lexer *tpp_restrict self,
                            /*1..1*/ tpp_lexer_openfile_result *tpp_restrict result);
#elif TPP_HAVE_LEXER_OPEN_INCLUDE_STRING
#define tpp_lexer_open_embed_string(self, result) \
	tpp_lexer_open_include_string(self, result)
#endif /* ... */




#if TPP_HAVE_LEXER_SKIP
/* Check that the currently loaded token is `tok`. If so, `tpp_lexer_yield_blocking()` to
 * the next token (which is also returned). Otherwise, trigger 'TPP_W_UNEXPECTED_TOKEN'
 * and (if that warning wasn't fatal), try to seek ahead to see if `tok` can be found
 * somewhere close by (depending on what `tok` and what was actually loaded on entry)
 *
 * NOTE: This function automatically handles `TPP_TOK_EWOULDBLOCK`
 *
 * @return: * :                The token that comes after the one that was just skipped
 * @return: TPP_TOK_ENOMEM:    Out of memory
 * @return: TPP_TOK_EIO:       I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR: Lexer error
 * @return: TPP_TOK_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip(tpp_lexer *tpp_restrict self, tpp_token_id tok);

/* Same as `tpp_lexer_skip()`, but don't advance to the next token,
 * except in those cases where the requested `tok` could be found
 * a little further up ahead, and the implementation decided that
 * the tokens that lay in-between should be skipped.
 *
 * @return: * :                The currently loaded token
 * @return: tok:               Success
 * @return: TPP_TOK_ENOMEM:    Out of memory
 * @return: TPP_TOK_EIO:       I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR: Lexer error
 * @return: TPP_TOK_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_require(tpp_lexer *tpp_restrict self, tpp_token_id tok);
#endif /* TPP_HAVE_LEXER_SKIP */



#if TPP_HAVE_LEXER_TRYSKIP_RAW
#define TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL      0x0000 /* Normal flags */
#define TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONSPACE 0x0001 /* Rollback if a NOLINE-COMMENT or SPACE token is hit */
#define TPP_LEXER_TRYSKIP_RAW_FLAG_STOPONLF    0x0002 /* Rollback if a LINE-COMMENT or LF token is hit */
#define TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV    0x0004 /* On success, include the previous token in the selected text-area, too */

/* Make use of:
 * - `tpp_lexer_seek_start()`
 * - `tpp_lexer_yieldraw_at()`
 * - `tpp_lexer_manualpopfile_start(self)`
 * to seek ahead to the next token, skipping whitespace/line-feed (+resp. comments)
 * based on `flags`, check if said *next token* is equal to `expected` (with some extra-
 * extra handling when `TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS && expected == '<'`).
 * If that is the case, commit the lexer such that it points at a token equal to
 * the specified `expected`, truly disposing of any files popped in the mean-time.
 * Otherwise (the next token is `!= expected`), roll back any changes made, such
 * that `self` once again points at the same token it did upon entry. In either
 * case, return the ID of whatever token came next.
 *
 * NOTE: This function automatically handles `TPP_TOK_EWOULDBLOCK` by blocking!
 *
 * @return: * :                The next token (rollback)
 * @return: expected:          The next token (commit; iow: this is now also the current token)
 * @return: TPP_TOK_ENOMEM:    Out of memory
 * @return: TPP_TOK_EIO:       I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR: Lexer error
 * @return: TPP_TOK_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_tryskip_raw(tpp_lexer *tpp_restrict self, tpp_token_id expected,
                      unsigned int flags);
#endif /* TPP_HAVE_LEXER_TRYSKIP_RAW */



#if TPP_HAVE_LEXER_SEEKPP_RPAREN

#define TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL     0x0000
#define TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS    0x0001 /* Store varargs info in p_argv[IN(*p_argc) - 1] */
#if TPP_CONF_ISRT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
#define TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC 0x0002 /* Do not strip whitespace/comments around arguments */
#endif /* TPP_CONF_ISRT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_ISRT(TPP_HAVE_MAGIC_WHITESPACE)
#define TPP_LEXER_SEEK_RPAREN_FLAG_MAGIC_WHITESPACE 0x0004 /* Add extra whitespace during across EOF when necessary */
#endif /* TPP_CONF_ISRT(TPP_HAVE_MAGIC_WHITESPACE) */
#if TPP_HAVE_LEXER_MANUALPOPFILE
#define TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK    0x0008 /* Use `tpp_lexer_manualpopfile_popfile()` to pop files */
#endif /* TPP_HAVE_LEXER_MANUALPOPFILE */
#define TPP_LEXER_SEEK_RPAREN_FLAG_NOWARNEOF  0x0010 /* Do not emit `TPP_W_EOF_IN_ARGUMENT_LIST` warnings */

typedef struct tpp_lexer_arginfo {
	/* NOTE: Leading/trailing whitespace in arguments is controlled by `TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC` */
	tpp_char const     *tlai_start;  /* [1..1][<= tlai_end] Pointer to argument start text data */
	tpp_char const     *tlai_end;    /* [1..1][>= tlai_start] Pointer to argument end text data */
	TPP_REF tpp_string *tlai_chunk;  /* [0..1] Chunk of text containing [tlai_start,tlai_end), or `NULL` if statically allocated */
	/* XXX: Come up with a smart way of tracking debug info for custom printed arguments
	 *      -> need to be able to track lcinfo for custom char ranges (any range of chars
	 *         from this string must be able to map to its own file/line/col triple)
	 *      -> also must adjust tpp_file_getlcinfo() to support this, and somehow also
	 *         incorporate tpp_file_getrealfilename()/tpp_file_getfilename() to support
	 *         different filenames based on char position
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
} tpp_lexer_arginfo;

#define tpp_lexer_arginfo_init_empty(self) \
	(void)((self)->tlai_chunk = NULL, (self)->tlai_start = (self)->tlai_end = NULL)
#define tpp_lexer_arginfo_fini(self)                                            \
	((void)(!(self)->tlai_chunk || (tpp_string_decref((self)->tlai_chunk), 0)), \
	 tpp_dbg_memset(self, sizeof(*(self))))
#define tpp_lexer_arginfo_copy(dst, src) \
	(void)(*(dst) = *(src), (!(self)->tlai_chunk || (tpp_string_incref((self)->tlai_chunk), 0)))
#define tpp_lexer_arginfo_move(dst, src) \
	(void)(*(dst) = *(src), tpp_dbg_memset(src, sizeof(*(src))))

/* Seek the first unmatched `)`-token, whilst collecting information
 * about every `,`-separated text-area encountered until then.
 *
 * NOTES:
 *  - This function is used to parse the argument list for user-defined
 *    macros, as well as a couple of built-in macros.
 *  - This function preserves the effective `tf_tpos` (aka. tpp_token_getstart())
 *    of the final output file (and when using `tpp_lexer_manualpopfile_start()`:
 *    all intermediate popped files also)
 *
 * @param: p_argv: [out]    Output buffer for the bounds of macro
 *                          arguments encountered along the way.
 *                          The size of this buffer is `IN(*p_argc)`
 * @param: p_argc: [in/out] In:  Size of provided `p_argv` buffer (in elements)
 *                          Out: Number of arguments actually encountered. May
 *                               be set to a number greater tha `IN(*p_argc)`, in
 *                               which case the last argument (`IN(*p_argc) - 1`)
 *                               is treated as a varargs argument. Unless the
 *                               `TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS` flag is
 *                               given in this case, this also causes a warning
 *                               to be emitted.
 * @param: p_rollback_pos:  [out] Set to the position that the current file should
 *                                be rewound to for the sake of performing a rollback
 * @param: opt_function_name_for_messages:
 *                          Function name for warning messages
 * @param: flags:           Set of `TPP_LEXER_SEEK_RPAREN_FLAG_*`
 *
 * @return: TPP_TOK_EOF:       EOF was encountered before an unmatched `)` was found
 * @return: TPP_TOK_RPAREN:    Unmatched closing `)` was encountered
 * @return: TPP_TOK_ENOMEM:    Out of memory
 * @return: TPP_TOK_EIO:       I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR: Lexer error
 * @return: TPP_TOK_EUSER(*):  User-defined error from hook */
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

/* Same as above, but always initializes *exactly* `argc` arguments,
 * and automatically emits `TPP_W_TOO_FEW_ARGUMENTS` when fewer were
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




#if TPP_HAVE_LEXER_GETKEYWORDFEATURE || TPP_HAVE_CPP_PREDEFINED_MACROS
typedef struct tpp_macro_expansion {
	TPP_REF tpp_string *TPP_INTERNAL(tme_chunk); /* [0..1] Expansion chunk buffer (or `NULL` if expansion is statically allocated) */
	tpp_char const     *TPP_INTERNAL(tme_start); /* [1..1] Start of expansion text */
	tpp_char const     *TPP_INTERNAL(tme_end);   /* [1..1] End of expansion text */
} tpp_macro_expansion;

#define tpp_macro_expansion_init_chunk_ex_inherited(self, chunk, text, text_size) \
	(void)((self)->TPP_INTERNAL(tme_chunk) = (chunk),                             \
	       (self)->TPP_INTERNAL(tme_start) = (text),                              \
	       (self)->TPP_INTERNAL(tme_end)   = (text) + (text_size))
#define tpp_macro_expansion_init_chunk_ex(self, chunk, text, text_size) \
	(tpp_string_incref(chunk), tpp_macro_expansion_init_chunk_ex_inherited(self, chunk, text, text_size))
#define tpp_macro_expansion_init_chunk_inherited(self, chunk) \
	tpp_macro_expansion_init_chunk_ex_inherited(self, chunk, tpp_string_str(chunk), tpp_string_len(chunk))
#define tpp_macro_expansion_init_chunk(self, chunk) \
	(tpp_string_incref(chunk), tpp_macro_expansion_init_chunk_inherited(self, chunk))
#define tpp_macro_expansion_init_cstr(self, text, text_size) \
	tpp_macro_expansion_init_chunk_ex_inherited(self, NULL, (tpp_char const *)(text), text_size)
#define tpp_macro_expansion_init_conststr(self, CONSTstr) \
	tpp_macro_expansion_init_cstr(self, CONSTstr, (sizeof(CONSTstr) - sizeof(char)))
#define tpp_macro_expansion_fini(self)                    \
	((self)->TPP_INTERNAL(tme_chunk)                      \
	 ? tpp_string_decref((self)->TPP_INTERNAL(tme_chunk)) \
	 : (void)0)

#define tpp_macro_expansion_getchunk(self) (self)->TPP_INTERNAL(tme_chunk)
#define tpp_macro_expansion_getstart(self) (self)->TPP_INTERNAL(tme_start)
#define tpp_macro_expansion_getend(self)   (self)->TPP_INTERNAL(tme_end)
#define tpp_macro_expansion_gettext(self)  (self)->TPP_INTERNAL(tme_start)
#define tpp_macro_expansion_getsize(self) \
	((tpp_size)((self)->TPP_INTERNAL(tme_end) - (self)->TPP_INTERNAL(tme_start)))
#endif /* TPP_HAVE_LEXER_GETKEYWORDFEATURE || TPP_HAVE_CPP_PREDEFINED_MACROS */


#if TPP_HAVE_LEXER_GETKEYWORDFEATURE
/* Return the effective expansion of a feature-keyword `kwd`.
 * @param: feature_kind: One of `TPP_KWD___has_attribute`, `TPP_KWD___has_feature`, etc.
 * @return: TPP_EOK:    Success: expansion was stored in `result`
 * @return: TPP_ENOENT: SOFT_ERROR: keyword has no expansion (caller should expand to `0` instead)
 * @return: TPP_E*:     HARD_ERROR: error returned by a user-defined feature-test callback. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 4)) tpp_errno TPPCALL
tpp_lexer_getkeywordfeature(tpp_lexer *tpp_restrict self,
                            tpp_keyword const *tpp_restrict kwd,
                            tpp_token_id feature_kind,
                            tpp_macro_expansion *tpp_restrict result);
#endif /* TPP_HAVE_LEXER_GETKEYWORDFEATURE */


#if TPP_HAVE_CPP_PREDEFINED_MACROS
/* Return the effective expansion of a predefined keyword-style macro.
 * @return: TPP_EOK:    Success: expansion was stored in `result`
 * @return: TPP_ENOENT: SOFT_ERROR: keyword has no expansion
 * @return: TPP_E*:     HARD_ERROR: error returned by a user-defined feature-test callback. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_getpredefinedmacro(tpp_lexer *tpp_restrict self,
                             tpp_keyword const *tpp_restrict kwd,
                             tpp_macro_expansion *tpp_restrict result);
#endif /* TPP_HAVE_CPP_PREDEFINED_MACROS */


#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
/* Returns true if `kwd` should be considered to be `#if defined()`
 * Does all the handling necessary to determine the correct state of
 * builtin/predefined macros. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_getkeyworddefined(tpp_lexer *tpp_restrict self,
                            tpp_keyword const *tpp_restrict kwd);
#elif TPP_HAVE_CPP_MACROS
#define tpp_lexer_getkeyworddefined(self, kwd) tpp_keyword_canundef(kwd)
#else /* ... */
#define tpp_lexer_getkeyworddefined(self, kwd) 0
#endif /* !... */


#if TPP_HAVE_LEXER_ISIDENTIFIER
/* Returns true if `kwd` should is considered a `__is_identifier()`
 *
 * When that is the case, `__is_identifier()` expands to `1` (rather
 * than `0`) for that keyword, and the user attempting to define a
 * macro of the same name triggers a `-Wkeyword-macro` warning. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_isidentifier(tpp_lexer *tpp_restrict self,
                       tpp_keyword const *tpp_restrict kwd);
#elif TPP_HAVE_LEXER_ISIDENTIFIER_DEFAULT
#define tpp_lexer_isidentifier(self, kwd) tpp_keyword_isbuiltin(kwd)
#else /* ... */
#define tpp_lexer_isidentifier(self, kwd) 0
#endif /* !... */


#if TPP_HAVE_LEXER_DECODEINT
/* Decode the current token (which should be `TPP_TOK_ISNUMBER`) into an integer
 * When the current token is `TPP_TOK_ISFLOAT`, it will be trimmed (i.e. decimal
 * and exponents will be removed from the token, such that the characters that
 * make up those parts of the token will be yielded next)
 *
 * @param: p_suffix_start: When non-NULL, store a pointer to the first character
 *                         of the int token's suffix (e.g. `ull` in `123ull`).
 *                         Note that when `TPP_HAVE_BSE` is enabled, you will
 *                         have to skip over `\`-sequences yourself. However, TPP
 *                         will ensure that pointer written here points *after*
 *                         a potential BSE between the int and the suffix. The
 *                         suffix always ends at `tpp_lexer_gettokenend(self)`,
 *                         and if there is no suffix, this function will store
 *                         a pointer to `tpp_lexer_gettokenend(self)` instead.
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_ex(tpp_lexer *tpp_restrict self,
                       tpp_intmax *tpp_restrict result,
                       tpp_char const **p_suffix_start);
#define tpp_lexer_decodeint(self, result) \
	tpp_lexer_decodeint_ex(self, result, NULL)
#endif /* TPP_HAVE_LEXER_DECODEINT */

/* Decode the current token (which should be TPP_TOK_ISINT) into an integer
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EUSER(*):  User-defined error from hook */
#if TPP_HAVE_LEXER_DECODEINT_EXPR
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodeint_expr(tpp_lexer *tpp_restrict self,
                         tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE_LEXER_DECODEINT_EXPR */


#if TPP_HAVE_LEXER_DECODEFLOAT
/* Decode the current token (which should be `TPP_TOK_ISNUMBER`) into a float.
 * When the current token is `TPP_TOK_ISINT`, it will be parsed as integer
 * first, and that integer will be converted into the equivalent `tpp_float`.
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat_ex(tpp_lexer *tpp_restrict self,
                         tpp_float *tpp_restrict result,
                         tpp_char const **p_suffix_start);
#define tpp_lexer_decodefloat(self, result) \
	tpp_lexer_decodefloat_ex(self, result, NULL)
#endif /* TPP_HAVE_LEXER_DECODEFLOAT */

/* Decode the current token (which should be TPP_TOK_ISFLOAT) into a float
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Lexer error happened
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EUSER(*):  User-defined error from hook */
#if TPP_HAVE_LEXER_DECODEFLOAT_EXPR
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_decodefloat_expr(tpp_lexer *tpp_restrict self,
                           tpp_expr_value *tpp_restrict result);
#endif /* TPP_HAVE_LEXER_DECODEFLOAT_EXPR */





#if TPP_HAVE_LEXER_DECODESTRING
typedef struct tpp_lexer_decodestring_config {
	tpp_formatprinter   tldsc_dataprinter; /* [1..1] Printer for 1-byte-per-character data.
	                                        * When output format has multiple bytes per character,
	                                        * every byte passed to this printer must result in 1
	                                        * character being appended to the output string:
	                                        * Input: `"foo"`
	                                        * -> `tldsc_dataprinter("foo")`
	                                        * -> `XCHAR[3]{ 0x66, 0x6f, 0x6f }`
	                                        *
	                                        * Input: `"\x12"`
	                                        * -> `tldsc_dataprinter("\x12")`
	                                        * -> `XCHAR[1]{ 0x12 }`
	                                        *
	                                        * If the input file is `!tpp_file_isutf8`, then this printer
	                                        * is also used for non-ASCII bytes. However, in this case any
	                                        * such non-ascii byte shouldn't be considered part of any sort
	                                        * of unicode sequence either, so that's intended:
	                                        * Input: `"Stra[C3,9F]e"`
	                                        * -> tldsc_dataprinter("Stra\xC3\x{9F}e")
	                                        * -> `XCHAR[7]{ 0x53, 0x74, 0x72, 0x61, 0xc3, 0x9f, 0x65 }`
	                                        */
#if TPP_HAVE_UNICODE
	tpp_formatprinter   tldsc_utf8printer; /* [1..1] Printer for utf-8 formatted input data.
	                                        * Very similar to `tldsc_dataprinter`, especially in that
	                                        * ASCII data may either be passed to `tldsc_dataprinter` or
	                                        * this callback (behavior in both cases must be the same),
	                                        * except that whereas `tldsc_dataprinter` must add 1 character
	                                        * to the output string for every byte in the given data-buffer,
	                                        * this function treats bytes in range [0x80,0xff] special, in
	                                        * that it must treat such bytes as initiating a utf-8 character
	                                        * sequences that may span multiple bytes.
	                                        *
	                                        * How exactly such a multi-byte utf-8 sequence is then encoded
	                                        * in the actual output string (i.e. in terms of `XCHAR`s) is
	                                        * not mandated, and is left up to the implementation. However,
	                                        * here are some suggestions:
	                                        * -> `tldsc_utf8printer("\xC3\x9F")`
	                                        *    -> `XCHAR[2]{ 0xC3, 0x9F }`  (Keep utf-8 encoding in output)
	                                        *    -> `XCHAR[1]{ 0x00DF }`      (Use UCS2 encoding in output)
	                                        *
	                                        * This printer is *always* for the following 2 cases:
	                                        * - Input: `"Straße"`    (when `tpp_file_isutf8()` and a non-ASCII sequence is encountered)
	                                        * - Input: `"\u00DF"`    (here, `tldsc_utf8printer("\xC3\x9F")` is called with the utf-8 representation)
	                                        *
	                                        * The implementation is however also allowed to use this callback
	                                        * for ASCII-only input data! */
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
	tpp_ssize (TPPCALL *tldsc_bigprinter)(void *arg, tpp_lexer *tpp_restrict lexer, tpp_uintmax value);
	                                     /* [0..1] Printer for [2+]-byte-per-character data. When non-NULL,
	                                      * and `\x1234` is used, this printer is called with `0x1234` as
	                                      * value, and it should append `XCHAR[1]{0x1234}` to the output
	                                      * string. If the given `value` is too large to fit `XCHAR`, then
	                                      * this callback should emit a warning:
	                                      * >> tpp_errno error = tpp_lexer_warnf(lexer, TPP_W_CHARACTER_TOO_LARGE);
	                                      * >> return TPP_SSIZE_ASERR_OR_EOK(error); */
#endif /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
	void               *tldsc_arg;       /* [?..?] Cookie argument for other printers */
} tpp_lexer_decodestring_config;

/* Initialize a simple decodestring configuration (suitable for emitting utf-8 data) */
#define tpp_lexer_decodestring_config_init_simple(self, printer, arg)    \
	(_tpp_lexer_decodestring_config_init_simple_base(self, printer, arg) \
	 _tpp_lexer_decodestring_config_init_simple_big(self))
#if TPP_HAVE_UNICODE
#define _tpp_lexer_decodestring_config_init_simple_base(self, printer, arg) \
	(self)->tldsc_dataprinter = (self)->tldsc_utf8printer = (printer),      \
	(self)->tldsc_arg = (arg)
#else /* TPP_HAVE_UNICODE */
#define _tpp_lexer_decodestring_config_init_simple_base(self, printer, arg) \
	(self)->tldsc_dataprinter = (printer),                                  \
	(self)->tldsc_arg = (arg)
#endif /* !TPP_HAVE_UNICODE */
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
#define _tpp_lexer_decodestring_config_init_simple_big(self) , (self)->tldsc_bigprinter = NULL
#else /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
#define _tpp_lexer_decodestring_config_init_simple_big(self) /* nothing */
#endif /* !TPP_HAVE_STRING_ESCAPE_BIGCHAR */

/* Print the unescaped representation of the string-token described by `self`
 * The caller must ensure that `TPP_TOK_ISSTRING(tpp_lexer_gettoken(self)->tt_id)`
 *
 * @param: config: Printer configuration
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): Either one of the printers returned this value, or
 *                                          a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory  (can only happen inside of `tpp_lexer_warnf()`)
 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_decodestring(tpp_lexer *tpp_restrict self,
                       tpp_lexer_decodestring_config const *tpp_restrict config);

/* Flags for `tpp_lexer_parsestring()` & friends. */
#define TPP_LEXER_PARSESTRING_FLAG_NORMAL      0x0000 /* Normal flags */
#if TPP_HAVE_STRING_AUTO_CONCAT
#define TPP_LEXER_PARSESTRING_FLAG_STOPONSPACE 0x0001 /* Stop if a NOLINE-COMMENT or SPACE token is hit */
#define TPP_LEXER_PARSESTRING_FLAG_STOPONLF    0x0002 /* Stop if a LINE-COMMENT or LF token is hit */
#endif /* TPP_HAVE_STRING_AUTO_CONCAT */
#if TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS
#define TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS  0x0004 /* For `tpp_lexer_parsestring_cb()` only: allow `cb` to be called with a stack-allocated
                                                       * buffers (and a valid, but unrelated, possibly `NULL`-`chunk` argument) when decoding a
                                                       * string like `"\xFF"`. When this flag it's set, TPP will be forced to allocate a fresh
                                                       * heap-tpp_string for such tokens, which will be passed to `cb` instead.
                                                       *
                                                       * This flag may be set to speed up callbacks that don't `tpp_string_incref()` the given
                                                       * `chunk` argument, but is otherwise optional. */
#else /* TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS */
#define TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS  0x0000 /* No-op */
#endif /* !TPP_HAVE_LEXER_PARSESTRING_FLAG_ALLOWTEMPS */

/* Same as `tpp_lexer_decodestring()`, but also `tpp_lexer_yield()` to the next token.
 * Then, if that token is also string-like (`TPP_TOK_ISSTRING()`), decode it also,
 * then yield again, and so on, until a non-string-like token is encountered, an
 * error happens, or one of the printers returned a negative value.
 *
 * HINT: This function automatically handles `TPP_EWOULDBLOCK` during
 *       yield by trying again with TPP_FILE_FLAGS_NONBLOCK disabled.
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: * :  Sum of positive return values from printers
 * @return: < 0: First negative return value from printers
 * @return: TPP_SSIZE_OFERR(TPP_ELEXERROR): Either one of the printers returned this value, or
 *                                          a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM):    Out of memory
 * @return: TPP_SSIZE_OFERR(TPP_EIO):       I/O error while yielding to next token
 * @return: TPP_SSIZE_OFERR(TPP_EUSER(*)):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_parsestring_ex(tpp_lexer *tpp_restrict self,
                         tpp_lexer_decodestring_config const *tpp_restrict config,
                         unsigned int flags);

/* Convenience wrapper around `tpp_lexer_parsestring_ex()`
 * On success (`!TPP_ISERR(return)`), caller must `tpp_string_decref(*p_result)`
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Either one of the printers returned this value, or
 *                         a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O error while yielding to next token
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring(tpp_lexer *tpp_restrict self,
                      /*out*/ TPP_REF tpp_string **tpp_restrict p_result,
                      unsigned int flags);

/* Wrapper around `tpp_lexer_parsestring()` that passes the actual string data
 * to a given callback. This function also enables some (optional) optimizations
 * for the most common case where the string token in `self` isn't followed by
 * another string token, and can be printed as a singular chunk. When this is
 * the case, no intermediate heap-buffer needs to be created, as `cb` can just
 * be invoked using the currently loaded file's content-buffer.
 *
 * @param: cb.arg:   Cookie argument (s.a. `arg`)
 * @param: cb.chunk: The string-chunk containing `str` (or `NULL` if `str` is statically allocated)
 *                   NOTE: May be non-NULL, even if `str` is statically allocated!
 * @param: flags:    Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Either one of the printers returned this value, or
 *                         a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O error while yielding to next token
 * @return: TPP_EUSER(*):  User-defined error from hook
 * @return: * :            Return value of given `cb` */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring_cb(tpp_lexer *self,
                         tpp_errno (TPPCALL *cb)(void *arg, tpp_string *chunk,
                                                 tpp_char const *str, tpp_size length),
                         void *arg, unsigned int flags);
#endif /* TPP_HAVE_LEXER_DECODESTRING */

#if TPP_HAVE_LEXER_PARSECHARACTER_LITERAL
/* Convenience wrapper to parse a character integer literal
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Either one of the printers returned this value, or
 *                         a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O error while yielding to next token
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsecharacter_literal(tpp_lexer *tpp_restrict self,
                                 /*out*/ tpp_uintmax *tpp_restrict p_result,
                                 unsigned int flags);
#endif /* TPP_HAVE_LEXER_PARSECHARACTER_LITERAL */

#if TPP_HAVE_LEXER_PARSESTRING_EXPR
/* Convenience wrapper around `tpp_lexer_parsestring()`
 * On success (`!TPP_ISERR(return)`), caller must `tpp_expr_value_fini(result)`
 *
 * @param: flags: Set of `TPP_LEXER_PARSESTRING_FLAG_*`
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Either one of the printers returned this value, or
 *                         a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O error while yielding to next token
 * @return: TPP_EUSER(*):  User-defined error from hook */
#ifndef tpp_lexer_parsestring_expr
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsestring_expr(tpp_lexer *tpp_restrict self,
                           /*out*/ tpp_expr_value *tpp_restrict result,
                           unsigned int flags);
#endif /* !tpp_lexer_parsestring_expr */
#endif /* TPP_HAVE_LEXER_PARSESTRING_EXPR */



#if TPP_HAVE_LEXER_PARSECHARACTER_EXPR
/* Convenience wrapper around `tpp_lexer_parsecharacter()`
 * On success (!TPP_ISERR(return)), caller must `tpp_expr_value_fini(result)`
 *
 * @param: flags: Set of `TPP_LEXER_PARSECHARACTER_FLAG_*`
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ELEXERROR: Either one of the printers returned this value, or
 *                         a lexer error happened (s.a. `tpp_lexer_warnf()`).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O error while yielding to next token
 * @return: TPP_EUSER(*):  User-defined error from hook */
#ifndef tpp_lexer_parsecharacter_expr
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_parsecharacter_expr(tpp_lexer *tpp_restrict self,
                              /*out*/ tpp_expr_value *tpp_restrict result,
                              unsigned int flags);
#endif /* !tpp_lexer_parsecharacter_expr */
#endif /* TPP_HAVE_LEXER_PARSECHARACTER_EXPR */



#if TPP_HAVE_WARNINGS
typedef struct tpp_lexer_printf_info {
	tpp_file       *tlpfi_file;     /* [0..1] Current file (source for filename, and basis for `tlpfi_pos`) */
	tpp_char const *tlpfi_pos;      /* [0..1][valid_if(tlpfi_file != NULL)] Current position in `tlpfi_file` */
	char const     *tlpfi_filename; /* [0..1] Filename used by `%Pf`, or `NULL` if `tlpfi_file` must be used */
	tpp_lcinfo      tlpfi_lc;       /* L/C info to use, or `TPP_LCINFO_INVALID` if `tlpfi_pos` must be used */
} tpp_lexer_printf_info;

#define tpp_lexer_printf_info_init_at(self, file, pos) \
	(void)((self)->tlpfi_file     = (file),            \
	       (self)->tlpfi_pos      = (pos),             \
	       (self)->tlpfi_filename = NULL,              \
	       tpp_lcinfo_init_invalid(&(self)->tlpfi_lc))
#define tpp_lexer_printf_info_init_lc(self, filename, lc) \
	(void)((self)->tlpfi_file     = NULL,                 \
	       (self)->tlpfi_filename = (filename),           \
	       (self)->tlpfi_lc       = (lc))

/* Interpret + print a warning-message `format` string.
 * The following %-encoded escape sequences are recognized:
 * - `%[`    Start quoting text
 * - `%]`    Stop quoting text
 * - `%Pl`   1-based line described by `info`
 * - `%Pc`   1-based column described by `info`
 * - `%Pf`   Filename described by `info`
 * - `%Pt`   `%[current-token%]`   (based on tpp_lexer_gettoken(self))
 * - `%s`    As defined by stdc, using va_arg(args, char *)
 * - `%.*s`  As defined by stdc, using va_arg(args, int) + va_arg(args, char *)
 * - `%.Ns`  As defined by stdc, using va_arg(args, char *)
 * - `%.NPt` `%[<N bytes starting at *info->tlpfi_pos*>%]`   (clamped if too big)
 * - `%d`    As defined by stdc, using va_arg(args, int)
 * - `%u`    As defined by stdc, using va_arg(args, unsigned int)
 * - `%c`    As defined by stdc, using va_arg(args, int)
 * - `%%`    `%` (emit a singular %-character)
 *
 * @param: info:    Information for special format descriptors
 *                  (unpopulated parts may be populated lazily)
 * @param: printer: Output printer for formatted text
 * @param: arg:     Cookie argument for `printer`
 * @param: format:  Format pattern (see above)
 * @param: args:    Extra varargs-arguments for `format`
 * @return: >= 0:   Sum of return values of `printer`
 * @return: < 0:    First negative return value of `printer` */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 5)) tpp_ssize TPPVCALL
tpp_lexer_printf_warning(tpp_lexer const *self, tpp_lexer_printf_info *info,
                         tpp_formatprinter printer, void *arg,
                         char const *format, ...);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 5)) tpp_ssize TPPCALL
tpp_lexer_vprintf_warning(tpp_lexer const *self, tpp_lexer_printf_info *info,
                          tpp_formatprinter printer, void *arg,
                          char const *format, va_list args);

/* Print the actual warning message (and only the message, including
 * its trailing linefeed) to `printer`. When `id` uses `TPP_WARNING_EX`,
 * the warning's printer callback is invoked with the relevant parameters
 *
 * @return: >= 0: Success (sum of return values of `printer`)
 * @return: < 0:  Failure (first negative return value of `printer`)
 * @return: TPP_SSIZE_OFERR(*): A `TPP_WARNING_EX` returned something other than `TPP_EOK` */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_lexer_vwarnf_mesg(tpp_lexer *tpp_restrict self,
                      tpp_lexer_printf_info *tpp_restrict info,
                      tpp_formatprinter printer, void *arg,
                      tpp_warning_id id, va_list args);

/* Emits the specified lexer warning at the start of the current token.
 * @param: args: Format arguments specific to `id` (see `%`-sequences in warning expressions)
 * @return: TPP_EOK:       Warning was emitted, but you may proceed
 * @return: TPP_ELEXERROR: Warning was emitted, but was configured as an error
 * @return: TPP_EUSER(*):  Error while printing warning */
TPP_DECL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, va_list args);
TPP_DECL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, ...);
TPP_DECL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_at(tpp_lexer *tpp_restrict self, tpp_file *file,
                    tpp_char const *pos, tpp_warning_id id, va_list args);
TPP_DECL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_file *file,
                   tpp_char const *pos, tpp_warning_id id, ...);
TPP_DECL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_vwarnf_lc(tpp_lexer *tpp_restrict self, char const *filename,
                    tpp_lcinfo lc, tpp_warning_id id, va_list args);
TPP_DECL TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPVCALL
tpp_lexer_warnf_lc(tpp_lexer *tpp_restrict self, char const *filename,
                   tpp_lcinfo lc, tpp_warning_id id, ...);
#else /* TPP_HAVE_WARNINGS */
#define tpp_lexer_vwarnf(self, id, args)                  TPP_EOK
#define tpp_lexer_vwarnf_at(self, file, pos, id, args)    TPP_EOK
#define tpp_lexer_vwarnf_lc(self, filename, lc, id, args) TPP_EOK
#if TPP_HOST_HAVE_PP_VARARGS
#define tpp_lexer_warnf(self, id, ...)                    TPP_EOK
#define tpp_lexer_warnf_at(self, file, pos, id, ...)      TPP_EOK
#define tpp_lexer_warnf_lc(self, filename, lc, id, ...)   TPP_EOK
#else /* TPP_HOST_HAVE_PP_VARARGS */
TPP_INLINE TPP_COLDCALL tpp_errno TPPVCALL
tpp_lexer_warnf(tpp_lexer *tpp_restrict self, tpp_warning_id id, ...) {
	(void)self;
	(void)id;
	return TPP_EOK;
}

TPP_INLINE TPP_COLDCALL tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, tpp_file *file,
                   tpp_char const *pos, tpp_warning_id id, ...) {
	(void)self;
	(void)file;
	(void)pos;
	(void)id;
	return TPP_EOK;
}

TPP_INLINE TPP_COLDCALL tpp_errno TPPVCALL
tpp_lexer_warnf_at(tpp_lexer *tpp_restrict self, char const *filename,
                   tpp_lcinfo lc, tpp_warning_id id, ...) {
	(void)self;
	(void)filename;
	(void)lc;
	(void)id;
	return TPP_EOK;
}
#endif /* !TPP_HOST_HAVE_PP_VARARGS */
#endif /* !TPP_HAVE_WARNINGS */


/* Warn if the current file's `#ifdef`-stack is non-empty.
 * @return: * : See `tpp_lexer_warnf()` */
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



/* Dump all user-defined macros and assertions to `printer`
 * @param: what: Set of `TPP_LEXER_DUMP_DEFINITIONS_*`
 * @return: * :  Sum of return values of `printer`
 * @return: < 0: First negative return value of `printer` */
#if TPP_HAVE_LEXER_DUMP_DEFINITIONS
TPP_DECL TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_dump_definitions(tpp_lexer *tpp_restrict self,
                           tpp_formatprinter printer, void *arg,
                           unsigned int what);
#if TPP_HAVE_CPP_MACROS
#define TPP_LEXER_DUMP_DEFINITIONS_MACROS           0x0001 /* `#define foo bar` */
#else /* TPP_HAVE_CPP_MACROS */
#define TPP_LEXER_DUMP_DEFINITIONS_MACROS           0x0000 /* no-op */
#endif /* !TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_CPP_BUILTIN_MACROS
#define TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS   0x0002 /* `#define __LINE__ <magic>`, `#define __TPP_VERSION__ 300` (yes: also prints *predefined* macros) */
#else /* TPP_HAVE_CPP_BUILTIN_MACROS */
#define TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS   0x0000 /* no-op */
#endif /* !TPP_HAVE_CPP_BUILTIN_MACROS */
#if TPP_HAVE_CPP_ASSERT
#define TPP_LEXER_DUMP_DEFINITIONS_ASSERTS          0x0004 /* `#assert foo(bar)` */
#else /* TPP_HAVE_CPP_ASSERT */
#define TPP_LEXER_DUMP_DEFINITIONS_ASSERTS          0x0000 /* no-op */
#endif /* !TPP_HAVE_CPP_ASSERT */
#if TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES
#define TPP_LEXER_DUMP_DEFINITIONS_KEYWORD_FEATURES 0x0008 /* `#pragma TPP __has_feature(foo) = "123"` */
#else /* TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES */
#define TPP_LEXER_DUMP_DEFINITIONS_KEYWORD_FEATURES 0x0000 /* no-op */
#endif /* !TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES */
#if TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION
#define TPP_LEXER_DUMP_DEFINITIONS_EXTENSIONS       0x0010 /* `#pragma TPP extension("-ffoo")` -- Where different from default */
#else /* TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION */
#define TPP_LEXER_DUMP_DEFINITIONS_EXTENSIONS       0x0000 /* no-op */
#endif /* !TPP_HAVE_PRAGMA_EXTENSION && !TPP_HAVE_PRAGMA_TPP_EXTENSION */
#if TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING
#define TPP_LEXER_DUMP_DEFINITIONS_WARNINGS         0x0020 /* `#pragma TPP warning("-Wfoo")` -- Where different from default */
#else /* TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING */
#define TPP_LEXER_DUMP_DEFINITIONS_WARNINGS         0x0000 /* no-op */
#endif /* !TPP_HAVE_PRAGMA_WARNING && !TPP_HAVE_PRAGMA_TPP_WARNING */
#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
#define TPP_LEXER_DUMP_DEFINITIONS_INCLUDES         0x0040 /* `#pragma TPP include_path("/usr/include")` */
#else /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */
#define TPP_LEXER_DUMP_DEFINITIONS_INCLUDES         0x0000 /* no-op */
#endif /* !TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */
#define TPP_LEXER_DUMP_DEFINITIONS_ALL              0x0fff

#if TPP_HAVE_LEXER_DUMP_DEFINITIONS_SORTED && (TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_ASSERT)
#define TPP_LEXER_DUMP_DEFINITIONS_SORTED     0x1000 /* Sort macros/assertion-keys based on their name's first appearance */
#else /* TPP_HAVE_LEXER_DUMP_DEFINITIONS_SORTED && (TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_ASSERT) */
#define TPP_LEXER_DUMP_DEFINITIONS_SORTED     0x0000 /* no-op */
#endif /* !TPP_HAVE_LEXER_DUMP_DEFINITIONS_SORTED || (!TPP_HAVE_CPP_MACROS && !TPP_HAVE_CPP_ASSERT) */
#if TPP_HAVE_LEXER_DUMP_DEFINITIONS_EXTRAINFO
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO  0x8000 /* Include comments containing some extra info on macro definitions */
#else /* TPP_HAVE_LEXER_DUMP_DEFINITIONS_EXTRAINFO */
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO  0x0000 /* no-op */
#endif /* !TPP_HAVE_LEXER_DUMP_DEFINITIONS_EXTRAINFO */
#endif /* TPP_HAVE_LEXER_DUMP_DEFINITIONS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_H */
