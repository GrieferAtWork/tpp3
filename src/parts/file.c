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
#ifndef GUARD_TPP_FILE_C
#define GUARD_TPP_FILE_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "error.h"
#include "file-io.h"
#include "file.h"
#include "keyword.h"
#include "lexer.h"
#include "macro.h"
#include "string.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* Width of `\t` as reported by `tpp_file_getlcinfo()` */
#if TPP_TABSIZE < 0
TPP_IMPL tpp_column _tpp_tabsize = -TPP_TABSIZE;
#endif /* TPP_TABSIZE < 0 */


#if TPP_HAVE_FILE_SETFILENAME
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_user_filename) ==
                  tpp_offsetof(tpp_file, tf_data.td_text.tft_user_filename));
#endif /* TPP_HAVE_FILE_SETFILENAME */

#if TPP_DEBUG && 0
#ifndef TPP_IO_CHUNKSIZE
#define TPP_IO_CHUNKSIZE 1
#endif /* !TPP_IO_CHUNKSIZE */
#ifndef TPP_IO_MINREAD
#define TPP_IO_MINREAD 1
#endif /* !TPP_IO_MINREAD */
#else /* TPP_DEBUG */
#ifndef TPP_IO_CHUNKSIZE
#define TPP_IO_CHUNKSIZE (64 * 1024)
#endif /* !TPP_IO_CHUNKSIZE */
#ifndef TPP_IO_MINREAD
#define TPP_IO_MINREAD 1024
#endif /* !TPP_IO_MINREAD */
#endif /* !TPP_DEBUG */


#if TPP_HAVE_IFDEF_STACK
/* Allocate an additional `#ifdef`-stack entry, and return a pointer to it.
 * This function will increment `self->tids_cnt`, but it is up to the
 * caller to initialize the returned `#ifdef`-stack entry
 *
 * @return: * :   The (uninitialized) `#ifdef`-stack entry
 * @return: NULL: Out of memory (`TPP_ENOMEM`) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_ifdef_stack_entry *TPPCALL
tpp_ifdef_stack_append(tpp_ifdef_stack *tpp_restrict self) {
	tpp_ifdef_stack_entry *result;
	tpp_assert(self->tids_cnt <= self->tids_alc);
	if (self->tids_cnt >= self->tids_alc) {
		tpp_ifdef_stack_entry *new_vec;
		/* Only allocate in batches-of-1: #ifdef-stacks don't usually grow very deep */
		tpp_size new_alloc = self->tids_cnt + 1;

		/* Use a minimal buffer size of "4": most files won't exceed this depth. */
		if (new_alloc < 4)
			new_alloc = 4;
		new_vec = (tpp_ifdef_stack_entry *)tpp_tryrealloc(self->tids_vec,
		                                                  new_alloc *
		                                                  sizeof(tpp_ifdef_stack_entry));
		if tpp_unlikely(!new_vec) {
			new_alloc = self->tids_cnt + 1;
			new_vec = (tpp_ifdef_stack_entry *)tpp_realloc(self->tids_vec,
			                                               new_alloc *
			                                               sizeof(tpp_ifdef_stack_entry));
			if tpp_unlikely(!new_vec)
				return NULL;
		}
		self->tids_vec = new_vec;
		self->tids_alc = new_alloc;
	}
	tpp_assert(self->tids_cnt < self->tids_alc);
	result = self->tids_vec + self->tids_cnt;
	++self->tids_cnt;
#if TPP_DEBUG
	tpp_memset(result, 0xcc, sizeof(*result));
#endif /* TPP_DEBUG */
	return result;
}

/* Do the inverse of `tpp_ifdef_stack_append()`, removing the last entry, and
 * potentially freeing unused memory.
 * The caller is responsible to ensure that `tpp_ifdef_stack_isnonempty(self)` */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_ifdef_stack_remove(tpp_ifdef_stack *tpp_restrict self) {
	tpp_assert(self->tids_cnt != 0);
	tpp_assert(self->tids_cnt <= self->tids_alc);
	--self->tids_cnt;
#ifndef __OPTIMIZE_SIZE__
	{
		tpp_size unused_count = self->tids_alc - self->tids_cnt;
		if (unused_count > 8) {
			/* Free unused memory */
			tpp_ifdef_stack_entry *new_vec;
			new_vec = (tpp_ifdef_stack_entry *)tpp_tryrealloc(self->tids_vec,
			                                                  self->tids_alc *
			                                                  sizeof(tpp_ifdef_stack_entry));
			if tpp_likely(new_vec) {
				self->tids_vec = new_vec;
				self->tids_alc = self->tids_cnt;
			}
		}
	}
#endif /* !__OPTIMIZE_SIZE__ */
}
#endif /* TPP_HAVE_IFDEF_STACK */

#if TPP_HAVE_FILE_MACRO_TRACKARGS
struct tpp_macro_argbuf;

/* Release an argument buffer back to a macro. */
TPP_INTERN_DECL TPP_NONNULL((1, 2)) void TPPCALL
tpp_macro_release_argbuf(tpp_macro *tpp_restrict macro,
                         struct tpp_macro_argbuf *tpp_restrict buffer);
#endif /* TPP_HAVE_FILE_MACRO_TRACKARGS */


/* Tell an I/O file that it has been initialized, causing its associated
 * keyword's `tkm_file_inclcount` to be updated if necessary. */
#if TPP_HAVE_KEYWORD_INCLCOUNT
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
_tpp_file_io_notify_initialized(tpp_file *tpp_restrict self) {
	tpp_assert(self->tf_kind == TPP_FILE_KIND_IO);
#if TPP_HAVE_FILE_NOKWD
	if (!(self->tf_flags & TPP_FILE_FLAGS_NOKWD) && self->tf_data.td_io.tff_name)
#else /* TPP_HAVE_FILE_NOKWD */
	if (self->tf_data.td_io.tff_name)
#endif /* !TPP_HAVE_FILE_NOKWD */
	{
		tpp_keyword *kwd;
		tpp_keyword_misc *misc;
		kwd = (tpp_keyword *)((char const *)self->tf_data.td_io.tff_name -
		                      tpp_offsetof(tpp_keyword, tk_kwd));
		misc = kwd->tk_misc;
		if (misc) {
			/* Update inclusion counter (if allocated and initialized) */
			if (misc->tkm_file_inclcount != TPP_SIZE_MAX)
				++misc->tkm_file_inclcount;
		}
	}
}
#endif /* TPP_HAVE_KEYWORD_INCLCOUNT */


/* Finalize the given file. */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_file_fini(tpp_file *tpp_restrict self) {
	if (self->tf_chunk)
		tpp_string_decref(self->tf_chunk);
#if TPP_HAVE_IFDEF_STACK
	tpp_ifdef_stack_fini(&self->tf_ifdef);
#endif /* TPP_HAVE_IFDEF_STACK */
	switch (self->tf_kind) {
	case TPP_FILE_KIND_IO:
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_KEYWORD_INCLCOUNT
#if TPP_HAVE_FILE_NOKWD
		if (!(self->tf_flags & TPP_FILE_FLAGS_NOKWD) && self->tf_data.td_io.tff_name)
#else /* TPP_HAVE_FILE_NOKWD */
		if (self->tf_data.td_io.tff_name)
#endif /* !TPP_HAVE_FILE_NOKWD */
		{
			/* If the file's keyword still has a valid "tkm_file_guard",
			 * then we can set its "TPP_KEYWORD_FLAG_HDR_GUARD_VALID" flag. */
			tpp_keyword *kwd;
			tpp_keyword_misc *misc;
			kwd = (tpp_keyword *)((char const *)self->tf_data.td_io.tff_name -
			                      tpp_offsetof(tpp_keyword, tk_kwd));
			misc = kwd->tk_misc;
			if (misc) {
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
				if (self->tf_pos >= self->tf_end && misc->tkm_file_guard) {
					/* Yes! We got a #ifndef-style #include-guard for this file now! */
					misc->tkm_flags |= TPP_KEYWORD_FLAG_HDR_GUARD_VALID;
				}
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_KEYWORD_INCLCOUNT
				if (misc->tkm_file_inclcount != TPP_SIZE_MAX)
					--misc->tkm_file_inclcount;
#endif /* TPP_HAVE_KEYWORD_INCLCOUNT */
			}
		}
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS || TPP_HAVE_KEYWORD_INCLCOUNT */
#if TPP_HAVE_FILE_NOCLOSE
		if (!(self->tf_flags & TPP_FILE_FLAGS_NOCLOSE))
#endif /* TPP_HAVE_FILE_NOCLOSE */
		{
			tpp_io_close(self->tf_data.td_io.tff_file);
		}
#if (!TPP_HAVE_USER_KEYWORDS && TPP_HAVE_LEXER_OPENFILE) || TPP_HAVE_FILE_SETFILENAME
		TPP_FALLTHRU
#endif /* (!TPP_HAVE_USER_KEYWORDS && TPP_HAVE_LEXER_OPENFILE) || TPP_HAVE_FILE_SETFILENAME */
	case TPP_FILE_KIND_TEXT:
#if !TPP_HAVE_USER_KEYWORDS && TPP_HAVE_LEXER_OPENFILE
		if (self->tf_flags & TPP_FILE_FLAGS_FREENAME) {
#if TPP_HAVE_FILE_DUMMY
			tpp_file *prev;
			/* Must also clear filename out of any dummy file that had been pushed
			 * Note that this shouldn't matter for properly generated code, since
			 * any dummy file pushed should be popped explicitly before the file
			 * that declared the dummy file ends.
			 *
			 * However, we might still get here during error recovery, or when the
			 * user was making use of manually written "# <linenum>" directives.
			 */
			for (prev = self->tf_prev;
			     prev && prev->tf_kind == TPP_FILE_KIND_DUMMY;
			     prev = prev->tf_prev) {
				if (prev->tf_data.td_dummy.tfd_name != self->tf_data.td_io.tff_name)
					break;
				prev->tf_data.td_dummy.tfd_name = NULL;
			}
#endif /* TPP_HAVE_FILE_DUMMY */

			tpp_free((char *)self->tf_data.td_io.tff_name);
		}
#if TPP_HAVE_FILE_SETFILENAME && TPP_HAVE_FILE_DUMMY
		TPP_FALLTHRU
#endif /* TPP_HAVE_FILE_SETFILENAME && TPP_HAVE_FILE_DUMMY */
#endif /* !TPP_HAVE_USER_KEYWORDS && TPP_HAVE_LEXER_OPENFILE */
#if TPP_HAVE_FILE_SETFILENAME
#if TPP_HAVE_FILE_DUMMY
	case TPP_FILE_KIND_DUMMY:
#endif /* TPP_HAVE_FILE_DUMMY */
		if (self->tf_data.td_text.tft_user_filename)
			tpp_string_decref(self->tf_data.td_text.tft_user_filename);
		break;
#endif /* TPP_HAVE_FILE_SETFILENAME */


#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO: {
		tpp_macro *macro = self->tf_data.td_macro.tfm_macro;
#if TPP_HAVE_FILE_MACRO_TRACKARGS
		if (tpp_macro_isfunction(macro)) {
			tpp_size i, macro_argc = macro->tm_data.tmd_func.tmf_argc;
			tpp_lexer_arginfo *argv = self->tf_data.td_macro.tfm_args;
			for (i = 0; i < macro_argc; ++i) {
				tpp_lexer_arginfo *arginfo = &argv[i];
				tpp_lexer_arginfo_fini(arginfo);
			}
			tpp_macro_release_argbuf(macro, (struct tpp_macro_argbuf *)argv);
		}
#endif /* TPP_HAVE_FILE_MACRO_TRACKARGS */
		tpp_assert(macro->tm_expansions != 0);
		--macro->tm_expansions;
		tpp_macro_decref(macro);
	}	break;
#endif /* TPP_HAVE_CPP_MACROS */
	default: break;
	}
	tpp_dbg_memset(self, sizeof(*self));
}

/* Update `self` according to text-data from [text,text+size) */
#if TPP_HAVE_UNICODE
TPP_IMPL TPP_WUNUSED tpp_lcinfo TPPCALL
tpp_lcinfo_account_ex(tpp_lcinfo lc, tpp_char const *text,
                      tpp_size size, tpp_file_encoding enc)
#else /* TPP_HAVE_UNICODE */
TPP_IMPL TPP_WUNUSED tpp_lcinfo TPPCALL
tpp_lcinfo_account(tpp_lcinfo lc, tpp_char const *text, tpp_size size)
#endif /* !TPP_HAVE_UNICODE */
{
	tpp_char const *endp = text + size;
	tpp_line line  = tpp_lcinfo_getline(lc);
	tpp_column col = tpp_lcinfo_getcol(lc);
	if (!tpp_lcinfo_isvalid(lc))
		return lc; /* Don't account for changes when LC is invalid */
	while (text < endp) {
		tpp_char ch = *text++;
		switch (ch) {

		case TPP_ASCII_TAB: {
			tpp_column ts = tpp_gettabsize();
			col += ts;
			col -= col % ts;
		}	break;

#if TPP_HAVE_ASSUME_ASCII_CTYPE
		case TPP_ASCII_CR:
#if TPP_HAVE_CR_LF_DETECTION
			if (text < endp && *text == TPP_ASCII_LF)
				++text;
			TPP_FALLTHRU
#endif /* TPP_HAVE_CR_LF_DETECTION */
		case TPP_ASCII_LF: {
#if TPP_HAVE_UNICODE
handle_linefeed:
#endif /* TPP_HAVE_UNICODE */
			++line;
			col = 0;
		}	break;
#endif /* TPP_HAVE_ASSUME_ASCII_CTYPE */

		default:
#if TPP_HAVE_UNICODE
			if (TPP_FILE_ENCODING_ISUTF8(enc) && tpp_ascii_ismb(ch)) {
				/* Check for unicode linefeed characters */
				tpp_unichar uch;
				--text;
				/* FIXME: What if the load-process of the current chunk has stopped in the
				 *        middle of a utf-8 sequence? When that happens, we'd be getting some
				 *        weak undefined result from `tpp_unicode_readutf8()`, and our column
				 *        counter could go out of whack (and if it would have been a unicode
				 *        linefeed, then our line counter might even break)
				 * Solution: when unicode is enabled, this function must also take some kind
				 *           of extra read/write state parameter (akin to `mbstate_t`) that
				 *           we can update if the memory being accounted ends in the middle
				 *           of a utf-8 sequence. */
				uch = tpp_unicode_readutf8(&text, endp);
				if (tpp_unicode_islf(uch))
					goto handle_linefeed;
			} else
#endif /* TPP_HAVE_UNICODE */
#if !TPP_HAVE_ASSUME_ASCII_CTYPE
			if (tpp_ascii_islf(ch)) {
#if TPP_HAVE_CR_LF_DETECTION
				if (ch == TPP_ASCII_CR && text < endp && *text == TPP_ASCII_LF)
					++text;
#endif /* TPP_HAVE_CR_LF_DETECTION */
#if TPP_HAVE_UNICODE
handle_linefeed:
#endif /* TPP_HAVE_UNICODE */
				++line;
				col = 0;
				break;
			} else
#endif /* !TPP_HAVE_ASSUME_ASCII_CTYPE */
			{
			}
			++col;
			break;
		}
	}
	return tpp_lcinfo_of(line, col);
}


#if TPP_HAVE_FILE_LC_CACHE
/* Find+return a pointer 1 past the last line-feed in [ptr,end)
 * If no such linefeed exists, return `NULL` instead. */
static TPP_WUNUSED tpp_char const *TPPCALL
tpp_lcinfo_find_last_linefeed(tpp_file const *tpp_restrict self,
                              tpp_char const *ptr, tpp_char const *end) {
	(void)self;
#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(self)) {
		while (end > ptr) {
			tpp_unichar uc;
			tpp_char const *nend = end;
			uc = tpp_unicode_readutf8_bck(ptr, &nend);
			if (tpp_unicode_islf(uc))
				return end;
			end = nend;
		}
	} else
#endif /* TPP_HAVE_UNICODE */
	{
		while (end > ptr) {
			tpp_char ch = end[-1];
			if (tpp_ascii_islf(ch))
				return end;
			--end;
		}
	}
	return NULL;
}

/* Count the # of linefeeds in [ptr,end) */
static TPP_WUNUSED tpp_line TPPCALL
tpp_lcinfo_count_linefeed(tpp_file const *tpp_restrict self,
                          tpp_char const *ptr, tpp_char const *end) {
	tpp_line result = 0;
	(void)self;
#if TPP_HAVE_UNICODE
	if (tpp_file_isutf8(self)) {
		while (ptr < end) {
			tpp_unichar uc;
			uc = tpp_unicode_readutf8(&ptr, end);
			if (tpp_unicode_islf(uc)) {
				++result;
#if TPP_HAVE_CR_LF_DETECTION
				if (uc == TPP_ASCII_CR && (ptr < end && *ptr == TPP_ASCII_LF))
					++ptr;
#endif /* TPP_HAVE_CR_LF_DETECTION */
			}
		}
	} else
#endif /* TPP_HAVE_UNICODE */
	{
		while (ptr < end) {
			tpp_char ch = *ptr++;
			if (tpp_ascii_islf(ch)) {
				++result;
#if TPP_HAVE_CR_LF_DETECTION
				if (ch == TPP_ASCII_CR && (ptr < end && *ptr == TPP_ASCII_LF))
					++ptr;
#endif /* TPP_HAVE_CR_LF_DETECTION */
			}
		}
	}
	return result;
}
#endif /* TPP_HAVE_FILE_LC_CACHE */


#if TPP_HAVE_UNICODE
#define TPP_UNICODE_16TO8_MAXBUF(num_chars) ((num_chars) * 3) /* Max # of bytes needed to convert UTF-16 to UTF-8 */
#define TPP_UNICODE_32TO8_MAXBUF(num_chars) ((num_chars) * 7) /* Max # of bytes needed to convert UTF-32 to UTF-8 */

#define TPP_FILE_UTF16_IOSIZE(bufsize) (((bufsize) / 3) * 2) /* # of bytes to read from underlying UTF-16 file */
#define TPP_FILE_UTF32_IOSIZE(bufsize) (((bufsize) / 7) * 4) /* # of bytes to read from underlying UTF-32 file */

#if TPP_HAVE_FILE_ENCODING_EMBED
#define TPP_UNICODE_EMBED_MAXBUF(num_chars) ((num_chars) * 4) /* Max # of bytes needed to convert EMBED to UTF-8 */
#define TPP_FILE_EMBED_IOSIZE(bufsize)      ((bufsize) / 4)   /* # of bytes to read from underlying EMBED file */
#define TPP_FILE_EMBED_MINEXTRA             4                 /* max(1, TPP_UNICODE_EMBED_MAXBUF(1)) */
#endif /* TPP_HAVE_FILE_ENCODING_EMBED */

#define TPP_FILE_UTF16_MINEXTRA 3  /* max(2, TPP_UNICODE_16TO8_MAXBUF(1)) */
#define TPP_FILE_UTF32_MINEXTRA 7  /* max(4, TPP_UNICODE_32TO8_MAXBUF(1)) */
#define TPP_FILE_MINEXTRA       10 /* max(1, TPP_FILE_UTF16_MINEXTRA, TPP_FILE_UTF32_MINEXTRA, TPP_FILE_EMBED_MINEXTRA) + lengthof(tff_encdat.tffed_unicode.tffu_tailv) */

#define tpp_writeutf8_rev tpp_writeutf8_rev /* Define as macro so "emitter-io.c" may re-use */
static TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_writeutf8_rev(tpp_char *dst, tpp_unichar uc) {
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

#define TPP_UTF16_HIGH_SURROGATE_MIN 0xd800
#define TPP_UTF16_HIGH_SURROGATE_MAX 0xdbff
#define TPP_UTF16_LOW_SURROGATE_MIN  0xdc00
#define TPP_UTF16_LOW_SURROGATE_MAX  0xdfff
#define TPP_UTF16_SURROGATE_SHIFT    0x10000

union tpp_word16 {
	uint_least16_t w16;
	uint_least8_t w8[2];
#define tpp_word16_getle(self)        \
	(((uint_least16_t)(self).w8[0]) | \
	 ((uint_least16_t)(self).w8[1] << 8))
#define tpp_word16_getbe(self)        \
	(((uint_least16_t)(self).w8[1]) | \
	 ((uint_least16_t)(self).w8[0] << 8))
};

union tpp_word32 {
	uint_least32_t w32;
	uint_least8_t w8[4];
#define tpp_word32_getle(self)              \
	(((uint_least32_t)(self).w8[0]) |       \
	 ((uint_least32_t)(self).w8[1] << 8) |  \
	 ((uint_least32_t)(self).w8[2] << 16) | \
	 ((uint_least32_t)(self).w8[3] << 24))
#define tpp_word32_getbe(self)              \
	(((uint_least32_t)(self).w8[3]) |       \
	 ((uint_least32_t)(self).w8[2] << 8) |  \
	 ((uint_least32_t)(self).w8[1] << 16) | \
	 ((uint_least32_t)(self).w8[0] << 24))
};

/* @param: src_count: # of utf-16 words in `src` (== # of bytes/2) */
static TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_char *TPPCALL
tpp_utf16le_to_utf8(uint_least16_t const *src, tpp_size src_count, tpp_char *dst_end) {
	for (src += src_count; src_count; --src_count) {
		uint_least16_t ord;
		union tpp_word16 word;
		word.w16 = *--src;
		ord = tpp_word16_getle(word);
		if (ord >= TPP_UTF16_LOW_SURROGATE_MIN &&
		    ord <= TPP_UTF16_LOW_SURROGATE_MAX && src_count) {
			/* Second word of surrogate pair */
			tpp_unichar uc;
			word.w16 = *--src;
			--src_count;
			uc = tpp_word16_getle(word);
			/* "ord_lo" *should* be in range:
			 * [TPP_UTF16_HIGH_SURROGATE_MIN,TPP_UTF16_HIGH_SURROGATE_MAX] */
			uc &= (TPP_UTF16_HIGH_SURROGATE_MAX - TPP_UTF16_HIGH_SURROGATE_MIN);
			uc <<= 10;
			uc |= ord - TPP_UTF16_LOW_SURROGATE_MIN;
			uc += TPP_UTF16_SURROGATE_SHIFT;
			dst_end = tpp_writeutf8_rev(dst_end, uc);
		} else {
			dst_end = tpp_writeutf8_rev(dst_end, ord);
		}
	}
	return dst_end;
}

static TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_char *TPPCALL
tpp_utf16be_to_utf8(uint_least16_t const *src, tpp_size src_count, tpp_char *dst_end) {
	for (src += src_count; src_count; --src_count) {
		uint_least16_t ord;
		union tpp_word16 word;
		word.w16 = *--src;
		ord = tpp_word16_getbe(word);
		if (ord >= TPP_UTF16_LOW_SURROGATE_MIN &&
		    ord <= TPP_UTF16_LOW_SURROGATE_MAX && src_count) {
			/* Second word of surrogate pair */
			tpp_unichar uc;
			word.w16 = *--src;
			--src_count;
			uc = tpp_word16_getbe(word);
			/* "ord_lo" *should* be in range:
			 * [TPP_UTF16_HIGH_SURROGATE_MIN,TPP_UTF16_HIGH_SURROGATE_MAX] */
			uc &= (TPP_UTF16_HIGH_SURROGATE_MAX - TPP_UTF16_HIGH_SURROGATE_MIN);
			uc <<= 10;
			uc |= ord - TPP_UTF16_LOW_SURROGATE_MIN;
			uc += TPP_UTF16_SURROGATE_SHIFT;
			dst_end = tpp_writeutf8_rev(dst_end, uc);
		} else {
			dst_end = tpp_writeutf8_rev(dst_end, ord);
		}
	}
	return dst_end;
}

/* @param: src_count: # of utf-32 words in `src` (== # of bytes/4) */
static TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_char *TPPCALL
tpp_utf32le_to_utf8(uint_least32_t const *src, tpp_size src_count, tpp_char *dst_end) {
	for (src += src_count; src_count; --src_count) {
		uint_least32_t ord;
		union tpp_word32 word;
		word.w32 = *--src;
		ord = tpp_word32_getle(word);
		dst_end = tpp_writeutf8_rev(dst_end, ord);
	}
	return dst_end;
}

static TPP_WUNUSED TPP_NONNULL((1, 3)) tpp_char *TPPCALL
tpp_utf32be_to_utf8(uint_least32_t const *src, tpp_size src_count, tpp_char *dst_end) {
	for (src += src_count; src_count; --src_count) {
		uint_least32_t ord;
		union tpp_word32 word;
		word.w32 = *--src;
		ord = tpp_word32_getbe(word);
		dst_end = tpp_writeutf8_rev(dst_end, ord);
	}
	return dst_end;
}
#else /* TPP_HAVE_UNICODE */
#define TPP_FILE_MINEXTRA 1
#endif /* !TPP_HAVE_UNICODE */

/* The minimum amount of extra read bytes must allow for the worst-case
 * buffer limits imposed by unicode conversion and tail buffering. */
#if TPP_IO_MINREAD < TPP_FILE_MINEXTRA
#undef TPP_IO_MINREAD
#define TPP_IO_MINREAD TPP_FILE_MINEXTRA
#endif /* TPP_IO_MINREAD < TPP_FILE_MINEXTRA */


#if TPP_HAVE_FILE_ENCODING_EMBED
static tpp_char *TPPCALL
tpp_embed_to_utf8(unsigned char const *src, tpp_size src_count, tpp_char *dst_end) {
	src += src_count;
	while (src_count--) {
		unsigned char b = *--src;
		*--dst_end = (char)tpp_ascii_ofdigit(b % 10);
		if (b >= 10) {
			*--dst_end = (char)tpp_ascii_ofdigit((b / 10) % 10);
			if (b >= 100)
				*--dst_end = (char)tpp_ascii_ofdigit(b / 100);
		}
		*--dst_end = ',';
	}
	return dst_end;
}
#endif /* TPP_HAVE_FILE_ENCODING_EMBED */



#if TPP_HAVE_FILE_GETHASH
static TPP_PURECALL TPP_WUNUSED tpp_hash TPPCALL
tpp_file_hash_combine(tpp_hash result, tpp_char const *tpp_restrict text, tpp_size size) {
	tpp_size i;
	for (i = 0; i < size; ++i) {
		tpp_char ch = text[i];
		result = tpp_hash_combine_char(result, ch);
	}
	return result;
}
#endif /* TPP_HAVE_FILE_GETHASH */

static TPP_NONNULL((1)) void TPPCALL
tpp_file_io_prepare_unload(tpp_file *tpp_restrict self,
                           tpp_char const *text, tpp_size size) {
	self->tf_data.td_io.tff_start_lc = tpp_lcinfo_account_ex(self->tf_data.td_io.tff_start_lc,
	                                                         text, size, self->tf_enc);
#if TPP_HAVE_FILE_GETHASH
	self->tf_data.td_io.tff_hash = tpp_file_hash_combine(self->tf_data.td_io.tff_hash, text, size);
#endif /* TPP_HAVE_FILE_GETHASH */
}

/* Try to expand the currently loaded `self->tf_chunk`:
 * - If the file's kind isn't `TPP_FILE_KIND_IO`, return `TPP_EOK`
 * - Allocate a new `tpp_string` suitable for holding both
 *   [tf_pos,tf_end), as well as at least 1 additional byte.
 *   HINT: When `!tpp_string_isshared(self->tf_chunk)`, the old
 *         chunk string may simply be re-used (since it'd be free'd
 *         during the re-assignment below anyways). If this isn't
 *         intended (iow: you want to keep the contents of the previous
 *         chunk loaded into memory), you can simply `tpp_string_incref`
 *         it before calling this function.
 * - Copy [tf_pos,tf_end) into this new string
 * - Read from the underlying file into the tail of the new string
 * #if TPP_HAVE_UNICODE
 *   - If the underlying file's encoding is TPP_FILE_ENCODING_UTF(16|32)_(LE|BE),
 *     the read data is converted to utf-8 at this point.
 * #endif // TPP_HAVE_UNICODE
 *   - If the underlying file could not be read, return `TPP_EIO`
 *   - If nothing could be read, free the new string and return `TPP_EOK`
 *   - Else:
 *     - adjust `tf_pos` to point into the new string, and
 *       set `tf_end` to point at the end of the new string.
 *     - replace `tf_chunk` with the new string
 *     - return `TPP_EOK`
 * @return: TPP_EOK:         Either the current chunk was expanded (the delta
 *                           between `tf_pos` and `tf_end` has increased), or
 *                           no further data can be read from `self`.
 * @return: TPP_EIO:         I/O error
 * @return: TPP_ENOMEM:      Out of memory
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: TPP_EWOULDBLOCK: Operation would block.
 * #endif // TPP_HAVE_FILE_NONBLOCK */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_file_expandchunk(tpp_file *tpp_restrict self) {
	/* TLDR: retain [self->tf_pos,self->tf_end) and make sure that "self->tf_end"
	 *       is increased by at least 1 byte. If that is impossible due to EOF,
	 *       don't increase "self->tf_end", but still return EOK. */
	tpp_size old_inuse, new_size, io_size;
	TPP_REF tpp_string *old_chunk;
	TPP_REF tpp_string *new_chunk;
	tpp_ssize read_status;
	tpp_char *io_dst;
	tpp_char const *base;
#if TPP_HAVE_UNICODE
	bool is_first_chunk;
again:
#endif /* TPP_HAVE_UNICODE */

	if (self->tf_kind != TPP_FILE_KIND_IO)
		return TPP_EOK;
	base = self->tf_pos;
#if TPP_HAVE_FILE_KEEPPOS
	if (self->tf_data.td_io.ttf_keep && base > self->tf_data.td_io.ttf_keep)
		base = self->tf_data.td_io.ttf_keep;
#endif /* TPP_HAVE_FILE_KEEPPOS */
	old_inuse = (tpp_size)(self->tf_end - base);
	old_chunk = self->tf_chunk;
	new_size  = TPP_IO_CHUNKSIZE;
	if (new_size < old_inuse + TPP_IO_MINREAD)
		new_size = old_inuse + TPP_IO_MINREAD;
	tpp_assert(base <= self->tf_end);
	tpp_assert(!old_chunk || base >= tpp_string_str(old_chunk));
	tpp_assert(!old_chunk || self->tf_end <= tpp_string_end(old_chunk));
	if (old_chunk && !tpp_string_isshared(old_chunk)) {
		/* Can re-use the old chunk */
		tpp_size unused_head;
		unused_head = (tpp_size)(base - tpp_string_str(old_chunk));
#ifndef __OPTIMIZE_SIZE__
		if (unused_head)
#endif /* !__OPTIMIZE_SIZE__ */
		{
			tpp_file_io_prepare_unload(self, tpp_string_str(old_chunk), unused_head);
			tpp_memmovedown(tpp_string_str(old_chunk), base, old_inuse);
			base -= unused_head;
			self->tf_pos -= unused_head;
			self->tf_end -= unused_head;
#if TPP_HAVE_FILE_KEEPPOS
			if (self->tf_data.td_io.ttf_keep)
				self->tf_data.td_io.ttf_keep -= unused_head;
#endif /* TPP_HAVE_FILE_KEEPPOS */
#if TPP_HAVE_FILE_LC_CACHE
			if (self->tf_lcpos) {
				self->tf_lcpos -= unused_head;
				if (self->tf_lcpos < base)
					self->tf_lcpos = NULL; /* Cache fell out-of-scope */
			}
#endif /* TPP_HAVE_FILE_LC_CACHE */
		}
		tpp_assert(base == tpp_string_str(old_chunk));
		tpp_assert(self->tf_end == tpp_string_str(old_chunk) + old_inuse);
		if (tpp_string_len(old_chunk) >= new_size) {
			/* Don't even need to realloc() the old chunk! */
reuse_old_chunk:
			new_chunk = old_chunk;
			new_size  = tpp_string_len(old_chunk);
		} else {
			/* Must realloc() the old chunk */
#if TPP_HAVE_FILE_LC_CACHE
			tpp_size lc_rel = (tpp_size)(self->tf_lcpos - tpp_string_str(old_chunk));
#endif /* TPP_HAVE_FILE_LC_CACHE */
#if TPP_HAVE_FILE_KEEPPOS
			tpp_size ps_rel = (tpp_size)(self->tf_pos - base);
			tpp_size kp_rel = (tpp_size)(self->tf_data.td_io.ttf_keep - base);
#endif /* TPP_HAVE_FILE_KEEPPOS */
			new_chunk = (TPP_REF tpp_string *)_tpp_string_tryrealloc(old_chunk, new_size);
			if tpp_unlikely(!new_chunk) {
				new_size = old_inuse + TPP_FILE_MINEXTRA;
				if (tpp_string_len(old_chunk) >= new_size)
					goto reuse_old_chunk;
				new_chunk = (TPP_REF tpp_string *)_tpp_string_realloc(old_chunk, new_size);
				if tpp_unlikely(!new_chunk)
					return TPP_ENOMEM;
			}
			tpp_assert(!tpp_string_isshared(new_chunk));
			new_chunk->ts_str[new_size] = '\0';
			new_chunk->ts_len = new_size;
			base = tpp_string_str(new_chunk);
#if TPP_HAVE_FILE_KEEPPOS
			self->tf_pos = base + ps_rel;
			if (self->tf_data.td_io.ttf_keep) {
				self->tf_data.td_io.ttf_keep = base + kp_rel;
			} else {
				tpp_assert(ps_rel == 0 && "Without a keep-pointer, there should be no delta between 'tf_pos' and 'base'");
			}
#else /* TPP_HAVE_FILE_KEEPPOS */
			self->tf_pos = base;
#endif /* !TPP_HAVE_FILE_KEEPPOS */
			self->tf_end = base + old_inuse;
#if TPP_HAVE_FILE_LC_CACHE
			if (self->tf_lcpos)
				self->tf_lcpos = base + lc_rel;
#endif /* TPP_HAVE_FILE_LC_CACHE */
		}
		self->tf_chunk = new_chunk;
#if TPP_HAVE_UNICODE
		is_first_chunk = false;
#endif /* TPP_HAVE_UNICODE */
	} else {
#if TPP_HAVE_FILE_KEEPPOS
		tpp_size ps_rel;
		tpp_size kp_rel;
#endif /* TPP_HAVE_FILE_KEEPPOS */
		/* Must allocate a completely new chunk */
		new_chunk = tpp_string_trymalloc(new_size);
		if tpp_unlikely(!new_chunk) {
			new_size = old_inuse + TPP_FILE_MINEXTRA;
			new_chunk = tpp_string_malloc(new_size);
			if tpp_unlikely(!new_chunk)
				return TPP_ENOMEM;
		}
		tpp_memcpy(tpp_string_str(new_chunk), base, old_inuse);
		if (old_chunk) {
			tpp_size unused_head = (tpp_size)(base - tpp_string_str(old_chunk));
#if TPP_HAVE_FILE_KEEPPOS
			ps_rel = (tpp_size)(self->tf_pos - base);
			kp_rel = (tpp_size)(self->tf_data.td_io.ttf_keep - base);
#endif /* TPP_HAVE_FILE_KEEPPOS */
			tpp_file_io_prepare_unload(self, tpp_string_str(old_chunk), unused_head);
			tpp_assert(tpp_string_isshared(old_chunk));
			tpp_string_decref_nokill(old_chunk);
#if TPP_HAVE_UNICODE
			is_first_chunk = false;
#endif /* TPP_HAVE_UNICODE */
		} else {
			tpp_assert(self->tf_pos == self->tf_end);
#if TPP_HAVE_FILE_KEEPPOS
			tpp_assert(self->tf_data.td_io.ttf_keep == NULL ||
			           self->tf_data.td_io.ttf_keep == self->tf_pos);
			ps_rel = 0;
			kp_rel = 0;
#endif /* TPP_HAVE_FILE_KEEPPOS */
			tpp_lcinfo_init(&self->tf_data.td_io.tff_start_lc, 0, 0);
#if TPP_HAVE_FILE_GETHASH
			self->tf_data.td_io.tff_hash = TPP_HASH_INITIAL;
#endif /* TPP_HAVE_FILE_GETHASH */
#if TPP_HAVE_UNICODE
			is_first_chunk = true;
#endif /* TPP_HAVE_UNICODE */
		}
#if TPP_HAVE_FILE_LC_CACHE
		if (self->tf_lcpos) {
			self->tf_lcpos = tpp_string_str(new_chunk) + (self->tf_lcpos - base);
			if (self->tf_lcpos < tpp_string_str(new_chunk))
				self->tf_lcpos = NULL; /* Cache fell out-of-scope */
		}
#endif /* TPP_HAVE_FILE_LC_CACHE */
		base = tpp_string_str(new_chunk);
#if TPP_HAVE_FILE_KEEPPOS
		self->tf_pos = base + ps_rel;
		if (self->tf_data.td_io.ttf_keep) {
			self->tf_data.td_io.ttf_keep = base + kp_rel;
		} else {
			tpp_assert(ps_rel == 0 && "Without a keep-pointer, there should be no delta between 'tf_pos' and 'base'");
		}
#else /* TPP_HAVE_FILE_KEEPPOS */
		self->tf_pos = base;
#endif /* !TPP_HAVE_FILE_KEEPPOS */
		self->tf_end   = base + old_inuse;
		self->tf_chunk = new_chunk; /* Inherit reference */
	}

	tpp_assert(self->tf_chunk == new_chunk);
	tpp_assert(base == tpp_string_str(new_chunk));
	tpp_assert(self->tf_pos >= tpp_string_str(new_chunk));
	tpp_assert(self->tf_end <= tpp_string_end(new_chunk));
	tpp_assert(self->tf_pos <= self->tf_end);
#if TPP_HAVE_FILE_KEEPPOS
	tpp_assert((self->tf_data.td_io.ttf_keep == NULL) ||
	           (self->tf_data.td_io.ttf_keep >= tpp_string_str(new_chunk) &&
	            self->tf_data.td_io.ttf_keep <= tpp_string_end(new_chunk)));
#endif /* TPP_HAVE_FILE_KEEPPOS */
	io_size = (tpp_size)(tpp_string_end(new_chunk) - self->tf_end);
	tpp_assert(io_size >= 1 && "Reallocations above should have ensured sufficient space!");
	io_dst = (tpp_char *)self->tf_end;
#if TPP_HAVE_UNICODE
	switch (self->tf_enc) {
	case TPP_FILE_ENCODING_UTF16_LE:
	case TPP_FILE_ENCODING_UTF16_BE:
		io_size = TPP_FILE_UTF16_IOSIZE(io_size);
amend_tail_data:
		if (self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc) {
			tpp_memcpy(io_dst, self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailv, self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc);
			io_dst += self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc;
			io_size -= self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc;
		}
		break;
	case TPP_FILE_ENCODING_UTF32_LE:
	case TPP_FILE_ENCODING_UTF32_BE:
		io_size = TPP_FILE_UTF32_IOSIZE(io_size);
		goto amend_tail_data;
#if TPP_HAVE_FILE_ENCODING_EMBED
	case TPP_FILE_ENCODING_EMBED:
		io_size = TPP_FILE_EMBED_IOSIZE(io_size);
		if ((tpp_uintmax)io_size > self->tf_data.td_io.tff_encdat.tffed_embedlimit) {
			io_size = (tpp_size)self->tf_data.td_io.tff_encdat.tffed_embedlimit;
			/* Check for special case: stop reading data from embedded file */
			if tpp_unlikely(io_size == 0)
				goto done; /* EOF */
#define WANT_done
		}
		break;
#endif /* TPP_HAVE_FILE_ENCODING_EMBED */
	default: break;
	}
	tpp_assert(io_size >= 1 && "Value of 'TPP_FILE_MINEXTRA' should have ensured this");
#endif /* TPP_HAVE_UNICODE */


	/* Do an I/O read from the underlying file */
#if TPP_HAVE_FILE_NONBLOCK
	read_status = tpp_io_read(self->tf_data.td_io.tff_file, io_dst, io_size,
	                          self->tf_flags & TPP_FILE_FLAGS_NONBLOCK);
#else /* TPP_HAVE_FILE_NONBLOCK */
	read_status = tpp_io_read(self->tf_data.td_io.tff_file, io_dst, io_size);
#endif /* !TPP_HAVE_FILE_NONBLOCK */

	/* Check for errors that may have happened during the read */
	if (TPP_SSIZE_ISERR(read_status))
		return TPP_SSIZE_ASERR(read_status);

	/* Detect codec + convert to utf-8 */
#if TPP_HAVE_UNICODE
	switch (self->tf_enc) {
	case TPP_FILE_ENCODING_UTF8:
		if (is_first_chunk) {
			self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc = 0;

			/* Detect BOM and multi-byte encodings */
			if ((tpp_size)read_status >= 3 && (io_dst[0] == 0xef && io_dst[1] == 0xbb && io_dst[2] == 0xbf)) {
				read_status -= 3; /* UTF-8-BOM */
				tpp_memmovedown(io_dst, io_dst + 3, read_status);
				self->tf_enc = TPP_FILE_ENCODING_FORCE_UTF8;
				if tpp_unlikely(read_status == 0)
					goto again;
			} else if ((tpp_size)read_status >= 4 && (io_dst[0] == 0x00 && io_dst[1] == 0x00 &&
			                                          io_dst[2] == 0xfe && io_dst[3] == 0xff)) {
				read_status -= 4; /* UTF-32-BE-BOM */
				tpp_memmovedown(io_dst, io_dst + 4, read_status);
				self->tf_enc = TPP_FILE_ENCODING_UTF32_BE;
				goto convert_multiword_to_utf8;
			} else if ((tpp_size)read_status >= 4 && (io_dst[0] == 0xff && io_dst[1] == 0xfe &&
			                                          io_dst[2] == 0x00 && io_dst[3] == 0x00)) {
				read_status -= 4; /* UTF-32-LE-BOM */
				tpp_memmovedown(io_dst, io_dst + 4, read_status);
				self->tf_enc = TPP_FILE_ENCODING_UTF32_LE;
				goto convert_multiword_to_utf8;
			} else if ((tpp_size)read_status >= 2 && (io_dst[0] == 0xfe && io_dst[1] == 0xff)) {
				read_status -= 2; /* UTF-16-BE-BOM */
				tpp_memmovedown(io_dst, io_dst + 2, read_status);
				self->tf_enc = TPP_FILE_ENCODING_UTF16_BE;
				goto convert_multiword_to_utf8;
			} else if ((tpp_size)read_status >= 2 && (io_dst[0] == 0xff && io_dst[1] == 0xfe)) {
				read_status -= 2; /* UTF-16-LE-BOM */
				tpp_memmovedown(io_dst, io_dst + 2, read_status);
				self->tf_enc = TPP_FILE_ENCODING_UTF16_LE;
				goto convert_multiword_to_utf8;
			} else {
				/* Guess multi-byte encodings based on present 0-bytes in first 2 characters */
				if ((tpp_size)read_status >= 8 && (io_dst[0] && !io_dst[1] && !io_dst[2] && !io_dst[3] &&
				                                   io_dst[4] && !io_dst[5] && !io_dst[6] && !io_dst[7])) {
					self->tf_enc = TPP_FILE_ENCODING_UTF32_LE;
					goto convert_multiword_to_utf8;
				}
				if ((tpp_size)read_status >= 8 && (!io_dst[0] && !io_dst[1] && !io_dst[2] && io_dst[3] &&
				                                   !io_dst[4] && !io_dst[5] && !io_dst[6] && io_dst[7])) {
					self->tf_enc = TPP_FILE_ENCODING_UTF32_BE;
					goto convert_multiword_to_utf8;
				}
				if ((tpp_size)read_status >= 4 && (io_dst[0] && !io_dst[1] &&
				                                   io_dst[2] && !io_dst[3])) {
					self->tf_enc = TPP_FILE_ENCODING_UTF16_LE;
					goto convert_multiword_to_utf8;
				}
				if ((tpp_size)read_status >= 4 && (!io_dst[0] && io_dst[1] &&
				                                   !io_dst[2] && io_dst[3])) {
					self->tf_enc = TPP_FILE_ENCODING_UTF16_BE;
					goto convert_multiword_to_utf8;
				}
			}
			/* Continue to assume that input is encoded as TPP_FILE_ENCODING_UTF8 */
		} else {
			/* No conversion needed... */
		}
		break;
	case TPP_FILE_ENCODING_ASCII:
	case TPP_FILE_ENCODING_FORCE_UTF8:
		/* No conversion needed for these 2... */
		break;
	case TPP_FILE_ENCODING_UTF16_LE:
	case TPP_FILE_ENCODING_UTF16_BE:
	case TPP_FILE_ENCODING_UTF32_LE:
	case TPP_FILE_ENCODING_UTF32_BE: {
		tpp_char *dst_base, *dst_end;
		tpp_size tail_mask, tail_size, out_size;
convert_multiword_to_utf8:
		if tpp_unlikely(read_status == 0)
			goto done; /* EOF */
#define WANT_done
		if (self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc) {
			io_dst -= self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc;
			read_status += self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc;
			self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc = 0;
		}

		/* Store unaligned tail data in out-of-band "tail" buffer */
		tail_mask = (self->tf_enc == TPP_FILE_ENCODING_UTF16_LE ||
		             self->tf_enc == TPP_FILE_ENCODING_UTF16_BE)
		            ? 1
		            : 3;
		tail_size = (tpp_size)read_status & tail_mask;
		if (tail_size) {
			tpp_char *tail_base;
			read_status -= tail_size;
			tail_base = io_dst + (tpp_size)read_status;
			self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc = (uint_least8_t)tail_size;
			tpp_memcpy(self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailv, tail_base, tail_size);
		}

		dst_end = tpp_string_end(new_chunk);
		switch (self->tf_enc) {
		case TPP_FILE_ENCODING_UTF16_LE:
		case TPP_FILE_ENCODING_UTF16_BE: {
			uint_least16_t const *dst16 = (uint_least16_t const *)io_dst;
			tpp_size words = (tpp_size)read_status / 2;
			union tpp_word16 raw_last_word;
			uint_least16_t last_word;
			if tpp_unlikely(!words) {
				dst_base = dst_end;
				break;
			}
			raw_last_word.w16 = dst16[words - 1];
			if (self->tf_enc == TPP_FILE_ENCODING_UTF16_LE) {
				last_word = tpp_word16_getle(raw_last_word);
			} else {
				last_word = tpp_word16_getbe(raw_last_word);
			}
			if (last_word >= TPP_UTF16_HIGH_SURROGATE_MIN &&
			    last_word <= TPP_UTF16_HIGH_SURROGATE_MAX) {
				/* Last word is a HIGH_UTF16 surrogate -> exclude from conversion
				 * and add to tail (this character can only be decoded when the
				 * "next word" has also been fully read, where that "next word"
				 * should be the LOW_UTF16 surrogate) */
				tpp_memmoveup(self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailv + 2,
				              self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailv, tail_size);
				self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailv[0] = raw_last_word.w8[0];
				self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailv[1] = raw_last_word.w8[1];
				self->tf_data.td_io.tff_encdat.tffed_unicode.tffu_tailc += 2;
				--words;
			}
			if (self->tf_enc == TPP_FILE_ENCODING_UTF16_LE) {
				dst_base = tpp_utf16le_to_utf8(dst16, words, dst_end);
			} else {
				dst_base = tpp_utf16be_to_utf8(dst16, words, dst_end);
			}
		}	break;

		case TPP_FILE_ENCODING_UTF32_LE:
			dst_base = tpp_utf32le_to_utf8((uint_least32_t const *)io_dst,
			                               (tpp_size)read_status / 4,
			                               dst_end);
			break;
		case TPP_FILE_ENCODING_UTF32_BE:
			dst_base = tpp_utf32be_to_utf8((uint_least32_t const *)io_dst,
			                               (tpp_size)read_status / 4,
			                               dst_end);
			break;
		default: tpp_unreachable();
		}

		/* Shift decoded unicode data to its final position. */
		out_size = (tpp_size)(dst_end - dst_base);
		io_dst = (tpp_char *)self->tf_end;
		tpp_memmovedown(io_dst, dst_base, out_size);
		if tpp_unlikely(out_size == 0) {
			/* This can happen (e.g.) when a single byte was read from a pipe,
			 * but the multi-byte codec requires at least 2/4 bytes in order
			 * to form the next byte(s) of UTF-8 input data.
			 *
			 * So-as not to confuse the caller into thinking that EOF has been
			 * reached, simply loop around and read some more... */
			goto again;
		}

		self->tf_end += out_size;
		goto done;
#define WANT_done
	}	break;

#if TPP_HAVE_FILE_ENCODING_EMBED
	case TPP_FILE_ENCODING_EMBED: {
		tpp_char *dst_base, *dst_end;
		tpp_size out_size;
		if ((tpp_uintmax)(tpp_size)read_status > self->tf_data.td_io.tff_encdat.tffed_embedlimit)
			read_status = (tpp_size)self->tf_data.td_io.tff_encdat.tffed_embedlimit;
		self->tf_data.td_io.tff_encdat.tffed_embedlimit -= (tpp_size)read_status;
		if tpp_unlikely(read_status == 0)
			goto done; /* EOF */
		tpp_assert(io_dst == (tpp_char *)self->tf_end);
		dst_end = tpp_string_end(new_chunk);
		dst_base = tpp_embed_to_utf8((unsigned char const *)io_dst,
		                             (tpp_size)read_status, dst_end);
		if (is_first_chunk)
			++dst_base; /* Skip over first leading "," in initial chunk */
		out_size = (tpp_size)(dst_end - dst_base);
		tpp_memmovedown(io_dst, dst_base, out_size);
		self->tf_end += out_size;
		goto done;
#define WANT_done
	}	break;
#endif /* TPP_HAVE_FILE_ENCODING_EMBED */

	default: tpp_unreachable();
	}
#elif TPP_HAVE_FILE_ENCODING_EMBED
#error "`TPP_HAVE_FILE_ENCODING_EMBED` cannot be enabled without `TPP_HAVE_UNICODE`"
#endif /* ... */

	/* Remember that more buffer space is now available! */
	self->tf_end += (tpp_size)read_status;
	/* Debug-initialize the unused buffer tail */
done:
#if TPP_DEBUG
	tpp_memset((tpp_char *)self->tf_end, 0x66,
	           (tpp_size)(tpp_string_end(self->tf_chunk) - self->tf_end));
#endif /* !TPP_DEBUG */
	return TPP_EOK;
}



/* These are needed for the shared
 * >> case TPP_FILE_KIND_IO:
 * >> case TPP_FILE_KIND_TEXT: {
 * in `tpp_file_getlcinfo()` */
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_name) ==
                  tpp_offsetof(tpp_file, tf_data.td_text.tft_name));
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_start_lc) ==
                  tpp_offsetof(tpp_file, tf_data.td_text.tft_start_lc));
#if TPP_HAVE_FILE_DUMMY
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_name) ==
                  tpp_offsetof(tpp_file, tf_data.td_dummy.tfd_name));
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_start_lc) ==
                  tpp_offsetof(tpp_file, tf_data.td_dummy.tfd_start_lc));
#endif /* TPP_HAVE_FILE_DUMMY */


#if TPP_HAVE_CPP_MACROS
/* Figure out the line/column of `pos` in `expanded_text`, as produced
 * by `self`, which must be `TPP_MACRO_KIND_ISFUNC(self->tm_kind)`. */
TPP_INTERN_DECL TPP_NONNULL((1, 2, 3, 4, 5)) void TPPCALL
tpp_macro_func_lcinfo(tpp_macro const *self,
                      tpp_file const *expanded_text_file,
                      tpp_string const *expanded_text,
                      tpp_char const *pos,
                      tpp_lcinfo_ex *tpp_restrict result);
#endif /* TPP_HAVE_CPP_MACROS */


/* Return line/column information (0-based) for `pos`
 * @return: TPP_LCINFO_INVALID: line/column information could not be determined */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_lcinfo TPPCALL
tpp_file_getlcinfo(tpp_file *tpp_restrict self, tpp_char const *pos) {
	tpp_lcinfo result;
	if tpp_unlikely(!self->tf_chunk) {
		if (self->tf_kind == TPP_FILE_KIND_IO)
			return tpp_lcinfo_of(0, 0); /* Start of I/O file with nothing loaded, yet */
#if TPP_HAVE_FILE_DUMMY
		if (self->tf_kind == TPP_FILE_KIND_DUMMY)
			return self->tf_data.td_dummy.tfd_start_lc;
#endif /* TPP_HAVE_FILE_DUMMY */
		return TPP_LCINFO_INVALID;
	}

	/* Ensure that the given point is part of the currently loaded chunk.
	 * It may not necessarily be for reasons such as `tpp_lexer_parsestring_cb()`
	 * being used with `TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS`. In all of these
	 * cases, LC-information is simply not available */
	if (pos < tpp_string_str(self->tf_chunk) ||
	    pos > tpp_string_end(self->tf_chunk))
		return TPP_LCINFO_INVALID;

	/* Check against the cache */
#if TPP_HAVE_FILE_LC_CACHE
	if (self->tf_lcpos) {
		tpp_size delta_from_chunk;
		tpp_size delta_from_lcpos;
		if (pos >= self->tf_lcpos) {
			result = self->tf_lcval;
			result = tpp_lcinfo_account_ex(result, self->tf_lcpos,
			                               (tpp_size)(pos - self->tf_lcpos),
			                               self->tf_enc);
			goto done;
		}
		delta_from_chunk = (tpp_size)(pos - tpp_string_str(self->tf_chunk));
		delta_from_lcpos = (tpp_size)(self->tf_lcpos - pos);
		if (delta_from_chunk > (delta_from_lcpos * 2)) {
			/* Given "pos" is much closer to "tf_lcpos", so try to work off of the cache! */
			tpp_char const *last_linefeed;
			last_linefeed = tpp_lcinfo_find_last_linefeed(self, tpp_string_str(self->tf_chunk), pos);
			if (last_linefeed) {
				tpp_line num_lf = tpp_lcinfo_count_linefeed(self, last_linefeed, self->tf_lcpos);
				tpp_line last_linefeed_lno = tpp_lcinfo_getline(self->tf_lcval) - num_lf;
				tpp_lcinfo_init(&result, last_linefeed_lno, 0);
				result = tpp_lcinfo_account_ex(result, last_linefeed,
				                               (tpp_size)(pos - last_linefeed),
				                               self->tf_enc);
				goto done;
			}
		}
	}
#endif /* TPP_HAVE_FILE_LC_CACHE */

	/* Cannot use cache -> must calculate off of known file-base */
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO:
	case TPP_FILE_KIND_TEXT:
#if TPP_HAVE_FILE_DUMMY
	case TPP_FILE_KIND_DUMMY:
#endif /* TPP_HAVE_FILE_DUMMY */
	{
		result = self->tf_data.td_io.tff_start_lc;
		result = tpp_lcinfo_account_ex(result, tpp_string_str(self->tf_chunk),
		                               (tpp_size)(pos - tpp_string_str(self->tf_chunk)),
		                               self->tf_enc);
	}	break;

#if TPP_HAVE_FILE_SUBTEXT
	case TPP_FILE_KIND_SUBTEXT: {
		tpp_file *parent = self;
		do {
			tpp_assert(self->tf_tprev && "SUBTEXT file without traceback predecessor");
			parent = parent->tf_tprev;
		} while (parent->tf_kind == TPP_FILE_KIND_SUBTEXT);
		if (parent->tf_chunk == self->tf_chunk)
			return tpp_file_getlcinfo(parent, pos);
#if 0 /* This would also be (kind-of) valid. But what would be event better, is if we could
       * reverse-engineer how "self->tf_chunk" was constructed out of "parent->tf_chunk"... */
		return tpp_file_getlcinfo(parent, parent->tf_tpos);
#else
		return TPP_LCINFO_INVALID;
#endif
	}	break;
#endif /* TPP_HAVE_FILE_SUBTEXT */

#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO: {
		tpp_macro const *macro = self->tf_data.td_macro.tfm_macro;
		if (self->tf_chunk != macro->tm_body_chunk) {
			/* Complicated case (where caching is also impossible):
			 * This file is the result of a function-style macro expansion,
			 * meaning that origin line/column information is related-to,
			 * but not linear to actual text data:
			 *
			 * Line   012345678911111111112
			 *                  01234567890
			 * [1] >> #define add(a, b) a+b
			 * [2] >> add(foo, bar);
			 *
			 * OUTPUT:
			 * >> foo   // line=1, col=18
			 * >> +     // line=1, col=19
			 * >> bar   // line=1, col=20 */
			tpp_lcinfo_ex info;
			tpp_assert(TPP_MACRO_KIND_ISFUNC(macro->tm_kind));
			tpp_macro_func_lcinfo(macro, self, self->tf_chunk, pos, &info);
			result = info.tlcix_info;
			goto done_nocache;
		} else {
			result = macro->tm_body_lc;
			result = tpp_lcinfo_account_ex(result, macro->tm_body_start,
			                               (tpp_size)(pos - macro->tm_body_start),
			                               self->tf_enc);
		}
	}	break;
#endif /* TPP_HAVE_CPP_MACROS */
	default: tpp_unreachable();
	}

#if TPP_HAVE_FILE_LC_CACHE
done:
	self->tf_lcpos = pos;
	self->tf_lcval = result;
#endif /* TPP_HAVE_FILE_LC_CACHE */
#if TPP_HAVE_CPP_MACROS
done_nocache:
#endif /* TPP_HAVE_CPP_MACROS */
	return result;
}

/* Same as `tpp_file_getlcinfo()`, but if the current file is an expanded macro, see if
 * the specified `pos` points into the expanded portion of a macro argument, in which
 * case this function also (tries to) include information on where that argument was
 * projected from. */
#if TPP_HAVE_CPP_MACROS
TPP_IMPL TPP_NONNULL((1, 2, 3)) void TPPCALL
tpp_file_getlcinfo_ex(tpp_file *tpp_restrict self, tpp_char const *pos,
                      tpp_lcinfo_ex *tpp_restrict result) {
#if TPP_HAVE_FILE_SUBTEXT
	if (self->tf_kind == TPP_FILE_KIND_SUBTEXT) {
		tpp_file *parent = self;
		do {
			tpp_assert(self->tf_tprev && "SUBTEXT file without traceback predecessor");
			parent = parent->tf_tprev;
		} while (parent->tf_kind == TPP_FILE_KIND_SUBTEXT);
		if (parent->tf_chunk == self->tf_chunk)
			self = parent;
	}
#endif /* TPP_HAVE_FILE_SUBTEXT */

	if (self->tf_chunk && self->tf_kind == TPP_FILE_KIND_MACRO) {
		tpp_macro const *macro = self->tf_data.td_macro.tfm_macro;
		tpp_assert(pos >= tpp_string_str(self->tf_chunk));
		tpp_assert(pos <= tpp_string_end(self->tf_chunk));
		if (self->tf_chunk != macro->tm_body_chunk) {
			tpp_assert(TPP_MACRO_KIND_ISFUNC(macro->tm_kind));
			tpp_macro_func_lcinfo(macro, self, self->tf_chunk, pos, result);
			return;
		}
	}
	result->tlcix_projfile = NULL;
	result->tlcix_info = tpp_file_getlcinfo(self, pos);
}
#endif /* TPP_HAVE_CPP_MACROS */



/* These are needed for the shared
 * >> case TPP_FILE_KIND_IO:
 * >> case TPP_FILE_KIND_TEXT:
 * in `tpp_file_getrealfilename()` */
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_name) ==
                  tpp_offsetof(tpp_file, tf_data.td_text.tft_name));
#if TPP_HAVE_FILE_DUMMY
TPP_STATIC_ASSERT(tpp_offsetof(tpp_file, tf_data.td_io.tff_name) ==
                  tpp_offsetof(tpp_file, tf_data.td_dummy.tfd_name));
#endif /* TPP_HAVE_FILE_DUMMY */

/* Returns the filename of `self`, or `NULL` if unknown. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_getrealfilename(tpp_file const *tpp_restrict self) {
#if TPP_HAVE_FILE_SUBTEXT
again:
#endif /* TPP_HAVE_FILE_SUBTEXT */
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO:
	case TPP_FILE_KIND_TEXT:
#if TPP_HAVE_FILE_DUMMY
	case TPP_FILE_KIND_DUMMY:
#endif /* TPP_HAVE_FILE_DUMMY */
		return self->tf_data.td_io.tff_name;

#if TPP_HAVE_FILE_SUBTEXT
	case TPP_FILE_KIND_SUBTEXT:
		tpp_assert(self->tf_tprev && "SUBTEXT file without traceback predecessor");
		self = self->tf_tprev;
		goto again;
#endif /* TPP_HAVE_FILE_SUBTEXT */

#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO:
		return self->tf_data.td_macro.tfm_macro->tm_deffile;
#endif /* TPP_HAVE_CPP_MACROS */

	default: tpp_unreachable();
	}
}

/* Same as `tpp_file_getrealfilename()`, but may be overwritten by `#line` directives */
#if TPP_HAVE_FILE_SETFILENAME
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_getfilename(tpp_file const *tpp_restrict self) {
#if TPP_HAVE_FILE_SUBTEXT
again:
#endif /* TPP_HAVE_FILE_SUBTEXT */
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO:
	case TPP_FILE_KIND_TEXT:
#if TPP_HAVE_FILE_DUMMY
	case TPP_FILE_KIND_DUMMY:
#endif /* TPP_HAVE_FILE_DUMMY */
	{
		TPP_REF tpp_string *user;
		user = self->tf_data.td_io.tff_user_filename;
		if (user != NULL)
			return (char const *)user->ts_str;
		return self->tf_data.td_io.tff_name;
	}	break;

#if TPP_HAVE_FILE_SUBTEXT
	case TPP_FILE_KIND_SUBTEXT:
		tpp_assert(self->tf_tprev && "SUBTEXT file without traceback predecessor");
		self = self->tf_tprev;
		goto again;
#endif /* TPP_HAVE_FILE_SUBTEXT */

#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO:
		return self->tf_data.td_macro.tfm_macro->tm_deffile;
#endif /* TPP_HAVE_CPP_MACROS */

	default: tpp_unreachable();
	}
}

/* Same as `tpp_file_getfilename()`, but if the filename was overwritten
 * by use of `tpp_file_setfilename()`, this returns the string object that
 * was passed during that override (so the caller can `tpp_string_incref()`
 * that string to preserve it across further tpp_file_setfilename-calls).
 *
 * WARNING: This returns `NULL` if the current filename wasn't set by a call
 *          to `tpp_file_setfilename()`, even when `tpp_file_getfilename()`
 *          would return non-NULL */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_string *TPPCALL
tpp_file_getfilenamestr(tpp_file const *tpp_restrict self) {
#if TPP_HAVE_FILE_SUBTEXT
again:
#endif /* TPP_HAVE_FILE_SUBTEXT */
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO:
	case TPP_FILE_KIND_TEXT:
#if TPP_HAVE_FILE_DUMMY
	case TPP_FILE_KIND_DUMMY:
#endif /* TPP_HAVE_FILE_DUMMY */
		return self->tf_data.td_io.tff_user_filename;

#if TPP_HAVE_FILE_SUBTEXT
	case TPP_FILE_KIND_SUBTEXT:
		tpp_assert(self->tf_tprev && "SUBTEXT file without traceback predecessor");
		self = self->tf_tprev;
		goto again;
#endif /* TPP_HAVE_FILE_SUBTEXT */

#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO:
		break;
#endif /* TPP_HAVE_CPP_MACROS */

	default: tpp_unreachable();
	}
	return NULL;
}

/* Sets the user-filename override of `self` to `filename`
 *
 * NOTE: The caller must ensure that:
 *       >> self->tf_kind == TPP_FILE_KIND_IO ||
 *       >> self->tf_kind == TPP_FILE_KIND_TEXT;
 *
 * You may also pass `NULL` for `filename` to disable the override */
TPP_IMPL TPP_NONNULL((1)) void TPPCALL
tpp_file_setfilename(tpp_file *tpp_restrict self, tpp_string *filename) {
	tpp_string *old_override;
	tpp_assert(self->tf_kind == TPP_FILE_KIND_IO ||
	           self->tf_kind == TPP_FILE_KIND_TEXT);
	old_override = self->tf_data.td_text.tft_user_filename;
	self->tf_data.td_text.tft_user_filename = filename;
	if (filename)
		tpp_string_incref(filename);
	if (old_override)
		tpp_string_decref(old_override);
}
#endif /* TPP_HAVE_FILE_SETFILENAME */


/* Set the (0-based) line that applies to `pos`
 * (as returned by `tpp_file_getlcinfo()`) in `self`
 *
 * NOTE: The caller must ensure that:
 *       >> self->tf_kind == TPP_FILE_KIND_IO ||
 *       >> self->tf_kind == TPP_FILE_KIND_TEXT; */
#if TPP_HAVE_FILE_SETLINE
TPP_IMPL TPP_NONNULL((1, 2)) void TPPCALL
tpp_file_setline(tpp_file *tpp_restrict self,
                 tpp_char const *pos, tpp_line line) {
	tpp_lcinfo cur_info;
	tpp_line cur_line, delta, start_line;
	tpp_column start_col;
	tpp_assert(self->tf_kind == TPP_FILE_KIND_IO ||
	           self->tf_kind == TPP_FILE_KIND_TEXT);
	cur_info = tpp_file_getlcinfo(self, pos);
	if (!tpp_lcinfo_isvalid(cur_info))
		return;
	cur_line   = tpp_lcinfo_getline(cur_info);
	delta      = line - cur_line;
	start_line = tpp_lcinfo_getline(self->tf_data.td_text.tft_start_lc) + delta;
	start_col  = tpp_lcinfo_getcol(self->tf_data.td_text.tft_start_lc);
	tpp_lcinfo_init(&self->tf_data.td_text.tft_start_lc, start_line, start_col);

	/* Invalidate cache */
#if TPP_HAVE_FILE_LC_CACHE
	self->tf_lcpos = NULL;
#endif /* TPP_HAVE_FILE_LC_CACHE */
}
#endif /* TPP_HAVE_FILE_SETLINE */


#if TPP_HAVE_FILE_GETHASH
/* Return the hash of all (decoded) bytes read from `self` up to (but not including) `pos`
 * This *includes* the bytes of any already-unloaded chunk of `self`, though `pos` must
 * point into the current chunk (past hash values from previous chunks cannot be determined)
 *
 * Also note that the hash can *only* be determined when `tpp_file_getchunk(self) != NULL`.
 * If the file doesn't have an input chunk (e.g.: its contents are statically allocated),
 * then this function always returns the same value. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_hash TPPCALL
tpp_file_gethash(tpp_file const *tpp_restrict self, tpp_char const *pos) {
	tpp_hash result = 1;
	if (self->tf_chunk != NULL &&
	    pos >= tpp_string_str(self->tf_chunk) &&
	    pos <= tpp_string_end(self->tf_chunk)) {
		if (self->tf_kind == TPP_FILE_KIND_IO)
			result = self->tf_data.td_io.tff_hash;
		result = tpp_file_hash_combine(result, tpp_string_str(self->tf_chunk),
		                               (tpp_size)(pos - tpp_string_str(self->tf_chunk)));
	}
	return result;
}
#endif /* TPP_HAVE_FILE_GETHASH */


#if TPP_HAVE_FILE_GETFULLHASH
/* Same as `tpp_file_gethash()`, but also includes the hash values of all parent files,
 * such that the `tpp_file_gethash(f, tpp_file_getlastpos(f))` of every file reachable
 * via `tpp_file_getprev()` is included in the return value in one way or another. Note
 * that the value returned here may be different from the hash that would be calculated
 * if all `#include`-ed files had been inlined into each other. */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_hash TPPCALL
tpp_file_getfullhash(tpp_file const *tpp_restrict self, tpp_char const *pos) {
	tpp_hash result = tpp_file_gethash(self, pos);
	tpp_file const *iter = self;
	while ((iter = tpp_file_getprev(iter)) != NULL) {
		tpp_hash iter_hash = tpp_file_gethash(iter, tpp_file_getlastpos(iter));
		result = tpp_hash_combine_hash(result, iter_hash);
	}
	return result;
}
#endif /* TPP_HAVE_FILE_GETFULLHASH */



#if TPP_HAVE_FILE_GETREALFILENAMEKWD
/* Returns the filename *keyword* (which may not always be available,
 * even when `tpp_file_getrealfilename()` returns non-NULL) */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) struct tpp_keyword *TPPCALL
tpp_file_getrealfilenamekwd(tpp_file const *tpp_restrict self) {
#if TPP_HAVE_FILE_SUBTEXT
again:
#endif /* TPP_HAVE_FILE_SUBTEXT */
	switch (self->tf_kind) {

	case TPP_FILE_KIND_IO:
	case TPP_FILE_KIND_TEXT: {
		char const *filename;
#if TPP_HAVE_FILE_NOKWD
		if (self->tf_flags & TPP_FILE_FLAGS_NOKWD)
			return NULL; /* Name isn't actually a keyword... */
#endif /* TPP_HAVE_FILE_NOKWD */
		filename = self->tf_data.td_io.tff_name;
		if (filename == NULL)
			return NULL;
		return (struct tpp_keyword *)(filename - offsetof(struct tpp_keyword, tk_kwd));
	}	break;

#if TPP_HAVE_FILE_SUBTEXT
	case TPP_FILE_KIND_SUBTEXT:
		tpp_assert(self->tf_tprev && "SUBTEXT file without traceback predecessor");
		self = self->tf_tprev;
		goto again;
#endif /* TPP_HAVE_FILE_SUBTEXT */

#if TPP_HAVE_FILE_DUMMY
	case TPP_FILE_KIND_DUMMY:
#endif /* TPP_HAVE_FILE_DUMMY */
#if TPP_HAVE_CPP_MACROS
	case TPP_FILE_KIND_MACRO:
#endif /* TPP_HAVE_CPP_MACROS */
		return NULL;

	default: tpp_unreachable();
	}
}
#endif /* TPP_HAVE_FILE_GETREALFILENAMEKWD */


#if TPP_HAVE_INCLUDE_STACK
/* Returns the last file in the `#include`-stack (using `tf_tprev`) */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getbasefile(tpp_file const *tpp_restrict self) {
	while (!tpp_file_isbasefile(self))
		self = self->tf_tprev;
	return (tpp_file *)self;
}

#if TPP_HAVE_CPP_MACROS || TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_FILE_DUMMY
/* Returns the first `tf_kind==TPP_FILE_KIND_IO || tf_kind==TPP_FILE_KIND_TEXT` file
 * in the `#include`-stack (using `tf_tprev`). If no such file exists, returns `NULL` */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_gettextfile(tpp_file const *tpp_restrict self) {
	tpp_file *iter = (tpp_file *)self;
	while (iter->tf_kind != TPP_FILE_KIND_IO &&
	       iter->tf_kind != TPP_FILE_KIND_TEXT) {
		iter = iter->tf_tprev;
		if (iter == NULL)
			break;
	}
	return iter;
}

/* Similar to `tpp_file_gettextfile()`, but re-return `self` instead of returning `NULL`.
 * Also: try not to return a file with invalid L/C information (which could otherwise
 *       happen as a result of manually pushed text files that don't contain L/C info)
 *
 * The term `lc` here refers to the fact that this is the file that's used as basis
 * for the builtin __FILE__, __LINE__ and __COLUMN__ macros. */
TPP_IMPL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getlcfile(tpp_file const *tpp_restrict self) {
	tpp_file *iter = (tpp_file *)self;
	while (!tpp_file_haslcinfo(iter)) {
		iter = iter->tf_tprev;
		if (iter == NULL) {
			iter = tpp_file_gettextfile(self);
			return iter ? iter : (tpp_file *)self;
		}
	}
	return iter;
}
#endif /* TPP_HAVE_CPP_MACROS || TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_FILE_DUMMY */

#if TPP_HAVE_FILE_DUMMY
/* Push/pop a so-called *dummy-file* that goes between `self` and its parent,
 * which is a copy of `self`, but with all file/chunk-data stripped, except
 * that the current values for the following are preserved (for tracebacks):
 * - `tpp_file_getfilename(self)`
 * - `tpp_file_getlcinfo(self, pos)`   (returned by `tpp_file_getlcinfo()` for any pointer)
 *
 * NOTES:
 * - The caller must ensure that `self->tf_kind == TPP_FILE_KIND_IO || self->tf_kind == TPP_FILE_KIND_TEXT`
 * - Used to implement gcc's `# <linenum> <filename> 1` directive
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_IMPL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_file_pushdummy(tpp_file *tpp_restrict self, tpp_char const *pos) {
	tpp_char const dummy_text[] = { 0 };
	tpp_file *const dummy = tpp_file_alloc();
	if tpp_unlikely(!dummy)
		return TPP_ENOMEM;
	tpp_assert(self->tf_kind == TPP_FILE_KIND_IO ||
	           self->tf_kind == TPP_FILE_KIND_TEXT);
	tpp_dbg_memset(dummy, sizeof(*dummy));
	dummy->tf_tpos  = dummy_text;
	dummy->tf_pos   = dummy_text;
	dummy->tf_chunk = NULL;
	dummy->tf_end   = dummy_text;
	dummy->tf_prev  = self->tf_prev;
	dummy->tf_tprev = self->tf_tprev;
	self->tf_prev   = dummy;
	self->tf_tprev  = dummy;
	(void)0 _tpp_file_init_lcpos(dummy);
	(void)0 _tpp_file_init_ifdef(dummy);
	dummy->tf_kind = TPP_FILE_KIND_DUMMY;
	(void)0 _tpp_file_init_enc(dummy);
	(void)0 _tpp_file_init_flags(dummy, TPP_FILE_FLAGS_NORMAL);
	dummy->tf_data.td_dummy.tfd_name     = self->tf_data.td_io.tff_name;
	dummy->tf_data.td_dummy.tfd_start_lc = tpp_file_getlcinfo(self, pos);
#if TPP_HAVE_FILE_SETFILENAME
	dummy->tf_data.td_dummy.tfd_user_filename = self->tf_data.td_text.tft_user_filename;
	if (dummy->tf_data.td_dummy.tfd_user_filename)
		tpp_string_incref(dummy->tf_data.td_dummy.tfd_user_filename);
#endif /* TPP_HAVE_FILE_SETFILENAME */
	return TPP_EOK;
}


/* Check if `self` has a parent, and if so: if that parent is a *dummy*
 * file. If that is the case, unlink that dummy file from the `#include`-
 * stack and free it. Otherwise, do nothing
 *
 * NOTES:
 * - The caller must ensure that `self->tf_kind == TPP_FILE_KIND_IO || self->tf_kind == TPP_FILE_KIND_TEXT`
 * - Used to implement gcc's `# <linenum> <filename> 2` directive
 *
 * @return: true:  Success (a dummy file was popped)
 * @return: false: Failure (there was no dummy file to pop) */
TPP_IMPL TPP_NONNULL((1)) bool TPPCALL
tpp_file_popdummy(tpp_file *tpp_restrict self) {
	tpp_file *const prev = self->tf_prev;
	tpp_assert(self->tf_kind == TPP_FILE_KIND_IO ||
	           self->tf_kind == TPP_FILE_KIND_TEXT);
	if (prev == NULL)
		return false; /* No predecessor */
	if (prev->tf_kind != TPP_FILE_KIND_DUMMY)
		return false; /* Not a dummy file */
	self->tf_prev  = prev->tf_prev;
	self->tf_tprev = prev->tf_tprev;
	tpp_file_fini(prev);
	tpp_file_free(prev);
	return true;
}
#endif /* TPP_HAVE_FILE_DUMMY */

#endif /* TPP_HAVE_INCLUDE_STACK */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FILE_C */
