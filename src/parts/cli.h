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
#define TPP_CLI_LOADER_STATE_DDASH  1 /* State after "--" was encountered */

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
#if TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH
	tpp_token_id TPP_INTERNAL(tcl_search_include_path_mode);
#define _tpp_cli_loader_init_tcl_search_include_path_mode(self) , (self)->TPP_INTERNAL(tcl_search_include_path_mode) = TPP_TOK_EOF
#else /* TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH */
#define _tpp_cli_loader_init_tcl_search_include_path_mode(self) /* nothing */
#endif /* !TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH */
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


/* Get/Set/Reset the `-fsearch-include-path=...` mode of the CLI loader.
 * For this purpose, token IDs are used as follows:
 * - `TPP_TOK_EOF`:            Don't do anything special (default)
 * - `TPP_TOK_INCPATH_LANGLE`: Search as in `#include <my/file.c>`
 * - `TPP_TOK_INCPATH_DQUOTE`: Search as in `#include "my/file.c"` */
#if TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH
#define tpp_cli_loader_get_search_include_path_mode(self) \
	(self)->TPP_INTERNAL(tcl_search_include_path_mode)
#define tpp_cli_loader_set_search_include_path_mode(self, mode) \
	(void)(tpp_assert((mode) == TPP_TOK_EOF ||                  \
	                  (mode) == TPP_TOK_INCPATH_LANGLE ||       \
	                  (mode) == TPP_TOK_INCPATH_DQUOTE),        \
	       (self)->TPP_INTERNAL(tcl_search_include_path_mode) = (mode))
#define tpp_cli_loader_reset_search_include_path_mode(self, mode) \
	(void)((self)->TPP_INTERNAL(tcl_search_include_path_mode) = TPP_TOK_EOF)
#else /* TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH */
#define tpp_cli_loader_get_search_include_path_mode(self)         TPP_TOK_EOF
#define tpp_cli_loader_reset_search_include_path_mode(self, mode) (void)0
#endif /* !TPP_HAVE_CLI_DASH_FSEARCH_INCLUDE_PATH */


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
	       _tpp_cli_loader_init_prefix(self)                             \
	       _tpp_cli_loader_init_sysroot(self)                            \
	       _tpp_cli_loader_init_include(self)                            \
	       _tpp_cli_loader_init_tcl_search_include_path_mode(self))
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
 * @return: TPP_EOK:       Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:    SOFT_ERROR: Argument could not be understood (but no
 *                         warning was emitted). You must either handle it yourself
 *                         by treating it as an argument for *your* compiler's
 *                         CLI, or as an input file for it, or emit a warning
 *                         informing the user that their CLI argument was not
 *                         understood.
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parsearg(tpp_cli_loader *tpp_restrict self, char const *arg);

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
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_cli_loader_parseflag(tpp_cli_loader *tpp_restrict self, char const **p_arg);

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
 * @return: TPP_EOK:       Success (`*p_argc` and `*p_argv` were updated such that
 *                         they contain all unrecognized arguments, as well as all
 *                         input files for your compiler).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_cli_loader_parseargv(tpp_cli_loader *tpp_restrict self,
                         int *p_argc, char ***p_argv);


#if TPP_HAVE_CLI_SETINPUTS
/* Use the given `argc` and `argv` as inputs for the lexer.
 *
 * This function should be used to pass everything on your `argv` following
 * a potential `--` argument, as well as all arguments that don't start
 * with a leading `-` (you should permutate your `argv` similar to how
 * that is also done by `tpp_cli_loader_parseargv()` such that all unknown
 * arguments, as well as a potential `--` argument appear last).
 *
 * This function will *always* initialize the lexer's *file-stack*, such
 * that upon successful return from this function, the caller is responsible
 * to finalize that part of the lexer using `tpp_lexer_finifile()`.
 *
 * - If there are inputs, a warning `TPP_W_NO_INPUT_FILES` is emitted.
 * - If one of the inputs cannot be opened, a warning `TPP_W_NO_SUCH_FILE` is emitted.
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_setinputs(tpp_cli_loader *tpp_restrict self,
                         int argc, char **argv);
#endif /* TPP_HAVE_CLI_SETINPUTS */


/* Ensure that `self` is in a *normal* state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_cli_loader_getlexer(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the `#include`-stack.
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_cli_loader_flush(tpp_cli_loader *tpp_restrict self);

#if TPP_HAVE_CLI_HELP
/* Returns supported CLI parameters, and human-readable information
 * for them in the form of:
 * >> "-Dmacro[=def]\0--define-macro macro[=def]\0\0"
 * >> "Define a macro with def, or 1 as its value.\0"
 * >> "-Umacro\0--undefine-macro macro\0\0"
 * >> "Undefined macro\0"
 * >> ...
 * >> "\0"
 *
 * Format (repeated):
 * >> <SPELLING1>[\0<SPELLING2>][\0<SPELLING3>][...]\0\0<DESCRIPTION>\0
 * >> \0
 *
 * The end is reached when "DESCRIPTION" is immediately followed
 * by another NUL-character, where there would otherwise be the
 * first character of the first spelling of the next CLI parameter.
 *
 * CLI parameter spellings are sorted such that the most *prominent*
 * spelling comes first. For the sake of keeping your `--help` readable,
 * I suggest you only print `SPELLING1` and have some kind of `--help all`
 * option (or similar) that will print *all* spellings.
 *
 * To enumerate available options, you can use code like this:
 * ```c
 * char const *iter = tpp_cli_loader_help;
 * while (*iter) {
 *     bool first = true;
 *     // Print spellings
 *     do {
 *         printf("%s%s", first ? "" : " ", iter);
 *         iter += tpp_strlen(iter) + 1;
 *         first = false;
 *     } while (*iter);
 *     ++iter;
 *     // Print description
 *     printf("\n\t\t\t%s\n", iter);
 *     iter += tpp_strlen(iter) + 1;
 * }
 * ```
 *
 * NOTE: This string doesn't contain information about CLI flags:
 * - `TPP_HAVE_CLI_DASH_FEXTENSION`
 * - `TPP_HAVE_CLI_DASH_WWARNING`
 *
 * If you want to print help for those, you must enumerate them using:
 * - `tpp_extension_getname()`
 * - `tpp_warning_group_getnames()`
 */
#if !TPP_USE_STATIC
TPP_CONST_DECL char const tpp_cli_loader_help[];
#endif /* !TPP_USE_STATIC */
#endif /* TPP_HAVE_CLI_HELP */
#endif /* TPP_HAVE_CLI */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CLI_H */
