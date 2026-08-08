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
 * - TODO
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
/* When enabled, only include dependencies if
 * `#include`-stack doesn't contain any system
 * headers */
#ifndef TPP_MAKEFILE_HAVE_USER_DEPENDENCIES
#define TPP_MAKEFILE_HAVE_USER_DEPENDENCIES (TPP_CONF_FEAT0)
#endif /* !TPP_MAKEFILE_HAVE_USER_DEPENDENCIES */


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

/************************************************************************/
/* File: parts/optional/makefile/mf-features.h                          */
/************************************************************************/
TPP_DECL_BEGIN

#undef TPP_MAKEFILE_HAVE_FEATURES
#if (TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES))
#define TPP_MAKEFILE_HAVE_FEATURES 1
#else /* ... */
#define TPP_MAKEFILE_HAVE_FEATURES 0
#endif/* !... */

#if TPP_MAKEFILE_HAVE_FEATURES
typedef enum tpp_makefile_feature_id {
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
	TPP_MAKEFILE_FEAT_USER_DEPENDENCIES,
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
	TPP_MAKEFILE_FEAT_COUNT
} tpp_makefile_feature_id;

typedef union tpp_makefile_features {
	struct {
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
		unsigned int TPP_MAKEFILE_INTERNAL(teff_USER_DEPENDENCIES): 1;
#define _tpp_makefile_has_USER_DEPENDENCIES(self) (self)->TPP_MAKEFILE_INTERNAL(te_feat).TPP_MAKEFILE_INTERNAL(tef_flags).TPP_MAKEFILE_INTERNAL(teff_USER_DEPENDENCIES)
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
	} TPP_MAKEFILE_INTERNAL(tef_flags);
	unsigned char TPP_MAKEFILE_INTERNAL(tetf_bitset)[TPP_MAKEFILE_FEAT_COUNT ? ((TPP_MAKEFILE_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_makefile_features;

#if !TPP_USE_STATIC
TPP_CONST_DECL tpp_makefile_features const tpp_makefile_features_default;
#endif /* !TPP_USE_STATIC */

#define tpp_makefile_features_getid(self, id) \
	((self)->TPP_MAKEFILE_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_makefile_features_enable(self, id) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_makefile_features_disable(self, id) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_makefile_features_setid(self, id, enabled) \
	((enabled) ? tpp_makefile_features_enable(self, id) : tpp_makefile_features_disable(self, id))
#define tpp_makefile_features_init(self)            (void)(*(self) = tpp_makefile_features_default)
#define tpp_makefile_features_reset(self)           (void)(*(self) = tpp_makefile_features_default)
#define tpp_makefile_features_fini(self)            tpp_dbg_memset(self, sizeof(tpp_makefile_features))
#endif /* TPP_MAKEFILE_HAVE_FEATURES */

#if TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
#define _tpp_makefile_has_USER_DEPENDENCIES(self) TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
#endif /* TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */

/************************************************************************/
/* File: parts/optional/makefile/mf.h                                   */
/************************************************************************/

#undef TPP_MAKEFILE_HAVE_FLAGS
#if 0 /* TODO */
#define TPP_MAKEFILE_HAVE_FLAGS 1
#else /* ... */
#define TPP_MAKEFILE_HAVE_FLAGS 0
#endif /* !... */

#if TPP_MAKEFILE_HAVE_FLAGS
#define tpp_makefile_flags uint_least8_t
#define TPP_MAKEFILE_FLAG_NORMAL UINT8_C(0x00)
#endif /* TPP_MAKEFILE_HAVE_FLAGS */


typedef struct tpp_makefile {
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER)
	tpp_lexer            *TPP_MAKEFILE_INTERNAL(tmf_lexer);  /* [1..1][const] The lexer whose tokens are being emitted */
#define _tpp_makefile_init_lexer(self, lexer) (self)->TPP_MAKEFILE_INTERNAL(tmf_lexer) = (lexer)
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define _tpp_makefile_init_lexer(self, lexer) tpp_assert(tpp_makefile_getlexer(self) == (lexer))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
	tpp_formatprinter     TPP_MAKEFILE_INTERNAL(tmf_output); /* [1..1][const] Makefile output printer (the makefile itself will be passed as argument) */
#if TPP_MAKEFILE_HAVE_FEATURES
	tpp_makefile_features TPP_MAKEFILE_INTERNAL(tmf_feat);   /* Makefile feature configuration */
#define _tpp_makefile_init_feat(self) , tpp_makefile_features_init(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat))
#define _tpp_makefile_fini_feat(self) , tpp_makefile_features_fini(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat))
#else /* TPP_MAKEFILE_HAVE_FEATURES */
#define _tpp_makefile_init_feat(self) /* nothing */
#define _tpp_makefile_fini_feat(self) /* nothing */
#endif /* !TPP_MAKEFILE_HAVE_FEATURES */
} tpp_makefile;

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given makefile.
 *
 * @param: lexer:  The lexer whose tokens are being emitted
 * @param: output: Default output printer. On error, must return one of `TPP_SSIZE_OFERR(*)` */
#define tpp_makefile_init(self, lexer, output)                  \
	(void)(_tpp_makefile_init_lexer(self, lexer),               \
	       (self)->TPP_MAKEFILE_INTERNAL(tmf_output) = (output) \
	       _tpp_makefile_init_feat(self))
#define tpp_makefile_fini(self) \
	tpp_dbg_memset(self, sizeof(tpp_makefile))

/* Retrieve components of the makefile. */
#define tpp_makefile_getoutput(self) (self)->TPP_MAKEFILE_INTERNAL(tmf_output)
#if TPP_HAVE_HOOK_COOKIES && !defined(TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER)
#define tpp_makefile_getlexer(self)   ((self)->TPP_MAKEFILE_INTERNAL(tmf_lexer))
#define tpp_makefile_ofcookie(cookie) ((tpp_makefile *)(cookie))
#else /* TPP_HAVE_HOOK_COOKIES && !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#ifndef TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER
#error "Invalid configuration: under '-DTPP_HAVE_HOOK_COOKIES=0' you must specify a macro '#define TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER (offsetof(MY_CONTAINER, makefile) - offsetof(MY_CONTAINER, lexer))' to specify how to retrieve the makefile from a lexer"
#endif /* !TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */
#define tpp_makefile_getlexer(self)   ((tpp_lexer *)((char *)(self) - TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER))
#define tpp_makefile_ofcookie(cookie) ((tpp_makefile *)((char *)(self) + TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER))
#endif /* !TPP_HAVE_HOOK_COOKIES || TPP_CONFIG_OFFSETOF_MAKEFILE_FROM_LEXER */

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
#define tpp_makefile_getfeature(self, TPP_MAKEFILE_FEAT_x)          tpp_makefile_features_getid(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat), TPP_MAKEFILE_FEAT_x)
#define tpp_makefile_setfeature(self, TPP_MAKEFILE_FEAT_x, enabled) tpp_makefile_features_setid(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat), TPP_MAKEFILE_FEAT_x, enabled)
#define tpp_makefile_enablefeature(self, TPP_MAKEFILE_FEAT_x)       tpp_makefile_features_enable(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat), TPP_MAKEFILE_FEAT_x)
#define tpp_makefile_disablefeature(self, TPP_MAKEFILE_FEAT_x)      tpp_makefile_features_disable(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat), TPP_MAKEFILE_FEAT_x)
#define tpp_makefile_resetfeatures(self)                            tpp_makefile_features_reset(&(self)->TPP_MAKEFILE_INTERNAL(tmf_feat))
#else /* TPP_MAKEFILE_HAVE_FEATURES */
#define tpp_makefile_getfeature(self, TPP_MAKEFILE_FEAT_x) 0
#define tpp_makefile_resetfeatures(self)                   (void)0
#endif /* !TPP_MAKEFILE_HAVE_FEATURES */

/* TODO: API to enable/disable the makefile's callback into the lexer's `NEW_DEPENDENCY` hook */

/************************************************************************/
/* File: parts/optional/makefile/mf-cli.h                               */
/************************************************************************/

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
#if !TPP_USE_STATIC
TPP_CONST_DECL char const tpp_makefile_cli_loader_help[];
#endif /* !TPP_USE_STATIC */
#endif /* TPP_MAKEFILE_HAVE_CLI_HELP */
#endif /* TPP_MAKEFILE_HAVE_CLI */

TPP_DECL_END

#endif /* !GUARD_TPP_MAKEFILE_AMALGAMATION_H */
/* clang-format on */
