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
/* clang-format off */
#ifndef GUARD_TPP_EMITTER_AMALGAMATION_H
#define GUARD_TPP_EMITTER_AMALGAMATION_H 1

/************************************************************************/
/* File: parts/optional/emitter/api.h                                   */
/************************************************************************/
/* ========= Emitter =========
 *
 * Brief:
 * - Turns a sequence of tokens back into something that can be understood
 *   by an(other) preprocessor with only minimal capabilities.
 * - Retain (make sure it doesn't change) all information normally returned by:
 *   - tpp_file_getfilename(tpp_lexer_getlcfile(LEXER))
 *   - tpp_file_getstartlcinfo(tpp_lexer_getlcfile(LEXER))
 *   - tpp_file_getendlcinfo(tpp_lexer_getlcfile(LEXER))   (only if token-passthrough is enabled)
 * - One downside is that the emitter (must be) allowed to inject additional
 *   line-feed/whitespace tokens between any 2 regular tokens. This is required
 *   because the emitter must be able to emit `#line` directives whenever the
 *   effective line/column/filename changes between 2 adjacent tokens in such a
 *   way that those 2 tokens aren't adjacent.
 * - The emitter will also emit *magic* whitespace whenever 2 adjacent tokens
 *   require the presence of such to prevent accidental token concatenation:
 *   >> #define FOO() foo
 *   >> foo()bar
 *   emitted like this:
 *   >> foo
 *   >> #line 2
 *   >>    bar
 */

#ifndef TPP_AMALGAMATION_H
#define TPP_AMALGAMATION_H "tpp-amalgamation.h"
#endif /* !TPP_AMALGAMATION_H */
#include TPP_AMALGAMATION_H

#ifndef TPP_EMITTER_BUILDING
#define TPP_EMITTER_BUILDING 0
#endif /* !TPP_EMITTER_BUILDING */

/* Used to wrap an identifier that is considered to be internal to TPP
 * Identifiers wrapped as such should **NOT** be accessed directly.
 * Instead, the provided TPP APIs should be used. */
#ifndef TPP_EMITTER_INTERNAL
#if TPP_EMITTER_BUILDING
#define TPP_EMITTER_INTERNAL(x) x
#else /* TPP_EMITTER_BUILDING */
#define TPP_EMITTER_INTERNAL(x) _em_##x
#endif /* !TPP_EMITTER_BUILDING */
#endif /* !TPP_EMITTER_INTERNAL */

/************************************************************************/
/* File: parts/optional/emitter/config.h                                */
/************************************************************************/
/* Enable support for re-emission of unknown pragmas. Requires that the TPP core
 * is configured to allow runtime override of its `TPP_HAVE_UNKNOWN_PRAGMA_HOOK`
 * hook (since the emitter needs to be able to override that hook during its
 * initialization)
 *
 * Can be configured in one of 3 ways:
 * - `0`:  Disabled (unknown pragmas cause warnings and are not (re-)emitted
 * - `1`:  Enabled
 * - `-1`: Available (but not enabled by default)
 *
 * When not *Disabled*, can be turned on/off using:
 * - `tpp_emitter_set_reemit_unknown_pragma()`
 * - `tpp_emitter_enable_reemit_unknown_pragma()`
 * - `tpp_emitter_disable_reemit_unknown_pragma()`
 */
#ifndef TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
#define TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA \
	(TPP_HOOK_ISRT(TPP_HAVE_UNKNOWN_PRAGMA_HOOK) ? 1 : 0)
#endif /* !TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */

/* When enabled, any `TPP_TOK_SPACE`-token is emitted as an (appropriately long)
 * sequence of ` `-characters, rather than as an echo of the original token's
 * space characters (thereby normalizing any unicode whitespace or other control
 * characters to `U+0020 SPACE`). */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_SPACE
#define TPP_EMITTER_HAVE_NORMALIZE_SPACE TPP_CONF_FEAT1
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_SPACE */

/* When enabled, any `TPP_TOK_LF`-token is emitted as a `\n`-character, rather
 * than as an echo of the original token's linefeed bytes (thereby normalizing
 * any unicode linefeed, CR, or CRLF sequences to LF). */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_LF
#define TPP_EMITTER_HAVE_NORMALIZE_LF TPP_CONF_FEAT1
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_LF */

/* When enabled, any `TPP_TOK_ISSTRING`-token is emitted as a `TPP_TOK_C_STRING`
 * (or `TPP_TOK_C_CHAR`, when `TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS` is
 * enabled in the lexer) token (though only done if the desired target token
 * is enabled).
 *
 * In order to do this normalization, the string is decoded and re-encoded via
 * use of `tpp_lexer_decodestring()` and `tpp_token_encodestring()`, thereby
 * allowing a consumer of the preprocessor output to only have to support a
 * greatly reduced set of string tokens (and escape sequences) in order to
 * fully understand *any* kind of string token that may be produced by TPP. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_C_STRING
#define TPP_EMITTER_HAVE_NORMALIZE_C_STRING ((TPP_HAVE_TOK_C_STRING && TPP_HAVE_TOKEN_ENCODESTRING && TPP_HAVE_LEXER_DECODESTRING) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_C_STRING */

/* When enabled, normalize `\u`, `\U` and `\N` escape sequences in keywords
 * names to their actual utf-8 character representation (also causes BSE
 * sequences to be normalized, though if that's all you want (and not just
 * for keywords), you could also enable `TPP_EMITTER_HAVE_NORMALIZE_BSE`).
 *
 * This feature is also required to emit `__TPP_IDENTIFIER()` as the actual
 * identifier, rather than as a copy of the identifier itself.
 *
 * NOTE: Keywords containing unicode characters that aren't SYMCONT will
 *       still see those characters get escaped by `\U`, though that's the
 *       only escape sequence that will still be emitted (and thus needs
 *       to be understood by a consuming preprocessor).
 *
 * NOTE: The *empty* keyword (i.e. `__TPP_IDENTIFIER("")`) continues to
 *       be emitted as `__TPP_IDENTIFIER("")`, since there's no other way
 *       to write that identifier. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS
#define TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS ((TPP_HAVE_IDENTIFIER_ESCAPE_UNI || TPP_HAVE_IDENTIFIER_ESCAPE_NAMED) ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS */

/* Remove \-escaped line-feeds from generic tokens. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_BSE
#define TPP_EMITTER_HAVE_NORMALIZE_BSE (TPP_HAVE_BSE ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_BSE */

/* Normalize trigraph sequences in generic tokens. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS
#define TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS (TPP_HAVE_TRIGRAPHS ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS */

/* Normalize digraph sequences in generic tokens. */
#ifndef TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS
#define TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS (TPP_HAVE_DIGRAPHS ? TPP_CONF_FEAT1 : 0)
#endif /* !TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS */

/* Inhibit emission of `#line` directives, as well as (re-)alignment
 * of the output stream in order to match source L/C info. When this
 * is enabled, it is *highly* suggested that you turn on emission of
 * SPACE+LF tokens in the source lexer, since otherwise the emitter
 * will (probably) put everything on 1 line.
 *
 * Can be used to implement the `-P` CLI switch */
#ifndef TPP_EMITTER_HAVE_NOLINE
#define TPP_EMITTER_HAVE_NOLINE TPP_CONF_FEAT0
#endif /* !TPP_EMITTER_HAVE_NOLINE */

/* When inside of a macro -- so-as to prevent every token from causing
 * another `#line`-directive being emitted, don't be too precise
 * in terms of *all* tokens needing to have the proper column:
 * ```c
 * #define my_macro  10+20+30+40
 * 5+my_macro+50
 * ```
 *
 * Without this (technically more correct):
 * ```c
 * 5+10
 * #line 2
 *   +
 * #line 2
 *   20
 * #line 2
 *   +
 * #line 2
 *   30
 * #line 2
 *   +
 * #line 2
 *   40      +50
 * ```
 *
 * With this:
 * ```c
 * 5+10+20+30+40
 * #line 2
 *           +50
 * ``` */
#ifndef TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES
#define TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES TPP_CONF_FEAT1
#endif /* !TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES */

/* TODO: Config to select between use of `# <linenum>` and `#line` */

/* TODO: Config to enable emission of 1/2/3/4 flags in `# <linenum>`-directives */

/* TODO: Configs for each of the CLI switches listed in "frontend.c" */

/************************************************************************/
/* File: parts/optional/emitter/emitter-features.h                      */
/************************************************************************/
TPP_DECL_BEGIN

#undef TPP_EMITTER_HAVE_FEATURES
#if (TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) ||   \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF) ||      \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) ||\
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) ||\
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE) ||     \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) ||\
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) ||\
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE) ||            \
     TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES))
#define TPP_EMITTER_HAVE_FEATURES 1
#else /* ... */
#define TPP_EMITTER_HAVE_FEATURES 0
#endif/* !... */

#if TPP_EMITTER_HAVE_FEATURES
typedef enum tpp_emitter_feature_id {
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
	TPP_EMITTER_FEAT_NORMALIZE_SPACE,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF)
	TPP_EMITTER_FEAT_NORMALIZE_LF,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
	TPP_EMITTER_FEAT_NORMALIZE_C_STRING,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
	TPP_EMITTER_FEAT_NORMALIZE_KEYWORDS,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
	TPP_EMITTER_FEAT_NORMALIZE_BSE,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
	TPP_EMITTER_FEAT_NORMALIZE_TRIGRAPHS,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
	TPP_EMITTER_FEAT_NORMALIZE_DIGRAPHS,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
	TPP_EMITTER_FEAT_NOLINE,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES)
	TPP_EMITTER_FEAT_RELAXED_MACRO_LINE_RULES,
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES) */
	TPP_EMITTER_FEAT_COUNT
} tpp_emitter_feature_id;

typedef union tpp_emitter_features {
	struct {
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_SPACE): 1;
#define _tpp_emitter_has_NORMALIZE_SPACE(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_SPACE)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_LF): 1;
#define _tpp_emitter_has_NORMALIZE_LF(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_LF)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_STRING): 1;
#define _tpp_emitter_has_NORMALIZE_C_STRING(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_STRING)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_KEYWORDS): 1;
#define _tpp_emitter_has_NORMALIZE_KEYWORDS(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_KEYWORDS)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_BSE): 1;
#define _tpp_emitter_has_NORMALIZE_BSE(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_BSE)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_TRIGRAPHS): 1;
#define _tpp_emitter_has_NORMALIZE_TRIGRAPHS(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_TRIGRAPHS)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
		unsigned int TPP_EMITTER_INTERNAL(teff_NORMALIZE_DIGRAPHS): 1;
#define _tpp_emitter_has_NORMALIZE_DIGRAPHS(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NORMALIZE_DIGRAPHS)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
		unsigned int TPP_EMITTER_INTERNAL(teff_NOLINE): 1;
#define _tpp_emitter_has_NOLINE(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_NOLINE)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES)
		unsigned int TPP_EMITTER_INTERNAL(teff_RELAXED_MACRO_LINE_RULES): 1;
#define _tpp_emitter_has_RELAXED_MACRO_LINE_RULES(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_RELAXED_MACRO_LINE_RULES)
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES) */
	} TPP_EMITTER_INTERNAL(tef_flags);
	unsigned char TPP_EMITTER_INTERNAL(tetf_bitset)[TPP_EMITTER_FEAT_COUNT ? ((TPP_EMITTER_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_emitter_features;

TPP_CONST_DECL tpp_emitter_features const tpp_emitter_features_default;

#define tpp_emitter_features_getid(self, id) \
	((self)->TPP_EMITTER_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_emitter_features_enable(self, id) \
	(void)((self)->TPP_EMITTER_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_emitter_features_disable(self, id) \
	(void)((self)->TPP_EMITTER_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_emitter_features_setid(self, id, enabled) \
	((enabled) ? tpp_emitter_features_enable(self, id) : tpp_emitter_features_disable(self, id))
#define tpp_emitter_features_init(self)            (void)(*(self) = tpp_emitter_features_default)
#define tpp_emitter_features_reset(self)           (void)(*(self) = tpp_emitter_features_default)
#define tpp_emitter_features_fini(self)            tpp_dbg_memset(self, sizeof(tpp_emitter_features))
#endif /* TPP_EMITTER_HAVE_FEATURES */

#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
#define _tpp_emitter_has_NORMALIZE_SPACE(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_SPACE) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_LF)
#define _tpp_emitter_has_NORMALIZE_LF(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_LF)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_LF) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
#define _tpp_emitter_has_NORMALIZE_C_STRING(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
#define _tpp_emitter_has_NORMALIZE_KEYWORDS(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_BSE)
#define _tpp_emitter_has_NORMALIZE_BSE(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_BSE) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
#define _tpp_emitter_has_NORMALIZE_TRIGRAPHS(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
#define _tpp_emitter_has_NORMALIZE_DIGRAPHS(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NOLINE)
#define _tpp_emitter_has_NOLINE(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NOLINE)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES)
#define _tpp_emitter_has_RELAXED_MACRO_LINE_RULES(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES)
#endif /* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_RELAXED_MACRO_LINE_RULES) */

/************************************************************************/
/* File: parts/optional/emitter/emitter.h                               */
/************************************************************************/

typedef struct tpp_emitter_state {
#if TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE)
	tpp_lcinfo          TPP_EMITTER_INTERNAL(tes_curpos);          /* Current line/column position in output (with respect to emitted `#line` directives) */
	char const         *TPP_EMITTER_INTERNAL(tes_curfilename);     /* [0..1] The filename (tpp_file_getfilename()) that goes with `tes_curpos` (or "NULL" if unknown, or this is the first token) */
	TPP_REF tpp_string *TPP_EMITTER_INTERNAL(tes_curfilename_str); /* [0..1] Same as `tes_curfilename`, but keeps a reference to `tpp_file_getfilenamestr()` so custom filename overrides aren't free'd early */
#define _tpp_emitter_state_init_cur(self)                             \
	, tpp_lcinfo_init((self)->TPP_EMITTER_INTERNAL(tes_curpos), 0, 0) \
	, (self)->TPP_EMITTER_INTERNAL(tes_curfilename)     = NULL        \
	, (self)->TPP_EMITTER_INTERNAL(tes_curfilename_str) = NULL
#define _tpp_emitter_state_fini_cur(self)                                          \
	, (self)->TPP_EMITTER_INTERNAL(tes_curfilename_str)                            \
	  ? (void)tpp_string_decref((self)->TPP_EMITTER_INTERNAL(tes_curfilename_str)) \
	  : (void)0
#else /* TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
#define _tpp_emitter_state_init_cur(self) /* nothing */
#define _tpp_emitter_state_fini_cur(self) /* nothing */
#endif /* !TPP_CONF_MAYBE_0(TPP_EMITTER_HAVE_NOLINE) */
	tpp_token_id        TPP_EMITTER_INTERNAL(tes_prevtok);         /* Last token ID (preceding the token currently being emitted).
	                                                                * When the current token is the first, this is `TPP_TOK_EOF` */
} tpp_emitter_state;

#define tpp_emitter_state_init(self)                               \
	(void)((self)->TPP_EMITTER_INTERNAL(tes_prevtok) = TPP_TOK_EOF \
	       _tpp_emitter_state_init_cur(self))
#define tpp_emitter_state_fini(self) \
	(void)((void)0 _tpp_emitter_state_fini_cur(self))

typedef struct tpp_emitter {
	/* NOTE: The lexer will *always* be at offset=0 here, meaning you're allowed to do:
	 * >> typedef struct my_compiler {
	 * >>     ...;
	 * >>     union {
	 * >>         tpp_lexer   l_lexer;     // Common lexer object
	 * >>         tpp_emitter l_emitter;   // For implementing a -E switch
	 * >>         struct {
	 * >>             tpp_lexer lmy_lexer; // Lexer for my compiler
	 * >>             ...                  // Stuff not needed when -E is active
	 * >>         } l_my_lexer;
	 * >>     } mc_lexer;
	 * >> } my_compiler; */
	tpp_lexer            TPP_EMITTER_INTERNAL(te_lexer);  /* Lexer supplying the emitter's input token stream */
	tpp_formatprinter    TPP_EMITTER_INTERNAL(te_output); /* [1..1][const] Emitter output printer (the emitter itself will be passed as argument) */
	tpp_emitter_state    TPP_EMITTER_INTERNAL(te_state);  /* Emitter output state */
#if TPP_EMITTER_HAVE_FEATURES
	tpp_emitter_features TPP_EMITTER_INTERNAL(te_feat);   /* Emitter feature configuration */
#define _tpp_emitter_init_feat(self) , tpp_emitter_features_init(&(self)->TPP_EMITTER_INTERNAL(te_feat))
#define _tpp_emitter_fini_feat(self) , tpp_emitter_features_fini(&(self)->TPP_EMITTER_INTERNAL(te_feat))
#else /* TPP_EMITTER_HAVE_FEATURES */
#define _tpp_emitter_init_feat(self) /* nothing */
#define _tpp_emitter_fini_feat(self) /* nothing */
#endif /* !TPP_EMITTER_HAVE_FEATURES */
} tpp_emitter;

/* Initialize (after `tpp_lexer_init()` was called) or finalize
 * (before `tpp_lexer_fini()` is called) a given emitter.
 *
 * @param: output: Output printer. On error, must return one of `TPP_SSIZE_OFERR(*)`*/
TPP_DECL TPP_NONNULL((1, 2)) void TPPCALL
tpp_emitter_init_after_lexer(tpp_emitter *tpp_restrict self,
                             tpp_formatprinter output);
TPP_DECL TPP_NONNULL((1)) void TPPCALL
tpp_emitter_fini_before_lexer(tpp_emitter *tpp_restrict self);

/* Initialize/finalize a given emitter, as well as the underlying lexer.
 *
 * The caller is still responsible to initialize/finalize the lexer's file:
 * - `tpp_lexer_initfile_*(tpp_emitter_getlexer(self))`
 * - `tpp_lexer_finifile(tpp_emitter_getlexer(self))`
 *
 * @param: output: Output printer. On error, must return one of `TPP_SSIZE_OFERR(*)` */
#define tpp_emitter_init(self, output)                        \
	(tpp_lexer_init(&(self)->TPP_EMITTER_INTERNAL(te_lexer)), \
	 tpp_emitter_init_after_lexer(self, output))
#define tpp_emitter_fini(self)            \
	(tpp_emitter_fini_before_lexer(self), \
	 tpp_lexer_fini(&(self)->TPP_EMITTER_INTERNAL(te_lexer)))

/* Retrieve components of the emitter. */
#define tpp_emitter_getlexer(self)  (&(self)->TPP_EMITTER_INTERNAL(te_lexer))
#define tpp_emitter_getoutput(self) (self)->TPP_EMITTER_INTERNAL(te_output)

/* Reverse-engineer the emitter of the lexer returned by `tpp_emitter_getlexer()` */
#define tpp_emitter_oflexer(lexer) ((tpp_emitter *)(lexer))

/* Helpers for quickly printing stuff to the emitter's output.
 * WARNING: Careless use of these functions may result in the emitter's
 *          idea of its current output file/line/column becoming invalid. */
#define tpp_emitter_output_printraw(self, text, len) \
	tpp_formatprinter_print(tpp_emitter_getoutput(self), self, text, len)
#define tpp_emitter_output_printraw_cstr(self, text, len) \
	tpp_formatprinter_print_cstr(tpp_emitter_getoutput(self), self, text, len)
#define tpp_emitter_output_printraw_conststr(self, CONSTstr) \
	tpp_formatprinter_print_conststr(tpp_emitter_getoutput(self), self, CONSTstr)

/* Check if a runtime-configurable config option `conf` in `TPP_EMITTER_HAVE_<conf>` is currently enabled.
 * When `TPP_EMITTER_HAVE_<conf>` is configured as `TPP_CONF_ISCONST()`, return that constant instead. */
#define tpp_emitter_has(self, conf) _tpp_emitter_has_##conf(self)

/* Features... */
#if TPP_EMITTER_HAVE_FEATURES
#define tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_x)          tpp_emitter_features_getid(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_setfeature(self, TPP_EMITTER_FEAT_x, enabled) tpp_emitter_features_setid(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x, enabled)
#define tpp_emitter_enablefeature(self, TPP_EMITTER_FEAT_x)       tpp_emitter_features_enable(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_disablefeature(self, TPP_EMITTER_FEAT_x)      tpp_emitter_features_disable(&(self)->TPP_EMITTER_INTERNAL(te_feat), TPP_EMITTER_FEAT_x)
#define tpp_emitter_resetfeatures(self)                           tpp_emitter_features_reset(&(self)->TPP_EMITTER_INTERNAL(te_feat))
#else /* TPP_EMITTER_HAVE_FEATURES */
#define tpp_emitter_getfeature(self, TPP_EMITTER_FEAT_x) 0
#define tpp_emitter_resetfeatures(self)                  (void)0
#endif /* !TPP_EMITTER_HAVE_FEATURES */


/* Emit the token currently loaded into `tpp_emitter_getlexer(self)`,
 * and update the emitter's `te_state` accordingly
 *
 * @return: * :  Sum of return values of `tpp_emitter_getoutput(self)`
 * @return: < 0: First negative return value of `tpp_emitter_getoutput(self)` */
TPP_DECL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent(tpp_emitter *tpp_restrict self);


/* API support for (re-)emission of unknown `#pragma` directives */
#if TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA
#define tpp_emitter_enable_reemit_unknown_pragma(self) \
	tpp_lexer_sethook_unknown_pragma(tpp_emitter_getlexer(self), &_tpp_emitter_hook_unknown_pragma)
#define tpp_emitter_disable_reemit_unknown_pragma(self) \
	tpp_lexer_resethook_unknown_pragma(tpp_emitter_getlexer(self))
#define tpp_emitter_get_reemit_unknown_pragma(self) \
	(tpp_lexer_gethook_unknown_pragma(tpp_emitter_getlexer(self)) == &_tpp_emitter_hook_unknown_pragma)
#define tpp_emitter_set_reemit_unknown_pragma(self, v)    \
	((v) ? tpp_emitter_enable_reemit_unknown_pragma(self) \
	     : tpp_emitter_disable_reemit_unknown_pragma(self))

TPP_DECL TPP_WUNUSED TPP_NONNULL((1)) tpp_errno TPPCALL
_tpp_emitter_hook_unknown_pragma(tpp_lexer *tpp_restrict self);
#endif /* TPP_EMITTER_HAVE_REEMIT_UNKNOWN_PRAGMA */

TPP_DECL_END

#endif /* !GUARD_TPP_EMITTER_AMALGAMATION_H */
/* clang-format on */
