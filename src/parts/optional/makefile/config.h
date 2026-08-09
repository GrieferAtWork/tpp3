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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_CONFIG_H
#define GUARD_TPP_OPTIONAL_MAKEFILE_CONFIG_H 1

#include "api.h"

/*[[[tpp-begin]]]*/

#if !TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
#error "Sorry: In order to use TPP's MAKEFILE source extension, `TPP_HAVE_NEW_DEPENDENCY_HOOK` must be configured to allow runtime overrides"
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

/* When enabled, only include dependencies if
 * `#include`-stack doesn't contain any system
 * headers */
#ifndef TPP_MAKEFILE_HAVE_USER_DEPENDENCIES
#define TPP_MAKEFILE_HAVE_USER_DEPENDENCIES (TPP_HAVE_FILE_SYSHDR ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_MAKEFILE_HAVE_USER_DEPENDENCIES */

/* When enabled, the Makefile keeps track of all dependencies encountered
 * over time, and (once flushed) will print all of them a second time in
 * the form of empty (dummy) targets. This is necessary in case one of those
 * dependencies is deleted, since otherwise Makefile will complain about
 * deleted files when reading in dependencies again.
 *
 * Used to implement the `-MP` CLI switch */
#ifndef TPP_MAKEFILE_HAVE_PHONY
#define TPP_MAKEFILE_HAVE_PHONY (TPP_CONF_FEAT0)
#endif /* !TPP_MAKEFILE_HAVE_PHONY */

/* (soft-)limit on how long lines in the dependency output should get.
 * Whenever a line (tries to) become longer than this, the next dependency
 * isn't appended to the same line, and instead a \-escaped linefeed is
 * inserted and the dependency is emitted to the next line.
 *
 * - When negative, the limit becomes configurable at runtime, with the
 *   absolute value of this configuration being used as default.
 * - When positive, the limit is hard-coded as that value
 * - When `0`, the limit is hard-coded as infinite (wrapping never happens) */
#ifndef TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
#define TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH (-72)
#endif /* !TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */


/* Provide an API surrounding `tpp_makefile_io_handle` that can be used to
 * open/create files for writing, and then write to those files. This in
 * turn is necessary to implement stuff like the `-MF` CLI switch. */
#ifndef TPP_MAKEFILE_HAVE_IO_HANDLE
#define TPP_MAKEFILE_HAVE_IO_HANDLE (TPP_HAVE_PROFILE_ALL)
#endif /* !TPP_MAKEFILE_HAVE_IO_HANDLE */

/* Provide an API `tpp_makefile_setoutput_io()` that can be used to override
 * the makefile's output printer with one that prints data into a linked I/O
 * handle of type `tpp_makefile_io_handle`. */
#ifndef TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO
#define TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO (TPP_HAVE_PROFILE_ALL && TPP_MAKEFILE_HAVE_IO_HANDLE)
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO */

/* Provide an API `tpp_makefile_setoutput_io_ex()` that can also be used to
 * set a flag specifying that the specified I/O handle should *not* be closed
 * when the makefile is finalized (or a different output is assigned) */
#ifndef TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE
#define TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE (TPP_HAVE_PROFILE_ALL && TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO)
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE */

/* Provide an API `tpp_makefile_setoutput_file()` that is a concenience
 * wrapper around `tpp_makefile_setoutput_io()` by automatically opening
 * the file in question and assigning its output. */
#ifndef TPP_MAKEFILE_HAVE_OUTPUT_FILE
#define TPP_MAKEFILE_HAVE_OUTPUT_FILE (TPP_HAVE_PROFILE_ALL && TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO)
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE */


/************************************************************************/
/* MAKEFILE CLI CONFIG                                                  */
/************************************************************************/

/* Enable support for `tpp_makefile_cli_loader` */
#ifndef TPP_MAKEFILE_HAVE_CLI
#define TPP_MAKEFILE_HAVE_CLI TPP_HAVE_CLI
#endif /* !TPP_MAKEFILE_HAVE_CLI */

/* Enable support for `tpp_makefile_cli_loader_help`, which exposes a small
 * database of supported commandline flags in a human-readable format that
 * can also be rendered (fairly) easily. */
#ifndef TPP_MAKEFILE_HAVE_CLI_HELP
#define TPP_MAKEFILE_HAVE_CLI_HELP (TPP_HAVE_PROFILE_ALL && TPP_MAKEFILE_HAVE_CLI)
#endif /* !TPP_MAKEFILE_HAVE_CLI_HELP */

/* Include extra spellings (i.e.: in addition to the primary spelling) of CLI options. */
#ifndef TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS
#define TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS (TPP_MAKEFILE_HAVE_CLI_HELP && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS */

/* `-M`, `--dependencies`: Turn on makefile generator, and
 * set the CLI loader's `ONLYMAKEFILE` flag such that it will
 * consume all of the lexer's tokens during its flush phase. */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_M
#define TPP_MAKEFILE_HAVE_CLI_DASH_M (TPP_MAKEFILE_HAVE_CLI)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_M */

/* `-MM`, `--user-dependencies`: Like `-M` but also turn on `TPP_MAKEFILE_HAVE_USER_DEPENDENCIES` */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MM
#define TPP_MAKEFILE_HAVE_CLI_DASH_MM \
	(TPP_MAKEFILE_HAVE_CLI && TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES))
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MM */

/* `-MF file`: Redirect makefile output to `file` (but doesn't clear the `ONLYMAKEFILE`
 * flag if used with `-M` or `-MM` -- for that, you must use `-MD` or `-MMD` instead,
 * which also (optionally) accept this flag) */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MF
#define TPP_MAKEFILE_HAVE_CLI_DASH_MF \
	(TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_OUTPUT_FILE)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MF */

/* TODO: `-MG`, `--print-missing-file-dependencies`: */

/* Extension to `TPP_MAKEFILE_HAVE_CLI_DASH_MF`: when the
 * specified filename is `-`, output to `stdout` instead */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH
#define TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH \
	(TPP_MAKEFILE_HAVE_CLI_DASH_MF && TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH */

/* `-MT TARGET`: Specifies the exact text that to use as the makefile target name. */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MT
#define TPP_MAKEFILE_HAVE_CLI_DASH_MT (TPP_MAKEFILE_HAVE_CLI)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MT */

/* `-MQ TARGET`: Same as `TPP_MAKEFILE_HAVE_CLI_DASH_MT`, but perform makefile escaping on `TARGET` */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MQ
#define TPP_MAKEFILE_HAVE_CLI_DASH_MQ (TPP_MAKEFILE_HAVE_CLI)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MQ */

/* `-MD`: Similar to `-M`, but don't consume all input and instead auto-determine
 * output filename (unless specified by `-MF FILE`) based on the `output_filename`
 * argument passed to `tpp_makefile_cli_loader_flush()` or `__BASE_FILE__`:
 * - If `output_filename` is given, then the makefile output is
 *   `output_filename.rpartition(".").first + ".d"` (unless that
 *   file does not contain a `"."` in its last path component, in
 *   which case the `".d"` is simply appended as-is)
 *   NOTE: The `".d"` here can be configured by `TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION`
 * - If `output_filename` isn't given, `__BASE_FILE__` is used instead
 *   of it, but with the same transformation as described above. */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MD
#define TPP_MAKEFILE_HAVE_CLI_DASH_MD \
	(TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_OUTPUT_FILE)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MD */

/* `-MMD`: Same as `-MD`, but also turn on `TPP_MAKEFILE_HAVE_USER_DEPENDENCIES` */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MMD
#define TPP_MAKEFILE_HAVE_CLI_DASH_MMD \
	(TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_OUTPUT_FILE && TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES))
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MMD */

/* `-MP`: Turn on `TPP_MAKEFILE_HAVE_PHONY` */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MP
#define TPP_MAKEFILE_HAVE_CLI_DASH_MP \
	(TPP_MAKEFILE_HAVE_CLI && TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_PHONY))
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MP */

/* TODO: CLI option to control `TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH` */



/* Controls whether `TPP_MAKEFILE_HAVE_CLI_DASH_M`
 * and `TPP_MAKEFILE_HAVE_CLI_DASH_MM` will cause the
 * lexer's entire input to be consumed during a call
 * to `tpp_makefile_cli_loader_flush()`.
 *
 * s.a.:
 * - `tpp_makefile_cli_loader_getonlymakefile()`
 * - `tpp_makefile_cli_loader_setonlymakefile()` */
#ifndef TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
#define TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE \
	(TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM)
#endif /* !TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */


/* Default file extensions appended by `-MD` and `-MMD` */
#ifndef TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION
#define TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION ".d"
#endif /* !TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION */

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_CONFIG_H */
