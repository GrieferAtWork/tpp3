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

TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_PAREN) == TPP_TOK_LPAREN);
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_BRACKET) == TPP_TOK_LBRACKET);
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_BRACE) == TPP_TOK_LBRACE);
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK(TPP_MACRO_KIND_FUNC_ANGLE) == TPP_TOK_LANGLE);
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

typedef struct tpp_macro_expinfo {
	tpp_char *tmei_expand_data; /* [0..tmei_expand_size][owned_if(MAYBE)]
	                             * Buffer containing the expanded argument text. */
	tpp_size  tmei_expand_size; /* Length of "tmei_expand_data" (in bytes) */
} tpp_macro_expinfo;

#define tpp_macro_expinfo_fini(self, arginfo)                      \
	((self)->tmei_expand_data != (tpp_char *)(arginfo)->tlai_start \
	 ? tpp_free((self)->tmei_expand_data)                          \
	 : (void)0)
#define tpp_macro_expinfo_getdata(self) ((self)->tmei_expand_data)
#define tpp_macro_expinfo_getsize(self) ((self)->tmei_expand_size)

/* Initialize "self" by expanding `argument_start...argument_end'
 * @return: TPP_EOK: Success
 * @return: * :      Error (must be propagated) */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_macro_expinfo_init(tpp_macro_expinfo *tpp_restrict self,
                       tpp_lexer_arginfo const *tpp_restrict arginfo,
                       tpp_lexer *tpp_restrict lexer) {
	/* Must actually expand argument data! (i.e.: when text contains
	 * "__TPP_EVAL(10+20)", then our output must contain "30")
	 *
	 * NOTES:
	 *  - This can only really be implemented using tpp_lexer_yield(),
	 *    and then appending the start/end ranges of successive tokens
	 *    to a string buffer. However, for this we need a special flag
	 *    for the lexer that will cause it to *always* emit space/lf/
	 *    comment tokens (since those must be retained in expanded
	 *    text).
	 *  - In order to force-overwrite the lexer to only yield tokens
	 *    from the macro-argument area, we can use tpp_file_pusheof()
	 *    (or rather: tpp_file_pusheof_fast, whilst ensuring that
	 *    any extra files pushed by tpp_lexer_yield() are always once
	 *    again popped; including in case of errors)
	 *  - For the sake of performance, "simple" arguments are treated
	 *    specially by not making use of a secondary heap-buffer.
	 *    "simple" here means that tpp_lexer_yield()-ing "arginfo"
	 *    produces an uninterrupted stream of tokens all originating
	 *    from the file providing argument information. Or in other
	 *    words: "simple" means that expanding the argument doesn't
	 *    change anything about it. */
	(void)lexer;
	/* TODO: New lexer state flag that force-enables emission of all tokens */
	/* TODO */
	self->tmei_expand_data = (tpp_char *)arginfo->tlai_start;
	self->tmei_expand_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
	return TPP_EOK;
}

typedef struct tpp_macro_argbuf {
	tpp_lexer_arginfo tmab_arginfo[TPP_FLEX_ARRAY]; /* [:tmf_argc] Origin argument info vector */
/*	tpp_macro_expinfo tmab_expinfo[TPP_FLEX_ARRAY];  * [:tmf_argc] Argument expansion info vector. */
} tpp_macro_argbuf;

#define tpp_macro_argbuf_sizeof(argc) \
	((argc) * (sizeof(tpp_lexer_arginfo) + sizeof(tpp_macro_expinfo)))
#define tpp_macro_argbuf_getarginfo(self, argc) \
	((self)->tmab_arginfo)
#define tpp_macro_argbuf_getexpinfo(self, argc) \
	((tpp_macro_expinfo *)((self)->tmab_arginfo + (argc)))


/* >> tpp_lexer_seek_rparen_flags_frommacro(macro)
 * Return `TPP_LEXER_SEEK_RPAREN_FLAG_*' flags needed by "macro" */
#if !TPP_HAVE_MACRO_FLAGS
#define tpp_lexer_seek_rparen_flags_frommacro(macro) TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL
#else /* !TPP_HAVE_MACRO_FLAGS */
#if defined(TPP_MACRO_FLAG_VARIADIC) && !defined(TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS)
#error "Invalid configuration: macros require varargs support, but 'TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS' isn't available"
#endif /* !TPP_MACRO_FLAG_VARIADIC && !TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS */
#if defined(TPP_MACRO_FLAG_KEEPARGSPC) && !defined(TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC)
#error "Invalid configuration: macros require keep-argument-space support, but 'TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC' isn't available"
#endif /* !TPP_MACRO_FLAG_KEEPARGSPC && !TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC */
#undef TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON
#if ((defined(TPP_MACRO_FLAG_VARIADIC) && TPP_MACRO_FLAG_VARIADIC == TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS) && \
     (defined(TPP_MACRO_FLAG_KEEPARGSPC) && TPP_MACRO_FLAG_KEEPARGSPC == TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC))
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON (TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS | TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC)
#elif defined(TPP_MACRO_FLAG_VARIADIC) && TPP_MACRO_FLAG_VARIADIC == TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS
#elif (TPP_MACRO_FLAG_KEEPARGSPC) && TPP_MACRO_FLAG_KEEPARGSPC == TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC
#endif
#ifdef TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON
#define tpp_lexer_seek_rparen_flags_frommacro(macro) ((macro)->tm_flags & TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON)
#else /* TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON */
#ifdef TPP_MACRO_FLAG_VARIADIC
#define _tpp_macro_get_seek_flag_varargs(macro) (((macro)->tm_flags & TPP_MACRO_FLAG_VARIADIC) ? TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS : 0)
#else /* TPP_MACRO_FLAG_VARIADIC */
#define _tpp_macro_get_seek_flag_varargs(macro) 0
#endif /* !TPP_MACRO_FLAG_VARIADIC */
#ifdef TPP_MACRO_FLAG_KEEPARGSPC
#define _tpp_macro_get_seek_flag_keepargspc(macro) (((macro)->tm_flags & TPP_MACRO_FLAG_KEEPARGSPC) ? TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC : 0)
#else /* TPP_MACRO_FLAG_KEEPARGSPC */
#define _tpp_macro_get_seek_flag_keepargspc(macro) 0
#endif /* !TPP_MACRO_FLAG_KEEPARGSPC */
#define tpp_lexer_seek_rparen_flags_frommacro(macro) \
	(_tpp_macro_get_seek_flag_varargs(macro) |       \
	 _tpp_macro_get_seek_flag_keepargspc(macro))
#endif /* !TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON */
#endif /* TPP_HAVE_MACRO_FLAGS */


/* Return the invocation argument buffer of "macro"
 * @return: NULL: Out of memory (TPP_ENOMEM) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_argbuf *TPPCALL
tpp_macro_require_argbuf(tpp_macro *tpp_restrict macro) {
	tpp_macro_argbuf *result = macro->tm_data.tmd_func.tmf_argbuf;
	if (result == NULL) {
		tpp_size size = tpp_macro_argbuf_sizeof(macro->tm_data.tmd_func.tmf_argc);
		result = (tpp_macro_argbuf *)tpp_malloc(size);
		macro->tm_data.tmd_func.tmf_argbuf = result;
	}
	return result;
}


#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
static tpp_ssize TPP_FORMATPRINTER_CC
tpp_count_printer(void *arg, tpp_char const *text, tpp_size num_bytes) {
	(void)arg;
	(void)text;
	return (tpp_ssize)num_bytes;
}

static tpp_ssize TPP_FORMATPRINTER_CC
tpp_buffer_printer(void *arg, tpp_char const *text, tpp_size num_bytes) {
	tpp_char **p_dst = (tpp_char **)arg;
	tpp_char *dst = *p_dst;
	tpp_memcpy(dst, text, num_bytes);
	dst += num_bytes;
	*p_dst = dst;
	return 0;
}
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */

/* Perform the expansion of a user-defined, function-style macro.
 *
 * @return: tpp_lexer_gettoken(self)->tt_id : Function-style macro cannot be expanded
 * @return: TPP_TOK_EOF: Success -- caller should yield again to load the
 *                                  first macro's first expansion token.
 * @return: TPP_TOK_ENOMEM: Out of memory */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_expand_macro_function(tpp_lexer *tpp_restrict self,
                                tpp_macro *tpp_restrict macro) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_lexer_seek_backup backup;
	tpp_char const *pos;
	tpp_token_id tok;
	tpp_size i, argc;    /* # of arguments given in-source during invocation */
	tpp_size macro_argc; /* == macro->tm_data.tmd_func.tmf_argc */
	tpp_macro_argbuf *argbuf;
	tpp_lexer_arginfo *invoke_arginfo; /* == argbuf->tmab_arginfo */
	tpp_macro_expinfo *invoke_expinfo; /* == argbuf->tmab_expinfo */
	tpp_string *result_chunk;
	tpp_file *prev_file;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	char va_nargs[TPP_UTOA_MAXLEN]; /* Value for __VA_NARGS__ */
	tpp_size va_nargs_len = 0; /* XXX: Initialization doesn't matter; only here to shut up compiler warnings */
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
	tpp_size result_chunk_size;
	tpp_assert(TPP_MACRO_KIND_ISFUNC(macro->tm_kind));

	/* Seek ahead to find the '('-token expected by the macro. */
	pos = tpp_lexer_seek_begin(self, &backup, false);
	do {
		tok = tpp_lexer_yieldraw_at(self, &pos);
	} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
	if (TPP_TOK_ISERR(tok))
		goto err_rollback;

	if (tok == TPP_MACRO_KIND_ASTOK(macro->tm_kind)) {
		/* Got the opening parenthesis token! */
	} else
#if (TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS && \
     (TPP_HAVE_TPP_TOK_LANGLE_EQUAL ||         \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE ||        \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL ||  \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE || \
      TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL))
	if (macro->tm_kind == TPP_MACRO_KIND_FUNC_ANGLE) {
		/* Check for multi-char tokens that start with '<' */
		switch (tok) {
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL
		case TPP_TOK_LANGLE_EQUAL: /* "<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE
		case TPP_TOK_LANGLE_LANGLE: /* "<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
		case TPP_TOK_LANGLE_LANGLE_EQUAL: /* "<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
		case TPP_TOK_LANGLE_LANGLE_LANGLE: /* "<<<" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
		case TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: /* "<<<=" */
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
			/* Convert to "<" token */
			tpp_assert(tpp_lexer_gettoken(self)->tt_start < pos);
			tpp_assert(tpp_lexer_gettoken(self)->tt_start[0] == '<');
			pos = tpp_lexer_gettoken(self)->tt_start + 1;
			tok = TPP_TOK_OFCHAR('<');
			tpp_lexer_gettoken(self)->tt_id = tok;
			break;
		default: goto rollback;
		}
	} else
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */
	{
		goto rollback;
	}

	/* Load argument buffer of macro */
	argbuf = tpp_macro_require_argbuf(macro);
	if tpp_unlikely(!argbuf)
		goto err_rollback_nomem;
	argc = macro_argc = macro->tm_data.tmd_func.tmf_argc;
	invoke_arginfo = tpp_macro_argbuf_getarginfo(argbuf, argc);
	invoke_expinfo = tpp_macro_argbuf_getexpinfo(argbuf, argc);

	/* Load parameters of function-style macro */
	tok = tpp_lexer_seek_rparen_ex(self, &pos, invoke_arginfo, &argc,
	                               (char const *)backup.tlsb_kwd->tk_kwd,
	                               tpp_lexer_seek_rparen_flags_frommacro(macro),
	                               TPP_MACRO_KIND_ASTOK(macro->tm_kind));
	if (TPP_TOK_ISERR(tok))
		goto err_rollback;
	tpp_assert(macro_argc == macro->tm_data.tmd_func.tmf_argc);
	if (argc < macro_argc) {
		/* Too few arguments */

		/* Check for special case: exactly 1 argument is missing, and macro takes varargs.
		 * In this case, the last argument becomes optional and must be treated as empty. */
#ifdef TPP_MACRO_FLAG_VARIADIC
		if ((macro->tm_flags & TPP_MACRO_FLAG_VARIADIC) &&
		    (argc == macro_argc - 1)) {
			/* This is OK */
		} else
#endif /* TPP_MACRO_FLAG_VARIADIC */
		{
#if TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS
			tpp_errno error;
			error = tpp_lexer_warnf_at(self, pos, TPP_W_TOO_FEW_ARGUMENTS,
			                           (char const *)backup.tlsb_kwd->tk_kwd,
			                           (unsigned int)macro_argc,
			                           (unsigned int)argc);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				goto err_rollback;
			}
		}
#endif /* TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */
		for (i = argc; i < macro_argc; ++i) {
			/* Initialize as empty arguments (actually assigned pointer
			 * here doesn't matter; so-long as "tlai_start == tlai_end",
			 * the argument is considered to be empty, and no data will
			 * be dereferenced) */
#if TPP_DEBUG
			invoke_arginfo[i].tlai_start = NULL;
			invoke_arginfo[i].tlai_end   = NULL;
#else /* TPP_DEBUG */
			invoke_arginfo[i].tlai_start = invoke_arginfo[i].tlai_end;
#endif /* !TPP_DEBUG */
		}
	}

	/* Figure out how much space is needed for the resulting string-chunk */
	result_chunk_size = macro->tm_data.tmd_func.tmf_expbase;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT
	if (argc >= macro_argc)
		result_chunk_size += macro->tm_data.tmd_func.tmf_n_vaopt;
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VAOPT */
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	if (macro->tm_data.tmd_func.tmf_n_vanargs) {
		char *endp;
		tpp_size va_nargs_value = 0;
		if (argc >= macro_argc)
			va_nargs_value = (argc - macro_argc) + 1;
		endp = tpp_utoa(va_nargs, va_nargs_value);
		va_nargs_len = (tpp_size)((va_nargs + tpp_lengthof(va_nargs)) - endp);
		tpp_memmovedown(va_nargs, endp, va_nargs_len);
		result_chunk_size += va_nargs_len * macro->tm_data.tmd_func.tmf_n_vanargs;
	}
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/

	/* Account for extra space needed by inserted arguments.
	 * This is also the part where arguments are recursively
	 * expanded */
	for (i = 0; i < macro_argc; ++i) {
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
		if (arg->tma_ins_exp) {
			tpp_errno error;
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			error = tpp_macro_expinfo_init(expand, arginfo, self);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				goto err_rollback_invoke_expinfo_i;
			}

			/* Account for expanded text */
			result_chunk_size += (arg->tma_ins_exp * tpp_macro_expinfo_getsize(expand));
		}

#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
		if (arg->tma_ins_str) {
			tpp_size raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
			tpp_size str_size = (tpp_size)tpp_token_encodestring(&tpp_count_printer, NULL,
			                                                     arginfo->tlai_start, raw_size);
/*			str_size += 2; * Account for leading/trailing " or ' characters -- Already account for during compilation */
			result_chunk_size += (arg->tma_ins_str * str_size);
		}
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT */
#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
		if (arg->tma_ins != 0) {
			tpp_size raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
			result_chunk_size += (arg->tma_ins * raw_size);
		}
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */
	}

	/* Allocate the perfectly-sized chunk that will describe the expanded macro's text */
	result_chunk = tpp_string_malloc(result_chunk_size);
	if tpp_unlikely(!result_chunk)
		goto err_rollback_invoke_expinfo_nomem;

	/* Produce body-chunk-string for function-style macro expansion */
	{
		tpp_char *dst = tpp_string_str(result_chunk);
		tpp_char const *src = macro->tm_body_start;
		tpp_macro_opcode const *pc = macro->tm_data.tmd_func.tmf_expand;
		tpp_macro_opcode opcode;
next_op:
		opcode = *pc++;
		switch (opcode) {

		case TPP_MACRO_OPCODE_END:
			break;

		case TPP_MACRO_OPCODE_SKIP:
			src += *pc++;
			goto next_op;

		case TPP_MACRO_OPCODE_COPY: {
			tpp_size bytes = *pc++;
			tpp_memcpy(dst, src, bytes);
			src += bytes;
			dst += bytes;
			goto next_op;
		}

		case TPP_MACRO_OPCODE_INS_EXP: {
			tpp_size argi = *pc++;
			tpp_macro_expinfo const *expinfo = &invoke_expinfo[argi];
			tpp_assert(argi < macro_argc);
			tpp_assert(macro->tm_data.tmd_func.tmf_argv[argi].tma_ins_exp != 0);
			tpp_memcpy(dst,
			           tpp_macro_expinfo_getdata(expinfo),
			           tpp_macro_expinfo_getsize(expinfo));
			dst += tpp_macro_expinfo_getsize(expinfo);
			src += *pc++;
			goto next_op;
		}

#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT
		case TPP_MACRO_OPCODE_INS_STR: {
			/* +2  Insert argument[ARG[0]] ("-escaped) and advance macro body template reader by ARG[1] bytes */
			tpp_size argi = *pc++;
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[argi];
			tpp_size raw_size;
			tpp_assert(argi < macro_argc);
			tpp_assert(macro->tm_data.tmd_func.tmf_argv[argi].tma_ins_str != 0);
			*dst++ = '"';
			raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
			tpp_token_encodestring(&tpp_buffer_printer, &dst, arginfo->tlai_start, raw_size);
			*dst++ = '"';
			src += *pc++;
			goto next_op;
		}
#endif /* TPP_HAVE_STRINGIZE_MACRO_ARGUMENT */

#if TPP_HAVE_CHARIZE_MACRO_ARGUMENT
		case TPP_MACRO_OPCODE_INS_CHR: {
			/* +2  Insert argument[ARG[0]] ("-escaped) and advance macro body template reader by ARG[1] bytes */
			tpp_size argi = *pc++;
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[argi];
			tpp_size raw_size;
			tpp_assert(argi < macro_argc);
			tpp_assert(macro->tm_data.tmd_func.tmf_argv[argi].tma_ins_str != 0);
			*dst++ = '\'';
			raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
			tpp_token_encodestring(&tpp_buffer_printer, &dst, arginfo->tlai_start, raw_size);
			*dst++ = '\'';
			src += *pc++;
			goto next_op;
		}
#endif /* TPP_HAVE_CHARIZE_MACRO_ARGUMENT */

#if TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT
		case TPP_MACRO_OPCODE_INS: {
			tpp_size argi = *pc++;
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[argi];
			tpp_size raw_size;
			tpp_assert(argi < macro_argc);
			tpp_assert(macro->tm_data.tmd_func.tmf_argv[argi].tma_ins != 0);
			raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
			tpp_memcpy(dst, arginfo->tlai_start, raw_size);
			dst += raw_size;
			src += *pc++;
			goto next_op;
		}
#endif /* TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT || TPP_HAVE_GLUE_MACRO_ARGUMENT */

#if TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS
		case TPP_MACRO_OPCODE_VA_COMMA: {
			src += *pc++;
			if (argc >= macro_argc)
				*dst++ = ',';
			goto next_op;
		}
#endif /* TPP_HAVE_VA_COMMA_IN_MACROS || TPP_HAVE_VA_GLUE_COMMA_IN_MACROS */

#if TPP_HAVE_VA_OPT_IN_MACROS
		case TPP_MACRO_OPCODE_VA_OPT: {
			tpp_size bytes;
			src += *pc++;
			bytes = *pc++;
			if (argc >= macro_argc) {
				tpp_memcpy(dst, src, bytes);
				dst += bytes;
			}
			src += bytes;
			src += *pc++;
			goto next_op;
		}
#endif /* TPP_HAVE_VA_OPT_IN_MACROS */

#if TPP_HAVE_VA_NARGS_IN_MACROS
		case TPP_MACRO_OPCODE_VA_NARGS: {
			src += *pc++;

			/* NOTE: If you leave "va_nargs_len" uninitialized, this right here would
			 *       be marked as a use of an uninitialized variable by the compiler.
			 * That is technically true, but:
			 * - The "TPP_MACRO_OPCODE_VA_NARGS" opcode may only be emitted if
			 *   the macro compiler sets "tm_data.tmd_func.tmf_n_vanargs != 0"
			 * - When "tm_data.tmd_func.tmf_n_vanargs != 0", then "va_nargs_len"
			 *   gets initialized to its correct value above! */
			tpp_memcpy(dst, va_nargs, va_nargs_len);
			dst += va_nargs_len;
			goto next_op;
		}
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */

		default: tpp_unreachable();
		}
		tpp_assert((dst == tpp_string_end(result_chunk)) &&
		           "This failing means that argument specs and macro opcodes have diverged ("
		           "this shouldn't happen and indicates a bug in `tpp_macro_builder_compile()')");
		tpp_assert((src <= macro->tm_body_end) &&
		           "Similar to 'dst'-assertion, but source-body pointer went beyond end of "
		           "macro definition body (bug in `tpp_macro_builder_compile()')");
	}

	/* Cleanup temporary expansion buffers. */
	for (i = 0; i < macro_argc; ++i) {
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		if (arg->tma_ins_exp) {
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
			tpp_macro_expinfo_fini(expand, arginfo);
		}
	}

#if TPP_HAVE_MACRO_RECURSION
	if (macro->tm_expansions > 0) {
		/* Check if an identical body-chunk-string already exists
		 * somewhere on the tf_tprev-based #include-stack.
		 * -> If a duplicate expansion was encountered, rollback & don't expand */
		tpp_file *iter = file;
		do {
			if (iter->tf_kind == TPP_FILE_KIND_MACRO &&
			    iter->tf_data.td_macro.tfm_macro == macro) {
				tpp_string const *existing_chunk = iter->tf_chunk;
				if (tpp_string_equals(existing_chunk, result_chunk)) {
					/* Duplicate chunk!!! -> Mustn't expand (else: would result in infinite loop) */
					tpp_string_destroy(result_chunk);
					goto rollback;
				}
			}
		} while ((iter = iter->tf_tprev) != NULL);
	}
#endif /* TPP_HAVE_MACRO_RECURSION */

	/* Set-up "result_chunk" such that it will read from "body-chunk-string" */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_rollback_result_chunk_nomem;
	*prev_file = *file;
	prev_file->tf_pos = pos; /* Override return-file to continue parsing after ')'-token */
	file->tf_pos   = tpp_string_str(result_chunk);
	file->tf_chunk = result_chunk; /* Inherit reference */
	file->tf_end   = tpp_string_end(result_chunk);
	_tpp_file_init_common(file);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	file->tf_kind  = TPP_FILE_KIND_MACRO;
#if TPP_HAVE_UNICODE
	file->tf_enc = macro->tm_body_enc;
#endif /* TPP_HAVE_UNICODE */
	file->tf_data.td_macro.tfm_macro = macro;
	tpp_macro_incref(macro);
	++macro->tm_expansions;
	return TPP_TOK_EOF;
rollback:
	return tpp_lexer_seek_rollback(self, &backup);
err_rollback_result_chunk_nomem:
	tok = TPP_TOK_ENOMEM;
/*err_rollback_result_chunk:*/
	tpp_string_destroy(result_chunk);
	goto err_rollback;
err_rollback_invoke_expinfo_nomem:
	tok = TPP_TOK_ENOMEM;
/*err_rollback_invoke_expinfo:*/
	i = macro_argc;
err_rollback_invoke_expinfo_i:
	while (i) {
		--i;
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		if (arg->tma_ins_exp) {
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
			tpp_macro_expinfo_fini(expand, arginfo);
		}
	}
err_rollback:
	tpp_lexer_seek_rollback(self, &backup);
	return tok;
err_rollback_nomem:
	tok = TPP_TOK_ENOMEM;
	goto err_rollback;
}

/* Perform the expansion of a user-defined "macro", with the lexer's
 * current token set to point at the macro's identifier (meaning that
 * you have to seek ahead in order to find the opening '(' token in
 * case of a function-style macro).
 *
 * @return: tpp_lexer_gettoken(self)->tt_id : Function-style macro cannot be expanded
 * @return: TPP_TOK_EOF: Success -- caller should yield again to load the
 *                                  first macro's first expansion token.
 * @return: TPP_TOK_ENOMEM: Out of memory */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_expand_macro(tpp_lexer *tpp_restrict self,
                       tpp_macro *tpp_restrict macro) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file;
	if (TPP_MACRO_KIND_ISFUNC(macro->tm_kind))
		return tpp_lexer_expand_macro_function(self, macro);

	/* Simple case: keyword-style macro */

	/* Allocate a new file that will be used to backup the current input file. */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem;
	*prev_file = *file;
	file->tf_pos   = macro->tm_body_start;
	file->tf_chunk = macro->tm_body_chunk;
	file->tf_end   = macro->tm_body_end;
	if (file->tf_chunk)
		tpp_string_incref(file->tf_chunk);

	_tpp_file_init_common(file);
	file->tf_prev  = prev_file;
	file->tf_tprev = prev_file;
	file->tf_kind  = TPP_FILE_KIND_MACRO;
#if TPP_HAVE_UNICODE
	file->tf_enc = macro->tm_body_enc;
#endif /* TPP_HAVE_UNICODE */
	file->tf_data.td_macro.tfm_macro = macro;
	tpp_macro_incref(macro);
	++macro->tm_expansions;

	(void)macro;
	return TPP_TOK_EOF;
err_nomem:
	return TPP_TOK_ENOMEM;
}

static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_push_textfile(tpp_lexer *tpp_restrict self,
                        tpp_char const *text, tpp_size textsize) {
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem;
	*prev_file = *file;

	file->tf_pos   = text;
	file->tf_chunk = NULL;
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
	return TPP_TOK_ENOMEM;
}

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
	 TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS)

#if TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO

#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
struct tpp_lexer_handle_string_feature_test_data {
	tpp_lexer   *tlhsftd_lexer;        /* [1..1] Lexer */
	tpp_token_id tlhsftd_mode;         /* Feature-test mode */
	tpp_char     tlhsftd_expansion[1]; /* Desired expansion */
};

static tpp_errno TPPCALL
tpp_lexer_handle_string_feature_test_cb(void *arg, tpp_char const *str, tpp_size length) {
	struct tpp_lexer_handle_string_feature_test_data *data;
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
	tpp_char const *pos = tpp_lexer_seek_begin(self, &backup, false);
	tpp_token_id tok;
	unsigned int recursion;
#if TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS
	struct tpp_lexer_handle_string_feature_test_data data;
#define tpp_feature_test_macro_expansion data.tlhsftd_expansion
#else /* TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
	tpp_char expansion[1];
#define tpp_feature_test_macro_expansion expansion
#endif /* !TPP_HAVE_STRING_FEATURE_FLAG_TEST_MACROS */
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
#if TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS
		/* Deal with keyword-style feature tests... */
		if (TPP_TOK_ISKEYWORD(tok)) {
			tpp_keyword const *feature_keyword;
			tpp_keyword_flags mask, flags;

			/* Load keyword flags. */
			feature_keyword = tpp_lexer_gettoken(self)->tt_kwd;
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
#if TPP_HAVE_MACRO___is_identifier
			case TPP_KWD___is_identifier:
				/* Something is considered to be an "identifier" if it's not a builtin keyword. */
				if (!TPP_TOK_ISBUILTINKEYWORD(feature_keyword->tk_id))
					tpp_feature_test_macro_expansion[0] = '1';
				goto after_expansion_mode_assignment;
#endif /* TPP_HAVE_MACRO___is_identifier */
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
#if TPP_HAVE_MACRO___is_identifier
after_expansion_mode_assignment:
#endif /* TPP_HAVE_MACRO___is_identifier */
			do {
				tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
				if tpp_unlikely(TPP_TOK_ISERR(tok))
					goto err_tok;
			} while (TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok));
		}
#endif /* TPP_HAVE_KEYWORD_FEATURE_FLAG_TEST_MACROS */
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
		if (tok == TPP_TOK_EOF)
			goto rollback;
		tok = tpp_lexer_yieldraw_at_blocking(self, &pos);
		if tpp_unlikely(TPP_TOK_ISERR(tok))
			goto err_tok;
	}
	tpp_lexer_seek_commit(self, pos);
	return tpp_lexer_push_textfile(self, tpp_feature_test_macro_expansion, 1);
rollback:
	tok = backup.tlsb_id;
err_tok:
	tpp_lexer_seek_rollback(self, &backup);
	return tok;
#undef tpp_feature_test_macro_expansion
}
#endif /* TPP_HAVE_TPP_LEXER_HANDLE_FEATURE_TEST_MACRO */

#endif /* TPP_HAVE_CPP_MACROS */

/* Handle a keyword-style macro.
 * @return: TPP_TOK_EOF: Caller should yield again.
 * @return: * : The new expansion token after keywords were handled */
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_token_id TPPCALL
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
	{
		/* TODO */
	}	break;
#endif /* TPP_HAVE_MACRO___LINE__ || TPP_HAVE_MACRO___COLUMN__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___TIME__ || TPP_HAVE_MACRO___DATE__ || TPP_HAVE_MACRO___TIMESTAMP__
#if TPP_HAVE_MACRO___TIME__
	case TPP_KWD___TIME__:
#endif /* TPP_HAVE_MACRO___TIME__ */
#if TPP_HAVE_MACRO___DATE__
	case TPP_KWD___DATE__:
#endif /* TPP_HAVE_MACRO___DATE__ */
#if TPP_HAVE_MACRO___TIMESTAMP__
	case TPP_KWD___TIMESTAMP__:
#endif /* TPP_HAVE_MACRO___TIMESTAMP__ */
	{
		/* TODO: -Wdate-time (disabled by default) */
		/* TODO */
	}	break;
#endif /* TPP_HAVE_MACRO___TIME__ || TPP_HAVE_MACRO___DATE__ || TPP_HAVE_MACRO___TIMESTAMP__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__ || TPP_HAVE_MACRO___INCLUDE_DEPTH__
#if TPP_HAVE_MACRO___INCLUDE_LEVEL__
	case TPP_KWD___INCLUDE_LEVEL__:
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ */
#if TPP_HAVE_MACRO___INCLUDE_DEPTH__
	case TPP_KWD___INCLUDE_DEPTH__:
#endif /* TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
	{
		/* TODO */
	}	break;
#endif /* TPP_HAVE_MACRO___INCLUDE_LEVEL__ || TPP_HAVE_MACRO___INCLUDE_DEPTH__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_MACRO___COUNTER__
	case TPP_KWD___COUNTER__:
		/* TODO */
		break;
#endif /* TPP_HAVE_MACRO___COUNTER__ */
/************************************************************************/



/************************************************************************/
#if TPP_HAVE_NUMERIC_DATE_MACROS
	/* TODO: __DATE_DAY__, __DATE_WDAY__, __DATE_YDAY__, __DATE_MONTH__, __DATE_YEAR__ */
	/* TODO: -Wdate-time (disabled by default) */
#endif /* !TPP_HAVE_NUMERIC_DATE_MACROS */
#if TPP_HAVE_NUMERIC_TIME_MACROS
	/* TODO: __TIME_SEC__, __TIME_MIN__, __TIME_HOUR__ */
	/* TODO: -Wdate-time (disabled by default) */
#endif /* !TPP_HAVE_NUMERIC_TIME_MACROS */
#if TPP_HAVE_MACRO___TPP_EVAL
	/* TODO: __TPP_EVAL */
#endif /* !TPP_HAVE_MACRO___TPP_EVAL */
#if TPP_HAVE_MACRO___TPP_UNIQUE
	/* TODO: __TPP_UNIQUE */
#endif /* !TPP_HAVE_MACRO___TPP_UNIQUE */
#if TPP_HAVE_MACRO___TPP_LOAD_FILE
	/* TODO: __TPP_LOAD_FILE */
#endif /* !TPP_HAVE_MACRO___TPP_LOAD_FILE */
#if TPP_HAVE_MACRO___TPP_COUNTER
	/* TODO: __TPP_COUNTER */
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */
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
			return tpp_lexer_push_textfile(self, builtin_macro->tbm_body,
			                               builtin_macro->tbm_body_size);
		}
	}	break;

	}

	/* Fallback: act as though the macro takes no arguments, and expands to itself:
	 * >> #define SOME_MACRO SOME_MACRO */
#endif /* TPP_HAVE_CPP_MACROS */
	return tok;
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
