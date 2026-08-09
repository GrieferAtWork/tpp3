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
#define tecl_emitter                            TPP_EMITTER_INTERNAL(tecl_emitter)
#define tecl_state                              TPP_EMITTER_INTERNAL(tecl_state)
#define tecl_flags                              TPP_EMITTER_INTERNAL(tecl_flags)
#define te_feat                                 TPP_EMITTER_INTERNAL(te_feat)
#define tef_flags                               TPP_EMITTER_INTERNAL(tef_flags)
#define tef_bitset                              TPP_EMITTER_INTERNAL(tef_bitset)
#define teff_NORMALIZE_SPACE                    TPP_EMITTER_INTERNAL(teff_NORMALIZE_SPACE)
#define teff_NORMALIZE_LF                       TPP_EMITTER_INTERNAL(teff_NORMALIZE_LF)
#define teff_NORMALIZE_C_STRING                 TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_STRING)
#define teff_NORMALIZE_C_INT                    TPP_EMITTER_INTERNAL(teff_NORMALIZE_C_INT)
#define teff_NORMALIZE_KEYWORDS                 TPP_EMITTER_INTERNAL(teff_NORMALIZE_KEYWORDS)
#define teff_NORMALIZE_BSE                      TPP_EMITTER_INTERNAL(teff_NORMALIZE_BSE)
#define teff_NORMALIZE_TRIGRAPHS                TPP_EMITTER_INTERNAL(teff_NORMALIZE_TRIGRAPHS)
#define teff_NORMALIZE_DIGRAPHS                 TPP_EMITTER_INTERNAL(teff_NORMALIZE_DIGRAPHS)
#define teff_NOLINE                             TPP_EMITTER_INTERNAL(teff_NOLINE)
#define teff_RELAXED_MACRO_COLUMN               TPP_EMITTER_INTERNAL(teff_RELAXED_MACRO_COLUMN)
#define teff_USE_CPP_DIGIT                      TPP_EMITTER_INTERNAL(teff_USE_CPP_DIGIT)
#define teff_USE_CPP_DIGIT_FLAGS                TPP_EMITTER_INTERNAL(teff_USE_CPP_DIGIT_FLAGS)
#define teff_USE_CPP_DIGIT_WORKING_DIRECTORY    TPP_EMITTER_INTERNAL(teff_USE_CPP_DIGIT_WORKING_DIRECTORY)
#define teff_REEMIT_MACRO_DEFINITIONS_LAZY      TPP_EMITTER_INTERNAL(teff_REEMIT_MACRO_DEFINITIONS_LAZY)
#define teff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY TPP_EMITTER_INTERNAL(teff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#define teff_TRACE_INCLUDES                     TPP_EMITTER_INTERNAL(teff_TRACE_INCLUDES)
#define tesf_curpos                             TPP_EMITTER_INTERNAL(tesf_curpos)
#define tesf_fname                              TPP_EMITTER_INTERNAL(tesf_fname)
#define tesf_fname_str                          TPP_EMITTER_INTERNAL(tesf_fname_str)
#define tesf_flags                              TPP_EMITTER_INTERNAL(tesf_flags)
#define tesfs_file                              TPP_EMITTER_INTERNAL(tesfs_file)
#define tesfs_filec                             TPP_EMITTER_INTERNAL(tesfs_filec)
#define tesfs_filev                             TPP_EMITTER_INTERNAL(tesfs_filev)
#define tes_curfile                             TPP_EMITTER_INTERNAL(tes_curfile)
#define tes_cached_filec                        TPP_EMITTER_INTERNAL(tes_cached_filec)
#define tes_cached_filev                        TPP_EMITTER_INTERNAL(tes_cached_filev)
#define tes_flags                               TPP_EMITTER_INTERNAL(tes_flags)
#define tes_prevtok                             TPP_EMITTER_INTERNAL(tes_prevtok)
#define te_lexer                                TPP_EMITTER_INTERNAL(te_lexer)
#define te_output                               TPP_EMITTER_INTERNAL(te_output)
#define te_state                                TPP_EMITTER_INTERNAL(te_state)
#define te_mode                                 TPP_EMITTER_INTERNAL(te_mode)
#define te_linethreshold                        TPP_EMITTER_INTERNAL(te_linethreshold)
/*[[[end]]]*/

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EXPOSE_INTERNALS_H */
