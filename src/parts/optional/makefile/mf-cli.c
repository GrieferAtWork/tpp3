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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_MF_CLI_C
#define GUARD_TPP_OPTIONAL_MAKEFILE_MF_CLI_C 1
#define TPP_BUILDING_OPTIONAL 1
#define TPP_MAKEFILE_BUILDING 1

#include "api.h"

#include "config.h"
#include "mf-cli.h"
#include "mf.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_MAKEFILE_HAVE_CLI

/* Define a function `tpp_makefile_cli_warnf()` */
#undef TPP_HAVE_MAKEFILE_CLI_WARN
#define TPP_HAVE_MAKEFILE_CLI_WARN \
	(TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT)

#if TPP_HAVE_MAKEFILE_CLI_WARN
static TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_cli_warnf(tpp_makefile *tpp_restrict self, tpp_char const *token_start,
                       tpp_size token_size, tpp_warning_id id, ...) {
	/* XXX: Use of `TPP_INTERNAL` here isn't allowed -- come up with
	 *      APIs to allow users to do this stuff without needing to
	 *      access TPP internal! */
	tpp_lexer *const lexer = tpp_makefile_getlexer(self);
	tpp_errno result;
	va_list args;
	union TPP_INTERNAL(tpp_lexer_core) saved_core = lexer->TPP_INTERNAL(tl_core);
	tpp_file *const file = tpp_lexer_getfile(lexer);
	tpp_file_init_text_utf8(file, TPP_CONFIG_CLI_FILENAME,
	                        NULL, token_start, token_size,
	                        TPP_LCINFO_INVALID, TPP_FILE_FLAGS_NORMAL);
	file->TPP_INTERNAL(tf_tpos) = token_start;
	file->TPP_INTERNAL(tf_pos)  = token_start + token_size;
	va_start(args, id);
	result = tpp_lexer_vwarnf(lexer, id, args);
	va_end(args);
	lexer->TPP_INTERNAL(tl_core) = saved_core;
	return result;
}
#endif /* TPP_HAVE_MAKEFILE_CLI_WARN */


enum {
	_TPP_MAKEFILE_CLI_LOADER_STATE_FIRST_INTERNAL = TPP_MAKEFILE_CLI_LOADER_STATE_DDASH,
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
	TPP_MAKEFILE_CLI_LOADER_STATE_MF, /* -MF FILE */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
	TPP_MAKEFILE_CLI_LOADER_STATE_MT, /* -MT TARGET */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
	TPP_MAKEFILE_CLI_LOADER_STATE_MQ, /* -MQ TARGET */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
};


/* Make makefile for being turned on (during `tpp_makefile_cli_loader_flush()`) */
#if TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM || TPP_MAKEFILE_HAVE_CLI_DASH_MG
static TPP_NONNULL((1)) void TPPCALL
tpp_makefile_cli_loader_enable_with_makefile_only(tpp_makefile_cli_loader *tpp_restrict self) {
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
	tpp_makefile_cli_loader_enableonlymakefile(self);
#endif /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
	tpp_makefile_cli_loader_enablemakefile(self);
}
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM || TPP_MAKEFILE_HAVE_CLI_DASH_MG */

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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_cli_loader_parsearg(tpp_makefile_cli_loader *tpp_restrict self, char const *arg) {
#define tpp_streq(at, CONSTstr) \
	(tpp_memcmp(at, CONSTstr, sizeof(CONSTstr) - sizeof(char)) == 0)
	switch (self->tmkfcl_state) {

	case TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL: {
		if (*arg++ != '-')
			break; /* All arguments must start with at least 1 "-" */
		switch (*arg++) {

		case '-':
			/* --foo argument */
			switch (*arg++) {

			case '\0':
				self->tmkfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_DDASH; /* -- */
				return TPP_EOK;

			case 'd':
#if TPP_MAKEFILE_HAVE_CLI_DASH_M
				if (tpp_streq(arg, "ependencies\0")) { /* --dependencies */
					tpp_makefile_cli_loader_enable_with_makefile_only(self);
					return TPP_EOK;
				} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M */
				{
				}
				break;

			case 'u':
#if TPP_MAKEFILE_HAVE_CLI_DASH_MM
				if (tpp_streq(arg, "ser-dependencies\0")) { /* --user-dependencies */
					tpp_makefile_enablefeature(self->tmkfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
					tpp_makefile_cli_loader_enable_with_makefile_only(self);
					return TPP_EOK;
				} else
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MM */
				{
				}
				break;

			case 'p':
#if TPP_MAKEFILE_HAVE_CLI_DASH_MG
				if (tpp_streq(arg, "rint-missing-file-dependencies\0")) { /* --print-missing-file-dependencies */
					tpp_makefile_cli_loader_enable_with_makefile_only(self);
					return tpp_makefile_enable_missing_file_dependencies(self->tmkfcl_mf);
				} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MG */
				{
				}
				break;

			default: break;
			}
			break;

		case 'M':
#if TPP_MAKEFILE_HAVE_CLI_DASH_MMD
			if (tpp_streq(arg, "MD\0")) {
				self->tmkfcl_flags |= _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT;
				tpp_makefile_enablefeature(self->tmkfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
				tpp_makefile_cli_loader_enablemakefile(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MMD */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MD
			if (tpp_streq(arg, "D\0")) {
				self->tmkfcl_flags |= _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT;
				tpp_makefile_cli_loader_enablemakefile(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MD */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
			if (*arg == 'F') {
				if (*++arg) {
					self->tmkfcl_outfile = arg; /* -MFfoo.m */
				} else {
					self->tmkfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_MF; /* -MF foo.m */
				}
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MP
			if (tpp_streq(arg, "P\0")) {
				tpp_makefile_enablefeature(self->tmkfcl_mf, TPP_MAKEFILE_FEAT_PHONY);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MP */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
			if (*arg == 'T') {
				if (*++arg) {
					tpp_makefile_cli_loader_settarget_mt(self, arg); /* -MTmain.o */
				} else {
					self->tmkfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_MT; /* -MT main.o */
				}
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
			if (*arg == 'Q') {
				if (*++arg) {
					tpp_makefile_cli_loader_settarget_mq(self, arg); /* -MQmain.o */
				} else {
					self->tmkfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_MQ; /* -MQ main.o */
				}
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MG
			if (tpp_streq(arg, "G\0")) {
				tpp_makefile_cli_loader_enable_with_makefile_only(self);
				return tpp_makefile_enable_missing_file_dependencies(self->tmkfcl_mf);
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MG */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MM
			if (tpp_streq(arg, "M\0")) {
				tpp_makefile_enablefeature(self->tmkfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
				tpp_makefile_cli_loader_enable_with_makefile_only(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MM */
#if TPP_MAKEFILE_HAVE_CLI_DASH_M
			if (tpp_streq(arg, "\0")) {
				tpp_makefile_cli_loader_enable_with_makefile_only(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M */
			{
			}
			break;

		default: break;
		}
	}	break;

	case TPP_MAKEFILE_CLI_LOADER_STATE_DDASH:
		break; /* Don't accept any more arguments after having encountered a "--" arguments */

#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
	case TPP_MAKEFILE_CLI_LOADER_STATE_MF:
		self->tmkfcl_state   = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL;
		self->tmkfcl_outfile = arg;
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */

#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
	case TPP_MAKEFILE_CLI_LOADER_STATE_MT:
		self->tmkfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL;
		tpp_makefile_cli_loader_settarget_mt(self, arg);
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */

#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
	case TPP_MAKEFILE_CLI_LOADER_STATE_MQ:
		self->tmkfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL;
		tpp_makefile_cli_loader_settarget_mq(self, arg);
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MQ */

	default: tpp_unreachable();
	}
	return TPP_ENOENT;
#undef tpp_streq
}


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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_cli_loader_parseflag(tpp_makefile_cli_loader *tpp_restrict self, char const **p_arg) {
	char const *arg = *p_arg;
	char flag = *arg++;
	(void)self;
	(void)flag;
	(void)arg;
	switch (flag) {

		/* No flags defined... */

	default: break;
	}
	return TPP_ENOENT;
}



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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_makefile_cli_loader_parseargv(tpp_makefile_cli_loader *tpp_restrict self,
                                 int *p_argc, char ***p_argv) {
	tpp_errno result = TPP_EOK;
	int argc    = *p_argc;
	char **argv = *p_argv;
	unsigned int unknown_count = 0;
	while (argc > 0) {
		char *arg = argv[0];
		result = tpp_makefile_cli_loader_parsearg(self, arg);
		if (TPP_ISERR(result)) {
			if (result != TPP_ENOENT)
				break;
			/* Add "arg" to trailing list of unknown arguments */
			--argc;
			(void)tpp_memmovedown(&argv[0], &argv[1],
			                      (argc + unknown_count) *
			                      sizeof(char *));
			argv[argc + unknown_count] = arg;
			++unknown_count;
			result = TPP_EOK;
			continue;
		}
		if (tpp_makefile_cli_loader_hasddash(self)) {
			if (unknown_count) {
				/* Right now, "argv" looks like this:
				 * >> argv = { "--", "file1.c", "file2.c", "-unknown-arg", "file0.c" }
				 * >> argc = 3
				 * >> unknown_count = 2
				 *
				 * Our job now is to make `argv` look like this:
				 * >> argv = { "-unknown-arg", "file0.c", "--", "file1.c", "file2.c" } */
				unsigned int shift_count = unknown_count;
				unsigned int total_count_minus_1 = argc + unknown_count - 1;
				while (shift_count--) {
					arg = argv[0];
					(void)tpp_memmovedown(&argv[0], &argv[1],
					                      total_count_minus_1 *
					                      sizeof(char *));
					argv[total_count_minus_1] = arg;
				}
			}
			break;
		}
		++argv;
		--argc;
	}
	*p_argc = argc + unknown_count;
	*p_argv = argv;
	return result;
}

struct tpp_makefile_cli_default_target_data {
	tpp_makefile *tmfcdtd_mf;    /* [1..1][const] Makefile */
	tpp_size      tmfcdtd_count; /* # of printed bytes */
};

TPP_FORMATPRINTER_DEFINE(tpp_makefile_cli_default_target_printer, arg, text, num_bytes) {
	tpp_size count;
	tpp_ssize result;
	struct tpp_makefile_cli_default_target_data *data;
	data   = (struct tpp_makefile_cli_default_target_data *)arg;
	result = tpp_makefile_escape(tpp_makefile_getoutput(data->tmfcdtd_mf),
	                             data->tmfcdtd_mf, text, num_bytes, &count);
	data->tmfcdtd_count += count;
	return result;
}

#if TPP_MAKEFILE_HAVE_CLI_ENV_MD || TPP_MAKEFILE_HAVE_CLI_ENV_MMD
static char const tpp_makefile_cli_loader_target_printed_marker[] = "";

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_makefile_cli_loader_configure_with_env_impl(tpp_makefile_cli_loader *tpp_restrict self,
                                                char const *env_value) {
	tpp_errno error;
	char const *target_start;

	/* Turn on the makefile */
	tpp_makefile_cli_loader_enablemakefile(self); /* Set flag that CLI turned on Makefile */
	error = tpp_makefile_enable(self->tmkfcl_mf); /* Turn on the actual makefile */
	if (TPP_ISERR(error))
		return error;

	/* Find start of Makefile target in environment variable.
	 * NOTE: We mirror GCC behavior here, in that we *only* accept U+0020 SPACE
	 *       as separator between filename and target, with no way of quoting
	 *       or escaping the filename such that it could contain SPACE, too. */
	target_start = tpp_strchr(env_value, ' ');
	if (target_start) {
		tpp_ssize output_temp;
		tpp_size output_count;
		tpp_size filename_len;
		char *filename_copy;

		/* Set output filename of Makefile */
		filename_len = (tpp_size)(target_start - env_value);
#ifdef tpp_alloca
		filename_copy = (char *)tpp_alloca((filename_len + 1) * sizeof(char));
		*(char *)tpp_mempcpy(filename_copy, env_value, filename_len * sizeof(char)) = '\0';
		error = tpp_makefile_setoutput_file(self->tmkfcl_mf, filename_copy);
#else /* tpp_alloca */
		filename_copy = (char *)tpp_malloc((filename_len + 1) * sizeof(char));
		if tpp_unlikely(!filename_copy)
			return TPP_ENOMEM;
		*(char *)tpp_mempcpy(filename_copy, env_value, filename_len * sizeof(char)) = '\0';
		error = tpp_makefile_setoutput_file(self->tmkfcl_mf, filename_copy);
		tpp_free(filename_copy);
#endif /* !tpp_alloca */
		if (TPP_ISERR(error))
			return error;

		/* Also mirroring GCC: if there might be multiple, consecutive
		 * spaces, then we *only* skip the first of them. */
		++target_start;

		/* Print target name if environment variable contains it. */
		output_count = tpp_strlen(target_start);
		output_temp = tpp_makefile_output_printraw_cstr(self->tmkfcl_mf, target_start, output_count);
		if (output_temp < 0)
			return TPP_SSIZE_ASERR(output_temp);
		(void)output_count;
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
		self->tmkfcl_mf->tmkf_curcol = (tpp_column)output_count;
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

		/* Remember that we've already printed the target */
		self->tmkfcl_target = tpp_makefile_cli_loader_target_printed_marker;
	} else {
		/* No target name contained in variable
		 * -> entire variable is name of Makefile output file */
		error = tpp_makefile_setoutput_file(self->tmkfcl_mf, env_value);
	}

	return error;
}

#if TPP_MAKEFILE_HAVE_CLI_ENV_MD
static TPP_NOINLINE tpp_errno TPPCALL
tpp_makefile_cli_loader_configure_with_env_md(void *arg, char const *envvalue) {
	tpp_makefile_cli_loader *me = (tpp_makefile_cli_loader *)arg;
	/* Emit *all* dependencies... */
#if TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
	tpp_makefile_disablefeature(me->tmkfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
#endif /* TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */

	/* ... but actually wait: don't emit dependencies for the preprocessor "main" file(s) */
#if TPP_MAKEFILE_HAVE_CLI_ENV_MD_OMITS_MAIN_FILE
	tpp_makefile_cli_loader_enableomitmainfile(me);
#endif /* TPP_MAKEFILE_HAVE_CLI_ENV_MD_OMITS_MAIN_FILE */
	return tpp_makefile_cli_loader_configure_with_env_impl(me, envvalue);
}
#endif /* TPP_MAKEFILE_HAVE_CLI_ENV_MD */

#if TPP_MAKEFILE_HAVE_CLI_ENV_MMD
static TPP_NOINLINE tpp_errno TPPCALL
tpp_makefile_cli_loader_configure_with_env_mmd(void *arg, char const *envvalue) {
	tpp_makefile_cli_loader *me = (tpp_makefile_cli_loader *)arg;
	/* Only emit *user*-dependencies (iow: don't emit dependencies
	 * if there are any system headers on the `#include`-stack) */
#if TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
	tpp_makefile_enablefeature(me->tmkfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
#endif /* TPP_CONF_ISRT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
	return tpp_makefile_cli_loader_configure_with_env_impl(me, envvalue);
}
#endif /* TPP_MAKEFILE_HAVE_CLI_ENV_MMD */
#endif /* TPP_MAKEFILE_HAVE_CLI_ENV_MD || TPP_MAKEFILE_HAVE_CLI_ENV_MMD */


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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_cli_loader_flush(tpp_makefile_cli_loader *tpp_restrict self,
                              char const *output_filename) {
	tpp_errno error;
	tpp_ssize output_temp;
	tpp_size output_count;

	/* Emit a warning if the CLI loader isn't in a neutral state */
#if TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT
	if (self->tmkfcl_state != TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL &&
	    self->tmkfcl_state != TPP_MAKEFILE_CLI_LOADER_STATE_DDASH) {
		error = tpp_makefile_cli_warnf(self->tmkfcl_mf, NULL, 0,
		                               TPP_W_MISSING_CLI_ARGUMENT);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT */

	/* Check if Makefile generation is enabled (if it isn't don't do anything else)
	 *
	 * NOTE: Checking environ only happens if CLI didn't already turn on Makefile.
	 *       As such, if both CLI and environ are present, then environ won't ever
	 *       get checked, meaning CLI overrides whatever environ might say. */
#if TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED
	if (!tpp_makefile_cli_loader_getmakefileenabled(self)) {
#if TPP_MAKEFILE_HAVE_CLI_ENV_MD || TPP_MAKEFILE_HAVE_CLI_ENV_MMD
		/* Check for environment variables. */
#if TPP_MAKEFILE_HAVE_CLI_ENV_MMD
		error = tpp_io_withenv(TPP_MAKEFILE_CONFIG_CLI_ENV_MMD,
		                       &tpp_makefile_cli_loader_configure_with_env_mmd,
		                       self);
		if (error == TPP_ENOENT)
#endif /* TPP_MAKEFILE_HAVE_CLI_ENV_MMD */
		{
#if TPP_MAKEFILE_HAVE_CLI_ENV_MD
			error = tpp_io_withenv(TPP_MAKEFILE_CONFIG_CLI_ENV_MD,
			                       &tpp_makefile_cli_loader_configure_with_env_md,
			                       self);
#endif /* TPP_MAKEFILE_HAVE_CLI_ENV_MD */
		}
		if (TPP_ISERR(error) && error != TPP_ENOENT)
			return error;
		if (tpp_makefile_cli_loader_getmakefileenabled(self)) {
			/* Makefile was enabled */
			if (self->tmkfcl_target == tpp_makefile_cli_loader_target_printed_marker)
				goto after_print_output_filename;
			goto do_print_output_filename;
		}
#endif /* TPP_MAKEFILE_HAVE_CLI_ENV_MD || TPP_MAKEFILE_HAVE_CLI_ENV_MMD */
		return TPP_EOK;
	}
#endif /* TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED */

	/* Turn on the linked makefile */
	error = tpp_makefile_enable(self->tmkfcl_mf);
	if (TPP_ISERR(error))
		return error;

	/* Redirect output to a custom file */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
	if (self->tmkfcl_outfile) {
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH
		if (tpp_strcmp(self->tmkfcl_outfile, "-") == 0) {
#ifdef tpp_makefile_io_getstdout
			tpp_makefile_io_handle handle = tpp_makefile_io_getstdout();
			tpp_makefile_setoutput_io_ex(self->tmkfcl_mf, handle, true);
#else /* tpp_makefile_io_getstdout */
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH' is enabled, but no way to retrieve STDOUT handle"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !tpp_makefile_io_getstdout */
		} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH */
		{
			error = tpp_makefile_setoutput_file(self->tmkfcl_mf,
			                                    self->tmkfcl_outfile);
			if (TPP_ISERR(error))
				return error;
		}
	} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
	{
#if TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD
		if (self->tmkfcl_flags & _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT) {
			char const *filename = output_filename;
			char const *filename_end;
			tpp_size filename_len;
			char *output_buf, *ptr;
			/* Auto-determine output filename when -MD or -MMD was used,
			 * based on `output_filename`, or if not given: __FILE__ */
			if (filename == NULL) {
				tpp_file const *lc = tpp_lexer_getlcfile(tpp_makefile_getlexer(self->tmkfcl_mf));
				filename = tpp_file_getrealfilename(lc);
				if (filename == NULL)
					filename = ""; /* Unknown filename :( */
			}
			filename_end = filename;
			filename_end += tpp_strlen(filename);
			for (;;) {
				char ch;
				if (filename_end <= filename) {
use_full_filename:
					filename_end += tpp_strlen(filename_end);
					break;
				}
				ch = *--filename_end;
				if (TPP_FS_ISSEP(ch))
					goto use_full_filename;
				if (ch == '.')
					break;
			}
			filename_len = (tpp_size)(filename_end - filename);
			output_buf = (char *)tpp_malloc((filename_len * sizeof(char)) +
			                                sizeof(TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION));
			if tpp_unlikely(!output_buf)
				return TPP_ENOMEM;
			ptr = (char *)tpp_mempcpy(output_buf, filename, filename_len * sizeof(char));
			(void)tpp_memcpy(ptr, TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION,
			                 sizeof(TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION));
			/* Use this one as filename */
			error = tpp_makefile_setoutput_file(self->tmkfcl_mf, output_buf);
			tpp_free(output_buf);
			if (TPP_ISERR(error))
				return error;
		}
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD */
	}

	/* Print makefile target */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ
	if (self->tmkfcl_target) {
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT && TPP_MAKEFILE_HAVE_CLI_DASH_MQ
		if (self->tmkfcl_flags & _TPP_MAKEFILE_CLI_LOADER_FLAG_TARGETESCAPE) {
			output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self->tmkfcl_mf), self->tmkfcl_mf,
			                                  (tpp_char const *)self->tmkfcl_target,
			                                  tpp_strlen(self->tmkfcl_target),
			                                  &output_count);
		} else {
			output_count = tpp_strlen(self->tmkfcl_target);
			output_temp = tpp_makefile_output_printraw_cstr(self->tmkfcl_mf, self->tmkfcl_target, output_count);
		}
#elif TPP_MAKEFILE_HAVE_CLI_DASH_MT
		output_count = tpp_strlen(self->tmkfcl_target);
		output_temp = tpp_makefile_output_printraw_cstr(self->tmkfcl_mf, self->tmkfcl_target, output_count);
#else /* ... */
		output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self->tmkfcl_mf), self->tmkfcl_mf,
		                                  (tpp_char const *)self->tmkfcl_target,
		                                  tpp_strlen(self->tmkfcl_target), &output_count);
#endif /* !... */
	} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
	{
#if TPP_MAKEFILE_HAVE_CLI_ENV_MD || TPP_MAKEFILE_HAVE_CLI_ENV_MMD
do_print_output_filename:
#endif /* TPP_MAKEFILE_HAVE_CLI_ENV_MD || TPP_MAKEFILE_HAVE_CLI_ENV_MMD */
		if (output_filename) {
			output_count = tpp_strlen(output_filename);
			output_temp = tpp_makefile_output_printraw_cstr(self->tmkfcl_mf, output_filename, output_count);
		} else {
			/* Auto-determine target name based on __FILE__ */
			tpp_file const *lc = tpp_lexer_getlcfile(tpp_makefile_getlexer(self->tmkfcl_mf));
			char const *lc_filename = tpp_file_getrealfilename(lc);
			if tpp_unlikely(lc_filename == NULL) {
				output_count = 0;
				output_temp  = 0;
			} else {
				struct tpp_makefile_cli_default_target_data data;
				data.tmfcdtd_count = 0;
				data.tmfcdtd_mf    = self->tmkfcl_mf;
				output_temp = tpp_makefile_cli_print_default_target(self, tpp_formatprinter_of(tpp_makefile_cli_default_target_printer),
				                                                    &data, lc_filename);
				output_count = data.tmfcdtd_count;
			}
		}
	}
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);
	(void)output_count;
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	self->tmkfcl_mf->tmkf_curcol = (tpp_column)output_count;
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */
#if TPP_MAKEFILE_HAVE_CLI_ENV_MD || TPP_MAKEFILE_HAVE_CLI_ENV_MMD
after_print_output_filename:
#endif /* TPP_MAKEFILE_HAVE_CLI_ENV_MD || TPP_MAKEFILE_HAVE_CLI_ENV_MMD */

	/* Print the trailing `:` following the target name */
	output_temp = tpp_makefile_output_printraw_conststr(self->tmkfcl_mf, ":");
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	self->tmkfcl_mf->tmkf_curcol += 1;
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

	/* Emit dependencies for every (distinct) file currently
	 * on the #include-stack (since those won't appear as
	 * "new" dependencies anymore after this point!) */
#if TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK)
	if (!tpp_makefile_cli_loader_getomitmainfile(self)) {
		tpp_lexer *const lexer = tpp_makefile_getlexer(self->tmkfcl_mf);
		tpp_file const *file = tpp_lexer_getfile(lexer);
		do {
#if TPP_HAVE_FILE_GETREALFILENAMEKWD
			tpp_keyword const *kwd = tpp_file_getrealfilenamekwd(file);
			if (kwd)
#else /* TPP_HAVE_FILE_GETREALFILENAMEKWD */
			tpp_keyword const *kwd;
			char const *kwd_str = tpp_file_getrealfilename(file);
			if (kwd_str && tpp_file_getrealfilenameiskwd(file) &&
			    (kwd = tpp_keyword_fromcstr(kwd_str), 1))
#endif /* !TPP_HAVE_FILE_GETREALFILENAMEKWD */
			{
				tpp_keyword *wkwd = tpp_lexer_kwds_copybuiltin(lexer, kwd);
				if tpp_unlikely(!wkwd)
					return TPP_ENOMEM;
#if TPP_HOOK_HASCOOKIE(TPP_HAVE_NEW_DEPENDENCY_HOOK)
				error = _tpp_makefile_new_dependency_hook(self->tmkfcl_mf, wkwd);
#else /* TPP_HOOK_HASCOOKIE(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
				error = _tpp_makefile_new_dependency_hook(lexer, wkwd);
#endif /* !TPP_HOOK_HASCOOKIE(TPP_HAVE_NEW_DEPENDENCY_HOOK) */
				if (TPP_ISERR(error))
					return error;
			}
		} while ((file = tpp_file_getprev(file)) != NULL);
	}
#endif /* TPP_HOOK_ISRT(TPP_HAVE_NEW_DEPENDENCY_HOOK) */

	/* Consume all tokens from the lexer (if enabled) */
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
	if (tpp_makefile_cli_loader_getonlymakefile(self)) {
		tpp_token_id tok;
		tpp_lexer *const lexer = tpp_makefile_getlexer(self->tmkfcl_mf);
		do {
			tok = tpp_lexer_yield(lexer);
		} while (!TPP_TOK_ISERR_OR_EOF(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}
#endif /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */

	return TPP_EOK;
}

#if TPP_MAKEFILE_HAVE_CLI_HELP
#undef TPP_CLI_HELP1
#undef TPP_CLI_HELP2
#define TPP_CLI_HELP1(spelling, description) \
	spelling "\0\0" description "\0"
#if TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS
#define TPP_CLI_HELP2(spelling1, spelling2, description) \
	spelling1 "\0" spelling2 "\0\0" description "\0"
#else /* TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS */
#define TPP_CLI_HELP2(spelling1, spelling2, description) \
	TPP_CLI_HELP1(spelling1, description)
#endif /* !TPP_MAKEFILE_HAVE_CLI_HELP_ALL_SPELLINGS */

/* Returns supported CLI parameters, and human-readable information
 * for them. Same format as `tpp_cli_loader_help` (see for more info) */
TPP_CONST_IMPL char const tpp_makefile_cli_loader_help[] =
#if TPP_MAKEFILE_HAVE_CLI_DASH_M
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
TPP_CLI_HELP2("-M", "--dependencies", "Discard preprocessor output and generate a Makefile")
#else /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
TPP_CLI_HELP2("-M", "--dependencies", "Generate a Makefile")
#endif /* !TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MM
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
TPP_CLI_HELP2("-MM", "--user-dependencies",
              "Discard preprocessor output and generate a Makefile\n"
              "Omit dependencies from #pragma GCC system_header files")
#else /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
TPP_CLI_HELP2("-MM", "--user-dependencies",
              "Generate a Makefile\n"
              "Omit dependencies from #pragma GCC system_header files")
#endif /* !TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MM */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
TPP_CLI_HELP1("-MF FILE", "Write Makefile output to FILE")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
TPP_CLI_HELP1("-MT TARGET", "Use TARGET in Makefile output")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
TPP_CLI_HELP1("-MQ TARGET", "Use TARGET in Makefile output, but escape it first")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MG
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
TPP_CLI_HELP2("-MG", "--print-missing-file-dependencies",
              "Discard preprocessor output and generate a Makefile\n"
              "Also include missing #include-s in the Makefile")
#else /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
TPP_CLI_HELP2("-MG", "--print-missing-file-dependencies",
              "Generate a Makefile\n"
              "Also include missing #include-s in the Makefile")
#endif /* !TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MG */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MP
TPP_CLI_HELP1("-MP", "All dependencies are repeated as phony/dummy targets")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MP */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MD
TPP_CLI_HELP1("-MD", "Generate a Makefile\n"
                     "The output filename is determined automatically")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MD */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MMD
TPP_CLI_HELP1("-MMD", "Generate a Makefile\n"
                      "The output filename is determined automatically\n"
                      "Omit dependencies from #pragma GCC system_header files")
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MMD */
"";

#undef TPP_CLI_HELP1
#undef TPP_CLI_HELP2
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
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 4)) tpp_ssize TPPCALL
tpp_makefile_cli_print_default_target(tpp_makefile_cli_loader *tpp_restrict self,
                                      tpp_formatprinter printer, void *arg,
                                      char const *input_filename) {
	/* Auto-determine target name based on __FILE__ */
	tpp_ssize temp, result = 0;
	char const *iter, *after_last_sep, *before_last_dot;
	(void)self;

	iter = after_last_sep = input_filename;
	for (;;) {
		char ch = *iter++;
		if (!ch)
			break;
		if (TPP_FS_ISSEP(ch))
			after_last_sep = iter;
	}
	before_last_dot = iter - 1;
	tpp_assert(*before_last_dot == '\0');
	iter = after_last_sep;
	for (; *iter; ++iter) {
		if (*iter == '.')
			before_last_dot = iter;
	}

	/* Print leading path prefix */
#if !TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY
	temp = tpp_formatprinter_print_conststr(printer, arg, TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX);
	if (temp < 0)
		goto err_temp;
	result += temp;
#endif /* !TPP_MAKEFILE_DEFAULT_TARGET_PATH_PREFIX_IS_EMPTY */

	/* Print directory-heading, plus everything up to (but excluding)
	 * the final '.' preceding the extension. When configured as such,
	 * also print `TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX`. */
#if !TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY
	temp = tpp_formatprinter_print_cstr(printer, arg, input_filename,
	                                    (tpp_size)(after_last_sep - input_filename));
	if (temp < 0)
		goto err_temp;
	result += temp;
	temp = tpp_formatprinter_print_conststr(printer, arg, TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX);
	if (temp < 0)
		goto err_temp;
	result += temp;
	temp = tpp_formatprinter_print_cstr(printer, arg, after_last_sep,
	                                    (tpp_size)(before_last_dot - after_last_sep));
	if (temp < 0)
		goto err_temp;
	result += temp;
#else /* !TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY */
	temp = tpp_formatprinter_print_cstr(printer, arg, input_filename,
	                                    (tpp_size)(before_last_dot - input_filename));
	if (temp < 0)
		goto err_temp;
	result += temp;
#endif /* !TPP_MAKEFILE_DEFAULT_TARGET_FILENAME_PREFIX_IS_EMPTY */

	/* Print the target's default object-file extension. */
	temp = tpp_formatprinter_print_conststr(printer, arg, TPP_MAKEFILE_DEFAULT_TARGET_EXTENSION);
	if (temp < 0)
		goto err_temp;
	result += temp;
	return result;
err_temp:
	return temp;
}
#endif /* !tpp_makefile_cli_print_default_target */

#endif /* TPP_MAKEFILE_HAVE_CLI */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_CLI_C */
