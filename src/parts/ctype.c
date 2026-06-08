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
TPP_CONST_IMPL uint_least8_t const tpp_ctype[256] = {
/*[[[deemon
for (local i = 0; i < 256; ++i) {
	if ((i % 16) == 0)
		print "\t",;
	local flags = 0;
	if ((i >= 'a'.ord() && i <= 'z'.ord()) ||
	    (i >= 'A'.ord() && i <= 'Z'.ord()) ||
	    i == '_'.ord() || i == '$'.ord())
		flags |= _TPP_CTYPE_ISSYMSTRT | _TPP_CTYPE_ISSYMCONT;
	if (i >= '0'.ord() && i <= '9'.ord())
		flags |= _TPP_CTYPE_ISSYMCONT | _TPP_CTYPE_ISDIGIT;
	if (i >= 0x80)
		flags |= _TPP_CTYPE_ISNONASCII;
	if (i in ['\r'.ord(), '\n'.ord()]) {
		flags |= _TPP_CTYPE_ISLF;
	} else if (i <= 32) {
		flags |= _TPP_CTYPE_ISSPACE;
	}
	print "0x%.2x," % flags,;
	if ((i % 16) == 15)
		print;
}
]]]*/
	0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x10,0x04,0x04,0x10,0x04,0x04,
	0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
	0x04,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x03,
	0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
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
		*--result = (char)('0' + digit);
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


#if TPP_HAVE_TPP_UNICODE_WRITEUTF8
/* Encode "uch" as utf-8 into "buf" and return the pointer after the last-written byte. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_unicode_writeutf8(tpp_char buf[TPP_UTF8_MAXLEN], tpp_unichar uc) {
	tpp_char *dst = buf;
	if (uc <= TPP_UTF8_1BYTE_MAX) {
		*dst++ = (uint8_t)uc;
	} else if (uc <= TPP_UTF8_2BYTE_MAX) {
		*dst++ = 0xc0 | (uint8_t)((uc >> 6) /* & 0x1f*/);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_3BYTE_MAX) {
		*dst++ = 0xe0 | (uint8_t)((uc >> 12) /* & 0x0f*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_4BYTE_MAX) {
		*dst++ = 0xf0 | (uint8_t)((uc >> 18) /* & 0x07*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_5BYTE_MAX) {
		*dst++ = 0xf8 | (uint8_t)((uc >> 24) /* & 0x03*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 18) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else if (uc <= TPP_UTF8_6BYTE_MAX) {
		*dst++ = 0xfc | (uint8_t)((uc >> 30) /* & 0x01*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 24) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 18) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	} else {
		*dst++ = 0xfe;
		*dst++ = 0x80 | (uint8_t)((uc >> 30) & 0x03 /* & 0x3f*/);
		*dst++ = 0x80 | (uint8_t)((uc >> 24) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 18) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 12) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc >> 6) & 0x3f);
		*dst++ = 0x80 | (uint8_t)((uc) & 0x3f);
	}
	return dst;
}
#endif /* TPP_HAVE_TPP_UNICODE_WRITEUTF8 */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CTYPE_C */
