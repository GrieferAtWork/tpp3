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
local configs: {string...} = [];
for (local line: File.open("config.h", "rb").read().decode("utf-8").splitlines(false)) {
	local TPP_HAVE_FOO, extName;
	try {
		TPP_HAVE_FOO, extName = line.rescanf(r'#\s*define\s+(\w+)\s*[^/]+/\*\s*"-f([^"]+)"')...;
	} catch (...) {
		continue;
	}
	local CONF = TPP_HAVE_FOO.lsstrip("TPP_HAVE_");
	configs.append(CONF);
}

local configsLen = (configs.each.length > ...);
print("#undef TPP_HAVE_FEATURES");
print("#if (", "\\\n     ".join(configs.map(
	e -> f"TPP_CONF_IS_FEAT(TPP_HAVE_{e}) ||".ljust(configsLen + 31))).rstrip(" |"), ")");
print("#define TPP_HAVE_FEATURES 1");
print("#else /" "* ... *" "/");
print("#define TPP_HAVE_FEATURES 0");
print("#endif/" "* !... *" "/");
print();
print("#if TPP_HAVE_FEATURES");
print("typedef enum tpp_feature_id {");
for (local CONF: configs) {
	print("#if TPP_CONF_IS_FEAT(TPP_HAVE_", CONF, ")");
	print("	TPP_FEAT_", CONF, ",");
	print("#endif /" "* TPP_CONF_IS_FEAT(TPP_HAVE_", CONF, ") *" "/");
}
print("	TPP_FEAT_COUNT");
print("} tpp_feature_id;");
print();
print("typedef union tpp_features {");
print("	struct {");
for (local CONF: configs) {
	print("#if TPP_CONF_IS_FEAT(TPP_HAVE_", CONF, ")");
	print("		unsigned int TPP_INTERNAL(tff_", CONF, "): 1;");
	print("#define _tpp_lexer_has_", CONF, "(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_", CONF, ")");
	print("#endif /" "* TPP_CONF_IS_FEAT(TPP_HAVE_", CONF, ") *" "/");
}
print("	} TPP_INTERNAL(tf_flags);");
print("	unsigned char TPP_INTERNAL(ttf_bitset)[TPP_FEAT_COUNT ? ((TPP_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];");
print("} tpp_features;");
print("");
print("TPP_CONST_DECL tpp_features const tpp_features_default;");
print("");
print("#define tpp_features_getid(self, id) \\");
print("	((self)->TPP_INTERNAL(ttf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_features_enable(self, id) \\");
print("	(void)((self)->TPP_INTERNAL(ttf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_features_disable(self, id) \\");
print("	(void)((self)->TPP_INTERNAL(ttf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))");
print("#define tpp_features_setid(self, id, enabled) \\");
print("	((enabled) ? tpp_features_enable(self, id) : tpp_features_disable(self, id))");
print("#define tpp_features_init(self)            (void)(*(self) = tpp_features_default)");
print("#define tpp_features_reset(self)           (void)(*(self) = tpp_features_default)");
print("#define tpp_features_fini(self)            tpp_dbg_memset(self, sizeof(tpp_features))");
print("#endif /" "* TPP_HAVE_FEATURES *" "/");
print;
for (local CONF: configs) {
	print("#if TPP_CONF_IS_CONST(TPP_HAVE_", CONF, ")");
	print("#define _tpp_lexer_has_", CONF, "(self) TPP_CONF_DEFAULT(TPP_HAVE_", CONF, ")");
	print("#endif /" "* TPP_CONF_IS_CONST(TPP_HAVE_", CONF, ") *" "/");
}
]]]*/
#undef TPP_HAVE_FEATURES
#if (TPP_CONF_IS_FEAT(TPP_HAVE_BSE) ||                                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_BSE_WHITESPACE) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_E_IN_STRINGS) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_S_IN_STRINGS) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIRECTIVES) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_MACROS) ||                             \
     TPP_CONF_IS_FEAT(TPP_HAVE_MAGIC_WHITESPACE) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BUILTIN_MACROS) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EXCLAIM) ||                            \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BLANK) ||                              \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIGIT_LINE) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_LINE) ||                               \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE) ||                            \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE_NEXT) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IMPORT) ||                             \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IF_ELSE_ENDIF) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DEFINE) ||                             \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ASSERT) ||                             \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ERROR) ||                              \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_WARNING) ||                            \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IDENT_SCCS) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_PRAGMA) ||                             \
     TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EMBED) ||                              \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO__Pragma) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___pragma) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_attribute) ||            \
     TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_builtin) ||              \
     TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute) ||        \
     TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute) ||   \
     TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_extension) ||            \
     TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_feature) ||              \
     TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_c_attribute) ||          \
     TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES) ||          \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_identifier) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_deprecated) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_poisoned) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_extension) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_extension) ||            \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_warning) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_warning) ||              \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include_next) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_embed) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE__) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___LINE__) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIME__) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___DATE__) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COLUMN__) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___BASE_FILE__) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE_NAME__) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_LEVEL__) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_DEPTH__) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COUNTER__) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIMESTAMP__) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_DATE_MACROS) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_TIME_MACROS) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EVAL) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EXEC) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_UNIQUE) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_LOAD_FILE) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNTER) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_RANDOM) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE) ||              \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_PACK) ||                   \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SUBSTR) ||                 \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SIZE) ||                   \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_IDENTIFIER) ||                 \
     TPP_CONF_IS_FEAT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS) ||          \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) ||              \
     TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_RECURSION) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TRADITIONAL_MACROS) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_NAMED_VARARGS_IN_MACROS) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_VA_ARGS_IN_MACROS) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_VA_COMMA_IN_MACROS) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_VA_OPT_IN_MACROS) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_VA_NARGS_IN_MACROS) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT) ||                 \
     TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT) ||             \
     TPP_CONF_IS_FEAT(TPP_HAVE_GLUE_MACRO_ARGUMENT) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_PUSH_MACRO) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ONCE) ||                            \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_DEPRECATED) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_EXTENSION) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_WARNING) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ERROR) ||                           \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_REGION) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_EXEC) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) ||           \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_POISON) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_WARNING) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_ERROR) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS) ||                              \
     TPP_CONF_IS_FEAT(TPP_HAVE_DIGRAPHS) ||                               \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LF) ||                                 \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SPACE) ||                              \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COMMENT) ||                            \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_COMMENT) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_COMMENT) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_COMMENT) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_BRACE_COMMENT) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HTML_COMMENT) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SQL_COMMENT) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_COMMENT) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SHELL_COMMENT) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_COMMENT) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_COMMENT) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SHELL_COMMENT) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SLASH_COMMENT) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_AT_COMMENT) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOLLAR) ||                             \
     TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_INT) ||                              \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_HEX) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_FLOAT) ||                            \
     TPP_CONF_IS_FEAT(TPP_HAVE_SMART_FLOAT_TOKENS) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_CHAR) ||                             \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_STRING) ||                           \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL) ||             \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL) ||            \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL) ||            \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL) ||           \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL) ||           \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL) ||              \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL) ||              \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL) ||             \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL) ||             \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_STRING_LITERAL) ||                 \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_CHAR_LITERAL) ||                   \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_STRING_LITERAL) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_CHAR_LITERAL) ||                 \
     TPP_CONF_IS_FEAT(TPP_HAVE_STRING_ALLOW_MULTILINE) ||                 \
     TPP_CONF_IS_FEAT(TPP_HAVE_STRING_AUTO_CONCAT) ||                     \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EXCLAIM) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_POUND_POUND) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL) ||              \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_EQUAL) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_AMP) ||                            \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_EQUAL) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR_EQUAL) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_DOT) ||                           \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_LANGLE_MINUS) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_EQUAL) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_PLUS) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_EQUAL) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_MINUS) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_EQUAL) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_STAR) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_STAR) ||                           \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT) ||                            \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT_DOT) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH_EQUAL) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_EQUAL) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_COLON) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_EQUAL) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_LANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_RANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_MINUS) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_RANGLE) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EXCLAIM) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT) ||              \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AMP) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR_STAR) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PLUS) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_MINUS) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH_SLASH) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_COLON) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL) ||                  \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_QMARK) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT) ||                           \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT_AT) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_HAT) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PIPE) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_TILDE) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_LANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_RANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_LANGLE) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL) ||                       \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE) ||                      \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_MINUS) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL) ||                \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_EQUAL) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_QMARK) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_EQUAL) ||                           \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT) ||                              \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_EQUAL) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_EQUAL) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_HAT) ||                            \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_EQUAL) ||                         \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_PIPE) ||                          \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_EQUAL) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_TILDE) ||                        \
     TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_DEFINED) ||                   \
     TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR) ||            \
     TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_STRINGS) ||                   \
     TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FLOATS) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT) ||       \
     TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS) ||    \
     TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR) ||               \
     TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS) ||           \
     TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS) ||            \
     TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX) ||      \
     TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX) ||       \
     TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX) ||    \
     TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX) ||    \
     TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX) ||   \
     TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX) ||  \
     TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS) ||        \
     TPP_CONF_IS_FEAT(TPP_HAVE_EXTERN_C_FOR_SYSHDR) ||                    \
     TPP_CONF_IS_FEAT(TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH) ||             \
     TPP_CONF_IS_FEAT(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION) ||         \
     TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED))
#define TPP_HAVE_FEATURES 1
#else /* ... */
#define TPP_HAVE_FEATURES 0
#endif/* !... */

#if TPP_HAVE_FEATURES
typedef enum tpp_feature_id {
#if TPP_CONF_IS_FEAT(TPP_HAVE_BSE)
	TPP_FEAT_BSE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BSE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BSE_WHITESPACE)
	TPP_FEAT_BSE_WHITESPACE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BSE_WHITESPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
	TPP_FEAT_ESCAPE_IN_IDENTIFIERS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_E_IN_STRINGS)
	TPP_FEAT_ESCAPE_E_IN_STRINGS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_E_IN_STRINGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_S_IN_STRINGS)
	TPP_FEAT_ESCAPE_S_IN_STRINGS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_S_IN_STRINGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIRECTIVES)
	TPP_FEAT_CPP_DIRECTIVES,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIRECTIVES) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_MACROS)
	TPP_FEAT_CPP_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MAGIC_WHITESPACE)
	TPP_FEAT_MAGIC_WHITESPACE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MAGIC_WHITESPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BUILTIN_MACROS)
	TPP_FEAT_CPP_BUILTIN_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BUILTIN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EXCLAIM)
	TPP_FEAT_CPP_EXCLAIM,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BLANK)
	TPP_FEAT_CPP_BLANK,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BLANK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIGIT_LINE)
	TPP_FEAT_CPP_DIGIT_LINE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIGIT_LINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_LINE)
	TPP_FEAT_CPP_LINE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_LINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE)
	TPP_FEAT_CPP_INCLUDE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE_NEXT)
	TPP_FEAT_CPP_INCLUDE_NEXT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE_NEXT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IMPORT)
	TPP_FEAT_CPP_IMPORT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IMPORT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IF_ELSE_ENDIF)
	TPP_FEAT_CPP_IF_ELSE_ENDIF,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IF_ELSE_ENDIF) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DEFINE)
	TPP_FEAT_CPP_DEFINE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DEFINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ASSERT)
	TPP_FEAT_CPP_ASSERT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ASSERT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ERROR)
	TPP_FEAT_CPP_ERROR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ERROR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_WARNING)
	TPP_FEAT_CPP_WARNING,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_WARNING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IDENT_SCCS)
	TPP_FEAT_CPP_IDENT_SCCS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IDENT_SCCS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_PRAGMA)
	TPP_FEAT_CPP_PRAGMA,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_PRAGMA) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EMBED)
	TPP_FEAT_CPP_EMBED,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EMBED) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO__Pragma)
	TPP_FEAT_MACRO__Pragma,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO__Pragma) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___pragma)
	TPP_FEAT_MACRO___pragma,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___pragma) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_attribute)
	TPP_FEAT_CLANG_MACRO___has_attribute,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_builtin)
	TPP_FEAT_CLANG_MACRO___has_builtin,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_builtin) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute)
	TPP_FEAT_CLANG_MACRO___has_cpp_attribute,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute)
	TPP_FEAT_CLANG_MACRO___has_declspec_attribute,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_extension)
	TPP_FEAT_CLANG_MACRO___has_extension,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_extension) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_feature)
	TPP_FEAT_CLANG_MACRO___has_feature,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_feature) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_c_attribute)
	TPP_FEAT_CLANG_MACRO___has_c_attribute,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_c_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES)
	TPP_FEAT_CLANG_EXTENSIONS_ARE_FEATURES,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_identifier)
	TPP_FEAT_MACRO___is_identifier,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_identifier) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_deprecated)
	TPP_FEAT_MACRO___is_deprecated,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_deprecated) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_poisoned)
	TPP_FEAT_MACRO___is_poisoned,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_poisoned) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_extension)
	TPP_FEAT_MACRO___has_extension,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_extension) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_extension)
	TPP_FEAT_MACRO___has_known_extension,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_extension) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_warning)
	TPP_FEAT_MACRO___has_warning,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_warning) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_warning)
	TPP_FEAT_MACRO___has_known_warning,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_warning) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include)
	TPP_FEAT_MACRO___has_include,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include_next)
	TPP_FEAT_MACRO___has_include_next,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include_next) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_embed)
	TPP_FEAT_MACRO___has_embed,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_embed) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE__)
	TPP_FEAT_MACRO___FILE__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___LINE__)
	TPP_FEAT_MACRO___LINE__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___LINE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIME__)
	TPP_FEAT_MACRO___TIME__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIME__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___DATE__)
	TPP_FEAT_MACRO___DATE__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___DATE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COLUMN__)
	TPP_FEAT_MACRO___COLUMN__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COLUMN__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___BASE_FILE__)
	TPP_FEAT_MACRO___BASE_FILE__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___BASE_FILE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE_NAME__)
	TPP_FEAT_MACRO___FILE_NAME__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE_NAME__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_LEVEL__)
	TPP_FEAT_MACRO___INCLUDE_LEVEL__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_LEVEL__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_DEPTH__)
	TPP_FEAT_MACRO___INCLUDE_DEPTH__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_DEPTH__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COUNTER__)
	TPP_FEAT_MACRO___COUNTER__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COUNTER__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIMESTAMP__)
	TPP_FEAT_MACRO___TIMESTAMP__,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIMESTAMP__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_DATE_MACROS)
	TPP_FEAT_NUMERIC_DATE_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_DATE_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_TIME_MACROS)
	TPP_FEAT_NUMERIC_TIME_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_TIME_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EVAL)
	TPP_FEAT_MACRO___TPP_EVAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EVAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EXEC)
	TPP_FEAT_MACRO___TPP_EXEC,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EXEC) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_UNIQUE)
	TPP_FEAT_MACRO___TPP_UNIQUE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_UNIQUE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_LOAD_FILE)
	TPP_FEAT_MACRO___TPP_LOAD_FILE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_LOAD_FILE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNTER)
	TPP_FEAT_MACRO___TPP_COUNTER,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNTER) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_RANDOM)
	TPP_FEAT_MACRO___TPP_RANDOM,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_RANDOM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE)
	TPP_FEAT_MACRO___TPP_STR_DECOMPILE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_PACK)
	TPP_FEAT_MACRO___TPP_STR_PACK,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_PACK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SUBSTR)
	TPP_FEAT_MACRO___TPP_STR_SUBSTR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SUBSTR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SIZE)
	TPP_FEAT_MACRO___TPP_STR_SIZE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SIZE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS)
	TPP_FEAT_MACRO___TPP_COUNT_TOKENS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_IDENTIFIER)
	TPP_FEAT_MACRO___TPP_IDENTIFIER,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_IDENTIFIER) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS)
	TPP_FEAT_ALTERNATIVE_MACRO_PARENTHESIS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
	TPP_FEAT_MACRO_ARGUMENT_WHITESPACE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_RECURSION)
	TPP_FEAT_MACRO_RECURSION,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_RECURSION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TRADITIONAL_MACROS)
	TPP_FEAT_TRADITIONAL_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TRADITIONAL_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_NAMED_VARARGS_IN_MACROS)
	TPP_FEAT_NAMED_VARARGS_IN_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_NAMED_VARARGS_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_ARGS_IN_MACROS)
	TPP_FEAT_VA_ARGS_IN_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_ARGS_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_COMMA_IN_MACROS)
	TPP_FEAT_VA_COMMA_IN_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_COMMA_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_OPT_IN_MACROS)
	TPP_FEAT_VA_OPT_IN_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_OPT_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_NARGS_IN_MACROS)
	TPP_FEAT_VA_NARGS_IN_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_NARGS_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS)
	TPP_FEAT_VA_GLUE_COMMA_IN_MACROS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT)
	TPP_FEAT_STRINGIZE_MACRO_ARGUMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT)
	TPP_FEAT_CHARIZE_MACRO_ARGUMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT)
	TPP_FEAT_DONT_EXPAND_MACRO_ARGUMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_GLUE_MACRO_ARGUMENT)
	TPP_FEAT_GLUE_MACRO_ARGUMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_GLUE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_PUSH_MACRO)
	TPP_FEAT_PRAGMA_PUSH_MACRO,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_PUSH_MACRO) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ONCE)
	TPP_FEAT_PRAGMA_ONCE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ONCE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_DEPRECATED)
	TPP_FEAT_PRAGMA_DEPRECATED,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_DEPRECATED) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_EXTENSION)
	TPP_FEAT_PRAGMA_EXTENSION,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_EXTENSION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_WARNING)
	TPP_FEAT_PRAGMA_WARNING,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_WARNING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE)
	TPP_FEAT_PRAGMA_MESSAGE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ERROR)
	TPP_FEAT_PRAGMA_ERROR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ERROR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_REGION)
	TPP_FEAT_PRAGMA_REGION,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_REGION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_EXEC)
	TPP_FEAT_PRAGMA_TPP_EXEC,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_EXEC) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS)
	TPP_FEAT_PRAGMA_TPP_SET_KEYWORD_FLAGS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_POISON)
	TPP_FEAT_PRAGMA_GCC_POISON,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_POISON) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_WARNING)
	TPP_FEAT_PRAGMA_GCC_WARNING,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_WARNING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_ERROR)
	TPP_FEAT_PRAGMA_GCC_ERROR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_ERROR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER)
	TPP_FEAT_PRAGMA_GCC_SYSTEM_HEADER,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC)
	TPP_FEAT_PRAGMA_GCC_DIAGNOSTIC,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
	TPP_FEAT_PRAGMA_GCC_DEPENDENCY,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS)
	TPP_FEAT_TRIGRAPHS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_DIGRAPHS)
	TPP_FEAT_DIGRAPHS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_DIGRAPHS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LF)
	TPP_FEAT_TOK_LF,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LF) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SPACE)
	TPP_FEAT_TOK_SPACE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COMMENT)
	TPP_FEAT_TOK_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_COMMENT)
	TPP_FEAT_TOK_CXX_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_COMMENT)
	TPP_FEAT_TOK_C_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_COMMENT)
	TPP_FEAT_TOK_PASCAL_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_BRACE_COMMENT)
	TPP_FEAT_TOK_PASCAL_BRACE_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_BRACE_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HTML_COMMENT)
	TPP_FEAT_TOK_HTML_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HTML_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SQL_COMMENT)
	TPP_FEAT_TOK_SQL_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SQL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_COMMENT)
	TPP_FEAT_TOK_AT_AT_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SHELL_COMMENT)
	TPP_FEAT_TOK_SHELL_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SHELL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_COMMENT)
	TPP_FEAT_TOK_SLASH_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_COMMENT)
	TPP_FEAT_TOK_AT_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SHELL_COMMENT)
	TPP_FEAT_TOK_SOL_SHELL_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SHELL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SLASH_COMMENT)
	TPP_FEAT_TOK_SOL_SLASH_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SLASH_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_AT_COMMENT)
	TPP_FEAT_TOK_SOL_AT_COMMENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_AT_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOLLAR)
	TPP_FEAT_TOK_DOLLAR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOLLAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE)
	TPP_FEAT_THOUSANDS_SEPARATOR_UNDERSCORE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK)
	TPP_FEAT_THOUSANDS_SEPARATOR_SINGLETICK,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_INT)
	TPP_FEAT_TOK_C_INT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_INT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_HEX)
	TPP_FEAT_TOK_PASCAL_HEX,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_HEX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_FLOAT)
	TPP_FEAT_TOK_C_FLOAT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_FLOAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_SMART_FLOAT_TOKENS)
	TPP_FEAT_SMART_FLOAT_TOKENS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_SMART_FLOAT_TOKENS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_CHAR)
	TPP_FEAT_TOK_C_CHAR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_CHAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_STRING)
	TPP_FEAT_TOK_C_STRING,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_STRING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL)
	TPP_FEAT_TOK_CXX_RAW_STRING_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL)
	TPP_FEAT_TOK_CXX_WIDE_STRING_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL)
	TPP_FEAT_TOK_CXX_UTF8_STRING_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL)
	TPP_FEAT_TOK_CXX_UTF16_STRING_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL)
	TPP_FEAT_TOK_CXX_UTF32_STRING_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL)
	TPP_FEAT_TOK_CXX_RAW_CHAR_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL)
	TPP_FEAT_TOK_CXX_WIDE_CHAR_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL)
	TPP_FEAT_TOK_CXX_UTF8_CHAR_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL)
	TPP_FEAT_TOK_CXX_UTF16_CHAR_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL)
	TPP_FEAT_TOK_CXX_UTF32_CHAR_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_STRING_LITERAL)
	TPP_FEAT_TOK_RAW_STRING_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_CHAR_LITERAL)
	TPP_FEAT_TOK_RAW_CHAR_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_STRING_LITERAL)
	TPP_FEAT_TOK_BLOCK_STRING_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_CHAR_LITERAL)
	TPP_FEAT_TOK_BLOCK_CHAR_LITERAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_STRING_ALLOW_MULTILINE)
	TPP_FEAT_STRING_ALLOW_MULTILINE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_STRING_ALLOW_MULTILINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_STRING_AUTO_CONCAT)
	TPP_FEAT_STRING_AUTO_CONCAT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_STRING_AUTO_CONCAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EXCLAIM)
	TPP_FEAT_TOK_EXCLAIM_EXCLAIM,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL)
	TPP_FEAT_TOK_EXCLAIM_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL)
	TPP_FEAT_TOK_EXCLAIM_EQUAL_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_POUND_POUND)
	TPP_FEAT_TOK_POUND_POUND,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_POUND_POUND) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT)
	TPP_FEAT_TOK_PERCENT_PERCENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL)
	TPP_FEAT_TOK_PERCENT_PERCENT_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_EQUAL)
	TPP_FEAT_TOK_PERCENT_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_AMP)
	TPP_FEAT_TOK_AMP_AMP,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_AMP) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_EQUAL)
	TPP_FEAT_TOK_AMP_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR)
	TPP_FEAT_TOK_STAR_STAR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR_EQUAL)
	TPP_FEAT_TOK_STAR_STAR_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_DOT)
	TPP_FEAT_TOK_STAR_DOT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_DOT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_LANGLE_MINUS)
	TPP_FEAT_TOK_STAR_LANGLE_MINUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_EQUAL)
	TPP_FEAT_TOK_STAR_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_PLUS)
	TPP_FEAT_TOK_PLUS_PLUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_PLUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_EQUAL)
	TPP_FEAT_TOK_PLUS_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_MINUS)
	TPP_FEAT_TOK_MINUS_MINUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE)
	TPP_FEAT_TOK_MINUS_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE)
	TPP_FEAT_TOK_MINUS_LANGLE_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE)
	TPP_FEAT_TOK_MINUS_LANGLE_LANGLE_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_EQUAL)
	TPP_FEAT_TOK_MINUS_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE)
	TPP_FEAT_TOK_MINUS_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_STAR)
	TPP_FEAT_TOK_MINUS_RANGLE_STAR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE)
	TPP_FEAT_TOK_MINUS_RANGLE_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE)
	TPP_FEAT_TOK_MINUS_RANGLE_RANGLE_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_STAR)
	TPP_FEAT_TOK_DOT_STAR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT)
	TPP_FEAT_TOK_DOT_DOT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT_DOT)
	TPP_FEAT_TOK_DOT_DOT_DOT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT_DOT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH)
	TPP_FEAT_TOK_SLASH_SLASH,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH_EQUAL)
	TPP_FEAT_TOK_SLASH_SLASH_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_EQUAL)
	TPP_FEAT_TOK_SLASH_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_COLON)
	TPP_FEAT_TOK_COLON_COLON,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_COLON) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_EQUAL)
	TPP_FEAT_TOK_COLON_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS)
	TPP_FEAT_TOK_LANGLE_MINUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_LANGLE)
	TPP_FEAT_TOK_LANGLE_MINUS_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_RANGLE)
	TPP_FEAT_TOK_LANGLE_MINUS_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE)
	TPP_FEAT_TOK_LANGLE_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_MINUS)
	TPP_FEAT_TOK_LANGLE_LANGLE_MINUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE)
	TPP_FEAT_TOK_LANGLE_LANGLE_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS)
	TPP_FEAT_TOK_LANGLE_LANGLE_LANGLE_MINUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
	TPP_FEAT_TOK_LANGLE_LANGLE_LANGLE_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL)
	TPP_FEAT_TOK_LANGLE_LANGLE_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL)
	TPP_FEAT_TOK_LANGLE_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE)
	TPP_FEAT_TOK_LANGLE_EQUAL_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE)
	TPP_FEAT_TOK_LANGLE_EQUAL_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_RANGLE)
	TPP_FEAT_TOK_LANGLE_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EXCLAIM)
	TPP_FEAT_TOK_EQUAL_EXCLAIM,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT)
	TPP_FEAT_TOK_EQUAL_PERCENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT)
	TPP_FEAT_TOK_EQUAL_PERCENT_PERCENT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AMP)
	TPP_FEAT_TOK_EQUAL_AMP,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AMP) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR)
	TPP_FEAT_TOK_EQUAL_STAR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR_STAR)
	TPP_FEAT_TOK_EQUAL_STAR_STAR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PLUS)
	TPP_FEAT_TOK_EQUAL_PLUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PLUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_MINUS)
	TPP_FEAT_TOK_EQUAL_MINUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH)
	TPP_FEAT_TOK_EQUAL_SLASH,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH_SLASH)
	TPP_FEAT_TOK_EQUAL_SLASH_SLASH,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH_SLASH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_COLON)
	TPP_FEAT_TOK_EQUAL_COLON,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_COLON) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE)
	TPP_FEAT_TOK_EQUAL_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE)
	TPP_FEAT_TOK_EQUAL_LANGLE_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE)
	TPP_FEAT_TOK_EQUAL_LANGLE_LANGLE_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL)
	TPP_FEAT_TOK_EQUAL_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM)
	TPP_FEAT_TOK_EQUAL_EQUAL_EXCLAIM,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL)
	TPP_FEAT_TOK_EQUAL_EQUAL_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE)
	TPP_FEAT_TOK_EQUAL_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE)
	TPP_FEAT_TOK_EQUAL_RANGLE_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
	TPP_FEAT_TOK_EQUAL_RANGLE_RANGLE_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_QMARK)
	TPP_FEAT_TOK_EQUAL_QMARK,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_QMARK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT)
	TPP_FEAT_TOK_EQUAL_AT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT_AT)
	TPP_FEAT_TOK_EQUAL_AT_AT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT_AT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_HAT)
	TPP_FEAT_TOK_EQUAL_HAT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_HAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PIPE)
	TPP_FEAT_TOK_EQUAL_PIPE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PIPE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_TILDE)
	TPP_FEAT_TOK_EQUAL_TILDE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_TILDE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS)
	TPP_FEAT_TOK_RANGLE_MINUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_LANGLE)
	TPP_FEAT_TOK_RANGLE_MINUS_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_RANGLE)
	TPP_FEAT_TOK_RANGLE_MINUS_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_LANGLE)
	TPP_FEAT_TOK_RANGLE_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL)
	TPP_FEAT_TOK_RANGLE_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE)
	TPP_FEAT_TOK_RANGLE_EQUAL_LANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE)
	TPP_FEAT_TOK_RANGLE_EQUAL_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE)
	TPP_FEAT_TOK_RANGLE_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_MINUS)
	TPP_FEAT_TOK_RANGLE_RANGLE_MINUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL)
	TPP_FEAT_TOK_RANGLE_RANGLE_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE)
	TPP_FEAT_TOK_RANGLE_RANGLE_RANGLE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS)
	TPP_FEAT_TOK_RANGLE_RANGLE_RANGLE_MINUS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
	TPP_FEAT_TOK_RANGLE_RANGLE_RANGLE_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_EQUAL)
	TPP_FEAT_TOK_QMARK_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_QMARK)
	TPP_FEAT_TOK_QMARK_QMARK,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_QMARK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_EQUAL)
	TPP_FEAT_TOK_AT_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT)
	TPP_FEAT_TOK_AT_AT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_EQUAL)
	TPP_FEAT_TOK_AT_AT_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_EQUAL)
	TPP_FEAT_TOK_HAT_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_HAT)
	TPP_FEAT_TOK_HAT_HAT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_HAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_EQUAL)
	TPP_FEAT_TOK_PIPE_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_PIPE)
	TPP_FEAT_TOK_PIPE_PIPE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_PIPE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_EQUAL)
	TPP_FEAT_TOK_TILDE_EQUAL,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_TILDE)
	TPP_FEAT_TOK_TILDE_TILDE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_TILDE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_DEFINED)
	TPP_FEAT_BUILTIN_EXPR_DEFINED,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_DEFINED) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR)
	TPP_FEAT_DONT_EXPAND_DEFINED_IN_EXPR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_STRINGS)
	TPP_FEAT_BUILTIN_EXPR_STRINGS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_STRINGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FLOATS)
	TPP_FEAT_BUILTIN_EXPR_FLOATS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FLOATS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)
	TPP_FEAT_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS)
	TPP_FEAT_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR)
	TPP_FEAT_BUILTIN_EXPR_LOGICAL_XOR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS)
	TPP_FEAT_BUILTIN_EXPR_BINARY_LITERALS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS)
	TPP_FEAT_BUILTIN_EXPR_OCTAL_LITERALS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX)
	TPP_FEAT_LEXER_DECODEINT_FIXED_TYPE_SUFFIX,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX)
	TPP_FEAT_LEXER_DECODEINT_SIZE_TYPE_SUFFIX,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX)
	TPP_FEAT_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX)
	TPP_FEAT_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX)
	TPP_FEAT_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX)
	TPP_FEAT_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS)
	TPP_FEAT_BUILTIN_EXPR_CHARACTER_LITERALS,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_EXTERN_C_FOR_SYSHDR)
	TPP_FEAT_EXTERN_C_FOR_SYSHDR,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_EXTERN_C_FOR_SYSHDR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH)
	TPP_FEAT_SEARCH_SYSTEM_INCLUDE_PATH,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE)
	TPP_FEAT_INCLUDE_RELATIVE_TO_EVERY_FILE,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION)
	TPP_FEAT_PRAGMA_MESSAGE_PRINTS_LOCATION,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED)
	TPP_FEAT_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED,
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED) */
	TPP_FEAT_COUNT
} tpp_feature_id;

typedef union tpp_features {
	struct {
#if TPP_CONF_IS_FEAT(TPP_HAVE_BSE)
		unsigned int TPP_INTERNAL(tff_BSE): 1;
#define _tpp_lexer_has_BSE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BSE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BSE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BSE_WHITESPACE)
		unsigned int TPP_INTERNAL(tff_BSE_WHITESPACE): 1;
#define _tpp_lexer_has_BSE_WHITESPACE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BSE_WHITESPACE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BSE_WHITESPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
		unsigned int TPP_INTERNAL(tff_ESCAPE_IN_IDENTIFIERS): 1;
#define _tpp_lexer_has_ESCAPE_IN_IDENTIFIERS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_ESCAPE_IN_IDENTIFIERS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_E_IN_STRINGS)
		unsigned int TPP_INTERNAL(tff_ESCAPE_E_IN_STRINGS): 1;
#define _tpp_lexer_has_ESCAPE_E_IN_STRINGS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_ESCAPE_E_IN_STRINGS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_E_IN_STRINGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_S_IN_STRINGS)
		unsigned int TPP_INTERNAL(tff_ESCAPE_S_IN_STRINGS): 1;
#define _tpp_lexer_has_ESCAPE_S_IN_STRINGS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_ESCAPE_S_IN_STRINGS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ESCAPE_S_IN_STRINGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIRECTIVES)
		unsigned int TPP_INTERNAL(tff_CPP_DIRECTIVES): 1;
#define _tpp_lexer_has_CPP_DIRECTIVES(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_DIRECTIVES)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIRECTIVES) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_MACROS)
		unsigned int TPP_INTERNAL(tff_CPP_MACROS): 1;
#define _tpp_lexer_has_CPP_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MAGIC_WHITESPACE)
		unsigned int TPP_INTERNAL(tff_MAGIC_WHITESPACE): 1;
#define _tpp_lexer_has_MAGIC_WHITESPACE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MAGIC_WHITESPACE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MAGIC_WHITESPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BUILTIN_MACROS)
		unsigned int TPP_INTERNAL(tff_CPP_BUILTIN_MACROS): 1;
#define _tpp_lexer_has_CPP_BUILTIN_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_BUILTIN_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BUILTIN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EXCLAIM)
		unsigned int TPP_INTERNAL(tff_CPP_EXCLAIM): 1;
#define _tpp_lexer_has_CPP_EXCLAIM(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_EXCLAIM)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BLANK)
		unsigned int TPP_INTERNAL(tff_CPP_BLANK): 1;
#define _tpp_lexer_has_CPP_BLANK(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_BLANK)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_BLANK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIGIT_LINE)
		unsigned int TPP_INTERNAL(tff_CPP_DIGIT_LINE): 1;
#define _tpp_lexer_has_CPP_DIGIT_LINE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_DIGIT_LINE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DIGIT_LINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_LINE)
		unsigned int TPP_INTERNAL(tff_CPP_LINE): 1;
#define _tpp_lexer_has_CPP_LINE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_LINE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_LINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE)
		unsigned int TPP_INTERNAL(tff_CPP_INCLUDE): 1;
#define _tpp_lexer_has_CPP_INCLUDE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_INCLUDE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE_NEXT)
		unsigned int TPP_INTERNAL(tff_CPP_INCLUDE_NEXT): 1;
#define _tpp_lexer_has_CPP_INCLUDE_NEXT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_INCLUDE_NEXT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_INCLUDE_NEXT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IMPORT)
		unsigned int TPP_INTERNAL(tff_CPP_IMPORT): 1;
#define _tpp_lexer_has_CPP_IMPORT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_IMPORT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IMPORT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IF_ELSE_ENDIF)
		unsigned int TPP_INTERNAL(tff_CPP_IF_ELSE_ENDIF): 1;
#define _tpp_lexer_has_CPP_IF_ELSE_ENDIF(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_IF_ELSE_ENDIF)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IF_ELSE_ENDIF) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DEFINE)
		unsigned int TPP_INTERNAL(tff_CPP_DEFINE): 1;
#define _tpp_lexer_has_CPP_DEFINE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_DEFINE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_DEFINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ASSERT)
		unsigned int TPP_INTERNAL(tff_CPP_ASSERT): 1;
#define _tpp_lexer_has_CPP_ASSERT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_ASSERT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ASSERT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ERROR)
		unsigned int TPP_INTERNAL(tff_CPP_ERROR): 1;
#define _tpp_lexer_has_CPP_ERROR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_ERROR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_ERROR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_WARNING)
		unsigned int TPP_INTERNAL(tff_CPP_WARNING): 1;
#define _tpp_lexer_has_CPP_WARNING(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_WARNING)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_WARNING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IDENT_SCCS)
		unsigned int TPP_INTERNAL(tff_CPP_IDENT_SCCS): 1;
#define _tpp_lexer_has_CPP_IDENT_SCCS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_IDENT_SCCS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_IDENT_SCCS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_PRAGMA)
		unsigned int TPP_INTERNAL(tff_CPP_PRAGMA): 1;
#define _tpp_lexer_has_CPP_PRAGMA(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_PRAGMA)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_PRAGMA) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EMBED)
		unsigned int TPP_INTERNAL(tff_CPP_EMBED): 1;
#define _tpp_lexer_has_CPP_EMBED(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CPP_EMBED)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CPP_EMBED) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO__Pragma)
		unsigned int TPP_INTERNAL(tff_MACRO__Pragma): 1;
#define _tpp_lexer_has_MACRO__Pragma(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO__Pragma)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO__Pragma) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___pragma)
		unsigned int TPP_INTERNAL(tff_MACRO___pragma): 1;
#define _tpp_lexer_has_MACRO___pragma(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___pragma)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___pragma) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_attribute)
		unsigned int TPP_INTERNAL(tff_CLANG_MACRO___has_attribute): 1;
#define _tpp_lexer_has_CLANG_MACRO___has_attribute(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CLANG_MACRO___has_attribute)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_builtin)
		unsigned int TPP_INTERNAL(tff_CLANG_MACRO___has_builtin): 1;
#define _tpp_lexer_has_CLANG_MACRO___has_builtin(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CLANG_MACRO___has_builtin)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_builtin) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute)
		unsigned int TPP_INTERNAL(tff_CLANG_MACRO___has_cpp_attribute): 1;
#define _tpp_lexer_has_CLANG_MACRO___has_cpp_attribute(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CLANG_MACRO___has_cpp_attribute)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute)
		unsigned int TPP_INTERNAL(tff_CLANG_MACRO___has_declspec_attribute): 1;
#define _tpp_lexer_has_CLANG_MACRO___has_declspec_attribute(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CLANG_MACRO___has_declspec_attribute)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_extension)
		unsigned int TPP_INTERNAL(tff_CLANG_MACRO___has_extension): 1;
#define _tpp_lexer_has_CLANG_MACRO___has_extension(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CLANG_MACRO___has_extension)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_extension) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_feature)
		unsigned int TPP_INTERNAL(tff_CLANG_MACRO___has_feature): 1;
#define _tpp_lexer_has_CLANG_MACRO___has_feature(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CLANG_MACRO___has_feature)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_feature) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_c_attribute)
		unsigned int TPP_INTERNAL(tff_CLANG_MACRO___has_c_attribute): 1;
#define _tpp_lexer_has_CLANG_MACRO___has_c_attribute(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CLANG_MACRO___has_c_attribute)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_MACRO___has_c_attribute) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES)
		unsigned int TPP_INTERNAL(tff_CLANG_EXTENSIONS_ARE_FEATURES): 1;
#define _tpp_lexer_has_CLANG_EXTENSIONS_ARE_FEATURES(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CLANG_EXTENSIONS_ARE_FEATURES)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_identifier)
		unsigned int TPP_INTERNAL(tff_MACRO___is_identifier): 1;
#define _tpp_lexer_has_MACRO___is_identifier(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___is_identifier)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_identifier) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_deprecated)
		unsigned int TPP_INTERNAL(tff_MACRO___is_deprecated): 1;
#define _tpp_lexer_has_MACRO___is_deprecated(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___is_deprecated)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_deprecated) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_poisoned)
		unsigned int TPP_INTERNAL(tff_MACRO___is_poisoned): 1;
#define _tpp_lexer_has_MACRO___is_poisoned(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___is_poisoned)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___is_poisoned) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_extension)
		unsigned int TPP_INTERNAL(tff_MACRO___has_extension): 1;
#define _tpp_lexer_has_MACRO___has_extension(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___has_extension)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_extension) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_extension)
		unsigned int TPP_INTERNAL(tff_MACRO___has_known_extension): 1;
#define _tpp_lexer_has_MACRO___has_known_extension(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___has_known_extension)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_extension) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_warning)
		unsigned int TPP_INTERNAL(tff_MACRO___has_warning): 1;
#define _tpp_lexer_has_MACRO___has_warning(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___has_warning)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_warning) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_warning)
		unsigned int TPP_INTERNAL(tff_MACRO___has_known_warning): 1;
#define _tpp_lexer_has_MACRO___has_known_warning(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___has_known_warning)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_known_warning) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include)
		unsigned int TPP_INTERNAL(tff_MACRO___has_include): 1;
#define _tpp_lexer_has_MACRO___has_include(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___has_include)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include_next)
		unsigned int TPP_INTERNAL(tff_MACRO___has_include_next): 1;
#define _tpp_lexer_has_MACRO___has_include_next(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___has_include_next)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_include_next) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_embed)
		unsigned int TPP_INTERNAL(tff_MACRO___has_embed): 1;
#define _tpp_lexer_has_MACRO___has_embed(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___has_embed)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___has_embed) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE__)
		unsigned int TPP_INTERNAL(tff_MACRO___FILE__): 1;
#define _tpp_lexer_has_MACRO___FILE__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___FILE__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___LINE__)
		unsigned int TPP_INTERNAL(tff_MACRO___LINE__): 1;
#define _tpp_lexer_has_MACRO___LINE__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___LINE__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___LINE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIME__)
		unsigned int TPP_INTERNAL(tff_MACRO___TIME__): 1;
#define _tpp_lexer_has_MACRO___TIME__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TIME__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIME__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___DATE__)
		unsigned int TPP_INTERNAL(tff_MACRO___DATE__): 1;
#define _tpp_lexer_has_MACRO___DATE__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___DATE__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___DATE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COLUMN__)
		unsigned int TPP_INTERNAL(tff_MACRO___COLUMN__): 1;
#define _tpp_lexer_has_MACRO___COLUMN__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___COLUMN__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COLUMN__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___BASE_FILE__)
		unsigned int TPP_INTERNAL(tff_MACRO___BASE_FILE__): 1;
#define _tpp_lexer_has_MACRO___BASE_FILE__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___BASE_FILE__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___BASE_FILE__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE_NAME__)
		unsigned int TPP_INTERNAL(tff_MACRO___FILE_NAME__): 1;
#define _tpp_lexer_has_MACRO___FILE_NAME__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___FILE_NAME__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___FILE_NAME__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_LEVEL__)
		unsigned int TPP_INTERNAL(tff_MACRO___INCLUDE_LEVEL__): 1;
#define _tpp_lexer_has_MACRO___INCLUDE_LEVEL__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___INCLUDE_LEVEL__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_LEVEL__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_DEPTH__)
		unsigned int TPP_INTERNAL(tff_MACRO___INCLUDE_DEPTH__): 1;
#define _tpp_lexer_has_MACRO___INCLUDE_DEPTH__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___INCLUDE_DEPTH__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___INCLUDE_DEPTH__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COUNTER__)
		unsigned int TPP_INTERNAL(tff_MACRO___COUNTER__): 1;
#define _tpp_lexer_has_MACRO___COUNTER__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___COUNTER__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___COUNTER__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIMESTAMP__)
		unsigned int TPP_INTERNAL(tff_MACRO___TIMESTAMP__): 1;
#define _tpp_lexer_has_MACRO___TIMESTAMP__(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TIMESTAMP__)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TIMESTAMP__) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_DATE_MACROS)
		unsigned int TPP_INTERNAL(tff_NUMERIC_DATE_MACROS): 1;
#define _tpp_lexer_has_NUMERIC_DATE_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_NUMERIC_DATE_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_DATE_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_TIME_MACROS)
		unsigned int TPP_INTERNAL(tff_NUMERIC_TIME_MACROS): 1;
#define _tpp_lexer_has_NUMERIC_TIME_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_NUMERIC_TIME_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_NUMERIC_TIME_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EVAL)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_EVAL): 1;
#define _tpp_lexer_has_MACRO___TPP_EVAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_EVAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EVAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EXEC)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_EXEC): 1;
#define _tpp_lexer_has_MACRO___TPP_EXEC(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_EXEC)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_EXEC) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_UNIQUE)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_UNIQUE): 1;
#define _tpp_lexer_has_MACRO___TPP_UNIQUE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_UNIQUE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_UNIQUE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_LOAD_FILE)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_LOAD_FILE): 1;
#define _tpp_lexer_has_MACRO___TPP_LOAD_FILE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_LOAD_FILE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_LOAD_FILE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNTER)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_COUNTER): 1;
#define _tpp_lexer_has_MACRO___TPP_COUNTER(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_COUNTER)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNTER) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_RANDOM)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_RANDOM): 1;
#define _tpp_lexer_has_MACRO___TPP_RANDOM(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_RANDOM)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_RANDOM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_STR_DECOMPILE): 1;
#define _tpp_lexer_has_MACRO___TPP_STR_DECOMPILE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_STR_DECOMPILE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_PACK)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_STR_PACK): 1;
#define _tpp_lexer_has_MACRO___TPP_STR_PACK(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_STR_PACK)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_PACK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SUBSTR)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_STR_SUBSTR): 1;
#define _tpp_lexer_has_MACRO___TPP_STR_SUBSTR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_STR_SUBSTR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SUBSTR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SIZE)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_STR_SIZE): 1;
#define _tpp_lexer_has_MACRO___TPP_STR_SIZE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_STR_SIZE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_STR_SIZE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_COUNT_TOKENS): 1;
#define _tpp_lexer_has_MACRO___TPP_COUNT_TOKENS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_COUNT_TOKENS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_IDENTIFIER)
		unsigned int TPP_INTERNAL(tff_MACRO___TPP_IDENTIFIER): 1;
#define _tpp_lexer_has_MACRO___TPP_IDENTIFIER(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO___TPP_IDENTIFIER)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO___TPP_IDENTIFIER) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS)
		unsigned int TPP_INTERNAL(tff_ALTERNATIVE_MACRO_PARENTHESIS): 1;
#define _tpp_lexer_has_ALTERNATIVE_MACRO_PARENTHESIS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_ALTERNATIVE_MACRO_PARENTHESIS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
		unsigned int TPP_INTERNAL(tff_MACRO_ARGUMENT_WHITESPACE): 1;
#define _tpp_lexer_has_MACRO_ARGUMENT_WHITESPACE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO_ARGUMENT_WHITESPACE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_RECURSION)
		unsigned int TPP_INTERNAL(tff_MACRO_RECURSION): 1;
#define _tpp_lexer_has_MACRO_RECURSION(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_MACRO_RECURSION)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_MACRO_RECURSION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TRADITIONAL_MACROS)
		unsigned int TPP_INTERNAL(tff_TRADITIONAL_MACROS): 1;
#define _tpp_lexer_has_TRADITIONAL_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TRADITIONAL_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TRADITIONAL_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_NAMED_VARARGS_IN_MACROS)
		unsigned int TPP_INTERNAL(tff_NAMED_VARARGS_IN_MACROS): 1;
#define _tpp_lexer_has_NAMED_VARARGS_IN_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_NAMED_VARARGS_IN_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_NAMED_VARARGS_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_ARGS_IN_MACROS)
		unsigned int TPP_INTERNAL(tff_VA_ARGS_IN_MACROS): 1;
#define _tpp_lexer_has_VA_ARGS_IN_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_VA_ARGS_IN_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_ARGS_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_COMMA_IN_MACROS)
		unsigned int TPP_INTERNAL(tff_VA_COMMA_IN_MACROS): 1;
#define _tpp_lexer_has_VA_COMMA_IN_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_VA_COMMA_IN_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_COMMA_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_OPT_IN_MACROS)
		unsigned int TPP_INTERNAL(tff_VA_OPT_IN_MACROS): 1;
#define _tpp_lexer_has_VA_OPT_IN_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_VA_OPT_IN_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_OPT_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_NARGS_IN_MACROS)
		unsigned int TPP_INTERNAL(tff_VA_NARGS_IN_MACROS): 1;
#define _tpp_lexer_has_VA_NARGS_IN_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_VA_NARGS_IN_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_NARGS_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS)
		unsigned int TPP_INTERNAL(tff_VA_GLUE_COMMA_IN_MACROS): 1;
#define _tpp_lexer_has_VA_GLUE_COMMA_IN_MACROS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_VA_GLUE_COMMA_IN_MACROS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT)
		unsigned int TPP_INTERNAL(tff_STRINGIZE_MACRO_ARGUMENT): 1;
#define _tpp_lexer_has_STRINGIZE_MACRO_ARGUMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_STRINGIZE_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT)
		unsigned int TPP_INTERNAL(tff_CHARIZE_MACRO_ARGUMENT): 1;
#define _tpp_lexer_has_CHARIZE_MACRO_ARGUMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_CHARIZE_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT)
		unsigned int TPP_INTERNAL(tff_DONT_EXPAND_MACRO_ARGUMENT): 1;
#define _tpp_lexer_has_DONT_EXPAND_MACRO_ARGUMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_DONT_EXPAND_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_GLUE_MACRO_ARGUMENT)
		unsigned int TPP_INTERNAL(tff_GLUE_MACRO_ARGUMENT): 1;
#define _tpp_lexer_has_GLUE_MACRO_ARGUMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_GLUE_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_GLUE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_PUSH_MACRO)
		unsigned int TPP_INTERNAL(tff_PRAGMA_PUSH_MACRO): 1;
#define _tpp_lexer_has_PRAGMA_PUSH_MACRO(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_PUSH_MACRO)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_PUSH_MACRO) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ONCE)
		unsigned int TPP_INTERNAL(tff_PRAGMA_ONCE): 1;
#define _tpp_lexer_has_PRAGMA_ONCE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_ONCE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ONCE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_DEPRECATED)
		unsigned int TPP_INTERNAL(tff_PRAGMA_DEPRECATED): 1;
#define _tpp_lexer_has_PRAGMA_DEPRECATED(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_DEPRECATED)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_DEPRECATED) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_EXTENSION)
		unsigned int TPP_INTERNAL(tff_PRAGMA_EXTENSION): 1;
#define _tpp_lexer_has_PRAGMA_EXTENSION(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_EXTENSION)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_EXTENSION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_WARNING)
		unsigned int TPP_INTERNAL(tff_PRAGMA_WARNING): 1;
#define _tpp_lexer_has_PRAGMA_WARNING(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_WARNING)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_WARNING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE)
		unsigned int TPP_INTERNAL(tff_PRAGMA_MESSAGE): 1;
#define _tpp_lexer_has_PRAGMA_MESSAGE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_MESSAGE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ERROR)
		unsigned int TPP_INTERNAL(tff_PRAGMA_ERROR): 1;
#define _tpp_lexer_has_PRAGMA_ERROR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_ERROR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_ERROR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_REGION)
		unsigned int TPP_INTERNAL(tff_PRAGMA_REGION): 1;
#define _tpp_lexer_has_PRAGMA_REGION(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_REGION)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_REGION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_EXEC)
		unsigned int TPP_INTERNAL(tff_PRAGMA_TPP_EXEC): 1;
#define _tpp_lexer_has_PRAGMA_TPP_EXEC(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_TPP_EXEC)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_EXEC) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS)
		unsigned int TPP_INTERNAL(tff_PRAGMA_TPP_SET_KEYWORD_FLAGS): 1;
#define _tpp_lexer_has_PRAGMA_TPP_SET_KEYWORD_FLAGS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_TPP_SET_KEYWORD_FLAGS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_POISON)
		unsigned int TPP_INTERNAL(tff_PRAGMA_GCC_POISON): 1;
#define _tpp_lexer_has_PRAGMA_GCC_POISON(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_GCC_POISON)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_POISON) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_WARNING)
		unsigned int TPP_INTERNAL(tff_PRAGMA_GCC_WARNING): 1;
#define _tpp_lexer_has_PRAGMA_GCC_WARNING(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_GCC_WARNING)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_WARNING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_ERROR)
		unsigned int TPP_INTERNAL(tff_PRAGMA_GCC_ERROR): 1;
#define _tpp_lexer_has_PRAGMA_GCC_ERROR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_GCC_ERROR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_ERROR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER)
		unsigned int TPP_INTERNAL(tff_PRAGMA_GCC_SYSTEM_HEADER): 1;
#define _tpp_lexer_has_PRAGMA_GCC_SYSTEM_HEADER(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_GCC_SYSTEM_HEADER)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC)
		unsigned int TPP_INTERNAL(tff_PRAGMA_GCC_DIAGNOSTIC): 1;
#define _tpp_lexer_has_PRAGMA_GCC_DIAGNOSTIC(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_GCC_DIAGNOSTIC)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
		unsigned int TPP_INTERNAL(tff_PRAGMA_GCC_DEPENDENCY): 1;
#define _tpp_lexer_has_PRAGMA_GCC_DEPENDENCY(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_GCC_DEPENDENCY)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS)
		unsigned int TPP_INTERNAL(tff_TRIGRAPHS): 1;
#define _tpp_lexer_has_TRIGRAPHS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TRIGRAPHS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TRIGRAPHS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_DIGRAPHS)
		unsigned int TPP_INTERNAL(tff_DIGRAPHS): 1;
#define _tpp_lexer_has_DIGRAPHS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_DIGRAPHS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_DIGRAPHS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LF)
		unsigned int TPP_INTERNAL(tff_TOK_LF): 1;
#define _tpp_lexer_has_TOK_LF(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LF)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LF) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SPACE)
		unsigned int TPP_INTERNAL(tff_TOK_SPACE): 1;
#define _tpp_lexer_has_TOK_SPACE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SPACE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SPACE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_COMMENT): 1;
#define _tpp_lexer_has_TOK_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_COMMENT): 1;
#define _tpp_lexer_has_TOK_CXX_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_C_COMMENT): 1;
#define _tpp_lexer_has_TOK_C_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_C_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_PASCAL_COMMENT): 1;
#define _tpp_lexer_has_TOK_PASCAL_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PASCAL_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_BRACE_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_PASCAL_BRACE_COMMENT): 1;
#define _tpp_lexer_has_TOK_PASCAL_BRACE_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PASCAL_BRACE_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_BRACE_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HTML_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_HTML_COMMENT): 1;
#define _tpp_lexer_has_TOK_HTML_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_HTML_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HTML_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SQL_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_SQL_COMMENT): 1;
#define _tpp_lexer_has_TOK_SQL_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SQL_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SQL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_AT_AT_COMMENT): 1;
#define _tpp_lexer_has_TOK_AT_AT_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_AT_AT_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SHELL_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_SHELL_COMMENT): 1;
#define _tpp_lexer_has_TOK_SHELL_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SHELL_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SHELL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_SLASH_COMMENT): 1;
#define _tpp_lexer_has_TOK_SLASH_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SLASH_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_AT_COMMENT): 1;
#define _tpp_lexer_has_TOK_AT_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_AT_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SHELL_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_SOL_SHELL_COMMENT): 1;
#define _tpp_lexer_has_TOK_SOL_SHELL_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SOL_SHELL_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SHELL_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SLASH_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_SOL_SLASH_COMMENT): 1;
#define _tpp_lexer_has_TOK_SOL_SLASH_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SOL_SLASH_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_SLASH_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_AT_COMMENT)
		unsigned int TPP_INTERNAL(tff_TOK_SOL_AT_COMMENT): 1;
#define _tpp_lexer_has_TOK_SOL_AT_COMMENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SOL_AT_COMMENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SOL_AT_COMMENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOLLAR)
		unsigned int TPP_INTERNAL(tff_TOK_DOLLAR): 1;
#define _tpp_lexer_has_TOK_DOLLAR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_DOLLAR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOLLAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE)
		unsigned int TPP_INTERNAL(tff_THOUSANDS_SEPARATOR_UNDERSCORE): 1;
#define _tpp_lexer_has_THOUSANDS_SEPARATOR_UNDERSCORE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_THOUSANDS_SEPARATOR_UNDERSCORE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK)
		unsigned int TPP_INTERNAL(tff_THOUSANDS_SEPARATOR_SINGLETICK): 1;
#define _tpp_lexer_has_THOUSANDS_SEPARATOR_SINGLETICK(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_THOUSANDS_SEPARATOR_SINGLETICK)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_INT)
		unsigned int TPP_INTERNAL(tff_TOK_C_INT): 1;
#define _tpp_lexer_has_TOK_C_INT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_C_INT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_INT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_HEX)
		unsigned int TPP_INTERNAL(tff_TOK_PASCAL_HEX): 1;
#define _tpp_lexer_has_TOK_PASCAL_HEX(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PASCAL_HEX)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PASCAL_HEX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_FLOAT)
		unsigned int TPP_INTERNAL(tff_TOK_C_FLOAT): 1;
#define _tpp_lexer_has_TOK_C_FLOAT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_C_FLOAT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_FLOAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_SMART_FLOAT_TOKENS)
		unsigned int TPP_INTERNAL(tff_SMART_FLOAT_TOKENS): 1;
#define _tpp_lexer_has_SMART_FLOAT_TOKENS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_SMART_FLOAT_TOKENS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_SMART_FLOAT_TOKENS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_CHAR)
		unsigned int TPP_INTERNAL(tff_TOK_C_CHAR): 1;
#define _tpp_lexer_has_TOK_C_CHAR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_C_CHAR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_CHAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_STRING)
		unsigned int TPP_INTERNAL(tff_TOK_C_STRING): 1;
#define _tpp_lexer_has_TOK_C_STRING(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_C_STRING)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_C_STRING) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_RAW_STRING_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_RAW_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_RAW_STRING_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_WIDE_STRING_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_WIDE_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_WIDE_STRING_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_UTF8_STRING_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_UTF8_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_UTF8_STRING_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_UTF16_STRING_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_UTF16_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_UTF16_STRING_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_UTF32_STRING_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_UTF32_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_UTF32_STRING_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_RAW_CHAR_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_RAW_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_RAW_CHAR_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_WIDE_CHAR_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_WIDE_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_WIDE_CHAR_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_UTF8_CHAR_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_UTF8_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_UTF8_CHAR_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_UTF16_CHAR_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_UTF16_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_UTF16_CHAR_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_CXX_UTF32_CHAR_LITERAL): 1;
#define _tpp_lexer_has_TOK_CXX_UTF32_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_CXX_UTF32_CHAR_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_STRING_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_RAW_STRING_LITERAL): 1;
#define _tpp_lexer_has_TOK_RAW_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RAW_STRING_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_CHAR_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_RAW_CHAR_LITERAL): 1;
#define _tpp_lexer_has_TOK_RAW_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RAW_CHAR_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_STRING_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_BLOCK_STRING_LITERAL): 1;
#define _tpp_lexer_has_TOK_BLOCK_STRING_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_BLOCK_STRING_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_STRING_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_CHAR_LITERAL)
		unsigned int TPP_INTERNAL(tff_TOK_BLOCK_CHAR_LITERAL): 1;
#define _tpp_lexer_has_TOK_BLOCK_CHAR_LITERAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_BLOCK_CHAR_LITERAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_BLOCK_CHAR_LITERAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_STRING_ALLOW_MULTILINE)
		unsigned int TPP_INTERNAL(tff_STRING_ALLOW_MULTILINE): 1;
#define _tpp_lexer_has_STRING_ALLOW_MULTILINE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_STRING_ALLOW_MULTILINE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_STRING_ALLOW_MULTILINE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_STRING_AUTO_CONCAT)
		unsigned int TPP_INTERNAL(tff_STRING_AUTO_CONCAT): 1;
#define _tpp_lexer_has_STRING_AUTO_CONCAT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_STRING_AUTO_CONCAT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_STRING_AUTO_CONCAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EXCLAIM)
		unsigned int TPP_INTERNAL(tff_TOK_EXCLAIM_EXCLAIM): 1;
#define _tpp_lexer_has_TOK_EXCLAIM_EXCLAIM(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EXCLAIM_EXCLAIM)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_EXCLAIM_EQUAL): 1;
#define _tpp_lexer_has_TOK_EXCLAIM_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EXCLAIM_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_EXCLAIM_EQUAL_EQUAL): 1;
#define _tpp_lexer_has_TOK_EXCLAIM_EQUAL_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EXCLAIM_EQUAL_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_POUND_POUND)
		unsigned int TPP_INTERNAL(tff_TOK_POUND_POUND): 1;
#define _tpp_lexer_has_TOK_POUND_POUND(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_POUND_POUND)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_POUND_POUND) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT)
		unsigned int TPP_INTERNAL(tff_TOK_PERCENT_PERCENT): 1;
#define _tpp_lexer_has_TOK_PERCENT_PERCENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PERCENT_PERCENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_PERCENT_PERCENT_EQUAL): 1;
#define _tpp_lexer_has_TOK_PERCENT_PERCENT_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PERCENT_PERCENT_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_PERCENT_EQUAL): 1;
#define _tpp_lexer_has_TOK_PERCENT_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PERCENT_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PERCENT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_AMP)
		unsigned int TPP_INTERNAL(tff_TOK_AMP_AMP): 1;
#define _tpp_lexer_has_TOK_AMP_AMP(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_AMP_AMP)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_AMP) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_AMP_EQUAL): 1;
#define _tpp_lexer_has_TOK_AMP_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_AMP_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AMP_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR)
		unsigned int TPP_INTERNAL(tff_TOK_STAR_STAR): 1;
#define _tpp_lexer_has_TOK_STAR_STAR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_STAR_STAR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_STAR_STAR_EQUAL): 1;
#define _tpp_lexer_has_TOK_STAR_STAR_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_STAR_STAR_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_STAR_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_DOT)
		unsigned int TPP_INTERNAL(tff_TOK_STAR_DOT): 1;
#define _tpp_lexer_has_TOK_STAR_DOT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_STAR_DOT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_DOT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_LANGLE_MINUS)
		unsigned int TPP_INTERNAL(tff_TOK_STAR_LANGLE_MINUS): 1;
#define _tpp_lexer_has_TOK_STAR_LANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_STAR_LANGLE_MINUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_STAR_EQUAL): 1;
#define _tpp_lexer_has_TOK_STAR_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_STAR_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_STAR_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_PLUS)
		unsigned int TPP_INTERNAL(tff_TOK_PLUS_PLUS): 1;
#define _tpp_lexer_has_TOK_PLUS_PLUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PLUS_PLUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_PLUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_PLUS_EQUAL): 1;
#define _tpp_lexer_has_TOK_PLUS_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PLUS_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PLUS_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_MINUS)
		unsigned int TPP_INTERNAL(tff_TOK_MINUS_MINUS): 1;
#define _tpp_lexer_has_TOK_MINUS_MINUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_MINUS_MINUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_MINUS_LANGLE): 1;
#define _tpp_lexer_has_TOK_MINUS_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_MINUS_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_MINUS_LANGLE_LANGLE): 1;
#define _tpp_lexer_has_TOK_MINUS_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_MINUS_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_MINUS_LANGLE_LANGLE_LANGLE): 1;
#define _tpp_lexer_has_TOK_MINUS_LANGLE_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_MINUS_LANGLE_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_MINUS_EQUAL): 1;
#define _tpp_lexer_has_TOK_MINUS_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_MINUS_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_MINUS_RANGLE): 1;
#define _tpp_lexer_has_TOK_MINUS_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_MINUS_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_STAR)
		unsigned int TPP_INTERNAL(tff_TOK_MINUS_RANGLE_STAR): 1;
#define _tpp_lexer_has_TOK_MINUS_RANGLE_STAR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_MINUS_RANGLE_STAR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_MINUS_RANGLE_RANGLE): 1;
#define _tpp_lexer_has_TOK_MINUS_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_MINUS_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_MINUS_RANGLE_RANGLE_RANGLE): 1;
#define _tpp_lexer_has_TOK_MINUS_RANGLE_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_MINUS_RANGLE_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_STAR)
		unsigned int TPP_INTERNAL(tff_TOK_DOT_STAR): 1;
#define _tpp_lexer_has_TOK_DOT_STAR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_DOT_STAR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT)
		unsigned int TPP_INTERNAL(tff_TOK_DOT_DOT): 1;
#define _tpp_lexer_has_TOK_DOT_DOT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_DOT_DOT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT_DOT)
		unsigned int TPP_INTERNAL(tff_TOK_DOT_DOT_DOT): 1;
#define _tpp_lexer_has_TOK_DOT_DOT_DOT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_DOT_DOT_DOT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_DOT_DOT_DOT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH)
		unsigned int TPP_INTERNAL(tff_TOK_SLASH_SLASH): 1;
#define _tpp_lexer_has_TOK_SLASH_SLASH(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SLASH_SLASH)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_SLASH_SLASH_EQUAL): 1;
#define _tpp_lexer_has_TOK_SLASH_SLASH_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SLASH_SLASH_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_SLASH_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_SLASH_EQUAL): 1;
#define _tpp_lexer_has_TOK_SLASH_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_SLASH_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_SLASH_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_COLON)
		unsigned int TPP_INTERNAL(tff_TOK_COLON_COLON): 1;
#define _tpp_lexer_has_TOK_COLON_COLON(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_COLON_COLON)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_COLON) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_COLON_EQUAL): 1;
#define _tpp_lexer_has_TOK_COLON_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_COLON_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_COLON_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_MINUS): 1;
#define _tpp_lexer_has_TOK_LANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_MINUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_MINUS_LANGLE): 1;
#define _tpp_lexer_has_TOK_LANGLE_MINUS_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_MINUS_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_MINUS_RANGLE): 1;
#define _tpp_lexer_has_TOK_LANGLE_MINUS_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_MINUS_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_LANGLE): 1;
#define _tpp_lexer_has_TOK_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_MINUS)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_MINUS): 1;
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_MINUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_LANGLE): 1;
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_LANGLE_MINUS): 1;
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_LANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_LANGLE_MINUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_LANGLE_EQUAL): 1;
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_LANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_EQUAL): 1;
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_LANGLE_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_EQUAL): 1;
#define _tpp_lexer_has_TOK_LANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_EQUAL_LANGLE): 1;
#define _tpp_lexer_has_TOK_LANGLE_EQUAL_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_EQUAL_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_EQUAL_RANGLE): 1;
#define _tpp_lexer_has_TOK_LANGLE_EQUAL_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_EQUAL_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_LANGLE_RANGLE): 1;
#define _tpp_lexer_has_TOK_LANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_LANGLE_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_LANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EXCLAIM)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_EXCLAIM): 1;
#define _tpp_lexer_has_TOK_EQUAL_EXCLAIM(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_EXCLAIM)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_PERCENT): 1;
#define _tpp_lexer_has_TOK_EQUAL_PERCENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_PERCENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_PERCENT_PERCENT): 1;
#define _tpp_lexer_has_TOK_EQUAL_PERCENT_PERCENT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_PERCENT_PERCENT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AMP)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_AMP): 1;
#define _tpp_lexer_has_TOK_EQUAL_AMP(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_AMP)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AMP) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_STAR): 1;
#define _tpp_lexer_has_TOK_EQUAL_STAR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_STAR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR_STAR)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_STAR_STAR): 1;
#define _tpp_lexer_has_TOK_EQUAL_STAR_STAR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_STAR_STAR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_STAR_STAR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PLUS)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_PLUS): 1;
#define _tpp_lexer_has_TOK_EQUAL_PLUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_PLUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PLUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_MINUS)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_MINUS): 1;
#define _tpp_lexer_has_TOK_EQUAL_MINUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_MINUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_SLASH): 1;
#define _tpp_lexer_has_TOK_EQUAL_SLASH(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_SLASH)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH_SLASH)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_SLASH_SLASH): 1;
#define _tpp_lexer_has_TOK_EQUAL_SLASH_SLASH(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_SLASH_SLASH)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_SLASH_SLASH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_COLON)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_COLON): 1;
#define _tpp_lexer_has_TOK_EQUAL_COLON(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_COLON)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_COLON) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_LANGLE): 1;
#define _tpp_lexer_has_TOK_EQUAL_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_LANGLE_LANGLE): 1;
#define _tpp_lexer_has_TOK_EQUAL_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_LANGLE_LANGLE_LANGLE): 1;
#define _tpp_lexer_has_TOK_EQUAL_LANGLE_LANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_LANGLE_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_EQUAL): 1;
#define _tpp_lexer_has_TOK_EQUAL_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_EQUAL_EXCLAIM): 1;
#define _tpp_lexer_has_TOK_EQUAL_EQUAL_EXCLAIM(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_EQUAL_EXCLAIM)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_EQUAL_EQUAL): 1;
#define _tpp_lexer_has_TOK_EQUAL_EQUAL_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_EQUAL_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_RANGLE): 1;
#define _tpp_lexer_has_TOK_EQUAL_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_RANGLE_RANGLE): 1;
#define _tpp_lexer_has_TOK_EQUAL_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_RANGLE_RANGLE_RANGLE): 1;
#define _tpp_lexer_has_TOK_EQUAL_RANGLE_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_QMARK)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_QMARK): 1;
#define _tpp_lexer_has_TOK_EQUAL_QMARK(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_QMARK)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_QMARK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_AT): 1;
#define _tpp_lexer_has_TOK_EQUAL_AT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_AT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT_AT)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_AT_AT): 1;
#define _tpp_lexer_has_TOK_EQUAL_AT_AT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_AT_AT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_AT_AT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_HAT)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_HAT): 1;
#define _tpp_lexer_has_TOK_EQUAL_HAT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_HAT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_HAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PIPE)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_PIPE): 1;
#define _tpp_lexer_has_TOK_EQUAL_PIPE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_PIPE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_PIPE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_TILDE)
		unsigned int TPP_INTERNAL(tff_TOK_EQUAL_TILDE): 1;
#define _tpp_lexer_has_TOK_EQUAL_TILDE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_EQUAL_TILDE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_EQUAL_TILDE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_MINUS): 1;
#define _tpp_lexer_has_TOK_RANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_MINUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_MINUS_LANGLE): 1;
#define _tpp_lexer_has_TOK_RANGLE_MINUS_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_MINUS_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_MINUS_RANGLE): 1;
#define _tpp_lexer_has_TOK_RANGLE_MINUS_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_MINUS_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_LANGLE): 1;
#define _tpp_lexer_has_TOK_RANGLE_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_EQUAL): 1;
#define _tpp_lexer_has_TOK_RANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_EQUAL_LANGLE): 1;
#define _tpp_lexer_has_TOK_RANGLE_EQUAL_LANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_EQUAL_LANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_EQUAL_RANGLE): 1;
#define _tpp_lexer_has_TOK_RANGLE_EQUAL_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_EQUAL_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_RANGLE): 1;
#define _tpp_lexer_has_TOK_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_MINUS)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_MINUS): 1;
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_MINUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_EQUAL): 1;
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_RANGLE): 1;
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_RANGLE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_RANGLE_MINUS): 1;
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_RANGLE_MINUS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_RANGLE_MINUS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_RANGLE_EQUAL): 1;
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_RANGLE_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_QMARK_EQUAL): 1;
#define _tpp_lexer_has_TOK_QMARK_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_QMARK_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_QMARK)
		unsigned int TPP_INTERNAL(tff_TOK_QMARK_QMARK): 1;
#define _tpp_lexer_has_TOK_QMARK_QMARK(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_QMARK_QMARK)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_QMARK_QMARK) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_AT_EQUAL): 1;
#define _tpp_lexer_has_TOK_AT_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_AT_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT)
		unsigned int TPP_INTERNAL(tff_TOK_AT_AT): 1;
#define _tpp_lexer_has_TOK_AT_AT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_AT_AT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_AT_AT_EQUAL): 1;
#define _tpp_lexer_has_TOK_AT_AT_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_AT_AT_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_AT_AT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_HAT_EQUAL): 1;
#define _tpp_lexer_has_TOK_HAT_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_HAT_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_HAT)
		unsigned int TPP_INTERNAL(tff_TOK_HAT_HAT): 1;
#define _tpp_lexer_has_TOK_HAT_HAT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_HAT_HAT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_HAT_HAT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_PIPE_EQUAL): 1;
#define _tpp_lexer_has_TOK_PIPE_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PIPE_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_PIPE)
		unsigned int TPP_INTERNAL(tff_TOK_PIPE_PIPE): 1;
#define _tpp_lexer_has_TOK_PIPE_PIPE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_PIPE_PIPE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_PIPE_PIPE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_EQUAL)
		unsigned int TPP_INTERNAL(tff_TOK_TILDE_EQUAL): 1;
#define _tpp_lexer_has_TOK_TILDE_EQUAL(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_TILDE_EQUAL)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_EQUAL) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_TILDE)
		unsigned int TPP_INTERNAL(tff_TOK_TILDE_TILDE): 1;
#define _tpp_lexer_has_TOK_TILDE_TILDE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_TOK_TILDE_TILDE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_TOK_TILDE_TILDE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_DEFINED)
		unsigned int TPP_INTERNAL(tff_BUILTIN_EXPR_DEFINED): 1;
#define _tpp_lexer_has_BUILTIN_EXPR_DEFINED(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BUILTIN_EXPR_DEFINED)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_DEFINED) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR)
		unsigned int TPP_INTERNAL(tff_DONT_EXPAND_DEFINED_IN_EXPR): 1;
#define _tpp_lexer_has_DONT_EXPAND_DEFINED_IN_EXPR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_DONT_EXPAND_DEFINED_IN_EXPR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_STRINGS)
		unsigned int TPP_INTERNAL(tff_BUILTIN_EXPR_STRINGS): 1;
#define _tpp_lexer_has_BUILTIN_EXPR_STRINGS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BUILTIN_EXPR_STRINGS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_STRINGS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FLOATS)
		unsigned int TPP_INTERNAL(tff_BUILTIN_EXPR_FLOATS): 1;
#define _tpp_lexer_has_BUILTIN_EXPR_FLOATS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BUILTIN_EXPR_FLOATS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_FLOATS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)
		unsigned int TPP_INTERNAL(tff_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT): 1;
#define _tpp_lexer_has_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS)
		unsigned int TPP_INTERNAL(tff_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS): 1;
#define _tpp_lexer_has_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR)
		unsigned int TPP_INTERNAL(tff_BUILTIN_EXPR_LOGICAL_XOR): 1;
#define _tpp_lexer_has_BUILTIN_EXPR_LOGICAL_XOR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BUILTIN_EXPR_LOGICAL_XOR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS)
		unsigned int TPP_INTERNAL(tff_BUILTIN_EXPR_BINARY_LITERALS): 1;
#define _tpp_lexer_has_BUILTIN_EXPR_BINARY_LITERALS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BUILTIN_EXPR_BINARY_LITERALS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS)
		unsigned int TPP_INTERNAL(tff_BUILTIN_EXPR_OCTAL_LITERALS): 1;
#define _tpp_lexer_has_BUILTIN_EXPR_OCTAL_LITERALS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BUILTIN_EXPR_OCTAL_LITERALS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX)
		unsigned int TPP_INTERNAL(tff_LEXER_DECODEINT_FIXED_TYPE_SUFFIX): 1;
#define _tpp_lexer_has_LEXER_DECODEINT_FIXED_TYPE_SUFFIX(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_LEXER_DECODEINT_FIXED_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX)
		unsigned int TPP_INTERNAL(tff_LEXER_DECODEINT_SIZE_TYPE_SUFFIX): 1;
#define _tpp_lexer_has_LEXER_DECODEINT_SIZE_TYPE_SUFFIX(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_LEXER_DECODEINT_SIZE_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX)
		unsigned int TPP_INTERNAL(tff_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX): 1;
#define _tpp_lexer_has_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX)
		unsigned int TPP_INTERNAL(tff_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX): 1;
#define _tpp_lexer_has_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX)
		unsigned int TPP_INTERNAL(tff_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX): 1;
#define _tpp_lexer_has_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX)
		unsigned int TPP_INTERNAL(tff_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX): 1;
#define _tpp_lexer_has_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS)
		unsigned int TPP_INTERNAL(tff_BUILTIN_EXPR_CHARACTER_LITERALS): 1;
#define _tpp_lexer_has_BUILTIN_EXPR_CHARACTER_LITERALS(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_BUILTIN_EXPR_CHARACTER_LITERALS)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_EXTERN_C_FOR_SYSHDR)
		unsigned int TPP_INTERNAL(tff_EXTERN_C_FOR_SYSHDR): 1;
#define _tpp_lexer_has_EXTERN_C_FOR_SYSHDR(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_EXTERN_C_FOR_SYSHDR)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_EXTERN_C_FOR_SYSHDR) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH)
		unsigned int TPP_INTERNAL(tff_SEARCH_SYSTEM_INCLUDE_PATH): 1;
#define _tpp_lexer_has_SEARCH_SYSTEM_INCLUDE_PATH(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_SEARCH_SYSTEM_INCLUDE_PATH)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE)
		unsigned int TPP_INTERNAL(tff_INCLUDE_RELATIVE_TO_EVERY_FILE): 1;
#define _tpp_lexer_has_INCLUDE_RELATIVE_TO_EVERY_FILE(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_INCLUDE_RELATIVE_TO_EVERY_FILE)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION)
		unsigned int TPP_INTERNAL(tff_PRAGMA_MESSAGE_PRINTS_LOCATION): 1;
#define _tpp_lexer_has_PRAGMA_MESSAGE_PRINTS_LOCATION(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_MESSAGE_PRINTS_LOCATION)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION) */
#if TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED)
		unsigned int TPP_INTERNAL(tff_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED): 1;
#define _tpp_lexer_has_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED(self) (self)->TPP_INTERNAL(tl_feat).TPP_INTERNAL(tf_flags).TPP_INTERNAL(tff_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED)
#endif /* TPP_CONF_IS_FEAT(TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED) */
	} TPP_INTERNAL(tf_flags);
	unsigned char TPP_INTERNAL(ttf_bitset)[TPP_FEAT_COUNT ? ((TPP_FEAT_COUNT + TPP_CHAR_BIT - 1) / TPP_CHAR_BIT) : 1];
} tpp_features;

TPP_CONST_DECL tpp_features const tpp_features_default;

#define tpp_features_getid(self, id) \
	((self)->TPP_INTERNAL(ttf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] & (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_features_enable(self, id) \
	(void)((self)->TPP_INTERNAL(ttf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] |= (1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_features_disable(self, id) \
	(void)((self)->TPP_INTERNAL(ttf_bitset)[(unsigned int)(id) / TPP_CHAR_BIT] &= ~(1 << ((unsigned int)(id) % TPP_CHAR_BIT)))
#define tpp_features_setid(self, id, enabled) \
	((enabled) ? tpp_features_enable(self, id) : tpp_features_disable(self, id))
#define tpp_features_init(self)            (void)(*(self) = tpp_features_default)
#define tpp_features_reset(self)           (void)(*(self) = tpp_features_default)
#define tpp_features_fini(self)            tpp_dbg_memset(self, sizeof(tpp_features))
#endif /* TPP_HAVE_FEATURES */

#if TPP_CONF_IS_CONST(TPP_HAVE_BSE)
#define _tpp_lexer_has_BSE(self) TPP_CONF_DEFAULT(TPP_HAVE_BSE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BSE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BSE_WHITESPACE)
#define _tpp_lexer_has_BSE_WHITESPACE(self) TPP_CONF_DEFAULT(TPP_HAVE_BSE_WHITESPACE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BSE_WHITESPACE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
#define _tpp_lexer_has_ESCAPE_IN_IDENTIFIERS(self) TPP_CONF_DEFAULT(TPP_HAVE_ESCAPE_IN_IDENTIFIERS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_ESCAPE_IN_IDENTIFIERS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_ESCAPE_E_IN_STRINGS)
#define _tpp_lexer_has_ESCAPE_E_IN_STRINGS(self) TPP_CONF_DEFAULT(TPP_HAVE_ESCAPE_E_IN_STRINGS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_ESCAPE_E_IN_STRINGS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_ESCAPE_S_IN_STRINGS)
#define _tpp_lexer_has_ESCAPE_S_IN_STRINGS(self) TPP_CONF_DEFAULT(TPP_HAVE_ESCAPE_S_IN_STRINGS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_ESCAPE_S_IN_STRINGS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_DIRECTIVES)
#define _tpp_lexer_has_CPP_DIRECTIVES(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_DIRECTIVES)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_DIRECTIVES) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_MACROS)
#define _tpp_lexer_has_CPP_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MAGIC_WHITESPACE)
#define _tpp_lexer_has_MAGIC_WHITESPACE(self) TPP_CONF_DEFAULT(TPP_HAVE_MAGIC_WHITESPACE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MAGIC_WHITESPACE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_BUILTIN_MACROS)
#define _tpp_lexer_has_CPP_BUILTIN_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_BUILTIN_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_BUILTIN_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_EXCLAIM)
#define _tpp_lexer_has_CPP_EXCLAIM(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_EXCLAIM)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_EXCLAIM) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_BLANK)
#define _tpp_lexer_has_CPP_BLANK(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_BLANK)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_BLANK) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_DIGIT_LINE)
#define _tpp_lexer_has_CPP_DIGIT_LINE(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_DIGIT_LINE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_DIGIT_LINE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_LINE)
#define _tpp_lexer_has_CPP_LINE(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_LINE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_LINE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_INCLUDE)
#define _tpp_lexer_has_CPP_INCLUDE(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_INCLUDE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_INCLUDE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_INCLUDE_NEXT)
#define _tpp_lexer_has_CPP_INCLUDE_NEXT(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_INCLUDE_NEXT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_INCLUDE_NEXT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_IMPORT)
#define _tpp_lexer_has_CPP_IMPORT(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_IMPORT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_IMPORT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_IF_ELSE_ENDIF)
#define _tpp_lexer_has_CPP_IF_ELSE_ENDIF(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_IF_ELSE_ENDIF)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_IF_ELSE_ENDIF) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_DEFINE)
#define _tpp_lexer_has_CPP_DEFINE(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_DEFINE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_DEFINE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_ASSERT)
#define _tpp_lexer_has_CPP_ASSERT(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_ASSERT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_ASSERT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_ERROR)
#define _tpp_lexer_has_CPP_ERROR(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_ERROR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_ERROR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_WARNING)
#define _tpp_lexer_has_CPP_WARNING(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_WARNING)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_WARNING) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_IDENT_SCCS)
#define _tpp_lexer_has_CPP_IDENT_SCCS(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_IDENT_SCCS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_IDENT_SCCS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_PRAGMA)
#define _tpp_lexer_has_CPP_PRAGMA(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_PRAGMA)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_PRAGMA) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CPP_EMBED)
#define _tpp_lexer_has_CPP_EMBED(self) TPP_CONF_DEFAULT(TPP_HAVE_CPP_EMBED)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CPP_EMBED) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO__Pragma)
#define _tpp_lexer_has_MACRO__Pragma(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO__Pragma)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO__Pragma) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___pragma)
#define _tpp_lexer_has_MACRO___pragma(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___pragma)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___pragma) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_attribute)
#define _tpp_lexer_has_CLANG_MACRO___has_attribute(self) TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_attribute)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_attribute) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_builtin)
#define _tpp_lexer_has_CLANG_MACRO___has_builtin(self) TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_builtin)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_builtin) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_cpp_attribute)
#define _tpp_lexer_has_CLANG_MACRO___has_cpp_attribute(self) TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_cpp_attribute)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_cpp_attribute) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_declspec_attribute)
#define _tpp_lexer_has_CLANG_MACRO___has_declspec_attribute(self) TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_declspec_attribute)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_declspec_attribute) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_extension)
#define _tpp_lexer_has_CLANG_MACRO___has_extension(self) TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_extension)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_extension) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_feature)
#define _tpp_lexer_has_CLANG_MACRO___has_feature(self) TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_feature)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_feature) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_c_attribute)
#define _tpp_lexer_has_CLANG_MACRO___has_c_attribute(self) TPP_CONF_DEFAULT(TPP_HAVE_CLANG_MACRO___has_c_attribute)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CLANG_MACRO___has_c_attribute) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES)
#define _tpp_lexer_has_CLANG_EXTENSIONS_ARE_FEATURES(self) TPP_CONF_DEFAULT(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CLANG_EXTENSIONS_ARE_FEATURES) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___is_identifier)
#define _tpp_lexer_has_MACRO___is_identifier(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___is_identifier)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___is_identifier) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___is_deprecated)
#define _tpp_lexer_has_MACRO___is_deprecated(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___is_deprecated)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___is_deprecated) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___is_poisoned)
#define _tpp_lexer_has_MACRO___is_poisoned(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___is_poisoned)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___is_poisoned) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_extension)
#define _tpp_lexer_has_MACRO___has_extension(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_extension)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_extension) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_known_extension)
#define _tpp_lexer_has_MACRO___has_known_extension(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_known_extension)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_known_extension) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_warning)
#define _tpp_lexer_has_MACRO___has_warning(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_warning)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_warning) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_known_warning)
#define _tpp_lexer_has_MACRO___has_known_warning(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_known_warning)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_known_warning) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_include)
#define _tpp_lexer_has_MACRO___has_include(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_include)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_include) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_include_next)
#define _tpp_lexer_has_MACRO___has_include_next(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_include_next)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_include_next) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_embed)
#define _tpp_lexer_has_MACRO___has_embed(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___has_embed)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___has_embed) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___FILE__)
#define _tpp_lexer_has_MACRO___FILE__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___FILE__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___FILE__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___LINE__)
#define _tpp_lexer_has_MACRO___LINE__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___LINE__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___LINE__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TIME__)
#define _tpp_lexer_has_MACRO___TIME__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TIME__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TIME__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___DATE__)
#define _tpp_lexer_has_MACRO___DATE__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___DATE__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___DATE__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___COLUMN__)
#define _tpp_lexer_has_MACRO___COLUMN__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___COLUMN__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___COLUMN__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___BASE_FILE__)
#define _tpp_lexer_has_MACRO___BASE_FILE__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___BASE_FILE__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___BASE_FILE__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___FILE_NAME__)
#define _tpp_lexer_has_MACRO___FILE_NAME__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___FILE_NAME__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___FILE_NAME__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___INCLUDE_LEVEL__)
#define _tpp_lexer_has_MACRO___INCLUDE_LEVEL__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___INCLUDE_LEVEL__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___INCLUDE_LEVEL__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___INCLUDE_DEPTH__)
#define _tpp_lexer_has_MACRO___INCLUDE_DEPTH__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___INCLUDE_DEPTH__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___INCLUDE_DEPTH__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___COUNTER__)
#define _tpp_lexer_has_MACRO___COUNTER__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___COUNTER__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___COUNTER__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TIMESTAMP__)
#define _tpp_lexer_has_MACRO___TIMESTAMP__(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TIMESTAMP__)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TIMESTAMP__) */
#if TPP_CONF_IS_CONST(TPP_HAVE_NUMERIC_DATE_MACROS)
#define _tpp_lexer_has_NUMERIC_DATE_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_NUMERIC_DATE_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_NUMERIC_DATE_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_NUMERIC_TIME_MACROS)
#define _tpp_lexer_has_NUMERIC_TIME_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_NUMERIC_TIME_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_NUMERIC_TIME_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_EVAL)
#define _tpp_lexer_has_MACRO___TPP_EVAL(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_EVAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_EVAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_EXEC)
#define _tpp_lexer_has_MACRO___TPP_EXEC(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_EXEC)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_EXEC) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_UNIQUE)
#define _tpp_lexer_has_MACRO___TPP_UNIQUE(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_UNIQUE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_UNIQUE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_LOAD_FILE)
#define _tpp_lexer_has_MACRO___TPP_LOAD_FILE(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_LOAD_FILE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_LOAD_FILE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_COUNTER)
#define _tpp_lexer_has_MACRO___TPP_COUNTER(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_COUNTER)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_COUNTER) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_RANDOM)
#define _tpp_lexer_has_MACRO___TPP_RANDOM(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_RANDOM)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_RANDOM) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_STR_DECOMPILE)
#define _tpp_lexer_has_MACRO___TPP_STR_DECOMPILE(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_DECOMPILE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_STR_DECOMPILE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_STR_PACK)
#define _tpp_lexer_has_MACRO___TPP_STR_PACK(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_PACK)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_STR_PACK) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_STR_SUBSTR)
#define _tpp_lexer_has_MACRO___TPP_STR_SUBSTR(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_SUBSTR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_STR_SUBSTR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_STR_SIZE)
#define _tpp_lexer_has_MACRO___TPP_STR_SIZE(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_STR_SIZE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_STR_SIZE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_COUNT_TOKENS)
#define _tpp_lexer_has_MACRO___TPP_COUNT_TOKENS(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_COUNT_TOKENS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_COUNT_TOKENS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_IDENTIFIER)
#define _tpp_lexer_has_MACRO___TPP_IDENTIFIER(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO___TPP_IDENTIFIER)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO___TPP_IDENTIFIER) */
#if TPP_CONF_IS_CONST(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS)
#define _tpp_lexer_has_ALTERNATIVE_MACRO_PARENTHESIS(self) TPP_CONF_DEFAULT(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_ALTERNATIVE_MACRO_PARENTHESIS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
#define _tpp_lexer_has_MACRO_ARGUMENT_WHITESPACE(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO_ARGUMENT_WHITESPACE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_MACRO_RECURSION)
#define _tpp_lexer_has_MACRO_RECURSION(self) TPP_CONF_DEFAULT(TPP_HAVE_MACRO_RECURSION)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_MACRO_RECURSION) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TRADITIONAL_MACROS)
#define _tpp_lexer_has_TRADITIONAL_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_TRADITIONAL_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TRADITIONAL_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_NAMED_VARARGS_IN_MACROS)
#define _tpp_lexer_has_NAMED_VARARGS_IN_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_NAMED_VARARGS_IN_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_NAMED_VARARGS_IN_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_VA_ARGS_IN_MACROS)
#define _tpp_lexer_has_VA_ARGS_IN_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_VA_ARGS_IN_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_VA_ARGS_IN_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_VA_COMMA_IN_MACROS)
#define _tpp_lexer_has_VA_COMMA_IN_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_VA_COMMA_IN_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_VA_COMMA_IN_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_VA_OPT_IN_MACROS)
#define _tpp_lexer_has_VA_OPT_IN_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_VA_OPT_IN_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_VA_OPT_IN_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_VA_NARGS_IN_MACROS)
#define _tpp_lexer_has_VA_NARGS_IN_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_VA_NARGS_IN_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_VA_NARGS_IN_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS)
#define _tpp_lexer_has_VA_GLUE_COMMA_IN_MACROS(self) TPP_CONF_DEFAULT(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_VA_GLUE_COMMA_IN_MACROS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT)
#define _tpp_lexer_has_STRINGIZE_MACRO_ARGUMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_STRINGIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_CHARIZE_MACRO_ARGUMENT)
#define _tpp_lexer_has_CHARIZE_MACRO_ARGUMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_CHARIZE_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_CHARIZE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT)
#define _tpp_lexer_has_DONT_EXPAND_MACRO_ARGUMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_DONT_EXPAND_MACRO_ARGUMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_GLUE_MACRO_ARGUMENT)
#define _tpp_lexer_has_GLUE_MACRO_ARGUMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_GLUE_MACRO_ARGUMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_GLUE_MACRO_ARGUMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_PUSH_MACRO)
#define _tpp_lexer_has_PRAGMA_PUSH_MACRO(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_PUSH_MACRO)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_PUSH_MACRO) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_ONCE)
#define _tpp_lexer_has_PRAGMA_ONCE(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_ONCE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_ONCE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_DEPRECATED)
#define _tpp_lexer_has_PRAGMA_DEPRECATED(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_DEPRECATED)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_DEPRECATED) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_EXTENSION)
#define _tpp_lexer_has_PRAGMA_EXTENSION(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_EXTENSION)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_EXTENSION) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_WARNING)
#define _tpp_lexer_has_PRAGMA_WARNING(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_WARNING)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_WARNING) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_MESSAGE)
#define _tpp_lexer_has_PRAGMA_MESSAGE(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_MESSAGE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_MESSAGE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_ERROR)
#define _tpp_lexer_has_PRAGMA_ERROR(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_ERROR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_ERROR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_REGION)
#define _tpp_lexer_has_PRAGMA_REGION(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_REGION)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_REGION) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_TPP_EXEC)
#define _tpp_lexer_has_PRAGMA_TPP_EXEC(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_TPP_EXEC)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_TPP_EXEC) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS)
#define _tpp_lexer_has_PRAGMA_TPP_SET_KEYWORD_FLAGS(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_TPP_SET_KEYWORD_FLAGS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_POISON)
#define _tpp_lexer_has_PRAGMA_GCC_POISON(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_POISON)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_POISON) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_WARNING)
#define _tpp_lexer_has_PRAGMA_GCC_WARNING(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_WARNING)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_WARNING) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_ERROR)
#define _tpp_lexer_has_PRAGMA_GCC_ERROR(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_ERROR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_ERROR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER)
#define _tpp_lexer_has_PRAGMA_GCC_SYSTEM_HEADER(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_SYSTEM_HEADER) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC)
#define _tpp_lexer_has_PRAGMA_GCC_DIAGNOSTIC(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_DIAGNOSTIC) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
#define _tpp_lexer_has_PRAGMA_GCC_DEPENDENCY(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_GCC_DEPENDENCY)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_GCC_DEPENDENCY) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TRIGRAPHS)
#define _tpp_lexer_has_TRIGRAPHS(self) TPP_CONF_DEFAULT(TPP_HAVE_TRIGRAPHS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TRIGRAPHS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_DIGRAPHS)
#define _tpp_lexer_has_DIGRAPHS(self) TPP_CONF_DEFAULT(TPP_HAVE_DIGRAPHS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_DIGRAPHS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LF)
#define _tpp_lexer_has_TOK_LF(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LF)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LF) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SPACE)
#define _tpp_lexer_has_TOK_SPACE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SPACE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SPACE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_COMMENT)
#define _tpp_lexer_has_TOK_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_COMMENT)
#define _tpp_lexer_has_TOK_CXX_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_COMMENT)
#define _tpp_lexer_has_TOK_C_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_C_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PASCAL_COMMENT)
#define _tpp_lexer_has_TOK_PASCAL_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PASCAL_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PASCAL_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PASCAL_BRACE_COMMENT)
#define _tpp_lexer_has_TOK_PASCAL_BRACE_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PASCAL_BRACE_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PASCAL_BRACE_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_HTML_COMMENT)
#define _tpp_lexer_has_TOK_HTML_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_HTML_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_HTML_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SQL_COMMENT)
#define _tpp_lexer_has_TOK_SQL_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SQL_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SQL_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_AT_COMMENT)
#define _tpp_lexer_has_TOK_AT_AT_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_AT_AT_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_AT_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SHELL_COMMENT)
#define _tpp_lexer_has_TOK_SHELL_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SHELL_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SHELL_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SLASH_COMMENT)
#define _tpp_lexer_has_TOK_SLASH_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SLASH_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SLASH_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_COMMENT)
#define _tpp_lexer_has_TOK_AT_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_AT_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SOL_SHELL_COMMENT)
#define _tpp_lexer_has_TOK_SOL_SHELL_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SOL_SHELL_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SOL_SHELL_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SOL_SLASH_COMMENT)
#define _tpp_lexer_has_TOK_SOL_SLASH_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SOL_SLASH_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SOL_SLASH_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SOL_AT_COMMENT)
#define _tpp_lexer_has_TOK_SOL_AT_COMMENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SOL_AT_COMMENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SOL_AT_COMMENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_DOLLAR)
#define _tpp_lexer_has_TOK_DOLLAR(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_DOLLAR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_DOLLAR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE)
#define _tpp_lexer_has_THOUSANDS_SEPARATOR_UNDERSCORE(self) TPP_CONF_DEFAULT(TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_THOUSANDS_SEPARATOR_UNDERSCORE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK)
#define _tpp_lexer_has_THOUSANDS_SEPARATOR_SINGLETICK(self) TPP_CONF_DEFAULT(TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_THOUSANDS_SEPARATOR_SINGLETICK) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_INT)
#define _tpp_lexer_has_TOK_C_INT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_C_INT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_INT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PASCAL_HEX)
#define _tpp_lexer_has_TOK_PASCAL_HEX(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PASCAL_HEX)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PASCAL_HEX) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_FLOAT)
#define _tpp_lexer_has_TOK_C_FLOAT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_C_FLOAT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_FLOAT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_SMART_FLOAT_TOKENS)
#define _tpp_lexer_has_SMART_FLOAT_TOKENS(self) TPP_CONF_DEFAULT(TPP_HAVE_SMART_FLOAT_TOKENS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_SMART_FLOAT_TOKENS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_CHAR)
#define _tpp_lexer_has_TOK_C_CHAR(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_C_CHAR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_CHAR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_STRING)
#define _tpp_lexer_has_TOK_C_STRING(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_C_STRING)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_C_STRING) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL)
#define _tpp_lexer_has_TOK_CXX_RAW_STRING_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL)
#define _tpp_lexer_has_TOK_CXX_WIDE_STRING_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_WIDE_STRING_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL)
#define _tpp_lexer_has_TOK_CXX_UTF8_STRING_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF8_STRING_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL)
#define _tpp_lexer_has_TOK_CXX_UTF16_STRING_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF16_STRING_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL)
#define _tpp_lexer_has_TOK_CXX_UTF32_STRING_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF32_STRING_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL)
#define _tpp_lexer_has_TOK_CXX_RAW_CHAR_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL)
#define _tpp_lexer_has_TOK_CXX_WIDE_CHAR_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_WIDE_CHAR_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL)
#define _tpp_lexer_has_TOK_CXX_UTF8_CHAR_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF8_CHAR_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL)
#define _tpp_lexer_has_TOK_CXX_UTF16_CHAR_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF16_CHAR_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL)
#define _tpp_lexer_has_TOK_CXX_UTF32_CHAR_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_CXX_UTF32_CHAR_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RAW_STRING_LITERAL)
#define _tpp_lexer_has_TOK_RAW_STRING_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RAW_STRING_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RAW_STRING_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RAW_CHAR_LITERAL)
#define _tpp_lexer_has_TOK_RAW_CHAR_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RAW_CHAR_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RAW_CHAR_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_BLOCK_STRING_LITERAL)
#define _tpp_lexer_has_TOK_BLOCK_STRING_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_BLOCK_STRING_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_BLOCK_STRING_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_BLOCK_CHAR_LITERAL)
#define _tpp_lexer_has_TOK_BLOCK_CHAR_LITERAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_BLOCK_CHAR_LITERAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_BLOCK_CHAR_LITERAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_STRING_ALLOW_MULTILINE)
#define _tpp_lexer_has_STRING_ALLOW_MULTILINE(self) TPP_CONF_DEFAULT(TPP_HAVE_STRING_ALLOW_MULTILINE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_STRING_ALLOW_MULTILINE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_STRING_AUTO_CONCAT)
#define _tpp_lexer_has_STRING_AUTO_CONCAT(self) TPP_CONF_DEFAULT(TPP_HAVE_STRING_AUTO_CONCAT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_STRING_AUTO_CONCAT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EXCLAIM_EXCLAIM)
#define _tpp_lexer_has_TOK_EXCLAIM_EXCLAIM(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EXCLAIM_EXCLAIM)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EXCLAIM_EXCLAIM) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EXCLAIM_EQUAL)
#define _tpp_lexer_has_TOK_EXCLAIM_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EXCLAIM_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EXCLAIM_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL)
#define _tpp_lexer_has_TOK_EXCLAIM_EQUAL_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EXCLAIM_EQUAL_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_POUND_POUND)
#define _tpp_lexer_has_TOK_POUND_POUND(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_POUND_POUND)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_POUND_POUND) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PERCENT_PERCENT)
#define _tpp_lexer_has_TOK_PERCENT_PERCENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PERCENT_PERCENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PERCENT_PERCENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL)
#define _tpp_lexer_has_TOK_PERCENT_PERCENT_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PERCENT_PERCENT_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PERCENT_EQUAL)
#define _tpp_lexer_has_TOK_PERCENT_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PERCENT_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PERCENT_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_AMP_AMP)
#define _tpp_lexer_has_TOK_AMP_AMP(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_AMP_AMP)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_AMP_AMP) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_AMP_EQUAL)
#define _tpp_lexer_has_TOK_AMP_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_AMP_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_AMP_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_STAR)
#define _tpp_lexer_has_TOK_STAR_STAR(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_STAR_STAR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_STAR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_STAR_EQUAL)
#define _tpp_lexer_has_TOK_STAR_STAR_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_STAR_STAR_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_STAR_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_DOT)
#define _tpp_lexer_has_TOK_STAR_DOT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_STAR_DOT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_DOT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_LANGLE_MINUS)
#define _tpp_lexer_has_TOK_STAR_LANGLE_MINUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_STAR_LANGLE_MINUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_LANGLE_MINUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_EQUAL)
#define _tpp_lexer_has_TOK_STAR_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_STAR_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_STAR_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PLUS_PLUS)
#define _tpp_lexer_has_TOK_PLUS_PLUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PLUS_PLUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PLUS_PLUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PLUS_EQUAL)
#define _tpp_lexer_has_TOK_PLUS_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PLUS_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PLUS_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_MINUS)
#define _tpp_lexer_has_TOK_MINUS_MINUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_MINUS_MINUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_MINUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_LANGLE)
#define _tpp_lexer_has_TOK_MINUS_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_MINUS_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE)
#define _tpp_lexer_has_TOK_MINUS_LANGLE_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE)
#define _tpp_lexer_has_TOK_MINUS_LANGLE_LANGLE_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_EQUAL)
#define _tpp_lexer_has_TOK_MINUS_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_MINUS_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_RANGLE)
#define _tpp_lexer_has_TOK_MINUS_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_MINUS_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_RANGLE_STAR)
#define _tpp_lexer_has_TOK_MINUS_RANGLE_STAR(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_MINUS_RANGLE_STAR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_RANGLE_STAR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE)
#define _tpp_lexer_has_TOK_MINUS_RANGLE_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE)
#define _tpp_lexer_has_TOK_MINUS_RANGLE_RANGLE_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_MINUS_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_DOT_STAR)
#define _tpp_lexer_has_TOK_DOT_STAR(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_DOT_STAR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_DOT_STAR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_DOT_DOT)
#define _tpp_lexer_has_TOK_DOT_DOT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_DOT_DOT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_DOT_DOT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_DOT_DOT_DOT)
#define _tpp_lexer_has_TOK_DOT_DOT_DOT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_DOT_DOT_DOT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_DOT_DOT_DOT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SLASH_SLASH)
#define _tpp_lexer_has_TOK_SLASH_SLASH(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SLASH_SLASH)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SLASH_SLASH) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SLASH_SLASH_EQUAL)
#define _tpp_lexer_has_TOK_SLASH_SLASH_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SLASH_SLASH_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SLASH_SLASH_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_SLASH_EQUAL)
#define _tpp_lexer_has_TOK_SLASH_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_SLASH_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_SLASH_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_COLON_COLON)
#define _tpp_lexer_has_TOK_COLON_COLON(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_COLON_COLON)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_COLON_COLON) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_COLON_EQUAL)
#define _tpp_lexer_has_TOK_COLON_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_COLON_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_COLON_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_MINUS)
#define _tpp_lexer_has_TOK_LANGLE_MINUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_MINUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_MINUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_MINUS_LANGLE)
#define _tpp_lexer_has_TOK_LANGLE_MINUS_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_MINUS_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_MINUS_RANGLE)
#define _tpp_lexer_has_TOK_LANGLE_MINUS_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_MINUS_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE)
#define _tpp_lexer_has_TOK_LANGLE_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_MINUS)
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_MINUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_LANGLE_MINUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_MINUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE)
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS)
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_LANGLE_MINUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_MINUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_LANGLE_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL)
#define _tpp_lexer_has_TOK_LANGLE_LANGLE_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_LANGLE_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_EQUAL)
#define _tpp_lexer_has_TOK_LANGLE_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE)
#define _tpp_lexer_has_TOK_LANGLE_EQUAL_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE)
#define _tpp_lexer_has_TOK_LANGLE_EQUAL_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_RANGLE)
#define _tpp_lexer_has_TOK_LANGLE_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_LANGLE_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_LANGLE_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_EXCLAIM)
#define _tpp_lexer_has_TOK_EQUAL_EXCLAIM(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_EXCLAIM)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_PERCENT)
#define _tpp_lexer_has_TOK_EQUAL_PERCENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_PERCENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_PERCENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT)
#define _tpp_lexer_has_TOK_EQUAL_PERCENT_PERCENT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_PERCENT_PERCENT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_AMP)
#define _tpp_lexer_has_TOK_EQUAL_AMP(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_AMP)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_AMP) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_STAR)
#define _tpp_lexer_has_TOK_EQUAL_STAR(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_STAR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_STAR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_STAR_STAR)
#define _tpp_lexer_has_TOK_EQUAL_STAR_STAR(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_STAR_STAR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_STAR_STAR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_PLUS)
#define _tpp_lexer_has_TOK_EQUAL_PLUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_PLUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_PLUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_MINUS)
#define _tpp_lexer_has_TOK_EQUAL_MINUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_MINUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_MINUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_SLASH)
#define _tpp_lexer_has_TOK_EQUAL_SLASH(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_SLASH)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_SLASH) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_SLASH_SLASH)
#define _tpp_lexer_has_TOK_EQUAL_SLASH_SLASH(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_SLASH_SLASH)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_SLASH_SLASH) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_COLON)
#define _tpp_lexer_has_TOK_EQUAL_COLON(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_COLON)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_COLON) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_LANGLE)
#define _tpp_lexer_has_TOK_EQUAL_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE)
#define _tpp_lexer_has_TOK_EQUAL_LANGLE_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE)
#define _tpp_lexer_has_TOK_EQUAL_LANGLE_LANGLE_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_LANGLE_LANGLE_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_EQUAL)
#define _tpp_lexer_has_TOK_EQUAL_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM)
#define _tpp_lexer_has_TOK_EQUAL_EQUAL_EXCLAIM(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_EQUAL_EXCLAIM) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL)
#define _tpp_lexer_has_TOK_EQUAL_EQUAL_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_EQUAL_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_RANGLE)
#define _tpp_lexer_has_TOK_EQUAL_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE)
#define _tpp_lexer_has_TOK_EQUAL_RANGLE_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
#define _tpp_lexer_has_TOK_EQUAL_RANGLE_RANGLE_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_QMARK)
#define _tpp_lexer_has_TOK_EQUAL_QMARK(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_QMARK)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_QMARK) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_AT)
#define _tpp_lexer_has_TOK_EQUAL_AT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_AT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_AT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_AT_AT)
#define _tpp_lexer_has_TOK_EQUAL_AT_AT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_AT_AT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_AT_AT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_HAT)
#define _tpp_lexer_has_TOK_EQUAL_HAT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_HAT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_HAT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_PIPE)
#define _tpp_lexer_has_TOK_EQUAL_PIPE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_PIPE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_PIPE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_TILDE)
#define _tpp_lexer_has_TOK_EQUAL_TILDE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_EQUAL_TILDE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_EQUAL_TILDE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_MINUS)
#define _tpp_lexer_has_TOK_RANGLE_MINUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_MINUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_MINUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_MINUS_LANGLE)
#define _tpp_lexer_has_TOK_RANGLE_MINUS_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_MINUS_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_MINUS_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_MINUS_RANGLE)
#define _tpp_lexer_has_TOK_RANGLE_MINUS_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_MINUS_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_MINUS_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_LANGLE)
#define _tpp_lexer_has_TOK_RANGLE_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_EQUAL)
#define _tpp_lexer_has_TOK_RANGLE_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE)
#define _tpp_lexer_has_TOK_RANGLE_EQUAL_LANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_EQUAL_LANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE)
#define _tpp_lexer_has_TOK_RANGLE_EQUAL_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_EQUAL_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE)
#define _tpp_lexer_has_TOK_RANGLE_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_MINUS)
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_MINUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_RANGLE_MINUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_MINUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL)
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE)
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_RANGLE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS)
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_RANGLE_MINUS(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_MINUS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
#define _tpp_lexer_has_TOK_RANGLE_RANGLE_RANGLE_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_RANGLE_RANGLE_RANGLE_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_QMARK_EQUAL)
#define _tpp_lexer_has_TOK_QMARK_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_QMARK_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_QMARK_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_QMARK_QMARK)
#define _tpp_lexer_has_TOK_QMARK_QMARK(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_QMARK_QMARK)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_QMARK_QMARK) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_EQUAL)
#define _tpp_lexer_has_TOK_AT_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_AT_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_AT)
#define _tpp_lexer_has_TOK_AT_AT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_AT_AT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_AT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_AT_EQUAL)
#define _tpp_lexer_has_TOK_AT_AT_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_AT_AT_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_AT_AT_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_HAT_EQUAL)
#define _tpp_lexer_has_TOK_HAT_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_HAT_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_HAT_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_HAT_HAT)
#define _tpp_lexer_has_TOK_HAT_HAT(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_HAT_HAT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_HAT_HAT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PIPE_EQUAL)
#define _tpp_lexer_has_TOK_PIPE_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PIPE_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PIPE_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_PIPE_PIPE)
#define _tpp_lexer_has_TOK_PIPE_PIPE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_PIPE_PIPE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_PIPE_PIPE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_TILDE_EQUAL)
#define _tpp_lexer_has_TOK_TILDE_EQUAL(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_TILDE_EQUAL)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_TILDE_EQUAL) */
#if TPP_CONF_IS_CONST(TPP_HAVE_TOK_TILDE_TILDE)
#define _tpp_lexer_has_TOK_TILDE_TILDE(self) TPP_CONF_DEFAULT(TPP_HAVE_TOK_TILDE_TILDE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_TOK_TILDE_TILDE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_DEFINED)
#define _tpp_lexer_has_BUILTIN_EXPR_DEFINED(self) TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_DEFINED)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_DEFINED) */
#if TPP_CONF_IS_CONST(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR)
#define _tpp_lexer_has_DONT_EXPAND_DEFINED_IN_EXPR(self) TPP_CONF_DEFAULT(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_DONT_EXPAND_DEFINED_IN_EXPR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_STRINGS)
#define _tpp_lexer_has_BUILTIN_EXPR_STRINGS(self) TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_STRINGS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_STRINGS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_FLOATS)
#define _tpp_lexer_has_BUILTIN_EXPR_FLOATS(self) TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_FLOATS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_FLOATS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)
#define _tpp_lexer_has_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT(self) TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_OPTIONAL_TT) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS)
#define _tpp_lexer_has_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS(self) TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_IF_ELSE_IN_EXPRESSIONS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR)
#define _tpp_lexer_has_BUILTIN_EXPR_LOGICAL_XOR(self) TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_LOGICAL_XOR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS)
#define _tpp_lexer_has_BUILTIN_EXPR_BINARY_LITERALS(self) TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_BINARY_LITERALS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS)
#define _tpp_lexer_has_BUILTIN_EXPR_OCTAL_LITERALS(self) TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_OCTAL_LITERALS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX)
#define _tpp_lexer_has_LEXER_DECODEINT_FIXED_TYPE_SUFFIX(self) TPP_CONF_DEFAULT(TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEINT_FIXED_TYPE_SUFFIX) */
#if TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX)
#define _tpp_lexer_has_LEXER_DECODEINT_SIZE_TYPE_SUFFIX(self) TPP_CONF_DEFAULT(TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEINT_SIZE_TYPE_SUFFIX) */
#if TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX)
#define _tpp_lexer_has_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX(self) TPP_CONF_DEFAULT(TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEINT_FIXED_LENGTH_SUFFIX) */
#if TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX)
#define _tpp_lexer_has_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX(self) TPP_CONF_DEFAULT(TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEFLOAT_FIXED_TYPE_SUFFIX) */
#if TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX)
#define _tpp_lexer_has_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX(self) TPP_CONF_DEFAULT(TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEFLOAT_DOUBLE_TYPE_SUFFIX) */
#if TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX)
#define _tpp_lexer_has_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX(self) TPP_CONF_DEFAULT(TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_LEXER_DECODEFLOAT_DECIMAL_TYPE_SUFFIX) */
#if TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS)
#define _tpp_lexer_has_BUILTIN_EXPR_CHARACTER_LITERALS(self) TPP_CONF_DEFAULT(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_BUILTIN_EXPR_CHARACTER_LITERALS) */
#if TPP_CONF_IS_CONST(TPP_HAVE_EXTERN_C_FOR_SYSHDR)
#define _tpp_lexer_has_EXTERN_C_FOR_SYSHDR(self) TPP_CONF_DEFAULT(TPP_HAVE_EXTERN_C_FOR_SYSHDR)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_EXTERN_C_FOR_SYSHDR) */
#if TPP_CONF_IS_CONST(TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH)
#define _tpp_lexer_has_SEARCH_SYSTEM_INCLUDE_PATH(self) TPP_CONF_DEFAULT(TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_SEARCH_SYSTEM_INCLUDE_PATH) */
#if TPP_CONF_IS_CONST(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE)
#define _tpp_lexer_has_INCLUDE_RELATIVE_TO_EVERY_FILE(self) TPP_CONF_DEFAULT(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_INCLUDE_RELATIVE_TO_EVERY_FILE) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION)
#define _tpp_lexer_has_PRAGMA_MESSAGE_PRINTS_LOCATION(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_MESSAGE_PRINTS_LOCATION) */
#if TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED)
#define _tpp_lexer_has_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED(self) TPP_CONF_DEFAULT(TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED)
#endif /* TPP_CONF_IS_CONST(TPP_HAVE_PRAGMA_MESSAGE_OMITS_TRAILING_LINEFEED) */
/*[[[end]]]*/

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_FEATURES_H */
