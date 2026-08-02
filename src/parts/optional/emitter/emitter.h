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
#ifndef GUARD_TPP_OPTIONAL_EMITTER_EMITTER_H
#define GUARD_TPP_OPTIONAL_EMITTER_EMITTER_H 1

#include "../../../tpp.h"
/**/

#include "config.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

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

/* TODO: Config to normalize (simplify) tokens before re-emitting them
 *       (each of these things listed here should also have its own,
 *       individual sub-config that only takes effect when the main
 *       config is enabled):
 * - BSE, \u, \U and \N sequences are removed from keywords
 * - BSE sequences are removed from all others tokens, too
 * - Strings are decoded, then re-encoded using `tpp_token_encodestring()`
 *   to force normalization (though `tldsc_bigprinter` is forwarded as-is) */

/* TODO: Config to select between use of `# <linenum>` and `#line` */

/* TODO: Config to enable emission of 1/2/3/4 flags in `# <linenum>`-directives */

/* TODO: Config to enable re-emission of unknown pragmas */

/* TODO: Configs for each of the GCC's CLI features listed in "frontend.c" */

typedef struct tpp_emitter_state {
	tpp_token_id        tes_prevtok;         /* Last token ID (preceding the token currently being emitted).
	                                         * When the current token is the first, this is `TPP_TOK_EOF` */
	tpp_lcinfo          tes_curpos;          /* Current line/column position in output (with respect to emitted `#line` directives) */
	char const         *tes_curfilename;     /* [0..1] The filename (tpp_file_getfilename()) that goes with `tes_curpos` (or "NULL" if unknown, or this is the first token) */
	TPP_REF tpp_string *tes_curfilename_str; /* [0..1] Same as `tes_curfilename`, but keeps a reference to `tpp_file_getfilenamestr()` so custom filename overrides aren't free'd early */
} tpp_emitter_state;

#define tpp_emitter_state_init(self)                  \
	(void)((self)->tes_prevtok = TPP_TOK_EOF,         \
	       tpp_lcinfo_init((self)->tes_curpos, 0, 0), \
	       (self)->tes_curfilename     = NULL,        \
	       (self)->tes_curfilename_str = NULL)
#define tpp_emitter_state_fini(self) \
	(void)((self)->tes_curfilename_str && (tpp_string_decref((self)->tes_curfilename_str), 1))

typedef struct tpp_emitter {
	tpp_lexer          *te_lexer;  /* [1..1][const] Lexer supplying the emitters input token stream */
	tpp_formatprinter   te_output; /* [1..1][const] Emitter output printer */
	void               *te_outarg; /* [?..?][const] Emitter output argument */
	tpp_emitter_state   te_state;  /* Emitter output state */
} tpp_emitter;

/* Initialize/finalize a given emitter */
#define tpp_emitter_init(self, lexer, output, outarg) \
	((self)->te_lexer  = (lexer),                     \
	 (self)->te_output = (output),                    \
	 (self)->te_outarg = (outarg),                    \
	 tpp_emitter_state_init(&(self)->te_state))
#define tpp_emitter_fini(self) \
	tpp_emitter_state_fini(&(self)->te_state)

/* Emit the token currently loaded into `self->te_lexer`,
 * and update the emitter's `te_state` accordingly
 *
 * @return: * :  Sum of return values of `self->te_output`
 * @return: < 0: First negative return value of `self->te_output` */
TPP_DECL /*TPP_WUNUSED*/ TPP_NONNULL((1)) tpp_ssize TPPCALL
tpp_emitter_emitcurrent(tpp_emitter *tpp_restrict self);

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_H */
