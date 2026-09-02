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
#ifndef GUARD_TPP_STRING_H
#define GUARD_TPP_STRING_H 1

#include "api.h"
#include "config.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_KEYWORD_ASSTRING || TPP_HAVE_STATIC_EMPTY_STRING
/* Need to use atomic reference counters because:
 * - `TPP_HAVE_KEYWORD_ASSTRING`: keywords are effectively strings,
 *   and builtin keywords (`tpp_builtin_getkeyword()`) are global.
 * - `TPP_HAVE_STATIC_EMPTY_STRING`: `tpp_string_newempty()` returns
 *   a global singleton. */
#define _tpp_string_refcnt          tpp_refcnt_atomic
#define _TPP_STRING_REFCNT_INIT     TPP_REFCNT_ATOMIC_INIT
#define _tpp_string_refcnt_init     tpp_refcnt_atomic_init
#define _tpp_string_refcnt_inc      tpp_refcnt_atomic_inc
#define _tpp_string_refcnt_decfetch tpp_refcnt_atomic_decfetch
#define _tpp_string_refcnt_isshared tpp_refcnt_atomic_isshared
#define _tpp_string_refcnt_dec      tpp_refcnt_atomic_dec
#else /* TPP_HAVE_KEYWORD_ASSTRING || TPP_HAVE_STATIC_EMPTY_STRING */
/* Can use non-atomic reference counters in `tpp_string` in this configuration */
#define _tpp_string_refcnt          tpp_refcnt
#define _TPP_STRING_REFCNT_INIT     TPP_REFCNT_INIT
#define _tpp_string_refcnt_init     tpp_refcnt_init
#define _tpp_string_refcnt_inc      tpp_refcnt_inc
#define _tpp_string_refcnt_decfetch tpp_refcnt_decfetch
#define _tpp_string_refcnt_isshared tpp_refcnt_isshared
#define _tpp_string_refcnt_dec      tpp_refcnt_dec
#endif /* !TPP_HAVE_KEYWORD_ASSTRING && !TPP_HAVE_STATIC_EMPTY_STRING */

typedef struct tpp_string {
	_tpp_string_refcnt TPP_INTERNAL(ts_refcnt);              /* Reference counter (must be atomic in this configuration) */
	tpp_size           TPP_INTERNAL(ts_len);                 /* [const] Length of the string */
	tpp_char           TPP_INTERNAL(ts_str)[TPP_FLEX_ARRAY]; /* [const][ts_len] String content */
/*	tpp_char           TPP_INTERNAL(ts_nul);                  * [const][== 0] Trailing `\0`-character */
} tpp_string;

/* Helper macro to statically define a string. */
#define TPP_STRING_DEFINE(name, value)                          \
	struct {                                                    \
		_tpp_string_refcnt TPP_INTERNAL(ts_refcnt);             \
		tpp_size           TPP_INTERNAL(ts_len);                \
		tpp_char           TPP_INTERNAL(ts_str)[sizeof(value)]; \
	} name = {                                                  \
		/* .ts_refcnt = */ TPP_REFCNT_ATOMIC_INIT(1),           \
		/* .ts_len    = */ sizeof(value) - sizeof(char),        \
		/* .ts_str    = */ value                                \
	}

/* Internal allocation API */
#define _tpp_string_sizeof(len)        (tpp_offsetof(tpp_string, ts_str) + ((len) + 1) * sizeof(tpp_char))
#define _tpp_string_trymalloc(len)     ((tpp_string *)tpp_trymalloc(_tpp_string_sizeof(len)))
#define _tpp_string_malloc(len)        ((tpp_string *)tpp_malloc(_tpp_string_sizeof(len)))
#define _tpp_string_tryrealloc(p, len) ((tpp_string *)tpp_tryrealloc(p, _tpp_string_sizeof(len)))
#define _tpp_string_realloc(p, len)    ((tpp_string *)tpp_realloc(p, _tpp_string_sizeof(len)))
#define _tpp_string_free(p)            tpp_free(p)

/* Public API */
#define tpp_string_len(self)  ((self)->TPP_INTERNAL(ts_len))
#define tpp_string_str(self)  ((self)->TPP_INTERNAL(ts_str))
#define tpp_string_end(self)  (tpp_string_str(self) + tpp_string_len(self))
#define tpp_string_cstr(self) ((char const *)tpp_string_str(self))
#define tpp_string_equals(lhs, rhs)                       \
	(tpp_string_len(lhs) == tpp_string_len(rhs) &&        \
	 tpp_memcmp(tpp_string_str(lhs), tpp_string_str(rhs), \
	            tpp_string_len(lhs) * sizeof(tpp_char)) == 0)
#define tpp_string_equals_conststr(lhs, rhs_CONSTstr)                \
	(tpp_string_len(lhs) == (sizeof(rhs_CONSTstr) - sizeof(char)) && \
	 tpp_memcmp(tpp_string_str(lhs), rhs_CONSTstr,                   \
	            (sizeof(rhs_CONSTstr) - sizeof(char)) * sizeof(tpp_char)) == 0)

/* Helpers for interacting with TPP strings */
#define tpp_string_destroy(self)  _tpp_string_free(self)
#define tpp_string_incref(self)   _tpp_string_refcnt_inc(&(self)->TPP_INTERNAL(ts_refcnt))
#define tpp_string_isshared(self) _tpp_string_refcnt_isshared(&(self)->TPP_INTERNAL(ts_refcnt))
#define tpp_string_decref(self) \
	(void)(_tpp_string_refcnt_decfetch(&(self)->TPP_INTERNAL(ts_refcnt)) || (tpp_string_destroy(self), 0))
#define tpp_string_decref_nokill(self) _tpp_string_refcnt_dec(&(self)->TPP_INTERNAL(ts_refcnt))

/* Allocate new (uninitialized) string buffers
 * @return: NULL: Propagate TPP_ENOMEM */
TPP_DECL TPP_WUNUSED tpp_string *TPPCALL tpp_string_trymalloc(tpp_size len);
TPP_DECL TPP_WUNUSED tpp_string *TPPCALL tpp_string_malloc(tpp_size len);


#if TPP_HAVE_STATIC_EMPTY_STRING
struct TPP_INTERNAL(tpp_string_empty_struct) {
	_tpp_string_refcnt TPP_INTERNAL(ts_refcnt); /* Reference counter */
	tpp_size           TPP_INTERNAL(ts_len);    /* [const] Length of the string */
	tpp_char           TPP_INTERNAL(ts_nul);    /* [const][== 0] Trailing `\0`-character */
};

#if !TPP_USE_STATIC
TPP_DECL struct TPP_INTERNAL(tpp_string_empty_struct) _tpp_string_empty;
#endif /* !TPP_USE_STATIC */

#define tpp_string_newempty()               \
	(tpp_string_incref(&_tpp_string_empty), \
	 (TPP_REF tpp_string *)&_tpp_string_empty)
#else /* TPP_HAVE_STATIC_EMPTY_STRING */
#define tpp_string_newempty() tpp_string_malloc(0)
#endif /* !TPP_HAVE_STATIC_EMPTY_STRING */



/************************************************************************/
/* STRING BUILDER                                                       */
/************************************************************************/
#if TPP_HAVE_STRING_BUILDER
typedef struct tpp_string_builder {
	tpp_string *TPP_INTERNAL(tsb_buf); /* [0..1][owned] Allocated string buffer (`ts_len` in here is then *allocated* buffer size) */
	tpp_size    TPP_INTERNAL(tsb_len); /* [<= tsb_buf->ts_len] Used buffer size */
} tpp_string_builder;

/* Initialize / finalize a given `tpp_string_builder *self` */
#define tpp_string_builder_init(self)            \
	(void)((self)->TPP_INTERNAL(tsb_buf) = NULL, \
	       (self)->TPP_INTERNAL(tsb_len) = 0)
#define tpp_string_builder_fini(self) \
	_tpp_string_free((self)->TPP_INTERNAL(tsb_buf))

/* Return the # of used bytes */
#define tpp_string_builder_getlen(self) (self)->TPP_INTERNAL(tsb_len)

/* Check if the builder is empty */
#define tpp_string_builder_isempty(self) ((self)->TPP_INTERNAL(tsb_len) == 0)

/* Package `self` into a tpp string and return said string.
 * This function never fails, but it *DOES* finalize `self`
 * iow: DO NOT CALL `tpp_string_builder_fini()` AFTER THIS FUNCTION!
 *
 * @return: * :   The string that was written to this builder
 * @return: NULL: Out-of-memory (only if `!TPP_HAVE_STATIC_EMPTY_STRING`) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) TPP_REF tpp_string *TPPCALL
tpp_string_builder_pack(/*inherit(always)*/ tpp_string_builder *tpp_restrict self);

/* Allocate (and return) an additional buffer of at least `num_bytes` characters,
 * to-be initialized by the caller at the end of all string data that has already
 * been allocated to the given builder.
 *
 * @return: * :   Pointer to the base of a `num_bytes`-bytes
 *                long buffer (to-be initialized by the caller)
 *                This pointer ONLY remains valid until the next
 *                call to this function with the same `self`.
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_string_builder_alloc(tpp_string_builder *tpp_restrict self, tpp_size num_bytes);
#if TPP_HAVE_STRING_BUILDER_TRYALLOC
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_string_builder_tryalloc(tpp_string_builder *tpp_restrict self, tpp_size num_bytes);
#endif /* TPP_HAVE_STRING_BUILDER_TRYALLOC */

/* After a call to `tpp_string_builder_tryalloc()` that didn't actually end up
 * needing all memory, use this function release the unused, trailing portion. */
#define tpp_string_builder_release(self, num_unused_trailing_bytes)                  \
	(void)(tpp_assert((self)->TPP_INTERNAL(tsb_len) >= (num_unused_trailing_bytes)), \
	       (self)->TPP_INTERNAL(tsb_len) -= (num_unused_trailing_bytes))

/* Assign a new length to `self`, releasing unused, trailing memory */
#define tpp_string_builder_truncate(self, new_length)                 \
	(void)(tpp_assert((self)->TPP_INTERNAL(tsb_len) >= (new_length)), \
	       (self)->TPP_INTERNAL(tsb_len) = (new_length))

/* Print `text` into `tpp_string_builder *self`
 * @return: num_bytes:                   Success
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM): Out of memory */
#if !TPP_USE_STATIC
TPP_FORMATPRINTER_DECL(tpp_string_builder_print);
#endif /* !TPP_USE_STATIC */
#define tpp_string_builder_doprint(builder, text, num_bytes) \
	tpp_formatprinter_print_byname(tpp_string_builder_print, builder, text, num_bytes)
#endif /* TPP_HAVE_STRING_BUILDER */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_STRING_H */
