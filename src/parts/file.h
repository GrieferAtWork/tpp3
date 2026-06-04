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
#ifndef GUARD_TPP_FILE_H
#define GUARD_TPP_FILE_H 1

#include "api.h"
#include "config.h"
#include "string.h"
#include "error.h"
#include "file-io.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

typedef enum tpp_file_kind {
	TPP_FILE_KIND_IO,    /* File found on hard-disk */
	TPP_FILE_KIND_TEXT,  /* Explicitly injected text (same as `TPP_FILE_KIND_IO', but single-chunk'd) */
#if TPP_HAVE_CPP_MACROS
	TPP_FILE_KIND_MACRO, /* Expanded macro */
#endif /* TPP_HAVE_CPP_MACROS */
} tpp_file_kind;

#if TPP_HAVE_UNICODE
typedef enum tpp_file_encoding {
	TPP_FILE_ENCODING_UTF8,       /* Assume utf-8 until an invalid byte sequence is encountered (when that happens, set to `TPP_FILE_ENCODING_ASCII') */
	TPP_FILE_ENCODING_ASCII,      /* An invalid utf-8 byte sequence is encountered */
	TPP_FILE_ENCODING_FORCE_UTF8, /* Always use utf-8 (used when a BOM was encountered at the start of the file) */

	/* The following are only used internally and (despite their names) must be treated as "TPP_FILE_ENCODING_FORCE_UTF8" */
	TPP_FILE_ENCODING_UTF16_LE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-16 (le) and auto-converted to utf-8 during reading */
	TPP_FILE_ENCODING_UTF16_BE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-16 (be) and auto-converted to utf-8 during reading */
	TPP_FILE_ENCODING_UTF32_LE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-32 (le) and auto-converted to utf-8 during reading */
	TPP_FILE_ENCODING_UTF32_BE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-32 (be) and auto-converted to utf-8 during reading */
} tpp_file_encoding;
#define TPP_FILE_ENCODING_ISUTF8(enc)  ((enc) != TPP_FILE_ENCODING_ASCII)
#define TPP_FILE_ENCODING_ISASCII(enc) ((enc) == TPP_FILE_ENCODING_ASCII)
#endif /* TPP_HAVE_UNICODE */


#undef TPP_HAVE_FILE_IOFLAGS
#if (TPP_HAVE_FILE_NONBLOCK || \
     TPP_HAVE_FILE_NOCLOSE ||  \
     TPP_HAVE_FILE_SYSHDR ||   \
     TPP_HAVE_FILE_NOKWD)
#define TPP_HAVE_FILE_IOFLAGS 1
#else /* ... */
#define TPP_HAVE_FILE_IOFLAGS 0
#endif /* !... */

#if TPP_HAVE_FILE_IOFLAGS
#define tpp_file_ioflags uint_least8_t /* Set of `TPP_FILE_IOFLAGS_*' */
#define TPP_FILE_IOFLAGS_NORMAL   UINT8_C(0x00) /* Normal flags */
#if TPP_HAVE_FILE_NONBLOCK
#define TPP_FILE_IOFLAGS_NONBLOCK UINT8_C(0x01) /* Do non-blocking I/O */
#endif /* TPP_HAVE_FILE_NONBLOCK */
#if TPP_HAVE_FILE_NOCLOSE
#define TPP_FILE_IOFLAGS_NOCLOSE  UINT8_C(0x02) /* Don't `tpp_io_close(tff_file)' on destruction */
#endif /* TPP_HAVE_FILE_NOCLOSE */
#if TPP_HAVE_FILE_SYSHDR
#define TPP_FILE_IOFLAGS_SYSHDR   UINT8_C(0x04) /* Suppress all warnings produced in the context of this file */
#endif /* TPP_HAVE_FILE_SYSHDR */
#if TPP_HAVE_FILE_NOKWD
#define TPP_FILE_IOFLAGS_NOKWD    UINT8_C(0x08) /* The file's "tff_name" field isn't actually a "tpp_keyword", but rather a raw \0-terminated C string. */
#endif /* TPP_HAVE_FILE_NOKWD */
#endif /* TPP_HAVE_FILE_IOFLAGS */


struct tpp_keyword;
#if TPP_HAVE_CPP_MACROS
struct tpp_macro;
#endif /* TPP_HAVE_CPP_MACROS */

typedef struct tpp_file {
	/* Important: "tf_pos" and "tf_chunk" must come first, so they can shadow the tail of "tpp_token" */
	tpp_char const     *tf_pos;   /* [0..1][<= tf_end] File pointer to next unread byte. */
	TPP_REF tpp_string *tf_chunk; /* [0..1][const_if(tf_kind != TPP_FILE_KIND_IO)] Currently loaded text-chunk (mutable for text-files) */
	tpp_char const     *tf_end;   /* [0..1][>= tf_chunk->ts_str && <= tf_chunk->ts_str+tf_chunk->ts_len][const_if(tf_kind != TPP_FILE_KIND_IO)] End of effective file content (mutable for text-files) */
#if TPP_HAVE_INCLUDE_STACK
	struct tpp_file    *tf_prev;  /* [0..1] Parent file in #include stack */
#endif /* TPP_HAVE_INCLUDE_STACK */
#if TPP_HAVE_FILE_LC_CACHE
	tpp_char const     *tf_lcpos; /* [0..1] Position that `tf_lcval' applies to. */
	tpp_lcinfo          tf_lcval; /* [valid_if(tf_lcpos)] Cached line/column at `tf_lcpos' */
#endif /* TPP_HAVE_FILE_LC_CACHE */
	tpp_file_kind       tf_kind;  /* [const] File kind */
#if TPP_HAVE_UNICODE
	tpp_file_encoding   tf_enc;   /* File encoding */
#endif /* TPP_HAVE_UNICODE */
	union {
		struct {
			struct tpp_keyword *tff_name;     /* [0..1][const] Filename by which this file was included (if available) */
			tpp_io_handle       tff_file;     /* [owned_if(!TPP_FILE_IOFLAGS_NOCLOSE)] Underlying I/O file (set to tpp_io_handle_INVALID after EOF) */
			tpp_lcinfo          tff_start_lc; /* [valid_if(tf_chunk != NULL)] Line/Column numbers (0-based) of `tf_chunk->ts_str' */
#if TPP_HAVE_FILE_IOFLAGS
			tpp_file_ioflags    tff_flags;    /* File flags (set of `TPP_FILE_IOFLAGS_*') */
#endif /* TPP_HAVE_FILE_IOFLAGS */
#if TPP_HAVE_UNICODE
			uint_least8_t tff_tailc;    /* [valid_if(tf_enc) == TPP_FILE_ENCODING_UTF[16|32]_[LE|BE]] Read, unaligned tail data */
			unsigned char tff_tailv[3]; /* [valid_if(tf_enc) == TPP_FILE_ENCODING_UTF[16|32]_[LE|BE]] Read, unaligned tail data */
#endif /* TPP_HAVE_UNICODE */
		} td_io; /* [tf_kind == TPP_FILE_KIND_IO] */

		struct {
			char const *tft_name; /* [0..1][const] Filename for messages (if available) */
		} td_text; /* [tf_kind == TPP_FILE_KIND_TEXT] */

#if TPP_HAVE_CPP_MACROS
		struct {
			TPP_REF struct tpp_macro *tfm_macro; /* [1..1][const] The macro definition that produced this file as its expansion */
		} td_macro; /* [tf_kind == TPP_FILE_KIND_MACRO] */
#endif /* TPP_HAVE_CPP_MACROS */
	} tf_data;
} tpp_file;


#if TPP_HAVE_INCLUDE_STACK
#define _tpp_file_init_prev(self) , (self)->tf_prev = NULL
#else /* TPP_HAVE_INCLUDE_STACK */
#define _tpp_file_init_prev(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_STACK */

#if TPP_HAVE_FILE_LC_CACHE
#define _tpp_file_init_lc(self) , (self)->tf_lcpos = NULL
#else /* TPP_HAVE_FILE_LC_CACHE */
#define _tpp_file_init_lc(self) /* nothing */
#endif /* !TPP_HAVE_FILE_LC_CACHE */

#if TPP_HAVE_UNICODE
#define _tpp_file_init_enc(self)       , (self)->tf_enc = TPP_FILE_ENCODING_UTF8
#define _tpp_file_init_enc_ex(self, v) , (self)->tf_enc = v
#else /* TPP_HAVE_UNICODE */
#define _tpp_file_init_enc(self)       /* nothing */
#define _tpp_file_init_enc_ex(self, v) /* nothing */
#endif /* !TPP_HAVE_UNICODE */

#if TPP_HAVE_FILE_IOFLAGS
#define _tpp_file_init_ioflags(self, flags) , (self)->tf_data.td_io.tff_flags = (flags)
#else /* TPP_HAVE_FILE_SYSHDR */
#define _tpp_file_init_ioflags(self, flags) /* nothing */
#endif /* !TPP_HAVE_FILE_SYSHDR */

#if TPP_HAVE_UNICODE
#define tpp_file_isutf8(self)  TPP_FILE_ENCODING_ISUTF8((self)->tf_enc)
#define tpp_file_isascii(self) TPP_FILE_ENCODING_ISASCII((self)->tf_enc)
#else /* TPP_HAVE_UNICODE */
#define tpp_file_isutf8(self)  0
#define tpp_file_isascii(self) 1
#endif /* !TPP_HAVE_UNICODE */



/* Initialize "self " as a "TPP_FILE_KIND_IO" file
 * @param: tpp_keyword     *filename: [0..1] Filename (if known)
 * @param: tpp_io_handle    fp:       File descriptor (inherited)
 * @param: tpp_file_ioflags flags:    I/O file flags (set of `TPP_FILE_IOFLAGS_*') */
#define tpp_file_init_io(self, filename, /*inherit*/ fp) \
	tpp_file_init_io_ex(self, filename, fp, TPP_FILE_IOFLAGS_NORMAL)
#define tpp_file_init_io_ex(self, filename, /*inherit*/ fp, flags)   \
	(void)((self)->tf_pos   = NULL,                                  \
	       (self)->tf_chunk = NULL,                                  \
	       (self)->tf_end   = NULL                                   \
	       _tpp_file_init_prev(self)                                 \
	       _tpp_file_init_lc(self),                                  \
	       (self)->tf_kind = TPP_FILE_KIND_IO                        \
	       _tpp_file_init_enc(self),                                 \
	       (self)->tf_data.td_io.tff_name = (filename),              \
	       (self)->tf_data.td_io.tff_file = (fp),                    \
	       tpp_lcinfo_init((self)->tf_data.td_io.tff_start_lc, 0, 0) \
	       _tpp_file_init_ioflags(self, flags))



/* Initialize "self " as a "TPP_FILE_KIND_TEXT" file
 * @param: char const       *filename:  [0..1] Filename (if known)
 * @param: void const       *text:      File data base pointer
 * @param: tpp_size          text_size: File data size
 * @param: tpp_file_encoding encoding:  File data encoding */
#define tpp_file_init_text_ascii(self, filename, text, text_size) \
	tpp_file_init_text_ex(self, filename, text, TPP_FILE_ENCODING_ASCII)
#define tpp_file_init_text_ex(self, filename, text, text_size, encoding) \
	(void)((self)->tf_pos   = (tpp_char const *)(text),                  \
	       (self)->tf_chunk = NULL,                                      \
	       (self)->tf_end   = (tpp_char const *)(text) + (text_size)     \
	       _tpp_file_init_prev(self)                                     \
	       _tpp_file_init_lc(self),                                      \
	       (self)->tf_kind = TPP_FILE_KIND_TEXT                          \
	       _tpp_file_init_enc_ex(self, encoding),                        \
	       (self)->tf_data.td_text.tft_name = (filename))



/* Finalize the given file. */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_file_fini(tpp_file *tpp_restrict self);


/* Try to expand the currently loaded `self->tf_chunk':
 * - If the file's kind isn't `TPP_FILE_KIND_IO', return `TPP_EOK'
 * - Allocate a new `struct tpp_string' suitable for holding both
 *   [tf_pos,tf_end), as well as at least 1 additional byte.
 *   HINT: When `!tpp_string_isshared(self->tf_chunk)', the old
 *         chunk string may simply be re-used (since it'd be free'd
 *         during the re-assignment below anyways). If this isn't
 *         intended (iow: you want to keep the contents of the previous
 *         chunk loaded into memory), you can simply `tpp_string_incref'
 *         it before calling this function.
 * - Copy [tf_pos,tf_end) into this new string
 * - Read from the underlying file into the tail of the new string
 * #if TPP_HAVE_UNICODE
 *   - If the underlying file's encoding is TPP_FILE_ENCODING_UTF(16|32)_(LE|BE),
 *     the read data is converted to utf-8 at this point.
 * #endif // TPP_HAVE_UNICODE
 *   - If the underlying file could not be read, return `TPP_EIO'
 *   - If nothing could be read, free the new string and return `TPP_EOK'
 *   - Else:
 *     - adjust `tf_pos' to point into the new string, and
 *       set `tf_end' to point at the end of the new string.
 *     - replace `tf_chunk' with the new string
 *     - return `TPP_EOK'
 * @return: TPP_EOK: Either the current chunk was expanded (the delta
 *                   between `tf_pos' and `tf_end' has increased), or
 *                   no further data can be read from `self'.
 * @return: TPP_EIO: I/O error
 * #if TPP_HAVE_FILE_NONBLOCK
 * @return: TPP_EWOULDBLOCK: Operation would block.
 * #endif // TPP_HAVE_FILE_NONBLOCK */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_file_expandchunk(tpp_file *tpp_restrict self);

/* Encode/decode pointer<=>text-offset such that effective positions
 * are retained across calls to `tpp_file_expandchunk()'.
 *
 * NOTE: This needs to use "tf_pos" as relative base, since the start
 *       of the currently loaded chunk can change if another chunk is
 *       allocated that doesn't include the already-read buffer area
 *       located in [tf_chunk->ts_str,tf_pos) */
#define tpp_file_ptr2rel(self, ptr) (tpp_size)((ptr) - (self)->tf_pos)
#define tpp_file_rel2ptr(self, rel) ((self)->tf_pos + (rel))



/* Return line/column information (1-based) for "pos" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_lcinfo TPPCALL
tpp_file_lcinfo(tpp_file *tpp_restrict self,
                tpp_char const *pos);

/* Returns the filename of "self", or "NULL" if unknown. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_filename(tpp_file const *tpp_restrict self);

/* Returns the filename "keyword" (which may not always be
 * available, even when "tpp_file_filename()" returns non-NULL) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) struct tpp_keyword *TPPCALL
tpp_file_filename_kwd(tpp_file const *tpp_restrict self);

/* Returns the first tf_kind=TPP_FILE_KIND_IO file in the #include-stack
 * If no such file exists, simply re-return "self". This function never
 * returns "NULL" */
#if TPP_HAVE_INCLUDE_STACK
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getiofile(tpp_file const *tpp_restrict self);
#else /* TPP_HAVE_INCLUDE_STACK */
#define tpp_file_getiofile(self) (self)
#endif /* !TPP_HAVE_INCLUDE_STACK */

/* Size of \t as reported by `tpp_file_lcinfo()' */
#if TPP_TABSIZE >= 0
#define tpp_gettabsize() TPP_TABSIZE
#else /* TPP_TABSIZE >= 0 */
TPP_DECL tpp_column tpp_tabsize;
#define tpp_gettabsize()  tpp_tabsize
#define tpp_settabsize(v) (void)(tpp_tabsize = (v))
#endif /* TPP_TABSIZE < 0 */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FILE_H */
