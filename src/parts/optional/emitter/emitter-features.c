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
/*!depends config.h*/
#ifndef GUARD_TPP_OPTIONAL_EMITTER_EMITTER_FEATURES_C
#define GUARD_TPP_OPTIONAL_EMITTER_EMITTER_FEATURES_C 1
#define TPP_BUILDING_OPTIONAL 1
#define TPP_EMITTER_BUILDING 1

#include "api.h"

#include "config.h"
#include "emitter-features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/*[[[deemon
import * from deemon;
local configs: {string...} = [];
for (local line: File.open("config.h", "rb").read().decode("utf-8").splitlines(false)) {
	local TPP_EMITTER_HAVE_FOO, defValue;
	try {
		TPP_EMITTER_HAVE_FOO, defValue = line.rescanf(r'#\s*define\s+(\w+)\s*([^/]+)')...;
	} catch (...) {
		continue;
	}
	if (defValue.recontains(r"\bTPP_CONF_FEAT[01]\b")) {
		local CONF = TPP_EMITTER_HAVE_FOO.lsstrip("TPP_EMITTER_HAVE_");
		configs.append(CONF);
	}
}

print("#if TPP_EMITTER_HAVE_FEATURES");
print("TPP_CONST_IMPL tpp_emitter_features const tpp_emitter_features_default = {");
print("	/" "* .temf_flags = *" "/ {");
local configsLen = (configs.each.length > ...);
for (local CONF: configs) {
	print("#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_", CONF, ")");
	print("		/" "* .temff_", CONF, " " * (configsLen - #CONF), " = *" "/ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_", CONF, "),");
	print("#endif /" "* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_", CONF, ") *" "/");
}
print("	}");
print("};");
print("#endif /" "* TPP_EMITTER_HAVE_FEATURES *" "/");
]]]*/
#if TPP_EMITTER_HAVE_FEATURES
TPP_CONST_IMPL tpp_emitter_features const tpp_emitter_features_default = {
	/* .temf_flags = */ {
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE)
		/* .temff_NORMALIZE_SPACE                    = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_SPACE),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_SPACE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF)
		/* .temff_NORMALIZE_LF                       = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_LF),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_LF) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING)
		/* .temff_NORMALIZE_C_STRING                 = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_STRING) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT)
		/* .temff_NORMALIZE_C_INT                    = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_C_INT),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_C_INT) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS)
		/* .temff_NORMALIZE_KEYWORDS                 = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_KEYWORDS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE)
		/* .temff_NORMALIZE_BSE                      = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_BSE),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_BSE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS)
		/* .temff_NORMALIZE_TRIGRAPHS                = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_TRIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS)
		/* .temff_NORMALIZE_DIGRAPHS                 = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NORMALIZE_DIGRAPHS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE)
		/* .temff_NOLINE                             = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_NOLINE),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_NOLINE) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN)
		/* .temff_RELAXED_MACRO_COLUMN               = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_RELAXED_MACRO_COLUMN) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT)
		/* .temff_USE_CPP_DIGIT                      = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS)
		/* .temff_USE_CPP_DIGIT_FLAGS                = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_FLAGS) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY)
		/* .temff_USE_CPP_DIGIT_WORKING_DIRECTORY    = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_USE_CPP_DIGIT_WORKING_DIRECTORY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY)
		/* .temff_REEMIT_MACRO_DEFINITIONS_NAME_ONLY = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_REEMIT_MACRO_DEFINITIONS_NAME_ONLY) */
#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES)
		/* .temff_TRACE_INCLUDES                     = */ TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_TRACE_INCLUDES),
#endif /* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_TRACE_INCLUDES) */
	}
};
#endif /* TPP_EMITTER_HAVE_FEATURES */
/*[[[end]]]*/

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_FEATURES_C */
