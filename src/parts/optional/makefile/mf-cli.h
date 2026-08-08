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

#undef TPP_MAKEFILE_HAVE_CLI_DASH_FLAGS
#define TPP_MAKEFILE_HAVE_CLI_DASH_FLAGS \
	0

#if TPP_MAKEFILE_HAVE_CLI_DASH_FLAGS
#define _tpp_makefile_cli_loader_flags uint_least32_t
#define _TPP_MAKEFILE_CLI_LOADER_FLAG_NORMAL UINT32_C(0x00000000)
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_FLAGS */


typedef struct tpp_makefile_cli_loader {
	tpp_makefile *TPP_MAKEFILE_INTERNAL(tmfcl_mf);    /* [1..1][const] The makefile being configured by this CLI loader */
	unsigned int  TPP_MAKEFILE_INTERNAL(tmfcl_state); /* CLI loader state (meaning of value is internal, except for `TPP_MAKEFILE_CLI_LOADER_STATE_*` listed above) */
#if TPP_MAKEFILE_HAVE_CLI_DASH_FLAGS
	_tpp_makefile_cli_loader_flags TPP_MAKEFILE_INTERNAL(tmfcl_flags);
#define _tpp_makefile_cli_loader_init_flags(self) , (self)->TPP_MAKEFILE_INTERNAL(tmfcl_flags) = _TPP_MAKEFILE_CLI_LOADER_FLAG_NORMAL
#else /* TPP_MAKEFILE_HAVE_CLI_DASH_FLAGS */
#define _tpp_makefile_cli_loader_init_flags(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_FLAGS */
} tpp_makefile_cli_loader;

/* Initialize a CLI loader for `makefile`
 *
 * The CLI loader must be used on a lexer/makefile that has already been initialized
 * itself (as per `tpp_makefile_init()`), though whether or not the its initial
 * file has already been initialized doesn't matter (the CLI loader will never
 * make persistent modifications to a lexer's current file/token). */
#define tpp_makefile_cli_loader_init(self, makefile)                                         \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmfcl_mf)    = (makefile),                          \
	       (self)->TPP_MAKEFILE_INTERNAL(tmfcl_state) = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL \
	       _tpp_makefile_cli_loader_init_flags(self))
#define tpp_makefile_cli_loader_fini(self) \
	tpp_dbg_memset(self, sizeof(tpp_makefile_cli_loader))

/* Return the makefile that is being initialized by the given CLI loader. */
#define tpp_makefile_cli_loader_getmakefile(self) \
	(self)->TPP_MAKEFILE_INTERNAL(tmfcl_mf)

/* Check if a "--" argument was encountered during CLI parsing.
 * Once that is the case, `tpp_makefile_cli_loader_parsearg()` will
 * no longer accept additional CLI arguments, and all remaining
 * arguments should be treated as input files (for the compiler
 * that you're building) */
#define tpp_makefile_cli_loader_hasddash(self) \
	((self)->TPP_MAKEFILE_INTERNAL(tmfcl_state) == TPP_MAKEFILE_CLI_LOADER_STATE_DDASH)

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
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_cli_loader_flush(tpp_makefile_cli_loader *tpp_restrict self);

#if TPP_MAKEFILE_HAVE_CLI_HELP
/* Returns supported CLI parameters, and human-readable information
 * for them. Same format as `tpp_cli_loader_help` (see for more info) */
TPP_CONST_DECL char const tpp_makefile_cli_loader_help[];
#endif /* TPP_MAKEFILE_HAVE_CLI_HELP */
#endif /* TPP_MAKEFILE_HAVE_CLI */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_CLI_H */
