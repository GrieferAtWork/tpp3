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
#ifndef GUARD_TPP_LEXER_DUMP_C
#define GUARD_TPP_LEXER_DUMP_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "warnings.h"
#include "lexer.h"
#include "macro.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_DUMP_DEFINITIONS

#define tpp_do(err, expr)                    \
	do {                                     \
		if tpp_unlikely((temp = (expr)) < 0) \
			goto err;                        \
		result += temp;                      \
	} while (0)

typedef struct tpp_lexer_dumper {
	tpp_lexer        *tld_lexer;   /* [1..1][const] Lexer */
	tpp_formatprinter tld_printer; /* [1..1][const] Printer */
	void             *tld_arg;     /* [?..?][const] Cookie for "tld_printer" */
	tpp_ssize         tld_result;  /* Accumulated results of "tld_printer", or negative on error */
	unsigned int      tld_what;    /* What to print (set of `TPP_LEXER_DUMP_DEFINITIONS_*') */
#if TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION
#if TPP_HAVE_CPP_MACROS
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
	bool tld_enabled_keepargspc;
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION)
	bool tld_enabled_macro_recursion;
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION) */
#endif /* TPP_HAVE_CPP_MACROS */
#endif /* TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION */
} tpp_lexer_dumper;



#define tpp_lexer_dumper_haserr(self) ((self)->tld_result < 0)
#define tpp_lexer_dumper_do(self, expr)       \
	do {                                      \
		if (!tpp_lexer_dumper_haserr(self)) { \
			tpp_ssize _temp = (expr);         \
			if tpp_unlikely(_temp < 0) {      \
				(self)->tld_result = _temp;   \
			} else {                          \
				(self)->tld_result += _temp;  \
			}                                 \
		}                                     \
	} while (0)

#define tpp_lexer_dumper_getprinter(self) (self)->tld_printer
#define tpp_lexer_dumper_getarg(self)     (self)->tld_arg

#if 1 /* Extract common functions to stand-along functions to prevent code bloat */
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_do_print_conststr(tpp_lexer_dumper *tpp_restrict self,
                                   char const *const_STR) {
	tpp_lexer_dumper_do(
	self, tpp_formatprinter_print_cstr(tpp_lexer_dumper_getprinter(self),
	                                   tpp_lexer_dumper_getarg(self),
	                                   const_STR, tpp_strlen(const_STR)));
}
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_do_print(tpp_lexer_dumper *tpp_restrict self,
                          tpp_char const *text, tpp_size num_bytes) {
	tpp_lexer_dumper_do(
	self, tpp_formatprinter_print(tpp_lexer_dumper_getprinter(self),
	                              tpp_lexer_dumper_getarg(self),
	                              text, num_bytes));
}

#ifdef tpp_formatprinter_print_cstr_IS_DEFAULT
#define tpp_lexer_dumper_do_print_cstr tpp_lexer_dumper_do_print_cstr
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_do_print_cstr(tpp_lexer_dumper *tpp_restrict self,
                               char const *text, tpp_size num_bytes) {
	tpp_lexer_dumper_do(
	self, tpp_formatprinter_print_cstr(tpp_lexer_dumper_getprinter(self),
	                                   tpp_lexer_dumper_getarg(self),
	                                   text, num_bytes));
}
#endif /* tpp_formatprinter_print_cstr_IS_DEFAULT */
#else
#define tpp_lexer_dumper_do_print_conststr(self, STR)                         \
	tpp_lexer_dumper_do(                                                      \
	self, tpp_formatprinter_print_conststr(tpp_lexer_dumper_getprinter(self), \
	                                       tpp_lexer_dumper_getarg(self),     \
	                                       STR))
#define tpp_lexer_dumper_do_print(self, text, num_bytes)             \
	tpp_lexer_dumper_do(                                             \
	self, tpp_formatprinter_print(tpp_lexer_dumper_getprinter(self), \
	                              tpp_lexer_dumper_getarg(self),     \
	                              text, num_bytes))
#endif
#ifndef tpp_lexer_dumper_do_print_cstr
#define tpp_lexer_dumper_do_print_cstr(self, text, num_bytes)             \
	tpp_lexer_dumper_do(                                                  \
	self, tpp_formatprinter_print_cstr(tpp_lexer_dumper_getprinter(self), \
	                                   tpp_lexer_dumper_getarg(self),     \
	                                   text, num_bytes))
#endif /* !tpp_lexer_dumper_do_print_cstr */

#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
#if TPP_HAVE_TOKEN_ENCODESTRING
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_do_print_escaped(tpp_lexer_dumper *tpp_restrict self,
                                  tpp_char const *text, tpp_size num_bytes) {
	tpp_lexer_dumper_do(
	self, tpp_token_encodestring(tpp_lexer_dumper_getprinter(self),
	                             tpp_lexer_dumper_getarg(self),
	                             text, num_bytes));
}
#else /* TPP_HAVE_TOKEN_ENCODESTRING */
#define tpp_lexer_dumper_do_print_escaped(self, text, num_bytes) \
	tpp_lexer_dumper_do_print(self, text, num_bytes)
#endif /* !TPP_HAVE_TOKEN_ENCODESTRING */
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */


#if TPP_HAVE_PRAGMA_EXTENSION
#define tpp_lexer_dumper_do_print_enable_extension(self, extname) \
	tpp_lexer_dumper_do_print_conststr(self, "#pragma extension(\"-f" extname "\")\n")
#define tpp_lexer_dumper_do_print_disable_extension(self, extname) \
	tpp_lexer_dumper_do_print_conststr(self, "#pragma extension(\"-fno-" extname "\")\n")
#elif TPP_HAVE_PRAGMA_TPP_EXTENSION
#define tpp_lexer_dumper_do_print_enable_extension(self, extname) \
	tpp_lexer_dumper_do_print_conststr(self, "#pragma TPP extension(\"-f" extname "\")\n")
#define tpp_lexer_dumper_do_print_disable_extension(self, extname) \
	tpp_lexer_dumper_do_print_conststr(self, "#pragma TPP extension(\"-fno-" extname "\")\n")
#endif /* ... */
#define tpp_lexer_dumper_do_print_set_extension(self, extname, enabled) \
	do {                                                                \
		if (enabled) {                                                  \
			tpp_lexer_dumper_do_print_enable_extension(self, extname);  \
		} else {                                                        \
			tpp_lexer_dumper_do_print_disable_extension(self, extname); \
		}                                                               \
	} while (0)

	/* Dump builtin macros */
#if TPP_HAVE_CPP_BUILTIN_MACROS
static TPP_NONNULL((1)) void TPPCALL
tpp_lexer_dumper_print_builtin_macros(tpp_lexer_dumper *tpp_restrict self) {
	tpp_token_id kwd_iter = TPP_TOK_KEYWORD_BEGIN;
#if TPP_HAVE_USER_KEYWORDS
	tpp_token_id kwd_end = TPP_TOK_USERKEYWORD_BEGIN;
#else /* TPP_HAVE_USER_KEYWORDS */
	tpp_token_id kwd_end = TPP_TOK_USERKEYWORD;
#endif /* !TPP_HAVE_USER_KEYWORDS */
	for (; kwd_iter < kwd_end; kwd_iter = (tpp_token_id)((unsigned int)kwd_iter + 1)) {
		tpp_keyword const *keyword = tpp_builtin_getkeyword_byid(kwd_iter);
		if (keyword && tpp_lexer_getkeyworddefined(self->tld_lexer, keyword)) {
			tpp_builtin_macro const *expansion = tpp_macro_getbuiltin(kwd_iter);
			tpp_lexer_dumper_do_print_conststr(self, "#define ");
			tpp_lexer_dumper_do_print(self, tpp_keyword_getstr(keyword), tpp_keyword_getlen(keyword));
			if (expansion) {
				tpp_lexer_dumper_do_print_conststr(self, " ");
				tpp_lexer_dumper_do_print(self,
				                          tpp_builtin_macro_getbody(expansion),
				                          tpp_builtin_macro_getsize(expansion));
			} else {
				/* All magic builtins provided by TPP follow the rule:
				 * - If it ends with a trailing _, then it's a keyword (__LINE__, etc.)
				 * - If it doesn't end with a trailing _, then it's a function
				 *
				 * And since we should only get here for builtin macros that don't
				 * have their expansion defined by `TPP_BUILTIN_MACRO()`, that
				 * should only happen for TPP's *own* builtin macros. */
				bool is_function = tpp_keyword_getlen(keyword) &&
				                   tpp_keyword_getstr(keyword)[tpp_keyword_getlen(keyword) - 1] != '_';

				/* Not really what's happening, but should get the point across that it's *magic* */
				if (is_function)
					tpp_lexer_dumper_do_print_conststr(self, "(ARGS)");
				tpp_lexer_dumper_do_print_conststr(self, " ");
				tpp_lexer_dumper_do_print(self, tpp_keyword_getstr(keyword), tpp_keyword_getlen(keyword));
				if (is_function)
					tpp_lexer_dumper_do_print_conststr(self, "(ARGS)");
			}
			tpp_lexer_dumper_do_print_conststr(self, "\n");
			if (tpp_lexer_dumper_haserr(self))
				break;
		}
	}
}
#endif /* TPP_HAVE_CPP_BUILTIN_MACROS */


#if TPP_HAVE_CPP_MACROS
static TPP_NONNULL((1, 2, 3)) void TPPCALL
tpp_lexer_dumper_printmacro(tpp_lexer_dumper *tpp_restrict self,
                            tpp_keyword const *tpp_restrict keyword,
                            tpp_macro const *tpp_restrict macro) {
#if TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
	if (!!self->tld_enabled_keepargspc != !!tpp_macro_keepsargspc(macro)) {
		tpp_lexer_dumper_do_print_set_extension(self, TPP_EXTNAME_MACRO_ARGUMENT_WHITESPACE,
		                                        tpp_macro_keepsargspc(macro));
		self->tld_enabled_keepargspc = tpp_macro_keepsargspc(macro);
	}
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION)
	if (!!self->tld_enabled_macro_recursion != !!tpp_macro_allowsselfexpansion(macro)) {
		tpp_lexer_dumper_do_print_set_extension(self, TPP_EXTNAME_MACRO_RECURSION,
		                                        tpp_macro_allowsselfexpansion(macro));
		self->tld_enabled_macro_recursion = tpp_macro_allowsselfexpansion(macro);
	}
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION) */
#endif /* TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION */

#if TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO
#ifndef TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START /* #ifndef: allow for user-overrides */
#if TPP_HAVE_TOK_C_COMMENT
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "/*"
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "*/"
#elif TPP_HAVE_TOK_PASCAL_COMMENT
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "(*"
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "*)"
#elif TPP_HAVE_TOK_PASCAL_BRACE_COMMENT
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "{"
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "}"
#elif TPP_HAVE_TOK_CXX_COMMENT
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "// "
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "\n"
#elif TPP_HAVE_TOK_SQL_COMMENT
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "-- "
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "\n"
#elif TPP_HAVE_TOK_AT_AT_COMMENT
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "@@ "
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "\n"
#elif TPP_HAVE_TOK_SLASH_COMMENT || TPP_HAVE_TOK_SOL_SLASH_COMMENT
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "/ "
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "\n"
#elif TPP_HAVE_TOK_AT_COMMENT || TPP_HAVE_TOK_SOL_AT_COMMENT
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "@ "
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "\n"
#elif TPP_HAVE_TOK_SHELL_COMMENT || TPP_HAVE_TOK_SOL_SHELL_COMMENT
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "# "
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "\n"
#else /* ... */
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START "["
#define TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END   "]\n"
#endif /* !... */
#endif /* !TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START */
	{
		char ubuf[TPP_UTOA_MAXLEN];
		char const *p = tpp_utoa(ubuf, (tpp_uintmax)keyword->tk_id);
		tpp_lexer_dumper_do_print_conststr(self, TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_START);
		tpp_lexer_dumper_do_print_cstr(self, p, (tpp_size)(ubuf + tpp_lengthof(ubuf) - p));
		if (macro->tm_deffile) {
			tpp_lexer_dumper_do_print_conststr(self, "@");
			tpp_lexer_dumper_do_print_cstr(self, macro->tm_deffile, tpp_strlen(macro->tm_deffile));
			if (tpp_lcinfo_isvalid(macro->tm_deflc)) {
				char ibuf[TPP_ITOA_MAXLEN];
				tpp_lexer_dumper_do_print_conststr(self, ":");
				p = tpp_itoa(ibuf, (tpp_intmax)tpp_lcinfo_getline(macro->tm_deflc) + 1);
				tpp_lexer_dumper_do_print_cstr(self, p, (tpp_size)(ibuf + tpp_lengthof(ibuf) - p));
				tpp_lexer_dumper_do_print_conststr(self, ":");
				p = tpp_itoa(ibuf, (tpp_intmax)tpp_lcinfo_getcol(macro->tm_deflc) + 1);
				tpp_lexer_dumper_do_print_cstr(self, p, (tpp_size)(ibuf + tpp_lengthof(ibuf) - p));
			}
		}
		tpp_lexer_dumper_do_print_conststr(self, TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO_END);
	}
#endif /* TPP_LEXER_DUMP_DEFINITIONS_EXTRAINFO */

	tpp_lexer_dumper_do_print_conststr(self, "#define ");
	tpp_lexer_dumper_do_print(self, tpp_keyword_getstr(keyword), tpp_keyword_getlen(keyword));
	if (tpp_macro_isfunction(macro)) {
		tpp_char lparen = (tpp_char)tpp_macro_getfunclparen(macro);
		tpp_char rparen = (tpp_char)tpp_macro_getfuncrparen(macro);
		tpp_size i, argc = tpp_macro_getfuncargc(macro);
		tpp_lexer_dumper_do_print(self, &lparen, 1);
		for (i = 0; i < argc; ++i) {
			tpp_token_id arg_tok = tpp_macro_getfuncargtok(macro, i);
			if (i != 0)
				tpp_lexer_dumper_do_print_conststr(self, ", ");
#if TPP_HAVE_VA_ARGS_IN_MACROS
			if ((i != argc - 1) || !tpp_macro_isvarargs(macro) || arg_tok != TPP_KWD___VA_ARGS__)
#endif /* TPP_HAVE_VA_ARGS_IN_MACROS */
			{
				tpp_keyword const *arg_kwd = tpp_keywords_getkeyword_byid(&self->tld_lexer->tl_kwds, arg_tok);
				char const *arg_name = arg_kwd ? tpp_keyword_getcstr(arg_kwd) : "?";
				tpp_lexer_dumper_do_print_cstr(self, arg_name, tpp_strlen(arg_name));
			}
#if TPP_HAVE_NAMED_VARARGS_IN_MACROS || TPP_HAVE_VA_ARGS_IN_MACROS
			if ((i == argc - 1) && tpp_macro_isvarargs(macro))
				tpp_lexer_dumper_do_print_conststr(self, "...");
#endif /* TPP_HAVE_NAMED_VARARGS_IN_MACROS || TPP_HAVE_VA_ARGS_IN_MACROS */
		}
		tpp_lexer_dumper_do_print(self, &rparen, 1);
	}
	tpp_lexer_dumper_do_print_conststr(self, " ");
	tpp_lexer_dumper_do_print(self, macro->tm_body_start,
	                                (tpp_size)(macro->tm_body_end - macro->tm_body_start));
	tpp_lexer_dumper_do_print_conststr(self, "\n");
}
#endif /* TPP_HAVE_CPP_MACROS */

#if TPP_HAVE_CPP_ASSERT
/* Find the keyword with the smallest token id that is ">= min_token_id" */
#if TPP_LEXER_DUMP_DEFINITIONS_SORTED
static TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_assertions_find_nextafter(tpp_assertions const *tpp_restrict self,
                              tpp_token_id min_token_id) {
	tpp_hash i;
	tpp_keyword const *result = NULL;
	for (i = 0; i <= self->tass_bckm; ++i) {
		tpp_keyword const *keyword = self->tass_bckv[i].tas_value;
		if (keyword && keyword->tk_id >= min_token_id) {
#ifndef __OPTIMIZE_SIZE__
			if (keyword->tk_id == min_token_id)
				return keyword; /* Perfect match! */
#endif /* !__OPTIMIZE_SIZE__ */
			if (result == NULL || result->tk_id > keyword->tk_id)
				result = keyword;
		}
	}
	return result;
}
#endif /* TPP_LEXER_DUMP_DEFINITIONS_SORTED */

static TPP_NONNULL((1, 2, 3)) void TPPCALL
tpp_lexer_dumper_printassert(tpp_lexer_dumper *tpp_restrict self,
                             tpp_keyword const *keyword,
                             tpp_keyword const *value) {
	tpp_lexer_dumper_do_print_conststr(self, "#assert ");
	tpp_lexer_dumper_do_print(self, tpp_keyword_getstr(keyword), tpp_keyword_getlen(keyword));
	tpp_lexer_dumper_do_print_conststr(self, "(");
	tpp_lexer_dumper_do_print(self, tpp_keyword_getstr(value), tpp_keyword_getlen(value));
	tpp_lexer_dumper_do_print_conststr(self, ")\n");
}

static TPP_NONNULL((1, 2, 3)) void TPPCALL
tpp_lexer_dumper_printasserts(tpp_lexer_dumper *tpp_restrict self,
                              tpp_keyword const *tpp_restrict keyword,
                              tpp_assertions const *tpp_restrict assertions) {
	tpp_hash i;
	if (!assertions->tass_assc)
		return;
#if TPP_LEXER_DUMP_DEFINITIONS_SORTED
	if (self->tld_what & TPP_LEXER_DUMP_DEFINITIONS_SORTED) {
		tpp_token_id id = TPP_TOK_KEYWORD_BEGIN;
		for (;;) {
			tpp_keyword const *value = tpp_assertions_find_nextafter(assertions, id);
			if (value == NULL)
				break;
			tpp_lexer_dumper_printassert(self, keyword, value);
			if (tpp_lexer_dumper_haserr(self))
				return;
			id = (tpp_token_id)((unsigned int)value->tk_id + 1);
		}
	} else
#endif /* TPP_LEXER_DUMP_DEFINITIONS_SORTED */
	{
		for (i = 0; i <= assertions->tass_bckm; ++i) {
			tpp_assertion const *ent = &assertions->tass_bckv[i];
			tpp_keyword const *value = ent->tas_value;
			if (value) {
				tpp_lexer_dumper_printassert(self, keyword, value);
				if (tpp_lexer_dumper_haserr(self))
					return;
			}
		}
	}
}
#endif /* TPP_HAVE_CPP_ASSERT */

#if TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_ASSERT
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_printkeyword(tpp_lexer_dumper *tpp_restrict self,
                              tpp_keyword const *tpp_restrict keyword) {
#if TPP_HAVE_CPP_MACROS
	if (self->tld_what & TPP_LEXER_DUMP_DEFINITIONS_MACROS) {
		tpp_macro const *macro = keyword->tk_macro;
		if (macro && !tpp_lexer_dumper_haserr(self))
			tpp_lexer_dumper_printmacro(self, keyword, macro);
	}
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_CPP_ASSERT
	if (self->tld_what & TPP_LEXER_DUMP_DEFINITIONS_ASSERTS) {
		tpp_keyword_misc const *misc = keyword->tk_misc;
		if (misc && !tpp_lexer_dumper_haserr(self))
			tpp_lexer_dumper_printasserts(self, keyword, &misc->tkm_assertions);
	}
#endif /* TPP_HAVE_CPP_ASSERT */
}

/* Find the keyword with the smallest token id that is ">= min_token_id" */
#if TPP_LEXER_DUMP_DEFINITIONS_SORTED
static TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_keywords_find_nextafter(tpp_keywords const *tpp_restrict self,
                            tpp_token_id min_token_id) {
	tpp_hash i;
	tpp_keyword const *result = NULL;
	for (i = 0; i <= self->tks_bckm; ++i) {
		tpp_keyword const *chain = self->tks_bckv[i];
		for (; chain; chain = chain->tk_next) {
			if (chain->tk_id >= min_token_id) {
#ifndef __OPTIMIZE_SIZE__
				if (chain->tk_id == min_token_id)
					return chain; /* Perfect match! */
#endif /* !__OPTIMIZE_SIZE__ */
				if (result == NULL || result->tk_id > chain->tk_id)
					result = chain;
			}
		}
	}
	return result;
}
#endif /* TPP_LEXER_DUMP_DEFINITIONS_SORTED */

static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_printkeywords(tpp_lexer_dumper *tpp_restrict self,
                               tpp_keywords const *tpp_restrict keywords) {
#if TPP_LEXER_DUMP_DEFINITIONS_SORTED
	if (self->tld_what & TPP_LEXER_DUMP_DEFINITIONS_SORTED) {
		tpp_token_id id = TPP_TOK_KEYWORD_BEGIN;
		for (;;) {
			tpp_keyword const *kwd = tpp_keywords_find_nextafter(keywords, id);
			if (kwd == NULL)
				break;
			tpp_lexer_dumper_printkeyword(self, kwd);
			if (tpp_lexer_dumper_haserr(self))
				return;
			id = (tpp_token_id)((unsigned int)kwd->tk_id + 1);
		}
	} else
#endif /* TPP_LEXER_DUMP_DEFINITIONS_SORTED */
	{
		tpp_hash i;
		for (i = 0; i <= keywords->tks_bckm; ++i) {
			tpp_keyword const *kwd = keywords->tks_bckv[i];
			for (; kwd; kwd = kwd->tk_next) {
				tpp_lexer_dumper_printkeyword(self, kwd);
				if (tpp_lexer_dumper_haserr(self))
					return;
			}
		}
	}
}
#endif /* TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_ASSERT */


#if TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION
/* Return the default (or last-configured) value of "id" */
static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_lexer_dumper_getextdefault(tpp_lexer_dumper const *self, tpp_extension_id id) {
	(void)self;
	switch (id) {
#if TPP_HAVE_CPP_MACROS
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
	case TPP_EXT_MACRO_ARGUMENT_WHITESPACE:
		return self->tld_enabled_keepargspc;
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION)
	case TPP_EXT_MACRO_RECURSION:
		return self->tld_enabled_macro_recursion;
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION) */
#endif /* TPP_HAVE_CPP_MACROS */
	default: break;
	}
	return tpp_extensions_state_getid(&tpp_extensions_state_default, id);
}

static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_printextensions(tpp_lexer_dumper *tpp_restrict self,
                                 tpp_extensions const *tpp_restrict extensions) {
	tpp_extension_id id = (tpp_extension_id)0;
	for (; (unsigned int)id < (unsigned int)TPP_EXT_COUNT;
	     id = (tpp_extension_id)((unsigned int)id + 1)) {
		bool ext_curr = !!tpp_extensions_getid(extensions, id);
		bool ext_defl = !!tpp_lexer_dumper_getextdefault(self, id);
		if (ext_curr != ext_defl) {
			/* Extension state differs from default */
			char const *ext_name = tpp_extension_getname(id);
			if tpp_unlikely(ext_name == NULL)
				ext_name = "?";
#if TPP_HAVE_PRAGMA_EXTENSION
			tpp_lexer_dumper_do_print_conststr(self, "#pragma extension(\"-f");
#elif TPP_HAVE_PRAGMA_TPP_EXTENSION
			tpp_lexer_dumper_do_print_conststr(self, "#pragma TPP extension(\"-f");
#endif /* ... */
			if (!ext_curr)
				tpp_lexer_dumper_do_print_conststr(self, "no-");
			tpp_lexer_dumper_do_print_cstr(self, ext_name, tpp_strlen(ext_name));
			tpp_lexer_dumper_do_print_conststr(self, "\")\n");
			if (tpp_lexer_dumper_haserr(self))
				return;
		}
	}
}
#endif /* TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION */


#if TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_printwarnings(tpp_lexer_dumper *tpp_restrict self,
                               tpp_warnings const *tpp_restrict warnings) {
	tpp_warning_context_id ctx_id = (tpp_warning_context_id)0;
	for (; (unsigned int)ctx_id < (unsigned int)TPP_WC_COUNT;
	     ctx_id = (tpp_warning_context_id)((unsigned int)ctx_id + 1)) {
		tpp_warning_state warn_curr = tpp_warnings_getctx(warnings, ctx_id);
		tpp_warning_state warn_defl = tpp_warnings_state_get(&tpp_warnings_state_default, ctx_id);
		if (warn_curr != warn_defl) {
#if TPP_HAVE_WARNING_NUMBERS
			if (tpp_warning_context_id_isnumber(ctx_id)) {
				tpp_warning_id wid = tpp_warning_context_id_aswarning(ctx_id);
				unsigned int const number = tpp_warning_getnumbers(wid)[0];
				char buf[TPP_UTOA_MAXLEN];
				char const *p = tpp_utoa(buf, number);
				char const *mode_prefix;
				tpp_assert(wid != TPP_W_COUNT && "But... it's a numbered warning");
				tpp_assert(number != TPP_WARNING_NUMBER_INVALID && "But... it's a numbered warning");
				switch (warn_curr) {
				case TPP_WSTATE_DISABLED:
					mode_prefix = "disable: ";
					break;
#if TPP_HAVE_WARNING_ERROR
				case TPP_WSTATE_ERROR:
					mode_prefix = "error: ";
					break;
#endif /* TPP_HAVE_WARNING_ERROR */
				case TPP_WSTATE_FATAL:
					mode_prefix = "fatal: ";
					break;
#if TPP_HAVE_WARNING_SUPPRESS
				case TPP_WSTATE_SUPPRESS:
					mode_prefix = "suppress: ";
					break;
#endif /* TPP_HAVE_WARNING_SUPPRESS */
#if 0
				default: tpp_unreachable();
				case TPP_WSTATE_WARN:
#else
				default:
#endif
					mode_prefix = "enable: ";
					break;
				}
#if TPP_HAVE_PRAGMA_EXTENSION
				tpp_lexer_dumper_do_print_conststr(self, "#pragma warning(");
#elif TPP_HAVE_PRAGMA_TPP_EXTENSION
				tpp_lexer_dumper_do_print_conststr(self, "#pragma TPP warning(");
#endif /* ... */
				tpp_lexer_dumper_do_print_cstr(self, mode_prefix, tpp_strlen(mode_prefix));
				tpp_lexer_dumper_do_print_cstr(self, p, (tpp_size)(buf + tpp_lengthof(buf) - p));
				tpp_lexer_dumper_do_print_conststr(self, ")\n");
			} else
#endif /* TPP_HAVE_WARNING_NUMBERS */
			{
				tpp_warning_group_id gid = tpp_warning_context_id_asgroup(ctx_id);
				char const *group_names = tpp_warning_group_getnames(gid);
				char const *mode_prefix;
				tpp_assert(tpp_warning_context_id_isgroup(ctx_id));
				tpp_assert(group_names && "But... it should be a warning group!");
				switch (warn_curr) {
				case TPP_WSTATE_DISABLED:
					mode_prefix = "no-";
					break;
				case TPP_WSTATE_FATAL:
#if TPP_HAVE_WARNING_ERROR
					mode_prefix = "fatal: \"-W";
					if (0) {
				case TPP_WSTATE_ERROR:
						mode_prefix = "error: \"-W";
					}
#if TPP_HAVE_PRAGMA_EXTENSION
					tpp_lexer_dumper_do_print_conststr(self, "#pragma warning(");
#elif TPP_HAVE_PRAGMA_TPP_EXTENSION
					tpp_lexer_dumper_do_print_conststr(self, "#pragma TPP warning(");
#endif /* ... */
					tpp_lexer_dumper_do_print_cstr(self, mode_prefix, tpp_strlen(mode_prefix));
#else /* TPP_HAVE_WARNING_ERROR */
#if TPP_HAVE_PRAGMA_EXTENSION
					tpp_lexer_dumper_do_print_conststr(self, "#pragma warning(fatal: \"-W");
#elif TPP_HAVE_PRAGMA_TPP_EXTENSION
					tpp_lexer_dumper_do_print_conststr(self, "#pragma TPP warning(fatal: \"-W");
#endif /* ... */
#endif /* !TPP_HAVE_WARNING_ERROR */
					tpp_lexer_dumper_do_print_cstr(self, group_names, tpp_strlen(group_names));
					tpp_lexer_dumper_do_print_conststr(self, "\")");
					continue;
#if TPP_HAVE_WARNING_SUPPRESS
				case TPP_WSTATE_SUPPRESS:
					mode_prefix = "sup-";
					break;
#endif /* TPP_HAVE_WARNING_SUPPRESS */
#if 0
				default: tpp_unreachable();
				case TPP_WSTATE_WARN:
#else
				default:
#endif
					mode_prefix = "";
					break;
				}

#if TPP_HAVE_PRAGMA_EXTENSION
				tpp_lexer_dumper_do_print_conststr(self, "#pragma warning(\"-W");
#elif TPP_HAVE_PRAGMA_TPP_EXTENSION
				tpp_lexer_dumper_do_print_conststr(self, "#pragma TPP warning(\"-W");
#endif /* ... */
				tpp_lexer_dumper_do_print_cstr(self, mode_prefix, tpp_strlen(mode_prefix));
				tpp_lexer_dumper_do_print_cstr(self, group_names, tpp_strlen(group_names));
				tpp_lexer_dumper_do_print_conststr(self, "\")\n");
			}
			if (tpp_lexer_dumper_haserr(self))
				return;
		}
	}
}
#endif /* TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING */


#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_printincludes_list(tpp_lexer_dumper *tpp_restrict self,
                                    tpp_include_path_list const *tpp_restrict paths
#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
                                    , char const *prefix
                                    , tpp_size prefix_len
#endif /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
                                    ) {
	tpp_size i;
	for (i = 0; i < paths->tipl_size; ++i) {
		tpp_include_path_entry const *entry = &paths->tipl_list[i];
		tpp_lexer_dumper_do_print_conststr(self, "#pragma TPP include_path(");
#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
		tpp_lexer_dumper_do_print_cstr(self, prefix, prefix_len);
#endif /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
		tpp_lexer_dumper_do_print_conststr(self, "\"");
#if TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING
		tpp_lexer_dumper_do_print_escaped(self,
		                                  tpp_string_str(entry->tipe_pathstr),
		                                  tpp_string_len(entry->tipe_pathstr));
#else /* TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
		tpp_lexer_dumper_do_print_escaped(self, entry->tipe_path, tpp_strlen(entry->tipe_path));
#endif /* !TPP_HAVE_INCLUDE_PATH_ENTRY_IS_STRING */
		tpp_lexer_dumper_do_print_conststr(self, "\")\n");
		if (tpp_lexer_dumper_haserr(self))
			break;
	}
}

static TPP_NONNULL((1, 2)) void TPPCALL
tpp_lexer_dumper_printincludes(tpp_lexer_dumper *tpp_restrict self,
                               tpp_include_paths const *tpp_restrict includes) {
#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
	tpp_lexer_dumper_printincludes_list(self, &includes->tip_system_list, "", 0);
#if TPP_HAVE_INCLUDE_PATH_QUOTE
	if (!tpp_lexer_dumper_haserr(self))
		tpp_lexer_dumper_printincludes_list(self, &includes->tip_quote_list, "quote: ", 7);
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */
#if TPP_HAVE_INCLUDE_PATH_SYSHDR
	if (!tpp_lexer_dumper_haserr(self))
		tpp_lexer_dumper_printincludes_list(self, &includes->tip_syshdr_list, "system: ", 8);
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */
#if TPP_HAVE_INCLUDE_PATH_AFTER
	if (!tpp_lexer_dumper_haserr(self))
		tpp_lexer_dumper_printincludes_list(self, &includes->tip_after_list, "dirafter: ", 10);
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */
#else /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
	tpp_lexer_dumper_printincludes_list(self, &includes->tip_system_list);
#endif /* !TPP_HAVE_INCLUDE_PATH_MULTIPLE */
}
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */


/* Dump all user-defined macros and assertions to "printer"
 * @param: what: Set of `TPP_LEXER_DUMP_DEFINITIONS_*'
 * @return: * :  Sum of return values of "printer"
 * @return: < 0: First negative return value of "printer" */
TPP_IMPL TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_lexer_dump_definitions(tpp_lexer *tpp_restrict self,
                           tpp_formatprinter printer, void *arg,
                           unsigned int what) {
	tpp_lexer_dumper dumper;
	dumper.tld_lexer   = self;
	dumper.tld_printer = printer;
	dumper.tld_arg     = arg;
	dumper.tld_result  = 0;
	dumper.tld_what    = what;
#if TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION
#if TPP_HAVE_CPP_MACROS
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
	dumper.tld_enabled_keepargspc = TPP_CONF_DEFAULT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE);
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION)
	dumper.tld_enabled_macro_recursion = TPP_CONF_DEFAULT(TPP_HAVE_MACRO_RECURSION);
#endif /* TPP_CONF_IS_EXT(TPP_HAVE_MACRO_RECURSION) */
#endif /* TPP_HAVE_CPP_MACROS */
#endif /* TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION */

	/* Dump builtin macros */
#if TPP_HAVE_CPP_BUILTIN_MACROS
	if ((what & TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS) &&
	    !tpp_lexer_dumper_haserr(&dumper))
		tpp_lexer_dumper_print_builtin_macros(&dumper);
#endif /* TPP_HAVE_CPP_BUILTIN_MACROS */

	/* Dump macros & assertions */
#if TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_ASSERT
	if ((what & (TPP_LEXER_DUMP_DEFINITIONS_MACROS | TPP_LEXER_DUMP_DEFINITIONS_ASSERTS)) &&
	    !tpp_lexer_dumper_haserr(&dumper))
		tpp_lexer_dumper_printkeywords(&dumper, &self->tl_kwds);
#endif /* TPP_HAVE_CPP_MACROS || TPP_HAVE_CPP_ASSERT */

	/* Dump extensions */
#if TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION
	if ((what & TPP_LEXER_DUMP_DEFINITIONS_EXTENSIONS) &&
	    !tpp_lexer_dumper_haserr(&dumper))
		tpp_lexer_dumper_printextensions(&dumper, &self->tl_exts);
#endif /* TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION */

	/* Dump warnings */
#if TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING
	if ((what & TPP_LEXER_DUMP_DEFINITIONS_WARNINGS) &&
	    !tpp_lexer_dumper_haserr(&dumper))
		tpp_lexer_dumper_printwarnings(&dumper, &self->tl_warn);
#endif /* TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING */

#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
	if ((what & TPP_LEXER_DUMP_DEFINITIONS_INCLUDES) &&
	    !tpp_lexer_dumper_haserr(&dumper))
		tpp_lexer_dumper_printincludes(&dumper, &self->tl_include_paths);
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */

	return dumper.tld_result;
}
#endif /* TPP_HAVE_LEXER_DUMP_DEFINITIONS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_DUMP_C */
