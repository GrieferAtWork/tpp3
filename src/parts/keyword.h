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
/*!depends config.h*/
#ifndef GUARD_TPP_KEYWORD_H
#define GUARD_TPP_KEYWORD_H 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "error.h"
#include "preparse.h"
#include "file-io.h"
#include "file.h"
#include "string.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_CPP_MACROS
#if TPP_HAVE_CPP_BUILTIN_MACROS || TPP_HAVE_CPP_PREDEFINED_MACROS
#define _TPP_KEYWORD_MACRO_BUILTIN      ((TPP_REF struct tpp_macro *)0) /* Use builtin/predefined definition */
#define _TPP_KEYWORD_MACRO_UNDEFINED    ((TPP_REF struct tpp_macro *)1) /* Macro is undefined */
#define _TPP_KEYWORD_MACRO_ISDEFINED(x) ((tpp_size)(x) > 1)
#else /* TPP_HAVE_CPP_BUILTIN_MACROS || TPP_HAVE_CPP_PREDEFINED_MACROS */
#define _TPP_KEYWORD_MACRO_UNDEFINED    ((TPP_REF struct tpp_macro *)0) /* Macro is undefined */
#define _TPP_KEYWORD_MACRO_ISDEFINED(x) ((x) != _TPP_KEYWORD_MACRO_UNDEFINED)
#endif /* !TPP_HAVE_CPP_BUILTIN_MACROS && !TPP_HAVE_CPP_PREDEFINED_MACROS */
#endif /* TPP_HAVE_CPP_MACROS */

#if TPP_HAVE_PRAGMA_PUSH_MACRO
struct tpp_macro;
typedef struct tpp_macro_pushent {
	TPP_REF struct tpp_macro *TPP_INTERNAL(tmpe_macro); /* [0..1] The macro that was pushed, or one of `_TPP_KEYWORD_MACRO_*`. */
	tpp_size                  TPP_INTERNAL(tmpe_count); /* # of times that `tmpe_macro` was pushed without the macro actually having changed */
} tpp_macro_pushent;

typedef struct tpp_macro_pushstack {
	tpp_size           TPP_INTERNAL(tmps_cnt); /* # of elements on `tmps_vec` */
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
 * @return: NULL: Out-of-memory (`TPP_ENOMEM`) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_macro_pushent *TPPCALL
tpp_macro_pushstack_append(tpp_macro_pushstack *tpp_restrict self);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */


#if TPP_HAVE_KEYWORD_MISC

/* Keyword flags... */
#if TPP_HAVE_KEYWORD_FLAGS
#define tpp_keyword_flags uint_least16_t /* Set of `TPP_KEYWORD_FLAG_*` */
#define TPP_KEYWORD_FLAG_NORMAL           UINT16_C(0x0000) /* Normal flags */
#if TPP_HAVE_CPP_PREDEFINED_MACROS
#define TPP_KEYWORD_FLAG_UNDEF_PREDEFINED UINT16_C(0x0001) /* Do not expand  */
#endif /* TPP_HAVE_CPP_PREDEFINED_MACROS */
#if TPP_HAVE_PRAGMA_DEPRECATED || TPP_HAVE_MACRO___is_deprecated
#define TPP_KEYWORD_FLAG_IS_DEPRECATED    UINT16_C(0x0002) /* Warn when the keyword appears as the result of lexical processing. */
#endif /* TPP_HAVE_PRAGMA_DEPRECATED || TPP_HAVE_MACRO___is_deprecated */
#if TPP_HAVE_PRAGMA_GCC_POISON || TPP_HAVE_MACRO___is_poisoned
#define TPP_KEYWORD_FLAG_IS_POISONED      UINT16_C(0x0004) /* Extension for `TPP_KEYWORD_FLAG_IS_DEPRECATED`:
                                                            * Don't emit a warning if the keyword is used inside of a macro.
                                                            * -> Only warn if it is used from a text file. */
#endif /* TPP_HAVE_PRAGMA_GCC_POISON || TPP_HAVE_MACRO___is_poisoned */
#if TPP_HAVE_CPP_IMPORT
#define TPP_KEYWORD_FLAG_HDR_IMPORTED     UINT16_C(0x0010) /* Set after this header was `#import`-ed */
#endif /* TPP_HAVE_CPP_IMPORT */
#if TPP_HAVE_PRAGMA_ONCE
#define TPP_KEYWORD_FLAG_HDR_ONCE         UINT16_C(0x0020) /* Set after `#pragma once` was encountered */
#endif /* TPP_HAVE_PRAGMA_ONCE */
#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
#define TPP_KEYWORD_FLAG_HDR_GUARD_VALID  UINT16_C(0x0040) /* The configured `tkm_file_guard` is valid and should be used
                                                            * (Set when the file is removed from the `#include`-stack with
                                                            * its `#ifdef`-stack empty, and `tkm_file_guard != NULL`) */
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#endif /* TPP_HAVE_KEYWORD_FLAGS */

struct tpp_keyword;

#if TPP_HAVE_KEYWORD_FEATURES
typedef struct tpp_keyword_feature {
	TPP_REF tpp_string *TPP_INTERNAL(tkf_expansion); /* [0..1] Expansion of relevant feature-test for this keyword
	                                                  *        When `NULL`, feature-test expands to the string `0` */
} tpp_keyword_feature;

#define _tpp_keyword_feature_init(self) \
	(void)((self)->TPP_INTERNAL(tkf_expansion) = NULL)
#define _tpp_keyword_feature_fini(self)                       \
	((self)->TPP_INTERNAL(tkf_expansion)                      \
	 ? tpp_string_decref((self)->TPP_INTERNAL(tkf_expansion)) \
	 : (void)0)
#define _tpp_keyword_feature_reset(self) \
	(_tpp_keyword_feature_fini(self),    \
	 _tpp_keyword_feature_init(self))

typedef struct tpp_keyword_features {
/*[[[deemon
import KEYWORD_FEATURE_KINDS from .config;
for (local kind: KEYWORD_FEATURE_KINDS) {
	local KIND = kind.upper();
	print("#if TPP_HAVE_KEYWORD_FEATURE_", KIND);
	print("	tpp_keyword_feature TPP_INTERNAL(tkfs_", kind, "); /" "* Expansion for `__", kind, "()` *" "/");
	print("#define _tpp_keyword_features_with_", kind, "(self, cb) cb(&(self)->TPP_INTERNAL(tkfs_", kind, "))");
	print("#else /" "* TPP_HAVE_KEYWORD_FEATURE_", KIND, " *" "/");
	print("#define _tpp_keyword_features_with_", kind, "(self, cb) /" "* nothing *" "/");
	print("#endif /" "* !TPP_HAVE_KEYWORD_FEATURE_", KIND, " *" "/");
	print;
}
]]]*/
#if TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE
	tpp_keyword_feature TPP_INTERNAL(tkfs_has_attribute); /* Expansion for `__has_attribute()` */
#define _tpp_keyword_features_with_has_attribute(self, cb) cb(&(self)->TPP_INTERNAL(tkfs_has_attribute))
#else /* TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE */
#define _tpp_keyword_features_with_has_attribute(self, cb) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE */

#if TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN
	tpp_keyword_feature TPP_INTERNAL(tkfs_has_builtin); /* Expansion for `__has_builtin()` */
#define _tpp_keyword_features_with_has_builtin(self, cb) cb(&(self)->TPP_INTERNAL(tkfs_has_builtin))
#else /* TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN */
#define _tpp_keyword_features_with_has_builtin(self, cb) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN */

#if TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE
	tpp_keyword_feature TPP_INTERNAL(tkfs_has_cpp_attribute); /* Expansion for `__has_cpp_attribute()` */
#define _tpp_keyword_features_with_has_cpp_attribute(self, cb) cb(&(self)->TPP_INTERNAL(tkfs_has_cpp_attribute))
#else /* TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE */
#define _tpp_keyword_features_with_has_cpp_attribute(self, cb) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE */

#if TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE
	tpp_keyword_feature TPP_INTERNAL(tkfs_has_declspec_attribute); /* Expansion for `__has_declspec_attribute()` */
#define _tpp_keyword_features_with_has_declspec_attribute(self, cb) cb(&(self)->TPP_INTERNAL(tkfs_has_declspec_attribute))
#else /* TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE */
#define _tpp_keyword_features_with_has_declspec_attribute(self, cb) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE */

#if TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION
	tpp_keyword_feature TPP_INTERNAL(tkfs_has_extension); /* Expansion for `__has_extension()` */
#define _tpp_keyword_features_with_has_extension(self, cb) cb(&(self)->TPP_INTERNAL(tkfs_has_extension))
#else /* TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION */
#define _tpp_keyword_features_with_has_extension(self, cb) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION */

#if TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE
	tpp_keyword_feature TPP_INTERNAL(tkfs_has_feature); /* Expansion for `__has_feature()` */
#define _tpp_keyword_features_with_has_feature(self, cb) cb(&(self)->TPP_INTERNAL(tkfs_has_feature))
#else /* TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE */
#define _tpp_keyword_features_with_has_feature(self, cb) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE */

#if TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE
	tpp_keyword_feature TPP_INTERNAL(tkfs_has_c_attribute); /* Expansion for `__has_c_attribute()` */
#define _tpp_keyword_features_with_has_c_attribute(self, cb) cb(&(self)->TPP_INTERNAL(tkfs_has_c_attribute))
#else /* TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE */
#define _tpp_keyword_features_with_has_c_attribute(self, cb) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE */
/*[[[end]]]*/
} tpp_keyword_features;

/*[[[deemon
import KEYWORD_FEATURE_KINDS from .config;
local maxLen = KEYWORD_FEATURE_KINDS.each.length > ...;
print("#define _tpp_keyword_features_witheach(self, cb) ", " " * (maxLen - 5), "\\\n", " \\\n".join(
	for (local kind: KEYWORD_FEATURE_KINDS)
		f"	, _tpp_keyword_features_with_{kind}(self, cb){
			" " * (maxLen - #kind)
		}"
));
]]]*/
#define _tpp_keyword_features_witheach(self, cb)                  \
	, _tpp_keyword_features_with_has_attribute(self, cb)          \
	, _tpp_keyword_features_with_has_builtin(self, cb)            \
	, _tpp_keyword_features_with_has_cpp_attribute(self, cb)      \
	, _tpp_keyword_features_with_has_declspec_attribute(self, cb) \
	, _tpp_keyword_features_with_has_extension(self, cb)          \
	, _tpp_keyword_features_with_has_feature(self, cb)            \
	, _tpp_keyword_features_with_has_c_attribute(self, cb)
/*[[[end]]]*/

#define _tpp_keyword_features_init(self) \
	((void)0 _tpp_keyword_features_witheach(self, _tpp_keyword_feature_init))
#define _tpp_keyword_features_fini(self) \
	((void)0 _tpp_keyword_features_witheach(self, _tpp_keyword_feature_fini))
#define _tpp_keyword_features_reset(self) \
	((void)0 _tpp_keyword_features_witheach(self, _tpp_keyword_feature_fini) \
	         _tpp_keyword_features_witheach(self, _tpp_keyword_feature_init))
#endif /* TPP_HAVE_KEYWORD_FEATURES */


#if TPP_HAVE_CPP_ASSERT
typedef struct tpp_assertion {
	struct tpp_keyword const *TPP_INTERNAL(tas_value); /* [0..1] Assertion value, or `NULL` if entry is unused */
} tpp_assertion;

typedef struct tpp_assertions {
	tpp_size       TPP_INTERNAL(tass_assc); /* Amount of assertions made. */
	tpp_hash       TPP_INTERNAL(tass_bckm); /* Allocated bucket mask. */
	tpp_assertion *TPP_INTERNAL(tass_bckv); /* [0..tass_bckm+1][owned] Hash-map of assertions */
} tpp_assertions;

#define tpp_assertions_init(self)               \
	(void)((self)->TPP_INTERNAL(tass_assc) = 0, \
	       (self)->TPP_INTERNAL(tass_bckm) = 0, \
	       (self)->TPP_INTERNAL(tass_bckv) = NULL)
#define tpp_assertions_fini(self)               \
	(tpp_free((self)->TPP_INTERNAL(tass_bckv)), \
	 tpp_dbg_memset(self, sizeof(tpp_assertions)))

/* Return the # of assertions made */
#define tpp_assertions_getcount(self) \
	((self)->TPP_INTERNAL(tass_assc))

/* Check if *any* keywords have been asserted within the given assertion-set `self` */
#define tpp_assertions_containsany(self) \
	((self)->TPP_INTERNAL(tass_assc) != 0)

/* Delete *all* assertions made within `self` */
#define tpp_assertions_unassertall(self) \
	(void)(tpp_assertions_fini(self),    \
	       tpp_assertions_init(self))

#if TPP_HAVE_LEXER_COPY
/* Copy the given set of assertions
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_assertions_copy(tpp_assertions *tpp_restrict self,
                    tpp_assertions const *tpp_restrict from);
#endif /* TPP_HAVE_LEXER_COPY */

/* Check if a given `value` is being asserted by `self` */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) bool TPPCALL
tpp_assertions_contains(tpp_assertions const *tpp_restrict self,
                        struct tpp_keyword const *tpp_restrict value);

/* Assert a given `value` within `self`.
 * @return: TPP_EOK:    Assertion was added
 * @return: TPP_ENOENT: Assertion was already added before (SOFT_ERROR)
 * @return: TPP_ENOMEM: Out of memory (HARD_ERROR) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_assertions_assert(tpp_assertions *tpp_restrict self,
                      struct tpp_keyword const *tpp_restrict value);

/* Unassert a given `value` within `self`.
 * @return: true:  Assertion was removed
 * @return: false: Assertion didn't exist in the first place */
TPP_DECL TPP_NONNULL((1, 2)) bool TPPCALL
tpp_assertions_unassert(tpp_assertions *tpp_restrict self,
                        struct tpp_keyword const *tpp_restrict value);
#endif /* TPP_HAVE_CPP_ASSERT */


typedef struct tpp_keyword_misc {
#if TPP_HAVE_KEYWORD_FLAGS
	tpp_keyword_flags TPP_INTERNAL(tkm_flags); /* Set of `TPP_KEYWORD_FLAG_*` */
#define _tpp_keyword_misc_init_flags(self) , (self)->TPP_INTERNAL(tkm_flags) = TPP_KEYWORD_FLAG_NORMAL
#else /* TPP_HAVE_KEYWORD_FLAGS */
#define _tpp_keyword_misc_init_flags(self) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_FLAGS */

#if TPP_HAVE_KEYWORD_FEATURES
	tpp_keyword_features TPP_INTERNAL(tkm_features); /* Expansions of `__has_*` feature-test macros */
#define _tpp_keyword_misc_init_features(self) , _tpp_keyword_features_init(&(self)->TPP_INTERNAL(tkm_features))
#define _tpp_keyword_misc_featurebykind(self, kind) \
	((tpp_keyword_feature *)((char *)(self) + (tpp_size)(unsigned int)(kind)))
#else /* TPP_HAVE_KEYWORD_FEATURES */
#define _tpp_keyword_misc_init_features(self) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_FEATURES */

#if TPP_HAVE_CPP_ASSERT
	tpp_assertions TPP_INTERNAL(tkm_assertions); /* Assertions made for the associated keyword */
#define _tpp_keyword_misc_init_assertions(self) , tpp_assertions_init(&(self)->TPP_INTERNAL(tkm_assertions))
#else /* TPP_HAVE_CPP_ASSERT */
#define _tpp_keyword_misc_init_assertions(self) /* nothing */
#endif /* !TPP_HAVE_CPP_ASSERT */

#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
	struct tpp_keyword const *TPP_INTERNAL(tkm_file_guard); /* [0..1] Name of the #include guard for this file, or NULL if unknown. */
#define _tpp_keyword_misc_init_file_guard(self) , (self)->TPP_INTERNAL(tkm_file_guard) = NULL
#else /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */
#define _tpp_keyword_misc_init_file_guard(self) /* nothing */
#endif /* !TPP_HAVE_IFNDEF_INCLUDE_GUARDS */

#if TPP_HAVE_KEYWORD_INCLCOUNT
	tpp_size TPP_INTERNAL(tkm_file_inclcount); /* # of times that this file exists on the `#include`-stack (or `TPP_SIZE_MAX` if unknown) */
#define _tpp_keyword_misc_init_file_inclcount(self) , (self)->TPP_INTERNAL(tkm_file_inclcount) = TPP_SIZE_MAX
#else /* TPP_HAVE_KEYWORD_INCLCOUNT */
#define _tpp_keyword_misc_init_file_inclcount(self) /* nothing */
#endif /* !TPP_HAVE_KEYWORD_INCLCOUNT */

#if TPP_HAVE_PRAGMA_PUSH_MACRO
	tpp_macro_pushstack TPP_INTERNAL(tkm_macro_pushstack); /* For `#pragma push_macro()` */
#define _tpp_keyword_misc_init_macro_pushstack(self) , tpp_macro_pushstack_init(&(self)->TPP_INTERNAL(tkm_macro_pushstack))
#else /* TPP_HAVE_PRAGMA_PUSH_MACRO */
#define _tpp_keyword_misc_init_macro_pushstack(self) /* nothing */
#endif /* !TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_MACRO___TPP_COUNTER
	tpp_counter TPP_INTERNAL(tkm_builtin_counter); /* Next value for `__TPP_COUNTER()` */
#define _tpp_keyword_misc_init_builtin_counter(self) , (self)->TPP_INTERNAL(tkm_builtin_counter) = 0
#else /* TPP_HAVE_MACRO___TPP_COUNTER */
#define _tpp_keyword_misc_init_builtin_counter(self) /* nothing */
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */

#if TPP_HAVE_KEYWORD_USERDATA
	void          *TPP_INTERNAL(tkm_userdata_ptr); /* [?..?] User-data pointer (initialize to `NULL`) */
	void (TPPCALL *TPP_INTERNAL(tkm_userdata_dtor))(void *ptr); /* [0..1] Optional finalizer for user-data */
#define _tpp_keyword_misc_init_userdata(self)   , (self)->TPP_INTERNAL(tkm_userdata_ptr) = NULL, (self)->TPP_INTERNAL(tkm_userdata_dtor) = NULL
#else /* TPP_HAVE_KEYWORD_USERDATA */
#define _tpp_keyword_misc_init_userdata(self)   /* nothing */
#endif /* !TPP_HAVE_KEYWORD_USERDATA */
} tpp_keyword_misc;

#define _tpp_keyword_misc_init(self)                    \
	(void)((void)0 _tpp_keyword_misc_init_flags(self)   \
	       _tpp_keyword_misc_init_features(self)        \
	       _tpp_keyword_misc_init_assertions(self)      \
	       _tpp_keyword_misc_init_file_guard(self)      \
	       _tpp_keyword_misc_init_file_inclcount(self)  \
	       _tpp_keyword_misc_init_macro_pushstack(self) \
	       _tpp_keyword_misc_init_builtin_counter(self) \
	       _tpp_keyword_misc_init_userdata(self))
#define _tpp_keyword_misc_alloc()    ((tpp_keyword_misc *)tpp_malloc(sizeof(tpp_keyword_misc)))
#define _tpp_keyword_misc_tryalloc() ((tpp_keyword_misc *)tpp_trymalloc(sizeof(tpp_keyword_misc)))
#define _tpp_keyword_misc_free(p)    tpp_free(p)
#endif /* TPP_HAVE_KEYWORD_MISC */


#if TPP_HAVE_CPP_MACROS
struct tpp_macro;
#endif /* TPP_HAVE_CPP_MACROS */

#undef TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
typedef struct tpp_keyword {
	tpp_token_id              TPP_INTERNAL(tk_id);                  /* [const] Keyword ID */
#if TPP_HAVE_CPP_MACROS
	TPP_REF struct tpp_macro *TPP_INTERNAL(tk_macro);               /* [0..1][const_if(IS_BUILTIN)] Macro definition or one of `_TPP_KEYWORD_MACRO_*` */
#define TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS 1
#endif /* TPP_HAVE_CPP_MACROS */
#if TPP_HAVE_KEYWORD_MISC
	tpp_keyword_misc         *TPP_INTERNAL(tk_misc);                /* [0..1][const_if(IS_BUILTIN)][owned] Misc. keyword data (lazily allocated) */
#define TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS 1
#endif /* TPP_HAVE_KEYWORD_MISC */
	tpp_hash                  TPP_INTERNAL(tk_hash);                /* [const] Hash for `tk_kwd` */
	struct tpp_keyword       *TPP_INTERNAL(tk_next);                /* [0..1] Next keyword with a similar hash */
#if TPP_HAVE_KEYWORD_ASSTRING
	tpp_refcnt_atomic         TPP_INTERNAL(tk_refcnt);              /* Keyword reference count (for binary compatibility with `tpp_string`) */
#endif /* TPP_HAVE_KEYWORD_ASSTRING */
	tpp_size                  TPP_INTERNAL(tk_len);                 /* [const] # of bytes (char-s) in `tk_kwd` (excluding trailing `\0`) */
	tpp_char                  TPP_INTERNAL(tk_kwd)[TPP_FLEX_ARRAY]; /* [const][tk_len] Keyword string (in input encoding; `\0`-terminated; never contains `\`-escaped linefeeds) */
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
 * as any other *misc*-related, custom data being assigned to keywords */
#ifndef TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
#define TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS 0
#endif /* !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */

/* Use this macro for comparing keywords instead of doing `a == b`
 * We need to compare the IDs of keywords, since builtin keywords
 * may need to be copied into the current lexer's `tpp_keywords`
 * if `tk_macro` or `tk_misc` need to be modified */
#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
#define tpp_keyword_equals(a, b) ((a)->TPP_INTERNAL(tk_id) == (b)->TPP_INTERNAL(tk_id))
#else /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */
#define tpp_keyword_equals(a, b) ((a) == (b))
#endif /* !TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */

/* Public API for accessing `tpp_keyword` internals */
#if TPP_HAVE_USER_KEYWORDS
#define tpp_keyword_isuser(self)    TPP_TOK_ISUSERKEYWORD((self)->TPP_INTERNAL(tk_id))
#define tpp_keyword_isbuiltin(self) (!tpp_keyword_isuser(self))
#else /* TPP_HAVE_USER_KEYWORDS */
#define tpp_keyword_isuser(self)    0
#define tpp_keyword_isbuiltin(self) 1
#endif /* !TPP_HAVE_USER_KEYWORDS */
#define tpp_keyword_getid(self)     ((self)->TPP_INTERNAL(tk_id))
#define tpp_keyword_getstr(self)    ((self)->TPP_INTERNAL(tk_kwd))
#define tpp_keyword_getcstr(self)   ((char const *)(self)->TPP_INTERNAL(tk_kwd))
#define tpp_keyword_getlen(self)    ((self)->TPP_INTERNAL(tk_len))
#define tpp_keyword_gethash(self)   ((self)->TPP_INTERNAL(tk_hash))
#if TPP_HAVE_CPP_MACROS
#define tpp_keyword_getmacro(self)  ((self)->TPP_INTERNAL(tk_macro))
#define tpp_keyword_hasmacro(self)  _TPP_KEYWORD_MACRO_ISDEFINED((self)->TPP_INTERNAL(tk_macro))
#endif /* TPP_HAVE_CPP_MACROS */

/* Check if `self` matches the C, constant string literal `CONSTstr` */
#define tpp_keyword_equals_conststr(self, CONSTstr)                       \
	(tpp_keyword_getlen(self) == (sizeof(CONSTstr) / sizeof(char)) - 1 && \
	 tpp_memcmp(tpp_keyword_getstr(self), CONSTstr, sizeof(CONSTstr) - sizeof(char)) == 0)

/* Check if `self` matches `str...+=len` */
#define tpp_keyword_equals_str(self, /*tpp_char const **/ str, len) \
	(tpp_keyword_getlen(self) == (len) &&                           \
	 tpp_memcmp(tpp_keyword_getstr(self), str, (len) * sizeof(char)) == 0)

/* Check if `self` matches `cstr...+=len` */
#define tpp_keyword_equals_cstr(self, /*char const **/ cstr, len) \
	tpp_keyword_equals_str(self, (tpp_char const *)(cstr), len)


/* Convert back-and-forth between keywords and strings */
#if TPP_HAVE_KEYWORD_ASSTRING
#define _TPP_KEYWORD_STRING_ABI_START TPP_INTERNAL(tk_refcnt)
#define tpp_keyword_asstring(self) ((tpp_string *)&(self)->_TPP_KEYWORD_STRING_ABI_START)
#define tpp_string_askeyword(self) ((tpp_keyword *)((char *)(self) - tpp_offsetof(tpp_keyword, _TPP_KEYWORD_STRING_ABI_START)))
#endif /* TPP_HAVE_KEYWORD_ASSTRING */

#if TPP_HAVE_KEYWORD_USERDATA
/* Get the user-data pointer for `self`
 * @return: NULL: No pointer set, or set pointer is `NULL` */
#define tpp_keyword_getuserdata(self)                                \
	((self)->TPP_INTERNAL(tk_misc)                                   \
	 ? (self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_userdata_ptr) \
	 : NULL)

/* Returns the linked destructor (which you can also think of as a
 * sort-of type-pointer describing what `tpp_keyword_getuserdata(self)`
 * actually means) */
#define tpp_keyword_getuserdata_dtor(self)                            \
	((self)->TPP_INTERNAL(tk_misc)                                    \
	 ? (self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_userdata_dtor) \
	 : NULL)

/* Set the user-data pointer for `self`
 * @param: destroy_prev: When true, and `tpp_keyword_getuserdata_dtor(self) != NULL`,
 *                       as well as `tpp_keyword_getuserdata(self) != NULL` on entry,
 *                       invoke the existing destructor on the old user-data after
 *                       assigning the new `ptr` and `dtor`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_setuserdata(tpp_keyword *tpp_restrict self,
                        void *ptr, void (TPPCALL *dtor)(void *ptr),
                        bool destroy_prev);
#endif /* TPP_HAVE_KEYWORD_USERDATA */

#if TPP_HAVE_PRAGMA_PUSH_MACRO
/* Push the current macro-definition of `self`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory (TPP_ENOMEM) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_pushmacro(tpp_keyword *tpp_restrict self);

/* Pop the current macro-definition of `self`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOENT: Macro-push-stack was already empty (soft-error) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_popmacro(tpp_keyword *tpp_restrict self);
#endif /* TPP_HAVE_PRAGMA_PUSH_MACRO */

#if TPP_HAVE_CPP_MACROS
/* Check if `self` can be #undef'd such that it changes state after that action
 * If `self` has a builtin/predefined macro definition, that definition will
 * no longer be expanded. */
#define tpp_keyword_canundef(self) \
	(tpp_keyword_getmacro(self) != _TPP_KEYWORD_MACRO_UNDEFINED)

/* Similar to `tpp_keyword_canundef()`, but only check if there is a user-defined
 * definition that can be deleted. Returns `false` if the keyword is currently
 * configured such that a (potential) builtin/predefined macro is expanded. */
#define tpp_keyword_canundefuser(self) \
	_TPP_KEYWORD_MACRO_ISDEFINED(tpp_keyword_getmacro(self))

/* Delete the macro definition of `self` (including any builtin/predefined definition) */
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keyword_undef(tpp_keyword *tpp_restrict self);

/* Similar to `tpp_keyword_undef()`, but only delete user-defined macro expansions,
 * and -- if there might be a builtin/predefined macro related to `self` -- that
 * macro is re-enabled. */
#if TPP_HAVE_CPP_BUILTIN_MACROS
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keyword_undefuser(tpp_keyword *tpp_restrict self);
#else /* TPP_HAVE_CPP_BUILTIN_MACROS */
#define tpp_keyword_undefuser(self) tpp_keyword_undef(self)
#endif /* !TPP_HAVE_CPP_BUILTIN_MACROS */

#else /* TPP_HAVE_CPP_MACROS */
#define tpp_keyword_canundef(self)     0
#define tpp_keyword_canundefuser(self) 0
#endif /* !TPP_HAVE_CPP_MACROS */


#if TPP_HAVE_IFNDEF_INCLUDE_GUARDS
/* Return the keyword registered as #ifndef-guard of
 * the given (should-be) filename-keyword `self`
 *
 * If no keyword has been registered for this purpose,
 * return `NULL` instead. */
#define tpp_keyword_get_file_guard(self)                           \
	((self)->TPP_INTERNAL(tk_misc)                                 \
	 ? (self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_file_guard) \
	 : NULL)

/* Set the keyword registered as #ifndef-guard of
 * the given (should-be) filename-keyword `self`
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_set_file_guard(tpp_keyword *self, tpp_keyword const *guard);
#endif /* TPP_HAVE_IFNDEF_INCLUDE_GUARDS */



#if TPP_HAVE_MACRO___TPP_COUNTER
/* Return the next value that will be returned by __TPP_COUNTER() for this keyword */
#define tpp_keyword_get_builtin_counter(self)                           \
	((self)->TPP_INTERNAL(tk_misc)                                      \
	 ? (self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_builtin_counter) \
	 : 0)

/* Reset the counter such that `tpp_keyword_get_builtin_counter()` returns `0` */
#define tpp_keyword_reset_builtin_counter(self)                                     \
	((self)->TPP_INTERNAL(tk_misc)                                                  \
	 ? (void)((self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_builtin_counter) = 0) \
	 : (void)0)

/* Fetch+increment the __TPP_COUNTER() value of this keyword
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_keyword_inc_builtin_counter(tpp_keyword *tpp_restrict self,
                                tpp_counter *tpp_restrict p_result);
#else /* TPP_HAVE_MACRO___TPP_COUNTER */
#define tpp_keyword_get_builtin_counter(self)   0
#define tpp_keyword_reset_builtin_counter(self) (void)0
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */



#if TPP_HAVE_KEYWORD_FLAGS
/* Return the flags (set of `TPP_KEYWORD_FLAG_*`) linked to `self` */
#define tpp_keyword_getflags(self)                            \
	((self)->TPP_INTERNAL(tk_misc)                            \
	 ? (self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_flags) \
	 : TPP_KEYWORD_FLAG_NORMAL)

/* Set the flags (set of `TPP_KEYWORD_FLAG_*`) linked to `self`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_setflags(tpp_keyword *tpp_restrict self,
                     tpp_keyword_flags flags);
#endif /* TPP_HAVE_KEYWORD_FLAGS */



#if TPP_HAVE_KEYWORD_FEATURES
typedef enum tpp_keyword_feature_kind {
/*[[[deemon
import KEYWORD_FEATURE_KINDS from .config;
for (local kind: KEYWORD_FEATURE_KINDS) {
	local KIND = kind.upper();
	print("#if TPP_HAVE_KEYWORD_FEATURE_", KIND);
	print("	TPP_KEYWORD_FEATURE_KIND_", KIND, " = tpp_offsetof(tpp_keyword_misc, TPP_INTERNAL(tkm_features).TPP_INTERNAL(tkfs_", kind, ")), /" "* Expansion for `__has_", kind, "()` *" "/");
	print("#endif /" "* TPP_HAVE_KEYWORD_FEATURE_", KIND, " *" "/");
}
]]]*/
#if TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE
	TPP_KEYWORD_FEATURE_KIND_HAS_ATTRIBUTE = tpp_offsetof(tpp_keyword_misc, TPP_INTERNAL(tkm_features).TPP_INTERNAL(tkfs_has_attribute)), /* Expansion for `__has_has_attribute()` */
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN
	TPP_KEYWORD_FEATURE_KIND_HAS_BUILTIN = tpp_offsetof(tpp_keyword_misc, TPP_INTERNAL(tkm_features).TPP_INTERNAL(tkfs_has_builtin)), /* Expansion for `__has_has_builtin()` */
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_BUILTIN */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE
	TPP_KEYWORD_FEATURE_KIND_HAS_CPP_ATTRIBUTE = tpp_offsetof(tpp_keyword_misc, TPP_INTERNAL(tkm_features).TPP_INTERNAL(tkfs_has_cpp_attribute)), /* Expansion for `__has_has_cpp_attribute()` */
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_CPP_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE
	TPP_KEYWORD_FEATURE_KIND_HAS_DECLSPEC_ATTRIBUTE = tpp_offsetof(tpp_keyword_misc, TPP_INTERNAL(tkm_features).TPP_INTERNAL(tkfs_has_declspec_attribute)), /* Expansion for `__has_has_declspec_attribute()` */
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_DECLSPEC_ATTRIBUTE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION
	TPP_KEYWORD_FEATURE_KIND_HAS_EXTENSION = tpp_offsetof(tpp_keyword_misc, TPP_INTERNAL(tkm_features).TPP_INTERNAL(tkfs_has_extension)), /* Expansion for `__has_has_extension()` */
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_EXTENSION */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE
	TPP_KEYWORD_FEATURE_KIND_HAS_FEATURE = tpp_offsetof(tpp_keyword_misc, TPP_INTERNAL(tkm_features).TPP_INTERNAL(tkfs_has_feature)), /* Expansion for `__has_has_feature()` */
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_FEATURE */
#if TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE
	TPP_KEYWORD_FEATURE_KIND_HAS_C_ATTRIBUTE = tpp_offsetof(tpp_keyword_misc, TPP_INTERNAL(tkm_features).TPP_INTERNAL(tkfs_has_c_attribute)), /* Expansion for `__has_has_c_attribute()` */
#endif /* TPP_HAVE_KEYWORD_FEATURE_HAS_C_ATTRIBUTE */
/*[[[end]]]*/
} tpp_keyword_feature_kind;

#define tpp_keyword_resetfeature(self, kind)                                                                  \
	((self)->TPP_INTERNAL(tk_misc)                                                                            \
	 ? (void)_tpp_keyword_feature_reset(_tpp_keyword_misc_featurebykind((self)->TPP_INTERNAL(tk_misc), kind)) \
	 : (void)0)
#define tpp_keyword_resetfeatures(self)                                                              \
	((self)->TPP_INTERNAL(tk_misc)                                                                   \
	 ? (void)_tpp_keyword_features_reset(&(self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_features)) \
	 : (void)0)

/* Return the string-expansion of the feature-check `kind` when
 * given `self` as an argument. Returns `NULL` when no custom
 * expansion has been defined (caller must still check for pre-
 * defined/builtin feature-check expansions)
 *
 * You should probably call `tpp_lexer_getkeywordfeature()`
 * instead of this function, since this one doesn't handle
 * builtin expansions!
 *
 * @return: * :   The custom override for what `self` should expand to
 * @return: NULL: No custom override present. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) TPP_REF tpp_string *TPPCALL
tpp_keyword_getfeature(tpp_keyword const *tpp_restrict self,
                       tpp_keyword_feature_kind kind);

/* Set the text that a feature-check of `kind` expands to when used
 * with the given keyword `self`. You may also pass `NULL` for `value`
 * to reset that specific feature back to its builtin (or `0`) state.
 *
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
tpp_keyword_setfeature(tpp_keyword *tpp_restrict self,
                       tpp_keyword_feature_kind kind,
                       tpp_string *value);
#endif /* TPP_HAVE_KEYWORD_FEATURES */



#if TPP_HAVE_CPP_ASSERT
/* Return the # of assertions made */
#define tpp_keyword_getassertcount(self) \
	((self)->TPP_INTERNAL(tk_misc) ? tpp_assertions_getcount((self)->TPP_INTERNAL(tk_misc)) : 0)

/* Check if *any* keywords have been asserted within the given assertion-set `self` */
#define tpp_keyword_containsanyassert(self) \
	((self)->TPP_INTERNAL(tk_misc) && tpp_assertions_containsany(&(self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_assertions)))

/* Delete *all* assertions made within `self` */
#define tpp_keyword_unassertall(self)                                                           \
	((self)->TPP_INTERNAL(tk_misc)                                                              \
	 ? tpp_assertions_unassertall(&(self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_assertions)) \
	 : (void)0)

/* Check if a given `value` is being asserted by `self` */
#define tpp_keyword_containsassert(self, value) \
	((self)->TPP_INTERNAL(tk_misc) && tpp_assertions_contains(&(self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_assertions), value))

/* Assert a given `value` within `self`.
 * @return: TPP_EOK:    Assertion was added
 * @return: TPP_ENOENT: Assertion was already added before (SOFT_ERROR)
 * @return: TPP_ENOMEM: Out of memory (HARD_ERROR) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_errno TPPCALL
tpp_keyword_addassert(tpp_keyword *self, tpp_keyword const *value);

/* Unassert a given `value` within `self`.
 * @return: true:  Assertion was removed
 * @return: false: Assertion didn't exist in the first place */
#define tpp_keyword_unassert(self, value) \
	((self)->TPP_INTERNAL(tk_misc) && tpp_assertions_unassert(&(self)->TPP_INTERNAL(tk_misc)->TPP_INTERNAL(tkm_assertions), value))
#else /* TPP_HAVE_CPP_ASSERT */
#define tpp_keyword_getassertcount(self)        0
#define tpp_keyword_containsanyassert(self)     false
#define tpp_keyword_unassertall(self)           (void)0
#define tpp_keyword_containsassert(self, value) false
#define tpp_keyword_unassert(self, value)       false
#endif /* !TPP_HAVE_CPP_ASSERT */



/* Calculate the hash of a given keyword string */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof(tpp_char const *tpp_restrict kwd, tpp_size len);

/* Initial hash value */
#define TPP_HASH_INITIAL TPP_HASH_C(1)

/* >> tpp_hash tpp_hash_combine_char(tpp_hash a, tpp_char b);
 * Combine a given hash value `a` with a character `b`. */
#define tpp_hash_combine_char(a, b) ((a) * 263 + /*(tpp_char)*/(b))

/* >> tpp_hash tpp_hash_combine_hash(tpp_hash a, tpp_hash b);
 * Combine a given hash value `a` with another hash value `b`. */
#define tpp_hash_combine_hash(a, b) ((a) * 263 + /*(tpp_hash)*/(b))


#if (_TPP_HAVE_BSE_FILE_PARAM ||                                                       \
     (TPP_HAVE_IDENTIFIER_ESCAPE_NAMED && (TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM || \
                                           TPP_CONF_ISRT(TPP_HAVE_TRIGRAPHS))))
struct tpp_lexer;
#define _tpp_esc_lexer__PARAM  , struct tpp_lexer const *tpp_restrict lexer
#define _tpp_esc_lexer__ARG(x) , x
#else /* ... */
#define _tpp_esc_lexer__PARAM  /* nothing */
#define _tpp_esc_lexer__ARG(x) /* nothing */
#endif /* !... */


#if TPP_HAVE_ESCAPED_KEYWORDS
/* Same as `tpp_hashof()`, but skip over `\`-escaped linefeeds when calculating the hash */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_hash TPPCALL
tpp_hashof_esc_(tpp_char const *tpp_restrict kwd, tpp_size len _tpp_esc_lexer__PARAM);
#define tpp_hashof_esc(kwd, len, lexer) tpp_hashof_esc_(kwd, len _tpp_esc_lexer__ARG(lexer))

/* Copy `in_text...+=len` to `out_text`, whilst removing `\`-escaped linefeeds
 * The caller must ensure that `out_text` has space for at least `len` bytes,
 * and the actual # of used bytes of `out_text` is returned. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_size TPPCALL
tpp_without_esc_(tpp_char *tpp_restrict out_text,
                 tpp_char const *tpp_restrict in_text,
                 tpp_size len _tpp_esc_lexer__PARAM);
#define tpp_without_esc(out_text, in_text, len, lexer) \
	tpp_without_esc_(out_text, in_text, len _tpp_esc_lexer__ARG(lexer))

/* Compare 2 strings, one of which may contain `\`-escaped linefeeds that must be skipped. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 3)) int TPPCALL
tpp_memcmp_esc_(tpp_char const *lhs_without_esc, tpp_size lhs_len,
                tpp_char const *rhs_with_esc, tpp_size rhs_len
                _tpp_esc_lexer__PARAM);
#define tpp_memcmp_esc(lhs_without_esc, lhs_len, rhs_with_esc, rhs_len, lexer) \
	tpp_memcmp_esc_(lhs_without_esc, lhs_len, rhs_with_esc, rhs_len _tpp_esc_lexer__ARG(lexer))
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Lookup one of the built-in, predefined keywords */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword(tpp_char const *tpp_restrict kwd,
                       tpp_size len, tpp_hash hash);
TPP_DECL TPP_WUNUSED tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_byid(enum tpp_token_id id);
#if TPP_HAVE_ESCAPED_KEYWORDS
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_keyword const *TPPCALL
tpp_builtin_getkeyword_esc_(tpp_char const *tpp_restrict kwd,
                            tpp_size len, tpp_hash hash
                            _tpp_esc_lexer__PARAM);
#define tpp_builtin_getkeyword_esc(kwd, len, hash, lexer) \
	tpp_builtin_getkeyword_esc_(kwd, len, hash _tpp_esc_lexer__ARG(lexer))
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
	                                               *       set to `1`. For this purpose, `tpp_keywords_fini()` asserts that
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

/* Reset (re-initialize) `self` */
#define tpp_keywords_reset(self) \
	(tpp_keywords_fini(self), tpp_keywords_init(self))


/* Lookup keywords within the given keywords-table **ONLY**
 * @return: * :   The keyword in question
 * @return: NULL: No such keyword (consider using `tpp_keywords_getkeyword()` to
 *                also check for builtin keywords, or `tpp_keywords_newkeyword()`
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
                              _tpp_esc_lexer__PARAM);
#define _tpp_keywords_getkeyword_esc(self, kwd, len, hash, lexer) \
	_tpp_keywords_getkeyword_esc_(self, kwd, len, hash _tpp_esc_lexer__ARG(lexer))
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Same as above, but also search the built-in keyword table (`tpp_builtin_getkeyword()`) */
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
                             _tpp_esc_lexer__PARAM);
#define tpp_keywords_getkeyword_esc(self, kwd, len, hash, lexer) \
	tpp_keywords_getkeyword_esc_(self, kwd, len, hash _tpp_esc_lexer__ARG(lexer))
#endif /* TPP_HAVE_ESCAPED_KEYWORDS */


/* Same as above, but if the keyword doesn't exist in `self` or the builtin
 * keyword table, a new keyword is allocated, given an ID, and inserted into `self`
 * @return: * :   The keyword associated with `kwd` (possibly having been just allocated)
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
                             _tpp_esc_lexer__PARAM);
#define tpp_keywords_newkeyword_esc(self, kwd, len, hash, lexer) \
	tpp_keywords_newkeyword_esc_(self, kwd, len, hash _tpp_esc_lexer__ARG(lexer))
#endif /* TPP_HAVE_BSE */


#if TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS
/* Check if `kwd` is contained in `self`.
 * If so: do nothing and simply re-return `kwd`
 *
 * Otherwise, assume that `kwd` is a *builtin* keyword (as returned
 * by `tpp_builtin_getkeyword()`), in which the keyword is copied,
 * inserted into `self`, and said copy is returned.
 *
 * This function must be used to make a keyword *writable* (which is
 * required before its `tk_macro` / `tk_misc` fields can safely be
 * written to (and in the later case: all fields of a potentially
 * pointed-to `tpp_keyword_misc`, too)
 *
 * @return: * :   A writable copy of `kwd`
 * @return: NULL: Out of memory (`TPP_ENOMEM`) */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2)) tpp_keyword *TPPCALL
tpp_keywords_copybuiltin(tpp_keywords *tpp_restrict self,
                         tpp_keyword const *tpp_restrict kwd);
#endif /* TPP_HAVE_COPYABLE_BUILTIN_KEYWORDS */
#endif /* TPP_HAVE_USER_KEYWORDS */


#if TPP_HAVE_KEYWORDS_UNDEFALL
/* Delete all user-defined macro definitions */
#if TPP_HAVE_CPP_MACROS
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_undefalluser(tpp_keywords *tpp_restrict self);
#else /* TPP_HAVE_CPP_MACROS */
#define tpp_keywords_undefalluser(self) (void)0
#endif /* !TPP_HAVE_CPP_MACROS */
#endif /* TPP_HAVE_KEYWORDS_UNDEFALL */


#if TPP_HAVE_KEYWORDS_UNASSERTALL
/* Delete all user-defined keyword assertions */
#if TPP_HAVE_CPP_ASSERT
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_unassertall(tpp_keywords *tpp_restrict self);
#else /* TPP_HAVE_CPP_ASSERT */
#define tpp_keywords_unassertall(self) (void)0
#endif /* !TPP_HAVE_CPP_ASSERT */
#endif /* TPP_HAVE_KEYWORDS_UNASSERTALL */


#if TPP_HAVE_KEYWORDS_RESETFLAGS
/* Modify the flags of all keywords as `flags = flags & keep_mask` */
#if TPP_HAVE_KEYWORD_FLAGS
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_resetflags(tpp_keywords *tpp_restrict self,
                        tpp_keyword_flags keep_mask);
#else /* TPP_HAVE_KEYWORD_FLAGS */
#define tpp_keywords_resetflags(self, keep_mask) (void)0
#endif /* !TPP_HAVE_KEYWORD_FLAGS */
#endif /* TPP_HAVE_KEYWORDS_RESETFLAGS */

#if TPP_HAVE_KEYWORDS_RESETFEATURES
/* Reset all uses of `tpp_keyword_setfeature()` */
#if TPP_HAVE_KEYWORD_FEATURES
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_resetfeatures(tpp_keywords *tpp_restrict self);
#else /* TPP_HAVE_KEYWORD_FEATURES */
#define tpp_keywords_resetfeatures(self) (void)0
#endif /* !TPP_HAVE_KEYWORD_FEATURES */
#endif /* TPP_HAVE_KEYWORDS_RESETFEATURES */

#if TPP_HAVE_KEYWORDS_RESETCOUNTERS
/* Call `tpp_keyword_reset_builtin_counter()` on every keyword, thereby
 * resetting all side-effects of expansions of `__TPP_COUNTER` thus far. */
#if TPP_HAVE_MACRO___TPP_COUNTER
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_keywords_resetcounters(tpp_keywords *tpp_restrict self);
#else /* TPP_HAVE_MACRO___TPP_COUNTER */
#define tpp_keywords_resetcounters(self) (void)0
#endif /* !TPP_HAVE_MACRO___TPP_COUNTER */
#endif /* TPP_HAVE_KEYWORDS_RESETCOUNTERS */


TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_KEYWORD_H */
