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
#ifndef GUARD_TPP_KEYWORD_H
#define GUARD_TPP_KEYWORD_H 1

#include "api.h"

#include "config.h"
#include "error.h"
#include "file.h"
#include "file-io.h"
#include "string.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_PRAGMA_PUSH_MACRO
struct tpp_macro;
typedef struct tpp_macro_pushent {
	TPP_REF struct tpp_macro *TPP_INTERNAL(tmpe_macro); /* [0..1] The macro that was pushed, or "NULL" if not defined at the time. */
	tpp_size                  TPP_INTERNAL(tmpe_count); /* # of times that `tmpe_macro' was pushed without the macro actually having changed */
} tpp_macro_pushent;

typedef struct tpp_macro_pushstack {
	tpp_size           TPP_INTERNAL(tmps_cnt); /* # of elements on `tmps_vec' */
	tpp_macro_pushent *TPP_INTERNAL(tmps_vec); /* [0..tmps_vec][owned] Vector of pushed macros (push_macro appends at the end; pop_macro takes from the end) */
} tpp_macro_pushstack;

/* Initialize/finalize a given macro-push stack */
#define tpp_macro_pushstack_init(self)         \
	(void)((self)->TPP_INTERNAL(tmps_cnt) = 0, \
	       (self)->TPP_INTERNAL(tmps_vec) = NULL)
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_macro_pushstack_fini(tpp_macro_pushstack *tpp_restrict self);
#if TPP_HAVE_LEXER_COPY
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_macro_pushstack_copy(tpp_macro_pushstack *tpp_restrict self,
                         tpp_macro_pushstack const *tpp_restrict from);
#endif /* TPP_HAVE_LEXER_COPY */

/* Allocate space for- and return a new (uninitialized) macro-push entry
 * @return: * :   The newly allocated macro-push entry.
 * @return: NULL: Out-of-memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_pushent *TPPCALL
tpp_macro_pushstack_append(tpp_macro_pushstack *tpp_restrict self);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#undef TPP_HAVE_KEYWORD_FLAGS
#if (TPP_HAVE_PRAGMA_ONCE ||                          \
     TPP_HAVE_CPP_IMPORT ||                           \
     TPP_HAVE_CLANG_MACRO___has_attribute ||          \
     TPP_HAVE_CLANG_MACRO___has_builtin ||            \
     TPP_HAVE_CLANG_MACRO___has_cpp_attribute ||      \
     TPP_HAVE_CLANG_MACRO___has_declspec_attribute || \
     TPP_HAVE_CLANG_MACRO___has_extension ||          \
     TPP_HAVE_CLANG_MACRO___has_feature ||            \
     TPP_HAVE_CLANG_MACRO___has_c_attribute ||        \
     TPP_HAVE_MACRO___is_deprecated ||                \
     TPP_HAVE_MACRO___is_poisoned ||                  \
     TPP_HAVE_PRAGMA_DEPRECATED ||                    \
     TPP_HAVE_PRAGMA_GCC_POISON ||                    \
     TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS)
#define TPP_HAVE_KEYWORD_FLAGS 1
#else /* ... */
#define TPP_HAVE_KEYWORD_FLAGS 0
#endif /* !... */

#undef TPP_HAVE_KEYWORD_MISC
#if (TPP_HAVE_KEYWORD_FLAGS ||         \
     TPP_HAVE_IFNDEF_INCLUDE_GUARDS || \
     TPP_HAVE_PRAGMA_PUSH_MACRO ||     \
     TPP_HAVE_MACRO___TPP_COUNTER ||   \
     TPP_HAVE_KEYWORD_USERDATA)
#define TPP_HAVE_KEYWORD_MISC 1
#else /* ... */
#define TPP_HAVE_KEYWORD_MISC 0
#endif /* !... */



#if TPP_HAVE_KEYWORD_MISC

/* Keyword flags... */
#if TPP_HAVE_KEYWORD_FLAGS
#define tpp_keyword_flags uint_least32_t /* Set of `TPP_KEYWORD_FLAG_*' */
#define TPP_KEYWORD_FLAG_NORMAL                 UINT32_C(0x00000000) /* Normal flags */
#if TPP_HAVE_CLANG_MACRO___has_attribute
#define TPP_KEYWORD_FLAG_HAS_ATTRIBUTE          UINT32_C(0x00000001) /* `__has_attribute()' */
#endif /* TPP_HAVE_CLANG_MACRO___has_attribute */
#if TPP_HAVE_CLANG_MACRO___has_builtin
#define TPP_KEYWORD_FLAG_HAS_BUILTIN            UINT32_C(0x00000002) /* `__has_builtin()' */
#endif /* TPP_HAVE_CLANG_MACRO___has_builtin */
#if TPP_HAVE_CLANG_MACRO___has_cpp_attribute
#define TPP_KEYWORD_FLAG_HAS_CPP_ATTRIBUTE      UINT32_C(0x00000004) /* `__has_cpp_attribute()' */
#endif /* TPP_HAVE_CLANG_MACRO___has_cpp_attribute */
#if TPP_HAVE_CLANG_MACRO___has_declspec_attribute
#define TPP_KEYWORD_FLAG_HAS_DECLSPEC_ATTRIBUTE UINT32_C(0x00000008) /* `__has_declspec_attribute()' */
#endif /* TPP_HAVE_CLANG_MACRO___has_declspec_attribute */
#if TPP_HAVE_CLANG_MACRO___has_extension
#define TPP_KEYWORD_FLAG_HAS_EXTENSION          UINT32_C(0x00000010) /* `__has_extension()' */
#endif /* TPP_HAVE_CLANG_MACRO___has_extension */
#if TPP_HAVE_CLANG_MACRO___has_feature
#define TPP_KEYWORD_FLAG_HAS_FEATURE            UINT32_C(0x00000020) /* `__has_feature()' */
#endif /* TPP_HAVE_CLANG_MACRO___has_feature */
#if TPP_HAVE_PRAGMA_DEPRECATED || TPP_HAVE_MACRO___is_deprecated
#define TPP_KEYWORD_FLAG_IS_DEPRECATED          UINT32_C(0x00000040) /* Warn when the keyword appears as the result of lexical processing. */
#endif /* TPP_HAVE_PRAGMA_DEPRECATED || TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS
#define TPP_KEYWORD_FLAG_USERMASK               UINT32_C(0x0000007f) /* Set of flags modifiable through `#pragma tpp_set_keyword_flags()'. */
#endif /* TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS */
#if TPP_HAVE_PRAGMA_GCC_POISON || TPP_HAVE_MACRO___is_poisoned
#define TPP_KEYWORD_FLAG_IS_POISONED            UINT32_C(0x00000080) /* Extension for `TPP_KEYWORD_FLAG_IS_DEPRECATED':
                                                                      * Don't emit a warning if the keyword is used inside of a macro.
                                                                      * -> Only warn if it is used from a text file. */
#endif /* TPP_HAVE_PRAGMA_GCC_POISON || TPP_HAVE_MACRO___is_poisoned */
#if TPP_HAVE_CLANG_MACRO___has_c_attribute
#define TPP_KEYWORD_FLAG_HAS_C_ATTRIBUTE        UINT32_C(0x00000100) /* `__has_c_attribute()' */
#endif /* TPP_HAVE_CLANG_MACRO___has_c_attribute */
#if TPP_HAVE_CPP_IMPORT
#define TPP_KEYWORD_FLAG_HDR_IMPORTED           UINT32_C(0x20000000) /* Set after this header was `#import'-ed */
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_PRAGMA_ONCE
#define TPP_KEYWORD_FLAG_HDR_ONCE               UINT32_C(0x40000000) /* Set after `#pragma once' was encountered */
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#define TPP_KEYWORD_FLAG_HDR_GUARD_VALID        UINT32_C(0x80000000) /* The configured "tkm_file_guard" is valid and should be used
                                                                      * (Set when the file is removed from the #include-stack with
                                                                      * its #ifdef-stack empty, and "tkm_file_guard != NULL") */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#endif /* TPP_HAVE_KEYWORD_FLAGS */

struct tpp_keyword;
typedef struct tpp_keyword_misc {
#if TPP_HAVE_KEYWORD_FLAGS
	tpp_keyword_flags TPP_INTERNAL(tkm_flags); /* Set of `TPP_KEYWORD_FLAG_*' */
#endif /* TPP_HAVE_KEYWORD_FLAGS */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
	struct tpp_keyword const *TPP_INTERNAL(tkm_file_guard); /* [0..1] Name of the #include guard for this file, or NULL if unknown. */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#if TPP_HAVE_PRAGMA_PUSH_MACRO
	tpp_macro_pushstack TPP_INTERNAL(tkm_macro_pushstack); /* For `#pragma push_macro()' */
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#if TPP_HAVE_MACRO___TPP_COUNTER
	tpp_size TPP_INTERNAL(tkm_builtin_counter); /* Next value for __TPP_COUNTER */
#endif /* TPP_HAVE_MACRO___TPP_COUNTER */
#if TPP_HAVE_KEYWORD_USERDATA
	void          *TPP_INTERNAL(tkm_userdata_ptr); /* [?..?] User-data pointer (initialize to "NULL") */
	void (TPPCALL *TPP_INTERNAL(tkm_userdata_dtor))(void *ptr); /* [0..1] Optional finalizer for user-data */
#define tpp_keyword_misc_setuserdata(self, ptr, dtor) \
	(void)((self)->tkm_userdata_ptr  = (ptr),         \
	       (self)->tkm_userdata_dtor = (dtor))
#define tpp_keyword_misc_getuserdata(self)      ((self)->tkm_userdata_ptr)
#define tpp_keyword_misc_getuserdata_dtor(self) ((self)->tkm_userdata_dtor)
#else /* TPP_HAVE_KEYWORD_USERDATA */
#define tpp_keyword_misc_getuserdata(self)      ((void *)NULL)
#define tpp_keyword_misc_getuserdata_dtor(self) ((void (TPPCALL *)(void *))NULL)
#endif /* !TPP_HAVE_KEYWORD_USERDATA */
} tpp_keyword_misc;

#define _tpp_keyword_misc_alloc() ((tpp_keyword_misc *)tpp_malloc(sizeof(tpp_keyword_misc)))
#define _tpp_keyword_misc_free(p) tpp_free(p)
#endif /* TPP_HAVE_KEYWORD_MISC */


#if TPP_HAVE_CPP_MACROS
struct tpp_macro;
#endif /* TPP_HAVE_CPP_MACROS */

#undef TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
typedef struct tpp_keyword {
	tpp_token_id              TPP_INTERNAL(tk_id);                  /* [const] Keyword ID */
	struct tpp_keyword       *TPP_INTERNAL(tk_next);                /* [0..1] Next keyword with a similar hash */
#if TPP_HAVE_CPP_MACROS
	TPP_REF struct tpp_macro *TPP_INTERNAL(tk_macro);               /* [0..1][const_if(IS_BUILTIN)] Macro definition */
#define TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS 1
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	tpp_keyword_misc         *TPP_INTERNAL(tk_misc);                /* [0..1][const_if(IS_BUILTIN)][owned] Misc. keyword data (lazily allocated) */
#define TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS 1
#endif /* TPP_HAVE_KEYWORD_MISC */
	tpp_hash                  TPP_INTERNAL(tk_hash);                /* [const] Hash for `tk_kwd' */
#if TPP_HAVE_KEYWORD_ASSTRING
	tpp_refcnt_atomic         TPP_INTERNAL(tk_refcnt);              /* Keyword reference count (for binary compatibility with "tpp_string") */
#endif /* TPP_HAVE_KEYWORD_ASSTRING */
	tpp_size                  TPP_INTERNAL(tk_len);                 /* [const] # of bytes (char-s) in `tk_kwd' (excluding trailing \0) */
	tpp_char                  TPP_INTERNAL(tk_kwd)[TPP_FLEX_ARRAY]; /* [const][tk_len] Keyword string (in input encoding; \0-terminated; never contains \-escaped linefeeds) */
/*	tpp_char                  TPP_INTERNAL(tk_nul);                  * [const][== 0] Ensure ZERO-termination of the keyword name. */
} tpp_keyword;

#if TPP_HAVE_USER_KEYWORDS
#define tpp_keyword_sizeof(len) \
	(tpp_offsetof(tpp_keyword, TPP_INTERNAL(tk_kwd)) + ((len) + 1) * sizeof(tpp_char))
#define _tpp_keyword_alloc(len)         ((tpp_keyword *)tpp_malloc(tpp_keyword_sizeof(len)))
#define _tpp_keyword_tryalloc(len)      ((tpp_keyword *)tpp_trymalloc(tpp_keyword_sizeof(len)))
#define _tpp_keyword_realloc(p, len)    ((tpp_keyword *)tpp_realloc(p, tpp_keyword_sizeof(len)))
#define _tpp_keyword_tryrealloc(p, len) ((tpp_keyword *)tpp_tryrealloc(p, tpp_keyword_sizeof(len)))
#define _tpp_keyword_free(p)            tpp_free(p)
#endif /* TPP_HAVE_USER_KEYWORDS */

/* When true, there are certain actions that require builtin keywords
 * to be copied into the current lexer's keyword table. These include
 * user-defined macros (with built-in identifiers as names), as well
 * as any other "misc"-related, custom data being assigned to keywords */
#ifndef TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
#define TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS 0
#endif /* !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */

/* Use this macro for comparing keywords instead of doing "a == b"
 * We need to compare the IDs of keywords, since builtin keywords
 * may need to be copied into the current lexer's `tpp_keywords'
 * if `tk_macro' or `tk_misc' need to be modified */
#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
#define tpp_keyword_equals(a, b) ((a)->TPP_INTERNAL(tk_id) == (b)->TPP_INTERNAL(tk_id))
#else /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */
#define tpp_keyword_equals(a, b) ((a) == (b))
#endif /* !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */

/* Public API for accessing "tpp_keyword" internals */
#define tpp_keyword_isuser(self)     TPP_TOK_ISUSERKEYWORD((self)->TPP_INTERNAL(tk_id))
#define tpp_keyword_isbuiltin(self)  (!tpp_keyword_isuser(self))
#define tpp_keyword_getid(self)      ((self)->TPP_INTERNAL(tk_id))
#define tpp_keyword_getkwd(self)     ((self)->TPP_INTERNAL(tk_kwd))
#define tpp_keyword_getkwdcstr(self) ((char const *)(self)->TPP_INTERNAL(tk_kwd))
#define tpp_keyword_getkwdlen(self)  ((self)->TPP_INTERNAL(tk_len))
#define tpp_keyword_getkwdhash(self) ((self)->TPP_INTERNAL(tk_hash))
#if TPP_HAVE_CPP_MACROS
#define tpp_keyword_getmacro(self) ((self)->TPP_INTERNAL(tk_macro))
#endif /* TPP_HAVE_CPP_MACROS */

/* Check if "self" matches the C, constant string literal "cstr" */
#define tpp_keyword_equals_cstr(self, cstr)                               \
	((self)->TPP_INTERNAL(tk_len) == (sizeof(cstr) / sizeof(char)) - 1 && \
	 tpp_memcmp((self)->TPP_INTERNAL(tk_kwd), cstr, sizeof(cstr) - sizeof(char)) == 0)


/* Convert back-and-forth between keywords and strings */
#if TPP_HAVE_KEYWORD_ASSTRING
#define _TPP_KEYWORD_STRING_ABI_START TPP_INTERNAL(tk_refcnt)
#define tpp_keyword_asstring(self) ((tpp_string *)&(self)->_TPP_KEYWORD_STRING_ABI_START)
#define tpp_string_askeyword(self) ((tpp_keyword *)((char *)(self) - tpp_offsetof(tpp_keyword, _TPP_KEYWORD_STRING_ABI_START)))
#endif /* TPP_HAVE_KEYWORD_ASSTRING */

#if TPP_HAVE_KEYWORD_MISC
/* Ensure that `self->tk_misc' has been allocated and return it.
 * If it isn't already allocated, allocate+return it lazily.
 * WARNING: Only call this function on a "writable" keyword (s.a. `tpp_keywords_copybuiltin()')
 *
 * @return: * :   The "misc" data of "self" (freshly allocated)
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword_misc *TPPCALL
tpp_keyword_requiremisc(tpp_keyword *tpp_restrict self); /* TODO: This function shouldn't be part of the public API! */

/* Same as "tpp_keyword_requiremisc()", but don't lazily allocate,
 * and simply return "NULL" if "self" doesn't have misc-data, yet. */
#define tpp_keyword_getmisc(self) ((self)->TPP_INTERNAL(tk_misc))
#endif /* TPP_HAVE_KEYWORD_MISC */

#if TPP_HAVE_KEYWORD_USERDATA
/* Get the user-data pointer for "self"
 * @return: NULL: No pointer set, or set pointer is "NULL" */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) void *TPPCALL
tpp_keyword_getuserdata(tpp_keyword const *tpp_restrict self);

/* Set the user-data pointer for "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_setuserdata(tpp_keyword *tpp_restrict self,
                        void *ptr, void (TPPCALL *dtor)(void *ptr));
#endif /* TPP_HAVE_KEYWORD_USERDATA */

#if TPP_HAVE_PRAGMA_PUSH_MACRO
/* Push the current macro-definition of "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_pushmacro(tpp_keyword *tpp_restrict self);

/* Pop the current macro-definition of "self"
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: Macro-push-stack was already empty (soft-error) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_popmacro(tpp_keyword *tpp_restrict self);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_CPP_MACROS
/* Check if "tpp_keyword_undef()" can be invoked on "self" */
#define tpp_keyword_canundef(self) \
	(tpp_keyword_getmacro(self) != NULL)

/* Delete the macro definition of `self'.
 * The caller must ensure that `tpp_keyword_canundef(self)' */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keyword_undef(tpp_keyword *tpp_restrict self);
#endif /* TPP_HAVE_CPP_MACROS */



/* Calculate the hash of a given keyword string */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof(tpp_char const *tpp_restrict kwd, tpp_size len);


#if TPP_HAVE_BSE && TPP_HAVE_UNICODE
#define TPP_HAVE_BSE_FILE_PARAM 1
#define tpp_bse_file__PARAM  , tpp_file const *tpp_restrict file
#define tpp_bse_file__ARG(x) , x
#else /* TPP_HAVE_BSE && TPP_HAVE_UNICODE */
#define TPP_HAVE_BSE_FILE_PARAM 0
#define tpp_bse_file__PARAM  /* nothing */
#define tpp_bse_file__ARG(x) /* nothing */
#endif /* !TPP_HAVE_BSE || !TPP_HAVE_UNICODE */


/* Check if "ch" may be the first byte of a \-character */
#if TPP_HAVE_TRIGRAPHS
#define _tpp_maybe_isbackslash(ch) ((ch) == '\\' || (ch) == '?') /* ?: because "??/" maps to "\" */
#else /* TPP_HAVE_TRIGRAPHS */
#define _tpp_maybe_isbackslash(ch) ((ch) == '\\')
#endif /* !TPP_HAVE_TRIGRAPHS */

/* Check if "ch" may be the first byte of a LF-style character */
#if TPP_HAVE_UNICODE
#define _tpp_maybe_islf(ch) tpp_ascii_islfornascii(ch)
#else /* TPP_HAVE_UNICODE */
#define _tpp_maybe_islf(ch) tpp_ascii_islf(ch)
#endif /* !TPP_HAVE_UNICODE */

/* Helper macros to skip over BSE when parsing already-loaded text.
 * tpp_skipbse_fwd: If "pos" points at a \-character, skip forward until end of BSE (if it is one)
 * tpp_skipbse_bck: If "pos" points after a line-feed character, skip backward until start of BSE (if it is one) */
#if TPP_HAVE_BSE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_skipbse_fwd_(tpp_char const *pos, tpp_char const *end tpp_bse_file__PARAM);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_char const *TPPCALL
tpp_skipbse_bck_(tpp_char const *pos, tpp_char const *start tpp_bse_file__PARAM);
#define tpp_skipbse_fwd(pos, end, file)   (((pos) >= (end) || !_tpp_maybe_isbackslash(*(pos))) ? (pos) : tpp_skipbse_fwd_(pos, end tpp_bse_file__ARG(file)))
#define tpp_skipbse_bck(pos, start, file) (((pos) <= (start) || !_tpp_maybe_islf((pos)[-1])) ? (pos) : tpp_skipbse_bck_(pos, start tpp_bse_file__ARG(file)))
#else /* TPP_HAVE_BSE */
#define tpp_skipbse_fwd(pos, end, file)   (pos)
#define tpp_skipbse_bck(pos, start, file) (pos)
#endif /* !TPP_HAVE_BSE */


#if TPP_HAVE_ESCAPED_KEYWORDS
/* Same as `tpp_hashof()', but skip over \-escaped linefeeds when calculating the hash */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof_esc_(tpp_char const *tpp_restrict kwd, tpp_size len tpp_bse_file__PARAM);
#define tpp_hashof_esc(kwd, len, file) tpp_hashof_esc_(kwd, len tpp_bse_file__ARG(file))

/* Copy `in_text...+=len' to `out_text', whilst removing \-escaped linefeeds
 * The caller must ensure that `out_text' has space for at least `len' bytes,
 * and the actual # of used bytes of `out_text' is returned. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_without_esc_(tpp_char *tpp_restrict out_text,
                 tpp_char const *tpp_restrict in_text,
                 tpp_size len tpp_bse_file__PARAM);
#define tpp_without_esc(out_text, in_text, len, file) \
	tpp_without_esc_(out_text, in_text, len tpp_bse_file__ARG(file))

/* Compare 2 strings, one of which may contain \-escaped linefeeds that must be skipped. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) int TPPCALL
tpp_memcmp_esc_(tpp_char const *lhs_without_esc, tpp_size lhs_len,
                tpp_char const *rhs_with_esc, tpp_size rhs_len
                tpp_bse_file__PARAM);
#define tpp_memcmp_esc(lhs_without_esc, lhs_len, rhs_with_esc, rhs_len, file) \
	tpp_memcmp_esc_(lhs_without_esc, lhs_len, rhs_with_esc, rhs_len tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Lookup one of the built-in, pre-defined keywords */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword(tpp_char const *tpp_restrict kwd,
                       tpp_size len, tpp_hash hash);
TPP_DECL TPP_WUNUSED tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_byid(enum tpp_token_id id);
#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_esc_(tpp_char const *tpp_restrict kwd,
                            tpp_size len, tpp_hash hash
                            tpp_bse_file__PARAM);
#define tpp_builtin_getkeyword_esc(kwd, len, hash, file) \
	tpp_builtin_getkeyword_esc_(kwd, len, hash tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */



/* Custom keywords table */
#if TPP_HAVE_USER_KEYWORDS
typedef struct tpp_keywords {
	unsigned int          TPP_INTERNAL(tks_kwdc); /* Amount of keyword entries stored. */
	tpp_hash              TPP_INTERNAL(tks_bckm); /* Allocated bucket mask. */
	TPP_REF tpp_keyword **TPP_INTERNAL(tks_bckv); /* [0..1][owned][0..tks_bckc+1][owned] Resizable keyword hash-map vector.
	                                               * NOTE: When the keyword map is destroyed, all linked keywords are, too.
	                                               *       Since this only happens when a lexer is finalized, this should
	                                               *       only happen once *all* keywords have their reference counters
	                                               *       set to "1". For this purpose, "tpp_keywords_fini" asserts that
	                                               *       no keyword has some other reference count value. */
} tpp_keywords;

TPP_DECL TPP_REF tpp_keyword *tpp_keywords_empty_map[1]; /* Consider this one TPP_INTERNAL */

/* Initialize/finalize a given keywords table. */
#define tpp_keywords_init(self)                \
	(void)((self)->TPP_INTERNAL(tks_kwdc) = 0, \
	       (self)->TPP_INTERNAL(tks_bckm) = 0, \
	       (self)->TPP_INTERNAL(tks_bckv) = tpp_keywords_empty_map)
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_fini(tpp_keywords *tpp_restrict self);
#if TPP_HAVE_LEXER_COPY
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_keywords_copy(tpp_keywords *tpp_restrict self,
                  tpp_keywords const *tpp_restrict from);
#endif /* TPP_HAVE_LEXER_COPY */

/* Lookup keywords within the given keywords-table **ONLY**
 * @return: * :   The keyword in question
 * @return: NULL: No such keyword (consider using "tpp_keywords_getkeyword" to
 *                also check for builtin keywords, or "tpp_keywords_newkeyword"
 *                to do the same, but lazily create missing keywords) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword(tpp_keywords const *tpp_restrict self,
                         tpp_char const *tpp_restrict kwd,
                         tpp_size len, tpp_hash hash);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword_byid(tpp_keywords const *tpp_restrict self,
                              enum tpp_token_id id);
#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
_tpp_keywords_getkeyword_esc_(tpp_keywords const *tpp_restrict self,
                              tpp_char const *tpp_restrict kwd,
                              tpp_size len, tpp_hash hash
                              tpp_bse_file__PARAM);
#define _tpp_keywords_getkeyword_esc(self, kwd, len, hash, file) \
	_tpp_keywords_getkeyword_esc_(self, kwd, len, hash tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Same as above, but also search the built-in keyword table (tpp_builtin_getkeyword) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword(tpp_keywords const *tpp_restrict self,
                        tpp_char const *tpp_restrict kwd,
                        tpp_size len, tpp_hash hash);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword_byid(tpp_keywords const *tpp_restrict self,
                             enum tpp_token_id id);
#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_getkeyword_esc_(tpp_keywords const *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM);
#define tpp_keywords_getkeyword_esc(self, kwd, len, hash, file) \
	tpp_keywords_getkeyword_esc_(self, kwd, len, hash tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Same as above, but if the keyword doesn't exist in `self' or the builtin
 * keyword table, a new keyword is allocated, given an ID, and inserted into `self'
 * @return: * :   The keyword associated with `kwd' (possibly having been just allocated)
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_newkeyword(tpp_keywords *tpp_restrict self,
                        tpp_char const *tpp_restrict kwd,
                        tpp_size len, tpp_hash hash);
#if TPP_HAVE_BSE
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword const *TPPCALL
tpp_keywords_newkeyword_esc_(tpp_keywords *tpp_restrict self,
                             tpp_char const *tpp_restrict kwd,
                             tpp_size len, tpp_hash hash
                             tpp_bse_file__PARAM);
#define tpp_keywords_newkeyword_esc(self, kwd, len, hash, file) \
	tpp_keywords_newkeyword_esc_(self, kwd, len, hash tpp_bse_file__ARG(file))
#endif /* TPP_HAVE_BSE */


#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
/* Check if "kwd" is contained in "self".
 * If so: do nothing and simply re-return "kwd"
 *
 * Otherwise, assume that "kwd" is a "builtin" keyword (as returned
 * by `tpp_builtin_getkeyword()'), in which the keyword is copied,
 * inserted into "self", and said copy is returned.
 *
 * This function must be used to make a keyword "writable" (which is
 * required before its `tk_macro' / `tk_misc' fields can safely be
 * written to (and in the later case: all fields of a potentially
 * pointed-to `tpp_keyword_misc', too)
 *
 * @return: * :   A writable copy of "kwd"
 * @return: NULL: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
tpp_keywords_copybuiltin(tpp_keywords *tpp_restrict self,
                         tpp_keyword const *tpp_restrict kwd);
#endif /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */
#endif /* TPP_HAVE_USER_KEYWORDS */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_KEYWORD_H */
