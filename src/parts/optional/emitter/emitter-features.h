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
#ifndef GUARD_TPP_OPTIONAL_EMITTER_EMITTER_FEATURES_H
#define GUARD_TPP_OPTIONAL_EMITTER_EMITTER_FEATURES_H 1

#include "api.h"

#include "config.h"

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

local configsLen = (configs.each.length > ...);
print("#undef TPP_EMITTER_HAVE_FEATURES");
print("#if (", "\\\n     ".join(configs.map(
	e -> f"TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_{e}) ||".ljust(configsLen + 31))).rstrip(" |"), ")");
print("#define TPP_EMITTER_HAVE_FEATURES 1");
print("#else /" "* ... *" "/");
print("#define TPP_EMITTER_HAVE_FEATURES 0");
print("#endif/" "* !... *" "/");
print();
print("#if TPP_EMITTER_HAVE_FEATURES");
print("typedef enum tpp_emitter_feature_id {");
for (local CONF: configs) {
	print("#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_", CONF, ")");
	print("	TPP_EMITTER_FEAT_", CONF, ",");
	print("#endif /" "* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_", CONF, ") *" "/");
}
print("	TPP_EMITTER_FEAT_COUNT");
print("} tpp_emitter_feature_id;");
print();
print("typedef union tpp_emitter_features {");
print("	struct {");
for (local CONF: configs) {
	print("#if TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_", CONF, ")");
	print("		unsigned int TPP_EMITTER_INTERNAL(teff_", CONF, "): 1;");
	print("#define _tpp_emitter_has_", CONF, "(self) (self)->TPP_EMITTER_INTERNAL(te_feat).TPP_EMITTER_INTERNAL(tef_flags).TPP_EMITTER_INTERNAL(teff_", CONF, ")");
	print("#endif /" "* TPP_CONF_ISFEAT(TPP_EMITTER_HAVE_", CONF, ") *" "/");
}
print("	} TPP_EMITTER_INTERNAL(tef_flags);");
print("	unsigned char TPP_EMITTER_INTERNAL(tetf_bitset)[TPP_EMITTER_FEAT_COUNT ? ((TPP_EMITTER_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];");
print("} tpp_emitter_features;");
print("");
print("TPP_CONST_DECL tpp_emitter_features const tpp_emitter_features_default;");
print("");
print("#define tpp_emitter_features_getid(self, id) \\");
print("	((self)->TPP_EMITTER_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_emitter_features_enable(self, id) \\");
print("	(void)((self)->TPP_EMITTER_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_emitter_features_disable(self, id) \\");
print("	(void)((self)->TPP_EMITTER_INTERNAL(tetf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_emitter_features_setid(self, id, enabled) \\");
print("	((enabled) ? tpp_emitter_features_enable(self, id) : tpp_emitter_features_disable(self, id))");
print("#define tpp_emitter_features_init(self)            (void)(*(self) = tpp_emitter_features_default)");
print("#define tpp_emitter_features_reset(self)           (void)(*(self) = tpp_emitter_features_default)");
print("#define tpp_emitter_features_fini(self)            tpp_dbg_memset(self, sizeof(tpp_emitter_features))");
print("#endif /" "* TPP_EMITTER_HAVE_FEATURES *" "/");
print;
for (local CONF: configs) {
	print("#if TPP_CONF_ISCONST(TPP_EMITTER_HAVE_", CONF, ")");
	print("#define _tpp_emitter_has_", CONF, "(self) TPP_CONF_DEFAULT(TPP_EMITTER_HAVE_", CONF, ")");
	print("#endif /" "* TPP_CONF_ISCONST(TPP_EMITTER_HAVE_", CONF, ") *" "/");
}
]]]*/
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
/*[[[end]]]*/

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_OPTIONAL_EMITTER_EMITTER_FEATURES_H */
