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
#ifndef GUARD_TPP_LEXER_SEEK_RPAREN_C
#define GUARD_TPP_LEXER_SEEK_RPAREN_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "error.h"
#include "string.h"
#include "lexer.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_SEEK_RPAREN

typedef struct tpp_lexer_arginfo_rel {
	tpp_size tlair_start; /* [<= tlai_end] Relative offset to argument start text data */
	tpp_size tlair_end;   /* [>= tlai_start] Relative offset to argument end text data */
} tpp_lexer_arginfo_rel;

TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer_arginfo_rel, tlair_start) ==
                  tpp_offsetof(tpp_lexer_arginfo, tlai_start));
TPP_STATIC_ASSERT(tpp_offsetof(tpp_lexer_arginfo_rel, tlair_end) ==
                  tpp_offsetof(tpp_lexer_arginfo, tlai_end));
TPP_STATIC_ASSERT(sizeof(tpp_lexer_arginfo_rel) == sizeof(tpp_lexer_arginfo));

#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
#define TPP_RECURSION_CC_PAREN   0
#define TPP_RECURSION_CC_BRACKET 1
#define TPP_RECURSION_CC_BRACE   2
#define TPP_RECURSION_CC_ANGLE   3

static TPP_CONSTCALL TPP_WUNUSED unsigned int TPPCALL
tpp_get_recursion_cc(tpp_token_id lparen_kind) {
	switch (lparen_kind) {
	case TPP_TOK_OFCHAR('('):
		return TPP_RECURSION_CC_PAREN;
	case TPP_TOK_OFCHAR('['):
		return TPP_RECURSION_CC_BRACKET;
	case TPP_TOK_OFCHAR('{'):
		return TPP_RECURSION_CC_BRACE;
	case TPP_TOK_OFCHAR('<'):
		return TPP_RECURSION_CC_ANGLE;
	default: tpp_unreachable();
	}
}
#else /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
#define TPP_RECURSION_CC_PAREN 0
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN_EX */



/* Find the position of the next unmatched ')'-token, gathering information
 * about ranges of ','-token-separated text-sequences along the way.
 *
 * NOTE: This function automatically handles "TPP_TOK_EWOULDBLOCK" (meaning it always blocks)
 *
 * @param: p_pos: [in/out]  In:  Pointer to first character to start checking for ')'
 *                          Out: Pointer to the trailing ')', or EOF
 *                               if end-of-file was reached first, or
 *                               in case of TPP_TOK_EWOULDBLOCK.
 * @param: p_argv: [out]    Output buffer for the bounds of macro
 *                          arguments encountered along the way.
 *                          The size of this buffer is IN(*p_argc)
 * @param: p_argc: [in/out] In:  Size of provided "p_argv" buffer (in elements)
 *                          Out: Number of arguments actually encountered. May
 *                               be set to a number greater tha IN(*p_argc), in
 *                               which case only info about the first IN(*p_argc)
 *                               arguments is actually returned. But note the
 *                               `TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS' flag, which
 *                               causes the last element of `p_argv' to span up
 *                               to the closing ')'
 * @param: opt_function_name_for_messages:
 *                          Function name for too-many-arguments/end-of-params message
 * @param: flags:           Set of `TPP_LEXER_SEEK_RPAREN_FLAG_*'
 *
 * @return: TPP_TOK_EOF:         EOF was encountered before an unmatched ')' was found
 * @return: TPP_TOK_RPAREN:      Unmatched closing ')' was encountered
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seek_rparen_ex(tpp_lexer *tpp_restrict self,
                         tpp_char const **tpp_restrict p_pos,
                         tpp_lexer_arginfo *tpp_restrict p_argv,
                         tpp_size *tpp_restrict p_argc,
                         char const *opt_function_name_for_messages,
                         unsigned int flags,
                         tpp_token_id lparen_kind)
#else /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_token_id TPPCALL
tpp_lexer_seek_rparen(tpp_lexer *tpp_restrict self,
                      tpp_char const **tpp_restrict p_pos,
                      tpp_lexer_arginfo *tpp_restrict p_argv,
                      tpp_size *tpp_restrict p_argc,
                      char const *opt_function_name_for_messages,
                      unsigned int flags)
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN_EX */
{
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
#define tpp_lexer_seek_rparen_keepspace() (flags & TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC)
#else /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */
#define tpp_lexer_seek_rparen_keepspace() (TPP_HAVE_MACRO_ARGUMENT_WHITESPACE != 0)
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE >= 0 */

	tpp_lexer_arginfo_rel *const p_argv_rel = (tpp_lexer_arginfo_rel *)p_argv;
	tpp_file const *const file = tpp_lexer_getfile(self);
	tpp_size rel_start = tpp_file_ptr2rel(file, *p_pos);
	tpp_token_id tok;
	tpp_size const argv_bufsize = *p_argc;
	tpp_size i, argc = 0;
	tpp_size current_arg_rel_start;
	tpp_size current_arg_rel_end;
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
	/* Recursively nested ( ), [ ], { }, < > -pairs (in that order)
	 * The recursion level at any index can only be altered if all
	 * recursion levels with lower indices are set to 0:
	 * >> #define ARR[x] #x
	 * >> ARR[foo(])bar([)baz]
	 * Expands to 1 token:  "foo(])bar([)baz"
	 * rather than:         "foo("   )   bar   (   [   )   baz   ]   ) */
	tpp_ssize recursion[4] = { 0, 0, 0, 0 };
	unsigned int const tpp_recursion_cc = tpp_get_recursion_cc(lparen_kind);
#else /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
	tpp_ssize recursion[1] = { 0 }; /* Recursively nested ( )-pairs */
#define tpp_recursion_cc TPP_RECURSION_CC_PAREN
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN_EX */
	tpp_assert(p_pos != &file->tf_pos && "This would allow for file-popping and chunk deallocation, "
	                                     "neither of which are compatible with loading an argument "
	                                     "list");
	current_arg_rel_start = rel_start;
	current_arg_rel_end   = current_arg_rel_start;

	/* Yield first token. */
again_yield_and_switch_tok:
	tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
	switch (tok) {

	case TPP_TOK_EOF: {
#if TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST
		tpp_errno error;
		error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
		                           TPP_W_EOF_IN_ARGUMENT_LIST,
		                           opt_function_name_for_messages);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto done_err;
		}
#endif /* TPP_HAVE_TPP_W_EOF_IN_ARGUMENT_LIST */
		goto done;
	}	break;

	case TPP_TOK_SPACE:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT {
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
		if (tpp_lexer_seek_rparen_keepspace())
			break; /* When whitespace should be kept: treat it like a regular token */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0
		if (current_arg_rel_start == current_arg_rel_end) {
			/* Skip leading whitespace... */
			current_arg_rel_start = tpp_file_ptr2rel(file, *p_pos);
			current_arg_rel_end   = current_arg_rel_start;
			goto again_yield_and_switch_tok;
		}
		goto again_yield_and_switch_tok;
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE <= 0 */
	}	break;

	case '(':
		++recursion[0];
		break;

	case ')':
		if (recursion[TPP_RECURSION_CC_PAREN] == 0) {
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
			if (tpp_recursion_cc == TPP_RECURSION_CC_PAREN)
#endif /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
			{
				goto done;
			}
		}
		--recursion[TPP_RECURSION_CC_PAREN];
		break;

	case ',': {
		/* Make sure that we're not inside of a nested ( )-pair */
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_ANGLE] != 0)
			break;
#endif /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */

#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE
		if (tpp_lexer_seek_rparen_keepspace()) {
			/* Argument must includes whitespace preceding the ","-token */
			tpp_char const *comma_start = tpp_lexer_gettoken(self)->tt_start;
			current_arg_rel_end = tpp_file_ptr2rel(file, comma_start);
		}
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE */

		/* Write next argument */
		if (argc < argv_bufsize) {
			p_argv_rel[argc].tlair_start = current_arg_rel_start;
			p_argv_rel[argc].tlair_end   = current_arg_rel_end;
		}
		++argc;
		current_arg_rel_start = tpp_file_ptr2rel(file, *p_pos);
	}	break;

#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
	case '[':
		if (tpp_recursion_cc < TPP_RECURSION_CC_BRACKET)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		++recursion[TPP_RECURSION_CC_BRACKET];
		break;
	case ']':
		if (tpp_recursion_cc < TPP_RECURSION_CC_BRACKET)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] == 0)
			goto done;
		--recursion[TPP_RECURSION_CC_BRACKET];
		break;

	case '{':
		if (tpp_recursion_cc < TPP_RECURSION_CC_BRACE)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		++recursion[TPP_RECURSION_CC_BRACE];
		break;
	case '}':
		if (tpp_recursion_cc < TPP_RECURSION_CC_BRACE)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACE] == 0)
			goto done;
		--recursion[TPP_RECURSION_CC_BRACE];
		break;

#if (TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE ||              \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE ||       \
     TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_LANGLE_MINUS)
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	case TPP_TOK_LANGLE_EQUAL: /* "<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE: /* "<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_EQUAL: /* "<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	case TPP_TOK_LANGLE_LANGLE_LANGLE: /* "<<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	case TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: /* "<<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS
	case TPP_TOK_LANGLE_MINUS: /* "<-" */
#endif /* !TPP_HAVE_TPP_TOK_LANGLE_MINUS */
		/* Convert to "<" token */
		tpp_assert(tpp_lexer_gettoken(self)->tt_start < *p_pos);
		tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '<');
		*p_pos = tpp_lexer_gettoken(self)->tt_start + 1;
/*		tok = TPP_TOK_OFCHAR('<');             * Not necessary */
/*		tpp_lexer_gettoken(self)->tt_id = tok; * Not necessary */
		goto handle_langle;
#define WANT_handle_langle
		break;
#endif /* ... */



#if (TPP_HAVE_TPP_TOK_RANGLE_LANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||         \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||  \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	case TPP_TOK_RANGLE_LANGLE: /* "><" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE: /* ">>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	case TPP_TOK_RANGLE_EQUAL: /* ">=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_EQUAL: /* ">>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	case TPP_TOK_RANGLE_RANGLE_RANGLE: /* ">>>" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	case TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: /* ">>>=" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
		/* Convert to ">" token */
		tpp_assert(tpp_lexer_gettoken(self)->tt_start < *p_pos);
		tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '>');
		*p_pos = tpp_lexer_gettoken(self)->tt_start + 1;
		tok = TPP_TOK_OFCHAR('>');
		tpp_lexer_gettoken(self)->tt_id = tok;
		goto handle_rangle;
#define WANT_handle_rangle
		break;
#endif /* ... */



	/* Tokens where the first character is irrelevant, and need to be split into 2 */
#if (TPP_HAVE_TPP_TOK_MINUS_RANGLE ||               \
     TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR ||          \
     TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS ||          \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE ||               \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE || \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE ||               \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE ||        \
     TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE
	case TPP_TOK_MINUS_RANGLE: /* "->" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	case TPP_TOK_MINUS_RANGLE_STAR: /* "->*" */
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	case TPP_TOK_STAR_LANGLE_MINUS: /* "*<-" */
#endif /* !TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	case TPP_TOK_EQUAL_LANGLE: /* "=<" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE: /* "=<<" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	case TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE: /* "=<<<" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	case TPP_TOK_EQUAL_RANGLE: /* "=>" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE: /* "=>>" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	case TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE: /* "=>>>" */
#endif /* !TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
		/* Convert to 1-char token */
		tpp_assert(tpp_lexer_gettoken(self)->tt_start < *p_pos);
		*p_pos = tpp_lexer_gettoken(self)->tt_start + 1;
/*		tok = TPP_TOK_OFCHAR(tpp_lexer_gettoken(self)->tt_start[0]); * Not necessary */
/*		tpp_lexer_gettoken(self)->tt_id = tok;                       * Not necessary */
		break;
#endif /* ... */

	case '<':
#ifdef WANT_handle_langle
#undef WANT_handle_langle
handle_langle:
#endif /* WANT_handle_langle */
		if (tpp_recursion_cc < TPP_RECURSION_CC_ANGLE)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		++recursion[TPP_RECURSION_CC_ANGLE];
		break;

	case '>':
#ifdef WANT_handle_rangle
#undef WANT_handle_rangle
handle_rangle:
#endif /* WANT_handle_rangle */
		if (tpp_recursion_cc < TPP_RECURSION_CC_ANGLE)
			break;
		if (recursion[TPP_RECURSION_CC_PAREN] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACKET] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_BRACE] != 0)
			break;
		if (recursion[TPP_RECURSION_CC_ANGLE] == 0)
			goto done;
		--recursion[TPP_RECURSION_CC_ANGLE];
		break;

#endif /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */


	default:
		if (TPP_TOK_ISERR(tok))
			goto done_err;
		break;
	}
	current_arg_rel_end = tpp_file_ptr2rel(file, *p_pos);
	goto again_yield_and_switch_tok;

done:
	if (argc || (current_arg_rel_end > current_arg_rel_start)) {
		/* Write last argument */
		if (argc < argv_bufsize) {
			p_argv_rel[argc].tlair_start = current_arg_rel_start;
			p_argv_rel[argc].tlair_end   = current_arg_rel_end;
		} else if (argv_bufsize) {
			/* Update end-pointer of last argument (for varargs) */
			p_argv_rel[argv_bufsize - 1].tlair_end = current_arg_rel_end;
		}
		++argc;
	}
#if TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS
	if (argc > argv_bufsize && !(flags & TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS)) {
		tpp_errno error;
		tpp_size warning_pos = rel_start;
		if (argv_bufsize) {
			warning_pos = p_argv_rel[argv_bufsize - 1].tlair_start;
			/* XXX: "warning_pos" here points at the start of the last in-bounds
			 *      argument, but for optimal effect, this warning should probably
			 *      point at the ","-token *AFTER* the last in-bounds argument... */
		}
		error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, warning_pos),
		                           TPP_W_TOO_MANY_ARGUMENTS,
		                           opt_function_name_for_messages,
		                           (unsigned int)argv_bufsize,
		                           (unsigned int)argc);
		if (TPP_ISERR(error))
			tok = TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_TOO_MANY_ARGUMENTS */
done_err:
	*p_argc = argc;
	if (argc > argv_bufsize)
		argc = argv_bufsize;
	for (i = 0; i < argc; ++i) {
		p_argv[i].tlai_start = tpp_file_rel2ptr(file, p_argv_rel[i].tlair_start);
		p_argv[i].tlai_end   = tpp_file_rel2ptr(file, p_argv_rel[i].tlair_end);
	}
	return tok;
#undef tpp_recursion_cc
#undef tpp_lexer_seek_rparen_keepspace
}


/* Same as above, but always initializes *exactly* "argc" arguments,
 * and automatically emits "TPP_W_TOO_FEW_ARGUMENTS" when fewer were
 * parsed. */
#if TPP_HAVE_LEXER_SEEK_RPAREN_EX
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_token_id TPPCALL
tpp_lexer_seek_rparen_exact_ex(tpp_lexer *tpp_restrict self,
                               tpp_char const **tpp_restrict p_pos,
                               tpp_lexer_arginfo *tpp_restrict p_argv, tpp_size argc,
                               char const *opt_function_name_for_messages,
                               unsigned int flags, tpp_token_id lparen_kind)
#else /* TPP_HAVE_LEXER_SEEK_RPAREN_EX */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_token_id TPPCALL
tpp_lexer_seek_rparen_exact(tpp_lexer *tpp_restrict self,
                            tpp_char const **tpp_restrict p_pos,
                            tpp_lexer_arginfo *tpp_restrict p_argv, tpp_size argc,
                            char const *opt_function_name_for_messages,
                            unsigned int flags);
#endif /* !TPP_HAVE_LEXER_SEEK_RPAREN_EX */
{
	tpp_token_id result;
	tpp_size argc_actual = argc;
	result = tpp_lexer_seek_rparen_ex(self, p_pos, p_argv, &argc_actual,
	                                  opt_function_name_for_messages,
	                                  flags, lparen_kind);
	if (!TPP_TOK_ISERR(result) && argc_actual < argc) {
		tpp_size i;
		tpp_char const *fallback_pos = *p_pos;
#if TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS
		tpp_errno error;
		tpp_token *const token = tpp_lexer_gettoken(self);
		tpp_char const *saved_end = token->tt_end;
		token->tt_end = fallback_pos;
		error = tpp_lexer_warnf_at(self, fallback_pos, TPP_W_TOO_FEW_ARGUMENTS,
		                           opt_function_name_for_messages,
		                           (unsigned int)argc,
		                           (unsigned int)argc_actual);
		token->tt_end = saved_end;
		if (TPP_ISERR(error))
			result = TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */
		for (i = argc_actual; i < argc; ++i) {
			p_argv[i].tlai_start = fallback_pos;
			p_argv[i].tlai_end   = fallback_pos;
		}
	}
	return result;
}
#endif /* TPP_HAVE_LEXER_SEEK_RPAREN */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_SEEK_RPAREN_C */
