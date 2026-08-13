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

#ifndef TPP_HOST_HAS_ATTRIBUTE
#ifdef __has_attribute
#define TPP_HOST_HAS_ATTRIBUTE(x) __has_attribute(x)
#else /* __has_attribute */
#define TPP_HOST_HAS_ATTRIBUTE(x) 0
#endif /* !__has_attribute */
#endif /* !TPP_HOST_HAS_ATTRIBUTE */

/* Declaration providers for internal functions used across multiple source files.
 * HINT: These get hard-overwritten to "static" in "tpp-amalgamation.c" */
#ifndef TPP_INTERN_IMPL
#if (defined(_WIN64) || defined(WIN64) || \
     defined(_WIN32) || defined(WIN32) || \
     defined(__WIN32__) || defined(__CYGWIN__))
#define TPP_INTERN_IMPL /* nothing */
#elif TPP_HOST_HAS_ATTRIBUTE(__visibility__)
#define TPP_INTERN_IMPL __attribute__((__visibility__("hidden")))
#else /* ... */
#define TPP_INTERN_IMPL /* nothing */
#endif /* !... */
#endif /* !TPP_INTERN_IMPL */
#ifndef TPP_INTERN_DECL
#define TPP_INTERN_DECL extern TPP_INTERN_IMPL
#endif /* !TPP_INTERN_DECL */

/* Not for amalgamation: enable memory leak debugger */
#ifdef _MSC_VER
#if ((defined(TPP_BUILDING) && TPP_BUILDING) || \
     (defined(TPP_BUILDING_OPTIONAL) && TPP_BUILDING_OPTIONAL))
#define _CRT_SECURE_NO_WARNINGS    /* Know your... */
#define _CRT_NONSTDC_NO_WARNINGS   /* ... f$cking place ... */
#define _CRT_OBSOLETE_NO_DEPRECATE /* ... trash! */
#endif /* ... */
#define _CRTDBG_MAP_ALLOC 1
#include <crtdbg.h>
#endif /* _MSC_VER */


/*[[[tpp-begin]]]*/
#ifndef TPP_BUILDING
#define TPP_BUILDING 0
#endif /* !TPP_BUILDING */

#ifndef TPP_BUILDING_OPTIONAL
#define TPP_BUILDING_OPTIONAL 0
#endif /* !TPP_BUILDING_OPTIONAL */

#if TPP_BUILDING || TPP_BUILDING_OPTIONAL
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS    /* Know your... */
#define _CRT_NONSTDC_NO_WARNINGS   /* ... f$cking place ... */
#define _CRT_OBSOLETE_NO_DEPRECATE /* ... trash! */

/* Disable some warnings */
#pragma warning(disable: 4065) /* "switch statement contains 'default' but no 'case' labels" (cannot be avoided under certain feature-configurations) */
#pragma warning(disable: 4127) /* Conditional expression is constant (cannot be avoided when features are compile-time enabled/disabled) */

/* Disable some garbage inspection warnings */
#pragma warning(disable: 26446)
#pragma warning(disable: 26482)
#pragma warning(disable: 26438)
#pragma warning(disable: 26494)
#pragma warning(disable: 26496)
#pragma warning(disable: 26485)
#pragma warning(disable: 26448)
#pragma warning(disable: 26461) /* Kind-of like this one, but also warns in cases where adding "const" causes compiler errors due to function prototype conflicts... */
#pragma warning(disable: 26826)
#endif /* _MSC_VER */
#endif /* TPP_BUILDING || TPP_BUILDING_OPTIONAL */

/************************************************************************/
/* HOST COMPILER/SYSTEM/OS CONFIGURATION: TPP_HOST_*                    */
/************************************************************************/

/* Define if host OS is like windows */
#ifndef TPP_OS_WINDOWS
#if (defined(_WIN64) || defined(WIN64) || \
     defined(_WIN32) || defined(WIN32) || defined(__WIN32__))
#define TPP_OS_WINDOWS 1
#else /* ... */
#define TPP_OS_WINDOWS 0
#endif /* !... */
#endif /* !TPP_OS_WINDOWS */

/* Define if host OS is like unix */
#ifndef TPP_OS_UNIX
#if (defined(__unix__) || defined(__unix) || defined(unix) || \
     defined(__posix__) || defined(__posix) || defined(posix))
#define TPP_OS_UNIX 1
#else /* ... */
#define TPP_OS_UNIX 0
#endif /* !... */
#endif /* !TPP_OS_UNIX */

/* >> #define TPP_HOST_NO_SYSTEM_INCLUDES 1
 * Prevent TPP sources from doing `#include <foo.h>` -- instead, you must pre-
 * include all dependencies yourself before doing `#include "tpp-amalgamation.h"` */
#ifndef TPP_HOST_NO_SYSTEM_INCLUDES
#define TPP_HOST_NO_SYSTEM_INCLUDES 0
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */

#ifndef TPP_HOST_HAS_ATTRIBUTE
#ifdef __has_attribute
#define TPP_HOST_HAS_ATTRIBUTE(x) __has_attribute(x)
#else /* __has_attribute */
#define TPP_HOST_HAS_ATTRIBUTE(x) 0
#endif /* !__has_attribute */
#endif /* !TPP_HOST_HAS_ATTRIBUTE */

#ifndef TPP_HOST_HAS_DECLSPEC_ATTRIBUTE
#ifdef __has_declspec_attribute
#define TPP_HOST_HAS_DECLSPEC_ATTRIBUTE(x) __has_declspec_attribute(x)
#else /* __has_declspec_attribute */
#define TPP_HOST_HAS_DECLSPEC_ATTRIBUTE(x) 0
#endif /* !__has_declspec_attribute */
#endif /* !TPP_HOST_HAS_DECLSPEC_ATTRIBUTE */

#ifndef TPP_HOST_HAS_CPP_ATTRIBUTE
#ifdef __has_cpp_attribute
#define TPP_HOST_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else /* __has_cpp_attribute */
#define TPP_HOST_HAS_CPP_ATTRIBUTE(x) 0
#endif /* !__has_cpp_attribute */
#endif /* !TPP_HOST_HAS_CPP_ATTRIBUTE */

#ifndef TPP_HOST_HAS_INCLUDE
#ifdef __has_include
#define TPP_HOST_HAS_INCLUDE(x) __has_include(x)
#else /* __has_include */
#define TPP_HOST_HAS_INCLUDE(x) 0
#endif /* !__has_include */
#endif /* !TPP_HOST_HAS_INCLUDE */

#ifndef TPP_HOST_HAS_BUILTIN
#ifdef __has_builtin
#define TPP_HOST_HAS_BUILTIN(x) __has_builtin(x)
#else /* __has_builtin */
#define TPP_HOST_HAS_BUILTIN(x) 0
#endif /* !__has_builtin */
#endif /* !TPP_HOST_HAS_BUILTIN */

/* Does the host preprocessor have support for __VA_ARGS__? */
#ifndef TPP_HOST_HAVE_PP_VARARGS
#define TPP_HOST_HAVE_PP_VARARGS 1
#endif /* !TPP_HOST_HAVE_PP_VARARGS */

/* These headers should always be available (even when -ffreestanding)
 * Still: If "TPP_HOST_NO_SYSTEM_INCLUDES" is defined, don't include anything! */
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */



/************************************************************************/
/* TPP3 VERSION NUMBERS                                                 */
/************************************************************************/
#define TPP_API_VERSION              300   /* Api version (Version of this api). */
#define TPP_PREPROCESSOR_VERSION     300   /* Preprocessor version. */
#define TPP_PREPROCESSOR_VERSION_STR "300" /* str(TPP_PREPROCESSOR_VERSION) */



/************************************************************************/
/* TPP API CONFIGURATION                                                */
/************************************************************************/

/* The standard calling convention used by TPP APIs */
#ifndef TPPCALL
#define TPPCALL /* nothing */
#endif /* !TPPCALL */

/* The standard calling convention used by TPP APIs for variadic functions. */
#ifndef TPPVCALL
#define TPPVCALL /* nothing */
#endif /* !TPPVCALL */

#ifndef TPP_GCC_VERSION_NUM
#ifdef __GNUC__
#ifndef TPP_GCC_VERSION_MINOR
#ifdef __GNUC_MINOR__
#define TPP_GCC_VERSION_MINOR __GNUC_MINOR__
#else /* __GNUC_MINOR__ */
#define TPP_GCC_VERSION_MINOR 0
#endif /* !__GNUC_MINOR__ */
#endif /* !TPP_GCC_VERSION_MINOR */
#ifndef TPP_GCC_VERSION_PATCH
#ifdef __GNUC_PATCH__
#define TPP_GCC_VERSION_PATCH __GNUC_PATCH__
#elif defined(__GNUC_PATCHLEVEL__)
#define TPP_GCC_VERSION_PATCH __GNUC_PATCHLEVEL__
#else /* __GNUC_PATCH__ */
#define TPP_GCC_VERSION_PATCH 0
#endif /* !__GNUC_PATCH__ */
#endif /* !TPP_GCC_VERSION_PATCH */
#define TPP_GCC_VERSION_NUM (__GNUC__ * 10000 + TPP_GCC_VERSION_MINOR * 100 + TPP_GCC_VERSION_PATCH)
#else /* __GNUC__ */
#define TPP_GCC_VERSION_NUM 0
#endif /* !__GNUC__ */
#endif /* !TPP_GCC_VERSION_NUM */

/* When defined to `1`, TPP is configured such that *everything* is declared
 * and implement as `static`. When that is the case, TPP is only accessible
 * within the source file that includes `tpp-amalgamation.c`, but also makes
 * it possible to have *multiple* (possibly differently configured) instances
 * of TPP exist within the same process.
 *
 * If you only indent to use TPP from a singular source file, enabling this
 * option is *highly* recommended, as it also allows compilers to done some
 * advanced optimizations:
 * - Since everything is contained in a single source file, the compiler can
 *   essentially optimization TPP like it would normally only do during a
 *   whole-program-optimization pass (which can normally only happen during
 *   linking)
 * - Since everything is defined `static`, compilers will tell you which
 *   functions/APIs are still enabled by your config, but aren't actually
 *   used anywhere (meaning you could just turn off whatever config(s) cause
 *   those functions to be provided in order to further optimize TPP) */
#ifndef TPP_USE_STATIC
#define TPP_USE_STATIC 0
#endif /* !TPP_USE_STATIC */


#ifndef TPP_IMPL
#if TPP_USE_STATIC
#define TPP_IMPL static
#elif (defined(_WIN64) || defined(WIN64) || \
       defined(_WIN32) || defined(WIN32) || \
       defined(__WIN32__) || defined(__CYGWIN__))
#define TPP_IMPL /* nothing */
#elif TPP_HOST_HAS_ATTRIBUTE(__visibility__)
#define TPP_IMPL __attribute__((__visibility__("hidden")))
#else /* ... */
#define TPP_IMPL /* nothing */
#endif /* !... */
#endif /* !TPP_IMPL */

#ifndef TPP_DECL
#if TPP_USE_STATIC
#define TPP_DECL static
#else /* TPP_USE_STATIC */
#define TPP_DECL extern TPP_IMPL
#endif /* !TPP_USE_STATIC */
#endif /* !TPP_DECL */

#ifndef TPP_CONST_IMPL
#if TPP_USE_STATIC
#define TPP_CONST_IMPL static
#elif defined(__cplusplus)
#define TPP_CONST_IMPL extern TPP_IMPL
#else /* __cplusplus */
#define TPP_CONST_IMPL TPP_IMPL
#endif /* !__cplusplus */
#endif /* !TPP_CONST_IMPL */
#if TPP_USE_STATIC
#undef TPP_CONST_DECL /* Can't forward-declare static data */
#elif !defined(TPP_CONST_DECL)
#define TPP_CONST_DECL TPP_DECL
#endif /* !TPP_CONST_DECL */

#ifndef tpp_assume
#ifdef _MSC_VER
#define tpp_assume(x) __assume(x)
#elif  TPP_HOST_HAS_BUILTIN(__builtin_assume)
#define tpp_assume(x) __builtin_assume(x)
#elif  TPP_HOST_HAS_ATTRIBUTE(__assume__) || TPP_GCC_VERSION_NUM >= 130000
#define tpp_assume(x) __attribute__((__assume__(x)))
#elif TPP_HOST_HAS_BUILTIN(__builtin_unreachable) || TPP_GCC_VERSION_NUM >= 40600
#define tpp_assume(x)                \
	do {                             \
		if (!(x))                    \
			__builtin_unreachable(); \
	} while (0)
#else /* ... */
#define tpp_assume(x) (void)0
#endif /* !... */
#endif /* !tpp_assume */

#ifndef tpp_restrict
#ifdef restrict
#define tpp_restrict restrict
#elif defined(_MSC_VER) || TPP_GCC_VERSION_NUM >= 29200
#define tpp_restrict __restrict
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ + 0) >= 199901
#define tpp_restrict restrict /* C99 */
#else /* ... */
#define tpp_restrict /* nothing */
#endif /* !... */
#endif /* !tpp_restrict */
#ifndef TPP_NONNULL
#if TPP_HOST_HAS_ATTRIBUTE(__nonnull__) || TPP_GCC_VERSION_NUM >= 30300
#define TPP_NONNULL(x) __attribute__((__nonnull__ x))
#else /* ... */
#define TPP_NONNULL(x) /* nothing */
#endif /* !... */
#endif /* !TPP_NONNULL */
#ifndef TPP_WUNUSED
#if TPP_HOST_HAS_ATTRIBUTE(__warn_unused_result__) || TPP_GCC_VERSION_NUM >= 30300
#define TPP_WUNUSED __attribute__((__warn_unused_result__))
#else /* ... */
#define TPP_WUNUSED /* nothing */
#endif /* !... */
#endif /* !TPP_WUNUSED */
#ifndef TPP_RETNONNULL
#if TPP_HOST_HAS_ATTRIBUTE(__returns_nonnull__) || TPP_GCC_VERSION_NUM != 0
#define TPP_RETNONNULL __attribute__((__returns_nonnull__))
#else /* ... */
#define TPP_RETNONNULL /* nothing */
#endif /* !... */
#endif /* !TPP_RETNONNULL */
#ifndef TPP_PURECALL
#if TPP_HOST_HAS_ATTRIBUTE(__pure__) || TPP_GCC_VERSION_NUM >= 29600
#define TPP_PURECALL __attribute__((__pure__))
#elif defined(_MSC_VER) || TPP_HOST_HAS_DECLSPEC_ATTRIBUTE(noalias)
#define TPP_PURECALL __declspec(noalias)
#else /* ... */
#define TPP_PURECALL /* nothing */
#endif /* !... */
#endif /* !TPP_PURECALL */
#ifndef TPP_CONSTCALL
#if TPP_HOST_HAS_ATTRIBUTE(__const__) || TPP_GCC_VERSION_NUM >= 20400
#define TPP_CONSTCALL __attribute__((__const__))
#else /* ... */
#define TPP_CONSTCALL TPP_PURECALL
#endif /* !... */
#endif /* !TPP_CONSTCALL */
#ifndef TPP_COLDCALL
#if TPP_HOST_HAS_ATTRIBUTE(__cold__) || TPP_GCC_VERSION_NUM >= 40300
#define TPP_COLDCALL __attribute__((__cold__))
#else /* ... */
#define TPP_COLDCALL /* nothing */
#endif /* !... */
#endif /* !TPP_COLDCALL */
#ifndef TPP_NOINLINE
#if TPP_HOST_HAS_ATTRIBUTE(__noinline__) || TPP_GCC_VERSION_NUM >= 29600
#define TPP_NOINLINE __attribute__((__noinline__))
#elif defined(_MSC_VER) || TPP_HOST_HAS_DECLSPEC_ATTRIBUTE(noinline)
#define TPP_NOINLINE __declspec(noinline)
#else /* ... */
#define TPP_NOINLINE /* nothing */
#endif /* !... */
#endif /* !TPP_NOINLINE */
#ifndef TPP_FLEX_ARRAY
#if defined(_MSC_VER) || (TPP_GCC_VERSION_NUM && TPP_GCC_VERSION_NUM < 20970)
#define TPP_FLEX_ARRAY 4096
#elif TPP_GCC_VERSION_NUM != 0
#define TPP_FLEX_ARRAY 0 /*__extension__*/
#else /* ... */
#define TPP_FLEX_ARRAY /* nothing */
#endif /* !... */
#endif /* !TPP_FLEX_ARRAY */
#ifndef TPP_FALLTHRU
#if TPP_HOST_HAS_CPP_ATTRIBUTE(fallthrough)
#define TPP_FALLTHRU [[fallthrough]];
#elif TPP_HOST_HAS_ATTRIBUTE(__fallthrough__) || TPP_GCC_VERSION_NUM >= 70000
#define TPP_FALLTHRU __attribute__((__fallthrough__));
#elif TPP_GCC_VERSION_NUM >= 60000
#define TPP_FALLTHRU __attribute__((fallthrough));
#else /* ... */
#define TPP_FALLTHRU /* @fallthrough@ */
#endif /* !... */
#endif /* !TPP_FALLTHRU */

#ifndef TPP_CHAR_BIT
#ifdef CHAR_BIT
#define TPP_CHAR_BIT CHAR_BIT
#elif defined(__CHAR_BIT__)
#define TPP_CHAR_BIT __CHAR_BIT__
#else /* ... */
#define TPP_CHAR_BIT 8
#endif /* !... */
#endif /* !TPP_CHAR_BIT */


/* Used to wrap an identifier that is considered to be internal to TPP
 * Identifiers wrapped as such should **NOT** be accessed directly.
 * Instead, the provided TPP APIs should be used. */
#ifndef TPP_INTERNAL
#if TPP_BUILDING
#define TPP_INTERNAL(x) x
#else /* TPP_BUILDING */
#define TPP_INTERNAL(x) _##x
#endif /* !TPP_BUILDING */
#endif /* !TPP_INTERNAL */

#ifndef TPP_HAVE_TPP2_COMPAT
#define TPP_HAVE_TPP2_COMPAT 0
#endif /* !TPP_HAVE_TPP2_COMPAT */

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
#elif defined(__TCC__) || defined(__DCC_VERSION__) || TPP_GCC_VERSION_NUM >= 20700
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
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <stddef.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_offsetof offsetof
#endif /* !tpp_offsetof */

#ifndef tpp_container_of
#define tpp_container_of(ptr, type, member) \
	((type *)((char *)(ptr) - tpp_offsetof(type, member)))
#endif /* !tpp_container_of */

#ifndef tpp_lengthof
#define tpp_lengthof(a) (sizeof(a) / sizeof(*(a)))
#endif /* !tpp_lengthof */

#ifndef tpp_unreachable
#ifdef _MSC_VER
#define tpp_unreachable() __assume(0)
#elif TPP_HOST_HAS_BUILTIN(__builtin_unreachable) || TPP_GCC_VERSION_NUM >= 40600
#define tpp_unreachable() __builtin_unreachable()
#else /* ... */
#define tpp_unreachable() do{}while(1)
#endif /* !... */
#endif /* !tpp_unreachable */

#ifndef tpp_expect
#if TPP_HOST_HAS_BUILTIN(__builtin_expect) || TPP_GCC_VERSION_NUM >= 29700
#define tpp_expect(expr, expected) __builtin_expect(expr, expected)
#else /* ... */
#define tpp_expect(expr, expected) expr
#define tpp_expect_IS_NOOP
#endif /* !... */
#endif /* !tpp_expect */

#ifndef tpp_likely
#ifdef tpp_expect_IS_NOOP
#define tpp_likely   /* nothing */
#define tpp_unlikely /* nothing */
#else /* tpp_expect_IS_NOOP */
#define tpp_likely(expr)   (tpp_expect(!!(expr), 1))
#define tpp_unlikely(expr) (tpp_expect(!!(expr), 0))
#endif /* !tpp_expect_IS_NOOP */
#endif /* !tpp_unlikely */

#ifndef tpp_size
#define tpp_size     size_t
#define TPP_SIZE_MAX SIZE_MAX
#endif /* !tpp_size */
#ifndef TPP_SIZEOF_tpp_size
#if TPP_SIZE_MAX == UINT32_C(0xffffffff)
#define TPP_SIZEOF_tpp_size 4
#elif TPP_SIZE_MAX == UINT64_C(0xffffffffffffffff)
#define TPP_SIZEOF_tpp_size 8
#elif TPP_SIZE_MAX == UINT16_C(0xffff)
#define TPP_SIZEOF_tpp_size 2
#elif TPP_SIZE_MAX == UINT8_C(0xff)
#define TPP_SIZEOF_tpp_size 1
#else /* ... */
#error "Unsupported 'TPP_SIZE_MAX' - Please provide your own '#define TPP_SIZEOF_tpp_size'"
#endif /* !... */
#endif /* !TPP_SIZEOF_tpp_size */
#ifndef tpp_ssize
#define tpp_ssize ptrdiff_t
#ifdef PTRDIFF_MAX
#define TPP_SSIZE_MAX PTRDIFF_MAX
#else /* PTRDIFF_MAX */
#define TPP_SSIZE_MAX ((TPP_SIZE_MAX >> 1) - 1)
#endif /* !PTRDIFF_MAX */
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
#ifdef UINT32_C
#define TPP_HASH_C UINT32_C
#else /* UINT32_C */
#define TPP_HASH_C(x) x
#endif /* !UINT32_C */
#endif /* !tpp_hash */
#ifndef tpp_line
#if UINT_FAST32_MAX == UINT32_C(0xffffffff)
#define TPP_SIZEOF_tpp_line   4
#define TPP_SIZEOF_tpp_column 4
#elif UINT_FAST32_MAX == UINT64_C(0xffffffffffffffff)
#define TPP_SIZEOF_tpp_line   8
#define TPP_SIZEOF_tpp_column 8
#else /* UINT_FAST32_MAX == ... */
#error "Unrecognized 'UINT_FAST32_MAX'"
#endif /* UINT_FAST32_MAX != ... */
#define tpp_line   int_fast32_t
#define tpp_column int_fast32_t
#endif /* !tpp_line */
#ifndef tpp_char
#define tpp_char unsigned char
#endif /* !tpp_char */
#ifndef tpp_unichar
#define tpp_unichar   uint_least32_t
#define TPP_UNICHAR_C UINT32_C
#endif /* !tpp_unichar */

#ifndef tpp_intmax
#define tpp_intmax      intmax_t
#define tpp_uintmax     uintmax_t
#define TPP_UINTMAX_MAX UINTMAX_MAX
#define TPP_UINTMAX_C   UINTMAX_C
#endif /* !tpp_intmax */

/* Counter type used to implement `__COUNTER__` and `__TPP_COUNTER` */
#ifndef tpp_counter
#define tpp_counter tpp_size
#endif /* !tpp_counter */

#ifndef tpp_float
#define tpp_float long double
#endif /* !tpp_float */

#ifndef TPP_REF
#define TPP_REF       /* nothing */
#define TPP_REF_IF(c) /* nothing */
#endif /* !TPP_REF */

#ifndef TPP_STATIC_ASSERT
#define _TPP_STATIC_ASSERT_ID2(line) tpp_static_assert_##line
#define _TPP_STATIC_ASSERT_ID(line) _TPP_STATIC_ASSERT_ID2(line)
#define TPP_STATIC_ASSERT(expr) typedef int _TPP_STATIC_ASSERT_ID(__LINE__)[(expr) ? 1 : -1]
#endif /* !TPP_STATIC_ASSERT */
#ifndef TPP_STATIC_ASSERT_MSG
#define TPP_STATIC_ASSERT_MSG(expr, msg) TPP_STATIC_ASSERT(expr)
#endif /* !TPP_STATIC_ASSERT_MSG */



/* String API */
#ifndef tpp_memcpy
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <string.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_strlen      strlen
#define tpp_strchr      strchr
#define _tpp_strnlen    strnlen
#define tpp_strcmp      strcmp
#define tpp_memcmp      memcmp
#define tpp_memcpy      memcpy
#define tpp_memset      memset
#define tpp_memchr      memchr
#define tpp_memmove     memmove
#define tpp_memmoveup   memmove /* Same as "tpp_memmove", but guaranties that "dst >= src" */
#define tpp_memmovedown memmove /* Same as "tpp_memmove", but guaranties that "dst <= src" */
#if 0 /* Define if available; else, TPP will provide its own */
#define tpp_memmem      memmem
#endif
#endif /* !tpp_memcpy */

/* Use our own, custom definition of `strnlen()` to:
 * - Prevent GCC `-Wstringop-overread` warnings:
 *   src/tpp-amalgamation.h:5602:25: warning: 'strnlen' specified bound 9223372036854775807 exceeds source size 9223372036854775806 [-Wstringop-overread]
 * - Slightly improve performance since calls to `strnlen()`
 *   with a very large limit get turned into call to `strlen()` */
#ifndef tpp_strnlen
#if defined(__GNUC__) || TPP_HOST_HAS_BUILTIN(__builtin_constant_p)
#define tpp_strnlen(s, n) ((__builtin_constant_p(n) && (n) >= TPP_SSIZE_MAX) ? tpp_strlen(s) : _tpp_strnlen(s, n))
#else /* ... */
#define tpp_strnlen _tpp_strnlen
#endif /* !... */
#endif /* !tpp_strnlen */

#ifndef tpp_mempcpy
#define tpp_mempcpy(d, s, n) ((void *)((char *)tpp_memcpy(d, s, n) + (n)))
#endif /* !tpp_mempcpy */
#ifndef tpp_bzero
#define tpp_bzero(p, n) (void)tpp_memset(p, 0, n)
#endif /* !tpp_bzero */
#ifndef tpp_dbg_memset
#if TPP_DEBUG
#define tpp_dbg_memset(p, n) (void)tpp_memset(p, 0xcc, n)
#else /* TPP_DEBUG */
#define tpp_dbg_memset(p, n) (void)0
#endif /* !TPP_DEBUG */
#endif /* !tpp_dbg_memset */



/* Heap API */
#ifndef tpp_malloc
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <stdlib.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_trymalloc(s)     malloc(s)     /* tpp_trymalloc -- use when failure allows for re-try */
#define tpp_malloc(s)        malloc(s)     /* tpp_malloc    -- use when failure means error-propagation */
#define tpp_tryrealloc(p, s) realloc(p, s)
#define tpp_realloc(p, s)    realloc(p, s)
#define tpp_free(p)          free(p)
#endif /* !tpp_malloc */

#ifndef tpp_alloca
#if TPP_HOST_HAS_BUILTIN(__builtin_alloca) || TPP_GCC_VERSION_NUM >= 29700
#define tpp_alloca __builtin_alloca
#elif defined(_MSC_VER)
#include <malloc.h>
#define tpp_alloca _alloca
#elif TPP_HOST_HAS_INCLUDE(<alloca.h>)
#include <alloca.h>
#ifdef alloca
#define tpp_alloca alloca
#endif /* alloca */
#endif /* ... */
#endif /* !tpp_alloca */


/* Assertions API */
#ifndef tpp_assert
#if TPP_DEBUG
#if !TPP_HOST_NO_SYSTEM_INCLUDES
#include <assert.h>
#endif /* !TPP_HOST_NO_SYSTEM_INCLUDES */
#define tpp_assert assert
#else /* TPP_DEBUG */
#define tpp_assert(expr) (void)0
#endif /* !TPP_DEBUG */
#endif /* !tpp_assert */



/* Wrapper that is placed around every system call that TPP may make.
 * You can override this to inject additional checks before/after each system call.
 * @param: return_error: A macro "#define return_error(error) <...>" that may be
 *                       called from within the body of "TPP_SYSCALL". It's effect
 *                       is that the associated function will return with the "error"
 *                       specified. You should only use this with "TPP_EIO" or
 *                       "TPP_ENOMEM", which will then be propagated accordingly.
 *                       Trying to return other errors may cause undefined behavior
 *
 * Example:
 * >> #define TPP_SYSCALL(expr, return_error) \
 * >>     do {                                \
 * >>         if (CHECK_FOR_INTERRUPTS())     \
 * >>             return_error(TPP_EIO);      \
 * >>         BEGIN_BLOCKING;                 \
 * >>         expr;                           \
 * >>         END_BLOCKING;                   \
 * >>     } while (0)
 */
#ifndef TPP_SYSCALL
#define TPP_SYSCALL(expr, return_error) \
	do {                                \
		TPP_SYSCALL_NOFAIL(expr);       \
	} while (0)
#endif /* !TPP_SYSCALL */
/* Same as "TPP_SYSCALL()", but used in places where there's
 * no way to indicate errors (e.g. "tpp_io_close()") */
#ifndef TPP_SYSCALL_NOFAIL
#define TPP_SYSCALL_NOFAIL(expr) \
	do {                         \
		expr;                    \
	} while (0)
#endif /* !TPP_SYSCALL_NOFAIL */


TPP_DECL_BEGIN

/* Format-printer API */
#ifndef tpp_formatprinter
#define tpp_formatprinter tpp_formatprinter
#define TPP_FORMATPRINTER_CC TPPCALL
typedef tpp_ssize (TPP_FORMATPRINTER_CC *tpp_formatprinter)(void *arg, tpp_char const *text, tpp_size num_bytes);
#define tpp_formatprinter_print(printer, arg, text, num_bytes) \
	((*printer)(arg, text, num_bytes))
#define tpp_formatprinter_print_conststr(printer, arg, CONSTstr) \
	((*printer)(arg, (tpp_char const *)(CONSTstr), sizeof(CONSTstr) - sizeof(char)))
#define TPP_FORMATPRINTER_DEFINE(name, arg, text, num_bytes) \
	tpp_ssize (TPP_FORMATPRINTER_CC name)(void *arg, tpp_char const *text, tpp_size num_bytes)
#endif /* !tpp_formatprinter */
#ifndef tpp_formatprinter_print_cstr
#define tpp_formatprinter_print_cstr_IS_DEFAULT
#define tpp_formatprinter_print_cstr(printer, arg, text, num_bytes) \
	tpp_formatprinter_print(printer, arg, (tpp_char const *)(text), num_bytes)
#endif /* !tpp_formatprinter_print_cstr */



/* Line/Column-information API */
#ifndef tpp_lcinfo
#if defined(INT_LEAST64_MAX) && defined(UINT32_MAX) && !TPP_HAVE_TPP2_COMPAT
#define tpp_lcinfo int_least64_t

#define tpp_lcinfo_equals(a, b)  ((a) == (b))
#define tpp_lcinfo_getline(self) ((tpp_line)((int32_t)(uint32_t)(self)))
#define tpp_lcinfo_getcol(self)  ((tpp_column)((int32_t)(uint32_t)((self) >> 32)))
#define tpp_lcinfo_of(line, col)               \
	(((tpp_lcinfo)(uint32_t)(int32_t)(line)) | \
	 ((tpp_lcinfo)(uint32_t)(int32_t)(col) << 32))
#else /* INT_LEAST64_MAX && UINT32_MAX && !TPP_HAVE_TPP2_COMPAT */
typedef struct tpp_lcinfo {
	tpp_line   TPP_INTERNAL(lci_line); /* Line */
	tpp_column TPP_INTERNAL(lci_col);  /* Column */
} tpp_lcinfo;
#define tpp_lcinfo tpp_lcinfo

#define tpp_lcinfo_getline(self) ((tpp_line)(self).TPP_INTERNAL(lci_line))
#define tpp_lcinfo_getcol(self)  ((tpp_column)(self).TPP_INTERNAL(lci_col))
#define tpp_lcinfo_init(p_self, line, col)        \
	(void)((self)->TPP_INTERNAL(lci_line) = line, \
	       (self)->TPP_INTERNAL(lci_col)  = col)

TPP_INLINE TPP_WUNUSED tpp_lcinfo TPPCALL
tpp_lcinfo_of(tpp_line line, tpp_column col) {
	tpp_lcinfo result;
	result.TPP_INTERNAL(lci_line) = line;
	result.TPP_INTERNAL(lci_col)  = col;
	return result;
}
#endif /* !INT_LEAST64_MAX || !UINT32_MAX || TPP_HAVE_TPP2_COMPAT */
#endif /* !tpp_lcinfo */

#ifndef tpp_lcinfo_init
#define tpp_lcinfo_init(self, line, col) \
	(void)(*(self) = tpp_lcinfo_of(line, col))
#endif /* !tpp_lcinfo_init */
#ifndef tpp_lcinfo_setline
#define tpp_lcinfo_setline(self, line) tpp_lcinfo_init(self, line, tpp_lcinfo_getcol(*(self)))
#endif /* !tpp_lcinfo_setline */
#ifndef tpp_lcinfo_setcol
#define tpp_lcinfo_setcol(self, col) tpp_lcinfo_init(self, tpp_lcinfo_getline(*(self)), col)
#endif /* !tpp_lcinfo_setcol */
#ifndef tpp_lcinfo_equals
#define tpp_lcinfo_equals(a, b)                        \
	(tpp_lcinfo_getline(a) == tpp_lcinfo_getline(b) && \
	 tpp_lcinfo_getcol(a) == tpp_lcinfo_getcol(b))
#endif /* !tpp_lcinfo_equals */

/* Specifies an invalid LC information object */
#ifndef TPP_LCINFO_INVALID
#define TPP_LCINFO_INVALID              tpp_lcinfo_of(-1, -1)
#define tpp_lcinfo_isvalid(x)           (tpp_lcinfo_getcol(x) >= 0)
#define tpp_lcinfo_init_invalid(p_self) tpp_lcinfo_init(p_self, -1, -1)
#endif /* !TPP_LCINFO_INVALID */

/* Check if "x" represents valid line/column information */
#ifndef tpp_lcinfo_isvalid
#define tpp_lcinfo_isvalid(x) (!tpp_lcinfo_equals(x, TPP_LCINFO_INVALID))
#endif /* !tpp_lcinfo_isvalid */
#ifndef tpp_lcinfo_init_invalid
#define tpp_lcinfo_init_invalid(p_self) (void)(*(p_self) = TPP_LCINFO_INVALID)
#endif /* !tpp_lcinfo_init_invalid */



/* Non-atomic reference counter API */
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



/* Atomic reference counter API */
#ifndef tpp_refcnt_atomic
/* WARNING: Multi-threaded applications must override this: this kind of
 *          reference counter is used in places where the linked component
 *          may be shared between multiple lexers (and thus: threads) */
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



/* Execute-once block */
#ifndef tpp_once
/* WARNING: Multi-threaded applications must override this */
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
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_API_H */
