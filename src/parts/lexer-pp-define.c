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
#ifndef GUARD_TPP_LEXER_PP_DEFINE_C
#define GUARD_TPP_LEXER_PP_DEFINE_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "features.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "macro.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_DIRECTIVES && TPP_HAVE_CPP_DEFINE

typedef struct tpp_macro_builder {
	tpp_size            mab_argc;        /* # of arguments */
	tpp_size            mab_arga;        /* Allocated # of arguments */
	tpp_macro_argument *mab_argv;        /* [0..mab_argc] Argument list */
#if TPP_HAVE_MACRO_FLAGS
	tpp_macro_flag      mab_flags;       /* Macro flags */
#define _tpp_macro_builder_init_flags(self) , (self)->mab_flags = TPP_MACRO_FLAG_NORMAL
#else /* TPP_HAVE_MACRO_FLAGS */
#define _tpp_macro_builder_init_flags(self) /* nothing */
#endif /* !TPP_HAVE_MACRO_FLAGS */
	tpp_ssize           mab_skiptotal;   /* # of characters skipped during expansion. */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
	tpp_size            mab_n_vaopt;     /* Amount of extra bytes inserted when varargs are given (if: tpp_lexer_seekpp_rparen:OUT(*p_argc) > tmf_argc). */
#define _tpp_macro_builder_init_n_vaopt(self) , (self)->mab_n_vaopt = 0
#else /* TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#define _tpp_macro_builder_init_n_vaopt(self) /* nothing */
#endif /* !TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	tpp_size            mab_n_vanargs;   /* Amount of times `__VA_NARGS__' is used in `tmf_expand'. */
#define _tpp_macro_builder_init_n_vanargs(self) , (self)->mab_n_vanargs = 0
#else /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
#define _tpp_macro_builder_init_n_vanargs(self) /* nothing */
#endif /* !TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
	tpp_size            mab_tmf_expandc; /* Used buffer size in `mab_macro->tm_data.tmd_func.tmf_expand' */
	tpp_size            mab_tmf_expanda; /* Allocated buffer size in `mab_macro->tm_data.tmd_func.tmf_expand' */
	tpp_macro          *mab_macro;       /* [0..*] The macro being built */
} tpp_macro_builder;

#define tpp_macro_builder_init(self)                \
	(void)((self)->mab_argv = NULL,                 \
	       (self)->mab_argc = 0,                    \
	       (self)->mab_arga = 0                     \
	       _tpp_macro_builder_init_flags(self),     \
	       (self)->mab_skiptotal = 0                \
	       _tpp_macro_builder_init_n_vaopt(self)    \
	       _tpp_macro_builder_init_n_vanargs(self), \
	       (self)->mab_tmf_expandc = 0,             \
	       (self)->mab_tmf_expanda = 0,             \
	       (self)->mab_macro = NULL)
#define tpp_macro_builder_fini(self) \
	(tpp_free((self)->mab_argv),     \
	 _tpp_macro_free((self)->mab_macro))

#ifdef __OPTIMIZE_SIZE__
#define tpp_macro_builder_truncate_argv(self) (void)0
#else /* __OPTIMIZE_SIZE__ */
static TPP_NONNULL((1)) void TPPCALL
tpp_macro_builder_truncate_argv(tpp_macro_builder *tpp_restrict self) {
	if (self->mab_argc < self->mab_arga) {
		/* Truncate unused memory */
		tpp_macro_argument *new_macro_argv;
		new_macro_argv = (tpp_macro_argument *)tpp_tryrealloc(self->mab_argv,
		                                                      self->mab_argc *
		                                                      sizeof(tpp_macro_argument));
		if tpp_likely(new_macro_argv)
			self->mab_argv = new_macro_argv;
	}
}
#endif /* !__OPTIMIZE_SIZE__ */

/* Check if "name" identifies a known argument. If so: return it. Otherwise, return "NULL" */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_argument *TPPCALL
tpp_macro_builder_getargument(tpp_macro_builder const *tpp_restrict self, tpp_token_id name) {
	tpp_size i;
	tpp_macro_argument *argv = self->mab_argv;
	for (i = 0; i < self->mab_argc; ++i) {
		if (argv[i].tma_id == name)
			return &argv[i];
	}
	return NULL;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_argument *TPPCALL
tpp_macro_builder_newargument(tpp_macro_builder *tpp_restrict self) {
	tpp_assert(self->mab_argc <= self->mab_arga);
	if (self->mab_argc >= self->mab_arga) {
		tpp_macro_argument *new_macro_argv;
		tpp_size new_macro_arga = self->mab_arga * 2;
		tpp_size min_macro_arga = self->mab_argc + 1;
		if (new_macro_arga < 3)
			new_macro_arga = 3;
		if (new_macro_arga < min_macro_arga)
			new_macro_arga = min_macro_arga;
		new_macro_argv = (tpp_macro_argument *)tpp_tryrealloc(self->mab_argv,
		                                                      new_macro_arga *
		                                                      sizeof(tpp_macro_argument));
		if tpp_unlikely(!new_macro_argv) {
			new_macro_arga = min_macro_arga;
			new_macro_argv = (tpp_macro_argument *)tpp_realloc(self->mab_argv,
			                                                   new_macro_arga *
			                                                   sizeof(tpp_macro_argument));
			if tpp_unlikely(!new_macro_argv)
				return NULL;
		}
		self->mab_argv = new_macro_argv;
		self->mab_arga = new_macro_arga;
	}
	tpp_assert(self->mab_argc < self->mab_arga);
	return &self->mab_argv[self->mab_argc++];
}


static TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_lexer_is_rparen_token(tpp_lexer *tpp_restrict self,
                          tpp_char const **p_pos,
                          tpp_token_id lparen_token) {
	(void)lparen_token;
	switch (tpp_lexer_gettoken(self)->tt_id) {
	case ')':
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
		if (lparen_token != '(')
			break;
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */
		return true;
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
	case ']':
		if (lparen_token == '[')
			return true;
		break;
	case '}':
		if (lparen_token == '{')
			return true;
		break;
	case '>':
		if (lparen_token == '<')
			return true;
		break;

#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||              \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||               \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||        \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE ||       \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL || \
     TPP_HAVE_TPP_TOK_RANGLE_LANGLE)
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
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	case TPP_TOK_RANGLE_LANGLE: /* "><" */
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
		/* Convert to ">" token */
		if (lparen_token == '<') {
			tpp_assert(tpp_lexer_gettoken(self)->tt_start < *p_pos);
			tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '>');
			*p_pos = tpp_lexer_gettoken(self)->tt_start + 1;
			tpp_lexer_gettoken(self)->tt_id = TPP_TOK_OFCHAR('>');
			return true;
		}
		break;
#endif /* ... */
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

	default: break;
	}
	return false;
}

/* Parse the parameter list, with self/p_pos pointing at the opening '('-token.
 * Upon success, self/p_pos points after the closing ')'-token. */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_macro_builder_parse_params(tpp_macro_builder *tpp_restrict builder,
                               tpp_lexer *tpp_restrict self,
                               tpp_char const **p_pos) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_token_id const lparen_token = token->tt_id;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_size rel_start = tpp_file_ptr2rel(file, *p_pos);
	tpp_token_id tok;
	(void)rel_start;
again_yield_macro_argument_list:
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tpp_lexer_is_rparen_token(self, p_pos, lparen_token))
		goto done_yield_rparen; /* Empty parameter list. */
/*again_switch_macro_argument_list:*/
	switch (tok) {
	case TPP_TOK_EOF:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT_LINE
		goto warn_unexpected_parameter_list_token;

#if TPP_HAVE_VA_ARGS_IN_MACROS
	case TPP_TOK_DOT_DOT_DOT:
		if (!tpp_lexer_has(self, VA_ARGS_IN_MACROS))
			break;
		tok = TPP_KWD___VA_ARGS__;
		builder->mab_flags |= TPP_MACRO_FLAG_VARIADIC;
		goto do_append_keyword_to_argument_list;
#define WANT_do_append_keyword_to_argument_list
#endif /* TPP_HAVE_VA_ARGS_IN_MACROS */

	default: break;
	}

	if (!TPP_TOK_ISKEYWORD(tok)) {
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
		/* Warning: expected keyword in function-style macro argument list */
		tpp_errno error;
		tpp_char const *saved_end = token->tt_end;
		token->tt_end = *p_pos;
		error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST);
		token->tt_end = saved_end;
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
		goto again_yield_macro_argument_list;
	}

	/* Append "tok" to macro argument list. */
	{
		tpp_macro_argument *arg;
#ifdef WANT_do_append_keyword_to_argument_list
#undef WANT_do_append_keyword_to_argument_list
do_append_keyword_to_argument_list:
#endif /* WANT_do_append_keyword_to_argument_list */

		/* Check if "tok" is already a known argument. */
#if TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME
		arg = tpp_macro_builder_getargument(builder, tok);
		if tpp_unlikely(arg) {
			tpp_errno error;
			tpp_char const *saved_end = token->tt_end;
			token->tt_end = *p_pos;
			error = tpp_lexer_warnf(self, TPP_W_DUPLICATE_MACRO_PARAMETER_NAME);
			token->tt_end = saved_end;
			if (TPP_ISERR(error))
				return error;
		}
#endif /* TPP_HAVE_TPP_W_DUPLICATE_MACRO_PARAMETER_NAME */

		/* Allocate new argument */
		arg = tpp_macro_builder_newargument(builder);
		if tpp_unlikely(!arg)
			return TPP_ENOMEM;
		arg->tma_id      = tok;
		arg->tma_ins_exp = 0;
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
		arg->tma_ins_str = 0;
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
		arg->tma_ins = 0;
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
	}

	/* Yield to next token (which should be one of ",", ")", or "...") */
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
	} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Check for named varargs, as in:
	 * >> #define printf(format, args...) fprintf(stderr, format,##args) */
#if TPP_HAVE_NAMED_VARARGS_IN_MACROS
	if (tok == TPP_TOK_DOT_DOT_DOT &&
	    !(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC) &&
	    tpp_lexer_has(self, NAMED_VARARGS_IN_MACROS)) {
		builder->mab_flags |= TPP_MACRO_FLAG_VARIADIC;
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}
#endif /* TPP_HAVE_NAMED_VARARGS_IN_MACROS */

	/* Check if there are more arguments... */
	if (tok == ',') {
#ifdef TPP_MACRO_FLAG_VARIADIC
		if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
#endif /* TPP_MACRO_FLAG_VARIADIC */
		{
			goto again_yield_macro_argument_list;
		}
	}

	if (tpp_lexer_is_rparen_token(self, p_pos, lparen_token)) {
done_yield_rparen:
		do {
			tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
		return TPP_TOK_ASERR_OR_EOK(tok);
	}

warn_unexpected_parameter_list_token:
	/* Warning: expected keyword in function-style macro argument list */
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
	{
		tpp_errno error;
		tpp_char const *saved_end = token->tt_end;
		token->tt_end = *p_pos;
		error = tpp_lexer_warnf_at(self, file, tpp_file_rel2ptr(file, rel_start),
		                           TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST);
		token->tt_end = saved_end;
		return error;
	}
#else /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST */
}


/* Allocate additional opcodes */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_opcode *TPPCALL
tpp_macro_builder_requireop(tpp_macro_builder *tpp_restrict self,
                            tpp_size num_opcode_words) {
	tpp_macro_opcode *result;
	tpp_size cur_alloc = self->mab_tmf_expanda;
	tpp_size min_alloc = self->mab_tmf_expandc + num_opcode_words;
	tpp_assert(self->mab_tmf_expandc <= cur_alloc);
	if (cur_alloc < min_alloc) {
		tpp_macro *new_macro;
		tpp_size new_alloc = self->mab_tmf_expanda * 2;
		if (new_alloc < 16)
			new_alloc = 16;
		if (new_alloc < min_alloc)
			new_alloc = min_alloc;
		new_macro = _tpp_macro_tryrealloc_function(self->mab_macro, new_alloc);
		if tpp_unlikely(!new_macro) {
			new_alloc = min_alloc;
			new_macro = _tpp_macro_realloc_function(self->mab_macro, new_alloc);
			if tpp_unlikely(!new_macro)
				return NULL;
		}
		self->mab_macro       = new_macro;
		self->mab_tmf_expanda = new_alloc;
	}
	tpp_assert(self->mab_tmf_expanda >= min_alloc);
	result = self->mab_macro->tm_data.tmd_func.tmf_expand;
	result += self->mab_tmf_expandc;
	self->mab_tmf_expandc += num_opcode_words;
	return result;
}

#define _tpp_macro_builder_appendops(err_nomem, self, n_ops, init)                  \
	do {                                                                            \
		tpp_macro_opcode *const opcodes = tpp_macro_builder_requireop(self, n_ops); \
		if tpp_unlikely(!opcodes)                                                   \
			goto err_nomem;                                                         \
		(init);                                                                     \
	} while (0)

#define tpp_macro_builder_append_end(err_nomem, self) \
	_tpp_macro_builder_appendops(err_nomem, self, 1, (opcodes[0] = TPP_MACRO_OPCODE_END))
#define tpp_macro_builder_append_skip(err_nomem, self, num_bytes)     \
	_tpp_macro_builder_appendops(err_nomem, self, 2,                  \
	                             (opcodes[0] = TPP_MACRO_OPCODE_SKIP, \
	                              (self)->mab_skiptotal += (opcodes[1] = (num_bytes))))
#define tpp_macro_builder_append_copy(err_nomem, self, num_bytes)     \
	_tpp_macro_builder_appendops(err_nomem, self, 2,                  \
	                             (opcodes[0] = TPP_MACRO_OPCODE_COPY, \
	                              opcodes[1] = (num_bytes)))
#define tpp_macro_builder_append_ins_exp(err_nomem, self, arg, skip_bytes)              \
	_tpp_macro_builder_appendops(err_nomem, self, 3,                                    \
	                             (opcodes[0] = TPP_MACRO_OPCODE_INS_EXP,                \
	                              opcodes[1] = (tpp_size)((arg) - (self)->mab_argv),    \
	                              (self)->mab_skiptotal += (opcodes[2] = (skip_bytes)), \
	                              ++(arg)->tma_ins_exp))
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#define tpp_macro_builder_append_ins_str(err_nomem, self, opcode, arg, skip_bytes)      \
	_tpp_macro_builder_appendops(err_nomem, self, 3,                                    \
	                             (opcodes[0] = opcode, /* TPP_MACRO_OPCODE_INS_STR or   \
	                                                    * TPP_MACRO_OPCODE_INS_CHR */   \
	                              opcodes[1] = (tpp_size)((arg) - (self)->mab_argv),    \
	                              (self)->mab_skiptotal += (opcodes[2] = (skip_bytes)), \
	                              (self)->mab_skiptotal -= 2 /* leading/trailing " */,  \
	                              ++(arg)->tma_ins_str))
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
#define tpp_macro_builder_append_ins(err_nomem, self, arg, skip_bytes)                  \
	_tpp_macro_builder_appendops(err_nomem, self, 3,                                    \
	                             (opcodes[0] = TPP_MACRO_OPCODE_INS,                    \
	                              opcodes[1] = (tpp_size)((arg) - (self)->mab_argv),    \
	                              (self)->mab_skiptotal += (opcodes[2] = (skip_bytes)), \
	                              ++(arg)->tma_ins))
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
#if TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
#define tpp_macro_builder_append_va_comma(err_nomem, self, skip_bytes)                  \
	_tpp_macro_builder_appendops(err_nomem, self, 2,                                    \
	                             (opcodes[0] = TPP_MACRO_OPCODE_VA_COMMA,               \
	                              (self)->mab_skiptotal += (opcodes[1] = (skip_bytes)), \
	                              ++(self)->mab_n_vaopt))
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */
#if TPP_HAVE_VA_OPT_IN_MACROS
#define tpp_macro_builder_append_va_opt(err_nomem, self, skip1, copy, skip2)       \
	_tpp_macro_builder_appendops(err_nomem, self, 4,                               \
	                             (opcodes[0] = TPP_MACRO_OPCODE_VA_OPT,            \
	                              (self)->mab_skiptotal += (opcodes[1] = (skip1)), \
	                              (self)->mab_skiptotal += (opcodes[2] = (copy)),  \
	                              (self)->mab_skiptotal += (opcodes[3] = (skip2)), \
	                              (self)->mab_n_vaopt += (copy)))
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */
#if TPP_HAVE_VA_NARGS_IN_MACROS
#define tpp_macro_builder_append_va_nargs(err_nomem, self, skip)                  \
	_tpp_macro_builder_appendops(err_nomem, self, 2,                              \
	                             (opcodes[0] = TPP_MACRO_OPCODE_VA_NARGS,         \
	                              (self)->mab_skiptotal += (opcodes[1] = (skip)), \
	                              ++(self)->mab_n_vanargs))
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */


#if TPP_HAVE_TRADITIONAL_MACROS
/* Compile a traditional macro (allowed to clobber the token in "builder") */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile_traditional_impl(tpp_macro_builder *tpp_restrict builder,
                                           tpp_lexer *tpp_restrict self,
                                           tpp_char const *body_start,
                                           tpp_char const *body_end) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_char const *body_iter = body_start;

	/* Scan for the body for keywords (including inside of "string" or (*comment*) tokens)
	 * to see if we can find mentions of arguments taken by the macro. Anything found here
	 * must then be used as a point to inject (expanded) arguments.
	 *
	 * As a consequence, you can "stringize" (kind-of) like this:
	 * >> #define str(x) "x"
	 * However, I say "kind-of" because this won't re-escape:
	 * >> str(foo)    // OK:   "foo"           (1 token)
	 * >> str("foo")  // Huh?  ""   foo   ""   (3 tokens)
	 *
	 * ... yeah. It's argument substitution in the most literal sense (which is also why
	 * it has been superseded by "modern" macro compilation for a very long time; as a
	 * matter of fact: ever since __STDC__ has been introduced). So yes: this sort of
	 * behavior actually goes back to those good 'ol <<K&R C>> times. */

	while (body_iter < body_end) {
		tpp_macro_argument *arg;
		tpp_token_id tok;
		tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
		switch (tok) {

#if TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
		TPP_CASE_TPP_TOK_COMMENT_NOLINE {
			if (TPP_TOK_ISCOMMENT_NOLINE(tok)) {
				/* Non-line comments must be deleted in order to support traditional cat operations!
				 * Also note that line-comments shouldn't be present at all (since those should have
				 * caused our caller to terminate the macro body, in case you're wondering) */
				if (token->tt_start > body_start) {
					tpp_macro_builder_append_copy(err_nomem, builder,
						                          (tpp_size)(token->tt_start - body_start));
				}
				tpp_macro_builder_append_skip(err_nomem, builder,
					                          (tpp_size)(body_iter - token->tt_start));
				body_start = body_iter;
			}
		}	continue; /* Not a keyword */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE */

#if TPP_HAVE_TPP_TOK_C_STRINGLIKE
		/* Strings must not actually be parsed as whole tokens!
		 *
		 * Since this can (easily) cause warnings to be emitted
		 * (~ala "string terminated by eol"), our caller has
		 * disabled them for us!
		 *
		 * This is needed for stuff like:
		 * >> #define str(x) "x"
		 */
		TPP_CASE_TPP_TOK_C_STRING {
			body_iter = token->tt_start + 1;
			continue;
		}
#endif /* TPP_HAVE_TPP_TOK_C_STRINGLIKE */

		default:
			/* Shouldn't really be able to produce errors, but better be safe. */
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (!TPP_TOK_ISKEYWORD(tok))
				continue;
			break;
		}


		/* Check if this keyword (identified by "tok") is an argument. */
		arg = tpp_macro_builder_getargument(builder, tok);
		if (!arg)
			continue; /* Not actually an argument */

		/* Append opcodes to copy text leading up to argument. */
		if (token->tt_start > body_start) {
			tpp_macro_builder_append_copy(err_nomem, builder,
			                              (tpp_size)(token->tt_start - body_start));
		}

		/* Append opcodes to insert argument */
		tpp_macro_builder_append_ins_exp(err_nomem, builder, arg,
		                                 (tpp_size)(body_iter - token->tt_start));

		/* Remember that input body text has been
		 * flushed until the end of the keyword. */
		body_start = body_iter;
	}

	/* Copy remainder. */
	if (body_start < body_end) {
		tpp_macro_builder_append_copy(err_nomem, builder,
		                              (tpp_size)(body_end - body_start));
	}

	/* Terminate body builder (*flexes muscles*) */
	tpp_macro_builder_append_end(err_nomem, builder);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}

#if TPP_HAVE_WARNINGS
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile_traditional(tpp_macro_builder *tpp_restrict builder,
                                      tpp_lexer *tpp_restrict self,
                                      tpp_char const *body_start,
                                      tpp_char const *body_end) {
	tpp_errno result;
	/* Disable warnings because compiler does some questionable stuff in order to parse
	 * the contents of strings in order to allow arguments to be embedded within them. */
	tpp_lexer_nowarnings_pushon(self);
	result = tpp_macro_builder_compile_traditional_impl(builder, self, body_start, body_end);
	tpp_lexer_nowarnings_pop(self);
	return result;
}
#else /* TPP_HAVE_WARNINGS */
#define tpp_macro_builder_compile_traditional(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_traditional_impl(builder, self, body_start, body_end)
#endif /* !TPP_HAVE_WARNINGS */
#endif /* TPP_HAVE_TRADITIONAL_MACROS */

#if TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS)
/* Compile a modern macro (allowed to clobber the token in "builder") */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile_modern(tpp_macro_builder *tpp_restrict builder,
                                 tpp_lexer *tpp_restrict self,
                                 tpp_char const *body_start,
                                 tpp_char const *body_end) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_char const *body_iter = body_start;
#if TPP_HAVE_GLUE_MACRO_ARGUMENT
	/* Specifies the start of a whitespace-area that is deleted by the ##-operator */
	tpp_char const *last_non_space_end = body_start;
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT */

	while (body_iter < body_end) {
		tpp_macro_argument *arg;
		tpp_token_id tok;
		tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
#if (TPP_HAVE_GLUE_MACRO_ARGUMENT || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS ||       \
     TPP_HAVE_VA_OPT_IN_MACROS || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT ||         \
     TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || \
     TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED || TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR)
again_switch_tok:
#endif /* ... */
		switch (tok) {


/************************************************************************/
#if TPP_HAVE_GLUE_MACRO_ARGUMENT
		case TPP_TOK_SPACE:
		TPP_CASE_TPP_TOK_COMMENT_NOLINE
			/* "continue" so-as not to update "last_non_space_end",
			 * allowing a potentially following ##-operator to delete
			 * this token. */
			continue;

		case TPP_TOK_POUND_POUND: {
			tpp_char const *argument_end;
			if (!tpp_lexer_has(self, GLUE_MACRO_ARGUMENT))
				break;
			/* Consume any whitespace that might following this operator.
			 * Whitespace preceding it is automatically consumed because
			 * we skip all not-already-flushed data after "last_non_space_end" */
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);

			/* Encode a copy operation for range: [body_start, last_non_space_end) */
			if (body_start < last_non_space_end) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(last_non_space_end - body_start));
				body_start = last_non_space_end;
			}

			/* Encode a skip operation for range: [body_start, token->tt_start) */
			if (body_start < token->tt_start) {
				tpp_macro_builder_append_skip(err_nomem, builder,
				                              (tpp_size)(token->tt_start - body_start));
				body_start = token->tt_start;
			}

handle_token_after_glue:
			/* If what follows after the ##-operator is another keyword,
			 * must treat it specially if it's a macro-argument! */
			if (!TPP_TOK_ISKEYWORD(tok))
				goto again_switch_tok;
			arg = tpp_macro_builder_getargument(builder, tok);
			if (arg == NULL)
				goto again_switch_tok; /* Must switch on "tok" in case it's something like __VA_NARGS__! */
			argument_end = body_iter; /* End of argument keyword after "##"-token */

			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);

			/* Check if there'll be another ##-operator after the argument */
			if (tok == TPP_TOK_POUND_POUND) {
				do {
					tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
				if (body_start < token->tt_start) {
					tpp_macro_builder_append_ins(err_nomem, builder, arg,
					                             (tpp_size)(token->tt_start - body_start));
					body_start = token->tt_start;
				}
				goto handle_token_after_glue;
			}

			/* End of ##-chain, but still: must insert the last argument *without* expansion! */
			tpp_macro_builder_append_ins(err_nomem, builder, arg,
			                             (tpp_size)(argument_end - body_start));
			last_non_space_end = argument_end;
			body_start = argument_end;
			goto again_switch_tok;
		}	break;
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
		case ',': {
			tpp_char const *start_of_comma;

			/* >> #define printf(format, ...) fprintf(stderr, format,##__VA_ARGS__)
			 *
			 * Compiled the same as (when ignoring whitespace):
			 * >> #define printf(format, ...) fprintf(stderr, format __VA_COMMA__ __VA_ARGS__) */
			if (!tpp_lexer_has(self, VA_GLUE_COMMA_IN_MACROS))
				break;
			if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
				break;
			start_of_comma = token->tt_start;
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (tok != TPP_TOK_POUND_POUND)
				goto again_switch_tok;
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (!TPP_TOK_ISKEYWORD(tok)) {
				/* Implement regular glue (simply deleting whitespace), or do nothing */
handle_not_varargs_argument_after_comma_glue:
#if TPP_HAVE_GLUE_MACRO_ARGUMENT
				if (tpp_lexer_has(self, GLUE_MACRO_ARGUMENT)) {
					++start_of_comma; /* Keep the "," character itself! */
					if (body_start < start_of_comma) {
						tpp_macro_builder_append_copy(err_nomem, builder,
						                              (tpp_size)(start_of_comma - body_start));
						body_start = start_of_comma;
					}
					/* Encode a skip operation for range: [body_start, token->tt_start) */
					if (body_start < token->tt_start) {
						tpp_macro_builder_append_skip(err_nomem, builder,
						                              (tpp_size)(token->tt_start - body_start));
						body_start = token->tt_start;
					}
				}
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT */
				goto again_switch_tok;
			}
			arg = tpp_macro_builder_getargument(builder, tok);
			if (!arg)
				goto handle_not_varargs_argument_after_comma_glue; /* Not an argument */
			if (arg != builder->mab_argv + builder->mab_argc - 1)
				goto handle_not_varargs_argument_after_comma_glue; /* Not the varargs argument */

			/* Yes! This *does* have to be encoded as a __VA_COMMA__! */
			if (body_start < start_of_comma) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(start_of_comma - body_start));
				body_start = start_of_comma;
			}
			/* Insert __VA_COMMA__ and skip template body until the start of the varargs argument */
			tpp_macro_builder_append_va_comma(err_nomem, builder,
			                                  (tpp_size)(token->tt_start - body_start));
			body_start = token->tt_start;
			goto handle_keyword_after_arg;
#define WANT_handle_keyword_after_arg
		}	break;
#endif /* TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_VA_COMMA_IN_MACROS
		case TPP_KWD___VA_COMMA__:
			if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
				goto handle_keyword;
			if (!tpp_lexer_has(self, VA_COMMA_IN_MACROS))
				goto handle_keyword;
#define WANT_handle_keyword
			if (body_start < token->tt_start) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(token->tt_start - body_start));
				body_start = token->tt_start;
			}
			tpp_macro_builder_append_va_comma(err_nomem, builder,
			                                  (tpp_size)(body_iter - body_start));
			body_start = body_iter;
			break;
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_VA_NARGS_IN_MACROS
		case TPP_KWD___VA_NARGS__:
			if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
				goto handle_keyword;
			if (!tpp_lexer_has(self, VA_NARGS_IN_MACROS))
				goto handle_keyword;
#define WANT_handle_keyword
			if (body_start < token->tt_start) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(token->tt_start - body_start));
				body_start = token->tt_start;
			}
			tpp_macro_builder_append_va_nargs(err_nomem, builder,
			                                  (tpp_size)(body_iter - body_start));
			body_start = body_iter;
			break;
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_VA_OPT_IN_MACROS
		case TPP_KWD___VA_OPT__: {
			tpp_char const *start_of_va_opt;
			tpp_char const *start_of_va_opt_body;
			tpp_char const *end_of_va_opt_body;
			unsigned int recursion;
			if (!(builder->mab_flags & TPP_MACRO_FLAG_VARIADIC))
				goto handle_keyword;
			if (!tpp_lexer_has(self, VA_OPT_IN_MACROS))
				goto handle_keyword;
#define WANT_handle_keyword
			start_of_va_opt = token->tt_start;

			/* Next token must be ( */
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (tok != '(') {
#if TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT
				tpp_errno error;
				tpp_char const *saved_end = token->tt_end;
				token->tt_end = body_iter;
				error = tpp_lexer_warnf_at(self, tpp_lexer_getfile(self), start_of_va_opt,
				                           TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT);
				token->tt_end = saved_end;
				if (TPP_ISERR(error))
					return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT */
				last_non_space_end = body_iter;
				break;
			}

			start_of_va_opt_body = body_iter;
			recursion = 0;
			for (;;) {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				switch (tok) {
				case TPP_TOK_EOF: {
#if TPP_HAVE_TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT
					tpp_errno error;
					tpp_char const *saved_end = token->tt_end;
					token->tt_end = body_iter;
					error = tpp_lexer_warnf_at(self, tpp_lexer_getfile(self), start_of_va_opt,
					                           TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT);
					token->tt_end = saved_end;
					if (TPP_ISERR(error))
						return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT */
					goto found_va_opt_body_end;
				}	break;
				case '(':
					++recursion;
					break;
				case ')':
					if (recursion == 0)
						goto found_va_opt_body_end;
					--recursion;
					break;
				default:
					if (TPP_TOK_ISERR(tok))
						return TPP_TOK_ASERR(tok);
					break;
				}
			}
found_va_opt_body_end:
			end_of_va_opt_body = token->tt_start;
			if (body_start < start_of_va_opt) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(start_of_va_opt - body_start));
				body_start = start_of_va_opt;
			}
			tpp_macro_builder_append_va_opt(err_nomem, builder,
			                                (tpp_size)(start_of_va_opt_body - start_of_va_opt),
			                                (tpp_size)(end_of_va_opt_body - start_of_va_opt_body),
			                                (tpp_size)(body_iter - end_of_va_opt_body));
			body_start = body_iter;
		}	break;
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || \
     TPP_HAVE_CHARIZE_MACRO_ARGUMENT ||   \
     TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT)
#if TPP_HAVE_TPP_TOK_POUND_COMMENT
		case TPP_TOK_POUND_COMMENT:
			/* Deal with special case of shell comments (which must be re-parsed as a #-token) */
			body_iter = token->tt_start + 1;
			TPP_FALLTHRU
#endif /* TPP_HAVE_TPP_TOK_POUND_COMMENT */
		case '#': {
			tpp_char const *start_of_pound;
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
			tpp_macro_opcode opcode;
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
			if (!tpp_lexer_has(self, STRINGIZE_MACRO_ARGUMENT) &&
			    !tpp_lexer_has(self, CHARIZE_MACRO_ARGUMENT) &&
			    !tpp_lexer_has(self, DONT_EXPAND_MACRO_ARGUMENT))
				break;
			start_of_pound = token->tt_start;
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);

#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
			if (tok == '!') {
				last_non_space_end = body_iter;
				if (!tpp_lexer_has(self, DONT_EXPAND_MACRO_ARGUMENT))
					break;
				do {
					tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
				if (!TPP_TOK_ISKEYWORD(tok))
					goto again_switch_tok;
				arg = tpp_macro_builder_getargument(builder, tok);
				if (!arg)
					goto again_switch_tok;
				if (body_start < start_of_pound) {
					tpp_macro_builder_append_copy(err_nomem, builder,
					                              (tpp_size)(start_of_pound - body_start));
					body_start = start_of_pound;
				}
				tpp_macro_builder_append_ins(err_nomem, builder, arg,
				                             (tpp_size)(body_iter - body_start));
				body_start = body_iter;
				break;
			} else
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT */
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT
			if (tok == '@') {
				if (!tpp_lexer_has(self, CHARIZE_MACRO_ARGUMENT))
					break;
				opcode = TPP_MACRO_OPCODE_INS_CHR;
				do {
					tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
				if (TPP_TOK_ISERR(tok))
					return TPP_TOK_ASERR(tok);
			} else
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
			{
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
				if (!tpp_lexer_has(self, STRINGIZE_MACRO_ARGUMENT))
					break;
				opcode = TPP_MACRO_OPCODE_INS_STR;
#else /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
				break;
#endif /* !TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
			}
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
			if (!TPP_TOK_ISKEYWORD(tok))
				goto again_switch_tok;
			arg = tpp_macro_builder_getargument(builder, tok);
			if (!arg)
				goto again_switch_tok;
			if (body_start < start_of_pound) {
				tpp_macro_builder_append_copy(err_nomem, builder,
				                              (tpp_size)(start_of_pound - body_start));
				body_start = start_of_pound;
			}
			tpp_macro_builder_append_ins_str(err_nomem, builder, opcode, arg,
			                                 (tpp_size)(body_iter - body_start));
			body_start = body_iter;
			break;
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT || TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
		}	break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED || TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR
		case TPP_KWD_defined: {
			/* case TPP_KWD_defined: -Wexpansion-to-defined
			 *
			 * Warn about use of "defined" in the body of function-style macros.
			 *
			 * BUT: Not in the following cases:
			 * >> #define foo(defined, x) defined(x)    // Macro has a parameter "defined"
			 * >> #define foo(x)          defined+x     // Next token isn't '(' or a keyword (also handles "defined(#!x)")
			 * >> #define foo(x)          defined(bar)  // Linked keyword isn't a macro argument
			 *
			 * Also: when -fdont-expand-defined is enabled, don't emit
			 *       the waring and instead suppress expansion of the
			 *       macro argument. */
#if TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED
			tpp_char const *start_of_defined = token->tt_start;
#endif /* TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED */
			arg = tpp_macro_builder_getargument(builder, TPP_KWD_defined);
			if (arg)
				goto handle_keyword_after_arg; /* "defined" is actually an argument! */
#define WANT_handle_keyword_after_arg

			last_non_space_end = body_iter;
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
			} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			if (tok == '(') {
				last_non_space_end = body_iter;
				do {
					tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
				} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
			}
			if (!TPP_TOK_ISKEYWORD(tok))
				goto again_switch_tok;
			arg = tpp_macro_builder_getargument(builder, tok);
			if (!arg)
				goto again_switch_tok;

			/* Got a keyword that is actually a macro argument after "defined"
			 * -> This is what this extension/warning is all about! */
#if TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR
			if (tpp_lexer_has(self, DONT_EXPAND_DEFINED_IN_EXPR)) {
				if (body_start < token->tt_start) {
					tpp_macro_builder_append_copy(err_nomem, builder,
					                              (tpp_size)(token->tt_start - body_start));
					body_start = token->tt_start;
				}
				tpp_macro_builder_append_ins(err_nomem, builder, arg,
				                             (tpp_size)(body_iter - body_start));
				body_start = body_iter;
				break;
			}
#endif /* TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR */

			/* Emit the warning */
#if TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED
			{
				tpp_errno error;
				tpp_char const *saved_end = token->tt_end;
				token->tt_end = body_iter;
				error = tpp_lexer_warnf_at(self, tpp_lexer_getfile(self), start_of_defined,
				                           TPP_W_EXPANSION_TO_DEFINED);
				token->tt_end = saved_end;
				if (TPP_ISERR(error))
					return error;
			}
#endif /* TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED */

			goto handle_keyword_after_arg;
#define WANT_handle_keyword_after_arg
		}	break;
#endif /* TPP_HAVE_TPP_W_EXPANSION_TO_DEFINED || TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR */
/************************************************************************/



/************************************************************************/
		default: {
			/* Shouldn't really be able to produce errors, but better be safe. */
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (TPP_TOK_ISKEYWORD(tok)) {
#ifdef WANT_handle_keyword
#undef WANT_handle_keyword
handle_keyword:
#endif /* WANT_handle_keyword */
				/* Check if this keyword (identified by "tok") is an argument. */
				arg = tpp_macro_builder_getargument(builder, tok);
				if (!arg)
					break; /* Not actually an argument */

#ifdef WANT_handle_keyword_after_arg
#undef WANT_handle_keyword_after_arg
handle_keyword_after_arg:
#endif /* WANT_handle_keyword_after_arg */
				/* Append opcodes to copy text leading up to argument. */
				if (body_start < token->tt_start) {
					tpp_macro_builder_append_copy(err_nomem, builder,
					                              (tpp_size)(token->tt_start - body_start));
					body_start = token->tt_start;
				}

#if TPP_HAVE_GLUE_MACRO_ARGUMENT
				if (tpp_lexer_has(self, GLUE_MACRO_ARGUMENT)) {
					/* Must seek ahead to the next non-whitespace token. if it's the
					 * ##-operator, then we have to insert the argument *WITHOUT* it
					 * being expanded! */
					tpp_char const *argument_end = body_iter;
					do {
						tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
					} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
					if (TPP_TOK_ISERR(tok))
						return TPP_TOK_ASERR(tok);

					/* Append opcodes to insert argument */
					if (tok == TPP_TOK_POUND_POUND) {
						do {
							tok = tpp_lexer_yieldraw_at_blocking(self, &body_iter);
						} while (TPP_TOK_ISSPACE_OR_COMMENT(tok));
						if (TPP_TOK_ISERR(tok))
							return TPP_TOK_ASERR(tok);
						/* Insert argument without expansion, and skip input until the start
						 * of the first non-whitespace token following the ##-operator itself. */
						tpp_macro_builder_append_ins(err_nomem, builder, arg,
						                             (tpp_size)(token->tt_start - body_start));
						body_start = token->tt_start;
						goto handle_token_after_glue;
					}

					/* Insert argument regularly (with expansion) and (since we've already
					 * processed data until that point), continue processing on the next
					 * non-whitespace token. */
					tpp_macro_builder_append_ins_exp(err_nomem, builder, arg,
					                                 (tpp_size)(argument_end - body_start));
					last_non_space_end = argument_end;
					body_start = argument_end;
					goto again_switch_tok;
				} else
#endif /* TPP_HAVE_GLUE_MACRO_ARGUMENT */
				{
#if TPP_CONF_MAYBE_0(TPP_HAVE_GLUE_MACRO_ARGUMENT)
					/* Append opcodes to insert argument */
					tpp_macro_builder_append_ins_exp(err_nomem, builder, arg,
					                                 (tpp_size)(body_iter - body_start));

					/* Remember that input body text has been
					 * flushed until the end of the keyword. */
					body_start = body_iter;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_GLUE_MACRO_ARGUMENT) */
				}
			}
		}	break;
/************************************************************************/

		}
		last_non_space_end = body_iter;
	}

	/* Copy remainder. */
	if (body_start < body_end) {
		tpp_macro_builder_append_copy(err_nomem, builder,
		                              (tpp_size)(body_end - body_start));
	}

	/* Terminate body builder */
	tpp_macro_builder_append_end(err_nomem, builder);
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TRADITIONAL_MACROS) */


#if TPP_CONF_IS_RT(TPP_HAVE_TRADITIONAL_MACROS)
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile(tpp_macro_builder *tpp_restrict builder,
                          tpp_lexer *tpp_restrict self,
                          tpp_char const *body_start,
                          tpp_char const *body_end) {
	if (tpp_lexer_has(self, TRADITIONAL_MACROS))
		return tpp_macro_builder_compile_traditional(builder, self, body_start, body_end);
	return tpp_macro_builder_compile_modern(builder, self, body_start, body_end);
}
#elif TPP_HAVE_TRADITIONAL_MACROS
#define tpp_macro_builder_compile(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_traditional(builder, self, body_start, body_end)
#else /* TPP_HAVE_TRADITIONAL_MACROS */
#define tpp_macro_builder_compile(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_modern(builder, self, body_start, body_end)
#endif /* !TPP_HAVE_TRADITIONAL_MACROS */

static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) TPP_REF tpp_macro *TPPCALL
tpp_macro_builder_pack(/*inherit(on_success)*/ tpp_macro_builder *tpp_restrict self,
                       tpp_lexer *tpp_restrict lexer,
                       tpp_char const *body_start,
                       tpp_char const *body_end,
                       tpp_lcinfo deflc, tpp_token_id lparen_token) {
	TPP_REF tpp_macro *result = self->mab_macro;
	tpp_file *const file = tpp_lexer_getfile(lexer);
	if tpp_unlikely(result == NULL) {
		tpp_assert(self->mab_tmf_expandc == 0);
		tpp_assert(self->mab_tmf_expanda == 0);

		/* Special case: this can happen if the macro's body is empty. */
		result = _tpp_macro_alloc_function(1); /* 1: TPP_MACRO_OPCODE_END */
		if tpp_unlikely(!result)
			return NULL;
		result->tm_data.tmd_func.tmf_expand[0] = TPP_MACRO_OPCODE_END;
	} else
#ifndef __OPTIMIZE_SIZE__
	if (self->mab_tmf_expandc < self->mab_tmf_expanda) {
		TPP_REF tpp_macro *new_result;
		new_result = _tpp_macro_tryrealloc_function(result, self->mab_tmf_expandc);
		if tpp_likely(new_result)
			result = new_result;
	} else
#endif /* !__OPTIMIZE_SIZE__ */
	{
	}

	/* Fill in fields of macro. */
	tpp_refcnt_init(&result->tm_refcnt, 1);
	result->tm_kind = TPP_MACRO_KIND_OFTOK(lparen_token);
#if TPP_HAVE_MACRO_FLAGS
	result->tm_flags = self->mab_flags;
#endif /* TPP_HAVE_MACRO_FLAGS */
	result->tm_body_chunk = file->tf_chunk;
	if (result->tm_body_chunk)
		tpp_string_incref(result->tm_body_chunk);
	result->tm_body_start = body_start;
	result->tm_body_end   = body_end;
#if TPP_HAVE_UNICODE
	result->tm_body_enc = file->tf_enc;
#endif /* TPP_HAVE_UNICODE */
	result->tm_expansions = 0;
	result->tm_deffile    = tpp_file_getrealfilename(file);
	if (result->tm_deffile) {
		result->tm_deflc   = deflc;
		result->tm_body_lc = tpp_file_getlcinfo(file, body_start);
	}
	result->tm_data.tmd_func.tmf_argc    = self->mab_argc;
	result->tm_data.tmd_func.tmf_argv    = self->mab_argv; /* Inherit data */
	result->tm_data.tmd_func.tmf_expbase = (tpp_size)(body_end - body_start);
	result->tm_data.tmd_func.tmf_expbase -= self->mab_skiptotal;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
	result->tm_data.tmd_func.tmf_n_vaopt = self->mab_n_vaopt;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	result->tm_data.tmd_func.tmf_n_vanargs = self->mab_n_vanargs;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
	result->tm_data.tmd_func.tmf_argbuf = NULL;
#if TPP_DEBUG
	tpp_memset(self, 0xcc, sizeof(*self));
#endif /* TPP_DEBUG */
	return result;
}


/* Parse a macro-definition, with self/p_pos pointing at the first non-inline-comment
 * token following the macro's name. (in the case of a keyword-style macro, this may
 * be a space-token!)
 *
 * Upon (successful) return, the lexer points at the TPP_TOK_LF (or line-comment)
 * following the definition, or is set to TPP_TOK_EOF if the macro definition is
 * followed by eof-of-file.
 *
 * @return: TPP_EOK: The newly parsed macro definition
 * @return: * :      Error */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_lexer_parse_macro_definition(tpp_lexer *tpp_restrict self,
                                 TPP_REF tpp_macro **p_macro,
                                 tpp_char const **p_pos,
                                 tpp_lcinfo deflc) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_token_id const lparen_token = token->tt_id;
	tpp_macro_builder builder;
	tpp_size rel_body_start;
	tpp_size rel_body_end;
	tpp_token_id tok;
	tpp_char const *body_start;
	tpp_char const *body_end;
	TPP_REF tpp_macro *macro;

	/* Check if this is going to be a function- or keyword-style definition */
	switch (lparen_token) {
	case '(':
		break;

#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
	case '[':
	case '{':
	case '<':
		if (tpp_lexer_has(self, ALTERNATIVE_MACRO_PARENTHESIS))
			break;
		TPP_FALLTHRU
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */
	default: {
		/* Keyword-style macro */
		tok = lparen_token;

		/* Find start of body */
		while (TPP_TOK_ISSPACE_OR_COMMENT(tok)) {
			tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
		rel_body_start = tpp_file_ptr2rel(file, token->tt_start);
		rel_body_end   = rel_body_start;

		/* Find end of body */
		while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
			/* Only update body on non-space tokens (thereby trimming trailing space/comments from macros) */
			if (!TPP_TOK_ISSPACE_OR_COMMENT(tok))
				rel_body_end = tpp_file_ptr2rel(file, *p_pos);
			tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}

		/* Allocate keyword-style macro definition */
		macro = _tpp_macro_alloc_keyword();
		if tpp_unlikely(!macro)
			return TPP_ENOMEM;

		/* Fill in macro information */
		tpp_refcnt_init(&macro->tm_refcnt, 1);
		macro->tm_kind = TPP_MACRO_KIND_KEYWORD;
#if TPP_HAVE_MACRO_FLAGS
		macro->tm_flags = TPP_MACRO_FLAG_NORMAL;
#endif /* TPP_HAVE_MACRO_FLAGS */
		macro->tm_body_chunk = token->tt_chunk;
		if (macro->tm_body_chunk)
			tpp_string_incref(macro->tm_body_chunk);
		macro->tm_body_start = tpp_file_rel2ptr(file, rel_body_start);
		macro->tm_body_end   = tpp_file_rel2ptr(file, rel_body_end);
#if TPP_HAVE_UNICODE
		macro->tm_body_enc = file->tf_enc;
#endif /* TPP_HAVE_UNICODE */
		macro->tm_expansions = 0;
		macro->tm_deffile = tpp_file_getrealfilename(file);
		if (macro->tm_deffile) {
			macro->tm_deflc   = deflc;
			macro->tm_body_lc = tpp_file_getlcinfo(file, macro->tm_body_start);
		}
		*p_macro = macro;
		return TPP_EOK;
	}	break;

	}

	/* Initialize flags for the macro being built. */
	tpp_macro_builder_init(&builder);
#if TPP_HAVE_MACRO_FLAGS
#if TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
	if (tpp_lexer_has(self, MACRO_ARGUMENT_WHITESPACE))
		builder.mab_flags |= TPP_MACRO_FLAG_KEEPARGSPC;
#endif /* TPP_CONF_IS_RT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_RT(TPP_HAVE_MACRO_RECURSION)
	if (tpp_lexer_has(self, MACRO_RECURSION))
		builder.mab_flags |= TPP_MACRO_FLAG_SELFEXPAND;
#endif /* TPP_CONF_IS_RT(TPP_HAVE_MACRO_RECURSION) */
#endif /* TPP_HAVE_MACRO_FLAGS */

	/* Parse macro argument list */
	error = tpp_macro_builder_parse_params(&builder, self, p_pos);
	if (TPP_ISERR(error))
		goto err_builder;
	tpp_macro_builder_truncate_argv(&builder);

	/* At this point, self/p_pos point at the macro body's first token. */
	rel_body_start = tpp_file_ptr2rel(file, token->tt_start);
	rel_body_end   = rel_body_start;
	tok            = token->tt_id;

	/* Find end of body (moving the lexer to point at the trailing EOF/LF/COMMENT token) */
	while (!TPP_TOK_ISLF_OR_COMMENT_OR_EOF(tok)) {
#if TPP_HAVE_TPP_TOK_POUND_COMMENT
again_scan_end_of_macro_body:
#endif /* TPP_HAVE_TPP_TOK_POUND_COMMENT */
		/* Only update body on non-space tokens (thereby trimming trailing space/comments from macros) */
		if (!TPP_TOK_ISSPACE_OR_COMMENT(tok))
			rel_body_end = tpp_file_ptr2rel(file, *p_pos);
		tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_builder;
		}
	}

#if TPP_HAVE_TPP_TOK_POUND_COMMENT
	if (tok == TPP_TOK_POUND_COMMENT) {
		/* Deal with special case of shell comments (which must be re-parsed as a #-token) */
		*p_pos = token->tt_start + 1;
		goto again_scan_end_of_macro_body;
	}
#endif /* TPP_HAVE_TPP_TOK_POUND_COMMENT */

	/* Compile the macro according to active lexer rules */
	body_start = tpp_file_rel2ptr(file, rel_body_start);
	body_end   = tpp_file_rel2ptr(file, rel_body_end);
	tpp_file_pusheof(file);
	tpp_file_seteof(file, body_end); /* This is needed by macro compilers */
	error = tpp_macro_builder_compile(&builder, self, body_start, body_end);
	tpp_file_popeof(file);
	if (TPP_ISERR(error))
		goto err_builder;

	/* Pack the macro together... */
	macro = tpp_macro_builder_pack(&builder, self,
	                               body_start, body_end,
	                               deflc, lparen_token);
	if tpp_unlikely(!macro) {
		error = TPP_ENOMEM;
		goto err_builder;
	}

	/* At this point "macro" has inherited "builder" (so *DONT* fini it) */
	*p_macro = macro;
	return TPP_EOK;
err_builder:
	tpp_macro_builder_fini(&builder);
	return error;
}

/* Handle a "#define" directive, with "self" pointing at the macro's name-keyword
 * @return: TPP_TOK_ISERR: Error
 * @return: TPP_TOK_EOF: Success; caller should yield the next raw token */
TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_define_directive(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	TPP_REF tpp_macro *macro;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_keyword *const keyword = tpp_keywords_copybuiltin(&self->tl_kwds, token->tt_kwd);
	tpp_char const *pos = token->tt_end;
	tpp_lcinfo deflc = tpp_file_getlcinfo(file, pos);
	if tpp_unlikely(!keyword)
		goto err_nomem;
	token->tt_end = token->tt_start; /* Ensure that the macro's name stays loaded */

	/* Yield to next token.
	 * NOTE: Because this is a "raw" yield, this is *always* able to yield TPP_TOK_SPACE
	 *       and other such tokens, meaning by the time "tpp_lexer_parse_macro_definition"
	 *       is called, it points *directly* after the end of the macro's name (except that
	 *       a potential \-escaped linefeed is skipped -- as such, the next token will only
	 *       be (e.g.) `(` if that is what *immediatly* follows the keyword, *WITHOUT* any
	 *       preceding whitespace, comment, etc.) */
	tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Parse+compile the actual macro */
	error = tpp_lexer_parse_macro_definition(self, &macro, &pos, deflc);
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);

	/* Setup token such that it describes the entire macro definition (for messages) */
	token->tt_start = token->tt_end;
	token->tt_end   = pos;

	/* XXX: -Wkeyword-macro  (warn about #define-ing builtin keywords) */

	/* Store the macro definition within the keyword. */
	if (!keyword->tk_macro) {
#if TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO
		if (!TPP_TOK_ISUSERKEYWORD(keyword->tk_id) &&
		    tpp_lexer_getkeyworddefined(self, keyword)) {
			/* Warning if macro is builtin and defined */
			error = tpp_lexer_warnf(self, TPP_W_DEFINE_BUILTIN_MACRO, keyword->tk_kwd);
			if (TPP_ISERR(error)) {
				tpp_macro_decref(macro);
				return TPP_TOK_OFERR(error);
			}
		}
#endif /* TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO */
		keyword->tk_macro = macro; /* Inherit reference */
	} else {
#if TPP_HAVE_TPP_W_REDEFINE_MACRO
		if (!tpp_macro_equals(keyword->tk_macro, macro)) {
			/* Warning about macro redefinition */
			error = tpp_lexer_warnf(self, TPP_W_REDEFINE_MACRO, keyword);
			if (TPP_ISERR(error)) {
				tpp_macro_decref(macro);
				return TPP_TOK_OFERR(error);
			}
		}
#endif /* TPP_HAVE_TPP_W_REDEFINE_MACRO */
		tpp_macro_decref(keyword->tk_macro);
		keyword->tk_macro = macro; /* Inherit reference */
	}

	return TPP_TOK_EOF;
err_nomem:
	return TPP_TOK_ENOMEM;
}
#endif /* TPP_HAVE_CPP_DIRECTIVES && TPP_HAVE_CPP_DEFINE */

#if TPP_HAVE_LEXER_CLI_DEFINE


static TPP_WUNUSED TPP_NONNULL((1, 2, 4)) tpp_errno TPPCALL
tpp_lexer_define_impl(tpp_lexer *tpp_restrict self,
                      char const *macro_name, tpp_size macro_name_len,
                      char const *macro_body, tpp_size macro_body_len) {
	tpp_char const *macro_params;
	tpp_char const *pos;
	tpp_size macro_params_len;
	tpp_size def_chunk_len;
	TPP_REF tpp_string *def_chunk;
	tpp_errno error;
	TPP_REF tpp_macro *macro;
	tpp_token_id tok;
	tpp_keyword const *ro_macro_keyword;
	tpp_keyword *macro_keyword;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_init_text_utf8(file, TPP_CONFIG_CLI_FILENAME,
	                        NULL, macro_name, macro_name_len,
	                        TPP_LCINFO_INVALID, TPP_FILE_FLAGS_NORMAL);
	do {
		tok = tpp_lexer_yieldraw(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (TPP_TOK_ISKEYWORD(tok)) {
		ro_macro_keyword = tpp_lexer_gettokenkwd(self);
	} else {
		/* Shouldn't really get here, unless someone
		 * tries to do something stupid like `-D+-*=42` */
		tpp_char const *token_start = tpp_lexer_gettokenstart(self);
		tpp_size token_len = tpp_lexer_gettokenlen(self);
#if TPP_HAVE_ESCAPED_KEYWORDS
		tpp_hash hash = tpp_hashof_esc(token_start, token_len, file);
		ro_macro_keyword = tpp_lexer_kwds_newkeyword_esc(self, token_start, token_len, hash, file);
#else /* TPP_HAVE_ESCAPED_KEYWORDS */
		tpp_hash hash = tpp_hashof(token_start, token_len);
		ro_macro_keyword = tpp_lexer_kwds_newkeyword(self, token_start, token_len, hash);
#endif /* !TPP_HAVE_ESCAPED_KEYWORDS */
		if tpp_unlikely(!ro_macro_keyword)
			goto err_nomem;
	}
	macro_keyword = tpp_lexer_kwds_copybuiltin(self, ro_macro_keyword);
	if tpp_unlikely(!macro_keyword)
		goto err_nomem;

	/* The current lexer position, alongside everything from "macro_body"
	 * must now be packaged into a string-chunk, and that string chunk
	 * must act as the input file of "tpp_lexer_parse_macro_definition()" */
	macro_params     = tpp_lexer_gettokenend(self);
	macro_params_len = (tpp_size)(file->tf_end - macro_params);
	def_chunk_len    = macro_params_len + 1 + macro_body_len;
	def_chunk = tpp_string_malloc(def_chunk_len);
	if tpp_unlikely(!def_chunk)
		goto err_nomem;
	{
		tpp_char *dst = tpp_string_str(def_chunk);
		dst = (tpp_char *)tpp_mempcpy(dst, macro_params, macro_params_len * sizeof(char));
		*dst++ = ' '; /* Always have whitespace here to prevent
		               * function-style macro in case of "-DFOO=(10)" */
		tpp_memcpy(dst, macro_body, macro_body_len * sizeof(char));
		tpp_assert((dst + macro_body_len) == tpp_string_end(def_chunk));
	}

	/* Parse the *actual* macro definition */
	tpp_file_init_text_utf8(file, TPP_CONFIG_CLI_FILENAME, def_chunk,
	                        tpp_string_str(def_chunk),
	                        tpp_string_len(def_chunk),
	                        TPP_LCINFO_INVALID, TPP_FILE_FLAGS_NORMAL);
	pos = tpp_string_str(def_chunk);
	tok = tpp_lexer_yieldraw_at(self, &pos);
	if (TPP_TOK_ISERR(tok)) {
		tpp_file_fini(file);
		return TPP_TOK_ASERR(tok);
	}
	error = tpp_lexer_parse_macro_definition(self, &macro, &pos, TPP_LCINFO_INVALID);
	tpp_file_fini(file); /* Lexer core (including the file) will be restored by caller */
	if (TPP_ISERR(error))
		return error;

	/* Store macro definition */
	if (macro_keyword->tk_macro)
		tpp_macro_decref(macro_keyword->tk_macro);
	macro_keyword->tk_macro = macro; /* Inherit reference */

	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}

/* Define (or override) a macro `macro_name` with a body definition `macro_body`
 * When `macro_name` contains an opening `(` character, it, as well as `macro_body`
 * are parsed as a function-like macro. The same also goes for `{`, `[` and `<`
 * when `TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS` is enabled.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 4)) tpp_errno TPPCALL
tpp_lexer_define(tpp_lexer *tpp_restrict self,
                 char const *macro_name, tpp_size macro_name_maxlen,
                 char const *macro_body, tpp_size macro_body_maxlen) {
	tpp_errno result;
	tpp_size macro_name_len = tpp_strnlen(macro_name, macro_name_maxlen);
	tpp_size macro_body_len = tpp_strnlen(macro_body, macro_body_maxlen);
	union tpp_lexer_core saved_core = self->tl_core;
	result = tpp_lexer_define_impl(self, macro_name, macro_name_len, macro_body, macro_body_len);
	self->tl_core = saved_core;
	return result;
}

/* Delete a macro definition
 * @return: true:  Success
 * @return: false: No such macro */
TPP_IMPL TPP_NONNULL((1, 2)) bool TPPCALL
tpp_lexer_undef(tpp_lexer *tpp_restrict self,
                char const *macro_name, tpp_size macro_name_maxlen) {
	tpp_size macro_name_len = tpp_strnlen(macro_name, macro_name_maxlen);
	tpp_hash hash = tpp_hashof((tpp_char const *)macro_name, macro_name_len);
	tpp_keyword *macro_keyword = _tpp_lexer_kwds_getkeyword(self, (tpp_char const *)macro_name, macro_name_len, hash);
	if (macro_keyword && tpp_keyword_canundef(macro_keyword)) {
		tpp_keyword_undef(macro_keyword);
		return true;
	}
	return false;
}
#endif /* TPP_HAVE_LEXER_CLI_DEFINE */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_PP_DEFINE_C */
