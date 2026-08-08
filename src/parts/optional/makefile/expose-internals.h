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
]]]*/
#define tmfcl_mf               TPP_MAKEFILE_INTERNAL(tmfcl_mf)
#define tmfcl_state            TPP_MAKEFILE_INTERNAL(tmfcl_state)
#define tmfcl_flags            TPP_MAKEFILE_INTERNAL(tmfcl_flags)
#define te_feat                TPP_MAKEFILE_INTERNAL(te_feat)
#define tef_flags              TPP_MAKEFILE_INTERNAL(tef_flags)
#define tetf_bitset            TPP_MAKEFILE_INTERNAL(tetf_bitset)
#define teff_USER_DEPENDENCIES TPP_MAKEFILE_INTERNAL(teff_USER_DEPENDENCIES)
#define tmf_lexer              TPP_MAKEFILE_INTERNAL(tmf_lexer)
#define tmf_output             TPP_MAKEFILE_INTERNAL(tmf_output)
#define tmf_feat               TPP_MAKEFILE_INTERNAL(tmf_feat)
/*[[[end]]]*/

/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_EXPOSE_INTERNALS_H */
