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
#ifndef GUARD_TPP_FEATURES_C
#define GUARD_TPP_FEATURES_C 1
#define TPP_BUILDING 1

#include "api.h"
#include "config.h"
#include "features.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

/*[[[deemon
import * from deemon;
local TPP_FEATURES = File.open("features.h", "rb").read()
	.rescanf(r".*TPP_FEATURES\s*=\s*\{([^}]*)\}")
	.first
	.strip()
	.split(",")
	.map(e -> e.strip().decode("c-escape"))
	.filter(e -> e)
	.frozen;
local len = (TPP_FEATURES.each.length > ...);
print("#if TPP_HAVE_FEATURES");
print("TPP_CONST_IMPL tpp_features const tpp_features_default = {");
print("	/" "* .tf_flags = *" "/ {");
for (local feat: TPP_FEATURES) {
	print("#if TPP_HAVE_", feat, " < 0");
	print("		/" "* .tff_TPP_FEAT_", feat, " " * (len - #feat), " = *" "/ TPP_HAVE_", feat, " == -1,");
	print("#endif /" "* TPP_HAVE_", feat, " < 0 *" "/");
}
print("	}");
print("};");
print("#endif /" "* TPP_HAVE_FEATURES *" "/");
]]]*/
#if TPP_HAVE_FEATURES
TPP_CONST_IMPL tpp_features const tpp_features_default = {
	/* .tf_flags = */ {
#if TPP_HAVE_TPP_TOK_LF < 0
		/* .tff_TPP_FEAT_TPP_TOK_LF                         = */ TPP_HAVE_TPP_TOK_LF == -1,
#endif /* TPP_HAVE_TPP_TOK_LF < 0 */
#if TPP_HAVE_TPP_TOK_SPACE < 0
		/* .tff_TPP_FEAT_TPP_TOK_SPACE                      = */ TPP_HAVE_TPP_TOK_SPACE == -1,
#endif /* TPP_HAVE_TPP_TOK_SPACE < 0 */
#if TPP_HAVE_TPP_TOK_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_COMMENT                    = */ TPP_HAVE_TPP_TOK_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_CXX_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_COMMENT                = */ TPP_HAVE_TPP_TOK_CXX_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_C_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_C_COMMENT                  = */ TPP_HAVE_TPP_TOK_C_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_C_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_PASCAL_COMMENT             = */ TPP_HAVE_TPP_TOK_PASCAL_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_PASCAL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_SHELL_COMMENT              = */ TPP_HAVE_TPP_TOK_SHELL_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_SHELL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_ASM_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_ASM_COMMENT                = */ TPP_HAVE_TPP_TOK_ASM_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_ASM_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_SQL_COMMENT < 0
		/* .tff_TPP_FEAT_TPP_TOK_SQL_COMMENT                = */ TPP_HAVE_TPP_TOK_SQL_COMMENT == -1,
#endif /* TPP_HAVE_TPP_TOK_SQL_COMMENT < 0 */
#if TPP_HAVE_TPP_TOK_INT < 0
		/* .tff_TPP_FEAT_TPP_TOK_INT                        = */ TPP_HAVE_TPP_TOK_INT == -1,
#endif /* TPP_HAVE_TPP_TOK_INT < 0 */
#if TPP_HAVE_TPP_TOK_FLOAT < 0
		/* .tff_TPP_FEAT_TPP_TOK_FLOAT                      = */ TPP_HAVE_TPP_TOK_FLOAT == -1,
#endif /* TPP_HAVE_TPP_TOK_FLOAT < 0 */
#if TPP_HAVE_TPP_TOK_CHAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_CHAR                       = */ TPP_HAVE_TPP_TOK_CHAR == -1,
#endif /* TPP_HAVE_TPP_TOK_CHAR < 0 */
#if TPP_HAVE_TPP_TOK_STRING < 0
		/* .tff_TPP_FEAT_TPP_TOK_STRING                     = */ TPP_HAVE_TPP_TOK_STRING == -1,
#endif /* TPP_HAVE_TPP_TOK_STRING < 0 */
#if TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_RAW_STRING_LITERAL     = */ TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_WIDE_STRING_LITERAL    = */ TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_UTF8_STRING_LITERAL    = */ TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_UTF16_STRING_LITERAL   = */ TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_CXX_UTF32_STRING_LITERAL   = */ TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RAW_STRING_LITERAL         = */ TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RAW_CHAR_LITERAL           = */ TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_BLOCK_STRING_LITERAL       = */ TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_BLOCK_CHAR_LITERAL         = */ TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL == -1,
#endif /* TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE              = */ TPP_HAVE_TPP_TOK_LANGLE_LANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE              = */ TPP_HAVE_TPP_TOK_RANGLE_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL                = */ TPP_HAVE_TPP_TOK_EQUAL_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL              = */ TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_EQUAL               = */ TPP_HAVE_TPP_TOK_RANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_EQUAL               = */ TPP_HAVE_TPP_TOK_LANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0
		/* .tff_TPP_FEAT_TPP_TOK_DOT_DOT_DOT                = */ TPP_HAVE_TPP_TOK_DOT_DOT_DOT == -1,
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT_DOT < 0 */
#if TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_PLUS_EQUAL                 = */ TPP_HAVE_TPP_TOK_PLUS_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_PLUS_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_MINUS_EQUAL                = */ TPP_HAVE_TPP_TOK_MINUS_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_MINUS_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_STAR_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_STAR_EQUAL                 = */ TPP_HAVE_TPP_TOK_STAR_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_STAR_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_SLASH_EQUAL                = */ TPP_HAVE_TPP_TOK_SLASH_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_SLASH_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_PERCENT_EQUAL              = */ TPP_HAVE_TPP_TOK_PERCENT_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_PERCENT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_EQUAL        = */ TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_EQUAL        = */ TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_AMP_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_AMP_EQUAL                  = */ TPP_HAVE_TPP_TOK_AMP_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_AMP_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_PIPE_EQUAL                 = */ TPP_HAVE_TPP_TOK_PIPE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_PIPE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_HAT_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_HAT_EQUAL                  = */ TPP_HAVE_TPP_TOK_HAT_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_HAT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_STAR_STAR_EQUAL            = */ TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_SLASH_SLASH_EQUAL          = */ TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_AT_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_AT_EQUAL                   = */ TPP_HAVE_TPP_TOK_AT_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_AT_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_POUND_POUND < 0
		/* .tff_TPP_FEAT_TPP_TOK_POUND_POUND                = */ TPP_HAVE_TPP_TOK_POUND_POUND == -1,
#endif /* TPP_HAVE_TPP_TOK_POUND_POUND < 0 */
#if TPP_HAVE_TPP_TOK_AMP_AMP < 0
		/* .tff_TPP_FEAT_TPP_TOK_AMP_AMP                    = */ TPP_HAVE_TPP_TOK_AMP_AMP == -1,
#endif /* TPP_HAVE_TPP_TOK_AMP_AMP < 0 */
#if TPP_HAVE_TPP_TOK_PIPE_PIPE < 0
		/* .tff_TPP_FEAT_TPP_TOK_PIPE_PIPE                  = */ TPP_HAVE_TPP_TOK_PIPE_PIPE == -1,
#endif /* TPP_HAVE_TPP_TOK_PIPE_PIPE < 0 */
#if TPP_HAVE_TPP_TOK_HAT_HAT < 0
		/* .tff_TPP_FEAT_TPP_TOK_HAT_HAT                    = */ TPP_HAVE_TPP_TOK_HAT_HAT == -1,
#endif /* TPP_HAVE_TPP_TOK_HAT_HAT < 0 */
#if TPP_HAVE_TPP_TOK_PLUS_PLUS < 0
		/* .tff_TPP_FEAT_TPP_TOK_PLUS_PLUS                  = */ TPP_HAVE_TPP_TOK_PLUS_PLUS == -1,
#endif /* TPP_HAVE_TPP_TOK_PLUS_PLUS < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_MINUS < 0
		/* .tff_TPP_FEAT_TPP_TOK_MINUS_MINUS                = */ TPP_HAVE_TPP_TOK_MINUS_MINUS == -1,
#endif /* TPP_HAVE_TPP_TOK_MINUS_MINUS < 0 */
#if TPP_HAVE_TPP_TOK_STAR_STAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_STAR_STAR                  = */ TPP_HAVE_TPP_TOK_STAR_STAR == -1,
#endif /* TPP_HAVE_TPP_TOK_STAR_STAR < 0 */
#if TPP_HAVE_TPP_TOK_SLASH_SLASH < 0
		/* .tff_TPP_FEAT_TPP_TOK_SLASH_SLASH                = */ TPP_HAVE_TPP_TOK_SLASH_SLASH == -1,
#endif /* TPP_HAVE_TPP_TOK_SLASH_SLASH < 0 */
#if TPP_HAVE_TPP_TOK_TILDE_TILDE < 0
		/* .tff_TPP_FEAT_TPP_TOK_TILDE_TILDE                = */ TPP_HAVE_TPP_TOK_TILDE_TILDE == -1,
#endif /* TPP_HAVE_TPP_TOK_TILDE_TILDE < 0 */
#if TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_TILDE_EQUAL                = */ TPP_HAVE_TPP_TOK_TILDE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_TILDE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE               = */ TPP_HAVE_TPP_TOK_MINUS_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_COLON_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_COLON_EQUAL                = */ TPP_HAVE_TPP_TOK_COLON_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_COLON_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_COLON_COLON < 0
		/* .tff_TPP_FEAT_TPP_TOK_COLON_COLON                = */ TPP_HAVE_TPP_TOK_COLON_COLON == -1,
#endif /* TPP_HAVE_TPP_TOK_COLON_COLON < 0 */
#if TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_MINUS_RANGLE_STAR          = */ TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR == -1,
#endif /* TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR < 0 */
#if TPP_HAVE_TPP_TOK_DOT_STAR < 0
		/* .tff_TPP_FEAT_TPP_TOK_DOT_STAR                   = */ TPP_HAVE_TPP_TOK_DOT_STAR == -1,
#endif /* TPP_HAVE_TPP_TOK_DOT_STAR < 0 */
#if TPP_HAVE_TPP_TOK_DOT_DOT < 0
		/* .tff_TPP_FEAT_TPP_TOK_DOT_DOT                    = */ TPP_HAVE_TPP_TOK_DOT_DOT == -1,
#endif /* TPP_HAVE_TPP_TOK_DOT_DOT < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_RANGLE              = */ TPP_HAVE_TPP_TOK_LANGLE_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE       = */ TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE       = */ TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE < 0 */
#if TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL = */ TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL = */ TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_EQUAL_EQUAL_EQUAL          = */ TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_EXCLAIM_EQUAL_EQUAL        = */ TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL < 0 */
#if TPP_HAVE_TPP_TOK_QMARK_QMARK < 0
		/* .tff_TPP_FEAT_TPP_TOK_QMARK_QMARK                = */ TPP_HAVE_TPP_TOK_QMARK_QMARK == -1,
#endif /* TPP_HAVE_TPP_TOK_QMARK_QMARK < 0 */
#if TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0
		/* .tff_TPP_FEAT_TPP_TOK_QMARK_EQUAL                = */ TPP_HAVE_TPP_TOK_QMARK_EQUAL == -1,
#endif /* TPP_HAVE_TPP_TOK_QMARK_EQUAL < 0 */
#if TPP_HAVE_CPP_DIRECTIVES < 0
		/* .tff_TPP_FEAT_CPP_DIRECTIVES                     = */ TPP_HAVE_CPP_DIRECTIVES == -1,
#endif /* TPP_HAVE_CPP_DIRECTIVES < 0 */
#if TPP_HAVE_CPP_MACROS < 0
		/* .tff_TPP_FEAT_CPP_MACROS                         = */ TPP_HAVE_CPP_MACROS == -1,
#endif /* TPP_HAVE_CPP_MACROS < 0 */
#if TPP_HAVE_CPP_EXCLAIM < 0
		/* .tff_TPP_FEAT_CPP_EXCLAIM                        = */ TPP_HAVE_CPP_EXCLAIM == -1,
#endif /* TPP_HAVE_CPP_EXCLAIM < 0 */
#if TPP_HAVE_CPP_BLANK < 0
		/* .tff_TPP_FEAT_CPP_BLANK                          = */ TPP_HAVE_CPP_BLANK == -1,
#endif /* TPP_HAVE_CPP_BLANK < 0 */
#if TPP_HAVE_CPP_DIGIT_LINE < 0
		/* .tff_TPP_FEAT_CPP_DIGIT_LINE                     = */ TPP_HAVE_CPP_DIGIT_LINE == -1,
#endif /* TPP_HAVE_CPP_DIGIT_LINE < 0 */
#if TPP_HAVE_CPP_LINE < 0
		/* .tff_TPP_FEAT_CPP_LINE                           = */ TPP_HAVE_CPP_LINE == -1,
#endif /* TPP_HAVE_CPP_LINE < 0 */
#if TPP_HAVE_CPP_INCLUDE < 0
		/* .tff_TPP_FEAT_CPP_INCLUDE                        = */ TPP_HAVE_CPP_INCLUDE == -1,
#endif /* TPP_HAVE_CPP_INCLUDE < 0 */
#if TPP_HAVE_CPP_INCLUDE_NEXT < 0
		/* .tff_TPP_FEAT_CPP_INCLUDE_NEXT                   = */ TPP_HAVE_CPP_INCLUDE_NEXT == -1,
#endif /* TPP_HAVE_CPP_INCLUDE_NEXT < 0 */
#if TPP_HAVE_CPP_IMPORT < 0
		/* .tff_TPP_FEAT_CPP_IMPORT                         = */ TPP_HAVE_CPP_IMPORT == -1,
#endif /* TPP_HAVE_CPP_IMPORT < 0 */
#if TPP_HAVE_CPP_IF_ELSE_ENDIF < 0
		/* .tff_TPP_FEAT_CPP_IF_ELSE_ENDIF                  = */ TPP_HAVE_CPP_IF_ELSE_ENDIF == -1,
#endif /* TPP_HAVE_CPP_IF_ELSE_ENDIF < 0 */
#if TPP_HAVE_CPP_DEFINE < 0
		/* .tff_TPP_FEAT_CPP_DEFINE                         = */ TPP_HAVE_CPP_DEFINE == -1,
#endif /* TPP_HAVE_CPP_DEFINE < 0 */
#if TPP_HAVE_CPP_ASSERT < 0
		/* .tff_TPP_FEAT_CPP_ASSERT                         = */ TPP_HAVE_CPP_ASSERT == -1,
#endif /* TPP_HAVE_CPP_ASSERT < 0 */
#if TPP_HAVE_CPP_ERROR < 0
		/* .tff_TPP_FEAT_CPP_ERROR                          = */ TPP_HAVE_CPP_ERROR == -1,
#endif /* TPP_HAVE_CPP_ERROR < 0 */
#if TPP_HAVE_CPP_WARNING < 0
		/* .tff_TPP_FEAT_CPP_WARNING                        = */ TPP_HAVE_CPP_WARNING == -1,
#endif /* TPP_HAVE_CPP_WARNING < 0 */
#if TPP_HAVE_CPP_IDENT < 0
		/* .tff_TPP_FEAT_CPP_IDENT                          = */ TPP_HAVE_CPP_IDENT == -1,
#endif /* TPP_HAVE_CPP_IDENT < 0 */
#if TPP_HAVE_CPP_SCSS < 0
		/* .tff_TPP_FEAT_CPP_SCSS                           = */ TPP_HAVE_CPP_SCSS == -1,
#endif /* TPP_HAVE_CPP_SCSS < 0 */
#if TPP_HAVE_CPP_PRAGMA < 0
		/* .tff_TPP_FEAT_CPP_PRAGMA                         = */ TPP_HAVE_CPP_PRAGMA == -1,
#endif /* TPP_HAVE_CPP_PRAGMA < 0 */
#if TPP_HAVE_STDC_PRAGMA < 0
		/* .tff_TPP_FEAT_STDC_PRAGMA                        = */ TPP_HAVE_STDC_PRAGMA == -1,
#endif /* TPP_HAVE_STDC_PRAGMA < 0 */
#if TPP_HAVE_MSVC_PRAGMA < 0
		/* .tff_TPP_FEAT_MSVC_PRAGMA                        = */ TPP_HAVE_MSVC_PRAGMA == -1,
#endif /* TPP_HAVE_MSVC_PRAGMA < 0 */
	}
};
#endif /* TPP_HAVE_FEATURES */
/*[[[end]]]*/

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FEATURES_C */
