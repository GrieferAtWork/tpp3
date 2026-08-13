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
/*!depends config.h*/
#ifndef GUARD_TPP_LEXER_PP_PRAGMA_C
#define GUARD_TPP_LEXER_PP_PRAGMA_C 1
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

#if TPP_HAVE_PRAGMA

/************************************************************************/
/* #pragma push_macro(undef, "foo", "bar")                              */
/* #pragma pop_macro("foo", "bar")                                      */
/************************************************************************/
#if TPP_HAVE_PRAGMA_PUSH_MACRO
struct tpp_lexer_handle_pushpopmacro_data {
	tpp_lexer   *tlhppmd_lexer; /* [1..1] Lexer */
	tpp_token_id tlhppmd_mode;  /* TPP_KWD_push_macro or TPP_KWD_pop_macro */
	bool         tlhppmd_undef; /* True if macros should be #undef'd after being pushed */
};

static tpp_errno TPPCALL
tpp_lexer_handle_pushpopmacro_cb(void *arg, tpp_string *chunk,
                                 tpp_char const *str, tpp_size length) {
	tpp_errno result;
	tpp_lexer *lexer;
	struct tpp_lexer_handle_pushpopmacro_data *data;
	tpp_keyword const *ro_keyword;
	tpp_keyword *keyword;
	tpp_hash hash = tpp_hashof(str, length);
	(void)chunk;
	data  = (struct tpp_lexer_handle_pushpopmacro_data *)arg;
	lexer = data->tlhppmd_lexer;

	/* Load keyword */
	ro_keyword = tpp_keywords_newkeyword(&lexer->tl_kwds, str, length, hash);
	if tpp_unlikely(!ro_keyword)
		goto err_nomem;

	/* Make keyword writable */
	keyword = tpp_keywords_copybuiltin(&lexer->tl_kwds, ro_keyword);
	if tpp_unlikely(!keyword)
		goto err_nomem;

	/* Push/pop the macro linked to this keyword. */
	if (data->tlhppmd_mode == TPP_KWD_push_macro) {
		result = tpp_keyword_pushmacro(keyword);
		if (data->tlhppmd_undef && !TPP_ISERR(result)) {
#if TPP_HAVE_MACRO_UNDEFINED_HOOK
			result = tpp_lexer_callhook_macro_undefined(lexer, keyword);
			if (!TPP_ISERR(result))
#endif /* TPP_HAVE_MACRO_UNDEFINED_HOOK */
			{
				tpp_keyword_undef(keyword); /* Also #undef the keyword if requested */
			}
		}
	} else {
		result = tpp_keyword_popmacro(keyword);
		if (result == TPP_ENOENT) {
			/* Emit a warning */
#if TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK
			result = tpp_lexer_warnf(lexer, TPP_W_POP_MACRO_EMPTY_STACK,
			                         (unsigned int)length, str);
#else /* TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
			result = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
		} else {
			tpp_assert(!TPP_ISERR(result));
#if TPP_HAVE_MACRO_DEFINED_HOOK
			if (_TPP_KEYWORD_MACRO_ISDEFINED(keyword->tk_macro))
				result = tpp_lexer_callhook_macro_defined(lexer, keyword, keyword->tk_macro);
#endif /* TPP_HAVE_MACRO_DEFINED_HOOK */
		}
	}
	return result;
err_nomem:
	return TPP_ENOMEM;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_pushpop_macro(tpp_lexer *tpp_restrict self, tpp_token_id mode) {
	tpp_errno error;
	struct tpp_lexer_handle_pushpopmacro_data data;
	tpp_token_id tok;
	data.tlhppmd_lexer = self;
	data.tlhppmd_mode  = mode;
	data.tlhppmd_undef = false;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tok == TPP_KWD_undef) {
		data.tlhppmd_undef = true;
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(','));
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
			tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}

again_parse_string:
	if (TPP_TOK_ISSTRING(tok)) {
		/* Parse+process string (using "tpp_lexer_parsestring_cb()") */
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_handle_pushpopmacro_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* TPP allows multiple strings to be specified here */
	if (tok == ',') {
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		goto again_parse_string;
	}

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */





/************************************************************************/
/* #pragma once                                                         */
/************************************************************************/
#if TPP_HAVE_PRAGMA_ONCE
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_once(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
	tpp_file const *textfile;
	tpp_keyword *textfile_kwd;

	/* Set the "TPP_KEYWORD_FLAG_HDR_ONCE" flag for the current text file */
	textfile     = tpp_file_gettextfile(tpp_lexer_getfile(self));
	textfile_kwd = tpp_file_getrealfilenamekwd(textfile);
	if (textfile_kwd) {
		tpp_keyword_flags flags = tpp_keyword_getflags(textfile_kwd);
		tpp_errno error = tpp_keyword_setflags(textfile_kwd, flags | TPP_KEYWORD_FLAG_HDR_ONCE);
		if (TPP_ISERR(error))
			return error;
	}

	/* Emit -Wpragma-once-outside-header if appropriate */
#if TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER
	if (tpp_file_isbasefile(textfile)) {
		tpp_errno error = tpp_lexer_warnf(self, TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_PRAGMA_ONCE_OUTSIDE_HEADER */

	tok = tpp_lexer_yieldraw_blocking(self);
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_ONCE */





/************************************************************************/
/* #pragma deprecated("foo")                                            */
/************************************************************************/
#if TPP_HAVE_PRAGMA_DEPRECATED
static tpp_errno TPPCALL
tpp_lexer_process_pragma_deprecated_cb(void *arg, tpp_string *chunk,
                                       tpp_char const *str, tpp_size length) {
	tpp_lexer *const lexer = (tpp_lexer *)arg;
	tpp_keyword const *ro_keyword;
	tpp_keyword *keyword;
	tpp_keyword_flags flags;
	tpp_hash hash = tpp_hashof(str, length);
	(void)chunk;
	ro_keyword = tpp_lexer_kwds_newkeyword(lexer, str, length, hash);
	if tpp_unlikely(!ro_keyword)
		return TPP_ENOMEM;
	keyword = tpp_lexer_kwds_copybuiltin(lexer, ro_keyword);
	if tpp_unlikely(!keyword)
		return TPP_ENOMEM;
	flags = tpp_keyword_getflags(keyword);
	return tpp_keyword_setflags(keyword, flags | TPP_KEYWORD_FLAG_IS_DEPRECATED);
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_deprecated(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
again_parse_string:
	if (TPP_TOK_ISSTRING(tok)) {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_deprecated_cb,
		                                 self, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* TPP allows multiple strings to be specified here */
	if (tok == ',') {
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		goto again_parse_string;
	}

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_DEPRECATED */





/************************************************************************/
/* #pragma extension(push, "-fmacro-recursion")                         */
/* #pragma extension("-fno-macro-recursion")                            */
/* #pragma extension(pop)                                               */
/************************************************************************/
#if TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION
static tpp_errno TPPCALL
tpp_lexer_process_pragma_extension_cb(void *arg, tpp_string *chunk,
                                      tpp_char const *str, tpp_size length) {
	tpp_errno error;
	tpp_lexer *lexer = (tpp_lexer *)arg;
	tpp_extension_id id;
	bool enable_extension = true;
	(void)chunk;
	if (length >= 2 && str[0] == '-' && str[1] == 'f')
		length -= 2, str += 2;
	if (length >= 3 && str[0] == 'n' && str[1] == 'o' && str[2] == '-')
		length -= 3, str += 3, enable_extension = false;
	id = tpp_extension_byname_ex((char const *)str, length);
	if ((unsigned int)id >= (unsigned int)TPP_EXT_COUNT) {
#if TPP_HAVE_TPP_W_UNKNOWN_EXTENSION
#if TPP_HAVE_TPP_EXTENSION_NEAREST
		tpp_extension_id nearest = tpp_extension_nearest_ex((char const *)str, length);
		char const *nearest_name = tpp_extension_getname(nearest);
		error = tpp_lexer_warnf(lexer, TPP_W_UNKNOWN_EXTENSION,
		                        (unsigned int)length, str, nearest_name);
#else /* TPP_HAVE_TPP_EXTENSION_NEAREST */
		error = tpp_lexer_warnf(lexer, TPP_W_UNKNOWN_EXTENSION,
		                        (unsigned int)length, str);
#endif /* !TPP_HAVE_TPP_EXTENSION_NEAREST */
#endif /* TPP_HAVE_TPP_W_UNKNOWN_EXTENSION */
	} else {
		error = tpp_lexer_setextension(lexer, id, enable_extension);
	}
	return error;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_extension(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
again_parse_string:
	switch (tok) {

#if TPP_HAVE_EXTENSIONS_PUSH_POP
	case TPP_KWD_push:
		tpp_lexer_pushextensions(self);
		tok   = tpp_lexer_yield_blocking(self);
		error = TPP_TOK_ASERR_OR_EOK(tok);
		break;

	case TPP_KWD_pop:
		if (tpp_lexer_canpopextensions(self)) {
			tpp_lexer_popextensions(self);
		} else {
#if TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS
			error = tpp_lexer_warnf(self, TPP_W_CANNOT_POP_EXTENSIONS);
#endif /* TPP_HAVE_TPP_W_CANNOT_POP_EXTENSIONS */
			if (TPP_ISERR(error))
				break;
		}
		tok   = tpp_lexer_yield_blocking(self);
		error = TPP_TOK_ASERR_OR_EOK(tok);
		break;
#endif /* TPP_HAVE_EXTENSIONS_PUSH_POP */

	TPP_CASE_TPP_TOK_STRING
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_extension_cb,
		                                 self, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
		break;

	default:
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
		break;
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* TPP allows multiple strings to be specified here */
	if (tok == ',') {
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		goto again_parse_string;
	}

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_EXTENSION || TPP_HAVE_PRAGMA_TPP_EXTENSION */





/************************************************************************/
/* #pragma warning(push, disable: 4068, "[-W]unknown-pragmas")          */
/* #pragma warning(enable: 4068, "[-W]unknown-pragmas")                 */
/* #pragma warning(suppress: 4068, "[-W]unknown-pragmas")               */
/* #pragma warning(error: 4068, "[-W]unknown-pragmas")                  */
/* #pragma warning(default: 4068, "[-W]unknown-pragmas")                */
/* #pragma warning(-1: 4068)                       // #pragma warning(error: 4068)                   */
/* #pragma warning(0: 4068)                        // #pragma warning(enable: 4068)                  */
/* #pragma warning(1: 4068)                        // #pragma warning(disable: 4068)                 */
/* #pragma warning(2: 4068)                        // #pragma warning(suppress: 4068)                */
/* #pragma warning("[-W]unknown-pragmas")          // #pragma warning(enable: "-Wunknown-pragmas")   */
/* #pragma warning("[-W]no-unknown-pragmas")       // #pragma warning(disable: "-Wunknown-pragmas")  */
/* #pragma warning("[-W]def-unknown-pragmas")      // #pragma warning(default: "-Wunknown-pragmas")  */
/* #pragma warning("[-W]sup-unknown-pragmas")      // #pragma warning(suppress: "-Wunknown-pragmas") */
/* #pragma warning("[-W]suppress-unknown-pragmas") // #pragma warning(suppress: "-Wunknown-pragmas") */
/* #pragma warning("[-W]error=unknown-pragmas")    // #pragma warning(error: "-Wunknown-pragmas")    */
/* #pragma warning(pop)                                                 */
/************************************************************************/
#if TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC
struct tpp_lexer_pragma_warning_state_data {
	tpp_lexer        *tlpwsd_lexer; /* [1..1] Lexer */
	tpp_warning_state tlpwsd_state; /* State to assign to warning */
};

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_pragma_warning_setstate_impl(tpp_lexer *tpp_restrict self,
                                       tpp_char const *str, tpp_size length,
                                       tpp_warning_state state) {
	tpp_warning_group_id gid;
	tpp_warning_context_id ctx_id;
	gid = tpp_warning_group_byname_ex((char const *)str, length);
	if ((unsigned int)gid >= (unsigned int)TPP_WG_COUNT) {
#if TPP_HAVE_TPP_W_UNKNOWN_WARNING
#if TPP_HAVE_TPP_WARNING_GROUP_NEAREST
		tpp_warning_group_id nearest = tpp_warning_group_nearest_ex((char const *)str, length);
		char const *nearest_name = tpp_warning_group_getnames(nearest);
		return tpp_lexer_warnf(self, TPP_W_UNKNOWN_WARNING,
		                       (unsigned int)length, str, nearest_name);
#else /* TPP_HAVE_TPP_WARNING_GROUP_NEAREST */
		return tpp_lexer_warnf(self, TPP_W_UNKNOWN_WARNING,
		                       (unsigned int)length, str, nearest_name);
#endif /* !TPP_HAVE_TPP_WARNING_GROUP_NEAREST */
#else /* TPP_HAVE_TPP_W_UNKNOWN_WARNING */
		return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_WARNING */
	}
	ctx_id = tpp_warning_context_id_ofgroup(gid);
	return tpp_lexer_setwarningctx(self, ctx_id, state);
}

static tpp_errno TPPCALL
tpp_lexer_pragma_warning_state_cb(void *arg, tpp_string *chunk,
                                  tpp_char const *str, tpp_size length) {
	struct tpp_lexer_pragma_warning_state_data *data;
	(void)chunk;
	data = (struct tpp_lexer_pragma_warning_state_data *)arg;
	if (length >= 2 && str[0] == '-' && str[1] == 'W')
		str += 2, length -= 2;
	return tpp_lexer_pragma_warning_setstate_impl(data->tlpwsd_lexer, str, length,
	                                              data->tlpwsd_state);
}
#endif /* TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING || TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */

#if TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING
static tpp_errno TPPCALL
tpp_lexer_pragma_warning_raw_cb(void *arg, tpp_string *chunk,
                                tpp_char const *str, tpp_size length) {
	tpp_lexer *const self = (tpp_lexer *)arg;
	tpp_warning_state state = TPP_WSTATE_WARN;
	(void)chunk;
	if (length >= 2 && str[0] == '-' && str[1] == 'W')
		str += 2, length -= 2;
	if (length >= 3 && str[0] == 'n' && str[1] == 'o' && str[2] == '-') {
		state = TPP_WSTATE_DISABLED;
		length -= 3;
		str += 3;
	} else if (length >= 6 &&
	           str[0] == 'e' && str[1] == 'r' && str[2] == 'r' &&
	           str[3] == 'o' && str[4] == 'r' && str[5] == '=') {
		state = TPP_WSTATE_ERROR_OR_FATAL;
		length -= 6;
		str += 6;
	} else
#if TPP_HAVE_WARNING_DEFAULT
	if (length >= 4 && str[0] == 'd' && str[1] == 'e' && str[2] == 'f' && str[3] == '-') {
		state = TPP_WSTATE_DEFAULT;
		length -= 4;
		str += 4;
	} else
#endif /* TPP_HAVE_WARNING_DEFAULT */
#if TPP_HAVE_WARNING_SUPPRESS
	if (length >= 4 && str[0] == 's' && str[1] == 'u' && str[2] == 'p' && str[3] == '-') {
		state = TPP_WSTATE_SUPPRESS;
		length -= 4;
		str += 4;
	} else if (length >= 9 &&
	           str[0] == 's' && str[1] == 'u' &&
	           str[2] == 'p' && str[3] == 'p' &&
	           str[4] == 'r' && str[5] == 'e' &&
	           str[6] == 's' && str[7] == 's' &&
	           str[8] == '-') {
		state = TPP_WSTATE_SUPPRESS;
		length -= 9;
		str += 9;
	} else
#endif /* TPP_HAVE_WARNING_SUPPRESS */
	{
	}
	return tpp_lexer_pragma_warning_setstate_impl(self, str, length, state);
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_warning(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	tpp_warning_state new_state;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
again_handle:
	switch (tok) {
	case TPP_TOK_SPACE:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT
again_yield_and_handle:
		tok = tpp_lexer_yield_blocking(self);
		goto again_handle;

#if TPP_HAVE_WARNINGS_PUSH_POP
	case TPP_KWD_push:
		tpp_lexer_pushwarnings(self);
		tok = tpp_lexer_yield_blocking(self);
		break;
	case TPP_KWD_pop:
		if (tpp_lexer_canpopwarnings(self)) {
			tpp_lexer_popwarnings(self);
		} else {
#if TPP_HAVE_TPP_W_CANNOT_POP_WARNINGS
			error = tpp_lexer_warnf(self, TPP_W_CANNOT_POP_WARNINGS);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				break;
			}
#endif /* TPP_HAVE_TPP_W_CANNOT_POP_WARNINGS */
		}
		tok = tpp_lexer_yield_blocking(self);
		break;
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */

#if TPP_HAVE_TOK_INT
	{
		tpp_intmax mode;
		bool negative;
	TPP_CASE_TPP_TOK_INT
		negative = false;
		if (0) {
	case '-':
			negative = true;
			do {
				tok = tpp_lexer_yield_blocking(self);
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (!TPP_TOK_ISINT(tok)) {
#if TPP_HAVE_TPP_W_EXPECTED_INT
				error = tpp_lexer_warnf(self, TPP_W_EXPECTED_INT);
				if (TPP_ISERR(error))
					return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_INT */
				break;
			}
		}
		error = tpp_lexer_decodeint(self, &mode);
		if (TPP_ISERR(error)) {
			tok = TPP_TOK_OFERR(error);
			break;
		}
		if (negative)
			mode = -mode;
		/* Mirror behavior of TPP2 */
		if (mode < 0) {
			new_state = TPP_WSTATE_FATAL;
		} else if (mode == 0) {
			new_state = TPP_WSTATE_WARN;
		} else if (mode == 1) {
			new_state = TPP_WSTATE_DISABLED;
		} else {
			new_state = TPP_WSTATE_SUPPRESS;
		}
		goto set_warning_state;
	}	break;
#endif /* TPP_HAVE_TOK_INT */
	case TPP_KWD_disable:
		new_state = TPP_WSTATE_DISABLED;
		goto set_warning_state;
	case TPP_KWD_enable:
		new_state = TPP_WSTATE_WARN;
		goto set_warning_state;
	case TPP_KWD_error:
		new_state = TPP_WSTATE_ERROR_OR_FATAL;
		goto set_warning_state;
#if TPP_HAVE_WARNING_SUPPRESS
	case TPP_KWD_suppress:
		new_state = TPP_WSTATE_SUPPRESS;
		goto set_warning_state;
#endif /* TPP_HAVE_WARNING_SUPPRESS */
#if TPP_HAVE_WARNING_DEFAULT
	case TPP_KWD_default:
		new_state = TPP_WSTATE_DEFAULT;
		goto set_warning_state;
#endif /* TPP_HAVE_WARNING_DEFAULT */
	case TPP_KWD_fatal: {
		new_state = TPP_WSTATE_FATAL;
set_warning_state:
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(':'));
again_handle_set_warning_state:
		switch (tok) {
		case TPP_TOK_SPACE:
		case TPP_TOK_LF:
		TPP_CASE_TPP_TOK_COMMENT
			tok = tpp_lexer_yield_blocking(self);
			goto again_handle_set_warning_state;

#if TPP_HAVE_TOK_INT && TPP_HAVE_WARNING_NUMBERS
		TPP_CASE_TPP_TOK_INT {
			tpp_intmax warning_number;
			tpp_warning_id warning_id;
			error = tpp_lexer_decodeint(self, &warning_number);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				break;
			}
			warning_id = TPP_W_COUNT;
#ifdef UINT_MAX
			if (warning_number >= 0 && warning_number <= (tpp_intmax)(tpp_uintmax)UINT_MAX)
#else /* UINT_MAX */
			if (warning_number >= 0 && warning_number <= (tpp_intmax)(tpp_uintmax)(unsigned int)-1)
#endif /* !UINT_MAX */
			{
				warning_id = tpp_warning_ofnumber((unsigned int)warning_number);
			}
			if ((unsigned int)warning_id >= (unsigned int)TPP_W_COUNT) {
#if TPP_HAVE_TPP_W_UNKNOWN_WARNING_NUMBER
				error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_WARNING_NUMBER, (int)warning_number);
#else /* TPP_HAVE_TPP_W_UNKNOWN_WARNING_NUMBER */
				error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_WARNING_NUMBER */
			} else {
				tpp_warning_context_id ctx_id;
				ctx_id = tpp_warning_context_id_ofwarning(warning_id);
				error = tpp_lexer_setwarningctx(self, ctx_id, new_state);
			}
			if (TPP_ISERR(error))
				return error;
			tok = tpp_lexer_yield_blocking(self);
		}	break;
#endif /* TPP_HAVE_TOK_INT && TPP_HAVE_WARNING_NUMBERS */

		TPP_CASE_TPP_TOK_STRING {
			struct tpp_lexer_pragma_warning_state_data data;
			data.tlpwsd_lexer = self;
			data.tlpwsd_state = new_state;
			error = tpp_lexer_parsestring_cb(self, &tpp_lexer_pragma_warning_state_cb, &data,
			                                 TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
			if (TPP_ISERR(error))
				return error;
			tok = tpp_lexer_gettok(self);
		}	break;

		default:
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING_AFTER_COLON
			if (!TPP_TOK_ISERR(tok)) {
				error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING_AFTER_COLON);
				if (TPP_ISERR(error))
					return error;
			}
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING_AFTER_COLON */
			break;
		}
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
			tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
#if TPP_HAVE_TOK_INT && TPP_HAVE_WARNING_NUMBERS
		if (TPP_TOK_ISINT(tok))
			goto again_handle_set_warning_state;
#endif /* TPP_HAVE_TOK_INT && TPP_HAVE_WARNING_NUMBERS */
		if (TPP_TOK_ISSTRING(tok))
			goto again_handle_set_warning_state;
	}	break;

	TPP_CASE_TPP_TOK_STRING
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_pragma_warning_raw_cb, self,
		                                 TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
		tok = TPP_TOK_OFERR_OR_EOF(error);
		break;

	default:
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING
		error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING);
#else /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING */
		tok = TPP_TOK_OFERR_OR_EOF(error);
		break;
	}
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tok == ',')
		goto again_yield_and_handle;

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING */





/************************************************************************/
/* #pragma message("...")                                               */
/* #pragma message "..."                                                */
/************************************************************************/
#if TPP_HAVE_PRAGMA_MESSAGE
#ifndef tpp_lexer_gethook_mesgprinter
#define tpp_lexer_gethook_mesgprinter(self)       (&tpp_dummy_printer)
#define tpp_lexer_gethookcookie_mesgprinter(self) (self)
#ifndef tpp_dummy_printer
#define tpp_dummy_printer tpp_dummy_printer
static TPP_FORMATPRINTER_DEFINE(tpp_dummy_printer, arg, text, num_bytes) {
	(void)arg;
	(void)text;
	(void)num_bytes;
	return 0;
}
#endif /* !tpp_dummy_printer */
#endif /* !tpp_lexer_gethook_mesgprinter */



static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_message(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
	bool has_lparen;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	has_lparen = tok == '(';
	if (has_lparen) {
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}

	if (TPP_TOK_ISSTRING(tok)) {
		tpp_ssize status;
		tpp_lexer_decodestring_config config;
		tpp_lexer_decodestring_config_init_simple(&config,
		                                          tpp_lexer_gethook_mesgprinter(self),
		                                          tpp_lexer_gethookcookie_mesgprinter(self));
#if TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION
		if (tpp_lexer_has(self, PRAGMA_MESSAGE_PRINTS_LOCATION)) {
			tpp_lexer_printf_info info;
			tpp_file *const lcfile = tpp_file_getlcfile(tpp_lexer_getfile(self));
			tpp_lexer_printf_info_init_at(&info, lcfile, tpp_file_getlastpos(lcfile));
			status = tpp_lexer_printf_warning(self, &info, config.tldsc_dataprinter, self,
			                                  tpp_lexer_getfileandlineformat(self));
			if (TPP_SSIZE_ISERR(status))
				return TPP_SSIZE_ASERR(status);
		}
#endif /* TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION */
		status = tpp_lexer_parsestring_ex(self, &config, TPP_LEXER_PARSESTRING_FLAG_NORMAL);
#if TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED
		if (tpp_lexer_has(self, PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED)) {
			if (status >= 0)
				status = tpp_formatprinter_print_conststr(config.tldsc_dataprinter, self, "\n");
		}
#endif /* TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED */
		if (TPP_SSIZE_ISERR(status))
			return TPP_SSIZE_ASERR(status);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		tpp_errno error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_STRING */
	}

	if (has_lparen) {
		tok = tpp_lexer_gettok(self);
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
			tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}
	return TPP_EOK;
}
#endif /* TPP_HAVE_PRAGMA_MESSAGE */





/************************************************************************/
/* #pragma error("...")                                                 */
/************************************************************************/
#if TPP_HAVE_PRAGMA_ERROR
static tpp_errno TPPCALL
tpp_lexer_process_pragma_error_cb(void *arg, tpp_string *chunk,
                                  tpp_char const *str, tpp_size length) {
	tpp_lexer *const self = (tpp_lexer *)arg;
	(void)chunk;
#if TPP_HAVE_TPP_W_ERROR
	return tpp_lexer_warnf(self, TPP_W_ERROR, (unsigned int)length, str);
#else /* TPP_HAVE_TPP_W_ERROR */
	(void)self;
	(void)str;
	(void)length;
	return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_ERROR */
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_error(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (TPP_TOK_ISSTRING(tok)) {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_error_cb, self,
		                                 TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else  /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_ERROR */





/************************************************************************/
/* #pragma GCC system_header                                            */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_system_header(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
#if TPP_HAVE_FILE_SYSHDR
	tpp_file *textfile = tpp_file_gettextfile(tpp_lexer_getfile(self));
	if (textfile != NULL)
		textfile->tf_flags |= TPP_FILE_FLAGS_SYSHDR;
#endif /* TPP_HAVE_FILE_SYSHDR */
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */





/************************************************************************/
/* #pragma GCC diagnostic push                                          */
/* #pragma GCC diagnostic warning "-Wunknown-pragmas"                   */
/* #pragma GCC diagnostic error "-Wunknown-pragmas"                     */
/* #pragma GCC diagnostic ignored "-Wunknown-pragmas"                   */
/* #pragma GCC diagnostic pop                                           */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_diagnostic(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	switch (tok) {

#if TPP_HAVE_WARNINGS_PUSH_POP
	case TPP_KWD_push:
		tpp_lexer_pushwarnings(self);
		tok = tpp_lexer_yield_blocking(self);
		break;
	case TPP_KWD_pop:
		if (tpp_lexer_canpopwarnings(self)) {
			tpp_lexer_popwarnings(self);
		} else {
#if TPP_HAVE_TPP_W_CANNOT_POP_WARNINGS
			error = tpp_lexer_warnf(self, TPP_W_CANNOT_POP_WARNINGS);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				break;
			}
#endif /* TPP_HAVE_TPP_W_CANNOT_POP_WARNINGS */
		}
		tok = tpp_lexer_yield_blocking(self);
		break;
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */

	case TPP_KWD_warning:
	case TPP_KWD_error:
	case TPP_KWD_ignored: {
		struct tpp_lexer_pragma_warning_state_data data;
		data.tlpwsd_lexer = self;
		switch (tok) {
		case TPP_KWD_warning:
			data.tlpwsd_state = TPP_WSTATE_WARN;
			break;
		case TPP_KWD_error:
			data.tlpwsd_state = TPP_WSTATE_ERROR_OR_FATAL;
			break;
		case TPP_KWD_ignored:
			data.tlpwsd_state = TPP_WSTATE_DISABLED;
			break;
		default: tpp_unreachable();
		}
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (TPP_TOK_ISSTRING(tok)) {
			error = tpp_lexer_parsestring_cb(self, &tpp_lexer_pragma_warning_state_cb, &data,
			                                 TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
		} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else  /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
		}
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_gettok(self);
	}	break;

	default:
#if TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_GCC_DIAGNOSTIC
		return tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_GCC_DIAGNOSTIC);
#else /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_GCC_DIAGNOSTIC */
		return TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_GCC_DIAGNOSTIC */
	}
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */





/************************************************************************/
/* #pragma GCC poison foo bar foobar ...                                */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_POISON
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_poison(tpp_lexer *tpp_restrict self) {
	/* TODO: Do this one right in TPP3:
	 * >> #define my_strcpy strcpy
	 * >> #define my_wrapper(x) x
	 * >> #pragma GCC poison strcpy
	 * >>
	 * >> ...
	 * >>
	 * >> strcpy(buf, "foo");              // Warning here...
	 * >> my_strcpy(buf, "foo");           // ... but not here
	 * >> my_wrapper(strcpy(buf, "foo"));  // Warning here, too (complicated)
	 *
	 * iow: only warn if the poisoned token appears *after* "#pragma GCC poison".
	 *      This will require replacing the "TPP_KEYWORD_FLAG_IS_POISONED" flag
	 *      with a version-counter which is allocated whenever a keyword is poisoned.
	 *      Additionally, macros will need to store the most-recent poison version,
	 *      and whenever a poisoned keyword is encountered, tpp_lexer_yield() must
	 *      check where the keyword originates from:
	 *      - If it's not from a macro: emit warning
	 *      - If it's from a macro:
	 *        - If the macro was created after the keyword was poisoned: emit warning
	 *        - If it's a keyword-macro: don't emit warning
	 *        - If it's a function-macro:
	 *          - If the keyword originates from the macro's source body: don't emit warning
	 *          - If the keyword originates from macro arguments:
	 *            - Restart check based on location of macro argument call site
	 *              Re-determining that location at this point will also be complicated,
	 *              but the same functionality is also needed in "tpp_macro_func_lcinfo()"
	 *              - If the macro argument call site cannot be determined (which can be
	 *                the case when the argument call site is no longer on the `#include`-
	 *                stack): emit warning
	 *          - If the keyword appeared because of something else (e.g. token concat): emit warning
	 */

	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_GCC_POISON */





/************************************************************************/
/* #pragma GCC warning "message"                                        */
/* #pragma GCC error "message"                                          */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_WARNING || TPP_HAVE_PRAGMA_GCC_ERROR
struct tpp_lexer_process_pragma_gcc_error_data {
	tpp_lexer     *tlppged_lexer; /* [1..1] Lexer */
	tpp_warning_id tlppged_wid;   /* Warning ID to emit */
};
static tpp_errno TPPCALL
tpp_lexer_process_pragma_gcc_error_cb(void *arg, tpp_string *chunk,
                                      tpp_char const *str, tpp_size length) {
	struct tpp_lexer_process_pragma_gcc_error_data *data;
	data = (struct tpp_lexer_process_pragma_gcc_error_data *)arg;
	(void)chunk;
	return tpp_lexer_warnf(data->tlppged_lexer, data->tlppged_wid, (unsigned int)length, str);
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_warning(tpp_lexer *tpp_restrict self, tpp_warning_id mode) {
	tpp_errno error;
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (TPP_TOK_ISSTRING(tok)) {
		struct tpp_lexer_process_pragma_gcc_error_data data;
		data.tlppged_lexer = self;
		data.tlppged_wid   = mode;
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_gcc_error_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_GCC_WARNING || TPP_HAVE_PRAGMA_GCC_ERROR */





/************************************************************************/
/* #pragma GCC dependency <file> [<message>]                            */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_DEPENDENCY
struct tpp_lexer_process_pragma_GCC_dependency_data {
	tpp_lexer      *tlppgdd_lexer;    /* [1..1] Current lexer */
	tpp_file const *tlppgdd_textfile; /* [1..1] The current tpp_file_gettextfile()-file */
	int             tlppgdd_cmpres;   /* Result of last-modified comparison (lhs: current file, rhs: dependency)
	                                   * Set to "0" if the comparison failed for some non-fatal reason. */
	char const     *tlppgdd_str;      /* `#include`-string (used during callback) */
	tpp_size        tlppgdd_length;   /* `#include`-string length (used during callback) */
	char           *tlppgdd_depfile;  /* [1..1][OUT][owned][valid_if(tlppgdd_cmpres < 0)] Filename of changed dependency. */
};

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_dependency_path_cb(void *arg, char const *relative_to
                                                tpp_lexer_foreach_include_path_flags__PARAM) {
	tpp_errno cmp_error;
	char *rhs_filename;
	struct tpp_lexer_process_pragma_GCC_dependency_data *data;
	tpp_file const *textfile;
	data = (struct tpp_lexer_process_pragma_GCC_dependency_data *)arg;
	textfile = data->tlppgdd_textfile;
	tpp_assert(textfile);
	tpp_assert(textfile->tf_kind == TPP_FILE_KIND_IO ||
	           textfile->tf_kind == TPP_FILE_KIND_TEXT);
#if TPP_HAVE_FILE_SYSHDR
	(void)flags;
#endif /* TPP_HAVE_FILE_SYSHDR */
	rhs_filename = tpp_joinpath(relative_to, data->tlppgdd_str, data->tlppgdd_length);
	if tpp_unlikely(!rhs_filename)
		return TPP_ENOMEM;
	cmp_error = tpp_io_compare_mtime(textfile->tf_data.td_io.tff_name,
	                                 textfile->tf_data.td_io.tff_file,
	                                 textfile->tf_kind == TPP_FILE_KIND_IO,
	                                 rhs_filename, &data->tlppgdd_cmpres);
	if (cmp_error == TPP_ELAST) { /* Unable to compare */
		data->tlppgdd_cmpres = 0;
		cmp_error = TPP_EOK;
	}
	if (TPP_ISERR(cmp_error) || data->tlppgdd_cmpres >= 0) {
		tpp_free(rhs_filename);
	} else {
		data->tlppgdd_depfile = rhs_filename;
	}
	return cmp_error;
}

static tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_dependency_cb(void *arg, char const *str, tpp_size length) {
	tpp_token_id mode;
	struct tpp_lexer_process_pragma_GCC_dependency_data *data;
	data = (struct tpp_lexer_process_pragma_GCC_dependency_data *)arg;
	data->tlppgdd_str    = str;
	data->tlppgdd_length = length;
	/* Check for special case: if the given filename is absolute,
	 * then we must skip all the include-path resolution! */
	if (TPP_FS_ISABS(str, length))
		return tpp_lexer_process_pragma_GCC_dependency_path_cb(data, NULL tpp_lexer_foreach_include_path_flags__ARG(TPP_FILE_FLAGS_NORMAL));
	mode = tpp_lexer_gettok(data->tlppgdd_lexer);
	tpp_assert(mode == '<' || mode == '"');
	return tpp_lexer_foreach_include_path(data->tlppgdd_lexer, mode,
	                                      &tpp_lexer_process_pragma_GCC_dependency_path_cb,
	                                      data);
}

#if TPP_HAVE_TPP_W_DEPENDENCY_CHANGED
static tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_dependency_changed_cb(void *arg, tpp_string *chunk,
                                                   tpp_char const *str, tpp_size length) {
	struct tpp_lexer_process_pragma_GCC_dependency_data *data;
	data = (struct tpp_lexer_process_pragma_GCC_dependency_data *)arg;
	(void)chunk;
	/* XXX: It's be nicer if the warning position pointed *at* the include-string */
	return tpp_lexer_warnf(data->tlppgdd_lexer, TPP_W_DEPENDENCY_CHANGED,
	                       data->tlppgdd_depfile, ": ",
	                       (unsigned int)length, str);
}
#endif /* TPP_HAVE_TPP_W_DEPENDENCY_CHANGED */

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_dependency(tpp_lexer *tpp_restrict self) {
	struct tpp_lexer_process_pragma_GCC_dependency_data data;
	tpp_token_id tok;
	tpp_errno error;
	do {
		tok = tpp_lexer_yield_include_string_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	data.tlppgdd_lexer   = self;
	data.tlppgdd_cmpres  = 0;
	data.tlppgdd_depfile = NULL;
	if (tok == '"' || tok == '<') {
		error = TPP_EOK;
		data.tlppgdd_textfile = tpp_file_gettextfile(tpp_lexer_getfile(self));
		if (data.tlppgdd_textfile) {
			error = tpp_lexer_decode_include_string_cb(self, &tpp_lexer_process_pragma_GCC_dependency_cb, &data);
			if (error == TPP_ENOENT) {
#if TPP_HAVE_TPP_W_NO_SUCH_FILE
				error = tpp_lexer_warnf(self, TPP_W_NO_SUCH_FILE);
#else /* TPP_HAVE_TPP_W_NO_SUCH_FILE */
				error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_NO_SUCH_FILE */
			}
		}
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_INCLUDE_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_INCLUDE_STRING */
	}
	tpp_assert(error != TPP_ENOENT);
	tpp_assert(error != TPP_ELAST);
	if (TPP_ISERR(error))
		return error;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok)) {
err_tok_data_depfile:
		if (data.tlppgdd_cmpres < 0)
			tpp_free(data.tlppgdd_depfile);
		return TPP_TOK_ASERR(tok);
	}
	if (TPP_TOK_ISSTRING(tok)) {
#if TPP_HAVE_TPP_W_DEPENDENCY_CHANGED
		if (data.tlppgdd_cmpres < 0) {
			error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_GCC_dependency_changed_cb,
			                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
		} else
#endif /* TPP_HAVE_TPP_W_DEPENDENCY_CHANGED */
		{
			do {
				tok = tpp_lexer_yield_blocking(self);
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok) ||
			         TPP_TOK_ISSTRING(tok));
			if (TPP_TOK_ISERR(tok))
				goto err_tok_data_depfile;
		}
	} else
#if TPP_HAVE_TPP_W_DEPENDENCY_CHANGED
	if (data.tlppgdd_cmpres < 0) {
		/* XXX: It's be nicer if the warning position pointed *at* the include-string */
		error = tpp_lexer_warnf(self, TPP_W_DEPENDENCY_CHANGED, data.tlppgdd_depfile, "", 0, "");
	} else
#endif /* TPP_HAVE_TPP_W_DEPENDENCY_CHANGED */
	{
		error = TPP_EOK;
	}
	if (data.tlppgdd_cmpres < 0)
		tpp_free(data.tlppgdd_depfile);
	return error;
}
#endif /* TPP_HAVE_PRAGMA_GCC_DEPENDENCY */





/************************************************************************/
/* #pragma tpp_exec("...")                                              */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC
static tpp_errno TPPCALL
tpp_lexer_pragma_tpp_exec_cb(void *arg, tpp_string *chunk,
                             tpp_char const *str, tpp_size length) {
	tpp_token_id tok;
	tpp_lexer *const self = (tpp_lexer *)arg;
	tpp_file *const file = tpp_lexer_getfile(self);
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

	/* Parse contents of string, but discard all tokens. */
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (!TPP_TOK_ISERR_OR_EOF(tok));

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

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_tpp_exec(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Parse string to execute */
	if (TPP_TOK_ISSTRING(tok)) {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_pragma_tpp_exec_cb,
		                                 self, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC */





/************************************************************************/
/* #pragma tpp_set_keyword_flags("foo", 0x7f)                           */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
#define TPP_LEGACY_KEYWORD_FLAG_HAS_ATTRIBUTE          0x01 /* Historical constant: `__has_attribute()` */
#define TPP_LEGACY_KEYWORD_FLAG_HAS_BUILTIN            0x02 /* Historical constant: `__has_builtin()` */
#define TPP_LEGACY_KEYWORD_FLAG_HAS_CPP_ATTRIBUTE      0x04 /* Historical constant: `__has_cpp_attribute()` */
#define TPP_LEGACY_KEYWORD_FLAG_HAS_DECLSPEC_ATTRIBUTE 0x08 /* Historical constant: `__has_declspec_attribute()` */
#define TPP_LEGACY_KEYWORD_FLAG_HAS_EXTENSION          0x10 /* Historical constant: `__has_extension()` */
#define TPP_LEGACY_KEYWORD_FLAG_HAS_FEATURE            0x20 /* Historical constant: `__has_feature()` */
#define TPP_LEGACY_KEYWORD_FLAG_IS_DEPRECATED          0x40 /* Historical constant: Warn when the keyword appears as the result of lexical processing. */
#define TPP_LEGACY_KEYWORD_FLAG_MASK                   0x7f /* Historical constant */

#if TPP_HAVE_KEYWORD_FEATURES
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_set_legacy_feature_expansion(tpp_keyword *tpp_restrict keyword,
                                         tpp_keyword_feature_kind kind,
                                         bool enabled) {
	static TPP_STRING_DEFINE(str_0, "0");
	static TPP_STRING_DEFINE(str_1, "1");
	tpp_string *value = enabled ? (tpp_string *)&str_1 : (tpp_string *)&str_0;
	return tpp_keyword_setfeature(keyword, kind, value);
}
#endif /* TPP_HAVE_KEYWORD_FEATURES */

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_set_legacy_flags(tpp_keyword *tpp_restrict keyword, uint_least8_t flags) {
	/* Modify keyword flags */
	tpp_errno error = TPP_EOK;
	tpp_keyword_flags old_kwd_flags = tpp_keyword_getflags(keyword);
	tpp_keyword_flags new_kwd_flags = old_kwd_flags & ~TPP_KEYWORD_FLAG_IS_DEPRECATED;
	if (flags & TPP_LEGACY_KEYWORD_FLAG_IS_DEPRECATED)
		new_kwd_flags |= TPP_KEYWORD_FLAG_IS_DEPRECATED;
	if (old_kwd_flags != new_kwd_flags) {
		error = tpp_keyword_setflags(keyword, new_kwd_flags);
		if (TPP_ISERR(error))
			goto done;
	}

	/* Apply overrides for feature-macros */
#if TPP_HAVE_KEYWORD_FEATURES
/*[[[deemon
import KEYWORD_FEATURE_KINDS from .config;
for (local kind: KEYWORD_FEATURE_KINDS - {
	"has_c_attribute" // Not part of legacy
}) {
	local KIND = kind.upper();
	print("#if TPP_HAVE_KEYWORD_FEATURE_", KIND);
	print("	error = tpp_keyword_set_legacy_feature_expansion(keyword, TPP_KEYWORD_FEATURE_KIND_", KIND, ",");
	print("	                                                 (flags & TPP_LEGACY_KEYWORD_FLAG_", KIND, ") != 0);");
	print("	if (TPP_ISERR(error))");
	print("		goto done;");
	print("#endif /* TPP_HAVE_KEYWORD_FEATURE_", KIND, " *" "/");
}
]]]*/
#if TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE
	error = tpp_keyword_set_legacy_feature_expansion(keyword, TPP_KEYWORD_FEATURE_KIND_HAS_ATTRIBUTE,
	                                                 (flags & TPP_LEGACY_KEYWORD_FLAG_HAS_ATTRIBUTE) != 0);
	if (TPP_ISERR(error))
		goto done;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN
	error = tpp_keyword_set_legacy_feature_expansion(keyword, TPP_KEYWORD_FEATURE_KIND_HAS_BUILTIN,
	                                                 (flags & TPP_LEGACY_KEYWORD_FLAG_HAS_BUILTIN) != 0);
	if (TPP_ISERR(error))
		goto done;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE
	error = tpp_keyword_set_legacy_feature_expansion(keyword, TPP_KEYWORD_FEATURE_KIND_HAS_CPP_ATTRIBUTE,
	                                                 (flags & TPP_LEGACY_KEYWORD_FLAG_HAS_CPP_ATTRIBUTE) != 0);
	if (TPP_ISERR(error))
		goto done;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE
	error = tpp_keyword_set_legacy_feature_expansion(keyword, TPP_KEYWORD_FEATURE_KIND_HAS_DECLSPEC_ATTRIBUTE,
	                                                 (flags & TPP_LEGACY_KEYWORD_FLAG_HAS_DECLSPEC_ATTRIBUTE) != 0);
	if (TPP_ISERR(error))
		goto done;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION
	error = tpp_keyword_set_legacy_feature_expansion(keyword, TPP_KEYWORD_FEATURE_KIND_HAS_EXTENSION,
	                                                 (flags & TPP_LEGACY_KEYWORD_FLAG_HAS_EXTENSION) != 0);
	if (TPP_ISERR(error))
		goto done;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE
	error = tpp_keyword_set_legacy_feature_expansion(keyword, TPP_KEYWORD_FEATURE_KIND_HAS_FEATURE,
	                                                 (flags & TPP_LEGACY_KEYWORD_FLAG_HAS_FEATURE) != 0);
	if (TPP_ISERR(error))
		goto done;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE */
/*[[[end]]]*/
#endif /* TPP_HAVE_KEYWORD_FEATURES */
done:
	return error;
}

struct tpp_lexer_process_pragma_tpp_set_keyword_flags_data {
	tpp_lexer   *tlpptskfd_lexer;   /* [1..1] The current lexer */
	tpp_keyword *tlpptskfd_keyword; /* [1..1][out] The keyword whose flags should be set */
};

static tpp_errno TPPCALL
tpp_lexer_process_pragma_tpp_set_keyword_flags_cb(void *arg, tpp_string *chunk,
                                                  tpp_char const *str, tpp_size length) {
	tpp_keyword const *ro_keyword;
	tpp_keyword *rw_keyword;
	struct tpp_lexer_process_pragma_tpp_set_keyword_flags_data *data;
	data = (struct tpp_lexer_process_pragma_tpp_set_keyword_flags_data *)arg;
	(void)chunk;
	ro_keyword = tpp_lexer_kwds_newkeyword(data->tlpptskfd_lexer, str, length, tpp_hashof(str, length));
	if tpp_unlikely(!ro_keyword)
		return TPP_ENOMEM;
	rw_keyword = tpp_lexer_kwds_copybuiltin(data->tlpptskfd_lexer, ro_keyword);
	if tpp_unlikely(!rw_keyword)
		return TPP_ENOMEM;
	data->tlpptskfd_keyword = rw_keyword;
	return TPP_EOK;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_tpp_set_keyword_flags(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	tpp_intmax value;
	struct tpp_lexer_process_pragma_tpp_set_keyword_flags_data data;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Parse string naming the keyword */
	data.tlpptskfd_lexer   = self;
	data.tlpptskfd_keyword = NULL;
	if (TPP_TOK_ISSTRING(tok)) {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_tpp_set_keyword_flags_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	if (TPP_ISERR(error))
		return error;
	tok = tpp_lexer_gettok(self);
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip ',' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(','));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Next token must be an integer */
	if (TPP_TOK_ISINT(tok)) {
		error = tpp_lexer_decodeint(self, &value);
		if (TPP_ISERR(error))
			return error;
		value &= TPP_LEGACY_KEYWORD_FLAG_MASK;
		if (data.tlpptskfd_keyword) {
			error = tpp_keyword_set_legacy_flags(data.tlpptskfd_keyword,
			                                     (uint_least8_t)value);
			if (TPP_ISERR(error))
				return error;
		}
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_INT
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_INT);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_INT */
	}
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */





/************************************************************************/
/* #pragma TPP include_path(push, + "/usr/include")                     */
/* #pragma TPP include_path(push, ++ "/usr/include")  // pushhead       */
/* #pragma TPP include_path("/usr/local/include")     // same as '+'    */
/* #pragma TPP include_path(- "/usr/include")                           */
/* #pragma TPP include_path(default: "/usr/include")  // same as direct */
/* #pragma TPP include_path(quote: "/usr/include")    // Added in TPP3  */
/* #pragma TPP include_path(system: "/usr/include")   // Added in TPP3  */
/* #pragma TPP include_path(dirafter: "/usr/include") // Added in TPP3  */
/* #pragma TPP include_path(embed: "/usr/include")    // Added in TPP3  */
/* #pragma TPP include_path(pop)                                        */
/* #pragma TPP include_path(clear)     // Only clears "tip_system_list" */
/* #pragma TPP include_path(default: clear)           // same as direct */
/* #pragma TPP include_path(quote: clear)             // Added in TPP3  */
/* #pragma TPP include_path(system: clear)            // Added in TPP3  */
/* #pragma TPP include_path(dirafter: clear)          // Added in TPP3  */
/* #pragma TPP include_path(embed: clear)             // Added in TPP3  */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
typedef enum tpp_lexer_process_pragma_TPP_include_path_mode {
	TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_DEFAULT,  /* Default mode (same as "TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_ADD_TAIL") */
	TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_ADD_TAIL, /* tpp_include_path_list_pushtail */
	TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_ADD_HEAD, /* tpp_include_path_list_pushhead */
	TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_REMOVE,   /* tpp_include_path_list_remove */
} tpp_lexer_process_pragma_TPP_include_path_mode;

struct tpp_lexer_process_pragma_TPP_include_path_data {
	tpp_lexer                                     *tlpptipd_lexer; /* [1..1] Current lexer */
#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
	tpp_include_path_kind                          tlpptipd_kind;  /* Path kind */
#endif /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
	tpp_lexer_process_pragma_TPP_include_path_mode tlpptipd_mode;  /* How to operate */
};

static tpp_errno TPPCALL
tpp_lexer_process_pragma_TPP_include_path_cb_impl(struct tpp_lexer_process_pragma_TPP_include_path_data *data,
                                                  char const *str) {
	tpp_errno result;
#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
#define tpp_local_pragma_include_path_kind data->tlpptipd_kind
#else /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
#define tpp_local_pragma_include_path_kind TPP_INCLUDE_PATH_KIND_SYSTEM
#endif /* !TPP_HAVE_INCLUDE_PATH_MULTIPLE */
	switch (data->tlpptipd_mode) {
	case TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_DEFAULT:
	case TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_ADD_TAIL:
		result = tpp_lexer_includes_addbykind(data->tlpptipd_lexer,
		                                      tpp_local_pragma_include_path_kind,
		                                      str, TPP_SIZE_MAX);
		break;
	case TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_ADD_HEAD:
		result = tpp_lexer_includes_addbykind_head(data->tlpptipd_lexer,
		                                           tpp_local_pragma_include_path_kind,
		                                           str, TPP_SIZE_MAX);
		break;
	case TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_REMOVE:
		result = tpp_lexer_includes_delbykind(data->tlpptipd_lexer,
		                                      tpp_local_pragma_include_path_kind,
		                                      str, TPP_SIZE_MAX);
		if (result == TPP_ENOENT) {
			/* XXX: Warning? */
			result = TPP_EOK;
		}
		break;
	default: tpp_unreachable();
	}
	return result;
#undef tpp_local_pragma_include_path_kind
}

static tpp_errno TPPCALL
tpp_lexer_process_pragma_TPP_include_path_cb(void *arg, tpp_string *chunk,
                                             tpp_char const *str, tpp_size length) {
	struct tpp_lexer_process_pragma_TPP_include_path_data *data;
	data = (struct tpp_lexer_process_pragma_TPP_include_path_data *)arg;
	(void)chunk;

	/* Add/remove path relative to current *real* filename.
	 * WARNING: This is something that TPP2 didn't use to do! */
	if (!TPP_FS_ISABS(str, length)) {
		tpp_file const *const lcfile = tpp_lexer_getlcfile(data->tlpptipd_lexer);
		char const *const real_filename = tpp_file_getrealfilename(lcfile);
		if (real_filename) {
			tpp_errno result;
			char *relpath = tpp_joinpath(real_filename, (char const *)str, length);
			if tpp_unlikely(!relpath)
				return TPP_ENOMEM;
			result = tpp_lexer_process_pragma_TPP_include_path_cb_impl(data, relpath);
			tpp_free(relpath);
			return result;
		}
	}

	/* Fallback: add/remove path as-is */
	return tpp_lexer_process_pragma_TPP_include_path_cb_impl(data, (char const *)str);
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_TPP_include_path(tpp_lexer *tpp_restrict self) {
	tpp_errno error;
	tpp_token_id tok;
	struct tpp_lexer_process_pragma_TPP_include_path_data data;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Skip leading '(' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('('));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	data.tlpptipd_lexer = self;
again_switch:
#if TPP_HAVE_INCLUDE_PATH_MULTIPLE
	data.tlpptipd_kind = TPP_INCLUDE_PATH_KIND_SYSTEM;
#endif /* TPP_HAVE_INCLUDE_PATH_MULTIPLE */
	data.tlpptipd_mode = TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_DEFAULT;
	switch (tok) {

	case TPP_TOK_SPACE:
	case TPP_TOK_LF:
	TPP_CASE_TPP_TOK_COMMENT
again_yield:
		tok = tpp_lexer_yield_blocking(self);
		goto again_switch;

#if TPP_HAVE_INCLUDE_PATH_PUSH_POP
	case TPP_KWD_push:
		tpp_lexer_pushincludes(self);
		tok = tpp_lexer_yield_blocking(self);
		break;

	case TPP_KWD_pop:
		if (tpp_lexer_canpopincludes(self)) {
			tpp_lexer_popincludes(self);
		} else {
#if TPP_HAVE_TPP_W_CANNOT_POP_INCLUDE_PATHS
			error = tpp_lexer_warnf(self, TPP_W_CANNOT_POP_INCLUDE_PATHS);
			if (TPP_ISERR(error))
				return error;
#endif /* TPP_HAVE_TPP_W_CANNOT_POP_INCLUDE_PATHS */
		}
		tok = tpp_lexer_yield_blocking(self);
		break;
#endif /* TPP_HAVE_INCLUDE_PATH_PUSH_POP */

#if TPP_HAVE_INCLUDE_PATH_QUOTE
	case TPP_KWD_quote:
		data.tlpptipd_kind = TPP_INCLUDE_PATH_KIND_QUOTE;
		goto skip_colon_and_andle_for_pathlist;
#define WANT_skip_colon_and_andle_for_pathlist
#endif /* TPP_HAVE_INCLUDE_PATH_QUOTE */

#if TPP_HAVE_INCLUDE_PATH_SYSHDR
	case TPP_KWD_system:
		data.tlpptipd_kind = TPP_INCLUDE_PATH_KIND_SYSHDR;
		goto skip_colon_and_andle_for_pathlist;
#define WANT_skip_colon_and_andle_for_pathlist
#endif /* TPP_HAVE_INCLUDE_PATH_SYSHDR */

#if TPP_HAVE_INCLUDE_PATH_AFTER
	case TPP_KWD_dirafter:
		data.tlpptipd_kind = TPP_INCLUDE_PATH_KIND_AFTER;
		goto skip_colon_and_andle_for_pathlist;
#define WANT_skip_colon_and_andle_for_pathlist
#endif /* TPP_HAVE_INCLUDE_PATH_AFTER */

#if TPP_HAVE_INCLUDE_PATH_EMBED
	case TPP_KWD_embed:
		data.tlpptipd_kind = TPP_INCLUDE_PATH_KIND_EMBED;
		goto skip_colon_and_andle_for_pathlist;
#define WANT_skip_colon_and_andle_for_pathlist
#endif /* TPP_HAVE_INCLUDE_PATH_EMBED */

	case TPP_KWD_default:
#ifdef WANT_skip_colon_and_andle_for_pathlist
#undef WANT_skip_colon_and_andle_for_pathlist
skip_colon_and_andle_for_pathlist:
#endif /* WANT_skip_colon_and_andle_for_pathlist */
		do {
			tok = tpp_lexer_yield_blocking(self);
		} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(':'));
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
			tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		TPP_FALLTHRU
	case '+':
	case '-':
#if TPP_HAVE_TOK_PLUS_PLUS
	case TPP_TOK_PLUS_PLUS:
#endif /* TPP_HAVE_TOK_PLUS_PLUS */
		/* Consume add-mode token */
		switch (tok) {
		case '-':
			data.tlpptipd_mode = TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_REMOVE;
			tok = tpp_lexer_yield_blocking(self);
			break;
		case '+':
			data.tlpptipd_mode = TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_ADD_TAIL;
#if TPP_CONF_MAYBE_0(TPP_HAVE_TOK_PLUS_PLUS)
			if (!tpp_lexer_has(self, TOK_PLUS_PLUS)) {
				tpp_file *const file = tpp_lexer_getfile(self);
				tpp_char const *pos = file->tf_pos;
				tpp_char ch;
				error = tpp_lexer_readchar(self, &pos, &ch);
				if (TPP_ISERR(error))
					return error;
				if (ch == '+') {
					data.tlpptipd_mode = TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_ADD_HEAD;
					file->tf_pos = pos;
				}
			}
#endif /* TPP_CONF_MAYBE_0(TPP_HAVE_TOK_PLUS_PLUS) */
			tok = tpp_lexer_yield_blocking(self);
			break;
#if TPP_HAVE_TOK_PLUS_PLUS
		case TPP_TOK_PLUS_PLUS:
			data.tlpptipd_mode = TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_ADD_HEAD;
			tok = tpp_lexer_yield_blocking(self);
			break;
#endif /* TPP_HAVE_TOK_PLUS_PLUS */
		default: break;
		}
		while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
			tok = tpp_lexer_yield_blocking(self);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
		if (data.tlpptipd_mode == TPP_LEXER_PROCESS_PRAGMA_TPP_INCLUDE_PATH_MODE_DEFAULT &&
			tpp_lexer_gettok(self) == TPP_KWD_clear) {
	case TPP_KWD_clear:
			error = tpp_lexer_includes_clearbykind(self, data.tlpptipd_kind);
			if (TPP_ISERR(error))
				return error;
			tok = tpp_lexer_yield_blocking(self);
			break;
		}
		if (TPP_TOK_ISSTRING(tok)) {
	TPP_CASE_TPP_TOK_STRING
			error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_TPP_include_path_cb,
			                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
		} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
			error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else  /* TPP_HAVE_TPP_W_EXPECTED_STRING */
			error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
		}
		if (TPP_ISERR(error))
			return error;
		tok = tpp_lexer_gettok(self);
		break;

	default:
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
#ifdef TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_TPP_INCLUDE_PATH
		error = tpp_lexer_warnf(self, TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_TPP_INCLUDE_PATH);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_TPP_INCLUDE_PATH */
		break;
	}

	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tok == ',')
		goto again_yield;
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */





/************************************************************************/
/* #pragma TPP __has_attribute(feature_test_keyword) = "expansion"      */
/* #pragma TPP __has_builtin(feature_test_keyword) = "expansion"        */
/* #pragma TPP __has_cpp_attribute(feature_test_keyword) = "expansion"  */
/* #pragma TPP __has_declspec_attribute(feature_test_keyword) = "expansion" */
/* #pragma TPP __has_extension(feature_test_keyword) = "expansion"      */
/* #pragma TPP __has_feature(feature_test_keyword) = "expansion"        */
/* #pragma TPP __has_c_attribute(feature_test_keyword) = "expansion"    */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES
static TPP_CONSTCALL TPP_WUNUSED tpp_keyword_feature_kind TPPCALL
tpp_keyword_feature_kind_oftok(tpp_token_id mode_tok) {
	switch (mode_tok) {
/*[[[deemon
import KEYWORD_FEATURE_KINDS from .config;
for (local kind: KEYWORD_FEATURE_KINDS) {
	local KIND = kind.upper();
	print("#if TPP_HAVE_KEYWORD_FEATURE_", KIND);
	print("	case TPP_KWD___", kind, ":");
	print("		return TPP_KEYWORD_FEATURE_KIND_", KIND, ";");
	print("#endif /" "* TPP_HAVE_KEYWORD_FEATURE_", KIND, " *" "/");
}
]]]*/
#if TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE
	case TPP_KWD___has_attribute:
		return TPP_KEYWORD_FEATURE_KIND_HAS_ATTRIBUTE;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN
	case TPP_KWD___has_builtin:
		return TPP_KEYWORD_FEATURE_KIND_HAS_BUILTIN;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE
	case TPP_KWD___has_cpp_attribute:
		return TPP_KEYWORD_FEATURE_KIND_HAS_CPP_ATTRIBUTE;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE
	case TPP_KWD___has_declspec_attribute:
		return TPP_KEYWORD_FEATURE_KIND_HAS_DECLSPEC_ATTRIBUTE;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION
	case TPP_KWD___has_extension:
		return TPP_KEYWORD_FEATURE_KIND_HAS_EXTENSION;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE
	case TPP_KWD___has_feature:
		return TPP_KEYWORD_FEATURE_KIND_HAS_FEATURE;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE
	case TPP_KWD___has_c_attribute:
		return TPP_KEYWORD_FEATURE_KIND_HAS_C_ATTRIBUTE;
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE */
/*[[[end]]]*/
	default: tpp_unreachable();
	}
	tpp_unreachable();
}

struct tpp_lexer_process_pragma_TPP_keyword_feature_data {
	tpp_keyword             *tlpptkfd_keyword; /* [1..1] Keyword whose feature to set */
	tpp_keyword_feature_kind tlpptkfd_kind;    /* The kind of feature to set */
};

static tpp_errno TPPCALL
tpp_lexer_process_pragma_TPP_keyword_feature_cb(void *arg, tpp_string *chunk,
                                                tpp_char const *str, tpp_size length) {
	struct tpp_lexer_process_pragma_TPP_keyword_feature_data *data;
	TPP_REF tpp_string *feature_str;
	tpp_errno result;
	data = (struct tpp_lexer_process_pragma_TPP_keyword_feature_data *)arg;
	if (chunk && tpp_string_str(chunk) == str && tpp_string_len(chunk) == length) {
		feature_str = chunk;
		tpp_string_incref(feature_str);
	} else {
		feature_str = tpp_string_malloc(length);
		if tpp_unlikely(!feature_str)
			return TPP_ENOMEM;
		tpp_memcpy(tpp_string_str(feature_str), str,
		           length * sizeof(tpp_char));
	}
	result = tpp_keyword_setfeature(data->tlpptkfd_keyword,
	                                data->tlpptkfd_kind,
	                                feature_str);
	tpp_string_decref(feature_str);
	return result;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_TPP_keyword_feature(tpp_lexer *tpp_restrict self) {
	tpp_keyword const *ro_keyword;
	struct tpp_lexer_process_pragma_TPP_keyword_feature_data data;
#if TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_PRAGMA_TPP_KEYWORD_FEATURES
	char const *mode_name = tpp_lexer_gettokenkwdcstr(self);
#endif /* TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_PRAGMA_TPP_KEYWORD_FEATURES */
	tpp_token_id const mode_tok = tpp_lexer_gettok(self);
	tpp_token_id tok;
	tpp_errno error;
	data.tlpptkfd_kind = tpp_keyword_feature_kind_oftok(mode_tok);
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	tok = tpp_lexer_require(self, TPP_TOK_OFCHAR('('));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	do {
		tok = tpp_lexer_yieldraw_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (TPP_TOK_ISKEYWORD(tok)) {
		ro_keyword = tpp_lexer_gettokenkwd(self);
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_PRAGMA_TPP_KEYWORD_FEATURES
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_IDENTIFIER_AFTER_PRAGMA_TPP_KEYWORD_FEATURES, mode_name);
		if (TPP_ISERR(error))
			return error;
#endif /* TPP_HAVE_TPP_W_EXPECTED_IDENTIFIER_AFTER_PRAGMA_TPP_KEYWORD_FEATURES */
		ro_keyword = tpp_lexer_kwds_getkeyword_byid(self, TPP_KWD_TPP);
		tpp_assert(ro_keyword);
	}
	data.tlpptkfd_keyword = tpp_lexer_kwds_copybuiltin(self, ro_keyword);
	if tpp_unlikely(!data.tlpptkfd_keyword)
		return TPP_ENOMEM;
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR('='));
	while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok))
		tok = tpp_lexer_yield_blocking(self);
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (TPP_TOK_ISSTRING(tok)) {
		error = tpp_lexer_parsestring_cb(self, &tpp_lexer_process_pragma_TPP_keyword_feature_cb,
		                                 &data, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	} else if (tok == TPP_KWD_default) {
		/* Restore default definition of feature */
		error = tpp_keyword_setfeature(data.tlpptkfd_keyword,
		                               data.tlpptkfd_kind,
		                               NULL);
		if (!TPP_ISERR(error)) {
			do {
				tok = tpp_lexer_yield_blocking(self);
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
		}
	} else {
#if TPP_HAVE_TPP_W_EXPECTED_STRING
		error = tpp_lexer_warnf(self, TPP_W_EXPECTED_STRING);
#else  /* TPP_HAVE_TPP_W_EXPECTED_STRING */
		error = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_EXPECTED_STRING */
	}
	return error;
}
#endif /* TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES */





/************************************************************************/
/* #pragma GCC ...                                                      */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
	tpp_lexer_seek_backup backup;
	tpp_char const *pos = tpp_lexer_seek_start(self, &backup);
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	switch (tok) {

#if TPP_HAVE_PRAGMA_GCC_POISON
	case TPP_KWD_poison:
		if (!tpp_lexer_has(self, PRAGMA_GCC_POISON))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_poison(self);
#endif /* TPP_HAVE_PRAGMA_GCC_POISON */

#if TPP_HAVE_PRAGMA_GCC_WARNING && TPP_HAVE_TPP_W_WARNING
	case TPP_KWD_warning:
		if (!tpp_lexer_has(self, PRAGMA_GCC_WARNING))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_warning(self, TPP_W_WARNING);
#endif /* TPP_HAVE_PRAGMA_GCC_WARNING && TPP_HAVE_TPP_W_WARNING */

#if TPP_HAVE_PRAGMA_GCC_ERROR && TPP_HAVE_TPP_W_ERROR
	case TPP_KWD_error:
		if (!tpp_lexer_has(self, PRAGMA_GCC_ERROR))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_warning(self, TPP_W_ERROR);
#endif /* TPP_HAVE_PRAGMA_GCC_ERROR && TPP_HAVE_TPP_W_ERROR */

#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
	case TPP_KWD_system_header:
		if (!tpp_lexer_has(self, PRAGMA_GCC_SYSTEM_HEADER))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_system_header(self);
#endif /* TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER */

#if TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC
	case TPP_KWD_diagnostic:
		if (!tpp_lexer_has(self, PRAGMA_GCC_DIAGNOSTIC))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_diagnostic(self);
#endif /* !TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */

#if TPP_HAVE_PRAGMA_GCC_DEPENDENCY
	case TPP_KWD_dependency:
		if (!tpp_lexer_has(self, PRAGMA_GCC_DEPENDENCY))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_dependency(self);
#endif /* !TPP_HAVE_PRAGMA_GCC_DEPENDENCY */

	default: break;
	}
	tpp_lexer_seek_rollback(self, &backup);
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_GCC */





/************************************************************************/
/* #pragma TPP ...                                                      */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_TPP(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok;
	tpp_lexer_seek_backup backup;
	tpp_char const *pos = tpp_lexer_seek_start(self, &backup);
	do {
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	switch (tok) {

#if TPP_HAVE_PRAGMA_TPP_EXTENSION
	case TPP_KWD_extension:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_extension(self);
#endif /* TPP_HAVE_PRAGMA_TPP_EXTENSION */

#if TPP_HAVE_PRAGMA_TPP_WARNING
	case TPP_KWD_warning:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_warning(self);
#endif /* TPP_HAVE_PRAGMA_TPP_WARNING */

#if TPP_HAVE_PRAGMA_TPP_TPP_EXEC
	case TPP_KWD_tpp_exec:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_tpp_exec(self);
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_EXEC */

#if TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
	case TPP_KWD_tpp_set_keyword_flags:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_tpp_set_keyword_flags(self);
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */

#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
	case TPP_KWD_include_path:
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_TPP_include_path(self);
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */

#if TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES
/*[[[deemon
import KEYWORD_FEATURE_KINDS from .config;
for (local kind: KEYWORD_FEATURE_KINDS) {
	local KIND = kind.upper();
	print("#if TPP_HAVE_KEYWORD_FEATURE_", KIND);
	print("	case TPP_KWD___", kind, ":");
	print("#endif /" "* TPP_HAVE_KEYWORD_FEATURE_", KIND, " *" "/");
}
]]]*/
#if TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE
	case TPP_KWD___has_attribute:
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN
	case TPP_KWD___has_builtin:
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE
	case TPP_KWD___has_cpp_attribute:
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE
	case TPP_KWD___has_declspec_attribute:
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION
	case TPP_KWD___has_extension:
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE
	case TPP_KWD___has_feature:
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE
	case TPP_KWD___has_c_attribute:
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE */
/*[[[end]]]*/
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_TPP_keyword_feature(self);
#endif /* TPP_HAVE_PRAGMA_TPP_KEYWORD_FEATURES */

	default: break;
	}
	tpp_lexer_seek_rollback(self, &backup);
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_TPP */





/* Process a `#pragma` directive, start at the first token that comes after
 * the leading `#pragma` (i.e.: the first token of the actual directive
 * itself)
 *
 * @return: TPP_EOK:    Success (but there may still be garbage after
 *                      the directive that hasn't been parsed, yet).
 * @return: TPP_ENOENT: Unknown pragma (soft-error; caller should not emit
 *                      `TPP_W_EXTRA_TOKENS_AFTER_PRAGMA_DIRECTIVE`)
 * @return: TPP_E*:     Error */
TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma(tpp_lexer *tpp_restrict self) {
	tpp_token_id tok = tpp_lexer_gettok(self);
	switch (tok) {

#if TPP_HAVE_PRAGMA_PUSH_MACRO
	case TPP_KWD_push_macro:
	case TPP_KWD_pop_macro:
		if (!tpp_lexer_has(self, PRAGMA_PUSH_MACRO))
			break;
		return tpp_lexer_process_pragma_pushpop_macro(self, tok);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_PRAGMA_ONCE
	case TPP_KWD_once:
		if (!tpp_lexer_has(self, PRAGMA_ONCE))
			break;
		return tpp_lexer_process_pragma_once(self);
#endif /* TPP_HAVE_PRAGMA_ONCE */

#if TPP_HAVE_PRAGMA_DEPRECATED
	case TPP_KWD_deprecated:
		if (!tpp_lexer_has(self, PRAGMA_DEPRECATED))
			break;
		return tpp_lexer_process_pragma_deprecated(self);
#endif /* TPP_HAVE_PRAGMA_DEPRECATED */

#if TPP_HAVE_PRAGMA_EXTENSION
	case TPP_KWD_extension:
		if (!tpp_lexer_has(self, PRAGMA_EXTENSION))
			break;
		return tpp_lexer_process_pragma_extension(self);
#endif /* TPP_HAVE_PRAGMA_EXTENSION */

#if TPP_HAVE_PRAGMA_WARNING
	case TPP_KWD_warning:
		if (!tpp_lexer_has(self, PRAGMA_WARNING))
			break;
		return tpp_lexer_process_pragma_warning(self);
#endif /* TPP_HAVE_PRAGMA_WARNING */

#if TPP_HAVE_PRAGMA_MESSAGE
	case TPP_KWD_message:
		if (!tpp_lexer_has(self, PRAGMA_MESSAGE))
			break;
		return tpp_lexer_process_pragma_message(self);
#endif /* TPP_HAVE_PRAGMA_MESSAGE */

#if TPP_HAVE_PRAGMA_ERROR
	case TPP_KWD_error:
		if (!tpp_lexer_has(self, PRAGMA_ERROR))
			break;
		return tpp_lexer_process_pragma_error(self);
#endif /* TPP_HAVE_PRAGMA_ERROR */

#if TPP_HAVE_PRAGMA_REGION
		/* >> #pragma region MY_REGION
		 * >> #pragma endregion MY_REGION */
	case TPP_KWD_region:
	case TPP_KWD_endregion:
		if (!tpp_lexer_has(self, PRAGMA_REGION))
			break;
		/* These #pragma-s are no-ops, and the remainder of the #pragma-line should
		 * just be ignored. Since (depending on how the #pragma is being generated),
		 * that might include an unknown number of characters, we cheat a little and
		 * return TPP_EENOENT here, which our caller(s) interprets as:
		 * - Skip all trailing tokens
		 * - Don't warn about the presence of trailing tokens
		 * - Translate to TPP_EOK */
		return TPP_ENOENT;
#endif /* TPP_HAVE_PRAGMA_REGION */

#if TPP_HAVE_PRAGMA_TPP_TPP_EXEC
	case TPP_KWD_tpp_exec:
		if (!tpp_lexer_has(self, PRAGMA_TPP_EXEC))
			break;
		return tpp_lexer_process_pragma_tpp_exec(self);
#endif /* TPP_HAVE_PRAGMA_TPP_TPP_EXEC */

#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
	case TPP_KWD_tpp_set_keyword_flags:
		if (!tpp_lexer_has(self, PRAGMA_TPP_SET_KEYWORD_FLAGS))
			break;
		return tpp_lexer_process_pragma_tpp_set_keyword_flags(self);
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */

#if TPP_HAVE_PRAGMA_GCC
	case TPP_KWD_GCC: {
		tpp_errno error = tpp_lexer_process_pragma_GCC(self);
		if (error != TPP_ENOENT)
			return error;
	}	break;
#endif /* TPP_HAVE_PRAGMA_GCC */

#if TPP_HAVE_PRAGMA_TPP
	case TPP_KWD_TPP: {
		tpp_errno error = tpp_lexer_process_pragma_TPP(self);
		if (error != TPP_ENOENT)
			return error;
	}	break;
#endif /* TPP_HAVE_PRAGMA_TPP */

	/* XXX: #pragma filetag(ENCODING)
	 * Allows users to specify the encoding of the source file, which TPP currently
	 * (tries to) detect automatically (since TPP requires that its input files supply
	 * it with purely UTF-8 data). However, I've already wanted to add some kind of
	 * extension that allows the user to supply an iconv-style library and specify
	 * that input files use some specific encoding -- this pragma could then override
	 * the encoding used thus far, and cause further input to be processed using this
	 * new ENCODING.
	 *
	 * ref: https://www.ibm.com/docs/en/zos/2.2.0?topic=descriptions-pragma-filetag
	 *
	 * s.a. `-finput-charset=...`
	 */

	/* XXX: Builtin support for STDC pragmas:
	 * >> #pragma STDC FENV_ACCESS ON/OFF/DEFAULT
	 * >> #pragma STDC FP_CONTRACT ON/OFF/DEFAULT
	 * >> #pragma STDC CX_LIMITED_RANGE ON/OFF/DEFAULT
	 */

	/* XXX: Builtin support #pragma pack(...) */
	/* XXX: Builtin support #pragma GCC visibility ... */
	/* XXX: #pragma clang diagnostic ... (identical to #pragma GCC diagnostic ...) */
	/* XXX: #pragma clang system_header  (identical to #pragma GCC system_header) */
	/* XXX: #pragma comment(lib, "foo")  (MS extension; call into user-defined hook) */
	/* XXX: #pragma export(name)         (clang extension; indicates that "name" should be exported from shlib) */
	/* XXX: #pragma optimize("", off)    (MS extension) */
	/* XXX: #pragma comment(copyright, "string") (clang extension) */
	/* XXX: #pragma clang deprecated(MIN, "use std::min instead") (clang extension) */
	/* XXX: #pragma clang restrict_expansion(MACRO_NAME, "<reason>") (clang extension) */
	/* XXX: #pragma clang final(FINAL_MACRO) (clang extension) */

	default: break;
	}

	/* User-defined callback hook to parse pragmas not known to TPP itself */
#if TPP_HAVE_UNKNOWN_PRAGMA_HOOK
	{
		tpp_errno error;
		error = tpp_lexer_callhook_unknown_pragma(self);
		if (error != TPP_ENOENT)
			return error;
	}
#endif /* TPP_HAVE_UNKNOWN_PRAGMA_HOOK */

#if TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS
	{
		tpp_errno error;
		error = tpp_lexer_warnf(self, TPP_W_UNKNOWN_PRAGMAS);
		if (!TPP_ISERR(error))
			error = TPP_ENOENT;
		return error;
	}
#else /* TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
	return TPP_ENOENT;
#endif /* !TPP_HAVE_TPP_W_UNKNOWN_PRAGMAS */
}
#endif /* TPP_HAVE_PRAGMA */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_PP_PRAGMA_C */
