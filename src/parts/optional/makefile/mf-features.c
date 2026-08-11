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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_MF_FEATURES_C
#define GUARD_TPP_OPTIONAL_MAKEFILE_MF_FEATURES_C 1
#define TPP_BUILDING_OPTIONAL 1
#define TPP_MAKEFILE_BUILDING 1

#include "api.h"

#include "config.h"
#include "mf-features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/*[[[deemon
import * from deemon;
local configs: {string...} = [];
for (local line: File.open("config.h", "rb").read().decode("utf-8")
		.replace("\\\n", "").splitlines(false)) {
	local TPP_MAKEFILE_HAVE_FOO, defValue;
	try {
		TPP_MAKEFILE_HAVE_FOO, defValue = line.rescanf(r'#\s*define\s+(\w+)\s*([^/]+)')...;
	} catch (...) {
		continue;
	}
	if (defValue.recontains(r"\bTPP_CONF_FEAT[01]\b")) {
		local CONF = TPP_MAKEFILE_HAVE_FOO.lsstrip("TPP_MAKEFILE_HAVE_");
		configs.append(CONF);
	}
}

print("#if TPP_MAKEFILE_HAVE_FEATURES");
print("TPP_CONST_IMPL tpp_makefile_features const tpp_makefile_features_default = {");
print("	/" "* .tmkf_flags = *" "/ {");
local configsLen = (configs.each.length > ...);
for (local CONF: configs) {
	print("#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_", CONF, ")");
	print("		/" "* .tmkff_", CONF, " " * (configsLen - #CONF), " = *" "/ TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_", CONF, "),");
	print("#endif /" "* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_", CONF, ") *" "/");
}
print("	}");
print("};");
print("#endif /" "* TPP_MAKEFILE_HAVE_FEATURES *" "/");
]]]*/
#if TPP_MAKEFILE_HAVE_FEATURES
TPP_CONST_IMPL tpp_makefile_features const tpp_makefile_features_default = {
	/* .tmkf_flags = */ {
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
		/* .tmkff_USER_DEPENDENCIES = */ TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES),
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY)
		/* .tmkff_PHONY             = */ TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_PHONY),
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY) */
	}
};
#endif /* TPP_MAKEFILE_HAVE_FEATURES */
/*[[[end]]]*/

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_FEATURES_C */
