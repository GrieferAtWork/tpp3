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
#define TPP_BUILDING_OPTIONAL 1
#ifdef USE_AMALGAMATION
#define TPP_PROFILE TPP_PROFILE_ALL /* Enable all features */
#define TPP_COMMON_HAVE_FEATURES 0  /* Use extensions for everything */

#include "tpp-amalgamation.c"
#include "tpp-emitter-amalgamation.c"
#else /* USE_AMALGAMATION */
#include "tpp.h"
#include "tpp-emitter.h"
#endif /* !USE_AMALGAMATION */

#include <stdio.h>

#if TPP_OS_WINDOWS
#include <Windows.h> /* For: `SetConsoleOutputCP(CP_UTF8)` */
#endif /* TPP_OS_WINDOWS */

TPP_DECL_BEGIN

typedef enum tpp_frontend_cli_state {
	TPP_FRONTEND_CLI_STATE_NORMAL,
	TPP_FRONTEND_CLI_STATE_DDASH,  /* After "--" */
	TPP_FRONTEND_CLI_STATE_OUTPUT, /* Expecting argument for "-o" */
} tpp_frontend_cli_state;

/* TPP frontend */
typedef struct tpp_frontend {
	tpp_emitter            tf_emitter;            /* Emitter and (contained within): lexer */
	FILE                  *tf_output_file;        /* [0..1] File for preprocessor output */
	tpp_cli_loader         tf_cli_loader;         /* CLI loader for lexer */
	tpp_emitter_cli_loader tf_emitter_cli_loader; /* CLI loader for emitter */
	tpp_frontend_cli_state tf_cli_state;          /* CLI parsing state */
} tpp_frontend;

static TPP_FORMATPRINTER_DEFINE(tpp_frontend_output_printer, arg, text, num_bytes) {
	tpp_emitter *emitter = (tpp_emitter *)arg;
	tpp_frontend *fe = tpp_container_of(emitter, tpp_frontend, tf_emitter);
	if (fe->tf_output_file == NULL)
		fe->tf_output_file = stdout;
	fwrite(text, 1, num_bytes, fe->tf_output_file);
	return 0;
}

static tpp_errno tpp_frontend_set_output_file(tpp_frontend *self, char const *filename) {
	if (self->tf_output_file != NULL)
		fclose(self->tf_output_file);
	if (strcmp(filename, "-") == 0) {
		self->tf_output_file = NULL; /* Causes `tpp_frontend_output_printer()` to use "stdout" */
		return TPP_EOK;
	}
	self->tf_output_file = fopen(filename, "wb");
	if (self->tf_output_file == NULL) {
		fprintf(stderr, "failed to open output file: %s\n", filename);
		return TPP_EIO;
	}
	return TPP_EOK;
}

#undef TPP_CLI_HELP1
#define TPP_CLI_HELP1(spelling, description) \
	spelling "\0\0" description "\0"
#undef TPP_CLI_HELP2
#define TPP_CLI_HELP2(spelling1, spelling2, description) \
	spelling1 "\0" spelling2 "\0\0" description "\0"

static char const tpp_frontend_help_text[] =
TPP_CLI_HELP1("--help", "Display this help")
TPP_CLI_HELP1("--help SUBJECT", "Display extra help on {cli|extensions|warnings}")
TPP_CLI_HELP1("--version", "Display version information")
TPP_CLI_HELP2("-o FILE", "--output=FILE", "Write output to FILE")
"";

static tpp_size tpp_help_maxspelling(char const *db, bool all_spellings) {
	tpp_size result = 0;
	char const *iter = db;
	while (*iter) {
		tpp_size len = tpp_strlen(iter);
		tpp_size options_len = len;
		iter += len + 1;
		while (*iter) {
			len = tpp_strlen(iter) + 1;
			if (all_spellings)
				options_len += len;
			iter += len;
		}
		if (result < options_len)
			result = options_len;
		++iter;
		iter += tpp_strlen(iter) + 1;
	}
	return result;
}

static void print_description(char const *desc, tpp_size indent) {
	for (;;) {
		char const *end = strchr(desc, '\n');
		if (!end) {
			puts(desc);
			break;
		}
		++end;
		fwrite(desc, sizeof(char), (tpp_size)(end - desc), stdout);
		printf("%*s", (int)(unsigned int)indent, "");
		desc = end;
	}
}

static void tpp_frontend_help_cli_for(char const *db, tpp_size padlen, bool all_spellings) {
	char const *iter = db;
	while (*iter) {
		/* Print spellings */
		tpp_size len = tpp_strlen(iter);
		tpp_size options_len = len;
		printf("%s", iter);
		iter += len + 1;
		while (*iter) {
			len = tpp_strlen(iter) + 1;
			if (all_spellings) {
				printf(" %s", iter);
				options_len += len;
			}
			iter += len;
		}
		++iter;
		/* Print description */
		printf(" %*s", (int)(unsigned int)(padlen - options_len), "");
		print_description(iter, padlen + 1);
		iter += tpp_strlen(iter) + 1;
	}
}

static void tpp_frontend_help_cli(bool all_spellings) {
	tpp_size padlen, temp;
	padlen = tpp_help_maxspelling(tpp_cli_loader_help, all_spellings);
	temp = tpp_help_maxspelling(tpp_emitter_cli_loader_help, all_spellings);
	if (padlen < temp)
		padlen = temp;
	temp = tpp_help_maxspelling(tpp_frontend_help_text, all_spellings);
	if (padlen < temp)
		padlen = temp;
	tpp_frontend_help_cli_for(tpp_frontend_help_text, padlen, all_spellings);
	tpp_frontend_help_cli_for(tpp_cli_loader_help, padlen, all_spellings);
	tpp_frontend_help_cli_for(tpp_emitter_cli_loader_help, padlen, all_spellings);
}

static void tpp_frontend_help_extensions(void) {
	tpp_extension_id id;
	for (id = (tpp_extension_id)0; id < TPP_EXT_COUNT;
	     id = (tpp_extension_id)((unsigned int)id + 1)) {
		char const *name = tpp_extension_getname(id);
		if (name)
			printf("-f[no-]%s\n", name);
	}
}

static void tpp_frontend_help_warnings(void) {
	tpp_warning_group_id id;
	for (id = (tpp_warning_group_id)0; id < TPP_WG_COUNT;
	     id = (tpp_warning_group_id)((unsigned int)id + 1)) {
		char const *names = tpp_warning_group_getnames(id);
		if (names) {
			printf("-W[no-]%s", names);
			names += tpp_strlen(names) + 1;
			while (*names) {
				printf(" -W[no-]%s", names);
				names += tpp_strlen(names) + 1;
			}
			putchar('\n');
		}
	}
}

static void tpp_frontend_help(char const *appname, int argc, char **argv) {
	if (!argc) {
		printf("Usage: %s [ARGS...] [--] [INFILES]\nARGS:\n", appname);
		tpp_frontend_help_cli(false);
		return;
	}
	do {
		char *what = *argv++;
		if (tpp_strcmp(what, "cli") == 0) {
			tpp_frontend_help_cli(true);
		} else if (tpp_strcmp(what, "extensions") == 0) {
			tpp_frontend_help_extensions();
		} else if (tpp_strcmp(what, "warnings") == 0) {
			tpp_frontend_help_warnings();
		} else {
			printf("Unknown help subject '%s'\n", what);
		}
	} while (--argc);
}

static void tpp_frontend_version(void) {
	printf("TPP version %d/%d\n"
	       "Copyright (c) 2017-2026 Griefer@Work\n"
	       "If you use this software in a product, an "
	       /**/ "acknowledgement in the product "
	       /**/ "documentation is required (see LICENSE)\n",
	       TPP_API_VERSION, TPP_PREPROCESSOR_VERSION);
}

static tpp_errno tpp_frontend_parsearg(tpp_frontend *self, char const *arg) {
#define tpp_streq(at, CONSTstr) \
	(tpp_memcmp(at, CONSTstr, sizeof(CONSTstr) - sizeof(char)) == 0)
	switch (self->tf_cli_state) {

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
	 *   No special handling needed in TPP backend (see "-MM")
	 *
	 * TODO: The `-M*` flags above should really (somehow) be handled by the TPP core!
	 *       Being able to generate Makefile-compatible dependency definitions really
	 *       is something that pretty much any compiler that's using the C preprocessor
	 *       should be capable of doing. Therefor, the functionality to do so should be
	 *       part of the TPP core, too!
	 *
	 * - "-fsearch-include-path[=kind]"  (kind=R"(user|system)", default="user")
	 *   No special handling needed in TPP backend
	 *   When kind=user, and the main input file could not be found, it must be
	 *   searched-for using `tpp_lexer_foreach_include_path(TPP_TOK_INCPATH_DQUOTE)`
	 *   When kind=system, and the main input file could not be found, it must be
	 *   searched-for using `tpp_lexer_foreach_include_path(TPP_TOK_INCPATH_LANGLE)`
	 */

	/* TODO: Support TPP2-style CLI aliases:
	 * - -f[no-]spc:            configure 'TPP_HAVE_TOK_SPACE'
	 * - -f[no-]lf:             configure 'TPP_HAVE_TOK_LF'
	 * - -f[no-]comments:       configure 'TPP_HAVE_TOK_COMMENT'
	 * - -f[no-]longstring:     turn on 'TPP_HAVE_STRING_ALLOW_MULTILINE' + disable '-Wno-multiline-string'
	 * - -f[no-]unify-pragma:   alias for '-f[no-]reemit-unknown-pragma'
	 * - -fline:                turn off 'TPP_EMITTER_HAVE_NOLINE', turn off 'TPP_EMITTER_HAVE_USE_CPP_DIGIT'
	 * - -fno-line:             turn on 'TPP_EMITTER_HAVE_NOLINE'
	 * - -fcpp-line:            turn off 'TPP_EMITTER_HAVE_NOLINE', turn on 'TPP_EMITTER_HAVE_USE_CPP_DIGIT'
	 * - --message-format=gcc:  tpp_lexer_setfileandlineformat("%Pf:%Pl:%Pc: ")
	 * - --message-format=msvc: tpp_lexer_setfileandlineformat("%Pf(%Pl, %Pc): ")
	 * - -i ...:                Construct TEXT-file from remainder of CLI, which is then used as input (use 'TPP_CONFIG_CLI_FILENAME' as filename)
	 * - --tok:                 tpp_emitter_setmode(TPP_EMITTER_MODE_BRACKET)
	 * - --pp:                  tpp_emitter_setmode(TODO), turn off SPACE/LF
	 */

	case TPP_FRONTEND_CLI_STATE_NORMAL:
		switch (*arg++) {
		case '-': {
			char const *after_dash = arg;
			switch (*arg++) {
			case '\0':
				return TPP_ENOENT;
			case 'o':
				if (*arg == '\0') {
					self->tf_cli_state = TPP_FRONTEND_CLI_STATE_OUTPUT;
					return TPP_EOK;
				}
				break;

			case '-':
				switch (*arg++) {
				case '\0':
					self->tf_cli_state = TPP_FRONTEND_CLI_STATE_DDASH; /* -- */
					return TPP_EOK;

				case 'o':
					if (tpp_streq(arg, "utput=")) { /* --output=... */
						arg += (sizeof("utput=") - sizeof(char));
						return tpp_frontend_set_output_file(self, arg);
					}
					break;

				default: break;
				}
				break;

			default: break;
			}

			/* Try to consult "flag" parsers. */
			while (*after_dash) {
				tpp_errno error = tpp_cli_loader_parseflag(&self->tf_cli_loader, &after_dash);
				if (error == TPP_ENOENT)
					error = tpp_emitter_cli_loader_parseflag(&self->tf_emitter_cli_loader, &after_dash);
				if (TPP_ISERR(error))
					return error;
			}
		}	break;

		default: break;
		}
		break;

	case TPP_FRONTEND_CLI_STATE_DDASH:
		break; /* Don't accept any more arguments */

	case TPP_FRONTEND_CLI_STATE_OUTPUT:
		self->tf_cli_state = TPP_FRONTEND_CLI_STATE_NORMAL;
		return tpp_frontend_set_output_file(self, arg);

	default: tpp_unreachable();
	}
	return TPP_ENOENT;
#undef tpp_streq
}

static tpp_errno tpp_frontend_parseargv(tpp_frontend *self, int *p_argc, char ***p_argv) {
	tpp_errno result = TPP_EOK;
	int argc    = *p_argc;
	char **argv = *p_argv;
	unsigned int unknown_count = 0;
	while (argc > 0) {
		char *arg = argv[0];
		result = tpp_frontend_parsearg(self, arg);
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
		if (self->tf_cli_state == TPP_FRONTEND_CLI_STATE_DDASH) {
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

#undef tpp_get_stdin
#ifdef tpp_io_handle_IS_HANDLE
#define tpp_get_stdin() GetStdHandle(STD_INPUT_HANDLE)
#endif /* tpp_io_handle_IS_HANDLE */

#ifdef tpp_io_handle_IS_int
#ifdef STDIN_FILENO
#define tpp_get_stdin() STDIN_FILENO
#else /* STDIN_FILENO */
#define tpp_get_stdin() 0
#endif /* !STDIN_FILENO */
#endif /* tpp_io_handle_IS_int */

#ifdef tpp_io_handle_IS_FILE
#define tpp_get_stdin() stdin
#endif /* tpp_io_handle_IS_FILE */

int main(int argc, char **argv) {
	tpp_errno error;
	int result = 1;
	tpp_frontend fe;
	bool first_file;
	char *appname = argv[0];
	char *fallback_input_filename;
	if (argc)
		--argc, ++argv; /* Skip "appname" argument */

#if TPP_OS_WINDOWS
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif /* TPP_OS_WINDOWS */

	/* Initialize frontend */
	tpp_emitter_init(&fe.tf_emitter, &tpp_frontend_output_printer);
	tpp_cli_loader_init(&fe.tf_cli_loader, tpp_emitter_getlexer(&fe.tf_emitter));
	tpp_emitter_cli_loader_init(&fe.tf_emitter_cli_loader, &fe.tf_emitter);
	fe.tf_output_file = NULL;
	fe.tf_cli_state = TPP_FRONTEND_CLI_STATE_NORMAL;

	/* Parse arguments... */
	error = tpp_cli_loader_parseargv(&fe.tf_cli_loader, &argc, &argv);
	if (!TPP_ISERR(error))
		error = tpp_emitter_cli_loader_parseargv(&fe.tf_emitter_cli_loader, &argc, &argv);
	if (!TPP_ISERR(error))
		error = tpp_frontend_parseargv(&fe, &argc, &argv);
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to parse arguments: %s\n", tpp_strerror(error));
		goto out_emitter_loader;
	}

	/* Parse remainder of argument list using our own CLI handler, as well */
	if (argc && tpp_strcmp(*argv, "--") == 0) {
		--argc;
		++argv;
	} else if (argc && (*argv)[0] == '-' && (*argv)[1])  {
		if (tpp_strcmp(*argv, "--help") == 0) {
			--argc;
			++argv;
			tpp_frontend_help(appname, argc, argv);
			result = 0;
			goto out_emitter_loader;
		} else if (tpp_strcmp(*argv, "--version") == 0) {
			tpp_frontend_version();
			result = 0;
			goto out_emitter_loader;
		}
		fprintf(stderr, "unknown argument %s\n"
		                "Usage: %s [ARGS...] [--] [INFILES]\n"
		                "See --help for more info\n",
		        *argv, appname);
		goto out_emitter_loader;
	}
	if (argc == 0) {
		argc = 1;
		argv = &fallback_input_filename;
		fallback_input_filename = (char *)"-";
	}

	first_file = true;
	do {
		char *filename;
		--argc;
		filename = argv[argc];
#ifdef tpp_get_stdin
		if (tpp_strcmp(filename, "-") == 0) {
			filename = (char *)"<stdin>";
			if (first_file) {
				tpp_lexer_initfile_io_ex(tpp_emitter_getlexer(&fe.tf_emitter),
				                         filename, tpp_get_stdin(),
				                         TPP_FILE_FLAGS_NOCLOSE);
				error = TPP_EOK;
			} else {
				error = tpp_lexer_pushfile_io_ex(tpp_emitter_getlexer(&fe.tf_emitter),
				                                 filename, tpp_get_stdin(),
				                                 TPP_FILE_FLAGS_NOCLOSE);
			}
		} else
#endif /* tpp_get_stdin */
		{
			if (first_file) {
				error = tpp_lexer_initfile_open(tpp_emitter_getlexer(&fe.tf_emitter),
				                                filename, TPP_SIZE_MAX);
			} else {
				error = tpp_lexer_pushfile_open(tpp_emitter_getlexer(&fe.tf_emitter),
				                                filename, TPP_SIZE_MAX);
			}
		}
		if (TPP_ISERR(error)) {
			fprintf(stderr, "failed to open '%s': %s\n", filename, tpp_strerror(error));
			if (first_file)
				goto out_emitter_loader;
			goto out_emitter_loader_file;
		}
		first_file = false;
	} while (argc);

	/* Flush CLI loaders */
	error = tpp_emitter_cli_loader_flush(&fe.tf_emitter_cli_loader);
	if (!TPP_ISERR(error))
		error = tpp_cli_loader_flush(&fe.tf_cli_loader);

	/* Finalize CLI loaders */
	tpp_cli_loader_fini(&fe.tf_cli_loader);
	tpp_emitter_cli_loader_fini(&fe.tf_emitter_cli_loader);

	/* Final check for errors. */
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to complete arguments: %s\n", tpp_strerror(error));
		goto out_emitter_file;
	}

	/* Yield & re-emit tokens. */
	for (;;) {
		tpp_token_id const tok = tpp_lexer_yield(tpp_emitter_getlexer(&fe.tf_emitter));
		if (TPP_TOK_ISERR(tok)) {
			fprintf(stderr, "yield failed: %s\n", tpp_strerror(TPP_TOK_ASERR(tok)));
			goto out_emitter_file;
		}
		if (tok == TPP_TOK_EOF)
			break;

		(void)tpp_emitter_emitcurrent(&fe.tf_emitter);
	}

	/* Return "0" if there were no TPP_WSTATE_ERROR-level messages */
	if (tpp_lexer_geterrorcount(tpp_emitter_getlexer(&fe.tf_emitter)) == 0)
		result = 0;

out_emitter_file:
	tpp_lexer_finifile(tpp_emitter_getlexer(&fe.tf_emitter));
out_emitter:
	tpp_emitter_fini(&fe.tf_emitter);
#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif /* _MSC_VER */
	return result;
out_emitter_loader_file:
	tpp_lexer_finifile(tpp_emitter_getlexer(&fe.tf_emitter));
out_emitter_loader:
	tpp_cli_loader_fini(&fe.tf_cli_loader);
	tpp_emitter_cli_loader_fini(&fe.tf_emitter_cli_loader);
	goto out_emitter;
}

TPP_DECL_END
