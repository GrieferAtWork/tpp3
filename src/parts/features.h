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
#ifndef GUARD_TPP_FEATURES_H
#define GUARD_TPP_FEATURES_H 1

#include "api.h"
#include "config.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/*[[[deemon
import * from deemon;
local features: {string: string} = Dict();
for (local line: File.open("config.h", "rb").read().decode("utf-8").splitlines(false)) {
	local TPP_HAVE_FOO, featureName;
	try {
		TPP_HAVE_FOO, featureName = line.rescanf(r'#\s*define\s+(\w+)\s*[^/]+/\*\s*"TPP_FEAT_([^"]+)"')...;
	} catch (...) {
		continue;
	}
	features["TPP_FEAT_" + featureName] = TPP_HAVE_FOO;
}

local keysLen = (features.keys.each.length > ...);
local valuesLen = (features.values.each.length > ...);
print("#undef TPP_HAVE_FEATURES");
print("#if (", "\\\n     ".join(features.values.map(
	e -> f"({e} < 0) ||".ljust(valuesLen + 10))).rstrip(" |"), ")");
print("#define TPP_HAVE_FEATURES 1");
print("#else /" "* ... *" "/");
print("#define TPP_HAVE_FEATURES 0");
print("#endif/" "* !... *" "/");
print();
print("#if TPP_HAVE_FEATURES");
print("typedef enum tpp_feature_id {");
for (local featureName, TPP_HAVE_FOO: features) {
	print("#if ", TPP_HAVE_FOO, " < 0");
	print("	", featureName, ",");
	print("#endif /" "* ", TPP_HAVE_FOO, " < 0 *" "/");
}
print("	TPP_FEAT_COUNT");
print("} tpp_feature_id;");
print();
print("typedef union tpp_features {");
print("	struct {");
for (local featureName, TPP_HAVE_FOO: features) {
	print("#if ", TPP_HAVE_FOO, " < 0");
	print("		unsigned int tff_", featureName, ": 1;");
	print("#define _tpp_features_get_", featureName, "(self) tpp_expect((self)->tf_flags.tff_", featureName, ", ", TPP_HAVE_FOO, " == -1)");
	print("#else /" "* ", TPP_HAVE_FOO, " < 0 *" "/");
	print("#define _tpp_features_get_", featureName, "(self) ", TPP_HAVE_FOO);
	print("#endif /" "* ", TPP_HAVE_FOO, " >= 0 *" "/");
}
print("	} tf_flags;");
print("	unsigned char ttf_bitset[TPP_FEAT_COUNT ? ((TPP_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];");
print("} tpp_features;");
print("");
print("TPP_CONST_DECL tpp_features const tpp_features_default;");
print("");
print("#define tpp_features_getid(self, id) \\");
print("	((self)->ttf_bitset[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_features_enable(self, id) \\");
print("	(void)((self)->ttf_bitset[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_features_disable(self, id) \\");
print("	(void)((self)->ttf_bitset[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_features_setid(self, id, enabled) \\");
print("	((enabled) ? tpp_features_enable(self, id) : tpp_features_disable(self, id))");
print("#endif /" "* TPP_HAVE_FEATURES *" "/");
]]]*/
#undef TPP_HAVE_FEATURES
#if ((TPP_HAVE_TPP_TOK_LF < 0) ||                         \
     (TPP_HAVE_TPP_TOK_SPACE < 0) ||                      \
     (TPP_HAVE_TPP_TOK_COMMENT < 0) ||                    \
     (TPP_HAVE_TPP_TOK_CXX_COMMENT < 0) ||                \
     (TPP_HAVE_TPP_TOK_C_COMMENT < 0) ||                  \
     (TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0) ||             \
     (TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0) ||              \
     (TPP_HAVE_TPP_TOK_ASM_COMMENT < 0) ||                \
     (TPP_HAVE_TPP_TOK_SQL_COMMENT < 0) ||                \
     (TPP_HAVE_TPP_TOK_DOLLAR < 0) ||                     \
     (TPP_HAVE_TPP_TOK_INT < 0) ||                        \
     (TPP_HAVE_TPP_TOK_FLOAT < 0) ||                      \
     (TPP_HAVE_TPP_TOK_CHAR < 0) ||                       \
     (TPP_HAVE_TPP_TOK_STRING < 0) ||                     \
     (TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0) ||     \
     (TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0) ||    \
     (TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0) ||    \
     (TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0) ||   \
     (TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0) ||   \
     (TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0) ||         \
     (TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0) ||           \
     (TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0) ||       \
     (TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0) ||         \
     (TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0) ||              \
     (TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0) ||              \
     (TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0) ||              \
     (TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0) ||               \
     (TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0) ||               \
     (TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0) ||                \
     (TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0) ||                 \
     (TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_STAR_EQUAL < 0) ||                 \
     (TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0) ||              \
     (TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0) ||        \
     (TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0) ||        \
     (TPP_HAVE_TPP_TOK_AMP_EQUAL < 0) ||                  \
     (TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0) ||                 \
     (TPP_HAVE_TPP_TOK_HAT_EQUAL < 0) ||                  \
     (TPP_HAVE_TPP_TOK_SLASH_SLASH < 0) ||                \
     (TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0) ||          \
     (TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0) ||            \
     (TPP_HAVE_TPP_TOK_AT_EQUAL < 0) ||                   \
     (TPP_HAVE_TPP_TOK_POUND_POUND < 0) ||                \
     (TPP_HAVE_TPP_TOK_AMP_AMP < 0) ||                    \
     (TPP_HAVE_TPP_TOK_PIPE_PIPE < 0) ||                  \
     (TPP_HAVE_TPP_TOK_HAT_HAT < 0) ||                    \
     (TPP_HAVE_TPP_TOK_PLUS_PLUS < 0) ||                  \
     (TPP_HAVE_TPP_TOK_MINUS_MINUS < 0) ||                \
     (TPP_HAVE_TPP_TOK_STAR_STAR < 0) ||                  \
     (TPP_HAVE_TPP_TOK_TILDE_TILDE < 0) ||                \
     (TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0) ||               \
     (TPP_HAVE_TPP_TOK_COLON_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_COLON_COLON < 0) ||                \
     (TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0) ||          \
     (TPP_HAVE_TPP_TOK_DOT_STAR < 0) ||                   \
     (TPP_HAVE_TPP_TOK_DOT_DOT < 0) ||                    \
     (TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0) ||              \
     (TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0) ||       \
     (TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0) ||       \
     (TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0) || \
     (TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0) || \
     (TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0) ||          \
     (TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0) ||        \
     (TPP_HAVE_TPP_TOK_QMARK_QMARK < 0) ||                \
     (TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0) ||                \
     (TPP_HAVE_TPP_TOK_RANGLE_LANGLE < 0) ||              \
     (TPP_HAVE_TPP_TOK_EQUAL_PLUS < 0) ||                 \
     (TPP_HAVE_TPP_TOK_EQUAL_MINUS < 0) ||                \
     (TPP_HAVE_TPP_TOK_EQUAL_STAR < 0) ||                 \
     (TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR < 0) ||            \
     (TPP_HAVE_TPP_TOK_EQUAL_SLASH < 0) ||                \
     (TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH < 0) ||          \
     (TPP_HAVE_TPP_TOK_EQUAL_PERCENT < 0) ||              \
     (TPP_HAVE_TPP_TOK_EQUAL_AMP < 0) ||                  \
     (TPP_HAVE_TPP_TOK_EQUAL_PIPE < 0) ||                 \
     (TPP_HAVE_TPP_TOK_EQUAL_HAT < 0) ||                  \
     (TPP_HAVE_TPP_TOK_EQUAL_LANGLE < 0) ||               \
     (TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE < 0) ||        \
     (TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE < 0) || \
     (TPP_HAVE_TPP_TOK_EQUAL_RANGLE < 0) ||               \
     (TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE < 0) ||        \
     (TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE < 0) || \
     (TPP_HAVE_TPP_TOK_EQUAL_AT < 0) ||                   \
     (TPP_HAVE_TPP_TOK_EQUAL_TILDE < 0) ||                \
     (TPP_HAVE_TPP_TOK_EQUAL_COLON < 0) ||                \
     (TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM < 0) ||              \
     (TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM < 0) ||        \
     (TPP_HAVE_TPP_TOK_EQUAL_QMARK < 0) ||                \
     (TPP_HAVE_TPP_TOK_LANGLE_MINUS < 0) ||               \
     (TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS < 0) ||          \
     (TPP_HAVE_TPP_TOK_STAR_DOT < 0) ||                   \
     (TPP_HAVE_CPP_DIRECTIVES < 0) ||                     \
     (TPP_HAVE_CPP_MACROS < 0) ||                         \
     (TPP_HAVE_CPP_BLANK < 0) ||                          \
     (TPP_HAVE_CPP_DIGIT_LINE < 0) ||                     \
     (TPP_HAVE_CPP_LINE < 0) ||                           \
     (TPP_HAVE_CPP_IF_ELSE_ENDIF < 0) ||                  \
     (TPP_HAVE_CPP_DEFINE < 0) ||                         \
     (TPP_HAVE_CPP_PRAGMA < 0))
#define TPP_HAVE_FEATURES 1
#else /* ... */
#define TPP_HAVE_FEATURES 0
#endif/* !... */

#if TPP_HAVE_FEATURES
typedef enum tpp_feature_id {
#if TPP_HAVE_TPP_TOK_LF < 0
	TPP_FEAT_TPP_TOK_LF,
#endif /* TPP_HAVE_TPP_TOK_LF < 0 */
#if TPP_HAVE_TPP_TOK_SPACE < 0
	TPP_FEAT_TPP_TOK_SPACE,
#endif /* TPP_HAVE_TPP_TOK_SPACE < 0 */
#if TPP_HAVE_TPP_TOK_COMMENT < 0
	TPP_FEAT_TPP_TOK_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_CXX_COMMENT < 0
	TPP_FEAT_TPP_TOK_CXX_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_C_COMMENT < 0
	TPP_FEAT_TPP_TOK_C_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_C_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0
	TPP_FEAT_TPP_TOK_PASCAL_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0
	TPP_FEAT_TPP_TOK_SHELL_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_ASM_COMMENT < 0
	TPP_FEAT_TPP_TOK_ASM_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_SQL_COMMENT < 0
	TPP_FEAT_TPP_TOK_SQL_COMMENT,
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_DOLLAR < 0
	TPP_FEAT_TPP_TOK_DOLLAR,
#endif /* TPP_HAVE_TPP_TOK_DOLLAR < 0 */
#if TPP_HAVE_TPP_TOK_INT < 0
	TPP_FEAT_TPP_TOK_INT,
#endif /* TPP_HAVE_TPP_TOK_INT < 0 */
#if TPP_HAVE_TPP_TOK_FLOAT < 0
	TPP_FEAT_TPP_TOK_FLOAT,
#endif /* TPP_HAVE_TPP_TOK_FLOAT < 0 */
#if TPP_HAVE_TPP_TOK_CHAR < 0
	TPP_FEAT_TPP_TOK_CHAR,
#endif /* TPP_HAVE_TPP_TOK_CHAR < 0 */
#if TPP_HAVE_TPP_TOK_STRING < 0
	TPP_FEAT_TPP_TOK_STRING,
#endif /* TPP_HAVE_TPP_TOK_STRING < 0 */
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0
	TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0
	TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0
	TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL,
#endif /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0
	TPP_FEAT_TPP_TOK_LANGLE_LANGLE,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0
	TPP_FEAT_TPP_TOK_RANGLE_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0
	TPP_FEAT_TPP_TOK_EQUAL_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0
	TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_RANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_LANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0
	TPP_FEAT_TPP_TOK_DOT_DOT_DOT,
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0 */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0
	TPP_FEAT_TPP_TOK_PLUS_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0
	TPP_FEAT_TPP_TOK_MINUS_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL < 0
	TPP_FEAT_TPP_TOK_STAR_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0
	TPP_FEAT_TPP_TOK_SLASH_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0
	TPP_FEAT_TPP_TOK_PERCENT_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL < 0
	TPP_FEAT_TPP_TOK_AMP_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0
	TPP_FEAT_TPP_TOK_PIPE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL < 0
	TPP_FEAT_TPP_TOK_HAT_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH < 0
	TPP_FEAT_TPP_TOK_SLASH_SLASH,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0
	TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0
	TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_AT_EQUAL < 0
	TPP_FEAT_TPP_TOK_AT_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_POUND_POUND < 0
	TPP_FEAT_TPP_TOK_POUND_POUND,
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND < 0 */
#if TPP_HAVE_TPP_TOK_AMP_AMP < 0
	TPP_FEAT_TPP_TOK_AMP_AMP,
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP < 0 */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE < 0
	TPP_FEAT_TPP_TOK_PIPE_PIPE,
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE < 0 */
#if TPP_HAVE_TPP_TOK_HAT_HAT < 0
	TPP_FEAT_TPP_TOK_HAT_HAT,
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT < 0 */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS < 0
	TPP_FEAT_TPP_TOK_PLUS_PLUS,
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS < 0
	TPP_FEAT_TPP_TOK_MINUS_MINUS,
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR < 0
	TPP_FEAT_TPP_TOK_STAR_STAR,
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR < 0 */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE < 0
	TPP_FEAT_TPP_TOK_TILDE_TILDE,
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE < 0 */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0
	TPP_FEAT_TPP_TOK_TILDE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0
	TPP_FEAT_TPP_TOK_MINUS_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL < 0
	TPP_FEAT_TPP_TOK_COLON_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_COLON_COLON < 0
	TPP_FEAT_TPP_TOK_COLON_COLON,
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0
	TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR,
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0 */
#if TPP_HAVE_TPP_TOK_DOT_STAR < 0
	TPP_FEAT_TPP_TOK_DOT_STAR,
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR < 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT < 0
	TPP_FEAT_TPP_TOK_DOT_DOT,
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0
	TPP_FEAT_TPP_TOK_LANGLE_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0
	TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0
	TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0
	TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0
	TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0
	TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK < 0
	TPP_FEAT_TPP_TOK_QMARK_QMARK,
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK < 0 */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0
	TPP_FEAT_TPP_TOK_QMARK_EQUAL,
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE < 0
	TPP_FEAT_TPP_TOK_RANGLE_LANGLE,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS < 0
	TPP_FEAT_TPP_TOK_EQUAL_PLUS,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS < 0
	TPP_FEAT_TPP_TOK_EQUAL_MINUS,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR < 0
	TPP_FEAT_TPP_TOK_EQUAL_STAR,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR < 0
	TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH < 0
	TPP_FEAT_TPP_TOK_EQUAL_SLASH,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH < 0
	TPP_FEAT_TPP_TOK_EQUAL_SLASH_SLASH,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT < 0
	TPP_FEAT_TPP_TOK_EQUAL_PERCENT,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP < 0
	TPP_FEAT_TPP_TOK_EQUAL_AMP,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE < 0
	TPP_FEAT_TPP_TOK_EQUAL_PIPE,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT < 0
	TPP_FEAT_TPP_TOK_EQUAL_HAT,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE < 0
	TPP_FEAT_TPP_TOK_EQUAL_LANGLE,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE < 0
	TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE < 0
	TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE < 0
	TPP_FEAT_TPP_TOK_EQUAL_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE < 0
	TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE < 0
	TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_AT < 0
	TPP_FEAT_TPP_TOK_EQUAL_AT,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE < 0
	TPP_FEAT_TPP_TOK_EQUAL_TILDE,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON < 0
	TPP_FEAT_TPP_TOK_EQUAL_COLON,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM < 0
	TPP_FEAT_TPP_TOK_EQUAL_EXCLAIM,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM < 0
	TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK < 0
	TPP_FEAT_TPP_TOK_EQUAL_QMARK,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS < 0
	TPP_FEAT_TPP_TOK_LANGLE_MINUS,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS < 0
	TPP_FEAT_TPP_TOK_STAR_LANGLE_MINUS,
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_STAR_DOT < 0
	TPP_FEAT_TPP_TOK_STAR_DOT,
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT < 0 */
#if TPP_HAVE_CPP_DIRECTIVES < 0
	TPP_FEAT_CPP_DIRECTIVES,
#endif /* TPP_HAVE_CPP_DIRECTIVES < 0 */
#if TPP_HAVE_CPP_MACROS < 0
	TPP_FEAT_CPP_MACROS,
#endif /* TPP_HAVE_CPP_MACROS < 0 */
#if TPP_HAVE_CPP_BLANK < 0
	TPP_FEAT_CPP_BLANK,
#endif /* TPP_HAVE_CPP_BLANK < 0 */
#if TPP_HAVE_CPP_DIGIT_LINE < 0
	TPP_FEAT_CPP_DIGIT_LINE,
#endif /* TPP_HAVE_CPP_DIGIT_LINE < 0 */
#if TPP_HAVE_CPP_LINE < 0
	TPP_FEAT_CPP_LINE,
#endif /* TPP_HAVE_CPP_LINE < 0 */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF < 0
	TPP_FEAT_CPP_IF_ELSE_ENDIF,
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF < 0 */
#if TPP_HAVE_CPP_DEFINE < 0
	TPP_FEAT_CPP_DEFINE,
#endif /* TPP_HAVE_CPP_DEFINE < 0 */
#if TPP_HAVE_CPP_PRAGMA < 0
	TPP_FEAT_CPP_PRAGMA,
#endif /* TPP_HAVE_CPP_PRAGMA < 0 */
	TPP_FEAT_COUNT
} tpp_feature_id;

typedef union tpp_features {
	struct {
#if TPP_HAVE_TPP_TOK_LF < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LF: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LF(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LF, TPP_HAVE_TPP_TOK_LF == -1)
#else /* TPP_HAVE_TPP_TOK_LF < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LF(self) TPP_HAVE_TPP_TOK_LF
#endif /* TPP_HAVE_TPP_TOK_LF >= 0 */
#if TPP_HAVE_TPP_TOK_SPACE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SPACE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SPACE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SPACE, TPP_HAVE_TPP_TOK_SPACE == -1)
#else /* TPP_HAVE_TPP_TOK_SPACE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SPACE(self) TPP_HAVE_TPP_TOK_SPACE
#endif /* TPP_HAVE_TPP_TOK_SPACE >= 0 */
#if TPP_HAVE_TPP_TOK_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_COMMENT, TPP_HAVE_TPP_TOK_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COMMENT(self) TPP_HAVE_TPP_TOK_COMMENT
#endif /* TPP_HAVE_TPP_TOK_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_COMMENT, TPP_HAVE_TPP_TOK_CXX_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_COMMENT(self) TPP_HAVE_TPP_TOK_CXX_COMMENT
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_C_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_C_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_C_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_C_COMMENT, TPP_HAVE_TPP_TOK_C_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_C_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_C_COMMENT(self) TPP_HAVE_TPP_TOK_C_COMMENT
#endif /* TPP_HAVE_TPP_TOK_C_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PASCAL_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PASCAL_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PASCAL_COMMENT, TPP_HAVE_TPP_TOK_PASCAL_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PASCAL_COMMENT(self) TPP_HAVE_TPP_TOK_PASCAL_COMMENT
#endif /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SHELL_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SHELL_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SHELL_COMMENT, TPP_HAVE_TPP_TOK_SHELL_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SHELL_COMMENT(self) TPP_HAVE_TPP_TOK_SHELL_COMMENT
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_ASM_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_ASM_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_ASM_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_ASM_COMMENT, TPP_HAVE_TPP_TOK_ASM_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_ASM_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_ASM_COMMENT(self) TPP_HAVE_TPP_TOK_ASM_COMMENT
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_SQL_COMMENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SQL_COMMENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SQL_COMMENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SQL_COMMENT, TPP_HAVE_TPP_TOK_SQL_COMMENT == -1)
#else /* TPP_HAVE_TPP_TOK_SQL_COMMENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SQL_COMMENT(self) TPP_HAVE_TPP_TOK_SQL_COMMENT
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT >= 0 */
#if TPP_HAVE_TPP_TOK_DOLLAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_DOLLAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOLLAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_DOLLAR, TPP_HAVE_TPP_TOK_DOLLAR == -1)
#else /* TPP_HAVE_TPP_TOK_DOLLAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOLLAR(self) TPP_HAVE_TPP_TOK_DOLLAR
#endif /* TPP_HAVE_TPP_TOK_DOLLAR >= 0 */
#if TPP_HAVE_TPP_TOK_INT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_INT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_INT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_INT, TPP_HAVE_TPP_TOK_INT == -1)
#else /* TPP_HAVE_TPP_TOK_INT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_INT(self) TPP_HAVE_TPP_TOK_INT
#endif /* TPP_HAVE_TPP_TOK_INT >= 0 */
#if TPP_HAVE_TPP_TOK_FLOAT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_FLOAT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_FLOAT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_FLOAT, TPP_HAVE_TPP_TOK_FLOAT == -1)
#else /* TPP_HAVE_TPP_TOK_FLOAT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_FLOAT(self) TPP_HAVE_TPP_TOK_FLOAT
#endif /* TPP_HAVE_TPP_TOK_FLOAT >= 0 */
#if TPP_HAVE_TPP_TOK_CHAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CHAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CHAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CHAR, TPP_HAVE_TPP_TOK_CHAR == -1)
#else /* TPP_HAVE_TPP_TOK_CHAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CHAR(self) TPP_HAVE_TPP_TOK_CHAR
#endif /* TPP_HAVE_TPP_TOK_CHAR >= 0 */
#if TPP_HAVE_TPP_TOK_STRING < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STRING: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STRING(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STRING, TPP_HAVE_TPP_TOK_STRING == -1)
#else /* TPP_HAVE_TPP_TOK_STRING < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STRING(self) TPP_HAVE_TPP_TOK_STRING
#endif /* TPP_HAVE_TPP_TOK_STRING >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL, TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL, TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL, TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL(self) TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL, TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL(self) TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL, TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL == -1)
#else /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL(self) TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
#endif /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE, TPP_HAVE_TPP_TOK_LANGLE_LANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE(self) TPP_HAVE_TPP_TOK_LANGLE_LANGLE
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE, TPP_HAVE_TPP_TOK_RANGLE_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE(self) TPP_HAVE_TPP_TOK_RANGLE_RANGLE
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL, TPP_HAVE_TPP_TOK_EQUAL_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL(self) TPP_HAVE_TPP_TOK_EQUAL_EQUAL
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL, TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL(self) TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_EQUAL, TPP_HAVE_TPP_TOK_RANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_RANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_EQUAL, TPP_HAVE_TPP_TOK_LANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_LANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_DOT_DOT_DOT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_DOT_DOT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_DOT_DOT_DOT, TPP_HAVE_TPP_TOK_DOT_DOT_DOT == -1)
#else /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_DOT_DOT(self) TPP_HAVE_TPP_TOK_DOT_DOT_DOT
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT >= 0 */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PLUS_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PLUS_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PLUS_EQUAL, TPP_HAVE_TPP_TOK_PLUS_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PLUS_EQUAL(self) TPP_HAVE_TPP_TOK_PLUS_EQUAL
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_MINUS_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_MINUS_EQUAL, TPP_HAVE_TPP_TOK_MINUS_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_EQUAL(self) TPP_HAVE_TPP_TOK_MINUS_EQUAL
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STAR_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STAR_EQUAL, TPP_HAVE_TPP_TOK_STAR_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_STAR_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_EQUAL(self) TPP_HAVE_TPP_TOK_STAR_EQUAL
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SLASH_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SLASH_EQUAL, TPP_HAVE_TPP_TOK_SLASH_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_EQUAL(self) TPP_HAVE_TPP_TOK_SLASH_EQUAL
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PERCENT_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PERCENT_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PERCENT_EQUAL, TPP_HAVE_TPP_TOK_PERCENT_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PERCENT_EQUAL(self) TPP_HAVE_TPP_TOK_PERCENT_EQUAL
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL, TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL, TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_AMP_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AMP_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_AMP_EQUAL, TPP_HAVE_TPP_TOK_AMP_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_AMP_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AMP_EQUAL(self) TPP_HAVE_TPP_TOK_AMP_EQUAL
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PIPE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PIPE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PIPE_EQUAL, TPP_HAVE_TPP_TOK_PIPE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PIPE_EQUAL(self) TPP_HAVE_TPP_TOK_PIPE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_HAT_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_HAT_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_HAT_EQUAL, TPP_HAVE_TPP_TOK_HAT_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_HAT_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_HAT_EQUAL(self) TPP_HAVE_TPP_TOK_HAT_EQUAL
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SLASH_SLASH: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_SLASH(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SLASH_SLASH, TPP_HAVE_TPP_TOK_SLASH_SLASH == -1)
#else /* TPP_HAVE_TPP_TOK_SLASH_SLASH < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_SLASH(self) TPP_HAVE_TPP_TOK_SLASH_SLASH
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH >= 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL, TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL(self) TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL, TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL(self) TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_AT_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_AT_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AT_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_AT_EQUAL, TPP_HAVE_TPP_TOK_AT_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_AT_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AT_EQUAL(self) TPP_HAVE_TPP_TOK_AT_EQUAL
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_POUND_POUND < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_POUND_POUND: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_POUND_POUND(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_POUND_POUND, TPP_HAVE_TPP_TOK_POUND_POUND == -1)
#else /* TPP_HAVE_TPP_TOK_POUND_POUND < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_POUND_POUND(self) TPP_HAVE_TPP_TOK_POUND_POUND
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND >= 0 */
#if TPP_HAVE_TPP_TOK_AMP_AMP < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_AMP_AMP: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AMP_AMP(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_AMP_AMP, TPP_HAVE_TPP_TOK_AMP_AMP == -1)
#else /* TPP_HAVE_TPP_TOK_AMP_AMP < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_AMP_AMP(self) TPP_HAVE_TPP_TOK_AMP_AMP
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP >= 0 */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PIPE_PIPE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PIPE_PIPE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PIPE_PIPE, TPP_HAVE_TPP_TOK_PIPE_PIPE == -1)
#else /* TPP_HAVE_TPP_TOK_PIPE_PIPE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PIPE_PIPE(self) TPP_HAVE_TPP_TOK_PIPE_PIPE
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE >= 0 */
#if TPP_HAVE_TPP_TOK_HAT_HAT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_HAT_HAT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_HAT_HAT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_HAT_HAT, TPP_HAVE_TPP_TOK_HAT_HAT == -1)
#else /* TPP_HAVE_TPP_TOK_HAT_HAT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_HAT_HAT(self) TPP_HAVE_TPP_TOK_HAT_HAT
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT >= 0 */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_PLUS_PLUS: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PLUS_PLUS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_PLUS_PLUS, TPP_HAVE_TPP_TOK_PLUS_PLUS == -1)
#else /* TPP_HAVE_TPP_TOK_PLUS_PLUS < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_PLUS_PLUS(self) TPP_HAVE_TPP_TOK_PLUS_PLUS
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS >= 0 */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_MINUS_MINUS: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_MINUS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_MINUS_MINUS, TPP_HAVE_TPP_TOK_MINUS_MINUS == -1)
#else /* TPP_HAVE_TPP_TOK_MINUS_MINUS < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_MINUS(self) TPP_HAVE_TPP_TOK_MINUS_MINUS
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS >= 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STAR_STAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_STAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STAR_STAR, TPP_HAVE_TPP_TOK_STAR_STAR == -1)
#else /* TPP_HAVE_TPP_TOK_STAR_STAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_STAR(self) TPP_HAVE_TPP_TOK_STAR_STAR
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR >= 0 */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_TILDE_TILDE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_TILDE_TILDE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_TILDE_TILDE, TPP_HAVE_TPP_TOK_TILDE_TILDE == -1)
#else /* TPP_HAVE_TPP_TOK_TILDE_TILDE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_TILDE_TILDE(self) TPP_HAVE_TPP_TOK_TILDE_TILDE
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE >= 0 */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_TILDE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_TILDE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_TILDE_EQUAL, TPP_HAVE_TPP_TOK_TILDE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_TILDE_EQUAL(self) TPP_HAVE_TPP_TOK_TILDE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE, TPP_HAVE_TPP_TOK_MINUS_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_RANGLE(self) TPP_HAVE_TPP_TOK_MINUS_RANGLE
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_COLON_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COLON_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_COLON_EQUAL, TPP_HAVE_TPP_TOK_COLON_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_COLON_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COLON_EQUAL(self) TPP_HAVE_TPP_TOK_COLON_EQUAL
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_COLON_COLON < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_COLON_COLON: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COLON_COLON(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_COLON_COLON, TPP_HAVE_TPP_TOK_COLON_COLON == -1)
#else /* TPP_HAVE_TPP_TOK_COLON_COLON < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_COLON_COLON(self) TPP_HAVE_TPP_TOK_COLON_COLON
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON >= 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR, TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR == -1)
#else /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR(self) TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR >= 0 */
#if TPP_HAVE_TPP_TOK_DOT_STAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_DOT_STAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_STAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_DOT_STAR, TPP_HAVE_TPP_TOK_DOT_STAR == -1)
#else /* TPP_HAVE_TPP_TOK_DOT_STAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_STAR(self) TPP_HAVE_TPP_TOK_DOT_STAR
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR >= 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_DOT_DOT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_DOT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_DOT_DOT, TPP_HAVE_TPP_TOK_DOT_DOT == -1)
#else /* TPP_HAVE_TPP_TOK_DOT_DOT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_DOT_DOT(self) TPP_HAVE_TPP_TOK_DOT_DOT
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_RANGLE, TPP_HAVE_TPP_TOK_LANGLE_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_RANGLE(self) TPP_HAVE_TPP_TOK_LANGLE_RANGLE
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE, TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE(self) TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE, TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE(self) TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL, TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL, TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL(self) TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL, TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL(self) TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL, TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL(self) TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_QMARK_QMARK: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_QMARK_QMARK(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_QMARK_QMARK, TPP_HAVE_TPP_TOK_QMARK_QMARK == -1)
#else /* TPP_HAVE_TPP_TOK_QMARK_QMARK < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_QMARK_QMARK(self) TPP_HAVE_TPP_TOK_QMARK_QMARK
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK >= 0 */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_QMARK_EQUAL: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_QMARK_EQUAL(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_QMARK_EQUAL, TPP_HAVE_TPP_TOK_QMARK_EQUAL == -1)
#else /* TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_QMARK_EQUAL(self) TPP_HAVE_TPP_TOK_QMARK_EQUAL
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL >= 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_LANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_RANGLE_LANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_LANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_RANGLE_LANGLE, TPP_HAVE_TPP_TOK_RANGLE_LANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_RANGLE_LANGLE(self) TPP_HAVE_TPP_TOK_RANGLE_LANGLE
#endif /* TPP_HAVE_TPP_TOK_RANGLE_LANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_PLUS < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_PLUS: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_PLUS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_PLUS, TPP_HAVE_TPP_TOK_EQUAL_PLUS == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_PLUS < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_PLUS(self) TPP_HAVE_TPP_TOK_EQUAL_PLUS
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PLUS >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_MINUS < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_MINUS: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_MINUS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_MINUS, TPP_HAVE_TPP_TOK_EQUAL_MINUS == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_MINUS < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_MINUS(self) TPP_HAVE_TPP_TOK_EQUAL_MINUS
#endif /* TPP_HAVE_TPP_TOK_EQUAL_MINUS >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_STAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_STAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_STAR, TPP_HAVE_TPP_TOK_EQUAL_STAR == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_STAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_STAR(self) TPP_HAVE_TPP_TOK_EQUAL_STAR
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR, TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_STAR_STAR(self) TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
#endif /* TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_SLASH: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_SLASH(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_SLASH, TPP_HAVE_TPP_TOK_EQUAL_SLASH == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_SLASH < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_SLASH(self) TPP_HAVE_TPP_TOK_EQUAL_SLASH
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_SLASH_SLASH: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_SLASH_SLASH(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_SLASH_SLASH, TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_SLASH_SLASH(self) TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
#endif /* TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_PERCENT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_PERCENT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_PERCENT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_PERCENT, TPP_HAVE_TPP_TOK_EQUAL_PERCENT == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_PERCENT(self) TPP_HAVE_TPP_TOK_EQUAL_PERCENT
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PERCENT >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_AMP < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_AMP: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_AMP(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_AMP, TPP_HAVE_TPP_TOK_EQUAL_AMP == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_AMP < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_AMP(self) TPP_HAVE_TPP_TOK_EQUAL_AMP
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AMP >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_PIPE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_PIPE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_PIPE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_PIPE, TPP_HAVE_TPP_TOK_EQUAL_PIPE == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_PIPE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_PIPE(self) TPP_HAVE_TPP_TOK_EQUAL_PIPE
#endif /* TPP_HAVE_TPP_TOK_EQUAL_PIPE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_HAT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_HAT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_HAT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_HAT, TPP_HAVE_TPP_TOK_EQUAL_HAT == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_HAT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_HAT(self) TPP_HAVE_TPP_TOK_EQUAL_HAT
#endif /* TPP_HAVE_TPP_TOK_EQUAL_HAT >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_LANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_LANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_LANGLE, TPP_HAVE_TPP_TOK_EQUAL_LANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_LANGLE(self) TPP_HAVE_TPP_TOK_EQUAL_LANGLE
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE, TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE(self) TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE, TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE(self) TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
#endif /* TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_RANGLE, TPP_HAVE_TPP_TOK_EQUAL_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_RANGLE(self) TPP_HAVE_TPP_TOK_EQUAL_RANGLE
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE, TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE(self) TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE, TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE(self) TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
#endif /* TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_AT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_AT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_AT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_AT, TPP_HAVE_TPP_TOK_EQUAL_AT == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_AT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_AT(self) TPP_HAVE_TPP_TOK_EQUAL_AT
#endif /* TPP_HAVE_TPP_TOK_EQUAL_AT >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_TILDE < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_TILDE: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_TILDE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_TILDE, TPP_HAVE_TPP_TOK_EQUAL_TILDE == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_TILDE < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_TILDE(self) TPP_HAVE_TPP_TOK_EQUAL_TILDE
#endif /* TPP_HAVE_TPP_TOK_EQUAL_TILDE >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_COLON < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_COLON: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_COLON(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_COLON, TPP_HAVE_TPP_TOK_EQUAL_COLON == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_COLON < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_COLON(self) TPP_HAVE_TPP_TOK_EQUAL_COLON
#endif /* TPP_HAVE_TPP_TOK_EQUAL_COLON >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_EXCLAIM: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EXCLAIM(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_EXCLAIM, TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EXCLAIM(self) TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM, TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EXCLAIM(self) TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM >= 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_QMARK < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_EQUAL_QMARK: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_QMARK(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_EQUAL_QMARK, TPP_HAVE_TPP_TOK_EQUAL_QMARK == -1)
#else /* TPP_HAVE_TPP_TOK_EQUAL_QMARK < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_EQUAL_QMARK(self) TPP_HAVE_TPP_TOK_EQUAL_QMARK
#endif /* TPP_HAVE_TPP_TOK_EQUAL_QMARK >= 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_MINUS < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_LANGLE_MINUS: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_MINUS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_LANGLE_MINUS, TPP_HAVE_TPP_TOK_LANGLE_MINUS == -1)
#else /* TPP_HAVE_TPP_TOK_LANGLE_MINUS < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_LANGLE_MINUS(self) TPP_HAVE_TPP_TOK_LANGLE_MINUS
#endif /* TPP_HAVE_TPP_TOK_LANGLE_MINUS >= 0 */
#if TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STAR_LANGLE_MINUS: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_LANGLE_MINUS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STAR_LANGLE_MINUS, TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS == -1)
#else /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_LANGLE_MINUS(self) TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
#endif /* TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS >= 0 */
#if TPP_HAVE_TPP_TOK_STAR_DOT < 0
		unsigned int tff_TPP_FEAT_TPP_TOK_STAR_DOT: 1;
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_DOT(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_TPP_TOK_STAR_DOT, TPP_HAVE_TPP_TOK_STAR_DOT == -1)
#else /* TPP_HAVE_TPP_TOK_STAR_DOT < 0 */
#define _tpp_features_get_TPP_FEAT_TPP_TOK_STAR_DOT(self) TPP_HAVE_TPP_TOK_STAR_DOT
#endif /* TPP_HAVE_TPP_TOK_STAR_DOT >= 0 */
#if TPP_HAVE_CPP_DIRECTIVES < 0
		unsigned int tff_TPP_FEAT_CPP_DIRECTIVES: 1;
#define _tpp_features_get_TPP_FEAT_CPP_DIRECTIVES(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_DIRECTIVES, TPP_HAVE_CPP_DIRECTIVES == -1)
#else /* TPP_HAVE_CPP_DIRECTIVES < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_DIRECTIVES(self) TPP_HAVE_CPP_DIRECTIVES
#endif /* TPP_HAVE_CPP_DIRECTIVES >= 0 */
#if TPP_HAVE_CPP_MACROS < 0
		unsigned int tff_TPP_FEAT_CPP_MACROS: 1;
#define _tpp_features_get_TPP_FEAT_CPP_MACROS(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_MACROS, TPP_HAVE_CPP_MACROS == -1)
#else /* TPP_HAVE_CPP_MACROS < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_MACROS(self) TPP_HAVE_CPP_MACROS
#endif /* TPP_HAVE_CPP_MACROS >= 0 */
#if TPP_HAVE_CPP_BLANK < 0
		unsigned int tff_TPP_FEAT_CPP_BLANK: 1;
#define _tpp_features_get_TPP_FEAT_CPP_BLANK(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_BLANK, TPP_HAVE_CPP_BLANK == -1)
#else /* TPP_HAVE_CPP_BLANK < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_BLANK(self) TPP_HAVE_CPP_BLANK
#endif /* TPP_HAVE_CPP_BLANK >= 0 */
#if TPP_HAVE_CPP_DIGIT_LINE < 0
		unsigned int tff_TPP_FEAT_CPP_DIGIT_LINE: 1;
#define _tpp_features_get_TPP_FEAT_CPP_DIGIT_LINE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_DIGIT_LINE, TPP_HAVE_CPP_DIGIT_LINE == -1)
#else /* TPP_HAVE_CPP_DIGIT_LINE < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_DIGIT_LINE(self) TPP_HAVE_CPP_DIGIT_LINE
#endif /* TPP_HAVE_CPP_DIGIT_LINE >= 0 */
#if TPP_HAVE_CPP_LINE < 0
		unsigned int tff_TPP_FEAT_CPP_LINE: 1;
#define _tpp_features_get_TPP_FEAT_CPP_LINE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_LINE, TPP_HAVE_CPP_LINE == -1)
#else /* TPP_HAVE_CPP_LINE < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_LINE(self) TPP_HAVE_CPP_LINE
#endif /* TPP_HAVE_CPP_LINE >= 0 */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF < 0
		unsigned int tff_TPP_FEAT_CPP_IF_ELSE_ENDIF: 1;
#define _tpp_features_get_TPP_FEAT_CPP_IF_ELSE_ENDIF(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_IF_ELSE_ENDIF, TPP_HAVE_CPP_IF_ELSE_ENDIF == -1)
#else /* TPP_HAVE_CPP_IF_ELSE_ENDIF < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_IF_ELSE_ENDIF(self) TPP_HAVE_CPP_IF_ELSE_ENDIF
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF >= 0 */
#if TPP_HAVE_CPP_DEFINE < 0
		unsigned int tff_TPP_FEAT_CPP_DEFINE: 1;
#define _tpp_features_get_TPP_FEAT_CPP_DEFINE(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_DEFINE, TPP_HAVE_CPP_DEFINE == -1)
#else /* TPP_HAVE_CPP_DEFINE < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_DEFINE(self) TPP_HAVE_CPP_DEFINE
#endif /* TPP_HAVE_CPP_DEFINE >= 0 */
#if TPP_HAVE_CPP_PRAGMA < 0
		unsigned int tff_TPP_FEAT_CPP_PRAGMA: 1;
#define _tpp_features_get_TPP_FEAT_CPP_PRAGMA(self) tpp_expect((self)->tf_flags.tff_TPP_FEAT_CPP_PRAGMA, TPP_HAVE_CPP_PRAGMA == -1)
#else /* TPP_HAVE_CPP_PRAGMA < 0 */
#define _tpp_features_get_TPP_FEAT_CPP_PRAGMA(self) TPP_HAVE_CPP_PRAGMA
#endif /* TPP_HAVE_CPP_PRAGMA >= 0 */
	} tf_flags;
	unsigned char ttf_bitset[TPP_FEAT_COUNT ? ((TPP_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_features;

TPP_CONST_DECL tpp_features const tpp_features_default;

#define tpp_features_getid(self, id) \
	((self)->ttf_bitset[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_features_enable(self, id) \
	(void)((self)->ttf_bitset[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_features_disable(self, id) \
	(void)((self)->ttf_bitset[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_features_setid(self, id, enabled) \
	((enabled) ? tpp_features_enable(self, id) : tpp_features_disable(self, id))
#endif /* TPP_HAVE_FEATURES */
/*[[[end]]]*/

#if TPP_HAVE_FEATURES
#define tpp_features_init(self) (void)(*(self) = tpp_features_default)
#define tpp_features_fini(self) (void)0
#define tpp_features_get(self, TPP_FEAT_x) _tpp_features_get_##TPP_FEAT_x(self)
#else /* TPP_HAVE_FEATURES */
#define tpp_features_get(self, TPP_FEAT_x) 1
#endif /* !TPP_HAVE_FEATURES */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FEATURES_H */
