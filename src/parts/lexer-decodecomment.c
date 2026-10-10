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
#ifndef GUARD_TPP_LEXER_DECODECOMMENT_C
#define GUARD_TPP_LEXER_DECODECOMMENT_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "lexer.h"
#include "preparse.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_LEXER_DECODECOMMENT
/* Initialize `result` as the start/end sub-range of the actual comment text.
 * That means that any leading (or in case of block-style comments: trailing)
 * character-sequence used to start (or end) the comment token will *NOT* be
 * included in the result.
 *
 * Additionally, any BSE sequences just after the leading (or just before the
 * trailing) comment-character-sequence is *NOT* included in `*result`, meaning
 * that (assuming the range is non-empty), `*tpp_token_range_getstart(result)`
 * is the first character of the actual comment.
 *
 * Examples:
 *
 * | -------------- | ---------------- |
 * | Line comment   | Block comment    |
 * | -------------- | ---------------- |
 * |      ↓start    |                  |
 * | >> // foo      |  >> (* foo *)    |
 * | >> <next line> |  start↑    ↑end  |
 * |   ↑end         |                  |
 *
 * @return: TPP_LEXER_DECODECOMMENT_NONE:  `!TPP_TOK_ISCOMMENT(tpp_lexer_gettok(self))`
 * @return: TPP_LEXER_DECODECOMMENT_LINE:  `TPP_TOK_ISCOMMENT_LINE(tpp_lexer_gettok(self))`
 * @return: TPP_LEXER_DECODECOMMENT_BLOCK: `TPP_TOK_ISCOMMENT_NOLINE(tpp_lexer_gettok(self))` */
#if TPP_HAVE_TOK_COMMENTLIKE
TPP_IMPL TPP_NONNULL((1, 2)) unsigned int TPPCALL
tpp_lexer_decodecomment(tpp_lexer const *tpp_restrict self,
                        tpp_token_range *tpp_restrict result) {
	unsigned int kind = TPP_LEXER_DECODECOMMENT_NONE;
	tpp_char const *start = tpp_lexer_gettokenstart(self);
	tpp_char const *end = tpp_lexer_gettokenend(self);
	switch (tpp_lexer_gettok(self)) {

#if TPP_HAVE_TOK_C_COMMENT || TPP_HAVE_TOK_PASCAL_COMMENT
	_TPP_CASE_TPP_TOK_C_COMMENT
	_TPP_CASE_TPP_TOK_PASCAL_COMMENT
		kind = TPP_LEXER_DECODECOMMENT_BLOCK;
		if (start >= end)
			break;
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		if (start >= end)
			break;
		/* *start == '*' */
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		if (start >= end)
			break;
#if TPP_HAVE_TOK_C_COMMENT && TPP_HAVE_TOK_PASCAL_COMMENT
		if (end[-1] == ((tpp_lexer_gettok(self) == TPP_TOK_C_COMMENT) ? '/' : ')'))
#elif TPP_HAVE_TOK_C_COMMENT
		if (end[-1] == '/')
#else /* TPP_HAVE_TOK_PASCAL_COMMENT */
		if (end[-1] == ')')
#endif /* ... */
		{
			tpp_char const *new_end = end - 1;
			new_end = tpp_preparse_skipbse_bck(self, start, new_end);
			if ((new_end - 1) > start && new_end[-1] == '*')
				end = tpp_preparse_skipbse_bck(self, start, new_end - 1);
		}
		break;
#endif /* TPP_HAVE_TOK_C_COMMENT || TPP_HAVE_TOK_PASCAL_COMMENT */

#if TPP_HAVE_TOK_PASCAL_BRACE_COMMENT
	case TPP_TOK_PASCAL_BRACE_COMMENT:
		kind = TPP_LEXER_DECODECOMMENT_BLOCK;
		if (start >= end)
			break;
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		if (start >= end)
			break;
		if (end[-1] == '}')
			end = tpp_preparse_skipbse_bck(self, start, end - 1);
		break;
#endif /* TPP_HAVE_TOK_PASCAL_BRACE_COMMENT */

#if TPP_HAVE_TOK_HTML_COMMENT
	case TPP_TOK_HTML_COMMENT:
		kind = TPP_LEXER_DECODECOMMENT_BLOCK;
		if (start >= end)
			break;
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		if (start >= end)
			break;
		/* *start == '!' */
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		if (start >= end)
			break;
		/* *start == '-' */
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		if (start >= end)
			break;
		/* *start == '-' */
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		if (start >= end)
			break;
		if (end[-1] == '>') {
			tpp_char const *new_end = end - 1;
			new_end = tpp_preparse_skipbse_bck(self, start, new_end);
			if ((new_end - 2) > start && new_end[-1] == '-') {
				--new_end;
				new_end = tpp_preparse_skipbse_bck(self, start, new_end);
				if ((new_end - 1) > start && new_end[-1] == '-')
					end = tpp_preparse_skipbse_bck(self, start, new_end - 1);
			}
		}
		break;
#endif /* TPP_HAVE_TOK_HTML_COMMENT */


#define TPP_HAVE_TOK_LINE_COMMENT_2CHAR                      \
	(TPP_HAVE_TOK_CXX_COMMENT || TPP_HAVE_TOK_CXX_COMMENT || \
	 TPP_HAVE_TOK_AT_AT_COMMENT)
#define TPP_HAVE_TOK_LINE_COMMENT_1CHAR                           \
	(TPP_HAVE_TOK_SHELL_COMMENT || TPP_HAVE_TOK_SLASH_COMMENT ||  \
	 TPP_HAVE_TOK_AT_COMMENT || TPP_HAVE_TOK_SOL_SHELL_COMMENT || \
	 TPP_HAVE_TOK_SOL_SLASH_COMMENT || TPP_HAVE_TOK_SOL_AT_COMMENT)

#if TPP_HAVE_TOK_LINE_COMMENT_2CHAR
	_TPP_CASE_TPP_TOK_CXX_COMMENT
	_TPP_CASE_TPP_TOK_SQL_COMMENT
	_TPP_CASE_TPP_TOK_AT_AT_COMMENT
		kind = TPP_LEXER_DECODECOMMENT_LINE;
		if (start >= end)
			break;
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
#if TPP_HAVE_TOK_LINE_COMMENT_1CHAR
		TPP_FALLTHRU
#endif /* TPP_HAVE_TOK_LINE_COMMENT_1CHAR */
#endif /* TPP_HAVE_TOK_LINE_COMMENT_2CHAR */
#if TPP_HAVE_TOK_LINE_COMMENT_2CHAR || TPP_HAVE_TOK_LINE_COMMENT_1CHAR
	_TPP_CASE_TPP_TOK_SHELL_COMMENT
	_TPP_CASE_TPP_TOK_SLASH_COMMENT
	_TPP_CASE_TPP_TOK_AT_COMMENT
	_TPP_CASE_TPP_TOK_SOL_SHELL_COMMENT
	_TPP_CASE_TPP_TOK_SOL_SLASH_COMMENT
	_TPP_CASE_TPP_TOK_SOL_AT_COMMENT
		kind = TPP_LEXER_DECODECOMMENT_LINE;
		if (start >= end)
			break;
		start = tpp_preparse_skipbse_fwd(self, start + 1, end);
		break;
#endif /* TPP_HAVE_TOK_LINE_COMMENT_2CHAR || TPP_HAVE_TOK_LINE_COMMENT_1CHAR */
#undef TPP_HAVE_TOK_LINE_COMMENT_1CHAR
#undef TPP_HAVE_TOK_LINE_COMMENT_2CHAR

	default: break;
	}
	tpp_token_range_init(result, start, end);
	return kind;
}
#endif /* TPP_HAVE_TOK_COMMENTLIKE */
#endif /* TPP_HAVE_LEXER_DECODECOMMENT */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_DECODECOMMENT_C */
