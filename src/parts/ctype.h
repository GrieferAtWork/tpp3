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
#ifndef GUARD_TPP_CTYPE_H
#define GUARD_TPP_CTYPE_H 1

#include "api.h"
#include "config.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/* You can override the builtin ctype matrix if you already have your own.
 * however yours should match ASCII expectations, especially when it comes
 * to control characters. If this is not the case */
#undef TPP_HAVE_BUILTIN_CTYPE
#if (!defined(tpp_ascii_issymstrt) ||    \
     !defined(tpp_ascii_issymcont) ||    \
     !defined(tpp_ascii_isdigit) ||      \
     !defined(tpp_ascii_isspace) ||      \
     !defined(tpp_ascii_islf) ||         \
     !defined(tpp_ascii_islf_or_mblf) || \
     !defined(tpp_ascii_isspace_nolf))
#define TPP_HAVE_BUILTIN_CTYPE 1
#else /* ... */
#define TPP_HAVE_BUILTIN_CTYPE 0
#endif /* !... */

/* Assume that all "tpp_ascii_is*" macros are ascii-compatible
 * This allows for some additional optimizations when it comes
 * to routing character bytes to token decoders.
 *
 * iow: When this is enabled, "tpp_ascii_islf()" in a switch can
 *      be replaced with "case TPP_ASCII_LF: case TPP_ASCII_CR:"
 */
#ifndef TPP_HAVE_ASSUME_ASCII_CTYPE
#define TPP_HAVE_ASSUME_ASCII_CTYPE (TPP_HAVE_BUILTIN_CTYPE || 1)
#endif /* !TPP_HAVE_ASSUME_ASCII_CTYPE */


/* Ordinals for special character with special meaning */
#define TPP_ASCII_TAB 9  /* \t */
#define TPP_ASCII_LF  10 /* \n */
#define TPP_ASCII_CR  13 /* \r */


#if TPP_HAVE_BUILTIN_CTYPE
#define _TPP_CTYPE_ISSYMSTRT  0x01 /* Symbol start character */
#define _TPP_CTYPE_ISSYMCONT  0x02 /* Symbol continuation character */
#define _TPP_CTYPE_ISSPACE    0x04 /* Space character (excluding \r and \n) */
#define _TPP_CTYPE_ISLF       0x08 /* Linefeed character (\r and \n) */
#define _TPP_CTYPE_ISDIGIT    0x10 /* 0-9 */
#define _TPP_CTYPE_ISMBLF     0x20 /* First byte of utf8-encoded unicode linefeed character sequence (0xc2 + 0xe2 right now) */

#if UINT_LEAST8_MAX == 0xff
#define _tpp_ascii_mask(ch) ((uint_least8_t)(ch))
#else /* UINT_LEAST8_MAX == 0xff */
#define _tpp_ascii_mask(ch) ((ch) & 0xff)
#endif /* UINT_LEAST8_MAX != 0xff */
TPP_CONST_DECL uint_least8_t const _tpp_ctype[256]; /* Don't access directly! (considered TPP_INTERNAL) */
#define tpp_ascii_issymstrt(ch)    (_tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISSYMSTRT)
#define tpp_ascii_issymcont(ch)    (_tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISSYMCONT)
#define tpp_ascii_isdigit(ch)      (_tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISDIGIT)
#define tpp_ascii_isspace(ch)      (_tpp_ctype[_tpp_ascii_mask(ch)] & (_TPP_CTYPE_ISSPACE | _TPP_CTYPE_ISLF))
#define tpp_ascii_islf(ch)         (_tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISLF)
#define tpp_ascii_islf_or_mblf(ch) (_tpp_ctype[_tpp_ascii_mask(ch)] & (_TPP_CTYPE_ISLF | _TPP_CTYPE_ISMBLF))
#define tpp_ascii_isspace_nolf(ch) (_tpp_ctype[_tpp_ascii_mask(ch)] & _TPP_CTYPE_ISSPACE)
#endif /* TPP_HAVE_BUILTIN_CTYPE */

/* Check if "ch" is the first byte of a multi-byte UTF-8 sequence */
#ifndef tpp_ascii_ismb
#if 0 /* Setting "1" here reduces the chances of unicode encoding errors being detected */
#define tpp_ascii_ismb(ch) ((ch) >= 0xc0)
#else
#define tpp_ascii_ismb(ch) ((ch) >= 0x80)
#endif
#endif /* !tpp_ascii_ismb */

#ifndef tpp_ascii_isspace_nolf
#define tpp_ascii_isspace_nolf(ch) (tpp_ascii_isspace(ch) && !tpp_ascii_islf(ch))
#endif /* !tpp_ascii_isspace_nolf */
/*[[[deemon
import UTF8_LF_FIRST_BYTES from ".token-encodestring-mblf";
print("#ifndef tpp_ascii_islf_or_mblf");
print("#define tpp_ascii_islf_or_mblf(ch) (tpp_ascii_islf(ch)",
	"".join(for (local b: UTF8_LF_FIRST_BYTES) f" || (ch) == {b.hex()}"),
	")");
print("#endif /" "* !tpp_ascii_islf_or_mblf *" "/");
]]]*/
#ifndef tpp_ascii_islf_or_mblf
#define tpp_ascii_islf_or_mblf(ch) (tpp_ascii_islf(ch) || (ch) == 0xc2 || (ch) == 0xe2)
#endif /* !tpp_ascii_islf_or_mblf */
/*[[[end]]]*/


/* Macros used to implement integer conversion */
#ifndef tpp_ascii_asdigit
#define tpp_ascii_asdigit(ch) ((ch) - '0') /* tpp_ascii_isdigit-ch => value */
#define tpp_ascii_ofdigit(v)  ('0' + (v))  /* value => tpp_ascii_isdigit-ch */
#endif /* !tpp_ascii_ofdigit */
#ifndef tpp_ascii_isoctdigit
#define tpp_ascii_isoctdigit(ch) ((ch) >= '0' && (ch) <= '7')
#define tpp_ascii_asoctdigit(ch) tpp_ascii_asdigit(ch)
#define tpp_ascii_ofoctdigit(v)  tpp_ascii_ofdigit(v)
#endif /* !tpp_ascii_isoctdigit */
#ifndef tpp_ascii_islwrxdigit
#define tpp_ascii_islwrxdigit(ch) ((ch) >= 'a' && (ch) <= 'f')
#define tpp_ascii_aslwrxdigit(ch) (10 + ((ch) - 'a')) /* tpp_ascii_islwrxdigit-ch => value */
#define tpp_ascii_oflwrxdigit(v)  ('a' + ((v) - 10))  /* value => tpp_ascii_islwrxdigit-ch */
#endif /* !tpp_ascii_islwrxdigit */
#ifndef tpp_ascii_isuprxdigit
#define tpp_ascii_isuprxdigit(ch) ((ch) >= 'A' && (ch) <= 'F')
#define tpp_ascii_asuprxdigit(ch) (10 + ((ch) - 'A')) /* tpp_ascii_isuprxdigit-ch => value */
#define tpp_ascii_ofuprxdigit(v)  ('A' + ((v) - 10))  /* value => tpp_ascii_isuprxdigit-ch */
#endif /* !tpp_ascii_isuprxdigit */

#ifndef tpp_ascii_isxdigit
#define tpp_ascii_isxdigit(ch) \
	(tpp_ascii_isdigit(ch) || tpp_ascii_islwrxdigit(ch) || tpp_ascii_isuprxdigit(ch))
#define tpp_ascii_asxdigit(ch) \
	(tpp_ascii_isdigit(ch) ? tpp_ascii_asdigit(ch) : tpp_ascii_islwrxdigit(ch) ? tpp_ascii_aslwrxdigit(ch) : tpp_ascii_asuprxdigit(ch))
#endif /* !tpp_ascii_isxdigit */

#if TPP_HAVE_UNICODE

/* Built-in unicode character traits database */
#if TPP_HAVE_BUILTIN_CTYPE_UNICODE
#if (defined(tpp_unicode_isspace_nolf) || \
     defined(tpp_unicode_issymstrt) ||    \
     defined(tpp_unicode_issymcont) ||    \
     defined(tpp_unicode_isspace) ||      \
     defined(tpp_unicode_islf))
#define TPP_HAVE_BUILTIN_CTYPE_UNICODE 0 /* So your compiler shows you where the definition comes from */
#if !TPP_IGNORE_INVALID_CONFIGURATION
#error "User-supplied unicode trait functions defined, but 'TPP_HAVE_BUILTIN_CTYPE_UNICODE=1'. Please use 'TPP_HAVE_BUILTIN_CTYPE_UNICODE=0' when providing your own traits"
#endif /* !TPP_IGNORE_INVALID_CONFIGURATION */
#endif /* ... */

#ifndef _TPP_CTYPE_ISSYMSTRT
#define _TPP_CTYPE_ISSYMSTRT  0x01 /* Symbol start character */
#define _TPP_CTYPE_ISSYMCONT  0x02 /* Symbol continuation character */
#define _TPP_CTYPE_ISSPACE    0x04 /* Space character (excluding \r and \n) */
#define _TPP_CTYPE_ISLF       0x08 /* Linefeed character (\r and \n) */
#endif /* !_TPP_CTYPE_ISSYMSTRT */

TPP_DECL TPP_CONSTCALL TPP_WUNUSED uint_least8_t TPPCALL _tpp_unicode_traits(tpp_unichar uch); /* Don't access directly! (considered TPP_INTERNAL) */
#define tpp_unicode_isspace_nolf(ord) (_tpp_unicode_traits(ord) & _TPP_CTYPE_ISSPACE)
#define tpp_unicode_issymstrt(ord)    (_tpp_unicode_traits(ord) & _TPP_CTYPE_ISSYMSTRT)
#define tpp_unicode_issymcont(ord)    (_tpp_unicode_traits(ord) & _TPP_CTYPE_ISSYMCONT)
#define tpp_unicode_isspace(ord)      (_tpp_unicode_traits(ord) & (_TPP_CTYPE_ISSPACE | _TPP_CTYPE_ISLF))
#define tpp_unicode_islf(ord)         (_tpp_unicode_traits(ord) & _TPP_CTYPE_ISLF)
#endif /* TPP_HAVE_BUILTIN_CTYPE_UNICODE */



/* Unicode character traits (all of these take "tpp_unichar ord")
 *
 * If you want TPP to understand character traits across the full
 * unicode character range, you must define (at least) the following
 * macros prior to including any TPP header/source:
 * >> #define tpp_unicode_isspace(ord)    <"ord" is a unicode SPACE or LF character>
 * >> #define tpp_unicode_islf(ord)       <"ord" is a unicode LF character>
 * >> #define tpp_unicode_issymstrt(ord)  <"ord" may be the first character of a keyword>
 * >> #define tpp_unicode_issymcont(ord)  <"ord" may be the 2nd+ character of a keyword>
 *
 * These macros are then used by "tpp_lexer_yieldraw_at()" to determine
 * the meaning of unicode characters encountered as part of input text,
 * with any character not satisfying at least one of the above traits
 * being yielded as a "TPP_TOK_UNICHAR" token. */
#ifndef tpp_unicode_isspace_nolf
#if defined(tpp_unicode_isspace) && defined(tpp_unicode_islf)
#define tpp_unicode_isspace_nolf(ord) (tpp_unicode_isspace(ord) && !tpp_unicode_islf(ord))
#else /* tpp_unicode_isspace && tpp_unicode_islf */
#define tpp_unicode_isspace_nolf(ord) ((ord) <= 0xff && tpp_ascii_isspace_nolf(ord))
#endif /* !tpp_unicode_isspace || !tpp_unicode_islf */
#endif /* !tpp_unicode_isspace_nolf */
#ifndef tpp_unicode_issymstrt
#define tpp_unicode_issymstrt(ord) ((ord) >= 0x80 || tpp_ascii_issymstrt(ord))
#endif /* !tpp_unicode_issymstrt */
#ifndef tpp_unicode_issymcont
#define tpp_unicode_issymcont(ord) ((ord) >= 0x80 || tpp_ascii_issymcont(ord))
#endif /* !tpp_unicode_issymcont */
#ifndef tpp_unicode_isspace
#define tpp_unicode_isspace(ord) ((ord) <= 0xff && tpp_ascii_isspace(ord))
#endif /* !tpp_unicode_isspace */
#ifndef tpp_unicode_islf
#define tpp_unicode_islf(ord) ((ord) <= 0xff && tpp_ascii_islf(ord))
#endif /* !tpp_unicode_islf */

/* Read a single unicode character from a given utf-8 blob.
 * WARNING: This function doesn't do any validity checking,
 *          allowing over-long utf-8 sequences, as well as
 *          incorrectly positioned UTF-8 continuation bytes. */
TPP_DECL /*TPP_WUNUSED*/ TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8(tpp_char const **p_pos, tpp_char const *end);

/* Same as `tpp_unicode_readutf8()', but read in reverse */
TPP_DECL /*TPP_WUNUSED*/ TPP_NONNULL((1, 2)) tpp_unichar TPPCALL
tpp_unicode_readutf8_rev(tpp_char const **p_end, tpp_char const *base);
#endif /* TPP_HAVE_UNICODE */


#ifndef TPP_UTOA_MAXLEN
#if TPP_UINTMAX_MAX <= TPP_UINTMAX_C(255)
#define TPP_UTOA_MAXLEN 3  /* "255" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(65535)
#define TPP_UTOA_MAXLEN 5  /* "65535" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(4294967295)
#define TPP_UTOA_MAXLEN 10 /* "4294967295" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(18446744073709551615)
#define TPP_UTOA_MAXLEN 20 /* "18446744073709551615" */
#elif TPP_UINTMAX_MAX <= TPP_UINTMAX_C(340282366920938463463374607431768211455)
#define TPP_UTOA_MAXLEN 39 /* "340282366920938463463374607431768211455" */
#else /* TPP_UINTMAX_MAX <= ... */
#error "Unsupported 'TPP_UINTMAX_MAX'"
#endif /* TPP_UINTMAX_MAX > ... */
#endif /* !TPP_UTOA_MAXLEN */
#ifndef TPP_ITOA_MAXLEN
#define TPP_ITOA_MAXLEN (TPP_UTOA_MAXLEN + 1) /* +1 for leading "-" */
#endif /* !TPP_ITOA_MAXLEN */


/* Convert an integer into a string */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL tpp_utoa(char buf[TPP_UTOA_MAXLEN], tpp_uintmax value);
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) char *TPPCALL tpp_itoa(char buf[TPP_ITOA_MAXLEN], tpp_intmax value);


/* Convert a float into a string */
#if TPP_HAVE_FTOA
#define TPP_FTOA_MAXLEN ((TPP_ITOA_MAXLEN * 2) + 1)
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_size TPPCALL tpp_ftoa(char buf[TPP_FTOA_MAXLEN], tpp_float value);
#endif /* TPP_HAVE_FTOA */


#define TPP_UTF8_1BYTE_MAX ((TPP_UNICHAR_C(1) << 7) - 1)
#define TPP_UTF8_2BYTE_MAX ((TPP_UNICHAR_C(1) << 11) - 1)
#define TPP_UTF8_3BYTE_MAX ((TPP_UNICHAR_C(1) << 16) - 1)
#define TPP_UTF8_4BYTE_MAX ((TPP_UNICHAR_C(1) << 21) - 1)
#define TPP_UTF8_5BYTE_MAX ((TPP_UNICHAR_C(1) << 26) - 1)
#define TPP_UTF8_6BYTE_MAX ((TPP_UNICHAR_C(1) << 31) - 1)

#define TPP_UTF8_MAXLEN 7 /* Enough to write *any* 32-bit unicode ordinal as utf-8 (including invalid ones) */

#if TPP_HAVE_TPP_UNICODE_WRITEUTF8
/* Encode "uch" as utf-8 into "buf" and return the pointer after the last-written byte. */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_char *TPPCALL
tpp_unicode_writeutf8(tpp_char buf[TPP_UTF8_MAXLEN], tpp_unichar uc);
#endif /* TPP_HAVE_TPP_UNICODE_WRITEUTF8 */


#if TPP_HAVE_TPP_FUZZY_MEMCMP
/* Quantify the "fuzziness" of how close 2 memory-blocks are to each
 * other (less means closer to each other, and "0" means identical)
 *
 * #ifndef tpp_alloca
 * @return: TPP_SIZE_MAX: Cannot compare strings (insufficient memory,
 *                        and no tpp_alloca() function available to
 *                        supplement).
 *                        The implementation uses "tpp_trymalloc", so
 *                        this shouldn't be considered a fatal error
 * #endif // !tpp_alloca */
TPP_DECL TPP_WUNUSED tpp_size TPPCALL
tpp_fuzzy_memcmp(tpp_char const *lhs, tpp_size lhs_len,
                 tpp_char const *rhs, tpp_size rhs_len);
#endif /* TPP_HAVE_TPP_FUZZY_MEMCMP */



#undef TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM
#if (TPP_HAVE_UNICODE_BYNAME_LOOKUP &&                         \
     (TPP_CONF_IS_RT(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ICASE) ||  \
      TPP_CONF_IS_RT(TPP_HAVE_UNICODE_BYNAME_LOOKUP_ISPACE) || \
      TPP_CONF_IS_RT(TPP_HAVE_BSE) || TPP_HAVE_UNICODE))
#define TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM 1
#else /* ... */
#define TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM 0
#endif /* !... */

/* Specifies that `tpp_decode_named_escape()` requires an extra `lexer`-parameter */
#undef TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM
#if (TPP_HAVE_DECODE_NAMED_ESCAPE &&                         \
     ((TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES &&                \
       TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM) ||        \
      TPP_CONF_IS_RT(TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES) || \
      TPP_CONF_IS_RT(TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD) ||   \
      TPP_CONF_IS_RT(TPP_HAVE_ESCAPE_NAMED_XML) ||           \
      TPP_CONF_IS_RT(TPP_HAVE_TRIGRAPHS) ||                  \
      TPP_CONF_IS_RT(TPP_HAVE_BSE) ||                        \
      (TPP_HAVE_BSE && TPP_HAVE_UNICODE)))
#define TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM 1
#else /* ... */
#define TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM 0
#endif /* !... */

#if TPP_HAVE_XML_ENTITY_LOOKUP
#ifndef tpp_xml_entity_lookup
/* Return the unicode ordinal associated with `name`
 * @return: TPP_XML_ENTITY_LOOKUP_UNKNOWN: Unknown entity name
 * @return: * : Unicode ordinal for specified entity */
TPP_DECL TPP_PURECALL TPP_WUNUSED TPP_NONNULL((1)) tpp_unichar TPPCALL
tpp_xml_entity_lookup(char const *tpp_restrict name, bool has_trailing_semicolon);

/* Returned by `tpp_xml_entity_lookup()` when entity name is unrecognized */
#define TPP_XML_ENTITY_LOOKUP_UNKNOWN 0

#define TPP_XML_ENTITY_LOOKUP_MAXLEN 31 /* Length of the longest, known XML entity */
#define TPP_XML_ENTITY_LOOKUP_MINLEN 2  /* Length of the shortest, known XML entity */
#endif /* !tpp_xml_entity_lookup */
#endif /* TPP_HAVE_XML_ENTITY_LOOKUP */

#if TPP_HAVE_UNICODE_BYNAME_LOOKUP
#ifndef tpp_unicode_byname_lookup
/* Max # of unicode characters generated by `tpp_unicode_byname_lookup()` */
#define TPP_UNICODE_BYNAME_LOOKUP_MAXUC 10

/* Return the unicode ordinal associated with `*p_iter`
 * @return: 0 : Unknown (`*p_iter` was left unchanged)
 * @return: * : # of unicode ordinals written to "result" */
#if TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM
struct tpp_lexer;
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_size TPPCALL
tpp_unicode_byname_lookup(tpp_char const **p_iter, tpp_char const *end,
                          tpp_unichar uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC],
                          struct tpp_lexer const *tpp_restrict lexer);
#else /* TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
tpp_unicode_byname_lookup(tpp_char const **p_iter, tpp_char const *end,
                          tpp_unichar uc[TPP_UNICODE_BYNAME_LOOKUP_MAXUC]);
#endif /* !TPP_HAVE_UNICODE_BYNAME_LOOKUP_LEXER_PARAM */
#endif /* !tpp_unicode_byname_lookup */
#endif /* TPP_HAVE_UNICODE_BYNAME_LOOKUP */


#if TPP_HAVE_DECODE_NAMED_ESCAPE

/* Max # of unicode UTF-32 characters returned by `tpp_decode_named_escape()` */
#if TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES
#define TPP_DECODE_NAMED_ESCAPE_MAXLEN TPP_UNICODE_BYNAME_LOOKUP_MAXUC
#else /* TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES */
#define TPP_DECODE_NAMED_ESCAPE_MAXLEN 1
#endif /* !TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES */

/* Decode a named \N{...} sequence and update `*p_iter` to point to
 * its end. This function implement the routing between the different
 * (currently enabled) named escape decoders, as configured by:
 * - `TPP_HAVE_ESCAPE_NAMED_UNICODE_NAMES`
 * - `TPP_HAVE_ESCAPE_NAMED_UNICODE_ORD`
 * - `TPP_HAVE_ESCAPE_NAMED_XML`
 *
 * @return: 0 : Unknown named sequence (`*p_iter` is unchanged) 
 * @return: * : The # of characters written to `result` (always `<= TPP_DECODE_NAMED_ESCAPE_MAXLEN`)*/
#if TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM
struct tpp_lexer;
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3, 4)) tpp_size TPPCALL
tpp_decode_named_escape(tpp_char const **p_iter, tpp_char const *end,
                        tpp_unichar result[TPP_DECODE_NAMED_ESCAPE_MAXLEN],
                        struct tpp_lexer const *tpp_restrict lexer);
#else /* TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM */
TPP_DECL TPP_WUNUSED TPP_NONNULL((1, 2, 3)) tpp_size TPPCALL
_tpp_decode_named_escape(tpp_char const **p_iter, tpp_char const *end,
                         tpp_unichar result[TPP_DECODE_NAMED_ESCAPE_MAXLEN]);
#define tpp_decode_named_escape(p_iter, end, result, lexer) \
	_tpp_decode_named_escape(p_iter, end, result)
#endif /* !TPP_HAVE_DECODE_NAMED_ESCAPE_LEXER_PARAM */
#else /* TPP_HAVE_DECODE_NAMED_ESCAPE */
#define TPP_DECODE_NAMED_ESCAPE_MAXLEN 0
#define tpp_decode_named_escape(p_iter, end, result, lexer) \
	TPP_SSIZE_OFERR(TPP_ENOENT)
#endif /* !TPP_HAVE_DECODE_NAMED_ESCAPE */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_CTYPE_H */
