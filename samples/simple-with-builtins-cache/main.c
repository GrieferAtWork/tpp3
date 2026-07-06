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
		char const *fn;
		tpp_lcinfo lc;
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
		fn = tpp_lexer_getlcfilename(&lexer);
		lc = tpp_lexer_getlcinfo(&lexer);
		printf("[%s:%d:%d:%s(%d):%.*s",
		       fn ? fn : "?",
		       (int)(tpp_lcinfo_getline(lc) + 1),
		       (int)(tpp_lcinfo_getcol(lc) + 1),
		       desc, tok,
		       (int)tpp_lexer_gettokenlen(&lexer),
		       tpp_lexer_gettokenstart(&lexer));
		printf("]\n");
	}
	result = 0;
out:
	tpp_lexer_finifile(&lexer);
	tpp_lexer_fini(&lexer);
	return result;
}

TPP_DECL_END
