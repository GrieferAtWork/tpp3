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
};

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

			default: break;
			}
			break;

		/* TODO: Support for CLI arguments that must be handled by front-end:
		 * - "-M", "--dependencies"
		 *   - Using TPP_HAVE_NEW_DEPENDENCY_HOOK
		 * - "-MM", "--user-dependencies"
		 *   - Don't emit if #include-stack contains a `tpp_file_getsystemheader()`-file
		 * - "-MF file"
		 *   No special handling needed in TPP backend
		 * - "-MG", "--print-missing-file-dependencies"
		 *   - Use `TPP_HAVE_INCLUDE_NOT_FOUND_HOOK` (with a `TPP_EOK` return value)
		 *     to suppress `TPP_W_NO_SUCH_FILE` warnings, whilst at the same time
		 *     using `tpp_lexer_decode_include_string_cb()` to add the missing include's
		 *     filename to the set of dependencies
		 * - "-MP"
		 *   No special handling needed in TPP backend
		 * - "-MT target"
		 *   No special handling needed in TPP backend
		 * - "-MQ target"
		 *   No special handling needed in TPP backend
		 * - "-MD", "--write-dependencies"
		 *   No special handling needed in TPP backend
		 * - "-MMD", "--write-user-dependencies"
		 *   No special handling needed in TPP backend (see "-MM") */

		default: break;
		}
	}	break;

	case TPP_MAKEFILE_CLI_LOADER_STATE_DDASH:
		break; /* Don't accept any more arguments after having encountered a "--" arguments */

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
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_makefile_cli_loader_flush(tpp_makefile_cli_loader *tpp_restrict self) {
	(void)self;

	/* Emit a warning if the CLI loader isn't in a neutral state */
#if TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT
	if (self->tmfcl_state != TPP_MAKEFILE_CLI_LOADER_STATE_NORMAL &&
	    self->tmfcl_state != TPP_MAKEFILE_CLI_LOADER_STATE_DDASH) {
		tpp_errno error = tpp_makefile_cli_warnf(self->tmfcl_mf, NULL, 0,
		                                         TPP_W_MISSING_CLI_ARGUMENT);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT */

	/* TODO: If "-M" was used (meaning that *only* a makefile should be
	 *       created, without any regular token emission having to happen),
	 *       then we have to yield all input *right here*, since if we
	 *       didn't do so now, output would also go through a potential
	 *       emitter (or the target compiler) -- For the later case,
	 *       there also needs to be a flag to tell the caller that input
	 *       was already used up and actually *shouldn't* be passed on
	 *       to the target compiler. */

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
/* TODO */
"";
#undef TPP_CLI_HELP1
#undef TPP_CLI_HELP2
#endif /* TPP_MAKEFILE_HAVE_CLI_HELP */

#endif /* TPP_MAKEFILE_HAVE_CLI */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_CLI_C */
