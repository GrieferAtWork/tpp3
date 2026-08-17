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
/* clang-format off */
#ifndef GUARD_TPP_MAKEFILE_AMALGAMATION_H
#define GUARD_TPP_MAKEFILE_AMALGAMATION_H 1

/************************************************************************/
/* File: parts/optional/makefile/api.h                                  */
/************************************************************************/
/* ========= Makefile =========
 *
 * Brief:
 * - Unified API for generating Makefile-compatible dependency descriptions
 *   of input files (possibly in parallel to the *actual* process of pre-
 *   processing said input files)
 * - Can be used to implement GCC's entire set of `-M*` CLI flags
 * - In order to (correctly) configure + use this API, it is *highly*
 *   recommended that you also make use of `tpp_makefile_cli_loader`,
 *   as that one actually does all the work of printing the initial
 *   input file's target object filename.
 * - The `tpp_makefile` component is only responsible for printing
 *   new include dependencies as they appear!
 */

#ifndef TPP_AMALGAMATION_H
#define TPP_AMALGAMATION_H "tpp-amalgamation.h"
#endif /* !TPP_AMALGAMATION_H */
#include TPP_AMALGAMATION_H

#ifndef TPP_MAKEFILE_BUILDING
#define TPP_MAKEFILE_BUILDING 0
#endif /* !TPP_MAKEFILE_BUILDING */

/* Used to wrap an identifier that is considered to be internal to TPP
 * Identifiers wrapped as such should **NOT** be accessed directly.
 * Instead, the provided TPP APIs should be used. */
#ifndef TPP_MAKEFILE_INTERNAL
#if TPP_MAKEFILE_BUILDING
#define TPP_MAKEFILE_INTERNAL(x) x
#else /* TPP_MAKEFILE_BUILDING */
#define TPP_MAKEFILE_INTERNAL(x) _mf_##x
#endif /* !TPP_MAKEFILE_BUILDING */
#endif /* !TPP_MAKEFILE_INTERNAL */

/************************************************************************/
/* File: parts/optional/makefile/config.h                               */
/************************************************************************/
#if !TPP_IGNORE_INVALID_CONFIGURATION && !TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
#error "Sorry: In order to use TPP's MAKEFILE source extension, `TPP_HAVE_NEW_DEPENDENCY_HOOK` must be configured to allow runtime overrides"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION && !TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

/* When enabled, only include dependencies if
 * `#include`-stack doesn't contain any system
 * headers
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_MAKEFILE_HAVE_USER_DEPENDENCIES
#define TPP_MAKEFILE_HAVE_USER_DEPENDENCIES (TPP_HAVE_FILE_SYSHDR ? TPP_CONF_FEAT0 : 0)
#endif /* !TPP_MAKEFILE_HAVE_USER_DEPENDENCIES */

/* Enable API support for handling `TPP_HOOK_INCLUDE_NOT_FOUND` by emitting
 * the associated file as an additional dependency. Useful when wanting to
 * (somewhat accurately) determine dependencies, where some of them might
 * not exist, yet.
 *
 * Used to implement `-MG` (aka. `--print-missing-file-dependencies`).
 *
 * When enabled, the following APIs become available:
 * - `tpp_makefile_enable_missing_file_dependencies()`
 * - `tpp_makefile_disable_missing_file_dependencies()`
 * - `tpp_makefile_get_missing_file_dependencies_enabled()`
 * - `tpp_makefile_set_missing_file_dependencies_enabled()`
 *
 * Note that even when this is enabled, printing of missing file
 * dependencies is disabled by default (enable it by making a
 * call to `tpp_makefile_enable_missing_file_dependencies()`).
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
#define TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES TPP_HOOK_ISRT(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
#endif /* !TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */

/* When enabled, the Makefile keeps track of all dependencies encountered
 * over time, and (once flushed) will print all of them a second time in
 * the form of empty (dummy) targets. This is necessary in case one of those
 * dependencies is deleted, since otherwise Makefile will complain about
 * deleted files when reading in dependencies again.
 *
 * Used to implement the `-MP` CLI switch
 *
 * Configure as one of:
 * - `TPP_CONF_0`, `0`: Disabled
 * - `TPP_CONF_1`, `1`: Enabled
 * - `TPP_CONF_FEAT0`: Available (disabled by default)
 * - `TPP_CONF_FEAT1`: Available (enabled by default) */
#ifndef TPP_MAKEFILE_HAVE_PHONY
#define TPP_MAKEFILE_HAVE_PHONY (TPP_CONF_FEAT0)
#endif /* !TPP_MAKEFILE_HAVE_PHONY */

/* (soft-)limit on how long lines in the dependency output should get.
 * Whenever a line (tries to) become longer than this, the next dependency
 * isn't appended to the same line, and instead a `\`-escaped linefeed is
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
 * turn is necessary to implement stuff like the `-MF` CLI switch.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_IO_HANDLE
#define TPP_MAKEFILE_HAVE_IO_HANDLE (TPP_HAVE_PROFILE_ALL)
#endif /* !TPP_MAKEFILE_HAVE_IO_HANDLE */

/* Provide an API `tpp_makefile_setoutput_io()` that can be used to override
 * the makefile's output printer with one that prints data into a linked I/O
 * handle of type `tpp_makefile_io_handle`.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO
#define TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO (TPP_HAVE_PROFILE_ALL && TPP_MAKEFILE_HAVE_IO_HANDLE)
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO */

/* Provide an API `tpp_makefile_setoutput_io_ex()` that can also be used to
 * set a flag specifying that the specified I/O handle should *not* be closed
 * when the makefile is finalized (or a different output is assigned)
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE
#define TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE (TPP_HAVE_PROFILE_ALL && TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO)
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE */

/* Provide an API `tpp_makefile_setoutput_file()` that is a concenience
 * wrapper around `tpp_makefile_setoutput_io()` by automatically opening
 * the file in question and assigning its output.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_OUTPUT_FILE
#define TPP_MAKEFILE_HAVE_OUTPUT_FILE (TPP_HAVE_PROFILE_ALL && TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO)
#endif /* !TPP_MAKEFILE_HAVE_OUTPUT_FILE */


/************************************************************************/
/* MAKEFILE CLI CONFIG                                                  */
/************************************************************************/

/* Enable support for `tpp_makefile_cli_loader`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI
#define TPP_MAKEFILE_HAVE_CLI TPP_HAVE_CLI
#endif /* !TPP_MAKEFILE_HAVE_CLI */

/* Enable support for `tpp_makefile_cli_loader_help`, which exposes a small
 * database of supported commandline flags in a human-readable format that
 * can also be rendered (fairly) easily.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_HELP
#define TPP_MAKEFILE_HAVE_CLI_HELP (TPP_MAKEFILE_HAVE_CLI && TPP_HAVE_CLI_HELP)
#endif /* !TPP_MAKEFILE_HAVE_CLI_HELP */

/* Include extra spellings (i.e.: in addition to the primary spelling) of CLI options.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS
#define TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS (TPP_MAKEFILE_HAVE_CLI_HELP && TPP_HAVE_PROFILE_NOT_MINIMAL)
#endif /* !TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS */

/* `-M`, `--dependencies`: Turn on makefile generator, and
 * set the CLI loader's `ONLYMAKEFILE` flag such that it will
 * consume all of the lexer's tokens during its flush phase.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_M
#define TPP_MAKEFILE_HAVE_CLI_DASH_M (TPP_MAKEFILE_HAVE_CLI)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_M */

/* `-MM`, `--user-dependencies`: Like `-M` but also turn on `TPP_MAKEFILE_HAVE_USER_DEPENDENCIES`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MM
#define TPP_MAKEFILE_HAVE_CLI_DASH_MM \
	(TPP_MAKEFILE_HAVE_CLI && TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES))
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MM */

/* `-MF file`: Redirect makefile output to `file` (but doesn't clear the `ONLYMAKEFILE`
 * flag if used with `-M` or `-MM` -- for that, you must use `-MD` or `-MMD` instead,
 * which also (optionally) accept this flag)
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MF
#define TPP_MAKEFILE_HAVE_CLI_DASH_MF \
	(TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_OUTPUT_FILE)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MF */

/* Extension to `TPP_MAKEFILE_HAVE_CLI_DASH_MF`: when the
 * specified filename is `-`, output to `stdout` instead
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH
#define TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH \
	(TPP_MAKEFILE_HAVE_CLI_DASH_MF && TPP_MAKEFILE_HAVE_OUTPUT_FILE_IO_NOCLOSE)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH */

/* `-MG`, `--print-missing-file-dependencies`:
 * Turn on printing of missing file dependencies within the linked makefile/lexer.
 * Also disables regular preprocessor output like `TPP_MAKEFILE_HAVE_CLI_DASH_M`.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MG
#define TPP_MAKEFILE_HAVE_CLI_DASH_MG \
	(TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MG */

/* `-MT TARGET`: Specifies the exact text that to use as the makefile target name.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MT
#define TPP_MAKEFILE_HAVE_CLI_DASH_MT (TPP_MAKEFILE_HAVE_CLI)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MT */

/* `-MQ TARGET`: Same as `TPP_MAKEFILE_HAVE_CLI_DASH_MT`, but perform makefile escaping on `TARGET`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MQ
#define TPP_MAKEFILE_HAVE_CLI_DASH_MQ (TPP_MAKEFILE_HAVE_CLI)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MQ */

/* `-MD`: Similar to `-M`, but don't consume all input and instead auto-determine
 * output filename (unless specified by `-MF FILE`) based on the `output_filename`
 * argument passed to `tpp_makefile_cli_loader_flush()` or `__FILE__`:
 * - If `output_filename` is given, then the makefile output is
 *   `output_filename.rpartition(".").first + ".d"` (unless that
 *   file does not contain a `"."` in its last path component, in
 *   which case the `".d"` is simply appended as-is)
 *   NOTE: The `".d"` here can be configured by `TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION`
 * - If `output_filename` isn't given, `__FILE__` is used instead
 *   of it, but with the same transformation as described above.
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MD
#define TPP_MAKEFILE_HAVE_CLI_DASH_MD \
	(TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_OUTPUT_FILE)
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MD */

/* `-MMD`: Same as `-MD`, but also turn on `TPP_MAKEFILE_HAVE_USER_DEPENDENCIES`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MMD
#define TPP_MAKEFILE_HAVE_CLI_DASH_MMD \
	(TPP_MAKEFILE_HAVE_CLI && TPP_MAKEFILE_HAVE_OUTPUT_FILE && TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES))
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MMD */

/* `-MP`: Turn on `TPP_MAKEFILE_HAVE_PHONY`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_DASH_MP
#define TPP_MAKEFILE_HAVE_CLI_DASH_MP \
	(TPP_MAKEFILE_HAVE_CLI && TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_PHONY))
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MP */

/* XXX: CLI option to control `TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH` */



/* Controls whether `TPP_MAKEFILE_HAVE_CLI_DASH_M`
 * and `TPP_MAKEFILE_HAVE_CLI_DASH_MM` will cause the
 * lexer's entire input to be consumed during a call
 * to `tpp_makefile_cli_loader_flush()`.
 *
 * s.a.:
 * - `tpp_makefile_cli_loader_getonlymakefile()`
 * - `tpp_makefile_cli_loader_setonlymakefile()`
 *
 * Configure as one of:
 * - `0`: Disabled
 * - `1`: Enabled */
#ifndef TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
#define TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE \
	(TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM || TPP_MAKEFILE_HAVE_CLI_DASH_MG)
#endif /* !TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */


/* Default file extensions appended by `-MD` and `-MMD` */
#ifndef TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION
#define TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION ".d"
#endif /* !TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION */


/* Configuration macros affecting the behavior of `tpp_makefile_cli_print_default_target()`:
 * - `TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX`:
 *   - Printed as a prefix preceding the default target filename
 *   - When empty, `TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY` should be defined to `1`.
 * - `TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX`:
 *   - Printed between the last directory component of `__BASE_NAME__` and the its filename portion.
 *   - When empty, `TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY` should be defined to `1`.
 * - `TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION`:
 *   - Extension that replaces everything after (and including) the last `.` within the last directory
 *     component of `__BASE_NAME__`.
 *   - Should be defined to whatever is the default extension of your compiler's object-file /
 *     source-cache / precompiled-source / whatever-you-want-to-call-it is.
 *
 * If these configuration macros aren't enough to accurately describe how your compiler's default
 * object file format looks like, you should just override `tpp_makefile_cli_print_default_target()`
 * directory (which can be done by pre-defining a macro of that same name before `#include`-ing
 * TPP's **MAKEFILE** source extension)
 *
 * Examples:
 * - C Compiler ("{PATH}/{filename}.o"):
 *   ```c
 *   #define TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION ".o"
 *   ```
 *
 * - Java Compiler ("{PATH}/{filename}.class"):
 *   ```c
 *   #define TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION ".class"
 *   ```
 *
 * - Deemon ("{PATH}/.{filename}.dec"):
 *   ```c
 *   #define TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX "."
 *   #define TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION ".dec"
 *   ```
 */
#ifndef TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX
#define TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX ""
#define TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY 1
#endif /* !TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX */
#ifndef TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY
#define TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY 0
#endif /* !TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY */
#ifndef TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX
#define TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX ""
#define TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY 1
#endif /* !TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX */
#ifndef TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY
#define TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY 0
#endif /* !TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY */
#ifndef TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION
#ifdef _MSC_VER
#define TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION ".obj"
#else /* _MSC_VER */
#define TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION ".o"
#endif /* !_MSC_VER */
#endif /* !TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION */

/************************************************************************/
/* File: parts/optional/makefile/mf-io.h                                */
/************************************************************************/
TPP_DECL_BEGIN

#if TPP_MAKEFILE_HAVE_IO_HANDLE

#ifndef tpp_makefile_io_handle
#define tpp_makefile_io_handle_IS_BUILTIN
#ifdef tpp_io_handle_IS_BUILTIN
#define tpp_makefile_io_handle tpp_io_handle
#ifdef tpp_io_handle_IS_HANDLE
#define tpp_makefile_io_handle_IS_HANDLE
#endif /* !tpp_io_handle_IS_HANDLE */
#ifdef tpp_io_handle_IS_int
#define tpp_makefile_io_handle_IS_int
#endif /* !tpp_io_handle_IS_int */
#ifdef tpp_io_handle_IS_FILE
#define tpp_makefile_io_handle_IS_FILE
#endif /* !tpp_io_handle_IS_FILE */
#elif TPP_OS_WINDOWS
#if !TPP_HOST_NO_SYSTEM_INCLUDES
TPP_DECL_END
#include <Windows.h>
TPP_DECL_BEGIN
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_makefile_io_handle HANDLE
#define tpp_makefile_io_handle_IS_HANDLE
#elif TPP_OS_UNIX
#define tpp_makefile_io_handle int
#define tpp_makefile_io_handle_IS_int
#else /* ... */
#if !TPP_HOST_NO_SYSTEM_INCLUDES
TPP_DECL_END
#include <stdio.h>
TPP_DECL_BEGIN
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_makefile_io_handle FILE *
#define tpp_makefile_io_handle_IS_FILE
#endif /* !... */
#endif /* !tpp_makefile_io_handle */


#ifdef tpp_makefile_io_handle_IS_BUILTIN

/* Return a handle for the hosting process's STDOUT stream */
#ifdef tpp_makefile_io_handle_IS_HANDLE
#define tpp_makefile_io_getstdout() GetStdHandle(STD_OUTPUT_HANDLE)
#elif defined(tpp_makefile_io_handle_IS_int)
#ifdef STDOUT_FILENO
#define tpp_makefile_io_getstdout() STDOUT_FILENO
#else /* STDOUT_FILENO */
#define tpp_makefile_io_getstdout() 1
#endif /* !STDOUT_FILENO */
#elif defined(tpp_makefile_io_handle_IS_FILE)
#define tpp_makefile_io_getstdout() stdout
#endif /* ... */

/* Open a file for writing
 * @return: TPP_EOK:    Success (*p_result was populated and must eventually be closed by caller)
 * @return: TPP_ENOENT: Some parent-path-component of `filename` does not exist
 * @return: TPP_EIO:    I/O error
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_io_open(/*utf-8*/ char const *tpp_restrict filename,
                     tpp_makefile_io_handle *tpp_restrict p_result);

/* Close a file previously opened by `tpp_makefile_io_open()` */
#ifdef tpp_io_handle_IS_BUILTIN
#define tpp_makefile_io_close(file) tpp_io_close(file)
#else /* tpp_io_handle_IS_BUILTIN */
TPP_DECL void TPPCALL tpp_makefile_io_close(tpp_makefile_io_handle file);
#endif /* !tpp_io_handle_IS_BUILTIN */

/* Write data from `buf` into a given `file`
 * @return: * : The # of bytes written from `buf` (at most `bufsize`)
 * @return: TPP_SSIZE_OFERR(TPP_EIO):    I/O error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM): Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((2)) tpp_ssize TPPCALL
tpp_makefile_io_write(tpp_makefile_io_handle file, void const *buf, tpp_size bufsize);

#endif /* tpp_makefile_io_handle_IS_BUILTIN */
#endif /* TPP_MAKEFILE_HAVE_IO_HANDLE */

/************************************************************************/
/* File: parts/optional/makefile/mf-features.h                          */
/************************************************************************/

#undef TPP_MAKEFILE_HAVE_FEATURES
#if (TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) ||\
     TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY))
#define TPP_MAKEFILE_HAVE_FEATURES 1
#else /* ... */
#define TPP_MAKEFILE_HAVE_FEATURES 0
#endif/* !... */

#if TPP_MAKEFILE_HAVE_FEATURES
typedef enum tpp_makefile_feature_id {
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
	TPP_MAKEFILE_FEAT_USER_DEPENDENCIES,
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY)
	TPP_MAKEFILE_FEAT_PHONY,
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY) */
	TPP_MAKEFILE_FEAT_COUNT
} tpp_makefile_feature_id;

typedef union tpp_makefile_features {
	struct {
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
		unsigned int TPP_MAKEFILE_INTERNAL(tmkff_USER_DEPENDENCIES): 1;
#define _tpp_makefile_has_USER_DEPENDENCIES(self) (self)->TPP_MAKEFILE_INTERNAL(tmkf_feat).TPP_MAKEFILE_INTERNAL(tmkf_flags).TPP_MAKEFILE_INTERNAL(tmkff_USER_DEPENDENCIES)
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY)
		unsigned int TPP_MAKEFILE_INTERNAL(tmkff_PHONY): 1;
#define _tpp_makefile_has_PHONY(self) (self)->TPP_MAKEFILE_INTERNAL(tmkf_feat).TPP_MAKEFILE_INTERNAL(tmkf_flags).TPP_MAKEFILE_INTERNAL(tmkff_PHONY)
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY) */
	} TPP_MAKEFILE_INTERNAL(tmkf_flags);
	unsigned char TPP_MAKEFILE_INTERNAL(tmkf_bitset)[TPP_MAKEFILE_FEAT_COUNT ? ((TPP_MAKEFILE_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_makefile_features;

#if !TPP_USE_STATIC
TPP_CONST_DECL tpp_makefile_features const tpp_makefile_features_default;
#endif /* !TPP_USE_STATIC */

#define tpp_makefile_features_getid(self, id) \
	((self)->TPP_MAKEFILE_INTERNAL(tmkf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_makefile_features_enable(self, id) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_makefile_features_disable(self, id) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_makefile_features_setid(self, id, enabled) \
	((enabled) ? tpp_makefile_features_enable(self, id) : tpp_makefile_features_disable(self, id))
#define tpp_makefile_features_init(self)            (void)(*(self) = tpp_makefile_features_default)
#define tpp_makefile_features_reset(self)           (void)(*(self) = tpp_makefile_features_default)
#define tpp_makefile_features_fini(self)            tpp_dbg_memset(self, sizeof(tpp_makefile_features))
#endif /* TPP_MAKEFILE_HAVE_FEATURES */

#if TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
#define _tpp_makefile_has_USER_DEPENDENCIES(self) TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
#endif /* TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
#if TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_PHONY)
#define _tpp_makefile_has_PHONY(self) TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_PHONY)
#endif /* TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_PHONY) */

/************************************************************************/
/* File: parts/optional/makefile/mf.h                                   */
/************************************************************************/

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
#ifndef TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER
	tpp_lexer *TPP_MAKEFILE_INTERNAL(tmkf_lexer);
#define _tpp_makefile_init_lexer(self, lexer) (self)->TPP_MAKEFILE_INTERNAL(tmkf_lexer) = (lexer)
#else /* !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define _tpp_makefile_init_lexer(self, lexer) tpp_assert(tpp_makefile_getlexer(self) == (lexer))
#endif /* TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */

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
#define tpp_makefile_init(self, lexer, output)                   \
	(void)(_tpp_makefile_init_lexer(self, lexer),                \
	       (self)->TPP_MAKEFILE_INTERNAL(tmkf_output) = (output) \
	       _tpp_makefile_init_output_file(self)                  \
	       _tpp_makefile_init_feat(self)                         \
	       _tpp_makefile_init_flags(self)                        \
	       _tpp_makefile_init_depv(self)                         \
	       _tpp_makefile_init_col(self))
#define tpp_makefile_fini(self)                                  \
	(void)((void)0 _tpp_makefile_fini_depv(self)                 \
	       _tpp_makefile_fini_output_file(self),                 \
	       /* Disable any hooks that may have been enabled */    \
	       tpp_makefile_disable(self),                           \
	       tpp_makefile_disable_missing_file_dependencies(self), \
	       tpp_dbg_memset(self, sizeof(tpp_makefile)))


/* Retrieve components of the makefile. */
#define tpp_makefile_getoutput(self) (self)->TPP_MAKEFILE_INTERNAL(tmkf_output)
#ifdef TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER
#define tpp_makefile_getlexer(self)  ((tpp_lexer *)((char *)(self) - TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER))
#define _tpp_makefile_oflexer(lexer) ((tpp_makefile *)((char *)(cookie) + TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER))
#else /* TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define tpp_makefile_getlexer(self)  ((self)->TPP_MAKEFILE_INTERNAL(tmkf_lexer))
#endif /* !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */


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
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_NEW_DEPENDENCY_HOOK)
#define tpp_makefile_enable(self)     tpp_lexer_addhook_new_dependency_ex(tpp_makefile_getlexer(self), &_tpp_makefile_new_dependency_hook, self)
#define tpp_makefile_getenabled(self) tpp_lexer_hashook_new_dependency_ex(tpp_makefile_getlexer(self), &_tpp_makefile_new_dependency_hook, self)
#define tpp_makefile_disable(self)    tpp_lexer_delhook_new_dependency_ex(tpp_makefile_getlexer(self), &_tpp_makefile_new_dependency_hook, self)
#define _tpp_makefile_new_dependency_hook_cookie      void *
#define _tpp_makefile_new_dependency_hook_ofcookie(x) ((tpp_makefile *)(x))
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
#define tpp_makefile_enable(self)     tpp_lexer_addhook_new_dependency(tpp_makefile_getlexer(self), &_tpp_makefile_new_dependency_hook)
#define tpp_makefile_getenabled(self) tpp_lexer_hashook_new_dependency(tpp_makefile_getlexer(self), &_tpp_makefile_new_dependency_hook)
#define tpp_makefile_disable(self)    tpp_lexer_delhook_new_dependency(tpp_makefile_getlexer(self), &_tpp_makefile_new_dependency_hook)
#define _tpp_makefile_new_dependency_hook_cookie tpp_lexer *
#ifdef TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER
#define _tpp_makefile_new_dependency_hook_ofcookie(x) _tpp_makefile_oflexer(x)
#else /* TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define _tpp_makefile_new_dependency_hook_ofcookie(x) ((tpp_emitter *)((x) + 1))
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "`TPP_HAVE_NEW_DEPENDENCY_HOOK` is configured without cookies, but `TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER` isn't defined"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
#define tpp_makefile_setenabled(self, enabled) \
	((enabled) ? tpp_makefile_enable(self)     \
	           : (tpp_makefile_disable(self), TPP_EOK))
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_makefile_new_dependency_hook(_tpp_makefile_new_dependency_hook_cookie cookie,
                                  tpp_keyword *filename_kwd);
#else /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
#define tpp_makefile_getenabled(self) false
#define tpp_makefile_disable(self)    (void)0
#endif /* !TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */



/* Handle missing file dependencies by (blindly) emitting them to the makefile */
#if TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK)
#define tpp_makefile_enable_missing_file_dependencies(self)      tpp_lexer_addhook_include_not_found_ex(tpp_makefile_getlexer(self), &_tpp_makefile_include_not_found_hook, self)
#define tpp_makefile_get_missing_file_dependencies_enabled(self) tpp_lexer_hashook_include_not_found_ex(tpp_makefile_getlexer(self), &_tpp_makefile_include_not_found_hook, self)
#define tpp_makefile_disable_missing_file_dependencies(self)     tpp_lexer_delhook_include_not_found_ex(tpp_makefile_getlexer(self), &_tpp_makefile_include_not_found_hook, self)
#define _tpp_makefile_include_not_found_hook_cookie      void *
#define _tpp_makefile_include_not_found_hook_ofcookie(x) ((tpp_makefile *)(x))
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */
#define tpp_makefile_enable_missing_file_dependencies(self)      tpp_lexer_addhook_include_not_found(tpp_makefile_getlexer(self), &_tpp_makefile_include_not_found_hook)
#define tpp_makefile_get_missing_file_dependencies_enabled(self) tpp_lexer_hashook_include_not_found(tpp_makefile_getlexer(self), &_tpp_makefile_include_not_found_hook)
#define tpp_makefile_disable_missing_file_dependencies(self)     tpp_lexer_delhook_include_not_found(tpp_makefile_getlexer(self), &_tpp_makefile_include_not_found_hook)
#define _tpp_makefile_include_not_found_hook_cookie tpp_lexer *
#ifdef TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER
#define _tpp_makefile_include_not_found_hook_ofcookie(x) _tpp_makefile_oflexer(x)
#else /* TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define _tpp_makefile_include_not_found_hook_ofcookie(x) ((tpp_emitter *)((x) + 1))
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "`TPP_HAVE_INCLUDE_NOT_FOUND_HOOK` is configured without cookies, but `TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER` isn't defined"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_INCLUDE_NOT_FOUND_HOOK) */
#define tpp_makefile_set_missing_file_dependencies_enabled(self, v) \
	((v) ? tpp_makefile_enable_missing_file_dependencies(self)      \
	     : (tpp_makefile_disable_missing_file_dependencies(self), TPP_EOK))
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno
_tpp_makefile_include_not_found_hook(_tpp_makefile_include_not_found_hook_cookie cookie,
                                     tpp_hook_include_kind include_kind);
#else /* TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */
#define tpp_makefile_get_missing_file_dependencies_enabled(self) 0
#define tpp_makefile_disable_missing_file_dependencies(self)     (void)0
#endif /* !TPP_MAKEFILE_HAVE_MISSING_FILE_DEPENDENCIES */



/* Apply Makefile-escape rules to `data...+=num_bytes` and print the result to `printer` */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_ssize TPPCALL
tpp_makefile_escape(tpp_formatprinter printer, void *arg,
                    tpp_char const *data, tpp_size num_bytes,
                    tpp_size *p_num_bytes_printed);

/************************************************************************/
/* File: parts/optional/makefile/mf-cli.h                               */
/************************************************************************/

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

#endif /* !GUARD_TPP_MAKEFILE_AMALGAMATION_H */
/* clang-format on */
