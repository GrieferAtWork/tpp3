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
#include "lexer.h"
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


typedef struct tpp_macro_func_lcscan_vars {
	tpp_macro_opcode const *tmflcsv_pc;           /* [1..1] Macro expansion program counter */
	tpp_char const         *tmflcsv_expand_start; /* [1..1] Start of macro expansion */
	tpp_char const         *tmflcsv_expand_end;   /* [1..1] End of macro expansion */
	tpp_char const         *tmflcsv_expand_pos;   /* [1..1] Position to find in macro expansion */
	tpp_char const         *tmflcsv_body_start;   /* [1..1] Start of macro body */
	tpp_char const         *tmflcsv_body_end;     /* [1..1] End of macro body */
	tpp_macro_opcode const *tmflcsv_argopcode;    /* [0..1][out] When non-null on success, "pos" is located within text expanded by this opcode.
	                                               * In this case, tmflcsv_expand_start/tmflcsv_body_start describe the state at the start of
	                                               * the argument insertion:
	                                               * - TPP_MACRO_OPCODE_INS_EXP
	                                               * - TPP_MACRO_OPCODE_INS_STR
	                                               * - TPP_MACRO_OPCODE_INS_CHR
	                                               * - TPP_MACRO_OPCODE_INS
	                                               * NOTES:
	                                               * - argument_index = tmflcsv_argopcode[1]
	                                               * - offset_into_expansion_of_argument = tmflcsv_expand_pos - tmflcsv_expand_start
	                                               * - start_of_argument = tmflcsv_expand_start  (gets overwritten to location where argument is known to terminate)
	                                               * - end_of_argument = tmflcsv_expand_end      (gets overwritten to location where argument is known to terminate)
	                                               * - length_of_argument_in_body = tmflcsv_argopcode[2] */
} tpp_macro_func_lcscan_vars;

#ifndef tpp_memmem
static TPP_PURECALL TPP_WUNUSED void *
tpp_memmem(void const *haystack, size_t haystack_length,
           void const *needle, size_t needle_length) {
	unsigned char *candidate, marker;
	if tpp_unlikely(!needle_length)
		return (void *)haystack;
	if tpp_unlikely(needle_length > haystack_length)
		return NULL;
	haystack_length -= (needle_length - 1);
	marker = *(unsigned char const *)needle;
	while ((candidate = (unsigned char *)tpp_memchr(haystack, marker, haystack_length)) != NULL) {
		if (tpp_memcmp(candidate, needle, needle_length) == 0)
			return (void *)candidate;
		++candidate;
		haystack_length = ((unsigned char const *)haystack + haystack_length) - candidate;
		haystack        = (void const *)candidate;
	}
	return NULL;
}
#endif /* !tpp_memmem */

#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
/* Return a pointer *after* the next unescaped "quote_ch", or re-return "end" */
static TPP_PURECALL TPP_RETNONNULL TPP_WUNUSED tpp_char const *TPPCALL
tpp_find_next_unescaped_quote(tpp_char const *text, tpp_char const *end, tpp_char quote_ch) {
	while (text < end) {
		tpp_char ch = *text++;
		if (ch == quote_ch)
			break;
		/* No need to handle ??/-trigraphs here: only used to scan output
		 * of "tpp_token_encodestring", which doesn't emit trigraphs! */
		if (ch == '\\' && text < end)
			++text; /* Skip next character */
	}
	return text;
}
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */

static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_macro_func_lcscan(tpp_macro_func_lcscan_vars *tpp_restrict self) {
	tpp_macro_opcode opcode;
	tpp_macro_func_lcscan_vars nested_vars;
	tpp_macro_opcode const *p_align_opcode = NULL; /* Pointer to TPP_MACRO_OPCODE_INS*-like opcode required for alignment */
	tpp_char const *p_align_body_start = NULL;     /* Macro body start pointer for "p_align_opcode" */
again_read_opcode:
	opcode = *self->tmflcsv_pc++;
	switch (opcode) {

	case TPP_MACRO_OPCODE_END:
		if (p_align_opcode) {
			/* Position is within an argument */
			self->tmflcsv_body_start = p_align_body_start;
			self->tmflcsv_argopcode  = p_align_opcode;
			return true;
		}
		if (self->tmflcsv_expand_start >= self->tmflcsv_expand_pos) {
			self->tmflcsv_argopcode = NULL;
			return true; /* Location found! */
		}
		/* End of body before match could be found */
		return false;

	case TPP_MACRO_OPCODE_SKIP:
		self->tmflcsv_body_start += *self->tmflcsv_pc++;
		break;

	case TPP_MACRO_OPCODE_COPY: {
		tpp_size template_size = *self->tmflcsv_pc++;
		tpp_char const *template_base = self->tmflcsv_body_start;
		self->tmflcsv_body_start += template_size;
		tpp_assert(self->tmflcsv_body_start <= self->tmflcsv_body_end);
		if (!template_size)
			break;
		if (p_align_opcode) {
			/* After argument -> seek match */
			tpp_char const *scanner = self->tmflcsv_expand_start;
			for (;;) {
				tpp_char const *candidate;
				tpp_assert(scanner <= self->tmflcsv_expand_end);
				candidate = (tpp_char const *)tpp_memmem(scanner, (tpp_size)(self->tmflcsv_expand_end - scanner),
				                                         template_base, template_size);
				if (!candidate)
					return false;
				if (candidate > self->tmflcsv_expand_pos) {
					/* Requested position is within preceding "p_align_opcode" */
					self->tmflcsv_body_start = p_align_body_start;
					self->tmflcsv_argopcode  = p_align_opcode;
					self->tmflcsv_expand_end = candidate;
					return true;
				}
				candidate += template_size;
				if (candidate > self->tmflcsv_expand_pos) {
					/* Requested position is after preceding "p_align_opcode" */
					tpp_size rewind = (tpp_size)(candidate - self->tmflcsv_expand_pos);
					self->tmflcsv_body_start -= rewind;
					self->tmflcsv_argopcode  = NULL;
					return true;
				}
				nested_vars = *self;
				nested_vars.tmflcsv_expand_start = candidate;
				if (tpp_macro_func_lcscan(&nested_vars)) {
					*self = nested_vars;
					return true;
				}
				scanner = candidate + 1;
			}
			tpp_unreachable();
		}
		/* Immediate text -> output must match */
		if (tpp_memcmp(self->tmflcsv_expand_start, template_base, template_size) != 0)
			return false;
		self->tmflcsv_expand_start += template_size;
		tpp_assert(self->tmflcsv_expand_start <= self->tmflcsv_expand_end);
		if (self->tmflcsv_expand_start > self->tmflcsv_expand_pos) {
			tpp_size rewind = (tpp_size)(self->tmflcsv_expand_start - self->tmflcsv_expand_pos);
			self->tmflcsv_body_start -= rewind;
			self->tmflcsv_argopcode = NULL;
			return true; /* Location found! */
		}
	}	break;

#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
	case TPP_MACRO_OPCODE_INS:
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
	case TPP_MACRO_OPCODE_INS_EXP: {
		/* NOTE: If "p_align_opcode != NULL" at this point, the given "pos" (may) actually be
		 *       part of multiple arguments:
		 * >> #define cat(a, b) a##b
		 * >> cat(10, 20)
		 *
		 * The produced "1020" token has 2 projection arguments: "10" and "20",
		 * with "10" already being located in "p_align_opcode" when we get here,
		 * and "20" being described by the the currently-read opcode.
		 *
		 * Too keep things simple, we only ever return info about "20" if we
		 */
#if 0 /* Set to "1" to return info about "10" in the above example; else, info about "20" is returned */
		if (p_align_opcode == NULL)
#endif
		{
			p_align_opcode     = self->tmflcsv_pc - 1;
			p_align_body_start = self->tmflcsv_body_start;
		}
		++self->tmflcsv_pc; /* Argument index */
		self->tmflcsv_body_start += *self->tmflcsv_pc++;
	}	break;

#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
	case TPP_MACRO_OPCODE_INS_STR:
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT
	case TPP_MACRO_OPCODE_INS_CHR:
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
	{
		tpp_char const *string_end;
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT && TPP_HAVE_CHARIZE_MACRO_ARGUMENT
		tpp_char const quote_ch = opcode == TPP_MACRO_OPCODE_INS_STR ? '"' : '\'';
#elif TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
		tpp_char const quote_ch = '"';
#elif TPP_HAVE_CHARIZE_MACRO_ARGUMENT
		tpp_char const quote_ch = '\'';
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
		if (p_align_opcode) {
			tpp_char const *scanner = self->tmflcsv_expand_start;
			for (;;) {
				tpp_char const *candidate;
				tpp_assert(scanner <= self->tmflcsv_expand_end);
				candidate = (tpp_char const *)tpp_memchr(scanner, quote_ch,
				                                         (tpp_size)(self->tmflcsv_expand_end - scanner));
				if (!candidate)
					return false;
				if (candidate > self->tmflcsv_expand_pos) {
					/* Requested position is within preceding "p_align_opcode" */
					self->tmflcsv_body_start = p_align_body_start;
					self->tmflcsv_argopcode  = p_align_opcode;
					self->tmflcsv_expand_end = candidate;
					return true;
				}
				candidate = tpp_find_next_unescaped_quote(candidate, self->tmflcsv_expand_end, quote_ch);
				if (candidate > self->tmflcsv_expand_pos) {
					/* Requested position is within this stringized argument */
					self->tmflcsv_argopcode = self->tmflcsv_pc - 1;
					return true;
				}
				nested_vars = *self;
				nested_vars.tmflcsv_expand_start = candidate;
				if (tpp_macro_func_lcscan(&nested_vars)) {
					*self = nested_vars;
					return true;
				}
				scanner = candidate + 1;
			}
			tpp_unreachable();
		}
		if (*self->tmflcsv_expand_start != quote_ch)
			return false;
		string_end = self->tmflcsv_expand_start + 1;
		string_end = tpp_find_next_unescaped_quote(string_end, self->tmflcsv_expand_end, quote_ch);
		if (string_end > self->tmflcsv_expand_pos) {
			/* Found it within this argument! */
			self->tmflcsv_argopcode  = self->tmflcsv_pc - 1;
			self->tmflcsv_expand_end = string_end;
			return true;
		}
	}	break;
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */

#if TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS || TPP_HAVE_VA_OPT_IN_MACROS
	{
		tpp_char const *va_opt_text_base;
		tpp_size va_opt_text_size;
#if TPP_HAVE_VA_OPT_IN_MACROS
	case TPP_MACRO_OPCODE_VA_OPT:
		self->tmflcsv_body_start += *self->tmflcsv_pc++; /* Head */
		va_opt_text_base = self->tmflcsv_body_start;
		va_opt_text_size = *self->tmflcsv_pc++;          /* Body */
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */
#if TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
		if (0) {
	case TPP_MACRO_OPCODE_VA_COMMA:
			va_opt_text_base = (tpp_char const *)",";
			va_opt_text_size = 1;
		}
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */
		self->tmflcsv_body_start += *self->tmflcsv_pc++; /* Tail */
		if (p_align_opcode) {
			/* If there is *already* an argument pending, then we have no
			 * was of determining if a trailing "," (or whatever) is part of
			 * that argument's expansion, or was generated by __VA_COMMA__:
			 * >> #define wtf(a, ...) a##__VA_COMMA__
			 * >> wtf(10, 20, 30);  // Expands to "10,", but we can't know if that
			 * >>                   // "," is from "__VA_COMMA__" or from arguments */
			break;
		}
		if ((tpp_size)(self->tmflcsv_expand_end - self->tmflcsv_expand_start) >= va_opt_text_size &&
			tpp_memcmp(self->tmflcsv_expand_start, va_opt_text_base, va_opt_text_size) == 0) {
			self->tmflcsv_expand_start += va_opt_text_size;
			if (self->tmflcsv_expand_start > self->tmflcsv_expand_pos) {
				self->tmflcsv_body_start -= *--self->tmflcsv_pc; /* Undo tail */
#if TPP_HAVE_VA_OPT_IN_MACROS
#if TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
				if (opcode == TPP_MACRO_OPCODE_VA_OPT)
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */
				{
					tpp_size rewind = (tpp_size)(self->tmflcsv_expand_start - self->tmflcsv_expand_pos);
					/* Found position in va-opt text */
					self->tmflcsv_body_start -= rewind;
				}
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */
				self->tmflcsv_argopcode = NULL;
				return true;
			}

			/* Check if we can resolve the position with the VA_OPT text skipped */
			nested_vars = *self;
			if (tpp_macro_func_lcscan(&nested_vars)) {
				*self = nested_vars;
				return true;
			}
			self->tmflcsv_expand_start -= va_opt_text_size;
		}

		/* Continue parsing under the assuming that there were no variable arguments... */
	}	break;
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS || TPP_HAVE_VA_OPT_IN_MACROS */

#if TPP_HAVE_VA_NARGS_IN_MACROS
	case TPP_MACRO_OPCODE_VA_NARGS: {
		tpp_char nargs_ch;
		self->tmflcsv_body_start += *self->tmflcsv_pc++;
		if (p_align_opcode) {
			tpp_char const *scanner = self->tmflcsv_expand_start;
			for (;;) {
				tpp_assert(scanner <= self->tmflcsv_expand_end);
				for (;;) {
					if (scanner >= self->tmflcsv_expand_end)
						return false;
					nargs_ch = *scanner;
					if (nargs_ch >= '0' && nargs_ch <= '9')
						break;
					++scanner;
				}
				if (scanner > self->tmflcsv_expand_pos) {
					/* Requested position is within preceding "p_align_opcode" */
					self->tmflcsv_body_start = p_align_body_start;
					self->tmflcsv_argopcode  = p_align_opcode;
					self->tmflcsv_expand_end = scanner;
					return true;
				}
				++scanner;
				if (scanner > self->tmflcsv_expand_pos) {
					self->tmflcsv_argopcode = NULL;
					return true; /* Requested position is within this __VA_NARGS__ argument */
				}
				nested_vars = *self;
				nested_vars.tmflcsv_expand_start = scanner;
				if (tpp_macro_func_lcscan(&nested_vars)) {
					*self = nested_vars;
					return true;
				}
			}
			tpp_unreachable();
		}
		if (self->tmflcsv_expand_start >= self->tmflcsv_expand_pos) { /* ">=" because __VA_NARGS__ is always non-empty */
			self->tmflcsv_argopcode = NULL;
			return true; /* Location found! */
		}
		/* Scan decimals and try to match what comes after... */
		for (;;) {
			nargs_ch = *self->tmflcsv_expand_start;
			if (!(nargs_ch >= '0' && nargs_ch <= '9'))
				return false;
			++self->tmflcsv_expand_start;
			nested_vars = *self;
			if (tpp_macro_func_lcscan(&nested_vars)) {
				*self = nested_vars;
				return true;
			}
			if (self->tmflcsv_expand_start > self->tmflcsv_expand_pos) {
				self->tmflcsv_argopcode = NULL;
				return true; /* Location found! */
			}
		}
	}	break;
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */

	default: tpp_unreachable();
	}
	goto again_read_opcode;
}

/* Return "(tpp_size)(argument_pos - argument_start)", but do special handling when
 * "TPP_MACRO_OPCODE_INS_STR" / "TPP_MACRO_OPCODE_INS_CHR", where the offset into
 * the text that was originally encoded (via tpp_token_encodestring()) is returned
 * instead. */
static tpp_size TPPCALL
tpp_macro_determine_argument_offset(tpp_macro_opcode argument_opcode,
                                    tpp_char const *argument_start,
                                    tpp_char const *argument_pos) {
	switch (argument_opcode) {
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
	case TPP_MACRO_OPCODE_INS_STR:
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT
	case TPP_MACRO_OPCODE_INS_CHR:
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
	{
		tpp_size result = 0;
		++argument_start; /* Skip over leading " or ' character (which was added during encoding) */
		while (argument_start < argument_pos) {
			tpp_char ch = *argument_start++;
			++result;
			if (ch == '\\') {
#if TPP_HAVE_UNICODE
				ch = *argument_start++;
				/* Deal with 4-byte encoding of "\xc2" and "\xe2"
				 * All other encodings are 2-byte (which is the
				 * offset already within "argument_start") */
				if (ch == 'x')
					argument_start += 2;
#else /* TPP_HAVE_UNICODE */
				/* Without unicode, tpp_token_encodestring() always emits 1 extra byte after \ */
				++argument_start;
#endif /* !TPP_HAVE_UNICODE */
			}
		}
		return result;
	}	break;
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
	default: break;
	}
	return (tpp_size)(argument_pos - argument_start);
}


/* Figure out the line/column of "pos" in "expanded_text", as produced
 * by "self", which must be "TPP_MACRO_KIND_ISFUNC(self->tm_kind)". */
TPP_INTERN_IMPL TPP_NONNULL((1, 2, 3, 4, 5)) void TPPCALL
tpp_macro_func_lcinfo(tpp_macro const *self,
                      tpp_file const *expanded_text_file,
                      tpp_string const *expanded_text,
                      tpp_char const *pos,
                      tpp_lcinfo_ex *tpp_restrict result) {
	tpp_macro_func_lcscan_vars vars;
	tpp_assert(tpp_macro_isfunction(self));
	tpp_assert(expanded_text != self->tm_body_chunk);
	tpp_assert(expanded_text_file->tf_chunk == expanded_text);
	tpp_assert(expanded_text_file->tf_kind == TPP_FILE_KIND_MACRO);
	tpp_assert(expanded_text_file->tf_data.td_macro.tfm_macro == self);
	result->tlcix_info = TPP_LCINFO_INVALID;
	result->tlcix_projfile = NULL;

	vars.tmflcsv_pc           = self->tm_data.tmd_func.tmf_expand;
	vars.tmflcsv_expand_start = tpp_string_str(expanded_text);
	vars.tmflcsv_expand_end   = tpp_string_end(expanded_text);
	vars.tmflcsv_expand_pos   = pos;
	vars.tmflcsv_body_start   = self->tm_body_start;
	vars.tmflcsv_body_end     = self->tm_body_end;
	if (!tpp_macro_func_lcscan(&vars))
		return; /* Unable to translate pointers */

	/* Encode LC info as position within the macro's body. */
	tpp_assert(vars.tmflcsv_body_start >= self->tm_body_start);
	tpp_assert(vars.tmflcsv_body_start <= self->tm_body_end);
	result->tlcix_info = tpp_lcinfo_account_ex(self->tm_body_lc, self->tm_body_start,
	                                           (tpp_size)(vars.tmflcsv_body_start - self->tm_body_start),
	                                           self->tm_body_enc);

	/* Check if position belongs to a macro argument */
	if (vars.tmflcsv_argopcode != NULL && expanded_text_file->tf_tprev) {
		tpp_macro_opcode const argument_opcode = vars.tmflcsv_argopcode[0];
		tpp_size const argument_index          = vars.tmflcsv_argopcode[1];
		tpp_file *const invocation_file        = expanded_text_file->tf_tprev;
		tpp_size const argument_offset = tpp_macro_determine_argument_offset(argument_opcode, vars.tmflcsv_expand_start, pos);
		tpp_assert(argument_index < self->tm_data.tmd_func.tmf_argc);

		/* At this point, the state looks like this:
		 * >> #define foo(a, b) 10+a+20+b+30
		 * >> foo(11, "a\x22") END
		 *
		 * --- Assuming requesting info on the \ in "a\x22":
		 *
		 * expanded_text_file: 10+11+20+"a\x22"+30
		 *                              ^ ^    ^
		 *                              | |    vars.tmflcsv_expand_end
		 *                              | pos
		 *                              vars.tmflcsv_expand_start
		 *
		 * invocation_file:    foo(11, "a\x22") END
		 *                     ^               ^
		 *                     |               invocation_file->tf_pos
		 *                     invocation_file->tf_tpos
		 * argument_index:     1  (second argument)
		 * argument_offset:    2  (0: <">, 1: <a>, 2: <\>)
		 *
		 * Of course, there are (lots of) cases where we'll be unable
		 * to *actually* determine the origin of a projection (such as
		 * when the argument's text is in a file that has already been
		 * popped, or we're unable to determine the macro argument list:
		 * >> #define bar foo(11,
		 * >> bar "a\x22") */
		{
#if TPP_HAVE_FILE_MACRO_TRACKARGS
			tpp_lexer_arginfo const *const argv = expanded_text_file->tf_data.td_macro.tfm_args;
			tpp_lexer_arginfo const *const arg = &argv[argument_index];
			if (!arg->tlai_chunk) {
				/* No chunk -> no LC info */
			} else if (arg->tlai_chunk == invocation_file->tf_chunk) {
				/* Same chunk as invocation file! */
				if (argument_opcode == TPP_MACRO_OPCODE_INS_EXP) {
					/* Special (but sadly most likely) case: argument (may have) gotten expanded
					 * -> Because of this, we have to ensure that the argument content (at least
					 *    up to the requested position) doesn't differ between its expanded and
					 *    non-expanded forms. */
					tpp_char const *const expanded_argument_start = vars.tmflcsv_expand_start;
/*					tpp_char const *const expanded_argument_end   = vars.tmflcsv_expand_end; */
					tpp_char const *const orig_argument_start     = arg->tlai_start;
					tpp_char const *const orig_argument_end       = arg->tlai_end;
					tpp_size const orig_argument_size = (tpp_size)(orig_argument_end - orig_argument_start);
					tpp_assert(argument_offset <= (tpp_size)(vars.tmflcsv_expand_end - expanded_argument_start));

					/* XXX: It'd be great if there was some way for us to be smarter about this:
					 * >> #define MAC1(x)
					 * >> #define MAC2(x) x
					 * >> #define foo(a) 10+a+20
					 * >> foo(MAC1(3) MAC2(3))
					 *
					 * When requesting debug info about the "3" token, we're unable to satisfy
					 * the request because we have no way of knowing which "3" the caller is
					 * actually talking about
					 *
					 * The only way this could really be done is by saving LC info for every
					 * token when expanding a macro's arguments (but that's wholly overkill)
					 */
					if (argument_offset > orig_argument_size)
						return; /* Expansion did happen, and it happened *big* time (offset) */
					if (tpp_memcmp(orig_argument_start, expanded_argument_start, argument_offset) != 0)
						return;
				}

				/* Was able to determine projected (argument) code location */
				result->tlcix_projfile = invocation_file;
				result->tlcix_projpos  = arg->tlai_start + argument_offset;
				tpp_assert(arg->tlai_start >= tpp_string_str(arg->tlai_chunk));
				tpp_assert(arg->tlai_start <= arg->tlai_end);
				tpp_assert(arg->tlai_end <= tpp_string_end(arg->tlai_chunk));
				tpp_assert(result->tlcix_projpos >= arg->tlai_start);
				tpp_assert(result->tlcix_projpos <= arg->tlai_end);
			} else {
				/* XXX: LC information could still be retrieved from "tpp_lexer_arginfo",
				 *      once that "smart way of tracking debug info" has been implemented. */
			}
#else /* TPP_HAVE_FILE_MACRO_TRACKARGS */
			tpp_char const *const argument_start   = vars.tmflcsv_expand_start;
			tpp_char const *const argument_end     = vars.tmflcsv_expand_end;
			tpp_char const lparen_ch = (tpp_char)tpp_macro_getfunclparen(self);
			tpp_char const rparen_ch = (tpp_char)tpp_macro_getfuncrparen(self);
			tpp_char const *macro_args_start = invocation_file->tf_tpos;
			tpp_char const *macro_args_end   = invocation_file->tf_pos;
			tpp_size arg_iter;

			/* WARNING: This is completely imprecise: the ( and ) tokens can be in comments
			 *          or strings, in which case we can easily get the wrong ones here!
			 * -> This is why "TPP_HAVE_FILE_MACRO_TRACKARGS" needs to be enabled for
			 *    improved projection tracking */
			while (macro_args_start < macro_args_end && *macro_args_start != lparen_ch)
				++macro_args_start;
			if (macro_args_start < macro_args_end && *macro_args_start == lparen_ch)
				++macro_args_start;
			while (macro_args_end > macro_args_start && macro_args_end[-1] != rparen_ch)
				--macro_args_end;
			if (macro_args_end > macro_args_start && macro_args_end[-1] == rparen_ch)
				--macro_args_end;
			for (arg_iter = 0; arg_iter < argument_index; ++arg_iter) {
				macro_args_start = (tpp_char const *)tpp_memchr(macro_args_start, ',',
				                                                (tpp_size)(macro_args_end -
				                                                           macro_args_start));
				if (!macro_args_start)
					return;
				++macro_args_start;
			}
			if (macro_args_start < macro_args_end) {
				tpp_char const *projection_pos;
				projection_pos = (tpp_char const *)tpp_memmem(macro_args_start, (tpp_size)(macro_args_end - macro_args_start),
				                                              argument_start, (tpp_size)(argument_end - argument_start));
				if (projection_pos) {
					projection_pos += argument_offset;
					if (projection_pos >= macro_args_start && projection_pos <= macro_args_end) {
						/* This one's ~probably~ it (but there's no way to guaranty that. Even if
						 * we tried to re-parse tokens here, and ignoring the fact that we don't
						 * have direct access to the lexer, the lexer's config may have changed
						 * since the macro was called, in which case the same problems would arise
						 * once again (e.g.: (*pascal-comments*) were enabled during the initial
						 * parse, but are now disabled, meaning we might think the projected macro
						 * argument is a piece of text located within said pascal comment)) */
						result->tlcix_projfile = invocation_file;
						result->tlcix_projpos  = projection_pos;
					}
				}
			}
#endif /* !TPP_HAVE_FILE_MACRO_TRACKARGS */
		}
	}
}

#endif /* TPP_HAVE_CPP_MACROS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_MACRO_C */
