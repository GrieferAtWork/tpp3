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
#ifndef GUARD_TPP_LEXER_PARSEEXPR_C
#define GUARD_TPP_LEXER_PARSEEXPR_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "error.h"
#include "expr.h"
#include "lexer.h"
#include "string.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_BUILTIN_PARSEEXPR_HOOK
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_expr(tpp_lexer *tpp_restrict self, tpp_expr_value *result);

#undef TPP_HAVE_TPP_PX_UNARY_SUFFIX
#define TPP_HAVE_TPP_PX_UNARY_SUFFIX TPP_HAVE_BUILTIN_EXPR_STRINGS
#if TPP_HAVE_TPP_PX_UNARY_SUFFIX
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_unary(tpp_lexer *tpp_restrict self, tpp_expr_value *result);
#else /* TPP_HAVE_TPP_PX_UNARY_SUFFIX */
#define tpp_px_unary(self, result) tpp_px_unary_prefix(self, result)
#endif /* !TPP_HAVE_TPP_PX_UNARY_SUFFIX */

TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_forexpr(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
	do {
		result = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(result));
	return result;
}

TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip_forexpr(tpp_lexer *tpp_restrict self, tpp_token_id id) {
	tpp_token_id result = tpp_lexer_skip(self, id);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(result))
		result = tpp_lexer_yield_blocking(self);
	return result;
}

/************************************************************************/
/* LEVEL #0 : UNARY PREFIX                                              */
/************************************************************************/
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_unary_prefix(tpp_lexer *tpp_restrict self, tpp_expr_value *result) {
	tpp_token_id tok = tpp_lexer_gettok(self);
again:
	switch (tpp_lexer_gettok(self)) {
	case TPP_TOK_SPACE:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
	_TPP_CASE_TPP_TOK_ASM_COMMENT
#if !TPP_HAVE_BUILTIN_EXPR_STRINGS && !TPP_HAVE_CPP_ASSERT
	_TPP_CASE_TPP_TOK_SHELL_COMMENT
#endif /* !TPP_HAVE_BUILTIN_EXPR_STRINGS && !TPP_HAVE_CPP_ASSERT */
#if (TPP_HAVE_BUILTIN_EXPR_STRINGS || TPP_HAVE_CPP_ASSERT) && TPP_HAVE_TOK_SHELL_COMMENT
handle_comment:
#endif /* (TPP_HAVE_BUILTIN_EXPR_STRINGS || TPP_HAVE_CPP_ASSERT) && TPP_HAVE_TOK_SHELL_COMMENT */
		/* Skip over whitespace */
		tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		goto again;

	case '(': {
		tpp_errno error;
		tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		error = tpp_px_expr(self, result);
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_skip_forexpr(self, TPP_TOK_OFCHAR(')'));
		return TPP_TOK_ASERR_OR_EOK(tok);
	}	break;

#if (TPP_HAVE_TOK_MINUS_MINUS || \
     TPP_HAVE_TOK_PLUS_PLUS ||   \
     TPP_HAVE_TOK_TILDE_TILDE || \
     TPP_HAVE_TOK_EXCLAIM_EXCLAIM)
#if TPP_HAVE_TOK_MINUS_MINUS
	case TPP_TOK_MINUS_MINUS:
#endif /* TPP_HAVE_TOK_MINUS_MINUS */
#if TPP_HAVE_TOK_PLUS_PLUS
	case TPP_TOK_PLUS_PLUS:
#endif /* TPP_HAVE_TOK_PLUS_PLUS */
#if TPP_HAVE_TOK_TILDE_TILDE
	case TPP_TOK_TILDE_TILDE:
#endif /* TPP_HAVE_TOK_TILDE_TILDE */
#if TPP_HAVE_TOK_EXCLAIM_EXCLAIM
	case TPP_TOK_EXCLAIM_EXCLAIM:
#endif /* TPP_HAVE_TOK_EXCLAIM_EXCLAIM */
	{
		/* Handling for multi-char tokens:  --  ++  ~~  !! */
		tpp_token *const token = tpp_lexer_gettoken(self);
		token->tt_end = token->tt_start + 1;
		token->tt_id = tok = TPP_TOK_OFCHAR(*token->tt_start);
	}	TPP_FALLTHRU
#endif /* ... */
	case '!':
	case '+':
	case '-':
	case '~': {
		tpp_errno error;
		tpp_token_id const op = tok;
		tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (!result)
			return tpp_px_unary(self, NULL);
		error = tpp_px_unary(self, result);
		if (!TPP_ISERR(error)) {
			tpp_expr_value new_result;
			switch (op) {
			case '+': error = tpp_expr_value_pos(self, result, &new_result); break;
			case '-': error = tpp_expr_value_neg(self, result, &new_result); break;
			case '~': error = tpp_expr_value_inv(self, result, &new_result); break;
			case '!': {
				bool b_value;
				error = tpp_expr_value_asbool(self, result, &b_value);
				if (TPP_ISERR(error))
					return error;
				error = tpp_expr_value_init_bool(&new_result, !b_value);
			}	break;
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(result);
			if (!TPP_ISERR(error))
				tpp_expr_value_move(result, &new_result);
		}
		return error;
	}	break;

#if TPP_HAVE_TOK_INT
	TPP_CASE_TPP_TOK_INT {
		tpp_errno error;
		if (result == NULL)
			break;
		error = tpp_lexer_decodeint_expr(self, result);
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_TOK_INT */

#if TPP_HAVE_BUILTIN_EXPR_FLOATS && TPP_HAVE_TOK_FLOAT
	TPP_CASE_TPP_TOK_FLOAT {
		tpp_errno error;
		if (result == NULL)
			break;
		error = tpp_lexer_decodefloat_expr(self, result);
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_FLOATS && TPP_HAVE_TOK_FLOAT */

#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS != 0
	TPP_CASE_TPP_TOK_STRING_SQUOTE
		if (tpp_lexer_has(self, BUILTIN_EXPR_CHARACTER_LITERALS)) {
			if (result)
				return tpp_lexer_parsecharacter_expr(self, result, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
			do {
				tok = tpp_lexer_yield_forexpr(self);
			} while (TPP_TOK_ISSTRING(tok));
			return TPP_TOK_ASERR_OR_EOK(tok);
		}
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		TPP_FALLTHRU
#else /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		break;
#endif /* !TPP_HAVE_BUILTIN_EXPR_STRINGS */
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS != 0 */

#if TPP_HAVE_BUILTIN_EXPR_STRINGS
#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS == 0
	TPP_CASE_TPP_TOK_STRING_SQUOTE
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS == 0 */
	TPP_CASE_TPP_TOK_STRING_DQUOTE
		if (!tpp_lexer_has(self, BUILTIN_EXPR_STRINGS))
			break;
		if (result)
			return tpp_lexer_parsestring_expr(self, result, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		do {
			tok = tpp_lexer_yield_forexpr(self);
		} while (TPP_TOK_ISSTRING(tok));
		return TPP_TOK_ASERR_OR_EOK(tok);
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */


#if TPP_HAVE_BUILTIN_EXPR_STRINGS || TPP_HAVE_CPP_ASSERT
#if TPP_HAVE_TOK_SHELL_COMMENT
	case TPP_TOK_SHELL_COMMENT: {
		tpp_token *const token = tpp_lexer_gettoken(self);
		if (!tpp_lexer_has(self, CPP_ASSERT) &&
		    !tpp_lexer_has(self, BUILTIN_EXPR_STRINGS))
			goto handle_comment;
		/* Convert to '#'-token */
		token->tt_id = TPP_TOK_OFCHAR('#');
		token->tt_end = token->tt_start + 1;
#if TPP_HAVE_TRIGRAPHS
		if (*token->tt_start == '?') {
			token->tt_end += 2;
		} else
#endif /* TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_DIGRAPHS
		if (*token->tt_start == '%') {
			token->tt_end += 1;
		} else
#endif /* TPP_HAVE_DIGRAPHS */
		{
		}
	}	TPP_FALLTHRU
#endif /* !TPP_HAVE_TOK_SHELL_COMMENT */
	case '#':
		/* Preprocessor assertions */
		if (!tpp_lexer_has(self, CPP_ASSERT) &&
		    !tpp_lexer_has(self, BUILTIN_EXPR_STRINGS))
			goto handle_default;
#define WANT_handle_default
		/* XXX: This probably shouldn't expand macros if it ends up being an assertion...
		 *      But if it ends up being a string, then we *must* expand macros...
		 * However: TPP2 also used to expand macros here, so there's that excuse I
		 *          needed. Just follow whatt TPP2 did and always expand macros here! */
		tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);

#if TPP_HAVE_CPP_ASSERT
		if (tpp_lexer_has(self, CPP_ASSERT) && TPP_TOK_ISKEYWORD(tok)) {
			tpp_keyword const *assertion_key = tpp_lexer_gettokenkwd(self);
			tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
			                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (tok == TPP_TOK_OFCHAR('(')) {
				bool is_asserted = false;
				/* Assertion */
				do {
					tok = tpp_lexer_yieldraw_blocking(self);
				} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
				if (TPP_TOK_ISKEYWORD(tok)) {
					tpp_keyword const *assertion_value = tpp_lexer_gettokenkwd(self);
					tpp_keyword_misc const *misc = assertion_key->tk_misc;
					if (misc != NULL)
						is_asserted = tpp_assertions_contains(&misc->tkm_assertions, assertion_value);
					tok = tpp_lexer_yield_forexpr(self);
					if (TPP_TOK_ISERR(tok))
						return TPP_TOK_ASERR(tok);
				} else {
#if TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_ASSERTION
					tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_IDENTIFIER_AFTER_ASSERTION,
					                                  tpp_keyword_getcstr(assertion_key));
					if (TPP_ISERR(error))
						return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_ASSERTION */
				}
				tok = tpp_lexer_skip_forexpr(self, TPP_TOK_OFCHAR(')'));
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
				if (is_asserted && result)
					return tpp_expr_value_init_one(result);
				goto done;
			}
		}
#endif /* TPP_HAVE_CPP_ASSERT */

		/* length-operator for string expressions */
#if TPP_HAVE_BUILTIN_EXPR_STRINGS
		if (tpp_lexer_has(self, BUILTIN_EXPR_STRINGS)) {
			tpp_errno error;
			if (!result)
				return tpp_px_unary(self, NULL);
			error = tpp_px_unary(self, result);
			if (!TPP_ISERR(error)) {
				tpp_expr_value lengthof;
				error = tpp_expr_value_lengthof(self, result, &lengthof);
				tpp_expr_value_fini(result);
				if (!TPP_ISERR(error))
					tpp_expr_value_move(result, &lengthof);
			}
			return error;
		}
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS */
		goto handle_default;
#define WANT_handle_default
#endif /* TPP_HAVE_BUILTIN_EXPR_STRINGS || TPP_HAVE_CPP_ASSERT */


#if TPP_HAVE_BUILTIN_EXPR_DEFINED
	case TPP_KWD_defined: {
		bool is_defined;
		bool has_paren;
		if (!tpp_lexer_has(self, BUILTIN_EXPR_DEFINED))
			goto handle_default;
#define WANT_handle_default
		do {
			tok = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		has_paren = tok == '(';
		if (has_paren) {
			do {
				tok = tpp_lexer_yieldraw_blocking(self);
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		}
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (TPP_TOK_ISKEYWORD(tok)) {
			is_defined = tpp_lexer_getkeyworddefined(self, tpp_lexer_gettokenkwd(self));
			tok = tpp_lexer_yield_forexpr(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		} else {
#if TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED);
			if (TPP_ISERR(error))
				return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_DEFINED */
			is_defined = false;
			/* Be smart about stuff that should still be consumed here */
#if TPP_HAVE_TOK_STRINGLIKE || TPP_HAVE_TOK_INT || TPP_HAVE_TOK_FLOAT
			if (TPP_TOK_ISSTRING(tok) || TPP_TOK_ISINT(tok) || TPP_TOK_ISFLOAT(tok)) {
				tok = tpp_lexer_yield_forexpr(self);
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
			}
#endif /* TPP_HAVE_TOK_STRINGLIKE || TPP_HAVE_TOK_INT || TPP_HAVE_TOK_FLOAT */
		}
		if (has_paren) {
			tok = tpp_lexer_skip_forexpr(self, TPP_TOK_OFCHAR(')'));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
		if (result)
			return tpp_expr_value_init_bool(result, is_defined);
		return TPP_EOK;
	}	break;
#endif /* TPP_HAVE_BUILTIN_EXPR_DEFINED */

#if TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS
	case TPP_KWD_if: {
		tpp_errno error;
		bool is_true;
		if (!tpp_lexer_has(self, BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS))
			goto handle_default;
#define WANT_handle_default
again_handle_if:
		tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('(')); /* Doesn't have to be "tpp_lexer_skip_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		error = tpp_px_expr(self, result);
		if (TPP_ISERR(error))
			return error;
		is_true = false;
		if (result) {
			error = tpp_expr_value_asbool(self, result, &is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
		}
		tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')')); /* Doesn't have to be "tpp_lexer_skip_forexpr" */
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		error = tpp_px_expr(self, is_true ? result : NULL);
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_gettok(self);
		if (tok == TPP_KWD_elif) {
			if (is_true /*&& result*/) {
				tpp_lexer_gettoken(self)->tt_id = TPP_KWD_if;
				error = tpp_px_unary_prefix(self, NULL);
				if (TPP_ISERR(error))
					tpp_expr_value_fini(result);
				return error;
			}
			goto again_handle_if;
		} else if (tok == TPP_KWD_else) {
			tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
			if (TPP_TOK_ISERR(tok)) {
				if (is_true /*&& result*/)
					tpp_expr_value_fini(result);
				return TPP_TOK_ASERR(tok);
			}
			if (is_true /*&& result*/) {
				error = tpp_px_expr(self, NULL);
				if (TPP_ISERR(error))
					tpp_expr_value_fini(result);
				return error;
			}
			return tpp_px_expr(self, result);
		} else {
			if (!is_true && result)
				return tpp_expr_value_init_zero(result);
			return TPP_EOK;
		}
		tpp_unreachable();
	}	break;
#endif /* !TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS */

#ifdef WANT_handle_default
#undef WANT_handle_default
handle_default:
#endif /* WANT_handle_default */
	default: {
		tpp_errno error = TPP_EOK;
		if (TPP_TOK_ISKEYWORD(tok)) {
			unsigned int nesting;
#if TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION
			if (result != NULL)
				error = tpp_lexer_warnf(self, TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION);
#endif /* TPP_HAVE_TPP_W_UNDEFINED_KEYWORD_IN_EXPRESSION */

			/* Be smart if the next token is '(' */
			tok = tpp_lexer_yield_forexpr(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (tok != '(')
				goto done;
			nesting = 0;
			for (;;) {
				tok = tpp_lexer_yield_forexpr(self);
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
				if (tok == '(') {
					++nesting;
				} else if (tok == ')') {
					if (nesting == 0)
						break;
					--nesting;
				} else if (tok == TPP_TOK_EOF) {
					break;
				}
			}
		} else {
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION
			error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION);
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_EXPRESSION */
		}
		if (TPP_ISERR(error))
			return error;
	}	break;

	}

	/* Skip over token and set result to "0" */
	tok = tpp_lexer_yield_forexpr(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
done:
	if (result == NULL)
		return TPP_EOK;
	return tpp_expr_value_init_zero(result);
}

#define TPP_DEFINE_PX_PARSER(tpp_px_prev, tpp_px_this, tpp_px_this_suffix, TPP_TEST_PX_THIS_SUFFIX) \
	static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL                              \
	tpp_px_this(tpp_lexer *tpp_restrict self, tpp_expr_value *result) {                             \
		tpp_errno error = tpp_px_prev(self, result);                                                \
		if (!TPP_ISERR(error) && TPP_TEST_PX_THIS_SUFFIX(tpp_lexer_gettok(self)))                   \
			error = tpp_px_this_suffix(self, result);                                               \
		return error;                                                                               \
	}


/************************************************************************/
/* LEVEL #1 : UNARY SUFFIX                                              */
/************************************************************************/
#if TPP_HAVE_TPP_PX_UNARY_SUFFIX
#define TPP_CASE_PX_UNARY_SUFFIX      case '[':
#define TPP_TEST_PX_UNARY_SUFFIX(tok) ((tok) == '[')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_unary_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_token_id tok;
	tpp_assert(TPP_TEST_PX_UNARY_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok))
			goto err_r_tok;
		if (tok == ':') {
			tok = tpp_lexer_yield_forexpr(self);
			if (TPP_TOK_ISERR(tok))
				goto err_r_tok;
			if (tok == ']') {
				/* Full range */
				if (result) {
					tpp_expr_value new_result;
					error = tpp_expr_value_getrange(self, result, NULL, NULL, &new_result);
					tpp_expr_value_fini(result);
					if (TPP_ISERR(error))
						return error;
					tpp_expr_value_move(result, &new_result);
				}
			} else {
				/* hi-only range */
				if (result) {
					tpp_expr_value hi, new_result;
					error = tpp_px_expr(self, &hi);
					if (TPP_ISERR(error))
						goto err_r;
					error = tpp_expr_value_getrange(self, result, NULL, &hi, &new_result);
					tpp_expr_value_fini(&hi);
					tpp_expr_value_fini(result);
					if (TPP_ISERR(error))
						return error;
					tpp_expr_value_move(result, &new_result);
				} else {
					error = tpp_px_expr(self, NULL);
					if (TPP_ISERR(error))
						return error;
				}
			}
		} else if (result) {
			tpp_expr_value index, new_result;
			error = tpp_px_expr(self, &index);
			if (TPP_ISERR(error))
				goto err_r_tok;
			tok = tpp_lexer_gettok(self);
			while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
				tok = tpp_lexer_yield_blocking(self);
				if (TPP_TOK_ISERR(tok)) {
err_r_tok_index:
					tpp_expr_value_fini(&index);
					goto err_r_tok;
				}
			}
			if (tok == ':') {
				tok = tpp_lexer_yield_forexpr(self);
				if (TPP_TOK_ISERR(tok))
					goto err_r_tok_index;
				if (tok == ']') {
					error = tpp_expr_value_getrange(self, result, &index, NULL, &new_result);
				} else {
					tpp_expr_value hi;
					error = tpp_px_expr(self, &hi);
					if (TPP_ISERR(error)) {
						tpp_expr_value_fini(&index);
						goto err_r;
					}
					error = tpp_expr_value_getrange(self, result, &index, &hi, &new_result);
					tpp_expr_value_fini(&hi);
				}
			} else {
				error = tpp_expr_value_getindex(self, result, &index, &new_result);
			}
			tpp_expr_value_fini(&index);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_expr(self, NULL);
			if (TPP_ISERR(error))
				return error;
			tok = tpp_lexer_gettok(self);
			while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
				tok = tpp_lexer_yield_blocking(self);
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
			}
			if (tok == ':') {
				tok = tpp_lexer_yield_forexpr(self);
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
			}
		}
		tok = tpp_lexer_gettok(self);
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			tok = tpp_lexer_yield_blocking(self);
			if (TPP_TOK_ISERR(tok))
				goto err_r_tok;
		}
		tok = tpp_lexer_skip_forexpr(self, TPP_TOK_OFCHAR(']'));
		if (TPP_TOK_ISERR(tok))
			goto err_r_tok;
	} while (TPP_TEST_PX_UNARY_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r_tok:
	error = TPP_TOK_ASERR(tok);
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}

TPP_DEFINE_PX_PARSER(tpp_px_unary_prefix, tpp_px_unary, tpp_px_unary_suffix, TPP_TEST_PX_UNARY_SUFFIX)
#endif /* TPP_HAVE_TPP_PX_UNARY_SUFFIX */



/************************************************************************/
/* LEVEL #2 : PRODUCT                                                   */
/************************************************************************/
#define TPP_CASE_PX_PROD_SUFFIX      case '*': case '/': case '%':
#define TPP_TEST_PX_PROD_SUFFIX(tok) ((tok) == '*' || (tok) == '/' || (tok) == '%')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_prod_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_PROD_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_unary(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
			case '*': error = tpp_expr_value_mul(self, result, &rhs, &new_result); break;
			case '/': error = tpp_expr_value_div(self, result, &rhs, &new_result); break;
			case '%': error = tpp_expr_value_mod(self, result, &rhs, &new_result); break;
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_unary(self, NULL);
			if (TPP_ISERR(error))
				goto err_r;
		}
	} while (TPP_TEST_PX_PROD_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_unary, tpp_px_prod, tpp_px_prod_suffix, TPP_TEST_PX_PROD_SUFFIX)


/************************************************************************/
/* LEVEL #3 : SUM                                                       */
/************************************************************************/
#define TPP_CASE_PX_SUM_SUFFIX      case '+': case '-':
#define TPP_TEST_PX_SUM_SUFFIX(tok) ((tok) == '+' || (tok) == '-')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_sum_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_SUM_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_prod(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
			case '+': error = tpp_expr_value_add(self, result, &rhs, &new_result); break;
			case '-': error = tpp_expr_value_sub(self, result, &rhs, &new_result); break;
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_prod(self, NULL);
			if (TPP_ISERR(error))
				goto err_r;
		}
	} while (TPP_TEST_PX_SUM_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_prod, tpp_px_sum, tpp_px_sum_suffix, TPP_TEST_PX_SUM_SUFFIX)


/************************************************************************/
/* LEVEL #4 : SHIFT                                                     */
/************************************************************************/
#undef TPP_HAVE_PX_SHIFT_SUFFIX
#if TPP_HAVE_TOK_LANGLE_LANGLE || TPP_HAVE_TOK_RANGLE_RANGLE
#define TPP_HAVE_PX_SHIFT_SUFFIX 1
#if TPP_HAVE_TOK_LANGLE_LANGLE && TPP_HAVE_TOK_RANGLE_RANGLE
#define TPP_CASE_PX_SHIFT_SUFFIX \
	case TPP_TOK_LANGLE_LANGLE:  \
	case TPP_TOK_RANGLE_RANGLE:
#define TPP_TEST_PX_SHIFT_SUFFIX(tok) ((tok) == TPP_TOK_LANGLE_LANGLE || (tok) == TPP_TOK_RANGLE_RANGLE)
#elif TPP_HAVE_TOK_LANGLE_LANGLE
#define TPP_CASE_PX_SHIFT_SUFFIX      case TPP_TOK_LANGLE_LANGLE:
#define TPP_TEST_PX_SHIFT_SUFFIX(tok) ((tok) == TPP_TOK_LANGLE_LANGLE)
#else /* ... */
#define TPP_CASE_PX_SHIFT_SUFFIX      case TPP_TOK_RANGLE_RANGLE:
#define TPP_TEST_PX_SHIFT_SUFFIX(tok) ((tok) == TPP_TOK_RANGLE_RANGLE)
#endif /* !... */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_shift_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_SHIFT_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_sum(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
#if TPP_HAVE_TOK_LANGLE_LANGLE
			case TPP_TOK_LANGLE_LANGLE: error = tpp_expr_value_shl(self, result, &rhs, &new_result); break;
#endif /* TPP_HAVE_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TOK_RANGLE_RANGLE
			case TPP_TOK_RANGLE_RANGLE: error = tpp_expr_value_shr(self, result, &rhs, &new_result); break;
#endif /* TPP_HAVE_TOK_RANGLE_RANGLE */
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_sum(self, NULL);
			if (TPP_ISERR(error))
				return error;
		}
	} while (TPP_TEST_PX_SHIFT_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_sum, tpp_px_shift, tpp_px_shift_suffix, TPP_TEST_PX_SHIFT_SUFFIX)
#else /* TPP_HAVE_TOK_LANGLE_LANGLE || TPP_HAVE_TOK_RANGLE_RANGLE */
#define TPP_HAVE_PX_SHIFT_SUFFIX          0
#define TPP_CASE_PX_SHIFT_SUFFIX          /* nothing */
#define TPP_TEST_PX_SHIFT_SUFFIX(tok)     0
#define tpp_px_shift_suffix(self, result) TPP_EOK
#define tpp_px_shift(self, result)        tpp_px_sum(self, result)
#endif /* !TPP_HAVE_TOK_LANGLE_LANGLE && !TPP_HAVE_TOK_RANGLE_RANGLE */


/************************************************************************/
/* LEVEL #5 : CMP                                                       */
/************************************************************************/
#if TPP_HAVE_TOK_LANGLE_EQUAL && TPP_HAVE_TOK_RANGLE_EQUAL
#define TPP_CASE_PX_CMP_SUFFIX_EXTRA \
	case TPP_TOK_LANGLE_EQUAL:       \
	case TPP_TOK_RANGLE_EQUAL:
#define TPP_TEST_PX_CMP_SUFFIX_EXTRA(tok) ((tok) == TPP_TOK_LANGLE_EQUAL || (tok) == TPP_TOK_RANGLE_EQUAL)
#elif TPP_HAVE_TOK_LANGLE_EQUAL
#define TPP_CASE_PX_CMP_SUFFIX_EXTRA      case TPP_TOK_LANGLE_EQUAL:
#define TPP_TEST_PX_CMP_SUFFIX_EXTRA(tok) ((tok) == TPP_TOK_LANGLE_EQUAL)
#else /* ... */
#define TPP_CASE_PX_CMP_SUFFIX_EXTRA      case TPP_TOK_RANGLE_EQUAL:
#define TPP_TEST_PX_CMP_SUFFIX_EXTRA(tok) ((tok) == TPP_TOK_RANGLE_EQUAL)
#endif /* !... */
#define TPP_CASE_PX_CMP_SUFFIX      case '<': case '>': TPP_CASE_PX_CMP_SUFFIX_EXTRA
#define TPP_TEST_PX_CMP_SUFFIX(tok) ((tok) == '<' || (tok) == '>' || TPP_TEST_PX_CMP_SUFFIX_EXTRA(tok))
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_cmp_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_CMP_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			bool cmp_result;
			tpp_expr_value rhs;
			error = tpp_px_shift(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
			case '<': error = tpp_expr_value_cmp_lo(self, result, &rhs, &cmp_result); break;
			case '>': error = tpp_expr_value_cmp_gr(self, result, &rhs, &cmp_result); break;
#if TPP_HAVE_TOK_LANGLE_EQUAL
			case TPP_TOK_LANGLE_EQUAL: error = tpp_expr_value_cmp_le(self, result, &rhs, &cmp_result); break;
#endif /* TPP_HAVE_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TOK_RANGLE_EQUAL
			case TPP_TOK_RANGLE_EQUAL: error = tpp_expr_value_cmp_ge(self, result, &rhs, &cmp_result); break;
#endif /* TPP_HAVE_TOK_RANGLE_EQUAL */
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_expr_value_init_bool(result, cmp_result);
		} else {
			error = tpp_px_shift(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_CMP_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_shift, tpp_px_cmp, tpp_px_cmp_suffix, TPP_TEST_PX_CMP_SUFFIX)


/************************************************************************/
/* LEVEL #6 : CMPEQ                                                     */
/************************************************************************/
#undef TPP_HAVE_PX_CMPEQ_SUFFIX
#if TPP_HAVE_TOK_EQUAL_EQUAL || TPP_HAVE_TOK_EXCLAIM_EQUAL
#define TPP_HAVE_PX_CMPEQ_SUFFIX 1
#if TPP_HAVE_TOK_EQUAL_EQUAL && TPP_HAVE_TOK_EXCLAIM_EQUAL
#define TPP_CASE_PX_CMPEQ_SUFFIX \
	case TPP_TOK_EQUAL_EQUAL:    \
	case TPP_TOK_EXCLAIM_EQUAL:
#define TPP_TEST_PX_CMPEQ_SUFFIX(tok) ((tok) == TPP_TOK_EQUAL_EQUAL || (tok) == TPP_TOK_EXCLAIM_EQUAL)
#elif TPP_HAVE_TOK_EQUAL_EQUAL
#define TPP_CASE_PX_CMPEQ_SUFFIX      case TPP_TOK_EQUAL_EQUAL:
#define TPP_TEST_PX_CMPEQ_SUFFIX(tok) ((tok) == TPP_TOK_EQUAL_EQUAL)
#else /* ... */
#define TPP_CASE_PX_CMPEQ_SUFFIX      case TPP_TOK_EXCLAIM_EQUAL:
#define TPP_TEST_PX_CMPEQ_SUFFIX(tok) ((tok) == TPP_TOK_EXCLAIM_EQUAL)
#endif /* !... */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_cmpeq_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_CMPEQ_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id const what = tpp_lexer_gettok(self);
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			bool cmp_result;
			tpp_expr_value rhs;
			error = tpp_px_cmp(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			switch (what) {
#if TPP_HAVE_TOK_EQUAL_EQUAL
			case TPP_TOK_EQUAL_EQUAL: error = tpp_expr_value_cmp_eq(self, result, &rhs, &cmp_result); break;
#endif /* TPP_HAVE_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TOK_EXCLAIM_EQUAL
			case TPP_TOK_EXCLAIM_EQUAL: error = tpp_expr_value_cmp_ne(self, result, &rhs, &cmp_result); break;
#endif /* TPP_HAVE_TOK_EXCLAIM_EQUAL */
			default: tpp_unreachable();
			}
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_expr_value_init_bool(result, cmp_result);
		} else {
			error = tpp_px_cmp(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_CMPEQ_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_cmp, tpp_px_cmpeq, tpp_px_cmpeq_suffix, TPP_TEST_PX_CMPEQ_SUFFIX)
#else /* TPP_HAVE_TOK_EQUAL_EQUAL || TPP_HAVE_TOK_EXCLAIM_EQUAL */
#define TPP_HAVE_PX_CMPEQ_SUFFIX          0
#define TPP_CASE_PX_CMPEQ_SUFFIX          /* nothing */
#define TPP_TEST_PX_CMPEQ_SUFFIX(tok)     0
#define tpp_px_cmpeq_suffix(self, result) TPP_EOK
#define tpp_px_cmpeq(self, result)        tpp_px_cmp(self, result)
#endif /* !TPP_HAVE_TOK_EQUAL_EQUAL && !TPP_HAVE_TOK_EXCLAIM_EQUAL */


/************************************************************************/
/* LEVEL #7 : AND                                                       */
/************************************************************************/
#define TPP_CASE_PX_AND_SUFFIX      case '&':
#define TPP_TEST_PX_AND_SUFFIX(tok) ((tok) == '&')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_and_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_AND_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_cmpeq(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			error = tpp_expr_value_and(self, result, &rhs, &new_result);
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_cmpeq(self, NULL);
			if (TPP_ISERR(error))
				return error;
		}
	} while (TPP_TEST_PX_AND_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_cmpeq, tpp_px_and, tpp_px_and_suffix, TPP_TEST_PX_AND_SUFFIX)


/************************************************************************/
/* LEVEL #8 : XOR                                                       */
/************************************************************************/
#define TPP_CASE_PX_XOR_SUFFIX      case '^':
#define TPP_TEST_PX_XOR_SUFFIX(tok) ((tok) == '^')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_xor_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_XOR_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_and(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			error = tpp_expr_value_xor(self, result, &rhs, &new_result);
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_and(self, NULL);
			if (TPP_ISERR(error))
				return error;
		}
	} while (TPP_TEST_PX_XOR_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_and, tpp_px_xor, tpp_px_xor_suffix, TPP_TEST_PX_XOR_SUFFIX)


/************************************************************************/
/* LEVEL #9 : OR                                                        */
/************************************************************************/
#define TPP_CASE_PX_OR_SUFFIX      case '|':
#define TPP_TEST_PX_OR_SUFFIX(tok) ((tok) == '|')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_or_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_assert(TPP_TEST_PX_OR_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_r;
		}
		if (result) {
			tpp_expr_value rhs, new_result;
			error = tpp_px_xor(self, &rhs);
			if (TPP_ISERR(error))
				goto err_r;
			error = tpp_expr_value_or(self, result, &rhs, &new_result);
			tpp_expr_value_fini(&rhs);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			tpp_expr_value_move(result, &new_result);
		} else {
			error = tpp_px_xor(self, NULL);
			if (TPP_ISERR(error))
				return error;
		}
	} while (TPP_TEST_PX_OR_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
err_r:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}
TPP_DEFINE_PX_PARSER(tpp_px_xor, tpp_px_or, tpp_px_or_suffix, TPP_TEST_PX_OR_SUFFIX)


/************************************************************************/
/* LEVEL #10 : LAND                                                     */
/************************************************************************/
#undef TPP_HAVE_PX_LAND_SUFFIX
#if TPP_HAVE_TOK_AMP_AMP
#define TPP_HAVE_PX_LAND_SUFFIX      1
#define TPP_CASE_PX_LAND_SUFFIX      case TPP_TOK_AMP_AMP:
#define TPP_TEST_PX_LAND_SUFFIX(tok) ((tok) == TPP_TOK_AMP_AMP)
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_land_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_assert(TPP_TEST_PX_LAND_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_errno error;
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			if (result)
				tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		if (result) {
			bool is_true;
			error = tpp_expr_value_asbool(self, result, &is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_px_or(self, is_true ? result : NULL);
			if (TPP_ISERR(error))
				return error;
			if (is_true) {
				error = tpp_expr_value_asbool(self, result, &is_true);
				tpp_expr_value_fini(result);
				if (TPP_ISERR(error))
					return error;
			}
			error = tpp_expr_value_init_bool(result, is_true);
		} else {
			error = tpp_px_or(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_LAND_SUFFIX(tpp_lexer_gettok(self)));
#if TPP_HAVE_TPP_W_PAREN_AROUND_LAND && TPP_HAVE_TOK_PIPE_PIPE
	if (tpp_lexer_gettok(self) == TPP_TOK_PIPE_PIPE) {
		tpp_errno error = tpp_lexer_warnf(self, TPP_W_PAREN_AROUND_LAND);
		if (TPP_ISERR(error)) {
			if (result)
				tpp_expr_value_fini(result);
			return error;
		}
	}
#endif /* TPP_HAVE_TPP_W_PAREN_AROUND_LAND && TPP_HAVE_TOK_PIPE_PIPE */
	return TPP_EOK;
}
TPP_DEFINE_PX_PARSER(tpp_px_or, tpp_px_land, tpp_px_land_suffix, TPP_TEST_PX_LAND_SUFFIX)
#else /* TPP_HAVE_TOK_AMP_AMP */
#define TPP_HAVE_PX_LAND_SUFFIX          0
#define TPP_CASE_PX_LAND_SUFFIX          /* nothing */
#define TPP_TEST_PX_LAND_SUFFIX(tok)     0
#define tpp_px_land_suffix(self, result) TPP_EOK
#define tpp_px_land(self, result)        tpp_px_or(self, result)
#endif /* !TPP_HAVE_TOK_AMP_AMP */


/************************************************************************/
/* LEVEL #11 : LXOR                                                     */
/************************************************************************/
#undef TPP_HAVE_PX_LXOR_SUFFIX
#if TPP_HAVE_TOK_HAT_HAT
#define TPP_HAVE_PX_LXOR_SUFFIX      1
#define TPP_CASE_PX_LXOR_SUFFIX      case TPP_TOK_HAT_HAT:
#define TPP_TEST_PX_LXOR_SUFFIX(tok) ((tok) == TPP_TOK_HAT_HAT)
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_lxor_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_assert(TPP_TEST_PX_LXOR_SUFFIX(tpp_lexer_gettok(self)));
	if (!tpp_lexer_has(self, BUILTIN_EXPR_LOGICAL_XOR))
		return TPP_EOK;
	do {
		tpp_errno error;
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			if (result)
				tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		if (result) {
			bool lhs_is_true, rhs_is_true;
			error = tpp_expr_value_asbool(self, result, &lhs_is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_px_land(self, result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_expr_value_asbool(self, result, &rhs_is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_expr_value_init_bool(result, lhs_is_true ^ rhs_is_true);
		} else {
			error = tpp_px_land(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_LXOR_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
}
TPP_DEFINE_PX_PARSER(tpp_px_land, tpp_px_lxor, tpp_px_lxor_suffix, TPP_TEST_PX_LXOR_SUFFIX)
#else /* TPP_HAVE_TOK_HAT_HAT */
#define TPP_HAVE_PX_LXOR_SUFFIX          0
#define TPP_CASE_PX_LXOR_SUFFIX          /* nothing */
#define TPP_TEST_PX_LXOR_SUFFIX(tok)     0
#define tpp_px_lxor_suffix(self, result) TPP_EOK
#define tpp_px_lxor(self, result)        tpp_px_land(self, result)
#endif /* !TPP_HAVE_TOK_HAT_HAT */


/************************************************************************/
/* LEVEL #12 : LOR                                                      */
/************************************************************************/
#undef TPP_HAVE_PX_LOR_SUFFIX
#if TPP_HAVE_TOK_PIPE_PIPE
#define TPP_HAVE_PX_LOR_SUFFIX      1
#define TPP_CASE_PX_LOR_SUFFIX      case TPP_TOK_PIPE_PIPE:
#define TPP_TEST_PX_LOR_SUFFIX(tok) ((tok) == TPP_TOK_PIPE_PIPE)
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_lor_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_assert(TPP_TEST_PX_LOR_SUFFIX(tpp_lexer_gettok(self)));
	do {
		tpp_errno error;
		tpp_token_id tok = tpp_lexer_yield_forexpr(self);
		if (TPP_TOK_ISERR(tok)) {
			if (result)
				tpp_expr_value_fini(result);
			return TPP_TOK_ASERR(tok);
		}
		if (result) {
			bool is_true;
			error = tpp_expr_value_asbool(self, result, &is_true);
			tpp_expr_value_fini(result);
			if (TPP_ISERR(error))
				return error;
			error = tpp_px_lxor(self, is_true ? NULL : result);
			if (TPP_ISERR(error))
				return error;
			if (!is_true) {
				error = tpp_expr_value_asbool(self, result, &is_true);
				tpp_expr_value_fini(result);
				if (TPP_ISERR(error))
					return error;
			}
			error = tpp_expr_value_init_bool(result, is_true);
		} else {
			error = tpp_px_lxor(self, NULL);
		}
		if (TPP_ISERR(error))
			return error;
	} while (TPP_TEST_PX_LOR_SUFFIX(tpp_lexer_gettok(self)));
	return TPP_EOK;
}
TPP_DEFINE_PX_PARSER(tpp_px_lxor, tpp_px_lor, tpp_px_lor_suffix, TPP_TEST_PX_LOR_SUFFIX)
#else /* TPP_HAVE_TOK_PIPE_PIPE */
#define TPP_HAVE_PX_LOR_SUFFIX          0
#define TPP_CASE_PX_LOR_SUFFIX          /* nothing */
#define TPP_TEST_PX_LOR_SUFFIX(tok)     0
#define tpp_px_lor_suffix(self, result) TPP_EOK
#define tpp_px_lor(self, result)        tpp_px_lxor(self, result)
#endif /* !TPP_HAVE_TOK_PIPE_PIPE */


/************************************************************************/
/* LEVEL #13 : QUESTION                                                 */
/************************************************************************/
#define TPP_CASE_PX_QUESTION_SUFFIX      case '?':
#define TPP_TEST_PX_QUESTION_SUFFIX(tok) ((tok) == '?')
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_question_suffix(tpp_lexer *tpp_restrict self, /*opt:[in|out]*/ tpp_expr_value *result) {
	tpp_errno error;
	tpp_token_id tok;
	tpp_assert(TPP_TEST_PX_QUESTION_SUFFIX(tpp_lexer_gettok(self)));
	tok = tpp_lexer_yield_forexpr(self);
	if (TPP_TOK_ISERR(tok))
		goto err_result_tok;
	if (result) {
		bool cond_is_true;
		error = tpp_expr_value_asbool(self, result, &cond_is_true);
		if (TPP_ISERR(error))
			goto err_result;
		tok = tpp_lexer_gettok(self);
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			tok = tpp_lexer_yield_blocking(self);
			if (TPP_TOK_ISERR(tok))
				goto err_result_tok;
		}
#if TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT
		if (tok == ':' && tpp_lexer_has(self, BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)) {
			tok = tpp_lexer_yield_forexpr(self);
			if (TPP_TOK_ISERR(tok))
				goto err_result_tok;
			if (cond_is_true) {
				error = tpp_px_expr(self, NULL);
				if (TPP_ISERR(error))
					tpp_expr_value_fini(result);
				return error;
			}
			tpp_expr_value_fini(result);
			return tpp_px_expr(self, result);
		} else
#endif /* TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT */
		{
			tpp_expr_value_fini(result);
			error = tpp_px_lor(self, cond_is_true ? result : NULL);
			if (TPP_ISERR(error))
				return error;
			tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(':')); /* Doesn't have to be "tpp_lexer_skip_forexpr" */
			if (TPP_TOK_ISERR(tok)) {
				if (cond_is_true)
					tpp_expr_value_fini(result);
				return TPP_TOK_ASERR(tok);
			}
			if (cond_is_true) {
				error = tpp_px_expr(self, NULL);
				if (TPP_ISERR(error))
					tpp_expr_value_fini(result);
				return error;
			}
			return tpp_px_expr(self, result);
		}
	} else {
		if (tok != ':') {
			error = tpp_px_lor(self, NULL);
			if (TPP_ISERR(error))
				return error;
			tok = tpp_lexer_gettok(self);
		}
		if (tok == ':') {
			tok = tpp_lexer_yield_forexpr(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			return tpp_px_expr(self, NULL);
		}
	}
	return TPP_EOK;
err_result_tok:
	error = TPP_TOK_ASERR(tok);
err_result:
	if (result)
		tpp_expr_value_fini(result);
	return error;
}



#undef TPP_DEFINE_PX_PARSER



static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_px_expr(tpp_lexer *tpp_restrict self, tpp_expr_value *result) {
	tpp_token_id tok;
	tpp_errno error;

	/* Evaluate expression prefix. */
	error = tpp_px_unary_prefix(self, result);
	if (TPP_ISERR(error))
		return error;

	/* Evaluate expression suffix. */
	tok = tpp_lexer_gettok(self);
	switch (tok) {

#if TPP_HAVE_TPP_PX_UNARY_SUFFIX
		/*if (TPP_TEST_PX_UNARY_SUFFIX(tok))*/ {
	TPP_CASE_PX_UNARY_SUFFIX
			error = tpp_px_unary_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
		if (TPP_TEST_PX_PROD_SUFFIX(tok))
#endif /* TPP_HAVE_TPP_PX_UNARY_SUFFIX */
		{
	TPP_CASE_PX_PROD_SUFFIX
			error = tpp_px_prod_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
		if (TPP_TEST_PX_SUM_SUFFIX(tok)) {
	TPP_CASE_PX_SUM_SUFFIX
			error = tpp_px_sum_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#if TPP_HAVE_PX_SHIFT_SUFFIX
		if (TPP_TEST_PX_SHIFT_SUFFIX(tok)) {
	TPP_CASE_PX_SHIFT_SUFFIX
			error = tpp_px_shift_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_SHIFT_SUFFIX */
		if (TPP_TEST_PX_CMP_SUFFIX(tok)) {
	TPP_CASE_PX_CMP_SUFFIX
			error = tpp_px_cmp_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#if TPP_HAVE_PX_CMPEQ_SUFFIX
		if (TPP_TEST_PX_CMPEQ_SUFFIX(tok)) {
	TPP_CASE_PX_CMPEQ_SUFFIX
			error = tpp_px_cmpeq_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_CMPEQ_SUFFIX */
		if (TPP_TEST_PX_AND_SUFFIX(tok)) {
	TPP_CASE_PX_AND_SUFFIX
			error = tpp_px_and_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
		if (TPP_TEST_PX_XOR_SUFFIX(tok)) {
	TPP_CASE_PX_XOR_SUFFIX
			error = tpp_px_xor_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
		if (TPP_TEST_PX_OR_SUFFIX(tok)) {
	TPP_CASE_PX_OR_SUFFIX
			error = tpp_px_or_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#if TPP_HAVE_PX_LAND_SUFFIX
		if (TPP_TEST_PX_LAND_SUFFIX(tok)) {
	TPP_CASE_PX_LAND_SUFFIX
			error = tpp_px_land_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_LAND_SUFFIX */
#if TPP_HAVE_PX_LXOR_SUFFIX
		if (TPP_TEST_PX_LXOR_SUFFIX(tok)) {
	TPP_CASE_PX_LXOR_SUFFIX
			error = tpp_px_lxor_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_LXOR_SUFFIX */
#if TPP_HAVE_PX_LOR_SUFFIX
		if (TPP_TEST_PX_LOR_SUFFIX(tok)) {
	TPP_CASE_PX_LOR_SUFFIX
			error = tpp_px_lor_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
		}
#endif /* TPP_HAVE_PX_LOR_SUFFIX */
		if (TPP_TEST_PX_QUESTION_SUFFIX(tok)) {
	TPP_CASE_PX_QUESTION_SUFFIX
#if 1
			return tpp_px_question_suffix(self, result);
#else
			error = tpp_px_question_suffix(self, result);
			if (TPP_ISERR(error))
				break;
			tok = tpp_lexer_gettok(self);
#endif
		}
		break;
	default: break;
	}
	return error;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_lexer_builtin_parseexpr(struct tpp_lexer *tpp_restrict self,
                             tpp_expr_value *tpp_restrict result) {
	tpp_token_id tok = tpp_lexer_yield_blocking(self); /* Doesn't have to be "tpp_lexer_yield_forexpr" */
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	return tpp_px_expr(self, result);
}
#endif /* TPP_HAVE_BUILTIN_PARSEEXPR_HOOK */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_PARSEEXPR_C */
