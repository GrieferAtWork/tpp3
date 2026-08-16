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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_MF_H
#define GUARD_TPP_OPTIONAL_MAKEFILE_MF_H 1

#include "api.h"

#include "config.h"
#include "mf-io.h"
#include "mf-features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#undef TPP_MAKEFILE_HAVE_FLAGS
#if TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE
#define TPP_MAKEFILE_HAVE_FLAGS 1
#else /* ... */
#define TPP_MAKEFILE_HAVE_FLAGS 0
#endif /* !... */

#if TPP_MAKEFILE_HAVE_FLAGS
#define tpp_makefile_flags tpp_uint_least8
#define TPP_MAKEFILE_FLAG_NORMAL         TPP_UINT_LEAST8_C(0x00) /* Normal flags */
#if TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE
#define TPP_MAKEFILE_FLAG_OUTPUT_NOCLOSE TPP_UINT_LEAST8_C(0x01) /* Don't close an output file handle */
#endif /* TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE */
#endif /* TPP_MAKEFILE_HAVE_FLAGS */


typedef struct tpp_makefile {
	/* [1..1][const] The lexer whose tokens are being emitted */
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER)
	tpp_lexer *TPP_MAKEFILE_INTERNAL(tmkf_lexer);
#define _tpp_makefile_init_lexer(self, lexer) (self)->TPP_MAKEFILE_INTERNAL(tmkf_lexer) = (lexer)
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define _tpp_makefile_init_lexer(self, lexer) tpp_assert(tpp_makefile_getlexer(self) == (lexer))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */

	/* [1..1][const] Makefile output printer (the makefile itself will be passed as argument) */
	tpp_formatprinter TPP_MAKEFILE_INTERNAL(tmkf_output);

	/* [valid_if(tmkf_output == &_tpp_makefile_builtin_file_output)] Output-to-file information */
#if TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO
	tpp_makefile_io_handle TPP_MAKEFILE_INTERNAL(tmkf_output_file);
#define _tpp_makefile_init_output_file(self) /* nothing */
#if TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE
#define _tpp_makefile_fini_output_file_before_reassign(self)                                     \
	, (self)->TPP_MAKEFILE_INTERNAL(tmkf_output) == &_tpp_makefile_builtin_file_output            \
	  ? (((self)->TPP_MAKEFILE_INTERNAL(tmkf_flags) & TPP_MAKEFILE_FLAG_OUTPUT_NOCLOSE)           \
	     ? (void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_flags) &= ~TPP_MAKEFILE_FLAG_OUTPUT_NOCLOSE) \
	     : tpp_makefile_io_close((self)->TPP_MAKEFILE_INTERNAL(tmkf_output_file)))                \
	  : (void)0
#define _tpp_makefile_fini_output_file(self)                                              \
	, ((self)->TPP_MAKEFILE_INTERNAL(tmkf_output) == &_tpp_makefile_builtin_file_output && \
	   !((self)->TPP_MAKEFILE_INTERNAL(tmkf_flags) & TPP_MAKEFILE_FLAG_OUTPUT_NOCLOSE))    \
	  ? tpp_makefile_io_close((self)->TPP_MAKEFILE_INTERNAL(tmkf_output_file))             \
	  : (void)0
#else /* TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE */
#define _tpp_makefile_fini_output_file(self)                                            \
	, ((self)->TPP_MAKEFILE_INTERNAL(tmkf_output) == &_tpp_makefile_builtin_file_output) \
	  ? tpp_makefile_io_close((self)->TPP_MAKEFILE_INTERNAL(tmkf_output_file))           \
	  : (void)0
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE */
#else /* TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO */
#define _tpp_makefile_init_output_file(self) /* nothing */
#define _tpp_makefile_fini_output_file(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO */
#ifndef _tpp_makefile_fini_output_file_before_reassign
#define _tpp_makefile_fini_output_file_before_reassign _tpp_makefile_fini_output_file
#endif /* !_tpp_makefile_fini_output_file_before_reassign */

	/* Makefile feature configuration */
#if TPP_MAKEFILE_HAVE_FEATURES
	tpp_makefile_features TPP_MAKEFILE_INTERNAL(tmkf_feat);
#define _tpp_makefile_init_feat(self) , tpp_makefile_features_init(&(self)->TPP_MAKEFILE_INTERNAL(tmkf_feat))
#define _tpp_makefile_fini_feat(self) , tpp_makefile_features_fini(&(self)->TPP_MAKEFILE_INTERNAL(tmkf_feat))
#else /* TPP_MAKEFILE_HAVE_FEATURES */
#define _tpp_makefile_init_feat(self) /* nothing */
#define _tpp_makefile_fini_feat(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_FEATURES */

	/* Makefile flags (set of `TPP_MAKEFILE_FLAG_*`) */
#if TPP_MAKEFILE_HAVE_FLAGS
	tpp_makefile_flags TPP_MAKEFILE_INTERNAL(tmkf_flags);
#define _tpp_makefile_init_flags(self) , (self)->TPP_MAKEFILE_INTERNAL(tmkf_flags) = TPP_MAKEFILE_FLAG_NORMAL
#else /* TPP_MAKEFILE_HAVE_FLAGS */
#define _tpp_makefile_init_flags(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_FLAGS */

#if TPP_MAKEFILE_HAVE_PHONY || TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
	tpp_size            TPP_MAKEFILE_INTERNAL(tmkf_depc); /* # of elements in `tmkf_depv` */
	tpp_size            TPP_MAKEFILE_INTERNAL(tmkf_depa); /* Allocated size of `tmkf_depv` */
	tpp_keyword const **TPP_MAKEFILE_INTERNAL(tmkf_depv); /* [1..1][0..tmkf_depc][owned] Vector of dependencies (for replay as phonies) */
#define _tpp_makefile_init_depv(self) , (self)->TPP_MAKEFILE_INTERNAL(tmkf_depc) = (self)->TPP_MAKEFILE_INTERNAL(tmkf_depa) = 0, (self)->TPP_MAKEFILE_INTERNAL(tmkf_depv) = NULL
#define _tpp_makefile_fini_depv(self) , tpp_free((self)->TPP_MAKEFILE_INTERNAL(tmkf_depv))
#else /* TPP_MAKEFILE_HAVE_PHONY || TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */
#define _tpp_makefile_init_depv(self) /* nothing */
#define _tpp_makefile_fini_depv(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_PHONY && !TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */

	/* Current/maximum column position before lines are wrapped */
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	tpp_column TPP_MAKEFILE_INTERNAL(tmkf_curcol); /* Current column position */
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH < 0
	tpp_column TPP_MAKEFILE_INTERNAL(tmkf_maxcol); /* Max column position */
#define tpp_makefile_getmaxcol(self)     (self)->TPP_MAKEFILE_INTERNAL(tmkf_maxcol)
#define tpp_makefile_setmaxcol(self, v)  (void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_maxcol) = (v))
#define tpp_makefile_disablemaxcol(self) (void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_maxcol) = -1)
#define _tpp_makefile_init_col(self)     , (self)->TPP_MAKEFILE_INTERNAL(tmkf_curcol) = 0, (self)->TPP_MAKEFILE_INTERNAL(tmkf_maxcol) = (-TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH)
#else /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH < 0 */
#define _tpp_makefile_init_col(self) , (self)->TPP_MAKEFILE_INTERNAL(tmkf_curcol) = 0
#define tpp_makefile_getmaxcol(self) TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH >= 0 */
#else /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */
#define _tpp_makefile_init_col(self)     /* nothing */
#define tpp_makefile_getmaxcol(self)     (-1)
#define tpp_makefile_disablemaxcol(self) (void)0
#endif /* !TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */
} tpp_makefile;

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given makefile.
 *
 * @param: lexer:  The lexer whose tokens are being emitted
 * @param: output: Default output printer. On error, must return one of `TPP_SSIZE_OFERR(*)` */
#define tpp_makefile_init(self, lexer, output)                  \
	(void)(_tpp_makefile_init_lexer(self, lexer),               \
	       (self)->TPP_MAKEFILE_INTERNAL(tmkf_output) = (output) \
	       _tpp_makefile_init_output_file(self)                 \
	       _tpp_makefile_init_feat(self)                        \
	       _tpp_makefile_init_flags(self)                       \
	       _tpp_makefile_init_depv(self)                        \
	       _tpp_makefile_init_col(self))
#define tpp_makefile_fini(self)                                  \
	(void)((void)0 _tpp_makefile_fini_depv(self)                 \
	       _tpp_makefile_fini_output_file(self),                 \
	       tpp_makefile_disable(self),                           \
	       tpp_makefile_disable_missing_file_dependencies(self), \
	       tpp_dbg_memset(self, sizeof(tpp_makefile)))

/* Retrieve components of the makefile. */
#define tpp_makefile_getoutput(self)    (self)->TPP_MAKEFILE_INTERNAL(tmkf_output)
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER)
#define tpp_makefile_getlexer(self)   ((self)->TPP_MAKEFILE_INTERNAL(tmkf_lexer))
#define tpp_makefile_ofcookie(cookie) ((tpp_makefile *)(cookie))
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#ifndef TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: under '-DTPP_HAVE_HOOK_COOKIES=0' you must specify a macro '#define TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER (offsetof(MY_CONTAINER, makefile) - offsetof(MY_CONTAINER, lexer))' to specify how to retrieve the makefile from a lexer"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#define TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER sizeof(tpp_lexer)
#endif /* !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define tpp_makefile_getlexer(self)   ((tpp_lexer *)((char *)(self) - TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER))
#define tpp_makefile_ofcookie(cookie) ((tpp_makefile *)((char *)(cookie) + TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */

/* Assign a different output printer to "self" (also closes the old
 * printer's file, if one was assigned by `tpp_makefile_setoutput_io()`
 * or `tpp_makefile_setoutput_file()`) */
#define tpp_makefile_setoutput(self, v)                                  \
	(void)((void)0 _tpp_makefile_fini_output_file_before_reassign(self), \
	       (self)->TPP_MAKEFILE_INTERNAL(tmkf_output) = (v))


/* Same as `tpp_makefile_setoutput()`, but assign a `tpp_makefile_io_handle`
 * as output target. Note that the given `handle` is *ALWAYS* inherited. If
 * this isn't what you want, use `tpp_makefile_setoutput_io_ex()` instead. */
#if TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO
#define tpp_makefile_setoutput_io(self, /*inherit(always)*/ handle)      \
	(void)((void)0 _tpp_makefile_fini_output_file_before_reassign(self), \
	       (self)->TPP_MAKEFILE_INTERNAL(tmkf_output_file) = (handle),    \
	       (self)->TPP_MAKEFILE_INTERNAL(tmkf_output) = &_tpp_makefile_builtin_file_output)
TPP_DECL TPP_FORMATPRINTER_DEFINE(_tpp_makefile_builtin_file_output, arg, text, num_bytes);

/* Check if output is directed to a file, i.e. has been configured by one of:
 * - `tpp_makefile_setoutput_io()`
 * - `tpp_makefile_setoutput_io_ex()`
 * - `tpp_makefile_setoutput_file()` */
#define tpp_makefile_isoutput_io(self) \
	(tpp_makefile_getoutput(self) == &_tpp_makefile_builtin_file_output)

/* Return the I/O handle set by `tpp_makefile_setoutput_io()`. Behavior is weak
 * undefined (and return value is entirely undefined) if the current output wasn't
 * set by `tpp_makefile_setoutput_io()`, `tpp_makefile_setoutput_io_ex()` or
 * `tpp_makefile_setoutput_file()`.
 *
 * To check if output is directed to a file, use `tpp_makefile_isoutput_io(self)` */
#define tpp_makefile_getoutput_io(self) \
	((self)->TPP_MAKEFILE_INTERNAL(tmkf_output_file))
#else /* TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO */
#define tpp_makefile_isoutput_io(self) 0
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO */


/* Same as `tpp_makefile_setoutput_io()`, but takes an additional argument
 * which can be used to specifiy that the given `handle` should *NOT* be
 * closed when the makefile is finalize, or a different output is assigned. */
#if TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE
#define tpp_makefile_setoutput_io_ex(self, /*inherit(always)*/ handle, noclose) \
	(void)(tpp_makefile_setoutput_io(self, handle),                             \
	       (noclose) ? tpp_makefile_enable_output_noclose(self)                 \
	                 : (void)0)

/* Get/set the NOCLOSE status set by `tpp_makefile_setoutput_io_ex()`
 * Behavior is weak undefined if output wasn't re-directed to a custom
 * file, or was set by `tpp_makefile_setoutput()` or the initializer. */
#define tpp_makefile_get_output_noclose(self) \
	((self)->TPP_MAKEFILE_INTERNAL(tmkf_flags) & TPP_MAKEFILE_FLAG_OUTPUT_NOCLOSE)
#define tpp_makefile_set_output_noclose(self, v)    \
	((v) ? tpp_makefile_enable_output_noclose(self) \
	     : tpp_makefile_disable_output_noclose(self))
#define tpp_makefile_enable_output_noclose(self) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_flags) |= TPP_MAKEFILE_FLAG_OUTPUT_NOCLOSE)
#define tpp_makefile_disable_output_noclose(self) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_flags) &= ~TPP_MAKEFILE_FLAG_OUTPUT_NOCLOSE)
#else /* TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE */
#define tpp_makefile_get_output_noclose(self)     0
#define tpp_makefile_disable_output_noclose(self) (void)0
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE */


/* Same as `tpp_makefile_setoutput_io()`, but can be used to *directly*
 * initialize output by opening (or creating) a file on-disk.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: Some parent-path-component of `filename` does not exist
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_MAKEFILE_HAVE_OUTPUT_FILE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_setoutput_file(tpp_makefile *tpp_restrict self,
                            char const *tpp_restrict filename);
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE */


/* Helpers for quickly printing stuff to the makefile's output.
 * WARNING: Careless use of these functions may result in the makefile's
 *          idea of its current output file/line/column becoming invalid. */
#define tpp_makefile_output_printraw(self, text, len) \
	tpp_formatprinter_print(tpp_makefile_getoutput(self), self, text, len)
#define tpp_makefile_output_printraw_cstr(self, text, len) \
	tpp_formatprinter_print_cstr(tpp_makefile_getoutput(self), self, text, len)
#define tpp_makefile_output_printraw_conststr(self, CONSTstr) \
	tpp_formatprinter_print_conststr(tpp_makefile_getoutput(self), self, CONSTstr)

/* Check if a runtime-configurable config option `conf` in `TPP_MAKEFILE_HAVE_<conf>` is currently enabled.
 * When `TPP_MAKEFILE_HAVE_<conf>` is configured as `TPP_CONF_ISCONST()`, return that constant instead. */
#define tpp_makefile_has(self, conf) _tpp_makefile_has_##conf(self)

/* Features... */
#if TPP_MAKEFILE_HAVE_FEATURES
#define tpp_makefile_getfeature(self, TPP_MAKEFILE_FEAT_x)          tpp_makefile_features_getid(&(self)->TPP_MAKEFILE_INTERNAL(tmkf_feat), TPP_MAKEFILE_FEAT_x)
#define tpp_makefile_setfeature(self, TPP_MAKEFILE_FEAT_x, enabled) tpp_makefile_features_setid(&(self)->TPP_MAKEFILE_INTERNAL(tmkf_feat), TPP_MAKEFILE_FEAT_x, enabled)
#define tpp_makefile_enablefeature(self, TPP_MAKEFILE_FEAT_x)       tpp_makefile_features_enable(&(self)->TPP_MAKEFILE_INTERNAL(tmkf_feat), TPP_MAKEFILE_FEAT_x)
#define tpp_makefile_disablefeature(self, TPP_MAKEFILE_FEAT_x)      tpp_makefile_features_disable(&(self)->TPP_MAKEFILE_INTERNAL(tmkf_feat), TPP_MAKEFILE_FEAT_x)
#define tpp_makefile_resetfeatures(self)                            tpp_makefile_features_reset(&(self)->TPP_MAKEFILE_INTERNAL(tmkf_feat))
#else /* TPP_MAKEFILE_HAVE_FEATURES */
#define tpp_makefile_getfeature(self, TPP_MAKEFILE_FEAT_x) 0
#define tpp_makefile_resetfeatures(self)                   (void)0
#endif /* !TPP_MAKEFILE_HAVE_FEATURES */


/* Must be called *ONCE* after all input has been processed.
 * - This function will print a final, trailing LF, and (if enabled) re-
 *   emit dependencies as dummy targets (see `TPP_MAKEFILE_HAVE_PHONY`).
 * - This function is a no-op if `tpp_makefile_getenabled(self)` is `false`.
 *
 * @return: TPP_EOK: Success (or no-op)
 * @return: * : Error (as returned by `tpp_makefile_getoutput(self)`) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_flush(tpp_makefile *tpp_restrict self);



/* The main (mandatory) `NEW_DEPENDECY` hook that's used to
 * get notified whenever the lexer encounters a new dependency */
#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
#define tpp_makefile_getenabled(self) \
	(tpp_lexer_gethook_new_dependency(tpp_makefile_getlexer(self)) == &_tpp_makefile_new_dependency_hook)
#define tpp_makefile_setenabled(self, enabled) \
	((enabled) ? (tpp_makefile_enable(self))   \
	           : (tpp_makefile_disable(self), TPP_EOK))
#if TPP_HAVE_HOOK_COOKIES
#define tpp_makefile_enable(self)                                            \
	(tpp_lexer_sethook_new_dependency_ex(tpp_makefile_getlexer(self),        \
	                                     &_tpp_makefile_new_dependency_hook, \
	                                     self),                              \
	 TPP_EOK)
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_makefile_enable(self)                                          \
	(tpp_lexer_sethook_new_dependency(tpp_makefile_getlexer(self),         \
	                                  &_tpp_makefile_new_dependency_hook), \
	 TPP_EOK)
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_makefile_disable(self) \
	tpp_lexer_resethook_new_dependency(tpp_makefile_getlexer(self))
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_makefile_new_dependency_hook(tpp_hook_cookie cookie, tpp_keyword *filename_kwd);
#else /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
#define tpp_makefile_getenabled(self) 0
#define tpp_makefile_disable(self)    (void)0
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */



/* Handle missing file dependencies by (blindly) emitting them to the makefile */
#if TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
#define tpp_makefile_get_missing_file_dependencies_enabled(self) \
	(tpp_lexer_gethook_include_not_found(tpp_makefile_getlexer(self)) == &_tpp_makefile_include_not_found_hook)
#define tpp_makefile_set_missing_file_dependencies_enabled(self, v) \
	((v) ? (tpp_makefile_enable_missing_file_dependencies(self))    \
	     : (tpp_makefile_disable_missing_file_dependencies(self), TPP_EOK))
#if TPP_HAVE_HOOK_COOKIES
#define tpp_makefile_enable_missing_file_dependencies(self)                        \
	(tpp_lexer_sethook_include_not_found_ex(tpp_makefile_getlexer(self),           \
	                                        &_tpp_makefile_include_not_found_hook, \
	                                        self),                                 \
	 TPP_EOK)
#else /* TPP_HAVE_HOOK_COOKIES */
#define tpp_makefile_enable_missing_file_dependencies(self)                      \
	(tpp_lexer_sethook_include_not_found(tpp_makefile_getlexer(self),            \
	                                     &_tpp_makefile_include_not_found_hook), \
	 TPP_EOK)
#endif /* !TPP_HAVE_HOOK_COOKIES */
#define tpp_makefile_disable_missing_file_dependencies(self) \
	tpp_lexer_resethook_include_not_found(tpp_makefile_getlexer(self))
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno
_tpp_makefile_include_not_found_hook(tpp_hook_cookie cookie, tpp_hook_include_kind include_kind);
#else /* TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */
#define tpp_makefile_get_missing_file_dependencies_enabled(self) 0
#define tpp_makefile_disable_missing_file_dependencies(self)     (void)0
#endif /* !TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */



/* Apply Makefile-escape rules to `data...+=num_bytes` and print the result to `printer` */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_ssize TPPCALL
tpp_makefile_escape(tpp_formatprinter printer, void *arg,
                    tpp_char const *data, tpp_size num_bytes,
                    tpp_size *p_num_bytes_printed);

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_H */
