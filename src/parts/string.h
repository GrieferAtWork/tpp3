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

typedef struct tpp_string {
	tpp_refcnt_atomic TPP_INTERNAL(ts_refcnt);              /* Reference counter (must be atomic because of "_tpp_string_empty") */
	tpp_size          TPP_INTERNAL(ts_len);                 /* [const] Length of the string */
	tpp_char          TPP_INTERNAL(ts_str)[TPP_FLEX_ARRAY]; /* [const][ts_len] String content */
/*	tpp_char          TPP_INTERNAL(ts_nul);                  * [const][== 0] Trailing \0-character */
} tpp_string;

/* Public API */
#define tpp_string_len(self)  ((self)->TPP_INTERNAL(ts_len))
#define tpp_string_str(self)  ((self)->TPP_INTERNAL(ts_str))
#define tpp_string_end(self)  (tpp_string_str(self) + tpp_string_len(self))
#define tpp_string_cstr(self) ((char const *)tpp_string_str(self))
#define tpp_string_equals(lhs, rhs)                       \
	(tpp_string_len(lhs) == tpp_string_len(rhs) &&        \
	 tpp_memcmp(tpp_string_str(lhs), tpp_string_str(rhs), \
	            tpp_string_len(lhs) * sizeof(tpp_char)) == 0)
#define tpp_string_equals_cstr(lhs, rhs_cstr)                    \
	(tpp_string_len(lhs) == (sizeof(rhs_cstr) - sizeof(char)) && \
	 tpp_memcmp(tpp_string_str(lhs), rhs_cstr,                   \
	            (sizeof(rhs_cstr) - sizeof(char)) * sizeof(tpp_char)) == 0)

#define tpp_string_sizeof(len)         (tpp_offsetof(tpp_string, ts_str) + ((len) + 1) * sizeof(tpp_char))
#define _tpp_string_trymalloc(len)     ((tpp_string *)tpp_trymalloc(tpp_string_sizeof(len)))
#define _tpp_string_malloc(len)        ((tpp_string *)tpp_malloc(tpp_string_sizeof(len)))
#define _tpp_string_tryrealloc(p, len) ((tpp_string *)tpp_tryrealloc(p, tpp_string_sizeof(len)))
#define _tpp_string_realloc(p, len)    ((tpp_string *)tpp_realloc(p, tpp_string_sizeof(len)))
#define _tpp_string_free(p)            tpp_free(p)

/* Helpers for interacting with TPP strings */
#define tpp_string_destroy(self)  _tpp_string_free(self)
#define tpp_string_incref(self)   tpp_refcnt_atomic_inc(&(self)->TPP_INTERNAL(ts_refcnt))
#define tpp_string_isshared(self) tpp_refcnt_atomic_isshared(&(self)->TPP_INTERNAL(ts_refcnt))
#define tpp_string_decref(self) \
	(void)(tpp_refcnt_atomic_decfetch(&(self)->TPP_INTERNAL(ts_refcnt)) || (tpp_string_destroy(self), 0))
#define tpp_string_decref_nokill(self) tpp_refcnt_atomic_dec(&(self)->TPP_INTERNAL(ts_refcnt))

/* Allocate new (uninitialized) string buffers
 * @return: NULL: Propagate TPP_ENOMEM */
TPP_DECL TPP_WUNUSED tpp_string *TPPCALL tpp_string_trymalloc(tpp_size len);
TPP_DECL TPP_WUNUSED tpp_string *TPPCALL tpp_string_malloc(tpp_size len);


TPP_DECL struct tpp_string_empty_struct {
	tpp_refcnt_atomic TPP_INTERNAL(ts_refcnt); /* Reference counter */
	tpp_size          TPP_INTERNAL(ts_len);    /* [const] Length of the string */
	tpp_char          TPP_INTERNAL(ts_nul);    /* [const][== 0] Trailing \0-character */
} _tpp_string_empty;

#define tpp_string_newempty()               \
	(tpp_string_incref(&_tpp_string_empty), \
	 (TPP_REF tpp_string *)&_tpp_string_empty)



/************************************************************************/
/* STRING BUILDER                                                       */
/************************************************************************/

typedef struct tpp_string_builder {
	tpp_string *TPP_INTERNAL(tsb_buf); /* [0..1][owned] Allocated string buffer ("ts_len" in here is then *allocated* buffer size) */
	tpp_size    TPP_INTERNAL(tsb_len); /* [<= tsb_buf->ts_len] Used buffer size */
} tpp_string_builder;

/* Initialize / finalize a given "tpp_string_builder *self" */
#define tpp_string_builder_init(self)            \
	(void)((self)->TPP_INTERNAL(tsb_buf) = NULL, \
	       (self)->TPP_INTERNAL(tsb_len) = 0)
#define tpp_string_builder_fini(self) \
	_tpp_string_free((self)->TPP_INTERNAL(tsb_buf))

/* Package "self" into a tpp string and return said string.
 * This function never fails, but it *DOES* finalize "self"
 * iow: DO NOT CALL `tpp_string_builder_fini()' AFTER THIS FUNCTION!
 *
 * @return: * : The string that was written to this builder */
TPP_DECL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) TPP_REF tpp_string *TPPCALL
tpp_string_builder_pack(/*inherit(always)*/ tpp_string_builder *tpp_restrict self);

/* Allocate (and return) an additional buffer of at least "num_bytes" characters,
 * to-be initialized by the caller at the end of all string data that has already
 * been allocated to the given builder.
 *
 * @return: * :   Pointer to the base of a "num_bytes"-bytes
 *                long buffer (to-be initialized by the caller)
 *                This pointer ONLY remains valid until the next
 *                call to this function with the same "self".
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_string_builder_alloc(tpp_string_builder *tpp_restrict self, tpp_size num_bytes);

/* Print "text" into "tpp_string_builder *self"
 * @return: num_bytes:            Success
 * @return: (tpp_size)TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_FORMATPRINTER_DEFINE(tpp_string_builder_print, arg, text, num_bytes);

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_STRING_H */
