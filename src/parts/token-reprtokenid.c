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
#ifndef GUARD_TPP_TOKEN_REPRTOKENID_C
#define GUARD_TPP_TOKEN_REPRTOKENID_C 1
#define TPP_BUILDING 1

#include "api.h"

#include "config.h"
#include "ctype.h"
#include "error.h"
#include "keyword.h"
#include "string.h"
#include "token.h"

/*[[[tpp-begin]]]*/
TPP_DECL_BEGIN

#if TPP_HAVE_REPRTOKENID

/*[[[deemon
import * from deemon;

class TokenId {
	this = default;
	@@ID: "TPP_TOK_EQUAL_EQUAL_EQUAL"
	public member id: string;
	@@Repr: "==="
	public member tokenRepr: string;
	@@Condition: "TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL"
	public member condition: string;
}

local tokens: {TokenId...} = [];
local conditionStack = [];
local inside = false;
for (local line: File.open("token.h").read().decode("utf-8").splitlines(false)) {
	if (!inside) {
		if ("TPP_TOK_MULTICHAR_BEGIN" in line)
			inside = true;
		continue;
	}
	if ("TPP_TOK_MULTICHAR_END" in line)
		break;
	if (line.startswith("#if")) {
		conditionStack.append(line[3:].strip());
	} else if (line.startswith("#elif")) {
		local last = conditionStack.popback();
		conditionStack.append(f"!{last} && {line[5:].strip()}");
	} else if (line.startswith("#else")) {
		local last = conditionStack.popback();
		conditionStack.append(f"!{last}");
	} else if (line.startswith("#endif")) {
		conditionStack.popback();
	} else {
		local tokenName, comment;
		try {
			tokenName, comment = line.rescanf(r"\s*(\w+)\s*,\s*(//.*|/\*.*\*" r"/)")...;
		} catch (...) {
			try {
				tokenName = line.rescanf(r"\s*(\w+)\s*,")...;
				comment = "";
			} catch (...) {
				continue;
			}
		}
		try {
			comment = comment.rescanf(r'[/\*\s]*"([^"]*)"').first;
		} catch (...) {
			continue;
		}
		if (tokenName.startswith("_"))
			continue;
		local tokenId = TokenId(
			id: tokenName,
			tokenRepr: comment,
			condition: " && ".join(conditionStack));
		tokens.append(tokenId);
	}
}

local oneCharTokenNames = List([""] * 256);
for (local i: [:256]) {
	local ch = string.chr(i);
	if (i < 128 && ch.isgraph()) {
		oneCharTokenNames[i] = ch;
	} else {
		oneCharTokenNames[i] = f"\\x{i.tostr(16, 2)}";
	}
}
oneCharTokenNames[0] = "<eof>";
oneCharTokenNames['\n'.ord()] = "<linefeed>";
oneCharTokenNames[' '.ord()] = "<space>";

class TokenString {
	this = default;
	public member name: string;
	public member value: string;
	public member condition: string = "1";
}

function parenCondition(c: string): string {
	if (!c.rematches(r"\w+") && !c.rematches(r"\([^)]*\)(\s*\|\|\s*\([^)]*\))*"))
		return f"({c})";
	return c;
}

local tokenStrings: {string: TokenString} = Dict();
function addTokenString(name: string, value: string, condition: string = "1") {
	local ts: TokenString | none = tokenStrings.get(value);
	if (ts is none) {
		ts = TokenString(name: name, value: value, condition: condition);
		tokenStrings[value] = ts;
	} else if (condition == "1") {
		ts.condition = "1";
	} else if (ts.condition != "1") {
		ts.condition = f"{parenCondition(ts.condition)} || {parenCondition(condition)}";
	}
}

for (local oneCharTokenName: oneCharTokenNames) {
	local symName;
	if (oneCharTokenName.issymbol() && oneCharTokenName != "$") {
		symName = oneCharTokenName;
	} else if (#oneCharTokenName == 1) {
		symName = f"ord_{oneCharTokenName.ord().tostr(16, 2)}";
	} else {
		symName = oneCharTokenName.replace("\\", "")
			.replace("<", "")
			.replace(">", "");
	}
	addTokenString(symName, oneCharTokenName);
}
for (local t: tokens)
	addTokenString(t.id, t.tokenRepr, t.condition);

print("static struct tpp_token_repr_strings_struct {");
for (local ts: tokenStrings.values) {
	if (ts.condition != "1")
		print(f"#if {ts.condition}");
	print(f"	char ttr_{ts.name}[{#ts.value + 1}];");
	if (ts.condition != "1")
		print(f"#endif /" f"* {ts.condition} *" f"/");
}
print("} const tpp_token_repr_strings = {");
for (local ts: tokenStrings.values) {
	if (ts.condition != "1")
		print(f"#if {ts.condition}");
	print(f"	/" f"* .ttr_{ts.name} = *" f"/ {repr ts.value},");
	if (ts.condition != "1")
		print(f"#endif /" f"* {ts.condition} *" f"/");
}
print("};");
print("static uint_least16_t const tpp_token_repr_offsets[] = {");
for (local i, value: oneCharTokenNames.enumerate()) {
	local ts = tokenStrings[value];
	print(f"	/" f"* [{i.hex()}] = *" f"/ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_{ts.name}),");
}
for (local t: tokens) {
	local ts = tokenStrings[t.tokenRepr];
	if (t.condition != "1")
		print(f"#if {t.condition}");
	print(f"	/" f"* [{t.id}] = *" f"/ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_{ts.name}),");
	if (t.condition != "1")
		print(f"#endif /" f"* {t.condition} *" f"/");
}
print("};");
print("TPP_STATIC_ASSERT(tpp_lengthof(tpp_token_repr_offsets) == TPP_TOK_MULTICHAR_END);");
]]]*/
static struct tpp_token_repr_strings_struct {
	char ttr_eof[6];
	char ttr_x01[5];
	char ttr_x02[5];
	char ttr_x03[5];
	char ttr_x04[5];
	char ttr_x05[5];
	char ttr_x06[5];
	char ttr_x07[5];
	char ttr_x08[5];
	char ttr_x09[5];
	char ttr_linefeed[11];
	char ttr_x0b[5];
	char ttr_x0c[5];
	char ttr_x0d[5];
	char ttr_x0e[5];
	char ttr_x0f[5];
	char ttr_x10[5];
	char ttr_x11[5];
	char ttr_x12[5];
	char ttr_x13[5];
	char ttr_x14[5];
	char ttr_x15[5];
	char ttr_x16[5];
	char ttr_x17[5];
	char ttr_x18[5];
	char ttr_x19[5];
	char ttr_x1a[5];
	char ttr_x1b[5];
	char ttr_x1c[5];
	char ttr_x1d[5];
	char ttr_x1e[5];
	char ttr_x1f[5];
	char ttr_space[8];
	char ttr_ord_21[2];
	char ttr_ord_22[2];
	char ttr_ord_23[2];
	char ttr_ord_24[2];
	char ttr_ord_25[2];
	char ttr_ord_26[2];
	char ttr_ord_27[2];
	char ttr_ord_28[2];
	char ttr_ord_29[2];
	char ttr_ord_2a[2];
	char ttr_ord_2b[2];
	char ttr_ord_2c[2];
	char ttr_ord_2d[2];
	char ttr_ord_2e[2];
	char ttr_ord_2f[2];
	char ttr_ord_30[2];
	char ttr_ord_31[2];
	char ttr_ord_32[2];
	char ttr_ord_33[2];
	char ttr_ord_34[2];
	char ttr_ord_35[2];
	char ttr_ord_36[2];
	char ttr_ord_37[2];
	char ttr_ord_38[2];
	char ttr_ord_39[2];
	char ttr_ord_3a[2];
	char ttr_ord_3b[2];
	char ttr_ord_3c[2];
	char ttr_ord_3d[2];
	char ttr_ord_3e[2];
	char ttr_ord_3f[2];
	char ttr_ord_40[2];
	char ttr_A[2];
	char ttr_B[2];
	char ttr_C[2];
	char ttr_D[2];
	char ttr_E[2];
	char ttr_F[2];
	char ttr_G[2];
	char ttr_H[2];
	char ttr_I[2];
	char ttr_J[2];
	char ttr_K[2];
	char ttr_L[2];
	char ttr_M[2];
	char ttr_N[2];
	char ttr_O[2];
	char ttr_P[2];
	char ttr_Q[2];
	char ttr_R[2];
	char ttr_S[2];
	char ttr_T[2];
	char ttr_U[2];
	char ttr_V[2];
	char ttr_W[2];
	char ttr_X[2];
	char ttr_Y[2];
	char ttr_Z[2];
	char ttr_ord_5b[2];
	char ttr_ord_5c[2];
	char ttr_ord_5d[2];
	char ttr_ord_5e[2];
	char ttr__[2];
	char ttr_ord_60[2];
	char ttr_a[2];
	char ttr_b[2];
	char ttr_c[2];
	char ttr_d[2];
	char ttr_e[2];
	char ttr_f[2];
	char ttr_g[2];
	char ttr_h[2];
	char ttr_i[2];
	char ttr_j[2];
	char ttr_k[2];
	char ttr_l[2];
	char ttr_m[2];
	char ttr_n[2];
	char ttr_o[2];
	char ttr_p[2];
	char ttr_q[2];
	char ttr_r[2];
	char ttr_s[2];
	char ttr_t[2];
	char ttr_u[2];
	char ttr_v[2];
	char ttr_w[2];
	char ttr_x[2];
	char ttr_y[2];
	char ttr_z[2];
	char ttr_ord_7b[2];
	char ttr_ord_7c[2];
	char ttr_ord_7d[2];
	char ttr_ord_7e[2];
	char ttr_x7f[5];
	char ttr_x80[5];
	char ttr_x81[5];
	char ttr_x82[5];
	char ttr_x83[5];
	char ttr_x84[5];
	char ttr_x85[5];
	char ttr_x86[5];
	char ttr_x87[5];
	char ttr_x88[5];
	char ttr_x89[5];
	char ttr_x8a[5];
	char ttr_x8b[5];
	char ttr_x8c[5];
	char ttr_x8d[5];
	char ttr_x8e[5];
	char ttr_x8f[5];
	char ttr_x90[5];
	char ttr_x91[5];
	char ttr_x92[5];
	char ttr_x93[5];
	char ttr_x94[5];
	char ttr_x95[5];
	char ttr_x96[5];
	char ttr_x97[5];
	char ttr_x98[5];
	char ttr_x99[5];
	char ttr_x9a[5];
	char ttr_x9b[5];
	char ttr_x9c[5];
	char ttr_x9d[5];
	char ttr_x9e[5];
	char ttr_x9f[5];
	char ttr_xa0[5];
	char ttr_xa1[5];
	char ttr_xa2[5];
	char ttr_xa3[5];
	char ttr_xa4[5];
	char ttr_xa5[5];
	char ttr_xa6[5];
	char ttr_xa7[5];
	char ttr_xa8[5];
	char ttr_xa9[5];
	char ttr_xaa[5];
	char ttr_xab[5];
	char ttr_xac[5];
	char ttr_xad[5];
	char ttr_xae[5];
	char ttr_xaf[5];
	char ttr_xb0[5];
	char ttr_xb1[5];
	char ttr_xb2[5];
	char ttr_xb3[5];
	char ttr_xb4[5];
	char ttr_xb5[5];
	char ttr_xb6[5];
	char ttr_xb7[5];
	char ttr_xb8[5];
	char ttr_xb9[5];
	char ttr_xba[5];
	char ttr_xbb[5];
	char ttr_xbc[5];
	char ttr_xbd[5];
	char ttr_xbe[5];
	char ttr_xbf[5];
	char ttr_xc0[5];
	char ttr_xc1[5];
	char ttr_xc2[5];
	char ttr_xc3[5];
	char ttr_xc4[5];
	char ttr_xc5[5];
	char ttr_xc6[5];
	char ttr_xc7[5];
	char ttr_xc8[5];
	char ttr_xc9[5];
	char ttr_xca[5];
	char ttr_xcb[5];
	char ttr_xcc[5];
	char ttr_xcd[5];
	char ttr_xce[5];
	char ttr_xcf[5];
	char ttr_xd0[5];
	char ttr_xd1[5];
	char ttr_xd2[5];
	char ttr_xd3[5];
	char ttr_xd4[5];
	char ttr_xd5[5];
	char ttr_xd6[5];
	char ttr_xd7[5];
	char ttr_xd8[5];
	char ttr_xd9[5];
	char ttr_xda[5];
	char ttr_xdb[5];
	char ttr_xdc[5];
	char ttr_xdd[5];
	char ttr_xde[5];
	char ttr_xdf[5];
	char ttr_xe0[5];
	char ttr_xe1[5];
	char ttr_xe2[5];
	char ttr_xe3[5];
	char ttr_xe4[5];
	char ttr_xe5[5];
	char ttr_xe6[5];
	char ttr_xe7[5];
	char ttr_xe8[5];
	char ttr_xe9[5];
	char ttr_xea[5];
	char ttr_xeb[5];
	char ttr_xec[5];
	char ttr_xed[5];
	char ttr_xee[5];
	char ttr_xef[5];
	char ttr_xf0[5];
	char ttr_xf1[5];
	char ttr_xf2[5];
	char ttr_xf3[5];
	char ttr_xf4[5];
	char ttr_xf5[5];
	char ttr_xf6[5];
	char ttr_xf7[5];
	char ttr_xf8[5];
	char ttr_xf9[5];
	char ttr_xfa[5];
	char ttr_xfb[5];
	char ttr_xfc[5];
	char ttr_xfd[5];
	char ttr_xfe[5];
	char ttr_xff[5];
#if TPP_HAVE_UNICODE
	char ttr_TPP_TOK_UNICHAR[20];
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	char ttr_TPP_TOK_INT[10];
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	char ttr_TPP_TOK_FLOAT[8];
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT)
	char ttr_TPP_TOK_C_COMMENT[10];
#endif /* (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT) */
#if (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL)
	char ttr_TPP_TOK_STRING[9];
#endif /* (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL) */
#if (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
	char ttr_TPP_TOK_CHAR[7];
#endif /* (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL) */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	char ttr_TPP_TOK_EXCLAIM_EXCLAIM[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	char ttr_TPP_TOK_EXCLAIM_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	char ttr_TPP_TOK_EXCLAIM_EQUAL_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND && TPP_HAVE_TPP_TOK_POUND_POUND
	char ttr_TPP_TOK_POUND_POUND[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND && TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT
	char ttr_TPP_TOK_PERCENT_PERCENT[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL
	char ttr_TPP_TOK_PERCENT_PERCENT_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	char ttr_TPP_TOK_PERCENT_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_AMP
	char ttr_TPP_TOK_AMP_AMP[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_EQUAL
	char ttr_TPP_TOK_AMP_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR
	char ttr_TPP_TOK_STAR_STAR[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	char ttr_TPP_TOK_STAR_STAR_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_DOT
	char ttr_TPP_TOK_STAR_DOT[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	char ttr_TPP_TOK_STAR_LANGLE_MINUS[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_EQUAL
	char ttr_TPP_TOK_STAR_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_PLUS
	char ttr_TPP_TOK_PLUS_PLUS[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_EQUAL
	char ttr_TPP_TOK_PLUS_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_MINUS
	char ttr_TPP_TOK_MINUS_MINUS[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE
	char ttr_TPP_TOK_MINUS_LANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE
	char ttr_TPP_TOK_MINUS_LANGLE_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
	char ttr_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE[5];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_EQUAL
	char ttr_TPP_TOK_MINUS_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE
	char ttr_TPP_TOK_MINUS_RANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	char ttr_TPP_TOK_MINUS_RANGLE_STAR[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE
	char ttr_TPP_TOK_MINUS_RANGLE_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
	char ttr_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE[5];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_STAR
	char ttr_TPP_TOK_DOT_STAR[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT
	char ttr_TPP_TOK_DOT_DOT[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	char ttr_TPP_TOK_DOT_DOT_DOT[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH
	char ttr_TPP_TOK_SLASH_SLASH[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	char ttr_TPP_TOK_SLASH_SLASH_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_EQUAL
	char ttr_TPP_TOK_SLASH_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_COLON
	char ttr_TPP_TOK_COLON_COLON[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_EQUAL
	char ttr_TPP_TOK_COLON_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS
	char ttr_TPP_TOK_LANGLE_MINUS[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	char ttr_TPP_TOK_LANGLE_MINUS_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	char ttr_TPP_TOK_LANGLE_MINUS_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	char ttr_TPP_TOK_LANGLE_LANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS
	char ttr_TPP_TOK_LANGLE_LANGLE_MINUS[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	char ttr_TPP_TOK_LANGLE_LANGLE_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS
	char ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS[5];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	char ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL[5];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	char ttr_TPP_TOK_LANGLE_LANGLE_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	char ttr_TPP_TOK_LANGLE_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	char ttr_TPP_TOK_LANGLE_EQUAL_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	char ttr_TPP_TOK_LANGLE_EQUAL_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	char ttr_TPP_TOK_LANGLE_RANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
	char ttr_TPP_TOK_EQUAL_EXCLAIM[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT
	char ttr_TPP_TOK_EQUAL_PERCENT[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT
	char ttr_TPP_TOK_EQUAL_PERCENT_PERCENT[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AMP
	char ttr_TPP_TOK_EQUAL_AMP[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR
	char ttr_TPP_TOK_EQUAL_STAR[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
	char ttr_TPP_TOK_EQUAL_STAR_STAR[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PLUS
	char ttr_TPP_TOK_EQUAL_PLUS[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_MINUS
	char ttr_TPP_TOK_EQUAL_MINUS[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH
	char ttr_TPP_TOK_EQUAL_SLASH[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
	char ttr_TPP_TOK_EQUAL_SLASH_SLASH[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_COLON
	char ttr_TPP_TOK_EQUAL_COLON[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	char ttr_TPP_TOK_EQUAL_LANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	char ttr_TPP_TOK_EQUAL_LANGLE_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	char ttr_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE[5];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	char ttr_TPP_TOK_EQUAL_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	char ttr_TPP_TOK_EQUAL_EQUAL_EXCLAIM[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	char ttr_TPP_TOK_EQUAL_EQUAL_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	char ttr_TPP_TOK_EQUAL_RANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	char ttr_TPP_TOK_EQUAL_RANGLE_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	char ttr_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE[5];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_QMARK
	char ttr_TPP_TOK_EQUAL_QMARK[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT
	char ttr_TPP_TOK_EQUAL_AT[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT_AT
	char ttr_TPP_TOK_EQUAL_AT_AT[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT_AT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_HAT
	char ttr_TPP_TOK_EQUAL_HAT[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PIPE
	char ttr_TPP_TOK_EQUAL_PIPE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_TILDE
	char ttr_TPP_TOK_EQUAL_TILDE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS
	char ttr_TPP_TOK_RANGLE_MINUS[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	char ttr_TPP_TOK_RANGLE_MINUS_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	char ttr_TPP_TOK_RANGLE_MINUS_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	char ttr_TPP_TOK_RANGLE_LANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	char ttr_TPP_TOK_RANGLE_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	char ttr_TPP_TOK_RANGLE_EQUAL_LANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	char ttr_TPP_TOK_RANGLE_EQUAL_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	char ttr_TPP_TOK_RANGLE_RANGLE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS
	char ttr_TPP_TOK_RANGLE_RANGLE_MINUS[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	char ttr_TPP_TOK_RANGLE_RANGLE_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	char ttr_TPP_TOK_RANGLE_RANGLE_RANGLE[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS
	char ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS[5];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	char ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL[5];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_EQUAL
	char ttr_TPP_TOK_QMARK_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_QMARK
	char ttr_TPP_TOK_QMARK_QMARK[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_EQUAL
	char ttr_TPP_TOK_AT_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT
	char ttr_TPP_TOK_AT_AT[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT_EQUAL
	char ttr_TPP_TOK_AT_AT_EQUAL[4];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_EQUAL
	char ttr_TPP_TOK_HAT_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_HAT
	char ttr_TPP_TOK_HAT_HAT[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_EQUAL
	char ttr_TPP_TOK_PIPE_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_PIPE
	char ttr_TPP_TOK_PIPE_PIPE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_EQUAL
	char ttr_TPP_TOK_TILDE_EQUAL[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_TILDE
	char ttr_TPP_TOK_TILDE_TILDE[3];
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_TILDE */
} const tpp_token_repr_strings = {
	/* .ttr_eof = */ "<eof>",
	/* .ttr_x01 = */ "\\x01",
	/* .ttr_x02 = */ "\\x02",
	/* .ttr_x03 = */ "\\x03",
	/* .ttr_x04 = */ "\\x04",
	/* .ttr_x05 = */ "\\x05",
	/* .ttr_x06 = */ "\\x06",
	/* .ttr_x07 = */ "\\x07",
	/* .ttr_x08 = */ "\\x08",
	/* .ttr_x09 = */ "\\x09",
	/* .ttr_linefeed = */ "<linefeed>",
	/* .ttr_x0b = */ "\\x0b",
	/* .ttr_x0c = */ "\\x0c",
	/* .ttr_x0d = */ "\\x0d",
	/* .ttr_x0e = */ "\\x0e",
	/* .ttr_x0f = */ "\\x0f",
	/* .ttr_x10 = */ "\\x10",
	/* .ttr_x11 = */ "\\x11",
	/* .ttr_x12 = */ "\\x12",
	/* .ttr_x13 = */ "\\x13",
	/* .ttr_x14 = */ "\\x14",
	/* .ttr_x15 = */ "\\x15",
	/* .ttr_x16 = */ "\\x16",
	/* .ttr_x17 = */ "\\x17",
	/* .ttr_x18 = */ "\\x18",
	/* .ttr_x19 = */ "\\x19",
	/* .ttr_x1a = */ "\\x1a",
	/* .ttr_x1b = */ "\\x1b",
	/* .ttr_x1c = */ "\\x1c",
	/* .ttr_x1d = */ "\\x1d",
	/* .ttr_x1e = */ "\\x1e",
	/* .ttr_x1f = */ "\\x1f",
	/* .ttr_space = */ "<space>",
	/* .ttr_ord_21 = */ "!",
	/* .ttr_ord_22 = */ "\"",
	/* .ttr_ord_23 = */ "#",
	/* .ttr_ord_24 = */ "$",
	/* .ttr_ord_25 = */ "%",
	/* .ttr_ord_26 = */ "&",
	/* .ttr_ord_27 = */ "\'",
	/* .ttr_ord_28 = */ "(",
	/* .ttr_ord_29 = */ ")",
	/* .ttr_ord_2a = */ "*",
	/* .ttr_ord_2b = */ "+",
	/* .ttr_ord_2c = */ ",",
	/* .ttr_ord_2d = */ "-",
	/* .ttr_ord_2e = */ ".",
	/* .ttr_ord_2f = */ "/",
	/* .ttr_ord_30 = */ "0",
	/* .ttr_ord_31 = */ "1",
	/* .ttr_ord_32 = */ "2",
	/* .ttr_ord_33 = */ "3",
	/* .ttr_ord_34 = */ "4",
	/* .ttr_ord_35 = */ "5",
	/* .ttr_ord_36 = */ "6",
	/* .ttr_ord_37 = */ "7",
	/* .ttr_ord_38 = */ "8",
	/* .ttr_ord_39 = */ "9",
	/* .ttr_ord_3a = */ ":",
	/* .ttr_ord_3b = */ ";",
	/* .ttr_ord_3c = */ "<",
	/* .ttr_ord_3d = */ "=",
	/* .ttr_ord_3e = */ ">",
	/* .ttr_ord_3f = */ "?",
	/* .ttr_ord_40 = */ "@",
	/* .ttr_A = */ "A",
	/* .ttr_B = */ "B",
	/* .ttr_C = */ "C",
	/* .ttr_D = */ "D",
	/* .ttr_E = */ "E",
	/* .ttr_F = */ "F",
	/* .ttr_G = */ "G",
	/* .ttr_H = */ "H",
	/* .ttr_I = */ "I",
	/* .ttr_J = */ "J",
	/* .ttr_K = */ "K",
	/* .ttr_L = */ "L",
	/* .ttr_M = */ "M",
	/* .ttr_N = */ "N",
	/* .ttr_O = */ "O",
	/* .ttr_P = */ "P",
	/* .ttr_Q = */ "Q",
	/* .ttr_R = */ "R",
	/* .ttr_S = */ "S",
	/* .ttr_T = */ "T",
	/* .ttr_U = */ "U",
	/* .ttr_V = */ "V",
	/* .ttr_W = */ "W",
	/* .ttr_X = */ "X",
	/* .ttr_Y = */ "Y",
	/* .ttr_Z = */ "Z",
	/* .ttr_ord_5b = */ "[",
	/* .ttr_ord_5c = */ "\\",
	/* .ttr_ord_5d = */ "]",
	/* .ttr_ord_5e = */ "^",
	/* .ttr__ = */ "_",
	/* .ttr_ord_60 = */ "`",
	/* .ttr_a = */ "a",
	/* .ttr_b = */ "b",
	/* .ttr_c = */ "c",
	/* .ttr_d = */ "d",
	/* .ttr_e = */ "e",
	/* .ttr_f = */ "f",
	/* .ttr_g = */ "g",
	/* .ttr_h = */ "h",
	/* .ttr_i = */ "i",
	/* .ttr_j = */ "j",
	/* .ttr_k = */ "k",
	/* .ttr_l = */ "l",
	/* .ttr_m = */ "m",
	/* .ttr_n = */ "n",
	/* .ttr_o = */ "o",
	/* .ttr_p = */ "p",
	/* .ttr_q = */ "q",
	/* .ttr_r = */ "r",
	/* .ttr_s = */ "s",
	/* .ttr_t = */ "t",
	/* .ttr_u = */ "u",
	/* .ttr_v = */ "v",
	/* .ttr_w = */ "w",
	/* .ttr_x = */ "x",
	/* .ttr_y = */ "y",
	/* .ttr_z = */ "z",
	/* .ttr_ord_7b = */ "{",
	/* .ttr_ord_7c = */ "|",
	/* .ttr_ord_7d = */ "}",
	/* .ttr_ord_7e = */ "~",
	/* .ttr_x7f = */ "\\x7f",
	/* .ttr_x80 = */ "\\x80",
	/* .ttr_x81 = */ "\\x81",
	/* .ttr_x82 = */ "\\x82",
	/* .ttr_x83 = */ "\\x83",
	/* .ttr_x84 = */ "\\x84",
	/* .ttr_x85 = */ "\\x85",
	/* .ttr_x86 = */ "\\x86",
	/* .ttr_x87 = */ "\\x87",
	/* .ttr_x88 = */ "\\x88",
	/* .ttr_x89 = */ "\\x89",
	/* .ttr_x8a = */ "\\x8a",
	/* .ttr_x8b = */ "\\x8b",
	/* .ttr_x8c = */ "\\x8c",
	/* .ttr_x8d = */ "\\x8d",
	/* .ttr_x8e = */ "\\x8e",
	/* .ttr_x8f = */ "\\x8f",
	/* .ttr_x90 = */ "\\x90",
	/* .ttr_x91 = */ "\\x91",
	/* .ttr_x92 = */ "\\x92",
	/* .ttr_x93 = */ "\\x93",
	/* .ttr_x94 = */ "\\x94",
	/* .ttr_x95 = */ "\\x95",
	/* .ttr_x96 = */ "\\x96",
	/* .ttr_x97 = */ "\\x97",
	/* .ttr_x98 = */ "\\x98",
	/* .ttr_x99 = */ "\\x99",
	/* .ttr_x9a = */ "\\x9a",
	/* .ttr_x9b = */ "\\x9b",
	/* .ttr_x9c = */ "\\x9c",
	/* .ttr_x9d = */ "\\x9d",
	/* .ttr_x9e = */ "\\x9e",
	/* .ttr_x9f = */ "\\x9f",
	/* .ttr_xa0 = */ "\\xa0",
	/* .ttr_xa1 = */ "\\xa1",
	/* .ttr_xa2 = */ "\\xa2",
	/* .ttr_xa3 = */ "\\xa3",
	/* .ttr_xa4 = */ "\\xa4",
	/* .ttr_xa5 = */ "\\xa5",
	/* .ttr_xa6 = */ "\\xa6",
	/* .ttr_xa7 = */ "\\xa7",
	/* .ttr_xa8 = */ "\\xa8",
	/* .ttr_xa9 = */ "\\xa9",
	/* .ttr_xaa = */ "\\xaa",
	/* .ttr_xab = */ "\\xab",
	/* .ttr_xac = */ "\\xac",
	/* .ttr_xad = */ "\\xad",
	/* .ttr_xae = */ "\\xae",
	/* .ttr_xaf = */ "\\xaf",
	/* .ttr_xb0 = */ "\\xb0",
	/* .ttr_xb1 = */ "\\xb1",
	/* .ttr_xb2 = */ "\\xb2",
	/* .ttr_xb3 = */ "\\xb3",
	/* .ttr_xb4 = */ "\\xb4",
	/* .ttr_xb5 = */ "\\xb5",
	/* .ttr_xb6 = */ "\\xb6",
	/* .ttr_xb7 = */ "\\xb7",
	/* .ttr_xb8 = */ "\\xb8",
	/* .ttr_xb9 = */ "\\xb9",
	/* .ttr_xba = */ "\\xba",
	/* .ttr_xbb = */ "\\xbb",
	/* .ttr_xbc = */ "\\xbc",
	/* .ttr_xbd = */ "\\xbd",
	/* .ttr_xbe = */ "\\xbe",
	/* .ttr_xbf = */ "\\xbf",
	/* .ttr_xc0 = */ "\\xc0",
	/* .ttr_xc1 = */ "\\xc1",
	/* .ttr_xc2 = */ "\\xc2",
	/* .ttr_xc3 = */ "\\xc3",
	/* .ttr_xc4 = */ "\\xc4",
	/* .ttr_xc5 = */ "\\xc5",
	/* .ttr_xc6 = */ "\\xc6",
	/* .ttr_xc7 = */ "\\xc7",
	/* .ttr_xc8 = */ "\\xc8",
	/* .ttr_xc9 = */ "\\xc9",
	/* .ttr_xca = */ "\\xca",
	/* .ttr_xcb = */ "\\xcb",
	/* .ttr_xcc = */ "\\xcc",
	/* .ttr_xcd = */ "\\xcd",
	/* .ttr_xce = */ "\\xce",
	/* .ttr_xcf = */ "\\xcf",
	/* .ttr_xd0 = */ "\\xd0",
	/* .ttr_xd1 = */ "\\xd1",
	/* .ttr_xd2 = */ "\\xd2",
	/* .ttr_xd3 = */ "\\xd3",
	/* .ttr_xd4 = */ "\\xd4",
	/* .ttr_xd5 = */ "\\xd5",
	/* .ttr_xd6 = */ "\\xd6",
	/* .ttr_xd7 = */ "\\xd7",
	/* .ttr_xd8 = */ "\\xd8",
	/* .ttr_xd9 = */ "\\xd9",
	/* .ttr_xda = */ "\\xda",
	/* .ttr_xdb = */ "\\xdb",
	/* .ttr_xdc = */ "\\xdc",
	/* .ttr_xdd = */ "\\xdd",
	/* .ttr_xde = */ "\\xde",
	/* .ttr_xdf = */ "\\xdf",
	/* .ttr_xe0 = */ "\\xe0",
	/* .ttr_xe1 = */ "\\xe1",
	/* .ttr_xe2 = */ "\\xe2",
	/* .ttr_xe3 = */ "\\xe3",
	/* .ttr_xe4 = */ "\\xe4",
	/* .ttr_xe5 = */ "\\xe5",
	/* .ttr_xe6 = */ "\\xe6",
	/* .ttr_xe7 = */ "\\xe7",
	/* .ttr_xe8 = */ "\\xe8",
	/* .ttr_xe9 = */ "\\xe9",
	/* .ttr_xea = */ "\\xea",
	/* .ttr_xeb = */ "\\xeb",
	/* .ttr_xec = */ "\\xec",
	/* .ttr_xed = */ "\\xed",
	/* .ttr_xee = */ "\\xee",
	/* .ttr_xef = */ "\\xef",
	/* .ttr_xf0 = */ "\\xf0",
	/* .ttr_xf1 = */ "\\xf1",
	/* .ttr_xf2 = */ "\\xf2",
	/* .ttr_xf3 = */ "\\xf3",
	/* .ttr_xf4 = */ "\\xf4",
	/* .ttr_xf5 = */ "\\xf5",
	/* .ttr_xf6 = */ "\\xf6",
	/* .ttr_xf7 = */ "\\xf7",
	/* .ttr_xf8 = */ "\\xf8",
	/* .ttr_xf9 = */ "\\xf9",
	/* .ttr_xfa = */ "\\xfa",
	/* .ttr_xfb = */ "\\xfb",
	/* .ttr_xfc = */ "\\xfc",
	/* .ttr_xfd = */ "\\xfd",
	/* .ttr_xfe = */ "\\xfe",
	/* .ttr_xff = */ "\\xff",
#if TPP_HAVE_UNICODE
	/* .ttr_TPP_TOK_UNICHAR = */ "<unicode character>",
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	/* .ttr_TPP_TOK_INT = */ "<integer>",
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	/* .ttr_TPP_TOK_FLOAT = */ "<float>",
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT)
	/* .ttr_TPP_TOK_C_COMMENT = */ "<comment>",
#endif /* (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT) || (TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT) */
#if (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL)
	/* .ttr_TPP_TOK_STRING = */ "<string>",
#endif /* (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL) */
#if (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL)
	/* .ttr_TPP_TOK_CHAR = */ "<char>",
#endif /* (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL) || (TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL) */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	/* .ttr_TPP_TOK_EXCLAIM_EXCLAIM = */ "!!",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	/* .ttr_TPP_TOK_EXCLAIM_EQUAL = */ "!=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	/* .ttr_TPP_TOK_EXCLAIM_EQUAL_EQUAL = */ "!==",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND && TPP_HAVE_TPP_TOK_POUND_POUND
	/* .ttr_TPP_TOK_POUND_POUND = */ "##",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND && TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT
	/* .ttr_TPP_TOK_PERCENT_PERCENT = */ "%%",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL
	/* .ttr_TPP_TOK_PERCENT_PERCENT_EQUAL = */ "%%=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	/* .ttr_TPP_TOK_PERCENT_EQUAL = */ "%=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_AMP
	/* .ttr_TPP_TOK_AMP_AMP = */ "&&",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_EQUAL
	/* .ttr_TPP_TOK_AMP_EQUAL = */ "&=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR
	/* .ttr_TPP_TOK_STAR_STAR = */ "**",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	/* .ttr_TPP_TOK_STAR_STAR_EQUAL = */ "**=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_DOT
	/* .ttr_TPP_TOK_STAR_DOT = */ "*.",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	/* .ttr_TPP_TOK_STAR_LANGLE_MINUS = */ "*<-",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_EQUAL
	/* .ttr_TPP_TOK_STAR_EQUAL = */ "*=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_PLUS
	/* .ttr_TPP_TOK_PLUS_PLUS = */ "++",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_EQUAL
	/* .ttr_TPP_TOK_PLUS_EQUAL = */ "+=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_MINUS
	/* .ttr_TPP_TOK_MINUS_MINUS = */ "--",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE
	/* .ttr_TPP_TOK_MINUS_LANGLE = */ "-<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE
	/* .ttr_TPP_TOK_MINUS_LANGLE_LANGLE = */ "-<<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
	/* .ttr_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE = */ "-<<<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_EQUAL
	/* .ttr_TPP_TOK_MINUS_EQUAL = */ "-=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE
	/* .ttr_TPP_TOK_MINUS_RANGLE = */ "->",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	/* .ttr_TPP_TOK_MINUS_RANGLE_STAR = */ "->*",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE
	/* .ttr_TPP_TOK_MINUS_RANGLE_RANGLE = */ "->>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
	/* .ttr_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE = */ "->>>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_STAR
	/* .ttr_TPP_TOK_DOT_STAR = */ ".*",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT
	/* .ttr_TPP_TOK_DOT_DOT = */ "..",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	/* .ttr_TPP_TOK_DOT_DOT_DOT = */ "...",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH
	/* .ttr_TPP_TOK_SLASH_SLASH = */ "//",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	/* .ttr_TPP_TOK_SLASH_SLASH_EQUAL = */ "//=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_EQUAL
	/* .ttr_TPP_TOK_SLASH_EQUAL = */ "/=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_COLON
	/* .ttr_TPP_TOK_COLON_COLON = */ "::",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_EQUAL
	/* .ttr_TPP_TOK_COLON_EQUAL = */ ":=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS
	/* .ttr_TPP_TOK_LANGLE_MINUS = */ "<-",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	/* .ttr_TPP_TOK_LANGLE_MINUS_LANGLE = */ "<-<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	/* .ttr_TPP_TOK_LANGLE_MINUS_RANGLE = */ "<->",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	/* .ttr_TPP_TOK_LANGLE_LANGLE = */ "<<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS
	/* .ttr_TPP_TOK_LANGLE_LANGLE_MINUS = */ "<<-",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	/* .ttr_TPP_TOK_LANGLE_LANGLE_LANGLE = */ "<<<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS
	/* .ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS = */ "<<<-",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	/* .ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL = */ "<<<=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	/* .ttr_TPP_TOK_LANGLE_LANGLE_EQUAL = */ "<<=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	/* .ttr_TPP_TOK_LANGLE_EQUAL = */ "<=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	/* .ttr_TPP_TOK_LANGLE_EQUAL_LANGLE = */ "<=<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	/* .ttr_TPP_TOK_LANGLE_EQUAL_RANGLE = */ "<=>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	/* .ttr_TPP_TOK_LANGLE_RANGLE = */ "<>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
	/* .ttr_TPP_TOK_EQUAL_EXCLAIM = */ "=!",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT
	/* .ttr_TPP_TOK_EQUAL_PERCENT = */ "=%",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT
	/* .ttr_TPP_TOK_EQUAL_PERCENT_PERCENT = */ "=%%",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AMP
	/* .ttr_TPP_TOK_EQUAL_AMP = */ "=&",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR
	/* .ttr_TPP_TOK_EQUAL_STAR = */ "=*",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
	/* .ttr_TPP_TOK_EQUAL_STAR_STAR = */ "=**",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PLUS
	/* .ttr_TPP_TOK_EQUAL_PLUS = */ "=+",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_MINUS
	/* .ttr_TPP_TOK_EQUAL_MINUS = */ "=-",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH
	/* .ttr_TPP_TOK_EQUAL_SLASH = */ "=/",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
	/* .ttr_TPP_TOK_EQUAL_SLASH_SLASH = */ "=//",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_COLON
	/* .ttr_TPP_TOK_EQUAL_COLON = */ "=:",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	/* .ttr_TPP_TOK_EQUAL_LANGLE = */ "=<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	/* .ttr_TPP_TOK_EQUAL_LANGLE_LANGLE = */ "=<<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	/* .ttr_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE = */ "=<<<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	/* .ttr_TPP_TOK_EQUAL_EQUAL = */ "==",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	/* .ttr_TPP_TOK_EQUAL_EQUAL_EXCLAIM = */ "==!",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	/* .ttr_TPP_TOK_EQUAL_EQUAL_EQUAL = */ "===",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	/* .ttr_TPP_TOK_EQUAL_RANGLE = */ "=>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	/* .ttr_TPP_TOK_EQUAL_RANGLE_RANGLE = */ "=>>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	/* .ttr_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE = */ "=>>>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_QMARK
	/* .ttr_TPP_TOK_EQUAL_QMARK = */ "=?",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT
	/* .ttr_TPP_TOK_EQUAL_AT = */ "=@",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT_AT
	/* .ttr_TPP_TOK_EQUAL_AT_AT = */ "=@@",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT_AT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_HAT
	/* .ttr_TPP_TOK_EQUAL_HAT = */ "=^",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PIPE
	/* .ttr_TPP_TOK_EQUAL_PIPE = */ "=|",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_TILDE
	/* .ttr_TPP_TOK_EQUAL_TILDE = */ "=~",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS
	/* .ttr_TPP_TOK_RANGLE_MINUS = */ ">-",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	/* .ttr_TPP_TOK_RANGLE_MINUS_LANGLE = */ ">-<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	/* .ttr_TPP_TOK_RANGLE_MINUS_RANGLE = */ ">->",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	/* .ttr_TPP_TOK_RANGLE_LANGLE = */ "><",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	/* .ttr_TPP_TOK_RANGLE_EQUAL = */ ">=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	/* .ttr_TPP_TOK_RANGLE_EQUAL_LANGLE = */ ">=<",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	/* .ttr_TPP_TOK_RANGLE_EQUAL_RANGLE = */ ">=>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	/* .ttr_TPP_TOK_RANGLE_RANGLE = */ ">>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS
	/* .ttr_TPP_TOK_RANGLE_RANGLE_MINUS = */ ">>-",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	/* .ttr_TPP_TOK_RANGLE_RANGLE_EQUAL = */ ">>=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	/* .ttr_TPP_TOK_RANGLE_RANGLE_RANGLE = */ ">>>",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS
	/* .ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS = */ ">>>-",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	/* .ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL = */ ">>>=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_EQUAL
	/* .ttr_TPP_TOK_QMARK_EQUAL = */ "?=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_QMARK
	/* .ttr_TPP_TOK_QMARK_QMARK = */ "??",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_EQUAL
	/* .ttr_TPP_TOK_AT_EQUAL = */ "@=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT
	/* .ttr_TPP_TOK_AT_AT = */ "@@",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT_EQUAL
	/* .ttr_TPP_TOK_AT_AT_EQUAL = */ "@@=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_EQUAL
	/* .ttr_TPP_TOK_HAT_EQUAL = */ "^=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_HAT
	/* .ttr_TPP_TOK_HAT_HAT = */ "^^",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_EQUAL
	/* .ttr_TPP_TOK_PIPE_EQUAL = */ "|=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_PIPE
	/* .ttr_TPP_TOK_PIPE_PIPE = */ "||",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_EQUAL
	/* .ttr_TPP_TOK_TILDE_EQUAL = */ "~=",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_TILDE
	/* .ttr_TPP_TOK_TILDE_TILDE = */ "~~",
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_TILDE */
};
static uint_least16_t const tpp_token_repr_offsets[] = {
	/* [0x0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_eof),
	/* [0x1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x01),
	/* [0x2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x02),
	/* [0x3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x03),
	/* [0x4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x04),
	/* [0x5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x05),
	/* [0x6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x06),
	/* [0x7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x07),
	/* [0x8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x08),
	/* [0x9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x09),
	/* [0xa] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_linefeed),
	/* [0xb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0b),
	/* [0xc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0c),
	/* [0xd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0d),
	/* [0xe] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0e),
	/* [0xf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x0f),
	/* [0x10] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x10),
	/* [0x11] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x11),
	/* [0x12] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x12),
	/* [0x13] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x13),
	/* [0x14] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x14),
	/* [0x15] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x15),
	/* [0x16] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x16),
	/* [0x17] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x17),
	/* [0x18] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x18),
	/* [0x19] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x19),
	/* [0x1a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1a),
	/* [0x1b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1b),
	/* [0x1c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1c),
	/* [0x1d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1d),
	/* [0x1e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1e),
	/* [0x1f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x1f),
	/* [0x20] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_space),
	/* [0x21] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_21),
	/* [0x22] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_22),
	/* [0x23] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_23),
	/* [0x24] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_24),
	/* [0x25] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_25),
	/* [0x26] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_26),
	/* [0x27] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_27),
	/* [0x28] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_28),
	/* [0x29] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_29),
	/* [0x2a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2a),
	/* [0x2b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2b),
	/* [0x2c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2c),
	/* [0x2d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2d),
	/* [0x2e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2e),
	/* [0x2f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_2f),
	/* [0x30] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_30),
	/* [0x31] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_31),
	/* [0x32] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_32),
	/* [0x33] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_33),
	/* [0x34] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_34),
	/* [0x35] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_35),
	/* [0x36] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_36),
	/* [0x37] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_37),
	/* [0x38] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_38),
	/* [0x39] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_39),
	/* [0x3a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3a),
	/* [0x3b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3b),
	/* [0x3c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3c),
	/* [0x3d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3d),
	/* [0x3e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3e),
	/* [0x3f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_3f),
	/* [0x40] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_40),
	/* [0x41] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_A),
	/* [0x42] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_B),
	/* [0x43] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_C),
	/* [0x44] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_D),
	/* [0x45] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_E),
	/* [0x46] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_F),
	/* [0x47] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_G),
	/* [0x48] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_H),
	/* [0x49] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_I),
	/* [0x4a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_J),
	/* [0x4b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_K),
	/* [0x4c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_L),
	/* [0x4d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_M),
	/* [0x4e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_N),
	/* [0x4f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_O),
	/* [0x50] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_P),
	/* [0x51] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_Q),
	/* [0x52] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_R),
	/* [0x53] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_S),
	/* [0x54] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_T),
	/* [0x55] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_U),
	/* [0x56] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_V),
	/* [0x57] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_W),
	/* [0x58] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_X),
	/* [0x59] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_Y),
	/* [0x5a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_Z),
	/* [0x5b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_5b),
	/* [0x5c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_5c),
	/* [0x5d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_5d),
	/* [0x5e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_5e),
	/* [0x5f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr__),
	/* [0x60] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_60),
	/* [0x61] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_a),
	/* [0x62] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_b),
	/* [0x63] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_c),
	/* [0x64] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_d),
	/* [0x65] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_e),
	/* [0x66] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_f),
	/* [0x67] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_g),
	/* [0x68] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_h),
	/* [0x69] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_i),
	/* [0x6a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_j),
	/* [0x6b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_k),
	/* [0x6c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_l),
	/* [0x6d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_m),
	/* [0x6e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_n),
	/* [0x6f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_o),
	/* [0x70] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_p),
	/* [0x71] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_q),
	/* [0x72] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_r),
	/* [0x73] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_s),
	/* [0x74] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_t),
	/* [0x75] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_u),
	/* [0x76] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_v),
	/* [0x77] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_w),
	/* [0x78] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x),
	/* [0x79] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_y),
	/* [0x7a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_z),
	/* [0x7b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_7b),
	/* [0x7c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_7c),
	/* [0x7d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_7d),
	/* [0x7e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_ord_7e),
	/* [0x7f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x7f),
	/* [0x80] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x80),
	/* [0x81] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x81),
	/* [0x82] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x82),
	/* [0x83] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x83),
	/* [0x84] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x84),
	/* [0x85] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x85),
	/* [0x86] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x86),
	/* [0x87] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x87),
	/* [0x88] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x88),
	/* [0x89] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x89),
	/* [0x8a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8a),
	/* [0x8b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8b),
	/* [0x8c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8c),
	/* [0x8d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8d),
	/* [0x8e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8e),
	/* [0x8f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x8f),
	/* [0x90] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x90),
	/* [0x91] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x91),
	/* [0x92] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x92),
	/* [0x93] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x93),
	/* [0x94] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x94),
	/* [0x95] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x95),
	/* [0x96] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x96),
	/* [0x97] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x97),
	/* [0x98] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x98),
	/* [0x99] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x99),
	/* [0x9a] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9a),
	/* [0x9b] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9b),
	/* [0x9c] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9c),
	/* [0x9d] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9d),
	/* [0x9e] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9e),
	/* [0x9f] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_x9f),
	/* [0xa0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa0),
	/* [0xa1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa1),
	/* [0xa2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa2),
	/* [0xa3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa3),
	/* [0xa4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa4),
	/* [0xa5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa5),
	/* [0xa6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa6),
	/* [0xa7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa7),
	/* [0xa8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa8),
	/* [0xa9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xa9),
	/* [0xaa] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xaa),
	/* [0xab] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xab),
	/* [0xac] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xac),
	/* [0xad] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xad),
	/* [0xae] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xae),
	/* [0xaf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xaf),
	/* [0xb0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb0),
	/* [0xb1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb1),
	/* [0xb2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb2),
	/* [0xb3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb3),
	/* [0xb4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb4),
	/* [0xb5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb5),
	/* [0xb6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb6),
	/* [0xb7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb7),
	/* [0xb8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb8),
	/* [0xb9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xb9),
	/* [0xba] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xba),
	/* [0xbb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbb),
	/* [0xbc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbc),
	/* [0xbd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbd),
	/* [0xbe] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbe),
	/* [0xbf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xbf),
	/* [0xc0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc0),
	/* [0xc1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc1),
	/* [0xc2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc2),
	/* [0xc3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc3),
	/* [0xc4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc4),
	/* [0xc5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc5),
	/* [0xc6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc6),
	/* [0xc7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc7),
	/* [0xc8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc8),
	/* [0xc9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xc9),
	/* [0xca] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xca),
	/* [0xcb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xcb),
	/* [0xcc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xcc),
	/* [0xcd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xcd),
	/* [0xce] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xce),
	/* [0xcf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xcf),
	/* [0xd0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd0),
	/* [0xd1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd1),
	/* [0xd2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd2),
	/* [0xd3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd3),
	/* [0xd4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd4),
	/* [0xd5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd5),
	/* [0xd6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd6),
	/* [0xd7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd7),
	/* [0xd8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd8),
	/* [0xd9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xd9),
	/* [0xda] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xda),
	/* [0xdb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xdb),
	/* [0xdc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xdc),
	/* [0xdd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xdd),
	/* [0xde] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xde),
	/* [0xdf] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xdf),
	/* [0xe0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe0),
	/* [0xe1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe1),
	/* [0xe2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe2),
	/* [0xe3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe3),
	/* [0xe4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe4),
	/* [0xe5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe5),
	/* [0xe6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe6),
	/* [0xe7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe7),
	/* [0xe8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe8),
	/* [0xe9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xe9),
	/* [0xea] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xea),
	/* [0xeb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xeb),
	/* [0xec] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xec),
	/* [0xed] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xed),
	/* [0xee] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xee),
	/* [0xef] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xef),
	/* [0xf0] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf0),
	/* [0xf1] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf1),
	/* [0xf2] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf2),
	/* [0xf3] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf3),
	/* [0xf4] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf4),
	/* [0xf5] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf5),
	/* [0xf6] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf6),
	/* [0xf7] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf7),
	/* [0xf8] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf8),
	/* [0xf9] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xf9),
	/* [0xfa] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfa),
	/* [0xfb] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfb),
	/* [0xfc] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfc),
	/* [0xfd] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfd),
	/* [0xfe] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xfe),
	/* [0xff] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_xff),
#if TPP_HAVE_UNICODE
	/* [TPP_TOK_UNICHAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_UNICHAR),
#endif /* TPP_HAVE_UNICODE */
#if TPP_HAVE_TPP_TOK_INT
	/* [TPP_TOK_INT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_INT),
#endif /* TPP_HAVE_TPP_TOK_INT */
#if TPP_HAVE_TPP_TOK_FLOAT
	/* [TPP_TOK_FLOAT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_FLOAT),
#endif /* TPP_HAVE_TPP_TOK_FLOAT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT
	/* [TPP_TOK_C_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_C_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT
	/* [TPP_TOK_PASCAL_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_PASCAL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT
	/* [TPP_TOK_CXX_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_CXX_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT
	/* [TPP_TOK_SHELL_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SHELL_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT
	/* [TPP_TOK_ASM_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_ASM_COMMENT */
#if TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT
	/* [TPP_TOK_SQL_COMMENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_C_COMMENT),
#endif /* TPP_HAVE_TPP_TOK_COMMENTLIKE && TPP_HAVE_TPP_TOK_SQL_COMMENT */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING
	/* [TPP_TOK_STRING] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_STRING */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL
	/* [TPP_TOK_CXX_WIDE_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_WIDE_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL
	/* [TPP_TOK_CXX_UTF8_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF8_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL
	/* [TPP_TOK_CXX_UTF16_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF16_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL
	/* [TPP_TOK_CXX_UTF32_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF32_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_STRING_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL
	/* [TPP_TOK_RAW_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL
	/* [TPP_TOK_BLOCK_STRING_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_STRING_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR
	/* [TPP_TOK_CHAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CHAR */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL
	/* [TPP_TOK_CXX_WIDE_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_WIDE_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_WIDE_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL
	/* [TPP_TOK_CXX_UTF8_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF8_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF8_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL
	/* [TPP_TOK_CXX_UTF16_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF16_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF16_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL
	/* [TPP_TOK_CXX_UTF32_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL
	/* [TPP_TOK_CXX_RAW_UTF32_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_CXX_UTF32_CHAR_LITERAL && TPP_HAVE_TPP_TOK_CXX_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL
	/* [TPP_TOK_RAW_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_CHAR),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_RAW_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL
	/* [TPP_TOK_BLOCK_CHAR_LITERAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STRING),
#endif /* TPP_HAVE_TPP_TOK_STRINGLIKE && TPP_HAVE_TPP_TOK_BLOCK_CHAR_LITERAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM
	/* [TPP_TOK_EXCLAIM_EXCLAIM] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EXCLAIM_EXCLAIM),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL
	/* [TPP_TOK_EXCLAIM_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EXCLAIM_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL
	/* [TPP_TOK_EXCLAIM_EQUAL_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EXCLAIM_EQUAL_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EXCLAIM && TPP_HAVE_TPP_TOK_EXCLAIM_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND && TPP_HAVE_TPP_TOK_POUND_POUND
	/* [TPP_TOK_POUND_POUND] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_POUND_POUND),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_POUND && TPP_HAVE_TPP_TOK_POUND_POUND */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT
	/* [TPP_TOK_PERCENT_PERCENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PERCENT_PERCENT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL
	/* [TPP_TOK_PERCENT_PERCENT_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PERCENT_PERCENT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_EQUAL
	/* [TPP_TOK_PERCENT_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PERCENT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PERCENT && TPP_HAVE_TPP_TOK_PERCENT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_AMP
	/* [TPP_TOK_AMP_AMP] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_AMP_AMP),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_AMP */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_EQUAL
	/* [TPP_TOK_AMP_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_AMP_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AMP && TPP_HAVE_TPP_TOK_AMP_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR
	/* [TPP_TOK_STAR_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_STAR),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL
	/* [TPP_TOK_STAR_STAR_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_STAR_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_DOT
	/* [TPP_TOK_STAR_DOT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_DOT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_DOT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS
	/* [TPP_TOK_STAR_LANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_LANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_EQUAL
	/* [TPP_TOK_STAR_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_STAR_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_STAR && TPP_HAVE_TPP_TOK_STAR_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_PLUS
	/* [TPP_TOK_PLUS_PLUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PLUS_PLUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_PLUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_EQUAL
	/* [TPP_TOK_PLUS_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PLUS_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PLUS && TPP_HAVE_TPP_TOK_PLUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_MINUS
	/* [TPP_TOK_MINUS_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE
	/* [TPP_TOK_MINUS_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE
	/* [TPP_TOK_MINUS_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE
	/* [TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_EQUAL
	/* [TPP_TOK_MINUS_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE
	/* [TPP_TOK_MINUS_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR
	/* [TPP_TOK_MINUS_RANGLE_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_RANGLE_STAR),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE
	/* [TPP_TOK_MINUS_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE
	/* [TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_MINUS && TPP_HAVE_TPP_TOK_MINUS_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_STAR
	/* [TPP_TOK_DOT_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_DOT_STAR),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT
	/* [TPP_TOK_DOT_DOT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_DOT_DOT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT_DOT
	/* [TPP_TOK_DOT_DOT_DOT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_DOT_DOT_DOT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_DOT && TPP_HAVE_TPP_TOK_DOT_DOT_DOT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH
	/* [TPP_TOK_SLASH_SLASH] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_SLASH_SLASH),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL
	/* [TPP_TOK_SLASH_SLASH_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_SLASH_SLASH_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_EQUAL
	/* [TPP_TOK_SLASH_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_SLASH_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_SLASH && TPP_HAVE_TPP_TOK_SLASH_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_COLON
	/* [TPP_TOK_COLON_COLON] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_COLON_COLON),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_COLON */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_EQUAL
	/* [TPP_TOK_COLON_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_COLON_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_COLON && TPP_HAVE_TPP_TOK_COLON_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS
	/* [TPP_TOK_LANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE
	/* [TPP_TOK_LANGLE_MINUS_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_MINUS_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE
	/* [TPP_TOK_LANGLE_MINUS_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_MINUS_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE
	/* [TPP_TOK_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS
	/* [TPP_TOK_LANGLE_LANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE
	/* [TPP_TOK_LANGLE_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS
	/* [TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL
	/* [TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL
	/* [TPP_TOK_LANGLE_LANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_LANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL
	/* [TPP_TOK_LANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE
	/* [TPP_TOK_LANGLE_EQUAL_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_EQUAL_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE
	/* [TPP_TOK_LANGLE_EQUAL_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_EQUAL_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE
	/* [TPP_TOK_LANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_LANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_LANGLE && TPP_HAVE_TPP_TOK_LANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM
	/* [TPP_TOK_EQUAL_EXCLAIM] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_EXCLAIM),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT
	/* [TPP_TOK_EQUAL_PERCENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_PERCENT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT
	/* [TPP_TOK_EQUAL_PERCENT_PERCENT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_PERCENT_PERCENT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PERCENT_PERCENT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AMP
	/* [TPP_TOK_EQUAL_AMP] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_AMP),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AMP */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR
	/* [TPP_TOK_EQUAL_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_STAR),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR
	/* [TPP_TOK_EQUAL_STAR_STAR] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_STAR_STAR),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_STAR_STAR */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PLUS
	/* [TPP_TOK_EQUAL_PLUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_PLUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PLUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_MINUS
	/* [TPP_TOK_EQUAL_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH
	/* [TPP_TOK_EQUAL_SLASH] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_SLASH),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH
	/* [TPP_TOK_EQUAL_SLASH_SLASH] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_SLASH_SLASH),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_SLASH_SLASH */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_COLON
	/* [TPP_TOK_EQUAL_COLON] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_COLON),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_COLON */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE
	/* [TPP_TOK_EQUAL_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE
	/* [TPP_TOK_EQUAL_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE
	/* [TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_LANGLE_LANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL
	/* [TPP_TOK_EQUAL_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM
	/* [TPP_TOK_EQUAL_EQUAL_EXCLAIM] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_EQUAL_EXCLAIM),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EXCLAIM */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL
	/* [TPP_TOK_EQUAL_EQUAL_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_EQUAL_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_EQUAL_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE
	/* [TPP_TOK_EQUAL_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE
	/* [TPP_TOK_EQUAL_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE
	/* [TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_QMARK
	/* [TPP_TOK_EQUAL_QMARK] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_QMARK),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_QMARK */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT
	/* [TPP_TOK_EQUAL_AT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_AT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT_AT
	/* [TPP_TOK_EQUAL_AT_AT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_AT_AT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_AT_AT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_HAT
	/* [TPP_TOK_EQUAL_HAT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_HAT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_HAT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PIPE
	/* [TPP_TOK_EQUAL_PIPE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_PIPE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_PIPE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_TILDE
	/* [TPP_TOK_EQUAL_TILDE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_EQUAL_TILDE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_EQUAL && TPP_HAVE_TPP_TOK_EQUAL_TILDE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS
	/* [TPP_TOK_RANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE
	/* [TPP_TOK_RANGLE_MINUS_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_MINUS_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE
	/* [TPP_TOK_RANGLE_MINUS_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_MINUS_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_MINUS_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_LANGLE
	/* [TPP_TOK_RANGLE_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL
	/* [TPP_TOK_RANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE
	/* [TPP_TOK_RANGLE_EQUAL_LANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_EQUAL_LANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_LANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE
	/* [TPP_TOK_RANGLE_EQUAL_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_EQUAL_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_EQUAL_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE
	/* [TPP_TOK_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS
	/* [TPP_TOK_RANGLE_RANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL
	/* [TPP_TOK_RANGLE_RANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE
	/* [TPP_TOK_RANGLE_RANGLE_RANGLE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_RANGLE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS
	/* [TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_MINUS */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL
	/* [TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_RANGLE && TPP_HAVE_TPP_TOK_RANGLE_RANGLE_RANGLE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_EQUAL
	/* [TPP_TOK_QMARK_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_QMARK_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_QMARK
	/* [TPP_TOK_QMARK_QMARK] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_QMARK_QMARK),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_QMARK && TPP_HAVE_TPP_TOK_QMARK_QMARK */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_EQUAL
	/* [TPP_TOK_AT_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_AT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT
	/* [TPP_TOK_AT_AT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_AT_AT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT_EQUAL
	/* [TPP_TOK_AT_AT_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_AT_AT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_AT && TPP_HAVE_TPP_TOK_AT_AT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_EQUAL
	/* [TPP_TOK_HAT_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_HAT_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_HAT
	/* [TPP_TOK_HAT_HAT] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_HAT_HAT),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_HAT && TPP_HAVE_TPP_TOK_HAT_HAT */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_EQUAL
	/* [TPP_TOK_PIPE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PIPE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_PIPE
	/* [TPP_TOK_PIPE_PIPE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_PIPE_PIPE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_PIPE && TPP_HAVE_TPP_TOK_PIPE_PIPE */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_EQUAL
	/* [TPP_TOK_TILDE_EQUAL] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_TILDE_EQUAL),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_EQUAL */
#if TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_TILDE
	/* [TPP_TOK_TILDE_TILDE] = */ tpp_offsetof(struct tpp_token_repr_strings_struct, ttr_TPP_TOK_TILDE_TILDE),
#endif /* TPP_HAVE_TPP_TOK_MC_STARTSWITH_TILDE && TPP_HAVE_TPP_TOK_TILDE_TILDE */
};
TPP_STATIC_ASSERT(tpp_lengthof(tpp_token_repr_offsets) == TPP_TOK_MULTICHAR_END);
/*[[[end]]]*/

/* Similar to `tpp_strtokenid()', but returns a (canonical) representation of "id" */
TPP_IMPL TPP_WUNUSED char const *TPPCALL
tpp_reprtokenid(tpp_token_id id) {
	if ((unsigned int)id < (unsigned int)TPP_TOK_MULTICHAR_END)
		return (char const *)&tpp_token_repr_strings + tpp_token_repr_offsets[(unsigned int)id];
	if ((unsigned int)id < (unsigned int)TPP_TOK_USERKEYWORD_BEGIN) {
		tpp_keyword const *kwd = tpp_builtin_getkeyword_byid(id);
		if (kwd)
			return (char const *)kwd->tk_kwd;
	}
	return NULL;
}
#endif /* TPP_HAVE_REPRTOKENID */

TPP_DECL_END
/*[[[tpp-end]]]*/

#endif /* !GUARD_TPP_TOKEN_REPRTOKENID_C */
