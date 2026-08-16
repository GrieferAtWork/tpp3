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
#ifndef GUARD_TPP_OPTIONAL_EMITTER_EMITTER_IO_C
#define GUARD_TPP_OPTIONAL_EMITTER_EMITTER_IO_C 1
#define TPP_BUILDING_OPTIONAL 1
#define TPP_EMITTER_BUILDING 1

#include "api.h"

#include "config.h"
#include "emitter-io.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_EMITTER_HAVE_IO_PRINTPWD
#ifndef tpp_io_printpwd

#if !TPP_HOST_NO_SYSTEM_INCLUDES
TPP_DECL_END
#if TPP_OS_WINDOWS
#include <Windows.h>
#elif TPP_OS_UNIX
#include <unistd.h>
#include <errno.h>
#endif /* TPP_OS_UNIX */
TPP_DECL_BEGIN
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */

#ifndef TPP_PATH_MAX
#ifdef PATH_MAX
#define TPP_PATH_MAX PATH_MAX
#elif defined(MAX_PATH)
#define TPP_PATH_MAX MAX_PATH
#else /* ... */
#define TPP_PATH_MAX 260
#endif /* !... */
#endif /* !TPP_PATH_MAX */

#if TPP_OS_WINDOWS
#ifdef tpp_writeutf8_rev
#define tpp_io_printpwd_writeutf8_rev tpp_writeutf8_rev
#else /* tpp_writeutf8_rev */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_io_printpwd_writeutf8_rev(tpp_char *dst, tpp_unichar uc) {
	if (uc <= TPP_UTF8_1BYTE_MAX) {
		*--dst = (tpp_char)uc;
	} else if (uc <= TPP_UTF8_2BYTE_MAX) {
		*--dst = 0x80 | (tpp_char)((uc) & 0x3f);
		*--dst = 0xc0 | (tpp_char)((uc >> 6) /* & 0x1f*/);
	} else if (uc <= TPP_UTF8_3BYTE_MAX) {
		*--dst = 0x80 | (tpp_char)((uc) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*--dst = 0xe0 | (tpp_char)((uc >> 12) /* & 0x0f*/);
	} else if (uc <= TPP_UTF8_4BYTE_MAX) {
		*--dst = 0x80 | (tpp_char)((uc) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 12) & 0x3f);
		*--dst = 0xf0 | (tpp_char)((uc >> 18) /* & 0x07*/);
	} else if (uc <= TPP_UTF8_5BYTE_MAX) {
		*--dst = 0x80 | (tpp_char)((uc) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 12) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 18) & 0x3f);
		*--dst = 0xf8 | (tpp_char)((uc >> 24) /* & 0x03*/);
	} else if (uc <= TPP_UTF8_6BYTE_MAX) {
		*--dst = 0x80 | (tpp_char)((uc) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 12) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 18) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 24) & 0x3f);
		*--dst = 0xfc | (tpp_char)((uc >> 30) /* & 0x01*/);
	} else {
		*--dst = 0x80 | (tpp_char)((uc) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 6) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 12) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 18) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 24) & 0x3f);
		*--dst = 0x80 | (tpp_char)((uc >> 30) & 0x03 /* & 0x3f*/);
		*--dst = 0xfe;
	}
	return dst;
}
#endif /* !tpp_writeutf8_rev */

/* @param: src_count: # of utf-16 words in `src` (== # of bytes/2) */
static TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_char *TPPCALL
tpp_io_printpwd_utf16_to_utf8(tpp_uint_least16 const *src, tpp_size src_count, tpp_char *dst_end) {
	for (src += src_count; src_count; --src_count) {
		tpp_uint_least16 ord = *--src;
		if (ord >= 0xdc00 /*TPP_UTF16_LOW_SURROGATE_MIN*/ &&
		    ord <= 0xdfff /*TPP_UTF16_LOW_SURROGATE_MAX*/ && src_count) {
			/* Second word of surrogate pair */
			tpp_unichar uc = *--src;
			--src_count;
			/* "ord_lo" *should* be in range:
			 * [TPP_UTF16_HIGH_SURROGATE_MIN,TPP_UTF16_HIGH_SURROGATE_MAX] */
			uc &= (0xdbff /*TPP_UTF16_HIGH_SURROGATE_MAX*/ -
			       0xd800 /*TPP_UTF16_HIGH_SURROGATE_MIN*/);
			uc <<= 10;
			uc |= ord - 0xdc00 /*TPP_UTF16_LOW_SURROGATE_MIN*/;
			uc += 0x10000 /*TPP_UTF16_SURROGATE_SHIFT*/;
			dst_end = tpp_io_printpwd_writeutf8_rev(dst_end, uc);
		} else {
			dst_end = tpp_io_printpwd_writeutf8_rev(dst_end, ord);
		}
	}
	return dst_end;
}
#endif /* TPP_OS_WINDOWS */


/* Print the hosting process's current-working-directory ($PWD,
 * aka `getcwd()`) to the given `printer`. Needed primarily to
 * implement `TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY`
 *
 * @return: * :  Sum of positive return values of `printer`
 * @return: < 0: First negative return value of `printer`
 * @return: TPP_SSIZE_OFERR(TPP_EIO):    I/O error
 * @return: TPP_SSIZE_OFERR(TPP_ENOMEM): Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_io_printpwd(tpp_formatprinter printer, void *arg) {
#if TPP_OS_WINDOWS || TPP_OS_UNIX
#if TPP_OS_WINDOWS
#define tpp_io_printpwd_char WCHAR
#else /* TPP_OS_WINDOWS */
#define tpp_io_printpwd_char char
#endif /* !TPP_OS_WINDOWS */
	tpp_ssize result;
	tpp_size buflen = TPP_PATH_MAX;
	tpp_io_printpwd_char *buf = (tpp_io_printpwd_char *)tpp_trymalloc(buflen * sizeof(tpp_io_printpwd_char));
	if tpp_unlikely(!buf) {
		buflen = 2;
		buf = (tpp_io_printpwd_char *)tpp_malloc(buflen * sizeof(tpp_io_printpwd_char));
		if tpp_unlikely(!buf)
			return TPP_SSIZE_OFERR(TPP_ENOMEM);
	}
	for (;;) {
		tpp_size new_buflen;
		tpp_io_printpwd_char *new_buf;
#if TPP_OS_WINDOWS
		DWORD status = GetCurrentDirectoryW((DWORD)buflen, buf);
		if (status < (DWORD)buflen)
			break;
		if (status == 0) {
			tpp_free(buf);
			return TPP_SSIZE_OFERR(TPP_EIO);
		}
#else /* TPP_OS_WINDOWS */
		int saved_errno = errno;
		if (getcwd(buf, buflen))
			break;
		if (errno != ERANGE) {
			tpp_free(buf);
			return TPP_SSIZE_OFERR(TPP_EIO);
		}
		errno = saved_errno;
#endif /* !TPP_OS_WINDOWS */

		new_buflen = buflen * 2;
		if (new_buflen <= buflen) {
			tpp_free(buf);
			return TPP_SSIZE_OFERR(TPP_ENOMEM);
		}
		new_buf = (tpp_io_printpwd_char *)tpp_tryrealloc(buf, new_buflen * sizeof(tpp_io_printpwd_char));
		if tpp_unlikely(!new_buf) {
			new_buflen = buflen + 1;
			new_buf = (tpp_io_printpwd_char *)tpp_realloc(buf, new_buflen * sizeof(tpp_io_printpwd_char));
			if tpp_unlikely(!new_buf) {
				tpp_free(buf);
				return TPP_SSIZE_OFERR(TPP_ENOMEM);
			}
		}
		buf = new_buf;
		buflen = new_buflen;
	}

#if TPP_OS_WINDOWS
	/* Inplace-convert utf-16 to utf-8 */
	{
		tpp_size textlen;
		tpp_char *utf8_end, *utf8_base;
		for (textlen = 0; textlen < buflen; ++textlen) {
			if (!buf[textlen])
				break;
		}
		utf8_end  = (tpp_char *)&buf[textlen];
		utf8_base = tpp_io_printpwd_utf16_to_utf8((tpp_uint_least16 const *)buf, textlen, utf8_end);
		result = tpp_formatprinter_print_cstr(printer, arg, utf8_base,
		                                      (tpp_size)(utf8_end - utf8_base));
	}
#else /* TPP_OS_WINDOWS */
	result = tpp_formatprinter_print_cstr(printer, arg, buf,
	                                      tpp_strnlen(buf, buflen));
#endif /* !TPP_OS_WINDOWS */
	tpp_free(buf);
	return result;
#else /* ... */
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "Invalid configuration: `tpp_io_printpwd()` is enabled, but no way to implement for this OS"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
	return tpp_formatprinter_print_conststr(printer, arg, ".");
#endif /* !... */
}
#endif /* !tpp_io_printpwd */
#endif /* TPP_EMITTER_HAVE_IO_PRINTPWD */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_IO_C */
