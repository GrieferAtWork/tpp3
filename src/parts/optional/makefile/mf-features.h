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
#ifndef GUARD_TPP_OPTIONAL_MAKEFILE_MF_FEATURES_H
#define GUARD_TPP_OPTIONAL_MAKEFILE_MF_FEATURES_H 1

#include "api.h"

#include "config.h"

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

local configsLen = (configs.each.length > ...);
print("#undef TPP_MAKEFILE_HAVE_FEATURES");
print("#if (", "\\\n     ".join(configs.map(
	e -> f"TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_{e}) ||".ljust(configsLen + 38))).rstrip(" |"), ")");
print("#define TPP_MAKEFILE_HAVE_FEATURES 1");
print("#else /" "* ... *" "/");
print("#define TPP_MAKEFILE_HAVE_FEATURES 0");
print("#endif/" "* !... *" "/");
print();
print("#if TPP_MAKEFILE_HAVE_FEATURES");
print("typedef enum tpp_makefile_feature_id {");
for (local CONF: configs) {
	print("#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_", CONF, ")");
	print("	TPP_MAKEFILE_FEAT_", CONF, ",");
	print("#endif /" "* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_", CONF, ") *" "/");
}
print("	TPP_MAKEFILE_FEAT_COUNT");
print("} tpp_makefile_feature_id;");
print();
print("typedef union tpp_makefile_features {");
print("	struct {");
for (local CONF: configs) {
	print("#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_", CONF, ")");
	print("		unsigned int TPP_MAKEFILE_INTERNAL(tmkff_", CONF, "): 1;");
	print("#define _tpp_makefile_has_", CONF, "(self) (self)->TPP_MAKEFILE_INTERNAL(tmkf_feat).TPP_MAKEFILE_INTERNAL(tmkf_flags).TPP_MAKEFILE_INTERNAL(tmkff_", CONF, ")");
	print("#endif /" "* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_", CONF, ") *" "/");
}
print("	} TPP_MAKEFILE_INTERNAL(tmkf_flags);");
print("	unsigned char TPP_MAKEFILE_INTERNAL(tmkf_bitset)[TPP_MAKEFILE_FEAT_COUNT ? ((TPP_MAKEFILE_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];");
print("} tpp_makefile_features;");
print("");
print("#if !TPP_USE_STATIC");
print("TPP_CONST_DECL tpp_makefile_features const tpp_makefile_features_default;");
print("#endif /" "* !TPP_USE_STATIC *" "/");
print("");
print("#define tpp_makefile_features_getid(self, id) \\");
print("	((self)->TPP_MAKEFILE_INTERNAL(tmkf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_makefile_features_enable(self, id) \\");
print("	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_makefile_features_disable(self, id) \\");
print("	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_makefile_features_setid(self, id, enabled) \\");
print("	((enabled) ? tpp_makefile_features_enable(self, id) : tpp_makefile_features_disable(self, id))");
print("#define tpp_makefile_features_init(self)            (void)(*(self) = tpp_makefile_features_default)");
print("#define tpp_makefile_features_reset(self)           (void)(*(self) = tpp_makefile_features_default)");
print("#define tpp_makefile_features_fini(self)            tpp_dbg_memset(self, sizeof(tpp_makefile_features))");
print("#endif /" "* TPP_MAKEFILE_HAVE_FEATURES *" "/");
print;
for (local CONF: configs) {
	print("#if TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_", CONF, ")");
	print("#define _tpp_makefile_has_", CONF, "(self) TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_", CONF, ")");
	print("#endif /" "* TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_", CONF, ") *" "/");
}
]]]*/
#undef TPP_MAKEFILE_HAVE_FEATURES
#if (TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) ||\
     TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY))
#define TPP_MAKEFILE_HAVE_FEATURES 1
#else /* ... */
#define TPP_MAKEFILE_HAVE_FEATURES 0
#endif/* !... */

#if TPP_MAKEFILE_HAVE_FEATURES
typedef enum tpp_makefile_feature_id {
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
	TPP_MAKEFILE_FEAT_USER_DEPENDENCIES,
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY)
	TPP_MAKEFILE_FEAT_PHONY,
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY) */
	TPP_MAKEFILE_FEAT_COUNT
} tpp_makefile_feature_id;

typedef union tpp_makefile_features {
	struct {
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
		unsigned int TPP_MAKEFILE_INTERNAL(tmkff_USER_DEPENDENCIES): 1;
#define _tpp_makefile_has_USER_DEPENDENCIES(self) (self)->TPP_MAKEFILE_INTERNAL(tmkf_feat).TPP_MAKEFILE_INTERNAL(tmkf_flags).TPP_MAKEFILE_INTERNAL(tmkff_USER_DEPENDENCIES)
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
#if TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY)
		unsigned int TPP_MAKEFILE_INTERNAL(tmkff_PHONY): 1;
#define _tpp_makefile_has_PHONY(self) (self)->TPP_MAKEFILE_INTERNAL(tmkf_feat).TPP_MAKEFILE_INTERNAL(tmkf_flags).TPP_MAKEFILE_INTERNAL(tmkff_PHONY)
#endif /* TPP_CONF_ISFEAT(TPP_MAKEFILE_HAVE_PHONY) */
	} TPP_MAKEFILE_INTERNAL(tmkf_flags);
	unsigned char TPP_MAKEFILE_INTERNAL(tmkf_bitset)[TPP_MAKEFILE_FEAT_COUNT ? ((TPP_MAKEFILE_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_makefile_features;

#if !TPP_USE_STATIC
TPP_CONST_DECL tpp_makefile_features const tpp_makefile_features_default;
#endif /* !TPP_USE_STATIC */

#define tpp_makefile_features_getid(self, id) \
	((self)->TPP_MAKEFILE_INTERNAL(tmkf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_makefile_features_enable(self, id) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_makefile_features_disable(self, id) \
	(void)((self)->TPP_MAKEFILE_INTERNAL(tmkf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_makefile_features_setid(self, id, enabled) \
	((enabled) ? tpp_makefile_features_enable(self, id) : tpp_makefile_features_disable(self, id))
#define tpp_makefile_features_init(self)            (void)(*(self) = tpp_makefile_features_default)
#define tpp_makefile_features_reset(self)           (void)(*(self) = tpp_makefile_features_default)
#define tpp_makefile_features_fini(self)            tpp_dbg_memset(self, sizeof(tpp_makefile_features))
#endif /* TPP_MAKEFILE_HAVE_FEATURES */

#if TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
#define _tpp_makefile_has_USER_DEPENDENCIES(self) TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES)
#endif /* TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_USER_DEPENDENCIES) */
#if TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_PHONY)
#define _tpp_makefile_has_PHONY(self) TPP_CONF_DEFAULT(TPP_MAKEFILE_HAVE_PHONY)
#endif /* TPP_CONF_ISCONST(TPP_MAKEFILE_HAVE_PHONY) */
/*[[[end]]]*/

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_MAKEFILE_MF_FEATURES_H */
