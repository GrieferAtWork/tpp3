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

/* Static initializer for `tpp_lexer`
 * >> static tpp_lexer my_lexer =
 * >> #define TPP_LEXER_SELF my_lexer
 * >> #include "tpp-lexer-init.h"
 * >> ;
 */
#ifndef TPP_LEXER_SELF
#error "Must `#define TPP_LEXER_SELF ...` as the lexer's self-expression"
#endif /* !TPP_LEXER_SELF */
{
	/* .TPP_INTERNAL(tl_core) = */ {
		/* .TPP_INTERNAL(tlc_tok) = */ {
			/* .TPP_INTERNAL(tt_id)    = */ TPP_TOK_EOF,
			/* .TPP_INTERNAL(tt_kwd)   = */ NULL,
#if TPP_HAVE_TOKEN_NUMBER
			/* .TPP_INTERNAL(tt_num)   = */ 0,
#endif /* TPP_HAVE_TOKEN_NUMBER */
			/* .TPP_INTERNAL(tt_range) = */ {
				/* .TPP_INTERNAL(ttr_start) = */ NULL,
				/* .TPP_INTERNAL(ttr_end)   = */ NULL
			},
			/* .TPP_INTERNAL(tt_chunk) = */ NULL
		}
	},
#if TPP_HAVE_LEXER_STATE_FLAGS
	/* .TPP_INTERNAL(tl_state) = */ TPP_LEXER_STATE_FLAG_NORMAL,
#endif /* TPP_HAVE_LEXER_STATE_FLAGS */
#if TPP_HAVE_USER_KEYWORDS
	/* .TPP_INTERNAL(tl_kwds) = */ TPP_KEYWORDS_INIT(TPP_LEXER_SELF.TPP_INTERNAL(tl_kwds)),
#endif /* TPP_HAVE_USER_KEYWORDS */
#if TPP_HAVE_EXTENSIONS
	/* .TPP_INTERNAL(tl_exts) = */ {
		/* .TPP_INTERNAL(te_state) = */ {
			/* .TPP_INTERNAL(tes_flags) = */ {
#define TPP_DEFS
#define TPP_EXTENSION(id, name, default) /* .tef_##id = */ default,
#undef GUARD_TPP_AMALGAMATION_H
#include "tpp-amalgamation.h"
#undef TPP_DEFS
			},
		},
#if TPP_HAVE_EXTENSIONS_PUSH_POP
		/* .TPP_INTERNAL(te_pushcnt) = */ 0,
		/* .TPP_INTERNAL(te_prev)    = */ NULL
#endif /* TPP_HAVE_EXTENSIONS_PUSH_POP */
	},
#endif /* TPP_HAVE_EXTENSIONS */
#if TPP_HAVE_FEATURES
	/* .TPP_INTERNAL(tl_feat) = */ TPP_FEATURES_INIT(TPP_LEXER_SELF.TPP_INTERNAL(tl_feat)),
#endif /* TPP_HAVE_FEATURES */
#if TPP_HAVE_INCLUDE_PATH
	/* .TPP_INTERNAL(tl_include_paths) = */ TPP_INCLUDE_PATHS_INIT(TPP_LEXER_SELF.TPP_INTERNAL(tl_include_paths)),
#endif /* TPP_HAVE_INCLUDE_PATH */
#if TPP_HAVE_INCLUDE_PATH_ENVIRON
	/* .TPP_INTERNAL(tl_envinclude_paths) = */ TPP_ENVINCLUDE_PATHS_INIT(TPP_LEXER_SELF.TPP_INTERNAL(tl_envinclude_paths)),
#endif /* TPP_HAVE_INCLUDE_PATH_ENVIRON */
#if TPP_HAVE_HOOKS
	/* .TPP_INTERNAL(tl_hooks) = */ TPP_HOOKS_INIT(TPP_LEXER_SELF.TPP_INTERNAL(tl_hooks), &TPP_LEXER_SELF),
#endif /* TPP_HAVE_HOOKS */
#if TPP_HAVE_WARNINGS
	/* .TPP_INTERNAL(tl_warn) = */ {
		/* .TPP_INTERNAL(tw_state) = */ {
			/* .TPP_INTERNAL(tws_state) = */ {
#define TPP_DEFS
#define TPP_WGROUP(wgroup_id, names, default) \
		/* .twsg_##wgroup_id  = */ (unsigned int)default,
#undef GUARD_TPP_AMALGAMATION_H
#include "tpp-amalgamation.h"
#if TPP_HAVE_WARNING_NUMBERS
#define TPP_DECLARE_NUMBERED_WARNING(numbers_default) \
		/* .twsn_##warning_id = */ (unsigned int)numbers_default,
#define TPP_WARNING(warning_id, wgroup_ids, numbers, numbers_default, format) \
		TPP_TUPLE_IF_NONEMPTY(numbers, TPP_DECLARE_NUMBERED_WARNING, numbers_default)
#undef GUARD_TPP_AMALGAMATION_H
#include "tpp-amalgamation.h"
#undef TPP_DECLARE_NUMBERED_WARNING
#endif /* TPP_HAVE_WARNING_NUMBERS */
#undef TPP_DEFS
			}
		},
#if TPP_HAVE_WARNING_SUPPRESS
		/* .TPP_INTERNAL(tw_suppressions) = */ TPP_WARNING_SUPPRESSIONS_INIT(TPP_LEXER_SELF.TPP_INTERNAL(tl_warn).TPP_INTERNAL(tw_suppressions)),
#endif /* TPP_HAVE_WARNING_SUPPRESS */
#if TPP_HAVE_WARNINGS_PUSH_POP
		/* .TPP_INTERNAL(tw_pushcnt) = */ 0,
		/* .TPP_INTERNAL(tw_prev)    = */ NULL
#endif /* TPP_HAVE_WARNINGS_PUSH_POP */
	},
#endif /* TPP_HAVE_WARNINGS */
#if TPP_HAVE_WARNING_ERROR
	/* .TPP_INTERNAL(tl_error_count) = */ 0,
#endif /* TPP_HAVE_WARNING_ERROR */
#if TPP_HAVE_LEXER_WARNING_COUNT
	/* .TPP_INTERNAL(tl_warning_count) = */ 0,
#endif /* TPP_HAVE_LEXER_WARNING_COUNT */
#if TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED && TPP_MAX_INCLUDE_DEPTH < 0
	/* .TPP_INTERNAL(tl_inclusion_limit) = */ -TPP_MAX_INCLUDE_DEPTH,
#endif /* TPP_HAVE_TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED && TPP_MAX_INCLUDE_DEPTH < 0 */
#if TPP_HAVE_MACRO_RECURSION && TPP_MAX_RECURSIVE_MACRO_DEPTH < 0
	/* .TPP_INTERNAL(tl_recursive_macro_limit) = */ -TPP_MAX_RECURSIVE_MACRO_DEPTH,
#endif /* TPP_HAVE_MACRO_RECURSION && TPP_MAX_RECURSIVE_MACRO_DEPTH < 0 */
#if TPP_HAVE_MACRO___COUNTER__
	/* .TPP_INTERNAL(tl_builtin_counter) = */ 0,
#endif /* TPP_HAVE_MACRO___COUNTER__ */
	/* Value for current time (expansion of `__TIME__` can't change between multiple expansions) */
#if TPP_HAVE_LEXER_TIME
	/* .TPP_INTERNAL(tl_time) = */ TPP_TIME_INIT_EMPTY(TPP_LEXER_SELF.TPP_INTERNAL(tl_time)),
#endif /* TPP_HAVE_LEXER_TIME */
#if TPP_HAVE_LEXER_RAND
	/* .TPP_INTERNAL(tl_rngseed) = */ 0,
#endif /* TPP_HAVE_LEXER_RAND */
#if TPP_HAVE_RT_FILE_AND_LINE_FORMAT
	/* .TPP_INTERNAL(tl_file_and_line_format) = */ TPP_CONFIG_FILE_AND_LINE_FORMAT,
#endif /* TPP_HAVE_RT_FILE_AND_LINE_FORMAT */
#if TPP_HAVE_LEXER_USERPWD
	/* .TPP_INTERNAL(tl_userpwd) = */ NULL,
#endif /* TPP_HAVE_LEXER_USERPWD */
}
#undef TPP_LEXER_SELF