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
// TODO: Assert that none of the names conflict with the TPP core, or other source extensions
]]]*/
#define temcl_emitter                            TPP_EMITTER_INTERNAL(temcl_emitter)
#define temcl_state                              TPP_EMITTER_INTERNAL(temcl_state)
#define temcl_flags                              TPP_EMITTER_INTERNAL(temcl_flags)
#define tem_feat                                 TPP_EMITTER_INTERNAL(tem_feat)
#define temf_flags                               TPP_EMITTER_INTERNAL(temf_flags)
#define temf_bitset                              TPP_EMITTER_INTERNAL(temf_bitset)
#define temff_NORMALIZE_SPACE                    TPP_EMITTER_INTERNAL(temff_NORMALIZE_SPACE)
#define temff_NORMALIZE_LF                       TPP_EMITTER_INTERNAL(temff_NORMALIZE_LF)
#define temff_NORMALIZE_C_STRING                 TPP_EMITTER_INTERNAL(temff_NORMALIZE_C_STRING)
#define temff_NORMALIZE_C_INT                    TPP_EMITTER_INTERNAL(temff_NORMALIZE_C_INT)
#define temff_NORMALIZE_KEYWORDS                 TPP_EMITTER_INTERNAL(temff_NORMALIZE_KEYWORDS)
#define temff_NORMALIZE_BSE                      TPP_EMITTER_INTERNAL(temff_NORMALIZE_BSE)
#define temff_NORMALIZE_TRIGRAPHS                TPP_EMITTER_INTERNAL(temff_NORMALIZE_TRIGRAPHS)
#define temff_NORMALIZE_DIGRAPHS                 TPP_EMITTER_INTERNAL(temff_NORMALIZE_DIGRAPHS)
#define temff_NOLINE                             TPP_EMITTER_INTERNAL(temff_NOLINE)
#define temff_RELAXED_MACRO_COLUMN               TPP_EMITTER_INTERNAL(temff_RELAXED_MACRO_COLUMN)
#define temff_USE_CPP_DIGIT                      TPP_EMITTER_INTERNAL(temff_USE_CPP_DIGIT)
#define temff_USE_CPP_DIGIT_FLAGS                TPP_EMITTER_INTERNAL(temff_USE_CPP_DIGIT_FLAGS)
#define temff_USE_CPP_DIGIT_WORKING_DIRECTORY    TPP_EMITTER_INTERNAL(temff_USE_CPP_DIGIT_WORKING_DIRECTORY)
#define temff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY TPP_EMITTER_INTERNAL(temff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
#define temff_TRACE_INCLUDES                     TPP_EMITTER_INTERNAL(temff_TRACE_INCLUDES)
#define temsf_curpos                             TPP_EMITTER_INTERNAL(temsf_curpos)
#define temsf_fname                              TPP_EMITTER_INTERNAL(temsf_fname)
#define temsf_fname_str                          TPP_EMITTER_INTERNAL(temsf_fname_str)
#define temsf_flags                              TPP_EMITTER_INTERNAL(temsf_flags)
#define temsfs_file                              TPP_EMITTER_INTERNAL(temsfs_file)
#define temsfs_filec                             TPP_EMITTER_INTERNAL(temsfs_filec)
#define temsfs_filev                             TPP_EMITTER_INTERNAL(temsfs_filev)
#define tems_curfile                             TPP_EMITTER_INTERNAL(tems_curfile)
#define tems_cached_filec                        TPP_EMITTER_INTERNAL(tems_cached_filec)
#define tems_cached_filev                        TPP_EMITTER_INTERNAL(tems_cached_filev)
#define tems_flags                               TPP_EMITTER_INTERNAL(tems_flags)
#define tems_prevtok                             TPP_EMITTER_INTERNAL(tems_prevtok)
#define tem_lexer                                TPP_EMITTER_INTERNAL(tem_lexer)
#define tem_output                               TPP_EMITTER_INTERNAL(tem_output)
#define tem_state                                TPP_EMITTER_INTERNAL(tem_state)
#define tem_mode                                 TPP_EMITTER_INTERNAL(tem_mode)
#define tem_linethreshold                        TPP_EMITTER_INTERNAL(tem_linethreshold)
/*[[[end]]]*/

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EXPOSE_INTERNALS_H */
