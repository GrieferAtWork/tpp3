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
#ifndef GUARD_TPP_MACRO_H
#define GUARD_TPP_MACRO_H 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "string.h"
#include "token.h"
#include "file.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS

#define tpp_macro_kind uint_least8_t
#define TPP_MACRO_KIND_KEYWORD      UINT8_C(0)
#define TPP_MACRO_KIND_FUNC_PAREN   '('
#define TPP_MACRO_KIND_ISFUNC(kind) ((kind) != TPP_MACRO_KIND_KEYWORD)
#define TPP_MACRO_KIND_ASTOK(kind)  ((tpp_token_id)(kind))
#define TPP_MACRO_KIND_OFTOK(tok)   ((tpp_macro_kind)(unsigned int)(tok))
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
#define TPP_MACRO_KIND_FUNC_BRACKET '['
#define TPP_MACRO_KIND_FUNC_BRACE   '{'
#define TPP_MACRO_KIND_FUNC_ANGLE   '<'
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

#undef TPP_HAVE_MACRO_FLAGS
#if (TPP_HAVE_NAMED_VARARGS_IN_MACROS || \
     TPP_HAVE_VA_ARGS_IN_MACROS ||       \
     TPP_HAVE_MACRO_RECURSION ||         \
     TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
#define TPP_HAVE_MACRO_FLAGS 1
#else /* ... */
#define TPP_HAVE_MACRO_FLAGS 0
#endif /* !... */

#undef TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
#undef TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
#if (TPP_HAVE_VA_COMMA_IN_MACROS ||      \
     TPP_HAVE_VA_GLUE_COMMA_IN_MACROS || \
     TPP_HAVE_VA_OPT_IN_MACROS)
#define TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT 1
#else /* ... */
#define TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT 0
#endif /* !... */
#if TPP_HAVE_VA_NARGS_IN_MACROS
#define TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS 1
#else /* ...*/
#define TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS 0
#endif /* !...*/



#if TPP_HAVE_MACRO_FLAGS
#define tpp_macro_flag uint_least8_t /* Set of `TPP_MACRO_FLAG_*' */
#define TPP_MACRO_FLAG_NORMAL     UINT8_C(0x00) /* Normal flags */
#if TPP_HAVE_NAMED_VARARGS_IN_MACROS || TPP_HAVE_VA_ARGS_IN_MACROS
#define TPP_MACRO_FLAG_VARIADIC   UINT8_C(0x01) /* The last argument of the function is variadic. */
#endif /* TPP_HAVE_NAMED_VARARGS_IN_MACROS || TPP_HAVE_VA_ARGS_IN_MACROS */
#if TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0
#define TPP_MACRO_FLAG_KEEPARGSPC UINT8_C(0x02) /* When set, keep whitespace surrounding macro arguments during invocation.
                                                 * WARNING: Also affects recursive macro expansion. */
#endif /* TPP_HAVE_MACRO_ARGUMENT_WHITESPACE < 0 */
#if TPP_HAVE_MACRO_RECURSION < 0
#define TPP_MACRO_FLAG_SELFEXPAND UINT8_C(0x04) /* After being expanded, this function is allowed to re-invoke itself and be expanded, when
                                                 * the generated text is not identical to a previous iteration. (s.a.: `-fmacro-recursion') */
#endif /* TPP_HAVE_MACRO_RECURSION < 0 */
#endif /* TPP_HAVE_MACRO_FLAGS */


typedef struct tpp_macro_argument {
	tpp_token_id    tma_id;      /* [const] (Keyword) token id associated with this argument name. */
	tpp_size        tma_ins_exp; /* [const] Amount of times the argument is inserted after expansion. */
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
	tpp_size        tma_ins_str; /* [const] Amount of times the argument is inserted in its string-escaped form. */
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
	tpp_size        tma_ins;     /* [const] Amount of times the argument is inserted without expansion. */
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT */
#if TPP_DEBUG
	tpp_char const *tma_name;    /* [1..1][const] Name of this macro (aliases `struct tpp_keyword::tk_kwd'). */
#endif /* TPP_DEBUG */
} tpp_macro_argument;


/* Opcodes for function-tyle macro expansion */
#define tpp_macro_opcode tpp_size
#if TPP_BUILDING
enum {
	TPP_MACRO_OPCODE_END,      /* +0  Expansion has finished */
	TPP_MACRO_OPCODE_SKIP,     /* +1  Advance macro body template reader by ARG[0] bytes */
	TPP_MACRO_OPCODE_COPY,     /* +1  Copy ARG[0] bytes from macro body template & advance reader */
	TPP_MACRO_OPCODE_INS_EXP,  /* +2  Insert argument[ARG[0]] (expanded) and advance macro body template reader by ARG[1] bytes */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT
	TPP_MACRO_OPCODE_INS,      /* +2  Insert argument[ARG[0]] (non-expanded) and advance macro body template reader by ARG[1] bytes */
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT */
#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
	TPP_MACRO_OPCODE_INS_STR,  /* +2  Insert argument[ARG[0]] ("-escaped) and advance macro body template reader by ARG[1] bytes */
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */
#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT
	TPP_MACRO_OPCODE_INS_CHR,  /* +2  Insert argument[ARG[0]] ('-escaped) and advance macro body template reader by ARG[1] bytes */
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
	TPP_MACRO_OPCODE_VA_COMMA, /* +1  TPP_MACRO_OPCODE_SKIP[ARG[0]]; If varargs are non-empty: insert a ','-character */
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */
#if TPP_HAVE_VA_OPT_IN_MACROS
	TPP_MACRO_OPCODE_VA_OPT,   /* +3  TPP_MACRO_OPCODE_SKIP[ARG[0]]; If varargs are non-empty: TPP_MACRO_OPCODE_COPY[ARG[1]]; TPP_MACRO_OPCODE_SKIP[ARG[2]] */
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */
};
#endif /* TPP_BUILDING */

struct tpp_keyword;
typedef struct tpp_macro {
	tpp_refcnt          tm_refcnt;     /* Reference count */
	tpp_macro_kind      tm_kind;       /* [const] Macro kind (one of `TPP_MACRO_KIND_*') */
#if TPP_HAVE_MACRO_FLAGS
	tpp_macro_flag      tm_flags;      /* [const] Macro flags (set of `TPP_MACRO_FLAG_*') */
#endif /* TPP_HAVE_MACRO_FLAGS */
	TPP_REF tpp_string *tm_body_chunk; /* [0..1][const] Data-chunk containing the macro's body (or "NULL" if static or originating from file with "tf_chunk == NULL") */
	tpp_char const     *tm_body_start; /* [1..1][const] Pointer to start of macro body */
	tpp_char const     *tm_body_end;   /* [1..1][const] Pointer to end of macro body */
#if TPP_HAVE_UNICODE
	tpp_file_encoding   tm_body_enc;   /* Encoding of body. */
#endif /* TPP_HAVE_UNICODE */
	tpp_size            tm_expansions; /* The amount of existing expansions of this macro.
	                                    * NOTE: Depending on the `TPP_MACRO_FLAG_SELFEXPAND' flag,
	                                    *       this value may not be allowed to exceed one(1). */
	char const         *tm_deffile;    /* [0..1][const] File in which this macro was defined (or "NULL" if unknown / custom definition) */
	tpp_lcinfo          tm_deflc;      /* [const][valid_if(tm_deffile != NULL)] Macro name line/column (0-based) */
	tpp_lcinfo          tm_body_lc;    /* [const][valid_if(tm_deffile != NULL)] Macro body line/column (0-based) */
	union {
		struct {
			tpp_size            tmf_argc;       /* [const] Amount of arguments this macro-function takes */
			tpp_macro_argument *tmf_argv;       /* [const][0..f_argc][owned] Vector of argument information (used for fast calculation of the expanded macro's size) */
			tpp_size            tmf_skiptotal;  /* [const] Max amount of bytes skipped during expansion. */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
			tpp_size            tmf_n_vaopt;    /* [const] Amount of extra bytes inserted when varargs are given (if: tpp_lexer_seek_rparen:OUT(*p_argc) > tmf_argc). */
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
			tpp_size            tmf_n_vanargs;  /* [const] Amount of times `__VA_NARGS__' is used in `tmf_expand'. */
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
			void               *tmf_argbuf;     /* [0..1][owned] Internal preallocated cache for a required temporary buffer used during expansion.
			                                     * NOTE: Implementation-wise, this is a vector of `argcache_t' (an internal, hidden data structure). */
			tpp_macro_opcode    tmf_expand[TPP_FLEX_ARRAY]; /* [const][1..1] Sequence of `TPP_MACRO_OPCODE_*'-opcodes, together with their operands */
		} tmd_func; /* [TPP_MACRO_KIND_ISFUNC(tm_kind)] */
	} tm_data;
} tpp_macro;

#define tpp_macro_malloc_keyword() \
	((tpp_macro *)tpp_malloc(tpp_offsetof(tpp_macro, tm_data)))
#define tpp_macro_malloc_function(expand_count)                                     \
	((tpp_macro *)tpp_malloc(tpp_offsetof(tpp_macro, tm_data.tmd_func.tmf_expand) + \
	                         ((expand_count) * sizeof(tpp_macro_opcode))))
#define tpp_macro_trymalloc_function(expand_count)                                     \
	((tpp_macro *)tpp_trymalloc(tpp_offsetof(tpp_macro, tm_data.tmd_func.tmf_expand) + \
	                            ((expand_count) * sizeof(tpp_macro_opcode))))
#define tpp_macro_realloc_function(p, expand_count)                                     \
	((tpp_macro *)tpp_realloc(p, tpp_offsetof(tpp_macro, tm_data.tmd_func.tmf_expand) + \
	                             ((expand_count) * sizeof(tpp_macro_opcode))))
#define tpp_macro_tryrealloc_function(p, expand_count)                                     \
	((tpp_macro *)tpp_tryrealloc(p, tpp_offsetof(tpp_macro, tm_data.tmd_func.tmf_expand) + \
	                                ((expand_count) * sizeof(tpp_macro_opcode))))
#define tpp_macro_free(p) tpp_free(p)

TPP_DECL TPP_NONNULL((1)) void TPPCALL tpp_macro_destroy(tpp_macro *tpp_restrict self);
#define tpp_macro_isshared(self) tpp_refcnt_isshared(&(self)->tm_refcnt)
#define tpp_macro_incref(self)   tpp_refcnt_inc(&(self)->tm_refcnt)
#define tpp_macro_decref(self)   (void)(tpp_refcnt_decfetch(&(self)->tm_refcnt) || (tpp_macro_destroy(self), 0))

#if TPP_HAVE_MACRO_EQUALS
/* Compare 2 macro definitions to see if they are identical. */
TPP_DECL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_macro_equals(tpp_macro const *lhs, tpp_macro const *rhs);
#endif /* TPP_HAVE_MACRO_EQUALS */

#if TPP_BUILDING
/* Figure out the line/column of "pos" in "expanded_text", as produced
 * by "self", which must be "TPP_MACRO_KIND_ISFUNC(self->tm_kind)". */
TPP_INTERN_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_lcinfo TPPCALL
tpp_macro_func_lcinfo(tpp_macro const *tpp_restrict self,
                      tpp_string const *expanded_text,
                      tpp_char const *pos);
#endif /* TPP_BUILDING */


typedef struct tpp_builtin_macro {
	tpp_size tbm_body_size;            /* Length of "tbm_body" (in characters; excluding trailing NUL) */
	tpp_char tbm_body[TPP_FLEX_ARRAY]; /* [tbm_body_size] Body text (followed by a trailing NUL) */
} tpp_builtin_macro;

#define TPP_BUILTIN_MACRO_DEFINE(name, value)                      \
	struct name##_struct {                                         \
		tpp_size tbm_body_size;                                    \
		char tbm_body[sizeof(value) / sizeof(char)];               \
	} const name = {                                               \
		/* .tbm_body_size = */ (sizeof(value) / sizeof(char)) - 1, \
		/* .tbm_body      = */ value,                              \
	}

/* Return the hard-coded expansion of the builtin macro linked to "id".
 * If "id" isn't a builtin keyword, or that keyword doesn't specify a
 * value for "TPP_BUILTIN_MACRO()", return "NULL" instead. */
TPP_DECL TPP_CONSTCALL TPP_WUNUSED tpp_builtin_macro const *TPPCALL
tpp_macro_getbuiltin(tpp_token_id id);

#endif /* TPP_HAVE_CPP_MACROS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_MACRO_H */
