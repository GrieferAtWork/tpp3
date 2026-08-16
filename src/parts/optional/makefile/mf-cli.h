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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_MF_CLI_H
#define GUARD_TPP_OPTIONAL_MAKEFILE_MF_CLI_H 1

#include "api.h"

#include "config.h"
#include "mf.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_MAKEFILE_HAVE_CLI

/* Publicly exposed CLI loader states */
#define TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL 0 /* Normal state */
#define TPP_MAKEFILE_CLI_LOADER_STATE_DDASH  1 /* State after "--" was encountered (causing all remaining ) */

#undef TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED
#define TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED                        \
	((TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM) || \
	 (TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD))
	 
#undef TPP_MAKEFILE_HAVE_CLI_LOADER_FLAGS
#define TPP_MAKEFILE_HAVE_CLI_LOADER_FLAGS                               \
	(TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED ||                         \
	 TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE ||                               \
	 (TPP_MAKEFILE_HAVE_CLI_DASH_MT && TPP_MAKEFILE_HAVE_CLI_DASH_MQ) || \
	 (TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD))

#if TPP_MAKEFILE_HAVE_CLI_LOADER_FLAGS
#define _tpp_makefile_cli_loader_flags tpp_uint_least8
#define _TPP_MAKEFILE_CLI_LOADER_FLAG_NORMAL       TPP_UINT_LEAST8_C(0x00) /* Normal flags */
#if TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED
#define _TPP_MAKEFILE_CLI_LOADER_FLAG_ENABLED      TPP_UINT_LEAST8_C(0x01) /* Enable Makefile generation */
#endif /* TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED */
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
#define _TPP_MAKEFILE_CLI_LOADER_FLAG_ONLYMAKEFILE TPP_UINT_LEAST8_C(0x02) /* Consume all tokens because only a Makefile should be generated */
#endif /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT && TPP_MAKEFILE_HAVE_CLI_DASH_MQ
#define _TPP_MAKEFILE_CLI_LOADER_FLAG_TARGETESCAPE TPP_UINT_LEAST8_C(0x04) /* Escape target name using `tpp_makefile_escape()` */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT && TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD
#define _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT   TPP_UINT_LEAST8_C(0x08) /* Automatically determine output filename if not pre-defined */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD */
#endif /* TPP_MAKEFILE_HAVE_CLI_LOADER_FLAGS */


typedef struct tpp_makefile_cli_loader {
	tpp_makefile *TPP_MAKEFILE_INTERNAL(tmkfcl_mf);      /* [1..1][const] The makefile being configured by this CLI loader */
	unsigned int  TPP_MAKEFILE_INTERNAL(tmkfcl_state);   /* CLI loader state (meaning of value is internal, except for `TPP_MAKEFILE_CLI_LOADER_STATE_*` listed above) */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ
	char const   *TPP_MAKEFILE_INTERNAL(tmkfcl_target);  /* [0..1][const] Target specified by `-MT TARGET` or `-MQ TARGET` */
#define _tpp_makefile_cli_loader_init_target(self) , (self)->TPP_MAKEFILE_INTERNAL(tmkfcl_target) = NULL
#else /* TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
#define _tpp_makefile_cli_loader_init_target(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MT && !TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
	char const   *TPP_MAKEFILE_INTERNAL(tmkfcl_outfile); /* [0..1][const] Filename specified by `-MF FILE` */
#define _tpp_makefile_cli_loader_init_outfile(self) , (self)->TPP_MAKEFILE_INTERNAL(tmkfcl_outfile) = NULL
#else /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
#define _tpp_makefile_cli_loader_init_outfile(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MF */
#if TPP_MAKEFILE_HAVE_CLI_LOADER_FLAGS
	_tpp_makefile_cli_loader_flags TPP_MAKEFILE_INTERNAL(tmkfcl_flags);
#define _tpp_makefile_cli_loader_init_flags(self) , (self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) = _TPP_MAKEFILE_CLI_LOADER_FLAG_NORMAL
#else /* TPP_MAKEFILE_HAVE_CLI_LOADER_FLAGS */
#define _tpp_makefile_cli_loader_init_flags(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_CLI_LOADER_FLAGS */
} tpp_makefile_cli_loader;

/* Initialize a CLI loader for `makefile`
 *
 * The CLI loader must be used on a lexer/makefile that has already been initialized
 * itself (as per `tpp_makefile_init()`), though whether or not the its initial
 * file has already been initialized doesn't matter (the CLI loader will never
 * make persistent modifications to a lexer's current file/token). */
#define tpp_makefile_cli_loader_init(self, makefile)                                         \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_mf)    = (makefile),                          \
	       (self)->TPP_MAKEFILE_INTERNAL(tmkfcl_state) = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL \
	       _tpp_makefile_cli_loader_init_target(self)                                        \
	       _tpp_makefile_cli_loader_init_outfile(self)                                       \
	       _tpp_makefile_cli_loader_init_flags(self))
#define tpp_makefile_cli_loader_fini(self) \
	tpp_dbg_memset(self, sizeof(tpp_makefile_cli_loader))

/* Return the makefile that is being initialized by the given CLI loader. */
#define tpp_makefile_cli_loader_getmakefile(self) \
	(self)->TPP_MAKEFILE_INTERNAL(tmkfcl_mf)

/* Check if a "--" argument was encountered during CLI parsing.
 * Once that is the case, `tpp_makefile_cli_loader_parsearg()` will
 * no longer accept additional CLI arguments, and all remaining
 * arguments should be treated as input files (for the compiler
 * that you're building) */
#define tpp_makefile_cli_loader_hasddash(self) \
	((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_state) == TPP_MAKEFILE_CLI_LOADER_STATE_DDASH)


/* Control if `tpp_makefile_cli_loader_flush()` will turn eanble the makefile */
#if TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED
#define tpp_makefile_cli_loader_getmakefileenabled(self)  \
	((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) & _TPP_MAKEFILE_CLI_LOADER_FLAG_ENABLED)
#define tpp_makefile_cli_loader_setmakefileenabled(self, v) \
	((v) ? tpp_makefile_cli_loader_enablemakefile(self)     \
	     : tpp_makefile_cli_loader_disablemakefile(self))
#define tpp_makefile_cli_loader_enablemakefile(self) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) |= _TPP_MAKEFILE_CLI_LOADER_FLAG_ENABLED)
#define tpp_makefile_cli_loader_disablemakefile(self) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) &= ~_TPP_MAKEFILE_CLI_LOADER_FLAG_ENABLED)
#else /* TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED */
#define tpp_makefile_cli_loader_enablemakefile(self)     (void)0
#define tpp_makefile_cli_loader_getmakefileenabled(self) 1
#endif /* !TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED */


/* Get/set the target, as specified by `-MT` and `-MQ` */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ
#define tpp_makefile_cli_loader_gettarget(self) (self)->TPP_MAKEFILE_INTERNAL(tmkfcl_target)
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT && TPP_MAKEFILE_HAVE_CLI_DASH_MQ
#define tpp_makefile_cli_loader_settarget_mt(self, target)         \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_target) = (target), \
	       (self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) &= ~_TPP_MAKEFILE_CLI_LOADER_FLAG_TARGETESCAPE)
#define tpp_makefile_cli_loader_settarget_mq(self, target)         \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_target) = (target), \
	       (self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) |= _TPP_MAKEFILE_CLI_LOADER_FLAG_TARGETESCAPE)
#define tpp_makefile_cli_loader_gettarget_ismt(self) ((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) & _TPP_MAKEFILE_CLI_LOADER_FLAG_TARGETESCAPE)
#define tpp_makefile_cli_loader_gettarget_ismq(self) (!tpp_makefile_cli_loader_gettarget_ismt(self))
#elif TPP_MAKEFILE_HAVE_CLI_DASH_MT
#define tpp_makefile_cli_loader_settarget_mt(self, target) (void)((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_target) = (target))
#define tpp_makefile_cli_loader_gettarget_ismt(self)       1
#define tpp_makefile_cli_loader_gettarget_ismq(self)       0
#elif TPP_MAKEFILE_HAVE_CLI_DASH_MQ
#define tpp_makefile_cli_loader_settarget_mq(self, target) (void)((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_target) = (target))
#define tpp_makefile_cli_loader_gettarget_ismt(self)       0
#define tpp_makefile_cli_loader_gettarget_ismq(self)       1
#endif /* ... */


/* Get/set whether or not a call to `tpp_makefile_cli_loader_flush()`
 * will (or has if that call was already made) consumed *all* tokens
 * from the linked lexer.
 *
 * This is supposed to happen when `-M` (aka. `--dependencies`) is
 * used in a context where conventions imply that generation of a
 * Makefile is the *only* thing that's supposed to happen (i.e.: no
 * compilation or the like is supposed to take place)
 *
 * By `#define TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE 0`,
 * this behavior can be hard-disabled at compile-time. Similarly, a
 * call to `tpp_makefile_cli_loader_disableonlymakefile(self)` just
 * prior to calling `tpp_makefile_cli_loader_flush()` will prevent
 * this token-consumption step from happening as well. */
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
#define tpp_makefile_cli_loader_getonlymakefile(self) \
	((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) & _TPP_MAKEFILE_CLI_LOADER_FLAG_ONLYMAKEFILE)
#define tpp_makefile_cli_loader_setonlymakefile(self, v)    \
	((v) ? tpp_makefile_cli_loader_enableonlymakefile(self) \
	     : tpp_makefile_cli_loader_disableonlymakefile(self))
#define tpp_makefile_cli_loader_enableonlymakefile(self) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) |= _TPP_MAKEFILE_CLI_LOADER_FLAG_ONLYMAKEFILE)
#define tpp_makefile_cli_loader_disableonlymakefile(self) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkfcl_flags) &= ~_TPP_MAKEFILE_CLI_LOADER_FLAG_ONLYMAKEFILE)
#else /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
#define tpp_makefile_cli_loader_getonlymakefile(self)     0
#define tpp_makefile_cli_loader_disableonlymakefile(self) (void)0
#endif /* !TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */


/* Feed an argument to the loader. How exactly the argument is parsed
 * depends on the loader's current state, but sufficed to say: in its
 * default/initial state, `arg` is a CLI argument as you'd expect.
 *
 * WARNING: When you call this function, you must guaranty that `arg` remains
 *          valid, allocated, and unaltered until `tpp_makefile_cli_loader_fini()` is
 *          called.
 *
 * @return: TPP_EOK:       Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:    SOFT_ERROR: Argument could not be understood (but no
 *                         warning was emitted). You must either handle it yourself
 *                         by treating it as an argument for *your* compiler's
 *                         CLI, or as an input file for the lexer, or emit a
 *                         warning informing the user that their CLI argument
 *                         was not understood. You should also probably try to
 *                         pass it to `tpp_cli_loader_parsearg()`.
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_cli_loader_parsearg(tpp_makefile_cli_loader *tpp_restrict self, char const *arg);

/* Try to parse a *flag*-style parameter, that is: an argument that actually consists
 * of multiple, tightly packed parameters, whilst having a singular, leading `-` (that
 * was already skipped by the caller).
 *
 * Example: `-PH` or `-HP`
 * - This argument consists of 2 flags `-H` and `-P`, which are simply concatenated
 *   into a single argument here. This function will then parse one of those flags
 *   from `**p_arg` (iow: `**p_arg` must be one of `H` or `P`), and advance `*p_arg`
 *   to either the end of the argument, or the next *flag*-style parameter.
 *
 * @return: TPP_EOK:    Success (`*p_arg` was updated to point to the next *flag*-style
 *                      parameter, or the argument string's end)
 * @return: TPP_ENOENT: Did not recognize the flag in `**p_arg` (caller should try to
 *                      handle the flag in a different context).
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_cli_loader_parseflag(tpp_makefile_cli_loader *tpp_restrict self, char const **p_arg);

/* Convenience wrapper around `tpp_makefile_cli_loader_parsearg()`.
 * For more information, see `tpp_cli_loader_parseargv()`.
 *
 * @return: TPP_EOK:       Success (`*p_argc` and `*p_argv` were updated such that
 *                         they contain all unrecognized arguments, as well as all
 *                         input files for the lexer).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_makefile_cli_loader_parseargv(tpp_makefile_cli_loader *tpp_restrict self,
                                  int *p_argc, char ***p_argv);

/* Ensure that `self` is in a *normal* state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_makefile_cli_loader_getmakefile(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the `#include`-stack.
 *
 * When `tpp_makefile_cli_loader_getonlymakefile(self)` returns true, a call
 * to this function will cause *all* tokens to be consumed (and discarded)
 * from the linked lexer, which is needed to implement the conventional
 * behavior of the `-M` CLI flag. This behavior can however be hard-suppressed
 * by `#define TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE 0`.
 *
 * @param: output_filename: [0..1] Name of the compiler output file (used to generate
 *                                 a fallback dependency filename for `-MD` / `-MMD`)
 *                                 If present, also used as the default name of the
 *                                 target that's written in the makefile (otherwise,
 *                                 that target is derived from `__FILE__`)
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_cli_loader_flush(tpp_makefile_cli_loader *tpp_restrict self,
                              char const *output_filename);

#if TPP_MAKEFILE_HAVE_CLI_HELP
/* Returns supported CLI parameters, and human-readable information
 * for them. Same format as `tpp_cli_loader_help` (see for more info) */
#if !TPP_USE_STATIC
TPP_CONST_DECL char const tpp_makefile_cli_loader_help[];
#endif /* !TPP_USE_STATIC */
#endif /* TPP_MAKEFILE_HAVE_CLI_HELP */

/* Print the default target name of `input_filename`, when no explicit output is known.
 * The default implementation of this function is affected by the following configs:
 * - `TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX`
 * - `TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY`
 * - `TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX`
 * - `TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY`
 * - `TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION`
 *
 * If the desired behavior cannot be achieved using only those function, it is recommended
 * to fully override this function (which can be done by pre-defining a macro with the same
 * name as this function before `#include`-ing TPP's **MAKEFILE** source extension), and
 * then implementing that function such that it behaves as desired.
 *
 * @param: self:           The CLI loader (can be used to gain access to the linked makefile/lexer)
 * @param: printer:        Output printer that the (unescaped) object filename should be printed to
 * @param: input_filename: The `__FILE__` filename of the linked lexer
 * @return: * : Sum of return values of `printer` */
#ifndef tpp_makefile_cli_print_default_target
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 4)) tpp_ssize TPPCALL
tpp_makefile_cli_print_default_target(tpp_makefile_cli_loader *tpp_restrict self,
                                      tpp_formatprinter printer, void *arg,
                                      char const *input_filename);
#endif /* !tpp_makefile_cli_print_default_target */

#endif /* TPP_MAKEFILE_HAVE_CLI */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_CLI_H */
