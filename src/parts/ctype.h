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
#ifndef GUARD_TPP_CTYPE_H
#define GUARD_TPP_CTYPE_H 1

#include "api.h"
#include "config.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#undef TPP_HAVE_BUILTIN_CTYPE
#if (!defined(tpp_ascii_issymstrt) ||    \
     !defined(tpp_ascii_issymcont) ||    \
     !defined(tpp_ascii_isdigit) ||      \
     !defined(tpp_ascii_isspace) ||      \
     !defined(tpp_ascii_islf) ||         \
     !defined(tpp_ascii_isspaceornul) || \
     !defined(tpp_ascii_islfornul) ||    \
     !defined(tpp_ascii_isspace_nolf))
#define TPP_HAVE_BUILTIN_CTYPE 1
#else /* ... */
#define TPP_HAVE_BUILTIN_CTYPE 0
#endif /* !... */


#if TPP_HAVE_BUILTIN_CTYPE
#define _TPP_CTYPE_ISSYMSTRT  0x01 /* Symbol start character */
#define _TPP_CTYPE_ISSYMCONT  0x02 /* Symbol continuation character */
#define _TPP_CTYPE_ISSPACE    0x04 /* Space character */
#define _TPP_CTYPE_ISDIGIT    0x08 /* Space character */
#define _TPP_CTYPE_ISLF       0x10 /* Linefeed character (\r and \n) */
#define _TPP_CTYPE_ISNONASCII 0x20 /* 128+ */

TPP_CONST_DECL uint_least8_t const tpp_ctype[256]; /* Don't access directly! (considered TPP_INTERNAL) */
#if UINT_LEAST8_MAX == 0xff
#define _tpp_ascii_mask(ch) ((uint_least8_t)(ch))
#else /* UINT_LEAST8_MAX == 0xff */
#define _tpp_ascii_mask(ch) ((ch) & 0xff)
#endif /* UINT_LEAST8_MAX != 0xff */

#ifndef tpp_ascii_issymstrt
#define tpp_ascii_issymstrt(ch)    (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISSYMSTRT)
#endif /* !tpp_ascii_issymstrt */
#ifndef tpp_ascii_issymcont
#define tpp_ascii_issymcont(ch)    (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISSYMCONT)
#endif /* !tpp_ascii_issymcont */
#ifndef tpp_ascii_isdigit
#define tpp_ascii_isdigit(ch)      (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISDIGIT)
#endif /* !tpp_ascii_isdigit */
#ifndef tpp_ascii_isspace
#define tpp_ascii_isspace(ch)      (tpp_ctype[_tpp_ascii_mask(ch)] & (_TPP_CTYPE_ISSPACE | _TPP_CTYPE_ISLF))
#endif /* !tpp_ascii_isspace */
#ifndef tpp_ascii_islf
#define tpp_ascii_islf(ch)         (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISLF)
#endif /* !tpp_ascii_islf */
#ifndef tpp_ascii_islfornascii
#define tpp_ascii_islfornascii(ch) (tpp_ctype[_tpp_ascii_mask(ch)] & (_TPP_CTYPE_ISLF | _TPP_CTYPE_ISNONASCII))
#endif /* !tpp_ascii_islfornascii */
#ifndef tpp_ascii_isspace_nolf
#define tpp_ascii_isspace_nolf(ch) (tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISSPACE)
#endif /* !tpp_ascii_isspace_nolf */
#endif /* TPP_HAVE_BUILTIN_CTYPE */

/* TODO: "tpp_ascii_islfornascii()" could also be replaced with "tpp_ascii_islf_or_maybe_utf8_lf()",
 *       which would just also return true of the first byte of a utf-8 sequence that evaluates to a
 *       unicode linefeed character */
#ifndef tpp_ascii_islfornascii
#define tpp_ascii_islfornascii(ch) (tpp_ascii_islf(ch) || (ch) >= 0x80)
#endif /* !tpp_ascii_islfornascii */
#ifndef tpp_ascii_isspace_nolf
#define tpp_ascii_isspace_nolf(ch) (tpp_ascii_isspace(ch) && !tpp_ascii_islf(ch))
#endif /* !tpp_ascii_isspace_nolf */


#if TPP_HAVE_UNICODE

/* Unicode character traits (all of these take "tpp_unichar ord") */
#ifndef tpp_unicode_issymstrt
#define tpp_unicode_issymstrt(ord)    ((ord) >= 0x80 || tpp_ascii_issymstrt(ord))
#endif /* !tpp_unicode_issymstrt */
#ifndef tpp_unicode_issymcont
#define tpp_unicode_issymcont(ord)    ((ord) >= 0x80 || tpp_ascii_issymcont(ord))
#endif /* !tpp_unicode_issymcont */
#ifndef tpp_unicode_isdigit
#define tpp_unicode_isdigit(ord)      ((ord) < 0x80 && tpp_ascii_isdigit(ord))
#endif /* !tpp_unicode_isdigit */
#ifndef tpp_unicode_isspace
#define tpp_unicode_isspace(ord)      ((ord) <= 0xff && tpp_ascii_isspace(ord))
#endif /* !tpp_unicode_isspace */
#ifndef tpp_unicode_islf
#define tpp_unicode_islf(ord)         ((ord) <= 0xff && tpp_ascii_islf(ord))
#endif /* !tpp_unicode_islf */
#ifndef tpp_unicode_isspace_nolf
#define tpp_unicode_isspace_nolf(ord) ((ord) <= 0xff && tpp_ascii_isspace_nolf(ord))
#endif /* !tpp_unicode_isspace_nolf */

/* Read a single unicode character from a given utf-8 blob.
 * WARNING: This function doesn't do any validity checking,
 *          allowing over-long utf-8 sequences, as well as
 *          incorrectly positioned UTF-8 continuation bytes. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8(tpp_char const **p_pos, tpp_char const *end);

/* Same as `tpp_unicode_readutf8()', but read in reverse */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8_rev(tpp_char const **p_end, tpp_char const *base);
#endif /* TPP_HAVE_UNICODE */

#ifndef TPP_UTOA_MAXLEN
#if TPP_UINTMAX_MAX <= TPP_UINTMAX_C(255)
#define TPP_UTOA_MAXLEN 3  /* "255" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(65535)
#define TPP_UTOA_MAXLEN 5  /* "65535" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(4294967295)
#define TPP_UTOA_MAXLEN 10 /* "4294967295" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(18446744073709551615)
#define TPP_UTOA_MAXLEN 20 /* "18446744073709551615" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(340282366920938463463374607431768211455)
#define TPP_UTOA_MAXLEN 39 /* "340282366920938463463374607431768211455" */
#else /* TPP_UINTMAX_MAX <= ... */
#error "Unsupported 'TPP_UINTMAX_MAX'"
#endif /* TPP_UINTMAX_MAX > ... */
#endif /* !TPP_UTOA_MAXLEN */
#ifndef TPP_ITOA_MAXLEN
#define TPP_ITOA_MAXLEN (TPP_UTOA_MAXLEN + 1) /* +1 for leading "-" */
#endif /* !TPP_ITOA_MAXLEN */

/* Convert an integer into a string */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL tpp_utoa(char buf[TPP_UTOA_MAXLEN], tpp_uintmax value);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL tpp_itoa(char buf[TPP_ITOA_MAXLEN], tpp_intmax value);


#define TPP_UTF8_1BYTE_MAX ((UINT32_C(1) << 7) - 1)
#define TPP_UTF8_2BYTE_MAX ((UINT32_C(1) << 11) - 1)
#define TPP_UTF8_3BYTE_MAX ((UINT32_C(1) << 16) - 1)
#define TPP_UTF8_4BYTE_MAX ((UINT32_C(1) << 21) - 1)
#define TPP_UTF8_5BYTE_MAX ((UINT32_C(1) << 26) - 1)
#define TPP_UTF8_6BYTE_MAX ((UINT32_C(1) << 31) - 1)

#define TPP_UTF8_MAXLEN 7 /* Enough to write *any* 32-bit unicode ordinal as utf-8 (including invalid ones) */

#if TPP_HAVE_TPP_UNICODE_WRITEUTF8
/* Encode "uch" as utf-8 into "buf" and return the pointer after the last-written byte. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_unicode_writeutf8(tpp_char buf[TPP_UTF8_MAXLEN], tpp_unichar uc);
#endif /* TPP_HAVE_TPP_UNICODE_WRITEUTF8 */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CTYPE_H */
