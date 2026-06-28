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
#ifndef GUARD_TPP_MACRO_C
#define GUARD_TPP_MACRO_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "macro.h"
#include "string.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_macro_destroy(tpp_macro *tpp_restrict self) {
	if (self->tm_body_chunk)
		tpp_string_decref(self->tm_body_chunk);
	if (TPP_MACRO_KIND_ISFUNC(self->tm_kind)) {
		tpp_free(self->tm_data.tmd_func.tmf_argv);
		tpp_free(self->tm_data.tmd_func.tmf_argbuf);
	}
	_tpp_macro_free(self);
}

#if TPP_HAVE_LEXER_COPY
static uint_fast8_t const tpp_macro_opcode_length[] = {
	/* [TPP_MACRO_OPCODE_END]      = */ 1 + 0,
	/* [TPP_MACRO_OPCODE_SKIP]     = */ 1 + 1,
	/* [TPP_MACRO_OPCODE_COPY]     = */ 1 + 1,
	/* [TPP_MACRO_OPCODE_INS_EXP]  = */ 1 + 2,
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
	/* [TPP_MACRO_OPCODE_INS_STR]  = */ 1 + 2,
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT
	/* [TPP_MACRO_OPCODE_INS_CHR]  = */ 1 + 2,
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
	/* [TPP_MACRO_OPCODE_INS]      = */ 1 + 2,
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
#if TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
	/* [TPP_MACRO_OPCODE_VA_COMMA] = */ 1 + 1,
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */
#if TPP_HAVE_VA_OPT_IN_MACROS
	/* [TPP_MACRO_OPCODE_VA_OPT]   = */ 1 + 3,
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */
#if TPP_HAVE_VA_NARGS_IN_MACROS
	/* [TPP_MACRO_OPCODE_VA_NARGS] = */ 1 + 1,
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */
};

static TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) tpp_size TPPCALL
tpp_macro_func_expand_count(tpp_macro const *tpp_restrict self) {
	tpp_macro_opcode const *iter = self->tm_data.tmd_func.tmf_expand;
	tpp_assert(tpp_macro_isfunction(self));
	for (;;) {
		tpp_macro_opcode op = *iter;
		tpp_assert(op < tpp_lengthof(tpp_macro_opcode_length));
		iter += tpp_macro_opcode_length[op];
		if (op == TPP_MACRO_OPCODE_END)
			break;
	}
	return (tpp_size)(iter - self->tm_data.tmd_func.tmf_expand);
}

/* Allocate+return a hard-copy of "self"
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) TPP_REF tpp_macro *TPPCALL
tpp_macro_copy(tpp_macro const *tpp_restrict self) {
	TPP_REF tpp_macro *result;
	if (tpp_macro_iskeyword(self)) {
		result = _tpp_macro_alloc_keyword();
		if tpp_unlikely(!result)
			return NULL;
	} else {
		tpp_macro_argument *argv_copy;
		tpp_size expand_count = tpp_macro_func_expand_count(self);
		result = _tpp_macro_alloc_function(expand_count);
		if tpp_unlikely(!result)
			return NULL;
		tpp_memcpy(result->tm_data.tmd_func.tmf_expand,
		           self->tm_data.tmd_func.tmf_expand,
		           expand_count * sizeof(tpp_macro_opcode));
		result->tm_data.tmd_func.tmf_argc = self->tm_data.tmd_func.tmf_argc;
		argv_copy = (tpp_macro_argument *)tpp_malloc(result->tm_data.tmd_func.tmf_argc *
		                                             sizeof(tpp_macro_argument));
		if tpp_unlikely(!argv_copy) {
			_tpp_macro_free(result);
			return NULL;
		}
		tpp_memcpy(argv_copy, self->tm_data.tmd_func.tmf_argv,
		           result->tm_data.tmd_func.tmf_argc *
		           sizeof(tpp_macro_argument));
		result->tm_data.tmd_func.tmf_argv    = argv_copy;
		result->tm_data.tmd_func.tmf_expbase = self->tm_data.tmd_func.tmf_expbase;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
		result->tm_data.tmd_func.tmf_n_vaopt = self->tm_data.tmd_func.tmf_n_vaopt;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
		result->tm_data.tmd_func.tmf_n_vanargs = self->tm_data.tmd_func.tmf_n_vanargs;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
		result->tm_data.tmd_func.tmf_argbuf = NULL; /* Lazily allocated once needed */
	}
	tpp_refcnt_init(&result->tm_refcnt, 1);
	result->tm_kind = self->tm_kind;
#if TPP_HAVE_MACRO_FLAGS
	result->tm_flags = self->tm_flags;
#endif /* TPP_HAVE_MACRO_FLAGS */
	result->tm_body_chunk = self->tm_body_chunk;
	if (result->tm_body_chunk) /* Strings can be shared like this (even across threads) */
		tpp_string_incref(result->tm_body_chunk);
	result->tm_body_start = self->tm_body_start;
	result->tm_body_end = self->tm_body_end;
#if TPP_HAVE_UNICODE
	result->tm_body_enc = self->tm_body_enc;
#endif /* TPP_HAVE_UNICODE */
	result->tm_expansions = 0;                /* New lexer has empty #include-stack, so reset this */
	result->tm_deffile    = self->tm_deffile; /* Relocated later if it's a keyword filename */
	result->tm_deflc      = self->tm_deflc;
	result->tm_body_lc    = self->tm_body_lc;
	return result;
}
#endif /* TPP_HAVE_LEXER_COPY */

#if TPP_HAVE_MACRO_EQUALS
/* Compare 2 macro definitions to see if they are identical. */
TPP_IMPL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_macro_equals(tpp_macro const *lhs, tpp_macro const *rhs) {
	if (lhs->tm_kind != rhs->tm_kind)
		goto nope;
#if TPP_HAVE_MACRO_FLAGS
	if (lhs->tm_flags != rhs->tm_flags)
		goto nope;
#endif /* TPP_HAVE_MACRO_FLAGS */
#if TPP_HAVE_UNICODE
	if (lhs->tm_body_enc != rhs->tm_body_enc)
		goto nope;
#endif /* TPP_HAVE_UNICODE */

	/* Compare bodies */
	{
		tpp_size lhs_body_len = (tpp_size)(lhs->tm_body_end - lhs->tm_body_start);
		tpp_size rhs_body_len = (tpp_size)(rhs->tm_body_end - rhs->tm_body_start);
		if (lhs_body_len != rhs_body_len)
			goto nope;
		if (tpp_memcmp(lhs->tm_body_start, rhs->tm_body_start, lhs_body_len) != 0)
			goto nope;
	}

	/* In case of function-like macro, also compare parameters
	 * (we already compared "kind", so if one's a function, we
	 * know that both of them are) */
	if (tpp_macro_isfunction(lhs)) {
		if (lhs->tm_data.tmd_func.tmf_argc != rhs->tm_data.tmd_func.tmf_argc)
			goto nope;
		if (lhs->tm_data.tmd_func.tmf_expbase != rhs->tm_data.tmd_func.tmf_expbase)
			goto nope;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
		if (lhs->tm_data.tmd_func.tmf_n_vaopt != rhs->tm_data.tmd_func.tmf_n_vaopt)
			goto nope;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
		if (lhs->tm_data.tmd_func.tmf_n_vanargs != rhs->tm_data.tmd_func.tmf_n_vanargs)
			goto nope;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
		if (tpp_memcmp(lhs->tm_data.tmd_func.tmf_argv,
		               rhs->tm_data.tmd_func.tmf_argv,
		               lhs->tm_data.tmd_func.tmf_argc *
		               sizeof(tpp_macro_argument)) != 0)
			goto nope;
		/* No need to compare "tmf_expand" -- if everything until here is equal
		 * (especially the bodies), we can assume that expansion opcodes are, too */
	}
	return true;
nope:
	return false;
}
#endif /* TPP_HAVE_MACRO_EQUALS */


/* Figure out the line/column of "pos" in "expanded_text", as produced
 * by "self", which must be "TPP_MACRO_KIND_ISFUNC(self->tm_kind)". */
TPP_INTERN_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_lcinfo TPPCALL
tpp_macro_func_lcinfo(tpp_macro const *tpp_restrict self,
                      tpp_string const *expanded_text,
                      tpp_char const *pos) {
	(void)self;
	(void)expanded_text;
	(void)pos;
	/* TODO */
	return TPP_LCINFO_INVALID;
}

#endif /* TPP_HAVE_CPP_MACROS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_MACRO_C */
