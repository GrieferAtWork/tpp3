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
#define TPP_PROFILE TPP_PROFILE_ALL
#define TPP_COMMON_HAVE_FEATURES 0

#include "tpp-amalgamation.c"
#else /* USE_AMALGAMATION */
#include "tpp.h"
#endif /* !USE_AMALGAMATION */

#include <stdio.h>

#if TPP_OS_WINDOWS
#include <Windows.h>
#endif /* TPP_OS_WINDOWS */

TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_DUMP_DEFINITIONS
static TPP_FORMATPRINTER_DEFINE(dump_defs_printer, arg, text, num_bytes) {
	(void)arg;
	fwrite(text, 1, num_bytes, stdout);
	return 0;
}
#endif /* TPP_HAVE_LEXER_DUMP_DEFINITIONS */

/* TODO: Properly write this one out (with all the bells n' whistles of GCC's CPP) */
int main(int argc, char **argv) {
	int result    = 1;
	char *appname = argv[0];
	tpp_errno error;
	tpp_lexer lexer;
	tpp_cli_loader cli_loader;
	char const *filename = "input.c";

#if TPP_OS_WINDOWS
	SetConsoleOutputCP(CP_UTF8);
#endif /* TPP_OS_WINDOWS */

	tpp_lexer_init(&lexer);
	tpp_cli_loader_init(&cli_loader, &lexer);
	if (argc)
		--argc, ++argv; /* Skip "appname" argument */
	error = tpp_cli_loader_parseargv(&cli_loader, &argc, &argv);
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to parse arguments: %s\n", tpp_strerror(error));
		goto out_lexer_loader;
	}

	/* XXX: Support for CLI arguments that must be handled by front-end:
	 * - "-M", "--dependencies"
	 * - "-MM", "--user-dependencies"
	 * - "-MF file"
	 * - "-MG", "--print-missing-file-dependencies"
	 * - "-MP"
	 * - "-MT target"
	 * - "-MQ target"
	 * - "-MD", "--write-dependencies"
	 * - "-MMD", "--write-user-dependencies"
	 * - "-fsearch-include-path[=kind]"
	 * - "-fworking-directory"
	 * - "-P", "--no-line-commands"
	 * - "-H", "--trace-includes"
	 * - "-dM", "--dump=M"
	 * - "-dD", "--dump=D"
	 * - "-dN", "--dump=N"
	 * - "-dI", "--dump=I"
	 * - "-dU", "--dump=U"
	 */
	if (argc && strcmp(*argv, "--") == 0)
		--argc, ++argv;
	if (argc == 1) {
		filename = argv[0];
	} else if (argc != 0) {
		fprintf(stderr, "bad arguments\nUSAGE: %s [ARGS...] [INFILE]\n", appname);
		goto out_lexer_loader;
	}
	error = tpp_lexer_initfile_open(&lexer, filename, TPP_SIZE_MAX);
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to open '%s': %s\n", filename, tpp_strerror(error));
		goto out_lexer_loader;
	}
	error = tpp_cli_loader_flush(&cli_loader);
	tpp_cli_loader_fini(&cli_loader);
	if (TPP_ISERR(error)) {
		fprintf(stderr, "failed to complete arguments: %s\n", tpp_strerror(error));
		goto out_lexer_file;
	}

	for (;;) {
		tpp_token_id tok = tpp_lexer_yield(&lexer);
		if (TPP_TOK_ISERR(tok)) {
			fprintf(stderr, "yield failed: %s\n", tpp_strerror(TPP_TOK_ASERR(tok)));
			break;
		}
		if (tok == TPP_TOK_EOF)
			break;
#if 0
		fwrite(tpp_lexer_gettokenstart(&lexer), 1,
		       tpp_lexer_gettokenlen(&lexer), stdout);
#elif 0
		printf("[%.*s]",
		       (int)tpp_lexer_gettokenlen(&lexer),
		       tpp_lexer_gettokenstart(&lexer));
#elif 1
		{
			char const *desc = tpp_strtokenid(tok);
			if (desc == NULL && tpp_lexer_hastokenkwd(&lexer))
				desc = tpp_lexer_gettokenkwdcstr(&lexer);
			if (desc == NULL)
				desc = "?";
			printf("[%s:%.*s]", desc,
			       (int)tpp_lexer_gettokenlen(&lexer),
			       tpp_lexer_gettokenstart(&lexer));
		}
#else
		{
			tpp_lcinfo_ex lc;
			tpp_file *file = tpp_lexer_getfile(&lexer);
			char const *lexer_filename = tpp_file_getfilename(file);
			char const *desc = tpp_strtokenid(tok);
			if (desc == NULL && tpp_lexer_hastokenkwd(&lexer))
				desc = tpp_lexer_gettokenkwdcstr(&lexer);
			if (desc == NULL)
				desc = "?";
			tpp_file_getlcinfo_ex(file, tpp_lexer_gettokenstart(&lexer), &lc);
			printf("[%s:%d:%d:%s(%d):%.*s",
			       lexer_filename ? lexer_filename : "?",
			       (int)(tpp_lcinfo_getline(lc.tlcix_info) + 1),
			       (int)(tpp_lcinfo_getcol(lc.tlcix_info) + 1),
			       desc, tok,
			       (int)tpp_lexer_gettokenlen(&lexer),
			       tpp_lexer_gettokenstart(&lexer));
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
	tpp_lexer_dump_definitions(&lexer, &dump_defs_printer, NULL,
	                           TPP_LEXER_DUMP_DEFINITIONS_ALL |
	                           TPP_LEXER_DUMP_DEFINITIONS_SORTED |
	                           TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO);
#endif /* TPP_HAVE_LEXER_DUMP_DEFINITIONS */
	if (tpp_lexer_geterrorcount(&lexer)) {
		fprintf(stderr, "There were lexer errors\n");
		goto out_lexer_file;
	}
	result = 0;

out_lexer_file:
	tpp_lexer_finifile(&lexer);
out_lexer:
	tpp_lexer_fini(&lexer);
#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif /* _MSC_VER */
	return result;
out_lexer_loader:
	tpp_cli_loader_fini(&cli_loader);
	goto out_lexer;
}

TPP_DECL_END
