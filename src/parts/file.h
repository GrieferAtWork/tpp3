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
#define TPP_FILE_IOFLAGS_NOKWD    UINT8_C(0x08) /* The file's "tff_name" field isn't actually a "tpp_keyword::tk_kwd", but rather a raw \0-terminated C string. */
#endif /* TPP_HAVE_FILE_NOKWD */
#endif /* TPP_HAVE_FILE_IOFLAGS */



#if TPP_HAVE_IFDEF_STACK
typedef enum tpp_ifdef_mode {
	TPP_IFDEF_MODE_IFDEF, /* Inside of an active #ifdef (or #if, #elif, ...) block.
	                       * Allowed follow-up directives are: "#elif", "#else", "#endif" */
	TPP_IFDEF_MODE_ELSE,  /* Inside of an active #else-block (following an already-skipped #if-block)
	                       * Allowed follow-up directives are: "#endif" */
} tpp_ifdef_mode;

typedef struct tpp_ifdef_stack_entry {
	tpp_ifdef_mode TPP_INTERNAL(tidse_mode);    /* Behavioral mode of this entry */
	tpp_lcinfo     TPP_INTERNAL(tidse_created); /* line/column info of #if/#ifdef/#ifndef directive that created this block */
	tpp_lcinfo     TPP_INTERNAL(tidse_updated); /* line/column info of last directive that updated this block.
	                                             * Or in other words: staring-position of current sub-block
	                                             * e.g.: after #elif or #else, points at that directive, while
	                                             *       "tidse_created" continues to point at initial #if/... */
} tpp_ifdef_stack_entry;

typedef struct tpp_ifdef_stack {
	tpp_size               TPP_INTERNAL(tids_alc); /* Allocated size of `tids_vec' */
	tpp_size               TPP_INTERNAL(tids_cnt); /* Used size of `tids_vec' */
	tpp_ifdef_stack_entry *TPP_INTERNAL(tids_vec); /* [0..tids_cnt][owned] Vector of active #ifdef-stack entries */
} tpp_ifdef_stack;

#define tpp_ifdef_stack_init(self) \
	(void)((self)->TPP_INTERNAL(tids_alc) = 0,   \
	       (self)->TPP_INTERNAL(tids_cnt) = 0,   \
	       (self)->TPP_INTERNAL(tids_vec) = NULL)
#define tpp_ifdef_stack_fini(self) \
	tpp_free((self)->TPP_INTERNAL(tids_vec))

/* Check if the given #ifdef-stack is non-empty */
#define tpp_ifdef_stack_isnonempty(self) \
	((self)->TPP_INTERNAL(tids_cnt) != 0)

/* Check if the most-recent #ifdef-stack entry indicates
 * that TPP is currently inside of a #else-block. The caller
 * must ensure that "tpp_ifdef_stack_isnonempty(self)" */
#define tpp_ifdef_stack_isafterelse(self) \
	((self)->TPP_INTERNAL(tids_vec)[(self)->TPP_INTERNAL(tids_cnt) - 1].TPP_INTERNAL(tidse_mode) == TPP_IFDEF_MODE_ELSE)

/* Allocate an additional #ifdef-stack entry, and return a pointer to it.
 * This function will increment `self->tids_cnt', but it is up to the
 * caller to initialize the returned #ifdef-stack entry
 *
 * @return: * :   The (uninitialized) #ifdef-stack entry
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_ifdef_stack_entry *TPPCALL
tpp_ifdef_stack_append(tpp_ifdef_stack *tpp_restrict self);

/* Do the inverse of "tpp_ifdef_stack_append()", removing the last entry, and
 * potentially freeing unused memory.
 * The caller is responsible to ensure that `tpp_ifdef_stack_isnonempty(self)' */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_ifdef_stack_remove(tpp_ifdef_stack *tpp_restrict self);
#endif /* TPP_HAVE_IFDEF_STACK */


struct tpp_keyword;
#if TPP_HAVE_CPP_MACROS
struct tpp_macro;
#endif /* TPP_HAVE_CPP_MACROS */

typedef struct tpp_file {
#if TPP_HAVE_INCLUDE_STACK
	tpp_char const     *TPP_INTERNAL(tf_tpos);  /* [?..?][valid_if(DID_CALL(tpp_lexer_yieldraw))]
	                                             * Start of last-loaded token (also valid in "tf_tprev"-files)
	                                             * WARNING: This field is NOT maintained/updated by `tpp_file_*' APIs
	                                             *          It is only here so it overlaps with the lexer's token's
	                                             *          "tt_start" field, such that said field is saved when
	                                             *          a new file is pushed onto the #include-stack, and can
	                                             *          then be used to calculate line/column information when
	                                             *          lexer prints its #include-stack. */
#endif /* TPP_HAVE_INCLUDE_STACK */
	/* Important: "tf_pos" and "tf_chunk" must come first, so they can shadow the tail of "tpp_token" */
	tpp_char const     *TPP_INTERNAL(tf_pos);   /* [0..1][<= tf_end] File pointer to next unread byte. */
	TPP_REF tpp_string *TPP_INTERNAL(tf_chunk); /* [0..1][const_if(tf_kind != TPP_FILE_KIND_IO)] Currently loaded text-chunk (mutable for text-files) */
	tpp_char const     *TPP_INTERNAL(tf_end);   /* [0..1][>= tf_chunk->ts_str && <= tf_chunk->ts_str+tf_chunk->ts_len][const_if(tf_kind != TPP_FILE_KIND_IO)] End of effective file content (mutable for text-files) */
#if TPP_HAVE_INCLUDE_STACK
	struct tpp_file    *TPP_INTERNAL(tf_prev);  /* [0..1] Parent file in #include stack */
	struct tpp_file    *TPP_INTERNAL(tf_tprev); /* [0..1] Real parent for the purposes of message tracebacks (not affected by `tpp_file_autopopfile_pushoff') */
#define _tpp_file_init_prev(self) , (self)->tf_prev = NULL, (self)->tf_tprev = NULL
#else /* TPP_HAVE_INCLUDE_STACK */
#define _tpp_file_init_prev(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_STACK */
#if TPP_HAVE_FILE_LC_CACHE
	tpp_char const     *TPP_INTERNAL(tf_lcpos); /* [0..1] Position that `tf_lcval' applies to. */
	tpp_lcinfo          TPP_INTERNAL(tf_lcval); /* [valid_if(tf_lcpos)] Cached line/column at `tf_lcpos' */
#define _tpp_file_init_lcpos(self) , (self)->tf_lcpos = NULL
#else /* TPP_HAVE_FILE_LC_CACHE */
#define _tpp_file_init_lcpos(self) /* nothing */
#endif /* !TPP_HAVE_FILE_LC_CACHE */
#if TPP_HAVE_IFDEF_STACK
	tpp_ifdef_stack     TPP_INTERNAL(tf_ifdef); /* #ifdef-stack */
#define _tpp_file_init_ifdef(self) , tpp_ifdef_stack_init(&(self)->TPP_INTERNAL(tf_ifdef))
#else /* TPP_HAVE_IFDEF_STACK */
#define _tpp_file_init_ifdef(self) /* nothing */
#endif /* !TPP_HAVE_IFDEF_STACK */
	tpp_file_kind       TPP_INTERNAL(tf_kind);  /* [const] File kind */
#if TPP_HAVE_UNICODE
	tpp_file_encoding   TPP_INTERNAL(tf_enc);   /* File encoding */
#define _tpp_file_init_enc(self)       , (self)->TPP_INTERNAL(tf_enc) = TPP_FILE_ENCODING_UTF8
#define _tpp_file_init_enc_ex(self, v) , (self)->TPP_INTERNAL(tf_enc) = v
#else /* TPP_HAVE_UNICODE */
#define _tpp_file_init_enc(self)       /* nothing */
#define _tpp_file_init_enc_ex(self, v) /* nothing */
#endif /* !TPP_HAVE_UNICODE */
	union {
		struct {
			char const      *TPP_INTERNAL(tff_name);     /* [0..1][const] Filename by which this file was included (if available) */
			tpp_lcinfo       TPP_INTERNAL(tff_start_lc); /* [valid_if(tf_chunk != NULL)] Line/Column numbers (0-based) of `tf_chunk->ts_str' */
#if TPP_HAVE_FILE_USER_FILENAME
			TPP_REF tpp_string *TPP_INTERNAL(tff_user_filename); /* [0..1] User-defined override for name of this file */
#define _tpp_file_init_io_user_filename(self) , (self)->tf_data.td_io.TPP_INTERNAL(tff_user_filename) = NULL
#else /* TPP_HAVE_FILE_USER_FILENAME */
#define _tpp_file_init_io_user_filename(self) /* nothing */
#endif /* !TPP_HAVE_FILE_USER_FILENAME */
			tpp_io_handle    TPP_INTERNAL(tff_file);     /* [owned_if(!TPP_FILE_IOFLAGS_NOCLOSE)] Underlying I/O file (set to tpp_io_handle_INVALID after EOF) */
#if TPP_HAVE_FILE_IOFLAGS
			tpp_file_ioflags TPP_INTERNAL(tff_flags);    /* File flags (set of `TPP_FILE_IOFLAGS_*') */
#endif /* TPP_HAVE_FILE_IOFLAGS */
#if TPP_HAVE_UNICODE
			uint_least8_t TPP_INTERNAL(tff_tailc);    /* [valid_if(tf_enc) == TPP_FILE_ENCODING_UTF[16|32]_[LE|BE]] Read, unaligned tail data */
			unsigned char TPP_INTERNAL(tff_tailv)[3]; /* [valid_if(tf_enc) == TPP_FILE_ENCODING_UTF[16|32]_[LE|BE]] Read, unaligned tail data */
#endif /* TPP_HAVE_UNICODE */
		} TPP_INTERNAL(td_io); /* [tf_kind == TPP_FILE_KIND_IO] */

		struct {
			char const *TPP_INTERNAL(tft_name);     /* [0..1][const] Filename for messages (if available) */
			tpp_lcinfo  TPP_INTERNAL(tft_start_lc); /* [valid_if(tf_chunk != NULL)] Line/Column numbers (0-based) of `tf_chunk->ts_str' */
#if TPP_HAVE_FILE_USER_FILENAME
			TPP_REF tpp_string *TPP_INTERNAL(tft_user_filename); /* [0..1] User-defined override for name of this file */
#define _tpp_file_init_text_user_filename(self) , (self)->tf_data.td_text.TPP_INTERNAL(tft_user_filename) = NULL
#else /* TPP_HAVE_FILE_USER_FILENAME */
#define _tpp_file_init_text_user_filename(self) /* nothing */
#endif /* !TPP_HAVE_FILE_USER_FILENAME */
		} TPP_INTERNAL(td_text); /* [tf_kind == TPP_FILE_KIND_TEXT] */

#if TPP_HAVE_CPP_MACROS
		struct {
			/* [1..1][const] The macro definition that produced this file
			 * as its expansion (also holds a reference to "tm_expansions") */
			TPP_REF struct tpp_macro *TPP_INTERNAL(tfm_macro);
		} TPP_INTERNAL(td_macro); /* [tf_kind == TPP_FILE_KIND_MACRO] */
#endif /* TPP_HAVE_CPP_MACROS */
	} TPP_INTERNAL(tf_data);
} tpp_file;

#define tpp_file_alloc() ((tpp_file *)tpp_malloc(sizeof(tpp_file)))
#define tpp_file_free(p) tpp_free(p)

#if TPP_HAVE_FILE_IOFLAGS
#define _tpp_file_init_ioflags(self, flags) , (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(tff_flags) = (flags)
#else /* TPP_HAVE_FILE_SYSHDR */
#define _tpp_file_init_ioflags(self, flags) /* nothing */
#endif /* !TPP_HAVE_FILE_SYSHDR */

#if TPP_HAVE_UNICODE
#define tpp_file_isutf8(self)  TPP_FILE_ENCODING_ISUTF8((self)->TPP_INTERNAL(tf_enc))
#define tpp_file_isascii(self) TPP_FILE_ENCODING_ISASCII((self)->TPP_INTERNAL(tf_enc))
#else /* TPP_HAVE_UNICODE */
#define tpp_file_isutf8(self)  0
#define tpp_file_isascii(self) 1
#endif /* !TPP_HAVE_UNICODE */

/* Public API for accessing internal components of "tpp_file" */
#define tpp_file_getkind(self)  ((self)->TPP_INTERNAL(tf_kind))
#define tpp_file_getpos(self)   ((self)->TPP_INTERNAL(tf_pos))
#define tpp_file_getend(self)   ((self)->TPP_INTERNAL(tf_end))
#define tpp_file_getchunk(self) ((self)->TPP_INTERNAL(tf_chunk))
#if TPP_HAVE_IFDEF_STACK
#define tpp_file_getifdef(self) (&(self)->TPP_INTERNAL(tf_ifdef))
#endif /* !TPP_HAVE_IFDEF_STACK */

/* Initialize common fields of "self" */
#define _tpp_file_init_common(self) \
	_tpp_file_init_lcpos(self)      \
	_tpp_file_init_ifdef(self)

/* Temporarily disable automatic pop-to-prev-file on EOF */
#if TPP_HAVE_INCLUDE_STACK
#define tpp_file_autopopfile_pushoff(self)                            \
	do {                                                              \
		tpp_file *const _tfapfp_prev = (self)->TPP_INTERNAL(tf_prev); \
		(self)->TPP_INTERNAL(tf_prev) = NULL
#define tpp_file_autopopfile_break(self)                                                \
		(void)(tpp_assert((self)->TPP_INTERNAL(tf_prev) == NULL && "New files pushed"), \
		       (self)->TPP_INTERNAL(tf_prev) = _tfapfp_prev)
#define tpp_file_autopopfile_pop(self)    \
		tpp_file_autopopfile_break(self); \
	} while (0)
#else /* TPP_HAVE_INCLUDE_STACK */
#define tpp_file_autopopfile_pushoff(self) do {
#define tpp_file_autopopfile_break(self)   (void)0
#define tpp_file_autopopfile_pop(self)     } while (0)
#endif /* !TPP_HAVE_INCLUDE_STACK */


#if TPP_HAVE_CPP_MACROS
#define _tpp_file_io2text(self)                                   \
	((self)->TPP_INTERNAL(tf_kind) == TPP_FILE_KIND_IO            \
	 ? (void)((self)->TPP_INTERNAL(tf_kind) = TPP_FILE_KIND_TEXT) \
	 : (void)0)
#else /* TPP_HAVE_CPP_MACROS */
#define _tpp_file_io2text(self) \
	(void)((self)->TPP_INTERNAL(tf_kind) = TPP_FILE_KIND_TEXT)
#endif /* !TPP_HAVE_CPP_MACROS */


/* - Disable automatic popping of "self" from the #include-stack
 * - Disable I/O expansion by reading additional data from the file
 * - Make it so the file's EOF position can be overwritten freely
 *   (such that trying to yield additional tokens at/beyond that
 *   position will cause "tpp_lexer_yieldraw()" to return TPP_TOK_EOF)
 * - The previous state can be restored by `tpp_file_popeof()'
 *
 * WARNING:
 * - The caller must ensure that all files pushed are
 *   also popped before breaking out of a PUSHEOF block
 * - These functions don't save/restore the #ifdef-stack
 *   For that, also make use of "tpp_file_pushifdef()" */
#define tpp_file_pusheof(self)                              \
	do {                                                    \
		tpp_file_kind const _tfpeof_kind = (self)->TPP_INTERNAL(tf_kind); \
		tpp_char const *const _tfpeof_end = (self)->TPP_INTERNAL(tf_end); \
		_tpp_file_io2text(self)
#define tpp_file_seteof(self, end) \
		(void)((self)->TPP_INTERNAL(tf_end) = (end))
#define tpp_file_breakeof(self)                             \
		(void)((self)->TPP_INTERNAL(tf_end)  = _tfpeof_end, \
		       (self)->TPP_INTERNAL(tf_kind) = _tfpeof_kind)
#define tpp_file_popeof(self)    \
		tpp_file_breakeof(self); \
	} while (0)


/* Save/restore the position where the next token will be read from */
#define tpp_file_pushpos(self) \
	do {                       \
		tpp_char const *const _tfppos_pos = (self)->TPP_INTERNAL(tf_pos)
#define tpp_file_setpos(self, pos) \
		(void)((self)->TPP_INTERNAL(tf_pos) = (pos))
#define tpp_file_breakpos(self) \
		(void)((self)->TPP_INTERNAL(tf_pos) = _tfppos_pos)
#define tpp_file_poppos(self)    \
		tpp_file_breakpos(self); \
	} while (0)

/* Push (+clear) and later (clear+)restore the #ifdef-stack of a given file */
#if TPP_HAVE_IFDEF_STACK
#define tpp_file_pushifdef(self)                                             \
	do {                                                                     \
		tpp_ifdef_stack const _tfpid_stack = (self)->TPP_INTERNAL(tf_ifdef); \
		tpp_ifdef_stack_init(&(self)->TPP_INTERNAL(tf_ifdef))
#define tpp_file_breakifdef(self)                                     \
		(void)(tpp_ifdef_stack_fini(&(self)->TPP_INTERNAL(tf_ifdef)), \
		       (self)->TPP_INTERNAL(tf_ifdef) = _tfpid_stack)
#define tpp_file_popifdef(self)    \
		tpp_file_breakifdef(self); \
	} while (0)
#else /* TPP_HAVE_IFDEF_STACK */
#define tpp_file_pushifdef(self)  do {
#define tpp_file_breakifdef(self) (void)0
#define tpp_file_popifdef(self)   } while (0)
#endif /* !TPP_HAVE_IFDEF_STACK */



/* Initialize "self " as a "TPP_FILE_KIND_IO" file
 * @param: char const      *filename: [0..1] Filename (if known)
 * @param: tpp_io_handle    fp:       File descriptor (inherited)
 * @param: tpp_file_ioflags flags:    I/O file flags (set of `TPP_FILE_IOFLAGS_*') */
#define tpp_file_init_io(self, filename, /*inherit*/ fp) \
	tpp_file_init_io_ex(self, filename, fp, TPP_FILE_IOFLAGS_NORMAL)
#define tpp_file_init_io_ex(self, filename, /*inherit*/ fp, flags)                                             \
	(void)((self)->TPP_INTERNAL(tf_pos)   = NULL,                                                              \
	       (self)->TPP_INTERNAL(tf_chunk) = NULL,                                                              \
	       (self)->TPP_INTERNAL(tf_end)   = NULL                                                               \
	       _tpp_file_init_prev(self),                                                                          \
	       (self)->TPP_INTERNAL(tf_kind) = TPP_FILE_KIND_IO                                                    \
	       _tpp_file_init_enc(self)                                                                            \
	       _tpp_file_init_common(self),                                                                        \
	       (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(tff_name) = (filename),              \
	       (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(tff_file) = (fp),                    \
	       tpp_lcinfo_init((self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(tff_start_lc), 0, 0) \
	       _tpp_file_init_io_user_filename(self)                                                               \
	       _tpp_file_init_ioflags(self, flags))



/* Initialize "self " as a "TPP_FILE_KIND_TEXT" file
 * @param: char const         *filename:  [0..1] Filename (if known)
 * @param: TPP_REF tpp_string *chunk:     File data chunk
 * @param: void const         *text:      File data base pointer
 * @param: tpp_size            text_size: File data size
 * @param: tpp_lcinfo          start_lc:  [valid_if(chunk)] 0-based line/column info for start of "text"
 * @param: tpp_file_encoding   encoding:  File data encoding */
#define tpp_file_init_text_ascii(self, filename, chunk, text, text_size, start_lc) \
	tpp_file_init_text_ex(self, filename, chunk, text, start_lc, TPP_FILE_ENCODING_ASCII)
#define tpp_file_init_text_ex(self, filename, chunk, text, text_size, start_lc, encoding)               \
	(void)((self)->TPP_INTERNAL(tf_pos)   = (tpp_char const *)(text),                                   \
	       (self)->TPP_INTERNAL(tf_chunk) = (chunk),                                                    \
	       (self)->TPP_INTERNAL(tf_end)   = (tpp_char const *)(text) + (text_size)                      \
	       _tpp_file_init_prev(self),                                                                   \
	       (self)->TPP_INTERNAL(tf_kind) = TPP_FILE_KIND_TEXT                                           \
	       _tpp_file_init_enc_ex(self, encoding)                                                        \
	       _tpp_file_init_common(self)                                                                  \
	       _tpp_file_init_text_user_filename(self),                                                     \
	       (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_text).TPP_INTERNAL(tft_name)     = (filename), \
	       (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_text).TPP_INTERNAL(tft_start_lc) = (start_lc))



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
#define tpp_file_ptr2rel(self, ptr) (tpp_size)((ptr) - (self)->TPP_INTERNAL(tf_pos))
#define tpp_file_rel2ptr(self, rel) ((self)->TPP_INTERNAL(tf_pos) + (rel))



/* Return line/column information (1-based) for "pos" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_lcinfo TPPCALL
tpp_file_lcinfo(tpp_file *tpp_restrict self,
                tpp_char const *pos);

/* Returns the filename of "self", or "NULL" if unknown. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_filename(tpp_file const *tpp_restrict self);

/* Same as `tpp_file_filename()', but may be overwritten by "#line" directives */
#if TPP_HAVE_FILE_USER_FILENAME
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_userfilename(tpp_file const *tpp_restrict self);

/* Sets the user-filename override of "self" to "filename"
 *
 * NOTE: The caller must ensure that:
 *       >> self->tf_kind == TPP_FILE_KIND_IO ||
 *       >> self->tf_kind == TPP_FILE_KIND_TEXT;
 *
 * You may also pass "NULL" for `filename' to disable the override */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_file_setuserfilename(tpp_file *tpp_restrict self,
                         tpp_string *tpp_restrict filename);
#else /* TPP_HAVE_FILE_USER_FILENAME */
#define tpp_file_userfilename(self) tpp_file_filename(self)
#endif /* !TPP_HAVE_FILE_USER_FILENAME */

/* Set the (0-based) line that applies to "pos" (as returned by "tpp_file_lcinfo") in "self"
 *
 * NOTE: The caller must ensure that:
 *       >> self->tf_kind == TPP_FILE_KIND_IO ||
 *       >> self->tf_kind == TPP_FILE_KIND_TEXT; */
#if TPP_HAVE_FILE_SETLINE
TPP_DECL TPP_NONNULL((1, 2)) void TPPCALL
tpp_file_setline(tpp_file *tpp_restrict self,
                 tpp_char const *pos, tpp_line line);
#endif /* TPP_HAVE_FILE_SETLINE */

/* Returns the filename "keyword" (which may not always be
 * available, even when "tpp_file_filename()" returns non-NULL) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) struct tpp_keyword *TPPCALL
tpp_file_filename_kwd(tpp_file const *tpp_restrict self);

#if TPP_HAVE_INCLUDE_STACK
/* Returns the first tf_kind=TPP_FILE_KIND_IO file in the #include-stack (using "tf_tprev")
 * If no such file exists, simply re-return "self". This function never returns "NULL" */
TPP_DECL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getiofile(tpp_file const *tpp_restrict self);

#if TPP_HAVE_CPP_MACROS
/* Returns the first tf_kind!=TPP_FILE_KIND_MACRO file in the #include-stack (using "tf_tprev")
 * If no such file exists, returns "NULL" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_gettextfile(tpp_file const *tpp_restrict self);

/* Same as `tpp_file_gettextfile()', but re-return "self" instead of returning "NULL"
 * The term "lc" here refers to the fact that this is the file that's used as basis
 * for the builtin __FILE__, __LINE__ and __COLUMN__ macros. */
TPP_DECL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getlcfile(tpp_file const *tpp_restrict self);
#else /* TPP_HAVE_CPP_MACROS */
#define tpp_file_gettextfile(self) ((tpp_file *)(self))
#define tpp_file_getlcfile(self)   ((tpp_file *)(self))
#endif /* !TPP_HAVE_CPP_MACROS */
#else /* TPP_HAVE_INCLUDE_STACK */
#define tpp_file_getiofile(self) ((tpp_file *)(self))
#define tpp_file_getlcfile(self) ((tpp_file *)(self))
#if TPP_HAVE_CPP_MACROS
#define tpp_file_gettextfile(self) ((self)->TPP_INTERNAL(tf_kind) == TPP_FILE_KIND_MACRO ? NULL : ((tpp_file *)(self)))
#else /* TPP_HAVE_CPP_MACROS */
#define tpp_file_gettextfile(self) ((tpp_file *)(self))
#endif /* !TPP_HAVE_CPP_MACROS */
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
