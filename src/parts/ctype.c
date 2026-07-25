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
#ifndef GUARD_TPP_CTYPE_C
#define GUARD_TPP_CTYPE_C 1
#define TPP_BUILDING 1

#include "api.h"
#include "config.h"
#include "ctype.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_BUILTIN_CTYPE
TPP_CONST_IMPL uint_least8_t const _tpp_ctype[256] = {
/*[[[deemon
import UTF8_LF_FIRST_BYTES from ".token-encodestring-mblf";
import getAsciiFlags from ".ctype-db.ctype-unicode";
local asciiFlags = getAsciiFlags();
for (local i = 0; i < 256; ++i) {
	if ((i % 16) == 0)
		print "\t",;
	local flags = 0;
	if (i < #asciiFlags)
		flags |= asciiFlags[i];
	if (i >= '0'.ord() && i <= '9'.ord())
		flags |= _TPP_CTYPE_ISDIGIT;
	if (i in UTF8_LF_FIRST_BYTES)
		flags |= _TPP_CTYPE_ISMBLF;
	print "0x%.2x," % flags,;
	if ((i % 16) == 15)
		print;
}
]]]*/
	0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x08,0x04,0x04,0x08,0x04,0x04,
	0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
	0x04,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x03,
	0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//[[[end]]]
};
#endif /* TPP_HAVE_BUILTIN_CTYPE */


/* Convert an integer into a string */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL
tpp_utoa(char buf[TPP_UTOA_MAXLEN], tpp_uintmax value) {
	char *result = buf + TPP_UTOA_MAXLEN;
	do {
		uint_least8_t digit;
		digit = (uint_least8_t)(value % 10);
		value = value / 10;
		*--result = (char)tpp_ascii_ofdigit(digit);
	} while (value != 0);
	return result;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL
tpp_itoa(char buf[TPP_ITOA_MAXLEN], tpp_intmax value) {
	char *result;
	bool is_neg = value < 0;
	if (is_neg)
		value = -value;
	result = tpp_utoa(buf + TPP_ITOA_MAXLEN - TPP_UTOA_MAXLEN, value);
	if (is_neg)
		*--result = '-';
	return result;
}


/* Convert a float into a string */
#if TPP_HAVE_FTOA
TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL
tpp_inplace_utoa(char buf[TPP_UTOA_MAXLEN], tpp_uintmax value) {
	char *p = tpp_utoa(buf, value);
	tpp_size len = (tpp_size)((buf + TPP_UTOA_MAXLEN) - p);
	tpp_memmovedown(buf, p, len);
	return buf + len;
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_size TPPCALL
tpp_ftoa(char buf[TPP_FTOA_MAXLEN], tpp_float value) {
	tpp_uintmax whole;
	char *p = buf;
	char *decimal_start;
	tpp_size decimal_len;
	if (value < 0) {
		*p++ = '-';
		value = -value;
	}
	whole = (tpp_uintmax)value;
	p = tpp_inplace_utoa(p, whole);
	value -= (tpp_float)whole;
	whole = (tpp_uintmax)(value * 10000000000.0); /* 10 digits */
	tpp_assert(whole < TPP_UINTMAX_C(10000000000));
	*p++ = '.';
	decimal_start = tpp_utoa(p, whole);
	tpp_assert(decimal_start >= p);
	decimal_len = (tpp_size)((p + TPP_UTOA_MAXLEN) - decimal_start);
	while (decimal_len < 10) {
		*--decimal_start = '0';
		++decimal_len;
	}
	tpp_assert(decimal_start >= p);
	while ((decimal_len > 1) && decimal_start[decimal_len - 1] == '0')
		--decimal_len;
	tpp_memmovedown(decimal_start, p, decimal_len);
	p += decimal_len;
	return (tpp_size)(p - buf);
}
#endif /* TPP_HAVE_FTOA */


#if TPP_HAVE_TPP_UNICODE_WRITEUTF8
/* Encode "uch" as utf-8 into "buf" and return the pointer after the last-written byte. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_unicode_writeutf8(tpp_char buf[TPP_UTF8_MAXLEN], tpp_unichar uc) {
	tpp_char *dst = buf;
	if (uc <= TPP_UTF8_1BYTE_MAX) {
		*dst++ = (tpp_char)uc;
	} else if (uc <= TPP_UTF8_2BYTE_MAX) {
		*dst++ = 0xc0 | (tpp_char)((uc >> 6) /* & 0x1f*/);
		*dst++ = 0x80 | (tpp_char)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_3BYTE_MAX) {
		*dst++ = 0xe0 | (tpp_char)((uc >> 12) /* & 0x0f*/);
		*dst++ = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_4BYTE_MAX) {
		*dst++ = 0xf0 | (tpp_char)((uc >> 18) /* & 0x07*/);
		*dst++ = 0x80 | (tpp_char)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_5BYTE_MAX) {
		*dst++ = 0xf8 | (tpp_char)((uc >> 24) /* & 0x03*/);
		*dst++ = 0x80 | (tpp_char)((uc >> 18) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_6BYTE_MAX) {
		*dst++ = 0xfc | (tpp_char)((uc >> 30) /* & 0x01*/);
		*dst++ = 0x80 | (tpp_char)((uc >> 24) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc >> 18) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc) & 0x3f);
	} else {
		*dst++ = 0xfe;
		*dst++ = 0x80 | (tpp_char)((uc >> 30) & 0x03 /* & 0x3f*/);
		*dst++ = 0x80 | (tpp_char)((uc >> 24) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc >> 18) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (tpp_char)((uc) & 0x3f);
	}
	return dst;
}
#endif /* TPP_HAVE_TPP_UNICODE_WRITEUTF8 */

#if TPP_HAVE_TPP_FUZZY_MEMCMP
/* Quantify the "fuzziness" of how close 2 memory-blocks are to each
 * other (less means closer to each other, and "0" means identical)
 *
 * #ifndef tpp_alloca
 * @return: TPP_SIZE_MAX: Cannot compare strings (insufficient memory,
 *                        and no tpp_alloca() function available to
 *                        supplement).
 *                        The implementation uses "tpp_trymalloc", so
 *                        this shouldn't be considered a fatal error
 * #endif // !tpp_alloca */
TPP_IMPL TPP_WUNUSED tpp_size TPPCALL
tpp_fuzzy_memcmp(tpp_char const *lhs, tpp_size lhs_len,
                 tpp_char const *rhs, tpp_size rhs_len) {
	tpp_size *v0, *v1, i, j, cost, temp;
#ifdef tpp_alloca
	bool isheap;
#endif /* tpp_alloca */
	if tpp_unlikely(!lhs_len)
		return rhs_len;
	if tpp_unlikely(!rhs_len)
		return lhs_len;
#ifdef tpp_alloca
	if (rhs_len > (128 + 1) * sizeof(tpp_size))
#endif /* tpp_alloca */
	{
		v0 = (tpp_size *)tpp_trymalloc((rhs_len + 1) * sizeof(tpp_size));
		if tpp_unlikely(!v0) {
#ifdef tpp_alloca
			goto allocate_stack;
#else /* tpp_alloca */
			return TPP_SIZE_MAX;
#endif /* !tpp_alloca */
		}
		v1 = (tpp_size *)tpp_trymalloc((rhs_len + 1) * sizeof(tpp_size));
		if tpp_unlikely(!v1) {
			free(v0);
#ifdef tpp_alloca
			goto allocate_stack;
#else /* tpp_alloca */
			return TPP_SIZE_MAX;
#endif /* !tpp_alloca */
		}
#ifdef tpp_alloca
		isheap = true;
#endif /* tpp_alloca */
	}
#ifdef tpp_alloca
	else {
allocate_stack:
		v0     = (tpp_size *)tpp_alloca((rhs_len + 1) * sizeof(tpp_size));
		v1     = (tpp_size *)tpp_alloca((rhs_len + 1) * sizeof(tpp_size));
		isheap = false;
	}
#endif /* tpp_alloca */
	for (i = 0; i < rhs_len; ++i)
		v0[i] = i;
	for (i = 0; i < lhs_len; ++i) {
		v1[0] = i + 1;
		for (j = 0; j < rhs_len; ++j) {
			tpp_char lhs_ch = lhs[i];
			tpp_char rhs_ch = rhs[j];
			cost = (lhs_ch == rhs_ch) ? 0 : 1;
			cost += v0[j];
			temp = v1[j] + 1;
			if (temp < cost)
				cost = temp;
			temp = v0[j + 1] + 1;
			if (temp < cost)
				cost = temp;
			v1[j + 1] = cost;
		}
		tpp_memcpy(v0, v1, rhs_len * sizeof(tpp_size));
	}
	temp = v1[rhs_len];
#ifdef tpp_alloca
	if (isheap)
#endif /* tpp_alloca */
	{
		tpp_free(v0);
		tpp_free(v1);
	}
	return temp;
}
#endif /* TPP_HAVE_TPP_FUZZY_MEMCMP */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CTYPE_C */
