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
#ifndef GUARD_TPP_LEXER_YIELDPP_C
#define GUARD_TPP_LEXER_YIELDPP_C 1
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

#if TPP_HAVE_TRIGRAPHS && TPP_HAVE_DIGRAPHS
#define tpp_is_start_of_hash(ch) ((ch) == '#' || (ch) == '?' || (ch) == '%')
#elif TPP_HAVE_DIGRAPHS
#define tpp_is_start_of_hash(ch) ((ch) == '#' || (ch) == '%')
#elif TPP_HAVE_TRIGRAPHS
#define tpp_is_start_of_hash(ch) ((ch) == '#' || (ch) == '?')
#else /* ... */
#define tpp_is_start_of_hash(ch) ((ch) == '#')
#endif /* !... */

#if TPP_HAVE_CPP_DIRECTIVES

#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT
#undef tpp_lexer_seek_eol__STYLE_PARAM
#undef tpp_lexer_seek_eol__STYLE_ARG
#if TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED
#define tpp_lexer_seek_eol__STYLE_PARAM  , tpp_token_id comment_style
#define tpp_lexer_seek_eol__STYLE_ARG(x) , x
#else /* TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */
#define tpp_lexer_seek_eol__STYLE_PARAM  /* nothing */
#define tpp_lexer_seek_eol__STYLE_ARG(x) /* nothing */
#endif /* !TPP_HAVE_TPP_W_LINE_COMMENT_CONTINUED */

/* From "./lexer-yieldraw.c" */
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_lexer_seek_eol(tpp_lexer *tpp_restrict self,
                   tpp_char const **tpp_restrict p_pos
                   tpp_lexer_seek_eol__STYLE_PARAM);
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING || TPP_HAVE_TPP_TOK_SHELL_COMMENT */


#if TPP_HAVE_PRAGMA
/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword
 * @return: TPP_ENOENT: Unknown pragma (warning was already emitted; caller
 *                      should seek until after macro) */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);

#if TPP_HAVE_PRAGMA_PUSH_MACRO
struct tpp_lexer_handle_pushpopmacro_data {
	tpp_lexer *tlhppmd_lexer; /* [1..1] Lexer */
	bool       tlhppmd_push;  /* True if "push_macro", false if "pop_macro" */
};

static tpp_errno TPPCALL
tpp_lexer_handle_pushpopmacro_cb(void *arg, tpp_char const *str, tpp_size length) {
	tpp_errno result;
	struct tpp_lexer_handle_pushpopmacro_data *data;
	tpp_keyword const *ro_keyword;
	tpp_keyword *keyword;
	tpp_hash hash = tpp_hashof(str, length);
	data = (struct tpp_lexer_handle_pushpopmacro_data *)arg;

	/* Load keyword */
	ro_keyword = tpp_keywords_newkeyword(&data->tlhppmd_lexer->tl_kwds, str, length, hash);
	if tpp_unlikely(!ro_keyword)
		goto err_nomem;

	/* Make keyword writable */
	keyword = tpp_keywords_copybuiltin(&data->tlhppmd_lexer->tl_kwds, ro_keyword);
	if tpp_unlikely(!keyword)
		goto err_nomem;

	/* Push/pop the macro linked to this keyword. */
	if (data->tlhppmd_push)
		return tpp_keyword_pushmacro(keyword);

	result = tpp_keyword_popmacro(keyword);
	tpp_assert(result == TPP_EOK ||
	           result == TPP_ENOENT);
	if (result == TPP_ENOENT) {
		/* Emit a warning */
#if TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK
		result = tpp_lexer_warnf(data->tlhppmd_lexer, TPP_W_POP_MACRO_EMPTY_STACK,
		                         (unsigned int)length, str);
#else /* TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
		result = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
	}
	return result;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_token_id tok = token->tt_id;
	switch (tok) {

#if TPP_HAVE_PRAGMA_PUSH_MACRO
	case TPP_KWD_push_macro:
	case TPP_KWD_pop_macro: {
		tpp_errno error;
		struct tpp_lexer_handle_pushpopmacro_data data;
		data.tlhppmd_lexer = self;
		data.tlhppmd_push  = tok == TPP_KWD_push_macro;
		if (!tpp_lexer_getext(self, TPP_EXT_PRAGMA_PUSH_MACRO))
			goto unknown_pragma;
		tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);

		/* Skip leading '(' */
		tok = tpp_lexer_skip_blocking(self, TPP_TOK_OFCHAR('('));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);

		/* Parse+process string (using "tpp_lexer_parsestring_cb()") */
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_pushpopmacro_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_STOPONLF);
		if (error != TPP_EOK)
			return error;

		/* Skip trailing ')' */
		tok = tpp_lexer_skip_blocking(self, TPP_TOK_OFCHAR(')'));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}	break;
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_PRAGMA_ONCE
	case TPP_KWD_once: {
		tpp_file const *iofile;
		tpp_keyword *iofile_kwd;
		if (!tpp_lexer_getext(self, TPP_EXT_PRAGMA_ONCE))
			goto unknown_pragma;
		iofile     = tpp_file_getiofile(tpp_lexer_getfile(self));
		iofile_kwd = tpp_file_filename_kwd(iofile);
		if (iofile_kwd) {
			tpp_keyword_misc *misc;
			misc = tpp_keyword_requiremisc(iofile_kwd);
			if tpp_unlikely(!misc)
				return TPP_ENOMEM;
			misc->tkm_flags |= TPP_KEYWORD_FLAG_HDR_ONCE;
		}
#if TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER
#if TPP_HAVE_INCLUDE_STACK
		if (iofile->tf_prev == NULL)
#endif /* TPP_HAVE_INCLUDE_STACK */
		{
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER);
			if (error != TPP_EOK)
				return error;
		}
#endif /* TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER */
		tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}	break;
#endif /* TPP_HAVE_PRAGMA_ONCE */

/* Support for: #pragma deprecated("foo") */
#ifndef TPP_HAVE_PRAGMA_DEPRECATED
#define TPP_HAVE_PRAGMA_DEPRECATED (TPP_COMMON_HAVE_PRAGMA ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_DEPRECATED */

/* Support for: #pragma GCC poison foo */
#ifndef TPP_HAVE_PRAGMA_GCC_POISON
#define TPP_HAVE_PRAGMA_GCC_POISON (TPP_COMMON_HAVE_PRAGMA_GCC ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_GCC_POISON */

/* Support for: #pragma tpp_set_keyword_flags */
#ifndef TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS (TPP_COMMON_HAVE_PRAGMA ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

/* Support for: #pragma GCC system_header */
#ifndef TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
#define TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER (TPP_COMMON_HAVE_PRAGMA_GCC ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

/* Support for: #pragma extension(...) */
#ifndef TPP_HAVE_PRAGMA_EXTENSION
#define TPP_HAVE_PRAGMA_EXTENSION ((TPP_COMMON_HAVE_PRAGMA && TPP_HAVE_EXTENSIONS) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_EXTENSION */

/* Support for: #pragma extension(push) */
#ifndef TPP_HAVE_PRAGMA_EXTENSION_PUSH
#define TPP_HAVE_PRAGMA_EXTENSION_PUSH ((TPP_HAVE_PRAGMA_EXTENSION && TPP_HAVE_EXTENSIONS_PUSH_POP) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_EXTENSION_PUSH */

/* Support for: #pragma warning(...) */
#ifndef TPP_HAVE_PRAGMA_WARNING
#define TPP_HAVE_PRAGMA_WARNING ((TPP_COMMON_HAVE_PRAGMA && TPP_HAVE_WARNINGS) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_WARNING */

/* Support for: #pragma warning(push) */
#ifndef TPP_HAVE_PRAGMA_WARNING_PUSH
#define TPP_HAVE_PRAGMA_WARNING_PUSH ((TPP_HAVE_PRAGMA_WARNING && TPP_HAVE_WARNINGS_PUSH_POP) ? -1 : 0)
#endif /* !TPP_HAVE_PRAGMA_WARNING_PUSH */

	default: goto unknown_pragma;
	}
	return TPP_EOK;
#if TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS
	{
		tpp_errno error;
unknown_pragma:
		error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_PRAGMAS);
		if (error == TPP_EOK)
			error = TPP_ENOENT;
		return error;
	}
#else /* TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
unknown_pragma:
	return TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
}

/* Process a pragma directive, starting after the "TPP_KWD_pragma" keyword */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_pragma_directive(tpp_lexer *tpp_restrict self) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_errno error = tpp_lexer_process_pragma(self);
	if (error != TPP_EOK) {
		if (error == TPP_ENOENT)
			goto skip_garbage_without_warning;
		return TPP_TOK_OFERR(error);
	}
	while (TPP_TOK_ISSPACE_OR_COMMENT(token->tt_id)) {
		tpp_token_id tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
	}
	if (TPP_TOK_ISLF_OR_COMMENT(token->tt_id))
		return TPP_TOK_EOF;
	if (token->tt_id == TPP_TOK_EOF)
		return TPP_TOK_EOF;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
	error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE);
	if (error != TPP_EOK)
		return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE */
skip_garbage_without_warning:
	for (;;) {
		tpp_token_id tok = tpp_lexer_yieldraw_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
		if (TPP_TOK_ISLF_OR_COMMENT(tok))
			break;
	}
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_PRAGMA */

#if TPP_HAVE_CPP_DEFINE
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
	tpp_size            mab_skiptotal;   /* Max amount of characters skipped during expansion. */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
	tpp_size            mab_n_vaopt;     /* Amount of extra bytes inserted when varargs are given (if: tpp_lexer_seek_rparen:OUT(*p_argc) > tmf_argc). */
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
	 tpp_macro_free((self)->mab_macro))

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

#if (TPP_HAVE_TPP_TOK_RANGLE_RANGLE ||        \
     TPP_HAVE_TPP_TOK_RANGLE_EQUAL ||         \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL ||  \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE || \
     TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
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
	tpp_file const *const file = tpp_lexer_getfile(self);
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
		if (!tpp_lexer_getext(self, TPP_EXT_VA_ARGS_IN_MACROS))
			break;
		tok = TPP_KWD___VA_ARGS__;
#if TPP_DEBUG
		/* Needed for "tpp_macro_argument::tma_name" */
		token->tt_kwd = tpp_builtin_getkeyword_byid(TPP_KWD___VA_ARGS__);
		tpp_assert(token->tt_kwd != NULL);
#endif /* TPP_DEBUG */
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
		if (error != TPP_EOK)
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
			if (error != TPP_EOK)
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
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
		arg->tma_ins = 0;
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT */
#if TPP_DEBUG
		tpp_assert(token->tt_kwd);
		arg->tma_name = token->tt_kwd->tk_kwd;
#endif /* TPP_DEBUG */
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
	    tpp_lexer_getext(self, TPP_EXT_NAMED_VARARGS_IN_MACROS)) {
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
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		return TPP_EOK;
	}

warn_unexpected_parameter_list_token:
	/* Warning: expected keyword in function-style macro argument list */
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_MACRO_PARAMETER_LIST
	{
		tpp_errno error;
		tpp_char const *saved_end = token->tt_end;
		token->tt_end = *p_pos;
		error = tpp_lexer_warnf_at(self, tpp_file_rel2ptr(file, rel_start),
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
		new_macro = tpp_macro_tryrealloc_function(self->mab_macro, new_alloc);
		if tpp_unlikely(!new_macro) {
			new_alloc = min_alloc;
			new_macro = tpp_macro_realloc_function(self->mab_macro, new_alloc);
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

#if TPP_HAVE_TRADITIONAL_MACROS != 0
/* Compile a traditional macro (allowed to clobber the token in "builder") */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile_traditional(tpp_macro_builder *tpp_restrict builder,
                                      tpp_lexer *tpp_restrict self,
                                      tpp_char const *body_start,
                                      tpp_char const *body_end) {
	tpp_macro_opcode *opcodes;
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
		tok = tpp_lexer_yieldraw_at(self, &body_iter);
		switch (tok) {

#if TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
		TPP_CASE_TPP_TOK_COMMENT_NOLINE {
			if (TPP_TOK_ISCOMMENT_NOLINE(tok)) {
				/* Non-line comments must be deleted in order to support traditional cat operations!
				 * Also note that line-comments shouldn't be present at all (since those should have
				 * caused our caller to terminate the macro body, in case you're wondering) */
				if (token->tt_start > body_start) {
					opcodes = tpp_macro_builder_requireop(builder, 2);
					if tpp_unlikely(!opcodes)
						goto err_nomem;
					opcodes[0] = TPP_MACRO_OPCODE_COPY;
					opcodes[1] = (tpp_size)(token->tt_start - body_start);
				}
				builder->mab_skiptotal += (tpp_size)(body_iter - token->tt_start);
				body_start = body_iter;
			}
		}	continue; /* Not a keyword */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE */

#if TPP_HAVE_TPP_TOK_STRINGLIKE
		/* Strings must not actually be parsed as whole tokens!
		 *
		 * Since this can (easily) cause warnings to be emitted
		 * (~ala "string terminated by eol"), our caller has
		 * disabled them for us!
		 *
		 * This is needed for stuff like:
		 * >> #define str(x) "x"
		 */
		TPP_CASE_TPP_TOK_STRING {
			body_iter = token->tt_start + 1;
			continue;
		}
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE */

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
			opcodes = tpp_macro_builder_requireop(builder, 2);
			if tpp_unlikely(!opcodes)
				goto err_nomem;
			opcodes[0] = TPP_MACRO_OPCODE_COPY;
			opcodes[1] = (tpp_size)(token->tt_start - body_start);
		}

		/* Append opcodes to insert argument */
		opcodes = tpp_macro_builder_requireop(builder, 3);
		if tpp_unlikely(!opcodes)
			goto err_nomem;
		opcodes[0] = TPP_MACRO_OPCODE_INS_EXP;
		opcodes[1] = (tpp_size)(arg - builder->mab_argv);
		opcodes[2] = (tpp_size)(body_iter - token->tt_start);

		/* Account for expansion in size trackings. */
		builder->mab_skiptotal += (tpp_size)(body_iter - token->tt_start);
		++arg->tma_ins_exp;

		/* Remember that input body text has been
		 * flushed until the end of the keyword. */
		body_start = body_iter;
	}

	/* Append opcodes to copy remainder. */
	if (body_start < body_end) {
		opcodes = tpp_macro_builder_requireop(builder, 2);
		if tpp_unlikely(!opcodes)
			goto err_nomem;
		opcodes[0] = TPP_MACRO_OPCODE_COPY;
		opcodes[1] = (tpp_size)(body_end - body_start);
	}

	/* Terminate body builder (*flexes muscles*) */
	opcodes = tpp_macro_builder_requireop(builder, 1);
	if tpp_unlikely(!opcodes)
		goto err_nomem;
	opcodes[0] = TPP_MACRO_OPCODE_END;
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}
#endif /* TPP_HAVE_TRADITIONAL_MACROS != 0 */

#if TPP_HAVE_TRADITIONAL_MACROS <= 0
/* Compile a modern macro (allowed to clobber the token in "builder") */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile_modern(tpp_macro_builder *tpp_restrict builder,
                                 tpp_lexer *tpp_restrict self,
                                 tpp_char const *body_start,
                                 tpp_char const *body_end) {
	tpp_macro_opcode *opcodes;
	(void)self;
	/* TODO */

	opcodes = tpp_macro_builder_requireop(builder, 3);
	if tpp_unlikely(!opcodes)
		return TPP_ENOMEM;
	opcodes[0] = TPP_MACRO_OPCODE_COPY;
	opcodes[1] = (tpp_size)(body_end - body_start);
	opcodes[2] = TPP_MACRO_OPCODE_END;
	return TPP_EOK;
}
#endif /* TPP_HAVE_TRADITIONAL_MACROS <= 0 */


#if TPP_HAVE_TRADITIONAL_MACROS < 0
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_builder_compile(tpp_macro_builder *tpp_restrict builder,
                          tpp_lexer *tpp_restrict self,
                          tpp_char const *body_start,
                          tpp_char const *body_end) {
	if (tpp_lexer_getext(self, TPP_EXT_TRADITIONAL_MACROS))
		return tpp_macro_builder_compile_traditional(builder, self, body_start, body_end);
	return tpp_macro_builder_compile_modern(builder, self, body_start, body_end);
}
#elif TPP_HAVE_TRADITIONAL_MACROS == 0
#define tpp_macro_builder_compile(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_modern(builder, self, body_start, body_end)
#else /* TPP_HAVE_TRADITIONAL_MACROS > 0 */
#define tpp_macro_builder_compile(builder, self, body_start, body_end) \
	tpp_macro_builder_compile_traditional(builder, self, body_start, body_end)
#endif /* TPP_HAVE_TRADITIONAL_MACROS... */

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
		result = tpp_macro_malloc_function(1); /* 1: TPP_MACRO_OPCODE_END */
		if tpp_unlikely(!result)
			return NULL;
		result->tm_data.tmd_func.tmf_expand[0] = TPP_MACRO_OPCODE_END;
	} else
#ifndef __OPTIMIZE_SIZE__
	if (self->mab_tmf_expandc < self->mab_tmf_expanda) {
		TPP_REF tpp_macro *new_result;
		new_result = tpp_macro_tryrealloc_function(result, self->mab_tmf_expandc);
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
	result->tm_deffile    = tpp_file_filename(file);
	if (result->tm_deffile) {
		result->tm_deflc   = deflc;
		result->tm_body_lc = tpp_file_lcinfo(file, body_start);
	}
	result->tm_data.tmd_func.tmf_argc    = self->mab_argc;
	result->tm_data.tmd_func.tmf_argv    = self->mab_argv; /* Inherit data */
	result->tm_data.tmd_func.tmf_expbase = (tpp_size)(body_end - body_start);
	tpp_assert(result->tm_data.tmd_func.tmf_expbase >= self->mab_skiptotal);
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
		if (tpp_lexer_getext(self, TPP_EXT_ALTERNATIVE_MACRO_PARENTHESIS))
			break;
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */
		/* FALLTHRU */
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
		while (!TPP_TOK_ISLF_OR_COMMENT(tok) && tok != TPP_TOK_EOF) {
			rel_body_end = tpp_file_ptr2rel(file, *p_pos);
			tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}

		/* Allocate keyword-style macro definition */
		macro = tpp_macro_malloc_keyword();
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
		macro->tm_deffile = tpp_file_filename(file);
		if (macro->tm_deffile) {
			macro->tm_deflc   = deflc;
			macro->tm_body_lc = tpp_file_lcinfo(file, macro->tm_body_start);
		}
		*p_macro = macro;
		return TPP_EOK;
	}	break;

	}

	/* Initialize flags for the macro being built. */
	tpp_macro_builder_init(&builder);
#if TPP_HAVE_MACRO_FLAGS
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
	if (tpp_lexer_getext(self, TPP_EXT_MACRO_ARGUMENT_WHITESPACE))
		builder.mab_flags |= TPP_MACRO_FLAG_KEEPARGSPC;
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */
#if TPP_HAVE_MACRO_RECURSION < 0
	if (tpp_lexer_getext(self, TPP_EXT_MACRO_RECURSION))
		builder.mab_flags |= TPP_MACRO_FLAG_SELFEXPAND;
#endif /* TPP_HAVE_MACRO_RECURSION < 0 */
#endif /* TPP_HAVE_MACRO_FLAGS */

	/* Parse macro argument list */
	error = tpp_macro_builder_parse_params(&builder, self, p_pos);
	if (error != TPP_EOK)
		goto err_builder;
	tpp_macro_builder_truncate_argv(&builder);

	/* At this point, self/p_pos point at the macro body's first token. */
	rel_body_start = tpp_file_ptr2rel(file, token->tt_start);
	rel_body_end   = rel_body_start;
	tok            = token->tt_id;

	/* Find end of body (moving the lexer to point at the trailing EOF/LF/COMMENT token) */
	while (tok != TPP_TOK_EOF && !TPP_TOK_ISLF_OR_COMMENT(tok)) {
		rel_body_end = tpp_file_ptr2rel(file, *p_pos);
		tok = tpp_lexer_yieldraw_at_blocking(self, p_pos);
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
			goto err_builder;
		}
	}

	/* Compile the macro according to active lexer rules */
	body_start = tpp_file_rel2ptr(file, rel_body_start);
	body_end   = tpp_file_rel2ptr(file, rel_body_end);
	tpp_file_pusheof_fast(file, body_end); /* This is needed by macro compilers */
#if TPP_HAVE_WARNINGS
	tpp_lexer_state_push(self, ~0, TPP_LEXER_STATE_FLAG_NOWARNINGS);
#endif /* TPP_HAVE_WARNINGS */
	error = tpp_macro_builder_compile(&builder, self, body_start, body_end);
#if TPP_HAVE_WARNINGS
	tpp_lexer_state_pop(self, ~0, TPP_LEXER_STATE_FLAG_NOWARNINGS);
#endif /* TPP_HAVE_WARNINGS */
	tpp_file_popeof_fast(file);
	if (error != TPP_EOK)
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
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_define_directive(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	TPP_REF tpp_macro *macro;
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_keyword *const keyword = tpp_keywords_copybuiltin(&self->tl_kwds, token->tt_kwd);
	tpp_char const *pos = token->tt_end;
	tpp_lcinfo deflc = tpp_file_lcinfo(file, pos);
	if tpp_unlikely(!keyword)
		goto err_nomem;
	token->tt_end = token->tt_start; /* Ensure that the macro's name stays loaded */
	token->tt_end = pos;
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	} while (TPP_TOK_ISCOMMENT_NOLINE(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Parse+compile the actual macro */
	error = tpp_lexer_parse_macro_definition(self, &macro, &pos, deflc);
	if tpp_unlikely(error != TPP_EOK)
		return TPP_TOK_OFERR(error);

	/* Setup token such that it describes the entire macro definition (for messages) */
	token->tt_start = token->tt_end;
	token->tt_end   = pos;

	/* Store the macro definition within the keyword. */
	if (!keyword->tk_macro) {
#if TPP_HAVE_TPP_W_DEFINE_BUILTIN_MACRO
		if (!TPP_TOK_ISUSERKEYWORD(keyword->tk_id) &&
		    tpp_lexer_getkeyworddefined(self, keyword)) {
			/* Warning if macro is builtin and defined */
			error = tpp_lexer_warnf_at(self, token->tt_end,
			                           TPP_W_DEFINE_BUILTIN_MACRO,
			                           keyword->tk_kwd);
			if (error != TPP_EOK) {
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
			error = tpp_lexer_warnf_at(self, token->tt_end,
			                           TPP_W_REDEFINE_MACRO,
			                           keyword);
			if (error != TPP_EOK) {
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
#endif /* TPP_HAVE_CPP_DEFINE */


/* Process a preprocessor directive, with the currently loaded token being the leading '#'
 * Upon successful return (!TPP_TOK_ISERR(return)), the caller will yield another raw token
 * @return: TPP_TOK_ISERR         : Error
 * @return: TPP_TOK_EOF           : Caller should yield the next raw token
 * @return: TPP_TOK_SHELL_COMMENT : Directive was transformed to a shell-comment which the caller should re-emit */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_process_directive(tpp_lexer *tpp_restrict self) {
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token_id result;
	tpp_char const *directive_iter;
	tpp_assert(token->tt_id == '#');
	directive_iter = token->tt_end;
	file->tf_pos = token->tt_start; /* Make sure that the start of the directive remains loaded. */

	/* Load token that comes after leading '#' */
again_yield_directive_iter:
	result = tpp_lexer_yieldraw_at_blocking(self, &directive_iter);
	switch (result) {

#if TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		/* Skip over multi-line comment tokens to allow code like:
		 * >> #(*
		 * >> comment
		 * >> *)define foo 42 */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_NOLINE */
	case TPP_TOK_SPACE:
		goto again_yield_directive_iter;

/************************************************************************/
#if TPP_HAVE_CPP_BLANK
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE
	TPP_CASE_TPP_TOK_COMMENT_LINE
		/* line-like comment tokens must be treated as the "blank" directive,
		 * since they effective cap-off the directive via a commented line-feed */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE */
	case TPP_TOK_EOF:
	case TPP_TOK_LF:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_BLANK))
			goto handle_unknown_directive;
		break;
#endif /* TPP_HAVE_CPP_BLANK */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_EXCLAIM
	case '!':
		if (!tpp_lexer_getext(self, TPP_EXT_CPP_EXCLAIM))
			goto handle_unknown_directive;
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_EXCLAIM */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIGIT_LINE
	case TPP_TOK_INT:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DIGIT_LINE))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_DIGIT_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_LINE
	case TPP_KWD_line:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_LINE))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_LINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT
	{
#if TPP_HAVE_CPP_INCLUDE
		if (0) {
	case TPP_KWD_include:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_INCLUDE))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_INCLUDE */
#if TPP_HAVE_CPP_INCLUDE_NEXT
		if (0) {
	case TPP_KWD_include_next:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_INCLUDE_NEXT))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
#if TPP_HAVE_CPP_IMPORT
		if (0) {
	case TPP_KWD_import:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_IMPORT))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_IMPORT */

		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}
#endif /* TPP_HAVE_CPP_INCLUDE || TPP_HAVE_CPP_INCLUDE_NEXT || TPP_HAVE_CPP_IMPORT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IF_ELSE_ENDIF
	case TPP_KWD_if:
	case TPP_KWD_elif:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line

	case TPP_KWD_ifdef:
	case TPP_KWD_ifndef:
	case TPP_KWD_elifdef:
	case TPP_KWD_elifndef:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line

	case TPP_KWD_else:
	case TPP_KWD_endif:
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_IF_ELSE_ENDIF))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DEFINE
	case TPP_KWD_define: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DEFINE))
			goto handle_unknown_directive;
		token->tt_end = directive_iter;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (!TPP_TOK_ISKEYWORD(result)) {
#if TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE, "define");
			if (error != TPP_EOK)
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
			goto seek_end_of_line;
#define WANT_seek_end_of_line
		}
		return tpp_lexer_process_define_directive(self);
	}	break;

	case TPP_KWD_undef: {
		tpp_keyword const *ro_keyword;
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DEFINE))
			goto handle_unknown_directive;
		token->tt_end = directive_iter;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (!TPP_TOK_ISKEYWORD(result)) {
#if TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE, "undef");
			if (error != TPP_EOK)
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_MACRO_NAME_IN_DIRECTIVE */
			goto seek_end_of_line;
#define WANT_seek_end_of_line
		}

		/* Delete keyword definition */
		ro_keyword = tpp_lexer_gettoken(self)->tt_kwd;
		if (ro_keyword->tk_macro) {
			tpp_keyword *keyword = tpp_keywords_copybuiltin(&self->tl_kwds, ro_keyword);
			if tpp_unlikely(!keyword)
				return TPP_TOK_ENOMEM;
			tpp_assert(keyword->tk_macro);
			tpp_macro_decref(keyword->tk_macro);
			keyword->tk_macro = NULL;
		} else
#if TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO
		if (tpp_lexer_getkeyworddefined(self, ro_keyword)) {
			/* Builtin keyword... */
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_CANNOT_UNDEF_BUILTIN_MACRO);
			if (error != TPP_EOK)
				return TPP_TOK_OFERR(error);
		} else
#endif /* TPP_HAVE_TPP_W_CANNOT_UNDEF_BUILTIN_MACRO */
		{
		}

		/* Seek to next token (which should be a line-feed) */
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		if (TPP_TOK_ISLF_OR_COMMENT(result))
			break;
		if (result == TPP_TOK_EOF)
			break;
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE
		{
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE, "undef");
			if (error != TPP_EOK)
				return TPP_TOK_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_DIRECTIVE */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
	}	break;
#endif /* TPP_HAVE_CPP_DEFINE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ASSERT
	case TPP_KWD_assert:
	case TPP_KWD_unassert:
		if (!tpp_lexer_getext(self, TPP_EXT_CPP_ASSERT))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_ASSERT */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING
	{
		tpp_size rel_token_start;
		tpp_size rel_message_start;
		tpp_size rel_message_end;
		tpp_char const *message_start;
		tpp_char const *message_end;
		tpp_size message_size;
		tpp_errno error;
#if TPP_HAVE_CPP_ERROR
		if (0) {
	case TPP_KWD_error:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_ERROR))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_ERROR */
#if TPP_HAVE_CPP_WARNING
		if (0) {
	case TPP_KWD_warning:
			if (!tpp_lexer_getext(self, TPP_EXT_CPP_WARNING))
				goto handle_unknown_directive;
		}
#endif /* TPP_HAVE_CPP_WARNING */
		rel_token_start   = tpp_file_ptr2rel(file, token->tt_start);
		rel_message_start = tpp_file_ptr2rel(file, directive_iter);
		error = tpp_lexer_seek_eol(self, &directive_iter tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_EOF));
		rel_message_end = tpp_file_ptr2rel(file, directive_iter);
		token->tt_start = tpp_file_rel2ptr(file, rel_token_start);
		if (error != TPP_EOK)
			return TPP_TOK_OFERR(error);

		/* Load range of message string. */
		message_start = tpp_file_rel2ptr(file, rel_message_start);
		message_end   = tpp_file_rel2ptr(file, rel_message_end);

		/* Remember that this is where the next token should begin. */
		token->tt_end = directive_iter;

		/* Strip leading/trailing whitespace */
#if TPP_HAVE_UNICODE
		if (tpp_file_isutf8(tpp_lexer_getfile(self))) {
			while (message_start < message_end) {
				tpp_char const *nstart = message_start;
				tpp_unichar uc = tpp_unicode_readutf8(&nstart, message_end);
				if (!tpp_unicode_isspace(uc))
					break;
				message_start = nstart;
			}
			while (message_start < message_end) {
				tpp_char const *nend = message_end;
				tpp_unichar uc = tpp_unicode_readutf8_rev(&nend, message_start);
				if (!tpp_unicode_isspace(uc))
					break;
				message_end = nend;
			}
		} else
#endif /* TPP_HAVE_UNICODE */
		{
			while (message_start < message_end && tpp_ascii_isspace(*message_start))
				++message_start;
			while (message_start < message_end && tpp_ascii_isspace(message_end[-1]))
				--message_end;
		}

		/* Emit error/warning message */
		message_size = (tpp_size)(message_end - message_start);
		switch (result) {
		case TPP_KWD_error:
#if TPP_HAVE_TPP_W_ERROR
			error = tpp_lexer_warnf(self, TPP_W_ERROR, (unsigned int)message_size, message_start);
#endif /* TPP_HAVE_TPP_W_ERROR */
			break;
		case TPP_KWD_warning:
#if TPP_HAVE_TPP_W_WARNING
			error = tpp_lexer_warnf(self, TPP_W_WARNING, (unsigned int)message_size, message_start);
#endif /* TPP_HAVE_TPP_W_WARNING */
			break;
		default: tpp_unreachable();
		}
		if (error != TPP_EOK)
			return TPP_TOK_OFERR(error);
		break;
	}
#endif /* TPP_HAVE_CPP_ERROR || TPP_HAVE_CPP_WARNING */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_IDENT_SCSS
	case TPP_KWD_ident:
	case TPP_KWD_scss:
		if (!tpp_lexer_getext(self, TPP_EXT_CPP_IDENT_SCSS))
			goto handle_unknown_directive;
		/* TODO */
		goto seek_end_of_line;
#define WANT_seek_end_of_line
#endif /* TPP_HAVE_CPP_IDENT_SCSS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_PRAGMA
	case TPP_KWD_pragma: {
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_PRAGMA))
			goto handle_unknown_directive;
		token->tt_end = directive_iter;
		do {
			result = tpp_lexer_yieldraw_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_COMMENT(result));
		if (TPP_TOK_ISERR(result))
			return result;
		return tpp_lexer_process_pragma_directive(self);
	}	break;
#endif /* TPP_HAVE_CPP_PRAGMA */
/************************************************************************/



	default:
		if (TPP_TOK_ISERR(result))
			return result;
handle_unknown_directive:
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SHELL_COMMENT)
#if TPP_HAVE_DIGRAPHS
		    && *file->tf_pos != '%' /* Digraph "%:" must never become a shell comment (since
		                             * "%:" is a **TOKEN** substitution, but not a **CHARACTER**
		                             * substitution like "??=" is) */
#endif /* TPP_HAVE_DIGRAPHS */
		    ) {
			/* Seek next end-of-line character (rather than next end-of-line token)
			 * The difference here comes into play when considering that multi-line
			 * comment tokens can disguise line-feed tokens, but since we're to
			 * treat the #-line as a comment on its own, that means that we're
			 * already *within* a comment (semantically speaking), and comment
			 * nesting isn't something that's allowed! */
			tpp_errno error;
			tpp_char const *eol;
			/* "file->tf_pos" was saved as the start of the '#' */
			tpp_assert(tpp_is_start_of_hash(*file->tf_pos));
			token->tt_start = eol = file->tf_pos;
			error = tpp_lexer_seek_eol(self, &eol tpp_lexer_seek_eol__STYLE_ARG(TPP_TOK_SHELL_COMMENT));
			if (error != TPP_EOK) {
				token->tt_start = file->tf_pos;
				token->tt_end = file->tf_pos + 1;
#if TPP_HAVE_TRIGRAPHS
				if (*file->tf_pos == '?') {
					token->tt_end += 2;
				} else
#endif /* TPP_HAVE_TRIGRAPHS */
#if TPP_HAVE_DIGRAPHS
				if (*file->tf_pos == '%') {
					token->tt_end += 1;
				} else
#endif /* TPP_HAVE_DIGRAPHS */
				{
				}
				token->tt_id = (tpp_token_id)'#';
				self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
				return TPP_TOK_OFERR(error);
			}
			token->tt_end = eol;
			token->tt_id = TPP_TOK_SHELL_COMMENT;

			/* Tell caller to re-emit what we thought was a directive as a shell comment */
			return TPP_TOK_SHELL_COMMENT;
		} else
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		{
#if defined(WANT_seek_end_of_line) || TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0
#if TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE
			tpp_errno error;
			error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_DIRECTIVE);
			if (error != TPP_EOK)
				return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_UNKNOWN_DIRECTIVE */
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0 */
	
			/* Seek until we hit LF or EOF. Caller has disabled "autopopfile",
			 * so this'll always stay within the file currently being processed. */
#ifdef WANT_seek_end_of_line
#undef WANT_seek_end_of_line
seek_end_of_line:
#endif /* WANT_seek_end_of_line */
			while (!TPP_TOK_ISLF_OR_COMMENT(result) && result != TPP_TOK_EOF) {
				result = tpp_lexer_yieldraw_blocking(self);
				if (TPP_TOK_ISERR(result))
					break;
			}
#endif /* WANT_seek_end_of_line || TPP_HAVE_TPP_TOK_SHELL_COMMENT <= 0 */
		}
		break;
	}
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_CPP_DIRECTIVES */



/* Wrapper around `tpp_lexer_yieldraw()' that filters certain tokens (based on
 * configured features), and implements handling for preprocessor directives,
 * like "#define", "#include", etc:
 * - TPP_TOK_LF:      Filtered based on `TPP_HAVE_TPP_TOK_LF' / `TPP_FEAT_TPP_TOK_LF'
 * - TPP_TOK_SPACE:   Filtered based on `TPP_HAVE_TPP_TOK_SPACE' / `TPP_FEAT_TPP_TOK_SPACE'
 * - TPP_TOK_COMMENT: Filtered based on `TPP_HAVE_TPP_TOK_COMMENT' / `TPP_FEAT_TPP_TOK_COMMENT'
 *
 * @return: * :                  The newly read token (after accounting for preprocessor directives)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldraw(self);
	switch (result) {


/************************************************************************/
#if TPP_HAVE_TPP_TOK_SPACE <= 0 /* Always, or conditionally disabled */
	case TPP_TOK_SPACE:
#if TPP_HAVE_TPP_TOK_SPACE < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_SPACE))
			break; /* Enabled */
#endif /* TPP_HAVE_TPP_TOK_SPACE < 0 */
		goto again;
#elif TPP_HAVE_CPP_DIRECTIVES
	case TPP_TOK_SPACE:
		break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_TPP_TOK_COMMENTLIKE
	_TPP_CASE_TPP_TOK_SHELL_COMMENT
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT && TPP_HAVE_CPP_DIRECTIVES
		if (!(self->tl_state & TPP_LEXER_STATE_FLAG_NODIRECTIVES) &&
			tpp_lexer_getfeat(self, TPP_FEAT_CPP_DIRECTIVES)) {
			tpp_token *const token = tpp_lexer_gettoken(self);

			/* Must re-parse comment as a preprocessor directive instead! */
			token->tt_id  = (tpp_token_id)'#';
			tpp_assert(tpp_is_start_of_hash(*token->tt_start));
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

			tpp_lexer_autopopfile_pushoff(self);
			result = tpp_lexer_process_directive(self);
			tpp_lexer_autopopfile_pop(self);
			if (TPP_TOK_ISERR(result))
				break;
			if (result == TPP_TOK_EOF)
				goto again;

			/* Deal with case where PP-directive wasn't recognized,
			 * and should thus be emitted as shell-comment token. */
			tpp_assert(result == TPP_TOK_SHELL_COMMENT);

			/* Fallthru to regular maybe-emit-comment code below... */
		}
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT && TPP_HAVE_CPP_DIRECTIVES */
		/* FALLTHRU */
#if TPP_HAVE_TPP_TOK_COMMENT <= 0 /* Always, or conditionally disabled */
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_ASM_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
#if TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE_LINE && TPP_HAVE_CPP_DIRECTIVES */
		/* FALLTHRU */
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
#if TPP_HAVE_TPP_TOK_COMMENT < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COMMENT))
			break; /* Comments are enabled -> emit to caller */
#endif /* TPP_HAVE_TPP_TOK_COMMENT < 0 */
		goto again;
#elif TPP_HAVE_CPP_DIRECTIVES
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_ASM_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
	TPP_CASE_TPP_TOK_COMMENT_NOLINE
		break;
#endif /* ... */
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIRECTIVES || TPP_HAVE_TPP_TOK_LF <= 0 /* Always, or conditionally disabled */
	case TPP_TOK_LF: {
#if TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen a linefeed. */
		self->tl_state &= ~TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#if TPP_HAVE_TPP_TOK_LF <= 0 /* Always, or conditionally disabled */
#if TPP_HAVE_TPP_TOK_LF < 0
		if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_LF))
			break; /* Enabled */
#endif /* TPP_HAVE_TPP_TOK_LF < 0 */
		goto again;
#else /* TPP_HAVE_TPP_TOK_LF <= 0 */
		break;
#endif /* TPP_HAVE_TPP_TOK_LF > 0 */
	}
#endif /* TPP_HAVE_CPP_DIRECTIVES || TPP_HAVE_TPP_TOK_LF <= 0 */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_CPP_DIRECTIVES
	case '#':
		if (self->tl_state & TPP_LEXER_STATE_FLAG_NODIRECTIVES)
			break; /* Not allowed here... */
		if (!tpp_lexer_getfeat(self, TPP_FEAT_CPP_DIRECTIVES))
			break; /* Directives are disabled. */
		tpp_lexer_autopopfile_pushoff(self);
		result = tpp_lexer_process_directive(self);
		tpp_lexer_autopopfile_pop(self);
		if (TPP_TOK_ISERR(result))
			break;
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT
		if (result != TPP_TOK_EOF) {
			/* Emit as a shell-comment (if enabled). Otherwise, check next raw token. */
			tpp_assert(result == TPP_TOK_SHELL_COMMENT);
			if (tpp_lexer_getfeat(self, TPP_FEAT_TPP_TOK_COMMENT))
				break;
		}
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT */
		goto again;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
/************************************************************************/



/************************************************************************/
	default:
#if TPP_HAVE_CPP_DIRECTIVES
		/* Remember that we've seen something that will prevent CPP directives */
		self->tl_state |= TPP_LEXER_STATE_FLAG_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
		break;
/************************************************************************/
	}
	return result;
}


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELDPP_C */
