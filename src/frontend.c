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
#define TPP_PROFILE TPP_PROFILE_ALL /* Enable everything */
#define TPP_COMMON_HAVE_FEATURES 0  /* Use extensions instead of features (so everything is configurable via `-f[no-]extension`) */
#define TPP_USE_STATIC 1            /* Define everything with `static` linkage */

/* Disable some stuff turned on by `TPP_PROFILE_ALL` but not actually used */
#define TPP_HAVE_LEXER_COPY             0
#define TPP_HAVE_KEYWORDS_UNDEFALLUSER  0
#define TPP_HAVE_KEYWORDS_UNASSERTALL   0
#define TPP_HAVE_KEYWORDS_RESETFLAGS    0
#define TPP_HAVE_KEYWORDS_RESETFEATURES 0
#define TPP_HAVE_KEYWORDS_RESETCOUNTERS 0
#define TPP_HAVE_LEXER_PUSHFILE_TEXT    0

/* Pull in TPP sources */
#include "tpp-amalgamation.c"          /* CORE */
#include "tpp-emitter-amalgamation.c"  /* Source extension: EMITTER */
#include "tpp-makefile-amalgamation.c" /* Source extension: MAKEFILE */
#else /* USE_AMALGAMATION */
#include "tpp.h"
#include "tpp-emitter.h"
#include "tpp-makefile.h"
#endif /* !USE_AMALGAMATION */

#include <stdio.h>

#if TPP_OS_WINDOWS
#include <Windows.h> /* For: `SetConsoleOutputCP(CP_UTF8)` */
#ifdef _MSC_VER
#include <fcntl.h>
#include <io.h>
#endif /* _MSC_VER */
#endif /* TPP_OS_WINDOWS */

/* Enable TPP2-compatibility CLI options */
#ifndef TPP_FRONTEND_HAVE_TPP2_COMPAT
#define TPP_FRONTEND_HAVE_TPP2_COMPAT TPP_HAVE_PROFILE_ALL
#endif /* !TPP_FRONTEND_HAVE_TPP2_COMPAT */

TPP_DECL_BEGIN

typedef enum tpp_frontend_cli_state {
	TPP_FRONTEND_CLI_STATE_NORMAL,
	TPP_FRONTEND_CLI_STATE_DDASH,  /* After "--" */
	TPP_FRONTEND_CLI_STATE_OUTPUT, /* Expecting argument for "-o" */
} tpp_frontend_cli_state;

/* TPP frontend */
typedef struct tpp_frontend {
	tpp_lexer               tf_lexer;               /* Lexer */
	tpp_emitter             tf_emitter;             /* Emitter */
	tpp_makefile            tf_makefile;            /* Makefile */
	FILE                   *tf_output_file;         /* [0..1] File for preprocessor output */
	char const             *tf_output_filename;     /* [0..1] Filename for preprocessor output */
	tpp_cli_loader          tf_cli_loader;          /* CLI loader for lexer */
	tpp_emitter_cli_loader  tf_emitter_cli_loader;  /* CLI loader for emitter */
	tpp_makefile_cli_loader tf_makefile_cli_loader; /* CLI loader for makefile */
	tpp_frontend_cli_state  tf_cli_state;           /* CLI parsing state */
} tpp_frontend;

static tpp_ssize TPPCALL
tpp_frontend_output_printer(tpp_frontend *fe, tpp_char const *text, tpp_size num_bytes) {
	if (fe->tf_output_file == NULL)
		fe->tf_output_file = stdout;
	fwrite(text, 1, num_bytes, fe->tf_output_file);
	return 0;
}

static TPP_FORMATPRINTER_DEFINE(tpp_frontend_emitter_output_printer, arg, text, num_bytes) {
	tpp_emitter *emitter = (tpp_emitter *)arg;
	tpp_frontend *fe = tpp_container_of(emitter, tpp_frontend, tf_emitter);
	return tpp_frontend_output_printer(fe, text, num_bytes);
}

static TPP_FORMATPRINTER_DEFINE(tpp_frontend_makefile_output_printer, arg, text, num_bytes) {
	tpp_makefile *makefile = (tpp_makefile *)arg;
	tpp_frontend *fe = tpp_container_of(makefile, tpp_frontend, tf_makefile);
	return tpp_frontend_output_printer(fe, text, num_bytes);
}

static tpp_errno tpp_frontend_set_output_file(tpp_frontend *self, char const *filename) {
	if (self->tf_output_file != NULL)
		fclose(self->tf_output_file);
	if (strcmp(filename, "-") == 0) {
		self->tf_output_file = NULL; /* Causes `tpp_frontend_output_printer()` to use "stdout" */
		self->tf_output_filename = NULL;
		return TPP_EOK;
	}
	self->tf_output_file = fopen(filename, "wb");
	if (self->tf_output_file == NULL) {
		fprintf(stderr, "failed to open output file: %s\n", filename);
		return TPP_EIO;
	}
	self->tf_output_filename = filename;
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
#if TPP_FRONTEND_HAVE_TPP2_COMPAT
TPP_CLI_HELP1("-f[no]spc", "Alias for -f[no-]tok-space")
TPP_CLI_HELP1("-f[no]lf", "Alias for -f[no-]tok-lf")
TPP_CLI_HELP1("-f[no]comments", "Alias for -f[no-]tok-comment")
TPP_CLI_HELP1("-f[no]longstring", "Alias for -fstring-allow-multiline, -Wno-multiline-string")
TPP_CLI_HELP1("-f[no]unify-pragma", "Alias for -f[no-]reemit-unknown-pragma")
TPP_CLI_HELP1("-fline", "Alias for -fno-use-cpp-digit")
TPP_CLI_HELP1("-fno-line", "Alias for -P")
TPP_CLI_HELP1("-fcpp-line", "Alias for -fuse-cpp-digit")
TPP_CLI_HELP1("--message-format=GCC|MSVC", "Set file-and-line format in messages")
TPP_CLI_HELP1("--tok", "Alias for --mode=bracket")
TPP_CLI_HELP1("--pp", "Alias for --mode=zero")
#endif /* TPP_FRONTEND_HAVE_TPP2_COMPAT */
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
	temp = tpp_help_maxspelling(tpp_makefile_cli_loader_help, all_spellings);
	if (padlen < temp)
		padlen = temp;
	temp = tpp_help_maxspelling(tpp_frontend_help_text, all_spellings);
	if (padlen < temp)
		padlen = temp;
	tpp_frontend_help_cli_for(tpp_frontend_help_text, padlen, all_spellings);
	tpp_frontend_help_cli_for(tpp_cli_loader_help, padlen, all_spellings);
	tpp_frontend_help_cli_for(tpp_emitter_cli_loader_help, padlen, all_spellings);
	tpp_frontend_help_cli_for(tpp_makefile_cli_loader_help, padlen, all_spellings);
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

	/* TODO: - "-fsearch-include-path[=kind]"  (kind=R"(user|system)")
	 *       - "-fsearch-include-path"         (same as "-fsearch-include-path=user")
	 * - No special handling needed in TPP backend
	 *   When kind=user, and the main input file could not be found, it must be
	 *   searched-for using `tpp_lexer_foreach_include_path(TPP_TOK_INCPATH_DQUOTE)`
	 *   When kind=system, and the main input file could not be found, it must be
	 *   searched-for using `tpp_lexer_foreach_include_path(TPP_TOK_INCPATH_LANGLE)`
	 *
	 * TODO: For the purpose of pushing the initial (set of) input files, rather than
	 *       do so here, `tpp_cli_loader` should have an API that off-loads that task
	 *       onto it, thus simplifying what needs to be done by the frontend. */

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

#if TPP_FRONTEND_HAVE_TPP2_COMPAT
				case 'm':
					if (tpp_streq(arg, "essage-format=")) { /* --message-format=... */
						arg += (sizeof("essage-format=") - sizeof(char));
						if (tpp_streq(arg, "gcc\0")) {
							tpp_lexer_setfileandlineformat(&self->tf_lexer,
							                               "%Pf:%Pl:%Pc: ");
							return TPP_EOK;
						} else if (tpp_streq(arg, "msvc\0")) {
							tpp_lexer_setfileandlineformat(&self->tf_lexer,
							                               "%Pf(%Pl, %Pc): ");
							return TPP_EOK;
						}
					}
					break;
				case 't':
					if (tpp_streq(arg, "ok\0")) { /* --tok */
						tpp_emitter_setmode(&self->tf_emitter, TPP_EMITTER_MODE_BRACKET);
						return TPP_EOK;
					}
					break;
				case 'p':
					if (tpp_streq(arg, "p\0")) { /* --pp */
						tpp_emitter_setmode(&self->tf_emitter, TPP_EMITTER_MODE_ZERO);
						return TPP_EOK;
					}
					break;
#endif /* TPP_FRONTEND_HAVE_TPP2_COMPAT */

				default: break;
				}
				break;

			case 'f': {
				bool no = false;
				if (tpp_streq(arg, "no-"))
					arg += 3, no = true;
#if TPP_FRONTEND_HAVE_TPP2_COMPAT
				if (tpp_streq(arg, "spc\0")) {
					return tpp_lexer_setextension(&self->tf_lexer, TPP_EXT_TOK_SPACE, !no);
				} else if (tpp_streq(arg, "lf\0")) {
					return tpp_lexer_setextension(&self->tf_lexer, TPP_EXT_TOK_LF, !no);
				} else if (tpp_streq(arg, "comments\0")) {
					return tpp_lexer_setextension(&self->tf_lexer, TPP_EXT_TOK_COMMENT, !no);
				} else if (tpp_streq(arg, "longstring\0")) {
					tpp_errno error;
					error = tpp_lexer_setextension(&self->tf_lexer, TPP_EXT_STRING_ALLOW_MULTILINE, no);
					if (!TPP_ISERR(error)) {
						error = tpp_lexer_setwarninggrp(&self->tf_lexer, TPP_WG_MULTILINE_STRING,
						                                no ? TPP_WSTATE_WARN : TPP_WSTATE_DISABLED);
					}
					return error;
				} else if (tpp_streq(arg, "unify-pragma\0")) {
					tpp_emitter_set_reemit_unknown_pragma(&self->tf_emitter, !no);
					return TPP_EOK;
				} else if (tpp_streq(arg, "line\0")) {
					if (no) {
						tpp_emitter_enablefeature(&self->tf_emitter, TPP_EMITTER_FEAT_NOLINE);
					} else {
						tpp_emitter_disablefeature(&self->tf_emitter, TPP_EMITTER_FEAT_NOLINE);
						tpp_emitter_disablefeature(&self->tf_emitter, TPP_EMITTER_FEAT_USE_CPP_DIGIT);
					}
					return TPP_EOK;
				} else if (tpp_streq(arg, "cpp-line\0") && !no) {
					tpp_emitter_disablefeature(&self->tf_emitter, TPP_EMITTER_FEAT_NOLINE);
					tpp_emitter_enablefeature(&self->tf_emitter, TPP_EMITTER_FEAT_USE_CPP_DIGIT);
					return TPP_EOK;
				}
#endif /* TPP_FRONTEND_HAVE_TPP2_COMPAT */
			}	break;

#if TPP_FRONTEND_HAVE_TPP2_COMPAT
			case 'i': {
				if (*arg == '\0') {
					/* XXX: Support TPP2-style CLI aliases:
					 * - -i ...: Construct TEXT-file from remainder of CLI, which is then
					 *          used as input (using 'TPP_CONFIG_CLI_FILENAME' as filename) */
				}
			}	break;
#endif /* TPP_FRONTEND_HAVE_TPP2_COMPAT */

			default: break;
			}

			/* Try to consult "flag" parsers. */
			while (*after_dash) {
				tpp_errno error = tpp_cli_loader_parseflag(&self->tf_cli_loader, &after_dash);
				if (error == TPP_ENOENT)
					error = tpp_emitter_cli_loader_parseflag(&self->tf_emitter_cli_loader, &after_dash);
				if (error == TPP_ENOENT)
					error = tpp_makefile_cli_loader_parseflag(&self->tf_makefile_cli_loader, &after_dash);
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
#ifdef _MSC_VER
	/* Disable stupid LF -> CRLF conversion for standard streams on windows */
	(void)_setmode(0, _O_BINARY);
	(void)_setmode(1, _O_BINARY);
	(void)_setmode(2, _O_BINARY);
#endif /* _MSC_VER */
#endif /* TPP_OS_WINDOWS */

	/* Initialize frontend */
	tpp_lexer_init(&fe.tf_lexer);
	tpp_emitter_init(&fe.tf_emitter, &fe.tf_lexer, &tpp_frontend_emitter_output_printer);
	tpp_makefile_init(&fe.tf_makefile, &fe.tf_lexer, &tpp_frontend_makefile_output_printer);
	tpp_cli_loader_init(&fe.tf_cli_loader, &fe.tf_lexer);
	tpp_emitter_cli_loader_init(&fe.tf_emitter_cli_loader, &fe.tf_emitter);
	tpp_makefile_cli_loader_init(&fe.tf_makefile_cli_loader, &fe.tf_makefile);
	fe.tf_output_file = NULL;
	fe.tf_output_filename = NULL;
	fe.tf_cli_state = TPP_FRONTEND_CLI_STATE_NORMAL;

	/* Parse arguments... */
	error = tpp_cli_loader_parseargv(&fe.tf_cli_loader, &argc, &argv); /* CORE */
	if (!TPP_ISERR(error))
		error = tpp_emitter_cli_loader_parseargv(&fe.tf_emitter_cli_loader, &argc, &argv); /* EMITTER */
	if (!TPP_ISERR(error))
		error = tpp_makefile_cli_loader_parseargv(&fe.tf_makefile_cli_loader, &argc, &argv); /* MAKEFILE */
	if (!TPP_ISERR(error))
		error = tpp_frontend_parseargv(&fe, &argc, &argv); /* FRONTEND */
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to parse arguments: %s\n", tpp_strerror(error));
		goto out_emitter_cli;
	}

	/* TODO: Support for environment variables, as described here: https://gcc.gnu.org/onlinedocs/cpp/Environment-Variables.html
	 * TODO: Where possible, these variables should *NOT* be checked-for and parsed *here*,
	 *       but should instead only be checked-for and parsed when they become relevant:
	 * - e.g. `SOURCE_DATE_EPOCH` should only be checked just before `tpp_time_now()`
	 * - e.g. the different include-path variables should be loaded in `tpp_lexer_foreach_include_path()`
	 */

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
			goto out_emitter_cli;
		} else if (tpp_strcmp(*argv, "--version") == 0) {
			tpp_frontend_version();
			result = 0;
			goto out_emitter_cli;
		}
		fprintf(stderr, "unknown argument %s\n"
		                "Usage: %s [ARGS...] [--] [INFILES]\n"
		                "See --help for more info\n",
		        *argv, appname);
		goto out_emitter_cli;
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
				tpp_lexer_initfile_io_ex(&fe.tf_lexer,
				                         filename, tpp_get_stdin(),
				                         TPP_FILE_FLAGS_NOCLOSE);
				error = TPP_EOK;
			} else {
				error = tpp_lexer_pushfile_io_ex(&fe.tf_lexer,
				                                 filename, tpp_get_stdin(),
				                                 TPP_FILE_FLAGS_NOCLOSE);
			}
		} else
#endif /* tpp_get_stdin */
		{
			if (first_file) {
				error = tpp_lexer_initfile_open(&fe.tf_lexer,
				                                filename, TPP_SIZE_MAX);
			} else {
				error = tpp_lexer_pushfile_open(&fe.tf_lexer,
				                                filename, TPP_SIZE_MAX);
			}
		}
		if (TPP_ISERR(error)) {
			fprintf(stderr, "failed to open '%s': %s\n", filename, tpp_strerror(error));
			if (first_file)
				goto out_emitter_cli;
			goto out_emitter_cli_file;
		}
		first_file = false;
	} while (argc);

	/* Flush CLI loaders */
	error = tpp_makefile_cli_loader_flush(&fe.tf_makefile_cli_loader,
	                                      fe.tf_output_filename);
	if (!TPP_ISERR(error))
		error = tpp_emitter_cli_loader_flush(&fe.tf_emitter_cli_loader);
	if (!TPP_ISERR(error))
		error = tpp_cli_loader_flush(&fe.tf_cli_loader);

	/* Finalize CLI loaders */
	tpp_makefile_cli_loader_fini(&fe.tf_makefile_cli_loader);
	tpp_emitter_cli_loader_fini(&fe.tf_emitter_cli_loader);
	tpp_cli_loader_fini(&fe.tf_cli_loader);

	/* Final check for errors. */
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to complete arguments: %s\n", tpp_strerror(error));
		goto out_emitter_file;
	}

	/* Yield & re-emit tokens. */
	for (;;) {
		tpp_token_id const tok = tpp_lexer_yield(&fe.tf_lexer);
		if (TPP_TOK_ISERR(tok)) {
			fprintf(stderr, "yield failed: %s\n", tpp_strerror(TPP_TOK_ASERR(tok)));
			goto out_emitter_file;
		}
		if (tok == TPP_TOK_EOF)
			break;

		(void)tpp_emitter_emitcurrent(&fe.tf_emitter);
	}

	/* Flush Makefile output */
	error = tpp_makefile_flush(&fe.tf_makefile);
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to flush makefile: %s\n", tpp_strerror(error));
		goto out_emitter_file;
	}

	/* Return "0" if there were no TPP_WSTATE_ERROR-level messages */
	if (tpp_lexer_geterrorcount(&fe.tf_lexer) == 0)
		result = 0;

out_emitter_file:
	tpp_lexer_finifile(&fe.tf_lexer);
out_emitter:
	tpp_emitter_fini(&fe.tf_emitter);
	tpp_lexer_fini(&fe.tf_lexer);
#if defined(_MSC_VER) && defined(_CRTDBG_MAP_ALLOC)
	_CrtDumpMemoryLeaks();
#endif /* _MSC_VER && _CRTDBG_MAP_ALLOC */
	return result;
out_emitter_cli_file:
	tpp_lexer_finifile(&fe.tf_lexer);
out_emitter_cli:
	tpp_makefile_cli_loader_fini(&fe.tf_makefile_cli_loader);
	tpp_emitter_cli_loader_fini(&fe.tf_emitter_cli_loader);
	tpp_cli_loader_fini(&fe.tf_cli_loader);
	goto out_emitter;
}

TPP_DECL_END
