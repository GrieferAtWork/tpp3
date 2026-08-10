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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_EXPOSE_INTERNALS_H
#define GUARD_TPP_OPTIONAL_MAKEFILE_EXPOSE_INTERNALS_H 1

#include "api.h"
#include "config.h"

/*[[[tpp-begin]]]*/

/* If "tpp-emitter-amalgamation.h" was already included, re-define
 * `TPP_MAKEFILE_INTERNAL()` identifers to their unescaped names. */
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
	for (local match: data.reglocateall(r"TPP_MAKEFILE_INTERNAL\(\s*(\w+)\s*\)")) {
		local ident = match[1];
		if (ident !in identifiers && ident !in ["x"])
			identifiers.append(ident);
	}
}
local maxLen = identifiers.each.length > ...;
for (local ident: identifiers)
	print("#define ", ident, " " * (maxLen - #ident), " TPP_MAKEFILE_INTERNAL(", ident, ")");
// TODO: Assert that none of the names conflict with the TPP core, or other source extensions
]]]*/
#define tmkfcl_mf               TPP_MAKEFILE_INTERNAL(tmkfcl_mf)
#define tmkfcl_state            TPP_MAKEFILE_INTERNAL(tmkfcl_state)
#define tmkfcl_target           TPP_MAKEFILE_INTERNAL(tmkfcl_target)
#define tmkfcl_outfile          TPP_MAKEFILE_INTERNAL(tmkfcl_outfile)
#define tmkfcl_flags            TPP_MAKEFILE_INTERNAL(tmkfcl_flags)
#define tmkf_feat               TPP_MAKEFILE_INTERNAL(tmkf_feat)
#define tmkf_flags              TPP_MAKEFILE_INTERNAL(tmkf_flags)
#define tmkf_bitset             TPP_MAKEFILE_INTERNAL(tmkf_bitset)
#define tmkff_USER_DEPENDENCIES TPP_MAKEFILE_INTERNAL(tmkff_USER_DEPENDENCIES)
#define tmkff_PHONY             TPP_MAKEFILE_INTERNAL(tmkff_PHONY)
#define tmkf_lexer              TPP_MAKEFILE_INTERNAL(tmkf_lexer)
#define tmkf_output             TPP_MAKEFILE_INTERNAL(tmkf_output)
#define tmkf_output_file        TPP_MAKEFILE_INTERNAL(tmkf_output_file)
#define tmkf_depc               TPP_MAKEFILE_INTERNAL(tmkf_depc)
#define tmkf_depa               TPP_MAKEFILE_INTERNAL(tmkf_depa)
#define tmkf_depv               TPP_MAKEFILE_INTERNAL(tmkf_depv)
#define tmkf_curcol             TPP_MAKEFILE_INTERNAL(tmkf_curcol)
#define tmkf_maxcol             TPP_MAKEFILE_INTERNAL(tmkf_maxcol)
/*[[[end]]]*/

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_EXPOSE_INTERNALS_H */
