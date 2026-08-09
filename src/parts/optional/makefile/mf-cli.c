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

/*[[[deemon
for (local option, what: {
//	{"TOK_SPACE", "enable"},
}) {
	local extraArgs = what == "set" ? ", v" : "";
	print("#if TPP_CONF_ISEXT(TPP_HAVE_", option, ")");
	print("#define tpp_lexer_", what, "_", option, "(self", extraArgs, ") tpp_lexer_", what, "extension(self, TPP_EXT_", option, extraArgs, ")");
	print("#elif TPP_CONF_ISFEAT(TPP_HAVE_", option, ")");
	print("#define tpp_lexer_", what, "_", option, "(self", extraArgs, ") (tpp_lexer_", what, "feature(self, TPP_FEAT_", option, extraArgs, "), TPP_EOK)");
	print("#else /" "* ... *" "/");
	print("#define tpp_lexer_", what, "_", option, "(self", extraArgs, ") TPP_EOK");
	print("#endif /" "* !... *" "/");
}
]]]*/

/*[[[end]]]*/


/*[[[deemon
for (local option, what: {
//	{"NORMALIZE_SPACE", "set"},
}) {
	local extraArgs = what == "set" ? ", v" : "";
	print("#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_", option, ")");
	print("#define tpp_makefile_", what, "_", option, "(self", extraArgs, ") tpp_makefile_", what, "feature(self, TPP_MAKEFILE_FEAT_", option, extraArgs, ")");
	print("#else /" "* ... *" "/");
	print("#define tpp_makefile_", what, "_", option, "(self", extraArgs, ") (void)0");
	print("#endif /" "* !... *" "/");
}
]]]*/

/*[[[end]]]*/


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
#if TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_cli_loader_enable_with_makefile_only(tpp_makefile_cli_loader *tpp_restrict self) {
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
	tpp_makefile_cli_loader_enableonlymakefile(self);
#endif /* TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE */
	tpp_makefile_cli_loader_enablemakefile(self);
	return TPP_EOK;
}
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M || TPP_MAKEFILE_HAVE_CLI_DASH_MM */

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
	switch (self->tmfcl_state) {

	case TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL: {
		if (*arg++ != '-')
			break; /* All arguments must start with at least 1 "-" */
		switch (*arg++) {

		case '-':
			/* --foo argument */
			switch (*arg++) {

			case '\0':
				self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_DDASH; /* -- */
				return TPP_EOK;

			case 'd':
#if TPP_MAKEFILE_HAVE_CLI_DASH_M
				if (tpp_streq(arg, "ependencies\0")) { /* --dependencies */
					return tpp_makefile_cli_loader_enable_with_makefile_only(self);
				} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_M */
				{
				}
				break;

			case 'u':
#if TPP_MAKEFILE_HAVE_CLI_DASH_MM
				if (tpp_streq(arg, "ser-dependencies\0")) { /* --user-dependencies */
					tpp_makefile_enablefeature(self->tmfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
					return tpp_makefile_cli_loader_enable_with_makefile_only(self);
				} else
#endif /* !TPP_MAKEFILE_HAVE_CLI_DASH_MM */
				{
				}
				break;

			default: break;
			}
			break;

		/* TODO: Support for CLI arguments that must be handled by front-end:
		 * - "-MG", "--print-missing-file-dependencies"
		 *   - Use `TPP_HAVE_INCLUDE_NOT_FOUND_HOOK` (with a `TPP_EOK` return value)
		 *     to suppress `TPP_W_NO_SUCH_FILE` warnings, whilst at the same time
		 *     using `tpp_lexer_decode_include_string_cb()` to add the missing include's
		 *     filename to the set of dependencies */


		case 'M':
#if TPP_MAKEFILE_HAVE_CLI_DASH_MMD
			if (tpp_streq(arg, "MD\0")) {
				self->tmfcl_flags |= _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT;
				tpp_makefile_enablefeature(self->tmfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
				tpp_makefile_cli_loader_enablemakefile(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MMD */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MD
			if (tpp_streq(arg, "D\0")) {
				self->tmfcl_flags |= _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT;
				tpp_makefile_cli_loader_enablemakefile(self);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MD */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
			if (*arg == 'F') {
				if (*++arg) {
					self->tmfcl_outfile = arg; /* -MFfoo.m */
				} else {
					self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_MF; /* -MF foo.m */
				}
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MP
			if (tpp_streq(arg, "P\0")) {
				tpp_makefile_enablefeature(self->tmfcl_mf, TPP_MAKEFILE_FEAT_PHONY);
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MP */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
			if (*arg == 'T') {
				if (*++arg) {
					tpp_makefile_cli_loader_settarget_mt(self, arg); /* -MTmain.o */
				} else {
					self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_MT; /* -MT main.o */
				}
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
			if (*arg == 'Q') {
				if (*++arg) {
					tpp_makefile_cli_loader_settarget_mq(self, arg); /* -MQmain.o */
				} else {
					self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_MQ; /* -MQ main.o */
				}
				return TPP_EOK;
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MM
			if (tpp_streq(arg, "M\0")) {
				tpp_makefile_enablefeature(self->tmfcl_mf, TPP_MAKEFILE_FEAT_USER_DEPENDENCIES);
				return tpp_makefile_cli_loader_enable_with_makefile_only(self);
			} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MM */
#if TPP_MAKEFILE_HAVE_CLI_DASH_M
			if (tpp_streq(arg, "\0")) {
				return tpp_makefile_cli_loader_enable_with_makefile_only(self);
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
		self->tmfcl_state   = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL;
		self->tmfcl_outfile = arg;
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */

#if TPP_MAKEFILE_HAVE_CLI_DASH_MT
	case TPP_MAKEFILE_CLI_LOADER_STATE_MT:
		self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL;
		tpp_makefile_cli_loader_settarget_mt(self, arg);
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT */

#if TPP_MAKEFILE_HAVE_CLI_DASH_MQ
	case TPP_MAKEFILE_CLI_LOADER_STATE_MQ:
		self->tmfcl_state = TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL;
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
			tpp_memmovedown(&argv[0], &argv[1],
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
					tpp_memmovedown(&argv[0], &argv[1], total_count_minus_1 * sizeof(char *));
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
 *                                 that target is derived from `__BASE_FILE__`)
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
	if (self->tmfcl_state != TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL &&
	    self->tmfcl_state != TPP_MAKEFILE_CLI_LOADER_STATE_DDASH) {
		error = tpp_makefile_cli_warnf(self->tmfcl_mf, NULL, 0,
		                               TPP_W_MISSING_CLI_ARGUMENT);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT */

	/* Check if Makefile generation is enabled (if it isn't don't do anything else) */
#if TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED
	if (!tpp_makefile_cli_loader_getmakefileenabled(self))
		return TPP_EOK;
#endif /* TPP_MAKEFILE_HAVE_CLI_LOADER_FLAG_ENABLED */

	/* Turn on the linked makefile */
	error = tpp_makefile_enable(self->tmfcl_mf);
	if (TPP_ISERR(error))
		return error;

	/* Redirect output to a custom file */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF
	if (self->tmfcl_outfile) {
#if TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH
		if (tpp_strcmp(self->tmfcl_outfile, "-") == 0) {
#ifdef tpp_makefile_io_getstdout
			tpp_makefile_io_handle handle = tpp_makefile_io_getstdout();
			tpp_makefile_setoutput_io_ex(self->tmfcl_mf, handle, true);
#else /* tpp_makefile_io_getstdout */
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: 'TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH' is enabled, but no way to retrieve STDOUT handle"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* !tpp_makefile_io_getstdout */
		} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF_DASH */
		{
			error = tpp_makefile_setoutput_file(self->tmfcl_mf,
			                                    self->tmfcl_outfile);
			if (TPP_ISERR(error))
				return error;
		}
	} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MF */
	{
#if TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD
		if (self->tmfcl_flags & _TPP_MAKEFILE_CLI_LOADER_FLAG_AUTOOUTPUT) {
			char const *filename = output_filename;
			char const *filename_end;
			tpp_size filename_len;
			char *output_buf, *ptr;
			/* Auto-determine output filename when -MD or -MMD was used,
			 * based on `output_filename`, or if not given: __BASE_FILE__ */
			if (filename == NULL) {
				tpp_file const *bf = tpp_lexer_getbasefile(tpp_makefile_getlexer(self->tmfcl_mf));
				filename = tpp_file_getfilename(bf);
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
			tpp_memcpy(ptr, TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION,
			           sizeof(TPP_MAKEFILE_CONFIG_DEFAULT_EXTENSION));
			/* Use this one as filename */
			error = tpp_makefile_setoutput_file(self->tmfcl_mf, output_buf);
			tpp_free(output_buf);
			if (TPP_ISERR(error))
				return error;
		}
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MD || TPP_MAKEFILE_HAVE_CLI_DASH_MMD */
	}

	/* Print makefile target */
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ
	if (self->tmfcl_target) {
#if TPP_MAKEFILE_HAVE_CLI_DASH_MT && TPP_MAKEFILE_HAVE_CLI_DASH_MQ
		if (self->tmfcl_flags & _TPP_MAKEFILE_CLI_LOADER_FLAG_TARGETESCAPE) {
			output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self->tmfcl_mf), self->tmfcl_mf,
			                                  (tpp_char const *)self->tmfcl_target,
			                                  tpp_strlen(self->tmfcl_target),
			                                  &output_count);
		} else {
			output_count = tpp_strlen(self->tmfcl_target);
			output_temp = tpp_makefile_output_printraw_cstr(self->tmfcl_mf, self->tmfcl_target, output_count);
		}
#elif TPP_MAKEFILE_HAVE_CLI_DASH_MT
		output_count = tpp_strlen(self->tmfcl_target);
		output_temp = tpp_makefile_output_printraw_cstr(self->tmfcl_mf, self->tmfcl_target, output_count);
#else /* ... */
		output_temp = tpp_makefile_escape(tpp_makefile_getoutput(self->tmfcl_mf), self->tmfcl_mf,
		                                  (tpp_char const *)self->tmfcl_target,
		                                  tpp_strlen(self->tmfcl_target), &output_count);
#endif /* !... */
	} else
#endif /* TPP_MAKEFILE_HAVE_CLI_DASH_MT || TPP_MAKEFILE_HAVE_CLI_DASH_MQ */
	{
		/* TODO: Auto-determine target name based on __BASE_FILE__:
		 * For C:      `__BASE_FILE__.rpartition(".").first + ".o"`
		 * For deemon: `__BASE_FILE__.rpartition("/").first + "." + __BASE_FILE__.rpartition("/").last.rsstrip(".dee") + ".dec"`
		 * ...
		 * What's done here must be completely overwritable by the user,
		 * with the default implementation simply doing what's right for C.
		 */
		output_count = 4;
		output_temp = tpp_makefile_output_printraw_conststr(self->tmfcl_mf, "TODO");
	}
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);
	(void)output_count;
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	self->tmfcl_mf->tmf_curcol = (tpp_column)output_count;
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

	/* Print the trailing `:` following the target name */
	output_temp = tpp_makefile_output_printraw_conststr(self->tmfcl_mf, ":");
	if (output_temp < 0)
		return TPP_SSIZE_ASERR(output_temp);
#if TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH
	self->tmfcl_mf->tmf_curcol += 1;
#endif /* TPP_MAKEFILE_CONFIG_MAX_LINE_LENGTH */

	/* Consume all tokens from the lexer (if enabled) */
#if TPP_MAKEFILE_HAVE_CLI_ONLYMAKEFILE
	if (tpp_makefile_cli_loader_getonlymakefile(self)) {
		tpp_token_id tok;
		tpp_lexer *const lexer = tpp_makefile_getlexer(self->tmfcl_mf);
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

#endif /* TPP_MAKEFILE_HAVE_CLI */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_CLI_C */
