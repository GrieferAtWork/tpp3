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
/* clang-format off */
#ifndef GUARD_TPP_EMITTER_AMALGAMATION_C
#define GUARD_TPP_EMITTER_AMALGAMATION_C 1

#ifndef TPP_EMITTER_AMALGAMATION_H
#define TPP_EMITTER_AMALGAMATION_H "tpp-emitter-amalgamation.h"
#endif /* !TPP_EMITTER_AMALGAMATION_H */

/* Include "tpp-emitter-amalgamation.h" header... */
#ifndef GUARD_TPP_EMITTER_AMALGAMATION_H
#undef TPP_EMITTER_BUILDING
#define TPP_EMITTER_BUILDING 1
#include TPP_EMITTER_AMALGAMATION_H
#endif /* !GUARD_TPP_EMITTER_AMALGAMATION_H */

#if !TPP_EMITTER_BUILDING
/************************************************************************/
/* File: parts/optional/emitter/expose-internals.h                      */
/************************************************************************/
/* If "tpp-emitter-amalgamation.h" was already included, re-define
 * `TPP_EMITTER_INTERNAL()` identifers to their unescaped names. */
#define temcl_emitter                            TPP_EMITTER_INTERNAL(temcl_emitter)
#define temcl_state                              TPP_EMITTER_INTERNAL(temcl_state)
#define temcl_flags                              TPP_EMITTER_INTERNAL(temcl_flags)
#define tem_feat                                 TPP_EMITTER_INTERNAL(tem_feat)
#define temf_flags                               TPP_EMITTER_INTERNAL(temf_flags)
#define temf_bitset                              TPP_EMITTER_INTERNAL(temf_bitset)
#define temff_NORMALIZE_SPACE                    TPP_EMITTER_INTERNAL(temff_NORMALIZE_SPACE)
#define temff_NORMALIZE_LF                       TPP_EMITTER_INTERNAL(temff_NORMALIZE_LF)
#define temff_NORMALIZE_C_STRING                 TPP_EMITTER_INTERNAL(temff_NORMALIZE_C_STRING)
#define temff_NORMALIZE_C_INT                    TPP_EMITTER_INTERNAL(temff_NORMALIZE_C_INT)
#define temff_NORMALIZE_KEYWORDS                 TPP_EMITTER_INTERNAL(temff_NORMALIZE_KEYWORDS)
#define temff_NORMALIZE_BSE                      TPP_EMITTER_INTERNAL(temff_NORMALIZE_BSE)
#define temff_NORMALIZE_TRIGRAPHS                TPP_EMITTER_INTERNAL(temff_NORMALIZE_TRIGRAPHS)
#define temff_NORMALIZE_DIGRAPHS                 TPP_EMITTER_INTERNAL(temff_NORMALIZE_DIGRAPHS)
#define temff_NOLINE                             TPP_EMITTER_INTERNAL(temff_NOLINE)
#define temff_RELAXED_MACRO_COLUMN               TPP_EMITTER_INTERNAL(temff_RELAXED_MACRO_COLUMN)
#define temff_USE_CPP_DIGIT                      TPP_EMITTER_INTERNAL(temff_USE_CPP_DIGIT)
#define temff_USE_CPP_DIGIT_FLAGS                TPP_EMITTER_INTERNAL(temff_USE_CPP_DIGIT_FLAGS)
#define temff_USE_CPP_DIGIT_WORKING_DIRECTORY    TPP_EMITTER_INTERNAL(temff_USE_CPP_DIGIT_WORKING_DIRECTORY)
#define temff_REEMIT_MACRO_DEFINITIONS_LAZY      TPP_EMITTER_INTERNAL(temff_REEMIT_MACRO_DEFINITIONS_LAZY)
#define temff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY TPP_EMITTER_INTERNAL(temff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#define temff_TRACE_INCLUDES                     TPP_EMITTER_INTERNAL(temff_TRACE_INCLUDES)
#define temsf_curpos                             TPP_EMITTER_INTERNAL(temsf_curpos)
#define temsf_fname                              TPP_EMITTER_INTERNAL(temsf_fname)
#define temsf_fname_str                          TPP_EMITTER_INTERNAL(temsf_fname_str)
#define temsf_flags                              TPP_EMITTER_INTERNAL(temsf_flags)
#define temsfs_file                              TPP_EMITTER_INTERNAL(temsfs_file)
#define temsfs_filec                             TPP_EMITTER_INTERNAL(temsfs_filec)
#define temsfs_filev                             TPP_EMITTER_INTERNAL(temsfs_filev)
#define tems_curfile                             TPP_EMITTER_INTERNAL(tems_curfile)
#define tems_cached_filec                        TPP_EMITTER_INTERNAL(tems_cached_filec)
#define tems_cached_filev                        TPP_EMITTER_INTERNAL(tems_cached_filev)
#define tems_flags                               TPP_EMITTER_INTERNAL(tems_flags)
#define tems_prevtok                             TPP_EMITTER_INTERNAL(tems_prevtok)
#define tem_lexer                                TPP_EMITTER_INTERNAL(tem_lexer)
#define tem_output                               TPP_EMITTER_INTERNAL(tem_output)
#define tem_state                                TPP_EMITTER_INTERNAL(tem_state)
#define tem_mode                                 TPP_EMITTER_INTERNAL(tem_mode)
#define tem_linethreshold                        TPP_EMITTER_INTERNAL(tem_linethreshold)

#endif /* !TPP_EMITTER_BUILDING */

/************************************************************************/
/* File: parts/optional/emitter/emitter-io.c                            */
/************************************************************************/
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
TPP_IMPL TPP_WUNUSED tpp_ssize TPPCALL
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

/************************************************************************/
/* File: parts/optional/emitter/emitter-features.c                      */
/************************************************************************/

#if TPP_EMITTER_HAVE_FEATURES
TPP_CONST_IMPL tpp_emitter_features const tpp_emitter_features_default = {
	/* .temf_flags = */ {
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
		/* .temff_NORMALIZE_SPACE                    = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_SPACE),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF)
		/* .temff_NORMALIZE_LF                       = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_LF),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
		/* .temff_NORMALIZE_C_STRING                 = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT)
		/* .temff_NORMALIZE_C_INT                    = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_C_INT),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
		/* .temff_NORMALIZE_KEYWORDS                 = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
		/* .temff_NORMALIZE_BSE                      = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_BSE),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
		/* .temff_NORMALIZE_TRIGRAPHS                = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
		/* .temff_NORMALIZE_DIGRAPHS                 = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
		/* .temff_NOLINE                             = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NOLINE),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN)
		/* .temff_RELAXED_MACRO_COLUMN               = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT)
		/* .temff_USE_CPP_DIGIT                      = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
		/* .temff_USE_CPP_DIGIT_FLAGS                = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
		/* .temff_USE_CPP_DIGIT_WORKING_DIRECTORY    = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
		/* .temff_REEMIT_MACRO_DEFINITIONS_LAZY      = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
		/* .temff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
		/* .temff_TRACE_INCLUDES                     = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_TRACE_INCLUDES),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
	}
};
#endif /* TPP_EMITTER_HAVE_FEATURES */

/************************************************************************/
/* File: parts/optional/emitter/emitter.c                               */
/************************************************************************/

#if TPP_EMITTER_HAVE_CURPOS
static TPP_NONNULL((1)) void TPPCALL
tpp_emitter_state_file_init(tpp_emitter_state_file *tpp_restrict self) {
	self->temsf_fname     = NULL;
	self->temsf_fname_str = NULL;
}

static TPP_NONNULL((1)) void TPPCALL
tpp_emitter_state_file_fini(tpp_emitter_state_file *tpp_restrict self) {
	if (self->temsf_fname_str)
		tpp_string_decref(self->temsf_fname_str);
	tpp_dbg_memset(self, sizeof(*self));
}

static TPP_NONNULL((1)) void TPPCALL
tpp_emitter_state_files_fini(tpp_emitter_state_files *tpp_restrict self) {
	tpp_emitter_state_file_fini(&self->temsfs_file);
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	{
		tpp_size i;
		for (i = 0; i < self->temsfs_filec; ++i)
			tpp_emitter_state_file_fini(&self->temsfs_filev[i]);
		tpp_free(self->temsfs_filev);
	}
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
}

#endif /* TPP_EMITTER_HAVE_CURPOS */

static TPP_NONNULL((1)) void TPPCALL
tpp_emitter_state_init(tpp_emitter_state *tpp_restrict self) {
#if TPP_EMITTER_HAVE_CURPOS
	tpp_lcstate_init(&self->tems_curfile.temsfs_file.temsf_curpos, 0, 0);
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	self->tems_cached_filec = 0;
	self->tems_cached_filev = NULL;
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
	tpp_emitter_state_file_init(&self->tems_curfile.temsfs_file);
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	self->tems_curfile.temsfs_filec = 0;
	self->tems_curfile.temsfs_filev = NULL;
#if _TPP_EMITTER_STATE_FLAGS_MASK
	self->tems_curfile.temsfs_file.temsf_flags = 0;
#endif /* _TPP_EMITTER_STATE_FLAGS_MASK */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#endif /* TPP_EMITTER_HAVE_CURPOS */
#if TPP_EMITTER_HAVE_FLAGS
	self->tems_flags = TPP_EMITTER_FLAG_NORMAL;
#endif /* TPP_EMITTER_HAVE_FLAGS */
	self->tems_prevtok = TPP_TOK_EOF;
}

static TPP_NONNULL((1)) void TPPCALL
tpp_emitter_state_fini(tpp_emitter_state *tpp_restrict self) {
#if TPP_EMITTER_HAVE_CURPOS
	tpp_emitter_state_files_fini(&self->tems_curfile);
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	tpp_free(self->tems_cached_filev);
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#endif /* TPP_EMITTER_HAVE_CURPOS */
	tpp_dbg_memset(self, sizeof(*self));
}

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given emitter.
 *
 * @param: output: Output printer. On error, must return one of `TPP_SSIZE_OFERR(*)`
 * @param: lexer:  The lexer whose tokens are being emitted
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: One of the default-enabled hooks could not be registered */
#ifndef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_init(tpp_emitter *tpp_restrict self,
                 tpp_lexer *tpp_restrict lexer,
                 tpp_formatprinter output)
#else /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_init(tpp_emitter *tpp_restrict self,
                  tpp_formatprinter output)
#endif /* TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
{
	tpp_errno result = TPP_EOK;
#ifndef TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER
	self->tem_lexer = lexer;
#endif /* !TPP_CONFIG_OFFSETOF_EMITTER_FROM_LEXER */
	self->tem_output = output;
	tpp_emitter_state_init(&self->tem_state);
#if TPP_EMITTER_HAVE_FEATURES
	tpp_emitter_features_init(&self->tem_feat);
#endif /* TPP_EMITTER_HAVE_FEATURES */
#if TPP_EMITTER_MODE_HAVE_MULTIPLE
	self->tem_mode = _TPP_EMITTER_MODE_DEFAULT;
#endif /* TPP_EMITTER_MODE_HAVE_MULTIPLE */
#if TPP_EMITTER_CONFIG_LINE_THRESHOLD < 0
	self->tem_linethreshold = -TPP_EMITTER_CONFIG_LINE_THRESHOLD;
#endif /* TPP_EMITTER_CONFIG_LINE_THRESHOLD < 0 */

	/* Register default hooks */
#if TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA > 0
	result = tpp_emitter_enable_reemit_unknown_pragma(self);
	if (TPP_ISERR(result))
		goto handle_error;
#define HAVE_handle_error
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA > 0 */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS > 0
	result = tpp_emitter_enable_reemit_macro_definitions(self);
	if (TPP_ISERR(result))
		goto handle_error;
#define HAVE_handle_error
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS > 0 */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES > 0
	result = tpp_emitter_enable_reemit_include_directives(self);
	if (TPP_ISERR(result))
		goto handle_error;
#define HAVE_handle_error
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES > 0 */
#if (TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) || \
     TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_TRACE_INCLUDES) ||                \
     TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS))
	result = _tpp_emitter_enable_file_pushed_hook(self); /* Must be turned on by default */
	if (TPP_ISERR(result))
		goto handle_error;
#define HAVE_handle_error
#endif /* ... */
#if TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
	result = _tpp_emitter_enable_file_popped_hook(self);
	if (TPP_ISERR(result))
		goto handle_error;
#define HAVE_handle_error
#endif /* TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
	return result;
#ifdef HAVE_handle_error
handle_error:
	tpp_emitter_fini(self);
	return result;
#endif /* HAVE_handle_error */
}

TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_emitter_fini(tpp_emitter *tpp_restrict self) {
	/* Clear lexer hooks */
	tpp_emitter_disable_reemit_unknown_pragma(self);
	tpp_emitter_disable_reemit_macro_definitions(self);
	tpp_emitter_disable_reemit_include_directives(self);
	_tpp_emitter_disable_file_pushed_hook(self);
	_tpp_emitter_disable_file_popped_hook(self);

	/* Finalize remainder of emitter state */
	tpp_emitter_state_fini(&self->tem_state);
#if TPP_EMITTER_HAVE_FEATURES
	tpp_emitter_features_fini(&self->tem_feat);
#endif /* TPP_EMITTER_HAVE_FEATURES */
	tpp_dbg_memset(self, sizeof(*self));
}



static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_emitter_printrepeat(tpp_emitter *tpp_restrict self,
                        tpp_size *tpp_restrict p_num_printed,
                        tpp_size count, tpp_char ch) {
	tpp_ssize temp, result = 0;
	tpp_char buffer[512];
	tpp_size bufmax = sizeof(buffer);
	if (bufmax > count)
		bufmax = count;
	tpp_memset(buffer, ch, bufmax * sizeof(tpp_char));
	*p_num_printed = 0;
	while (count) {
		temp = tpp_emitter_output_printraw(self, buffer, bufmax);
		if (temp < 0)
			goto err_temp;
		*p_num_printed += bufmax;
		result += temp;
		count -= bufmax;
		if (bufmax > count)
			bufmax = count;
	}
	return result;
err_temp:
	return temp;
}

/* Emit space characters (and update `self->tem_state`) */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_printspace(tpp_emitter *tpp_restrict self, tpp_column count) {
	tpp_size num_printed;
	tpp_ssize result = tpp_emitter_printrepeat(self, &num_printed, (tpp_size)count, ' ');
#if TPP_EMITTER_HAVE_CURPOS
	tpp_lcstate_setcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos,
	                   tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) +
	                   num_printed);
#endif /* TPP_EMITTER_HAVE_CURPOS */
	return result;
}

#if TPP_EMITTER_HAVE_CURPOS
#define tpp_emitter_getprinter(self) tpp_formatprinter_of(_tpp_emitter_print)
TPP_FORMATPRINTER_DEFINE(_tpp_emitter_print, arg, text, num_bytes) {
	tpp_emitter *self = (tpp_emitter *)arg;
	tpp_ssize result = tpp_emitter_output_printraw(self, text, num_bytes);
	if (result >= 0) {
		tpp_lcstate_account_ex(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos, text, num_bytes,
		                       tpp_file_getencoding(tpp_lexer_getfile(tpp_emitter_getlexer(self))));
	}
	return result;
}
#else /* TPP_EMITTER_HAVE_CURPOS */
#define tpp_emitter_getprinter(self) tpp_emitter_getoutput(self)
#endif /* !TPP_EMITTER_HAVE_CURPOS */
#define tpp_emitter_print(self, text, num_bytes) \
	tpp_formatprinter_print(tpp_emitter_getprinter(self), self, text, num_bytes)
#define tpp_emitter_print_cstr(self, cstr, num_bytes) \
	tpp_formatprinter_print_cstr(tpp_emitter_getprinter(self), self, cstr, num_bytes)
#define tpp_emitter_print_conststr(self, CONSTstr) \
	tpp_formatprinter_print_conststr(tpp_emitter_getprinter(self), self, CONSTstr)



/* Emit linefeed characters (and update `self->tem_state`) */
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) || TPP_EMITTER_HAVE_NORMALIZE_LF
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_printlf(tpp_emitter *tpp_restrict self, tpp_line count) {
	tpp_size num_printed;
	tpp_ssize result = tpp_emitter_printrepeat(self, &num_printed, (tpp_size)count, '\n');
#if TPP_EMITTER_HAVE_CURPOS
	tpp_lcstate_init(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos,
	                 tpp_lcstate_getline(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) + num_printed,
	                 num_printed ? 0 : tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos));
#endif /* TPP_EMITTER_HAVE_CURPOS */
	return result;
}
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) || TPP_EMITTER_HAVE_NORMALIZE_LF */


#if TPP_EMITTER_HAVE_NORMALIZE_C_STRING || (TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE))
#if TPP_HAVE_TOKEN_ENCODESTRING
#define tpp_emitter_print_encodestring_call(self, text, num_bytes) \
	tpp_token_encodestring(tpp_emitter_getprinter(self), self, text, num_bytes)
#else /* TPP_HAVE_TOKEN_ENCODESTRING */
#define tpp_emitter_print_encodestring_call(self, text, num_bytes) \
	tpp_emitter_output_printraw(self, text, num_bytes)
#endif /* !TPP_HAVE_TOKEN_ENCODESTRING */

TPP_FORMATPRINTER_DEFINE(tpp_emitter_print_encodestring, arg, text, num_bytes) {
	return tpp_emitter_print_encodestring_call((tpp_emitter *)arg, text, num_bytes);
}
#endif /* TPP_EMITTER_HAVE_NORMALIZE_C_STRING || (TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)) */


#if TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
/* Emit a `#line` directive (and update `self->tem_state`) */
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_USE_CPP_DIGIT)
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_print_line_directive(tpp_emitter *tpp_restrict self,
                                 tpp_line line, char const *filename,
                                 tpp_string *filename_str) {
	tpp_ssize temp, result = 0;
	char buffer[(sizeof("\n#line ") - sizeof(char)) + TPP_ITOA_MAXLEN + 2];
	char *ptr = buffer, *buf_temp;
	tpp_size partlen;
	tpp_column oldcol = tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos);
	char const *emit_filename = filename;
	if (self->tem_state.tems_curfile.temsfs_file.temsf_fname == filename)
		emit_filename = NULL;
	if (oldcol != 0)
		*ptr++ = '\n';
	*ptr++ = '#';
	*ptr++ = 'l';
	*ptr++ = 'i';
	*ptr++ = 'n';
	*ptr++ = 'e';
	*ptr++ = ' ';
	buf_temp = tpp_itoa(ptr, line + 1);
	partlen = (tpp_size)(ptr + TPP_ITOA_MAXLEN - buf_temp);
	tpp_memmovedown(ptr, buf_temp, partlen * sizeof(char));
	ptr += partlen;
	if (emit_filename) {
		*ptr++ = ' ';
		*ptr++ = '"';
	} else {
		*ptr++ = '\n';
	}
	temp = tpp_emitter_output_printraw_cstr(self, buffer, (tpp_size)(ptr - buffer));
	if (temp < 0)
		goto err_temp;
	result += temp;
	if (emit_filename) {
		partlen = tpp_strlen(emit_filename);
		temp = tpp_emitter_print_encodestring_call(self, (tpp_char const *)emit_filename, partlen);
		if (temp < 0) {
			tpp_lcstate_init(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos,
			                 tpp_lcstate_getline(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) +
			                 (oldcol ? 1 : 0),
			                 (tpp_size)(ptr - buffer));
			goto err_temp;
		}
		result += temp;
		temp = tpp_emitter_output_printraw_conststr(self, "\"\n");
		if (temp < 0) {
			tpp_lcstate_setcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos,
			                   (tpp_size)(ptr - buffer) + partlen);
			goto err_temp;
		}
		result += temp;
	}

	/* Remember the newly assigned filename */
	self->tem_state.tems_curfile.temsfs_file.temsf_fname = filename;
	if (filename_str != self->tem_state.tems_curfile.temsfs_file.temsf_fname_str) {
		if (filename_str)
			tpp_string_incref(filename_str);
		if (self->tem_state.tems_curfile.temsfs_file.temsf_fname_str)
			tpp_string_decref(self->tem_state.tems_curfile.temsfs_file.temsf_fname_str);
		self->tem_state.tems_curfile.temsfs_file.temsf_fname_str = filename_str;
	}
	tpp_lcstate_init(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos, line, 0);
	return result;
err_temp:
	return temp;
}
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_USE_CPP_DIGIT) */

#if !TPP_EMITTER_HAVE_USE_CPP_DIGIT
#define tpp_emitter_pushed_files_changed(self, lcfile) false
#else /* TPP_EMITTER_HAVE_USE_CPP_DIGIT */

#define tpp_emitter_print_cpp_digit_pushfile(self, ent, filename) \
	tpp_emitter_print_cpp_digit_applyfile(self, ent, filename, '1')
#define tpp_emitter_print_cpp_digit_popfile(self, ent, filename) \
	tpp_emitter_print_cpp_digit_applyfile(self, ent, filename, '2')
#define tpp_emitter_print_cpp_digit_setfile(self, ent, filename) \
	tpp_emitter_print_cpp_digit_applyfile(self, ent, filename, '\0')
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_emitter_print_cpp_digit_applyfile_(tpp_emitter *tpp_restrict self,
                                       tpp_emitter_state_file const *tpp_restrict ent,
                                       char const *filename
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
                                       , char flag
#define tpp_emitter_print_cpp_digit_applyfile(self, ent, filename, flag) \
	tpp_emitter_print_cpp_digit_applyfile_(self, ent, filename, flag)
#else /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
#define tpp_emitter_print_cpp_digit_applyfile(self, ent, filename, flag) \
	tpp_emitter_print_cpp_digit_applyfile_(self, ent, filename)
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
                                       ) {
	tpp_ssize temp, result;
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	char buf[sizeof("#  \"2 3 4\n") + TPP_ITOA_MAXLEN - sizeof(char)];
#else /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
	char buf[sizeof("#  \"2\n") + TPP_ITOA_MAXLEN - sizeof(char)];
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
	char *buf_temp, *ptr = buf;
	tpp_size partlen;
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#if _TPP_EMITTER_STATE_FLAGS_MASK
	if (!filename && (flag || ent->temsf_flags))
#else /* _TPP_EMITTER_STATE_FLAGS_MASK */
	if (!filename && flag)
#endif /* !_TPP_EMITTER_STATE_FLAGS_MASK */
	{
		/* Flags must be set -- need a filename */
		filename = ent->temsf_fname;
		if (filename == NULL)
			filename = "?"; /* Could happen if underlying file has no name... */
	}
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */

	*ptr++ = '#';
	*ptr++ = ' ';
	buf_temp = tpp_itoa(ptr, tpp_lcstate_getline(&ent->temsf_curpos) + 1);
	partlen = (tpp_size)(ptr + TPP_ITOA_MAXLEN - buf_temp);
	tpp_memmovedown(ptr, buf_temp, partlen * sizeof(char));
	ptr += partlen;
	if (filename) {
		*ptr++ = ' ';
		*ptr++ = '\"';
	} else {
		*ptr++ = '\n';
	}
	result = tpp_emitter_output_printraw_cstr(self, buf, (tpp_size)(ptr - buf));
	if (filename && result >= 0) {
		temp = tpp_emitter_print_encodestring_call(self, (tpp_char const *)filename, tpp_strlen(filename));
		if (temp < 0)
			return temp;
		result += temp;
		ptr = buf;
		*ptr++ = '"';
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
		if (flag) {
			*ptr++ = ' ';
			*ptr++ = flag;
		}
#if TPP_HAVE_FILE_SYSHDR
		if (ent->temsf_flags & TPP_FILE_FLAGS_SYSHDR) {
			*ptr++ = ' ';
			*ptr++ = '3';
		}
#endif /* TPP_HAVE_FILE_SYSHDR */
#if TPP_HAVE_FILE_EXTERN_C
		if (ent->temsf_flags & TPP_FILE_FLAGS_EXTERN_C) {
			*ptr++ = ' ';
			*ptr++ = '4';
		}
#endif /* TPP_HAVE_FILE_EXTERN_C */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
		*ptr++ = '\n';
		temp = tpp_emitter_output_printraw_cstr(self, buf, (tpp_size)(ptr - buf));
		if (temp < 0)
			return temp;
		result += temp;
	}
	tpp_lcstate_setcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos, 0);
	return result;
}

#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_print_cpp_digit_working_directory(tpp_emitter *tpp_restrict self,
                                              tpp_line line) {
	tpp_ssize temp, result;
	char buf[sizeof("#  \"") + TPP_ITOA_MAXLEN - sizeof(char)];
	char *buf_temp, *ptr = buf;
	tpp_size partlen;
	tpp_formatprinter printer;
	*ptr++ = '#';
	*ptr++ = ' ';
	buf_temp = tpp_itoa(ptr, tpp_lcinfo_getline(line) + 1);
	partlen = (tpp_size)(ptr + TPP_ITOA_MAXLEN - buf_temp);
	tpp_memmovedown(ptr, buf_temp, partlen * sizeof(char));
	ptr += partlen;
	*ptr++ = ' ';
	*ptr++ = '\"';
	result = tpp_emitter_output_printraw_cstr(self, buf, (tpp_size)(ptr - buf));
	if (result < 0)
		return result;
	printer = tpp_formatprinter_of(tpp_emitter_print_encodestring);
	/* Print the lexer's USERPWD override instead of the process's *actual* PWD */
#if TPP_HAVE_LEXER_USERPWD
	if (tpp_lexer_hasuserpwd(tpp_emitter_getlexer(self))) {
		temp = tpp_lexer_printuserpwd(tpp_emitter_getlexer(self), printer, self);
	} else
#endif /* TPP_HAVE_LEXER_USERPWD */
	{
		temp = tpp_io_printpwd(printer, self);
	}
	if (temp < 0)
		return temp;
	result += temp;
	temp = tpp_emitter_output_printraw_conststr(self, "//\"\n");
	if (temp < 0)
		return temp;
	result += temp;
	return result;
}
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */

#if !TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
#define tpp_emitter_pushed_files_changed(self, lcfile) false
#else /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
static TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_prevlc(tpp_file *file) {
	do {
		file = tpp_file_getprev(file);
	} while (file && (tpp_file_getkind(file) != TPP_FILE_KIND_IO &&
	                  tpp_file_getkind(file) != TPP_FILE_KIND_TEXT));
	return file;
}

/* Check if the stack of files pushed by  */
static TPP_WUNUSED TPP_PURECALL TPP_NONNULL((1, 2)) bool TPPCALL
tpp_emitter_pushed_files_changed(tpp_emitter *tpp_restrict self,
                                 tpp_file *tpp_restrict lcfile) {
	tpp_size i;
	if (!tpp_emitter_has(self, USE_CPP_DIGIT))
		return false;
	if (!tpp_emitter_has(self, USE_CPP_DIGIT_FLAGS))
		return false;
#if _TPP_EMITTER_STATE_FLAGS_MASK
	/* This one must be checked *before* "tems_fchanged" in case of "#pragma GCC system_header" */
	if ((self->tem_state.tems_curfile.temsfs_file.temsf_flags) !=
	    (tpp_file_getflags(lcfile) & _TPP_EMITTER_STATE_FLAGS_MASK))
		return true;
#endif /* _TPP_EMITTER_STATE_FLAGS_MASK */
	if (!(self->tem_state.tems_flags & TPP_EMITTER_FLAG_FCHANGED))
		return false;

	/* Check file-stack for changes. */
	for (i = self->tem_state.tems_curfile.temsfs_filec; i--;) {
		char const *actual_filename;
		tpp_lcinfo actual_lcinfo;
		tpp_emitter_state_file *expected;
		lcfile = tpp_file_prevlc(lcfile);
		if (lcfile == NULL)
			return true; /* Stack became smaller (file popped) */
		expected = &self->tem_state.tems_curfile.temsfs_filev[i];
		actual_filename = tpp_file_getfilename(lcfile);
		if (expected->temsf_fname != actual_filename) {
			if (expected->temsf_fname && actual_filename &&
			    tpp_strcmp(expected->temsf_fname, actual_filename) == 0) {
				tpp_string *actual_string = tpp_file_getfilenamestr(lcfile);
				expected->temsf_fname = actual_filename;
				if (actual_string)
					tpp_string_incref(actual_string);
				if (expected->temsf_fname_str)
					tpp_string_decref(expected->temsf_fname_str);
				expected->temsf_fname = actual_filename;
				expected->temsf_fname_str = actual_string;
			} else {
				return true; /* Stack changed: filename */
			}
		}

#if _TPP_EMITTER_STATE_FLAGS_MASK
		if (expected->temsf_flags != (tpp_file_getflags(lcfile) & _TPP_EMITTER_STATE_FLAGS_MASK))
			return true; /* Stack changed: flags */
#endif /* _TPP_EMITTER_STATE_FLAGS_MASK */
		actual_lcinfo = tpp_file_getstartlcinfo(lcfile);
		if (tpp_lcstate_getline(&expected->temsf_curpos) !=
		    tpp_lcinfo_getline(actual_lcinfo))
			return true; /* Stack changed: line */
	}
	if (tpp_file_prevlc(lcfile) != NULL)
		return true; /* Stack became larger (file pushed) */

	/* Nothing actually changed. */
	self->tem_state.tems_flags &= ~TPP_EMITTER_FLAG_FCHANGED;
	return false;
}

/* Capture the state of the #include-stack */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_state_files_capture(tpp_emitter_state_files *tpp_restrict self,
                                tpp_file *tpp_restrict lcfile, tpp_lcinfo lcinfo,
                                char const *filename, tpp_string *filename_str, tpp_size cached_filec,
                                /*inherit(always)*/tpp_emitter_state_file *cached_filev) {
	tpp_size i, extra_file_count;
	tpp_file *lciter;
	tpp_lcstate_initlc(&self->temsfs_file.temsf_curpos, lcinfo);
	self->temsfs_file.temsf_fname = filename;
	self->temsfs_file.temsf_fname_str = filename_str;
	if (filename_str)
		tpp_string_incref(filename_str);
#if _TPP_EMITTER_STATE_FLAGS_MASK
	self->temsfs_file.temsf_flags = tpp_file_getflags(lcfile) & _TPP_EMITTER_STATE_FLAGS_MASK;
#endif /* _TPP_EMITTER_STATE_FLAGS_MASK */

	extra_file_count = 0;
	for (lciter = lcfile; (lciter = tpp_file_prevlc(lciter)) != NULL;)
		++extra_file_count;
	self->temsfs_filec = extra_file_count;
	self->temsfs_filev = cached_filev;
#ifndef __OPTIMIZE_SIZE__
	if (extra_file_count < cached_filec) {
		tpp_emitter_state_file *newbuf;
		newbuf = (tpp_emitter_state_file *)tpp_tryrealloc(cached_filev,
		                                                  (extra_file_count ? extra_file_count : 1) *
		                                                  sizeof(tpp_emitter_state_file));
		if (newbuf)
			self->temsfs_filev = newbuf;
	} else
#endif /* !__OPTIMIZE_SIZE__ */
	if (extra_file_count > cached_filec) {
		tpp_emitter_state_file *newbuf;
		newbuf = (tpp_emitter_state_file *)tpp_realloc(cached_filev,
			                                           extra_file_count *
			                                           sizeof(tpp_emitter_state_file));
		if tpp_unlikely(!newbuf)
			goto err_nomem;
		self->temsfs_filev = newbuf;
	}

	/* Populate file metadata entries. */
	for (lciter = lcfile, i = extra_file_count; (lciter = tpp_file_prevlc(lciter)) != NULL;) {
		tpp_emitter_state_file *ent;
		tpp_lcinfo lcinfo_iter;
		tpp_assert(i != 0);
		--i;
		ent = &self->temsfs_filev[i];
		lcinfo_iter = tpp_file_getstartlcinfo(lciter);
		tpp_lcstate_initlc(&ent->temsf_curpos, lcinfo_iter);
		ent->temsf_fname = tpp_file_getfilename(lciter);
		ent->temsf_fname_str = tpp_file_getfilenamestr(lciter);
		if (ent->temsf_fname_str)
			tpp_string_incref(ent->temsf_fname_str);
#if _TPP_EMITTER_STATE_FLAGS_MASK
		ent->temsf_flags = tpp_file_getflags(lciter) & _TPP_EMITTER_STATE_FLAGS_MASK;
#endif /* _TPP_EMITTER_STATE_FLAGS_MASK */
	}
	tpp_assert(i == 0);

	return TPP_EOK;
err_nomem:
	tpp_emitter_state_files_fini(self);
	return TPP_ENOMEM;
}

static TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) tpp_emitter_state_file const *TPPCALL
tpp_emitter_state_files_getfile(tpp_emitter_state_files const *tpp_restrict self,
                                tpp_size index) {
	tpp_assert(index <= self->temsfs_filec + 1);
	if (index < self->temsfs_filec)
		return &self->temsfs_filev[index];
	return &self->temsfs_file;
}

/* Print directives to migrate from the file-state of "self" to that of "new_files" */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_emitter_print_files_diff(tpp_emitter *tpp_restrict self,
                             tpp_emitter_state_files const *tpp_restrict new_files) {
#if TPP_DEBUG
	tpp_ssize delta = 0;
#endif /* TPP_DEBUG */
	tpp_ssize temp, result = 0;
	tpp_emitter_state_file const *oldent, *newent;
	tpp_emitter_state_files const *const old_files = &self->tem_state.tems_curfile;
	tpp_size num_identical, i, pop_until;
	tpp_size const old_file_count = old_files->temsfs_filec + 1;
	tpp_size const new_file_count = new_files->temsfs_filec + 1;
	tpp_size const num_common = old_file_count < new_file_count ? old_file_count : new_file_count;
	for (num_identical = 0; num_identical < num_common; ++num_identical) {
		oldent = tpp_emitter_state_files_getfile(old_files, num_identical);
		newent = tpp_emitter_state_files_getfile(new_files, num_identical);
		if (oldent->temsf_fname != newent->temsf_fname) {
			if (oldent->temsf_fname && newent->temsf_fname &&
			    tpp_strcmp(oldent->temsf_fname, newent->temsf_fname) == 0)
				((tpp_emitter_state_file *)oldent)->temsf_fname = newent->temsf_fname;
			if (oldent->temsf_fname != newent->temsf_fname)
				break;
		}
		if (tpp_lcstate_getline(&oldent->temsf_curpos) !=
		    tpp_lcstate_getline(&newent->temsf_curpos))
			break;
#if _TPP_EMITTER_STATE_FLAGS_MASK
		if (oldent->temsf_flags != newent->temsf_flags)
			break;
#endif /* _TPP_EMITTER_STATE_FLAGS_MASK */
	}

	/* Must emit directives to transform
	 * `old_files[num_identical:old_file_count]` into
	 * `new_files[num_identical:new_file_count]` */
	tpp_assert(num_identical <= old_file_count);
	tpp_assert(num_identical <= new_file_count);

	/* Emit directives to pop files */
	pop_until = num_identical + 1;
	if (new_file_count == num_identical)
		--pop_until;
	for (i = old_file_count - 1; i > pop_until;) {
		oldent = tpp_emitter_state_files_getfile(old_files, --i);
		temp = tpp_emitter_print_cpp_digit_popfile(self, oldent, oldent->temsf_fname);
		if (temp < 0)
			goto err_temp;
		result += temp;
#if TPP_DEBUG
		--delta;
#endif /* TPP_DEBUG */
	}

	/* Emit directives to re-adjust the last shared file */
	newent = tpp_emitter_state_files_getfile(new_files, num_identical);
	if (i > num_identical || i == new_file_count) {
		temp = tpp_emitter_print_cpp_digit_popfile(self, newent, newent->temsf_fname);
#if TPP_DEBUG
		--delta;
#endif /* TPP_DEBUG */
	} else if (old_file_count == 1 && /* Must emit a set-directive if this is the first one... */
	           (num_identical == 0 || !(self->tem_state.tems_flags & TPP_EMITTER_FLAG_HASLINE))) {
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
		if (!(self->tem_state.tems_flags & TPP_EMITTER_FLAG_HASLINE) &&
			tpp_emitter_has(self, USE_CPP_DIGIT_WORKING_DIRECTORY)) {
			tpp_line line;
			temp = tpp_emitter_print_cpp_digit_setfile(self, newent, newent->temsf_fname);
			if (temp < 0)
				goto err_temp;
			result += temp;
			line = tpp_lcstate_getline(&newent->temsf_curpos);
			temp = tpp_emitter_print_cpp_digit_working_directory(self, line);
			if (temp < 0)
				goto err_temp;
			result += temp;
		}
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */
		oldent = tpp_emitter_state_files_getfile(old_files, num_identical);
		temp = tpp_emitter_print_cpp_digit_setfile(self, newent,
		                                           (oldent->temsf_fname == newent->temsf_fname
		                                            ? NULL
		                                            : newent->temsf_fname));
		++i;
	} else {
		temp = 0; /* Files are about to be pushed, so no need for a line-setter */
		++i;
	}
	if (temp < 0)
		goto err_temp;
	result += temp;

	/* Emit directives to push files */
	for (; i < new_file_count; ++i) {
		newent = tpp_emitter_state_files_getfile(new_files, i);
		temp = tpp_emitter_print_cpp_digit_pushfile(self, newent, newent->temsf_fname);
		if (temp < 0)
			goto err_temp;
		result += temp;
#if TPP_DEBUG
		++delta;
#endif /* TPP_DEBUG */
	}

#if TPP_DEBUG
	tpp_assert((old_file_count + delta) == new_file_count);
#endif /* TPP_DEBUG */

	return result;
err_temp:
	return temp;
}
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_ssize TPPCALL
tpp_emitter_print_cpp_digit_directive(tpp_emitter *tpp_restrict self,
                                      tpp_file *tpp_restrict lcfile, tpp_line line,
                                      char const *filename, tpp_string *filename_str) {
	tpp_ssize temp, result = 0;
	(void)lcfile;
	/* Ensure that the emitter's output is placed at the start of a line.
	 * This can always simply be done at the very start since by the time
	 * we get here, we know that we *have* to emit *at least* 1 directive! */
	if (tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) != 0) {
		result = tpp_emitter_output_printraw_conststr(self, "\n");
		if (result < 0)
			return result;
		tpp_lcstate_init(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos,
		                 tpp_lcstate_getline(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) + 1,
		                 0);
	}

#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	if (!tpp_emitter_has(self, USE_CPP_DIGIT_FLAGS) ||
	    !(self->tem_state.tems_flags & TPP_EMITTER_FLAG_FCHANGED))
#else /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
	if (1)
#endif /* !TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
	{
		/* Simple case: files didn't change, so we
		 * don't have to (re-)build file tables! */
		char const *emit_filename = filename;
		tpp_emitter_state_file *ent;
		ent = &self->tem_state.tems_curfile.temsfs_file;
		if (ent->temsf_fname == filename)
			emit_filename = NULL;
		tpp_lcstate_setline(&ent->temsf_curpos, line);
		ent->temsf_fname = filename;
		if (ent->temsf_fname_str)
			tpp_string_decref(ent->temsf_fname_str);
		ent->temsf_fname_str = filename_str;
		if (filename_str)
			tpp_string_incref(filename_str);
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS && _TPP_EMITTER_STATE_FLAGS_MASK
		ent->temsf_flags = tpp_file_getflags(lcfile) & _TPP_EMITTER_STATE_FLAGS_MASK;
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS && _TPP_EMITTER_STATE_FLAGS_MASK */
		temp = tpp_emitter_print_cpp_digit_setfile(self, ent, emit_filename);
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
		if (temp >= 0 && !(self->tem_state.tems_flags & TPP_EMITTER_FLAG_HASLINE) &&
			tpp_emitter_has(self, USE_CPP_DIGIT_WORKING_DIRECTORY)) {
			result += temp;
			tpp_assert(tpp_lcstate_getline(&ent->temsf_curpos) == line);
			temp = tpp_emitter_print_cpp_digit_working_directory(self, line);
			if (temp < 0)
				return temp;
			result += temp;
			temp = tpp_emitter_print_cpp_digit_setfile(self, ent, filename);
		}
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */
	} else {
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
		tpp_errno error;
		tpp_emitter_state_files newfiles;
		tpp_size i, cached_filec = self->tem_state.tems_cached_filec;
		tpp_emitter_state_file *cached_filev = self->tem_state.tems_cached_filev;
		self->tem_state.tems_cached_filec = 0;
		self->tem_state.tems_cached_filev = NULL;

		/* Capture current file-state of lexer. */
		error = tpp_emitter_state_files_capture(&newfiles, lcfile,
		                                        tpp_lcinfo_of(line, 0),
		                                        filename, filename_str,
		                                        cached_filec, cached_filev);
		if (TPP_ISERR(error))
			return TPP_SSIZE_OFERR(error);

		/* Emit directives to migrate from previous
		 * file-state to the one just-captured. */
		temp = tpp_emitter_print_files_diff(self, &newfiles);

		/* Save old state-buffer as new cache */
		tpp_emitter_state_file_fini(&self->tem_state.tems_curfile.temsfs_file);
		for (i = 0; i < self->tem_state.tems_curfile.temsfs_filec; ++i)
			tpp_emitter_state_file_fini(&self->tem_state.tems_curfile.temsfs_filev[i]);
		self->tem_state.tems_cached_filec = self->tem_state.tems_curfile.temsfs_filec;
		self->tem_state.tems_cached_filev = self->tem_state.tems_curfile.temsfs_filev;

		/* Remember new file-state. */
		self->tem_state.tems_curfile = newfiles;
		self->tem_state.tems_flags &= ~TPP_EMITTER_FLAG_FCHANGED;
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */
	}
	if (temp < 0)
		return temp;
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY
	self->tem_state.tems_flags |= TPP_EMITTER_FLAG_HASLINE;
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS || TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY */
	result += temp;
	return result;
}
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT */
#endif /* TPP_EMITTER_HAVE_MODE_EMIT && TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */


#if TPP_EMITTER_HAVE_NORMALIZE_C_STRING
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
struct tpp_emitter_printbig_data {
	tpp_emitter *tepbd_emitter; /* [1..1] Emitter */
	tpp_char     tepbd_quote;   /* Used "-character */
	bool         tepbd_after_x; /* True if after \x-sequence (meaning next regular byte mustn't be 0-9, a-f, A-F) */
};

TPP_FORMATPRINTER_DEFINE(tpp_emitter_printbig_normal, arg, text, num_bytes) {
	tpp_ssize temp, result = 0;
	struct tpp_emitter_printbig_data *data;
	data = (struct tpp_emitter_printbig_data *)arg;
	if (!num_bytes)
		return 0;
	if (data->tepbd_after_x) {
		if (tpp_ascii_isxdigit(*text)) {
			tpp_char seq[3];
			seq[0] = data->tepbd_quote;
			seq[1] = ' ';
			seq[2] = data->tepbd_quote;
			result = tpp_emitter_print(data->tepbd_emitter, seq, 3);
			if (result < 0)
				return result;
		}
		data->tepbd_after_x = false;
	}
	temp = tpp_emitter_print_encodestring_call(data->tepbd_emitter, text, num_bytes);
	if (temp < 0)
		return temp;
	result += temp;
	return result;
}

static tpp_ssize TPPCALL
tpp_emitter_printbig_big(void *arg, tpp_lexer *tpp_restrict lexer, tpp_uintmax value) {
#if TPP_UINTMAX_MAX <= TPP_UINTMAX_C(0xffff)
	tpp_char seq[sizeof("\\xFFFF") - sizeof(char)];
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(0xffffffff)
	tpp_char seq[sizeof("\\xFFFFFFFF") - sizeof(char)];
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(0xffffffffffffffff)
	tpp_char seq[sizeof("\\xFFFFFFFFFFFFFFFF") - sizeof(char)];
#else /* TPP_UINTMAX_MAX <= ... */
	tpp_char seq[sizeof("\\xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF") - sizeof(char)];
#endif /* TPP_UINTMAX_MAX < ... */
	tpp_char *seq_dst = seq + sizeof(seq);
	struct tpp_emitter_printbig_data *data;
	data = (struct tpp_emitter_printbig_data *)arg;
	do {
		tpp_char nibble = value & 0xf;
		*--seq_dst = tpp_ascii_ofuprxdigit(nibble);
		value >>= 4;
	} while (value);
	*--seq_dst = 'x';
	*--seq_dst = '\\';
	data->tepbd_after_x = true;
	(void)lexer;
	return tpp_emitter_print(data->tepbd_emitter, seq_dst, (tpp_size)(seq + sizeof(seq) - seq_dst));
}
#endif /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
#endif /* TPP_EMITTER_HAVE_NORMALIZE_C_STRING */


#if !TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS
#define tpp_emitter_print_keyword tpp_emitter_print
#else /* TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_print_keyword(tpp_emitter *tpp_restrict self,
                          tpp_char const *text, size_t num_bytes) {
	tpp_ssize temp, result = 0;
	tpp_char const *end = text + num_bytes;
	tpp_char const *iter = text;
	bool is_first = true;
	if (iter >= end) {
		/* Special case: *empty* keyword */
		return tpp_emitter_print_conststr(self, "__TPP_IDENTIFIER(\"\")");
	}
	do {
		tpp_char const *uc_start = iter;
#if TPP_HAVE_UNICODE
		tpp_unichar uc = tpp_unicode_readutf8(&iter, end);
		if (is_first ? !tpp_unicode_issymstrt(uc)
		             : !tpp_unicode_issymcont(uc))
#else /* TPP_HAVE_UNICODE */
		tpp_char ch = *iter++;
		if (is_first ? !tpp_ascii_issymstrt(ch)
		             : !tpp_ascii_issymcont(ch))
#endif /* !TPP_HAVE_UNICODE */
		{
			/* Must escape! */
			tpp_char buf[sizeof("\\U12345678") - sizeof(char)], *dst = buf;
			temp = tpp_emitter_print(self, text, (tpp_size)(uc_start - text));
			if (temp < 0)
				goto err_temp;
			result += temp;
			*dst++ = '\\';
			*dst++ = 'U';
#if TPP_HAVE_UNICODE
			*dst++ = tpp_ascii_touprxdigit((uc & 0xf0000000) >> 28);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x0f000000) >> 24);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x00f00000) >> 20);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x000f0000) >> 16);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x0000f000) >> 12);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x00000f00) >> 8);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x000000f0) >> 4);
			*dst++ = tpp_ascii_touprxdigit((uc & 0x0000000f));
#else /* TPP_HAVE_UNICODE */
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = '0';
			*dst++ = tpp_ascii_touprxdigit((ch & 0xf0) >> 4);
			*dst++ = tpp_ascii_touprxdigit((ch & 0x0f));
#endif /* !TPP_HAVE_UNICODE */
			text = iter;
			temp = tpp_emitter_print(self, buf, sizeof(buf));
			if (temp < 0)
				goto err_temp;
			result += temp;
		}
		is_first = false;
	} while (iter < end);

	/* Flush remainder */
	temp = tpp_emitter_print(self, text, (tpp_size)(end - text));
	if (temp < 0)
		goto err_temp;
	result += temp;
	return result;
err_temp:
	return temp;
}
#endif /* TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */


#if !TPP_EMITTER_HAVE_NORMALIZE_BSE && !TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS
#define tpp_emitter_print_generic tpp_emitter_print
#else /* !TPP_EMITTER_HAVE_NORMALIZE_BSE && !TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_print_generic(tpp_emitter *tpp_restrict self,
                          tpp_char const *text, size_t num_bytes) {
	tpp_ssize temp, result = 0;
	tpp_char const *end = text + num_bytes;
	tpp_char const *iter = text;
	while (iter < end) {
#if TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS
		if (iter[0] == '?' && (iter + 2) < end && iter[1] == '?' &&
		    tpp_emitter_has(self, NORMALIZE_TRIGRAPHS)) {
			tpp_char trich;
			switch (iter[2]) {
			case '=': trich = '#'; break;
			case '(': trich = '['; break;
			case '/': trich = '\\'; break;
			case ')': trich = ']'; break;
			case '\'': trich = '^'; break;
			case '<': trich = '{'; break;
			case '!': trich = '|'; break;
			case '>': trich = '}'; break;
			case '-': trich = '~'; break;
			default: goto not_a_trigraph;
			}
			temp = tpp_emitter_print(self, text, (tpp_size)(iter - text));
			if (temp < 0)
				goto err_temp;
			result += temp;
			temp = tpp_emitter_print(self, &trich, 1);
			if (temp < 0)
				goto err_temp;
			result += temp;
			iter += 3;
			text = iter;
		}
not_a_trigraph:
#endif /* TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */

#if TPP_EMITTER_HAVE_NORMALIZE_BSE
		if (tpp_emitter_has(self, NORMALIZE_BSE)) {
			tpp_char const *new_iter;
			new_iter = tpp_preparse_skipbse_fwd(tpp_emitter_getlexer(self), iter, end);
			tpp_assert(new_iter >= iter);
			if (new_iter > iter) {
				temp = tpp_emitter_print(self, text, (tpp_size)(iter - text));
				if (temp < 0)
					goto err_temp;
				result += temp;
				iter = text = new_iter;
			}
		}
#endif /* TPP_EMITTER_HAVE_NORMALIZE_BSE */
		++iter;
	}
	temp = tpp_emitter_print(self, text, (tpp_size)(end - text));
	if (temp < 0)
		goto err_temp;
	result += temp;
	return result;
err_temp:
	return temp;
}
#endif /* TPP_EMITTER_HAVE_NORMALIZE_BSE || TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */


/* Emit the currently loaded token (and update `self->tem_state`) */
#if !TPP_EMITTER_HAVE_MODE_EMIT
#define tpp_emitter_print_current_token(self)                                      \
	tpp_emitter_print_generic(self,                                                \
	                          tpp_lexer_gettokenstart(tpp_emitter_getlexer(self)), \
	                          tpp_lexer_gettokenlen(tpp_emitter_getlexer(self)))
#else /* !TPP_EMITTER_HAVE_MODE_EMIT */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_print_current_token(tpp_emitter *tpp_restrict self) {
	tpp_lexer *const lexer = tpp_emitter_getlexer(self);
	tpp_char const *token_start = tpp_lexer_gettokenstart(lexer);
	tpp_char const *token_end = tpp_lexer_gettokenend(lexer);

	/* Configs to normalize certain tokens (see "emitter.h") */
	switch (tpp_lexer_gettok(lexer)) {

#if TPP_EMITTER_HAVE_NORMALIZE_SPACE
	case TPP_TOK_SPACE: {
		tpp_size space_count;
		if (!tpp_emitter_has(self, NORMALIZE_SPACE))
			break;
		space_count = 0;
		while (token_start < token_end) {
			tpp_char ch = *token_start++;
			(void)ch;
			++space_count;
#if TPP_HAVE_UNICODE
			if (tpp_ascii_ismb(ch) && tpp_file_isutf8(tpp_lexer_getfile(lexer)))
				token_start += tpp_unicode_utf8seqlen_mb_getmax(ch) - 1;
#endif /* TPP_HAVE_UNICODE */
			token_start = tpp_preparse_skipbse_fwd(lexer, token_start, token_end);
		}
		return tpp_emitter_printspace(self, (tpp_column)space_count);
	}	break;
#endif /* TPP_EMITTER_HAVE_NORMALIZE_SPACE */

#if TPP_EMITTER_HAVE_NORMALIZE_LF
	case TPP_TOK_LF: {
		if (!tpp_emitter_has(self, NORMALIZE_LF))
			break;
		return tpp_emitter_printlf(self, 1);
	}	break;
#endif /* TPP_EMITTER_HAVE_NORMALIZE_LF */

#if TPP_EMITTER_HAVE_NORMALIZE_C_STRING
	TPP_CASE_TPP_TOK_STRING {
		tpp_lexer_decodestring_config config;
		tpp_ssize temp, result;
		tpp_char quote;
		if (!tpp_emitter_has(self, NORMALIZE_C_STRING))
			break;
#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
		if (TPP_TOK_ISSTRING_SQUOTE(tpp_lexer_gettok(lexer)) &&
		    tpp_lexer_has(lexer, BUILTIN_EXPR_CHARACTER_LITERALS)) {
			quote = '\'';
		} else
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
		{
			quote = '"';
		}

		/* Emit string encoding prefix. */
		result = 0;
		switch (tpp_lexer_gettok(lexer)) {
#if TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL
		_TPP_CASE_TPP_TOK_CXX_WIDE_STRING_LITERAL
		_TPP_CASE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
		_TPP_CASE_TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL
		_TPP_CASE_TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL
#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
			if (quote != '"') {
				if (!tpp_lexer_has(lexer, TOK_CXX_WIDE_CHAR_LITERAL))
					goto print_generic_string;
			} else
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
			{
				if (!tpp_lexer_has(lexer, TOK_CXX_WIDE_STRING_LITERAL))
					goto print_generic_string;
			}
			result = tpp_emitter_print_conststr(self, "L");
			if (result < 0)
				return result;
			break;
#endif /* TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL || TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL */

#if TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL
		_TPP_CASE_TPP_TOK_CXX_UTF8_STRING_LITERAL
		_TPP_CASE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
		_TPP_CASE_TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL
		_TPP_CASE_TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL
#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
			if (quote != '"') {
				if (!tpp_lexer_has(lexer, TOK_CXX_UTF8_CHAR_LITERAL))
					goto print_generic_string;
			} else
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
			{
				if (!tpp_lexer_has(lexer, TOK_CXX_UTF8_STRING_LITERAL))
					goto print_generic_string;
			}
			result = tpp_emitter_print_conststr(self, "u8");
			if (result < 0)
				return result;
			break;
#endif /* TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL */

#if TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL
		_TPP_CASE_TPP_TOK_CXX_UTF16_STRING_LITERAL
		_TPP_CASE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
		_TPP_CASE_TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL
		_TPP_CASE_TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL
#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
			if (quote != '"') {
				if (!tpp_lexer_has(lexer, TOK_CXX_UTF16_CHAR_LITERAL))
					goto print_generic_string;
			} else
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
			{
				if (!tpp_lexer_has(lexer, TOK_CXX_UTF16_STRING_LITERAL))
					goto print_generic_string;
			}
			result = tpp_emitter_print_conststr(self, "u");
			if (result < 0)
				return result;
			break;
#endif /* TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL */

#if TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL
		_TPP_CASE_TPP_TOK_CXX_UTF32_STRING_LITERAL
		_TPP_CASE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
		_TPP_CASE_TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL
		_TPP_CASE_TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL
#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
			if (quote != '"') {
				if (!tpp_lexer_has(lexer, TOK_CXX_UTF32_CHAR_LITERAL))
					goto print_generic_string;
			} else
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
			{
				if (!tpp_lexer_has(lexer, TOK_CXX_UTF32_STRING_LITERAL))
					goto print_generic_string;
			}
			result = tpp_emitter_print_conststr(self, "U");
			if (result < 0)
				return result;
			break;
#endif /* TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL || TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL */

		default:
#if TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS
			if (quote != '"') {
				if (!tpp_lexer_has(lexer, TOK_C_CHAR))
					goto print_generic_string;
			} else
#endif /* TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS */
			{
				if (!tpp_lexer_has(lexer, TOK_C_STRING))
					goto print_generic_string;
			}
			break;
		}

		temp = tpp_emitter_print(self, &quote, 1);
		if (temp < 0)
			return temp;
		result += temp;

		/* Decode+re-encode string on-the-fly. */
#if TPP_HAVE_STRING_ESCAPE_BIGCHAR
		{
			struct tpp_emitter_printbig_data data;
			data.tepbd_emitter = self;
			data.tepbd_quote   = quote;
			data.tepbd_after_x = false;
			config.tldsc_arg = &data;
			config.tldsc_dataprinter = tpp_formatprinter_of(tpp_emitter_printbig_normal);
#if TPP_HAVE_UNICODE
			config.tldsc_utf8printer = tpp_formatprinter_of(tpp_emitter_printbig_normal);
#endif /* TPP_HAVE_UNICODE */
			config.tldsc_bigprinter = &tpp_emitter_printbig_big;
			temp = tpp_lexer_decodestring(lexer, &config);
		}
#else /* TPP_HAVE_STRING_ESCAPE_BIGCHAR */
		tpp_lexer_decodestring_config_init_simple(&config, tpp_formatprinter_of(tpp_emitter_print_encodestring), self);
		temp = tpp_lexer_decodestring(lexer, &config);
#endif /* !TPP_HAVE_STRING_ESCAPE_BIGCHAR */
		temp = tpp_emitter_print(self, &quote, 1);
		if (temp < 0)
			return temp;
		result += temp;
		return result;
print_generic_string:;
	}	break;
#endif /* TPP_EMITTER_HAVE_NORMALIZE_C_STRING */

#if TPP_EMITTER_HAVE_NORMALIZE_C_INT
	TPP_CASE_TPP_TOK_INT {
#if TPP_HAVE_EXPR_VALUE_PRINTREPR || defined(tpp_expr_value_printrepr)
		tpp_expr_intvalue intval_expr;
		tpp_char const *suffix_start;
		tpp_errno error;
		tpp_ssize temp, result;
		if (!tpp_emitter_has(self, NORMALIZE_C_INT))
			break;
		error = tpp_lexer_decodeint_ex(lexer, &intval_expr, &suffix_start);
		if (TPP_ISERR(error))
			return TPP_SSIZE_OFERR(error);
		result = tpp_expr_intvalue_printrepr(tpp_emitter_getlexer(self), &intval_expr,
			                                 tpp_emitter_getprinter(self), self);
		tpp_expr_intvalue_fini(&intval_expr);
		if (result < 0)
			return result;
		temp = tpp_emitter_print_generic(self, suffix_start, (tpp_size)(token_end - suffix_start));
		if (temp < 0)
			return temp;
		result += temp;
		return result;
#else /* TPP_HAVE_EXPR_VALUE_PRINTREPR || tpp_expr_value_printrepr */
		tpp_intmax intval;
		tpp_expr_intvalue intval_expr;
		tpp_char const *suffix_start;
		char buf[TPP_ITOA_MAXLEN], *intbase;
		tpp_errno error;
		tpp_ssize temp, result;
		if (!tpp_emitter_has(self, NORMALIZE_C_INT))
			break;
		error = tpp_lexer_decodeint_ex(lexer, &intval_expr, &suffix_start);
		if (TPP_ISERR(error))
			return TPP_SSIZE_OFERR(error);
		error = tpp_expr_intvalue_asintmax(&intval_expr, &intval);
		tpp_expr_intvalue_fini(&intval_expr);
		if (TPP_ISERR(error)) {
#if TPP_EXPR_INTVALUE_ASINTMAX_CANOVERFLOW
			if (error == TPP_ENOENT)
				break;
#endif /* TPP_EXPR_INTVALUE_ASINTMAX_CANOVERFLOW */
			return TPP_SSIZE_OFERR(error);
		}
		intbase = tpp_itoa(buf, intval);
		result = tpp_emitter_print_cstr(self, intbase, (tpp_size)(buf + TPP_ITOA_MAXLEN - intbase));
		if (result < 0)
			return result;
		temp = tpp_emitter_print_generic(self, suffix_start, (tpp_size)(token_end - suffix_start));
		if (temp < 0)
			return temp;
		result += temp;
		return result;
#endif /* !TPP_HAVE_EXPR_VALUE_PRINTREPR && !tpp_expr_value_printrepr */
	}	break;
#endif /* TPP_EMITTER_HAVE_NORMALIZE_C_INT */

#if TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS
	case '{':
	case '[':
	case '}':
	case ']':
	case '#': {
		tpp_char digraph_esc[1];
		if (!tpp_emitter_has(self, NORMALIZE_DIGRAPHS))
			break;
		digraph_esc[0] = (tpp_char)tpp_lexer_gettok(lexer);
		return tpp_emitter_print(self, digraph_esc, 1);
	}	break;

#if TPP_HAVE_TOK_POUND_POUND
	case TPP_TOK_POUND_POUND:
		if (!tpp_emitter_has(self, NORMALIZE_DIGRAPHS))
			break;
		return tpp_emitter_print_conststr(self, "##");
#endif /* TPP_HAVE_TOK_POUND_POUND */
#endif /* TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS */

	default: {
#if TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS
		if (TPP_TOK_ISKEYWORD(tpp_lexer_gettok(lexer))) {
			if (tpp_emitter_has(self, NORMALIZE_KEYWORDS)) {
				tpp_keyword const *const kwd = tpp_lexer_gettokenkwd(lexer);
				tpp_char const *kwd_start = tpp_keyword_getstr(kwd);
				tpp_size kwd_len = tpp_keyword_getlen(kwd);
				return tpp_emitter_print_keyword(self, kwd_start, kwd_len);
			}
		} else
#endif /* TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */
		{
		}
	}	break;
	}

	/* Do generic processing for this type of token */
	return tpp_emitter_print_generic(self, token_start, (tpp_size)(token_end - token_start));
}
#endif /* TPP_EMITTER_HAVE_MODE_EMIT */

#if TPP_EMITTER_HAVE_MODE_EMIT
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent_emit(tpp_emitter *tpp_restrict self) {
	tpp_ssize temp, result = 0;
	tpp_lexer const *const lexer = tpp_emitter_getlexer(self);
	tpp_token_id const tok = tpp_lexer_gettok(lexer);
#if TPP_HAVE_LEXER_REQUIRE_WHITESPACE
	bool const require_whitespace = tpp_lexer_require_whitespace(lexer, self->tem_state.tems_prevtok, tok);
#elif !TPP_IGNORE_INVALID_CONFIGURATION
#error "Use of 'TPP_EMITTER_HAVE_MODE_EMIT' requires `tpp_lexer_require_whitespace()`, but that function is disabled: `-DTPP_HAVE_LEXER_REQUIRE_WHITESPACE=0`"
#endif /* ... */

	if (tpp_emitter_has(self, NOLINE)) {
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
emit_without_alignment:;
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_HAVE_LEXER_REQUIRE_WHITESPACE
		if (require_whitespace) {
			temp = tpp_emitter_printspace(self, 1);
			if (temp < 0)
				goto err_temp;
			result += temp;
		}
#endif /* TPP_HAVE_LEXER_REQUIRE_WHITESPACE */
	} else {
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
		tpp_file *const lcfile = tpp_lexer_getlcfile(lexer);
		char const *const lc_filename = tpp_file_getfilename(lcfile);
		tpp_lcinfo const lcinfo = tpp_file_getstartlcinfo(lcfile);
		bool pushes_changed;

		/* Don't align anything if there is no LC info (except to inject required whitespace) */
		if (!tpp_lcinfo_isvalid(lcinfo))
			goto emit_without_alignment;

		/* Check if the pointed-to filename changed, but its contents didn't. */
		if (self->tem_state.tems_curfile.temsfs_file.temsf_fname != lc_filename &&
		    self->tem_state.tems_curfile.temsfs_file.temsf_fname != NULL && lc_filename != NULL &&
		    tpp_strcmp(self->tem_state.tems_curfile.temsfs_file.temsf_fname, lc_filename) == 0) {
			tpp_string *new_filename_string = tpp_file_getfilenamestr(lcfile);
			if (new_filename_string)
				tpp_string_incref(new_filename_string);
			if (self->tem_state.tems_curfile.temsfs_file.temsf_fname_str)
				tpp_string_decref(self->tem_state.tems_curfile.temsfs_file.temsf_fname_str);
			self->tem_state.tems_curfile.temsfs_file.temsf_fname     = lc_filename;
			self->tem_state.tems_curfile.temsfs_file.temsf_fname_str = new_filename_string;
		}

		/* Check if stuff needs to be realigned (by injecting whitespace). */
		pushes_changed = tpp_emitter_pushed_files_changed(self, lcfile);
		if (self->tem_state.tems_curfile.temsfs_file.temsf_fname != lc_filename ||
		    !tpp_lcinfo_equals(tpp_lcstate_getlc(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos), lcinfo) ||
#if TPP_HAVE_LEXER_REQUIRE_WHITESPACE
		    require_whitespace ||
#endif /* TPP_HAVE_LEXER_REQUIRE_WHITESPACE */
		    pushes_changed) {
			tpp_line oldline = tpp_lcstate_getline(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos);
			tpp_line newline = tpp_lcinfo_getline(lcinfo);
			tpp_column oldcol = tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos);
			tpp_column newcol = tpp_lcinfo_getcol(lcinfo);
			bool need_line_directive = pushes_changed;

			if (self->tem_state.tems_curfile.temsfs_file.temsf_fname != lc_filename) {
				need_line_directive = true;
			} else if (newline < oldline) {
				need_line_directive = true;
			} else
#if TPP_EMITTER_CONFIG_LINE_THRESHOLD
			if (newline >= (oldline + tpp_emitter_getlinethreshold(self)) &&
			    (oldline + tpp_emitter_getlinethreshold(self)) >= oldline) {
				need_line_directive = true;
			} else
#endif /* TPP_EMITTER_CONFIG_LINE_THRESHOLD */
			if (newline == oldline) {
				if (newcol < oldcol) {
#if TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN
					if (lcfile != tpp_lexer_getfile(lexer) &&
					    tpp_file_ismacro(tpp_lexer_getfile(lexer)) &&
						tpp_emitter_has(self, RELAXED_MACRO_COLUMN)) {
						/* Inside of a macro -- so-as to prevent every token from causing
						 * another `#line`-directive being emitted, don't be too precise
						 * in terms of *all* tokens needing to have the proper column */
#if TPP_HAVE_LEXER_REQUIRE_WHITESPACE
						if (require_whitespace)
							newcol = oldcol + 1;
#endif /* TPP_HAVE_LEXER_REQUIRE_WHITESPACE */
					} else
#endif /* TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN */
					{
						need_line_directive = true;
					}
				} else
#if TPP_HAVE_LEXER_REQUIRE_WHITESPACE
				if (newcol == oldcol && require_whitespace) {
					need_line_directive = true;
				} else
#endif /* TPP_HAVE_LEXER_REQUIRE_WHITESPACE */
				{
				}
			}
			if (need_line_directive) {
#if !TPP_EMITTER_HAVE_USE_CPP_DIGIT
				temp = tpp_emitter_print_line_directive(self, newline, lc_filename,
				                                        tpp_file_getfilenamestr(lcfile));
#elif !TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_USE_CPP_DIGIT)
				temp = tpp_emitter_print_cpp_digit_directive(self, lcfile, newline, lc_filename,
				                                             tpp_file_getfilenamestr(lcfile));
#else /* ... */
				if (tpp_emitter_has(self, USE_CPP_DIGIT)) {
					temp = tpp_emitter_print_cpp_digit_directive(self, lcfile, newline, lc_filename,
					                                             tpp_file_getfilenamestr(lcfile));
				} else {
					temp = tpp_emitter_print_line_directive(self, newline, lc_filename,
					                                        tpp_file_getfilenamestr(lcfile));
				}
#endif /* !... */
				if (temp < 0)
					goto err_temp;
				result += temp;
				oldline = tpp_lcstate_getline(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos);
				oldcol  = tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos);
			}

			/* Align with extra line-feed characters. */
			if (newline > oldline) {
				temp = tpp_emitter_printlf(self, (tpp_line)(newline - oldline));
				if (temp < 0)
					goto err_temp;
				result += temp;
				oldcol = tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos);
				tpp_assert(oldcol == 0);
			}

			/* Align with extra space characters. */
			if (newcol > oldcol) {
				temp = tpp_emitter_printspace(self, (tpp_column)(newcol - oldcol));
				if (temp < 0)
					goto err_temp;
				result += temp;
				tpp_assert(tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) == newcol);
			}
		}
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
	}

	/* Actually emit the token */
	temp = tpp_emitter_print_current_token(self);
	if (temp < 0)
		goto err_temp;
	result += temp;

	/* Remember the last-emitted token */
	self->tem_state.tems_prevtok = tok;
	return result;
err_temp:
	return temp;
}
#endif /* TPP_EMITTER_HAVE_MODE_EMIT */


/* API support for (re-)emission of unknown `#pragma` directives */
#if TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_unknown_pragma(_tpp_emitter_hook_unknown_pragma_cookie cookie) {
	tpp_emitter *const self = _tpp_emitter_hook_unknown_pragma_ofcookie(cookie);
	tpp_lexer *const lexer = tpp_emitter_getlexer(self);
	tpp_ssize temp;
	tpp_token_id prev_token;
	tpp_token_id tok = tpp_lexer_gettok(lexer);
#if TPP_EMITTER_HAVE_CURPOS
	if (tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) != 0) {
		temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			goto err_temp;
	}
#endif /* TPP_EMITTER_HAVE_CURPOS */
	temp = tpp_emitter_print_conststr(self, "#pragma");
	if (temp < 0)
		goto err_temp;
	/* Print+yield tokens until the #pragma's end is reached */
	prev_token = TPP_KWD_pragma;
	while (tok != TPP_TOK_EOF) {
		if (!TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tok)) {
			/* Print token... */
			if (tpp_lexer_require_whitespace(lexer, prev_token, tok)) {
				temp = tpp_emitter_print_conststr(self, " ");
				if (temp < 0)
					goto err_temp;
			}
			temp = tpp_emitter_print_current_token(self);
			if (temp < 0)
				goto err_temp;
			prev_token = tok;
		}
		tok = tpp_lexer_yieldraw(lexer);
		if (TPP_TOK_ISERR(tok))
			return TPP_TOK_ASERR(tok);
	}
	temp = tpp_emitter_print_conststr(self, "\n");
	if (temp < 0)
		goto err_temp;
	self->tem_state.tems_prevtok = TPP_TOK_EOF;
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */



/* API support for (re-)emission of `#define` and `#undef` directives */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS || TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_dump_define(tpp_emitter *tpp_restrict self,
                        tpp_keyword const *tpp_restrict name,
                        tpp_macro const *tpp_restrict macro) {
	tpp_ssize temp;
	temp = tpp_emitter_print_conststr(self, "#define ");
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print(self, tpp_keyword_getstr(name), tpp_keyword_getlen(name));
	if (temp < 0)
		goto err_temp;
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
	if (macro && !tpp_emitter_has(self, REEMIT_MACRO_DEFINITIONS_NAME_ONLY)) {
		/* Print function-like argument list. */
		if (tpp_macro_isfunction(macro)) {
			tpp_size i, argc = tpp_macro_getfuncargc(macro);
			tpp_char lparen = tpp_macro_getfunclparen(macro);
			tpp_char rparen = tpp_macro_getfuncrparen(macro);
			temp = tpp_emitter_print(self, &lparen, 1);
			if (temp < 0)
				goto err_temp;
			for (i = 0; i < argc; ++i) {
				tpp_token_id arg = tpp_macro_getfuncargtok(macro, i);
				tpp_keyword const *kwd;
				if (i != 0) {
					temp = tpp_emitter_print_conststr(self, ", ");
					if (temp < 0)
						goto err_temp;
				}
#if TPP_HAVE_VA_ARGS_IN_MACROS
				if ((i == argc - 1) && arg == TPP_KWD___VA_ARGS__)
					break;
#endif /* TPP_HAVE_VA_ARGS_IN_MACROS */
				kwd = tpp_lexer_kwds_getkeyword_byid(tpp_emitter_getlexer(self), arg);
				if (kwd) {
					temp = tpp_emitter_print_keyword(self,
					                                 tpp_keyword_getstr(kwd),
					                                 tpp_keyword_getlen(kwd));
				} else { /* Should never get here */
					temp = tpp_emitter_print_conststr(self, "?");
				}
				if (temp < 0)
					goto err_temp;
			}
			if (tpp_macro_isvarargs(macro)) {
				temp = tpp_emitter_print_conststr(self, "...");
				if (temp < 0)
					goto err_temp;
			}
			temp = tpp_emitter_print(self, &rparen, 1);
			if (temp < 0)
				goto err_temp;
		}

		/* Print macro body... */
		if (tpp_macro_getbodylen(macro)) {
			temp = tpp_emitter_printspace(self, 1);
			if (temp < 0)
				goto err_temp;
			temp = tpp_emitter_print_generic(self,
			                                 tpp_macro_getbodystart(macro),
			                                 tpp_macro_getbodylen(macro));
			if (temp < 0)
				goto err_temp;
		}
	}
#endif /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */
	temp = tpp_emitter_print_conststr(self, "\n");
	if (temp < 0)
		goto err_temp;
	self->tem_state.tems_prevtok = TPP_TOK_EOF;
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_dump_undef(tpp_emitter *tpp_restrict self,
                       tpp_keyword const *tpp_restrict name) {
	tpp_ssize temp;
	temp = tpp_emitter_print_conststr(self, "#undef ");
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print(self, tpp_keyword_getstr(name), tpp_keyword_getlen(name));
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print_conststr(self, "\n");
	if (temp < 0)
		goto err_temp;
	self->tem_state.tems_prevtok = TPP_TOK_EOF;
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS || TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_defined(_tpp_emitter_hook_macro_defined_cookie cookie,
                                tpp_keyword *tpp_restrict name,
                                tpp_macro *tpp_restrict macro) {
	tpp_emitter *self = _tpp_emitter_hook_macro_defined_ofcookie(cookie);
#if TPP_EMITTER_HAVE_CURPOS
	if (tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) != 0) {
		tpp_ssize temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			return TPP_SSIZE_ASERR(temp);
	}
#endif /* TPP_EMITTER_HAVE_CURPOS */
	return tpp_emitter_dump_define(self, name, macro);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
_tpp_emitter_hook_macro_undefined(_tpp_emitter_hook_macro_undefined_cookie cookie,
                                  tpp_keyword *tpp_restrict name) {
	tpp_emitter *self = _tpp_emitter_hook_macro_undefined_ofcookie(cookie);
#if TPP_EMITTER_HAVE_CURPOS
	if (tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) != 0) {
		tpp_ssize temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			return TPP_SSIZE_ASERR(temp);
	}
#endif /* TPP_EMITTER_HAVE_CURPOS */
	return tpp_emitter_dump_undef(self, name);
}

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_enable_reemit_macro_definitions(tpp_emitter *tpp_restrict self) {
	tpp_errno result = _tpp_emitter_enable_macro_defined_hook(self);
	if (!TPP_ISERR(result)) {
		result = _tpp_emitter_enable_macro_undefined_hook(self);
		if (TPP_ISERR(result))
			_tpp_emitter_disable_macro_defined_hook(self);
	}
	return result;
}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */



/* API support for (re-)emission of `#include` (and friends) directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_include_encountered(_tpp_emitter_hook_include_encountered_cookie cookie,
                                      tpp_hook_include_kind include_kind) {
	tpp_emitter *const self = _tpp_emitter_hook_include_encountered_ofcookie(cookie);
	tpp_lexer const *const lexer = tpp_emitter_getlexer(self);
	tpp_ssize temp;
#if TPP_EMITTER_HAVE_CURPOS
	if (tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) != 0) {
		temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			goto err_temp;
	}
#endif /* TPP_EMITTER_HAVE_CURPOS */
	switch (include_kind) {
#if TPP_HAVE_CPP_INCLUDE
	case TPP_HOOK_INCLUDE_KIND_INCLUDE:
		temp = tpp_emitter_print_conststr(self, "#include ");
		break;
#endif /* TPP_HAVE_CPP_INCLUDE */
#if TPP_HAVE_CPP_INCLUDE_NEXT
	case TPP_HOOK_INCLUDE_KIND_INCLUDE_NEXT:
		temp = tpp_emitter_print_conststr(self, "#include_next ");
		break;
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT */
#if TPP_HAVE_CPP_IMPORT
	case TPP_HOOK_INCLUDE_KIND_IMPORT:
		temp = tpp_emitter_print_conststr(self, "#import ");
		break;
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_CPP_EMBED
	case TPP_HOOK_INCLUDE_KIND_EMBED:
		temp = tpp_emitter_print_conststr(self, "#embed ");
		break;
#endif /* TPP_HAVE_CPP_EMBED */
	default: tpp_unreachable();
	}
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print_generic(self,
	                                 tpp_lexer_gettokenstart(lexer),
	                                 tpp_lexer_gettokenlen(lexer));
	if (temp < 0)
		goto err_temp;
	temp = tpp_emitter_print_conststr(self, "\n");
	if (temp < 0)
		goto err_temp;
	self->tem_state.tems_prevtok = TPP_TOK_EOF;
	return TPP_EOK;
err_temp:
	return TPP_SSIZE_ASERR(temp);
}
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */


#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
static TPP_NONNULL((1, 2)) void TPPCALL
tpp_emitter_maybe_set_files_changes(tpp_emitter *tpp_restrict self,
                                    tpp_file const *tpp_restrict changed_file) {
	if (tpp_emitter_has(self, USE_CPP_DIGIT_FLAGS) &&
	    tpp_file_haslcinfo(changed_file))
		self->tem_state.tems_flags |= TPP_EMITTER_FLAG_FCHANGED;
}
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */

#if TPP_EMITTER_HAVE_HOOK_FILE_PUSHED
#if TPP_EMITTER_HAVE_TRACE_INCLUDES
static TPP_NOINLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_trace_include_printdots(tpp_lexer *tpp_restrict lexer, tpp_size count) {
	tpp_char buffer[512];
	tpp_size bufmax = sizeof(buffer);
	if (bufmax > count)
		bufmax = count;
	tpp_memset(buffer, '.', bufmax * sizeof(tpp_char));
	while (count) {
		tpp_ssize temp = tpp_lexer_callhook_mesgprinter(lexer, buffer, bufmax);
		if (temp < 0)
			return TPP_SSIZE_ASERR(temp);
		count -= bufmax;
		if (bufmax > count)
			bufmax = count;
	}
	return TPP_EOK;
}

static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_trace_include(tpp_emitter *tpp_restrict self,
                          tpp_file const *tpp_restrict file) {
	tpp_ssize temp;
	tpp_errno error;
	tpp_size depth = 1;
	tpp_file const *iter = file;
	char const *filename = tpp_file_getrealfilename(file);
	tpp_size filename_len;
	while ((iter = tpp_file_getprev(iter)) != NULL) {
		if (tpp_file_getkind(iter) == TPP_FILE_KIND_IO)
			++depth;
	}
	error = tpp_emitter_trace_include_printdots(tpp_emitter_getlexer(self), depth);
	if (TPP_ISERR(error))
		return error;
	temp = tpp_lexer_callhook_mesgprinter(tpp_emitter_getlexer(self),
	                                      (tpp_char const *)" ", 1);
	if (temp < 0)
		return TPP_SSIZE_ASERR(temp);
	if (filename == NULL)
		filename = "?";
	filename_len = tpp_strlen(filename);
	temp = tpp_lexer_callhook_mesgprinter(tpp_emitter_getlexer(self),
	                                      (tpp_char const *)filename, filename_len);
	if (temp < 0)
		return TPP_SSIZE_ASERR(temp);
	temp = tpp_lexer_callhook_mesgprinter(tpp_emitter_getlexer(self),
	                                      (tpp_char const *)"\n", 1);
	if (temp < 0)
		return TPP_SSIZE_ASERR(temp);
	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_TRACE_INCLUDES */

#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
static void TPPCALL tpp_emitter_reemit_macro_used_dtor(void *ptr) {
	TPP_REF tpp_macro *macro = (TPP_REF tpp_macro *)ptr;
	tpp_macro_decref(macro);
}

/* Handle "macro" being used by being pushed onto the `#include`-stack. */
static TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_reemit_macro_used(tpp_emitter *tpp_restrict self,
                              tpp_macro *tpp_restrict macro) {
	tpp_errno error;
	tpp_macro *last_definition = NULL;
	tpp_keyword *name = tpp_macro_getname(macro);
	if (!name)
		return TPP_EOK;
	if (tpp_keyword_getuserdata_dtor(name) == &tpp_emitter_reemit_macro_used_dtor)
		last_definition = (tpp_macro *)tpp_keyword_getuserdata_dtor(name);
	if (last_definition == macro)
		return TPP_EOK; /* Already emitted! */

	/* Must emit a new definition! */
#if TPP_EMITTER_HAVE_CURPOS
	if (tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) != 0) {
		tpp_ssize temp = tpp_emitter_print_conststr(self, "\n");
		if (temp < 0)
			return TPP_SSIZE_ASERR(temp);
	}
#endif /* TPP_EMITTER_HAVE_CURPOS */
	if (last_definition) {
		error = tpp_emitter_dump_undef(self, name);
		if (TPP_ISERR(error))
			return error;
	}
	error = tpp_emitter_dump_define(self, name, macro);
	if (TPP_ISERR(error))
		return error;

	/* Remember the current definition within the keyword. */
	error = tpp_keyword_setuserdata(name, macro, &tpp_emitter_reemit_macro_used_dtor, true);
	if (TPP_ISERR(error))
		return error;
	tpp_macro_incref(macro); /* Extra reference stored in user-data of keyword "name" */
	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_file_pushed(_tpp_emitter_hook_file_pushed_cookie cookie) {
	tpp_emitter *const self = _tpp_emitter_hook_file_pushed_ofcookie(cookie);
	tpp_lexer const *const lexer = tpp_emitter_getlexer(self);
	tpp_file const *const file = tpp_lexer_getfile(lexer);

	/* Deal with include tracing */
#if TPP_EMITTER_HAVE_TRACE_INCLUDES
	if (tpp_file_getkind(file) == TPP_FILE_KIND_IO) {
		if (tpp_emitter_has(self, TRACE_INCLUDES)) {
			tpp_errno error = tpp_emitter_trace_include(self, file);
			if (TPP_ISERR(error))
				return error;
		}
	}
#endif /* TPP_EMITTER_HAVE_TRACE_INCLUDES */

	/* Deal with lazy macro definitions */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
	if (tpp_file_ismacro(file)) {
		if (tpp_emitter_has(self, REEMIT_MACRO_DEFINITIONS_LAZY)) {
			tpp_macro *macro = tpp_file_getmacro(file);
			return tpp_emitter_reemit_macro_used(self, macro);
		}
	}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
	tpp_emitter_maybe_set_files_changes(self, file);
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_HOOK_FILE_PUSHED */

/* Extension to `TPP_EMITTER_HAVE_USE_CPP_DIGIT`: also use 1/2/3/4 flags */
#if TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS
TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_emitter_hook_file_popped(_tpp_emitter_hook_file_popped_cookie cookie) {
	tpp_emitter *const self = _tpp_emitter_hook_file_popped_ofcookie(cookie);
	tpp_lexer const *const lexer = tpp_emitter_getlexer(self);
	tpp_emitter_maybe_set_files_changes(self, tpp_lexer_getfile(lexer));
}

#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_enable_use_cpp_digit_flags(tpp_emitter *tpp_restrict self) {
	tpp_errno result = _tpp_emitter_enable_file_pushed_hook(self);
	if (!TPP_ISERR(result)) {
		result = _tpp_emitter_enable_file_popped_hook(self);
		if (TPP_ISERR(result)) {
			_tpp_emitter_maybe_disable_file_pushed_hook(self);
		} else {
			tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_USE_CPP_DIGIT_FLAGS);
		}
	}
	return result;
}
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#endif /* TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS */


/* API support for *lazy* (re-)emission of `#define` and `#undef` directives */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY)
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_enable_reemit_macro_definitions_lazy(tpp_emitter *tpp_restrict self) {
	tpp_errno const result = _tpp_emitter_enable_file_pushed_hook(self);
	if (!TPP_ISERR(result))
		tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_LAZY);
	return result;
}
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY) */


/* API support for tracing of #incude-depth and files */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_enable_trace_includes(tpp_emitter *tpp_restrict self) {
	tpp_errno const result = _tpp_emitter_enable_file_pushed_hook(self);
	if (!TPP_ISERR(result))
		tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_TRACE_INCLUDES);
	return result;
}
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */


#if TPP_EMITTER_HAVE_MODE_BRACKET
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent_bracket(tpp_emitter *tpp_restrict self) {
	tpp_ssize temp, result;
	result = tpp_emitter_print_conststr(self, "[");
	if (result < 0)
		return result;
	temp = tpp_emitter_print_current_token(self);
	if (temp < 0)
		return temp;
	result += temp;
	temp = tpp_emitter_print_conststr(self, "]");
	if (temp < 0)
		return temp;
	result += temp;
	return result;
}
#endif /* TPP_EMITTER_HAVE_MODE_BRACKET */


#if TPP_EMITTER_HAVE_MODE_TYPED
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent_typed(tpp_emitter *tpp_restrict self) {
	tpp_ssize temp, result;
	char const *strtoken;
	tpp_lexer const *const lexer = tpp_emitter_getlexer(self);
	result = tpp_emitter_print_conststr(self, "[");
	if (result < 0)
		return result;
	strtoken = tpp_strtokenid(tpp_lexer_gettok(lexer));
	if (strtoken == NULL && tpp_lexer_hastokenkwd(lexer))
		strtoken = tpp_lexer_gettokenkwdcstr(lexer);
	if (strtoken == NULL)
		strtoken = "?";
	temp = tpp_emitter_print_cstr(self, strtoken, tpp_strlen(strtoken));
	if (temp < 0)
		return temp;
	result += temp;
	temp = tpp_emitter_print_conststr(self, ":");
	if (temp < 0)
		return temp;
	result += temp;
	temp = tpp_emitter_print_current_token(self);
	if (temp < 0)
		return temp;
	result += temp;
	temp = tpp_emitter_print_conststr(self, "]");
	if (temp < 0)
		return temp;
	result += temp;
	return result;
}
#endif /* TPP_EMITTER_HAVE_MODE_TYPED */

#if TPP_EMITTER_HAVE_MODE_ZERO
static TPP_WUNUSED TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent_zero(tpp_emitter *tpp_restrict self) {
	tpp_ssize temp, result;
	result = tpp_emitter_print_current_token(self);
	if (result < 0)
		return result;
	temp = tpp_emitter_print_conststr(self, "\0");
	if (temp < 0)
		return temp;
	result += temp;
	return result;
}
#endif /* TPP_EMITTER_HAVE_MODE_ZERO */





/* Emit the token currently loaded into `tpp_emitter_getlexer(self)`,
 * and update the emitter's `tem_state` accordingly
 *
 * @return: * :  Sum of return values of `tpp_emitter_getoutput(self)`
 * @return: < 0: First negative return value of `tpp_emitter_getoutput(self)` */
TPP_IMPL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent(tpp_emitter *tpp_restrict self) {

	/* Check if current token is a keyword with a linked macro. */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
	if (tpp_lexer_hastokenkwd(tpp_emitter_getlexer(self)) &&
	    tpp_emitter_has(self, REEMIT_MACRO_DEFINITIONS_LAZY)) {
		tpp_keyword const *keyword = tpp_lexer_gettokenkwd(tpp_emitter_getlexer(self));
		if (!tpp_keyword_hasmacro(keyword) &&
		    tpp_keyword_getuserdata_dtor(keyword) == &tpp_emitter_reemit_macro_used_dtor) {
			tpp_errno error;
			/* Must emit an #undef directive */
#if TPP_EMITTER_HAVE_CURPOS
			if (tpp_lcstate_getcol(&self->tem_state.tems_curfile.temsfs_file.temsf_curpos) != 0) {
				tpp_ssize temp = tpp_emitter_print_conststr(self, "\n");
				if (temp < 0)
					return temp;
			}
#endif /* TPP_EMITTER_HAVE_CURPOS */
			error = tpp_emitter_dump_undef(self, keyword);
			if (TPP_ISERR(error))
				return error;
			error = tpp_keyword_setuserdata((tpp_keyword *)keyword, NULL, NULL, true);
			if (TPP_ISERR(error))
				return TPP_SSIZE_OFERR(error);
		}
	}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

	switch (tpp_expect(tpp_emitter_getmode(self), _TPP_EMITTER_MODE_DEFAULT)) {

#if TPP_EMITTER_HAVE_MODE_EMIT
	case TPP_EMITTER_MODE_EMIT:
		return tpp_emitter_emitcurrent_emit(self);
#endif /* TPP_EMITTER_HAVE_MODE_EMIT */

#if TPP_EMITTER_HAVE_MODE_DISPOSE
	case TPP_EMITTER_MODE_DISPOSE:
		return 0; /* Dispose tokens... */
#endif /* TPP_EMITTER_HAVE_MODE_DISPOSE */

#if TPP_EMITTER_HAVE_MODE_BRACKET
	case TPP_EMITTER_MODE_BRACKET:
		return tpp_emitter_emitcurrent_bracket(self);
#endif /* TPP_EMITTER_HAVE_MODE_BRACKET */

#if TPP_EMITTER_HAVE_MODE_TYPED
	case TPP_EMITTER_MODE_TYPED:
		return tpp_emitter_emitcurrent_typed(self);
#endif /* TPP_EMITTER_HAVE_MODE_TYPED */

#if TPP_EMITTER_HAVE_MODE_ZERO
	case TPP_EMITTER_MODE_ZERO:
		return tpp_emitter_emitcurrent_zero(self);
#endif /* TPP_EMITTER_HAVE_MODE_ZERO */

	default: tpp_unreachable();
	}
	tpp_unreachable();
}

/************************************************************************/
/* File: parts/optional/emitter/emitter-cli.c                           */
/************************************************************************/

#if TPP_EMITTER_HAVE_CLI

#if TPP_CONF_ISEXT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_enable_TOK_SPACE(self) tpp_lexer_enableextension(self, TPP_EXT_TOK_SPACE)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_enable_TOK_SPACE(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TOK_SPACE), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TOK_SPACE(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_LF)
#define tpp_lexer_enable_TOK_LF(self) tpp_lexer_enableextension(self, TPP_EXT_TOK_LF)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_LF)
#define tpp_lexer_enable_TOK_LF(self) (tpp_lexer_enablefeature(self, TPP_FEAT_TOK_LF), TPP_EOK)
#else /* ... */
#define tpp_lexer_enable_TOK_LF(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_disable_TOK_SPACE(self) tpp_lexer_disableextension(self, TPP_EXT_TOK_SPACE)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_SPACE)
#define tpp_lexer_disable_TOK_SPACE(self) (tpp_lexer_disablefeature(self, TPP_FEAT_TOK_SPACE), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_TOK_SPACE(self) TPP_EOK
#endif /* !... */
#if TPP_CONF_ISEXT(TPP_HAVE_TOK_LF)
#define tpp_lexer_disable_TOK_LF(self) tpp_lexer_disableextension(self, TPP_EXT_TOK_LF)
#elif TPP_CONF_ISFEAT(TPP_HAVE_TOK_LF)
#define tpp_lexer_disable_TOK_LF(self) (tpp_lexer_disablefeature(self, TPP_FEAT_TOK_LF), TPP_EOK)
#else /* ... */
#define tpp_lexer_disable_TOK_LF(self) TPP_EOK
#endif /* !... */


#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
#define tpp_emitter_set_NORMALIZE_SPACE(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_SPACE, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_SPACE(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF)
#define tpp_emitter_set_NORMALIZE_LF(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_LF, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_LF(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
#define tpp_emitter_set_NORMALIZE_C_STRING(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_C_STRING, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_C_STRING(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT)
#define tpp_emitter_set_NORMALIZE_C_INT(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_C_INT, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_C_INT(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
#define tpp_emitter_set_NORMALIZE_KEYWORDS(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_KEYWORDS, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_KEYWORDS(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
#define tpp_emitter_set_NORMALIZE_BSE(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_BSE, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_BSE(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
#define tpp_emitter_set_NORMALIZE_TRIGRAPHS(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_TRIGRAPHS, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_TRIGRAPHS(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
#define tpp_emitter_set_NORMALIZE_DIGRAPHS(self, v) tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_NORMALIZE_DIGRAPHS, v)
#else /* ... */
#define tpp_emitter_set_NORMALIZE_DIGRAPHS(self, v) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
#define tpp_emitter_enable_NOLINE(self) tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_NOLINE)
#else /* ... */
#define tpp_emitter_enable_NOLINE(self) (void)0
#endif /* !... */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
#define tpp_emitter_disable_NOLINE(self) tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_NOLINE)
#else /* ... */
#define tpp_emitter_disable_NOLINE(self) (void)0
#endif /* !... */


/* Define a function `tpp_emitter_cli_warnf()` */
#undef TPP_HAVE_EMITTER_CLI_WARN
#define TPP_HAVE_EMITTER_CLI_WARN \
	(TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT)

#if TPP_HAVE_EMITTER_CLI_WARN
static TPP_COLDCALL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_warnf(tpp_emitter *tpp_restrict self, tpp_char const *token_start,
                      tpp_size token_size, tpp_warning_id id, ...) {
	/* XXX: Use of `TPP_INTERNAL` here isn't allowed -- come up with
	 *      APIs to allow users to do this stuff without needing to
	 *      access TPP internal! */
	tpp_lexer *const lexer = tpp_emitter_getlexer(self);
	tpp_errno result;
	va_list args;
	union TPP_INTERNAL(tpp_lexer_core) saved_core = lexer->TPP_INTERNAL(tl_core);
	tpp_file *const file = tpp_lexer_getfile(lexer);
	tpp_file_init_text_utf8(file, TPP_CONFIG_CLI_FILENAME,
	                        NULL, token_start, token_size,
	                        TPP_LCINFO_INVALID, TPP_FILE_FLAGS_NORMAL);
	file->TPP_INTERNAL(tf_tpos) = token_start;
	file->TPP_INTERNAL(tf_pos)  = token_start + token_size;
	va_start(args, id);
	result = tpp_lexer_vwarnf(lexer, id, args);
	va_end(args);
	lexer->TPP_INTERNAL(tl_core) = saved_core;
	return result;
}
#endif /* TPP_HAVE_EMITTER_CLI_WARN */


#if TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_no_line_commands(tpp_emitter_cli_loader *tpp_restrict self) {
	tpp_errno result;
	(void)self;
	tpp_emitter_enable_NOLINE(self->temcl_emitter);

	/* Enable emission of SPACE/LF tokens */
	result = tpp_lexer_enable_TOK_SPACE(tpp_emitter_getlexer(self->temcl_emitter));
	if (!TPP_ISERR(result))
		result = tpp_lexer_enable_TOK_LF(tpp_emitter_getlexer(self->temcl_emitter));
	return result;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */


#if TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
#define tpp_emitter_cli_enable_trace_includes(self) \
	tpp_emitter_enable_trace_includes((self)->temcl_emitter)
#else /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
#define tpp_emitter_cli_enable_trace_includes(self) TPP_EOK
#endif /* !TPP_CONF_ISRT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
#endif /* TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_M
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_M(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Turn on re-emission of additional macros */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
	{
		tpp_errno error = tpp_emitter_enable_reemit_macro_definitions(self->temcl_emitter);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

	/* Set flag to dump definitions of builtin/predefined macros later. */
	self->temcl_flags |= _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M;

	/* Set emitter mode to "TPP_EMITTER_MODE_DISPOSE" */
#if TPP_EMITTER_HAVE_MODE_DISPOSE
	tpp_emitter_setmode(self->temcl_emitter, TPP_EMITTER_MODE_DISPOSE);
#endif /* TPP_EMITTER_HAVE_MODE_DISPOSE */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_M */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_D
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_D(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Turn on re-emission of additional macros */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
	{
		tpp_errno error = tpp_emitter_enable_reemit_macro_definitions(self->temcl_emitter);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

	/* Set flag to dump definitions of builtin/predefined macros later. */
	self->temcl_flags |= _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M;

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_D */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_N
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_N(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Turn on re-emission of additional macros */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS
	{
		tpp_errno error = tpp_emitter_enable_reemit_macro_definitions(self->temcl_emitter);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS */

	/* Set flag to dump definitions of builtin/predefined macros later. */
	self->temcl_flags |= _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M;

	/* *only* print the name of macros in `#define` directives */
#if TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
	tpp_emitter_enablefeature(self->temcl_emitter, TPP_EMITTER_FEAT_REEMIT_MACRO_DEFINITIONS_NAME_ONLY);
#endif /* TPP_CONF_ISRT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_N */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_I
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_I(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Turn on re-emission of #include-directives */
#if TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES
	{
		tpp_errno error = tpp_emitter_enable_reemit_include_directives(self->temcl_emitter);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_EMITTER_HAVE_REEMIT_INCLUDE_DIRECTIVES */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_I */


#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_U
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump_U(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Turn on lazy re-emission of #define-directives */
#if TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY
	{
		tpp_errno error = tpp_emitter_enable_reemit_macro_definitions_lazy(self->temcl_emitter);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_LAZY */

	return TPP_EOK;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_U */


#undef TPP_EMITTER_HAVE_CLI_DASH_DUMP
#define TPP_EMITTER_HAVE_CLI_DASH_DUMP   \
	(TPP_EMITTER_HAVE_CLI_DASH_DUMP_M || \
	 TPP_EMITTER_HAVE_CLI_DASH_DUMP_D || \
	 TPP_EMITTER_HAVE_CLI_DASH_DUMP_N || \
	 TPP_EMITTER_HAVE_CLI_DASH_DUMP_I || \
	 TPP_EMITTER_HAVE_CLI_DASH_DUMP_U)
#undef TPP_EMITTER_HAVE_CLI_DASH_MODE
#define TPP_EMITTER_HAVE_CLI_DASH_MODE         \
	(TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT ||    \
	 TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE || \
	 TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET || \
	 TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED ||   \
	 TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO)


enum {
	_TPP_EMITTER_CLI_LOADER_STATE_FIRST_INTERNAL = TPP_EMITTER_CLI_LOADER_STATE_DDASH,
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
	TPP_EMITTER_CLI_LOADER_STATE_DUMP,
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */
};

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dump(tpp_emitter_cli_loader *tpp_restrict self,
                            tpp_char what) {
	switch (what) {

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_M
	case 'M':
		return tpp_emitter_cli_enable_dump_M(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_M */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_D
	case 'D':
		return tpp_emitter_cli_enable_dump_D(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_D */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_N
	case 'N':
		return tpp_emitter_cli_enable_dump_N(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_N */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_I
	case 'I':
		return tpp_emitter_cli_enable_dump_I(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_I */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_U
	case 'U':
		return tpp_emitter_cli_enable_dump_U(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_U */

	default: break;
	}
	return TPP_ENOENT;
}

static TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_enable_dumps(tpp_emitter_cli_loader *tpp_restrict self,
                             char const *whats) {
	tpp_errno result = TPP_EOK;
	for (;;) {
		char what = *whats++;
		if (what == '\0')
			break;
		result = tpp_emitter_cli_enable_dump(self, what);
		if (TPP_ISERR(result))
			break;
	}
	return result;
}
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */


/* Feed an argument to the loader. How exactly the argument is parsed
 * depends on the loader's current state, but sufficed to say: in its
 * default/initial state, `arg` is a CLI argument as you'd expect.
 *
 * WARNING: When you call this function, you must guaranty that `arg` remains
 *          valid, allocated, and unaltered until `tpp_emitter_cli_loader_fini()` is
 *          called.
 *
 * @return: TPP_EOK:       Success (argument was parsed + consumed)
 * @return: TPP_ENOENT:    SOFT_ERROR: Argument could not be understood (but no
 *                         warning was emitted). You must either handle it yourself
 *                         by treating it as an argument for *your* compiler's
 *                         CLI, or as an input file for the lexer, or emit a
 *                         warning informing the user that their CLI argument
 *                         was not understood. You should also probably try to
 *                         pass it to `tpp_cli_loader_parsearg()`.
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parsearg(tpp_emitter_cli_loader *tpp_restrict self, char const *arg) {
#define tpp_streq(at, CONSTstr) \
	(tpp_memcmp(at, CONSTstr, sizeof(CONSTstr) - sizeof(char)) == 0)
	switch (self->temcl_state) {

	case TPP_EMITTER_CLI_LOADER_STATE_NORMAL: {
		if (*arg++ != '-')
			break; /* All arguments must start with at least 1 "-" */
		switch (*arg++) {

		case '-':
			/* --foo argument */
			switch (*arg++) {

			case '\0':
				self->temcl_state = TPP_EMITTER_CLI_LOADER_STATE_DDASH; /* -- */
				return TPP_EOK;

			case 'n':
#if TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
				if (tpp_streq(arg, "o-line-commands\0")) {
					return tpp_emitter_cli_enable_no_line_commands(self);
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */
				{
				}
				break;

			case 't':
#if TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
				if (tpp_streq(arg, "race-includes\0")) {
					return tpp_emitter_cli_enable_trace_includes(self);
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */
				{
				}
				break;

			case 'd':
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
				if (tpp_streq(arg, "ump")) { /* --dump= */
					arg += (sizeof("ump") - sizeof(char));
					if (*arg == '=') {
						return tpp_emitter_cli_enable_dumps(self, arg + 1);
					} else if (*arg == '\0') {
						self->temcl_state = TPP_EMITTER_CLI_LOADER_STATE_DUMP;
						return TPP_EOK;
					}
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */
				{
				}
				break;

			case 'm':
#if TPP_EMITTER_HAVE_CLI_DASH_MODE
				if (tpp_streq(arg, "ode=")) {
					arg += (sizeof("ode=") - sizeof(char));
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT
					if (tpp_streq(arg, "emit\0")) {
						tpp_errno error;
						tpp_emitter_setmode(self->temcl_emitter, TPP_EMITTER_MODE_EMIT);
						tpp_emitter_disable_NOLINE(self->temcl_emitter);
						/* Turn off emission of `SPACE` and `LF` tokens */
						error = tpp_lexer_disable_TOK_SPACE(tpp_emitter_getlexer(self->temcl_emitter));
						if (!TPP_ISERR(error))
							error = tpp_lexer_disable_TOK_LF(tpp_emitter_getlexer(self->temcl_emitter));
						return error;
					} else
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE
					if (tpp_streq(arg, "dispose\0")) {
						tpp_emitter_setmode(self->temcl_emitter, TPP_EMITTER_MODE_DISPOSE);
						return TPP_EOK;
					} else
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET
					if (tpp_streq(arg, "bracket\0")) {
						tpp_errno error;
						tpp_emitter_setmode(self->temcl_emitter, TPP_EMITTER_MODE_BRACKET);
						error = tpp_lexer_enable_TOK_SPACE(tpp_emitter_getlexer(self->temcl_emitter));
						if (!TPP_ISERR(error))
							error = tpp_lexer_enable_TOK_LF(tpp_emitter_getlexer(self->temcl_emitter));
						return error;
					} else
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED
					if (tpp_streq(arg, "typed\0")) {
						tpp_errno error;
						tpp_emitter_setmode(self->temcl_emitter, TPP_EMITTER_MODE_TYPED);
						error = tpp_lexer_enable_TOK_SPACE(tpp_emitter_getlexer(self->temcl_emitter));
						if (!TPP_ISERR(error))
							error = tpp_lexer_enable_TOK_LF(tpp_emitter_getlexer(self->temcl_emitter));
						return error;
					} else
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO
					if (tpp_streq(arg, "zero\0")) {
						tpp_errno error;
						tpp_emitter_setmode(self->temcl_emitter, TPP_EMITTER_MODE_ZERO);
						error = tpp_lexer_disable_TOK_SPACE(tpp_emitter_getlexer(self->temcl_emitter));
						if (!TPP_ISERR(error))
							error = tpp_lexer_disable_TOK_LF(tpp_emitter_getlexer(self->temcl_emitter));
						return error;
					} else
#endif /* !TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO */
					{
					}
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_MODE */
				{
				}
				break;

			default: break;
			}
			break;

		case 'P':
#if TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
			if (*arg == '\0')
				return tpp_emitter_cli_enable_no_line_commands(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */
			break;

		case 'H':
#if TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
			if (*arg == '\0')
				return tpp_emitter_cli_enable_trace_includes(self);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */
			break;

		case 'd':
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
			if (*arg)
				return tpp_emitter_cli_enable_dumps(self, arg);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */
			break;

		case 'f': {
#if (TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN ||  \
     TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA || \
     TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY ||     \
     TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT ||         \
     TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS ||   \
     TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD ||         \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE ||       \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF ||          \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS ||     \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT ||         \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS ||    \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE ||         \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS ||   \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS ||    \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE)
			bool no = false;
			if (tpp_streq(arg, "no-"))
				no = true, arg += 3;
#endif /* ... */

#if TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN
			if (tpp_streq(arg, "relaxed-macro-column\0")) {
				tpp_emitter_setfeature(self->temcl_emitter, TPP_EMITTER_FEAT_RELAXED_MACRO_COLUMN, !no);
				return TPP_EOK;
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN */
#if TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA
			if (tpp_streq(arg, "reemit-unknown-pragma\0")) {
				return tpp_emitter_set_reemit_unknown_pragma(self->temcl_emitter, !no);
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA */
#if TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY
			if (tpp_streq(arg, "working-directory\0")) {
				tpp_emitter_setfeature(self->temcl_emitter, TPP_EMITTER_FEAT_USE_CPP_DIGIT_WORKING_DIRECTORY, !no);
				return TPP_EOK;
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY */
#if TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT
			if (tpp_streq(arg, "use-cpp-digit\0")) {
				tpp_emitter_setfeature(self->temcl_emitter, TPP_EMITTER_FEAT_USE_CPP_DIGIT, !no);
				return TPP_EOK;
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT */
#if TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS
			if (tpp_streq(arg, "use-cpp-digit-flags\0")) {
				return tpp_emitter_set_use_cpp_digit_flags(self->temcl_emitter, !no);
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS */
#if TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD
			if (tpp_streq(arg, "line-threshold\0") && no) {
				tpp_emitter_disablelinethreshold(self->temcl_emitter);
				return TPP_EOK;
			} else if (tpp_streq(arg, "line-threshold=") && !no) {
				tpp_line threshold = 0;
				arg += (sizeof("line-threshold=") - sizeof(char));
				for (; tpp_ascii_isdigit(*arg); ++arg) {
					threshold *= 10;
					threshold += tpp_ascii_asdigit(*arg);
				}
				tpp_emitter_setlinethreshold(self->temcl_emitter, threshold);
				return TPP_EOK;
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD */
#if (TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE ||     \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF ||        \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS ||   \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT ||       \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS ||  \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE ||       \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS || \
     TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS)
			if (tpp_streq(arg, "normalize=")) {
				arg += (sizeof("normalize=") - sizeof(char));
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE
				if (tpp_streq(arg, "space\0")) {
					tpp_emitter_set_NORMALIZE_SPACE(self->temcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF
				if (tpp_streq(arg, "lf\0")) {
					tpp_emitter_set_NORMALIZE_LF(self->temcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS
				if (tpp_streq(arg, "strings\0")) {
					tpp_emitter_set_NORMALIZE_C_STRING(self->temcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT
				if (tpp_streq(arg, "int\0")) {
					tpp_emitter_set_NORMALIZE_C_INT(self->temcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS
				if (tpp_streq(arg, "keywords\0")) {
					tpp_emitter_set_NORMALIZE_KEYWORDS(self->temcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE
				if (tpp_streq(arg, "bse\0")) {
					tpp_emitter_set_NORMALIZE_BSE(self->temcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS
				if (tpp_streq(arg, "trigraphs\0")) {
					tpp_emitter_set_NORMALIZE_TRIGRAPHS(self->temcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS
				if (tpp_streq(arg, "digraphs\0")) {
					tpp_emitter_set_NORMALIZE_DIGRAPHS(self->temcl_emitter, !no);
					return TPP_EOK;
				} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS */
				{
				}
		} else
#endif /* ... */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE
			if (tpp_streq(arg, "normalize\0")) {
				tpp_emitter_set_NORMALIZE_SPACE(self->temcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_LF(self->temcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_C_STRING(self->temcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_C_INT(self->temcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_KEYWORDS(self->temcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_BSE(self->temcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_TRIGRAPHS(self->temcl_emitter, !no);
				tpp_emitter_set_NORMALIZE_DIGRAPHS(self->temcl_emitter, !no);
				return TPP_EOK;
			} else
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE */
			{
			}
		}	break;

		default: break;
		}
	}	break;

	case TPP_EMITTER_CLI_LOADER_STATE_DDASH:
		break; /* Don't accept any more arguments after having encountered a "--" arguments */

#if TPP_EMITTER_HAVE_CLI_DASH_DUMP
	case TPP_EMITTER_CLI_LOADER_STATE_DUMP:
		self->temcl_state = TPP_EMITTER_CLI_LOADER_STATE_NORMAL;
		return tpp_emitter_cli_enable_dumps(self, arg);
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP */

	default: tpp_unreachable();
	}
	return TPP_ENOENT;
#undef tpp_streq
}


/* Try to parse a *flag*-style parameter, that is: an argument that actually consists
 * of multiple, tightly packed parameters, whilst having a singular, leading `-` (that
 * was already skipped by the caller).
 *
 * Example: `-PH` or `-HP`
 * - This argument consists of 2 flags `-H` and `-P`, which are simply concatenated
 *   into a single argument here. This function will then parse one of those flags
 *   from `**p_arg` (iow: `**p_arg` must be one of `H` or `P`), and advance `*p_arg`
 *   to either the end of the argument, or the next *flag*-style parameter.
 *
 * @return: TPP_EOK:    Success (`*p_arg` was updated to point to the next *flag*-style
 *                      parameter, or the argument string's end)
 * @return: TPP_ENOENT: Did not recognize the flag in `**p_arg` (caller should try to
 *                      handle the flag in a different context).
 * @return: TPP_ENOMEM:    HARD_ERROR: Out of memory
 * @return: TPP_EIO:       HARD_ERROR: I/O Error
 * @return: TPP_ELEXERROR: HARD_ERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  HARD_ERROR: User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parseflag(tpp_emitter_cli_loader *tpp_restrict self, char const **p_arg) {
	char const *arg = *p_arg;
	char flag = *arg++;
	(void)self;
	(void)flag;
	(void)arg;
	switch (flag) {

#if TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
	case 'P': {
		*p_arg = arg;
		return tpp_emitter_cli_enable_no_line_commands(self);
	}	break;
#endif /* TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */

#if TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
	case 'H': {
		*p_arg = arg;
		return tpp_emitter_cli_enable_trace_includes(self);
	}	break;
#endif /* TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */

	default: break;
	}
	return TPP_ENOENT;
}



/* Convenience wrapper around `tpp_emitter_cli_loader_parsearg()`.
 * For more information, see `tpp_cli_loader_parseargv()`.
 *
 * @return: TPP_EOK:       Success (`*p_argc` and `*p_argv` were updated such that
 *                         they contain all unrecognized arguments, as well as all
 *                         input files for the lexer).
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_errno TPPCALL
tpp_emitter_cli_loader_parseargv(tpp_emitter_cli_loader *tpp_restrict self,
                                 int *p_argc, char ***p_argv) {
	tpp_errno result = TPP_EOK;
	int argc    = *p_argc;
	char **argv = *p_argv;
	unsigned int unknown_count = 0;
	while (argc > 0) {
		char *arg = argv[0];
		result = tpp_emitter_cli_loader_parsearg(self, arg);
		if (TPP_ISERR(result)) {
			if (result != TPP_ENOENT)
				break;
			/* Add "arg" to trailing list of unknown arguments */
			--argc;
			tpp_memmovedown(&argv[0], &argv[1],
			                (argc + unknown_count) *
			                sizeof(char *));
			argv[argc + unknown_count] = arg;
			++unknown_count;
			result = TPP_EOK;
			continue;
		}
		if (tpp_emitter_cli_loader_hasddash(self)) {
			if (unknown_count) {
				/* Right now, "argv" looks like this:
				 * >> argv = { "--", "file1.c", "file2.c", "-unknown-arg", "file0.c" }
				 * >> argc = 3
				 * >> unknown_count = 2
				 *
				 * Our job now is to make `argv` look like this:
				 * >> argv = { "-unknown-arg", "file0.c", "--", "file1.c", "file2.c" } */
				unsigned int shift_count = unknown_count;
				unsigned int total_count_minus_1 = argc + unknown_count - 1;
				while (shift_count--) {
					arg = argv[0];
					tpp_memmovedown(&argv[0], &argv[1], total_count_minus_1 * sizeof(char *));
					argv[total_count_minus_1] = arg;
				}
			}
			break;
		}
		++argv;
		--argc;
	}
	*p_argc = argc + unknown_count;
	*p_argv = argv;
	return result;
}

/* Ensure that `self` is in a *normal* state (meaning that there aren't any remaining,
 * unterminated multi-argument parameters). If that is not the case, then a warning
 * `TPP_W_MISSING_CLI_ARGUMENT` is emitted on `tpp_emitter_cli_loader_getemitter(self)`
 *
 * Unlike the other CLI loader functions above, this one *MUST* be called
 * *AFTER* the lexer's initial input file has been initialized, as it may
 * need to push additional files onto the `#include`-stack.
 *
 * @return: TPP_EOK:       Success
 * @return: TPP_ENOMEM:    Out of memory
 * @return: TPP_EIO:       I/O Error
 * @return: TPP_ELEXERROR: A lexer error was thrown
 * @return: TPP_EUSER(*):  User-defined error from hook */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_emitter_cli_loader_flush(tpp_emitter_cli_loader *tpp_restrict self) {
	(void)self;

	/* Emit a warning if the CLI loader isn't in a neutral state */
#if TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT
	if (self->temcl_state != TPP_EMITTER_CLI_LOADER_STATE_NORMAL &&
	    self->temcl_state != TPP_EMITTER_CLI_LOADER_STATE_DDASH) {
		tpp_errno error = tpp_emitter_cli_warnf(self->temcl_emitter, NULL, 0,
		                                        TPP_W_MISSING_CLI_ARGUMENT);
		if (TPP_ISERR(error))
			return error;
	}
#endif /* TPP_HAVE_TPP_W_MISSING_CLI_ARGUMENT */

#if (TPP_EMITTER_HAVE_CLI_DASH_DUMP_M || TPP_EMITTER_HAVE_CLI_DASH_DUMP_D || TPP_EMITTER_HAVE_CLI_DASH_DUMP_N) && TPP_HAVE_LEXER_DUMP_DEFINITIONS
	if (self->temcl_flags & _TPP_EMITTER_CLI_LOADER_FLAG_DUMP_M) {
		tpp_ssize temp;
		temp = tpp_lexer_dump_definitions(tpp_emitter_getlexer(self->temcl_emitter),
		                                  tpp_emitter_getoutput(self->temcl_emitter),
		                                  self->temcl_emitter,
		                                  TPP_LEXER_DUMP_DEFINITIONS_BUILTIN_MACROS);
		if (temp < 0)
			return TPP_SSIZE_ASERR(temp);
	}
#endif /* (TPP_EMITTER_HAVE_CLI_DASH_DUMP_M || TPP_EMITTER_HAVE_CLI_DASH_DUMP_D || TPP_EMITTER_HAVE_CLI_DASH_DUMP_N) && TPP_HAVE_LEXER_DUMP_DEFINITIONS */

	return TPP_EOK;
}

#if TPP_EMITTER_HAVE_CLI_HELP
#undef TPP_CLI_HELP1
#undef TPP_CLI_HELP2
#define TPP_CLI_HELP1(spelling, description) \
	spelling "\0\0" description "\0"
#if TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS
#define TPP_CLI_HELP2(spelling1, spelling2, description) \
	spelling1 "\0" spelling2 "\0\0" description "\0"
#else /* TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS */
#define TPP_CLI_HELP2(spelling1, spelling2, description) \
	TPP_CLI_HELP1(spelling1, description)
#endif /* !TPP_EMITTER_HAVE_CLI_HELP_ALL_SPELLINGS */

/* Returns supported CLI parameters, and human-readable information
 * for them. Same format as `tpp_cli_loader_help` (see for more info) */
TPP_CONST_IMPL char const tpp_emitter_cli_loader_help[] =
#if TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS
TPP_CLI_HELP2("-P", "--no-line-commands",
              "Disable emission of #line-directives")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_NO_LINE_COMMANDS */
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_M
TPP_CLI_HELP2("-dM", "--dump=M",
              "Dump builtin/predefined macros\n"
              "Dump #define and #undef directives as they appear\n"
              "Turn off emission of preprocessor output")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_M */
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_D
TPP_CLI_HELP2("-dD", "--dump=D",
              "Like -dM but don't turn off emission of preprocessor output")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_D */
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_N
TPP_CLI_HELP2("-dN", "--dump=N",
              "Like -dD but only print name when emitting #define")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_N */
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_I
TPP_CLI_HELP2("-dI", "--dump=I",
              "Emit #include directives")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_I */
#if TPP_EMITTER_HAVE_CLI_DASH_DUMP_U
TPP_CLI_HELP2("-dU", "--dump=U",
              "Similar to -dD, but emit #define/#undef lazily")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_DUMP_U */
#if TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES
TPP_CLI_HELP2("-H", "--trace-includes",
              "Print visual representation of #include-stack when files are pushed")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_TRACE_INCLUDES */
#if TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN
TPP_CLI_HELP1("-f[no-]relaxed-macro-column",
              "Relax column retention rules when emitting tokens from within macros")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FRELAXED_MACRO_COLUMN */
#if TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA
TPP_CLI_HELP1("-f[no-]reemit-unknown-pragma",
              "Re-emit unknown #pragma directives")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FREEMIT_UNKNOWN_PRAGMA */
#if TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY
TPP_CLI_HELP1("-f[no-]working-directory",
              "Include the preprocessors PWD in output")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FWORKING_DIRECTORY */
#if TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT
TPP_CLI_HELP1("-f[no-]use-cpp-digit",
              "Use # LINENUM instead of #line")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT */
#if TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS
TPP_CLI_HELP1("-f[no-]use-cpp-digit-flags",
              "Produce 1/2/3/4 flags in # LINENUM directives")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FUSE_CPP_DIGIT_FLAGS */
#if TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD
TPP_CLI_HELP1("-f[no-]line-threshold=COUNT",
              "Max # of blank lines before #line is emitted")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_LINE_THRESHOLD */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE
TPP_CLI_HELP1("-f[no-]normalize=space",
              "Emit SPACE tokens using only ASCII SPACE")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_SPACE */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF
TPP_CLI_HELP1("-f[no-]normalize=lf",
              "Emit LF tokens using only ASCII LF")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_LF */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS
TPP_CLI_HELP1("-f[no-]normalize=strings",
              "Re-encode string tokens as \"foo\" or 'foo' before emission")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_STRINGS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT
TPP_CLI_HELP1("-f[no-]normalize=int",
              "Re-encode integer tokens as decimals before emission")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_INT */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS
TPP_CLI_HELP1("-f[no-]normalize=keywords",
              "Re-encode keywords before emission")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_KEYWORDS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE
TPP_CLI_HELP1("-f[no-]normalize=bse",
              "Remove \\-escaped newline before emission")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_BSE */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS
TPP_CLI_HELP1("-f[no-]normalize=trigraphs",
              "Remove trigraph sequences before emission")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_TRIGRAPHS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS
TPP_CLI_HELP1("-f[no-]normalize=digraphs",
              "Remove digraph sequences before emission")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE_DIGRAPHS */
#if TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE
TPP_CLI_HELP1("-f[no-]normalize",
              "Turn all normalization on/off")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_FNORMALIZE */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT
TPP_CLI_HELP1("--mode=emit",
              "Set emitter to emit preprocessor output (default)")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_MODE_EMIT */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE
TPP_CLI_HELP1("--mode=dispose",
              "Set emitter to dispose output (except for special output)")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_MODE_DISPOSE */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET
TPP_CLI_HELP1("--mode=bracket",
              "Set emitter to print tokens in [bracket] notation\n"
              "Turn on emission of SPACE/LF tokens")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_MODE_BRACKET */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED
TPP_CLI_HELP1("--mode=typed",
              "Set emitter to print tokens in [TYPE:REPR] notation\n"
              "Turn on emission of SPACE/LF tokens")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_MODE_TYPED */
#if TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO
TPP_CLI_HELP1("--mode=zero",
              "Set emitter to print tokens in REPR\\0 notation\n"
              "Turn off emission of SPACE/LF tokens")
#endif /* TPP_EMITTER_HAVE_CLI_DASH_MODE_ZERO */
"";
#undef TPP_CLI_HELP1
#undef TPP_CLI_HELP2
#endif /* TPP_EMITTER_HAVE_CLI_HELP */

#endif /* TPP_EMITTER_HAVE_CLI */

TPP_DECL_END

#endif /* !GUARD_TPP_EMITTER_AMALGAMATION_C */
/* clang-format on */
