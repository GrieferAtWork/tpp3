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
#include <Windows.h>
#endif /* TPP_OS_WINDOWS */

TPP_DECL_BEGIN

static TPP_FORMATPRINTER_DEFINE(output_printer, arg, text, num_bytes) {
	(void)arg;
	fwrite(text, 1, num_bytes, stdout);
	return 0;
}

int main(int argc, char **argv) {
	int result    = 1;
	char *appname = argv[0];
	tpp_errno error;
	tpp_cli_loader cli_loader;
	tpp_emitter emitter;
	tpp_lexer *lexer;
	char const *filename = "input.c";

#if TPP_OS_WINDOWS
	SetConsoleOutputCP(CP_UTF8);
#endif /* TPP_OS_WINDOWS */

	tpp_emitter_init(&emitter, &output_printer);
	lexer = tpp_emitter_getlexer(&emitter);
	tpp_cli_loader_init(&cli_loader, lexer);
	if (argc)
		--argc, ++argv; /* Skip "appname" argument */
	error = tpp_cli_loader_parseargv(&cli_loader, &argc, &argv);
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to parse arguments: %s\n", tpp_strerror(error));
		goto out_emitter_loader;
	}

	/* TODO: Support for CLI arguments that must be handled by front-end:
	 * - "-o file", "--output file", "--output=file"
	 *   No special handling needed in TPP backend
	 *   Simply redirect what below stuff often calls "preprocessor output" to "file"
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
	 * - "-fsearch-include-path[=kind]"  (kind=R"(user|system)", default="user")
	 *   No special handling needed in TPP backend
	 *   When kind=user, and the main input file could not be found, it must be
	 *   searched-for using `tpp_lexer_foreach_include_path(TPP_TOK_INCPATH_DQUOTE)`
	 *   When kind=system, and the main input file could not be found, it must be
	 *   searched-for using `tpp_lexer_foreach_include_path(TPP_TOK_INCPATH_LANGLE)`
	 * - "-fworking-directory"
	 *   No special handling needed in TPP backend
	 *   The first time the frontend emits a `# <linenum>` marker, it must simply
	 *   follow this up by emitting a second marker like: `# 1 "$(pwd)//"`
	 *
	 * - "-P", "--no-line-commands"
	 *   No special handling needed in TPP backend
	 *   Frontend simply mustn't emit any `# <linenum>` or `#line` directives
	 * - "-H", "--trace-includes"
	 *   - Use TPP_HAVE_FILE_PUSHED_HOOK to filter for TPP_FILE_KIND_IO files being
	 *     pushed onto the #include-stack. Whenever that has happened, print a line
	 *     like this to stderr:
	 *     >> print("." * NUMBER_OF_IO_FILES_ON_INCLUDE_STACK, " ", tpp_file_getrealfilename(file));
	 * - "-dM", "--dump=M"
	 *   - Call `tpp_lexer_dump_definitions(TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS)` and
	 *     print the results to our output just before the first call to `tpp_lexer_yield()`.
	 *   - Set hooks for:
	 *     - TPP_HAVE_MACRO_DEFINED_HOOK
	 *     - TPP_HAVE_MACRO_UNDEFINED_HOOK
	 *     ... that print a replication of the operation in its canonical form to our preprocessor output
	 *   - Also: turn off output of tokens to preprocessor output
	 * - "-dD", "--dump=D"
	 *   No special handling needed in TPP backend
	 *   Same as "-dM", but don't turn off output of tokens to preprocessor output
	 * - "-dN", "--dump=N"
	 *   No special handling needed in TPP backend
	 *   Same as "-dD", but only write `#define FOO` for `#define FOO(x, y) 10` or `#define FOO 20`
	 * - "-dI", "--dump=I"
	 *   - Set hook for TPP_HAVE_INCLUDE_ENCOUNTERED_HOOK that echoes the operator
	 *     to preprocessor output
	 * - "-dU", "--dump=U"
	 *   No (additional) special handling needed in TPP backend
	 *   - Have a map `currently_defined_used_macros: {tpp_keyword: tpp_macro}`
	 *   - Use TPP_HAVE_FILE_PUSHED_HOOK to watch for TPP_FILE_KIND_MACRO-files being pushed
	 *     - Whenever this happens, see if the associated macro is in `currently_defined_used_macros`,
	 *       and if the macro stored in `currently_defined_used_macros` is the same one that's just
	 *       been pushed onto the #include-stack:
	 *       - If the macros differ, print a `#undef <NAME>` line first
	 *       - If the macros differ, or there wasn't a pre-existing macro under
	 *         that name, print a `#define <NAME>...` line to declare the new macro's
	 *         definition
	 *   - Whenever a TPP_TOK_ISKEYWORD()-token is read from the preprocessor, and
	 *     the associated keyword doesn't have a user-defined macro expansion, check
	 *     `currently_defined_used_macros` if it still contains a macro definition
	 *     for that keyword. If so: remove it from `currently_defined_used_macros`,
	 *     and print a `#undef <NAME>` line before the keyword-token is handled as
	 *     per usual.
	 */
	if (argc && strcmp(*argv, "--") == 0)
		--argc, ++argv;
	if (argc == 1) {
		filename = argv[0];
	} else if (argc != 0) {
		fprintf(stderr, "bad arguments\nUSAGE: %s [ARGS...] [INFILE]\n", appname);
		goto out_emitter_loader;
	}
	error = tpp_lexer_initfile_open(lexer, filename, TPP_SIZE_MAX);
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to open '%s': %s\n", filename, tpp_strerror(error));
		goto out_emitter_loader;
	}
	error = tpp_cli_loader_flush(&cli_loader);
	tpp_cli_loader_fini(&cli_loader);
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to complete arguments: %s\n", tpp_strerror(error));
		goto out_emitter_file;
	}
	for (;;) {
		tpp_token_id const tok = tpp_lexer_yield(lexer);
		if (TPP_TOK_ISERR(tok)) {
			fprintf(stderr, "yield failed: %s\n", tpp_strerror(TPP_TOK_ASERR(tok)));
			break;
		}
		if (tok == TPP_TOK_EOF)
			break;

#if 1
		(void)tpp_emitter_emitcurrent(&emitter);
#elif 0
		fwrite(tpp_lexer_gettokenstart(lexer), 1,
		       tpp_lexer_gettokenlen(lexer), stdout);
#elif 0
		printf("[%.*s]",
		       (int)tpp_lexer_gettokenlen(lexer),
		       tpp_lexer_gettokenstart(lexer));
#elif 1
		{
			char const *desc = tpp_strtokenid(tok);
			if (desc == NULL && tpp_lexer_hastokenkwd(lexer))
				desc = tpp_lexer_gettokenkwdcstr(lexer);
			if (desc == NULL)
				desc = "?";
			printf("[%s:%.*s]", desc,
			       (int)tpp_lexer_gettokenlen(lexer),
			       tpp_lexer_gettokenstart(lexer));
		}
#else
		{
			tpp_lcinfo_ex lc;
			tpp_file *file = tpp_lexer_getfile(lexer);
			char const *lexer_filename = tpp_file_getfilename(file);
			char const *desc = tpp_strtokenid(tok);
			if (desc == NULL && tpp_lexer_hastokenkwd(lexer))
				desc = tpp_lexer_gettokenkwdcstr(lexer);
			if (desc == NULL)
				desc = "?";
			tpp_file_getlcinfo_ex(file, tpp_lexer_gettokenstart(lexer), &lc);
			printf("[%s:%d:%d:%s(%d):%.*s",
			       lexer_filename ? lexer_filename : "?",
			       (int)(tpp_lcinfo_getline(lc.tlcix_info) + 1),
			       (int)(tpp_lcinfo_getcol(lc.tlcix_info) + 1),
			       desc, tok,
			       (int)tpp_lexer_gettokenlen(lexer),
			       tpp_lexer_gettokenstart(lexer));
#if TPP_HAVE_CPP_MACROS
			while (lc.tlcix_projfile) {
				tpp_file_getlcinfo_ex(lc.tlcix_projfile, lc.tlcix_projpos, &lc);
				lexer_filename = tpp_file_getfilename(file);
				printf(" --- %s:%d:%d",
				       lexer_filename ? lexer_filename : "?",
				       (int)(tpp_lcinfo_getline(lc.tlcix_info) + 1),
				       (int)(tpp_lcinfo_getcol(lc.tlcix_info) + 1));
			}
#endif /* TPP_HAVE_CPP_MACROS */
		}
		printf("]\n");
#endif
	}

#if TPP_HAVE_LEXER_DUMP_DEFINITIONS
	tpp_lexer_dump_definitions(lexer, &output_printer, NULL,
	                           TPP_LEXER_DUMP_DEFINITIONS_ALL |
	                           TPP_LEXER_DUMP_DEFINITIONS_SORTED |
	                           TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO);
#endif /* TPP_HAVE_LEXER_DUMP_DEFINITIONS */
	if (tpp_lexer_geterrorcount(lexer)) {
		fprintf(stderr, "There were lexer errors\n");
		goto out_emitter_file;
	}
	result = 0;

out_emitter_file:
	tpp_lexer_finifile(lexer);
out_emitter:
	tpp_emitter_fini(&emitter);
#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif /* _MSC_VER */
	return result;
out_emitter_loader:
	tpp_cli_loader_fini(&cli_loader);
	goto out_emitter;
}

TPP_DECL_END
