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
#ifndef GUARD_TPP_STRING_C
#define GUARD_TPP_STRING_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "string.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

TPP_IMPL TPP_WUNUSED tpp_string *TPPCALL
tpp_string_trymalloc(tpp_size len) {
	tpp_string *result = _tpp_string_trymalloc(len);
	if tpp_likely(result) {
		tpp_refcnt_atomic_init(&result->ts_refcnt, 1);
		result->ts_len      = len;
		result->ts_str[len] = '\0';
	}
	return result;
}

TPP_IMPL TPP_WUNUSED tpp_string *TPPCALL
tpp_string_malloc(tpp_size len) {
	tpp_string *result = _tpp_string_malloc(len);
	if tpp_likely(result) {
		tpp_refcnt_atomic_init(&result->ts_refcnt, 1);
		result->ts_len      = len;
		result->ts_str[len] = '\0';
	}
	return result;
}

TPP_IMPL struct tpp_string_empty_struct _tpp_string_empty = {
	/* .ts_refcnt = */ TPP_REFCNT_ATOMIC_INIT(1),
	/* .ts_len    = */ 0,
	/* .ts_nul    = */ 0,
};


/************************************************************************/
/* STRING BUILDER                                                       */
/************************************************************************/

#if TPP_HAVE_STRING_BUILDER
/* Package `self` into a tpp string and return said string.
 * This function never fails, but it *DOES* finalize `self`
 * iow: DO NOT CALL `tpp_string_builder_fini()` AFTER THIS FUNCTION!
 *
 * @return: * : The string that was written to this builder */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) TPP_REF tpp_string *TPPCALL
tpp_string_builder_pack(/*inherit(always)*/ tpp_string_builder *tpp_restrict self) {
	TPP_REF tpp_string *result;

	/* Deal with special case: empty string */
	if (self->tsb_len == 0) {
		_tpp_string_free(self->tsb_buf);
		return tpp_string_newempty();
	}

	/* Truncate buffer to used length, and initialize reference counter */
	result = self->tsb_buf;
	tpp_assert(result);
	tpp_assert(self->tsb_len <= result->ts_len);
	if (self->tsb_len < result->ts_len) {
		result = _tpp_string_tryrealloc(result, self->tsb_len);
		if tpp_unlikely(!result)
			result = self->tsb_buf;
		result->ts_len = self->tsb_len;
		result->ts_str[result->ts_len] = (tpp_char)'\0';
	}
	tpp_refcnt_atomic_init(&result->ts_refcnt, 1);
	return result;
}

#ifndef TPP_STRING_BUILDER_MINALLOC
#define TPP_STRING_BUILDER_MINALLOC 64
#endif /* !TPP_STRING_BUILDER_MINALLOC */


TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_string_builder_alloc_impl(tpp_string_builder *tpp_restrict self,
                              tpp_size num_bytes, bool try_alloc) {
	tpp_char *result;
	tpp_string *buffer = self->tsb_buf;
	tpp_size cur_alloc = buffer ? buffer->ts_len : 0;
	tpp_size min_alloc = self->tsb_len + num_bytes;
	tpp_assert(cur_alloc >= self->tsb_len);
	if (cur_alloc < min_alloc) {
		tpp_size new_alloc = cur_alloc * 2;
#if TPP_STRING_BUILDER_MINALLOC > 1
		if (new_alloc < TPP_STRING_BUILDER_MINALLOC)
			new_alloc = TPP_STRING_BUILDER_MINALLOC;
#endif /* TPP_STRING_BUILDER_MINALLOC > 1 */
		if (new_alloc < min_alloc)
			new_alloc = min_alloc;
		buffer = _tpp_string_tryrealloc(buffer, new_alloc);
		if tpp_unlikely(!buffer) {
			new_alloc = min_alloc;
			buffer = try_alloc ? _tpp_string_tryrealloc(self->tsb_buf, new_alloc)
			                   : _tpp_string_realloc(self->tsb_buf, new_alloc);
			if tpp_unlikely(!buffer)
				return NULL;
		}
		buffer->ts_len = new_alloc;
		self->tsb_buf = buffer;
		tpp_assert(new_alloc >= min_alloc);
	}

	tpp_assert(buffer || !num_bytes);
	result = buffer->ts_str + self->tsb_len; /* Not a NULL dereference: "ts_str" is an *inline* array */
	self->tsb_len += num_bytes;
	return result;
}

/* Allocate (and return) an additional buffer of at least "num_bytes" characters,
 * to-be initialized by the caller at the end of all string data that has already
 * been allocated to the given builder.
 *
 * @return: * :   Pointer to the base of a "num_bytes"-bytes
 *                long buffer (to-be initialized by the caller)
 *                This pointer ONLY remains valid until the next
 *                call to this function with the same `self`.
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_string_builder_alloc(tpp_string_builder *tpp_restrict self,
                         tpp_size num_bytes) {
	return tpp_string_builder_alloc_impl(self, num_bytes, false);
}

#if TPP_HAVE_STRING_BUILDER_TRYALLOC
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_string_builder_tryalloc(tpp_string_builder *tpp_restrict self,
                            tpp_size num_bytes) {
	return tpp_string_builder_alloc_impl(self, num_bytes, true);
}
#endif /* TPP_HAVE_STRING_BUILDER_TRYALLOC */

/* Print "text" into "tpp_string_builder *self"
 * @return: num_bytes:                   Success
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM): Out of memory */
TPP_FORMATPRINTER_IMPL(tpp_string_builder_print, arg, text, num_bytes) {
	tpp_string_builder *me = (tpp_string_builder *)arg;
	tpp_char *dst = tpp_string_builder_alloc(me, num_bytes);
	if tpp_unlikely(!dst)
		goto err_nomem;
	(void)tpp_memcpy(dst, text, num_bytes);
	return (tpp_ssize)num_bytes;
err_nomem:
	return TPP_SSIZE_OFERR(TPP_ENOMEM);
}
#endif /* TPP_HAVE_STRING_BUILDER */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_STRING_C */
