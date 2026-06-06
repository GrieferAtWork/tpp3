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
#ifndef GUARD_TPP_API_H
#define GUARD_TPP_API_H 1

/*[[[tpp-begin]]]*/
#ifndef TPP_BUILDING
#define TPP_BUILDING 0
#endif /* !TPP_BUILDING */

#if TPP_BUILDING
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_OBSOLETE_NO_DEPRECATE

/* Disable some warnings */
#pragma warning(disable: 4065) /* "switch statement contains 'default' but no 'case' labels" */

/* Disable some garbage inspection warnings */
#pragma warning(disable: 26446)
#pragma warning(disable: 26482)
#pragma warning(disable: 26438)
#pragma warning(disable: 26494)
#pragma warning(disable: 26496)
#pragma warning(disable: 26485)
#pragma warning(disable: 26448)
#endif /* _MSC_VER */
#endif /* TPP_BUILDING */

#ifndef TPP_NO_SYSTEM_INCLUDES
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#endif /* !TPP_NO_SYSTEM_INCLUDES */

#ifndef __SIZEOF_POINTER__
#ifdef __has_include
#if __has_include(<hybrid/typecore.h>)
#include <hybrid/typecore.h>
#endif /* __has_include(<hybrid/typecore.h>) */
#endif /* __has_include */
#ifndef __SIZEOF_POINTER__
#ifdef SIZE_MAX
#if SIZE_MAX == UINT32_C(0xffffffff)
#define __SIZEOF_POINTER__ 4
#elif SIZE_MAX == UINT64_C(0xffffffffffffffff)
#define __SIZEOF_POINTER__ 8
#elif SIZE_MAX == UINT16_C(0xffff)
#define __SIZEOF_POINTER__ 2
#elif SIZE_MAX == UINT8_C(0xff)
#define __SIZEOF_POINTER__ 1
#endif /* ... */
#endif /* SIZE_MAX */
#ifndef __SIZEOF_POINTER__
#if defined(_WIN64) || defined(WIN64)
#define __SIZEOF_POINTER__ 8
#elif defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
#define __SIZEOF_POINTER__ 4
#endif /* !_WIN32 && __WIN32__ */
#if !defined(__SIZEOF_POINTER__) && !defined(__DEEMON__)
#error "No way to determine '__SIZEOF_POINTER__'"
#endif /* !__SIZEOF_POINTER__ */
#endif /* !__SIZEOF_POINTER__ */
#endif /* !__SIZEOF_POINTER__ */
#endif /* !__SIZEOF_POINTER__ */

#ifndef __SIZEOF_SIZE_T__
#define __SIZEOF_SIZE_T__ __SIZEOF_POINTER__
#endif /* !__SIZEOF_SIZE_T__ */

#ifndef __SIZEOF_INT__
#ifdef __has_include
#if __has_include(<hybrid/limitcore.h>)
#include <hybrid/limitcore.h>
#endif /* __has_include(<hybrid/limitcore.h>) */
#if __has_include(<limits.h>)
#include <limits.h>
#endif /* __has_include(<limits.h>) */
#endif /* __has_include */
#ifndef __SIZEOF_INT__
#ifdef INT_MAX
#if INT_MAX == 127
#define __SIZEOF_INT__ 1
#elif INT_MAX == 32767
#define __SIZEOF_INT__ 2
#elif INT_MAX == 2147483647
#define __SIZEOF_INT__ 4
#elif INT_MAX == 9223372036854775807
#define __SIZEOF_INT__ 8
#endif /* ... */
#endif /* INT_MAX */
#ifndef __SIZEOF_INT__
#define __SIZEOF_INT__ 4
#endif /* !__SIZEOF_INT__ */
#endif /* !__SIZEOF_INT__ */
#endif /* !__SIZEOF_INT__ */

#define TPP_PREPROCESSOR_VERSION     300 /* Preprocessor version. */
#define TPP_API_VERSION              300 /* Api version (Version of this api). */
#define TPP_PREPROCESSOR_VERSION_STR "300"

/* The standard calling convention used by TPP APIs */
#ifndef TPPCALL
#define TPPCALL /* nothing */
#endif /* !TPPCALL */

/* The standard calling convention used by TPP APIs for variadic functions. */
#ifndef TPPVCALL
#define TPPVCALL /* nothing */
#endif /* !TPPVCALL */

#ifndef TPP_DECL
#define TPP_DECL extern
#endif /* !TPP_DECL */
#ifndef TPP_IMPL
#define TPP_IMPL /* nothing */
#endif /* !TPP_IMPL */
#ifndef TPP_CONST_DECL
#define TPP_CONST_DECL TPP_DECL
#endif /* !TPP_CONST_DECL */
#ifndef TPP_CONST_IMPL
#define TPP_CONST_IMPL TPP_IMPL
#endif /* !TPP_CONST_IMPL */
#ifndef TPP_INTERN_DECL
#define TPP_INTERN_DECL extern
#endif /* !TPP_INTERN_DECL */
#ifndef TPP_INTERN_IMPL
#define TPP_INTERN_IMPL /* nothing */
#endif /* !TPP_INTERN_IMPL */
#ifndef tpp_restrict
#define tpp_restrict __restrict
#endif /* !tpp_restrict */
#ifndef TPP_NONNULL
#define TPP_NONNULL(x) /* nothing */
#endif /* !TPP_NONNULL */
#ifndef TPP_WUNUSED
#define TPP_WUNUSED /* nothing */
#endif /* !TPP_WUNUSED */
#ifndef TPP_RETNONNULL
#define TPP_RETNONNULL /* nothing */
#endif /* !TPP_RETNONNULL */
#ifndef TPP_PURECALL
#define TPP_PURECALL /* nothing */
#endif /* !TPP_PURECALL */
#ifndef TPP_CONSTCALL
#define TPP_CONSTCALL /* nothing */
#endif /* !TPP_CONSTCALL */
#ifndef TPP_NOINLINE
#ifdef _MSC_VER
#define TPP_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define TPP_NOINLINE __attribute__((__noinline__))
#else /* ... */
#define TPP_NOINLINE /* nothing */
#endif /* !... */
#endif /* !TPP_NOINLINE */
#ifndef TPP_FLEX_ARRAY
#define TPP_FLEX_ARRAY 4096
#endif /* !TPP_FLEX_ARRAY */

/* Does the host preprocessor have support for __VA_ARGS__? */
#ifndef TPP_HOST_HAVE_PP_VARARGS
#define TPP_HOST_HAVE_PP_VARARGS 1
#endif /* !TPP_HOST_HAVE_PP_VARARGS */

#ifndef TPP_CHAR_BIT
#ifdef CHAR_BIT
#define TPP_CHAR_BIT CHAR_BIT
#elif defined(__CHAR_BIT__)
#define TPP_CHAR_BIT __CHAR_BIT__
#else /* ... */
#define TPP_CHAR_BIT 8
#endif /* !... */
#endif /* !TPP_CHAR_BIT */


#ifndef TPP_DEBUG
#ifdef NDEBUG
#define TPP_DEBUG 0
#else /* NDEBUG */
#define TPP_DEBUG 1
#endif /* !NDEBUG */
#endif /* !TPP_DEBUG */

#ifndef TPP_INLINE
#if defined(__cplusplus) || defined(inline)
#define TPP_INLINE static inline
#elif defined(_MSC_VER)
#define TPP_INLINE static __inline
#elif defined(__GNUC__) || defined(__TCC__) || defined(__DCC_VERSION__)
#define TPP_INLINE static __inline__
#else /* ... */
#define TPP_INLINE static
#endif /* !... */
#endif /* !TPP_INLINE */

#ifdef __cplusplus
#define TPP_DECL_BEGIN extern "C" {
#define TPP_DECL_END   }
#else /* __cplusplus */
#define TPP_DECL_BEGIN /* nothing */
#define TPP_DECL_END   /* nothing */
#endif /* !__cplusplus */

#ifndef tpp_offsetof
#include <stddef.h>
#define tpp_offsetof offsetof
#endif /* !tpp_offsetof */

#ifndef tpp_lengthof
#define tpp_lengthof(a) (sizeof(a) / sizeof(*(a)))
#endif /* !tpp_lengthof */

#ifndef tpp_unreachable
#ifdef _MSC_VER
#define tpp_unreachable() __assume(0)
#else /* _MSC_VER */
#define tpp_unreachable() __builtin_unreachable()
#endif /* !_MSC_VER */
#endif /* !tpp_unreachable */

#ifndef tpp_memcpy
#include <string.h>
#define tpp_strlen      strlen
#define tpp_strnlen     strnlen
#define tpp_strcmp      strcmp
#define tpp_memcmp      memcmp
#define tpp_memcpy      memcpy
#define tpp_memset      memset
#define tpp_memmove     memmove
#define tpp_memmoveup   memmove
#define tpp_memmovedown memmove
#endif /* !tpp_memcpy */

#ifndef tpp_expect
#define tpp_expect(expr, expected) expr
#define tpp_expect_IS_NOOP
#endif /* !tpp_expect */

#ifndef tpp_likely
#ifdef tpp_expect_IS_NOOP
#define tpp_likely   /* nothing */
#define tpp_unlikely /* nothing */
#else /* tpp_expect_IS_NOOP */
#define tpp_likely(expr)   tpp_expect(!!(expr), 1)
#define tpp_unlikely(expr) tpp_expect(!!(expr), 0)
#endif /* !tpp_expect_IS_NOOP */
#endif /* !tpp_unlikely */

#ifndef tpp_size
#define TPP_SIZEOF_tpp_size __SIZEOF_SIZE_T__
#define TPP_SIZE_MAX SIZE_MAX
#define tpp_size size_t
#endif /* !tpp_size */
#ifndef tpp_ssize
#define tpp_ssize ptrdiff_t
#endif /* !tpp_ssize */
#ifndef tpp_hash
#if UINT_FAST32_MAX == UINT32_C(0xffffffff)
#define TPP_SIZEOF_tpp_hash 4
#elif UINT_FAST32_MAX == UINT64_C(0xffffffffffffffff)
#define TPP_SIZEOF_tpp_hash 8
#else /* UINT_FAST32_MAX == ... */
#error "Unrecognized 'UINT_FAST32_MAX'"
#endif /* UINT_FAST32_MAX != ... */
#define tpp_hash uint_fast32_t
#endif /* !tpp_hash */
#ifndef tpp_line
#define tpp_line   int_fast32_t
#define tpp_column int_fast32_t
#endif /* !tpp_line */
#ifndef tpp_char
#define tpp_char unsigned char
#endif /* !tpp_char */
#ifndef tpp_unichar
#define tpp_unichar uint_least32_t
#endif /* !tpp_unichar */

#ifndef tpp_intmax
#define tpp_intmax      intmax_t
#define tpp_uintmax     uintmax_t
#define TPP_UINTMAX_MAX UINTMAX_MAX
#define TPP_UINTMAX_C   UINTMAX_C
#endif /* !tpp_intmax */

#ifndef TPP_REF
#define TPP_REF       /* nothing */
#define TPP_REF_IF(c) /* nothing */
#endif /* !TPP_REF */

#ifndef TPP_STATIC_ASSERT
#define _TPP_STATIC_ASSERT_ID2(line) tpp_static_assert_##line
#define _TPP_STATIC_ASSERT_ID(line) _TPP_STATIC_ASSERT_ID2(line)
#define TPP_STATIC_ASSERT(expr) typedef int _TPP_STATIC_ASSERT_ID(__LINE__)[(expr) ? 1 : -1]
#endif /* !TPP_STATIC_ASSERT */

#ifndef tpp_malloc
#include <stdlib.h>
#define tpp_trymalloc(s)     malloc(s)     /* tpp_trymalloc -- use when failure allows for re-try */
#define tpp_malloc(s)        malloc(s)     /* tpp_malloc    -- use when failure means error-propagation */
#define tpp_tryrealloc(p, s) realloc(p, s)
#define tpp_realloc(p, s)    realloc(p, s)
#define tpp_free(p)          free(p)
#endif /* !tpp_malloc */

TPP_DECL_BEGIN

#ifndef tpp_formatprinter
#define tpp_formatprinter tpp_formatprinter
#define TPP_FORMATPRINTER_CC TPPCALL
typedef tpp_ssize (TPP_FORMATPRINTER_CC *tpp_formatprinter)(void *arg, tpp_char const *text, tpp_size num_bytes);
#define tpp_formatprinter_print(printer, arg, text, num_bytes) \
	((*printer)(arg, text, num_bytes))
#endif /* !tpp_formatprinter */

#ifndef tpp_lcinfo
typedef struct {
	tpp_line   lci_line; /* Line */
	tpp_column lci_col;  /* Column */
} tpp_lcinfo;
#define tpp_lcinfo tpp_lcinfo

#define tpp_lcinfo_getline(self) ((tpp_line)(self).lci_line)
#define tpp_lcinfo_getcol(self)  ((tpp_column)(self).lci_col)
#define tpp_lcinfo_init(self, line, col) \
	(void)((self).lci_line = line, (self).lci_col = col)

TPP_INLINE TPP_WUNUSED tpp_lcinfo TPPCALL
tpp_lcinfo_of(tpp_line line, tpp_column col) {
	tpp_lcinfo result;
	result.lci_line = line;
	result.lci_col  = col;
	return result;
}
#endif /* !tpp_lcinfo */

#ifndef tpp_lcinfo_init
#define tpp_lcinfo_init(self, line, col) \
	(void)((self) = tpp_lcinfo_of(line, col))
#endif /* !tpp_lcinfo_init */

#ifndef tpp_refcnt
/* NOTE: Multi-threaded applications can leave this alone: a single
 *       TPP lexer can only ever be used by a single thread, meaning
 *       that reference counts don't need to be atomic, because all
 *       components are thread-local. */
typedef struct {
	uint_fast32_t trc_count; /* Reference counter */
} tpp_refcnt;
#define tpp_refcnt             tpp_refcnt
#define TPP_REFCNT_INIT(v)     { v }
#define tpp_refcnt_init(p, v)  (void)((p)->trc_count = (v))
#define tpp_refcnt_inc(p)      (void)(++(p)->trc_count)
#define tpp_refcnt_decfetch(p) (--(p)->trc_count)
#define tpp_refcnt_isshared(p) ((p)->trc_count > 1)
#endif /* !tpp_refcnt */
#ifndef tpp_refcnt_dec
#define tpp_refcnt_dec(p) (void)tpp_refcnt_decfetch(p)
#endif /* !tpp_refcnt_dec */

#ifndef tpp_refcnt_atomic
/* WARNING: Multi-threaded applications must override this */
typedef struct {
	uint_fast32_t trca_count; /* Reference counter */
} tpp_refcnt_atomic;
#define tpp_refcnt_atomic             tpp_refcnt_atomic
#define TPP_REFCNT_ATOMIC_INIT(v)     { v }
#define tpp_refcnt_atomic_init(p, v)  (void)((p)->trca_count = (v))
#define tpp_refcnt_atomic_inc(p)      (void)(++(p)->trca_count)
#define tpp_refcnt_atomic_decfetch(p) (--(p)->trca_count)
#define tpp_refcnt_atomic_isshared(p) ((p)->trca_count > 1)
#endif /* !tpp_refcnt_atomic */
#ifndef tpp_refcnt_atomic_dec
#define tpp_refcnt_atomic_dec(p) (void)tpp_refcnt_atomic_decfetch(p)
#endif /* !tpp_refcnt_atomic_dec */

/* WARNING: Multi-threaded applications must override this */
#ifndef tpp_once
#define tpp_once(expr)             \
	do {                           \
		static int _to_didrun = 0; \
		if (!_to_didrun) {         \
			_to_didrun = 1;        \
			expr;                  \
		}                          \
	} while (0)
#endif /* !tpp_once */

TPP_DECL_END


#ifndef tpp_assert
#include <assert.h>
#define tpp_assert assert
#endif /* !tpp_assert */
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_API_H */
