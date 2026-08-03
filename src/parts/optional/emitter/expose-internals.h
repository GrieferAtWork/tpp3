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
/*!depends *.h, *.c*/
#ifndef GUARD_TPP_OPTIONAL_EMITTER_EXPOSE_INTERNALS_H
#define GUARD_TPP_OPTIONAL_EMITTER_EXPOSE_INTERNALS_H 1

#include "api.h"
#include "config.h"

/*[[[tpp-begin]]]*/

/* If "tpp-emitter-amalgamation.h" was already included, re-define
 * `TPP_EMITTER_INTERNAL()` identifers to their unescaped names. */
/*[[[deemon
import * from deemon;
import fs;
local identifiers = [];
for (local name: fs.dir(".").sorted()) {
	if (name == "expose-internals.h")
		continue;
	if (!name.endswith(".h") && !name.endswith(".c"))
		continue;
	local data = File.open(name, "rb").read().decode("utf-8");
	for (local match: data.reglocateall(r"TPP_EMITTER_INTERNAL\(\s*(\w+)\s*\)")) {
		local ident = match[1];
		if (ident !in identifiers && ident !in ["x"])
			identifiers.append(ident);
	}
}
local maxLen = identifiers.each.length > ...;
for (local ident: identifiers)
	print("#define ", ident, " " * (maxLen - #ident), " TPP_EMITTER_INTERNAL(", ident, ")");
]]]*/
#define tcl_emitter                             TPP_EMITTER_INTERNAL(tcl_emitter)
#define tcl_state                               TPP_EMITTER_INTERNAL(tcl_state)
#define tcl_flags                               TPP_EMITTER_INTERNAL(tcl_flags)
#define te_feat                                 TPP_EMITTER_INTERNAL(te_feat)
#define tef_flags                               TPP_EMITTER_INTERNAL(tef_flags)
#define tetf_bitset                             TPP_EMITTER_INTERNAL(tetf_bitset)
#define teff_NORMALIZE_SPACE                    TPP_EMITTER_INTERNAL(teff_NORMALIZE_SPACE)
#define teff_NORMALIZE_LF                       TPP_EMITTER_INTERNAL(teff_NORMALIZE_LF)
#define teff_NORMALIZE_C_STRING                 TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_STRING)
#define teff_NORMALIZE_KEYWORDS                 TPP_EMITTER_INTERNAL(teff_NORMALIZE_KEYWORDS)
#define teff_NORMALIZE_BSE                      TPP_EMITTER_INTERNAL(teff_NORMALIZE_BSE)
#define teff_NORMALIZE_TRIGRAPHS                TPP_EMITTER_INTERNAL(teff_NORMALIZE_TRIGRAPHS)
#define teff_NORMALIZE_DIGRAPHS                 TPP_EMITTER_INTERNAL(teff_NORMALIZE_DIGRAPHS)
#define teff_NOLINE                             TPP_EMITTER_INTERNAL(teff_NOLINE)
#define teff_RELAXED_MACRO_LINE_RULES           TPP_EMITTER_INTERNAL(teff_RELAXED_MACRO_LINE_RULES)
#define teff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY TPP_EMITTER_INTERNAL(teff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#define tes_curpos                              TPP_EMITTER_INTERNAL(tes_curpos)
#define tes_curfilename                         TPP_EMITTER_INTERNAL(tes_curfilename)
#define tes_curfilename_str                     TPP_EMITTER_INTERNAL(tes_curfilename_str)
#define tes_prevtok                             TPP_EMITTER_INTERNAL(tes_prevtok)
#define te_lexer                                TPP_EMITTER_INTERNAL(te_lexer)
#define te_output                               TPP_EMITTER_INTERNAL(te_output)
#define te_state                                TPP_EMITTER_INTERNAL(te_state)
#define te_mode                                 TPP_EMITTER_INTERNAL(te_mode)
/*[[[end]]]*/

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EXPOSE_INTERNALS_H */
