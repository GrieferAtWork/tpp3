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
#ifndef GUARD_TPP2_FORWARD_H
#define GUARD_TPP2_FORWARD_H 1

/* Forward-compatibility header **FOR** TPP2 that adds TPP3-style APIs implement on-top of TPP2's API
 *
 * Same as `tpp2.h`, this header is meant to make it easier to transition from TPP2 to TPP3 */

#include "deprecated-tpp2/tpp.h"

#ifdef __cplusplus
#define TPP_DECL_BEGIN extern "C" {
#define TPP_DECL_END   }
#else /* __cplusplus */
#define TPP_DECL_BEGIN /* nothing */
#define TPP_DECL_END   /* nothing */
#endif /* !__cplusplus */

TPP_DECL_BEGIN

/************************************************************************/
/* "parts/api.h"                                                        */
/************************************************************************/

#ifndef TPP_USE_STATIC
#define TPP_USE_STATIC 0
#endif /* !TPP_USE_STATIC */

#ifndef tpp_memcmp
#define tpp_memcmp memcmp
#endif /* !tpp_memcmp */

#ifndef tpp_offsetof
#define tpp_offsetof offsetof
#endif /* !tpp_offsetof */

#ifndef tpp_container_of
#define tpp_container_of(ptr, type, member) \
	((type *)((char *)(ptr) - tpp_offsetof(type, member)))
#endif /* !tpp_container_of */

#ifdef TPP
#define TPP_FORWARD_COMPAT TPP
#else /* TPP */
#define TPP_FORWARD_COMPAT(x) TPP_##x
#endif /* !TPP */

#ifndef tpp_restrict
#define tpp_restrict __restrict
#endif /* !tpp_restrict */

#ifndef TPP_CHAR_BIT
#ifdef CHAR_BIT
#define TPP_CHAR_BIT CHAR_BIT
#elif defined(__CHAR_BIT__)
#define TPP_CHAR_BIT __CHAR_BIT__
#else /* ... */
#define TPP_CHAR_BIT 8
#endif /* !... */
#endif /* !TPP_CHAR_BIT */

#ifndef TPP_HAVE_TPP2_COMPAT
#define TPP_HAVE_TPP2_COMPAT 1
#endif /* !TPP_HAVE_TPP2_COMPAT */

#ifndef TPP_DEBUG
#define TPP_DEBUG TPP_CONFIG_DEBUG
#endif /* !TPP_DEBUG */

#ifndef TPP_INLINE
#define TPP_INLINE TPP_LOCAL
#endif /* !TPP_INLINE */

#ifndef tpp_uint_least8
#ifdef UINT_LEAST8_MAX
#define tpp_int_least8  int_least8_t
#define tpp_uint_least8 uint_least8_t
#define TPP_UINT_LEAST8_MAX UINT_LEAST8_MAX
#ifdef UINT8_C
#define TPP_UINT_LEAST8_C(x) UINT8_C(x)
#endif /* UINT8_C */
#else /* UINT_LEAST8_MAX */
#define tpp_int_least8  signed char
#define tpp_uint_least8 unsigned char
#define TPP_UINT_LEAST8_MAX 0xff
#endif /* !UINT_LEAST8_MAX */
#endif /* !tpp_uint_least8 */
#ifndef TPP_UINT_LEAST8_C
#define TPP_UINT_LEAST8_C(x) x
#endif /* !TPP_UINT_LEAST8_C */

#ifndef tpp_uint_least16
#ifdef UINT_LEAST16_MAX
#define tpp_int_least16  int_least16_t
#define tpp_uint_least16 uint_least16_t
#define TPP_UINT_LEAST16_MAX UINT_LEAST16_MAX
#ifdef UINT16_C
#define TPP_UINT_LEAST16_C(x) UINT16_C(x)
#endif /* UINT16_C */
#else /* UINT_LEAST16_MAX */
#define tpp_int_least16  signed short
#define tpp_uint_least16 unsigned short
#define TPP_UINT_LEAST16_MAX 0xffff
#endif /* !UINT_LEAST16_MAX */
#endif /* !tpp_uint_least16 */
#ifndef TPP_UINT_LEAST16_C
#define TPP_UINT_LEAST16_C(x) x
#endif /* !TPP_UINT_LEAST16_C */

#ifndef tpp_uint_least32
#ifdef UINT_LEAST32_MAX
#define tpp_int_least32  int_least32_t
#define tpp_uint_least32 uint_least32_t
#define TPP_UINT_LEAST32_MAX UINT_LEAST32_MAX
#ifdef UINT32_C
#define TPP_UINT_LEAST32_C UINT32_C
#endif /* UINT32_C */
#else /* UINT_LEAST32_MAX */
#define tpp_int_least32  signed int
#define tpp_uint_least32 unsigned int
#define TPP_UINT_LEAST32_MAX 0xffffffffu
#define TPP_UINT_LEAST32_C(x) x##u
#endif /* !UINT_LEAST32_MAX */
#endif /* !tpp_uint_least32 */
#ifndef TPP_UINT_LEAST32_C
#define TPP_UINT_LEAST32_C(x) x
#endif /* !TPP_UINT_LEAST32_C */

#ifndef tpp_uint_least64
#ifdef UINT_LEAST64_MAX
#define tpp_int_least64  int_least64_t
#define tpp_uint_least64 uint_least64_t
#define TPP_UINT_LEAST64_MAX UINT_LEAST64_MAX
#ifdef UINT64_C
#define TPP_UINT_LEAST64_C(x) UINT64_C(x)
#endif /* UINT64_C */
#endif /* !UINT_LEAST64_MAX */
#endif /* !tpp_uint_least64 */
#ifndef TPP_UINT_LEAST64_C
#define TPP_UINT_LEAST64_C(x) x
#endif /* !TPP_UINT_LEAST64_C */

#ifndef tpp_uint_fast8
#ifdef UINT_FAST8_MAX
#define tpp_int_fast8      int_fast8_t
#define tpp_uint_fast8     uint_fast8_t
#define TPP_UINT_FAST8_MAX UINT_FAST8_MAX
#ifdef UINT8_C
#define TPP_UINT_FAST8_C(x) UINT8_C(x)
#endif /* UINT8_C */
#else /* UINT_FAST8_MAX */
#define tpp_int_fast8       tpp_int_least8
#define tpp_uint_fast8      tpp_uint_least8
#define TPP_UINT_FAST8_MAX  TPP_UINT_LEAST8_MAX
#define TPP_UINT_FAST8_C(x) TPP_UINT_LEAST8_C(x)
#endif /* !UINT_FAST8_MAX */
#endif /* !tpp_uint_fast8 */
#ifndef TPP_UINT_FAST8_C
#define TPP_UINT_FAST8_C(x) TPP_UINT_LEAST8_C(x)
#endif /* !TPP_UINT_FAST8_C */

#ifndef tpp_uint_fast16
#ifdef UINT_FAST16_MAX
#define tpp_int_fast16      int_fast16_t
#define tpp_uint_fast16     uint_fast16_t
#define TPP_UINT_FAST16_MAX UINT_FAST16_MAX
#ifdef UINT16_C
#define TPP_UINT_FAST16_C(x) UINT16_C(x)
#endif /* UINT16_C */
#else /* UINT_FAST16_MAX */
#define tpp_int_fast16       tpp_int_least16
#define tpp_uint_fast16      tpp_uint_least16
#define TPP_UINT_FAST16_MAX  TPP_UINT_LEAST16_MAX
#define TPP_UINT_FAST16_C(x) TPP_UINT_LEAST16_C(x)
#endif /* !UINT_FAST16_MAX */
#endif /* !tpp_uint_fast16 */
#ifndef TPP_UINT_FAST16_C
#define TPP_UINT_FAST16_C(x) TPP_UINT_LEAST16_C(x)
#endif /* !TPP_UINT_FAST16_C */

#ifndef tpp_uint_fast32
#ifdef UINT_FAST32_MAX
#define tpp_int_fast32      int_fast32_t
#define tpp_uint_fast32     uint_fast32_t
#define TPP_UINT_FAST32_MAX UINT_FAST32_MAX
#ifdef UINT32_C
#define TPP_UINT_FAST32_C(x) UINT32_C(x)
#endif /* UINT32_C */
#else /* UINT_FAST32_MAX */
#define tpp_int_fast32       tpp_int_least32
#define tpp_uint_fast32      tpp_uint_least32
#define TPP_UINT_FAST32_MAX  TPP_UINT_LEAST32_MAX
#define TPP_UINT_FAST32_C(x) TPP_UINT_LEAST32_C(x)
#endif /* !UINT_FAST32_MAX */
#endif /* !tpp_uint_fast32 */
#ifndef TPP_UINT_FAST32_C
#define TPP_UINT_FAST32_C(x) TPP_UINT_LEAST32_C(x)
#endif /* !TPP_UINT_FAST32_C */

#ifndef tpp_intmax
#define tpp_intmax      TPP_FORWARD_COMPAT(tint_t)
#define tpp_uintmax     TPP_FORWARD_COMPAT(tint_t)
#define TPP_UINTMAX_MAX UINT_LEAST64_MAX
#define TPP_UINTMAX_C   UINT64_C
#endif /* !tpp_intmax */

#ifndef tpp_size
#define tpp_size     size_t
#define TPP_SIZE_MAX SIZE_MAX
#endif /* !tpp_size */

#ifndef tpp_ssize
#define tpp_ssize     ptrdiff_t
#define TPP_SSIZE_MAX PTRDIFF_MAX
#endif /* !tpp_ssize */

#ifndef tpp_hash
#define tpp_hash      TPP_FORWARD_COMPAT(hash_t)
#define TPP_HASH_MAX  TPP_SIZE_MAX
#if TPP_SIZE_MAX <= TPP_UINT_LEAST16_MAX
#define TPP_HASH_C(x) TPP_UINT_LEAST16_C(x)
#elif TPP_SIZE_MAX <= TPP_UINT_LEAST32_MAX
#define TPP_HASH_C(x) TPP_UINT_LEAST32_C(x)
#else /* ... */
#define TPP_HASH_C(x) TPP_UINT_LEAST64_C(x)
#endif /* !... */
#endif /* !tpp_hash */

#ifndef tpp_line
#define tpp_line   TPP_FORWARD_COMPAT(line_t)
#define tpp_column TPP_FORWARD_COMPAT(col_t)
#endif /* !tpp_line */

#ifndef tpp_char
#define tpp_char unsigned char
#endif /* !tpp_char */

#ifndef tpp_unichar
#define tpp_unichar   tpp_uint_least32
#define TPP_UNICHAR_C TPP_UINT_LEAST32_C
#endif /* !tpp_unichar */

#ifndef tpp_counter
#define tpp_counter TPP_FORWARD_COMPAT(tint_t)
#endif /* !tpp_counter */

#ifndef tpp_float
#define tpp_float TPP_FORWARD_COMPAT(tfloat_t)
#endif /* !tpp_float */

#ifndef TPP_REF
#define TPP_REF /* nothing */
#endif /* !TPP_REF */

#ifndef TPP_STATIC_ASSERT
#define _TPP_STATIC_ASSERT_ID2(line) tpp_static_assert_##line
#define _TPP_STATIC_ASSERT_ID(line) _TPP_STATIC_ASSERT_ID2(line)
#define TPP_STATIC_ASSERT(expr) typedef int _TPP_STATIC_ASSERT_ID(__LINE__)[(expr) ? 1 : -1]
#endif /* !TPP_STATIC_ASSERT */
#ifndef TPP_STATIC_ASSERT_MSG
#define TPP_STATIC_ASSERT_MSG(expr, msg) TPP_STATIC_ASSERT(expr)
#endif /* !TPP_STATIC_ASSERT_MSG */


/* Format-printer API */
#ifndef tpp_formatprinter
#define tpp_formatprinter TPP_FORWARD_COMPAT(printer_t)
#define tpp_formatprinter_print(printer, arg, text, num_bytes) \
	((*printer)(arg, (char const *)(text), num_bytes))
#define tpp_formatprinter_print_byname(NAME, arg, text, num_bytes) \
	NAME(arg, (char const *)(text), num_bytes)
#define tpp_formatprinter_of(NAME) (&NAME)
#define TPP_FORMATPRINTER_DEFINE(NAME, arg, text, num_bytes) \
	static ptrdiff_t (TPPCALL NAME)(void *closure, char const *tpp_restrict buf, size_t bufsize)
#if !TPP_USE_STATIC
#define TPP_FORMATPRINTER_IMPL(NAME, arg, text, num_bytes) \
	TPP_IMPL ptrdiff_t (TPPCALL NAME)(void *closure, char const *tpp_restrict buf, size_t bufsize)
#define TPP_FORMATPRINTER_DECL(NAME) \
	TPP_DECL ptrdiff_t (TPPCALL NAME)(void *closure, char const *tpp_restrict buf, size_t bufsize)
#endif /* !TPP_USE_STATIC */
#endif /* !tpp_formatprinter */

#if TPP_USE_STATIC
#undef TPP_FORMATPRINTER_DECL
#undef TPP_FORMATPRINTER_IMPL
#define TPP_FORMATPRINTER_IMPL TPP_FORMATPRINTER_DEFINE
#endif /* TPP_USE_STATIC */

#ifndef tpp_formatprinter_print_byname
#define tpp_formatprinter_print_byname(NAME, arg, text, num_bytes) \
	tpp_formatprinter_print(tpp_formatprinter_of(NAME), arg, text, num_bytes)
#endif /* !tpp_formatprinter_print_byname */

#ifndef tpp_formatprinter_print_cstr
#define tpp_formatprinter_print_cstr_IS_DEFAULT
#define tpp_formatprinter_print_cstr(printer, arg, text, num_bytes) \
	tpp_formatprinter_print(printer, arg, (tpp_char const *)(text), num_bytes)
#endif /* !tpp_formatprinter_print_cstr */
#ifndef tpp_formatprinter_print_conststr
#define tpp_formatprinter_print_conststr(printer, arg, CONSTstr) \
	tpp_formatprinter_print_cstr(printer, arg, CONSTstr, sizeof(CONSTstr) - sizeof(char))
#endif /* !tpp_formatprinter_print_conststr */



/* Line/Column-information API */
#ifndef tpp_lcinfo
#define tpp_lcinfo struct TPPLCInfo
#define tpp_lcinfo_getline(self) ((self).lc_line)
#define tpp_lcinfo_getcol(self)  ((self).lc_col)
#define tpp_lcinfo_init(p_self, line, col) \
	(void)((p_self)->lc_line = (line),     \
	       (p_self)->lc_col  = (col))
#define tpp_lcinfo_setline(p_self, line) (void)((p_self)->lc_line = (line))
#define tpp_lcinfo_setcol(p_self, col)   (void)((p_self)->lc_col = (col))

TPP_INLINE /*TPP_WUNUSED*/ tpp_lcinfo TPPCALL
tpp_lcinfo_of(tpp_line line, tpp_column col) {
	tpp_lcinfo result;
	result.lc_line = line;
	result.lc_col  = col;
	return result;
}
#endif /* !tpp_lcinfo */

#ifndef tpp_lcinfo_init
#define tpp_lcinfo_init(p_self, line, col) \
	(void)(*(p_self) = tpp_lcinfo_of(line, col))
#endif /* !tpp_lcinfo_init */
#ifndef tpp_lcinfo_setline
#define tpp_lcinfo_setline(p_self, line) tpp_lcinfo_init(p_self, line, tpp_lcinfo_getcol(*(p_self)))
#endif /* !tpp_lcinfo_setline */
#ifndef tpp_lcinfo_setcol
#define tpp_lcinfo_setcol(p_self, col) tpp_lcinfo_init(p_self, tpp_lcinfo_getline(*(p_self)), col)
#endif /* !tpp_lcinfo_setcol */
#ifndef tpp_lcinfo_equals
#define tpp_lcinfo_equals(a, b)                        \
	(tpp_lcinfo_getline(a) == tpp_lcinfo_getline(b) && \
	 tpp_lcinfo_getcol(a) == tpp_lcinfo_getcol(b))
#endif /* !tpp_lcinfo_equals */

/* Specifies an invalid LC information object */
#ifndef TPP_LCINFO_INVALID
#define TPP_LCINFO_INVALID              tpp_lcinfo_of(-1, -1)
#define tpp_lcinfo_isvalid(self)        (tpp_lcinfo_getcol(self) >= 0)
#define tpp_lcinfo_init_invalid(p_self) tpp_lcinfo_init(p_self, -1, -1)
#endif /* !TPP_LCINFO_INVALID */

/* Check if "x" represents valid line/column information */
#ifndef tpp_lcinfo_isvalid
#define tpp_lcinfo_isvalid(x) (!tpp_lcinfo_equals(x, TPP_LCINFO_INVALID))
#endif /* !tpp_lcinfo_isvalid */
#ifndef tpp_lcinfo_init_invalid
#define tpp_lcinfo_init_invalid(p_self) (void)(*(p_self) = TPP_LCINFO_INVALID)
#endif /* !tpp_lcinfo_init_invalid */



/************************************************************************/
/* "parts/errno.h"                                                      */
/************************************************************************/

#define tpp_errno       int
#define TPP_EOK         0    /* NO_ERROR */
#define TPP_ENOMEM      (-1) /* HARD_ERROR */
#define TPP_EIO         (-1) /* HARD_ERROR */
#define TPP_ELEXERROR   (-1) /* HARD_ERROR */
#define TPP_EWOULDBLOCK (-2) /* SOFT_ERROR */
#define TPP_ENOENT      (-2) /* SOFT_ERROR */
#define TPP_EMASKED     (-2) /* SOFT_ERROR */

#define TPP_ISERR(x) ((x) < 0)

#define /*tpp_ssize*/ TPP_SSIZE_OFERR(/*tpp_errno*/ e) ((tpp_ssize)(int)(e))
#define /*tpp_errno*/ TPP_SSIZE_ASERR(/*tpp_ssize*/ v) ((tpp_errno)(int)(v))
#define /*tpp_ssize*/ TPP_SSIZE_OFERR_OR_EOK(/*tpp_errno*/ e) ((tpp_ssize)(int)(e))
#define /*tpp_errno*/ TPP_SSIZE_ASERR_OR_EOK(/*tpp_ssize*/ v) ((tpp_errno)(int)(v))
#define /*bool*/ TPP_SSIZE_ISERR(/*tpp_ssize*/ v) ((v) < 0)
#define /*bool*/ TPP_SSIZE_ISERR_OR_EOK(/*tpp_ssize*/ v) ((v) <= 0)




/************************************************************************/
/* "parts/string.h"                                                     */
/************************************************************************/
#define tpp_string                     struct TPPString
#define TPP_STRING_DEFINE(name, value) TPPSTRING_DEF(name, value)
#define tpp_string_len(self)           TPPString_SIZE(self)
#define tpp_string_str(self)           ((tpp_char *)TPPString_TEXT(self))
#define tpp_string_end(self)           (tpp_string_str(self) + tpp_string_len(self))
#define tpp_string_cstr(self)          TPPString_TEXT(self)
#define tpp_string_equals(lhs, rhs)                       \
	(tpp_string_len(lhs) == tpp_string_len(rhs) &&        \
	 tpp_memcmp(tpp_string_str(lhs), tpp_string_str(rhs), \
	            tpp_string_len(lhs) * sizeof(tpp_char)) == 0)
#define tpp_string_equals_conststr(lhs, rhs_CONSTstr)                \
	(tpp_string_len(lhs) == (sizeof(rhs_CONSTstr) - sizeof(char)) && \
	 tpp_memcmp(tpp_string_str(lhs), rhs_CONSTstr,                   \
	            (sizeof(rhs_CONSTstr) - sizeof(char)) * sizeof(tpp_char)) == 0)
#define tpp_string_destroy(self)       TPPString_Free(self)
#define tpp_string_incref(self)        TPPString_Incref(self)
#define tpp_string_isshared(self)      TPPString_Shared(self)
#define tpp_string_decref(self)        TPPString_Decref(self)
#define tpp_string_decref_nokill(self) (void)(--(x)->s_refcnt)
#define tpp_string_malloc(len) TPPString_NewSized(len)
/*#define tpp_string_trymalloc(len) TPPString_NewSized(len)*/
#define tpp_string_newempty() TPPString_NewEmpty()



/************************************************************************/
/* "parts/token-aliases.h"                                              */
/************************************************************************/

/*[[[deemon
import * from deemon;
global currentCondition = "1";
function alias(nameWithoutNsPrefix: string, condition: string = "1") {
	if (condition != currentCondition) {
		if (currentCondition != "1")
			print("#endif /" "* ... *" "/");
		if (condition != "1")
			print("#if ", condition);
		currentCondition = condition;
	}
	print("#define TPP_", nameWithoutNsPrefix, " TPP_FORWARD_COMPAT(", nameWithoutNsPrefix, ")");
}
alias("KWD_if", "!defined(TPP_CONFIG_USERDEFINED_KWD_IF) && !defined(TPP_CONFIG_USERDEFINED_KWD_if)");
alias("KWD_ifdef", "!defined(TPP_CONFIG_USERDEFINED_KWD_IFDEF) && !defined(TPP_CONFIG_USERDEFINED_KWD_ifdef)");
alias("KWD_ifndef", "!defined(TPP_CONFIG_USERDEFINED_KWD_IFNDEF) && !defined(TPP_CONFIG_USERDEFINED_KWD_ifndef)");
alias("KWD_elif", "!defined(TPP_CONFIG_USERDEFINED_KWD_ELIF) && !defined(TPP_CONFIG_USERDEFINED_KWD_elif)");
alias("KWD_else", "!defined(TPP_CONFIG_USERDEFINED_KWD_ELSE) && !defined(TPP_CONFIG_USERDEFINED_KWD_else)");
alias("KWD_endif", "!defined(TPP_CONFIG_USERDEFINED_KWD_ENDIF) && !defined(TPP_CONFIG_USERDEFINED_KWD_endif)");
alias("KWD_define", "!defined(TPP_CONFIG_USERDEFINED_KWD_DEFINE) && !defined(TPP_CONFIG_USERDEFINED_KWD_define)");
alias("KWD_defined", "!defined(TPP_CONFIG_USERDEFINED_KWD_DEFINED) && !defined(TPP_CONFIG_USERDEFINED_KWD_defined)");
alias("KWD_undef", "!defined(TPP_CONFIG_USERDEFINED_KWD_UNDEF) && !defined(TPP_CONFIG_USERDEFINED_KWD_undef)");
alias("KWD_include", "!defined(TPP_CONFIG_USERDEFINED_KWD_INCLUDE) && !defined(TPP_CONFIG_USERDEFINED_KWD_include)");
alias("KWD_include_next", "(!defined(TPP_CONFIG_EXTENSION_INCLUDE_NEXT) || TPP_CONFIG_EXTENSION_INCLUDE_NEXT) && !defined(TPP_CONFIG_USERDEFINED_KWD_INCLUDE_NEXT) && !defined(TPP_CONFIG_USERDEFINED_KWD_include_next)");
alias("KWD_import", "(!defined(TPP_CONFIG_EXTENSION_IMPORT) || TPP_CONFIG_EXTENSION_IMPORT) && !defined(TPP_CONFIG_USERDEFINED_KWD_IMPORT) && !defined(TPP_CONFIG_USERDEFINED_KWD_import)");
alias("KWD_line", "!defined(TPP_CONFIG_USERDEFINED_KWD_LINE) && !defined(TPP_CONFIG_USERDEFINED_KWD_line)");
alias("KWD_error", "!defined(TPP_CONFIG_USERDEFINED_KWD_ERROR) && !defined(TPP_CONFIG_USERDEFINED_KWD_error)");
alias("KWD_warning", "!defined(TPP_CONFIG_USERDEFINED_KWD_WARNING) && !defined(TPP_CONFIG_USERDEFINED_KWD_warning)");
alias("KWD_ident", "(!defined(TPP_CONFIG_EXTENSION_IDENT_SCCS) || TPP_CONFIG_EXTENSION_IDENT_SCCS) && !defined(TPP_CONFIG_USERDEFINED_KWD_IDENT) && !defined(TPP_CONFIG_USERDEFINED_KWD_ident)");
alias("KWD_sccs", "(!defined(TPP_CONFIG_EXTENSION_IDENT_SCCS) || TPP_CONFIG_EXTENSION_IDENT_SCCS) && !defined(TPP_CONFIG_USERDEFINED_KWD_SCCS) && !defined(TPP_CONFIG_USERDEFINED_KWD_sccs)");
alias("KWD_assert", "(!defined(TPP_CONFIG_EXTENSION_ASSERTIONS) || TPP_CONFIG_EXTENSION_ASSERTIONS) && !defined(TPP_CONFIG_USERDEFINED_KWD_ASSERT) && !defined(TPP_CONFIG_USERDEFINED_KWD_assert)");
alias("KWD_unassert", "(!defined(TPP_CONFIG_EXTENSION_ASSERTIONS) || TPP_CONFIG_EXTENSION_ASSERTIONS) && !defined(TPP_CONFIG_USERDEFINED_KWD_UNASSERT) && !defined(TPP_CONFIG_USERDEFINED_KWD_unassert)");
alias("KWD_pragma", "!defined(TPP_CONFIG_USERDEFINED_KWD_PRAGMA) && !defined(TPP_CONFIG_USERDEFINED_KWD_pragma)");
alias("KWD__Pragma", "!defined(TPP_CONFIG_USERDEFINED_KWD__PRAGMA) && !defined(TPP_CONFIG_USERDEFINED_KWD__Pragma)");
alias("KWD___pragma", "(!defined(TPP_CONFIG_EXTENSION_MSVC_PRAGMA) || TPP_CONFIG_EXTENSION_MSVC_PRAGMA) && !defined(TPP_CONFIG_USERDEFINED_KWD___PRAGMA) && !defined(TPP_CONFIG_USERDEFINED_KWD___pragma)");
alias("KWD___FILE__");
alias("KWD___LINE__");
alias("KWD___TIME__");
alias("KWD___DATE__");
alias("KWD___BASE_FILE__", "!defined(TPP_CONFIG_EXTENSION_BASEFILE) || TPP_CONFIG_EXTENSION_BASEFILE");
alias("KWD___INCLUDE_LEVEL__", "!defined(TPP_CONFIG_EXTENSION_INCLUDE_LEVEL) || TPP_CONFIG_EXTENSION_INCLUDE_LEVEL");
alias("KWD___INCLUDE_DEPTH__", "!defined(TPP_CONFIG_EXTENSION_INCLUDE_LEVEL) || TPP_CONFIG_EXTENSION_INCLUDE_LEVEL");
alias("KWD___COUNTER__", "!defined(TPP_CONFIG_EXTENSION_COUNTER) || TPP_CONFIG_EXTENSION_COUNTER");
alias("KWD___TIMESTAMP__", "!defined(TPP_CONFIG_EXTENSION_TIMESTAMP) || TPP_CONFIG_EXTENSION_TIMESTAMP");
alias("KWD___COLUMN__", "!defined(TPP_CONFIG_EXTENSION_COLUMN) || TPP_CONFIG_EXTENSION_COLUMN");
alias("KWD___is_identifier", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___is_builtin_identifier", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___is_deprecated", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___is_poisoned", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_attribute", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_builtin", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_tpp_builtin", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_cpp_attribute", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_declspec_attribute", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_feature", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_extension", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_warning", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_known_extension", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_known_warning", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES");
alias("KWD___has_include", "!defined(TPP_CONFIG_EXTENSION_HAS_INCLUDE) || TPP_CONFIG_EXTENSION_HAS_INCLUDE");
alias("KWD___has_include_next", "!defined(TPP_CONFIG_EXTENSION_HAS_INCLUDE) || TPP_CONFIG_EXTENSION_HAS_INCLUDE");
alias("KWD___VA_ARGS__", "!defined(TPP_CONFIG_EXTENSION_VA_ARGS) || TPP_CONFIG_EXTENSION_VA_ARGS");
alias("KWD___VA_COMMA__", "!defined(TPP_CONFIG_EXTENSION_VA_COMMA) || TPP_CONFIG_EXTENSION_VA_COMMA");
alias("KWD___VA_NARGS__", "!defined(TPP_CONFIG_EXTENSION_VA_NARGS) || TPP_CONFIG_EXTENSION_VA_NARGS");
alias("KWD___VA_OPT__", "!defined(TPP_CONFIG_EXTENSION_VA_OPT) || TPP_CONFIG_EXTENSION_VA_OPT");
alias("KWD___TPP_EVAL", "!defined(TPP_CONFIG_EXTENSION_TPP_EVAL) || TPP_CONFIG_EXTENSION_TPP_EVAL");
alias("KWD___TPP_LOAD_FILE", "!defined(TPP_CONFIG_EXTENSION_TPP_LOAD_FILE) || TPP_CONFIG_EXTENSION_TPP_LOAD_FILE");
alias("KWD___TPP_COUNTER", "!defined(TPP_CONFIG_EXTENSION_TPP_COUNTER) || TPP_CONFIG_EXTENSION_TPP_COUNTER");
alias("KWD___TPP_RANDOM", "!defined(TPP_CONFIG_EXTENSION_TPP_RANDOM) || TPP_CONFIG_EXTENSION_TPP_RANDOM");
alias("KWD___TPP_STR_DECOMPILE", "!defined(TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE) || TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE");
alias("KWD___TPP_STR_AT", "!defined(TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR) || TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR");
alias("KWD___TPP_STR_SUBSTR", "!defined(TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR) || TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR");
alias("KWD___TPP_STR_PACK", "!defined(TPP_CONFIG_EXTENSION_TPP_STR_PACK) || TPP_CONFIG_EXTENSION_TPP_STR_PACK");
alias("KWD___TPP_STR_SIZE", "!defined(TPP_CONFIG_EXTENSION_TPP_STR_SIZE) || TPP_CONFIG_EXTENSION_TPP_STR_SIZE");
alias("KWD___TPP_UNIQUE", "!defined(TPP_CONFIG_EXTENSION_TPP_UNIQUE) || TPP_CONFIG_EXTENSION_TPP_UNIQUE");
alias("KWD___TPP_COUNT_TOKENS", "!defined(TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS) || TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS");
alias("KWD___TPP_IDENTIFIER", "!defined(TPP_CONFIG_EXTENSION_TPP_IDENTIFIER) || TPP_CONFIG_EXTENSION_TPP_IDENTIFIER");
alias("KWD___DATE_DAY__", "!defined(TPP_CONFIG_EXTENSION_DATEUTILS) || TPP_CONFIG_EXTENSION_DATEUTILS");
alias("KWD___DATE_WDAY__", "!defined(TPP_CONFIG_EXTENSION_DATEUTILS) || TPP_CONFIG_EXTENSION_DATEUTILS");
alias("KWD___DATE_YDAY__", "!defined(TPP_CONFIG_EXTENSION_DATEUTILS) || TPP_CONFIG_EXTENSION_DATEUTILS");
alias("KWD___DATE_MONTH__", "!defined(TPP_CONFIG_EXTENSION_DATEUTILS) || TPP_CONFIG_EXTENSION_DATEUTILS");
alias("KWD___DATE_YEAR__", "!defined(TPP_CONFIG_EXTENSION_DATEUTILS) || TPP_CONFIG_EXTENSION_DATEUTILS");
alias("KWD___TIME_SEC__", "!defined(TPP_CONFIG_EXTENSION_TIMEUTILS) || TPP_CONFIG_EXTENSION_TIMEUTILS");
alias("KWD___TIME_MIN__", "!defined(TPP_CONFIG_EXTENSION_TIMEUTILS) || TPP_CONFIG_EXTENSION_TIMEUTILS");
alias("KWD___TIME_HOUR__", "!defined(TPP_CONFIG_EXTENSION_TIMEUTILS) || TPP_CONFIG_EXTENSION_TIMEUTILS");
alias("KWD_tpp_dollar_is_alpha", "!defined(TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA) || TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA");
alias("KWD_tpp_va_args", "!defined(TPP_CONFIG_EXTENSION_VA_ARGS) || TPP_CONFIG_EXTENSION_VA_ARGS");
alias("KWD_tpp_named_va_args", "!defined(TPP_CONFIG_EXTENSION_GCC_VA_ARGS) || TPP_CONFIG_EXTENSION_GCC_VA_ARGS");
alias("KWD_tpp_va_comma", "!defined(TPP_CONFIG_EXTENSION_VA_COMMA) || TPP_CONFIG_EXTENSION_VA_COMMA");
alias("KWD_tpp_msvc_integer_suffix", "!defined(TPP_CONFIG_EXTENSION_MSVC_FIXED_INT) || TPP_CONFIG_EXTENSION_MSVC_FIXED_INT");
alias("KWD_tpp_charize_operator", "!defined(TPP_CONFIG_EXTENSION_HASH_AT) || TPP_CONFIG_EXTENSION_HASH_AT");
alias("KWD_tpp_trigraphs", "!defined(TPP_CONFIG_FEATURE_TRIGRAPHS) || TPP_CONFIG_FEATURE_TRIGRAPHS");
alias("KWD_tpp_digraphs", "!defined(TPP_CONFIG_FEATURE_DIGRAPHS) || TPP_CONFIG_FEATURE_DIGRAPHS");
alias("KWD_tpp_reemit_unknown_pragmas");
alias("KWD_tpp_pragma_push_macro");
alias("KWD_tpp_pragma_pop_macro");
alias("KWD_tpp_pragma_region");
alias("KWD_tpp_pragma_endregion");
alias("KWD_tpp_pragma_warning");
alias("KWD_tpp_pragma_message");
alias("KWD_tpp_pragma_error");
alias("KWD_tpp_pragma_once");
alias("KWD_tpp_pragma_tpp_exec");
alias("KWD_tpp_pragma_deprecated");
alias("KWD_tpp_pragma_tpp_set_keyword_flags");
alias("KWD_tpp_directive_include_next", "!defined(TPP_CONFIG_EXTENSION_INCLUDE_NEXT) || TPP_CONFIG_EXTENSION_INCLUDE_NEXT");
alias("KWD_tpp_directive_import", "!defined(TPP_CONFIG_EXTENSION_IMPORT) || TPP_CONFIG_EXTENSION_IMPORT");
alias("KWD_tpp_directive_warning", "!defined(TPP_CONFIG_EXTENSION_WARNING) || TPP_CONFIG_EXTENSION_WARNING");
alias("KWD_tpp_lxor", "!defined(TPP_CONFIG_EXTENSION_LXOR) || TPP_CONFIG_EXTENSION_LXOR");
alias("KWD_tpp_token_tilde_tilde");
alias("KWD_tpp_token_pow");
alias("KWD_tpp_token_lxor");
alias("KWD_tpp_token_arrow");
alias("KWD_tpp_token_colon_assign");
alias("KWD_tpp_token_colon_colon");
alias("KWD_tpp_macro_calling_conventions", "!defined(TPP_CONFIG_EXTENSION_ALTMAC) || TPP_CONFIG_EXTENSION_ALTMAC");
alias("KWD_tpp_strict_whitespace");
alias("KWD_tpp_strict_integer_overflow");
alias("KWD_tpp_support_ansi_characters");
alias("KWD_tpp_emit_lf_after_directive");
alias("KWD_tpp_if_cond_expression", "!defined(TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR) || TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR");
alias("KWD_tpp_debug");
alias("KWD___TPP_VERSION__");
alias("EXT_TRIGRAPHS", "!defined(TPP_CONFIG_FEATURE_TRIGRAPHS)");
alias("EXT_DIGRAPHS", "!defined(TPP_CONFIG_FEATURE_DIGRAPHS)");
alias("EXT_GCC_VA_ARGS", "!defined(TPP_CONFIG_EXTENSION_GCC_VA_ARGS)");
alias("EXT_GCC_VA_COMMA", "!defined(TPP_CONFIG_EXTENSION_GCC_VA_COMMA)");
alias("EXT_GCC_IFELSE", "!defined(TPP_CONFIG_EXTENSION_GCC_IFELSE)");
alias("EXT_VA_COMMA", "!defined(TPP_CONFIG_EXTENSION_VA_COMMA)");
alias("EXT_VA_OPT", "!defined(TPP_CONFIG_EXTENSION_VA_OPT)");
alias("EXT_VA_NARGS", "!defined(TPP_CONFIG_EXTENSION_VA_NARGS)");
alias("EXT_VA_ARGS", "!defined(TPP_CONFIG_EXTENSION_VA_ARGS)");
alias("EXT_STR_E", "!defined(TPP_CONFIG_EXTENSION_STR_E)");
alias("EXT_ALTMAC", "!defined(TPP_CONFIG_EXTENSION_ALTMAC)");
alias("EXT_RECMAC", "!defined(TPP_CONFIG_EXTENSION_RECMAC)");
alias("EXT_ARGSPACE", "!defined(TPP_CONFIG_EXTENSION_ARGSPACE)");
alias("EXT_BININTEGRAL", "!defined(TPP_CONFIG_EXTENSION_BININTEGRAL)");
alias("EXT_MSVC_PRAGMA", "!defined(TPP_CONFIG_EXTENSION_MSVC_PRAGMA)");
alias("EXT_STRINGOPS", "!defined(TPP_CONFIG_EXTENSION_STRINGOPS)");
alias("EXT_HASH_AT", "!defined(TPP_CONFIG_EXTENSION_HASH_AT)");
alias("EXT_HASH_XCLAIM", "!defined(TPP_CONFIG_EXTENSION_HASH_XCLAIM)");
alias("EXT_WARNING", "!defined(TPP_CONFIG_EXTENSION_WARNING)");
alias("EXT_SHEBANG", "!defined(TPP_CONFIG_EXTENSION_SHEBANG)");
alias("EXT_INCLUDE_NEXT", "!defined(TPP_CONFIG_EXTENSION_INCLUDE_NEXT)");
alias("EXT_IMPORT", "!defined(TPP_CONFIG_EXTENSION_IMPORT)");
alias("EXT_IDENT_SCCS", "!defined(TPP_CONFIG_EXTENSION_IDENT_SCCS)");
alias("EXT_BASEFILE", "!defined(TPP_CONFIG_EXTENSION_BASEFILE)");
alias("EXT_INCLUDE_LEVEL", "!defined(TPP_CONFIG_EXTENSION_INCLUDE_LEVEL)");
alias("EXT_COUNTER", "!defined(TPP_CONFIG_EXTENSION_COUNTER)");
alias("EXT_CLANG_FEATURES", "!defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES)");
alias("EXT_HAS_INCLUDE", "!defined(TPP_CONFIG_EXTENSION_HAS_INCLUDE)");
alias("EXT_LXOR", "!defined(TPP_CONFIG_EXTENSION_LXOR)");
alias("EXT_MULTICHAR_CONST", "!defined(TPP_CONFIG_EXTENSION_MULTICHAR_CONST)");
alias("EXT_DATEUTILS", "!defined(TPP_CONFIG_EXTENSION_DATEUTILS)");
alias("EXT_TIMEUTILS", "!defined(TPP_CONFIG_EXTENSION_TIMEUTILS)");
alias("EXT_TIMESTAMP", "!defined(TPP_CONFIG_EXTENSION_TIMESTAMP)");
alias("EXT_COLUMN", "!defined(TPP_CONFIG_EXTENSION_COLUMN)");
alias("EXT_TPP_EVAL", "!defined(TPP_CONFIG_EXTENSION_TPP_EVAL)");
alias("EXT_TPP_UNIQUE", "!defined(TPP_CONFIG_EXTENSION_TPP_UNIQUE)");
alias("EXT_TPP_LOAD_FILE", "!defined(TPP_CONFIG_EXTENSION_TPP_LOAD_FILE)");
alias("EXT_TPP_COUNTER", "!defined(TPP_CONFIG_EXTENSION_TPP_COUNTER)");
alias("EXT_TPP_RANDOM", "!defined(TPP_CONFIG_EXTENSION_TPP_RANDOM)");
alias("EXT_TPP_STR_DECOMPILE", "!defined(TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE)");
alias("EXT_TPP_STR_SUBSTR", "!defined(TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR)");
alias("EXT_TPP_STR_PACK", "!defined(TPP_CONFIG_EXTENSION_TPP_STR_PACK)");
alias("EXT_TPP_STR_SIZE", "!defined(TPP_CONFIG_EXTENSION_TPP_STR_SIZE)");
alias("EXT_TPP_COUNT_TOKENS", "!defined(TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS)");
alias("EXT_TPP_IDENTIFIER", "!defined(TPP_CONFIG_EXTENSION_TPP_IDENTIFIER)");
alias("EXT_DOLLAR_IS_ALPHA", "!defined(TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA)");
alias("EXT_ASSERTIONS", "!defined(TPP_CONFIG_EXTENSION_ASSERTIONS)");
alias("EXT_CANONICAL_HEADERS", "!defined(TPP_CONFIG_EXTENSION_CANONICAL_HEADERS)");
alias("EXT_EXT_ARE_FEATURES", "!defined(TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES)");
alias("EXT_MSVC_FIXED_INT", "!defined(TPP_CONFIG_EXTENSION_MSVC_FIXED_INT)");
alias("EXT_NO_EXPAND_DEFINED", "!defined(TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED)");
alias("EXT_IFELSE_IN_EXPR", "!defined(TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR)");
alias("EXT_EXTENDED_IDENTS", "!defined(TPP_CONFIG_EXTENSION_EXTENDED_IDENTS)");
alias("EXT_TRADITIONAL_MACRO", "!defined(TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO)");
alias("EXT_BUILTIN_FUNCTIONS", "TPP_CONFIG_MINGCCFUNC < 2 && TPP_CONFIG_GCCFUNC");
alias("EXT_CPU_MACROS", "TPP_CONFIG_MINGCCFUNC < 2 && !TPP_CONFIG_MINMACRO");
alias("EXT_SYSTEM_MACROS", "TPP_CONFIG_MINGCCFUNC < 2 && !TPP_CONFIG_MINMACRO");
alias("EXT_UTILITY_MACROS", "TPP_CONFIG_MINGCCFUNC < 2 && !TPP_CONFIG_MINMACRO");
alias("WG_COMMENT");
alias("WG_COMMENTS");
alias("WG_MACROS");
alias("WG_SYNTAX");
alias("WG_USAGE");
alias("WG_VALUE");
alias("WG_BOOLVALUE");
alias("WG_USER");
alias("WG_ENVIRON");
alias("WG_LIMIT");
alias("WG_UNDEF");
alias("WG_TRIGRAPHS");
alias("WG_EXPANSION_TO_DEFINED");
alias("WG_QUALITY");
alias("WG_DEPRECATED");
alias("WG_DEPENDENCY");
if (currentCondition != "1")
	print("#endif /" "* ... *" "/");

function rename(oldName: string, newName: string) {
	print("#define ", newName, " TPP_FORWARD_COMPAT(", oldName, ")");
}
rename("W_UNKNOWN_PREPROCESSOR_DIRECTIVE", "TPP_W_UNKNOWN_DIRECTIVE");
rename("W_ERROR", "TPP_W_ERROR");
rename("W_WARNING", "TPP_W_WARNING");
rename("W_TOO_MANY_MACRO_ARGUMENTS", "TPP_W_TOO_MANY_ARGUMENTS");
rename("W_EOF_IN_MACRO_ARGUMENT_LIST", "TPP_W_EOF_IN_ARGUMENT_LIST");
rename("W_REDEFINING_MACRO", "TPP_W_REDEFINE_MACRO");
rename("W_DEPRECATED_IDENTIFIER", "TPP_W_DEPRECATED_KEYWORD");
rename("W_ARGUMENT_NAMED_ALREADY_TAKEN", "TPP_W_DUPLICATE_MACRO_PARAMETER_NAME");
rename("W_SLASHSTAR_INSIDE_OF_COMMENT", "TPP_W_SLASHSTAR_INSIDE_OF_COMMENT");
rename("W_LINE_COMMENT_CONTINUED", "TPP_W_LINE_COMMENT_CONTINUED");
rename("W_NOT_ENGOUH_MACRO_ARGUMENTS", "TPP_W_TOO_FEW_ARGUMENTS");
rename("W_STRING_TERMINATED_BY_LINEFEED", "TPP_W_STRING_TERMINATED_BY_LINEFEED");
rename("W_STRING_TERMINATED_BY_EOF", "TPP_W_STRING_TERMINATED_BY_EOF");
rename("W_COMMENT_TERMINATED_BY_EOF", "TPP_W_COMMENT_TERMINATED_BY_EOF");
rename("W_ENCOUNTERED_TRIGRAPH", "TPP_W_ENCOUNTERED_TRIGRAPH");
rename("W_DEFINED_IN_MACRO_BODY", "TPP_W_EXPANSION_TO_DEFINED");
rename("W_EXPECTED_LPAREN_AFTER_VA_OPT", "TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT");
rename("W_EXPECTED_RPAREN_AFTER_VA_OPT", "TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT");
rename("W_IF_WITHOUT_ENDIF", "TPP_W_EOF_BEFORE_ENDIF");
rename("W_ENDIF_WITHOUT_IF", "TPP_W_ENDIF_WITHOUT_IF");
rename("W_DIVIDE_BY_ZERO", "TPP_W_DIVIDE_BY_ZERO");
rename("W_EXPECTED_INCLUDE_STRING", "TPP_W_EXPECTED_INCLUDE_STRING");
rename("W_FILE_NOT_FOUND", "TPP_W_NO_SUCH_FILE");
rename("W_UNKNOWN_EXTENSION", "TPP_W_UNKNOWN_EXTENSION");
rename("W_CANT_POP_EXTENSIONS", "TPP_W_CANNOT_POP_EXTENSIONS");
rename("W_CANT_POP_WARNINGS", "TPP_W_CANNOT_POP_WARNINGS");
rename("W_INVALID_WARNING", "TPP_W_UNKNOWN_WARNING");                              // or: TPP_W_UNKNOWN_WARNING_NUMBER
rename("W_EXPECTED_WARNING_NAMEORID", "TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING"); // or: TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING_AFTER_COLON
rename("W_EXPECTED_KEYWORD_AFTER_ASSERT", "TPP_W_EXPECTED_ASSERTION_KEY_IN_DIRECTIVE");
rename("W_EXPECTED_KEYWORD_AFTER_PREDICATE", "TPP_W_EXPECTED_ASSERTION_VALUE_IN_DIRECTIVE");
rename("W_EXPECTED_KEYWORD_AFTER_EXPR_PRED", "TPP_W_EXPECTED_IDENTIFIER_AFTER_ASSERTION");
rename("W_DEPENDENCY_CHANGED", "TPP_W_DEPENDENCY_CHANGED");
rename("W_INCLUDE_RECURSION_LIMIT_EXCEEDED", "TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED");
rename("W_NONPARTABLE_FILENAME_CASING", "TPP_W_NONPORTABLE_FILENAME_CASING");
rename("W_MACRO_RECURSION_LIMIT_EXCEEDED", "TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED");
rename("W_CONSIDER_PAREN_AROUND_LAND", "TPP_W_PAREN_AROUND_LAND");
rename("W_CANT_POP_INCLUDE_PATH", "TPP_W_CANNOT_POP_INCLUDE_PATHS");
rename("W_INVALID_FLOAT_SUFFIX", "TPP_W_INVALID_FLOAT");
rename("W_SPECIAL_ARGUMENT_NAME", "TPP_W_RESERVED_MACRO_PARAMETER_NAME");
rename("W_VA_KEYWORD_IN_REGULAR_MACRO", "TPP_W_RESERVED_MACRO_KEYWORD");

]]]*/
#if !defined(TPP_CONFIG_USERDEFINED_KWD_IF) && !defined(TPP_CONFIG_USERDEFINED_KWD_if)
#define TPP_KWD_if TPP_FORWARD_COMPAT(KWD_if)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_IFDEF) && !defined(TPP_CONFIG_USERDEFINED_KWD_ifdef)
#define TPP_KWD_ifdef TPP_FORWARD_COMPAT(KWD_ifdef)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_IFNDEF) && !defined(TPP_CONFIG_USERDEFINED_KWD_ifndef)
#define TPP_KWD_ifndef TPP_FORWARD_COMPAT(KWD_ifndef)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_ELIF) && !defined(TPP_CONFIG_USERDEFINED_KWD_elif)
#define TPP_KWD_elif TPP_FORWARD_COMPAT(KWD_elif)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_ELSE) && !defined(TPP_CONFIG_USERDEFINED_KWD_else)
#define TPP_KWD_else TPP_FORWARD_COMPAT(KWD_else)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_ENDIF) && !defined(TPP_CONFIG_USERDEFINED_KWD_endif)
#define TPP_KWD_endif TPP_FORWARD_COMPAT(KWD_endif)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_DEFINE) && !defined(TPP_CONFIG_USERDEFINED_KWD_define)
#define TPP_KWD_define TPP_FORWARD_COMPAT(KWD_define)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_DEFINED) && !defined(TPP_CONFIG_USERDEFINED_KWD_defined)
#define TPP_KWD_defined TPP_FORWARD_COMPAT(KWD_defined)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_UNDEF) && !defined(TPP_CONFIG_USERDEFINED_KWD_undef)
#define TPP_KWD_undef TPP_FORWARD_COMPAT(KWD_undef)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_INCLUDE) && !defined(TPP_CONFIG_USERDEFINED_KWD_include)
#define TPP_KWD_include TPP_FORWARD_COMPAT(KWD_include)
#endif /* ... */
#if (!defined(TPP_CONFIG_EXTENSION_INCLUDE_NEXT) || TPP_CONFIG_EXTENSION_INCLUDE_NEXT) && !defined(TPP_CONFIG_USERDEFINED_KWD_INCLUDE_NEXT) && !defined(TPP_CONFIG_USERDEFINED_KWD_include_next)
#define TPP_KWD_include_next TPP_FORWARD_COMPAT(KWD_include_next)
#endif /* ... */
#if (!defined(TPP_CONFIG_EXTENSION_IMPORT) || TPP_CONFIG_EXTENSION_IMPORT) && !defined(TPP_CONFIG_USERDEFINED_KWD_IMPORT) && !defined(TPP_CONFIG_USERDEFINED_KWD_import)
#define TPP_KWD_import TPP_FORWARD_COMPAT(KWD_import)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_LINE) && !defined(TPP_CONFIG_USERDEFINED_KWD_line)
#define TPP_KWD_line TPP_FORWARD_COMPAT(KWD_line)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_ERROR) && !defined(TPP_CONFIG_USERDEFINED_KWD_error)
#define TPP_KWD_error TPP_FORWARD_COMPAT(KWD_error)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_WARNING) && !defined(TPP_CONFIG_USERDEFINED_KWD_warning)
#define TPP_KWD_warning TPP_FORWARD_COMPAT(KWD_warning)
#endif /* ... */
#if (!defined(TPP_CONFIG_EXTENSION_IDENT_SCCS) || TPP_CONFIG_EXTENSION_IDENT_SCCS) && !defined(TPP_CONFIG_USERDEFINED_KWD_IDENT) && !defined(TPP_CONFIG_USERDEFINED_KWD_ident)
#define TPP_KWD_ident TPP_FORWARD_COMPAT(KWD_ident)
#endif /* ... */
#if (!defined(TPP_CONFIG_EXTENSION_IDENT_SCCS) || TPP_CONFIG_EXTENSION_IDENT_SCCS) && !defined(TPP_CONFIG_USERDEFINED_KWD_SCCS) && !defined(TPP_CONFIG_USERDEFINED_KWD_sccs)
#define TPP_KWD_sccs TPP_FORWARD_COMPAT(KWD_sccs)
#endif /* ... */
#if (!defined(TPP_CONFIG_EXTENSION_ASSERTIONS) || TPP_CONFIG_EXTENSION_ASSERTIONS) && !defined(TPP_CONFIG_USERDEFINED_KWD_ASSERT) && !defined(TPP_CONFIG_USERDEFINED_KWD_assert)
#define TPP_KWD_assert TPP_FORWARD_COMPAT(KWD_assert)
#endif /* ... */
#if (!defined(TPP_CONFIG_EXTENSION_ASSERTIONS) || TPP_CONFIG_EXTENSION_ASSERTIONS) && !defined(TPP_CONFIG_USERDEFINED_KWD_UNASSERT) && !defined(TPP_CONFIG_USERDEFINED_KWD_unassert)
#define TPP_KWD_unassert TPP_FORWARD_COMPAT(KWD_unassert)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD_PRAGMA) && !defined(TPP_CONFIG_USERDEFINED_KWD_pragma)
#define TPP_KWD_pragma TPP_FORWARD_COMPAT(KWD_pragma)
#endif /* ... */
#if !defined(TPP_CONFIG_USERDEFINED_KWD__PRAGMA) && !defined(TPP_CONFIG_USERDEFINED_KWD__Pragma)
#define TPP_KWD__Pragma TPP_FORWARD_COMPAT(KWD__Pragma)
#endif /* ... */
#if (!defined(TPP_CONFIG_EXTENSION_MSVC_PRAGMA) || TPP_CONFIG_EXTENSION_MSVC_PRAGMA) && !defined(TPP_CONFIG_USERDEFINED_KWD___PRAGMA) && !defined(TPP_CONFIG_USERDEFINED_KWD___pragma)
#define TPP_KWD___pragma TPP_FORWARD_COMPAT(KWD___pragma)
#endif /* ... */
#define TPP_KWD___FILE__ TPP_FORWARD_COMPAT(KWD___FILE__)
#define TPP_KWD___LINE__ TPP_FORWARD_COMPAT(KWD___LINE__)
#define TPP_KWD___TIME__ TPP_FORWARD_COMPAT(KWD___TIME__)
#define TPP_KWD___DATE__ TPP_FORWARD_COMPAT(KWD___DATE__)
#if !defined(TPP_CONFIG_EXTENSION_BASEFILE) || TPP_CONFIG_EXTENSION_BASEFILE
#define TPP_KWD___BASE_FILE__ TPP_FORWARD_COMPAT(KWD___BASE_FILE__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_INCLUDE_LEVEL) || TPP_CONFIG_EXTENSION_INCLUDE_LEVEL
#define TPP_KWD___INCLUDE_LEVEL__ TPP_FORWARD_COMPAT(KWD___INCLUDE_LEVEL__)
#define TPP_KWD___INCLUDE_DEPTH__ TPP_FORWARD_COMPAT(KWD___INCLUDE_DEPTH__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_COUNTER) || TPP_CONFIG_EXTENSION_COUNTER
#define TPP_KWD___COUNTER__ TPP_FORWARD_COMPAT(KWD___COUNTER__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TIMESTAMP) || TPP_CONFIG_EXTENSION_TIMESTAMP
#define TPP_KWD___TIMESTAMP__ TPP_FORWARD_COMPAT(KWD___TIMESTAMP__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_COLUMN) || TPP_CONFIG_EXTENSION_COLUMN
#define TPP_KWD___COLUMN__ TPP_FORWARD_COMPAT(KWD___COLUMN__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES) || TPP_CONFIG_EXTENSION_CLANG_FEATURES
#define TPP_KWD___is_identifier TPP_FORWARD_COMPAT(KWD___is_identifier)
#define TPP_KWD___is_builtin_identifier TPP_FORWARD_COMPAT(KWD___is_builtin_identifier)
#define TPP_KWD___is_deprecated TPP_FORWARD_COMPAT(KWD___is_deprecated)
#define TPP_KWD___is_poisoned TPP_FORWARD_COMPAT(KWD___is_poisoned)
#define TPP_KWD___has_attribute TPP_FORWARD_COMPAT(KWD___has_attribute)
#define TPP_KWD___has_builtin TPP_FORWARD_COMPAT(KWD___has_builtin)
#define TPP_KWD___has_tpp_builtin TPP_FORWARD_COMPAT(KWD___has_tpp_builtin)
#define TPP_KWD___has_cpp_attribute TPP_FORWARD_COMPAT(KWD___has_cpp_attribute)
#define TPP_KWD___has_declspec_attribute TPP_FORWARD_COMPAT(KWD___has_declspec_attribute)
#define TPP_KWD___has_feature TPP_FORWARD_COMPAT(KWD___has_feature)
#define TPP_KWD___has_extension TPP_FORWARD_COMPAT(KWD___has_extension)
#define TPP_KWD___has_warning TPP_FORWARD_COMPAT(KWD___has_warning)
#define TPP_KWD___has_known_extension TPP_FORWARD_COMPAT(KWD___has_known_extension)
#define TPP_KWD___has_known_warning TPP_FORWARD_COMPAT(KWD___has_known_warning)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_HAS_INCLUDE) || TPP_CONFIG_EXTENSION_HAS_INCLUDE
#define TPP_KWD___has_include TPP_FORWARD_COMPAT(KWD___has_include)
#define TPP_KWD___has_include_next TPP_FORWARD_COMPAT(KWD___has_include_next)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_ARGS) || TPP_CONFIG_EXTENSION_VA_ARGS
#define TPP_KWD___VA_ARGS__ TPP_FORWARD_COMPAT(KWD___VA_ARGS__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_COMMA) || TPP_CONFIG_EXTENSION_VA_COMMA
#define TPP_KWD___VA_COMMA__ TPP_FORWARD_COMPAT(KWD___VA_COMMA__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_NARGS) || TPP_CONFIG_EXTENSION_VA_NARGS
#define TPP_KWD___VA_NARGS__ TPP_FORWARD_COMPAT(KWD___VA_NARGS__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_OPT) || TPP_CONFIG_EXTENSION_VA_OPT
#define TPP_KWD___VA_OPT__ TPP_FORWARD_COMPAT(KWD___VA_OPT__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_EVAL) || TPP_CONFIG_EXTENSION_TPP_EVAL
#define TPP_KWD___TPP_EVAL TPP_FORWARD_COMPAT(KWD___TPP_EVAL)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_LOAD_FILE) || TPP_CONFIG_EXTENSION_TPP_LOAD_FILE
#define TPP_KWD___TPP_LOAD_FILE TPP_FORWARD_COMPAT(KWD___TPP_LOAD_FILE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_COUNTER) || TPP_CONFIG_EXTENSION_TPP_COUNTER
#define TPP_KWD___TPP_COUNTER TPP_FORWARD_COMPAT(KWD___TPP_COUNTER)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_RANDOM) || TPP_CONFIG_EXTENSION_TPP_RANDOM
#define TPP_KWD___TPP_RANDOM TPP_FORWARD_COMPAT(KWD___TPP_RANDOM)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE) || TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE
#define TPP_KWD___TPP_STR_DECOMPILE TPP_FORWARD_COMPAT(KWD___TPP_STR_DECOMPILE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR) || TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR
#define TPP_KWD___TPP_STR_AT TPP_FORWARD_COMPAT(KWD___TPP_STR_AT)
#define TPP_KWD___TPP_STR_SUBSTR TPP_FORWARD_COMPAT(KWD___TPP_STR_SUBSTR)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_STR_PACK) || TPP_CONFIG_EXTENSION_TPP_STR_PACK
#define TPP_KWD___TPP_STR_PACK TPP_FORWARD_COMPAT(KWD___TPP_STR_PACK)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_STR_SIZE) || TPP_CONFIG_EXTENSION_TPP_STR_SIZE
#define TPP_KWD___TPP_STR_SIZE TPP_FORWARD_COMPAT(KWD___TPP_STR_SIZE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_UNIQUE) || TPP_CONFIG_EXTENSION_TPP_UNIQUE
#define TPP_KWD___TPP_UNIQUE TPP_FORWARD_COMPAT(KWD___TPP_UNIQUE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS) || TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS
#define TPP_KWD___TPP_COUNT_TOKENS TPP_FORWARD_COMPAT(KWD___TPP_COUNT_TOKENS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_IDENTIFIER) || TPP_CONFIG_EXTENSION_TPP_IDENTIFIER
#define TPP_KWD___TPP_IDENTIFIER TPP_FORWARD_COMPAT(KWD___TPP_IDENTIFIER)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_DATEUTILS) || TPP_CONFIG_EXTENSION_DATEUTILS
#define TPP_KWD___DATE_DAY__ TPP_FORWARD_COMPAT(KWD___DATE_DAY__)
#define TPP_KWD___DATE_WDAY__ TPP_FORWARD_COMPAT(KWD___DATE_WDAY__)
#define TPP_KWD___DATE_YDAY__ TPP_FORWARD_COMPAT(KWD___DATE_YDAY__)
#define TPP_KWD___DATE_MONTH__ TPP_FORWARD_COMPAT(KWD___DATE_MONTH__)
#define TPP_KWD___DATE_YEAR__ TPP_FORWARD_COMPAT(KWD___DATE_YEAR__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TIMEUTILS) || TPP_CONFIG_EXTENSION_TIMEUTILS
#define TPP_KWD___TIME_SEC__ TPP_FORWARD_COMPAT(KWD___TIME_SEC__)
#define TPP_KWD___TIME_MIN__ TPP_FORWARD_COMPAT(KWD___TIME_MIN__)
#define TPP_KWD___TIME_HOUR__ TPP_FORWARD_COMPAT(KWD___TIME_HOUR__)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA) || TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA
#define TPP_KWD_tpp_dollar_is_alpha TPP_FORWARD_COMPAT(KWD_tpp_dollar_is_alpha)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_ARGS) || TPP_CONFIG_EXTENSION_VA_ARGS
#define TPP_KWD_tpp_va_args TPP_FORWARD_COMPAT(KWD_tpp_va_args)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_GCC_VA_ARGS) || TPP_CONFIG_EXTENSION_GCC_VA_ARGS
#define TPP_KWD_tpp_named_va_args TPP_FORWARD_COMPAT(KWD_tpp_named_va_args)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_COMMA) || TPP_CONFIG_EXTENSION_VA_COMMA
#define TPP_KWD_tpp_va_comma TPP_FORWARD_COMPAT(KWD_tpp_va_comma)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_MSVC_FIXED_INT) || TPP_CONFIG_EXTENSION_MSVC_FIXED_INT
#define TPP_KWD_tpp_msvc_integer_suffix TPP_FORWARD_COMPAT(KWD_tpp_msvc_integer_suffix)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_HASH_AT) || TPP_CONFIG_EXTENSION_HASH_AT
#define TPP_KWD_tpp_charize_operator TPP_FORWARD_COMPAT(KWD_tpp_charize_operator)
#endif /* ... */
#if !defined(TPP_CONFIG_FEATURE_TRIGRAPHS) || TPP_CONFIG_FEATURE_TRIGRAPHS
#define TPP_KWD_tpp_trigraphs TPP_FORWARD_COMPAT(KWD_tpp_trigraphs)
#endif /* ... */
#if !defined(TPP_CONFIG_FEATURE_DIGRAPHS) || TPP_CONFIG_FEATURE_DIGRAPHS
#define TPP_KWD_tpp_digraphs TPP_FORWARD_COMPAT(KWD_tpp_digraphs)
#endif /* ... */
#define TPP_KWD_tpp_reemit_unknown_pragmas TPP_FORWARD_COMPAT(KWD_tpp_reemit_unknown_pragmas)
#define TPP_KWD_tpp_pragma_push_macro TPP_FORWARD_COMPAT(KWD_tpp_pragma_push_macro)
#define TPP_KWD_tpp_pragma_pop_macro TPP_FORWARD_COMPAT(KWD_tpp_pragma_pop_macro)
#define TPP_KWD_tpp_pragma_region TPP_FORWARD_COMPAT(KWD_tpp_pragma_region)
#define TPP_KWD_tpp_pragma_endregion TPP_FORWARD_COMPAT(KWD_tpp_pragma_endregion)
#define TPP_KWD_tpp_pragma_warning TPP_FORWARD_COMPAT(KWD_tpp_pragma_warning)
#define TPP_KWD_tpp_pragma_message TPP_FORWARD_COMPAT(KWD_tpp_pragma_message)
#define TPP_KWD_tpp_pragma_error TPP_FORWARD_COMPAT(KWD_tpp_pragma_error)
#define TPP_KWD_tpp_pragma_once TPP_FORWARD_COMPAT(KWD_tpp_pragma_once)
#define TPP_KWD_tpp_pragma_tpp_exec TPP_FORWARD_COMPAT(KWD_tpp_pragma_tpp_exec)
#define TPP_KWD_tpp_pragma_deprecated TPP_FORWARD_COMPAT(KWD_tpp_pragma_deprecated)
#define TPP_KWD_tpp_pragma_tpp_set_keyword_flags TPP_FORWARD_COMPAT(KWD_tpp_pragma_tpp_set_keyword_flags)
#if !defined(TPP_CONFIG_EXTENSION_INCLUDE_NEXT) || TPP_CONFIG_EXTENSION_INCLUDE_NEXT
#define TPP_KWD_tpp_directive_include_next TPP_FORWARD_COMPAT(KWD_tpp_directive_include_next)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_IMPORT) || TPP_CONFIG_EXTENSION_IMPORT
#define TPP_KWD_tpp_directive_import TPP_FORWARD_COMPAT(KWD_tpp_directive_import)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_WARNING) || TPP_CONFIG_EXTENSION_WARNING
#define TPP_KWD_tpp_directive_warning TPP_FORWARD_COMPAT(KWD_tpp_directive_warning)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_LXOR) || TPP_CONFIG_EXTENSION_LXOR
#define TPP_KWD_tpp_lxor TPP_FORWARD_COMPAT(KWD_tpp_lxor)
#endif /* ... */
#define TPP_KWD_tpp_token_tilde_tilde TPP_FORWARD_COMPAT(KWD_tpp_token_tilde_tilde)
#define TPP_KWD_tpp_token_pow TPP_FORWARD_COMPAT(KWD_tpp_token_pow)
#define TPP_KWD_tpp_token_lxor TPP_FORWARD_COMPAT(KWD_tpp_token_lxor)
#define TPP_KWD_tpp_token_arrow TPP_FORWARD_COMPAT(KWD_tpp_token_arrow)
#define TPP_KWD_tpp_token_colon_assign TPP_FORWARD_COMPAT(KWD_tpp_token_colon_assign)
#define TPP_KWD_tpp_token_colon_colon TPP_FORWARD_COMPAT(KWD_tpp_token_colon_colon)
#if !defined(TPP_CONFIG_EXTENSION_ALTMAC) || TPP_CONFIG_EXTENSION_ALTMAC
#define TPP_KWD_tpp_macro_calling_conventions TPP_FORWARD_COMPAT(KWD_tpp_macro_calling_conventions)
#endif /* ... */
#define TPP_KWD_tpp_strict_whitespace TPP_FORWARD_COMPAT(KWD_tpp_strict_whitespace)
#define TPP_KWD_tpp_strict_integer_overflow TPP_FORWARD_COMPAT(KWD_tpp_strict_integer_overflow)
#define TPP_KWD_tpp_support_ansi_characters TPP_FORWARD_COMPAT(KWD_tpp_support_ansi_characters)
#define TPP_KWD_tpp_emit_lf_after_directive TPP_FORWARD_COMPAT(KWD_tpp_emit_lf_after_directive)
#if !defined(TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR) || TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR
#define TPP_KWD_tpp_if_cond_expression TPP_FORWARD_COMPAT(KWD_tpp_if_cond_expression)
#endif /* ... */
#define TPP_KWD_tpp_debug TPP_FORWARD_COMPAT(KWD_tpp_debug)
#define TPP_KWD___TPP_VERSION__ TPP_FORWARD_COMPAT(KWD___TPP_VERSION__)
#if !defined(TPP_CONFIG_FEATURE_TRIGRAPHS)
#define TPP_EXT_TRIGRAPHS TPP_FORWARD_COMPAT(EXT_TRIGRAPHS)
#endif /* ... */
#if !defined(TPP_CONFIG_FEATURE_DIGRAPHS)
#define TPP_EXT_DIGRAPHS TPP_FORWARD_COMPAT(EXT_DIGRAPHS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_GCC_VA_ARGS)
#define TPP_EXT_GCC_VA_ARGS TPP_FORWARD_COMPAT(EXT_GCC_VA_ARGS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_GCC_VA_COMMA)
#define TPP_EXT_GCC_VA_COMMA TPP_FORWARD_COMPAT(EXT_GCC_VA_COMMA)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_GCC_IFELSE)
#define TPP_EXT_GCC_IFELSE TPP_FORWARD_COMPAT(EXT_GCC_IFELSE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_COMMA)
#define TPP_EXT_VA_COMMA TPP_FORWARD_COMPAT(EXT_VA_COMMA)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_OPT)
#define TPP_EXT_VA_OPT TPP_FORWARD_COMPAT(EXT_VA_OPT)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_NARGS)
#define TPP_EXT_VA_NARGS TPP_FORWARD_COMPAT(EXT_VA_NARGS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_VA_ARGS)
#define TPP_EXT_VA_ARGS TPP_FORWARD_COMPAT(EXT_VA_ARGS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_STR_E)
#define TPP_EXT_STR_E TPP_FORWARD_COMPAT(EXT_STR_E)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_ALTMAC)
#define TPP_EXT_ALTMAC TPP_FORWARD_COMPAT(EXT_ALTMAC)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_RECMAC)
#define TPP_EXT_RECMAC TPP_FORWARD_COMPAT(EXT_RECMAC)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_ARGSPACE)
#define TPP_EXT_ARGSPACE TPP_FORWARD_COMPAT(EXT_ARGSPACE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_BININTEGRAL)
#define TPP_EXT_BININTEGRAL TPP_FORWARD_COMPAT(EXT_BININTEGRAL)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_MSVC_PRAGMA)
#define TPP_EXT_MSVC_PRAGMA TPP_FORWARD_COMPAT(EXT_MSVC_PRAGMA)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_STRINGOPS)
#define TPP_EXT_STRINGOPS TPP_FORWARD_COMPAT(EXT_STRINGOPS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_HASH_AT)
#define TPP_EXT_HASH_AT TPP_FORWARD_COMPAT(EXT_HASH_AT)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_HASH_XCLAIM)
#define TPP_EXT_HASH_XCLAIM TPP_FORWARD_COMPAT(EXT_HASH_XCLAIM)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_WARNING)
#define TPP_EXT_WARNING TPP_FORWARD_COMPAT(EXT_WARNING)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_SHEBANG)
#define TPP_EXT_SHEBANG TPP_FORWARD_COMPAT(EXT_SHEBANG)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_INCLUDE_NEXT)
#define TPP_EXT_INCLUDE_NEXT TPP_FORWARD_COMPAT(EXT_INCLUDE_NEXT)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_IMPORT)
#define TPP_EXT_IMPORT TPP_FORWARD_COMPAT(EXT_IMPORT)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_IDENT_SCCS)
#define TPP_EXT_IDENT_SCCS TPP_FORWARD_COMPAT(EXT_IDENT_SCCS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_BASEFILE)
#define TPP_EXT_BASEFILE TPP_FORWARD_COMPAT(EXT_BASEFILE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_INCLUDE_LEVEL)
#define TPP_EXT_INCLUDE_LEVEL TPP_FORWARD_COMPAT(EXT_INCLUDE_LEVEL)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_COUNTER)
#define TPP_EXT_COUNTER TPP_FORWARD_COMPAT(EXT_COUNTER)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_CLANG_FEATURES)
#define TPP_EXT_CLANG_FEATURES TPP_FORWARD_COMPAT(EXT_CLANG_FEATURES)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_HAS_INCLUDE)
#define TPP_EXT_HAS_INCLUDE TPP_FORWARD_COMPAT(EXT_HAS_INCLUDE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_LXOR)
#define TPP_EXT_LXOR TPP_FORWARD_COMPAT(EXT_LXOR)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_MULTICHAR_CONST)
#define TPP_EXT_MULTICHAR_CONST TPP_FORWARD_COMPAT(EXT_MULTICHAR_CONST)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_DATEUTILS)
#define TPP_EXT_DATEUTILS TPP_FORWARD_COMPAT(EXT_DATEUTILS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TIMEUTILS)
#define TPP_EXT_TIMEUTILS TPP_FORWARD_COMPAT(EXT_TIMEUTILS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TIMESTAMP)
#define TPP_EXT_TIMESTAMP TPP_FORWARD_COMPAT(EXT_TIMESTAMP)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_COLUMN)
#define TPP_EXT_COLUMN TPP_FORWARD_COMPAT(EXT_COLUMN)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_EVAL)
#define TPP_EXT_TPP_EVAL TPP_FORWARD_COMPAT(EXT_TPP_EVAL)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_UNIQUE)
#define TPP_EXT_TPP_UNIQUE TPP_FORWARD_COMPAT(EXT_TPP_UNIQUE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_LOAD_FILE)
#define TPP_EXT_TPP_LOAD_FILE TPP_FORWARD_COMPAT(EXT_TPP_LOAD_FILE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_COUNTER)
#define TPP_EXT_TPP_COUNTER TPP_FORWARD_COMPAT(EXT_TPP_COUNTER)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_RANDOM)
#define TPP_EXT_TPP_RANDOM TPP_FORWARD_COMPAT(EXT_TPP_RANDOM)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_STR_DECOMPILE)
#define TPP_EXT_TPP_STR_DECOMPILE TPP_FORWARD_COMPAT(EXT_TPP_STR_DECOMPILE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_STR_SUBSTR)
#define TPP_EXT_TPP_STR_SUBSTR TPP_FORWARD_COMPAT(EXT_TPP_STR_SUBSTR)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_STR_PACK)
#define TPP_EXT_TPP_STR_PACK TPP_FORWARD_COMPAT(EXT_TPP_STR_PACK)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_STR_SIZE)
#define TPP_EXT_TPP_STR_SIZE TPP_FORWARD_COMPAT(EXT_TPP_STR_SIZE)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_COUNT_TOKENS)
#define TPP_EXT_TPP_COUNT_TOKENS TPP_FORWARD_COMPAT(EXT_TPP_COUNT_TOKENS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TPP_IDENTIFIER)
#define TPP_EXT_TPP_IDENTIFIER TPP_FORWARD_COMPAT(EXT_TPP_IDENTIFIER)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_DOLLAR_IS_ALPHA)
#define TPP_EXT_DOLLAR_IS_ALPHA TPP_FORWARD_COMPAT(EXT_DOLLAR_IS_ALPHA)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_ASSERTIONS)
#define TPP_EXT_ASSERTIONS TPP_FORWARD_COMPAT(EXT_ASSERTIONS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_CANONICAL_HEADERS)
#define TPP_EXT_CANONICAL_HEADERS TPP_FORWARD_COMPAT(EXT_CANONICAL_HEADERS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_EXT_ARE_FEATURES)
#define TPP_EXT_EXT_ARE_FEATURES TPP_FORWARD_COMPAT(EXT_EXT_ARE_FEATURES)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_MSVC_FIXED_INT)
#define TPP_EXT_MSVC_FIXED_INT TPP_FORWARD_COMPAT(EXT_MSVC_FIXED_INT)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_NO_EXPAND_DEFINED)
#define TPP_EXT_NO_EXPAND_DEFINED TPP_FORWARD_COMPAT(EXT_NO_EXPAND_DEFINED)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_IFELSE_IN_EXPR)
#define TPP_EXT_IFELSE_IN_EXPR TPP_FORWARD_COMPAT(EXT_IFELSE_IN_EXPR)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_EXTENDED_IDENTS)
#define TPP_EXT_EXTENDED_IDENTS TPP_FORWARD_COMPAT(EXT_EXTENDED_IDENTS)
#endif /* ... */
#if !defined(TPP_CONFIG_EXTENSION_TRADITIONAL_MACRO)
#define TPP_EXT_TRADITIONAL_MACRO TPP_FORWARD_COMPAT(EXT_TRADITIONAL_MACRO)
#endif /* ... */
#if TPP_CONFIG_MINGCCFUNC < 2 && TPP_CONFIG_GCCFUNC
#define TPP_EXT_BUILTIN_FUNCTIONS TPP_FORWARD_COMPAT(EXT_BUILTIN_FUNCTIONS)
#endif /* ... */
#if TPP_CONFIG_MINGCCFUNC < 2 && !TPP_CONFIG_MINMACRO
#define TPP_EXT_CPU_MACROS TPP_FORWARD_COMPAT(EXT_CPU_MACROS)
#define TPP_EXT_SYSTEM_MACROS TPP_FORWARD_COMPAT(EXT_SYSTEM_MACROS)
#define TPP_EXT_UTILITY_MACROS TPP_FORWARD_COMPAT(EXT_UTILITY_MACROS)
#endif /* ... */
#define TPP_WG_COMMENT TPP_FORWARD_COMPAT(WG_COMMENT)
#define TPP_WG_COMMENTS TPP_FORWARD_COMPAT(WG_COMMENTS)
#define TPP_WG_MACROS TPP_FORWARD_COMPAT(WG_MACROS)
#define TPP_WG_SYNTAX TPP_FORWARD_COMPAT(WG_SYNTAX)
#define TPP_WG_USAGE TPP_FORWARD_COMPAT(WG_USAGE)
#define TPP_WG_VALUE TPP_FORWARD_COMPAT(WG_VALUE)
#define TPP_WG_BOOLVALUE TPP_FORWARD_COMPAT(WG_BOOLVALUE)
#define TPP_WG_USER TPP_FORWARD_COMPAT(WG_USER)
#define TPP_WG_ENVIRON TPP_FORWARD_COMPAT(WG_ENVIRON)
#define TPP_WG_LIMIT TPP_FORWARD_COMPAT(WG_LIMIT)
#define TPP_WG_UNDEF TPP_FORWARD_COMPAT(WG_UNDEF)
#define TPP_WG_TRIGRAPHS TPP_FORWARD_COMPAT(WG_TRIGRAPHS)
#define TPP_WG_EXPANSION_TO_DEFINED TPP_FORWARD_COMPAT(WG_EXPANSION_TO_DEFINED)
#define TPP_WG_QUALITY TPP_FORWARD_COMPAT(WG_QUALITY)
#define TPP_WG_DEPRECATED TPP_FORWARD_COMPAT(WG_DEPRECATED)
#define TPP_WG_DEPENDENCY TPP_FORWARD_COMPAT(WG_DEPENDENCY)
#define TPP_W_UNKNOWN_DIRECTIVE TPP_FORWARD_COMPAT(W_UNKNOWN_PREPROCESSOR_DIRECTIVE)
#define TPP_W_ERROR TPP_FORWARD_COMPAT(W_ERROR)
#define TPP_W_WARNING TPP_FORWARD_COMPAT(W_WARNING)
#define TPP_W_TOO_MANY_ARGUMENTS TPP_FORWARD_COMPAT(W_TOO_MANY_MACRO_ARGUMENTS)
#define TPP_W_EOF_IN_ARGUMENT_LIST TPP_FORWARD_COMPAT(W_EOF_IN_MACRO_ARGUMENT_LIST)
#define TPP_W_REDEFINE_MACRO TPP_FORWARD_COMPAT(W_REDEFINING_MACRO)
#define TPP_W_DEPRECATED_KEYWORD TPP_FORWARD_COMPAT(W_DEPRECATED_IDENTIFIER)
#define TPP_W_DUPLICATE_MACRO_PARAMETER_NAME TPP_FORWARD_COMPAT(W_ARGUMENT_NAMED_ALREADY_TAKEN)
#define TPP_W_SLASHSTAR_INSIDE_OF_COMMENT TPP_FORWARD_COMPAT(W_SLASHSTAR_INSIDE_OF_COMMENT)
#define TPP_W_LINE_COMMENT_CONTINUED TPP_FORWARD_COMPAT(W_LINE_COMMENT_CONTINUED)
#define TPP_W_TOO_FEW_ARGUMENTS TPP_FORWARD_COMPAT(W_NOT_ENGOUH_MACRO_ARGUMENTS)
#define TPP_W_STRING_TERMINATED_BY_LINEFEED TPP_FORWARD_COMPAT(W_STRING_TERMINATED_BY_LINEFEED)
#define TPP_W_STRING_TERMINATED_BY_EOF TPP_FORWARD_COMPAT(W_STRING_TERMINATED_BY_EOF)
#define TPP_W_COMMENT_TERMINATED_BY_EOF TPP_FORWARD_COMPAT(W_COMMENT_TERMINATED_BY_EOF)
#define TPP_W_ENCOUNTERED_TRIGRAPH TPP_FORWARD_COMPAT(W_ENCOUNTERED_TRIGRAPH)
#define TPP_W_EXPANSION_TO_DEFINED TPP_FORWARD_COMPAT(W_DEFINED_IN_MACRO_BODY)
#define TPP_W_EXPECTED_LPAREN_AFTER_VA_OPT TPP_FORWARD_COMPAT(W_EXPECTED_LPAREN_AFTER_VA_OPT)
#define TPP_W_EXPECTED_RPAREN_AFTER_VA_OPT TPP_FORWARD_COMPAT(W_EXPECTED_RPAREN_AFTER_VA_OPT)
#define TPP_W_EOF_BEFORE_ENDIF TPP_FORWARD_COMPAT(W_IF_WITHOUT_ENDIF)
#define TPP_W_ENDIF_WITHOUT_IF TPP_FORWARD_COMPAT(W_ENDIF_WITHOUT_IF)
#define TPP_W_DIVIDE_BY_ZERO TPP_FORWARD_COMPAT(W_DIVIDE_BY_ZERO)
#define TPP_W_EXPECTED_INCLUDE_STRING TPP_FORWARD_COMPAT(W_EXPECTED_INCLUDE_STRING)
#define TPP_W_NO_SUCH_FILE TPP_FORWARD_COMPAT(W_FILE_NOT_FOUND)
#define TPP_W_UNKNOWN_EXTENSION TPP_FORWARD_COMPAT(W_UNKNOWN_EXTENSION)
#define TPP_W_CANNOT_POP_EXTENSIONS TPP_FORWARD_COMPAT(W_CANT_POP_EXTENSIONS)
#define TPP_W_CANNOT_POP_WARNINGS TPP_FORWARD_COMPAT(W_CANT_POP_WARNINGS)
#define TPP_W_UNKNOWN_WARNING TPP_FORWARD_COMPAT(W_INVALID_WARNING)
#define TPP_W_UNEXPECTED_TOKEN_IN_PRAGMA_WARNING TPP_FORWARD_COMPAT(W_EXPECTED_WARNING_NAMEORID)
#define TPP_W_EXPECTED_ASSERTION_KEY_IN_DIRECTIVE TPP_FORWARD_COMPAT(W_EXPECTED_KEYWORD_AFTER_ASSERT)
#define TPP_W_EXPECTED_ASSERTION_VALUE_IN_DIRECTIVE TPP_FORWARD_COMPAT(W_EXPECTED_KEYWORD_AFTER_PREDICATE)
#define TPP_W_EXPECTED_IDENTIFIER_AFTER_ASSERTION TPP_FORWARD_COMPAT(W_EXPECTED_KEYWORD_AFTER_EXPR_PRED)
#define TPP_W_DEPENDENCY_CHANGED TPP_FORWARD_COMPAT(W_DEPENDENCY_CHANGED)
#define TPP_W_INCLUDE_RECURSION_LIMIT_EXCEEDED TPP_FORWARD_COMPAT(W_INCLUDE_RECURSION_LIMIT_EXCEEDED)
#define TPP_W_NONPORTABLE_FILENAME_CASING TPP_FORWARD_COMPAT(W_NONPARTABLE_FILENAME_CASING)
#define TPP_W_MACRO_RECURSION_LIMIT_EXCEEDED TPP_FORWARD_COMPAT(W_MACRO_RECURSION_LIMIT_EXCEEDED)
#define TPP_W_PAREN_AROUND_LAND TPP_FORWARD_COMPAT(W_CONSIDER_PAREN_AROUND_LAND)
#define TPP_W_CANNOT_POP_INCLUDE_PATHS TPP_FORWARD_COMPAT(W_CANT_POP_INCLUDE_PATH)
#define TPP_W_INVALID_FLOAT TPP_FORWARD_COMPAT(W_INVALID_FLOAT_SUFFIX)
#define TPP_W_RESERVED_MACRO_PARAMETER_NAME TPP_FORWARD_COMPAT(W_SPECIAL_ARGUMENT_NAME)
#define TPP_W_RESERVED_MACRO_KEYWORD TPP_FORWARD_COMPAT(W_VA_KEYWORD_IN_REGULAR_MACRO)
/*[[[end]]]*/



/************************************************************************/
/* "parts/token.h"                                                      */
/************************************************************************/

#define tpp_token_id                       TPP_FORWARD_COMPAT(tok_t)
#define TPP_TOK_EOF                        TPP_FORWARD_COMPAT(TOK_EOF)
#define TPP_TOK_LF                         TPP_FORWARD_COMPAT(TOK_LF)
#define TPP_TOK_SPACE                      TPP_FORWARD_COMPAT(TOK_SPACE)
#define TPP_TOK_EXCLAIM                    TPP_FORWARD_COMPAT(TOK_NOT)
#define TPP_TOK_POUND                      TPP_FORWARD_COMPAT(TOK_HASH)
#define TPP_TOK_DOLLAR                     ((tpp_token_id)'$')
#define TPP_TOK_PERCENT                    TPP_FORWARD_COMPAT(TOK_MOD)
#define TPP_TOK_AMP                        TPP_FORWARD_COMPAT(TOK_AND)
#define TPP_TOK_LPAREN                     TPP_FORWARD_COMPAT(TOK_LPAREN)
#define TPP_TOK_RPAREN                     TPP_FORWARD_COMPAT(TOK_RPAREN)
#define TPP_TOK_STAR                       TPP_FORWARD_COMPAT(TOK_MUL)
#define TPP_TOK_PLUS                       TPP_FORWARD_COMPAT(TOK_ADD)
#define TPP_TOK_COMMA                      TPP_FORWARD_COMPAT(TOK_COMMA)
#define TPP_TOK_MINUS                      TPP_FORWARD_COMPAT(TOK_SUB)
#define TPP_TOK_DOT                        TPP_FORWARD_COMPAT(TOK_DOT)
#define TPP_TOK_SLASH                      TPP_FORWARD_COMPAT(TOK_DIV)
#define TPP_TOK_COLON                      TPP_FORWARD_COMPAT(TOK_COLON)
#define TPP_TOK_SEMICOLON                  TPP_FORWARD_COMPAT(TOK_SEMICOLON)
#define TPP_TOK_LANGLE                     TPP_FORWARD_COMPAT(TOK_LANGLE)
/*#define TPP_TOK_EQUAL                    TPP_FORWARD_COMPAT(TOK_ASSIGN)*/ /* Ambiguity -- TPP2 defined this as "==" */
#define TPP_TOK_RANGLE                     TPP_FORWARD_COMPAT(TOK_RANGLE)
#define TPP_TOK_QMARK                      TPP_FORWARD_COMPAT(TOK_QUESTION)
#define TPP_TOK_AT                         TPP_FORWARD_COMPAT(TOK_AT)
#define TPP_TOK_LBRACKET                   TPP_FORWARD_COMPAT(TOK_LBRACKET)
#define TPP_TOK_BACKSLASH                  TPP_FORWARD_COMPAT(TOK_BACKSLASH)
#define TPP_TOK_RBRACKET                   TPP_FORWARD_COMPAT(TOK_RBRACKET)
#define TPP_TOK_HAT                        TPP_FORWARD_COMPAT(TOK_XOR)
#define TPP_TOK_BACKTICK                   ((tpp_token_id)'`')
#define TPP_TOK_LBRACE                     TPP_FORWARD_COMPAT(TOK_LBRACE)
#define TPP_TOK_PIPE                       TPP_FORWARD_COMPAT(TOK_OR)
#define TPP_TOK_RBRACE                     TPP_FORWARD_COMPAT(TOK_RBRACE)
#define TPP_TOK_TILDE                      TPP_FORWARD_COMPAT(TOK_TILDE)
#define TPP_TOK_LANGLE_LANGLE              TPP_FORWARD_COMPAT(TOK_SHL)
#define TPP_TOK_RANGLE_RANGLE              TPP_FORWARD_COMPAT(TOK_SHR)
#define TPP_TOK_EQUAL_EQUAL                TPP_FORWARD_COMPAT(TOK_EQUAL)
#define TPP_TOK_EXCLAIM_EQUAL              TPP_FORWARD_COMPAT(TOK_NOT_EQUAL)
#define TPP_TOK_RANGLE_EQUAL               TPP_FORWARD_COMPAT(TOK_GREATER_EQUAL)
#define TPP_TOK_LANGLE_EQUAL               TPP_FORWARD_COMPAT(TOK_LOWER_EQUAL)
#define TPP_TOK_DOT_DOT_DOT                TPP_FORWARD_COMPAT(TOK_DOTS)
#define TPP_TOK_PLUS_EQUAL                 TPP_FORWARD_COMPAT(TOK_ADD_EQUAL)
#define TPP_TOK_MINUS_EQUAL                TPP_FORWARD_COMPAT(TOK_SUB_EQUAL)
#define TPP_TOK_STAR_EQUAL                 TPP_FORWARD_COMPAT(TOK_MUL_EQUAL)
#define TPP_TOK_SLASH_EQUAL                TPP_FORWARD_COMPAT(TOK_DIV_EQUAL)
#define TPP_TOK_PERCENT_EQUAL              TPP_FORWARD_COMPAT(TOK_MOD_EQUAL)
#define TPP_TOK_LANGLE_LANGLE_EQUAL        TPP_FORWARD_COMPAT(TOK_SHL_EQUAL)
#define TPP_TOK_RANGLE_RANGLE_EQUAL        TPP_FORWARD_COMPAT(TOK_SHR_EQUAL)
#define TPP_TOK_AMP_EQUAL                  TPP_FORWARD_COMPAT(TOK_AND_EQUAL)
#define TPP_TOK_PIPE_EQUAL                 TPP_FORWARD_COMPAT(TOK_OR_EQUAL)
#define TPP_TOK_HAT_EQUAL                  TPP_FORWARD_COMPAT(TOK_XOR_EQUAL)
#define TPP_TOK_STAR_STAR_EQUAL            TPP_FORWARD_COMPAT(TOK_POW_EQUAL)
#define TPP_TOK_AT_EQUAL                   TPP_FORWARD_COMPAT(TOK_AT_EQUAL)
#define TPP_TOK_POUND_POUND                TPP_FORWARD_COMPAT(TOK_GLUE)
#define TPP_TOK_AMP_AMP                    TPP_FORWARD_COMPAT(TOK_LAND)
#define TPP_TOK_PIPE_PIPE                  TPP_FORWARD_COMPAT(TOK_LOR)
#define TPP_TOK_HAT_HAT                    TPP_FORWARD_COMPAT(TOK_LXOR)
#define TPP_TOK_PLUS_PLUS                  TPP_FORWARD_COMPAT(TOK_INC)
#define TPP_TOK_MINUS_MINUS                TPP_FORWARD_COMPAT(TOK_DEC)
#define TPP_TOK_STAR_STAR                  TPP_FORWARD_COMPAT(TOK_POW)
#define TPP_TOK_MINUS_RANGLE               TPP_FORWARD_COMPAT(TOK_ARROW)
#define TPP_TOK_COLON_COLON                TPP_FORWARD_COMPAT(TOK_NAMESPACE)
#define TPP_TOK_COLON_EQUAL                TPP_FORWARD_COMPAT(TOK_COLON_EQUAL)
#define TPP_TOK_MINUS_RANGLE_STAR          TPP_FORWARD_COMPAT(TOK_ARROW_STAR)
#define TPP_TOK_DOT_DOT                    TPP_FORWARD_COMPAT(TOK_DOTDOT)
#define TPP_TOK_LANGLE_RANGLE              TPP_FORWARD_COMPAT(TOK_LOGT)
#define TPP_TOK_LANGLE_LANGLE_LANGLE       TPP_FORWARD_COMPAT(TOK_LANGLE3)
#define TPP_TOK_RANGLE_RANGLE_RANGLE       TPP_FORWARD_COMPAT(TOK_RANGLE3)
#define TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL TPP_FORWARD_COMPAT(TOK_LANGLE3_EQUAL)
#define TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL TPP_FORWARD_COMPAT(TOK_RANGLE3_EQUAL)
#define TPP_TOK_EQUAL_EQUAL_EQUAL          TPP_FORWARD_COMPAT(TOK_EQUAL3)
#define TPP_TOK_EXCLAIM_EQUAL_EQUAL        TPP_FORWARD_COMPAT(TOK_NOT_EQUAL3)
#define TPP_TOK_QMARK_QMARK                TPP_FORWARD_COMPAT(TOK_QMARK_QMARK)
#define TPP_TOK_KEYWORD_BEGIN              TPP_FORWARD_COMPAT(TOK_KEYWORD_BEGIN)
#define TPP_TOK_MULTICHAR_END              TPP_FORWARD_COMPAT(TOK_TWOCHAR_END)

#define TPP_TOK_ISERR(id)         ((int)(id) < 0)
#define TPP_TOK_ISERR_OR_EOF(id)  ((int)(id) <= 0)
#define TPP_TOK_OFERR(err)        ((tpp_token_id)(int)(err))
#define TPP_TOK_OFERR_OR_EOF(err) ((tpp_token_id)(int)(err))
#define TPP_TOK_ASERR(id)         ((tpp_errno)(int)(id))
#define TPP_TOK_ASERR_OR_EOK(id)  (TPP_TOK_ISERR(id) ? TPP_TOK_ASERR(id) : TPP_EOK)
#define TPP_TOK_OFCHAR(ch)        ((tpp_token_id)(unsigned int)(tpp_char)(ch))
#define TPP_TOK_ISCHAR(id)        ((unsigned int)(id) <= 0xff)

#define TPP_CASE_TPP_TOK_INT           case TPP_FORWARD_COMPAT(TOK_INT):
#define TPP_TOK_ISINT(id)              ((id) == TPP_FORWARD_COMPAT(TOK_INT))
#define TPP_CASE_TPP_TOK_FLOAT         case TPP_FORWARD_COMPAT(TOK_FLOAT):
#define TPP_TOK_ISFLOAT(id)            ((id) == TPP_FORWARD_COMPAT(TOK_FLOAT))
#define TPP_CASE_TPP_TOK_NUMBER        TPP_CASE_TPP_TOK_INT TPP_CASE_TPP_TOK_FLOAT
#define TPP_TOK_ISNUMBER(id)           (TPP_TOK_ISINT(id) || TPP_TOK_ISFLOAT(id))
#define TPP_TOK_ISSTRING_DQUOTE(id)    ((id) == TPP_FORWARD_COMPAT(TOK_STRING))
#define TPP_TOK_ISSTRING_SQUOTE(id)    ((id) == TPP_FORWARD_COMPAT(TOK_CHAR))
#define TPP_CASE_TPP_TOK_STRING_DQUOTE case TPP_FORWARD_COMPAT(TOK_STRING):
#define TPP_CASE_TPP_TOK_STRING_SQUOTE case TPP_FORWARD_COMPAT(TOK_CHAR):
#define TPP_TOK_ISSTRING(id)           (TPP_TOK_ISSTRING_DQUOTE(id) || TPP_TOK_ISSTRING_SQUOTE(id))
#define TPP_CASE_TPP_TOK_STRING        TPP_CASE_TPP_TOK_STRING_DQUOTE TPP_CASE_TPP_TOK_STRING_SQUOTE
#define TPP_TOK_ISCOMMENT(id)          ((id) == TPP_FORWARD_COMPAT(TOK_COMMENT))
#define TPP_CASE_TPP_TOK_COMMENT       case TPP_FORWARD_COMPAT(TOK_COMMENT):
#define TPP_TOK_ISCOMMENT_LINE(id)     TPP_TOK_ISCOMMENT(id) /* !!! Semantic difference: this also matches line-comments */
#define TPP_TOK_ISCOMMENT_NOLINE(id)   TPP_TOK_ISCOMMENT(id) /* !!! Semantic difference: this also matches block-comments */

#define TPP_TOK_ISSPACE_OR_COMMENT(id)              ((id) == TPP_TOK_SPACE || TPP_TOK_ISCOMMENT_NOLINE(id))
#define TPP_TOK_ISLF_OR_COMMENT(id)                 ((id) == TPP_TOK_LF || TPP_TOK_ISCOMMENT_LINE(id))
#define TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(id)        ((id) == TPP_TOK_SPACE || (id) == TPP_TOK_LF || TPP_TOK_ISCOMMENT(id))
#define TPP_TOK_ISLF_OR_COMMENT_OR_EOF(id)          ((id) == TPP_TOK_LF || TPP_TOK_ISCOMMENT_LINE(id) || (id) == TPP_TOK_EOF)
#define TPP_TOK_ISSPACE_OR_LF_OR_COMMENT_OR_EOF(id) ((id) == TPP_TOK_SPACE || (id) == TPP_TOK_LF || TPP_TOK_ISCOMMENT(id) || (id) == TPP_TOK_EOF)
#define TPP_TOK_ISKEYWORD(id)                       TPP_ISKEYWORD(id)
#define TPP_TOK_ISUSERKEYWORD(id)                   TPP_ISUSERKEYWORD(id)
#define TPP_TOK_ISBUILTINKEYWORD(id)                (TPP_TOK_ISKEYWORD(id) && !TPP_TOK_ISUSERKEYWORD(id))

#define tpp_token_haskwd(self)     TPP_TOK_ISKEYWORD(tpp_token_getid(self))
#define tpp_token_getid(self)      ((self)->t_id)
#define tpp_token_getkwd(self)     ((self)->t_kwd) /* Only valid when `tpp_token_haskwd(self)` */
#define tpp_token_getstart(self)   ((tpp_char const *)(self)->t_begin)
#define tpp_token_getend(self)     ((tpp_char const *)(self)->t_end) /* WARNING: Don't dereference -- pointed-to memory may not have been loaded! */
#define tpp_token_getlen(self)     ((tpp_size)(tpp_token_getend(self) - tpp_token_getstart(self)))
#define tpp_token_getkwdcstr(self) ((self)->t_kwd->k_name)
#define tpp_token_getkwdstr(self)  ((tpp_char const *)(self)->t_kwd->k_name)
#define tpp_token_getkwdlen(self)  ((self)->t_kwd->k_size)
#define tpp_token_setid(self, id)   (void)((self)->t_id = (id))
#define tpp_token_setkwd(self, kwd) (void)((self)->t_id = ((self)->t_kwd = (kwd))->k_id)
/* Not exposed since this wouldn't also set the position in the linked file.
 * Use `tpp_lexer_settokenrange()` or `tpp_lexer_settokenend()` instead! */
/*#define tpp_token_setrange(self, start, end)  \
	(void)((self)->t_begin = (char *)(start), \
	       (self)->t_end   = (char *)(end))*/

/* Convenience aliases */
#define tpp_token_iseof(self)                    (tpp_token_getid(self) == TPP_TOK_EOF)
#define tpp_token_isspace_or_comment(self)       TPP_TOK_ISSPACE_OR_COMMENT(tpp_token_getid(self))
#define tpp_token_islf_or_comment(self)          TPP_TOK_ISLF_OR_COMMENT(tpp_token_getid(self))
#define tpp_token_isspace_or_lf_or_comment(self) TPP_TOK_ISSPACE_OR_LF_OR_COMMENT(tpp_token_getid(self))
#define tpp_token_iskeyword(self)                TPP_TOK_ISKEYWORD(tpp_token_getid(self))
#define tpp_token_isuserkeyword(self)            TPP_TOK_ISUSERKEYWORD(tpp_token_getid(self))
#define tpp_token_isbuiltinkeyword(self)         TPP_TOK_ISBUILTINKEYWORD(tpp_token_getid(self))
#define tpp_token_iscomment(self)                TPP_TOK_ISCOMMENT(tpp_token_getid(self))
#define tpp_token_iscomment_line(self)           TPP_TOK_ISCOMMENT_LINE(tpp_token_getid(self))
#define tpp_token_iscomment_noline(self)         TPP_TOK_ISCOMMENT_NOLINE(tpp_token_getid(self))
#define tpp_token_isstring(self)                 TPP_TOK_ISSTRING(tpp_token_getid(self))




/************************************************************************/
/* "parts/file.h"                                                    */
/************************************************************************/

#define tpp_file struct TPPFile

#define TPP_FILE_FLAGS_NOGUARD TPP_TEXTFILE_FLAG_NOGUARD
#define TPP_FILE_FLAGS_SYSHDR  TPP_TEXTFILE_FLAG_SYSHEADER
#ifdef TPP_CONFIG_NONBLOCKING_IO
#define TPP_FILE_FLAGS_NONBLOCK TPP_TEXTFILE_FLAG_NONBLOCK
#endif /* TPP_CONFIG_NONBLOCKING_IO */

#define tpp_file_isutf8(self)   1
#define tpp_file_isascii(self)  0

#define TPP_FILE_KIND_IO    TPPFILE_KIND_TEXT
#define TPP_FILE_KIND_TEXT  TPPFILE_KIND_EXPLICIT
#define TPP_FILE_KIND_MACRO TPPFILE_KIND_MACRO

#define tpp_file_getkind(self)   ((self)->f_kind)
#define tpp_file_getpos(self)    ((tpp_char const *)(self)->f_pos)
#define tpp_file_getend(self)    ((tpp_char const *)(self)->f_end)
#define tpp_file_getchunk(self)  ((self)->f_text)
#define tpp_file_haschunk(self)  1 /* ((self)->f_text != NULL) */
#define tpp_file_haslcinfo(self) ((self)->f_kind != TPPFILE_KIND_EXPLICIT)
#define tpp_file_ismacro(self)   (tpp_file_getkind(self) == TPP_FILE_KIND_MACRO)
#define tpp_file_getflags(self) ((self)->f_textfile.f_flags)

#ifdef TPP_CONFIG_NONBLOCKING_IO
#define tpp_file_getnonblock(self) (tpp_file_getflags(self) & TPP_FILE_FLAGS_NONBLOCK)
#define tpp_file_setnonblock(self, v)    \
	((v) ? tpp_file_enablenonblock(self) \
	     : tpp_file_disablenonblock(self))
#define tpp_file_enablenonblock(self)                              \
	(void)(tpp_assert(tpp_file_getkind(self) == TPP_FILE_KIND_IO), \
	       (self)->f_textfile.f_flags |= TPP_FILE_FLAGS_NONBLOCK)
#define tpp_file_disablenonblock(self)                             \
	(void)(tpp_assert(tpp_file_getkind(self) == TPP_FILE_KIND_IO), \
	       (self)->f_textfile.f_flags &= ~TPP_FILE_FLAGS_NONBLOCK)
#else /* TPP_CONFIG_NONBLOCKING_IO */
#define tpp_file_getnonblock(self)     false
#define tpp_file_disablenonblock(self) (void)0
#endif /* !TPP_CONFIG_NONBLOCKING_IO */
#define tpp_file_getsyshdr(self)     (tpp_file_getflags(self) & TPP_FILE_FLAGS_SYSHDR)
#define tpp_file_setsyshdr(self, v)  ((v) ? tpp_file_enablesyshdr(self) : tpp_file_disablesyshdr(self))
#define tpp_file_enablesyshdr(self)  (void)((self)->f_textfile.f_flags |= TPP_FILE_FLAGS_SYSHDR)
#define tpp_file_disablesyshdr(self) (void)((self)->f_textfile.f_flags &= ~TPP_FILE_FLAGS_SYSHDR)
#define tpp_file_getsystemheader(self) tpp_file_getsyshdr(self)
#define tpp_file_getprev(self) (self)->f_prev
#define tpp_file_isbasefile(self) ((self)->f_prev == NULL)

TPP_INLINE /*TPP_WUNUSED TPP_NONNULL((1))*/ tpp_lcinfo TPPCALL
tpp_file_getlcinfo(tpp_file *tpp_restrict self, tpp_char const *pos) {
	tpp_lcinfo result;
	TPPFile_LCAt(self, &result, (char const *)pos);
	return result;
}
#define tpp_file_getendlcinfo(self)    tpp_file_getlcinfo(self, tpp_file_getpos(self))
#define tpp_file_getrealfilename(self) TPPFile_RealFilename(self, NULL)
#define tpp_file_getfilename(self)     TPPFile_Filename(self, NULL)

TPP_INLINE /*TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1))*/ tpp_file *TPPCALL
tpp_file_gettextfile(tpp_file const *tpp_restrict self) {
	tpp_file *result = (tpp_file *)self;
	while (result && result->f_kind != TPPFILE_KIND_TEXT)
		result = result->f_prev;
	return result;
}
TPP_INLINE /*TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1))*/ tpp_file *TPPCALL
tpp_file_getlcfile(tpp_file const *tpp_restrict self) {
	tpp_file *result = tpp_file_gettextfile(self);
	return result ? result : (tpp_file *)self;
}
TPP_INLINE /*TPP_RETNONNULL TPP_WUNUSED TPP_NONNULL((1))*/ tpp_file *TPPCALL
tpp_file_getbasefile(tpp_file const *tpp_restrict self) {
	tpp_file *result = (tpp_file *)self;
	while (result->f_prev)
		result = result->f_prev;
	return result;
}

typedef struct tpp_lcinfo_ex {
	tpp_lcinfo tlcix_info; /* Line/column information, or `TPP_LCINFO_INVALID` if unknown */
} tpp_lcinfo_ex;
#define tpp_file_getlcinfo_ex(self, pos, result) \
	TPPFile_LCAt(self, &(result)->tlcix_info, (char const *)pos)



/************************************************************************/
/* "parts/keyword.h"                                                    */
/************************************************************************/

#define tpp_keyword                 struct TPPKeyword
#define tpp_keyword_equals(a, b)    ((a) == (b))
#define tpp_keyword_isuser(self)    TPP_ISUSERKEYWORD((self)->k_id)
#define tpp_keyword_isbuiltin(self) (!tpp_keyword_isuser(self))
#define tpp_keyword_getid(self)     ((self)->k_id)
#define tpp_keyword_getstr(self)    ((tpp_char const *)(self)->k_name)
#define tpp_keyword_getcstr(self)   ((self)->k_name)
#define tpp_keyword_getlen(self)    ((self)->k_size)
#define tpp_keyword_gethash(self)   ((self)->k_hash)
#define tpp_keyword_hasmacro(self)  ((self)->k_macro != NULL)
#define tpp_keyword_fromstr(str)    ((tpp_keyword const *)tpp_container_of((char *)(str), struct TPPKeyword, k_name))
#define tpp_keyword_fromcstr(cstr)  ((tpp_keyword const *)tpp_container_of(str, struct TPPKeyword, k_name))
#define tpp_keyword_equals_conststr(self, CONSTstr)                       \
	(tpp_keyword_getlen(self) == (sizeof(CONSTstr) / sizeof(char)) - 1 && \
	 tpp_memcmp(tpp_keyword_getstr(self), CONSTstr, sizeof(CONSTstr) - sizeof(char)) == 0)
#define tpp_keyword_equals_str(self, /*tpp_char const **/ str, len) \
	(tpp_keyword_getlen(self) == (len) &&                           \
	 tpp_memcmp(tpp_keyword_getstr(self), str, (len) * sizeof(char)) == 0)
#define tpp_keyword_equals_cstr(self, /*char const **/ cstr, len) \
	tpp_keyword_equals_str(self, cstr, len)

#define TPP_HASH_INITIAL 1
#define tpp_hash_combine_char(a, b) ((a) * 263 + /*(tpp_char)*/(b))
#define tpp_hash_combine_hash(a, b) ((a) * 263 + /*(tpp_hash)*/(b))
#define tpp_hashof(kwd, len) TPP_Hashof(kwd, len)

#define tpp_keywords struct TPPKeywordMap




/************************************************************************/
/* "parts/lexer.h"                                                      */
/************************************************************************/

#define TPP_LEXER_STATE_FLAG_NORMAL     TPPLEXER_FLAG_NONE
#define TPP_LEXER_STATE_FLAG_NOWARNINGS TPPLEXER_FLAG_NO_WARNINGS

#define tpp_lexer struct TPPLexer

#define tpp_lexer_geterrorcount(self)    (self)->l_errorcount
#define tpp_lexer_seterrorcount(self, v) (void)((self)->l_errorcount = (v))
#define tpp_lexer_reseterrorcount(self)  (void)((self)->l_errorcount = 0)

#define tpp_lexer_geterrorlimit(self)    ((self)->l_maxerrors)
#define tpp_lexer_seterrorlimit(self, v) (void)((self)->l_maxerrors = (v))
#define tpp_lexer_reseterrorlimit(self)  (void)((self)->l_maxerrors = TPPLEXER_DEFAULT_LIMIT_ECNT)

#define tpp_lexer_getwarningcount(self)    (self)->l_warncount
#define tpp_lexer_setwarningcount(self, v) (void)((self)->l_warncount = (v))
#define tpp_lexer_resetwarningcount(self)  (void)((self)->l_warncount = 0)

#define tpp_lexer_getinclusionlimit(self)    ((self)->l_limit_incl)
#define tpp_lexer_setinclusionlimit(self, v) (void)((self)->l_limit_incl = (v))
#define tpp_lexer_resetinclusionlimit(self)  (void)((self)->l_limit_incl = -TPP_MAX_INCLUDE_DEPTH)

#define tpp_lexer_getrecursivemacrolimit(self)    ((self)->l_limit_mrec)
#define tpp_lexer_setrecursivemacrolimit(self, v) (void)((self)->l_limit_mrec = (v))
#define tpp_lexer_resetrecursivemacrolimit(self)  (void)((self)->l_limit_mrec = -TPP_MAX_RECURSIVE_MACRO_DEPTH)

#define tpp_lexer_getnextcounter(self)    ((self)->l_counter)
#define tpp_lexer_setnextcounter(self, v) (void)((self)->l_counter = (v))
#define tpp_lexer_resetnextcounter(self)  (void)((self)->l_counter = 0)

#define tpp_lexer_gettok(self)          ((self)->l_token.t_id)
#define tpp_lexer_gettoken(self)        (&(self)->l_token)
#define tpp_lexer_hastokenkwd(self)     TPP_ISKEYWORD(tpp_lexer_gettok(self))
#define tpp_lexer_gettokenkwd(self)     ((self)->l_token.t_kwd)
#define tpp_lexer_gettokenkwdcstr(self) ((self)->l_token.t_kwd->k_name)
#define tpp_lexer_gettokenkwdstr(self)  ((tpp_char const *)(self)->l_token.t_kwd->k_name)
#define tpp_lexer_gettokenkwdlen(self)  ((self)->l_token.t_kwd->k_size)
#define tpp_lexer_gettokenstart(self)   ((tpp_char const *)(self)->l_token.t_begin)
#define tpp_lexer_gettokenend(self)     ((tpp_char const *)(self)->l_token.t_end)
#define tpp_lexer_gettokenlen(self)     ((tpp_size)(tpp_lexer_gettokenend(self) - tpp_lexer_gettokenstart(self)))
#define tpp_lexer_settokenid(self, id)  (void)((self)->l_token.t_id = (id))
#define tpp_lexer_settokenrange(self, start, end)           \
	(void)((self)->l_token.t_begin       = (char *)(start), \
	       (self)->l_token.t_file->f_pos = ((self)->l_token.t_end = (char *)(end)))
#define tpp_lexer_settokenend(self, end) \
	(void)((self)->l_token.t_file->f_pos = ((self)->l_token.t_end = (char *)(end)))

/* Current file */
#define tpp_lexer_getfile(self)     ((self)->l_token.t_file)
#define tpp_lexer_getfilekind(self) tpp_file_getkind(tpp_lexer_getfile(self))
#define tpp_lexer_getlcfile(self)   tpp_file_getlcfile(tpp_lexer_getfile(self))   /* [1..1] Returns the file that is used to determine `__LINE__` and `__COLUMN__` */
#define tpp_lexer_gettextfile(self) tpp_file_gettextfile(tpp_lexer_getfile(self)) /* [0..1] Return the last I/O or TEXT file */
#define tpp_lexer_getbasefile(self) tpp_file_getbasefile(tpp_lexer_getfile(self)) /* [1..1] Return the *base* file (that is: the last one in the `#include`-stack) */


#define tpp_lexer_getlcinfoat(self, pos)                 tpp_file_getlcinfo(tpp_lexer_getfile(self), pos)
#define tpp_lexer_getlcinfoat_ex(self, pos, result)      tpp_file_getlcinfo_ex(tpp_lexer_getfile(self), pos, result)
#define tpp_lexer_getlcinfoattokenstart(self)            tpp_lexer_getlcinfoat(self, tpp_lexer_gettokenstart(self))
#define tpp_lexer_getlcinfoattokenstart_ex(self, result) tpp_lexer_getlcinfoat_ex(self, tpp_lexer_gettokenstart(self), result)
#define tpp_lexer_getlcinfoattokenend(self)              tpp_lexer_getlcinfoat(self, tpp_lexer_gettokenend(self))
#define tpp_lexer_getlcinfoattokenend_ex(self, result)   tpp_lexer_getlcinfoat_ex(self, tpp_lexer_gettokenend(self), result)

/* Convenience L/C information helpers.
 * If you don't want to bother learning what all the above does, then it's these that
 * you want to use -- these return the values as reported by __LINE__ and __COLUMN__. */
#define tpp_lexer_getlcfilename(self)  tpp_file_getfilename(tpp_lexer_getlcfile(self))    /* [0..1] Value of `__FILE__` */
#define tpp_lexer_getendlcinfo(self)   tpp_file_getendlcinfo(tpp_lexer_getlcfile(self))   /* Theoretical value of `__LINE__` / `__COLUMN__` at end of current token */
#define tpp_lexer_getlcinfo(self)      tpp_lexer_getstartlcinfo(self)                     /* Convenience alias to make it clear what you want to use */
TPP_INLINE tpp_lcinfo tpp_lexer_getstartlcinfo(tpp_lexer *self) {
	tpp_file *const lcfile = tpp_lexer_getlcfile(self);
	tpp_file const *const curfile = tpp_lexer_getfile(self);
	tpp_char const *const pos = curfile == lcfile ? tpp_lexer_gettokenstart(self) : tpp_file_getpos(lcfile);
	return tpp_file_getlcinfo(lcfile, pos);
}

/* Wrappers for keywords API */
#if TPP_CONFIG_ONELEXER == 3
#define tpp_lexer_getkeyword_byid(self, id)               ((tpp_keyword const *)TPPLexer_LookupKeywordID_(self, id))
#define tpp_lexer_getkeyword(self, kwd, len)              ((tpp_keyword const *)TPPLexer_LookupKeyword_(self, kwd, len, 0))
#define tpp_lexer_getkeyword_ex(self, kwd, len, hash)     ((tpp_keyword const *)TPPLexer_LookupKeyword_(self, kwd, len, 0))
#define tpp_lexer_newkeyword(self, kwd, len)              ((tpp_keyword const *)TPPLexer_LookupKeyword_(self, kwd, len, 1))
#define tpp_lexer_newkeyword_ex(self, kwd, len, hash)     ((tpp_keyword const *)TPPLexer_LookupKeyword_(self, kwd, len, 1))
#define tpp_lexer_getkeyword_esc(self, kwd, len)          ((tpp_keyword const *)TPPLexer_LookupEscapedKeyword_(self, kwd, len, 0))
#define tpp_lexer_getkeyword_esc_ex(self, kwd, len, hash) ((tpp_keyword const *)TPPLexer_LookupEscapedKeyword_(self, kwd, len, 0))
#define tpp_lexer_newkeyword_esc(self, kwd, len)          ((tpp_keyword const *)TPPLexer_LookupEscapedKeyword_(self, kwd, len, 1))
#define tpp_lexer_newkeyword_esc_ex(self, kwd, len, hash) ((tpp_keyword const *)TPPLexer_LookupEscapedKeyword_(self, kwd, len, 1))
#else /* TPP_CONFIG_ONELEXER == 3 */
#define tpp_lexer_getkeyword_byid(self, id)               ((void)(self), (tpp_keyword const *)TPPLexer_LookupKeywordID(id))
#define tpp_lexer_getkeyword(self, kwd, len)              ((void)(self), (tpp_keyword const *)TPPLexer_LookupKeyword(kwd, len, 0))
#define tpp_lexer_getkeyword_ex(self, kwd, len, hash)     ((void)(self), (tpp_keyword const *)TPPLexer_LookupKeyword(kwd, len, 0))
#define tpp_lexer_newkeyword(self, kwd, len)              ((void)(self), (tpp_keyword const *)TPPLexer_LookupKeyword(kwd, len, 1))
#define tpp_lexer_newkeyword_ex(self, kwd, len, hash)     ((void)(self), (tpp_keyword const *)TPPLexer_LookupKeyword(kwd, len, 1))
#define tpp_lexer_getkeyword_esc(self, kwd, len)          ((void)(self), (tpp_keyword const *)TPPLexer_LookupEscapedKeyword(kwd, len, 0))
#define tpp_lexer_getkeyword_esc_ex(self, kwd, len, hash) ((void)(self), (tpp_keyword const *)TPPLexer_LookupEscapedKeyword(kwd, len, 0))
#define tpp_lexer_newkeyword_esc(self, kwd, len)          ((void)(self), (tpp_keyword const *)TPPLexer_LookupEscapedKeyword(kwd, len, 1))
#define tpp_lexer_newkeyword_esc_ex(self, kwd, len, hash) ((void)(self), (tpp_keyword const *)TPPLexer_LookupEscapedKeyword(kwd, len, 1))
#endif /* TPP_CONFIG_ONELEXER != 3 */
#define tpp_lexer_copybuiltinkwd(self, kwd) ((tpp_keyword *)(kwd))

/* Lexer component reset */
#define tpp_lexer_resetextensions(self)     TPPLexer_Reset(self, TPPLEXER_RESET_ESTATE)
#define tpp_lexer_resetwarnings(self)       TPPLexer_Reset(self, TPPLEXER_RESET_WSTATE)
#define tpp_lexer_resetincludes(self)       TPPLexer_Reset(self, TPPLEXER_RESET_SYSPATHS)
#define tpp_lexer_resetallkwds(self)        TPPLexer_Reset(self, TPPLEXER_RESET_KEYWORDS)
#define tpp_lexer_undefalluser(self)        TPPLexer_Reset(self, TPPLEXER_RESET_MACRO)
#define tpp_lexer_unassertallkwds(self)     TPPLexer_Reset(self, TPPLEXER_RESET_ASSERT)
#define tpp_lexer_resetallkwdcounters(self) TPPLexer_Reset(self, TPPLEXER_RESET_COUNTER)


#define tpp_lexer_has(self, feat) tpp_lexer_getextension(self, TPP_EXT_##feat)
#define tpp_lexer_getextension(self, TPP_EXT_x)     ((self)->l_extensions.es_bitset[(TPP_EXT_x) / 8] & (1 << ((TPP_EXT_x) % 8)))
#define tpp_lexer_enableextension(self, TPP_EXT_x)  (void)((self)->l_extensions.es_bitset[(TPP_EXT_x) / 8] |= (1 << ((TPP_EXT_x) % 8)))
#define tpp_lexer_disableextension(self, TPP_EXT_x) (void)((self)->l_extensions.es_bitset[(TPP_EXT_x) / 8] &= ~(1 << ((TPP_EXT_x) % 8)))
#define tpp_lexer_setextension(self, TPP_EXT_x, enabled)    \
	((enabled) ? tpp_lexer_enableextension(self, TPP_EXT_x) \
	           : tpp_lexer_disableextension(self, TPP_EXT_x))


/* Define (or override) a macro `macro_name` with a body definition `macro_body`
 * When `macro_name` contains an opening `(` character, it, as well as `macro_body`
 * are parsed as a function-like macro. The same also goes for `{`, `[` and `<`
 * when `TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS` is enabled.
 *
 * @param: macro_name:        Name of the macro to define. This string may contain
 *                            a `(`-character following the macro's name, in which
 *                            case the remainder of this string acts as a parameter
 *                            list for a function-like macro definition.
 * @param: macro_name_maxlen: Max length of `macro_name` (in bytes). Used in a call
 *                            too `tpp_strnlen()`, so you may also pass `TPP_SIZE_MAX`
 *                            if you know that `macro_name` is NUL-terminated.
 * @param: macro_body:        Body of the macro to define.
 * @param: macro_body_maxlen: Max length of `macro_body` (in bytes). Used in a call
 *                            too `tpp_strnlen()`, so you may also pass `TPP_SIZE_MAX`
 *                            if you know that `macro_body` is NUL-terminated.
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_CONFIG_ONELEXER == 3
#define tpp_lexer_define(self, macro_name, macro_name_maxlen, macro_body, macro_body_maxlen) \
	(TPPLexer_Define_(self, macro_name, macro_name_maxlen,                                   \
	                  macro_body, macro_body_maxlen,                                         \
	                  TPPLEXER_DEFINE_FLAG_NONE) == 0                                        \
	 ? TPP_ENOMEM                                                                            \
	 : TPP_EOK)
#else  /* TPP_CONFIG_ONELEXER == 3 */
#define tpp_lexer_define(self, macro_name, macro_name_maxlen, macro_body, macro_body_maxlen) \
	((void)(self), TPPLexer_Define(macro_name, macro_name_maxlen,                            \
	                               macro_body, macro_body_maxlen,                            \
	                               TPPLEXER_DEFINE_FLAG_NONE) == 0                           \
	               ? TPP_ENOMEM                                                              \
	               : TPP_EOK)
#endif /* TPP_CONFIG_ONELEXER != 3 */

/* Delete a macro definition
 * @param: macro_name:        Name of the macro to undefine.
 * @param: macro_name_maxlen: Max length of `macro_name` (in bytes). Used in a call
 *                            too `tpp_strnlen()`, so you may also pass `TPP_SIZE_MAX`
 *                            if you know that `macro_name` is NUL-terminated.
 * @return: true:  Success
 * @return: false: No such macro */
#if TPP_CONFIG_ONELEXER == 3
#define tpp_lexer_undef(self, macro_name, macro_name_maxlen) \
	(!!TPPLexer_Undef_(self, macro_name, macro_name_maxlen))
#else /* TPP_CONFIG_ONELEXER == 3 */
#define tpp_lexer_undef(self, macro_name, macro_name_maxlen) \
	((void)(self), !!TPPLexer_Undef(macro_name, macro_name_maxlen))
#endif /* TPP_CONFIG_ONELEXER != 3 */

#ifndef TPP_CONFIG_NO_ASSERTIONS
/* Add a new keyword assertions for `key` and `value`.
 * This is the same as doing `#assert {key}({value})`
 * @return: TPP_EOK:    Success
 * @return: TPP_ENOMEM: Out of memory */
#if TPP_CONFIG_ONELEXER == 3
#define tpp_lexer_assert(self, key, key_maxlen, value, value_maxlen) \
	(TPPLexer_AddAssert_(self, macro_name, macro_name_maxlen,        \
	                     macro_body, macro_body_maxlen) == 0         \
	 ? TPP_ENOMEM                                                    \
	 : TPP_EOK)
#else  /* TPP_CONFIG_ONELEXER == 3 */
#define tpp_lexer_assert(self, key, key_maxlen, value, value_maxlen)      \
	((void)(self), TPPLexer_AddAssert(macro_name, macro_name_maxlen,      \
	                                  macro_body, macro_body_maxlen) == 0 \
	               ? TPP_ENOMEM                                           \
	               : TPP_EOK)
#endif /* TPP_CONFIG_ONELEXER != 3 */


/* Delete a new keyword assertions for `key` and `value`.
 * This is the same as doing `#unassert {key}({value})`
 * @return: true:  Success
 * @return: false: No such assertion */
#if TPP_CONFIG_ONELEXER == 3
#define tpp_lexer_unassert(self, key, key_maxlen, value, value_maxlen) \
	(!!TPPLexer_DelAssert_(self, macro_name, macro_name_maxlen,        \
	                       macro_body, macro_body_maxlen))
#else /* TPP_CONFIG_ONELEXER == 3 */
#define tpp_lexer_unassert(self, key, key_maxlen, value, value_maxlen) \
	((void)(self), !!TPPLexer_DelAssert(macro_name, macro_name_maxlen, \
	                                    macro_body, macro_body_maxlen))
#endif /* TPP_CONFIG_ONELEXER != 3 */
#endif /* !TPP_CONFIG_NO_ASSERTIONS */




/* Temporarily modify lexer state flags */
#define _tpp_lexer_pushstate(self, mask, flags)                        \
	do {                                                               \
		tpp_lexer_state_flags const _tlsp_old_flags = (self)->l_flags; \
		(self)->l_flags = ((self)->l_flags & (tpp_lexer_state_flags)(mask)) | (tpp_lexer_state_flags)(flags)
#define _tpp_lexer_enablestate(self, state)  (void)((self)->l_flags |= (state))
#define _tpp_lexer_disablestate(self, state) (void)((self)->l_flags &= ~(state))
#define _tpp_lexer_breakstate(self) \
		(void)((self)->l_flags = _tlsp_old_flags)
#define _tpp_lexer_popstate(self)    \
		_tpp_lexer_breakstate(self); \
	} while (0)
#define _tpp_lexer_pushstate_on(self, flags)  _tpp_lexer_pushstate(self, ~0, flags)
#define _tpp_lexer_pushstate_off(self, flags) _tpp_lexer_pushstate(self, ~(flags), 0)

/* Alter the lexer state such that no warning messages are produces. */
#define tpp_lexer_nowarnings_ison(self)   ((self)->l_flags & TPP_LEXER_STATE_FLAG_NOWARNINGS))
#define tpp_lexer_nowarnings_pushon(self) _tpp_lexer_pushstate_on(self, TPP_LEXER_STATE_FLAG_NOWARNINGS)
#define tpp_lexer_nowarnings_break(self)  _tpp_lexer_breakstate(self)
#define tpp_lexer_nowarnings_pop(self)    _tpp_lexer_popstate(self)


#if TPP_CONFIG_ONELEXER == 3
#define tpp_lexer_yieldraw_blocking(self) TPPLexer_YieldRaw_(self)
#define tpp_lexer_yieldpp_blocking(self)  TPPLexer_YieldPP_(self)
#define tpp_lexer_yield_blocking(self)    TPPLexer_Yield_(self)
#else /* TPP_CONFIG_ONELEXER == 3 */
#define tpp_lexer_yieldraw_blocking(self) ((void)(self), TPPLexer_YieldRaw())
#define tpp_lexer_yieldpp_blocking(self)  ((void)(self), TPPLexer_YieldPP())
#define tpp_lexer_yield_blocking(self)    ((void)(self), TPPLexer_Yield())
#endif /* TPP_CONFIG_ONELEXER != 3 */

#if TPP_HAVE_FILE_NONBLOCK
#if TPP_CONFIG_ONELEXER == 3
#define tpp_lexer_yieldraw(self) TPPLexer_YieldRawNB_(self)
#define tpp_lexer_yieldpp(self)  TPPLexer_YieldPPNB_(self)
#define tpp_lexer_yield(self)    TPPLexer_YieldNB_(self)
#else /* TPP_CONFIG_ONELEXER == 3 */
#define tpp_lexer_yieldraw(self) ((void)(self), TPPLexer_YieldRawNB())
#define tpp_lexer_yieldpp(self)  ((void)(self), TPPLexer_YieldPPNB())
#define tpp_lexer_yield(self)    ((void)(self), TPPLexer_YieldNB())
#endif /* TPP_CONFIG_ONELEXER != 3 */
#else /* TPP_HAVE_FILE_NONBLOCK */
#define tpp_lexer_yieldraw(self) tpp_lexer_yieldraw_blocking(self)
#define tpp_lexer_yieldpp(self)  tpp_lexer_yieldpp_blocking(self)
#define tpp_lexer_yield(self)    tpp_lexer_yield_blocking(self)
#endif /* !TPP_HAVE_FILE_NONBLOCK */

#if TPP_CONFIG_ONELEXER == 3
#define tpp_lexer_warnf(self, ...) (TPPLexer_Warn_(self, __VA_ARGS__) ? TPP_EOK : TPP_ELEXERROR)
#else /* TPP_CONFIG_ONELEXER == 2 */
#define tpp_lexer_warnf(self, ...) ((void)(self), TPPLexer_Warn(__VA_ARGS__) ? TPP_EOK : TPP_ELEXERROR)
#endif /* TPP_CONFIG_ONELEXER != 2 */




/************************************************************************/
/* "parts/preparse.h"                                                   */
/************************************************************************/
#define tpp_preparse_skipbse_fwd(lexer, pos, end)   ((void)(lexer), _tpp_preparse_skipbse_fwd(pos, end))
#define tpp_preparse_skipbse_bck(lexer, start, pos) ((void)(lexer), _tpp_preparse_skipbse_bck(start, pos))

/* TPP2 only (and always) supported BSE, so that's what we emulate here... */
TPP_INLINE /*TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2))*/ tpp_char const *TPPCALL
_tpp_preparse_skipbse_fwd(tpp_char const *pos, tpp_char const *end) {
	while (*pos == '\\' && pos + 1 < end) {
		if (pos[1] == '\n') {
			pos += 2;
		} else if (pos[1] == '\r') {
			pos += (pos + 2 < end && pos[2] == '\n') ? 3 : 2;
		} else {
			break;
		}
	}
	return pos;
}

TPP_INLINE /*TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1, 2))*/ tpp_char const *TPPCALL
_tpp_preparse_skipbse_bck(tpp_char const *start, tpp_char const *pos) {
	for (;;) {
		if (pos[-1] == '\n' && (pos - 1) > start) {
			if (pos[-2] == '\\') {
				pos -= 2;
			} else if (pos[-2] == '\r' && (pos - 2) > start && pos[-3] == '\\') {
				pos -= 3;
			} else {
				break;
			}
		} else if (pos[-1] == '\r' && (pos - 1) > start && pos[-2] == '\\') {
			pos -= 2;
		} else {
			break;
		}
	}
	return pos;
}

TPP_DECL_END

#endif /* !GUARD_TPP2_FORWARD_H */
