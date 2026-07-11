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

#include "tpp.h"

#include <stdio.h>

#if TPP_OS_WINDOWS
#include <Windows.h>
#endif /* TPP_OS_WINDOWS */

TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_DUMP_DEFINITIONS
static TPP_FORMATPRINTER_DEFINE(dump_defs_printer, arg, text, num_bytes) {
	(void)arg;
	fwrite(text, 1, num_bytes, stderr);
	return 0;
}
#endif /* TPP_HAVE_LEXER_DUMP_DEFINITIONS */

/* TODO: Properly write this one out (with all the bells n' whistles of GCC's CPP) */

int main(int argc, char *argv[]) {
	int result = 1;
	tpp_lexer lexer;
	tpp_errno error;
	tpp_token_id tok;
	char const *filename = "input.c";
	if (argc) {
		--argc;
		++argv;
	}

#if TPP_OS_WINDOWS
	SetConsoleOutputCP(CP_UTF8);
#endif /* TPP_OS_WINDOWS */

	if (argc)
		filename = *argv;
	tpp_lexer_init(&lexer);
	error = tpp_lexer_initfile_open(&lexer, filename, TPP_SIZE_MAX);
	if (TPP_ISERR(error)) {
#if TPP_HAVE_STRERROR
		fprintf(stderr, "Initialization failed: %s\n", tpp_strerror(error));
#else /* TPP_HAVE_STRERROR */
		fprintf(stderr, "Initialization failed: %d\n", (int)error);
#endif /* !TPP_HAVE_STRERROR */
		tpp_lexer_fini(&lexer);
		return 1;
	}

	for (;;) {
		tok = tpp_lexer_yield(&lexer);
		if (TPP_TOK_ISERR(tok))
			break;
		if (tok == TPP_TOK_EOF) {
			error = tpp_lexer_warn_nonempty_ifdef(&lexer);
			if (TPP_ISERR(error))
				tok = TPP_TOK_OFERR(error);
			break;
		}
#if 0
		fwrite(tpp_lexer_gettokenstart(&lexer), 1,
		       tpp_lexer_gettokenlen(&lexer), stdout);
#elif 1
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
	if (TPP_TOK_ISERR(tok)) {
#if TPP_HAVE_STRERROR
		fprintf(stderr, "Yield failed: %s\n", tpp_strerror(TPP_TOK_ASERR(tok)));
#else /* TPP_HAVE_STRERROR */
		fprintf(stderr, "Yield failed: %d\n", (int)TPP_TOK_ASERR(tok));
#endif /* !TPP_HAVE_STRERROR */
		goto out;
	}
#if TPP_HAVE_LEXER_DUMP_DEFINITIONS
	tpp_lexer_dump_definitions(&lexer, &dump_defs_printer, NULL,
	                           TPP_LEXER_DUMP_DEFINITIONS_ALL |
	                           TPP_LEXER_DUMP_DEFINITIONS_SORTED |
	                           TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO);
#endif /* TPP_HAVE_LEXER_DUMP_DEFINITIONS */

	if (tpp_lexer_geterrorcount(&lexer)) {
		fprintf(stderr, "There were lexer errors\n");
		goto out;
	}
	result = 0;

out:
	tpp_lexer_finifile(&lexer);
	tpp_lexer_fini(&lexer);

#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif /* _MSC_VER */
	return result;
}

TPP_DECL_END
