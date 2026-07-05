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

#include "config.h"

#define TPP_CONFIG_USERDEFS_FILENAME \
	"../samples/simple-with-builtins-cache/defs.h"
#define TPP_CONFIG_BUILTINS_FILENAME \
	"../samples/simple-with-builtins-cache/make-amalgamation-builtins.inl"
#include "../../src/tpp-amalgamation.c"

#include <stdio.h>

TPP_DECL_BEGIN

int main(int argc, char *argv[]) {
	int result = 1;
	tpp_lexer lexer;
	tpp_errno error;
	tpp_token_id tok;
	char const *filename = "input.txt";
	if (argc) {
		--argc;
		++argv;
	}

	if (argc)
		filename = *argv;
	tpp_lexer_init(&lexer);
	error = tpp_lexer_initfile_open(&lexer, filename, TPP_SIZE_MAX);
	if (error != TPP_EOK) {
		fprintf(stderr, "Initialization failed: %s\n", tpp_strerror(error));
		tpp_lexer_fini(&lexer);
		return 1;
	}

	for (;;) {
		tpp_lcinfo_ex lc;
		tpp_file *file             = tpp_lexer_getfile(&lexer);
		char const *lexer_filename = tpp_file_getfilename(file);
		char const *desc;
		tok = tpp_lexer_yield(&lexer);
		if (TPP_TOK_ISERR(tok)) {
			fprintf(stderr, "\nYield failed: %s\n", tpp_strerror(TPP_TOK_ASERR(tok)));
			goto out;
		}
		if (tok == TPP_TOK_EOF)
			break;
		desc = tpp_strtokenid(tok);
		if (desc == NULL && tpp_lexer_hastokenkwd(&lexer))
			desc = tpp_lexer_gettokenkwdcstr(&lexer);
		if (desc == NULL)
			desc = "?";
		file           = tpp_lexer_getfile(&lexer);
		lexer_filename = tpp_file_getfilename(file);
		tpp_file_getlcinfo_ex(file, tpp_lexer_gettokenstart(&lexer), &lc);
		printf("[%s:%d:%d:%s(%d):%.*s",
		       lexer_filename ? lexer_filename : "?",
		       (int)(tpp_lcinfo_getline(lc.tlcix_info) + 1),
		       (int)(tpp_lcinfo_getcol(lc.tlcix_info) + 1),
		       desc, tok,
		       (int)tpp_lexer_gettokenlen(&lexer),
		       tpp_lexer_gettokenstart(&lexer));
		while (lc.tlcix_projfile) {
			tpp_file_getlcinfo_ex(lc.tlcix_projfile, lc.tlcix_projpos, &lc);
			lexer_filename = tpp_file_getfilename(file);
			printf(" --- %s:%d:%d",
			       lexer_filename ? lexer_filename : "?",
			       (int)(tpp_lcinfo_getline(lc.tlcix_info) + 1),
			       (int)(tpp_lcinfo_getcol(lc.tlcix_info) + 1));
		}
		printf("]\n");
	}
	result = 0;
out:
	tpp_lexer_finifile(&lexer);
	tpp_lexer_fini(&lexer);
	return result;
}

TPP_DECL_END
