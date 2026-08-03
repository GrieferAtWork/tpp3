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
	TPP_FILE_KIND_IO,      /* File found on hard-disk */
	TPP_FILE_KIND_TEXT,    /* Explicitly injected text (same as `TPP_FILE_KIND_IO`, but single-chunk'd and non-expandable) */
#if TPP_HAVE_FILE_SUBTEXT
	TPP_FILE_KIND_SUBTEXT, /* Same as `TPP_FILE_KIND_TEXT`, but used to describe temporary sub-chunk files */
#endif /* TPP_HAVE_FILE_SUBTEXT */
#if TPP_HAVE_CPP_MACROS
	TPP_FILE_KIND_MACRO,   /* Expanded macro */
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_FILE_DUMMY
	TPP_FILE_KIND_DUMMY,   /* Dummy file */
#endif /* TPP_HAVE_FILE_DUMMY */
} tpp_file_kind;

#if TPP_HAVE_UNICODE
typedef enum tpp_file_encoding {
	TPP_FILE_ENCODING_UTF8,       /* Assume utf-8 until an invalid byte sequence is encountered (when that happens, set to `TPP_FILE_ENCODING_ASCII`) */
	TPP_FILE_ENCODING_ASCII,      /* An invalid utf-8 byte sequence is encountered */
	TPP_FILE_ENCODING_FORCE_UTF8, /* Always use utf-8 (used when a BOM was encountered at the start of the file) */

	/* The following are only used internally and (despite their names) must be treated as `TPP_FILE_ENCODING_FORCE_UTF8` */
	TPP_FILE_ENCODING_UTF16_LE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-16 (le) and auto-converted to utf-8 during reading */
	TPP_FILE_ENCODING_UTF16_BE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-16 (be) and auto-converted to utf-8 during reading */
	TPP_FILE_ENCODING_UTF32_LE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-32 (le) and auto-converted to utf-8 during reading */
	TPP_FILE_ENCODING_UTF32_BE,   /* TPP_FILE_KIND_IO-only: Underlying file is in utf-32 (be) and auto-converted to utf-8 during reading */
#if TPP_HAVE_FILE_ENCODING_EMBED
	TPP_FILE_ENCODING_EMBED,      /* TPP_FILE_KIND_IO-only: Convert file bytes to sequence of ,-separated integers */
#endif /* TPP_HAVE_FILE_ENCODING_EMBED */
} tpp_file_encoding;
#define TPP_FILE_ENCODING_ISUTF8(enc)  ((enc) != TPP_FILE_ENCODING_ASCII)
#define TPP_FILE_ENCODING_ISASCII(enc) ((enc) == TPP_FILE_ENCODING_ASCII)
#endif /* TPP_HAVE_UNICODE */


/* Given a `lc` value for the start of `text`, calculate (and return)
 * a new value that describes the effect value at `text+size`. */
#if TPP_HAVE_UNICODE
TPP_DECL TPP_WUNUSED tpp_lcinfo TPPCALL
tpp_lcinfo_account_ex(tpp_lcinfo lc, tpp_char const *text,
                      tpp_size size, tpp_file_encoding enc);
#define tpp_lcinfo_account(lc, text, size) \
	tpp_lcinfo_account_ex(lc, text, size, TPP_FILE_ENCODING_UTF8)
#else /* TPP_HAVE_UNICODE */
TPP_DECL TPP_WUNUSED tpp_lcinfo TPPCALL
tpp_lcinfo_account(tpp_lcinfo lc, tpp_char const *text, tpp_size size);
#define tpp_lcinfo_account_ex(lc, text, size, enc) \
	tpp_lcinfo_account(lc, text, size)
#endif /* !TPP_HAVE_UNICODE */



#undef TPP_HAVE_FILE_FLAGS
#if (TPP_HAVE_FILE_NONBLOCK ||                               \
     TPP_HAVE_FILE_NOCLOSE ||                                \
     TPP_HAVE_FILE_NOKWD ||                                  \
     (!TPP_HAVE_USER_KEYWORDS && TPP_HAVE_LEXER_OPENFILE) || \
     TPP_HAVE_FILE_SYSHDR ||                                 \
     TPP_HAVE_FILE_EXTERN_C ||                               \
     TPP_HAVE_IFNDEF_INCLUDE_GUARDS ||                       \
     TPP_HAVE_CPP_DIRECTIVES)
#define TPP_HAVE_FILE_FLAGS 1
#else /* ... */
#define TPP_HAVE_FILE_FLAGS 0
#endif /* !... */

#if TPP_HAVE_FILE_FLAGS
#define tpp_file_flags uint_least8_t /* Set of `TPP_FILE_FLAGS_*` */
#define TPP_FILE_FLAGS_NORMAL       UINT8_C(0x00) /* Normal flags */
#if TPP_HAVE_FILE_NONBLOCK
#define TPP_FILE_FLAGS_NONBLOCK     UINT8_C(0x01) /* `TPP_FILE_KIND_IO`: Do non-blocking I/O */
#endif /* TPP_HAVE_FILE_NONBLOCK */
#if TPP_HAVE_FILE_NOCLOSE
#define TPP_FILE_FLAGS_NOCLOSE      UINT8_C(0x02) /* `TPP_FILE_KIND_IO`: Don't `tpp_io_close(tff_file)` on destruction */
#endif /* TPP_HAVE_FILE_NOCLOSE */
#if TPP_HAVE_FILE_NOKWD
#define TPP_FILE_FLAGS_NOKWD        UINT8_C(0x04) /* `TPP_FILE_KIND_IO` + `TPP_FILE_KIND_TEXT`: The file's `tff_name` field isn't actually a `tpp_keyword::tk_kwd`, but rather a raw `\0`-terminated C string. */
#endif /* TPP_HAVE_FILE_NOKWD */
#if !TPP_HAVE_USER_KEYWORDS && TPP_HAVE_LEXER_OPENFILE
#define TPP_FILE_FLAGS_FREENAME     UINT8_C(0x08) /* `TPP_FILE_KIND_IO` + `TPP_FILE_KIND_TEXT`: Must tpp_free(tff_name) when the file is finalized */
#endif /* !TPP_HAVE_USER_KEYWORDS && TPP_HAVE_LEXER_OPENFILE */
#if TPP_HAVE_FILE_SYSHDR
#define TPP_FILE_FLAGS_SYSHDR       UINT8_C(0x10) /* `TPP_FILE_KIND_IO` + `TPP_FILE_KIND_TEXT`: Suppress all warnings produced in the context of this file */
#endif /* TPP_HAVE_FILE_SYSHDR */
#if TPP_HAVE_FILE_EXTERN_C
#define TPP_FILE_FLAGS_EXTERN_C     UINT8_C(0x20) /* `TPP_FILE_KIND_IO` + `TPP_FILE_KIND_TEXT`: Treat everything within the file as being wrapped by an implicit `extern "C"` */
#endif /* TPP_HAVE_FILE_EXTERN_C */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#define TPP_FILE_FLAGS_NOGUARD      UINT8_C(0x40) /* A non-COMMENT/SPACE/LF (or blank/comment directive) was encountered since the start of the
                                                   * file. A `#ifndef`-directive encountered at this point can never count as a `#include`-guard. */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_CPP_DIRECTIVES
#define TPP_FILE_FLAGS_NODIRECTIVES UINT8_C(0x80) /* A non-COMMENT/SPACE token was encountered since the last
                                                   * `TPP_TOK_LF`, meaning PP-directives may not be parsed. */
#endif /* TPP_HAVE_CPP_DIRECTIVES */
#endif /* TPP_HAVE_FILE_FLAGS */



#if TPP_HAVE_IFDEF_STACK
typedef enum tpp_ifdef_mode {
	TPP_IFDEF_MODE_IFDEF, /* Inside of an active `#ifdef` (or `#if`, `#elif`, ...) block.
	                       * Allowed follow-up directives are: `#elif`, `#else`, `#endif` */
	TPP_IFDEF_MODE_ELSE,  /* Inside of an active `#else`-block (following an already-skipped `#if`-block)
	                       * Allowed follow-up directives are: `#endif` */
} tpp_ifdef_mode;

typedef struct tpp_ifdef_stack_entry {
	tpp_ifdef_mode TPP_INTERNAL(tidse_mode);    /* Behavioral mode of this entry */
	tpp_lcinfo     TPP_INTERNAL(tidse_created); /* line/column info of `#if`/`#ifdef`/`#ifndef` directive that created this block */
	tpp_lcinfo     TPP_INTERNAL(tidse_updated); /* line/column info of last directive that updated this block.
	                                             * Or in other words: staring-position of current sub-block
	                                             * e.g.: after `#elif` or `#else`, points at that directive, while
	                                             *       `tidse_created` continues to point at initial #if/... */
} tpp_ifdef_stack_entry;

#define tpp_ifdef_stack_entry_getcreated(self) (self)->TPP_INTERNAL(tidse_created)
#define tpp_ifdef_stack_entry_getupdated(self) (self)->TPP_INTERNAL(tidse_updated)

typedef struct tpp_ifdef_stack {
	tpp_size               TPP_INTERNAL(tids_alc); /* Allocated size of `tids_vec` */
	tpp_size               TPP_INTERNAL(tids_cnt); /* Used size of `tids_vec` */
	tpp_ifdef_stack_entry *TPP_INTERNAL(tids_vec); /* [0..tids_cnt][owned] Vector of active `#ifdef`-stack entries */
} tpp_ifdef_stack;

#define tpp_ifdef_stack_init(self)             \
	(void)((self)->TPP_INTERNAL(tids_alc) = 0, \
	       (self)->TPP_INTERNAL(tids_cnt) = 0, \
	       (self)->TPP_INTERNAL(tids_vec) = NULL)
#define tpp_ifdef_stack_fini(self) \
	tpp_free((self)->TPP_INTERNAL(tids_vec))

/* Clear the `#ifdef`-stack */
#define tpp_ifdef_stack_clear(self) \
	(tpp_ifdef_stack_fini(self),    \
	 tpp_ifdef_stack_init(self))

/* Check if the given `#ifdef`-stack is empty */
#define tpp_ifdef_stack_isempty(self) \
	((self)->TPP_INTERNAL(tids_cnt) == 0)

/* Return the last `#ifdef`-stack entry. The caller
 * must ensure that `!tpp_ifdef_stack_isempty(self)` */
#define tpp_ifdef_stack_getlast(self) \
	(&(self)->TPP_INTERNAL(tids_vec)[(self)->TPP_INTERNAL(tids_cnt) - 1])

/* Check if the most-recent `#ifdef`-stack entry indicates
 * that TPP is currently inside of a `#else`-block. The caller
 * must ensure that `!tpp_ifdef_stack_isempty(self)` */
#define tpp_ifdef_stack_isafterelse(self) \
	((self)->TPP_INTERNAL(tids_vec)[(self)->TPP_INTERNAL(tids_cnt) - 1].TPP_INTERNAL(tidse_mode) == TPP_IFDEF_MODE_ELSE)

/* Allocate an additional `#ifdef`-stack entry, and return a pointer to it.
 * This function will increment `self->tids_cnt`, but it is up to the
 * caller to initialize the returned `#ifdef`-stack entry
 *
 * @return: * :   The (uninitialized) `#ifdef`-stack entry
 * @return: NULL: Out of memory (`TPP_ENOMEM`) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_ifdef_stack_entry *TPPCALL
tpp_ifdef_stack_append(tpp_ifdef_stack *tpp_restrict self);

/* Do the inverse of `tpp_ifdef_stack_append()`, removing the last entry, and
 * potentially freeing unused memory.
 * The caller is responsible to ensure that `tpp_ifdef_stack_isnonempty(self)` */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_ifdef_stack_remove(tpp_ifdef_stack *tpp_restrict self);
#endif /* TPP_HAVE_IFDEF_STACK */


struct tpp_keyword;
#if TPP_HAVE_CPP_MACROS
struct tpp_macro;
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_FILE_MACRO_TRACKARGS
struct tpp_lexer_arginfo;
#endif /* TPP_HAVE_FILE_MACRO_TRACKARGS */

typedef struct tpp_file {
	tpp_char const     *TPP_INTERNAL(tf_tpos);  /* [?..?][valid_if(DID_CALL(tpp_lexer_yieldraw))]
	                                             * Start of last-loaded token (also valid in `tf_tprev`-files)
	                                             * WARNING: This field is NOT maintained/updated by `tpp_file_*` APIs
	                                             *          It is only here so it overlaps with the lexer's token's
	                                             *          `tt_start` field, such that said field is saved when
	                                             *          a new file is pushed onto the `#include`-stack, and can
	                                             *          then be used to calculate line/column information when
	                                             *          lexer prints its `#include`-stack. */
	/* Important: `tf_pos` and `tf_chunk` must come first, so they can shadow the tail of `tpp_token` */
	tpp_char const     *TPP_INTERNAL(tf_pos);   /* [0..1][<= tf_end] File pointer to next unread byte. */
	TPP_REF tpp_string *TPP_INTERNAL(tf_chunk); /* [0..1][const_if(tf_kind != TPP_FILE_KIND_IO)] Currently loaded text-chunk (mutable for text-files)
	                                             * WARNING: `tf_tpos` / `tf_pos` / `tf_end` may *NOT* necessarily point *into* this chunk (they are only
	                                             *          required to point into this chunk when when `tf_kind == TPP_FILE_KIND_IO`; all other kinds
	                                             *          of files do not guaranty that the text being parsed belongs to the current chunk; only that
	                                             *          the currently loaded chunk can be incref'd to prevent the text being parsed from being unloaded,
	                                             *          except when whoever assigned the chunk knows that nothing will try to incref it, as is the
	                                             *          whole point of the `TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS` flag).
	                                             *          That can happen when (e.g.) `tpp_file_subtext_setchunk_fromstring()` is used to assign
	                                             *          a chunk that doesn't actually contain the associated buffer (which can easily happen
	                                             *          when `tpp_lexer_parsestring_cb()` is used with `TPP_LEXER_PARSESTRING_FLAG_ALLOWTEMPS`,
	                                             *          or for a number of other reasons) */
	tpp_char const     *TPP_INTERNAL(tf_end);   /* [0..1][const_if(tf_kind != TPP_FILE_KIND_IO)] End of effective file content (mutable for text-files) */
#if TPP_HAVE_INCLUDE_STACK
	struct tpp_file    *TPP_INTERNAL(tf_prev);  /* [0..1] Parent file in #include stack */
	struct tpp_file    *TPP_INTERNAL(tf_tprev); /* [0..1] Real parent for the purposes of message tracebacks (not affected by `tpp_file_autopopfile_pushoff()`) */
#define _tpp_file_init_prev(self) , (self)->TPP_INTERNAL(tf_prev) = NULL, (self)->TPP_INTERNAL(tf_tprev) = NULL
#else /* TPP_HAVE_INCLUDE_STACK */
#define _tpp_file_init_prev(self) /* nothing */
#endif /* !TPP_HAVE_INCLUDE_STACK */
#if TPP_HAVE_FILE_LC_CACHE
	tpp_char const     *TPP_INTERNAL(tf_lcpos); /* [0..1] Position that `tf_lcval` applies to. */
	tpp_lcinfo          TPP_INTERNAL(tf_lcval); /* [valid_if(tf_lcpos)] Cached line/column at `tf_lcpos` */
#define _tpp_file_init_lcpos(self) , (self)->TPP_INTERNAL(tf_lcpos) = NULL
#else /* TPP_HAVE_FILE_LC_CACHE */
#define _tpp_file_init_lcpos(self) /* nothing */
#endif /* !TPP_HAVE_FILE_LC_CACHE */
#if TPP_HAVE_IFDEF_STACK
	tpp_ifdef_stack     TPP_INTERNAL(tf_ifdef); /* `#ifdef`-stack */
#define _tpp_file_init_ifdef(self) , tpp_ifdef_stack_init(&(self)->TPP_INTERNAL(tf_ifdef))
#else /* TPP_HAVE_IFDEF_STACK */
#define _tpp_file_init_ifdef(self) /* nothing */
#endif /* !TPP_HAVE_IFDEF_STACK */
	tpp_file_kind       TPP_INTERNAL(tf_kind);  /* [const] File kind */
#if TPP_HAVE_UNICODE
	tpp_file_encoding   TPP_INTERNAL(tf_enc);   /* File encoding */
#define _tpp_file_init_enc_ex(self, v) , (self)->TPP_INTERNAL(tf_enc) = v
#else /* TPP_HAVE_UNICODE */
#define _tpp_file_init_enc(self)       /* nothing */
#endif /* !TPP_HAVE_UNICODE */
#if TPP_HAVE_FILE_FLAGS
	tpp_file_flags      TPP_INTERNAL(tf_flags); /* File flags (set of `TPP_FILE_FLAGS_*`) */
#define _tpp_file_init_flags(self, v) , (self)->TPP_INTERNAL(tf_flags) = (v)
#else /* TPP_HAVE_FILE_FLAGS */
#define _tpp_file_init_flags(self, v) /* nothing */
#endif /* !TPP_HAVE_FILE_FLAGS */
#define _tpp_file_init_enc(self)   _tpp_file_init_enc_ex(self, TPP_FILE_ENCODING_UTF8)
	union {
		struct {
			char const *TPP_INTERNAL(tff_name);     /* [0..1][const] Filename by which this file was included (if available) */
			tpp_lcinfo  TPP_INTERNAL(tff_start_lc); /* [valid_if(tf_chunk != NULL)] Line/Column numbers (0-based) of `tf_chunk->ts_str`, or `TPP_LCINFO_INVALID` */
#if TPP_HAVE_FILE_SETFILENAME
			TPP_REF tpp_string *TPP_INTERNAL(tff_user_filename); /* [0..1] User-defined override for name of this file */
#define _tpp_file_init_io_user_filename(self) , (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(tff_user_filename) = NULL
#else /* TPP_HAVE_FILE_SETFILENAME */
#define _tpp_file_init_io_user_filename(self) /* nothing */
#endif /* !TPP_HAVE_FILE_SETFILENAME */
#if TPP_HAVE_FILE_GETHASH
			tpp_hash TPP_INTERNAL(tff_hash); /* [valid_if(tf_chunk != NULL)] Hash of all (decoded) file data up to the start of the current chunk */
#endif /* TPP_HAVE_FILE_GETHASH */
#if TPP_HAVE_FILE_KEEPPOS
			/* Need an additional `tpp_char const *ttf_keep` here whose only purpose
			 * is to set an extra, lower-bound of effective text-data to always
			 * keep loaded. But this pointer can also just be NULL, in which case
			 * only data in [tf_pos,tf_end) must be kept loaded!
			 * This pointer is needed when parsing the argument list of macros,
			 * and is needed to keep already-parsed argument data in memory when
			 * using tpp_lexer_yieldpp (which doesn't have a *_at variant), as
			 * well as keep the names of macros loaded such that (once the end
			 * of a macro argument list is found), the bottom-most macro name
			 * is still loaded in memory for `tpp_file_getlastpos()`:
			 * >> #define foo(a, b) a+b
			 * >> #define bar       foo(10
			 * >> bar,20)
			 * 1. While parsing macro arguments, when `bar` is popped from
			 *    the `#include`-stack, the string `10` from `bar` is saved
			 *    in `tpp_lexer_arginfo`, alongside a TPP_REF to the chunk
			 *    containing that string
			 * 2. Then, in the main file `tf_tpos` will point at `bar`, but
			 *    the next call to tpp_lexer_yieldpp() (that will eventually
			 *    return `,`) would be allocated to deallocate the chunk that
			 *    contains `bar` (which we don't want). Because of that, the
			 *    macro-argument parsing code must set `ttf_keep = tf_tpos`
			 *    (assuming that `!ttf_keep || ttf_keep > tf_tpos`) before
			 *    continuing to yield tokens, such that `bar` will continue
			 *    to remain loaded
			 * 3. Once the argument is fully loaded, `ttf_keep` can be reset
			 *    to its previous value, and `tf_tpos` can once again be set
			 *    to `bar`, allowing tracebacks to display the correct location */
			tpp_char const  *TPP_INTERNAL(ttf_keep);     /* [0..1][<= tf_end] Extra pointer specifying a position in memory that must be kept loaded.
			                                              * When `tpp_file_expandchunk()` is called, and this is field is non-NULL, it will
			                                              * be updated to point to the same effective position in a newly allocated chunk.
			                                              * If it is also less than `tf_pos`, then it represents of lower bound of memory
			                                              * to retain in a new file chunk. */
#define _tpp_file_init_io_keep(self) , (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(ttf_keep) = NULL
#else /* TPP_HAVE_FILE_KEEPPOS */
#define _tpp_file_init_io_keep(self) /* nothing */
#endif /* !TPP_HAVE_FILE_KEEPPOS */
			tpp_io_handle    TPP_INTERNAL(tff_file);     /* [owned_if(!TPP_FILE_FLAGS_NOCLOSE)] Underlying I/O file */
#if TPP_HAVE_UNICODE
			union {
				struct {
					uint_least8_t TPP_INTERNAL(tffu_tailc);    /* [valid_if(tf_enc) == TPP_FILE_ENCODING_UTF[16|32]_[LE|BE]] Read, unaligned tail data */
					unsigned char TPP_INTERNAL(tffu_tailv)[3]; /* [valid_if(tf_enc) == TPP_FILE_ENCODING_UTF[16|32]_[LE|BE]] Read, unaligned tail data */
				} TPP_INTERNAL(tffed_unicode);
#if TPP_HAVE_FILE_ENCODING_EMBED
				tpp_uintmax TPP_INTERNAL(tffed_embedlimit); /* Max # of remaining bytes that may be embedded */
#endif /* TPP_HAVE_FILE_ENCODING_EMBED */
			} TPP_INTERNAL(tff_encdat);
#endif /* TPP_HAVE_UNICODE */
		} TPP_INTERNAL(td_io); /* [tf_kind == TPP_FILE_KIND_IO] */

		struct {
			char const *TPP_INTERNAL(tft_name);     /* [0..1][const] Filename for messages (if available) */
			tpp_lcinfo  TPP_INTERNAL(tft_start_lc); /* [valid_if(tf_chunk != NULL)] Line/Column numbers (0-based) of `tf_chunk->ts_str`, or `TPP_LCINFO_INVALID` */
#if TPP_HAVE_FILE_SETFILENAME
			TPP_REF tpp_string *TPP_INTERNAL(tft_user_filename); /* [0..1] User-defined override for name of this file */
#define _tpp_file_init_text_user_filename(self) , (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_text).TPP_INTERNAL(tft_user_filename) = NULL
#else /* TPP_HAVE_FILE_SETFILENAME */
#define _tpp_file_init_text_user_filename(self) /* nothing */
#endif /* !TPP_HAVE_FILE_SETFILENAME */
		} TPP_INTERNAL(td_text); /* [tf_kind == TPP_FILE_KIND_TEXT] */

#if TPP_HAVE_CPP_MACROS
		struct {
			/* [1..1][const] The macro definition that produced this file
			 * as its expansion (also holds a reference to `tm_expansions`) */
			TPP_REF struct tpp_macro *TPP_INTERNAL(tfm_macro);
#if TPP_HAVE_FILE_MACRO_TRACKARGS
			/* [1..1][valid_if(tpp_macro_isfunction(tfm_macro))][owned][const]
			 * Arguments passed during macro invocation */
			struct tpp_lexer_arginfo *TPP_INTERNAL(tfm_args);
#endif /* TPP_HAVE_FILE_MACRO_TRACKARGS */
		} TPP_INTERNAL(td_macro); /* [tf_kind == TPP_FILE_KIND_MACRO] */
#endif /* TPP_HAVE_CPP_MACROS */

#if TPP_HAVE_FILE_DUMMY
		struct {
			char const *TPP_INTERNAL(tfd_name);     /* [0..1][const] Filename for messages (if available) */
			tpp_lcinfo  TPP_INTERNAL(tfd_start_lc); /* Line/Column numbers (0-based), or `TPP_LCINFO_INVALID` */
#if TPP_HAVE_FILE_SETFILENAME
			TPP_REF tpp_string *TPP_INTERNAL(tfd_user_filename); /* [0..1] User-defined override for name of this file */
#endif /* TPP_HAVE_FILE_SETFILENAME */
		} TPP_INTERNAL(td_dummy); /* [tf_kind == TPP_FILE_KIND_DUMMY] */
#endif /* TPP_HAVE_FILE_DUMMY */

	} TPP_INTERNAL(tf_data);
} tpp_file;

#define tpp_file_alloc() ((tpp_file *)tpp_malloc(sizeof(tpp_file)))
#define tpp_file_free(p) tpp_free(p)
#define tpp_file_move(dst, src) \
	(void)(*(dst) = *(src), tpp_dbg_memset(src, sizeof(tpp_file)))

#if TPP_HAVE_UNICODE
#define tpp_file_isutf8(self)  TPP_FILE_ENCODING_ISUTF8((self)->TPP_INTERNAL(tf_enc))
#define tpp_file_isascii(self) TPP_FILE_ENCODING_ISASCII((self)->TPP_INTERNAL(tf_enc))
#define tpp_file_getencoding(self) (self)->TPP_INTERNAL(tf_enc)
#else /* TPP_HAVE_UNICODE */
#define tpp_file_isutf8(self)  0
#define tpp_file_isascii(self) 1
#endif /* !TPP_HAVE_UNICODE */

/* Public API for accessing internal components of `tpp_file` */
#define tpp_file_getkind(self)  ((self)->TPP_INTERNAL(tf_kind))
#define tpp_file_getpos(self)   ((self)->TPP_INTERNAL(tf_pos))
#define tpp_file_getend(self)   ((self)->TPP_INTERNAL(tf_end))
#define tpp_file_getchunk(self) ((self)->TPP_INTERNAL(tf_chunk))
#if TPP_HAVE_IFDEF_STACK
#define tpp_file_getifdef(self) (&(self)->TPP_INTERNAL(tf_ifdef))
#endif /* !TPP_HAVE_IFDEF_STACK */


/* Access macro information */
#if TPP_HAVE_CPP_MACROS
#define tpp_file_ismacro(self)  (tpp_file_getkind(self) == TPP_FILE_KIND_MACRO)
#define tpp_file_getmacro(self) ((self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_macro).TPP_INTERNAL(tfm_macro))
#endif /* TPP_HAVE_CPP_MACROS */

/* Returns a pointer to the start of the effectively relevant source.
 * - For the currently loaded file, this is the start of the current
 *   token. If no tokens have been yielded yet, the value returned by
 *   this function is undefined.
 * - For macros further up the `#include`-stack, this (tries to) point
 *   to the start of the macro's name. However, if the macro's name
 *   was located in a file that has since been popped, this will
 *   instead be the start of whatever macro-invocation originally
 *   caused whatever file to be pushed which then eventually contained
 *   the start of the current macro-call:
 *   >> #define foo(a, b) a+b
 *   >> #define bar       foo(10
 *   >> bar,20)
 *      ^      ^ tpp_file_getpos(self)   (These are the pointed-to positions from the parent-
 *      tpp_file_getlastpos()             file of the one containing the expanded `10+20` text)
 */
#define tpp_file_getlastpos(self) ((self)->TPP_INTERNAL(tf_tpos))



/* Check if the next `tpp_lexer_yieldpp()` done in the context of this file is
 * allowed to parse directives. Since directives are only allowed to appear
 * directly following a line-feed within the same file, or at the start of some
 * file, this function allows you to check if the file is in a scenario where
 * the lexer yielding tokens from it is allowed to parse directives.  */
#if TPP_HAVE_CPP_DIRECTIVES
#define tpp_file_getallowdirectives(self) \
	(!((self)->TPP_INTERNAL(tf_flags) & TPP_FILE_FLAGS_NODIRECTIVES))
#else /* TPP_HAVE_CPP_DIRECTIVES */
#define tpp_file_getallowdirectives(self) 0
#endif /* !TPP_HAVE_CPP_DIRECTIVES */


/* Check if `self` has either been included via a -isystem path,
 * or made use of `#pragma GCC system_header`. In either case,
 * when this flags is set for `tpp_file_gettextfile(file)`, in
 * a call to `tpp_lexer_warnf()`, and the warning would otherwise
 * be emitted as `TPP_WSTATE_WARN`, the warning is ignored instead */
#if TPP_HAVE_FILE_SYSHDR
#define tpp_file_getsystemheader(self) \
	((self)->TPP_INTERNAL(tf_flags) & TPP_FILE_FLAGS_SYSHDR)
#else /* TPP_HAVE_FILE_SYSHDR */
#define tpp_file_getsystemheader(self) 0
#endif /* !TPP_HAVE_FILE_SYSHDR */


/* Check if the contents of `self` should be treated as being wrapped
 * by an implicit `extern "C"` block in C++. This flag is set for the
 * current *text-file* (tpp_file_gettextfile(tpp_lexer_getfile(self))),
 * and is controlled using  */
#if TPP_HAVE_FILE_EXTERN_C
#define tpp_file_getextern_c(self) \
	((self)->TPP_INTERNAL(tf_flags) & TPP_FILE_FLAGS_EXTERN_C)
#endif /* TPP_HAVE_FILE_EXTERN_C */


/* Return the predecessor of `self` for the purposes of #include tracebacks.
 * If `self` has no precessor (see `tpp_file_isbasefile()`), return `NULL`. */
#if TPP_HAVE_INCLUDE_STACK
#define tpp_file_getprev(self) (self)->TPP_INTERNAL(tf_tprev)
#else /* TPP_HAVE_INCLUDE_STACK */
#define tpp_file_getprev(self) ((tpp_file *)NULL)
#endif /* !TPP_HAVE_INCLUDE_STACK */


/* Check if `self` is the *base*-file (that is: the file that
 * doesn't have a parent, meaning that EOF here *will* result
 * in the lexer having to indicate TPP_TOK_EOF on all fronts) */
#if TPP_HAVE_INCLUDE_STACK
#define tpp_file_isbasefile(self) ((self)->TPP_INTERNAL(tf_tprev) == NULL)
#else /* TPP_HAVE_INCLUDE_STACK */
#define tpp_file_isbasefile(self) 1
#endif /* !TPP_HAVE_INCLUDE_STACK */


/* Initialize common fields of `self` */
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


/* - Disable I/O expansion by reading additional data from the file
 * - Make it so the file's EOF position can be overwritten freely
 *   (such that trying to yield additional tokens at/beyond that
 *   position will cause `tpp_lexer_yieldraw()` to return `TPP_TOK_EOF`)
 * - The previous state can be restored by `tpp_file_popeof()`
 *
 * WARNING:
 * - The caller must ensure that all files pushed are
 *   also popped before breaking out of a PUSHEOF block
 * - These functions don't save/restore the `#ifdef`-stack
 *   For that, also make use of `tpp_file_pushifdef()` */
#define tpp_file_pusheof(self)                                            \
	do {                                                                  \
		tpp_file_kind const _tfpeof_kind = (self)->TPP_INTERNAL(tf_kind); \
		tpp_char const *const _tfpeof_end = (self)->TPP_INTERNAL(tf_end); \
		_tpp_file_io2text(self)
#define tpp_file_setpos(self, pos) \
		(void)((self)->TPP_INTERNAL(tf_pos) = (pos))
#define tpp_file_seteof(self, end) \
		(void)((self)->TPP_INTERNAL(tf_end) = (end))
#define tpp_file_breakeof(self)                             \
		(void)((self)->TPP_INTERNAL(tf_end)  = _tfpeof_end, \
		       (self)->TPP_INTERNAL(tf_kind) = _tfpeof_kind)
#define tpp_file_popeof(self)    \
		tpp_file_breakeof(self); \
	} while (0)


/* Push (+clear) and later (clear+)restore the `#ifdef`-stack of a given file */
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


#if TPP_HAVE_FILE_KEEPPOS
/* Returns the keep-pointer for the file (which may be `NULL`).
 * Return value is undefined if `tpp_file_getkind(self) != TPP_FILE_KIND_IO` */
#define tpp_file_getkeep(self) \
	(self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(ttf_keep)

/* Push the current keep-pointer for `self` and setup a new pointer `ptr`
 * This macro has no effect if `tpp_file_getkind(self) != TPP_FILE_KIND_IO` */
#define tpp_file_pushkeep(self, ptr)                                                          \
	do {                                                                                      \
		tpp_size _tfpk_oldkeep = 0;                                                           \
		tpp_assert(!(self)->TPP_INTERNAL(tf_chunk) ||                                         \
		           (ptr) >= tpp_string_str((self)->TPP_INTERNAL(tf_chunk)));                  \
		tpp_assert((ptr) <= (self)->TPP_INTERNAL(tf_end));                                    \
		if (!tpp_file_getkeep(self)) {                                                        \
			_tfpk_oldkeep = TPP_SIZE_MAX;                                                     \
			(self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(ttf_keep) = (ptr); \
		} else if ((ptr) < tpp_file_getkeep(self)) {                                          \
			_tfpk_oldkeep = tpp_file_getkeep(self) - (ptr);                                   \
			(self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(ttf_keep) = (ptr); \
		}
#define tpp_file_breakkeep(self)                                                                   \
		(_tfpk_oldkeep == TPP_SIZE_MAX                                                             \
		 ? (void)((self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(ttf_keep) = NULL) \
		 : (void)((self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(ttf_keep) += _tfpk_oldkeep))
#define tpp_file_popkeep(self)    \
		tpp_file_breakkeep(self); \
	} while (0)
#else /* TPP_HAVE_FILE_KEEPPOS */
#define tpp_file_getkeep(self) tpp_file_getpos(self)
#endif /* !TPP_HAVE_FILE_KEEPPOS */


/* Push a (temporary) extra-file onto the `#include`-stack (replacing `self`)
 * whose contents can be overwritten to be a (sub-)text chunk of the actual
 * current file. If that (sub-)text chunk is the same as the chunk currently
 * loaded on the (then) parent-file, LC information remains available. Else,
 * calls to `tpp_file_getlcinfo()` return `TPP_LCINFO_INVALID` within the sub-
 * text file.
 *
 * These functions are meant for:
 * - tpp_file_subtext_setchunk_fromarg / tpp_lexer_seekpp_rparen:
 *   Re-parsing text retrieved from `tpp_lexer_arginfo`, as is necessary
 *   when expanding the arguments of macros. Because macro arguments can
 *   span across multiple files, and may contain (at that point removed)
 *   preprocessor directives, such processing is done via sub-text files:
 *   >> #define foo(a, b) a+b
 *   >> #define bar       foo(10 _,_
 *   >> bar 20)    // [10][ ][_][,][_][ ][20]
 *   The second argument `b` is `_ 20`, but those tokens are split across
 *   2 different text locations: `_` and ` 20`. As such, there will be no
 *   LC information for available within the sub-text of `b`. On the other
 *   hand, `a` is `10 _`, which appears consecutive thus continues to have
 *   LC information available
 * - tpp_file_subtext_setchunk_fromstring / tpp_lexer_parsestring_cb:
 *   Re-parsing text from a decoded string literal, as is necessary for
 *   evaluation of the `_Pragma()` builtin, as well as `#pragma tpp_exec()`
 *   Here, the same restrictions apply: if the contained string is single-
 *   chunked (meaning it's decoded text can alias its source file), then
 *   LC information is available. Otherwise, it isn't:
 *   - LC information available in pragma:
 *     >> _Pragma("push_macro('foo')")
 *   - No LC information available in pragma:
 *     >> _Pragma("push_macro(\"foo\")")
 *     (because string decode doesn't map linearly to source file)
 *
 * Usage:
 * >> tpp_errno TPPCALL my_parsestring_cb(void *arg, tpp_string *chunk,
   >>                                     tpp_char const *str, tpp_size length) {
 * >>     tpp_token_id tok;
 * >>     tpp_lexer *lexer = (tpp_lexer *)arg;
 * >>     tpp_file *file = tpp_lexer_getfile(lexer);
 * >>     tpp_file_subtext_push(file);
 * >>     tpp_file_subtext_setchunk_fromstring(file, chunk, str, length);
 * >>     tok = tpp_lexer_yield(lexer);
 * >>     ...
 * >>     tpp_file_subtext_pop(file);
 * >> }
 *
 * NOTES:
 * - A call to `tpp_file_subtext_push()` is similar to the combination of:
 *   - `tpp_file_pusheof()`
 *   - `tpp_file_pushifdef()`
 *   - `tpp_file_autopopfile_pushoff()`
 * - A call to `tpp_file_subtext_setchunk()` automatically does:
 *   - `tpp_file_seteof()`
 */
#if TPP_HAVE_FILE_SUBTEXT
#if TPP_HAVE_IFDEF_STACK
#define _tpp_file_subtext_init_ifdef(self) , tpp_ifdef_stack_init(&(self)->TPP_INTERNAL(tf_ifdef))
#define _tpp_file_subtext_fini_ifdef(self) tpp_ifdef_stack_fini(&(self)->TPP_INTERNAL(tf_ifdef)),
#else /* TPP_HAVE_IFDEF_STACK */
#define _tpp_file_subtext_init_ifdef(self)   /* nothing */
#define _tpp_file_subtext_fini_ifdef(self)   /* nothing */
#endif /* !TPP_HAVE_IFDEF_STACK */

#define tpp_file_subtext_push(self)                                   \
	do {                                                              \
		tpp_file _tfptf_prev = *(self);                               \
		(self)->TPP_INTERNAL(tf_prev)  = NULL; /* Prevent file pop */ \
		(self)->TPP_INTERNAL(tf_tprev) = &_tfptf_prev;                \
		(self)->TPP_INTERNAL(tf_kind)  = TPP_FILE_KIND_SUBTEXT        \
		_tpp_file_subtext_init_ifdef(self)                            \
		_tpp_file_init_lcpos(self)
#define tpp_file_subtext_setchunk(self, chunk, pos, end) \
		(void)((self)->TPP_INTERNAL(tf_chunk) = (chunk), \
		       (self)->TPP_INTERNAL(tf_pos)   = (pos),   \
		       (self)->TPP_INTERNAL(tf_end)   = (end))
#define tpp_file_subtext_setchunk_fromarg(self, arg) \
		tpp_file_subtext_setchunk(self, (arg)->tlai_chunk, (arg)->tlai_start, (arg)->tlai_end)
#define tpp_file_subtext_setchunk_fromstring(self, chunk, start, len) \
		tpp_file_subtext_setchunk(self, chunk, start, (start) + (len))
#define _tpp_file_subtext_break_common(self)                \
		tpp_assert((self)->TPP_INTERNAL(tf_prev) == NULL && \
		           "Extra files were pushed"),              \
		*(self) = _tfptf_prev
#define tpp_file_subtext_break(self)              \
		(void)(_tpp_file_subtext_fini_ifdef(self) \
		       _tpp_file_subtext_break_common(self))
#define tpp_file_subtext_pop(self)    \
		tpp_file_subtext_break(self); \
	} while (0)
#endif /* TPP_HAVE_FILE_SUBTEXT */



/* Tell an I/O file that it has been initialized, causing its associated
 * keyword's `tkm_file_inclcount` to be updated if necessary. */
#if TPP_HAVE_KEYWORD_INCLCOUNT
TPP_DECL TPP_NONNULL((1)) void TPPCALL
_tpp_file_io_notify_initialized(tpp_file *tpp_restrict self);
#else /* TPP_HAVE_KEYWORD_INCLCOUNT */
#define _tpp_file_io_notify_initialized(self) (void)0
#endif /* !TPP_HAVE_KEYWORD_INCLCOUNT */

/* Initialize `self` as a `TPP_FILE_KIND_IO` file
 * @param: char const    *filename: [0..1] Filename (if known)
 * @param: tpp_io_handle  fp:       File descriptor (inherited)
 * @param: tpp_file_flags flags:    File flags (set of `TPP_FILE_FLAGS_*`) */
#if TPP_HAVE_FILE_NOKWD
#define tpp_file_init_io(self, filename, /*inherit*/ fp) tpp_file_init_io_ex(self, filename, fp, TPP_FILE_FLAGS_NOKWD)
#else /* TPP_HAVE_FILE_NOKWD */
#define tpp_file_init_io(self, filename, /*inherit*/ fp) tpp_file_init_io_ex(self, filename, fp, TPP_FILE_FLAGS_NORMAL)
#endif /* !TPP_HAVE_FILE_NOKWD */
#define tpp_file_init_io_ex(self, filename, /*inherit*/ fp, flags) \
	tpp_file_init_io_ex2(self, filename, /*inherit*/ fp, flags, TPP_FILE_ENCODING_UTF8)
#define tpp_file_init_io_ex2(self, filename, /*inherit*/ fp, flags, enc)                          \
	(void)((self)->TPP_INTERNAL(tf_pos)   = NULL,                                                 \
	       (self)->TPP_INTERNAL(tf_chunk) = NULL,                                                 \
	       (self)->TPP_INTERNAL(tf_end)   = NULL                                                  \
	       _tpp_file_init_prev(self),                                                             \
	       (self)->TPP_INTERNAL(tf_kind) = TPP_FILE_KIND_IO                                       \
	       _tpp_file_init_enc_ex(self, enc)                                                       \
	       _tpp_file_init_flags(self, flags)                                                      \
	       _tpp_file_init_common(self),                                                           \
	       (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(tff_name) = (filename), \
	       (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_io).TPP_INTERNAL(tff_file) = (fp)        \
	       _tpp_file_init_io_user_filename(self)                                                  \
	       _tpp_file_init_io_keep(self),                                                          \
	       _tpp_file_io_notify_initialized(self))

/* Initialize `self` from a given `tpp_lexer_openfile_result` */
#if TPP_HAVE_LEXER_OPENFILE
#define tpp_file_init_io_from_ofr(self, /*inherit*/ /*tpp_lexer_openfile_result **/ ofr) \
	tpp_file_init_io_from_ofr_ex(self, ofr, TPP_FILE_ENCODING_UTF8)
#define tpp_file_init_io_from_ofr_ex(self, /*inherit*/ /*tpp_lexer_openfile_result **/ ofr, enc) \
	tpp_file_init_io_ex2(self, tpp_lexer_openfile_result_getfilename(ofr),                       \
	                     (ofr)->tlofr_handle, tpp_lexer_openfile_result_getfileflags(ofr), enc)
#endif /* TPP_HAVE_LEXER_OPENFILE */



/* Initialize `self ` as a `TPP_FILE_KIND_TEXT` file
 * @param: char const         *filename:  [0..1] Filename (if known)
 * @param: TPP_REF tpp_string *chunk:     [inherit(always)] File data chunk
 * @param: void const         *text:      File data base pointer
 * @param: tpp_size            text_size: File data size
 * @param: tpp_lcinfo          start_lc:  [valid_if(chunk)] 0-based line/column info for start of "text", or `TPP_LCINFO_INVALID`
 * @param: tpp_file_encoding   encoding:  File data encoding
 * @param: tpp_file_flags      flags:     File flags (set of `TPP_FILE_FLAGS_*`) */
#define tpp_file_init_text(self, filename, chunk, text, text_size, start_lc, flags) \
	tpp_file_init_text_ex(self, filename, chunk, text, text_size, start_lc, flags, TPP_FILE_ENCODING_UTF8)
#define tpp_file_init_text_ascii(self, filename, chunk, text, text_size, start_lc, flags) \
	tpp_file_init_text_ex(self, filename, chunk, text, text_size, start_lc, flags, TPP_FILE_ENCODING_ASCII)
#if TPP_HAVE_UNICODE
#define tpp_file_init_text_utf8(self, filename, chunk, text, text_size, start_lc, flags) \
	tpp_file_init_text_ex(self, filename, chunk, text, text_size, start_lc, flags, TPP_FILE_ENCODING_FORCE_UTF8)
#endif /* TPP_HAVE_UNICODE */
#define tpp_file_init_text_ex(self, filename, chunk, text, text_size, start_lc, flags, encoding)        \
	(void)((self)->TPP_INTERNAL(tf_pos)   = (tpp_char const *)(text),                                   \
	       (self)->TPP_INTERNAL(tf_chunk) = (chunk),                                                    \
	       (self)->TPP_INTERNAL(tf_end)   = (tpp_char const *)(text) + (text_size)                      \
	       _tpp_file_init_prev(self),                                                                   \
	       (self)->TPP_INTERNAL(tf_kind) = TPP_FILE_KIND_TEXT                                           \
	       _tpp_file_init_enc_ex(self, encoding)                                                        \
	       _tpp_file_init_flags(self, flags)                                                            \
	       _tpp_file_init_common(self)                                                                  \
	       _tpp_file_init_text_user_filename(self),                                                     \
	       (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_text).TPP_INTERNAL(tft_name)     = (filename), \
	       (self)->TPP_INTERNAL(tf_data).TPP_INTERNAL(td_text).TPP_INTERNAL(tft_start_lc) = (start_lc))


/* Initialize `self` as a macro expansion file (internal API) */
#if TPP_HAVE_CPP_MACROS
#define _tpp_file_init_macro(self, prev_file, /*inherit(always)*/ macro, \
                             /*inherit(always)*/ chunk, start, end)      \
	(void)((self)->tf_pos   = (start),                                   \
	       (self)->tf_chunk = (chunk),                                   \
	       (self)->tf_end   = (end)                                      \
	       _tpp_file_init_common(file),                                  \
	       (self)->tf_prev = (self)->tf_tprev = (prev_file),             \
	       (self)->tf_kind = TPP_FILE_KIND_MACRO                         \
	       _tpp_file_init_enc_ex(self, (macro)->tm_body_enc)             \
	       _tpp_file_init_macro_flags(self),                             \
	       ++((self)->tf_data.td_macro.tfm_macro = macro)->tm_expansions)
#if TPP_HAVE_CPP_DIRECTIVES
#define _tpp_file_init_macro_flags(self) _tpp_file_init_flags(self, TPP_FILE_FLAGS_NODIRECTIVES)
#else /* TPP_HAVE_CPP_DIRECTIVES */
#define _tpp_file_init_macro_flags(self) _tpp_file_init_flags(self, TPP_FILE_FLAGS_NORMAL)
#endif /* !TPP_HAVE_CPP_DIRECTIVES */
#endif /* TPP_HAVE_CPP_MACROS */



/* Finalize the given file. */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_file_fini(tpp_file *tpp_restrict self);


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
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_file_expandchunk(tpp_file *tpp_restrict self);

/* Encode/decode pointer<=>text-offset such that effective positions
 * are retained across calls to `tpp_file_expandchunk()`.
 *
 * WARNING: Any calls that modify `tf_pos`, or `tpp_token::tt_end`,
 *          including `tpp_lexer_yieldraw()` will CLOBBER all relative
 *          offsets within the current file. `tpp_lexer_yieldraw_at()`
 *          however will not (since that one doesn't modify the file's
 *          position as it uses the one given as argument instead).
 *
 * NOTES:
 *  - This needs to use `tf_pos` as relative base, since the start
 *    of the currently loaded chunk can change if another chunk is
 *    allocated that doesn't include the already-read buffer area
 *    located in `[tf_chunk->ts_str,tf_pos)`
 *  - Use these functions to support memory relocation across calls
 *    to `tpp_file_expandchunk()` (which may relocate the current
 *    text chunk)
 */
#define tpp_file_ptr2rel(self, ptr) (tpp_size)((ptr) - tpp_file_getpos(self))
#define tpp_file_rel2ptr(self, rel) (tpp_file_getpos(self) + (rel))

/* Same as above, but pointers are relative to the file's keep-position */
#if TPP_HAVE_FILE_KEEPPOS
#define tpp_file_keep_ptr2rel(self, ptr) (tpp_size)((ptr) - tpp_file_getkeep(self))
#define tpp_file_keep_rel2ptr(self, rel) (tpp_file_getkeep(self) + (rel))
#endif /* TPP_HAVE_FILE_KEEPPOS */


/* Return line/column information (0-based) for `pos`
 * @return: TPP_LCINFO_INVALID: line/column information could not be determined */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_lcinfo TPPCALL
tpp_file_getlcinfo(tpp_file *tpp_restrict self, tpp_char const *pos);

/* Helpers to return line/column information (0-based) for the
 * start/end positions of the last thing read from the file `self`:
 * - For the current file, this describes the start/end of the
 *   currently loaded token
 * - If the current file is a macro (`tpp_file_ismacro()`), and
 *   this function is called on its parent (`tpp_file_getprev()`),
 *   then this returns the bounds of the token sequence that was
 *   used to make the macro call
 * - If the current file is an I/O or TEXT file, and this function
 *   is called on its parent (`tpp_file_getprev()`), then this returns
 *   the bounds of the `#include`-directive that was used to include
 *   the child-file.
 * - When passed file returned by `tpp_file_getlcfile()`, this will
 *   return the line/column values associated with the `__LINE__`
 *   macro (and it's TPP `__COLUMN__` extension). This last case is
 *   what you probably want to use.
 *
 * Examples:
 * >> #define assert(x) (void)((x) || (_assert(#x, __FILE__, __LINE__, __COLUMN__), 0))
 * >> ...
 * >>
 * >> if (x)
 * >>     assert(y);
 *        ^        ^ tpp_file_getpos() / tpp_file_getendlcinfo()
 *        tpp_file_getlastpos() / tpp_file_getstartlcinfo()
 *
 * iow: `tpp_file_getlastpos()` position for tracebacks (points at what *caused* a macro/file push)
 *      `tpp_file_getpos()` position of next byte to-be parsed once lexer returns to this file. */
TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_lcinfo TPPCALL
tpp_file_getstartlcinfo(tpp_file *tpp_restrict self) {
	return tpp_file_getlcinfo(self, tpp_file_getlastpos(self));
}
TPP_INLINE TPP_WUNUSED TPP_NONNULL((1)) tpp_lcinfo TPPCALL
tpp_file_getendlcinfo(tpp_file *tpp_restrict self) {
	return tpp_file_getlcinfo(self, tpp_file_getpos(self));
}

/* Returns the filename of `self`, or `NULL` if unknown. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_getrealfilename(tpp_file const *tpp_restrict self);

#if TPP_HAVE_FILE_GETREALFILENAMEKWD
/* Returns the filename *keyword* (which may not always be available,
 * even when `tpp_file_getrealfilename()` returns non-NULL) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) struct tpp_keyword *TPPCALL
tpp_file_getrealfilenamekwd(tpp_file const *tpp_restrict self);
#endif /* TPP_HAVE_FILE_GETREALFILENAMEKWD */

/* Same as `tpp_file_getrealfilename()`, but may be overwritten by "#line" directives */
#if TPP_HAVE_FILE_SETFILENAME
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) /*utf-8*/ char const *TPPCALL
tpp_file_getfilename(tpp_file const *tpp_restrict self);

/* Same as `tpp_file_getfilename()`, but if the filename was overwritten
 * by use of `tpp_file_setfilename()`, this returns the string object that
 * was passed during that override (so the caller can `tpp_string_incref()`
 * that string to preserve it across further tpp_file_setfilename-calls).
 *
 * WARNING: This returns `NULL` if the current filename wasn't set by a call
 *          to `tpp_file_setfilename()`, even when `tpp_file_getfilename()`
 *          would return non-NULL */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_string *TPPCALL
tpp_file_getfilenamestr(tpp_file const *tpp_restrict self);

/* Sets the user-filename override of `self` to `filename`
 *
 * NOTE: The caller must ensure that:
 *       >> self->tf_kind == TPP_FILE_KIND_IO ||
 *       >> self->tf_kind == TPP_FILE_KIND_TEXT;
 *
 * You may also pass `NULL` for `filename` to disable the override */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_file_setfilename(tpp_file *tpp_restrict self, tpp_string *filename);
#else /* TPP_HAVE_FILE_SETFILENAME */
#define tpp_file_getfilename(self)    tpp_file_getrealfilename(self)
#define tpp_file_getfilenamestr(self) ((tpp_string *)NULL)
#endif /* !TPP_HAVE_FILE_SETFILENAME */

/* Set the (0-based) line that applies to `pos`
 * (as returned by `tpp_file_getlcinfo()`) in `self`
 *
 * NOTE: The caller must ensure that:
 *       >> self->tf_kind == TPP_FILE_KIND_IO ||
 *       >> self->tf_kind == TPP_FILE_KIND_TEXT; */
#if TPP_HAVE_FILE_SETLINE
TPP_DECL TPP_NONNULL((1, 2)) void TPPCALL
tpp_file_setline(tpp_file *tpp_restrict self,
                 tpp_char const *pos, tpp_line line);
#endif /* TPP_HAVE_FILE_SETLINE */

#if TPP_HAVE_FILE_GETHASH
/* Return the hash of all (decoded) bytes read from `self` up to (but not including) `pos`
 * This *includes* the bytes of any already-unloaded chunk of `self`, though `pos` must
 * point into the current chunk (past hash values from previous chunks cannot be determined)
 *
 * Also note that the hash can *only* be determined when `tpp_file_getchunk(self) != NULL`.
 * If the file doesn't have an input chunk (e.g.: its contents are statically allocated),
 * then this function always returns the same value. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_hash TPPCALL
tpp_file_gethash(tpp_file const *tpp_restrict self, tpp_char const *pos);
#endif /* TPP_HAVE_FILE_GETHASH */

#if TPP_HAVE_FILE_GETFULLHASH
/* Same as `tpp_file_gethash()`, but also includes the hash values of all parent files,
 * such that the `tpp_file_gethash(f, tpp_file_getlastpos(f))` of every file reachable
 * via `tpp_file_getprev()` is included in the return value in one way or another. Note
 * that the value returned here may be different from the hash that would be calculated
 * if all `#include`-ed files had been inlined into each other. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_hash TPPCALL
tpp_file_getfullhash(tpp_file const *tpp_restrict self, tpp_char const *pos);
#endif /* TPP_HAVE_FILE_GETFULLHASH */

typedef struct tpp_lcinfo_ex {
	tpp_lcinfo      tlcix_info;     /* Line/column information, or `TPP_LCINFO_INVALID` if unknown */
#if TPP_HAVE_CPP_MACROS
	/* Projection (source) file:
	 * >> #define foo(x) 10+x+20
	 * >> foo(15)
	 *
	 * When requesting lcinfo about the `15` token in the expanded
	 * file (`10+15+20`), `tlcix_projfile` points at the file containing
	 * `foo(15)`, and `tlcix_projpos` points at the `15` in `foo(15)`.
	 * Meanwhile, `tlcix_info` will point at the `x` token in the
	 * definition of `foo`.
	 *
	 * There are many reasons why projection can fail, even when you
	 * might think that there isn't a reason why it should. This is
	 * because there are some edge-cases where TPP3 is unable to
	 * retroactively determine the correct position of tokens, such
	 * as use of `##`-operators, or changing lexer features between
	 * the macro being expanded, and `tpp_file_getlcinfo_ex()` being
	 * called. Depending on configuration (and especially changes
	 * thereof), the projection location might even be wrong. */
	tpp_file       *tlcix_projfile; /* [0..1] Projection source file, or NULL if queried position wasn't projected */
	tpp_char const *tlcix_projpos;  /* [1..1][valid_if(tlcix_fromfile)] Position in `tlcix_projfile` */
#endif /* TPP_HAVE_CPP_MACROS */
} tpp_lcinfo_ex;

/* Same as `tpp_file_getlcinfo()`, but if the current file is an expanded macro, see if
 * the specified `pos` points into the expanded portion of a macro argument, in which
 * case this function also (tries to) include information on where that argument was
 * projected from. */
#if TPP_HAVE_CPP_MACROS
TPP_DECL TPP_NONNULL((1, 2, 3)) void TPPCALL
tpp_file_getlcinfo_ex(tpp_file *tpp_restrict self, tpp_char const *pos,
                      tpp_lcinfo_ex *tpp_restrict result);
#else /* TPP_HAVE_CPP_MACROS */
#define tpp_file_getlcinfo_ex(self, pos, result) \
	(void)((result)->tlcix_info = tpp_file_getlcinfo(self, pos))
#endif /* !TPP_HAVE_CPP_MACROS */




#if TPP_HAVE_INCLUDE_STACK
/* Returns the last file in the `#include`-stack (using `tf_tprev`) */
TPP_DECL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getbasefile(tpp_file const *tpp_restrict self);

#if TPP_HAVE_CPP_MACROS || TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_FILE_DUMMY
/* Returns the first `tf_kind==TPP_FILE_KIND_IO || tf_kind==TPP_FILE_KIND_TEXT` file
 * in the `#include`-stack (using `tf_tprev`). If no such file exists, returns `NULL` */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_gettextfile(tpp_file const *tpp_restrict self);

/* Similar to `tpp_file_gettextfile()`, but re-return `self` instead of returning `NULL`.
 * Also: try not to return a file with invalid L/C information (which could otherwise
 *       happen as a result of manually pushed text files that don't contain L/C info)
 *
 * The term `lc` here refers to the fact that this is the file that's used as basis
 * for the builtin __FILE__, __LINE__ and __COLUMN__ macros. */
TPP_DECL TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1)) tpp_file *TPPCALL
tpp_file_getlcfile(tpp_file const *tpp_restrict self);
#else /* TPP_HAVE_CPP_MACROS || TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_FILE_DUMMY */
#define tpp_file_gettextfile(self) ((tpp_file *)(self))
#define tpp_file_getlcfile(self)   ((tpp_file *)(self))
#endif /* !TPP_HAVE_CPP_MACROS || TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_FILE_DUMMY */

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
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_file_pushdummy(tpp_file *tpp_restrict self, tpp_char const *pos);

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
TPP_DECL TPP_NONNULL((1)) bool TPPCALL
tpp_file_popdummy(tpp_file *tpp_restrict self);
#endif /* TPP_HAVE_FILE_DUMMY */

#else /* TPP_HAVE_INCLUDE_STACK */
#define tpp_file_getlcfile(self) ((tpp_file *)(self))
#if TPP_HAVE_CPP_MACROS || TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_FILE_DUMMY
#define tpp_file_gettextfile(self)                         \
	(((self)->TPP_INTERNAL(tf_kind) == TPP_FILE_KIND_IO || \
	  (self)->TPP_INTERNAL(tf_kind) == TPP_FILE_KIND_TEXT) \
	 ? ((tpp_file *)(self))                                \
	 : NULL)
#else /* TPP_HAVE_CPP_MACROS || TPP_HAVE_FILE_SUBTEXT || TPP_HAVE_FILE_DUMMY */
#define tpp_file_gettextfile(self) ((tpp_file *)(self))
#endif /* !TPP_HAVE_CPP_MACROS && !TPP_HAVE_FILE_SUBTEXT && !TPP_HAVE_FILE_DUMMY */
#endif /* !TPP_HAVE_INCLUDE_STACK */


/* Width of `\t` as reported by `tpp_file_getlcinfo()` */
#if TPP_TABSIZE >= 0
#define tpp_gettabsize() TPP_TABSIZE
#else /* TPP_TABSIZE >= 0 */
TPP_DECL tpp_column _tpp_tabsize; /* Internal API -- use getters/setters below */
#define tpp_gettabsize()  _tpp_tabsize
#define tpp_settabsize(v) (void)(_tpp_tabsize = (v))
#endif /* TPP_TABSIZE < 0 */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FILE_H */
