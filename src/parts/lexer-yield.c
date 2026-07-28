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
#include "features.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "macro.h"
#include "time.h"
#include "token.h"

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
	tpp_file_move(prev_file, file);
	tpp_file_init_text(file, NULL, chunk, text, textsize,
	                   TPP_LCINFO_INVALID, TPP_FILE_FLAGS_NORMAL);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	return TPP_TOK_EOF;
err_nomem:
	if (chunk)
		tpp_string_decref(chunk);
	return TPP_TOK_ENOMEM;
}

#if (TPP_HAVE_MACRO___FILE__ ||      \
     TPP_HAVE_MACRO___BASE_FILE__ || \
     TPP_HAVE_MACRO___FILE_NAME__)
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_push_textfile_string_esc(tpp_lexer *tpp_restrict self,
                                   tpp_char const *unescaped_text,
                                   tpp_size unescaped_size) {
	TPP_REF tpp_string *chunk;
	tpp_string_builder builder;
	tpp_string_builder_init(&builder);
	if (tpp_string_builder_print(&builder, (tpp_char const *)"\"", 1) < 0)
		goto err_builder;
	if (tpp_token_encodestring(&tpp_string_builder_print, &builder, unescaped_text, unescaped_size) < 0)
		goto err_builder;
	if (tpp_string_builder_print(&builder, (tpp_char const *)"\"", 1) < 0)
		goto err_builder;
	chunk = tpp_string_builder_pack(&builder);
	return tpp_lexer_push_textfile_inherited(self, tpp_string_str(chunk), tpp_string_len(chunk), chunk);
err_builder:
	tpp_string_builder_fini(&builder);
	return TPP_TOK_ENOMEM;
}
#endif /* ... */

/* Support for feature-test-style macros */
#undef TPP_HAVE_KEYWORD_TEST_MACROS
#define TPP_HAVE_KEYWORD_TEST_MACROS \
	(TPP_HAVE_MACRO___TPP_UNIQUE ||  \
	 TPP_HAVE_MACRO___TPP_COUNTER)

/* Support for feature-test-style macros */
#undef TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
#define TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS     \
	(TPP_HAVE_CLANG_MACRO___has_attribute ||          \
	 TPP_HAVE_CLANG_MACRO___has_builtin ||            \
	 TPP_HAVE_CLANG_MACRO___has_cpp_attribute ||      \
	 TPP_HAVE_CLANG_MACRO___has_declspec_attribute || \
	 TPP_HAVE_CLANG_MACRO___has_extension ||          \
	 TPP_HAVE_CLANG_MACRO___has_feature ||            \
	 TPP_HAVE_CLANG_MACRO___has_c_attribute ||        \
	 TPP_HAVE_MACRO___is_identifier ||                \
	 TPP_HAVE_MACRO___is_deprecated ||                \
	 TPP_HAVE_MACRO___is_poisoned)

#undef TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
#define TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS \
	(TPP_HAVE_MACRO___has_extension ||           \
	 TPP_HAVE_MACRO___has_known_extension ||     \
	 TPP_HAVE_MACRO___has_warning ||             \
	 TPP_HAVE_MACRO___has_known_warning)

#undef TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO
#define TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO \
	(TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS ||    \
	 TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS ||     \
	 TPP_HAVE_KEYWORD_TEST_MACROS)

#undef TPP_HAVE_LEXER_PUSH_TEXTFILE_INT
#define TPP_HAVE_LEXER_PUSH_TEXTFILE_INT  \
	(TPP_HAVE_MACRO___COUNTER__ ||        \
	 TPP_HAVE_MACRO___LINE__ ||           \
	 TPP_HAVE_MACRO___COLUMN__ ||         \
	 TPP_HAVE_MACRO___INCLUDE_LEVEL__ ||  \
	 TPP_HAVE_MACRO___INCLUDE_DEPTH__ ||  \
	 TPP_HAVE_NUMERIC_DATE_MACROS ||      \
	 TPP_HAVE_NUMERIC_TIME_MACROS ||      \
	 TPP_HAVE_MACRO___TPP_STR_SIZE ||     \
	 TPP_HAVE_MACRO___TPP_COUNT_TOKENS || \
	 TPP_HAVE_MACRO___TPP_RANDOM)

#undef TPP_HAVE_LEXER_PUSH_TEXTFILE
#define TPP_HAVE_LEXER_PUSH_TEXTFILE                 \
	(TPP_HAVE_LEXER_PUSH_TEXTFILE_INT ||             \
	 TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO || \
	 TPP_HAVE_MACRO___TIME__ ||                      \
	 TPP_HAVE_MACRO___DATE__ ||                      \
	 TPP_HAVE_MACRO___TIMESTAMP__)

#if TPP_HAVE_LEXER_PUSH_TEXTFILE
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
#endif /* TPP_HAVE_LEXER_PUSH_TEXTFILE */

#if TPP_HAVE_LEXER_PUSH_TEXTFILE_INT
static TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_push_textfile_int(tpp_lexer *tpp_restrict self,
                            tpp_intmax value) {
	char buf[TPP_ITOA_MAXLEN];
	char const *p = tpp_itoa(buf, value);
	return tpp_lexer_push_textfile(self, (tpp_char const *)p,
	                               (tpp_size)(buf + tpp_lengthof(buf) - p));
}
#endif /* TPP_HAVE_LEXER_PUSH_TEXTFILE_INT */


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
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}

	do {
		tok = tpp_lexer_yieldpp_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));

	/* Default to expanding to "0" */
	tpp_feature_test_macro_expansion[0] = '0';

	/* Deal with special case of "__has_extension()" (which is overloaded for TPP) */
#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
	if (TPP_TOK_ISSTRING(tok)) {
		tpp_errno error;
#if TPP_HAVE_CLANG_MACRO___has_extension
		if (mode == TPP_KWD___has_extension &&
		    !tpp_lexer_has(self, MACRO___has_extension))
			goto seek_end_of_macro;
#define WANT_seek_end_of_macro
#endif /* TPP_HAVE_CLANG_MACRO___has_extension */
		data.tlhsftd_lexer = self;
		data.tlhsftd_mode  = mode;

		/* Parse the string that the user entered. */
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_string_feature_test_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		tok = tpp_lexer_gettok(self);
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
				if (tpp_lexer_isidentifier(self, feature_keyword))
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
					tpp_errno error;
					tpp_keyword *rw_keyword;
					tpp_counter counter;
			case TPP_KWD___TPP_COUNTER:
					rw_keyword = tpp_keywords_copybuiltin(&self->tl_kwds, feature_keyword);
					if tpp_unlikely(!rw_keyword)
						return TPP_TOK_ENOMEM;
					error = tpp_keyword_inc_builtin_counter(rw_keyword, &counter);
					if (TPP_ISERR(error))
						return TPP_TOK_OFERR(error);
					expansion_value = (tpp_intmax)counter;
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
				if (feature_keyword)
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
				if (tpp_lexer_has(self, CLANG_EXTENSIONS_ARE_FEATURES))
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
				tok = tpp_lexer_yieldpp_blocking(self);
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return tok;
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
			break;
		tok = tpp_lexer_yieldpp_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
	}
	return tpp_lexer_push_textfile(self, tpp_feature_test_macro_expansion,
	                               tpp_feature_test_macro_expansion_len);
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
 * @return: TPP_ENOENT: Unknown pragma (soft-error; caller should not emit
 *                      "TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE")
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
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
	if (!TPP_ISERR(result))
		result = tpp_lexer_warn_nonempty_ifdef(self);
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */

	/* Make sure that absolutely *nothing* is left on the #include-stack!
	 * This is important in case the pragma terminated *inside* of a macro,
	 * or "TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE" was emitted about a
	 * trailing token
	 *
	 * This should only really be relevant when it comes to recovering from
	 * faulty user-code... */
	tpp_lexer_popallfiles(self);
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
	tpp_file_subtext_push(file);
	tpp_file_subtext_setchunk_fromstring(file, chunk, str, length);

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

	tpp_file_subtext_pop(file);
	return result;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle__Pragma(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
	tpp_errno error;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	if (!TPP_TOK_ISSTRING(tok)) {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	} else {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_yield_handle__Pragma_string,
		                                 self, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	}
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return tok;
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		return tok;
	return TPP_TOK_EOF;
}
#endif /* TPP_HAVE_MACRO__Pragma */


#if TPP_HAVE_MACRO___pragma
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___pragma(tpp_lexer *tpp_restrict self) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_lexer_arginfo argv[1];
	tpp_token_id tok;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	tok = tpp_lexer_seekpp_rparen_exact(self, argv, 1, "__pragma",
	                                    TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL);
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Push a sub-text file to (re-)parse the __pragma content */
	tpp_file_subtext_push(file);
	tpp_file_subtext_setchunk_fromarg(file, &argv[0]);
	do {
		tok = tpp_lexer_yieldpp_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (!TPP_TOK_ISERR(tok)) {
		tpp_errno error;
		error = tpp_lexer_process_pragma_until_eof(self);
		tok = TPP_TOK_OFERR_OR_EOF(error);
	}
	tpp_file_subtext_pop(file);
	tpp_lexer_arginfo_fini(&argv[0]);
	return tok;
}
#endif /* TPP_HAVE_MACRO___pragma */


#if TPP_HAVE_MACRO___LINE__ || TPP_HAVE_MACRO___COLUMN__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_lcinfo(tpp_lexer *tpp_restrict self, tpp_token_id what) {
	tpp_intmax value;
	tpp_lcinfo info = tpp_lexer_getlcinfo(self);
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


#undef TPP_HAVE_TIME_MACROS
#define TPP_HAVE_TIME_MACROS         \
	(TPP_HAVE_MACRO___TIME__ ||      \
	 TPP_HAVE_MACRO___DATE__ ||      \
	 TPP_HAVE_MACRO___TIMESTAMP__ || \
	 TPP_HAVE_NUMERIC_DATE_MACROS || \
	 TPP_HAVE_NUMERIC_TIME_MACROS)
#if TPP_HAVE_TIME_MACROS

#if TPP_HAVE_MACRO___DATE__ || TPP_HAVE_MACRO___TIMESTAMP__
static char const tpp_date_month_names[12][3] = {
	{ 'J', 'a', 'n' },
	{ 'F', 'e', 'b' },
	{ 'M', 'a', 'r' },
	{ 'A', 'p', 'r' },
	{ 'M', 'a', 'y' },
	{ 'J', 'u', 'n' },
	{ 'J', 'u', 'l' },
	{ 'A', 'u', 'g' },
	{ 'S', 'e', 'p' },
	{ 'O', 'c', 't' },
	{ 'N', 'o', 'v' },
	{ 'D', 'e', 'c' },
};
#endif /* TPP_HAVE_MACRO___DATE__ || TPP_HAVE_MACRO___TIMESTAMP__ */

#if TPP_HAVE_MACRO___TIMESTAMP__
static char const tpp_date_wday_names[7][3] = {
	{ 'S', 'u', 'n' },
	{ 'M', 'o', 'n' },
	{ 'T', 'u', 'e' },
	{ 'W', 'e', 'd' },
	{ 'T', 'h', 'u' },
	{ 'F', 'r', 'i' },
	{ 'S', 'a', 't' },
};
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */

#if TPP_HAVE_MACRO___TIME__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TIME__(tpp_lexer *tpp_restrict self, tpp_tm const *cur) {
	char buf[tpp_lengthof("\"00:00:00\"") - 1], *p = buf;
	*p++ = '"';
	*p++ = (char)('0' + (tpp_tm_gethour(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_gethour(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getmin(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getmin(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getsec(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getsec(cur) % 10));
	*p++ = '"';
	tpp_assert(p == (buf + tpp_lengthof(buf)));
	return tpp_lexer_push_textfile(self, (tpp_char const *)buf, (tpp_size)(p - buf));
}
#endif /* TPP_HAVE_MACRO___TIME__ */

#if TPP_HAVE_MACRO___DATE__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___DATE__(tpp_lexer *tpp_restrict self, tpp_tm const *cur) {
	char buf[tpp_lengthof("\"Jan 10 2000\"") - 1], *p = buf;
	char const *mon = tpp_date_month_names[(tpp_tm_getmon(cur) - 1) % 12];
	*p++ = '"';
	*p++ = mon[0];
	*p++ = mon[1];
	*p++ = mon[2];
	*p++ = ' ';
	*p++ = (tpp_tm_getmday(cur) >= 10) ? (char)('0' + (tpp_tm_getmday(cur) / 10)) : ' ';
	*p++ = (char)('0' + (tpp_tm_getmday(cur) % 10));
	*p++ = ' ';
	*p++ = (char)('0' + ((tpp_tm_getyear(cur) / 1000) % 10));
	*p++ = (char)('0' + ((tpp_tm_getyear(cur) / 100) % 10));
	*p++ = (char)('0' + ((tpp_tm_getyear(cur) / 10) % 10));
	*p++ = (char)('0' + (tpp_tm_getyear(cur) % 10));
	*p++ = '"';
	tpp_assert(p == (buf + tpp_lengthof(buf)));
	return tpp_lexer_push_textfile(self, (tpp_char const *)buf, (tpp_size)(p - buf));
}
#endif /* TPP_HAVE_MACRO___DATE__ */

#if TPP_HAVE_MACRO___TIMESTAMP__
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TIMESTAMP__(tpp_lexer *tpp_restrict self, tpp_tm const *cur) {
	char buf[tpp_lengthof("\"Mon Jan 12 00:00:00 2026\"") - 1], *p = buf;
	char const *mon = tpp_date_month_names[(tpp_tm_getmon(cur) - 1) % 12];
	char const *day = tpp_date_wday_names[tpp_tm_getwday(cur) % 7];
	*p++ = '"';
	*p++ = day[0];
	*p++ = day[1];
	*p++ = day[2];
	*p++ = ' ';
	*p++ = mon[0];
	*p++ = mon[1];
	*p++ = mon[2];
	*p++ = ' ';
	*p++ = (tpp_tm_getmday(cur) >= 10) ? (char)('0' + (tpp_tm_getmday(cur) / 10)) : ' ';
	*p++ = (char)('0' + (tpp_tm_getmday(cur) % 10));
	*p++ = ' ';
	*p++ = (char)('0' + (tpp_tm_gethour(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_gethour(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getmin(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getmin(cur) % 10));
	*p++ = ':';
	*p++ = (char)('0' + (tpp_tm_getsec(cur) / 10));
	*p++ = (char)('0' + (tpp_tm_getsec(cur) % 10));
	*p++ = ' ';
	*p++ = (char)('0' + ((tpp_tm_getyear(cur) / 1000) % 10));
	*p++ = (char)('0' + ((tpp_tm_getyear(cur) / 100) % 10));
	*p++ = (char)('0' + ((tpp_tm_getyear(cur) / 10) % 10));
	*p++ = (char)('0' + (tpp_tm_getyear(cur) % 10));
	*p++ = '"';
	tpp_assert(p == (buf + tpp_lengthof(buf)));
	return tpp_lexer_push_textfile(self, (tpp_char const *)buf, (tpp_size)(p - buf));
}
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */


static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_time_macro(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_tm cur;
	tpp_errno error;

	/* Initialize time if this is the first *time* (pun intended) we get here */
	if (tpp_time_isempty(tpp_lexer_gettimeptr(self))) {
		error = tpp_time_now(tpp_lexer_gettimeptr(self));
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#if TPP_HAVE_TPP_W_DATE_TIME
		error = tpp_lexer_warnf(self, TPP_W_DATE_TIME);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
#endif /* TPP_HAVE_TPP_W_DATE_TIME */
	}

	/* Load tm from time */
	error = tpp_tm_fromtime(&cur, tpp_lexer_gettimeptr(self));
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);

	/* Process time according to given "tok" */
	switch (tok) {
#if TPP_HAVE_MACRO___TIME__
	case TPP_KWD___TIME__:
		return tpp_lexer_yield_handle___TIME__(self, &cur);
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
	case TPP_KWD___DATE__:
		return tpp_lexer_yield_handle___DATE__(self, &cur);
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
	case TPP_KWD___TIMESTAMP__:
		return tpp_lexer_yield_handle___TIMESTAMP__(self, &cur);
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
#if TPP_HAVE_NUMERIC_DATE_MACROS
	case TPP_KWD___DATE_DAY__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getmday(&cur));
	case TPP_KWD___DATE_WDAY__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getwday(&cur));
	case TPP_KWD___DATE_YDAY__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getyday(&cur));
	case TPP_KWD___DATE_MONTH__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getmon(&cur));
	case TPP_KWD___DATE_YEAR__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getyear(&cur));
#endif /* !TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
	case TPP_KWD___TIME_SEC__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getsec(&cur));
	case TPP_KWD___TIME_MIN__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_getmin(&cur));
	case TPP_KWD___TIME_HOUR__:
		return tpp_lexer_push_textfile_int(self, tpp_tm_gethour(&cur));
#endif /* !TPP_HAVE_NUMERIC_TIME_MACROS */
	default: tpp_unreachable();
	}
	tpp_unreachable();
}
#endif /* TPP_HAVE_TIME_MACROS */


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


#if TPP_HAVE_MACRO___TPP_IDENTIFIER
struct tpp_lexer_handle_tpp_identifier_data {
	tpp_lexer         *tlhtid_lexer;   /* [1..1] Lexer */
	tpp_keyword const *tlhtid_keyword; /* [1..1] The identified keyword */
};

static TPP_WUNUSED tpp_errno TPPCALL
tpp_lexer_handle_tpp_identifier_cb(void *arg, tpp_string *chunk,
                                   tpp_char const *str, tpp_size length) {
	tpp_keyword const *kwd;
	struct tpp_lexer_handle_tpp_identifier_data *data;
	(void)chunk;
	data = (struct tpp_lexer_handle_tpp_identifier_data *)arg;
	kwd = tpp_keywords_newkeyword(&data->tlhtid_lexer->tl_kwds,
	                              str, length, tpp_hashof(str, length));
	if tpp_unlikely(!kwd)
		return TPP_ENOMEM;
	data->tlhtid_keyword = kwd;
	return TPP_EOK;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_IDENTIFIER(tpp_lexer *tpp_restrict self) {
	struct tpp_lexer_handle_tpp_identifier_data data;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_lexer_arginfo argv[1];
	tpp_token_id tok;
	data.tlhtid_keyword = token->tt_kwd;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	tok = tpp_lexer_seekpp_rparen_exact(self, argv, 1, "__TPP_IDENTIFIER",
	                                    TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL);
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Setup file to (re-)parse the identifier string */
	tpp_file_subtext_push(file);
	tpp_file_subtext_setchunk_fromarg(file, &argv[0]);
	tok = tpp_lexer_yield(self); /* Pre-loaded by `tpp_lexer_seekpp_rparen_exact()', so no need for `tpp_lexer_yield_blocking()' */
	data.tlhtid_lexer = self;
	if (TPP_TOK_ISSTRING(tok)) {
		tpp_errno error;
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_tpp_identifier_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
		tok = TPP_TOK_OFERR_OR_EOF(error);
	}
	if (!TPP_TOK_ISERR(tok)) {
		if (tpp_lexer_gettok(self) != TPP_TOK_EOF) {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
			tok = TPP_TOK_OFERR_OR_EOF(error);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			tok = TPP_TOK_EOF;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
			tpp_lexer_popallfiles(self);
		}
	}
	tpp_file_subtext_pop(file);
	tpp_lexer_arginfo_fini(&argv[0]);

	if (!TPP_TOK_ISERR(tok)) {
		tpp_assert(data.tlhtid_keyword);
		/* Setup current token to refer to "data.tlhtid_keyword" */
		token->tt_id    = tok = data.tlhtid_keyword->tk_id;
		token->tt_kwd   = data.tlhtid_keyword;
/*		token->tt_start = ...;  * Already correct (points at the '__TPP_IDENTIFIER'-keyword) */
/*		token->tt_end   = ...;  * Already correct (points after the trailing ')'-token) */
	}
	return tok;
}
#endif /* TPP_HAVE_MACRO___TPP_IDENTIFIER */


#if TPP_HAVE_MACRO___TPP_EVAL
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_EVAL(tpp_lexer *tpp_restrict self) {
	tpp_expr_value eval_result;
	tpp_string_builder eval_repr_builder;
	TPP_REF tpp_string *eval_repr;
	tpp_ssize eval_repr_print_status;
	tpp_errno error;
	tpp_token_id tok;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}

	/* Setup file to (re-)parse the eval expression */
	error = tpp_lexer_callhook_parseexpr(self, &eval_result);
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return tok;
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Print representation of evaluation result */
	tpp_string_builder_init(&eval_repr_builder);
	eval_repr_print_status = tpp_expr_value_printrepr(&eval_result,
	                                                  &tpp_string_builder_print,
	                                                  &eval_repr_builder);
	tpp_expr_value_fini(&eval_result);
	if tpp_unlikely(TPP_SSIZE_ISERR(eval_repr_print_status)) {
		tpp_string_builder_fini(&eval_repr_builder);
		error = TPP_SSIZE_ASERR(eval_repr_print_status);
		return TPP_TOK_OFERR(error);
	}

	/* Pack representation into a string... */
	eval_repr = tpp_string_builder_pack(&eval_repr_builder);

	/* ... and push that string as a text file. */
	return tpp_lexer_push_textfile_inherited(self, tpp_string_str(eval_repr),
	                                         tpp_string_len(eval_repr),
	                                         eval_repr);
}
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */


#if TPP_HAVE_MACRO___has_include || TPP_HAVE_MACRO___has_include_next
#if TPP_HAVE_LEXER_OPENFILE_EX
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___has_include(tpp_lexer *tpp_restrict self,
                                     tpp_lexer_openfile_flags mask_flags)
#else /* TPP_HAVE_LEXER_OPENFILE_EX */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_simple___has_include(tpp_lexer *tpp_restrict self)
#define tpp_lexer_yield_handle___has_include(self, mask_flags) \
	tpp_lexer_yield_handle_simple___has_include(self)
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
{
	tpp_errno ofr_error;
	tpp_token_id tok;
	char const *expansion_result;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}

	do {
		tok = tpp_lexer_yield_include_string_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;
	if (tok == '"' || tok == '<') {
		tpp_lexer_openfile_result ofr;
#if !TPP_HAVE_LEXER_OPENFILE_EX
		ofr_error = tpp_lexer_open_include_string(self, &ofr);
#else /* !TPP_HAVE_LEXER_OPENFILE_EX */
		/* Enable all header-related flags: if the header has been included
		 * in relation to one of these, then we know it exists without having
		 * to check its file */
#if TPP_HAVE_PRAGMA_ONCE
		mask_flags |= TPP_LEXER_OPENFILE_FLAG_HDR_ONCE;
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
		mask_flags |= TPP_LEXER_OPENFILE_FLAG_HDR_GUARDED;
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_CPP_IMPORT
		mask_flags |= TPP_LEXER_OPENFILE_FLAG_HDR_IMPORTED;
#endif /* TPP_HAVE_CPP_IMPORT */
		ofr_error = tpp_lexer_open_include_string_ex(self, &ofr, mask_flags);
		if (ofr_error == TPP_EMASKED) {
			ofr_error = TPP_EOK;
		} else
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
		{
			if (ofr_error == TPP_EOK)
				tpp_lexer_openfile_result_fini(&ofr);
		}
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING
		ofr_error = tpp_lexer_warnf(self, TPP_W_EXPECTED_INCLUDE_STRING);
		if (!TPP_ISERR(ofr_error))
			ofr_error = TPP_ENOENT;
#else /* TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
		ofr_error = TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
	}
	if (ofr_error != TPP_EOK && ofr_error != TPP_ENOENT)
		return TPP_TOK_OFERR(ofr_error);
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		return tok;
	expansion_result = ofr_error == TPP_ENOENT ? "0" : "1";
	return tpp_lexer_push_textfile_inherited(self, (tpp_char const *)expansion_result, 1, NULL);
}
#endif /* TPP_HAVE_MACRO___has_include || TPP_HAVE_MACRO___has_include_next */


#if TPP_HAVE_MACRO___has_embed

/* Minimal/adjusted parameter handler for __has_embed */
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_embed_builder_handle_param_forhas(tpp_uintmax *tpp_restrict p_limit,
                                      tpp_lexer *tpp_restrict lexer,
                                      tpp_token_id param_kwd);

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___has_embed(tpp_lexer *tpp_restrict self) {
	tpp_lexer_openfile_result ofr;
	tpp_errno ofr_error;
	tpp_token_id tok;
	char const *expansion_result;
	tpp_uintmax param_limit = TPP_UINTMAX_MAX;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}

	do {
		tok = tpp_lexer_yield_include_string_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;
	if (tok == '"' || tok == '<') {
#if TPP_HAVE_LEXER_OPENFILE_EX
		ofr_error = tpp_lexer_open_include_string_ex(self, &ofr, TPP_LEXER_OPENFILE_FLAG_WARN_CASING);
#else /* TPP_HAVE_LEXER_OPENFILE_EX */
		ofr_error = tpp_lexer_open_include_string(self, &ofr);
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
#if TPP_HAVE_LEXER_OPENFILE_EX
		if (ofr_error == TPP_EMASKED)
			ofr_error = TPP_ENOENT; /* Shouldn't happen */
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
	} else {
		tpp_bzero(&ofr, sizeof(ofr)); /* To prevent compiler warnings; init here isn't actually necessary */
#if TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING
		ofr_error = tpp_lexer_warnf(self, TPP_W_EXPECTED_INCLUDE_STRING);
		if (!TPP_ISERR(ofr_error))
			ofr_error = TPP_ENOENT;
#else /* TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
		ofr_error = TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
	}
	if (ofr_error != TPP_EOK && ofr_error != TPP_ENOENT)
		return TPP_TOK_OFERR(ofr_error);

	/* Parse extra parameters */
	for (;;) {
		tpp_errno error;
		/* Yield to the first parameter (or just straight to the trailing ')') */
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			goto err_tok_ofr;
		if (!TPP_TOK_ISKEYWORD(tok))
			break;
		error = tpp_embed_builder_handle_param_forhas(&param_limit, self, tok);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto err_tok_ofr;
		}
	}

	/* Determine result of test */
	if (ofr_error != TPP_EOK) {
		expansion_result = TPP_CONFIG_VALUEOF_STDC_EMBED_NOT_FOUND;
	} else {
		if (param_limit == 0) {
			/* Always empty! */
			expansion_result = TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY;
		} else {
			unsigned char first_byte;
			tpp_ssize read_status = tpp_io_read_blocking(ofr.tlofr_handle, &first_byte, 1);
			if tpp_unlikely(TPP_SSIZE_ISERR(read_status)) {
				tok = TPP_TOK_OFERR(TPP_SSIZE_ASERR(read_status));
				goto err_tok_ofr;
			}
			if (read_status == 0) {
				expansion_result = TPP_CONFIG_VALUEOF_STDC_EMBED_EMPTY;
			} else {
				expansion_result = TPP_CONFIG_VALUEOF_STDC_EMBED_FOUND;
			}
		}
		tpp_lexer_openfile_result_fini(&ofr);
	}

	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		return tok;
	return tpp_lexer_push_textfile_inherited(self, (tpp_char const *)expansion_result,
	                                         tpp_strlen(expansion_result), NULL);
err_tok_ofr:
	if (ofr_error == TPP_EOK)
		tpp_lexer_openfile_result_fini(&ofr);
	return tok;
}
#endif /* TPP_HAVE_MACRO___has_embed */


#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE || TPP_HAVE_MACRO___TPP_STR_SUBSTR
struct tpp_lexer_handle_str_decompile_string_data {
	TPP_REF tpp_string *tlhsdsd_chunk;   /* [0..1] Out: chunk containing string */
	tpp_char const     *tlhsdsd_str;     /* String to decompile */
	tpp_size            tlhsdsd_length;  /* Length of "tlhsdsd_str" */
};
static TPP_WUNUSED tpp_errno TPPCALL
tpp_lexer_handle_str_decompile_string(void *arg, tpp_string *chunk,
                                      tpp_char const *str, tpp_size length) {
	struct tpp_lexer_handle_str_decompile_string_data *data;
	data = (struct tpp_lexer_handle_str_decompile_string_data *)arg;
	if (chunk)
		tpp_string_incref(chunk);
	data->tlhsdsd_chunk  = chunk;
	data->tlhsdsd_str    = str;
	data->tlhsdsd_length = length;
	return TPP_EOK;
}
#endif /* TPP_HAVE_MACRO___TPP_STR_DECOMPILE || TPP_HAVE_MACRO___TPP_STR_SUBSTR */

#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_STR_DECOMPILE(tpp_lexer *tpp_restrict self) {
	struct tpp_lexer_handle_str_decompile_string_data data;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file;
	tpp_lexer_arginfo argv[1];
	tpp_token_id tok;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	tok = tpp_lexer_seekpp_rparen_exact(self, argv, 1, "__TPP_STR_DECOMPILE",
	                                    TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL);
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Setup file to (re-)parse the string that's being decompiled */
	tpp_file_subtext_push(file);
	tpp_file_subtext_setchunk_fromarg(file, &argv[0]);
	tok = tpp_lexer_yield(self); /* Pre-loaded by `tpp_lexer_seekpp_rparen_exact()', so no need for `tpp_lexer_yield_blocking()' */
	if (TPP_TOK_ISSTRING(tok)) {
		tpp_errno error;
		/* IMPORTANT: Don't set "TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS" here! */
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_str_decompile_string,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		tok = TPP_TOK_OFERR_OR_EOF(error);
	} else {
		data.tlhsdsd_chunk  = NULL;
		data.tlhsdsd_str    = NULL;
		data.tlhsdsd_length = 0;
	}
	if (!TPP_TOK_ISERR(tok)) {
		if (tpp_lexer_gettok(self) != TPP_TOK_EOF) {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
			if (TPP_ISERR(error)) {
				if (data.tlhsdsd_chunk)
					tpp_string_decref(data.tlhsdsd_chunk);
				tok = TPP_TOK_OFERR(error);
			}
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			tok = TPP_TOK_EOF;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
			tpp_lexer_popallfiles(self);
		}
	}
	tpp_file_subtext_pop(file);
	tpp_lexer_arginfo_fini(&argv[0]);
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Check for special case: empty string -> don't have to decompile anything! */
	if (data.tlhsdsd_length == 0) {
		if (data.tlhsdsd_chunk)
			tpp_string_decref(data.tlhsdsd_chunk);
		return TPP_TOK_EOF;
	}

	/* Push a sub-text file describing the decoded contents of the string */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file) {
		if (data.tlhsdsd_chunk)
			tpp_string_decref(data.tlhsdsd_chunk);
		return TPP_TOK_ENOMEM;
	}
	*prev_file = *file;
	file->tf_kind  = TPP_FILE_KIND_SUBTEXT
	_tpp_file_subtext_init_ifdef(file)
	_tpp_file_init_lcpos(file);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	file->tf_chunk = data.tlhsdsd_chunk; /* Inherit reference */
	file->tf_pos   = data.tlhsdsd_str;
	file->tf_end   = data.tlhsdsd_str + data.tlhsdsd_length;
	return TPP_TOK_EOF; /* Instruct caller to yield the first token from the subtext file */
}
#endif /* !TPP_HAVE_MACRO___TPP_STR_DECOMPILE */


#if TPP_HAVE_MACRO___TPP_STR_PACK
static TPP_FORMATPRINTER_DEFINE(tpp_string_builder_print_encoded, arg, text, num_bytes) {
	return tpp_token_encodestring(&tpp_string_builder_print, arg, text, num_bytes);
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_STR_PACK(tpp_lexer *tpp_restrict self) {
	tpp_string_builder builder;
	TPP_REF tpp_string *string;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file;
	tpp_lexer_arginfo argv[1];
	tpp_token_id tok;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	/* TODO: Re-write this to not use `tpp_lexer_seekpp_rparen_exact`, but parse tokens directly! */
	tok = tpp_lexer_seekpp_rparen_exact(self, argv, 1, "__TPP_STR_PACK",
	                                    TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL |
	                                    TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS);
	if (TPP_TOK_ISERR(tok))
		return tok;
	tpp_string_builder_init(&builder);

	/* Setup file to (re-)parse the string that's being decompiled */
	tpp_file_subtext_push(file);
	tpp_file_subtext_setchunk_fromarg(file, &argv[0]);
	if (tpp_string_builder_print(&builder, (tpp_char const *)"\"", 1) < 0) {
err_tok_subtext_builder:
		tpp_file_subtext_break(file);
		tpp_string_builder_fini(&builder);
		tpp_lexer_arginfo_fini(&argv[0]);
		return tok;
	}
	for (;;) {
		tpp_ssize status;
		tok = tpp_lexer_yield(self); /* Pre-loaded by `tpp_lexer_seekpp_rparen_exact()', so no need for `tpp_lexer_yield_blocking()' */
		switch (tok) {
		case TPP_TOK_EOF:
			goto done_inner_loop;

		case TPP_TOK_SPACE:
		case TPP_TOK_LF:
		TPP_CASE_TPP_TOK_COMMENT
		case ',': /* Ignore all ','-token in here! */
			break;

		TPP_CASE_TPP_TOK_STRING {
			tpp_lexer_decodestring_config config;
			tpp_lexer_decodestring_config_init_simple(&config, &tpp_string_builder_print_encoded, &builder);
			status = tpp_lexer_decodestring(self, &config);
handle_status:
			if (TPP_SSIZE_ISERR(status)) {
				tok = TPP_TOK_OFERR(TPP_SSIZE_ASERR(status));
				goto err_tok_subtext_builder;
			}
		}	break;

#if TPP_HAVE_TOK_INT
		TPP_CASE_TPP_TOK_INT {
			tpp_intmax value;
			tpp_char value_ch[1];
			tpp_errno error = tpp_lexer_decodeint(self, &value);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				goto err_tok_subtext_builder;
			}
			value_ch[0] = (tpp_char)value;
			status = tpp_string_builder_print_encoded(&builder, value_ch, 1);
			goto handle_status;
		}	break;
#endif /* TPP_HAVE_TOK_INT */

		default:
			if (TPP_TOK_ISERR(tok))
				goto err_tok_subtext_builder;
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK
			{
				tpp_errno error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK);
				if (TPP_ISERR(error)) {
					tok = TPP_TOK_OFERR(error);
					goto err_tok_subtext_builder;
				}
			}
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_TPP_STR_PACK */
			break;
		}
	}
done_inner_loop:
	if (tpp_string_builder_print(&builder, (tpp_char const *)"\"", 1) < 0)
		goto err_tok_subtext_builder;
	tpp_file_subtext_pop(file);
	tpp_lexer_arginfo_fini(&argv[0]);
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Push a sub-text file describing the decoded contents of the string */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file) {
		tpp_string_builder_fini(&builder);
		return TPP_TOK_ENOMEM;
	}
	tpp_file_move(prev_file, file);
	string = tpp_string_builder_pack(&builder);
	tpp_file_init_text_ex(file, NULL, string,
	                      tpp_string_str(string),
	                      tpp_string_len(string),
	                      TPP_LCINFO_INVALID,
	                      TPP_FILE_FLAGS_NORMAL,
	                      file->tf_enc);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	return TPP_TOK_EOF; /* Instruct caller to yield the first token from the subtext file */
}
#endif /* !TPP_HAVE_MACRO___TPP_STR_PACK */


#if TPP_HAVE_MACRO___TPP_COUNT_TOKENS
struct tpp_lexer_handle_count_tokens_data {
	tpp_lexer *tlhctd_lexer; /* [1..1] */
	tpp_intmax tlhctd_count; /* Token count */
};

static TPP_WUNUSED tpp_errno TPPCALL
tpp_lexer_handle_count_tokens(void *arg, tpp_string *chunk, tpp_char const *str, tpp_size length) {
	tpp_token_id tok;
	struct tpp_lexer_handle_count_tokens_data *const data = (struct tpp_lexer_handle_count_tokens_data *)arg;
	tpp_lexer *const self = data->tlhctd_lexer;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file_subtext_push(file);
	tpp_file_subtext_setchunk_fromstring(file, chunk, str, length);
	for (;;) {
		tok = tpp_lexer_yieldraw(self);
		if (TPP_TOK_ISERR_OR_EOF(tok))
			break;
		/* Important: our result must *NOT* depend on "TPP_LEXER_STATE_FLAG_ALLTOKENS",
		 *            however it *does* have to depend on "tpp_lexer_has()"! */
		switch (tok) {
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SPACE)
		case TPP_TOK_SPACE:
			if (!tpp_lexer_has(self, TOK_SPACE))
				continue;
			break;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TOK_SPACE) */
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_LF)
		case TPP_TOK_LF:
			if (!tpp_lexer_has(self, TOK_LF))
				continue;
			break;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TOK_LF) */
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_COMMENT)
		TPP_CASE_TPP_TOK_COMMENT
			if (!tpp_lexer_has(self, TOK_COMMENT))
				continue;
			break;
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TOK_COMMENT) */
		default: break;
		}
		++data->tlhctd_count;
	}
	tpp_file_subtext_pop(file);
	return TPP_TOK_ASERR_OR_EOK(tok);
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_COUNT_TOKENS(tpp_lexer *tpp_restrict self) {
	struct tpp_lexer_handle_count_tokens_data data;
	tpp_token_id tok;
	tpp_errno error;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	data.tlhctd_lexer = self;
	data.tlhctd_count = 0;
	if (!TPP_TOK_ISSTRING(tok)) {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	} else {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_count_tokens,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	}
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return tok;
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		return tok;
	return tpp_lexer_push_textfile_int(self, data.tlhctd_count);
}
#endif /* !TPP_HAVE_MACRO___TPP_COUNT_TOKENS */


#if TPP_HAVE_MACRO___TPP_STR_SIZE
static TPP_FORMATPRINTER_DEFINE(tpp_lexer_handle_str_size, arg, str, length) {
	(void)arg;
	(void)str;
	return (tpp_ssize)length;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_STR_SIZE(tpp_lexer *tpp_restrict self) {
	tpp_size str_length;
	tpp_token_id tok;
	tpp_errno error;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;

	str_length = 0;
	error = TPP_EOK;
	if (!TPP_TOK_ISSTRING(tok)) {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#endif /* TPP_HAVE_TPP_W_EXPECTED_STRING */
	} else {
		tpp_ssize status;
		tpp_lexer_decodestring_config config;
		tpp_lexer_decodestring_config_init_simple(&config, &tpp_lexer_handle_str_size, NULL);
		status = tpp_lexer_parsestring_ex(self, &config, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
		if (TPP_SSIZE_ISERR(status)) {
			error = TPP_SSIZE_ASERR(status);
		} else {
			str_length = (tpp_size)status;
		}
	}
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return tok;
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		return tok;
	return tpp_lexer_push_textfile_int(self, (tpp_intmax)(tpp_uintmax)str_length);
}
#endif /* !TPP_HAVE_MACRO___TPP_STR_SIZE */




#if TPP_HAVE_MACRO___TPP_EXEC
struct tpp_lexer_handle_exec_data {
	tpp_string_builder tlhed_builder; /* Expansion string builder */
	tpp_lexer         *tlhed_lexer;   /* [1..1] Lexer */
};
static TPP_WUNUSED tpp_errno TPPCALL
tpp_lexer_handle_exec_cb(void *arg, tpp_string *chunk,
                         tpp_char const *str, tpp_size length) {
	tpp_token_id tok;
	struct tpp_lexer_handle_exec_data *const data = (struct tpp_lexer_handle_exec_data *)arg;
	tpp_lexer *const self = data->tlhed_lexer;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token const *const token = tpp_lexer_gettoken(self);
#if TPP_HAVE_MAGIC_WHITESPACE
	tpp_token_id prev_tok = TPP_TOK_SPACE;
#endif /* TPP_HAVE_MAGIC_WHITESPACE */
#if TPP_HAVE_CPP_DIRECTIVES
	tpp_file_flags saved_flags;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
	tpp_lexer_state_flags saved_state;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
	tpp_file_subtext_push(file);
	tpp_file_subtext_setchunk_fromstring(file, chunk, str, length);

	/* Allow directive parsing starting with the first token */
#if TPP_HAVE_CPP_DIRECTIVES
	saved_flags = file->tf_flags;
	file->tf_flags &= ~TPP_FILE_FLAGS_NODIRECTIVES;
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
	saved_state = self->tl_state;
	self->tl_state &= ~TPP_LEXER_STATE_FLAG_ALLTOKENS;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */

	/* Pretty much the same as "tpp_lexer_pragma_tpp_exec_cb()", but gather token reprs */
	for (;;) {
		tpp_ssize print_status;
		tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR_OR_EOF(tok))
			break;
#if TPP_HAVE_MAGIC_WHITESPACE
		if (tpp_lexer_has(self, MAGIC_WHITESPACE)) {
			if (tpp_lexer_require_whitespace(self, prev_tok, tok)) {
				print_status = tpp_string_builder_print(&data->tlhed_builder, (tpp_char const *)" ", 1);
				if tpp_unlikely(TPP_SSIZE_ISERR(print_status)) {
					tok = TPP_TOK_OFERR(TPP_SSIZE_ASERR(print_status));
					break;
				}
			}
			prev_tok = tok;
		}
#endif /* TPP_HAVE_MAGIC_WHITESPACE */
		print_status = tpp_string_builder_print(&data->tlhed_builder,
		                                        tpp_token_getstart(token),
		                                        tpp_token_getlen(token));
		if tpp_unlikely(TPP_SSIZE_ISERR(print_status)) {
			tok = TPP_TOK_OFERR(TPP_SSIZE_ASERR(print_status));
			break;
		}
	}

	tpp_lexer_popallfiles(self);
#if TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS
	self->tl_state = saved_state;
#endif /* TPP_HAVE_LEXER_STATE_FLAG_ALLTOKENS */
#if TPP_HAVE_CPP_DIRECTIVES
	file->tf_flags |= (saved_flags & TPP_FILE_FLAGS_NODIRECTIVES);
#endif /* TPP_HAVE_CPP_DIRECTIVES */
	tpp_file_subtext_pop(file);
	return TPP_TOK_ASERR_OR_EOK(tok);
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_EXEC(tpp_lexer *tpp_restrict self) {
	struct tpp_lexer_handle_exec_data data;
	TPP_REF tpp_string *exec_result;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file;
	tpp_lexer_arginfo argv[1];
	tpp_token_id tok;

	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	tok = tpp_lexer_seekpp_rparen_exact(self, argv, 1, "__TPP_EXEC",
	                                    TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL);
	if (TPP_TOK_ISERR(tok))
		return tok;

	/* Setup file to (re-)parse the string that's being exec'd */
	tpp_file_subtext_push(file);
	tpp_file_subtext_setchunk_fromarg(file, &argv[0]);
	tok = tpp_lexer_yield(self); /* Pre-loaded by `tpp_lexer_seekpp_rparen_exact()', so no need for `tpp_lexer_yield_blocking()' */

	tpp_string_builder_init(&data.tlhed_builder);
	data.tlhed_lexer = self;
	if (TPP_TOK_ISSTRING(tok)) {
		tpp_errno error;
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_exec_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
		tok = TPP_TOK_OFERR_OR_EOF(error);
	}
	if (!TPP_TOK_ISERR(tok)) {
		if (tpp_lexer_gettok(self) != TPP_TOK_EOF) {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
			tok = TPP_TOK_OFERR_OR_EOF(error);
#endif /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			tpp_lexer_popallfiles(self);
		}
	}
	tpp_file_subtext_pop(file);
	tpp_lexer_arginfo_fini(&argv[0]);
	if (TPP_TOK_ISERR(tok)) {
		tpp_string_builder_fini(&data.tlhed_builder);
		return tok;
	}

	/* Push a sub-text file describing the decoded contents of the string */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file) {
		tpp_string_builder_fini(&data.tlhed_builder);
		return TPP_TOK_ENOMEM;
	}
	*prev_file = *file;
	exec_result = tpp_string_builder_pack(&data.tlhed_builder);
	tpp_file_init_text_ex(file, NULL, exec_result,
	                      tpp_string_str(exec_result),
	                      tpp_string_len(exec_result),
	                      TPP_LCINFO_INVALID,
	                      TPP_FILE_FLAGS_NORMAL,
	                      file->tf_enc);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	return TPP_TOK_EOF;
}
#endif /* !TPP_HAVE_MACRO___TPP_EXEC */




#if TPP_HAVE_MACRO___TPP_RANDOM || TPP_HAVE_MACRO___TPP_STR_SUBSTR
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_parse_simple_int(tpp_lexer *tpp_restrict self, tpp_intmax *p_value) {
	bool neg = false;
	tpp_token_id tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
again_yield:
		tok = tpp_lexer_yield_blocking(self);
	}
	if (TPP_TOK_ISERR(tok))
		return tok;
	switch (tok) {
	case '-':
		neg = !neg;
		goto again_yield;
#if TPP_HAVE_TOK_MINUS_MINUS
	case TPP_TOK_MINUS_MINUS:
		goto again_yield;
#endif /* TPP_HAVE_TOK_MINUS_MINUS */
	default: break;
	}
	tok = tpp_lexer_require_number(self);
	if (TPP_TOK_ISERR(tok))
		return tok;
	*p_value = 0;
	if (TPP_TOK_ISINT(tok)) {
		tpp_errno error;
		error = tpp_lexer_decodeint(self, p_value);
		if (TPP_ISERR(error))
			return TPP_TOK_OFERR(error);
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	}
	return tok;
}
#endif /* TPP_HAVE_MACRO___TPP_RANDOM || TPP_HAVE_MACRO___TPP_STR_SUBSTR */

#if TPP_HAVE_MACRO___TPP_RANDOM
#if TPP_SIZEOF_tpp_hash == 4
#define TPP_HASH_MAX TPP_HASH_C(0xffffffff)
#elif TPP_SIZEOF_tpp_hash == 8
#define TPP_HASH_MAX TPP_HASH_C(0xffffffffffffffff)
#elif TPP_SIZEOF_tpp_hash == 2
#define TPP_HASH_MAX TPP_HASH_C(0xffff)
#elif TPP_SIZEOF_tpp_hash == 1
#define TPP_HASH_MAX TPP_HASH_C(0xff)
#else /* ... */
#error "Unsupported 'TPP_SIZEOF_tpp_hash'"
#endif /* !... */

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_RANDOM(tpp_lexer *tpp_restrict self) {
	tpp_intmax lo, hi, result;
	tpp_token_id tok;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return tok;
	tok = tpp_lexer_parse_simple_int(self, &lo);
	if (TPP_TOK_ISERR(tok))
		return tok;
	if (tok == ',') {
		tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return tok;
		tok = tpp_lexer_parse_simple_int(self, &hi);
		if (TPP_TOK_ISERR(tok))
			return tok;
	} else {
		hi = lo;
		lo = 0;
	}
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		return tok;
	result = lo;
	if (lo < hi) {
		tpp_uintmax range = (tpp_uintmax)(hi - lo);
		tpp_uintmax uresult = tpp_lexer_nextrand(self);
#if TPP_HASH_MAX < TPP_UINTMAX_MAX
		if (range > TPP_HASH_MAX) {
			uresult <<= (TPP_SIZEOF_tpp_hash * TPP_CHAR_BIT);
			uresult |= tpp_lexer_nextrand(self);
		}
#endif /* TPP_HASH_MAX < TPP_UINTMAX_MAX */
		uresult %= range; /* XXX: This isn't fair if "TPP_UINTMAX_MAX+1" isn't a multiple of "range" */
		result += (tpp_intmax)uresult;
	}
	return tpp_lexer_push_textfile_int(self, result);
}
#endif /* !TPP_HAVE_MACRO___TPP_RANDOM */




#if TPP_HAVE_MACRO___TPP_STR_SUBSTR
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_STR_SUBSTR(tpp_lexer *tpp_restrict self,
                                        tpp_char quote_ch) {
	struct tpp_lexer_handle_str_decompile_string_data data;
	tpp_intmax index, length;
	tpp_token_id tok;
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file;
	tpp_string_builder result_builder;
	TPP_REF tpp_string *result_str;
	tpp_ssize print_status;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return tok;
	if (TPP_TOK_ISSTRING(tok)) {
		/* IMPORTANT: Don't set "TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS" here! */
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_str_decompile_string,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_TOK_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
		data.tlhsdsd_chunk  = NULL;
		data.tlhsdsd_str    = NULL;
		data.tlhsdsd_length = 0;
	}
	if (TPP_ISERR(error))
		return TPP_TOK_OFERR(error);
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(','));
	if (TPP_TOK_ISERR(tok))
		goto err_tok_data;
	tok = tpp_lexer_parse_simple_int(self, &index);
	if (TPP_TOK_ISERR(tok))
		goto err_tok_data;
	if (tok == ',') {
		tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			goto err_tok_data;
		tok = tpp_lexer_parse_simple_int(self, &length);
		if (TPP_TOK_ISERR(tok))
			goto err_tok_data;
	} else {
		length = 1;
	}
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		goto err_tok_data;
	if ((tpp_uintmax)index >= (tpp_uintmax)data.tlhsdsd_length)
		index = (tpp_intmax)(tpp_uintmax)data.tlhsdsd_length;
	if (length < 0 || ((tpp_uintmax)index + (tpp_uintmax)length) < (tpp_uintmax)index ||
	    ((tpp_uintmax)index + (tpp_uintmax)length) > (tpp_uintmax)data.tlhsdsd_length)
		length = (tpp_intmax)(tpp_uintmax)(tpp_size)(data.tlhsdsd_length - (tpp_size)index);
	data.tlhsdsd_str += (tpp_size)(tpp_uintmax)index;
	data.tlhsdsd_length = (tpp_size)(tpp_uintmax)length;
	tpp_string_builder_init(&result_builder);
	print_status = tpp_string_builder_print(&result_builder, &quote_ch, 1);
	if tpp_unlikely(print_status < 0) {
		tok = TPP_TOK_OFERR(TPP_SSIZE_ASERR(print_status));
		goto err_tok_data_result_builder;
	}
	print_status = tpp_token_encodestring(&tpp_string_builder_print, &result_builder,
	                                      data.tlhsdsd_str, data.tlhsdsd_length);
	if tpp_unlikely(print_status < 0) {
		tok = TPP_TOK_OFERR(TPP_SSIZE_ASERR(print_status));
		goto err_tok_data_result_builder;
	}
	print_status = tpp_string_builder_print(&result_builder, &quote_ch, 1);
	if tpp_unlikely(print_status < 0) {
		tok = TPP_TOK_OFERR(TPP_SSIZE_ASERR(print_status));
		goto err_tok_data_result_builder;
	}
	result_str = tpp_string_builder_pack(&result_builder);
	if (data.tlhsdsd_chunk)
		tpp_string_decref(data.tlhsdsd_chunk);

	/* Push the substring as a new file */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem_result_str;
	tpp_file_move(prev_file, file);
	tpp_file_init_text_ex(file, NULL, result_str,
	                      tpp_string_str(result_str),
	                      tpp_string_len(result_str),
	                      TPP_LCINFO_INVALID,
	                      TPP_FILE_FLAGS_NORMAL,
	                      prev_file->tf_enc);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	return TPP_TOK_EOF;
err_nomem_result_str:
	data.tlhsdsd_chunk = result_str;
	tok = TPP_TOK_ENOMEM;
err_tok_data:
	if (data.tlhsdsd_chunk)
		tpp_string_decref(data.tlhsdsd_chunk);
	return tok;
err_tok_data_result_builder:
	tpp_string_builder_fini(&result_builder);
	goto err_tok_data;
}
#endif /* !TPP_HAVE_MACRO___TPP_STR_SUBSTR */




#if TPP_HAVE_MACRO___TPP_LOAD_FILE
struct tpp_string_builder_inplace_escape_data {
	tpp_string_builder *tsbied_builder; /* [1..1] Builder */
	tpp_char           *tsbied_text;    /* [0..tsbied_size] Remaining data to escape */
	tpp_size            tsbied_size;    /* Size of "tsbied_size" (in bytes) */
};

#define TPP_STRING_BUILDER_INPLACE_ESCAPE_RESTART ((tpp_ssize)(TPP_ELAST - 1))

static TPP_FORMATPRINTER_DEFINE(tpp_string_builder_inplace_escape_cb, arg, text, num_bytes) {
	tpp_char *buf;
	tpp_size offset, delta_size, remaining;
	struct tpp_string_builder_inplace_escape_data *data;
	data = (struct tpp_string_builder_inplace_escape_data *)arg;
	if (data->tsbied_text == text) {
		tpp_assert(data->tsbied_size >= num_bytes);
		data->tsbied_text += num_bytes;
		data->tsbied_size -= num_bytes;
		return 0;
	}

	/* Replace the next byte with "text...+=num_bytes" */
	--data->tsbied_size;
	++data->tsbied_text;
	delta_size = num_bytes - 1;

	/* Allocate additional space for escaped data. */
	offset = data->tsbied_text - data->tsbied_builder->tsb_buf->ts_str;
	buf = tpp_string_builder_alloc(data->tsbied_builder, delta_size);
	data->tsbied_text = data->tsbied_builder->tsb_buf->ts_str + offset;
	if tpp_unlikely(!buf)
		return TPP_SSIZE_OFERR(TPP_ENOMEM);

	/* Move buffer up to make space for escaped representation */
	remaining = (data->tsbied_builder->tsb_buf->ts_str +
	             tpp_string_builder_getlen(data->tsbied_builder)) -
	            data->tsbied_text;
	tpp_memmoveup(data->tsbied_text + delta_size,
	              data->tsbied_text, remaining * sizeof(tpp_char));

	/* Inject escaped representation */
	tpp_memcpy(data->tsbied_text - 1, text, num_bytes * sizeof(tpp_char));
	data->tsbied_text += delta_size;

	/* Tell caller to restart "tpp_token_encodestring()" from updated base position */
	return TPP_STRING_BUILDER_INPLACE_ESCAPE_RESTART;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_string_builder_inplace_escape(tpp_string_builder *tpp_restrict self,
                                  tpp_char *text, tpp_size num_bytes) {
	struct tpp_string_builder_inplace_escape_data data;
	data.tsbied_builder = self;
	data.tsbied_text    = text;
	data.tsbied_size    = num_bytes;
	while (data.tsbied_size) {
		tpp_ssize status = tpp_token_encodestring(&tpp_string_builder_inplace_escape_cb,
		                                          &data, data.tsbied_text, data.tsbied_size);
		if (status == TPP_STRING_BUILDER_INPLACE_ESCAPE_RESTART)
			continue;
		if (status == 0) {
			tpp_assert(data.tsbied_size == 0);
			break;
		}
		tpp_assert(TPP_SSIZE_ISERR(status));
		return TPP_SSIZE_ASERR(status);
	}
	return TPP_EOK;
}

/* Read all data from "handle", escape it like "tpp_token_encodestring"
 * would, then append the escaped contents onto "self". */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_string_builder_print_escaped_file(tpp_string_builder *tpp_restrict self,
                                      tpp_io_handle handle) {
	tpp_size bufsize = 512;
	for (;;) {
		tpp_size new_bufsize;
		tpp_errno error;
		tpp_ssize read_status;
		tpp_size used_bufsize = bufsize;
		tpp_char *buf = tpp_string_builder_tryalloc(self, used_bufsize);
		if tpp_unlikely(!buf) {
			used_bufsize = 1;
			buf = tpp_string_builder_alloc(self, used_bufsize);
			if tpp_unlikely(!buf)
				goto err_nomem;
		}
		read_status = tpp_io_read_blocking(handle, buf, used_bufsize);
		if (TPP_SSIZE_ISERR(read_status))
			return TPP_SSIZE_ASERR(read_status);
		if (used_bufsize > (tpp_size)read_status) {
			tpp_size unused = used_bufsize - (tpp_size)read_status;
			tpp_string_builder_release(self, unused);
		}
		error = tpp_string_builder_inplace_escape(self, buf, (tpp_size)read_status);
		if (TPP_ISERR(error))
			return error;
		if ((tpp_size)read_status < used_bufsize)
			break; /* Done! */
		new_bufsize = tpp_string_builder_getlen(self) >> 1;
		if (new_bufsize > 64 * 1024)
			new_bufsize = 64 * 1024;
		if (bufsize < new_bufsize)
			bufsize = new_bufsize;
	}
	return TPP_EOK;
err_nomem:
	return TPP_ENOMEM;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle___TPP_LOAD_FILE(tpp_lexer *tpp_restrict self) {
	/* Pretty much the same as:
	 * >> #define __TPP_LOAD_FILE(filename) __TPP_STR_PACK(__TPP_EXEC("#embed " #filename)) */
	tpp_lexer_openfile_result ofr;
	tpp_errno ofr_error;
	tpp_token_id tok;
	TPP_REF tpp_string *result_str;
	tpp_string_builder result_builder;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file;
	tok = tpp_lexer_tryskip_raw(self, TPP_TOK_OFCHAR('('),
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_NORMAL);
	if (tok != TPP_TOK_OFCHAR('(')) {
		if (!TPP_TOK_ISERR(tok))
			tok = tpp_lexer_gettok(self);
		return tok;
	}
	do {
		tok = tpp_lexer_yield_include_string_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return tok;
	if (tok == '"' || tok == '<') {
#if TPP_HAVE_LEXER_OPENFILE_EX
		ofr_error = tpp_lexer_open_include_string_ex(self, &ofr, TPP_LEXER_OPENFILE_FLAG_WARN_CASING);
#else /* TPP_HAVE_LEXER_OPENFILE_EX */
		ofr_error = tpp_lexer_open_include_string(self, &ofr);
#endif /* !TPP_HAVE_LEXER_OPENFILE_EX */
#if TPP_HAVE_LEXER_OPENFILE_EX
		if (ofr_error == TPP_EMASKED)
			ofr_error = TPP_ENOENT; /* Shouldn't happen */
#endif /* TPP_HAVE_LEXER_OPENFILE_EX */
	} else {
		tpp_bzero(&ofr, sizeof(ofr)); /* To prevent compiler warnings; init here isn't actually necessary */
#if TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING
		ofr_error = tpp_lexer_warnf(self, TPP_W_EXPECTED_INCLUDE_STRING);
		if (!TPP_ISERR(ofr_error))
			ofr_error = TPP_ENOENT;
#else /* TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
		ofr_error = TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
	}
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		goto err_tok_ofr;
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR(')'));
	if (TPP_TOK_ISERR(tok))
		goto err_tok_ofr;

	/* Load+escape the entire file into a string literal.
	 * NOTE: There'd be no point in doing something similar to `TPP_FILE_ENCODING_EMBED',
	 *       since unlike `#embed', which can just lazily load file-data because it produces
	 *       a `,'-separated sequence of integer tokens, we're actually only producing a
	 *       single string token, which would just be loaded in its entirety anyways the
	 *       next time a call to `tpp_lexer_yieldraw()' was made -- so we wouldn't gain
	 *       anything...
	 * In order for something like that to make sense, `tpp_lexer_yieldraw()' would firstly
	 * need to be able to yield the start of a string token *without* having to parse that
	 * string token in its entirety first -- which would be impossible since it'd mean that
	 * when `tpp_lexer_yieldraw()' returns after partially loading a string-encoded file,
	 * there'd be no valid value that could be returned by `tpp_lexer_gettokenend()' (since
	 * the end of the token wouldn't be loaded yet at that point). */
	tpp_string_builder_init(&result_builder);
	if (tpp_string_builder_print(&result_builder, (tpp_char const *)"\"", 1) < 0) {
err_nomem_ofr_result_builder:
		tok = TPP_TOK_ENOMEM;
err_tok_ofr_result_builder:
		tpp_string_builder_fini(&result_builder);
		goto err_tok_ofr;
	}
	if (ofr_error == TPP_EOK) {
		tpp_errno error = tpp_string_builder_print_escaped_file(&result_builder, ofr.tlofr_handle);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			goto err_tok_ofr_result_builder;
		}
		tpp_lexer_openfile_result_fini(&ofr);
	}
	if (tpp_string_builder_print(&result_builder, (tpp_char const *)"\"", 1) < 0)
		goto err_nomem_ofr_result_builder;
	result_str = tpp_string_builder_pack(&result_builder);

	/* Push the substring as a new file */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem_result_str;
	tpp_file_move(prev_file, file);
	tpp_file_init_text_ex(file, NULL, result_str,
	                      tpp_string_str(result_str),
	                      tpp_string_len(result_str),
	                      TPP_LCINFO_INVALID,
	                      TPP_FILE_FLAGS_NORMAL,
	                      prev_file->tf_enc);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	return TPP_TOK_EOF;
err_tok_ofr:
	if (ofr_error == TPP_EOK)
		tpp_lexer_openfile_result_fini(&ofr);
	return tok;
err_nomem_result_str:
	tpp_string_decref(result_str);
	return TPP_TOK_ENOMEM;
}
#endif /* !TPP_HAVE_MACRO___TPP_LOAD_FILE */




#if TPP_HAVE_CPP_BUILTIN_MACROS
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
#if TPP_HAVE_MACRO___has_include
	case TPP_KWD___has_include:
		return tpp_lexer_yield_handle___has_include(self, TPP_LEXER_OPENFILE_FLAG_WARN_CASING);
#endif /* TPP_HAVE_MACRO___has_include */
#if TPP_HAVE_MACRO___has_include_next
	case TPP_KWD___has_include_next:
		return tpp_lexer_yield_handle___has_include(self, TPP_LEXER_OPENFILE_FLAG_WARN_CASING | TPP_LEXER_OPENFILE_FLAG_INCLUDE_NEXT);
#endif /* TPP_HAVE_MACRO___has_include_next */
/************************************************************************/



/************************************************************************/
#ifdef TPP_HAVE_MACRO___has_embed
	case TPP_KWD___has_embed:
		return tpp_lexer_yield_handle___has_embed(self);
#endif /* TPP_HAVE_MACRO___has_embed */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___FILE__
	case TPP_KWD___FILE__: {
		char const *filename = tpp_lexer_getlcfilename(self);
		if (filename == NULL)
			filename = "?";
		return tpp_lexer_push_textfile_string_esc(self, (tpp_char const *)filename,
		                                          tpp_strlen(filename));
	}	break;
#endif /* TPP_HAVE_MACRO___FILE__ */
#if TPP_HAVE_MACRO___BASE_FILE__
	case TPP_KWD___BASE_FILE__: {
		tpp_file const *file = tpp_file_getbasefile(tpp_lexer_getfile(self));
		char const *filename = tpp_file_getfilename(file);
		if (filename == NULL)
			filename = "?";
		return tpp_lexer_push_textfile_string_esc(self, (tpp_char const *)filename,
		                                          tpp_strlen(filename));
	}	break;
#endif /* TPP_HAVE_MACRO___BASE_FILE__ */
#if TPP_HAVE_MACRO___FILE_NAME__
	case TPP_KWD___FILE_NAME__: {
		tpp_file const *file = tpp_lexer_getlcfile(self);
		char const *basename;
		char const *filename = tpp_file_getfilename(file);
		if (filename == NULL)
			filename = "?";
		basename = filename;
		for (;;) {
			char ch = *filename++;
			if (!ch)
				break;
			if (TPP_FS_ISSEP(ch))
				basename = filename;
		}
		return tpp_lexer_push_textfile_string_esc(self, (tpp_char const *)basename,
		                                          tpp_strlen(basename));
	}	break;
#endif /* TPP_HAVE_MACRO___FILE_NAME__ */
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
#if TPP_HAVE_TIME_MACROS
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
		return tpp_lexer_yield_handle_time_macro(self, tok);
#endif /* TPP_HAVE_TIME_MACROS */
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
#if TPP_HAVE_MACRO___TPP_IDENTIFIER
	case TPP_KWD___TPP_IDENTIFIER:
		return tpp_lexer_yield_handle___TPP_IDENTIFIER(self);
#endif /* !TPP_HAVE_MACRO___TPP_IDENTIFIER */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_EVAL
	case TPP_KWD___TPP_EVAL:
		return tpp_lexer_yield_handle___TPP_EVAL(self);
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_STR_DECOMPILE
	case TPP_KWD___TPP_STR_DECOMPILE:
		return tpp_lexer_yield_handle___TPP_STR_DECOMPILE(self);
#endif /* !TPP_HAVE_MACRO___TPP_STR_DECOMPILE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_STR_PACK
	case TPP_KWD___TPP_STR_PACK:
		return tpp_lexer_yield_handle___TPP_STR_PACK(self);
#endif /* !TPP_HAVE_MACRO___TPP_STR_PACK */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_COUNT_TOKENS
	case TPP_KWD___TPP_COUNT_TOKENS:
		return tpp_lexer_yield_handle___TPP_COUNT_TOKENS(self);
#endif /* !TPP_HAVE_MACRO___TPP_COUNT_TOKENS */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_STR_SIZE
	case TPP_KWD___TPP_STR_SIZE:
		return tpp_lexer_yield_handle___TPP_STR_SIZE(self);
#endif /* !TPP_HAVE_MACRO___TPP_STR_SIZE */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_EXEC
	case TPP_KWD___TPP_EXEC:
		return tpp_lexer_yield_handle___TPP_EXEC(self);
#endif /* !TPP_HAVE_MACRO___TPP_EXEC */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_RANDOM
	case TPP_KWD___TPP_RANDOM:
		return tpp_lexer_yield_handle___TPP_RANDOM(self);
#endif /* !TPP_HAVE_MACRO___TPP_RANDOM */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_STR_SUBSTR
	case TPP_KWD___TPP_STR_SUBSTR:
		return tpp_lexer_yield_handle___TPP_STR_SUBSTR(self, '"');
	case TPP_KWD___TPP_STR_AT:
		return tpp_lexer_yield_handle___TPP_STR_SUBSTR(self, '\'');
#endif /* !TPP_HAVE_MACRO___TPP_STR_SUBSTR */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TPP_LOAD_FILE
	case TPP_KWD___TPP_LOAD_FILE:
		return tpp_lexer_yield_handle___TPP_LOAD_FILE(self);
#endif /* !TPP_HAVE_MACRO___TPP_LOAD_FILE */
/************************************************************************/



	default: {
#if TPP_HAVE_CPP_PREDEFINED_MACROS
		/* Check for a pre-defined, builtin macro expansion */
		tpp_builtin_macro const *builtin_macro;
		if (!tpp_lexer_has(self, CPP_PREDEFINED_MACROS))
			break;

		builtin_macro = tpp_macro_getbuiltin(tok);
		if (builtin_macro != NULL) {
			return tpp_lexer_push_textfile_inherited(self, builtin_macro->tbm_body,
			                                         builtin_macro->tbm_body_size,
			                                         NULL);
		}
#endif /* TPP_HAVE_CPP_PREDEFINED_MACROS */
	}	break;

	}

	/* Fallback: act as though the macro takes no arguments, and expands to itself:
	 * >> #define SOME_MACRO SOME_MACRO */
	return tok;
}
#endif /* TPP_HAVE_CPP_BUILTIN_MACROS */
#endif /* TPP_HAVE_CPP_MACROS */

/* Handle a keyword-style macro (used to implement "tpp_lexer_yield()").
 * @return: TPP_TOK_EOF: Caller should yield again.
 * @return: * : The new expansion token after keywords were handled */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_handle_keyword(tpp_lexer *tpp_restrict self, tpp_token_id tok) {
	tpp_token const *const token = tpp_lexer_gettoken(self);
	tpp_keyword const *const keyword = tpp_token_getkwd(token);
	(void)keyword;
	tpp_assert(TPP_TOK_ISKEYWORD(tok));

	/* Emit warnings for "deprecated" keywords. */
#if TPP_HAVE_TPP_W_DEPRECATED_KEYWORD && TPP_HAVE_PRAGMA_DEPRECATED
	if (keyword->tk_misc) {
		tpp_keyword_misc const *misc = keyword->tk_misc;
		if (misc->tkm_flags & TPP_KEYWORD_FLAG_IS_DEPRECATED) {
#if TPP_HAVE_PRAGMA_GCC_POISON && TPP_HAVE_CPP_MACROS
			if ((misc->tkm_flags & TPP_KEYWORD_FLAG_IS_POISONED) &&
			    (tpp_lexer_getfilekind(self) == TPP_FILE_KIND_MACRO)) {
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

	/* Check if macros are runtime-configurable and currently disabled. */
#if TPP_CONF_IS_RT(TPP_HAVE_CPP_MACROS)
	if (!tpp_lexer_has(self, CPP_MACROS))
		return tok;
#endif /* TPP_CONF_IS_RT(TPP_HAVE_CPP_MACROS) */

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
				if (!tpp_macro_allowsselfexpansion(macro))
#endif /* TPP_HAVE_MACRO_RECURSION */
				{
					return tok;
				}
			}

			/* Expand user-defined macro... */
			return tpp_lexer_expand_macro(self, macro);
		}
	}

	/* Expand builtin macros (if enabled)
	 * NOTE: Don't need to check "TPP_FEAT_CPP_BUILTIN_MACROS" here; that feature
	 *       flag (if enabled) was already checked by "tpp_lexer_getkeyworddefined()" */
#if TPP_HAVE_CPP_BUILTIN_MACROS
	return tpp_lexer_yield_handle_builtin_macro(self, tok);
#else /* TPP_HAVE_CPP_BUILTIN_MACROS */
	return tok;
#endif /* !TPP_HAVE_CPP_BUILTIN_MACROS */
#else /* TPP_HAVE_CPP_MACROS */
	return tok;
#endif /* !TPP_HAVE_CPP_MACROS */
}

/* Wrapper around `tpp_lexer_yieldpp()' that adds handling for macro expansion.
 * @return: * :                  The newly read token (after accounting for macros)
 * @return: TPP_TOK_ENOMEM:      Out of memory
 * @return: TPP_TOK_EIO:         I/O error while trying to read from file
 * @return: TPP_TOK_EWOULDBLOCK: Current file uses "TPP_FILE_FLAGS_NONBLOCK" and operation would have blocked
 * @return: TPP_TOK_ELEXERROR:   Lexer error
 * @return: TPP_TOK_EWARNPRINT:  Error while printing a warning */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldpp(self);
	if (TPP_TOK_ISKEYWORD(result)) {
		/* Do macro expansion... */
		result = tpp_lexer_yield_handle_keyword(self, result);
		if (result == TPP_TOK_EOF)
			goto again;
	}
	return result;
}


#if TPP_HAVE_FILE_NONBLOCK

/* Same as `tpp_lexer_yield()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_FLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yield_blocking(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yield(self);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_flags & TPP_FILE_FLAGS_NONBLOCK);
		file->tf_flags &= ~TPP_FILE_FLAGS_NONBLOCK;
		tpp_file_autopopfile_pushoff(file);
		result = tpp_lexer_yield(self);
		tpp_file_autopopfile_pop(file);
		file->tf_flags |= TPP_FILE_FLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}

/* Same as `tpp_lexer_yieldpp()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_FLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
tpp_lexer_yieldpp_blocking(tpp_lexer *tpp_restrict self) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldpp(self);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_flags & TPP_FILE_FLAGS_NONBLOCK);
		file->tf_flags &= ~TPP_FILE_FLAGS_NONBLOCK;
		tpp_file_autopopfile_pushoff(file);
		result = tpp_lexer_yieldpp(self);
		tpp_file_autopopfile_pop(file);
		file->tf_flags |= TPP_FILE_FLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}

/* Same as `tpp_lexer_yieldraw_at()', but handle "TPP_TOK_EWOULDBLOCK" by temporarily
 * clearing the "TPP_FILE_FLAGS_NONBLOCK" flag, and re-attempting the yield. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_yieldraw_at_blocking(tpp_lexer *tpp_restrict self, tpp_char const **p_pos) {
	tpp_token_id result;
again:
	result = tpp_lexer_yieldraw_at(self, p_pos);
	if (result == TPP_TOK_EWOULDBLOCK) {
		tpp_file *const file = tpp_lexer_getfile(self);
		tpp_assert(file->tf_kind == TPP_FILE_KIND_IO);
		tpp_assert(file->tf_flags & TPP_FILE_FLAGS_NONBLOCK);
		file->tf_flags &= ~TPP_FILE_FLAGS_NONBLOCK;
		tpp_file_autopopfile_pushoff(file);
		result = tpp_lexer_yieldraw_at(self, p_pos);
		tpp_file_autopopfile_pop(file);
		file->tf_flags |= TPP_FILE_FLAGS_NONBLOCK;
		if (result == TPP_TOK_EOF)
			goto again; /* EOF was encountered after blocking... */
		tpp_assert(result != TPP_TOK_EWOULDBLOCK);
	}
	return result;
}

#endif /* TPP_HAVE_FILE_NONBLOCK */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELD_C */
