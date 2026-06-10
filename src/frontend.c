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
	error = tpp_lexer_init_filename(&lexer, filename);
	if (TPP_ISERR(error)) {
		fprintf(stderr, "Initialization failed: %s\n", tpp_strerror(error));
		return 1;
	}

	for (;;) {
		char const *desc, *lexer_filename;
		tpp_lcinfo lc;
		tpp_file *file;
		tok = tpp_lexer_yield(&lexer);
		if (TPP_TOK_ISERR(tok))
			break;
		if (tok == TPP_TOK_EOF) {
			error = tpp_lexer_warn_nonempty_ifdef(&lexer);
			if (TPP_ISERR(error))
				tok = TPP_TOK_OFERR(error);
			break;
		}
		desc = tpp_strtokenid(tok);
		if (desc == NULL && TPP_TOK_ISKEYWORD(tok))
			desc = tpp_keyword_getkwdcstr(tpp_lexer_gettokenkwd(&lexer));
		if (desc == NULL)
			desc = "?";
		file = tpp_lexer_getfile(&lexer);
		lexer_filename = tpp_file_filename(file);
		lc = tpp_file_lcinfo(file, tpp_lexer_gettokenstart(&lexer));
#if 1
		printf("[%.*s]",
		       (int)tpp_lexer_gettokenlen(&lexer),
		       tpp_lexer_gettokenstart(&lexer));
#else
		printf("[%s:%d:%d:%s(%d):%.*s]\n",
		       lexer_filename ? lexer_filename : "?",
		       tpp_lcinfo_getline(lc) + 1,
		       tpp_lcinfo_getcol(lc) + 1,
		       desc, tok,
		       (int)tpp_lexer_gettokenlen(&lexer),
		       tpp_lexer_gettokenstart(&lexer));
#endif
	}
	if (TPP_TOK_ISERR(tok)) {
		fprintf(stderr, "Yield failed: %s\n", tpp_strerror(TPP_TOK_ASERR(tok)));
		goto out;
	}
	if (tpp_lexer_geterrorcount(&lexer)) {
		fprintf(stderr, "There were lexer errors\n");
		goto out;
	}
	result = 0;
out:
	tpp_lexer_fini(&lexer);
	return result;
}

TPP_DECL_END
