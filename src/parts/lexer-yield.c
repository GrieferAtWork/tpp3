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
#ifndef GUARD_TPP_LEXER_YIELD_C
#define GUARD_TPP_LEXER_YIELD_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "extensions.h"
#include "file.h"
#include "keyword.h"
#include "macro.h"
#include "lexer.h"
#include "token.h"
#include "features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS

/* Perform the expansion of a user-defined "macro", with the lexer's
 * current token set to point at the macro's identifier (meaning that
 * you have to seek ahead in order to find the opening '(' token in
 * case of a function-style macro).
 *
 * @return: tpp_lexer_gettoken(self)->tt_id : Function-style macro cannot be expanded
 * @return: TPP_TOK_EOF: Success -- caller should yield again to load the
 *                                  first macro's first expansion token.
 * @return: TPP_TOK_ENOMEM: Out of memory */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_expand_macro(tpp_lexer *tpp_restrict self,
                       tpp_macro *tpp_restrict macro);

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_push_textfile_inherited(tpp_lexer *tpp_restrict self,
                                  tpp_char const *text, tpp_size textsize,
                                  /*0..1,inherit(always)*/ TPP_REF tpp_string *chunk) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem;
	*prev_file = *file;
	file->tf_pos   = text;
	file->tf_chunk = chunk;
	file->tf_end   = text + textsize;
	_tpp_file_init_common(file);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	file->tf_kind  = TPP_FILE_KIND_TEXT;
#if TPP_HAVE_UNICODE
	file->tf_enc = TPP_FILE_ENCODING_FORCE_UTF8;
#endif /* TPP_HAVE_UNICODE */
	file->tf_data.td_text.tft_name = NULL;
	return TPP_TOK_EOF;
err_nomem:
	if (chunk)
		tpp_string_decref(chunk);
	return TPP_TOK_ENOMEM;
}

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_push_textfile(tpp_lexer *tpp_restrict self,
                        tpp_char const *text,
                        tpp_size textsize) {
	TPP_REF tpp_string *chunk;
	chunk = tpp_string_malloc(textsize);
	if tpp_unlikely(!chunk)
		return TPP_TOK_ENOMEM;
	tpp_memcpy(tpp_string_str(chunk), text, textsize);
	return tpp_lexer_push_textfile_inherited(self, tpp_string_str(chunk),
	                                         textsize, chunk);
}

#if (TPP_HAVE_MACRO___COUNTER__ ||       \
     TPP_HAVE_MACRO___LINE__ ||          \
     TPP_HAVE_MACRO___COLUMN__ ||        \
     TPP_HAVE_MACRO___INCLUDE_LEVEL__ || \
     TPP_HAVE_MACRO___INCLUDE_DEPTH__)
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_push_textfile_int(tpp_lexer *tpp_restrict self,
                            tpp_intmax value) {
	char buf[TPP_ITOA_MAXLEN];
	char const *p = tpp_itoa(buf, value);
	return tpp_lexer_push_textfile(self, (tpp_char const *)p,
	                               (tpp_size)(buf + tpp_lengthof(buf) - p));
}
#endif /* ... */

/* Support for feature-test-style macros */
#undef TPP_HAVE_KEYWORD_TEST_MACROS
#if (TPP_HAVE_MACRO___TPP_UNIQUE || \
     TPP_HAVE_MACRO___TPP_COUNTER)
#define TPP_HAVE_KEYWORD_TEST_MACROS 1
#else /* ... */
#define TPP_HAVE_KEYWORD_TEST_MACROS 0
#endif /* !... */

/* Support for feature-test-style macros */
#undef TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
#if (TPP_HAVE_CLANG_MACRO___has_attribute ||          \
     TPP_HAVE_CLANG_MACRO___has_builtin ||            \
     TPP_HAVE_CLANG_MACRO___has_cpp_attribute ||      \
     TPP_HAVE_CLANG_MACRO___has_declspec_attribute || \
     TPP_HAVE_CLANG_MACRO___has_extension ||          \
     TPP_HAVE_CLANG_MACRO___has_feature ||            \
     TPP_HAVE_CLANG_MACRO___has_c_attribute ||        \
     TPP_HAVE_MACRO___is_identifier ||                \
     TPP_HAVE_MACRO___is_deprecated ||                \
     TPP_HAVE_MACRO___is_poisoned)
#define TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS 1
#else /* ... */
#define TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS 0
#endif /* !... */

#undef TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
#if (TPP_HAVE_MACRO___has_extension ||       \
     TPP_HAVE_MACRO___has_known_extension || \
     TPP_HAVE_MACRO___has_warning ||         \
     TPP_HAVE_MACRO___has_known_warning)
#define TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS 1
#else /* ... */
#define TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS 0
#endif /* !... */

#undef TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO
#define TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO \
	(TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS ||    \
	 TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS ||     \
	 TPP_HAVE_KEYWORD_TEST_MACROS)

#if TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO

#undef TPP_FEATURE_FLAG_EXPANSION_MAXLEN
#if TPP_HAVE_MACRO___TPP_UNIQUE || TPP_HAVE_MACRO___TPP_COUNTER
#define TPP_FEATURE_FLAG_EXPANSION_MAXLEN TPP_ITOA_MAXLEN
#else /* ... */
#define TPP_FEATURE_FLAG_EXPANSION_MAXLEN 1
#endif /* !... */

#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
struct tpp_lexer_handle_string_feature_test_data {
	tpp_lexer   *tlhsftd_lexer; /* [1..1] Lexer */
	tpp_token_id tlhsftd_mode;  /* Feature-test mode */
	tpp_char     tlhsftd_expansion[TPP_FEATURE_FLAG_EXPANSION_MAXLEN]; /* Desired expansion */
};

static tpp_errno TPPCALL
tpp_lexer_handle_string_feature_test_cb(void *arg, tpp_string *chunk,
                                        tpp_char const *str, tpp_size length) {
	struct tpp_lexer_handle_string_feature_test_data *data;
	(void)chunk;
	data = (struct tpp_lexer_handle_string_feature_test_data *)arg;
	switch (data->tlhsftd_mode) {

#if TPP_HAVE_MACRO___has_extension || TPP_HAVE_MACRO___has_known_extension
#if TPP_HAVE_MACRO___has_extension
	case TPP_KWD___has_extension:
#endif /* TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_MACRO___has_known_extension
	case TPP_KWD___has_known_extension:
#endif /* TPP_HAVE_MACRO___has_known_extension */
	{
		tpp_extension_id extension_id;
		if (length >= 2 && str[0] == '-' && str[1] == 'f') {
			str += 2;
			length -= 2;
		}
		extension_id = tpp_extension_byname_ex((char const *)str, length);
		if ((unsigned int)extension_id < (unsigned int)TPP_EXT_COUNT) {
#if TPP_HAVE_MACRO___has_extension
			if (data->tlhsftd_mode == TPP_KWD___has_extension) {
				if (tpp_extensions_getid(&data->tlhsftd_lexer->tl_exts, extension_id))
					data->tlhsftd_expansion[0] = '1';
			} else
#endif /* TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_MACRO___has_known_extension
			if (data->tlhsftd_mode == TPP_KWD___has_known_extension) {
				data->tlhsftd_expansion[0] = '1'; /* We only even get here if the extension is known! */
			} else
#endif /* TPP_HAVE_MACRO___has_known_extension */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_MACRO___has_extension || TPP_HAVE_MACRO___has_known_extension */

#if TPP_HAVE_MACRO___has_warning || TPP_HAVE_MACRO___has_known_warning
#if TPP_HAVE_MACRO___has_warning
	case TPP_KWD___has_warning:
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
	case TPP_KWD___has_known_warning:
#endif /* TPP_HAVE_MACRO___has_known_warning */
	{
		tpp_warning_group_id warning_group_id;
		if (length >= 2 && str[0] == '-' && str[1] == 'W') {
			str += 2;
			length -= 2;
		}
		warning_group_id = tpp_warning_group_byname_ex((char const *)str, length);
		if ((unsigned int)warning_group_id < (unsigned int)TPP_WG_COUNT) {
#if TPP_HAVE_MACRO___has_warning
			if (data->tlhsftd_mode == TPP_KWD___has_warning) {
				tpp_warning_context_id ctx_id = tpp_warning_context_id_ofgroup(warning_group_id);
				tpp_warning_state state = tpp_warnings_getctx(&data->tlhsftd_lexer->tl_warn, ctx_id);
				if (tpp_warning_state_willemit(state))
					data->tlhsftd_expansion[0] = '1';
			} else
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
			if (data->tlhsftd_mode == TPP_KWD___has_known_warning) {
				data->tlhsftd_expansion[0] = '1'; /* We only even get here if the warning is known! */
			} else
#endif /* TPP_HAVE_MACRO___has_known_warning */
			{
			}
		}
	}	break;
#endif /* TPP_HAVE_MACRO___has_warning || TPP_HAVE_MACRO___has_known_warning */

	default: break;
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_handle_feature_test_macro(tpp_lexer *tpp_restrict self, tpp_token_id mode) {
	tpp_lexer_seek_backup backup;
	tpp_char const *pos = tpp_lexer_seek_begin(self, &backup);
	tpp_token_id tok;
	unsigned int recursion;
#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
	struct tpp_lexer_handle_string_feature_test_data data;
#define tpp_feature_test_macro_expansion data.tlhsftd_expansion
#else /* TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
	tpp_char expansion[TPP_FEATURE_FLAG_EXPANSION_MAXLEN];
#define tpp_feature_test_macro_expansion expansion
#endif /* !TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
#if TPP_FEATURE_FLAG_EXPANSION_MAXLEN > 1
	tpp_size tpp_feature_test_macro_expansion_len = 1;
#else /* TPP_FEATURE_FLAG_EXPANSION_MAXLEN > 1 */
#define tpp_feature_test_macro_expansion_len 1
#endif /* TPP_FEATURE_FLAG_EXPANSION_MAXLEN <= 1 */
again_yield:
	tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	if tpp_unlikely(TPP_TOK_ISERR(tok))
		goto err_tok;
	if (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		goto again_yield;
	if (tok != TPP_TOK_OFCHAR('('))
		goto rollback;

	/* Yield feature keyword */
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
		if tpp_unlikely(TPP_TOK_ISERR(tok))
			goto err_tok;
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));

	/* Default to expanding to "0" */
	tpp_feature_test_macro_expansion[0] = '0';

	/* Deal with special case of "__has_extension()" (which is overloaded for TPP) */
#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
	if (TPP_TOK_ISSTRING(tok)) {
		tpp_errno error;
#if TPP_HAVE_CLANG_MACRO___has_extension
		if (mode == TPP_KWD___has_extension &&
		    !tpp_lexer_getext(self, TPP_EXT_MACRO___has_extension))
			goto seek_end_of_macro;
#define WANT_seek_end_of_macro
#endif /* TPP_HAVE_CLANG_MACRO___has_extension */

		/* Parse the string that the user entered. */
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_string_feature_test_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
	} else
#endif /* TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
	{
#if TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS || TPP_HAVE_KEYWORD_TEST_MACROS
		/* Deal with keyword-style feature tests... */
		if (TPP_TOK_ISKEYWORD(tok)) {
			tpp_keyword const *feature_keyword;
			tpp_keyword_flags mask, flags;

			/* Load keyword. */
			feature_keyword = tpp_lexer_gettoken(self)->tt_kwd;
			switch (mode) {

#if TPP_HAVE_MACRO___is_identifier
			case TPP_KWD___is_identifier:
				/* Something is considered to be an "identifier" if it's not a builtin keyword. */
				if (!TPP_TOK_ISBUILTINKEYWORD(feature_keyword->tk_id))
					tpp_feature_test_macro_expansion[0] = '1';
				goto after_expansion_mode_assignment;
#define WANT_after_expansion_mode_assignment
#endif /* TPP_HAVE_MACRO___is_identifier */

#if TPP_HAVE_MACRO___TPP_UNIQUE || TPP_HAVE_MACRO___TPP_COUNTER
			{
				tpp_intmax expansion_value;
				char *expansion_dst;
#if TPP_HAVE_MACRO___TPP_UNIQUE
				if(0) {
			case TPP_KWD___TPP_UNIQUE:
					expansion_value = (tpp_intmax)feature_keyword->tk_id;
				}
#endif /* TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_COUNTER
				if(0) {
					tpp_keyword_misc *misc;
					tpp_keyword *rw_keyword;
			case TPP_KWD___TPP_COUNTER:
					rw_keyword = tpp_keywords_copybuiltin(&self->tl_kwds, feature_keyword);
					if tpp_unlikely(!rw_keyword)
						return TPP_TOK_ENOMEM;
					misc = tpp_keyword_requiremisc(rw_keyword);
					if tpp_unlikely(!misc)
						return TPP_TOK_ENOMEM;
					expansion_value = misc->tkm_builtin_counter++;
				}
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
				expansion_dst = tpp_itoa((char *)tpp_feature_test_macro_expansion, expansion_value);
				tpp_feature_test_macro_expansion_len = (tpp_size)((char *)tpp_feature_test_macro_expansion +
					                                              tpp_lengthof(tpp_feature_test_macro_expansion) -
					                                              expansion_dst);
				tpp_memmovedown(tpp_feature_test_macro_expansion, expansion_dst, tpp_feature_test_macro_expansion_len);
				goto after_expansion_mode_assignment;
#define WANT_after_expansion_mode_assignment
			}	break;
#endif /* !TPP_HAVE_MACRO___TPP_UNIQUE || TPP_HAVE_MACRO___TPP_COUNTER */

#if TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
			default: break;
#else /* TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS */
			default: tpp_unreachable();
#endif /* !TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS */
			}

#if TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
			/* Load keyword flags. */
			flags = tpp_lexer_getkeywordflags(self, feature_keyword);

			/* Also include flags after stripping leading/trailing _-s */
			if (feature_keyword->tk_kwd[0] == '_' ||
			    feature_keyword->tk_kwd[feature_keyword->tk_len - 1] == '_') {
				tpp_char const *strip = feature_keyword->tk_kwd;
				tpp_size strip_len = feature_keyword->tk_len;
				tpp_hash strip_hash;
				while (*strip == '_') {
					++strip;
					--strip_len;
				}
				while (strip[strip_len - 1] == '_')
					--strip_len;
				strip_hash = tpp_hashof(strip, strip_len);
				feature_keyword = tpp_keywords_getkeyword(&self->tl_kwds, strip,
				                                          strip_len, strip_hash);
				flags |= tpp_lexer_getkeywordflags(self, feature_keyword);
			}

			/* Determine expansion based on "mode" and "flags" */
			switch (mode) {
#if TPP_HAVE_CLANG_MACRO___has_attribute
			case TPP_KWD___has_attribute:
				mask = TPP_KEYWORD_FLAG_HAS_ATTRIBUTE;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
			case TPP_KWD___has_builtin:
				mask = TPP_KEYWORD_FLAG_HAS_BUILTIN;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
			case TPP_KWD___has_cpp_attribute:
				mask = TPP_KEYWORD_FLAG_HAS_CPP_ATTRIBUTE;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
			case TPP_KWD___has_declspec_attribute:
				mask = TPP_KEYWORD_FLAG_HAS_DECLSPEC_ATTRIBUTE;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension
			case TPP_KWD___has_extension:
				mask = TPP_KEYWORD_FLAG_HAS_EXTENSION;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
			case TPP_KWD___has_feature:
				mask = TPP_KEYWORD_FLAG_HAS_FEATURE;
#if TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES
				if (tpp_lexer_getext(self, TPP_EXT_CLANG_EXTENSIONS_ARE_FEATURES))
					mask |= TPP_KEYWORD_FLAG_HAS_EXTENSION;
#endif /* TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES */
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
			case TPP_KWD___has_c_attribute:
				mask = TPP_KEYWORD_FLAG_HAS_C_ATTRIBUTE;
				break;
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_MACRO___is_deprecated
			case TPP_KWD___is_deprecated:
				mask = TPP_KEYWORD_FLAG_IS_DEPRECATED;
				break;
#endif /* TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_MACRO___is_poisoned
			case TPP_KWD___is_poisoned:
				mask = TPP_KEYWORD_FLAG_IS_POISONED;
				break;
#endif /* TPP_HAVE_MACRO___is_poisoned */
			default: tpp_unreachable();
			}
			tpp_feature_test_macro_expansion[0] = (flags & mask) != 0 ? '1' : '0';
#endif /* TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS */

#ifdef WANT_after_expansion_mode_assignment
#undef WANT_after_expansion_mode_assignment
after_expansion_mode_assignment:
#endif /* WANT_after_expansion_mode_assignment */
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
			if tpp_unlikely(TPP_TOK_ISERR(tok))
				goto err_tok;
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS || TPP_HAVE_KEYWORD_TEST_MACROS */
	}

#ifdef WANT_seek_end_of_macro
#undef WANT_seek_end_of_macro
seek_end_of_macro:
#endif /* WANT_seek_end_of_macro */
	recursion = 0;
	for (;;) {
		if (tok == '(') {
			++recursion;
		} else if (tok == ')') {
			if (recursion == 0)
				break;
			--recursion;
		}
		tpp_feature_test_macro_expansion[0] = '0';
#if TPP_FEATURE_FLAG_EXPANSION_MAXLEN > 1
		tpp_feature_test_macro_expansion_len = 1;
#endif /* TPP_FEATURE_FLAG_EXPANSION_MAXLEN > 1 */
		if (tok == TPP_TOK_EOF)
			goto rollback;
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
		if tpp_unlikely(TPP_TOK_ISERR(tok))
			goto err_tok;
	}
	tpp_lexer_seek_commit(self, pos);
	return tpp_lexer_push_textfile(self, tpp_feature_test_macro_expansion,
	                               tpp_feature_test_macro_expansion_len);
rollback:
	tok = backup.tlsb_id;
err_tok:
	tpp_lexer_seek_rollback(self, &backup);
	return tok;
#undef tpp_feature_test_macro_expansion_len
#undef tpp_feature_test_macro_expansion
}
#endif /* TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO */


#if TPP_HAVE_PRAGMA
/* Process a #pragma directive, start at the first token that comes after
 * the leading "#pragma" (i.e.: the first token of the actual directive
 * itself)
 *
 * @return: TPP_EOK:    Success (but there may still be garbage after
 *                      the directive that hasn't been parsed, yet).
 * @return: TPP_ENOENT: Unknown pragma (soft-error; must be handled by caller)
 * @return: TPP_E*:     Error */
TPP_INTERN_DECL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self);
#endif /* TPP_HAVE_PRAGMA */


#if TPP_HAVE_MACRO__Pragma || TPP_HAVE_MACRO___pragma
static TPP_WUNUSED tpp_errno TPPCALL 
tpp_lexer_process_pragma_until_eof(tpp_lexer *tpp_restrict self) {
	tpp_errno result = tpp_lexer_process_pragma(self);
	if (result == TPP_ENOENT) {
		result = TPP_EOK; /* Don't warn about trailing tokens in this case */
	} else
#if TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE
	if (!TPP_ISERR(result)) {
		/* Warn about trailing tokens */
		tpp_token_id tok = tpp_lexer_gettoken(self)->tt_id;
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			tok = tpp_lexer_yieldraw_blocking(self);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
		if (tok != TPP_TOK_EOF)
			result = tpp_lexer_warnf(self, TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE);
	} else
#endif /* TPP_HAVE_TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE */
	{
	}
	return result;
}
#endif /* TPP_HAVE_MACRO__Pragma || TPP_HAVE_MACRO___pragma */

#if TPP_HAVE_MACRO__Pragma
static TPP_WUNUSED tpp_errno TPPCALL 
tpp_lexer_yield_handle__Pragma_string(void *arg, tpp_string *chunk,
                                      tpp_char const *str, tpp_size length) {
	tpp_token_id tok;
	tpp_errno result;
	tpp_lexer *self = (tpp_lexer *)arg;
	tpp_file *const file = tpp_lexer_getfile(self);
	TPP_REF tpp_string *const saved__tf_chunk = file->tf_chunk;
	tpp_file_kind const saved_kind = file->tf_kind;
	tpp_lcinfo const saved_lcinfo = file->tf_data.td_text.tft_start_lc;
	tpp_assert(file->tf_prev == NULL);

	/* (re-)configure "file" to point at "str" (and setup LC info as close as possible)
	 * Really though: LC info will only be perfectly precise when "str" is actually still
	 * part of the original buffer. Otherwise, it will be off. */
	if (file->tf_chunk != chunk) {
		tpp_lcinfo lc = tpp_file_lcinfo(file, file->tf_pos);
		file->tf_chunk = chunk;
		file->tf_kind = TPP_FILE_KIND_TEXT;
		file->tf_data.td_text.tft_start_lc = lc;
#if TPP_HAVE_FILE_LC_CACHE
		file->tf_lcpos = NULL;
#endif /* TPP_HAVE_FILE_LC_CACHE */
	}
	tpp_file_setpos(file, str);
	tpp_file_seteof(file, str + length);

	/* Yield decoded _Pragma-string as a token. */
	do {
		tok = tpp_lexer_yieldraw(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok)) {
		result = TPP_TOK_ASERR(tok);
	} else {
		/* Process _Pragma string as a pragma */
		result = tpp_lexer_process_pragma_until_eof(self);
	}
	file->tf_data.td_text.tft_start_lc = saved_lcinfo;
	file->tf_kind = saved_kind;
	file->tf_chunk = saved__tf_chunk;
	return result;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle__Pragma(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_lexer_seek_backup backup;
	tpp_char const *pos = tpp_lexer_seek_begin(self, &backup);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_lexer_arginfo argv[1];
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if tpp_unlikely(TPP_TOK_ISERR(tok))
		goto err_tok;
	if (tok != '(')
		goto rollback;
	tok = tpp_lexer_seek_rparen_exact(self, &pos, argv, 1, "_Pragma",
	                                  TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL);
	if (TPP_TOK_ISERR(tok))
		goto err_tok;
	tpp_file_pusheof(file);
	tpp_file_pushifdef(file);

	/* Setup file to (re-)parse the _Pragma string */
	tpp_file_setpos(file, argv[0].tlai_start);
	tpp_file_seteof(file, argv[0].tlai_end);
	tok = tpp_lexer_yield(self);
	if (!TPP_TOK_ISSTRING(tok)) {
		if (TPP_TOK_ISERR(tok)) {
			error = TPP_TOK_ASERR(tok);
		} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
		}
	} else {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_yield_handle__Pragma_string,
		                                 self, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		if (error == TPP_EOK) {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			if (tpp_lexer_gettoken(self)->tt_id != TPP_TOK_EOF) {
				/* Warning if current token isn't EOF */
				error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
			} else
#endif /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			{
				/* TODO: Warning if #ifdef-stack isn't empty */
			}
		}
	}
	tpp_file_popifdef(file);
	tpp_file_popeof(file);
	tpp_file_setpos(file, pos); /* Continue parsing after the closing ')' once pragma is finished */

	tok = TPP_TOK_EOF;
	if (TPP_ISERR(error))
		tok = TPP_TOK_OFERR(error);
	return tok;
rollback:
	tok = backup.tlsb_id;
err_tok:
	tpp_lexer_seek_rollback(self, &backup);
	return tok;
}
#endif /* TPP_HAVE_MACRO__Pragma */


#if TPP_HAVE_MACRO___pragma
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___pragma(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_lexer_seek_backup backup;
	tpp_char const *pos = tpp_lexer_seek_begin(self, &backup);
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_lexer_arginfo argv[1];
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if tpp_unlikely(TPP_TOK_ISERR(tok))
		goto err_tok;
	if (tok != '(')
		goto rollback;
	tok = tpp_lexer_seek_rparen_exact(self, &pos, argv, 1, "__pragma",
	                                  TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL);
	if (TPP_TOK_ISERR(tok))
		goto err_tok;
	tpp_file_pusheof(file);
	tpp_file_pushifdef(file);

	/* Setup file to (re-)parse the __pragma content */
	tpp_file_setpos(file, argv[0].tlai_start);
	tpp_file_seteof(file, argv[0].tlai_end);
	tok = tpp_lexer_yield(self);
	if (TPP_TOK_ISERR(tok)) {
		error = TPP_TOK_ASERR(tok);
	} else {
		error = tpp_lexer_process_pragma_until_eof(self);
		if (error == TPP_EOK) {
			/* TODO: Warning if #ifdef-stack isn't empty */
		}
	}
	tpp_file_popifdef(file);
	tpp_file_popeof(file);
	tpp_file_setpos(file, pos); /* Continue parsing after the closing ')' once pragma is finished */
	tok = TPP_TOK_EOF;
	if (TPP_ISERR(error))
		tok = TPP_TOK_OFERR(error);
	return tok;
rollback:
	tok = backup.tlsb_id;
err_tok:
	tpp_lexer_seek_rollback(self, &backup);
	return tok;
}
#endif /* TPP_HAVE_MACRO___pragma */


#if TPP_HAVE_MACRO___LINE__ || TPP_HAVE_MACRO___COLUMN__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_lcinfo(tpp_lexer *tpp_restrict self, tpp_token_id what) {
	tpp_lcinfo info;
	tpp_file *file = tpp_lexer_getfile(self);
	tpp_intmax value;
	while (file->tf_tprev && file->tf_kind == TPP_FILE_KIND_MACRO)
		file = file->tf_tprev;
	/* HINT: Meaning of "tf_tpos" / "tf_pos" here:
	 * >> #define assert(x) (... || (_assert(x, __FILE__, __LINE__, __COLUMN__)))
	 * >> ...
	 * >> 
	 * >> if (x)
	 * >>     assert(y);
	 *        ^        ^
	 *        tf_tpos  tf_pos
	 *
	 * iow: "tf_tpos" position for tracebacks (points at what "caused" a macro/file push)
	 *      "tf_pos" position of next byte to-be parsed once lexer returns to this file
	 *
	 * For the sake of being pretty, we use "tf_tpos" since that's the location of the
	 * name of the macro that's currently being expanded. */
	info = tpp_file_lcinfo(file, file->tf_tpos);
	switch (what) {
#if TPP_HAVE_MACRO___LINE__
	case TPP_KWD___LINE__:
		value = tpp_lcinfo_getline(info);
		break;
#endif /* TPP_HAVE_MACRO___LINE__ */
#if TPP_HAVE_MACRO___COLUMN__
	case TPP_KWD___COLUMN__:
		value = tpp_lcinfo_getcol(info);
		break;
#endif /* TPP_HAVE_MACRO___COLUMN__ */
	default: tpp_unreachable();
	}
	++value;
	return tpp_lexer_push_textfile_int(self, value);
}
#endif /* ... */


#if TPP_HAVE_MACRO___COUNTER__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___COUNTER__(tpp_lexer *tpp_restrict self) {
	return tpp_lexer_push_textfile_int(self, self->tl_builtin_counter++);
}
#endif /* ... */


#if TPP_HAVE_MACRO___INCLUDE_LEVEL__ || TPP_HAVE_MACRO___INCLUDE_DEPTH__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___INCLUDE_LEVEL__(tpp_lexer *tpp_restrict self) {
	tpp_intmax value = -1;
	tpp_file const *iter = tpp_lexer_getfile(self);
	do {
		if (iter->tf_kind != TPP_FILE_KIND_MACRO)
			++value;
	} while ((iter = iter->tf_tprev) != NULL);
	return tpp_lexer_push_textfile_int(self, value);
}
#endif /* ... */



/* Handle a builtin macro.
 * @return: TPP_TOK_EOF: Caller should yield again.
 * @return: * : The new expansion token after keywords were handled */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_builtin_macro(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	/* Deal with pre-defined macros. */
	switch (tok) {


/************************************************************************/
#if TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO
#if TPP_HAVE_CLANG_MACRO___has_attribute
	case TPP_KWD___has_attribute:
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
	case TPP_KWD___has_builtin:
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
	case TPP_KWD___has_cpp_attribute:
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
	case TPP_KWD___has_declspec_attribute:
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension
	case TPP_KWD___has_extension:
#endif /* TPP_HAVE_CLANG_MACRO___has_extension || TPP_HAVE_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
	case TPP_KWD___has_feature:
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
	case TPP_KWD___has_c_attribute:
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_MACRO___is_identifier
	case TPP_KWD___is_identifier:
#endif /* TPP_HAVE_MACRO___is_identifier */
#if TPP_HAVE_MACRO___is_deprecated
	case TPP_KWD___is_deprecated:
#endif /* TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_MACRO___is_poisoned
	case TPP_KWD___is_poisoned:
#endif /* TPP_HAVE_MACRO___is_poisoned */
#if TPP_HAVE_MACRO___has_known_extension
	case TPP_KWD___has_known_extension:
#endif /* TPP_HAVE_MACRO___has_known_extension */
#if TPP_HAVE_MACRO___has_warning
	case TPP_KWD___has_warning:
#endif /* TPP_HAVE_MACRO___has_warning */
#if TPP_HAVE_MACRO___has_known_warning
	case TPP_KWD___has_known_warning:
#endif /* TPP_HAVE_MACRO___has_known_warning */
#if TPP_HAVE_MACRO___TPP_UNIQUE
	case TPP_KWD___TPP_UNIQUE:
#endif /* !TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_COUNTER
	case TPP_KWD___TPP_COUNTER:
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */
		return tpp_lexer_handle_feature_test_macro(self, tok);
#endif /* TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_MACRO___has_include ||      \
     TPP_HAVE_MACRO___has_include_next || \
     TPP_HAVE_MACRO___has_embed)
#if TPP_HAVE_MACRO___has_include
	case TPP_KWD___has_include:
#endif /* TPP_HAVE_MACRO___has_include */
#if TPP_HAVE_MACRO___has_include_next
	case TPP_KWD___has_include_next:
#endif /* TPP_HAVE_MACRO___has_include_next */
#if TPP_HAVE_MACRO___has_embed
	case TPP_KWD___has_embed:
#endif /* TPP_HAVE_MACRO___has_embed */
	{
		/* TODO */
		/* TODO: __has_embed (https://en.cppreference.com/c/preprocessor/embed) */
	}	break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__
#if TPP_HAVE_MACRO___FILE__
	case TPP_KWD___FILE__:
#endif /* TPP_HAVE_MACRO___FILE__ */
#if TPP_HAVE_MACRO___BASE_FILE__
	case TPP_KWD___BASE_FILE__:
#endif /* TPP_HAVE_MACRO___BASE_FILE__ */
	{
		/* TODO */
	}	break;
#endif /* TPP_HAVE_MACRO___FILE__ || TPP_HAVE_MACRO___BASE_FILE__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___LINE__ || TPP_HAVE_MACRO___COLUMN__
#if TPP_HAVE_MACRO___LINE__
	case TPP_KWD___LINE__:
#endif /* TPP_HAVE_MACRO___LINE__ */
#if TPP_HAVE_MACRO___COLUMN__
	case TPP_KWD___COLUMN__:
#endif /* TPP_HAVE_MACRO___COLUMN__ */
		return tpp_lexer_yield_handle_lcinfo(self, tok);
#endif /* TPP_HAVE_MACRO___LINE__ || TPP_HAVE_MACRO___COLUMN__ */
/************************************************************************/



/************************************************************************/
#if (TPP_HAVE_MACRO___TIME__ ||      \
     TPP_HAVE_MACRO___DATE__ ||      \
     TPP_HAVE_MACRO___TIMESTAMP__ || \
     TPP_HAVE_NUMERIC_DATE_MACROS || \
     TPP_HAVE_NUMERIC_TIME_MACROS)
#if TPP_HAVE_MACRO___TIME__
	case TPP_KWD___TIME__:
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
	case TPP_KWD___DATE__:
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
	case TPP_KWD___TIMESTAMP__:
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
#if TPP_HAVE_NUMERIC_DATE_MACROS
	case TPP_KWD___DATE_DAY__:
	case TPP_KWD___DATE_WDAY__:
	case TPP_KWD___DATE_YDAY__:
	case TPP_KWD___DATE_MONTH__:
	case TPP_KWD___DATE_YEAR__:
#endif /* !TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
	case TPP_KWD___TIME_SEC__:
	case TPP_KWD___TIME_MIN__:
	case TPP_KWD___TIME_HOUR__:
#endif /* !TPP_HAVE_NUMERIC_TIME_MACROS */
	{
		/* TODO: -Wdate-time (warning should be disabled by default) */
		/* TODO */
	}	break;
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__ || TPP_HAVE_MACRO___INCLUDE_DEPTH__
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__
	case TPP_KWD___INCLUDE_LEVEL__:
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ */
#if TPP_HAVE_MACRO___INCLUDE_DEPTH__
	case TPP_KWD___INCLUDE_DEPTH__:
#endif /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
		return tpp_lexer_yield_handle___INCLUDE_LEVEL__(self);
#endif /* ... */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___COUNTER__
	case TPP_KWD___COUNTER__:
		return tpp_lexer_yield_handle___COUNTER__(self);
#endif /* TPP_HAVE_MACRO___COUNTER__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO__Pragma
	case TPP_KWD__Pragma:
		return tpp_lexer_yield_handle__Pragma(self);
#endif /* TPP_HAVE_MACRO__Pragma */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___pragma
	case TPP_KWD___pragma:
		return tpp_lexer_yield_handle___pragma(self);
#endif /* TPP_HAVE_MACRO___pragma */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_EVAL
	/* TODO: __TPP_EVAL */
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */
#if TPP_HAVE_MACRO___TPP_LOAD_FILE
	/* TODO: __TPP_LOAD_FILE */
#endif /* !TPP_HAVE_MACRO___TPP_LOAD_FILE */
#if TPP_HAVE_MACRO___TPP_RANDOM
	/* TODO: __TPP_RANDOM */
#endif /* !TPP_HAVE_MACRO___TPP_RANDOM */
#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE
	/* TODO: __TPP_STR_DECOMPILE */
#endif /* !TPP_HAVE_MACRO___TPP_STR_DECOMPILE */
#if TPP_HAVE_MACRO___TPP_STR_SUBSTR
	/* TODO: __TPP_STR_SUBSTR */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SUBSTR */
#if TPP_HAVE_MACRO___TPP_STR_PACK
	/* TODO: __TPP_STR_PACK */
#endif /* !TPP_HAVE_MACRO___TPP_STR_PACK */
#if TPP_HAVE_MACRO___TPP_STR_SIZE
	/* TODO: __TPP_STR_SIZE */
#endif /* !TPP_HAVE_MACRO___TPP_STR_SIZE */
#if TPP_HAVE_MACRO___TPP_COUNT_TOKENS
	/* TODO: __TPP_COUNT_TOKENS */
#endif /* !TPP_HAVE_MACRO___TPP_COUNT_TOKENS */
#if TPP_HAVE_MACRO___TPP_IDENTIFIER
	/* TODO: __TPP_IDENTIFIER */
#endif /* !TPP_HAVE_MACRO___TPP_IDENTIFIER */
/************************************************************************/



	default: {
		/* Check for a pre-defined, builtin macro expansion */
		tpp_builtin_macro const *builtin_macro;
		builtin_macro = tpp_macro_getbuiltin(tok);
		if (builtin_macro != NULL) {
			return tpp_lexer_push_textfile_inherited(self, builtin_macro->tbm_body,
			                                         builtin_macro->tbm_body_size,
			                                         NULL);
		}
	}	break;

	}

	/* Fallback: act as though the macro takes no arguments, and expands to itself:
	 * >> #define SOME_MACRO SOME_MACRO */
	return tok;
}
#endif /* TPP_HAVE_CPP_MACROS */

/* Handle a keyword-style macro.
 * @return: TPP_TOK_EOF: Caller should yield again.
 * @return: * : The new expansion token after keywords were handled */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_keyword(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_keyword const *const keyword = token->tt_kwd;

	/* Emit warnings for "deprecated" keywords. */
#if TPP_HAVE_TPP_W_DEPRECATED_KEYWORD && TPP_HAVE_PRAGMA_DEPRECATED
	if (keyword->tk_misc) {
		tpp_keyword_misc const *misc = keyword->tk_misc;
		if (misc->tkm_flags & TPP_KEYWORD_FLAG_IS_DEPRECATED) {
#if TPP_HAVE_PRAGMA_GCC_POISON && TPP_HAVE_CPP_MACROS
			if ((misc->tkm_flags & TPP_KEYWORD_FLAG_IS_POISONED) &&
			    (tpp_lexer_getfile(self)->tf_kind == TPP_FILE_KIND_MACRO)) {
				/* Don't emit warning */
			} else
#endif /* TPP_HAVE_PRAGMA_GCC_POISON && TPP_HAVE_CPP_MACROS */
			{
				tpp_errno error = tpp_lexer_warnf(self, TPP_W_DEPRECATED_KEYWORD);
				if (TPP_ISERR(error))
					return TPP_TOK_OFERR(error);
			}
		}
	}
#endif /* TPP_HAVE_TPP_W_DEPRECATED_KEYWORD && TPP_HAVE_PRAGMA_DEPRECATED */

#if TPP_HAVE_CPP_MACROS
	/* Check if this keyword should be expanded as a macro.
	 * This also does the is-enabled checks for builtin macros. */
#if TPP_HAVE_LEXER_GETKEYWORDDEFINED
	if (!tpp_lexer_getkeyworddefined(self, keyword))
		return tok;
#endif /* TPP_HAVE_LEXER_GETKEYWORDDEFINED */

	/* Check for explicitly defined macros... */
	{
		tpp_macro *const macro = keyword->tk_macro;
		if (macro) {
			/* Check if expansion of the macro is allowed. */
			if (macro->tm_expansions > 0) {
#if TPP_HAVE_MACRO_RECURSION
				if (!(macro->tm_flags & TPP_MACRO_FLAG_SELFEXPAND))
#endif /* TPP_HAVE_MACRO_RECURSION */
				{
					return tok;
				}
			}

			/* Expand user-defined macro... */
			return tpp_lexer_expand_macro(self, macro);
		}
	}
	return tpp_lexer_yield_handle_builtin_macro(self, tok);
#else /* TPP_HAVE_CPP_MACROS */
	return tok;
#endif /* !TPP_HAVE_CPP_MACROS */
}

/* Wrapper around `tpp_lexer_yieldpp()' that adds handling for macro expansion.
 * @return: * :                  The newly read token (after accounting for macros)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldpp(self);
	if (TPP_TOK_ISKEYWORD(result)) {
		result = tpp_lexer_yield_handle_keyword(self, result);
		if (result == TPP_TOK_EOF)
			goto again;
	}
	return result;
}


#if TPP_HAVE_FILE_NONBLOCK
/* Same as `tpp_lexer_yield()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_blocking(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yield(self);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NONBLOCK);
		file->tf_data.td_io.tff_flags &= ~TPP_FILE_IOFLAGS_NONBLOCK;
		tpp_lexer_autopopfile_pushoff(self);
		result = tpp_lexer_yield(self);
		tpp_lexer_autopopfile_pop(self);
		file->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}

/* Same as `tpp_lexer_yieldraw_at()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_blocking(tpp_lexer *tpp_restrict self, tpp_char const **p_pos) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldraw_at(self, p_pos);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_data.td_io.tff_flags & TPP_FILE_IOFLAGS_NONBLOCK);
		file->tf_data.td_io.tff_flags &= ~TPP_FILE_IOFLAGS_NONBLOCK;
		tpp_lexer_autopopfile_pushoff(self);
		result = tpp_lexer_yieldraw_at(self, p_pos);
		tpp_lexer_autopopfile_pop(self);
		file->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}

#endif /* TPP_HAVE_FILE_NONBLOCK */


#if TPP_HAVE_LEXER_SKIP
/* Check that the currently loaded token is 'tok'. If so, "tpp_lexer_yield_blocking()" to
 * the next token (which is also returned). Otherwise, trigger 'TPP_W_UNEXPECTED_TOKEN'
 * and (if that warning wasn't fatal), try to seek ahead to see if "tok" can be found
 * somewhere close by (depending on what 'tok' and what was actually loaded on entry)
 *
 * @return: * :                  The token that comes after the one that was just skipped
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_IOFLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	if tpp_likely(token->tt_id == tok)
		return tpp_lexer_yield(self);
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN
	{
		tpp_errno error;
		char const *expected = tpp_lexer_reprtokenid(self, tok);
		if tpp_unlikely(!expected)
			expected = "?";
		error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN, expected);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
	}
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN */

	/* TODO: Try to seek ahead (within the current line) to
	 *       find "tok" when it's (e.g.) a '(' (to deal with
	 *       cases where the user added some extra, unrelated
	 *       tokens before the one we're expecting) */

	return token->tt_id;
}

#if TPP_HAVE_FILE_NONBLOCK
/* Same as `tpp_lexer_skip()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_IOFLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_skip_blocking(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token_id result = tpp_lexer_skip(self, tok);
	if (result == TPP_TOK_EWOULDBLOCK)
		result = tpp_lexer_yield_blocking(self);
	return result;
}
#endif /* TPP_HAVE_FILE_NONBLOCK */

#endif /* TPP_HAVE_LEXER_SKIP */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELD_C */
