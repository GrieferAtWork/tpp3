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
#ifndef GUARD_TPP_CLI_H
#define GUARD_TPP_CLI_H 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "lexer.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CLI

/* Publicly exposed CLI loader states */
#define TPP_CLI_LOADER_STATE_NORMAL 0 /* Normal state */
#define TPP_CLI_LOADER_STATE_DDASH  1 /* State after "--" was encountered (causing all remaining ) */

#undef TPP_HAVE_CLI_NEEDS_FINI
typedef struct tpp_cli_loader {
	tpp_lexer   *TPP_INTERNAL(tcl_lexer);  /* [1..1][const] The lexer being configured by this CLI loader */
	unsigned int TPP_INTERNAL(tcl_state);  /* CLI loader state (meaning of value is internal, except for `TPP_CLI_LOADER_STATE_*` listed above) */
#if TPP_HAVE_CLI_DASH_IPREFIX
	char const  *TPP_INTERNAL(tcl_prefix); /* [0..1][const] Current path prefix for `TPP_HAVE_CLI_DASH_IWITHPREFIX` and `TPP_HAVE_CLI_DASH_IWITHPREFIXBEFORE` */
#define _tpp_cli_loader_init_prefix(self) , (self)->TPP_INTERNAL(tcl_prefix) = NULL
#else /* TPP_HAVE_CLI_DASH_IPREFIX */
#define _tpp_cli_loader_init_prefix(self) /* nothing */
#endif /* !TPP_HAVE_CLI_DASH_IPREFIX */
#if TPP_HAVE_CLI_DASH_ISYSROOT
	char const  *TPP_INTERNAL(tcl_sysroot); /* [0..1][const] Sysroot prefix */
#define _tpp_cli_loader_init_sysroot(self) , (self)->TPP_INTERNAL(tcl_sysroot) = TPP_CONFIG_CLI_DEFAULT_SYSROOT
#else /* TPP_HAVE_CLI_DASH_ISYSROOT */
#define _tpp_cli_loader_init_sysroot(self) /* nothing */
#endif /* !TPP_HAVE_CLI_DASH_ISYSROOT */
#if TPP_HAVE_CLI_DASH_INCLUDE
#define TPP_HAVE_CLI_NEEDS_FINI 1
	tpp_lexer_openfile_result *TPP_INTERNAL(tcl_includev); /* [0..tcl_includec][owned] Extra files to #include at start of main input file */
	tpp_size                   TPP_INTERNAL(tcl_includec); /* # of elements in `tcl_includev` */
#define _tpp_cli_loader_init_include(self) , (self)->TPP_INTERNAL(tcl_includev) = NULL, (self)->TPP_INTERNAL(tcl_includec) = 0
#else /* TPP_HAVE_CLI_DASH_INCLUDE */
#define _tpp_cli_loader_init_include(self) /* nothing */
#endif /* !TPP_HAVE_CLI_DASH_INCLUDE */
} tpp_cli_loader;

/* Return the lexer that is being initialized by the given CLI loader. */
#define tpp_cli_loader_getlexer(self) \
	(self)->TPP_INTERNAL(tcl_lexer)

/* Check if a "--" argument was encountered during CLI parsing.
 * Once that is the case, `tpp_cli_loader_parsearg()` will no
 * longer accept additional CLI arguments, and all remaining
 * arguments should be treated as input files (for the compiler
 * that you're building) */
#define tpp_cli_loader_hasddash(self) \
	((self)->TPP_INTERNAL(tcl_state) == TPP_CLI_LOADER_STATE_DDASH)

#ifndef TPP_HAVE_CLI_NEEDS_FINI
#define TPP_HAVE_CLI_NEEDS_FINI 0
#endif /* !TPP_HAVE_CLI_NEEDS_FINI */

/* Initialize a CLI loader for `lexer`
 *
 * The CLI loader must be used on a lexer that has already been initialized
 * itself (as per `tpp_lexer_init()`), though whether or not the its initial
 * file has already been initialized doesn't matter (the CLI loader will never
 * make persistent modifications to a lexer's current file/token). */
#define tpp_cli_loader_init(self, lexer)                                 \
	(void)((self)->TPP_INTERNAL(tcl_lexer) = (lexer),                    \
	       (self)->TPP_INTERNAL(tcl_state) = TPP_CLI_LOADER_STATE_NORMAL \
	       _tpp_cli_loader_init_include(self)                            \
	       _tpp_cli_loader_init_prefix(self)                             \
	       _tpp_cli_loader_init_sysroot(self))
#if TPP_HAVE_CLI_NEEDS_FINI
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_cli_loader_fini(tpp_cli_loader *tpp_restrict self);
#else /* TPP_HAVE_CLI_NEEDS_FINI */
#define tpp_cli_loader_fini(self) tpp_dbg_memset(self, sizeof(tpp_cli_loader))
#endif /* !TPP_HAVE_CLI_NEEDS_FINI */

/* Feed an argument to the loader. How exactly the argument is parsed
 * depends on the loader's current state, but sufficed to say: in its
 * default/initial state, `arg` should be something like `-Dfoo=bar`
 * or `-I/usr/include`, etc.
 *
 * WARNING: When you call this function, you must guaranty that `arg` remains
 *          valid, allocated, and unaltered until `tpp_cli_loader_fini()` is
 *          called.
 *
 * @return: TPP_EOK:        Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:     SOFT_ERROR: Argument could not be understood (but no
 *                          warning was emitted). You must either handle it yourself
 *                          by treating it as an argument for *your* compiler's
 *                          CLI, or as an input file for it, or emit a warning
 *                          informing the user that their CLI argument was not
 *                          understood.
 * @return: TPP_ENOMEM:     HARD_ERROR: Out of memory
 * @return: TPP_EIO:        HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR:  HARD_ERROR: A lexer error was thrown
 * @return: TPP_EWARNPRINT: HARD_ERROR: An error happened within a warning printer */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parsearg(tpp_cli_loader *tpp_restrict self, char const *arg);

/* Convenience wrapper around `tpp_cli_loader_parsearg()`:
 * - This function passes every argument given to `tpp_cli_loader_parsearg()`
 *   in the order they appear within the specified argument vector (as it was
 *   originally passed to your `main()`)
 * - Anything for which `tpp_cli_loader_parsearg()` returns `TPP_ENOENT` is
 *   pushed to the back of the argument vector such that it will still appear
 *   within `(*p_argv)` and be accounted for in `(*p_argc)` on return, as
 *   well as still appear in the same order.
 * - If an "--" argument is encountered, one of 2 things happen:
 *   - If there were already arguments that were pushed to the back, then
 *     all remaning un-parsed arguments (*INCLUDING*) the "--" argument are
 *     also pushed to the back and this function returns with `TPP_EOK` while
 *     the remainder of the argument list looks like:
 *     >> `{ "-unknown-arg", "--", "file1.c", "file2.c" }`
 *   - If no arguments were pushed to the back yet, then the given `p_argc`
 *     and `p_argv` are updated to point to everything that comes after the
 *     "--" argument (*INCLUDING* the "--" argument itself), and the function
 *     returns `TPP_EOK` while the remainder of the argument list looks like:
 *     >> `{ "--", "file1.c", "file2.c" }`
 * - Before calling this method, be sure to strip the leading program-name
 *   argument from the argc/argv your `main()` was given, before calling
 *   this function!
 *
 * @return: TPP_EOK:        Success (`*p_argc` and `*p_argv` were updated such that
 *                          they contain all unrecognized arguments, as well as all
 *                          input files for your compiler).
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O Error
 * @return: TPP_ELEXERROR:  A lexer error was thrown
 * @return: TPP_EWARNPRINT: An error happened within a warning printer */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_cli_loader_parseargv(tpp_cli_loader *tpp_restrict self,
                         int *p_argc, char ***p_argv);

/* Ensure that `self` is in a *normal* state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_cli_loader_getlexer(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the `#include`-stack.
 *
 * @return: TPP_EOK:        Success
 * @return: TPP_ENOMEM:     Out of memory
 * @return: TPP_EIO:        I/O Error
 * @return: TPP_ELEXERROR:  A lexer error was thrown
 * @return: TPP_EWARNPRINT: An error happened within a warning printer */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_flush(tpp_cli_loader *tpp_restrict self);
#endif /* TPP_HAVE_CLI */

/* TODO: API to query supported CLI flags, for use by someone wanting to implement `--help`,
 *       or get a list of supported flags. */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CLI_H */
