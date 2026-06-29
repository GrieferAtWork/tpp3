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
		if (data->tlhppmd_undef) {
			/* Also #undef the keyword if requested */
			if (tpp_keyword_canundef(keyword))
				tpp_keyword_undef(keyword);
		}
	} else {
		result = tpp_keyword_popmacro(keyword);
		tpp_assert(!TPP_ISERR(result) ||
		           result == TPP_ENOENT);
		if (result == TPP_ENOENT) {
			/* Emit a warning */
#if TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK
			result = tpp_lexer_warnf(lexer, TPP_W_POP_MACRO_EMPTY_STACK,
			                         (unsigned int)length, str);
#else /* TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
			result = TPP_EOK;
#endif /* !TPP_HAVE_TPP_W_POP_MACRO_EMPTY_STACK */
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
	tpp_file const *iofile;
	tpp_keyword *iofile_kwd;
	iofile     = tpp_file_getiofile(tpp_lexer_getfile(self));
	iofile_kwd = tpp_file_getrealfilenamekwd(iofile);
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
	tpp_lexer *lexer = (tpp_lexer *)arg;
	tpp_keyword const *ro_keyword;
	tpp_keyword *keyword;
	tpp_keyword_misc *misc;
	tpp_hash hash = tpp_hashof(str, length);
	(void)chunk;
	ro_keyword = tpp_keywords_newkeyword(&lexer->tl_kwds, str, length, hash);
	if tpp_unlikely(!ro_keyword)
		return TPP_ENOMEM;
	keyword = tpp_keywords_copybuiltin(&lexer->tl_kwds, ro_keyword);
	if tpp_unlikely(!keyword)
		return TPP_ENOMEM;
	misc = tpp_keyword_requiremisc(keyword);
	if tpp_unlikely(!misc)
		return TPP_ENOMEM;
	misc->tkm_flags |= TPP_KEYWORD_FLAG_IS_DEPRECATED;
	return TPP_EOK;
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
/* #pragma warning(pop)                                                 */
/************************************************************************/
#if TPP_HAVE_PRAGMA_WARNING || TPP_HAVE_PRAGMA_TPP_WARNING
struct tpp_lexer_pragma_warning_state_data {
	tpp_lexer        *tlpwsd_lexer; /* [1..1] Lexer */
	tpp_warning_state tlpwsd_state; /* State to assign to warning */
};

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_pragma_warning_setstate_impl(tpp_lexer *self,
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

#if TPP_HAVE_TPP_TOK_INT
	{
		tpp_intmax mode;
		bool negative;
	case TPP_TOK_INT:
		negative = false;
		if (0) {
	case '-':
			negative = true;
			do {
				tok = tpp_lexer_yield_blocking(self);
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			tok = tpp_lexer_require(self, TPP_TOK_INT);
			if (TPP_TOK_ISERR(tok))
				return TPP_TOK_ASERR(tok);
			if (tok != TPP_TOK_INT)
				break;
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
#endif /* TPP_HAVE_TPP_TOK_INT */
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

#if TPP_HAVE_TPP_TOK_INT && TPP_HAVE_WARNING_NUMBERS
		case TPP_TOK_INT: {
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
#endif /* TPP_HAVE_TPP_TOK_INT && TPP_HAVE_WARNING_NUMBERS */

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
#if TPP_HAVE_TPP_TOK_INT && TPP_HAVE_WARNING_NUMBERS
		if (tok == TPP_TOK_INT)
			goto again_handle_set_warning_state;
#endif /* TPP_HAVE_TPP_TOK_INT && TPP_HAVE_WARNING_NUMBERS */
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
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_message(tpp_lexer *tpp_restrict self) {
	/* TODO: Need another user-overwritable output printer (similar to the lexer's warning printer) */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_MESSAGE */





/************************************************************************/
/* #pragma error("...")                                                 */
/************************************************************************/
#if TPP_HAVE_PRAGMA_ERROR
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_error(tpp_lexer *tpp_restrict self) {
	/* TODO */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_ERROR */





/************************************************************************/
/* #pragma GCC system_header                                            */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_system_header(tpp_lexer *tpp_restrict self) {
	tpp_file *iofile = tpp_file_getiofile(tpp_lexer_getfile(self));
	if (iofile->tf_kind == TPP_FILE_KIND_IO)
		iofile->tf_data.td_io.tff_flags |= TPP_FILE_IOFLAGS_SYSHDR;
	return TPP_EOK;
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
	/* TODO */
	/* TODO: Only support push when "TPP_HAVE_WARNINGS_PUSH_POP" */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC */





/************************************************************************/
/* #pragma GCC poison foo bar foobar ...                                */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_POISON
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_poison(tpp_lexer *tpp_restrict self) {
	/* TODO */
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
	 *                the case when the argument call site is no longer on the #include-
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
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_warning(tpp_lexer *tpp_restrict self, tpp_warning_id mode) {
	/* TODO */
	(void)self;
	(void)mode;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_GCC_WARNING || TPP_HAVE_PRAGMA_GCC_ERROR */





/************************************************************************/
/* #pragma GCC dependency <file> [<message>]                            */
/************************************************************************/
#if TPP_HAVE_PRAGMA_GCC_DEPENDENCY
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_GCC_dependency(tpp_lexer *tpp_restrict self) {
	/* TODO */
	(void)self;
	return TPP_ENOENT;
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
	tpp_file_subtext_push(file);
	tpp_file_subtext_setchunk_fromstring(file, chunk, str, length);
	/* Parse contents of string, but discard all tokens. */
	do {
		tok = tpp_lexer_yield_blocking(self);
	} while (!TPP_TOK_ISERR_OR_EOF(tok));

	/* Force cleanup in case of error, and warn about unclosed #if-blocks */
	for (;;) {
#if TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF
		if (!TPP_TOK_ISERR(tok)) {
			tpp_errno error = tpp_lexer_warn_nonempty_ifdef(self);
			if (TPP_ISERR(error))
				tok = TPP_TOK_OFERR(error);
		}
#endif /* TPP_HAVE_TPP_W_EOF_BEFORE_ENDIF */
		if (!tpp_lexer_canpopfile(self))
			break;
		tpp_lexer_popfile(self);
	}
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
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);

	/* Parse string to execute */
	error = tpp_lexer_parsestring_cb(self, &tpp_lexer_pragma_tpp_exec_cb,
	                                 self, TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS);
	if (TPP_ISERR(error))
		return error;

	/* Skip trailing ')' */
	tok = tpp_lexer_skip(self, TPP_TOK_OFCHAR(')'));
	return TPP_TOK_ASERR_OR_EOK(tok);
}
#endif /* TPP_HAVE_PRAGMA_TPP_EXEC || TPP_HAVE_PRAGMA_TPP_TPP_EXEC */





/************************************************************************/
/* #pragma tpp_set_keyword_flags("foo", 0x7f)                           */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_tpp_set_keyword_flags(tpp_lexer *tpp_restrict self) {
	/* TODO */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS || TPP_HAVE_PRAGMA_TPP_TPP_SET_KEYWORD_FLAGS */





/************************************************************************/
/* #pragma TPP include_path(push, + "/usr/include")                     */
/* #pragma TPP include_path("/usr/local/include")   // same as '+'      */
/* #pragma TPP include_path(- "/usr/include")                           */
/* #pragma TPP include_path(pop)                                        */
/* #pragma TPP include_path(clear)                                      */
/************************************************************************/
#if TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_lexer_process_pragma_TPP_include_path(tpp_lexer *tpp_restrict self) {
	/* TODO */
	/* TODO: Only support push when "TPP_HAVE_INCLUDE_PATH_PUSH_POP" */
	/* TODO: Expand this extension to allow modification of the quote- and after- path lists */
	(void)self;
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_TPP_INCLUDE_PATH */





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

#if TPP_HAVE_PRAGMA_GCC_WARNING
	case TPP_KWD_warning:
		if (!tpp_lexer_has(self, PRAGMA_GCC_WARNING))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_warning(self, TPP_W_WARNING);
#endif /* TPP_HAVE_PRAGMA_GCC_WARNING */

#if TPP_HAVE_PRAGMA_GCC_ERROR
	case TPP_KWD_error:
		if (!tpp_lexer_has(self, PRAGMA_GCC_ERROR))
			break;
		tpp_lexer_seek_commit(self, pos);
		return tpp_lexer_process_pragma_GCC_warning(self, TPP_W_ERROR);
#endif /* TPP_HAVE_PRAGMA_GCC_ERROR */

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

	default: break;
	}
	tpp_lexer_seek_rollback(self, &backup);
	return TPP_ENOENT;
}
#endif /* TPP_HAVE_PRAGMA_TPP */





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

	/* TODO: User-defined callback hook to parse pragmas not known to TPP itself */

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
