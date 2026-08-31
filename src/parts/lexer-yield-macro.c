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
#ifndef GUARD_TPP_LEXER_YIELD_MACRO_C
#define GUARD_TPP_LEXER_YIELD_MACRO_C 1
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

TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK_LPAREN(TPP_MACRO_KIND_FUNC_PAREN) == TPP_TOK_LPAREN);
#if TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK_LPAREN(TPP_MACRO_KIND_FUNC_BRACKET) == TPP_TOK_LBRACKET);
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK_LPAREN(TPP_MACRO_KIND_FUNC_BRACE) == TPP_TOK_LBRACE);
TPP_STATIC_ASSERT(TPP_MACRO_KIND_ASTOK_LPAREN(TPP_MACRO_KIND_FUNC_ANGLE) == TPP_TOK_LANGLE);
#endif /* TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS */

typedef struct tpp_string_buffer {
	tpp_size  tsb_size;  /* Used buffer size */
	tpp_size  tsb_alloc; /* Allocated buffer size */
	tpp_char *tsb_data;  /* [0..1] String buffer */
} tpp_string_buffer;

#define tpp_string_buffer_init(self) \
	(void)((self)->tsb_size  = 0,    \
	       (self)->tsb_alloc = 0,    \
	       (self)->tsb_data  = NULL)
#define tpp_string_buffer_fini(self) \
	(tpp_free((self)->tsb_data),     \
	 tpp_dbg_memset(self, sizeof(tpp_string_buffer)))

#ifndef TPP_MACRO_ARGUMENT_BUFFER_MINSIZE
#define TPP_MACRO_ARGUMENT_BUFFER_MINSIZE 64
#endif /* !TPP_MACRO_ARGUMENT_BUFFER_MINSIZE */

static TPP_WUNUSED TPP_NONNULL((1)) bool TPPCALL
tpp_string_buffer_append(tpp_string_buffer *tpp_restrict self,
                         tpp_char const *data, tpp_size num_bytes) {
	tpp_size min_alloc = self->tsb_size + num_bytes;
	tpp_assert(self->tsb_size <= self->tsb_alloc);
	if (min_alloc > self->tsb_alloc) {
		tpp_char *new_buffer;
		tpp_size new_alloc = self->tsb_alloc * 2;
#if TPP_MACRO_ARGUMENT_BUFFER_MINSIZE > 1
		if (new_alloc < TPP_MACRO_ARGUMENT_BUFFER_MINSIZE)
			new_alloc = TPP_MACRO_ARGUMENT_BUFFER_MINSIZE;
#endif /* TPP_MACRO_ARGUMENT_BUFFER_MINSIZE > 1 */
		if (new_alloc < min_alloc)
			new_alloc = min_alloc;
		new_buffer = (tpp_char *)tpp_tryrealloc(self->tsb_data, new_alloc);
		if tpp_unlikely(!new_buffer) {
			new_alloc = min_alloc;
			new_buffer = (tpp_char *)tpp_realloc(self->tsb_data, new_alloc);
			if tpp_unlikely(!new_buffer)
				return false;
		}
		self->tsb_data  = new_buffer;
		self->tsb_alloc = min_alloc;
	}
	tpp_assert(min_alloc <= self->tsb_alloc);
	(void)tpp_memcpy(self->tsb_data + self->tsb_size, data, num_bytes);
	self->tsb_size += num_bytes;
	return true;
}

#ifdef __OPTIMIZE_SIZE__
#define tpp_string_buffer_truncate(self) (void)0
#else /* __OPTIMIZE_SIZE__ */
static TPP_NONNULL((1)) void TPPCALL
tpp_string_buffer_truncate(tpp_string_buffer *tpp_restrict self) {
	tpp_assert(self->tsb_size <= self->tsb_alloc);
	if (self->tsb_size < self->tsb_alloc) {
		tpp_char *newbuf = (tpp_char *)tpp_tryrealloc(self->tsb_data, self->tsb_size);
		if tpp_likely(newbuf)
			self->tsb_data = newbuf;
	}
}
#endif /* !__OPTIMIZE_SIZE__ */

typedef struct tpp_macro_expinfo {
	tpp_char *tmei_expand_data; /* [0..tmei_expand_size][owned_if(MAYBE)]
	                             * Buffer containing the expanded argument text. */
	tpp_size  tmei_expand_size; /* Length of `tmei_expand_data` (in bytes) */
} tpp_macro_expinfo;

#define tpp_macro_expinfo_fini(self, arginfo)                      \
	((self)->tmei_expand_data != (tpp_char *)(arginfo)->tlai_start \
	 ? tpp_free((self)->tmei_expand_data)                          \
	 : (void)0)
#define tpp_macro_expinfo_getdata(self) ((self)->tmei_expand_data)
#define tpp_macro_expinfo_getsize(self) ((self)->tmei_expand_size)

/* Initialize `self` by expanding `argument_start...argument_end`
 * @return: TPP_EOK: Success
 * @return: * :      Error (must be propagated) */
static TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_errno TPPCALL
tpp_macro_expinfo_init(tpp_macro_expinfo *tpp_restrict self,
                       tpp_lexer_arginfo const *tpp_restrict arginfo,
                       tpp_lexer *tpp_restrict lexer,
                       tpp_macro const *tpp_restrict macro) {
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
	 *    change anything about it.
	 *
	 * HINT:
	 * - Our caller has set-up a context as follows:
	 *   >> tpp_file_subtext_push(file);
	 *   >> tpp_lexer_alltokens_pushon(lexer);
	 */
	tpp_token const *const token = tpp_lexer_gettoken(lexer);
	tpp_string_buffer buffer;
	tpp_token_id tok;
#if TPP_HAVE_MAGIC_WHITESPACE
	tpp_token_id prev_tok = TPP_TOK_SPACE;
#endif /* TPP_HAVE_MAGIC_WHITESPACE */
	tpp_char const *expected_simple_tok_start;
	tpp_assert(tpp_lexer_getfile(lexer)->tf_prev == NULL);
	expected_simple_tok_start = arginfo->tlai_start;
	(void)macro;

next_tok:
	tok = tpp_lexer_yield(lexer); /* Caller has pre-loaded, so no need for `tpp_lexer_yield_blocking()` */
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tok == TPP_TOK_EOF) {
		/* Simple case: it's a "simple" argument (that doesn't do anything when expanded) */
		tpp_assert(expected_simple_tok_start <= arginfo->tlai_end);
		self->tmei_expand_data = (tpp_char *)arginfo->tlai_start;
		self->tmei_expand_size = (tpp_size)(expected_simple_tok_start - arginfo->tlai_start);
		goto done;
	}
	if (token->tt_start == expected_simple_tok_start) {
		/* Yup: just a simple continuation */
		expected_simple_tok_start = token->tt_end;
		tpp_assert(expected_simple_tok_start <= arginfo->tlai_end &&
		           "Token spans beyond (expected) EOF?");
		tpp_assert(tpp_lexer_getfile(lexer)->tf_prev == NULL &&
		           "Nothing should have pushed a new file (because "
		           "that wouldn't be 'simple', meaning that the "
		           "'token->tt_start == expected_simple_tok_start'"
		           "check should have failed at some point)");
		goto next_tok;
	}

	/* Complicated case: got a token that isn't adjacent to its predecessor */
	tpp_string_buffer_init(&buffer);
	if (expected_simple_tok_start > arginfo->tlai_start) {
		if (!tpp_string_buffer_append(&buffer, arginfo->tlai_start,
		                              (tpp_size)(expected_simple_tok_start -
		                                         arginfo->tlai_start)))
			goto err_builder_nomem;
	}

	/* Print representation of tokens to "buffer" */
again_print_token:

#if TPP_HAVE_MAGIC_WHITESPACE
	/* Inject an extra whitespace token if last token and this one could
	 * accidentally be interpreted as a single token:
	 * >> #define FOO() foo
	 * >> #define BAR   bar
	 * >> #define SCAN(x) x
	 * >> SCAN(FOO()BAR)
	 *
	 * During expansion of "SCAN", we will get here 2 times, with 2 tokens:
	 * - "foo"
	 * - "bar"
	 *
	 * Without extra handling to add whitespace, the "foo" and "bar" tokens
	 * will appear as a singular "foobar" token in the final expansion!
	 *
	 * Fun fact: this is actually something that MSVC gets wrong:
	 * >> #define FOO()   foo
	 * >> #define BAR     bar
	 * >> #define STR(x)  #x
	 * >> #define STR2(x) STR(x)
	 * >> #define STR3(x) STR2(x)
	 * >> #define foobar  wtf_why_is_this_macro_expanded
	 * >> static char const a[] = STR3(FOO()BAR);
	 * >> TPP_STATIC_ASSERT(sizeof(a) == 31); // [foo][bar] -> [foobar] -> [wtf_why_is_this_macro_expanded]
	 *
	 * Possible expansions:
	 * - static char const a[] = "foo bar";                        // TPP (compliant text-based preprocessor)
	 * - static char const a[] = "foobar";                         // GCC (compliant token-based preprocessor)
	 * - static char const a[] = "wtf_why_is_this_macro_expanded"; // MSVC (non-compliant text-based preprocessor)
	 *
	 * Note that GCC also gets it right here, whilst at the same time proving that
	 * it uses a token-based preprocessor (it expands to a[] = "foobar", iow: it
	 * doesn't add any whitespace, but also doesn't fall into the trap MSVC falls
	 * into, and TPP is able to emulate under "-DTPP_HAVE_MAGIC_WHITESPACE=0")
	 */
	if (tpp_macro_hasmagicwhitespace(macro)) {
		if (tpp_lexer_require_whitespace(lexer, prev_tok, tok)) {
			if (!tpp_string_buffer_append(&buffer, (tpp_char const *)" ", 1))
				goto err_builder_nomem;
		}
		prev_tok = tok;
	}
#endif /* TPP_HAVE_MAGIC_WHITESPACE */
	if (!tpp_string_buffer_append(&buffer, token->tt_start,
	                              (tpp_size)(token->tt_end -
	                                         token->tt_start)))
		goto err_builder_nomem;
	tok = tpp_lexer_yield(lexer); /* Caller has pre-loaded, so no need for `tpp_lexer_yield_blocking()` */
	if (TPP_TOK_ISERR(tok))
		return TPP_TOK_ASERR(tok);
	if (tok != TPP_TOK_EOF)
		goto again_print_token;

	/* EOF reached -> argument fully expanded! */
	tpp_string_buffer_truncate(&buffer);
	self->tmei_expand_data = buffer.tsb_data;
	self->tmei_expand_size = buffer.tsb_size;
done:
	/* Verify that the `#ifdef`-stack of "file" is empty.
	 * If it isn't, emit warnings and clear it now. */
	return tpp_lexer_warn_nonempty_ifdef(lexer);
err_builder_nomem:
	tpp_string_buffer_fini(&buffer);
	return TPP_ENOMEM;
}

typedef struct tpp_macro_argbuf {
	tpp_lexer_arginfo tmab_arginfo[4096/*TPP_FLEX_ARRAY*/]; /* [:tmf_argc] Origin argument info vector */
/*	tpp_macro_expinfo tmab_expinfo[      TPP_FLEX_ARRAY  ];  * [:tmf_argc] Argument expansion info vector. */
} tpp_macro_argbuf;

#define tpp_macro_argbuf_sizeof(argc) \
	((argc) * (sizeof(tpp_lexer_arginfo) + sizeof(tpp_macro_expinfo)))
#define tpp_macro_argbuf_getarginfo(self, argc) \
	((self)->tmab_arginfo)
#define tpp_macro_argbuf_getexpinfo(self, argc) \
	((tpp_macro_expinfo *)((self)->tmab_arginfo + (argc)))


/* >> tpp_lexer_seek_rparen_flags_frommacro(macro)
 * Return `TPP_LEXER_SEEK_RPAREN_FLAG_*` flags needed by "macro" */
#if !TPP_HAVE_MACRO_FLAGS
#define tpp_lexer_seek_rparen_flags_frommacro(macro) TPP_LEXER_SEEK_RPAREN_FLAG_NORMAL
#else /* !TPP_HAVE_MACRO_FLAGS */
#if defined(TPP_MACRO_FLAG_VARIADIC) && !defined(TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS)
#error "Invalid internal configuration: macros require varargs support, but 'TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS' isn't available"
#endif /* !TPP_MACRO_FLAG_VARIADIC && !TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS */
#if defined(TPP_MACRO_FLAG_KEEPARGSPC) && !defined(TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC)
#error "Invalid internal configuration: macros require keep-argument-space support, but 'TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC' isn't available"
#endif /* !TPP_MACRO_FLAG_KEEPARGSPC && !TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC */
#if defined(TPP_MACRO_FLAG_MAGIC_WHITESPACE) && !defined(TPP_LEXER_SEEK_RPAREN_FLAG_MAGIC_WHITESPACE)
#error "Invalid internal configuration: macros require magic-whitespace support, but 'TPP_LEXER_SEEK_RPAREN_FLAG_MAGIC_WHITESPACE' isn't available"
#endif /* !TPP_MACRO_FLAG_MAGIC_WHITESPACE && !TPP_LEXER_SEEK_RPAREN_FLAG_MAGIC_WHITESPACE */
#undef TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON
#if ((defined(TPP_MACRO_FLAG_VARIADIC) && TPP_MACRO_FLAG_VARIADIC == TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS) && \
     (defined(TPP_MACRO_FLAG_KEEPARGSPC) && TPP_MACRO_FLAG_KEEPARGSPC == TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC) && \
     (defined(TPP_MACRO_FLAG_MAGIC_WHITESPACE) && TPP_MACRO_FLAG_MAGIC_WHITESPACE == TPP_LEXER_SEEK_RPAREN_FLAG_MAGIC_WHITESPACE))
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON (TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS | TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC | TPP_LEXER_SEEK_RPAREN_FLAG_MAGIC_WHITESPACE)
#elif defined(TPP_MACRO_FLAG_VARIADIC) && TPP_MACRO_FLAG_VARIADIC == TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON TPP_LEXER_SEEK_RPAREN_FLAG_VARARGS
#elif defined(TPP_MACRO_FLAG_KEEPARGSPC) && TPP_MACRO_FLAG_KEEPARGSPC == TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON TPP_LEXER_SEEK_RPAREN_FLAG_KEEPARGSPC
#elif defined(TPP_MACRO_FLAG_MAGIC_WHITESPACE) && TPP_MACRO_FLAG_MAGIC_WHITESPACE == TPP_LEXER_SEEK_RPAREN_FLAG_MAGIC_WHITESPACE
#define TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON TPP_LEXER_SEEK_RPAREN_FLAG_MAGIC_WHITESPACE
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
#ifdef TPP_MACRO_FLAG_MAGIC_WHITESPACE
#define _tpp_macro_get_seek_flag_magic_whitespace(macro) (((macro)->tm_flags & TPP_MACRO_FLAG_MAGIC_WHITESPACE) ? TPP_LEXER_SEEK_RPAREN_FLAG_MAGIC_WHITESPACE : 0)
#else /* TPP_MACRO_FLAG_MAGIC_WHITESPACE */
#define _tpp_macro_get_seek_flag_magic_whitespace(macro) 0
#endif /* !TPP_MACRO_FLAG_MAGIC_WHITESPACE */
#define tpp_lexer_seek_rparen_flags_frommacro(macro) \
	(_tpp_macro_get_seek_flag_varargs(macro) |       \
	 _tpp_macro_get_seek_flag_keepargspc(macro) |    \
	 _tpp_macro_get_seek_flag_magic_whitespace(macro))
#endif /* !TPP_LEXER_SEEK_RPAREN_FLAGS_FROMMACRO_COMMON */
#endif /* TPP_HAVE_MACRO_FLAGS */


/* Return the invocation argument buffer of `macro`
 * @return: NULL: Out of memory (TPP_ENOMEM) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_argbuf *TPPCALL
tpp_macro_acquire_argbuf(tpp_macro *tpp_restrict macro) {
	tpp_macro_argbuf *result = macro->tm_data.tmd_func.tmf_argbuf;
	if (result == NULL) {
		tpp_size size = tpp_macro_argbuf_sizeof(macro->tm_data.tmd_func.tmf_argc);
		result = (tpp_macro_argbuf *)tpp_malloc(size);
	} else {
		/* Steal buffer (to prevent duplicate buffer use on recursion) */
		macro->tm_data.tmd_func.tmf_argbuf = NULL;
	}
	return result;
}

/* Release an argument buffer back to a macro. */
TPP_INTERN_IMPL TPP_NONNULL((1, 2)) void TPPCALL
tpp_macro_release_argbuf(tpp_macro *tpp_restrict macro,
                         struct tpp_macro_argbuf *tpp_restrict buffer) {
	if (macro->tm_data.tmd_func.tmf_argbuf == NULL) {
		/* Likely case (when there was no recursion): can cache buffer in macro */
		macro->tm_data.tmd_func.tmf_argbuf = buffer;
	} else {
		/* This case can happen during recursion, when another invocation
		 * of the same macro appears somewhere in the arguments given to it:
		 *
		 * >> #define min(a, b) ((a) < (b) ? (a) : (b))
		 * >> min(10, min(20, 30));
		 * ^ Notice how another call to "min" appears in the arguments of
		 *   an outer call. As such, at one point both calls need to be
		 *   active at the same time, at which point there will need to
		 *   be (at least) 2 "tpp_macro_argbuf" for the same macro.
		 *
		 * The point where we get here is when the outer "min" completes,
		 * and wants to dispose of its argument buffer, but notices that
		 * the macro already has a buffer cached within (namely: the one
		 * that was used by the inner invocation) */
		tpp_free(buffer);
	}
}


#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
TPP_FORMATPRINTER_DEFINE(tpp_count_printer, arg, text, num_bytes) {
	(void)arg;
	(void)text;
	return (tpp_ssize)num_bytes;
}

TPP_FORMATPRINTER_DEFINE(tpp_buffer_printer, arg, text, num_bytes) {
	tpp_char **p_dst = (tpp_char **)arg;
	tpp_char *dst = *p_dst;
	dst = (tpp_char *)tpp_mempcpy(dst, text, num_bytes);
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
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_token *const token = tpp_lexer_gettoken(self);
	tpp_keyword const *const macro_keyword = tpp_lexer_gettokenkwd(self);
	tpp_char const *rollback_pos;
	tpp_char const *macro_call_start_pos;
#if TPP_HAVE_MACRO_RECURSION
	tpp_size const macro_keyword_len = tpp_lexer_gettokenlen(self);
#endif /* TPP_HAVE_MACRO_RECURSION */
	tpp_token_id tok;
	tpp_token_id lparen_kind;
	tpp_size i, argc;    /* # of arguments given in-source during invocation */
	tpp_size macro_argc; /* == macro->tm_data.tmd_func.tmf_argc */
	tpp_macro_argbuf *argbuf;
	tpp_lexer_arginfo *invoke_arginfo; /* == argbuf->tmab_arginfo */
	tpp_macro_expinfo *invoke_expinfo; /* == argbuf->tmab_expinfo */
	tpp_file *prev_file;
	tpp_string *result_chunk;
	tpp_size result_chunk_size;
#if TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS
	char va_nargs[TPP_UTOA_MAXLEN]; /* Value for __VA_NARGS__ */
	tpp_size va_nargs_len = 0; /* XXX: Initialization doesn't matter; only here to shut up compiler warnings */
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/
	tpp_assert(TPP_MACRO_KIND_ISFUNC(macro->tm_kind));

	/* Skip the initial macro-argument-start '('-token */
	lparen_kind = TPP_MACRO_KIND_ASTOK_LPAREN(macro->tm_kind);
	tok = tpp_lexer_tryskip_raw(self, lparen_kind,
	                            TPP_LEXER_TRYSKIP_RAW_FLAG_INCLPREV);
	if (tok != lparen_kind) {
		if (TPP_TOK_ISERR(tok))
			goto err_tok;
		return token->tt_id; /* Missing opening '(' */
	}

	/* Load argument buffer of macro */
	argbuf = tpp_macro_acquire_argbuf(macro);
	if tpp_unlikely(!argbuf)
		return TPP_TOK_ENOMEM;
	argc = macro_argc = macro->tm_data.tmd_func.tmf_argc;
	invoke_arginfo = tpp_macro_argbuf_getarginfo(argbuf, argc);
	invoke_expinfo = tpp_macro_argbuf_getexpinfo(argbuf, argc);

	/* Create a reference to the macro (that will eventually be inherited by the produced file)
	 * Do this very early on because:
	 * - Argument scanning may need to call tpp_lexer_yieldpp(), which might #undef the macro
	 * - Argument expansion may need to call tpp_lexer_yield(), which can do the same */
	tpp_macro_incref(macro);

	/* Enter a block where file popping can be rolled back. */
	tpp_lexer_manualpopfile_start(self);
	tpp_lexer_alltokens_pushon(self);

	/* Parse arguments */
#if TPP_HAVE_LEXER_SEEKPP_RPAREN_EX
	tok = tpp_lexer_seekpp_rparen_ex(self, invoke_arginfo, &argc, &rollback_pos,
	                                 (char const *)macro_keyword->tk_kwd,
	                                 tpp_lexer_seek_rparen_flags_frommacro(macro) |
	                                 TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK,
	                                 TPP_MACRO_KIND_ASTOK_LPAREN(macro->tm_kind));
#else /* TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	tok = tpp_lexer_seekpp_rparen(self, invoke_arginfo, &argc, &rollback_pos,
	                              (char const *)macro_keyword->tk_kwd,
	                              tpp_lexer_seek_rparen_flags_frommacro(macro) |
	                              TPP_LEXER_SEEK_RPAREN_FLAG_POPRLBK);
#endif /* !TPP_HAVE_LEXER_SEEKPP_RPAREN_EX */
	if (TPP_TOK_ISERR(tok))
		goto err_tok_macro_argbuf_rollback;
	macro_call_start_pos = file->tf_tpos;

	tpp_assert(macro_argc == macro->tm_data.tmd_func.tmf_argc);
	if (argc < macro_argc) {
		/* Too few arguments */
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
			invoke_arginfo[i].tlai_chunk = NULL;
		}

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
			if (argc == 0 && macro_argc == 1) {
				/* This is something the standard explicitly allows (see `test/stdc-6.10.3.5_5.h`) */
			} else {
				error = tpp_lexer_warnf(self, TPP_W_TOO_FEW_ARGUMENTS,
				                        (char const *)macro_keyword->tk_kwd,
				                        (unsigned int)macro_argc,
				                        (unsigned int)argc);
				if (TPP_ISERR(error)) {
					tok = TPP_TOK_OFERR(error);
					goto err_tok_macro_argbuf_rollback_arginfo;
				}
			}
#endif /* TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */
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
		(void)tpp_memmovedown(va_nargs, endp, va_nargs_len);
		result_chunk_size += va_nargs_len * macro->tm_data.tmd_func.tmf_n_vanargs;
	}
#endif /* TPP_HAVE_MACRO_DATA_FUNC_N_VANARGS*/

	/* Account for extra space needed by inserted arguments.
	 * This is also the part where arguments are recursively
	 * expanded */
	{
		tpp_file_subtext_push(file); /* tpp_macro_expinfo_init() needs this (to manually re-parse arguments) */
		for (i = 0; i < macro_argc; ++i) {
			tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
			if (arg->tma_ins_exp) {
				tpp_macro_expinfo *expand = &invoke_expinfo[i];
				tpp_file_subtext_setchunk_fromarg(file, arginfo);
				error = tpp_macro_expinfo_init(expand, arginfo, self, macro);
				if (TPP_ISERR(error)) {
					tok = TPP_TOK_OFERR(error);
					tpp_lexer_popallfiles(self);
					tpp_file_subtext_break(file);
					goto err_tok_macro_argbuf_rollback_arginfo_expinfo_i;
				}
				tpp_assert(!tpp_lexer_canpopfile(self));

				/* Account for expanded text */
				result_chunk_size += (arg->tma_ins_exp * tpp_macro_expinfo_getsize(expand));
			}

#if TPP_HAVE_STRINGIZE_MACRO_ARGUMENT || TPP_HAVE_CHARIZE_MACRO_ARGUMENT
			if (arg->tma_ins_str) {
				tpp_size raw_size = (tpp_size)(arginfo->tlai_end - arginfo->tlai_start);
				tpp_size str_size = (tpp_size)tpp_token_encodestring(tpp_formatprinter_of(tpp_count_printer), NULL,
				                                                     arginfo->tlai_start, raw_size);
/*				str_size += 2; * Account for leading/trailing " or ' characters -- Already account for during compilation */
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
		tpp_file_subtext_pop(file);
	}

	/* Allocate the perfectly-sized chunk that will describe the expanded macro's text */
	result_chunk = tpp_string_malloc(result_chunk_size);
	if tpp_unlikely(!result_chunk)
		goto err_nomem_macro_argbuf_rollback_arginfo_expinfo;

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
			dst = (tpp_char *)tpp_mempcpy(dst, src, bytes);
			src += bytes;
			goto next_op;
		}

		case TPP_MACRO_OPCODE_INS_EXP: {
			tpp_size argi = *pc++;
			tpp_macro_expinfo const *expinfo = &invoke_expinfo[argi];
			tpp_assert(argi < macro_argc);
			tpp_assert(macro->tm_data.tmd_func.tmf_argv[argi].tma_ins_exp != 0);
			dst = (tpp_char *)tpp_mempcpy(dst,
			                              tpp_macro_expinfo_getdata(expinfo),
			                              tpp_macro_expinfo_getsize(expinfo));
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
			tpp_token_encodestring(tpp_formatprinter_of(tpp_buffer_printer), &dst, arginfo->tlai_start, raw_size);
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
			tpp_token_encodestring(tpp_formatprinter_of(tpp_buffer_printer), &dst, arginfo->tlai_start, raw_size);
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
			dst = (tpp_char *)tpp_mempcpy(dst, arginfo->tlai_start, raw_size);
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
			if (argc >= macro_argc)
				dst = (tpp_char *)tpp_mempcpy(dst, src, bytes);
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
			dst = (tpp_char *)tpp_mempcpy(dst, va_nargs, va_nargs_len);
			goto next_op;
		}
#endif /* TPP_HAVE_VA_NARGS_IN_MACROS */

		default: tpp_unreachable();
		}
		tpp_assert((dst == tpp_string_end(result_chunk)) &&
		           "This failing means that argument specs and macro opcodes have diverged ("
		           "this shouldn't happen and indicates a bug in `tpp_macro_builder_compile()`)");
		tpp_assert((src <= macro->tm_body_end) &&
		           "Similar to 'dst'-assertion, but source-body pointer went beyond end of "
		           "macro definition body (bug in `tpp_macro_builder_compile()`)");
	}

	/* Cleanup temporary expansion buffers. */
	for (i = 0; i < macro_argc; ++i) {
		tpp_lexer_arginfo *arginfo = &invoke_arginfo[i];
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		if (arg->tma_ins_exp) {
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			tpp_macro_expinfo_fini(expand, arginfo);
		}
#if !TPP_HAVE_FILE_MACRO_TRACKARGS
		tpp_lexer_arginfo_fini(arginfo);
#endif /* !TPP_HAVE_FILE_MACRO_TRACKARGS */
	}

	/* Release argument buffer back to macro (unless it'll be saved in the file) */
#if !TPP_HAVE_FILE_MACRO_TRACKARGS
	tpp_macro_release_argbuf(macro, argbuf);
#endif /* !TPP_HAVE_FILE_MACRO_TRACKARGS */

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
#if TPP_MAX_RECURSIVE_MACRO_DEPTH != 0
handle_duplicate_chunk:
#endif /* TPP_MAX_RECURSIVE_MACRO_DEPTH != 0 */
					tpp_string_destroy(result_chunk);
#if TPP_HAVE_FILE_MACRO_TRACKARGS
					for (i = 0; i < macro_argc; ++i) {
						tpp_lexer_arginfo *arginfo = &invoke_arginfo[i];
						tpp_lexer_arginfo_fini(arginfo);
					}
					tpp_macro_release_argbuf(macro, argbuf);
#endif /* TPP_HAVE_FILE_MACRO_TRACKARGS */
					tpp_macro_decref(macro);
					goto done_rollback;
				}
			}
		} while ((iter = iter->tf_tprev) != NULL);
#if TPP_MAX_RECURSIVE_MACRO_DEPTH != 0
		if (macro->tm_expansions > tpp_lexer_getrecursivemacrolimit(self)) {
			/* Emit a warning and disallow expansion if the
			 * macro's recursion limit has been exceeded */
#if TPP_HAVE_TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED
			error = tpp_lexer_warnf(self, TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED,
			                        macro_keyword, macro);
			if (TPP_ISERR(error)) {
				tok = TPP_TOK_OFERR(error);
				goto err_tok_macro_result_chunk_argbuf_rollback;
#define WANT_err_tok_macro_result_chunk_argbuf_rollback
			}
#endif /* TPP_HAVE_TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED */
			goto handle_duplicate_chunk;
		}
#endif /* TPP_MAX_RECURSIVE_MACRO_DEPTH != 0 */
	}
#endif /* TPP_HAVE_MACRO_RECURSION */

	/* Set-up "result_chunk" such that it will read from "body-chunk-string" */
	prev_file = tpp_file_alloc();
	tpp_lexer_alltokens_break(self);
	if tpp_unlikely(!prev_file) {
		tok = TPP_TOK_ENOMEM;
#ifdef WANT_err_tok_macro_result_chunk_argbuf_rollback
#undef WANT_err_tok_macro_result_chunk_argbuf_rollback
err_tok_macro_result_chunk_argbuf_rollback:
#endif /* WANT_err_tok_macro_result_chunk_argbuf_rollback */
		tpp_lexer_manualpopfile_break_rollback(self);
#if TPP_HAVE_FILE_MACRO_TRACKARGS
		for (i = 0; i < macro_argc; ++i) {
			tpp_lexer_arginfo *arginfo = &invoke_arginfo[i];
			tpp_lexer_arginfo_fini(arginfo);
		}
		tpp_macro_release_argbuf(macro, argbuf);
#endif /* TPP_HAVE_FILE_MACRO_TRACKARGS */
		tpp_string_decref(result_chunk);
		goto err_tok_macro;
	}
	tpp_lexer_manualpopfile_break_commit(self);
	tpp_file_move(prev_file, file);
	prev_file->tf_tpos = macro_call_start_pos;
	_tpp_file_init_macro(file, prev_file,
	                     macro,        /* Inherit the reference created at the very start */
	                     result_chunk, /* Inherit reference */
	                     tpp_string_str(result_chunk),
	                     tpp_string_end(result_chunk));
#if TPP_HAVE_FILE_MACRO_TRACKARGS
	file->tf_data.td_macro.tfm_args = invoke_arginfo;
#endif /* TPP_HAVE_FILE_MACRO_TRACKARGS */
	error = tpp_lexer_callhook_file_pushed(self);
	return TPP_TOK_OFERR_OR_EOF(error);
#if TPP_HAVE_MACRO_RECURSION
done_rollback:
	tpp_lexer_alltokens_break(self);
	file->tf_pos = rollback_pos;
	tpp_lexer_manualpopfile_break_rollback(self);

	/* NOTE: After this rollback, any preprocessor directives already
	 *       parsed will be parsed again. - While this isn't necessarily
	 *       intentional, we can only get here when "-fmacro-recursion"
	 *       is enabled, which is a TPP-specific extension, so this does
	 *       not violate any standard. */
	token->tt_end = token->tt_start + macro_keyword_len;
	token->tt_kwd = macro_keyword;
	token->tt_id  = macro_keyword->tk_id;
	return macro_keyword->tk_id;
#endif /* TPP_HAVE_MACRO_RECURSION */

err_nomem_macro_argbuf_rollback_arginfo_expinfo:
	tok = TPP_TOK_ENOMEM;
/*err_tok_macro_argbuf_rollback_arginfo_expinfo:*/
	i = macro_argc;
err_tok_macro_argbuf_rollback_arginfo_expinfo_i:
	while (i--) {
		tpp_macro_argument const *arg = &macro->tm_data.tmd_func.tmf_argv[i];
		if (arg->tma_ins_exp) {
			tpp_macro_expinfo *expand = &invoke_expinfo[i];
			tpp_lexer_arginfo const *arginfo = &invoke_arginfo[i];
			tpp_macro_expinfo_fini(expand, arginfo);
		}
	}
#if TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS
err_tok_macro_argbuf_rollback_arginfo:
#endif /* TPP_HAVE_TPP_W_TOO_FEW_ARGUMENTS */
	while (macro_argc--)
		tpp_lexer_arginfo_fini(&invoke_arginfo[macro_argc]);
err_tok_macro_argbuf_rollback:
	tpp_lexer_alltokens_pop(self);
	tpp_lexer_manualpopfile_end_rollback(self);
/*err_tok_macro_argbuf:*/
	tpp_macro_release_argbuf(macro, argbuf);
err_tok_macro:
	tpp_macro_decref(macro);
err_tok:
	/* Reset token so another attempt to yield will get us here again */
	token->tt_end = token->tt_start;
	return tok;
}

/* Perform the expansion of a user-defined `macro`, with the lexer's
 * current token set to point at the macro's identifier (meaning that
 * you have to seek ahead in order to find the opening `(` token in
 * case of a function-style macro).
 *
 * @return: tpp_lexer_gettoken(self)->tt_id : Function-style macro cannot be expanded
 * @return: TPP_TOK_EOF: Success -- caller should yield again to load the
 *                                  first macro's first expansion token.
 * @return: TPP_TOK_ENOMEM: Out of memory */
TPP_INTERN_IMPL TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_token_id TPPCALL
tpp_lexer_expand_macro(tpp_lexer *tpp_restrict self,
                       tpp_macro *tpp_restrict macro) {
	tpp_errno error;
	tpp_file *const file = tpp_lexer_getfile(self);
	tpp_file *prev_file;
	if (TPP_MACRO_KIND_ISFUNC(macro->tm_kind))
		return tpp_lexer_expand_macro_function(self, macro);

	/* Simple case: keyword-style macro */

	/* Allocate a new file that will be used to backup the current input file. */
	prev_file = tpp_file_alloc();
	if tpp_unlikely(!prev_file)
		goto err_nomem;
	tpp_file_move(prev_file, file);
	if (macro->tm_body_chunk)
		tpp_string_incref(macro->tm_body_chunk);
	tpp_macro_incref(macro);
	_tpp_file_init_macro(file, prev_file,
	                     macro,                /* Inherit reference */
	                     macro->tm_body_chunk, /* Inherit reference */
	                     macro->tm_body_start,
	                     macro->tm_body_end);
	error = tpp_lexer_callhook_file_pushed(self);
	return TPP_TOK_OFERR_OR_EOF(error);
err_nomem:
	return TPP_TOK_ENOMEM;
}
#endif /* TPP_HAVE_CPP_MACROS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_LEXER_YIELD_MACRO_C */
